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
#include "../actsutility/actsutility.h"
#endif 
#include "actspartition.h"
using namespace std;

#ifdef SW
actspartition::actspartition(){ actsutilityobj = new actsutility(); }
actspartition::~actspartition(){}
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
resetkeyandvalues(unsigned int enable, atp_uint512_dt destbuffer[NUMSUBWORKERS][PADDEDDESTBUFFER_SIZE]){
	if(enable == OFF){ return; }
	uint512_dt ZeroKVS;
	keyvalue_t ZeroKV; ZeroKV.key = 0; ZeroKV.value = 0;
	#ifdef _WIDEWORD
	ZeroKVS = 0;
	#else 
	for(vector_type v=0; v<VECTOR_SIZE; v++){ ZeroKVS.data[v] = ZeroKV; }
	#endif 
	for(partition_type i=0; i<PADDEDDESTBUFFER_SIZE; i++){ 
		destbuffer[0][i] = ZeroKVS;
	}
	return;
}
void 
	#ifdef SW 
	actspartition::
	#endif 
resetmanykeyandvalues(keyvalue_t buffer[NUMSUBWORKERS][NUM_PARTITIONS], unsigned int size){
	for(unsigned int i=0; i<size; i++){ 
		buffer[0][i].key = 0; buffer[0][i].value = 0; 
	}
	return;
}
void 
	#ifdef SW 
	actspartition::
	#endif 
resetmanyvalues(keyvalue_t buffer[NUMSUBWORKERS][NUM_PARTITIONS], unsigned int size){
	for(unsigned int i=0; i<size; i++){ 
		buffer[0][i].value = 0; 
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
calculatemanyoffsets(keyvalue_t buffer[NUMSUBWORKERS][NUM_PARTITIONS], unsigned int size, unsigned int base, unsigned int skipspacing){
	for(partition_type i=1; i<size; i++){ 
		buffer[0][i].key = allignhigher_KV(buffer[0][i-1].key + buffer[0][i-1].value + skipspacing); 
	}
	return;
}

bool 
	#ifdef SW 
	actspartition::
	#endif 
allowOp(unsigned int i_inloop, buffer_type chunk_size, keyvalue_t keyvalue, unsigned int command){
	if(((WithinValidRange(i_inloop, chunk_size * VECTOR_SIZE) == 1)
		&& (keyvalue.key != INVALIDDATA))
			|| (command == APPENDINVALIDSCOMMAND)
				){ return true; }
	return false;
	
	// return true;
}
keyvalue_t 
	#ifdef SW 
	actspartition::
	#endif 
get(unsigned int enable, unsigned int command, uint512_dt * buffer, buffer_type offset_kvs, buffer_type addr){
	#pragma HLS INLINE
	if(enable == ON){
		if(command == APPENDINVALIDSCOMMAND){
			keyvalue_t InvalidKV; InvalidKV.key = INVALIDDATA; InvalidKV.value = INVALIDDATA;
			return InvalidKV;
		} else {
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("get", addr, SRCBUFFER_SIZE * VECTOR_SIZE, NAp, NAp, NAp);
			#endif 
			return getkeyvalue(buffer, offset_kvs, addr);
		}
	}
}
unsigned int 
	#ifdef SW 
	actspartition::
	#endif 
compute(unsigned int enable, unsigned int command, keyvalue_t keyvalue, unsigned int currentLOP, unsigned int upperlimit, unsigned int partition){
	#pragma HLS INLINE
	if(command == COLLECTGLOBALSTATSCOMMAND){
		return getglobalpartition(keyvalue, upperlimit);
	} else if(command == COLLECTSTATSCOMMAND){
		return getpartition(keyvalue, currentLOP, upperlimit);
	} else if(command == PARTITIONKEYVALUECOMMAND){
		return getpartition(keyvalue, currentLOP, upperlimit);
	} else if(command == APPENDINVALIDSCOMMAND){
		return partition;
	} else { return NAp; }
}
unsigned int 
	#ifdef SW 
	actspartition::
	#endif 
getindex(unsigned int enable, unsigned int command, keyvalue_t localcapsule, unsigned int partition, unsigned int i_inloop, unsigned int ovsize){
	#pragma HLS INLINE
	if(command == COLLECTGLOBALSTATSCOMMAND){ return partition; }
	else if(command == COLLECTSTATSCOMMAND){}
	else if(command == PARTITIONKEYVALUECOMMAND){ return localcapsule.key + localcapsule.value; } 
	else if(command == APPENDINVALIDSCOMMAND){
		if(i_inloop < ovsize){ return localcapsule.key + localcapsule.value + i_inloop; }
		// else { return ((PADDEDDESTBUFFER_SIZE * VECTOR_SIZE) - 1); } 
		else { return NOBUFFERADDRESS; } 
	} 
	else { return NAp; }
}
void 
	#ifdef SW 
	actspartition::
	#endif 
set(unsigned int enable, unsigned int command, uint512_dt * destbuffer, buffer_type offset_kvs, buffer_type index, keyvalue_t keyvalue, keyvalue_t keyvalue2){
	#pragma HLS INLINE
	keyvalue_t keyvaluetoset;
	if(command == COLLECTGLOBALSTATSCOMMAND){ keyvaluetoset = keyvalue2; }
	else if(command == COLLECTSTATSCOMMAND){ keyvaluetoset = keyvalue; }
	else if(command == PARTITIONKEYVALUECOMMAND){ keyvaluetoset = keyvalue; }
	else if(command == APPENDINVALIDSCOMMAND){ keyvaluetoset = keyvalue; }
	else {}
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("set", index, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, NAp, NAp, NAp);
	#endif 
	setkeyvalue(destbuffer, offset_kvs, index, keyvaluetoset); // optimized. set for collect stats is redundant
	return;
}
void 
	#ifdef SW 
	actspartition::
	#endif 
update(unsigned int enable, unsigned int command, keyvalue_t * keyvalue){
	#pragma HLS INLINE
	if(enable == ON){ keyvalue->value += 1; }
}

void 
	#ifdef SW 
	actspartition::
	#endif 
readglobalstats0(unsigned int enable, uint512_dt * kvdram, keyvalue_t buffer[NUM_PARTITIONS], unsigned int offset, unsigned int currentLOP, unsigned int sourceordest){
	if(enable == OFF){ return; }
	unsigned int skipsize = getskipsize(currentLOP, sourceordest);
	unsigned int loffset = 0;
	READ_LOOP: for (buffer_type i=0; i<NUM_PARTITIONS; i++){
		buffer[i] = getkeyvalue(kvdram, BASEOFFSET_STATSDRAM_KVS, offset + loffset);
		loffset += skipsize;
	}
	resetvalues(buffer, NUM_PARTITIONS);
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->printkeyvalues("readglobalstats", buffer, NUM_PARTITIONS);
	#endif 
	return;
}

void 
	#ifdef SW 
	actspartition::
	#endif 
saveglobalstats0(unsigned int enable, uint512_dt * kvdram, atp_uint512_dt buffer[NUMSUBWORKERS][PADDEDDESTBUFFER_SIZE]){
	if(enable == OFF){ return; }
	for(unsigned sw=0; sw<NUMSUBWORKERS; sw++){
		batch_type dramoffset_kvs = BASEOFFSET_STATSDRAM_KVS + (sw * PADDEDDESTBUFFER_SIZE);
		SAVEGLOBALSTATS_LOOP: for (buffer_type i=0; i<PADDEDDESTBUFFER_SIZE; i++){
		#pragma HLS PIPELINE II=1
			kvdram[dramoffset_kvs + i] = buffer[sw][i]; 
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalvar_savestats_counttotalstatswritten(VECTOR_SIZE);
			#endif
		}
	}
}

void 
	#ifdef SW 
	actspartition::
	#endif
prepareglobalstats0(unsigned int enable, uint512_dt * kvdram){
	if(enable == OFF){ return; }
	
	uint512_dt VSUMM;
	uint512_vec_dt Vsum;
	
	for (buffer_type i=0; i<PADDEDDESTBUFFER_SIZE; i++){
		for(vector_type v=0; v<VECTOR_SIZE; v++){ 
			Vsum.data[v].key = 0; Vsum.data[v].value = 0; 
		}
		#ifdef _WIDEWORD
		uint512_vec_dt Vtemp;
		for(unsigned int sw=0; sw<NUMSUBWORKERS; sw++){
			uint512_dt U = kvdram[BASEOFFSET_STATSDRAM_KVS + (sw * PADDEDDESTBUFFER_SIZE) + i];
			
			Vtemp.data[0].key = U.range(31, 0);
			Vtemp.data[0].value = U.range(63, 32);

			Vtemp.data[1].key = U.range(95, 64);
			Vtemp.data[1].value = U.range(127, 96);
		
			Vtemp.data[2].key = U.range(159, 128);
			Vtemp.data[2].value = U.range(191, 160);
		
			Vtemp.data[3].key = U.range(223, 192);
			Vtemp.data[3].value = U.range(255, 224);
		
			Vtemp.data[4].key = U.range(287, 256);
			Vtemp.data[4].value = U.range(319, 288);
		
			Vtemp.data[5].key = U.range(351, 320);
			Vtemp.data[5].value = U.range(383, 352);
		
			Vtemp.data[6].key = U.range(415, 384);
			Vtemp.data[6].value = U.range(447, 416);
		
			Vtemp.data[7].key = U.range(479, 448);
			Vtemp.data[7].value = U.range(511, 480);
			
			for(vector_type v=0; v<VECTOR_SIZE; v++){ Vsum.data[v].value += Vtemp.data[v].value; }
		}
		
		// uint512_dt VSUMM;
		VSUMM.range(31, 0) = Vsum.data[0].key;
		VSUMM.range(63, 32) = Vsum.data[0].value;

		VSUMM.range(95, 64) = Vsum.data[1].key;
		VSUMM.range(127, 96) = Vsum.data[1].value;
	
		VSUMM.range(159, 128) = Vsum.data[2].key;
		VSUMM.range(191, 160) = Vsum.data[2].value;
	
		VSUMM.range(223, 192) = Vsum.data[3].key;
		VSUMM.range(255, 224) = Vsum.data[3].value;
	
		VSUMM.range(287, 256) = Vsum.data[4].key;
		VSUMM.range(319, 288) = Vsum.data[4].value;
	
		VSUMM.range(351, 320) = Vsum.data[5].key;
		VSUMM.range(383, 352) = Vsum.data[5].value;
	
		VSUMM.range(415, 384) = Vsum.data[6].key;
		VSUMM.range(447, 416) = Vsum.data[6].value;
	
		VSUMM.range(479, 448) = Vsum.data[7].key;
		VSUMM.range(511, 480) = Vsum.data[7].value;
		
		kvdram[BASEOFFSET_STATSDRAM_KVS + i] = VSUMM;
		#else
		for(vector_type v=0; v<VECTOR_SIZE; v++){ VSUMM.data[v].key = 0; VSUMM.data[v].value = 0; }
		for(unsigned int sw=0; sw<NUMSUBWORKERS; sw++){
			uint512_dt U = kvdram[BASEOFFSET_STATSDRAM_KVS + (sw * PADDEDDESTBUFFER_SIZE) + i];
			for(vector_type v=0; v<VECTOR_SIZE; v++){ VSUMM.data[v].value += U.data[v].value; }
			kvdram[BASEOFFSET_STATSDRAM_KVS + i] = VSUMM;
		}
		#endif
	}
	
	unsigned int Vrunningsum = 0;
	for (buffer_type i=0; i<PADDEDDESTBUFFER_SIZE; i++){
		uint512_dt tempV = kvdram[BASEOFFSET_STATSDRAM_KVS + i];
		customcalculateoffsets(&tempV, VECTOR_SIZE, Vrunningsum);
		#ifdef _WIDEWORD
		Vrunningsum = tempV.range(479, 448) + tempV.range(511, 480);
		#else 
		Vrunningsum = tempV.data[VECTOR_SIZE-1].key + tempV.data[VECTOR_SIZE-1].value;
		#endif
		
		kvdram[BASEOFFSET_STATSDRAM_KVS + i] = tempV;
	}
}

void 
	#ifdef SW 
	actspartition::
	#endif 
readKVS0(unsigned int enable, uint512_dt * kvdram, atp_uint512_dt buffer[NUMSUBWORKERS][SRCBUFFER_SIZE], batch_type offset_kvs, alw_travstate_t travstate){
	if(enable == OFF){ return; }
	unsigned int analysis_srcbuffersz = SRCBUFFER_SIZE;
	for(unsigned int sw=0; sw<NUMSUBWORKERS; sw++){
		buffer_type chunk_size = getchunksize(SRCBUFFER_SIZE, travstate, sw * SRCBUFFER_SIZE);
		batch_type dramoffset_kvs = offset_kvs + (sw * SRCBUFFER_SIZE);
		READKVS_LOOP: for (buffer_type i=0; i<chunk_size; i++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
		#pragma HLS PIPELINE II=1
			buffer[sw][i] = kvdram[dramoffset_kvs + i]; 
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
			#endif
		}
	}
	return;
}
	
void 
	#ifdef SW 
	actspartition::
	#endif 
execute0(unsigned int enable, unsigned int * enables, unsigned int command, atp_uint512_dt sourcebuffer[NUMSUBWORKERS][SRCBUFFER_SIZE], atp_uint512_dt destbuffer[NUMSUBWORKERS][PADDEDDESTBUFFER_SIZE], keyvalue_t localcapsule[NUMSUBWORKERS][NUM_PARTITIONS], buffer_type chunk_size[NUMSUBWORKERS], alw_sweepparams_t sweepparams, partition_type partition){
	#pragma HLS INLINE OFF
	if(enable == OFF){ return; }
	unsigned int analysis_srcbuffersz = SRCBUFFER_SIZE * VECTOR_SIZE;
	
	buffer_type ovsize0 = allignhigher_KV(localcapsule[0][partition].value) - localcapsule[0][partition].value; // MODIFYME?
	
	partition_type p0;
	buffer_type index0;
	keyvalue_t keyvalue20;
	
	EXECUTE_LOOP: for(buffer_type k=0; k<chunk_size[0] * VECTOR_SIZE; k++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
	#pragma HLS PIPELINE II=3
		keyvalue_t keyvalue0 = get(enables[0], command, sourcebuffer[0], 0, k);
		
		if(allowOp(k, chunk_size[0], keyvalue0, command)){ p0 = compute(enables[1], command, keyvalue0, sweepparams.currentLOP, sweepparams.upperlimit, partition); }
		
		if(allowOp(k, chunk_size[0], keyvalue0, command)){ index0 = getindex(enables[2], command, localcapsule[0][p0], p0, k, ovsize0); }
		
		if(allowOp(k, chunk_size[0], keyvalue0, command) && (index0 != NOBUFFERADDRESS)){ keyvalue20 = get(enables[3], command, destbuffer[0], 0, index0); }
		
		if(allowOp(k, chunk_size[0], keyvalue0, command)){ update(enables[4], command, &keyvalue20); }
		
		if(allowOp(k, chunk_size[0], keyvalue0, command) && (index0 != NOBUFFERADDRESS)){ set(enables[5], command, destbuffer[0], 0, index0, keyvalue0, keyvalue20); }
		
		if(allowOp(k, chunk_size[0], keyvalue0, command)){ update(enables[6], command, &localcapsule[0][p0]); }
		
		#ifdef _DEBUGMODE_CHECKS2
		if(command == COLLECTGLOBALSTATSCOMMAND){ actsutilityobj->globalvar_extractcapsules_counttotalkvsread(1 * NUMSUBWORKERS); }
		else if(command == COLLECTSTATSCOMMAND){ actsutilityobj->globalvar_collectstats_counttotalkvsread(1 * NUMSUBWORKERS); }
		else if(command == PARTITIONKEYVALUECOMMAND){ actsutilityobj->globalstats_countkvspartitioned(1 * NUMSUBWORKERS); }
		else if(command == APPENDINVALIDSCOMMAND){
			if(k < ovsize0){ actsutilityobj->globalvar_savepartitions_countinvalids(1); }
 }
		else {}
		#endif
	}
	return;
}

void 
	#ifdef SW 
	actspartition::
	#endif 
launchexecute0(unsigned int enable, unsigned int collectglobalstatscmd, unsigned int collectstatsandpartitioncmd, atp_uint512_dt sourcebuffer[NUMSUBWORKERS][SRCBUFFER_SIZE], atp_uint512_dt destbuffer[NUMSUBWORKERS][PADDEDDESTBUFFER_SIZE], keyvalue_t localcapsule[NUMSUBWORKERS][NUM_PARTITIONS], alw_sweepparams_t sweepparams, alw_travstate_t travstate){			
	if(enable == OFF){ return; }
	unsigned int analysis_srcbuffersz = SRCBUFFER_SIZE * VECTOR_SIZE;
	unsigned int enables[8];
	unsigned int command;
	
	buffer_type chunk_size[NUMSUBWORKERS];
	#pragma HLS ARRAY_PARTITION variable=chunk_size complete
	for(unsigned int sw=0; sw<NUMSUBWORKERS; sw++){ chunk_size[sw] = getchunksize(SRCBUFFER_SIZE, travstate, sw * SRCBUFFER_SIZE); }
	
	if(collectglobalstatscmd == ON){ 
		enables[0] = ON; // get
		enables[1] = ON; // compute 
		enables[2] = ON; // get setindex 
		enables[3] = ON; // get
		enables[4] = ON; // update
		enables[5] = ON; // set 
		enables[6] = OFF; // update
		command = COLLECTGLOBALSTATSCOMMAND;
		
		execute0(ON, enables, command, sourcebuffer, destbuffer, localcapsule, chunk_size, sweepparams, 0);
	} else if(collectstatsandpartitioncmd == ON){
		resetmanykeyandvalues(localcapsule, NUM_PARTITIONS); 
		
		enables[0] = ON; // get
		enables[1] = ON; // compute 
		enables[2] = OFF; // get index 
		enables[3] = OFF; // get
		enables[4] = OFF; // update 
		enables[5] = ON; // set 
		enables[6] = ON; // update
		command = COLLECTSTATSCOMMAND;
		execute0(ON, enables, command, sourcebuffer, destbuffer, localcapsule, chunk_size, sweepparams, 0);
		
		calculatemanyoffsets(localcapsule, NUM_PARTITIONS, 0, 0);
		resetmanyvalues(localcapsule, NUM_PARTITIONS);
		
		enables[0] = ON; // get
		enables[1] = ON; // compute
		enables[2] = ON; // get index
		enables[3] = OFF; // get
		enables[4] = OFF; // update
		enables[5] = ON; // set 
		enables[6] = ON; // update
		command = PARTITIONKEYVALUECOMMAND;
		execute0(ON, enables, command, sourcebuffer, destbuffer, localcapsule, chunk_size, sweepparams, 0);
		
		// edge conditions
		keyvalue_t InvalidKV; InvalidKV.key = INVALIDDATA; InvalidKV.value = INVALIDDATA;
		APPENDINVALIDS_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
			chunk_size[0] = 1;
			enables[0] = ON; // get
			enables[1] = ON; // compute
			enables[2] = ON; // get index
			enables[3] = OFF; // get
			enables[4] = OFF; // update
			enables[5] = ON; // set 
			enables[6] = OFF; // update
			command = APPENDINVALIDSCOMMAND;
			execute0(ON, enables, command, sourcebuffer, destbuffer, localcapsule, chunk_size, sweepparams, p);
		}
	} else {}
	return;
}

void 
	#ifdef SW 
	actspartition::
	#endif 
saveKVS0(unsigned int enable, uint512_dt * kvdram, atp_uint512_dt buffer[NUMSUBWORKERS][PADDEDDESTBUFFER_SIZE], keyvalue_t * globalcapsule, keyvalue_t localcapsule[NUMSUBWORKERS][NUM_PARTITIONS], batch_type globalbaseaddress_kvs){				
	if(enable == OFF){ return; }
	unsigned int analysis_destpartitionsz = PADDEDDESTBUFFER_SIZE / NUM_PARTITIONS;
		SAVEPARTITIONS_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
			batch_type dramoffset_kvs = globalbaseaddress_kvs + ((globalcapsule[p].key + globalcapsule[p].value) / VECTOR_SIZE);
			buffer_type bramoffset_kvs = localcapsule[0][p].key / VECTOR_SIZE;
			buffer_type realsize_kvs = (localcapsule[0][p].value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			buffer_type size_kvs = getpartitionwritesz(realsize_kvs, bramoffset_kvs);
			
			SAVEPARTITIONS_LOOP1B: for(buffer_type i=0; i<size_kvs; i++){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_destpartitionsz avg=analysis_destpartitionsz
			#pragma HLS PIPELINE II=1
				kvdram[dramoffset_kvs + i] = buffer[0][bramoffset_kvs + i];
				#ifdef _DEBUGMODE_STATS
				actsutilityobj->globalstats_countkvspartitionswritten_actual(VECTOR_SIZE);
				#endif
			}
			globalcapsule[p].value += allignhigher_KV(localcapsule[0][p].value);
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countkvspartitionswritten(size_kvs * VECTOR_SIZE);
			actsutilityobj->globalvar_inmemory_counttotalvalidkeyvalues(localcapsule[0][p].value);
			#endif
		}
	return;
}

void 
	#ifdef SW 
	actspartition::
	#endif 
dispatch0(uint512_dt * kvdram, atp_uint512_dt sourcebuffer[NUMSUBWORKERS][SRCBUFFER_SIZE], atp_uint512_dt destbuffer[NUMSUBWORKERS][PADDEDDESTBUFFER_SIZE], keyvalue_t localcapsule[NUMSUBWORKERS][NUM_PARTITIONS], keyvalue_t globalcapsule[NUM_PARTITIONS], alw_config config, alw_sweepparams_t sweepparams, alw_travstate_t travstate){
	unsigned int analysis_mainloop = KVDATA_BATCHSIZE_KVS / (NUMSUBWORKERS * SRCBUFFER_SIZE);
	MAIN_LOOP: for(unsigned int offset_kvs=travstate.begin_kvs; offset_kvs<travstate.end_kvs; offset_kvs+=NUMSUBWORKERS * SRCBUFFER_SIZE){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_mainloop avg=analysis_mainloop	
		travstate.i_kvs = offset_kvs;
		
		readKVS0(config.enablereadkvs, kvdram, sourcebuffer, (sweepparams.worksourcebaseaddress_kvs + offset_kvs), travstate);
		
		launchexecute0(config.enabledispatch, config.collectglobalstatscmd, config.collectstatsandpartitioncmd, sourcebuffer, destbuffer, localcapsule, sweepparams, travstate);
		
		saveKVS0(config.enablesavekvs, kvdram, destbuffer, globalcapsule, localcapsule, sweepparams.workdestbaseaddress_kvs);
	}
	return;
}

extern "C" {
void 
	#ifdef SW 
	actspartition:: 
	#endif
topkernel( uint512_dt * sourceAvolume ){
#pragma HLS INTERFACE m_axi port = sourceAvolume offset = slave bundle = gmem0

#pragma HLS INTERFACE s_axilite port = sourceAvolume bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATA_PACK variable = sourceAvolume
	#ifdef _DEBUGMODE_KERNELPRINTS2
	cout<<"Light weight ACTS started."<<endl;
	#endif 

	atp_uint512_dt sourceAbuffer[NUMSUBWORKERS][SRCBUFFER_SIZE];
	atp_uint512_dt destAbuffer[NUMSUBWORKERS][PADDEDDESTBUFFER_SIZE]; // REMOVEME? +1
	keyvalue_t localcapsuleA[NUMSUBWORKERS][NUM_PARTITIONS];
	keyvalue_t globalcapsuleA[NUM_PARTITIONS];
	alw_travstate_t travstateA;
	alw_config config;
	alw_sweepparams_t sweepparams; 
	
	#pragma HLS array_partition variable = sourceAbuffer //block dim=2
	#pragma HLS array_partition variable = destAbuffer //block dim=2
	#pragma HLS array_partition variable = localcapsuleA //block dim=2
	#pragma HLS DATA_PACK variable = sourceAbuffer
	#pragma HLS DATA_PACK variable = destAbuffer
	
	unsigned int analysis_mainloop = KVDATA_BATCHSIZE_KVS / SRCBUFFER_SIZE;
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->printglobalvars();
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
	
	ACTSLW_LOOP1: for(unsigned int currentLOP=0; currentLOP<=2; currentLOP++){ //TREE_DEPTH//+1:reduce
		sweepparams.currentLOP = currentLOP;
		if(currentLOP == 0 || (currentLOP % 2) == 1){ sweepparams.worksourcebaseaddress_kvs = BASEOFFSET_KVDRAM_KVS; sweepparams.workdestbaseaddress_kvs = BASEOFFSET_KVDRAMWORKSPACE_KVS; }
		else { sweepparams.worksourcebaseaddress_kvs = BASEOFFSET_KVDRAMWORKSPACE_KVS; sweepparams.workdestbaseaddress_kvs = BASEOFFSET_KVDRAM_KVS; }

		unsigned int num_source_partitions = (1 << (NUM_PARTITIONS_POW * subandcap(currentLOP, 1)));
		unsigned int sourceskipsize = getskipsize(currentLOP, SOURCE);
		#ifdef _DEBUGMODE_KERNELPRINTS2
		actsutilityobj->print4(">>>>>>> currentLOP", "num_source_partitions", "sourceskipsize", "NAp", currentLOP, num_source_partitions, sourceskipsize, NAp);
		#endif 
		
		if(currentLOP == 0){ config.collectglobalstatscmd = ON; config.collectstatsandpartitioncmd = OFF; config.reducecmd = OFF; config.enablereadkvs = ON; config.enabledispatch = ON; config.enablesavekvs = OFF; config.enableresetdestbuffer = ON; config.enablereadglobalstats = OFF; config.enablesaveglobalstats = ON; } 
		else { config.collectglobalstatscmd = OFF; config.collectstatsandpartitioncmd = ON; config.reducecmd = OFF; config.enablereadkvs = ON; config.enabledispatch = ON; config.enablesavekvs = ON; config.enableresetdestbuffer = OFF; config.enablereadglobalstats = ON; config.enablesaveglobalstats = OFF; }
		
		resetkeyandvalues(config.enableresetdestbuffer, destAbuffer);
		
		readglobalstats0(config.enablereadglobalstats, sourceAvolume, globalcapsuleA, 0, sweepparams.currentLOP, DEST);
		
		ACTSLW_LOOP1B: for(unsigned int source_partition=0; source_partition<num_source_partitions; source_partition+=1){
			sweepparams.upperlimit = globalparamsA.vbegin + (source_partition * sourceskipsize * (BATCH_RANGE / NUMLASTLEVELPARTITIONS));
			
			keyvalue_t keyvalue = getkeyvalue(sourceAvolume, BASEOFFSET_STATSDRAM_KVS, source_partition * sourceskipsize);
			keyvalue_t nextkeyvalue; nextkeyvalue.key = globalparamsA.runsize;
			if(currentLOP >= 2 && source_partition < (num_source_partitions - 1)){ nextkeyvalue = getkeyvalue(sourceAvolume, BASEOFFSET_STATSDRAM_KVS, (source_partition + 1) * sourceskipsize); }

			travstateA.begin_kvs = keyvalue.key / VECTOR_SIZE; 
			travstateA.end_kvs = nextkeyvalue.key / VECTOR_SIZE;
			travstateA.skip_kvs = SRCBUFFER_SIZE;
			#ifdef _DEBUGMODE_KERNELPRINTS2
			actsutilityobj->print4("### source_partition", "sweepparams.upperlimit", "travstateA.begin", "travstateA.end", source_partition, sweepparams.upperlimit, travstateA.begin_kvs * VECTOR_SIZE, travstateA.end_kvs * VECTOR_SIZE);
			#endif 
			
			dispatch0(sourceAvolume, sourceAbuffer, destAbuffer, localcapsuleA, globalcapsuleA, config, sweepparams, travstateA);
		}
		
		saveglobalstats0(config.enablesaveglobalstats, sourceAvolume, destAbuffer);
 
		
		prepareglobalstats0(config.enablesaveglobalstats, sourceAvolume);
		
		#ifdef _DEBUGMODE_KERNELPRINTS2
		actsutilityobj->printglobalvars();
		actsutilityobj->clearglobalvars();
		actsutilityobj->printkeyvalues("actspartition::head::globalcapsule", (keyvalue_t *)globalcapsuleA, NUM_PARTITIONS);
		if(currentLOP==1){ actsutilityobj->scankeyvalues((keyvalue_t *)(&sourceAvolume[sweepparams.workdestbaseaddress_kvs]), globalcapsuleA, (1 << (NUM_PARTITIONS_POW * currentLOP)), BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * 1))); }
		#endif 
	}
	return;
}
}








