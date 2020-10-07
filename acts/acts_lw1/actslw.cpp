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

// kernel utilities
unsigned int 
	#ifdef SW 
	actslw::
	#endif 
GET_KVDATA_RANGE_PERSSDPARTITION_POW(unsigned int groupid){
	if(groupid == LOWINDEGREESGROUPID){ return KVDATA_RANGE_PERSSDPARTITION_POW; }
	else if(groupid == HIGHINDEGREESGROUPID){ return KVDATA_RANGE_PERSSDPARTITION2_POW; }
	else{ return KVDATA_RANGE_PERSSDPARTITION_POW; }
}
unsigned int 
	#ifdef SW 
	actslw::
	#endif 
GET_KVDATA_RANGE_PERSSDPARTITION(unsigned int groupid){
	if(groupid == LOWINDEGREESGROUPID){ return KVDATA_RANGE_PERSSDPARTITION; }
	else if(groupid == HIGHINDEGREESGROUPID){ return KVDATA_RANGE_PERSSDPARTITION2; }
	else{ return KVDATA_RANGE_PERSSDPARTITION; }
}
unsigned int 
	#ifdef SW 
	actslw::
	#endif 
GET_KVDATA_RANGE_PERSSDPARTITION_KVS(unsigned int groupid){
	if(groupid == LOWINDEGREESGROUPID){ return KVDATA_RANGE_PERSSDPARTITION_KVS; }
	else if(groupid == HIGHINDEGREESGROUPID){ return KVDATA_RANGE_PERSSDPARTITION2_KVS; }
	else{ return KVDATA_RANGE_PERSSDPARTITION_KVS; }
}
unsigned int 
	#ifdef SW 
	actslw::
	#endif 
GET_BATCH_RANGE_POW(unsigned int groupid){
	if(groupid == LOWINDEGREESGROUPID){ return BATCH_RANGE_POW; }
	else if(groupid == HIGHINDEGREESGROUPID){ return BATCH_RANGE2_POW; }
	else{ return BATCH_RANGE_POW; }
}
unsigned int 
	#ifdef SW 
	actslw::
	#endif 
GET_BATCH_RANGE(unsigned int groupid){
	if(groupid == LOWINDEGREESGROUPID){ return BATCH_RANGE; }
	else if(groupid == HIGHINDEGREESGROUPID){ return BATCH_RANGE2; }
	else{ return BATCH_RANGE; }
}
unsigned int 
	#ifdef SW 
	actslw::
	#endif 
GET_BATCH_RANGE_KVS(unsigned int groupid){
	if(groupid == LOWINDEGREESGROUPID){ return BATCH_RANGE_KVS; }
	else if(groupid == HIGHINDEGREESGROUPID){ return BATCH_RANGE2_KVS; }
	else{ return BATCH_RANGE_KVS; }
}
unsigned int 
	#ifdef SW 
	actslw::
	#endif 
GET_TREE_DEPTH(unsigned int groupid){
	if(groupid == LOWINDEGREESGROUPID){ return TREE_DEPTH; }
	else if(groupid == HIGHINDEGREESGROUPID){ return TREE_DEPTH2; }
	else{ return TREE_DEPTH; }
}
unsigned int 
	#ifdef SW 
	actslw::
	#endif 
GET_APPLYVERTEXBUFFERSZ(unsigned int groupid){
	if(groupid == LOWINDEGREESGROUPID){ return APPLYVERTEXBUFFERSZ; }
	else if(groupid == HIGHINDEGREESGROUPID){ return APPLYVERTEXBUFFERSZ2; }
	else{ return APPLYVERTEXBUFFERSZ; }
}
unsigned int 
	#ifdef SW 
	actslw::
	#endif 
GET_APPLYVERTEXBUFFERSZ_KVS(unsigned int groupid){
	if(groupid == LOWINDEGREESGROUPID){ return APPLYVERTEXBUFFERSZ_KVS; }
	else if(groupid == HIGHINDEGREESGROUPID){ return APPLYVERTEXBUFFERSZ2_KVS; }
	else{ return APPLYVERTEXBUFFERSZ_KVS; }
}
unsigned int 
	#ifdef SW 
	actslw::
	#endif 
GET_NUMLASTLEVELPARTITIONS(unsigned int groupid){
	if(groupid == LOWINDEGREESGROUPID){ return NUMLASTLEVELPARTITIONS; }
	else if(groupid == HIGHINDEGREESGROUPID){ return NUMLASTLEVELPARTITIONS2; }
	else{ return NUMLASTLEVELPARTITIONS; }
}
buffer_type
	#ifdef SW 
	actslw::
	#endif 
allignhigher_KV(buffer_type val){
	buffer_type fac = (val + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	return (fac * VECTOR_SIZE);
}
batch_type
	#ifdef SW 
	actslw::
	#endif
getskipsize(step_type currentLOP, bool_type sourceORdest, globalparams_t globalparams){
	analysis_type analysis_treedepth = TREE_DEPTH;
	batch_type result;
	
	if(currentLOP == 0){ currentLOP = 1; }
	if(sourceORdest == SOURCE){ result = GET_NUMLASTLEVELPARTITIONS(globalparams.groupid); }
	else if (sourceORdest == DEST){ result = GET_NUMLASTLEVELPARTITIONS(globalparams.groupid) / NUM_PARTITIONS; }
	else {}
	for(step_type i=0; i<(currentLOP-1); i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_treedepth avg=analysis_treedepth
		result = result / NUM_PARTITIONS;
	}
	return result;
}
void 
	#ifdef SW 
	actslw::
	#endif 
copykeyandvalues(keyvalue_t * buffer1, keyvalue_t * buffer2, buffer_type size){
	for(partition_type i=0; i<size; i++){ buffer1[i] = buffer2[i]; }
}
void 
	#ifdef SW 
	actslw::
	#endif 
resetkeyandvalues(skeyvalue_t * buffer, buffer_type size){
	for(buffer_type i=0; i<size; i++){ buffer[i].key = 0; buffer[i].value = 0; }
}
void 
	#ifdef SW 
	actslw::
	#endif 
resetvalues(keyvalue_t * buffer, buffer_type size){
	for(buffer_type i=0; i<size; i++){ buffer[i].value = 0; }
}
void 
	#ifdef SW 
	actslw::
	#endif 
resetmanykeyandvalues(skeyvalue_t buffer[VECTOR_SIZE][NUM_PARTITIONS], buffer_type size){
	for(buffer_type i=0; i<size; i++){
		buffer[0][i].key = 0; buffer[0][i].value = 0; 
		buffer[1][i].key = 0; buffer[1][i].value = 0; 
		buffer[2][i].key = 0; buffer[2][i].value = 0; 
		buffer[3][i].key = 0; buffer[3][i].value = 0; 
		buffer[4][i].key = 0; buffer[4][i].value = 0; 
		buffer[5][i].key = 0; buffer[5][i].value = 0; 
		buffer[6][i].key = 0; buffer[6][i].value = 0; 
		buffer[7][i].key = 0; buffer[7][i].value = 0; 
	}
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif 
resetmanykeyandvalues(keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], buffer_type size){
	for(buffer_type i=0; i<size; i++){
		buffer[0][i].key = 0; buffer[0][i].value = 0; 
		buffer[1][i].key = 0; buffer[1][i].value = 0; 
		buffer[2][i].key = 0; buffer[2][i].value = 0; 
		buffer[3][i].key = 0; buffer[3][i].value = 0; 
		buffer[4][i].key = 0; buffer[4][i].value = 0; 
		buffer[5][i].key = 0; buffer[5][i].value = 0; 
		buffer[6][i].key = 0; buffer[6][i].value = 0; 
		buffer[7][i].key = 0; buffer[7][i].value = 0; 
	}
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif 
resetmanykeyandvalues(keyvalue_t buffer[NUM_PARTITIONS], buffer_type size){
	for(buffer_type i=0; i<size; i++){
		buffer[i].key = 0; buffer[i].value = 0; 
	}
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif 
resetmanyvalues(skeyvalue_t buffer[VECTOR_SIZE][NUM_PARTITIONS], buffer_type size){
	for(buffer_type i=0; i<size; i++){ 
		buffer[0][i].value = 0;
		buffer[1][i].value = 0;
		buffer[2][i].value = 0;
		buffer[3][i].value = 0;
		buffer[4][i].value = 0;
		buffer[5][i].value = 0;
		buffer[6][i].value = 0;
		buffer[7][i].value = 0;
	}
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif
accumkeysandvalues(keyvalue_t * buffer1, keyvalue_t * buffer2, buffer_type size){
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
accumkeysandvalues(keyvalue_t * buffer1, keyvalue_t * buffer2, keyvalue_t * buffer3, buffer_type size){
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
getchunksize(buffer_type buffer_size, travstate_t travstate, buffer_type localoffset){
	buffer_type chunk_size = buffer_size;
	batch_type i = travstate.i_kvs + localoffset;
	if (i > travstate.end_kvs){ chunk_size = 0; }
	else if ((i + buffer_size) > travstate.end_kvs){ chunk_size = travstate.end_kvs - i; }
	else {}
	return chunk_size;
}
partition_type
	#ifdef SW 
	actslw::
	#endif 
getpartition(keyvalue_t keyvalue, step_type currentLOP, vertex_t upperlimit, unsigned int batch_range_pow, unsigned int groupid){
	// partition_type partition = ((keyvalue.key - upperlimit) >> (BATCH_RANGE_POW - (NUM_PARTITIONS_POW * currentLOP)));
	partition_type partition = ((keyvalue.key - upperlimit) >> (batch_range_pow - (NUM_PARTITIONS_POW * currentLOP)));
	
	#ifdef _DEBUGMODE_CHECKS2
	if(partition >= NUM_PARTITIONS){ actsutilityobj->globalstats_counterrorsingetpartition(1); }
	#endif 
	
	#ifdef ENABLE_PERFECTACCURACY
		#ifdef _DEBUGMODE_CHECKS2
		if(partition >= NUM_PARTITIONS){ cout<<"getpartition::ERROR 1. partition out of bounds partition: "<<partition<<", keyvalue.key: "<<keyvalue.key<<", NUM_PARTITIONS: "<<NUM_PARTITIONS<<", keyvalue.key: "<<keyvalue.key<<", upperlimit: "<<upperlimit<<", currentLOP: "<<currentLOP<<", batch_range_pow: "<<batch_range_pow<<endl; exit(EXIT_FAILURE); }
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
buffer_type 
	#ifdef SW 
	actslw::
	#endif 
getglobalpartition(keyvalue_t keyvalue, vertex_t upperlimit, unsigned int batch_range_pow, unsigned int groupid){
	// partition_type globalpartition = ((keyvalue.key - upperlimit) >> (BATCH_RANGE_POW - (NUM_PARTITIONS_POW * TREE_DEPTH)));
	// partition_type globalpartition = ((keyvalue.key - upperlimit) >> (GET_BATCH_RANGE_POW(groupid) - (NUM_PARTITIONS_POW * GET_TREE_DEPTH(groupid))));
	partition_type globalpartition = ((keyvalue.key - upperlimit) >> (batch_range_pow - (NUM_PARTITIONS_POW * GET_TREE_DEPTH(groupid))));
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("actslw::getglobalpartition", globalpartition, GET_NUMLASTLEVELPARTITIONS(groupid), keyvalue.key, upperlimit, NAp);
	#endif
	return globalpartition;
}
value_t 
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
value_t 
	#ifdef SW 
	actslw::
	#endif 
mergefunc(value_t value1, value_t value2, unsigned int GraphAlgo){
	// #pragma HLS INLINE
	value_t ret = 0;
	#ifdef PR_ALGORITHM
	ret = value1 + value2;
	#elif defined(BFS_ALGORITHM)
	ret = min(value1, value2);
	#elif defined(BC_ALGORITHM)
	ret = min(value1, value2);
	#endif
	return ret;
}
void 
	#ifdef SW 
	actslw::
	#endif 
copykeyvalues(keyvalue_t * buffer1, keyvalue_t * buffer2, buffer_type size){
	COPYKEYS_LOOP: for(buffer_type i=0; i<size; i++){ buffer1[i] = buffer2[i]; }
}
buffer_type 
	#ifdef SW 
	actslw::
	#endif 
getpartitionwritesz_original(buffer_type realsize_kvs, buffer_type bramoffset_kvs){
	#pragma HLS INLINE // REMOVEME.
	// #pragma HLS INLINE OFF
	buffer_type size_kvs;
	
	#ifdef ENABLE_APPROXIMATEPARTITIONWRITES
if(realsize_kvs >= 0*PADSKIP && realsize_kvs < 1*PADSKIP){
		size_kvs = 1*PADSKIP;
	}
else if(realsize_kvs >= 1*PADSKIP && realsize_kvs < 2*PADSKIP){
		size_kvs = 2*PADSKIP;
	}
else if(realsize_kvs >= 2*PADSKIP && realsize_kvs < 3*PADSKIP){
		size_kvs = 3*PADSKIP;
	}
else if(realsize_kvs >= 3*PADSKIP && realsize_kvs < 4*PADSKIP){
		size_kvs = 4*PADSKIP;
	}
else if(realsize_kvs >= 4*PADSKIP && realsize_kvs < 5*PADSKIP){
		size_kvs = 5*PADSKIP;
	}
else if(realsize_kvs >= 5*PADSKIP && realsize_kvs < 6*PADSKIP){
		size_kvs = 6*PADSKIP;
	}
else if(realsize_kvs >= 6*PADSKIP && realsize_kvs < 7*PADSKIP){
		size_kvs = 7*PADSKIP;
	}
else if(realsize_kvs >= 7*PADSKIP && realsize_kvs < 8*PADSKIP){
		size_kvs = 8*PADSKIP;
	}
else if(realsize_kvs >= 8*PADSKIP && realsize_kvs < 9*PADSKIP){
		size_kvs = 9*PADSKIP;
	}
else if(realsize_kvs >= 9*PADSKIP && realsize_kvs < 10*PADSKIP){
		size_kvs = 10*PADSKIP;
	}
else if(realsize_kvs >= 10*PADSKIP && realsize_kvs < 11*PADSKIP){
		size_kvs = 11*PADSKIP;
	}
else if(realsize_kvs >= 11*PADSKIP && realsize_kvs < 12*PADSKIP){
		size_kvs = 12*PADSKIP;
	}
else if(realsize_kvs >= 12*PADSKIP && realsize_kvs < 13*PADSKIP){
		size_kvs = 13*PADSKIP;
	}
else if(realsize_kvs >= 13*PADSKIP && realsize_kvs < 14*PADSKIP){
		size_kvs = 14*PADSKIP;
	}
else if(realsize_kvs >= 14*PADSKIP && realsize_kvs < 15*PADSKIP){
		size_kvs = 15*PADSKIP;
	}
else if(realsize_kvs >= 15*PADSKIP && realsize_kvs < 16*PADSKIP){
		size_kvs = 16*PADSKIP;
	}
else if(realsize_kvs >= 16*PADSKIP && realsize_kvs < 17*PADSKIP){
		size_kvs = 17*PADSKIP;
	}
else if(realsize_kvs >= 17*PADSKIP && realsize_kvs < 18*PADSKIP){
		size_kvs = 18*PADSKIP;
	}
else if(realsize_kvs >= 18*PADSKIP && realsize_kvs < 19*PADSKIP){
		size_kvs = 19*PADSKIP;
	}
else if(realsize_kvs >= 19*PADSKIP && realsize_kvs < 20*PADSKIP){
		size_kvs = 20*PADSKIP;
	}
else if(realsize_kvs >= 20*PADSKIP && realsize_kvs < 21*PADSKIP){
		size_kvs = 21*PADSKIP;
	}
else if(realsize_kvs >= 21*PADSKIP && realsize_kvs < 22*PADSKIP){
		size_kvs = 22*PADSKIP;
	}
else if(realsize_kvs >= 22*PADSKIP && realsize_kvs < 23*PADSKIP){
		size_kvs = 23*PADSKIP;
	}
else if(realsize_kvs >= 23*PADSKIP && realsize_kvs < 24*PADSKIP){
		size_kvs = 24*PADSKIP;
	}
else if(realsize_kvs >= 24*PADSKIP && realsize_kvs < 25*PADSKIP){
		size_kvs = 25*PADSKIP;
	}
else if(realsize_kvs >= 25*PADSKIP && realsize_kvs < 26*PADSKIP){
		size_kvs = 26*PADSKIP;
	}
else if(realsize_kvs >= 26*PADSKIP && realsize_kvs < 27*PADSKIP){
		size_kvs = 27*PADSKIP;
	}
else if(realsize_kvs >= 27*PADSKIP && realsize_kvs < 28*PADSKIP){
		size_kvs = 28*PADSKIP;
	}
else if(realsize_kvs >= 28*PADSKIP && realsize_kvs < 29*PADSKIP){
		size_kvs = 29*PADSKIP;
	}
else if(realsize_kvs >= 29*PADSKIP && realsize_kvs < 30*PADSKIP){
		size_kvs = 30*PADSKIP;
	}
else if(realsize_kvs >= 30*PADSKIP && realsize_kvs < 31*PADSKIP){
		size_kvs = 31*PADSKIP;
	}
else if(realsize_kvs >= 31*PADSKIP && realsize_kvs < 32*PADSKIP){
		size_kvs = 32*PADSKIP;
	}
else if(realsize_kvs >= 32*PADSKIP && realsize_kvs < 33*PADSKIP){
		size_kvs = 33*PADSKIP;
	}
else if(realsize_kvs >= 33*PADSKIP && realsize_kvs < 34*PADSKIP){
		size_kvs = 34*PADSKIP;
	}
else if(realsize_kvs >= 34*PADSKIP && realsize_kvs < 35*PADSKIP){
		size_kvs = 35*PADSKIP;
	}
else if(realsize_kvs >= 35*PADSKIP && realsize_kvs < 36*PADSKIP){
		size_kvs = 36*PADSKIP;
	}
else if(realsize_kvs >= 36*PADSKIP && realsize_kvs < 37*PADSKIP){
		size_kvs = 37*PADSKIP;
	}
else if(realsize_kvs >= 37*PADSKIP && realsize_kvs < 38*PADSKIP){
		size_kvs = 38*PADSKIP;
	}
else if(realsize_kvs >= 38*PADSKIP && realsize_kvs < 39*PADSKIP){
		size_kvs = 39*PADSKIP;
	}
else if(realsize_kvs >= 39*PADSKIP && realsize_kvs < 40*PADSKIP){
		size_kvs = 40*PADSKIP;
	}
else if(realsize_kvs >= 40*PADSKIP && realsize_kvs < 41*PADSKIP){
		size_kvs = 41*PADSKIP;
	}
else if(realsize_kvs >= 41*PADSKIP && realsize_kvs < 42*PADSKIP){
		size_kvs = 42*PADSKIP;
	}
else if(realsize_kvs >= 42*PADSKIP && realsize_kvs < 43*PADSKIP){
		size_kvs = 43*PADSKIP;
	}
else if(realsize_kvs >= 43*PADSKIP && realsize_kvs < 44*PADSKIP){
		size_kvs = 44*PADSKIP;
	}
else if(realsize_kvs >= 44*PADSKIP && realsize_kvs < 45*PADSKIP){
		size_kvs = 45*PADSKIP;
	}
else if(realsize_kvs >= 45*PADSKIP && realsize_kvs < 46*PADSKIP){
		size_kvs = 46*PADSKIP;
	}
else if(realsize_kvs >= 46*PADSKIP && realsize_kvs < 47*PADSKIP){
		size_kvs = 47*PADSKIP;
	}
else if(realsize_kvs >= 47*PADSKIP && realsize_kvs < 48*PADSKIP){
		size_kvs = 48*PADSKIP;
	}
else if(realsize_kvs >= 48*PADSKIP && realsize_kvs < 49*PADSKIP){
		size_kvs = 49*PADSKIP;
	}
else if(realsize_kvs >= 49*PADSKIP && realsize_kvs < 50*PADSKIP){
		size_kvs = 50*PADSKIP;
	}
else if(realsize_kvs >= 50*PADSKIP && realsize_kvs < 51*PADSKIP){
		size_kvs = 51*PADSKIP;
	}
else if(realsize_kvs >= 51*PADSKIP && realsize_kvs < 52*PADSKIP){
		size_kvs = 52*PADSKIP;
	}
else if(realsize_kvs >= 52*PADSKIP && realsize_kvs < 53*PADSKIP){
		size_kvs = 53*PADSKIP;
	}
else if(realsize_kvs >= 53*PADSKIP && realsize_kvs < 54*PADSKIP){
		size_kvs = 54*PADSKIP;
	}
else if(realsize_kvs >= 54*PADSKIP && realsize_kvs < 55*PADSKIP){
		size_kvs = 55*PADSKIP;
	}
else if(realsize_kvs >= 55*PADSKIP && realsize_kvs < 56*PADSKIP){
		size_kvs = 56*PADSKIP;
	}
else if(realsize_kvs >= 56*PADSKIP && realsize_kvs < 57*PADSKIP){
		size_kvs = 57*PADSKIP;
	}
else if(realsize_kvs >= 57*PADSKIP && realsize_kvs < 58*PADSKIP){
		size_kvs = 58*PADSKIP;
	}
else if(realsize_kvs >= 58*PADSKIP && realsize_kvs < 59*PADSKIP){
		size_kvs = 59*PADSKIP;
	}
else if(realsize_kvs >= 59*PADSKIP && realsize_kvs < 60*PADSKIP){
		size_kvs = 60*PADSKIP;
	}
else if(realsize_kvs >= 60*PADSKIP && realsize_kvs < 61*PADSKIP){
		size_kvs = 61*PADSKIP;
	}
else if(realsize_kvs >= 61*PADSKIP && realsize_kvs < 62*PADSKIP){
		size_kvs = 62*PADSKIP;
	}
else if(realsize_kvs >= 62*PADSKIP && realsize_kvs < 63*PADSKIP){
		size_kvs = 63*PADSKIP;
	}
else if(realsize_kvs >= 63*PADSKIP && realsize_kvs < 64*PADSKIP){
		size_kvs = 64*PADSKIP;
	}
	else {
		#ifdef ENABLE_PERFECTACCURACY
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"WARNING:getpartitionwritesz: should not get here. something might be wrong. realsize_kvs: "<<realsize_kvs<<", size_kvs: "<<size_kvs<<", last size_kvs: "<<(64*PADSKIP)<<", OPTIMALSIZE: "<<OPTIMALSIZE<<", PADSKIP: "<<PADSKIP<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl;				 
		exit(EXIT_FAILURE);
		#endif
		#else 
		// size_kvs = PADDEDDESTBUFFER_SIZE;
		size_kvs = OPTIMALSIZE;
		#endif 
	}
	#ifdef SW 
	if((bramoffset_kvs + size_kvs) >= PADDEDDESTBUFFER_SIZE){ size_kvs = PADDEDDESTBUFFER_SIZE - bramoffset_kvs - 1; } 
	#endif 
	#else 
	size_kvs = realsize_kvs;
	#endif
	return size_kvs;
}
buffer_type 
	#ifdef SW 
	actslw::
	#endif 
getpartitionwritesz_original2(buffer_type realsize_kvs, buffer_type bramoffset_kvs){
	#pragma HLS INLINE // REMOVEME.
	// #pragma HLS INLINE OFF
	buffer_type size_kvs;
	
	#ifdef ENABLE_APPROXIMATEPARTITIONWRITES
if(realsize_kvs >= 0*8 && realsize_kvs < 1*8){
		size_kvs = 1*8;
	}
else if(realsize_kvs >= 1*8 && realsize_kvs < 2*8){
		size_kvs = 2*8;
	}
else if(realsize_kvs >= 2*8 && realsize_kvs < 3*8){
		size_kvs = 3*8;
	}
else if(realsize_kvs >= 3*8 && realsize_kvs < 4*8){
		size_kvs = 4*8;
	}
else if(realsize_kvs >= 4*8 && realsize_kvs < 5*8){
		size_kvs = 5*8;
	}
else if(realsize_kvs >= 5*8 && realsize_kvs < 6*8){
		size_kvs = 6*8;
	}
else if(realsize_kvs >= 6*8 && realsize_kvs < 7*8){
		size_kvs = 7*8;
	}
else if(realsize_kvs >= 7*8 && realsize_kvs < 8*8){
		size_kvs = 8*8;
	}
else if(realsize_kvs >= 8*8 && realsize_kvs < 9*8){
		size_kvs = 9*8;
	}
else if(realsize_kvs >= 9*8 && realsize_kvs < 10*8){
		size_kvs = 10*8;
	}
else if(realsize_kvs >= 10*8 && realsize_kvs < 11*8){
		size_kvs = 11*8;
	}
else if(realsize_kvs >= 11*8 && realsize_kvs < 12*8){
		size_kvs = 12*8;
	}
else if(realsize_kvs >= 12*8 && realsize_kvs < 13*8){
		size_kvs = 13*8;
	}
else if(realsize_kvs >= 13*8 && realsize_kvs < 14*8){
		size_kvs = 14*8;
	}
else if(realsize_kvs >= 14*8 && realsize_kvs < 15*8){
		size_kvs = 15*8;
	}
else if(realsize_kvs >= 15*8 && realsize_kvs < 16*8){
		size_kvs = 16*8;
	}
else if(realsize_kvs >= 16*8 && realsize_kvs < 17*8){
		size_kvs = 17*8;
	}
else if(realsize_kvs >= 17*8 && realsize_kvs < 18*8){
		size_kvs = 18*8;
	}
else if(realsize_kvs >= 18*8 && realsize_kvs < 19*8){
		size_kvs = 19*8;
	}
else if(realsize_kvs >= 19*8 && realsize_kvs < 20*8){
		size_kvs = 20*8;
	}
else if(realsize_kvs >= 20*8 && realsize_kvs < 21*8){
		size_kvs = 21*8;
	}
else if(realsize_kvs >= 21*8 && realsize_kvs < 22*8){
		size_kvs = 22*8;
	}
else if(realsize_kvs >= 22*8 && realsize_kvs < 23*8){
		size_kvs = 23*8;
	}
else if(realsize_kvs >= 23*8 && realsize_kvs < 24*8){
		size_kvs = 24*8;
	}
else if(realsize_kvs >= 24*8 && realsize_kvs < 25*8){
		size_kvs = 25*8;
	}
else if(realsize_kvs >= 25*8 && realsize_kvs < 26*8){
		size_kvs = 26*8;
	}
else if(realsize_kvs >= 26*8 && realsize_kvs < 27*8){
		size_kvs = 27*8;
	}
else if(realsize_kvs >= 27*8 && realsize_kvs < 28*8){
		size_kvs = 28*8;
	}
else if(realsize_kvs >= 28*8 && realsize_kvs < 29*8){
		size_kvs = 29*8;
	}
else if(realsize_kvs >= 29*8 && realsize_kvs < 30*8){
		size_kvs = 30*8;
	}
else if(realsize_kvs >= 30*8 && realsize_kvs < 31*8){
		size_kvs = 31*8;
	}
else if(realsize_kvs >= 31*8 && realsize_kvs < 32*8){
		size_kvs = 32*8;
	}
else if(realsize_kvs >= 32*8 && realsize_kvs < 33*8){
		size_kvs = 33*8;
	}
else if(realsize_kvs >= 33*8 && realsize_kvs < 34*8){
		size_kvs = 34*8;
	}
else if(realsize_kvs >= 34*8 && realsize_kvs < 35*8){
		size_kvs = 35*8;
	}
else if(realsize_kvs >= 35*8 && realsize_kvs < 36*8){
		size_kvs = 36*8;
	}
else if(realsize_kvs >= 36*8 && realsize_kvs < 37*8){
		size_kvs = 37*8;
	}
else if(realsize_kvs >= 37*8 && realsize_kvs < 38*8){
		size_kvs = 38*8;
	}
else if(realsize_kvs >= 38*8 && realsize_kvs < 39*8){
		size_kvs = 39*8;
	}
else if(realsize_kvs >= 39*8 && realsize_kvs < 40*8){
		size_kvs = 40*8;
	}
else if(realsize_kvs >= 40*8 && realsize_kvs < 41*8){
		size_kvs = 41*8;
	}
else if(realsize_kvs >= 41*8 && realsize_kvs < 42*8){
		size_kvs = 42*8;
	}
else if(realsize_kvs >= 42*8 && realsize_kvs < 43*8){
		size_kvs = 43*8;
	}
else if(realsize_kvs >= 43*8 && realsize_kvs < 44*8){
		size_kvs = 44*8;
	}
else if(realsize_kvs >= 44*8 && realsize_kvs < 45*8){
		size_kvs = 45*8;
	}
else if(realsize_kvs >= 45*8 && realsize_kvs < 46*8){
		size_kvs = 46*8;
	}
else if(realsize_kvs >= 46*8 && realsize_kvs < 47*8){
		size_kvs = 47*8;
	}
else if(realsize_kvs >= 47*8 && realsize_kvs < 48*8){
		size_kvs = 48*8;
	}
else if(realsize_kvs >= 48*8 && realsize_kvs < 49*8){
		size_kvs = 49*8;
	}
else if(realsize_kvs >= 49*8 && realsize_kvs < 50*8){
		size_kvs = 50*8;
	}
else if(realsize_kvs >= 50*8 && realsize_kvs < 51*8){
		size_kvs = 51*8;
	}
else if(realsize_kvs >= 51*8 && realsize_kvs < 52*8){
		size_kvs = 52*8;
	}
else if(realsize_kvs >= 52*8 && realsize_kvs < 53*8){
		size_kvs = 53*8;
	}
else if(realsize_kvs >= 53*8 && realsize_kvs < 54*8){
		size_kvs = 54*8;
	}
else if(realsize_kvs >= 54*8 && realsize_kvs < 55*8){
		size_kvs = 55*8;
	}
else if(realsize_kvs >= 55*8 && realsize_kvs < 56*8){
		size_kvs = 56*8;
	}
else if(realsize_kvs >= 56*8 && realsize_kvs < 57*8){
		size_kvs = 57*8;
	}
else if(realsize_kvs >= 57*8 && realsize_kvs < 58*8){
		size_kvs = 58*8;
	}
else if(realsize_kvs >= 58*8 && realsize_kvs < 59*8){
		size_kvs = 59*8;
	}
else if(realsize_kvs >= 59*8 && realsize_kvs < 60*8){
		size_kvs = 60*8;
	}
else if(realsize_kvs >= 60*8 && realsize_kvs < 61*8){
		size_kvs = 61*8;
	}
else if(realsize_kvs >= 61*8 && realsize_kvs < 62*8){
		size_kvs = 62*8;
	}
else if(realsize_kvs >= 62*8 && realsize_kvs < 63*8){
		size_kvs = 63*8;
	}
else if(realsize_kvs >= 63*8 && realsize_kvs < 64*8){
		size_kvs = 64*8;
	}
	else {
		#ifdef ENABLE_PERFECTACCURACY
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"WARNING:getpartitionwritesz: should not get here. something might be wrong. realsize_kvs: "<<realsize_kvs<<", size_kvs: "<<size_kvs<<", last size_kvs: "<<(64*PADSKIP)<<", OPTIMALSIZE: "<<OPTIMALSIZE<<", PADSKIP: "<<PADSKIP<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl;				 
		exit(EXIT_FAILURE);
		#endif
		#else 
		// size_kvs = PADDEDDESTBUFFER_SIZE;
		size_kvs = OPTIMALSIZE;
		#endif 
	}
	#ifdef SW 
	if((bramoffset_kvs + size_kvs) >= PADDEDDESTBUFFER_SIZE){ size_kvs = PADDEDDESTBUFFER_SIZE - bramoffset_kvs - 1; } 
	#endif 
	#else 
	size_kvs = realsize_kvs;
	#endif
	return size_kvs;
}
buffer_type 
	#ifdef SW 
	actslw::
	#endif 
getpartitionwritesz(buffer_type realsize_kvs, buffer_type bramoffset_kvs){
	#pragma HLS INLINE
	buffer_type size_kvs = 0;
	
	#ifdef ENABLE_APPROXIMATEPARTITIONWRITES
 // SECOND SET:: (my_variable): 2, (my_base): 0
		
if(realsize_kvs >= 0 && realsize_kvs < 2){ size_kvs = 2; }
else if(realsize_kvs >= 2 && realsize_kvs < 4){ size_kvs = 4; }
else if(realsize_kvs >= 4 && realsize_kvs < 6){ size_kvs = 6; }
else if(realsize_kvs >= 6 && realsize_kvs < 8){ size_kvs = 8; }
else if(realsize_kvs >= 8 && realsize_kvs < 10){ size_kvs = 10; }
else if(realsize_kvs >= 10 && realsize_kvs < 12){ size_kvs = 12; }
else if(realsize_kvs >= 12 && realsize_kvs < 14){ size_kvs = 14; }
else if(realsize_kvs >= 14 && realsize_kvs < 16){ size_kvs = 16; }
else if(realsize_kvs >= 16 && realsize_kvs < 18){ size_kvs = 18; }
else if(realsize_kvs >= 18 && realsize_kvs < 20){ size_kvs = 20; }
else if(realsize_kvs >= 20 && realsize_kvs < 22){ size_kvs = 22; }
else if(realsize_kvs >= 22 && realsize_kvs < 24){ size_kvs = 24; }
else if(realsize_kvs >= 24 && realsize_kvs < 26){ size_kvs = 26; }
else if(realsize_kvs >= 26 && realsize_kvs < 28){ size_kvs = 28; }
else if(realsize_kvs >= 28 && realsize_kvs < 30){ size_kvs = 30; }
else if(realsize_kvs >= 30 && realsize_kvs < 32){ size_kvs = 32; }
else if(realsize_kvs >= 32 && realsize_kvs < 34){ size_kvs = 34; }
else if(realsize_kvs >= 34 && realsize_kvs < 36){ size_kvs = 36; }
else if(realsize_kvs >= 36 && realsize_kvs < 38){ size_kvs = 38; }
else if(realsize_kvs >= 38 && realsize_kvs < 40){ size_kvs = 40; }
else if(realsize_kvs >= 40 && realsize_kvs < 42){ size_kvs = 42; }
else if(realsize_kvs >= 42 && realsize_kvs < 44){ size_kvs = 44; }
else if(realsize_kvs >= 44 && realsize_kvs < 46){ size_kvs = 46; }
else if(realsize_kvs >= 46 && realsize_kvs < 48){ size_kvs = 48; }
else if(realsize_kvs >= 48 && realsize_kvs < 50){ size_kvs = 50; }
else if(realsize_kvs >= 50 && realsize_kvs < 52){ size_kvs = 52; }
else if(realsize_kvs >= 52 && realsize_kvs < 54){ size_kvs = 54; }
else if(realsize_kvs >= 54 && realsize_kvs < 56){ size_kvs = 56; }
else if(realsize_kvs >= 56 && realsize_kvs < 58){ size_kvs = 58; }
else if(realsize_kvs >= 58 && realsize_kvs < 60){ size_kvs = 60; }
else if(realsize_kvs >= 60 && realsize_kvs < 62){ size_kvs = 62; }
else if(realsize_kvs >= 62 && realsize_kvs < 64){ size_kvs = 64; }
		
 // SECOND SET:: (my_variable): 4, (my_base): 64
		
		else if(realsize_kvs >= 64 && realsize_kvs < 68){ size_kvs = 68; }
		else if(realsize_kvs >= 68 && realsize_kvs < 72){ size_kvs = 72; }
		else if(realsize_kvs >= 72 && realsize_kvs < 76){ size_kvs = 76; }
		else if(realsize_kvs >= 76 && realsize_kvs < 80){ size_kvs = 80; }
		else if(realsize_kvs >= 80 && realsize_kvs < 84){ size_kvs = 84; }
		else if(realsize_kvs >= 84 && realsize_kvs < 88){ size_kvs = 88; }
		else if(realsize_kvs >= 88 && realsize_kvs < 92){ size_kvs = 92; }
		else if(realsize_kvs >= 92 && realsize_kvs < 96){ size_kvs = 96; }
		else if(realsize_kvs >= 96 && realsize_kvs < 100){ size_kvs = 100; }
		else if(realsize_kvs >= 100 && realsize_kvs < 104){ size_kvs = 104; }
		else if(realsize_kvs >= 104 && realsize_kvs < 108){ size_kvs = 108; }
		else if(realsize_kvs >= 108 && realsize_kvs < 112){ size_kvs = 112; }
		else if(realsize_kvs >= 112 && realsize_kvs < 116){ size_kvs = 116; }
		else if(realsize_kvs >= 116 && realsize_kvs < 120){ size_kvs = 120; }
		else if(realsize_kvs >= 120 && realsize_kvs < 124){ size_kvs = 124; }
		else if(realsize_kvs >= 124 && realsize_kvs < 128){ size_kvs = 128; }
		
 // THIRD SET:: (my_variable): 8, (my_base): 128
		
		else if(realsize_kvs >= 128 && realsize_kvs < 136){ size_kvs = 136; }
		else if(realsize_kvs >= 136 && realsize_kvs < 144){ size_kvs = 144; }
		else if(realsize_kvs >= 144 && realsize_kvs < 152){ size_kvs = 152; }
		else if(realsize_kvs >= 152 && realsize_kvs < 160){ size_kvs = 160; }
		else if(realsize_kvs >= 160 && realsize_kvs < 168){ size_kvs = 168; }
		else if(realsize_kvs >= 168 && realsize_kvs < 176){ size_kvs = 176; }
		else if(realsize_kvs >= 176 && realsize_kvs < 184){ size_kvs = 184; }
		else if(realsize_kvs >= 184 && realsize_kvs < 192){ size_kvs = 192; }
		else if(realsize_kvs >= 192 && realsize_kvs < 200){ size_kvs = 200; }
		else if(realsize_kvs >= 200 && realsize_kvs < 208){ size_kvs = 208; }
		else if(realsize_kvs >= 208 && realsize_kvs < 216){ size_kvs = 216; }
		else if(realsize_kvs >= 216 && realsize_kvs < 224){ size_kvs = 224; }
		else if(realsize_kvs >= 224 && realsize_kvs < 232){ size_kvs = 232; }
		else if(realsize_kvs >= 232 && realsize_kvs < 240){ size_kvs = 240; }
		else if(realsize_kvs >= 240 && realsize_kvs < 248){ size_kvs = 248; }
		else if(realsize_kvs >= 248 && realsize_kvs < 256){ size_kvs = 256; }
		
 // FOURTH SET:: (my_variable): 16, (my_base): 256
		
		else if(realsize_kvs >= 256 && realsize_kvs < 272){ size_kvs = 272; }
		else if(realsize_kvs >= 272 && realsize_kvs < 288){ size_kvs = 288; }
		else if(realsize_kvs >= 288 && realsize_kvs < 304){ size_kvs = 304; }
		else if(realsize_kvs >= 304 && realsize_kvs < 320){ size_kvs = 320; }
		else if(realsize_kvs >= 320 && realsize_kvs < 336){ size_kvs = 336; }
		else if(realsize_kvs >= 336 && realsize_kvs < 352){ size_kvs = 352; }
		else if(realsize_kvs >= 352 && realsize_kvs < 368){ size_kvs = 368; }
		else if(realsize_kvs >= 368 && realsize_kvs < 384){ size_kvs = 384; }
		else if(realsize_kvs >= 384 && realsize_kvs < 400){ size_kvs = 400; }
		else if(realsize_kvs >= 400 && realsize_kvs < 416){ size_kvs = 416; }
		else if(realsize_kvs >= 416 && realsize_kvs < 432){ size_kvs = 432; }
		else if(realsize_kvs >= 432 && realsize_kvs < 448){ size_kvs = 448; }
		else if(realsize_kvs >= 448 && realsize_kvs < 464){ size_kvs = 464; }
		else if(realsize_kvs >= 464 && realsize_kvs < 480){ size_kvs = 480; }
		else if(realsize_kvs >= 480 && realsize_kvs < 496){ size_kvs = 496; }
		else if(realsize_kvs >= 496 && realsize_kvs < 512){ size_kvs = 512; }
		
		else {
			cout<<"WARNING:getpartitionwritesz: should not get here. something might be wrong. realsize_kvs: "<<realsize_kvs<<", size_kvs: "<<size_kvs<<", last size_kvs: "<<(64*PADSKIP)<<", OPTIMALSIZE: "<<OPTIMALSIZE<<", PADSKIP: "<<PADSKIP<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl;				 
			
			#ifdef ENABLE_PERFECTACCURACY
			#ifdef _DEBUGMODE_CHECKS2
			cout<<"WARNING:getpartitionwritesz: should not get here. something might be wrong. realsize_kvs: "<<realsize_kvs<<", size_kvs: "<<size_kvs<<", last size_kvs: "<<(64*PADSKIP)<<", OPTIMALSIZE: "<<OPTIMALSIZE<<", PADSKIP: "<<PADSKIP<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl;				 
			exit(EXIT_FAILURE);
			#endif
			#else
			size_kvs = OPTIMALSIZE;
			#endif 
		}
		#ifdef SW 
		if((bramoffset_kvs + size_kvs) >= PADDEDDESTBUFFER_SIZE){ size_kvs = PADDEDDESTBUFFER_SIZE - bramoffset_kvs - 1; } 
		#endif
	#else 
		size_kvs = realsize_kvs;
	#endif
	return size_kvs;
}
buffer_type
	#ifdef SW 
	actslw::
	#endif 
withinvalidrange(buffer_type val1, buffer_type val2){
	#pragma HLS INLINE
	if(val1 < val2){ return 1; }
	else { return 0; }
}
void 
	#ifdef SW 
	actslw::
	#endif 
calculateoffsets(keyvalue_t * buffer, buffer_type size, buffer_type base, buffer_type skipspacing){
	unsigned int analysis_size = NUMLASTLEVELPARTITIONS;
	
	buffer[0].key += base;
	for(partition_type i=1; i<size; i++){ 
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_size avg=analysis_size	
		buffer[i].key = allignhigher_KV(buffer[i-1].key + buffer[i-1].value + skipspacing); 
	}
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif 
calculateoffsets(keyvalue_t * buffer, buffer_type size, buffer_type base, buffer_type skipspacing[NUM_PARTITIONS]){
	unsigned int analysis_size = NUMLASTLEVELPARTITIONS;
	
	buffer[0].key += base;
	for(partition_type i=1; i<size; i++){ 
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_size avg=analysis_size	
		buffer[i].key = allignhigher_KV(buffer[i-1].key + buffer[i-1].value + skipspacing[i-1]); 
	}
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif 
calculateunallignedoffsets(skeyvalue_t buffer[NUM_PARTITIONS], buffer_type size, buffer_type base, buffer_type skipspacing){
	for(partition_type i=1; i<size; i++){ 
		buffer[i].key = buffer[i-1].key + buffer[i-1].value + skipspacing; 
	}
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif 
calculatemanyoffsets(skeyvalue_t buffer[VECTOR_SIZE][NUM_PARTITIONS], buffer_type size, buffer_type base, buffer_type skipspacing){
	for(partition_type i=1; i<size; i++){ 
		buffer[0][i].key = allignhigher_KV(buffer[0][i-1].key + buffer[0][i-1].value + skipspacing); 
		buffer[1][i].key = allignhigher_KV(buffer[1][i-1].key + buffer[1][i-1].value + skipspacing); 
		buffer[2][i].key = allignhigher_KV(buffer[2][i-1].key + buffer[2][i-1].value + skipspacing); 
		buffer[3][i].key = allignhigher_KV(buffer[3][i-1].key + buffer[3][i-1].value + skipspacing); 
		buffer[4][i].key = allignhigher_KV(buffer[4][i-1].key + buffer[4][i-1].value + skipspacing); 
		buffer[5][i].key = allignhigher_KV(buffer[5][i-1].key + buffer[5][i-1].value + skipspacing); 
		buffer[6][i].key = allignhigher_KV(buffer[6][i-1].key + buffer[6][i-1].value + skipspacing); 
		buffer[7][i].key = allignhigher_KV(buffer[7][i-1].key + buffer[7][i-1].value + skipspacing); 
	}
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif 
calculatemanyunallignedoffsets(skeyvalue_t buffer[VECTOR_SIZE][NUM_PARTITIONS], buffer_type size, buffer_type base, buffer_type skipspacing){
	for(partition_type i=1; i<size; i++){ 
		buffer[0][i].key = buffer[0][i-1].key + buffer[0][i-1].value + skipspacing; 
		buffer[1][i].key = buffer[1][i-1].key + buffer[1][i-1].value + skipspacing; 
		buffer[2][i].key = buffer[2][i-1].key + buffer[2][i-1].value + skipspacing; 
		buffer[3][i].key = buffer[3][i-1].key + buffer[3][i-1].value + skipspacing; 
		buffer[4][i].key = buffer[4][i-1].key + buffer[4][i-1].value + skipspacing; 
		buffer[5][i].key = buffer[5][i-1].key + buffer[5][i-1].value + skipspacing; 
		buffer[6][i].key = buffer[6][i-1].key + buffer[6][i-1].value + skipspacing; 
		buffer[7][i].key = buffer[7][i-1].key + buffer[7][i-1].value + skipspacing; 
	}
	return;
}
batch_type
	#ifdef SW 
	actslw::
	#endif 
getvaluecount(keyvalue_t * keyvalues, unsigned int size){
	unsigned int totalnumkeyvalues = 0;
	for(unsigned int p=0; p<size; p++){ totalnumkeyvalues += keyvalues[p].value; }
	return totalnumkeyvalues;
}
batch_type
	#ifdef SW 
	actslw::
	#endif 
get_num_source_partitions(step_type currentLOP){
	#pragma HLS INLINE
	analysis_type analysis_treedepth = TREE_DEPTH;
	
	if(currentLOP == 0){ currentLOP = 1; }
	batch_type pow = 1;
	for(step_type i=0; i<(currentLOP-1); i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_treedepth avg=analysis_treedepth
		pow = pow * NUM_PARTITIONS;
	}
	return pow;
}
globalparams_t 
	#ifdef SW 
	actslw::
	#endif 
getglobalparams(uint512_dt * sourcevolume){
	globalparams_t globalparams;
	#ifdef _WIDEWORD
	globalparams.runkernelcommand = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNKERNELCOMMANDID].range(31, 0);
	globalparams.processcommand = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_PROCESSCOMMANDID].range(31, 0);
	globalparams.collectstatscommand = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_COLLECTSTATSCOMMANDID].range(31, 0);
	globalparams.partitioncommand = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_PARTITIONCOMMANDID].range(31, 0);
	globalparams.reducecommand = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_APPLYUPDATESCOMMANDID].range(31, 0);
	globalparams.vbegin = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_VOFFSET].range(31, 0); 
	globalparams.vsize = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_VSIZE].range(31, 0); 
	globalparams.treedepth = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_TREEDEPTH].range(31, 0); 
	globalparams.LLOPnumpartitions = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_FINALNUMPARTITIONS].range(31, 0);
	globalparams.GraphIter = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_GRAPHITERATIONID].range(31, 0);
	globalparams.GraphAlgo = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_GRAPHALGORITHMID].range(31, 0);
	globalparams.batchsize = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BATCHSIZE].range(31, 0);
	globalparams.runsize = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].range(31, 0);
	globalparams.nextbatchoffset = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NEXTBATCHOFFSET].range(31, 0);
	globalparams.statsalreadycollected = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_STATSALREADYCOLLECTED].range(31, 0);
	globalparams.groupid = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_GROUPID].range(31, 0);
	globalparams.beginLOP = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BEGINLOP].range(31, 0);
	globalparams.endLOP = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_ENDLOP].range(31, 0);
	globalparams.batch_range = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BATCHRANGE].range(31, 0);
	globalparams.batch_range_pow = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BATCHRANGE_POW].range(31, 0);
	globalparams.applyvertexbuffersz = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_APPLYVERTEXBUFFERSZ].range(31, 0);
	globalparams.applyvertexbuffersz_kvs = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_APPLYVERTEXBUFFERSZ_KVS].range(31, 0);
	globalparams.baseaddr_destkvs_kvs = 0;
	#else 
	globalparams.runkernelcommand = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNKERNELCOMMANDID].data[0].key;
	globalparams.processcommand = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_PROCESSCOMMANDID].data[0].key;
	globalparams.collectstatscommand = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_COLLECTSTATSCOMMANDID].data[0].key;
	globalparams.partitioncommand = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_PARTITIONCOMMANDID].data[0].key;
	globalparams.reducecommand = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_APPLYUPDATESCOMMANDID].data[0].key;
	globalparams.vbegin = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_VOFFSET].data[0].key; 
	globalparams.vsize = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_VSIZE].data[0].key; 
	globalparams.treedepth = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_TREEDEPTH].data[0].key; 
	globalparams.LLOPnumpartitions = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_FINALNUMPARTITIONS].data[0].key;
	globalparams.GraphIter = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_GRAPHITERATIONID].data[0].key;
	globalparams.GraphAlgo = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_GRAPHALGORITHMID].data[0].key;
	globalparams.batchsize = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BATCHSIZE].data[0].key;
	globalparams.runsize = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key;
	globalparams.nextbatchoffset = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NEXTBATCHOFFSET].data[0].key;
	globalparams.statsalreadycollected = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_STATSALREADYCOLLECTED].data[0].key;
	globalparams.groupid = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_GROUPID].data[0].key;
	globalparams.beginLOP = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BEGINLOP].data[0].key;
	globalparams.endLOP = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_ENDLOP].data[0].key;
	globalparams.batch_range = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BATCHRANGE].data[0].key;
	globalparams.batch_range_pow = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BATCHRANGE_POW].data[0].key;
	globalparams.applyvertexbuffersz = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_APPLYVERTEXBUFFERSZ].data[0].key;
	globalparams.applyvertexbuffersz_kvs = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_APPLYVERTEXBUFFERSZ_KVS].data[0].key;
	globalparams.baseaddr_destkvs_kvs = 0;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->printglobalparameters("actslw::getglobalparams:: printing global parameters", globalparams);
	#endif
	return globalparams;
}
sweepparams_t 
	#ifdef SW 
	actslw::
	#endif 
getsweepparams(globalparams_t globalparams, step_type currentLOP, batch_type source_partition){
	sweepparams_t sweepparams;
	batch_type sourceskipsize = getskipsize(currentLOP, SOURCE, globalparams);
	
	sweepparams.currentLOP = currentLOP;
	if(currentLOP == 0 || (currentLOP % 2) == 1){ sweepparams.worksourcebaseaddress_kvs = BASEOFFSET_KVDRAM_KVS; sweepparams.workdestbaseaddress_kvs = BASEOFFSET_KVDRAMWORKSPACE_KVS; }
	else { sweepparams.worksourcebaseaddress_kvs = BASEOFFSET_KVDRAMWORKSPACE_KVS; sweepparams.workdestbaseaddress_kvs = BASEOFFSET_KVDRAM_KVS; }
	
	sweepparams.upperlimit = globalparams.vbegin + (source_partition * sourceskipsize * (GET_BATCH_RANGE(globalparams.groupid) / GET_NUMLASTLEVELPARTITIONS(globalparams.groupid)));
	
	sweepparams.source_partition = source_partition;
	return sweepparams;
}
config_t 
	#ifdef SW 
	actslw::
	#endif 
getconfig(step_type currentLOP, unsigned int groupid){
	config_t config;
	
	if(currentLOP == 0){ config.enablecollectglobalstats = ON; config.enablepartition = OFF; config.enablereduce = OFF; } 
	else if(currentLOP > 0 && currentLOP <= GET_TREE_DEPTH(groupid)){ config.enablecollectglobalstats = OFF; config.enablepartition = ON; config.enablereduce = OFF; }
	else { config.enablecollectglobalstats = OFF; config.enablepartition = OFF; config.enablereduce = ON; }
	return config;
}
travstate_t 
	#ifdef SW 
	actslw::
	#endif 
gettravstate(uint512_dt * kvdram, globalparams_t globalparams, step_type currentLOP, batch_type sourcestatsmarker){
	travstate_t travstate;
	keyvalue_t keyvalue;
	keyvalue_t nextkeyvalue;
	
	if(currentLOP == 1){ keyvalue.key = 0; }
	else { keyvalue = kvdram[BASEOFFSET_STATSDRAM_KVS + sourcestatsmarker].data[0]; }
	
	if(currentLOP == 1){ nextkeyvalue.key = globalparams.runsize; }
	else { nextkeyvalue.key = keyvalue.key + keyvalue.value; }
		
	travstate.begin_kvs = keyvalue.key / VECTOR_SIZE; 
	travstate.end_kvs = nextkeyvalue.key / VECTOR_SIZE;
	travstate.skip_kvs = SRCBUFFER_SIZE;
	travstate.i_kvs = travstate.begin_kvs;
	return travstate;	
}

// collect globalstats functions 
void 
	#ifdef SW 
	actslw::
	#endif 
readglobalstats0(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[NUM_PARTITIONS], batch_type offset_kvs){
	if(enable == OFF){ return; }
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("readglobalstats0", offset_kvs + NUM_PARTITIONS, BASEOFFSET_STATSDRAM_KVS + KVSTATSDRAMSZ_KVS + 1, NAp, NAp, NAp);
	#endif
	
	READGLOBALSTATS_LOOP: for (buffer_type i=0; i<NUM_PARTITIONS; i++){
	#pragma HLS PIPELINE II=1
		globalstatsbuffer[i] = kvdram[offset_kvs + i].data[0];
	}
	return;
}

void 
	#ifdef SW 
	actslw::
	#endif
collectglobalstats0(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], step_type currentLOP, vertex_t upperlimit, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE;

	COLLECTGLOBALSTATS_LOOP: for(buffer_type i=0; i<SRCBUFFER_SIZE; i++){ // REMOVEME? this should be a variable
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
		
		partition_type p0 = 0;
		if(keyvalue0.key != INVALIDDATA){ p0 = getpartition(keyvalue0, currentLOP, upperlimit, globalparams.batch_range_pow, globalparams.groupid); }
		partition_type p1 = 0;
		if(keyvalue1.key != INVALIDDATA){ p1 = getpartition(keyvalue1, currentLOP, upperlimit, globalparams.batch_range_pow, globalparams.groupid); }
		partition_type p2 = 0;
		if(keyvalue2.key != INVALIDDATA){ p2 = getpartition(keyvalue2, currentLOP, upperlimit, globalparams.batch_range_pow, globalparams.groupid); }
		partition_type p3 = 0;
		if(keyvalue3.key != INVALIDDATA){ p3 = getpartition(keyvalue3, currentLOP, upperlimit, globalparams.batch_range_pow, globalparams.groupid); }
		partition_type p4 = 0;
		if(keyvalue4.key != INVALIDDATA){ p4 = getpartition(keyvalue4, currentLOP, upperlimit, globalparams.batch_range_pow, globalparams.groupid); }
		partition_type p5 = 0;
		if(keyvalue5.key != INVALIDDATA){ p5 = getpartition(keyvalue5, currentLOP, upperlimit, globalparams.batch_range_pow, globalparams.groupid); }
		partition_type p6 = 0;
		if(keyvalue6.key != INVALIDDATA){ p6 = getpartition(keyvalue6, currentLOP, upperlimit, globalparams.batch_range_pow, globalparams.groupid); }
		partition_type p7 = 0;
		if(keyvalue7.key != INVALIDDATA){ p7 = getpartition(keyvalue7, currentLOP, upperlimit, globalparams.batch_range_pow, globalparams.groupid); }
		
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[0]", p0, NUM_PARTITIONS, keyvalue0.key, GET_BATCH_RANGE(globalparams.groupid), NAp);
		#endif 
		if(keyvalue0.key != INVALIDDATA){ destbuffer[0][p0].value += 1; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[1]", p1, NUM_PARTITIONS, keyvalue1.key, GET_BATCH_RANGE(globalparams.groupid), NAp);
		#endif 
		if(keyvalue1.key != INVALIDDATA){ destbuffer[1][p1].value += 1; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[2]", p2, NUM_PARTITIONS, keyvalue2.key, GET_BATCH_RANGE(globalparams.groupid), NAp);
		#endif 
		if(keyvalue2.key != INVALIDDATA){ destbuffer[2][p2].value += 1; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[3]", p3, NUM_PARTITIONS, keyvalue3.key, GET_BATCH_RANGE(globalparams.groupid), NAp);
		#endif 
		if(keyvalue3.key != INVALIDDATA){ destbuffer[3][p3].value += 1; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[4]", p4, NUM_PARTITIONS, keyvalue4.key, GET_BATCH_RANGE(globalparams.groupid), NAp);
		#endif 
		if(keyvalue4.key != INVALIDDATA){ destbuffer[4][p4].value += 1; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[5]", p5, NUM_PARTITIONS, keyvalue5.key, GET_BATCH_RANGE(globalparams.groupid), NAp);
		#endif 
		if(keyvalue5.key != INVALIDDATA){ destbuffer[5][p5].value += 1; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[6]", p6, NUM_PARTITIONS, keyvalue6.key, GET_BATCH_RANGE(globalparams.groupid), NAp);
		#endif 
		if(keyvalue6.key != INVALIDDATA){ destbuffer[6][p6].value += 1; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[7]", p7, NUM_PARTITIONS, keyvalue7.key, GET_BATCH_RANGE(globalparams.groupid), NAp);
		#endif 
		if(keyvalue7.key != INVALIDDATA){ destbuffer[7][p7].value += 1; }
	}
	return;
}

void 
	#ifdef SW 
	actslw::
	#endif
prepareglobalstats0(bool_type enable, keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t globalstatsbuffer[NUM_PARTITIONS], globalparams_t globalparams){
	if(enable == OFF){ return; }
	
	PREPAREGLOBALSTATS_LOOP1: for(vector_type v=0; v<VECTOR_SIZE; v++){
		PREPAREGLOBALSTATS_LOOP1B: for(buffer_type i=0; i<NUM_PARTITIONS; i++){
			globalstatsbuffer[i].value += buffer[v][i].value;
		}
	}
	return;
}

void 
	#ifdef SW 
	actslw::
	#endif 
saveglobalstats0(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[NUM_PARTITIONS], batch_type offset_kvs){
	if(enable == OFF){ return; }
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("saveglobalstats0", offset_kvs + NUM_PARTITIONS, BASEOFFSET_STATSDRAM_KVS + KVSTATSDRAMSZ_KVS + 1, offset_kvs, NUM_PARTITIONS, KVSTATSDRAMSZ_KVS);
	#endif
	
	SAVEGLOBALSTATS_LOOP: for (buffer_type i=0; i<NUM_PARTITIONS; i++){
	#pragma HLS PIPELINE II=1
		kvdram[offset_kvs + i].data[0] = globalstatsbuffer[i];
		// cout<<"################## saveglobalstats0:: globalstatsbuffer["<<i<<"] ["<<globalstatsbuffer[i].key<<", "<<globalstatsbuffer[i].value<<"] saved at kvdram["<<offset_kvs + i<<"]"<<endl;
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalvar_savestats_counttotalstatswritten(VECTOR_SIZE);
		#endif
	}
	
	/* #ifdef _DEBUGMODE_CHECKS2
	for (buffer_type i=0; i<NUM_PARTITIONS; i++){
		if(i < (NUM_PARTITIONS-1)){ if(globalstatsbuffer[i].key + globalstatsbuffer[i].value >= globalstatsbuffer[i+1].key){ cout<<"saveglobalstats0::ERROR. overlap at "<<i<<" and "<<i+1<<endl; exit(EXIT_FAILURE); }}
	}
	#endif 
	exit(EXIT_SUCCESS); */
	return;
}

// partition functions
void 
	#ifdef SW 
	actslw::
	#endif 
readkeyvalues0(bool_type enable, uint512_dt * kvdram, keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], batch_type offset_kvs, travstate_t travstate){
	if(enable == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE;
	buffer_type chunk_size = getchunksize(SRCBUFFER_SIZE, travstate, 0);
	
	READKVS_LOOP: for (buffer_type i=0; i<chunk_size; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
	#pragma HLS PIPELINE II=1
		buffer[0][i] = kvdram[offset_kvs + i].data[0]; 
		buffer[1][i] = kvdram[offset_kvs + i].data[1]; 
		buffer[2][i] = kvdram[offset_kvs + i].data[2]; 
		buffer[3][i] = kvdram[offset_kvs + i].data[3]; 
		buffer[4][i] = kvdram[offset_kvs + i].data[4]; 
		buffer[5][i] = kvdram[offset_kvs + i].data[5]; 
		buffer[6][i] = kvdram[offset_kvs + i].data[6]; 
		buffer[7][i] = kvdram[offset_kvs + i].data[7]; 
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	return;
}

void 
	#ifdef SW 
	actslw::
	#endif
partitionkeyvalues0(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t localcapsule[VECTOR_SIZE][NUM_PARTITIONS], step_type currentLOP, vertex_t upperlimit, travstate_t travstate, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE;
	buffer_type chunk_size = getchunksize(SRCBUFFER_SIZE, travstate, 0);

	resetmanykeyandvalues(localcapsule, NUM_PARTITIONS);
	
	PARTITIONKEYVALUES_LOOP1: for(step_type c=0; c<2; c++){
		PARTITIONKEYVALUES_LOOP1B: for(buffer_type i=0; i<chunk_size; i++){
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
			
			partition_type p0 = 0;
			if(keyvalue0.key != INVALIDDATA){ p0 = getpartition(keyvalue0, currentLOP, upperlimit, globalparams.batch_range_pow, globalparams.groupid); } 
			partition_type p1 = 0;
			if(keyvalue1.key != INVALIDDATA){ p1 = getpartition(keyvalue1, currentLOP, upperlimit, globalparams.batch_range_pow, globalparams.groupid); } 
			partition_type p2 = 0;
			if(keyvalue2.key != INVALIDDATA){ p2 = getpartition(keyvalue2, currentLOP, upperlimit, globalparams.batch_range_pow, globalparams.groupid); } 
			partition_type p3 = 0;
			if(keyvalue3.key != INVALIDDATA){ p3 = getpartition(keyvalue3, currentLOP, upperlimit, globalparams.batch_range_pow, globalparams.groupid); } 
			partition_type p4 = 0;
			if(keyvalue4.key != INVALIDDATA){ p4 = getpartition(keyvalue4, currentLOP, upperlimit, globalparams.batch_range_pow, globalparams.groupid); } 
			partition_type p5 = 0;
			if(keyvalue5.key != INVALIDDATA){ p5 = getpartition(keyvalue5, currentLOP, upperlimit, globalparams.batch_range_pow, globalparams.groupid); } 
			partition_type p6 = 0;
			if(keyvalue6.key != INVALIDDATA){ p6 = getpartition(keyvalue6, currentLOP, upperlimit, globalparams.batch_range_pow, globalparams.groupid); } 
			partition_type p7 = 0;
			if(keyvalue7.key != INVALIDDATA){ p7 = getpartition(keyvalue7, currentLOP, upperlimit, globalparams.batch_range_pow, globalparams.groupid); } 
			
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", localcapsule[0][p0].key + localcapsule[0][p0].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", localcapsule[1][p1].key + localcapsule[1][p1].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", localcapsule[2][p2].key + localcapsule[2][p2].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", localcapsule[3][p3].key + localcapsule[3][p3].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", localcapsule[4][p4].key + localcapsule[4][p4].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", localcapsule[5][p5].key + localcapsule[5][p5].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", localcapsule[6][p6].key + localcapsule[6][p6].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", localcapsule[7][p7].key + localcapsule[7][p7].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			#endif
			if(keyvalue0.key != INVALIDDATA){ destbuffer[0][localcapsule[0][p0].key + localcapsule[0][p0].value] = keyvalue0; }
			if(keyvalue1.key != INVALIDDATA){ destbuffer[1][localcapsule[1][p1].key + localcapsule[1][p1].value] = keyvalue1; }
			if(keyvalue2.key != INVALIDDATA){ destbuffer[2][localcapsule[2][p2].key + localcapsule[2][p2].value] = keyvalue2; }
			if(keyvalue3.key != INVALIDDATA){ destbuffer[3][localcapsule[3][p3].key + localcapsule[3][p3].value] = keyvalue3; }
			if(keyvalue4.key != INVALIDDATA){ destbuffer[4][localcapsule[4][p4].key + localcapsule[4][p4].value] = keyvalue4; }
			if(keyvalue5.key != INVALIDDATA){ destbuffer[5][localcapsule[5][p5].key + localcapsule[5][p5].value] = keyvalue5; }
			if(keyvalue6.key != INVALIDDATA){ destbuffer[6][localcapsule[6][p6].key + localcapsule[6][p6].value] = keyvalue6; }
			if(keyvalue7.key != INVALIDDATA){ destbuffer[7][localcapsule[7][p7].key + localcapsule[7][p7].value] = keyvalue7; }
			
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", p0, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", p1, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", p2, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", p3, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", p4, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", p5, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", p6, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", p7, NUM_PARTITIONS, NAp, NAp, NAp);
			#endif
			if(keyvalue0.key != INVALIDDATA){ localcapsule[0][p0].value += 1; }
			if(keyvalue1.key != INVALIDDATA){ localcapsule[1][p1].value += 1; }
			if(keyvalue2.key != INVALIDDATA){ localcapsule[2][p2].value += 1; }
			if(keyvalue3.key != INVALIDDATA){ localcapsule[3][p3].value += 1; }
			if(keyvalue4.key != INVALIDDATA){ localcapsule[4][p4].value += 1; }
			if(keyvalue5.key != INVALIDDATA){ localcapsule[5][p5].value += 1; }
			if(keyvalue6.key != INVALIDDATA){ localcapsule[6][p6].value += 1; }
			if(keyvalue7.key != INVALIDDATA){ localcapsule[7][p7].value += 1; }
		}
		if(c==0){ calculatemanyunallignedoffsets(localcapsule, NUM_PARTITIONS, 0, 0); } 
		if(c==0){ resetmanyvalues(localcapsule, NUM_PARTITIONS); }
	}
	
	#ifdef _DEBUGMODE_STATS
	for(vector_type v=0; v<VECTOR_SIZE; v++){ actsutilityobj->globalvar_inmemory_counttotalvalidkeyvalues(actsutilityobj->ugetvaluecount((keyvalue_t *)localcapsule[v], NUM_PARTITIONS)); } // REMOVEME. unmatched data types
	#endif
	return;
}

void 
	#ifdef SW 
	actslw::
	#endif 
savekeyvalues0(bool_type enable, uint512_dt * kvdram, keyvalue_t buffer[8][PADDEDDESTBUFFER_SIZE], keyvalue_t * globalcapsule, skeyvalue_t localcapsule[NUM_PARTITIONS], batch_type globalbaseaddress_kvs, globalparams_t globalparams){				
	if(enable == OFF){ return; }
	
	#ifdef _DEBUGMODE_CHECKS
	actsutilityobj->printkeyvalues("savekeyvalues0::localcapsule", localcapsule, NUM_PARTITIONS);
	actsutilityobj->printvaluecount("savekeyvalues0::localcapsule", localcapsule, NUM_PARTITIONS);
	actsutilityobj->scankeyvalues("savekeyvalues0::buffer", (keyvalue_t *)buffer, localcapsule, NUM_PARTITIONS, GET_BATCH_RANGE(globalparams.groupid) / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif 
	
	analysis_type analysis_destpartitionsz = PADDEDDESTBUFFER_SIZE / NUM_PARTITIONS;
	SAVEPARTITIONS_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		batch_type dramoffset_kvs = globalbaseaddress_kvs + ((globalcapsule[p].key + globalcapsule[p].value) / VECTOR_SIZE);
		buffer_type bramoffset_kvs = localcapsule[p].key / VECTOR_SIZE;
		buffer_type realsize_kvs = localcapsule[p].value / VECTOR_SIZE;
		buffer_type size_kvs = getpartitionwritesz(realsize_kvs, bramoffset_kvs);
		
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("savekeyvalues0 23", bramoffset_kvs + size_kvs, PADDEDDESTBUFFER_SIZE + 1, p, NAp, NAp);
		actsutilityobj->checkoutofbounds("savekeyvalues0 25", ((globalcapsule[p].key + globalcapsule[p].value) / VECTOR_SIZE), KVDRAMSZ + 1, p, NAp, NAp);
		#endif
		SAVEPARTITIONS_LOOP1B: for(buffer_type i=0; i<size_kvs; i++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_destpartitionsz avg=analysis_destpartitionsz
		#pragma HLS PIPELINE II=1
			kvdram[dramoffset_kvs + i].data[0] = buffer[0][bramoffset_kvs + i]; 
			kvdram[dramoffset_kvs + i].data[1] = buffer[1][bramoffset_kvs + i]; 
			kvdram[dramoffset_kvs + i].data[2] = buffer[2][bramoffset_kvs + i]; 
			kvdram[dramoffset_kvs + i].data[3] = buffer[3][bramoffset_kvs + i]; 
			kvdram[dramoffset_kvs + i].data[4] = buffer[4][bramoffset_kvs + i]; 
			kvdram[dramoffset_kvs + i].data[5] = buffer[5][bramoffset_kvs + i]; 
			kvdram[dramoffset_kvs + i].data[6] = buffer[6][bramoffset_kvs + i]; 
			kvdram[dramoffset_kvs + i].data[7] = buffer[7][bramoffset_kvs + i]; 
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countkvspartitionswritten_actual(VECTOR_SIZE);
			#endif
		}
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvspartitionswritten(realsize_kvs * VECTOR_SIZE);
		#endif
	}
	SAVEPARTITIONS_LOOP2: for(partition_type p=0; p<NUM_PARTITIONS; p++){ globalcapsule[p].value += localcapsule[p].value; }
	
	#ifdef _DEBUGMODE_CHECKS
	actsutilityobj->printkeyvalues("savekeyvalues0::globalcapsule", globalcapsule, NUM_PARTITIONS);
	actsutilityobj->scankeyvalues("savekeyvalues0::kvdram", (keyvalue_t *)(&kvdram[BASEOFFSET_KVDRAMWORKSPACE_KVS]), globalcapsule, NUM_PARTITIONS, GET_BATCH_RANGE(globalparams.groupid) / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif 
	return;
}

// reduce functions
void 
	#ifdef SW 
	actslw::
	#endif 
readvertices0(bool_type enable, uint512_dt * kvdram, keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], batch_type offset_kvs){
	if(enable == OFF){ return; }
		
	READVERTICES_LOOP: for (buffer_type i=0; i<PADDEDDESTBUFFER_SIZE; i++){
	#pragma HLS PIPELINE II=1
		buffer[0][i] = kvdram[offset_kvs + i].data[0]; 
		buffer[1][i] = kvdram[offset_kvs + i].data[1]; 
		buffer[2][i] = kvdram[offset_kvs + i].data[2]; 
		buffer[3][i] = kvdram[offset_kvs + i].data[3]; 
		buffer[4][i] = kvdram[offset_kvs + i].data[4]; 
		buffer[5][i] = kvdram[offset_kvs + i].data[5]; 
		buffer[6][i] = kvdram[offset_kvs + i].data[6]; 
		buffer[7][i] = kvdram[offset_kvs + i].data[7]; 
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	#ifdef _DEBUGMODE_KERNELPRINTS2
	cout<< TIMINGRESULTSCOLOR<<"readvertices0:: vertices read: offset: "<<(offset_kvs - BASEOFFSET_VERTICESDATA_KVS) * VECTOR_SIZE<<", number of vertex datas read: "<<(PADDEDDESTBUFFER_SIZE * VECTOR_SIZE * 2)<< RESET<<endl;
	#endif
	return;
}

void 
	#ifdef SW 
	actslw::
	#endif 
replicatedata0(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], unsigned int sourcebank){
	if(enable == OFF){ return; }
	
	REPLICATEDATA_LOOP1: for(buffer_type i=0; i<PADDEDDESTBUFFER_SIZE; i++){
		destbuffer[0][i] = sourcebuffer[sourcebank][i];
		destbuffer[1][i] = sourcebuffer[sourcebank][i];
		destbuffer[2][i] = sourcebuffer[sourcebank][i];
		destbuffer[3][i] = sourcebuffer[sourcebank][i];
		destbuffer[4][i] = sourcebuffer[sourcebank][i];
		destbuffer[5][i] = sourcebuffer[sourcebank][i];
		destbuffer[6][i] = sourcebuffer[sourcebank][i];
		destbuffer[7][i] = sourcebuffer[sourcebank][i];
	}
	return;
}

void 
	#ifdef SW 
	actslw::
	#endif
reduce0(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], vertex_t upperlimit, unsigned int GraphIter, unsigned int GraphAlgo, travstate_t travstate, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE;
	buffer_type chunk_size = getchunksize(SRCBUFFER_SIZE, travstate, 0);

	REDUCE_LOOP: for(buffer_type i=0; i<chunk_size; i++){
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
		
		vertex_t loc0 = keyvalue0.key - upperlimit;
		vertex_t loc1 = keyvalue1.key - upperlimit;
		vertex_t loc2 = keyvalue2.key - upperlimit;
		vertex_t loc3 = keyvalue3.key - upperlimit;
		vertex_t loc4 = keyvalue4.key - upperlimit;
		vertex_t loc5 = keyvalue5.key - upperlimit;
		vertex_t loc6 = keyvalue6.key - upperlimit;
		vertex_t loc7 = keyvalue7.key - upperlimit;
		
		if(loc0 >= GET_APPLYVERTEXBUFFERSZ(globalparams.groupid) && keyvalue0.key != INVALIDDATA){
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTREDUCEERRORS
			cout<<"ERROR SEEN @ reduce0:: i: "<<i<<", loc0: "<<loc0<<", keyvalue0.key: "<<keyvalue0.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<GET_APPLYVERTEXBUFFERSZ(globalparams.groupid)<<endl; 
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
		if(loc1 >= GET_APPLYVERTEXBUFFERSZ(globalparams.groupid) && keyvalue1.key != INVALIDDATA){
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTREDUCEERRORS
			cout<<"ERROR SEEN @ reduce0:: i: "<<i<<", loc1: "<<loc1<<", keyvalue1.key: "<<keyvalue1.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<GET_APPLYVERTEXBUFFERSZ(globalparams.groupid)<<endl; 
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
		if(loc2 >= GET_APPLYVERTEXBUFFERSZ(globalparams.groupid) && keyvalue2.key != INVALIDDATA){
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTREDUCEERRORS
			cout<<"ERROR SEEN @ reduce0:: i: "<<i<<", loc2: "<<loc2<<", keyvalue2.key: "<<keyvalue2.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<GET_APPLYVERTEXBUFFERSZ(globalparams.groupid)<<endl; 
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
		if(loc3 >= GET_APPLYVERTEXBUFFERSZ(globalparams.groupid) && keyvalue3.key != INVALIDDATA){
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTREDUCEERRORS
			cout<<"ERROR SEEN @ reduce0:: i: "<<i<<", loc3: "<<loc3<<", keyvalue3.key: "<<keyvalue3.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<GET_APPLYVERTEXBUFFERSZ(globalparams.groupid)<<endl; 
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
		if(loc4 >= GET_APPLYVERTEXBUFFERSZ(globalparams.groupid) && keyvalue4.key != INVALIDDATA){
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTREDUCEERRORS
			cout<<"ERROR SEEN @ reduce0:: i: "<<i<<", loc4: "<<loc4<<", keyvalue4.key: "<<keyvalue4.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<GET_APPLYVERTEXBUFFERSZ(globalparams.groupid)<<endl; 
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
		if(loc5 >= GET_APPLYVERTEXBUFFERSZ(globalparams.groupid) && keyvalue5.key != INVALIDDATA){
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTREDUCEERRORS
			cout<<"ERROR SEEN @ reduce0:: i: "<<i<<", loc5: "<<loc5<<", keyvalue5.key: "<<keyvalue5.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<GET_APPLYVERTEXBUFFERSZ(globalparams.groupid)<<endl; 
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
		if(loc6 >= GET_APPLYVERTEXBUFFERSZ(globalparams.groupid) && keyvalue6.key != INVALIDDATA){
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTREDUCEERRORS
			cout<<"ERROR SEEN @ reduce0:: i: "<<i<<", loc6: "<<loc6<<", keyvalue6.key: "<<keyvalue6.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<GET_APPLYVERTEXBUFFERSZ(globalparams.groupid)<<endl; 
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
		if(loc7 >= GET_APPLYVERTEXBUFFERSZ(globalparams.groupid) && keyvalue7.key != INVALIDDATA){
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTREDUCEERRORS
			cout<<"ERROR SEEN @ reduce0:: i: "<<i<<", loc7: "<<loc7<<", keyvalue7.key: "<<keyvalue7.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<GET_APPLYVERTEXBUFFERSZ(globalparams.groupid)<<endl; 
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
		
		loc0 = loc0 / 2;
		loc1 = loc1 / 2;
		loc2 = loc2 / 2;
		loc3 = loc3 / 2;
		loc4 = loc4 / 2;
		loc5 = loc5 / 2;
		loc6 = loc6 / 2;
		loc7 = loc7 / 2;
		
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
		
		value_t temp0 = reducefunc(vprop0.key, vprop0.value, keyvalue0.value, GraphIter, GraphAlgo);
		value_t temp1 = reducefunc(vprop1.key, vprop1.value, keyvalue1.value, GraphIter, GraphAlgo);
		value_t temp2 = reducefunc(vprop2.key, vprop2.value, keyvalue2.value, GraphIter, GraphAlgo);
		value_t temp3 = reducefunc(vprop3.key, vprop3.value, keyvalue3.value, GraphIter, GraphAlgo);
		value_t temp4 = reducefunc(vprop4.key, vprop4.value, keyvalue4.value, GraphIter, GraphAlgo);
		value_t temp5 = reducefunc(vprop5.key, vprop5.value, keyvalue5.value, GraphIter, GraphAlgo);
		value_t temp6 = reducefunc(vprop6.key, vprop6.value, keyvalue6.value, GraphIter, GraphAlgo);
		value_t temp7 = reducefunc(vprop7.key, vprop7.value, keyvalue7.value, GraphIter, GraphAlgo);
		
		vprop0.value = temp0;
		vprop1.value = temp1;
		vprop2.value = temp2;
		vprop3.value = temp3;
		vprop4.value = temp4;
		vprop5.value = temp5;
		vprop6.value = temp6;
		vprop7.value = temp7;
		
		if(keyvalue0.key != INVALIDDATA){ destbuffer[0][loc0] = vprop0; }
		if(keyvalue1.key != INVALIDDATA){ destbuffer[1][loc1] = vprop1; }
		if(keyvalue2.key != INVALIDDATA){ destbuffer[2][loc2] = vprop2; }
		if(keyvalue3.key != INVALIDDATA){ destbuffer[3][loc3] = vprop3; }
		if(keyvalue4.key != INVALIDDATA){ destbuffer[4][loc4] = vprop4; }
		if(keyvalue5.key != INVALIDDATA){ destbuffer[5][loc5] = vprop5; }
		if(keyvalue6.key != INVALIDDATA){ destbuffer[6][loc6] = vprop6; }
		if(keyvalue7.key != INVALIDDATA){ destbuffer[7][loc7] = vprop7; }
		
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsreduced(VECTOR_SIZE);
		if(keyvalue0.key != INVALIDDATA){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(1); }
		if(keyvalue1.key != INVALIDDATA){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(1); }
		if(keyvalue2.key != INVALIDDATA){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(1); }
		if(keyvalue3.key != INVALIDDATA){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(1); }
		if(keyvalue4.key != INVALIDDATA){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(1); }
		if(keyvalue5.key != INVALIDDATA){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(1); }
		if(keyvalue6.key != INVALIDDATA){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(1); }
		if(keyvalue7.key != INVALIDDATA){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(1); }
		#endif
	}
	return;
}

void 
	#ifdef SW 
	actslw::
	#endif
unifydata0(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], unsigned int destbank, unsigned int GraphAlgo){
	if(enable == OFF){ return; }

	UNIFYDATA_LOOP1: for(vector_type v=0; v<VECTOR_SIZE; v++){
		UNIFYDATA_LOOP1B: for(buffer_type i=0; i<PADDEDDESTBUFFER_SIZE; i++){
		// #pragma HLS PIPELINE II=1
			// destbuffer[destbank][i].value += sourcebuffer[v][i].value;
			destbuffer[destbank][i].value = mergefunc(destbuffer[destbank][i].value, sourcebuffer[v][i].value, GraphAlgo);
		}
	}
	return;
}

void 
	#ifdef SW 
	actslw::
	#endif 
savevertices0(bool_type enable, uint512_dt * kvdram, keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], batch_type offset_kvs){
	if(enable == OFF){ return; }
		
	SAVEVERTICES_LOOP: for (buffer_type i=0; i<PADDEDDESTBUFFER_SIZE; i++){
	#pragma HLS PIPELINE II=1
		kvdram[offset_kvs + i].data[0] = buffer[0][i]; 
		kvdram[offset_kvs + i].data[1] = buffer[1][i]; 
		kvdram[offset_kvs + i].data[2] = buffer[2][i]; 
		kvdram[offset_kvs + i].data[3] = buffer[3][i]; 
		kvdram[offset_kvs + i].data[4] = buffer[4][i]; 
		kvdram[offset_kvs + i].data[5] = buffer[5][i]; 
		kvdram[offset_kvs + i].data[6] = buffer[6][i]; 
		kvdram[offset_kvs + i].data[7] = buffer[7][i]; 
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvswritten(VECTOR_SIZE);
		#endif
	}
	#ifdef _DEBUGMODE_KERNELPRINTS2
	cout<< TIMINGRESULTSCOLOR<<"savevertices0:: vertices saved: offset: "<<(offset_kvs - BASEOFFSET_VERTICESDATA_KVS) * VECTOR_SIZE<<", number of vertex datas written: "<<(PADDEDDESTBUFFER_SIZE * VECTOR_SIZE * 2)<< RESET<<endl;
	#endif
	return;
}

// in-memory partition functions
void 
	#ifdef SW 
	actslw::
	#endif
combineSetof1stoSetof20_I0(bool_type enable, keyvalue_t buffer_setof1M0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof1M1[PADDEDDESTBUFFER_SIZE], 
															keyvalue_t buffer_setof2R0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2R1[PADDEDDESTBUFFER_SIZE], 
																skeyvalue_t localcapsuleM[NUM_PARTITIONS], skeyvalue_t localcapsuleN[NUM_PARTITIONS], skeyvalue_t localcapsuleR[NUM_PARTITIONS], globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE / NUM_PARTITIONS;
	buffer_type index=0, begin=0, end=0;
	keyvalue_t NullKV; NullKV.key = INVALIDDATA; NullKV.value = INVALIDDATA;
	
	resetkeyandvalues(localcapsuleR, NUM_PARTITIONS);
	
	EXECUTE_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		buffer_type counter = 0;
		
		EXECUTE_LOOP1B: for(step_type i=0; i<2; i++){
			if(i==0){ begin = localcapsuleM[p].key; end = localcapsuleM[p].key + localcapsuleM[p].value; }
			else { begin = localcapsuleN[p].key; end = localcapsuleN[p].key + localcapsuleN[p].value; }
		
			EXECUTE_LOOP1C: for(buffer_type k=begin; k<end; k++){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("combineSetof1stoSetof20_I0 1", index, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("combineSetof1stoSetof20_I0 2", k, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
				#endif
				if(i==0){
					if(counter % 2 == 0){
						buffer_setof2R0[index] = buffer_setof1M0[k];
					} else {
						buffer_setof2R1[index] = buffer_setof1M0[k];
					}
				} else {
					if(counter % 2 == 0){
						buffer_setof2R0[index] = buffer_setof1M1[k];
					} else {
						buffer_setof2R1[index] = buffer_setof1M1[k];
					}
				}
				counter+=1; if(counter % 2 == 0){ index++; }
			}
		}
		
		localcapsuleR[p].value = localcapsuleM[p].value + localcapsuleN[p].value; 
		if(counter % 2 != 0){ buffer_setof2R1[index] = NullKV; localcapsuleR[p].value += 1; index += 1; } // edge conditions
		#ifdef _DEBUGMODE_STATS
		if(counter % 2 != 0){ actsutilityobj->globalvar_savepartitions_countinvalids(1); }
		#endif 
	}
	
	calculateunallignedoffsets(localcapsuleR, NUM_PARTITIONS, 0, 0);
	#ifdef _DEBUGMODE_CHECKS
	actsutilityobj->scankeyvalues("combineSetof1stoSetof20_I0::buffer_setof2M", (keyvalue_t *)buffer_setof1M, localcapsuleM, NUM_PARTITIONS, GET_BATCH_RANGE(globalparams.groupid) / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof1stoSetof20_I0::buffer_setof2", (keyvalue_t *)buffer_setof2, localcapsuleR, NUM_PARTITIONS, GET_BATCH_RANGE(globalparams.groupid) / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif
combineSetof1stoSetof20_I1(bool_type enable, keyvalue_t buffer_setof1M0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof1M1[PADDEDDESTBUFFER_SIZE], 
															keyvalue_t buffer_setof2R0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2R1[PADDEDDESTBUFFER_SIZE], 
																skeyvalue_t localcapsuleM[NUM_PARTITIONS], skeyvalue_t localcapsuleN[NUM_PARTITIONS], skeyvalue_t localcapsuleR[NUM_PARTITIONS], globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE / NUM_PARTITIONS;
	buffer_type index=0, begin=0, end=0;
	keyvalue_t NullKV; NullKV.key = INVALIDDATA; NullKV.value = INVALIDDATA;
	
	resetkeyandvalues(localcapsuleR, NUM_PARTITIONS);
	
	EXECUTE_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		buffer_type counter = 0;
		
		EXECUTE_LOOP1B: for(step_type i=0; i<2; i++){
			if(i==0){ begin = localcapsuleM[p].key; end = localcapsuleM[p].key + localcapsuleM[p].value; }
			else { begin = localcapsuleN[p].key; end = localcapsuleN[p].key + localcapsuleN[p].value; }
		
			EXECUTE_LOOP1C: for(buffer_type k=begin; k<end; k++){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("combineSetof1stoSetof20_I1 1", index, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("combineSetof1stoSetof20_I1 2", k, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
				#endif
				if(i==0){
					if(counter % 2 == 0){
						buffer_setof2R0[index] = buffer_setof1M0[k];
					} else {
						buffer_setof2R1[index] = buffer_setof1M0[k];
					}
				} else {
					if(counter % 2 == 0){
						buffer_setof2R0[index] = buffer_setof1M1[k];
					} else {
						buffer_setof2R1[index] = buffer_setof1M1[k];
					}
				}
				counter+=1; if(counter % 2 == 0){ index++; }
			}
		}
		
		localcapsuleR[p].value = localcapsuleM[p].value + localcapsuleN[p].value; 
		if(counter % 2 != 0){ buffer_setof2R1[index] = NullKV; localcapsuleR[p].value += 1; index += 1; } // edge conditions
		#ifdef _DEBUGMODE_STATS
		if(counter % 2 != 0){ actsutilityobj->globalvar_savepartitions_countinvalids(1); }
		#endif 
	}
	
	calculateunallignedoffsets(localcapsuleR, NUM_PARTITIONS, 0, 0);
	#ifdef _DEBUGMODE_CHECKS
	actsutilityobj->scankeyvalues("combineSetof1stoSetof20_I1::buffer_setof2M", (keyvalue_t *)buffer_setof1M, localcapsuleM, NUM_PARTITIONS, GET_BATCH_RANGE(globalparams.groupid) / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof1stoSetof20_I1::buffer_setof2", (keyvalue_t *)buffer_setof2, localcapsuleR, NUM_PARTITIONS, GET_BATCH_RANGE(globalparams.groupid) / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif
combineSetof1stoSetof20_I2(bool_type enable, keyvalue_t buffer_setof1M0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof1M1[PADDEDDESTBUFFER_SIZE], 
															keyvalue_t buffer_setof2R0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2R1[PADDEDDESTBUFFER_SIZE], 
																skeyvalue_t localcapsuleM[NUM_PARTITIONS], skeyvalue_t localcapsuleN[NUM_PARTITIONS], skeyvalue_t localcapsuleR[NUM_PARTITIONS], globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE / NUM_PARTITIONS;
	buffer_type index=0, begin=0, end=0;
	keyvalue_t NullKV; NullKV.key = INVALIDDATA; NullKV.value = INVALIDDATA;
	
	resetkeyandvalues(localcapsuleR, NUM_PARTITIONS);
	
	EXECUTE_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		buffer_type counter = 0;
		
		EXECUTE_LOOP1B: for(step_type i=0; i<2; i++){
			if(i==0){ begin = localcapsuleM[p].key; end = localcapsuleM[p].key + localcapsuleM[p].value; }
			else { begin = localcapsuleN[p].key; end = localcapsuleN[p].key + localcapsuleN[p].value; }
		
			EXECUTE_LOOP1C: for(buffer_type k=begin; k<end; k++){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("combineSetof1stoSetof20_I2 1", index, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("combineSetof1stoSetof20_I2 2", k, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
				#endif
				if(i==0){
					if(counter % 2 == 0){
						buffer_setof2R0[index] = buffer_setof1M0[k];
					} else {
						buffer_setof2R1[index] = buffer_setof1M0[k];
					}
				} else {
					if(counter % 2 == 0){
						buffer_setof2R0[index] = buffer_setof1M1[k];
					} else {
						buffer_setof2R1[index] = buffer_setof1M1[k];
					}
				}
				counter+=1; if(counter % 2 == 0){ index++; }
			}
		}
		
		localcapsuleR[p].value = localcapsuleM[p].value + localcapsuleN[p].value; 
		if(counter % 2 != 0){ buffer_setof2R1[index] = NullKV; localcapsuleR[p].value += 1; index += 1; } // edge conditions
		#ifdef _DEBUGMODE_STATS
		if(counter % 2 != 0){ actsutilityobj->globalvar_savepartitions_countinvalids(1); }
		#endif 
	}
	
	calculateunallignedoffsets(localcapsuleR, NUM_PARTITIONS, 0, 0);
	#ifdef _DEBUGMODE_CHECKS
	actsutilityobj->scankeyvalues("combineSetof1stoSetof20_I2::buffer_setof2M", (keyvalue_t *)buffer_setof1M, localcapsuleM, NUM_PARTITIONS, GET_BATCH_RANGE(globalparams.groupid) / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof1stoSetof20_I2::buffer_setof2", (keyvalue_t *)buffer_setof2, localcapsuleR, NUM_PARTITIONS, GET_BATCH_RANGE(globalparams.groupid) / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif
combineSetof1stoSetof20_I3(bool_type enable, keyvalue_t buffer_setof1M0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof1M1[PADDEDDESTBUFFER_SIZE], 
															keyvalue_t buffer_setof2R0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2R1[PADDEDDESTBUFFER_SIZE], 
																skeyvalue_t localcapsuleM[NUM_PARTITIONS], skeyvalue_t localcapsuleN[NUM_PARTITIONS], skeyvalue_t localcapsuleR[NUM_PARTITIONS], globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE / NUM_PARTITIONS;
	buffer_type index=0, begin=0, end=0;
	keyvalue_t NullKV; NullKV.key = INVALIDDATA; NullKV.value = INVALIDDATA;
	
	resetkeyandvalues(localcapsuleR, NUM_PARTITIONS);
	
	EXECUTE_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		buffer_type counter = 0;
		
		EXECUTE_LOOP1B: for(step_type i=0; i<2; i++){
			if(i==0){ begin = localcapsuleM[p].key; end = localcapsuleM[p].key + localcapsuleM[p].value; }
			else { begin = localcapsuleN[p].key; end = localcapsuleN[p].key + localcapsuleN[p].value; }
		
			EXECUTE_LOOP1C: for(buffer_type k=begin; k<end; k++){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("combineSetof1stoSetof20_I3 1", index, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("combineSetof1stoSetof20_I3 2", k, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
				#endif
				if(i==0){
					if(counter % 2 == 0){
						buffer_setof2R0[index] = buffer_setof1M0[k];
					} else {
						buffer_setof2R1[index] = buffer_setof1M0[k];
					}
				} else {
					if(counter % 2 == 0){
						buffer_setof2R0[index] = buffer_setof1M1[k];
					} else {
						buffer_setof2R1[index] = buffer_setof1M1[k];
					}
				}
				counter+=1; if(counter % 2 == 0){ index++; }
			}
		}
		
		localcapsuleR[p].value = localcapsuleM[p].value + localcapsuleN[p].value; 
		if(counter % 2 != 0){ buffer_setof2R1[index] = NullKV; localcapsuleR[p].value += 1; index += 1; } // edge conditions
		#ifdef _DEBUGMODE_STATS
		if(counter % 2 != 0){ actsutilityobj->globalvar_savepartitions_countinvalids(1); }
		#endif 
	}
	
	calculateunallignedoffsets(localcapsuleR, NUM_PARTITIONS, 0, 0);
	#ifdef _DEBUGMODE_CHECKS
	actsutilityobj->scankeyvalues("combineSetof1stoSetof20_I3::buffer_setof2M", (keyvalue_t *)buffer_setof1M, localcapsuleM, NUM_PARTITIONS, GET_BATCH_RANGE(globalparams.groupid) / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof1stoSetof20_I3::buffer_setof2", (keyvalue_t *)buffer_setof2, localcapsuleR, NUM_PARTITIONS, GET_BATCH_RANGE(globalparams.groupid) / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif
combineSetof2stoSetof40_I0(bool_type enable, keyvalue_t buffer_setof2M0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2M1[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2M2[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2M3[PADDEDDESTBUFFER_SIZE], 
															keyvalue_t buffer_setof4R0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4R1[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4R2[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4R3[PADDEDDESTBUFFER_SIZE], 
																skeyvalue_t localcapsuleM[NUM_PARTITIONS], skeyvalue_t localcapsuleN[NUM_PARTITIONS], skeyvalue_t localcapsuleR[NUM_PARTITIONS], globalparams_t globalparams){			
	if(enable == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE / NUM_PARTITIONS;
	buffer_type index=0, begin=0, end=0;
	keyvalue_t NullKV; NullKV.key = INVALIDDATA; NullKV.value = INVALIDDATA;
	
	resetkeyandvalues(localcapsuleR, NUM_PARTITIONS);
	
	EXECUTE_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		buffer_type counter = 0;
		
		EXECUTE_LOOP1B: for(step_type i=0; i<2; i++){
			
			if(i==0){ begin = localcapsuleM[p].key / 2; end = (localcapsuleM[p].key + localcapsuleM[p].value) / 2; }
			else { begin = localcapsuleN[p].key / 2; end = (localcapsuleN[p].key + localcapsuleN[p].value) / 2; }
			
			EXECUTE_LOOP1C: for(buffer_type k=begin; k<end; k++){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("combineSetof2stoSetof40_I0 1", index, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("combineSetof2stoSetof40_I0 2", k, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
				#endif
				if(i==0){
					if(counter % 2 == 0){
						buffer_setof4R0[index] = buffer_setof2M0[k];
						buffer_setof4R1[index] = buffer_setof2M1[k]; 
					} else {
						buffer_setof4R2[index] = buffer_setof2M0[k];
						buffer_setof4R3[index] = buffer_setof2M1[k]; 
					}
				} else {
					if(counter % 2 == 0){
						buffer_setof4R0[index] = buffer_setof2M2[k];
						buffer_setof4R1[index] = buffer_setof2M3[k]; 
					} else {
						buffer_setof4R2[index] = buffer_setof2M2[k];
						buffer_setof4R3[index] = buffer_setof2M3[k]; 
					}
				}
				counter+=1; if(counter % 2 == 0){ index++; }
			}
		}
		
		localcapsuleR[p].value = localcapsuleM[p].value + localcapsuleN[p].value; 
		if(counter % 2 != 0){ buffer_setof4R2[index] = NullKV; buffer_setof4R3[index] = NullKV; localcapsuleR[p].value += 2; index += 1; } // edge conditions
		#ifdef _DEBUGMODE_STATS
		if(counter % 2 != 0){ actsutilityobj->globalvar_savepartitions_countinvalids(2); }
		#endif 
	}
	
	calculateunallignedoffsets(localcapsuleR, NUM_PARTITIONS, 0, 0);
	#ifdef _DEBUGMODE_CHECKS
	actsutilityobj->scankeyvalues("combineSetof2stoSetof40_I0::buffer_setof2M", (keyvalue_t *)buffer_setof2M, localcapsuleM, NUM_PARTITIONS, GET_BATCH_RANGE(globalparams.groupid) / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof2stoSetof40_I0::buffer_setof2N", (keyvalue_t *)buffer_setof2N, localcapsuleN, NUM_PARTITIONS, GET_BATCH_RANGE(globalparams.groupid) / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof2stoSetof40_I0::buffer_setof4", (keyvalue_t *)buffer_setof4, localcapsuleR, NUM_PARTITIONS, GET_BATCH_RANGE(globalparams.groupid) / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif
combineSetof2stoSetof40_I1(bool_type enable, keyvalue_t buffer_setof2M0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2M1[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2M2[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2M3[PADDEDDESTBUFFER_SIZE], 
															keyvalue_t buffer_setof4R0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4R1[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4R2[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4R3[PADDEDDESTBUFFER_SIZE], 
																skeyvalue_t localcapsuleM[NUM_PARTITIONS], skeyvalue_t localcapsuleN[NUM_PARTITIONS], skeyvalue_t localcapsuleR[NUM_PARTITIONS], globalparams_t globalparams){			
	if(enable == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE / NUM_PARTITIONS;
	buffer_type index=0, begin=0, end=0;
	keyvalue_t NullKV; NullKV.key = INVALIDDATA; NullKV.value = INVALIDDATA;
	
	resetkeyandvalues(localcapsuleR, NUM_PARTITIONS);
	
	EXECUTE_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		buffer_type counter = 0;
		
		EXECUTE_LOOP1B: for(step_type i=0; i<2; i++){
			
			if(i==0){ begin = localcapsuleM[p].key / 2; end = (localcapsuleM[p].key + localcapsuleM[p].value) / 2; }
			else { begin = localcapsuleN[p].key / 2; end = (localcapsuleN[p].key + localcapsuleN[p].value) / 2; }
			
			EXECUTE_LOOP1C: for(buffer_type k=begin; k<end; k++){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("combineSetof2stoSetof40_I1 1", index, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("combineSetof2stoSetof40_I1 2", k, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
				#endif
				if(i==0){
					if(counter % 2 == 0){
						buffer_setof4R0[index] = buffer_setof2M0[k];
						buffer_setof4R1[index] = buffer_setof2M1[k]; 
					} else {
						buffer_setof4R2[index] = buffer_setof2M0[k];
						buffer_setof4R3[index] = buffer_setof2M1[k]; 
					}
				} else {
					if(counter % 2 == 0){
						buffer_setof4R0[index] = buffer_setof2M2[k];
						buffer_setof4R1[index] = buffer_setof2M3[k]; 
					} else {
						buffer_setof4R2[index] = buffer_setof2M2[k];
						buffer_setof4R3[index] = buffer_setof2M3[k]; 
					}
				}
				counter+=1; if(counter % 2 == 0){ index++; }
			}
		}
		
		localcapsuleR[p].value = localcapsuleM[p].value + localcapsuleN[p].value; 
		if(counter % 2 != 0){ buffer_setof4R2[index] = NullKV; buffer_setof4R3[index] = NullKV; localcapsuleR[p].value += 2; index += 1; } // edge conditions
		#ifdef _DEBUGMODE_STATS
		if(counter % 2 != 0){ actsutilityobj->globalvar_savepartitions_countinvalids(2); }
		#endif 
	}
	
	calculateunallignedoffsets(localcapsuleR, NUM_PARTITIONS, 0, 0);
	#ifdef _DEBUGMODE_CHECKS
	actsutilityobj->scankeyvalues("combineSetof2stoSetof40_I1::buffer_setof2M", (keyvalue_t *)buffer_setof2M, localcapsuleM, NUM_PARTITIONS, GET_BATCH_RANGE(globalparams.groupid) / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof2stoSetof40_I1::buffer_setof2N", (keyvalue_t *)buffer_setof2N, localcapsuleN, NUM_PARTITIONS, GET_BATCH_RANGE(globalparams.groupid) / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof2stoSetof40_I1::buffer_setof4", (keyvalue_t *)buffer_setof4, localcapsuleR, NUM_PARTITIONS, GET_BATCH_RANGE(globalparams.groupid) / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif
combineSetof4stoSetof80_I0(bool_type enable, keyvalue_t buffer_setof4M0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4M1[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4M2[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4M3[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4M4[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4M5[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4M6[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4M7[PADDEDDESTBUFFER_SIZE],
															keyvalue_t buffer_setof8R0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof8R1[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof8R2[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof8R3[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof8R4[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof8R5[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof8R6[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof8R7[PADDEDDESTBUFFER_SIZE],
																skeyvalue_t localcapsuleM[NUM_PARTITIONS], skeyvalue_t localcapsuleN[NUM_PARTITIONS], skeyvalue_t localcapsuleR[NUM_PARTITIONS], globalparams_t globalparams){				
	if(enable == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE / NUM_PARTITIONS;
	buffer_type index=0, begin=0, end=0;
	keyvalue_t NullKV; NullKV.key = INVALIDDATA; NullKV.value = INVALIDDATA;
	
	resetkeyandvalues(localcapsuleR, NUM_PARTITIONS);
	
	EXECUTE_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		buffer_type counter = 0;
		
		EXECUTE_LOOP1B: for(step_type i=0; i<2; i++){
			if(i==0){ begin = localcapsuleM[p].key / 4; end = (localcapsuleM[p].key + localcapsuleM[p].value) / 4; }
			else { begin = localcapsuleN[p].key / 4; end = (localcapsuleN[p].key + localcapsuleN[p].value) / 4; }
			
			EXECUTE_LOOP1C: for(buffer_type k=begin; k<end; k++){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("combineSetof4stoSetof80_I0 1", index, PADDEDDESTBUFFER_SIZE, p, counter, NAp);
				actsutilityobj->checkoutofbounds("combineSetof4stoSetof80_I0 2", k, PADDEDDESTBUFFER_SIZE, p, counter, NAp);
				#endif 
				if(i==0){
					if(counter % 2 == 0){
						buffer_setof8R0[index] = buffer_setof4M0[k];
						buffer_setof8R1[index] = buffer_setof4M1[k]; 
						buffer_setof8R2[index] = buffer_setof4M2[k]; 
						buffer_setof8R3[index] = buffer_setof4M3[k];
					} else {
						buffer_setof8R4[index] = buffer_setof4M0[k];
						buffer_setof8R5[index] = buffer_setof4M1[k]; 
						buffer_setof8R6[index] = buffer_setof4M2[k]; 
						buffer_setof8R7[index] = buffer_setof4M3[k];
					}
				} else {
					if(counter % 2 == 0){
						buffer_setof8R0[index] = buffer_setof4M4[k];
						buffer_setof8R1[index] = buffer_setof4M5[k]; 
						buffer_setof8R2[index] = buffer_setof4M6[k]; 
						buffer_setof8R3[index] = buffer_setof4M7[k];
					} else {
						buffer_setof8R4[index] = buffer_setof4M4[k];
						buffer_setof8R5[index] = buffer_setof4M5[k]; 
						buffer_setof8R6[index] = buffer_setof4M6[k]; 
						buffer_setof8R7[index] = buffer_setof4M7[k];
					}
				}
				counter+=1; if(counter % 2 == 0){ index++; }
			}
		}
		
		localcapsuleR[p].value = localcapsuleM[p].value + localcapsuleN[p].value; 
		if(counter % 2 != 0){ buffer_setof8R4[index] = NullKV; buffer_setof8R5[index] = NullKV; 
			buffer_setof8R6[index] = NullKV; buffer_setof8R7[index] = NullKV;
			localcapsuleR[p].value += 4; index += 1; }
		#ifdef _DEBUGMODE_STATS
		if(counter % 2 != 0){ actsutilityobj->globalvar_savepartitions_countinvalids(4); }
		#endif 
	}
	
	calculateunallignedoffsets(localcapsuleR, NUM_PARTITIONS, 0, 0);
	#ifdef _DEBUGMODE_CHECKS
	actsutilityobj->scankeyvalues("combineSetof4stoSetof80_I0::buffer_setof2M", (keyvalue_t *)buffer_setof8, localcapsuleR, NUM_PARTITIONS, GET_BATCH_RANGE(globalparams.groupid) / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	return;
}

// group functions
void 
	#ifdef SW 
	actslw::
	#endif
combineSetof1stoSetof2s0(bool_type enable, keyvalue_t buffer_setof1[8][PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2[8][PADDEDDESTBUFFER_SIZE], skeyvalue_t templocalcapsule_so1[8][NUM_PARTITIONS], skeyvalue_t templocalcapsule_so2[4][NUM_PARTITIONS], globalparams_t globalparams){
	#pragma HLS INLINE
	// 1s->2s
	combineSetof1stoSetof20_I0(enable, buffer_setof1[0], buffer_setof1[1], buffer_setof2[0], buffer_setof2[1], templocalcapsule_so1[0], templocalcapsule_so1[1], templocalcapsule_so2[0], globalparams);
	combineSetof1stoSetof20_I1(enable, buffer_setof1[2], buffer_setof1[3], buffer_setof2[2], buffer_setof2[3], templocalcapsule_so1[2], templocalcapsule_so1[3], templocalcapsule_so2[1], globalparams);
	combineSetof1stoSetof20_I2(enable, buffer_setof1[4], buffer_setof1[5], buffer_setof2[4], buffer_setof2[5], templocalcapsule_so1[4], templocalcapsule_so1[5], templocalcapsule_so2[2], globalparams);
	combineSetof1stoSetof20_I3(enable, buffer_setof1[6], buffer_setof1[7], buffer_setof2[6], buffer_setof2[7], templocalcapsule_so1[6], templocalcapsule_so1[7], templocalcapsule_so2[3], globalparams);
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif
combineSetof2stoSetof4s0(bool_type enable, keyvalue_t buffer_setof2[8][PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4[8][PADDEDDESTBUFFER_SIZE], skeyvalue_t templocalcapsule_so2[4][NUM_PARTITIONS], skeyvalue_t templocalcapsule_so4[2][NUM_PARTITIONS], globalparams_t globalparams){
	#pragma HLS INLINE
	// 2s->4s
	combineSetof2stoSetof40_I0(enable, buffer_setof2[0], buffer_setof2[1], buffer_setof2[2], buffer_setof2[3], buffer_setof4[0], buffer_setof4[1], buffer_setof4[2], buffer_setof4[3], templocalcapsule_so2[0], templocalcapsule_so2[1], templocalcapsule_so4[0], globalparams);
	combineSetof2stoSetof40_I1(enable, buffer_setof2[4], buffer_setof2[5], buffer_setof2[6], buffer_setof2[7], buffer_setof4[4], buffer_setof4[5], buffer_setof4[6], buffer_setof4[7], templocalcapsule_so2[2], templocalcapsule_so2[3], templocalcapsule_so4[1], globalparams);
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif
combineSetof4stoSetof8s0(bool_type enable, keyvalue_t buffer_setof4[8][PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof8[8][PADDEDDESTBUFFER_SIZE], skeyvalue_t templocalcapsule_so4[2][NUM_PARTITIONS], skeyvalue_t templocalcapsule_so8[NUM_PARTITIONS], globalparams_t globalparams){
	#pragma HLS INLINE
	// 4s->8s
	combineSetof4stoSetof80_I0(enable, buffer_setof4[0], buffer_setof4[1], buffer_setof4[2], buffer_setof4[3], buffer_setof4[4], buffer_setof4[5], buffer_setof4[6], buffer_setof4[7], 
												buffer_setof8[0], buffer_setof8[1], buffer_setof8[2], buffer_setof8[3], buffer_setof8[4], buffer_setof8[5], buffer_setof8[6], buffer_setof8[7], 
													templocalcapsule_so4[0], templocalcapsule_so4[1], templocalcapsule_so8, globalparams);
	return;
}

// main function
void 
	#ifdef SW 
	actslw::
	#endif 
dispatch0(uint512_dt * kvdram){
	analysis_type analysis_mainloop = MAXKVDATA_BATCHSIZE_KVS / (NUMPIPELINES * SRCBUFFER_SIZE);
	analysis_type analysis_numllops = 1;
	analysis_type analysis_numsourcepartitions = 1;
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->printparameters();
	actsutilityobj->printglobalvars();
	actsutilityobj->clearglobalvars();
	#endif
	
	keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = sourcebuffer
	
	keyvalue_t buffer_setof1[8][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer_setof1
	skeyvalue_t templocalcapsule_so1[8][NUM_PARTITIONS];
	#pragma HLS array_partition variable = templocalcapsule_so1
	
	keyvalue_t buffer_setof2[8][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer_setof2
	skeyvalue_t templocalcapsule_so2[4][NUM_PARTITIONS];
	#pragma HLS array_partition variable = templocalcapsule_so2
	
	keyvalue_t buffer_setof4[8][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer_setof4
	skeyvalue_t templocalcapsule_so4[2][NUM_PARTITIONS];
	#pragma HLS array_partition variable = templocalcapsule_so4
	
	keyvalue_t buffer_setof8[8][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer_setof8
	skeyvalue_t templocalcapsule_so8[NUM_PARTITIONS];

	keyvalue_t globalstatsbuffer[NUM_PARTITIONS];
	batch_type skipsizes[NUM_PARTITIONS];
	
	batch_type sourcestatsmarker = 0;
	batch_type deststatsmarker = 1;
	batch_type destoffset = 0;
	config_t config;
	globalparams_t globalparams = getglobalparams(kvdram);
	sweepparams_t sweepparams;
	
	actsutilityobj->printkeyvalues("dispatch0.kvdram", (keyvalue_t *)(&kvdram[BASEOFFSET_KVDRAM_KVS]), 16); // REMOVEME.

	// start launch
	MAIN_LOOP1: for(step_type currentLOP=1; currentLOP<=globalparams.treedepth; currentLOP++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_numllops avg=analysis_numllops	
	
		batch_type num_source_partitions = get_num_source_partitions(currentLOP);
		
		// resetmanykeyandvalues(buffer_setof1, PADDEDDESTBUFFER_SIZE); // FIXME? shouldn't this be within MAIN_LOOP1B???
		// resetmanykeyandvalues(globalstatsbuffer, NUM_PARTITIONS); // REMOVEME?
		
		destoffset = 0;
		
		MAIN_LOOP1B: for(batch_type source_partition=0; source_partition<num_source_partitions; source_partition+=1){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_numsourcepartitions avg=analysis_numsourcepartitions	
		
			resetmanykeyandvalues(buffer_setof1, NUM_PARTITIONS); // FIXME? small bug when this is outside [resetmanykeyandvalues(buffer_setof1, PADDEDDESTBUFFER_SIZE);]
			resetmanykeyandvalues(globalstatsbuffer, NUM_PARTITIONS);
		
			sweepparams = getsweepparams(globalparams, currentLOP, source_partition);
			travstate_t travstate = gettravstate(kvdram, globalparams, currentLOP, sourcestatsmarker);
			travstate_t CStravstate = travstate;
			travstate_t Ptravstate = travstate;
			travstate_t Rtravstate = travstate;
			#ifdef _DEBUGMODE_KERNELPRINTS2
			actsutilityobj->setstructs(config, sweepparams, travstate);
			#endif
			
			// collect stats
			if(currentLOP >= 0 && currentLOP <= GET_TREE_DEPTH(globalparams.groupid)){ config.enablecollectglobalstats = ON; config.enablepartition = OFF; config.enablereduce = OFF; } 
			else { CStravstate.begin_kvs = 0; CStravstate.end_kvs = 0; config.enablecollectglobalstats = OFF; }
			#ifdef _DEBUGMODE_KERNELPRINTS2
			if(config.enablecollectglobalstats == ON){ actsutilityobj->print7("### dispatch0::collectgstats:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, CStravstate.begin_kvs * VECTOR_SIZE, CStravstate.end_kvs * VECTOR_SIZE, (CStravstate.end_kvs - CStravstate.begin_kvs) * VECTOR_SIZE, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams.currentLOP)), sweepparams.currentLOP); }					
			#endif
			MAIN_LOOP1C_COLLECTGLOBALSTATS: for(batch_type offset_kvs=CStravstate.begin_kvs; offset_kvs<CStravstate.end_kvs; offset_kvs+=CStravstate.skip_kvs){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_mainloop avg=analysis_mainloop
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print4("### dispatch0::collectglobalstats:: offset_kvs", "begin_kvs", "end_kvs", "skip", offset_kvs, CStravstate.begin_kvs, CStravstate.end_kvs, SRCBUFFER_SIZE);
				#endif
				
				CStravstate.i_kvs = offset_kvs;
				
				readkeyvalues0(ON, kvdram, sourcebuffer, (sweepparams.worksourcebaseaddress_kvs + offset_kvs), CStravstate);
				
				collectglobalstats0(ON, sourcebuffer, buffer_setof1, sweepparams.currentLOP, sweepparams.upperlimit, globalparams);
			}
			prepareglobalstats0(config.enablecollectglobalstats, buffer_setof1, globalstatsbuffer, globalparams);
			for(unsigned int i=0; i<NUM_PARTITIONS; i++){ skipsizes[i] = ((((globalstatsbuffer[i].value / VECTOR_SIZE) + 1) / SRCBUFFER_SIZE) * (NUM_PARTITIONS * VECTOR_SIZE * 2)) + 128; } //'128' is safety padd
			calculateoffsets(globalstatsbuffer, NUM_PARTITIONS, destoffset, skipsizes);
			resetvalues(globalstatsbuffer, NUM_PARTITIONS);
			saveglobalstats0(config.enablecollectglobalstats, kvdram, globalstatsbuffer, BASEOFFSET_STATSDRAM_KVS + deststatsmarker);
			
			// partition
			if(currentLOP >= 0 && currentLOP <= GET_TREE_DEPTH(globalparams.groupid)){ config.enablecollectglobalstats = OFF; config.enablepartition = ON; config.enablereduce = OFF; } 
			else { Ptravstate.begin_kvs = 0; Ptravstate.end_kvs = 0; config.enablepartition = OFF; }
			#ifdef _DEBUGMODE_KERNELPRINTS2
			if(config.enablepartition == ON){ actsutilityobj->print7("### dispatch0::partition:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, Ptravstate.begin_kvs * VECTOR_SIZE, Ptravstate.end_kvs * VECTOR_SIZE, (Ptravstate.end_kvs - Ptravstate.begin_kvs) * VECTOR_SIZE, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams.currentLOP)), sweepparams.currentLOP);	}				
			#endif
			readglobalstats0(config.enablepartition, kvdram, globalstatsbuffer, BASEOFFSET_STATSDRAM_KVS + deststatsmarker);
			resetvalues(globalstatsbuffer, NUM_PARTITIONS);
			MAIN_LOOP1D_PARTITION: for(batch_type offset_kvs=Ptravstate.begin_kvs; offset_kvs<Ptravstate.end_kvs; offset_kvs+=Ptravstate.skip_kvs){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_mainloop avg=analysis_mainloop
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print4("### dispatch0::partition:: offset_kvs", "begin_kvs", "end_kvs", "skip", offset_kvs, Ptravstate.begin_kvs, Ptravstate.end_kvs, SRCBUFFER_SIZE);
				#endif
				
				Ptravstate.i_kvs = offset_kvs;
				
				readkeyvalues0(ON, kvdram, sourcebuffer, (sweepparams.worksourcebaseaddress_kvs + offset_kvs), Ptravstate);
			
				partitionkeyvalues0(ON, sourcebuffer, buffer_setof1, templocalcapsule_so1, sweepparams.currentLOP, sweepparams.upperlimit, Ptravstate, globalparams);

				// 1s->2s
				combineSetof1stoSetof2s0(ON, buffer_setof1, buffer_setof2, templocalcapsule_so1, templocalcapsule_so2, globalparams);

				// 2s->4s
				combineSetof2stoSetof4s0(ON, buffer_setof2, buffer_setof4, templocalcapsule_so2, templocalcapsule_so4, globalparams);
				
				// 4s->8s
				combineSetof4stoSetof8s0(ON, buffer_setof4, buffer_setof8, templocalcapsule_so4, templocalcapsule_so8, globalparams);
				
				savekeyvalues0(ON, kvdram, buffer_setof8, globalstatsbuffer, templocalcapsule_so8, sweepparams.workdestbaseaddress_kvs, globalparams);
				///// overflow. pipeline continuing from top...
			}
			saveglobalstats0(config.enablepartition, kvdram, globalstatsbuffer, BASEOFFSET_STATSDRAM_KVS + deststatsmarker);
			
			// reduce 
			if(currentLOP == (GET_TREE_DEPTH(globalparams.groupid) + 1)){ config.enablecollectglobalstats = OFF; config.enablepartition = OFF; config.enablereduce = ON; } 
			else { Rtravstate.begin_kvs = 0; Rtravstate.end_kvs = 0; config.enablereduce = OFF; }
			#ifdef _DEBUGMODE_KERNELPRINTS2
			if(config.enablereduce == ON){ actsutilityobj->print7("### dispatch0::reduce:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, Rtravstate.begin_kvs * VECTOR_SIZE, Rtravstate.end_kvs * VECTOR_SIZE, (Rtravstate.end_kvs - Rtravstate.begin_kvs) * VECTOR_SIZE, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams.currentLOP)), sweepparams.currentLOP); }							
			#endif
			
			if((source_partition % 8) == 0){ readvertices0(config.enablereduce, kvdram, sourcebuffer, (BASEOFFSET_VERTICESDATA_KVS + (source_partition * (GET_APPLYVERTEXBUFFERSZ_KVS(globalparams.groupid) / 2)))); }
			replicatedata0(config.enablereduce, sourcebuffer, buffer_setof2, (source_partition % 8)); // FIXME '0' // (source_partition % 8)
			MAIN_LOOP1E_REDUCE: for(batch_type offset_kvs=Rtravstate.begin_kvs; offset_kvs<Rtravstate.end_kvs; offset_kvs+=Rtravstate.skip_kvs){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_mainloop avg=analysis_mainloop
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print4("### dispatch0::reduce:: offset_kvs", "begin_kvs", "end_kvs", "skip", offset_kvs, Rtravstate.begin_kvs, Rtravstate.end_kvs, SRCBUFFER_SIZE);
				#endif
				
				Rtravstate.i_kvs = offset_kvs;
	
				readkeyvalues0(ON, kvdram, buffer_setof1, (sweepparams.worksourcebaseaddress_kvs + offset_kvs), Rtravstate);
			
				reduce0(ON, buffer_setof1, buffer_setof2, sweepparams.upperlimit, globalparams.GraphIter, globalparams.GraphAlgo, Rtravstate, globalparams);
			}
			unifydata0(config.enablereduce, buffer_setof2, buffer_setof4, (source_partition % 8), globalparams.GraphAlgo); // FIXME '0' // (source_partition % 8)
			if((source_partition % 8) == 7){ savevertices0(config.enablereduce, kvdram, buffer_setof4, (BASEOFFSET_VERTICESDATA_KVS + (source_partition * (GET_APPLYVERTEXBUFFERSZ_KVS(globalparams.groupid) / 2)))); }
			
			sourcestatsmarker += 1;
			deststatsmarker += NUM_PARTITIONS;
			destoffset = globalstatsbuffer[NUM_PARTITIONS-1].key + globalstatsbuffer[NUM_PARTITIONS-1].value + 64;
			
			#ifdef _DEBUGMODE_KERNELPRINTS2
			if(config.enablereduce == OFF){ actsutilityobj->printpartitionresult2(OFF, kvdram, globalstatsbuffer, sweepparams); }
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
topkernel( uint512_dt * sourceAvolume ){
#pragma HLS INTERFACE m_axi port = sourceAvolume offset = slave bundle = gmem0 // max_read_burst_length=64 max_write_burst_length=64

#pragma HLS INTERFACE s_axilite port = sourceAvolume bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATA_PACK variable = sourceAvolume

	#ifdef _DEBUGMODE_KERNELPRINTS2
	cout<<"Light weight ACTS started."<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"... Acts Launched... size: "<<sourceAvolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key<<endl; 
	#endif 
	
	dispatch0(sourceAvolume);
	return;
}
}







