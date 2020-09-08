#include <string>
#include <string.h>
#include <iostream>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <ctime>
#include <functional>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <iomanip>
#include <cmath>
#include <fstream>
#include "../../include/common.h"
#include "../include/actscommon.h"
#ifndef FPGA_IMPL
#include "../../src/utility/utility.h"
#endif 
#ifndef HW
#include "../../acts/actsutility/actsutility.h"
#endif 
#include "actspartition.h"
using namespace std;

#ifdef SW
actspartition::actspartition(){ actsutilityobj = new actsutility(); }
actspartition::~actspartition(){}
#endif
#ifdef SWEMU
actsutility * actsutilityobj = new actsutility();
#endif

unsigned int 
	#ifdef SW 
	actspartition::
	#endif 
subandcap(unsigned int val1, unsigned int val2){
	if(val1 < val2){ return 0; }
	else { return val1 - val2; }
}
unsigned int 
	#ifdef SW 
	actspartition::
	#endif 
allignhigher_KV(unsigned int val){
	unsigned int fac = (val + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	return (fac * VECTOR_SIZE);
}
batch_type
	#ifdef SW 
	actspartition::
	#endif
getskipsize(unsigned int currentLOP, unsigned int sourceordest){
	unsigned int analysis_treedepth;
	unsigned int result;
	if(sourceordest == SOURCE){ result = NUMLASTLEVELPARTITIONS; }
	else if (sourceordest == DEST){ result = NUMLASTLEVELPARTITIONS / NUM_PARTITIONS; }
	else {}
	for(unsigned int i=0; i<(currentLOP-1); i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_treedepth avg=analysis_treedepth
		result = result / NUM_PARTITIONS;
	}
	return result;
}
void 
	#ifdef SW 
	actspartition::
	#endif 
getglobalparams(uint512_dt * sourcevolume, globalparams_t * globalparams){
	#ifdef _WIDEWORD
	globalparams->runkernelcommand = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNKERNELCOMMANDID].range(31, 0);
	globalparams->processcommand = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_PROCESSCOMMANDID].range(31, 0);
	globalparams->collectstatscommand = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_COLLECTSTATSCOMMANDID].range(31, 0);
	globalparams->partitioncommand = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_PARTITIONCOMMANDID].range(31, 0);
	globalparams->reducecommand = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_APPLYUPDATESCOMMANDID].range(31, 0);
	globalparams->vbegin = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_VOFFSET].range(31, 0); 
	globalparams->vsize = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_VSIZE].range(31, 0); 
	globalparams->treedepth = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_TREEDEPTH].range(31, 0); 
	globalparams->LLOPnumpartitions = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_FINALNUMPARTITIONS].range(31, 0);
	globalparams->GraphIter = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_GRAPHITERATIONID].range(31, 0);
	globalparams->batchsize = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BATCHSIZE].range(31, 0);
	globalparams->runsize = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].range(31, 0);
	globalparams->nextbatchoffset = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NEXTBATCHOFFSET].range(31, 0);
	globalparams->statsalreadycollected = NAp;
	globalparams->baseaddr_destkvs_kvs = 0;
	#else 
	globalparams->runkernelcommand = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNKERNELCOMMANDID].data[0].key;
	globalparams->processcommand = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_PROCESSCOMMANDID].data[0].key;
	globalparams->collectstatscommand = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_COLLECTSTATSCOMMANDID].data[0].key;
	globalparams->partitioncommand = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_PARTITIONCOMMANDID].data[0].key;
	globalparams->reducecommand = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_APPLYUPDATESCOMMANDID].data[0].key;
	globalparams->vbegin = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_VOFFSET].data[0].key; 
	globalparams->vsize = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_VSIZE].data[0].key; 
	globalparams->treedepth = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_TREEDEPTH].data[0].key; 
	globalparams->LLOPnumpartitions = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_FINALNUMPARTITIONS].data[0].key;
	globalparams->GraphIter = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_GRAPHITERATIONID].data[0].key;
	globalparams->batchsize = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BATCHSIZE].data[0].key;
	globalparams->runsize = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key;
	globalparams->nextbatchoffset = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NEXTBATCHOFFSET].data[0].key;
	globalparams->statsalreadycollected = NAp;
	globalparams->baseaddr_destkvs_kvs = 0;
	#endif 
}
void 
	#ifdef SW 
	actspartition::
	#endif 
copykeyandvalues(keyvalue_t * buffer1, keyvalue_t * buffer2, unsigned int size){
	for(partition_type i=0; i<size; i++){ buffer1[i] = buffer2[i]; }
}
void 
	#ifdef SW 
	actspartition::
	#endif 
resetkeyandvalues(keyvalue_t * buffer, unsigned int size){
	for(unsigned int i=0; i<size; i++){ buffer[i].key = 0; buffer[i].value = 0; }
}
void 
	#ifdef SW 
	actspartition::
	#endif 
resetvalues(keyvalue_t * buffer, unsigned int size){
	for(unsigned int i=0; i<size; i++){ buffer[i].value = 0; }
}
void 
	#ifdef SW 
	actspartition::
	#endif 
resetkeyandvalues(unsigned int enable, uint512_dt destbuffer[PADDEDDESTBUFFER_SIZE]){
	if(enable == OFF){ return; }
	uint512_dt ZeroKVS;
	keyvalue_t ZeroKV; ZeroKV.key = 0; ZeroKV.value = 0;
	#ifdef _WIDEWORD
	ZeroKVS = 0;
	#else 
	for(vector_type v=0; v<VECTOR_SIZE; v++){ ZeroKVS.data[v] = ZeroKV; }
	#endif 
	for(partition_type i=0; i<PADDEDDESTBUFFER_SIZE; i++){ 
		destbuffer[i] = ZeroKVS;
	}
	return;
}
void 
	#ifdef SW 
	actspartition::
	#endif 
resetmanykeyandvalues(keyvalue_t buffer[NUM_PARTITIONS], unsigned int size){
	for(unsigned int i=0; i<size; i++){ 
		buffer[i].key = 0; buffer[i].value = 0; 
	}
	return;
}
void 
	#ifdef SW 
	actspartition::
	#endif 
resetmanyvalues(keyvalue_t buffer[NUM_PARTITIONS], unsigned int size){
	for(unsigned int i=0; i<size; i++){ 
		buffer[i].value = 0; 
	}
	return;
}
void 
	#ifdef SW 
	actspartition::
	#endif
accumkeysandvalues(keyvalue_t * buffer1, keyvalue_t * buffer2, unsigned int size){
	for(partition_type i=0; i<size; i++){
	#pragma HLS PIPELINE II=1
		keyvalue_t keyvalue1 = buffer1[i];
		keyvalue_t keyvalue2 = buffer2[i];
		keyvalue1.key += keyvalue2.key;
		keyvalue1.value += keyvalue2.value;
		buffer1[i] = keyvalue1;
	}
	return;
}
unsigned int 
	#ifdef SW 
	actspartition::
	#endif 
checkandforce(unsigned int val, unsigned int limit){ if(val >= limit){ return 0; } else { return val; }}
buffer_type 
	#ifdef SW 
	actspartition::
	#endif 
getchunksize(buffer_type buffer_size, alw_travstate_t travstate, unsigned int localoffset){
	// #pragma HLS INLINE
	buffer_type chunk_size = buffer_size;
	batch_type i = travstate.i_kvs + localoffset;
	if (i > travstate.end_kvs){ chunk_size = 0; }
	else if ((i + buffer_size) > travstate.end_kvs){ chunk_size = travstate.end_kvs - i; }
	else {}
	return chunk_size;
}
unsigned int 
	#ifdef SW 
	actspartition::
	#endif 
getpartition(keyvalue_t keyvalue, unsigned int currentLOP, vertex_t upperlimit){
	partition_type partition = ((keyvalue.key - upperlimit) >> (BATCH_RANGE_POW - (NUM_PARTITIONS_POW * currentLOP)));
	#ifndef ENABLE_PERFECTACCURACY
	#ifdef _DEBUGMODE_CHECKS2
	if(partition >= NUM_PARTITIONS){ actsutilityobj->globalstats_counterrorsingetpartition(1); }
	// if(partition >= NUM_PARTITIONS){ cout<<"getpartition ERROR: out of range. partition: "<<partition<<", NUM_PARTITIONS: "<<NUM_PARTITIONS<<", keyvalue.key: "<<keyvalue.key<<", upperlimit: "<<upperlimit<<", currentLOP: "<<currentLOP<<endl; }
	#endif 
	partition = checkandforce(partition, NUM_PARTITIONS); 
	#endif
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("actspartition::getpartition", partition, NUM_PARTITIONS, keyvalue.key, upperlimit, currentLOP);
	#endif
	return partition;
}
unsigned int 
	#ifdef SW 
	actspartition::
	#endif 
getglobalpartition(keyvalue_t keyvalue, vertex_t upperlimit){
	partition_type globalpartition = ((keyvalue.key - upperlimit) >> (BATCH_RANGE_POW - (NUM_PARTITIONS_POW * TREE_DEPTH)));
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("actspartition::getglobalpartition", globalpartition, NUMLASTLEVELPARTITIONS, keyvalue.key, upperlimit, NAp);
	#endif
	return globalpartition;
}
keyvalue_t 
	#ifdef SW 
	actspartition::
	#endif 
getkeyvalue(uint512_dt * buffer, batch_type offset_kvs, batch_type addr){
	#pragma HLS INLINE
	keyvalue_t keyvalue;
	batch_type idy = addr / VECTOR_SIZE;
	vector_type idx = addr % VECTOR_SIZE;
	
	#ifdef _WIDEWORD
if(idx==0){	
		keyvalue.key = buffer[offset_kvs + idy].range(31, 0);
		keyvalue.value = buffer[offset_kvs + idy].range(63, 32);
	}
else if(idx==1){	
		keyvalue.key = buffer[offset_kvs + idy].range(95, 64);
		keyvalue.value = buffer[offset_kvs + idy].range(127, 96);
	}
else if(idx==2){	
		keyvalue.key = buffer[offset_kvs + idy].range(159, 128);
		keyvalue.value = buffer[offset_kvs + idy].range(191, 160);
	}
else if(idx==3){	
		keyvalue.key = buffer[offset_kvs + idy].range(223, 192);
		keyvalue.value = buffer[offset_kvs + idy].range(255, 224);
	}
else if(idx==4){	
		keyvalue.key = buffer[offset_kvs + idy].range(287, 256);
		keyvalue.value = buffer[offset_kvs + idy].range(319, 288);
	}
else if(idx==5){	
		keyvalue.key = buffer[offset_kvs + idy].range(351, 320);
		keyvalue.value = buffer[offset_kvs + idy].range(383, 352);
	}
else if(idx==6){	
		keyvalue.key = buffer[offset_kvs + idy].range(415, 384);
		keyvalue.value = buffer[offset_kvs + idy].range(447, 416);
	}
else if(idx==7){	
		keyvalue.key = buffer[offset_kvs + idy].range(479, 448);
		keyvalue.value = buffer[offset_kvs + idy].range(511, 480);
	}
	else {}
	#else
	keyvalue = buffer[offset_kvs + idy].data[idx];
	#endif
	return keyvalue;
}
keyvalue_t 
	#ifdef SW 
	actspartition::
	#endif 
getkeyvalue(uint512_dt * buffer, vector_type idx){
	#pragma HLS INLINE
	keyvalue_t keyvalue;
	
	#ifdef _WIDEWORD
if(idx==0){	
		keyvalue.key = buffer->range(31, 0);
		keyvalue.value = buffer->range(63, 32);
	}
else if(idx==1){	
		keyvalue.key = buffer->range(95, 64);
		keyvalue.value = buffer->range(127, 96);
	}
else if(idx==2){	
		keyvalue.key = buffer->range(159, 128);
		keyvalue.value = buffer->range(191, 160);
	}
else if(idx==3){	
		keyvalue.key = buffer->range(223, 192);
		keyvalue.value = buffer->range(255, 224);
	}
else if(idx==4){	
		keyvalue.key = buffer->range(287, 256);
		keyvalue.value = buffer->range(319, 288);
	}
else if(idx==5){	
		keyvalue.key = buffer->range(351, 320);
		keyvalue.value = buffer->range(383, 352);
	}
else if(idx==6){	
		keyvalue.key = buffer->range(415, 384);
		keyvalue.value = buffer->range(447, 416);
	}
else if(idx==7){	
		keyvalue.key = buffer->range(479, 448);
		keyvalue.value = buffer->range(511, 480);
	}
	else {}
	#else
	keyvalue = buffer->data[idx];
	#endif
	return keyvalue;
}
void 
	#ifdef SW 
	actspartition::
	#endif 
setkeyvalue(uint512_dt * buffer, batch_type offset_kvs, batch_type addr, keyvalue_t keyvalue){
	#pragma HLS INLINE
	batch_type idy = addr / VECTOR_SIZE;
	vector_type idx = addr % VECTOR_SIZE;
	
	#ifdef _WIDEWORD
if(idx==0){	
		buffer[offset_kvs + idy].range(31, 0) = keyvalue.key;
		buffer[offset_kvs + idy].range(63, 32) = keyvalue.value;
	}
else if(idx==1){	
		buffer[offset_kvs + idy].range(95, 64) = keyvalue.key;
		buffer[offset_kvs + idy].range(127, 96) = keyvalue.value;
	}
else if(idx==2){	
		buffer[offset_kvs + idy].range(159, 128) = keyvalue.key;
		buffer[offset_kvs + idy].range(191, 160) = keyvalue.value;
	}
else if(idx==3){	
		buffer[offset_kvs + idy].range(223, 192) = keyvalue.key;
		buffer[offset_kvs + idy].range(255, 224) = keyvalue.value;
	}
else if(idx==4){	
		buffer[offset_kvs + idy].range(287, 256) = keyvalue.key;
		buffer[offset_kvs + idy].range(319, 288) = keyvalue.value;
	}
else if(idx==5){	
		buffer[offset_kvs + idy].range(351, 320) = keyvalue.key;
		buffer[offset_kvs + idy].range(383, 352) = keyvalue.value;
	}
else if(idx==6){	
		buffer[offset_kvs + idy].range(415, 384) = keyvalue.key;
		buffer[offset_kvs + idy].range(447, 416) = keyvalue.value;
	}
else if(idx==7){	
		buffer[offset_kvs + idy].range(479, 448) = keyvalue.key;
		buffer[offset_kvs + idy].range(511, 480) = keyvalue.value;
	}
	else {}
	#else
	buffer[offset_kvs + idy].data[idx] = keyvalue;
	#endif
	return;
}
void 
	#ifdef SW 
	actspartition::
	#endif 
setkeyvalue(uint512_dt * buffer, vector_type idx, keyvalue_t keyvalue){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
if(idx==0){	
		buffer->range(31, 0) = keyvalue.key;
		buffer->range(63, 32) = keyvalue.value;
	}
else if(idx==1){	
		buffer->range(95, 64) = keyvalue.key;
		buffer->range(127, 96) = keyvalue.value;
	}
else if(idx==2){	
		buffer->range(159, 128) = keyvalue.key;
		buffer->range(191, 160) = keyvalue.value;
	}
else if(idx==3){	
		buffer->range(223, 192) = keyvalue.key;
		buffer->range(255, 224) = keyvalue.value;
	}
else if(idx==4){	
		buffer->range(287, 256) = keyvalue.key;
		buffer->range(319, 288) = keyvalue.value;
	}
else if(idx==5){	
		buffer->range(351, 320) = keyvalue.key;
		buffer->range(383, 352) = keyvalue.value;
	}
else if(idx==6){	
		buffer->range(415, 384) = keyvalue.key;
		buffer->range(447, 416) = keyvalue.value;
	}
else if(idx==7){	
		buffer->range(479, 448) = keyvalue.key;
		buffer->range(511, 480) = keyvalue.value;
	}
	else {}
	#else
	buffer->data[idx] = keyvalue;
	#endif
	return;
}
void 
	#ifdef SW 
	actspartition::
	#endif 
setkey(uint512_dt * buffer, vector_type idx, keyy_t key){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
if(idx==0){
		buffer->range(31, 0) = key;
	}
else if(idx==1){
		buffer->range(95, 64) = key;
	}
else if(idx==2){
		buffer->range(159, 128) = key;
	}
else if(idx==3){
		buffer->range(223, 192) = key;
	}
else if(idx==4){
		buffer->range(287, 256) = key;
	}
else if(idx==5){
		buffer->range(351, 320) = key;
	}
else if(idx==6){
		buffer->range(415, 384) = key;
	}
else if(idx==7){
		buffer->range(479, 448) = key;
	}
	else {}
	#else
	buffer->data[idx].key = key;
	#endif
	return;
}
void 
	#ifdef SW 
	actspartition::
	#endif 
copykeyvalues(keyvalue_t * buffer1, keyvalue_t * buffer2, unsigned int size){
	COPYKEYS_LOOP: for(buffer_type i=0; i<size; i++){ buffer1[i] = buffer2[i]; }
}
buffer_type 
	#ifdef SW 
	actspartition::
	#endif 
getpartitionwritesz(buffer_type realsize_kvs, buffer_type bramoffset_kvs){
	#pragma HLS INLINE
	buffer_type size_kvs;
	
	#ifdef ENABLE_APPROXIMATEPARTITIONWRITES
	if(realsize_kvs >= 0*PADSKIP && realsize_kvs < 1*PADSKIP){
		size_kvs = 1*PADSKIP;
	} else if(realsize_kvs >= 1*PADSKIP && realsize_kvs < 2*PADSKIP){
		size_kvs = 2*PADSKIP;
	} else if(realsize_kvs >= 2*PADSKIP && realsize_kvs < 3*PADSKIP){
		size_kvs = 3*PADSKIP;
	} else if(realsize_kvs >= 3*PADSKIP && realsize_kvs < 4*PADSKIP){
		size_kvs = 4*PADSKIP;
	} else if(realsize_kvs >= 4*PADSKIP && realsize_kvs < 5*PADSKIP){
		size_kvs = 5*PADSKIP;
	} else if(realsize_kvs >= 5*PADSKIP && realsize_kvs < 6*PADSKIP){
		size_kvs = 6*PADSKIP;
	} else if(realsize_kvs >= 6*PADSKIP && realsize_kvs < 7*PADSKIP){
		size_kvs = 7*PADSKIP;
	} else if(realsize_kvs >= 7*PADSKIP && realsize_kvs < 8*PADSKIP){
		size_kvs = 8*PADSKIP;
	} else if(realsize_kvs >= 8*PADSKIP && realsize_kvs < 9*PADSKIP){
		size_kvs = 9*PADSKIP;
	} else if(realsize_kvs >= 9*PADSKIP && realsize_kvs < 10*PADSKIP){
		size_kvs = 10*PADSKIP;
	} else if(realsize_kvs >= 10*PADSKIP && realsize_kvs < 11*PADSKIP){
		size_kvs = 11*PADSKIP;
	} else if(realsize_kvs >= 11*PADSKIP && realsize_kvs < 12*PADSKIP){
		size_kvs = 12*PADSKIP;
	} else if(realsize_kvs >= 12*PADSKIP && realsize_kvs < 13*PADSKIP){
		size_kvs = 13*PADSKIP;
	} else if(realsize_kvs >= 13*PADSKIP && realsize_kvs < 14*PADSKIP){
		size_kvs = 14*PADSKIP;
	} else if(realsize_kvs >= 14*PADSKIP && realsize_kvs < 15*PADSKIP){
		size_kvs = 15*PADSKIP;
	} else if(realsize_kvs >= 15*PADSKIP && realsize_kvs < 16*PADSKIP){
		size_kvs = 16*PADSKIP;
	} else if(realsize_kvs >= 16*PADSKIP && realsize_kvs < 17*PADSKIP){
		size_kvs = 17*PADSKIP;
	} else if(realsize_kvs >= 17*PADSKIP && realsize_kvs < 18*PADSKIP){
		size_kvs = 18*PADSKIP;
	} else if(realsize_kvs >= 18*PADSKIP && realsize_kvs < 19*PADSKIP){
		size_kvs = 19*PADSKIP;
	} else if(realsize_kvs >= 19*PADSKIP && realsize_kvs < 20*PADSKIP){
		size_kvs = 20*PADSKIP;
	} else if(realsize_kvs >= 20*PADSKIP && realsize_kvs < 21*PADSKIP){
		size_kvs = 21*PADSKIP;
	} else if(realsize_kvs >= 21*PADSKIP && realsize_kvs < 22*PADSKIP){
		size_kvs = 22*PADSKIP;
	} else if(realsize_kvs >= 22*PADSKIP && realsize_kvs < 23*PADSKIP){
		size_kvs = 23*PADSKIP;
	} else if(realsize_kvs >= 23*PADSKIP && realsize_kvs < 24*PADSKIP){
		size_kvs = 24*PADSKIP;
	} else {
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"WARNING:getpartitionwritesz: should not get here. something might be wrong. realsize_kvs: "<<realsize_kvs<<", size_kvs: "<<size_kvs<<", OPTIMALSIZE: "<<OPTIMALSIZE<<", PADSKIP: "<<PADSKIP<<endl; 
		exit(EXIT_FAILURE);
		#endif
		size_kvs = PADDEDDESTBUFFER_SIZE;
	}
	#ifdef SW 
	if((bramoffset_kvs + size_kvs) >= PADDEDDESTBUFFER_SIZE){ size_kvs = PADDEDDESTBUFFER_SIZE - bramoffset_kvs - 1; } 
	#endif 
	#else 
	size_kvs = realsize_kvs;
	#endif
	return size_kvs;
}

unsigned int 
	#ifdef SW 
	actspartition::
	#endif 
WithinValidRange(unsigned int val1, unsigned int val2){
	#pragma HLS INLINE
	if(val1 < val2){ return 1; }
	else { return 0; }
}

void 
	#ifdef SW 
	actspartition::
	#endif 
calculateoffsets(keyvalue_t * buffer, unsigned int size, unsigned int base, unsigned int skipspacing){
	#pragma HLS INLINE
	buffer[0].key = base;
	for(partition_type i=1; i<size; i++){ 
	// #pragma HLS UNROLL
		buffer[i].key = allignhigher_KV(buffer[i-1].key + buffer[i-1].value + skipspacing); 
	}
}
void 
	#ifdef SW 
	actspartition::
	#endif 
customcalculateoffsets(uint512_dt * buffer, unsigned int size, unsigned int base){
	#pragma HLS INLINE
	unsigned int analysis_vectorsize = VECTOR_SIZE;
	setkey(buffer, 0, base);
	for(vector_type v=1; v<size; v++){ 
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_vectorsize avg=analysis_vectorsize
	#pragma HLS UNROLL
		keyvalue_t nminus1kv = getkeyvalue(buffer, v-1);
		unsigned int skipby = ((nminus1kv.value / VECTOR_SIZE) / (SRCBUFFER_SIZE / NUM_PARTITIONS)) * VECTOR_SIZE;
		keyy_t nkey = allignhigher_KV(nminus1kv.key + nminus1kv.value + skipby);
		setkey(buffer, v, nkey);
	}
}
void 
	#ifdef SW 
	actspartition::
	#endif 
calculatemanyoffsets(keyvalue_t buffer[NUM_PARTITIONS], unsigned int size, unsigned int base, unsigned int skipspacing){
	for(partition_type i=1; i<size; i++){ 
		buffer[i].key = allignhigher_KV(buffer[i-1].key + buffer[i-1].value + skipspacing); 
	}
	return;
}

void 
	#ifdef SW 
	actspartition::
	#endif 
readglobalstats0(unsigned int enable, uint512_dt * kvdram, keyvalue_t buffer[NUM_PARTITIONS], unsigned int offset, unsigned int currentLOP, unsigned int sourceordest){
	if(enable == OFF){ return; }
	resetkeyandvalues(buffer, NUM_PARTITIONS);
	// Not yet Implemented
	return;
}

void 
	#ifdef SW 
	actspartition::
	#endif 
saveglobalstats0(unsigned int enable, uint512_dt * kvdram, uint512_dt buffer[PADDEDDESTBUFFER_SIZE]){
	if(enable == OFF){ return; }
	// Not yet Implemented
}

void 
	#ifdef SW 
	actspartition::
	#endif
prepareglobalstats0(unsigned int enable, uint512_dt * kvdram){
	if(enable == OFF){ return; }
	// Not yet Implemented
}

void 
	#ifdef SW 
	actspartition::
	#endif 
read0(unsigned int enable, uint512_dt * kvdram, keyvalue_t buffer[VECTOR_SIZE][SRCBUFFER_SIZE], batch_type offset_kvs, alw_travstate_t travstate){
	if(enable == OFF){ return; }
	unsigned int analysis_srcbuffersz = SRCBUFFER_SIZE;
	buffer_type chunk_size = getchunksize(SRCBUFFER_SIZE, travstate, SRCBUFFER_SIZE);
	batch_type dramoffset_kvs = offset_kvs + SRCBUFFER_SIZE;
	
	READKVS_LOOP: for (buffer_type i=0; i<chunk_size; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
	#pragma HLS PIPELINE II=1
		buffer[0][i] = kvdram[dramoffset_kvs + i].data[0]; 
		buffer[1][i] = kvdram[dramoffset_kvs + i].data[1]; 
		buffer[2][i] = kvdram[dramoffset_kvs + i].data[2]; 
		buffer[3][i] = kvdram[dramoffset_kvs + i].data[3]; 
		buffer[4][i] = kvdram[dramoffset_kvs + i].data[4]; 
		buffer[5][i] = kvdram[dramoffset_kvs + i].data[5]; 
		buffer[6][i] = kvdram[dramoffset_kvs + i].data[6]; 
		buffer[7][i] = kvdram[dramoffset_kvs + i].data[7]; 
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	return;
}
	
void 
	#ifdef SW 
	actspartition::
	#endif
partitionkeyvalues0(keyvalue_t sourcebuffer[VECTOR_SIZE][SRCBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][SRCBUFFER_SIZE], keyvalue_t localcapsule[VECTOR_SIZE][NUM_PARTITIONS], unsigned int currentLOP, unsigned int upperlimit){
	unsigned int analysis_srcbuffersz = SRCBUFFER_SIZE;
	for(unsigned int i=0; i<SRCBUFFER_SIZE; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
	#pragma HLS PIPELINE II=1
		keyvalue_t keyvalue0 = sourcebuffer[0][i];
		keyvalue_t keyvalue1 = sourcebuffer[1][i];
		keyvalue_t keyvalue2 = sourcebuffer[2][i];
		keyvalue_t keyvalue3 = sourcebuffer[3][i];
		keyvalue_t keyvalue4 = sourcebuffer[4][i];
		keyvalue_t keyvalue5 = sourcebuffer[5][i];
		keyvalue_t keyvalue6 = sourcebuffer[6][i];
		keyvalue_t keyvalue7 = sourcebuffer[7][i];
		
		partition_type p0 = getpartition(keyvalue0, currentLOP, upperlimit);
		partition_type p1 = getpartition(keyvalue1, currentLOP, upperlimit);
		partition_type p2 = getpartition(keyvalue2, currentLOP, upperlimit);
		partition_type p3 = getpartition(keyvalue3, currentLOP, upperlimit);
		partition_type p4 = getpartition(keyvalue4, currentLOP, upperlimit);
		partition_type p5 = getpartition(keyvalue5, currentLOP, upperlimit);
		partition_type p6 = getpartition(keyvalue6, currentLOP, upperlimit);
		partition_type p7 = getpartition(keyvalue7, currentLOP, upperlimit);
		
		destbuffer[0][localcapsule[0][p0].key + localcapsule[0][p0].value] = keyvalue0;
		destbuffer[1][localcapsule[1][p1].key + localcapsule[1][p1].value] = keyvalue1;
		destbuffer[2][localcapsule[2][p2].key + localcapsule[2][p2].value] = keyvalue2;
		destbuffer[3][localcapsule[3][p3].key + localcapsule[3][p3].value] = keyvalue3;
		destbuffer[4][localcapsule[4][p4].key + localcapsule[4][p4].value] = keyvalue4;
		destbuffer[5][localcapsule[5][p5].key + localcapsule[5][p5].value] = keyvalue5;
		destbuffer[6][localcapsule[6][p6].key + localcapsule[6][p6].value] = keyvalue6;
		destbuffer[7][localcapsule[7][p7].key + localcapsule[7][p7].value] = keyvalue7;
		
		localcapsule[0][p0].value += 1;
		localcapsule[1][p1].value += 1;
		localcapsule[2][p2].value += 1;
		localcapsule[3][p3].value += 1;
		localcapsule[4][p4].value += 1;
		localcapsule[5][p5].value += 1;
		localcapsule[6][p6].value += 1;
		localcapsule[7][p7].value += 1;
	}
	return;
}
void 
	#ifdef SW 
	actspartition::
	#endif
combineSetof1stoSetof20_I0(keyvalue_t buffer_setof1M[SRCBUFFER_SIZE], keyvalue_t buffer_setof1N[SRCBUFFER_SIZE], uint128_dt buffer_setof2[SRCBUFFER_SIZE], keyvalue_t localcapsuleM[NUM_PARTITIONS], keyvalue_t localcapsuleN[NUM_PARTITIONS]){
	unsigned int analysis_srcbuffersz = SRCBUFFER_SIZE / NUM_PARTITIONS;
	unsigned int index=0, counter=0, begin=0, end=0;
	EXECUTE_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		EXECUTE_LOOP1B: for(unsigned int i=0; i<2; i++){
			if(i==0){ begin = localcapsuleM[p].key; end = localcapsuleM[p].key + localcapsuleM[p].value; }
			else { begin = localcapsuleN[p].key; end = localcapsuleN[p].key + localcapsuleN[p].value; }
			
			EXECUTE_LOOP1C: for(unsigned int k=begin; k<end; k++){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
			#pragma HLS PIPELINE II=1
				if(counter % 2 == 0){
					buffer_setof2[index].data[0] = buffer_setof1M[k];
				} else {
					buffer_setof2[index].data[1] = buffer_setof1N[k];
				}
				counter+=1; if(counter % 2 == 0){ index++; }
			}
		}
	}
	accumkeysandvalues(localcapsuleM, localcapsuleN, NUM_PARTITIONS);
	return;
}
void 
	#ifdef SW 
	actspartition::
	#endif
combineSetof1stoSetof20_I1(keyvalue_t buffer_setof1M[SRCBUFFER_SIZE], keyvalue_t buffer_setof1N[SRCBUFFER_SIZE], uint128_dt buffer_setof2[SRCBUFFER_SIZE], keyvalue_t localcapsuleM[NUM_PARTITIONS], keyvalue_t localcapsuleN[NUM_PARTITIONS]){
	unsigned int analysis_srcbuffersz = SRCBUFFER_SIZE / NUM_PARTITIONS;
	unsigned int index=0, counter=0, begin=0, end=0;
	EXECUTE_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		EXECUTE_LOOP1B: for(unsigned int i=0; i<2; i++){
			if(i==0){ begin = localcapsuleM[p].key; end = localcapsuleM[p].key + localcapsuleM[p].value; }
			else { begin = localcapsuleN[p].key; end = localcapsuleN[p].key + localcapsuleN[p].value; }
			
			EXECUTE_LOOP1C: for(unsigned int k=begin; k<end; k++){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
			#pragma HLS PIPELINE II=1
				if(counter % 2 == 0){
					buffer_setof2[index].data[0] = buffer_setof1M[k];
				} else {
					buffer_setof2[index].data[1] = buffer_setof1N[k];
				}
				counter+=1; if(counter % 2 == 0){ index++; }
			}
		}
	}
	accumkeysandvalues(localcapsuleM, localcapsuleN, NUM_PARTITIONS);
	return;
}
void 
	#ifdef SW 
	actspartition::
	#endif
combineSetof1stoSetof20_I2(keyvalue_t buffer_setof1M[SRCBUFFER_SIZE], keyvalue_t buffer_setof1N[SRCBUFFER_SIZE], uint128_dt buffer_setof2[SRCBUFFER_SIZE], keyvalue_t localcapsuleM[NUM_PARTITIONS], keyvalue_t localcapsuleN[NUM_PARTITIONS]){
	unsigned int analysis_srcbuffersz = SRCBUFFER_SIZE / NUM_PARTITIONS;
	unsigned int index=0, counter=0, begin=0, end=0;
	EXECUTE_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		EXECUTE_LOOP1B: for(unsigned int i=0; i<2; i++){
			if(i==0){ begin = localcapsuleM[p].key; end = localcapsuleM[p].key + localcapsuleM[p].value; }
			else { begin = localcapsuleN[p].key; end = localcapsuleN[p].key + localcapsuleN[p].value; }
			
			EXECUTE_LOOP1C: for(unsigned int k=begin; k<end; k++){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
			#pragma HLS PIPELINE II=1
				if(counter % 2 == 0){
					buffer_setof2[index].data[0] = buffer_setof1M[k];
				} else {
					buffer_setof2[index].data[1] = buffer_setof1N[k];
				}
				counter+=1; if(counter % 2 == 0){ index++; }
			}
		}
	}
	accumkeysandvalues(localcapsuleM, localcapsuleN, NUM_PARTITIONS);
	return;
}
void 
	#ifdef SW 
	actspartition::
	#endif
combineSetof1stoSetof20_I3(keyvalue_t buffer_setof1M[SRCBUFFER_SIZE], keyvalue_t buffer_setof1N[SRCBUFFER_SIZE], uint128_dt buffer_setof2[SRCBUFFER_SIZE], keyvalue_t localcapsuleM[NUM_PARTITIONS], keyvalue_t localcapsuleN[NUM_PARTITIONS]){
	unsigned int analysis_srcbuffersz = SRCBUFFER_SIZE / NUM_PARTITIONS;
	unsigned int index=0, counter=0, begin=0, end=0;
	EXECUTE_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		EXECUTE_LOOP1B: for(unsigned int i=0; i<2; i++){
			if(i==0){ begin = localcapsuleM[p].key; end = localcapsuleM[p].key + localcapsuleM[p].value; }
			else { begin = localcapsuleN[p].key; end = localcapsuleN[p].key + localcapsuleN[p].value; }
			
			EXECUTE_LOOP1C: for(unsigned int k=begin; k<end; k++){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
			#pragma HLS PIPELINE II=1
				if(counter % 2 == 0){
					buffer_setof2[index].data[0] = buffer_setof1M[k];
				} else {
					buffer_setof2[index].data[1] = buffer_setof1N[k];
				}
				counter+=1; if(counter % 2 == 0){ index++; }
			}
		}
	}
	accumkeysandvalues(localcapsuleM, localcapsuleN, NUM_PARTITIONS);
	return;
}
void 
	#ifdef SW 
	actspartition::
	#endif
combineSetof2stoSetof40_I0(uint128_dt buffer_setof2M[SRCBUFFER_SIZE], uint128_dt buffer_setof2N[SRCBUFFER_SIZE], uint256_dt buffer_setof4[SRCBUFFER_SIZE], keyvalue_t localcapsuleM[NUM_PARTITIONS], keyvalue_t localcapsuleN[NUM_PARTITIONS]){
	unsigned int analysis_srcbuffersz = SRCBUFFER_SIZE / NUM_PARTITIONS;
	unsigned int index=0, counter=0, begin=0, end=0;
	EXECUTE_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		EXECUTE_LOOP1B: for(unsigned int i=0; i<2; i++){
			if(i==0){ begin = localcapsuleM[p].key / 2; end = (localcapsuleM[p].key + localcapsuleM[p].value) / 2; }
			else { begin = localcapsuleN[p].key / 2; end = (localcapsuleN[p].key + localcapsuleN[p].value) / 2; }
			
			EXECUTE_LOOP1C: for(unsigned int k=begin; k<end; k++){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
			#pragma HLS PIPELINE II=1
				if(counter % 2 == 0){
					buffer_setof4[index].data[0] = buffer_setof2M[k].data[0];
					buffer_setof4[index].data[1] = buffer_setof2M[k].data[1]; 
				} else {
					buffer_setof4[index].data[2] = buffer_setof2N[k].data[0];
					buffer_setof4[index].data[3] = buffer_setof2N[k].data[1]; 
				}
				counter+=1; if(counter % 2 == 0){ index++; }
			}
		}
	}
	accumkeysandvalues(localcapsuleM, localcapsuleN, NUM_PARTITIONS);
	return;
}
void 
	#ifdef SW 
	actspartition::
	#endif
combineSetof2stoSetof40_I1(uint128_dt buffer_setof2M[SRCBUFFER_SIZE], uint128_dt buffer_setof2N[SRCBUFFER_SIZE], uint256_dt buffer_setof4[SRCBUFFER_SIZE], keyvalue_t localcapsuleM[NUM_PARTITIONS], keyvalue_t localcapsuleN[NUM_PARTITIONS]){
	unsigned int analysis_srcbuffersz = SRCBUFFER_SIZE / NUM_PARTITIONS;
	unsigned int index=0, counter=0, begin=0, end=0;
	EXECUTE_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		EXECUTE_LOOP1B: for(unsigned int i=0; i<2; i++){
			if(i==0){ begin = localcapsuleM[p].key / 2; end = (localcapsuleM[p].key + localcapsuleM[p].value) / 2; }
			else { begin = localcapsuleN[p].key / 2; end = (localcapsuleN[p].key + localcapsuleN[p].value) / 2; }
			
			EXECUTE_LOOP1C: for(unsigned int k=begin; k<end; k++){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
			#pragma HLS PIPELINE II=1
				if(counter % 2 == 0){
					buffer_setof4[index].data[0] = buffer_setof2M[k].data[0];
					buffer_setof4[index].data[1] = buffer_setof2M[k].data[1]; 
				} else {
					buffer_setof4[index].data[2] = buffer_setof2N[k].data[0];
					buffer_setof4[index].data[3] = buffer_setof2N[k].data[1]; 
				}
				counter+=1; if(counter % 2 == 0){ index++; }
			}
		}
	}
	accumkeysandvalues(localcapsuleM, localcapsuleN, NUM_PARTITIONS);
	return;
}
void 
	#ifdef SW 
	actspartition::
	#endif
combineSetof4stoSetof80_I0(uint256_dt buffer_setof4M[SRCBUFFER_SIZE], uint256_dt buffer_setof4N[SRCBUFFER_SIZE], uint512_dt buffer_setof8s[SRCBUFFER_SIZE], keyvalue_t localcapsuleM[NUM_PARTITIONS], keyvalue_t localcapsuleN[NUM_PARTITIONS]){
	unsigned int analysis_srcbuffersz = SRCBUFFER_SIZE / NUM_PARTITIONS;
	unsigned int index=0, counter=0, begin=0, end=0;
	EXECUTE_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		EXECUTE_LOOP1B: for(unsigned int i=0; i<2; i++){
			if(i==0){ begin = localcapsuleM[p].key / 4; end = (localcapsuleM[p].key + localcapsuleM[p].value) / 4; }
			else { begin = localcapsuleN[p].key / 4; end = (localcapsuleN[p].key + localcapsuleN[p].value) / 4; }
			
			EXECUTE_LOOP1C: for(unsigned int k=begin; k<end; k++){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
			#pragma HLS PIPELINE II=1
				if(counter % 2 == 0){
					buffer_setof8s[index].data[0] = buffer_setof4M[k].data[0];
					buffer_setof8s[index].data[1] = buffer_setof4M[k].data[1]; 
					buffer_setof8s[index].data[2] = buffer_setof4M[k].data[2]; 
					buffer_setof8s[index].data[3] = buffer_setof4M[k].data[3];
				} else {
					buffer_setof8s[index].data[4] = buffer_setof4N[k].data[0];
					buffer_setof8s[index].data[5] = buffer_setof4N[k].data[1]; 
					buffer_setof8s[index].data[6] = buffer_setof4N[k].data[2]; 
					buffer_setof8s[index].data[7] = buffer_setof4N[k].data[3];
				}
				counter+=1; if(counter % 2 == 0){ index++; }
			}
		}
	}
	accumkeysandvalues(localcapsuleM, localcapsuleN, NUM_PARTITIONS);
	return;
}

void 
	#ifdef SW 
	actspartition::
	#endif 
save0(unsigned int enable, uint512_dt * kvdram, uint512_dt buffer[PADDEDDESTBUFFER_SIZE], keyvalue_t * globalcapsule, keyvalue_t localcapsule[NUM_PARTITIONS], batch_type globalbaseaddress_kvs){				
	if(enable == OFF){ return; }
	unsigned int analysis_destpartitionsz = PADDEDDESTBUFFER_SIZE / NUM_PARTITIONS;
	SAVEPARTITIONS_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		batch_type dramoffset_kvs = globalbaseaddress_kvs + ((globalcapsule[p].key + globalcapsule[p].value) / VECTOR_SIZE);
		buffer_type bramoffset_kvs = localcapsule[p].key / VECTOR_SIZE;
		buffer_type realsize_kvs = (localcapsule[p].value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
		buffer_type size_kvs = getpartitionwritesz(realsize_kvs, bramoffset_kvs);
		
		SAVEPARTITIONS_LOOP1B: for(buffer_type i=0; i<size_kvs; i++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_destpartitionsz avg=analysis_destpartitionsz
		#pragma HLS PIPELINE II=1
			kvdram[dramoffset_kvs + i] = buffer[bramoffset_kvs + i];
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countkvspartitionswritten_actual(VECTOR_SIZE);
			#endif
		}
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvspartitionswritten(size_kvs * VECTOR_SIZE);
		actsutilityobj->globalvar_inmemory_counttotalvalidkeyvalues(localcapsule[p].value);
		#endif
	}
	SAVEPARTITIONS_LOOP2: for(partition_type p=0; p<NUM_PARTITIONS; p++){ globalcapsule[p].value += allignhigher_KV(localcapsule[p].value); }
	return;
}

void 
	#ifdef SW 
	actspartition::
	#endif 
dispatch0(uint512_dt * kvdram, keyvalue_t sourcebuffer[VECTOR_SIZE][SRCBUFFER_SIZE], uint512_dt destbuffer[PADDEDDESTBUFFER_SIZE], keyvalue_t localcapsule[NUM_PARTITIONS], keyvalue_t globalcapsule[NUM_PARTITIONS], alw_config config, alw_sweepparams_t sweepparams, alw_travstate_t travstate){
	unsigned int analysis_mainloop = KVDATA_BATCHSIZE_KVS / SRCBUFFER_SIZE;
	
	keyvalue_t buffer_setof1[8][SRCBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer_setof1
	
	uint128_dt buffer_setof2[4][SRCBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer_setof2
	
	uint256_dt buffer_setof4[2][SRCBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer_setof4
	
	keyvalue_t templocalcapsule[8][NUM_PARTITIONS];
	#pragma HLS array_partition variable = templocalcapsule
	
	MAIN_LOOP: for(unsigned int offset_kvs=travstate.begin_kvs; offset_kvs<travstate.end_kvs; offset_kvs+=SRCBUFFER_SIZE)
	// MAIN_LOOP: for(unsigned int offset_kvs=travstate.begin_kvs; offset_kvs<(travstate.begin_kvs + (16 * SRCBUFFER_SIZE)); offset_kvs+=SRCBUFFER_SIZE)
	{
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_mainloop avg=analysis_mainloop	
		#ifdef _DEBUGMODE_KERNELPRINTS
		actsutilityobj->print4("### dispatch0:: offset_kvs", "begin_kvs", "end_kvs", "skip", offset_kvs, travstate.begin_kvs, travstate.end_kvs, SRCBUFFER_SIZE);
		#endif 
		travstate.i_kvs = offset_kvs;
		
		resetkeyandvalues(templocalcapsule[0], NUM_PARTITIONS);
		resetkeyandvalues(templocalcapsule[1], NUM_PARTITIONS);
		resetkeyandvalues(templocalcapsule[2], NUM_PARTITIONS);
		resetkeyandvalues(templocalcapsule[3], NUM_PARTITIONS);
		resetkeyandvalues(templocalcapsule[4], NUM_PARTITIONS);
		resetkeyandvalues(templocalcapsule[5], NUM_PARTITIONS);
		resetkeyandvalues(templocalcapsule[6], NUM_PARTITIONS);
		resetkeyandvalues(templocalcapsule[7], NUM_PARTITIONS);
		
		read0(config.enablereadkvs, kvdram, sourcebuffer, (sweepparams.worksourcebaseaddress_kvs + offset_kvs), travstate);
		
		// 8->8
		partitionkeyvalues0(sourcebuffer, buffer_setof1, templocalcapsule, sweepparams.currentLOP, sweepparams.upperlimit);

		// 8->4
		combineSetof1stoSetof20_I0(buffer_setof1[0], buffer_setof1[1], buffer_setof2[0], templocalcapsule[0], templocalcapsule[1]);
		combineSetof1stoSetof20_I1(buffer_setof1[2], buffer_setof1[3], buffer_setof2[1], templocalcapsule[2], templocalcapsule[3]);
		combineSetof1stoSetof20_I2(buffer_setof1[4], buffer_setof1[5], buffer_setof2[2], templocalcapsule[4], templocalcapsule[5]);
		combineSetof1stoSetof20_I3(buffer_setof1[6], buffer_setof1[7], buffer_setof2[3], templocalcapsule[6], templocalcapsule[7]);

		// 4->2
		combineSetof2stoSetof40_I0(buffer_setof2[0], buffer_setof2[1], buffer_setof4[0], templocalcapsule[0], templocalcapsule[2]);
		combineSetof2stoSetof40_I1(buffer_setof2[2], buffer_setof2[3], buffer_setof4[1], templocalcapsule[4], templocalcapsule[6]);
		
		// 2->1
		combineSetof4stoSetof80_I0(buffer_setof4[0], buffer_setof4[1], destbuffer, templocalcapsule[0], templocalcapsule[4]);
		
		copykeyvalues(localcapsule, templocalcapsule[0], NUM_PARTITIONS);
		#ifdef _DEBUGMODE_KERNELPRINTS
		actsutilityobj->printkeyvalues("(fin) dispatch0:: localcapsule::", localcapsule, NUM_PARTITIONS);
		actsutilityobj->printvaluecount("(fin) dispatch0:: localcapsule", localcapsule, NUM_PARTITIONS);
		// exit(EXIT_SUCCESS);
		#endif 
	
		save0(config.enablesavekvs, kvdram, destbuffer, globalcapsule, localcapsule, sweepparams.workdestbaseaddress_kvs);
		break;
	}
	return;
}

extern "C" {
void 
	#ifdef SW 
	actspartition:: 
	#endif
topkernel( uint512_dt * sourceAvolume ){
#pragma HLS INTERFACE m_axi port = sourceAvolume offset = slave bundle = gmem0 // max_read_burst_length=64 max_write_burst_length=64

#pragma HLS INTERFACE s_axilite port = sourceAvolume bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATA_PACK variable = sourceAvolume

	#ifdef _DEBUGMODE_KERNELPRINTS2
	cout<<"Light weight ACTS started."<<endl;
	#endif 
	
	keyvalue_t sourceAbuffer[VECTOR_SIZE][SRCBUFFER_SIZE];
	uint512_dt destAbuffer[PADDEDDESTBUFFER_SIZE];
	keyvalue_t localcapsuleA[NUM_PARTITIONS];
	keyvalue_t globalcapsuleA[NUM_PARTITIONS];
	alw_travstate_t travstateA;
	alw_sweepparams_t sweepparamsA; 
	alw_config config;
	#pragma HLS array_partition variable = sourceAbuffer //block dim=2
	
	unsigned int analysis_mainloop = KVDATA_BATCHSIZE_KVS / SRCBUFFER_SIZE;
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->clearglobalvars();
	#endif
	
	globalparams_t globalparamsA;
	getglobalparams(sourceAvolume, &globalparamsA);
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->printglobalparameters("actspartition:: printing global parameters for instance A", globalparamsA);
	#endif
	resetkeyandvalues(globalcapsuleA, NUM_PARTITIONS);
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->printglobalvars();
	actsutilityobj->clearglobalvars();
	#endif
	
	ACTSLW_LOOP1: for(unsigned int currentLOP=1; currentLOP<=1; currentLOP++){ //TREE_DEPTH//+1:reduce //globalparamsA.treedepth // REMOVEME.
		sweepparamsA.currentLOP = currentLOP;
		if(currentLOP == 0 || (currentLOP % 2) == 1){ sweepparamsA.worksourcebaseaddress_kvs = BASEOFFSET_KVDRAM_KVS; sweepparamsA.workdestbaseaddress_kvs = BASEOFFSET_KVDRAMWORKSPACE_KVS; }
		else { sweepparamsA.worksourcebaseaddress_kvs = BASEOFFSET_KVDRAMWORKSPACE_KVS; sweepparamsA.workdestbaseaddress_kvs = BASEOFFSET_KVDRAM_KVS; }
		unsigned int num_source_partitions = (1 << (NUM_PARTITIONS_POW * subandcap(currentLOP, 1))); // REMOVEME.
		unsigned int sourceskipsize = getskipsize(currentLOP, SOURCE);
		#ifdef _DEBUGMODE_KERNELPRINTS2
		actsutilityobj->print4(">>>>>>> currentLOP", "num_source_partitions", "sourceskipsize", "NAp", currentLOP, num_source_partitions, sourceskipsize, NAp);
		#endif 
		if(currentLOP == 0){ config.collectglobalstatscmd = ON; config.collectstatsandpartitioncmd = OFF; config.reducecmd = OFF; config.enablereadkvs = ON; config.enabledispatch = ON; config.enablesavekvs = OFF; config.enableresetdestbuffer = ON; config.enablereadglobalstats = OFF; config.enablesaveglobalstats = ON; } 
		else { config.collectglobalstatscmd = OFF; config.collectstatsandpartitioncmd = ON; config.reducecmd = OFF; config.enablereadkvs = ON; config.enabledispatch = ON; config.enablesavekvs = ON; config.enableresetdestbuffer = OFF; config.enablereadglobalstats = ON; config.enablesaveglobalstats = OFF; }
		
		resetkeyandvalues(config.enableresetdestbuffer, destAbuffer);
		
		readglobalstats0(config.enablereadglobalstats, sourceAvolume, globalcapsuleA, 0, sweepparamsA.currentLOP, DEST);
		
		ACTSLW_LOOP1B: for(unsigned int source_partition=0; source_partition<num_source_partitions; source_partition+=1){
			sweepparamsA.upperlimit = globalparamsA.vbegin + (source_partition * sourceskipsize * (BATCH_RANGE / NUMLASTLEVELPARTITIONS));
			
			keyvalue_t keyvalueA;
			if(currentLOP < 2){ keyvalueA.key = 0; }
			else { keyvalueA = getkeyvalue(sourceAvolume, BASEOFFSET_STATSDRAM_KVS, source_partition * sourceskipsize); }
			keyvalue_t nextkeyvalueA; nextkeyvalueA.key = globalparamsA.runsize;
			if(currentLOP >= 2 && source_partition < (num_source_partitions - 1)){ nextkeyvalueA = getkeyvalue(sourceAvolume, BASEOFFSET_STATSDRAM_KVS, (source_partition + 1) * sourceskipsize); }
			travstateA.begin_kvs = keyvalueA.key / VECTOR_SIZE; 
			travstateA.end_kvs = nextkeyvalueA.key / VECTOR_SIZE;
			travstateA.skip_kvs = SRCBUFFER_SIZE;
			
			#ifdef _DEBUGMODE_KERNELPRINTS2
			actsutilityobj->print4("### source_partition", "sweepparamsA.upperlimit", "travstateA.begin", "travstateA.end", source_partition, sweepparamsA.upperlimit, travstateA.begin_kvs * VECTOR_SIZE, travstateA.end_kvs * VECTOR_SIZE);
			#endif 
			dispatch0(sourceAvolume, sourceAbuffer, destAbuffer, localcapsuleA, globalcapsuleA, config, sweepparamsA, travstateA);
		}
		
		saveglobalstats0(config.enablesaveglobalstats, sourceAvolume, destAbuffer);
 
		
 // REMOVEME.
		prepareglobalstats0(config.enablesaveglobalstats, sourceAvolume);
		
		#ifdef _DEBUGMODE_KERNELPRINTS2
		actsutilityobj->printglobalvars();
		actsutilityobj->clearglobalvars();
		if(currentLOP>0){ actsutilityobj->printkeyvalues("actspartition::head::globalcapsule", (keyvalue_t *)globalcapsuleA, NUM_PARTITIONS); }
		/* if(currentLOP==1){ actsutilityobj->scankeyvalues((keyvalue_t *)(&sourceAvolume[sweepparamsA.workdestbaseaddress_kvs]), globalcapsuleA, (1 << (NUM_PARTITIONS_POW * currentLOP)), BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * 1))); } */
		#endif 
	}
	return;
}
}








