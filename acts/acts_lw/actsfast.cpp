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
#include "../../include/config_params.h"
#include "../../include/common.h"
#include "../include/actscommon.h"
#ifndef FPGA_IMPL
#include "../../src/utility/utility.h"
#endif 
#ifndef HW
#include "../../acts/actsutility/actsutility.h"
#endif 
#include "actsfast.h"
using namespace std;

#define NUMACTSFASTPIPELINES 2 // CRITICAL REMOVEME? // INVESTIGATEME. cause of long latency?
#if NUMACTSFASTPIPELINES==1
#define FPP0
#endif 
#if NUMACTSFASTPIPELINES==2
#define FPP0
#define FPP1
#endif

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
amin(unsigned int val1, unsigned int val2){
	if(val1 < val2){ return val1; }
	else { return val2; }
}
batch_type
	#ifdef SW 
	actslw::
	#endif
allignlower_KV(batch_type val){
	batch_type fac = val / VECTOR_SIZE;
	return (fac * VECTOR_SIZE);
}
batch_type
	#ifdef SW 
	actslw::
	#endif 
allignhigher_KV(batch_type val){
	batch_type fac = (val + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	return (fac * VECTOR_SIZE);
}
batch_type
	#ifdef SW 
	actslw::
	#endif
allignlowerto4_KV(batch_type val){
	batch_type fac = val / 4;
	return (fac * 4);
}
batch_type
	#ifdef SW 
	actslw::
	#endif 
allignhigherto4_KV(batch_type val){
	batch_type fac = (val + (4 - 1)) / 4;
	return (fac * 4);
}
batch_type
	#ifdef SW 
	actslw::
	#endif
getskipsize(step_type currentLOP, bool_type sourceORdest, globalparams_t globalparams){
	analysis_type analysis_treedepth = TREE_DEPTH;
	batch_type result;
	
	if(currentLOP == 0){ currentLOP = 1; }
	if(sourceORdest == SOURCE){ result = globalparams.LLOPnumpartitions; }
	else if (sourceORdest == DEST){ result = globalparams.LLOPnumpartitions / NUM_PARTITIONS; } // FIXME. use TREE_DEPTH for less lut?
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
resetkeyandvalues(skeyvalue_t * buffer, buffer_type size, unsigned int resetval){
	for(buffer_type i=0; i<size; i++){ buffer[i].key = resetval; buffer[i].value = resetval; }
}
void 
	#ifdef SW 
	actslw::
	#endif 
resetvalues(keyvalue_t * buffer, buffer_type size, unsigned int resetval){
	for(buffer_type i=0; i<size; i++){ buffer[i].value = resetval; }
}
void 
	#ifdef SW 
	actslw::
	#endif 
resetvalues(value_t * buffer, buffer_type size, unsigned int resetval){
	for(buffer_type i=0; i<size; i++){ buffer[i] = resetval; }
}
void 
	#ifdef SW 
	actslw::
	#endif 
resetmanykeyandvalues(skeyvalue_t buffer[VECTOR_SIZE][NUM_PARTITIONS], buffer_type size, unsigned int resetval){
	for(buffer_type i=0; i<size; i++){
		buffer[0][i].key = resetval; buffer[0][i].value = resetval; 
		buffer[1][i].key = resetval; buffer[1][i].value = resetval; 
		buffer[2][i].key = resetval; buffer[2][i].value = resetval; 
		buffer[3][i].key = resetval; buffer[3][i].value = resetval; 
		buffer[4][i].key = resetval; buffer[4][i].value = resetval; 
		buffer[5][i].key = resetval; buffer[5][i].value = resetval; 
		buffer[6][i].key = resetval; buffer[6][i].value = resetval; 
		buffer[7][i].key = resetval; buffer[7][i].value = resetval; 
	}
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif 
resetmanykeyandvalues(keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], buffer_type size, unsigned int resetval){
	for(buffer_type i=0; i<size; i++){
		buffer[0][i].key = resetval; buffer[0][i].value = resetval; 
		buffer[1][i].key = resetval; buffer[1][i].value = resetval; 
		buffer[2][i].key = resetval; buffer[2][i].value = resetval; 
		buffer[3][i].key = resetval; buffer[3][i].value = resetval; 
		buffer[4][i].key = resetval; buffer[4][i].value = resetval; 
		buffer[5][i].key = resetval; buffer[5][i].value = resetval; 
		buffer[6][i].key = resetval; buffer[6][i].value = resetval; 
		buffer[7][i].key = resetval; buffer[7][i].value = resetval; 
	}
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif 
resetmanykeyandvalues(keyvalue_t * buffer, buffer_type size, unsigned int resetval){
	for(buffer_type i=0; i<size; i++){
		buffer[i].key = resetval; buffer[i].value = resetval; 
	}
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif 
resetmanyvalues(skeyvalue_t buffer[VECTOR_SIZE][NUM_PARTITIONS], buffer_type size, unsigned int resetval){
	for(buffer_type i=0; i<size; i++){ 
		buffer[0][i].value = resetval;
		buffer[1][i].value = resetval;
		buffer[2][i].value = resetval;
		buffer[3][i].value = resetval;
		buffer[4][i].value = resetval;
		buffer[5][i].value = resetval;
		buffer[6][i].value = resetval;
		buffer[7][i].value = resetval;
	}
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif 
resetmanyvalues(buffer_type buffer0[NUM_PARTITIONS],buffer_type buffer1[NUM_PARTITIONS],buffer_type buffer2[NUM_PARTITIONS],buffer_type buffer3[NUM_PARTITIONS],buffer_type buffer4[NUM_PARTITIONS],buffer_type buffer5[NUM_PARTITIONS],buffer_type buffer6[NUM_PARTITIONS],buffer_type buffer7[NUM_PARTITIONS], buffer_type size, unsigned int resetval){
	for(buffer_type i=0; i<size; i++){ 
		buffer0[i] = resetval;
		buffer1[i] = resetval;
		buffer2[i] = resetval;
		buffer3[i] = resetval;
		buffer4[i] = resetval;
		buffer5[i] = resetval;
		buffer6[i] = resetval;
		buffer7[i] = resetval;
	}
	return;
}
buffer_type 
	#ifdef SW 
	actslw::
	#endif 
getchunksize_kvs(buffer_type buffer_size, travstate_t travstate, buffer_type localoffset){
	buffer_type chunk_size = buffer_size;
	batch_type i = travstate.i_kvs + localoffset;
	if (i > travstate.end_kvs){ chunk_size = 0; }
	else if ((i + buffer_size) > travstate.end_kvs){ chunk_size = travstate.end_kvs - i; }
	else {}
	return chunk_size;
}
buffer_type 
	#ifdef SW 
	actslw::
	#endif 
getchunksize(buffer_type buffersz, travstate_t travstate, buffer_type localoffset){
	buffer_type chunksz = buffersz;
	batch_type i = travstate.i + localoffset;
	if (i > travstate.end){ chunksz = 0; }
	else if ((i + buffersz) > travstate.end){ chunksz = travstate.end - i; }
	else {}
	return chunksz;
}
partition_type
	#ifdef SW 
	actslw::
	#endif 
getpartition(bool_type enable, keyvalue_t keyvalue, step_type currentLOP, vertex_t upperlimit, unsigned int batch_range_pow){
	partition_type partition;
	#ifdef OPTMZ
	if(enable == ON){
		partition = ((keyvalue.key - upperlimit) >> (batch_range_pow - (NUM_PARTITIONS_POW * currentLOP)));
	} else {
		partition = 0;
	}
	#else 
	partition = ((keyvalue.key - upperlimit) >> (batch_range_pow - (NUM_PARTITIONS_POW * currentLOP)));
	#endif 
	
	#ifdef _DEBUGMODE_CHECKS2
	if(partition >= NUM_PARTITIONS){ actsutilityobj->globalstats_counterrorsingetpartition(1); }
	#endif 
	
	#ifdef ENABLE_PERFECTACCURACY
		#ifdef _DEBUGMODE_CHECKS2
		if(partition >= NUM_PARTITIONS){ cout<<"getpartition::ERROR 1. partition out of bounds partition: "<<partition<<", keyvalue.key: "<<keyvalue.key<<", NUM_PARTITIONS: "<<NUM_PARTITIONS<<", keyvalue.key: "<<keyvalue.key<<", upperlimit: "<<upperlimit<<", currentLOP: "<<currentLOP<<", batch_range_pow: "<<batch_range_pow<<endl; exit(EXIT_FAILURE); }
		#endif
	#endif 
	#ifndef ENABLE_PERFECTACCURACY
		if(partition >= NUM_PARTITIONS){ partition = (((1 << NUM_PARTITIONS_POW) - 1) & (partition >> (1 - 1))); } // FIXME. REMOVEME. PERFECTIONTEST.
	#endif
	
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("actslw::getpartition 2", partition, NUM_PARTITIONS, keyvalue.key, upperlimit, currentLOP);
	#endif
	return partition;
}
partition_type
	#ifdef SW 
	actslw::
	#endif 
getglobalpartition(keyvalue_t keyvalue, step_type currentLOP, vertex_t upperlimit, unsigned int batch_range_pow){
	partition_type partition = ((keyvalue.key - upperlimit) >> (batch_range_pow - (NUM_PARTITIONS_POW * 2))); // NEWCHANGE.
	// partition_type partition = ((keyvalue.key - upperlimit) >> (batch_range_pow - (NUM_PARTITIONS_POW * TREE_DEPTH)));
	
	#ifdef _DEBUGMODE_CHECKS2
	// actsutilityobj->checkoutofbounds("actslw::getglobalpartition", partition, (1 << (NUM_PARTITIONS_POW * TREE_DEPTH)), keyvalue.key, upperlimit, currentLOP);
	actsutilityobj->checkoutofbounds("actslw::getglobalpartition", partition, 256, keyvalue.key, upperlimit, currentLOP);
	#endif
	return partition;
}
value_t 
	#ifdef SW 
	actslw::
	#endif 
reducefunc(value_t vtemp, value_t res, unsigned int GraphIter, unsigned int GraphAlgo){
	value_t temp = 0;
	#ifdef PR_ALGORITHM
	temp = vtemp + res;
	#elif defined(BFS_ALGORITHM)
	temp = amin(vtemp, GraphIter);
	#elif defined(SSSP_ALGORITHM)
	temp = amin(vtemp, res);
	#elif defined(BC_ALGORITHM)
	temp = amin(vtemp, res);
	#endif
	return temp;
	
	// value_t temp = 0;
	// if(GraphAlgo == PAGERANK){
		// temp = vtemp + res;
	// } else if (GraphAlgo == BREADTHFIRSTSEARCH){
		// temp = amin(vtemp, GraphIter);
	// } else if (GraphAlgo == SSSP){
		// temp = amin(vtemp, res);
	// } else { temp = 0; }
	// return temp;
}
value_t 
	#ifdef SW 
	actslw::
	#endif 
processedgefunc(value_t Uprop, unsigned int edgeweight, unsigned int voutdegree, unsigned int GraphIter, unsigned int GraphAlgo){
	// value_t ret = 0;
	// #ifdef PR_ALGORITHM
	// ret = Uprop;
	// #elif defined(BFS_ALGORITHM)
	// ret = NAp;
	// #elif defined(SSSP_ALGORITHM)
	// ret = Uprop + edgeweight;
	// #elif defined(BC_ALGORITHM)
	// ret = Uprop + edgeweight;
	// #endif
	// return ret;
	
	value_t res = 0;
	#ifdef PR_ALGORITHM
	res = Uprop / voutdegree;
	#elif defined(BFS_ALGORITHM)
	res = NAp;
	#elif defined(SSSP_ALGORITHM)
	res = Uprop + edgeweight;
	#elif defined(BC_ALGORITHM)
	res = Uprop + edgeweight;
	#endif
	return res;
	
	// value_t res = 0;
	// if(GraphAlgo == PAGERANK){
		// res = Uprop / voutdegree;
	// } else if (GraphAlgo == BREADTHFIRSTSEARCH){
		// res = NAp;
	// } else if (GraphAlgo == SSSP){
		// res = Uprop + edgeweight;
	// } else { res = 0; }
	// return res;
}
value_t 
	#ifdef SW 
	actslw::
	#endif 
mergefunc(value_t value1, value_t value2, unsigned int GraphAlgo){
	value_t res = 0;
	#ifdef PR_ALGORITHM
	res = value1 + value2;
	#elif defined(BFS_ALGORITHM)
	res = amin(value1, value2);
	#elif defined(SSSP_ALGORITHM)
	res = amin(value1, value2);
	#elif defined(BC_ALGORITHM)
	res = amin(value1, value2);
	#endif
	return res;
	
	// value_t res = 0;
	// if(GraphAlgo == PAGERANK){
		// res = value1 + value2;
	// } else if (GraphAlgo == BREADTHFIRSTSEARCH){
		// res = amin(value1, value2);
	// } else if (GraphAlgo == SSSP){
		// res = amin(value1, value2);
	// } else { res = 0; }
	// return res;
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
getpartitionwritesz(buffer_type realsize_kvs, buffer_type bramoffset_kvs){
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
		
		else if(realsize_kvs == PADDEDDESTBUFFER_SIZE){ size_kvs = PADDEDDESTBUFFER_SIZE; }
		
		else {
			cout<<"WARNING:getpartitionwritesz: should not get here. something might be wrong. realsize_kvs: "<<realsize_kvs<<", size_kvs: "<<size_kvs<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl;				 
			
			#ifdef ENABLE_PERFECTACCURACY
			#ifdef _DEBUGMODE_CHECKS2
			cout<<"WARNING:getpartitionwritesz: should not get here. something might be wrong. realsize_kvs: "<<realsize_kvs<<", size_kvs: "<<size_kvs<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl;				 
			exit(EXIT_FAILURE);
			#endif
			#else
			size_kvs = PADDEDDESTBUFFER_SIZE / NUM_PARTITIONS;
			#endif 
		}
		#if defined(SW) || defined(SWEMU) || defined(HW) // NEWCHANGE.
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
	// #pragma HLS INLINE
	if(val1 < val2){ return 1; }
	else { return 0; }
}
void 
	#ifdef SW 
	actslw::
	#endif 
calculateoffsets(keyvalue_t * buffer, buffer_type size, batch_type base, batch_type skipspacing[NUM_PARTITIONS]){
	unsigned int analysis_size = NUMLASTLEVELPARTITIONS;
	
	buffer[0].key += base;
	for(buffer_type i=1; i<size; i++){ 
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_size avg=analysis_size	
		buffer[i].key = allignhigher_KV(buffer[i-1].key + buffer[i-1].value + skipspacing[i-1]); 
	}
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif 
calculateunallignedoffsets(skeyvalue_t buffer[NUM_PARTITIONS], buffer_type size, batch_type base, batch_type skipspacing){
	for(buffer_type i=1; i<size; i++){ 
		buffer[i].key = buffer[i-1].key + buffer[i-1].value + skipspacing; 
	}
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif 
calculatemanyoffsets(skeyvalue_t buffer[VECTOR_SIZE][NUM_PARTITIONS], buffer_type size, batch_type base, batch_type skipspacing){
	for(buffer_type i=1; i<size; i++){ 
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
calculatemanyoffsets_allignby4(skeyvalue_t buffer[VECTOR_SIZE][NUM_PARTITIONS], buffer_type size, batch_type base, batch_type skipspacing){
	for(buffer_type i=1; i<size; i++){ 
		buffer[0][i].key = allignhigherto4_KV(buffer[0][i-1].key + buffer[0][i-1].value + skipspacing); 
		buffer[1][i].key = allignhigherto4_KV(buffer[1][i-1].key + buffer[1][i-1].value + skipspacing); 
		buffer[2][i].key = allignhigherto4_KV(buffer[2][i-1].key + buffer[2][i-1].value + skipspacing); 
		buffer[3][i].key = allignhigherto4_KV(buffer[3][i-1].key + buffer[3][i-1].value + skipspacing); 
		buffer[4][i].key = allignhigherto4_KV(buffer[4][i-1].key + buffer[4][i-1].value + skipspacing); 
		buffer[5][i].key = allignhigherto4_KV(buffer[5][i-1].key + buffer[5][i-1].value + skipspacing); 
		buffer[6][i].key = allignhigherto4_KV(buffer[6][i-1].key + buffer[6][i-1].value + skipspacing); 
		buffer[7][i].key = allignhigherto4_KV(buffer[7][i-1].key + buffer[7][i-1].value + skipspacing); 
	}
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif 
calculatemanyoffsets_allignby4_lower(skeyvalue_t buffer[VECTOR_SIZE][NUM_PARTITIONS], buffer_type limitsz[VECTOR_SIZE][NUM_PARTITIONS], buffer_type size, buffer_type cutoff){
	analysis_type analysis_loopcount = NUM_PARTITIONS;
	
	for(buffer_type i=0; i<size; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount	
	#pragma HLS PIPELINE II=2 // NOTEME. laxeable
		buffer_type nextoffset0 = allignlowerto4_KV(buffer[0][i].key + buffer[0][i].value);
		if(nextoffset0 > cutoff){
			nextoffset0 = cutoff; 
			limitsz[0][i] = cutoff - buffer[0][i].key;
		} else {
			limitsz[0][i] = allignlowerto4_KV(buffer[0][i].value);
		}
		if(i<size-1){ buffer[0][i+1].key = nextoffset0; }
		buffer_type nextoffset1 = allignlowerto4_KV(buffer[1][i].key + buffer[1][i].value);
		if(nextoffset1 > cutoff){
			nextoffset1 = cutoff; 
			limitsz[1][i] = cutoff - buffer[1][i].key;
		} else {
			limitsz[1][i] = allignlowerto4_KV(buffer[1][i].value);
		}
		if(i<size-1){ buffer[1][i+1].key = nextoffset1; }
		buffer_type nextoffset2 = allignlowerto4_KV(buffer[2][i].key + buffer[2][i].value);
		if(nextoffset2 > cutoff){
			nextoffset2 = cutoff; 
			limitsz[2][i] = cutoff - buffer[2][i].key;
		} else {
			limitsz[2][i] = allignlowerto4_KV(buffer[2][i].value);
		}
		if(i<size-1){ buffer[2][i+1].key = nextoffset2; }
		buffer_type nextoffset3 = allignlowerto4_KV(buffer[3][i].key + buffer[3][i].value);
		if(nextoffset3 > cutoff){
			nextoffset3 = cutoff; 
			limitsz[3][i] = cutoff - buffer[3][i].key;
		} else {
			limitsz[3][i] = allignlowerto4_KV(buffer[3][i].value);
		}
		if(i<size-1){ buffer[3][i+1].key = nextoffset3; }
		buffer_type nextoffset4 = allignlowerto4_KV(buffer[4][i].key + buffer[4][i].value);
		if(nextoffset4 > cutoff){
			nextoffset4 = cutoff; 
			limitsz[4][i] = cutoff - buffer[4][i].key;
		} else {
			limitsz[4][i] = allignlowerto4_KV(buffer[4][i].value);
		}
		if(i<size-1){ buffer[4][i+1].key = nextoffset4; }
		buffer_type nextoffset5 = allignlowerto4_KV(buffer[5][i].key + buffer[5][i].value);
		if(nextoffset5 > cutoff){
			nextoffset5 = cutoff; 
			limitsz[5][i] = cutoff - buffer[5][i].key;
		} else {
			limitsz[5][i] = allignlowerto4_KV(buffer[5][i].value);
		}
		if(i<size-1){ buffer[5][i+1].key = nextoffset5; }
		buffer_type nextoffset6 = allignlowerto4_KV(buffer[6][i].key + buffer[6][i].value);
		if(nextoffset6 > cutoff){
			nextoffset6 = cutoff; 
			limitsz[6][i] = cutoff - buffer[6][i].key;
		} else {
			limitsz[6][i] = allignlowerto4_KV(buffer[6][i].value);
		}
		if(i<size-1){ buffer[6][i+1].key = nextoffset6; }
		buffer_type nextoffset7 = allignlowerto4_KV(buffer[7][i].key + buffer[7][i].value);
		if(nextoffset7 > cutoff){
			nextoffset7 = cutoff; 
			limitsz[7][i] = cutoff - buffer[7][i].key;
		} else {
			limitsz[7][i] = allignlowerto4_KV(buffer[7][i].value);
		}
		if(i<size-1){ buffer[7][i+1].key = nextoffset7; }
	}
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif 
calculatemanyunallignedoffsets(skeyvalue_t buffer[VECTOR_SIZE][NUM_PARTITIONS], buffer_type size, batch_type base, batch_type skipspacing){
	for(buffer_type i=1; i<size; i++){ 
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
getglobalparams(uint512_dt * kvdram){
	globalparams_t globalparams;
	#ifdef _WIDEWORD
	globalparams.command = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_COMMANDID].range(31, 0);
	globalparams.runkernelcommand = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNKERNELCOMMANDID].range(31, 0);
	globalparams.processcommand = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_PROCESSCOMMANDID].range(31, 0);
	globalparams.collectstatscommand = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_COLLECTSTATSCOMMANDID].range(31, 0);
	globalparams.partitioncommand = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_PARTITIONCOMMANDID].range(31, 0);
	globalparams.reducecommand = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_APPLYUPDATESCOMMANDID].range(31, 0);
	globalparams.finalnumpartitions = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_FINALNUMPARTITIONSID].range(31, 0);
	globalparams.treedepthid = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_TREEDEPTHID].range(31, 0);
	globalparams.ssdpartitionid = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_SSDPARTITIONID].range(31, 0);
	globalparams.srcvoffset = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_SRCVOFFSET].range(31, 0); // not used
	globalparams.srcvsize = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_SRCVSIZE].range(31, 0);
	globalparams.srcvsize_kvs = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_SRCVSIZE_KVS].range(31, 0);
	globalparams.edgessize = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_EDGESSIZE].range(31, 0);
	globalparams.edgessize_kvs = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_EDGESSIZE_KVS].range(31, 0);
	globalparams.destvoffset = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_DESTVOFFSET].range(31, 0);
	globalparams.actvvsize = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_ACTVVSIZE].range(31, 0);
	globalparams.firstvid = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_FIRSTVID].range(31, 0);
	globalparams.firstkey = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_FIRSTKEY].range(31, 0);
	globalparams.firstvalue = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_FIRSTVALUE].range(31, 0); // not used
	globalparams.treedepth = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_TREEDEPTH].range(31, 0);
	globalparams.LLOPnumpartitions = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_FINALNUMPARTITIONS].range(31, 0);
	globalparams.batchsize = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BATCHSIZE].range(31, 0);
	globalparams.runsize = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].range(31, 0);
	globalparams.runsize_kvs = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE_KVS].range(31, 0);
	globalparams.nextbatchoffset = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NEXTBATCHOFFSET].range(31, 0);
	globalparams.GraphIter = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_GRAPHITERATIONID].range(31, 0);
	globalparams.GraphAlgo = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_GRAPHALGORITHMID].range(31, 0);
	globalparams.statsalreadycollected = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_STATSALREADYCOLLECTED].range(31, 0);
	globalparams.groupid = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_GROUPID].range(31, 0);
	globalparams.beginLOP = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BEGINLOP].range(31, 0);
	globalparams.endLOP = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_ENDLOP].range(31, 0);
	globalparams.numLOPs = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NUMLOPS].range(31, 0);
	globalparams.batch_range = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BATCHRANGE].range(31, 0);
	globalparams.batch_range_kvs = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BATCHRANGE_KVS].range(31, 0);
	globalparams.batch_range_pow = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BATCHRANGE_POW].range(31, 0);
	globalparams.applyvertexbuffersz = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_APPLYVERTEXBUFFERSZ].range(31, 0);
	globalparams.applyvertexbuffersz_kvs = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_APPLYVERTEXBUFFERSZ_KVS].range(31, 0);
	globalparams.baseoffset_messagesdram_kvs = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BASEOFFSET_MESSAGESDRAM_KVS].range(31, 0);
	globalparams.baseoffset_kvdrambuffer_kvs = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BASEOFFSET_KVDRAMBUFFER_KVS].range(31, 0);
	globalparams.baseoffset_kvdram_kvs = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BASEOFFSET_KVDRAM_KVS].range(31, 0);
	globalparams.baseoffset_kvdramworkspace_kvs = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BASEOFFSET_KVDRAMWORKSPACE_KVS].range(31, 0);
	globalparams.baseoffset_statsdram_kvs = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BASEOFFSET_STATSDRAM_KVS].range(31, 0);
	globalparams.baseoffset_activevertices_kvs = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BASEOFFSET_ACTIVEVERTICES_KVS].range(31, 0);
	globalparams.baseoffset_edgesdata_kvs = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BASEOFFSET_EDGESDATA_KVS].range(31, 0);
	globalparams.baseoffset_vertexptr_kvs = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BASEOFFSET_VERTEXPTR_KVS].range(31, 0);
	globalparams.baseoffset_verticesdata_kvs = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BASEOFFSET_VERTICESDATA_KVS].range(31, 0);
	#else 
	globalparams.command = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_COMMANDID].data[0].key;
	globalparams.runkernelcommand = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNKERNELCOMMANDID].data[0].key;
	globalparams.processcommand = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_PROCESSCOMMANDID].data[0].key;
	globalparams.collectstatscommand = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_COLLECTSTATSCOMMANDID].data[0].key;
	globalparams.partitioncommand = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_PARTITIONCOMMANDID].data[0].key;
	globalparams.reducecommand = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_APPLYUPDATESCOMMANDID].data[0].key;
	globalparams.finalnumpartitions = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_FINALNUMPARTITIONSID].data[0].key;
	globalparams.treedepthid = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_TREEDEPTHID].data[0].key;
	globalparams.ssdpartitionid = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_SSDPARTITIONID].data[0].key;
	globalparams.srcvoffset = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_SRCVOFFSET].data[0].key;
	globalparams.srcvsize = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_SRCVSIZE].data[0].key;
	globalparams.srcvsize_kvs = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_SRCVSIZE_KVS].data[0].key;
	globalparams.edgessize = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_EDGESSIZE].data[0].key;
	globalparams.edgessize_kvs = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_EDGESSIZE_KVS].data[0].key;
	globalparams.destvoffset = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_DESTVOFFSET].data[0].key;
	globalparams.actvvsize = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_ACTVVSIZE].data[0].key;
	globalparams.firstvid = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_FIRSTVID].data[0].key;
	globalparams.firstkey = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_FIRSTKEY].data[0].key;
	globalparams.firstvalue = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_FIRSTVALUE].data[0].key;
	globalparams.treedepth = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_TREEDEPTH].data[0].key;
	globalparams.LLOPnumpartitions = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_FINALNUMPARTITIONS].data[0].key;
	globalparams.batchsize = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BATCHSIZE].data[0].key;
	globalparams.runsize = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key;
	globalparams.runsize_kvs = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE_KVS].data[0].key;
	globalparams.nextbatchoffset = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NEXTBATCHOFFSET].data[0].key;
	globalparams.GraphIter = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_GRAPHITERATIONID].data[0].key;
	globalparams.GraphAlgo = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_GRAPHALGORITHMID].data[0].key;
	globalparams.statsalreadycollected = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_STATSALREADYCOLLECTED].data[0].key;
	globalparams.groupid = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_GROUPID].data[0].key;
	globalparams.beginLOP = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BEGINLOP].data[0].key;
	globalparams.endLOP = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_ENDLOP].data[0].key;
	globalparams.numLOPs = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NUMLOPS].data[0].key;
	globalparams.batch_range = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BATCHRANGE].data[0].key;
	globalparams.batch_range_kvs = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BATCHRANGE_KVS].data[0].key;
	globalparams.batch_range_pow = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BATCHRANGE_POW].data[0].key;
	globalparams.applyvertexbuffersz = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_APPLYVERTEXBUFFERSZ].data[0].key;
	globalparams.applyvertexbuffersz_kvs = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_APPLYVERTEXBUFFERSZ_KVS].data[0].key;
	globalparams.baseoffset_messagesdram_kvs = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BASEOFFSET_MESSAGESDRAM_KVS].data[0].key;
	globalparams.baseoffset_kvdrambuffer_kvs = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BASEOFFSET_KVDRAMBUFFER_KVS].data[0].key;
	globalparams.baseoffset_kvdram_kvs = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BASEOFFSET_KVDRAM_KVS].data[0].key;
	globalparams.baseoffset_kvdramworkspace_kvs = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BASEOFFSET_KVDRAMWORKSPACE_KVS].data[0].key;
	globalparams.baseoffset_statsdram_kvs = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BASEOFFSET_STATSDRAM_KVS].data[0].key;
	globalparams.baseoffset_activevertices_kvs = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BASEOFFSET_ACTIVEVERTICES_KVS].data[0].key;
	globalparams.baseoffset_edgesdata_kvs = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BASEOFFSET_EDGESDATA_KVS].data[0].key;
	globalparams.baseoffset_vertexptr_kvs = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BASEOFFSET_VERTEXPTR_KVS].data[0].key;
	globalparams.baseoffset_verticesdata_kvs = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BASEOFFSET_VERTICESDATA_KVS].data[0].key;
	#endif 
	// globalparams.baseaddr_destkvs_kvs = 0;
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

	#ifdef MERGEPROCESSEDGESANDPARTITIONSTAGE // NEWCHANGE. // FIXME. remove 'currentLOP == 0'
	if(currentLOP == 0 || (currentLOP % 2) == 1){ sweepparams.worksourcebaseaddress_kvs = globalparams.baseoffset_kvdram_kvs; sweepparams.workdestbaseaddress_kvs = globalparams.baseoffset_kvdramworkspace_kvs; }
	else { sweepparams.worksourcebaseaddress_kvs = globalparams.baseoffset_kvdramworkspace_kvs; sweepparams.workdestbaseaddress_kvs = globalparams.baseoffset_kvdram_kvs; }
	#else 
	if((currentLOP % 2) == 1){ sweepparams.worksourcebaseaddress_kvs = globalparams.baseoffset_kvdram_kvs; sweepparams.workdestbaseaddress_kvs = globalparams.baseoffset_kvdramworkspace_kvs; }
	else { sweepparams.worksourcebaseaddress_kvs = globalparams.baseoffset_kvdramworkspace_kvs; sweepparams.workdestbaseaddress_kvs = globalparams.baseoffset_kvdram_kvs; }
	#endif 

	sweepparams.upperlimit = globalparams.destvoffset + (source_partition * sourceskipsize * (globalparams.batch_range / globalparams.LLOPnumpartitions));
	
	sweepparams.source_partition = source_partition;
	return sweepparams;
}
travstate_t 
	#ifdef SW 
	actslw::
	#endif 
gettravstate(uint512_dt * kvdram, globalparams_t globalparams, step_type currentLOP, batch_type sourcestatsmarker){
	travstate_t travstate;
	keyvalue_t keyvalue;
	keyvalue_t nextkeyvalue;
	
	if(currentLOP == 0){ keyvalue.key = 0; }
	else if(currentLOP == 1){ keyvalue.key = 0; }
	#ifdef _WIDEWORD
	else { keyvalue.key = kvdram[globalparams.baseoffset_statsdram_kvs + sourcestatsmarker].range(31, 0); 
		   keyvalue.value = kvdram[globalparams.baseoffset_statsdram_kvs + sourcestatsmarker].range(63, 32); }
	#else 
	else { keyvalue = kvdram[globalparams.baseoffset_statsdram_kvs + sourcestatsmarker].data[0]; }
	#endif 
	
	if(currentLOP == 0){ nextkeyvalue.key = globalparams.runsize; }
	else if(currentLOP == 1){ nextkeyvalue.key = globalparams.runsize; }
	else { nextkeyvalue.key = keyvalue.key + keyvalue.value; }
		
	travstate.begin_kvs = keyvalue.key / VECTOR_SIZE; 
	travstate.end_kvs = (nextkeyvalue.key + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	
	travstate.size_kvs = travstate.end_kvs - travstate.begin_kvs;
	travstate.skip_kvs = SRCBUFFER_SIZE;
	travstate.i_kvs = travstate.begin_kvs;
	return travstate;	
}
bool 
	#ifdef SW 
	actslw::
	#endif
inprocessedgesstage(unsigned int currentLOP){
	#ifdef MERGEPROCESSEDGESANDPARTITIONSTAGE
	if(currentLOP == 1){ return true; } // NEWCHANGE.
	else { return false; }
	#else 
	if(currentLOP == 0){ return true; }
	else { return false; }
	#endif 
}
bool 
	#ifdef SW 
	actslw::
	#endif
incollectstatsstage(unsigned int currentLOP, globalparams_t globalparams){
	#ifdef MERGEPROCESSEDGESANDPARTITIONSTAGE
	if(currentLOP >= 2 && currentLOP <= globalparams.treedepth){ return true; }
	else { return false; }
	#else 
	if(currentLOP >= 1 && currentLOP <= globalparams.treedepth){ return true; }
	else { return false; }
	#endif 
}
bool 
	#ifdef SW 
	actslw::
	#endif
inpartitionstage(unsigned int currentLOP, globalparams_t globalparams){
	#ifdef MERGEPROCESSEDGESANDPARTITIONSTAGE
	if(currentLOP >= 2 && currentLOP <= globalparams.treedepth){ return true; }
	else { return false; }
	#else 
	if(currentLOP >= 1 && currentLOP <= globalparams.treedepth){ return true; }
	else { return false; }
	#endif 
}
bool 
	#ifdef SW 
	actslw::
	#endif
inreducestage(unsigned int currentLOP, globalparams_t globalparams){
	if(currentLOP == (globalparams.treedepth + 1)){ return true; }
	else { return false; }
}
void 
	#ifdef SW 
	actslw::
	#endif
calculateglobaloffsets(keyvalue_t * globalstatsbuffer, batch_type * skipsizes,  batch_type offset, batch_type size){
	#ifdef COLLECTSTATSOFFLINE // FIXME. GENERALIZEME.
	for(partition_type p=0; p<size; p++){ batch_type A = (globalstatsbuffer[p].value + (VECTOR_SIZE-1)) / VECTOR_SIZE; batch_type B = (A + (SRCBUFFER_SIZE-1)) / SRCBUFFER_SIZE; if(B < 80){ B = B * 2; } batch_type C = ((4 * 4 * 2) * NUM_PARTITIONS) + VECTOR_SIZE; skipsizes[p] = (B * C) + 128; }
	#else 
	for(partition_type p=0; p<size; p++){ batch_type A = (globalstatsbuffer[p].value + (VECTOR_SIZE-1)) / VECTOR_SIZE; batch_type B = (A + (SRCBUFFER_SIZE-1)) / SRCBUFFER_SIZE; if(B < 80){ B = B * 2; } batch_type C = ((4 * 4 * 2) * NUM_PARTITIONS) + VECTOR_SIZE; skipsizes[p] = (B * C) + 128; } 
	#endif 
	
	calculateoffsets(globalstatsbuffer, size, offset, skipsizes);
	return;
}

// collect globalstats functions 
void 
	#ifdef SW 
	actslw::
	#endif 
readglobalstats(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ], batch_type offset_kvs){ 
	if(enable == OFF){ return; }
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("readglobalstats", offset_kvs + NUM_PARTITIONS, BASEOFFSET_STATSDRAM_KVS + KVSTATSDRAMSZ_KVS + 1, NAp, NAp, NAp);
	#endif
	
	READGLOBALSTATS_LOOP: for (buffer_type i=0; i<NUM_PARTITIONS; i++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		globalstatsbuffer[i].key = kvdram[offset_kvs + i].range(31, 0);
		globalstatsbuffer[i].value = kvdram[offset_kvs + i].range(63, 32);
		#else 
		globalstatsbuffer[i] = kvdram[offset_kvs + i].data[0];
		#endif 
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printkeyvalues("readglobalstats.globalstatsbuffer", globalstatsbuffer, NUM_PARTITIONS); 
	#endif
	return;
}

void 
	#ifdef SW 
	actslw::
	#endif
collectglobalstats(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], step_type currentLOP, vertex_t upperlimit, travstate_t travstate, globalparams_t globalparams){					
	if(enable == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE;
	buffer_type chunk_size = getchunksize_kvs(SRCBUFFER_SIZE, travstate, 0);

	COLLECTGLOBALSTATS_LOOP: for(buffer_type i=0; i<chunk_size; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
	#ifdef MAXPERFORMANCE
	#pragma HLS PIPELINE II=2
	#else 
	#pragma HLS PIPELINE II=2 // 3	
	#endif 
	
		keyvalue_t keyvalue0 = sourcebuffer[0][i];
		keyvalue_t keyvalue1 = sourcebuffer[1][i];
		keyvalue_t keyvalue2 = sourcebuffer[2][i];
		keyvalue_t keyvalue3 = sourcebuffer[3][i];
		keyvalue_t keyvalue4 = sourcebuffer[4][i];
		keyvalue_t keyvalue5 = sourcebuffer[5][i];
		keyvalue_t keyvalue6 = sourcebuffer[6][i];
		keyvalue_t keyvalue7 = sourcebuffer[7][i];
		
		partition_type p0 = 0;
		if(keyvalue0.key != INVALIDDATA){ p0 = getpartition(ON, keyvalue0, currentLOP, upperlimit, globalparams.batch_range_pow); }
		partition_type p1 = 0;
		if(keyvalue1.key != INVALIDDATA){ p1 = getpartition(ON, keyvalue1, currentLOP, upperlimit, globalparams.batch_range_pow); }
		partition_type p2 = 0;
		if(keyvalue2.key != INVALIDDATA){ p2 = getpartition(ON, keyvalue2, currentLOP, upperlimit, globalparams.batch_range_pow); }
		partition_type p3 = 0;
		if(keyvalue3.key != INVALIDDATA){ p3 = getpartition(ON, keyvalue3, currentLOP, upperlimit, globalparams.batch_range_pow); }
		partition_type p4 = 0;
		if(keyvalue4.key != INVALIDDATA){ p4 = getpartition(ON, keyvalue4, currentLOP, upperlimit, globalparams.batch_range_pow); }
		partition_type p5 = 0;
		if(keyvalue5.key != INVALIDDATA){ p5 = getpartition(ON, keyvalue5, currentLOP, upperlimit, globalparams.batch_range_pow); }
		partition_type p6 = 0;
		if(keyvalue6.key != INVALIDDATA){ p6 = getpartition(ON, keyvalue6, currentLOP, upperlimit, globalparams.batch_range_pow); }
		partition_type p7 = 0;
		if(keyvalue7.key != INVALIDDATA){ p7 = getpartition(ON, keyvalue7, currentLOP, upperlimit, globalparams.batch_range_pow); }
		
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats. out of bounds for destbuffer[0]", p0, NUM_PARTITIONS, keyvalue0.key, globalparams.batch_range, NAp);
		#endif 
		if(keyvalue0.key != INVALIDDATA){ destbuffer[0][p0].value += 1; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats. out of bounds for destbuffer[1]", p1, NUM_PARTITIONS, keyvalue1.key, globalparams.batch_range, NAp);
		#endif 
		if(keyvalue1.key != INVALIDDATA){ destbuffer[1][p1].value += 1; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats. out of bounds for destbuffer[2]", p2, NUM_PARTITIONS, keyvalue2.key, globalparams.batch_range, NAp);
		#endif 
		if(keyvalue2.key != INVALIDDATA){ destbuffer[2][p2].value += 1; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats. out of bounds for destbuffer[3]", p3, NUM_PARTITIONS, keyvalue3.key, globalparams.batch_range, NAp);
		#endif 
		if(keyvalue3.key != INVALIDDATA){ destbuffer[3][p3].value += 1; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats. out of bounds for destbuffer[4]", p4, NUM_PARTITIONS, keyvalue4.key, globalparams.batch_range, NAp);
		#endif 
		if(keyvalue4.key != INVALIDDATA){ destbuffer[4][p4].value += 1; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats. out of bounds for destbuffer[5]", p5, NUM_PARTITIONS, keyvalue5.key, globalparams.batch_range, NAp);
		#endif 
		if(keyvalue5.key != INVALIDDATA){ destbuffer[5][p5].value += 1; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats. out of bounds for destbuffer[6]", p6, NUM_PARTITIONS, keyvalue6.key, globalparams.batch_range, NAp);
		#endif 
		if(keyvalue6.key != INVALIDDATA){ destbuffer[6][p6].value += 1; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats. out of bounds for destbuffer[7]", p7, NUM_PARTITIONS, keyvalue7.key, globalparams.batch_range, NAp);
		#endif 
		if(keyvalue7.key != INVALIDDATA){ destbuffer[7][p7].value += 1; }
	}
	return;
}

void 
	#ifdef SW 
	actslw::
	#endif
fastcollectglobalstats(bool_type enable, keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ], step_type currentLOP, batch_type source_partition){					
	if(enable == OFF){ return; }
	
	if(currentLOP == 1){ // FIXME. OPTIMIZEME. make simpler
		batch_type per = (1 << (NUM_PARTITIONS_POW * 2)) / NUM_PARTITIONS; // NEWCHANGE.
		for(partition_type p=0; p<NUM_PARTITIONS; p++){
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("fastcollectglobalstats 1", NUM_PARTITIONS + (p*per) + (per-1), NUM_PARTITIONS + (1 << (NUM_PARTITIONS_POW * TREE_DEPTH)), p, per, NUM_PARTITIONS);
			#endif
			for(batch_type j=0; j<per; j++){ globalstatsbuffer[p].value += globalstatsbuffer[NUM_PARTITIONS + (p*per) + j].value; }
		}
	} else if(currentLOP == 2){
		batch_type per = 1;
		for(partition_type p=0; p<NUM_PARTITIONS; p++){
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("fastcollectglobalstats 2", NUM_PARTITIONS + (source_partition*NUM_PARTITIONS) + (p*per) + (per-1), NUM_PARTITIONS + (1 << (NUM_PARTITIONS_POW * TREE_DEPTH)), p, per, NUM_PARTITIONS);
			#endif
			for(batch_type j=0; j<per; j++){ globalstatsbuffer[p].value += globalstatsbuffer[NUM_PARTITIONS + (source_partition*NUM_PARTITIONS) + (p*per) + j].value; }
		}
	} else {
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"ERROR. NO SUPPORT FOR CLOP > 2. EXITING..."<<endl;
		exit(EXIT_FAILURE);
		#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printkeyvalues("fastcollectglobalstats.globalstatsbuffer", globalstatsbuffer, NUM_PARTITIONS); 
	#endif
	return;
}

void 
	#ifdef SW 
	actslw::
	#endif
prepareglobalstats(bool_type enable, keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ], globalparams_t globalparams){
	if(enable == OFF){ return; }
	
	PREPAREGLOBALSTATS_LOOP1: for(vector_type v=0; v<VECTOR_SIZE; v++){
		PREPAREGLOBALSTATS_LOOP1B: for(buffer_type i=0; i<NUM_PARTITIONS; i++){
			globalstatsbuffer[i].value += buffer[v][i].value;
		}
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printkeyvalues("prepareglobalstats", globalstatsbuffer, NUM_PARTITIONS); 
	#endif
	return;
}

void 
	#ifdef SW 
	actslw::
	#endif
prepareglobalstats2(bool_type enable, keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ], batch_type offset, globalparams_t globalparams){
	if(enable == OFF){ return; }

	PREPAREGLOBALSTATS_LOOP1: for(vector_type v=0; v<VECTOR_SIZE; v++){
		PREPAREGLOBALSTATS_LOOP1B: for(buffer_type i=0; i<GLOBALSTATSBUFFERSZ; i++){
			globalstatsbuffer[offset + i].value += buffer[v][i].value;
		}
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printkeyvalues("prepareglobalstats2.globalstatsbuffer", globalstatsbuffer, NUM_PARTITIONS+256); 
	#endif
	return;
}

void 
	#ifdef SW 
	actslw::
	#endif 
saveglobalstats(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ], batch_type offset_kvs){ 
	if(enable == OFF){ return; }
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("saveglobalstats", offset_kvs + NUM_PARTITIONS, BASEOFFSET_STATSDRAM_KVS + KVSTATSDRAMSZ_KVS + 1, offset_kvs, NUM_PARTITIONS, KVSTATSDRAMSZ_KVS);
	#endif
	
	SAVEGLOBALSTATS_LOOP: for (buffer_type i=0; i<NUM_PARTITIONS; i++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		kvdram[offset_kvs + i].range(31, 0) = globalstatsbuffer[i].key;
		kvdram[offset_kvs + i].range(63, 32) = globalstatsbuffer[i].value;
		#else 
		kvdram[offset_kvs + i].data[0] = globalstatsbuffer[i];
		#endif 
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalvar_savestats_counttotalstatswritten(VECTOR_SIZE);
		#endif
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printkeyvalues("saveglobalstats.globalstatsbuffer", globalstatsbuffer, NUM_PARTITIONS); 
	#endif
	// exit(EXIT_SUCCESS);
	return;
}

// partition functions
void 
	#ifdef SW 
	actslw::
	#endif 
readkeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], batch_type offset_kvs, batch_type size_kvs, travstate_t travstate){
	if(enable == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE;
	buffer_type chunk_size = getchunksize_kvs(size_kvs, travstate, 0);
	
	READKVS_LOOP: for (buffer_type i=0; i<chunk_size; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		buffer[0][i].key = kvdram[offset_kvs + i].range(31, 0);
		buffer[0][i].value = kvdram[offset_kvs + i].range(63, 32);
		buffer[1][i].key = kvdram[offset_kvs + i].range(95, 64);
		buffer[1][i].value = kvdram[offset_kvs + i].range(127, 96);
		buffer[2][i].key = kvdram[offset_kvs + i].range(159, 128);
		buffer[2][i].value = kvdram[offset_kvs + i].range(191, 160);
		buffer[3][i].key = kvdram[offset_kvs + i].range(223, 192);
		buffer[3][i].value = kvdram[offset_kvs + i].range(255, 224);
		buffer[4][i].key = kvdram[offset_kvs + i].range(287, 256);
		buffer[4][i].value = kvdram[offset_kvs + i].range(319, 288);
		buffer[5][i].key = kvdram[offset_kvs + i].range(351, 320);
		buffer[5][i].value = kvdram[offset_kvs + i].range(383, 352);
		buffer[6][i].key = kvdram[offset_kvs + i].range(415, 384);
		buffer[6][i].value = kvdram[offset_kvs + i].range(447, 416);
		buffer[7][i].key = kvdram[offset_kvs + i].range(479, 448);
		buffer[7][i].value = kvdram[offset_kvs + i].range(511, 480);
		#else 
		buffer[0][i] = kvdram[offset_kvs + i].data[0]; 
		buffer[1][i] = kvdram[offset_kvs + i].data[1]; 
		buffer[2][i] = kvdram[offset_kvs + i].data[2]; 
		buffer[3][i] = kvdram[offset_kvs + i].data[3]; 
		buffer[4][i] = kvdram[offset_kvs + i].data[4]; 
		buffer[5][i] = kvdram[offset_kvs + i].data[5]; 
		buffer[6][i] = kvdram[offset_kvs + i].data[6]; 
		buffer[7][i] = kvdram[offset_kvs + i].data[7]; 
		#endif 
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"readkeyvalues:: keyvalues read: offset_kvs: "<<offset_kvs<<", size_kvs: "<<size_kvs<<", chunk_size: "<<chunk_size<<endl;
	#endif
	return;
}

void 
	#ifdef SW 
	actslw::
	#endif
partitionkeyvalues_coarsegrainedpipeline(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t localcapsule[VECTOR_SIZE][NUM_PARTITIONS], step_type currentLOP, vertex_t upperlimit, travstate_t travstate, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE;
	buffer_type chunk_size = getchunksize_kvs(SRCBUFFER_SIZE, travstate, 0);

	resetmanykeyandvalues(localcapsule, NUM_PARTITIONS, 0);
	
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
			if(keyvalue0.key != INVALIDDATA){ p0 = getpartition(ON, keyvalue0, currentLOP, upperlimit, globalparams.batch_range_pow); } 
			partition_type p1 = 0;
			if(keyvalue1.key != INVALIDDATA){ p1 = getpartition(ON, keyvalue1, currentLOP, upperlimit, globalparams.batch_range_pow); } 
			partition_type p2 = 0;
			if(keyvalue2.key != INVALIDDATA){ p2 = getpartition(ON, keyvalue2, currentLOP, upperlimit, globalparams.batch_range_pow); } 
			partition_type p3 = 0;
			if(keyvalue3.key != INVALIDDATA){ p3 = getpartition(ON, keyvalue3, currentLOP, upperlimit, globalparams.batch_range_pow); } 
			partition_type p4 = 0;
			if(keyvalue4.key != INVALIDDATA){ p4 = getpartition(ON, keyvalue4, currentLOP, upperlimit, globalparams.batch_range_pow); } 
			partition_type p5 = 0;
			if(keyvalue5.key != INVALIDDATA){ p5 = getpartition(ON, keyvalue5, currentLOP, upperlimit, globalparams.batch_range_pow); } 
			partition_type p6 = 0;
			if(keyvalue6.key != INVALIDDATA){ p6 = getpartition(ON, keyvalue6, currentLOP, upperlimit, globalparams.batch_range_pow); } 
			partition_type p7 = 0;
			if(keyvalue7.key != INVALIDDATA){ p7 = getpartition(ON, keyvalue7, currentLOP, upperlimit, globalparams.batch_range_pow); } 
			
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("partitionkeyvalues_coarsegrainedpipeline", localcapsule[0][p0].key + localcapsule[0][p0].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues_coarsegrainedpipeline", localcapsule[1][p1].key + localcapsule[1][p1].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues_coarsegrainedpipeline", localcapsule[2][p2].key + localcapsule[2][p2].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues_coarsegrainedpipeline", localcapsule[3][p3].key + localcapsule[3][p3].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues_coarsegrainedpipeline", localcapsule[4][p4].key + localcapsule[4][p4].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues_coarsegrainedpipeline", localcapsule[5][p5].key + localcapsule[5][p5].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues_coarsegrainedpipeline", localcapsule[6][p6].key + localcapsule[6][p6].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues_coarsegrainedpipeline", localcapsule[7][p7].key + localcapsule[7][p7].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
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
			actsutilityobj->checkoutofbounds("partitionkeyvalues_coarsegrainedpipeline", p0, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues_coarsegrainedpipeline", p1, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues_coarsegrainedpipeline", p2, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues_coarsegrainedpipeline", p3, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues_coarsegrainedpipeline", p4, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues_coarsegrainedpipeline", p5, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues_coarsegrainedpipeline", p6, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues_coarsegrainedpipeline", p7, NUM_PARTITIONS, NAp, NAp, NAp);
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
		if(c==0){ resetmanyvalues(localcapsule, NUM_PARTITIONS, 0); }
	}
	
	#ifdef _DEBUGMODE_STATS
	for(vector_type v=0; v<VECTOR_SIZE; v++){ actsutilityobj->globalvar_inmemory_counttotalvalidkeyvalues(actsutilityobj->ugetvaluecount((keyvalue_t *)localcapsule[v], NUM_PARTITIONS)); } // REMOVEME. unmatched data types
	#endif
	return;
}

buffer_type 
	#ifdef SW 
	actslw::
	#endif
fastpartitionkeyvalues(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t localcapsule[VECTOR_SIZE][NUM_PARTITIONS], step_type currentLOP, vertex_t upperlimit, travstate_t travstate, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return 0; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE;
	analysis_type analysis_edgecond1 = 32;
	
	buffer_type limitsz[VECTOR_SIZE][NUM_PARTITIONS];
	#pragma HLS array_partition variable = limitsz
	value_t ovsz[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=ovsz complete
	resetvalues(ovsz, VECTOR_SIZE, 0);
	value_t sumvalues[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=sumvalues complete
	resetvalues(sumvalues, VECTOR_SIZE, 0);
	
	buffer_type cutoff = 0;
	buffer_type chunk_size = getchunksize_kvs(SRCBUFFER_SIZE, travstate, 0);
	resetmanykeyandvalues(localcapsule, NUM_PARTITIONS, 0);
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int v=0; v<1; v++){ actsutilityobj->printkeyvalues("fastpartitionkeyvalues.destbuffer[0]", destbuffer[v], 32); }
	for(unsigned int v=0; v<1; v++){ actsutilityobj->printkeyvalues("fastpartitionkeyvalues.localcapsule[0]", (keyvalue_t *)localcapsule[v], NUM_PARTITIONS); }
	#endif
	
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
			if(keyvalue0.value != INVALIDDATA){ p0 = getpartition(ON, keyvalue0, currentLOP, upperlimit, globalparams.batch_range_pow); } // NEWCHANGE.
			partition_type p1 = 0;
			if(keyvalue1.value != INVALIDDATA){ p1 = getpartition(ON, keyvalue1, currentLOP, upperlimit, globalparams.batch_range_pow); } // NEWCHANGE.
			partition_type p2 = 0;
			if(keyvalue2.value != INVALIDDATA){ p2 = getpartition(ON, keyvalue2, currentLOP, upperlimit, globalparams.batch_range_pow); } // NEWCHANGE.
			partition_type p3 = 0;
			if(keyvalue3.value != INVALIDDATA){ p3 = getpartition(ON, keyvalue3, currentLOP, upperlimit, globalparams.batch_range_pow); } // NEWCHANGE.
			partition_type p4 = 0;
			if(keyvalue4.value != INVALIDDATA){ p4 = getpartition(ON, keyvalue4, currentLOP, upperlimit, globalparams.batch_range_pow); } // NEWCHANGE.
			partition_type p5 = 0;
			if(keyvalue5.value != INVALIDDATA){ p5 = getpartition(ON, keyvalue5, currentLOP, upperlimit, globalparams.batch_range_pow); } // NEWCHANGE.
			partition_type p6 = 0;
			if(keyvalue6.value != INVALIDDATA){ p6 = getpartition(ON, keyvalue6, currentLOP, upperlimit, globalparams.batch_range_pow); } // NEWCHANGE.
			partition_type p7 = 0;
			if(keyvalue7.value != INVALIDDATA){ p7 = getpartition(ON, keyvalue7, currentLOP, upperlimit, globalparams.batch_range_pow); } // NEWCHANGE.
			
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("fastpartitionkeyvalues", localcapsule[0][p0].key + localcapsule[0][p0].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("fastpartitionkeyvalues", localcapsule[1][p1].key + localcapsule[1][p1].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("fastpartitionkeyvalues", localcapsule[2][p2].key + localcapsule[2][p2].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("fastpartitionkeyvalues", localcapsule[3][p3].key + localcapsule[3][p3].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("fastpartitionkeyvalues", localcapsule[4][p4].key + localcapsule[4][p4].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("fastpartitionkeyvalues", localcapsule[5][p5].key + localcapsule[5][p5].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("fastpartitionkeyvalues", localcapsule[6][p6].key + localcapsule[6][p6].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("fastpartitionkeyvalues", localcapsule[7][p7].key + localcapsule[7][p7].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			#endif
			
 
			buffer_type loc0 = 0;
			if(keyvalue0.value != INVALIDDATA){ // NEWCHANGE.
				if(c==0){ localcapsule[0][p0].value += 1; }
				else{
					if(localcapsule[0][p0].value < limitsz[0][p0]){
						loc0 = localcapsule[0][p0].key + localcapsule[0][p0].value; 
						localcapsule[0][p0].value += 1; 
					} else {
						loc0 = cutoff + ovsz[0];
						ovsz[0] += 1;
					}
					#ifdef _DEBUGMODE_CHECKS2
					actsutilityobj->checkoutofbounds("fastpartitionkeyvalues.loc0", loc0, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
					#endif 
				}
			}
 
			buffer_type loc1 = 0;
			if(keyvalue1.value != INVALIDDATA){ // NEWCHANGE.
				if(c==0){ localcapsule[1][p1].value += 1; }
				else{
					if(localcapsule[1][p1].value < limitsz[1][p1]){
						loc1 = localcapsule[1][p1].key + localcapsule[1][p1].value; 
						localcapsule[1][p1].value += 1; 
					} else {
						loc1 = cutoff + ovsz[1];
						ovsz[1] += 1;
					}
					#ifdef _DEBUGMODE_CHECKS2
					actsutilityobj->checkoutofbounds("fastpartitionkeyvalues.loc1", loc1, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
					#endif 
				}
			}
 
			buffer_type loc2 = 0;
			if(keyvalue2.value != INVALIDDATA){ // NEWCHANGE.
				if(c==0){ localcapsule[2][p2].value += 1; }
				else{
					if(localcapsule[2][p2].value < limitsz[2][p2]){
						loc2 = localcapsule[2][p2].key + localcapsule[2][p2].value; 
						localcapsule[2][p2].value += 1; 
					} else {
						loc2 = cutoff + ovsz[2];
						ovsz[2] += 1;
					}
					#ifdef _DEBUGMODE_CHECKS2
					actsutilityobj->checkoutofbounds("fastpartitionkeyvalues.loc2", loc2, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
					#endif 
				}
			}
 
			buffer_type loc3 = 0;
			if(keyvalue3.value != INVALIDDATA){ // NEWCHANGE.
				if(c==0){ localcapsule[3][p3].value += 1; }
				else{
					if(localcapsule[3][p3].value < limitsz[3][p3]){
						loc3 = localcapsule[3][p3].key + localcapsule[3][p3].value; 
						localcapsule[3][p3].value += 1; 
					} else {
						loc3 = cutoff + ovsz[3];
						ovsz[3] += 1;
					}
					#ifdef _DEBUGMODE_CHECKS2
					actsutilityobj->checkoutofbounds("fastpartitionkeyvalues.loc3", loc3, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
					#endif 
				}
			}
 
			buffer_type loc4 = 0;
			if(keyvalue4.value != INVALIDDATA){ // NEWCHANGE.
				if(c==0){ localcapsule[4][p4].value += 1; }
				else{
					if(localcapsule[4][p4].value < limitsz[4][p4]){
						loc4 = localcapsule[4][p4].key + localcapsule[4][p4].value; 
						localcapsule[4][p4].value += 1; 
					} else {
						loc4 = cutoff + ovsz[4];
						ovsz[4] += 1;
					}
					#ifdef _DEBUGMODE_CHECKS2
					actsutilityobj->checkoutofbounds("fastpartitionkeyvalues.loc4", loc4, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
					#endif 
				}
			}
 
			buffer_type loc5 = 0;
			if(keyvalue5.value != INVALIDDATA){ // NEWCHANGE.
				if(c==0){ localcapsule[5][p5].value += 1; }
				else{
					if(localcapsule[5][p5].value < limitsz[5][p5]){
						loc5 = localcapsule[5][p5].key + localcapsule[5][p5].value; 
						localcapsule[5][p5].value += 1; 
					} else {
						loc5 = cutoff + ovsz[5];
						ovsz[5] += 1;
					}
					#ifdef _DEBUGMODE_CHECKS2
					actsutilityobj->checkoutofbounds("fastpartitionkeyvalues.loc5", loc5, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
					#endif 
				}
			}
 
			buffer_type loc6 = 0;
			if(keyvalue6.value != INVALIDDATA){ // NEWCHANGE.
				if(c==0){ localcapsule[6][p6].value += 1; }
				else{
					if(localcapsule[6][p6].value < limitsz[6][p6]){
						loc6 = localcapsule[6][p6].key + localcapsule[6][p6].value; 
						localcapsule[6][p6].value += 1; 
					} else {
						loc6 = cutoff + ovsz[6];
						ovsz[6] += 1;
					}
					#ifdef _DEBUGMODE_CHECKS2
					actsutilityobj->checkoutofbounds("fastpartitionkeyvalues.loc6", loc6, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
					#endif 
				}
			}
 
			buffer_type loc7 = 0;
			if(keyvalue7.value != INVALIDDATA){ // NEWCHANGE.
				if(c==0){ localcapsule[7][p7].value += 1; }
				else{
					if(localcapsule[7][p7].value < limitsz[7][p7]){
						loc7 = localcapsule[7][p7].key + localcapsule[7][p7].value; 
						localcapsule[7][p7].value += 1; 
					} else {
						loc7 = cutoff + ovsz[7];
						ovsz[7] += 1;
					}
					#ifdef _DEBUGMODE_CHECKS2
					actsutilityobj->checkoutofbounds("fastpartitionkeyvalues.loc7", loc7, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
					#endif 
				}
			}
			
			if(keyvalue0.value != INVALIDDATA && c == 1){ destbuffer[0][loc0] = keyvalue0; } // NEWCHANGE.
			if(keyvalue1.value != INVALIDDATA && c == 1){ destbuffer[1][loc1] = keyvalue1; } // NEWCHANGE.
			if(keyvalue2.value != INVALIDDATA && c == 1){ destbuffer[2][loc2] = keyvalue2; } // NEWCHANGE.
			if(keyvalue3.value != INVALIDDATA && c == 1){ destbuffer[3][loc3] = keyvalue3; } // NEWCHANGE.
			if(keyvalue4.value != INVALIDDATA && c == 1){ destbuffer[4][loc4] = keyvalue4; } // NEWCHANGE.
			if(keyvalue5.value != INVALIDDATA && c == 1){ destbuffer[5][loc5] = keyvalue5; } // NEWCHANGE.
			if(keyvalue6.value != INVALIDDATA && c == 1){ destbuffer[6][loc6] = keyvalue6; } // NEWCHANGE.
			if(keyvalue7.value != INVALIDDATA && c == 1){ destbuffer[7][loc7] = keyvalue7; } // NEWCHANGE.
			
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("fastpartitionkeyvalues.p0", p0, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("fastpartitionkeyvalues.p1", p1, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("fastpartitionkeyvalues.p2", p2, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("fastpartitionkeyvalues.p3", p3, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("fastpartitionkeyvalues.p4", p4, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("fastpartitionkeyvalues.p5", p5, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("fastpartitionkeyvalues.p6", p6, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("fastpartitionkeyvalues.p7", p7, NUM_PARTITIONS, NAp, NAp, NAp);
			#endif
		}
		
		if(c==0){
			for(partition_type p=0; p<NUM_PARTITIONS; p++){
				sumvalues[0] += allignlowerto4_KV(localcapsule[0][p].value);
				sumvalues[1] += allignlowerto4_KV(localcapsule[1][p].value);
				sumvalues[2] += allignlowerto4_KV(localcapsule[2][p].value);
				sumvalues[3] += allignlowerto4_KV(localcapsule[3][p].value);
				sumvalues[4] += allignlowerto4_KV(localcapsule[4][p].value);
				sumvalues[5] += allignlowerto4_KV(localcapsule[5][p].value);
				sumvalues[6] += allignlowerto4_KV(localcapsule[6][p].value);
				sumvalues[7] += allignlowerto4_KV(localcapsule[7][p].value);
			}
			cutoff = INFINITI;
			for(vector_type v=0; v<VECTOR_SIZE; v++){ if(sumvalues[v] < cutoff){ cutoff = sumvalues[v]; }}
			calculatemanyoffsets_allignby4_lower(localcapsule, limitsz, NUM_PARTITIONS, cutoff);
			resetmanyvalues(localcapsule, NUM_PARTITIONS, 0);
		}
	}
	for(buffer_type i=cutoff; i<SRCBUFFER_SIZE; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=16 avg=16	
	#pragma HLS PIPELINE II=1
		sourcebuffer[0][i] = destbuffer[0][i];
		sourcebuffer[1][i] = destbuffer[1][i];
		sourcebuffer[2][i] = destbuffer[2][i];
		sourcebuffer[3][i] = destbuffer[3][i];
		sourcebuffer[4][i] = destbuffer[4][i];
		sourcebuffer[5][i] = destbuffer[5][i];
		sourcebuffer[6][i] = destbuffer[6][i];
		sourcebuffer[7][i] = destbuffer[7][i];
	}
	
	#ifdef _DEBUGMODE_STATS
	for(vector_type v=0; v<VECTOR_SIZE; v++){ actsutilityobj->globalvar_inmemory_counttotalvalidkeyvalues(actsutilityobj->ugetvaluecount((keyvalue_t *)localcapsule[v], NUM_PARTITIONS)); } // REMOVEME. unmatched data types
	#endif
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("fastpartitionkeyvalues:: cutoff", cutoff, SRCBUFFER_SIZE+1, NAp, NAp, NAp);
	// for(unsigned int v=0; v<VECTOR_SIZE; v++){ if(ovsz[v] != ovsz[0]){ cout<<"ovsz["<<v<<"] != ovsz[0]. cutoff: "<<cutoff<<". printint values and exiting..."<<endl; actsutilityobj->printvalues("fastpartitionkeyvalues.ovsz", ovsz, VECTOR_SIZE); exit(EXIT_FAILURE); }} // FIXME.
	// for(unsigned int v=0; v<VECTOR_SIZE; v++){ actsutilityobj->checkforequal("fastpartitionkeyvalues:: ovsz["+std::to_string(v)+"].ovsz[0]", ovsz[v], ovsz[0], v, ovsz[0], NAp); }
	// actsutilityobj->checkforequal("fastpartitionkeyvalues.cutoff+ovsz[0],chunk_size", cutoff+ovsz[0], chunk_size, cutoff, ovsz[0], NAp); // REMOVEME.
	actsutilityobj->updatemincutoffseen(cutoff);
	actsutilityobj->updatemaxcutoffseen(cutoff);
	#endif
	#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_RUNKERNELPRINTS)
	cout<<"--------------- cutoff: "<<cutoff<<", ovsz[0]: "<<ovsz[0]<<", (cutoff+ovsz[0]): "<<cutoff+ovsz[0]<<", chunk_size: "<<chunk_size<<"  -----------------------------"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS
	unsigned int dummy[NUM_PARTITIONS];
	if(false){ actsutilityobj->printprofile("fastpartitionkeyvalues.destbuffer", (keyvalue_t *)&destbuffer[0], PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, currentLOP, upperlimit, globalparams.batch_range_pow); }
	if(false){ actsutilityobj->printprofileso1("fastpartitionkeyvalues.destbuffer", destbuffer, localcapsule, currentLOP, upperlimit, globalparams.batch_range_pow, dummy); }
	for(unsigned int v=0; v<8; v++){ actsutilityobj->printkeyvalues("fastpartitionkeyvalues.localcapsule[v]", (keyvalue_t *)localcapsule[v], NUM_PARTITIONS); }
	for(unsigned int v=0; v<8; v++){ actsutilityobj->printvalues("fastpartitionkeyvalues.limitsz[v]", (value_t *)limitsz[v], NUM_PARTITIONS); }
	#endif
	return cutoff;
}

buffer_type 
	#ifdef SW 
	actslw::
	#endif
maxpartitionkeyvalues(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t localcapsule[VECTOR_SIZE][NUM_PARTITIONS], step_type currentLOP, vertex_t upperlimit, travstate_t travstate, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return 0; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE;
	analysis_type analysis_edgecond1 = 32;
	
	buffer_type templocalcapsule0[NUM_PARTITIONS];
	#pragma HLS array_partition variable = templocalcapsule0 complete
	buffer_type templocalcapsule1[NUM_PARTITIONS];
	#pragma HLS array_partition variable = templocalcapsule1 complete
	buffer_type templocalcapsule2[NUM_PARTITIONS];
	#pragma HLS array_partition variable = templocalcapsule2 complete
	buffer_type templocalcapsule3[NUM_PARTITIONS];
	#pragma HLS array_partition variable = templocalcapsule3 complete
	buffer_type templocalcapsule4[NUM_PARTITIONS];
	#pragma HLS array_partition variable = templocalcapsule4 complete
	buffer_type templocalcapsule5[NUM_PARTITIONS];
	#pragma HLS array_partition variable = templocalcapsule5 complete
	buffer_type templocalcapsule6[NUM_PARTITIONS];
	#pragma HLS array_partition variable = templocalcapsule6 complete
	buffer_type templocalcapsule7[NUM_PARTITIONS];
	#pragma HLS array_partition variable = templocalcapsule7 complete
	
	buffer_type limitsz[VECTOR_SIZE][NUM_PARTITIONS];
	#pragma HLS array_partition variable = limitsz
	value_t ovsz[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=ovsz complete
	resetvalues(ovsz, VECTOR_SIZE, 0);
	value_t sumvalues[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=sumvalues complete
	resetvalues(sumvalues, VECTOR_SIZE, 0);
	
	buffer_type cutoff = 0;
	buffer_type chunk_size = getchunksize_kvs(SRCBUFFER_SIZE, travstate, 0);
	
	for(buffer_type i=0; i<NUM_PARTITIONS; i++){
		templocalcapsule0[i] = 0; 
		localcapsule[0][i].key = 0;
		templocalcapsule1[i] = 0; 
		localcapsule[1][i].key = 0;
		templocalcapsule2[i] = 0; 
		localcapsule[2][i].key = 0;
		templocalcapsule3[i] = 0; 
		localcapsule[3][i].key = 0;
		templocalcapsule4[i] = 0; 
		localcapsule[4][i].key = 0;
		templocalcapsule5[i] = 0; 
		localcapsule[5][i].key = 0;
		templocalcapsule6[i] = 0; 
		localcapsule[6][i].key = 0;
		templocalcapsule7[i] = 0; 
		localcapsule[7][i].key = 0;
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int v=0; v<1; v++){ actsutilityobj->printkeyvalues("fastpartitionkeyvalues.destbuffer[0]", destbuffer[v], 32); }
	for(unsigned int v=0; v<1; v++){ actsutilityobj->printkeyvalues("fastpartitionkeyvalues.localcapsule[0]", (keyvalue_t *)localcapsule[v], NUM_PARTITIONS); }
	#endif 
	
	PARTITIONKEYVALUES_LOOP1: for(step_type c=0; c<2; c++){
		PARTITIONKEYVALUES_LOOP1B: for(buffer_type i=0; i<chunk_size; i++){
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
			
			partition_type p0 = 0;
			if(keyvalue0.key != INVALIDDATA){ p0 = getpartition(ON, keyvalue0, currentLOP, upperlimit, globalparams.batch_range_pow); } 
			partition_type p1 = 0;
			if(keyvalue1.key != INVALIDDATA){ p1 = getpartition(ON, keyvalue1, currentLOP, upperlimit, globalparams.batch_range_pow); } 
			partition_type p2 = 0;
			if(keyvalue2.key != INVALIDDATA){ p2 = getpartition(ON, keyvalue2, currentLOP, upperlimit, globalparams.batch_range_pow); } 
			partition_type p3 = 0;
			if(keyvalue3.key != INVALIDDATA){ p3 = getpartition(ON, keyvalue3, currentLOP, upperlimit, globalparams.batch_range_pow); } 
			partition_type p4 = 0;
			if(keyvalue4.key != INVALIDDATA){ p4 = getpartition(ON, keyvalue4, currentLOP, upperlimit, globalparams.batch_range_pow); } 
			partition_type p5 = 0;
			if(keyvalue5.key != INVALIDDATA){ p5 = getpartition(ON, keyvalue5, currentLOP, upperlimit, globalparams.batch_range_pow); } 
			partition_type p6 = 0;
			if(keyvalue6.key != INVALIDDATA){ p6 = getpartition(ON, keyvalue6, currentLOP, upperlimit, globalparams.batch_range_pow); } 
			partition_type p7 = 0;
			if(keyvalue7.key != INVALIDDATA){ p7 = getpartition(ON, keyvalue7, currentLOP, upperlimit, globalparams.batch_range_pow); } 
			
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("fastpartitionkeyvalues", localcapsule[0][p0].key + templocalcapsule0[p0], PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("fastpartitionkeyvalues", localcapsule[1][p1].key + templocalcapsule1[p1], PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("fastpartitionkeyvalues", localcapsule[2][p2].key + templocalcapsule2[p2], PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("fastpartitionkeyvalues", localcapsule[3][p3].key + templocalcapsule3[p3], PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("fastpartitionkeyvalues", localcapsule[4][p4].key + templocalcapsule4[p4], PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("fastpartitionkeyvalues", localcapsule[5][p5].key + templocalcapsule5[p5], PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("fastpartitionkeyvalues", localcapsule[6][p6].key + templocalcapsule6[p6], PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("fastpartitionkeyvalues", localcapsule[7][p7].key + templocalcapsule7[p7], PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			#endif
			
 
			buffer_type loc0 = 0; // NEWCHANGE '=0'
			if(keyvalue0.key != INVALIDDATA){
				if(c==0){ templocalcapsule0[p0] += 1; }
				else{
					if(templocalcapsule0[p0] < limitsz[0][p0]){ 
						loc0 = localcapsule[0][p0].key + templocalcapsule0[p0]; 
						templocalcapsule0[p0] += 1; 
					} else { 
						loc0 = cutoff + ovsz[0];
						ovsz[0] += 1;
					}
				}
			}
 
			buffer_type loc1 = 0; // NEWCHANGE '=0'
			if(keyvalue1.key != INVALIDDATA){
				if(c==0){ templocalcapsule1[p1] += 1; }
				else{
					if(templocalcapsule1[p1] < limitsz[1][p1]){ 
						loc1 = localcapsule[1][p1].key + templocalcapsule1[p1]; 
						templocalcapsule1[p1] += 1; 
					} else { 
						loc1 = cutoff + ovsz[1];
						ovsz[1] += 1;
					}
				}
			}
 
			buffer_type loc2 = 0; // NEWCHANGE '=0'
			if(keyvalue2.key != INVALIDDATA){
				if(c==0){ templocalcapsule2[p2] += 1; }
				else{
					if(templocalcapsule2[p2] < limitsz[2][p2]){ 
						loc2 = localcapsule[2][p2].key + templocalcapsule2[p2]; 
						templocalcapsule2[p2] += 1; 
					} else { 
						loc2 = cutoff + ovsz[2];
						ovsz[2] += 1;
					}
				}
			}
 
			buffer_type loc3 = 0; // NEWCHANGE '=0'
			if(keyvalue3.key != INVALIDDATA){
				if(c==0){ templocalcapsule3[p3] += 1; }
				else{
					if(templocalcapsule3[p3] < limitsz[3][p3]){ 
						loc3 = localcapsule[3][p3].key + templocalcapsule3[p3]; 
						templocalcapsule3[p3] += 1; 
					} else { 
						loc3 = cutoff + ovsz[3];
						ovsz[3] += 1;
					}
				}
			}
 
			buffer_type loc4 = 0; // NEWCHANGE '=0'
			if(keyvalue4.key != INVALIDDATA){
				if(c==0){ templocalcapsule4[p4] += 1; }
				else{
					if(templocalcapsule4[p4] < limitsz[4][p4]){ 
						loc4 = localcapsule[4][p4].key + templocalcapsule4[p4]; 
						templocalcapsule4[p4] += 1; 
					} else { 
						loc4 = cutoff + ovsz[4];
						ovsz[4] += 1;
					}
				}
			}
 
			buffer_type loc5 = 0; // NEWCHANGE '=0'
			if(keyvalue5.key != INVALIDDATA){
				if(c==0){ templocalcapsule5[p5] += 1; }
				else{
					if(templocalcapsule5[p5] < limitsz[5][p5]){ 
						loc5 = localcapsule[5][p5].key + templocalcapsule5[p5]; 
						templocalcapsule5[p5] += 1; 
					} else { 
						loc5 = cutoff + ovsz[5];
						ovsz[5] += 1;
					}
				}
			}
 
			buffer_type loc6 = 0; // NEWCHANGE '=0'
			if(keyvalue6.key != INVALIDDATA){
				if(c==0){ templocalcapsule6[p6] += 1; }
				else{
					if(templocalcapsule6[p6] < limitsz[6][p6]){ 
						loc6 = localcapsule[6][p6].key + templocalcapsule6[p6]; 
						templocalcapsule6[p6] += 1; 
					} else { 
						loc6 = cutoff + ovsz[6];
						ovsz[6] += 1;
					}
				}
			}
 
			buffer_type loc7 = 0; // NEWCHANGE '=0'
			if(keyvalue7.key != INVALIDDATA){
				if(c==0){ templocalcapsule7[p7] += 1; }
				else{
					if(templocalcapsule7[p7] < limitsz[7][p7]){ 
						loc7 = localcapsule[7][p7].key + templocalcapsule7[p7]; 
						templocalcapsule7[p7] += 1; 
					} else { 
						loc7 = cutoff + ovsz[7];
						ovsz[7] += 1;
					}
				}
			}
			
			#ifdef _DEBUGMODE_CHECKS2
			if(keyvalue0.key != INVALIDDATA && c == 1){ actsutilityobj->checkoutofbounds("(((((( loc0", loc0, PADDEDDESTBUFFER_SIZE, cutoff, ovsz[0], NAp); }
			if(keyvalue1.key != INVALIDDATA && c == 1){ actsutilityobj->checkoutofbounds("(((((( loc1", loc1, PADDEDDESTBUFFER_SIZE, cutoff, ovsz[1], NAp); }
			if(keyvalue2.key != INVALIDDATA && c == 1){ actsutilityobj->checkoutofbounds("(((((( loc2", loc2, PADDEDDESTBUFFER_SIZE, cutoff, ovsz[2], NAp); }
			if(keyvalue3.key != INVALIDDATA && c == 1){ actsutilityobj->checkoutofbounds("(((((( loc3", loc3, PADDEDDESTBUFFER_SIZE, cutoff, ovsz[3], NAp); }
			if(keyvalue4.key != INVALIDDATA && c == 1){ actsutilityobj->checkoutofbounds("(((((( loc4", loc4, PADDEDDESTBUFFER_SIZE, cutoff, ovsz[4], NAp); }
			if(keyvalue5.key != INVALIDDATA && c == 1){ actsutilityobj->checkoutofbounds("(((((( loc5", loc5, PADDEDDESTBUFFER_SIZE, cutoff, ovsz[5], NAp); }
			if(keyvalue6.key != INVALIDDATA && c == 1){ actsutilityobj->checkoutofbounds("(((((( loc6", loc6, PADDEDDESTBUFFER_SIZE, cutoff, ovsz[6], NAp); }
			if(keyvalue7.key != INVALIDDATA && c == 1){ actsutilityobj->checkoutofbounds("(((((( loc7", loc7, PADDEDDESTBUFFER_SIZE, cutoff, ovsz[7], NAp); }
			#endif 
			
			if(keyvalue0.key != INVALIDDATA && c == 1){ destbuffer[0][loc0] = keyvalue0; }
			if(keyvalue1.key != INVALIDDATA && c == 1){ destbuffer[1][loc1] = keyvalue1; }
			if(keyvalue2.key != INVALIDDATA && c == 1){ destbuffer[2][loc2] = keyvalue2; }
			if(keyvalue3.key != INVALIDDATA && c == 1){ destbuffer[3][loc3] = keyvalue3; }
			if(keyvalue4.key != INVALIDDATA && c == 1){ destbuffer[4][loc4] = keyvalue4; }
			if(keyvalue5.key != INVALIDDATA && c == 1){ destbuffer[5][loc5] = keyvalue5; }
			if(keyvalue6.key != INVALIDDATA && c == 1){ destbuffer[6][loc6] = keyvalue6; }
			if(keyvalue7.key != INVALIDDATA && c == 1){ destbuffer[7][loc7] = keyvalue7; }
			
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("fastpartitionkeyvalues", p0, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("fastpartitionkeyvalues", p1, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("fastpartitionkeyvalues", p2, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("fastpartitionkeyvalues", p3, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("fastpartitionkeyvalues", p4, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("fastpartitionkeyvalues", p5, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("fastpartitionkeyvalues", p6, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("fastpartitionkeyvalues", p7, NUM_PARTITIONS, NAp, NAp, NAp);
			#endif
		}
		
		for(unsigned int p=0; p<NUM_PARTITIONS; p++){ // copy back
			localcapsule[0][p].value = templocalcapsule0[p];
			localcapsule[1][p].value = templocalcapsule1[p];
			localcapsule[2][p].value = templocalcapsule2[p];
			localcapsule[3][p].value = templocalcapsule3[p];
			localcapsule[4][p].value = templocalcapsule4[p];
			localcapsule[5][p].value = templocalcapsule5[p];
			localcapsule[6][p].value = templocalcapsule6[p];
			localcapsule[7][p].value = templocalcapsule7[p];
			
			templocalcapsule0[p] = 0;
			templocalcapsule1[p] = 0;
			templocalcapsule2[p] = 0;
			templocalcapsule3[p] = 0;
			templocalcapsule4[p] = 0;
			templocalcapsule5[p] = 0;
			templocalcapsule6[p] = 0;
			templocalcapsule7[p] = 0;
 
			
		}
		if(c==0){
			for(partition_type p=0; p<NUM_PARTITIONS; p++){
				sumvalues[0] += allignlowerto4_KV(localcapsule[0][p].value);
				sumvalues[1] += allignlowerto4_KV(localcapsule[1][p].value);
				sumvalues[2] += allignlowerto4_KV(localcapsule[2][p].value);
				sumvalues[3] += allignlowerto4_KV(localcapsule[3][p].value);
				sumvalues[4] += allignlowerto4_KV(localcapsule[4][p].value);
				sumvalues[5] += allignlowerto4_KV(localcapsule[5][p].value);
				sumvalues[6] += allignlowerto4_KV(localcapsule[6][p].value);
				sumvalues[7] += allignlowerto4_KV(localcapsule[7][p].value);
			}
			cutoff = INFINITI;
			for(vector_type v=0; v<VECTOR_SIZE; v++){ if(sumvalues[v] < cutoff){ cutoff = sumvalues[v]; }}
			calculatemanyoffsets_allignby4_lower(localcapsule, limitsz, NUM_PARTITIONS, cutoff); 
		}
	}
	
	for(buffer_type i=cutoff; i<SRCBUFFER_SIZE; i++){ // CRITICAL FIXME.// JUSTCHANGED
	#pragma HLS LOOP_TRIPCOUNT min=0 max=16 avg=16	
	#pragma HLS PIPELINE II=1
		sourcebuffer[0][i] = destbuffer[0][i];
		sourcebuffer[1][i] = destbuffer[1][i];
		sourcebuffer[2][i] = destbuffer[2][i];
		sourcebuffer[3][i] = destbuffer[3][i];
		sourcebuffer[4][i] = destbuffer[4][i];
		sourcebuffer[5][i] = destbuffer[5][i];
		sourcebuffer[6][i] = destbuffer[6][i];
		sourcebuffer[7][i] = destbuffer[7][i];
	}
	
	#ifdef _DEBUGMODE_STATS
	for(vector_type v=0; v<VECTOR_SIZE; v++){ actsutilityobj->globalvar_inmemory_counttotalvalidkeyvalues(actsutilityobj->ugetvaluecount((keyvalue_t *)localcapsule[v], NUM_PARTITIONS)); } // REMOVEME. unmatched data types
	#endif
	#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_RUNKERNELPRINTS)
	cout<<"--------------- cutoff: "<<cutoff<<", ovsz[0]: "<<ovsz[0]<<", (cutoff+ovsz[0]): "<<cutoff+ovsz[0]<<", chunk_size: "<<chunk_size<<"  -----------------------------"<<endl;
	#endif 
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("maxpartitionkeyvalues:: cutoff", cutoff, SRCBUFFER_SIZE+1, NAp, NAp, NAp);
	// for(unsigned int v=0; v<VECTOR_SIZE; v++){ if(ovsz[v] != ovsz[0]){ cout<<"ovsz["<<v<<"] != ovsz[0]. cutoff: "<<cutoff<<". printint values and exiting..."<<endl; actsutilityobj->printvalues("fastpartitionkeyvalues.ovsz", ovsz, VECTOR_SIZE); exit(EXIT_FAILURE); }} // FIXME.
	// for(unsigned int v=0; v<VECTOR_SIZE; v++){ actsutilityobj->checkforequal("maxpartitionkeyvalues:: ovsz["+std::to_string(v)+"].ovsz[0]", ovsz[v], ovsz[0], v, ovsz[0], NAp); }
	// actsutilityobj->checkforequal("maxpartitionkeyvalues.cutoff+ovsz[0],chunk_size", cutoff+ovsz[0], chunk_size, cutoff, ovsz[0], NAp); // REMOVEME.
	actsutilityobj->updatemincutoffseen(cutoff);
	actsutilityobj->updatemaxcutoffseen(cutoff);
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	unsigned int dummy[NUM_PARTITIONS];
	if(false){ actsutilityobj->printprofile("fastpartitionkeyvalues.destbuffer", (keyvalue_t *)&destbuffer[0], PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, currentLOP, upperlimit, globalparams.batch_range_pow); }
	if(false){ actsutilityobj->printprofileso1("fastpartitionkeyvalues.destbuffer", destbuffer, localcapsule, currentLOP, upperlimit, globalparams.batch_range_pow, dummy); }
	for(unsigned int v=0; v<8; v++){ actsutilityobj->printkeyvalues("fastpartitionkeyvalues.localcapsule[v]", (keyvalue_t *)localcapsule[v], NUM_PARTITIONS); }
	for(unsigned int v=0; v<8; v++){ actsutilityobj->printvalues("fastpartitionkeyvalues.limitsz[v]", (value_t *)limitsz[v], NUM_PARTITIONS); }
	#endif
	return cutoff;
}

buffer_type 
	#ifdef SW 
	actslw::
	#endif
partitionkeyvalues_finegrainedpipeline(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t localcapsule[VECTOR_SIZE][NUM_PARTITIONS], step_type currentLOP, vertex_t upperlimit, travstate_t travstate, buffer_type size_kvs, globalparams_t globalparams){
	#pragma HLS INLINE
	#ifdef MAXPERFORMANCE
	return maxpartitionkeyvalues(enable, sourcebuffer, destbuffer, localcapsule, currentLOP, upperlimit, travstate, size_kvs, globalparams);
	#else
	return fastpartitionkeyvalues(enable, sourcebuffer, destbuffer, localcapsule, currentLOP, upperlimit, travstate, size_kvs, globalparams);
	#endif
}

void 
	#ifdef SW 
	actslw::
	#endif 
savekeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_t buffer[8][PADDEDDESTBUFFER_SIZE], keyvalue_t * globalcapsule, skeyvalue_t localcapsule[NUM_PARTITIONS], batch_type globalbaseaddress_kvs, globalparams_t globalparams){				
	if(enable == OFF){ return; }

	#ifdef _DEBUGMODE_CHECKS
	actsutilityobj->printkeyvalues("savekeyvalues::localcapsule", localcapsule, NUM_PARTITIONS);
	actsutilityobj->printvaluecount("savekeyvalues::localcapsule", localcapsule, NUM_PARTITIONS);
	actsutilityobj->scankeyvalues("savekeyvalues::buffer", (keyvalue_t *)buffer, localcapsule, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	
	analysis_type analysis_destpartitionsz = PADDEDDESTBUFFER_SIZE / NUM_PARTITIONS;
	SAVEPARTITIONS_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		batch_type dramoffset_kvs = globalbaseaddress_kvs + ((globalcapsule[p].key + globalcapsule[p].value) / VECTOR_SIZE);
		buffer_type bramoffset_kvs = localcapsule[p].key / VECTOR_SIZE;
		buffer_type realsize_kvs = localcapsule[p].value / VECTOR_SIZE;
		buffer_type size_kvs = getpartitionwritesz(realsize_kvs, bramoffset_kvs);
		
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("savekeyvalues 23", bramoffset_kvs + size_kvs, PADDEDDESTBUFFER_SIZE + 1, p, NAp, NAp);
		actsutilityobj->checkoutofbounds("savekeyvalues 25", ((globalcapsule[p].key + globalcapsule[p].value) / VECTOR_SIZE), KVDRAMSZ + 1, p, NAp, NAp);
		#endif
		SAVEPARTITIONS_LOOP1B: for(buffer_type i=0; i<size_kvs; i++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_destpartitionsz avg=analysis_destpartitionsz
		#pragma HLS PIPELINE II=1
			#ifdef _WIDEWORD
			kvdram[dramoffset_kvs + i].range(31, 0) = buffer[0][bramoffset_kvs + i].key; 
			kvdram[dramoffset_kvs + i].range(63, 32) = buffer[0][bramoffset_kvs + i].value; 
			kvdram[dramoffset_kvs + i].range(95, 64) = buffer[1][bramoffset_kvs + i].key; 
			kvdram[dramoffset_kvs + i].range(127, 96) = buffer[1][bramoffset_kvs + i].value; 
			kvdram[dramoffset_kvs + i].range(159, 128) = buffer[2][bramoffset_kvs + i].key; 
			kvdram[dramoffset_kvs + i].range(191, 160) = buffer[2][bramoffset_kvs + i].value; 
			kvdram[dramoffset_kvs + i].range(223, 192) = buffer[3][bramoffset_kvs + i].key; 
			kvdram[dramoffset_kvs + i].range(255, 224) = buffer[3][bramoffset_kvs + i].value; 
			kvdram[dramoffset_kvs + i].range(287, 256) = buffer[4][bramoffset_kvs + i].key; 
			kvdram[dramoffset_kvs + i].range(319, 288) = buffer[4][bramoffset_kvs + i].value; 
			kvdram[dramoffset_kvs + i].range(351, 320) = buffer[5][bramoffset_kvs + i].key; 
			kvdram[dramoffset_kvs + i].range(383, 352) = buffer[5][bramoffset_kvs + i].value; 
			kvdram[dramoffset_kvs + i].range(415, 384) = buffer[6][bramoffset_kvs + i].key; 
			kvdram[dramoffset_kvs + i].range(447, 416) = buffer[6][bramoffset_kvs + i].value; 
			kvdram[dramoffset_kvs + i].range(479, 448) = buffer[7][bramoffset_kvs + i].key; 
			kvdram[dramoffset_kvs + i].range(511, 480) = buffer[7][bramoffset_kvs + i].value; 
			#else 
			kvdram[dramoffset_kvs + i].data[0] = buffer[0][bramoffset_kvs + i]; 
			kvdram[dramoffset_kvs + i].data[1] = buffer[1][bramoffset_kvs + i]; 
			kvdram[dramoffset_kvs + i].data[2] = buffer[2][bramoffset_kvs + i]; 
			kvdram[dramoffset_kvs + i].data[3] = buffer[3][bramoffset_kvs + i]; 
			kvdram[dramoffset_kvs + i].data[4] = buffer[4][bramoffset_kvs + i]; 
			kvdram[dramoffset_kvs + i].data[5] = buffer[5][bramoffset_kvs + i]; 
			kvdram[dramoffset_kvs + i].data[6] = buffer[6][bramoffset_kvs + i]; 
			kvdram[dramoffset_kvs + i].data[7] = buffer[7][bramoffset_kvs + i]; 
			#endif 
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countkvspartitionswritten_actual(VECTOR_SIZE);
			#endif
		}
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvspartitionswritten(realsize_kvs * VECTOR_SIZE);
		#endif
	}
	SAVEPARTITIONS_LOOP2: for(partition_type p=0; p<NUM_PARTITIONS; p++){ globalcapsule[p].value += localcapsule[p].value; }
	
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("savekeyvalues::globalcapsule 34", globalcapsule[NUM_PARTITIONS-1].key + globalcapsule[NUM_PARTITIONS-1].value, KVDRAMSZ, NAp, NAp, NAp);
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"readkeyvalues:: keyvalues saved: offset_kvs from: "<<globalbaseaddress_kvs + ((globalcapsule[0].key + globalcapsule[0].value) / VECTOR_SIZE)<<endl;
	actsutilityobj->printkeyvalues("actsutility::savekeyvalues: globalcapsule.", globalcapsule, NUM_PARTITIONS);
	#endif
	// actsutilityobj->printglobalvars();
	// exit(EXIT_SUCCESS);
	return;
}

// reduce functions
void 
	#ifdef SW 
	actslw::
	#endif 
readvertices(bool_type enable, uint512_dt * kvdram, keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], batch_type offset_kvs, buffer_type size_kvs){
	if(enable == OFF){ return; }
		
	READVERTICES_LOOP: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		buffer[0][i].key = kvdram[offset_kvs + i].range(31, 0); 
		buffer[0][i].value = kvdram[offset_kvs + i].range(63, 32); 
		buffer[1][i].key = kvdram[offset_kvs + i].range(95, 64); 
		buffer[1][i].value = kvdram[offset_kvs + i].range(127, 96); 
		buffer[2][i].key = kvdram[offset_kvs + i].range(159, 128); 
		buffer[2][i].value = kvdram[offset_kvs + i].range(191, 160); 
		buffer[3][i].key = kvdram[offset_kvs + i].range(223, 192); 
		buffer[3][i].value = kvdram[offset_kvs + i].range(255, 224); 
		buffer[4][i].key = kvdram[offset_kvs + i].range(287, 256); 
		buffer[4][i].value = kvdram[offset_kvs + i].range(319, 288); 
		buffer[5][i].key = kvdram[offset_kvs + i].range(351, 320); 
		buffer[5][i].value = kvdram[offset_kvs + i].range(383, 352); 
		buffer[6][i].key = kvdram[offset_kvs + i].range(415, 384); 
		buffer[6][i].value = kvdram[offset_kvs + i].range(447, 416); 
		buffer[7][i].key = kvdram[offset_kvs + i].range(479, 448); 
		buffer[7][i].value = kvdram[offset_kvs + i].range(511, 480); 
		#else 
		buffer[0][i] = kvdram[offset_kvs + i].data[0]; 
		buffer[1][i] = kvdram[offset_kvs + i].data[1]; 
		buffer[2][i] = kvdram[offset_kvs + i].data[2]; 
		buffer[3][i] = kvdram[offset_kvs + i].data[3]; 
		buffer[4][i] = kvdram[offset_kvs + i].data[4]; 
		buffer[5][i] = kvdram[offset_kvs + i].data[5]; 
		buffer[6][i] = kvdram[offset_kvs + i].data[6]; 
		buffer[7][i] = kvdram[offset_kvs + i].data[7]; 
		#endif 
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	#ifdef _DEBUGMODE_KERNELPRINTS2
	cout<< TIMINGRESULTSCOLOR<<"readvertices:: vertices read: offset: "<<(offset_kvs - BASEOFFSET_VERTEXPTR_KVS) * VECTOR_SIZE<<"-"<<(offset_kvs + size_kvs - BASEOFFSET_VERTEXPTR_KVS) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl;
	#endif
	return;
}

void 
	#ifdef SW 
	actslw::
	#endif 
replicatedata(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], buffer_type sourceoffset, buffer_type size){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount = PADDEDDESTBUFFER_SIZE;

	vector_type src_v = 0;
	buffer_type src_i = 0;
	buffer_type sourceoffset_kvs = sourceoffset / VECTOR_SIZE;
	REPLICATEDATA_LOOP: for(buffer_type i=0; i<size; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("replicatedata.1", i, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, sourceoffset, sourceoffset_kvs, size);
		actsutilityobj->checkoutofbounds("replicatedata.2", sourceoffset + src_i, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, sourceoffset, sourceoffset_kvs, size);
		#endif
		
		keyvalue_t keyvalue = sourcebuffer[src_v][sourceoffset_kvs + src_i]; // get a single source
		src_v++; if(src_v==VECTOR_SIZE){ src_v=0; src_i+=1; }
		
		destbuffer[0][i] = keyvalue; // load to multiple destinations
		destbuffer[1][i] = keyvalue; // load to multiple destinations
		destbuffer[2][i] = keyvalue; // load to multiple destinations
		destbuffer[3][i] = keyvalue; // load to multiple destinations
		destbuffer[4][i] = keyvalue; // load to multiple destinations
		destbuffer[5][i] = keyvalue; // load to multiple destinations
		destbuffer[6][i] = keyvalue; // load to multiple destinations
		destbuffer[7][i] = keyvalue; // load to multiple destinations
	}
	return;
}

void 
	#ifdef SW 
	actslw::
	#endif
reduce(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], vertex_t upperlimit, unsigned int GraphIter, unsigned int GraphAlgo, travstate_t travstate, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE;
	buffer_type chunk_size = getchunksize_kvs(SRCBUFFER_SIZE, travstate, 0);

	REDUCE_LOOP: for(buffer_type i=0; i<chunk_size; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz
	#ifdef MAXPERFORMANCE
	// #pragma HLS PIPELINE II=2
	#pragma HLS PIPELINE II=3
	#else 
	#pragma HLS PIPELINE II=3	
	#endif 
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
		
		#ifdef _DEBUGMODE_KERNELPRINTS
		if(keyvalue0.key != INVALIDDATA){ cout<<"REDUCE SEEN @ reduce:: i: "<<i<<", loc0: "<<loc0<<", keyvalue0.key: "<<keyvalue0.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; }
		if(keyvalue1.key != INVALIDDATA){ cout<<"REDUCE SEEN @ reduce:: i: "<<i<<", loc1: "<<loc1<<", keyvalue1.key: "<<keyvalue1.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; }
		if(keyvalue2.key != INVALIDDATA){ cout<<"REDUCE SEEN @ reduce:: i: "<<i<<", loc2: "<<loc2<<", keyvalue2.key: "<<keyvalue2.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; }
		if(keyvalue3.key != INVALIDDATA){ cout<<"REDUCE SEEN @ reduce:: i: "<<i<<", loc3: "<<loc3<<", keyvalue3.key: "<<keyvalue3.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; }
		if(keyvalue4.key != INVALIDDATA){ cout<<"REDUCE SEEN @ reduce:: i: "<<i<<", loc4: "<<loc4<<", keyvalue4.key: "<<keyvalue4.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; }
		if(keyvalue5.key != INVALIDDATA){ cout<<"REDUCE SEEN @ reduce:: i: "<<i<<", loc5: "<<loc5<<", keyvalue5.key: "<<keyvalue5.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; }
		if(keyvalue6.key != INVALIDDATA){ cout<<"REDUCE SEEN @ reduce:: i: "<<i<<", loc6: "<<loc6<<", keyvalue6.key: "<<keyvalue6.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; }
		if(keyvalue7.key != INVALIDDATA){ cout<<"REDUCE SEEN @ reduce:: i: "<<i<<", loc7: "<<loc7<<", keyvalue7.key: "<<keyvalue7.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; }
		#endif 
		
		if(loc0 >= globalparams.applyvertexbuffersz && keyvalue0.key != INVALIDDATA){
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->globalstats_counterrorsinreduce(1);
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTREDUCEERRORS
			cout<<"ERROR SEEN @ reduce:: i: "<<i<<", loc0: "<<loc0<<", keyvalue0.key: "<<keyvalue0.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			if(actsutilityobj->globalstats_getcounterrorsinreduce() > 0){ cout<<"too many ("<<actsutilityobj->globalstats_getcounterrorsinreduce()<<") reduce errors. EXITING"<<endl; exit(EXIT_FAILURE); } // REMOVEME. // FIXME. ENSURE PERFECTION.
			#endif 
			#endif
			loc0 = 0;
		} // REMOVEME.
		if(loc1 >= globalparams.applyvertexbuffersz && keyvalue1.key != INVALIDDATA){
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->globalstats_counterrorsinreduce(1);
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTREDUCEERRORS
			cout<<"ERROR SEEN @ reduce:: i: "<<i<<", loc1: "<<loc1<<", keyvalue1.key: "<<keyvalue1.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			if(actsutilityobj->globalstats_getcounterrorsinreduce() > 0){ cout<<"too many ("<<actsutilityobj->globalstats_getcounterrorsinreduce()<<") reduce errors. EXITING"<<endl; exit(EXIT_FAILURE); } // REMOVEME. // FIXME. ENSURE PERFECTION.
			#endif 
			#endif
			loc1 = 0;
		} // REMOVEME.
		if(loc2 >= globalparams.applyvertexbuffersz && keyvalue2.key != INVALIDDATA){
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->globalstats_counterrorsinreduce(1);
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTREDUCEERRORS
			cout<<"ERROR SEEN @ reduce:: i: "<<i<<", loc2: "<<loc2<<", keyvalue2.key: "<<keyvalue2.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			if(actsutilityobj->globalstats_getcounterrorsinreduce() > 0){ cout<<"too many ("<<actsutilityobj->globalstats_getcounterrorsinreduce()<<") reduce errors. EXITING"<<endl; exit(EXIT_FAILURE); } // REMOVEME. // FIXME. ENSURE PERFECTION.
			#endif 
			#endif
			loc2 = 0;
		} // REMOVEME.
		if(loc3 >= globalparams.applyvertexbuffersz && keyvalue3.key != INVALIDDATA){
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->globalstats_counterrorsinreduce(1);
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTREDUCEERRORS
			cout<<"ERROR SEEN @ reduce:: i: "<<i<<", loc3: "<<loc3<<", keyvalue3.key: "<<keyvalue3.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			if(actsutilityobj->globalstats_getcounterrorsinreduce() > 0){ cout<<"too many ("<<actsutilityobj->globalstats_getcounterrorsinreduce()<<") reduce errors. EXITING"<<endl; exit(EXIT_FAILURE); } // REMOVEME. // FIXME. ENSURE PERFECTION.
			#endif 
			#endif
			loc3 = 0;
		} // REMOVEME.
		if(loc4 >= globalparams.applyvertexbuffersz && keyvalue4.key != INVALIDDATA){
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->globalstats_counterrorsinreduce(1);
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTREDUCEERRORS
			cout<<"ERROR SEEN @ reduce:: i: "<<i<<", loc4: "<<loc4<<", keyvalue4.key: "<<keyvalue4.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			if(actsutilityobj->globalstats_getcounterrorsinreduce() > 0){ cout<<"too many ("<<actsutilityobj->globalstats_getcounterrorsinreduce()<<") reduce errors. EXITING"<<endl; exit(EXIT_FAILURE); } // REMOVEME. // FIXME. ENSURE PERFECTION.
			#endif 
			#endif
			loc4 = 0;
		} // REMOVEME.
		if(loc5 >= globalparams.applyvertexbuffersz && keyvalue5.key != INVALIDDATA){
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->globalstats_counterrorsinreduce(1);
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTREDUCEERRORS
			cout<<"ERROR SEEN @ reduce:: i: "<<i<<", loc5: "<<loc5<<", keyvalue5.key: "<<keyvalue5.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			if(actsutilityobj->globalstats_getcounterrorsinreduce() > 0){ cout<<"too many ("<<actsutilityobj->globalstats_getcounterrorsinreduce()<<") reduce errors. EXITING"<<endl; exit(EXIT_FAILURE); } // REMOVEME. // FIXME. ENSURE PERFECTION.
			#endif 
			#endif
			loc5 = 0;
		} // REMOVEME.
		if(loc6 >= globalparams.applyvertexbuffersz && keyvalue6.key != INVALIDDATA){
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->globalstats_counterrorsinreduce(1);
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTREDUCEERRORS
			cout<<"ERROR SEEN @ reduce:: i: "<<i<<", loc6: "<<loc6<<", keyvalue6.key: "<<keyvalue6.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			if(actsutilityobj->globalstats_getcounterrorsinreduce() > 0){ cout<<"too many ("<<actsutilityobj->globalstats_getcounterrorsinreduce()<<") reduce errors. EXITING"<<endl; exit(EXIT_FAILURE); } // REMOVEME. // FIXME. ENSURE PERFECTION.
			#endif 
			#endif
			loc6 = 0;
		} // REMOVEME.
		if(loc7 >= globalparams.applyvertexbuffersz && keyvalue7.key != INVALIDDATA){
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->globalstats_counterrorsinreduce(1);
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTREDUCEERRORS
			cout<<"ERROR SEEN @ reduce:: i: "<<i<<", loc7: "<<loc7<<", keyvalue7.key: "<<keyvalue7.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			if(actsutilityobj->globalstats_getcounterrorsinreduce() > 0){ cout<<"too many ("<<actsutilityobj->globalstats_getcounterrorsinreduce()<<") reduce errors. EXITING"<<endl; exit(EXIT_FAILURE); } // REMOVEME. // FIXME. ENSURE PERFECTION.
			#endif 
			#endif
			loc7 = 0;
		} // REMOVEME.
		
		vertex_t rowindex0 = loc0 / 2;
		vertex_t colindex0 = loc0 % 2;
		vertex_t rowindex1 = loc1 / 2;
		vertex_t colindex1 = loc1 % 2;
		vertex_t rowindex2 = loc2 / 2;
		vertex_t colindex2 = loc2 % 2;
		vertex_t rowindex3 = loc3 / 2;
		vertex_t colindex3 = loc3 % 2;
		vertex_t rowindex4 = loc4 / 2;
		vertex_t colindex4 = loc4 % 2;
		vertex_t rowindex5 = loc5 / 2;
		vertex_t colindex5 = loc5 % 2;
		vertex_t rowindex6 = loc6 / 2;
		vertex_t colindex6 = loc6 % 2;
		vertex_t rowindex7 = loc7 / 2;
		vertex_t colindex7 = loc7 % 2;
		
		keyvalue_t vprop0;
		if(keyvalue0.key != INVALIDDATA){ vprop0 = destbuffer[0][rowindex0]; }
		keyvalue_t vprop1;
		if(keyvalue1.key != INVALIDDATA){ vprop1 = destbuffer[1][rowindex1]; }
		keyvalue_t vprop2;
		if(keyvalue2.key != INVALIDDATA){ vprop2 = destbuffer[2][rowindex2]; }
		keyvalue_t vprop3;
		if(keyvalue3.key != INVALIDDATA){ vprop3 = destbuffer[3][rowindex3]; }
		keyvalue_t vprop4;
		if(keyvalue4.key != INVALIDDATA){ vprop4 = destbuffer[4][rowindex4]; }
		keyvalue_t vprop5;
		if(keyvalue5.key != INVALIDDATA){ vprop5 = destbuffer[5][rowindex5]; }
		keyvalue_t vprop6;
		if(keyvalue6.key != INVALIDDATA){ vprop6 = destbuffer[6][rowindex6]; }
		keyvalue_t vprop7;
		if(keyvalue7.key != INVALIDDATA){ vprop7 = destbuffer[7][rowindex7]; }
		
		value_t temp0;
		if(colindex0 == 0){ temp0 = vprop0.key; }
		else { temp0 = vprop0.value; }
		value_t temp1;
		if(colindex1 == 0){ temp1 = vprop1.key; }
		else { temp1 = vprop1.value; }
		value_t temp2;
		if(colindex2 == 0){ temp2 = vprop2.key; }
		else { temp2 = vprop2.value; }
		value_t temp3;
		if(colindex3 == 0){ temp3 = vprop3.key; }
		else { temp3 = vprop3.value; }
		value_t temp4;
		if(colindex4 == 0){ temp4 = vprop4.key; }
		else { temp4 = vprop4.value; }
		value_t temp5;
		if(colindex5 == 0){ temp5 = vprop5.key; }
		else { temp5 = vprop5.value; }
		value_t temp6;
		if(colindex6 == 0){ temp6 = vprop6.key; }
		else { temp6 = vprop6.value; }
		value_t temp7;
		if(colindex7 == 0){ temp7 = vprop7.key; }
		else { temp7 = vprop7.value; }
		
		value_t rettemp0 = reducefunc(temp0, keyvalue0.value, GraphIter, GraphAlgo);
		value_t rettemp1 = reducefunc(temp1, keyvalue1.value, GraphIter, GraphAlgo);
		value_t rettemp2 = reducefunc(temp2, keyvalue2.value, GraphIter, GraphAlgo);
		value_t rettemp3 = reducefunc(temp3, keyvalue3.value, GraphIter, GraphAlgo);
		value_t rettemp4 = reducefunc(temp4, keyvalue4.value, GraphIter, GraphAlgo);
		value_t rettemp5 = reducefunc(temp5, keyvalue5.value, GraphIter, GraphAlgo);
		value_t rettemp6 = reducefunc(temp6, keyvalue6.value, GraphIter, GraphAlgo);
		value_t rettemp7 = reducefunc(temp7, keyvalue7.value, GraphIter, GraphAlgo);
		
		#ifdef _DEBUGMODE_KERNELPRINTS
		if(keyvalue0.key != INVALIDDATA){ cout<<"REDUCEFUNC RESULT @ reduce:: rettemp0: "<<rettemp0<<endl; }
		if(keyvalue1.key != INVALIDDATA){ cout<<"REDUCEFUNC RESULT @ reduce:: rettemp1: "<<rettemp1<<endl; }
		if(keyvalue2.key != INVALIDDATA){ cout<<"REDUCEFUNC RESULT @ reduce:: rettemp2: "<<rettemp2<<endl; }
		if(keyvalue3.key != INVALIDDATA){ cout<<"REDUCEFUNC RESULT @ reduce:: rettemp3: "<<rettemp3<<endl; }
		if(keyvalue4.key != INVALIDDATA){ cout<<"REDUCEFUNC RESULT @ reduce:: rettemp4: "<<rettemp4<<endl; }
		if(keyvalue5.key != INVALIDDATA){ cout<<"REDUCEFUNC RESULT @ reduce:: rettemp5: "<<rettemp5<<endl; }
		if(keyvalue6.key != INVALIDDATA){ cout<<"REDUCEFUNC RESULT @ reduce:: rettemp6: "<<rettemp6<<endl; }
		if(keyvalue7.key != INVALIDDATA){ cout<<"REDUCEFUNC RESULT @ reduce:: rettemp7: "<<rettemp7<<endl; }
		#endif 
		
		if(colindex0 == 0){ vprop0.key = rettemp0; }
		else { vprop0.value = rettemp0; }
		if(colindex1 == 0){ vprop1.key = rettemp1; }
		else { vprop1.value = rettemp1; }
		if(colindex2 == 0){ vprop2.key = rettemp2; }
		else { vprop2.value = rettemp2; }
		if(colindex3 == 0){ vprop3.key = rettemp3; }
		else { vprop3.value = rettemp3; }
		if(colindex4 == 0){ vprop4.key = rettemp4; }
		else { vprop4.value = rettemp4; }
		if(colindex5 == 0){ vprop5.key = rettemp5; }
		else { vprop5.value = rettemp5; }
		if(colindex6 == 0){ vprop6.key = rettemp6; }
		else { vprop6.value = rettemp6; }
		if(colindex7 == 0){ vprop7.key = rettemp7; }
		else { vprop7.value = rettemp7; }
		
		if(keyvalue0.key != INVALIDDATA){ destbuffer[0][rowindex0] = vprop0; }
		if(keyvalue1.key != INVALIDDATA){ destbuffer[1][rowindex1] = vprop1; }
		if(keyvalue2.key != INVALIDDATA){ destbuffer[2][rowindex2] = vprop2; }
		if(keyvalue3.key != INVALIDDATA){ destbuffer[3][rowindex3] = vprop3; }
		if(keyvalue4.key != INVALIDDATA){ destbuffer[4][rowindex4] = vprop4; }
		if(keyvalue5.key != INVALIDDATA){ destbuffer[5][rowindex5] = vprop5; }
		if(keyvalue6.key != INVALIDDATA){ destbuffer[6][rowindex6] = vprop6; }
		if(keyvalue7.key != INVALIDDATA){ destbuffer[7][rowindex7] = vprop7; }
		
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
unifydata(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], buffer_type destoffset, buffer_type size, unsigned int GraphAlgo){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount = (APPLYVERTEXBUFFERSZ / 2);// / VECTOR_SIZE;
	
	buffer_type dest_v = 0;
	buffer_type dest_i = 0;
	buffer_type destoffset_kvs = destoffset / VECTOR_SIZE;
	UNIFYDATA_LOOP: for(buffer_type i=0; i<size; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE
		#ifdef PR_ALGORITHM
		value_t data1 = 0;
		value_t data2 = 0;
		#elif defined(BFS_ALGORITHM)
		value_t data1 = INFINITI;
		value_t data2 = INFINITI;
		#elif defined(SSSP_ALGORITHM)
		value_t data1 = INFINITI;
		value_t data2 = INFINITI;
		#elif defined(BC_ALGORITHM)
		value_t data1 = INFINITI;
		value_t data2 = INFINITI;
		#endif
		
		// value_t data1 = 0;
		// value_t data2 = 0;
		// if(GraphAlgo == PAGERANK){
			// data1 = 0;
			// data2 = 0;
		// } else if (GraphAlgo == BREADTHFIRSTSEARCH){
			// data1 = INFINITI;
			// data2 = INFINITI;
		// } else if (GraphAlgo == SSSP){
			// data1 = INFINITI;
			// data2 = INFINITI;
		// } else { 
			// data1 = 0;
			// data2 = 0;
		// }
		
		for(vector_type v=0; v<VECTOR_SIZE; v++){ // unify multple sources
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("unifydata", i, PADDEDDESTBUFFER_SIZE, destoffset, destoffset_kvs, size);
			#endif
			data1 = mergefunc(data1, sourcebuffer[v][i].key, GraphAlgo); // merge all values on source row
			data2 = mergefunc(data2, sourcebuffer[v][i].value, GraphAlgo); // merge all values on source row
		}
		
		destbuffer[dest_v][destoffset_kvs + dest_i].key = data1; // set to single destination
		destbuffer[dest_v][destoffset_kvs + dest_i].value = data2; // set to single destination
		dest_v+=1; if(dest_v == VECTOR_SIZE){ dest_v=0; dest_i+=1; }
	}	
	return;
}

void 
	#ifdef SW 
	actslw::
	#endif 
savevertices(bool_type enable, uint512_dt * kvdram, keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], batch_type offset_kvs, buffer_type size_kvs){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount = APPLYVERTEXBUFFERSZ / 2;
	
	SAVEVERTICES_LOOP: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		kvdram[offset_kvs + i].range(31, 0) = buffer[0][i].key; 
		kvdram[offset_kvs + i].range(63, 32) = buffer[0][i].value; 
		kvdram[offset_kvs + i].range(95, 64) = buffer[1][i].key; 
		kvdram[offset_kvs + i].range(127, 96) = buffer[1][i].value; 
		kvdram[offset_kvs + i].range(159, 128) = buffer[2][i].key; 
		kvdram[offset_kvs + i].range(191, 160) = buffer[2][i].value; 
		kvdram[offset_kvs + i].range(223, 192) = buffer[3][i].key; 
		kvdram[offset_kvs + i].range(255, 224) = buffer[3][i].value; 
		kvdram[offset_kvs + i].range(287, 256) = buffer[4][i].key; 
		kvdram[offset_kvs + i].range(319, 288) = buffer[4][i].value; 
		kvdram[offset_kvs + i].range(351, 320) = buffer[5][i].key; 
		kvdram[offset_kvs + i].range(383, 352) = buffer[5][i].value; 
		kvdram[offset_kvs + i].range(415, 384) = buffer[6][i].key; 
		kvdram[offset_kvs + i].range(447, 416) = buffer[6][i].value; 
		kvdram[offset_kvs + i].range(479, 448) = buffer[7][i].key; 
		kvdram[offset_kvs + i].range(511, 480) = buffer[7][i].value; 
		#else 
		kvdram[offset_kvs + i].data[0] = buffer[0][i];
		kvdram[offset_kvs + i].data[1] = buffer[1][i];
		kvdram[offset_kvs + i].data[2] = buffer[2][i];
		kvdram[offset_kvs + i].data[3] = buffer[3][i];
		kvdram[offset_kvs + i].data[4] = buffer[4][i];
		kvdram[offset_kvs + i].data[5] = buffer[5][i];
		kvdram[offset_kvs + i].data[6] = buffer[6][i];
		kvdram[offset_kvs + i].data[7] = buffer[7][i];
		#endif 
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvswritten(VECTOR_SIZE);
		#endif
	}
	#ifdef _DEBUGMODE_KERNELPRINTS2
	cout<< TIMINGRESULTSCOLOR<<"savevertices:: vertices saved: offset: "<<offset_kvs * VECTOR_SIZE<<"-"<<(offset_kvs + size_kvs) * VECTOR_SIZE<<", number of vertex datas written: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl;
	#endif
	return;
}

// process-edges function
void 
	#ifdef SW 
	actslw::
	#endif
process_edges(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], vertex_t upperlimit, unsigned int GraphIter, unsigned int GraphAlgo, travstate_t travstate, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE;
	buffer_type chunk_size = getchunksize_kvs(SRCBUFFER_SIZE, travstate, 0);
	
	PROCESSEDGES_LOOP: for(buffer_type i=0; i<chunk_size; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
	#pragma HLS PIPELINE II=2
		keyvalue_t edge0 = destbuffer[0][i];
		keyvalue_t edge1 = destbuffer[1][i];
		keyvalue_t edge2 = destbuffer[2][i];
		keyvalue_t edge3 = destbuffer[3][i];
		keyvalue_t edge4 = destbuffer[4][i];
		keyvalue_t edge5 = destbuffer[5][i];
		keyvalue_t edge6 = destbuffer[6][i];
		keyvalue_t edge7 = destbuffer[7][i];
		
		value_t localsourceid0 = edge0.key - upperlimit;
		value_t localsourceid1 = edge1.key - upperlimit;
		value_t localsourceid2 = edge2.key - upperlimit;
		value_t localsourceid3 = edge3.key - upperlimit;
		value_t localsourceid4 = edge4.key - upperlimit;
		value_t localsourceid5 = edge5.key - upperlimit;
		value_t localsourceid6 = edge6.key - upperlimit;
		value_t localsourceid7 = edge7.key - upperlimit;
		
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"PROCESSEDGE SEEN @ process_edges:: i: "<<i<<", destbuffer[0][i].key: "<<destbuffer[0][i].value<<", destbuffer[0][i].value: "<<destbuffer[0][i].key<<", upperlimit: "<<upperlimit<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
		cout<<"PROCESSEDGE SEEN @ process_edges:: i: "<<i<<", destbuffer[1][i].key: "<<destbuffer[1][i].value<<", destbuffer[1][i].value: "<<destbuffer[1][i].key<<", upperlimit: "<<upperlimit<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
		cout<<"PROCESSEDGE SEEN @ process_edges:: i: "<<i<<", destbuffer[2][i].key: "<<destbuffer[2][i].value<<", destbuffer[2][i].value: "<<destbuffer[2][i].key<<", upperlimit: "<<upperlimit<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
		cout<<"PROCESSEDGE SEEN @ process_edges:: i: "<<i<<", destbuffer[3][i].key: "<<destbuffer[3][i].value<<", destbuffer[3][i].value: "<<destbuffer[3][i].key<<", upperlimit: "<<upperlimit<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
		cout<<"PROCESSEDGE SEEN @ process_edges:: i: "<<i<<", destbuffer[4][i].key: "<<destbuffer[4][i].value<<", destbuffer[4][i].value: "<<destbuffer[4][i].key<<", upperlimit: "<<upperlimit<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
		cout<<"PROCESSEDGE SEEN @ process_edges:: i: "<<i<<", destbuffer[5][i].key: "<<destbuffer[5][i].value<<", destbuffer[5][i].value: "<<destbuffer[5][i].key<<", upperlimit: "<<upperlimit<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
		cout<<"PROCESSEDGE SEEN @ process_edges:: i: "<<i<<", destbuffer[6][i].key: "<<destbuffer[6][i].value<<", destbuffer[6][i].value: "<<destbuffer[6][i].key<<", upperlimit: "<<upperlimit<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
		cout<<"PROCESSEDGE SEEN @ process_edges:: i: "<<i<<", destbuffer[7][i].key: "<<destbuffer[7][i].value<<", destbuffer[7][i].value: "<<destbuffer[7][i].key<<", upperlimit: "<<upperlimit<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
	
		#endif 
		
		if(localsourceid0 >= PADDEDDESTBUFFER_SIZE){
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->globalstats_counterrorsinprocessedges(1);
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTPROCESSEDGESERRORS
			cout<<"ERROR SEEN @ process_edges:: i: "<<i<<", sourceid: "<<destbuffer[0][i].key<<", localsourceid0: "<<localsourceid0<<", upperlimit: "<<upperlimit<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			if(actsutilityobj->globalstats_getcounterrorsinprocessedges() > 100000){ cout<<"too many errors seen @ process_edges ("<<actsutilityobj->globalstats_getcounterrorsinprocessedges()<<"). EXITING"<<endl; exit(EXIT_FAILURE); }
			#endif 
			#endif
			// localsourceid0 = 0;
		} // REMOVEME.
		if(localsourceid1 >= PADDEDDESTBUFFER_SIZE){
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->globalstats_counterrorsinprocessedges(1);
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTPROCESSEDGESERRORS
			cout<<"ERROR SEEN @ process_edges:: i: "<<i<<", sourceid: "<<destbuffer[1][i].key<<", localsourceid1: "<<localsourceid1<<", upperlimit: "<<upperlimit<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			if(actsutilityobj->globalstats_getcounterrorsinprocessedges() > 100000){ cout<<"too many errors seen @ process_edges ("<<actsutilityobj->globalstats_getcounterrorsinprocessedges()<<"). EXITING"<<endl; exit(EXIT_FAILURE); }
			#endif 
			#endif
			// localsourceid1 = 0;
		} // REMOVEME.
		if(localsourceid2 >= PADDEDDESTBUFFER_SIZE){
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->globalstats_counterrorsinprocessedges(1);
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTPROCESSEDGESERRORS
			cout<<"ERROR SEEN @ process_edges:: i: "<<i<<", sourceid: "<<destbuffer[2][i].key<<", localsourceid2: "<<localsourceid2<<", upperlimit: "<<upperlimit<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			if(actsutilityobj->globalstats_getcounterrorsinprocessedges() > 100000){ cout<<"too many errors seen @ process_edges ("<<actsutilityobj->globalstats_getcounterrorsinprocessedges()<<"). EXITING"<<endl; exit(EXIT_FAILURE); }
			#endif 
			#endif
			// localsourceid2 = 0;
		} // REMOVEME.
		if(localsourceid3 >= PADDEDDESTBUFFER_SIZE){
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->globalstats_counterrorsinprocessedges(1);
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTPROCESSEDGESERRORS
			cout<<"ERROR SEEN @ process_edges:: i: "<<i<<", sourceid: "<<destbuffer[3][i].key<<", localsourceid3: "<<localsourceid3<<", upperlimit: "<<upperlimit<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			if(actsutilityobj->globalstats_getcounterrorsinprocessedges() > 100000){ cout<<"too many errors seen @ process_edges ("<<actsutilityobj->globalstats_getcounterrorsinprocessedges()<<"). EXITING"<<endl; exit(EXIT_FAILURE); }
			#endif 
			#endif
			// localsourceid3 = 0;
		} // REMOVEME.
		if(localsourceid4 >= PADDEDDESTBUFFER_SIZE){
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->globalstats_counterrorsinprocessedges(1);
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTPROCESSEDGESERRORS
			cout<<"ERROR SEEN @ process_edges:: i: "<<i<<", sourceid: "<<destbuffer[4][i].key<<", localsourceid4: "<<localsourceid4<<", upperlimit: "<<upperlimit<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			if(actsutilityobj->globalstats_getcounterrorsinprocessedges() > 100000){ cout<<"too many errors seen @ process_edges ("<<actsutilityobj->globalstats_getcounterrorsinprocessedges()<<"). EXITING"<<endl; exit(EXIT_FAILURE); }
			#endif 
			#endif
			// localsourceid4 = 0;
		} // REMOVEME.
		if(localsourceid5 >= PADDEDDESTBUFFER_SIZE){
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->globalstats_counterrorsinprocessedges(1);
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTPROCESSEDGESERRORS
			cout<<"ERROR SEEN @ process_edges:: i: "<<i<<", sourceid: "<<destbuffer[5][i].key<<", localsourceid5: "<<localsourceid5<<", upperlimit: "<<upperlimit<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			if(actsutilityobj->globalstats_getcounterrorsinprocessedges() > 100000){ cout<<"too many errors seen @ process_edges ("<<actsutilityobj->globalstats_getcounterrorsinprocessedges()<<"). EXITING"<<endl; exit(EXIT_FAILURE); }
			#endif 
			#endif
			// localsourceid5 = 0;
		} // REMOVEME.
		if(localsourceid6 >= PADDEDDESTBUFFER_SIZE){
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->globalstats_counterrorsinprocessedges(1);
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTPROCESSEDGESERRORS
			cout<<"ERROR SEEN @ process_edges:: i: "<<i<<", sourceid: "<<destbuffer[6][i].key<<", localsourceid6: "<<localsourceid6<<", upperlimit: "<<upperlimit<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			if(actsutilityobj->globalstats_getcounterrorsinprocessedges() > 100000){ cout<<"too many errors seen @ process_edges ("<<actsutilityobj->globalstats_getcounterrorsinprocessedges()<<"). EXITING"<<endl; exit(EXIT_FAILURE); }
			#endif 
			#endif
			// localsourceid6 = 0;
		} // REMOVEME.
		if(localsourceid7 >= PADDEDDESTBUFFER_SIZE){
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->globalstats_counterrorsinprocessedges(1);
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTPROCESSEDGESERRORS
			cout<<"ERROR SEEN @ process_edges:: i: "<<i<<", sourceid: "<<destbuffer[7][i].key<<", localsourceid7: "<<localsourceid7<<", upperlimit: "<<upperlimit<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			if(actsutilityobj->globalstats_getcounterrorsinprocessedges() > 100000){ cout<<"too many errors seen @ process_edges ("<<actsutilityobj->globalstats_getcounterrorsinprocessedges()<<"). EXITING"<<endl; exit(EXIT_FAILURE); }
			#endif 
			#endif
			// localsourceid7 = 0;
		} // REMOVEME.
		
		keyvalue_t vertexupdate0;
		if(localsourceid0 < PADDEDDESTBUFFER_SIZE){
			vertexupdate0.key = edge0.value;
			vertexupdate0.value = processedgefunc(sourcebuffer[0][localsourceid0].value, 1, 1, globalparams.GraphIter, globalparams.GraphAlgo); }
		keyvalue_t vertexupdate1;
		if(localsourceid1 < PADDEDDESTBUFFER_SIZE){
			vertexupdate1.key = edge1.value;
			vertexupdate1.value = processedgefunc(sourcebuffer[1][localsourceid1].value, 1, 1, globalparams.GraphIter, globalparams.GraphAlgo); }
		keyvalue_t vertexupdate2;
		if(localsourceid2 < PADDEDDESTBUFFER_SIZE){
			vertexupdate2.key = edge2.value;
			vertexupdate2.value = processedgefunc(sourcebuffer[2][localsourceid2].value, 1, 1, globalparams.GraphIter, globalparams.GraphAlgo); }
		keyvalue_t vertexupdate3;
		if(localsourceid3 < PADDEDDESTBUFFER_SIZE){
			vertexupdate3.key = edge3.value;
			vertexupdate3.value = processedgefunc(sourcebuffer[3][localsourceid3].value, 1, 1, globalparams.GraphIter, globalparams.GraphAlgo); }
		keyvalue_t vertexupdate4;
		if(localsourceid4 < PADDEDDESTBUFFER_SIZE){
			vertexupdate4.key = edge4.value;
			vertexupdate4.value = processedgefunc(sourcebuffer[4][localsourceid4].value, 1, 1, globalparams.GraphIter, globalparams.GraphAlgo); }
		keyvalue_t vertexupdate5;
		if(localsourceid5 < PADDEDDESTBUFFER_SIZE){
			vertexupdate5.key = edge5.value;
			vertexupdate5.value = processedgefunc(sourcebuffer[5][localsourceid5].value, 1, 1, globalparams.GraphIter, globalparams.GraphAlgo); }
		keyvalue_t vertexupdate6;
		if(localsourceid6 < PADDEDDESTBUFFER_SIZE){
			vertexupdate6.key = edge6.value;
			vertexupdate6.value = processedgefunc(sourcebuffer[6][localsourceid6].value, 1, 1, globalparams.GraphIter, globalparams.GraphAlgo); }
		keyvalue_t vertexupdate7;
		if(localsourceid7 < PADDEDDESTBUFFER_SIZE){
			vertexupdate7.key = edge7.value;
			vertexupdate7.value = processedgefunc(sourcebuffer[7][localsourceid7].value, 1, 1, globalparams.GraphIter, globalparams.GraphAlgo); }
		
		if(localsourceid0 < PADDEDDESTBUFFER_SIZE){ destbuffer[0][i] = vertexupdate0; }
		if(localsourceid1 < PADDEDDESTBUFFER_SIZE){ destbuffer[1][i] = vertexupdate1; }
		if(localsourceid2 < PADDEDDESTBUFFER_SIZE){ destbuffer[2][i] = vertexupdate2; }
		if(localsourceid3 < PADDEDDESTBUFFER_SIZE){ destbuffer[3][i] = vertexupdate3; }
		if(localsourceid4 < PADDEDDESTBUFFER_SIZE){ destbuffer[4][i] = vertexupdate4; }
		if(localsourceid5 < PADDEDDESTBUFFER_SIZE){ destbuffer[5][i] = vertexupdate5; }
		if(localsourceid6 < PADDEDDESTBUFFER_SIZE){ destbuffer[6][i] = vertexupdate6; }
		if(localsourceid7 < PADDEDDESTBUFFER_SIZE){ destbuffer[7][i] = vertexupdate7; }
		
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsprocessed(VECTOR_SIZE);
		if(localsourceid0 < PADDEDDESTBUFFER_SIZE){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); }
		if(localsourceid1 < PADDEDDESTBUFFER_SIZE){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); }
		if(localsourceid2 < PADDEDDESTBUFFER_SIZE){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); }
		if(localsourceid3 < PADDEDDESTBUFFER_SIZE){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); }
		if(localsourceid4 < PADDEDDESTBUFFER_SIZE){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); }
		if(localsourceid5 < PADDEDDESTBUFFER_SIZE){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); }
		if(localsourceid6 < PADDEDDESTBUFFER_SIZE){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); }
		if(localsourceid7 < PADDEDDESTBUFFER_SIZE){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); }
		#endif
	}
	return;
}

void 
	#ifdef SW 
	actslw::
	#endif
process_edges(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t statsbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], vertex_t upperlimit, unsigned int GraphIter, unsigned int GraphAlgo, travstate_t travstate, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE;
	buffer_type chunk_size = getchunksize_kvs(SRCBUFFER_SIZE, travstate, 0);
	
	PROCESSEDGES_LOOP: for(buffer_type i=0; i<chunk_size; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
	#pragma HLS PIPELINE II=2
		keyvalue_t edge0 = destbuffer[0][i];
		keyvalue_t edge1 = destbuffer[1][i];
		keyvalue_t edge2 = destbuffer[2][i];
		keyvalue_t edge3 = destbuffer[3][i];
		keyvalue_t edge4 = destbuffer[4][i];
		keyvalue_t edge5 = destbuffer[5][i];
		keyvalue_t edge6 = destbuffer[6][i];
		keyvalue_t edge7 = destbuffer[7][i];
		
		value_t localsourceid0 = edge0.key - upperlimit;
		value_t localsourceid1 = edge1.key - upperlimit;
		value_t localsourceid2 = edge2.key - upperlimit;
		value_t localsourceid3 = edge3.key - upperlimit;
		value_t localsourceid4 = edge4.key - upperlimit;
		value_t localsourceid5 = edge5.key - upperlimit;
		value_t localsourceid6 = edge6.key - upperlimit;
		value_t localsourceid7 = edge7.key - upperlimit;
		
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"PROCESSEDGE SEEN @ process_edges:: i: "<<i<<", destbuffer[0][i].key: "<<destbuffer[0][i].value<<", destbuffer[0][i].value: "<<destbuffer[0][i].key<<", upperlimit: "<<upperlimit<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
		cout<<"PROCESSEDGE SEEN @ process_edges:: i: "<<i<<", destbuffer[1][i].key: "<<destbuffer[1][i].value<<", destbuffer[1][i].value: "<<destbuffer[1][i].key<<", upperlimit: "<<upperlimit<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
		cout<<"PROCESSEDGE SEEN @ process_edges:: i: "<<i<<", destbuffer[2][i].key: "<<destbuffer[2][i].value<<", destbuffer[2][i].value: "<<destbuffer[2][i].key<<", upperlimit: "<<upperlimit<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
		cout<<"PROCESSEDGE SEEN @ process_edges:: i: "<<i<<", destbuffer[3][i].key: "<<destbuffer[3][i].value<<", destbuffer[3][i].value: "<<destbuffer[3][i].key<<", upperlimit: "<<upperlimit<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
		cout<<"PROCESSEDGE SEEN @ process_edges:: i: "<<i<<", destbuffer[4][i].key: "<<destbuffer[4][i].value<<", destbuffer[4][i].value: "<<destbuffer[4][i].key<<", upperlimit: "<<upperlimit<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
		cout<<"PROCESSEDGE SEEN @ process_edges:: i: "<<i<<", destbuffer[5][i].key: "<<destbuffer[5][i].value<<", destbuffer[5][i].value: "<<destbuffer[5][i].key<<", upperlimit: "<<upperlimit<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
		cout<<"PROCESSEDGE SEEN @ process_edges:: i: "<<i<<", destbuffer[6][i].key: "<<destbuffer[6][i].value<<", destbuffer[6][i].value: "<<destbuffer[6][i].key<<", upperlimit: "<<upperlimit<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
		cout<<"PROCESSEDGE SEEN @ process_edges:: i: "<<i<<", destbuffer[7][i].key: "<<destbuffer[7][i].value<<", destbuffer[7][i].value: "<<destbuffer[7][i].key<<", upperlimit: "<<upperlimit<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
	
		#endif 
		
		if(localsourceid0 >= PADDEDDESTBUFFER_SIZE){
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->globalstats_counterrorsinprocessedges(1);
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTPROCESSEDGESERRORS
			cout<<"ERROR SEEN @ process_edges:: i: "<<i<<", sourceid: "<<destbuffer[0][i].key<<", localsourceid0: "<<localsourceid0<<", upperlimit: "<<upperlimit<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			if(actsutilityobj->globalstats_getcounterrorsinprocessedges() > 100000){ cout<<"too many errors seen @ process_edges ("<<actsutilityobj->globalstats_getcounterrorsinprocessedges()<<"). EXITING"<<endl; exit(EXIT_FAILURE); }
			#endif 
			#endif
			// localsourceid0 = 0;
		} // REMOVEME.
		if(localsourceid1 >= PADDEDDESTBUFFER_SIZE){
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->globalstats_counterrorsinprocessedges(1);
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTPROCESSEDGESERRORS
			cout<<"ERROR SEEN @ process_edges:: i: "<<i<<", sourceid: "<<destbuffer[1][i].key<<", localsourceid1: "<<localsourceid1<<", upperlimit: "<<upperlimit<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			if(actsutilityobj->globalstats_getcounterrorsinprocessedges() > 100000){ cout<<"too many errors seen @ process_edges ("<<actsutilityobj->globalstats_getcounterrorsinprocessedges()<<"). EXITING"<<endl; exit(EXIT_FAILURE); }
			#endif 
			#endif
			// localsourceid1 = 0;
		} // REMOVEME.
		if(localsourceid2 >= PADDEDDESTBUFFER_SIZE){
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->globalstats_counterrorsinprocessedges(1);
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTPROCESSEDGESERRORS
			cout<<"ERROR SEEN @ process_edges:: i: "<<i<<", sourceid: "<<destbuffer[2][i].key<<", localsourceid2: "<<localsourceid2<<", upperlimit: "<<upperlimit<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			if(actsutilityobj->globalstats_getcounterrorsinprocessedges() > 100000){ cout<<"too many errors seen @ process_edges ("<<actsutilityobj->globalstats_getcounterrorsinprocessedges()<<"). EXITING"<<endl; exit(EXIT_FAILURE); }
			#endif 
			#endif
			// localsourceid2 = 0;
		} // REMOVEME.
		if(localsourceid3 >= PADDEDDESTBUFFER_SIZE){
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->globalstats_counterrorsinprocessedges(1);
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTPROCESSEDGESERRORS
			cout<<"ERROR SEEN @ process_edges:: i: "<<i<<", sourceid: "<<destbuffer[3][i].key<<", localsourceid3: "<<localsourceid3<<", upperlimit: "<<upperlimit<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			if(actsutilityobj->globalstats_getcounterrorsinprocessedges() > 100000){ cout<<"too many errors seen @ process_edges ("<<actsutilityobj->globalstats_getcounterrorsinprocessedges()<<"). EXITING"<<endl; exit(EXIT_FAILURE); }
			#endif 
			#endif
			// localsourceid3 = 0;
		} // REMOVEME.
		if(localsourceid4 >= PADDEDDESTBUFFER_SIZE){
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->globalstats_counterrorsinprocessedges(1);
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTPROCESSEDGESERRORS
			cout<<"ERROR SEEN @ process_edges:: i: "<<i<<", sourceid: "<<destbuffer[4][i].key<<", localsourceid4: "<<localsourceid4<<", upperlimit: "<<upperlimit<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			if(actsutilityobj->globalstats_getcounterrorsinprocessedges() > 100000){ cout<<"too many errors seen @ process_edges ("<<actsutilityobj->globalstats_getcounterrorsinprocessedges()<<"). EXITING"<<endl; exit(EXIT_FAILURE); }
			#endif 
			#endif
			// localsourceid4 = 0;
		} // REMOVEME.
		if(localsourceid5 >= PADDEDDESTBUFFER_SIZE){
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->globalstats_counterrorsinprocessedges(1);
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTPROCESSEDGESERRORS
			cout<<"ERROR SEEN @ process_edges:: i: "<<i<<", sourceid: "<<destbuffer[5][i].key<<", localsourceid5: "<<localsourceid5<<", upperlimit: "<<upperlimit<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			if(actsutilityobj->globalstats_getcounterrorsinprocessedges() > 100000){ cout<<"too many errors seen @ process_edges ("<<actsutilityobj->globalstats_getcounterrorsinprocessedges()<<"). EXITING"<<endl; exit(EXIT_FAILURE); }
			#endif 
			#endif
			// localsourceid5 = 0;
		} // REMOVEME.
		if(localsourceid6 >= PADDEDDESTBUFFER_SIZE){
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->globalstats_counterrorsinprocessedges(1);
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTPROCESSEDGESERRORS
			cout<<"ERROR SEEN @ process_edges:: i: "<<i<<", sourceid: "<<destbuffer[6][i].key<<", localsourceid6: "<<localsourceid6<<", upperlimit: "<<upperlimit<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			if(actsutilityobj->globalstats_getcounterrorsinprocessedges() > 100000){ cout<<"too many errors seen @ process_edges ("<<actsutilityobj->globalstats_getcounterrorsinprocessedges()<<"). EXITING"<<endl; exit(EXIT_FAILURE); }
			#endif 
			#endif
			// localsourceid6 = 0;
		} // REMOVEME.
		if(localsourceid7 >= PADDEDDESTBUFFER_SIZE){
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->globalstats_counterrorsinprocessedges(1);
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTPROCESSEDGESERRORS
			cout<<"ERROR SEEN @ process_edges:: i: "<<i<<", sourceid: "<<destbuffer[7][i].key<<", localsourceid7: "<<localsourceid7<<", upperlimit: "<<upperlimit<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			if(actsutilityobj->globalstats_getcounterrorsinprocessedges() > 100000){ cout<<"too many errors seen @ process_edges ("<<actsutilityobj->globalstats_getcounterrorsinprocessedges()<<"). EXITING"<<endl; exit(EXIT_FAILURE); }
			#endif 
			#endif
			// localsourceid7 = 0;
		} // REMOVEME.
		
		keyvalue_t vertexupdate0;
		if(localsourceid0 < PADDEDDESTBUFFER_SIZE){
			vertexupdate0.key = edge0.value;
			vertexupdate0.value = processedgefunc(sourcebuffer[0][localsourceid0].value, 1, 1, globalparams.GraphIter, globalparams.GraphAlgo); }
		keyvalue_t vertexupdate1;
		if(localsourceid1 < PADDEDDESTBUFFER_SIZE){
			vertexupdate1.key = edge1.value;
			vertexupdate1.value = processedgefunc(sourcebuffer[1][localsourceid1].value, 1, 1, globalparams.GraphIter, globalparams.GraphAlgo); }
		keyvalue_t vertexupdate2;
		if(localsourceid2 < PADDEDDESTBUFFER_SIZE){
			vertexupdate2.key = edge2.value;
			vertexupdate2.value = processedgefunc(sourcebuffer[2][localsourceid2].value, 1, 1, globalparams.GraphIter, globalparams.GraphAlgo); }
		keyvalue_t vertexupdate3;
		if(localsourceid3 < PADDEDDESTBUFFER_SIZE){
			vertexupdate3.key = edge3.value;
			vertexupdate3.value = processedgefunc(sourcebuffer[3][localsourceid3].value, 1, 1, globalparams.GraphIter, globalparams.GraphAlgo); }
		keyvalue_t vertexupdate4;
		if(localsourceid4 < PADDEDDESTBUFFER_SIZE){
			vertexupdate4.key = edge4.value;
			vertexupdate4.value = processedgefunc(sourcebuffer[4][localsourceid4].value, 1, 1, globalparams.GraphIter, globalparams.GraphAlgo); }
		keyvalue_t vertexupdate5;
		if(localsourceid5 < PADDEDDESTBUFFER_SIZE){
			vertexupdate5.key = edge5.value;
			vertexupdate5.value = processedgefunc(sourcebuffer[5][localsourceid5].value, 1, 1, globalparams.GraphIter, globalparams.GraphAlgo); }
		keyvalue_t vertexupdate6;
		if(localsourceid6 < PADDEDDESTBUFFER_SIZE){
			vertexupdate6.key = edge6.value;
			vertexupdate6.value = processedgefunc(sourcebuffer[6][localsourceid6].value, 1, 1, globalparams.GraphIter, globalparams.GraphAlgo); }
		keyvalue_t vertexupdate7;
		if(localsourceid7 < PADDEDDESTBUFFER_SIZE){
			vertexupdate7.key = edge7.value;
			vertexupdate7.value = processedgefunc(sourcebuffer[7][localsourceid7].value, 1, 1, globalparams.GraphIter, globalparams.GraphAlgo); }
		
		if(localsourceid0 < PADDEDDESTBUFFER_SIZE){ destbuffer[0][i] = vertexupdate0; }
		if(localsourceid1 < PADDEDDESTBUFFER_SIZE){ destbuffer[1][i] = vertexupdate1; }
		if(localsourceid2 < PADDEDDESTBUFFER_SIZE){ destbuffer[2][i] = vertexupdate2; }
		if(localsourceid3 < PADDEDDESTBUFFER_SIZE){ destbuffer[3][i] = vertexupdate3; }
		if(localsourceid4 < PADDEDDESTBUFFER_SIZE){ destbuffer[4][i] = vertexupdate4; }
		if(localsourceid5 < PADDEDDESTBUFFER_SIZE){ destbuffer[5][i] = vertexupdate5; }
		if(localsourceid6 < PADDEDDESTBUFFER_SIZE){ destbuffer[6][i] = vertexupdate6; }
		if(localsourceid7 < PADDEDDESTBUFFER_SIZE){ destbuffer[7][i] = vertexupdate7; }
		
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsprocessed(VECTOR_SIZE);
		if(localsourceid0 < PADDEDDESTBUFFER_SIZE){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); }
		if(localsourceid1 < PADDEDDESTBUFFER_SIZE){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); }
		if(localsourceid2 < PADDEDDESTBUFFER_SIZE){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); }
		if(localsourceid3 < PADDEDDESTBUFFER_SIZE){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); }
		if(localsourceid4 < PADDEDDESTBUFFER_SIZE){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); }
		if(localsourceid5 < PADDEDDESTBUFFER_SIZE){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); }
		if(localsourceid6 < PADDEDDESTBUFFER_SIZE){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); }
		if(localsourceid7 < PADDEDDESTBUFFER_SIZE){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); }
		#endif
		
		#ifdef EMBEDDEDCOLLECTSTATS
		partition_type p0 = 0;
		if(localsourceid0 < PADDEDDESTBUFFER_SIZE){ p0 = getglobalpartition(vertexupdate0, 0, 0, globalparams.batch_range_pow); } // CRITICAL FIXME. upper not always 0
		partition_type p1 = 0;
		if(localsourceid1 < PADDEDDESTBUFFER_SIZE){ p1 = getglobalpartition(vertexupdate1, 0, 0, globalparams.batch_range_pow); } // CRITICAL FIXME. upper not always 0
		partition_type p2 = 0;
		if(localsourceid2 < PADDEDDESTBUFFER_SIZE){ p2 = getglobalpartition(vertexupdate2, 0, 0, globalparams.batch_range_pow); } // CRITICAL FIXME. upper not always 0
		partition_type p3 = 0;
		if(localsourceid3 < PADDEDDESTBUFFER_SIZE){ p3 = getglobalpartition(vertexupdate3, 0, 0, globalparams.batch_range_pow); } // CRITICAL FIXME. upper not always 0
		partition_type p4 = 0;
		if(localsourceid4 < PADDEDDESTBUFFER_SIZE){ p4 = getglobalpartition(vertexupdate4, 0, 0, globalparams.batch_range_pow); } // CRITICAL FIXME. upper not always 0
		partition_type p5 = 0;
		if(localsourceid5 < PADDEDDESTBUFFER_SIZE){ p5 = getglobalpartition(vertexupdate5, 0, 0, globalparams.batch_range_pow); } // CRITICAL FIXME. upper not always 0
		partition_type p6 = 0;
		if(localsourceid6 < PADDEDDESTBUFFER_SIZE){ p6 = getglobalpartition(vertexupdate6, 0, 0, globalparams.batch_range_pow); } // CRITICAL FIXME. upper not always 0
		partition_type p7 = 0;
		if(localsourceid7 < PADDEDDESTBUFFER_SIZE){ p7 = getglobalpartition(vertexupdate7, 0, 0, globalparams.batch_range_pow); } // CRITICAL FIXME. upper not always 0
		
		if(localsourceid0 < PADDEDDESTBUFFER_SIZE){ statsbuffer[0][p0].value += 1; }
		if(localsourceid1 < PADDEDDESTBUFFER_SIZE){ statsbuffer[1][p1].value += 1; }
		if(localsourceid2 < PADDEDDESTBUFFER_SIZE){ statsbuffer[2][p2].value += 1; }
		if(localsourceid3 < PADDEDDESTBUFFER_SIZE){ statsbuffer[3][p3].value += 1; }
		if(localsourceid4 < PADDEDDESTBUFFER_SIZE){ statsbuffer[4][p4].value += 1; }
		if(localsourceid5 < PADDEDDESTBUFFER_SIZE){ statsbuffer[5][p5].value += 1; }
		if(localsourceid6 < PADDEDDESTBUFFER_SIZE){ statsbuffer[6][p6].value += 1; }
		if(localsourceid7 < PADDEDDESTBUFFER_SIZE){ statsbuffer[7][p7].value += 1; }
		#endif
	}
	return;
}

void 
	#ifdef SW 
	actslw::
	#endif
process_edges(bool_type enable, value_t sourcedata, keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], unsigned int GraphIter, unsigned int GraphAlgo, travstate_t travstate, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE;
	buffer_type chunk_size = getchunksize_kvs(SRCBUFFER_SIZE, travstate, 0);
	
	PROCESSEDGES_LOOP: for(buffer_type i=0; i<chunk_size; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
	#pragma HLS PIPELINE II=2
		keyvalue_t edge0 = destbuffer[0][i];
		keyvalue_t edge1 = destbuffer[1][i];
		keyvalue_t edge2 = destbuffer[2][i];
		keyvalue_t edge3 = destbuffer[3][i];
		keyvalue_t edge4 = destbuffer[4][i];
		keyvalue_t edge5 = destbuffer[5][i];
		keyvalue_t edge6 = destbuffer[6][i];
		keyvalue_t edge7 = destbuffer[7][i];
		
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"PROCESSEDGE SEEN @ process_edges:: i: "<<i<<", destbuffer[0][i].key: "<<destbuffer[0][i].key<<", destbuffer[0][i].value: "<<destbuffer[0][i].value<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
		cout<<"PROCESSEDGE SEEN @ process_edges:: i: "<<i<<", destbuffer[1][i].key: "<<destbuffer[1][i].key<<", destbuffer[1][i].value: "<<destbuffer[1][i].value<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
		cout<<"PROCESSEDGE SEEN @ process_edges:: i: "<<i<<", destbuffer[2][i].key: "<<destbuffer[2][i].key<<", destbuffer[2][i].value: "<<destbuffer[2][i].value<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
		cout<<"PROCESSEDGE SEEN @ process_edges:: i: "<<i<<", destbuffer[3][i].key: "<<destbuffer[3][i].key<<", destbuffer[3][i].value: "<<destbuffer[3][i].value<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
		cout<<"PROCESSEDGE SEEN @ process_edges:: i: "<<i<<", destbuffer[4][i].key: "<<destbuffer[4][i].key<<", destbuffer[4][i].value: "<<destbuffer[4][i].value<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
		cout<<"PROCESSEDGE SEEN @ process_edges:: i: "<<i<<", destbuffer[5][i].key: "<<destbuffer[5][i].key<<", destbuffer[5][i].value: "<<destbuffer[5][i].value<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
		cout<<"PROCESSEDGE SEEN @ process_edges:: i: "<<i<<", destbuffer[6][i].key: "<<destbuffer[6][i].key<<", destbuffer[6][i].value: "<<destbuffer[6][i].value<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
		cout<<"PROCESSEDGE SEEN @ process_edges:: i: "<<i<<", destbuffer[7][i].key: "<<destbuffer[7][i].key<<", destbuffer[7][i].value: "<<destbuffer[7][i].value<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
	
		#endif
		
		keyvalue_t vertexupdate0;
		vertexupdate0.key = edge0.value;
		vertexupdate0.value = processedgefunc(sourcedata, 1, 1, globalparams.GraphIter, globalparams.GraphAlgo);
		keyvalue_t vertexupdate1;
		vertexupdate1.key = edge1.value;
		vertexupdate1.value = processedgefunc(sourcedata, 1, 1, globalparams.GraphIter, globalparams.GraphAlgo);
		keyvalue_t vertexupdate2;
		vertexupdate2.key = edge2.value;
		vertexupdate2.value = processedgefunc(sourcedata, 1, 1, globalparams.GraphIter, globalparams.GraphAlgo);
		keyvalue_t vertexupdate3;
		vertexupdate3.key = edge3.value;
		vertexupdate3.value = processedgefunc(sourcedata, 1, 1, globalparams.GraphIter, globalparams.GraphAlgo);
		keyvalue_t vertexupdate4;
		vertexupdate4.key = edge4.value;
		vertexupdate4.value = processedgefunc(sourcedata, 1, 1, globalparams.GraphIter, globalparams.GraphAlgo);
		keyvalue_t vertexupdate5;
		vertexupdate5.key = edge5.value;
		vertexupdate5.value = processedgefunc(sourcedata, 1, 1, globalparams.GraphIter, globalparams.GraphAlgo);
		keyvalue_t vertexupdate6;
		vertexupdate6.key = edge6.value;
		vertexupdate6.value = processedgefunc(sourcedata, 1, 1, globalparams.GraphIter, globalparams.GraphAlgo);
		keyvalue_t vertexupdate7;
		vertexupdate7.key = edge7.value;
		vertexupdate7.value = processedgefunc(sourcedata, 1, 1, globalparams.GraphIter, globalparams.GraphAlgo);
		
		destbuffer[0][i] = vertexupdate0; // edge0;
		destbuffer[1][i] = vertexupdate1; // edge1;
		destbuffer[2][i] = vertexupdate2; // edge2;
		destbuffer[3][i] = vertexupdate3; // edge3;
		destbuffer[4][i] = vertexupdate4; // edge4;
		destbuffer[5][i] = vertexupdate5; // edge5;
		destbuffer[6][i] = vertexupdate6; // edge6;
		destbuffer[7][i] = vertexupdate7; // edge7;
	
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsprocessed(VECTOR_SIZE);
		actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1);
		actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1);
		actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1);
		actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1);
		actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1);
		actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1);
		actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1);
		actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1);
		#endif
	}
	return;
}

// in-memory partition functions (1)
void 
	#ifdef SW 
	actslw::
	#endif
combineSetof1stoSetof2_I0(bool_type enable, keyvalue_t buffer_setof1M0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof1M1[PADDEDDESTBUFFER_SIZE], 
															keyvalue_t buffer_setof2R0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2R1[PADDEDDESTBUFFER_SIZE], 
																skeyvalue_t localcapsuleM[NUM_PARTITIONS], skeyvalue_t localcapsuleN[NUM_PARTITIONS], skeyvalue_t localcapsuleR[NUM_PARTITIONS], globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE / NUM_PARTITIONS;
	buffer_type index=0, begin=0, end=0;
	keyvalue_t NullKV; NullKV.key = INVALIDDATA; NullKV.value = INVALIDDATA;
	
	resetkeyandvalues(localcapsuleR, NUM_PARTITIONS, 0);
	
	EXECUTE_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		buffer_type counter = 0;
		
		EXECUTE_LOOP1B: for(step_type i=0; i<2; i++){
			if(i==0){ begin = localcapsuleM[p].key; end = localcapsuleM[p].key + localcapsuleM[p].value; }
			else { begin = localcapsuleN[p].key; end = localcapsuleN[p].key + localcapsuleN[p].value; }
		
			EXECUTE_LOOP1C: for(buffer_type k=begin; k<end; k++){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("combineSetof1stoSetof2_I0 1", index, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("combineSetof1stoSetof2_I0 2", k, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
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
	actsutilityobj->scankeyvalues("combineSetof1stoSetof2_I0::buffer_setof2M", (keyvalue_t *)buffer_setof1M, localcapsuleM, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof1stoSetof2_I0::buffer_setof2", (keyvalue_t *)buffer_setof2, localcapsuleR, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif
combineSetof1stoSetof2_I1(bool_type enable, keyvalue_t buffer_setof1M0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof1M1[PADDEDDESTBUFFER_SIZE], 
															keyvalue_t buffer_setof2R0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2R1[PADDEDDESTBUFFER_SIZE], 
																skeyvalue_t localcapsuleM[NUM_PARTITIONS], skeyvalue_t localcapsuleN[NUM_PARTITIONS], skeyvalue_t localcapsuleR[NUM_PARTITIONS], globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE / NUM_PARTITIONS;
	buffer_type index=0, begin=0, end=0;
	keyvalue_t NullKV; NullKV.key = INVALIDDATA; NullKV.value = INVALIDDATA;
	
	resetkeyandvalues(localcapsuleR, NUM_PARTITIONS, 0);
	
	EXECUTE_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		buffer_type counter = 0;
		
		EXECUTE_LOOP1B: for(step_type i=0; i<2; i++){
			if(i==0){ begin = localcapsuleM[p].key; end = localcapsuleM[p].key + localcapsuleM[p].value; }
			else { begin = localcapsuleN[p].key; end = localcapsuleN[p].key + localcapsuleN[p].value; }
		
			EXECUTE_LOOP1C: for(buffer_type k=begin; k<end; k++){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("combineSetof1stoSetof2_I1 1", index, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("combineSetof1stoSetof2_I1 2", k, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
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
	actsutilityobj->scankeyvalues("combineSetof1stoSetof2_I1::buffer_setof2M", (keyvalue_t *)buffer_setof1M, localcapsuleM, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof1stoSetof2_I1::buffer_setof2", (keyvalue_t *)buffer_setof2, localcapsuleR, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif
combineSetof1stoSetof2_I2(bool_type enable, keyvalue_t buffer_setof1M0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof1M1[PADDEDDESTBUFFER_SIZE], 
															keyvalue_t buffer_setof2R0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2R1[PADDEDDESTBUFFER_SIZE], 
																skeyvalue_t localcapsuleM[NUM_PARTITIONS], skeyvalue_t localcapsuleN[NUM_PARTITIONS], skeyvalue_t localcapsuleR[NUM_PARTITIONS], globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE / NUM_PARTITIONS;
	buffer_type index=0, begin=0, end=0;
	keyvalue_t NullKV; NullKV.key = INVALIDDATA; NullKV.value = INVALIDDATA;
	
	resetkeyandvalues(localcapsuleR, NUM_PARTITIONS, 0);
	
	EXECUTE_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		buffer_type counter = 0;
		
		EXECUTE_LOOP1B: for(step_type i=0; i<2; i++){
			if(i==0){ begin = localcapsuleM[p].key; end = localcapsuleM[p].key + localcapsuleM[p].value; }
			else { begin = localcapsuleN[p].key; end = localcapsuleN[p].key + localcapsuleN[p].value; }
		
			EXECUTE_LOOP1C: for(buffer_type k=begin; k<end; k++){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("combineSetof1stoSetof2_I2 1", index, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("combineSetof1stoSetof2_I2 2", k, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
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
	actsutilityobj->scankeyvalues("combineSetof1stoSetof2_I2::buffer_setof2M", (keyvalue_t *)buffer_setof1M, localcapsuleM, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof1stoSetof2_I2::buffer_setof2", (keyvalue_t *)buffer_setof2, localcapsuleR, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif
combineSetof1stoSetof2_I3(bool_type enable, keyvalue_t buffer_setof1M0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof1M1[PADDEDDESTBUFFER_SIZE], 
															keyvalue_t buffer_setof2R0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2R1[PADDEDDESTBUFFER_SIZE], 
																skeyvalue_t localcapsuleM[NUM_PARTITIONS], skeyvalue_t localcapsuleN[NUM_PARTITIONS], skeyvalue_t localcapsuleR[NUM_PARTITIONS], globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE / NUM_PARTITIONS;
	buffer_type index=0, begin=0, end=0;
	keyvalue_t NullKV; NullKV.key = INVALIDDATA; NullKV.value = INVALIDDATA;
	
	resetkeyandvalues(localcapsuleR, NUM_PARTITIONS, 0);
	
	EXECUTE_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		buffer_type counter = 0;
		
		EXECUTE_LOOP1B: for(step_type i=0; i<2; i++){
			if(i==0){ begin = localcapsuleM[p].key; end = localcapsuleM[p].key + localcapsuleM[p].value; }
			else { begin = localcapsuleN[p].key; end = localcapsuleN[p].key + localcapsuleN[p].value; }
		
			EXECUTE_LOOP1C: for(buffer_type k=begin; k<end; k++){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("combineSetof1stoSetof2_I3 1", index, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("combineSetof1stoSetof2_I3 2", k, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
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
	actsutilityobj->scankeyvalues("combineSetof1stoSetof2_I3::buffer_setof2M", (keyvalue_t *)buffer_setof1M, localcapsuleM, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof1stoSetof2_I3::buffer_setof2", (keyvalue_t *)buffer_setof2, localcapsuleR, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif
combineSetof2stoSetof4_I0(bool_type enable, keyvalue_t buffer_setof2M0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2M1[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2M2[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2M3[PADDEDDESTBUFFER_SIZE], 
															keyvalue_t buffer_setof4R0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4R1[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4R2[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4R3[PADDEDDESTBUFFER_SIZE], 
																skeyvalue_t localcapsuleM[NUM_PARTITIONS], skeyvalue_t localcapsuleN[NUM_PARTITIONS], skeyvalue_t localcapsuleR[NUM_PARTITIONS], globalparams_t globalparams){			
	if(enable == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE / NUM_PARTITIONS;
	buffer_type index=0, begin=0, end=0;
	keyvalue_t NullKV; NullKV.key = INVALIDDATA; NullKV.value = INVALIDDATA;
	
	resetkeyandvalues(localcapsuleR, NUM_PARTITIONS, 0);
	
	EXECUTE_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		buffer_type counter = 0;
		
		EXECUTE_LOOP1B: for(step_type i=0; i<2; i++){
			
			if(i==0){ begin = localcapsuleM[p].key / 2; end = (localcapsuleM[p].key + localcapsuleM[p].value) / 2; }
			else { begin = localcapsuleN[p].key / 2; end = (localcapsuleN[p].key + localcapsuleN[p].value) / 2; }
			
			EXECUTE_LOOP1C: for(buffer_type k=begin; k<end; k++){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("combineSetof2stoSetof4_I0 1", index, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("combineSetof2stoSetof4_I0 2", k, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
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
	actsutilityobj->scankeyvalues("combineSetof2stoSetof4_I0::buffer_setof2M", (keyvalue_t *)buffer_setof2M, localcapsuleM, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof2stoSetof4_I0::buffer_setof2N", (keyvalue_t *)buffer_setof2N, localcapsuleN, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof2stoSetof4_I0::buffer_setof4", (keyvalue_t *)buffer_setof4, localcapsuleR, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif
combineSetof2stoSetof4_I1(bool_type enable, keyvalue_t buffer_setof2M0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2M1[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2M2[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2M3[PADDEDDESTBUFFER_SIZE], 
															keyvalue_t buffer_setof4R0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4R1[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4R2[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4R3[PADDEDDESTBUFFER_SIZE], 
																skeyvalue_t localcapsuleM[NUM_PARTITIONS], skeyvalue_t localcapsuleN[NUM_PARTITIONS], skeyvalue_t localcapsuleR[NUM_PARTITIONS], globalparams_t globalparams){			
	if(enable == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE / NUM_PARTITIONS;
	buffer_type index=0, begin=0, end=0;
	keyvalue_t NullKV; NullKV.key = INVALIDDATA; NullKV.value = INVALIDDATA;
	
	resetkeyandvalues(localcapsuleR, NUM_PARTITIONS, 0);
	
	EXECUTE_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		buffer_type counter = 0;
		
		EXECUTE_LOOP1B: for(step_type i=0; i<2; i++){
			
			if(i==0){ begin = localcapsuleM[p].key / 2; end = (localcapsuleM[p].key + localcapsuleM[p].value) / 2; }
			else { begin = localcapsuleN[p].key / 2; end = (localcapsuleN[p].key + localcapsuleN[p].value) / 2; }
			
			EXECUTE_LOOP1C: for(buffer_type k=begin; k<end; k++){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("combineSetof2stoSetof4_I1 1", index, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("combineSetof2stoSetof4_I1 2", k, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
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
	actsutilityobj->scankeyvalues("combineSetof2stoSetof4_I1::buffer_setof2M", (keyvalue_t *)buffer_setof2M, localcapsuleM, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof2stoSetof4_I1::buffer_setof2N", (keyvalue_t *)buffer_setof2N, localcapsuleN, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof2stoSetof4_I1::buffer_setof4", (keyvalue_t *)buffer_setof4, localcapsuleR, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif
combineSetof4stoSetof8_I0(bool_type enable, keyvalue_t buffer_setof4M0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4M1[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4M2[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4M3[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4M4[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4M5[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4M6[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4M7[PADDEDDESTBUFFER_SIZE],
															keyvalue_t buffer_setof8R0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof8R1[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof8R2[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof8R3[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof8R4[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof8R5[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof8R6[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof8R7[PADDEDDESTBUFFER_SIZE],
																skeyvalue_t localcapsuleM[NUM_PARTITIONS], skeyvalue_t localcapsuleN[NUM_PARTITIONS], skeyvalue_t localcapsuleR[NUM_PARTITIONS], globalparams_t globalparams){				
	if(enable == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE / NUM_PARTITIONS;
	buffer_type index=0, begin=0, end=0;
	keyvalue_t NullKV; NullKV.key = INVALIDDATA; NullKV.value = INVALIDDATA;
	
	resetkeyandvalues(localcapsuleR, NUM_PARTITIONS, 0);
	
	EXECUTE_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		buffer_type counter = 0;
		
		EXECUTE_LOOP1B: for(step_type i=0; i<2; i++){
			if(i==0){ begin = localcapsuleM[p].key / 4; end = (localcapsuleM[p].key + localcapsuleM[p].value) / 4; }
			else { begin = localcapsuleN[p].key / 4; end = (localcapsuleN[p].key + localcapsuleN[p].value) / 4; }
			
			EXECUTE_LOOP1C: for(buffer_type k=begin; k<end; k++){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("combineSetof4stoSetof8_I0 1", index, PADDEDDESTBUFFER_SIZE, p, counter, NAp);
				actsutilityobj->checkoutofbounds("combineSetof4stoSetof8_I0 2", k, PADDEDDESTBUFFER_SIZE, p, counter, NAp);
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
	actsutilityobj->scankeyvalues("combineSetof4stoSetof8_I0::buffer_setof2M", (keyvalue_t *)buffer_setof8, localcapsuleR, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	return;
}

void 
	#ifdef SW 
	actslw::
	#endif
combineSetof1stoSetof2s(bool_type enable, keyvalue_t buffer_setof1[8][PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2[8][PADDEDDESTBUFFER_SIZE], skeyvalue_t templocalcapsule_so1[8][NUM_PARTITIONS], skeyvalue_t templocalcapsule_so2[4][NUM_PARTITIONS], globalparams_t globalparams){
	// #pragma HLS INLINE
	// 1s->2s
	combineSetof1stoSetof2_I0(enable, buffer_setof1[0], buffer_setof1[1], buffer_setof2[0], buffer_setof2[1], templocalcapsule_so1[0], templocalcapsule_so1[1], templocalcapsule_so2[0], globalparams);
	combineSetof1stoSetof2_I1(enable, buffer_setof1[2], buffer_setof1[3], buffer_setof2[2], buffer_setof2[3], templocalcapsule_so1[2], templocalcapsule_so1[3], templocalcapsule_so2[1], globalparams);
	combineSetof1stoSetof2_I2(enable, buffer_setof1[4], buffer_setof1[5], buffer_setof2[4], buffer_setof2[5], templocalcapsule_so1[4], templocalcapsule_so1[5], templocalcapsule_so2[2], globalparams);
	combineSetof1stoSetof2_I3(enable, buffer_setof1[6], buffer_setof1[7], buffer_setof2[6], buffer_setof2[7], templocalcapsule_so1[6], templocalcapsule_so1[7], templocalcapsule_so2[3], globalparams);
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif
combineSetof2stoSetof4s(bool_type enable, keyvalue_t buffer_setof2[8][PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4[8][PADDEDDESTBUFFER_SIZE], skeyvalue_t templocalcapsule_so2[4][NUM_PARTITIONS], skeyvalue_t templocalcapsule_so4[2][NUM_PARTITIONS], globalparams_t globalparams){
	// #pragma HLS INLINE
	// 2s->4s
	combineSetof2stoSetof4_I0(enable, buffer_setof2[0], buffer_setof2[1], buffer_setof2[2], buffer_setof2[3], buffer_setof4[0], buffer_setof4[1], buffer_setof4[2], buffer_setof4[3], templocalcapsule_so2[0], templocalcapsule_so2[1], templocalcapsule_so4[0], globalparams);
	combineSetof2stoSetof4_I1(enable, buffer_setof2[4], buffer_setof2[5], buffer_setof2[6], buffer_setof2[7], buffer_setof4[4], buffer_setof4[5], buffer_setof4[6], buffer_setof4[7], templocalcapsule_so2[2], templocalcapsule_so2[3], templocalcapsule_so4[1], globalparams);
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif
combineSetof4stoSetof8s(bool_type enable, keyvalue_t buffer_setof4[8][PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof8[8][PADDEDDESTBUFFER_SIZE], skeyvalue_t templocalcapsule_so4[2][NUM_PARTITIONS], skeyvalue_t templocalcapsule_so8[NUM_PARTITIONS], globalparams_t globalparams){
	// #pragma HLS INLINE
	// 4s->8s
	combineSetof4stoSetof8_I0(enable, buffer_setof4[0], buffer_setof4[1], buffer_setof4[2], buffer_setof4[3], buffer_setof4[4], buffer_setof4[5], buffer_setof4[6], buffer_setof4[7], 
												buffer_setof8[0], buffer_setof8[1], buffer_setof8[2], buffer_setof8[3], buffer_setof8[4], buffer_setof8[5], buffer_setof8[6], buffer_setof8[7], 
													templocalcapsule_so4[0], templocalcapsule_so4[1], templocalcapsule_so8, globalparams);
	return;
}

// in-memory partition functions (2)
void 
	#ifdef SW 
	actslw::
	#endif
runpipeline(bool_type enable, keyvalue_t bufferA[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t buffer1capsule[VECTOR_SIZE][NUM_PARTITIONS], 
				keyvalue_t bufferB[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t bufferBcapsule[4][NUM_PARTITIONS], 
					keyvalue_t bufferC[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t bufferCcapsule[2][NUM_PARTITIONS],
						keyvalue_t bufferD[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t bufferDcapsule[NUM_PARTITIONS],
							unsigned int currentLOP, unsigned int upperlimit, buffer_type cutoff, batch_type shiftcount, globalparams_t globalparams){		
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE;
	if(enable == OFF){ return; } // NEWCHANGE. CHECKWITHVHLS.
	
	keyvalue_t kvA0[4];
	keyvalue_t kvA2[4];
	keyvalue_t kvA4[4];
	keyvalue_t kvA6[4];
	#pragma HLS ARRAY_PARTITION variable=kvA0 complete
	#pragma HLS ARRAY_PARTITION variable=kvA2 complete
	#pragma HLS ARRAY_PARTITION variable=kvA4 complete
	#pragma HLS ARRAY_PARTITION variable=kvA6 complete
	
	keyvalue_t kvB0[4];
	keyvalue_t kvB1[4];
	keyvalue_t kvB2[4];
	keyvalue_t kvB3[4];
	keyvalue_t kvB4[4];
	keyvalue_t kvB5[4];
	keyvalue_t kvB6[4];
	keyvalue_t kvB7[4];
	#pragma HLS ARRAY_PARTITION variable=kvB0 complete
	#pragma HLS ARRAY_PARTITION variable=kvB1 complete
	#pragma HLS ARRAY_PARTITION variable=kvB2 complete
	#pragma HLS ARRAY_PARTITION variable=kvB3 complete
	#pragma HLS ARRAY_PARTITION variable=kvB4 complete
	#pragma HLS ARRAY_PARTITION variable=kvB5 complete
	#pragma HLS ARRAY_PARTITION variable=kvB6 complete
	#pragma HLS ARRAY_PARTITION variable=kvB7 complete
	
	keyvalue_t kvC0[4];
	keyvalue_t kvC1[4];
	keyvalue_t kvC2[4];
	keyvalue_t kvC3[4];
	keyvalue_t kvC4[4];
	keyvalue_t kvC5[4];
	keyvalue_t kvC6[4];
	keyvalue_t kvC7[4];
	#pragma HLS ARRAY_PARTITION variable=kvC0 complete
	#pragma HLS ARRAY_PARTITION variable=kvC1 complete
	#pragma HLS ARRAY_PARTITION variable=kvC2 complete
	#pragma HLS ARRAY_PARTITION variable=kvC3 complete
	#pragma HLS ARRAY_PARTITION variable=kvC4 complete
	#pragma HLS ARRAY_PARTITION variable=kvC5 complete
	#pragma HLS ARRAY_PARTITION variable=kvC6 complete
	#pragma HLS ARRAY_PARTITION variable=kvC7 complete
	
	keyvalue_t _kvC0[4];
	keyvalue_t _kvC1[4];
	keyvalue_t _kvC2[4];
	keyvalue_t _kvC3[4];
	keyvalue_t _kvC4[4];
	keyvalue_t _kvC5[4];
	keyvalue_t _kvC6[4];
	keyvalue_t _kvC7[4];
	#pragma HLS ARRAY_PARTITION variable=_kvC0 complete
	#pragma HLS ARRAY_PARTITION variable=_kvC1 complete
	#pragma HLS ARRAY_PARTITION variable=_kvC2 complete
	#pragma HLS ARRAY_PARTITION variable=_kvC3 complete
	#pragma HLS ARRAY_PARTITION variable=_kvC4 complete
	#pragma HLS ARRAY_PARTITION variable=_kvC5 complete
	#pragma HLS ARRAY_PARTITION variable=_kvC6 complete
	#pragma HLS ARRAY_PARTITION variable=_kvC7 complete
	
	bool_type enablebufferA = ON;
	bool_type enablebufferB = ON;
	bool_type enablebufferC = ON;
	bool_type enablebufferD = ON;
	#ifdef OPTMZ
	if(shiftcount >= 1){ enablebufferB = ON; } else { enablebufferB = OFF; }
	if(shiftcount >= 2){ enablebufferC = ON; } else { enablebufferC = OFF; }
	if(shiftcount >= 3){ enablebufferD = ON; } else { enablebufferD = OFF; }
	#endif 
	
	#ifdef MAXPERFORMANCE
	#pragma HLS ARRAY_PARTITION variable=bufferDcapsule complete
	#endif 

	#ifdef _DEBUGMODE_CHECKS2 
	unsigned int before_pcountso1[NUM_PARTITIONS];
	unsigned int before_pcountso2[NUM_PARTITIONS];
	unsigned int before_pcountso4[NUM_PARTITIONS];
	unsigned int before_pcountso8[NUM_PARTITIONS]; 
	actsutilityobj->intrarunpipelinecheck_shifting(enablebufferD, "intrarunpipelinecheck_shifting[before]: bufferA,bufferB,bufferC,bufferD", 
						bufferA, buffer1capsule, bufferB, bufferBcapsule, bufferC, bufferCcapsule, bufferD, bufferDcapsule,
						before_pcountso1, before_pcountso2, before_pcountso4, before_pcountso8,
						currentLOP, upperlimit, globalparams.batch_range_pow);
	#endif
	#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_RUNKERNELPRINTS)
	cout<<"runpipeline: before_pcountso1: "<<actsutilityobj->countvalues("", before_pcountso1, NUM_PARTITIONS)<<endl;
	cout<<"runpipeline: before_pcountso2: "<<actsutilityobj->countvalues("", before_pcountso2, NUM_PARTITIONS)<<endl;
	cout<<"runpipeline: before_pcountso4: "<<actsutilityobj->countvalues("", before_pcountso4, NUM_PARTITIONS)<<endl;
	cout<<"runpipeline: before_pcountso8: "<<actsutilityobj->countvalues("", before_pcountso8, NUM_PARTITIONS)<<endl;
	#endif 
	#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_RUNKERNELPRINTS)
	actsutilityobj->printvalues("runpipeline: printing before_pcountso1.", before_pcountso1, NUM_PARTITIONS);
	actsutilityobj->printvalues("runpipeline: printing before_pcountso2.", before_pcountso2, NUM_PARTITIONS);
	actsutilityobj->printvalues("runpipeline: printing before_pcountso4.", before_pcountso4, NUM_PARTITIONS);
	actsutilityobj->printvalues("runpipeline: printing before_pcountso8.", before_pcountso8, NUM_PARTITIONS);
	#endif
	
	// shift partition function
	bufferDcapsule[0].key = 0;
	for(partition_type p=0; p<NUM_PARTITIONS; p++){
		if(p<NUM_PARTITIONS-1){ bufferDcapsule[p+1].key = bufferDcapsule[p].key + allignhigher_KV(((bufferCcapsule[0][p].value + bufferCcapsule[1][p].value) * 4)); } // FIXME. CHECKWITHVHLS.
		bufferDcapsule[p].value = 0;
		
		bufferCcapsule[0][p].key = (bufferBcapsule[0][p].key + bufferBcapsule[1][p].key) / 2;
		bufferCcapsule[1][p].key = (bufferBcapsule[2][p].key + bufferBcapsule[3][p].key) / 2;
		bufferCcapsule[0][p].value = 0;
		bufferCcapsule[1][p].value = 0;
		
		bufferBcapsule[0][p].key = (buffer1capsule[0][p].key + buffer1capsule[1][p].key) / 2;
		bufferBcapsule[1][p].key = (buffer1capsule[2][p].key + buffer1capsule[3][p].key) / 2;
		bufferBcapsule[2][p].key = (buffer1capsule[4][p].key + buffer1capsule[5][p].key) / 2;
		bufferBcapsule[3][p].key = (buffer1capsule[6][p].key + buffer1capsule[7][p].key) / 2;
		bufferBcapsule[0][p].value = 0;
		bufferBcapsule[1][p].value = 0;
		bufferBcapsule[2][p].value = 0;
		bufferBcapsule[3][p].value = 0;
	}
	
	#ifdef _DEBUGMODE_CHECKS2
	for(partition_type p=0; p<NUM_PARTITIONS; p++){
		actsutilityobj->checkfordivisibleby(enablebufferD, "bufferDcapsule[p].key", bufferDcapsule[p].key, 1);
		
		actsutilityobj->checkfordivisibleby(enablebufferC, "bufferCcapsule[0][p].key", bufferCcapsule[0][p].key, 1);
		actsutilityobj->checkfordivisibleby(enablebufferC, "bufferCcapsule[1][p].key", bufferCcapsule[1][p].key, 1);
		
		actsutilityobj->checkfordivisibleby(enablebufferB, "bufferBcapsule[0][p].key", bufferBcapsule[0][p].key, 2);
		actsutilityobj->checkfordivisibleby(enablebufferB, "bufferBcapsule[1][p].key", bufferBcapsule[1][p].key, 2);
		actsutilityobj->checkfordivisibleby(enablebufferB, "bufferBcapsule[2][p].key", bufferBcapsule[2][p].key, 2);
		actsutilityobj->checkfordivisibleby(enablebufferB, "bufferBcapsule[3][p].key", bufferBcapsule[3][p].key, 2);
		
		actsutilityobj->checkfordivisibleby(enablebufferA, "buffer1capsule[0][p].key", buffer1capsule[0][p].key, 4);
		actsutilityobj->checkfordivisibleby(enablebufferA, "buffer1capsule[1][p].key", buffer1capsule[1][p].key, 4);
		actsutilityobj->checkfordivisibleby(enablebufferA, "buffer1capsule[2][p].key", buffer1capsule[2][p].key, 4);
		actsutilityobj->checkfordivisibleby(enablebufferA, "buffer1capsule[3][p].key", buffer1capsule[3][p].key, 4);
		actsutilityobj->checkfordivisibleby(enablebufferA, "buffer1capsule[4][p].key", buffer1capsule[4][p].key, 4);
		actsutilityobj->checkfordivisibleby(enablebufferA, "buffer1capsule[5][p].key", buffer1capsule[5][p].key, 4);
		actsutilityobj->checkfordivisibleby(enablebufferA, "buffer1capsule[6][p].key", buffer1capsule[6][p].key, 4);
		actsutilityobj->checkfordivisibleby(enablebufferA, "buffer1capsule[7][p].key", buffer1capsule[7][p].key, 4);
	}
	for(unsigned int v=0; v<0; v++){ actsutilityobj->printkeyvalues("[before] runpipeline.bufferA[0]", bufferA[v], 16); }
	for(unsigned int v=0; v<0; v++){ actsutilityobj->printkeyvalues("[before] runpipeline.bufferBcapsule[v]", (keyvalue_t *)bufferBcapsule[v], NUM_PARTITIONS); }
	#endif
	#ifdef _DEBUGMODE_CHECKS2
	keyvalue_t CHK[4]; 
	keyvalue_t CHKK[8]; 
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int v=0; v<4; v++){ actsutilityobj->printkeyvalues("runpipeline.bufferBcapsule[v]", (keyvalue_t *)bufferBcapsule[v], NUM_PARTITIONS); }
	for(unsigned int v=0; v<2; v++){ actsutilityobj->printkeyvalues("runpipeline.bufferCcapsule[v]", (keyvalue_t *)bufferCcapsule[v], NUM_PARTITIONS); }
	for(unsigned int v=0; v<1; v++){ actsutilityobj->printkeyvalues("runpipeline.bufferDcapsule[v]", (keyvalue_t *)bufferDcapsule, NUM_PARTITIONS); }
	#endif 
	
	keyvalue_t dummykv;
	dummykv.key = ((NUM_PARTITIONS-1) << (globalparams.batch_range_pow - (NUM_PARTITIONS_POW * currentLOP))) + upperlimit;
	dummykv.value = INVALIDDATA;

	unsigned int n=0;
	RUNPIPELINE_LOOP1: for(n=0; n<2; n++){
		RUNPIPELINE_LOOP1B: for(buffer_type k=0; k<SRCBUFFER_SIZE; k+=4){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
		#pragma HLS PIPELINE II=4
		
			#ifdef _DEBUGMODE_KERNELPRINTS
			if(k % 16 == 0){ cout<<"k: "<<k<<endl; }
			#endif
			
			if(k < cutoff){
				kvA0[0] = bufferA[0+n][k];
				kvA0[1] = bufferA[0+n][k+1];
				kvA0[2] = bufferA[0+n][k+2];
				kvA0[3] = bufferA[0+n][k+3];
				
				kvA2[0] = bufferA[2+n][k]; 
				kvA2[1] = bufferA[2+n][k+1];
				kvA2[2] = bufferA[2+n][k+2];
				kvA2[3] = bufferA[2+n][k+3];
				
				kvA4[0] = bufferA[4+n][k];
				kvA4[1] = bufferA[4+n][k+1];
				kvA4[2] = bufferA[4+n][k+2];
				kvA4[3] = bufferA[4+n][k+3];
				
				kvA6[0] = bufferA[6+n][k];
				kvA6[1] = bufferA[6+n][k+1];
				kvA6[2] = bufferA[6+n][k+2];
				kvA6[3] = bufferA[6+n][k+3];
			} else {
				kvA0[0] = dummykv;
				kvA0[1] = dummykv;
				kvA0[2] = dummykv;
				kvA0[3] = dummykv;
				
				kvA2[0] = dummykv; 
				kvA2[1] = dummykv;
				kvA2[2] = dummykv;
				kvA2[3] = dummykv;
				
				kvA4[0] = dummykv;
				kvA4[1] = dummykv;
				kvA4[2] = dummykv;
				kvA4[3] = dummykv;
				
				kvA6[0] = dummykv;
				kvA6[1] = dummykv;
				kvA6[2] = dummykv;
				kvA6[3] = dummykv;
			}
		
			#ifdef _DEBUGMODE_KERNELPRINTS
			for(unsigned int m=0; m<4; m++){ cout<<"kvA0["<<m<<"].key: "<<kvA0[m].key<<", kvA0["<<m<<"].value: "<<kvA0[m].value<<endl; }
			cout<<endl;
			for(unsigned int m=0; m<4; m++){ cout<<"kvA2["<<m<<"].key: "<<kvA2[m].key<<", kvA2["<<m<<"].value: "<<kvA2[m].value<<endl; }
			cout<<endl;
			for(unsigned int m=0; m<4; m++){ cout<<"kvA4["<<m<<"].key: "<<kvA4[m].key<<", kvA4["<<m<<"].value: "<<kvA4[m].value<<endl; }
			cout<<endl;
			for(unsigned int m=0; m<4; m++){ cout<<"kvA6["<<m<<"].key: "<<kvA6[m].key<<", kvA6["<<m<<"].value: "<<kvA6[m].value<<endl; }
			cout<<endl;
			#endif
			
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkn(enablebufferA, "kvA0", kvA0, currentLOP, upperlimit, globalparams.batch_range_pow, 4);
			actsutilityobj->checkn(enablebufferA, "kvA2", kvA2, currentLOP, upperlimit, globalparams.batch_range_pow, 4);
			actsutilityobj->checkn(enablebufferA, "kvA4", kvA4, currentLOP, upperlimit, globalparams.batch_range_pow, 4);
			actsutilityobj->checkn(enablebufferA, "kvA6", kvA6, currentLOP, upperlimit, globalparams.batch_range_pow, 4);
			#endif
			
			partition_type pA0 = getpartition(enablebufferA, kvA0[0], currentLOP, upperlimit, globalparams.batch_range_pow);
			partition_type pA2 = getpartition(enablebufferA, kvA2[0], currentLOP, upperlimit, globalparams.batch_range_pow);
			partition_type pA4 = getpartition(enablebufferA, kvA4[0], currentLOP, upperlimit, globalparams.batch_range_pow);
			partition_type pA6 = getpartition(enablebufferA, kvA6[0], currentLOP, upperlimit, globalparams.batch_range_pow);
			
			/// LOADING FROM AND INTO B
			buffer_type posB0 = bufferBcapsule[0][pA0].key + bufferBcapsule[0][pA0].value;
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkfordivisibleby(enablebufferB, "posB0", posB0, 2);
			actsutilityobj->checkoutofbounds(enablebufferB, "posB0", posB0, SRCBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferB, "posB0", posB0, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			#endif 
			kvB0[0] = bufferB[0][posB0]; kvB1[0] = bufferB[1][posB0];
			kvB0[1] = bufferB[0][posB0+1]; kvB1[1] = bufferB[1][posB0+1];
			bufferB[0][posB0] = kvA0[0]; bufferB[1][posB0] = kvA0[1]; 
			bufferB[0][posB0+1] = kvA0[2]; bufferB[1][posB0+1] = kvA0[3];
			bufferBcapsule[0][pA0].value += 2;
		
			buffer_type posB2 = bufferBcapsule[1][pA2].key + bufferBcapsule[1][pA2].value; 
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkfordivisibleby(enablebufferB, "posB2", posB2, 2);
			actsutilityobj->checkoutofbounds(enablebufferB, "posB2", posB2, SRCBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferB, "posB2", posB2, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			#endif 
			kvB2[0] = bufferB[2][posB2]; kvB3[0] = bufferB[3][posB2];
			kvB2[1] = bufferB[2][posB2+1]; kvB3[1] = bufferB[3][posB2+1];
			bufferB[2][posB2] = kvA2[0]; bufferB[3][posB2] = kvA2[1];
			bufferB[2][posB2+1] = kvA2[2]; bufferB[3][posB2+1] = kvA2[3];
			bufferBcapsule[1][pA2].value += 2;
			
			buffer_type posB4 = bufferBcapsule[2][pA4].key + bufferBcapsule[2][pA4].value; 
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkfordivisibleby(enablebufferB, "posB4", posB4, 2);
			actsutilityobj->checkoutofbounds(enablebufferB, "posB4", posB4, SRCBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferB, "posB4", posB4, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			#endif 
			kvB4[0] = bufferB[4][posB4]; kvB5[0] = bufferB[5][posB4];
			kvB4[1] = bufferB[4][posB4+1]; kvB5[1] = bufferB[5][posB4+1];
			bufferB[4][posB4] = kvA4[0]; bufferB[5][posB4] = kvA4[1];
			bufferB[4][posB4+1] = kvA4[2]; bufferB[5][posB4+1] = kvA4[3];
			bufferBcapsule[2][pA4].value += 2;
			
			buffer_type posB6 = bufferBcapsule[3][pA6].key + bufferBcapsule[3][pA6].value; 
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkfordivisibleby(enablebufferB, "posB6", posB6, 2);
			actsutilityobj->checkoutofbounds(enablebufferB, "posB6", posB6, SRCBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferB, "posB6", posB6, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			#endif 
			kvB6[0] = bufferB[6][posB6]; kvB7[0] = bufferB[7][posB6];
			kvB6[1] = bufferB[6][posB6+1]; kvB7[1] = bufferB[7][posB6+1];
			bufferB[6][posB6] = kvA6[0]; bufferB[7][posB6] = kvA6[1];
			bufferB[6][posB6+1] = kvA6[2]; bufferB[7][posB6+1] = kvA6[3];
			bufferBcapsule[3][pA6].value += 2;
			
			partition_type pB0 = getpartition(enablebufferB, kvB0[0], currentLOP, upperlimit, globalparams.batch_range_pow);
			partition_type pB2 = getpartition(enablebufferB, kvB2[0], currentLOP, upperlimit, globalparams.batch_range_pow);
			partition_type pB4 = getpartition(enablebufferB, kvB4[0], currentLOP, upperlimit, globalparams.batch_range_pow);
			partition_type pB6 = getpartition(enablebufferB, kvB6[0], currentLOP, upperlimit, globalparams.batch_range_pow);
			
			#ifdef _DEBUGMODE_CHECKS2
			CHK[0] = kvB0[0]; CHK[1] = kvB1[0]; CHK[2] = kvB0[1]; CHK[3] = kvB1[1];
			actsutilityobj->checkn(enablebufferB, "kvB0,kvB1", CHK, currentLOP, upperlimit, globalparams.batch_range_pow, 4);
			CHK[0] = kvB2[0]; CHK[1] = kvB3[0]; CHK[2] = kvB2[1]; CHK[3] = kvB3[1];
			actsutilityobj->checkn(enablebufferB, "kvB2,kvB3", CHK, currentLOP, upperlimit, globalparams.batch_range_pow, 4);
			CHK[0] = kvB4[0]; CHK[1] = kvB5[0]; CHK[2] = kvB4[1]; CHK[3] = kvB5[1];
			actsutilityobj->checkn(enablebufferB, "kvB4,kvB5", CHK, currentLOP, upperlimit, globalparams.batch_range_pow, 4);
			CHK[0] = kvB6[0]; CHK[1] = kvB7[0]; CHK[2] = kvB6[1]; CHK[3] = kvB7[1];
			actsutilityobj->checkn(enablebufferB, "kvB6,kvB7", CHK, currentLOP, upperlimit, globalparams.batch_range_pow, 4);
			#endif
			
			/// LOADING FROM AND INTO C
			buffer_type posC0 = bufferCcapsule[0][pB0].key + bufferCcapsule[0][pB0].value;
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds(enablebufferC, "posC0", posC0, SRCBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferC, "posC0", posC0, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			#endif 
			kvC0[0] = bufferC[0][posC0]; kvC1[0] = bufferC[1][posC0]; kvC2[0] = bufferC[2][posC0]; kvC3[0] = bufferC[3][posC0];
			bufferC[0][posC0] = kvB0[0]; bufferC[1][posC0] = kvB1[0]; bufferC[2][posC0] = kvB0[1]; bufferC[3][posC0] = kvB1[1];
			bufferCcapsule[0][pB0].value += 1;
			
			buffer_type _posC0 = bufferCcapsule[0][pB2].key + bufferCcapsule[0][pB2].value;
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds(enablebufferC, "_posC0", _posC0, SRCBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferC, "_posC0", _posC0, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			#endif 
			_kvC0[0] = bufferC[0][_posC0]; _kvC1[0] = bufferC[1][_posC0]; _kvC2[0] = bufferC[2][_posC0]; _kvC3[0] = bufferC[3][_posC0];
			bufferC[0][_posC0] = kvB2[0]; bufferC[1][_posC0] = kvB3[0]; bufferC[2][_posC0] = kvB2[1]; bufferC[3][_posC0] = kvB3[1];
			bufferCcapsule[0][pB2].value += 1;
			
			buffer_type posC4 = bufferCcapsule[1][pB4].key + bufferCcapsule[1][pB4].value;
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds(enablebufferC, "posC4", posC4, SRCBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferC, "posC4", posC4, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			#endif 
			kvC4[0] = bufferC[4][posC4]; kvC5[0] = bufferC[5][posC4]; kvC6[0] = bufferC[6][posC4]; kvC7[0] = bufferC[7][posC4]; 
			bufferC[4][posC4] = kvB4[0]; bufferC[5][posC4] = kvB5[0]; bufferC[6][posC4] = kvB4[1]; bufferC[7][posC4] = kvB5[1];
			bufferCcapsule[1][pB4].value += 1;
			
			buffer_type _posC4 = bufferCcapsule[1][pB6].key + bufferCcapsule[1][pB6].value;
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds(enablebufferC, "_posC4", _posC4, SRCBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferC, "_posC4", _posC4, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			#endif 
			_kvC4[0] = bufferC[4][_posC4]; _kvC5[0] = bufferC[5][_posC4]; _kvC6[0] = bufferC[6][_posC4]; _kvC7[0] = bufferC[7][_posC4];
			bufferC[4][_posC4] = kvB6[0]; bufferC[5][_posC4] = kvB7[0]; bufferC[6][_posC4] = kvB6[1]; bufferC[7][_posC4] = kvB7[1];
			bufferCcapsule[1][pB6].value += 1;
			
			partition_type pC0 = getpartition(enablebufferC, kvC0[0], currentLOP, upperlimit, globalparams.batch_range_pow);
			partition_type _pC0 = getpartition(enablebufferC, _kvC0[0], currentLOP, upperlimit, globalparams.batch_range_pow);
			partition_type pC4 = getpartition(enablebufferC, kvC4[0], currentLOP, upperlimit, globalparams.batch_range_pow);
			partition_type _pC4 = getpartition(enablebufferC, _kvC4[0], currentLOP, upperlimit, globalparams.batch_range_pow);
			
			#ifdef _DEBUGMODE_CHECKS2
			CHK[0] = kvC0[0]; CHK[1] = kvC1[0]; CHK[2] = kvC2[0]; CHK[3] = kvC3[0];
			actsutilityobj->checkn(enablebufferC, "kvC0[0],kvC1[0],kvC2[0],kvC3[0]", CHK, currentLOP, upperlimit, globalparams.batch_range_pow, 4);
			
			CHK[0] = _kvC0[0]; CHK[1] = _kvC1[0]; CHK[2] = _kvC2[0]; CHK[3] = _kvC3[0];
			actsutilityobj->checkn(enablebufferC, "_kvC0[0],_kvC1[0],_kvC2[0],_kvC3[0]", CHK, currentLOP, upperlimit, globalparams.batch_range_pow, 4);
			
			CHK[0] = kvC4[0]; CHK[1] = kvC5[0]; CHK[2] = kvC6[0]; CHK[3] = kvC7[0];
			actsutilityobj->checkn(enablebufferC, "kvC4[0],kvC5[0],kvC6[0],kvC7[0]", CHK, currentLOP, upperlimit, globalparams.batch_range_pow, 4);
			
			CHK[0] = _kvC4[0]; CHK[1] = _kvC5[0]; CHK[2] = _kvC6[0]; CHK[3] = _kvC7[0];
			actsutilityobj->checkn(enablebufferC, "_kvC4[0],_kvC5[0],_kvC6[0],_kvC7[0]", CHK, currentLOP, upperlimit, globalparams.batch_range_pow, 4);
			#endif
			
			/// LOADING FROM AND INTO D
			buffer_type posD0 = bufferDcapsule[pC0].key + bufferDcapsule[pC0].value;
			unsigned int yoffset0 = posD0 / 8;
			unsigned int xoffset0 = posD0 % 8;
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkfordivisibleby(enablebufferD, "posD0", posD0, 4);
			actsutilityobj->checkoutofbounds(enablebufferD, "posD0", posD0, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferD, "yoffset0", yoffset0, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferD, "xoffset0+4", xoffset0+4, VECTOR_SIZE+1, NAp, NAp, NAp);
			// if((kvC0[0].value == INVALIDDATA) && (kvC1[0].value == INVALIDDATA) && (kvC2[0].value == INVALIDDATA) && (kvC3[0].value == INVALIDDATA)){ cout<<"--- HEY! ALL INVALIDS AT D!. kvC0"<<endl; }//exit(EXIT_FAILURE); } // FIXME.
			// if((kvC0[0].value == INVALIDDATA) && !((kvC0[0].value == INVALIDDATA) && (kvC1[0].value == INVALIDDATA) && (kvC2[0].value == INVALIDDATA) && (kvC3[0].value == INVALIDDATA))){ cout<<"--- FIRST INVALID != ALL INVALIDS AT D!. kvC0"<<endl; exit(EXIT_FAILURE); }
			#endif 
			if(xoffset0 == 0){
				bufferD[0][yoffset0] = kvC0[0]; bufferD[1][yoffset0] = kvC1[0]; bufferD[2][yoffset0] = kvC2[0]; bufferD[3][yoffset0] = kvC3[0]; 
			} else {
				bufferD[4][yoffset0] = kvC0[0]; bufferD[5][yoffset0] = kvC1[0]; bufferD[6][yoffset0] = kvC2[0]; bufferD[7][yoffset0] = kvC3[0]; 
			}
			// bufferDcapsule[pC0].value += 4; 
			// if(kvC0[0].value != INVALIDDATA){ bufferDcapsule[pC0].value += 4; } // CHECKWITHVHLS.
			if(!((kvC0[0].value == INVALIDDATA) && (kvC1[0].value == INVALIDDATA) && (kvC2[0].value == INVALIDDATA) && (kvC3[0].value == INVALIDDATA))){ bufferDcapsule[pC0].value += 4; } // CHECKWITHVHLS.
			
			buffer_type _posD0 = bufferDcapsule[_pC0].key + bufferDcapsule[_pC0].value;
			unsigned int yoffset1 = _posD0 / 8;
			unsigned int xoffset1 = _posD0 % 8;
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkfordivisibleby(enablebufferD, "_posD0", _posD0, 4);
			actsutilityobj->checkoutofbounds(enablebufferD, "_posD0", _posD0, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferD, "yoffset1", yoffset1, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferD, "xoffset1+4", xoffset1+4, VECTOR_SIZE+1, NAp, NAp, NAp);
			// if((_kvC0[0].value == INVALIDDATA) && (_kvC1[0].value == INVALIDDATA) && (_kvC2[0].value == INVALIDDATA) && (_kvC3[0].value == INVALIDDATA)){ cout<<"--- HEY! ALL INVALIDS AT D!. _kvC0"<<endl; }// exit(EXIT_FAILURE); } // FIXME.
			// if((_kvC0[0].value == INVALIDDATA) && !((_kvC0[0].value == INVALIDDATA) && (_kvC1[0].value == INVALIDDATA) && (_kvC2[0].value == INVALIDDATA) && (_kvC3[0].value == INVALIDDATA))){ cout<<"--- FIRST INVALID != ALL INVALIDS AT D!. _kvC0"<<endl; exit(EXIT_FAILURE); }
			#endif 
			if(xoffset1 == 0){
				bufferD[0][yoffset1] = _kvC0[0]; bufferD[1][yoffset1] = _kvC1[0]; bufferD[2][yoffset1] = _kvC2[0]; bufferD[3][yoffset1] = _kvC3[0]; 
			} else {
				bufferD[4][yoffset1] = _kvC0[0]; bufferD[5][yoffset1] = _kvC1[0]; bufferD[6][yoffset1] = _kvC2[0]; bufferD[7][yoffset1] = _kvC3[0]; 
			}
			// bufferDcapsule[_pC0].value += 4;
			// if(_kvC0[0].value != INVALIDDATA){ bufferDcapsule[_pC0].value += 4; } // CHECKWITHVHLS.
			if(!((_kvC0[0].value == INVALIDDATA) && (_kvC1[0].value == INVALIDDATA) && (_kvC2[0].value == INVALIDDATA) && (_kvC3[0].value == INVALIDDATA))){ bufferDcapsule[_pC0].value += 4; } // CHECKWITHVHLS.
			
			buffer_type __posD0 = bufferDcapsule[pC4].key + bufferDcapsule[pC4].value;
			unsigned int yoffset2 = __posD0 / 8;
			unsigned int xoffset2 = __posD0 % 8;
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkfordivisibleby(enablebufferD, "__posD0", __posD0, 4);
			actsutilityobj->checkoutofbounds(enablebufferD, "__posD0", __posD0, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferD, "yoffset2", yoffset2, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferD, "xoffset2+4", xoffset2+4, VECTOR_SIZE+1, NAp, NAp, NAp);
			// if((kvC4[0].value == INVALIDDATA) && (kvC5[0].value == INVALIDDATA) && (kvC6[0].value == INVALIDDATA) && (kvC7[0].value == INVALIDDATA)){ cout<<"--- HEY! ALL INVALIDS AT D!. kvC4"<<endl; }// exit(EXIT_FAILURE); } // FIXME.
			// if((kvC4[0].value == INVALIDDATA) && !((kvC4[0].value == INVALIDDATA) && (kvC5[0].value == INVALIDDATA) && (kvC6[0].value == INVALIDDATA) && (kvC7[0].value == INVALIDDATA))){ cout<<"--- FIRST INVALID != ALL INVALIDS AT D!. kvC4"<<endl; exit(EXIT_FAILURE); }
			#endif 
			if(xoffset2 == 0){
				bufferD[0][yoffset2] = kvC4[0]; bufferD[1][yoffset2] = kvC5[0]; bufferD[2][yoffset2] = kvC6[0]; bufferD[3][yoffset2] = kvC7[0]; 
			} else {
				bufferD[4][yoffset2] = kvC4[0]; bufferD[5][yoffset2] = kvC5[0]; bufferD[6][yoffset2] = kvC6[0]; bufferD[7][yoffset2] = kvC7[0]; 
			}
			// bufferDcapsule[pC4].value += 4; 
			// if(kvC4[0].value != INVALIDDATA){ bufferDcapsule[pC4].value += 4; } // CHECKWITHVHLS.
			if(!((kvC4[0].value == INVALIDDATA) && (kvC5[0].value == INVALIDDATA) && (kvC6[0].value == INVALIDDATA) && (kvC7[0].value == INVALIDDATA))){ bufferDcapsule[pC4].value += 4; } // CHECKWITHVHLS.
			
			buffer_type ___posD0 = bufferDcapsule[_pC4].key + bufferDcapsule[_pC4].value;
			unsigned int yoffset3 = ___posD0 / 8;
			unsigned int xoffset3 = ___posD0 % 8;
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkfordivisibleby(enablebufferD, "___posD0", ___posD0, 4);
			actsutilityobj->checkoutofbounds(enablebufferD, "___posD0", ___posD0, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferD, "yoffset3", yoffset3, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferD, "xoffset3+4", xoffset3+4, VECTOR_SIZE+1, NAp, NAp, NAp);
			// if((_kvC4[0].value == INVALIDDATA) && (_kvC5[0].value == INVALIDDATA) && (_kvC6[0].value == INVALIDDATA) && (_kvC7[0].value == INVALIDDATA)){ cout<<"--- HEY! ALL INVALIDS AT D!. _kvC4"<<endl; }//exit(EXIT_FAILURE); } // FIXME.
			// if((_kvC4[0].value == INVALIDDATA) && !((_kvC4[0].value == INVALIDDATA) && (_kvC5[0].value == INVALIDDATA) && (_kvC6[0].value == INVALIDDATA) && (_kvC7[0].value == INVALIDDATA))){ cout<<"--- FIRST INVALID != ALL INVALIDS AT D!. _kvC4"<<endl; exit(EXIT_FAILURE); }
			#endif 
			if(xoffset3 == 0){
				bufferD[0][yoffset3] = _kvC4[0]; bufferD[1][yoffset3] = _kvC5[0]; bufferD[2][yoffset3] = _kvC6[0]; bufferD[3][yoffset3] = _kvC7[0]; 
			} else {
				bufferD[4][yoffset3] = _kvC4[0]; bufferD[5][yoffset3] = _kvC5[0]; bufferD[6][yoffset3] = _kvC6[0]; bufferD[7][yoffset3] = _kvC7[0]; 
			}
			// bufferDcapsule[_pC4].value += 4;
			// if((_kvC4[0].value != INVALIDDATA)){ bufferDcapsule[_pC4].value += 4; } // CHECKWITHVHLS.
			if(!((_kvC4[0].value == INVALIDDATA) && (_kvC5[0].value == INVALIDDATA) && (_kvC6[0].value == INVALIDDATA) && (_kvC7[0].value == INVALIDDATA))){ bufferDcapsule[_pC4].value += 4; } // CHECKWITHVHLS.
		}
	}
	for(partition_type p=0; p<NUM_PARTITIONS; p++){ // CHECKWITHVHLS
		keyvalue_t dummykv;
		dummykv.key = (p << (globalparams.batch_range_pow - (NUM_PARTITIONS_POW * currentLOP))) + upperlimit;
		dummykv.value = INVALIDDATA;
		if(bufferDcapsule[p].value % 8 == 4){
			unsigned int yoffset = (bufferDcapsule[p].key + bufferDcapsule[p].value) / VECTOR_SIZE;
			bufferD[4][yoffset] = dummykv; bufferD[5][yoffset] = dummykv; bufferD[6][yoffset] = dummykv; bufferD[7][yoffset] = dummykv; 
			bufferDcapsule[p].value += 4;
		}
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkfordivisibleby(enablebufferD, "bufferDcapsule[p].value", bufferDcapsule[p].value, 8);
		#endif
	}
	
	#if defined(_DEBUGMODE_CHECKS2) && not defined(OPTMZ) // not fair for OPTMZ ('random values present from prevs')
	actsutilityobj->checkprofile(enablebufferB, "runpipeline::checkprofile::bufferB", bufferB, SRCBUFFER_SIZE, currentLOP, upperlimit, globalparams.batch_range_pow, 4, SRCBUFFER_SIZE*VECTOR_SIZE); 
	actsutilityobj->checkprofile(enablebufferC, "runpipeline::checkprofile::bufferC", bufferC, SRCBUFFER_SIZE, currentLOP, upperlimit, globalparams.batch_range_pow, 4, SRCBUFFER_SIZE*VECTOR_SIZE); 
	actsutilityobj->checkprofile(enablebufferD, "runpipeline::checkprofile::bufferD", bufferD, SRCBUFFER_SIZE, currentLOP, upperlimit, globalparams.batch_range_pow, 4, SRCBUFFER_SIZE*VECTOR_SIZE); 
	#endif 
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkbufferprofile(enablebufferD, "runpipeline::checkbufferprofile::bufferD", bufferD, (keyvalue_t *)bufferDcapsule, currentLOP, upperlimit, globalparams.batch_range_pow); 
	#endif
	
	#ifdef _DEBUGMODE_CHECKS2
	unsigned int after_pcountso1[NUM_PARTITIONS];
	unsigned int after_pcountso2[NUM_PARTITIONS];
	unsigned int after_pcountso4[NUM_PARTITIONS];
	unsigned int after_pcountso8[NUM_PARTITIONS];
	actsutilityobj->intrarunpipelinecheck_shifting(enablebufferD, "intrarunpipelinecheck_shifting[after]: bufferA,bufferB,bufferC,bufferD", 
						bufferA, buffer1capsule, bufferB, bufferBcapsule, bufferC, bufferCcapsule, bufferD, bufferDcapsule,
						after_pcountso1, after_pcountso2, after_pcountso4, after_pcountso8,
						currentLOP, upperlimit, globalparams.batch_range_pow);
	#endif
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->hcheckforequal(enablebufferD, "runpipeline: checking if before_pcountso1 == after_pcountso2", before_pcountso1, after_pcountso2, NUM_PARTITIONS, NAp, NAp, NAp);
	actsutilityobj->hcheckforequal(enablebufferD, "runpipeline: checking if before_pcountso2 == after_pcountso4", before_pcountso2, after_pcountso4, NUM_PARTITIONS, NAp, NAp, NAp);
	actsutilityobj->hcheckforequal(enablebufferD, "runpipeline: checking if before_pcountso4 == after_pcountso8", before_pcountso4, after_pcountso8, NUM_PARTITIONS, NAp, NAp, NAp);
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"intra-runpipeline shift check passed. "<<endl;
	#endif 
	#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_RUNKERNELPRINTS)
	cout<<"runpipeline: after_pcountso1: "<<actsutilityobj->countvalues("", after_pcountso1, NUM_PARTITIONS)<<endl;
	cout<<"runpipeline: after_pcountso2: "<<actsutilityobj->countvalues("", after_pcountso2, NUM_PARTITIONS)<<endl;
	cout<<"runpipeline: after_pcountso4: "<<actsutilityobj->countvalues("", after_pcountso4, NUM_PARTITIONS)<<endl;
	cout<<"runpipeline: after_pcountso8: "<<actsutilityobj->countvalues("", after_pcountso8, NUM_PARTITIONS)<<endl;
	#endif 
	#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_RUNKERNELPRINTS)
	actsutilityobj->printvalues("runpipeline: printing after_pcountso1.", after_pcountso1, NUM_PARTITIONS);
	actsutilityobj->printvalues("runpipeline: printing after_pcountso2.", after_pcountso2, NUM_PARTITIONS);
	actsutilityobj->printvalues("runpipeline: printing after_pcountso4.", after_pcountso4, NUM_PARTITIONS);
	actsutilityobj->printvalues("runpipeline: printing after_pcountso8.", after_pcountso8, NUM_PARTITIONS);
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printkeyvalues("+++[viewing] runpipeline.bufferA", (keyvalue_t *)&bufferA[0], 4);
	actsutilityobj->printkeyvalues("+++[viewing] runpipeline.bufferB", (keyvalue_t *)&bufferB[0], 4);
	actsutilityobj->printkeyvalues("+++[viewing] runpipeline.bufferC", (keyvalue_t *)&bufferC[0], 4);
	actsutilityobj->printkeyvalues("+++[viewing] runpipeline.bufferD", (keyvalue_t *)&bufferD[0], 4);
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printkeyvalues("+++[after] runpipeline.buffer1capsule[0]", (keyvalue_t *)buffer1capsule[0], NUM_PARTITIONS);
	actsutilityobj->printkeyvalues("+++[after] runpipeline.buffer1capsule[1]", (keyvalue_t *)buffer1capsule[1], NUM_PARTITIONS);
	actsutilityobj->printkeyvalues("+++[after] runpipeline.buffer1capsule[2]", (keyvalue_t *)buffer1capsule[2], NUM_PARTITIONS);
	actsutilityobj->printkeyvalues("+++[after] runpipeline.buffer1capsule[3]", (keyvalue_t *)buffer1capsule[3], NUM_PARTITIONS);
	actsutilityobj->printkeyvalues("+++[after] runpipeline.buffer1capsule[4]", (keyvalue_t *)buffer1capsule[4], NUM_PARTITIONS);
	actsutilityobj->printkeyvalues("+++[after] runpipeline.buffer1capsule[5]", (keyvalue_t *)buffer1capsule[5], NUM_PARTITIONS);
	actsutilityobj->printkeyvalues("+++[after] runpipeline.buffer1capsule[6]", (keyvalue_t *)buffer1capsule[6], NUM_PARTITIONS);
	actsutilityobj->printkeyvalues("+++[after] runpipeline.buffer1capsule[7]", (keyvalue_t *)buffer1capsule[7], NUM_PARTITIONS);
	actsutilityobj->printkeyvalues("+++[after] runpipeline.bufferBcapsule[0]", (keyvalue_t *)bufferBcapsule[0], NUM_PARTITIONS);
	actsutilityobj->printkeyvalues("+++[after] runpipeline.bufferBcapsule[1]", (keyvalue_t *)bufferBcapsule[1], NUM_PARTITIONS);
	actsutilityobj->printkeyvalues("+++[after] runpipeline.bufferBcapsule[2]", (keyvalue_t *)bufferBcapsule[2], NUM_PARTITIONS);
	actsutilityobj->printkeyvalues("+++[after] runpipeline.bufferBcapsule[3]", (keyvalue_t *)bufferBcapsule[3], NUM_PARTITIONS);
	actsutilityobj->printkeyvalues("+++[after] runpipeline.bufferCcapsule[0]", (keyvalue_t *)bufferCcapsule[0], NUM_PARTITIONS);
	actsutilityobj->printkeyvalues("+++[after] runpipeline.bufferCcapsule[1]", (keyvalue_t *)bufferCcapsule[1], NUM_PARTITIONS);
	actsutilityobj->printkeyvalues("+++[after] runpipeline.bufferDcapsule", (keyvalue_t *)bufferDcapsule, NUM_PARTITIONS);
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	partition_type dummy[NUM_PARTITIONS];
	actsutilityobj->printprofileso1(enablebufferA, "bufferA", bufferA, buffer1capsule, currentLOP, upperlimit, globalparams.batch_range_pow, dummy);
	actsutilityobj->printprofileso2(enablebufferB, "bufferB", bufferB, bufferBcapsule, currentLOP, upperlimit, globalparams.batch_range_pow, dummy);
	actsutilityobj->printprofileso4(enablebufferC, "bufferC", bufferC, bufferCcapsule, currentLOP, upperlimit, globalparams.batch_range_pow, dummy);
	actsutilityobj->printkeyvalues("+++[after] runpipeline.bufferDcapsule", (keyvalue_t *)bufferDcapsule, NUM_PARTITIONS);
	#endif 	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printprofile(enablebufferA, "bufferA", (keyvalue_t *)&bufferA[0], PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, currentLOP, upperlimit, globalparams.batch_range_pow);
	actsutilityobj->printprofile(enablebufferB, "bufferB", (keyvalue_t *)&bufferB[0], PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, currentLOP, upperlimit, globalparams.batch_range_pow);
	actsutilityobj->printprofile(enablebufferC, "bufferC", (keyvalue_t *)&bufferC[0], PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, currentLOP, upperlimit, globalparams.batch_range_pow);
	actsutilityobj->printprofile(enablebufferD, "bufferD", (keyvalue_t *)&bufferD[0], PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, currentLOP, upperlimit, globalparams.batch_range_pow);
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(buffer_type i=0; i<PADDEDDESTBUFFER_SIZE; i++){
		for(vector_type v=0; v<VECTOR_SIZE; v++){ CHKK[v] = bufferD[v][i]; }
		actsutilityobj->checkn(ON, "bufferD[v][i]", CHKK, currentLOP, upperlimit, globalparams.batch_range_pow, 8);
	}
	
	actsutilityobj->getvaluecountexcept("bufferA", (keyvalue_t *)bufferA, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, INVALIDDATA);
	actsutilityobj->getvaluecountexcept("bufferB", (keyvalue_t *)bufferB, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, INVALIDDATA);
	actsutilityobj->getvaluecountexcept("bufferC", (keyvalue_t *)bufferC, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, INVALIDDATA);
	actsutilityobj->getvaluecountexcept("bufferD", (keyvalue_t *)bufferD, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, INVALIDDATA);
	
	for(unsigned int v=0; v<4; v++){ actsutilityobj->printkeyvalues("+++[after] runpipeline.bufferBcapsule[v]", (keyvalue_t *)bufferBcapsule[v], NUM_PARTITIONS); }
	for(unsigned int v=0; v<2; v++){ actsutilityobj->printkeyvalues("+++[after] runpipeline.bufferCcapsule[v]", (keyvalue_t *)bufferCcapsule[v], NUM_PARTITIONS); }
	for(unsigned int v=0; v<1; v++){ actsutilityobj->printkeyvalues("+++[after] runpipeline.bufferDcapsule[v]", (keyvalue_t *)bufferDcapsule, NUM_PARTITIONS); }
	#endif
	return;
}

// collect stats phase 
void 
	#ifdef SW 
	actslw::
	#endif
collectstats(
		bool_type enable, // NEWCHANGE. CHECKWITHVHLS.
		uint512_dt * kvdram,
		keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer_setof1[8][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so1[8][NUM_PARTITIONS],
		keyvalue_t buffer_setof2[8][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so2[4][NUM_PARTITIONS],
		keyvalue_t buffer_setof4[8][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so4[2][NUM_PARTITIONS],
		keyvalue_t buffer_setof8[8][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so8[NUM_PARTITIONS],
		keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ], 
		config_t config,
		globalparams_t globalparams,
		sweepparams_t sweepparams,
		travstate_t ctravstate){
	#pragma HLS INLINE
	analysis_type analysis_collectstatsloop = KVDATA_BATCHSIZE_KVS / SRCBUFFER_SIZE;
	if(enable == OFF){ return; } // NEWCHANGE. CHECKWITHVHLS.

	unsigned int enable_retreivestats = OFF;
	unsigned int enable_generatestats = ON;
	#ifdef EMBEDDEDCOLLECTSTATS
	if(config.enablecollectglobalstats == ON){
		if(sweepparams.currentLOP >= 3){ enable_retreivestats = OFF; enable_generatestats = ON; }
		else { enable_retreivestats = ON; enable_generatestats = OFF; ctravstate.begin_kvs = 0; ctravstate.end_kvs = 0; }
	} else { enable_retreivestats = OFF; enable_generatestats = OFF; }
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS2
	if(config.enablecollectglobalstats == ON){ if(enable_retreivestats == ON){ cout<<"collectstats:: retrieving stats..."<<endl; } else { cout<<"collectstats:: generating stats..."<<endl; }}
	#endif 
	
	// retrieve stats
	resetmanykeyandvalues(globalstatsbuffer, NUM_PARTITIONS, 0); // REMOVEME. redundant?
	fastcollectglobalstats(enable_retreivestats, globalstatsbuffer, sweepparams.currentLOP, sweepparams.source_partition);
	
	// generate stats 
	MAIN_LOOP1C_COLLECTGLOBALSTATS: for(batch_type offset_kvs=ctravstate.begin_kvs; offset_kvs<ctravstate.end_kvs; offset_kvs+=ctravstate.skip_kvs){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_collectstatsloop avg=analysis_collectstatsloop
		#ifdef _DEBUGMODE_KERNELPRINTS
		actsutilityobj->print4("### dispatch::collectglobalstats:: offset_kvs", "begin_kvs", "end_kvs", "skip", offset_kvs, ctravstate.begin_kvs, ctravstate.end_kvs, SRCBUFFER_SIZE);
		#endif
		
		ctravstate.i_kvs = offset_kvs;
		
		readkeyvalues(ON, kvdram, sourcebuffer, (sweepparams.worksourcebaseaddress_kvs + offset_kvs), SRCBUFFER_SIZE, ctravstate);
		
		collectglobalstats(ON, sourcebuffer, buffer_setof1, sweepparams.currentLOP, sweepparams.upperlimit, ctravstate, globalparams);
	}
	prepareglobalstats(enable_generatestats, buffer_setof1, globalstatsbuffer, globalparams);
	return;
}

// partition updates phase
void 
	#ifdef SW 
	actslw::
	#endif
partitionupdates_coarsegrainedpipeline(
		bool_type enable,
		uint512_dt * kvdram,
		keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer_setof1[8][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so1[8][NUM_PARTITIONS],
		keyvalue_t buffer_setof2[8][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so2[4][NUM_PARTITIONS],
		keyvalue_t buffer_setof4[8][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so4[2][NUM_PARTITIONS],
		keyvalue_t buffer_setof8[8][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so8[NUM_PARTITIONS],
		keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ], 
		config_t config,
		globalparams_t globalparams,
		sweepparams_t sweepparams,
		travstate_t ptravstate){
	#pragma HLS INLINE
	analysis_type analysis_partitionloop = KVDATA_BATCHSIZE_KVS / (2 * SRCBUFFER_SIZE);
	if(enable == OFF){ return; } // NEWCHANGE. CHECKWITHVHLS.
	
	travstate_t ptravstatepp0 = ptravstate;
	travstate_t ptravstatepp1 = ptravstate;
	travstate_t ptravstatepp2 = ptravstate;
	MAIN_LOOP1D_PARTITION: for(batch_type offset_kvs=ptravstate.begin_kvs; offset_kvs<ptravstate.end_kvs; offset_kvs+=ptravstate.skip_kvs * NUMPIPELINES){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_partitionloop avg=analysis_partitionloop
		#ifdef _DEBUGMODE_KERNELPRINTS
		actsutilityobj->print4("### dispatch::partition:: offset_kvs", "begin_kvs", "end_kvs", "skip", offset_kvs, ptravstate.begin_kvs, ptravstate.end_kvs, SRCBUFFER_SIZE);
		#endif
		
		ptravstate.i_kvs = offset_kvs;
		ptravstatepp0.i_kvs = offset_kvs;
		ptravstatepp1.i_kvs = offset_kvs + SRCBUFFER_SIZE;
		ptravstatepp2.i_kvs = offset_kvs + (2 * SRCBUFFER_SIZE);
		unsigned int en; if(offset_kvs == ptravstate.begin_kvs){ en = OFF; } else { en = ON; }
		
		readkeyvalues(ON, kvdram, sourcebuffer, (sweepparams.worksourcebaseaddress_kvs + offset_kvs), SRCBUFFER_SIZE, ptravstatepp0);
		#ifdef PP1 // pipeline overflow from bottom
		savekeyvalues(en, kvdram, buffer_setof8, globalstatsbuffer, templocalcapsule_so8, sweepparams.workdestbaseaddress_kvs, globalparams);
		#endif
		#ifdef PP2 // pipeline overflow from bottom
		combineSetof4stoSetof8s(en, buffer_setof4, buffer_setof8, templocalcapsule_so4, templocalcapsule_so8, globalparams);
		#endif
		
		partitionkeyvalues_coarsegrainedpipeline(ON, sourcebuffer, buffer_setof1, templocalcapsule_so1, sweepparams.currentLOP, sweepparams.upperlimit, ptravstatepp0, SRCBUFFER_SIZE, globalparams);
		#ifdef PP1
		readkeyvalues(ON, kvdram, sourcebuffer, (sweepparams.worksourcebaseaddress_kvs + offset_kvs + SRCBUFFER_SIZE), SRCBUFFER_SIZE, ptravstatepp1);
		#endif
		#ifdef PP2 // pipeline overflow from bottom
		savekeyvalues(en, kvdram, buffer_setof8, globalstatsbuffer, templocalcapsule_so8, sweepparams.workdestbaseaddress_kvs, globalparams);
		#endif
		
		// 1s->2s
		combineSetof1stoSetof2s(ON, buffer_setof1, buffer_setof2, templocalcapsule_so1, templocalcapsule_so2, globalparams);
		#ifdef PP1
		partitionkeyvalues_coarsegrainedpipeline(ON, sourcebuffer, buffer_setof1, templocalcapsule_so1, sweepparams.currentLOP, sweepparams.upperlimit, ptravstatepp1, SRCBUFFER_SIZE, globalparams);
		#endif
		#ifdef PP2
		readkeyvalues(ON, kvdram, sourcebuffer, (sweepparams.worksourcebaseaddress_kvs + offset_kvs + SRCBUFFER_SIZE + SRCBUFFER_SIZE), SRCBUFFER_SIZE, ptravstatepp2);
		#endif
		
		// 2s->4s
		combineSetof2stoSetof4s(ON, buffer_setof2, buffer_setof4, templocalcapsule_so2, templocalcapsule_so4, globalparams);
		#ifdef PP1
		combineSetof1stoSetof2s(ON, buffer_setof1, buffer_setof2, templocalcapsule_so1, templocalcapsule_so2, globalparams);
		#endif
		#ifdef PP2
		partitionkeyvalues_coarsegrainedpipeline(ON, sourcebuffer, buffer_setof1, templocalcapsule_so1, sweepparams.currentLOP, sweepparams.upperlimit, ptravstatepp2, SRCBUFFER_SIZE, globalparams);
		#endif
		
		// 4s->8s
		combineSetof4stoSetof8s(ON, buffer_setof4, buffer_setof8, templocalcapsule_so4, templocalcapsule_so8, globalparams);
		#ifdef PP1
		combineSetof2stoSetof4s(ON, buffer_setof2, buffer_setof4, templocalcapsule_so2, templocalcapsule_so4, globalparams);
		#endif
		#ifdef PP2
		combineSetof1stoSetof2s(ON, buffer_setof1, buffer_setof2, templocalcapsule_so1, templocalcapsule_so2, globalparams);
		#endif
		
		savekeyvalues(ON, kvdram, buffer_setof8, globalstatsbuffer, templocalcapsule_so8, sweepparams.workdestbaseaddress_kvs, globalparams);
		#ifdef PP1
		combineSetof4stoSetof8s(ON, buffer_setof4, buffer_setof8, templocalcapsule_so4, templocalcapsule_so8, globalparams);
		#endif
		#ifdef PP2
		combineSetof2stoSetof4s(ON, buffer_setof2, buffer_setof4, templocalcapsule_so2, templocalcapsule_so4, globalparams);
		#endif
		///// overflow. pipeline continuing from top...
	}
	#ifdef PP1 // pipeline overflow from bottom
	savekeyvalues(config.enablepartition, kvdram, buffer_setof8, globalstatsbuffer, templocalcapsule_so8, sweepparams.workdestbaseaddress_kvs, globalparams);
	#endif
	#ifdef PP2 // pipeline overflow from bottom
	combineSetof4stoSetof8s(config.enablepartition, buffer_setof4, buffer_setof8, templocalcapsule_so4, templocalcapsule_so8, globalparams);
	#endif
	
	#ifdef PP2 // pipeline overflow from bottom
	savekeyvalues(config.enablepartition, kvdram, buffer_setof8, globalstatsbuffer, templocalcapsule_so8, sweepparams.workdestbaseaddress_kvs, globalparams);
	#endif
	return;
}

void 
	#ifdef SW 
	actslw::
	#endif
partitionupdates_finegrainedpipeline(
		bool_type enable,
		uint512_dt * kvdram,
		keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer_setof1[8][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so1[8][NUM_PARTITIONS],
		keyvalue_t buffer_setof2[8][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so2[4][NUM_PARTITIONS],
		keyvalue_t buffer_setof4[8][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so4[2][NUM_PARTITIONS],
		keyvalue_t buffer_setof8[8][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so8[NUM_PARTITIONS],
		keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ],
		config_t config,
		globalparams_t globalparams,
		sweepparams_t sweepparams,
		travstate_t ptravstate,
		batch_type sourcebaseaddr_kvs,
		batch_type destbaseaddr_kvs){
	#ifndef MERGEPROCESSEDGESANDPARTITIONSTAGE
	#pragma HLS INLINE
	#endif
	/** 
	0 (A:filled, B:filled)
	1 (A:filled, B:filled, C:filled)
	2 (A:filled, B:filled, C:filled, D:filled) */
	analysis_type analysis_partitionloop = KVDATA_BATCHSIZE_KVS / (NUMACTSFASTPIPELINES * SRCBUFFER_SIZE);
	if(enable == OFF){ return; } 
	
	travstate_t ptravstatepp0 = ptravstate;
	travstate_t ptravstatepp1 = ptravstate;
	
	#ifndef OPTMZ
	resetmanykeyandvalues(buffer_setof1, PADDEDDESTBUFFER_SIZE, sweepparams.upperlimit);
	resetmanykeyandvalues(buffer_setof2, PADDEDDESTBUFFER_SIZE, sweepparams.upperlimit); 
	resetmanykeyandvalues(buffer_setof4, PADDEDDESTBUFFER_SIZE, sweepparams.upperlimit); 
	resetmanykeyandvalues(buffer_setof8, PADDEDDESTBUFFER_SIZE, sweepparams.upperlimit); 
	#endif
	
	resetmanykeyandvalues(templocalcapsule_so1, NUM_PARTITIONS, 0);
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->resetkeyvalues((keyvalue_t *)&templocalcapsule_so2[0], 4*NUM_PARTITIONS);
	actsutilityobj->resetkeyvalues((keyvalue_t *)&templocalcapsule_so4[0], 2*NUM_PARTITIONS);
	actsutilityobj->resetkeyvalues((keyvalue_t *)&templocalcapsule_so8[0], NUM_PARTITIONS);
	#endif 
	
	bool_type pp0readen = ON;
	bool_type pp1readen = ON;
	bool_type pp0runpipelineen = ON;
	bool_type pp1runpipelineen = ON;
	
	bool_type pp0partitionen = ON;
	bool_type pp1partitionen = ON;
	
	bool_type pp0writeen = ON;
	bool_type pp1writeen = ON;
	buffer_type pp0cutoff = SRCBUFFER_SIZE;
	buffer_type pp1cutoff = SRCBUFFER_SIZE;
	batch_type pp0readoffset_kvs = ptravstate.begin_kvs;
	batch_type pp1readoffset_kvs = ptravstate.begin_kvs + SRCBUFFER_SIZE;
	buffer_type pp0readsize_kvs = SRCBUFFER_SIZE;
	buffer_type pp1readsize_kvs = SRCBUFFER_SIZE;
	batch_type itercount = 0;
	batch_type flushsize = 0;

	batch_type paddsize_kvs = 2*ptravstate.skip_kvs;
	batch_type cutoffpaddsize_kvs = 2048*ptravstate.skip_kvs; // CRITICAL FIXME. // JUSTCHANGED.
	// batch_type cutoffpaddsize_kvs = 0; 
	batch_type approxendoffset_kvs = ptravstate.end_kvs + cutoffpaddsize_kvs + paddsize_kvs;

	MAINLOOP_PARTITION: for(batch_type offset_kvs=ptravstate.begin_kvs; offset_kvs<approxendoffset_kvs; offset_kvs+=ptravstate.skip_kvs * NUMACTSFASTPIPELINES){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_partitionloop avg=analysis_partitionloop
		#ifdef _DEBUGMODE_KERNELPRINTS
		actsutilityobj->print6("### dispatch::partition:: offset0_kvs", "size0_kvs", "begin_kvs", "end_kvs", "skip", "itercount", pp0readoffset_kvs, pp0readsize_kvs, ptravstate.begin_kvs, ptravstate.end_kvs, SRCBUFFER_SIZE, itercount);
		#ifdef FPP1
		actsutilityobj->print6("### dispatch::partition:: offset1_kvs", "size1_kvs", "begin_kvs", "end_kvs", "skip", "itercount", pp1readoffset_kvs, pp1readsize_kvs, ptravstate.begin_kvs, ptravstate.end_kvs, SRCBUFFER_SIZE, itercount);
		#endif
		#endif
		ptravstate.i_kvs = NAp;
		ptravstatepp0.i_kvs = pp0readoffset_kvs;
		#ifdef FPP1
		ptravstatepp1.i_kvs = pp1readoffset_kvs;
		if(itercount >= 2){ pp0writeen = ON; } else { pp0writeen = OFF; }
		if(itercount >= 4){ pp1writeen = ON; } else { pp1writeen = OFF; }
		if(itercount >= 2){ pp1runpipelineen = ON; } else { pp1runpipelineen = OFF; }
		#else 
		if(itercount >= 2){ pp0writeen = ON; } else { pp0writeen = OFF; }
		#endif
	
		readkeyvalues(ON, kvdram, sourcebuffer, (sourcebaseaddr_kvs + pp0readoffset_kvs), pp0readsize_kvs, ptravstatepp0); 
		#ifdef FPP1
		runpipeline(pp1runpipelineen, buffer_setof1, templocalcapsule_so1, buffer_setof2, templocalcapsule_so2, buffer_setof4, templocalcapsule_so4, buffer_setof8, templocalcapsule_so8, sweepparams.currentLOP, sweepparams.upperlimit, pp1cutoff, (itercount-2)+1, globalparams);
		#endif 
		
		pp0cutoff = partitionkeyvalues_finegrainedpipeline(ON, sourcebuffer, buffer_setof1, templocalcapsule_so1, sweepparams.currentLOP, sweepparams.upperlimit, ptravstatepp0, pp0readsize_kvs, globalparams);
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->collectstats(ON, buffer_setof1, pp0cutoff , sweepparams.currentLOP, sweepparams.upperlimit, globalparams.batch_range_pow, 0, itercount%MYSTATSYSIZE);
			actsutilityobj->collectstats(ON, buffer_setof1, pp0cutoff, sweepparams.currentLOP, sweepparams.upperlimit, globalparams.batch_range_pow, 7, 1);
			#endif 
		#ifdef FPP1
		savekeyvalues(pp1writeen, kvdram, buffer_setof8, globalstatsbuffer, templocalcapsule_so8, destbaseaddr_kvs, globalparams); 
			#ifdef _DEBUGMODE_CHECKS2
			if(pp1writeen==ON){ actsutilityobj->collectstats(ON, buffer_setof8, (keyvalue_t *)templocalcapsule_so8, sweepparams.currentLOP, sweepparams.upperlimit, globalparams.batch_range_pow, 1, (itercount+1)%MYSTATSYSIZE); }
			#endif 
		#endif 
		
		runpipeline(ON, buffer_setof1, templocalcapsule_so1, buffer_setof2, templocalcapsule_so2, buffer_setof4, templocalcapsule_so4, buffer_setof8, templocalcapsule_so8, sweepparams.currentLOP, sweepparams.upperlimit, pp0cutoff, itercount, globalparams);
		#ifdef FPP1
		readkeyvalues(ON, kvdram, sourcebuffer, (sourcebaseaddr_kvs + pp1readoffset_kvs), pp1readsize_kvs, ptravstatepp1);
		#endif
		
		savekeyvalues(pp0writeen, kvdram, buffer_setof8, globalstatsbuffer, templocalcapsule_so8, destbaseaddr_kvs, globalparams);
			#ifdef _DEBUGMODE_CHECKS2
			if(pp0writeen==ON){ actsutilityobj->collectstats(ON, buffer_setof8, (keyvalue_t *)templocalcapsule_so8, sweepparams.currentLOP, sweepparams.upperlimit, globalparams.batch_range_pow, 1, itercount%MYSTATSYSIZE); }
			#endif
		#ifdef FPP1
		pp1cutoff = partitionkeyvalues_finegrainedpipeline(pp1partitionen, sourcebuffer, buffer_setof1, templocalcapsule_so1, sweepparams.currentLOP, sweepparams.upperlimit, ptravstatepp1, pp1readsize_kvs, globalparams);
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->collectstats(ON, buffer_setof1, pp1cutoff , sweepparams.currentLOP, sweepparams.upperlimit, globalparams.batch_range_pow, 0, (itercount+1)%MYSTATSYSIZE);
			actsutilityobj->collectstats(ON, buffer_setof1, pp1cutoff, sweepparams.currentLOP, sweepparams.upperlimit, globalparams.batch_range_pow, 7, 1);
			#endif 
		#endif
		
		#ifdef FPP1
		pp0readoffset_kvs = pp1readoffset_kvs + pp1readsize_kvs;
		pp0readsize_kvs = pp0cutoff;
		pp1readoffset_kvs = pp0readoffset_kvs + pp0cutoff;
		pp1readsize_kvs = pp1cutoff;
		#else 
		pp0readoffset_kvs = pp0readoffset_kvs + pp0readsize_kvs;
		pp0readsize_kvs = pp0cutoff;
		#endif
		
		if(pp0readoffset_kvs >= ptravstate.end_kvs){ // FIXME. edge condition for perfect accuracy.
			if(flushsize >= 2){ 
				cout<<"partitionupdates successful. all pipeline stages flushed. breaking out..."<<endl; 
				cout<<"pp0readoffset_kvs: "<<pp0readoffset_kvs<<", pp1readoffset_kvs: "<<pp1readoffset_kvs<<", ptravstate.end_kvs: "<<ptravstate.end_kvs<<endl;
				break; }
			flushsize += 1;
		}
			
		itercount += NUMACTSFASTPIPELINES;
		
		#ifdef _DEBUGMODE_CHECKS2
		if(itercount%MYSTATSYSIZE == MYSTATSYSIZE-1){ actsutilityobj->intrapartitioncheck(); }
		#endif
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printglobalvars();
	actsutilityobj->clearglobalvars();
	#endif
	return;
}

// reduce phase 
void 
	#ifdef SW 
	actslw::
	#endif
reduceupdates(
		bool_type enable, // NEWCHANGE. CHECKWITHVHLS.
		uint512_dt * kvdram,
		keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer_setof1[8][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so1[8][NUM_PARTITIONS],
		keyvalue_t buffer_setof2[8][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so2[4][NUM_PARTITIONS],
		keyvalue_t buffer_setof4[8][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so4[2][NUM_PARTITIONS],
		keyvalue_t buffer_setof8[8][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so8[NUM_PARTITIONS],
		keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ], 
		config_t config,
		globalparams_t globalparams,
		sweepparams_t sweepparams,
		travstate_t rtravstate){
	#pragma HLS INLINE
	analysis_type analysis_reduceloop = KVDATA_BATCHSIZE_KVS / SRCBUFFER_SIZE;
	if(enable == OFF){ return; } // NEWCHANGE. CHECKWITHVHLS.
	
	if((sweepparams.source_partition % NUMVERTEXPARTITIONSPERLOAD) == 0){ readvertices(config.enablereduce, kvdram, sourcebuffer, (globalparams.baseoffset_verticesdata_kvs + (sweepparams.source_partition * (globalparams.applyvertexbuffersz_kvs / 2))), PADDEDDESTBUFFER_SIZE); }
	replicatedata(config.enablereduce, sourcebuffer, buffer_setof2, (sweepparams.source_partition % NUMVERTEXPARTITIONSPERLOAD) * (globalparams.applyvertexbuffersz / 2), globalparams.applyvertexbuffersz / 2);
	MAIN_LOOP1E_REDUCE: for(batch_type offset_kvs=rtravstate.begin_kvs; offset_kvs<rtravstate.end_kvs; offset_kvs+=rtravstate.skip_kvs){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_reduceloop avg=analysis_reduceloop
		#ifdef _DEBUGMODE_KERNELPRINTS
		actsutilityobj->print4("### dispatch::reduce:: offset_kvs", "begin_kvs", "end_kvs", "skip", offset_kvs, rtravstate.begin_kvs, rtravstate.end_kvs, SRCBUFFER_SIZE);
		#endif
		
		rtravstate.i_kvs = offset_kvs;

		readkeyvalues(ON, kvdram, buffer_setof1, (sweepparams.worksourcebaseaddress_kvs + offset_kvs), SRCBUFFER_SIZE, rtravstate);
	
		reduce(ON, buffer_setof1, buffer_setof2, sweepparams.upperlimit, globalparams.GraphIter, globalparams.GraphAlgo, rtravstate, globalparams);
	}
	unifydata(config.enablereduce, buffer_setof2, sourcebuffer, (sweepparams.source_partition % NUMVERTEXPARTITIONSPERLOAD) * (globalparams.applyvertexbuffersz / 2), globalparams.applyvertexbuffersz / 2, globalparams.GraphAlgo);
	if((sweepparams.source_partition % NUMVERTEXPARTITIONSPERLOAD) == NUMVERTEXPARTITIONSPERLOAD-1){ savevertices(config.enablereduce, kvdram, sourcebuffer, (globalparams.baseoffset_verticesdata_kvs + ((sweepparams.source_partition - (NUMVERTEXPARTITIONSPERLOAD-1)) * (globalparams.applyvertexbuffersz_kvs / 2))), PADDEDDESTBUFFER_SIZE); }
	return;
}

// process edges phase 
void 
	#ifdef SW 
	actslw::
	#endif
processedges(
		bool_type enable, // NEWCHANGE. CHECKWITHVHLS.
		uint512_dt * kvdram,
		keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer_setof1[8][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so1[8][NUM_PARTITIONS],
		keyvalue_t buffer_setof2[8][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so2[4][NUM_PARTITIONS],
		keyvalue_t buffer_setof4[8][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so4[2][NUM_PARTITIONS],
		keyvalue_t buffer_setof8[8][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so8[NUM_PARTITIONS],
		keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ], 
		config_t config,
		globalparams_t globalparams,
		sweepparams_t sweepparams,
		travstate_t avtravstate){
	#pragma HLS INLINE
	analysis_type analysis_processedges_overallloop = BATCH_RANGE_KVS / PADDEDDESTBUFFER_SIZE;
	analysis_type analysis_processedges_loadedgebatch = 1;
	
	batch_type kvoffset_kvs = 0;
	#ifdef _WIDEWORD
	vertex_t firstvptr = kvdram[globalparams.baseoffset_vertexptr_kvs + (globalparams.srcvoffset / VECTOR_SIZE)].range(31, 0);
	#else 
	vertex_t firstvptr = kvdram[globalparams.baseoffset_vertexptr_kvs + (globalparams.srcvoffset / VECTOR_SIZE)].data[0].key;
	#endif 
	
	#ifdef _DEBUGMODE_KERNELPRINTS2
	if(config.enableprocessedges == ON){ actsutilityobj->print7("### dispatch::process_edges:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, avtravstate.begin_kvs * VECTOR_SIZE, avtravstate.end_kvs * VECTOR_SIZE, (avtravstate.end_kvs - avtravstate.begin_kvs) * VECTOR_SIZE, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams.currentLOP)), sweepparams.currentLOP); }							
	#endif
	MAIN_LOOP1F_PROCESSEDGES: for(batch_type voffset_kvs=avtravstate.begin_kvs; voffset_kvs<avtravstate.end_kvs; voffset_kvs+=SRCBUFFER_SIZE){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_processedges_overallloop avg=analysis_processedges_overallloop
		#ifdef _DEBUGMODE_KERNELPRINTS2
		actsutilityobj->print4("### dispatch::process_edges:: voffset", "vbegin", "vend", "vskip", voffset_kvs * VECTOR_SIZE, avtravstate.begin_kvs * VECTOR_SIZE, avtravstate.size_kvs * VECTOR_SIZE, SRCBUFFER_SIZE * VECTOR_SIZE);
		#endif
		
		readvertices(ON, kvdram, sourcebuffer, globalparams.baseoffset_vertexptr_kvs + (globalparams.srcvoffset / VECTOR_SIZE) + voffset_kvs, SRCBUFFER_SIZE + 1);
		for(batch_type v=0; v<VECTOR_SIZE; v+=1){
			replicatedata(ON, sourcebuffer, buffer_setof2, v * SRCBUFFER_SIZE, SRCBUFFER_SIZE + 1);
			
			vertex_t srcvlocaloffset = (voffset_kvs * VECTOR_SIZE) + (v * SRCBUFFER_SIZE);
			vertex_t myfirstsrcvid = globalparams.srcvoffset + (voffset_kvs * VECTOR_SIZE) + (v * SRCBUFFER_SIZE);
			vertex_t mylastsrcvid = myfirstsrcvid + SRCBUFFER_SIZE;// - 1;
			if(srcvlocaloffset >= globalparams.srcvsize){ mylastsrcvid = myfirstsrcvid; }
			if((srcvlocaloffset < globalparams.srcvsize) && (srcvlocaloffset + SRCBUFFER_SIZE >= globalparams.srcvsize)){ mylastsrcvid = myfirstsrcvid + globalparams.srcvsize - srcvlocaloffset; }
			
			keyy_t beginvptr = buffer_setof2[0][0].key;
			keyy_t endvptr = buffer_setof2[0][SRCBUFFER_SIZE].key;
			if(srcvlocaloffset >= globalparams.srcvsize){ endvptr = beginvptr; }
			if((srcvlocaloffset < globalparams.srcvsize) && (srcvlocaloffset + SRCBUFFER_SIZE >= globalparams.srcvsize)){ endvptr = buffer_setof2[0][globalparams.srcvsize - srcvlocaloffset - 1].key; }
			
			keyy_t localbeginvptr = beginvptr - firstvptr;
			keyy_t localendvptr = endvptr - firstvptr;
			keyy_t numedges = localendvptr - localbeginvptr + 2*VECTOR_SIZE;
			if(localbeginvptr == localendvptr){ numedges = 0; }
			
			#ifdef _DEBUGMODE_CHECKS2
			if(localendvptr < localbeginvptr){ cout<<"dispatch::ERROR: localendvptr < localbeginvptr. localbeginvptr: "<<localbeginvptr<<", localendvptr: "<<localendvptr<<endl; exit(EXIT_FAILURE); }
			if(localendvptr < globalparams.edgessize){ actsutilityobj->checkptr(myfirstsrcvid, mylastsrcvid, localbeginvptr, localendvptr, (keyvalue_t *)&kvdram[BASEOFFSET_EDGESDATA_KVS]); }
			#endif
			
			keyy_t localbeginvptr_kvs = localbeginvptr / VECTOR_SIZE;
			keyy_t numedges_kvs = numedges / VECTOR_SIZE;
			
			travstate_t edgestravstate;
			edgestravstate.begin_kvs = globalparams.baseoffset_edgesdata_kvs + (localbeginvptr / VECTOR_SIZE);
			edgestravstate.size_kvs = numedges_kvs;
			edgestravstate.end_kvs = edgestravstate.begin_kvs + edgestravstate.size_kvs;
			#ifdef _DEBUGMODE_KERNELPRINTS2
			cout<<"[localbeginvptr: "<<localbeginvptr<<", localendvptr: "<<localendvptr<<"][edgestravstate: begin: "<<(edgestravstate.begin_kvs - BASEOFFSET_EDGESDATA_KVS) * VECTOR_SIZE<<", end: "<<(edgestravstate.end_kvs - BASEOFFSET_EDGESDATA_KVS) * VECTOR_SIZE<<", size: "<<edgestravstate.size_kvs * VECTOR_SIZE<<"][voffset: "<<voffset_kvs * VECTOR_SIZE<<"][v: "<<v<<"]"<<endl;
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			if(localendvptr < localbeginvptr){ cout<<"ERROR: localendvptr < localbeginvptr. EXITING..."<<endl; exit(EXIT_FAILURE); }
			#endif
				
			for(batch_type eoffset_kvs=localbeginvptr_kvs; eoffset_kvs<localbeginvptr_kvs + numedges_kvs; eoffset_kvs+=SRCBUFFER_SIZE){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_processedges_loadedgebatch avg=analysis_processedges_loadedgebatch
				#ifdef _DEBUGMODE_KERNELPRINTS2
				actsutilityobj->print4("### dispatch::process_edges:: eoffset", "ebegin", "esize", "eskip", eoffset_kvs * VECTOR_SIZE, localbeginvptr_kvs * VECTOR_SIZE, numedges_kvs * VECTOR_SIZE, SRCBUFFER_SIZE * VECTOR_SIZE);
				#endif
				
				edgestravstate.i_kvs = globalparams.baseoffset_edgesdata_kvs + eoffset_kvs;
	
				readkeyvalues(ON, kvdram, buffer_setof4, globalparams.baseoffset_edgesdata_kvs + eoffset_kvs, SRCBUFFER_SIZE, edgestravstate);
				
				#ifdef EMBEDDEDCOLLECTSTATS
				process_edges(ON, buffer_setof2, buffer_setof4, buffer_setof1, globalparams.srcvoffset + (voffset_kvs * VECTOR_SIZE) + (v * SRCBUFFER_SIZE), globalparams.GraphIter, globalparams.GraphAlgo, edgestravstate, globalparams);
				#else 
				process_edges(ON, buffer_setof2, buffer_setof4, globalparams.srcvoffset + (voffset_kvs * VECTOR_SIZE) + (v * SRCBUFFER_SIZE), globalparams.GraphIter, globalparams.GraphAlgo, edgestravstate, globalparams);
				#endif 
				
				buffer_type savechunk_size = getchunksize_kvs(SRCBUFFER_SIZE, edgestravstate, 0);
				savevertices(ON, kvdram, buffer_setof4, globalparams.baseoffset_kvdram_kvs + kvoffset_kvs, savechunk_size); // is actually 'save keyvalues'
				kvoffset_kvs += savechunk_size;
			}
		}
	}
	return;
}

void 
	#ifdef SW 
	actslw::
	#endif
processedges(
		bool_type enable, // NEWCHANGE. CHECKWITHVHLS.
		uint512_dt * kvdram,
		keyvalue_t vertices1buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t vertices2buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer_setof1[8][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so1[8][NUM_PARTITIONS],
		keyvalue_t buffer_setof2[8][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so2[4][NUM_PARTITIONS],
		keyvalue_t buffer_setof4[8][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so4[2][NUM_PARTITIONS],
		keyvalue_t buffer_setof8[8][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so8[NUM_PARTITIONS],
		keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ], 
		config_t config,
		globalparams_t globalparams,
		sweepparams_t sweepparams,
		travstate_t avtravstate){
	// process edges is integrated with partition updates
	#pragma HLS INLINE
	analysis_type analysis_processedges_overallloop = BATCH_RANGE_KVS / PADDEDDESTBUFFER_SIZE;
	analysis_type analysis_processedges_loadedgebatch = 1;
	if(enable == OFF){ return; } // NEWCHANGE. CHECKWITHVHLS.
	
	batch_type kvoffset_kvs = 0;
	#ifdef _WIDEWORD
	vertex_t firstvptr = kvdram[globalparams.baseoffset_vertexptr_kvs + (globalparams.srcvoffset / VECTOR_SIZE)].range(31, 0);
	#else 
	vertex_t firstvptr = kvdram[globalparams.baseoffset_vertexptr_kvs + (globalparams.srcvoffset / VECTOR_SIZE)].data[0].key;
	#endif 
	
	#ifdef _DEBUGMODE_KERNELPRINTS2
	if(config.enableprocessedges == ON){ actsutilityobj->print7("### dispatch::process_edges:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, avtravstate.begin_kvs * VECTOR_SIZE, avtravstate.end_kvs * VECTOR_SIZE, (avtravstate.end_kvs - avtravstate.begin_kvs) * VECTOR_SIZE, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams.currentLOP)), sweepparams.currentLOP); }							
	#endif
	MAIN_LOOP1F_PROCESSEDGES: for(batch_type voffset_kvs=avtravstate.begin_kvs; voffset_kvs<avtravstate.end_kvs; voffset_kvs+=SRCBUFFER_SIZE){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_processedges_overallloop avg=analysis_processedges_overallloop
		#ifdef _DEBUGMODE_KERNELPRINTS2
		actsutilityobj->print4("### dispatch::process_edges:: voffset", "vbegin", "vend", "vskip", voffset_kvs * VECTOR_SIZE, avtravstate.begin_kvs * VECTOR_SIZE, avtravstate.size_kvs * VECTOR_SIZE, SRCBUFFER_SIZE * VECTOR_SIZE);
		#endif
		
		readvertices(ON, kvdram, vertices1buffer, globalparams.baseoffset_vertexptr_kvs + (globalparams.srcvoffset / VECTOR_SIZE) + voffset_kvs, SRCBUFFER_SIZE + 1);
		for(batch_type v=0; v<VECTOR_SIZE; v+=1){
			replicatedata(ON, vertices1buffer, vertices2buffer, v * SRCBUFFER_SIZE, SRCBUFFER_SIZE + 1);	
			
			vertex_t srcvlocaloffset = (voffset_kvs * VECTOR_SIZE) + (v * SRCBUFFER_SIZE);
			vertex_t myfirstsrcvid = globalparams.srcvoffset + (voffset_kvs * VECTOR_SIZE) + (v * SRCBUFFER_SIZE);
			vertex_t mylastsrcvid = myfirstsrcvid + SRCBUFFER_SIZE;// - 1;
			if(srcvlocaloffset >= globalparams.srcvsize){ mylastsrcvid = myfirstsrcvid; }
			if((srcvlocaloffset < globalparams.srcvsize) && (srcvlocaloffset + SRCBUFFER_SIZE >= globalparams.srcvsize)){ mylastsrcvid = myfirstsrcvid + globalparams.srcvsize - srcvlocaloffset; }
			
			keyy_t beginvptr = vertices2buffer[0][0].key;
			keyy_t endvptr = vertices2buffer[0][SRCBUFFER_SIZE].key;
			if(srcvlocaloffset >= globalparams.srcvsize){ endvptr = beginvptr; }
			if((srcvlocaloffset < globalparams.srcvsize) && (srcvlocaloffset + SRCBUFFER_SIZE >= globalparams.srcvsize)){ endvptr = vertices2buffer[0][globalparams.srcvsize - srcvlocaloffset - 1].key; }
			
			keyy_t localbeginvptr = beginvptr - firstvptr;
			keyy_t localendvptr = endvptr - firstvptr;
			keyy_t numedges = localendvptr - localbeginvptr + 2*VECTOR_SIZE;
			if(localbeginvptr == localendvptr){ numedges = 0; }
			
			#ifdef _DEBUGMODE_CHECKS2
			if(localendvptr < localbeginvptr){ cout<<"dispatch::ERROR: localendvptr < localbeginvptr. localbeginvptr: "<<localbeginvptr<<", localendvptr: "<<localendvptr<<endl; exit(EXIT_FAILURE); }
			if(localendvptr < globalparams.edgessize){ actsutilityobj->checkptr(myfirstsrcvid, mylastsrcvid, localbeginvptr, localendvptr, (keyvalue_t *)&kvdram[BASEOFFSET_EDGESDATA_KVS]); }
			#endif
			
			keyy_t localbeginvptr_kvs = localbeginvptr / VECTOR_SIZE;
			keyy_t numedges_kvs = numedges / VECTOR_SIZE;
			
			travstate_t edgestravstate;
			edgestravstate.begin_kvs = globalparams.baseoffset_edgesdata_kvs + (localbeginvptr / VECTOR_SIZE);
			edgestravstate.size_kvs = numedges_kvs;
			edgestravstate.end_kvs = edgestravstate.begin_kvs + edgestravstate.size_kvs;
			#ifdef _DEBUGMODE_KERNELPRINTS2
			cout<<"[localbeginvptr: "<<localbeginvptr<<", localendvptr: "<<localendvptr<<"][edgestravstate: begin: "<<(edgestravstate.begin_kvs - BASEOFFSET_EDGESDATA_KVS) * VECTOR_SIZE<<", end: "<<(edgestravstate.end_kvs - BASEOFFSET_EDGESDATA_KVS) * VECTOR_SIZE<<", size: "<<edgestravstate.size_kvs * VECTOR_SIZE<<"][voffset: "<<voffset_kvs * VECTOR_SIZE<<"][v: "<<v<<"]"<<endl;
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			if(localendvptr < localbeginvptr){ cout<<"ERROR: localendvptr < localbeginvptr. EXITING..."<<endl; exit(EXIT_FAILURE); }
			actsutilityobj->clearallstats();
			#endif
			
			travstate_t ptravstate;
			ptravstate.begin_kvs = localbeginvptr_kvs;
			ptravstate.size_kvs = numedges_kvs;
			ptravstate.end_kvs = ptravstate.begin_kvs + ptravstate.size_kvs;
			ptravstate.skip_kvs = SRCBUFFER_SIZE;
			ptravstate.i_kvs = ptravstate.begin_kvs;
			partitionupdates_finegrainedpipeline(
				ON,
				kvdram,
				sourcebuffer,
				buffer_setof1,
				templocalcapsule_so1,
				buffer_setof2,
				templocalcapsule_so2,
				buffer_setof4,
				templocalcapsule_so4,
				buffer_setof8,
				templocalcapsule_so8,
				globalstatsbuffer,
				config,
				globalparams,
				sweepparams,
				ptravstate,
				globalparams.baseoffset_edgesdata_kvs,
				globalparams.baseoffset_kvdramworkspace_kvs);
			// exit(EXIT_SUCCESS);
			
			#ifdef _DEBUGMODE_KERNELPRINTS
			actsutilityobj->printglobalvars();
			actsutilityobj->clearglobalvars();
			#endif
			// exit(EXIT_SUCCESS);
		}
	}
	return;
}

// main function
void 
	#ifdef SW 
	actslw::
	#endif 
dispatch(uint512_dt * kvdram){
	analysis_type analysis_numllops = 1;
	analysis_type analysis_numsourcepartitions = 1;
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printparameters();
	actsutilityobj->printglobalvars();
	#endif 
	#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(_DEBUGMODE_CHECKS2)
	actsutilityobj->clearglobalvars();
	#endif
	
	#ifdef MERGEPROCESSEDGESANDPARTITIONSTAGE
	keyvalue_t vertices1buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = vertices1buffer
	keyvalue_t vertices2buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = vertices2buffer
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
	
	keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ]; 
	batch_type skipsizes[NUM_PARTITIONS];
	#ifdef _DEBUGMODE_CHECKS2
	keyvalue_t BIGKV[NUM_PARTITIONS];
	#endif
	
	batch_type sourcestatsmarker = 0;
	batch_type deststatsmarker = 1;
	batch_type destoffset = 0;
	config_t config;
	globalparams_t globalparams = getglobalparams(kvdram);
	sweepparams_t sweepparams;
	#ifdef _DEBUGMODE_CHECKS2
	if(globalparams.runsize >= MAXKVDATA_BATCHSIZE){ cout<<"dispatch:ERROR. runsize too large!. globalparams.runsize: "<<globalparams.runsize<<", MAXKVDATA_BATCHSIZE: "<<MAXKVDATA_BATCHSIZE<<". EXITING"<<endl; exit(EXIT_FAILURE); }
	#endif
	
	// start launch
	MAIN_LOOP1: for(step_type currentLOP=globalparams.beginLOP; currentLOP<(globalparams.beginLOP + globalparams.numLOPs); currentLOP++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_numllops avg=analysis_numllops	
	
		batch_type num_source_partitions = get_num_source_partitions(currentLOP);
		destoffset = 0;
		
		MAIN_LOOP1B: for(batch_type source_partition=0; source_partition<num_source_partitions; source_partition+=1){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_numsourcepartitions avg=analysis_numsourcepartitions	
		
			#ifdef EMBEDDEDCOLLECTSTATS
			resetmanykeyandvalues(buffer_setof1, GLOBALSTATSBUFFERSZ, 0);
			#else 
			resetmanykeyandvalues(buffer_setof1, NUM_PARTITIONS, 0);
			#endif 
			resetmanykeyandvalues(globalstatsbuffer, NUM_PARTITIONS, 0);
		
			sweepparams = getsweepparams(globalparams, currentLOP, source_partition);
			travstate_t travstate = gettravstate(kvdram, globalparams, currentLOP, sourcestatsmarker);
			travstate_t ctravstate = travstate;
			travstate_t ptravstate = travstate;
			travstate_t rtravstate = travstate;
			travstate_t avtravstate;
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->setstructs(config, sweepparams, travstate);
			actsutilityobj->clearallstats();
			#endif 
			
			// process all edges
			#ifdef PROCESSALLEDGES
			if(inprocessedgesstage(currentLOP) == true){ avtravstate.begin_kvs = 0; }
			if(inprocessedgesstage(currentLOP) == true){ avtravstate.end_kvs = avtravstate.begin_kvs + globalparams.srcvsize_kvs; avtravstate.size_kvs = globalparams.srcvsize_kvs; }
			if(inprocessedgesstage(currentLOP) == true){ config.enableprocessedges = ON; config.enablecollectglobalstats = OFF; config.enablepartition = OFF; config.enablereduce = OFF; }  // FIXME. REMOVEME. use srcvoffset instead?
			else { avtravstate.begin_kvs = 0; avtravstate.end_kvs = 0; config.enableprocessedges = OFF; }
			#ifdef MERGEPROCESSEDGESANDPARTITIONSTAGE
			if(inprocessedgesstage(currentLOP) == true){ 
			readglobalstats(ON, kvdram, globalstatsbuffer, globalparams.baseoffset_statsdram_kvs + deststatsmarker);
			for(partition_type p=0; p<NUM_PARTITIONS; p++){ batch_type A = (globalstatsbuffer[p].value + (VECTOR_SIZE-1)) / VECTOR_SIZE; batch_type B = (A + (SRCBUFFER_SIZE-1)) / SRCBUFFER_SIZE; if(B < 80){ B = B * 2; } batch_type C = ((4 * 4 * 2) * NUM_PARTITIONS) + VECTOR_SIZE; skipsizes[p] = (B * C) + 128; } //'128' is safety padd // FIXME. REMOVEME.
			calculateoffsets(globalstatsbuffer, NUM_PARTITIONS, destoffset, skipsizes);
			saveglobalstats(ON, kvdram, globalstatsbuffer, globalparams.baseoffset_statsdram_kvs + deststatsmarker);
			resetvalues(globalstatsbuffer, NUM_PARTITIONS, 0); }
			#endif
			processedges(
				config.enableprocessedges,
				kvdram,
				#ifdef MERGEPROCESSEDGESANDPARTITIONSTAGE
				vertices1buffer,
				vertices2buffer,
				#endif 
				sourcebuffer,
				buffer_setof1,
				templocalcapsule_so1,
				buffer_setof2,
				templocalcapsule_so2,
				buffer_setof4,
				templocalcapsule_so4,
				buffer_setof8,
				templocalcapsule_so8,
				globalstatsbuffer,
				config,
				globalparams,
				sweepparams,
				avtravstate);
			#ifdef EMBEDDEDCOLLECTSTATS
			prepareglobalstats2(config.enableprocessedges, buffer_setof1, globalstatsbuffer, NUM_PARTITIONS, globalparams);
			#endif
			#if defined(_DEBUGMODE_CHECKS2) & defined(ENABLE_PERFECTACCURACY)
			if(config.enableprocessedges == ON){ actsutilityobj->checkforlessthanthan("dispatch::finished process_edges function.", actsutilityobj->globalstats_getcountnumvalidprocessedges(), globalparams.runsize, 100000); } //
			#endif
			#endif
			
			// collect stats
			#ifdef COLLECTSTATS
			if(incollectstatsstage(currentLOP, globalparams) == true){ config.enableprocessedges = OFF; config.enablecollectglobalstats = ON; config.enablepartition = OFF; config.enablereduce = OFF; } 
			else { ctravstate.begin_kvs = 0; ctravstate.end_kvs = 0; config.enablecollectglobalstats = OFF; }
			if(incollectstatsstage(currentLOP, globalparams) == true && (ctravstate.end_kvs - ctravstate.begin_kvs) == 0){ ctravstate.begin_kvs = 0; ctravstate.end_kvs = 0; config.enablecollectglobalstats = OFF; } // NEWCHANGE.
			#ifdef _DEBUGMODE_KERNELPRINTS3
			#ifndef COLLECTSTATSOFFLINE
			if(config.enablecollectglobalstats == ON){ actsutilityobj->print7("### dispatch::collectgstats:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, ctravstate.begin_kvs * VECTOR_SIZE, ctravstate.end_kvs * VECTOR_SIZE, (ctravstate.end_kvs - ctravstate.begin_kvs) * VECTOR_SIZE, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams.currentLOP)), sweepparams.currentLOP); }					
			#endif
			#endif
			#ifdef COLLECTSTATSOFFLINE
				readglobalstats(config.enablecollectglobalstats, kvdram, globalstatsbuffer, globalparams.baseoffset_statsdram_kvs + deststatsmarker);
			#else
				collectstats(
					config.enablecollectglobalstats,
					kvdram,
					sourcebuffer,
					buffer_setof1,
					templocalcapsule_so1,
					buffer_setof2,
					templocalcapsule_so2,
					buffer_setof4,
					templocalcapsule_so4,
					buffer_setof8,
					templocalcapsule_so8,
					globalstatsbuffer,
					config,
					globalparams,
					sweepparams,
					ctravstate);
			#endif
			#if defined(_DEBUGMODE_CHECKS2) && defined(COLLECTSTATSOFFLINE)
			if(config.enablecollectglobalstats == ON){ copykeyvalues(actsutilityobj->getmykeyvalues(7), globalstatsbuffer, NUM_PARTITIONS); }
			#endif
			calculateglobaloffsets(globalstatsbuffer, skipsizes, destoffset, NUM_PARTITIONS);
			#ifdef _DEBUGMODE_CHECKS2
			resetvalues(BIGKV, NUM_PARTITIONS, 0);
			for(partition_type p=0; p<NUM_PARTITIONS; p++){ BIGKV[p].value = globalstatsbuffer[p].value + skipsizes[p]; }
			#endif
			#ifdef COLLECTSTATSOFFLINE
			if(incollectstatsstage(currentLOP, globalparams) == true){ saveglobalstats(config.enablecollectglobalstats, kvdram, globalstatsbuffer, globalparams.baseoffset_statsdram_kvs + deststatsmarker); }
			#endif
			resetvalues(globalstatsbuffer, NUM_PARTITIONS, 0);
			#ifndef COLLECTSTATSOFFLINE // REMOVEME?. redundant, unnecessary?
			if(incollectstatsstage(currentLOP, globalparams) == true){ saveglobalstats(config.enablecollectglobalstats, kvdram, globalstatsbuffer, globalparams.baseoffset_statsdram_kvs + deststatsmarker); }
			#endif
			#endif
			
			// partition
			#ifdef PARTITIONUPDATES
			if(inpartitionstage(currentLOP, globalparams) == true && (ptravstate.size_kvs > 0)){ config.enableprocessedges = OFF; config.enablecollectglobalstats = OFF; config.enablepartition = ON; config.enablereduce = OFF; } 
			else { ptravstate.begin_kvs = 0; ptravstate.end_kvs = 0; config.enablepartition = OFF; }
			if(ptravstate.size_kvs == 0){ ptravstate.begin_kvs = 0; ptravstate.end_kvs = 0; config.enablepartition = OFF; } // CHANGEMEBACK.
			#ifdef _DEBUGMODE_KERNELPRINTS3
			if((config.enablepartition == ON) && (currentLOP >= 1) && (currentLOP <= globalparams.treedepth)){ actsutilityobj->print7("### dispatch::partition:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, ptravstate.begin_kvs * VECTOR_SIZE, ptravstate.end_kvs * VECTOR_SIZE, ptravstate.size_kvs * VECTOR_SIZE, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams.currentLOP)), sweepparams.currentLOP); }	
			#endif
			resetvalues(globalstatsbuffer, NUM_PARTITIONS, 0);
			#ifdef ACTSFAST
			partitionupdates_finegrainedpipeline
			#else 
			partitionupdates_coarsegrainedpipeline
			#endif 
			(
				config.enablepartition,
				kvdram,
				sourcebuffer,
				buffer_setof1,
				templocalcapsule_so1,
				buffer_setof2,
				templocalcapsule_so2,
				buffer_setof4,
				templocalcapsule_so4,
				buffer_setof8,
				templocalcapsule_so8,
				globalstatsbuffer,
				config,
				globalparams,
				sweepparams,
				ptravstate,
				sweepparams.worksourcebaseaddress_kvs,
				sweepparams.workdestbaseaddress_kvs);
			#if defined(_DEBUGMODE_CHECKS2) && defined(COLLECTSTATSOFFLINE)
			if(config.enablepartition == ON){ actsutilityobj->postpartitioncheck(kvdram, globalstatsbuffer, ptravstate, sweepparams, globalparams); }//exit(EXIT_SUCCESS); }
			#endif 
			#ifndef COLLECTSTATSOFFLINE
			if(inpartitionstage(currentLOP, globalparams) == true){ saveglobalstats(config.enablepartition, kvdram, globalstatsbuffer, globalparams.baseoffset_statsdram_kvs + deststatsmarker); }
			#endif 
			#if defined(_DEBUGMODE_CHECKS2) && defined(ENABLE_PERFECTACCURACY) && not defined(ACTSFAST) // REMOVEME 'ACTSFAST'
			if(config.enablereduce == OFF){ actsutilityobj->checkforgreaterthan("dispatch. comparing BIGKV & globalstatsbuffer", BIGKV, globalstatsbuffer, NUM_PARTITIONS); }
			#endif
			#endif
			
			// reduce 
			#ifdef REDUCEUPDATES
			if(inreducestage(currentLOP, globalparams) == true){ config.enableprocessedges = OFF; config.enablecollectglobalstats = OFF; config.enablepartition = OFF; config.enablereduce = ON; } 
			else { rtravstate.begin_kvs = 0; rtravstate.end_kvs = 0; config.enablereduce = OFF; }
			if((rtravstate.end_kvs - rtravstate.begin_kvs) == 0){ rtravstate.begin_kvs = 0; rtravstate.end_kvs = 0; config.enablereduce = OFF; } 
			#ifdef _DEBUGMODE_KERNELPRINTS2
			if(config.enablereduce == ON){ actsutilityobj->print7("### dispatch::reduce:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, rtravstate.begin_kvs * VECTOR_SIZE, rtravstate.end_kvs * VECTOR_SIZE, (rtravstate.end_kvs - rtravstate.begin_kvs) * VECTOR_SIZE, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams.currentLOP)), sweepparams.currentLOP); }							
			#endif
			reduceupdates(
				config.enablereduce,
				kvdram,
				sourcebuffer,
				buffer_setof1,
				templocalcapsule_so1,
				buffer_setof2,
				templocalcapsule_so2,
				buffer_setof4,
				templocalcapsule_so4,
				buffer_setof8,
				templocalcapsule_so8,
				globalstatsbuffer,
				config,
				globalparams,
				sweepparams,
				rtravstate);
			#endif 
			
			if(currentLOP > 0){
				sourcestatsmarker += 1;
				deststatsmarker += NUM_PARTITIONS;
				destoffset = globalstatsbuffer[NUM_PARTITIONS-1].key + globalstatsbuffer[NUM_PARTITIONS-1].value + 64; }
			
			#ifdef _DEBUGMODE_CHECKS
			if(config.enablereduce == OFF){ actsutilityobj->printpartitionresult2(ON, kvdram, globalstatsbuffer, sweepparams); }
			#endif
			#ifdef _DEBUGMODE_KERNELPRINTS
			actsutilityobj->printglobalvars();
			actsutilityobj->clearglobalvars();
			#endif
		}
		#ifdef _DEBUGMODE_KERNELPRINTS3
		actsutilityobj->printglobalvars();
		#endif 
		#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(_DEBUGMODE_CHECKS2)
		actsutilityobj->clearglobalvars();
		#endif
	}
	#ifdef _DEBUGMODE_CHECKS
	actsutilityobj->countvalueslessthan("dispatch", (value_t *)&kvdram[BASEOFFSET_VERTICESDATA_KVS], BATCH_RANGE, INFINITI);
	#endif 
	return;
}

// top
extern "C" {
void 
	#ifdef SW 
	actslw:: 
	#endif
topkernel(uint512_dt * kvdram){
#pragma HLS INTERFACE m_axi port = kvdram offset = slave bundle = gmem0 // max_read_burst_length=64 max_write_burst_length=64

#pragma HLS INTERFACE s_axilite port = kvdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATA_PACK variable = kvdram

	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printparameters();
	// cout<<"---------------------------------------------------------------  SRCBUFFER_SIZE: "<<SRCBUFFER_SIZE<<endl;
	// exit(EXIT_SUCCESS);
	#endif 
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	#ifdef _WIDEWORD
	cout<<">>> Light weight ACTS (L2) Launched... size: "<<(unsigned int)(kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].range(31, 0))<<endl; 
	#else
	cout<<">>> Light weight ACTS (L2) Launched... size: "<<kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key<<endl; 
	#endif
	#endif
	
	dispatch(kvdram);
	return;
}
}







