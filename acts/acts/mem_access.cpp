#include "mem_access.h"
using namespace std;

#ifdef SW
mem_access::mem_access(){ actsutilityobj = new actsutility(); acts_utilobj = new acts_util(); }
mem_access::~mem_access(){}
#endif

fetchmessage_t
	#ifdef SW 
	mem_access::
	#endif 
MEMACCESS_readkeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs, travstate_t travstate, globalparams_t globalparams){
	fetchmessage_t fetchmessage;
	fetchmessage.chunksize_kvs = -1;
	fetchmessage.nextoffset_kvs = -1;
	if(enable == OFF){ return fetchmessage; }
	
	analysis_type analysis_loopcount = BLOCKRAM_SIZE;
	buffer_type chunk_size = acts_utilobj->UTIL_getchunksize_kvs(size_kvs, travstate, 0);
	
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
		
		buffer[0][i] = acts_utilobj->UTIL_GETKV(mykeyvalue0);
		buffer[1][i] = acts_utilobj->UTIL_GETKV(mykeyvalue1);
		buffer[2][i] = acts_utilobj->UTIL_GETKV(mykeyvalue2);
		buffer[3][i] = acts_utilobj->UTIL_GETKV(mykeyvalue3);
		buffer[4][i] = acts_utilobj->UTIL_GETKV(mykeyvalue4);
		buffer[5][i] = acts_utilobj->UTIL_GETKV(mykeyvalue5);
		buffer[6][i] = acts_utilobj->UTIL_GETKV(mykeyvalue6);
		buffer[7][i] = acts_utilobj->UTIL_GETKV(mykeyvalue7);
		
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"readkeyvalues:: keyvalues read: offset_kvs: "<<offset_kvs<<", size_kvs: "<<size_kvs<<", chunk_size: "<<chunk_size<<endl;
	#endif
	return fetchmessage;
}

void
	#ifdef SW 
	mem_access::
	#endif 
MEMACCESS_savekeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_buffer_t buffer[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_t * globalcapsule, keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], batch_type globalbaseaddress_kvs, globalparams_t globalparams){				
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
		buffer_type size_kvs = acts_utilobj->UTIL_getpartitionwritesz(realsize_kvs, bramoffset_kvs);
		
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("savekeyvalues 23", bramoffset_kvs + size_kvs, DESTBLOCKRAM_SIZE + 1, p, NAp, NAp);
		actsutilityobj->checkoutofbounds("savekeyvalues 25", ((globalcapsule[p].key + globalcapsule[p].value) / VECTOR_SIZE), globalparams.SIZE_KVDRAM + 1, p, NAp, NAp);
		#endif
		if(realsize_kvs > 0){
			SAVEPARTITIONS_LOOP1B: for(buffer_type i=0; i<size_kvs; i++){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_destpartitionsz avg=analysis_destpartitionsz
			#pragma HLS PIPELINE II=1
				keyvalue_t mykeyvalue0 = acts_utilobj->UTIL_GETKV(buffer[0][bramoffset_kvs + i]);
				keyvalue_t mykeyvalue1 = acts_utilobj->UTIL_GETKV(buffer[1][bramoffset_kvs + i]);
				keyvalue_t mykeyvalue2 = acts_utilobj->UTIL_GETKV(buffer[2][bramoffset_kvs + i]);
				keyvalue_t mykeyvalue3 = acts_utilobj->UTIL_GETKV(buffer[3][bramoffset_kvs + i]);
				keyvalue_t mykeyvalue4 = acts_utilobj->UTIL_GETKV(buffer[4][bramoffset_kvs + i]);
				keyvalue_t mykeyvalue5 = acts_utilobj->UTIL_GETKV(buffer[5][bramoffset_kvs + i]);
				keyvalue_t mykeyvalue6 = acts_utilobj->UTIL_GETKV(buffer[6][bramoffset_kvs + i]);
				keyvalue_t mykeyvalue7 = acts_utilobj->UTIL_GETKV(buffer[7][bramoffset_kvs + i]);
				
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

void
	#ifdef SW 
	mem_access::
	#endif 
MEMACCESS_readkeyvalues(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_buffer_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams){
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
		
		buffer[0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV(mykeyvalue0);
		buffer[1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV(mykeyvalue1);
		buffer[2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV(mykeyvalue2);
		buffer[3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV(mykeyvalue3);
		buffer[4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV(mykeyvalue4);
		buffer[5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV(mykeyvalue5);
		buffer[6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV(mykeyvalue6);
		buffer[7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV(mykeyvalue7);
		
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

void
	#ifdef SW 
	mem_access::
	#endif 
MEMACCESS_savekeyvalues(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_buffer_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount =  BLOCKRAM_SIZE;
	
	SAVEKEYVALUES2_LOOP: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE II=1
		keyvalue_t mykeyvalue0 = acts_utilobj->UTIL_GETKV(buffer[0][bufferoffset_kvs + i]);
		keyvalue_t mykeyvalue1 = acts_utilobj->UTIL_GETKV(buffer[1][bufferoffset_kvs + i]);
		keyvalue_t mykeyvalue2 = acts_utilobj->UTIL_GETKV(buffer[2][bufferoffset_kvs + i]);
		keyvalue_t mykeyvalue3 = acts_utilobj->UTIL_GETKV(buffer[3][bufferoffset_kvs + i]);
		keyvalue_t mykeyvalue4 = acts_utilobj->UTIL_GETKV(buffer[4][bufferoffset_kvs + i]);
		keyvalue_t mykeyvalue5 = acts_utilobj->UTIL_GETKV(buffer[5][bufferoffset_kvs + i]);
		keyvalue_t mykeyvalue6 = acts_utilobj->UTIL_GETKV(buffer[6][bufferoffset_kvs + i]);
		keyvalue_t mykeyvalue7 = acts_utilobj->UTIL_GETKV(buffer[7][bufferoffset_kvs + i]);
	
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

void
	#ifdef SW 
	mem_access::
	#endif 
MEMACCESS_readvdata(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams){
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
		
		buffer[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		
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
	mem_access::
	#endif 
MEMACCESS_savevdata(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount =  REDUCESZ / 2;
	
	SAVEVDATA_LOOP: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE II=1
		keyvalue_t mykeyvalue0 = acts_utilobj->UTIL_GETKV2(buffer[begincol + 0][bufferoffset_kvs + i]);
		keyvalue_t mykeyvalue1 = acts_utilobj->UTIL_GETKV2(buffer[begincol + 1][bufferoffset_kvs + i]);
		keyvalue_t mykeyvalue2 = acts_utilobj->UTIL_GETKV2(buffer[begincol + 2][bufferoffset_kvs + i]);
		keyvalue_t mykeyvalue3 = acts_utilobj->UTIL_GETKV2(buffer[begincol + 3][bufferoffset_kvs + i]);
		keyvalue_t mykeyvalue4 = acts_utilobj->UTIL_GETKV2(buffer[begincol + 4][bufferoffset_kvs + i]);
		keyvalue_t mykeyvalue5 = acts_utilobj->UTIL_GETKV2(buffer[begincol + 5][bufferoffset_kvs + i]);
		keyvalue_t mykeyvalue6 = acts_utilobj->UTIL_GETKV2(buffer[begincol + 6][bufferoffset_kvs + i]);
		keyvalue_t mykeyvalue7 = acts_utilobj->UTIL_GETKV2(buffer[begincol + 7][bufferoffset_kvs + i]);
	
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

void
	#ifdef SW 
	mem_access::
	#endif 
MEMACCESS_loadvmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount1 = BLOCKRAM_SIZE;
	analysis_type analysis_loopcount2 = BLOCKRAM_SIZE / 16;
	analysis_type analysis_loopcount3 = BLOCKRAM_SIZE;
	
	buffer_type transfsize = size_kvs * 16;
	uint32_type bitsbuffer[MAXREDUCEBUFFERSZ];

	LOADVMASKS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
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

	unitBRAMwidth_type tempvmask;
	#pragma HLS DATA_PACK variable = tempvmask			
	LOADVMASKS_LOOP2: for (buffer_type i=0; i<transfsize; i++){ // transfsize, reducebuffersz
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount3 avg=analysis_loopcount3
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		tempvmask.data[0].key = bitsbuffer[i].range(0, 0);
		tempvmask.data[0].value = bitsbuffer[i].range(1, 1);
		tempvmask.data[1].key = bitsbuffer[i].range(2, 2);
		tempvmask.data[1].value = bitsbuffer[i].range(3, 3);
		tempvmask.data[2].key = bitsbuffer[i].range(4, 4);
		tempvmask.data[2].value = bitsbuffer[i].range(5, 5);
		tempvmask.data[3].key = bitsbuffer[i].range(6, 6);
		tempvmask.data[3].value = bitsbuffer[i].range(7, 7);
		tempvmask.data[4].key = bitsbuffer[i].range(8, 8);
		tempvmask.data[4].value = bitsbuffer[i].range(9, 9);
		tempvmask.data[5].key = bitsbuffer[i].range(10, 10);
		tempvmask.data[5].value = bitsbuffer[i].range(11, 11);
		tempvmask.data[6].key = bitsbuffer[i].range(12, 12);
		tempvmask.data[6].value = bitsbuffer[i].range(13, 13);
		tempvmask.data[7].key = bitsbuffer[i].range(14, 14);
		tempvmask.data[7].value = bitsbuffer[i].range(15, 15);
		tempvmask.data[8].key = bitsbuffer[i].range(16, 16);
		tempvmask.data[8].value = bitsbuffer[i].range(17, 17);
		tempvmask.data[9].key = bitsbuffer[i].range(18, 18);
		tempvmask.data[9].value = bitsbuffer[i].range(19, 19);
		tempvmask.data[10].key = bitsbuffer[i].range(20, 20);
		tempvmask.data[10].value = bitsbuffer[i].range(21, 21);
		tempvmask.data[11].key = bitsbuffer[i].range(22, 22);
		tempvmask.data[11].value = bitsbuffer[i].range(23, 23);
		tempvmask.data[12].key = bitsbuffer[i].range(24, 24);
		tempvmask.data[12].value = bitsbuffer[i].range(25, 25);
		tempvmask.data[13].key = bitsbuffer[i].range(26, 26);
		tempvmask.data[13].value = bitsbuffer[i].range(27, 27);
		tempvmask.data[14].key = bitsbuffer[i].range(28, 28);
		tempvmask.data[14].value = bitsbuffer[i].range(29, 29);
		tempvmask.data[15].key = bitsbuffer[i].range(30, 30);
		tempvmask.data[15].value = bitsbuffer[i].range(31, 31);
		#else 
		tempvmask.data[0].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		tempvmask.data[0].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		tempvmask.data[1].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		tempvmask.data[1].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		tempvmask.data[2].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		tempvmask.data[2].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		tempvmask.data[3].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		tempvmask.data[3].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		tempvmask.data[4].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		tempvmask.data[4].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		tempvmask.data[5].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		tempvmask.data[5].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		tempvmask.data[6].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		tempvmask.data[6].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		tempvmask.data[7].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		tempvmask.data[7].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		tempvmask.data[8].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		tempvmask.data[8].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		tempvmask.data[9].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		tempvmask.data[9].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		tempvmask.data[10].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		tempvmask.data[10].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		tempvmask.data[11].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		tempvmask.data[11].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		tempvmask.data[12].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		tempvmask.data[12].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		tempvmask.data[13].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		tempvmask.data[13].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		tempvmask.data[14].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		tempvmask.data[14].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		tempvmask.data[15].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		tempvmask.data[15].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		
		vmask[i] = tempvmask;	
	}
	return;
}

void
	#ifdef SW 
	mem_access::
	#endif 
MEMACCESS_loadmany1vmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount1 = BLOCKRAM_SIZE;
	analysis_type analysis_loopcount2 = BLOCKRAM_SIZE / 16;
	analysis_type analysis_loopcount3 = BLOCKRAM_SIZE;
	
	buffer_type transfsize = size_kvs * 16;
	uint32_type bitsbuffer[MAXREDUCEBUFFERSZ];

	LOADVMASKS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
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

	unitBRAMwidth_type tempvmask;
	#pragma HLS DATA_PACK variable = tempvmask	
	LOADVMASKS_LOOP2: for (buffer_type i=0; i<transfsize; i++){ // transfsize, reducebuffersz
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount3 avg=analysis_loopcount3
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		tempvmask.data[0].key = bitsbuffer[i].range(0, 0);
		tempvmask.data[0].value = bitsbuffer[i].range(1, 1);
		tempvmask.data[1].key = bitsbuffer[i].range(2, 2);
		tempvmask.data[1].value = bitsbuffer[i].range(3, 3);
		tempvmask.data[2].key = bitsbuffer[i].range(4, 4);
		tempvmask.data[2].value = bitsbuffer[i].range(5, 5);
		tempvmask.data[3].key = bitsbuffer[i].range(6, 6);
		tempvmask.data[3].value = bitsbuffer[i].range(7, 7);
		tempvmask.data[4].key = bitsbuffer[i].range(8, 8);
		tempvmask.data[4].value = bitsbuffer[i].range(9, 9);
		tempvmask.data[5].key = bitsbuffer[i].range(10, 10);
		tempvmask.data[5].value = bitsbuffer[i].range(11, 11);
		tempvmask.data[6].key = bitsbuffer[i].range(12, 12);
		tempvmask.data[6].value = bitsbuffer[i].range(13, 13);
		tempvmask.data[7].key = bitsbuffer[i].range(14, 14);
		tempvmask.data[7].value = bitsbuffer[i].range(15, 15);
		tempvmask.data[8].key = bitsbuffer[i].range(16, 16);
		tempvmask.data[8].value = bitsbuffer[i].range(17, 17);
		tempvmask.data[9].key = bitsbuffer[i].range(18, 18);
		tempvmask.data[9].value = bitsbuffer[i].range(19, 19);
		tempvmask.data[10].key = bitsbuffer[i].range(20, 20);
		tempvmask.data[10].value = bitsbuffer[i].range(21, 21);
		tempvmask.data[11].key = bitsbuffer[i].range(22, 22);
		tempvmask.data[11].value = bitsbuffer[i].range(23, 23);
		tempvmask.data[12].key = bitsbuffer[i].range(24, 24);
		tempvmask.data[12].value = bitsbuffer[i].range(25, 25);
		tempvmask.data[13].key = bitsbuffer[i].range(26, 26);
		tempvmask.data[13].value = bitsbuffer[i].range(27, 27);
		tempvmask.data[14].key = bitsbuffer[i].range(28, 28);
		tempvmask.data[14].value = bitsbuffer[i].range(29, 29);
		tempvmask.data[15].key = bitsbuffer[i].range(30, 30);
		tempvmask.data[15].value = bitsbuffer[i].range(31, 31);
		#else 
		tempvmask.data[0].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		tempvmask.data[0].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		tempvmask.data[1].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		tempvmask.data[1].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		tempvmask.data[2].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		tempvmask.data[2].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		tempvmask.data[3].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		tempvmask.data[3].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		tempvmask.data[4].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		tempvmask.data[4].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		tempvmask.data[5].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		tempvmask.data[5].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		tempvmask.data[6].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		tempvmask.data[6].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		tempvmask.data[7].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		tempvmask.data[7].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		tempvmask.data[8].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		tempvmask.data[8].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		tempvmask.data[9].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		tempvmask.data[9].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		tempvmask.data[10].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		tempvmask.data[10].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		tempvmask.data[11].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		tempvmask.data[11].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		tempvmask.data[12].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		tempvmask.data[12].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		tempvmask.data[13].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		tempvmask.data[13].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		tempvmask.data[14].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		tempvmask.data[14].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		tempvmask.data[15].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		tempvmask.data[15].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		
		// mask 
		vmask0[i] = tempvmask;	
	}
	return;
}
void
	#ifdef SW 
	mem_access::
	#endif 
MEMACCESS_loadmany2vmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount1 = BLOCKRAM_SIZE;
	analysis_type analysis_loopcount2 = BLOCKRAM_SIZE / 16;
	analysis_type analysis_loopcount3 = BLOCKRAM_SIZE;
	
	buffer_type transfsize = size_kvs * 16;
	uint32_type bitsbuffer[MAXREDUCEBUFFERSZ];

	LOADVMASKS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
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

	unitBRAMwidth_type tempvmask;
	#pragma HLS DATA_PACK variable = tempvmask	
	LOADVMASKS_LOOP2: for (buffer_type i=0; i<transfsize; i++){ // transfsize, reducebuffersz
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount3 avg=analysis_loopcount3
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		tempvmask.data[0].key = bitsbuffer[i].range(0, 0);
		tempvmask.data[0].value = bitsbuffer[i].range(1, 1);
		tempvmask.data[1].key = bitsbuffer[i].range(2, 2);
		tempvmask.data[1].value = bitsbuffer[i].range(3, 3);
		tempvmask.data[2].key = bitsbuffer[i].range(4, 4);
		tempvmask.data[2].value = bitsbuffer[i].range(5, 5);
		tempvmask.data[3].key = bitsbuffer[i].range(6, 6);
		tempvmask.data[3].value = bitsbuffer[i].range(7, 7);
		tempvmask.data[4].key = bitsbuffer[i].range(8, 8);
		tempvmask.data[4].value = bitsbuffer[i].range(9, 9);
		tempvmask.data[5].key = bitsbuffer[i].range(10, 10);
		tempvmask.data[5].value = bitsbuffer[i].range(11, 11);
		tempvmask.data[6].key = bitsbuffer[i].range(12, 12);
		tempvmask.data[6].value = bitsbuffer[i].range(13, 13);
		tempvmask.data[7].key = bitsbuffer[i].range(14, 14);
		tempvmask.data[7].value = bitsbuffer[i].range(15, 15);
		tempvmask.data[8].key = bitsbuffer[i].range(16, 16);
		tempvmask.data[8].value = bitsbuffer[i].range(17, 17);
		tempvmask.data[9].key = bitsbuffer[i].range(18, 18);
		tempvmask.data[9].value = bitsbuffer[i].range(19, 19);
		tempvmask.data[10].key = bitsbuffer[i].range(20, 20);
		tempvmask.data[10].value = bitsbuffer[i].range(21, 21);
		tempvmask.data[11].key = bitsbuffer[i].range(22, 22);
		tempvmask.data[11].value = bitsbuffer[i].range(23, 23);
		tempvmask.data[12].key = bitsbuffer[i].range(24, 24);
		tempvmask.data[12].value = bitsbuffer[i].range(25, 25);
		tempvmask.data[13].key = bitsbuffer[i].range(26, 26);
		tempvmask.data[13].value = bitsbuffer[i].range(27, 27);
		tempvmask.data[14].key = bitsbuffer[i].range(28, 28);
		tempvmask.data[14].value = bitsbuffer[i].range(29, 29);
		tempvmask.data[15].key = bitsbuffer[i].range(30, 30);
		tempvmask.data[15].value = bitsbuffer[i].range(31, 31);
		#else 
		tempvmask.data[0].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		tempvmask.data[0].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		tempvmask.data[1].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		tempvmask.data[1].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		tempvmask.data[2].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		tempvmask.data[2].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		tempvmask.data[3].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		tempvmask.data[3].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		tempvmask.data[4].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		tempvmask.data[4].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		tempvmask.data[5].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		tempvmask.data[5].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		tempvmask.data[6].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		tempvmask.data[6].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		tempvmask.data[7].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		tempvmask.data[7].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		tempvmask.data[8].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		tempvmask.data[8].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		tempvmask.data[9].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		tempvmask.data[9].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		tempvmask.data[10].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		tempvmask.data[10].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		tempvmask.data[11].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		tempvmask.data[11].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		tempvmask.data[12].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		tempvmask.data[12].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		tempvmask.data[13].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		tempvmask.data[13].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		tempvmask.data[14].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		tempvmask.data[14].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		tempvmask.data[15].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		tempvmask.data[15].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		
		// mask 
		vmask0[i] = tempvmask;	
		vmask1[i] = tempvmask;	
	}
	return;
}
void
	#ifdef SW 
	mem_access::
	#endif 
MEMACCESS_loadmany3vmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount1 = BLOCKRAM_SIZE;
	analysis_type analysis_loopcount2 = BLOCKRAM_SIZE / 16;
	analysis_type analysis_loopcount3 = BLOCKRAM_SIZE;
	
	buffer_type transfsize = size_kvs * 16;
	uint32_type bitsbuffer[MAXREDUCEBUFFERSZ];

	LOADVMASKS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
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

	unitBRAMwidth_type tempvmask;
	#pragma HLS DATA_PACK variable = tempvmask	
	LOADVMASKS_LOOP2: for (buffer_type i=0; i<transfsize; i++){ // transfsize, reducebuffersz
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount3 avg=analysis_loopcount3
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		tempvmask.data[0].key = bitsbuffer[i].range(0, 0);
		tempvmask.data[0].value = bitsbuffer[i].range(1, 1);
		tempvmask.data[1].key = bitsbuffer[i].range(2, 2);
		tempvmask.data[1].value = bitsbuffer[i].range(3, 3);
		tempvmask.data[2].key = bitsbuffer[i].range(4, 4);
		tempvmask.data[2].value = bitsbuffer[i].range(5, 5);
		tempvmask.data[3].key = bitsbuffer[i].range(6, 6);
		tempvmask.data[3].value = bitsbuffer[i].range(7, 7);
		tempvmask.data[4].key = bitsbuffer[i].range(8, 8);
		tempvmask.data[4].value = bitsbuffer[i].range(9, 9);
		tempvmask.data[5].key = bitsbuffer[i].range(10, 10);
		tempvmask.data[5].value = bitsbuffer[i].range(11, 11);
		tempvmask.data[6].key = bitsbuffer[i].range(12, 12);
		tempvmask.data[6].value = bitsbuffer[i].range(13, 13);
		tempvmask.data[7].key = bitsbuffer[i].range(14, 14);
		tempvmask.data[7].value = bitsbuffer[i].range(15, 15);
		tempvmask.data[8].key = bitsbuffer[i].range(16, 16);
		tempvmask.data[8].value = bitsbuffer[i].range(17, 17);
		tempvmask.data[9].key = bitsbuffer[i].range(18, 18);
		tempvmask.data[9].value = bitsbuffer[i].range(19, 19);
		tempvmask.data[10].key = bitsbuffer[i].range(20, 20);
		tempvmask.data[10].value = bitsbuffer[i].range(21, 21);
		tempvmask.data[11].key = bitsbuffer[i].range(22, 22);
		tempvmask.data[11].value = bitsbuffer[i].range(23, 23);
		tempvmask.data[12].key = bitsbuffer[i].range(24, 24);
		tempvmask.data[12].value = bitsbuffer[i].range(25, 25);
		tempvmask.data[13].key = bitsbuffer[i].range(26, 26);
		tempvmask.data[13].value = bitsbuffer[i].range(27, 27);
		tempvmask.data[14].key = bitsbuffer[i].range(28, 28);
		tempvmask.data[14].value = bitsbuffer[i].range(29, 29);
		tempvmask.data[15].key = bitsbuffer[i].range(30, 30);
		tempvmask.data[15].value = bitsbuffer[i].range(31, 31);
		#else 
		tempvmask.data[0].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		tempvmask.data[0].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		tempvmask.data[1].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		tempvmask.data[1].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		tempvmask.data[2].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		tempvmask.data[2].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		tempvmask.data[3].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		tempvmask.data[3].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		tempvmask.data[4].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		tempvmask.data[4].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		tempvmask.data[5].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		tempvmask.data[5].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		tempvmask.data[6].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		tempvmask.data[6].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		tempvmask.data[7].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		tempvmask.data[7].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		tempvmask.data[8].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		tempvmask.data[8].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		tempvmask.data[9].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		tempvmask.data[9].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		tempvmask.data[10].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		tempvmask.data[10].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		tempvmask.data[11].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		tempvmask.data[11].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		tempvmask.data[12].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		tempvmask.data[12].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		tempvmask.data[13].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		tempvmask.data[13].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		tempvmask.data[14].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		tempvmask.data[14].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		tempvmask.data[15].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		tempvmask.data[15].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		
		// mask 
		vmask0[i] = tempvmask;	
		vmask1[i] = tempvmask;	
		vmask2[i] = tempvmask;	
	}
	return;
}
void
	#ifdef SW 
	mem_access::
	#endif 
MEMACCESS_loadmany4vmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount1 = BLOCKRAM_SIZE;
	analysis_type analysis_loopcount2 = BLOCKRAM_SIZE / 16;
	analysis_type analysis_loopcount3 = BLOCKRAM_SIZE;
	
	buffer_type transfsize = size_kvs * 16;
	uint32_type bitsbuffer[MAXREDUCEBUFFERSZ];

	LOADVMASKS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
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

	unitBRAMwidth_type tempvmask;
	#pragma HLS DATA_PACK variable = tempvmask	
	LOADVMASKS_LOOP2: for (buffer_type i=0; i<transfsize; i++){ // transfsize, reducebuffersz
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount3 avg=analysis_loopcount3
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		tempvmask.data[0].key = bitsbuffer[i].range(0, 0);
		tempvmask.data[0].value = bitsbuffer[i].range(1, 1);
		tempvmask.data[1].key = bitsbuffer[i].range(2, 2);
		tempvmask.data[1].value = bitsbuffer[i].range(3, 3);
		tempvmask.data[2].key = bitsbuffer[i].range(4, 4);
		tempvmask.data[2].value = bitsbuffer[i].range(5, 5);
		tempvmask.data[3].key = bitsbuffer[i].range(6, 6);
		tempvmask.data[3].value = bitsbuffer[i].range(7, 7);
		tempvmask.data[4].key = bitsbuffer[i].range(8, 8);
		tempvmask.data[4].value = bitsbuffer[i].range(9, 9);
		tempvmask.data[5].key = bitsbuffer[i].range(10, 10);
		tempvmask.data[5].value = bitsbuffer[i].range(11, 11);
		tempvmask.data[6].key = bitsbuffer[i].range(12, 12);
		tempvmask.data[6].value = bitsbuffer[i].range(13, 13);
		tempvmask.data[7].key = bitsbuffer[i].range(14, 14);
		tempvmask.data[7].value = bitsbuffer[i].range(15, 15);
		tempvmask.data[8].key = bitsbuffer[i].range(16, 16);
		tempvmask.data[8].value = bitsbuffer[i].range(17, 17);
		tempvmask.data[9].key = bitsbuffer[i].range(18, 18);
		tempvmask.data[9].value = bitsbuffer[i].range(19, 19);
		tempvmask.data[10].key = bitsbuffer[i].range(20, 20);
		tempvmask.data[10].value = bitsbuffer[i].range(21, 21);
		tempvmask.data[11].key = bitsbuffer[i].range(22, 22);
		tempvmask.data[11].value = bitsbuffer[i].range(23, 23);
		tempvmask.data[12].key = bitsbuffer[i].range(24, 24);
		tempvmask.data[12].value = bitsbuffer[i].range(25, 25);
		tempvmask.data[13].key = bitsbuffer[i].range(26, 26);
		tempvmask.data[13].value = bitsbuffer[i].range(27, 27);
		tempvmask.data[14].key = bitsbuffer[i].range(28, 28);
		tempvmask.data[14].value = bitsbuffer[i].range(29, 29);
		tempvmask.data[15].key = bitsbuffer[i].range(30, 30);
		tempvmask.data[15].value = bitsbuffer[i].range(31, 31);
		#else 
		tempvmask.data[0].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		tempvmask.data[0].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		tempvmask.data[1].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		tempvmask.data[1].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		tempvmask.data[2].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		tempvmask.data[2].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		tempvmask.data[3].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		tempvmask.data[3].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		tempvmask.data[4].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		tempvmask.data[4].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		tempvmask.data[5].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		tempvmask.data[5].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		tempvmask.data[6].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		tempvmask.data[6].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		tempvmask.data[7].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		tempvmask.data[7].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		tempvmask.data[8].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		tempvmask.data[8].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		tempvmask.data[9].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		tempvmask.data[9].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		tempvmask.data[10].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		tempvmask.data[10].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		tempvmask.data[11].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		tempvmask.data[11].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		tempvmask.data[12].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		tempvmask.data[12].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		tempvmask.data[13].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		tempvmask.data[13].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		tempvmask.data[14].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		tempvmask.data[14].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		tempvmask.data[15].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		tempvmask.data[15].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		
		// mask 
		vmask0[i] = tempvmask;	
		vmask1[i] = tempvmask;	
		vmask2[i] = tempvmask;	
		vmask3[i] = tempvmask;	
	}
	return;
}
void
	#ifdef SW 
	mem_access::
	#endif 
MEMACCESS_loadmany5vmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount1 = BLOCKRAM_SIZE;
	analysis_type analysis_loopcount2 = BLOCKRAM_SIZE / 16;
	analysis_type analysis_loopcount3 = BLOCKRAM_SIZE;
	
	buffer_type transfsize = size_kvs * 16;
	uint32_type bitsbuffer[MAXREDUCEBUFFERSZ];

	LOADVMASKS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
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

	unitBRAMwidth_type tempvmask;
	#pragma HLS DATA_PACK variable = tempvmask	
	LOADVMASKS_LOOP2: for (buffer_type i=0; i<transfsize; i++){ // transfsize, reducebuffersz
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount3 avg=analysis_loopcount3
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		tempvmask.data[0].key = bitsbuffer[i].range(0, 0);
		tempvmask.data[0].value = bitsbuffer[i].range(1, 1);
		tempvmask.data[1].key = bitsbuffer[i].range(2, 2);
		tempvmask.data[1].value = bitsbuffer[i].range(3, 3);
		tempvmask.data[2].key = bitsbuffer[i].range(4, 4);
		tempvmask.data[2].value = bitsbuffer[i].range(5, 5);
		tempvmask.data[3].key = bitsbuffer[i].range(6, 6);
		tempvmask.data[3].value = bitsbuffer[i].range(7, 7);
		tempvmask.data[4].key = bitsbuffer[i].range(8, 8);
		tempvmask.data[4].value = bitsbuffer[i].range(9, 9);
		tempvmask.data[5].key = bitsbuffer[i].range(10, 10);
		tempvmask.data[5].value = bitsbuffer[i].range(11, 11);
		tempvmask.data[6].key = bitsbuffer[i].range(12, 12);
		tempvmask.data[6].value = bitsbuffer[i].range(13, 13);
		tempvmask.data[7].key = bitsbuffer[i].range(14, 14);
		tempvmask.data[7].value = bitsbuffer[i].range(15, 15);
		tempvmask.data[8].key = bitsbuffer[i].range(16, 16);
		tempvmask.data[8].value = bitsbuffer[i].range(17, 17);
		tempvmask.data[9].key = bitsbuffer[i].range(18, 18);
		tempvmask.data[9].value = bitsbuffer[i].range(19, 19);
		tempvmask.data[10].key = bitsbuffer[i].range(20, 20);
		tempvmask.data[10].value = bitsbuffer[i].range(21, 21);
		tempvmask.data[11].key = bitsbuffer[i].range(22, 22);
		tempvmask.data[11].value = bitsbuffer[i].range(23, 23);
		tempvmask.data[12].key = bitsbuffer[i].range(24, 24);
		tempvmask.data[12].value = bitsbuffer[i].range(25, 25);
		tempvmask.data[13].key = bitsbuffer[i].range(26, 26);
		tempvmask.data[13].value = bitsbuffer[i].range(27, 27);
		tempvmask.data[14].key = bitsbuffer[i].range(28, 28);
		tempvmask.data[14].value = bitsbuffer[i].range(29, 29);
		tempvmask.data[15].key = bitsbuffer[i].range(30, 30);
		tempvmask.data[15].value = bitsbuffer[i].range(31, 31);
		#else 
		tempvmask.data[0].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		tempvmask.data[0].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		tempvmask.data[1].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		tempvmask.data[1].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		tempvmask.data[2].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		tempvmask.data[2].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		tempvmask.data[3].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		tempvmask.data[3].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		tempvmask.data[4].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		tempvmask.data[4].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		tempvmask.data[5].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		tempvmask.data[5].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		tempvmask.data[6].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		tempvmask.data[6].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		tempvmask.data[7].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		tempvmask.data[7].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		tempvmask.data[8].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		tempvmask.data[8].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		tempvmask.data[9].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		tempvmask.data[9].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		tempvmask.data[10].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		tempvmask.data[10].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		tempvmask.data[11].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		tempvmask.data[11].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		tempvmask.data[12].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		tempvmask.data[12].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		tempvmask.data[13].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		tempvmask.data[13].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		tempvmask.data[14].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		tempvmask.data[14].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		tempvmask.data[15].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		tempvmask.data[15].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		
		// mask 
		vmask0[i] = tempvmask;	
		vmask1[i] = tempvmask;	
		vmask2[i] = tempvmask;	
		vmask3[i] = tempvmask;	
		vmask4[i] = tempvmask;	
	}
	return;
}
void
	#ifdef SW 
	mem_access::
	#endif 
MEMACCESS_loadmany6vmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount1 = BLOCKRAM_SIZE;
	analysis_type analysis_loopcount2 = BLOCKRAM_SIZE / 16;
	analysis_type analysis_loopcount3 = BLOCKRAM_SIZE;
	
	buffer_type transfsize = size_kvs * 16;
	uint32_type bitsbuffer[MAXREDUCEBUFFERSZ];

	LOADVMASKS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
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

	unitBRAMwidth_type tempvmask;
	#pragma HLS DATA_PACK variable = tempvmask	
	LOADVMASKS_LOOP2: for (buffer_type i=0; i<transfsize; i++){ // transfsize, reducebuffersz
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount3 avg=analysis_loopcount3
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		tempvmask.data[0].key = bitsbuffer[i].range(0, 0);
		tempvmask.data[0].value = bitsbuffer[i].range(1, 1);
		tempvmask.data[1].key = bitsbuffer[i].range(2, 2);
		tempvmask.data[1].value = bitsbuffer[i].range(3, 3);
		tempvmask.data[2].key = bitsbuffer[i].range(4, 4);
		tempvmask.data[2].value = bitsbuffer[i].range(5, 5);
		tempvmask.data[3].key = bitsbuffer[i].range(6, 6);
		tempvmask.data[3].value = bitsbuffer[i].range(7, 7);
		tempvmask.data[4].key = bitsbuffer[i].range(8, 8);
		tempvmask.data[4].value = bitsbuffer[i].range(9, 9);
		tempvmask.data[5].key = bitsbuffer[i].range(10, 10);
		tempvmask.data[5].value = bitsbuffer[i].range(11, 11);
		tempvmask.data[6].key = bitsbuffer[i].range(12, 12);
		tempvmask.data[6].value = bitsbuffer[i].range(13, 13);
		tempvmask.data[7].key = bitsbuffer[i].range(14, 14);
		tempvmask.data[7].value = bitsbuffer[i].range(15, 15);
		tempvmask.data[8].key = bitsbuffer[i].range(16, 16);
		tempvmask.data[8].value = bitsbuffer[i].range(17, 17);
		tempvmask.data[9].key = bitsbuffer[i].range(18, 18);
		tempvmask.data[9].value = bitsbuffer[i].range(19, 19);
		tempvmask.data[10].key = bitsbuffer[i].range(20, 20);
		tempvmask.data[10].value = bitsbuffer[i].range(21, 21);
		tempvmask.data[11].key = bitsbuffer[i].range(22, 22);
		tempvmask.data[11].value = bitsbuffer[i].range(23, 23);
		tempvmask.data[12].key = bitsbuffer[i].range(24, 24);
		tempvmask.data[12].value = bitsbuffer[i].range(25, 25);
		tempvmask.data[13].key = bitsbuffer[i].range(26, 26);
		tempvmask.data[13].value = bitsbuffer[i].range(27, 27);
		tempvmask.data[14].key = bitsbuffer[i].range(28, 28);
		tempvmask.data[14].value = bitsbuffer[i].range(29, 29);
		tempvmask.data[15].key = bitsbuffer[i].range(30, 30);
		tempvmask.data[15].value = bitsbuffer[i].range(31, 31);
		#else 
		tempvmask.data[0].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		tempvmask.data[0].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		tempvmask.data[1].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		tempvmask.data[1].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		tempvmask.data[2].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		tempvmask.data[2].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		tempvmask.data[3].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		tempvmask.data[3].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		tempvmask.data[4].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		tempvmask.data[4].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		tempvmask.data[5].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		tempvmask.data[5].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		tempvmask.data[6].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		tempvmask.data[6].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		tempvmask.data[7].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		tempvmask.data[7].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		tempvmask.data[8].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		tempvmask.data[8].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		tempvmask.data[9].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		tempvmask.data[9].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		tempvmask.data[10].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		tempvmask.data[10].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		tempvmask.data[11].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		tempvmask.data[11].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		tempvmask.data[12].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		tempvmask.data[12].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		tempvmask.data[13].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		tempvmask.data[13].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		tempvmask.data[14].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		tempvmask.data[14].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		tempvmask.data[15].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		tempvmask.data[15].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		
		// mask 
		vmask0[i] = tempvmask;	
		vmask1[i] = tempvmask;	
		vmask2[i] = tempvmask;	
		vmask3[i] = tempvmask;	
		vmask4[i] = tempvmask;	
		vmask5[i] = tempvmask;	
	}
	return;
}
void
	#ifdef SW 
	mem_access::
	#endif 
MEMACCESS_loadmany7vmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5[BLOCKRAM_SIZE],unitBRAMwidth_type vmask6[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount1 = BLOCKRAM_SIZE;
	analysis_type analysis_loopcount2 = BLOCKRAM_SIZE / 16;
	analysis_type analysis_loopcount3 = BLOCKRAM_SIZE;
	
	buffer_type transfsize = size_kvs * 16;
	uint32_type bitsbuffer[MAXREDUCEBUFFERSZ];

	LOADVMASKS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
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

	unitBRAMwidth_type tempvmask;
	#pragma HLS DATA_PACK variable = tempvmask	
	LOADVMASKS_LOOP2: for (buffer_type i=0; i<transfsize; i++){ // transfsize, reducebuffersz
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount3 avg=analysis_loopcount3
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		tempvmask.data[0].key = bitsbuffer[i].range(0, 0);
		tempvmask.data[0].value = bitsbuffer[i].range(1, 1);
		tempvmask.data[1].key = bitsbuffer[i].range(2, 2);
		tempvmask.data[1].value = bitsbuffer[i].range(3, 3);
		tempvmask.data[2].key = bitsbuffer[i].range(4, 4);
		tempvmask.data[2].value = bitsbuffer[i].range(5, 5);
		tempvmask.data[3].key = bitsbuffer[i].range(6, 6);
		tempvmask.data[3].value = bitsbuffer[i].range(7, 7);
		tempvmask.data[4].key = bitsbuffer[i].range(8, 8);
		tempvmask.data[4].value = bitsbuffer[i].range(9, 9);
		tempvmask.data[5].key = bitsbuffer[i].range(10, 10);
		tempvmask.data[5].value = bitsbuffer[i].range(11, 11);
		tempvmask.data[6].key = bitsbuffer[i].range(12, 12);
		tempvmask.data[6].value = bitsbuffer[i].range(13, 13);
		tempvmask.data[7].key = bitsbuffer[i].range(14, 14);
		tempvmask.data[7].value = bitsbuffer[i].range(15, 15);
		tempvmask.data[8].key = bitsbuffer[i].range(16, 16);
		tempvmask.data[8].value = bitsbuffer[i].range(17, 17);
		tempvmask.data[9].key = bitsbuffer[i].range(18, 18);
		tempvmask.data[9].value = bitsbuffer[i].range(19, 19);
		tempvmask.data[10].key = bitsbuffer[i].range(20, 20);
		tempvmask.data[10].value = bitsbuffer[i].range(21, 21);
		tempvmask.data[11].key = bitsbuffer[i].range(22, 22);
		tempvmask.data[11].value = bitsbuffer[i].range(23, 23);
		tempvmask.data[12].key = bitsbuffer[i].range(24, 24);
		tempvmask.data[12].value = bitsbuffer[i].range(25, 25);
		tempvmask.data[13].key = bitsbuffer[i].range(26, 26);
		tempvmask.data[13].value = bitsbuffer[i].range(27, 27);
		tempvmask.data[14].key = bitsbuffer[i].range(28, 28);
		tempvmask.data[14].value = bitsbuffer[i].range(29, 29);
		tempvmask.data[15].key = bitsbuffer[i].range(30, 30);
		tempvmask.data[15].value = bitsbuffer[i].range(31, 31);
		#else 
		tempvmask.data[0].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		tempvmask.data[0].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		tempvmask.data[1].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		tempvmask.data[1].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		tempvmask.data[2].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		tempvmask.data[2].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		tempvmask.data[3].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		tempvmask.data[3].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		tempvmask.data[4].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		tempvmask.data[4].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		tempvmask.data[5].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		tempvmask.data[5].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		tempvmask.data[6].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		tempvmask.data[6].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		tempvmask.data[7].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		tempvmask.data[7].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		tempvmask.data[8].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		tempvmask.data[8].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		tempvmask.data[9].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		tempvmask.data[9].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		tempvmask.data[10].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		tempvmask.data[10].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		tempvmask.data[11].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		tempvmask.data[11].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		tempvmask.data[12].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		tempvmask.data[12].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		tempvmask.data[13].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		tempvmask.data[13].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		tempvmask.data[14].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		tempvmask.data[14].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		tempvmask.data[15].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		tempvmask.data[15].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		
		// mask 
		vmask0[i] = tempvmask;	
		vmask1[i] = tempvmask;	
		vmask2[i] = tempvmask;	
		vmask3[i] = tempvmask;	
		vmask4[i] = tempvmask;	
		vmask5[i] = tempvmask;	
		vmask6[i] = tempvmask;	
	}
	return;
}
void
	#ifdef SW 
	mem_access::
	#endif 
MEMACCESS_loadmany8vmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5[BLOCKRAM_SIZE],unitBRAMwidth_type vmask6[BLOCKRAM_SIZE],unitBRAMwidth_type vmask7[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount1 = BLOCKRAM_SIZE;
	analysis_type analysis_loopcount2 = BLOCKRAM_SIZE / 16;
	analysis_type analysis_loopcount3 = BLOCKRAM_SIZE;
	
	buffer_type transfsize = size_kvs * 16;
	uint32_type bitsbuffer[MAXREDUCEBUFFERSZ];

	LOADVMASKS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
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

	unitBRAMwidth_type tempvmask;
	#pragma HLS DATA_PACK variable = tempvmask	
	LOADVMASKS_LOOP2: for (buffer_type i=0; i<transfsize; i++){ // transfsize, reducebuffersz
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount3 avg=analysis_loopcount3
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		tempvmask.data[0].key = bitsbuffer[i].range(0, 0);
		tempvmask.data[0].value = bitsbuffer[i].range(1, 1);
		tempvmask.data[1].key = bitsbuffer[i].range(2, 2);
		tempvmask.data[1].value = bitsbuffer[i].range(3, 3);
		tempvmask.data[2].key = bitsbuffer[i].range(4, 4);
		tempvmask.data[2].value = bitsbuffer[i].range(5, 5);
		tempvmask.data[3].key = bitsbuffer[i].range(6, 6);
		tempvmask.data[3].value = bitsbuffer[i].range(7, 7);
		tempvmask.data[4].key = bitsbuffer[i].range(8, 8);
		tempvmask.data[4].value = bitsbuffer[i].range(9, 9);
		tempvmask.data[5].key = bitsbuffer[i].range(10, 10);
		tempvmask.data[5].value = bitsbuffer[i].range(11, 11);
		tempvmask.data[6].key = bitsbuffer[i].range(12, 12);
		tempvmask.data[6].value = bitsbuffer[i].range(13, 13);
		tempvmask.data[7].key = bitsbuffer[i].range(14, 14);
		tempvmask.data[7].value = bitsbuffer[i].range(15, 15);
		tempvmask.data[8].key = bitsbuffer[i].range(16, 16);
		tempvmask.data[8].value = bitsbuffer[i].range(17, 17);
		tempvmask.data[9].key = bitsbuffer[i].range(18, 18);
		tempvmask.data[9].value = bitsbuffer[i].range(19, 19);
		tempvmask.data[10].key = bitsbuffer[i].range(20, 20);
		tempvmask.data[10].value = bitsbuffer[i].range(21, 21);
		tempvmask.data[11].key = bitsbuffer[i].range(22, 22);
		tempvmask.data[11].value = bitsbuffer[i].range(23, 23);
		tempvmask.data[12].key = bitsbuffer[i].range(24, 24);
		tempvmask.data[12].value = bitsbuffer[i].range(25, 25);
		tempvmask.data[13].key = bitsbuffer[i].range(26, 26);
		tempvmask.data[13].value = bitsbuffer[i].range(27, 27);
		tempvmask.data[14].key = bitsbuffer[i].range(28, 28);
		tempvmask.data[14].value = bitsbuffer[i].range(29, 29);
		tempvmask.data[15].key = bitsbuffer[i].range(30, 30);
		tempvmask.data[15].value = bitsbuffer[i].range(31, 31);
		#else 
		tempvmask.data[0].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		tempvmask.data[0].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		tempvmask.data[1].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		tempvmask.data[1].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		tempvmask.data[2].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		tempvmask.data[2].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		tempvmask.data[3].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		tempvmask.data[3].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		tempvmask.data[4].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		tempvmask.data[4].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		tempvmask.data[5].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		tempvmask.data[5].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		tempvmask.data[6].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		tempvmask.data[6].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		tempvmask.data[7].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		tempvmask.data[7].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		tempvmask.data[8].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		tempvmask.data[8].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		tempvmask.data[9].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		tempvmask.data[9].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		tempvmask.data[10].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		tempvmask.data[10].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		tempvmask.data[11].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		tempvmask.data[11].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		tempvmask.data[12].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		tempvmask.data[12].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		tempvmask.data[13].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		tempvmask.data[13].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		tempvmask.data[14].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		tempvmask.data[14].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		tempvmask.data[15].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		tempvmask.data[15].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		
		// mask 
		vmask0[i] = tempvmask;	
		vmask1[i] = tempvmask;	
		vmask2[i] = tempvmask;	
		vmask3[i] = tempvmask;	
		vmask4[i] = tempvmask;	
		vmask5[i] = tempvmask;	
		vmask6[i] = tempvmask;	
		vmask7[i] = tempvmask;	
	}
	return;
}
void
	#ifdef SW 
	mem_access::
	#endif 
MEMACCESS_loadmany9vmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5[BLOCKRAM_SIZE],unitBRAMwidth_type vmask6[BLOCKRAM_SIZE],unitBRAMwidth_type vmask7[BLOCKRAM_SIZE],unitBRAMwidth_type vmask8[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount1 = BLOCKRAM_SIZE;
	analysis_type analysis_loopcount2 = BLOCKRAM_SIZE / 16;
	analysis_type analysis_loopcount3 = BLOCKRAM_SIZE;
	
	buffer_type transfsize = size_kvs * 16;
	uint32_type bitsbuffer[MAXREDUCEBUFFERSZ];

	LOADVMASKS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
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

	unitBRAMwidth_type tempvmask;
	#pragma HLS DATA_PACK variable = tempvmask	
	LOADVMASKS_LOOP2: for (buffer_type i=0; i<transfsize; i++){ // transfsize, reducebuffersz
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount3 avg=analysis_loopcount3
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		tempvmask.data[0].key = bitsbuffer[i].range(0, 0);
		tempvmask.data[0].value = bitsbuffer[i].range(1, 1);
		tempvmask.data[1].key = bitsbuffer[i].range(2, 2);
		tempvmask.data[1].value = bitsbuffer[i].range(3, 3);
		tempvmask.data[2].key = bitsbuffer[i].range(4, 4);
		tempvmask.data[2].value = bitsbuffer[i].range(5, 5);
		tempvmask.data[3].key = bitsbuffer[i].range(6, 6);
		tempvmask.data[3].value = bitsbuffer[i].range(7, 7);
		tempvmask.data[4].key = bitsbuffer[i].range(8, 8);
		tempvmask.data[4].value = bitsbuffer[i].range(9, 9);
		tempvmask.data[5].key = bitsbuffer[i].range(10, 10);
		tempvmask.data[5].value = bitsbuffer[i].range(11, 11);
		tempvmask.data[6].key = bitsbuffer[i].range(12, 12);
		tempvmask.data[6].value = bitsbuffer[i].range(13, 13);
		tempvmask.data[7].key = bitsbuffer[i].range(14, 14);
		tempvmask.data[7].value = bitsbuffer[i].range(15, 15);
		tempvmask.data[8].key = bitsbuffer[i].range(16, 16);
		tempvmask.data[8].value = bitsbuffer[i].range(17, 17);
		tempvmask.data[9].key = bitsbuffer[i].range(18, 18);
		tempvmask.data[9].value = bitsbuffer[i].range(19, 19);
		tempvmask.data[10].key = bitsbuffer[i].range(20, 20);
		tempvmask.data[10].value = bitsbuffer[i].range(21, 21);
		tempvmask.data[11].key = bitsbuffer[i].range(22, 22);
		tempvmask.data[11].value = bitsbuffer[i].range(23, 23);
		tempvmask.data[12].key = bitsbuffer[i].range(24, 24);
		tempvmask.data[12].value = bitsbuffer[i].range(25, 25);
		tempvmask.data[13].key = bitsbuffer[i].range(26, 26);
		tempvmask.data[13].value = bitsbuffer[i].range(27, 27);
		tempvmask.data[14].key = bitsbuffer[i].range(28, 28);
		tempvmask.data[14].value = bitsbuffer[i].range(29, 29);
		tempvmask.data[15].key = bitsbuffer[i].range(30, 30);
		tempvmask.data[15].value = bitsbuffer[i].range(31, 31);
		#else 
		tempvmask.data[0].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		tempvmask.data[0].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		tempvmask.data[1].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		tempvmask.data[1].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		tempvmask.data[2].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		tempvmask.data[2].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		tempvmask.data[3].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		tempvmask.data[3].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		tempvmask.data[4].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		tempvmask.data[4].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		tempvmask.data[5].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		tempvmask.data[5].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		tempvmask.data[6].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		tempvmask.data[6].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		tempvmask.data[7].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		tempvmask.data[7].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		tempvmask.data[8].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		tempvmask.data[8].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		tempvmask.data[9].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		tempvmask.data[9].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		tempvmask.data[10].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		tempvmask.data[10].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		tempvmask.data[11].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		tempvmask.data[11].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		tempvmask.data[12].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		tempvmask.data[12].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		tempvmask.data[13].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		tempvmask.data[13].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		tempvmask.data[14].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		tempvmask.data[14].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		tempvmask.data[15].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		tempvmask.data[15].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		
		// mask 
		vmask0[i] = tempvmask;	
		vmask1[i] = tempvmask;	
		vmask2[i] = tempvmask;	
		vmask3[i] = tempvmask;	
		vmask4[i] = tempvmask;	
		vmask5[i] = tempvmask;	
		vmask6[i] = tempvmask;	
		vmask7[i] = tempvmask;	
		vmask8[i] = tempvmask;	
	}
	return;
}
void
	#ifdef SW 
	mem_access::
	#endif 
MEMACCESS_loadmany10vmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5[BLOCKRAM_SIZE],unitBRAMwidth_type vmask6[BLOCKRAM_SIZE],unitBRAMwidth_type vmask7[BLOCKRAM_SIZE],unitBRAMwidth_type vmask8[BLOCKRAM_SIZE],unitBRAMwidth_type vmask9[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount1 = BLOCKRAM_SIZE;
	analysis_type analysis_loopcount2 = BLOCKRAM_SIZE / 16;
	analysis_type analysis_loopcount3 = BLOCKRAM_SIZE;
	
	buffer_type transfsize = size_kvs * 16;
	uint32_type bitsbuffer[MAXREDUCEBUFFERSZ];

	LOADVMASKS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
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

	unitBRAMwidth_type tempvmask;
	#pragma HLS DATA_PACK variable = tempvmask	
	LOADVMASKS_LOOP2: for (buffer_type i=0; i<transfsize; i++){ // transfsize, reducebuffersz
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount3 avg=analysis_loopcount3
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		tempvmask.data[0].key = bitsbuffer[i].range(0, 0);
		tempvmask.data[0].value = bitsbuffer[i].range(1, 1);
		tempvmask.data[1].key = bitsbuffer[i].range(2, 2);
		tempvmask.data[1].value = bitsbuffer[i].range(3, 3);
		tempvmask.data[2].key = bitsbuffer[i].range(4, 4);
		tempvmask.data[2].value = bitsbuffer[i].range(5, 5);
		tempvmask.data[3].key = bitsbuffer[i].range(6, 6);
		tempvmask.data[3].value = bitsbuffer[i].range(7, 7);
		tempvmask.data[4].key = bitsbuffer[i].range(8, 8);
		tempvmask.data[4].value = bitsbuffer[i].range(9, 9);
		tempvmask.data[5].key = bitsbuffer[i].range(10, 10);
		tempvmask.data[5].value = bitsbuffer[i].range(11, 11);
		tempvmask.data[6].key = bitsbuffer[i].range(12, 12);
		tempvmask.data[6].value = bitsbuffer[i].range(13, 13);
		tempvmask.data[7].key = bitsbuffer[i].range(14, 14);
		tempvmask.data[7].value = bitsbuffer[i].range(15, 15);
		tempvmask.data[8].key = bitsbuffer[i].range(16, 16);
		tempvmask.data[8].value = bitsbuffer[i].range(17, 17);
		tempvmask.data[9].key = bitsbuffer[i].range(18, 18);
		tempvmask.data[9].value = bitsbuffer[i].range(19, 19);
		tempvmask.data[10].key = bitsbuffer[i].range(20, 20);
		tempvmask.data[10].value = bitsbuffer[i].range(21, 21);
		tempvmask.data[11].key = bitsbuffer[i].range(22, 22);
		tempvmask.data[11].value = bitsbuffer[i].range(23, 23);
		tempvmask.data[12].key = bitsbuffer[i].range(24, 24);
		tempvmask.data[12].value = bitsbuffer[i].range(25, 25);
		tempvmask.data[13].key = bitsbuffer[i].range(26, 26);
		tempvmask.data[13].value = bitsbuffer[i].range(27, 27);
		tempvmask.data[14].key = bitsbuffer[i].range(28, 28);
		tempvmask.data[14].value = bitsbuffer[i].range(29, 29);
		tempvmask.data[15].key = bitsbuffer[i].range(30, 30);
		tempvmask.data[15].value = bitsbuffer[i].range(31, 31);
		#else 
		tempvmask.data[0].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		tempvmask.data[0].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		tempvmask.data[1].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		tempvmask.data[1].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		tempvmask.data[2].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		tempvmask.data[2].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		tempvmask.data[3].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		tempvmask.data[3].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		tempvmask.data[4].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		tempvmask.data[4].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		tempvmask.data[5].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		tempvmask.data[5].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		tempvmask.data[6].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		tempvmask.data[6].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		tempvmask.data[7].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		tempvmask.data[7].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		tempvmask.data[8].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		tempvmask.data[8].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		tempvmask.data[9].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		tempvmask.data[9].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		tempvmask.data[10].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		tempvmask.data[10].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		tempvmask.data[11].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		tempvmask.data[11].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		tempvmask.data[12].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		tempvmask.data[12].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		tempvmask.data[13].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		tempvmask.data[13].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		tempvmask.data[14].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		tempvmask.data[14].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		tempvmask.data[15].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		tempvmask.data[15].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		
		// mask 
		vmask0[i] = tempvmask;	
		vmask1[i] = tempvmask;	
		vmask2[i] = tempvmask;	
		vmask3[i] = tempvmask;	
		vmask4[i] = tempvmask;	
		vmask5[i] = tempvmask;	
		vmask6[i] = tempvmask;	
		vmask7[i] = tempvmask;	
		vmask8[i] = tempvmask;	
		vmask9[i] = tempvmask;	
	}
	return;
}
void
	#ifdef SW 
	mem_access::
	#endif 
MEMACCESS_loadmany11vmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5[BLOCKRAM_SIZE],unitBRAMwidth_type vmask6[BLOCKRAM_SIZE],unitBRAMwidth_type vmask7[BLOCKRAM_SIZE],unitBRAMwidth_type vmask8[BLOCKRAM_SIZE],unitBRAMwidth_type vmask9[BLOCKRAM_SIZE],unitBRAMwidth_type vmask10[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount1 = BLOCKRAM_SIZE;
	analysis_type analysis_loopcount2 = BLOCKRAM_SIZE / 16;
	analysis_type analysis_loopcount3 = BLOCKRAM_SIZE;
	
	buffer_type transfsize = size_kvs * 16;
	uint32_type bitsbuffer[MAXREDUCEBUFFERSZ];

	LOADVMASKS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
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

	unitBRAMwidth_type tempvmask;
	#pragma HLS DATA_PACK variable = tempvmask	
	LOADVMASKS_LOOP2: for (buffer_type i=0; i<transfsize; i++){ // transfsize, reducebuffersz
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount3 avg=analysis_loopcount3
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		tempvmask.data[0].key = bitsbuffer[i].range(0, 0);
		tempvmask.data[0].value = bitsbuffer[i].range(1, 1);
		tempvmask.data[1].key = bitsbuffer[i].range(2, 2);
		tempvmask.data[1].value = bitsbuffer[i].range(3, 3);
		tempvmask.data[2].key = bitsbuffer[i].range(4, 4);
		tempvmask.data[2].value = bitsbuffer[i].range(5, 5);
		tempvmask.data[3].key = bitsbuffer[i].range(6, 6);
		tempvmask.data[3].value = bitsbuffer[i].range(7, 7);
		tempvmask.data[4].key = bitsbuffer[i].range(8, 8);
		tempvmask.data[4].value = bitsbuffer[i].range(9, 9);
		tempvmask.data[5].key = bitsbuffer[i].range(10, 10);
		tempvmask.data[5].value = bitsbuffer[i].range(11, 11);
		tempvmask.data[6].key = bitsbuffer[i].range(12, 12);
		tempvmask.data[6].value = bitsbuffer[i].range(13, 13);
		tempvmask.data[7].key = bitsbuffer[i].range(14, 14);
		tempvmask.data[7].value = bitsbuffer[i].range(15, 15);
		tempvmask.data[8].key = bitsbuffer[i].range(16, 16);
		tempvmask.data[8].value = bitsbuffer[i].range(17, 17);
		tempvmask.data[9].key = bitsbuffer[i].range(18, 18);
		tempvmask.data[9].value = bitsbuffer[i].range(19, 19);
		tempvmask.data[10].key = bitsbuffer[i].range(20, 20);
		tempvmask.data[10].value = bitsbuffer[i].range(21, 21);
		tempvmask.data[11].key = bitsbuffer[i].range(22, 22);
		tempvmask.data[11].value = bitsbuffer[i].range(23, 23);
		tempvmask.data[12].key = bitsbuffer[i].range(24, 24);
		tempvmask.data[12].value = bitsbuffer[i].range(25, 25);
		tempvmask.data[13].key = bitsbuffer[i].range(26, 26);
		tempvmask.data[13].value = bitsbuffer[i].range(27, 27);
		tempvmask.data[14].key = bitsbuffer[i].range(28, 28);
		tempvmask.data[14].value = bitsbuffer[i].range(29, 29);
		tempvmask.data[15].key = bitsbuffer[i].range(30, 30);
		tempvmask.data[15].value = bitsbuffer[i].range(31, 31);
		#else 
		tempvmask.data[0].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		tempvmask.data[0].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		tempvmask.data[1].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		tempvmask.data[1].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		tempvmask.data[2].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		tempvmask.data[2].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		tempvmask.data[3].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		tempvmask.data[3].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		tempvmask.data[4].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		tempvmask.data[4].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		tempvmask.data[5].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		tempvmask.data[5].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		tempvmask.data[6].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		tempvmask.data[6].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		tempvmask.data[7].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		tempvmask.data[7].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		tempvmask.data[8].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		tempvmask.data[8].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		tempvmask.data[9].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		tempvmask.data[9].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		tempvmask.data[10].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		tempvmask.data[10].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		tempvmask.data[11].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		tempvmask.data[11].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		tempvmask.data[12].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		tempvmask.data[12].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		tempvmask.data[13].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		tempvmask.data[13].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		tempvmask.data[14].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		tempvmask.data[14].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		tempvmask.data[15].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		tempvmask.data[15].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		
		// mask 
		vmask0[i] = tempvmask;	
		vmask1[i] = tempvmask;	
		vmask2[i] = tempvmask;	
		vmask3[i] = tempvmask;	
		vmask4[i] = tempvmask;	
		vmask5[i] = tempvmask;	
		vmask6[i] = tempvmask;	
		vmask7[i] = tempvmask;	
		vmask8[i] = tempvmask;	
		vmask9[i] = tempvmask;	
		vmask10[i] = tempvmask;	
	}
	return;
}
void
	#ifdef SW 
	mem_access::
	#endif 
MEMACCESS_loadmany12vmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5[BLOCKRAM_SIZE],unitBRAMwidth_type vmask6[BLOCKRAM_SIZE],unitBRAMwidth_type vmask7[BLOCKRAM_SIZE],unitBRAMwidth_type vmask8[BLOCKRAM_SIZE],unitBRAMwidth_type vmask9[BLOCKRAM_SIZE],unitBRAMwidth_type vmask10[BLOCKRAM_SIZE],unitBRAMwidth_type vmask11[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount1 = BLOCKRAM_SIZE;
	analysis_type analysis_loopcount2 = BLOCKRAM_SIZE / 16;
	analysis_type analysis_loopcount3 = BLOCKRAM_SIZE;
	
	buffer_type transfsize = size_kvs * 16;
	uint32_type bitsbuffer[MAXREDUCEBUFFERSZ];

	LOADVMASKS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
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

	unitBRAMwidth_type tempvmask;
	#pragma HLS DATA_PACK variable = tempvmask	
	LOADVMASKS_LOOP2: for (buffer_type i=0; i<transfsize; i++){ // transfsize, reducebuffersz
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount3 avg=analysis_loopcount3
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		tempvmask.data[0].key = bitsbuffer[i].range(0, 0);
		tempvmask.data[0].value = bitsbuffer[i].range(1, 1);
		tempvmask.data[1].key = bitsbuffer[i].range(2, 2);
		tempvmask.data[1].value = bitsbuffer[i].range(3, 3);
		tempvmask.data[2].key = bitsbuffer[i].range(4, 4);
		tempvmask.data[2].value = bitsbuffer[i].range(5, 5);
		tempvmask.data[3].key = bitsbuffer[i].range(6, 6);
		tempvmask.data[3].value = bitsbuffer[i].range(7, 7);
		tempvmask.data[4].key = bitsbuffer[i].range(8, 8);
		tempvmask.data[4].value = bitsbuffer[i].range(9, 9);
		tempvmask.data[5].key = bitsbuffer[i].range(10, 10);
		tempvmask.data[5].value = bitsbuffer[i].range(11, 11);
		tempvmask.data[6].key = bitsbuffer[i].range(12, 12);
		tempvmask.data[6].value = bitsbuffer[i].range(13, 13);
		tempvmask.data[7].key = bitsbuffer[i].range(14, 14);
		tempvmask.data[7].value = bitsbuffer[i].range(15, 15);
		tempvmask.data[8].key = bitsbuffer[i].range(16, 16);
		tempvmask.data[8].value = bitsbuffer[i].range(17, 17);
		tempvmask.data[9].key = bitsbuffer[i].range(18, 18);
		tempvmask.data[9].value = bitsbuffer[i].range(19, 19);
		tempvmask.data[10].key = bitsbuffer[i].range(20, 20);
		tempvmask.data[10].value = bitsbuffer[i].range(21, 21);
		tempvmask.data[11].key = bitsbuffer[i].range(22, 22);
		tempvmask.data[11].value = bitsbuffer[i].range(23, 23);
		tempvmask.data[12].key = bitsbuffer[i].range(24, 24);
		tempvmask.data[12].value = bitsbuffer[i].range(25, 25);
		tempvmask.data[13].key = bitsbuffer[i].range(26, 26);
		tempvmask.data[13].value = bitsbuffer[i].range(27, 27);
		tempvmask.data[14].key = bitsbuffer[i].range(28, 28);
		tempvmask.data[14].value = bitsbuffer[i].range(29, 29);
		tempvmask.data[15].key = bitsbuffer[i].range(30, 30);
		tempvmask.data[15].value = bitsbuffer[i].range(31, 31);
		#else 
		tempvmask.data[0].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 0, 1);
		tempvmask.data[0].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 1, 1);
		tempvmask.data[1].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 2, 1);
		tempvmask.data[1].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 3, 1);
		tempvmask.data[2].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 4, 1);
		tempvmask.data[2].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 5, 1);
		tempvmask.data[3].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 6, 1);
		tempvmask.data[3].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 7, 1);
		tempvmask.data[4].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 8, 1);
		tempvmask.data[4].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 9, 1);
		tempvmask.data[5].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 10, 1);
		tempvmask.data[5].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 11, 1);
		tempvmask.data[6].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 12, 1);
		tempvmask.data[6].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 13, 1);
		tempvmask.data[7].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 14, 1);
		tempvmask.data[7].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 15, 1);
		tempvmask.data[8].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 16, 1);
		tempvmask.data[8].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 17, 1);
		tempvmask.data[9].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 18, 1);
		tempvmask.data[9].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 19, 1);
		tempvmask.data[10].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 20, 1);
		tempvmask.data[10].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 21, 1);
		tempvmask.data[11].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 22, 1);
		tempvmask.data[11].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 23, 1);
		tempvmask.data[12].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 24, 1);
		tempvmask.data[12].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 25, 1);
		tempvmask.data[13].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 26, 1);
		tempvmask.data[13].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 27, 1);
		tempvmask.data[14].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 28, 1);
		tempvmask.data[14].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 29, 1);
		tempvmask.data[15].key = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 30, 1);
		tempvmask.data[15].value = acts_utilobj->UTIL_READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
		
		// mask 
		vmask0[i] = tempvmask;	
		vmask1[i] = tempvmask;	
		vmask2[i] = tempvmask;	
		vmask3[i] = tempvmask;	
		vmask4[i] = tempvmask;	
		vmask5[i] = tempvmask;	
		vmask6[i] = tempvmask;	
		vmask7[i] = tempvmask;	
		vmask8[i] = tempvmask;	
		vmask9[i] = tempvmask;	
		vmask10[i] = tempvmask;	
		vmask11[i] = tempvmask;	
	}
	return;
}

void
	#ifdef SW 
	mem_access::
	#endif 
MEMACCESS_loadmany1vmasks_subp(bool_type enable, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], unitBRAMwidth_type vmask0_subp[BLOCKRAM_SIZE], buffer_type size_kvs){
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
		
		/* if(i % SUBPMASKFACTOR == SUBPMASKFACTOR-1){ // CRITICAL AUTOMATEME.
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask0_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask0_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
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
		} */
		#endif 
	}
	return;
}
void
	#ifdef SW 
	mem_access::
	#endif 
MEMACCESS_loadmany2vmasks_subp(bool_type enable, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], unitBRAMwidth_type vmask0_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1_subp[BLOCKRAM_SIZE], buffer_type size_kvs){
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
		
		/* if(i % SUBPMASKFACTOR == SUBPMASKFACTOR-1){ // CRITICAL AUTOMATEME.
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask0_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask0_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask1_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask1_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
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
		} */
		#endif 
	}
	return;
}
void
	#ifdef SW 
	mem_access::
	#endif 
MEMACCESS_loadmany3vmasks_subp(bool_type enable, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], unitBRAMwidth_type vmask0_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2_subp[BLOCKRAM_SIZE], buffer_type size_kvs){
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
		
		/* if(i % SUBPMASKFACTOR == SUBPMASKFACTOR-1){ // CRITICAL AUTOMATEME.
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask0_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask0_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask1_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask1_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask2_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask2_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
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
		} */
		#endif 
	}
	return;
}
void
	#ifdef SW 
	mem_access::
	#endif 
MEMACCESS_loadmany4vmasks_subp(bool_type enable, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], unitBRAMwidth_type vmask0_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3_subp[BLOCKRAM_SIZE], buffer_type size_kvs){
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
		
		/* if(i % SUBPMASKFACTOR == SUBPMASKFACTOR-1){ // CRITICAL AUTOMATEME.
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask0_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask0_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask1_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask1_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask2_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask2_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask3_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask3_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
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
		} */
		#endif 
	}
	return;
}
void
	#ifdef SW 
	mem_access::
	#endif 
MEMACCESS_loadmany5vmasks_subp(bool_type enable, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], unitBRAMwidth_type vmask0_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4_subp[BLOCKRAM_SIZE], buffer_type size_kvs){
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
		
		/* if(i % SUBPMASKFACTOR == SUBPMASKFACTOR-1){ // CRITICAL AUTOMATEME.
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask0_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask0_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask1_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask1_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask2_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask2_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask3_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask3_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask4_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask4_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
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
		} */
		#endif 
	}
	return;
}
void
	#ifdef SW 
	mem_access::
	#endif 
MEMACCESS_loadmany6vmasks_subp(bool_type enable, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], unitBRAMwidth_type vmask0_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5_subp[BLOCKRAM_SIZE], buffer_type size_kvs){
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
		
		/* if(i % SUBPMASKFACTOR == SUBPMASKFACTOR-1){ // CRITICAL AUTOMATEME.
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask0_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask0_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask1_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask1_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask2_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask2_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask3_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask3_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask4_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask4_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask5_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask5_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
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
		} */
		#endif 
	}
	return;
}
void
	#ifdef SW 
	mem_access::
	#endif 
MEMACCESS_loadmany7vmasks_subp(bool_type enable, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], unitBRAMwidth_type vmask0_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask6_subp[BLOCKRAM_SIZE], buffer_type size_kvs){
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
		
		/* if(i % SUBPMASKFACTOR == SUBPMASKFACTOR-1){ // CRITICAL AUTOMATEME.
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask0_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask0_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask1_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask1_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask2_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask2_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask3_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask3_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask4_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask4_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask5_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask5_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask6_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask6_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
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
		} */
		#endif 
	}
	return;
}
void
	#ifdef SW 
	mem_access::
	#endif 
MEMACCESS_loadmany8vmasks_subp(bool_type enable, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], unitBRAMwidth_type vmask0_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask6_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask7_subp[BLOCKRAM_SIZE], buffer_type size_kvs){
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
		
		/* if(i % SUBPMASKFACTOR == SUBPMASKFACTOR-1){ // CRITICAL AUTOMATEME.
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask0_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask0_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask1_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask1_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask2_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask2_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask3_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask3_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask4_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask4_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask5_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask5_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask6_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask6_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask7_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask7_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
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
		} */
		#endif 
	}
	return;
}
void
	#ifdef SW 
	mem_access::
	#endif 
MEMACCESS_loadmany9vmasks_subp(bool_type enable, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], unitBRAMwidth_type vmask0_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask6_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask7_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask8_subp[BLOCKRAM_SIZE], buffer_type size_kvs){
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
		
		/* if(i % SUBPMASKFACTOR == SUBPMASKFACTOR-1){ // CRITICAL AUTOMATEME.
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask0_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask0_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask1_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask1_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask2_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask2_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask3_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask3_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask4_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask4_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask5_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask5_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask6_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask6_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask7_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask7_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask8_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask8_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
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
		} */
		#endif 
	}
	return;
}
void
	#ifdef SW 
	mem_access::
	#endif 
MEMACCESS_loadmany10vmasks_subp(bool_type enable, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], unitBRAMwidth_type vmask0_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask6_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask7_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask8_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask9_subp[BLOCKRAM_SIZE], buffer_type size_kvs){
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
		
		/* if(i % SUBPMASKFACTOR == SUBPMASKFACTOR-1){ // CRITICAL AUTOMATEME.
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask0_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask0_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask1_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask1_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask2_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask2_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask3_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask3_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask4_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask4_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask5_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask5_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask6_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask6_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask7_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask7_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask8_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask8_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask9_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask9_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
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
		} */
		#endif 
	}
	return;
}
void
	#ifdef SW 
	mem_access::
	#endif 
MEMACCESS_loadmany11vmasks_subp(bool_type enable, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], unitBRAMwidth_type vmask0_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask6_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask7_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask8_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask9_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask10_subp[BLOCKRAM_SIZE], buffer_type size_kvs){
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
		
		/* if(i % SUBPMASKFACTOR == SUBPMASKFACTOR-1){ // CRITICAL AUTOMATEME.
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask0_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask0_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask1_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask1_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask2_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask2_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask3_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask3_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask4_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask4_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask5_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask5_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask6_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask6_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask7_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask7_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask8_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask8_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask9_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask9_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask10_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask10_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
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
		} */
		#endif 
	}
	return;
}
void
	#ifdef SW 
	mem_access::
	#endif 
MEMACCESS_loadmany12vmasks_subp(bool_type enable, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], unitBRAMwidth_type vmask0_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask6_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask7_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask8_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask9_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask10_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask11_subp[BLOCKRAM_SIZE], buffer_type size_kvs){
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
		
		/* if(i % SUBPMASKFACTOR == SUBPMASKFACTOR-1){ // CRITICAL AUTOMATEME.
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask0_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask0_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask0_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask1_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask1_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask1_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask2_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask2_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask2_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask3_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask3_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask3_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask4_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask4_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask4_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask5_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask5_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask5_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask6_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask6_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask6_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask7_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask7_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask7_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask8_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask8_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask8_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask9_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask9_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask9_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask10_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask10_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask10_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			}
			if(i%16==7){  // CRITICAL AUTOMATEME.
				vmask11_subp[i/SUBPMASKFACTOR].data[0].key = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask11_subp[i/SUBPMASKFACTOR].data[1].key = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask11_subp[i/SUBPMASKFACTOR].data[2].key = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask11_subp[i/SUBPMASKFACTOR].data[3].key = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask11_subp[i/SUBPMASKFACTOR].data[4].key = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask11_subp[i/SUBPMASKFACTOR].data[5].key = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask11_subp[i/SUBPMASKFACTOR].data[6].key = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask11_subp[i/SUBPMASKFACTOR].data[7].key = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask11_subp[i/SUBPMASKFACTOR].data[8].key = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask11_subp[i/SUBPMASKFACTOR].data[9].key = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask11_subp[i/SUBPMASKFACTOR].data[10].key = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask11_subp[i/SUBPMASKFACTOR].data[11].key = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask11_subp[i/SUBPMASKFACTOR].data[12].key = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask11_subp[i/SUBPMASKFACTOR].data[13].key = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask11_subp[i/SUBPMASKFACTOR].data[14].key = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask11_subp[i/SUBPMASKFACTOR].data[15].key = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
			} else {
				vmask11_subp[i/SUBPMASKFACTOR].data[0].value = tempvSUBmask.data[0].key | tempvSUBmask.data[0].value; 
				vmask11_subp[i/SUBPMASKFACTOR].data[1].value = tempvSUBmask.data[1].key | tempvSUBmask.data[1].value; 
				vmask11_subp[i/SUBPMASKFACTOR].data[2].value = tempvSUBmask.data[2].key | tempvSUBmask.data[2].value; 
				vmask11_subp[i/SUBPMASKFACTOR].data[3].value = tempvSUBmask.data[3].key | tempvSUBmask.data[3].value; 
				vmask11_subp[i/SUBPMASKFACTOR].data[4].value = tempvSUBmask.data[4].key | tempvSUBmask.data[4].value; 
				vmask11_subp[i/SUBPMASKFACTOR].data[5].value = tempvSUBmask.data[5].key | tempvSUBmask.data[5].value; 
				vmask11_subp[i/SUBPMASKFACTOR].data[6].value = tempvSUBmask.data[6].key | tempvSUBmask.data[6].value; 
				vmask11_subp[i/SUBPMASKFACTOR].data[7].value = tempvSUBmask.data[7].key | tempvSUBmask.data[7].value; 
				vmask11_subp[i/SUBPMASKFACTOR].data[8].value = tempvSUBmask.data[8].key | tempvSUBmask.data[8].value; 
				vmask11_subp[i/SUBPMASKFACTOR].data[9].value = tempvSUBmask.data[9].key | tempvSUBmask.data[9].value; 
				vmask11_subp[i/SUBPMASKFACTOR].data[10].value = tempvSUBmask.data[10].key | tempvSUBmask.data[10].value; 
				vmask11_subp[i/SUBPMASKFACTOR].data[11].value = tempvSUBmask.data[11].key | tempvSUBmask.data[11].value; 
				vmask11_subp[i/SUBPMASKFACTOR].data[12].value = tempvSUBmask.data[12].key | tempvSUBmask.data[12].value; 
				vmask11_subp[i/SUBPMASKFACTOR].data[13].value = tempvSUBmask.data[13].key | tempvSUBmask.data[13].value; 
				vmask11_subp[i/SUBPMASKFACTOR].data[14].value = tempvSUBmask.data[14].key | tempvSUBmask.data[14].value; 
				vmask11_subp[i/SUBPMASKFACTOR].data[15].value = tempvSUBmask.data[15].key | tempvSUBmask.data[15].value; 
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
		} */
		#endif 
	}
	return;
}

void
	#ifdef SW 
	mem_access::
	#endif 
MEMACCESS_loadvmask_p(uint512_dt * kvdram, uint32_type vmask_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs){
	LOADACTIVEPARTITIONS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
		#ifdef _WIDEWORD
		vmask_p[i] = kvdram[offset_kvs + i].range(31, 0);
		#else
		vmask_p[i] = kvdram[offset_kvs + i].data[0].key;
		#endif 
	}
	return;
}

void
	#ifdef SW 
	mem_access::
	#endif 
MEMACCESS_loadmany1vmask_p(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs){
	LOADACTIVEPARTITIONS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
		#ifdef _WIDEWORD
		vmask0_p[i] = kvdram[offset_kvs + i].range(31, 0);
		#else
		vmask0_p[i] = kvdram[offset_kvs + i].data[0].key;
		#endif 
	}
	return;
}
void
	#ifdef SW 
	mem_access::
	#endif 
MEMACCESS_loadmany2vmask_p(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs){
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
void
	#ifdef SW 
	mem_access::
	#endif 
MEMACCESS_loadmany3vmask_p(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs){
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
void
	#ifdef SW 
	mem_access::
	#endif 
MEMACCESS_loadmany4vmask_p(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs){
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
void
	#ifdef SW 
	mem_access::
	#endif 
MEMACCESS_loadmany5vmask_p(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs){
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
void
	#ifdef SW 
	mem_access::
	#endif 
MEMACCESS_loadmany6vmask_p(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs){
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
void
	#ifdef SW 
	mem_access::
	#endif 
MEMACCESS_loadmany7vmask_p(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs){
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
void
	#ifdef SW 
	mem_access::
	#endif 
MEMACCESS_loadmany8vmask_p(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE],uint32_type vmask7_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs){
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
void
	#ifdef SW 
	mem_access::
	#endif 
MEMACCESS_loadmany9vmask_p(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE],uint32_type vmask7_p[BLOCKRAM_SIZE],uint32_type vmask8_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs){
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
void
	#ifdef SW 
	mem_access::
	#endif 
MEMACCESS_loadmany10vmask_p(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE],uint32_type vmask7_p[BLOCKRAM_SIZE],uint32_type vmask8_p[BLOCKRAM_SIZE],uint32_type vmask9_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs){
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
void
	#ifdef SW 
	mem_access::
	#endif 
MEMACCESS_loadmany11vmask_p(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE],uint32_type vmask7_p[BLOCKRAM_SIZE],uint32_type vmask8_p[BLOCKRAM_SIZE],uint32_type vmask9_p[BLOCKRAM_SIZE],uint32_type vmask10_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs){
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
void
	#ifdef SW 
	mem_access::
	#endif 
MEMACCESS_loadmany12vmask_p(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE],uint32_type vmask7_p[BLOCKRAM_SIZE],uint32_type vmask8_p[BLOCKRAM_SIZE],uint32_type vmask9_p[BLOCKRAM_SIZE],uint32_type vmask10_p[BLOCKRAM_SIZE],uint32_type vmask11_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs){
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

void
	#ifdef SW 
	mem_access::
	#endif 
MEMACCESS_readglobalstats(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], batch_type offset_kvs, globalparams_t globalparams){ 
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

void
	#ifdef SW 
	mem_access::
	#endif 
MEMACCESS_saveglobalstats(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], batch_type offset_kvs, globalparams_t globalparams){ 
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
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printkeyvalues("saveglobalstats.globalstatsbuffer", globalstatsbuffer, NUM_PARTITIONS); 
	#endif
	return;
}

tuple_t
	#ifdef SW 
	mem_access::
	#endif
MEMACCESS_getvptrs( uint512_dt * kvdram, unsigned int beginoffset, unsigned int endoffset, unsigned int edgebankID){
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

unsigned int
	#ifdef SW 
	mem_access::
	#endif
MEMACCESS_getvptr(uint512_dt * kvdram, unsigned int baseoffset_kvs, unsigned int offset){
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

tuple_t
	#ifdef SW 
	mem_access::
	#endif
MEMACCESS_getvptrs_opt( uint512_dt * kvdram, unsigned int baseoffset_kvs, unsigned int beginoffset, unsigned int endoffset, unsigned int edgebankID){
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

void
	#ifdef SW 
	mem_access::
	#endif 
MEMACCESS_copyvs(uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], globalparams_t globalparamsK){
	analysis_type analysis_treedepth = TREE_DEPTH;
	analysis_type analysis_loop1 = 1;
	
	travstate_t rtravstate;
	
	#ifdef ENABLERECURSIVEPARTITIONING
	step_type currentLOP = globalparamsK.ACTSPARAMS_TREEDEPTH;
	batch_type num_source_partitions = acts_utilobj->UTIL_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH);
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

void
	#ifdef SW 
	mem_access::
	#endif 
MEMACCESS_copystats(uint512_dt * edges, uint512_dt * kvdram, globalparams_t globalparamsE, globalparams_t globalparamsK){
	analysis_type analysis_treedepth = TREE_DEPTH;
	analysis_type analysis_loop1 = 1;
	
	COPYSTATS_LOOP1: for(unsigned int k=0; k<globalparamsK.SIZE_KVSTATSDRAM; k++){
	#pragma HLS PIPELINE II=1
		kvdram[globalparamsK.BASEOFFSETKVS_STATSDRAM + k] = edges[globalparamsE.BASEOFFSETKVS_STATSDRAM + k];
	}
	return;
}

void
	#ifdef SW 
	mem_access::
	#endif 
MEMACCESS_copyallstats( uint512_dt * kvdram, globalparams_t globalparamsE, globalparams_t globalparamsK, unsigned int edgebankID){
	analysis_type analysis_treedepth = TREE_DEPTH;
	analysis_type analysis_loop1 = 1;

	return;
}

void
	#ifdef SW 
	mem_access::
	#endif 
MEMACCESS_commitkvstats(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams, unsigned int offset){
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