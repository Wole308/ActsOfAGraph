#include "mem_access.h"
using namespace std;

#ifdef SW
mem_access::mem_access(mydebug * _mydebugobj){ 
	actsutilityobj = new actsutility(); 
	acts_utilobj = new acts_util(_mydebugobj); 
	mydebugobj = _mydebugobj; 
}
mem_access::~mem_access(){}
#endif

// -------------------- key values -------------------- //
fetchmessage_t acts_all::MEMACCESS_readkeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs, travstate_t travstate, globalparams_t globalparams){
	fetchmessage_t fetchmessage;
	fetchmessage.chunksize_kvs = -1;
	fetchmessage.nextoffset_kvs = -1;
	if(enable == OFF){ return fetchmessage; }
	
	analysis_type analysis_loopcount = BLOCKRAM_SIZE;
	buffer_type chunk_size = UTIL_getchunksize_kvs(size_kvs, travstate, 0);
	
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
		
		buffer[0][i] = UTIL_GETKV(mykeyvalue0);
		buffer[1][i] = UTIL_GETKV(mykeyvalue1);
		buffer[2][i] = UTIL_GETKV(mykeyvalue2);
		buffer[3][i] = UTIL_GETKV(mykeyvalue3);
		buffer[4][i] = UTIL_GETKV(mykeyvalue4);
		buffer[5][i] = UTIL_GETKV(mykeyvalue5);
		buffer[6][i] = UTIL_GETKV(mykeyvalue6);
		buffer[7][i] = UTIL_GETKV(mykeyvalue7);
		
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"readkeyvalues:: keyvalues read: offset_kvs: "<<offset_kvs<<", size_kvs: "<<size_kvs<<", chunk_size: "<<chunk_size<<endl;
	#endif
	return fetchmessage;
}

void acts_all::MEMACCESS_savekeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_buffer_t buffer[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_t * globalcapsule, keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], batch_type globalbaseaddress_kvs, globalparams_t globalparams){				
	if(enable == OFF){ return; }
	analysis_type analysis_destpartitionsz = DESTBLOCKRAM_SIZE / NUM_PARTITIONS;
	
	#ifdef _DEBUGMODE_CHECKS
	actsutilityobj->printkeyvalues("savekeyvalues::localcapsule", localcapsule, MAX_NUM_PARTITIONS);
	actsutilityobj->printvaluecount("savekeyvalues::localcapsule", localcapsule, MAX_NUM_PARTITIONS);
	actsutilityobj->scankeyvalues("savekeyvalues::buffer", (keyvalue_t *)buffer, localcapsule, NUM_PARTITIONS, globalparams.SIZE_BATCHRANGE / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	
	SAVEPARTITIONS_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		batch_type dramoffset_kvs = globalbaseaddress_kvs + ((globalcapsule[p].key + globalcapsule[p].value) / VECTOR_SIZE);
		buffer_type bramoffset_kvs = localcapsule[p].key / VECTOR_SIZE;
		buffer_type realsize_kvs = localcapsule[p].value / VECTOR_SIZE;
		buffer_type size_kvs = UTIL_getpartitionwritesz(realsize_kvs, bramoffset_kvs);
		
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("savekeyvalues 23", bramoffset_kvs + size_kvs, DESTBLOCKRAM_SIZE + 1, p, NAp, NAp);
		actsutilityobj->checkoutofbounds("savekeyvalues 25", ((globalcapsule[p].key + globalcapsule[p].value) / VECTOR_SIZE), globalparams.SIZE_KVDRAM + 1, p, NAp, NAp);
		#endif
		if(realsize_kvs > 0){
			SAVEPARTITIONS_LOOP1B: for(buffer_type i=0; i<size_kvs; i++){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_destpartitionsz avg=analysis_destpartitionsz
			#pragma HLS PIPELINE II=1
				keyvalue_t mykeyvalue0 = UTIL_GETKV(buffer[0][bramoffset_kvs + i]);
				keyvalue_t mykeyvalue1 = UTIL_GETKV(buffer[1][bramoffset_kvs + i]);
				keyvalue_t mykeyvalue2 = UTIL_GETKV(buffer[2][bramoffset_kvs + i]);
				keyvalue_t mykeyvalue3 = UTIL_GETKV(buffer[3][bramoffset_kvs + i]);
				keyvalue_t mykeyvalue4 = UTIL_GETKV(buffer[4][bramoffset_kvs + i]);
				keyvalue_t mykeyvalue5 = UTIL_GETKV(buffer[5][bramoffset_kvs + i]);
				keyvalue_t mykeyvalue6 = UTIL_GETKV(buffer[6][bramoffset_kvs + i]);
				keyvalue_t mykeyvalue7 = UTIL_GETKV(buffer[7][bramoffset_kvs + i]);
				
				#ifdef _WIDEWORD
				kvdram[dramoffset_kvs + i].range(31, 0) = mykeyvalue0.key; 
				kvdram[dramoffset_kvs + i].range(63, 32) = mykeyvalue0.value; 
				kvdram[dramoffset_kvs + i].range(95, 64) = mykeyvalue1.key; 
				kvdram[dramoffset_kvs + i].range(127, 96) = mykeyvalue1.value; 
				kvdram[dramoffset_kvs + i].range(159, 128) = mykeyvalue2.key; 
				kvdram[dramoffset_kvs + i].range(191, 160) = mykeyvalue2.value; 
				kvdram[dramoffset_kvs + i].range(223, 192) = mykeyvalue3.key; 
				kvdram[dramoffset_kvs + i].range(255, 224) = mykeyvalue3.value; 
				kvdram[dramoffset_kvs + i].range(287, 256) = mykeyvalue4.key; 
				kvdram[dramoffset_kvs + i].range(319, 288) = mykeyvalue4.value; 
				kvdram[dramoffset_kvs + i].range(351, 320) = mykeyvalue5.key; 
				kvdram[dramoffset_kvs + i].range(383, 352) = mykeyvalue5.value; 
				kvdram[dramoffset_kvs + i].range(415, 384) = mykeyvalue6.key; 
				kvdram[dramoffset_kvs + i].range(447, 416) = mykeyvalue6.value; 
				kvdram[dramoffset_kvs + i].range(479, 448) = mykeyvalue7.key; 
				kvdram[dramoffset_kvs + i].range(511, 480) = mykeyvalue7.value; 
				#else
				kvdram[dramoffset_kvs + i].data[0].key = mykeyvalue0.key; 
				kvdram[dramoffset_kvs + i].data[0].value = mykeyvalue0.value; 
				kvdram[dramoffset_kvs + i].data[1].key = mykeyvalue1.key; 
				kvdram[dramoffset_kvs + i].data[1].value = mykeyvalue1.value; 
				kvdram[dramoffset_kvs + i].data[2].key = mykeyvalue2.key; 
				kvdram[dramoffset_kvs + i].data[2].value = mykeyvalue2.value; 
				kvdram[dramoffset_kvs + i].data[3].key = mykeyvalue3.key; 
				kvdram[dramoffset_kvs + i].data[3].value = mykeyvalue3.value; 
				kvdram[dramoffset_kvs + i].data[4].key = mykeyvalue4.key; 
				kvdram[dramoffset_kvs + i].data[4].value = mykeyvalue4.value; 
				kvdram[dramoffset_kvs + i].data[5].key = mykeyvalue5.key; 
				kvdram[dramoffset_kvs + i].data[5].value = mykeyvalue5.value; 
				kvdram[dramoffset_kvs + i].data[6].key = mykeyvalue6.key; 
				kvdram[dramoffset_kvs + i].data[6].value = mykeyvalue6.value; 
				kvdram[dramoffset_kvs + i].data[7].key = mykeyvalue7.key; 
				kvdram[dramoffset_kvs + i].data[7].value = mykeyvalue7.value; 
				#endif 
				
				#ifdef _DEBUGMODE_STATS
				actsutilityobj->globalstats_countkvspartitionswritten_actual(VECTOR_SIZE);
				#endif
			}
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countkvspartitionswritten(realsize_kvs * VECTOR_SIZE);
			#endif
		}
	}
	SAVEPARTITIONS_LOOP2: for(partition_type p=0; p<NUM_PARTITIONS; p++){ globalcapsule[p].value += localcapsule[p].value; }
	
	#if defined(ENABLE_PERFECTACCURACY) && defined(_DEBUGMODE_CHECKS2)
	for(unsigned int i=0; i<NUM_PARTITIONS-1; i++){ 
		if(globalcapsule[i].key + globalcapsule[i].value >= globalcapsule[i+1].key && globalcapsule[i].value > 0){ 
			cout<<"savekeyvalues::globalcapsule 33. ERROR. out of bounds. (globalcapsule["<<i<<"].key("<<globalcapsule[i].key<<") + globalcapsule["<<i<<"].value("<<globalcapsule[i].value<<") >= globalcapsule["<<i+1<<"].key("<<globalcapsule[i+1].key<<")) printing and exiting..."<<endl; 
			actsutilityobj->printkeyvalues("savekeyvalues::globalcapsule 34", (keyvalue_t *)globalcapsule, NUM_PARTITIONS); 
			exit(EXIT_FAILURE); 
		}
	}
	if(globalcapsule[NUM_PARTITIONS-1].key + globalcapsule[NUM_PARTITIONS-1].value >= globalparams.SIZE_KVDRAM){
		cout<<"savekeyvalues::globalcapsule 36. ERROR. out of bounds. (globalcapsule["<<NUM_PARTITIONS-1<<"].key("<<globalcapsule[NUM_PARTITIONS-1].key<<") + globalcapsule["<<NUM_PARTITIONS-1<<"].value("<<globalcapsule[NUM_PARTITIONS-1].value<<") >= globalparams.SIZE_KVDRAM("<<globalparams.SIZE_KVDRAM<<")) printing and exiting..."<<endl; 
		actsutilityobj->printkeyvalues("savekeyvalues::globalcapsule 37", (keyvalue_t *)globalcapsule, NUM_PARTITIONS); 
		exit(EXIT_FAILURE); 
	}
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS2
	cout<<"savekeyvalues:: keyvalues saved: offset_kvs from: "<<globalbaseaddress_kvs + ((globalcapsule[0].key + globalcapsule[0].value) / VECTOR_SIZE)<<endl;
	actsutilityobj->printkeyvalues("actsutility::savekeyvalues: globalcapsule.", (keyvalue_t *)globalcapsule, NUM_PARTITIONS);
	#endif
	return;
}

void acts_all::MEMACCESS_readkeyvalues(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_buffer_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount = BLOCKRAM_SIZE;
		
	READKEYVALUES2_LOOP: for (buffer_type i=0; i<size_kvs; i++){
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
		
		buffer[0][bufferoffset_kvs + i] = UTIL_GETKV(mykeyvalue0);
		buffer[1][bufferoffset_kvs + i] = UTIL_GETKV(mykeyvalue1);
		buffer[2][bufferoffset_kvs + i] = UTIL_GETKV(mykeyvalue2);
		buffer[3][bufferoffset_kvs + i] = UTIL_GETKV(mykeyvalue3);
		buffer[4][bufferoffset_kvs + i] = UTIL_GETKV(mykeyvalue4);
		buffer[5][bufferoffset_kvs + i] = UTIL_GETKV(mykeyvalue5);
		buffer[6][bufferoffset_kvs + i] = UTIL_GETKV(mykeyvalue6);
		buffer[7][bufferoffset_kvs + i] = UTIL_GETKV(mykeyvalue7);
		
		#endif 
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<< TIMINGRESULTSCOLOR<<"readkeyvalues:: vertices read: offset: "<<dramoffset_kvs * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl;
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(dramoffset_kvs >= globalparams.BASEOFFSETKVS_VERTEXPTR_ && dramoffset_kvs < globalparams.BASEOFFSETKVS_VERTEXPTR + VERTEXPTRSSZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"readkeyvalues:: vertices read: offset: "<<(dramoffset_kvs - globalparams.BASEOFFSETKVS_VERTEXPTR) * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs - globalparams.BASEOFFSETKVS_VERTEXPTR) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	if(dramoffset_kvs >= globalparams.BASEOFFSETKVS_VERTICESDATA_ && dramoffset_kvs < globalparams.BASEOFFSETKVS_DESTVERTICESDATA + VERTICESDATASZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"readkeyvalues:: vertices read: offset: "<<(dramoffset_kvs - globalparams.BASEOFFSETKVS_DESTVERTICESDATA) * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs - globalparams.BASEOFFSETKVS_DESTVERTICESDATA) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	#endif
	return;
}

void acts_all::MEMACCESS_savekeyvalues(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_buffer_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount =  BLOCKRAM_SIZE;
	
	SAVEKEYVALUES2_LOOP: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE II=1
		keyvalue_t mykeyvalue0 = UTIL_GETKV(buffer[0][bufferoffset_kvs + i]);
		keyvalue_t mykeyvalue1 = UTIL_GETKV(buffer[1][bufferoffset_kvs + i]);
		keyvalue_t mykeyvalue2 = UTIL_GETKV(buffer[2][bufferoffset_kvs + i]);
		keyvalue_t mykeyvalue3 = UTIL_GETKV(buffer[3][bufferoffset_kvs + i]);
		keyvalue_t mykeyvalue4 = UTIL_GETKV(buffer[4][bufferoffset_kvs + i]);
		keyvalue_t mykeyvalue5 = UTIL_GETKV(buffer[5][bufferoffset_kvs + i]);
		keyvalue_t mykeyvalue6 = UTIL_GETKV(buffer[6][bufferoffset_kvs + i]);
		keyvalue_t mykeyvalue7 = UTIL_GETKV(buffer[7][bufferoffset_kvs + i]);
	
		#ifdef _WIDEWORD
		kvdram[dramoffset_kvs + i].range(31, 0) = mykeyvalue0.key; 
		kvdram[dramoffset_kvs + i].range(63, 32) = mykeyvalue0.value; 
		kvdram[dramoffset_kvs + i].range(95, 64) = mykeyvalue1.key; 
		kvdram[dramoffset_kvs + i].range(127, 96) = mykeyvalue1.value; 
		kvdram[dramoffset_kvs + i].range(159, 128) = mykeyvalue2.key; 
		kvdram[dramoffset_kvs + i].range(191, 160) = mykeyvalue2.value; 
		kvdram[dramoffset_kvs + i].range(223, 192) = mykeyvalue3.key; 
		kvdram[dramoffset_kvs + i].range(255, 224) = mykeyvalue3.value; 
		kvdram[dramoffset_kvs + i].range(287, 256) = mykeyvalue4.key; 
		kvdram[dramoffset_kvs + i].range(319, 288) = mykeyvalue4.value; 
		kvdram[dramoffset_kvs + i].range(351, 320) = mykeyvalue5.key; 
		kvdram[dramoffset_kvs + i].range(383, 352) = mykeyvalue5.value; 
		kvdram[dramoffset_kvs + i].range(415, 384) = mykeyvalue6.key; 
		kvdram[dramoffset_kvs + i].range(447, 416) = mykeyvalue6.value; 
		kvdram[dramoffset_kvs + i].range(479, 448) = mykeyvalue7.key; 
		kvdram[dramoffset_kvs + i].range(511, 480) = mykeyvalue7.value; 
		#else 
		kvdram[dramoffset_kvs + i].data[0].key = mykeyvalue0.key;
		kvdram[dramoffset_kvs + i].data[0].value = mykeyvalue0.value;
		kvdram[dramoffset_kvs + i].data[1].key = mykeyvalue1.key;
		kvdram[dramoffset_kvs + i].data[1].value = mykeyvalue1.value;
		kvdram[dramoffset_kvs + i].data[2].key = mykeyvalue2.key;
		kvdram[dramoffset_kvs + i].data[2].value = mykeyvalue2.value;
		kvdram[dramoffset_kvs + i].data[3].key = mykeyvalue3.key;
		kvdram[dramoffset_kvs + i].data[3].value = mykeyvalue3.value;
		kvdram[dramoffset_kvs + i].data[4].key = mykeyvalue4.key;
		kvdram[dramoffset_kvs + i].data[4].value = mykeyvalue4.value;
		kvdram[dramoffset_kvs + i].data[5].key = mykeyvalue5.key;
		kvdram[dramoffset_kvs + i].data[5].value = mykeyvalue5.value;
		kvdram[dramoffset_kvs + i].data[6].key = mykeyvalue6.key;
		kvdram[dramoffset_kvs + i].data[6].value = mykeyvalue6.value;
		kvdram[dramoffset_kvs + i].data[7].key = mykeyvalue7.key;
		kvdram[dramoffset_kvs + i].data[7].value = mykeyvalue7.value;
		#endif 
		
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvswritten(VECTOR_SIZE);
		#endif
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"savekeyvalues:: vertices saved: offset: "<<dramoffset_kvs * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs) * VECTOR_SIZE<<", number of vertex datas written: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<<endl;
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS2
	if(dramoffset_kvs >= globalparams.BASEOFFSETKVS_VERTEXPTR && dramoffset_kvs < globalparams.BASEOFFSETKVS_VERTEXPTR + VERTEXPTRSSZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"savekeyvalues:: vertices saved: offset: "<<dramoffset_kvs-globalparams.BASEOFFSETKVS_VERTEXPTR * VECTOR_SIZE<<"-"<<((dramoffset_kvs + size_kvs)-globalparams.BASEOFFSETKVS_VERTEXPTR) * VECTOR_SIZE<<", number of vertex datas written: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	if(dramoffset_kvs >= globalparams.BASEOFFSETKVS_DESTVERTICESDATA && dramoffset_kvs < globalparams.BASEOFFSETKVS_DESTVERTICESDATA + VERTICESDATASZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"savekeyvalues:: vertices saved: offset: "<<(dramoffset_kvs-globalparams.BASEOFFSETKVS_DESTVERTICESDATA) * VECTOR_SIZE<<"-"<<((dramoffset_kvs-globalparams.BASEOFFSETKVS_DESTVERTICESDATA) + size_kvs) * VECTOR_SIZE<<", number of vertex datas written: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }					
	#endif
	return;
}

// -------------------- vdata -------------------- //
void acts_all::MEMACCESS_readvdata(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams){
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
		
		buffer[begincol + 0][bufferoffset_kvs + i] = UTIL_GETKV2(mykeyvalue0);
		buffer[begincol + 1][bufferoffset_kvs + i] = UTIL_GETKV2(mykeyvalue1);
		buffer[begincol + 2][bufferoffset_kvs + i] = UTIL_GETKV2(mykeyvalue2);
		buffer[begincol + 3][bufferoffset_kvs + i] = UTIL_GETKV2(mykeyvalue3);
		buffer[begincol + 4][bufferoffset_kvs + i] = UTIL_GETKV2(mykeyvalue4);
		buffer[begincol + 5][bufferoffset_kvs + i] = UTIL_GETKV2(mykeyvalue5);
		buffer[begincol + 6][bufferoffset_kvs + i] = UTIL_GETKV2(mykeyvalue6);
		buffer[begincol + 7][bufferoffset_kvs + i] = UTIL_GETKV2(mykeyvalue7);
		
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

void acts_all::MEMACCESS_savevdata(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount =  REDUCESZ / 2;
	
	SAVEVDATA_LOOP: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE II=1
		keyvalue_t mykeyvalue0 = UTIL_GETKV2(buffer[begincol + 0][bufferoffset_kvs + i]);
		keyvalue_t mykeyvalue1 = UTIL_GETKV2(buffer[begincol + 1][bufferoffset_kvs + i]);
		keyvalue_t mykeyvalue2 = UTIL_GETKV2(buffer[begincol + 2][bufferoffset_kvs + i]);
		keyvalue_t mykeyvalue3 = UTIL_GETKV2(buffer[begincol + 3][bufferoffset_kvs + i]);
		keyvalue_t mykeyvalue4 = UTIL_GETKV2(buffer[begincol + 4][bufferoffset_kvs + i]);
		keyvalue_t mykeyvalue5 = UTIL_GETKV2(buffer[begincol + 5][bufferoffset_kvs + i]);
		keyvalue_t mykeyvalue6 = UTIL_GETKV2(buffer[begincol + 6][bufferoffset_kvs + i]);
		keyvalue_t mykeyvalue7 = UTIL_GETKV2(buffer[begincol + 7][bufferoffset_kvs + i]);
	
		#ifdef _WIDEWORD
		kvdram[dramoffset_kvs + i].range(31, 0) = mykeyvalue0.key; 
		kvdram[dramoffset_kvs + i].range(63, 32) = mykeyvalue0.value; 
		kvdram[dramoffset_kvs + i].range(95, 64) = mykeyvalue1.key; 
		kvdram[dramoffset_kvs + i].range(127, 96) = mykeyvalue1.value; 
		kvdram[dramoffset_kvs + i].range(159, 128) = mykeyvalue2.key; 
		kvdram[dramoffset_kvs + i].range(191, 160) = mykeyvalue2.value; 
		kvdram[dramoffset_kvs + i].range(223, 192) = mykeyvalue3.key; 
		kvdram[dramoffset_kvs + i].range(255, 224) = mykeyvalue3.value; 
		kvdram[dramoffset_kvs + i].range(287, 256) = mykeyvalue4.key; 
		kvdram[dramoffset_kvs + i].range(319, 288) = mykeyvalue4.value; 
		kvdram[dramoffset_kvs + i].range(351, 320) = mykeyvalue5.key; 
		kvdram[dramoffset_kvs + i].range(383, 352) = mykeyvalue5.value; 
		kvdram[dramoffset_kvs + i].range(415, 384) = mykeyvalue6.key; 
		kvdram[dramoffset_kvs + i].range(447, 416) = mykeyvalue6.value; 
		kvdram[dramoffset_kvs + i].range(479, 448) = mykeyvalue7.key; 
		kvdram[dramoffset_kvs + i].range(511, 480) = mykeyvalue7.value; 
		#else 
		kvdram[dramoffset_kvs + i].data[0].key = mykeyvalue0.key;
		kvdram[dramoffset_kvs + i].data[0].value = mykeyvalue0.value;
		kvdram[dramoffset_kvs + i].data[1].key = mykeyvalue1.key;
		kvdram[dramoffset_kvs + i].data[1].value = mykeyvalue1.value;
		kvdram[dramoffset_kvs + i].data[2].key = mykeyvalue2.key;
		kvdram[dramoffset_kvs + i].data[2].value = mykeyvalue2.value;
		kvdram[dramoffset_kvs + i].data[3].key = mykeyvalue3.key;
		kvdram[dramoffset_kvs + i].data[3].value = mykeyvalue3.value;
		kvdram[dramoffset_kvs + i].data[4].key = mykeyvalue4.key;
		kvdram[dramoffset_kvs + i].data[4].value = mykeyvalue4.value;
		kvdram[dramoffset_kvs + i].data[5].key = mykeyvalue5.key;
		kvdram[dramoffset_kvs + i].data[5].value = mykeyvalue5.value;
		kvdram[dramoffset_kvs + i].data[6].key = mykeyvalue6.key;
		kvdram[dramoffset_kvs + i].data[6].value = mykeyvalue6.value;
		kvdram[dramoffset_kvs + i].data[7].key = mykeyvalue7.key;
		kvdram[dramoffset_kvs + i].data[7].value = mykeyvalue7.value;
		#endif 
	
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvswritten(VECTOR_SIZE);
		#endif
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"savevdata:: vertices saved: offset: "<<dramoffset_kvs * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs) * VECTOR_SIZE<<", number of vertex datas written: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<<endl;
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS2
	if(dramoffset_kvs >= globalparams.BASEOFFSETKVS_VERTEXPTR && dramoffset_kvs < globalparams.BASEOFFSETKVS_VERTEXPTR + VERTEXPTRSSZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"savekeyvalues:: vertices saved: offset: "<<dramoffset_kvs-globalparams.BASEOFFSETKVS_VERTEXPTR * VECTOR_SIZE<<"-"<<((dramoffset_kvs + size_kvs)-globalparams.BASEOFFSETKVS_VERTEXPTR) * VECTOR_SIZE<<", number of vertex datas written: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	if(dramoffset_kvs >= globalparams.BASEOFFSETKVS_DESTVERTICESDATA && dramoffset_kvs < globalparams.BASEOFFSETKVS_DESTVERTICESDATA + VERTICESDATASZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"savekeyvalues:: vertices saved: offset: "<<(dramoffset_kvs-globalparams.BASEOFFSETKVS_DESTVERTICESDATA) * VECTOR_SIZE<<"-"<<((dramoffset_kvs-globalparams.BASEOFFSETKVS_DESTVERTICESDATA) + size_kvs) * VECTOR_SIZE<<", number of vertex datas written: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }					
	#endif
	return;
}

// -------------------- vmasks -------------------- //
void acts_all::MEMACCESS_readvmasks(bool_type enable, uint512_dt * kvdram, unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type baseoffset_kvs, batch_type lbaseoffset_kvs, batch_type offset_kvs, buffer_type bufferoffset, buffer_type size_kvs){
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
		actsutilityobj->checkoutofbounds("MEMACCESS_readvmasks:", rlbaseoffset_kvs + roffset_kvs, NUMPROCESSEDGESPARTITIONS * VMASKBUFFERSZ_KVS, baseoffset_kvs, rlbaseoffset_kvs, roffset_kvs);
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
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		vmaskBITS[0][bufferoffset + i] = bitsbuffer[i/2].range(0, 0);
		vmaskBITS[1][bufferoffset + i] = bitsbuffer[i/2].range(1, 1);
		vmaskBITS[2][bufferoffset + i] = bitsbuffer[i/2].range(2, 2);
		vmaskBITS[3][bufferoffset + i] = bitsbuffer[i/2].range(3, 3);
		vmaskBITS[4][bufferoffset + i] = bitsbuffer[i/2].range(4, 4);
		vmaskBITS[5][bufferoffset + i] = bitsbuffer[i/2].range(5, 5);
		vmaskBITS[6][bufferoffset + i] = bitsbuffer[i/2].range(6, 6);
		vmaskBITS[7][bufferoffset + i] = bitsbuffer[i/2].range(7, 7);
		vmaskBITS[8][bufferoffset + i] = bitsbuffer[i/2].range(8, 8);
		vmaskBITS[9][bufferoffset + i] = bitsbuffer[i/2].range(9, 9);
		vmaskBITS[10][bufferoffset + i] = bitsbuffer[i/2].range(10, 10);
		vmaskBITS[11][bufferoffset + i] = bitsbuffer[i/2].range(11, 11);
		vmaskBITS[12][bufferoffset + i] = bitsbuffer[i/2].range(12, 12);
		vmaskBITS[13][bufferoffset + i] = bitsbuffer[i/2].range(13, 13);
		vmaskBITS[14][bufferoffset + i] = bitsbuffer[i/2].range(14, 14);
		vmaskBITS[15][bufferoffset + i] = bitsbuffer[i/2].range(15, 15);
		vmaskBITS[0][bufferoffset + i + 1] = bitsbuffer[i/2].range(16, 16);
		vmaskBITS[1][bufferoffset + i + 1] = bitsbuffer[i/2].range(17, 17);
		vmaskBITS[2][bufferoffset + i + 1] = bitsbuffer[i/2].range(18, 18);
		vmaskBITS[3][bufferoffset + i + 1] = bitsbuffer[i/2].range(19, 19);
		vmaskBITS[4][bufferoffset + i + 1] = bitsbuffer[i/2].range(20, 20);
		vmaskBITS[5][bufferoffset + i + 1] = bitsbuffer[i/2].range(21, 21);
		vmaskBITS[6][bufferoffset + i + 1] = bitsbuffer[i/2].range(22, 22);
		vmaskBITS[7][bufferoffset + i + 1] = bitsbuffer[i/2].range(23, 23);
		vmaskBITS[8][bufferoffset + i + 1] = bitsbuffer[i/2].range(24, 24);
		vmaskBITS[9][bufferoffset + i + 1] = bitsbuffer[i/2].range(25, 25);
		vmaskBITS[10][bufferoffset + i + 1] = bitsbuffer[i/2].range(26, 26);
		vmaskBITS[11][bufferoffset + i + 1] = bitsbuffer[i/2].range(27, 27);
		vmaskBITS[12][bufferoffset + i + 1] = bitsbuffer[i/2].range(28, 28);
		vmaskBITS[13][bufferoffset + i + 1] = bitsbuffer[i/2].range(29, 29);
		vmaskBITS[14][bufferoffset + i + 1] = bitsbuffer[i/2].range(30, 30);
		vmaskBITS[15][bufferoffset + i + 1] = bitsbuffer[i/2].range(31, 31);
		#else 
		vmaskBITS[0][bufferoffset + i] = UTIL_READFROM_UINT(bitsbuffer[i/2], 0, 1);
		vmaskBITS[1][bufferoffset + i] = UTIL_READFROM_UINT(bitsbuffer[i/2], 1, 1);
		vmaskBITS[2][bufferoffset + i] = UTIL_READFROM_UINT(bitsbuffer[i/2], 2, 1);
		vmaskBITS[3][bufferoffset + i] = UTIL_READFROM_UINT(bitsbuffer[i/2], 3, 1);
		vmaskBITS[4][bufferoffset + i] = UTIL_READFROM_UINT(bitsbuffer[i/2], 4, 1);
		vmaskBITS[5][bufferoffset + i] = UTIL_READFROM_UINT(bitsbuffer[i/2], 5, 1);
		vmaskBITS[6][bufferoffset + i] = UTIL_READFROM_UINT(bitsbuffer[i/2], 6, 1);
		vmaskBITS[7][bufferoffset + i] = UTIL_READFROM_UINT(bitsbuffer[i/2], 7, 1);
		vmaskBITS[8][bufferoffset + i] = UTIL_READFROM_UINT(bitsbuffer[i/2], 8, 1);
		vmaskBITS[9][bufferoffset + i] = UTIL_READFROM_UINT(bitsbuffer[i/2], 9, 1);
		vmaskBITS[10][bufferoffset + i] = UTIL_READFROM_UINT(bitsbuffer[i/2], 10, 1);
		vmaskBITS[11][bufferoffset + i] = UTIL_READFROM_UINT(bitsbuffer[i/2], 11, 1);
		vmaskBITS[12][bufferoffset + i] = UTIL_READFROM_UINT(bitsbuffer[i/2], 12, 1);
		vmaskBITS[13][bufferoffset + i] = UTIL_READFROM_UINT(bitsbuffer[i/2], 13, 1);
		vmaskBITS[14][bufferoffset + i] = UTIL_READFROM_UINT(bitsbuffer[i/2], 14, 1);
		vmaskBITS[15][bufferoffset + i] = UTIL_READFROM_UINT(bitsbuffer[i/2], 15, 1);
		vmaskBITS[0][bufferoffset + i + 1] = UTIL_READFROM_UINT(bitsbuffer[i/2], 16, 1);
		vmaskBITS[1][bufferoffset + i + 1] = UTIL_READFROM_UINT(bitsbuffer[i/2], 17, 1);
		vmaskBITS[2][bufferoffset + i + 1] = UTIL_READFROM_UINT(bitsbuffer[i/2], 18, 1);
		vmaskBITS[3][bufferoffset + i + 1] = UTIL_READFROM_UINT(bitsbuffer[i/2], 19, 1);
		vmaskBITS[4][bufferoffset + i + 1] = UTIL_READFROM_UINT(bitsbuffer[i/2], 20, 1);
		vmaskBITS[5][bufferoffset + i + 1] = UTIL_READFROM_UINT(bitsbuffer[i/2], 21, 1);
		vmaskBITS[6][bufferoffset + i + 1] = UTIL_READFROM_UINT(bitsbuffer[i/2], 22, 1);
		vmaskBITS[7][bufferoffset + i + 1] = UTIL_READFROM_UINT(bitsbuffer[i/2], 23, 1);
		vmaskBITS[8][bufferoffset + i + 1] = UTIL_READFROM_UINT(bitsbuffer[i/2], 24, 1);
		vmaskBITS[9][bufferoffset + i + 1] = UTIL_READFROM_UINT(bitsbuffer[i/2], 25, 1);
		vmaskBITS[10][bufferoffset + i + 1] = UTIL_READFROM_UINT(bitsbuffer[i/2], 26, 1);
		vmaskBITS[11][bufferoffset + i + 1] = UTIL_READFROM_UINT(bitsbuffer[i/2], 27, 1);
		vmaskBITS[12][bufferoffset + i + 1] = UTIL_READFROM_UINT(bitsbuffer[i/2], 28, 1);
		vmaskBITS[13][bufferoffset + i + 1] = UTIL_READFROM_UINT(bitsbuffer[i/2], 29, 1);
		vmaskBITS[14][bufferoffset + i + 1] = UTIL_READFROM_UINT(bitsbuffer[i/2], 30, 1);
		vmaskBITS[15][bufferoffset + i + 1] = UTIL_READFROM_UINT(bitsbuffer[i/2], 31, 1);
		#endif
	}
	return;
}

void acts_all::MEMACCESS_savemasks(bool_type enable, uint512_dt * kvdram, unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs, unsigned int vmaskp_offset_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MEMACCESS_savemasks:: saving vmask saved: offset_kvs: "<<offset_kvs<<", vmaskp_offset_kvs: "<<vmaskp_offset_kvs<<""<<endl;
	#endif
	
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
		// cout<<"MEMACCESS_savemasks:: maskA: "<<maskA<<", maskB: "<<maskB<<", cummvmask_sp: "<<cummvmask_sp<<endl;
		
		realvid = UTIL_GETREALVID(((k/2) * 32) + 32, globalparams.ACTSPARAMS_INSTID);
		#ifdef _DEBUGMODE_KERNELPRINTS
		if(realvid >= next_chpt){ cout<<"--- MEMACCESS_savemasks:: next checkpoint seen @ k: "<<k<<", realvid["<<k/2<<"]: "<<realvid<<", vpmasks_i: "<<vpmasks_i<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		#endif 
		if(realvid >= next_chpt){ vpmasks[vpmasks_i] = cummvmask_sp; cummvmask_sp = 0; vpmasks_i += 1; next_chpt += processpartitionsz; } // CRITICAL FIXME. TOO EXPENSIVE. CRITICAL FIXME.
	}
	#ifdef _DEBUGMODE_KERNELPRINTS2
	cout<<"--- MEMACCESS_savemasks:: FINISHED. realvid: "<<realvid<<", vpmasks_i: "<<vpmasks_i<<endl;
	#endif 
	#ifdef _DEBUGMODE_CHECKS3
	if(vpmasks_i != NUM_PEs){ cout<<"--- MEMACCESS_savemasks:: ERROR: vpmasks_i("<<vpmasks_i<<") != NUM_PEs("<<NUM_PEs<<"). EXITING...."<<endl; exit(EXIT_FAILURE); }
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

void acts_all::MEMACCESS_readmanyvmasks1(bool_type enable, uint512_dt * kvdram, unit1_type vmaskBITS0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount1 = BLOCKRAM_SIZE;
	analysis_type analysis_loopcount2 = BLOCKRAM_SIZE / 16;
	analysis_type analysis_loopcount3 = BLOCKRAM_SIZE;
	
	buffer_type transfsize = size_kvs * 16;
	uint32_type bitsbuffer[MAXREDUCEBUFFERSZ];

	READVMASKS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount1 avg=analysis_loopcount1
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		tempbuffer[0][i] = kvdram[offset_kvs + i].range(31, 0); 
		tempbuffer[1][i] = kvdram[offset_kvs + i].range(63, 32); 
		tempbuffer[2][i] = kvdram[offset_kvs + i].range(95, 64); 
		tempbuffer[3][i] = kvdram[offset_kvs + i].range(127, 96); 
		tempbuffer[4][i] = kvdram[offset_kvs + i].range(159, 128); 
		tempbuffer[5][i] = kvdram[offset_kvs + i].range(191, 160); 
		tempbuffer[6][i] = kvdram[offset_kvs + i].range(223, 192); 
		tempbuffer[7][i] = kvdram[offset_kvs + i].range(255, 224); 
		tempbuffer[8][i] = kvdram[offset_kvs + i].range(287, 256); 
		tempbuffer[9][i] = kvdram[offset_kvs + i].range(319, 288); 
		tempbuffer[10][i] = kvdram[offset_kvs + i].range(351, 320); 
		tempbuffer[11][i] = kvdram[offset_kvs + i].range(383, 352); 
		tempbuffer[12][i] = kvdram[offset_kvs + i].range(415, 384); 
		tempbuffer[13][i] = kvdram[offset_kvs + i].range(447, 416); 
		tempbuffer[14][i] = kvdram[offset_kvs + i].range(479, 448); 
		tempbuffer[15][i] = kvdram[offset_kvs + i].range(511, 480); 
		#else 
		tempbuffer[0][i].key = kvdram[offset_kvs + i].data[0].key;
		tempbuffer[0][i].value = kvdram[offset_kvs + i].data[0].value; 
		tempbuffer[1][i].key = kvdram[offset_kvs + i].data[1].key;
		tempbuffer[1][i].value = kvdram[offset_kvs + i].data[1].value; 
		tempbuffer[2][i].key = kvdram[offset_kvs + i].data[2].key;
		tempbuffer[2][i].value = kvdram[offset_kvs + i].data[2].value; 
		tempbuffer[3][i].key = kvdram[offset_kvs + i].data[3].key;
		tempbuffer[3][i].value = kvdram[offset_kvs + i].data[3].value; 
		tempbuffer[4][i].key = kvdram[offset_kvs + i].data[4].key;
		tempbuffer[4][i].value = kvdram[offset_kvs + i].data[4].value; 
		tempbuffer[5][i].key = kvdram[offset_kvs + i].data[5].key;
		tempbuffer[5][i].value = kvdram[offset_kvs + i].data[5].value; 
		tempbuffer[6][i].key = kvdram[offset_kvs + i].data[6].key;
		tempbuffer[6][i].value = kvdram[offset_kvs + i].data[6].value; 
		tempbuffer[7][i].key = kvdram[offset_kvs + i].data[7].key;
		tempbuffer[7][i].value = kvdram[offset_kvs + i].data[7].value; 
		
		#endif 
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	
	uint32_type D0;
	uint32_type D1;
	uint32_type D2;
	uint32_type D3;
	uint32_type D4;
	uint32_type D5;
	uint32_type D6;
	uint32_type D7;
	uint32_type D8;
	uint32_type D9;
	uint32_type D10;
	uint32_type D11;
	uint32_type D12;
	uint32_type D13;
	uint32_type D14;
	uint32_type D15;
	buffer_type index = 0;
	LOADVMASKS_LOOP1: for (buffer_type i=0; i<size_kvs; i++){
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
		bitsbuffer[index + 1] = tempbuffer[0][i].value;
		bitsbuffer[index + 2] = tempbuffer[1][i].key;
		bitsbuffer[index + 3] = tempbuffer[1][i].value;
		bitsbuffer[index + 4] = tempbuffer[2][i].key;
		bitsbuffer[index + 5] = tempbuffer[2][i].value;
		bitsbuffer[index + 6] = tempbuffer[3][i].key;
		bitsbuffer[index + 7] = tempbuffer[3][i].value;
		bitsbuffer[index + 8] = tempbuffer[4][i].key;
		bitsbuffer[index + 9] = tempbuffer[4][i].value;
		bitsbuffer[index + 10] = tempbuffer[5][i].key;
		bitsbuffer[index + 11] = tempbuffer[5][i].value;
		bitsbuffer[index + 12] = tempbuffer[6][i].key;
		bitsbuffer[index + 13] = tempbuffer[6][i].value;
		bitsbuffer[index + 14] = tempbuffer[7][i].key;
		bitsbuffer[index + 15] = tempbuffer[7][i].value;
		#endif
		
		index += VECTOR_SIZE * 2;
	}

	// u32 => u16bits	
	buffer_type index2 = 0;
	LOADVMASKS_LOOP2: for (buffer_type i=0; i<transfsize; i++){ // transfsize, reducebuffersz
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount3 avg=analysis_loopcount3
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		vmaskBITS0[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS0[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS0[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS0[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS0[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS0[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS0[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS0[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS0[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS0[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS0[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS0[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS0[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS0[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS0[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS0[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS0[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS0[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS0[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS0[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS0[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS0[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS0[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS0[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS0[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS0[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS0[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS0[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS0[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS0[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS0[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS0[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS0[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS0[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS0[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS0[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS0[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS0[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS0[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS0[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS0[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS0[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS0[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS0[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS0[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS0[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS0[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS0[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS0[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS0[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS0[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS0[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS0[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS0[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS0[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS0[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS0[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS0[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS0[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS0[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS0[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS0[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS0[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS0[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		
		index2 += 2;
	}
	return;
}
void acts_all::MEMACCESS_readmanyvmasks2(bool_type enable, uint512_dt * kvdram, unit1_type vmaskBITS0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS1[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount1 = BLOCKRAM_SIZE;
	analysis_type analysis_loopcount2 = BLOCKRAM_SIZE / 16;
	analysis_type analysis_loopcount3 = BLOCKRAM_SIZE;
	
	buffer_type transfsize = size_kvs * 16;
	uint32_type bitsbuffer[MAXREDUCEBUFFERSZ];

	READVMASKS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount1 avg=analysis_loopcount1
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		tempbuffer[0][i] = kvdram[offset_kvs + i].range(31, 0); 
		tempbuffer[1][i] = kvdram[offset_kvs + i].range(63, 32); 
		tempbuffer[2][i] = kvdram[offset_kvs + i].range(95, 64); 
		tempbuffer[3][i] = kvdram[offset_kvs + i].range(127, 96); 
		tempbuffer[4][i] = kvdram[offset_kvs + i].range(159, 128); 
		tempbuffer[5][i] = kvdram[offset_kvs + i].range(191, 160); 
		tempbuffer[6][i] = kvdram[offset_kvs + i].range(223, 192); 
		tempbuffer[7][i] = kvdram[offset_kvs + i].range(255, 224); 
		tempbuffer[8][i] = kvdram[offset_kvs + i].range(287, 256); 
		tempbuffer[9][i] = kvdram[offset_kvs + i].range(319, 288); 
		tempbuffer[10][i] = kvdram[offset_kvs + i].range(351, 320); 
		tempbuffer[11][i] = kvdram[offset_kvs + i].range(383, 352); 
		tempbuffer[12][i] = kvdram[offset_kvs + i].range(415, 384); 
		tempbuffer[13][i] = kvdram[offset_kvs + i].range(447, 416); 
		tempbuffer[14][i] = kvdram[offset_kvs + i].range(479, 448); 
		tempbuffer[15][i] = kvdram[offset_kvs + i].range(511, 480); 
		#else 
		tempbuffer[0][i].key = kvdram[offset_kvs + i].data[0].key;
		tempbuffer[0][i].value = kvdram[offset_kvs + i].data[0].value; 
		tempbuffer[1][i].key = kvdram[offset_kvs + i].data[1].key;
		tempbuffer[1][i].value = kvdram[offset_kvs + i].data[1].value; 
		tempbuffer[2][i].key = kvdram[offset_kvs + i].data[2].key;
		tempbuffer[2][i].value = kvdram[offset_kvs + i].data[2].value; 
		tempbuffer[3][i].key = kvdram[offset_kvs + i].data[3].key;
		tempbuffer[3][i].value = kvdram[offset_kvs + i].data[3].value; 
		tempbuffer[4][i].key = kvdram[offset_kvs + i].data[4].key;
		tempbuffer[4][i].value = kvdram[offset_kvs + i].data[4].value; 
		tempbuffer[5][i].key = kvdram[offset_kvs + i].data[5].key;
		tempbuffer[5][i].value = kvdram[offset_kvs + i].data[5].value; 
		tempbuffer[6][i].key = kvdram[offset_kvs + i].data[6].key;
		tempbuffer[6][i].value = kvdram[offset_kvs + i].data[6].value; 
		tempbuffer[7][i].key = kvdram[offset_kvs + i].data[7].key;
		tempbuffer[7][i].value = kvdram[offset_kvs + i].data[7].value; 
		
		#endif 
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	
	uint32_type D0;
	uint32_type D1;
	uint32_type D2;
	uint32_type D3;
	uint32_type D4;
	uint32_type D5;
	uint32_type D6;
	uint32_type D7;
	uint32_type D8;
	uint32_type D9;
	uint32_type D10;
	uint32_type D11;
	uint32_type D12;
	uint32_type D13;
	uint32_type D14;
	uint32_type D15;
	buffer_type index = 0;
	LOADVMASKS_LOOP1: for (buffer_type i=0; i<size_kvs; i++){
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
		bitsbuffer[index + 1] = tempbuffer[0][i].value;
		bitsbuffer[index + 2] = tempbuffer[1][i].key;
		bitsbuffer[index + 3] = tempbuffer[1][i].value;
		bitsbuffer[index + 4] = tempbuffer[2][i].key;
		bitsbuffer[index + 5] = tempbuffer[2][i].value;
		bitsbuffer[index + 6] = tempbuffer[3][i].key;
		bitsbuffer[index + 7] = tempbuffer[3][i].value;
		bitsbuffer[index + 8] = tempbuffer[4][i].key;
		bitsbuffer[index + 9] = tempbuffer[4][i].value;
		bitsbuffer[index + 10] = tempbuffer[5][i].key;
		bitsbuffer[index + 11] = tempbuffer[5][i].value;
		bitsbuffer[index + 12] = tempbuffer[6][i].key;
		bitsbuffer[index + 13] = tempbuffer[6][i].value;
		bitsbuffer[index + 14] = tempbuffer[7][i].key;
		bitsbuffer[index + 15] = tempbuffer[7][i].value;
		#endif
		
		index += VECTOR_SIZE * 2;
	}

	// u32 => u16bits	
	buffer_type index2 = 0;
	LOADVMASKS_LOOP2: for (buffer_type i=0; i<transfsize; i++){ // transfsize, reducebuffersz
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount3 avg=analysis_loopcount3
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		vmaskBITS0[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS0[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS0[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS0[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS0[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS0[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS0[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS0[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS0[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS0[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS0[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS0[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS0[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS0[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS0[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS0[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS0[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS0[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS0[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS0[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS0[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS0[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS0[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS0[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS0[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS0[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS0[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS0[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS0[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS0[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS0[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS0[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS0[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS0[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS0[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS0[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS0[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS0[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS0[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS0[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS0[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS0[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS0[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS0[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS0[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS0[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS0[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS0[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS0[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS0[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS0[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS0[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS0[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS0[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS0[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS0[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS0[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS0[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS0[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS0[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS0[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS0[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS0[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS0[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS1[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS1[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS1[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS1[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS1[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS1[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS1[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS1[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS1[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS1[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS1[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS1[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS1[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS1[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS1[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS1[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS1[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS1[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS1[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS1[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS1[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS1[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS1[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS1[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS1[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS1[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS1[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS1[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS1[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS1[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS1[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS1[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS1[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS1[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS1[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS1[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS1[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS1[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS1[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS1[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS1[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS1[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS1[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS1[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS1[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS1[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS1[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS1[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS1[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS1[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS1[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS1[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS1[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS1[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS1[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS1[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS1[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS1[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS1[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS1[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS1[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS1[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS1[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS1[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		
		index2 += 2;
	}
	return;
}
void acts_all::MEMACCESS_readmanyvmasks3(bool_type enable, uint512_dt * kvdram, unit1_type vmaskBITS0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS1[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS2[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount1 = BLOCKRAM_SIZE;
	analysis_type analysis_loopcount2 = BLOCKRAM_SIZE / 16;
	analysis_type analysis_loopcount3 = BLOCKRAM_SIZE;
	
	buffer_type transfsize = size_kvs * 16;
	uint32_type bitsbuffer[MAXREDUCEBUFFERSZ];

	READVMASKS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount1 avg=analysis_loopcount1
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		tempbuffer[0][i] = kvdram[offset_kvs + i].range(31, 0); 
		tempbuffer[1][i] = kvdram[offset_kvs + i].range(63, 32); 
		tempbuffer[2][i] = kvdram[offset_kvs + i].range(95, 64); 
		tempbuffer[3][i] = kvdram[offset_kvs + i].range(127, 96); 
		tempbuffer[4][i] = kvdram[offset_kvs + i].range(159, 128); 
		tempbuffer[5][i] = kvdram[offset_kvs + i].range(191, 160); 
		tempbuffer[6][i] = kvdram[offset_kvs + i].range(223, 192); 
		tempbuffer[7][i] = kvdram[offset_kvs + i].range(255, 224); 
		tempbuffer[8][i] = kvdram[offset_kvs + i].range(287, 256); 
		tempbuffer[9][i] = kvdram[offset_kvs + i].range(319, 288); 
		tempbuffer[10][i] = kvdram[offset_kvs + i].range(351, 320); 
		tempbuffer[11][i] = kvdram[offset_kvs + i].range(383, 352); 
		tempbuffer[12][i] = kvdram[offset_kvs + i].range(415, 384); 
		tempbuffer[13][i] = kvdram[offset_kvs + i].range(447, 416); 
		tempbuffer[14][i] = kvdram[offset_kvs + i].range(479, 448); 
		tempbuffer[15][i] = kvdram[offset_kvs + i].range(511, 480); 
		#else 
		tempbuffer[0][i].key = kvdram[offset_kvs + i].data[0].key;
		tempbuffer[0][i].value = kvdram[offset_kvs + i].data[0].value; 
		tempbuffer[1][i].key = kvdram[offset_kvs + i].data[1].key;
		tempbuffer[1][i].value = kvdram[offset_kvs + i].data[1].value; 
		tempbuffer[2][i].key = kvdram[offset_kvs + i].data[2].key;
		tempbuffer[2][i].value = kvdram[offset_kvs + i].data[2].value; 
		tempbuffer[3][i].key = kvdram[offset_kvs + i].data[3].key;
		tempbuffer[3][i].value = kvdram[offset_kvs + i].data[3].value; 
		tempbuffer[4][i].key = kvdram[offset_kvs + i].data[4].key;
		tempbuffer[4][i].value = kvdram[offset_kvs + i].data[4].value; 
		tempbuffer[5][i].key = kvdram[offset_kvs + i].data[5].key;
		tempbuffer[5][i].value = kvdram[offset_kvs + i].data[5].value; 
		tempbuffer[6][i].key = kvdram[offset_kvs + i].data[6].key;
		tempbuffer[6][i].value = kvdram[offset_kvs + i].data[6].value; 
		tempbuffer[7][i].key = kvdram[offset_kvs + i].data[7].key;
		tempbuffer[7][i].value = kvdram[offset_kvs + i].data[7].value; 
		
		#endif 
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	
	uint32_type D0;
	uint32_type D1;
	uint32_type D2;
	uint32_type D3;
	uint32_type D4;
	uint32_type D5;
	uint32_type D6;
	uint32_type D7;
	uint32_type D8;
	uint32_type D9;
	uint32_type D10;
	uint32_type D11;
	uint32_type D12;
	uint32_type D13;
	uint32_type D14;
	uint32_type D15;
	buffer_type index = 0;
	LOADVMASKS_LOOP1: for (buffer_type i=0; i<size_kvs; i++){
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
		bitsbuffer[index + 1] = tempbuffer[0][i].value;
		bitsbuffer[index + 2] = tempbuffer[1][i].key;
		bitsbuffer[index + 3] = tempbuffer[1][i].value;
		bitsbuffer[index + 4] = tempbuffer[2][i].key;
		bitsbuffer[index + 5] = tempbuffer[2][i].value;
		bitsbuffer[index + 6] = tempbuffer[3][i].key;
		bitsbuffer[index + 7] = tempbuffer[3][i].value;
		bitsbuffer[index + 8] = tempbuffer[4][i].key;
		bitsbuffer[index + 9] = tempbuffer[4][i].value;
		bitsbuffer[index + 10] = tempbuffer[5][i].key;
		bitsbuffer[index + 11] = tempbuffer[5][i].value;
		bitsbuffer[index + 12] = tempbuffer[6][i].key;
		bitsbuffer[index + 13] = tempbuffer[6][i].value;
		bitsbuffer[index + 14] = tempbuffer[7][i].key;
		bitsbuffer[index + 15] = tempbuffer[7][i].value;
		#endif
		
		index += VECTOR_SIZE * 2;
	}

	// u32 => u16bits	
	buffer_type index2 = 0;
	LOADVMASKS_LOOP2: for (buffer_type i=0; i<transfsize; i++){ // transfsize, reducebuffersz
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount3 avg=analysis_loopcount3
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		vmaskBITS0[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS0[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS0[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS0[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS0[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS0[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS0[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS0[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS0[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS0[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS0[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS0[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS0[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS0[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS0[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS0[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS0[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS0[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS0[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS0[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS0[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS0[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS0[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS0[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS0[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS0[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS0[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS0[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS0[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS0[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS0[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS0[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS0[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS0[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS0[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS0[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS0[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS0[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS0[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS0[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS0[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS0[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS0[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS0[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS0[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS0[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS0[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS0[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS0[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS0[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS0[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS0[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS0[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS0[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS0[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS0[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS0[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS0[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS0[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS0[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS0[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS0[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS0[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS0[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS1[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS1[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS1[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS1[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS1[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS1[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS1[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS1[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS1[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS1[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS1[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS1[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS1[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS1[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS1[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS1[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS1[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS1[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS1[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS1[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS1[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS1[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS1[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS1[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS1[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS1[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS1[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS1[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS1[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS1[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS1[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS1[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS1[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS1[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS1[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS1[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS1[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS1[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS1[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS1[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS1[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS1[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS1[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS1[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS1[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS1[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS1[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS1[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS1[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS1[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS1[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS1[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS1[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS1[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS1[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS1[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS1[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS1[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS1[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS1[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS1[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS1[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS1[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS1[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS2[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS2[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS2[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS2[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS2[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS2[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS2[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS2[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS2[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS2[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS2[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS2[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS2[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS2[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS2[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS2[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS2[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS2[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS2[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS2[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS2[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS2[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS2[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS2[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS2[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS2[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS2[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS2[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS2[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS2[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS2[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS2[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS2[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS2[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS2[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS2[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS2[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS2[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS2[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS2[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS2[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS2[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS2[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS2[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS2[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS2[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS2[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS2[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS2[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS2[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS2[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS2[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS2[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS2[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS2[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS2[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS2[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS2[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS2[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS2[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS2[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS2[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS2[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS2[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		
		index2 += 2;
	}
	return;
}
void acts_all::MEMACCESS_readmanyvmasks4(bool_type enable, uint512_dt * kvdram, unit1_type vmaskBITS0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS1[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS2[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS3[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount1 = BLOCKRAM_SIZE;
	analysis_type analysis_loopcount2 = BLOCKRAM_SIZE / 16;
	analysis_type analysis_loopcount3 = BLOCKRAM_SIZE;
	
	buffer_type transfsize = size_kvs * 16;
	uint32_type bitsbuffer[MAXREDUCEBUFFERSZ];

	READVMASKS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount1 avg=analysis_loopcount1
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		tempbuffer[0][i] = kvdram[offset_kvs + i].range(31, 0); 
		tempbuffer[1][i] = kvdram[offset_kvs + i].range(63, 32); 
		tempbuffer[2][i] = kvdram[offset_kvs + i].range(95, 64); 
		tempbuffer[3][i] = kvdram[offset_kvs + i].range(127, 96); 
		tempbuffer[4][i] = kvdram[offset_kvs + i].range(159, 128); 
		tempbuffer[5][i] = kvdram[offset_kvs + i].range(191, 160); 
		tempbuffer[6][i] = kvdram[offset_kvs + i].range(223, 192); 
		tempbuffer[7][i] = kvdram[offset_kvs + i].range(255, 224); 
		tempbuffer[8][i] = kvdram[offset_kvs + i].range(287, 256); 
		tempbuffer[9][i] = kvdram[offset_kvs + i].range(319, 288); 
		tempbuffer[10][i] = kvdram[offset_kvs + i].range(351, 320); 
		tempbuffer[11][i] = kvdram[offset_kvs + i].range(383, 352); 
		tempbuffer[12][i] = kvdram[offset_kvs + i].range(415, 384); 
		tempbuffer[13][i] = kvdram[offset_kvs + i].range(447, 416); 
		tempbuffer[14][i] = kvdram[offset_kvs + i].range(479, 448); 
		tempbuffer[15][i] = kvdram[offset_kvs + i].range(511, 480); 
		#else 
		tempbuffer[0][i].key = kvdram[offset_kvs + i].data[0].key;
		tempbuffer[0][i].value = kvdram[offset_kvs + i].data[0].value; 
		tempbuffer[1][i].key = kvdram[offset_kvs + i].data[1].key;
		tempbuffer[1][i].value = kvdram[offset_kvs + i].data[1].value; 
		tempbuffer[2][i].key = kvdram[offset_kvs + i].data[2].key;
		tempbuffer[2][i].value = kvdram[offset_kvs + i].data[2].value; 
		tempbuffer[3][i].key = kvdram[offset_kvs + i].data[3].key;
		tempbuffer[3][i].value = kvdram[offset_kvs + i].data[3].value; 
		tempbuffer[4][i].key = kvdram[offset_kvs + i].data[4].key;
		tempbuffer[4][i].value = kvdram[offset_kvs + i].data[4].value; 
		tempbuffer[5][i].key = kvdram[offset_kvs + i].data[5].key;
		tempbuffer[5][i].value = kvdram[offset_kvs + i].data[5].value; 
		tempbuffer[6][i].key = kvdram[offset_kvs + i].data[6].key;
		tempbuffer[6][i].value = kvdram[offset_kvs + i].data[6].value; 
		tempbuffer[7][i].key = kvdram[offset_kvs + i].data[7].key;
		tempbuffer[7][i].value = kvdram[offset_kvs + i].data[7].value; 
		
		#endif 
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	
	uint32_type D0;
	uint32_type D1;
	uint32_type D2;
	uint32_type D3;
	uint32_type D4;
	uint32_type D5;
	uint32_type D6;
	uint32_type D7;
	uint32_type D8;
	uint32_type D9;
	uint32_type D10;
	uint32_type D11;
	uint32_type D12;
	uint32_type D13;
	uint32_type D14;
	uint32_type D15;
	buffer_type index = 0;
	LOADVMASKS_LOOP1: for (buffer_type i=0; i<size_kvs; i++){
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
		bitsbuffer[index + 1] = tempbuffer[0][i].value;
		bitsbuffer[index + 2] = tempbuffer[1][i].key;
		bitsbuffer[index + 3] = tempbuffer[1][i].value;
		bitsbuffer[index + 4] = tempbuffer[2][i].key;
		bitsbuffer[index + 5] = tempbuffer[2][i].value;
		bitsbuffer[index + 6] = tempbuffer[3][i].key;
		bitsbuffer[index + 7] = tempbuffer[3][i].value;
		bitsbuffer[index + 8] = tempbuffer[4][i].key;
		bitsbuffer[index + 9] = tempbuffer[4][i].value;
		bitsbuffer[index + 10] = tempbuffer[5][i].key;
		bitsbuffer[index + 11] = tempbuffer[5][i].value;
		bitsbuffer[index + 12] = tempbuffer[6][i].key;
		bitsbuffer[index + 13] = tempbuffer[6][i].value;
		bitsbuffer[index + 14] = tempbuffer[7][i].key;
		bitsbuffer[index + 15] = tempbuffer[7][i].value;
		#endif
		
		index += VECTOR_SIZE * 2;
	}

	// u32 => u16bits	
	buffer_type index2 = 0;
	LOADVMASKS_LOOP2: for (buffer_type i=0; i<transfsize; i++){ // transfsize, reducebuffersz
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount3 avg=analysis_loopcount3
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		vmaskBITS0[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS0[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS0[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS0[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS0[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS0[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS0[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS0[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS0[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS0[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS0[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS0[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS0[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS0[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS0[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS0[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS0[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS0[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS0[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS0[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS0[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS0[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS0[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS0[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS0[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS0[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS0[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS0[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS0[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS0[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS0[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS0[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS0[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS0[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS0[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS0[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS0[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS0[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS0[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS0[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS0[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS0[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS0[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS0[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS0[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS0[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS0[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS0[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS0[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS0[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS0[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS0[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS0[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS0[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS0[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS0[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS0[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS0[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS0[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS0[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS0[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS0[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS0[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS0[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS1[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS1[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS1[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS1[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS1[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS1[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS1[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS1[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS1[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS1[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS1[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS1[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS1[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS1[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS1[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS1[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS1[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS1[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS1[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS1[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS1[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS1[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS1[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS1[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS1[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS1[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS1[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS1[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS1[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS1[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS1[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS1[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS1[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS1[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS1[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS1[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS1[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS1[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS1[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS1[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS1[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS1[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS1[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS1[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS1[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS1[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS1[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS1[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS1[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS1[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS1[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS1[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS1[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS1[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS1[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS1[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS1[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS1[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS1[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS1[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS1[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS1[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS1[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS1[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS2[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS2[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS2[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS2[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS2[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS2[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS2[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS2[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS2[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS2[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS2[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS2[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS2[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS2[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS2[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS2[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS2[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS2[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS2[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS2[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS2[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS2[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS2[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS2[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS2[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS2[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS2[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS2[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS2[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS2[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS2[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS2[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS2[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS2[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS2[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS2[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS2[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS2[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS2[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS2[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS2[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS2[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS2[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS2[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS2[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS2[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS2[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS2[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS2[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS2[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS2[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS2[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS2[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS2[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS2[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS2[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS2[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS2[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS2[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS2[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS2[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS2[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS2[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS2[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS3[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS3[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS3[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS3[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS3[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS3[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS3[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS3[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS3[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS3[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS3[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS3[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS3[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS3[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS3[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS3[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS3[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS3[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS3[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS3[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS3[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS3[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS3[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS3[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS3[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS3[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS3[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS3[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS3[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS3[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS3[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS3[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS3[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS3[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS3[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS3[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS3[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS3[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS3[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS3[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS3[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS3[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS3[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS3[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS3[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS3[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS3[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS3[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS3[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS3[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS3[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS3[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS3[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS3[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS3[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS3[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS3[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS3[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS3[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS3[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS3[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS3[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS3[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS3[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		
		index2 += 2;
	}
	return;
}
void acts_all::MEMACCESS_readmanyvmasks5(bool_type enable, uint512_dt * kvdram, unit1_type vmaskBITS0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS1[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS2[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS3[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS4[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount1 = BLOCKRAM_SIZE;
	analysis_type analysis_loopcount2 = BLOCKRAM_SIZE / 16;
	analysis_type analysis_loopcount3 = BLOCKRAM_SIZE;
	
	buffer_type transfsize = size_kvs * 16;
	uint32_type bitsbuffer[MAXREDUCEBUFFERSZ];

	READVMASKS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount1 avg=analysis_loopcount1
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		tempbuffer[0][i] = kvdram[offset_kvs + i].range(31, 0); 
		tempbuffer[1][i] = kvdram[offset_kvs + i].range(63, 32); 
		tempbuffer[2][i] = kvdram[offset_kvs + i].range(95, 64); 
		tempbuffer[3][i] = kvdram[offset_kvs + i].range(127, 96); 
		tempbuffer[4][i] = kvdram[offset_kvs + i].range(159, 128); 
		tempbuffer[5][i] = kvdram[offset_kvs + i].range(191, 160); 
		tempbuffer[6][i] = kvdram[offset_kvs + i].range(223, 192); 
		tempbuffer[7][i] = kvdram[offset_kvs + i].range(255, 224); 
		tempbuffer[8][i] = kvdram[offset_kvs + i].range(287, 256); 
		tempbuffer[9][i] = kvdram[offset_kvs + i].range(319, 288); 
		tempbuffer[10][i] = kvdram[offset_kvs + i].range(351, 320); 
		tempbuffer[11][i] = kvdram[offset_kvs + i].range(383, 352); 
		tempbuffer[12][i] = kvdram[offset_kvs + i].range(415, 384); 
		tempbuffer[13][i] = kvdram[offset_kvs + i].range(447, 416); 
		tempbuffer[14][i] = kvdram[offset_kvs + i].range(479, 448); 
		tempbuffer[15][i] = kvdram[offset_kvs + i].range(511, 480); 
		#else 
		tempbuffer[0][i].key = kvdram[offset_kvs + i].data[0].key;
		tempbuffer[0][i].value = kvdram[offset_kvs + i].data[0].value; 
		tempbuffer[1][i].key = kvdram[offset_kvs + i].data[1].key;
		tempbuffer[1][i].value = kvdram[offset_kvs + i].data[1].value; 
		tempbuffer[2][i].key = kvdram[offset_kvs + i].data[2].key;
		tempbuffer[2][i].value = kvdram[offset_kvs + i].data[2].value; 
		tempbuffer[3][i].key = kvdram[offset_kvs + i].data[3].key;
		tempbuffer[3][i].value = kvdram[offset_kvs + i].data[3].value; 
		tempbuffer[4][i].key = kvdram[offset_kvs + i].data[4].key;
		tempbuffer[4][i].value = kvdram[offset_kvs + i].data[4].value; 
		tempbuffer[5][i].key = kvdram[offset_kvs + i].data[5].key;
		tempbuffer[5][i].value = kvdram[offset_kvs + i].data[5].value; 
		tempbuffer[6][i].key = kvdram[offset_kvs + i].data[6].key;
		tempbuffer[6][i].value = kvdram[offset_kvs + i].data[6].value; 
		tempbuffer[7][i].key = kvdram[offset_kvs + i].data[7].key;
		tempbuffer[7][i].value = kvdram[offset_kvs + i].data[7].value; 
		
		#endif 
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	
	uint32_type D0;
	uint32_type D1;
	uint32_type D2;
	uint32_type D3;
	uint32_type D4;
	uint32_type D5;
	uint32_type D6;
	uint32_type D7;
	uint32_type D8;
	uint32_type D9;
	uint32_type D10;
	uint32_type D11;
	uint32_type D12;
	uint32_type D13;
	uint32_type D14;
	uint32_type D15;
	buffer_type index = 0;
	LOADVMASKS_LOOP1: for (buffer_type i=0; i<size_kvs; i++){
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
		bitsbuffer[index + 1] = tempbuffer[0][i].value;
		bitsbuffer[index + 2] = tempbuffer[1][i].key;
		bitsbuffer[index + 3] = tempbuffer[1][i].value;
		bitsbuffer[index + 4] = tempbuffer[2][i].key;
		bitsbuffer[index + 5] = tempbuffer[2][i].value;
		bitsbuffer[index + 6] = tempbuffer[3][i].key;
		bitsbuffer[index + 7] = tempbuffer[3][i].value;
		bitsbuffer[index + 8] = tempbuffer[4][i].key;
		bitsbuffer[index + 9] = tempbuffer[4][i].value;
		bitsbuffer[index + 10] = tempbuffer[5][i].key;
		bitsbuffer[index + 11] = tempbuffer[5][i].value;
		bitsbuffer[index + 12] = tempbuffer[6][i].key;
		bitsbuffer[index + 13] = tempbuffer[6][i].value;
		bitsbuffer[index + 14] = tempbuffer[7][i].key;
		bitsbuffer[index + 15] = tempbuffer[7][i].value;
		#endif
		
		index += VECTOR_SIZE * 2;
	}

	// u32 => u16bits	
	buffer_type index2 = 0;
	LOADVMASKS_LOOP2: for (buffer_type i=0; i<transfsize; i++){ // transfsize, reducebuffersz
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount3 avg=analysis_loopcount3
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		vmaskBITS0[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS0[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS0[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS0[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS0[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS0[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS0[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS0[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS0[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS0[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS0[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS0[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS0[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS0[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS0[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS0[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS0[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS0[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS0[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS0[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS0[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS0[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS0[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS0[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS0[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS0[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS0[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS0[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS0[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS0[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS0[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS0[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS0[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS0[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS0[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS0[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS0[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS0[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS0[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS0[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS0[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS0[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS0[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS0[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS0[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS0[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS0[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS0[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS0[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS0[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS0[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS0[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS0[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS0[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS0[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS0[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS0[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS0[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS0[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS0[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS0[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS0[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS0[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS0[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS1[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS1[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS1[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS1[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS1[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS1[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS1[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS1[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS1[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS1[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS1[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS1[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS1[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS1[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS1[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS1[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS1[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS1[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS1[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS1[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS1[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS1[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS1[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS1[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS1[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS1[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS1[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS1[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS1[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS1[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS1[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS1[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS1[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS1[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS1[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS1[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS1[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS1[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS1[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS1[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS1[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS1[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS1[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS1[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS1[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS1[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS1[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS1[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS1[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS1[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS1[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS1[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS1[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS1[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS1[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS1[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS1[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS1[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS1[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS1[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS1[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS1[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS1[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS1[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS2[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS2[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS2[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS2[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS2[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS2[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS2[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS2[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS2[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS2[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS2[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS2[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS2[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS2[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS2[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS2[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS2[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS2[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS2[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS2[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS2[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS2[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS2[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS2[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS2[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS2[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS2[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS2[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS2[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS2[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS2[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS2[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS2[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS2[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS2[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS2[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS2[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS2[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS2[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS2[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS2[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS2[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS2[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS2[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS2[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS2[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS2[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS2[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS2[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS2[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS2[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS2[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS2[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS2[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS2[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS2[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS2[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS2[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS2[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS2[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS2[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS2[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS2[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS2[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS3[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS3[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS3[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS3[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS3[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS3[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS3[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS3[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS3[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS3[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS3[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS3[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS3[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS3[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS3[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS3[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS3[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS3[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS3[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS3[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS3[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS3[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS3[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS3[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS3[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS3[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS3[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS3[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS3[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS3[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS3[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS3[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS3[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS3[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS3[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS3[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS3[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS3[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS3[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS3[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS3[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS3[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS3[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS3[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS3[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS3[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS3[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS3[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS3[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS3[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS3[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS3[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS3[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS3[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS3[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS3[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS3[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS3[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS3[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS3[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS3[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS3[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS3[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS3[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS4[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS4[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS4[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS4[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS4[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS4[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS4[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS4[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS4[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS4[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS4[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS4[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS4[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS4[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS4[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS4[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS4[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS4[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS4[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS4[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS4[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS4[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS4[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS4[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS4[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS4[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS4[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS4[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS4[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS4[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS4[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS4[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS4[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS4[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS4[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS4[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS4[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS4[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS4[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS4[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS4[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS4[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS4[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS4[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS4[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS4[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS4[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS4[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS4[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS4[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS4[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS4[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS4[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS4[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS4[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS4[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS4[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS4[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS4[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS4[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS4[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS4[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS4[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS4[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		
		index2 += 2;
	}
	return;
}
void acts_all::MEMACCESS_readmanyvmasks6(bool_type enable, uint512_dt * kvdram, unit1_type vmaskBITS0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS1[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS2[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS3[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS4[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS5[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount1 = BLOCKRAM_SIZE;
	analysis_type analysis_loopcount2 = BLOCKRAM_SIZE / 16;
	analysis_type analysis_loopcount3 = BLOCKRAM_SIZE;
	
	buffer_type transfsize = size_kvs * 16;
	uint32_type bitsbuffer[MAXREDUCEBUFFERSZ];

	READVMASKS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount1 avg=analysis_loopcount1
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		tempbuffer[0][i] = kvdram[offset_kvs + i].range(31, 0); 
		tempbuffer[1][i] = kvdram[offset_kvs + i].range(63, 32); 
		tempbuffer[2][i] = kvdram[offset_kvs + i].range(95, 64); 
		tempbuffer[3][i] = kvdram[offset_kvs + i].range(127, 96); 
		tempbuffer[4][i] = kvdram[offset_kvs + i].range(159, 128); 
		tempbuffer[5][i] = kvdram[offset_kvs + i].range(191, 160); 
		tempbuffer[6][i] = kvdram[offset_kvs + i].range(223, 192); 
		tempbuffer[7][i] = kvdram[offset_kvs + i].range(255, 224); 
		tempbuffer[8][i] = kvdram[offset_kvs + i].range(287, 256); 
		tempbuffer[9][i] = kvdram[offset_kvs + i].range(319, 288); 
		tempbuffer[10][i] = kvdram[offset_kvs + i].range(351, 320); 
		tempbuffer[11][i] = kvdram[offset_kvs + i].range(383, 352); 
		tempbuffer[12][i] = kvdram[offset_kvs + i].range(415, 384); 
		tempbuffer[13][i] = kvdram[offset_kvs + i].range(447, 416); 
		tempbuffer[14][i] = kvdram[offset_kvs + i].range(479, 448); 
		tempbuffer[15][i] = kvdram[offset_kvs + i].range(511, 480); 
		#else 
		tempbuffer[0][i].key = kvdram[offset_kvs + i].data[0].key;
		tempbuffer[0][i].value = kvdram[offset_kvs + i].data[0].value; 
		tempbuffer[1][i].key = kvdram[offset_kvs + i].data[1].key;
		tempbuffer[1][i].value = kvdram[offset_kvs + i].data[1].value; 
		tempbuffer[2][i].key = kvdram[offset_kvs + i].data[2].key;
		tempbuffer[2][i].value = kvdram[offset_kvs + i].data[2].value; 
		tempbuffer[3][i].key = kvdram[offset_kvs + i].data[3].key;
		tempbuffer[3][i].value = kvdram[offset_kvs + i].data[3].value; 
		tempbuffer[4][i].key = kvdram[offset_kvs + i].data[4].key;
		tempbuffer[4][i].value = kvdram[offset_kvs + i].data[4].value; 
		tempbuffer[5][i].key = kvdram[offset_kvs + i].data[5].key;
		tempbuffer[5][i].value = kvdram[offset_kvs + i].data[5].value; 
		tempbuffer[6][i].key = kvdram[offset_kvs + i].data[6].key;
		tempbuffer[6][i].value = kvdram[offset_kvs + i].data[6].value; 
		tempbuffer[7][i].key = kvdram[offset_kvs + i].data[7].key;
		tempbuffer[7][i].value = kvdram[offset_kvs + i].data[7].value; 
		
		#endif 
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	
	uint32_type D0;
	uint32_type D1;
	uint32_type D2;
	uint32_type D3;
	uint32_type D4;
	uint32_type D5;
	uint32_type D6;
	uint32_type D7;
	uint32_type D8;
	uint32_type D9;
	uint32_type D10;
	uint32_type D11;
	uint32_type D12;
	uint32_type D13;
	uint32_type D14;
	uint32_type D15;
	buffer_type index = 0;
	LOADVMASKS_LOOP1: for (buffer_type i=0; i<size_kvs; i++){
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
		bitsbuffer[index + 1] = tempbuffer[0][i].value;
		bitsbuffer[index + 2] = tempbuffer[1][i].key;
		bitsbuffer[index + 3] = tempbuffer[1][i].value;
		bitsbuffer[index + 4] = tempbuffer[2][i].key;
		bitsbuffer[index + 5] = tempbuffer[2][i].value;
		bitsbuffer[index + 6] = tempbuffer[3][i].key;
		bitsbuffer[index + 7] = tempbuffer[3][i].value;
		bitsbuffer[index + 8] = tempbuffer[4][i].key;
		bitsbuffer[index + 9] = tempbuffer[4][i].value;
		bitsbuffer[index + 10] = tempbuffer[5][i].key;
		bitsbuffer[index + 11] = tempbuffer[5][i].value;
		bitsbuffer[index + 12] = tempbuffer[6][i].key;
		bitsbuffer[index + 13] = tempbuffer[6][i].value;
		bitsbuffer[index + 14] = tempbuffer[7][i].key;
		bitsbuffer[index + 15] = tempbuffer[7][i].value;
		#endif
		
		index += VECTOR_SIZE * 2;
	}

	// u32 => u16bits	
	buffer_type index2 = 0;
	LOADVMASKS_LOOP2: for (buffer_type i=0; i<transfsize; i++){ // transfsize, reducebuffersz
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount3 avg=analysis_loopcount3
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		vmaskBITS0[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS0[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS0[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS0[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS0[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS0[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS0[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS0[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS0[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS0[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS0[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS0[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS0[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS0[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS0[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS0[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS0[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS0[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS0[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS0[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS0[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS0[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS0[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS0[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS0[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS0[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS0[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS0[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS0[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS0[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS0[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS0[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS0[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS0[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS0[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS0[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS0[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS0[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS0[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS0[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS0[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS0[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS0[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS0[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS0[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS0[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS0[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS0[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS0[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS0[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS0[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS0[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS0[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS0[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS0[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS0[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS0[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS0[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS0[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS0[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS0[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS0[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS0[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS0[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS1[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS1[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS1[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS1[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS1[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS1[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS1[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS1[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS1[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS1[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS1[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS1[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS1[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS1[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS1[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS1[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS1[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS1[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS1[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS1[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS1[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS1[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS1[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS1[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS1[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS1[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS1[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS1[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS1[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS1[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS1[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS1[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS1[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS1[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS1[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS1[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS1[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS1[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS1[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS1[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS1[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS1[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS1[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS1[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS1[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS1[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS1[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS1[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS1[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS1[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS1[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS1[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS1[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS1[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS1[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS1[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS1[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS1[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS1[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS1[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS1[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS1[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS1[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS1[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS2[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS2[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS2[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS2[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS2[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS2[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS2[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS2[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS2[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS2[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS2[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS2[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS2[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS2[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS2[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS2[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS2[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS2[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS2[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS2[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS2[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS2[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS2[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS2[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS2[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS2[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS2[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS2[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS2[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS2[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS2[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS2[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS2[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS2[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS2[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS2[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS2[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS2[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS2[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS2[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS2[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS2[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS2[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS2[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS2[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS2[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS2[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS2[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS2[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS2[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS2[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS2[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS2[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS2[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS2[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS2[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS2[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS2[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS2[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS2[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS2[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS2[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS2[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS2[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS3[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS3[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS3[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS3[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS3[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS3[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS3[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS3[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS3[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS3[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS3[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS3[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS3[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS3[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS3[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS3[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS3[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS3[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS3[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS3[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS3[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS3[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS3[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS3[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS3[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS3[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS3[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS3[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS3[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS3[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS3[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS3[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS3[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS3[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS3[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS3[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS3[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS3[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS3[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS3[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS3[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS3[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS3[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS3[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS3[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS3[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS3[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS3[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS3[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS3[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS3[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS3[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS3[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS3[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS3[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS3[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS3[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS3[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS3[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS3[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS3[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS3[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS3[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS3[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS4[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS4[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS4[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS4[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS4[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS4[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS4[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS4[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS4[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS4[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS4[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS4[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS4[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS4[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS4[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS4[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS4[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS4[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS4[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS4[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS4[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS4[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS4[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS4[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS4[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS4[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS4[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS4[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS4[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS4[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS4[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS4[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS4[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS4[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS4[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS4[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS4[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS4[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS4[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS4[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS4[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS4[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS4[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS4[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS4[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS4[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS4[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS4[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS4[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS4[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS4[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS4[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS4[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS4[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS4[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS4[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS4[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS4[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS4[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS4[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS4[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS4[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS4[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS4[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS5[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS5[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS5[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS5[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS5[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS5[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS5[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS5[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS5[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS5[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS5[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS5[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS5[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS5[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS5[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS5[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS5[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS5[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS5[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS5[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS5[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS5[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS5[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS5[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS5[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS5[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS5[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS5[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS5[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS5[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS5[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS5[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS5[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS5[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS5[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS5[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS5[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS5[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS5[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS5[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS5[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS5[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS5[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS5[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS5[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS5[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS5[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS5[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS5[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS5[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS5[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS5[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS5[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS5[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS5[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS5[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS5[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS5[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS5[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS5[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS5[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS5[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS5[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS5[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		
		index2 += 2;
	}
	return;
}
void acts_all::MEMACCESS_readmanyvmasks7(bool_type enable, uint512_dt * kvdram, unit1_type vmaskBITS0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS1[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS2[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS3[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS4[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS5[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS6[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount1 = BLOCKRAM_SIZE;
	analysis_type analysis_loopcount2 = BLOCKRAM_SIZE / 16;
	analysis_type analysis_loopcount3 = BLOCKRAM_SIZE;
	
	buffer_type transfsize = size_kvs * 16;
	uint32_type bitsbuffer[MAXREDUCEBUFFERSZ];

	READVMASKS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount1 avg=analysis_loopcount1
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		tempbuffer[0][i] = kvdram[offset_kvs + i].range(31, 0); 
		tempbuffer[1][i] = kvdram[offset_kvs + i].range(63, 32); 
		tempbuffer[2][i] = kvdram[offset_kvs + i].range(95, 64); 
		tempbuffer[3][i] = kvdram[offset_kvs + i].range(127, 96); 
		tempbuffer[4][i] = kvdram[offset_kvs + i].range(159, 128); 
		tempbuffer[5][i] = kvdram[offset_kvs + i].range(191, 160); 
		tempbuffer[6][i] = kvdram[offset_kvs + i].range(223, 192); 
		tempbuffer[7][i] = kvdram[offset_kvs + i].range(255, 224); 
		tempbuffer[8][i] = kvdram[offset_kvs + i].range(287, 256); 
		tempbuffer[9][i] = kvdram[offset_kvs + i].range(319, 288); 
		tempbuffer[10][i] = kvdram[offset_kvs + i].range(351, 320); 
		tempbuffer[11][i] = kvdram[offset_kvs + i].range(383, 352); 
		tempbuffer[12][i] = kvdram[offset_kvs + i].range(415, 384); 
		tempbuffer[13][i] = kvdram[offset_kvs + i].range(447, 416); 
		tempbuffer[14][i] = kvdram[offset_kvs + i].range(479, 448); 
		tempbuffer[15][i] = kvdram[offset_kvs + i].range(511, 480); 
		#else 
		tempbuffer[0][i].key = kvdram[offset_kvs + i].data[0].key;
		tempbuffer[0][i].value = kvdram[offset_kvs + i].data[0].value; 
		tempbuffer[1][i].key = kvdram[offset_kvs + i].data[1].key;
		tempbuffer[1][i].value = kvdram[offset_kvs + i].data[1].value; 
		tempbuffer[2][i].key = kvdram[offset_kvs + i].data[2].key;
		tempbuffer[2][i].value = kvdram[offset_kvs + i].data[2].value; 
		tempbuffer[3][i].key = kvdram[offset_kvs + i].data[3].key;
		tempbuffer[3][i].value = kvdram[offset_kvs + i].data[3].value; 
		tempbuffer[4][i].key = kvdram[offset_kvs + i].data[4].key;
		tempbuffer[4][i].value = kvdram[offset_kvs + i].data[4].value; 
		tempbuffer[5][i].key = kvdram[offset_kvs + i].data[5].key;
		tempbuffer[5][i].value = kvdram[offset_kvs + i].data[5].value; 
		tempbuffer[6][i].key = kvdram[offset_kvs + i].data[6].key;
		tempbuffer[6][i].value = kvdram[offset_kvs + i].data[6].value; 
		tempbuffer[7][i].key = kvdram[offset_kvs + i].data[7].key;
		tempbuffer[7][i].value = kvdram[offset_kvs + i].data[7].value; 
		
		#endif 
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	
	uint32_type D0;
	uint32_type D1;
	uint32_type D2;
	uint32_type D3;
	uint32_type D4;
	uint32_type D5;
	uint32_type D6;
	uint32_type D7;
	uint32_type D8;
	uint32_type D9;
	uint32_type D10;
	uint32_type D11;
	uint32_type D12;
	uint32_type D13;
	uint32_type D14;
	uint32_type D15;
	buffer_type index = 0;
	LOADVMASKS_LOOP1: for (buffer_type i=0; i<size_kvs; i++){
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
		bitsbuffer[index + 1] = tempbuffer[0][i].value;
		bitsbuffer[index + 2] = tempbuffer[1][i].key;
		bitsbuffer[index + 3] = tempbuffer[1][i].value;
		bitsbuffer[index + 4] = tempbuffer[2][i].key;
		bitsbuffer[index + 5] = tempbuffer[2][i].value;
		bitsbuffer[index + 6] = tempbuffer[3][i].key;
		bitsbuffer[index + 7] = tempbuffer[3][i].value;
		bitsbuffer[index + 8] = tempbuffer[4][i].key;
		bitsbuffer[index + 9] = tempbuffer[4][i].value;
		bitsbuffer[index + 10] = tempbuffer[5][i].key;
		bitsbuffer[index + 11] = tempbuffer[5][i].value;
		bitsbuffer[index + 12] = tempbuffer[6][i].key;
		bitsbuffer[index + 13] = tempbuffer[6][i].value;
		bitsbuffer[index + 14] = tempbuffer[7][i].key;
		bitsbuffer[index + 15] = tempbuffer[7][i].value;
		#endif
		
		index += VECTOR_SIZE * 2;
	}

	// u32 => u16bits	
	buffer_type index2 = 0;
	LOADVMASKS_LOOP2: for (buffer_type i=0; i<transfsize; i++){ // transfsize, reducebuffersz
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount3 avg=analysis_loopcount3
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		vmaskBITS0[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS0[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS0[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS0[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS0[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS0[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS0[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS0[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS0[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS0[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS0[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS0[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS0[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS0[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS0[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS0[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS0[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS0[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS0[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS0[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS0[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS0[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS0[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS0[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS0[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS0[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS0[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS0[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS0[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS0[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS0[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS0[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS0[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS0[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS0[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS0[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS0[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS0[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS0[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS0[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS0[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS0[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS0[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS0[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS0[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS0[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS0[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS0[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS0[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS0[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS0[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS0[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS0[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS0[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS0[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS0[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS0[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS0[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS0[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS0[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS0[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS0[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS0[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS0[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS1[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS1[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS1[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS1[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS1[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS1[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS1[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS1[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS1[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS1[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS1[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS1[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS1[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS1[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS1[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS1[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS1[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS1[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS1[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS1[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS1[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS1[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS1[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS1[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS1[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS1[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS1[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS1[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS1[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS1[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS1[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS1[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS1[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS1[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS1[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS1[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS1[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS1[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS1[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS1[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS1[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS1[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS1[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS1[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS1[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS1[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS1[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS1[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS1[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS1[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS1[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS1[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS1[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS1[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS1[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS1[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS1[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS1[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS1[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS1[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS1[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS1[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS1[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS1[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS2[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS2[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS2[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS2[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS2[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS2[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS2[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS2[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS2[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS2[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS2[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS2[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS2[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS2[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS2[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS2[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS2[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS2[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS2[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS2[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS2[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS2[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS2[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS2[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS2[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS2[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS2[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS2[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS2[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS2[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS2[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS2[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS2[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS2[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS2[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS2[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS2[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS2[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS2[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS2[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS2[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS2[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS2[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS2[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS2[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS2[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS2[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS2[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS2[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS2[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS2[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS2[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS2[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS2[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS2[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS2[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS2[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS2[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS2[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS2[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS2[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS2[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS2[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS2[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS3[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS3[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS3[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS3[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS3[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS3[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS3[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS3[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS3[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS3[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS3[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS3[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS3[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS3[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS3[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS3[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS3[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS3[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS3[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS3[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS3[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS3[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS3[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS3[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS3[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS3[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS3[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS3[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS3[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS3[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS3[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS3[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS3[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS3[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS3[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS3[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS3[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS3[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS3[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS3[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS3[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS3[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS3[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS3[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS3[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS3[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS3[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS3[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS3[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS3[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS3[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS3[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS3[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS3[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS3[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS3[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS3[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS3[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS3[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS3[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS3[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS3[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS3[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS3[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS4[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS4[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS4[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS4[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS4[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS4[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS4[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS4[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS4[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS4[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS4[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS4[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS4[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS4[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS4[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS4[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS4[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS4[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS4[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS4[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS4[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS4[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS4[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS4[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS4[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS4[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS4[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS4[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS4[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS4[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS4[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS4[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS4[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS4[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS4[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS4[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS4[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS4[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS4[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS4[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS4[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS4[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS4[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS4[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS4[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS4[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS4[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS4[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS4[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS4[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS4[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS4[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS4[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS4[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS4[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS4[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS4[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS4[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS4[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS4[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS4[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS4[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS4[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS4[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS5[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS5[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS5[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS5[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS5[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS5[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS5[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS5[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS5[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS5[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS5[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS5[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS5[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS5[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS5[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS5[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS5[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS5[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS5[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS5[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS5[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS5[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS5[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS5[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS5[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS5[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS5[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS5[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS5[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS5[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS5[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS5[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS5[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS5[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS5[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS5[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS5[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS5[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS5[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS5[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS5[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS5[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS5[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS5[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS5[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS5[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS5[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS5[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS5[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS5[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS5[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS5[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS5[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS5[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS5[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS5[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS5[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS5[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS5[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS5[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS5[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS5[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS5[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS5[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS6[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS6[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS6[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS6[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS6[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS6[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS6[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS6[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS6[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS6[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS6[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS6[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS6[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS6[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS6[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS6[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS6[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS6[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS6[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS6[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS6[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS6[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS6[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS6[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS6[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS6[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS6[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS6[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS6[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS6[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS6[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS6[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS6[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS6[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS6[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS6[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS6[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS6[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS6[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS6[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS6[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS6[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS6[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS6[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS6[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS6[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS6[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS6[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS6[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS6[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS6[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS6[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS6[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS6[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS6[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS6[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS6[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS6[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS6[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS6[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS6[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS6[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS6[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS6[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		
		index2 += 2;
	}
	return;
}
void acts_all::MEMACCESS_readmanyvmasks8(bool_type enable, uint512_dt * kvdram, unit1_type vmaskBITS0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS1[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS2[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS3[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS4[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS5[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS6[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS7[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount1 = BLOCKRAM_SIZE;
	analysis_type analysis_loopcount2 = BLOCKRAM_SIZE / 16;
	analysis_type analysis_loopcount3 = BLOCKRAM_SIZE;
	
	buffer_type transfsize = size_kvs * 16;
	uint32_type bitsbuffer[MAXREDUCEBUFFERSZ];

	READVMASKS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount1 avg=analysis_loopcount1
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		tempbuffer[0][i] = kvdram[offset_kvs + i].range(31, 0); 
		tempbuffer[1][i] = kvdram[offset_kvs + i].range(63, 32); 
		tempbuffer[2][i] = kvdram[offset_kvs + i].range(95, 64); 
		tempbuffer[3][i] = kvdram[offset_kvs + i].range(127, 96); 
		tempbuffer[4][i] = kvdram[offset_kvs + i].range(159, 128); 
		tempbuffer[5][i] = kvdram[offset_kvs + i].range(191, 160); 
		tempbuffer[6][i] = kvdram[offset_kvs + i].range(223, 192); 
		tempbuffer[7][i] = kvdram[offset_kvs + i].range(255, 224); 
		tempbuffer[8][i] = kvdram[offset_kvs + i].range(287, 256); 
		tempbuffer[9][i] = kvdram[offset_kvs + i].range(319, 288); 
		tempbuffer[10][i] = kvdram[offset_kvs + i].range(351, 320); 
		tempbuffer[11][i] = kvdram[offset_kvs + i].range(383, 352); 
		tempbuffer[12][i] = kvdram[offset_kvs + i].range(415, 384); 
		tempbuffer[13][i] = kvdram[offset_kvs + i].range(447, 416); 
		tempbuffer[14][i] = kvdram[offset_kvs + i].range(479, 448); 
		tempbuffer[15][i] = kvdram[offset_kvs + i].range(511, 480); 
		#else 
		tempbuffer[0][i].key = kvdram[offset_kvs + i].data[0].key;
		tempbuffer[0][i].value = kvdram[offset_kvs + i].data[0].value; 
		tempbuffer[1][i].key = kvdram[offset_kvs + i].data[1].key;
		tempbuffer[1][i].value = kvdram[offset_kvs + i].data[1].value; 
		tempbuffer[2][i].key = kvdram[offset_kvs + i].data[2].key;
		tempbuffer[2][i].value = kvdram[offset_kvs + i].data[2].value; 
		tempbuffer[3][i].key = kvdram[offset_kvs + i].data[3].key;
		tempbuffer[3][i].value = kvdram[offset_kvs + i].data[3].value; 
		tempbuffer[4][i].key = kvdram[offset_kvs + i].data[4].key;
		tempbuffer[4][i].value = kvdram[offset_kvs + i].data[4].value; 
		tempbuffer[5][i].key = kvdram[offset_kvs + i].data[5].key;
		tempbuffer[5][i].value = kvdram[offset_kvs + i].data[5].value; 
		tempbuffer[6][i].key = kvdram[offset_kvs + i].data[6].key;
		tempbuffer[6][i].value = kvdram[offset_kvs + i].data[6].value; 
		tempbuffer[7][i].key = kvdram[offset_kvs + i].data[7].key;
		tempbuffer[7][i].value = kvdram[offset_kvs + i].data[7].value; 
		
		#endif 
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	
	uint32_type D0;
	uint32_type D1;
	uint32_type D2;
	uint32_type D3;
	uint32_type D4;
	uint32_type D5;
	uint32_type D6;
	uint32_type D7;
	uint32_type D8;
	uint32_type D9;
	uint32_type D10;
	uint32_type D11;
	uint32_type D12;
	uint32_type D13;
	uint32_type D14;
	uint32_type D15;
	buffer_type index = 0;
	LOADVMASKS_LOOP1: for (buffer_type i=0; i<size_kvs; i++){
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
		bitsbuffer[index + 1] = tempbuffer[0][i].value;
		bitsbuffer[index + 2] = tempbuffer[1][i].key;
		bitsbuffer[index + 3] = tempbuffer[1][i].value;
		bitsbuffer[index + 4] = tempbuffer[2][i].key;
		bitsbuffer[index + 5] = tempbuffer[2][i].value;
		bitsbuffer[index + 6] = tempbuffer[3][i].key;
		bitsbuffer[index + 7] = tempbuffer[3][i].value;
		bitsbuffer[index + 8] = tempbuffer[4][i].key;
		bitsbuffer[index + 9] = tempbuffer[4][i].value;
		bitsbuffer[index + 10] = tempbuffer[5][i].key;
		bitsbuffer[index + 11] = tempbuffer[5][i].value;
		bitsbuffer[index + 12] = tempbuffer[6][i].key;
		bitsbuffer[index + 13] = tempbuffer[6][i].value;
		bitsbuffer[index + 14] = tempbuffer[7][i].key;
		bitsbuffer[index + 15] = tempbuffer[7][i].value;
		#endif
		
		index += VECTOR_SIZE * 2;
	}

	// u32 => u16bits	
	buffer_type index2 = 0;
	LOADVMASKS_LOOP2: for (buffer_type i=0; i<transfsize; i++){ // transfsize, reducebuffersz
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount3 avg=analysis_loopcount3
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		vmaskBITS0[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS0[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS0[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS0[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS0[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS0[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS0[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS0[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS0[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS0[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS0[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS0[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS0[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS0[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS0[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS0[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS0[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS0[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS0[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS0[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS0[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS0[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS0[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS0[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS0[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS0[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS0[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS0[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS0[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS0[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS0[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS0[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS0[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS0[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS0[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS0[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS0[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS0[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS0[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS0[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS0[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS0[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS0[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS0[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS0[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS0[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS0[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS0[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS0[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS0[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS0[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS0[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS0[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS0[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS0[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS0[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS0[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS0[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS0[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS0[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS0[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS0[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS0[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS0[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS1[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS1[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS1[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS1[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS1[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS1[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS1[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS1[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS1[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS1[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS1[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS1[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS1[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS1[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS1[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS1[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS1[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS1[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS1[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS1[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS1[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS1[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS1[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS1[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS1[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS1[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS1[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS1[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS1[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS1[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS1[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS1[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS1[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS1[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS1[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS1[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS1[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS1[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS1[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS1[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS1[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS1[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS1[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS1[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS1[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS1[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS1[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS1[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS1[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS1[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS1[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS1[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS1[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS1[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS1[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS1[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS1[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS1[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS1[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS1[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS1[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS1[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS1[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS1[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS2[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS2[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS2[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS2[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS2[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS2[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS2[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS2[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS2[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS2[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS2[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS2[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS2[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS2[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS2[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS2[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS2[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS2[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS2[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS2[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS2[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS2[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS2[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS2[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS2[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS2[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS2[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS2[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS2[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS2[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS2[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS2[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS2[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS2[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS2[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS2[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS2[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS2[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS2[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS2[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS2[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS2[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS2[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS2[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS2[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS2[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS2[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS2[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS2[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS2[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS2[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS2[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS2[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS2[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS2[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS2[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS2[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS2[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS2[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS2[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS2[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS2[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS2[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS2[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS3[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS3[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS3[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS3[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS3[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS3[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS3[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS3[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS3[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS3[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS3[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS3[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS3[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS3[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS3[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS3[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS3[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS3[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS3[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS3[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS3[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS3[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS3[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS3[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS3[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS3[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS3[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS3[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS3[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS3[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS3[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS3[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS3[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS3[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS3[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS3[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS3[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS3[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS3[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS3[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS3[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS3[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS3[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS3[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS3[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS3[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS3[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS3[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS3[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS3[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS3[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS3[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS3[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS3[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS3[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS3[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS3[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS3[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS3[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS3[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS3[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS3[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS3[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS3[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS4[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS4[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS4[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS4[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS4[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS4[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS4[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS4[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS4[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS4[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS4[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS4[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS4[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS4[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS4[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS4[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS4[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS4[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS4[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS4[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS4[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS4[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS4[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS4[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS4[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS4[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS4[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS4[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS4[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS4[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS4[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS4[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS4[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS4[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS4[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS4[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS4[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS4[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS4[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS4[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS4[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS4[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS4[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS4[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS4[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS4[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS4[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS4[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS4[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS4[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS4[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS4[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS4[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS4[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS4[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS4[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS4[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS4[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS4[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS4[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS4[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS4[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS4[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS4[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS5[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS5[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS5[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS5[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS5[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS5[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS5[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS5[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS5[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS5[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS5[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS5[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS5[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS5[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS5[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS5[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS5[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS5[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS5[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS5[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS5[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS5[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS5[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS5[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS5[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS5[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS5[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS5[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS5[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS5[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS5[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS5[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS5[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS5[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS5[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS5[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS5[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS5[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS5[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS5[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS5[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS5[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS5[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS5[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS5[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS5[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS5[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS5[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS5[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS5[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS5[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS5[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS5[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS5[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS5[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS5[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS5[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS5[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS5[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS5[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS5[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS5[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS5[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS5[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS6[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS6[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS6[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS6[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS6[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS6[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS6[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS6[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS6[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS6[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS6[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS6[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS6[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS6[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS6[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS6[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS6[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS6[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS6[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS6[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS6[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS6[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS6[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS6[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS6[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS6[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS6[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS6[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS6[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS6[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS6[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS6[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS6[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS6[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS6[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS6[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS6[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS6[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS6[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS6[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS6[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS6[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS6[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS6[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS6[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS6[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS6[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS6[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS6[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS6[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS6[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS6[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS6[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS6[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS6[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS6[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS6[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS6[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS6[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS6[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS6[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS6[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS6[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS6[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS7[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS7[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS7[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS7[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS7[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS7[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS7[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS7[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS7[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS7[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS7[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS7[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS7[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS7[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS7[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS7[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS7[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS7[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS7[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS7[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS7[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS7[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS7[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS7[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS7[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS7[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS7[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS7[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS7[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS7[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS7[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS7[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS7[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS7[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS7[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS7[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS7[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS7[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS7[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS7[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS7[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS7[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS7[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS7[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS7[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS7[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS7[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS7[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS7[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS7[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS7[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS7[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS7[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS7[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS7[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS7[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS7[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS7[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS7[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS7[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS7[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS7[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS7[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS7[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		
		index2 += 2;
	}
	return;
}
void acts_all::MEMACCESS_readmanyvmasks9(bool_type enable, uint512_dt * kvdram, unit1_type vmaskBITS0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS1[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS2[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS3[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS4[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS5[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS6[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS7[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS8[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount1 = BLOCKRAM_SIZE;
	analysis_type analysis_loopcount2 = BLOCKRAM_SIZE / 16;
	analysis_type analysis_loopcount3 = BLOCKRAM_SIZE;
	
	buffer_type transfsize = size_kvs * 16;
	uint32_type bitsbuffer[MAXREDUCEBUFFERSZ];

	READVMASKS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount1 avg=analysis_loopcount1
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		tempbuffer[0][i] = kvdram[offset_kvs + i].range(31, 0); 
		tempbuffer[1][i] = kvdram[offset_kvs + i].range(63, 32); 
		tempbuffer[2][i] = kvdram[offset_kvs + i].range(95, 64); 
		tempbuffer[3][i] = kvdram[offset_kvs + i].range(127, 96); 
		tempbuffer[4][i] = kvdram[offset_kvs + i].range(159, 128); 
		tempbuffer[5][i] = kvdram[offset_kvs + i].range(191, 160); 
		tempbuffer[6][i] = kvdram[offset_kvs + i].range(223, 192); 
		tempbuffer[7][i] = kvdram[offset_kvs + i].range(255, 224); 
		tempbuffer[8][i] = kvdram[offset_kvs + i].range(287, 256); 
		tempbuffer[9][i] = kvdram[offset_kvs + i].range(319, 288); 
		tempbuffer[10][i] = kvdram[offset_kvs + i].range(351, 320); 
		tempbuffer[11][i] = kvdram[offset_kvs + i].range(383, 352); 
		tempbuffer[12][i] = kvdram[offset_kvs + i].range(415, 384); 
		tempbuffer[13][i] = kvdram[offset_kvs + i].range(447, 416); 
		tempbuffer[14][i] = kvdram[offset_kvs + i].range(479, 448); 
		tempbuffer[15][i] = kvdram[offset_kvs + i].range(511, 480); 
		#else 
		tempbuffer[0][i].key = kvdram[offset_kvs + i].data[0].key;
		tempbuffer[0][i].value = kvdram[offset_kvs + i].data[0].value; 
		tempbuffer[1][i].key = kvdram[offset_kvs + i].data[1].key;
		tempbuffer[1][i].value = kvdram[offset_kvs + i].data[1].value; 
		tempbuffer[2][i].key = kvdram[offset_kvs + i].data[2].key;
		tempbuffer[2][i].value = kvdram[offset_kvs + i].data[2].value; 
		tempbuffer[3][i].key = kvdram[offset_kvs + i].data[3].key;
		tempbuffer[3][i].value = kvdram[offset_kvs + i].data[3].value; 
		tempbuffer[4][i].key = kvdram[offset_kvs + i].data[4].key;
		tempbuffer[4][i].value = kvdram[offset_kvs + i].data[4].value; 
		tempbuffer[5][i].key = kvdram[offset_kvs + i].data[5].key;
		tempbuffer[5][i].value = kvdram[offset_kvs + i].data[5].value; 
		tempbuffer[6][i].key = kvdram[offset_kvs + i].data[6].key;
		tempbuffer[6][i].value = kvdram[offset_kvs + i].data[6].value; 
		tempbuffer[7][i].key = kvdram[offset_kvs + i].data[7].key;
		tempbuffer[7][i].value = kvdram[offset_kvs + i].data[7].value; 
		
		#endif 
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	
	uint32_type D0;
	uint32_type D1;
	uint32_type D2;
	uint32_type D3;
	uint32_type D4;
	uint32_type D5;
	uint32_type D6;
	uint32_type D7;
	uint32_type D8;
	uint32_type D9;
	uint32_type D10;
	uint32_type D11;
	uint32_type D12;
	uint32_type D13;
	uint32_type D14;
	uint32_type D15;
	buffer_type index = 0;
	LOADVMASKS_LOOP1: for (buffer_type i=0; i<size_kvs; i++){
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
		bitsbuffer[index + 1] = tempbuffer[0][i].value;
		bitsbuffer[index + 2] = tempbuffer[1][i].key;
		bitsbuffer[index + 3] = tempbuffer[1][i].value;
		bitsbuffer[index + 4] = tempbuffer[2][i].key;
		bitsbuffer[index + 5] = tempbuffer[2][i].value;
		bitsbuffer[index + 6] = tempbuffer[3][i].key;
		bitsbuffer[index + 7] = tempbuffer[3][i].value;
		bitsbuffer[index + 8] = tempbuffer[4][i].key;
		bitsbuffer[index + 9] = tempbuffer[4][i].value;
		bitsbuffer[index + 10] = tempbuffer[5][i].key;
		bitsbuffer[index + 11] = tempbuffer[5][i].value;
		bitsbuffer[index + 12] = tempbuffer[6][i].key;
		bitsbuffer[index + 13] = tempbuffer[6][i].value;
		bitsbuffer[index + 14] = tempbuffer[7][i].key;
		bitsbuffer[index + 15] = tempbuffer[7][i].value;
		#endif
		
		index += VECTOR_SIZE * 2;
	}

	// u32 => u16bits	
	buffer_type index2 = 0;
	LOADVMASKS_LOOP2: for (buffer_type i=0; i<transfsize; i++){ // transfsize, reducebuffersz
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount3 avg=analysis_loopcount3
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		vmaskBITS0[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS0[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS0[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS0[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS0[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS0[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS0[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS0[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS0[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS0[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS0[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS0[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS0[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS0[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS0[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS0[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS0[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS0[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS0[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS0[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS0[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS0[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS0[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS0[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS0[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS0[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS0[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS0[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS0[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS0[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS0[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS0[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS0[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS0[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS0[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS0[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS0[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS0[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS0[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS0[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS0[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS0[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS0[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS0[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS0[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS0[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS0[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS0[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS0[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS0[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS0[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS0[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS0[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS0[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS0[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS0[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS0[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS0[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS0[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS0[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS0[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS0[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS0[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS0[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS1[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS1[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS1[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS1[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS1[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS1[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS1[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS1[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS1[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS1[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS1[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS1[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS1[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS1[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS1[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS1[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS1[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS1[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS1[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS1[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS1[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS1[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS1[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS1[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS1[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS1[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS1[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS1[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS1[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS1[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS1[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS1[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS1[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS1[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS1[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS1[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS1[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS1[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS1[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS1[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS1[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS1[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS1[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS1[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS1[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS1[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS1[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS1[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS1[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS1[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS1[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS1[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS1[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS1[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS1[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS1[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS1[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS1[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS1[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS1[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS1[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS1[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS1[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS1[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS2[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS2[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS2[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS2[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS2[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS2[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS2[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS2[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS2[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS2[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS2[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS2[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS2[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS2[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS2[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS2[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS2[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS2[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS2[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS2[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS2[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS2[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS2[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS2[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS2[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS2[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS2[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS2[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS2[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS2[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS2[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS2[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS2[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS2[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS2[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS2[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS2[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS2[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS2[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS2[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS2[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS2[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS2[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS2[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS2[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS2[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS2[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS2[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS2[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS2[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS2[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS2[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS2[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS2[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS2[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS2[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS2[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS2[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS2[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS2[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS2[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS2[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS2[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS2[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS3[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS3[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS3[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS3[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS3[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS3[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS3[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS3[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS3[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS3[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS3[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS3[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS3[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS3[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS3[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS3[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS3[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS3[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS3[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS3[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS3[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS3[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS3[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS3[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS3[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS3[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS3[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS3[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS3[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS3[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS3[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS3[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS3[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS3[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS3[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS3[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS3[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS3[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS3[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS3[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS3[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS3[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS3[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS3[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS3[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS3[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS3[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS3[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS3[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS3[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS3[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS3[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS3[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS3[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS3[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS3[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS3[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS3[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS3[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS3[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS3[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS3[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS3[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS3[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS4[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS4[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS4[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS4[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS4[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS4[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS4[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS4[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS4[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS4[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS4[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS4[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS4[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS4[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS4[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS4[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS4[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS4[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS4[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS4[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS4[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS4[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS4[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS4[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS4[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS4[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS4[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS4[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS4[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS4[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS4[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS4[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS4[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS4[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS4[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS4[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS4[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS4[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS4[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS4[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS4[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS4[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS4[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS4[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS4[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS4[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS4[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS4[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS4[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS4[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS4[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS4[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS4[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS4[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS4[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS4[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS4[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS4[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS4[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS4[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS4[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS4[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS4[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS4[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS5[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS5[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS5[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS5[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS5[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS5[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS5[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS5[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS5[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS5[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS5[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS5[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS5[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS5[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS5[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS5[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS5[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS5[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS5[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS5[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS5[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS5[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS5[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS5[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS5[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS5[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS5[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS5[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS5[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS5[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS5[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS5[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS5[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS5[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS5[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS5[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS5[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS5[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS5[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS5[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS5[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS5[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS5[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS5[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS5[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS5[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS5[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS5[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS5[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS5[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS5[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS5[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS5[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS5[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS5[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS5[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS5[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS5[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS5[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS5[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS5[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS5[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS5[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS5[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS6[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS6[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS6[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS6[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS6[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS6[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS6[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS6[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS6[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS6[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS6[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS6[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS6[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS6[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS6[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS6[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS6[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS6[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS6[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS6[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS6[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS6[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS6[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS6[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS6[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS6[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS6[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS6[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS6[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS6[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS6[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS6[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS6[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS6[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS6[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS6[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS6[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS6[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS6[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS6[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS6[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS6[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS6[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS6[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS6[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS6[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS6[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS6[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS6[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS6[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS6[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS6[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS6[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS6[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS6[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS6[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS6[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS6[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS6[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS6[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS6[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS6[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS6[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS6[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS7[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS7[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS7[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS7[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS7[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS7[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS7[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS7[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS7[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS7[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS7[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS7[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS7[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS7[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS7[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS7[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS7[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS7[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS7[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS7[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS7[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS7[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS7[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS7[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS7[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS7[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS7[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS7[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS7[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS7[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS7[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS7[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS7[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS7[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS7[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS7[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS7[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS7[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS7[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS7[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS7[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS7[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS7[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS7[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS7[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS7[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS7[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS7[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS7[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS7[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS7[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS7[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS7[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS7[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS7[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS7[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS7[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS7[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS7[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS7[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS7[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS7[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS7[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS7[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS8[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS8[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS8[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS8[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS8[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS8[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS8[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS8[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS8[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS8[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS8[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS8[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS8[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS8[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS8[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS8[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS8[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS8[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS8[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS8[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS8[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS8[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS8[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS8[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS8[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS8[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS8[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS8[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS8[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS8[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS8[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS8[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS8[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS8[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS8[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS8[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS8[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS8[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS8[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS8[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS8[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS8[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS8[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS8[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS8[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS8[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS8[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS8[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS8[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS8[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS8[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS8[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS8[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS8[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS8[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS8[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS8[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS8[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS8[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS8[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS8[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS8[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS8[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS8[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		
		index2 += 2;
	}
	return;
}
void acts_all::MEMACCESS_readmanyvmasks10(bool_type enable, uint512_dt * kvdram, unit1_type vmaskBITS0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS1[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS2[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS3[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS4[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS5[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS6[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS7[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS8[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS9[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount1 = BLOCKRAM_SIZE;
	analysis_type analysis_loopcount2 = BLOCKRAM_SIZE / 16;
	analysis_type analysis_loopcount3 = BLOCKRAM_SIZE;
	
	buffer_type transfsize = size_kvs * 16;
	uint32_type bitsbuffer[MAXREDUCEBUFFERSZ];

	READVMASKS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount1 avg=analysis_loopcount1
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		tempbuffer[0][i] = kvdram[offset_kvs + i].range(31, 0); 
		tempbuffer[1][i] = kvdram[offset_kvs + i].range(63, 32); 
		tempbuffer[2][i] = kvdram[offset_kvs + i].range(95, 64); 
		tempbuffer[3][i] = kvdram[offset_kvs + i].range(127, 96); 
		tempbuffer[4][i] = kvdram[offset_kvs + i].range(159, 128); 
		tempbuffer[5][i] = kvdram[offset_kvs + i].range(191, 160); 
		tempbuffer[6][i] = kvdram[offset_kvs + i].range(223, 192); 
		tempbuffer[7][i] = kvdram[offset_kvs + i].range(255, 224); 
		tempbuffer[8][i] = kvdram[offset_kvs + i].range(287, 256); 
		tempbuffer[9][i] = kvdram[offset_kvs + i].range(319, 288); 
		tempbuffer[10][i] = kvdram[offset_kvs + i].range(351, 320); 
		tempbuffer[11][i] = kvdram[offset_kvs + i].range(383, 352); 
		tempbuffer[12][i] = kvdram[offset_kvs + i].range(415, 384); 
		tempbuffer[13][i] = kvdram[offset_kvs + i].range(447, 416); 
		tempbuffer[14][i] = kvdram[offset_kvs + i].range(479, 448); 
		tempbuffer[15][i] = kvdram[offset_kvs + i].range(511, 480); 
		#else 
		tempbuffer[0][i].key = kvdram[offset_kvs + i].data[0].key;
		tempbuffer[0][i].value = kvdram[offset_kvs + i].data[0].value; 
		tempbuffer[1][i].key = kvdram[offset_kvs + i].data[1].key;
		tempbuffer[1][i].value = kvdram[offset_kvs + i].data[1].value; 
		tempbuffer[2][i].key = kvdram[offset_kvs + i].data[2].key;
		tempbuffer[2][i].value = kvdram[offset_kvs + i].data[2].value; 
		tempbuffer[3][i].key = kvdram[offset_kvs + i].data[3].key;
		tempbuffer[3][i].value = kvdram[offset_kvs + i].data[3].value; 
		tempbuffer[4][i].key = kvdram[offset_kvs + i].data[4].key;
		tempbuffer[4][i].value = kvdram[offset_kvs + i].data[4].value; 
		tempbuffer[5][i].key = kvdram[offset_kvs + i].data[5].key;
		tempbuffer[5][i].value = kvdram[offset_kvs + i].data[5].value; 
		tempbuffer[6][i].key = kvdram[offset_kvs + i].data[6].key;
		tempbuffer[6][i].value = kvdram[offset_kvs + i].data[6].value; 
		tempbuffer[7][i].key = kvdram[offset_kvs + i].data[7].key;
		tempbuffer[7][i].value = kvdram[offset_kvs + i].data[7].value; 
		
		#endif 
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	
	uint32_type D0;
	uint32_type D1;
	uint32_type D2;
	uint32_type D3;
	uint32_type D4;
	uint32_type D5;
	uint32_type D6;
	uint32_type D7;
	uint32_type D8;
	uint32_type D9;
	uint32_type D10;
	uint32_type D11;
	uint32_type D12;
	uint32_type D13;
	uint32_type D14;
	uint32_type D15;
	buffer_type index = 0;
	LOADVMASKS_LOOP1: for (buffer_type i=0; i<size_kvs; i++){
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
		bitsbuffer[index + 1] = tempbuffer[0][i].value;
		bitsbuffer[index + 2] = tempbuffer[1][i].key;
		bitsbuffer[index + 3] = tempbuffer[1][i].value;
		bitsbuffer[index + 4] = tempbuffer[2][i].key;
		bitsbuffer[index + 5] = tempbuffer[2][i].value;
		bitsbuffer[index + 6] = tempbuffer[3][i].key;
		bitsbuffer[index + 7] = tempbuffer[3][i].value;
		bitsbuffer[index + 8] = tempbuffer[4][i].key;
		bitsbuffer[index + 9] = tempbuffer[4][i].value;
		bitsbuffer[index + 10] = tempbuffer[5][i].key;
		bitsbuffer[index + 11] = tempbuffer[5][i].value;
		bitsbuffer[index + 12] = tempbuffer[6][i].key;
		bitsbuffer[index + 13] = tempbuffer[6][i].value;
		bitsbuffer[index + 14] = tempbuffer[7][i].key;
		bitsbuffer[index + 15] = tempbuffer[7][i].value;
		#endif
		
		index += VECTOR_SIZE * 2;
	}

	// u32 => u16bits	
	buffer_type index2 = 0;
	LOADVMASKS_LOOP2: for (buffer_type i=0; i<transfsize; i++){ // transfsize, reducebuffersz
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount3 avg=analysis_loopcount3
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		vmaskBITS0[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS0[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS0[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS0[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS0[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS0[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS0[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS0[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS0[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS0[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS0[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS0[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS0[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS0[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS0[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS0[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS0[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS0[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS0[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS0[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS0[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS0[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS0[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS0[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS0[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS0[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS0[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS0[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS0[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS0[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS0[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS0[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS0[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS0[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS0[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS0[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS0[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS0[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS0[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS0[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS0[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS0[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS0[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS0[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS0[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS0[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS0[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS0[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS0[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS0[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS0[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS0[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS0[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS0[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS0[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS0[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS0[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS0[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS0[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS0[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS0[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS0[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS0[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS0[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS1[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS1[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS1[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS1[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS1[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS1[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS1[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS1[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS1[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS1[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS1[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS1[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS1[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS1[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS1[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS1[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS1[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS1[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS1[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS1[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS1[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS1[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS1[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS1[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS1[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS1[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS1[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS1[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS1[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS1[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS1[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS1[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS1[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS1[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS1[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS1[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS1[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS1[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS1[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS1[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS1[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS1[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS1[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS1[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS1[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS1[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS1[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS1[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS1[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS1[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS1[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS1[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS1[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS1[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS1[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS1[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS1[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS1[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS1[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS1[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS1[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS1[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS1[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS1[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS2[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS2[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS2[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS2[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS2[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS2[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS2[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS2[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS2[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS2[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS2[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS2[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS2[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS2[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS2[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS2[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS2[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS2[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS2[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS2[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS2[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS2[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS2[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS2[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS2[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS2[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS2[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS2[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS2[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS2[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS2[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS2[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS2[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS2[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS2[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS2[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS2[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS2[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS2[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS2[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS2[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS2[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS2[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS2[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS2[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS2[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS2[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS2[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS2[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS2[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS2[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS2[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS2[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS2[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS2[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS2[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS2[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS2[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS2[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS2[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS2[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS2[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS2[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS2[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS3[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS3[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS3[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS3[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS3[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS3[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS3[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS3[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS3[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS3[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS3[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS3[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS3[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS3[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS3[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS3[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS3[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS3[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS3[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS3[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS3[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS3[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS3[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS3[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS3[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS3[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS3[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS3[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS3[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS3[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS3[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS3[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS3[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS3[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS3[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS3[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS3[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS3[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS3[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS3[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS3[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS3[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS3[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS3[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS3[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS3[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS3[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS3[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS3[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS3[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS3[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS3[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS3[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS3[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS3[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS3[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS3[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS3[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS3[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS3[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS3[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS3[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS3[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS3[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS4[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS4[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS4[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS4[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS4[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS4[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS4[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS4[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS4[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS4[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS4[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS4[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS4[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS4[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS4[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS4[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS4[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS4[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS4[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS4[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS4[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS4[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS4[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS4[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS4[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS4[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS4[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS4[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS4[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS4[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS4[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS4[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS4[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS4[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS4[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS4[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS4[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS4[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS4[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS4[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS4[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS4[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS4[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS4[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS4[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS4[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS4[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS4[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS4[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS4[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS4[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS4[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS4[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS4[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS4[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS4[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS4[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS4[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS4[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS4[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS4[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS4[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS4[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS4[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS5[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS5[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS5[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS5[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS5[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS5[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS5[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS5[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS5[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS5[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS5[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS5[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS5[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS5[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS5[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS5[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS5[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS5[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS5[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS5[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS5[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS5[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS5[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS5[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS5[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS5[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS5[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS5[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS5[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS5[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS5[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS5[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS5[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS5[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS5[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS5[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS5[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS5[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS5[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS5[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS5[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS5[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS5[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS5[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS5[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS5[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS5[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS5[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS5[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS5[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS5[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS5[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS5[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS5[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS5[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS5[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS5[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS5[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS5[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS5[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS5[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS5[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS5[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS5[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS6[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS6[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS6[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS6[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS6[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS6[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS6[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS6[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS6[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS6[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS6[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS6[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS6[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS6[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS6[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS6[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS6[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS6[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS6[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS6[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS6[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS6[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS6[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS6[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS6[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS6[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS6[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS6[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS6[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS6[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS6[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS6[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS6[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS6[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS6[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS6[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS6[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS6[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS6[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS6[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS6[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS6[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS6[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS6[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS6[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS6[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS6[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS6[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS6[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS6[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS6[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS6[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS6[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS6[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS6[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS6[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS6[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS6[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS6[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS6[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS6[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS6[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS6[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS6[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS7[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS7[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS7[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS7[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS7[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS7[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS7[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS7[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS7[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS7[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS7[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS7[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS7[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS7[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS7[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS7[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS7[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS7[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS7[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS7[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS7[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS7[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS7[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS7[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS7[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS7[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS7[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS7[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS7[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS7[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS7[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS7[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS7[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS7[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS7[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS7[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS7[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS7[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS7[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS7[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS7[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS7[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS7[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS7[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS7[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS7[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS7[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS7[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS7[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS7[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS7[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS7[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS7[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS7[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS7[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS7[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS7[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS7[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS7[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS7[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS7[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS7[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS7[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS7[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS8[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS8[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS8[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS8[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS8[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS8[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS8[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS8[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS8[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS8[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS8[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS8[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS8[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS8[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS8[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS8[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS8[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS8[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS8[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS8[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS8[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS8[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS8[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS8[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS8[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS8[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS8[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS8[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS8[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS8[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS8[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS8[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS8[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS8[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS8[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS8[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS8[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS8[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS8[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS8[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS8[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS8[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS8[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS8[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS8[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS8[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS8[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS8[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS8[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS8[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS8[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS8[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS8[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS8[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS8[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS8[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS8[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS8[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS8[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS8[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS8[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS8[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS8[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS8[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS9[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS9[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS9[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS9[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS9[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS9[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS9[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS9[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS9[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS9[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS9[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS9[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS9[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS9[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS9[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS9[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS9[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS9[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS9[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS9[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS9[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS9[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS9[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS9[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS9[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS9[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS9[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS9[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS9[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS9[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS9[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS9[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS9[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS9[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS9[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS9[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS9[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS9[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS9[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS9[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS9[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS9[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS9[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS9[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS9[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS9[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS9[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS9[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS9[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS9[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS9[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS9[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS9[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS9[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS9[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS9[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS9[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS9[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS9[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS9[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS9[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS9[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS9[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS9[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		
		index2 += 2;
	}
	return;
}
void acts_all::MEMACCESS_readmanyvmasks11(bool_type enable, uint512_dt * kvdram, unit1_type vmaskBITS0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS1[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS2[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS3[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS4[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS5[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS6[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS7[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS8[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS9[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS10[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount1 = BLOCKRAM_SIZE;
	analysis_type analysis_loopcount2 = BLOCKRAM_SIZE / 16;
	analysis_type analysis_loopcount3 = BLOCKRAM_SIZE;
	
	buffer_type transfsize = size_kvs * 16;
	uint32_type bitsbuffer[MAXREDUCEBUFFERSZ];

	READVMASKS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount1 avg=analysis_loopcount1
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		tempbuffer[0][i] = kvdram[offset_kvs + i].range(31, 0); 
		tempbuffer[1][i] = kvdram[offset_kvs + i].range(63, 32); 
		tempbuffer[2][i] = kvdram[offset_kvs + i].range(95, 64); 
		tempbuffer[3][i] = kvdram[offset_kvs + i].range(127, 96); 
		tempbuffer[4][i] = kvdram[offset_kvs + i].range(159, 128); 
		tempbuffer[5][i] = kvdram[offset_kvs + i].range(191, 160); 
		tempbuffer[6][i] = kvdram[offset_kvs + i].range(223, 192); 
		tempbuffer[7][i] = kvdram[offset_kvs + i].range(255, 224); 
		tempbuffer[8][i] = kvdram[offset_kvs + i].range(287, 256); 
		tempbuffer[9][i] = kvdram[offset_kvs + i].range(319, 288); 
		tempbuffer[10][i] = kvdram[offset_kvs + i].range(351, 320); 
		tempbuffer[11][i] = kvdram[offset_kvs + i].range(383, 352); 
		tempbuffer[12][i] = kvdram[offset_kvs + i].range(415, 384); 
		tempbuffer[13][i] = kvdram[offset_kvs + i].range(447, 416); 
		tempbuffer[14][i] = kvdram[offset_kvs + i].range(479, 448); 
		tempbuffer[15][i] = kvdram[offset_kvs + i].range(511, 480); 
		#else 
		tempbuffer[0][i].key = kvdram[offset_kvs + i].data[0].key;
		tempbuffer[0][i].value = kvdram[offset_kvs + i].data[0].value; 
		tempbuffer[1][i].key = kvdram[offset_kvs + i].data[1].key;
		tempbuffer[1][i].value = kvdram[offset_kvs + i].data[1].value; 
		tempbuffer[2][i].key = kvdram[offset_kvs + i].data[2].key;
		tempbuffer[2][i].value = kvdram[offset_kvs + i].data[2].value; 
		tempbuffer[3][i].key = kvdram[offset_kvs + i].data[3].key;
		tempbuffer[3][i].value = kvdram[offset_kvs + i].data[3].value; 
		tempbuffer[4][i].key = kvdram[offset_kvs + i].data[4].key;
		tempbuffer[4][i].value = kvdram[offset_kvs + i].data[4].value; 
		tempbuffer[5][i].key = kvdram[offset_kvs + i].data[5].key;
		tempbuffer[5][i].value = kvdram[offset_kvs + i].data[5].value; 
		tempbuffer[6][i].key = kvdram[offset_kvs + i].data[6].key;
		tempbuffer[6][i].value = kvdram[offset_kvs + i].data[6].value; 
		tempbuffer[7][i].key = kvdram[offset_kvs + i].data[7].key;
		tempbuffer[7][i].value = kvdram[offset_kvs + i].data[7].value; 
		
		#endif 
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	
	uint32_type D0;
	uint32_type D1;
	uint32_type D2;
	uint32_type D3;
	uint32_type D4;
	uint32_type D5;
	uint32_type D6;
	uint32_type D7;
	uint32_type D8;
	uint32_type D9;
	uint32_type D10;
	uint32_type D11;
	uint32_type D12;
	uint32_type D13;
	uint32_type D14;
	uint32_type D15;
	buffer_type index = 0;
	LOADVMASKS_LOOP1: for (buffer_type i=0; i<size_kvs; i++){
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
		bitsbuffer[index + 1] = tempbuffer[0][i].value;
		bitsbuffer[index + 2] = tempbuffer[1][i].key;
		bitsbuffer[index + 3] = tempbuffer[1][i].value;
		bitsbuffer[index + 4] = tempbuffer[2][i].key;
		bitsbuffer[index + 5] = tempbuffer[2][i].value;
		bitsbuffer[index + 6] = tempbuffer[3][i].key;
		bitsbuffer[index + 7] = tempbuffer[3][i].value;
		bitsbuffer[index + 8] = tempbuffer[4][i].key;
		bitsbuffer[index + 9] = tempbuffer[4][i].value;
		bitsbuffer[index + 10] = tempbuffer[5][i].key;
		bitsbuffer[index + 11] = tempbuffer[5][i].value;
		bitsbuffer[index + 12] = tempbuffer[6][i].key;
		bitsbuffer[index + 13] = tempbuffer[6][i].value;
		bitsbuffer[index + 14] = tempbuffer[7][i].key;
		bitsbuffer[index + 15] = tempbuffer[7][i].value;
		#endif
		
		index += VECTOR_SIZE * 2;
	}

	// u32 => u16bits	
	buffer_type index2 = 0;
	LOADVMASKS_LOOP2: for (buffer_type i=0; i<transfsize; i++){ // transfsize, reducebuffersz
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount3 avg=analysis_loopcount3
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		vmaskBITS0[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS0[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS0[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS0[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS0[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS0[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS0[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS0[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS0[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS0[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS0[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS0[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS0[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS0[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS0[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS0[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS0[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS0[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS0[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS0[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS0[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS0[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS0[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS0[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS0[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS0[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS0[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS0[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS0[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS0[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS0[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS0[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS0[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS0[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS0[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS0[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS0[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS0[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS0[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS0[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS0[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS0[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS0[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS0[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS0[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS0[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS0[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS0[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS0[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS0[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS0[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS0[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS0[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS0[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS0[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS0[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS0[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS0[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS0[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS0[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS0[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS0[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS0[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS0[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS1[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS1[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS1[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS1[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS1[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS1[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS1[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS1[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS1[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS1[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS1[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS1[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS1[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS1[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS1[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS1[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS1[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS1[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS1[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS1[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS1[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS1[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS1[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS1[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS1[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS1[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS1[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS1[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS1[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS1[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS1[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS1[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS1[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS1[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS1[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS1[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS1[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS1[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS1[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS1[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS1[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS1[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS1[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS1[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS1[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS1[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS1[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS1[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS1[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS1[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS1[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS1[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS1[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS1[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS1[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS1[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS1[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS1[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS1[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS1[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS1[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS1[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS1[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS1[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS2[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS2[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS2[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS2[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS2[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS2[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS2[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS2[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS2[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS2[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS2[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS2[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS2[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS2[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS2[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS2[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS2[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS2[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS2[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS2[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS2[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS2[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS2[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS2[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS2[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS2[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS2[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS2[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS2[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS2[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS2[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS2[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS2[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS2[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS2[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS2[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS2[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS2[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS2[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS2[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS2[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS2[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS2[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS2[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS2[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS2[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS2[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS2[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS2[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS2[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS2[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS2[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS2[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS2[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS2[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS2[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS2[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS2[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS2[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS2[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS2[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS2[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS2[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS2[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS3[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS3[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS3[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS3[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS3[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS3[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS3[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS3[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS3[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS3[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS3[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS3[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS3[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS3[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS3[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS3[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS3[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS3[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS3[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS3[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS3[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS3[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS3[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS3[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS3[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS3[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS3[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS3[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS3[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS3[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS3[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS3[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS3[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS3[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS3[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS3[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS3[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS3[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS3[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS3[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS3[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS3[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS3[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS3[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS3[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS3[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS3[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS3[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS3[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS3[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS3[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS3[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS3[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS3[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS3[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS3[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS3[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS3[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS3[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS3[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS3[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS3[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS3[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS3[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS4[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS4[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS4[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS4[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS4[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS4[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS4[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS4[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS4[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS4[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS4[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS4[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS4[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS4[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS4[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS4[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS4[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS4[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS4[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS4[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS4[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS4[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS4[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS4[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS4[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS4[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS4[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS4[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS4[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS4[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS4[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS4[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS4[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS4[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS4[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS4[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS4[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS4[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS4[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS4[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS4[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS4[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS4[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS4[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS4[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS4[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS4[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS4[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS4[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS4[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS4[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS4[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS4[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS4[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS4[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS4[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS4[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS4[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS4[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS4[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS4[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS4[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS4[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS4[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS5[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS5[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS5[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS5[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS5[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS5[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS5[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS5[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS5[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS5[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS5[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS5[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS5[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS5[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS5[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS5[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS5[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS5[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS5[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS5[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS5[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS5[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS5[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS5[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS5[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS5[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS5[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS5[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS5[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS5[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS5[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS5[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS5[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS5[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS5[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS5[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS5[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS5[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS5[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS5[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS5[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS5[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS5[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS5[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS5[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS5[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS5[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS5[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS5[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS5[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS5[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS5[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS5[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS5[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS5[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS5[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS5[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS5[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS5[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS5[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS5[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS5[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS5[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS5[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS6[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS6[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS6[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS6[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS6[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS6[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS6[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS6[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS6[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS6[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS6[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS6[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS6[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS6[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS6[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS6[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS6[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS6[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS6[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS6[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS6[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS6[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS6[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS6[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS6[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS6[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS6[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS6[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS6[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS6[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS6[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS6[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS6[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS6[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS6[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS6[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS6[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS6[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS6[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS6[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS6[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS6[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS6[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS6[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS6[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS6[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS6[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS6[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS6[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS6[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS6[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS6[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS6[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS6[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS6[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS6[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS6[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS6[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS6[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS6[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS6[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS6[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS6[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS6[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS7[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS7[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS7[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS7[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS7[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS7[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS7[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS7[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS7[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS7[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS7[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS7[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS7[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS7[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS7[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS7[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS7[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS7[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS7[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS7[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS7[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS7[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS7[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS7[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS7[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS7[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS7[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS7[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS7[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS7[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS7[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS7[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS7[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS7[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS7[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS7[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS7[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS7[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS7[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS7[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS7[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS7[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS7[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS7[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS7[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS7[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS7[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS7[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS7[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS7[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS7[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS7[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS7[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS7[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS7[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS7[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS7[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS7[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS7[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS7[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS7[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS7[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS7[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS7[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS8[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS8[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS8[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS8[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS8[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS8[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS8[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS8[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS8[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS8[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS8[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS8[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS8[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS8[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS8[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS8[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS8[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS8[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS8[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS8[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS8[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS8[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS8[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS8[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS8[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS8[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS8[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS8[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS8[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS8[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS8[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS8[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS8[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS8[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS8[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS8[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS8[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS8[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS8[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS8[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS8[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS8[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS8[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS8[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS8[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS8[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS8[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS8[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS8[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS8[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS8[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS8[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS8[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS8[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS8[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS8[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS8[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS8[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS8[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS8[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS8[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS8[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS8[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS8[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS9[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS9[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS9[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS9[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS9[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS9[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS9[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS9[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS9[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS9[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS9[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS9[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS9[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS9[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS9[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS9[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS9[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS9[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS9[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS9[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS9[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS9[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS9[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS9[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS9[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS9[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS9[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS9[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS9[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS9[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS9[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS9[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS9[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS9[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS9[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS9[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS9[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS9[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS9[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS9[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS9[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS9[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS9[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS9[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS9[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS9[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS9[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS9[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS9[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS9[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS9[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS9[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS9[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS9[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS9[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS9[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS9[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS9[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS9[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS9[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS9[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS9[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS9[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS9[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS10[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS10[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS10[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS10[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS10[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS10[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS10[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS10[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS10[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS10[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS10[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS10[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS10[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS10[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS10[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS10[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS10[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS10[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS10[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS10[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS10[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS10[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS10[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS10[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS10[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS10[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS10[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS10[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS10[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS10[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS10[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS10[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS10[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS10[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS10[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS10[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS10[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS10[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS10[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS10[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS10[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS10[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS10[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS10[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS10[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS10[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS10[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS10[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS10[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS10[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS10[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS10[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS10[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS10[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS10[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS10[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS10[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS10[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS10[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS10[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS10[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS10[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS10[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS10[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		
		index2 += 2;
	}
	return;
}
void acts_all::MEMACCESS_readmanyvmasks12(bool_type enable, uint512_dt * kvdram, unit1_type vmaskBITS0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS1[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS2[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS3[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS4[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS5[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS6[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS7[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS8[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS9[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS10[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmaskBITS11[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount1 = BLOCKRAM_SIZE;
	analysis_type analysis_loopcount2 = BLOCKRAM_SIZE / 16;
	analysis_type analysis_loopcount3 = BLOCKRAM_SIZE;
	
	buffer_type transfsize = size_kvs * 16;
	uint32_type bitsbuffer[MAXREDUCEBUFFERSZ];

	READVMASKS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount1 avg=analysis_loopcount1
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		tempbuffer[0][i] = kvdram[offset_kvs + i].range(31, 0); 
		tempbuffer[1][i] = kvdram[offset_kvs + i].range(63, 32); 
		tempbuffer[2][i] = kvdram[offset_kvs + i].range(95, 64); 
		tempbuffer[3][i] = kvdram[offset_kvs + i].range(127, 96); 
		tempbuffer[4][i] = kvdram[offset_kvs + i].range(159, 128); 
		tempbuffer[5][i] = kvdram[offset_kvs + i].range(191, 160); 
		tempbuffer[6][i] = kvdram[offset_kvs + i].range(223, 192); 
		tempbuffer[7][i] = kvdram[offset_kvs + i].range(255, 224); 
		tempbuffer[8][i] = kvdram[offset_kvs + i].range(287, 256); 
		tempbuffer[9][i] = kvdram[offset_kvs + i].range(319, 288); 
		tempbuffer[10][i] = kvdram[offset_kvs + i].range(351, 320); 
		tempbuffer[11][i] = kvdram[offset_kvs + i].range(383, 352); 
		tempbuffer[12][i] = kvdram[offset_kvs + i].range(415, 384); 
		tempbuffer[13][i] = kvdram[offset_kvs + i].range(447, 416); 
		tempbuffer[14][i] = kvdram[offset_kvs + i].range(479, 448); 
		tempbuffer[15][i] = kvdram[offset_kvs + i].range(511, 480); 
		#else 
		tempbuffer[0][i].key = kvdram[offset_kvs + i].data[0].key;
		tempbuffer[0][i].value = kvdram[offset_kvs + i].data[0].value; 
		tempbuffer[1][i].key = kvdram[offset_kvs + i].data[1].key;
		tempbuffer[1][i].value = kvdram[offset_kvs + i].data[1].value; 
		tempbuffer[2][i].key = kvdram[offset_kvs + i].data[2].key;
		tempbuffer[2][i].value = kvdram[offset_kvs + i].data[2].value; 
		tempbuffer[3][i].key = kvdram[offset_kvs + i].data[3].key;
		tempbuffer[3][i].value = kvdram[offset_kvs + i].data[3].value; 
		tempbuffer[4][i].key = kvdram[offset_kvs + i].data[4].key;
		tempbuffer[4][i].value = kvdram[offset_kvs + i].data[4].value; 
		tempbuffer[5][i].key = kvdram[offset_kvs + i].data[5].key;
		tempbuffer[5][i].value = kvdram[offset_kvs + i].data[5].value; 
		tempbuffer[6][i].key = kvdram[offset_kvs + i].data[6].key;
		tempbuffer[6][i].value = kvdram[offset_kvs + i].data[6].value; 
		tempbuffer[7][i].key = kvdram[offset_kvs + i].data[7].key;
		tempbuffer[7][i].value = kvdram[offset_kvs + i].data[7].value; 
		
		#endif 
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	
	uint32_type D0;
	uint32_type D1;
	uint32_type D2;
	uint32_type D3;
	uint32_type D4;
	uint32_type D5;
	uint32_type D6;
	uint32_type D7;
	uint32_type D8;
	uint32_type D9;
	uint32_type D10;
	uint32_type D11;
	uint32_type D12;
	uint32_type D13;
	uint32_type D14;
	uint32_type D15;
	buffer_type index = 0;
	LOADVMASKS_LOOP1: for (buffer_type i=0; i<size_kvs; i++){
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
		bitsbuffer[index + 1] = tempbuffer[0][i].value;
		bitsbuffer[index + 2] = tempbuffer[1][i].key;
		bitsbuffer[index + 3] = tempbuffer[1][i].value;
		bitsbuffer[index + 4] = tempbuffer[2][i].key;
		bitsbuffer[index + 5] = tempbuffer[2][i].value;
		bitsbuffer[index + 6] = tempbuffer[3][i].key;
		bitsbuffer[index + 7] = tempbuffer[3][i].value;
		bitsbuffer[index + 8] = tempbuffer[4][i].key;
		bitsbuffer[index + 9] = tempbuffer[4][i].value;
		bitsbuffer[index + 10] = tempbuffer[5][i].key;
		bitsbuffer[index + 11] = tempbuffer[5][i].value;
		bitsbuffer[index + 12] = tempbuffer[6][i].key;
		bitsbuffer[index + 13] = tempbuffer[6][i].value;
		bitsbuffer[index + 14] = tempbuffer[7][i].key;
		bitsbuffer[index + 15] = tempbuffer[7][i].value;
		#endif
		
		index += VECTOR_SIZE * 2;
	}

	// u32 => u16bits	
	buffer_type index2 = 0;
	LOADVMASKS_LOOP2: for (buffer_type i=0; i<transfsize; i++){ // transfsize, reducebuffersz
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount3 avg=analysis_loopcount3
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		vmaskBITS0[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS0[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS0[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS0[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS0[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS0[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS0[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS0[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS0[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS0[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS0[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS0[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS0[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS0[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS0[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS0[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS0[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS0[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS0[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS0[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS0[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS0[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS0[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS0[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS0[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS0[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS0[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS0[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS0[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS0[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS0[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS0[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS0[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS0[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS0[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS0[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS0[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS0[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS0[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS0[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS0[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS0[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS0[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS0[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS0[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS0[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS0[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS0[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS0[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS0[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS0[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS0[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS0[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS0[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS0[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS0[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS0[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS0[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS0[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS0[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS0[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS0[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS0[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS0[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS1[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS1[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS1[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS1[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS1[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS1[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS1[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS1[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS1[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS1[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS1[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS1[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS1[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS1[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS1[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS1[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS1[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS1[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS1[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS1[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS1[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS1[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS1[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS1[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS1[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS1[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS1[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS1[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS1[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS1[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS1[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS1[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS1[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS1[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS1[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS1[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS1[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS1[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS1[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS1[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS1[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS1[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS1[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS1[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS1[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS1[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS1[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS1[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS1[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS1[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS1[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS1[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS1[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS1[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS1[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS1[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS1[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS1[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS1[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS1[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS1[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS1[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS1[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS1[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS2[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS2[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS2[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS2[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS2[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS2[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS2[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS2[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS2[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS2[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS2[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS2[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS2[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS2[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS2[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS2[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS2[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS2[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS2[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS2[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS2[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS2[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS2[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS2[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS2[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS2[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS2[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS2[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS2[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS2[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS2[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS2[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS2[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS2[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS2[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS2[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS2[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS2[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS2[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS2[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS2[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS2[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS2[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS2[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS2[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS2[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS2[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS2[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS2[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS2[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS2[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS2[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS2[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS2[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS2[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS2[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS2[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS2[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS2[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS2[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS2[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS2[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS2[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS2[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS3[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS3[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS3[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS3[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS3[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS3[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS3[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS3[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS3[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS3[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS3[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS3[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS3[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS3[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS3[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS3[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS3[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS3[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS3[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS3[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS3[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS3[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS3[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS3[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS3[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS3[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS3[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS3[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS3[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS3[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS3[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS3[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS3[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS3[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS3[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS3[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS3[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS3[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS3[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS3[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS3[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS3[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS3[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS3[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS3[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS3[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS3[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS3[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS3[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS3[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS3[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS3[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS3[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS3[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS3[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS3[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS3[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS3[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS3[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS3[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS3[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS3[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS3[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS3[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS4[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS4[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS4[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS4[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS4[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS4[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS4[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS4[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS4[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS4[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS4[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS4[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS4[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS4[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS4[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS4[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS4[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS4[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS4[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS4[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS4[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS4[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS4[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS4[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS4[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS4[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS4[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS4[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS4[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS4[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS4[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS4[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS4[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS4[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS4[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS4[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS4[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS4[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS4[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS4[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS4[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS4[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS4[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS4[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS4[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS4[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS4[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS4[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS4[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS4[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS4[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS4[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS4[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS4[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS4[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS4[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS4[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS4[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS4[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS4[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS4[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS4[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS4[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS4[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS5[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS5[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS5[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS5[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS5[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS5[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS5[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS5[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS5[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS5[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS5[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS5[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS5[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS5[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS5[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS5[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS5[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS5[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS5[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS5[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS5[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS5[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS5[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS5[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS5[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS5[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS5[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS5[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS5[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS5[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS5[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS5[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS5[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS5[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS5[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS5[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS5[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS5[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS5[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS5[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS5[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS5[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS5[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS5[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS5[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS5[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS5[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS5[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS5[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS5[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS5[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS5[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS5[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS5[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS5[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS5[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS5[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS5[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS5[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS5[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS5[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS5[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS5[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS5[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS6[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS6[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS6[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS6[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS6[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS6[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS6[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS6[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS6[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS6[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS6[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS6[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS6[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS6[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS6[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS6[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS6[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS6[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS6[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS6[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS6[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS6[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS6[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS6[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS6[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS6[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS6[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS6[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS6[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS6[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS6[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS6[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS6[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS6[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS6[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS6[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS6[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS6[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS6[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS6[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS6[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS6[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS6[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS6[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS6[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS6[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS6[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS6[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS6[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS6[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS6[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS6[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS6[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS6[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS6[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS6[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS6[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS6[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS6[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS6[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS6[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS6[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS6[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS6[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS7[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS7[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS7[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS7[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS7[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS7[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS7[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS7[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS7[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS7[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS7[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS7[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS7[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS7[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS7[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS7[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS7[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS7[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS7[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS7[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS7[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS7[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS7[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS7[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS7[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS7[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS7[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS7[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS7[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS7[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS7[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS7[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS7[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS7[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS7[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS7[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS7[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS7[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS7[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS7[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS7[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS7[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS7[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS7[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS7[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS7[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS7[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS7[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS7[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS7[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS7[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS7[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS7[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS7[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS7[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS7[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS7[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS7[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS7[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS7[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS7[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS7[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS7[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS7[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS8[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS8[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS8[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS8[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS8[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS8[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS8[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS8[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS8[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS8[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS8[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS8[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS8[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS8[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS8[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS8[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS8[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS8[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS8[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS8[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS8[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS8[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS8[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS8[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS8[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS8[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS8[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS8[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS8[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS8[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS8[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS8[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS8[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS8[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS8[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS8[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS8[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS8[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS8[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS8[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS8[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS8[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS8[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS8[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS8[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS8[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS8[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS8[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS8[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS8[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS8[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS8[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS8[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS8[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS8[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS8[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS8[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS8[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS8[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS8[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS8[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS8[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS8[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS8[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS9[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS9[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS9[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS9[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS9[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS9[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS9[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS9[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS9[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS9[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS9[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS9[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS9[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS9[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS9[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS9[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS9[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS9[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS9[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS9[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS9[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS9[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS9[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS9[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS9[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS9[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS9[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS9[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS9[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS9[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS9[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS9[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS9[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS9[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS9[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS9[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS9[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS9[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS9[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS9[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS9[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS9[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS9[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS9[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS9[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS9[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS9[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS9[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS9[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS9[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS9[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS9[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS9[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS9[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS9[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS9[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS9[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS9[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS9[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS9[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS9[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS9[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS9[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS9[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS10[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS10[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS10[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS10[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS10[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS10[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS10[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS10[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS10[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS10[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS10[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS10[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS10[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS10[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS10[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS10[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS10[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS10[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS10[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS10[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS10[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS10[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS10[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS10[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS10[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS10[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS10[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS10[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS10[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS10[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS10[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS10[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS10[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS10[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS10[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS10[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS10[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS10[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS10[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS10[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS10[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS10[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS10[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS10[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS10[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS10[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS10[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS10[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS10[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS10[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS10[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS10[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS10[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS10[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS10[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS10[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS10[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS10[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS10[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS10[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS10[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS10[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS10[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS10[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		#ifdef _WIDEWORD
		vmaskBITS11[0][index2] = bitsbuffer[i].range(0, 0);
		vmaskBITS11[1][index2] = bitsbuffer[i].range(1, 1);
		vmaskBITS11[2][index2] = bitsbuffer[i].range(2, 2);
		vmaskBITS11[3][index2] = bitsbuffer[i].range(3, 3);
		vmaskBITS11[4][index2] = bitsbuffer[i].range(4, 4);
		vmaskBITS11[5][index2] = bitsbuffer[i].range(5, 5);
		vmaskBITS11[6][index2] = bitsbuffer[i].range(6, 6);
		vmaskBITS11[7][index2] = bitsbuffer[i].range(7, 7);
		vmaskBITS11[8][index2] = bitsbuffer[i].range(8, 8);
		vmaskBITS11[9][index2] = bitsbuffer[i].range(9, 9);
		vmaskBITS11[10][index2] = bitsbuffer[i].range(10, 10);
		vmaskBITS11[11][index2] = bitsbuffer[i].range(11, 11);
		vmaskBITS11[12][index2] = bitsbuffer[i].range(12, 12);
		vmaskBITS11[13][index2] = bitsbuffer[i].range(13, 13);
		vmaskBITS11[14][index2] = bitsbuffer[i].range(14, 14);
		vmaskBITS11[15][index2] = bitsbuffer[i].range(15, 15);
		vmaskBITS11[0][index2 + 1] = bitsbuffer[i].range(16, 16);
		vmaskBITS11[1][index2 + 1] = bitsbuffer[i].range(17, 17);
		vmaskBITS11[2][index2 + 1] = bitsbuffer[i].range(18, 18);
		vmaskBITS11[3][index2 + 1] = bitsbuffer[i].range(19, 19);
		vmaskBITS11[4][index2 + 1] = bitsbuffer[i].range(20, 20);
		vmaskBITS11[5][index2 + 1] = bitsbuffer[i].range(21, 21);
		vmaskBITS11[6][index2 + 1] = bitsbuffer[i].range(22, 22);
		vmaskBITS11[7][index2 + 1] = bitsbuffer[i].range(23, 23);
		vmaskBITS11[8][index2 + 1] = bitsbuffer[i].range(24, 24);
		vmaskBITS11[9][index2 + 1] = bitsbuffer[i].range(25, 25);
		vmaskBITS11[10][index2 + 1] = bitsbuffer[i].range(26, 26);
		vmaskBITS11[11][index2 + 1] = bitsbuffer[i].range(27, 27);
		vmaskBITS11[12][index2 + 1] = bitsbuffer[i].range(28, 28);
		vmaskBITS11[13][index2 + 1] = bitsbuffer[i].range(29, 29);
		vmaskBITS11[14][index2 + 1] = bitsbuffer[i].range(30, 30);
		vmaskBITS11[15][index2 + 1] = bitsbuffer[i].range(31, 31);
		#else 
		vmaskBITS11[0][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		vmaskBITS11[1][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		vmaskBITS11[2][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		vmaskBITS11[3][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		vmaskBITS11[4][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		vmaskBITS11[5][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		vmaskBITS11[6][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		vmaskBITS11[7][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		vmaskBITS11[8][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		vmaskBITS11[9][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		vmaskBITS11[10][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		vmaskBITS11[11][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		vmaskBITS11[12][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		vmaskBITS11[13][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		vmaskBITS11[14][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		vmaskBITS11[15][index2] = UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		vmaskBITS11[0][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		vmaskBITS11[1][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		vmaskBITS11[2][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		vmaskBITS11[3][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		vmaskBITS11[4][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		vmaskBITS11[5][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		vmaskBITS11[6][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		vmaskBITS11[7][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		vmaskBITS11[8][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		vmaskBITS11[9][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		vmaskBITS11[10][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		vmaskBITS11[11][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		vmaskBITS11[12][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		vmaskBITS11[13][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		vmaskBITS11[14][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		vmaskBITS11[15][index2 + 1] = UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		
		index2 += 2;
	}
	return;
}

// -------------------- pmasks -------------------- //
void acts_all::MEMACCESS_readpmask(uint512_dt * kvdram, uint32_type vmask_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs){
	LOADACTIVEPARTITIONS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
		#ifdef _WIDEWORD
		vmask_p[i] = kvdram[offset_kvs + i].range(31, 0);
		#else
		vmask_p[i] = kvdram[offset_kvs + i].data[0].key;
		#endif 
	}
	return;
}

void acts_all::MEMACCESS_readmanyspmask1(bool_type enable, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask0_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], buffer_type size_kvs){
	
	#ifdef NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNOTIMPLEMENTED // FIXME. NOT IMPLEMENTED.
	
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount1 = BLOCKRAM_SIZE;
	analysis_type analysis_loopcount2 = BLOCKRAM_SIZE / 16;
	analysis_type analysis_loopcount3 = BLOCKRAM_SIZE;
	
	buffer_type transfsize = size_kvs * 16;

	unitBRAMwidth_type tempvmask;
	#pragma HLS DATA_PACK variable = tempvmask	
	unitBRAMwidth_type tempvSUBmask;
	#pragma HLS DATA_PACK variable = tempvSUBmask		
	tempvSUBmask.data[0].key = 0;
	tempvSUBmask.data[0].value = 0;
	tempvSUBmask.data[1].key = 0;
	tempvSUBmask.data[1].value = 0;
	tempvSUBmask.data[2].key = 0;
	tempvSUBmask.data[2].value = 0;
	tempvSUBmask.data[3].key = 0;
	tempvSUBmask.data[3].value = 0;
	tempvSUBmask.data[4].key = 0;
	tempvSUBmask.data[4].value = 0;
	tempvSUBmask.data[5].key = 0;
	tempvSUBmask.data[5].value = 0;
	tempvSUBmask.data[6].key = 0;
	tempvSUBmask.data[6].value = 0;
	tempvSUBmask.data[7].key = 0;
	tempvSUBmask.data[7].value = 0;
	tempvSUBmask.data[8].key = 0;
	tempvSUBmask.data[8].value = 0;
	tempvSUBmask.data[9].key = 0;
	tempvSUBmask.data[9].value = 0;
	tempvSUBmask.data[10].key = 0;
	tempvSUBmask.data[10].value = 0;
	tempvSUBmask.data[11].key = 0;
	tempvSUBmask.data[11].value = 0;
	tempvSUBmask.data[12].key = 0;
	tempvSUBmask.data[12].value = 0;
	tempvSUBmask.data[13].key = 0;
	tempvSUBmask.data[13].value = 0;
	tempvSUBmask.data[14].key = 0;
	tempvSUBmask.data[14].value = 0;
	tempvSUBmask.data[15].key = 0;
	tempvSUBmask.data[15].value = 0;
	LOADVMASKSSUBP_LOOP2: for (buffer_type i=0; i<transfsize; i++){ // transfsize, reducebuffersz
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount3 avg=analysis_loopcount3
	#pragma HLS PIPELINE II=2
		
		tempvmask = vmask0[i];
		
		// subp mask 
		#ifdef ENABLE_SUBVMASKING
		tempvSUBmask.data[0].key = tempvSUBmask.data[0].key | tempvmask.data[0].key;
		tempvSUBmask.data[0].value = tempvSUBmask.data[0].value | tempvmask.data[0].value;
		tempvSUBmask.data[1].key = tempvSUBmask.data[1].key | tempvmask.data[1].key;
		tempvSUBmask.data[1].value = tempvSUBmask.data[1].value | tempvmask.data[1].value;
		tempvSUBmask.data[2].key = tempvSUBmask.data[2].key | tempvmask.data[2].key;
		tempvSUBmask.data[2].value = tempvSUBmask.data[2].value | tempvmask.data[2].value;
		tempvSUBmask.data[3].key = tempvSUBmask.data[3].key | tempvmask.data[3].key;
		tempvSUBmask.data[3].value = tempvSUBmask.data[3].value | tempvmask.data[3].value;
		tempvSUBmask.data[4].key = tempvSUBmask.data[4].key | tempvmask.data[4].key;
		tempvSUBmask.data[4].value = tempvSUBmask.data[4].value | tempvmask.data[4].value;
		tempvSUBmask.data[5].key = tempvSUBmask.data[5].key | tempvmask.data[5].key;
		tempvSUBmask.data[5].value = tempvSUBmask.data[5].value | tempvmask.data[5].value;
		tempvSUBmask.data[6].key = tempvSUBmask.data[6].key | tempvmask.data[6].key;
		tempvSUBmask.data[6].value = tempvSUBmask.data[6].value | tempvmask.data[6].value;
		tempvSUBmask.data[7].key = tempvSUBmask.data[7].key | tempvmask.data[7].key;
		tempvSUBmask.data[7].value = tempvSUBmask.data[7].value | tempvmask.data[7].value;
		tempvSUBmask.data[8].key = tempvSUBmask.data[8].key | tempvmask.data[8].key;
		tempvSUBmask.data[8].value = tempvSUBmask.data[8].value | tempvmask.data[8].value;
		tempvSUBmask.data[9].key = tempvSUBmask.data[9].key | tempvmask.data[9].key;
		tempvSUBmask.data[9].value = tempvSUBmask.data[9].value | tempvmask.data[9].value;
		tempvSUBmask.data[10].key = tempvSUBmask.data[10].key | tempvmask.data[10].key;
		tempvSUBmask.data[10].value = tempvSUBmask.data[10].value | tempvmask.data[10].value;
		tempvSUBmask.data[11].key = tempvSUBmask.data[11].key | tempvmask.data[11].key;
		tempvSUBmask.data[11].value = tempvSUBmask.data[11].value | tempvmask.data[11].value;
		tempvSUBmask.data[12].key = tempvSUBmask.data[12].key | tempvmask.data[12].key;
		tempvSUBmask.data[12].value = tempvSUBmask.data[12].value | tempvmask.data[12].value;
		tempvSUBmask.data[13].key = tempvSUBmask.data[13].key | tempvmask.data[13].key;
		tempvSUBmask.data[13].value = tempvSUBmask.data[13].value | tempvmask.data[13].value;
		tempvSUBmask.data[14].key = tempvSUBmask.data[14].key | tempvmask.data[14].key;
		tempvSUBmask.data[14].value = tempvSUBmask.data[14].value | tempvmask.data[14].value;
		tempvSUBmask.data[15].key = tempvSUBmask.data[15].key | tempvmask.data[15].key;
		tempvSUBmask.data[15].value = tempvSUBmask.data[15].value | tempvmask.data[15].value;
		
		if(i % 8 == 7){ // CRITICAL AUTOMATEME.
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask0_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask0_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask0_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask0_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask0_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask0_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask0_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask0_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask0_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask0_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask0_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask0_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask0_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask0_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask0_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask0_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask0_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask0_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask0_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask0_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask0_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask0_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask0_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask0_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask0_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask0_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask0_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask0_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask0_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask0_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask0_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask0_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			
			tempvSUBmask.data[0].key = 0;
			tempvSUBmask.data[0].value = 0;
			tempvSUBmask.data[1].key = 0;
			tempvSUBmask.data[1].value = 0;
			tempvSUBmask.data[2].key = 0;
			tempvSUBmask.data[2].value = 0;
			tempvSUBmask.data[3].key = 0;
			tempvSUBmask.data[3].value = 0;
			tempvSUBmask.data[4].key = 0;
			tempvSUBmask.data[4].value = 0;
			tempvSUBmask.data[5].key = 0;
			tempvSUBmask.data[5].value = 0;
			tempvSUBmask.data[6].key = 0;
			tempvSUBmask.data[6].value = 0;
			tempvSUBmask.data[7].key = 0;
			tempvSUBmask.data[7].value = 0;
			tempvSUBmask.data[8].key = 0;
			tempvSUBmask.data[8].value = 0;
			tempvSUBmask.data[9].key = 0;
			tempvSUBmask.data[9].value = 0;
			tempvSUBmask.data[10].key = 0;
			tempvSUBmask.data[10].value = 0;
			tempvSUBmask.data[11].key = 0;
			tempvSUBmask.data[11].value = 0;
			tempvSUBmask.data[12].key = 0;
			tempvSUBmask.data[12].value = 0;
			tempvSUBmask.data[13].key = 0;
			tempvSUBmask.data[13].value = 0;
			tempvSUBmask.data[14].key = 0;
			tempvSUBmask.data[14].value = 0;
			tempvSUBmask.data[15].key = 0;
			tempvSUBmask.data[15].value = 0;
		}
		#endif 
	}
	
	#endif 
	return;
}
void acts_all::MEMACCESS_readmanyspmask2(bool_type enable, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask0_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], buffer_type size_kvs){
	
	#ifdef NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNOTIMPLEMENTED // FIXME. NOT IMPLEMENTED.
	
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount1 = BLOCKRAM_SIZE;
	analysis_type analysis_loopcount2 = BLOCKRAM_SIZE / 16;
	analysis_type analysis_loopcount3 = BLOCKRAM_SIZE;
	
	buffer_type transfsize = size_kvs * 16;

	unitBRAMwidth_type tempvmask;
	#pragma HLS DATA_PACK variable = tempvmask	
	unitBRAMwidth_type tempvSUBmask;
	#pragma HLS DATA_PACK variable = tempvSUBmask		
	tempvSUBmask.data[0].key = 0;
	tempvSUBmask.data[0].value = 0;
	tempvSUBmask.data[1].key = 0;
	tempvSUBmask.data[1].value = 0;
	tempvSUBmask.data[2].key = 0;
	tempvSUBmask.data[2].value = 0;
	tempvSUBmask.data[3].key = 0;
	tempvSUBmask.data[3].value = 0;
	tempvSUBmask.data[4].key = 0;
	tempvSUBmask.data[4].value = 0;
	tempvSUBmask.data[5].key = 0;
	tempvSUBmask.data[5].value = 0;
	tempvSUBmask.data[6].key = 0;
	tempvSUBmask.data[6].value = 0;
	tempvSUBmask.data[7].key = 0;
	tempvSUBmask.data[7].value = 0;
	tempvSUBmask.data[8].key = 0;
	tempvSUBmask.data[8].value = 0;
	tempvSUBmask.data[9].key = 0;
	tempvSUBmask.data[9].value = 0;
	tempvSUBmask.data[10].key = 0;
	tempvSUBmask.data[10].value = 0;
	tempvSUBmask.data[11].key = 0;
	tempvSUBmask.data[11].value = 0;
	tempvSUBmask.data[12].key = 0;
	tempvSUBmask.data[12].value = 0;
	tempvSUBmask.data[13].key = 0;
	tempvSUBmask.data[13].value = 0;
	tempvSUBmask.data[14].key = 0;
	tempvSUBmask.data[14].value = 0;
	tempvSUBmask.data[15].key = 0;
	tempvSUBmask.data[15].value = 0;
	LOADVMASKSSUBP_LOOP2: for (buffer_type i=0; i<transfsize; i++){ // transfsize, reducebuffersz
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount3 avg=analysis_loopcount3
	#pragma HLS PIPELINE II=2
		
		tempvmask = vmask0[i];
		
		// subp mask 
		#ifdef ENABLE_SUBVMASKING
		tempvSUBmask.data[0].key = tempvSUBmask.data[0].key | tempvmask.data[0].key;
		tempvSUBmask.data[0].value = tempvSUBmask.data[0].value | tempvmask.data[0].value;
		tempvSUBmask.data[1].key = tempvSUBmask.data[1].key | tempvmask.data[1].key;
		tempvSUBmask.data[1].value = tempvSUBmask.data[1].value | tempvmask.data[1].value;
		tempvSUBmask.data[2].key = tempvSUBmask.data[2].key | tempvmask.data[2].key;
		tempvSUBmask.data[2].value = tempvSUBmask.data[2].value | tempvmask.data[2].value;
		tempvSUBmask.data[3].key = tempvSUBmask.data[3].key | tempvmask.data[3].key;
		tempvSUBmask.data[3].value = tempvSUBmask.data[3].value | tempvmask.data[3].value;
		tempvSUBmask.data[4].key = tempvSUBmask.data[4].key | tempvmask.data[4].key;
		tempvSUBmask.data[4].value = tempvSUBmask.data[4].value | tempvmask.data[4].value;
		tempvSUBmask.data[5].key = tempvSUBmask.data[5].key | tempvmask.data[5].key;
		tempvSUBmask.data[5].value = tempvSUBmask.data[5].value | tempvmask.data[5].value;
		tempvSUBmask.data[6].key = tempvSUBmask.data[6].key | tempvmask.data[6].key;
		tempvSUBmask.data[6].value = tempvSUBmask.data[6].value | tempvmask.data[6].value;
		tempvSUBmask.data[7].key = tempvSUBmask.data[7].key | tempvmask.data[7].key;
		tempvSUBmask.data[7].value = tempvSUBmask.data[7].value | tempvmask.data[7].value;
		tempvSUBmask.data[8].key = tempvSUBmask.data[8].key | tempvmask.data[8].key;
		tempvSUBmask.data[8].value = tempvSUBmask.data[8].value | tempvmask.data[8].value;
		tempvSUBmask.data[9].key = tempvSUBmask.data[9].key | tempvmask.data[9].key;
		tempvSUBmask.data[9].value = tempvSUBmask.data[9].value | tempvmask.data[9].value;
		tempvSUBmask.data[10].key = tempvSUBmask.data[10].key | tempvmask.data[10].key;
		tempvSUBmask.data[10].value = tempvSUBmask.data[10].value | tempvmask.data[10].value;
		tempvSUBmask.data[11].key = tempvSUBmask.data[11].key | tempvmask.data[11].key;
		tempvSUBmask.data[11].value = tempvSUBmask.data[11].value | tempvmask.data[11].value;
		tempvSUBmask.data[12].key = tempvSUBmask.data[12].key | tempvmask.data[12].key;
		tempvSUBmask.data[12].value = tempvSUBmask.data[12].value | tempvmask.data[12].value;
		tempvSUBmask.data[13].key = tempvSUBmask.data[13].key | tempvmask.data[13].key;
		tempvSUBmask.data[13].value = tempvSUBmask.data[13].value | tempvmask.data[13].value;
		tempvSUBmask.data[14].key = tempvSUBmask.data[14].key | tempvmask.data[14].key;
		tempvSUBmask.data[14].value = tempvSUBmask.data[14].value | tempvmask.data[14].value;
		tempvSUBmask.data[15].key = tempvSUBmask.data[15].key | tempvmask.data[15].key;
		tempvSUBmask.data[15].value = tempvSUBmask.data[15].value | tempvmask.data[15].value;
		
		if(i % 8 == 7){ // CRITICAL AUTOMATEME.
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask0_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask0_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask0_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask0_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask0_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask0_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask0_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask0_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask0_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask0_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask0_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask0_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask0_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask0_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask0_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask0_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask0_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask0_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask0_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask0_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask0_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask0_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask0_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask0_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask0_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask0_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask0_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask0_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask0_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask0_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask0_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask0_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask1_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask1_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask1_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask1_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask1_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask1_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask1_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask1_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask1_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask1_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask1_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask1_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask1_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask1_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask1_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask1_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask1_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask1_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask1_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask1_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask1_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask1_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask1_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask1_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask1_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask1_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask1_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask1_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask1_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask1_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask1_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask1_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			
			tempvSUBmask.data[0].key = 0;
			tempvSUBmask.data[0].value = 0;
			tempvSUBmask.data[1].key = 0;
			tempvSUBmask.data[1].value = 0;
			tempvSUBmask.data[2].key = 0;
			tempvSUBmask.data[2].value = 0;
			tempvSUBmask.data[3].key = 0;
			tempvSUBmask.data[3].value = 0;
			tempvSUBmask.data[4].key = 0;
			tempvSUBmask.data[4].value = 0;
			tempvSUBmask.data[5].key = 0;
			tempvSUBmask.data[5].value = 0;
			tempvSUBmask.data[6].key = 0;
			tempvSUBmask.data[6].value = 0;
			tempvSUBmask.data[7].key = 0;
			tempvSUBmask.data[7].value = 0;
			tempvSUBmask.data[8].key = 0;
			tempvSUBmask.data[8].value = 0;
			tempvSUBmask.data[9].key = 0;
			tempvSUBmask.data[9].value = 0;
			tempvSUBmask.data[10].key = 0;
			tempvSUBmask.data[10].value = 0;
			tempvSUBmask.data[11].key = 0;
			tempvSUBmask.data[11].value = 0;
			tempvSUBmask.data[12].key = 0;
			tempvSUBmask.data[12].value = 0;
			tempvSUBmask.data[13].key = 0;
			tempvSUBmask.data[13].value = 0;
			tempvSUBmask.data[14].key = 0;
			tempvSUBmask.data[14].value = 0;
			tempvSUBmask.data[15].key = 0;
			tempvSUBmask.data[15].value = 0;
		}
		#endif 
	}
	
	#endif 
	return;
}
void acts_all::MEMACCESS_readmanyspmask3(bool_type enable, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask0_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask2_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], buffer_type size_kvs){
	
	#ifdef NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNOTIMPLEMENTED // FIXME. NOT IMPLEMENTED.
	
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount1 = BLOCKRAM_SIZE;
	analysis_type analysis_loopcount2 = BLOCKRAM_SIZE / 16;
	analysis_type analysis_loopcount3 = BLOCKRAM_SIZE;
	
	buffer_type transfsize = size_kvs * 16;

	unitBRAMwidth_type tempvmask;
	#pragma HLS DATA_PACK variable = tempvmask	
	unitBRAMwidth_type tempvSUBmask;
	#pragma HLS DATA_PACK variable = tempvSUBmask		
	tempvSUBmask.data[0].key = 0;
	tempvSUBmask.data[0].value = 0;
	tempvSUBmask.data[1].key = 0;
	tempvSUBmask.data[1].value = 0;
	tempvSUBmask.data[2].key = 0;
	tempvSUBmask.data[2].value = 0;
	tempvSUBmask.data[3].key = 0;
	tempvSUBmask.data[3].value = 0;
	tempvSUBmask.data[4].key = 0;
	tempvSUBmask.data[4].value = 0;
	tempvSUBmask.data[5].key = 0;
	tempvSUBmask.data[5].value = 0;
	tempvSUBmask.data[6].key = 0;
	tempvSUBmask.data[6].value = 0;
	tempvSUBmask.data[7].key = 0;
	tempvSUBmask.data[7].value = 0;
	tempvSUBmask.data[8].key = 0;
	tempvSUBmask.data[8].value = 0;
	tempvSUBmask.data[9].key = 0;
	tempvSUBmask.data[9].value = 0;
	tempvSUBmask.data[10].key = 0;
	tempvSUBmask.data[10].value = 0;
	tempvSUBmask.data[11].key = 0;
	tempvSUBmask.data[11].value = 0;
	tempvSUBmask.data[12].key = 0;
	tempvSUBmask.data[12].value = 0;
	tempvSUBmask.data[13].key = 0;
	tempvSUBmask.data[13].value = 0;
	tempvSUBmask.data[14].key = 0;
	tempvSUBmask.data[14].value = 0;
	tempvSUBmask.data[15].key = 0;
	tempvSUBmask.data[15].value = 0;
	LOADVMASKSSUBP_LOOP2: for (buffer_type i=0; i<transfsize; i++){ // transfsize, reducebuffersz
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount3 avg=analysis_loopcount3
	#pragma HLS PIPELINE II=2
		
		tempvmask = vmask0[i];
		
		// subp mask 
		#ifdef ENABLE_SUBVMASKING
		tempvSUBmask.data[0].key = tempvSUBmask.data[0].key | tempvmask.data[0].key;
		tempvSUBmask.data[0].value = tempvSUBmask.data[0].value | tempvmask.data[0].value;
		tempvSUBmask.data[1].key = tempvSUBmask.data[1].key | tempvmask.data[1].key;
		tempvSUBmask.data[1].value = tempvSUBmask.data[1].value | tempvmask.data[1].value;
		tempvSUBmask.data[2].key = tempvSUBmask.data[2].key | tempvmask.data[2].key;
		tempvSUBmask.data[2].value = tempvSUBmask.data[2].value | tempvmask.data[2].value;
		tempvSUBmask.data[3].key = tempvSUBmask.data[3].key | tempvmask.data[3].key;
		tempvSUBmask.data[3].value = tempvSUBmask.data[3].value | tempvmask.data[3].value;
		tempvSUBmask.data[4].key = tempvSUBmask.data[4].key | tempvmask.data[4].key;
		tempvSUBmask.data[4].value = tempvSUBmask.data[4].value | tempvmask.data[4].value;
		tempvSUBmask.data[5].key = tempvSUBmask.data[5].key | tempvmask.data[5].key;
		tempvSUBmask.data[5].value = tempvSUBmask.data[5].value | tempvmask.data[5].value;
		tempvSUBmask.data[6].key = tempvSUBmask.data[6].key | tempvmask.data[6].key;
		tempvSUBmask.data[6].value = tempvSUBmask.data[6].value | tempvmask.data[6].value;
		tempvSUBmask.data[7].key = tempvSUBmask.data[7].key | tempvmask.data[7].key;
		tempvSUBmask.data[7].value = tempvSUBmask.data[7].value | tempvmask.data[7].value;
		tempvSUBmask.data[8].key = tempvSUBmask.data[8].key | tempvmask.data[8].key;
		tempvSUBmask.data[8].value = tempvSUBmask.data[8].value | tempvmask.data[8].value;
		tempvSUBmask.data[9].key = tempvSUBmask.data[9].key | tempvmask.data[9].key;
		tempvSUBmask.data[9].value = tempvSUBmask.data[9].value | tempvmask.data[9].value;
		tempvSUBmask.data[10].key = tempvSUBmask.data[10].key | tempvmask.data[10].key;
		tempvSUBmask.data[10].value = tempvSUBmask.data[10].value | tempvmask.data[10].value;
		tempvSUBmask.data[11].key = tempvSUBmask.data[11].key | tempvmask.data[11].key;
		tempvSUBmask.data[11].value = tempvSUBmask.data[11].value | tempvmask.data[11].value;
		tempvSUBmask.data[12].key = tempvSUBmask.data[12].key | tempvmask.data[12].key;
		tempvSUBmask.data[12].value = tempvSUBmask.data[12].value | tempvmask.data[12].value;
		tempvSUBmask.data[13].key = tempvSUBmask.data[13].key | tempvmask.data[13].key;
		tempvSUBmask.data[13].value = tempvSUBmask.data[13].value | tempvmask.data[13].value;
		tempvSUBmask.data[14].key = tempvSUBmask.data[14].key | tempvmask.data[14].key;
		tempvSUBmask.data[14].value = tempvSUBmask.data[14].value | tempvmask.data[14].value;
		tempvSUBmask.data[15].key = tempvSUBmask.data[15].key | tempvmask.data[15].key;
		tempvSUBmask.data[15].value = tempvSUBmask.data[15].value | tempvmask.data[15].value;
		
		if(i % 8 == 7){ // CRITICAL AUTOMATEME.
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask0_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask0_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask0_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask0_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask0_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask0_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask0_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask0_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask0_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask0_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask0_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask0_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask0_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask0_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask0_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask0_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask0_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask0_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask0_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask0_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask0_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask0_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask0_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask0_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask0_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask0_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask0_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask0_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask0_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask0_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask0_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask0_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask1_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask1_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask1_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask1_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask1_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask1_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask1_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask1_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask1_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask1_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask1_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask1_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask1_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask1_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask1_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask1_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask1_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask1_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask1_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask1_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask1_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask1_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask1_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask1_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask1_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask1_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask1_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask1_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask1_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask1_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask1_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask1_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask2_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask2_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask2_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask2_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask2_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask2_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask2_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask2_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask2_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask2_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask2_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask2_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask2_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask2_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask2_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask2_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask2_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask2_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask2_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask2_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask2_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask2_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask2_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask2_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask2_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask2_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask2_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask2_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask2_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask2_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask2_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask2_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			
			tempvSUBmask.data[0].key = 0;
			tempvSUBmask.data[0].value = 0;
			tempvSUBmask.data[1].key = 0;
			tempvSUBmask.data[1].value = 0;
			tempvSUBmask.data[2].key = 0;
			tempvSUBmask.data[2].value = 0;
			tempvSUBmask.data[3].key = 0;
			tempvSUBmask.data[3].value = 0;
			tempvSUBmask.data[4].key = 0;
			tempvSUBmask.data[4].value = 0;
			tempvSUBmask.data[5].key = 0;
			tempvSUBmask.data[5].value = 0;
			tempvSUBmask.data[6].key = 0;
			tempvSUBmask.data[6].value = 0;
			tempvSUBmask.data[7].key = 0;
			tempvSUBmask.data[7].value = 0;
			tempvSUBmask.data[8].key = 0;
			tempvSUBmask.data[8].value = 0;
			tempvSUBmask.data[9].key = 0;
			tempvSUBmask.data[9].value = 0;
			tempvSUBmask.data[10].key = 0;
			tempvSUBmask.data[10].value = 0;
			tempvSUBmask.data[11].key = 0;
			tempvSUBmask.data[11].value = 0;
			tempvSUBmask.data[12].key = 0;
			tempvSUBmask.data[12].value = 0;
			tempvSUBmask.data[13].key = 0;
			tempvSUBmask.data[13].value = 0;
			tempvSUBmask.data[14].key = 0;
			tempvSUBmask.data[14].value = 0;
			tempvSUBmask.data[15].key = 0;
			tempvSUBmask.data[15].value = 0;
		}
		#endif 
	}
	
	#endif 
	return;
}
void acts_all::MEMACCESS_readmanyspmask4(bool_type enable, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask0_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask2_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask3_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], buffer_type size_kvs){
	
	#ifdef NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNOTIMPLEMENTED // FIXME. NOT IMPLEMENTED.
	
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount1 = BLOCKRAM_SIZE;
	analysis_type analysis_loopcount2 = BLOCKRAM_SIZE / 16;
	analysis_type analysis_loopcount3 = BLOCKRAM_SIZE;
	
	buffer_type transfsize = size_kvs * 16;

	unitBRAMwidth_type tempvmask;
	#pragma HLS DATA_PACK variable = tempvmask	
	unitBRAMwidth_type tempvSUBmask;
	#pragma HLS DATA_PACK variable = tempvSUBmask		
	tempvSUBmask.data[0].key = 0;
	tempvSUBmask.data[0].value = 0;
	tempvSUBmask.data[1].key = 0;
	tempvSUBmask.data[1].value = 0;
	tempvSUBmask.data[2].key = 0;
	tempvSUBmask.data[2].value = 0;
	tempvSUBmask.data[3].key = 0;
	tempvSUBmask.data[3].value = 0;
	tempvSUBmask.data[4].key = 0;
	tempvSUBmask.data[4].value = 0;
	tempvSUBmask.data[5].key = 0;
	tempvSUBmask.data[5].value = 0;
	tempvSUBmask.data[6].key = 0;
	tempvSUBmask.data[6].value = 0;
	tempvSUBmask.data[7].key = 0;
	tempvSUBmask.data[7].value = 0;
	tempvSUBmask.data[8].key = 0;
	tempvSUBmask.data[8].value = 0;
	tempvSUBmask.data[9].key = 0;
	tempvSUBmask.data[9].value = 0;
	tempvSUBmask.data[10].key = 0;
	tempvSUBmask.data[10].value = 0;
	tempvSUBmask.data[11].key = 0;
	tempvSUBmask.data[11].value = 0;
	tempvSUBmask.data[12].key = 0;
	tempvSUBmask.data[12].value = 0;
	tempvSUBmask.data[13].key = 0;
	tempvSUBmask.data[13].value = 0;
	tempvSUBmask.data[14].key = 0;
	tempvSUBmask.data[14].value = 0;
	tempvSUBmask.data[15].key = 0;
	tempvSUBmask.data[15].value = 0;
	LOADVMASKSSUBP_LOOP2: for (buffer_type i=0; i<transfsize; i++){ // transfsize, reducebuffersz
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount3 avg=analysis_loopcount3
	#pragma HLS PIPELINE II=2
		
		tempvmask = vmask0[i];
		
		// subp mask 
		#ifdef ENABLE_SUBVMASKING
		tempvSUBmask.data[0].key = tempvSUBmask.data[0].key | tempvmask.data[0].key;
		tempvSUBmask.data[0].value = tempvSUBmask.data[0].value | tempvmask.data[0].value;
		tempvSUBmask.data[1].key = tempvSUBmask.data[1].key | tempvmask.data[1].key;
		tempvSUBmask.data[1].value = tempvSUBmask.data[1].value | tempvmask.data[1].value;
		tempvSUBmask.data[2].key = tempvSUBmask.data[2].key | tempvmask.data[2].key;
		tempvSUBmask.data[2].value = tempvSUBmask.data[2].value | tempvmask.data[2].value;
		tempvSUBmask.data[3].key = tempvSUBmask.data[3].key | tempvmask.data[3].key;
		tempvSUBmask.data[3].value = tempvSUBmask.data[3].value | tempvmask.data[3].value;
		tempvSUBmask.data[4].key = tempvSUBmask.data[4].key | tempvmask.data[4].key;
		tempvSUBmask.data[4].value = tempvSUBmask.data[4].value | tempvmask.data[4].value;
		tempvSUBmask.data[5].key = tempvSUBmask.data[5].key | tempvmask.data[5].key;
		tempvSUBmask.data[5].value = tempvSUBmask.data[5].value | tempvmask.data[5].value;
		tempvSUBmask.data[6].key = tempvSUBmask.data[6].key | tempvmask.data[6].key;
		tempvSUBmask.data[6].value = tempvSUBmask.data[6].value | tempvmask.data[6].value;
		tempvSUBmask.data[7].key = tempvSUBmask.data[7].key | tempvmask.data[7].key;
		tempvSUBmask.data[7].value = tempvSUBmask.data[7].value | tempvmask.data[7].value;
		tempvSUBmask.data[8].key = tempvSUBmask.data[8].key | tempvmask.data[8].key;
		tempvSUBmask.data[8].value = tempvSUBmask.data[8].value | tempvmask.data[8].value;
		tempvSUBmask.data[9].key = tempvSUBmask.data[9].key | tempvmask.data[9].key;
		tempvSUBmask.data[9].value = tempvSUBmask.data[9].value | tempvmask.data[9].value;
		tempvSUBmask.data[10].key = tempvSUBmask.data[10].key | tempvmask.data[10].key;
		tempvSUBmask.data[10].value = tempvSUBmask.data[10].value | tempvmask.data[10].value;
		tempvSUBmask.data[11].key = tempvSUBmask.data[11].key | tempvmask.data[11].key;
		tempvSUBmask.data[11].value = tempvSUBmask.data[11].value | tempvmask.data[11].value;
		tempvSUBmask.data[12].key = tempvSUBmask.data[12].key | tempvmask.data[12].key;
		tempvSUBmask.data[12].value = tempvSUBmask.data[12].value | tempvmask.data[12].value;
		tempvSUBmask.data[13].key = tempvSUBmask.data[13].key | tempvmask.data[13].key;
		tempvSUBmask.data[13].value = tempvSUBmask.data[13].value | tempvmask.data[13].value;
		tempvSUBmask.data[14].key = tempvSUBmask.data[14].key | tempvmask.data[14].key;
		tempvSUBmask.data[14].value = tempvSUBmask.data[14].value | tempvmask.data[14].value;
		tempvSUBmask.data[15].key = tempvSUBmask.data[15].key | tempvmask.data[15].key;
		tempvSUBmask.data[15].value = tempvSUBmask.data[15].value | tempvmask.data[15].value;
		
		if(i % 8 == 7){ // CRITICAL AUTOMATEME.
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask0_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask0_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask0_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask0_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask0_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask0_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask0_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask0_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask0_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask0_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask0_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask0_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask0_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask0_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask0_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask0_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask0_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask0_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask0_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask0_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask0_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask0_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask0_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask0_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask0_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask0_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask0_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask0_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask0_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask0_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask0_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask0_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask1_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask1_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask1_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask1_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask1_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask1_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask1_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask1_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask1_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask1_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask1_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask1_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask1_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask1_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask1_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask1_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask1_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask1_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask1_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask1_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask1_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask1_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask1_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask1_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask1_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask1_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask1_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask1_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask1_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask1_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask1_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask1_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask2_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask2_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask2_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask2_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask2_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask2_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask2_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask2_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask2_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask2_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask2_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask2_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask2_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask2_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask2_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask2_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask2_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask2_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask2_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask2_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask2_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask2_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask2_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask2_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask2_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask2_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask2_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask2_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask2_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask2_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask2_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask2_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask3_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask3_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask3_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask3_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask3_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask3_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask3_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask3_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask3_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask3_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask3_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask3_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask3_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask3_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask3_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask3_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask3_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask3_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask3_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask3_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask3_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask3_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask3_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask3_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask3_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask3_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask3_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask3_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask3_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask3_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask3_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask3_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			
			tempvSUBmask.data[0].key = 0;
			tempvSUBmask.data[0].value = 0;
			tempvSUBmask.data[1].key = 0;
			tempvSUBmask.data[1].value = 0;
			tempvSUBmask.data[2].key = 0;
			tempvSUBmask.data[2].value = 0;
			tempvSUBmask.data[3].key = 0;
			tempvSUBmask.data[3].value = 0;
			tempvSUBmask.data[4].key = 0;
			tempvSUBmask.data[4].value = 0;
			tempvSUBmask.data[5].key = 0;
			tempvSUBmask.data[5].value = 0;
			tempvSUBmask.data[6].key = 0;
			tempvSUBmask.data[6].value = 0;
			tempvSUBmask.data[7].key = 0;
			tempvSUBmask.data[7].value = 0;
			tempvSUBmask.data[8].key = 0;
			tempvSUBmask.data[8].value = 0;
			tempvSUBmask.data[9].key = 0;
			tempvSUBmask.data[9].value = 0;
			tempvSUBmask.data[10].key = 0;
			tempvSUBmask.data[10].value = 0;
			tempvSUBmask.data[11].key = 0;
			tempvSUBmask.data[11].value = 0;
			tempvSUBmask.data[12].key = 0;
			tempvSUBmask.data[12].value = 0;
			tempvSUBmask.data[13].key = 0;
			tempvSUBmask.data[13].value = 0;
			tempvSUBmask.data[14].key = 0;
			tempvSUBmask.data[14].value = 0;
			tempvSUBmask.data[15].key = 0;
			tempvSUBmask.data[15].value = 0;
		}
		#endif 
	}
	
	#endif 
	return;
}
void acts_all::MEMACCESS_readmanyspmask5(bool_type enable, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask0_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask2_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask3_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask4_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], buffer_type size_kvs){
	
	#ifdef NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNOTIMPLEMENTED // FIXME. NOT IMPLEMENTED.
	
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount1 = BLOCKRAM_SIZE;
	analysis_type analysis_loopcount2 = BLOCKRAM_SIZE / 16;
	analysis_type analysis_loopcount3 = BLOCKRAM_SIZE;
	
	buffer_type transfsize = size_kvs * 16;

	unitBRAMwidth_type tempvmask;
	#pragma HLS DATA_PACK variable = tempvmask	
	unitBRAMwidth_type tempvSUBmask;
	#pragma HLS DATA_PACK variable = tempvSUBmask		
	tempvSUBmask.data[0].key = 0;
	tempvSUBmask.data[0].value = 0;
	tempvSUBmask.data[1].key = 0;
	tempvSUBmask.data[1].value = 0;
	tempvSUBmask.data[2].key = 0;
	tempvSUBmask.data[2].value = 0;
	tempvSUBmask.data[3].key = 0;
	tempvSUBmask.data[3].value = 0;
	tempvSUBmask.data[4].key = 0;
	tempvSUBmask.data[4].value = 0;
	tempvSUBmask.data[5].key = 0;
	tempvSUBmask.data[5].value = 0;
	tempvSUBmask.data[6].key = 0;
	tempvSUBmask.data[6].value = 0;
	tempvSUBmask.data[7].key = 0;
	tempvSUBmask.data[7].value = 0;
	tempvSUBmask.data[8].key = 0;
	tempvSUBmask.data[8].value = 0;
	tempvSUBmask.data[9].key = 0;
	tempvSUBmask.data[9].value = 0;
	tempvSUBmask.data[10].key = 0;
	tempvSUBmask.data[10].value = 0;
	tempvSUBmask.data[11].key = 0;
	tempvSUBmask.data[11].value = 0;
	tempvSUBmask.data[12].key = 0;
	tempvSUBmask.data[12].value = 0;
	tempvSUBmask.data[13].key = 0;
	tempvSUBmask.data[13].value = 0;
	tempvSUBmask.data[14].key = 0;
	tempvSUBmask.data[14].value = 0;
	tempvSUBmask.data[15].key = 0;
	tempvSUBmask.data[15].value = 0;
	LOADVMASKSSUBP_LOOP2: for (buffer_type i=0; i<transfsize; i++){ // transfsize, reducebuffersz
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount3 avg=analysis_loopcount3
	#pragma HLS PIPELINE II=2
		
		tempvmask = vmask0[i];
		
		// subp mask 
		#ifdef ENABLE_SUBVMASKING
		tempvSUBmask.data[0].key = tempvSUBmask.data[0].key | tempvmask.data[0].key;
		tempvSUBmask.data[0].value = tempvSUBmask.data[0].value | tempvmask.data[0].value;
		tempvSUBmask.data[1].key = tempvSUBmask.data[1].key | tempvmask.data[1].key;
		tempvSUBmask.data[1].value = tempvSUBmask.data[1].value | tempvmask.data[1].value;
		tempvSUBmask.data[2].key = tempvSUBmask.data[2].key | tempvmask.data[2].key;
		tempvSUBmask.data[2].value = tempvSUBmask.data[2].value | tempvmask.data[2].value;
		tempvSUBmask.data[3].key = tempvSUBmask.data[3].key | tempvmask.data[3].key;
		tempvSUBmask.data[3].value = tempvSUBmask.data[3].value | tempvmask.data[3].value;
		tempvSUBmask.data[4].key = tempvSUBmask.data[4].key | tempvmask.data[4].key;
		tempvSUBmask.data[4].value = tempvSUBmask.data[4].value | tempvmask.data[4].value;
		tempvSUBmask.data[5].key = tempvSUBmask.data[5].key | tempvmask.data[5].key;
		tempvSUBmask.data[5].value = tempvSUBmask.data[5].value | tempvmask.data[5].value;
		tempvSUBmask.data[6].key = tempvSUBmask.data[6].key | tempvmask.data[6].key;
		tempvSUBmask.data[6].value = tempvSUBmask.data[6].value | tempvmask.data[6].value;
		tempvSUBmask.data[7].key = tempvSUBmask.data[7].key | tempvmask.data[7].key;
		tempvSUBmask.data[7].value = tempvSUBmask.data[7].value | tempvmask.data[7].value;
		tempvSUBmask.data[8].key = tempvSUBmask.data[8].key | tempvmask.data[8].key;
		tempvSUBmask.data[8].value = tempvSUBmask.data[8].value | tempvmask.data[8].value;
		tempvSUBmask.data[9].key = tempvSUBmask.data[9].key | tempvmask.data[9].key;
		tempvSUBmask.data[9].value = tempvSUBmask.data[9].value | tempvmask.data[9].value;
		tempvSUBmask.data[10].key = tempvSUBmask.data[10].key | tempvmask.data[10].key;
		tempvSUBmask.data[10].value = tempvSUBmask.data[10].value | tempvmask.data[10].value;
		tempvSUBmask.data[11].key = tempvSUBmask.data[11].key | tempvmask.data[11].key;
		tempvSUBmask.data[11].value = tempvSUBmask.data[11].value | tempvmask.data[11].value;
		tempvSUBmask.data[12].key = tempvSUBmask.data[12].key | tempvmask.data[12].key;
		tempvSUBmask.data[12].value = tempvSUBmask.data[12].value | tempvmask.data[12].value;
		tempvSUBmask.data[13].key = tempvSUBmask.data[13].key | tempvmask.data[13].key;
		tempvSUBmask.data[13].value = tempvSUBmask.data[13].value | tempvmask.data[13].value;
		tempvSUBmask.data[14].key = tempvSUBmask.data[14].key | tempvmask.data[14].key;
		tempvSUBmask.data[14].value = tempvSUBmask.data[14].value | tempvmask.data[14].value;
		tempvSUBmask.data[15].key = tempvSUBmask.data[15].key | tempvmask.data[15].key;
		tempvSUBmask.data[15].value = tempvSUBmask.data[15].value | tempvmask.data[15].value;
		
		if(i % 8 == 7){ // CRITICAL AUTOMATEME.
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask0_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask0_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask0_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask0_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask0_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask0_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask0_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask0_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask0_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask0_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask0_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask0_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask0_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask0_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask0_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask0_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask0_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask0_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask0_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask0_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask0_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask0_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask0_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask0_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask0_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask0_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask0_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask0_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask0_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask0_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask0_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask0_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask1_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask1_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask1_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask1_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask1_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask1_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask1_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask1_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask1_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask1_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask1_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask1_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask1_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask1_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask1_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask1_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask1_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask1_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask1_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask1_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask1_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask1_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask1_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask1_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask1_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask1_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask1_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask1_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask1_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask1_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask1_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask1_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask2_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask2_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask2_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask2_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask2_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask2_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask2_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask2_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask2_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask2_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask2_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask2_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask2_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask2_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask2_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask2_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask2_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask2_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask2_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask2_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask2_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask2_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask2_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask2_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask2_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask2_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask2_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask2_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask2_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask2_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask2_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask2_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask3_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask3_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask3_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask3_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask3_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask3_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask3_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask3_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask3_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask3_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask3_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask3_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask3_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask3_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask3_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask3_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask3_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask3_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask3_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask3_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask3_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask3_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask3_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask3_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask3_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask3_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask3_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask3_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask3_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask3_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask3_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask3_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask4_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask4_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask4_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask4_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask4_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask4_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask4_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask4_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask4_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask4_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask4_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask4_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask4_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask4_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask4_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask4_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask4_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask4_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask4_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask4_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask4_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask4_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask4_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask4_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask4_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask4_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask4_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask4_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask4_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask4_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask4_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask4_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			
			tempvSUBmask.data[0].key = 0;
			tempvSUBmask.data[0].value = 0;
			tempvSUBmask.data[1].key = 0;
			tempvSUBmask.data[1].value = 0;
			tempvSUBmask.data[2].key = 0;
			tempvSUBmask.data[2].value = 0;
			tempvSUBmask.data[3].key = 0;
			tempvSUBmask.data[3].value = 0;
			tempvSUBmask.data[4].key = 0;
			tempvSUBmask.data[4].value = 0;
			tempvSUBmask.data[5].key = 0;
			tempvSUBmask.data[5].value = 0;
			tempvSUBmask.data[6].key = 0;
			tempvSUBmask.data[6].value = 0;
			tempvSUBmask.data[7].key = 0;
			tempvSUBmask.data[7].value = 0;
			tempvSUBmask.data[8].key = 0;
			tempvSUBmask.data[8].value = 0;
			tempvSUBmask.data[9].key = 0;
			tempvSUBmask.data[9].value = 0;
			tempvSUBmask.data[10].key = 0;
			tempvSUBmask.data[10].value = 0;
			tempvSUBmask.data[11].key = 0;
			tempvSUBmask.data[11].value = 0;
			tempvSUBmask.data[12].key = 0;
			tempvSUBmask.data[12].value = 0;
			tempvSUBmask.data[13].key = 0;
			tempvSUBmask.data[13].value = 0;
			tempvSUBmask.data[14].key = 0;
			tempvSUBmask.data[14].value = 0;
			tempvSUBmask.data[15].key = 0;
			tempvSUBmask.data[15].value = 0;
		}
		#endif 
	}
	
	#endif 
	return;
}
void acts_all::MEMACCESS_readmanyspmask6(bool_type enable, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask0_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask2_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask3_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask4_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask5_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], buffer_type size_kvs){
	
	#ifdef NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNOTIMPLEMENTED // FIXME. NOT IMPLEMENTED.
	
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount1 = BLOCKRAM_SIZE;
	analysis_type analysis_loopcount2 = BLOCKRAM_SIZE / 16;
	analysis_type analysis_loopcount3 = BLOCKRAM_SIZE;
	
	buffer_type transfsize = size_kvs * 16;

	unitBRAMwidth_type tempvmask;
	#pragma HLS DATA_PACK variable = tempvmask	
	unitBRAMwidth_type tempvSUBmask;
	#pragma HLS DATA_PACK variable = tempvSUBmask		
	tempvSUBmask.data[0].key = 0;
	tempvSUBmask.data[0].value = 0;
	tempvSUBmask.data[1].key = 0;
	tempvSUBmask.data[1].value = 0;
	tempvSUBmask.data[2].key = 0;
	tempvSUBmask.data[2].value = 0;
	tempvSUBmask.data[3].key = 0;
	tempvSUBmask.data[3].value = 0;
	tempvSUBmask.data[4].key = 0;
	tempvSUBmask.data[4].value = 0;
	tempvSUBmask.data[5].key = 0;
	tempvSUBmask.data[5].value = 0;
	tempvSUBmask.data[6].key = 0;
	tempvSUBmask.data[6].value = 0;
	tempvSUBmask.data[7].key = 0;
	tempvSUBmask.data[7].value = 0;
	tempvSUBmask.data[8].key = 0;
	tempvSUBmask.data[8].value = 0;
	tempvSUBmask.data[9].key = 0;
	tempvSUBmask.data[9].value = 0;
	tempvSUBmask.data[10].key = 0;
	tempvSUBmask.data[10].value = 0;
	tempvSUBmask.data[11].key = 0;
	tempvSUBmask.data[11].value = 0;
	tempvSUBmask.data[12].key = 0;
	tempvSUBmask.data[12].value = 0;
	tempvSUBmask.data[13].key = 0;
	tempvSUBmask.data[13].value = 0;
	tempvSUBmask.data[14].key = 0;
	tempvSUBmask.data[14].value = 0;
	tempvSUBmask.data[15].key = 0;
	tempvSUBmask.data[15].value = 0;
	LOADVMASKSSUBP_LOOP2: for (buffer_type i=0; i<transfsize; i++){ // transfsize, reducebuffersz
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount3 avg=analysis_loopcount3
	#pragma HLS PIPELINE II=2
		
		tempvmask = vmask0[i];
		
		// subp mask 
		#ifdef ENABLE_SUBVMASKING
		tempvSUBmask.data[0].key = tempvSUBmask.data[0].key | tempvmask.data[0].key;
		tempvSUBmask.data[0].value = tempvSUBmask.data[0].value | tempvmask.data[0].value;
		tempvSUBmask.data[1].key = tempvSUBmask.data[1].key | tempvmask.data[1].key;
		tempvSUBmask.data[1].value = tempvSUBmask.data[1].value | tempvmask.data[1].value;
		tempvSUBmask.data[2].key = tempvSUBmask.data[2].key | tempvmask.data[2].key;
		tempvSUBmask.data[2].value = tempvSUBmask.data[2].value | tempvmask.data[2].value;
		tempvSUBmask.data[3].key = tempvSUBmask.data[3].key | tempvmask.data[3].key;
		tempvSUBmask.data[3].value = tempvSUBmask.data[3].value | tempvmask.data[3].value;
		tempvSUBmask.data[4].key = tempvSUBmask.data[4].key | tempvmask.data[4].key;
		tempvSUBmask.data[4].value = tempvSUBmask.data[4].value | tempvmask.data[4].value;
		tempvSUBmask.data[5].key = tempvSUBmask.data[5].key | tempvmask.data[5].key;
		tempvSUBmask.data[5].value = tempvSUBmask.data[5].value | tempvmask.data[5].value;
		tempvSUBmask.data[6].key = tempvSUBmask.data[6].key | tempvmask.data[6].key;
		tempvSUBmask.data[6].value = tempvSUBmask.data[6].value | tempvmask.data[6].value;
		tempvSUBmask.data[7].key = tempvSUBmask.data[7].key | tempvmask.data[7].key;
		tempvSUBmask.data[7].value = tempvSUBmask.data[7].value | tempvmask.data[7].value;
		tempvSUBmask.data[8].key = tempvSUBmask.data[8].key | tempvmask.data[8].key;
		tempvSUBmask.data[8].value = tempvSUBmask.data[8].value | tempvmask.data[8].value;
		tempvSUBmask.data[9].key = tempvSUBmask.data[9].key | tempvmask.data[9].key;
		tempvSUBmask.data[9].value = tempvSUBmask.data[9].value | tempvmask.data[9].value;
		tempvSUBmask.data[10].key = tempvSUBmask.data[10].key | tempvmask.data[10].key;
		tempvSUBmask.data[10].value = tempvSUBmask.data[10].value | tempvmask.data[10].value;
		tempvSUBmask.data[11].key = tempvSUBmask.data[11].key | tempvmask.data[11].key;
		tempvSUBmask.data[11].value = tempvSUBmask.data[11].value | tempvmask.data[11].value;
		tempvSUBmask.data[12].key = tempvSUBmask.data[12].key | tempvmask.data[12].key;
		tempvSUBmask.data[12].value = tempvSUBmask.data[12].value | tempvmask.data[12].value;
		tempvSUBmask.data[13].key = tempvSUBmask.data[13].key | tempvmask.data[13].key;
		tempvSUBmask.data[13].value = tempvSUBmask.data[13].value | tempvmask.data[13].value;
		tempvSUBmask.data[14].key = tempvSUBmask.data[14].key | tempvmask.data[14].key;
		tempvSUBmask.data[14].value = tempvSUBmask.data[14].value | tempvmask.data[14].value;
		tempvSUBmask.data[15].key = tempvSUBmask.data[15].key | tempvmask.data[15].key;
		tempvSUBmask.data[15].value = tempvSUBmask.data[15].value | tempvmask.data[15].value;
		
		if(i % 8 == 7){ // CRITICAL AUTOMATEME.
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask0_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask0_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask0_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask0_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask0_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask0_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask0_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask0_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask0_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask0_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask0_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask0_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask0_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask0_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask0_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask0_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask0_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask0_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask0_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask0_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask0_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask0_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask0_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask0_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask0_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask0_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask0_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask0_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask0_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask0_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask0_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask0_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask1_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask1_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask1_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask1_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask1_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask1_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask1_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask1_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask1_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask1_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask1_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask1_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask1_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask1_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask1_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask1_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask1_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask1_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask1_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask1_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask1_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask1_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask1_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask1_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask1_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask1_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask1_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask1_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask1_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask1_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask1_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask1_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask2_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask2_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask2_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask2_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask2_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask2_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask2_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask2_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask2_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask2_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask2_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask2_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask2_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask2_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask2_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask2_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask2_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask2_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask2_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask2_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask2_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask2_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask2_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask2_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask2_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask2_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask2_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask2_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask2_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask2_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask2_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask2_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask3_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask3_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask3_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask3_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask3_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask3_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask3_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask3_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask3_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask3_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask3_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask3_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask3_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask3_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask3_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask3_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask3_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask3_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask3_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask3_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask3_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask3_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask3_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask3_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask3_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask3_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask3_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask3_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask3_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask3_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask3_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask3_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask4_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask4_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask4_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask4_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask4_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask4_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask4_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask4_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask4_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask4_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask4_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask4_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask4_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask4_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask4_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask4_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask4_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask4_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask4_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask4_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask4_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask4_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask4_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask4_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask4_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask4_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask4_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask4_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask4_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask4_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask4_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask4_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask5_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask5_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask5_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask5_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask5_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask5_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask5_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask5_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask5_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask5_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask5_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask5_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask5_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask5_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask5_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask5_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask5_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask5_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask5_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask5_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask5_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask5_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask5_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask5_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask5_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask5_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask5_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask5_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask5_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask5_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask5_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask5_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			
			tempvSUBmask.data[0].key = 0;
			tempvSUBmask.data[0].value = 0;
			tempvSUBmask.data[1].key = 0;
			tempvSUBmask.data[1].value = 0;
			tempvSUBmask.data[2].key = 0;
			tempvSUBmask.data[2].value = 0;
			tempvSUBmask.data[3].key = 0;
			tempvSUBmask.data[3].value = 0;
			tempvSUBmask.data[4].key = 0;
			tempvSUBmask.data[4].value = 0;
			tempvSUBmask.data[5].key = 0;
			tempvSUBmask.data[5].value = 0;
			tempvSUBmask.data[6].key = 0;
			tempvSUBmask.data[6].value = 0;
			tempvSUBmask.data[7].key = 0;
			tempvSUBmask.data[7].value = 0;
			tempvSUBmask.data[8].key = 0;
			tempvSUBmask.data[8].value = 0;
			tempvSUBmask.data[9].key = 0;
			tempvSUBmask.data[9].value = 0;
			tempvSUBmask.data[10].key = 0;
			tempvSUBmask.data[10].value = 0;
			tempvSUBmask.data[11].key = 0;
			tempvSUBmask.data[11].value = 0;
			tempvSUBmask.data[12].key = 0;
			tempvSUBmask.data[12].value = 0;
			tempvSUBmask.data[13].key = 0;
			tempvSUBmask.data[13].value = 0;
			tempvSUBmask.data[14].key = 0;
			tempvSUBmask.data[14].value = 0;
			tempvSUBmask.data[15].key = 0;
			tempvSUBmask.data[15].value = 0;
		}
		#endif 
	}
	
	#endif 
	return;
}
void acts_all::MEMACCESS_readmanyspmask7(bool_type enable, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask0_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask2_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask3_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask4_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask5_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask6_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], buffer_type size_kvs){
	
	#ifdef NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNOTIMPLEMENTED // FIXME. NOT IMPLEMENTED.
	
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount1 = BLOCKRAM_SIZE;
	analysis_type analysis_loopcount2 = BLOCKRAM_SIZE / 16;
	analysis_type analysis_loopcount3 = BLOCKRAM_SIZE;
	
	buffer_type transfsize = size_kvs * 16;

	unitBRAMwidth_type tempvmask;
	#pragma HLS DATA_PACK variable = tempvmask	
	unitBRAMwidth_type tempvSUBmask;
	#pragma HLS DATA_PACK variable = tempvSUBmask		
	tempvSUBmask.data[0].key = 0;
	tempvSUBmask.data[0].value = 0;
	tempvSUBmask.data[1].key = 0;
	tempvSUBmask.data[1].value = 0;
	tempvSUBmask.data[2].key = 0;
	tempvSUBmask.data[2].value = 0;
	tempvSUBmask.data[3].key = 0;
	tempvSUBmask.data[3].value = 0;
	tempvSUBmask.data[4].key = 0;
	tempvSUBmask.data[4].value = 0;
	tempvSUBmask.data[5].key = 0;
	tempvSUBmask.data[5].value = 0;
	tempvSUBmask.data[6].key = 0;
	tempvSUBmask.data[6].value = 0;
	tempvSUBmask.data[7].key = 0;
	tempvSUBmask.data[7].value = 0;
	tempvSUBmask.data[8].key = 0;
	tempvSUBmask.data[8].value = 0;
	tempvSUBmask.data[9].key = 0;
	tempvSUBmask.data[9].value = 0;
	tempvSUBmask.data[10].key = 0;
	tempvSUBmask.data[10].value = 0;
	tempvSUBmask.data[11].key = 0;
	tempvSUBmask.data[11].value = 0;
	tempvSUBmask.data[12].key = 0;
	tempvSUBmask.data[12].value = 0;
	tempvSUBmask.data[13].key = 0;
	tempvSUBmask.data[13].value = 0;
	tempvSUBmask.data[14].key = 0;
	tempvSUBmask.data[14].value = 0;
	tempvSUBmask.data[15].key = 0;
	tempvSUBmask.data[15].value = 0;
	LOADVMASKSSUBP_LOOP2: for (buffer_type i=0; i<transfsize; i++){ // transfsize, reducebuffersz
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount3 avg=analysis_loopcount3
	#pragma HLS PIPELINE II=2
		
		tempvmask = vmask0[i];
		
		// subp mask 
		#ifdef ENABLE_SUBVMASKING
		tempvSUBmask.data[0].key = tempvSUBmask.data[0].key | tempvmask.data[0].key;
		tempvSUBmask.data[0].value = tempvSUBmask.data[0].value | tempvmask.data[0].value;
		tempvSUBmask.data[1].key = tempvSUBmask.data[1].key | tempvmask.data[1].key;
		tempvSUBmask.data[1].value = tempvSUBmask.data[1].value | tempvmask.data[1].value;
		tempvSUBmask.data[2].key = tempvSUBmask.data[2].key | tempvmask.data[2].key;
		tempvSUBmask.data[2].value = tempvSUBmask.data[2].value | tempvmask.data[2].value;
		tempvSUBmask.data[3].key = tempvSUBmask.data[3].key | tempvmask.data[3].key;
		tempvSUBmask.data[3].value = tempvSUBmask.data[3].value | tempvmask.data[3].value;
		tempvSUBmask.data[4].key = tempvSUBmask.data[4].key | tempvmask.data[4].key;
		tempvSUBmask.data[4].value = tempvSUBmask.data[4].value | tempvmask.data[4].value;
		tempvSUBmask.data[5].key = tempvSUBmask.data[5].key | tempvmask.data[5].key;
		tempvSUBmask.data[5].value = tempvSUBmask.data[5].value | tempvmask.data[5].value;
		tempvSUBmask.data[6].key = tempvSUBmask.data[6].key | tempvmask.data[6].key;
		tempvSUBmask.data[6].value = tempvSUBmask.data[6].value | tempvmask.data[6].value;
		tempvSUBmask.data[7].key = tempvSUBmask.data[7].key | tempvmask.data[7].key;
		tempvSUBmask.data[7].value = tempvSUBmask.data[7].value | tempvmask.data[7].value;
		tempvSUBmask.data[8].key = tempvSUBmask.data[8].key | tempvmask.data[8].key;
		tempvSUBmask.data[8].value = tempvSUBmask.data[8].value | tempvmask.data[8].value;
		tempvSUBmask.data[9].key = tempvSUBmask.data[9].key | tempvmask.data[9].key;
		tempvSUBmask.data[9].value = tempvSUBmask.data[9].value | tempvmask.data[9].value;
		tempvSUBmask.data[10].key = tempvSUBmask.data[10].key | tempvmask.data[10].key;
		tempvSUBmask.data[10].value = tempvSUBmask.data[10].value | tempvmask.data[10].value;
		tempvSUBmask.data[11].key = tempvSUBmask.data[11].key | tempvmask.data[11].key;
		tempvSUBmask.data[11].value = tempvSUBmask.data[11].value | tempvmask.data[11].value;
		tempvSUBmask.data[12].key = tempvSUBmask.data[12].key | tempvmask.data[12].key;
		tempvSUBmask.data[12].value = tempvSUBmask.data[12].value | tempvmask.data[12].value;
		tempvSUBmask.data[13].key = tempvSUBmask.data[13].key | tempvmask.data[13].key;
		tempvSUBmask.data[13].value = tempvSUBmask.data[13].value | tempvmask.data[13].value;
		tempvSUBmask.data[14].key = tempvSUBmask.data[14].key | tempvmask.data[14].key;
		tempvSUBmask.data[14].value = tempvSUBmask.data[14].value | tempvmask.data[14].value;
		tempvSUBmask.data[15].key = tempvSUBmask.data[15].key | tempvmask.data[15].key;
		tempvSUBmask.data[15].value = tempvSUBmask.data[15].value | tempvmask.data[15].value;
		
		if(i % 8 == 7){ // CRITICAL AUTOMATEME.
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask0_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask0_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask0_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask0_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask0_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask0_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask0_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask0_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask0_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask0_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask0_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask0_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask0_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask0_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask0_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask0_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask0_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask0_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask0_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask0_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask0_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask0_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask0_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask0_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask0_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask0_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask0_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask0_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask0_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask0_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask0_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask0_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask1_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask1_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask1_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask1_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask1_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask1_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask1_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask1_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask1_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask1_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask1_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask1_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask1_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask1_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask1_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask1_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask1_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask1_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask1_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask1_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask1_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask1_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask1_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask1_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask1_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask1_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask1_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask1_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask1_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask1_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask1_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask1_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask2_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask2_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask2_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask2_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask2_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask2_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask2_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask2_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask2_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask2_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask2_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask2_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask2_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask2_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask2_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask2_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask2_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask2_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask2_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask2_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask2_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask2_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask2_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask2_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask2_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask2_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask2_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask2_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask2_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask2_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask2_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask2_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask3_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask3_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask3_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask3_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask3_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask3_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask3_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask3_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask3_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask3_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask3_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask3_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask3_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask3_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask3_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask3_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask3_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask3_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask3_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask3_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask3_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask3_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask3_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask3_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask3_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask3_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask3_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask3_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask3_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask3_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask3_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask3_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask4_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask4_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask4_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask4_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask4_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask4_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask4_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask4_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask4_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask4_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask4_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask4_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask4_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask4_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask4_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask4_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask4_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask4_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask4_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask4_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask4_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask4_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask4_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask4_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask4_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask4_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask4_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask4_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask4_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask4_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask4_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask4_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask5_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask5_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask5_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask5_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask5_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask5_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask5_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask5_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask5_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask5_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask5_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask5_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask5_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask5_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask5_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask5_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask5_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask5_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask5_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask5_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask5_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask5_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask5_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask5_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask5_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask5_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask5_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask5_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask5_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask5_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask5_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask5_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask6_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask6_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask6_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask6_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask6_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask6_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask6_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask6_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask6_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask6_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask6_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask6_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask6_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask6_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask6_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask6_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask6_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask6_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask6_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask6_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask6_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask6_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask6_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask6_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask6_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask6_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask6_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask6_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask6_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask6_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask6_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask6_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			
			tempvSUBmask.data[0].key = 0;
			tempvSUBmask.data[0].value = 0;
			tempvSUBmask.data[1].key = 0;
			tempvSUBmask.data[1].value = 0;
			tempvSUBmask.data[2].key = 0;
			tempvSUBmask.data[2].value = 0;
			tempvSUBmask.data[3].key = 0;
			tempvSUBmask.data[3].value = 0;
			tempvSUBmask.data[4].key = 0;
			tempvSUBmask.data[4].value = 0;
			tempvSUBmask.data[5].key = 0;
			tempvSUBmask.data[5].value = 0;
			tempvSUBmask.data[6].key = 0;
			tempvSUBmask.data[6].value = 0;
			tempvSUBmask.data[7].key = 0;
			tempvSUBmask.data[7].value = 0;
			tempvSUBmask.data[8].key = 0;
			tempvSUBmask.data[8].value = 0;
			tempvSUBmask.data[9].key = 0;
			tempvSUBmask.data[9].value = 0;
			tempvSUBmask.data[10].key = 0;
			tempvSUBmask.data[10].value = 0;
			tempvSUBmask.data[11].key = 0;
			tempvSUBmask.data[11].value = 0;
			tempvSUBmask.data[12].key = 0;
			tempvSUBmask.data[12].value = 0;
			tempvSUBmask.data[13].key = 0;
			tempvSUBmask.data[13].value = 0;
			tempvSUBmask.data[14].key = 0;
			tempvSUBmask.data[14].value = 0;
			tempvSUBmask.data[15].key = 0;
			tempvSUBmask.data[15].value = 0;
		}
		#endif 
	}
	
	#endif 
	return;
}
void acts_all::MEMACCESS_readmanyspmask8(bool_type enable, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask0_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask2_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask3_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask4_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask5_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask6_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask7_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], buffer_type size_kvs){
	
	#ifdef NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNOTIMPLEMENTED // FIXME. NOT IMPLEMENTED.
	
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount1 = BLOCKRAM_SIZE;
	analysis_type analysis_loopcount2 = BLOCKRAM_SIZE / 16;
	analysis_type analysis_loopcount3 = BLOCKRAM_SIZE;
	
	buffer_type transfsize = size_kvs * 16;

	unitBRAMwidth_type tempvmask;
	#pragma HLS DATA_PACK variable = tempvmask	
	unitBRAMwidth_type tempvSUBmask;
	#pragma HLS DATA_PACK variable = tempvSUBmask		
	tempvSUBmask.data[0].key = 0;
	tempvSUBmask.data[0].value = 0;
	tempvSUBmask.data[1].key = 0;
	tempvSUBmask.data[1].value = 0;
	tempvSUBmask.data[2].key = 0;
	tempvSUBmask.data[2].value = 0;
	tempvSUBmask.data[3].key = 0;
	tempvSUBmask.data[3].value = 0;
	tempvSUBmask.data[4].key = 0;
	tempvSUBmask.data[4].value = 0;
	tempvSUBmask.data[5].key = 0;
	tempvSUBmask.data[5].value = 0;
	tempvSUBmask.data[6].key = 0;
	tempvSUBmask.data[6].value = 0;
	tempvSUBmask.data[7].key = 0;
	tempvSUBmask.data[7].value = 0;
	tempvSUBmask.data[8].key = 0;
	tempvSUBmask.data[8].value = 0;
	tempvSUBmask.data[9].key = 0;
	tempvSUBmask.data[9].value = 0;
	tempvSUBmask.data[10].key = 0;
	tempvSUBmask.data[10].value = 0;
	tempvSUBmask.data[11].key = 0;
	tempvSUBmask.data[11].value = 0;
	tempvSUBmask.data[12].key = 0;
	tempvSUBmask.data[12].value = 0;
	tempvSUBmask.data[13].key = 0;
	tempvSUBmask.data[13].value = 0;
	tempvSUBmask.data[14].key = 0;
	tempvSUBmask.data[14].value = 0;
	tempvSUBmask.data[15].key = 0;
	tempvSUBmask.data[15].value = 0;
	LOADVMASKSSUBP_LOOP2: for (buffer_type i=0; i<transfsize; i++){ // transfsize, reducebuffersz
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount3 avg=analysis_loopcount3
	#pragma HLS PIPELINE II=2
		
		tempvmask = vmask0[i];
		
		// subp mask 
		#ifdef ENABLE_SUBVMASKING
		tempvSUBmask.data[0].key = tempvSUBmask.data[0].key | tempvmask.data[0].key;
		tempvSUBmask.data[0].value = tempvSUBmask.data[0].value | tempvmask.data[0].value;
		tempvSUBmask.data[1].key = tempvSUBmask.data[1].key | tempvmask.data[1].key;
		tempvSUBmask.data[1].value = tempvSUBmask.data[1].value | tempvmask.data[1].value;
		tempvSUBmask.data[2].key = tempvSUBmask.data[2].key | tempvmask.data[2].key;
		tempvSUBmask.data[2].value = tempvSUBmask.data[2].value | tempvmask.data[2].value;
		tempvSUBmask.data[3].key = tempvSUBmask.data[3].key | tempvmask.data[3].key;
		tempvSUBmask.data[3].value = tempvSUBmask.data[3].value | tempvmask.data[3].value;
		tempvSUBmask.data[4].key = tempvSUBmask.data[4].key | tempvmask.data[4].key;
		tempvSUBmask.data[4].value = tempvSUBmask.data[4].value | tempvmask.data[4].value;
		tempvSUBmask.data[5].key = tempvSUBmask.data[5].key | tempvmask.data[5].key;
		tempvSUBmask.data[5].value = tempvSUBmask.data[5].value | tempvmask.data[5].value;
		tempvSUBmask.data[6].key = tempvSUBmask.data[6].key | tempvmask.data[6].key;
		tempvSUBmask.data[6].value = tempvSUBmask.data[6].value | tempvmask.data[6].value;
		tempvSUBmask.data[7].key = tempvSUBmask.data[7].key | tempvmask.data[7].key;
		tempvSUBmask.data[7].value = tempvSUBmask.data[7].value | tempvmask.data[7].value;
		tempvSUBmask.data[8].key = tempvSUBmask.data[8].key | tempvmask.data[8].key;
		tempvSUBmask.data[8].value = tempvSUBmask.data[8].value | tempvmask.data[8].value;
		tempvSUBmask.data[9].key = tempvSUBmask.data[9].key | tempvmask.data[9].key;
		tempvSUBmask.data[9].value = tempvSUBmask.data[9].value | tempvmask.data[9].value;
		tempvSUBmask.data[10].key = tempvSUBmask.data[10].key | tempvmask.data[10].key;
		tempvSUBmask.data[10].value = tempvSUBmask.data[10].value | tempvmask.data[10].value;
		tempvSUBmask.data[11].key = tempvSUBmask.data[11].key | tempvmask.data[11].key;
		tempvSUBmask.data[11].value = tempvSUBmask.data[11].value | tempvmask.data[11].value;
		tempvSUBmask.data[12].key = tempvSUBmask.data[12].key | tempvmask.data[12].key;
		tempvSUBmask.data[12].value = tempvSUBmask.data[12].value | tempvmask.data[12].value;
		tempvSUBmask.data[13].key = tempvSUBmask.data[13].key | tempvmask.data[13].key;
		tempvSUBmask.data[13].value = tempvSUBmask.data[13].value | tempvmask.data[13].value;
		tempvSUBmask.data[14].key = tempvSUBmask.data[14].key | tempvmask.data[14].key;
		tempvSUBmask.data[14].value = tempvSUBmask.data[14].value | tempvmask.data[14].value;
		tempvSUBmask.data[15].key = tempvSUBmask.data[15].key | tempvmask.data[15].key;
		tempvSUBmask.data[15].value = tempvSUBmask.data[15].value | tempvmask.data[15].value;
		
		if(i % 8 == 7){ // CRITICAL AUTOMATEME.
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask0_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask0_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask0_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask0_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask0_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask0_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask0_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask0_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask0_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask0_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask0_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask0_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask0_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask0_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask0_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask0_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask0_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask0_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask0_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask0_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask0_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask0_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask0_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask0_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask0_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask0_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask0_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask0_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask0_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask0_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask0_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask0_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask1_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask1_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask1_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask1_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask1_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask1_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask1_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask1_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask1_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask1_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask1_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask1_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask1_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask1_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask1_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask1_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask1_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask1_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask1_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask1_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask1_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask1_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask1_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask1_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask1_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask1_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask1_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask1_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask1_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask1_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask1_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask1_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask2_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask2_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask2_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask2_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask2_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask2_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask2_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask2_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask2_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask2_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask2_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask2_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask2_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask2_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask2_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask2_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask2_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask2_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask2_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask2_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask2_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask2_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask2_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask2_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask2_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask2_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask2_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask2_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask2_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask2_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask2_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask2_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask3_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask3_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask3_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask3_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask3_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask3_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask3_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask3_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask3_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask3_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask3_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask3_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask3_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask3_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask3_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask3_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask3_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask3_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask3_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask3_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask3_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask3_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask3_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask3_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask3_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask3_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask3_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask3_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask3_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask3_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask3_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask3_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask4_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask4_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask4_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask4_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask4_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask4_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask4_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask4_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask4_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask4_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask4_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask4_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask4_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask4_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask4_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask4_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask4_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask4_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask4_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask4_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask4_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask4_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask4_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask4_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask4_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask4_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask4_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask4_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask4_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask4_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask4_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask4_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask5_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask5_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask5_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask5_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask5_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask5_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask5_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask5_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask5_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask5_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask5_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask5_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask5_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask5_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask5_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask5_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask5_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask5_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask5_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask5_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask5_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask5_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask5_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask5_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask5_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask5_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask5_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask5_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask5_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask5_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask5_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask5_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask6_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask6_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask6_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask6_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask6_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask6_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask6_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask6_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask6_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask6_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask6_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask6_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask6_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask6_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask6_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask6_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask6_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask6_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask6_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask6_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask6_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask6_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask6_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask6_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask6_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask6_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask6_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask6_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask6_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask6_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask6_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask6_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask7_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask7_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask7_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask7_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask7_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask7_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask7_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask7_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask7_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask7_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask7_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask7_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask7_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask7_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask7_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask7_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask7_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask7_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask7_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask7_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask7_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask7_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask7_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask7_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask7_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask7_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask7_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask7_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask7_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask7_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask7_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask7_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			
			tempvSUBmask.data[0].key = 0;
			tempvSUBmask.data[0].value = 0;
			tempvSUBmask.data[1].key = 0;
			tempvSUBmask.data[1].value = 0;
			tempvSUBmask.data[2].key = 0;
			tempvSUBmask.data[2].value = 0;
			tempvSUBmask.data[3].key = 0;
			tempvSUBmask.data[3].value = 0;
			tempvSUBmask.data[4].key = 0;
			tempvSUBmask.data[4].value = 0;
			tempvSUBmask.data[5].key = 0;
			tempvSUBmask.data[5].value = 0;
			tempvSUBmask.data[6].key = 0;
			tempvSUBmask.data[6].value = 0;
			tempvSUBmask.data[7].key = 0;
			tempvSUBmask.data[7].value = 0;
			tempvSUBmask.data[8].key = 0;
			tempvSUBmask.data[8].value = 0;
			tempvSUBmask.data[9].key = 0;
			tempvSUBmask.data[9].value = 0;
			tempvSUBmask.data[10].key = 0;
			tempvSUBmask.data[10].value = 0;
			tempvSUBmask.data[11].key = 0;
			tempvSUBmask.data[11].value = 0;
			tempvSUBmask.data[12].key = 0;
			tempvSUBmask.data[12].value = 0;
			tempvSUBmask.data[13].key = 0;
			tempvSUBmask.data[13].value = 0;
			tempvSUBmask.data[14].key = 0;
			tempvSUBmask.data[14].value = 0;
			tempvSUBmask.data[15].key = 0;
			tempvSUBmask.data[15].value = 0;
		}
		#endif 
	}
	
	#endif 
	return;
}
void acts_all::MEMACCESS_readmanyspmask9(bool_type enable, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask0_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask2_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask3_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask4_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask5_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask6_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask7_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask8_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], buffer_type size_kvs){
	
	#ifdef NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNOTIMPLEMENTED // FIXME. NOT IMPLEMENTED.
	
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount1 = BLOCKRAM_SIZE;
	analysis_type analysis_loopcount2 = BLOCKRAM_SIZE / 16;
	analysis_type analysis_loopcount3 = BLOCKRAM_SIZE;
	
	buffer_type transfsize = size_kvs * 16;

	unitBRAMwidth_type tempvmask;
	#pragma HLS DATA_PACK variable = tempvmask	
	unitBRAMwidth_type tempvSUBmask;
	#pragma HLS DATA_PACK variable = tempvSUBmask		
	tempvSUBmask.data[0].key = 0;
	tempvSUBmask.data[0].value = 0;
	tempvSUBmask.data[1].key = 0;
	tempvSUBmask.data[1].value = 0;
	tempvSUBmask.data[2].key = 0;
	tempvSUBmask.data[2].value = 0;
	tempvSUBmask.data[3].key = 0;
	tempvSUBmask.data[3].value = 0;
	tempvSUBmask.data[4].key = 0;
	tempvSUBmask.data[4].value = 0;
	tempvSUBmask.data[5].key = 0;
	tempvSUBmask.data[5].value = 0;
	tempvSUBmask.data[6].key = 0;
	tempvSUBmask.data[6].value = 0;
	tempvSUBmask.data[7].key = 0;
	tempvSUBmask.data[7].value = 0;
	tempvSUBmask.data[8].key = 0;
	tempvSUBmask.data[8].value = 0;
	tempvSUBmask.data[9].key = 0;
	tempvSUBmask.data[9].value = 0;
	tempvSUBmask.data[10].key = 0;
	tempvSUBmask.data[10].value = 0;
	tempvSUBmask.data[11].key = 0;
	tempvSUBmask.data[11].value = 0;
	tempvSUBmask.data[12].key = 0;
	tempvSUBmask.data[12].value = 0;
	tempvSUBmask.data[13].key = 0;
	tempvSUBmask.data[13].value = 0;
	tempvSUBmask.data[14].key = 0;
	tempvSUBmask.data[14].value = 0;
	tempvSUBmask.data[15].key = 0;
	tempvSUBmask.data[15].value = 0;
	LOADVMASKSSUBP_LOOP2: for (buffer_type i=0; i<transfsize; i++){ // transfsize, reducebuffersz
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount3 avg=analysis_loopcount3
	#pragma HLS PIPELINE II=2
		
		tempvmask = vmask0[i];
		
		// subp mask 
		#ifdef ENABLE_SUBVMASKING
		tempvSUBmask.data[0].key = tempvSUBmask.data[0].key | tempvmask.data[0].key;
		tempvSUBmask.data[0].value = tempvSUBmask.data[0].value | tempvmask.data[0].value;
		tempvSUBmask.data[1].key = tempvSUBmask.data[1].key | tempvmask.data[1].key;
		tempvSUBmask.data[1].value = tempvSUBmask.data[1].value | tempvmask.data[1].value;
		tempvSUBmask.data[2].key = tempvSUBmask.data[2].key | tempvmask.data[2].key;
		tempvSUBmask.data[2].value = tempvSUBmask.data[2].value | tempvmask.data[2].value;
		tempvSUBmask.data[3].key = tempvSUBmask.data[3].key | tempvmask.data[3].key;
		tempvSUBmask.data[3].value = tempvSUBmask.data[3].value | tempvmask.data[3].value;
		tempvSUBmask.data[4].key = tempvSUBmask.data[4].key | tempvmask.data[4].key;
		tempvSUBmask.data[4].value = tempvSUBmask.data[4].value | tempvmask.data[4].value;
		tempvSUBmask.data[5].key = tempvSUBmask.data[5].key | tempvmask.data[5].key;
		tempvSUBmask.data[5].value = tempvSUBmask.data[5].value | tempvmask.data[5].value;
		tempvSUBmask.data[6].key = tempvSUBmask.data[6].key | tempvmask.data[6].key;
		tempvSUBmask.data[6].value = tempvSUBmask.data[6].value | tempvmask.data[6].value;
		tempvSUBmask.data[7].key = tempvSUBmask.data[7].key | tempvmask.data[7].key;
		tempvSUBmask.data[7].value = tempvSUBmask.data[7].value | tempvmask.data[7].value;
		tempvSUBmask.data[8].key = tempvSUBmask.data[8].key | tempvmask.data[8].key;
		tempvSUBmask.data[8].value = tempvSUBmask.data[8].value | tempvmask.data[8].value;
		tempvSUBmask.data[9].key = tempvSUBmask.data[9].key | tempvmask.data[9].key;
		tempvSUBmask.data[9].value = tempvSUBmask.data[9].value | tempvmask.data[9].value;
		tempvSUBmask.data[10].key = tempvSUBmask.data[10].key | tempvmask.data[10].key;
		tempvSUBmask.data[10].value = tempvSUBmask.data[10].value | tempvmask.data[10].value;
		tempvSUBmask.data[11].key = tempvSUBmask.data[11].key | tempvmask.data[11].key;
		tempvSUBmask.data[11].value = tempvSUBmask.data[11].value | tempvmask.data[11].value;
		tempvSUBmask.data[12].key = tempvSUBmask.data[12].key | tempvmask.data[12].key;
		tempvSUBmask.data[12].value = tempvSUBmask.data[12].value | tempvmask.data[12].value;
		tempvSUBmask.data[13].key = tempvSUBmask.data[13].key | tempvmask.data[13].key;
		tempvSUBmask.data[13].value = tempvSUBmask.data[13].value | tempvmask.data[13].value;
		tempvSUBmask.data[14].key = tempvSUBmask.data[14].key | tempvmask.data[14].key;
		tempvSUBmask.data[14].value = tempvSUBmask.data[14].value | tempvmask.data[14].value;
		tempvSUBmask.data[15].key = tempvSUBmask.data[15].key | tempvmask.data[15].key;
		tempvSUBmask.data[15].value = tempvSUBmask.data[15].value | tempvmask.data[15].value;
		
		if(i % 8 == 7){ // CRITICAL AUTOMATEME.
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask0_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask0_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask0_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask0_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask0_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask0_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask0_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask0_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask0_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask0_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask0_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask0_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask0_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask0_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask0_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask0_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask0_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask0_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask0_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask0_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask0_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask0_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask0_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask0_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask0_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask0_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask0_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask0_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask0_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask0_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask0_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask0_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask1_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask1_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask1_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask1_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask1_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask1_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask1_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask1_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask1_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask1_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask1_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask1_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask1_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask1_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask1_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask1_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask1_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask1_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask1_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask1_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask1_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask1_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask1_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask1_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask1_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask1_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask1_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask1_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask1_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask1_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask1_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask1_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask2_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask2_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask2_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask2_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask2_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask2_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask2_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask2_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask2_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask2_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask2_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask2_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask2_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask2_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask2_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask2_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask2_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask2_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask2_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask2_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask2_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask2_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask2_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask2_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask2_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask2_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask2_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask2_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask2_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask2_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask2_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask2_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask3_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask3_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask3_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask3_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask3_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask3_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask3_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask3_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask3_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask3_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask3_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask3_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask3_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask3_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask3_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask3_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask3_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask3_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask3_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask3_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask3_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask3_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask3_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask3_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask3_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask3_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask3_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask3_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask3_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask3_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask3_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask3_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask4_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask4_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask4_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask4_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask4_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask4_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask4_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask4_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask4_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask4_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask4_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask4_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask4_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask4_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask4_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask4_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask4_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask4_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask4_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask4_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask4_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask4_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask4_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask4_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask4_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask4_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask4_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask4_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask4_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask4_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask4_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask4_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask5_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask5_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask5_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask5_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask5_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask5_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask5_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask5_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask5_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask5_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask5_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask5_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask5_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask5_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask5_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask5_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask5_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask5_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask5_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask5_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask5_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask5_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask5_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask5_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask5_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask5_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask5_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask5_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask5_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask5_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask5_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask5_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask6_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask6_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask6_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask6_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask6_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask6_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask6_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask6_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask6_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask6_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask6_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask6_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask6_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask6_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask6_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask6_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask6_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask6_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask6_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask6_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask6_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask6_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask6_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask6_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask6_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask6_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask6_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask6_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask6_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask6_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask6_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask6_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask7_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask7_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask7_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask7_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask7_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask7_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask7_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask7_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask7_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask7_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask7_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask7_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask7_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask7_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask7_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask7_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask7_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask7_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask7_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask7_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask7_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask7_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask7_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask7_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask7_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask7_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask7_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask7_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask7_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask7_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask7_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask7_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask8_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask8_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask8_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask8_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask8_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask8_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask8_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask8_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask8_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask8_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask8_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask8_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask8_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask8_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask8_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask8_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask8_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask8_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask8_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask8_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask8_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask8_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask8_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask8_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask8_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask8_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask8_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask8_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask8_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask8_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask8_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask8_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			
			tempvSUBmask.data[0].key = 0;
			tempvSUBmask.data[0].value = 0;
			tempvSUBmask.data[1].key = 0;
			tempvSUBmask.data[1].value = 0;
			tempvSUBmask.data[2].key = 0;
			tempvSUBmask.data[2].value = 0;
			tempvSUBmask.data[3].key = 0;
			tempvSUBmask.data[3].value = 0;
			tempvSUBmask.data[4].key = 0;
			tempvSUBmask.data[4].value = 0;
			tempvSUBmask.data[5].key = 0;
			tempvSUBmask.data[5].value = 0;
			tempvSUBmask.data[6].key = 0;
			tempvSUBmask.data[6].value = 0;
			tempvSUBmask.data[7].key = 0;
			tempvSUBmask.data[7].value = 0;
			tempvSUBmask.data[8].key = 0;
			tempvSUBmask.data[8].value = 0;
			tempvSUBmask.data[9].key = 0;
			tempvSUBmask.data[9].value = 0;
			tempvSUBmask.data[10].key = 0;
			tempvSUBmask.data[10].value = 0;
			tempvSUBmask.data[11].key = 0;
			tempvSUBmask.data[11].value = 0;
			tempvSUBmask.data[12].key = 0;
			tempvSUBmask.data[12].value = 0;
			tempvSUBmask.data[13].key = 0;
			tempvSUBmask.data[13].value = 0;
			tempvSUBmask.data[14].key = 0;
			tempvSUBmask.data[14].value = 0;
			tempvSUBmask.data[15].key = 0;
			tempvSUBmask.data[15].value = 0;
		}
		#endif 
	}
	
	#endif 
	return;
}
void acts_all::MEMACCESS_readmanyspmask10(bool_type enable, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask0_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask2_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask3_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask4_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask5_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask6_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask7_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask8_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask9_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], buffer_type size_kvs){
	
	#ifdef NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNOTIMPLEMENTED // FIXME. NOT IMPLEMENTED.
	
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount1 = BLOCKRAM_SIZE;
	analysis_type analysis_loopcount2 = BLOCKRAM_SIZE / 16;
	analysis_type analysis_loopcount3 = BLOCKRAM_SIZE;
	
	buffer_type transfsize = size_kvs * 16;

	unitBRAMwidth_type tempvmask;
	#pragma HLS DATA_PACK variable = tempvmask	
	unitBRAMwidth_type tempvSUBmask;
	#pragma HLS DATA_PACK variable = tempvSUBmask		
	tempvSUBmask.data[0].key = 0;
	tempvSUBmask.data[0].value = 0;
	tempvSUBmask.data[1].key = 0;
	tempvSUBmask.data[1].value = 0;
	tempvSUBmask.data[2].key = 0;
	tempvSUBmask.data[2].value = 0;
	tempvSUBmask.data[3].key = 0;
	tempvSUBmask.data[3].value = 0;
	tempvSUBmask.data[4].key = 0;
	tempvSUBmask.data[4].value = 0;
	tempvSUBmask.data[5].key = 0;
	tempvSUBmask.data[5].value = 0;
	tempvSUBmask.data[6].key = 0;
	tempvSUBmask.data[6].value = 0;
	tempvSUBmask.data[7].key = 0;
	tempvSUBmask.data[7].value = 0;
	tempvSUBmask.data[8].key = 0;
	tempvSUBmask.data[8].value = 0;
	tempvSUBmask.data[9].key = 0;
	tempvSUBmask.data[9].value = 0;
	tempvSUBmask.data[10].key = 0;
	tempvSUBmask.data[10].value = 0;
	tempvSUBmask.data[11].key = 0;
	tempvSUBmask.data[11].value = 0;
	tempvSUBmask.data[12].key = 0;
	tempvSUBmask.data[12].value = 0;
	tempvSUBmask.data[13].key = 0;
	tempvSUBmask.data[13].value = 0;
	tempvSUBmask.data[14].key = 0;
	tempvSUBmask.data[14].value = 0;
	tempvSUBmask.data[15].key = 0;
	tempvSUBmask.data[15].value = 0;
	LOADVMASKSSUBP_LOOP2: for (buffer_type i=0; i<transfsize; i++){ // transfsize, reducebuffersz
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount3 avg=analysis_loopcount3
	#pragma HLS PIPELINE II=2
		
		tempvmask = vmask0[i];
		
		// subp mask 
		#ifdef ENABLE_SUBVMASKING
		tempvSUBmask.data[0].key = tempvSUBmask.data[0].key | tempvmask.data[0].key;
		tempvSUBmask.data[0].value = tempvSUBmask.data[0].value | tempvmask.data[0].value;
		tempvSUBmask.data[1].key = tempvSUBmask.data[1].key | tempvmask.data[1].key;
		tempvSUBmask.data[1].value = tempvSUBmask.data[1].value | tempvmask.data[1].value;
		tempvSUBmask.data[2].key = tempvSUBmask.data[2].key | tempvmask.data[2].key;
		tempvSUBmask.data[2].value = tempvSUBmask.data[2].value | tempvmask.data[2].value;
		tempvSUBmask.data[3].key = tempvSUBmask.data[3].key | tempvmask.data[3].key;
		tempvSUBmask.data[3].value = tempvSUBmask.data[3].value | tempvmask.data[3].value;
		tempvSUBmask.data[4].key = tempvSUBmask.data[4].key | tempvmask.data[4].key;
		tempvSUBmask.data[4].value = tempvSUBmask.data[4].value | tempvmask.data[4].value;
		tempvSUBmask.data[5].key = tempvSUBmask.data[5].key | tempvmask.data[5].key;
		tempvSUBmask.data[5].value = tempvSUBmask.data[5].value | tempvmask.data[5].value;
		tempvSUBmask.data[6].key = tempvSUBmask.data[6].key | tempvmask.data[6].key;
		tempvSUBmask.data[6].value = tempvSUBmask.data[6].value | tempvmask.data[6].value;
		tempvSUBmask.data[7].key = tempvSUBmask.data[7].key | tempvmask.data[7].key;
		tempvSUBmask.data[7].value = tempvSUBmask.data[7].value | tempvmask.data[7].value;
		tempvSUBmask.data[8].key = tempvSUBmask.data[8].key | tempvmask.data[8].key;
		tempvSUBmask.data[8].value = tempvSUBmask.data[8].value | tempvmask.data[8].value;
		tempvSUBmask.data[9].key = tempvSUBmask.data[9].key | tempvmask.data[9].key;
		tempvSUBmask.data[9].value = tempvSUBmask.data[9].value | tempvmask.data[9].value;
		tempvSUBmask.data[10].key = tempvSUBmask.data[10].key | tempvmask.data[10].key;
		tempvSUBmask.data[10].value = tempvSUBmask.data[10].value | tempvmask.data[10].value;
		tempvSUBmask.data[11].key = tempvSUBmask.data[11].key | tempvmask.data[11].key;
		tempvSUBmask.data[11].value = tempvSUBmask.data[11].value | tempvmask.data[11].value;
		tempvSUBmask.data[12].key = tempvSUBmask.data[12].key | tempvmask.data[12].key;
		tempvSUBmask.data[12].value = tempvSUBmask.data[12].value | tempvmask.data[12].value;
		tempvSUBmask.data[13].key = tempvSUBmask.data[13].key | tempvmask.data[13].key;
		tempvSUBmask.data[13].value = tempvSUBmask.data[13].value | tempvmask.data[13].value;
		tempvSUBmask.data[14].key = tempvSUBmask.data[14].key | tempvmask.data[14].key;
		tempvSUBmask.data[14].value = tempvSUBmask.data[14].value | tempvmask.data[14].value;
		tempvSUBmask.data[15].key = tempvSUBmask.data[15].key | tempvmask.data[15].key;
		tempvSUBmask.data[15].value = tempvSUBmask.data[15].value | tempvmask.data[15].value;
		
		if(i % 8 == 7){ // CRITICAL AUTOMATEME.
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask0_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask0_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask0_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask0_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask0_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask0_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask0_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask0_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask0_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask0_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask0_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask0_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask0_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask0_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask0_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask0_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask0_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask0_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask0_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask0_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask0_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask0_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask0_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask0_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask0_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask0_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask0_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask0_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask0_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask0_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask0_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask0_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask1_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask1_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask1_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask1_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask1_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask1_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask1_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask1_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask1_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask1_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask1_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask1_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask1_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask1_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask1_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask1_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask1_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask1_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask1_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask1_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask1_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask1_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask1_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask1_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask1_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask1_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask1_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask1_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask1_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask1_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask1_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask1_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask2_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask2_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask2_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask2_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask2_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask2_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask2_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask2_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask2_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask2_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask2_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask2_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask2_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask2_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask2_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask2_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask2_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask2_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask2_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask2_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask2_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask2_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask2_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask2_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask2_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask2_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask2_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask2_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask2_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask2_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask2_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask2_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask3_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask3_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask3_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask3_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask3_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask3_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask3_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask3_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask3_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask3_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask3_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask3_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask3_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask3_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask3_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask3_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask3_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask3_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask3_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask3_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask3_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask3_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask3_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask3_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask3_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask3_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask3_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask3_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask3_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask3_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask3_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask3_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask4_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask4_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask4_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask4_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask4_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask4_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask4_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask4_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask4_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask4_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask4_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask4_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask4_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask4_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask4_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask4_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask4_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask4_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask4_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask4_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask4_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask4_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask4_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask4_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask4_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask4_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask4_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask4_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask4_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask4_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask4_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask4_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask5_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask5_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask5_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask5_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask5_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask5_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask5_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask5_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask5_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask5_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask5_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask5_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask5_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask5_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask5_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask5_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask5_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask5_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask5_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask5_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask5_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask5_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask5_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask5_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask5_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask5_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask5_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask5_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask5_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask5_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask5_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask5_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask6_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask6_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask6_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask6_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask6_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask6_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask6_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask6_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask6_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask6_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask6_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask6_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask6_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask6_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask6_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask6_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask6_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask6_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask6_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask6_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask6_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask6_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask6_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask6_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask6_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask6_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask6_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask6_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask6_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask6_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask6_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask6_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask7_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask7_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask7_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask7_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask7_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask7_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask7_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask7_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask7_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask7_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask7_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask7_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask7_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask7_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask7_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask7_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask7_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask7_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask7_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask7_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask7_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask7_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask7_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask7_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask7_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask7_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask7_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask7_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask7_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask7_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask7_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask7_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask8_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask8_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask8_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask8_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask8_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask8_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask8_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask8_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask8_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask8_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask8_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask8_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask8_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask8_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask8_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask8_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask8_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask8_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask8_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask8_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask8_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask8_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask8_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask8_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask8_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask8_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask8_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask8_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask8_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask8_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask8_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask8_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask9_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask9_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask9_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask9_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask9_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask9_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask9_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask9_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask9_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask9_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask9_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask9_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask9_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask9_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask9_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask9_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask9_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask9_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask9_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask9_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask9_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask9_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask9_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask9_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask9_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask9_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask9_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask9_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask9_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask9_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask9_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask9_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			
			tempvSUBmask.data[0].key = 0;
			tempvSUBmask.data[0].value = 0;
			tempvSUBmask.data[1].key = 0;
			tempvSUBmask.data[1].value = 0;
			tempvSUBmask.data[2].key = 0;
			tempvSUBmask.data[2].value = 0;
			tempvSUBmask.data[3].key = 0;
			tempvSUBmask.data[3].value = 0;
			tempvSUBmask.data[4].key = 0;
			tempvSUBmask.data[4].value = 0;
			tempvSUBmask.data[5].key = 0;
			tempvSUBmask.data[5].value = 0;
			tempvSUBmask.data[6].key = 0;
			tempvSUBmask.data[6].value = 0;
			tempvSUBmask.data[7].key = 0;
			tempvSUBmask.data[7].value = 0;
			tempvSUBmask.data[8].key = 0;
			tempvSUBmask.data[8].value = 0;
			tempvSUBmask.data[9].key = 0;
			tempvSUBmask.data[9].value = 0;
			tempvSUBmask.data[10].key = 0;
			tempvSUBmask.data[10].value = 0;
			tempvSUBmask.data[11].key = 0;
			tempvSUBmask.data[11].value = 0;
			tempvSUBmask.data[12].key = 0;
			tempvSUBmask.data[12].value = 0;
			tempvSUBmask.data[13].key = 0;
			tempvSUBmask.data[13].value = 0;
			tempvSUBmask.data[14].key = 0;
			tempvSUBmask.data[14].value = 0;
			tempvSUBmask.data[15].key = 0;
			tempvSUBmask.data[15].value = 0;
		}
		#endif 
	}
	
	#endif 
	return;
}
void acts_all::MEMACCESS_readmanyspmask11(bool_type enable, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask0_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask2_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask3_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask4_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask5_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask6_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask7_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask8_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask9_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask10_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], buffer_type size_kvs){
	
	#ifdef NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNOTIMPLEMENTED // FIXME. NOT IMPLEMENTED.
	
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount1 = BLOCKRAM_SIZE;
	analysis_type analysis_loopcount2 = BLOCKRAM_SIZE / 16;
	analysis_type analysis_loopcount3 = BLOCKRAM_SIZE;
	
	buffer_type transfsize = size_kvs * 16;

	unitBRAMwidth_type tempvmask;
	#pragma HLS DATA_PACK variable = tempvmask	
	unitBRAMwidth_type tempvSUBmask;
	#pragma HLS DATA_PACK variable = tempvSUBmask		
	tempvSUBmask.data[0].key = 0;
	tempvSUBmask.data[0].value = 0;
	tempvSUBmask.data[1].key = 0;
	tempvSUBmask.data[1].value = 0;
	tempvSUBmask.data[2].key = 0;
	tempvSUBmask.data[2].value = 0;
	tempvSUBmask.data[3].key = 0;
	tempvSUBmask.data[3].value = 0;
	tempvSUBmask.data[4].key = 0;
	tempvSUBmask.data[4].value = 0;
	tempvSUBmask.data[5].key = 0;
	tempvSUBmask.data[5].value = 0;
	tempvSUBmask.data[6].key = 0;
	tempvSUBmask.data[6].value = 0;
	tempvSUBmask.data[7].key = 0;
	tempvSUBmask.data[7].value = 0;
	tempvSUBmask.data[8].key = 0;
	tempvSUBmask.data[8].value = 0;
	tempvSUBmask.data[9].key = 0;
	tempvSUBmask.data[9].value = 0;
	tempvSUBmask.data[10].key = 0;
	tempvSUBmask.data[10].value = 0;
	tempvSUBmask.data[11].key = 0;
	tempvSUBmask.data[11].value = 0;
	tempvSUBmask.data[12].key = 0;
	tempvSUBmask.data[12].value = 0;
	tempvSUBmask.data[13].key = 0;
	tempvSUBmask.data[13].value = 0;
	tempvSUBmask.data[14].key = 0;
	tempvSUBmask.data[14].value = 0;
	tempvSUBmask.data[15].key = 0;
	tempvSUBmask.data[15].value = 0;
	LOADVMASKSSUBP_LOOP2: for (buffer_type i=0; i<transfsize; i++){ // transfsize, reducebuffersz
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount3 avg=analysis_loopcount3
	#pragma HLS PIPELINE II=2
		
		tempvmask = vmask0[i];
		
		// subp mask 
		#ifdef ENABLE_SUBVMASKING
		tempvSUBmask.data[0].key = tempvSUBmask.data[0].key | tempvmask.data[0].key;
		tempvSUBmask.data[0].value = tempvSUBmask.data[0].value | tempvmask.data[0].value;
		tempvSUBmask.data[1].key = tempvSUBmask.data[1].key | tempvmask.data[1].key;
		tempvSUBmask.data[1].value = tempvSUBmask.data[1].value | tempvmask.data[1].value;
		tempvSUBmask.data[2].key = tempvSUBmask.data[2].key | tempvmask.data[2].key;
		tempvSUBmask.data[2].value = tempvSUBmask.data[2].value | tempvmask.data[2].value;
		tempvSUBmask.data[3].key = tempvSUBmask.data[3].key | tempvmask.data[3].key;
		tempvSUBmask.data[3].value = tempvSUBmask.data[3].value | tempvmask.data[3].value;
		tempvSUBmask.data[4].key = tempvSUBmask.data[4].key | tempvmask.data[4].key;
		tempvSUBmask.data[4].value = tempvSUBmask.data[4].value | tempvmask.data[4].value;
		tempvSUBmask.data[5].key = tempvSUBmask.data[5].key | tempvmask.data[5].key;
		tempvSUBmask.data[5].value = tempvSUBmask.data[5].value | tempvmask.data[5].value;
		tempvSUBmask.data[6].key = tempvSUBmask.data[6].key | tempvmask.data[6].key;
		tempvSUBmask.data[6].value = tempvSUBmask.data[6].value | tempvmask.data[6].value;
		tempvSUBmask.data[7].key = tempvSUBmask.data[7].key | tempvmask.data[7].key;
		tempvSUBmask.data[7].value = tempvSUBmask.data[7].value | tempvmask.data[7].value;
		tempvSUBmask.data[8].key = tempvSUBmask.data[8].key | tempvmask.data[8].key;
		tempvSUBmask.data[8].value = tempvSUBmask.data[8].value | tempvmask.data[8].value;
		tempvSUBmask.data[9].key = tempvSUBmask.data[9].key | tempvmask.data[9].key;
		tempvSUBmask.data[9].value = tempvSUBmask.data[9].value | tempvmask.data[9].value;
		tempvSUBmask.data[10].key = tempvSUBmask.data[10].key | tempvmask.data[10].key;
		tempvSUBmask.data[10].value = tempvSUBmask.data[10].value | tempvmask.data[10].value;
		tempvSUBmask.data[11].key = tempvSUBmask.data[11].key | tempvmask.data[11].key;
		tempvSUBmask.data[11].value = tempvSUBmask.data[11].value | tempvmask.data[11].value;
		tempvSUBmask.data[12].key = tempvSUBmask.data[12].key | tempvmask.data[12].key;
		tempvSUBmask.data[12].value = tempvSUBmask.data[12].value | tempvmask.data[12].value;
		tempvSUBmask.data[13].key = tempvSUBmask.data[13].key | tempvmask.data[13].key;
		tempvSUBmask.data[13].value = tempvSUBmask.data[13].value | tempvmask.data[13].value;
		tempvSUBmask.data[14].key = tempvSUBmask.data[14].key | tempvmask.data[14].key;
		tempvSUBmask.data[14].value = tempvSUBmask.data[14].value | tempvmask.data[14].value;
		tempvSUBmask.data[15].key = tempvSUBmask.data[15].key | tempvmask.data[15].key;
		tempvSUBmask.data[15].value = tempvSUBmask.data[15].value | tempvmask.data[15].value;
		
		if(i % 8 == 7){ // CRITICAL AUTOMATEME.
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask0_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask0_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask0_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask0_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask0_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask0_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask0_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask0_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask0_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask0_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask0_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask0_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask0_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask0_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask0_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask0_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask0_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask0_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask0_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask0_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask0_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask0_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask0_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask0_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask0_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask0_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask0_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask0_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask0_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask0_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask0_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask0_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask1_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask1_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask1_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask1_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask1_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask1_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask1_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask1_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask1_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask1_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask1_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask1_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask1_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask1_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask1_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask1_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask1_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask1_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask1_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask1_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask1_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask1_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask1_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask1_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask1_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask1_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask1_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask1_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask1_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask1_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask1_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask1_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask2_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask2_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask2_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask2_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask2_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask2_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask2_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask2_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask2_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask2_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask2_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask2_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask2_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask2_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask2_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask2_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask2_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask2_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask2_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask2_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask2_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask2_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask2_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask2_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask2_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask2_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask2_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask2_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask2_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask2_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask2_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask2_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask3_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask3_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask3_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask3_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask3_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask3_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask3_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask3_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask3_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask3_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask3_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask3_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask3_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask3_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask3_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask3_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask3_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask3_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask3_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask3_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask3_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask3_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask3_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask3_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask3_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask3_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask3_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask3_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask3_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask3_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask3_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask3_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask4_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask4_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask4_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask4_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask4_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask4_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask4_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask4_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask4_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask4_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask4_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask4_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask4_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask4_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask4_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask4_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask4_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask4_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask4_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask4_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask4_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask4_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask4_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask4_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask4_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask4_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask4_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask4_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask4_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask4_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask4_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask4_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask5_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask5_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask5_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask5_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask5_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask5_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask5_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask5_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask5_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask5_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask5_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask5_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask5_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask5_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask5_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask5_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask5_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask5_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask5_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask5_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask5_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask5_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask5_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask5_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask5_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask5_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask5_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask5_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask5_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask5_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask5_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask5_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask6_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask6_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask6_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask6_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask6_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask6_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask6_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask6_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask6_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask6_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask6_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask6_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask6_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask6_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask6_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask6_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask6_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask6_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask6_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask6_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask6_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask6_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask6_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask6_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask6_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask6_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask6_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask6_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask6_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask6_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask6_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask6_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask7_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask7_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask7_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask7_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask7_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask7_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask7_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask7_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask7_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask7_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask7_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask7_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask7_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask7_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask7_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask7_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask7_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask7_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask7_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask7_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask7_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask7_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask7_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask7_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask7_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask7_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask7_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask7_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask7_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask7_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask7_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask7_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask8_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask8_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask8_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask8_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask8_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask8_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask8_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask8_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask8_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask8_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask8_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask8_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask8_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask8_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask8_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask8_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask8_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask8_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask8_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask8_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask8_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask8_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask8_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask8_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask8_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask8_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask8_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask8_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask8_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask8_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask8_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask8_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask9_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask9_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask9_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask9_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask9_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask9_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask9_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask9_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask9_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask9_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask9_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask9_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask9_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask9_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask9_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask9_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask9_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask9_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask9_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask9_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask9_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask9_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask9_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask9_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask9_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask9_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask9_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask9_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask9_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask9_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask9_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask9_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask10_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask10_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask10_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask10_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask10_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask10_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask10_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask10_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask10_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask10_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask10_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask10_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask10_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask10_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask10_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask10_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask10_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask10_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask10_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask10_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask10_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask10_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask10_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask10_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask10_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask10_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask10_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask10_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask10_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask10_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask10_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask10_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			
			tempvSUBmask.data[0].key = 0;
			tempvSUBmask.data[0].value = 0;
			tempvSUBmask.data[1].key = 0;
			tempvSUBmask.data[1].value = 0;
			tempvSUBmask.data[2].key = 0;
			tempvSUBmask.data[2].value = 0;
			tempvSUBmask.data[3].key = 0;
			tempvSUBmask.data[3].value = 0;
			tempvSUBmask.data[4].key = 0;
			tempvSUBmask.data[4].value = 0;
			tempvSUBmask.data[5].key = 0;
			tempvSUBmask.data[5].value = 0;
			tempvSUBmask.data[6].key = 0;
			tempvSUBmask.data[6].value = 0;
			tempvSUBmask.data[7].key = 0;
			tempvSUBmask.data[7].value = 0;
			tempvSUBmask.data[8].key = 0;
			tempvSUBmask.data[8].value = 0;
			tempvSUBmask.data[9].key = 0;
			tempvSUBmask.data[9].value = 0;
			tempvSUBmask.data[10].key = 0;
			tempvSUBmask.data[10].value = 0;
			tempvSUBmask.data[11].key = 0;
			tempvSUBmask.data[11].value = 0;
			tempvSUBmask.data[12].key = 0;
			tempvSUBmask.data[12].value = 0;
			tempvSUBmask.data[13].key = 0;
			tempvSUBmask.data[13].value = 0;
			tempvSUBmask.data[14].key = 0;
			tempvSUBmask.data[14].value = 0;
			tempvSUBmask.data[15].key = 0;
			tempvSUBmask.data[15].value = 0;
		}
		#endif 
	}
	
	#endif 
	return;
}
void acts_all::MEMACCESS_readmanyspmask12(bool_type enable, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask0_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask2_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask3_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask4_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask5_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask6_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask7_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask8_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask9_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask10_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask11_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], buffer_type size_kvs){
	
	#ifdef NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNOTIMPLEMENTED // FIXME. NOT IMPLEMENTED.
	
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount1 = BLOCKRAM_SIZE;
	analysis_type analysis_loopcount2 = BLOCKRAM_SIZE / 16;
	analysis_type analysis_loopcount3 = BLOCKRAM_SIZE;
	
	buffer_type transfsize = size_kvs * 16;

	unitBRAMwidth_type tempvmask;
	#pragma HLS DATA_PACK variable = tempvmask	
	unitBRAMwidth_type tempvSUBmask;
	#pragma HLS DATA_PACK variable = tempvSUBmask		
	tempvSUBmask.data[0].key = 0;
	tempvSUBmask.data[0].value = 0;
	tempvSUBmask.data[1].key = 0;
	tempvSUBmask.data[1].value = 0;
	tempvSUBmask.data[2].key = 0;
	tempvSUBmask.data[2].value = 0;
	tempvSUBmask.data[3].key = 0;
	tempvSUBmask.data[3].value = 0;
	tempvSUBmask.data[4].key = 0;
	tempvSUBmask.data[4].value = 0;
	tempvSUBmask.data[5].key = 0;
	tempvSUBmask.data[5].value = 0;
	tempvSUBmask.data[6].key = 0;
	tempvSUBmask.data[6].value = 0;
	tempvSUBmask.data[7].key = 0;
	tempvSUBmask.data[7].value = 0;
	tempvSUBmask.data[8].key = 0;
	tempvSUBmask.data[8].value = 0;
	tempvSUBmask.data[9].key = 0;
	tempvSUBmask.data[9].value = 0;
	tempvSUBmask.data[10].key = 0;
	tempvSUBmask.data[10].value = 0;
	tempvSUBmask.data[11].key = 0;
	tempvSUBmask.data[11].value = 0;
	tempvSUBmask.data[12].key = 0;
	tempvSUBmask.data[12].value = 0;
	tempvSUBmask.data[13].key = 0;
	tempvSUBmask.data[13].value = 0;
	tempvSUBmask.data[14].key = 0;
	tempvSUBmask.data[14].value = 0;
	tempvSUBmask.data[15].key = 0;
	tempvSUBmask.data[15].value = 0;
	LOADVMASKSSUBP_LOOP2: for (buffer_type i=0; i<transfsize; i++){ // transfsize, reducebuffersz
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount3 avg=analysis_loopcount3
	#pragma HLS PIPELINE II=2
		
		tempvmask = vmask0[i];
		
		// subp mask 
		#ifdef ENABLE_SUBVMASKING
		tempvSUBmask.data[0].key = tempvSUBmask.data[0].key | tempvmask.data[0].key;
		tempvSUBmask.data[0].value = tempvSUBmask.data[0].value | tempvmask.data[0].value;
		tempvSUBmask.data[1].key = tempvSUBmask.data[1].key | tempvmask.data[1].key;
		tempvSUBmask.data[1].value = tempvSUBmask.data[1].value | tempvmask.data[1].value;
		tempvSUBmask.data[2].key = tempvSUBmask.data[2].key | tempvmask.data[2].key;
		tempvSUBmask.data[2].value = tempvSUBmask.data[2].value | tempvmask.data[2].value;
		tempvSUBmask.data[3].key = tempvSUBmask.data[3].key | tempvmask.data[3].key;
		tempvSUBmask.data[3].value = tempvSUBmask.data[3].value | tempvmask.data[3].value;
		tempvSUBmask.data[4].key = tempvSUBmask.data[4].key | tempvmask.data[4].key;
		tempvSUBmask.data[4].value = tempvSUBmask.data[4].value | tempvmask.data[4].value;
		tempvSUBmask.data[5].key = tempvSUBmask.data[5].key | tempvmask.data[5].key;
		tempvSUBmask.data[5].value = tempvSUBmask.data[5].value | tempvmask.data[5].value;
		tempvSUBmask.data[6].key = tempvSUBmask.data[6].key | tempvmask.data[6].key;
		tempvSUBmask.data[6].value = tempvSUBmask.data[6].value | tempvmask.data[6].value;
		tempvSUBmask.data[7].key = tempvSUBmask.data[7].key | tempvmask.data[7].key;
		tempvSUBmask.data[7].value = tempvSUBmask.data[7].value | tempvmask.data[7].value;
		tempvSUBmask.data[8].key = tempvSUBmask.data[8].key | tempvmask.data[8].key;
		tempvSUBmask.data[8].value = tempvSUBmask.data[8].value | tempvmask.data[8].value;
		tempvSUBmask.data[9].key = tempvSUBmask.data[9].key | tempvmask.data[9].key;
		tempvSUBmask.data[9].value = tempvSUBmask.data[9].value | tempvmask.data[9].value;
		tempvSUBmask.data[10].key = tempvSUBmask.data[10].key | tempvmask.data[10].key;
		tempvSUBmask.data[10].value = tempvSUBmask.data[10].value | tempvmask.data[10].value;
		tempvSUBmask.data[11].key = tempvSUBmask.data[11].key | tempvmask.data[11].key;
		tempvSUBmask.data[11].value = tempvSUBmask.data[11].value | tempvmask.data[11].value;
		tempvSUBmask.data[12].key = tempvSUBmask.data[12].key | tempvmask.data[12].key;
		tempvSUBmask.data[12].value = tempvSUBmask.data[12].value | tempvmask.data[12].value;
		tempvSUBmask.data[13].key = tempvSUBmask.data[13].key | tempvmask.data[13].key;
		tempvSUBmask.data[13].value = tempvSUBmask.data[13].value | tempvmask.data[13].value;
		tempvSUBmask.data[14].key = tempvSUBmask.data[14].key | tempvmask.data[14].key;
		tempvSUBmask.data[14].value = tempvSUBmask.data[14].value | tempvmask.data[14].value;
		tempvSUBmask.data[15].key = tempvSUBmask.data[15].key | tempvmask.data[15].key;
		tempvSUBmask.data[15].value = tempvSUBmask.data[15].value | tempvmask.data[15].value;
		
		if(i % 8 == 7){ // CRITICAL AUTOMATEME.
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask0_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask0_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask0_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask0_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask0_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask0_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask0_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask0_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask0_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask0_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask0_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask0_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask0_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask0_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask0_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask0_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask0_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask0_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask0_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask0_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask0_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask0_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask0_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask0_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask0_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask0_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask0_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask0_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask0_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask0_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask0_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask0_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask1_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask1_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask1_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask1_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask1_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask1_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask1_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask1_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask1_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask1_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask1_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask1_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask1_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask1_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask1_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask1_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask1_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask1_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask1_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask1_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask1_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask1_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask1_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask1_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask1_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask1_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask1_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask1_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask1_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask1_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask1_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask1_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask2_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask2_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask2_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask2_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask2_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask2_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask2_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask2_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask2_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask2_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask2_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask2_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask2_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask2_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask2_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask2_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask2_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask2_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask2_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask2_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask2_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask2_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask2_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask2_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask2_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask2_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask2_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask2_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask2_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask2_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask2_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask2_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask3_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask3_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask3_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask3_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask3_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask3_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask3_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask3_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask3_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask3_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask3_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask3_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask3_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask3_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask3_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask3_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask3_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask3_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask3_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask3_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask3_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask3_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask3_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask3_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask3_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask3_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask3_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask3_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask3_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask3_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask3_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask3_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask4_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask4_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask4_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask4_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask4_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask4_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask4_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask4_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask4_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask4_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask4_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask4_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask4_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask4_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask4_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask4_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask4_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask4_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask4_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask4_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask4_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask4_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask4_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask4_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask4_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask4_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask4_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask4_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask4_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask4_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask4_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask4_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask5_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask5_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask5_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask5_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask5_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask5_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask5_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask5_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask5_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask5_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask5_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask5_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask5_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask5_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask5_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask5_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask5_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask5_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask5_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask5_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask5_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask5_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask5_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask5_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask5_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask5_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask5_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask5_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask5_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask5_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask5_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask5_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask6_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask6_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask6_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask6_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask6_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask6_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask6_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask6_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask6_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask6_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask6_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask6_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask6_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask6_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask6_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask6_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask6_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask6_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask6_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask6_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask6_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask6_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask6_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask6_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask6_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask6_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask6_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask6_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask6_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask6_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask6_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask6_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask7_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask7_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask7_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask7_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask7_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask7_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask7_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask7_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask7_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask7_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask7_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask7_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask7_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask7_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask7_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask7_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask7_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask7_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask7_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask7_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask7_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask7_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask7_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask7_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask7_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask7_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask7_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask7_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask7_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask7_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask7_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask7_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask8_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask8_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask8_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask8_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask8_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask8_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask8_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask8_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask8_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask8_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask8_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask8_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask8_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask8_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask8_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask8_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask8_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask8_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask8_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask8_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask8_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask8_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask8_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask8_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask8_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask8_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask8_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask8_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask8_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask8_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask8_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask8_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask9_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask9_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask9_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask9_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask9_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask9_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask9_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask9_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask9_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask9_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask9_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask9_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask9_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask9_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask9_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask9_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask9_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask9_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask9_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask9_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask9_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask9_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask9_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask9_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask9_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask9_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask9_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask9_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask9_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask9_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask9_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask9_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask10_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask10_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask10_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask10_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask10_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask10_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask10_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask10_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask10_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask10_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask10_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask10_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask10_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask10_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask10_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask10_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask10_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask10_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask10_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask10_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask10_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask10_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask10_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask10_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask10_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask10_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask10_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask10_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask10_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask10_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask10_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask10_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask11_subp[i/16].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask11_subp[i/16].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask11_subp[i/16].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask11_subp[i/16].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask11_subp[i/16].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask11_subp[i/16].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask11_subp[i/16].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask11_subp[i/16].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask11_subp[i/16].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask11_subp[i/16].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask11_subp[i/16].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask11_subp[i/16].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask11_subp[i/16].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask11_subp[i/16].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask11_subp[i/16].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask11_subp[i/16].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask11_subp[i/16].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask11_subp[i/16].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask11_subp[i/16].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask11_subp[i/16].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask11_subp[i/16].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask11_subp[i/16].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask11_subp[i/16].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask11_subp[i/16].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask11_subp[i/16].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask11_subp[i/16].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask11_subp[i/16].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask11_subp[i/16].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask11_subp[i/16].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask11_subp[i/16].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask11_subp[i/16].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask11_subp[i/16].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			
			tempvSUBmask.data[0].key = 0;
			tempvSUBmask.data[0].value = 0;
			tempvSUBmask.data[1].key = 0;
			tempvSUBmask.data[1].value = 0;
			tempvSUBmask.data[2].key = 0;
			tempvSUBmask.data[2].value = 0;
			tempvSUBmask.data[3].key = 0;
			tempvSUBmask.data[3].value = 0;
			tempvSUBmask.data[4].key = 0;
			tempvSUBmask.data[4].value = 0;
			tempvSUBmask.data[5].key = 0;
			tempvSUBmask.data[5].value = 0;
			tempvSUBmask.data[6].key = 0;
			tempvSUBmask.data[6].value = 0;
			tempvSUBmask.data[7].key = 0;
			tempvSUBmask.data[7].value = 0;
			tempvSUBmask.data[8].key = 0;
			tempvSUBmask.data[8].value = 0;
			tempvSUBmask.data[9].key = 0;
			tempvSUBmask.data[9].value = 0;
			tempvSUBmask.data[10].key = 0;
			tempvSUBmask.data[10].value = 0;
			tempvSUBmask.data[11].key = 0;
			tempvSUBmask.data[11].value = 0;
			tempvSUBmask.data[12].key = 0;
			tempvSUBmask.data[12].value = 0;
			tempvSUBmask.data[13].key = 0;
			tempvSUBmask.data[13].value = 0;
			tempvSUBmask.data[14].key = 0;
			tempvSUBmask.data[14].value = 0;
			tempvSUBmask.data[15].key = 0;
			tempvSUBmask.data[15].value = 0;
		}
		#endif 
	}
	
	#endif 
	return;
}

void acts_all::MEMACCESS_readmanypmask1(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs){
	LOADACTIVEPARTITIONS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
		#ifdef _WIDEWORD
		vmask0_p[i] = kvdram[offset_kvs + i].range(31, 0);
		#else
		vmask0_p[i] = kvdram[offset_kvs + i].data[0].key;
		#endif 
	}
	return;
}
void acts_all::MEMACCESS_readmanypmask2(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs){
	LOADACTIVEPARTITIONS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
		#ifdef _WIDEWORD
		vmask0_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask1_p[i] = kvdram[offset_kvs + i].range(31, 0);
		#else
		vmask0_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask1_p[i] = kvdram[offset_kvs + i].data[0].key;
		#endif 
	}
	return;
}
void acts_all::MEMACCESS_readmanypmask3(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs){
	LOADACTIVEPARTITIONS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
		#ifdef _WIDEWORD
		vmask0_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask1_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask2_p[i] = kvdram[offset_kvs + i].range(31, 0);
		#else
		vmask0_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask1_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask2_p[i] = kvdram[offset_kvs + i].data[0].key;
		#endif 
	}
	return;
}
void acts_all::MEMACCESS_readmanypmask4(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs){
	LOADACTIVEPARTITIONS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
		#ifdef _WIDEWORD
		vmask0_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask1_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask2_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask3_p[i] = kvdram[offset_kvs + i].range(31, 0);
		#else
		vmask0_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask1_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask2_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask3_p[i] = kvdram[offset_kvs + i].data[0].key;
		#endif 
	}
	return;
}
void acts_all::MEMACCESS_readmanypmask5(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs){
	LOADACTIVEPARTITIONS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
		#ifdef _WIDEWORD
		vmask0_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask1_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask2_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask3_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask4_p[i] = kvdram[offset_kvs + i].range(31, 0);
		#else
		vmask0_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask1_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask2_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask3_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask4_p[i] = kvdram[offset_kvs + i].data[0].key;
		#endif 
	}
	return;
}
void acts_all::MEMACCESS_readmanypmask6(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs){
	LOADACTIVEPARTITIONS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
		#ifdef _WIDEWORD
		vmask0_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask1_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask2_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask3_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask4_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask5_p[i] = kvdram[offset_kvs + i].range(31, 0);
		#else
		vmask0_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask1_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask2_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask3_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask4_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask5_p[i] = kvdram[offset_kvs + i].data[0].key;
		#endif 
	}
	return;
}
void acts_all::MEMACCESS_readmanypmask7(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs){
	LOADACTIVEPARTITIONS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
		#ifdef _WIDEWORD
		vmask0_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask1_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask2_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask3_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask4_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask5_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask6_p[i] = kvdram[offset_kvs + i].range(31, 0);
		#else
		vmask0_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask1_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask2_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask3_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask4_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask5_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask6_p[i] = kvdram[offset_kvs + i].data[0].key;
		#endif 
	}
	return;
}
void acts_all::MEMACCESS_readmanypmask8(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE],uint32_type vmask7_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs){
	LOADACTIVEPARTITIONS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
		#ifdef _WIDEWORD
		vmask0_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask1_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask2_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask3_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask4_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask5_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask6_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask7_p[i] = kvdram[offset_kvs + i].range(31, 0);
		#else
		vmask0_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask1_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask2_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask3_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask4_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask5_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask6_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask7_p[i] = kvdram[offset_kvs + i].data[0].key;
		#endif 
	}
	return;
}
void acts_all::MEMACCESS_readmanypmask9(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE],uint32_type vmask7_p[BLOCKRAM_SIZE],uint32_type vmask8_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs){
	LOADACTIVEPARTITIONS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
		#ifdef _WIDEWORD
		vmask0_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask1_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask2_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask3_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask4_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask5_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask6_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask7_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask8_p[i] = kvdram[offset_kvs + i].range(31, 0);
		#else
		vmask0_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask1_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask2_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask3_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask4_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask5_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask6_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask7_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask8_p[i] = kvdram[offset_kvs + i].data[0].key;
		#endif 
	}
	return;
}
void acts_all::MEMACCESS_readmanypmask10(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE],uint32_type vmask7_p[BLOCKRAM_SIZE],uint32_type vmask8_p[BLOCKRAM_SIZE],uint32_type vmask9_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs){
	LOADACTIVEPARTITIONS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
		#ifdef _WIDEWORD
		vmask0_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask1_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask2_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask3_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask4_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask5_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask6_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask7_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask8_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask9_p[i] = kvdram[offset_kvs + i].range(31, 0);
		#else
		vmask0_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask1_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask2_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask3_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask4_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask5_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask6_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask7_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask8_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask9_p[i] = kvdram[offset_kvs + i].data[0].key;
		#endif 
	}
	return;
}
void acts_all::MEMACCESS_readmanypmask11(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE],uint32_type vmask7_p[BLOCKRAM_SIZE],uint32_type vmask8_p[BLOCKRAM_SIZE],uint32_type vmask9_p[BLOCKRAM_SIZE],uint32_type vmask10_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs){
	LOADACTIVEPARTITIONS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
		#ifdef _WIDEWORD
		vmask0_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask1_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask2_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask3_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask4_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask5_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask6_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask7_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask8_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask9_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask10_p[i] = kvdram[offset_kvs + i].range(31, 0);
		#else
		vmask0_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask1_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask2_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask3_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask4_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask5_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask6_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask7_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask8_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask9_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask10_p[i] = kvdram[offset_kvs + i].data[0].key;
		#endif 
	}
	return;
}
void acts_all::MEMACCESS_readmanypmask12(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE],uint32_type vmask7_p[BLOCKRAM_SIZE],uint32_type vmask8_p[BLOCKRAM_SIZE],uint32_type vmask9_p[BLOCKRAM_SIZE],uint32_type vmask10_p[BLOCKRAM_SIZE],uint32_type vmask11_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs){
	LOADACTIVEPARTITIONS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
		#ifdef _WIDEWORD
		vmask0_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask1_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask2_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask3_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask4_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask5_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask6_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask7_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask8_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask9_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask10_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask11_p[i] = kvdram[offset_kvs + i].range(31, 0);
		#else
		vmask0_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask1_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask2_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask3_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask4_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask5_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask6_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask7_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask8_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask9_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask10_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask11_p[i] = kvdram[offset_kvs + i].data[0].key;
		#endif 
	}
	return;
}

// -------------------- others -------------------- //
void acts_all::MEMACCESS_readglobalstats(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], batch_type offset_kvs, globalparams_t globalparams){ 
	if(enable == OFF){ return; }
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("readglobalstats", offset_kvs + NUM_PARTITIONS, globalparams.BASEOFFSETKVS_STATSDRAM + KVSTATSDRAMSZ_KVS + 1, NAp, NAp, NAp);
	#endif
	
	READGLOBALSTATS_LOOP: for (buffer_type i=0; i<NUM_PARTITIONS; i++){
		#if NUM_EDGE_BANKS==0
			keyvalue_t kv;
			#ifdef _WIDEWORD
			kv.key = kvdram[offset_kvs + i].range(31, 0); 
			kv.value = kvdram[offset_kvs + i].range(63, 32); 
			#else 
			kv.key = kvdram[offset_kvs + i].data[0].key;
			kv.value = kvdram[offset_kvs + i].data[0].value;
			#endif 
			globalstatsbuffer[i] = kv;
		#else 
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
		#endif 
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printkeyvalues("readglobalstats.globalstatsbuffer", globalstatsbuffer, NUM_PARTITIONS); 
	#endif
	return;
}

void acts_all::MEMACCESS_saveglobalstats(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], batch_type offset_kvs, globalparams_t globalparams){ 
	if(enable == OFF){ return; }
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("saveglobalstats", offset_kvs + NUM_PARTITIONS, globalparams.BASEOFFSETKVS_STATSDRAM + KVSTATSDRAMSZ_KVS + 1, offset_kvs, NUM_PARTITIONS, KVSTATSDRAMSZ_KVS);
	#endif
	
	SAVEGLOBALSTATS_LOOP: for (buffer_type i=0; i<NUM_PARTITIONS; i++){
		#if NUM_EDGE_BANKS==0
			#ifdef _WIDEWORD
			kvdram[offset_kvs + i].range(31, 0) = globalstatsbuffer[i].key; 
			kvdram[offset_kvs + i].range(63, 32) = globalstatsbuffer[i].value; 
			#else
			kvdram[offset_kvs + i].data[0].key = globalstatsbuffer[i].key; 
			kvdram[offset_kvs + i].data[0].value = globalstatsbuffer[i].value; 
			#endif
		#else
			#ifdef _WIDEWORD
 if(globalparams.VARS_WORKBATCH == 0){
				kvdram[offset_kvs + i].range(31, 0) = globalstatsbuffer[i].key; 
				kvdram[offset_kvs + i].range(63, 32) = globalstatsbuffer[i].value; 
			}
else if(globalparams.VARS_WORKBATCH == 1){
				kvdram[offset_kvs + i].range(95, 64) = globalstatsbuffer[i].key; 
				kvdram[offset_kvs + i].range(127, 96) = globalstatsbuffer[i].value; 
			}
else if(globalparams.VARS_WORKBATCH == 2){
				kvdram[offset_kvs + i].range(159, 128) = globalstatsbuffer[i].key; 
				kvdram[offset_kvs + i].range(191, 160) = globalstatsbuffer[i].value; 
			}
else if(globalparams.VARS_WORKBATCH == 3){
				kvdram[offset_kvs + i].range(223, 192) = globalstatsbuffer[i].key; 
				kvdram[offset_kvs + i].range(255, 224) = globalstatsbuffer[i].value; 
			}
else if(globalparams.VARS_WORKBATCH == 4){
				kvdram[offset_kvs + i].range(287, 256) = globalstatsbuffer[i].key; 
				kvdram[offset_kvs + i].range(319, 288) = globalstatsbuffer[i].value; 
			}
else if(globalparams.VARS_WORKBATCH == 5){
				kvdram[offset_kvs + i].range(351, 320) = globalstatsbuffer[i].key; 
				kvdram[offset_kvs + i].range(383, 352) = globalstatsbuffer[i].value; 
			}
else if(globalparams.VARS_WORKBATCH == 6){
				kvdram[offset_kvs + i].range(415, 384) = globalstatsbuffer[i].key; 
				kvdram[offset_kvs + i].range(447, 416) = globalstatsbuffer[i].value; 
			}
else {
				kvdram[offset_kvs + i].range(479, 448) = globalstatsbuffer[i].key; 
				kvdram[offset_kvs + i].range(511, 480) = globalstatsbuffer[i].value; 
			}
			#else 
 if(globalparams.VARS_WORKBATCH == 0){
				kvdram[offset_kvs + i].data[0].key = globalstatsbuffer[i].key; 
				kvdram[offset_kvs + i].data[0].value = globalstatsbuffer[i].value; 
			}
else if(globalparams.VARS_WORKBATCH == 1){
				kvdram[offset_kvs + i].data[1].key = globalstatsbuffer[i].key; 
				kvdram[offset_kvs + i].data[1].value = globalstatsbuffer[i].value; 
			}
else if(globalparams.VARS_WORKBATCH == 2){
				kvdram[offset_kvs + i].data[2].key = globalstatsbuffer[i].key; 
				kvdram[offset_kvs + i].data[2].value = globalstatsbuffer[i].value; 
			}
else if(globalparams.VARS_WORKBATCH == 3){
				kvdram[offset_kvs + i].data[3].key = globalstatsbuffer[i].key; 
				kvdram[offset_kvs + i].data[3].value = globalstatsbuffer[i].value; 
			}
else if(globalparams.VARS_WORKBATCH == 4){
				kvdram[offset_kvs + i].data[4].key = globalstatsbuffer[i].key; 
				kvdram[offset_kvs + i].data[4].value = globalstatsbuffer[i].value; 
			}
else if(globalparams.VARS_WORKBATCH == 5){
				kvdram[offset_kvs + i].data[5].key = globalstatsbuffer[i].key; 
				kvdram[offset_kvs + i].data[5].value = globalstatsbuffer[i].value; 
			}
else if(globalparams.VARS_WORKBATCH == 6){
				kvdram[offset_kvs + i].data[6].key = globalstatsbuffer[i].key; 
				kvdram[offset_kvs + i].data[6].value = globalstatsbuffer[i].value; 
			}
else {
				kvdram[offset_kvs + i].data[7].key = globalstatsbuffer[i].key; 
				kvdram[offset_kvs + i].data[7].value = globalstatsbuffer[i].value; 
			}
			#endif
		#endif 

		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalvar_savestats_counttotalstatswritten(VECTOR_SIZE);
		#endif
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->printkeyvalues("MEMACCESS_saveglobalstats.globalstatsbuffer", globalstatsbuffer, NUM_PARTITIONS); 
	#endif
	return;
}

tuple_t acts_all::MEMACCESS_getvptrs( uint512_dt * kvdram, unsigned int beginoffset, unsigned int endoffset, unsigned int edgebankID){
	#pragma HLS INLINE
	
	keyy_t beginvptr = 0;
	keyy_t endvptr = 0;
	
	#if NUM_EDGE_BANKS==0
		#ifdef _WIDEWORD
		beginvptr = kvdram[beginoffset].range(31, 0);
		endvptr = kvdram[endoffset].range(31, 0); 
		#else 
		beginvptr = kvdram[beginoffset].data[0].key;
		endvptr = kvdram[endoffset].data[0].key;
		#endif 
	#else
		if(edgebankID == 0){
			#ifdef _WIDEWORD
			beginvptr = edges0[beginoffset].range(31, 0);
			endvptr = edges0[endoffset].range(31, 0); 
			#else 
			beginvptr = edges0[beginoffset].data[0].key;
			endvptr = edges0[endoffset].data[0].key;
			#endif 
		} 
		#if NUM_EDGE_BANKS>1
		else if(edgebankID == 1){
			#ifdef _WIDEWORD
			beginvptr = edges1[beginoffset].range(31, 0);
			endvptr = edges1[endoffset].range(31, 0); 
			#else 
			beginvptr = edges1[beginoffset].data[0].key;
			endvptr = edges1[endoffset].data[0].key;
			#endif 
		} 
		#if NUM_EDGE_BANKS>2
		else if(edgebankID == 2){
			#ifdef _WIDEWORD
			beginvptr = edges2[beginoffset].range(31, 0);
			endvptr = edges2[endoffset].range(31, 0); 
			#else 
			beginvptr = edges2[beginoffset].data[0].key;
			endvptr = edges2[endoffset].data[0].key;
			#endif 
		} 
		#if NUM_EDGE_BANKS>3
		else if(edgebankID == 3){
			#ifdef _WIDEWORD
			beginvptr = edges3[beginoffset].range(31, 0);
			endvptr = edges3[endoffset].range(31, 0); 
			#else 
			beginvptr = edges3[beginoffset].data[0].key;
			endvptr = edges3[endoffset].data[0].key;
			#endif 
		} 
		#if NUM_EDGE_BANKS>4
		else if(edgebankID == 4){
			#ifdef _WIDEWORD
			beginvptr = edges4[beginoffset].range(31, 0);
			endvptr = edges4[endoffset].range(31, 0); 
			#else 
			beginvptr = edges4[beginoffset].data[0].key;
			endvptr = edges4[endoffset].data[0].key;
			#endif 
		} 
		#if NUM_EDGE_BANKS>5
		else if(edgebankID == 5){
			#ifdef _WIDEWORD
			beginvptr = edges5[beginoffset].range(31, 0);
			endvptr = edges5[endoffset].range(31, 0); 
			#else 
			beginvptr = edges5[beginoffset].data[0].key;
			endvptr = edges5[endoffset].data[0].key;
			#endif 
		} 
		#if NUM_EDGE_BANKS>6
		else if(edgebankID == 6){
			#ifdef _WIDEWORD
			beginvptr = edges6[beginoffset].range(31, 0);
			endvptr = edges6[endoffset].range(31, 0); 
			#else 
			beginvptr = edges6[beginoffset].data[0].key;
			endvptr = edges6[endoffset].data[0].key;
			#endif 
		}
		#if NUM_EDGE_BANKS>7
		else if(edgebankID == 7){
			#ifdef _WIDEWORD
			beginvptr = edges7[beginoffset].range(31, 0);
			endvptr = edges7[endoffset].range(31, 0); 
			#else 
			beginvptr = edges7[beginoffset].data[0].key;
			endvptr = edges7[endoffset].data[0].key;
			#endif 
		}
	#endif
	#endif 
	#endif 
	#endif 
	#endif
	#endif 
	#endif
	else {
		#ifdef _WIDEWORD 
		beginvptr = NAp; 
		endvptr = NAp; 
		#else 
		beginvptr = NAp;
		endvptr = NAp;
		#endif 
	}
	#endif 

	tuple_t t; t.A = beginvptr; t.B = endvptr;
	return t;
}

unsigned int acts_all::MEMACCESS_getvptr(uint512_dt * kvdram, unsigned int baseoffset_kvs, unsigned int offset){
	keyvalue_t vptr_kv;
	
	uint512_dt V = kvdram[baseoffset_kvs + (offset / 16)];
	unsigned int M = (offset % 16) / 2;
	
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

tuple_t acts_all::MEMACCESS_getvptrs_opt( uint512_dt * kvdram, unsigned int baseoffset_kvs, unsigned int beginoffset, unsigned int endoffset, unsigned int edgebankID){
	#pragma HLS INLINE
	keyy_t beginvptr = 0;
	keyy_t endvptr = 0;
	
	beginoffset = beginoffset / VPTR_SHRINK_RATIO; // convert-to-appropriate-skip-format
	endoffset = endoffset / VPTR_SHRINK_RATIO;
	
	#if NUM_EDGE_BANKS==0
		beginvptr = MEMACCESS_getvptr(kvdram, baseoffset_kvs, beginoffset);
		endvptr = MEMACCESS_getvptr(kvdram, baseoffset_kvs, endoffset);
	#else
		if(edgebankID == 0){
			beginvptr = MEMACCESS_getvptr(edges0, baseoffset_kvs, beginoffset);
			endvptr = MEMACCESS_getvptr(edges0, baseoffset_kvs, endoffset);
		} 
		#if NUM_EDGE_BANKS>1
		else if(edgebankID == 1){
			beginvptr = MEMACCESS_getvptr(edges1, baseoffset_kvs, beginoffset);
			endvptr = MEMACCESS_getvptr(edges1, baseoffset_kvs, endoffset);
		} 
		#if NUM_EDGE_BANKS>2
		else if(edgebankID == 2){
			beginvptr = MEMACCESS_getvptr(edges2, baseoffset_kvs, beginoffset);
			endvptr = MEMACCESS_getvptr(edges2, baseoffset_kvs, endoffset);
		} 
		#if NUM_EDGE_BANKS>3
		else if(edgebankID == 3){
			beginvptr = MEMACCESS_getvptr(edges3, baseoffset_kvs, beginoffset);
			endvptr = MEMACCESS_getvptr(edges3, baseoffset_kvs, endoffset);
		} 
		#if NUM_EDGE_BANKS>4
		else if(edgebankID == 4){
			beginvptr = MEMACCESS_getvptr(edges4, baseoffset_kvs, beginoffset);
			endvptr = MEMACCESS_getvptr(edges4, baseoffset_kvs, endoffset);
		} 
		#if NUM_EDGE_BANKS>5
		else if(edgebankID == 5){
			beginvptr = MEMACCESS_getvptr(edges5, baseoffset_kvs, beginoffset);
			endvptr = MEMACCESS_getvptr(edges5, baseoffset_kvs, endoffset);
		} 
		#if NUM_EDGE_BANKS>6
		else if(edgebankID == 6){
			beginvptr = MEMACCESS_getvptr(edges6, baseoffset_kvs, beginoffset);
			endvptr = MEMACCESS_getvptr(edges6, baseoffset_kvs, endoffset);
		}
		#if NUM_EDGE_BANKS>7
		else if(edgebankID == 7){
			beginvptr = MEMACCESS_getvptr(edges7, baseoffset_kvs, beginoffset);
			endvptr = MEMACCESS_getvptr(edges7, baseoffset_kvs, endoffset);
		}
	#endif
	#endif 
	#endif 
	#endif 
	#endif
	#endif 
	#endif
	else {
		#ifdef _WIDEWORD 
		beginvptr = NAp; 
		endvptr = NAp; 
		#else 
		beginvptr = NAp;
		endvptr = NAp;
		#endif 
	}
	#endif 

	tuple_t t; t.A = beginvptr; t.B = endvptr;
	return t;
}

void acts_all::MEMACCESS_copyvs(uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], globalparams_t globalparamsK){
	analysis_type analysis_treedepth = TREE_DEPTH;
	analysis_type analysis_loop1 = 1;
	
	travstate_t rtravstate;
	
	#ifdef ENABLERECURSIVEPARTITIONING
	step_type currentLOP = globalparamsK.ACTSPARAMS_TREEDEPTH;
	batch_type num_source_partitions = UTIL_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH);
	#else
	step_type currentLOP = globalparamsK.ACTSPARAMS_TREEDEPTH + 1;
	batch_type num_source_partitions = NUM_PARTITIONS;
	#endif
	buffer_type reducebuffersz = globalparamsK.SIZE_REDUCE / 2; // 512
	
	unsigned int sourcestatsmarker = 0;
	#ifdef ENABLERECURSIVEPARTITIONING
	LOADSRCVS_LOOP1: for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++)
	#else 
	LOADSRCVS_LOOP1: for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH; k++)
	#endif 
	{
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_treedepth avg=analysis_treedepth
		sourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); 
	}
	
	unsigned int gmask_buffer[BLOCKRAM_SIZE]; // AUTOMATEME.
	LOADSRCVS_LOOP2: for(unsigned int iterationidx=0; iterationidx<num_source_partitions; iterationidx++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		gmask_buffer[iterationidx] = kvdram[globalparamsK.BASEOFFSETKVS_VERTICESPARTITIONMASK + iterationidx].range(31, 0);
		#else 
		gmask_buffer[iterationidx] = kvdram[globalparamsK.BASEOFFSETKVS_VERTICESPARTITIONMASK + iterationidx].data[0].key;
		#endif
	}
	
	LOADSRCVS_LOOP3: for(batch_type iterationidx=0; iterationidx<num_source_partitions; iterationidx+=1){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loop1 avg=analysis_loop1
		if(gmask_buffer[iterationidx] > 0){
			MEMACCESS_readvdata(ON, kvdram, globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + (iterationidx * reducebuffersz * FETFACTOR), vbuffer, 0, 0, reducebuffersz, globalparamsK);
			MEMACCESS_readvdata(ON, kvdram, globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + (iterationidx * reducebuffersz * FETFACTOR) + reducebuffersz, vbuffer, 8, 0, reducebuffersz, globalparamsK);
			
			MEMACCESS_savevdata(ON, kvdram, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + (iterationidx * reducebuffersz * FETFACTOR), vbuffer, 0, 0, reducebuffersz, globalparamsK);
			MEMACCESS_savevdata(ON, kvdram, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + (iterationidx * reducebuffersz * FETFACTOR) + reducebuffersz, vbuffer, 8, 0, reducebuffersz, globalparamsK);
		}
	}
	return;
}

void acts_all::MEMACCESS_copystats(uint512_dt * edges, uint512_dt * kvdram, globalparams_t globalparamsE, globalparams_t globalparamsK){
	analysis_type analysis_treedepth = TREE_DEPTH;
	analysis_type analysis_loop1 = 1;
	
	COPYSTATS_LOOP1: for(unsigned int k=0; k<globalparamsK.SIZE_KVSTATSDRAM; k++){
	#pragma HLS PIPELINE II=1
		kvdram[globalparamsK.BASEOFFSETKVS_STATSDRAM + k] = edges[globalparamsE.BASEOFFSETKVS_STATSDRAM + k];
	}
	return;
}

void acts_all::MEMACCESS_copyallstats( uint512_dt * kvdram, globalparams_t globalparamsE, globalparams_t globalparamsK, unsigned int edgebankID){
	analysis_type analysis_treedepth = TREE_DEPTH;
	analysis_type analysis_loop1 = 1;

	return;
}

void acts_all::MEMACCESS_commitkvstats(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams, unsigned int offset){
	unsigned int totalnumpartitionsb4last = 0;
	RETRIEVEKVSTATS_LOOP1: for(unsigned int k=0; k<globalparams.ACTSPARAMS_TREEDEPTH; k++){ totalnumpartitionsb4last += (1 << (NUM_PARTITIONS_POW * k)); }
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

void acts_all::MEMACCESS_commitkvstats2(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams, unsigned int offset, unsigned int size){
	for(unsigned int k=0; k<size; k++){
		#ifdef _WIDEWORD
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + offset + k].range(63, 32) = buffer[k]; 
		#else
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + offset  + k].data[0].value = buffer[k]; 
		#endif
	}
	return;
}

void acts_all::MEMACCESS_retreievekvstats(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams, unsigned int offset, unsigned int size){
	for(unsigned int k=0; k<size; k++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		buffer[k] = kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + offset + k].range(63, 32); 
		#else
		buffer[k] = kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + offset + k].data[0].value; 
		#endif
	}
}