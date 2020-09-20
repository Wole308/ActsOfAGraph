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
#include "actslw.h"
using namespace std;

#ifdef SW
actslw::actslw(){ actsutilityobj = new actsutility(); }
actslw::~actslw(){}
#endif
#ifdef SWEMU
actsutility * actsutilityobj = new actsutility();
#endif

unsigned int 
	#ifdef SW 
	actslw::
	#endif 
subandcap(unsigned int val1, unsigned int val2){
	if(val1 < val2){ return 0; }
	else { return val1 - val2; }
}
unsigned int 
	#ifdef SW 
	actslw::
	#endif 
lowercapto(unsigned int val, unsigned int capval){
	if(val < capval){ return capval; }
	else { return val; }
}
unsigned int 
	#ifdef SW 
	actslw::
	#endif 
allignhigher_KV(unsigned int val){
	unsigned int fac = (val + (VECTOR1024_SIZE - 1)) / VECTOR1024_SIZE;
	return (fac * VECTOR1024_SIZE);
}
unsigned int 
	#ifdef SW 
	actslw::
	#endif 
allignby2higher_KV(unsigned int val){
	unsigned int fac = (val + (2 - 1)) / 2;
	return (fac * 2);
}
batch_type
	#ifdef SW 
	actslw::
	#endif
getskipsize(unsigned int currentLOP, unsigned int sourceORdest){
	unsigned int analysis_treedepth = TREE_DEPTH;
	unsigned int result;
	if(sourceORdest == SOURCE){ result = NUMLASTLEVELPARTITIONS; }
	else if (sourceORdest == DEST){ result = NUMLASTLEVELPARTITIONS / NUM_PARTITIONS; }
	else {}
	for(unsigned int i=0; i<(currentLOP-1); i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_treedepth avg=analysis_treedepth
		result = result / NUM_PARTITIONS;
	}
	return result;
}
void 
	#ifdef SW 
	actslw::
	#endif 
copykeyandvalues(keyvalue_t * buffer1, keyvalue_t * buffer2, unsigned int size){
	for(partition_type i=0; i<size; i++){ buffer1[i] = buffer2[i]; }
}
void 
	#ifdef SW 
	actslw::
	#endif 
resetkeyandvalues(keyvalue_t * buffer, unsigned int size){
	for(unsigned int i=0; i<size; i++){ buffer[i].key = 0; buffer[i].value = 0; }
}
void 
	#ifdef SW 
	actslw::
	#endif 
resetvalues(keyvalue_t * buffer, unsigned int size){
	for(unsigned int i=0; i<size; i++){ buffer[i].value = 0; }
}
void 
	#ifdef SW 
	actslw::
	#endif 
resetkeyandvalues(unsigned int enable, uint512_dt destbuffer[PADDEDDESTBUFFER_SIZE]){
	if(enable == OFF){ return; }
	uint512_dt ZeroKVS;
	keyvalue_t ZeroKV; ZeroKV.key = 0; ZeroKV.value = 0;
	#ifdef _WIDEWORD
	ZeroKVS = 0;
	#else 
	for(vector_type v=0; v<VECTOR1024_SIZE; v++){ ZeroKVS.data[v] = ZeroKV; }
	#endif 
	for(partition_type i=0; i<PADDEDDESTBUFFER_SIZE; i++){ 
		destbuffer[i] = ZeroKVS;
	}
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif 
resetmanykeyandvalues(keyvalue_t buffer[16][NUM_PARTITIONS], unsigned int size){
	for(unsigned int i=0; i<size; i++){
		buffer[0][i].key = 0; buffer[0][i].value = 0; 
		buffer[1][i].key = 0; buffer[1][i].value = 0; 
		buffer[2][i].key = 0; buffer[2][i].value = 0; 
		buffer[3][i].key = 0; buffer[3][i].value = 0; 
		buffer[4][i].key = 0; buffer[4][i].value = 0; 
		buffer[5][i].key = 0; buffer[5][i].value = 0; 
		buffer[6][i].key = 0; buffer[6][i].value = 0; 
		buffer[7][i].key = 0; buffer[7][i].value = 0; 
		buffer[8][i].key = 0; buffer[8][i].value = 0; 
		buffer[9][i].key = 0; buffer[9][i].value = 0; 
		buffer[10][i].key = 0; buffer[10][i].value = 0; 
		buffer[11][i].key = 0; buffer[11][i].value = 0; 
		buffer[12][i].key = 0; buffer[12][i].value = 0; 
		buffer[13][i].key = 0; buffer[13][i].value = 0; 
		buffer[14][i].key = 0; buffer[14][i].value = 0; 
		buffer[15][i].key = 0; buffer[15][i].value = 0; 
	}
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif 
resetmanykeyandvalues(keyvalue_t buffer[16][PADDEDDESTBUFFER_SIZE], unsigned int size){
	for(unsigned int i=0; i<size; i++){
		buffer[0][i].key = 0; buffer[0][i].value = 0; 
		buffer[1][i].key = 0; buffer[1][i].value = 0; 
		buffer[2][i].key = 0; buffer[2][i].value = 0; 
		buffer[3][i].key = 0; buffer[3][i].value = 0; 
		buffer[4][i].key = 0; buffer[4][i].value = 0; 
		buffer[5][i].key = 0; buffer[5][i].value = 0; 
		buffer[6][i].key = 0; buffer[6][i].value = 0; 
		buffer[7][i].key = 0; buffer[7][i].value = 0; 
		buffer[8][i].key = 0; buffer[8][i].value = 0; 
		buffer[9][i].key = 0; buffer[9][i].value = 0; 
		buffer[10][i].key = 0; buffer[10][i].value = 0; 
		buffer[11][i].key = 0; buffer[11][i].value = 0; 
		buffer[12][i].key = 0; buffer[12][i].value = 0; 
		buffer[13][i].key = 0; buffer[13][i].value = 0; 
		buffer[14][i].key = 0; buffer[14][i].value = 0; 
		buffer[15][i].key = 0; buffer[15][i].value = 0; 
	}
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif 
resetmanyvalues(keyvalue_t buffer[16][NUM_PARTITIONS], unsigned int size){
	for(unsigned int i=0; i<size; i++){ 
		buffer[0][i].value = 0;
		buffer[1][i].value = 0;
		buffer[2][i].value = 0;
		buffer[3][i].value = 0;
		buffer[4][i].value = 0;
		buffer[5][i].value = 0;
		buffer[6][i].value = 0;
		buffer[7][i].value = 0;
		buffer[8][i].value = 0;
		buffer[9][i].value = 0;
		buffer[10][i].value = 0;
		buffer[11][i].value = 0;
		buffer[12][i].value = 0;
		buffer[13][i].value = 0;
		buffer[14][i].value = 0;
		buffer[15][i].value = 0;
	}
	return;
}
void 
	#ifdef SW 
	actslw::
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
void 
	#ifdef SW 
	actslw::
	#endif
accumkeysandvalues(keyvalue_t * buffer1, keyvalue_t * buffer2, keyvalue_t * buffer3, unsigned int size){
	for(partition_type i=0; i<size; i++){
	#pragma HLS PIPELINE II=1
		keyvalue_t keyvalue1 = buffer1[i];
		keyvalue_t keyvalue2 = buffer2[i];
		keyvalue_t keyvalue3 = buffer3[i];
		keyvalue3.key = keyvalue1.key + keyvalue2.key;
		keyvalue3.value = keyvalue1.value + keyvalue2.value;
		buffer3[i] = keyvalue3;
	}
	return;
}
unsigned int 
	#ifdef SW 
	actslw::
	#endif 
checkandforce(unsigned int val, unsigned int limit){ if(val >= limit){ return 0; } else { return val; }}
buffer_type 
	#ifdef SW 
	actslw::
	#endif 
getchunksize(buffer_type buffer_size, travstate_t travstate, unsigned int localoffset){
	buffer_type chunk_size = buffer_size;
	batch_type i = travstate.i_kvs + localoffset;
	if (i > travstate.end_kvs){ chunk_size = 0; }
	else if ((i + buffer_size) > travstate.end_kvs){ chunk_size = travstate.end_kvs - i; }
	else {}
	return chunk_size;
}
unsigned int 
	#ifdef SW 
	actslw::
	#endif 
getpartition(keyvalue_t keyvalue, unsigned int currentLOP, vertex_t upperlimit){
	partition_type partition = ((keyvalue.key - upperlimit) >> (BATCH_RANGE_POW - (NUM_PARTITIONS_POW * currentLOP)));
	
	#ifdef _DEBUGMODE_CHECKS2
	if(partition >= NUM_PARTITIONS){ actsutilityobj->globalstats_counterrorsingetpartition(1); }
	#endif 
	
	#ifdef ENABLE_PERFECTACCURACY
		#ifdef _DEBUGMODE_CHECKS2
		if(partition >= NUM_PARTITIONS){ cout<<"getpartition::ERROR 1. partition out of bounds partition: "<<partition<<", keyvalue.key: "<<keyvalue.key<<", NUM_PARTITIONS: "<<NUM_PARTITIONS<<", keyvalue.key: "<<keyvalue.key<<", upperlimit: "<<upperlimit<<", currentLOP: "<<currentLOP<<endl; exit(EXIT_FAILURE); }
		#endif
	#endif 
	#ifndef ENABLE_PERFECTACCURACY
		// partition = checkandforce(partition, NUM_PARTITIONS); 
		if(partition >= NUM_PARTITIONS){ partition = (((1 << NUM_PARTITIONS_POW) - 1) & (partition >> (1 - 1))); } // FIXME. REMOVEME. PERFECTIONTEST.
	#endif
	
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("actslw::getpartition 2", partition, NUM_PARTITIONS, keyvalue.key, upperlimit, currentLOP);
	#endif
	return partition;
}
unsigned int 
	#ifdef SW 
	actslw::
	#endif 
getglobalpartition(keyvalue_t keyvalue, vertex_t upperlimit){
	partition_type globalpartition = ((keyvalue.key - upperlimit) >> (BATCH_RANGE_POW - (NUM_PARTITIONS_POW * TREE_DEPTH)));
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("actslw::getglobalpartition", globalpartition, NUMLASTLEVELPARTITIONS, keyvalue.key, upperlimit, NAp);
	#endif
	return globalpartition;
}
unsigned int 
	#ifdef SW 
	actslw::
	#endif 
reducefunc(keyy_t vid, value_t value, value_t edgeval, unsigned int GraphIter, unsigned int GraphAlgo){
	#pragma HLS INLINE
	value_t ret = 0;
	#ifdef PR_ALGORITHM
	ret = value + edgeval;
	#elif defined(BFS_ALGORITHM)
	ret = min(value, GraphIter);
	#elif defined(BC_ALGORITHM)
	ret = min(value, GraphIter);
	#endif
	/** if(GraphAlgo == PAGERANK){ ret = value + edgeval; }
	else if(GraphAlgo == BREADTHFIRSTSEARCH){ ret = min(value, GraphIter); }
	else if(GraphAlgo == BETWEENNESSCENTRALITY){ ret = min(value, GraphIter); }
	else {} */
	return ret;
}
void 
	#ifdef SW 
	actslw::
	#endif 
copykeyvalues(keyvalue_t * buffer1, keyvalue_t * buffer2, unsigned int size){
	COPYKEYS_LOOP: for(buffer_type i=0; i<size; i++){ buffer1[i] = buffer2[i]; }
}
buffer_type 
	#ifdef SW 
	actslw::
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
	actslw::
	#endif 
withinvalidrange(unsigned int val1, unsigned int val2){
	#pragma HLS INLINE
	if(val1 < val2){ return 1; }
	else { return 0; }
}
void 
	#ifdef SW 
	actslw::
	#endif 
calculateoffsets(keyvalue_t * buffer, unsigned int size, unsigned int base, unsigned int skipspacing){
	for(partition_type i=1; i<size; i++){ 
		buffer[i].key = allignhigher_KV(buffer[i-1].key + buffer[i-1].value + skipspacing); 
	}
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif 
calculateoffsets_allignby2(keyvalue_t * buffer, unsigned int size, unsigned int base, unsigned int skipspacing){
	for(partition_type i=1; i<size; i++){ 
		buffer[i].key = allignby2higher_KV(buffer[i-1].key + buffer[i-1].value + skipspacing); 
	}
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif 
calculateunallignedoffsets(keyvalue_t buffer[NUM_PARTITIONS], unsigned int size, unsigned int base, unsigned int skipspacing){
	for(partition_type i=1; i<size; i++){ 
		buffer[i].key = buffer[i-1].key + buffer[i-1].value + skipspacing; 
	}
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif 
calculatemanyoffsets(keyvalue_t buffer[16][NUM_PARTITIONS], unsigned int size, unsigned int base, unsigned int skipspacing){
	for(partition_type i=1; i<size; i++){ 
		buffer[0][i].key = allignhigher_KV(buffer[0][i-1].key + buffer[0][i-1].value + skipspacing); 
		buffer[1][i].key = allignhigher_KV(buffer[1][i-1].key + buffer[1][i-1].value + skipspacing); 
		buffer[2][i].key = allignhigher_KV(buffer[2][i-1].key + buffer[2][i-1].value + skipspacing); 
		buffer[3][i].key = allignhigher_KV(buffer[3][i-1].key + buffer[3][i-1].value + skipspacing); 
		buffer[4][i].key = allignhigher_KV(buffer[4][i-1].key + buffer[4][i-1].value + skipspacing); 
		buffer[5][i].key = allignhigher_KV(buffer[5][i-1].key + buffer[5][i-1].value + skipspacing); 
		buffer[6][i].key = allignhigher_KV(buffer[6][i-1].key + buffer[6][i-1].value + skipspacing); 
		buffer[7][i].key = allignhigher_KV(buffer[7][i-1].key + buffer[7][i-1].value + skipspacing); 
		buffer[8][i].key = allignhigher_KV(buffer[8][i-1].key + buffer[8][i-1].value + skipspacing); 
		buffer[9][i].key = allignhigher_KV(buffer[9][i-1].key + buffer[9][i-1].value + skipspacing); 
		buffer[10][i].key = allignhigher_KV(buffer[10][i-1].key + buffer[10][i-1].value + skipspacing); 
		buffer[11][i].key = allignhigher_KV(buffer[11][i-1].key + buffer[11][i-1].value + skipspacing); 
		buffer[12][i].key = allignhigher_KV(buffer[12][i-1].key + buffer[12][i-1].value + skipspacing); 
		buffer[13][i].key = allignhigher_KV(buffer[13][i-1].key + buffer[13][i-1].value + skipspacing); 
		buffer[14][i].key = allignhigher_KV(buffer[14][i-1].key + buffer[14][i-1].value + skipspacing); 
		buffer[15][i].key = allignhigher_KV(buffer[15][i-1].key + buffer[15][i-1].value + skipspacing); 
	}
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif 
calculatemanyunallignedoffsets(keyvalue_t buffer[16][NUM_PARTITIONS], unsigned int size, unsigned int base, unsigned int skipspacing){
	for(partition_type i=1; i<size; i++){ 
		buffer[0][i].key = buffer[0][i-1].key + buffer[0][i-1].value + skipspacing; 
		buffer[1][i].key = buffer[1][i-1].key + buffer[1][i-1].value + skipspacing; 
		buffer[2][i].key = buffer[2][i-1].key + buffer[2][i-1].value + skipspacing; 
		buffer[3][i].key = buffer[3][i-1].key + buffer[3][i-1].value + skipspacing; 
		buffer[4][i].key = buffer[4][i-1].key + buffer[4][i-1].value + skipspacing; 
		buffer[5][i].key = buffer[5][i-1].key + buffer[5][i-1].value + skipspacing; 
		buffer[6][i].key = buffer[6][i-1].key + buffer[6][i-1].value + skipspacing; 
		buffer[7][i].key = buffer[7][i-1].key + buffer[7][i-1].value + skipspacing; 
		buffer[8][i].key = buffer[8][i-1].key + buffer[8][i-1].value + skipspacing; 
		buffer[9][i].key = buffer[9][i-1].key + buffer[9][i-1].value + skipspacing; 
		buffer[10][i].key = buffer[10][i-1].key + buffer[10][i-1].value + skipspacing; 
		buffer[11][i].key = buffer[11][i-1].key + buffer[11][i-1].value + skipspacing; 
		buffer[12][i].key = buffer[12][i-1].key + buffer[12][i-1].value + skipspacing; 
		buffer[13][i].key = buffer[13][i-1].key + buffer[13][i-1].value + skipspacing; 
		buffer[14][i].key = buffer[14][i-1].key + buffer[14][i-1].value + skipspacing; 
		buffer[15][i].key = buffer[15][i-1].key + buffer[15][i-1].value + skipspacing; 
	}
	return;
}
unsigned int  
	#ifdef SW 
	actslw::
	#endif 
get_num_source_partitions(unsigned int currentLOP){
	#pragma HLS INLINE
	unsigned int treedepth = TREE_DEPTH;
	
	if(currentLOP == 0){ currentLOP = 1; }
	unsigned int pow = 1;
	for(unsigned int i=0; i<(currentLOP-1); i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=treedepth avg=treedepth
		pow = pow * NUM_PARTITIONS;
	}
	return pow;
}
globalparams_t 
	#ifdef SW 
	actslw::
	#endif 
getglobalparams(uint512_dt * sourcevolume1, uint512_dt * sourcevolume2){
	globalparams_t globalparams;
	#ifdef _WIDEWORD
	globalparams.runkernelcommand = sourcevolume1[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNKERNELCOMMANDID].range(31, 0);
	globalparams.processcommand = sourcevolume1[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_PROCESSCOMMANDID].range(31, 0);
	globalparams.collectstatscommand = sourcevolume1[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_COLLECTSTATSCOMMANDID].range(31, 0);
	globalparams.partitioncommand = sourcevolume1[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_PARTITIONCOMMANDID].range(31, 0);
	globalparams.reducecommand = sourcevolume1[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_APPLYUPDATESCOMMANDID].range(31, 0);
	globalparams.vbegin = sourcevolume1[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_VOFFSET].range(31, 0); 
	globalparams.vsize = sourcevolume1[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_VSIZE].range(31, 0); 
	globalparams.treedepth = sourcevolume1[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_TREEDEPTH].range(31, 0); 
	globalparams.LLOPnumpartitions = sourcevolume1[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_FINALNUMPARTITIONS].range(31, 0);
	globalparams.GraphIter = sourcevolume1[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_GRAPHITERATIONID].range(31, 0);
	globalparams.GraphAlgo = sourcevolume1[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_GRAPHALGORITHMID].range(31, 0);
	globalparams.batchsize = sourcevolume1[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BATCHSIZE].range(31, 0);
	globalparams.runsize = sourcevolume1[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].range(31, 0);
	globalparams.nextbatchoffset = sourcevolume1[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NEXTBATCHOFFSET].range(31, 0);
	globalparams.statsalreadycollected = NAp;
	globalparams.baseaddr_destkvs_kvs = 0;
	#else 
	globalparams.runkernelcommand = sourcevolume1[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNKERNELCOMMANDID].data[0].key;
	globalparams.processcommand = sourcevolume1[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_PROCESSCOMMANDID].data[0].key;
	globalparams.collectstatscommand = sourcevolume1[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_COLLECTSTATSCOMMANDID].data[0].key;
	globalparams.partitioncommand = sourcevolume1[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_PARTITIONCOMMANDID].data[0].key;
	globalparams.reducecommand = sourcevolume1[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_APPLYUPDATESCOMMANDID].data[0].key;
	globalparams.vbegin = sourcevolume1[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_VOFFSET].data[0].key; 
	globalparams.vsize = sourcevolume1[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_VSIZE].data[0].key; 
	globalparams.treedepth = sourcevolume1[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_TREEDEPTH].data[0].key; 
	globalparams.LLOPnumpartitions = sourcevolume1[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_FINALNUMPARTITIONS].data[0].key;
	globalparams.GraphIter = sourcevolume1[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_GRAPHITERATIONID].data[0].key;
	globalparams.GraphAlgo = sourcevolume1[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_GRAPHALGORITHMID].data[0].key;
	globalparams.batchsize = sourcevolume1[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BATCHSIZE].data[0].key;
	globalparams.runsize = sourcevolume1[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key;
	globalparams.nextbatchoffset = sourcevolume1[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NEXTBATCHOFFSET].data[0].key;
	globalparams.statsalreadycollected = NAp;
	globalparams.baseaddr_destkvs_kvs = 0;
	#endif 
	globalparams.runsize = globalparams.runsize * 2; //
	
	// globalparams.runsize = 11000000;
	
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->printglobalparameters("actslw::getglobalparams:: printing global parameters", globalparams);
	#endif
	return globalparams;
}
sweepparams_t 
	#ifdef SW 
	actslw::
	#endif 
getsweepparams(globalparams_t globalparams, unsigned int currentLOP, unsigned int source_partition){
	sweepparams_t sweepparams;
	unsigned int sourceskipsize = getskipsize(currentLOP, SOURCE);
	
	sweepparams.currentLOP = currentLOP;
	if(currentLOP == 0 || (currentLOP % 2) == 1){ sweepparams.worksourcebaseaddress_kvs = BASEOFFSET_KVDRAM_KVS; sweepparams.workdestbaseaddress_kvs = BASEOFFSET_KVDRAMWORKSPACE_KVS; }
	else { sweepparams.worksourcebaseaddress_kvs = BASEOFFSET_KVDRAMWORKSPACE_KVS; sweepparams.workdestbaseaddress_kvs = BASEOFFSET_KVDRAM_KVS; }
	
	sweepparams.upperlimit = globalparams.vbegin + (source_partition * sourceskipsize * (BATCH_RANGE / NUMLASTLEVELPARTITIONS));
	sweepparams.source_partition = source_partition;
	return sweepparams;
}
config_t 
	#ifdef SW 
	actslw::
	#endif 
getconfig(unsigned int currentLOP){
	config_t config;
	
	if(currentLOP == 0){ config.enablecollectglobalstats = ON; config.enablepartition = OFF; config.enablereduce = OFF; } 
	else if(currentLOP > 0 && currentLOP <= TREE_DEPTH){ config.enablecollectglobalstats = OFF; config.enablepartition = ON; config.enablereduce = OFF; }
	else { config.enablecollectglobalstats = OFF; config.enablepartition = OFF; config.enablereduce = ON; }
	return config;
}
travstate_t 
	#ifdef SW 
	actslw::
	#endif 
gettravstate(uint512_dt * kvdram1, uint512_dt * kvdram2, keyvalue_t globalstatsbuffer[PADDEDDESTBUFFER_SIZE], globalparams_t globalparams, config_t config, unsigned int currentLOP, unsigned int source_partition, unsigned int num_source_partitions){
	travstate_t travstate;
	unsigned int sourceskipsize = getskipsize(currentLOP, SOURCE);
	
	keyvalue_t keyvalue;
	if(currentLOP < 2){ keyvalue.key = 0; }
	else { 
		#ifdef USEBRAMFORGLOBALSTATS
		keyvalue = globalstatsbuffer[source_partition * sourceskipsize]; 
		#else 
		keyvalue = kvdram[BASEOFFSET_STATSDRAM_KVS + (source_partition * sourceskipsize)].data[0]; 
		#endif 
	}
	
	keyvalue_t nextkeyvalue;
	nextkeyvalue.key = globalparams.runsize;
	if(currentLOP >= 2){
		nextkeyvalue.key = keyvalue.key + keyvalue.value;
	}
	
	travstate.begin_kvs = keyvalue.key / VECTOR1024_SIZE; 
	travstate.end_kvs = nextkeyvalue.key / VECTOR1024_SIZE;
	
	if(config.enablecollectglobalstats == ON || config.enablereduce == ON){ travstate.skip_kvs = SRCBUFFER_SIZE; } 
	else{ travstate.skip_kvs = NUMPIPELINES * SRCBUFFER_SIZE; }
	travstate.i_kvs = travstate.begin_kvs;
	return travstate;	
}

void 
	#ifdef SW 
	actslw::
	#endif 
readglobalstats0(unsigned int enable, uint512_dt * kvdram1, uint512_dt * kvdram2, keyvalue_t globalstatsbuffer[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer[NUM_PARTITIONS], unsigned int offset_kvs, unsigned int currentLOP, unsigned int sourceORdest){
	if(enable == OFF){ return; }
	
	unsigned int skipsize = getskipsize(currentLOP, sourceORdest);
	unsigned int loffset_kvs = 0;
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("readglobalstats0", BASEOFFSET_STATSDRAM_KVS + offset_kvs + NUM_PARTITIONS, PADDEDKVSOURCEDRAMSZ_KVS + 1, NAp, NAp, NAp);
	#endif
	READGLOBALSTATS_LOOP: for (buffer_type i=0; i<NUM_PARTITIONS; i++){
	#pragma HLS PIPELINE II=1
		#ifdef USEBRAMFORGLOBALSTATS
		buffer[i] = globalstatsbuffer[offset_kvs + loffset_kvs]; 
		#else 
		buffer[i] = kvdram1[BASEOFFSET_STATSDRAM_KVS + offset_kvs + loffset_kvs].data[0]; FIXME. // FIXME
		#endif 
		loffset_kvs += skipsize;
	}
	resetvalues(buffer, NUM_PARTITIONS);
	return;
}

void 
	#ifdef SW 
	actslw::
	#endif 
saveglobalstats0(unsigned int enable, uint512_dt * kvdram1, uint512_dt * kvdram2, keyvalue_t globalstatsbuffer[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer[16][PADDEDDESTBUFFER_SIZE]){
	if(enable == OFF){ return; }
	
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("saveglobalstats0", BASEOFFSET_STATSDRAM_KVS + PADDEDDESTBUFFER_SIZE, PADDEDKVSOURCEDRAMSZ_KVS + 1, NAp, NAp, NAp);
	#endif
	SAVEGLOBALSTATS_LOOP: for (buffer_type i=0; i<PADDEDDESTBUFFER_SIZE; i++){
	#pragma HLS PIPELINE II=1
		#ifdef USEBRAMFORGLOBALSTATS
		globalstatsbuffer[i] = buffer[0][i];
		#else 
		kvdram1[BASEOFFSET_STATSDRAM_KVS + i].data[0] = buffer[0][i]; 
		kvdram1[BASEOFFSET_STATSDRAM_KVS + i].data[1] = buffer[1][i]; 
		kvdram1[BASEOFFSET_STATSDRAM_KVS + i].data[2] = buffer[2][i]; 
		kvdram1[BASEOFFSET_STATSDRAM_KVS + i].data[3] = buffer[3][i]; 
		kvdram1[BASEOFFSET_STATSDRAM_KVS + i].data[4] = buffer[4][i]; 
		kvdram1[BASEOFFSET_STATSDRAM_KVS + i].data[5] = buffer[5][i]; 
		kvdram1[BASEOFFSET_STATSDRAM_KVS + i].data[6] = buffer[6][i]; 
		kvdram1[BASEOFFSET_STATSDRAM_KVS + i].data[7] = buffer[7][i]; 
		kvdram2[BASEOFFSET_STATSDRAM_KVS + i].data[0] = buffer[8][i]; 
		kvdram2[BASEOFFSET_STATSDRAM_KVS + i].data[1] = buffer[9][i]; 
		kvdram2[BASEOFFSET_STATSDRAM_KVS + i].data[2] = buffer[10][i]; 
		kvdram2[BASEOFFSET_STATSDRAM_KVS + i].data[3] = buffer[11][i]; 
		kvdram2[BASEOFFSET_STATSDRAM_KVS + i].data[4] = buffer[12][i]; 
		kvdram2[BASEOFFSET_STATSDRAM_KVS + i].data[5] = buffer[13][i]; 
		kvdram2[BASEOFFSET_STATSDRAM_KVS + i].data[6] = buffer[14][i]; 
		kvdram2[BASEOFFSET_STATSDRAM_KVS + i].data[7] = buffer[15][i]; 
		#endif 
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalvar_savestats_counttotalstatswritten(VECTOR1024_SIZE);
		#endif
	}
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif 
updateglobalstats0(unsigned int enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[NUMLASTLEVELPARTITIONS], keyvalue_t buffer[NUM_PARTITIONS], unsigned int offset_kvs, unsigned int currentLOP, unsigned int sourceORdest){
	if(enable == OFF){ return; }
	
	unsigned int skipsize = getskipsize(currentLOP, sourceORdest);
	unsigned int loffset_kvs = 0;
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("updateglobalstats0", BASEOFFSET_STATSDRAM_KVS + offset_kvs + NUM_PARTITIONS, PADDEDKVSOURCEDRAMSZ_KVS + 1, NAp, NAp, NAp);
	#endif
	READGLOBALSTATS_LOOP: for (buffer_type i=0; i<NUM_PARTITIONS; i++){
	#pragma HLS PIPELINE II=1
		#ifdef USEBRAMFORGLOBALSTATS
		globalstatsbuffer[offset_kvs + loffset_kvs].value = buffer[i].value; 
		#else 
		kvdram[BASEOFFSET_STATSDRAM_KVS + offset_kvs + loffset_kvs].data[0].value = buffer[i].value; 
		#endif 
		loffset_kvs += skipsize;
	}
	return;
}

void 
	#ifdef SW 
	actslw::
	#endif 
readkeyvalues0(unsigned int enable, uint512_dt * kvdram1, uint512_dt * kvdram2, keyvalue_t buffer[16][SRCBUFFER_SIZE], batch_type offset_kvs, travstate_t travstate){
	if(enable == OFF){ return; }
	unsigned int analysis_srcbuffersz = SRCBUFFER_SIZE;
	buffer_type chunk_size = getchunksize(SRCBUFFER_SIZE, travstate, 0);
	
	READKVS_LOOP: for (buffer_type i=0; i<chunk_size; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
	#pragma HLS PIPELINE II=1
		buffer[0][i] = kvdram1[offset_kvs + i].data[0];
		buffer[1][i] = kvdram1[offset_kvs + i].data[1];
		buffer[2][i] = kvdram1[offset_kvs + i].data[2];
		buffer[3][i] = kvdram1[offset_kvs + i].data[3];
		buffer[4][i] = kvdram1[offset_kvs + i].data[4];
		buffer[5][i] = kvdram1[offset_kvs + i].data[5];
		buffer[6][i] = kvdram1[offset_kvs + i].data[6];
		buffer[7][i] = kvdram1[offset_kvs + i].data[7];
		buffer[8][i] = kvdram2[offset_kvs + i].data[0]; 
		buffer[9][i] = kvdram2[offset_kvs + i].data[1]; 
		buffer[10][i] = kvdram2[offset_kvs + i].data[2]; 
		buffer[11][i] = kvdram2[offset_kvs + i].data[3]; 
		buffer[12][i] = kvdram2[offset_kvs + i].data[4]; 
		buffer[13][i] = kvdram2[offset_kvs + i].data[5]; 
		buffer[14][i] = kvdram2[offset_kvs + i].data[6]; 
		buffer[15][i] = kvdram2[offset_kvs + i].data[7]; 
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR1024_SIZE);
		#endif
	}
	return;
}

void 
	#ifdef SW 
	actslw::
	#endif 
readvertices0(unsigned int enable, uint512_dt * kvdram1, uint512_dt * kvdram2, keyvalue_t buffer[16][PADDEDDESTBUFFER_SIZE], batch_type offset_kvs){
	if(enable == OFF){ return; }
		
	READVERTICES_LOOP: for (buffer_type i=0; i<PADDEDDESTBUFFER_SIZE; i++){
	#pragma HLS PIPELINE II=1
		buffer[0][i] = kvdram1[offset_kvs + i].data[0]; 
		buffer[1][i] = kvdram1[offset_kvs + i].data[1]; 
		buffer[2][i] = kvdram1[offset_kvs + i].data[2]; 
		buffer[3][i] = kvdram1[offset_kvs + i].data[3]; 
		buffer[4][i] = kvdram1[offset_kvs + i].data[4]; 
		buffer[5][i] = kvdram1[offset_kvs + i].data[5]; 
		buffer[6][i] = kvdram1[offset_kvs + i].data[6]; 
		buffer[7][i] = kvdram1[offset_kvs + i].data[7]; 
		buffer[8][i] = kvdram2[offset_kvs + i].data[0]; 
		buffer[9][i] = kvdram2[offset_kvs + i].data[1]; 
		buffer[10][i] = kvdram2[offset_kvs + i].data[2]; 
		buffer[11][i] = kvdram2[offset_kvs + i].data[3]; 
		buffer[12][i] = kvdram2[offset_kvs + i].data[4]; 
		buffer[13][i] = kvdram2[offset_kvs + i].data[5]; 
		buffer[14][i] = kvdram2[offset_kvs + i].data[6]; 
		buffer[15][i] = kvdram2[offset_kvs + i].data[7]; 
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR1024_SIZE);
		#endif
	}
	return;
}

void 
	#ifdef SW 
	actslw::
	#endif 
savekeyvalues0(unsigned int enable, uint512_dt * kvdram1, uint512_dt * kvdram2, uint1024_dt buffer[PADDEDDESTBUFFER_SIZE], keyvalue_t * globalcapsule, keyvalue_t localcapsule[NUM_PARTITIONS], batch_type globalbaseaddress_kvs){				
	if(enable == OFF){ return; }
	
	#ifdef _DEBUGMODE_CHECKS
	actsutilityobj->printkeyvalues("savekeyvalues0::localcapsule", localcapsule, NUM_PARTITIONS);
	actsutilityobj->printvaluecount("savekeyvalues0::localcapsule", localcapsule, NUM_PARTITIONS);
	actsutilityobj->scankeyvalues("savekeyvalues0::buffer", (keyvalue_t *)buffer, localcapsule, NUM_PARTITIONS, BATCH_RANGE / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif 

	unsigned int analysis_destpartitionsz = PADDEDDESTBUFFER_SIZE / NUM_PARTITIONS;
	SAVEPARTITIONS_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		batch_type dramoffset_kvs = globalbaseaddress_kvs + ((globalcapsule[p].key + globalcapsule[p].value) / VECTOR1024_SIZE);
		buffer_type bramoffset_kvs = localcapsule[p].key / VECTOR1024_SIZE;
		buffer_type realsize_kvs = localcapsule[p].value / VECTOR1024_SIZE;
		buffer_type size_kvs = getpartitionwritesz(realsize_kvs, bramoffset_kvs);
		
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("savekeyvalues0 23", bramoffset_kvs + size_kvs, PADDEDDESTBUFFER_SIZE + 1, p, bramoffset_kvs, size_kvs);
		#endif
		SAVEPARTITIONS_LOOP1B: for(buffer_type i=0; i<size_kvs; i++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_destpartitionsz avg=analysis_destpartitionsz
		#pragma HLS PIPELINE II=1
			kvdram1[dramoffset_kvs + i].data[0] = buffer[bramoffset_kvs + i].data[0]; // FIXME.
			kvdram1[dramoffset_kvs + i].data[1] = buffer[bramoffset_kvs + i].data[1]; // FIXME.
			kvdram1[dramoffset_kvs + i].data[2] = buffer[bramoffset_kvs + i].data[2]; // FIXME.
			kvdram1[dramoffset_kvs + i].data[3] = buffer[bramoffset_kvs + i].data[3]; // FIXME.
			kvdram1[dramoffset_kvs + i].data[4] = buffer[bramoffset_kvs + i].data[4]; // FIXME.
			kvdram1[dramoffset_kvs + i].data[5] = buffer[bramoffset_kvs + i].data[5]; // FIXME.
			kvdram1[dramoffset_kvs + i].data[6] = buffer[bramoffset_kvs + i].data[6]; // FIXME.
			kvdram1[dramoffset_kvs + i].data[7] = buffer[bramoffset_kvs + i].data[7]; // FIXME.
			kvdram2[dramoffset_kvs + i].data[0] = buffer[bramoffset_kvs + i].data[8]; // FIXME.
			kvdram2[dramoffset_kvs + i].data[1] = buffer[bramoffset_kvs + i].data[9]; // FIXME.
			kvdram2[dramoffset_kvs + i].data[2] = buffer[bramoffset_kvs + i].data[10]; // FIXME.
			kvdram2[dramoffset_kvs + i].data[3] = buffer[bramoffset_kvs + i].data[11]; // FIXME.
			kvdram2[dramoffset_kvs + i].data[4] = buffer[bramoffset_kvs + i].data[12]; // FIXME.
			kvdram2[dramoffset_kvs + i].data[5] = buffer[bramoffset_kvs + i].data[13]; // FIXME.
			kvdram2[dramoffset_kvs + i].data[6] = buffer[bramoffset_kvs + i].data[14]; // FIXME.
			kvdram2[dramoffset_kvs + i].data[7] = buffer[bramoffset_kvs + i].data[15]; // FIXME.
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countkvspartitionswritten_actual(VECTOR1024_SIZE);
			#endif
		}
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvspartitionswritten(realsize_kvs * VECTOR1024_SIZE);
		#endif
	}
	SAVEPARTITIONS_LOOP2: for(partition_type p=0; p<NUM_PARTITIONS; p++){ globalcapsule[p].value += localcapsule[p].value; }
	
	#ifdef _DEBUGMODE_CHECKS
	actsutilityobj->printkeyvalues("savekeyvalues0::globalcapsule", globalcapsule, NUM_PARTITIONS);
	actsutilityobj->scankeyvalues("savekeyvalues0::kvdram", (keyvalue_t *)(&kvdram[BASEOFFSET_KVDRAMWORKSPACE_KVS]), globalcapsule, NUM_PARTITIONS, BATCH_RANGE / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	return;
}

void 
	#ifdef SW 
	actslw::
	#endif 
savevertices0(unsigned int enable, uint512_dt * kvdram1, uint512_dt * kvdram2, keyvalue_t buffer[16][PADDEDDESTBUFFER_SIZE], batch_type offset_kvs){
	if(enable == OFF){ return; }
		
	SAVEVERTICES_LOOP: for (buffer_type i=0; i<PADDEDDESTBUFFER_SIZE; i++){
	#pragma HLS PIPELINE II=1
		kvdram1[offset_kvs + i].data[0] = buffer[0][i]; 
		kvdram1[offset_kvs + i].data[1] = buffer[1][i]; 
		kvdram1[offset_kvs + i].data[2] = buffer[2][i]; 
		kvdram1[offset_kvs + i].data[3] = buffer[3][i]; 
		kvdram1[offset_kvs + i].data[4] = buffer[4][i]; 
		kvdram1[offset_kvs + i].data[5] = buffer[5][i]; 
		kvdram1[offset_kvs + i].data[6] = buffer[6][i]; 
		kvdram1[offset_kvs + i].data[7] = buffer[7][i]; 
		kvdram2[offset_kvs + i].data[0] = buffer[8][i];
		kvdram2[offset_kvs + i].data[1] = buffer[9][i];
		kvdram2[offset_kvs + i].data[2] = buffer[10][i];
		kvdram2[offset_kvs + i].data[3] = buffer[11][i];
		kvdram2[offset_kvs + i].data[4] = buffer[12][i];
		kvdram2[offset_kvs + i].data[5] = buffer[13][i];
		kvdram2[offset_kvs + i].data[6] = buffer[14][i];
		kvdram2[offset_kvs + i].data[7] = buffer[15][i];
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvswritten(VECTOR1024_SIZE);
		#endif
	}
	return;
}

void 
	#ifdef SW 
	actslw::
	#endif
collectglobalstats0(unsigned int enable, keyvalue_t sourcebuffer[16][SRCBUFFER_SIZE], keyvalue_t destbuffer[16][PADDEDDESTBUFFER_SIZE], unsigned int upperlimit){
	if(enable == OFF){ return; }
	unsigned int analysis_srcbuffersz = SRCBUFFER_SIZE;

	COLLECTGLOBALSTATS_LOOP: for(unsigned int i=0; i<SRCBUFFER_SIZE; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
	#pragma HLS PIPELINE II=2
		keyvalue_t keyvalue0 = sourcebuffer[0][i];
		keyvalue_t keyvalue1 = sourcebuffer[1][i];
		keyvalue_t keyvalue2 = sourcebuffer[2][i];
		keyvalue_t keyvalue3 = sourcebuffer[3][i];
		keyvalue_t keyvalue4 = sourcebuffer[4][i];
		keyvalue_t keyvalue5 = sourcebuffer[5][i];
		keyvalue_t keyvalue6 = sourcebuffer[6][i];
		keyvalue_t keyvalue7 = sourcebuffer[7][i];
		keyvalue_t keyvalue8 = sourcebuffer[8][i];
		keyvalue_t keyvalue9 = sourcebuffer[9][i];
		keyvalue_t keyvalue10 = sourcebuffer[10][i];
		keyvalue_t keyvalue11 = sourcebuffer[11][i];
		keyvalue_t keyvalue12 = sourcebuffer[12][i];
		keyvalue_t keyvalue13 = sourcebuffer[13][i];
		keyvalue_t keyvalue14 = sourcebuffer[14][i];
		keyvalue_t keyvalue15 = sourcebuffer[15][i];
		
		partition_type p0 = getglobalpartition(keyvalue0, upperlimit);
		partition_type p1 = getglobalpartition(keyvalue1, upperlimit);
		partition_type p2 = getglobalpartition(keyvalue2, upperlimit);
		partition_type p3 = getglobalpartition(keyvalue3, upperlimit);
		partition_type p4 = getglobalpartition(keyvalue4, upperlimit);
		partition_type p5 = getglobalpartition(keyvalue5, upperlimit);
		partition_type p6 = getglobalpartition(keyvalue6, upperlimit);
		partition_type p7 = getglobalpartition(keyvalue7, upperlimit);
		partition_type p8 = getglobalpartition(keyvalue8, upperlimit);
		partition_type p9 = getglobalpartition(keyvalue9, upperlimit);
		partition_type p10 = getglobalpartition(keyvalue10, upperlimit);
		partition_type p11 = getglobalpartition(keyvalue11, upperlimit);
		partition_type p12 = getglobalpartition(keyvalue12, upperlimit);
		partition_type p13 = getglobalpartition(keyvalue13, upperlimit);
		partition_type p14 = getglobalpartition(keyvalue14, upperlimit);
		partition_type p15 = getglobalpartition(keyvalue15, upperlimit);
		
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[0]", p0, PADDEDDESTBUFFER_SIZE, keyvalue0.key, BATCH_RANGE, NAp);
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[1]", p1, PADDEDDESTBUFFER_SIZE, keyvalue1.key, BATCH_RANGE, NAp);
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[2]", p2, PADDEDDESTBUFFER_SIZE, keyvalue2.key, BATCH_RANGE, NAp);
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[3]", p3, PADDEDDESTBUFFER_SIZE, keyvalue3.key, BATCH_RANGE, NAp);
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[4]", p4, PADDEDDESTBUFFER_SIZE, keyvalue4.key, BATCH_RANGE, NAp);
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[5]", p5, PADDEDDESTBUFFER_SIZE, keyvalue5.key, BATCH_RANGE, NAp);
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[6]", p6, PADDEDDESTBUFFER_SIZE, keyvalue6.key, BATCH_RANGE, NAp);
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[7]", p7, PADDEDDESTBUFFER_SIZE, keyvalue7.key, BATCH_RANGE, NAp);
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[8]", p8, PADDEDDESTBUFFER_SIZE, keyvalue8.key, BATCH_RANGE, NAp);
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[9]", p9, PADDEDDESTBUFFER_SIZE, keyvalue9.key, BATCH_RANGE, NAp);
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[10]", p10, PADDEDDESTBUFFER_SIZE, keyvalue10.key, BATCH_RANGE, NAp);
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[11]", p11, PADDEDDESTBUFFER_SIZE, keyvalue11.key, BATCH_RANGE, NAp);
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[12]", p12, PADDEDDESTBUFFER_SIZE, keyvalue12.key, BATCH_RANGE, NAp);
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[13]", p13, PADDEDDESTBUFFER_SIZE, keyvalue13.key, BATCH_RANGE, NAp);
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[14]", p14, PADDEDDESTBUFFER_SIZE, keyvalue14.key, BATCH_RANGE, NAp);
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[15]", p15, PADDEDDESTBUFFER_SIZE, keyvalue15.key, BATCH_RANGE, NAp);
		#endif 
		destbuffer[0][p0].value += 1;
		destbuffer[1][p1].value += 1;
		destbuffer[2][p2].value += 1;
		destbuffer[3][p3].value += 1;
		destbuffer[4][p4].value += 1;
		destbuffer[5][p5].value += 1;
		destbuffer[6][p6].value += 1;
		destbuffer[7][p7].value += 1;
		destbuffer[8][p8].value += 1;
		destbuffer[9][p9].value += 1;
		destbuffer[10][p10].value += 1;
		destbuffer[11][p11].value += 1;
		destbuffer[12][p12].value += 1;
		destbuffer[13][p13].value += 1;
		destbuffer[14][p14].value += 1;
		destbuffer[15][p15].value += 1;
	}
	return;
}

void 
	#ifdef SW 
	actslw::
	#endif
prepareglobalstats0(unsigned int enable, keyvalue_t buffer[16][PADDEDDESTBUFFER_SIZE]){
	if(enable == OFF){ return; }
	
	PREPAREGLOBALSTATS_LOOP1: for(vector_type v=1; v<16; v++){
		PREPAREGLOBALSTATS_LOOP1B: for(buffer_type i=0; i<PADDEDDESTBUFFER_SIZE; i++){
			buffer[0][i].value += buffer[v][i].value;
		}
	}

	unsigned int skipsize = KVDRAMPADDING / NUMLASTLEVELPARTITIONS;
	skipsize = skipsize * 2; //
	calculateoffsets(buffer[0], NUMLASTLEVELPARTITIONS, 0, skipsize);
	resetvalues(buffer[0], PADDEDDESTBUFFER_SIZE);
	return;
}

void 
	#ifdef SW 
	actslw::
	#endif
partitionkeyvalues0(unsigned int enable, keyvalue_t sourcebuffer[16][SRCBUFFER_SIZE], keyvalue_t destbuffer[16][PADDEDDESTBUFFER_SIZE], keyvalue_t localcapsule[16][NUM_PARTITIONS], unsigned int currentLOP, unsigned int upperlimit, travstate_t travstate){
	if(enable == OFF){ return; }
	unsigned int analysis_srcbuffersz = SRCBUFFER_SIZE;
	buffer_type chunk_size = getchunksize(SRCBUFFER_SIZE, travstate, 0);

	resetmanykeyandvalues(localcapsule, NUM_PARTITIONS);
	
	PARTITIONKEYVALUES_LOOP1: for(unsigned int c=0; c<2; c++){
		PARTITIONKEYVALUES_LOOP1B: for(unsigned int i=0; i<chunk_size; i++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
		#pragma HLS PIPELINE II=2
			keyvalue_t keyvalue0 = sourcebuffer[0][i];
			keyvalue_t keyvalue1 = sourcebuffer[1][i];
			keyvalue_t keyvalue2 = sourcebuffer[2][i];
			keyvalue_t keyvalue3 = sourcebuffer[3][i];
			keyvalue_t keyvalue4 = sourcebuffer[4][i];
			keyvalue_t keyvalue5 = sourcebuffer[5][i];
			keyvalue_t keyvalue6 = sourcebuffer[6][i];
			keyvalue_t keyvalue7 = sourcebuffer[7][i];
			keyvalue_t keyvalue8 = sourcebuffer[8][i];
			keyvalue_t keyvalue9 = sourcebuffer[9][i];
			keyvalue_t keyvalue10 = sourcebuffer[10][i];
			keyvalue_t keyvalue11 = sourcebuffer[11][i];
			keyvalue_t keyvalue12 = sourcebuffer[12][i];
			keyvalue_t keyvalue13 = sourcebuffer[13][i];
			keyvalue_t keyvalue14 = sourcebuffer[14][i];
			keyvalue_t keyvalue15 = sourcebuffer[15][i];
			
			partition_type p0 = 0;
			if(keyvalue0.key != INVALIDDATA){ p0 = getpartition(keyvalue0, currentLOP, upperlimit); }
			partition_type p1 = 0;
			if(keyvalue1.key != INVALIDDATA){ p1 = getpartition(keyvalue1, currentLOP, upperlimit); }
			partition_type p2 = 0;
			if(keyvalue2.key != INVALIDDATA){ p2 = getpartition(keyvalue2, currentLOP, upperlimit); }
			partition_type p3 = 0;
			if(keyvalue3.key != INVALIDDATA){ p3 = getpartition(keyvalue3, currentLOP, upperlimit); }
			partition_type p4 = 0;
			if(keyvalue4.key != INVALIDDATA){ p4 = getpartition(keyvalue4, currentLOP, upperlimit); }
			partition_type p5 = 0;
			if(keyvalue5.key != INVALIDDATA){ p5 = getpartition(keyvalue5, currentLOP, upperlimit); }
			partition_type p6 = 0;
			if(keyvalue6.key != INVALIDDATA){ p6 = getpartition(keyvalue6, currentLOP, upperlimit); }
			partition_type p7 = 0;
			if(keyvalue7.key != INVALIDDATA){ p7 = getpartition(keyvalue7, currentLOP, upperlimit); }
			partition_type p8 = 0;
			if(keyvalue8.key != INVALIDDATA){ p8 = getpartition(keyvalue8, currentLOP, upperlimit); }
			partition_type p9 = 0;
			if(keyvalue9.key != INVALIDDATA){ p9 = getpartition(keyvalue9, currentLOP, upperlimit); }
			partition_type p10 = 0;
			if(keyvalue10.key != INVALIDDATA){ p10 = getpartition(keyvalue10, currentLOP, upperlimit); }
			partition_type p11 = 0;
			if(keyvalue11.key != INVALIDDATA){ p11 = getpartition(keyvalue11, currentLOP, upperlimit); }
			partition_type p12 = 0;
			if(keyvalue12.key != INVALIDDATA){ p12 = getpartition(keyvalue12, currentLOP, upperlimit); }
			partition_type p13 = 0;
			if(keyvalue13.key != INVALIDDATA){ p13 = getpartition(keyvalue13, currentLOP, upperlimit); }
			partition_type p14 = 0;
			if(keyvalue14.key != INVALIDDATA){ p14 = getpartition(keyvalue14, currentLOP, upperlimit); }
			partition_type p15 = 0;
			if(keyvalue15.key != INVALIDDATA){ p15 = getpartition(keyvalue15, currentLOP, upperlimit); }
			
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", localcapsule[0][p0].key + localcapsule[0][p0].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", localcapsule[1][p1].key + localcapsule[1][p1].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", localcapsule[2][p2].key + localcapsule[2][p2].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", localcapsule[3][p3].key + localcapsule[3][p3].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", localcapsule[4][p4].key + localcapsule[4][p4].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", localcapsule[5][p5].key + localcapsule[5][p5].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", localcapsule[6][p6].key + localcapsule[6][p6].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", localcapsule[7][p7].key + localcapsule[7][p7].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", localcapsule[8][p8].key + localcapsule[8][p8].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", localcapsule[9][p9].key + localcapsule[9][p9].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", localcapsule[10][p10].key + localcapsule[10][p10].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", localcapsule[11][p11].key + localcapsule[11][p11].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", localcapsule[12][p12].key + localcapsule[12][p12].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", localcapsule[13][p13].key + localcapsule[13][p13].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", localcapsule[14][p14].key + localcapsule[14][p14].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", localcapsule[15][p15].key + localcapsule[15][p15].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			#endif
			if(keyvalue0.key != INVALIDDATA){ destbuffer[0][localcapsule[0][p0].key + localcapsule[0][p0].value] = keyvalue0; }
			if(keyvalue1.key != INVALIDDATA){ destbuffer[1][localcapsule[1][p1].key + localcapsule[1][p1].value] = keyvalue1; }
			if(keyvalue2.key != INVALIDDATA){ destbuffer[2][localcapsule[2][p2].key + localcapsule[2][p2].value] = keyvalue2; }
			if(keyvalue3.key != INVALIDDATA){ destbuffer[3][localcapsule[3][p3].key + localcapsule[3][p3].value] = keyvalue3; }
			if(keyvalue4.key != INVALIDDATA){ destbuffer[4][localcapsule[4][p4].key + localcapsule[4][p4].value] = keyvalue4; }
			if(keyvalue5.key != INVALIDDATA){ destbuffer[5][localcapsule[5][p5].key + localcapsule[5][p5].value] = keyvalue5; }
			if(keyvalue6.key != INVALIDDATA){ destbuffer[6][localcapsule[6][p6].key + localcapsule[6][p6].value] = keyvalue6; }
			if(keyvalue7.key != INVALIDDATA){ destbuffer[7][localcapsule[7][p7].key + localcapsule[7][p7].value] = keyvalue7; }
			if(keyvalue8.key != INVALIDDATA){ destbuffer[8][localcapsule[8][p8].key + localcapsule[8][p8].value] = keyvalue8; }
			if(keyvalue9.key != INVALIDDATA){ destbuffer[9][localcapsule[9][p9].key + localcapsule[9][p9].value] = keyvalue9; }
			if(keyvalue10.key != INVALIDDATA){ destbuffer[10][localcapsule[10][p10].key + localcapsule[10][p10].value] = keyvalue10; }
			if(keyvalue11.key != INVALIDDATA){ destbuffer[11][localcapsule[11][p11].key + localcapsule[11][p11].value] = keyvalue11; }
			if(keyvalue12.key != INVALIDDATA){ destbuffer[12][localcapsule[12][p12].key + localcapsule[12][p12].value] = keyvalue12; }
			if(keyvalue13.key != INVALIDDATA){ destbuffer[13][localcapsule[13][p13].key + localcapsule[13][p13].value] = keyvalue13; }
			if(keyvalue14.key != INVALIDDATA){ destbuffer[14][localcapsule[14][p14].key + localcapsule[14][p14].value] = keyvalue14; }
			if(keyvalue15.key != INVALIDDATA){ destbuffer[15][localcapsule[15][p15].key + localcapsule[15][p15].value] = keyvalue15; }
			
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", p0, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", p1, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", p2, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", p3, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", p4, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", p5, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", p6, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", p7, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", p8, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", p9, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", p10, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", p11, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", p12, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", p13, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", p14, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", p15, NUM_PARTITIONS, NAp, NAp, NAp);
			#endif
			if(keyvalue0.key != INVALIDDATA){ localcapsule[0][p0].value += 1; }
			if(keyvalue1.key != INVALIDDATA){ localcapsule[1][p1].value += 1; }
			if(keyvalue2.key != INVALIDDATA){ localcapsule[2][p2].value += 1; }
			if(keyvalue3.key != INVALIDDATA){ localcapsule[3][p3].value += 1; }
			if(keyvalue4.key != INVALIDDATA){ localcapsule[4][p4].value += 1; }
			if(keyvalue5.key != INVALIDDATA){ localcapsule[5][p5].value += 1; }
			if(keyvalue6.key != INVALIDDATA){ localcapsule[6][p6].value += 1; }
			if(keyvalue7.key != INVALIDDATA){ localcapsule[7][p7].value += 1; }
			if(keyvalue8.key != INVALIDDATA){ localcapsule[8][p8].value += 1; }
			if(keyvalue9.key != INVALIDDATA){ localcapsule[9][p9].value += 1; }
			if(keyvalue10.key != INVALIDDATA){ localcapsule[10][p10].value += 1; }
			if(keyvalue11.key != INVALIDDATA){ localcapsule[11][p11].value += 1; }
			if(keyvalue12.key != INVALIDDATA){ localcapsule[12][p12].value += 1; }
			if(keyvalue13.key != INVALIDDATA){ localcapsule[13][p13].value += 1; }
			if(keyvalue14.key != INVALIDDATA){ localcapsule[14][p14].value += 1; }
			if(keyvalue15.key != INVALIDDATA){ localcapsule[15][p15].value += 1; }
		}
		if(c==0){ calculatemanyunallignedoffsets(localcapsule, NUM_PARTITIONS, 0, 0); } 
		if(c==0){ resetmanyvalues(localcapsule, NUM_PARTITIONS); }
	}
	
	#ifdef _DEBUGMODE_STATS
	for(vector_type v=0; v<VECTOR1024_SIZE; v++){ actsutilityobj->globalvar_inmemory_counttotalvalidkeyvalues(actsutilityobj->ugetvaluecount(localcapsule[v], NUM_PARTITIONS)); }
	#endif
	return;
}

void 
	#ifdef SW 
	actslw::
	#endif
reduce0(unsigned int enable, keyvalue_t sourcebuffer[16][SRCBUFFER_SIZE], keyvalue_t destbuffer[16][PADDEDDESTBUFFER_SIZE], unsigned int upperlimit, unsigned int GraphIter, unsigned int GraphAlgo, travstate_t travstate){
	if(enable == OFF){ return; }
	unsigned int analysis_srcbuffersz = SRCBUFFER_SIZE;
	buffer_type chunk_size = getchunksize(SRCBUFFER_SIZE, travstate, 0);

	REDUCE_LOOP: for(unsigned int i=0; i<chunk_size; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
	#pragma HLS PIPELINE II=2
		keyvalue_t keyvalue0 = sourcebuffer[0][i];
		keyvalue_t keyvalue1 = sourcebuffer[1][i];
		keyvalue_t keyvalue2 = sourcebuffer[2][i];
		keyvalue_t keyvalue3 = sourcebuffer[3][i];
		keyvalue_t keyvalue4 = sourcebuffer[4][i];
		keyvalue_t keyvalue5 = sourcebuffer[5][i];
		keyvalue_t keyvalue6 = sourcebuffer[6][i];
		keyvalue_t keyvalue7 = sourcebuffer[7][i];
		keyvalue_t keyvalue8 = sourcebuffer[8][i];
		keyvalue_t keyvalue9 = sourcebuffer[9][i];
		keyvalue_t keyvalue10 = sourcebuffer[10][i];
		keyvalue_t keyvalue11 = sourcebuffer[11][i];
		keyvalue_t keyvalue12 = sourcebuffer[12][i];
		keyvalue_t keyvalue13 = sourcebuffer[13][i];
		keyvalue_t keyvalue14 = sourcebuffer[14][i];
		keyvalue_t keyvalue15 = sourcebuffer[15][i];
		
		vertex_t loc0 = keyvalue0.key - upperlimit;
		vertex_t loc1 = keyvalue1.key - upperlimit;
		vertex_t loc2 = keyvalue2.key - upperlimit;
		vertex_t loc3 = keyvalue3.key - upperlimit;
		vertex_t loc4 = keyvalue4.key - upperlimit;
		vertex_t loc5 = keyvalue5.key - upperlimit;
		vertex_t loc6 = keyvalue6.key - upperlimit;
		vertex_t loc7 = keyvalue7.key - upperlimit;
		vertex_t loc8 = keyvalue8.key - upperlimit;
		vertex_t loc9 = keyvalue9.key - upperlimit;
		vertex_t loc10 = keyvalue10.key - upperlimit;
		vertex_t loc11 = keyvalue11.key - upperlimit;
		vertex_t loc12 = keyvalue12.key - upperlimit;
		vertex_t loc13 = keyvalue13.key - upperlimit;
		vertex_t loc14 = keyvalue14.key - upperlimit;
		vertex_t loc15 = keyvalue15.key - upperlimit;
		
		if(loc0 >= APPLYVERTEXBUFFERSZ && keyvalue0.key != INVALIDDATA){
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTREDUCEERRORS
			cout<<"ERROR SEEN @ reduce0:: i: "<<i<<", loc0: "<<loc0<<", keyvalue0.key: "<<keyvalue0.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<APPLYVERTEXBUFFERSZ <<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			exit(EXIT_FAILURE); // FIXME. PERFECTIONTEST.
			#endif 
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->globalstats_counterrorsinreduce(1);
			#endif
			loc0 = 0;
		} // REMOVEME.
		if(loc1 >= APPLYVERTEXBUFFERSZ && keyvalue1.key != INVALIDDATA){
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTREDUCEERRORS
			cout<<"ERROR SEEN @ reduce0:: i: "<<i<<", loc1: "<<loc1<<", keyvalue1.key: "<<keyvalue1.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<APPLYVERTEXBUFFERSZ <<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			exit(EXIT_FAILURE); // FIXME. PERFECTIONTEST.
			#endif 
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->globalstats_counterrorsinreduce(1);
			#endif
			loc1 = 0;
		} // REMOVEME.
		if(loc2 >= APPLYVERTEXBUFFERSZ && keyvalue2.key != INVALIDDATA){
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTREDUCEERRORS
			cout<<"ERROR SEEN @ reduce0:: i: "<<i<<", loc2: "<<loc2<<", keyvalue2.key: "<<keyvalue2.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<APPLYVERTEXBUFFERSZ <<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			exit(EXIT_FAILURE); // FIXME. PERFECTIONTEST.
			#endif 
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->globalstats_counterrorsinreduce(1);
			#endif
			loc2 = 0;
		} // REMOVEME.
		if(loc3 >= APPLYVERTEXBUFFERSZ && keyvalue3.key != INVALIDDATA){
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTREDUCEERRORS
			cout<<"ERROR SEEN @ reduce0:: i: "<<i<<", loc3: "<<loc3<<", keyvalue3.key: "<<keyvalue3.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<APPLYVERTEXBUFFERSZ <<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			exit(EXIT_FAILURE); // FIXME. PERFECTIONTEST.
			#endif 
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->globalstats_counterrorsinreduce(1);
			#endif
			loc3 = 0;
		} // REMOVEME.
		if(loc4 >= APPLYVERTEXBUFFERSZ && keyvalue4.key != INVALIDDATA){
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTREDUCEERRORS
			cout<<"ERROR SEEN @ reduce0:: i: "<<i<<", loc4: "<<loc4<<", keyvalue4.key: "<<keyvalue4.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<APPLYVERTEXBUFFERSZ <<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			exit(EXIT_FAILURE); // FIXME. PERFECTIONTEST.
			#endif 
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->globalstats_counterrorsinreduce(1);
			#endif
			loc4 = 0;
		} // REMOVEME.
		if(loc5 >= APPLYVERTEXBUFFERSZ && keyvalue5.key != INVALIDDATA){
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTREDUCEERRORS
			cout<<"ERROR SEEN @ reduce0:: i: "<<i<<", loc5: "<<loc5<<", keyvalue5.key: "<<keyvalue5.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<APPLYVERTEXBUFFERSZ <<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			exit(EXIT_FAILURE); // FIXME. PERFECTIONTEST.
			#endif 
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->globalstats_counterrorsinreduce(1);
			#endif
			loc5 = 0;
		} // REMOVEME.
		if(loc6 >= APPLYVERTEXBUFFERSZ && keyvalue6.key != INVALIDDATA){
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTREDUCEERRORS
			cout<<"ERROR SEEN @ reduce0:: i: "<<i<<", loc6: "<<loc6<<", keyvalue6.key: "<<keyvalue6.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<APPLYVERTEXBUFFERSZ <<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			exit(EXIT_FAILURE); // FIXME. PERFECTIONTEST.
			#endif 
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->globalstats_counterrorsinreduce(1);
			#endif
			loc6 = 0;
		} // REMOVEME.
		if(loc7 >= APPLYVERTEXBUFFERSZ && keyvalue7.key != INVALIDDATA){
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTREDUCEERRORS
			cout<<"ERROR SEEN @ reduce0:: i: "<<i<<", loc7: "<<loc7<<", keyvalue7.key: "<<keyvalue7.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<APPLYVERTEXBUFFERSZ <<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			exit(EXIT_FAILURE); // FIXME. PERFECTIONTEST.
			#endif 
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->globalstats_counterrorsinreduce(1);
			#endif
			loc7 = 0;
		} // REMOVEME.
		if(loc8 >= APPLYVERTEXBUFFERSZ && keyvalue8.key != INVALIDDATA){
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTREDUCEERRORS
			cout<<"ERROR SEEN @ reduce0:: i: "<<i<<", loc8: "<<loc8<<", keyvalue8.key: "<<keyvalue8.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<APPLYVERTEXBUFFERSZ <<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			exit(EXIT_FAILURE); // FIXME. PERFECTIONTEST.
			#endif 
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->globalstats_counterrorsinreduce(1);
			#endif
			loc8 = 0;
		} // REMOVEME.
		if(loc9 >= APPLYVERTEXBUFFERSZ && keyvalue9.key != INVALIDDATA){
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTREDUCEERRORS
			cout<<"ERROR SEEN @ reduce0:: i: "<<i<<", loc9: "<<loc9<<", keyvalue9.key: "<<keyvalue9.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<APPLYVERTEXBUFFERSZ <<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			exit(EXIT_FAILURE); // FIXME. PERFECTIONTEST.
			#endif 
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->globalstats_counterrorsinreduce(1);
			#endif
			loc9 = 0;
		} // REMOVEME.
		if(loc10 >= APPLYVERTEXBUFFERSZ && keyvalue10.key != INVALIDDATA){
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTREDUCEERRORS
			cout<<"ERROR SEEN @ reduce0:: i: "<<i<<", loc10: "<<loc10<<", keyvalue10.key: "<<keyvalue10.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<APPLYVERTEXBUFFERSZ <<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			exit(EXIT_FAILURE); // FIXME. PERFECTIONTEST.
			#endif 
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->globalstats_counterrorsinreduce(1);
			#endif
			loc10 = 0;
		} // REMOVEME.
		if(loc11 >= APPLYVERTEXBUFFERSZ && keyvalue11.key != INVALIDDATA){
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTREDUCEERRORS
			cout<<"ERROR SEEN @ reduce0:: i: "<<i<<", loc11: "<<loc11<<", keyvalue11.key: "<<keyvalue11.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<APPLYVERTEXBUFFERSZ <<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			exit(EXIT_FAILURE); // FIXME. PERFECTIONTEST.
			#endif 
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->globalstats_counterrorsinreduce(1);
			#endif
			loc11 = 0;
		} // REMOVEME.
		if(loc12 >= APPLYVERTEXBUFFERSZ && keyvalue12.key != INVALIDDATA){
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTREDUCEERRORS
			cout<<"ERROR SEEN @ reduce0:: i: "<<i<<", loc12: "<<loc12<<", keyvalue12.key: "<<keyvalue12.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<APPLYVERTEXBUFFERSZ <<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			exit(EXIT_FAILURE); // FIXME. PERFECTIONTEST.
			#endif 
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->globalstats_counterrorsinreduce(1);
			#endif
			loc12 = 0;
		} // REMOVEME.
		if(loc13 >= APPLYVERTEXBUFFERSZ && keyvalue13.key != INVALIDDATA){
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTREDUCEERRORS
			cout<<"ERROR SEEN @ reduce0:: i: "<<i<<", loc13: "<<loc13<<", keyvalue13.key: "<<keyvalue13.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<APPLYVERTEXBUFFERSZ <<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			exit(EXIT_FAILURE); // FIXME. PERFECTIONTEST.
			#endif 
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->globalstats_counterrorsinreduce(1);
			#endif
			loc13 = 0;
		} // REMOVEME.
		if(loc14 >= APPLYVERTEXBUFFERSZ && keyvalue14.key != INVALIDDATA){
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTREDUCEERRORS
			cout<<"ERROR SEEN @ reduce0:: i: "<<i<<", loc14: "<<loc14<<", keyvalue14.key: "<<keyvalue14.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<APPLYVERTEXBUFFERSZ <<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			exit(EXIT_FAILURE); // FIXME. PERFECTIONTEST.
			#endif 
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->globalstats_counterrorsinreduce(1);
			#endif
			loc14 = 0;
		} // REMOVEME.
		if(loc15 >= APPLYVERTEXBUFFERSZ && keyvalue15.key != INVALIDDATA){
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTREDUCEERRORS
			cout<<"ERROR SEEN @ reduce0:: i: "<<i<<", loc15: "<<loc15<<", keyvalue15.key: "<<keyvalue15.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<APPLYVERTEXBUFFERSZ <<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			exit(EXIT_FAILURE); // FIXME. PERFECTIONTEST.
			#endif 
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->globalstats_counterrorsinreduce(1);
			#endif
			loc15 = 0;
		} // REMOVEME.
		
		loc0 = loc0 / 2;
		loc1 = loc1 / 2;
		loc2 = loc2 / 2;
		loc3 = loc3 / 2;
		loc4 = loc4 / 2;
		loc5 = loc5 / 2;
		loc6 = loc6 / 2;
		loc7 = loc7 / 2;
		loc8 = loc8 / 2;
		loc9 = loc9 / 2;
		loc10 = loc10 / 2;
		loc11 = loc11 / 2;
		loc12 = loc12 / 2;
		loc13 = loc13 / 2;
		loc14 = loc14 / 2;
		loc15 = loc15 / 2;
		
		keyvalue_t vprop0;
		if(keyvalue0.key != INVALIDDATA){ vprop0 = destbuffer[0][loc0]; }
		keyvalue_t vprop1;
		if(keyvalue1.key != INVALIDDATA){ vprop1 = destbuffer[1][loc1]; }
		keyvalue_t vprop2;
		if(keyvalue2.key != INVALIDDATA){ vprop2 = destbuffer[2][loc2]; }
		keyvalue_t vprop3;
		if(keyvalue3.key != INVALIDDATA){ vprop3 = destbuffer[3][loc3]; }
		keyvalue_t vprop4;
		if(keyvalue4.key != INVALIDDATA){ vprop4 = destbuffer[4][loc4]; }
		keyvalue_t vprop5;
		if(keyvalue5.key != INVALIDDATA){ vprop5 = destbuffer[5][loc5]; }
		keyvalue_t vprop6;
		if(keyvalue6.key != INVALIDDATA){ vprop6 = destbuffer[6][loc6]; }
		keyvalue_t vprop7;
		if(keyvalue7.key != INVALIDDATA){ vprop7 = destbuffer[7][loc7]; }
		keyvalue_t vprop8;
		if(keyvalue8.key != INVALIDDATA){ vprop8 = destbuffer[8][loc8]; }
		keyvalue_t vprop9;
		if(keyvalue9.key != INVALIDDATA){ vprop9 = destbuffer[9][loc9]; }
		keyvalue_t vprop10;
		if(keyvalue10.key != INVALIDDATA){ vprop10 = destbuffer[10][loc10]; }
		keyvalue_t vprop11;
		if(keyvalue11.key != INVALIDDATA){ vprop11 = destbuffer[11][loc11]; }
		keyvalue_t vprop12;
		if(keyvalue12.key != INVALIDDATA){ vprop12 = destbuffer[12][loc12]; }
		keyvalue_t vprop13;
		if(keyvalue13.key != INVALIDDATA){ vprop13 = destbuffer[13][loc13]; }
		keyvalue_t vprop14;
		if(keyvalue14.key != INVALIDDATA){ vprop14 = destbuffer[14][loc14]; }
		keyvalue_t vprop15;
		if(keyvalue15.key != INVALIDDATA){ vprop15 = destbuffer[15][loc15]; }
		
		value_t temp0 = reducefunc(vprop0.key, vprop0.value, keyvalue0.value, GraphIter, GraphAlgo);
		value_t temp1 = reducefunc(vprop1.key, vprop1.value, keyvalue1.value, GraphIter, GraphAlgo);
		value_t temp2 = reducefunc(vprop2.key, vprop2.value, keyvalue2.value, GraphIter, GraphAlgo);
		value_t temp3 = reducefunc(vprop3.key, vprop3.value, keyvalue3.value, GraphIter, GraphAlgo);
		value_t temp4 = reducefunc(vprop4.key, vprop4.value, keyvalue4.value, GraphIter, GraphAlgo);
		value_t temp5 = reducefunc(vprop5.key, vprop5.value, keyvalue5.value, GraphIter, GraphAlgo);
		value_t temp6 = reducefunc(vprop6.key, vprop6.value, keyvalue6.value, GraphIter, GraphAlgo);
		value_t temp7 = reducefunc(vprop7.key, vprop7.value, keyvalue7.value, GraphIter, GraphAlgo);
		value_t temp8 = reducefunc(vprop8.key, vprop8.value, keyvalue8.value, GraphIter, GraphAlgo);
		value_t temp9 = reducefunc(vprop9.key, vprop9.value, keyvalue9.value, GraphIter, GraphAlgo);
		value_t temp10 = reducefunc(vprop10.key, vprop10.value, keyvalue10.value, GraphIter, GraphAlgo);
		value_t temp11 = reducefunc(vprop11.key, vprop11.value, keyvalue11.value, GraphIter, GraphAlgo);
		value_t temp12 = reducefunc(vprop12.key, vprop12.value, keyvalue12.value, GraphIter, GraphAlgo);
		value_t temp13 = reducefunc(vprop13.key, vprop13.value, keyvalue13.value, GraphIter, GraphAlgo);
		value_t temp14 = reducefunc(vprop14.key, vprop14.value, keyvalue14.value, GraphIter, GraphAlgo);
		value_t temp15 = reducefunc(vprop15.key, vprop15.value, keyvalue15.value, GraphIter, GraphAlgo);
		
		vprop0.value = temp0;
		vprop1.value = temp1;
		vprop2.value = temp2;
		vprop3.value = temp3;
		vprop4.value = temp4;
		vprop5.value = temp5;
		vprop6.value = temp6;
		vprop7.value = temp7;
		vprop8.value = temp8;
		vprop9.value = temp9;
		vprop10.value = temp10;
		vprop11.value = temp11;
		vprop12.value = temp12;
		vprop13.value = temp13;
		vprop14.value = temp14;
		vprop15.value = temp15;
		
		if(keyvalue0.key != INVALIDDATA){ destbuffer[0][loc0] = vprop0; }
		if(keyvalue1.key != INVALIDDATA){ destbuffer[1][loc1] = vprop1; }
		if(keyvalue2.key != INVALIDDATA){ destbuffer[2][loc2] = vprop2; }
		if(keyvalue3.key != INVALIDDATA){ destbuffer[3][loc3] = vprop3; }
		if(keyvalue4.key != INVALIDDATA){ destbuffer[4][loc4] = vprop4; }
		if(keyvalue5.key != INVALIDDATA){ destbuffer[5][loc5] = vprop5; }
		if(keyvalue6.key != INVALIDDATA){ destbuffer[6][loc6] = vprop6; }
		if(keyvalue7.key != INVALIDDATA){ destbuffer[7][loc7] = vprop7; }
		if(keyvalue8.key != INVALIDDATA){ destbuffer[8][loc8] = vprop8; }
		if(keyvalue9.key != INVALIDDATA){ destbuffer[9][loc9] = vprop9; }
		if(keyvalue10.key != INVALIDDATA){ destbuffer[10][loc10] = vprop10; }
		if(keyvalue11.key != INVALIDDATA){ destbuffer[11][loc11] = vprop11; }
		if(keyvalue12.key != INVALIDDATA){ destbuffer[12][loc12] = vprop12; }
		if(keyvalue13.key != INVALIDDATA){ destbuffer[13][loc13] = vprop13; }
		if(keyvalue14.key != INVALIDDATA){ destbuffer[14][loc14] = vprop14; }
		if(keyvalue15.key != INVALIDDATA){ destbuffer[15][loc15] = vprop15; }
		
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsreduced(VECTOR1024_SIZE);
		if(keyvalue0.key != INVALIDDATA){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(1); }
		if(keyvalue1.key != INVALIDDATA){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(1); }
		if(keyvalue2.key != INVALIDDATA){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(1); }
		if(keyvalue3.key != INVALIDDATA){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(1); }
		if(keyvalue4.key != INVALIDDATA){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(1); }
		if(keyvalue5.key != INVALIDDATA){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(1); }
		if(keyvalue6.key != INVALIDDATA){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(1); }
		if(keyvalue7.key != INVALIDDATA){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(1); }
		if(keyvalue8.key != INVALIDDATA){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(1); }
		if(keyvalue9.key != INVALIDDATA){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(1); }
		if(keyvalue10.key != INVALIDDATA){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(1); }
		if(keyvalue11.key != INVALIDDATA){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(1); }
		if(keyvalue12.key != INVALIDDATA){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(1); }
		if(keyvalue13.key != INVALIDDATA){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(1); }
		if(keyvalue14.key != INVALIDDATA){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(1); }
		if(keyvalue15.key != INVALIDDATA){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(1); }
		#endif
	}
	return;
}

void 
	#ifdef SW 
	actslw::
	#endif
combineSetof1stoSetof40_I0(unsigned int enable, keyvalue_t buffer_setof1M[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof1N[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof1O[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof1P[PADDEDDESTBUFFER_SIZE], uint256_dt buffer_setof4[PADDEDDESTBUFFER_SIZE], keyvalue_t localcapsuleM[NUM_PARTITIONS], keyvalue_t localcapsuleN[NUM_PARTITIONS], keyvalue_t localcapsuleO[NUM_PARTITIONS], keyvalue_t localcapsuleP[NUM_PARTITIONS], keyvalue_t localcapsuleR[NUM_PARTITIONS]){			
	if(enable == OFF){ return; }
	unsigned int analysis_srcbuffersz = SRCBUFFER_SIZE / NUM_PARTITIONS;
	unsigned int index=0, begin=0, end=0;
	keyvalue_t NullKV; NullKV.key = INVALIDDATA; NullKV.value = INVALIDDATA;
	
	resetkeyandvalues(localcapsuleR, NUM_PARTITIONS);
	
	EXECUTE_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS LOOP_TRIPCOUNT min=16 max=16 avg=16	
		unsigned int counter = 0;
		
		EXECUTE_LOOP1B: for(unsigned int i=0; i<4; i++){
			
			if(i==0){ begin = localcapsuleM[p].key; end = localcapsuleM[p].key + localcapsuleM[p].value; }
			else if(i==1){ begin = localcapsuleN[p].key; end = localcapsuleN[p].key + localcapsuleN[p].value; }
			else if(i==2){ begin = localcapsuleO[p].key; end = localcapsuleO[p].key + localcapsuleO[p].value; }
			else { begin = localcapsuleP[p].key; end = localcapsuleP[p].key + localcapsuleP[p].value; }
			
			EXECUTE_LOOP1C: for(unsigned int k=begin; k<end; k++){
			#pragma HLS LOOP_TRIPCOUNT min=analysis_srcbuffersz max=analysis_srcbuffersz avg=analysis_srcbuffersz	
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("combineSetof1stoSetof40_I0 1", index, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("combineSetof1stoSetof40_I0 2", k, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
				#endif
				
				unsigned int CK = counter % 4;
				
				if(i==0){
					if(CK == 0){
						buffer_setof4[index].data[0] = buffer_setof1M[k];
					} else if(CK == 1){
						buffer_setof4[index].data[1] = buffer_setof1M[k];
					} else if(CK == 2){
						buffer_setof4[index].data[2] = buffer_setof1M[k];
					} else {
						buffer_setof4[index].data[3] = buffer_setof1M[k];
					} 
				} else if(i==1) {
					if(CK == 0){
						buffer_setof4[index].data[0] = buffer_setof1N[k];
					} else if(CK == 1){
						buffer_setof4[index].data[1] = buffer_setof1N[k];
					} else if(CK == 2){
						buffer_setof4[index].data[2] = buffer_setof1N[k];
					} else {
						buffer_setof4[index].data[3] = buffer_setof1N[k];
					}
				} else if(i==2) {
					if(CK == 0){
						buffer_setof4[index].data[0] = buffer_setof1O[k];
					} else if(CK == 1){
						buffer_setof4[index].data[1] = buffer_setof1O[k];
					} else if(CK == 2){
						buffer_setof4[index].data[2] = buffer_setof1O[k];
					} else {
						buffer_setof4[index].data[3] = buffer_setof1O[k];
					}
				} else {
					if(CK == 0){
						buffer_setof4[index].data[0] = buffer_setof1P[k];
					} else if(CK == 1){
						buffer_setof4[index].data[1] = buffer_setof1P[k];
					} else if(CK == 2){
						buffer_setof4[index].data[2] = buffer_setof1P[k];
					} else {
						buffer_setof4[index].data[3] = buffer_setof1P[k];
					}
				}
				counter+=1; if(counter % 4 == 0){ index++; }
			}
		}
		
		localcapsuleR[p].value = localcapsuleM[p].value + localcapsuleN[p].value + localcapsuleO[p].value + localcapsuleP[p].value; 
		if(counter % 4 == 1){ 
						buffer_setof4[index].data[1] = NullKV; buffer_setof4[index].data[2] = NullKV; buffer_setof4[index].data[3] = NullKV; 
						localcapsuleR[p].value += 3; index += 1; } // edge conditions
		else if(counter % 4 == 2){ 
						buffer_setof4[index].data[2] = NullKV; buffer_setof4[index].data[3] = NullKV; 
						localcapsuleR[p].value += 2; index += 1; } // edge conditions
		else if(counter % 4 == 3){ 
						buffer_setof4[index].data[3] = NullKV; 
						localcapsuleR[p].value += 1; index += 1; } // edge conditions
		else {}
		
		#ifdef _DEBUGMODE_STATS
		if(counter % 4 == 1){ 
			actsutilityobj->globalvar_savepartitions_countinvalids(3);
		} else if(counter % 4 == 2){ 
			actsutilityobj->globalvar_savepartitions_countinvalids(2);
		} else if(counter % 4 == 3){ 
			actsutilityobj->globalvar_savepartitions_countinvalids(1);
		} else {}
		#endif
	}
	
	calculateunallignedoffsets(localcapsuleR, NUM_PARTITIONS, 0, 0);
	#ifdef _DEBUGMODE_CHECKS
	cout<<endl;
	actsutilityobj->scankeyvalues("combineSetof1stoSetof40_I0::buffer_setof1M", (keyvalue_t *)buffer_setof1M, localcapsuleM, NUM_PARTITIONS, BATCH_RANGE / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof1stoSetof40_I0::buffer_setof1N", (keyvalue_t *)buffer_setof1N, localcapsuleN, NUM_PARTITIONS, BATCH_RANGE / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof1stoSetof40_I0::buffer_setof1O", (keyvalue_t *)buffer_setof1O, localcapsuleO, NUM_PARTITIONS, BATCH_RANGE / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof1stoSetof40_I0::buffer_setof1P", (keyvalue_t *)buffer_setof1P, localcapsuleP, NUM_PARTITIONS, BATCH_RANGE / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof1stoSetof40_I0::buffer_setof4", (keyvalue_t *)buffer_setof4, localcapsuleR, NUM_PARTITIONS, BATCH_RANGE / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	// exit(EXIT_SUCCESS);
	return;
}

void 
	#ifdef SW 
	actslw::
	#endif
combineSetof1stoSetof40_I1(unsigned int enable, keyvalue_t buffer_setof1M[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof1N[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof1O[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof1P[PADDEDDESTBUFFER_SIZE], uint256_dt buffer_setof4[PADDEDDESTBUFFER_SIZE], keyvalue_t localcapsuleM[NUM_PARTITIONS], keyvalue_t localcapsuleN[NUM_PARTITIONS], keyvalue_t localcapsuleO[NUM_PARTITIONS], keyvalue_t localcapsuleP[NUM_PARTITIONS], keyvalue_t localcapsuleR[NUM_PARTITIONS]){			
	if(enable == OFF){ return; }
	unsigned int analysis_srcbuffersz = SRCBUFFER_SIZE / NUM_PARTITIONS;
	unsigned int index=0, begin=0, end=0;
	keyvalue_t NullKV; NullKV.key = INVALIDDATA; NullKV.value = INVALIDDATA;
	
	resetkeyandvalues(localcapsuleR, NUM_PARTITIONS);
	
	EXECUTE_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS LOOP_TRIPCOUNT min=16 max=16 avg=16	
		unsigned int counter = 0;
		
		EXECUTE_LOOP1B: for(unsigned int i=0; i<4; i++){
			
			if(i==0){ begin = localcapsuleM[p].key; end = localcapsuleM[p].key + localcapsuleM[p].value; }
			else if(i==1){ begin = localcapsuleN[p].key; end = localcapsuleN[p].key + localcapsuleN[p].value; }
			else if(i==2){ begin = localcapsuleO[p].key; end = localcapsuleO[p].key + localcapsuleO[p].value; }
			else { begin = localcapsuleP[p].key; end = localcapsuleP[p].key + localcapsuleP[p].value; }
			
			EXECUTE_LOOP1C: for(unsigned int k=begin; k<end; k++){
			#pragma HLS LOOP_TRIPCOUNT min=analysis_srcbuffersz max=analysis_srcbuffersz avg=analysis_srcbuffersz	
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("combineSetof1stoSetof40_I1 1", index, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("combineSetof1stoSetof40_I1 2", k, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
				#endif
				
				unsigned int CK = counter % 4;
				
				if(i==0){
					if(CK == 0){
						buffer_setof4[index].data[0] = buffer_setof1M[k];
					} else if(CK == 1){
						buffer_setof4[index].data[1] = buffer_setof1M[k];
					} else if(CK == 2){
						buffer_setof4[index].data[2] = buffer_setof1M[k];
					} else {
						buffer_setof4[index].data[3] = buffer_setof1M[k];
					} 
				} else if(i==1) {
					if(CK == 0){
						buffer_setof4[index].data[0] = buffer_setof1N[k];
					} else if(CK == 1){
						buffer_setof4[index].data[1] = buffer_setof1N[k];
					} else if(CK == 2){
						buffer_setof4[index].data[2] = buffer_setof1N[k];
					} else {
						buffer_setof4[index].data[3] = buffer_setof1N[k];
					}
				} else if(i==2) {
					if(CK == 0){
						buffer_setof4[index].data[0] = buffer_setof1O[k];
					} else if(CK == 1){
						buffer_setof4[index].data[1] = buffer_setof1O[k];
					} else if(CK == 2){
						buffer_setof4[index].data[2] = buffer_setof1O[k];
					} else {
						buffer_setof4[index].data[3] = buffer_setof1O[k];
					}
				} else {
					if(CK == 0){
						buffer_setof4[index].data[0] = buffer_setof1P[k];
					} else if(CK == 1){
						buffer_setof4[index].data[1] = buffer_setof1P[k];
					} else if(CK == 2){
						buffer_setof4[index].data[2] = buffer_setof1P[k];
					} else {
						buffer_setof4[index].data[3] = buffer_setof1P[k];
					}
				}
				counter+=1; if(counter % 4 == 0){ index++; }
			}
		}
		
		localcapsuleR[p].value = localcapsuleM[p].value + localcapsuleN[p].value + localcapsuleO[p].value + localcapsuleP[p].value; 
		if(counter % 4 == 1){ 
						buffer_setof4[index].data[1] = NullKV; buffer_setof4[index].data[2] = NullKV; buffer_setof4[index].data[3] = NullKV; 
						localcapsuleR[p].value += 3; index += 1; } // edge conditions
		else if(counter % 4 == 2){ 
						buffer_setof4[index].data[2] = NullKV; buffer_setof4[index].data[3] = NullKV; 
						localcapsuleR[p].value += 2; index += 1; } // edge conditions
		else if(counter % 4 == 3){ 
						buffer_setof4[index].data[3] = NullKV; 
						localcapsuleR[p].value += 1; index += 1; } // edge conditions
		else {}
		
		#ifdef _DEBUGMODE_STATS
		if(counter % 4 == 1){ 
			actsutilityobj->globalvar_savepartitions_countinvalids(3);
		} else if(counter % 4 == 2){ 
			actsutilityobj->globalvar_savepartitions_countinvalids(2);
		} else if(counter % 4 == 3){ 
			actsutilityobj->globalvar_savepartitions_countinvalids(1);
		} else {}
		#endif
	}
	
	calculateunallignedoffsets(localcapsuleR, NUM_PARTITIONS, 0, 0);
	#ifdef _DEBUGMODE_CHECKS
	cout<<endl;
	actsutilityobj->scankeyvalues("combineSetof1stoSetof40_I1::buffer_setof1M", (keyvalue_t *)buffer_setof1M, localcapsuleM, NUM_PARTITIONS, BATCH_RANGE / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof1stoSetof40_I1::buffer_setof1N", (keyvalue_t *)buffer_setof1N, localcapsuleN, NUM_PARTITIONS, BATCH_RANGE / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof1stoSetof40_I1::buffer_setof1O", (keyvalue_t *)buffer_setof1O, localcapsuleO, NUM_PARTITIONS, BATCH_RANGE / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof1stoSetof40_I1::buffer_setof1P", (keyvalue_t *)buffer_setof1P, localcapsuleP, NUM_PARTITIONS, BATCH_RANGE / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof1stoSetof40_I1::buffer_setof4", (keyvalue_t *)buffer_setof4, localcapsuleR, NUM_PARTITIONS, BATCH_RANGE / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	// exit(EXIT_SUCCESS);
	return;
}

void 
	#ifdef SW 
	actslw::
	#endif
combineSetof1stoSetof40_I2(unsigned int enable, keyvalue_t buffer_setof1M[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof1N[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof1O[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof1P[PADDEDDESTBUFFER_SIZE], uint256_dt buffer_setof4[PADDEDDESTBUFFER_SIZE], keyvalue_t localcapsuleM[NUM_PARTITIONS], keyvalue_t localcapsuleN[NUM_PARTITIONS], keyvalue_t localcapsuleO[NUM_PARTITIONS], keyvalue_t localcapsuleP[NUM_PARTITIONS], keyvalue_t localcapsuleR[NUM_PARTITIONS]){			
	if(enable == OFF){ return; }
	unsigned int analysis_srcbuffersz = SRCBUFFER_SIZE / NUM_PARTITIONS;
	unsigned int index=0, begin=0, end=0;
	keyvalue_t NullKV; NullKV.key = INVALIDDATA; NullKV.value = INVALIDDATA;
	
	resetkeyandvalues(localcapsuleR, NUM_PARTITIONS);
	
	EXECUTE_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS LOOP_TRIPCOUNT min=16 max=16 avg=16	
		unsigned int counter = 0;
		
		EXECUTE_LOOP1B: for(unsigned int i=0; i<4; i++){
			
			if(i==0){ begin = localcapsuleM[p].key; end = localcapsuleM[p].key + localcapsuleM[p].value; }
			else if(i==1){ begin = localcapsuleN[p].key; end = localcapsuleN[p].key + localcapsuleN[p].value; }
			else if(i==2){ begin = localcapsuleO[p].key; end = localcapsuleO[p].key + localcapsuleO[p].value; }
			else { begin = localcapsuleP[p].key; end = localcapsuleP[p].key + localcapsuleP[p].value; }
			
			EXECUTE_LOOP1C: for(unsigned int k=begin; k<end; k++){
			#pragma HLS LOOP_TRIPCOUNT min=analysis_srcbuffersz max=analysis_srcbuffersz avg=analysis_srcbuffersz	
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("combineSetof1stoSetof40_I2 1", index, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("combineSetof1stoSetof40_I2 2", k, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
				#endif
				
				unsigned int CK = counter % 4;
				
				if(i==0){
					if(CK == 0){
						buffer_setof4[index].data[0] = buffer_setof1M[k];
					} else if(CK == 1){
						buffer_setof4[index].data[1] = buffer_setof1M[k];
					} else if(CK == 2){
						buffer_setof4[index].data[2] = buffer_setof1M[k];
					} else {
						buffer_setof4[index].data[3] = buffer_setof1M[k];
					} 
				} else if(i==1) {
					if(CK == 0){
						buffer_setof4[index].data[0] = buffer_setof1N[k];
					} else if(CK == 1){
						buffer_setof4[index].data[1] = buffer_setof1N[k];
					} else if(CK == 2){
						buffer_setof4[index].data[2] = buffer_setof1N[k];
					} else {
						buffer_setof4[index].data[3] = buffer_setof1N[k];
					}
				} else if(i==2) {
					if(CK == 0){
						buffer_setof4[index].data[0] = buffer_setof1O[k];
					} else if(CK == 1){
						buffer_setof4[index].data[1] = buffer_setof1O[k];
					} else if(CK == 2){
						buffer_setof4[index].data[2] = buffer_setof1O[k];
					} else {
						buffer_setof4[index].data[3] = buffer_setof1O[k];
					}
				} else {
					if(CK == 0){
						buffer_setof4[index].data[0] = buffer_setof1P[k];
					} else if(CK == 1){
						buffer_setof4[index].data[1] = buffer_setof1P[k];
					} else if(CK == 2){
						buffer_setof4[index].data[2] = buffer_setof1P[k];
					} else {
						buffer_setof4[index].data[3] = buffer_setof1P[k];
					}
				}
				counter+=1; if(counter % 4 == 0){ index++; }
			}
		}
		
		localcapsuleR[p].value = localcapsuleM[p].value + localcapsuleN[p].value + localcapsuleO[p].value + localcapsuleP[p].value; 
		if(counter % 4 == 1){ 
						buffer_setof4[index].data[1] = NullKV; buffer_setof4[index].data[2] = NullKV; buffer_setof4[index].data[3] = NullKV; 
						localcapsuleR[p].value += 3; index += 1; } // edge conditions
		else if(counter % 4 == 2){ 
						buffer_setof4[index].data[2] = NullKV; buffer_setof4[index].data[3] = NullKV; 
						localcapsuleR[p].value += 2; index += 1; } // edge conditions
		else if(counter % 4 == 3){ 
						buffer_setof4[index].data[3] = NullKV; 
						localcapsuleR[p].value += 1; index += 1; } // edge conditions
		else {}
		
		#ifdef _DEBUGMODE_STATS
		if(counter % 4 == 1){ 
			actsutilityobj->globalvar_savepartitions_countinvalids(3);
		} else if(counter % 4 == 2){ 
			actsutilityobj->globalvar_savepartitions_countinvalids(2);
		} else if(counter % 4 == 3){ 
			actsutilityobj->globalvar_savepartitions_countinvalids(1);
		} else {}
		#endif
	}
	
	calculateunallignedoffsets(localcapsuleR, NUM_PARTITIONS, 0, 0);
	#ifdef _DEBUGMODE_CHECKS
	cout<<endl;
	actsutilityobj->scankeyvalues("combineSetof1stoSetof40_I2::buffer_setof1M", (keyvalue_t *)buffer_setof1M, localcapsuleM, NUM_PARTITIONS, BATCH_RANGE / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof1stoSetof40_I2::buffer_setof1N", (keyvalue_t *)buffer_setof1N, localcapsuleN, NUM_PARTITIONS, BATCH_RANGE / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof1stoSetof40_I2::buffer_setof1O", (keyvalue_t *)buffer_setof1O, localcapsuleO, NUM_PARTITIONS, BATCH_RANGE / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof1stoSetof40_I2::buffer_setof1P", (keyvalue_t *)buffer_setof1P, localcapsuleP, NUM_PARTITIONS, BATCH_RANGE / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof1stoSetof40_I2::buffer_setof4", (keyvalue_t *)buffer_setof4, localcapsuleR, NUM_PARTITIONS, BATCH_RANGE / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	// exit(EXIT_SUCCESS);
	return;
}

void 
	#ifdef SW 
	actslw::
	#endif
combineSetof1stoSetof40_I3(unsigned int enable, keyvalue_t buffer_setof1M[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof1N[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof1O[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof1P[PADDEDDESTBUFFER_SIZE], uint256_dt buffer_setof4[PADDEDDESTBUFFER_SIZE], keyvalue_t localcapsuleM[NUM_PARTITIONS], keyvalue_t localcapsuleN[NUM_PARTITIONS], keyvalue_t localcapsuleO[NUM_PARTITIONS], keyvalue_t localcapsuleP[NUM_PARTITIONS], keyvalue_t localcapsuleR[NUM_PARTITIONS]){			
	if(enable == OFF){ return; }
	unsigned int analysis_srcbuffersz = SRCBUFFER_SIZE / NUM_PARTITIONS;
	unsigned int index=0, begin=0, end=0;
	keyvalue_t NullKV; NullKV.key = INVALIDDATA; NullKV.value = INVALIDDATA;
	
	resetkeyandvalues(localcapsuleR, NUM_PARTITIONS);
	
	EXECUTE_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS LOOP_TRIPCOUNT min=16 max=16 avg=16	
		unsigned int counter = 0;
		
		EXECUTE_LOOP1B: for(unsigned int i=0; i<4; i++){
			
			if(i==0){ begin = localcapsuleM[p].key; end = localcapsuleM[p].key + localcapsuleM[p].value; }
			else if(i==1){ begin = localcapsuleN[p].key; end = localcapsuleN[p].key + localcapsuleN[p].value; }
			else if(i==2){ begin = localcapsuleO[p].key; end = localcapsuleO[p].key + localcapsuleO[p].value; }
			else { begin = localcapsuleP[p].key; end = localcapsuleP[p].key + localcapsuleP[p].value; }
			
			EXECUTE_LOOP1C: for(unsigned int k=begin; k<end; k++){
			#pragma HLS LOOP_TRIPCOUNT min=analysis_srcbuffersz max=analysis_srcbuffersz avg=analysis_srcbuffersz	
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("combineSetof1stoSetof40_I3 1", index, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("combineSetof1stoSetof40_I3 2", k, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
				#endif
				
				unsigned int CK = counter % 4;
				
				if(i==0){
					if(CK == 0){
						buffer_setof4[index].data[0] = buffer_setof1M[k];
					} else if(CK == 1){
						buffer_setof4[index].data[1] = buffer_setof1M[k];
					} else if(CK == 2){
						buffer_setof4[index].data[2] = buffer_setof1M[k];
					} else {
						buffer_setof4[index].data[3] = buffer_setof1M[k];
					} 
				} else if(i==1) {
					if(CK == 0){
						buffer_setof4[index].data[0] = buffer_setof1N[k];
					} else if(CK == 1){
						buffer_setof4[index].data[1] = buffer_setof1N[k];
					} else if(CK == 2){
						buffer_setof4[index].data[2] = buffer_setof1N[k];
					} else {
						buffer_setof4[index].data[3] = buffer_setof1N[k];
					}
				} else if(i==2) {
					if(CK == 0){
						buffer_setof4[index].data[0] = buffer_setof1O[k];
					} else if(CK == 1){
						buffer_setof4[index].data[1] = buffer_setof1O[k];
					} else if(CK == 2){
						buffer_setof4[index].data[2] = buffer_setof1O[k];
					} else {
						buffer_setof4[index].data[3] = buffer_setof1O[k];
					}
				} else {
					if(CK == 0){
						buffer_setof4[index].data[0] = buffer_setof1P[k];
					} else if(CK == 1){
						buffer_setof4[index].data[1] = buffer_setof1P[k];
					} else if(CK == 2){
						buffer_setof4[index].data[2] = buffer_setof1P[k];
					} else {
						buffer_setof4[index].data[3] = buffer_setof1P[k];
					}
				}
				counter+=1; if(counter % 4 == 0){ index++; }
			}
		}
		
		localcapsuleR[p].value = localcapsuleM[p].value + localcapsuleN[p].value + localcapsuleO[p].value + localcapsuleP[p].value; 
		if(counter % 4 == 1){ 
						buffer_setof4[index].data[1] = NullKV; buffer_setof4[index].data[2] = NullKV; buffer_setof4[index].data[3] = NullKV; 
						localcapsuleR[p].value += 3; index += 1; } // edge conditions
		else if(counter % 4 == 2){ 
						buffer_setof4[index].data[2] = NullKV; buffer_setof4[index].data[3] = NullKV; 
						localcapsuleR[p].value += 2; index += 1; } // edge conditions
		else if(counter % 4 == 3){ 
						buffer_setof4[index].data[3] = NullKV; 
						localcapsuleR[p].value += 1; index += 1; } // edge conditions
		else {}
		
		#ifdef _DEBUGMODE_STATS
		if(counter % 4 == 1){ 
			actsutilityobj->globalvar_savepartitions_countinvalids(3);
		} else if(counter % 4 == 2){ 
			actsutilityobj->globalvar_savepartitions_countinvalids(2);
		} else if(counter % 4 == 3){ 
			actsutilityobj->globalvar_savepartitions_countinvalids(1);
		} else {}
		#endif
	}
	
	calculateunallignedoffsets(localcapsuleR, NUM_PARTITIONS, 0, 0);
	#ifdef _DEBUGMODE_CHECKS
	cout<<endl;
	actsutilityobj->scankeyvalues("combineSetof1stoSetof40_I3::buffer_setof1M", (keyvalue_t *)buffer_setof1M, localcapsuleM, NUM_PARTITIONS, BATCH_RANGE / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof1stoSetof40_I3::buffer_setof1N", (keyvalue_t *)buffer_setof1N, localcapsuleN, NUM_PARTITIONS, BATCH_RANGE / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof1stoSetof40_I3::buffer_setof1O", (keyvalue_t *)buffer_setof1O, localcapsuleO, NUM_PARTITIONS, BATCH_RANGE / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof1stoSetof40_I3::buffer_setof1P", (keyvalue_t *)buffer_setof1P, localcapsuleP, NUM_PARTITIONS, BATCH_RANGE / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof1stoSetof40_I3::buffer_setof4", (keyvalue_t *)buffer_setof4, localcapsuleR, NUM_PARTITIONS, BATCH_RANGE / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	// exit(EXIT_SUCCESS);
	return;
}

void 
	#ifdef SW 
	actslw::
	#endif
combineSetof4stoSetof160_I0(unsigned int enable, uint256_dt buffer_setof4M[PADDEDDESTBUFFER_SIZE], uint256_dt buffer_setof4N[PADDEDDESTBUFFER_SIZE], uint256_dt buffer_setof4O[PADDEDDESTBUFFER_SIZE], uint256_dt buffer_setof4P[PADDEDDESTBUFFER_SIZE], uint1024_dt buffer_setof16[PADDEDDESTBUFFER_SIZE], keyvalue_t localcapsule4M[NUM_PARTITIONS], keyvalue_t localcapsule4N[NUM_PARTITIONS], keyvalue_t localcapsule4O[NUM_PARTITIONS], keyvalue_t localcapsule4P[NUM_PARTITIONS], keyvalue_t localcapsuleR[NUM_PARTITIONS]){			
	if(enable == OFF){ return; }
	unsigned int analysis_srcbuffersz = SRCBUFFER_SIZE / NUM_PARTITIONS;
	unsigned int index=0, begin=0, end=0;
	keyvalue_t NullKV; NullKV.key = INVALIDDATA; NullKV.value = INVALIDDATA;
	
	resetkeyandvalues(localcapsuleR, NUM_PARTITIONS);
	
	EXECUTE_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		unsigned int counter = 0;
		
		EXECUTE_LOOP1B: for(unsigned int i=0; i<4; i++){
			
			if(i==0){ begin = localcapsule4M[p].key / 4; end = (localcapsule4M[p].key + localcapsule4M[p].value) / 4; }
			else if(i==1){ begin = localcapsule4N[p].key / 4; end = (localcapsule4N[p].key + localcapsule4N[p].value) / 4; }
			else if(i==2){ begin = localcapsule4O[p].key / 4; end = (localcapsule4O[p].key + localcapsule4O[p].value) / 4; }
			else { begin = localcapsule4P[p].key / 4; end = (localcapsule4P[p].key + localcapsule4P[p].value) / 4; }
			
			EXECUTE_LOOP1C: for(unsigned int k=begin; k<end; k++){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("combineSetof4stoSetof160_I0 1", index, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("combineSetof4stoSetof160_I0 2", k, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
				#endif
				if(i==0){
					if(counter % 4 == 0){
						buffer_setof16[index].data[0] = buffer_setof4M[k].data[0];
						buffer_setof16[index].data[1] = buffer_setof4M[k].data[1];
						buffer_setof16[index].data[2] = buffer_setof4M[k].data[2];
						buffer_setof16[index].data[3] = buffer_setof4M[k].data[3];
					} else if(counter % 4 == 1){
						buffer_setof16[index].data[4] = buffer_setof4M[k].data[0];
						buffer_setof16[index].data[5] = buffer_setof4M[k].data[1];
						buffer_setof16[index].data[6] = buffer_setof4M[k].data[2];
						buffer_setof16[index].data[7] = buffer_setof4M[k].data[3];
					} else if(counter % 4 == 2){
						buffer_setof16[index].data[8] = buffer_setof4M[k].data[0];
						buffer_setof16[index].data[9] = buffer_setof4M[k].data[1];
						buffer_setof16[index].data[10] = buffer_setof4M[k].data[2];
						buffer_setof16[index].data[11] = buffer_setof4M[k].data[3];
					} else {
						buffer_setof16[index].data[12] = buffer_setof4M[k].data[0];
						buffer_setof16[index].data[13] = buffer_setof4M[k].data[1];
						buffer_setof16[index].data[14] = buffer_setof4M[k].data[2];
						buffer_setof16[index].data[15] = buffer_setof4M[k].data[3];
					} 
				} else if(i==1) {
					if(counter % 4 == 0){
						buffer_setof16[index].data[0] = buffer_setof4N[k].data[0];
						buffer_setof16[index].data[1] = buffer_setof4N[k].data[1];
						buffer_setof16[index].data[2] = buffer_setof4N[k].data[2];
						buffer_setof16[index].data[3] = buffer_setof4N[k].data[3];
					} else if(counter % 4 == 1){
						buffer_setof16[index].data[4] = buffer_setof4N[k].data[0];
						buffer_setof16[index].data[5] = buffer_setof4N[k].data[1];
						buffer_setof16[index].data[6] = buffer_setof4N[k].data[2];
						buffer_setof16[index].data[7] = buffer_setof4N[k].data[3];
					} else if(counter % 4 == 2){
						buffer_setof16[index].data[8] = buffer_setof4N[k].data[0];
						buffer_setof16[index].data[9] = buffer_setof4N[k].data[1];
						buffer_setof16[index].data[10] = buffer_setof4N[k].data[2];
						buffer_setof16[index].data[11] = buffer_setof4N[k].data[3];
					} else {
						buffer_setof16[index].data[12] = buffer_setof4N[k].data[0];
						buffer_setof16[index].data[13] = buffer_setof4N[k].data[1];
						buffer_setof16[index].data[14] = buffer_setof4N[k].data[2];
						buffer_setof16[index].data[15] = buffer_setof4N[k].data[3];
					}
				} else if(i==2) {
					if(counter % 4 == 0){
						buffer_setof16[index].data[0] = buffer_setof4O[k].data[0];
						buffer_setof16[index].data[1] = buffer_setof4O[k].data[1];
						buffer_setof16[index].data[2] = buffer_setof4O[k].data[2];
						buffer_setof16[index].data[3] = buffer_setof4O[k].data[3];
					} else if(counter % 4 == 1){
						buffer_setof16[index].data[4] = buffer_setof4O[k].data[0];
						buffer_setof16[index].data[5] = buffer_setof4O[k].data[1];
						buffer_setof16[index].data[6] = buffer_setof4O[k].data[2];
						buffer_setof16[index].data[7] = buffer_setof4O[k].data[3];
					} else if(counter % 4 == 2){
						buffer_setof16[index].data[8] = buffer_setof4O[k].data[0];
						buffer_setof16[index].data[9] = buffer_setof4O[k].data[1];
						buffer_setof16[index].data[10] = buffer_setof4O[k].data[2];
						buffer_setof16[index].data[11] = buffer_setof4O[k].data[3];
					} else {
						buffer_setof16[index].data[12] = buffer_setof4O[k].data[0];
						buffer_setof16[index].data[13] = buffer_setof4O[k].data[1];
						buffer_setof16[index].data[14] = buffer_setof4O[k].data[2];
						buffer_setof16[index].data[15] = buffer_setof4O[k].data[3];
					}
				} else {
					if(counter % 4 == 0){
						buffer_setof16[index].data[0] = buffer_setof4P[k].data[0];
						buffer_setof16[index].data[1] = buffer_setof4P[k].data[1];
						buffer_setof16[index].data[2] = buffer_setof4P[k].data[2];
						buffer_setof16[index].data[3] = buffer_setof4P[k].data[3];
					} else if(counter % 4 == 1){
						buffer_setof16[index].data[4] = buffer_setof4P[k].data[0];
						buffer_setof16[index].data[5] = buffer_setof4P[k].data[1];
						buffer_setof16[index].data[6] = buffer_setof4P[k].data[2];
						buffer_setof16[index].data[7] = buffer_setof4P[k].data[3];
					} else if(counter % 4 == 2){
						buffer_setof16[index].data[8] = buffer_setof4P[k].data[0];
						buffer_setof16[index].data[9] = buffer_setof4P[k].data[1];
						buffer_setof16[index].data[10] = buffer_setof4P[k].data[2];
						buffer_setof16[index].data[11] = buffer_setof4P[k].data[3];
					} else {
						buffer_setof16[index].data[12] = buffer_setof4P[k].data[0];
						buffer_setof16[index].data[13] = buffer_setof4P[k].data[1];
						buffer_setof16[index].data[14] = buffer_setof4P[k].data[2];
						buffer_setof16[index].data[15] = buffer_setof4P[k].data[3];
					}
				}
				counter+=1; if(counter % 4 == 0){ index++; }
			}
		}
		
		localcapsuleR[p].value = localcapsule4M[p].value + localcapsule4N[p].value + localcapsule4O[p].value + localcapsule4P[p].value; 
		if(counter % 4 == 1){ 
						buffer_setof16[index].data[4] = NullKV; buffer_setof16[index].data[5] = NullKV; buffer_setof16[index].data[6] = NullKV; buffer_setof16[index].data[7] = NullKV;  
						buffer_setof16[index].data[8] = NullKV; buffer_setof16[index].data[9] = NullKV; buffer_setof16[index].data[10] = NullKV; buffer_setof16[index].data[11] = NullKV;  
						buffer_setof16[index].data[12] = NullKV; buffer_setof16[index].data[13] = NullKV; buffer_setof16[index].data[14] = NullKV; buffer_setof16[index].data[15] = NullKV;  
						localcapsuleR[p].value += 12; index += 1; } // edge conditions ?FIXME-INCORRECT?
		else if(counter % 4 == 2){ 
						buffer_setof16[index].data[8] = NullKV; buffer_setof16[index].data[9] = NullKV; buffer_setof16[index].data[10] = NullKV; buffer_setof16[index].data[11] = NullKV;  
						buffer_setof16[index].data[12] = NullKV; buffer_setof16[index].data[13] = NullKV; buffer_setof16[index].data[14] = NullKV; buffer_setof16[index].data[15] = NullKV;  
						localcapsuleR[p].value += 8; index += 1; } // edge conditions ?FIXME-INCORRECT?
		else if(counter % 4 == 3){ 
						buffer_setof16[index].data[12] = NullKV; buffer_setof16[index].data[13] = NullKV; buffer_setof16[index].data[14] = NullKV; buffer_setof16[index].data[15] = NullKV;  
						localcapsuleR[p].value += 4; index += 1; } // edge conditions ?FIXME-INCORRECT?	
		else {}
		
		#ifdef _DEBUGMODE_STATS
		if(counter % 4 == 1){ 
			actsutilityobj->globalvar_savepartitions_countinvalids(12);
		} else if(counter % 4 == 2){ 
			actsutilityobj->globalvar_savepartitions_countinvalids(8);
		} else if(counter % 4 == 3){ 
			actsutilityobj->globalvar_savepartitions_countinvalids(4);
		} else {}
		#endif
	}
	
	calculateunallignedoffsets(localcapsuleR, NUM_PARTITIONS, 0, 0);
	#ifdef _DEBUGMODE_CHECKS
	cout<<endl;
	actsutilityobj->scankeyvalues("combineSetof4stoSetof160_I0::buffer_setof4M", (keyvalue_t *)buffer_setof4M, localcapsule4M, NUM_PARTITIONS, BATCH_RANGE / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof4stoSetof160_I0::buffer_setof4N", (keyvalue_t *)buffer_setof4N, localcapsule4N, NUM_PARTITIONS, BATCH_RANGE / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof4stoSetof160_I0::buffer_setof4O", (keyvalue_t *)buffer_setof4O, localcapsule4O, NUM_PARTITIONS, BATCH_RANGE / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof4stoSetof160_I0::buffer_setof4P", (keyvalue_t *)buffer_setof4P, localcapsule4P, NUM_PARTITIONS, BATCH_RANGE / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof4stoSetof160_I0::buffer_setof16", (keyvalue_t *)buffer_setof16, localcapsuleR, NUM_PARTITIONS, BATCH_RANGE / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	// exit(EXIT_SUCCESS);
	return;
}

// group functions
void 
	#ifdef SW 
	actslw::
	#endif
combineSetof1stoSetof4s0(unsigned int enable, keyvalue_t buffer_setof1[16][PADDEDDESTBUFFER_SIZE], uint256_dt buffer_setof4[4][PADDEDDESTBUFFER_SIZE], keyvalue_t templocalcapsule_so1[16][NUM_PARTITIONS], keyvalue_t templocalcapsule_so4[4][NUM_PARTITIONS]){
	#pragma HLS INLINE
	// 1s->4s
	combineSetof1stoSetof40_I0(enable, buffer_setof1[0], buffer_setof1[1], buffer_setof1[2], buffer_setof1[3], buffer_setof4[0], templocalcapsule_so1[0], templocalcapsule_so1[1], templocalcapsule_so1[2], templocalcapsule_so1[3], templocalcapsule_so4[0]);
	combineSetof1stoSetof40_I1(enable, buffer_setof1[4], buffer_setof1[5], buffer_setof1[6], buffer_setof1[7], buffer_setof4[1], templocalcapsule_so1[4], templocalcapsule_so1[5], templocalcapsule_so1[6], templocalcapsule_so1[7], templocalcapsule_so4[1]);
	combineSetof1stoSetof40_I2(enable, buffer_setof1[8], buffer_setof1[9], buffer_setof1[10], buffer_setof1[11], buffer_setof4[2], templocalcapsule_so1[8], templocalcapsule_so1[9], templocalcapsule_so1[10], templocalcapsule_so1[11], templocalcapsule_so4[2]);
	combineSetof1stoSetof40_I3(enable, buffer_setof1[12], buffer_setof1[13], buffer_setof1[14], buffer_setof1[15], buffer_setof4[3], templocalcapsule_so1[12], templocalcapsule_so1[13], templocalcapsule_so1[14], templocalcapsule_so1[15], templocalcapsule_so4[3]);
	// if(enable == ON){ exit(EXIT_SUCCESS); }
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif
combineSetof4stoSetof16s0(unsigned int enable, uint256_dt buffer_setof4[4][PADDEDDESTBUFFER_SIZE], uint1024_dt buffer_setof16[PADDEDDESTBUFFER_SIZE], keyvalue_t templocalcapsule_so4[4][NUM_PARTITIONS], keyvalue_t templocalcapsule_so16[NUM_PARTITIONS]){
	#pragma HLS INLINE
	// 4s->8s
	combineSetof4stoSetof160_I0(enable, buffer_setof4[0], buffer_setof4[1], buffer_setof4[2], buffer_setof4[3], buffer_setof16, templocalcapsule_so4[0], templocalcapsule_so4[1], templocalcapsule_so4[2], templocalcapsule_so4[3], templocalcapsule_so16);
	return;
}

// main function
void 
	#ifdef SW 
	actslw::
	#endif 
dispatch0(uint512_dt * kvdram1, uint512_dt * kvdram2){
	unsigned int analysis_mainloop = KVDATA_BATCHSIZE_KVS / (NUMPIPELINES * SRCBUFFER_SIZE);
	unsigned int analysis_numllops = TREE_DEPTH;
	unsigned int analysis_numsourcepartitions = 1;
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->printparameters();
	actsutilityobj->printglobalvars();
	actsutilityobj->clearglobalvars();
	#endif
	
	keyvalue_t sourcebuffer[16][SRCBUFFER_SIZE];
	#pragma HLS array_partition variable = sourcebuffer
	
	keyvalue_t buffer_setof1[16][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer_setof1
	keyvalue_t templocalcapsule_so1[16][NUM_PARTITIONS];
	#pragma HLS array_partition variable = templocalcapsule_so1
	
	uint256_dt buffer_setof4[4][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer_setof4
	keyvalue_t templocalcapsule_so4[4][NUM_PARTITIONS];
	#pragma HLS array_partition variable = templocalcapsule_so4
	
	uint1024_dt buffer_setof16[PADDEDDESTBUFFER_SIZE];
	keyvalue_t templocalcapsule_so16[NUM_PARTITIONS];
	
	keyvalue_t globaldestoffsets[NUM_PARTITIONS];
	keyvalue_t globalstatsbuffer[PADDEDDESTBUFFER_SIZE];
	
	globalparams_t globalparams = getglobalparams(kvdram1, kvdram2);
	sweepparams_t sweepparams;
	resetmanykeyandvalues(buffer_setof1, PADDEDDESTBUFFER_SIZE);

	// start launch
	MAIN_LOOP1: for(unsigned int currentLOP=0; currentLOP<=globalparams.treedepth; currentLOP++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_numllops avg=analysis_numllops	
	
		config_t config = getconfig(currentLOP);
		unsigned int num_source_partitions = get_num_source_partitions(currentLOP);
		
		MAIN_LOOP1B: for(unsigned int source_partition=0; source_partition<num_source_partitions; source_partition+=1){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_numsourcepartitions avg=analysis_numsourcepartitions	
		
			sweepparams = getsweepparams(globalparams, currentLOP, source_partition);
			travstate_t travstate = gettravstate(kvdram1, kvdram2, globalstatsbuffer, globalparams, config, currentLOP, source_partition, num_source_partitions);
			travstate_t travstatepp1 = travstate; 
			travstate_t travstatepp2 = travstate;
			#ifdef _DEBUGMODE_KERNELPRINTS2
			actsutilityobj->printpartitionstep(config, sweepparams, travstate, 0);
			actsutilityobj->setstructs(config, sweepparams, travstate);
			#endif
			
			// launch partition
			unsigned int destskipsize = getskipsize(sweepparams.currentLOP, DEST);
			readglobalstats0(config.enablepartition, kvdram1, kvdram2, globalstatsbuffer, globaldestoffsets, (sweepparams.source_partition * destskipsize * NUM_PARTITIONS), sweepparams.currentLOP, DEST);
			readvertices0(config.enablereduce, kvdram1, kvdram2, buffer_setof1, (BASEOFFSET_VERTICESDATA_KVS + sweepparams.upperlimit));
			
			MAIN_LOOP1C: for(unsigned int offset_kvs=travstate.begin_kvs; offset_kvs<travstate.end_kvs; offset_kvs+=travstate.skip_kvs){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_mainloop avg=analysis_mainloop	
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print4("### dispatch0:: offset_kvs", "begin_kvs", "end_kvs", "skip", offset_kvs, travstate.begin_kvs, travstate.end_kvs, SRCBUFFER_SIZE);
				#endif
				
				travstate.i_kvs = offset_kvs;
				travstatepp1.i_kvs = offset_kvs + SRCBUFFER_SIZE;
				travstatepp2.i_kvs = offset_kvs + (2 * SRCBUFFER_SIZE);
				
				readkeyvalues0(ON, kvdram1, kvdram2, sourcebuffer, (sweepparams.worksourcebaseaddress_kvs + offset_kvs), travstate);
				
				partitionkeyvalues0(config.enablepartition, sourcebuffer, buffer_setof1, templocalcapsule_so1, sweepparams.currentLOP, sweepparams.upperlimit, travstate);
				
				// 1s->2s
				combineSetof1stoSetof4s0(config.enablepartition, buffer_setof1, buffer_setof4, templocalcapsule_so1, templocalcapsule_so4);
				
				// 4s->16s
				combineSetof4stoSetof16s0(config.enablepartition, buffer_setof4, buffer_setof16, templocalcapsule_so4, templocalcapsule_so16);
				
				savekeyvalues0(config.enablepartition, kvdram1, kvdram2, buffer_setof16, globaldestoffsets, templocalcapsule_so16, sweepparams.workdestbaseaddress_kvs);
				
				collectglobalstats0(config.enablecollectglobalstats, sourcebuffer, buffer_setof1, sweepparams.upperlimit);
				
				reduce0(config.enablereduce, sourcebuffer, buffer_setof1, sweepparams.upperlimit, globalparams.GraphIter, globalparams.GraphAlgo, travstate);
			}
			
			prepareglobalstats0(config.enablecollectglobalstats, buffer_setof1);
			saveglobalstats0(config.enablecollectglobalstats, kvdram1, kvdram2, globalstatsbuffer, buffer_setof1);
			updateglobalstats0(config.enablepartition, kvdram1, globalstatsbuffer, globaldestoffsets, (sweepparams.source_partition * destskipsize * NUM_PARTITIONS), sweepparams.currentLOP, DEST);
			savevertices0(config.enablereduce, kvdram1, kvdram2, buffer_setof1, (BASEOFFSET_VERTICESDATA_KVS + sweepparams.upperlimit));
			
			#ifdef _DEBUGMODE_KERNELPRINTS2
			actsutilityobj->printpartitionresult(ON, kvdram1, globaldestoffsets, globalstatsbuffer, sweepparams);
			#endif
		}
		#ifdef _DEBUGMODE_KERNELPRINTS2
		actsutilityobj->printglobalvars();
		actsutilityobj->clearglobalvars();
		#endif
	}
	return;
}

// top
extern "C" {
void 
	#ifdef SW 
	actslw:: 
	#endif
topkernel( uint512_dt * sourceAvolume0, uint512_dt * sourceAvolume1 ){
#pragma HLS INTERFACE m_axi port = sourceAvolume0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = sourceAvolume1 offset = slave bundle = gmem1

#pragma HLS INTERFACE s_axilite port = sourceAvolume0 bundle = control
#pragma HLS INTERFACE s_axilite port = sourceAvolume1 bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATA_PACK variable = sourceAvolume0
#pragma HLS DATA_PACK variable = sourceAvolume1

	#ifdef _DEBUGMODE_KERNELPRINTS2
	cout<<"Light weight ACTS started."<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"... Acts Launched... size (v0): "<<sourceAvolume0[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key<<endl; 
	cout<<"... Acts Launched... size (v1): "<<sourceAvolume1[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key<<endl; 
	#endif 
	
	dispatch0(sourceAvolume0, sourceAvolume1);
	return;
}
}







