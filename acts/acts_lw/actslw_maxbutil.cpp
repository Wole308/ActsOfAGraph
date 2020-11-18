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
#include "actslw_maxbutil.h"
using namespace std;

#ifdef SW
actslw_maxbutil::actslw_maxbutil(){ actsutilityobj = new actsutility(); }
actslw_maxbutil::~actslw_maxbutil(){}
#endif
#ifdef SWEMU
actsutility * actsutilityobj = new actsutility();
#endif

// kernel utilities
unsigned int
	#ifdef SW 
	actslw_maxbutil::
	#endif 
amin(unsigned int val1, unsigned int val2){
	if(val1 < val2){ return val1; }
	else { return val2; }
}
batch_type
	#ifdef SW 
	actslw_maxbutil::
	#endif
allignlower_KV(batch_type val){
	batch_type fac = val / VECTOR_SIZE;
	return (fac * VECTOR_SIZE);
}
batch_type
	#ifdef SW 
	actslw_maxbutil::
	#endif 
allignhigher_KV(batch_type val){
	batch_type fac = (val + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	return (fac * VECTOR_SIZE);
}
batch_type
	#ifdef SW 
	actslw_maxbutil::
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
	actslw_maxbutil::
	#endif 
resetkeyandvalues(skeyvalue_t * buffer, buffer_type size){
	for(buffer_type i=0; i<size; i++){ buffer[i].key = 0; buffer[i].value = 0; }
}
void 
	#ifdef SW 
	actslw_maxbutil::
	#endif 
resetvalues(keyvalue_t * buffer, buffer_type size){
	for(buffer_type i=0; i<size; i++){ buffer[i].value = 0; }
}
void 
	#ifdef SW 
	actslw_maxbutil::
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
	actslw_maxbutil::
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
	actslw_maxbutil::
	#endif 
resetmanykeyandvalues(keyvalue_t buffer[NUM_PARTITIONS], buffer_type size){
	for(buffer_type i=0; i<size; i++){
		buffer[i].key = 0; buffer[i].value = 0; 
	}
	return;
}
void 
	#ifdef SW 
	actslw_maxbutil::
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
buffer_type 
	#ifdef SW 
	actslw_maxbutil::
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
	actslw_maxbutil::
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
	actslw_maxbutil::
	#endif 
getpartition(keyvalue_t keyvalue, step_type currentLOP, vertex_t upperlimit, unsigned int batch_range_pow){
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
		if(partition >= NUM_PARTITIONS){ partition = (((1 << NUM_PARTITIONS_POW) - 1) & (partition >> (1 - 1))); } // FIXME. REMOVEME. PERFECTIONTEST.
	#endif
	
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("actslw_maxbutil::getpartition 2", partition, NUM_PARTITIONS, keyvalue.key, upperlimit, currentLOP);
	#endif
	return partition;
}
value_t 
	#ifdef SW 
	actslw_maxbutil::
	#endif 
reducefunc(value_t vtemp, value_t res, unsigned int GraphIter, unsigned int GraphAlgo){
	value_t temp = 0;
	#ifdef PR_ALGORITHM
	temp = vtemp + res;
	#elif defined(BFS_ALGORITHM)
	temp = amin(vtemp, GraphIter); // NEWCHANGE.
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
	actslw_maxbutil::
	#endif 
processedgefunc(value_t Uprop, unsigned int edgeweight, unsigned int voutdegree, unsigned int GraphIter, unsigned int GraphAlgo){
	// value_t ret = 0;
	// #ifdef PR_ALGORITHM
	// ret = Uprop;
	// #elif defined(BFS_ALGORITHM)
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
	actslw_maxbutil::
	#endif 
mergefunc(value_t value1, value_t value2, unsigned int GraphAlgo){
	value_t res = 0;
	#ifdef PR_ALGORITHM
	res = value1 + value2;
	#elif defined(BFS_ALGORITHM)
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
	actslw_maxbutil::
	#endif 
copykeyvalues(keyvalue_t * buffer1, keyvalue_t * buffer2, buffer_type size){
	COPYKEYS_LOOP: for(buffer_type i=0; i<size; i++){ buffer1[i] = buffer2[i]; }
}
buffer_type 
	#ifdef SW 
	actslw_maxbutil::
	#endif 
getpartitionwritesz(buffer_type realsize_kvs, buffer_type bramoffset_kvs){
	// #pragma HLS INLINE
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
		// #if defined(SW) || defined(SWEMU) 
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
	actslw_maxbutil::
	#endif 
withinvalidrange(buffer_type val1, buffer_type val2){
	// #pragma HLS INLINE
	if(val1 < val2){ return 1; }
	else { return 0; }
}
void 
	#ifdef SW 
	actslw_maxbutil::
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
	actslw_maxbutil::
	#endif 
calculateunallignedoffsets(skeyvalue_t buffer[NUM_PARTITIONS], buffer_type size, batch_type base, batch_type skipspacing){
	for(buffer_type i=1; i<size; i++){ 
		buffer[i].key = buffer[i-1].key + buffer[i-1].value + skipspacing; 
	}
	return;
}
void 
	#ifdef SW 
	actslw_maxbutil::
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
	actslw_maxbutil::
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
	actslw_maxbutil::
	#endif 
getvaluecount(keyvalue_t * keyvalues, unsigned int size){
	unsigned int totalnumkeyvalues = 0;
	for(unsigned int p=0; p<size; p++){ totalnumkeyvalues += keyvalues[p].value; }
	return totalnumkeyvalues;
}
batch_type
	#ifdef SW 
	actslw_maxbutil::
	#endif 
get_num_source_partitions(step_type currentLOP){
	// #pragma HLS INLINE
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
	actslw_maxbutil::
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
	#endif 
	globalparams.baseaddr_destkvs_kvs = 0;
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->printglobalparameters("actslw_maxbutil::getglobalparams:: printing global parameters", globalparams);
	#endif
	return globalparams;
}
sweepparams_t 
	#ifdef SW 
	actslw_maxbutil::
	#endif 
getsweepparams(globalparams_t globalparams, step_type currentLOP, batch_type source_partition){
	sweepparams_t sweepparams;
	batch_type sourceskipsize = getskipsize(currentLOP, SOURCE, globalparams);
	
	sweepparams.currentLOP = currentLOP;
	if((currentLOP % 2) == 1){ sweepparams.worksourcebaseaddress_kvs = BASEOFFSET_KVDRAM_KVS; sweepparams.workdestbaseaddress_kvs = BASEOFFSET_KVDRAMWORKSPACE_KVS; }
	else { sweepparams.worksourcebaseaddress_kvs = BASEOFFSET_KVDRAMWORKSPACE_KVS; sweepparams.workdestbaseaddress_kvs = BASEOFFSET_KVDRAM_KVS; }
	
	sweepparams.upperlimit = globalparams.destvoffset + (source_partition * sourceskipsize * (globalparams.batch_range / globalparams.LLOPnumpartitions));
	
	sweepparams.source_partition = source_partition;
	return sweepparams;
}
travstate_t 
	#ifdef SW 
	actslw_maxbutil::
	#endif 
gettravstate(uint512_dt * kvdram, globalparams_t globalparams, step_type currentLOP, batch_type sourcestatsmarker){
	travstate_t travstate;
	keyvalue_t keyvalue;
	keyvalue_t nextkeyvalue;
	
	if(currentLOP == 0){ keyvalue.key = 0; }
	else if(currentLOP == 1){ keyvalue.key = 0; }
	#ifdef _WIDEWORD
	else { keyvalue.key = kvdram[BASEOFFSET_STATSDRAM_KVS + sourcestatsmarker].range(31, 0); 
		   keyvalue.value = kvdram[BASEOFFSET_STATSDRAM_KVS + sourcestatsmarker].range(63, 32); }
	#else 
	else { keyvalue = kvdram[BASEOFFSET_STATSDRAM_KVS + sourcestatsmarker].data[0]; }
	#endif 
	
	if(currentLOP == 0){ nextkeyvalue.key = globalparams.runsize; }
	else if(currentLOP == 1){ nextkeyvalue.key = globalparams.runsize; }
	else { nextkeyvalue.key = keyvalue.key + keyvalue.value; }
		
	travstate.begin_kvs = keyvalue.key / VECTOR_SIZE; 
	// travstate.end_kvs = nextkeyvalue.key / VECTOR_SIZE;
	travstate.end_kvs = (nextkeyvalue.key + (VECTOR_SIZE - 1)) / VECTOR_SIZE; // NEWCHANGE.
	travstate.size_kvs = travstate.end_kvs - travstate.begin_kvs;
	travstate.skip_kvs = SRCBUFFER_SIZE;
	travstate.i_kvs = travstate.begin_kvs;
	return travstate;	
}

// collect globalstats functions 
void 
	#ifdef SW 
	actslw_maxbutil::
	#endif 
readglobalstats00(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[NUM_PARTITIONS], batch_type offset_kvs){
	if(enable == OFF){ return; }
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("readglobalstats0", offset_kvs + NUM_PARTITIONS, BASEOFFSET_STATSDRAM_KVS + KVSTATSDRAMSZ_KVS + 1, NAp, NAp, NAp);
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
	return;
}

void 
	#ifdef SW 
	actslw_maxbutil::
	#endif
collectglobalstats00(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], step_type currentLOP, vertex_t upperlimit, travstate_t travstate, globalparams_t globalparams){					
	if(enable == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE;
	buffer_type chunk_size = getchunksize_kvs(SRCBUFFER_SIZE, travstate, 0);

	COLLECTGLOBALSTATS_LOOP: for(buffer_type i=0; i<chunk_size; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
	#pragma HLS PIPELINE II=3
		keyvalue_t keyvalue0 = sourcebuffer[0][i];
		keyvalue_t keyvalue1 = sourcebuffer[1][i];
		keyvalue_t keyvalue2 = sourcebuffer[2][i];
		keyvalue_t keyvalue3 = sourcebuffer[3][i];
		keyvalue_t keyvalue4 = sourcebuffer[4][i];
		keyvalue_t keyvalue5 = sourcebuffer[5][i];
		keyvalue_t keyvalue6 = sourcebuffer[6][i];
		keyvalue_t keyvalue7 = sourcebuffer[7][i];
		
		partition_type p0 = 0;
		if(keyvalue0.key != INVALIDDATA){ p0 = getpartition(keyvalue0, currentLOP, upperlimit, globalparams.batch_range_pow); }
		partition_type p1 = 0;
		if(keyvalue1.key != INVALIDDATA){ p1 = getpartition(keyvalue1, currentLOP, upperlimit, globalparams.batch_range_pow); }
		partition_type p2 = 0;
		if(keyvalue2.key != INVALIDDATA){ p2 = getpartition(keyvalue2, currentLOP, upperlimit, globalparams.batch_range_pow); }
		partition_type p3 = 0;
		if(keyvalue3.key != INVALIDDATA){ p3 = getpartition(keyvalue3, currentLOP, upperlimit, globalparams.batch_range_pow); }
		partition_type p4 = 0;
		if(keyvalue4.key != INVALIDDATA){ p4 = getpartition(keyvalue4, currentLOP, upperlimit, globalparams.batch_range_pow); }
		partition_type p5 = 0;
		if(keyvalue5.key != INVALIDDATA){ p5 = getpartition(keyvalue5, currentLOP, upperlimit, globalparams.batch_range_pow); }
		partition_type p6 = 0;
		if(keyvalue6.key != INVALIDDATA){ p6 = getpartition(keyvalue6, currentLOP, upperlimit, globalparams.batch_range_pow); }
		partition_type p7 = 0;
		if(keyvalue7.key != INVALIDDATA){ p7 = getpartition(keyvalue7, currentLOP, upperlimit, globalparams.batch_range_pow); }
		
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[0]", p0, NUM_PARTITIONS, keyvalue0.key, globalparams.batch_range, NAp);
		#endif 
		if(keyvalue0.key != INVALIDDATA){ destbuffer[0][p0].value += 1; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[1]", p1, NUM_PARTITIONS, keyvalue1.key, globalparams.batch_range, NAp);
		#endif 
		if(keyvalue1.key != INVALIDDATA){ destbuffer[1][p1].value += 1; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[2]", p2, NUM_PARTITIONS, keyvalue2.key, globalparams.batch_range, NAp);
		#endif 
		if(keyvalue2.key != INVALIDDATA){ destbuffer[2][p2].value += 1; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[3]", p3, NUM_PARTITIONS, keyvalue3.key, globalparams.batch_range, NAp);
		#endif 
		if(keyvalue3.key != INVALIDDATA){ destbuffer[3][p3].value += 1; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[4]", p4, NUM_PARTITIONS, keyvalue4.key, globalparams.batch_range, NAp);
		#endif 
		if(keyvalue4.key != INVALIDDATA){ destbuffer[4][p4].value += 1; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[5]", p5, NUM_PARTITIONS, keyvalue5.key, globalparams.batch_range, NAp);
		#endif 
		if(keyvalue5.key != INVALIDDATA){ destbuffer[5][p5].value += 1; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[6]", p6, NUM_PARTITIONS, keyvalue6.key, globalparams.batch_range, NAp);
		#endif 
		if(keyvalue6.key != INVALIDDATA){ destbuffer[6][p6].value += 1; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[7]", p7, NUM_PARTITIONS, keyvalue7.key, globalparams.batch_range, NAp);
		#endif 
		if(keyvalue7.key != INVALIDDATA){ destbuffer[7][p7].value += 1; }
	}
	return;
}

void 
	#ifdef SW 
	actslw_maxbutil::
	#endif
prepareglobalstats00(bool_type enable, keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t globalstatsbuffer[NUM_PARTITIONS], globalparams_t globalparams){
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
	actslw_maxbutil::
	#endif 
saveglobalstats00(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[NUM_PARTITIONS], batch_type offset_kvs){
	if(enable == OFF){ return; }
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("saveglobalstats0", offset_kvs + NUM_PARTITIONS, BASEOFFSET_STATSDRAM_KVS + KVSTATSDRAMSZ_KVS + 1, offset_kvs, NUM_PARTITIONS, KVSTATSDRAMSZ_KVS);
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
	return;
}

// partition functions
void 
	#ifdef SW 
	actslw_maxbutil::
	#endif 
readkeyvalues00(bool_type enable, uint512_dt * kvdram, keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], batch_type offset_kvs, travstate_t travstate){
	if(enable == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE;
	buffer_type chunk_size = getchunksize_kvs(SRCBUFFER_SIZE, travstate, 0);

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
	cout<<"readkeyvalues0:: keyvalues read: offset: "<<offset_kvs * VECTOR_SIZE<<"-"<<(offset_kvs + chunk_size) * VECTOR_SIZE<<", number of keyvalues read: "<<(chunk_size * VECTOR_SIZE)<<endl;
	// cout<<"readkeyvalues0:: keyvalues read: offset: "<<(offset_kvs - BASEOFFSET_EDGESDATA_KVS) * VECTOR_SIZE<<"-"<<(offset_kvs + chunk_size - BASEOFFSET_EDGESDATA_KVS) * VECTOR_SIZE<<", number of keyvalues read: "<<(chunk_size * VECTOR_SIZE)<<endl;
	#endif
	return;
}

void 
	#ifdef SW 
	actslw_maxbutil::
	#endif
partitionkeyvalues00(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t localcapsule[VECTOR_SIZE][NUM_PARTITIONS], step_type currentLOP, vertex_t upperlimit, travstate_t travstate, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE;
	buffer_type chunk_size = getchunksize_kvs(SRCBUFFER_SIZE, travstate, 0);

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
			if(keyvalue0.key != INVALIDDATA){ p0 = getpartition(keyvalue0, currentLOP, upperlimit, globalparams.batch_range_pow); } 
			partition_type p1 = 0;
			if(keyvalue1.key != INVALIDDATA){ p1 = getpartition(keyvalue1, currentLOP, upperlimit, globalparams.batch_range_pow); } 
			partition_type p2 = 0;
			if(keyvalue2.key != INVALIDDATA){ p2 = getpartition(keyvalue2, currentLOP, upperlimit, globalparams.batch_range_pow); } 
			partition_type p3 = 0;
			if(keyvalue3.key != INVALIDDATA){ p3 = getpartition(keyvalue3, currentLOP, upperlimit, globalparams.batch_range_pow); } 
			partition_type p4 = 0;
			if(keyvalue4.key != INVALIDDATA){ p4 = getpartition(keyvalue4, currentLOP, upperlimit, globalparams.batch_range_pow); } 
			partition_type p5 = 0;
			if(keyvalue5.key != INVALIDDATA){ p5 = getpartition(keyvalue5, currentLOP, upperlimit, globalparams.batch_range_pow); } 
			partition_type p6 = 0;
			if(keyvalue6.key != INVALIDDATA){ p6 = getpartition(keyvalue6, currentLOP, upperlimit, globalparams.batch_range_pow); } 
			partition_type p7 = 0;
			if(keyvalue7.key != INVALIDDATA){ p7 = getpartition(keyvalue7, currentLOP, upperlimit, globalparams.batch_range_pow); } 
			
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
	actslw_maxbutil::
	#endif 
savekeyvalues00(bool_type enable, uint512_dt * kvdram, keyvalue_t buffer[8][PADDEDDESTBUFFER_SIZE], keyvalue_t * globalcapsule, skeyvalue_t localcapsule[NUM_PARTITIONS], batch_type globalbaseaddress_kvs, globalparams_t globalparams){				
	if(enable == OFF){ return; }
	
	#ifdef _DEBUGMODE_CHECKS
	actsutilityobj->printkeyvalues("savekeyvalues0::localcapsule", localcapsule, NUM_PARTITIONS);
	actsutilityobj->printvaluecount("savekeyvalues0::localcapsule", localcapsule, NUM_PARTITIONS);
	actsutilityobj->scankeyvalues("savekeyvalues0::buffer", (keyvalue_t *)buffer, localcapsule, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
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
	actsutilityobj->checkoutofbounds("savekeyvalues0::globalcapsule 34", globalcapsule[NUM_PARTITIONS-1].key + globalcapsule[NUM_PARTITIONS-1].value, KVDRAMSZ, NAp, NAp, NAp);
	#endif
	return;
}

// in-memory partition functions
void 
	#ifdef SW 
	actslw_maxbutil::
	#endif
combineSetof1stoSetof200_I0(bool_type enable, keyvalue_t buffer_setof1M0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof1M1[PADDEDDESTBUFFER_SIZE], 
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
	actsutilityobj->scankeyvalues("combineSetof1stoSetof20_I0::buffer_setof2M", (keyvalue_t *)buffer_setof1M, localcapsuleM, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof1stoSetof20_I0::buffer_setof2", (keyvalue_t *)buffer_setof2, localcapsuleR, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	return;
}
void 
	#ifdef SW 
	actslw_maxbutil::
	#endif
combineSetof1stoSetof200_I1(bool_type enable, keyvalue_t buffer_setof1M0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof1M1[PADDEDDESTBUFFER_SIZE], 
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
	actsutilityobj->scankeyvalues("combineSetof1stoSetof20_I1::buffer_setof2M", (keyvalue_t *)buffer_setof1M, localcapsuleM, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof1stoSetof20_I1::buffer_setof2", (keyvalue_t *)buffer_setof2, localcapsuleR, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	return;
}
void 
	#ifdef SW 
	actslw_maxbutil::
	#endif
combineSetof1stoSetof200_I2(bool_type enable, keyvalue_t buffer_setof1M0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof1M1[PADDEDDESTBUFFER_SIZE], 
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
	actsutilityobj->scankeyvalues("combineSetof1stoSetof20_I2::buffer_setof2M", (keyvalue_t *)buffer_setof1M, localcapsuleM, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof1stoSetof20_I2::buffer_setof2", (keyvalue_t *)buffer_setof2, localcapsuleR, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	return;
}
void 
	#ifdef SW 
	actslw_maxbutil::
	#endif
combineSetof1stoSetof200_I3(bool_type enable, keyvalue_t buffer_setof1M0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof1M1[PADDEDDESTBUFFER_SIZE], 
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
	actsutilityobj->scankeyvalues("combineSetof1stoSetof20_I3::buffer_setof2M", (keyvalue_t *)buffer_setof1M, localcapsuleM, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof1stoSetof20_I3::buffer_setof2", (keyvalue_t *)buffer_setof2, localcapsuleR, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	return;
}
void 
	#ifdef SW 
	actslw_maxbutil::
	#endif
combineSetof2stoSetof400_I0(bool_type enable, keyvalue_t buffer_setof2M0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2M1[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2M2[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2M3[PADDEDDESTBUFFER_SIZE], 
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
	actsutilityobj->scankeyvalues("combineSetof2stoSetof40_I0::buffer_setof2M", (keyvalue_t *)buffer_setof2M, localcapsuleM, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof2stoSetof40_I0::buffer_setof2N", (keyvalue_t *)buffer_setof2N, localcapsuleN, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof2stoSetof40_I0::buffer_setof4", (keyvalue_t *)buffer_setof4, localcapsuleR, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	return;
}
void 
	#ifdef SW 
	actslw_maxbutil::
	#endif
combineSetof2stoSetof400_I1(bool_type enable, keyvalue_t buffer_setof2M0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2M1[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2M2[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2M3[PADDEDDESTBUFFER_SIZE], 
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
	actsutilityobj->scankeyvalues("combineSetof2stoSetof40_I1::buffer_setof2M", (keyvalue_t *)buffer_setof2M, localcapsuleM, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof2stoSetof40_I1::buffer_setof2N", (keyvalue_t *)buffer_setof2N, localcapsuleN, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof2stoSetof40_I1::buffer_setof4", (keyvalue_t *)buffer_setof4, localcapsuleR, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	return;
}
void 
	#ifdef SW 
	actslw_maxbutil::
	#endif
combineSetof4stoSetof800_I0(bool_type enable, keyvalue_t buffer_setof4M0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4M1[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4M2[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4M3[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4M4[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4M5[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4M6[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4M7[PADDEDDESTBUFFER_SIZE],
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
	actsutilityobj->scankeyvalues("combineSetof4stoSetof80_I0::buffer_setof2M", (keyvalue_t *)buffer_setof8, localcapsuleR, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	return;
}

// group functions
void 
	#ifdef SW 
	actslw_maxbutil::
	#endif
combineSetof1stoSetof2s00(bool_type enable, keyvalue_t buffer_setof1[8][PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2[8][PADDEDDESTBUFFER_SIZE], skeyvalue_t templocalcapsule_so1[8][NUM_PARTITIONS], skeyvalue_t templocalcapsule_so2[4][NUM_PARTITIONS], globalparams_t globalparams){
	// #pragma HLS INLINE
	// 1s->2s
	combineSetof1stoSetof200_I0(enable, buffer_setof1[0], buffer_setof1[1], buffer_setof2[0], buffer_setof2[1], templocalcapsule_so1[0], templocalcapsule_so1[1], templocalcapsule_so2[0], globalparams);
	combineSetof1stoSetof200_I1(enable, buffer_setof1[2], buffer_setof1[3], buffer_setof2[2], buffer_setof2[3], templocalcapsule_so1[2], templocalcapsule_so1[3], templocalcapsule_so2[1], globalparams);
	combineSetof1stoSetof200_I2(enable, buffer_setof1[4], buffer_setof1[5], buffer_setof2[4], buffer_setof2[5], templocalcapsule_so1[4], templocalcapsule_so1[5], templocalcapsule_so2[2], globalparams);
	combineSetof1stoSetof200_I3(enable, buffer_setof1[6], buffer_setof1[7], buffer_setof2[6], buffer_setof2[7], templocalcapsule_so1[6], templocalcapsule_so1[7], templocalcapsule_so2[3], globalparams);
	return;
}
void 
	#ifdef SW 
	actslw_maxbutil::
	#endif
combineSetof2stoSetof4s00(bool_type enable, keyvalue_t buffer_setof2[8][PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4[8][PADDEDDESTBUFFER_SIZE], skeyvalue_t templocalcapsule_so2[4][NUM_PARTITIONS], skeyvalue_t templocalcapsule_so4[2][NUM_PARTITIONS], globalparams_t globalparams){
	// #pragma HLS INLINE
	// 2s->4s
	combineSetof2stoSetof400_I0(enable, buffer_setof2[0], buffer_setof2[1], buffer_setof2[2], buffer_setof2[3], buffer_setof4[0], buffer_setof4[1], buffer_setof4[2], buffer_setof4[3], templocalcapsule_so2[0], templocalcapsule_so2[1], templocalcapsule_so4[0], globalparams);
	combineSetof2stoSetof400_I1(enable, buffer_setof2[4], buffer_setof2[5], buffer_setof2[6], buffer_setof2[7], buffer_setof4[4], buffer_setof4[5], buffer_setof4[6], buffer_setof4[7], templocalcapsule_so2[2], templocalcapsule_so2[3], templocalcapsule_so4[1], globalparams);
	return;
}
void 
	#ifdef SW 
	actslw_maxbutil::
	#endif
combineSetof4stoSetof8s00(bool_type enable, keyvalue_t buffer_setof4[8][PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof8[8][PADDEDDESTBUFFER_SIZE], skeyvalue_t templocalcapsule_so4[2][NUM_PARTITIONS], skeyvalue_t templocalcapsule_so8[NUM_PARTITIONS], globalparams_t globalparams){
	// #pragma HLS INLINE
	// 4s->8s
	combineSetof4stoSetof800_I0(enable, buffer_setof4[0], buffer_setof4[1], buffer_setof4[2], buffer_setof4[3], buffer_setof4[4], buffer_setof4[5], buffer_setof4[6], buffer_setof4[7], 
												buffer_setof8[0], buffer_setof8[1], buffer_setof8[2], buffer_setof8[3], buffer_setof8[4], buffer_setof8[5], buffer_setof8[6], buffer_setof8[7], 
													templocalcapsule_so4[0], templocalcapsule_so4[1], templocalcapsule_so8, globalparams);
	return;
}
// collect globalstats functions 
void 
	#ifdef SW 
	actslw_maxbutil::
	#endif 
readglobalstats01(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[NUM_PARTITIONS], batch_type offset_kvs){
	if(enable == OFF){ return; }
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("readglobalstats0", offset_kvs + NUM_PARTITIONS, BASEOFFSET_STATSDRAM_KVS + KVSTATSDRAMSZ_KVS + 1, NAp, NAp, NAp);
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
	return;
}

void 
	#ifdef SW 
	actslw_maxbutil::
	#endif
collectglobalstats01(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], step_type currentLOP, vertex_t upperlimit, travstate_t travstate, globalparams_t globalparams){					
	if(enable == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE;
	buffer_type chunk_size = getchunksize_kvs(SRCBUFFER_SIZE, travstate, 0);

	COLLECTGLOBALSTATS_LOOP: for(buffer_type i=0; i<chunk_size; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
	#pragma HLS PIPELINE II=3
		keyvalue_t keyvalue0 = sourcebuffer[0][i];
		keyvalue_t keyvalue1 = sourcebuffer[1][i];
		keyvalue_t keyvalue2 = sourcebuffer[2][i];
		keyvalue_t keyvalue3 = sourcebuffer[3][i];
		keyvalue_t keyvalue4 = sourcebuffer[4][i];
		keyvalue_t keyvalue5 = sourcebuffer[5][i];
		keyvalue_t keyvalue6 = sourcebuffer[6][i];
		keyvalue_t keyvalue7 = sourcebuffer[7][i];
		
		partition_type p0 = 0;
		if(keyvalue0.key != INVALIDDATA){ p0 = getpartition(keyvalue0, currentLOP, upperlimit, globalparams.batch_range_pow); }
		partition_type p1 = 0;
		if(keyvalue1.key != INVALIDDATA){ p1 = getpartition(keyvalue1, currentLOP, upperlimit, globalparams.batch_range_pow); }
		partition_type p2 = 0;
		if(keyvalue2.key != INVALIDDATA){ p2 = getpartition(keyvalue2, currentLOP, upperlimit, globalparams.batch_range_pow); }
		partition_type p3 = 0;
		if(keyvalue3.key != INVALIDDATA){ p3 = getpartition(keyvalue3, currentLOP, upperlimit, globalparams.batch_range_pow); }
		partition_type p4 = 0;
		if(keyvalue4.key != INVALIDDATA){ p4 = getpartition(keyvalue4, currentLOP, upperlimit, globalparams.batch_range_pow); }
		partition_type p5 = 0;
		if(keyvalue5.key != INVALIDDATA){ p5 = getpartition(keyvalue5, currentLOP, upperlimit, globalparams.batch_range_pow); }
		partition_type p6 = 0;
		if(keyvalue6.key != INVALIDDATA){ p6 = getpartition(keyvalue6, currentLOP, upperlimit, globalparams.batch_range_pow); }
		partition_type p7 = 0;
		if(keyvalue7.key != INVALIDDATA){ p7 = getpartition(keyvalue7, currentLOP, upperlimit, globalparams.batch_range_pow); }
		
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[0]", p0, NUM_PARTITIONS, keyvalue0.key, globalparams.batch_range, NAp);
		#endif 
		if(keyvalue0.key != INVALIDDATA){ destbuffer[0][p0].value += 1; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[1]", p1, NUM_PARTITIONS, keyvalue1.key, globalparams.batch_range, NAp);
		#endif 
		if(keyvalue1.key != INVALIDDATA){ destbuffer[1][p1].value += 1; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[2]", p2, NUM_PARTITIONS, keyvalue2.key, globalparams.batch_range, NAp);
		#endif 
		if(keyvalue2.key != INVALIDDATA){ destbuffer[2][p2].value += 1; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[3]", p3, NUM_PARTITIONS, keyvalue3.key, globalparams.batch_range, NAp);
		#endif 
		if(keyvalue3.key != INVALIDDATA){ destbuffer[3][p3].value += 1; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[4]", p4, NUM_PARTITIONS, keyvalue4.key, globalparams.batch_range, NAp);
		#endif 
		if(keyvalue4.key != INVALIDDATA){ destbuffer[4][p4].value += 1; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[5]", p5, NUM_PARTITIONS, keyvalue5.key, globalparams.batch_range, NAp);
		#endif 
		if(keyvalue5.key != INVALIDDATA){ destbuffer[5][p5].value += 1; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[6]", p6, NUM_PARTITIONS, keyvalue6.key, globalparams.batch_range, NAp);
		#endif 
		if(keyvalue6.key != INVALIDDATA){ destbuffer[6][p6].value += 1; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[7]", p7, NUM_PARTITIONS, keyvalue7.key, globalparams.batch_range, NAp);
		#endif 
		if(keyvalue7.key != INVALIDDATA){ destbuffer[7][p7].value += 1; }
	}
	return;
}

void 
	#ifdef SW 
	actslw_maxbutil::
	#endif
prepareglobalstats01(bool_type enable, keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t globalstatsbuffer[NUM_PARTITIONS], globalparams_t globalparams){
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
	actslw_maxbutil::
	#endif 
saveglobalstats01(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[NUM_PARTITIONS], batch_type offset_kvs){
	if(enable == OFF){ return; }
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("saveglobalstats0", offset_kvs + NUM_PARTITIONS, BASEOFFSET_STATSDRAM_KVS + KVSTATSDRAMSZ_KVS + 1, offset_kvs, NUM_PARTITIONS, KVSTATSDRAMSZ_KVS);
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
	return;
}

// partition functions
void 
	#ifdef SW 
	actslw_maxbutil::
	#endif 
readkeyvalues01(bool_type enable, uint512_dt * kvdram, keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], batch_type offset_kvs, travstate_t travstate){
	if(enable == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE;
	buffer_type chunk_size = getchunksize_kvs(SRCBUFFER_SIZE, travstate, 0);

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
	cout<<"readkeyvalues0:: keyvalues read: offset: "<<offset_kvs * VECTOR_SIZE<<"-"<<(offset_kvs + chunk_size) * VECTOR_SIZE<<", number of keyvalues read: "<<(chunk_size * VECTOR_SIZE)<<endl;
	// cout<<"readkeyvalues0:: keyvalues read: offset: "<<(offset_kvs - BASEOFFSET_EDGESDATA_KVS) * VECTOR_SIZE<<"-"<<(offset_kvs + chunk_size - BASEOFFSET_EDGESDATA_KVS) * VECTOR_SIZE<<", number of keyvalues read: "<<(chunk_size * VECTOR_SIZE)<<endl;
	#endif
	return;
}

void 
	#ifdef SW 
	actslw_maxbutil::
	#endif
partitionkeyvalues01(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t localcapsule[VECTOR_SIZE][NUM_PARTITIONS], step_type currentLOP, vertex_t upperlimit, travstate_t travstate, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE;
	buffer_type chunk_size = getchunksize_kvs(SRCBUFFER_SIZE, travstate, 0);

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
			if(keyvalue0.key != INVALIDDATA){ p0 = getpartition(keyvalue0, currentLOP, upperlimit, globalparams.batch_range_pow); } 
			partition_type p1 = 0;
			if(keyvalue1.key != INVALIDDATA){ p1 = getpartition(keyvalue1, currentLOP, upperlimit, globalparams.batch_range_pow); } 
			partition_type p2 = 0;
			if(keyvalue2.key != INVALIDDATA){ p2 = getpartition(keyvalue2, currentLOP, upperlimit, globalparams.batch_range_pow); } 
			partition_type p3 = 0;
			if(keyvalue3.key != INVALIDDATA){ p3 = getpartition(keyvalue3, currentLOP, upperlimit, globalparams.batch_range_pow); } 
			partition_type p4 = 0;
			if(keyvalue4.key != INVALIDDATA){ p4 = getpartition(keyvalue4, currentLOP, upperlimit, globalparams.batch_range_pow); } 
			partition_type p5 = 0;
			if(keyvalue5.key != INVALIDDATA){ p5 = getpartition(keyvalue5, currentLOP, upperlimit, globalparams.batch_range_pow); } 
			partition_type p6 = 0;
			if(keyvalue6.key != INVALIDDATA){ p6 = getpartition(keyvalue6, currentLOP, upperlimit, globalparams.batch_range_pow); } 
			partition_type p7 = 0;
			if(keyvalue7.key != INVALIDDATA){ p7 = getpartition(keyvalue7, currentLOP, upperlimit, globalparams.batch_range_pow); } 
			
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
	actslw_maxbutil::
	#endif 
savekeyvalues01(bool_type enable, uint512_dt * kvdram, keyvalue_t buffer[8][PADDEDDESTBUFFER_SIZE], keyvalue_t * globalcapsule, skeyvalue_t localcapsule[NUM_PARTITIONS], batch_type globalbaseaddress_kvs, globalparams_t globalparams){				
	if(enable == OFF){ return; }
	
	#ifdef _DEBUGMODE_CHECKS
	actsutilityobj->printkeyvalues("savekeyvalues0::localcapsule", localcapsule, NUM_PARTITIONS);
	actsutilityobj->printvaluecount("savekeyvalues0::localcapsule", localcapsule, NUM_PARTITIONS);
	actsutilityobj->scankeyvalues("savekeyvalues0::buffer", (keyvalue_t *)buffer, localcapsule, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
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
	actsutilityobj->checkoutofbounds("savekeyvalues0::globalcapsule 34", globalcapsule[NUM_PARTITIONS-1].key + globalcapsule[NUM_PARTITIONS-1].value, KVDRAMSZ, NAp, NAp, NAp);
	#endif
	return;
}

// in-memory partition functions
void 
	#ifdef SW 
	actslw_maxbutil::
	#endif
combineSetof1stoSetof201_I0(bool_type enable, keyvalue_t buffer_setof1M0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof1M1[PADDEDDESTBUFFER_SIZE], 
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
	actsutilityobj->scankeyvalues("combineSetof1stoSetof20_I0::buffer_setof2M", (keyvalue_t *)buffer_setof1M, localcapsuleM, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof1stoSetof20_I0::buffer_setof2", (keyvalue_t *)buffer_setof2, localcapsuleR, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	return;
}
void 
	#ifdef SW 
	actslw_maxbutil::
	#endif
combineSetof1stoSetof201_I1(bool_type enable, keyvalue_t buffer_setof1M0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof1M1[PADDEDDESTBUFFER_SIZE], 
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
	actsutilityobj->scankeyvalues("combineSetof1stoSetof20_I1::buffer_setof2M", (keyvalue_t *)buffer_setof1M, localcapsuleM, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof1stoSetof20_I1::buffer_setof2", (keyvalue_t *)buffer_setof2, localcapsuleR, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	return;
}
void 
	#ifdef SW 
	actslw_maxbutil::
	#endif
combineSetof1stoSetof201_I2(bool_type enable, keyvalue_t buffer_setof1M0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof1M1[PADDEDDESTBUFFER_SIZE], 
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
	actsutilityobj->scankeyvalues("combineSetof1stoSetof20_I2::buffer_setof2M", (keyvalue_t *)buffer_setof1M, localcapsuleM, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof1stoSetof20_I2::buffer_setof2", (keyvalue_t *)buffer_setof2, localcapsuleR, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	return;
}
void 
	#ifdef SW 
	actslw_maxbutil::
	#endif
combineSetof1stoSetof201_I3(bool_type enable, keyvalue_t buffer_setof1M0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof1M1[PADDEDDESTBUFFER_SIZE], 
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
	actsutilityobj->scankeyvalues("combineSetof1stoSetof20_I3::buffer_setof2M", (keyvalue_t *)buffer_setof1M, localcapsuleM, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof1stoSetof20_I3::buffer_setof2", (keyvalue_t *)buffer_setof2, localcapsuleR, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	return;
}
void 
	#ifdef SW 
	actslw_maxbutil::
	#endif
combineSetof2stoSetof401_I0(bool_type enable, keyvalue_t buffer_setof2M0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2M1[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2M2[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2M3[PADDEDDESTBUFFER_SIZE], 
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
	actsutilityobj->scankeyvalues("combineSetof2stoSetof40_I0::buffer_setof2M", (keyvalue_t *)buffer_setof2M, localcapsuleM, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof2stoSetof40_I0::buffer_setof2N", (keyvalue_t *)buffer_setof2N, localcapsuleN, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof2stoSetof40_I0::buffer_setof4", (keyvalue_t *)buffer_setof4, localcapsuleR, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	return;
}
void 
	#ifdef SW 
	actslw_maxbutil::
	#endif
combineSetof2stoSetof401_I1(bool_type enable, keyvalue_t buffer_setof2M0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2M1[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2M2[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2M3[PADDEDDESTBUFFER_SIZE], 
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
	actsutilityobj->scankeyvalues("combineSetof2stoSetof40_I1::buffer_setof2M", (keyvalue_t *)buffer_setof2M, localcapsuleM, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof2stoSetof40_I1::buffer_setof2N", (keyvalue_t *)buffer_setof2N, localcapsuleN, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof2stoSetof40_I1::buffer_setof4", (keyvalue_t *)buffer_setof4, localcapsuleR, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	return;
}
void 
	#ifdef SW 
	actslw_maxbutil::
	#endif
combineSetof4stoSetof801_I0(bool_type enable, keyvalue_t buffer_setof4M0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4M1[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4M2[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4M3[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4M4[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4M5[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4M6[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4M7[PADDEDDESTBUFFER_SIZE],
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
	actsutilityobj->scankeyvalues("combineSetof4stoSetof80_I0::buffer_setof2M", (keyvalue_t *)buffer_setof8, localcapsuleR, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	return;
}

// group functions
void 
	#ifdef SW 
	actslw_maxbutil::
	#endif
combineSetof1stoSetof2s01(bool_type enable, keyvalue_t buffer_setof1[8][PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2[8][PADDEDDESTBUFFER_SIZE], skeyvalue_t templocalcapsule_so1[8][NUM_PARTITIONS], skeyvalue_t templocalcapsule_so2[4][NUM_PARTITIONS], globalparams_t globalparams){
	// #pragma HLS INLINE
	// 1s->2s
	combineSetof1stoSetof201_I0(enable, buffer_setof1[0], buffer_setof1[1], buffer_setof2[0], buffer_setof2[1], templocalcapsule_so1[0], templocalcapsule_so1[1], templocalcapsule_so2[0], globalparams);
	combineSetof1stoSetof201_I1(enable, buffer_setof1[2], buffer_setof1[3], buffer_setof2[2], buffer_setof2[3], templocalcapsule_so1[2], templocalcapsule_so1[3], templocalcapsule_so2[1], globalparams);
	combineSetof1stoSetof201_I2(enable, buffer_setof1[4], buffer_setof1[5], buffer_setof2[4], buffer_setof2[5], templocalcapsule_so1[4], templocalcapsule_so1[5], templocalcapsule_so2[2], globalparams);
	combineSetof1stoSetof201_I3(enable, buffer_setof1[6], buffer_setof1[7], buffer_setof2[6], buffer_setof2[7], templocalcapsule_so1[6], templocalcapsule_so1[7], templocalcapsule_so2[3], globalparams);
	return;
}
void 
	#ifdef SW 
	actslw_maxbutil::
	#endif
combineSetof2stoSetof4s01(bool_type enable, keyvalue_t buffer_setof2[8][PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4[8][PADDEDDESTBUFFER_SIZE], skeyvalue_t templocalcapsule_so2[4][NUM_PARTITIONS], skeyvalue_t templocalcapsule_so4[2][NUM_PARTITIONS], globalparams_t globalparams){
	// #pragma HLS INLINE
	// 2s->4s
	combineSetof2stoSetof401_I0(enable, buffer_setof2[0], buffer_setof2[1], buffer_setof2[2], buffer_setof2[3], buffer_setof4[0], buffer_setof4[1], buffer_setof4[2], buffer_setof4[3], templocalcapsule_so2[0], templocalcapsule_so2[1], templocalcapsule_so4[0], globalparams);
	combineSetof2stoSetof401_I1(enable, buffer_setof2[4], buffer_setof2[5], buffer_setof2[6], buffer_setof2[7], buffer_setof4[4], buffer_setof4[5], buffer_setof4[6], buffer_setof4[7], templocalcapsule_so2[2], templocalcapsule_so2[3], templocalcapsule_so4[1], globalparams);
	return;
}
void 
	#ifdef SW 
	actslw_maxbutil::
	#endif
combineSetof4stoSetof8s01(bool_type enable, keyvalue_t buffer_setof4[8][PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof8[8][PADDEDDESTBUFFER_SIZE], skeyvalue_t templocalcapsule_so4[2][NUM_PARTITIONS], skeyvalue_t templocalcapsule_so8[NUM_PARTITIONS], globalparams_t globalparams){
	// #pragma HLS INLINE
	// 4s->8s
	combineSetof4stoSetof801_I0(enable, buffer_setof4[0], buffer_setof4[1], buffer_setof4[2], buffer_setof4[3], buffer_setof4[4], buffer_setof4[5], buffer_setof4[6], buffer_setof4[7], 
												buffer_setof8[0], buffer_setof8[1], buffer_setof8[2], buffer_setof8[3], buffer_setof8[4], buffer_setof8[5], buffer_setof8[6], buffer_setof8[7], 
													templocalcapsule_so4[0], templocalcapsule_so4[1], templocalcapsule_so8, globalparams);
	return;
}
// collect globalstats functions 
void 
	#ifdef SW 
	actslw_maxbutil::
	#endif 
readglobalstats02(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[NUM_PARTITIONS], batch_type offset_kvs){
	if(enable == OFF){ return; }
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("readglobalstats0", offset_kvs + NUM_PARTITIONS, BASEOFFSET_STATSDRAM_KVS + KVSTATSDRAMSZ_KVS + 1, NAp, NAp, NAp);
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
	return;
}

void 
	#ifdef SW 
	actslw_maxbutil::
	#endif
collectglobalstats02(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], step_type currentLOP, vertex_t upperlimit, travstate_t travstate, globalparams_t globalparams){					
	if(enable == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE;
	buffer_type chunk_size = getchunksize_kvs(SRCBUFFER_SIZE, travstate, 0);

	COLLECTGLOBALSTATS_LOOP: for(buffer_type i=0; i<chunk_size; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
	#pragma HLS PIPELINE II=3
		keyvalue_t keyvalue0 = sourcebuffer[0][i];
		keyvalue_t keyvalue1 = sourcebuffer[1][i];
		keyvalue_t keyvalue2 = sourcebuffer[2][i];
		keyvalue_t keyvalue3 = sourcebuffer[3][i];
		keyvalue_t keyvalue4 = sourcebuffer[4][i];
		keyvalue_t keyvalue5 = sourcebuffer[5][i];
		keyvalue_t keyvalue6 = sourcebuffer[6][i];
		keyvalue_t keyvalue7 = sourcebuffer[7][i];
		
		partition_type p0 = 0;
		if(keyvalue0.key != INVALIDDATA){ p0 = getpartition(keyvalue0, currentLOP, upperlimit, globalparams.batch_range_pow); }
		partition_type p1 = 0;
		if(keyvalue1.key != INVALIDDATA){ p1 = getpartition(keyvalue1, currentLOP, upperlimit, globalparams.batch_range_pow); }
		partition_type p2 = 0;
		if(keyvalue2.key != INVALIDDATA){ p2 = getpartition(keyvalue2, currentLOP, upperlimit, globalparams.batch_range_pow); }
		partition_type p3 = 0;
		if(keyvalue3.key != INVALIDDATA){ p3 = getpartition(keyvalue3, currentLOP, upperlimit, globalparams.batch_range_pow); }
		partition_type p4 = 0;
		if(keyvalue4.key != INVALIDDATA){ p4 = getpartition(keyvalue4, currentLOP, upperlimit, globalparams.batch_range_pow); }
		partition_type p5 = 0;
		if(keyvalue5.key != INVALIDDATA){ p5 = getpartition(keyvalue5, currentLOP, upperlimit, globalparams.batch_range_pow); }
		partition_type p6 = 0;
		if(keyvalue6.key != INVALIDDATA){ p6 = getpartition(keyvalue6, currentLOP, upperlimit, globalparams.batch_range_pow); }
		partition_type p7 = 0;
		if(keyvalue7.key != INVALIDDATA){ p7 = getpartition(keyvalue7, currentLOP, upperlimit, globalparams.batch_range_pow); }
		
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[0]", p0, NUM_PARTITIONS, keyvalue0.key, globalparams.batch_range, NAp);
		#endif 
		if(keyvalue0.key != INVALIDDATA){ destbuffer[0][p0].value += 1; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[1]", p1, NUM_PARTITIONS, keyvalue1.key, globalparams.batch_range, NAp);
		#endif 
		if(keyvalue1.key != INVALIDDATA){ destbuffer[1][p1].value += 1; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[2]", p2, NUM_PARTITIONS, keyvalue2.key, globalparams.batch_range, NAp);
		#endif 
		if(keyvalue2.key != INVALIDDATA){ destbuffer[2][p2].value += 1; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[3]", p3, NUM_PARTITIONS, keyvalue3.key, globalparams.batch_range, NAp);
		#endif 
		if(keyvalue3.key != INVALIDDATA){ destbuffer[3][p3].value += 1; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[4]", p4, NUM_PARTITIONS, keyvalue4.key, globalparams.batch_range, NAp);
		#endif 
		if(keyvalue4.key != INVALIDDATA){ destbuffer[4][p4].value += 1; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[5]", p5, NUM_PARTITIONS, keyvalue5.key, globalparams.batch_range, NAp);
		#endif 
		if(keyvalue5.key != INVALIDDATA){ destbuffer[5][p5].value += 1; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[6]", p6, NUM_PARTITIONS, keyvalue6.key, globalparams.batch_range, NAp);
		#endif 
		if(keyvalue6.key != INVALIDDATA){ destbuffer[6][p6].value += 1; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[7]", p7, NUM_PARTITIONS, keyvalue7.key, globalparams.batch_range, NAp);
		#endif 
		if(keyvalue7.key != INVALIDDATA){ destbuffer[7][p7].value += 1; }
	}
	return;
}

void 
	#ifdef SW 
	actslw_maxbutil::
	#endif
prepareglobalstats02(bool_type enable, keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t globalstatsbuffer[NUM_PARTITIONS], globalparams_t globalparams){
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
	actslw_maxbutil::
	#endif 
saveglobalstats02(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[NUM_PARTITIONS], batch_type offset_kvs){
	if(enable == OFF){ return; }
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("saveglobalstats0", offset_kvs + NUM_PARTITIONS, BASEOFFSET_STATSDRAM_KVS + KVSTATSDRAMSZ_KVS + 1, offset_kvs, NUM_PARTITIONS, KVSTATSDRAMSZ_KVS);
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
	return;
}

// partition functions
void 
	#ifdef SW 
	actslw_maxbutil::
	#endif 
readkeyvalues02(bool_type enable, uint512_dt * kvdram, keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], batch_type offset_kvs, travstate_t travstate){
	if(enable == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE;
	buffer_type chunk_size = getchunksize_kvs(SRCBUFFER_SIZE, travstate, 0);

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
	cout<<"readkeyvalues0:: keyvalues read: offset: "<<offset_kvs * VECTOR_SIZE<<"-"<<(offset_kvs + chunk_size) * VECTOR_SIZE<<", number of keyvalues read: "<<(chunk_size * VECTOR_SIZE)<<endl;
	// cout<<"readkeyvalues0:: keyvalues read: offset: "<<(offset_kvs - BASEOFFSET_EDGESDATA_KVS) * VECTOR_SIZE<<"-"<<(offset_kvs + chunk_size - BASEOFFSET_EDGESDATA_KVS) * VECTOR_SIZE<<", number of keyvalues read: "<<(chunk_size * VECTOR_SIZE)<<endl;
	#endif
	return;
}

void 
	#ifdef SW 
	actslw_maxbutil::
	#endif
partitionkeyvalues02(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t localcapsule[VECTOR_SIZE][NUM_PARTITIONS], step_type currentLOP, vertex_t upperlimit, travstate_t travstate, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE;
	buffer_type chunk_size = getchunksize_kvs(SRCBUFFER_SIZE, travstate, 0);

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
			if(keyvalue0.key != INVALIDDATA){ p0 = getpartition(keyvalue0, currentLOP, upperlimit, globalparams.batch_range_pow); } 
			partition_type p1 = 0;
			if(keyvalue1.key != INVALIDDATA){ p1 = getpartition(keyvalue1, currentLOP, upperlimit, globalparams.batch_range_pow); } 
			partition_type p2 = 0;
			if(keyvalue2.key != INVALIDDATA){ p2 = getpartition(keyvalue2, currentLOP, upperlimit, globalparams.batch_range_pow); } 
			partition_type p3 = 0;
			if(keyvalue3.key != INVALIDDATA){ p3 = getpartition(keyvalue3, currentLOP, upperlimit, globalparams.batch_range_pow); } 
			partition_type p4 = 0;
			if(keyvalue4.key != INVALIDDATA){ p4 = getpartition(keyvalue4, currentLOP, upperlimit, globalparams.batch_range_pow); } 
			partition_type p5 = 0;
			if(keyvalue5.key != INVALIDDATA){ p5 = getpartition(keyvalue5, currentLOP, upperlimit, globalparams.batch_range_pow); } 
			partition_type p6 = 0;
			if(keyvalue6.key != INVALIDDATA){ p6 = getpartition(keyvalue6, currentLOP, upperlimit, globalparams.batch_range_pow); } 
			partition_type p7 = 0;
			if(keyvalue7.key != INVALIDDATA){ p7 = getpartition(keyvalue7, currentLOP, upperlimit, globalparams.batch_range_pow); } 
			
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
	actslw_maxbutil::
	#endif 
savekeyvalues02(bool_type enable, uint512_dt * kvdram, keyvalue_t buffer[8][PADDEDDESTBUFFER_SIZE], keyvalue_t * globalcapsule, skeyvalue_t localcapsule[NUM_PARTITIONS], batch_type globalbaseaddress_kvs, globalparams_t globalparams){				
	if(enable == OFF){ return; }
	
	#ifdef _DEBUGMODE_CHECKS
	actsutilityobj->printkeyvalues("savekeyvalues0::localcapsule", localcapsule, NUM_PARTITIONS);
	actsutilityobj->printvaluecount("savekeyvalues0::localcapsule", localcapsule, NUM_PARTITIONS);
	actsutilityobj->scankeyvalues("savekeyvalues0::buffer", (keyvalue_t *)buffer, localcapsule, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
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
	actsutilityobj->checkoutofbounds("savekeyvalues0::globalcapsule 34", globalcapsule[NUM_PARTITIONS-1].key + globalcapsule[NUM_PARTITIONS-1].value, KVDRAMSZ, NAp, NAp, NAp);
	#endif
	return;
}

// in-memory partition functions
void 
	#ifdef SW 
	actslw_maxbutil::
	#endif
combineSetof1stoSetof202_I0(bool_type enable, keyvalue_t buffer_setof1M0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof1M1[PADDEDDESTBUFFER_SIZE], 
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
	actsutilityobj->scankeyvalues("combineSetof1stoSetof20_I0::buffer_setof2M", (keyvalue_t *)buffer_setof1M, localcapsuleM, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof1stoSetof20_I0::buffer_setof2", (keyvalue_t *)buffer_setof2, localcapsuleR, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	return;
}
void 
	#ifdef SW 
	actslw_maxbutil::
	#endif
combineSetof1stoSetof202_I1(bool_type enable, keyvalue_t buffer_setof1M0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof1M1[PADDEDDESTBUFFER_SIZE], 
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
	actsutilityobj->scankeyvalues("combineSetof1stoSetof20_I1::buffer_setof2M", (keyvalue_t *)buffer_setof1M, localcapsuleM, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof1stoSetof20_I1::buffer_setof2", (keyvalue_t *)buffer_setof2, localcapsuleR, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	return;
}
void 
	#ifdef SW 
	actslw_maxbutil::
	#endif
combineSetof1stoSetof202_I2(bool_type enable, keyvalue_t buffer_setof1M0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof1M1[PADDEDDESTBUFFER_SIZE], 
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
	actsutilityobj->scankeyvalues("combineSetof1stoSetof20_I2::buffer_setof2M", (keyvalue_t *)buffer_setof1M, localcapsuleM, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof1stoSetof20_I2::buffer_setof2", (keyvalue_t *)buffer_setof2, localcapsuleR, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	return;
}
void 
	#ifdef SW 
	actslw_maxbutil::
	#endif
combineSetof1stoSetof202_I3(bool_type enable, keyvalue_t buffer_setof1M0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof1M1[PADDEDDESTBUFFER_SIZE], 
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
	actsutilityobj->scankeyvalues("combineSetof1stoSetof20_I3::buffer_setof2M", (keyvalue_t *)buffer_setof1M, localcapsuleM, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof1stoSetof20_I3::buffer_setof2", (keyvalue_t *)buffer_setof2, localcapsuleR, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	return;
}
void 
	#ifdef SW 
	actslw_maxbutil::
	#endif
combineSetof2stoSetof402_I0(bool_type enable, keyvalue_t buffer_setof2M0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2M1[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2M2[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2M3[PADDEDDESTBUFFER_SIZE], 
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
	actsutilityobj->scankeyvalues("combineSetof2stoSetof40_I0::buffer_setof2M", (keyvalue_t *)buffer_setof2M, localcapsuleM, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof2stoSetof40_I0::buffer_setof2N", (keyvalue_t *)buffer_setof2N, localcapsuleN, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof2stoSetof40_I0::buffer_setof4", (keyvalue_t *)buffer_setof4, localcapsuleR, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	return;
}
void 
	#ifdef SW 
	actslw_maxbutil::
	#endif
combineSetof2stoSetof402_I1(bool_type enable, keyvalue_t buffer_setof2M0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2M1[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2M2[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2M3[PADDEDDESTBUFFER_SIZE], 
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
	actsutilityobj->scankeyvalues("combineSetof2stoSetof40_I1::buffer_setof2M", (keyvalue_t *)buffer_setof2M, localcapsuleM, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof2stoSetof40_I1::buffer_setof2N", (keyvalue_t *)buffer_setof2N, localcapsuleN, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof2stoSetof40_I1::buffer_setof4", (keyvalue_t *)buffer_setof4, localcapsuleR, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	return;
}
void 
	#ifdef SW 
	actslw_maxbutil::
	#endif
combineSetof4stoSetof802_I0(bool_type enable, keyvalue_t buffer_setof4M0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4M1[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4M2[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4M3[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4M4[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4M5[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4M6[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4M7[PADDEDDESTBUFFER_SIZE],
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
	actsutilityobj->scankeyvalues("combineSetof4stoSetof80_I0::buffer_setof2M", (keyvalue_t *)buffer_setof8, localcapsuleR, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	return;
}

// group functions
void 
	#ifdef SW 
	actslw_maxbutil::
	#endif
combineSetof1stoSetof2s02(bool_type enable, keyvalue_t buffer_setof1[8][PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2[8][PADDEDDESTBUFFER_SIZE], skeyvalue_t templocalcapsule_so1[8][NUM_PARTITIONS], skeyvalue_t templocalcapsule_so2[4][NUM_PARTITIONS], globalparams_t globalparams){
	// #pragma HLS INLINE
	// 1s->2s
	combineSetof1stoSetof202_I0(enable, buffer_setof1[0], buffer_setof1[1], buffer_setof2[0], buffer_setof2[1], templocalcapsule_so1[0], templocalcapsule_so1[1], templocalcapsule_so2[0], globalparams);
	combineSetof1stoSetof202_I1(enable, buffer_setof1[2], buffer_setof1[3], buffer_setof2[2], buffer_setof2[3], templocalcapsule_so1[2], templocalcapsule_so1[3], templocalcapsule_so2[1], globalparams);
	combineSetof1stoSetof202_I2(enable, buffer_setof1[4], buffer_setof1[5], buffer_setof2[4], buffer_setof2[5], templocalcapsule_so1[4], templocalcapsule_so1[5], templocalcapsule_so2[2], globalparams);
	combineSetof1stoSetof202_I3(enable, buffer_setof1[6], buffer_setof1[7], buffer_setof2[6], buffer_setof2[7], templocalcapsule_so1[6], templocalcapsule_so1[7], templocalcapsule_so2[3], globalparams);
	return;
}
void 
	#ifdef SW 
	actslw_maxbutil::
	#endif
combineSetof2stoSetof4s02(bool_type enable, keyvalue_t buffer_setof2[8][PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4[8][PADDEDDESTBUFFER_SIZE], skeyvalue_t templocalcapsule_so2[4][NUM_PARTITIONS], skeyvalue_t templocalcapsule_so4[2][NUM_PARTITIONS], globalparams_t globalparams){
	// #pragma HLS INLINE
	// 2s->4s
	combineSetof2stoSetof402_I0(enable, buffer_setof2[0], buffer_setof2[1], buffer_setof2[2], buffer_setof2[3], buffer_setof4[0], buffer_setof4[1], buffer_setof4[2], buffer_setof4[3], templocalcapsule_so2[0], templocalcapsule_so2[1], templocalcapsule_so4[0], globalparams);
	combineSetof2stoSetof402_I1(enable, buffer_setof2[4], buffer_setof2[5], buffer_setof2[6], buffer_setof2[7], buffer_setof4[4], buffer_setof4[5], buffer_setof4[6], buffer_setof4[7], templocalcapsule_so2[2], templocalcapsule_so2[3], templocalcapsule_so4[1], globalparams);
	return;
}
void 
	#ifdef SW 
	actslw_maxbutil::
	#endif
combineSetof4stoSetof8s02(bool_type enable, keyvalue_t buffer_setof4[8][PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof8[8][PADDEDDESTBUFFER_SIZE], skeyvalue_t templocalcapsule_so4[2][NUM_PARTITIONS], skeyvalue_t templocalcapsule_so8[NUM_PARTITIONS], globalparams_t globalparams){
	// #pragma HLS INLINE
	// 4s->8s
	combineSetof4stoSetof802_I0(enable, buffer_setof4[0], buffer_setof4[1], buffer_setof4[2], buffer_setof4[3], buffer_setof4[4], buffer_setof4[5], buffer_setof4[6], buffer_setof4[7], 
												buffer_setof8[0], buffer_setof8[1], buffer_setof8[2], buffer_setof8[3], buffer_setof8[4], buffer_setof8[5], buffer_setof8[6], buffer_setof8[7], 
													templocalcapsule_so4[0], templocalcapsule_so4[1], templocalcapsule_so8, globalparams);
	return;
}
// collect globalstats functions 
void 
	#ifdef SW 
	actslw_maxbutil::
	#endif 
readglobalstats03(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[NUM_PARTITIONS], batch_type offset_kvs){
	if(enable == OFF){ return; }
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("readglobalstats0", offset_kvs + NUM_PARTITIONS, BASEOFFSET_STATSDRAM_KVS + KVSTATSDRAMSZ_KVS + 1, NAp, NAp, NAp);
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
	return;
}

void 
	#ifdef SW 
	actslw_maxbutil::
	#endif
collectglobalstats03(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], step_type currentLOP, vertex_t upperlimit, travstate_t travstate, globalparams_t globalparams){					
	if(enable == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE;
	buffer_type chunk_size = getchunksize_kvs(SRCBUFFER_SIZE, travstate, 0);

	COLLECTGLOBALSTATS_LOOP: for(buffer_type i=0; i<chunk_size; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
	#pragma HLS PIPELINE II=3
		keyvalue_t keyvalue0 = sourcebuffer[0][i];
		keyvalue_t keyvalue1 = sourcebuffer[1][i];
		keyvalue_t keyvalue2 = sourcebuffer[2][i];
		keyvalue_t keyvalue3 = sourcebuffer[3][i];
		keyvalue_t keyvalue4 = sourcebuffer[4][i];
		keyvalue_t keyvalue5 = sourcebuffer[5][i];
		keyvalue_t keyvalue6 = sourcebuffer[6][i];
		keyvalue_t keyvalue7 = sourcebuffer[7][i];
		
		partition_type p0 = 0;
		if(keyvalue0.key != INVALIDDATA){ p0 = getpartition(keyvalue0, currentLOP, upperlimit, globalparams.batch_range_pow); }
		partition_type p1 = 0;
		if(keyvalue1.key != INVALIDDATA){ p1 = getpartition(keyvalue1, currentLOP, upperlimit, globalparams.batch_range_pow); }
		partition_type p2 = 0;
		if(keyvalue2.key != INVALIDDATA){ p2 = getpartition(keyvalue2, currentLOP, upperlimit, globalparams.batch_range_pow); }
		partition_type p3 = 0;
		if(keyvalue3.key != INVALIDDATA){ p3 = getpartition(keyvalue3, currentLOP, upperlimit, globalparams.batch_range_pow); }
		partition_type p4 = 0;
		if(keyvalue4.key != INVALIDDATA){ p4 = getpartition(keyvalue4, currentLOP, upperlimit, globalparams.batch_range_pow); }
		partition_type p5 = 0;
		if(keyvalue5.key != INVALIDDATA){ p5 = getpartition(keyvalue5, currentLOP, upperlimit, globalparams.batch_range_pow); }
		partition_type p6 = 0;
		if(keyvalue6.key != INVALIDDATA){ p6 = getpartition(keyvalue6, currentLOP, upperlimit, globalparams.batch_range_pow); }
		partition_type p7 = 0;
		if(keyvalue7.key != INVALIDDATA){ p7 = getpartition(keyvalue7, currentLOP, upperlimit, globalparams.batch_range_pow); }
		
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[0]", p0, NUM_PARTITIONS, keyvalue0.key, globalparams.batch_range, NAp);
		#endif 
		if(keyvalue0.key != INVALIDDATA){ destbuffer[0][p0].value += 1; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[1]", p1, NUM_PARTITIONS, keyvalue1.key, globalparams.batch_range, NAp);
		#endif 
		if(keyvalue1.key != INVALIDDATA){ destbuffer[1][p1].value += 1; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[2]", p2, NUM_PARTITIONS, keyvalue2.key, globalparams.batch_range, NAp);
		#endif 
		if(keyvalue2.key != INVALIDDATA){ destbuffer[2][p2].value += 1; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[3]", p3, NUM_PARTITIONS, keyvalue3.key, globalparams.batch_range, NAp);
		#endif 
		if(keyvalue3.key != INVALIDDATA){ destbuffer[3][p3].value += 1; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[4]", p4, NUM_PARTITIONS, keyvalue4.key, globalparams.batch_range, NAp);
		#endif 
		if(keyvalue4.key != INVALIDDATA){ destbuffer[4][p4].value += 1; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[5]", p5, NUM_PARTITIONS, keyvalue5.key, globalparams.batch_range, NAp);
		#endif 
		if(keyvalue5.key != INVALIDDATA){ destbuffer[5][p5].value += 1; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[6]", p6, NUM_PARTITIONS, keyvalue6.key, globalparams.batch_range, NAp);
		#endif 
		if(keyvalue6.key != INVALIDDATA){ destbuffer[6][p6].value += 1; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[7]", p7, NUM_PARTITIONS, keyvalue7.key, globalparams.batch_range, NAp);
		#endif 
		if(keyvalue7.key != INVALIDDATA){ destbuffer[7][p7].value += 1; }
	}
	return;
}

void 
	#ifdef SW 
	actslw_maxbutil::
	#endif
prepareglobalstats03(bool_type enable, keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t globalstatsbuffer[NUM_PARTITIONS], globalparams_t globalparams){
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
	actslw_maxbutil::
	#endif 
saveglobalstats03(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[NUM_PARTITIONS], batch_type offset_kvs){
	if(enable == OFF){ return; }
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("saveglobalstats0", offset_kvs + NUM_PARTITIONS, BASEOFFSET_STATSDRAM_KVS + KVSTATSDRAMSZ_KVS + 1, offset_kvs, NUM_PARTITIONS, KVSTATSDRAMSZ_KVS);
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
	return;
}

// partition functions
void 
	#ifdef SW 
	actslw_maxbutil::
	#endif 
readkeyvalues03(bool_type enable, uint512_dt * kvdram, keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], batch_type offset_kvs, travstate_t travstate){
	if(enable == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE;
	buffer_type chunk_size = getchunksize_kvs(SRCBUFFER_SIZE, travstate, 0);

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
	cout<<"readkeyvalues0:: keyvalues read: offset: "<<offset_kvs * VECTOR_SIZE<<"-"<<(offset_kvs + chunk_size) * VECTOR_SIZE<<", number of keyvalues read: "<<(chunk_size * VECTOR_SIZE)<<endl;
	// cout<<"readkeyvalues0:: keyvalues read: offset: "<<(offset_kvs - BASEOFFSET_EDGESDATA_KVS) * VECTOR_SIZE<<"-"<<(offset_kvs + chunk_size - BASEOFFSET_EDGESDATA_KVS) * VECTOR_SIZE<<", number of keyvalues read: "<<(chunk_size * VECTOR_SIZE)<<endl;
	#endif
	return;
}

void 
	#ifdef SW 
	actslw_maxbutil::
	#endif
partitionkeyvalues03(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t localcapsule[VECTOR_SIZE][NUM_PARTITIONS], step_type currentLOP, vertex_t upperlimit, travstate_t travstate, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE;
	buffer_type chunk_size = getchunksize_kvs(SRCBUFFER_SIZE, travstate, 0);

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
			if(keyvalue0.key != INVALIDDATA){ p0 = getpartition(keyvalue0, currentLOP, upperlimit, globalparams.batch_range_pow); } 
			partition_type p1 = 0;
			if(keyvalue1.key != INVALIDDATA){ p1 = getpartition(keyvalue1, currentLOP, upperlimit, globalparams.batch_range_pow); } 
			partition_type p2 = 0;
			if(keyvalue2.key != INVALIDDATA){ p2 = getpartition(keyvalue2, currentLOP, upperlimit, globalparams.batch_range_pow); } 
			partition_type p3 = 0;
			if(keyvalue3.key != INVALIDDATA){ p3 = getpartition(keyvalue3, currentLOP, upperlimit, globalparams.batch_range_pow); } 
			partition_type p4 = 0;
			if(keyvalue4.key != INVALIDDATA){ p4 = getpartition(keyvalue4, currentLOP, upperlimit, globalparams.batch_range_pow); } 
			partition_type p5 = 0;
			if(keyvalue5.key != INVALIDDATA){ p5 = getpartition(keyvalue5, currentLOP, upperlimit, globalparams.batch_range_pow); } 
			partition_type p6 = 0;
			if(keyvalue6.key != INVALIDDATA){ p6 = getpartition(keyvalue6, currentLOP, upperlimit, globalparams.batch_range_pow); } 
			partition_type p7 = 0;
			if(keyvalue7.key != INVALIDDATA){ p7 = getpartition(keyvalue7, currentLOP, upperlimit, globalparams.batch_range_pow); } 
			
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
	actslw_maxbutil::
	#endif 
savekeyvalues03(bool_type enable, uint512_dt * kvdram, keyvalue_t buffer[8][PADDEDDESTBUFFER_SIZE], keyvalue_t * globalcapsule, skeyvalue_t localcapsule[NUM_PARTITIONS], batch_type globalbaseaddress_kvs, globalparams_t globalparams){				
	if(enable == OFF){ return; }
	
	#ifdef _DEBUGMODE_CHECKS
	actsutilityobj->printkeyvalues("savekeyvalues0::localcapsule", localcapsule, NUM_PARTITIONS);
	actsutilityobj->printvaluecount("savekeyvalues0::localcapsule", localcapsule, NUM_PARTITIONS);
	actsutilityobj->scankeyvalues("savekeyvalues0::buffer", (keyvalue_t *)buffer, localcapsule, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
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
	actsutilityobj->checkoutofbounds("savekeyvalues0::globalcapsule 34", globalcapsule[NUM_PARTITIONS-1].key + globalcapsule[NUM_PARTITIONS-1].value, KVDRAMSZ, NAp, NAp, NAp);
	#endif
	return;
}

// in-memory partition functions
void 
	#ifdef SW 
	actslw_maxbutil::
	#endif
combineSetof1stoSetof203_I0(bool_type enable, keyvalue_t buffer_setof1M0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof1M1[PADDEDDESTBUFFER_SIZE], 
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
	actsutilityobj->scankeyvalues("combineSetof1stoSetof20_I0::buffer_setof2M", (keyvalue_t *)buffer_setof1M, localcapsuleM, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof1stoSetof20_I0::buffer_setof2", (keyvalue_t *)buffer_setof2, localcapsuleR, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	return;
}
void 
	#ifdef SW 
	actslw_maxbutil::
	#endif
combineSetof1stoSetof203_I1(bool_type enable, keyvalue_t buffer_setof1M0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof1M1[PADDEDDESTBUFFER_SIZE], 
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
	actsutilityobj->scankeyvalues("combineSetof1stoSetof20_I1::buffer_setof2M", (keyvalue_t *)buffer_setof1M, localcapsuleM, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof1stoSetof20_I1::buffer_setof2", (keyvalue_t *)buffer_setof2, localcapsuleR, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	return;
}
void 
	#ifdef SW 
	actslw_maxbutil::
	#endif
combineSetof1stoSetof203_I2(bool_type enable, keyvalue_t buffer_setof1M0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof1M1[PADDEDDESTBUFFER_SIZE], 
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
	actsutilityobj->scankeyvalues("combineSetof1stoSetof20_I2::buffer_setof2M", (keyvalue_t *)buffer_setof1M, localcapsuleM, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof1stoSetof20_I2::buffer_setof2", (keyvalue_t *)buffer_setof2, localcapsuleR, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	return;
}
void 
	#ifdef SW 
	actslw_maxbutil::
	#endif
combineSetof1stoSetof203_I3(bool_type enable, keyvalue_t buffer_setof1M0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof1M1[PADDEDDESTBUFFER_SIZE], 
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
	actsutilityobj->scankeyvalues("combineSetof1stoSetof20_I3::buffer_setof2M", (keyvalue_t *)buffer_setof1M, localcapsuleM, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof1stoSetof20_I3::buffer_setof2", (keyvalue_t *)buffer_setof2, localcapsuleR, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	return;
}
void 
	#ifdef SW 
	actslw_maxbutil::
	#endif
combineSetof2stoSetof403_I0(bool_type enable, keyvalue_t buffer_setof2M0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2M1[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2M2[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2M3[PADDEDDESTBUFFER_SIZE], 
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
	actsutilityobj->scankeyvalues("combineSetof2stoSetof40_I0::buffer_setof2M", (keyvalue_t *)buffer_setof2M, localcapsuleM, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof2stoSetof40_I0::buffer_setof2N", (keyvalue_t *)buffer_setof2N, localcapsuleN, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof2stoSetof40_I0::buffer_setof4", (keyvalue_t *)buffer_setof4, localcapsuleR, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	return;
}
void 
	#ifdef SW 
	actslw_maxbutil::
	#endif
combineSetof2stoSetof403_I1(bool_type enable, keyvalue_t buffer_setof2M0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2M1[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2M2[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2M3[PADDEDDESTBUFFER_SIZE], 
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
	actsutilityobj->scankeyvalues("combineSetof2stoSetof40_I1::buffer_setof2M", (keyvalue_t *)buffer_setof2M, localcapsuleM, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof2stoSetof40_I1::buffer_setof2N", (keyvalue_t *)buffer_setof2N, localcapsuleN, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof2stoSetof40_I1::buffer_setof4", (keyvalue_t *)buffer_setof4, localcapsuleR, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	return;
}
void 
	#ifdef SW 
	actslw_maxbutil::
	#endif
combineSetof4stoSetof803_I0(bool_type enable, keyvalue_t buffer_setof4M0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4M1[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4M2[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4M3[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4M4[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4M5[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4M6[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4M7[PADDEDDESTBUFFER_SIZE],
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
	actsutilityobj->scankeyvalues("combineSetof4stoSetof80_I0::buffer_setof2M", (keyvalue_t *)buffer_setof8, localcapsuleR, NUM_PARTITIONS, globalparams.batch_range / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	return;
}

// group functions
void 
	#ifdef SW 
	actslw_maxbutil::
	#endif
combineSetof1stoSetof2s03(bool_type enable, keyvalue_t buffer_setof1[8][PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2[8][PADDEDDESTBUFFER_SIZE], skeyvalue_t templocalcapsule_so1[8][NUM_PARTITIONS], skeyvalue_t templocalcapsule_so2[4][NUM_PARTITIONS], globalparams_t globalparams){
	// #pragma HLS INLINE
	// 1s->2s
	combineSetof1stoSetof203_I0(enable, buffer_setof1[0], buffer_setof1[1], buffer_setof2[0], buffer_setof2[1], templocalcapsule_so1[0], templocalcapsule_so1[1], templocalcapsule_so2[0], globalparams);
	combineSetof1stoSetof203_I1(enable, buffer_setof1[2], buffer_setof1[3], buffer_setof2[2], buffer_setof2[3], templocalcapsule_so1[2], templocalcapsule_so1[3], templocalcapsule_so2[1], globalparams);
	combineSetof1stoSetof203_I2(enable, buffer_setof1[4], buffer_setof1[5], buffer_setof2[4], buffer_setof2[5], templocalcapsule_so1[4], templocalcapsule_so1[5], templocalcapsule_so2[2], globalparams);
	combineSetof1stoSetof203_I3(enable, buffer_setof1[6], buffer_setof1[7], buffer_setof2[6], buffer_setof2[7], templocalcapsule_so1[6], templocalcapsule_so1[7], templocalcapsule_so2[3], globalparams);
	return;
}
void 
	#ifdef SW 
	actslw_maxbutil::
	#endif
combineSetof2stoSetof4s03(bool_type enable, keyvalue_t buffer_setof2[8][PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4[8][PADDEDDESTBUFFER_SIZE], skeyvalue_t templocalcapsule_so2[4][NUM_PARTITIONS], skeyvalue_t templocalcapsule_so4[2][NUM_PARTITIONS], globalparams_t globalparams){
	// #pragma HLS INLINE
	// 2s->4s
	combineSetof2stoSetof403_I0(enable, buffer_setof2[0], buffer_setof2[1], buffer_setof2[2], buffer_setof2[3], buffer_setof4[0], buffer_setof4[1], buffer_setof4[2], buffer_setof4[3], templocalcapsule_so2[0], templocalcapsule_so2[1], templocalcapsule_so4[0], globalparams);
	combineSetof2stoSetof403_I1(enable, buffer_setof2[4], buffer_setof2[5], buffer_setof2[6], buffer_setof2[7], buffer_setof4[4], buffer_setof4[5], buffer_setof4[6], buffer_setof4[7], templocalcapsule_so2[2], templocalcapsule_so2[3], templocalcapsule_so4[1], globalparams);
	return;
}
void 
	#ifdef SW 
	actslw_maxbutil::
	#endif
combineSetof4stoSetof8s03(bool_type enable, keyvalue_t buffer_setof4[8][PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof8[8][PADDEDDESTBUFFER_SIZE], skeyvalue_t templocalcapsule_so4[2][NUM_PARTITIONS], skeyvalue_t templocalcapsule_so8[NUM_PARTITIONS], globalparams_t globalparams){
	// #pragma HLS INLINE
	// 4s->8s
	combineSetof4stoSetof803_I0(enable, buffer_setof4[0], buffer_setof4[1], buffer_setof4[2], buffer_setof4[3], buffer_setof4[4], buffer_setof4[5], buffer_setof4[6], buffer_setof4[7], 
												buffer_setof8[0], buffer_setof8[1], buffer_setof8[2], buffer_setof8[3], buffer_setof8[4], buffer_setof8[5], buffer_setof8[6], buffer_setof8[7], 
													templocalcapsule_so4[0], templocalcapsule_so4[1], templocalcapsule_so8, globalparams);
	return;
}

// main function
void 
	#ifdef SW 
	actslw_maxbutil::
	#endif 
maxdispatch0( uint512_dt * kvdram0  ,uint512_dt * kvdram1  ,uint512_dt * kvdram2  ,uint512_dt * kvdram3 ){
	analysis_type analysis_collectstatsloop = KVDATA_BATCHSIZE_KVS / (SRCBUFFER_SIZE * 1); // NUMSUBWORKERS
	analysis_type analysis_partitionloop = KVDATA_BATCHSIZE_KVS / (SRCBUFFER_SIZE * NUMPIPELINES * 1); // NUMSUBWORKERS
	analysis_type analysis_reduceloop = KVDATA_BATCHSIZE_KVS / SRCBUFFER_SIZE;
	analysis_type analysis_processedges_overallloop = BATCH_RANGE_KVS / PADDEDDESTBUFFER_SIZE;
	analysis_type analysis_processedges_loadedgebatch = 1;
	analysis_type analysis_numllops = 1;
	analysis_type analysis_numsourcepartitions = 1;
	#if defined(_DEBUGMODE_KERNELPRINTS)
	actsutilityobj->printparameters();
	actsutilityobj->printglobalvars();
	#endif 
	#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(_DEBUGMODE_CHECKS2)
	actsutilityobj->clearglobalvars();
	#endif
		
	keyvalue_t sourcebuffer0[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = sourcebuffer0
	
	keyvalue_t buffer0_setof1[8][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer0_setof1
	skeyvalue_t templocalcapsule0_so1[8][NUM_PARTITIONS];
	#pragma HLS array_partition variable = templocalcapsule0_so1
	
	keyvalue_t buffer0_setof2[8][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer0_setof2
	skeyvalue_t templocalcapsule0_so2[4][NUM_PARTITIONS];
	#pragma HLS array_partition variable = templocalcapsule0_so2
	
	keyvalue_t buffer0_setof4[8][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer0_setof4
	skeyvalue_t templocalcapsule0_so4[2][NUM_PARTITIONS];
	#pragma HLS array_partition variable = templocalcapsule0_so4
	
	keyvalue_t buffer0_setof8[8][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer0_setof8
	skeyvalue_t templocalcapsule0_so8[NUM_PARTITIONS];

	keyvalue_t globalstatsbuffer0[NUM_PARTITIONS];
	batch_type skipsizes0[NUM_PARTITIONS];
	#ifdef _DEBUGMODE_CHECKS2
	keyvalue_t BIGKV0[NUM_PARTITIONS];
	#endif
	keyvalue_t sourcebuffer1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = sourcebuffer1
	
	keyvalue_t buffer1_setof1[8][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer1_setof1
	skeyvalue_t templocalcapsule1_so1[8][NUM_PARTITIONS];
	#pragma HLS array_partition variable = templocalcapsule1_so1
	
	keyvalue_t buffer1_setof2[8][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer1_setof2
	skeyvalue_t templocalcapsule1_so2[4][NUM_PARTITIONS];
	#pragma HLS array_partition variable = templocalcapsule1_so2
	
	keyvalue_t buffer1_setof4[8][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer1_setof4
	skeyvalue_t templocalcapsule1_so4[2][NUM_PARTITIONS];
	#pragma HLS array_partition variable = templocalcapsule1_so4
	
	keyvalue_t buffer1_setof8[8][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer1_setof8
	skeyvalue_t templocalcapsule1_so8[NUM_PARTITIONS];

	keyvalue_t globalstatsbuffer1[NUM_PARTITIONS];
	batch_type skipsizes1[NUM_PARTITIONS];
	#ifdef _DEBUGMODE_CHECKS2
	keyvalue_t BIGKV1[NUM_PARTITIONS];
	#endif
	keyvalue_t sourcebuffer2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = sourcebuffer2
	
	keyvalue_t buffer2_setof1[8][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer2_setof1
	skeyvalue_t templocalcapsule2_so1[8][NUM_PARTITIONS];
	#pragma HLS array_partition variable = templocalcapsule2_so1
	
	keyvalue_t buffer2_setof2[8][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer2_setof2
	skeyvalue_t templocalcapsule2_so2[4][NUM_PARTITIONS];
	#pragma HLS array_partition variable = templocalcapsule2_so2
	
	keyvalue_t buffer2_setof4[8][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer2_setof4
	skeyvalue_t templocalcapsule2_so4[2][NUM_PARTITIONS];
	#pragma HLS array_partition variable = templocalcapsule2_so4
	
	keyvalue_t buffer2_setof8[8][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer2_setof8
	skeyvalue_t templocalcapsule2_so8[NUM_PARTITIONS];

	keyvalue_t globalstatsbuffer2[NUM_PARTITIONS];
	batch_type skipsizes2[NUM_PARTITIONS];
	#ifdef _DEBUGMODE_CHECKS2
	keyvalue_t BIGKV2[NUM_PARTITIONS];
	#endif
	keyvalue_t sourcebuffer3[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = sourcebuffer3
	
	keyvalue_t buffer3_setof1[8][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer3_setof1
	skeyvalue_t templocalcapsule3_so1[8][NUM_PARTITIONS];
	#pragma HLS array_partition variable = templocalcapsule3_so1
	
	keyvalue_t buffer3_setof2[8][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer3_setof2
	skeyvalue_t templocalcapsule3_so2[4][NUM_PARTITIONS];
	#pragma HLS array_partition variable = templocalcapsule3_so2
	
	keyvalue_t buffer3_setof4[8][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer3_setof4
	skeyvalue_t templocalcapsule3_so4[2][NUM_PARTITIONS];
	#pragma HLS array_partition variable = templocalcapsule3_so4
	
	keyvalue_t buffer3_setof8[8][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer3_setof8
	skeyvalue_t templocalcapsule3_so8[NUM_PARTITIONS];

	keyvalue_t globalstatsbuffer3[NUM_PARTITIONS];
	batch_type skipsizes3[NUM_PARTITIONS];
	#ifdef _DEBUGMODE_CHECKS2
	keyvalue_t BIGKV3[NUM_PARTITIONS];
	#endif
	
	batch_type sourcestatsmarker0 = 0;
	batch_type deststatsmarker0 = 1;
	batch_type destoffset0 = 0;
	config_t config0;
	globalparams_t globalparams0 = getglobalparams(kvdram0);
	sweepparams_t sweepparams0;
	#ifdef _DEBUGMODE_CHECKS2
	if(globalparams0.runsize >= MAXKVDATA_BATCHSIZE){ cout<<"maxdispatch:ERROR. runsize too large!. globalparams0.runsize: "<<globalparams0.runsize<<", MAXKVDATA_BATCHSIZE: "<<MAXKVDATA_BATCHSIZE<<". EXITING"<<endl; exit(EXIT_FAILURE); }
	#endif
	batch_type sourcestatsmarker1 = 0;
	batch_type deststatsmarker1 = 1;
	batch_type destoffset1 = 0;
	config_t config1;
	globalparams_t globalparams1 = getglobalparams(kvdram1);
	sweepparams_t sweepparams1;
	#ifdef _DEBUGMODE_CHECKS2
	if(globalparams1.runsize >= MAXKVDATA_BATCHSIZE){ cout<<"maxdispatch:ERROR. runsize too large!. globalparams1.runsize: "<<globalparams1.runsize<<", MAXKVDATA_BATCHSIZE: "<<MAXKVDATA_BATCHSIZE<<". EXITING"<<endl; exit(EXIT_FAILURE); }
	#endif
	batch_type sourcestatsmarker2 = 0;
	batch_type deststatsmarker2 = 1;
	batch_type destoffset2 = 0;
	config_t config2;
	globalparams_t globalparams2 = getglobalparams(kvdram2);
	sweepparams_t sweepparams2;
	#ifdef _DEBUGMODE_CHECKS2
	if(globalparams2.runsize >= MAXKVDATA_BATCHSIZE){ cout<<"maxdispatch:ERROR. runsize too large!. globalparams2.runsize: "<<globalparams2.runsize<<", MAXKVDATA_BATCHSIZE: "<<MAXKVDATA_BATCHSIZE<<". EXITING"<<endl; exit(EXIT_FAILURE); }
	#endif
	batch_type sourcestatsmarker3 = 0;
	batch_type deststatsmarker3 = 1;
	batch_type destoffset3 = 0;
	config_t config3;
	globalparams_t globalparams3 = getglobalparams(kvdram3);
	sweepparams_t sweepparams3;
	#ifdef _DEBUGMODE_CHECKS2
	if(globalparams3.runsize >= MAXKVDATA_BATCHSIZE){ cout<<"maxdispatch:ERROR. runsize too large!. globalparams3.runsize: "<<globalparams3.runsize<<", MAXKVDATA_BATCHSIZE: "<<MAXKVDATA_BATCHSIZE<<". EXITING"<<endl; exit(EXIT_FAILURE); }
	#endif
	
	// start launch
	MAIN_LOOP1: for(step_type currentLOP=globalparams0.beginLOP; currentLOP<(globalparams0.beginLOP + globalparams0.numLOPs); currentLOP++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_numllops avg=analysis_numllops	
	
		batch_type num_source_partitions = get_num_source_partitions(currentLOP);
		destoffset0 = 0;
		destoffset1 = 0;
		destoffset2 = 0;
		destoffset3 = 0;
		
		MAIN_LOOP1B: for(batch_type source_partition=0; source_partition<num_source_partitions; source_partition+=1){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_numsourcepartitions avg=analysis_numsourcepartitions	
		
			resetmanykeyandvalues(buffer0_setof1, NUM_PARTITIONS);
			resetmanykeyandvalues(globalstatsbuffer0, NUM_PARTITIONS);
			resetmanykeyandvalues(buffer1_setof1, NUM_PARTITIONS);
			resetmanykeyandvalues(globalstatsbuffer1, NUM_PARTITIONS);
			resetmanykeyandvalues(buffer2_setof1, NUM_PARTITIONS);
			resetmanykeyandvalues(globalstatsbuffer2, NUM_PARTITIONS);
			resetmanykeyandvalues(buffer3_setof1, NUM_PARTITIONS);
			resetmanykeyandvalues(globalstatsbuffer3, NUM_PARTITIONS);
			
			sweepparams0 = getsweepparams(globalparams0, currentLOP, source_partition);
			travstate_t travstate0 = gettravstate(kvdram0, globalparams0, currentLOP, sourcestatsmarker0);
			travstate_t CStravstate0 = travstate0;
			travstate_t Ptravstate0 = travstate0;
			travstate_t Rtravstate0 = travstate0;
			travstate_t AVtravstate0;
			#ifdef _DEBUGMODE_KERNELPRINTS2
			actsutilityobj->setstructs(config0, sweepparams0, travstate0);
			#endif
			sweepparams1 = getsweepparams(globalparams1, currentLOP, source_partition);
			travstate_t travstate1 = gettravstate(kvdram1, globalparams1, currentLOP, sourcestatsmarker1);
			travstate_t CStravstate1 = travstate1;
			travstate_t Ptravstate1 = travstate1;
			travstate_t Rtravstate1 = travstate1;
			travstate_t AVtravstate1;
			#ifdef _DEBUGMODE_KERNELPRINTS2
			actsutilityobj->setstructs(config1, sweepparams1, travstate1);
			#endif
			sweepparams2 = getsweepparams(globalparams2, currentLOP, source_partition);
			travstate_t travstate2 = gettravstate(kvdram2, globalparams2, currentLOP, sourcestatsmarker2);
			travstate_t CStravstate2 = travstate2;
			travstate_t Ptravstate2 = travstate2;
			travstate_t Rtravstate2 = travstate2;
			travstate_t AVtravstate2;
			#ifdef _DEBUGMODE_KERNELPRINTS2
			actsutilityobj->setstructs(config2, sweepparams2, travstate2);
			#endif
			sweepparams3 = getsweepparams(globalparams3, currentLOP, source_partition);
			travstate_t travstate3 = gettravstate(kvdram3, globalparams3, currentLOP, sourcestatsmarker3);
			travstate_t CStravstate3 = travstate3;
			travstate_t Ptravstate3 = travstate3;
			travstate_t Rtravstate3 = travstate3;
			travstate_t AVtravstate3;
			#ifdef _DEBUGMODE_KERNELPRINTS2
			actsutilityobj->setstructs(config3, sweepparams3, travstate3);
			#endif
			
			// collect stats
			#ifdef COLLECTSTATS
			if(currentLOP >= 1 && currentLOP <= globalparams0.treedepth){ config0.enableprocessedges = OFF; config0.enablecollectglobalstats = ON; config0.enablepartition = OFF; config0.enablereduce = OFF; } 
			else { CStravstate0.begin_kvs = 0; CStravstate0.end_kvs = 0; config0.enablecollectglobalstats = OFF; }
			#ifdef _DEBUGMODE_KERNELPRINTS2
			if(config0.enablecollectglobalstats == ON){ actsutilityobj->print7("### maxdispatch0::collectgstats:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams0.source_partition, sweepparams0.upperlimit, CStravstate0.begin_kvs * VECTOR_SIZE, CStravstate0.end_kvs * VECTOR_SIZE, (CStravstate0.end_kvs - CStravstate0.begin_kvs) * VECTOR_SIZE, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams0.currentLOP)), sweepparams0.currentLOP); }					
			#endif
			if(currentLOP >= 1 && currentLOP <= globalparams1.treedepth){ config1.enableprocessedges = OFF; config1.enablecollectglobalstats = ON; config1.enablepartition = OFF; config1.enablereduce = OFF; } 
			else { CStravstate1.begin_kvs = 0; CStravstate1.end_kvs = 0; config1.enablecollectglobalstats = OFF; }
			#ifdef _DEBUGMODE_KERNELPRINTS2
			if(config1.enablecollectglobalstats == ON){ actsutilityobj->print7("### maxdispatch0::collectgstats:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams1.source_partition, sweepparams1.upperlimit, CStravstate1.begin_kvs * VECTOR_SIZE, CStravstate1.end_kvs * VECTOR_SIZE, (CStravstate1.end_kvs - CStravstate1.begin_kvs) * VECTOR_SIZE, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams1.currentLOP)), sweepparams1.currentLOP); }					
			#endif
			if(currentLOP >= 1 && currentLOP <= globalparams2.treedepth){ config2.enableprocessedges = OFF; config2.enablecollectglobalstats = ON; config2.enablepartition = OFF; config2.enablereduce = OFF; } 
			else { CStravstate2.begin_kvs = 0; CStravstate2.end_kvs = 0; config2.enablecollectglobalstats = OFF; }
			#ifdef _DEBUGMODE_KERNELPRINTS2
			if(config2.enablecollectglobalstats == ON){ actsutilityobj->print7("### maxdispatch0::collectgstats:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams2.source_partition, sweepparams2.upperlimit, CStravstate2.begin_kvs * VECTOR_SIZE, CStravstate2.end_kvs * VECTOR_SIZE, (CStravstate2.end_kvs - CStravstate2.begin_kvs) * VECTOR_SIZE, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams2.currentLOP)), sweepparams2.currentLOP); }					
			#endif
			if(currentLOP >= 1 && currentLOP <= globalparams3.treedepth){ config3.enableprocessedges = OFF; config3.enablecollectglobalstats = ON; config3.enablepartition = OFF; config3.enablereduce = OFF; } 
			else { CStravstate3.begin_kvs = 0; CStravstate3.end_kvs = 0; config3.enablecollectglobalstats = OFF; }
			#ifdef _DEBUGMODE_KERNELPRINTS2
			if(config3.enablecollectglobalstats == ON){ actsutilityobj->print7("### maxdispatch0::collectgstats:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams3.source_partition, sweepparams3.upperlimit, CStravstate3.begin_kvs * VECTOR_SIZE, CStravstate3.end_kvs * VECTOR_SIZE, (CStravstate3.end_kvs - CStravstate3.begin_kvs) * VECTOR_SIZE, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams3.currentLOP)), sweepparams3.currentLOP); }					
			#endif
			MAIN_LOOP1C_COLLECTGLOBALSTATS: for(batch_type offset_kvs=CStravstate0.begin_kvs; offset_kvs<CStravstate0.end_kvs; offset_kvs+=CStravstate0.skip_kvs * 1){ // NUMSUBWORKERS
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_collectstatsloop avg=analysis_collectstatsloop
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print4("### maxdispatch0::collectglobalstats:: offset_kvs", "begin_kvs", "end_kvs", "skip", offset_kvs, CStravstate.begin_kvs, CStravstate.end_kvs, SRCBUFFER_SIZE);
				#endif
				
				CStravstate0.i_kvs = offset_kvs;
				CStravstate1.i_kvs = offset_kvs;
				CStravstate2.i_kvs = offset_kvs;
				CStravstate3.i_kvs = offset_kvs;
				
				readkeyvalues00(ON, kvdram0, sourcebuffer0, (sweepparams0.worksourcebaseaddress_kvs + offset_kvs), CStravstate0);
				readkeyvalues01(ON, kvdram1, sourcebuffer1, (sweepparams1.worksourcebaseaddress_kvs + offset_kvs), CStravstate1);
				readkeyvalues02(ON, kvdram2, sourcebuffer2, (sweepparams2.worksourcebaseaddress_kvs + offset_kvs), CStravstate2);
				readkeyvalues03(ON, kvdram3, sourcebuffer3, (sweepparams3.worksourcebaseaddress_kvs + offset_kvs), CStravstate3);
				
				collectglobalstats00(ON, sourcebuffer0, buffer0_setof1, sweepparams0.currentLOP, sweepparams0.upperlimit, CStravstate0, globalparams0);
				collectglobalstats01(ON, sourcebuffer1, buffer1_setof1, sweepparams1.currentLOP, sweepparams1.upperlimit, CStravstate1, globalparams1);
				collectglobalstats02(ON, sourcebuffer2, buffer2_setof1, sweepparams2.currentLOP, sweepparams2.upperlimit, CStravstate2, globalparams2);
				collectglobalstats03(ON, sourcebuffer3, buffer3_setof1, sweepparams3.currentLOP, sweepparams3.upperlimit, CStravstate3, globalparams3);
			}
			prepareglobalstats00(config0.enablecollectglobalstats, buffer0_setof1, globalstatsbuffer0, globalparams0);
			for(partition_type p=0; p<NUM_PARTITIONS; p++){ batch_type A = (globalstatsbuffer0[p].value + (VECTOR_SIZE-1)) / VECTOR_SIZE; batch_type B = (A + (SRCBUFFER_SIZE-1)) / SRCBUFFER_SIZE; if(B < 80){ B = B * 2; } batch_type C = ((4 * 4 * 2) * NUM_PARTITIONS) + VECTOR_SIZE; skipsizes0[p] = (B * C) + 128; } //'128' is safety padd // FIXME. REMOVEME.
			prepareglobalstats01(config1.enablecollectglobalstats, buffer1_setof1, globalstatsbuffer1, globalparams1);
			for(partition_type p=0; p<NUM_PARTITIONS; p++){ batch_type A = (globalstatsbuffer1[p].value + (VECTOR_SIZE-1)) / VECTOR_SIZE; batch_type B = (A + (SRCBUFFER_SIZE-1)) / SRCBUFFER_SIZE; if(B < 80){ B = B * 2; } batch_type C = ((4 * 4 * 2) * NUM_PARTITIONS) + VECTOR_SIZE; skipsizes1[p] = (B * C) + 128; } //'128' is safety padd // FIXME. REMOVEME.
			prepareglobalstats02(config2.enablecollectglobalstats, buffer2_setof1, globalstatsbuffer2, globalparams2);
			for(partition_type p=0; p<NUM_PARTITIONS; p++){ batch_type A = (globalstatsbuffer2[p].value + (VECTOR_SIZE-1)) / VECTOR_SIZE; batch_type B = (A + (SRCBUFFER_SIZE-1)) / SRCBUFFER_SIZE; if(B < 80){ B = B * 2; } batch_type C = ((4 * 4 * 2) * NUM_PARTITIONS) + VECTOR_SIZE; skipsizes2[p] = (B * C) + 128; } //'128' is safety padd // FIXME. REMOVEME.
			prepareglobalstats03(config3.enablecollectglobalstats, buffer3_setof1, globalstatsbuffer3, globalparams3);
			for(partition_type p=0; p<NUM_PARTITIONS; p++){ batch_type A = (globalstatsbuffer3[p].value + (VECTOR_SIZE-1)) / VECTOR_SIZE; batch_type B = (A + (SRCBUFFER_SIZE-1)) / SRCBUFFER_SIZE; if(B < 80){ B = B * 2; } batch_type C = ((4 * 4 * 2) * NUM_PARTITIONS) + VECTOR_SIZE; skipsizes3[p] = (B * C) + 128; } //'128' is safety padd // FIXME. REMOVEME.
			
			#ifdef _DEBUGMODE_CHECKS2
			resetvalues(BIGKV0, NUM_PARTITIONS);
			for(partition_type p=0; p<NUM_PARTITIONS; p++){ BIGKV0[p].value = globalstatsbuffer0[p].value + skipsizes0[p]; }
			#endif
			calculateoffsets(globalstatsbuffer0, NUM_PARTITIONS, destoffset0, skipsizes0);
			resetvalues(globalstatsbuffer0, NUM_PARTITIONS);
			saveglobalstats00(config0.enablecollectglobalstats, kvdram0, globalstatsbuffer0, BASEOFFSET_STATSDRAM_KVS + deststatsmarker0);
			#ifdef _DEBUGMODE_CHECKS2
			resetvalues(BIGKV1, NUM_PARTITIONS);
			for(partition_type p=0; p<NUM_PARTITIONS; p++){ BIGKV1[p].value = globalstatsbuffer1[p].value + skipsizes1[p]; }
			#endif
			calculateoffsets(globalstatsbuffer1, NUM_PARTITIONS, destoffset1, skipsizes1);
			resetvalues(globalstatsbuffer1, NUM_PARTITIONS);
			saveglobalstats01(config1.enablecollectglobalstats, kvdram1, globalstatsbuffer1, BASEOFFSET_STATSDRAM_KVS + deststatsmarker1);
			#ifdef _DEBUGMODE_CHECKS2
			resetvalues(BIGKV2, NUM_PARTITIONS);
			for(partition_type p=0; p<NUM_PARTITIONS; p++){ BIGKV2[p].value = globalstatsbuffer2[p].value + skipsizes2[p]; }
			#endif
			calculateoffsets(globalstatsbuffer2, NUM_PARTITIONS, destoffset2, skipsizes2);
			resetvalues(globalstatsbuffer2, NUM_PARTITIONS);
			saveglobalstats02(config2.enablecollectglobalstats, kvdram2, globalstatsbuffer2, BASEOFFSET_STATSDRAM_KVS + deststatsmarker2);
			#ifdef _DEBUGMODE_CHECKS2
			resetvalues(BIGKV3, NUM_PARTITIONS);
			for(partition_type p=0; p<NUM_PARTITIONS; p++){ BIGKV3[p].value = globalstatsbuffer3[p].value + skipsizes3[p]; }
			#endif
			calculateoffsets(globalstatsbuffer3, NUM_PARTITIONS, destoffset3, skipsizes3);
			resetvalues(globalstatsbuffer3, NUM_PARTITIONS);
			saveglobalstats03(config3.enablecollectglobalstats, kvdram3, globalstatsbuffer3, BASEOFFSET_STATSDRAM_KVS + deststatsmarker3);
			#endif 
			
			// partition
			#ifdef PARTITIONUPDATES
			if((currentLOP >= 1) && (currentLOP <= globalparams0.treedepth) && (Ptravstate0.size_kvs > 0)){ config0.enableprocessedges = OFF; config0.enablecollectglobalstats = OFF; config0.enablepartition = ON; config0.enablereduce = OFF; } 
			else { Ptravstate0.begin_kvs = 0; Ptravstate0.end_kvs = 0; config0.enablepartition = OFF; }
			travstate_t Ptravstate0pp0 = Ptravstate0;
			travstate_t Ptravstate0pp1 = Ptravstate0;
			travstate_t Ptravstate0pp2 = Ptravstate0;
			#ifdef _DEBUGMODE_KERNELPRINTS2
			if((currentLOP >= 1) && (currentLOP <= globalparams0.treedepth)){ actsutilityobj->print7("### maxdispatch0::partition:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams0.source_partition, sweepparams0.upperlimit, Ptravstate0.begin_kvs * VECTOR_SIZE, Ptravstate0.end_kvs * VECTOR_SIZE, Ptravstate0.size_kvs * VECTOR_SIZE, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams0.currentLOP)), sweepparams0.currentLOP); }	
			#endif
			readglobalstats00(config0.enablepartition, kvdram0, globalstatsbuffer0, BASEOFFSET_STATSDRAM_KVS + deststatsmarker0);
			resetvalues(globalstatsbuffer0, NUM_PARTITIONS);
			if((currentLOP >= 1) && (currentLOP <= globalparams1.treedepth) && (Ptravstate1.size_kvs > 0)){ config1.enableprocessedges = OFF; config1.enablecollectglobalstats = OFF; config1.enablepartition = ON; config1.enablereduce = OFF; } 
			else { Ptravstate1.begin_kvs = 0; Ptravstate1.end_kvs = 0; config1.enablepartition = OFF; }
			travstate_t Ptravstate1pp0 = Ptravstate1;
			travstate_t Ptravstate1pp1 = Ptravstate1;
			travstate_t Ptravstate1pp2 = Ptravstate1;
			#ifdef _DEBUGMODE_KERNELPRINTS2
			if((currentLOP >= 1) && (currentLOP <= globalparams1.treedepth)){ actsutilityobj->print7("### maxdispatch0::partition:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams1.source_partition, sweepparams1.upperlimit, Ptravstate1.begin_kvs * VECTOR_SIZE, Ptravstate1.end_kvs * VECTOR_SIZE, Ptravstate1.size_kvs * VECTOR_SIZE, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams1.currentLOP)), sweepparams1.currentLOP); }	
			#endif
			readglobalstats01(config1.enablepartition, kvdram1, globalstatsbuffer1, BASEOFFSET_STATSDRAM_KVS + deststatsmarker1);
			resetvalues(globalstatsbuffer1, NUM_PARTITIONS);
			if((currentLOP >= 1) && (currentLOP <= globalparams2.treedepth) && (Ptravstate2.size_kvs > 0)){ config2.enableprocessedges = OFF; config2.enablecollectglobalstats = OFF; config2.enablepartition = ON; config2.enablereduce = OFF; } 
			else { Ptravstate2.begin_kvs = 0; Ptravstate2.end_kvs = 0; config2.enablepartition = OFF; }
			travstate_t Ptravstate2pp0 = Ptravstate2;
			travstate_t Ptravstate2pp1 = Ptravstate2;
			travstate_t Ptravstate2pp2 = Ptravstate2;
			#ifdef _DEBUGMODE_KERNELPRINTS2
			if((currentLOP >= 1) && (currentLOP <= globalparams2.treedepth)){ actsutilityobj->print7("### maxdispatch0::partition:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams2.source_partition, sweepparams2.upperlimit, Ptravstate2.begin_kvs * VECTOR_SIZE, Ptravstate2.end_kvs * VECTOR_SIZE, Ptravstate2.size_kvs * VECTOR_SIZE, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams2.currentLOP)), sweepparams2.currentLOP); }	
			#endif
			readglobalstats02(config2.enablepartition, kvdram2, globalstatsbuffer2, BASEOFFSET_STATSDRAM_KVS + deststatsmarker2);
			resetvalues(globalstatsbuffer2, NUM_PARTITIONS);
			if((currentLOP >= 1) && (currentLOP <= globalparams3.treedepth) && (Ptravstate3.size_kvs > 0)){ config3.enableprocessedges = OFF; config3.enablecollectglobalstats = OFF; config3.enablepartition = ON; config3.enablereduce = OFF; } 
			else { Ptravstate3.begin_kvs = 0; Ptravstate3.end_kvs = 0; config3.enablepartition = OFF; }
			travstate_t Ptravstate3pp0 = Ptravstate3;
			travstate_t Ptravstate3pp1 = Ptravstate3;
			travstate_t Ptravstate3pp2 = Ptravstate3;
			#ifdef _DEBUGMODE_KERNELPRINTS2
			if((currentLOP >= 1) && (currentLOP <= globalparams3.treedepth)){ actsutilityobj->print7("### maxdispatch0::partition:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams3.source_partition, sweepparams3.upperlimit, Ptravstate3.begin_kvs * VECTOR_SIZE, Ptravstate3.end_kvs * VECTOR_SIZE, Ptravstate3.size_kvs * VECTOR_SIZE, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams3.currentLOP)), sweepparams3.currentLOP); }	
			#endif
			readglobalstats03(config3.enablepartition, kvdram3, globalstatsbuffer3, BASEOFFSET_STATSDRAM_KVS + deststatsmarker3);
			resetvalues(globalstatsbuffer3, NUM_PARTITIONS);
			MAIN_LOOP1D_PARTITION: for(batch_type offset_kvs=Ptravstate0.begin_kvs; offset_kvs<Ptravstate0.end_kvs; offset_kvs+=Ptravstate0.skip_kvs * NUMPIPELINES * 1){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_partitionloop avg=analysis_partitionloop
				#ifdef _DEBUGMODE_KERNELPRINTS2
				actsutilityobj->print4("### maxdispatch0::partition:: offset_kvs", "begin_kvs", "end_kvs", "skip", offset_kvs, Ptravstate0.begin_kvs, Ptravstate0.end_kvs, SRCBUFFER_SIZE);
				actsutilityobj->print4("### maxdispatch0::partition:: offset_kvs", "begin_kvs", "end_kvs", "skip", offset_kvs, Ptravstate1.begin_kvs, Ptravstate1.end_kvs, SRCBUFFER_SIZE);
				actsutilityobj->print4("### maxdispatch0::partition:: offset_kvs", "begin_kvs", "end_kvs", "skip", offset_kvs, Ptravstate2.begin_kvs, Ptravstate2.end_kvs, SRCBUFFER_SIZE);
				actsutilityobj->print4("### maxdispatch0::partition:: offset_kvs", "begin_kvs", "end_kvs", "skip", offset_kvs, Ptravstate3.begin_kvs, Ptravstate3.end_kvs, SRCBUFFER_SIZE);
				#endif
				
				Ptravstate0.i_kvs = offset_kvs;
				Ptravstate0pp0.i_kvs = offset_kvs;
				Ptravstate0pp1.i_kvs = offset_kvs + SRCBUFFER_SIZE;
				Ptravstate0pp2.i_kvs = offset_kvs + (2 * SRCBUFFER_SIZE);
				unsigned int en0; if(offset_kvs == Ptravstate0.begin_kvs){ en0 = OFF; } else { en0 = ON; }
				Ptravstate1.i_kvs = offset_kvs;
				Ptravstate1pp0.i_kvs = offset_kvs;
				Ptravstate1pp1.i_kvs = offset_kvs + SRCBUFFER_SIZE;
				Ptravstate1pp2.i_kvs = offset_kvs + (2 * SRCBUFFER_SIZE);
				unsigned int en1; if(offset_kvs == Ptravstate1.begin_kvs){ en1 = OFF; } else { en1 = ON; }
				Ptravstate2.i_kvs = offset_kvs;
				Ptravstate2pp0.i_kvs = offset_kvs;
				Ptravstate2pp1.i_kvs = offset_kvs + SRCBUFFER_SIZE;
				Ptravstate2pp2.i_kvs = offset_kvs + (2 * SRCBUFFER_SIZE);
				unsigned int en2; if(offset_kvs == Ptravstate2.begin_kvs){ en2 = OFF; } else { en2 = ON; }
				Ptravstate3.i_kvs = offset_kvs;
				Ptravstate3pp0.i_kvs = offset_kvs;
				Ptravstate3pp1.i_kvs = offset_kvs + SRCBUFFER_SIZE;
				Ptravstate3pp2.i_kvs = offset_kvs + (2 * SRCBUFFER_SIZE);
				unsigned int en3; if(offset_kvs == Ptravstate3.begin_kvs){ en3 = OFF; } else { en3 = ON; }
				
				readkeyvalues00(ON, kvdram0, sourcebuffer0, (sweepparams0.worksourcebaseaddress_kvs + offset_kvs), Ptravstate0pp0);
				#ifdef PP1 // pipeline overflow from bottom
				savekeyvalues00(en0, kvdram0, buffer0_setof8, globalstatsbuffer0, templocalcapsule0_so8, sweepparams0.workdestbaseaddress_kvs, globalparams0);
				#endif
				#ifdef PP2 // pipeline overflow from bottom
				combineSetof4stoSetof8s00(en0, buffer0_setof4, buffer0_setof8, templocalcapsule0_so4, templocalcapsule0_so8, globalparams0);
				#endif
				readkeyvalues01(ON, kvdram1, sourcebuffer1, (sweepparams1.worksourcebaseaddress_kvs + offset_kvs), Ptravstate1pp0);
				#ifdef PP1 // pipeline overflow from bottom
				savekeyvalues01(en1, kvdram1, buffer1_setof8, globalstatsbuffer1, templocalcapsule1_so8, sweepparams1.workdestbaseaddress_kvs, globalparams1);
				#endif
				#ifdef PP2 // pipeline overflow from bottom
				combineSetof4stoSetof8s01(en1, buffer1_setof4, buffer1_setof8, templocalcapsule1_so4, templocalcapsule1_so8, globalparams1);
				#endif
				readkeyvalues02(ON, kvdram2, sourcebuffer2, (sweepparams2.worksourcebaseaddress_kvs + offset_kvs), Ptravstate2pp0);
				#ifdef PP1 // pipeline overflow from bottom
				savekeyvalues02(en2, kvdram2, buffer2_setof8, globalstatsbuffer2, templocalcapsule2_so8, sweepparams2.workdestbaseaddress_kvs, globalparams2);
				#endif
				#ifdef PP2 // pipeline overflow from bottom
				combineSetof4stoSetof8s02(en2, buffer2_setof4, buffer2_setof8, templocalcapsule2_so4, templocalcapsule2_so8, globalparams2);
				#endif
				readkeyvalues03(ON, kvdram3, sourcebuffer3, (sweepparams3.worksourcebaseaddress_kvs + offset_kvs), Ptravstate3pp0);
				#ifdef PP1 // pipeline overflow from bottom
				savekeyvalues03(en3, kvdram3, buffer3_setof8, globalstatsbuffer3, templocalcapsule3_so8, sweepparams3.workdestbaseaddress_kvs, globalparams3);
				#endif
				#ifdef PP2 // pipeline overflow from bottom
				combineSetof4stoSetof8s03(en3, buffer3_setof4, buffer3_setof8, templocalcapsule3_so4, templocalcapsule3_so8, globalparams3);
				#endif
				
				partitionkeyvalues00(ON, sourcebuffer0, buffer0_setof1, templocalcapsule0_so1, sweepparams0.currentLOP, sweepparams0.upperlimit, Ptravstate0pp0, globalparams0);
				#ifdef PP1
				readkeyvalues00(ON, kvdram0, sourcebuffer0, (sweepparams0.worksourcebaseaddress_kvs + offset_kvs + SRCBUFFER_SIZE), Ptravstate0pp1);
				#endif
				#ifdef PP2 // pipeline overflow from bottom
				savekeyvalues00(en0, kvdram0, buffer0_setof8, globalstatsbuffer0, templocalcapsule0_so8, sweepparams0.workdestbaseaddress_kvs, globalparams0);
				#endif
				partitionkeyvalues01(ON, sourcebuffer1, buffer1_setof1, templocalcapsule1_so1, sweepparams1.currentLOP, sweepparams1.upperlimit, Ptravstate1pp0, globalparams1);
				#ifdef PP1
				readkeyvalues01(ON, kvdram1, sourcebuffer1, (sweepparams1.worksourcebaseaddress_kvs + offset_kvs + SRCBUFFER_SIZE), Ptravstate1pp1);
				#endif
				#ifdef PP2 // pipeline overflow from bottom
				savekeyvalues01(en1, kvdram1, buffer1_setof8, globalstatsbuffer1, templocalcapsule1_so8, sweepparams1.workdestbaseaddress_kvs, globalparams1);
				#endif
				partitionkeyvalues02(ON, sourcebuffer2, buffer2_setof1, templocalcapsule2_so1, sweepparams2.currentLOP, sweepparams2.upperlimit, Ptravstate2pp0, globalparams2);
				#ifdef PP1
				readkeyvalues02(ON, kvdram2, sourcebuffer2, (sweepparams2.worksourcebaseaddress_kvs + offset_kvs + SRCBUFFER_SIZE), Ptravstate2pp1);
				#endif
				#ifdef PP2 // pipeline overflow from bottom
				savekeyvalues02(en2, kvdram2, buffer2_setof8, globalstatsbuffer2, templocalcapsule2_so8, sweepparams2.workdestbaseaddress_kvs, globalparams2);
				#endif
				partitionkeyvalues03(ON, sourcebuffer3, buffer3_setof1, templocalcapsule3_so1, sweepparams3.currentLOP, sweepparams3.upperlimit, Ptravstate3pp0, globalparams3);
				#ifdef PP1
				readkeyvalues03(ON, kvdram3, sourcebuffer3, (sweepparams3.worksourcebaseaddress_kvs + offset_kvs + SRCBUFFER_SIZE), Ptravstate3pp1);
				#endif
				#ifdef PP2 // pipeline overflow from bottom
				savekeyvalues03(en3, kvdram3, buffer3_setof8, globalstatsbuffer3, templocalcapsule3_so8, sweepparams3.workdestbaseaddress_kvs, globalparams3);
				#endif
				
				// 1s->2s
				combineSetof1stoSetof2s00(ON, buffer0_setof1, buffer0_setof2, templocalcapsule0_so1, templocalcapsule0_so2, globalparams0);
				#ifdef PP1
				partitionkeyvalues00(ON, sourcebuffer0, buffer0_setof1, templocalcapsule0_so1, sweepparams0.currentLOP, sweepparams0.upperlimit, Ptravstate0pp1, globalparams0);
				#endif
				#ifdef PP2
				readkeyvalues00(ON, kvdram0, sourcebuffer0, (sweepparams0.worksourcebaseaddress_kvs + offset_kvs + SRCBUFFER_SIZE + SRCBUFFER_SIZE), Ptravstate0pp2);
				#endif
				combineSetof1stoSetof2s01(ON, buffer1_setof1, buffer1_setof2, templocalcapsule1_so1, templocalcapsule1_so2, globalparams1);
				#ifdef PP1
				partitionkeyvalues01(ON, sourcebuffer1, buffer1_setof1, templocalcapsule1_so1, sweepparams1.currentLOP, sweepparams1.upperlimit, Ptravstate1pp1, globalparams1);
				#endif
				#ifdef PP2
				readkeyvalues01(ON, kvdram1, sourcebuffer1, (sweepparams1.worksourcebaseaddress_kvs + offset_kvs + SRCBUFFER_SIZE + SRCBUFFER_SIZE), Ptravstate1pp2);
				#endif
				combineSetof1stoSetof2s02(ON, buffer2_setof1, buffer2_setof2, templocalcapsule2_so1, templocalcapsule2_so2, globalparams2);
				#ifdef PP1
				partitionkeyvalues02(ON, sourcebuffer2, buffer2_setof1, templocalcapsule2_so1, sweepparams2.currentLOP, sweepparams2.upperlimit, Ptravstate2pp1, globalparams2);
				#endif
				#ifdef PP2
				readkeyvalues02(ON, kvdram2, sourcebuffer2, (sweepparams2.worksourcebaseaddress_kvs + offset_kvs + SRCBUFFER_SIZE + SRCBUFFER_SIZE), Ptravstate2pp2);
				#endif
				combineSetof1stoSetof2s03(ON, buffer3_setof1, buffer3_setof2, templocalcapsule3_so1, templocalcapsule3_so2, globalparams3);
				#ifdef PP1
				partitionkeyvalues03(ON, sourcebuffer3, buffer3_setof1, templocalcapsule3_so1, sweepparams3.currentLOP, sweepparams3.upperlimit, Ptravstate3pp1, globalparams3);
				#endif
				#ifdef PP2
				readkeyvalues03(ON, kvdram3, sourcebuffer3, (sweepparams3.worksourcebaseaddress_kvs + offset_kvs + SRCBUFFER_SIZE + SRCBUFFER_SIZE), Ptravstate3pp2);
				#endif
				
				// 2s->4s
				combineSetof2stoSetof4s00(ON, buffer0_setof2, buffer0_setof4, templocalcapsule0_so2, templocalcapsule0_so4, globalparams0);
				#ifdef PP1
				combineSetof1stoSetof2s00(ON, buffer0_setof1, buffer0_setof2, templocalcapsule0_so1, templocalcapsule0_so2, globalparams0);
				#endif
				#ifdef PP2
				partitionkeyvalues00(ON, sourcebuffer0, buffer0_setof1, templocalcapsule0_so1, sweepparams0.currentLOP, sweepparams0.upperlimit, Ptravstate0pp2, globalparams0);
				#endif
				combineSetof2stoSetof4s01(ON, buffer1_setof2, buffer1_setof4, templocalcapsule1_so2, templocalcapsule1_so4, globalparams1);
				#ifdef PP1
				combineSetof1stoSetof2s01(ON, buffer1_setof1, buffer1_setof2, templocalcapsule1_so1, templocalcapsule1_so2, globalparams1);
				#endif
				#ifdef PP2
				partitionkeyvalues01(ON, sourcebuffer1, buffer1_setof1, templocalcapsule1_so1, sweepparams1.currentLOP, sweepparams1.upperlimit, Ptravstate1pp2, globalparams1);
				#endif
				combineSetof2stoSetof4s02(ON, buffer2_setof2, buffer2_setof4, templocalcapsule2_so2, templocalcapsule2_so4, globalparams2);
				#ifdef PP1
				combineSetof1stoSetof2s02(ON, buffer2_setof1, buffer2_setof2, templocalcapsule2_so1, templocalcapsule2_so2, globalparams2);
				#endif
				#ifdef PP2
				partitionkeyvalues02(ON, sourcebuffer2, buffer2_setof1, templocalcapsule2_so1, sweepparams2.currentLOP, sweepparams2.upperlimit, Ptravstate2pp2, globalparams2);
				#endif
				combineSetof2stoSetof4s03(ON, buffer3_setof2, buffer3_setof4, templocalcapsule3_so2, templocalcapsule3_so4, globalparams3);
				#ifdef PP1
				combineSetof1stoSetof2s03(ON, buffer3_setof1, buffer3_setof2, templocalcapsule3_so1, templocalcapsule3_so2, globalparams3);
				#endif
				#ifdef PP2
				partitionkeyvalues03(ON, sourcebuffer3, buffer3_setof1, templocalcapsule3_so1, sweepparams3.currentLOP, sweepparams3.upperlimit, Ptravstate3pp2, globalparams3);
				#endif
				
				// 4s->8s
				combineSetof4stoSetof8s00(ON, buffer0_setof4, buffer0_setof8, templocalcapsule0_so4, templocalcapsule0_so8, globalparams0);
				#ifdef PP1
				combineSetof2stoSetof4s00(ON, buffer0_setof2, buffer0_setof4, templocalcapsule0_so2, templocalcapsule0_so4, globalparams0);
				#endif
				#ifdef PP2
				combineSetof1stoSetof2s00(ON, buffer0_setof1, buffer0_setof2, templocalcapsule0_so1, templocalcapsule0_so2, globalparams0);
				#endif
				combineSetof4stoSetof8s01(ON, buffer1_setof4, buffer1_setof8, templocalcapsule1_so4, templocalcapsule1_so8, globalparams1);
				#ifdef PP1
				combineSetof2stoSetof4s01(ON, buffer1_setof2, buffer1_setof4, templocalcapsule1_so2, templocalcapsule1_so4, globalparams1);
				#endif
				#ifdef PP2
				combineSetof1stoSetof2s01(ON, buffer1_setof1, buffer1_setof2, templocalcapsule1_so1, templocalcapsule1_so2, globalparams1);
				#endif
				combineSetof4stoSetof8s02(ON, buffer2_setof4, buffer2_setof8, templocalcapsule2_so4, templocalcapsule2_so8, globalparams2);
				#ifdef PP1
				combineSetof2stoSetof4s02(ON, buffer2_setof2, buffer2_setof4, templocalcapsule2_so2, templocalcapsule2_so4, globalparams2);
				#endif
				#ifdef PP2
				combineSetof1stoSetof2s02(ON, buffer2_setof1, buffer2_setof2, templocalcapsule2_so1, templocalcapsule2_so2, globalparams2);
				#endif
				combineSetof4stoSetof8s03(ON, buffer3_setof4, buffer3_setof8, templocalcapsule3_so4, templocalcapsule3_so8, globalparams3);
				#ifdef PP1
				combineSetof2stoSetof4s03(ON, buffer3_setof2, buffer3_setof4, templocalcapsule3_so2, templocalcapsule3_so4, globalparams3);
				#endif
				#ifdef PP2
				combineSetof1stoSetof2s03(ON, buffer3_setof1, buffer3_setof2, templocalcapsule3_so1, templocalcapsule3_so2, globalparams3);
				#endif
				
				savekeyvalues00(ON, kvdram0, buffer0_setof8, globalstatsbuffer0, templocalcapsule0_so8, sweepparams0.workdestbaseaddress_kvs, globalparams0);
				#ifdef PP1
				combineSetof4stoSetof8s00(ON, buffer0_setof4, buffer0_setof8, templocalcapsule0_so4, templocalcapsule0_so8, globalparams0);
				#endif
				#ifdef PP2
				combineSetof2stoSetof4s00(ON, buffer0_setof2, buffer0_setof4, templocalcapsule0_so2, templocalcapsule0_so4, globalparams0);
				#endif
				savekeyvalues01(ON, kvdram1, buffer1_setof8, globalstatsbuffer1, templocalcapsule1_so8, sweepparams1.workdestbaseaddress_kvs, globalparams1);
				#ifdef PP1
				combineSetof4stoSetof8s01(ON, buffer1_setof4, buffer1_setof8, templocalcapsule1_so4, templocalcapsule1_so8, globalparams1);
				#endif
				#ifdef PP2
				combineSetof2stoSetof4s01(ON, buffer1_setof2, buffer1_setof4, templocalcapsule1_so2, templocalcapsule1_so4, globalparams1);
				#endif
				savekeyvalues02(ON, kvdram2, buffer2_setof8, globalstatsbuffer2, templocalcapsule2_so8, sweepparams2.workdestbaseaddress_kvs, globalparams2);
				#ifdef PP1
				combineSetof4stoSetof8s02(ON, buffer2_setof4, buffer2_setof8, templocalcapsule2_so4, templocalcapsule2_so8, globalparams2);
				#endif
				#ifdef PP2
				combineSetof2stoSetof4s02(ON, buffer2_setof2, buffer2_setof4, templocalcapsule2_so2, templocalcapsule2_so4, globalparams2);
				#endif
				savekeyvalues03(ON, kvdram3, buffer3_setof8, globalstatsbuffer3, templocalcapsule3_so8, sweepparams3.workdestbaseaddress_kvs, globalparams3);
				#ifdef PP1
				combineSetof4stoSetof8s03(ON, buffer3_setof4, buffer3_setof8, templocalcapsule3_so4, templocalcapsule3_so8, globalparams3);
				#endif
				#ifdef PP2
				combineSetof2stoSetof4s03(ON, buffer3_setof2, buffer3_setof4, templocalcapsule3_so2, templocalcapsule3_so4, globalparams3);
				#endif
				///// overflow. pipeline continuing from top...
			}
			#ifdef PP1 // pipeline overflow from bottom
			savekeyvalues00(config0.enablepartition, kvdram0, buffer0_setof8, globalstatsbuffer0, templocalcapsule0_so8, sweepparams0.workdestbaseaddress_kvs, globalparams0);
			#endif
			#ifdef PP2 // pipeline overflow from bottom
			combineSetof4stoSetof8s00(config0.enablepartition, buffer0_setof4, buffer0_setof8, templocalcapsule0_so4, templocalcapsule0_so8, globalparams0);
			#endif
			#ifdef PP1 // pipeline overflow from bottom
			savekeyvalues01(config1.enablepartition, kvdram1, buffer1_setof8, globalstatsbuffer1, templocalcapsule1_so8, sweepparams1.workdestbaseaddress_kvs, globalparams1);
			#endif
			#ifdef PP2 // pipeline overflow from bottom
			combineSetof4stoSetof8s01(config1.enablepartition, buffer1_setof4, buffer1_setof8, templocalcapsule1_so4, templocalcapsule1_so8, globalparams1);
			#endif
			#ifdef PP1 // pipeline overflow from bottom
			savekeyvalues02(config2.enablepartition, kvdram2, buffer2_setof8, globalstatsbuffer2, templocalcapsule2_so8, sweepparams2.workdestbaseaddress_kvs, globalparams2);
			#endif
			#ifdef PP2 // pipeline overflow from bottom
			combineSetof4stoSetof8s02(config2.enablepartition, buffer2_setof4, buffer2_setof8, templocalcapsule2_so4, templocalcapsule2_so8, globalparams2);
			#endif
			#ifdef PP1 // pipeline overflow from bottom
			savekeyvalues03(config3.enablepartition, kvdram3, buffer3_setof8, globalstatsbuffer3, templocalcapsule3_so8, sweepparams3.workdestbaseaddress_kvs, globalparams3);
			#endif
			#ifdef PP2 // pipeline overflow from bottom
			combineSetof4stoSetof8s03(config3.enablepartition, buffer3_setof4, buffer3_setof8, templocalcapsule3_so4, templocalcapsule3_so8, globalparams3);
			#endif
			
			#ifdef PP2 // pipeline overflow from bottom
			savekeyvalues00(config0.enablepartition, kvdram0, buffer0_setof8, globalstatsbuffer0, templocalcapsule0_so8, sweepparams0.workdestbaseaddress_kvs, globalparams0);
			#endif
			#ifdef PP2 // pipeline overflow from bottom
			savekeyvalues01(config1.enablepartition, kvdram1, buffer1_setof8, globalstatsbuffer1, templocalcapsule1_so8, sweepparams1.workdestbaseaddress_kvs, globalparams1);
			#endif
			#ifdef PP2 // pipeline overflow from bottom
			savekeyvalues02(config2.enablepartition, kvdram2, buffer2_setof8, globalstatsbuffer2, templocalcapsule2_so8, sweepparams2.workdestbaseaddress_kvs, globalparams2);
			#endif
			#ifdef PP2 // pipeline overflow from bottom
			savekeyvalues03(config3.enablepartition, kvdram3, buffer3_setof8, globalstatsbuffer3, templocalcapsule3_so8, sweepparams3.workdestbaseaddress_kvs, globalparams3);
			#endif
			
			saveglobalstats00(config0.enablepartition, kvdram0, globalstatsbuffer0, BASEOFFSET_STATSDRAM_KVS + deststatsmarker0);
			#if defined(_DEBUGMODE_CHECKS2) && defined(ENABLE_PERFECTACCURACY)
			if(config0.enablereduce == OFF){ actsutilityobj->checkforgreaterthan("maxdispatch. comparing BIGKV0 & globalstatsbuffer0", BIGKV, globalstatsbuffer0, NUM_PARTITIONS); }
			#endif
			saveglobalstats01(config1.enablepartition, kvdram1, globalstatsbuffer1, BASEOFFSET_STATSDRAM_KVS + deststatsmarker1);
			#if defined(_DEBUGMODE_CHECKS2) && defined(ENABLE_PERFECTACCURACY)
			if(config1.enablereduce == OFF){ actsutilityobj->checkforgreaterthan("maxdispatch. comparing BIGKV1 & globalstatsbuffer1", BIGKV, globalstatsbuffer1, NUM_PARTITIONS); }
			#endif
			saveglobalstats02(config2.enablepartition, kvdram2, globalstatsbuffer2, BASEOFFSET_STATSDRAM_KVS + deststatsmarker2);
			#if defined(_DEBUGMODE_CHECKS2) && defined(ENABLE_PERFECTACCURACY)
			if(config2.enablereduce == OFF){ actsutilityobj->checkforgreaterthan("maxdispatch. comparing BIGKV2 & globalstatsbuffer2", BIGKV, globalstatsbuffer2, NUM_PARTITIONS); }
			#endif
			saveglobalstats03(config3.enablepartition, kvdram3, globalstatsbuffer3, BASEOFFSET_STATSDRAM_KVS + deststatsmarker3);
			#if defined(_DEBUGMODE_CHECKS2) && defined(ENABLE_PERFECTACCURACY)
			if(config3.enablereduce == OFF){ actsutilityobj->checkforgreaterthan("maxdispatch. comparing BIGKV3 & globalstatsbuffer3", BIGKV, globalstatsbuffer3, NUM_PARTITIONS); }
			#endif
			#endif
			
			if(currentLOP > 0){
				sourcestatsmarker0 += 1;
				deststatsmarker0 += NUM_PARTITIONS;
				destoffset0 = globalstatsbuffer0[NUM_PARTITIONS-1].key + globalstatsbuffer0[NUM_PARTITIONS-1].value + 64; }
			if(currentLOP > 0){
				sourcestatsmarker1 += 1;
				deststatsmarker1 += NUM_PARTITIONS;
				destoffset1 = globalstatsbuffer1[NUM_PARTITIONS-1].key + globalstatsbuffer1[NUM_PARTITIONS-1].value + 64; }
			if(currentLOP > 0){
				sourcestatsmarker2 += 1;
				deststatsmarker2 += NUM_PARTITIONS;
				destoffset2 = globalstatsbuffer2[NUM_PARTITIONS-1].key + globalstatsbuffer2[NUM_PARTITIONS-1].value + 64; }
			if(currentLOP > 0){
				sourcestatsmarker3 += 1;
				deststatsmarker3 += NUM_PARTITIONS;
				destoffset3 = globalstatsbuffer3[NUM_PARTITIONS-1].key + globalstatsbuffer3[NUM_PARTITIONS-1].value + 64; }
			
			#ifdef _DEBUGMODE_KERNELPRINTS2 // REMOVEME.
			if(config0.enablereduce == OFF){ actsutilityobj->printpartitionresult2(OFF, kvdram0, globalstatsbuffer0, sweepparams0); }
			if(config1.enablereduce == OFF){ actsutilityobj->printpartitionresult2(OFF, kvdram1, globalstatsbuffer1, sweepparams1); }
			if(config2.enablereduce == OFF){ actsutilityobj->printpartitionresult2(OFF, kvdram2, globalstatsbuffer2, sweepparams2); }
			if(config3.enablereduce == OFF){ actsutilityobj->printpartitionresult2(OFF, kvdram3, globalstatsbuffer3, sweepparams3); }
			#endif
		}
		#if defined(_DEBUGMODE_KERNELPRINTS2)
		actsutilityobj->printglobalvars();
		#endif 
		#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(_DEBUGMODE_CHECKS2)
		actsutilityobj->clearglobalvars();
		#endif
	}
	#ifdef _DEBUGMODE_CHECKS
	actsutilityobj->countvalueslessthan("maxdispatch", (value_t *)&kvdram0[BASEOFFSET_VERTICESDATA_KVS], BATCH_RANGE, INFINITI);
	actsutilityobj->countvalueslessthan("maxdispatch", (value_t *)&kvdram1[BASEOFFSET_VERTICESDATA_KVS], BATCH_RANGE, INFINITI);
	actsutilityobj->countvalueslessthan("maxdispatch", (value_t *)&kvdram2[BASEOFFSET_VERTICESDATA_KVS], BATCH_RANGE, INFINITI);
	actsutilityobj->countvalueslessthan("maxdispatch", (value_t *)&kvdram3[BASEOFFSET_VERTICESDATA_KVS], BATCH_RANGE, INFINITI);
	#endif 
	return;
}

// top
extern "C" {
void 
	#ifdef SW 
	actslw_maxbutil:: 
	#endif
topkernel(
  uint512_dt * sourceA0volume  ,uint512_dt * sourceA1volume  ,uint512_dt * sourceA2volume  ,uint512_dt * sourceA3volume 	){
#pragma HLS INTERFACE m_axi port = sourceA0volume offset = slave bundle = gmem0 // max_read_burst_length=64 max_write_burst_length=64
#pragma HLS INTERFACE m_axi port = sourceA1volume offset = slave bundle = gmem1 // max_read_burst_length=64 max_write_burst_length=64
#pragma HLS INTERFACE m_axi port = sourceA2volume offset = slave bundle = gmem2 // max_read_burst_length=64 max_write_burst_length=64
#pragma HLS INTERFACE m_axi port = sourceA3volume offset = slave bundle = gmem3 // max_read_burst_length=64 max_write_burst_length=64

#pragma HLS INTERFACE s_axilite port = sourceA0volume bundle = control
#pragma HLS INTERFACE s_axilite port = sourceA1volume bundle = control
#pragma HLS INTERFACE s_axilite port = sourceA2volume bundle = control
#pragma HLS INTERFACE s_axilite port = sourceA3volume bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATA_PACK variable = sourceA0volume
#pragma HLS DATA_PACK variable = sourceA1volume
#pragma HLS DATA_PACK variable = sourceA2volume
#pragma HLS DATA_PACK variable = sourceA3volume
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	#ifdef _WIDEWORD
	cout<<">>> Light weight ACTS MAXBUTIL (L2) Launched... size: "<<(unsigned int)(sourceA0volume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].range(31, 0))<<endl; 
	cout<<">>> Light weight ACTS MAXBUTIL (L2) Launched... size: "<<(unsigned int)(sourceA1volume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].range(31, 0))<<endl; 
	cout<<">>> Light weight ACTS MAXBUTIL (L2) Launched... size: "<<(unsigned int)(sourceA2volume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].range(31, 0))<<endl; 
	cout<<">>> Light weight ACTS MAXBUTIL (L2) Launched... size: "<<(unsigned int)(sourceA3volume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].range(31, 0))<<endl; 
	#else
	cout<<">>> Light weight ACTS MAXBUTIL (L2) Launched... size: "<<sourceA0volume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key<<endl; 
	cout<<">>> Light weight ACTS MAXBUTIL (L2) Launched... size: "<<sourceA1volume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key<<endl; 
	cout<<">>> Light weight ACTS MAXBUTIL (L2) Launched... size: "<<sourceA2volume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key<<endl; 
	cout<<">>> Light weight ACTS MAXBUTIL (L2) Launched... size: "<<sourceA3volume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key<<endl; 
	#endif
	#endif
	
	maxdispatch0( sourceA0volume  ,sourceA1volume  ,sourceA2volume  ,sourceA3volume );
	return;
}
}







