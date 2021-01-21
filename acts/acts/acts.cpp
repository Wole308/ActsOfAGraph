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
#include "acts.h"
using namespace std;

#define NUMACTSFASTPIPELINES 2
#if NUMACTSFASTPIPELINES==1
#define FPP0
#endif 
#if NUMACTSFASTPIPELINES==2
#define FPP0
#define FPP1
#endif
#define OPTMZ
#ifdef SIMPLEANDFASTPREPAREFUNC
#define WORKBUFFER_SIZE (SRCBUFFER_SIZE - (NUM_PARTITIONS*4))
#else 
#define WORKBUFFER_SIZE SRCBUFFER_SIZE
#endif

#ifdef SW
acts::acts(){ actsutilityobj = new actsutility(); }
acts::~acts(){}
#endif
#ifdef SWEMU
actsutility * actsutilityobj = new actsutility();
#endif

// NOTE: ACTS NOT automated for all datasets. see CRITICAL AUTOMATEME. (dataset dependent).

// kernel utilities
unsigned int
	#ifdef SW 
	acts::
	#endif 
amin(unsigned int val1, unsigned int val2){
	if(val1 < val2){ return val1; }
	else { return val2; }
}
batch_type
	#ifdef SW 
	acts::
	#endif
allignlowerto4_KV(batch_type val){
	batch_type fac = val / 4;
	return (fac * 4);
}
batch_type
	#ifdef SW 
	acts::
	#endif 
allignhigherto4_KV(batch_type val){
	batch_type fac = (val + (4 - 1)) / 4;
	return (fac * 4);
}
batch_type
	#ifdef SW 
	acts::
	#endif
allignlower_KV(batch_type val){
	batch_type fac = val / VECTOR_SIZE;
	return (fac * VECTOR_SIZE);
}
batch_type
	#ifdef SW 
	acts::
	#endif 
allignhigher_KV(batch_type val){
	batch_type fac = (val + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	return (fac * VECTOR_SIZE);
}
batch_type
	#ifdef SW 
	acts::
	#endif
allignlowerto16_KV(batch_type val){
	batch_type fac = val / 16;
	return (fac * 16);
}
batch_type
	#ifdef SW 
	acts::
	#endif 
allignhigherto16_KV(batch_type val){
	batch_type fac = (val + (16 - 1)) / 16;
	return (fac * 16);
}
ulong_dt 
	#ifdef SW 
	acts::
	#endif
CONVERTTOLONG_KV(keyvalue_t keyvalue){
	ulong_dt data;
	#ifdef FPGA_IMPL
	data.range(31, 0) = keyvalue.key; 
	data.range(63, 32) = keyvalue.value; 
	return data;
	#else
	return data; // NOT IMPLEMENTED.
	#endif
}
keyvalue_t
	#ifdef SW 
	acts::
	#endif
CONVERTTOKV_ULONG(ulong_dt data){
	keyvalue_t keyvalue;
	#ifdef FPGA_IMPL
	keyvalue.key = data.range(31, 0); 
	keyvalue.value = data.range(63, 32); 
	return keyvalue;
	#else
	return keyvalue; // NOT IMPLEMENTED.
	#endif
}
unsigned int 
	#ifdef SW 
	acts::
	#endif
GETMASK_UINT(unsigned int index, unsigned int size){
	unsigned int A = ((1 << (size)) - 1);
	unsigned int B = A << index;
	return B;
}
ulong_dt 
	#ifdef SW 
	acts::
	#endif
GETMASK_ULONG(ulong_dt index, ulong_dt size){
	ulong_dt A = ((1 << (size)) - 1);
	ulong_dt B = A << index;
	return B;
}
unsigned int 
	#ifdef SW 
	acts::
	#endif
READFROM_UINT(unsigned int data, unsigned int index, unsigned int size){ 
	return (((data) & GETMASK_UINT((index), (size))) >> (index)); 
}
unsigned int 
	#ifdef SW 
	acts::
	#endif
READFROM_ULONG(ulong_dt data, ulong_dt index, ulong_dt size){ 
	return (((data) & GETMASK_ULONG((index), (size))) >> (index)); 
}
unsigned int 
	#ifdef SW 
	acts::
	#endif
READFROM_ULONG(keyvalue_t keyvalue, ulong_dt index, ulong_dt size){
	ulong_dt data;
	#ifdef SW
	ulong_dt * thisdata = (ulong_dt *)&keyvalue;
	data = *thisdata;
	#else
	data = CONVERTTOLONG_KV(keyvalue); // VHLS CRITICAL CHECKME.
	#endif 
	return READFROM_ULONG(data, index, size);
}
void
	#ifdef SW 
	acts::
	#endif
WRITETO_ULONG(ulong_dt * data, ulong_dt index, ulong_dt size, ulong_dt value){
	// NB: write(000...00, 0);
	
	ulong_dt tempdata = *data;
	ulong_dt A = ((value) << (index));
	ulong_dt B = (~GETMASK_ULONG((index), (size)));
	ulong_dt C = ((tempdata) & (B));
	ulong_dt D = (C) | A;
	*data = D;
	// (tempdata) = ((tempdata) & (~GETMASK_ULONG((index), (size)))) | ((value) << (index));
	// *data = tempdata;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"WRITETO_ULONG. index: "<<index<<", size: "<<size<<", value: "<<value<<endl;
	cout<<"WRITETO_ULONG. tempdata"<<endl; actsutilityobj->ULONGTOBINARY(tempdata);
	cout<<"WRITETO_ULONG. A"<<endl; actsutilityobj->ULONGTOBINARY(A);
	cout<<"WRITETO_ULONG. B (~mask)"<<endl; actsutilityobj->ULONGTOBINARY(B);
	cout<<"WRITETO_ULONG. C"<<endl; actsutilityobj->ULONGTOBINARY(C);
	cout<<"WRITETO_ULONG. D (result)"<<endl; actsutilityobj->ULONGTOBINARY(D);
	#endif
	return; 
}
void
	#ifdef SW 
	acts::
	#endif
WRITETO_ULONG(keyvalue_t * keyvalue, ulong_dt index, ulong_dt size, ulong_dt value){ 
	#ifdef _WIDEWORD
	ulong_dt data = CONVERTTOLONG_KV(*keyvalue); // CRITICAL CHECKME.
	WRITETO_ULONG(&data, index, size, value);
	*keyvalue = CONVERTTOKV_ULONG(data);
	return;
	#else 
	ulong_dt * data = (ulong_dt *)keyvalue;
	WRITETO_ULONG(data, index, size, value);
	return;
	#endif 
}
batch_type
	#ifdef SW 
	acts::
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
	acts::
	#endif 
resetkeyandvalues(skeyvalue_t * buffer, buffer_type size, unsigned int resetval){
	for(buffer_type i=0; i<size; i++){ buffer[i].key = resetval; buffer[i].value = resetval; }
}
void 
	#ifdef SW 
	acts::
	#endif 
resetvalues(keyvalue_t * buffer, buffer_type size, unsigned int resetval){
	for(buffer_type i=0; i<size; i++){ buffer[i].value = resetval; }
}
void 
	#ifdef SW 
	acts::
	#endif 
resetvalues(value_t * buffer, buffer_type size, unsigned int resetval){
	for(buffer_type i=0; i<size; i++){ buffer[i] = resetval; }
}
void 
	#ifdef SW 
	acts::
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
	acts::
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
	acts::
	#endif 
resetmanykeyandvalues(keyvalue_t * buffer, buffer_type size, unsigned int resetval){
	for(buffer_type i=0; i<size; i++){
		buffer[i].key = resetval; buffer[i].value = resetval; 
	}
	return;
}
void 
	#ifdef SW 
	acts::
	#endif 
resetmanykeyandvalues(skeyvalue_t * buffer, buffer_type size, unsigned int resetval){
	for(buffer_type i=0; i<size; i++){
		buffer[i].key = resetval; buffer[i].value = resetval; 
	}
	return;
}
void 
	#ifdef SW 
	acts::
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
	acts::
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
	acts::
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
	acts::
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
	acts::
	#endif 
getglobalpartition(keyvalue_t keyvalue, vertex_t upperlimit, unsigned int batch_range_pow){
	partition_type partition = ((keyvalue.key - upperlimit) >> (batch_range_pow - (NUM_PARTITIONS_POW * 2))); 
	
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("acts::getglobalpartition", partition, 256, keyvalue.key, upperlimit, NAp);
	#endif
	return partition;
}
value_t 
	#ifdef SW 
	acts::
	#endif 
processedgefunc(value_t Uprop, unsigned int edgeweight, unsigned int voutdegree, unsigned int GraphIter, unsigned int GraphAlgo){
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
}
buffer_type 
	#ifdef SW 
	acts::
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
		#if defined(SW) || defined(SWEMU) || defined(HW) 
		if((bramoffset_kvs + size_kvs) >= PADDEDDESTBUFFER_SIZE){ size_kvs = PADDEDDESTBUFFER_SIZE - bramoffset_kvs - 1; } 
		#endif
	#else 
		size_kvs = realsize_kvs;
	#endif
	return size_kvs;
}
buffer_type
	#ifdef SW 
	acts::
	#endif 
withinvalidrange(buffer_type val1, buffer_type val2){
	// #pragma HLS INLINE
	if(val1 < val2){ return 1; }
	else { return 0; }
}
void 
	#ifdef SW 
	acts::
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
	acts::
	#endif 
calculateunallignedoffsets(skeyvalue_t buffer[NUM_PARTITIONS], buffer_type size, batch_type base, batch_type skipspacing){
	for(buffer_type i=1; i<size; i++){ 
		buffer[i].key = buffer[i-1].key + buffer[i-1].value + skipspacing; 
	}
	return;
}
void 
	#ifdef SW 
	acts::
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
	acts::
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
	acts::
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
	acts::
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
	acts::
	#endif 
getvaluecount(keyvalue_t * keyvalues, unsigned int size){
	unsigned int totalnumkeyvalues = 0;
	for(unsigned int p=0; p<size; p++){ totalnumkeyvalues += keyvalues[p].value; }
	return totalnumkeyvalues;
}
batch_type
	#ifdef SW 
	acts::
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
	acts::
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
	globalparams.applyvertexbuffersz_pow = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_APPLYVERTEXBUFFERSZ_POW].range(31, 0);
	globalparams.loadfactorforreduce = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_LOADFACTORFORREDUCE].range(31, 0);
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
	globalparams.applyvertexbuffersz_pow = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_APPLYVERTEXBUFFERSZ_POW].data[0].key;
	globalparams.loadfactorforreduce = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_LOADFACTORFORREDUCE].data[0].key;
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
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->printglobalparameters("acts::getglobalparams:: printing global parameters", globalparams);
	#endif
	return globalparams;
}
sweepparams_t 
	#ifdef SW 
	acts::
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

	unsigned int div = globalparams.batch_range;
	analysis_type analysis_lc = TREE_DEPTH;
	for(unsigned int i=0; i<globalparams.treedepth; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_lc avg=analysis_lc	
		div = div / NUM_PARTITIONS;
	}
	sweepparams.upperlimit = globalparams.destvoffset + (source_partition * sourceskipsize * div);
	// sweepparams.upperlimit = globalparams.destvoffset + (source_partition * sourceskipsize * (globalparams.batch_range / globalparams.LLOPnumpartitions));
	sweepparams.upperpartition = (sweepparams.upperlimit - globalparams.destvoffset) >> (globalparams.batch_range_pow - (NUM_PARTITIONS_POW * currentLOP)); //

	sweepparams.source_partition = source_partition;
	return sweepparams;
}
travstate_t 
	#ifdef SW 
	acts::
	#endif 
gettravstate(bool_type enable, uint512_dt * kvdram, globalparams_t globalparams, step_type currentLOP, batch_type sourcestatsmarker, batch_type source_partition, keyvalue_t travstates[MAXLOADFACTORFORREDUCE]){			
	#pragma HLS function_instantiate variable=kvdram
	analysis_type analysis_loop1 = LOADFACTORFORREDUCE;
	travstate_t travstate;
	if(enable == OFF){ return travstate; } // NEWCHANGE.
	keyvalue_t keyvalue;
	keyvalue_t nextkeyvalue;
	
	if(currentLOP == 0){ keyvalue.key = 0; }
	else if(currentLOP == 1){ keyvalue.key = 0; }
	else { 
		#ifdef _WIDEWORD
		keyvalue.key = kvdram[globalparams.baseoffset_statsdram_kvs + sourcestatsmarker].range(31, 0); 
		keyvalue.value = kvdram[globalparams.baseoffset_statsdram_kvs + sourcestatsmarker].range(63, 32);
		for(batch_type k=0; k<globalparams.loadfactorforreduce; k++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loop1 avg=analysis_loop1
			travstates[k].key = kvdram[globalparams.baseoffset_statsdram_kvs + sourcestatsmarker + k].range(31, 0);
			travstates[k].value = kvdram[globalparams.baseoffset_statsdram_kvs + sourcestatsmarker + k].range(63, 32);
		}
		#else
		keyvalue = kvdram[globalparams.baseoffset_statsdram_kvs + sourcestatsmarker].data[0]; 
		for(batch_type k=0; k<globalparams.loadfactorforreduce; k++){
			travstates[k] = kvdram[globalparams.baseoffset_statsdram_kvs + sourcestatsmarker + k].data[0];
		}
		#endif
	}
	
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
	acts::
	#endif
inprocessedgesstage(unsigned int currentLOP){
	#ifdef MERGEPROCESSEDGESANDPARTITIONSTAGE
	if(currentLOP == 1){ return true; } 
	else { return false; }
	#else 
	if(currentLOP == 0){ return true; }
	else { return false; }
	#endif 
}
bool 
	#ifdef SW 
	acts::
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
	acts::
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
	acts::
	#endif
inreducestage(unsigned int currentLOP, globalparams_t globalparams){
	if(currentLOP == (globalparams.treedepth + 1)){ return true; }
	else { return false; }
}
void 
	#ifdef SW 
	acts::
	#endif
calculateglobaloffsets(keyvalue_t * globalstatsbuffer, batch_type * skipsizes,  batch_type offset, batch_type size){
	for(partition_type p=0; p<size; p++){ batch_type A = (globalstatsbuffer[p].value + (VECTOR_SIZE-1)) / VECTOR_SIZE; batch_type B = (A + (SRCBUFFER_SIZE-1)) / SRCBUFFER_SIZE; if(B < 80){ B = B * 2; } batch_type C = ((4 * 4 * 2) * NUM_PARTITIONS) + VECTOR_SIZE; skipsizes[p] = (B * C) + 128; }
	calculateoffsets(globalstatsbuffer, size, offset, skipsizes);
	return;
}
keyvalue_t 
	#ifdef SW 
	acts::
	#endif
getkeyvalue(uint512_dt * keyvalues, unsigned int offset_kvs, vector_type v){
	uint512_vec_dt D;
	#ifdef _WIDEWORD
	D.data[0].key = keyvalues[offset_kvs].range(31, 0);
	D.data[0].value = keyvalues[offset_kvs].range(63, 32);
	D.data[1].key = keyvalues[offset_kvs].range(95, 64);
	D.data[1].value = keyvalues[offset_kvs].range(127, 96);
	D.data[2].key = keyvalues[offset_kvs].range(159, 128);
	D.data[2].value = keyvalues[offset_kvs].range(191, 160);
	D.data[3].key = keyvalues[offset_kvs].range(223, 192);
	D.data[3].value = keyvalues[offset_kvs].range(255, 224);
	D.data[4].key = keyvalues[offset_kvs].range(287, 256);
	D.data[4].value = keyvalues[offset_kvs].range(319, 288);
	D.data[5].key = keyvalues[offset_kvs].range(351, 320);
	D.data[5].value = keyvalues[offset_kvs].range(383, 352);
	D.data[6].key = keyvalues[offset_kvs].range(415, 384);
	D.data[6].value = keyvalues[offset_kvs].range(447, 416);
	D.data[7].key = keyvalues[offset_kvs].range(479, 448);
	D.data[7].value = keyvalues[offset_kvs].range(511, 480);
	#else
	D.data[0] = keyvalues[offset_kvs].data[0];
	D.data[1] = keyvalues[offset_kvs].data[1];
	D.data[2] = keyvalues[offset_kvs].data[2];
	D.data[3] = keyvalues[offset_kvs].data[3];
	D.data[4] = keyvalues[offset_kvs].data[4];
	D.data[5] = keyvalues[offset_kvs].data[5];
	D.data[6] = keyvalues[offset_kvs].data[6];
	D.data[7] = keyvalues[offset_kvs].data[7];
	#endif 
	return D.data[v];
}
uint512_vec_dt 
	#ifdef SW 
	acts::
	#endif
getkeyvalues(uint512_dt * keyvalues, unsigned int offset_kvs){
	#pragma HLS INLINE
	uint512_vec_dt D;
	#pragma HLS ARRAY_PARTITION variable=D complete
	
	#ifdef _WIDEWORD
	D.data[0].key = keyvalues[offset_kvs].range(31, 0);
	D.data[0].value = keyvalues[offset_kvs].range(63, 32);
	D.data[1].key = keyvalues[offset_kvs].range(95, 64);
	D.data[1].value = keyvalues[offset_kvs].range(127, 96);
	D.data[2].key = keyvalues[offset_kvs].range(159, 128);
	D.data[2].value = keyvalues[offset_kvs].range(191, 160);
	D.data[3].key = keyvalues[offset_kvs].range(223, 192);
	D.data[3].value = keyvalues[offset_kvs].range(255, 224);
	D.data[4].key = keyvalues[offset_kvs].range(287, 256);
	D.data[4].value = keyvalues[offset_kvs].range(319, 288);
	D.data[5].key = keyvalues[offset_kvs].range(351, 320);
	D.data[5].value = keyvalues[offset_kvs].range(383, 352);
	D.data[6].key = keyvalues[offset_kvs].range(415, 384);
	D.data[6].value = keyvalues[offset_kvs].range(447, 416);
	D.data[7].key = keyvalues[offset_kvs].range(479, 448);
	D.data[7].value = keyvalues[offset_kvs].range(511, 480);
	#else
	D.data[0] = keyvalues[offset_kvs].data[0];
	D.data[1] = keyvalues[offset_kvs].data[1];
	D.data[2] = keyvalues[offset_kvs].data[2];
	D.data[3] = keyvalues[offset_kvs].data[3];
	D.data[4] = keyvalues[offset_kvs].data[4];
	D.data[5] = keyvalues[offset_kvs].data[5];
	D.data[6] = keyvalues[offset_kvs].data[6];
	D.data[7] = keyvalues[offset_kvs].data[7];
	#endif 
	return D;
}
void 
	#ifdef SW 
	acts::
	#endif
setkeyvalue(uint512_dt * keyvalues, unsigned int offset_kvs, vector_type v, keyvalue_t keyvalue){
	/* #ifdef _WIDEWORD
	keyvalues[offset_kvs].range(31, 0) = keyvalue.key;
	keyvalues[offset_kvs].range(63, 32) = keyvalue.value;
	keyvalues[offset_kvs].range(95, 64) = keyvalue.key;
	keyvalues[offset_kvs].range(127, 96) = keyvalue.value;
	keyvalues[offset_kvs].range(159, 128) = keyvalue.key;
	keyvalues[offset_kvs].range(191, 160) = keyvalue.value;
	keyvalues[offset_kvs].range(223, 192) = keyvalue.key;
	keyvalues[offset_kvs].range(255, 224) = keyvalue.value;
	keyvalues[offset_kvs].range(287, 256) = keyvalue.key;
	keyvalues[offset_kvs].range(319, 288) = keyvalue.value;
	keyvalues[offset_kvs].range(351, 320) = keyvalue.key;
	keyvalues[offset_kvs].range(383, 352) = keyvalue.value;
	keyvalues[offset_kvs].range(415, 384) = keyvalue.key;
	keyvalues[offset_kvs].range(447, 416) = keyvalue.value;
	keyvalues[offset_kvs].range(479, 448) = keyvalue.key;
	keyvalues[offset_kvs].range(511, 480) = keyvalue.value;
	#else
	keyvalues[offset_kvs].data[0] = keyvalue;
	keyvalues[offset_kvs].data[1] = keyvalue;
	keyvalues[offset_kvs].data[2] = keyvalue;
	keyvalues[offset_kvs].data[3] = keyvalue;
	keyvalues[offset_kvs].data[4] = keyvalue;
	keyvalues[offset_kvs].data[5] = keyvalue;
	keyvalues[offset_kvs].data[6] = keyvalue;
	keyvalues[offset_kvs].data[7] = keyvalue;
	#endif  */
	
	
	uint512_vec_dt D = getkeyvalues(keyvalues, offset_kvs);
	// #ifdef _WIDEWORD
	// 	// D.data[0].key = keyvalues[offset_kvs].range(31, 0);
	// D.data[0].value = keyvalues[offset_kvs].range(63, 32);
	// 	// D.data[1].key = keyvalues[offset_kvs].range(95, 64);
	// D.data[1].value = keyvalues[offset_kvs].range(127, 96);
	// 	// D.data[2].key = keyvalues[offset_kvs].range(159, 128);
	// D.data[2].value = keyvalues[offset_kvs].range(191, 160);
	// 	// D.data[3].key = keyvalues[offset_kvs].range(223, 192);
	// D.data[3].value = keyvalues[offset_kvs].range(255, 224);
	// 	// D.data[4].key = keyvalues[offset_kvs].range(287, 256);
	// D.data[4].value = keyvalues[offset_kvs].range(319, 288);
	// 	// D.data[5].key = keyvalues[offset_kvs].range(351, 320);
	// D.data[5].value = keyvalues[offset_kvs].range(383, 352);
	// 	// D.data[6].key = keyvalues[offset_kvs].range(415, 384);
	// D.data[6].value = keyvalues[offset_kvs].range(447, 416);
	// 	// D.data[7].key = keyvalues[offset_kvs].range(479, 448);
	// D.data[7].value = keyvalues[offset_kvs].range(511, 480);
	// 	// #else
	// 	// D.data[0] = keyvalues[offset_kvs].data[0];
	// 	// D.data[1] = keyvalues[offset_kvs].data[1];
	// 	// D.data[2] = keyvalues[offset_kvs].data[2];
	// 	// D.data[3] = keyvalues[offset_kvs].data[3];
	// 	// D.data[4] = keyvalues[offset_kvs].data[4];
	// 	// D.data[5] = keyvalues[offset_kvs].data[5];
	// 	// D.data[6] = keyvalues[offset_kvs].data[6];
	// 	// D.data[7] = keyvalues[offset_kvs].data[7];
	// 	// #endif 
	
	D.data[v] = keyvalue;
	
	#ifdef _WIDEWORD
	keyvalues[offset_kvs].range(31, 0) = D.data[0].key;
	keyvalues[offset_kvs].range(63, 32) = D.data[0].value;
	keyvalues[offset_kvs].range(95, 64) = D.data[1].key;
	keyvalues[offset_kvs].range(127, 96) = D.data[1].value;
	keyvalues[offset_kvs].range(159, 128) = D.data[2].key;
	keyvalues[offset_kvs].range(191, 160) = D.data[2].value;
	keyvalues[offset_kvs].range(223, 192) = D.data[3].key;
	keyvalues[offset_kvs].range(255, 224) = D.data[3].value;
	keyvalues[offset_kvs].range(287, 256) = D.data[4].key;
	keyvalues[offset_kvs].range(319, 288) = D.data[4].value;
	keyvalues[offset_kvs].range(351, 320) = D.data[5].key;
	keyvalues[offset_kvs].range(383, 352) = D.data[5].value;
	keyvalues[offset_kvs].range(415, 384) = D.data[6].key;
	keyvalues[offset_kvs].range(447, 416) = D.data[6].value;
	keyvalues[offset_kvs].range(479, 448) = D.data[7].key;
	keyvalues[offset_kvs].range(511, 480) = D.data[7].value;
	#else
	keyvalues[offset_kvs].data[0] = D.data[0];
	keyvalues[offset_kvs].data[1] = D.data[1];
	keyvalues[offset_kvs].data[2] = D.data[2];
	keyvalues[offset_kvs].data[3] = D.data[3];
	keyvalues[offset_kvs].data[4] = D.data[4];
	keyvalues[offset_kvs].data[5] = D.data[5];
	keyvalues[offset_kvs].data[6] = D.data[6];
	keyvalues[offset_kvs].data[7] = D.data[7];
	#endif 
	
	return;
}
void 
	#ifdef SW 
	acts::
	#endif
setkeyvalues(uint512_dt * keyvalues, unsigned int offset_kvs, uint512_vec_dt D){
	#ifdef _WIDEWORD
	keyvalues[offset_kvs].range(31, 0) = D.data[0].key;
	keyvalues[offset_kvs].range(63, 32) = D.data[0].value;
	keyvalues[offset_kvs].range(95, 64) = D.data[1].key;
	keyvalues[offset_kvs].range(127, 96) = D.data[1].value;
	keyvalues[offset_kvs].range(159, 128) = D.data[2].key;
	keyvalues[offset_kvs].range(191, 160) = D.data[2].value;
	keyvalues[offset_kvs].range(223, 192) = D.data[3].key;
	keyvalues[offset_kvs].range(255, 224) = D.data[3].value;
	keyvalues[offset_kvs].range(287, 256) = D.data[4].key;
	keyvalues[offset_kvs].range(319, 288) = D.data[4].value;
	keyvalues[offset_kvs].range(351, 320) = D.data[5].key;
	keyvalues[offset_kvs].range(383, 352) = D.data[5].value;
	keyvalues[offset_kvs].range(415, 384) = D.data[6].key;
	keyvalues[offset_kvs].range(447, 416) = D.data[6].value;
	keyvalues[offset_kvs].range(479, 448) = D.data[7].key;
	keyvalues[offset_kvs].range(511, 480) = D.data[7].value;
	#else
	keyvalues[offset_kvs].data[0] = D.data[0];
	keyvalues[offset_kvs].data[1] = D.data[1];
	keyvalues[offset_kvs].data[2] = D.data[2];
	keyvalues[offset_kvs].data[3] = D.data[3];
	keyvalues[offset_kvs].data[4] = D.data[4];
	keyvalues[offset_kvs].data[5] = D.data[5];
	keyvalues[offset_kvs].data[6] = D.data[6];
	keyvalues[offset_kvs].data[7] = D.data[7];
	#endif 
	return;
}
void 
	#ifdef SW 
	acts::
	#endif
setkey(uint512_dt * keyvalues, unsigned int offset_kvs, vector_type v, keyy_t key){
	// keyvalue_t keyvalue = getkeyvalue(keyvalues, offset_kvs, v); 
	// keyvalue.key = key;
	// setkeyvalue(keyvalues, offset_kvs, v, keyvalue);
	
	uint512_vec_dt D = getkeyvalues(keyvalues, offset_kvs);
	// #ifdef _WIDEWORD
	// 	// D.data[0].key = keyvalues[offset_kvs].range(31, 0);
	// D.data[0].value = keyvalues[offset_kvs].range(63, 32);
	// 	// D.data[1].key = keyvalues[offset_kvs].range(95, 64);
	// D.data[1].value = keyvalues[offset_kvs].range(127, 96);
	// 	// D.data[2].key = keyvalues[offset_kvs].range(159, 128);
	// D.data[2].value = keyvalues[offset_kvs].range(191, 160);
	// 	// D.data[3].key = keyvalues[offset_kvs].range(223, 192);
	// D.data[3].value = keyvalues[offset_kvs].range(255, 224);
	// 	// D.data[4].key = keyvalues[offset_kvs].range(287, 256);
	// D.data[4].value = keyvalues[offset_kvs].range(319, 288);
	// 	// D.data[5].key = keyvalues[offset_kvs].range(351, 320);
	// D.data[5].value = keyvalues[offset_kvs].range(383, 352);
	// 	// D.data[6].key = keyvalues[offset_kvs].range(415, 384);
	// D.data[6].value = keyvalues[offset_kvs].range(447, 416);
	// 	// D.data[7].key = keyvalues[offset_kvs].range(479, 448);
	// D.data[7].value = keyvalues[offset_kvs].range(511, 480);
	// 	// #else
	// 	// D.data[0] = keyvalues[offset_kvs].data[0];
	// 	// D.data[1] = keyvalues[offset_kvs].data[1];
	// 	// D.data[2] = keyvalues[offset_kvs].data[2];
	// 	// D.data[3] = keyvalues[offset_kvs].data[3];
	// 	// D.data[4] = keyvalues[offset_kvs].data[4];
	// 	// D.data[5] = keyvalues[offset_kvs].data[5];
	// 	// D.data[6] = keyvalues[offset_kvs].data[6];
	// 	// D.data[7] = keyvalues[offset_kvs].data[7];
	// 	// #endif 
	
	D.data[v].key = key;
	
	setkeyvalues(keyvalues, offset_kvs, D);
	
	// #ifdef _WIDEWORD
	// 	// keyvalues[offset_kvs].range(31, 0) = D.data[0].key;
	// keyvalues[offset_kvs].range(63, 32) = D.data[0].value;
	// 	// keyvalues[offset_kvs].range(95, 64) = D.data[1].key;
	// keyvalues[offset_kvs].range(127, 96) = D.data[1].value;
	// 	// keyvalues[offset_kvs].range(159, 128) = D.data[2].key;
	// keyvalues[offset_kvs].range(191, 160) = D.data[2].value;
	// 	// keyvalues[offset_kvs].range(223, 192) = D.data[3].key;
	// keyvalues[offset_kvs].range(255, 224) = D.data[3].value;
	// 	// keyvalues[offset_kvs].range(287, 256) = D.data[4].key;
	// keyvalues[offset_kvs].range(319, 288) = D.data[4].value;
	// 	// keyvalues[offset_kvs].range(351, 320) = D.data[5].key;
	// keyvalues[offset_kvs].range(383, 352) = D.data[5].value;
	// 	// keyvalues[offset_kvs].range(415, 384) = D.data[6].key;
	// keyvalues[offset_kvs].range(447, 416) = D.data[6].value;
	// 	// keyvalues[offset_kvs].range(479, 448) = D.data[7].key;
	// keyvalues[offset_kvs].range(511, 480) = D.data[7].value;
	// 	// #else
	// 	// keyvalues[offset_kvs].data[0] = D.data[0];
	// 	// keyvalues[offset_kvs].data[1] = D.data[1];
	// 	// keyvalues[offset_kvs].data[2] = D.data[2];
	// 	// keyvalues[offset_kvs].data[3] = D.data[3];
	// 	// keyvalues[offset_kvs].data[4] = D.data[4];
	// 	// keyvalues[offset_kvs].data[5] = D.data[5];
	// 	// keyvalues[offset_kvs].data[6] = D.data[6];
	// 	// keyvalues[offset_kvs].data[7] = D.data[7];
	// 	// #endif
	return;
}

// partition function
partition_type
	#ifdef SW 
	acts::
	#endif 
#if defined(INMEMORYGP) && defined(BFS_ALGORITHM)
getpartition(bool_type enable, keyvalue_t keyvalue, step_type currentLOP, vertex_t upperlimit, unsigned int upperpartition, unsigned int batch_range_pow){
	partition_type partition;
	if(enable == ON){
		if(keyvalue.value == INVALIDDATA){ partition = keyvalue.key; } 
		else {
			unsigned int llp = READFROM_UINT(keyvalue.value, 32-COMPACTPARAM_BITSIZE_STREETADDR, COMPACTPARAM_BITSIZE_STREETADDR);
			partition = (llp >> (NUM_PARTITIONS_POW * (TREE_DEPTH - currentLOP))) - upperpartition;
		}
	} else { partition = 0; }
	
	#ifdef _DEBUGMODE_CHECKS2
	if(partition >= NUM_PARTITIONS){ actsutilityobj->globalstats_counterrorsingetpartition(1); }
	#endif 
	#ifdef ENABLE_PERFECTACCURACY
		#ifdef _DEBUGMODE_CHECKS2
		if(partition >= NUM_PARTITIONS){ cout<<"acts::getpartition::ERROR 1. partition out of bounds partition: "<<partition<<", keyvalue.key: "<<keyvalue.key<<", keyvalue.value: "<<keyvalue.value<<", NUM_PARTITIONS: "<<NUM_PARTITIONS<<", upperlimit: "<<upperlimit<<", currentLOP: "<<currentLOP<<", batch_range_pow: "<<batch_range_pow<<endl; exit(EXIT_FAILURE); }
		#endif
	#endif 
	#ifndef ENABLE_PERFECTACCURACY
		if(partition >= NUM_PARTITIONS){ partition = 0; }
	#endif
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("acts::getpartition 2", partition, NUM_PARTITIONS, keyvalue.key, upperlimit, currentLOP);
	#endif
	return partition;
}
#else
getpartition(bool_type enable, keyvalue_t keyvalue, step_type currentLOP, vertex_t upperlimit, unsigned int upperpartition, unsigned int batch_range_pow){
	partition_type partition;
	if(enable == ON){ 
		if(keyvalue.value == INVALIDDATA){ partition = keyvalue.key; } 
		else { partition = ((keyvalue.key - upperlimit) >> (batch_range_pow - (NUM_PARTITIONS_POW * currentLOP))); }
	} else { partition = 0; }
	
	#ifdef _DEBUGMODE_CHECKS2
	if(partition >= NUM_PARTITIONS){ actsutilityobj->globalstats_counterrorsingetpartition(1); }
	#endif 
	#ifdef ENABLE_PERFECTACCURACY
		#ifdef _DEBUGMODE_CHECKS2
		if(partition >= NUM_PARTITIONS){ cout<<"acts::getpartition::ERROR 1. partition out of bounds partition: "<<partition<<", keyvalue.key: "<<keyvalue.key<<", keyvalue.value: "<<keyvalue.value<<", NUM_PARTITIONS: "<<NUM_PARTITIONS<<", upperlimit: "<<upperlimit<<", currentLOP: "<<currentLOP<<", batch_range_pow: "<<batch_range_pow<<endl; exit(EXIT_FAILURE); }
		#endif
	#endif 
	#ifndef ENABLE_PERFECTACCURACY
		if(partition >= NUM_PARTITIONS){ partition = (((1 << NUM_PARTITIONS_POW) - 1) & (partition >> (1 - 1))); } // FIXME. REMOVEME. PERFECTIONTEST.
	#endif
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("acts::getpartition 2", partition, NUM_PARTITIONS, keyvalue.key, upperlimit, currentLOP);
	#endif
	return partition;
}
#endif 

// collect globalstats functions 
void 
	#ifdef SW 
	acts::
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
	acts::
	#endif
collectglobalstats(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], step_type currentLOP, sweepparams_t sweepparams, travstate_t travstate, globalparams_t globalparams){					
	if(enable == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE;
	buffer_type chunk_size = getchunksize_kvs(SRCBUFFER_SIZE, travstate, 0);
	unsigned int upperlimit = sweepparams.upperlimit;
	unsigned int upperpartition = sweepparams.upperpartition;
	
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
		if(keyvalue0.key != INVALIDDATA){ p0 = getpartition(ON, keyvalue0, currentLOP, upperlimit, upperpartition, globalparams.batch_range_pow); }
		partition_type p1 = 0;
		if(keyvalue1.key != INVALIDDATA){ p1 = getpartition(ON, keyvalue1, currentLOP, upperlimit, upperpartition, globalparams.batch_range_pow); }
		partition_type p2 = 0;
		if(keyvalue2.key != INVALIDDATA){ p2 = getpartition(ON, keyvalue2, currentLOP, upperlimit, upperpartition, globalparams.batch_range_pow); }
		partition_type p3 = 0;
		if(keyvalue3.key != INVALIDDATA){ p3 = getpartition(ON, keyvalue3, currentLOP, upperlimit, upperpartition, globalparams.batch_range_pow); }
		partition_type p4 = 0;
		if(keyvalue4.key != INVALIDDATA){ p4 = getpartition(ON, keyvalue4, currentLOP, upperlimit, upperpartition, globalparams.batch_range_pow); }
		partition_type p5 = 0;
		if(keyvalue5.key != INVALIDDATA){ p5 = getpartition(ON, keyvalue5, currentLOP, upperlimit, upperpartition, globalparams.batch_range_pow); }
		partition_type p6 = 0;
		if(keyvalue6.key != INVALIDDATA){ p6 = getpartition(ON, keyvalue6, currentLOP, upperlimit, upperpartition, globalparams.batch_range_pow); }
		partition_type p7 = 0;
		if(keyvalue7.key != INVALIDDATA){ p7 = getpartition(ON, keyvalue7, currentLOP, upperlimit, upperpartition, globalparams.batch_range_pow); }
		
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
	acts::
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
	acts::
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
	acts::
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
	acts::
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
	acts::
	#endif 
readkeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], batch_type offset_kvs, batch_type size_kvs, travstate_t travstate){
	#pragma HLS function_instantiate variable=kvdram
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
		
		/* ///// REMOVEME.
		if(kvdram[offset_kvs + i].data[0].value == 1729101824){
			cout<<"acts::readkeyvalues:: kvdram[offset_kvs + i].data[0].key: "<<kvdram[offset_kvs + i].data[0].key<<endl;
			cout<<"acts::readkeyvalues:: kvdram[offset_kvs + i].data[0].value: "<<kvdram[offset_kvs + i].data[0].value<<endl;
			exit(EXIT_SUCCESS);
		}
		///// REMOVEME. */
		
		buffer[1][i] = kvdram[offset_kvs + i].data[1]; 
		
		/* ///// REMOVEME.
		if(kvdram[offset_kvs + i].data[1].value == 1729101824){
			cout<<"acts::readkeyvalues:: kvdram[offset_kvs + i].data[1].key: "<<kvdram[offset_kvs + i].data[1].key<<endl;
			cout<<"acts::readkeyvalues:: kvdram[offset_kvs + i].data[1].value: "<<kvdram[offset_kvs + i].data[1].value<<endl;
			exit(EXIT_SUCCESS);
		}
		///// REMOVEME. */
		
		buffer[2][i] = kvdram[offset_kvs + i].data[2]; 
		
		/* ///// REMOVEME.
		if(kvdram[offset_kvs + i].data[2].value == 1729101824){
			cout<<"acts::readkeyvalues:: kvdram[offset_kvs + i].data[2].key: "<<kvdram[offset_kvs + i].data[2].key<<endl;
			cout<<"acts::readkeyvalues:: kvdram[offset_kvs + i].data[2].value: "<<kvdram[offset_kvs + i].data[2].value<<endl;
			exit(EXIT_SUCCESS);
		}
		///// REMOVEME. */
		
		buffer[3][i] = kvdram[offset_kvs + i].data[3]; 
		
		/* ///// REMOVEME.
		if(kvdram[offset_kvs + i].data[3].value == 1729101824){
			cout<<"acts::readkeyvalues:: kvdram[offset_kvs + i].data[3].key: "<<kvdram[offset_kvs + i].data[3].key<<endl;
			cout<<"acts::readkeyvalues:: kvdram[offset_kvs + i].data[3].value: "<<kvdram[offset_kvs + i].data[3].value<<endl;
			exit(EXIT_SUCCESS);
		}
		///// REMOVEME. */
		
		buffer[4][i] = kvdram[offset_kvs + i].data[4]; 
		
		/* ///// REMOVEME.
		if(kvdram[offset_kvs + i].data[4].value == 1729101824){
			cout<<"acts::readkeyvalues:: kvdram[offset_kvs + i].data[4].key: "<<kvdram[offset_kvs + i].data[4].key<<endl;
			cout<<"acts::readkeyvalues:: kvdram[offset_kvs + i].data[4].value: "<<kvdram[offset_kvs + i].data[4].value<<endl;
			exit(EXIT_SUCCESS);
		}
		///// REMOVEME. */
		
		buffer[5][i] = kvdram[offset_kvs + i].data[5]; 
		
		/* ///// REMOVEME.
		if(kvdram[offset_kvs + i].data[5].value == 1729101824){
			cout<<"acts::readkeyvalues:: kvdram[offset_kvs + i].data[5].key: "<<kvdram[offset_kvs + i].data[5].key<<endl;
			cout<<"acts::readkeyvalues:: kvdram[offset_kvs + i].data[5].value: "<<kvdram[offset_kvs + i].data[5].value<<endl;
			exit(EXIT_SUCCESS);
		}
		///// REMOVEME. */
		
		buffer[6][i] = kvdram[offset_kvs + i].data[6]; 
		
		/* ///// REMOVEME.
		if(kvdram[offset_kvs + i].data[6].value == 1729101824){
			cout<<"acts::readkeyvalues:: kvdram[offset_kvs + i].data[6].key: "<<kvdram[offset_kvs + i].data[6].key<<endl;
			cout<<"acts::readkeyvalues:: kvdram[offset_kvs + i].data[6].value: "<<kvdram[offset_kvs + i].data[6].value<<endl;
			exit(EXIT_SUCCESS);
		}
		///// REMOVEME. */
		
		buffer[7][i] = kvdram[offset_kvs + i].data[7]; 
		
		/* ///// REMOVEME.
		if(kvdram[offset_kvs + i].data[7].value == 1729101824){
			cout<<"acts::readkeyvalues:: kvdram[offset_kvs + i].data[7].key: "<<kvdram[offset_kvs + i].data[7].key<<endl;
			cout<<"acts::readkeyvalues:: kvdram[offset_kvs + i].data[7].value: "<<kvdram[offset_kvs + i].data[7].value<<endl;
			exit(EXIT_SUCCESS);
		}
		///// REMOVEME. */
		
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
	acts::
	#endif
preparekeyvalues_coarsegrainedpipeline(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t localcapsule[VECTOR_SIZE][NUM_PARTITIONS], step_type currentLOP, sweepparams_t sweepparams, travstate_t travstate, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE;
	buffer_type chunk_size = getchunksize_kvs(SRCBUFFER_SIZE, travstate, 0);
	unsigned int upperlimit = sweepparams.upperlimit;
	unsigned int upperpartition = sweepparams.upperpartition;

	resetmanykeyandvalues(localcapsule, NUM_PARTITIONS, 0);
	
	PREPAREKEYVALUES_LOOP1: for(step_type c=0; c<2; c++){
		PREPAREKEYVALUES_LOOP1B: for(buffer_type i=0; i<chunk_size; i++){
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
			if(keyvalue0.key != INVALIDDATA){ p0 = getpartition(ON, keyvalue0, currentLOP, upperlimit, upperpartition, globalparams.batch_range_pow); } 
			partition_type p1 = 0;
			if(keyvalue1.key != INVALIDDATA){ p1 = getpartition(ON, keyvalue1, currentLOP, upperlimit, upperpartition, globalparams.batch_range_pow); } 
			partition_type p2 = 0;
			if(keyvalue2.key != INVALIDDATA){ p2 = getpartition(ON, keyvalue2, currentLOP, upperlimit, upperpartition, globalparams.batch_range_pow); } 
			partition_type p3 = 0;
			if(keyvalue3.key != INVALIDDATA){ p3 = getpartition(ON, keyvalue3, currentLOP, upperlimit, upperpartition, globalparams.batch_range_pow); } 
			partition_type p4 = 0;
			if(keyvalue4.key != INVALIDDATA){ p4 = getpartition(ON, keyvalue4, currentLOP, upperlimit, upperpartition, globalparams.batch_range_pow); } 
			partition_type p5 = 0;
			if(keyvalue5.key != INVALIDDATA){ p5 = getpartition(ON, keyvalue5, currentLOP, upperlimit, upperpartition, globalparams.batch_range_pow); } 
			partition_type p6 = 0;
			if(keyvalue6.key != INVALIDDATA){ p6 = getpartition(ON, keyvalue6, currentLOP, upperlimit, upperpartition, globalparams.batch_range_pow); } 
			partition_type p7 = 0;
			if(keyvalue7.key != INVALIDDATA){ p7 = getpartition(ON, keyvalue7, currentLOP, upperlimit, upperpartition, globalparams.batch_range_pow); } 
			
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("preparekeyvalues_coarsegrainedpipeline", localcapsule[0][p0].key + localcapsule[0][p0].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("preparekeyvalues_coarsegrainedpipeline", localcapsule[1][p1].key + localcapsule[1][p1].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("preparekeyvalues_coarsegrainedpipeline", localcapsule[2][p2].key + localcapsule[2][p2].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("preparekeyvalues_coarsegrainedpipeline", localcapsule[3][p3].key + localcapsule[3][p3].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("preparekeyvalues_coarsegrainedpipeline", localcapsule[4][p4].key + localcapsule[4][p4].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("preparekeyvalues_coarsegrainedpipeline", localcapsule[5][p5].key + localcapsule[5][p5].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("preparekeyvalues_coarsegrainedpipeline", localcapsule[6][p6].key + localcapsule[6][p6].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("preparekeyvalues_coarsegrainedpipeline", localcapsule[7][p7].key + localcapsule[7][p7].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
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
			actsutilityobj->checkoutofbounds("preparekeyvalues_coarsegrainedpipeline", p0, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("preparekeyvalues_coarsegrainedpipeline", p1, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("preparekeyvalues_coarsegrainedpipeline", p2, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("preparekeyvalues_coarsegrainedpipeline", p3, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("preparekeyvalues_coarsegrainedpipeline", p4, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("preparekeyvalues_coarsegrainedpipeline", p5, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("preparekeyvalues_coarsegrainedpipeline", p6, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("preparekeyvalues_coarsegrainedpipeline", p7, NUM_PARTITIONS, NAp, NAp, NAp);
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
	acts::
	#endif
preparekeyvalues1_evencutoffs(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t localcapsule[VECTOR_SIZE][NUM_PARTITIONS], step_type currentLOP, sweepparams_t sweepparams, travstate_t travstate, buffer_type size_kvs, globalparams_t globalparams){
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
	unsigned int upperlimit = sweepparams.upperlimit;
	unsigned int upperpartition = sweepparams.upperpartition;
	resetmanykeyandvalues(localcapsule, NUM_PARTITIONS, 0);
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int v=0; v<1; v++){ actsutilityobj->printkeyvalues("preparekeyvalues1_evencutoffs.destbuffer[0]", destbuffer[v], 32); }
	for(unsigned int v=0; v<1; v++){ actsutilityobj->printkeyvalues("preparekeyvalues1_evencutoffs.localcapsule[0]", (keyvalue_t *)localcapsule[v], NUM_PARTITIONS); }
	#endif
	
	PREPAREKEYVALUES_LOOP1: for(step_type c=0; c<2; c++){
		PREPAREKEYVALUES_LOOP1B: for(buffer_type i=0; i<chunk_size; i++){
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
			if(keyvalue0.value != INVALIDDATA){ p0 = getpartition(ON, keyvalue0, currentLOP, upperlimit, upperpartition, globalparams.batch_range_pow); } // NEWCHANGE.
			partition_type p1 = 0;
			if(keyvalue1.value != INVALIDDATA){ p1 = getpartition(ON, keyvalue1, currentLOP, upperlimit, upperpartition, globalparams.batch_range_pow); } // NEWCHANGE.
			partition_type p2 = 0;
			if(keyvalue2.value != INVALIDDATA){ p2 = getpartition(ON, keyvalue2, currentLOP, upperlimit, upperpartition, globalparams.batch_range_pow); } // NEWCHANGE.
			partition_type p3 = 0;
			if(keyvalue3.value != INVALIDDATA){ p3 = getpartition(ON, keyvalue3, currentLOP, upperlimit, upperpartition, globalparams.batch_range_pow); } // NEWCHANGE.
			partition_type p4 = 0;
			if(keyvalue4.value != INVALIDDATA){ p4 = getpartition(ON, keyvalue4, currentLOP, upperlimit, upperpartition, globalparams.batch_range_pow); } // NEWCHANGE.
			partition_type p5 = 0;
			if(keyvalue5.value != INVALIDDATA){ p5 = getpartition(ON, keyvalue5, currentLOP, upperlimit, upperpartition, globalparams.batch_range_pow); } // NEWCHANGE.
			partition_type p6 = 0;
			if(keyvalue6.value != INVALIDDATA){ p6 = getpartition(ON, keyvalue6, currentLOP, upperlimit, upperpartition, globalparams.batch_range_pow); } // NEWCHANGE.
			partition_type p7 = 0;
			if(keyvalue7.value != INVALIDDATA){ p7 = getpartition(ON, keyvalue7, currentLOP, upperlimit, upperpartition, globalparams.batch_range_pow); } // NEWCHANGE.
			
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("preparekeyvalues1_evencutoffs", localcapsule[0][p0].key + localcapsule[0][p0].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("preparekeyvalues1_evencutoffs", localcapsule[1][p1].key + localcapsule[1][p1].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("preparekeyvalues1_evencutoffs", localcapsule[2][p2].key + localcapsule[2][p2].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("preparekeyvalues1_evencutoffs", localcapsule[3][p3].key + localcapsule[3][p3].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("preparekeyvalues1_evencutoffs", localcapsule[4][p4].key + localcapsule[4][p4].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("preparekeyvalues1_evencutoffs", localcapsule[5][p5].key + localcapsule[5][p5].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("preparekeyvalues1_evencutoffs", localcapsule[6][p6].key + localcapsule[6][p6].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("preparekeyvalues1_evencutoffs", localcapsule[7][p7].key + localcapsule[7][p7].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
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
					actsutilityobj->checkoutofbounds("preparekeyvalues1_evencutoffs.loc0", loc0, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
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
					actsutilityobj->checkoutofbounds("preparekeyvalues1_evencutoffs.loc1", loc1, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
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
					actsutilityobj->checkoutofbounds("preparekeyvalues1_evencutoffs.loc2", loc2, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
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
					actsutilityobj->checkoutofbounds("preparekeyvalues1_evencutoffs.loc3", loc3, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
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
					actsutilityobj->checkoutofbounds("preparekeyvalues1_evencutoffs.loc4", loc4, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
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
					actsutilityobj->checkoutofbounds("preparekeyvalues1_evencutoffs.loc5", loc5, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
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
					actsutilityobj->checkoutofbounds("preparekeyvalues1_evencutoffs.loc6", loc6, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
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
					actsutilityobj->checkoutofbounds("preparekeyvalues1_evencutoffs.loc7", loc7, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
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
			actsutilityobj->checkoutofbounds("preparekeyvalues1_evencutoffs.p0", p0, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("preparekeyvalues1_evencutoffs.p1", p1, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("preparekeyvalues1_evencutoffs.p2", p2, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("preparekeyvalues1_evencutoffs.p3", p3, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("preparekeyvalues1_evencutoffs.p4", p4, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("preparekeyvalues1_evencutoffs.p5", p5, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("preparekeyvalues1_evencutoffs.p6", p6, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("preparekeyvalues1_evencutoffs.p7", p7, NUM_PARTITIONS, NAp, NAp, NAp);
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
	actsutilityobj->checkoutofbounds("preparekeyvalues1_evencutoffs:: cutoff", cutoff, SRCBUFFER_SIZE+1, NAp, NAp, NAp);
	// for(unsigned int v=0; v<VECTOR_SIZE; v++){ if(ovsz[v] != ovsz[0]){ cout<<"ovsz["<<v<<"] != ovsz[0]. cutoff: "<<cutoff<<". printint values and exiting..."<<endl; actsutilityobj->printvalues("preparekeyvalues1_evencutoffs.ovsz", ovsz, VECTOR_SIZE); exit(EXIT_FAILURE); }} // FIXME.
	// for(unsigned int v=0; v<VECTOR_SIZE; v++){ actsutilityobj->checkforequal("preparekeyvalues1_evencutoffs:: ovsz["+std::to_string(v)+"].ovsz[0]", ovsz[v], ovsz[0], v, ovsz[0], NAp); }
	// actsutilityobj->checkforequal("preparekeyvalues1_evencutoffs.cutoff+ovsz[0],chunk_size", cutoff+ovsz[0], chunk_size, cutoff, ovsz[0], NAp); // REMOVEME.
	actsutilityobj->updatemincutoffseen(cutoff);
	actsutilityobj->updatemaxcutoffseen(cutoff);
	#endif
	#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_RUNKERNELPRINTS)
	cout<<"--------------- cutoff: "<<cutoff<<", ovsz[0]: "<<ovsz[0]<<", (cutoff+ovsz[0]): "<<cutoff+ovsz[0]<<", chunk_size: "<<chunk_size<<"  -----------------------------"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS
	unsigned int dummy[NUM_PARTITIONS];
	if(false){ actsutilityobj->printprofile("preparekeyvalues1_evencutoffs.destbuffer", (keyvalue_t *)&destbuffer[0], PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, currentLOP, upperlimit, globalparams.batch_range_pow); }
	if(false){ actsutilityobj->printprofileso1("preparekeyvalues1_evencutoffs.destbuffer", destbuffer, localcapsule, currentLOP, upperlimit, globalparams.batch_range_pow, dummy); }
	for(unsigned int v=0; v<8; v++){ actsutilityobj->printkeyvalues("preparekeyvalues1_evencutoffs.localcapsule[v]", (keyvalue_t *)localcapsule[v], NUM_PARTITIONS); }
	for(unsigned int v=0; v<8; v++){ actsutilityobj->printvalues("preparekeyvalues1_evencutoffs.limitsz[v]", (value_t *)limitsz[v], NUM_PARTITIONS); }
	#endif
	return cutoff;
}

buffer_type
	#ifdef SW 
	acts::
	#endif
preparekeyvalues2_evencutoffs(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t localcapsule[VECTOR_SIZE][NUM_PARTITIONS], step_type currentLOP, sweepparams_t sweepparams, travstate_t travstate, buffer_type size_kvs, globalparams_t globalparams){
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
	unsigned int upperlimit = sweepparams.upperlimit;
	unsigned int upperpartition = sweepparams.upperpartition;
	
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
	for(unsigned int v=0; v<1; v++){ actsutilityobj->printkeyvalues("preparekeyvalues2_evencutoffs.destbuffer[0]", destbuffer[v], 32); }
	for(unsigned int v=0; v<1; v++){ actsutilityobj->printkeyvalues("preparekeyvalues2_evencutoffs.localcapsule[0]", (keyvalue_t *)localcapsule[v], NUM_PARTITIONS); }
	#endif 
	
	PREPAREKEYVALUES_LOOP1: for(step_type c=0; c<2; c++){
		PREPAREKEYVALUES_LOOP1B: for(buffer_type i=0; i<chunk_size; i++){
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
			if(keyvalue0.key != INVALIDDATA){ p0 = getpartition(ON, keyvalue0, currentLOP, upperlimit, upperpartition, globalparams.batch_range_pow); } 
			partition_type p1 = 0;
			if(keyvalue1.key != INVALIDDATA){ p1 = getpartition(ON, keyvalue1, currentLOP, upperlimit, upperpartition, globalparams.batch_range_pow); } 
			partition_type p2 = 0;
			if(keyvalue2.key != INVALIDDATA){ p2 = getpartition(ON, keyvalue2, currentLOP, upperlimit, upperpartition, globalparams.batch_range_pow); } 
			partition_type p3 = 0;
			if(keyvalue3.key != INVALIDDATA){ p3 = getpartition(ON, keyvalue3, currentLOP, upperlimit, upperpartition, globalparams.batch_range_pow); } 
			partition_type p4 = 0;
			if(keyvalue4.key != INVALIDDATA){ p4 = getpartition(ON, keyvalue4, currentLOP, upperlimit, upperpartition, globalparams.batch_range_pow); } 
			partition_type p5 = 0;
			if(keyvalue5.key != INVALIDDATA){ p5 = getpartition(ON, keyvalue5, currentLOP, upperlimit, upperpartition, globalparams.batch_range_pow); } 
			partition_type p6 = 0;
			if(keyvalue6.key != INVALIDDATA){ p6 = getpartition(ON, keyvalue6, currentLOP, upperlimit, upperpartition, globalparams.batch_range_pow); } 
			partition_type p7 = 0;
			if(keyvalue7.key != INVALIDDATA){ p7 = getpartition(ON, keyvalue7, currentLOP, upperlimit, upperpartition, globalparams.batch_range_pow); } 
			
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("preparekeyvalues2_evencutoffs", localcapsule[0][p0].key + templocalcapsule0[p0], PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("preparekeyvalues2_evencutoffs", localcapsule[1][p1].key + templocalcapsule1[p1], PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("preparekeyvalues2_evencutoffs", localcapsule[2][p2].key + templocalcapsule2[p2], PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("preparekeyvalues2_evencutoffs", localcapsule[3][p3].key + templocalcapsule3[p3], PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("preparekeyvalues2_evencutoffs", localcapsule[4][p4].key + templocalcapsule4[p4], PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("preparekeyvalues2_evencutoffs", localcapsule[5][p5].key + templocalcapsule5[p5], PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("preparekeyvalues2_evencutoffs", localcapsule[6][p6].key + templocalcapsule6[p6], PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("preparekeyvalues2_evencutoffs", localcapsule[7][p7].key + templocalcapsule7[p7], PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
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
			actsutilityobj->checkoutofbounds("preparekeyvalues2_evencutoffs", p0, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("preparekeyvalues2_evencutoffs", p1, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("preparekeyvalues2_evencutoffs", p2, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("preparekeyvalues2_evencutoffs", p3, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("preparekeyvalues2_evencutoffs", p4, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("preparekeyvalues2_evencutoffs", p5, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("preparekeyvalues2_evencutoffs", p6, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("preparekeyvalues2_evencutoffs", p7, NUM_PARTITIONS, NAp, NAp, NAp);
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
	actsutilityobj->checkoutofbounds("preparekeyvalues2_evencutoffs:: cutoff", cutoff, SRCBUFFER_SIZE+1, NAp, NAp, NAp);
	// for(unsigned int v=0; v<VECTOR_SIZE; v++){ if(ovsz[v] != ovsz[0]){ cout<<"ovsz["<<v<<"] != ovsz[0]. cutoff: "<<cutoff<<". printint values and exiting..."<<endl; actsutilityobj->printvalues("preparekeyvalues2_evencutoffs.ovsz", ovsz, VECTOR_SIZE); exit(EXIT_FAILURE); }} // FIXME.
	// for(unsigned int v=0; v<VECTOR_SIZE; v++){ actsutilityobj->checkforequal("preparekeyvalues2_evencutoffs:: ovsz["+std::to_string(v)+"].ovsz[0]", ovsz[v], ovsz[0], v, ovsz[0], NAp); }
	// actsutilityobj->checkforequal("preparekeyvalues2_evencutoffs.cutoff+ovsz[0],chunk_size", cutoff+ovsz[0], chunk_size, cutoff, ovsz[0], NAp); // REMOVEME.
	actsutilityobj->updatemincutoffseen(cutoff);
	actsutilityobj->updatemaxcutoffseen(cutoff);
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	unsigned int dummy[NUM_PARTITIONS];
	if(false){ actsutilityobj->printprofile("preparekeyvalues2_evencutoffs.destbuffer", (keyvalue_t *)&destbuffer[0], PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, currentLOP, upperlimit, globalparams.batch_range_pow); }
	if(false){ actsutilityobj->printprofileso1("preparekeyvalues2_evencutoffs.destbuffer", destbuffer, localcapsule, currentLOP, upperlimit, globalparams.batch_range_pow, dummy); }
	for(unsigned int v=0; v<8; v++){ actsutilityobj->printkeyvalues("preparekeyvalues2_evencutoffs.localcapsule[v]", (keyvalue_t *)localcapsule[v], NUM_PARTITIONS); }
	for(unsigned int v=0; v<8; v++){ actsutilityobj->printvalues("preparekeyvalues2_evencutoffs.limitsz[v]", (value_t *)limitsz[v], NUM_PARTITIONS); }
	#endif
	return cutoff;
}

buffer_type 
	#ifdef SW 
	acts::
	#endif
preparekeyvalues_unevencutoffs(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t localcapsule[VECTOR_SIZE][NUM_PARTITIONS], step_type currentLOP, sweepparams_t sweepparams, travstate_t travstate, buffer_type size_kvs, buffer_type cutoffs[VECTOR_SIZE], globalparams_t globalparams){
	if(enable == OFF){ return 0; }
	analysis_type analysis_srcbuffersz = WORKBUFFER_SIZE;
	analysis_type analysis_dummyfiller = SRCBUFFER_SIZE - WORKBUFFER_SIZE;
	
	value_t emptyslot[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=emptyslot complete
	resetvalues(emptyslot, VECTOR_SIZE, 0);
	
	buffer_type chunk_size = getchunksize_kvs(WORKBUFFER_SIZE, travstate, 0);
	unsigned int upperlimit = sweepparams.upperlimit;
	unsigned int upperpartition = sweepparams.upperpartition;
	
	for(partition_type p=0; p<NUM_PARTITIONS; p++){ 
		localcapsule[0][p].key = 0;
		localcapsule[0][p].value = 0; 
		localcapsule[1][p].key = 0;
		localcapsule[1][p].value = 0; 
		localcapsule[2][p].key = 0;
		localcapsule[2][p].value = 0; 
		localcapsule[3][p].key = 0;
		localcapsule[3][p].value = 0; 
		localcapsule[4][p].key = 0;
		localcapsule[4][p].value = 0; 
		localcapsule[5][p].key = 0;
		localcapsule[5][p].value = 0; 
		localcapsule[6][p].key = 0;
		localcapsule[6][p].value = 0; 
		localcapsule[7][p].key = 0;
		localcapsule[7][p].value = 0; 
	}

	PREPAREKEYVALUES_LOOP1: for(buffer_type i=0; i<chunk_size; i++){
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
		
		bool_type valid0 = ON;
		if(keyvalue0.key != INVALIDDATA && keyvalue0.value != INVALIDDATA){ valid0 = ON; } else { valid0 = OFF; }
		bool_type valid1 = ON;
		if(keyvalue1.key != INVALIDDATA && keyvalue1.value != INVALIDDATA){ valid1 = ON; } else { valid1 = OFF; }
		bool_type valid2 = ON;
		if(keyvalue2.key != INVALIDDATA && keyvalue2.value != INVALIDDATA){ valid2 = ON; } else { valid2 = OFF; }
		bool_type valid3 = ON;
		if(keyvalue3.key != INVALIDDATA && keyvalue3.value != INVALIDDATA){ valid3 = ON; } else { valid3 = OFF; }
		bool_type valid4 = ON;
		if(keyvalue4.key != INVALIDDATA && keyvalue4.value != INVALIDDATA){ valid4 = ON; } else { valid4 = OFF; }
		bool_type valid5 = ON;
		if(keyvalue5.key != INVALIDDATA && keyvalue5.value != INVALIDDATA){ valid5 = ON; } else { valid5 = OFF; }
		bool_type valid6 = ON;
		if(keyvalue6.key != INVALIDDATA && keyvalue6.value != INVALIDDATA){ valid6 = ON; } else { valid6 = OFF; }
		bool_type valid7 = ON;
		if(keyvalue7.key != INVALIDDATA && keyvalue7.value != INVALIDDATA){ valid7 = ON; } else { valid7 = OFF; }
		
		partition_type p0 = 0;
		if(valid0 == ON){ p0 = getpartition(ON, keyvalue0, currentLOP, upperlimit, upperpartition, globalparams.batch_range_pow); }
		partition_type p1 = 0;
		if(valid1 == ON){ p1 = getpartition(ON, keyvalue1, currentLOP, upperlimit, upperpartition, globalparams.batch_range_pow); }
		partition_type p2 = 0;
		if(valid2 == ON){ p2 = getpartition(ON, keyvalue2, currentLOP, upperlimit, upperpartition, globalparams.batch_range_pow); }
		partition_type p3 = 0;
		if(valid3 == ON){ p3 = getpartition(ON, keyvalue3, currentLOP, upperlimit, upperpartition, globalparams.batch_range_pow); }
		partition_type p4 = 0;
		if(valid4 == ON){ p4 = getpartition(ON, keyvalue4, currentLOP, upperlimit, upperpartition, globalparams.batch_range_pow); }
		partition_type p5 = 0;
		if(valid5 == ON){ p5 = getpartition(ON, keyvalue5, currentLOP, upperlimit, upperpartition, globalparams.batch_range_pow); }
		partition_type p6 = 0;
		if(valid6 == ON){ p6 = getpartition(ON, keyvalue6, currentLOP, upperlimit, upperpartition, globalparams.batch_range_pow); }
		partition_type p7 = 0;
		if(valid7 == ON){ p7 = getpartition(ON, keyvalue7, currentLOP, upperlimit, upperpartition, globalparams.batch_range_pow); }
		
		if(valid0 == ON){
			if(localcapsule[0][p0].value == 0){ 
				localcapsule[0][p0].key = emptyslot[0]; emptyslot[0] += 4;
			} else if(localcapsule[0][p0].value % 4 == 0){ 
				localcapsule[0][p0].key = emptyslot[0]; emptyslot[0] += 4;
			} else {}
		}
		if(valid1 == ON){
			if(localcapsule[1][p1].value == 0){ 
				localcapsule[1][p1].key = emptyslot[1]; emptyslot[1] += 4;
			} else if(localcapsule[1][p1].value % 4 == 0){ 
				localcapsule[1][p1].key = emptyslot[1]; emptyslot[1] += 4;
			} else {}
		}
		if(valid2 == ON){
			if(localcapsule[2][p2].value == 0){ 
				localcapsule[2][p2].key = emptyslot[2]; emptyslot[2] += 4;
			} else if(localcapsule[2][p2].value % 4 == 0){ 
				localcapsule[2][p2].key = emptyslot[2]; emptyslot[2] += 4;
			} else {}
		}
		if(valid3 == ON){
			if(localcapsule[3][p3].value == 0){ 
				localcapsule[3][p3].key = emptyslot[3]; emptyslot[3] += 4;
			} else if(localcapsule[3][p3].value % 4 == 0){ 
				localcapsule[3][p3].key = emptyslot[3]; emptyslot[3] += 4;
			} else {}
		}
		if(valid4 == ON){
			if(localcapsule[4][p4].value == 0){ 
				localcapsule[4][p4].key = emptyslot[4]; emptyslot[4] += 4;
			} else if(localcapsule[4][p4].value % 4 == 0){ 
				localcapsule[4][p4].key = emptyslot[4]; emptyslot[4] += 4;
			} else {}
		}
		if(valid5 == ON){
			if(localcapsule[5][p5].value == 0){ 
				localcapsule[5][p5].key = emptyslot[5]; emptyslot[5] += 4;
			} else if(localcapsule[5][p5].value % 4 == 0){ 
				localcapsule[5][p5].key = emptyslot[5]; emptyslot[5] += 4;
			} else {}
		}
		if(valid6 == ON){
			if(localcapsule[6][p6].value == 0){ 
				localcapsule[6][p6].key = emptyslot[6]; emptyslot[6] += 4;
			} else if(localcapsule[6][p6].value % 4 == 0){ 
				localcapsule[6][p6].key = emptyslot[6]; emptyslot[6] += 4;
			} else {}
		}
		if(valid7 == ON){
			if(localcapsule[7][p7].value == 0){ 
				localcapsule[7][p7].key = emptyslot[7]; emptyslot[7] += 4;
			} else if(localcapsule[7][p7].value % 4 == 0){ 
				localcapsule[7][p7].key = emptyslot[7]; emptyslot[7] += 4;
			} else {}
		}
		
		buffer_type loc0 = localcapsule[0][p0].key + (localcapsule[0][p0].value % 4);
		buffer_type loc1 = localcapsule[1][p1].key + (localcapsule[1][p1].value % 4);
		buffer_type loc2 = localcapsule[2][p2].key + (localcapsule[2][p2].value % 4);
		buffer_type loc3 = localcapsule[3][p3].key + (localcapsule[3][p3].value % 4);
		buffer_type loc4 = localcapsule[4][p4].key + (localcapsule[4][p4].value % 4);
		buffer_type loc5 = localcapsule[5][p5].key + (localcapsule[5][p5].value % 4);
		buffer_type loc6 = localcapsule[6][p6].key + (localcapsule[6][p6].value % 4);
		buffer_type loc7 = localcapsule[7][p7].key + (localcapsule[7][p7].value % 4);
		
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("preparekeyvalues_unevencutoffs.localcapsule[0][p0].value", localcapsule[0][p0].value % 4, 4, localcapsule[0][p0].value, localcapsule[0][p0].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues_unevencutoffs.loc0", loc0, SRCBUFFER_SIZE, localcapsule[0][p0].key, localcapsule[0][p0].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues_unevencutoffs.localcapsule[1][p1].value", localcapsule[1][p1].value % 4, 4, localcapsule[1][p1].value, localcapsule[1][p1].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues_unevencutoffs.loc1", loc1, SRCBUFFER_SIZE, localcapsule[1][p1].key, localcapsule[1][p1].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues_unevencutoffs.localcapsule[2][p2].value", localcapsule[2][p2].value % 4, 4, localcapsule[2][p2].value, localcapsule[2][p2].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues_unevencutoffs.loc2", loc2, SRCBUFFER_SIZE, localcapsule[2][p2].key, localcapsule[2][p2].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues_unevencutoffs.localcapsule[3][p3].value", localcapsule[3][p3].value % 4, 4, localcapsule[3][p3].value, localcapsule[3][p3].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues_unevencutoffs.loc3", loc3, SRCBUFFER_SIZE, localcapsule[3][p3].key, localcapsule[3][p3].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues_unevencutoffs.localcapsule[4][p4].value", localcapsule[4][p4].value % 4, 4, localcapsule[4][p4].value, localcapsule[4][p4].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues_unevencutoffs.loc4", loc4, SRCBUFFER_SIZE, localcapsule[4][p4].key, localcapsule[4][p4].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues_unevencutoffs.localcapsule[5][p5].value", localcapsule[5][p5].value % 4, 4, localcapsule[5][p5].value, localcapsule[5][p5].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues_unevencutoffs.loc5", loc5, SRCBUFFER_SIZE, localcapsule[5][p5].key, localcapsule[5][p5].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues_unevencutoffs.localcapsule[6][p6].value", localcapsule[6][p6].value % 4, 4, localcapsule[6][p6].value, localcapsule[6][p6].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues_unevencutoffs.loc6", loc6, SRCBUFFER_SIZE, localcapsule[6][p6].key, localcapsule[6][p6].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues_unevencutoffs.localcapsule[7][p7].value", localcapsule[7][p7].value % 4, 4, localcapsule[7][p7].value, localcapsule[7][p7].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues_unevencutoffs.loc7", loc7, SRCBUFFER_SIZE, localcapsule[7][p7].key, localcapsule[7][p7].value, NAp);
		#endif
		
		if(valid0 == ON){
			destbuffer[0][loc0] = keyvalue0;
			localcapsule[0][p0].value += 1;
		}
		if(valid1 == ON){
			destbuffer[1][loc1] = keyvalue1;
			localcapsule[1][p1].value += 1;
		}
		if(valid2 == ON){
			destbuffer[2][loc2] = keyvalue2;
			localcapsule[2][p2].value += 1;
		}
		if(valid3 == ON){
			destbuffer[3][loc3] = keyvalue3;
			localcapsule[3][p3].value += 1;
		}
		if(valid4 == ON){
			destbuffer[4][loc4] = keyvalue4;
			localcapsule[4][p4].value += 1;
		}
		if(valid5 == ON){
			destbuffer[5][loc5] = keyvalue5;
			localcapsule[5][p5].value += 1;
		}
		if(valid6 == ON){
			destbuffer[6][loc6] = keyvalue6;
			localcapsule[6][p6].value += 1;
		}
		if(valid7 == ON){
			destbuffer[7][loc7] = keyvalue7;
			localcapsule[7][p7].value += 1;
		}
	}
	
	for(partition_type p=0; p<NUM_PARTITIONS; p++){
		keyvalue_t dummykv;
		// dummykv.key = (p << (globalparams.batch_range_pow - (NUM_PARTITIONS_POW * currentLOP))) + upperlimit;
		dummykv.key = p;
		dummykv.value = INVALIDDATA;
		for(vector_type k=0; k<4; k++){
		#pragma HLS PIPELINE II=2
			if(localcapsule[0][p].value > 0 && (localcapsule[0][p].value % 4) > 0){
				if(k >= localcapsule[0][p].value % 4){
					destbuffer[0][localcapsule[0][p].key + k] = dummykv;
					localcapsule[0][p].value += 1;
				}
			}
			if(localcapsule[1][p].value > 0 && (localcapsule[1][p].value % 4) > 0){
				if(k >= localcapsule[1][p].value % 4){
					destbuffer[1][localcapsule[1][p].key + k] = dummykv;
					localcapsule[1][p].value += 1;
				}
			}
			if(localcapsule[2][p].value > 0 && (localcapsule[2][p].value % 4) > 0){
				if(k >= localcapsule[2][p].value % 4){
					destbuffer[2][localcapsule[2][p].key + k] = dummykv;
					localcapsule[2][p].value += 1;
				}
			}
			if(localcapsule[3][p].value > 0 && (localcapsule[3][p].value % 4) > 0){
				if(k >= localcapsule[3][p].value % 4){
					destbuffer[3][localcapsule[3][p].key + k] = dummykv;
					localcapsule[3][p].value += 1;
				}
			}
			if(localcapsule[4][p].value > 0 && (localcapsule[4][p].value % 4) > 0){
				if(k >= localcapsule[4][p].value % 4){
					destbuffer[4][localcapsule[4][p].key + k] = dummykv;
					localcapsule[4][p].value += 1;
				}
			}
			if(localcapsule[5][p].value > 0 && (localcapsule[5][p].value % 4) > 0){
				if(k >= localcapsule[5][p].value % 4){
					destbuffer[5][localcapsule[5][p].key + k] = dummykv;
					localcapsule[5][p].value += 1;
				}
			}
			if(localcapsule[6][p].value > 0 && (localcapsule[6][p].value % 4) > 0){
				if(k >= localcapsule[6][p].value % 4){
					destbuffer[6][localcapsule[6][p].key + k] = dummykv;
					localcapsule[6][p].value += 1;
				}
			}
			if(localcapsule[7][p].value > 0 && (localcapsule[7][p].value % 4) > 0){
				if(k >= localcapsule[7][p].value % 4){
					destbuffer[7][localcapsule[7][p].key + k] = dummykv;
					localcapsule[7][p].value += 1;
				}
			}
		}
	}
	
	for(vector_type v=0; v<VECTOR_SIZE; v++){ cutoffs[v] = emptyslot[v]; }
	
	localcapsule[0][0].value += (SRCBUFFER_SIZE - emptyslot[0]);
	localcapsule[1][0].value += (SRCBUFFER_SIZE - emptyslot[1]);
	localcapsule[2][0].value += (SRCBUFFER_SIZE - emptyslot[2]);
	localcapsule[3][0].value += (SRCBUFFER_SIZE - emptyslot[3]);
	localcapsule[4][0].value += (SRCBUFFER_SIZE - emptyslot[4]);
	localcapsule[5][0].value += (SRCBUFFER_SIZE - emptyslot[5]);
	localcapsule[6][0].value += (SRCBUFFER_SIZE - emptyslot[6]);
	localcapsule[7][0].value += (SRCBUFFER_SIZE - emptyslot[7]);
	
	localcapsule[0][0].key = 0;
	localcapsule[1][0].key = 0;
	localcapsule[2][0].key = 0;
	localcapsule[3][0].key = 0;
	localcapsule[4][0].key = 0;
	localcapsule[5][0].key = 0;
	localcapsule[6][0].key = 0;
	localcapsule[7][0].key = 0;
	
	calculatemanyunallignedoffsets(localcapsule, NUM_PARTITIONS, 0, 0);

	#ifdef _DEBUGMODE_CHECKS2
	for(partition_type p=0; p<NUM_PARTITIONS; p++){
		for(vector_type v=0; v<VECTOR_SIZE; v++){
			actsutilityobj->checkfordivisibleby(ON, "preparekeyvalues_unevencutoffs.localcapsule[v][p].key", localcapsule[v][p].key, 4);
			// actsutilityobj->checkfordivisibleby(ON, "preparekeyvalues_unevencutoffs.localcapsule["+std::to_string(v)+"][p].key", localcapsule[v][p].key, 4);
		}
	}
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printkeyvalues("preparekeyvalues_unevencutoffs.localcapsule[0].2", (keyvalue_t *)localcapsule[0], NUM_PARTITIONS);
	#endif
	return SRCBUFFER_SIZE;
}

buffer_type 
	#ifdef SW 
	acts::
	#endif
preparekeyvalues_finegrainedpipeline(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t localcapsule[VECTOR_SIZE][NUM_PARTITIONS], step_type currentLOP, sweepparams_t sweepparams, travstate_t travstate, buffer_type size_kvs, buffer_type cutoffs[VECTOR_SIZE], globalparams_t globalparams){
	#pragma HLS INLINE
	#ifdef SIMPLEANDFASTPREPAREFUNC
		return preparekeyvalues_unevencutoffs(enable, sourcebuffer, destbuffer, localcapsule, currentLOP, sweepparams, travstate, size_kvs, cutoffs, globalparams);
	#else 
		#ifdef MAXPERFORMANCE
		return preparekeyvalues2_evencutoffs(enable, sourcebuffer, destbuffer, localcapsule, currentLOP, sweepparams, travstate, size_kvs, globalparams);
		#else
		return preparekeyvalues1_evencutoffs(enable, sourcebuffer, destbuffer, localcapsule, currentLOP, sweepparams, travstate, size_kvs, globalparams);
		#endif
	#endif 
}

void 
	#ifdef SW 
	acts::
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
	for(unsigned int i=0; i<NUM_PARTITIONS-1; i++){ 
		if(globalcapsule[i].key + globalcapsule[i].value >= globalcapsule[i+1].key){ cout<<"savekeyvalues::globalcapsule 33. ERROR. out of bounds. printing and exiting..."<<endl; actsutilityobj->printkeyvalues("savekeyvalues::globalcapsule 33", globalcapsule, NUM_PARTITIONS); exit(EXIT_FAILURE); }
		actsutilityobj->checkoutofbounds("savekeyvalues::globalcapsule 33", globalcapsule[i].key + globalcapsule[i].value, globalcapsule[i+1].key, i, NAp, NAp); 
	}
	actsutilityobj->checkoutofbounds("savekeyvalues::globalcapsule 34", globalcapsule[NUM_PARTITIONS-1].key + globalcapsule[NUM_PARTITIONS-1].value, KVDRAMSZ, NAp, NAp, NAp);
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"savekeyvalues:: keyvalues saved: offset_kvs from: "<<globalbaseaddress_kvs + ((globalcapsule[0].key + globalcapsule[0].value) / VECTOR_SIZE)<<endl;
	actsutilityobj->printkeyvalues("actsutility::savekeyvalues: globalcapsule.", globalcapsule, NUM_PARTITIONS);
	#endif
	return;
}

// reduce functions
void 
	#ifdef SW 
	acts::
	#endif 
readkeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], batch_type offset_kvs, buffer_type size_kvs){
	#pragma HLS function_instantiate variable=kvdram
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
	cout<< TIMINGRESULTSCOLOR<<"readkeyvalues:: vertices read: offset: "<<offset_kvs * VECTOR_SIZE<<"-"<<(offset_kvs + size_kvs) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl;
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<< TIMINGRESULTSCOLOR<<"readkeyvalues:: vertices read: offset: "<<(offset_kvs - BASEOFFSET_VERTEXPTR_KVS) * VECTOR_SIZE<<"-"<<(offset_kvs + size_kvs - BASEOFFSET_VERTEXPTR_KVS) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl;
	cout<< TIMINGRESULTSCOLOR<<"readkeyvalues:: vertices read: offset: "<<(offset_kvs - BASEOFFSET_VERTICESDATA_KVS) * VECTOR_SIZE<<"-"<<(offset_kvs + size_kvs - BASEOFFSET_VERTICESDATA_KVS) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl;
	#endif
	return;
}

void 
	#ifdef SW 
	acts::
	#endif 
savekeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], batch_type offset_kvs, buffer_type size_kvs){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount =  (APPLYVERTEXBUFFERSZ / VDATAPACKINGFACTOR) / 2;
	
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
	cout<<"savekeyvalues:: vertices saved: offset: "<<offset_kvs * VECTOR_SIZE<<"-"<<(offset_kvs + size_kvs) * VECTOR_SIZE<<", number of vertex datas written: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<<endl;
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<< TIMINGRESULTSCOLOR<<"savekeyvalues:: vertices saved: offset: "<<offset_kvs-BASEOFFSET_VERTEXPTR_KVS * VECTOR_SIZE<<"-"<<((offset_kvs + size_kvs)-BASEOFFSET_VERTEXPTR_KVS) * VECTOR_SIZE<<", number of vertex datas written: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl;
	cout<< TIMINGRESULTSCOLOR<<"savekeyvalues:: vertices saved: offset: "<<(offset_kvs-BASEOFFSET_VERTICESDATA_KVS) * VECTOR_SIZE<<"-"<<((offset_kvs-BASEOFFSET_VERTICESDATA_KVS) + size_kvs) * VECTOR_SIZE<<", number of vertex datas written: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl;
	#endif
	return;
}

void 
	#ifdef SW 
	acts::
	#endif 
savekeyvalues_sync(bool_type enable, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11, keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], batch_type offset_kvs, buffer_type size_kvs){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount =  (APPLYVERTEXBUFFERSZ / VDATAPACKINGFACTOR) / 2;
	
	SAVEKEYVALUESSYNC_LOOP: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		keyy_t K0 = buffer[0][i].key; 
		value_t V0 = buffer[0][i].value; 
		keyy_t K1 = buffer[1][i].key; 
		value_t V1 = buffer[1][i].value; 
		keyy_t K2 = buffer[2][i].key; 
		value_t V2 = buffer[2][i].value; 
		keyy_t K3 = buffer[3][i].key; 
		value_t V3 = buffer[3][i].value; 
		keyy_t K4 = buffer[4][i].key; 
		value_t V4 = buffer[4][i].value; 
		keyy_t K5 = buffer[5][i].key; 
		value_t V5 = buffer[5][i].value; 
		keyy_t K6 = buffer[6][i].key; 
		value_t V6 = buffer[6][i].value; 
		keyy_t K7 = buffer[7][i].key; 
		value_t V7 = buffer[7][i].value; 
		
		kvdram0[offset_kvs + i].range(31, 0) = K0; 
		kvdram0[offset_kvs + i].range(63, 32) = V0; 
		kvdram0[offset_kvs + i].range(95, 64) = K1; 
		kvdram0[offset_kvs + i].range(127, 96) = V1; 
		kvdram0[offset_kvs + i].range(159, 128) = K2; 
		kvdram0[offset_kvs + i].range(191, 160) = V2; 
		kvdram0[offset_kvs + i].range(223, 192) = K3; 
		kvdram0[offset_kvs + i].range(255, 224) = V3; 
		kvdram0[offset_kvs + i].range(287, 256) = K4; 
		kvdram0[offset_kvs + i].range(319, 288) = V4; 
		kvdram0[offset_kvs + i].range(351, 320) = K5; 
		kvdram0[offset_kvs + i].range(383, 352) = V5; 
		kvdram0[offset_kvs + i].range(415, 384) = K6; 
		kvdram0[offset_kvs + i].range(447, 416) = V6; 
		kvdram0[offset_kvs + i].range(479, 448) = K7; 
		kvdram0[offset_kvs + i].range(511, 480) = V7; 
		kvdram1[offset_kvs + i].range(31, 0) = K0; 
		kvdram1[offset_kvs + i].range(63, 32) = V0; 
		kvdram1[offset_kvs + i].range(95, 64) = K1; 
		kvdram1[offset_kvs + i].range(127, 96) = V1; 
		kvdram1[offset_kvs + i].range(159, 128) = K2; 
		kvdram1[offset_kvs + i].range(191, 160) = V2; 
		kvdram1[offset_kvs + i].range(223, 192) = K3; 
		kvdram1[offset_kvs + i].range(255, 224) = V3; 
		kvdram1[offset_kvs + i].range(287, 256) = K4; 
		kvdram1[offset_kvs + i].range(319, 288) = V4; 
		kvdram1[offset_kvs + i].range(351, 320) = K5; 
		kvdram1[offset_kvs + i].range(383, 352) = V5; 
		kvdram1[offset_kvs + i].range(415, 384) = K6; 
		kvdram1[offset_kvs + i].range(447, 416) = V6; 
		kvdram1[offset_kvs + i].range(479, 448) = K7; 
		kvdram1[offset_kvs + i].range(511, 480) = V7; 
		kvdram2[offset_kvs + i].range(31, 0) = K0; 
		kvdram2[offset_kvs + i].range(63, 32) = V0; 
		kvdram2[offset_kvs + i].range(95, 64) = K1; 
		kvdram2[offset_kvs + i].range(127, 96) = V1; 
		kvdram2[offset_kvs + i].range(159, 128) = K2; 
		kvdram2[offset_kvs + i].range(191, 160) = V2; 
		kvdram2[offset_kvs + i].range(223, 192) = K3; 
		kvdram2[offset_kvs + i].range(255, 224) = V3; 
		kvdram2[offset_kvs + i].range(287, 256) = K4; 
		kvdram2[offset_kvs + i].range(319, 288) = V4; 
		kvdram2[offset_kvs + i].range(351, 320) = K5; 
		kvdram2[offset_kvs + i].range(383, 352) = V5; 
		kvdram2[offset_kvs + i].range(415, 384) = K6; 
		kvdram2[offset_kvs + i].range(447, 416) = V6; 
		kvdram2[offset_kvs + i].range(479, 448) = K7; 
		kvdram2[offset_kvs + i].range(511, 480) = V7; 
		kvdram3[offset_kvs + i].range(31, 0) = K0; 
		kvdram3[offset_kvs + i].range(63, 32) = V0; 
		kvdram3[offset_kvs + i].range(95, 64) = K1; 
		kvdram3[offset_kvs + i].range(127, 96) = V1; 
		kvdram3[offset_kvs + i].range(159, 128) = K2; 
		kvdram3[offset_kvs + i].range(191, 160) = V2; 
		kvdram3[offset_kvs + i].range(223, 192) = K3; 
		kvdram3[offset_kvs + i].range(255, 224) = V3; 
		kvdram3[offset_kvs + i].range(287, 256) = K4; 
		kvdram3[offset_kvs + i].range(319, 288) = V4; 
		kvdram3[offset_kvs + i].range(351, 320) = K5; 
		kvdram3[offset_kvs + i].range(383, 352) = V5; 
		kvdram3[offset_kvs + i].range(415, 384) = K6; 
		kvdram3[offset_kvs + i].range(447, 416) = V6; 
		kvdram3[offset_kvs + i].range(479, 448) = K7; 
		kvdram3[offset_kvs + i].range(511, 480) = V7; 
		kvdram4[offset_kvs + i].range(31, 0) = K0; 
		kvdram4[offset_kvs + i].range(63, 32) = V0; 
		kvdram4[offset_kvs + i].range(95, 64) = K1; 
		kvdram4[offset_kvs + i].range(127, 96) = V1; 
		kvdram4[offset_kvs + i].range(159, 128) = K2; 
		kvdram4[offset_kvs + i].range(191, 160) = V2; 
		kvdram4[offset_kvs + i].range(223, 192) = K3; 
		kvdram4[offset_kvs + i].range(255, 224) = V3; 
		kvdram4[offset_kvs + i].range(287, 256) = K4; 
		kvdram4[offset_kvs + i].range(319, 288) = V4; 
		kvdram4[offset_kvs + i].range(351, 320) = K5; 
		kvdram4[offset_kvs + i].range(383, 352) = V5; 
		kvdram4[offset_kvs + i].range(415, 384) = K6; 
		kvdram4[offset_kvs + i].range(447, 416) = V6; 
		kvdram4[offset_kvs + i].range(479, 448) = K7; 
		kvdram4[offset_kvs + i].range(511, 480) = V7; 
		kvdram5[offset_kvs + i].range(31, 0) = K0; 
		kvdram5[offset_kvs + i].range(63, 32) = V0; 
		kvdram5[offset_kvs + i].range(95, 64) = K1; 
		kvdram5[offset_kvs + i].range(127, 96) = V1; 
		kvdram5[offset_kvs + i].range(159, 128) = K2; 
		kvdram5[offset_kvs + i].range(191, 160) = V2; 
		kvdram5[offset_kvs + i].range(223, 192) = K3; 
		kvdram5[offset_kvs + i].range(255, 224) = V3; 
		kvdram5[offset_kvs + i].range(287, 256) = K4; 
		kvdram5[offset_kvs + i].range(319, 288) = V4; 
		kvdram5[offset_kvs + i].range(351, 320) = K5; 
		kvdram5[offset_kvs + i].range(383, 352) = V5; 
		kvdram5[offset_kvs + i].range(415, 384) = K6; 
		kvdram5[offset_kvs + i].range(447, 416) = V6; 
		kvdram5[offset_kvs + i].range(479, 448) = K7; 
		kvdram5[offset_kvs + i].range(511, 480) = V7; 
		kvdram6[offset_kvs + i].range(31, 0) = K0; 
		kvdram6[offset_kvs + i].range(63, 32) = V0; 
		kvdram6[offset_kvs + i].range(95, 64) = K1; 
		kvdram6[offset_kvs + i].range(127, 96) = V1; 
		kvdram6[offset_kvs + i].range(159, 128) = K2; 
		kvdram6[offset_kvs + i].range(191, 160) = V2; 
		kvdram6[offset_kvs + i].range(223, 192) = K3; 
		kvdram6[offset_kvs + i].range(255, 224) = V3; 
		kvdram6[offset_kvs + i].range(287, 256) = K4; 
		kvdram6[offset_kvs + i].range(319, 288) = V4; 
		kvdram6[offset_kvs + i].range(351, 320) = K5; 
		kvdram6[offset_kvs + i].range(383, 352) = V5; 
		kvdram6[offset_kvs + i].range(415, 384) = K6; 
		kvdram6[offset_kvs + i].range(447, 416) = V6; 
		kvdram6[offset_kvs + i].range(479, 448) = K7; 
		kvdram6[offset_kvs + i].range(511, 480) = V7; 
		kvdram7[offset_kvs + i].range(31, 0) = K0; 
		kvdram7[offset_kvs + i].range(63, 32) = V0; 
		kvdram7[offset_kvs + i].range(95, 64) = K1; 
		kvdram7[offset_kvs + i].range(127, 96) = V1; 
		kvdram7[offset_kvs + i].range(159, 128) = K2; 
		kvdram7[offset_kvs + i].range(191, 160) = V2; 
		kvdram7[offset_kvs + i].range(223, 192) = K3; 
		kvdram7[offset_kvs + i].range(255, 224) = V3; 
		kvdram7[offset_kvs + i].range(287, 256) = K4; 
		kvdram7[offset_kvs + i].range(319, 288) = V4; 
		kvdram7[offset_kvs + i].range(351, 320) = K5; 
		kvdram7[offset_kvs + i].range(383, 352) = V5; 
		kvdram7[offset_kvs + i].range(415, 384) = K6; 
		kvdram7[offset_kvs + i].range(447, 416) = V6; 
		kvdram7[offset_kvs + i].range(479, 448) = K7; 
		kvdram7[offset_kvs + i].range(511, 480) = V7; 
		kvdram8[offset_kvs + i].range(31, 0) = K0; 
		kvdram8[offset_kvs + i].range(63, 32) = V0; 
		kvdram8[offset_kvs + i].range(95, 64) = K1; 
		kvdram8[offset_kvs + i].range(127, 96) = V1; 
		kvdram8[offset_kvs + i].range(159, 128) = K2; 
		kvdram8[offset_kvs + i].range(191, 160) = V2; 
		kvdram8[offset_kvs + i].range(223, 192) = K3; 
		kvdram8[offset_kvs + i].range(255, 224) = V3; 
		kvdram8[offset_kvs + i].range(287, 256) = K4; 
		kvdram8[offset_kvs + i].range(319, 288) = V4; 
		kvdram8[offset_kvs + i].range(351, 320) = K5; 
		kvdram8[offset_kvs + i].range(383, 352) = V5; 
		kvdram8[offset_kvs + i].range(415, 384) = K6; 
		kvdram8[offset_kvs + i].range(447, 416) = V6; 
		kvdram8[offset_kvs + i].range(479, 448) = K7; 
		kvdram8[offset_kvs + i].range(511, 480) = V7; 
		kvdram9[offset_kvs + i].range(31, 0) = K0; 
		kvdram9[offset_kvs + i].range(63, 32) = V0; 
		kvdram9[offset_kvs + i].range(95, 64) = K1; 
		kvdram9[offset_kvs + i].range(127, 96) = V1; 
		kvdram9[offset_kvs + i].range(159, 128) = K2; 
		kvdram9[offset_kvs + i].range(191, 160) = V2; 
		kvdram9[offset_kvs + i].range(223, 192) = K3; 
		kvdram9[offset_kvs + i].range(255, 224) = V3; 
		kvdram9[offset_kvs + i].range(287, 256) = K4; 
		kvdram9[offset_kvs + i].range(319, 288) = V4; 
		kvdram9[offset_kvs + i].range(351, 320) = K5; 
		kvdram9[offset_kvs + i].range(383, 352) = V5; 
		kvdram9[offset_kvs + i].range(415, 384) = K6; 
		kvdram9[offset_kvs + i].range(447, 416) = V6; 
		kvdram9[offset_kvs + i].range(479, 448) = K7; 
		kvdram9[offset_kvs + i].range(511, 480) = V7; 
		kvdram10[offset_kvs + i].range(31, 0) = K0; 
		kvdram10[offset_kvs + i].range(63, 32) = V0; 
		kvdram10[offset_kvs + i].range(95, 64) = K1; 
		kvdram10[offset_kvs + i].range(127, 96) = V1; 
		kvdram10[offset_kvs + i].range(159, 128) = K2; 
		kvdram10[offset_kvs + i].range(191, 160) = V2; 
		kvdram10[offset_kvs + i].range(223, 192) = K3; 
		kvdram10[offset_kvs + i].range(255, 224) = V3; 
		kvdram10[offset_kvs + i].range(287, 256) = K4; 
		kvdram10[offset_kvs + i].range(319, 288) = V4; 
		kvdram10[offset_kvs + i].range(351, 320) = K5; 
		kvdram10[offset_kvs + i].range(383, 352) = V5; 
		kvdram10[offset_kvs + i].range(415, 384) = K6; 
		kvdram10[offset_kvs + i].range(447, 416) = V6; 
		kvdram10[offset_kvs + i].range(479, 448) = K7; 
		kvdram10[offset_kvs + i].range(511, 480) = V7; 
		kvdram11[offset_kvs + i].range(31, 0) = K0; 
		kvdram11[offset_kvs + i].range(63, 32) = V0; 
		kvdram11[offset_kvs + i].range(95, 64) = K1; 
		kvdram11[offset_kvs + i].range(127, 96) = V1; 
		kvdram11[offset_kvs + i].range(159, 128) = K2; 
		kvdram11[offset_kvs + i].range(191, 160) = V2; 
		kvdram11[offset_kvs + i].range(223, 192) = K3; 
		kvdram11[offset_kvs + i].range(255, 224) = V3; 
		kvdram11[offset_kvs + i].range(287, 256) = K4; 
		kvdram11[offset_kvs + i].range(319, 288) = V4; 
		kvdram11[offset_kvs + i].range(351, 320) = K5; 
		kvdram11[offset_kvs + i].range(383, 352) = V5; 
		kvdram11[offset_kvs + i].range(415, 384) = K6; 
		kvdram11[offset_kvs + i].range(447, 416) = V6; 
		kvdram11[offset_kvs + i].range(479, 448) = K7; 
		kvdram11[offset_kvs + i].range(511, 480) = V7; 
		#else 
		kvdram0[offset_kvs + i].data[0] = buffer[0][i];
		kvdram0[offset_kvs + i].data[1] = buffer[1][i];
		kvdram0[offset_kvs + i].data[2] = buffer[2][i];
		kvdram0[offset_kvs + i].data[3] = buffer[3][i];
		kvdram0[offset_kvs + i].data[4] = buffer[4][i];
		kvdram0[offset_kvs + i].data[5] = buffer[5][i];
		kvdram0[offset_kvs + i].data[6] = buffer[6][i];
		kvdram0[offset_kvs + i].data[7] = buffer[7][i];
		kvdram1[offset_kvs + i].data[0] = buffer[0][i];
		kvdram1[offset_kvs + i].data[1] = buffer[1][i];
		kvdram1[offset_kvs + i].data[2] = buffer[2][i];
		kvdram1[offset_kvs + i].data[3] = buffer[3][i];
		kvdram1[offset_kvs + i].data[4] = buffer[4][i];
		kvdram1[offset_kvs + i].data[5] = buffer[5][i];
		kvdram1[offset_kvs + i].data[6] = buffer[6][i];
		kvdram1[offset_kvs + i].data[7] = buffer[7][i];
		kvdram2[offset_kvs + i].data[0] = buffer[0][i];
		kvdram2[offset_kvs + i].data[1] = buffer[1][i];
		kvdram2[offset_kvs + i].data[2] = buffer[2][i];
		kvdram2[offset_kvs + i].data[3] = buffer[3][i];
		kvdram2[offset_kvs + i].data[4] = buffer[4][i];
		kvdram2[offset_kvs + i].data[5] = buffer[5][i];
		kvdram2[offset_kvs + i].data[6] = buffer[6][i];
		kvdram2[offset_kvs + i].data[7] = buffer[7][i];
		kvdram3[offset_kvs + i].data[0] = buffer[0][i];
		kvdram3[offset_kvs + i].data[1] = buffer[1][i];
		kvdram3[offset_kvs + i].data[2] = buffer[2][i];
		kvdram3[offset_kvs + i].data[3] = buffer[3][i];
		kvdram3[offset_kvs + i].data[4] = buffer[4][i];
		kvdram3[offset_kvs + i].data[5] = buffer[5][i];
		kvdram3[offset_kvs + i].data[6] = buffer[6][i];
		kvdram3[offset_kvs + i].data[7] = buffer[7][i];
		kvdram4[offset_kvs + i].data[0] = buffer[0][i];
		kvdram4[offset_kvs + i].data[1] = buffer[1][i];
		kvdram4[offset_kvs + i].data[2] = buffer[2][i];
		kvdram4[offset_kvs + i].data[3] = buffer[3][i];
		kvdram4[offset_kvs + i].data[4] = buffer[4][i];
		kvdram4[offset_kvs + i].data[5] = buffer[5][i];
		kvdram4[offset_kvs + i].data[6] = buffer[6][i];
		kvdram4[offset_kvs + i].data[7] = buffer[7][i];
		kvdram5[offset_kvs + i].data[0] = buffer[0][i];
		kvdram5[offset_kvs + i].data[1] = buffer[1][i];
		kvdram5[offset_kvs + i].data[2] = buffer[2][i];
		kvdram5[offset_kvs + i].data[3] = buffer[3][i];
		kvdram5[offset_kvs + i].data[4] = buffer[4][i];
		kvdram5[offset_kvs + i].data[5] = buffer[5][i];
		kvdram5[offset_kvs + i].data[6] = buffer[6][i];
		kvdram5[offset_kvs + i].data[7] = buffer[7][i];
		kvdram6[offset_kvs + i].data[0] = buffer[0][i];
		kvdram6[offset_kvs + i].data[1] = buffer[1][i];
		kvdram6[offset_kvs + i].data[2] = buffer[2][i];
		kvdram6[offset_kvs + i].data[3] = buffer[3][i];
		kvdram6[offset_kvs + i].data[4] = buffer[4][i];
		kvdram6[offset_kvs + i].data[5] = buffer[5][i];
		kvdram6[offset_kvs + i].data[6] = buffer[6][i];
		kvdram6[offset_kvs + i].data[7] = buffer[7][i];
		kvdram7[offset_kvs + i].data[0] = buffer[0][i];
		kvdram7[offset_kvs + i].data[1] = buffer[1][i];
		kvdram7[offset_kvs + i].data[2] = buffer[2][i];
		kvdram7[offset_kvs + i].data[3] = buffer[3][i];
		kvdram7[offset_kvs + i].data[4] = buffer[4][i];
		kvdram7[offset_kvs + i].data[5] = buffer[5][i];
		kvdram7[offset_kvs + i].data[6] = buffer[6][i];
		kvdram7[offset_kvs + i].data[7] = buffer[7][i];
		kvdram8[offset_kvs + i].data[0] = buffer[0][i];
		kvdram8[offset_kvs + i].data[1] = buffer[1][i];
		kvdram8[offset_kvs + i].data[2] = buffer[2][i];
		kvdram8[offset_kvs + i].data[3] = buffer[3][i];
		kvdram8[offset_kvs + i].data[4] = buffer[4][i];
		kvdram8[offset_kvs + i].data[5] = buffer[5][i];
		kvdram8[offset_kvs + i].data[6] = buffer[6][i];
		kvdram8[offset_kvs + i].data[7] = buffer[7][i];
		kvdram9[offset_kvs + i].data[0] = buffer[0][i];
		kvdram9[offset_kvs + i].data[1] = buffer[1][i];
		kvdram9[offset_kvs + i].data[2] = buffer[2][i];
		kvdram9[offset_kvs + i].data[3] = buffer[3][i];
		kvdram9[offset_kvs + i].data[4] = buffer[4][i];
		kvdram9[offset_kvs + i].data[5] = buffer[5][i];
		kvdram9[offset_kvs + i].data[6] = buffer[6][i];
		kvdram9[offset_kvs + i].data[7] = buffer[7][i];
		kvdram10[offset_kvs + i].data[0] = buffer[0][i];
		kvdram10[offset_kvs + i].data[1] = buffer[1][i];
		kvdram10[offset_kvs + i].data[2] = buffer[2][i];
		kvdram10[offset_kvs + i].data[3] = buffer[3][i];
		kvdram10[offset_kvs + i].data[4] = buffer[4][i];
		kvdram10[offset_kvs + i].data[5] = buffer[5][i];
		kvdram10[offset_kvs + i].data[6] = buffer[6][i];
		kvdram10[offset_kvs + i].data[7] = buffer[7][i];
		kvdram11[offset_kvs + i].data[0] = buffer[0][i];
		kvdram11[offset_kvs + i].data[1] = buffer[1][i];
		kvdram11[offset_kvs + i].data[2] = buffer[2][i];
		kvdram11[offset_kvs + i].data[3] = buffer[3][i];
		kvdram11[offset_kvs + i].data[4] = buffer[4][i];
		kvdram11[offset_kvs + i].data[5] = buffer[5][i];
		kvdram11[offset_kvs + i].data[6] = buffer[6][i];
		kvdram11[offset_kvs + i].data[7] = buffer[7][i];
		#endif 
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvswritten(VECTOR_SIZE);
		#endif
	}
	#ifdef _DEBUGMODE_KERNELPRINTS2
	cout<< TIMINGRESULTSCOLOR<<"savekeyvalues_sync:: keyvalues saved: offset: "<<offset_kvs * VECTOR_SIZE<<"-"<<(offset_kvs + size_kvs) * VECTOR_SIZE<<", number of vertex datas written: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl;
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<< TIMINGRESULTSCOLOR<<"savekeyvalues_sync:: keyvalues saved: offset: "<<offset_kvs-BASEOFFSET_VERTEXPTR_KVS * VECTOR_SIZE<<"-"<<((offset_kvs + size_kvs)-BASEOFFSET_VERTEXPTR_KVS) * VECTOR_SIZE<<", number of vertex datas written: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl;
	cout<< TIMINGRESULTSCOLOR<<"savekeyvalues_sync:: keyvalues saved: offset: "<<(offset_kvs-BASEOFFSET_VERTICESDATA_KVS) * VECTOR_SIZE<<"-"<<((offset_kvs-BASEOFFSET_VERTICESDATA_KVS) + size_kvs) * VECTOR_SIZE<<", number of vertex datas written: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl;
	#endif
	return;
}

void 
	#ifdef SW 
	acts::
	#endif 
savekeyvalues_pipeline_sync(bool_type enable, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11, keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], batch_type offset_kvs, buffer_type size_kvs){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount =  (APPLYVERTEXBUFFERSZ / VDATAPACKINGFACTOR) / 2;
	
	keyvalue_t buffer0[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer0
	keyvalue_t buffer1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer1
	keyvalue_t buffer2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer2
	keyvalue_t buffer3[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer3
	keyvalue_t buffer4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer4
	keyvalue_t buffer5[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer5
	keyvalue_t buffer6[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer6
	keyvalue_t buffer7[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer7
	keyvalue_t buffer8[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer8
	keyvalue_t buffer9[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer9
	keyvalue_t buffer10[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer10
	keyvalue_t buffer11[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer11
	
	for(unsigned int i=0; i<size_kvs; i++){
	#pragma HLS PIPELINE II=1
		keyvalue_t keyvalue0 = buffer[0][i];
		keyvalue_t keyvalue1 = buffer[1][i];
		keyvalue_t keyvalue2 = buffer[2][i];
		keyvalue_t keyvalue3 = buffer[3][i];
		keyvalue_t keyvalue4 = buffer[4][i];
		keyvalue_t keyvalue5 = buffer[5][i];
		keyvalue_t keyvalue6 = buffer[6][i];
		keyvalue_t keyvalue7 = buffer[7][i];
	
		buffer0[0][i] = keyvalue0;
		buffer0[1][i] = keyvalue1;
		buffer0[2][i] = keyvalue2;
		buffer0[3][i] = keyvalue3;
		buffer0[4][i] = keyvalue4;
		buffer0[5][i] = keyvalue5;
		buffer0[6][i] = keyvalue6;
		buffer0[7][i] = keyvalue7;
		buffer1[0][i] = keyvalue0;
		buffer1[1][i] = keyvalue1;
		buffer1[2][i] = keyvalue2;
		buffer1[3][i] = keyvalue3;
		buffer1[4][i] = keyvalue4;
		buffer1[5][i] = keyvalue5;
		buffer1[6][i] = keyvalue6;
		buffer1[7][i] = keyvalue7;
		buffer2[0][i] = keyvalue0;
		buffer2[1][i] = keyvalue1;
		buffer2[2][i] = keyvalue2;
		buffer2[3][i] = keyvalue3;
		buffer2[4][i] = keyvalue4;
		buffer2[5][i] = keyvalue5;
		buffer2[6][i] = keyvalue6;
		buffer2[7][i] = keyvalue7;
		buffer3[0][i] = keyvalue0;
		buffer3[1][i] = keyvalue1;
		buffer3[2][i] = keyvalue2;
		buffer3[3][i] = keyvalue3;
		buffer3[4][i] = keyvalue4;
		buffer3[5][i] = keyvalue5;
		buffer3[6][i] = keyvalue6;
		buffer3[7][i] = keyvalue7;
		buffer4[0][i] = keyvalue0;
		buffer4[1][i] = keyvalue1;
		buffer4[2][i] = keyvalue2;
		buffer4[3][i] = keyvalue3;
		buffer4[4][i] = keyvalue4;
		buffer4[5][i] = keyvalue5;
		buffer4[6][i] = keyvalue6;
		buffer4[7][i] = keyvalue7;
		buffer5[0][i] = keyvalue0;
		buffer5[1][i] = keyvalue1;
		buffer5[2][i] = keyvalue2;
		buffer5[3][i] = keyvalue3;
		buffer5[4][i] = keyvalue4;
		buffer5[5][i] = keyvalue5;
		buffer5[6][i] = keyvalue6;
		buffer5[7][i] = keyvalue7;
		buffer6[0][i] = keyvalue0;
		buffer6[1][i] = keyvalue1;
		buffer6[2][i] = keyvalue2;
		buffer6[3][i] = keyvalue3;
		buffer6[4][i] = keyvalue4;
		buffer6[5][i] = keyvalue5;
		buffer6[6][i] = keyvalue6;
		buffer6[7][i] = keyvalue7;
		buffer7[0][i] = keyvalue0;
		buffer7[1][i] = keyvalue1;
		buffer7[2][i] = keyvalue2;
		buffer7[3][i] = keyvalue3;
		buffer7[4][i] = keyvalue4;
		buffer7[5][i] = keyvalue5;
		buffer7[6][i] = keyvalue6;
		buffer7[7][i] = keyvalue7;
		buffer8[0][i] = keyvalue0;
		buffer8[1][i] = keyvalue1;
		buffer8[2][i] = keyvalue2;
		buffer8[3][i] = keyvalue3;
		buffer8[4][i] = keyvalue4;
		buffer8[5][i] = keyvalue5;
		buffer8[6][i] = keyvalue6;
		buffer8[7][i] = keyvalue7;
		buffer9[0][i] = keyvalue0;
		buffer9[1][i] = keyvalue1;
		buffer9[2][i] = keyvalue2;
		buffer9[3][i] = keyvalue3;
		buffer9[4][i] = keyvalue4;
		buffer9[5][i] = keyvalue5;
		buffer9[6][i] = keyvalue6;
		buffer9[7][i] = keyvalue7;
		buffer10[0][i] = keyvalue0;
		buffer10[1][i] = keyvalue1;
		buffer10[2][i] = keyvalue2;
		buffer10[3][i] = keyvalue3;
		buffer10[4][i] = keyvalue4;
		buffer10[5][i] = keyvalue5;
		buffer10[6][i] = keyvalue6;
		buffer10[7][i] = keyvalue7;
		buffer11[0][i] = keyvalue0;
		buffer11[1][i] = keyvalue1;
		buffer11[2][i] = keyvalue2;
		buffer11[3][i] = keyvalue3;
		buffer11[4][i] = keyvalue4;
		buffer11[5][i] = keyvalue5;
		buffer11[6][i] = keyvalue6;
		buffer11[7][i] = keyvalue7;
	}
	
	SAVEKEYVALUESSYNC_LOOP: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		kvdram0[offset_kvs + i].range(31, 0) = buffer0[0][i].key; 
		kvdram0[offset_kvs + i].range(63, 32) = buffer0[0][i].value; 
		kvdram0[offset_kvs + i].range(95, 64) = buffer0[1][i].key; 
		kvdram0[offset_kvs + i].range(127, 96) = buffer0[1][i].value; 
		kvdram0[offset_kvs + i].range(159, 128) = buffer0[2][i].key; 
		kvdram0[offset_kvs + i].range(191, 160) = buffer0[2][i].value; 
		kvdram0[offset_kvs + i].range(223, 192) = buffer0[3][i].key; 
		kvdram0[offset_kvs + i].range(255, 224) = buffer0[3][i].value; 
		kvdram0[offset_kvs + i].range(287, 256) = buffer0[4][i].key; 
		kvdram0[offset_kvs + i].range(319, 288) = buffer0[4][i].value; 
		kvdram0[offset_kvs + i].range(351, 320) = buffer0[5][i].key; 
		kvdram0[offset_kvs + i].range(383, 352) = buffer0[5][i].value; 
		kvdram0[offset_kvs + i].range(415, 384) = buffer0[6][i].key; 
		kvdram0[offset_kvs + i].range(447, 416) = buffer0[6][i].value; 
		kvdram0[offset_kvs + i].range(479, 448) = buffer0[7][i].key; 
		kvdram0[offset_kvs + i].range(511, 480) = buffer0[7][i].value; 
		kvdram1[offset_kvs + i].range(31, 0) = buffer1[0][i].key; 
		kvdram1[offset_kvs + i].range(63, 32) = buffer1[0][i].value; 
		kvdram1[offset_kvs + i].range(95, 64) = buffer1[1][i].key; 
		kvdram1[offset_kvs + i].range(127, 96) = buffer1[1][i].value; 
		kvdram1[offset_kvs + i].range(159, 128) = buffer1[2][i].key; 
		kvdram1[offset_kvs + i].range(191, 160) = buffer1[2][i].value; 
		kvdram1[offset_kvs + i].range(223, 192) = buffer1[3][i].key; 
		kvdram1[offset_kvs + i].range(255, 224) = buffer1[3][i].value; 
		kvdram1[offset_kvs + i].range(287, 256) = buffer1[4][i].key; 
		kvdram1[offset_kvs + i].range(319, 288) = buffer1[4][i].value; 
		kvdram1[offset_kvs + i].range(351, 320) = buffer1[5][i].key; 
		kvdram1[offset_kvs + i].range(383, 352) = buffer1[5][i].value; 
		kvdram1[offset_kvs + i].range(415, 384) = buffer1[6][i].key; 
		kvdram1[offset_kvs + i].range(447, 416) = buffer1[6][i].value; 
		kvdram1[offset_kvs + i].range(479, 448) = buffer1[7][i].key; 
		kvdram1[offset_kvs + i].range(511, 480) = buffer1[7][i].value; 
		kvdram2[offset_kvs + i].range(31, 0) = buffer2[0][i].key; 
		kvdram2[offset_kvs + i].range(63, 32) = buffer2[0][i].value; 
		kvdram2[offset_kvs + i].range(95, 64) = buffer2[1][i].key; 
		kvdram2[offset_kvs + i].range(127, 96) = buffer2[1][i].value; 
		kvdram2[offset_kvs + i].range(159, 128) = buffer2[2][i].key; 
		kvdram2[offset_kvs + i].range(191, 160) = buffer2[2][i].value; 
		kvdram2[offset_kvs + i].range(223, 192) = buffer2[3][i].key; 
		kvdram2[offset_kvs + i].range(255, 224) = buffer2[3][i].value; 
		kvdram2[offset_kvs + i].range(287, 256) = buffer2[4][i].key; 
		kvdram2[offset_kvs + i].range(319, 288) = buffer2[4][i].value; 
		kvdram2[offset_kvs + i].range(351, 320) = buffer2[5][i].key; 
		kvdram2[offset_kvs + i].range(383, 352) = buffer2[5][i].value; 
		kvdram2[offset_kvs + i].range(415, 384) = buffer2[6][i].key; 
		kvdram2[offset_kvs + i].range(447, 416) = buffer2[6][i].value; 
		kvdram2[offset_kvs + i].range(479, 448) = buffer2[7][i].key; 
		kvdram2[offset_kvs + i].range(511, 480) = buffer2[7][i].value; 
		kvdram3[offset_kvs + i].range(31, 0) = buffer3[0][i].key; 
		kvdram3[offset_kvs + i].range(63, 32) = buffer3[0][i].value; 
		kvdram3[offset_kvs + i].range(95, 64) = buffer3[1][i].key; 
		kvdram3[offset_kvs + i].range(127, 96) = buffer3[1][i].value; 
		kvdram3[offset_kvs + i].range(159, 128) = buffer3[2][i].key; 
		kvdram3[offset_kvs + i].range(191, 160) = buffer3[2][i].value; 
		kvdram3[offset_kvs + i].range(223, 192) = buffer3[3][i].key; 
		kvdram3[offset_kvs + i].range(255, 224) = buffer3[3][i].value; 
		kvdram3[offset_kvs + i].range(287, 256) = buffer3[4][i].key; 
		kvdram3[offset_kvs + i].range(319, 288) = buffer3[4][i].value; 
		kvdram3[offset_kvs + i].range(351, 320) = buffer3[5][i].key; 
		kvdram3[offset_kvs + i].range(383, 352) = buffer3[5][i].value; 
		kvdram3[offset_kvs + i].range(415, 384) = buffer3[6][i].key; 
		kvdram3[offset_kvs + i].range(447, 416) = buffer3[6][i].value; 
		kvdram3[offset_kvs + i].range(479, 448) = buffer3[7][i].key; 
		kvdram3[offset_kvs + i].range(511, 480) = buffer3[7][i].value; 
		kvdram4[offset_kvs + i].range(31, 0) = buffer4[0][i].key; 
		kvdram4[offset_kvs + i].range(63, 32) = buffer4[0][i].value; 
		kvdram4[offset_kvs + i].range(95, 64) = buffer4[1][i].key; 
		kvdram4[offset_kvs + i].range(127, 96) = buffer4[1][i].value; 
		kvdram4[offset_kvs + i].range(159, 128) = buffer4[2][i].key; 
		kvdram4[offset_kvs + i].range(191, 160) = buffer4[2][i].value; 
		kvdram4[offset_kvs + i].range(223, 192) = buffer4[3][i].key; 
		kvdram4[offset_kvs + i].range(255, 224) = buffer4[3][i].value; 
		kvdram4[offset_kvs + i].range(287, 256) = buffer4[4][i].key; 
		kvdram4[offset_kvs + i].range(319, 288) = buffer4[4][i].value; 
		kvdram4[offset_kvs + i].range(351, 320) = buffer4[5][i].key; 
		kvdram4[offset_kvs + i].range(383, 352) = buffer4[5][i].value; 
		kvdram4[offset_kvs + i].range(415, 384) = buffer4[6][i].key; 
		kvdram4[offset_kvs + i].range(447, 416) = buffer4[6][i].value; 
		kvdram4[offset_kvs + i].range(479, 448) = buffer4[7][i].key; 
		kvdram4[offset_kvs + i].range(511, 480) = buffer4[7][i].value; 
		kvdram5[offset_kvs + i].range(31, 0) = buffer5[0][i].key; 
		kvdram5[offset_kvs + i].range(63, 32) = buffer5[0][i].value; 
		kvdram5[offset_kvs + i].range(95, 64) = buffer5[1][i].key; 
		kvdram5[offset_kvs + i].range(127, 96) = buffer5[1][i].value; 
		kvdram5[offset_kvs + i].range(159, 128) = buffer5[2][i].key; 
		kvdram5[offset_kvs + i].range(191, 160) = buffer5[2][i].value; 
		kvdram5[offset_kvs + i].range(223, 192) = buffer5[3][i].key; 
		kvdram5[offset_kvs + i].range(255, 224) = buffer5[3][i].value; 
		kvdram5[offset_kvs + i].range(287, 256) = buffer5[4][i].key; 
		kvdram5[offset_kvs + i].range(319, 288) = buffer5[4][i].value; 
		kvdram5[offset_kvs + i].range(351, 320) = buffer5[5][i].key; 
		kvdram5[offset_kvs + i].range(383, 352) = buffer5[5][i].value; 
		kvdram5[offset_kvs + i].range(415, 384) = buffer5[6][i].key; 
		kvdram5[offset_kvs + i].range(447, 416) = buffer5[6][i].value; 
		kvdram5[offset_kvs + i].range(479, 448) = buffer5[7][i].key; 
		kvdram5[offset_kvs + i].range(511, 480) = buffer5[7][i].value; 
		kvdram6[offset_kvs + i].range(31, 0) = buffer6[0][i].key; 
		kvdram6[offset_kvs + i].range(63, 32) = buffer6[0][i].value; 
		kvdram6[offset_kvs + i].range(95, 64) = buffer6[1][i].key; 
		kvdram6[offset_kvs + i].range(127, 96) = buffer6[1][i].value; 
		kvdram6[offset_kvs + i].range(159, 128) = buffer6[2][i].key; 
		kvdram6[offset_kvs + i].range(191, 160) = buffer6[2][i].value; 
		kvdram6[offset_kvs + i].range(223, 192) = buffer6[3][i].key; 
		kvdram6[offset_kvs + i].range(255, 224) = buffer6[3][i].value; 
		kvdram6[offset_kvs + i].range(287, 256) = buffer6[4][i].key; 
		kvdram6[offset_kvs + i].range(319, 288) = buffer6[4][i].value; 
		kvdram6[offset_kvs + i].range(351, 320) = buffer6[5][i].key; 
		kvdram6[offset_kvs + i].range(383, 352) = buffer6[5][i].value; 
		kvdram6[offset_kvs + i].range(415, 384) = buffer6[6][i].key; 
		kvdram6[offset_kvs + i].range(447, 416) = buffer6[6][i].value; 
		kvdram6[offset_kvs + i].range(479, 448) = buffer6[7][i].key; 
		kvdram6[offset_kvs + i].range(511, 480) = buffer6[7][i].value; 
		kvdram7[offset_kvs + i].range(31, 0) = buffer7[0][i].key; 
		kvdram7[offset_kvs + i].range(63, 32) = buffer7[0][i].value; 
		kvdram7[offset_kvs + i].range(95, 64) = buffer7[1][i].key; 
		kvdram7[offset_kvs + i].range(127, 96) = buffer7[1][i].value; 
		kvdram7[offset_kvs + i].range(159, 128) = buffer7[2][i].key; 
		kvdram7[offset_kvs + i].range(191, 160) = buffer7[2][i].value; 
		kvdram7[offset_kvs + i].range(223, 192) = buffer7[3][i].key; 
		kvdram7[offset_kvs + i].range(255, 224) = buffer7[3][i].value; 
		kvdram7[offset_kvs + i].range(287, 256) = buffer7[4][i].key; 
		kvdram7[offset_kvs + i].range(319, 288) = buffer7[4][i].value; 
		kvdram7[offset_kvs + i].range(351, 320) = buffer7[5][i].key; 
		kvdram7[offset_kvs + i].range(383, 352) = buffer7[5][i].value; 
		kvdram7[offset_kvs + i].range(415, 384) = buffer7[6][i].key; 
		kvdram7[offset_kvs + i].range(447, 416) = buffer7[6][i].value; 
		kvdram7[offset_kvs + i].range(479, 448) = buffer7[7][i].key; 
		kvdram7[offset_kvs + i].range(511, 480) = buffer7[7][i].value; 
		kvdram8[offset_kvs + i].range(31, 0) = buffer8[0][i].key; 
		kvdram8[offset_kvs + i].range(63, 32) = buffer8[0][i].value; 
		kvdram8[offset_kvs + i].range(95, 64) = buffer8[1][i].key; 
		kvdram8[offset_kvs + i].range(127, 96) = buffer8[1][i].value; 
		kvdram8[offset_kvs + i].range(159, 128) = buffer8[2][i].key; 
		kvdram8[offset_kvs + i].range(191, 160) = buffer8[2][i].value; 
		kvdram8[offset_kvs + i].range(223, 192) = buffer8[3][i].key; 
		kvdram8[offset_kvs + i].range(255, 224) = buffer8[3][i].value; 
		kvdram8[offset_kvs + i].range(287, 256) = buffer8[4][i].key; 
		kvdram8[offset_kvs + i].range(319, 288) = buffer8[4][i].value; 
		kvdram8[offset_kvs + i].range(351, 320) = buffer8[5][i].key; 
		kvdram8[offset_kvs + i].range(383, 352) = buffer8[5][i].value; 
		kvdram8[offset_kvs + i].range(415, 384) = buffer8[6][i].key; 
		kvdram8[offset_kvs + i].range(447, 416) = buffer8[6][i].value; 
		kvdram8[offset_kvs + i].range(479, 448) = buffer8[7][i].key; 
		kvdram8[offset_kvs + i].range(511, 480) = buffer8[7][i].value; 
		kvdram9[offset_kvs + i].range(31, 0) = buffer9[0][i].key; 
		kvdram9[offset_kvs + i].range(63, 32) = buffer9[0][i].value; 
		kvdram9[offset_kvs + i].range(95, 64) = buffer9[1][i].key; 
		kvdram9[offset_kvs + i].range(127, 96) = buffer9[1][i].value; 
		kvdram9[offset_kvs + i].range(159, 128) = buffer9[2][i].key; 
		kvdram9[offset_kvs + i].range(191, 160) = buffer9[2][i].value; 
		kvdram9[offset_kvs + i].range(223, 192) = buffer9[3][i].key; 
		kvdram9[offset_kvs + i].range(255, 224) = buffer9[3][i].value; 
		kvdram9[offset_kvs + i].range(287, 256) = buffer9[4][i].key; 
		kvdram9[offset_kvs + i].range(319, 288) = buffer9[4][i].value; 
		kvdram9[offset_kvs + i].range(351, 320) = buffer9[5][i].key; 
		kvdram9[offset_kvs + i].range(383, 352) = buffer9[5][i].value; 
		kvdram9[offset_kvs + i].range(415, 384) = buffer9[6][i].key; 
		kvdram9[offset_kvs + i].range(447, 416) = buffer9[6][i].value; 
		kvdram9[offset_kvs + i].range(479, 448) = buffer9[7][i].key; 
		kvdram9[offset_kvs + i].range(511, 480) = buffer9[7][i].value; 
		kvdram10[offset_kvs + i].range(31, 0) = buffer10[0][i].key; 
		kvdram10[offset_kvs + i].range(63, 32) = buffer10[0][i].value; 
		kvdram10[offset_kvs + i].range(95, 64) = buffer10[1][i].key; 
		kvdram10[offset_kvs + i].range(127, 96) = buffer10[1][i].value; 
		kvdram10[offset_kvs + i].range(159, 128) = buffer10[2][i].key; 
		kvdram10[offset_kvs + i].range(191, 160) = buffer10[2][i].value; 
		kvdram10[offset_kvs + i].range(223, 192) = buffer10[3][i].key; 
		kvdram10[offset_kvs + i].range(255, 224) = buffer10[3][i].value; 
		kvdram10[offset_kvs + i].range(287, 256) = buffer10[4][i].key; 
		kvdram10[offset_kvs + i].range(319, 288) = buffer10[4][i].value; 
		kvdram10[offset_kvs + i].range(351, 320) = buffer10[5][i].key; 
		kvdram10[offset_kvs + i].range(383, 352) = buffer10[5][i].value; 
		kvdram10[offset_kvs + i].range(415, 384) = buffer10[6][i].key; 
		kvdram10[offset_kvs + i].range(447, 416) = buffer10[6][i].value; 
		kvdram10[offset_kvs + i].range(479, 448) = buffer10[7][i].key; 
		kvdram10[offset_kvs + i].range(511, 480) = buffer10[7][i].value; 
		kvdram11[offset_kvs + i].range(31, 0) = buffer11[0][i].key; 
		kvdram11[offset_kvs + i].range(63, 32) = buffer11[0][i].value; 
		kvdram11[offset_kvs + i].range(95, 64) = buffer11[1][i].key; 
		kvdram11[offset_kvs + i].range(127, 96) = buffer11[1][i].value; 
		kvdram11[offset_kvs + i].range(159, 128) = buffer11[2][i].key; 
		kvdram11[offset_kvs + i].range(191, 160) = buffer11[2][i].value; 
		kvdram11[offset_kvs + i].range(223, 192) = buffer11[3][i].key; 
		kvdram11[offset_kvs + i].range(255, 224) = buffer11[3][i].value; 
		kvdram11[offset_kvs + i].range(287, 256) = buffer11[4][i].key; 
		kvdram11[offset_kvs + i].range(319, 288) = buffer11[4][i].value; 
		kvdram11[offset_kvs + i].range(351, 320) = buffer11[5][i].key; 
		kvdram11[offset_kvs + i].range(383, 352) = buffer11[5][i].value; 
		kvdram11[offset_kvs + i].range(415, 384) = buffer11[6][i].key; 
		kvdram11[offset_kvs + i].range(447, 416) = buffer11[6][i].value; 
		kvdram11[offset_kvs + i].range(479, 448) = buffer11[7][i].key; 
		kvdram11[offset_kvs + i].range(511, 480) = buffer11[7][i].value; 
		#else 
		kvdram0[offset_kvs + i].data[0] = buffer0[0][i];
		kvdram0[offset_kvs + i].data[1] = buffer0[1][i];
		kvdram0[offset_kvs + i].data[2] = buffer0[2][i];
		kvdram0[offset_kvs + i].data[3] = buffer0[3][i];
		kvdram0[offset_kvs + i].data[4] = buffer0[4][i];
		kvdram0[offset_kvs + i].data[5] = buffer0[5][i];
		kvdram0[offset_kvs + i].data[6] = buffer0[6][i];
		kvdram0[offset_kvs + i].data[7] = buffer0[7][i];
		kvdram1[offset_kvs + i].data[0] = buffer1[0][i];
		kvdram1[offset_kvs + i].data[1] = buffer1[1][i];
		kvdram1[offset_kvs + i].data[2] = buffer1[2][i];
		kvdram1[offset_kvs + i].data[3] = buffer1[3][i];
		kvdram1[offset_kvs + i].data[4] = buffer1[4][i];
		kvdram1[offset_kvs + i].data[5] = buffer1[5][i];
		kvdram1[offset_kvs + i].data[6] = buffer1[6][i];
		kvdram1[offset_kvs + i].data[7] = buffer1[7][i];
		kvdram2[offset_kvs + i].data[0] = buffer2[0][i];
		kvdram2[offset_kvs + i].data[1] = buffer2[1][i];
		kvdram2[offset_kvs + i].data[2] = buffer2[2][i];
		kvdram2[offset_kvs + i].data[3] = buffer2[3][i];
		kvdram2[offset_kvs + i].data[4] = buffer2[4][i];
		kvdram2[offset_kvs + i].data[5] = buffer2[5][i];
		kvdram2[offset_kvs + i].data[6] = buffer2[6][i];
		kvdram2[offset_kvs + i].data[7] = buffer2[7][i];
		kvdram3[offset_kvs + i].data[0] = buffer3[0][i];
		kvdram3[offset_kvs + i].data[1] = buffer3[1][i];
		kvdram3[offset_kvs + i].data[2] = buffer3[2][i];
		kvdram3[offset_kvs + i].data[3] = buffer3[3][i];
		kvdram3[offset_kvs + i].data[4] = buffer3[4][i];
		kvdram3[offset_kvs + i].data[5] = buffer3[5][i];
		kvdram3[offset_kvs + i].data[6] = buffer3[6][i];
		kvdram3[offset_kvs + i].data[7] = buffer3[7][i];
		kvdram4[offset_kvs + i].data[0] = buffer4[0][i];
		kvdram4[offset_kvs + i].data[1] = buffer4[1][i];
		kvdram4[offset_kvs + i].data[2] = buffer4[2][i];
		kvdram4[offset_kvs + i].data[3] = buffer4[3][i];
		kvdram4[offset_kvs + i].data[4] = buffer4[4][i];
		kvdram4[offset_kvs + i].data[5] = buffer4[5][i];
		kvdram4[offset_kvs + i].data[6] = buffer4[6][i];
		kvdram4[offset_kvs + i].data[7] = buffer4[7][i];
		kvdram5[offset_kvs + i].data[0] = buffer5[0][i];
		kvdram5[offset_kvs + i].data[1] = buffer5[1][i];
		kvdram5[offset_kvs + i].data[2] = buffer5[2][i];
		kvdram5[offset_kvs + i].data[3] = buffer5[3][i];
		kvdram5[offset_kvs + i].data[4] = buffer5[4][i];
		kvdram5[offset_kvs + i].data[5] = buffer5[5][i];
		kvdram5[offset_kvs + i].data[6] = buffer5[6][i];
		kvdram5[offset_kvs + i].data[7] = buffer5[7][i];
		kvdram6[offset_kvs + i].data[0] = buffer6[0][i];
		kvdram6[offset_kvs + i].data[1] = buffer6[1][i];
		kvdram6[offset_kvs + i].data[2] = buffer6[2][i];
		kvdram6[offset_kvs + i].data[3] = buffer6[3][i];
		kvdram6[offset_kvs + i].data[4] = buffer6[4][i];
		kvdram6[offset_kvs + i].data[5] = buffer6[5][i];
		kvdram6[offset_kvs + i].data[6] = buffer6[6][i];
		kvdram6[offset_kvs + i].data[7] = buffer6[7][i];
		kvdram7[offset_kvs + i].data[0] = buffer7[0][i];
		kvdram7[offset_kvs + i].data[1] = buffer7[1][i];
		kvdram7[offset_kvs + i].data[2] = buffer7[2][i];
		kvdram7[offset_kvs + i].data[3] = buffer7[3][i];
		kvdram7[offset_kvs + i].data[4] = buffer7[4][i];
		kvdram7[offset_kvs + i].data[5] = buffer7[5][i];
		kvdram7[offset_kvs + i].data[6] = buffer7[6][i];
		kvdram7[offset_kvs + i].data[7] = buffer7[7][i];
		kvdram8[offset_kvs + i].data[0] = buffer8[0][i];
		kvdram8[offset_kvs + i].data[1] = buffer8[1][i];
		kvdram8[offset_kvs + i].data[2] = buffer8[2][i];
		kvdram8[offset_kvs + i].data[3] = buffer8[3][i];
		kvdram8[offset_kvs + i].data[4] = buffer8[4][i];
		kvdram8[offset_kvs + i].data[5] = buffer8[5][i];
		kvdram8[offset_kvs + i].data[6] = buffer8[6][i];
		kvdram8[offset_kvs + i].data[7] = buffer8[7][i];
		kvdram9[offset_kvs + i].data[0] = buffer9[0][i];
		kvdram9[offset_kvs + i].data[1] = buffer9[1][i];
		kvdram9[offset_kvs + i].data[2] = buffer9[2][i];
		kvdram9[offset_kvs + i].data[3] = buffer9[3][i];
		kvdram9[offset_kvs + i].data[4] = buffer9[4][i];
		kvdram9[offset_kvs + i].data[5] = buffer9[5][i];
		kvdram9[offset_kvs + i].data[6] = buffer9[6][i];
		kvdram9[offset_kvs + i].data[7] = buffer9[7][i];
		kvdram10[offset_kvs + i].data[0] = buffer10[0][i];
		kvdram10[offset_kvs + i].data[1] = buffer10[1][i];
		kvdram10[offset_kvs + i].data[2] = buffer10[2][i];
		kvdram10[offset_kvs + i].data[3] = buffer10[3][i];
		kvdram10[offset_kvs + i].data[4] = buffer10[4][i];
		kvdram10[offset_kvs + i].data[5] = buffer10[5][i];
		kvdram10[offset_kvs + i].data[6] = buffer10[6][i];
		kvdram10[offset_kvs + i].data[7] = buffer10[7][i];
		kvdram11[offset_kvs + i].data[0] = buffer11[0][i];
		kvdram11[offset_kvs + i].data[1] = buffer11[1][i];
		kvdram11[offset_kvs + i].data[2] = buffer11[2][i];
		kvdram11[offset_kvs + i].data[3] = buffer11[3][i];
		kvdram11[offset_kvs + i].data[4] = buffer11[4][i];
		kvdram11[offset_kvs + i].data[5] = buffer11[5][i];
		kvdram11[offset_kvs + i].data[6] = buffer11[6][i];
		kvdram11[offset_kvs + i].data[7] = buffer11[7][i];
		#endif 
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvswritten(VECTOR_SIZE);
		#endif
	}
	#ifdef _DEBUGMODE_KERNELPRINTS2
	cout<< TIMINGRESULTSCOLOR<<"savekeyvalues_sync:: keyvalues saved: offset: "<<offset_kvs * VECTOR_SIZE<<"-"<<(offset_kvs + size_kvs) * VECTOR_SIZE<<", number of vertex datas written: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl;
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<< TIMINGRESULTSCOLOR<<"savekeyvalues_sync:: keyvalues saved: offset: "<<offset_kvs-BASEOFFSET_VERTEXPTR_KVS * VECTOR_SIZE<<"-"<<((offset_kvs + size_kvs)-BASEOFFSET_VERTEXPTR_KVS) * VECTOR_SIZE<<", number of vertex datas written: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl;
	cout<< TIMINGRESULTSCOLOR<<"savekeyvalues_sync:: keyvalues saved: offset: "<<(offset_kvs-BASEOFFSET_VERTICESDATA_KVS) * VECTOR_SIZE<<"-"<<((offset_kvs-BASEOFFSET_VERTICESDATA_KVS) + size_kvs) * VECTOR_SIZE<<", number of vertex datas written: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl;
	#endif
	return;
}

void 
	#ifdef SW 
	acts::
	#endif 
savekeyvalues_pipeline_sync(bool_type enable, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11, keyvalue_t offsetsandsizes[NUMCOMPUTEUNITS], keyvalue_t buffer0[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount =  (APPLYVERTEXBUFFERSZ / VDATAPACKINGFACTOR) / 2;
	
	// CRITICAL FIXME. static?
	static keyvalue_t buffer1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	static keyvalue_t buffer2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	static keyvalue_t buffer3[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	static keyvalue_t buffer4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	static keyvalue_t buffer5[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	static keyvalue_t buffer6[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	static keyvalue_t buffer7[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	static keyvalue_t buffer8[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	static keyvalue_t buffer9[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	static keyvalue_t buffer10[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	static keyvalue_t buffer11[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	
	offsetsandsizes[0].key = offset_kvs;
	offsetsandsizes[0].value = size_kvs;
	
	// SAVEKEYVALUESSYNC_LOOP: for (buffer_type i=0; i<size_kvs; i++){
	SAVEKEYVALUESSYNC_LOOP: for (buffer_type i=0; i<PADDEDDESTBUFFER_SIZE; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE II=1

			// read from 0
			keyvalue_t keyvalue00 = buffer0[0][i]; 
			keyvalue_t keyvalue01 = buffer0[1][i]; 
			keyvalue_t keyvalue02 = buffer0[2][i]; 
			keyvalue_t keyvalue03 = buffer0[3][i]; 
			keyvalue_t keyvalue04 = buffer0[4][i]; 
			keyvalue_t keyvalue05 = buffer0[5][i]; 
			keyvalue_t keyvalue06 = buffer0[6][i]; 
			keyvalue_t keyvalue07 = buffer0[7][i]; 
		
			#ifdef _WIDEWORD
			kvdram0[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(31, 0) = keyvalue00.key;
			kvdram0[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(63, 32) = keyvalue00.value;
			kvdram0[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(95, 64) = keyvalue01.key;
			kvdram0[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(127, 96) = keyvalue01.value;
			kvdram0[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(159, 128) = keyvalue02.key;
			kvdram0[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(191, 160) = keyvalue02.value;
			kvdram0[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(223, 192) = keyvalue03.key;
			kvdram0[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(255, 224) = keyvalue03.value;
			kvdram0[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(287, 256) = keyvalue04.key;
			kvdram0[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(319, 288) = keyvalue04.value;
			kvdram0[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(351, 320) = keyvalue05.key;
			kvdram0[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(383, 352) = keyvalue05.value;
			kvdram0[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(415, 384) = keyvalue06.key;
			kvdram0[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(447, 416) = keyvalue06.value;
			kvdram0[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(479, 448) = keyvalue07.key;
			kvdram0[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(511, 480) = keyvalue07.value;
			#else 
			kvdram0[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].data[0] = keyvalue00;
			kvdram0[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].data[1] = keyvalue01;
			kvdram0[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].data[2] = keyvalue02;
			kvdram0[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].data[3] = keyvalue03;
			kvdram0[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].data[4] = keyvalue04;
			kvdram0[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].data[5] = keyvalue05;
			kvdram0[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].data[6] = keyvalue06;
			kvdram0[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].data[7] = keyvalue07;
			#endif 
			// read from 1
			keyvalue_t keyvalue10 = buffer1[0][i]; 
			keyvalue_t keyvalue11 = buffer1[1][i]; 
			keyvalue_t keyvalue12 = buffer1[2][i]; 
			keyvalue_t keyvalue13 = buffer1[3][i]; 
			keyvalue_t keyvalue14 = buffer1[4][i]; 
			keyvalue_t keyvalue15 = buffer1[5][i]; 
			keyvalue_t keyvalue16 = buffer1[6][i]; 
			keyvalue_t keyvalue17 = buffer1[7][i]; 
		
			// write to 2 buffer
			buffer1[0][i] = keyvalue00; 
			buffer1[1][i] = keyvalue01; 
			buffer1[2][i] = keyvalue02; 
			buffer1[3][i] = keyvalue03; 
			buffer1[4][i] = keyvalue04; 
			buffer1[5][i] = keyvalue05; 
			buffer1[6][i] = keyvalue06; 
			buffer1[7][i] = keyvalue07; 
			
			// write to 2 dram
			#ifdef _WIDEWORD
			kvdram1[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(31, 0) = keyvalue00.key;
			kvdram1[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(63, 32) = keyvalue00.value;
			kvdram1[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(95, 64) = keyvalue01.key;
			kvdram1[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(127, 96) = keyvalue01.value;
			kvdram1[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(159, 128) = keyvalue02.key;
			kvdram1[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(191, 160) = keyvalue02.value;
			kvdram1[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(223, 192) = keyvalue03.key;
			kvdram1[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(255, 224) = keyvalue03.value;
			kvdram1[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(287, 256) = keyvalue04.key;
			kvdram1[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(319, 288) = keyvalue04.value;
			kvdram1[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(351, 320) = keyvalue05.key;
			kvdram1[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(383, 352) = keyvalue05.value;
			kvdram1[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(415, 384) = keyvalue06.key;
			kvdram1[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(447, 416) = keyvalue06.value;
			kvdram1[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(479, 448) = keyvalue07.key;
			kvdram1[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(511, 480) = keyvalue07.value;
			#else 
			kvdram1[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].data[0] = keyvalue00;
			kvdram1[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].data[1] = keyvalue01;
			kvdram1[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].data[2] = keyvalue02;
			kvdram1[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].data[3] = keyvalue03;
			kvdram1[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].data[4] = keyvalue04;
			kvdram1[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].data[5] = keyvalue05;
			kvdram1[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].data[6] = keyvalue06;
			kvdram1[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].data[7] = keyvalue07;
			#endif 
			// read from 2
			keyvalue_t keyvalue20 = buffer2[0][i]; 
			keyvalue_t keyvalue21 = buffer2[1][i]; 
			keyvalue_t keyvalue22 = buffer2[2][i]; 
			keyvalue_t keyvalue23 = buffer2[3][i]; 
			keyvalue_t keyvalue24 = buffer2[4][i]; 
			keyvalue_t keyvalue25 = buffer2[5][i]; 
			keyvalue_t keyvalue26 = buffer2[6][i]; 
			keyvalue_t keyvalue27 = buffer2[7][i]; 
		
			// write to 3 buffer
			buffer2[0][i] = keyvalue10; 
			buffer2[1][i] = keyvalue11; 
			buffer2[2][i] = keyvalue12; 
			buffer2[3][i] = keyvalue13; 
			buffer2[4][i] = keyvalue14; 
			buffer2[5][i] = keyvalue15; 
			buffer2[6][i] = keyvalue16; 
			buffer2[7][i] = keyvalue17; 
			
			// write to 3 dram
			#ifdef _WIDEWORD
			kvdram2[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[1].key + i].range(31, 0) = keyvalue10.key;
			kvdram2[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[1].key + i].range(63, 32) = keyvalue10.value;
			kvdram2[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[1].key + i].range(95, 64) = keyvalue11.key;
			kvdram2[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[1].key + i].range(127, 96) = keyvalue11.value;
			kvdram2[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[1].key + i].range(159, 128) = keyvalue12.key;
			kvdram2[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[1].key + i].range(191, 160) = keyvalue12.value;
			kvdram2[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[1].key + i].range(223, 192) = keyvalue13.key;
			kvdram2[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[1].key + i].range(255, 224) = keyvalue13.value;
			kvdram2[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[1].key + i].range(287, 256) = keyvalue14.key;
			kvdram2[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[1].key + i].range(319, 288) = keyvalue14.value;
			kvdram2[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[1].key + i].range(351, 320) = keyvalue15.key;
			kvdram2[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[1].key + i].range(383, 352) = keyvalue15.value;
			kvdram2[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[1].key + i].range(415, 384) = keyvalue16.key;
			kvdram2[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[1].key + i].range(447, 416) = keyvalue16.value;
			kvdram2[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[1].key + i].range(479, 448) = keyvalue17.key;
			kvdram2[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[1].key + i].range(511, 480) = keyvalue17.value;
			#else 
			kvdram2[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[1].key + i].data[0] = keyvalue10;
			kvdram2[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[1].key + i].data[1] = keyvalue11;
			kvdram2[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[1].key + i].data[2] = keyvalue12;
			kvdram2[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[1].key + i].data[3] = keyvalue13;
			kvdram2[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[1].key + i].data[4] = keyvalue14;
			kvdram2[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[1].key + i].data[5] = keyvalue15;
			kvdram2[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[1].key + i].data[6] = keyvalue16;
			kvdram2[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[1].key + i].data[7] = keyvalue17;
			#endif 
			// read from 3
			keyvalue_t keyvalue30 = buffer3[0][i]; 
			keyvalue_t keyvalue31 = buffer3[1][i]; 
			keyvalue_t keyvalue32 = buffer3[2][i]; 
			keyvalue_t keyvalue33 = buffer3[3][i]; 
			keyvalue_t keyvalue34 = buffer3[4][i]; 
			keyvalue_t keyvalue35 = buffer3[5][i]; 
			keyvalue_t keyvalue36 = buffer3[6][i]; 
			keyvalue_t keyvalue37 = buffer3[7][i]; 
		
			// write to 4 buffer
			buffer3[0][i] = keyvalue20; 
			buffer3[1][i] = keyvalue21; 
			buffer3[2][i] = keyvalue22; 
			buffer3[3][i] = keyvalue23; 
			buffer3[4][i] = keyvalue24; 
			buffer3[5][i] = keyvalue25; 
			buffer3[6][i] = keyvalue26; 
			buffer3[7][i] = keyvalue27; 
			
			// write to 4 dram
			#ifdef _WIDEWORD
			kvdram3[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[2].key + i].range(31, 0) = keyvalue20.key;
			kvdram3[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[2].key + i].range(63, 32) = keyvalue20.value;
			kvdram3[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[2].key + i].range(95, 64) = keyvalue21.key;
			kvdram3[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[2].key + i].range(127, 96) = keyvalue21.value;
			kvdram3[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[2].key + i].range(159, 128) = keyvalue22.key;
			kvdram3[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[2].key + i].range(191, 160) = keyvalue22.value;
			kvdram3[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[2].key + i].range(223, 192) = keyvalue23.key;
			kvdram3[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[2].key + i].range(255, 224) = keyvalue23.value;
			kvdram3[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[2].key + i].range(287, 256) = keyvalue24.key;
			kvdram3[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[2].key + i].range(319, 288) = keyvalue24.value;
			kvdram3[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[2].key + i].range(351, 320) = keyvalue25.key;
			kvdram3[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[2].key + i].range(383, 352) = keyvalue25.value;
			kvdram3[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[2].key + i].range(415, 384) = keyvalue26.key;
			kvdram3[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[2].key + i].range(447, 416) = keyvalue26.value;
			kvdram3[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[2].key + i].range(479, 448) = keyvalue27.key;
			kvdram3[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[2].key + i].range(511, 480) = keyvalue27.value;
			#else 
			kvdram3[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[2].key + i].data[0] = keyvalue20;
			kvdram3[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[2].key + i].data[1] = keyvalue21;
			kvdram3[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[2].key + i].data[2] = keyvalue22;
			kvdram3[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[2].key + i].data[3] = keyvalue23;
			kvdram3[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[2].key + i].data[4] = keyvalue24;
			kvdram3[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[2].key + i].data[5] = keyvalue25;
			kvdram3[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[2].key + i].data[6] = keyvalue26;
			kvdram3[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[2].key + i].data[7] = keyvalue27;
			#endif 
			// read from 4
			keyvalue_t keyvalue40 = buffer4[0][i]; 
			keyvalue_t keyvalue41 = buffer4[1][i]; 
			keyvalue_t keyvalue42 = buffer4[2][i]; 
			keyvalue_t keyvalue43 = buffer4[3][i]; 
			keyvalue_t keyvalue44 = buffer4[4][i]; 
			keyvalue_t keyvalue45 = buffer4[5][i]; 
			keyvalue_t keyvalue46 = buffer4[6][i]; 
			keyvalue_t keyvalue47 = buffer4[7][i]; 
		
			// write to 5 buffer
			buffer4[0][i] = keyvalue30; 
			buffer4[1][i] = keyvalue31; 
			buffer4[2][i] = keyvalue32; 
			buffer4[3][i] = keyvalue33; 
			buffer4[4][i] = keyvalue34; 
			buffer4[5][i] = keyvalue35; 
			buffer4[6][i] = keyvalue36; 
			buffer4[7][i] = keyvalue37; 
			
			// write to 5 dram
			#ifdef _WIDEWORD
			kvdram4[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[3].key + i].range(31, 0) = keyvalue30.key;
			kvdram4[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[3].key + i].range(63, 32) = keyvalue30.value;
			kvdram4[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[3].key + i].range(95, 64) = keyvalue31.key;
			kvdram4[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[3].key + i].range(127, 96) = keyvalue31.value;
			kvdram4[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[3].key + i].range(159, 128) = keyvalue32.key;
			kvdram4[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[3].key + i].range(191, 160) = keyvalue32.value;
			kvdram4[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[3].key + i].range(223, 192) = keyvalue33.key;
			kvdram4[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[3].key + i].range(255, 224) = keyvalue33.value;
			kvdram4[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[3].key + i].range(287, 256) = keyvalue34.key;
			kvdram4[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[3].key + i].range(319, 288) = keyvalue34.value;
			kvdram4[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[3].key + i].range(351, 320) = keyvalue35.key;
			kvdram4[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[3].key + i].range(383, 352) = keyvalue35.value;
			kvdram4[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[3].key + i].range(415, 384) = keyvalue36.key;
			kvdram4[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[3].key + i].range(447, 416) = keyvalue36.value;
			kvdram4[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[3].key + i].range(479, 448) = keyvalue37.key;
			kvdram4[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[3].key + i].range(511, 480) = keyvalue37.value;
			#else 
			kvdram4[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[3].key + i].data[0] = keyvalue30;
			kvdram4[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[3].key + i].data[1] = keyvalue31;
			kvdram4[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[3].key + i].data[2] = keyvalue32;
			kvdram4[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[3].key + i].data[3] = keyvalue33;
			kvdram4[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[3].key + i].data[4] = keyvalue34;
			kvdram4[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[3].key + i].data[5] = keyvalue35;
			kvdram4[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[3].key + i].data[6] = keyvalue36;
			kvdram4[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[3].key + i].data[7] = keyvalue37;
			#endif 
			// read from 5
			keyvalue_t keyvalue50 = buffer5[0][i]; 
			keyvalue_t keyvalue51 = buffer5[1][i]; 
			keyvalue_t keyvalue52 = buffer5[2][i]; 
			keyvalue_t keyvalue53 = buffer5[3][i]; 
			keyvalue_t keyvalue54 = buffer5[4][i]; 
			keyvalue_t keyvalue55 = buffer5[5][i]; 
			keyvalue_t keyvalue56 = buffer5[6][i]; 
			keyvalue_t keyvalue57 = buffer5[7][i]; 
		
			// write to 6 buffer
			buffer5[0][i] = keyvalue40; 
			buffer5[1][i] = keyvalue41; 
			buffer5[2][i] = keyvalue42; 
			buffer5[3][i] = keyvalue43; 
			buffer5[4][i] = keyvalue44; 
			buffer5[5][i] = keyvalue45; 
			buffer5[6][i] = keyvalue46; 
			buffer5[7][i] = keyvalue47; 
			
			// write to 6 dram
			#ifdef _WIDEWORD
			kvdram5[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[4].key + i].range(31, 0) = keyvalue40.key;
			kvdram5[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[4].key + i].range(63, 32) = keyvalue40.value;
			kvdram5[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[4].key + i].range(95, 64) = keyvalue41.key;
			kvdram5[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[4].key + i].range(127, 96) = keyvalue41.value;
			kvdram5[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[4].key + i].range(159, 128) = keyvalue42.key;
			kvdram5[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[4].key + i].range(191, 160) = keyvalue42.value;
			kvdram5[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[4].key + i].range(223, 192) = keyvalue43.key;
			kvdram5[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[4].key + i].range(255, 224) = keyvalue43.value;
			kvdram5[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[4].key + i].range(287, 256) = keyvalue44.key;
			kvdram5[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[4].key + i].range(319, 288) = keyvalue44.value;
			kvdram5[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[4].key + i].range(351, 320) = keyvalue45.key;
			kvdram5[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[4].key + i].range(383, 352) = keyvalue45.value;
			kvdram5[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[4].key + i].range(415, 384) = keyvalue46.key;
			kvdram5[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[4].key + i].range(447, 416) = keyvalue46.value;
			kvdram5[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[4].key + i].range(479, 448) = keyvalue47.key;
			kvdram5[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[4].key + i].range(511, 480) = keyvalue47.value;
			#else 
			kvdram5[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[4].key + i].data[0] = keyvalue40;
			kvdram5[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[4].key + i].data[1] = keyvalue41;
			kvdram5[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[4].key + i].data[2] = keyvalue42;
			kvdram5[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[4].key + i].data[3] = keyvalue43;
			kvdram5[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[4].key + i].data[4] = keyvalue44;
			kvdram5[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[4].key + i].data[5] = keyvalue45;
			kvdram5[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[4].key + i].data[6] = keyvalue46;
			kvdram5[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[4].key + i].data[7] = keyvalue47;
			#endif 
			// read from 6
			keyvalue_t keyvalue60 = buffer6[0][i]; 
			keyvalue_t keyvalue61 = buffer6[1][i]; 
			keyvalue_t keyvalue62 = buffer6[2][i]; 
			keyvalue_t keyvalue63 = buffer6[3][i]; 
			keyvalue_t keyvalue64 = buffer6[4][i]; 
			keyvalue_t keyvalue65 = buffer6[5][i]; 
			keyvalue_t keyvalue66 = buffer6[6][i]; 
			keyvalue_t keyvalue67 = buffer6[7][i]; 
		
			// write to 7 buffer
			buffer6[0][i] = keyvalue50; 
			buffer6[1][i] = keyvalue51; 
			buffer6[2][i] = keyvalue52; 
			buffer6[3][i] = keyvalue53; 
			buffer6[4][i] = keyvalue54; 
			buffer6[5][i] = keyvalue55; 
			buffer6[6][i] = keyvalue56; 
			buffer6[7][i] = keyvalue57; 
			
			// write to 7 dram
			#ifdef _WIDEWORD
			kvdram6[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[5].key + i].range(31, 0) = keyvalue50.key;
			kvdram6[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[5].key + i].range(63, 32) = keyvalue50.value;
			kvdram6[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[5].key + i].range(95, 64) = keyvalue51.key;
			kvdram6[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[5].key + i].range(127, 96) = keyvalue51.value;
			kvdram6[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[5].key + i].range(159, 128) = keyvalue52.key;
			kvdram6[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[5].key + i].range(191, 160) = keyvalue52.value;
			kvdram6[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[5].key + i].range(223, 192) = keyvalue53.key;
			kvdram6[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[5].key + i].range(255, 224) = keyvalue53.value;
			kvdram6[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[5].key + i].range(287, 256) = keyvalue54.key;
			kvdram6[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[5].key + i].range(319, 288) = keyvalue54.value;
			kvdram6[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[5].key + i].range(351, 320) = keyvalue55.key;
			kvdram6[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[5].key + i].range(383, 352) = keyvalue55.value;
			kvdram6[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[5].key + i].range(415, 384) = keyvalue56.key;
			kvdram6[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[5].key + i].range(447, 416) = keyvalue56.value;
			kvdram6[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[5].key + i].range(479, 448) = keyvalue57.key;
			kvdram6[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[5].key + i].range(511, 480) = keyvalue57.value;
			#else 
			kvdram6[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[5].key + i].data[0] = keyvalue50;
			kvdram6[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[5].key + i].data[1] = keyvalue51;
			kvdram6[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[5].key + i].data[2] = keyvalue52;
			kvdram6[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[5].key + i].data[3] = keyvalue53;
			kvdram6[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[5].key + i].data[4] = keyvalue54;
			kvdram6[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[5].key + i].data[5] = keyvalue55;
			kvdram6[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[5].key + i].data[6] = keyvalue56;
			kvdram6[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[5].key + i].data[7] = keyvalue57;
			#endif 
			// read from 7
			keyvalue_t keyvalue70 = buffer7[0][i]; 
			keyvalue_t keyvalue71 = buffer7[1][i]; 
			keyvalue_t keyvalue72 = buffer7[2][i]; 
			keyvalue_t keyvalue73 = buffer7[3][i]; 
			keyvalue_t keyvalue74 = buffer7[4][i]; 
			keyvalue_t keyvalue75 = buffer7[5][i]; 
			keyvalue_t keyvalue76 = buffer7[6][i]; 
			keyvalue_t keyvalue77 = buffer7[7][i]; 
		
			// write to 8 buffer
			buffer7[0][i] = keyvalue60; 
			buffer7[1][i] = keyvalue61; 
			buffer7[2][i] = keyvalue62; 
			buffer7[3][i] = keyvalue63; 
			buffer7[4][i] = keyvalue64; 
			buffer7[5][i] = keyvalue65; 
			buffer7[6][i] = keyvalue66; 
			buffer7[7][i] = keyvalue67; 
			
			// write to 8 dram
			#ifdef _WIDEWORD
			kvdram7[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[6].key + i].range(31, 0) = keyvalue60.key;
			kvdram7[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[6].key + i].range(63, 32) = keyvalue60.value;
			kvdram7[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[6].key + i].range(95, 64) = keyvalue61.key;
			kvdram7[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[6].key + i].range(127, 96) = keyvalue61.value;
			kvdram7[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[6].key + i].range(159, 128) = keyvalue62.key;
			kvdram7[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[6].key + i].range(191, 160) = keyvalue62.value;
			kvdram7[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[6].key + i].range(223, 192) = keyvalue63.key;
			kvdram7[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[6].key + i].range(255, 224) = keyvalue63.value;
			kvdram7[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[6].key + i].range(287, 256) = keyvalue64.key;
			kvdram7[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[6].key + i].range(319, 288) = keyvalue64.value;
			kvdram7[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[6].key + i].range(351, 320) = keyvalue65.key;
			kvdram7[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[6].key + i].range(383, 352) = keyvalue65.value;
			kvdram7[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[6].key + i].range(415, 384) = keyvalue66.key;
			kvdram7[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[6].key + i].range(447, 416) = keyvalue66.value;
			kvdram7[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[6].key + i].range(479, 448) = keyvalue67.key;
			kvdram7[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[6].key + i].range(511, 480) = keyvalue67.value;
			#else 
			kvdram7[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[6].key + i].data[0] = keyvalue60;
			kvdram7[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[6].key + i].data[1] = keyvalue61;
			kvdram7[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[6].key + i].data[2] = keyvalue62;
			kvdram7[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[6].key + i].data[3] = keyvalue63;
			kvdram7[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[6].key + i].data[4] = keyvalue64;
			kvdram7[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[6].key + i].data[5] = keyvalue65;
			kvdram7[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[6].key + i].data[6] = keyvalue66;
			kvdram7[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[6].key + i].data[7] = keyvalue67;
			#endif 
			// read from 8
			keyvalue_t keyvalue80 = buffer8[0][i]; 
			keyvalue_t keyvalue81 = buffer8[1][i]; 
			keyvalue_t keyvalue82 = buffer8[2][i]; 
			keyvalue_t keyvalue83 = buffer8[3][i]; 
			keyvalue_t keyvalue84 = buffer8[4][i]; 
			keyvalue_t keyvalue85 = buffer8[5][i]; 
			keyvalue_t keyvalue86 = buffer8[6][i]; 
			keyvalue_t keyvalue87 = buffer8[7][i]; 
		
			// write to 9 buffer
			buffer8[0][i] = keyvalue70; 
			buffer8[1][i] = keyvalue71; 
			buffer8[2][i] = keyvalue72; 
			buffer8[3][i] = keyvalue73; 
			buffer8[4][i] = keyvalue74; 
			buffer8[5][i] = keyvalue75; 
			buffer8[6][i] = keyvalue76; 
			buffer8[7][i] = keyvalue77; 
			
			// write to 9 dram
			#ifdef _WIDEWORD
			kvdram8[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[7].key + i].range(31, 0) = keyvalue70.key;
			kvdram8[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[7].key + i].range(63, 32) = keyvalue70.value;
			kvdram8[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[7].key + i].range(95, 64) = keyvalue71.key;
			kvdram8[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[7].key + i].range(127, 96) = keyvalue71.value;
			kvdram8[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[7].key + i].range(159, 128) = keyvalue72.key;
			kvdram8[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[7].key + i].range(191, 160) = keyvalue72.value;
			kvdram8[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[7].key + i].range(223, 192) = keyvalue73.key;
			kvdram8[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[7].key + i].range(255, 224) = keyvalue73.value;
			kvdram8[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[7].key + i].range(287, 256) = keyvalue74.key;
			kvdram8[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[7].key + i].range(319, 288) = keyvalue74.value;
			kvdram8[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[7].key + i].range(351, 320) = keyvalue75.key;
			kvdram8[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[7].key + i].range(383, 352) = keyvalue75.value;
			kvdram8[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[7].key + i].range(415, 384) = keyvalue76.key;
			kvdram8[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[7].key + i].range(447, 416) = keyvalue76.value;
			kvdram8[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[7].key + i].range(479, 448) = keyvalue77.key;
			kvdram8[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[7].key + i].range(511, 480) = keyvalue77.value;
			#else 
			kvdram8[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[7].key + i].data[0] = keyvalue70;
			kvdram8[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[7].key + i].data[1] = keyvalue71;
			kvdram8[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[7].key + i].data[2] = keyvalue72;
			kvdram8[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[7].key + i].data[3] = keyvalue73;
			kvdram8[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[7].key + i].data[4] = keyvalue74;
			kvdram8[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[7].key + i].data[5] = keyvalue75;
			kvdram8[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[7].key + i].data[6] = keyvalue76;
			kvdram8[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[7].key + i].data[7] = keyvalue77;
			#endif 
			// read from 9
			keyvalue_t keyvalue90 = buffer9[0][i]; 
			keyvalue_t keyvalue91 = buffer9[1][i]; 
			keyvalue_t keyvalue92 = buffer9[2][i]; 
			keyvalue_t keyvalue93 = buffer9[3][i]; 
			keyvalue_t keyvalue94 = buffer9[4][i]; 
			keyvalue_t keyvalue95 = buffer9[5][i]; 
			keyvalue_t keyvalue96 = buffer9[6][i]; 
			keyvalue_t keyvalue97 = buffer9[7][i]; 
		
			// write to 10 buffer
			buffer9[0][i] = keyvalue80; 
			buffer9[1][i] = keyvalue81; 
			buffer9[2][i] = keyvalue82; 
			buffer9[3][i] = keyvalue83; 
			buffer9[4][i] = keyvalue84; 
			buffer9[5][i] = keyvalue85; 
			buffer9[6][i] = keyvalue86; 
			buffer9[7][i] = keyvalue87; 
			
			// write to 10 dram
			#ifdef _WIDEWORD
			kvdram9[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[8].key + i].range(31, 0) = keyvalue80.key;
			kvdram9[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[8].key + i].range(63, 32) = keyvalue80.value;
			kvdram9[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[8].key + i].range(95, 64) = keyvalue81.key;
			kvdram9[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[8].key + i].range(127, 96) = keyvalue81.value;
			kvdram9[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[8].key + i].range(159, 128) = keyvalue82.key;
			kvdram9[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[8].key + i].range(191, 160) = keyvalue82.value;
			kvdram9[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[8].key + i].range(223, 192) = keyvalue83.key;
			kvdram9[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[8].key + i].range(255, 224) = keyvalue83.value;
			kvdram9[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[8].key + i].range(287, 256) = keyvalue84.key;
			kvdram9[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[8].key + i].range(319, 288) = keyvalue84.value;
			kvdram9[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[8].key + i].range(351, 320) = keyvalue85.key;
			kvdram9[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[8].key + i].range(383, 352) = keyvalue85.value;
			kvdram9[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[8].key + i].range(415, 384) = keyvalue86.key;
			kvdram9[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[8].key + i].range(447, 416) = keyvalue86.value;
			kvdram9[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[8].key + i].range(479, 448) = keyvalue87.key;
			kvdram9[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[8].key + i].range(511, 480) = keyvalue87.value;
			#else 
			kvdram9[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[8].key + i].data[0] = keyvalue80;
			kvdram9[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[8].key + i].data[1] = keyvalue81;
			kvdram9[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[8].key + i].data[2] = keyvalue82;
			kvdram9[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[8].key + i].data[3] = keyvalue83;
			kvdram9[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[8].key + i].data[4] = keyvalue84;
			kvdram9[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[8].key + i].data[5] = keyvalue85;
			kvdram9[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[8].key + i].data[6] = keyvalue86;
			kvdram9[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[8].key + i].data[7] = keyvalue87;
			#endif 
			// read from 10
			keyvalue_t keyvalue100 = buffer10[0][i]; 
			keyvalue_t keyvalue101 = buffer10[1][i]; 
			keyvalue_t keyvalue102 = buffer10[2][i]; 
			keyvalue_t keyvalue103 = buffer10[3][i]; 
			keyvalue_t keyvalue104 = buffer10[4][i]; 
			keyvalue_t keyvalue105 = buffer10[5][i]; 
			keyvalue_t keyvalue106 = buffer10[6][i]; 
			keyvalue_t keyvalue107 = buffer10[7][i]; 
		
			// write to 11 buffer
			buffer10[0][i] = keyvalue90; 
			buffer10[1][i] = keyvalue91; 
			buffer10[2][i] = keyvalue92; 
			buffer10[3][i] = keyvalue93; 
			buffer10[4][i] = keyvalue94; 
			buffer10[5][i] = keyvalue95; 
			buffer10[6][i] = keyvalue96; 
			buffer10[7][i] = keyvalue97; 
			
			// write to 11 dram
			#ifdef _WIDEWORD
			kvdram10[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[9].key + i].range(31, 0) = keyvalue90.key;
			kvdram10[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[9].key + i].range(63, 32) = keyvalue90.value;
			kvdram10[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[9].key + i].range(95, 64) = keyvalue91.key;
			kvdram10[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[9].key + i].range(127, 96) = keyvalue91.value;
			kvdram10[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[9].key + i].range(159, 128) = keyvalue92.key;
			kvdram10[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[9].key + i].range(191, 160) = keyvalue92.value;
			kvdram10[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[9].key + i].range(223, 192) = keyvalue93.key;
			kvdram10[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[9].key + i].range(255, 224) = keyvalue93.value;
			kvdram10[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[9].key + i].range(287, 256) = keyvalue94.key;
			kvdram10[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[9].key + i].range(319, 288) = keyvalue94.value;
			kvdram10[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[9].key + i].range(351, 320) = keyvalue95.key;
			kvdram10[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[9].key + i].range(383, 352) = keyvalue95.value;
			kvdram10[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[9].key + i].range(415, 384) = keyvalue96.key;
			kvdram10[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[9].key + i].range(447, 416) = keyvalue96.value;
			kvdram10[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[9].key + i].range(479, 448) = keyvalue97.key;
			kvdram10[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[9].key + i].range(511, 480) = keyvalue97.value;
			#else 
			kvdram10[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[9].key + i].data[0] = keyvalue90;
			kvdram10[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[9].key + i].data[1] = keyvalue91;
			kvdram10[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[9].key + i].data[2] = keyvalue92;
			kvdram10[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[9].key + i].data[3] = keyvalue93;
			kvdram10[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[9].key + i].data[4] = keyvalue94;
			kvdram10[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[9].key + i].data[5] = keyvalue95;
			kvdram10[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[9].key + i].data[6] = keyvalue96;
			kvdram10[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[9].key + i].data[7] = keyvalue97;
			#endif 
			// read from 11
			keyvalue_t keyvalue110 = buffer11[0][i]; 
			keyvalue_t keyvalue111 = buffer11[1][i]; 
			keyvalue_t keyvalue112 = buffer11[2][i]; 
			keyvalue_t keyvalue113 = buffer11[3][i]; 
			keyvalue_t keyvalue114 = buffer11[4][i]; 
			keyvalue_t keyvalue115 = buffer11[5][i]; 
			keyvalue_t keyvalue116 = buffer11[6][i]; 
			keyvalue_t keyvalue117 = buffer11[7][i]; 
		
			// write to 12 buffer
			buffer11[0][i] = keyvalue100; 
			buffer11[1][i] = keyvalue101; 
			buffer11[2][i] = keyvalue102; 
			buffer11[3][i] = keyvalue103; 
			buffer11[4][i] = keyvalue104; 
			buffer11[5][i] = keyvalue105; 
			buffer11[6][i] = keyvalue106; 
			buffer11[7][i] = keyvalue107; 
			
			// write to 12 dram
			#ifdef _WIDEWORD
			kvdram11[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[10].key + i].range(31, 0) = keyvalue100.key;
			kvdram11[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[10].key + i].range(63, 32) = keyvalue100.value;
			kvdram11[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[10].key + i].range(95, 64) = keyvalue101.key;
			kvdram11[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[10].key + i].range(127, 96) = keyvalue101.value;
			kvdram11[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[10].key + i].range(159, 128) = keyvalue102.key;
			kvdram11[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[10].key + i].range(191, 160) = keyvalue102.value;
			kvdram11[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[10].key + i].range(223, 192) = keyvalue103.key;
			kvdram11[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[10].key + i].range(255, 224) = keyvalue103.value;
			kvdram11[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[10].key + i].range(287, 256) = keyvalue104.key;
			kvdram11[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[10].key + i].range(319, 288) = keyvalue104.value;
			kvdram11[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[10].key + i].range(351, 320) = keyvalue105.key;
			kvdram11[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[10].key + i].range(383, 352) = keyvalue105.value;
			kvdram11[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[10].key + i].range(415, 384) = keyvalue106.key;
			kvdram11[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[10].key + i].range(447, 416) = keyvalue106.value;
			kvdram11[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[10].key + i].range(479, 448) = keyvalue107.key;
			kvdram11[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[10].key + i].range(511, 480) = keyvalue107.value;
			#else 
			kvdram11[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[10].key + i].data[0] = keyvalue100;
			kvdram11[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[10].key + i].data[1] = keyvalue101;
			kvdram11[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[10].key + i].data[2] = keyvalue102;
			kvdram11[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[10].key + i].data[3] = keyvalue103;
			kvdram11[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[10].key + i].data[4] = keyvalue104;
			kvdram11[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[10].key + i].data[5] = keyvalue105;
			kvdram11[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[10].key + i].data[6] = keyvalue106;
			kvdram11[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[10].key + i].data[7] = keyvalue107;
			#endif 
	}
	
	// shift offsets and sizes
	for(unsigned int i=NUMCOMPUTEUNITS-1; i>0; i--){ 
	#pragma HLS PIPELINE II=1
		offsetsandsizes[i].key = offsetsandsizes[i-1].key; 
		offsetsandsizes[i].value = offsetsandsizes[i-1].value; 
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS2
	cout<< TIMINGRESULTSCOLOR<<"savekeyvalues_pipeline_sync:: keyvalues saved: offset: "<<offset_kvs * VECTOR_SIZE<<"-"<<(offset_kvs + size_kvs) * VECTOR_SIZE<<", num keyvalues written: "<<size_kvs * VECTOR_SIZE<< RESET<<endl;
	#endif
	return;
}

void 
	#ifdef SW
	acts::
	#endif 
savekeyvalues_pipeline_2sync(bool_type enable, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11, keyvalue_t offsetsandsizes[NUMCOMPUTEUNITS], keyvalue_t buffer0[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount =  (APPLYVERTEXBUFFERSZ / VDATAPACKINGFACTOR) / 2;
	
	static keyvalue_t buffer1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	static keyvalue_t buffer2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	static keyvalue_t buffer3[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	static keyvalue_t buffer4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	static keyvalue_t buffer5[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	static keyvalue_t buffer6[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	static keyvalue_t buffer7[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	static keyvalue_t buffer8[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	static keyvalue_t buffer9[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	static keyvalue_t buffer10[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	static keyvalue_t buffer11[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	
	offsetsandsizes[0].key = offset_kvs;
	offsetsandsizes[0].value = size_kvs;
	
	// SAVEKEYVALUESSYNC_LOOP: for (buffer_type i=0; i<size_kvs; i++){
	SAVEKEYVALUESSYNC_LOOP: for (buffer_type i=0; i<PADDEDDESTBUFFER_SIZE; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE II=1 // CRITICAL FIXME.

			// read from 0
			keyvalue_t keyvalue00 = buffer0[0][i]; 
			keyvalue_t keyvalue01 = buffer0[1][i]; 
			keyvalue_t keyvalue02 = buffer0[2][i]; 
			keyvalue_t keyvalue03 = buffer0[3][i]; 
			keyvalue_t keyvalue04 = buffer0[4][i]; 
			keyvalue_t keyvalue05 = buffer0[5][i]; 
			keyvalue_t keyvalue06 = buffer0[6][i]; 
			keyvalue_t keyvalue07 = buffer0[7][i]; 
		
			#ifdef _WIDEWORD
			kvdram0[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(31, 0) = keyvalue00.key;
			kvdram0[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(63, 32) = keyvalue00.value;
			kvdram0[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(95, 64) = keyvalue01.key;
			kvdram0[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(127, 96) = keyvalue01.value;
			kvdram0[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(159, 128) = keyvalue02.key;
			kvdram0[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(191, 160) = keyvalue02.value;
			kvdram0[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(223, 192) = keyvalue03.key;
			kvdram0[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(255, 224) = keyvalue03.value;
			kvdram0[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(287, 256) = keyvalue04.key;
			kvdram0[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(319, 288) = keyvalue04.value;
			kvdram0[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(351, 320) = keyvalue05.key;
			kvdram0[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(383, 352) = keyvalue05.value;
			kvdram0[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(415, 384) = keyvalue06.key;
			kvdram0[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(447, 416) = keyvalue06.value;
			kvdram0[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(479, 448) = keyvalue07.key;
			kvdram0[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(511, 480) = keyvalue07.value;
			#else 
			kvdram0[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].data[0] = keyvalue00;
			kvdram0[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].data[1] = keyvalue01;
			kvdram0[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].data[2] = keyvalue02;
			kvdram0[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].data[3] = keyvalue03;
			kvdram0[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].data[4] = keyvalue04;
			kvdram0[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].data[5] = keyvalue05;
			kvdram0[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].data[6] = keyvalue06;
			kvdram0[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].data[7] = keyvalue07;
			#endif 
			// read from 1
			keyvalue_t keyvalue10 = buffer1[0][i]; 
			keyvalue_t keyvalue11 = buffer1[1][i]; 
			keyvalue_t keyvalue12 = buffer1[2][i]; 
			keyvalue_t keyvalue13 = buffer1[3][i]; 
			keyvalue_t keyvalue14 = buffer1[4][i]; 
			keyvalue_t keyvalue15 = buffer1[5][i]; 
			keyvalue_t keyvalue16 = buffer1[6][i]; 
			keyvalue_t keyvalue17 = buffer1[7][i]; 
		
			
			// write to 1 dram
			#ifdef _WIDEWORD
			kvdram1[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(31, 0) = keyvalue00.key;
			kvdram1[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(63, 32) = keyvalue00.value;
			kvdram1[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(95, 64) = keyvalue01.key;
			kvdram1[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(127, 96) = keyvalue01.value;
			kvdram1[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(159, 128) = keyvalue02.key;
			kvdram1[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(191, 160) = keyvalue02.value;
			kvdram1[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(223, 192) = keyvalue03.key;
			kvdram1[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(255, 224) = keyvalue03.value;
			kvdram1[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(287, 256) = keyvalue04.key;
			kvdram1[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(319, 288) = keyvalue04.value;
			kvdram1[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(351, 320) = keyvalue05.key;
			kvdram1[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(383, 352) = keyvalue05.value;
			kvdram1[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(415, 384) = keyvalue06.key;
			kvdram1[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(447, 416) = keyvalue06.value;
			kvdram1[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(479, 448) = keyvalue07.key;
			kvdram1[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].range(511, 480) = keyvalue07.value;
			#else 
			kvdram1[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].data[0] = keyvalue00;
			kvdram1[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].data[1] = keyvalue01;
			kvdram1[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].data[2] = keyvalue02;
			kvdram1[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].data[3] = keyvalue03;
			kvdram1[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].data[4] = keyvalue04;
			kvdram1[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].data[5] = keyvalue05;
			kvdram1[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].data[6] = keyvalue06;
			kvdram1[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[0].key + i].data[7] = keyvalue07;
			#endif 
			// read from 2
			keyvalue_t keyvalue20 = buffer2[0][i]; 
			keyvalue_t keyvalue21 = buffer2[1][i]; 
			keyvalue_t keyvalue22 = buffer2[2][i]; 
			keyvalue_t keyvalue23 = buffer2[3][i]; 
			keyvalue_t keyvalue24 = buffer2[4][i]; 
			keyvalue_t keyvalue25 = buffer2[5][i]; 
			keyvalue_t keyvalue26 = buffer2[6][i]; 
			keyvalue_t keyvalue27 = buffer2[7][i]; 
		
			
			// write to 2 dram
			#ifdef _WIDEWORD
			kvdram2[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[1].key + i].range(31, 0) = keyvalue10.key;
			kvdram2[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[1].key + i].range(63, 32) = keyvalue10.value;
			kvdram2[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[1].key + i].range(95, 64) = keyvalue11.key;
			kvdram2[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[1].key + i].range(127, 96) = keyvalue11.value;
			kvdram2[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[1].key + i].range(159, 128) = keyvalue12.key;
			kvdram2[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[1].key + i].range(191, 160) = keyvalue12.value;
			kvdram2[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[1].key + i].range(223, 192) = keyvalue13.key;
			kvdram2[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[1].key + i].range(255, 224) = keyvalue13.value;
			kvdram2[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[1].key + i].range(287, 256) = keyvalue14.key;
			kvdram2[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[1].key + i].range(319, 288) = keyvalue14.value;
			kvdram2[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[1].key + i].range(351, 320) = keyvalue15.key;
			kvdram2[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[1].key + i].range(383, 352) = keyvalue15.value;
			kvdram2[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[1].key + i].range(415, 384) = keyvalue16.key;
			kvdram2[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[1].key + i].range(447, 416) = keyvalue16.value;
			kvdram2[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[1].key + i].range(479, 448) = keyvalue17.key;
			kvdram2[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[1].key + i].range(511, 480) = keyvalue17.value;
			#else 
			kvdram2[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[1].key + i].data[0] = keyvalue10;
			kvdram2[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[1].key + i].data[1] = keyvalue11;
			kvdram2[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[1].key + i].data[2] = keyvalue12;
			kvdram2[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[1].key + i].data[3] = keyvalue13;
			kvdram2[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[1].key + i].data[4] = keyvalue14;
			kvdram2[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[1].key + i].data[5] = keyvalue15;
			kvdram2[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[1].key + i].data[6] = keyvalue16;
			kvdram2[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[1].key + i].data[7] = keyvalue17;
			#endif 
			// read from 3
			keyvalue_t keyvalue30 = buffer3[0][i]; 
			keyvalue_t keyvalue31 = buffer3[1][i]; 
			keyvalue_t keyvalue32 = buffer3[2][i]; 
			keyvalue_t keyvalue33 = buffer3[3][i]; 
			keyvalue_t keyvalue34 = buffer3[4][i]; 
			keyvalue_t keyvalue35 = buffer3[5][i]; 
			keyvalue_t keyvalue36 = buffer3[6][i]; 
			keyvalue_t keyvalue37 = buffer3[7][i]; 
		
			
			// write to 3 dram
			#ifdef _WIDEWORD
			kvdram3[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[2].key + i].range(31, 0) = keyvalue20.key;
			kvdram3[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[2].key + i].range(63, 32) = keyvalue20.value;
			kvdram3[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[2].key + i].range(95, 64) = keyvalue21.key;
			kvdram3[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[2].key + i].range(127, 96) = keyvalue21.value;
			kvdram3[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[2].key + i].range(159, 128) = keyvalue22.key;
			kvdram3[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[2].key + i].range(191, 160) = keyvalue22.value;
			kvdram3[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[2].key + i].range(223, 192) = keyvalue23.key;
			kvdram3[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[2].key + i].range(255, 224) = keyvalue23.value;
			kvdram3[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[2].key + i].range(287, 256) = keyvalue24.key;
			kvdram3[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[2].key + i].range(319, 288) = keyvalue24.value;
			kvdram3[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[2].key + i].range(351, 320) = keyvalue25.key;
			kvdram3[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[2].key + i].range(383, 352) = keyvalue25.value;
			kvdram3[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[2].key + i].range(415, 384) = keyvalue26.key;
			kvdram3[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[2].key + i].range(447, 416) = keyvalue26.value;
			kvdram3[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[2].key + i].range(479, 448) = keyvalue27.key;
			kvdram3[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[2].key + i].range(511, 480) = keyvalue27.value;
			#else 
			kvdram3[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[2].key + i].data[0] = keyvalue20;
			kvdram3[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[2].key + i].data[1] = keyvalue21;
			kvdram3[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[2].key + i].data[2] = keyvalue22;
			kvdram3[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[2].key + i].data[3] = keyvalue23;
			kvdram3[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[2].key + i].data[4] = keyvalue24;
			kvdram3[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[2].key + i].data[5] = keyvalue25;
			kvdram3[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[2].key + i].data[6] = keyvalue26;
			kvdram3[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[2].key + i].data[7] = keyvalue27;
			#endif 
			// read from 4
			keyvalue_t keyvalue40 = buffer4[0][i]; 
			keyvalue_t keyvalue41 = buffer4[1][i]; 
			keyvalue_t keyvalue42 = buffer4[2][i]; 
			keyvalue_t keyvalue43 = buffer4[3][i]; 
			keyvalue_t keyvalue44 = buffer4[4][i]; 
			keyvalue_t keyvalue45 = buffer4[5][i]; 
			keyvalue_t keyvalue46 = buffer4[6][i]; 
			keyvalue_t keyvalue47 = buffer4[7][i]; 
		
			
			// write to 4 dram
			#ifdef _WIDEWORD
			kvdram4[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[3].key + i].range(31, 0) = keyvalue30.key;
			kvdram4[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[3].key + i].range(63, 32) = keyvalue30.value;
			kvdram4[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[3].key + i].range(95, 64) = keyvalue31.key;
			kvdram4[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[3].key + i].range(127, 96) = keyvalue31.value;
			kvdram4[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[3].key + i].range(159, 128) = keyvalue32.key;
			kvdram4[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[3].key + i].range(191, 160) = keyvalue32.value;
			kvdram4[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[3].key + i].range(223, 192) = keyvalue33.key;
			kvdram4[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[3].key + i].range(255, 224) = keyvalue33.value;
			kvdram4[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[3].key + i].range(287, 256) = keyvalue34.key;
			kvdram4[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[3].key + i].range(319, 288) = keyvalue34.value;
			kvdram4[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[3].key + i].range(351, 320) = keyvalue35.key;
			kvdram4[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[3].key + i].range(383, 352) = keyvalue35.value;
			kvdram4[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[3].key + i].range(415, 384) = keyvalue36.key;
			kvdram4[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[3].key + i].range(447, 416) = keyvalue36.value;
			kvdram4[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[3].key + i].range(479, 448) = keyvalue37.key;
			kvdram4[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[3].key + i].range(511, 480) = keyvalue37.value;
			#else 
			kvdram4[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[3].key + i].data[0] = keyvalue30;
			kvdram4[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[3].key + i].data[1] = keyvalue31;
			kvdram4[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[3].key + i].data[2] = keyvalue32;
			kvdram4[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[3].key + i].data[3] = keyvalue33;
			kvdram4[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[3].key + i].data[4] = keyvalue34;
			kvdram4[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[3].key + i].data[5] = keyvalue35;
			kvdram4[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[3].key + i].data[6] = keyvalue36;
			kvdram4[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[3].key + i].data[7] = keyvalue37;
			#endif 
			// read from 5
			keyvalue_t keyvalue50 = buffer5[0][i]; 
			keyvalue_t keyvalue51 = buffer5[1][i]; 
			keyvalue_t keyvalue52 = buffer5[2][i]; 
			keyvalue_t keyvalue53 = buffer5[3][i]; 
			keyvalue_t keyvalue54 = buffer5[4][i]; 
			keyvalue_t keyvalue55 = buffer5[5][i]; 
			keyvalue_t keyvalue56 = buffer5[6][i]; 
			keyvalue_t keyvalue57 = buffer5[7][i]; 
		
			
			// write to 5 dram
			#ifdef _WIDEWORD
			kvdram5[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[4].key + i].range(31, 0) = keyvalue40.key;
			kvdram5[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[4].key + i].range(63, 32) = keyvalue40.value;
			kvdram5[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[4].key + i].range(95, 64) = keyvalue41.key;
			kvdram5[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[4].key + i].range(127, 96) = keyvalue41.value;
			kvdram5[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[4].key + i].range(159, 128) = keyvalue42.key;
			kvdram5[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[4].key + i].range(191, 160) = keyvalue42.value;
			kvdram5[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[4].key + i].range(223, 192) = keyvalue43.key;
			kvdram5[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[4].key + i].range(255, 224) = keyvalue43.value;
			kvdram5[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[4].key + i].range(287, 256) = keyvalue44.key;
			kvdram5[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[4].key + i].range(319, 288) = keyvalue44.value;
			kvdram5[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[4].key + i].range(351, 320) = keyvalue45.key;
			kvdram5[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[4].key + i].range(383, 352) = keyvalue45.value;
			kvdram5[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[4].key + i].range(415, 384) = keyvalue46.key;
			kvdram5[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[4].key + i].range(447, 416) = keyvalue46.value;
			kvdram5[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[4].key + i].range(479, 448) = keyvalue47.key;
			kvdram5[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[4].key + i].range(511, 480) = keyvalue47.value;
			#else 
			kvdram5[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[4].key + i].data[0] = keyvalue40;
			kvdram5[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[4].key + i].data[1] = keyvalue41;
			kvdram5[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[4].key + i].data[2] = keyvalue42;
			kvdram5[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[4].key + i].data[3] = keyvalue43;
			kvdram5[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[4].key + i].data[4] = keyvalue44;
			kvdram5[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[4].key + i].data[5] = keyvalue45;
			kvdram5[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[4].key + i].data[6] = keyvalue46;
			kvdram5[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[4].key + i].data[7] = keyvalue47;
			#endif 
			// read from 6
			keyvalue_t keyvalue60 = buffer6[0][i]; 
			keyvalue_t keyvalue61 = buffer6[1][i]; 
			keyvalue_t keyvalue62 = buffer6[2][i]; 
			keyvalue_t keyvalue63 = buffer6[3][i]; 
			keyvalue_t keyvalue64 = buffer6[4][i]; 
			keyvalue_t keyvalue65 = buffer6[5][i]; 
			keyvalue_t keyvalue66 = buffer6[6][i]; 
			keyvalue_t keyvalue67 = buffer6[7][i]; 
		
			
			// write to 6 dram
			#ifdef _WIDEWORD
			kvdram6[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[5].key + i].range(31, 0) = keyvalue50.key;
			kvdram6[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[5].key + i].range(63, 32) = keyvalue50.value;
			kvdram6[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[5].key + i].range(95, 64) = keyvalue51.key;
			kvdram6[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[5].key + i].range(127, 96) = keyvalue51.value;
			kvdram6[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[5].key + i].range(159, 128) = keyvalue52.key;
			kvdram6[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[5].key + i].range(191, 160) = keyvalue52.value;
			kvdram6[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[5].key + i].range(223, 192) = keyvalue53.key;
			kvdram6[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[5].key + i].range(255, 224) = keyvalue53.value;
			kvdram6[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[5].key + i].range(287, 256) = keyvalue54.key;
			kvdram6[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[5].key + i].range(319, 288) = keyvalue54.value;
			kvdram6[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[5].key + i].range(351, 320) = keyvalue55.key;
			kvdram6[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[5].key + i].range(383, 352) = keyvalue55.value;
			kvdram6[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[5].key + i].range(415, 384) = keyvalue56.key;
			kvdram6[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[5].key + i].range(447, 416) = keyvalue56.value;
			kvdram6[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[5].key + i].range(479, 448) = keyvalue57.key;
			kvdram6[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[5].key + i].range(511, 480) = keyvalue57.value;
			#else 
			kvdram6[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[5].key + i].data[0] = keyvalue50;
			kvdram6[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[5].key + i].data[1] = keyvalue51;
			kvdram6[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[5].key + i].data[2] = keyvalue52;
			kvdram6[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[5].key + i].data[3] = keyvalue53;
			kvdram6[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[5].key + i].data[4] = keyvalue54;
			kvdram6[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[5].key + i].data[5] = keyvalue55;
			kvdram6[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[5].key + i].data[6] = keyvalue56;
			kvdram6[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[5].key + i].data[7] = keyvalue57;
			#endif 
			// read from 7
			keyvalue_t keyvalue70 = buffer7[0][i]; 
			keyvalue_t keyvalue71 = buffer7[1][i]; 
			keyvalue_t keyvalue72 = buffer7[2][i]; 
			keyvalue_t keyvalue73 = buffer7[3][i]; 
			keyvalue_t keyvalue74 = buffer7[4][i]; 
			keyvalue_t keyvalue75 = buffer7[5][i]; 
			keyvalue_t keyvalue76 = buffer7[6][i]; 
			keyvalue_t keyvalue77 = buffer7[7][i]; 
		
			
			// write to 7 dram
			#ifdef _WIDEWORD
			kvdram7[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[6].key + i].range(31, 0) = keyvalue60.key;
			kvdram7[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[6].key + i].range(63, 32) = keyvalue60.value;
			kvdram7[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[6].key + i].range(95, 64) = keyvalue61.key;
			kvdram7[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[6].key + i].range(127, 96) = keyvalue61.value;
			kvdram7[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[6].key + i].range(159, 128) = keyvalue62.key;
			kvdram7[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[6].key + i].range(191, 160) = keyvalue62.value;
			kvdram7[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[6].key + i].range(223, 192) = keyvalue63.key;
			kvdram7[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[6].key + i].range(255, 224) = keyvalue63.value;
			kvdram7[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[6].key + i].range(287, 256) = keyvalue64.key;
			kvdram7[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[6].key + i].range(319, 288) = keyvalue64.value;
			kvdram7[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[6].key + i].range(351, 320) = keyvalue65.key;
			kvdram7[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[6].key + i].range(383, 352) = keyvalue65.value;
			kvdram7[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[6].key + i].range(415, 384) = keyvalue66.key;
			kvdram7[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[6].key + i].range(447, 416) = keyvalue66.value;
			kvdram7[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[6].key + i].range(479, 448) = keyvalue67.key;
			kvdram7[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[6].key + i].range(511, 480) = keyvalue67.value;
			#else 
			kvdram7[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[6].key + i].data[0] = keyvalue60;
			kvdram7[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[6].key + i].data[1] = keyvalue61;
			kvdram7[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[6].key + i].data[2] = keyvalue62;
			kvdram7[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[6].key + i].data[3] = keyvalue63;
			kvdram7[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[6].key + i].data[4] = keyvalue64;
			kvdram7[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[6].key + i].data[5] = keyvalue65;
			kvdram7[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[6].key + i].data[6] = keyvalue66;
			kvdram7[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[6].key + i].data[7] = keyvalue67;
			#endif 
			// read from 8
			keyvalue_t keyvalue80 = buffer8[0][i]; 
			keyvalue_t keyvalue81 = buffer8[1][i]; 
			keyvalue_t keyvalue82 = buffer8[2][i]; 
			keyvalue_t keyvalue83 = buffer8[3][i]; 
			keyvalue_t keyvalue84 = buffer8[4][i]; 
			keyvalue_t keyvalue85 = buffer8[5][i]; 
			keyvalue_t keyvalue86 = buffer8[6][i]; 
			keyvalue_t keyvalue87 = buffer8[7][i]; 
		
			
			// write to 8 dram
			#ifdef _WIDEWORD
			kvdram8[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[7].key + i].range(31, 0) = keyvalue70.key;
			kvdram8[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[7].key + i].range(63, 32) = keyvalue70.value;
			kvdram8[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[7].key + i].range(95, 64) = keyvalue71.key;
			kvdram8[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[7].key + i].range(127, 96) = keyvalue71.value;
			kvdram8[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[7].key + i].range(159, 128) = keyvalue72.key;
			kvdram8[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[7].key + i].range(191, 160) = keyvalue72.value;
			kvdram8[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[7].key + i].range(223, 192) = keyvalue73.key;
			kvdram8[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[7].key + i].range(255, 224) = keyvalue73.value;
			kvdram8[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[7].key + i].range(287, 256) = keyvalue74.key;
			kvdram8[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[7].key + i].range(319, 288) = keyvalue74.value;
			kvdram8[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[7].key + i].range(351, 320) = keyvalue75.key;
			kvdram8[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[7].key + i].range(383, 352) = keyvalue75.value;
			kvdram8[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[7].key + i].range(415, 384) = keyvalue76.key;
			kvdram8[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[7].key + i].range(447, 416) = keyvalue76.value;
			kvdram8[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[7].key + i].range(479, 448) = keyvalue77.key;
			kvdram8[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[7].key + i].range(511, 480) = keyvalue77.value;
			#else 
			kvdram8[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[7].key + i].data[0] = keyvalue70;
			kvdram8[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[7].key + i].data[1] = keyvalue71;
			kvdram8[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[7].key + i].data[2] = keyvalue72;
			kvdram8[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[7].key + i].data[3] = keyvalue73;
			kvdram8[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[7].key + i].data[4] = keyvalue74;
			kvdram8[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[7].key + i].data[5] = keyvalue75;
			kvdram8[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[7].key + i].data[6] = keyvalue76;
			kvdram8[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[7].key + i].data[7] = keyvalue77;
			#endif 
			// read from 9
			keyvalue_t keyvalue90 = buffer9[0][i]; 
			keyvalue_t keyvalue91 = buffer9[1][i]; 
			keyvalue_t keyvalue92 = buffer9[2][i]; 
			keyvalue_t keyvalue93 = buffer9[3][i]; 
			keyvalue_t keyvalue94 = buffer9[4][i]; 
			keyvalue_t keyvalue95 = buffer9[5][i]; 
			keyvalue_t keyvalue96 = buffer9[6][i]; 
			keyvalue_t keyvalue97 = buffer9[7][i]; 
		
			
			// write to 9 dram
			#ifdef _WIDEWORD
			kvdram9[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[8].key + i].range(31, 0) = keyvalue80.key;
			kvdram9[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[8].key + i].range(63, 32) = keyvalue80.value;
			kvdram9[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[8].key + i].range(95, 64) = keyvalue81.key;
			kvdram9[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[8].key + i].range(127, 96) = keyvalue81.value;
			kvdram9[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[8].key + i].range(159, 128) = keyvalue82.key;
			kvdram9[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[8].key + i].range(191, 160) = keyvalue82.value;
			kvdram9[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[8].key + i].range(223, 192) = keyvalue83.key;
			kvdram9[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[8].key + i].range(255, 224) = keyvalue83.value;
			kvdram9[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[8].key + i].range(287, 256) = keyvalue84.key;
			kvdram9[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[8].key + i].range(319, 288) = keyvalue84.value;
			kvdram9[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[8].key + i].range(351, 320) = keyvalue85.key;
			kvdram9[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[8].key + i].range(383, 352) = keyvalue85.value;
			kvdram9[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[8].key + i].range(415, 384) = keyvalue86.key;
			kvdram9[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[8].key + i].range(447, 416) = keyvalue86.value;
			kvdram9[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[8].key + i].range(479, 448) = keyvalue87.key;
			kvdram9[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[8].key + i].range(511, 480) = keyvalue87.value;
			#else 
			kvdram9[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[8].key + i].data[0] = keyvalue80;
			kvdram9[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[8].key + i].data[1] = keyvalue81;
			kvdram9[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[8].key + i].data[2] = keyvalue82;
			kvdram9[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[8].key + i].data[3] = keyvalue83;
			kvdram9[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[8].key + i].data[4] = keyvalue84;
			kvdram9[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[8].key + i].data[5] = keyvalue85;
			kvdram9[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[8].key + i].data[6] = keyvalue86;
			kvdram9[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[8].key + i].data[7] = keyvalue87;
			#endif 
			// read from 10
			keyvalue_t keyvalue100 = buffer10[0][i]; 
			keyvalue_t keyvalue101 = buffer10[1][i]; 
			keyvalue_t keyvalue102 = buffer10[2][i]; 
			keyvalue_t keyvalue103 = buffer10[3][i]; 
			keyvalue_t keyvalue104 = buffer10[4][i]; 
			keyvalue_t keyvalue105 = buffer10[5][i]; 
			keyvalue_t keyvalue106 = buffer10[6][i]; 
			keyvalue_t keyvalue107 = buffer10[7][i]; 
		
			
			// write to 10 dram
			#ifdef _WIDEWORD
			kvdram10[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[9].key + i].range(31, 0) = keyvalue90.key;
			kvdram10[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[9].key + i].range(63, 32) = keyvalue90.value;
			kvdram10[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[9].key + i].range(95, 64) = keyvalue91.key;
			kvdram10[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[9].key + i].range(127, 96) = keyvalue91.value;
			kvdram10[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[9].key + i].range(159, 128) = keyvalue92.key;
			kvdram10[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[9].key + i].range(191, 160) = keyvalue92.value;
			kvdram10[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[9].key + i].range(223, 192) = keyvalue93.key;
			kvdram10[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[9].key + i].range(255, 224) = keyvalue93.value;
			kvdram10[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[9].key + i].range(287, 256) = keyvalue94.key;
			kvdram10[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[9].key + i].range(319, 288) = keyvalue94.value;
			kvdram10[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[9].key + i].range(351, 320) = keyvalue95.key;
			kvdram10[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[9].key + i].range(383, 352) = keyvalue95.value;
			kvdram10[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[9].key + i].range(415, 384) = keyvalue96.key;
			kvdram10[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[9].key + i].range(447, 416) = keyvalue96.value;
			kvdram10[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[9].key + i].range(479, 448) = keyvalue97.key;
			kvdram10[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[9].key + i].range(511, 480) = keyvalue97.value;
			#else 
			kvdram10[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[9].key + i].data[0] = keyvalue90;
			kvdram10[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[9].key + i].data[1] = keyvalue91;
			kvdram10[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[9].key + i].data[2] = keyvalue92;
			kvdram10[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[9].key + i].data[3] = keyvalue93;
			kvdram10[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[9].key + i].data[4] = keyvalue94;
			kvdram10[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[9].key + i].data[5] = keyvalue95;
			kvdram10[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[9].key + i].data[6] = keyvalue96;
			kvdram10[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[9].key + i].data[7] = keyvalue97;
			#endif 
			// read from 11
			keyvalue_t keyvalue110 = buffer11[0][i]; 
			keyvalue_t keyvalue111 = buffer11[1][i]; 
			keyvalue_t keyvalue112 = buffer11[2][i]; 
			keyvalue_t keyvalue113 = buffer11[3][i]; 
			keyvalue_t keyvalue114 = buffer11[4][i]; 
			keyvalue_t keyvalue115 = buffer11[5][i]; 
			keyvalue_t keyvalue116 = buffer11[6][i]; 
			keyvalue_t keyvalue117 = buffer11[7][i]; 
		
			
			// write to 11 dram
			#ifdef _WIDEWORD
			kvdram11[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[10].key + i].range(31, 0) = keyvalue100.key;
			kvdram11[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[10].key + i].range(63, 32) = keyvalue100.value;
			kvdram11[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[10].key + i].range(95, 64) = keyvalue101.key;
			kvdram11[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[10].key + i].range(127, 96) = keyvalue101.value;
			kvdram11[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[10].key + i].range(159, 128) = keyvalue102.key;
			kvdram11[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[10].key + i].range(191, 160) = keyvalue102.value;
			kvdram11[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[10].key + i].range(223, 192) = keyvalue103.key;
			kvdram11[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[10].key + i].range(255, 224) = keyvalue103.value;
			kvdram11[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[10].key + i].range(287, 256) = keyvalue104.key;
			kvdram11[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[10].key + i].range(319, 288) = keyvalue104.value;
			kvdram11[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[10].key + i].range(351, 320) = keyvalue105.key;
			kvdram11[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[10].key + i].range(383, 352) = keyvalue105.value;
			kvdram11[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[10].key + i].range(415, 384) = keyvalue106.key;
			kvdram11[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[10].key + i].range(447, 416) = keyvalue106.value;
			kvdram11[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[10].key + i].range(479, 448) = keyvalue107.key;
			kvdram11[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[10].key + i].range(511, 480) = keyvalue107.value;
			#else 
			kvdram11[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[10].key + i].data[0] = keyvalue100;
			kvdram11[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[10].key + i].data[1] = keyvalue101;
			kvdram11[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[10].key + i].data[2] = keyvalue102;
			kvdram11[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[10].key + i].data[3] = keyvalue103;
			kvdram11[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[10].key + i].data[4] = keyvalue104;
			kvdram11[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[10].key + i].data[5] = keyvalue105;
			kvdram11[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[10].key + i].data[6] = keyvalue106;
			kvdram11[globalparams.baseoffset_activevertices_kvs + offsetsandsizes[10].key + i].data[7] = keyvalue107;
			#endif 
	}
	
	SAVEKEYVALUESSYNC_LOOP2: for (buffer_type i=0; i<PADDEDDESTBUFFER_SIZE; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE II=1 // CRITICAL FIXME.

			// read from 0
			keyvalue_t keyvalue00 = buffer0[0][i]; 
			keyvalue_t keyvalue01 = buffer0[1][i]; 
			keyvalue_t keyvalue02 = buffer0[2][i]; 
			keyvalue_t keyvalue03 = buffer0[3][i]; 
			keyvalue_t keyvalue04 = buffer0[4][i]; 
			keyvalue_t keyvalue05 = buffer0[5][i]; 
			keyvalue_t keyvalue06 = buffer0[6][i]; 
			keyvalue_t keyvalue07 = buffer0[7][i]; 
		

			// read from 1
			keyvalue_t keyvalue10 = buffer1[0][i]; 
			keyvalue_t keyvalue11 = buffer1[1][i]; 
			keyvalue_t keyvalue12 = buffer1[2][i]; 
			keyvalue_t keyvalue13 = buffer1[3][i]; 
			keyvalue_t keyvalue14 = buffer1[4][i]; 
			keyvalue_t keyvalue15 = buffer1[5][i]; 
			keyvalue_t keyvalue16 = buffer1[6][i]; 
			keyvalue_t keyvalue17 = buffer1[7][i]; 
		
			// write to 2 buffer
			buffer1[0][i] = keyvalue00; 
			buffer1[1][i] = keyvalue01; 
			buffer1[2][i] = keyvalue02; 
			buffer1[3][i] = keyvalue03; 
			buffer1[4][i] = keyvalue04; 
			buffer1[5][i] = keyvalue05; 
			buffer1[6][i] = keyvalue06; 
			buffer1[7][i] = keyvalue07; 
			
			// read from 2
			keyvalue_t keyvalue20 = buffer2[0][i]; 
			keyvalue_t keyvalue21 = buffer2[1][i]; 
			keyvalue_t keyvalue22 = buffer2[2][i]; 
			keyvalue_t keyvalue23 = buffer2[3][i]; 
			keyvalue_t keyvalue24 = buffer2[4][i]; 
			keyvalue_t keyvalue25 = buffer2[5][i]; 
			keyvalue_t keyvalue26 = buffer2[6][i]; 
			keyvalue_t keyvalue27 = buffer2[7][i]; 
		
			// write to 3 buffer
			buffer2[0][i] = keyvalue10; 
			buffer2[1][i] = keyvalue11; 
			buffer2[2][i] = keyvalue12; 
			buffer2[3][i] = keyvalue13; 
			buffer2[4][i] = keyvalue14; 
			buffer2[5][i] = keyvalue15; 
			buffer2[6][i] = keyvalue16; 
			buffer2[7][i] = keyvalue17; 
			
			// read from 3
			keyvalue_t keyvalue30 = buffer3[0][i]; 
			keyvalue_t keyvalue31 = buffer3[1][i]; 
			keyvalue_t keyvalue32 = buffer3[2][i]; 
			keyvalue_t keyvalue33 = buffer3[3][i]; 
			keyvalue_t keyvalue34 = buffer3[4][i]; 
			keyvalue_t keyvalue35 = buffer3[5][i]; 
			keyvalue_t keyvalue36 = buffer3[6][i]; 
			keyvalue_t keyvalue37 = buffer3[7][i]; 
		
			// write to 4 buffer
			buffer3[0][i] = keyvalue20; 
			buffer3[1][i] = keyvalue21; 
			buffer3[2][i] = keyvalue22; 
			buffer3[3][i] = keyvalue23; 
			buffer3[4][i] = keyvalue24; 
			buffer3[5][i] = keyvalue25; 
			buffer3[6][i] = keyvalue26; 
			buffer3[7][i] = keyvalue27; 
			
			// read from 4
			keyvalue_t keyvalue40 = buffer4[0][i]; 
			keyvalue_t keyvalue41 = buffer4[1][i]; 
			keyvalue_t keyvalue42 = buffer4[2][i]; 
			keyvalue_t keyvalue43 = buffer4[3][i]; 
			keyvalue_t keyvalue44 = buffer4[4][i]; 
			keyvalue_t keyvalue45 = buffer4[5][i]; 
			keyvalue_t keyvalue46 = buffer4[6][i]; 
			keyvalue_t keyvalue47 = buffer4[7][i]; 
		
			// write to 5 buffer
			buffer4[0][i] = keyvalue30; 
			buffer4[1][i] = keyvalue31; 
			buffer4[2][i] = keyvalue32; 
			buffer4[3][i] = keyvalue33; 
			buffer4[4][i] = keyvalue34; 
			buffer4[5][i] = keyvalue35; 
			buffer4[6][i] = keyvalue36; 
			buffer4[7][i] = keyvalue37; 
			
			// read from 5
			keyvalue_t keyvalue50 = buffer5[0][i]; 
			keyvalue_t keyvalue51 = buffer5[1][i]; 
			keyvalue_t keyvalue52 = buffer5[2][i]; 
			keyvalue_t keyvalue53 = buffer5[3][i]; 
			keyvalue_t keyvalue54 = buffer5[4][i]; 
			keyvalue_t keyvalue55 = buffer5[5][i]; 
			keyvalue_t keyvalue56 = buffer5[6][i]; 
			keyvalue_t keyvalue57 = buffer5[7][i]; 
		
			// write to 6 buffer
			buffer5[0][i] = keyvalue40; 
			buffer5[1][i] = keyvalue41; 
			buffer5[2][i] = keyvalue42; 
			buffer5[3][i] = keyvalue43; 
			buffer5[4][i] = keyvalue44; 
			buffer5[5][i] = keyvalue45; 
			buffer5[6][i] = keyvalue46; 
			buffer5[7][i] = keyvalue47; 
			
			// read from 6
			keyvalue_t keyvalue60 = buffer6[0][i]; 
			keyvalue_t keyvalue61 = buffer6[1][i]; 
			keyvalue_t keyvalue62 = buffer6[2][i]; 
			keyvalue_t keyvalue63 = buffer6[3][i]; 
			keyvalue_t keyvalue64 = buffer6[4][i]; 
			keyvalue_t keyvalue65 = buffer6[5][i]; 
			keyvalue_t keyvalue66 = buffer6[6][i]; 
			keyvalue_t keyvalue67 = buffer6[7][i]; 
		
			// write to 7 buffer
			buffer6[0][i] = keyvalue50; 
			buffer6[1][i] = keyvalue51; 
			buffer6[2][i] = keyvalue52; 
			buffer6[3][i] = keyvalue53; 
			buffer6[4][i] = keyvalue54; 
			buffer6[5][i] = keyvalue55; 
			buffer6[6][i] = keyvalue56; 
			buffer6[7][i] = keyvalue57; 
			
			// read from 7
			keyvalue_t keyvalue70 = buffer7[0][i]; 
			keyvalue_t keyvalue71 = buffer7[1][i]; 
			keyvalue_t keyvalue72 = buffer7[2][i]; 
			keyvalue_t keyvalue73 = buffer7[3][i]; 
			keyvalue_t keyvalue74 = buffer7[4][i]; 
			keyvalue_t keyvalue75 = buffer7[5][i]; 
			keyvalue_t keyvalue76 = buffer7[6][i]; 
			keyvalue_t keyvalue77 = buffer7[7][i]; 
		
			// write to 8 buffer
			buffer7[0][i] = keyvalue60; 
			buffer7[1][i] = keyvalue61; 
			buffer7[2][i] = keyvalue62; 
			buffer7[3][i] = keyvalue63; 
			buffer7[4][i] = keyvalue64; 
			buffer7[5][i] = keyvalue65; 
			buffer7[6][i] = keyvalue66; 
			buffer7[7][i] = keyvalue67; 
			
			// read from 8
			keyvalue_t keyvalue80 = buffer8[0][i]; 
			keyvalue_t keyvalue81 = buffer8[1][i]; 
			keyvalue_t keyvalue82 = buffer8[2][i]; 
			keyvalue_t keyvalue83 = buffer8[3][i]; 
			keyvalue_t keyvalue84 = buffer8[4][i]; 
			keyvalue_t keyvalue85 = buffer8[5][i]; 
			keyvalue_t keyvalue86 = buffer8[6][i]; 
			keyvalue_t keyvalue87 = buffer8[7][i]; 
		
			// write to 9 buffer
			buffer8[0][i] = keyvalue70; 
			buffer8[1][i] = keyvalue71; 
			buffer8[2][i] = keyvalue72; 
			buffer8[3][i] = keyvalue73; 
			buffer8[4][i] = keyvalue74; 
			buffer8[5][i] = keyvalue75; 
			buffer8[6][i] = keyvalue76; 
			buffer8[7][i] = keyvalue77; 
			
			// read from 9
			keyvalue_t keyvalue90 = buffer9[0][i]; 
			keyvalue_t keyvalue91 = buffer9[1][i]; 
			keyvalue_t keyvalue92 = buffer9[2][i]; 
			keyvalue_t keyvalue93 = buffer9[3][i]; 
			keyvalue_t keyvalue94 = buffer9[4][i]; 
			keyvalue_t keyvalue95 = buffer9[5][i]; 
			keyvalue_t keyvalue96 = buffer9[6][i]; 
			keyvalue_t keyvalue97 = buffer9[7][i]; 
		
			// write to 10 buffer
			buffer9[0][i] = keyvalue80; 
			buffer9[1][i] = keyvalue81; 
			buffer9[2][i] = keyvalue82; 
			buffer9[3][i] = keyvalue83; 
			buffer9[4][i] = keyvalue84; 
			buffer9[5][i] = keyvalue85; 
			buffer9[6][i] = keyvalue86; 
			buffer9[7][i] = keyvalue87; 
			
			// read from 10
			keyvalue_t keyvalue100 = buffer10[0][i]; 
			keyvalue_t keyvalue101 = buffer10[1][i]; 
			keyvalue_t keyvalue102 = buffer10[2][i]; 
			keyvalue_t keyvalue103 = buffer10[3][i]; 
			keyvalue_t keyvalue104 = buffer10[4][i]; 
			keyvalue_t keyvalue105 = buffer10[5][i]; 
			keyvalue_t keyvalue106 = buffer10[6][i]; 
			keyvalue_t keyvalue107 = buffer10[7][i]; 
		
			// write to 11 buffer
			buffer10[0][i] = keyvalue90; 
			buffer10[1][i] = keyvalue91; 
			buffer10[2][i] = keyvalue92; 
			buffer10[3][i] = keyvalue93; 
			buffer10[4][i] = keyvalue94; 
			buffer10[5][i] = keyvalue95; 
			buffer10[6][i] = keyvalue96; 
			buffer10[7][i] = keyvalue97; 
			
			// read from 11
			keyvalue_t keyvalue110 = buffer11[0][i]; 
			keyvalue_t keyvalue111 = buffer11[1][i]; 
			keyvalue_t keyvalue112 = buffer11[2][i]; 
			keyvalue_t keyvalue113 = buffer11[3][i]; 
			keyvalue_t keyvalue114 = buffer11[4][i]; 
			keyvalue_t keyvalue115 = buffer11[5][i]; 
			keyvalue_t keyvalue116 = buffer11[6][i]; 
			keyvalue_t keyvalue117 = buffer11[7][i]; 
		
			// write to 12 buffer
			buffer11[0][i] = keyvalue100; 
			buffer11[1][i] = keyvalue101; 
			buffer11[2][i] = keyvalue102; 
			buffer11[3][i] = keyvalue103; 
			buffer11[4][i] = keyvalue104; 
			buffer11[5][i] = keyvalue105; 
			buffer11[6][i] = keyvalue106; 
			buffer11[7][i] = keyvalue107; 
			
	}
	
	// shift offsets and sizes
	for(unsigned int i=NUMCOMPUTEUNITS-1; i>0; i--){ 
	#pragma HLS PIPELINE II=1
		offsetsandsizes[i].key = offsetsandsizes[i-1].key; 
		offsetsandsizes[i].value = offsetsandsizes[i-1].value; 
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS2
	cout<< TIMINGRESULTSCOLOR<<"savekeyvalues_pipeline_sync:: keyvalues saved: offset: "<<offset_kvs * VECTOR_SIZE<<"-"<<(offset_kvs + size_kvs) * VECTOR_SIZE<<", num keyvalues written: "<<size_kvs * VECTOR_SIZE<< RESET<<endl;
	#endif
	return;
}

void 
	#ifdef SW 
	acts::
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
		
		for(vector_type v=0; v<VECTOR_SIZE; v++){
		#pragma HLS UNROLL
			destbuffer[v][i] = keyvalue; // load to multiple destinations
		}
	}
	return;
}

void 
	#ifdef SW 
	acts::
	#endif 
replicatedata_syn(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer0[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer3[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer5[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer6[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer7[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer8[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer9[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer10[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer11[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], buffer_type sourceoffset, buffer_type size){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount = PADDEDDESTBUFFER_SIZE;
	// cout<<"replicatedata_syn:: sourceoffset: "<<sourceoffset<<", size: "<<size<<endl;
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("replicatedata_syn.0", sourceoffset + size, (PADDEDDESTBUFFER_SIZE * VECTOR_SIZE) + 1, sourceoffset, size, PADDEDDESTBUFFER_SIZE);
	#endif

	vector_type src_v = 0;
	buffer_type src_i = 0;
	buffer_type sourceoffset_kvs = sourceoffset / VECTOR_SIZE;
	REPLICATEDATA_LOOP: for(buffer_type i=0; i<size; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("replicatedata_syn.1", i, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, sourceoffset, sourceoffset_kvs, size);
		actsutilityobj->checkoutofbounds("replicatedata_syn.2", sourceoffset + src_i, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, sourceoffset, sourceoffset_kvs, size);
		#endif
		
		keyvalue_t keyvalue = sourcebuffer[src_v][sourceoffset_kvs + src_i];
		src_v++; if(src_v==VECTOR_SIZE){ src_v=0; src_i+=1; }
		
		for(vector_type v=0; v<VECTOR_SIZE; v++){
		#pragma HLS UNROLL
		
			destbuffer0[v][i] = keyvalue; 
			destbuffer1[v][i] = keyvalue; 
			destbuffer2[v][i] = keyvalue; 
			destbuffer3[v][i] = keyvalue; 
			destbuffer4[v][i] = keyvalue; 
			destbuffer5[v][i] = keyvalue; 
			destbuffer6[v][i] = keyvalue; 
			destbuffer7[v][i] = keyvalue; 
			destbuffer8[v][i] = keyvalue; 
			destbuffer9[v][i] = keyvalue; 
			destbuffer10[v][i] = keyvalue; 
			destbuffer11[v][i] = keyvalue; 
		}
	}
	return;
}

value_t 
	#ifdef SW 
	acts::
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
}

value_t 
	#ifdef SW 
	acts::
	#endif 
reducefunc_bfs(value_t vtemp, value_t res, unsigned int GraphIter, unsigned int GraphAlgo){
	value_t temp = 0;
	
	if(vtemp == UNVISITED){ temp = VISITED_IN_CURRENT_ITERATION; } 
	else if(vtemp == VISITED_IN_CURRENT_ITERATION){ temp = VISITED_IN_CURRENT_ITERATION; } // FIXME. CAUSEOFERROR?
	else if(vtemp == VISITED_IN_PAST_ITERATION){ temp = VISITED_IN_PAST_ITERATION; } 
	else {
		#ifdef _DEBUGMODE_CHECKS2
		#ifdef ENABLE_VOICEOUTKERNELERRORS
		cout<<"reducefunc_bfs: ERROR. should never get here. exiting..."<<endl;
		cout<<"reducefunc_bfs: vtemp: "<<vtemp<<", UNVISITED: "<<UNVISITED<<", VISITED_IN_CURRENT_ITERATION: "<<VISITED_IN_CURRENT_ITERATION<<", VISITED_IN_PAST_ITERATION: "<<VISITED_IN_PAST_ITERATION<<endl;
		#endif 
		#ifdef ENABLE_PERFECTACCURACY
		exit(EXIT_FAILURE);
		#endif 
		#endif 
	}
	return temp;
}

void 
	#ifdef SW 
	acts::
	#endif
reduce(bool_type enable1, bool_type enable2, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], sweepparams_t sweepparams, unsigned int GraphIter, unsigned int GraphAlgo, travstate_t travstate, globalparams_t globalparams){
	if(enable1 == OFF || enable2 == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE;
	buffer_type chunk_size = getchunksize_kvs(SRCBUFFER_SIZE, travstate, 0);
	unsigned int upperlimit = sweepparams.upperlimit;
	unsigned int upperpartition = sweepparams.upperpartition;
	
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
		if(keyvalue0.key != INVALIDDATA && keyvalue0.value != INVALIDDATA){ cout<<"REDUCE SEEN @ reduce:: i: "<<i<<", loc0: "<<loc0<<", keyvalue0.key: "<<keyvalue0.key<<", keyvalue0.value: "<<keyvalue0.value<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; }
		if(keyvalue1.key != INVALIDDATA && keyvalue1.value != INVALIDDATA){ cout<<"REDUCE SEEN @ reduce:: i: "<<i<<", loc1: "<<loc1<<", keyvalue1.key: "<<keyvalue1.key<<", keyvalue1.value: "<<keyvalue1.value<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; }
		if(keyvalue2.key != INVALIDDATA && keyvalue2.value != INVALIDDATA){ cout<<"REDUCE SEEN @ reduce:: i: "<<i<<", loc2: "<<loc2<<", keyvalue2.key: "<<keyvalue2.key<<", keyvalue2.value: "<<keyvalue2.value<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; }
		if(keyvalue3.key != INVALIDDATA && keyvalue3.value != INVALIDDATA){ cout<<"REDUCE SEEN @ reduce:: i: "<<i<<", loc3: "<<loc3<<", keyvalue3.key: "<<keyvalue3.key<<", keyvalue3.value: "<<keyvalue3.value<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; }
		if(keyvalue4.key != INVALIDDATA && keyvalue4.value != INVALIDDATA){ cout<<"REDUCE SEEN @ reduce:: i: "<<i<<", loc4: "<<loc4<<", keyvalue4.key: "<<keyvalue4.key<<", keyvalue4.value: "<<keyvalue4.value<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; }
		if(keyvalue5.key != INVALIDDATA && keyvalue5.value != INVALIDDATA){ cout<<"REDUCE SEEN @ reduce:: i: "<<i<<", loc5: "<<loc5<<", keyvalue5.key: "<<keyvalue5.key<<", keyvalue5.value: "<<keyvalue5.value<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; }
		if(keyvalue6.key != INVALIDDATA && keyvalue6.value != INVALIDDATA){ cout<<"REDUCE SEEN @ reduce:: i: "<<i<<", loc6: "<<loc6<<", keyvalue6.key: "<<keyvalue6.key<<", keyvalue6.value: "<<keyvalue6.value<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; }
		if(keyvalue7.key != INVALIDDATA && keyvalue7.value != INVALIDDATA){ cout<<"REDUCE SEEN @ reduce:: i: "<<i<<", loc7: "<<loc7<<", keyvalue7.key: "<<keyvalue7.key<<", keyvalue7.value: "<<keyvalue7.value<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; }
		#endif 
		
		if(loc0 >= globalparams.applyvertexbuffersz && keyvalue0.key != INVALIDDATA && keyvalue0.value != INVALIDDATA){
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
		if(loc1 >= globalparams.applyvertexbuffersz && keyvalue1.key != INVALIDDATA && keyvalue1.value != INVALIDDATA){
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
		if(loc2 >= globalparams.applyvertexbuffersz && keyvalue2.key != INVALIDDATA && keyvalue2.value != INVALIDDATA){
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
		if(loc3 >= globalparams.applyvertexbuffersz && keyvalue3.key != INVALIDDATA && keyvalue3.value != INVALIDDATA){
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
		if(loc4 >= globalparams.applyvertexbuffersz && keyvalue4.key != INVALIDDATA && keyvalue4.value != INVALIDDATA){
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
		if(loc5 >= globalparams.applyvertexbuffersz && keyvalue5.key != INVALIDDATA && keyvalue5.value != INVALIDDATA){
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
		if(loc6 >= globalparams.applyvertexbuffersz && keyvalue6.key != INVALIDDATA && keyvalue6.value != INVALIDDATA){
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
		if(loc7 >= globalparams.applyvertexbuffersz && keyvalue7.key != INVALIDDATA && keyvalue7.value != INVALIDDATA){
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
		if(keyvalue0.key != INVALIDDATA && keyvalue0.value != INVALIDDATA){ vprop0 = destbuffer[0][rowindex0]; }
		keyvalue_t vprop1;
		if(keyvalue1.key != INVALIDDATA && keyvalue1.value != INVALIDDATA){ vprop1 = destbuffer[1][rowindex1]; }
		keyvalue_t vprop2;
		if(keyvalue2.key != INVALIDDATA && keyvalue2.value != INVALIDDATA){ vprop2 = destbuffer[2][rowindex2]; }
		keyvalue_t vprop3;
		if(keyvalue3.key != INVALIDDATA && keyvalue3.value != INVALIDDATA){ vprop3 = destbuffer[3][rowindex3]; }
		keyvalue_t vprop4;
		if(keyvalue4.key != INVALIDDATA && keyvalue4.value != INVALIDDATA){ vprop4 = destbuffer[4][rowindex4]; }
		keyvalue_t vprop5;
		if(keyvalue5.key != INVALIDDATA && keyvalue5.value != INVALIDDATA){ vprop5 = destbuffer[5][rowindex5]; }
		keyvalue_t vprop6;
		if(keyvalue6.key != INVALIDDATA && keyvalue6.value != INVALIDDATA){ vprop6 = destbuffer[6][rowindex6]; }
		keyvalue_t vprop7;
		if(keyvalue7.key != INVALIDDATA && keyvalue7.value != INVALIDDATA){ vprop7 = destbuffer[7][rowindex7]; }
		
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
		if(keyvalue0.key != INVALIDDATA && keyvalue0.value != INVALIDDATA){ cout<<"REDUCEFUNC RESULT @ reduce:: rettemp0: "<<rettemp0<<endl; }
		if(keyvalue1.key != INVALIDDATA && keyvalue1.value != INVALIDDATA){ cout<<"REDUCEFUNC RESULT @ reduce:: rettemp1: "<<rettemp1<<endl; }
		if(keyvalue2.key != INVALIDDATA && keyvalue2.value != INVALIDDATA){ cout<<"REDUCEFUNC RESULT @ reduce:: rettemp2: "<<rettemp2<<endl; }
		if(keyvalue3.key != INVALIDDATA && keyvalue3.value != INVALIDDATA){ cout<<"REDUCEFUNC RESULT @ reduce:: rettemp3: "<<rettemp3<<endl; }
		if(keyvalue4.key != INVALIDDATA && keyvalue4.value != INVALIDDATA){ cout<<"REDUCEFUNC RESULT @ reduce:: rettemp4: "<<rettemp4<<endl; }
		if(keyvalue5.key != INVALIDDATA && keyvalue5.value != INVALIDDATA){ cout<<"REDUCEFUNC RESULT @ reduce:: rettemp5: "<<rettemp5<<endl; }
		if(keyvalue6.key != INVALIDDATA && keyvalue6.value != INVALIDDATA){ cout<<"REDUCEFUNC RESULT @ reduce:: rettemp6: "<<rettemp6<<endl; }
		if(keyvalue7.key != INVALIDDATA && keyvalue7.value != INVALIDDATA){ cout<<"REDUCEFUNC RESULT @ reduce:: rettemp7: "<<rettemp7<<endl; }
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
		
		if(keyvalue0.key != INVALIDDATA && keyvalue0.value != INVALIDDATA){ destbuffer[0][rowindex0] = vprop0; }
		if(keyvalue1.key != INVALIDDATA && keyvalue1.value != INVALIDDATA){ destbuffer[1][rowindex1] = vprop1; }
		if(keyvalue2.key != INVALIDDATA && keyvalue2.value != INVALIDDATA){ destbuffer[2][rowindex2] = vprop2; }
		if(keyvalue3.key != INVALIDDATA && keyvalue3.value != INVALIDDATA){ destbuffer[3][rowindex3] = vprop3; }
		if(keyvalue4.key != INVALIDDATA && keyvalue4.value != INVALIDDATA){ destbuffer[4][rowindex4] = vprop4; }
		if(keyvalue5.key != INVALIDDATA && keyvalue5.value != INVALIDDATA){ destbuffer[5][rowindex5] = vprop5; }
		if(keyvalue6.key != INVALIDDATA && keyvalue6.value != INVALIDDATA){ destbuffer[6][rowindex6] = vprop6; }
		if(keyvalue7.key != INVALIDDATA && keyvalue7.value != INVALIDDATA){ destbuffer[7][rowindex7] = vprop7; }
		
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsreduced(VECTOR_SIZE);
		if(keyvalue0.key != INVALIDDATA && keyvalue0.value != INVALIDDATA){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(1); }
		if(keyvalue1.key != INVALIDDATA && keyvalue1.value != INVALIDDATA){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(1); }
		if(keyvalue2.key != INVALIDDATA && keyvalue2.value != INVALIDDATA){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(1); }
		if(keyvalue3.key != INVALIDDATA && keyvalue3.value != INVALIDDATA){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(1); }
		if(keyvalue4.key != INVALIDDATA && keyvalue4.value != INVALIDDATA){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(1); }
		if(keyvalue5.key != INVALIDDATA && keyvalue5.value != INVALIDDATA){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(1); }
		if(keyvalue6.key != INVALIDDATA && keyvalue6.value != INVALIDDATA){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(1); }
		if(keyvalue7.key != INVALIDDATA && keyvalue7.value != INVALIDDATA){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(1); }
		#endif
	}
	return;
}

void 
	#ifdef SW 
	acts::
	#endif
reduce_bfs(bool_type enable1, bool_type enable2, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], sweepparams_t sweepparams, unsigned int GraphIter, unsigned int GraphAlgo, travstate_t travstate, globalparams_t globalparams){
	if(enable1 == OFF || enable2 == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE;
	buffer_type chunk_size = getchunksize_kvs(SRCBUFFER_SIZE, travstate, 0);
	
	vertex_t locs0 = 0;
	vertex_t rowindexs0 = 0;
	vertex_t colindexs0 = 0;
	value_t temps0 = 0;
	value_t rettemps0 = 0;
	keyvalue_t vprop0; vprop0.key = 0; vprop0.value = 0;
	bool_type en0 = ON;
	bool_type en20 = ON;
	vertex_t locs1 = 0;
	vertex_t rowindexs1 = 0;
	vertex_t colindexs1 = 0;
	value_t temps1 = 0;
	value_t rettemps1 = 0;
	keyvalue_t vprop1; vprop1.key = 0; vprop1.value = 0;
	bool_type en1 = ON;
	bool_type en21 = ON;
	vertex_t locs2 = 0;
	vertex_t rowindexs2 = 0;
	vertex_t colindexs2 = 0;
	value_t temps2 = 0;
	value_t rettemps2 = 0;
	keyvalue_t vprop2; vprop2.key = 0; vprop2.value = 0;
	bool_type en2 = ON;
	bool_type en22 = ON;
	vertex_t locs3 = 0;
	vertex_t rowindexs3 = 0;
	vertex_t colindexs3 = 0;
	value_t temps3 = 0;
	value_t rettemps3 = 0;
	keyvalue_t vprop3; vprop3.key = 0; vprop3.value = 0;
	bool_type en3 = ON;
	bool_type en23 = ON;
	vertex_t locs4 = 0;
	vertex_t rowindexs4 = 0;
	vertex_t colindexs4 = 0;
	value_t temps4 = 0;
	value_t rettemps4 = 0;
	keyvalue_t vprop4; vprop4.key = 0; vprop4.value = 0;
	bool_type en4 = ON;
	bool_type en24 = ON;
	vertex_t locs5 = 0;
	vertex_t rowindexs5 = 0;
	vertex_t colindexs5 = 0;
	value_t temps5 = 0;
	value_t rettemps5 = 0;
	keyvalue_t vprop5; vprop5.key = 0; vprop5.value = 0;
	bool_type en5 = ON;
	bool_type en25 = ON;
	vertex_t locs6 = 0;
	vertex_t rowindexs6 = 0;
	vertex_t colindexs6 = 0;
	value_t temps6 = 0;
	value_t rettemps6 = 0;
	keyvalue_t vprop6; vprop6.key = 0; vprop6.value = 0;
	bool_type en6 = ON;
	bool_type en26 = ON;
	vertex_t locs7 = 0;
	vertex_t rowindexs7 = 0;
	vertex_t colindexs7 = 0;
	value_t temps7 = 0;
	value_t rettemps7 = 0;
	keyvalue_t vprop7; vprop7.key = 0; vprop7.value = 0;
	bool_type en7 = ON;
	bool_type en27 = ON;
	
	unsigned int upperlimit = sweepparams.upperlimit;
	unsigned int upperpartition = sweepparams.upperpartition;

	REDUCE_LOOP: for(buffer_type i=0; i<chunk_size; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz
	
		keyvalue_t keyvalue0 = sourcebuffer[0][i];
		keyvalue_t keyvalue1 = sourcebuffer[1][i];
		keyvalue_t keyvalue2 = sourcebuffer[2][i];
		keyvalue_t keyvalue3 = sourcebuffer[3][i];
		keyvalue_t keyvalue4 = sourcebuffer[4][i];
		keyvalue_t keyvalue5 = sourcebuffer[5][i];
		keyvalue_t keyvalue6 = sourcebuffer[6][i];
		keyvalue_t keyvalue7 = sourcebuffer[7][i];
		
		if(keyvalue0.key != INVALIDDATA && keyvalue0.value != INVALIDDATA){ en0 = ON; } else { en0 = OFF; }
		if(keyvalue1.key != INVALIDDATA && keyvalue1.value != INVALIDDATA){ en1 = ON; } else { en1 = OFF; }
		if(keyvalue2.key != INVALIDDATA && keyvalue2.value != INVALIDDATA){ en2 = ON; } else { en2 = OFF; }
		if(keyvalue3.key != INVALIDDATA && keyvalue3.value != INVALIDDATA){ en3 = ON; } else { en3 = OFF; }
		if(keyvalue4.key != INVALIDDATA && keyvalue4.value != INVALIDDATA){ en4 = ON; } else { en4 = OFF; }
		if(keyvalue5.key != INVALIDDATA && keyvalue5.value != INVALIDDATA){ en5 = ON; } else { en5 = OFF; }
		if(keyvalue6.key != INVALIDDATA && keyvalue6.value != INVALIDDATA){ en6 = ON; } else { en6 = OFF; }
		if(keyvalue7.key != INVALIDDATA && keyvalue7.value != INVALIDDATA){ en7 = ON; } else { en7 = OFF; }
		
		#ifdef _WIDEWORD
		ulong_dt longdata0 = CONVERTTOLONG_KV(keyvalue0);
		unsigned int streetaddr0 = longdata0.range(COMPACTPARAM_STARTOFFSET_STREETADDR + COMPACTPARAM_BITSIZE_STREETADDR - 1, COMPACTPARAM_STARTOFFSET_STREETADDR);
		unsigned int numvalidkeys0 = longdata0.range(COMPACTPARAM_STARTOFFSET_NUMITEMS + COMPACTPARAM_BITSIZE_NUMITEMS - 1, COMPACTPARAM_STARTOFFSET_NUMITEMS);
		ulong_dt longdata1 = CONVERTTOLONG_KV(keyvalue1);
		unsigned int streetaddr1 = longdata1.range(COMPACTPARAM_STARTOFFSET_STREETADDR + COMPACTPARAM_BITSIZE_STREETADDR - 1, COMPACTPARAM_STARTOFFSET_STREETADDR);
		unsigned int numvalidkeys1 = longdata1.range(COMPACTPARAM_STARTOFFSET_NUMITEMS + COMPACTPARAM_BITSIZE_NUMITEMS - 1, COMPACTPARAM_STARTOFFSET_NUMITEMS);
		ulong_dt longdata2 = CONVERTTOLONG_KV(keyvalue2);
		unsigned int streetaddr2 = longdata2.range(COMPACTPARAM_STARTOFFSET_STREETADDR + COMPACTPARAM_BITSIZE_STREETADDR - 1, COMPACTPARAM_STARTOFFSET_STREETADDR);
		unsigned int numvalidkeys2 = longdata2.range(COMPACTPARAM_STARTOFFSET_NUMITEMS + COMPACTPARAM_BITSIZE_NUMITEMS - 1, COMPACTPARAM_STARTOFFSET_NUMITEMS);
		ulong_dt longdata3 = CONVERTTOLONG_KV(keyvalue3);
		unsigned int streetaddr3 = longdata3.range(COMPACTPARAM_STARTOFFSET_STREETADDR + COMPACTPARAM_BITSIZE_STREETADDR - 1, COMPACTPARAM_STARTOFFSET_STREETADDR);
		unsigned int numvalidkeys3 = longdata3.range(COMPACTPARAM_STARTOFFSET_NUMITEMS + COMPACTPARAM_BITSIZE_NUMITEMS - 1, COMPACTPARAM_STARTOFFSET_NUMITEMS);
		ulong_dt longdata4 = CONVERTTOLONG_KV(keyvalue4);
		unsigned int streetaddr4 = longdata4.range(COMPACTPARAM_STARTOFFSET_STREETADDR + COMPACTPARAM_BITSIZE_STREETADDR - 1, COMPACTPARAM_STARTOFFSET_STREETADDR);
		unsigned int numvalidkeys4 = longdata4.range(COMPACTPARAM_STARTOFFSET_NUMITEMS + COMPACTPARAM_BITSIZE_NUMITEMS - 1, COMPACTPARAM_STARTOFFSET_NUMITEMS);
		ulong_dt longdata5 = CONVERTTOLONG_KV(keyvalue5);
		unsigned int streetaddr5 = longdata5.range(COMPACTPARAM_STARTOFFSET_STREETADDR + COMPACTPARAM_BITSIZE_STREETADDR - 1, COMPACTPARAM_STARTOFFSET_STREETADDR);
		unsigned int numvalidkeys5 = longdata5.range(COMPACTPARAM_STARTOFFSET_NUMITEMS + COMPACTPARAM_BITSIZE_NUMITEMS - 1, COMPACTPARAM_STARTOFFSET_NUMITEMS);
		ulong_dt longdata6 = CONVERTTOLONG_KV(keyvalue6);
		unsigned int streetaddr6 = longdata6.range(COMPACTPARAM_STARTOFFSET_STREETADDR + COMPACTPARAM_BITSIZE_STREETADDR - 1, COMPACTPARAM_STARTOFFSET_STREETADDR);
		unsigned int numvalidkeys6 = longdata6.range(COMPACTPARAM_STARTOFFSET_NUMITEMS + COMPACTPARAM_BITSIZE_NUMITEMS - 1, COMPACTPARAM_STARTOFFSET_NUMITEMS);
		ulong_dt longdata7 = CONVERTTOLONG_KV(keyvalue7);
		unsigned int streetaddr7 = longdata7.range(COMPACTPARAM_STARTOFFSET_STREETADDR + COMPACTPARAM_BITSIZE_STREETADDR - 1, COMPACTPARAM_STARTOFFSET_STREETADDR);
		unsigned int numvalidkeys7 = longdata7.range(COMPACTPARAM_STARTOFFSET_NUMITEMS + COMPACTPARAM_BITSIZE_NUMITEMS - 1, COMPACTPARAM_STARTOFFSET_NUMITEMS);
		#else
		ulong_dt * longdata0 = (ulong_dt *)&keyvalue0;
		unsigned int streetaddr0 = READFROM_ULONG(*longdata0, COMPACTPARAM_STARTOFFSET_STREETADDR, COMPACTPARAM_BITSIZE_STREETADDR);
		unsigned int numvalidkeys0 = READFROM_ULONG(*longdata0, COMPACTPARAM_STARTOFFSET_NUMITEMS, COMPACTPARAM_BITSIZE_NUMITEMS);
		ulong_dt * longdata1 = (ulong_dt *)&keyvalue1;
		unsigned int streetaddr1 = READFROM_ULONG(*longdata1, COMPACTPARAM_STARTOFFSET_STREETADDR, COMPACTPARAM_BITSIZE_STREETADDR);
		unsigned int numvalidkeys1 = READFROM_ULONG(*longdata1, COMPACTPARAM_STARTOFFSET_NUMITEMS, COMPACTPARAM_BITSIZE_NUMITEMS);
		ulong_dt * longdata2 = (ulong_dt *)&keyvalue2;
		unsigned int streetaddr2 = READFROM_ULONG(*longdata2, COMPACTPARAM_STARTOFFSET_STREETADDR, COMPACTPARAM_BITSIZE_STREETADDR);
		unsigned int numvalidkeys2 = READFROM_ULONG(*longdata2, COMPACTPARAM_STARTOFFSET_NUMITEMS, COMPACTPARAM_BITSIZE_NUMITEMS);
		ulong_dt * longdata3 = (ulong_dt *)&keyvalue3;
		unsigned int streetaddr3 = READFROM_ULONG(*longdata3, COMPACTPARAM_STARTOFFSET_STREETADDR, COMPACTPARAM_BITSIZE_STREETADDR);
		unsigned int numvalidkeys3 = READFROM_ULONG(*longdata3, COMPACTPARAM_STARTOFFSET_NUMITEMS, COMPACTPARAM_BITSIZE_NUMITEMS);
		ulong_dt * longdata4 = (ulong_dt *)&keyvalue4;
		unsigned int streetaddr4 = READFROM_ULONG(*longdata4, COMPACTPARAM_STARTOFFSET_STREETADDR, COMPACTPARAM_BITSIZE_STREETADDR);
		unsigned int numvalidkeys4 = READFROM_ULONG(*longdata4, COMPACTPARAM_STARTOFFSET_NUMITEMS, COMPACTPARAM_BITSIZE_NUMITEMS);
		ulong_dt * longdata5 = (ulong_dt *)&keyvalue5;
		unsigned int streetaddr5 = READFROM_ULONG(*longdata5, COMPACTPARAM_STARTOFFSET_STREETADDR, COMPACTPARAM_BITSIZE_STREETADDR);
		unsigned int numvalidkeys5 = READFROM_ULONG(*longdata5, COMPACTPARAM_STARTOFFSET_NUMITEMS, COMPACTPARAM_BITSIZE_NUMITEMS);
		ulong_dt * longdata6 = (ulong_dt *)&keyvalue6;
		unsigned int streetaddr6 = READFROM_ULONG(*longdata6, COMPACTPARAM_STARTOFFSET_STREETADDR, COMPACTPARAM_BITSIZE_STREETADDR);
		unsigned int numvalidkeys6 = READFROM_ULONG(*longdata6, COMPACTPARAM_STARTOFFSET_NUMITEMS, COMPACTPARAM_BITSIZE_NUMITEMS);
		ulong_dt * longdata7 = (ulong_dt *)&keyvalue7;
		unsigned int streetaddr7 = READFROM_ULONG(*longdata7, COMPACTPARAM_STARTOFFSET_STREETADDR, COMPACTPARAM_BITSIZE_STREETADDR);
		unsigned int numvalidkeys7 = READFROM_ULONG(*longdata7, COMPACTPARAM_STARTOFFSET_NUMITEMS, COMPACTPARAM_BITSIZE_NUMITEMS);
		
		#endif

		unsigned int partitionupperlimit0 = streetaddr0 * (1 << globalparams.applyvertexbuffersz_pow); // CRITICAL VHLS CHECKME. (dataset dependent).
		// unsigned int partitionupperlimit0 = streetaddr0 * (1 << APPLYVERTEXBUFFERSZ_POW); // CRITICAL VHLS CHECKME. (dataset dependent).
		
		unsigned int partitionupperlimit1 = streetaddr1 * (1 << globalparams.applyvertexbuffersz_pow); // CRITICAL VHLS CHECKME. (dataset dependent).
		// unsigned int partitionupperlimit1 = streetaddr1 * (1 << APPLYVERTEXBUFFERSZ_POW); // CRITICAL VHLS CHECKME. (dataset dependent).
		
		unsigned int partitionupperlimit2 = streetaddr2 * (1 << globalparams.applyvertexbuffersz_pow); // CRITICAL VHLS CHECKME. (dataset dependent).
		// unsigned int partitionupperlimit2 = streetaddr2 * (1 << APPLYVERTEXBUFFERSZ_POW); // CRITICAL VHLS CHECKME. (dataset dependent).
		
		unsigned int partitionupperlimit3 = streetaddr3 * (1 << globalparams.applyvertexbuffersz_pow); // CRITICAL VHLS CHECKME. (dataset dependent).
		// unsigned int partitionupperlimit3 = streetaddr3 * (1 << APPLYVERTEXBUFFERSZ_POW); // CRITICAL VHLS CHECKME. (dataset dependent).
		
		unsigned int partitionupperlimit4 = streetaddr4 * (1 << globalparams.applyvertexbuffersz_pow); // CRITICAL VHLS CHECKME. (dataset dependent).
		// unsigned int partitionupperlimit4 = streetaddr4 * (1 << APPLYVERTEXBUFFERSZ_POW); // CRITICAL VHLS CHECKME. (dataset dependent).
		
		unsigned int partitionupperlimit5 = streetaddr5 * (1 << globalparams.applyvertexbuffersz_pow); // CRITICAL VHLS CHECKME. (dataset dependent).
		// unsigned int partitionupperlimit5 = streetaddr5 * (1 << APPLYVERTEXBUFFERSZ_POW); // CRITICAL VHLS CHECKME. (dataset dependent).
		
		unsigned int partitionupperlimit6 = streetaddr6 * (1 << globalparams.applyvertexbuffersz_pow); // CRITICAL VHLS CHECKME. (dataset dependent).
		// unsigned int partitionupperlimit6 = streetaddr6 * (1 << APPLYVERTEXBUFFERSZ_POW); // CRITICAL VHLS CHECKME. (dataset dependent).
		
		unsigned int partitionupperlimit7 = streetaddr7 * (1 << globalparams.applyvertexbuffersz_pow); // CRITICAL VHLS CHECKME. (dataset dependent).
		// unsigned int partitionupperlimit7 = streetaddr7 * (1 << APPLYVERTEXBUFFERSZ_POW); // CRITICAL VHLS CHECKME. (dataset dependent).
		
		
		REDUCE_LOOP2: for(buffer_type t=0; t<COMPACTPARAM_ITEMSIZE_TOTALDATA; t++){
		#pragma HLS PIPELINE II=3
			#ifdef _WIDEWORD // CRITICAL VHLS CHECKME.
				#ifdef KOOOKOOOO
				keyy_t keys0 = partitionupperlimit0 + longdata0.range(COMPACTPARAM_STARTOFFSET_DATA + t*COMPACTPARAM_BITSIZE_EACHDATA + COMPACTPARAM_BITSIZE_EACHDATA-1, COMPACTPARAM_STARTOFFSET_DATA + t*COMPACTPARAM_BITSIZE_EACHDATA); 
				keyy_t keys1 = partitionupperlimit1 + longdata1.range(COMPACTPARAM_STARTOFFSET_DATA + t*COMPACTPARAM_BITSIZE_EACHDATA + COMPACTPARAM_BITSIZE_EACHDATA-1, COMPACTPARAM_STARTOFFSET_DATA + t*COMPACTPARAM_BITSIZE_EACHDATA); 
				keyy_t keys2 = partitionupperlimit2 + longdata2.range(COMPACTPARAM_STARTOFFSET_DATA + t*COMPACTPARAM_BITSIZE_EACHDATA + COMPACTPARAM_BITSIZE_EACHDATA-1, COMPACTPARAM_STARTOFFSET_DATA + t*COMPACTPARAM_BITSIZE_EACHDATA); 
				keyy_t keys3 = partitionupperlimit3 + longdata3.range(COMPACTPARAM_STARTOFFSET_DATA + t*COMPACTPARAM_BITSIZE_EACHDATA + COMPACTPARAM_BITSIZE_EACHDATA-1, COMPACTPARAM_STARTOFFSET_DATA + t*COMPACTPARAM_BITSIZE_EACHDATA); 
				keyy_t keys4 = partitionupperlimit4 + longdata4.range(COMPACTPARAM_STARTOFFSET_DATA + t*COMPACTPARAM_BITSIZE_EACHDATA + COMPACTPARAM_BITSIZE_EACHDATA-1, COMPACTPARAM_STARTOFFSET_DATA + t*COMPACTPARAM_BITSIZE_EACHDATA); 
				keyy_t keys5 = partitionupperlimit5 + longdata5.range(COMPACTPARAM_STARTOFFSET_DATA + t*COMPACTPARAM_BITSIZE_EACHDATA + COMPACTPARAM_BITSIZE_EACHDATA-1, COMPACTPARAM_STARTOFFSET_DATA + t*COMPACTPARAM_BITSIZE_EACHDATA); 
				keyy_t keys6 = partitionupperlimit6 + longdata6.range(COMPACTPARAM_STARTOFFSET_DATA + t*COMPACTPARAM_BITSIZE_EACHDATA + COMPACTPARAM_BITSIZE_EACHDATA-1, COMPACTPARAM_STARTOFFSET_DATA + t*COMPACTPARAM_BITSIZE_EACHDATA); 
				keyy_t keys7 = partitionupperlimit7 + longdata7.range(COMPACTPARAM_STARTOFFSET_DATA + t*COMPACTPARAM_BITSIZE_EACHDATA + COMPACTPARAM_BITSIZE_EACHDATA-1, COMPACTPARAM_STARTOFFSET_DATA + t*COMPACTPARAM_BITSIZE_EACHDATA); 
				#else
				keyy_t keys0;
				keyy_t keys1;
				keyy_t keys2;
				keyy_t keys3;
				keyy_t keys4;
				keyy_t keys5;
				keyy_t keys6;
				keyy_t keys7;
				if(t==0){
					keys0 = partitionupperlimit0 + longdata0.range(COMPACTPARAM_BITSIZE_EACHDATA-1, 0);
					keys1 = partitionupperlimit1 + longdata1.range(COMPACTPARAM_BITSIZE_EACHDATA-1, 0);
					keys2 = partitionupperlimit2 + longdata2.range(COMPACTPARAM_BITSIZE_EACHDATA-1, 0);
					keys3 = partitionupperlimit3 + longdata3.range(COMPACTPARAM_BITSIZE_EACHDATA-1, 0);
					keys4 = partitionupperlimit4 + longdata4.range(COMPACTPARAM_BITSIZE_EACHDATA-1, 0);
					keys5 = partitionupperlimit5 + longdata5.range(COMPACTPARAM_BITSIZE_EACHDATA-1, 0);
					keys6 = partitionupperlimit6 + longdata6.range(COMPACTPARAM_BITSIZE_EACHDATA-1, 0);
					keys7 = partitionupperlimit7 + longdata7.range(COMPACTPARAM_BITSIZE_EACHDATA-1, 0);
				} else if(t==1){
					keys0 = partitionupperlimit0 + longdata0.range(2*COMPACTPARAM_BITSIZE_EACHDATA-1, COMPACTPARAM_BITSIZE_EACHDATA);
					keys1 = partitionupperlimit1 + longdata1.range(2*COMPACTPARAM_BITSIZE_EACHDATA-1, COMPACTPARAM_BITSIZE_EACHDATA);
					keys2 = partitionupperlimit2 + longdata2.range(2*COMPACTPARAM_BITSIZE_EACHDATA-1, COMPACTPARAM_BITSIZE_EACHDATA);
					keys3 = partitionupperlimit3 + longdata3.range(2*COMPACTPARAM_BITSIZE_EACHDATA-1, COMPACTPARAM_BITSIZE_EACHDATA);
					keys4 = partitionupperlimit4 + longdata4.range(2*COMPACTPARAM_BITSIZE_EACHDATA-1, COMPACTPARAM_BITSIZE_EACHDATA);
					keys5 = partitionupperlimit5 + longdata5.range(2*COMPACTPARAM_BITSIZE_EACHDATA-1, COMPACTPARAM_BITSIZE_EACHDATA);
					keys6 = partitionupperlimit6 + longdata6.range(2*COMPACTPARAM_BITSIZE_EACHDATA-1, COMPACTPARAM_BITSIZE_EACHDATA);
					keys7 = partitionupperlimit7 + longdata7.range(2*COMPACTPARAM_BITSIZE_EACHDATA-1, COMPACTPARAM_BITSIZE_EACHDATA);
				} else if(t==2){ // CRITICAL AUTOMATEME.
					keys0 = partitionupperlimit0 + longdata0.range(3*COMPACTPARAM_BITSIZE_EACHDATA-1, 2*COMPACTPARAM_BITSIZE_EACHDATA);
					keys1 = partitionupperlimit1 + longdata1.range(3*COMPACTPARAM_BITSIZE_EACHDATA-1, 2*COMPACTPARAM_BITSIZE_EACHDATA);
					keys2 = partitionupperlimit2 + longdata2.range(3*COMPACTPARAM_BITSIZE_EACHDATA-1, 2*COMPACTPARAM_BITSIZE_EACHDATA);
					keys3 = partitionupperlimit3 + longdata3.range(3*COMPACTPARAM_BITSIZE_EACHDATA-1, 2*COMPACTPARAM_BITSIZE_EACHDATA);
					keys4 = partitionupperlimit4 + longdata4.range(3*COMPACTPARAM_BITSIZE_EACHDATA-1, 2*COMPACTPARAM_BITSIZE_EACHDATA);
					keys5 = partitionupperlimit5 + longdata5.range(3*COMPACTPARAM_BITSIZE_EACHDATA-1, 2*COMPACTPARAM_BITSIZE_EACHDATA);
					keys6 = partitionupperlimit6 + longdata6.range(3*COMPACTPARAM_BITSIZE_EACHDATA-1, 2*COMPACTPARAM_BITSIZE_EACHDATA);
					keys7 = partitionupperlimit7 + longdata7.range(3*COMPACTPARAM_BITSIZE_EACHDATA-1, 2*COMPACTPARAM_BITSIZE_EACHDATA);
				} else {
					keys0 = 0;
					keys1 = 0;
					keys2 = 0;
					keys3 = 0;
					keys4 = 0;
					keys5 = 0;
					keys6 = 0;
					keys7 = 0;
					#ifdef _DEBUGMODE_CHECKS2
					cout<<"reduce_bfs. should never get here. exiting..."<<endl; 
					exit(EXIT_FAILURE);
					#endif 
				}
				#endif
			#else 
			keyy_t keys0 = partitionupperlimit0 + READFROM_ULONG(*longdata0, COMPACTPARAM_STARTOFFSET_DATA + t*COMPACTPARAM_BITSIZE_EACHDATA, COMPACTPARAM_BITSIZE_EACHDATA);
			keyy_t keys1 = partitionupperlimit1 + READFROM_ULONG(*longdata1, COMPACTPARAM_STARTOFFSET_DATA + t*COMPACTPARAM_BITSIZE_EACHDATA, COMPACTPARAM_BITSIZE_EACHDATA);
			keyy_t keys2 = partitionupperlimit2 + READFROM_ULONG(*longdata2, COMPACTPARAM_STARTOFFSET_DATA + t*COMPACTPARAM_BITSIZE_EACHDATA, COMPACTPARAM_BITSIZE_EACHDATA);
			keyy_t keys3 = partitionupperlimit3 + READFROM_ULONG(*longdata3, COMPACTPARAM_STARTOFFSET_DATA + t*COMPACTPARAM_BITSIZE_EACHDATA, COMPACTPARAM_BITSIZE_EACHDATA);
			keyy_t keys4 = partitionupperlimit4 + READFROM_ULONG(*longdata4, COMPACTPARAM_STARTOFFSET_DATA + t*COMPACTPARAM_BITSIZE_EACHDATA, COMPACTPARAM_BITSIZE_EACHDATA);
			keyy_t keys5 = partitionupperlimit5 + READFROM_ULONG(*longdata5, COMPACTPARAM_STARTOFFSET_DATA + t*COMPACTPARAM_BITSIZE_EACHDATA, COMPACTPARAM_BITSIZE_EACHDATA);
			keyy_t keys6 = partitionupperlimit6 + READFROM_ULONG(*longdata6, COMPACTPARAM_STARTOFFSET_DATA + t*COMPACTPARAM_BITSIZE_EACHDATA, COMPACTPARAM_BITSIZE_EACHDATA);
			keyy_t keys7 = partitionupperlimit7 + READFROM_ULONG(*longdata7, COMPACTPARAM_STARTOFFSET_DATA + t*COMPACTPARAM_BITSIZE_EACHDATA, COMPACTPARAM_BITSIZE_EACHDATA);
	
			#endif 
			
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"reduce_bfs: keys0: "<<keys0<<endl;
			cout<<"reduce_bfs: keys1: "<<keys1<<endl;
			cout<<"reduce_bfs: keys2: "<<keys2<<endl;
			cout<<"reduce_bfs: keys3: "<<keys3<<endl;
			cout<<"reduce_bfs: keys4: "<<keys4<<endl;
			cout<<"reduce_bfs: keys5: "<<keys5<<endl;
			cout<<"reduce_bfs: keys6: "<<keys6<<endl;
			cout<<"reduce_bfs: keys7: "<<keys7<<endl;
			#endif
			
			if(t < numvalidkeys0){ en20 = ON; } else { en20 = OFF; }
			if(t < numvalidkeys1){ en21 = ON; } else { en21 = OFF; }
			if(t < numvalidkeys2){ en22 = ON; } else { en22 = OFF; }
			if(t < numvalidkeys3){ en23 = ON; } else { en23 = OFF; }
			if(t < numvalidkeys4){ en24 = ON; } else { en24 = OFF; }
			if(t < numvalidkeys5){ en25 = ON; } else { en25 = OFF; }
			if(t < numvalidkeys6){ en26 = ON; } else { en26 = OFF; }
			if(t < numvalidkeys7){ en27 = ON; } else { en27 = OFF; }
			
			if(en0 == ON && en20 == ON){ 
				locs0 = keys0 - upperlimit; 
				rowindexs0 = locs0 / (2 * VDATAPACKINGFACTOR); // max:511
				colindexs0 = (locs0 % (2 * VDATAPACKINGFACTOR)) * 2;  // max:62
			}
			if(en1 == ON && en21 == ON){ 
				locs1 = keys1 - upperlimit; 
				rowindexs1 = locs1 / (2 * VDATAPACKINGFACTOR); // max:511
				colindexs1 = (locs1 % (2 * VDATAPACKINGFACTOR)) * 2;  // max:62
			}
			if(en2 == ON && en22 == ON){ 
				locs2 = keys2 - upperlimit; 
				rowindexs2 = locs2 / (2 * VDATAPACKINGFACTOR); // max:511
				colindexs2 = (locs2 % (2 * VDATAPACKINGFACTOR)) * 2;  // max:62
			}
			if(en3 == ON && en23 == ON){ 
				locs3 = keys3 - upperlimit; 
				rowindexs3 = locs3 / (2 * VDATAPACKINGFACTOR); // max:511
				colindexs3 = (locs3 % (2 * VDATAPACKINGFACTOR)) * 2;  // max:62
			}
			if(en4 == ON && en24 == ON){ 
				locs4 = keys4 - upperlimit; 
				rowindexs4 = locs4 / (2 * VDATAPACKINGFACTOR); // max:511
				colindexs4 = (locs4 % (2 * VDATAPACKINGFACTOR)) * 2;  // max:62
			}
			if(en5 == ON && en25 == ON){ 
				locs5 = keys5 - upperlimit; 
				rowindexs5 = locs5 / (2 * VDATAPACKINGFACTOR); // max:511
				colindexs5 = (locs5 % (2 * VDATAPACKINGFACTOR)) * 2;  // max:62
			}
			if(en6 == ON && en26 == ON){ 
				locs6 = keys6 - upperlimit; 
				rowindexs6 = locs6 / (2 * VDATAPACKINGFACTOR); // max:511
				colindexs6 = (locs6 % (2 * VDATAPACKINGFACTOR)) * 2;  // max:62
			}
			if(en7 == ON && en27 == ON){ 
				locs7 = keys7 - upperlimit; 
				rowindexs7 = locs7 / (2 * VDATAPACKINGFACTOR); // max:511
				colindexs7 = (locs7 % (2 * VDATAPACKINGFACTOR)) * 2;  // max:62
			}
			
			#ifdef _DEBUGMODE_CHECKS2
			if(en0 == ON && en20 == ON){
				if(locs0 >= globalparams.applyvertexbuffersz){ actsutilityobj->globalstats_counterrorsinreduce(1); }
				#ifdef ENABLE_VOICEOUTREDUCEERRORS
				if(locs0 >= globalparams.applyvertexbuffersz){ cout<<"ERROR SEEN @ reduce_bfs:: i: "<<i<<", locs0: "<<locs0<<", keys0: "<<keys0<<", keyvalue0.key: "<<keyvalue0.key<<", keyvalue0.value: "<<keyvalue0.value<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; }
				#endif 
				#ifdef ENABLE_PERFECTACCURACY
				if(locs0 >= globalparams.applyvertexbuffersz){ if(actsutilityobj->globalstats_getcounterrorsinreduce() > 0){ cout<<"too many ("<<actsutilityobj->globalstats_getcounterrorsinreduce()<<") reduce_bfs errors. EXITING"<<endl; exit(EXIT_FAILURE); } }
				// #endif
				actsutilityobj->checkoutofbounds("reduce_bfs.rowindexs0", rowindexs0, 512, keyvalue0.key, keyvalue0.value, locs0);
				actsutilityobj->checkoutofbounds("reduce_bfs.(64 - colindexs0)", colindexs0, 64, colindexs0, NAp, NAp);
				#endif
				#ifdef _DEBUGMODE_CHECKS2
				if(locs0 >= globalparams.applyvertexbuffersz){ if(actsutilityobj->globalstats_getcounterrorsinreduce() > 1000){ cout<<"too many ("<<actsutilityobj->globalstats_getcounterrorsinreduce()<<") reduce_bfs tolerable errors. EXITING"<<endl; exit(EXIT_FAILURE); } }
				#endif 
				#ifdef _DEBUGMODE_STATS
				actsutilityobj->globalstats_reduce_countvalidkvsreduced(1);
				actsutilityobj->globalstats_reduce_countvar1(keys0);
				#endif
			}
			if(en1 == ON && en21 == ON){
				if(locs1 >= globalparams.applyvertexbuffersz){ actsutilityobj->globalstats_counterrorsinreduce(1); }
				#ifdef ENABLE_VOICEOUTREDUCEERRORS
				if(locs1 >= globalparams.applyvertexbuffersz){ cout<<"ERROR SEEN @ reduce_bfs:: i: "<<i<<", locs1: "<<locs1<<", keys1: "<<keys1<<", keyvalue1.key: "<<keyvalue1.key<<", keyvalue1.value: "<<keyvalue1.value<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; }
				#endif 
				#ifdef ENABLE_PERFECTACCURACY
				if(locs1 >= globalparams.applyvertexbuffersz){ if(actsutilityobj->globalstats_getcounterrorsinreduce() > 0){ cout<<"too many ("<<actsutilityobj->globalstats_getcounterrorsinreduce()<<") reduce_bfs errors. EXITING"<<endl; exit(EXIT_FAILURE); } }
				// #endif
				actsutilityobj->checkoutofbounds("reduce_bfs.rowindexs1", rowindexs1, 512, keyvalue1.key, keyvalue1.value, locs1);
				actsutilityobj->checkoutofbounds("reduce_bfs.(64 - colindexs1)", colindexs1, 64, colindexs1, NAp, NAp);
				#endif
				#ifdef _DEBUGMODE_CHECKS2
				if(locs1 >= globalparams.applyvertexbuffersz){ if(actsutilityobj->globalstats_getcounterrorsinreduce() > 1000){ cout<<"too many ("<<actsutilityobj->globalstats_getcounterrorsinreduce()<<") reduce_bfs tolerable errors. EXITING"<<endl; exit(EXIT_FAILURE); } }
				#endif 
				#ifdef _DEBUGMODE_STATS
				actsutilityobj->globalstats_reduce_countvalidkvsreduced(1);
				actsutilityobj->globalstats_reduce_countvar1(keys1);
				#endif
			}
			if(en2 == ON && en22 == ON){
				if(locs2 >= globalparams.applyvertexbuffersz){ actsutilityobj->globalstats_counterrorsinreduce(1); }
				#ifdef ENABLE_VOICEOUTREDUCEERRORS
				if(locs2 >= globalparams.applyvertexbuffersz){ cout<<"ERROR SEEN @ reduce_bfs:: i: "<<i<<", locs2: "<<locs2<<", keys2: "<<keys2<<", keyvalue2.key: "<<keyvalue2.key<<", keyvalue2.value: "<<keyvalue2.value<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; }
				#endif 
				#ifdef ENABLE_PERFECTACCURACY
				if(locs2 >= globalparams.applyvertexbuffersz){ if(actsutilityobj->globalstats_getcounterrorsinreduce() > 0){ cout<<"too many ("<<actsutilityobj->globalstats_getcounterrorsinreduce()<<") reduce_bfs errors. EXITING"<<endl; exit(EXIT_FAILURE); } }
				// #endif
				actsutilityobj->checkoutofbounds("reduce_bfs.rowindexs2", rowindexs2, 512, keyvalue2.key, keyvalue2.value, locs2);
				actsutilityobj->checkoutofbounds("reduce_bfs.(64 - colindexs2)", colindexs2, 64, colindexs2, NAp, NAp);
				#endif
				#ifdef _DEBUGMODE_CHECKS2
				if(locs2 >= globalparams.applyvertexbuffersz){ if(actsutilityobj->globalstats_getcounterrorsinreduce() > 1000){ cout<<"too many ("<<actsutilityobj->globalstats_getcounterrorsinreduce()<<") reduce_bfs tolerable errors. EXITING"<<endl; exit(EXIT_FAILURE); } }
				#endif 
				#ifdef _DEBUGMODE_STATS
				actsutilityobj->globalstats_reduce_countvalidkvsreduced(1);
				actsutilityobj->globalstats_reduce_countvar1(keys2);
				#endif
			}
			if(en3 == ON && en23 == ON){
				if(locs3 >= globalparams.applyvertexbuffersz){ actsutilityobj->globalstats_counterrorsinreduce(1); }
				#ifdef ENABLE_VOICEOUTREDUCEERRORS
				if(locs3 >= globalparams.applyvertexbuffersz){ cout<<"ERROR SEEN @ reduce_bfs:: i: "<<i<<", locs3: "<<locs3<<", keys3: "<<keys3<<", keyvalue3.key: "<<keyvalue3.key<<", keyvalue3.value: "<<keyvalue3.value<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; }
				#endif 
				#ifdef ENABLE_PERFECTACCURACY
				if(locs3 >= globalparams.applyvertexbuffersz){ if(actsutilityobj->globalstats_getcounterrorsinreduce() > 0){ cout<<"too many ("<<actsutilityobj->globalstats_getcounterrorsinreduce()<<") reduce_bfs errors. EXITING"<<endl; exit(EXIT_FAILURE); } }
				// #endif
				actsutilityobj->checkoutofbounds("reduce_bfs.rowindexs3", rowindexs3, 512, keyvalue3.key, keyvalue3.value, locs3);
				actsutilityobj->checkoutofbounds("reduce_bfs.(64 - colindexs3)", colindexs3, 64, colindexs3, NAp, NAp);
				#endif
				#ifdef _DEBUGMODE_CHECKS2
				if(locs3 >= globalparams.applyvertexbuffersz){ if(actsutilityobj->globalstats_getcounterrorsinreduce() > 1000){ cout<<"too many ("<<actsutilityobj->globalstats_getcounterrorsinreduce()<<") reduce_bfs tolerable errors. EXITING"<<endl; exit(EXIT_FAILURE); } }
				#endif 
				#ifdef _DEBUGMODE_STATS
				actsutilityobj->globalstats_reduce_countvalidkvsreduced(1);
				actsutilityobj->globalstats_reduce_countvar1(keys3);
				#endif
			}
			if(en4 == ON && en24 == ON){
				if(locs4 >= globalparams.applyvertexbuffersz){ actsutilityobj->globalstats_counterrorsinreduce(1); }
				#ifdef ENABLE_VOICEOUTREDUCEERRORS
				if(locs4 >= globalparams.applyvertexbuffersz){ cout<<"ERROR SEEN @ reduce_bfs:: i: "<<i<<", locs4: "<<locs4<<", keys4: "<<keys4<<", keyvalue4.key: "<<keyvalue4.key<<", keyvalue4.value: "<<keyvalue4.value<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; }
				#endif 
				#ifdef ENABLE_PERFECTACCURACY
				if(locs4 >= globalparams.applyvertexbuffersz){ if(actsutilityobj->globalstats_getcounterrorsinreduce() > 0){ cout<<"too many ("<<actsutilityobj->globalstats_getcounterrorsinreduce()<<") reduce_bfs errors. EXITING"<<endl; exit(EXIT_FAILURE); } }
				// #endif
				actsutilityobj->checkoutofbounds("reduce_bfs.rowindexs4", rowindexs4, 512, keyvalue4.key, keyvalue4.value, locs4);
				actsutilityobj->checkoutofbounds("reduce_bfs.(64 - colindexs4)", colindexs4, 64, colindexs4, NAp, NAp);
				#endif
				#ifdef _DEBUGMODE_CHECKS2
				if(locs4 >= globalparams.applyvertexbuffersz){ if(actsutilityobj->globalstats_getcounterrorsinreduce() > 1000){ cout<<"too many ("<<actsutilityobj->globalstats_getcounterrorsinreduce()<<") reduce_bfs tolerable errors. EXITING"<<endl; exit(EXIT_FAILURE); } }
				#endif 
				#ifdef _DEBUGMODE_STATS
				actsutilityobj->globalstats_reduce_countvalidkvsreduced(1);
				actsutilityobj->globalstats_reduce_countvar1(keys4);
				#endif
			}
			if(en5 == ON && en25 == ON){
				if(locs5 >= globalparams.applyvertexbuffersz){ actsutilityobj->globalstats_counterrorsinreduce(1); }
				#ifdef ENABLE_VOICEOUTREDUCEERRORS
				if(locs5 >= globalparams.applyvertexbuffersz){ cout<<"ERROR SEEN @ reduce_bfs:: i: "<<i<<", locs5: "<<locs5<<", keys5: "<<keys5<<", keyvalue5.key: "<<keyvalue5.key<<", keyvalue5.value: "<<keyvalue5.value<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; }
				#endif 
				#ifdef ENABLE_PERFECTACCURACY
				if(locs5 >= globalparams.applyvertexbuffersz){ if(actsutilityobj->globalstats_getcounterrorsinreduce() > 0){ cout<<"too many ("<<actsutilityobj->globalstats_getcounterrorsinreduce()<<") reduce_bfs errors. EXITING"<<endl; exit(EXIT_FAILURE); } }
				// #endif
				actsutilityobj->checkoutofbounds("reduce_bfs.rowindexs5", rowindexs5, 512, keyvalue5.key, keyvalue5.value, locs5);
				actsutilityobj->checkoutofbounds("reduce_bfs.(64 - colindexs5)", colindexs5, 64, colindexs5, NAp, NAp);
				#endif
				#ifdef _DEBUGMODE_CHECKS2
				if(locs5 >= globalparams.applyvertexbuffersz){ if(actsutilityobj->globalstats_getcounterrorsinreduce() > 1000){ cout<<"too many ("<<actsutilityobj->globalstats_getcounterrorsinreduce()<<") reduce_bfs tolerable errors. EXITING"<<endl; exit(EXIT_FAILURE); } }
				#endif 
				#ifdef _DEBUGMODE_STATS
				actsutilityobj->globalstats_reduce_countvalidkvsreduced(1);
				actsutilityobj->globalstats_reduce_countvar1(keys5);
				#endif
			}
			if(en6 == ON && en26 == ON){
				if(locs6 >= globalparams.applyvertexbuffersz){ actsutilityobj->globalstats_counterrorsinreduce(1); }
				#ifdef ENABLE_VOICEOUTREDUCEERRORS
				if(locs6 >= globalparams.applyvertexbuffersz){ cout<<"ERROR SEEN @ reduce_bfs:: i: "<<i<<", locs6: "<<locs6<<", keys6: "<<keys6<<", keyvalue6.key: "<<keyvalue6.key<<", keyvalue6.value: "<<keyvalue6.value<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; }
				#endif 
				#ifdef ENABLE_PERFECTACCURACY
				if(locs6 >= globalparams.applyvertexbuffersz){ if(actsutilityobj->globalstats_getcounterrorsinreduce() > 0){ cout<<"too many ("<<actsutilityobj->globalstats_getcounterrorsinreduce()<<") reduce_bfs errors. EXITING"<<endl; exit(EXIT_FAILURE); } }
				// #endif
				actsutilityobj->checkoutofbounds("reduce_bfs.rowindexs6", rowindexs6, 512, keyvalue6.key, keyvalue6.value, locs6);
				actsutilityobj->checkoutofbounds("reduce_bfs.(64 - colindexs6)", colindexs6, 64, colindexs6, NAp, NAp);
				#endif
				#ifdef _DEBUGMODE_CHECKS2
				if(locs6 >= globalparams.applyvertexbuffersz){ if(actsutilityobj->globalstats_getcounterrorsinreduce() > 1000){ cout<<"too many ("<<actsutilityobj->globalstats_getcounterrorsinreduce()<<") reduce_bfs tolerable errors. EXITING"<<endl; exit(EXIT_FAILURE); } }
				#endif 
				#ifdef _DEBUGMODE_STATS
				actsutilityobj->globalstats_reduce_countvalidkvsreduced(1);
				actsutilityobj->globalstats_reduce_countvar1(keys6);
				#endif
			}
			if(en7 == ON && en27 == ON){
				if(locs7 >= globalparams.applyvertexbuffersz){ actsutilityobj->globalstats_counterrorsinreduce(1); }
				#ifdef ENABLE_VOICEOUTREDUCEERRORS
				if(locs7 >= globalparams.applyvertexbuffersz){ cout<<"ERROR SEEN @ reduce_bfs:: i: "<<i<<", locs7: "<<locs7<<", keys7: "<<keys7<<", keyvalue7.key: "<<keyvalue7.key<<", keyvalue7.value: "<<keyvalue7.value<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; }
				#endif 
				#ifdef ENABLE_PERFECTACCURACY
				if(locs7 >= globalparams.applyvertexbuffersz){ if(actsutilityobj->globalstats_getcounterrorsinreduce() > 0){ cout<<"too many ("<<actsutilityobj->globalstats_getcounterrorsinreduce()<<") reduce_bfs errors. EXITING"<<endl; exit(EXIT_FAILURE); } }
				// #endif
				actsutilityobj->checkoutofbounds("reduce_bfs.rowindexs7", rowindexs7, 512, keyvalue7.key, keyvalue7.value, locs7);
				actsutilityobj->checkoutofbounds("reduce_bfs.(64 - colindexs7)", colindexs7, 64, colindexs7, NAp, NAp);
				#endif
				#ifdef _DEBUGMODE_CHECKS2
				if(locs7 >= globalparams.applyvertexbuffersz){ if(actsutilityobj->globalstats_getcounterrorsinreduce() > 1000){ cout<<"too many ("<<actsutilityobj->globalstats_getcounterrorsinreduce()<<") reduce_bfs tolerable errors. EXITING"<<endl; exit(EXIT_FAILURE); } }
				#endif 
				#ifdef _DEBUGMODE_STATS
				actsutilityobj->globalstats_reduce_countvalidkvsreduced(1);
				actsutilityobj->globalstats_reduce_countvar1(keys7);
				#endif
			}
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countkvsreduced(VECTOR_SIZE*4);
			#endif
			#endif
			
			if(locs0 >= globalparams.applyvertexbuffersz && en0 == ON && en20 == ON){ locs0 = 0; }
			if(en0 == ON && en20 == ON){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("reduce_bfs 444a", colindexs0, 64, colindexs0, 0, NAp);
				#endif
				vprop0 = destbuffer[0][rowindexs0]; 
				temps0 = READFROM_ULONG(vprop0, colindexs0, 2); 
				rettemps0 = reducefunc_bfs(temps0, NAp, GraphIter, GraphAlgo); 
				WRITETO_ULONG(&vprop0, colindexs0, 2, rettemps0);
				destbuffer[0][rowindexs0] = vprop0; 
			}
			if(locs1 >= globalparams.applyvertexbuffersz && en1 == ON && en21 == ON){ locs1 = 0; }
			if(en1 == ON && en21 == ON){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("reduce_bfs 444a", colindexs1, 64, colindexs1, 1, NAp);
				#endif
				vprop1 = destbuffer[1][rowindexs1]; 
				temps1 = READFROM_ULONG(vprop1, colindexs1, 2); 
				rettemps1 = reducefunc_bfs(temps1, NAp, GraphIter, GraphAlgo); 
				WRITETO_ULONG(&vprop1, colindexs1, 2, rettemps1);
				destbuffer[1][rowindexs1] = vprop1; 
			}
			if(locs2 >= globalparams.applyvertexbuffersz && en2 == ON && en22 == ON){ locs2 = 0; }
			if(en2 == ON && en22 == ON){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("reduce_bfs 444a", colindexs2, 64, colindexs2, 2, NAp);
				#endif
				vprop2 = destbuffer[2][rowindexs2]; 
				temps2 = READFROM_ULONG(vprop2, colindexs2, 2); 
				rettemps2 = reducefunc_bfs(temps2, NAp, GraphIter, GraphAlgo); 
				WRITETO_ULONG(&vprop2, colindexs2, 2, rettemps2);
				destbuffer[2][rowindexs2] = vprop2; 
			}
			if(locs3 >= globalparams.applyvertexbuffersz && en3 == ON && en23 == ON){ locs3 = 0; }
			if(en3 == ON && en23 == ON){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("reduce_bfs 444a", colindexs3, 64, colindexs3, 3, NAp);
				#endif
				vprop3 = destbuffer[3][rowindexs3]; 
				temps3 = READFROM_ULONG(vprop3, colindexs3, 2); 
				rettemps3 = reducefunc_bfs(temps3, NAp, GraphIter, GraphAlgo); 
				WRITETO_ULONG(&vprop3, colindexs3, 2, rettemps3);
				destbuffer[3][rowindexs3] = vprop3; 
			}
			if(locs4 >= globalparams.applyvertexbuffersz && en4 == ON && en24 == ON){ locs4 = 0; }
			if(en4 == ON && en24 == ON){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("reduce_bfs 444a", colindexs4, 64, colindexs4, 4, NAp);
				#endif
				vprop4 = destbuffer[4][rowindexs4]; 
				temps4 = READFROM_ULONG(vprop4, colindexs4, 2); 
				rettemps4 = reducefunc_bfs(temps4, NAp, GraphIter, GraphAlgo); 
				WRITETO_ULONG(&vprop4, colindexs4, 2, rettemps4);
				destbuffer[4][rowindexs4] = vprop4; 
			}
			if(locs5 >= globalparams.applyvertexbuffersz && en5 == ON && en25 == ON){ locs5 = 0; }
			if(en5 == ON && en25 == ON){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("reduce_bfs 444a", colindexs5, 64, colindexs5, 5, NAp);
				#endif
				vprop5 = destbuffer[5][rowindexs5]; 
				temps5 = READFROM_ULONG(vprop5, colindexs5, 2); 
				rettemps5 = reducefunc_bfs(temps5, NAp, GraphIter, GraphAlgo); 
				WRITETO_ULONG(&vprop5, colindexs5, 2, rettemps5);
				destbuffer[5][rowindexs5] = vprop5; 
			}
			if(locs6 >= globalparams.applyvertexbuffersz && en6 == ON && en26 == ON){ locs6 = 0; }
			if(en6 == ON && en26 == ON){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("reduce_bfs 444a", colindexs6, 64, colindexs6, 6, NAp);
				#endif
				vprop6 = destbuffer[6][rowindexs6]; 
				temps6 = READFROM_ULONG(vprop6, colindexs6, 2); 
				rettemps6 = reducefunc_bfs(temps6, NAp, GraphIter, GraphAlgo); 
				WRITETO_ULONG(&vprop6, colindexs6, 2, rettemps6);
				destbuffer[6][rowindexs6] = vprop6; 
			}
			if(locs7 >= globalparams.applyvertexbuffersz && en7 == ON && en27 == ON){ locs7 = 0; }
			if(en7 == ON && en27 == ON){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("reduce_bfs 444a", colindexs7, 64, colindexs7, 7, NAp);
				#endif
				vprop7 = destbuffer[7][rowindexs7]; 
				temps7 = READFROM_ULONG(vprop7, colindexs7, 2); 
				rettemps7 = reducefunc_bfs(temps7, NAp, GraphIter, GraphAlgo); 
				WRITETO_ULONG(&vprop7, colindexs7, 2, rettemps7);
				destbuffer[7][rowindexs7] = vprop7; 
			}
			
			#ifdef _DEBUGMODE_KERNELPRINTS
			if(en0 == ON && en20 == ON){
				cout<<"REDUCE_BFS SEEN @ reduce_bfs:: i: "<<i<<", locs0: "<<locs0<<", keys0: "<<keys0<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; 
				cout<<"REDUCE_BFS: keys0: "<<keys0<<", rowindexs0: "<<rowindexs0<<", colindexs0: "<<colindexs0<<", bitpos: "<<64 - colindexs0 - 2<<endl;
				cout<<"REDUCE_BFS: vprop0.key: "<<vprop0.key<<", vprop0.value: "<<vprop0.value<<", rowindexs0: "<<rowindexs0<<endl;
				cout<<"REDUCE_BFS RESULT @ reduce_bfs:: temps0: "<<temps0<<", rettemps0: "<<rettemps0<<endl;
			}
			if(en1 == ON && en21 == ON){
				cout<<"REDUCE_BFS SEEN @ reduce_bfs:: i: "<<i<<", locs1: "<<locs1<<", keys1: "<<keys1<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; 
				cout<<"REDUCE_BFS: keys1: "<<keys1<<", rowindexs1: "<<rowindexs1<<", colindexs1: "<<colindexs1<<", bitpos: "<<64 - colindexs1 - 2<<endl;
				cout<<"REDUCE_BFS: vprop1.key: "<<vprop1.key<<", vprop1.value: "<<vprop1.value<<", rowindexs1: "<<rowindexs1<<endl;
				cout<<"REDUCE_BFS RESULT @ reduce_bfs:: temps1: "<<temps1<<", rettemps1: "<<rettemps1<<endl;
			}
			if(en2 == ON && en22 == ON){
				cout<<"REDUCE_BFS SEEN @ reduce_bfs:: i: "<<i<<", locs2: "<<locs2<<", keys2: "<<keys2<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; 
				cout<<"REDUCE_BFS: keys2: "<<keys2<<", rowindexs2: "<<rowindexs2<<", colindexs2: "<<colindexs2<<", bitpos: "<<64 - colindexs2 - 2<<endl;
				cout<<"REDUCE_BFS: vprop2.key: "<<vprop2.key<<", vprop2.value: "<<vprop2.value<<", rowindexs2: "<<rowindexs2<<endl;
				cout<<"REDUCE_BFS RESULT @ reduce_bfs:: temps2: "<<temps2<<", rettemps2: "<<rettemps2<<endl;
			}
			if(en3 == ON && en23 == ON){
				cout<<"REDUCE_BFS SEEN @ reduce_bfs:: i: "<<i<<", locs3: "<<locs3<<", keys3: "<<keys3<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; 
				cout<<"REDUCE_BFS: keys3: "<<keys3<<", rowindexs3: "<<rowindexs3<<", colindexs3: "<<colindexs3<<", bitpos: "<<64 - colindexs3 - 2<<endl;
				cout<<"REDUCE_BFS: vprop3.key: "<<vprop3.key<<", vprop3.value: "<<vprop3.value<<", rowindexs3: "<<rowindexs3<<endl;
				cout<<"REDUCE_BFS RESULT @ reduce_bfs:: temps3: "<<temps3<<", rettemps3: "<<rettemps3<<endl;
			}
			if(en4 == ON && en24 == ON){
				cout<<"REDUCE_BFS SEEN @ reduce_bfs:: i: "<<i<<", locs4: "<<locs4<<", keys4: "<<keys4<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; 
				cout<<"REDUCE_BFS: keys4: "<<keys4<<", rowindexs4: "<<rowindexs4<<", colindexs4: "<<colindexs4<<", bitpos: "<<64 - colindexs4 - 2<<endl;
				cout<<"REDUCE_BFS: vprop4.key: "<<vprop4.key<<", vprop4.value: "<<vprop4.value<<", rowindexs4: "<<rowindexs4<<endl;
				cout<<"REDUCE_BFS RESULT @ reduce_bfs:: temps4: "<<temps4<<", rettemps4: "<<rettemps4<<endl;
			}
			if(en5 == ON && en25 == ON){
				cout<<"REDUCE_BFS SEEN @ reduce_bfs:: i: "<<i<<", locs5: "<<locs5<<", keys5: "<<keys5<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; 
				cout<<"REDUCE_BFS: keys5: "<<keys5<<", rowindexs5: "<<rowindexs5<<", colindexs5: "<<colindexs5<<", bitpos: "<<64 - colindexs5 - 2<<endl;
				cout<<"REDUCE_BFS: vprop5.key: "<<vprop5.key<<", vprop5.value: "<<vprop5.value<<", rowindexs5: "<<rowindexs5<<endl;
				cout<<"REDUCE_BFS RESULT @ reduce_bfs:: temps5: "<<temps5<<", rettemps5: "<<rettemps5<<endl;
			}
			if(en6 == ON && en26 == ON){
				cout<<"REDUCE_BFS SEEN @ reduce_bfs:: i: "<<i<<", locs6: "<<locs6<<", keys6: "<<keys6<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; 
				cout<<"REDUCE_BFS: keys6: "<<keys6<<", rowindexs6: "<<rowindexs6<<", colindexs6: "<<colindexs6<<", bitpos: "<<64 - colindexs6 - 2<<endl;
				cout<<"REDUCE_BFS: vprop6.key: "<<vprop6.key<<", vprop6.value: "<<vprop6.value<<", rowindexs6: "<<rowindexs6<<endl;
				cout<<"REDUCE_BFS RESULT @ reduce_bfs:: temps6: "<<temps6<<", rettemps6: "<<rettemps6<<endl;
			}
			if(en7 == ON && en27 == ON){
				cout<<"REDUCE_BFS SEEN @ reduce_bfs:: i: "<<i<<", locs7: "<<locs7<<", keys7: "<<keys7<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; 
				cout<<"REDUCE_BFS: keys7: "<<keys7<<", rowindexs7: "<<rowindexs7<<", colindexs7: "<<colindexs7<<", bitpos: "<<64 - colindexs7 - 2<<endl;
				cout<<"REDUCE_BFS: vprop7.key: "<<vprop7.key<<", vprop7.value: "<<vprop7.value<<", rowindexs7: "<<rowindexs7<<endl;
				cout<<"REDUCE_BFS RESULT @ reduce_bfs:: temps7: "<<temps7<<", rettemps7: "<<rettemps7<<endl;
			}
			#endif
		}
	}
	return;
}

value_t 
	#ifdef SW 
	acts::
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
}

keyvalue_t 
	#ifdef SW 
	acts::
	#endif 
mergefunc_bfs(keyvalue_t data1, keyvalue_t data2, unsigned int GraphAlgo){
	keyy_t key = data1.key | data2.key;
	value_t value = data1.value | data2.value;
	
	keyvalue_t res;
	res.key = key;
	res.value = value;
	return res;
}

void 
	#ifdef SW 
	acts::
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

travstate_t 
	#ifdef SW 
	acts::
	#endif
unifydata_bfs(bool_type enable, uint512_dt * kvdram, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t actvvs[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
				travstate_t actvvstravstate, buffer_type destoffset, buffer_type size, 
					sweepparams_t sweepparams, globalparams_t globalparams){
	if(enable == OFF){ return actvvstravstate; }
	analysis_type analysis_loopcount = (APPLYVERTEXBUFFERSZ / 2);
	
	buffer_type dest_v = 0;
	buffer_type dest_i = 0;
	buffer_type destoffset_kvs = destoffset / VECTOR_SIZE;
	
	UNIFYDATA_LOOP: for(buffer_type i=0; i<size; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE
		// keyvalue_t data = sourcebuffer[0][i];
		keyvalue_t data; data.key = 0; data.value = 0; // NEWCHANGE.
		for(vector_type v=0; v<VECTOR_SIZE; v++){
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("unifydata_bfs.i", i, PADDEDDESTBUFFER_SIZE, destoffset, destoffset_kvs, size);
			#endif
			// data = mergefunc_bfs(data, sourcebuffer[v][i], globalparams.GraphAlgo);
			data.key = data.key | sourcebuffer[v][i].key;
			data.value = data.value | sourcebuffer[v][i].value;
		}
		
		if(data.key != 0 || data.value != 0){
			for(unsigned int t = 0; t < 64; t+=2){
				unsigned int vid = (sweepparams.source_partition * globalparams.applyvertexbuffersz) + i*32 + t/2; // CRITICAL FIXME. use less logic.
				unsigned int index = 64 - t - 2;
				unsigned int bitsize = 2;
				unsigned int vdata = READFROM_ULONG(data, index, bitsize);
				
				if(vdata == VISITED_IN_CURRENT_ITERATION){
					#ifdef _DEBUGMODE_KERNELPRINTS
					cout<<"unifydata_bfs: ACTIVE VERTICES seen for next iteration. vid: "<<vid<<endl;
					#endif
					#ifdef _DEBUGMODE_CHECKS2
					actsutilityobj->checkoutofbounds("unifydata_bfs.actvv loc", (actvvstravstate.k * 2*VECTOR_SIZE) + actvvstravstate.v, 2*PADDEDDESTBUFFER_SIZE*VECTOR_SIZE, actvvstravstate.v, actvvstravstate.k, NAp);
					actsutilityobj->checkoutofbounds("unifydata_bfs.actvvstravstate.v", actvvstravstate.v, 2*VECTOR_SIZE, NAp, NAp, NAp);
					actsutilityobj->checkoutofbounds("unifydata_bfs.actvvstravstate.k", actvvstravstate.k, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
					#endif
					if(actvvstravstate.v % 2 == 0){ actvvs[actvvstravstate.v / 2][actvvstravstate.k].key = vid; }
					else { actvvs[actvvstravstate.v / 2][actvvstravstate.k].value = vid; }
					actvvstravstate.v+=1; if(actvvstravstate.v == 2*VECTOR_SIZE){ actvvstravstate.v=0; actvvstravstate.k+=1; }
				}
			}
		}
		
		if(actvvstravstate.k >= PADDEDDESTBUFFER_SIZE-32){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"unifydata_bfs: saving and clearning actvvs... i: "<<actvvstravstate.i<<", i_kvs: "<<actvvstravstate.i_kvs<<", (v: "<<actvvstravstate.v<<", k: "<<actvvstravstate.k<<")"<<endl;
			#endif
			savekeyvalues(ON, kvdram, actvvs, globalparams.baseoffset_activevertices_kvs + actvvstravstate.i_kvs, actvvstravstate.k);

			actvvstravstate.i += actvvstravstate.k * (VECTOR_SIZE * 2);
			actvvstravstate.i_kvs += actvvstravstate.k;
			for(vector_type v=0; v<VECTOR_SIZE; v++){
			#pragma HLS UNROLL
				actvvs[v][0] = actvvs[v][actvvstravstate.k];
			}
			actvvstravstate.v = actvvstravstate.v;
			actvvstravstate.k = 0;
		}
		
		destbuffer[dest_v][destoffset_kvs + dest_i] = data;
		dest_v+=1; if(dest_v == VECTOR_SIZE){ dest_v=0; dest_i+=1; }
	}
	return actvvstravstate;
}

travstate_t 
	#ifdef SW 
	acts::
	#endif
unifydata_bfs_syn(bool_type enable, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11, keyvalue_t sourcebuffer0[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer3[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer5[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer6[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer7[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer8[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer9[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer10[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer11[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t actvvs[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
				travstate_t actvvstravstate, buffer_type destoffset, buffer_type size, 
					sweepparams_t sweepparams, globalparams_t globalparams){
	if(enable == OFF){ return actvvstravstate; }
	analysis_type analysis_loopcount = (APPLYVERTEXBUFFERSZ / VDATAPACKINGFACTOR) / 2;
	
	buffer_type dest_v = 0;
	buffer_type dest_i = 0;
	buffer_type destoffset_kvs = destoffset / VECTOR_SIZE;
	
	UNIFYDATA_LOOP: for(buffer_type i=0; i<size; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE
		keyvalue_t data; data.key = 0; data.value = 0;
		
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("unifydata_bfs.i", i, PADDEDDESTBUFFER_SIZE, destoffset, destoffset_kvs, size);
		#endif
		#ifdef KOOOKOOOO
			for(vector_type v=0; v<VECTOR_SIZE; v++){
			#pragma HLS UNROLL
				data.key = data.key | sourcebuffer0[v][i].key;
				data.value = data.value | sourcebuffer0[v][i].value;
				data.key = data.key | sourcebuffer1[v][i].key;
				data.value = data.value | sourcebuffer1[v][i].value;
				data.key = data.key | sourcebuffer2[v][i].key;
				data.value = data.value | sourcebuffer2[v][i].value;
				data.key = data.key | sourcebuffer3[v][i].key;
				data.value = data.value | sourcebuffer3[v][i].value;
				data.key = data.key | sourcebuffer4[v][i].key;
				data.value = data.value | sourcebuffer4[v][i].value;
				data.key = data.key | sourcebuffer5[v][i].key;
				data.value = data.value | sourcebuffer5[v][i].value;
				data.key = data.key | sourcebuffer6[v][i].key;
				data.value = data.value | sourcebuffer6[v][i].value;
				data.key = data.key | sourcebuffer7[v][i].key;
				data.value = data.value | sourcebuffer7[v][i].value;
				data.key = data.key | sourcebuffer8[v][i].key;
				data.value = data.value | sourcebuffer8[v][i].value;
				data.key = data.key | sourcebuffer9[v][i].key;
				data.value = data.value | sourcebuffer9[v][i].value;
				data.key = data.key | sourcebuffer10[v][i].key;
				data.value = data.value | sourcebuffer10[v][i].value;
				data.key = data.key | sourcebuffer11[v][i].key;
				data.value = data.value | sourcebuffer11[v][i].value;
			}
		#else 
			data.key = 
 sourcebuffer0[0][i].key 
| sourcebuffer0[1][i].key 
| sourcebuffer0[2][i].key 
| sourcebuffer0[3][i].key 
| sourcebuffer0[4][i].key 
| sourcebuffer0[5][i].key 
| sourcebuffer0[6][i].key 
| sourcebuffer0[7][i].key 
| sourcebuffer1[0][i].key 
| sourcebuffer1[1][i].key 
| sourcebuffer1[2][i].key 
| sourcebuffer1[3][i].key 
| sourcebuffer1[4][i].key 
| sourcebuffer1[5][i].key 
| sourcebuffer1[6][i].key 
| sourcebuffer1[7][i].key 
| sourcebuffer2[0][i].key 
| sourcebuffer2[1][i].key 
| sourcebuffer2[2][i].key 
| sourcebuffer2[3][i].key 
| sourcebuffer2[4][i].key 
| sourcebuffer2[5][i].key 
| sourcebuffer2[6][i].key 
| sourcebuffer2[7][i].key 
| sourcebuffer3[0][i].key 
| sourcebuffer3[1][i].key 
| sourcebuffer3[2][i].key 
| sourcebuffer3[3][i].key 
| sourcebuffer3[4][i].key 
| sourcebuffer3[5][i].key 
| sourcebuffer3[6][i].key 
| sourcebuffer3[7][i].key 
| sourcebuffer4[0][i].key 
| sourcebuffer4[1][i].key 
| sourcebuffer4[2][i].key 
| sourcebuffer4[3][i].key 
| sourcebuffer4[4][i].key 
| sourcebuffer4[5][i].key 
| sourcebuffer4[6][i].key 
| sourcebuffer4[7][i].key 
| sourcebuffer5[0][i].key 
| sourcebuffer5[1][i].key 
| sourcebuffer5[2][i].key 
| sourcebuffer5[3][i].key 
| sourcebuffer5[4][i].key 
| sourcebuffer5[5][i].key 
| sourcebuffer5[6][i].key 
| sourcebuffer5[7][i].key 
| sourcebuffer6[0][i].key 
| sourcebuffer6[1][i].key 
| sourcebuffer6[2][i].key 
| sourcebuffer6[3][i].key 
| sourcebuffer6[4][i].key 
| sourcebuffer6[5][i].key 
| sourcebuffer6[6][i].key 
| sourcebuffer6[7][i].key 
| sourcebuffer7[0][i].key 
| sourcebuffer7[1][i].key 
| sourcebuffer7[2][i].key 
| sourcebuffer7[3][i].key 
| sourcebuffer7[4][i].key 
| sourcebuffer7[5][i].key 
| sourcebuffer7[6][i].key 
| sourcebuffer7[7][i].key 
| sourcebuffer8[0][i].key 
| sourcebuffer8[1][i].key 
| sourcebuffer8[2][i].key 
| sourcebuffer8[3][i].key 
| sourcebuffer8[4][i].key 
| sourcebuffer8[5][i].key 
| sourcebuffer8[6][i].key 
| sourcebuffer8[7][i].key 
| sourcebuffer9[0][i].key 
| sourcebuffer9[1][i].key 
| sourcebuffer9[2][i].key 
| sourcebuffer9[3][i].key 
| sourcebuffer9[4][i].key 
| sourcebuffer9[5][i].key 
| sourcebuffer9[6][i].key 
| sourcebuffer9[7][i].key 
| sourcebuffer10[0][i].key 
| sourcebuffer10[1][i].key 
| sourcebuffer10[2][i].key 
| sourcebuffer10[3][i].key 
| sourcebuffer10[4][i].key 
| sourcebuffer10[5][i].key 
| sourcebuffer10[6][i].key 
| sourcebuffer10[7][i].key 
| sourcebuffer11[0][i].key 
| sourcebuffer11[1][i].key 
| sourcebuffer11[2][i].key 
| sourcebuffer11[3][i].key 
| sourcebuffer11[4][i].key 
| sourcebuffer11[5][i].key 
| sourcebuffer11[6][i].key 
| sourcebuffer11[7][i].key 
			;
			
			data.value = 
 sourcebuffer0[0][i].value 
| sourcebuffer0[1][i].value 
| sourcebuffer0[2][i].value 
| sourcebuffer0[3][i].value 
| sourcebuffer0[4][i].value 
| sourcebuffer0[5][i].value 
| sourcebuffer0[6][i].value 
| sourcebuffer0[7][i].value 
| sourcebuffer1[0][i].value 
| sourcebuffer1[1][i].value 
| sourcebuffer1[2][i].value 
| sourcebuffer1[3][i].value 
| sourcebuffer1[4][i].value 
| sourcebuffer1[5][i].value 
| sourcebuffer1[6][i].value 
| sourcebuffer1[7][i].value 
| sourcebuffer2[0][i].value 
| sourcebuffer2[1][i].value 
| sourcebuffer2[2][i].value 
| sourcebuffer2[3][i].value 
| sourcebuffer2[4][i].value 
| sourcebuffer2[5][i].value 
| sourcebuffer2[6][i].value 
| sourcebuffer2[7][i].value 
| sourcebuffer3[0][i].value 
| sourcebuffer3[1][i].value 
| sourcebuffer3[2][i].value 
| sourcebuffer3[3][i].value 
| sourcebuffer3[4][i].value 
| sourcebuffer3[5][i].value 
| sourcebuffer3[6][i].value 
| sourcebuffer3[7][i].value 
| sourcebuffer4[0][i].value 
| sourcebuffer4[1][i].value 
| sourcebuffer4[2][i].value 
| sourcebuffer4[3][i].value 
| sourcebuffer4[4][i].value 
| sourcebuffer4[5][i].value 
| sourcebuffer4[6][i].value 
| sourcebuffer4[7][i].value 
| sourcebuffer5[0][i].value 
| sourcebuffer5[1][i].value 
| sourcebuffer5[2][i].value 
| sourcebuffer5[3][i].value 
| sourcebuffer5[4][i].value 
| sourcebuffer5[5][i].value 
| sourcebuffer5[6][i].value 
| sourcebuffer5[7][i].value 
| sourcebuffer6[0][i].value 
| sourcebuffer6[1][i].value 
| sourcebuffer6[2][i].value 
| sourcebuffer6[3][i].value 
| sourcebuffer6[4][i].value 
| sourcebuffer6[5][i].value 
| sourcebuffer6[6][i].value 
| sourcebuffer6[7][i].value 
| sourcebuffer7[0][i].value 
| sourcebuffer7[1][i].value 
| sourcebuffer7[2][i].value 
| sourcebuffer7[3][i].value 
| sourcebuffer7[4][i].value 
| sourcebuffer7[5][i].value 
| sourcebuffer7[6][i].value 
| sourcebuffer7[7][i].value 
| sourcebuffer8[0][i].value 
| sourcebuffer8[1][i].value 
| sourcebuffer8[2][i].value 
| sourcebuffer8[3][i].value 
| sourcebuffer8[4][i].value 
| sourcebuffer8[5][i].value 
| sourcebuffer8[6][i].value 
| sourcebuffer8[7][i].value 
| sourcebuffer9[0][i].value 
| sourcebuffer9[1][i].value 
| sourcebuffer9[2][i].value 
| sourcebuffer9[3][i].value 
| sourcebuffer9[4][i].value 
| sourcebuffer9[5][i].value 
| sourcebuffer9[6][i].value 
| sourcebuffer9[7][i].value 
| sourcebuffer10[0][i].value 
| sourcebuffer10[1][i].value 
| sourcebuffer10[2][i].value 
| sourcebuffer10[3][i].value 
| sourcebuffer10[4][i].value 
| sourcebuffer10[5][i].value 
| sourcebuffer10[6][i].value 
| sourcebuffer10[7][i].value 
| sourcebuffer11[0][i].value 
| sourcebuffer11[1][i].value 
| sourcebuffer11[2][i].value 
| sourcebuffer11[3][i].value 
| sourcebuffer11[4][i].value 
| sourcebuffer11[5][i].value 
| sourcebuffer11[6][i].value 
| sourcebuffer11[7][i].value 
			;
		#endif 
		
		if(data.key != 0 || data.value != 0){
			for(unsigned int t = 0; t < 64; t+=2){
				unsigned int vid = (sweepparams.source_partition * globalparams.applyvertexbuffersz) + i*32 + t/2; // CRITICAL FIXME. use less logic.
				unsigned int index = 64 - t - 2;
				unsigned int bitsize = 2;
				unsigned int vdata = READFROM_ULONG(data, index, bitsize);
				
				if(vdata == VISITED_IN_CURRENT_ITERATION){
					#ifdef _DEBUGMODE_KERNELPRINTS
					cout<<"unifydata_bfs: ACTIVE VERTICES seen for next iteration. vid: "<<vid<<endl;
					#endif
					#ifdef _DEBUGMODE_CHECKS2
					actsutilityobj->checkoutofbounds("unifydata_bfs.actvv loc", (actvvstravstate.k * VECTOR_SIZE) + actvvstravstate.v, PADDEDDESTBUFFER_SIZE*VECTOR_SIZE, actvvstravstate.v, actvvstravstate.k, NAp);
					actsutilityobj->checkoutofbounds("unifydata_bfs.actvvstravstate.v", actvvstravstate.v, VECTOR_SIZE, NAp, NAp, NAp);
					actsutilityobj->checkoutofbounds("unifydata_bfs.actvvstravstate.k", actvvstravstate.k, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
					#endif
					actvvs[actvvstravstate.v][actvvstravstate.k].key = vid;
					actvvs[actvvstravstate.v][actvvstravstate.k].value = NAp;
					actvvstravstate.v+=1; if(actvvstravstate.v == VECTOR_SIZE){ actvvstravstate.v=0; actvvstravstate.k+=1; }
					WRITETO_ULONG(&data, index, bitsize, VISITED_IN_PAST_ITERATION);
				}
			}
		}
		
		if(actvvstravstate.k >= PADDEDDESTBUFFER_SIZE-32){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"unifydata_bfs: saving and clearning actvvs... i: "<<actvvstravstate.i<<", i_kvs: "<<actvvstravstate.i_kvs<<", (v: "<<actvvstravstate.v<<", k: "<<actvvstravstate.k<<")"<<endl;
			#endif
			savekeyvalues_sync(ON, kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9,kvdram10,kvdram11, actvvs, globalparams.baseoffset_activevertices_kvs + actvvstravstate.i_kvs, actvvstravstate.k);

			actvvstravstate.i += actvvstravstate.k * VECTOR_SIZE;
			actvvstravstate.i_kvs += actvvstravstate.k;
			for(vector_type v=0; v<VECTOR_SIZE; v++){
			#pragma HLS UNROLL
				actvvs[v][0] = actvvs[v][actvvstravstate.k];
			}
			actvvstravstate.v = actvvstravstate.v;
			actvvstravstate.k = 0;
		}
		
		destbuffer[dest_v][destoffset_kvs + dest_i] = data;
		dest_v+=1; if(dest_v == VECTOR_SIZE){ dest_v=0; dest_i+=1; }
	}
	return actvvstravstate;
}

bool_type 
	#ifdef SW 
	acts::
	#endif
unifydata_bfs_syn(bool_type enable, keyvalue_t sourcebuffer0[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer3[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer5[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer6[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer7[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer8[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer9[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer10[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer11[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
					keyvalue_t actvvs0[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t actvvscapsule0[8], buffer_type cutoffs0[VECTOR_SIZE],
					keyvalue_t actvvs1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t actvvscapsule1[8], buffer_type cutoffs1[VECTOR_SIZE],
					keyvalue_t actvvs2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t actvvscapsule2[8], buffer_type cutoffs2[VECTOR_SIZE],
					keyvalue_t actvvs3[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t actvvscapsule3[8], buffer_type cutoffs3[VECTOR_SIZE],
					buffer_type destoffset, buffer_type size, sweepparams_t sweepparams, globalparams_t globalparams){
	if(enable == OFF){ return OFF; }
	analysis_type analysis_loopcount = (APPLYVERTEXBUFFERSZ / VDATAPACKINGFACTOR) / 2;
	
	buffer_type dest_v = 0;
	buffer_type dest_i = 0;
	buffer_type destoffset_kvs = destoffset / VECTOR_SIZE;
	unsigned int nonzeroactvvsreturned = OFF;
	
	for(vector_type v=0; v<VECTOR_SIZE; v++){ 
	#pragma HLS UNROLL
		actvvscapsule0[v].key = 0; actvvscapsule0[v].value = 0; 
		actvvscapsule1[v].key = 0; actvvscapsule1[v].value = 0; 
		actvvscapsule2[v].key = 0; actvvscapsule2[v].value = 0; 
		actvvscapsule3[v].key = 0; actvvscapsule3[v].value = 0; 
	}
	
	UNIFYDATA_LOOP1: for(buffer_type i=0; i<size; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE II=2
		keyvalue_t data; data.key = 0; data.value = 0;
		keyvalue_t data0; data0.key = 0; data0.value = 0;
		keyvalue_t data1; data1.key = 0; data1.value = 0;
		keyvalue_t data2; data2.key = 0; data2.value = 0;
		keyvalue_t data3; data3.key = 0; data3.value = 0;
		keyvalue_t data4; data4.key = 0; data4.value = 0;
		keyvalue_t data5; data5.key = 0; data5.value = 0;
		keyvalue_t data6; data6.key = 0; data6.value = 0;
		keyvalue_t data7; data7.key = 0; data7.value = 0;
		keyvalue_t data8; data8.key = 0; data8.value = 0;
		keyvalue_t data9; data9.key = 0; data9.value = 0;
		keyvalue_t data10; data10.key = 0; data10.value = 0;
		keyvalue_t data11; data11.key = 0; data11.value = 0;
		
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("unifydata_bfs.i", i, PADDEDDESTBUFFER_SIZE, destoffset, destoffset_kvs, size);
		#endif
		
		data0.key =  sourcebuffer0[0][i].key | sourcebuffer0[1][i].key | sourcebuffer0[2][i].key | sourcebuffer0[3][i].key | sourcebuffer0[4][i].key | sourcebuffer0[5][i].key | sourcebuffer0[6][i].key | sourcebuffer0[7][i].key  ;
		data1.key =  sourcebuffer1[0][i].key | sourcebuffer1[1][i].key | sourcebuffer1[2][i].key | sourcebuffer1[3][i].key | sourcebuffer1[4][i].key | sourcebuffer1[5][i].key | sourcebuffer1[6][i].key | sourcebuffer1[7][i].key  ;
		data2.key =  sourcebuffer2[0][i].key | sourcebuffer2[1][i].key | sourcebuffer2[2][i].key | sourcebuffer2[3][i].key | sourcebuffer2[4][i].key | sourcebuffer2[5][i].key | sourcebuffer2[6][i].key | sourcebuffer2[7][i].key  ;
		data3.key =  sourcebuffer3[0][i].key | sourcebuffer3[1][i].key | sourcebuffer3[2][i].key | sourcebuffer3[3][i].key | sourcebuffer3[4][i].key | sourcebuffer3[5][i].key | sourcebuffer3[6][i].key | sourcebuffer3[7][i].key  ;
		data4.key =  sourcebuffer4[0][i].key | sourcebuffer4[1][i].key | sourcebuffer4[2][i].key | sourcebuffer4[3][i].key | sourcebuffer4[4][i].key | sourcebuffer4[5][i].key | sourcebuffer4[6][i].key | sourcebuffer4[7][i].key  ;
		data5.key =  sourcebuffer5[0][i].key | sourcebuffer5[1][i].key | sourcebuffer5[2][i].key | sourcebuffer5[3][i].key | sourcebuffer5[4][i].key | sourcebuffer5[5][i].key | sourcebuffer5[6][i].key | sourcebuffer5[7][i].key  ;
		data6.key =  sourcebuffer6[0][i].key | sourcebuffer6[1][i].key | sourcebuffer6[2][i].key | sourcebuffer6[3][i].key | sourcebuffer6[4][i].key | sourcebuffer6[5][i].key | sourcebuffer6[6][i].key | sourcebuffer6[7][i].key  ;
		data7.key =  sourcebuffer7[0][i].key | sourcebuffer7[1][i].key | sourcebuffer7[2][i].key | sourcebuffer7[3][i].key | sourcebuffer7[4][i].key | sourcebuffer7[5][i].key | sourcebuffer7[6][i].key | sourcebuffer7[7][i].key  ;
		data8.key =  sourcebuffer8[0][i].key | sourcebuffer8[1][i].key | sourcebuffer8[2][i].key | sourcebuffer8[3][i].key | sourcebuffer8[4][i].key | sourcebuffer8[5][i].key | sourcebuffer8[6][i].key | sourcebuffer8[7][i].key  ;
		data9.key =  sourcebuffer9[0][i].key | sourcebuffer9[1][i].key | sourcebuffer9[2][i].key | sourcebuffer9[3][i].key | sourcebuffer9[4][i].key | sourcebuffer9[5][i].key | sourcebuffer9[6][i].key | sourcebuffer9[7][i].key  ;
		data10.key =  sourcebuffer10[0][i].key | sourcebuffer10[1][i].key | sourcebuffer10[2][i].key | sourcebuffer10[3][i].key | sourcebuffer10[4][i].key | sourcebuffer10[5][i].key | sourcebuffer10[6][i].key | sourcebuffer10[7][i].key  ;
		data11.key =  sourcebuffer11[0][i].key | sourcebuffer11[1][i].key | sourcebuffer11[2][i].key | sourcebuffer11[3][i].key | sourcebuffer11[4][i].key | sourcebuffer11[5][i].key | sourcebuffer11[6][i].key | sourcebuffer11[7][i].key  ;
		
		data0.value =  sourcebuffer0[0][i].value | sourcebuffer0[1][i].value | sourcebuffer0[2][i].value | sourcebuffer0[3][i].value | sourcebuffer0[4][i].value | sourcebuffer0[5][i].value | sourcebuffer0[6][i].value | sourcebuffer0[7][i].value  ;
		data1.value =  sourcebuffer1[0][i].value | sourcebuffer1[1][i].value | sourcebuffer1[2][i].value | sourcebuffer1[3][i].value | sourcebuffer1[4][i].value | sourcebuffer1[5][i].value | sourcebuffer1[6][i].value | sourcebuffer1[7][i].value  ;
		data2.value =  sourcebuffer2[0][i].value | sourcebuffer2[1][i].value | sourcebuffer2[2][i].value | sourcebuffer2[3][i].value | sourcebuffer2[4][i].value | sourcebuffer2[5][i].value | sourcebuffer2[6][i].value | sourcebuffer2[7][i].value  ;
		data3.value =  sourcebuffer3[0][i].value | sourcebuffer3[1][i].value | sourcebuffer3[2][i].value | sourcebuffer3[3][i].value | sourcebuffer3[4][i].value | sourcebuffer3[5][i].value | sourcebuffer3[6][i].value | sourcebuffer3[7][i].value  ;
		data4.value =  sourcebuffer4[0][i].value | sourcebuffer4[1][i].value | sourcebuffer4[2][i].value | sourcebuffer4[3][i].value | sourcebuffer4[4][i].value | sourcebuffer4[5][i].value | sourcebuffer4[6][i].value | sourcebuffer4[7][i].value  ;
		data5.value =  sourcebuffer5[0][i].value | sourcebuffer5[1][i].value | sourcebuffer5[2][i].value | sourcebuffer5[3][i].value | sourcebuffer5[4][i].value | sourcebuffer5[5][i].value | sourcebuffer5[6][i].value | sourcebuffer5[7][i].value  ;
		data6.value =  sourcebuffer6[0][i].value | sourcebuffer6[1][i].value | sourcebuffer6[2][i].value | sourcebuffer6[3][i].value | sourcebuffer6[4][i].value | sourcebuffer6[5][i].value | sourcebuffer6[6][i].value | sourcebuffer6[7][i].value  ;
		data7.value =  sourcebuffer7[0][i].value | sourcebuffer7[1][i].value | sourcebuffer7[2][i].value | sourcebuffer7[3][i].value | sourcebuffer7[4][i].value | sourcebuffer7[5][i].value | sourcebuffer7[6][i].value | sourcebuffer7[7][i].value  ;
		data8.value =  sourcebuffer8[0][i].value | sourcebuffer8[1][i].value | sourcebuffer8[2][i].value | sourcebuffer8[3][i].value | sourcebuffer8[4][i].value | sourcebuffer8[5][i].value | sourcebuffer8[6][i].value | sourcebuffer8[7][i].value  ;
		data9.value =  sourcebuffer9[0][i].value | sourcebuffer9[1][i].value | sourcebuffer9[2][i].value | sourcebuffer9[3][i].value | sourcebuffer9[4][i].value | sourcebuffer9[5][i].value | sourcebuffer9[6][i].value | sourcebuffer9[7][i].value  ;
		data10.value =  sourcebuffer10[0][i].value | sourcebuffer10[1][i].value | sourcebuffer10[2][i].value | sourcebuffer10[3][i].value | sourcebuffer10[4][i].value | sourcebuffer10[5][i].value | sourcebuffer10[6][i].value | sourcebuffer10[7][i].value  ;
		data11.value =  sourcebuffer11[0][i].value | sourcebuffer11[1][i].value | sourcebuffer11[2][i].value | sourcebuffer11[3][i].value | sourcebuffer11[4][i].value | sourcebuffer11[5][i].value | sourcebuffer11[6][i].value | sourcebuffer11[7][i].value  ;
		
		data.key =  data0.key | data1.key | data2.key | data3.key | data4.key | data5.key | data6.key | data7.key | data8.key | data9.key | data10.key | data11.key  ;
		data.value =  data0.value | data1.value | data2.value | data3.value | data4.value | data5.value | data6.value | data7.value | data8.value | data9.value | data10.value | data11.value  ;
		
		#ifdef _WIDEWORD
		ulong_dt longdata = CONVERTTOLONG_KV(data);
		#endif 
		
		if(data.key != 0 || data.value != 0){
			UNIFYDATA_LOOP1B: for(unsigned int t = 0; t < 32; t+=4){
			#pragma HLS UNROLL
				nonzeroactvvsreturned = ON;
				
				unsigned int voffset = sweepparams.source_partition * globalparams.applyvertexbuffersz;
				unsigned int h = t/4;

				unsigned int vid0 = voffset + i*32 + (t+0);
				unsigned int vid1 = voffset + i*32 + (t+1);
				unsigned int vid2 = voffset + i*32 + (t+2);
				unsigned int vid3 = voffset + i*32 + (t+3);
				unsigned int j0 = 2*t;
				#ifdef _WIDEWORD
				unsigned int vdata0 = longdata.range(j0 + ((2*0)+2-1), j0 + (2*0)); 
				unsigned int vdata1 = longdata.range(j0 + ((2*1)+2-1), j0 + (2*1)); 
				unsigned int vdata2 = longdata.range(j0 + ((2*2)+2-1), j0 + (2*2)); 
				unsigned int vdata3 = longdata.range(j0 + ((2*3)+2-1), j0 + (2*3)); 
				#else 
				unsigned int vdata0 = READFROM_ULONG(data, j0 + (2*0), 2); 
				unsigned int vdata1 = READFROM_ULONG(data, j0 + (2*1), 2); 
				unsigned int vdata2 = READFROM_ULONG(data, j0 + (2*2), 2); 
				unsigned int vdata3 = READFROM_ULONG(data, j0 + (2*3), 2); 
				#endif 
		
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"unifydata_bfs_syn: ACTIVE VERTICES seen for next iteration. t: "<<t<<", vid0: "<<vid0<<", vid1: "<<vid1<<", vid2: "<<vid2<<", vid3: "<<vid3<<endl;
				cout<<"unifydata_bfs_syn: ACTIVE VERTICES seen for next iteration. t: "<<t<<", vdata0: "<<vdata0<<", vdata1: "<<vdata1<<", vdata2: "<<vdata2<<", vdata3: "<<vdata3<<endl;
				#endif
				
				// FIXME. actvvs.runpipeline_1partition
				if(vdata0 == VISITED_IN_CURRENT_ITERATION){ 
					actvvs0[h][actvvscapsule0[h].value].key = vid0; 
					actvvs0[h][actvvscapsule0[h].value].value = 777; 
				}
				if(vdata1 == VISITED_IN_CURRENT_ITERATION){ 
					actvvs1[h][actvvscapsule1[h].value].key = vid1; 
					actvvs1[h][actvvscapsule1[h].value].value = 777; 
				}
				if(vdata2 == VISITED_IN_CURRENT_ITERATION){ 
					actvvs2[h][actvvscapsule2[h].value].key = vid2; 
					actvvs2[h][actvvscapsule2[h].value].value = 777; 
				}
				if(vdata3 == VISITED_IN_CURRENT_ITERATION){ 
					actvvs3[h][actvvscapsule3[h].value].key = vid3; 
					actvvs3[h][actvvscapsule3[h].value].value = 777; 
				}
				if(vdata0 == VISITED_IN_CURRENT_ITERATION){ actvvscapsule0[h].value++; }
				if(vdata1 == VISITED_IN_CURRENT_ITERATION){ actvvscapsule1[h].value++; }
				if(vdata2 == VISITED_IN_CURRENT_ITERATION){ actvvscapsule2[h].value++; }
				if(vdata3 == VISITED_IN_CURRENT_ITERATION){ actvvscapsule3[h].value++; }
				if(vdata0 == VISITED_IN_CURRENT_ITERATION){ WRITETO_ULONG(&data, (j0+(2*0)), 2, VISITED_IN_PAST_ITERATION); }
				if(vdata1 == VISITED_IN_CURRENT_ITERATION){ WRITETO_ULONG(&data, (j0+(2*1)), 2, VISITED_IN_PAST_ITERATION); }
				if(vdata2 == VISITED_IN_CURRENT_ITERATION){ WRITETO_ULONG(&data, (j0+(2*2)), 2, VISITED_IN_PAST_ITERATION); }
				if(vdata3 == VISITED_IN_CURRENT_ITERATION){ WRITETO_ULONG(&data, (j0+(2*3)), 2, VISITED_IN_PAST_ITERATION); }
				
			}
		}
		
		destbuffer[dest_v][destoffset_kvs + dest_i] = data;
		dest_v+=1; if(dest_v == VECTOR_SIZE){ dest_v=0; dest_i+=1; }
	}
	
	// FIXME. actvvs.runpipeline_1partition
	keyvalue_t dummykv;
	dummykv.key = INVALIDDATA;
	dummykv.value = INVALIDDATA;
	UNIFYDATA_LOOP2: for(vector_type k=0; k<4; k++){
		for(vector_type v=0; v<VECTOR_SIZE; v++){
		#pragma HLS UNROLL
			if(actvvscapsule0[v].value > 0 && (actvvscapsule0[v].value % 4) > 0){ // CRITICAL FIXME. can use less logic.
				if(k >= actvvscapsule0[v].value % 4){
					actvvs0[v][actvvscapsule0[v].value] = dummykv;
					actvvscapsule0[v].value++;
				}
			}
			if(actvvscapsule1[v].value > 0 && (actvvscapsule1[v].value % 4) > 0){ // CRITICAL FIXME. can use less logic.
				if(k >= actvvscapsule1[v].value % 4){
					actvvs1[v][actvvscapsule1[v].value] = dummykv;
					actvvscapsule1[v].value++;
				}
			}
			if(actvvscapsule2[v].value > 0 && (actvvscapsule2[v].value % 4) > 0){ // CRITICAL FIXME. can use less logic.
				if(k >= actvvscapsule2[v].value % 4){
					actvvs2[v][actvvscapsule2[v].value] = dummykv;
					actvvscapsule2[v].value++;
				}
			}
			if(actvvscapsule3[v].value > 0 && (actvvscapsule3[v].value % 4) > 0){ // CRITICAL FIXME. can use less logic.
				if(k >= actvvscapsule3[v].value % 4){
					actvvs3[v][actvvscapsule3[v].value] = dummykv;
					actvvscapsule3[v].value++;
				}
			}
		}
	}
	UNIFYDATA_LOOP3: for(vector_type v=0; v<VECTOR_SIZE; v++){ 
	#pragma HLS UNROLL
		cutoffs0[v] = actvvscapsule0[v].value;
		cutoffs1[v] = actvvscapsule1[v].value;
		cutoffs2[v] = actvvscapsule2[v].value;
		cutoffs3[v] = actvvscapsule3[v].value;
	}
	return nonzeroactvvsreturned;
}

// apply functions 
void 
	#ifdef SW 
	acts::
	#endif
apply_bfs(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE]){
	if(enable == OFF){ return; }
	return;
}

// process-edges function [edge.key = dstvid, edge.value = src]
void 
	#ifdef SW 
	acts::
	#endif
process_edges(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], unsigned int upperlimit, unsigned int GraphIter, unsigned int GraphAlgo, travstate_t travstate, globalparams_t globalparams){
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
		
		value_t localsourceid0 = edge0.value - upperlimit;
		value_t localsourceid1 = edge1.value - upperlimit;
		value_t localsourceid2 = edge2.value - upperlimit;
		value_t localsourceid3 = edge3.value - upperlimit;
		value_t localsourceid4 = edge4.value - upperlimit;
		value_t localsourceid5 = edge5.value - upperlimit;
		value_t localsourceid6 = edge6.value - upperlimit;
		value_t localsourceid7 = edge7.value - upperlimit;
		
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"PROCESSEDGE SEEN @ process_edges:: i: "<<i<<", destbuffer[0][i].key: "<<destbuffer[0][i].key<<", destbuffer[0][i].value: "<<destbuffer[0][i].value<<", upperlimit: "<<upperlimit<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
		cout<<"PROCESSEDGE SEEN @ process_edges:: i: "<<i<<", destbuffer[1][i].key: "<<destbuffer[1][i].key<<", destbuffer[1][i].value: "<<destbuffer[1][i].value<<", upperlimit: "<<upperlimit<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
		cout<<"PROCESSEDGE SEEN @ process_edges:: i: "<<i<<", destbuffer[2][i].key: "<<destbuffer[2][i].key<<", destbuffer[2][i].value: "<<destbuffer[2][i].value<<", upperlimit: "<<upperlimit<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
		cout<<"PROCESSEDGE SEEN @ process_edges:: i: "<<i<<", destbuffer[3][i].key: "<<destbuffer[3][i].key<<", destbuffer[3][i].value: "<<destbuffer[3][i].value<<", upperlimit: "<<upperlimit<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
		cout<<"PROCESSEDGE SEEN @ process_edges:: i: "<<i<<", destbuffer[4][i].key: "<<destbuffer[4][i].key<<", destbuffer[4][i].value: "<<destbuffer[4][i].value<<", upperlimit: "<<upperlimit<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
		cout<<"PROCESSEDGE SEEN @ process_edges:: i: "<<i<<", destbuffer[5][i].key: "<<destbuffer[5][i].key<<", destbuffer[5][i].value: "<<destbuffer[5][i].value<<", upperlimit: "<<upperlimit<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
		cout<<"PROCESSEDGE SEEN @ process_edges:: i: "<<i<<", destbuffer[6][i].key: "<<destbuffer[6][i].key<<", destbuffer[6][i].value: "<<destbuffer[6][i].value<<", upperlimit: "<<upperlimit<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
		cout<<"PROCESSEDGE SEEN @ process_edges:: i: "<<i<<", destbuffer[7][i].key: "<<destbuffer[7][i].key<<", destbuffer[7][i].value: "<<destbuffer[7][i].value<<", upperlimit: "<<upperlimit<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
	
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
			vertexupdate0.key = edge0.key;
			vertexupdate0.value = processedgefunc(sourcebuffer[0][localsourceid0].value, 1, 1, globalparams.GraphIter, globalparams.GraphAlgo); }
		keyvalue_t vertexupdate1;
		if(localsourceid1 < PADDEDDESTBUFFER_SIZE){
			vertexupdate1.key = edge1.key;
			vertexupdate1.value = processedgefunc(sourcebuffer[1][localsourceid1].value, 1, 1, globalparams.GraphIter, globalparams.GraphAlgo); }
		keyvalue_t vertexupdate2;
		if(localsourceid2 < PADDEDDESTBUFFER_SIZE){
			vertexupdate2.key = edge2.key;
			vertexupdate2.value = processedgefunc(sourcebuffer[2][localsourceid2].value, 1, 1, globalparams.GraphIter, globalparams.GraphAlgo); }
		keyvalue_t vertexupdate3;
		if(localsourceid3 < PADDEDDESTBUFFER_SIZE){
			vertexupdate3.key = edge3.key;
			vertexupdate3.value = processedgefunc(sourcebuffer[3][localsourceid3].value, 1, 1, globalparams.GraphIter, globalparams.GraphAlgo); }
		keyvalue_t vertexupdate4;
		if(localsourceid4 < PADDEDDESTBUFFER_SIZE){
			vertexupdate4.key = edge4.key;
			vertexupdate4.value = processedgefunc(sourcebuffer[4][localsourceid4].value, 1, 1, globalparams.GraphIter, globalparams.GraphAlgo); }
		keyvalue_t vertexupdate5;
		if(localsourceid5 < PADDEDDESTBUFFER_SIZE){
			vertexupdate5.key = edge5.key;
			vertexupdate5.value = processedgefunc(sourcebuffer[5][localsourceid5].value, 1, 1, globalparams.GraphIter, globalparams.GraphAlgo); }
		keyvalue_t vertexupdate6;
		if(localsourceid6 < PADDEDDESTBUFFER_SIZE){
			vertexupdate6.key = edge6.key;
			vertexupdate6.value = processedgefunc(sourcebuffer[6][localsourceid6].value, 1, 1, globalparams.GraphIter, globalparams.GraphAlgo); }
		keyvalue_t vertexupdate7;
		if(localsourceid7 < PADDEDDESTBUFFER_SIZE){
			vertexupdate7.key = edge7.key;
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
	acts::
	#endif
process_edges(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t statsbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], unsigned int upperlimit, unsigned int GraphIter, unsigned int GraphAlgo, travstate_t travstate, globalparams_t globalparams){
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
		
		value_t localsourceid0 = edge0.value - upperlimit; 
		value_t localsourceid1 = edge1.value - upperlimit; 
		value_t localsourceid2 = edge2.value - upperlimit; 
		value_t localsourceid3 = edge3.value - upperlimit; 
		value_t localsourceid4 = edge4.value - upperlimit; 
		value_t localsourceid5 = edge5.value - upperlimit; 
		value_t localsourceid6 = edge6.value - upperlimit; 
		value_t localsourceid7 = edge7.value - upperlimit; 
		
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"PROCESSEDGE SEEN @ process_edges:: i: "<<i<<", destbuffer[0][i].key: "<<destbuffer[0][i].key<<", destbuffer[0][i].value: "<<destbuffer[0][i].value<<", upperlimit: "<<upperlimit<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
		cout<<"PROCESSEDGE SEEN @ process_edges:: i: "<<i<<", destbuffer[1][i].key: "<<destbuffer[1][i].key<<", destbuffer[1][i].value: "<<destbuffer[1][i].value<<", upperlimit: "<<upperlimit<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
		cout<<"PROCESSEDGE SEEN @ process_edges:: i: "<<i<<", destbuffer[2][i].key: "<<destbuffer[2][i].key<<", destbuffer[2][i].value: "<<destbuffer[2][i].value<<", upperlimit: "<<upperlimit<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
		cout<<"PROCESSEDGE SEEN @ process_edges:: i: "<<i<<", destbuffer[3][i].key: "<<destbuffer[3][i].key<<", destbuffer[3][i].value: "<<destbuffer[3][i].value<<", upperlimit: "<<upperlimit<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
		cout<<"PROCESSEDGE SEEN @ process_edges:: i: "<<i<<", destbuffer[4][i].key: "<<destbuffer[4][i].key<<", destbuffer[4][i].value: "<<destbuffer[4][i].value<<", upperlimit: "<<upperlimit<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
		cout<<"PROCESSEDGE SEEN @ process_edges:: i: "<<i<<", destbuffer[5][i].key: "<<destbuffer[5][i].key<<", destbuffer[5][i].value: "<<destbuffer[5][i].value<<", upperlimit: "<<upperlimit<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
		cout<<"PROCESSEDGE SEEN @ process_edges:: i: "<<i<<", destbuffer[6][i].key: "<<destbuffer[6][i].key<<", destbuffer[6][i].value: "<<destbuffer[6][i].value<<", upperlimit: "<<upperlimit<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
		cout<<"PROCESSEDGE SEEN @ process_edges:: i: "<<i<<", destbuffer[7][i].key: "<<destbuffer[7][i].key<<", destbuffer[7][i].value: "<<destbuffer[7][i].value<<", upperlimit: "<<upperlimit<<", PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl; 
	
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
			vertexupdate0.key = edge0.key;
			vertexupdate0.value = processedgefunc(sourcebuffer[0][localsourceid0].value, 1, 1, globalparams.GraphIter, globalparams.GraphAlgo); }
		keyvalue_t vertexupdate1;
		if(localsourceid1 < PADDEDDESTBUFFER_SIZE){
			vertexupdate1.key = edge1.key;
			vertexupdate1.value = processedgefunc(sourcebuffer[1][localsourceid1].value, 1, 1, globalparams.GraphIter, globalparams.GraphAlgo); }
		keyvalue_t vertexupdate2;
		if(localsourceid2 < PADDEDDESTBUFFER_SIZE){
			vertexupdate2.key = edge2.key;
			vertexupdate2.value = processedgefunc(sourcebuffer[2][localsourceid2].value, 1, 1, globalparams.GraphIter, globalparams.GraphAlgo); }
		keyvalue_t vertexupdate3;
		if(localsourceid3 < PADDEDDESTBUFFER_SIZE){
			vertexupdate3.key = edge3.key;
			vertexupdate3.value = processedgefunc(sourcebuffer[3][localsourceid3].value, 1, 1, globalparams.GraphIter, globalparams.GraphAlgo); }
		keyvalue_t vertexupdate4;
		if(localsourceid4 < PADDEDDESTBUFFER_SIZE){
			vertexupdate4.key = edge4.key;
			vertexupdate4.value = processedgefunc(sourcebuffer[4][localsourceid4].value, 1, 1, globalparams.GraphIter, globalparams.GraphAlgo); }
		keyvalue_t vertexupdate5;
		if(localsourceid5 < PADDEDDESTBUFFER_SIZE){
			vertexupdate5.key = edge5.key;
			vertexupdate5.value = processedgefunc(sourcebuffer[5][localsourceid5].value, 1, 1, globalparams.GraphIter, globalparams.GraphAlgo); }
		keyvalue_t vertexupdate6;
		if(localsourceid6 < PADDEDDESTBUFFER_SIZE){
			vertexupdate6.key = edge6.key;
			vertexupdate6.value = processedgefunc(sourcebuffer[6][localsourceid6].value, 1, 1, globalparams.GraphIter, globalparams.GraphAlgo); }
		keyvalue_t vertexupdate7;
		if(localsourceid7 < PADDEDDESTBUFFER_SIZE){
			vertexupdate7.key = edge7.key;
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
		if(localsourceid0 < PADDEDDESTBUFFER_SIZE){ p0 = getglobalpartition(vertexupdate0, globalparams.destvoffset, globalparams.batch_range_pow); } // CRITICAL FIXME. upper not always 0
		partition_type p1 = 0;
		if(localsourceid1 < PADDEDDESTBUFFER_SIZE){ p1 = getglobalpartition(vertexupdate1, globalparams.destvoffset, globalparams.batch_range_pow); } // CRITICAL FIXME. upper not always 0
		partition_type p2 = 0;
		if(localsourceid2 < PADDEDDESTBUFFER_SIZE){ p2 = getglobalpartition(vertexupdate2, globalparams.destvoffset, globalparams.batch_range_pow); } // CRITICAL FIXME. upper not always 0
		partition_type p3 = 0;
		if(localsourceid3 < PADDEDDESTBUFFER_SIZE){ p3 = getglobalpartition(vertexupdate3, globalparams.destvoffset, globalparams.batch_range_pow); } // CRITICAL FIXME. upper not always 0
		partition_type p4 = 0;
		if(localsourceid4 < PADDEDDESTBUFFER_SIZE){ p4 = getglobalpartition(vertexupdate4, globalparams.destvoffset, globalparams.batch_range_pow); } // CRITICAL FIXME. upper not always 0
		partition_type p5 = 0;
		if(localsourceid5 < PADDEDDESTBUFFER_SIZE){ p5 = getglobalpartition(vertexupdate5, globalparams.destvoffset, globalparams.batch_range_pow); } // CRITICAL FIXME. upper not always 0
		partition_type p6 = 0;
		if(localsourceid6 < PADDEDDESTBUFFER_SIZE){ p6 = getglobalpartition(vertexupdate6, globalparams.destvoffset, globalparams.batch_range_pow); } // CRITICAL FIXME. upper not always 0
		partition_type p7 = 0;
		if(localsourceid7 < PADDEDDESTBUFFER_SIZE){ p7 = getglobalpartition(vertexupdate7, globalparams.destvoffset, globalparams.batch_range_pow); } // CRITICAL FIXME. upper not always 0
		
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

// in-memory partition functions (1)
void 
	#ifdef SW 
	acts::
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
	acts::
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
	acts::
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
	acts::
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
	acts::
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
	acts::
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
	acts::
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
	acts::
	#endif
combineSetof1stoSetof2s(bool_type enable, keyvalue_t buffer_setof1[8][PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2[8][PADDEDDESTBUFFER_SIZE], skeyvalue_t templocalcapsule_so1[8][NUM_PARTITIONS], skeyvalue_t templocalcapsule_so2[4][NUM_PARTITIONS], globalparams_t globalparams){
	// 1s->2s
	combineSetof1stoSetof2_I0(enable, buffer_setof1[0], buffer_setof1[1], buffer_setof2[0], buffer_setof2[1], templocalcapsule_so1[0], templocalcapsule_so1[1], templocalcapsule_so2[0], globalparams);
	combineSetof1stoSetof2_I1(enable, buffer_setof1[2], buffer_setof1[3], buffer_setof2[2], buffer_setof2[3], templocalcapsule_so1[2], templocalcapsule_so1[3], templocalcapsule_so2[1], globalparams);
	combineSetof1stoSetof2_I2(enable, buffer_setof1[4], buffer_setof1[5], buffer_setof2[4], buffer_setof2[5], templocalcapsule_so1[4], templocalcapsule_so1[5], templocalcapsule_so2[2], globalparams);
	combineSetof1stoSetof2_I3(enable, buffer_setof1[6], buffer_setof1[7], buffer_setof2[6], buffer_setof2[7], templocalcapsule_so1[6], templocalcapsule_so1[7], templocalcapsule_so2[3], globalparams);
	return;
}
void 
	#ifdef SW 
	acts::
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
	acts::
	#endif
combineSetof4stoSetof8s(bool_type enable, keyvalue_t buffer_setof4[8][PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof8[8][PADDEDDESTBUFFER_SIZE], skeyvalue_t templocalcapsule_so4[2][NUM_PARTITIONS], skeyvalue_t templocalcapsule_so8[NUM_PARTITIONS], globalparams_t globalparams){
	// #pragma HLS INLINE
	// 4s->8s
	combineSetof4stoSetof8_I0(enable, buffer_setof4[0], buffer_setof4[1], buffer_setof4[2], buffer_setof4[3], buffer_setof4[4], buffer_setof4[5], buffer_setof4[6], buffer_setof4[7], 
												buffer_setof8[0], buffer_setof8[1], buffer_setof8[2], buffer_setof8[3], buffer_setof8[4], buffer_setof8[5], buffer_setof8[6], buffer_setof8[7], 
													templocalcapsule_so4[0], templocalcapsule_so4[1], templocalcapsule_so8, globalparams);
	return;
}

// pipelined partition functions
void 
	#ifdef SW 
	acts::
	#endif
runpipeline(bool_type enable, keyvalue_t bufferA[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t buffer1capsule[VECTOR_SIZE][NUM_PARTITIONS], 
				keyvalue_t bufferB[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t bufferBcapsule[4][NUM_PARTITIONS], 
					keyvalue_t bufferC[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t bufferCcapsule[2][NUM_PARTITIONS],
						keyvalue_t bufferD[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t bufferDcapsule[NUM_PARTITIONS],
							unsigned int currentLOP, sweepparams_t sweepparams, buffer_type cutoff, buffer_type cutoffs[VECTOR_SIZE], batch_type shiftcount, globalparams_t globalparams){		
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE;
	if(enable == OFF){ return; }
	
	unsigned int upperlimit = sweepparams.upperlimit;
	unsigned int upperpartition = sweepparams.upperpartition;
	
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
	
	buffer_type tempcutoffs[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=tempcutoffs complete
	
	bool_type enablebufferA = ON;
	bool_type enablebufferB = ON;
	bool_type enablebufferC = ON;
	bool_type enablebufferD = ON;
	#ifdef OPTMZ
	if(shiftcount >= 1){ enablebufferB = ON; } else { enablebufferB = OFF; }
	if(shiftcount >= 2){ enablebufferC = ON; } else { enablebufferC = OFF; }
	if(shiftcount >= 3){ enablebufferD = ON; } else { enablebufferD = OFF; }
	#endif 
	
	#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
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
	
	bufferDcapsule[0].key = 0;
	for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=2
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

		#if defined(_DEBUGMODE_CHECKS2) && defined(SW)
		if(p<NUM_PARTITIONS-1){ actsutilityobj->checkoutofbounds(ON, "yyy runp: bufferDcapsule[" + std::to_string(p+1) + "].key", bufferDcapsule[p+1].key, (PADDEDDESTBUFFER_SIZE * VECTOR_SIZE) + 1, NAp, NAp, NAp); }
		
		actsutilityobj->checkoutofbounds(ON, "runp: bufferCcapsule[0][" + std::to_string(p) + "].key", bufferCcapsule[0][p].key, SRCBUFFER_SIZE+1, NAp, NAp, NAp);
		actsutilityobj->checkoutofbounds(ON, "runp: bufferCcapsule[1][" + std::to_string(p) + "].key", bufferCcapsule[1][p].key, SRCBUFFER_SIZE+1, NAp, NAp, NAp);
		
		actsutilityobj->checkoutofbounds(ON, "runp: bufferBcapsule[0][" + std::to_string(p) + "].key", bufferBcapsule[0][p].key, SRCBUFFER_SIZE+1, NAp, NAp, NAp);
		actsutilityobj->checkoutofbounds(ON, "runp: bufferBcapsule[1][" + std::to_string(p) + "].key", bufferBcapsule[1][p].key, SRCBUFFER_SIZE+1, NAp, NAp, NAp);
		actsutilityobj->checkoutofbounds(ON, "runp: bufferBcapsule[2][" + std::to_string(p) + "].key", bufferBcapsule[2][p].key, SRCBUFFER_SIZE+1, NAp, NAp, NAp);
		actsutilityobj->checkoutofbounds(ON, "runp: bufferBcapsule[3][" + std::to_string(p) + "].key", bufferBcapsule[3][p].key, SRCBUFFER_SIZE+1, NAp, NAp, NAp);
		#endif 
		#if defined(_DEBUGMODE_CHECKS2) && defined(SWEMU)
		if(p<NUM_PARTITIONS-1){ actsutilityobj->checkoutofbounds(ON, "runp: bufferDcapsule[p+1].key", bufferDcapsule[p+1].key, (PADDEDDESTBUFFER_SIZE * VECTOR_SIZE) + 1, NAp, NAp, NAp); }

		actsutilityobj->checkoutofbounds(ON, "runp: bufferCcapsule[0][p].key", bufferCcapsule[0][p].key, SRCBUFFER_SIZE+1, NAp, NAp, NAp);
		actsutilityobj->checkoutofbounds(ON, "runp: bufferCcapsule[1][p].key", bufferCcapsule[1][p].key, SRCBUFFER_SIZE+1, NAp, NAp, NAp);
		
		actsutilityobj->checkoutofbounds(ON, "runp: bufferBcapsule[0][p].key", bufferBcapsule[0][p].key, SRCBUFFER_SIZE+1, NAp, NAp, NAp);
		actsutilityobj->checkoutofbounds(ON, "runp: bufferBcapsule[1][p].key", bufferBcapsule[1][p].key, SRCBUFFER_SIZE+1, NAp, NAp, NAp);
		actsutilityobj->checkoutofbounds(ON, "runp: bufferBcapsule[2][p].key", bufferBcapsule[2][p].key, SRCBUFFER_SIZE+1, NAp, NAp, NAp);
		actsutilityobj->checkoutofbounds(ON, "runp: bufferBcapsule[3][p].key", bufferBcapsule[3][p].key, SRCBUFFER_SIZE+1, NAp, NAp, NAp);
		#endif 
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
	#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
	keyvalue_t CHK[4]; 
	keyvalue_t CHKK[8]; 
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int v=0; v<4; v++){ actsutilityobj->printkeyvalues("runpipeline.bufferBcapsule[v]", (keyvalue_t *)bufferBcapsule[v], NUM_PARTITIONS); }
	for(unsigned int v=0; v<2; v++){ actsutilityobj->printkeyvalues("runpipeline.bufferCcapsule[v]", (keyvalue_t *)bufferCcapsule[v], NUM_PARTITIONS); }
	for(unsigned int v=0; v<1; v++){ actsutilityobj->printkeyvalues("runpipeline.bufferDcapsule[v]", (keyvalue_t *)bufferDcapsule, NUM_PARTITIONS); }
	#endif 
	
	keyvalue_t dummykv;
	dummykv.key = 0; // (0 << (globalparams.batch_range_pow - (NUM_PARTITIONS_POW * currentLOP))) + upperlimit; // NEWCHANGE.
	dummykv.value = INVALIDDATA;
	
	value_t tempbufferDcapsule[NUM_PARTITIONS];
	#ifdef MAXPERFORMANCE
	#pragma HLS ARRAY_PARTITION variable=tempbufferDcapsule complete
	#endif

	for(partition_type p=0; p<NUM_PARTITIONS; p++){ 
	#pragma HLS PIPELINE II=1
		tempbufferDcapsule[p] = bufferDcapsule[p].value; 
	}
	for(vector_type v=0; v<VECTOR_SIZE; v++){ 
	#pragma HLS PIPELINE II=1
		tempcutoffs[v] = cutoffs[v]; 
	}
	unsigned int n=0;
	RUNPIPELINE_LOOP1: for(n=0; n<2; n++){
		RUNPIPELINE_LOOP1B: for(buffer_type k=0; k<SRCBUFFER_SIZE; k+=4){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
		#pragma HLS PIPELINE II=4
		
			#ifdef _DEBUGMODE_KERNELPRINTS
			if(k % 16 == 0){ cout<<"k: "<<k<<endl; }
			#endif
			
			#ifdef SIMPLEANDFASTPREPAREFUNC
			if(k < tempcutoffs[0+n]){ kvA0[0] = bufferA[0+n][k]; kvA0[1] = bufferA[0+n][k+1]; kvA0[2] = bufferA[0+n][k+2]; kvA0[3] = bufferA[0+n][k+3]; } 
			else { kvA0[0] = dummykv; kvA0[1] = dummykv; kvA0[2] = dummykv; kvA0[3] = dummykv; }
			if(k < tempcutoffs[2+n]){ kvA2[0] = bufferA[2+n][k]; kvA2[1] = bufferA[2+n][k+1]; kvA2[2] = bufferA[2+n][k+2]; kvA2[3] = bufferA[2+n][k+3]; } 
			else { kvA2[0] = dummykv; kvA2[1] = dummykv; kvA2[2] = dummykv; kvA2[3] = dummykv; }
			if(k < tempcutoffs[4+n]){ kvA4[0] = bufferA[4+n][k]; kvA4[1] = bufferA[4+n][k+1]; kvA4[2] = bufferA[4+n][k+2]; kvA4[3] = bufferA[4+n][k+3]; } 
			else { kvA4[0] = dummykv; kvA4[1] = dummykv; kvA4[2] = dummykv; kvA4[3] = dummykv; }
			if(k < tempcutoffs[6+n]){ kvA6[0] = bufferA[6+n][k]; kvA6[1] = bufferA[6+n][k+1]; kvA6[2] = bufferA[6+n][k+2]; kvA6[3] = bufferA[6+n][k+3]; } 
			else { kvA6[0] = dummykv; kvA6[1] = dummykv; kvA6[2] = dummykv; kvA6[3] = dummykv; }
			#else 
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
			#endif 
		
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
			
			#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
			actsutilityobj->checkn(enablebufferA, "kvA0", kvA0, currentLOP, upperlimit, globalparams.batch_range_pow, 4);
			actsutilityobj->checkn(enablebufferA, "kvA2", kvA2, currentLOP, upperlimit, globalparams.batch_range_pow, 4);
			actsutilityobj->checkn(enablebufferA, "kvA4", kvA4, currentLOP, upperlimit, globalparams.batch_range_pow, 4);
			actsutilityobj->checkn(enablebufferA, "kvA6", kvA6, currentLOP, upperlimit, globalparams.batch_range_pow, 4);
			#endif
			
			partition_type pA0 = getpartition(enablebufferA, kvA0[0], currentLOP, upperlimit, upperpartition, globalparams.batch_range_pow);
			partition_type pA2 = getpartition(enablebufferA, kvA2[0], currentLOP, upperlimit, upperpartition, globalparams.batch_range_pow);
			partition_type pA4 = getpartition(enablebufferA, kvA4[0], currentLOP, upperlimit, upperpartition, globalparams.batch_range_pow);
			partition_type pA6 = getpartition(enablebufferA, kvA6[0], currentLOP, upperlimit, upperpartition, globalparams.batch_range_pow);
			
			/// LOADING FROM AND INTO B
			buffer_type posB0 = bufferBcapsule[0][pA0].key + bufferBcapsule[0][pA0].value;
			#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
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
			#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
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
			#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
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
			#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
			actsutilityobj->checkfordivisibleby(enablebufferB, "posB6", posB6, 2);
			actsutilityobj->checkoutofbounds(enablebufferB, "posB6", posB6, SRCBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferB, "posB6", posB6, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			#endif 
			kvB6[0] = bufferB[6][posB6]; kvB7[0] = bufferB[7][posB6];
			kvB6[1] = bufferB[6][posB6+1]; kvB7[1] = bufferB[7][posB6+1];
			bufferB[6][posB6] = kvA6[0]; bufferB[7][posB6] = kvA6[1];
			bufferB[6][posB6+1] = kvA6[2]; bufferB[7][posB6+1] = kvA6[3];
			bufferBcapsule[3][pA6].value += 2;
			
			partition_type pB0 = getpartition(enablebufferB, kvB0[0], currentLOP, upperlimit, upperpartition, globalparams.batch_range_pow);
			partition_type pB2 = getpartition(enablebufferB, kvB2[0], currentLOP, upperlimit, upperpartition, globalparams.batch_range_pow);
			partition_type pB4 = getpartition(enablebufferB, kvB4[0], currentLOP, upperlimit, upperpartition, globalparams.batch_range_pow);
			partition_type pB6 = getpartition(enablebufferB, kvB6[0], currentLOP, upperlimit, upperpartition, globalparams.batch_range_pow);
			
			#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
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
			#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
			actsutilityobj->checkoutofbounds(enablebufferC, "posC0", posC0, SRCBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferC, "posC0", posC0, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			#endif 
			kvC0[0] = bufferC[0][posC0]; kvC1[0] = bufferC[1][posC0]; kvC2[0] = bufferC[2][posC0]; kvC3[0] = bufferC[3][posC0];
			bufferC[0][posC0] = kvB0[0]; bufferC[1][posC0] = kvB1[0]; bufferC[2][posC0] = kvB0[1]; bufferC[3][posC0] = kvB1[1];
			bufferCcapsule[0][pB0].value += 1;
			
			buffer_type _posC0 = bufferCcapsule[0][pB2].key + bufferCcapsule[0][pB2].value;
			#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
			actsutilityobj->checkoutofbounds(enablebufferC, "_posC0", _posC0, SRCBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferC, "_posC0", _posC0, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			#endif 
			_kvC0[0] = bufferC[0][_posC0]; _kvC1[0] = bufferC[1][_posC0]; _kvC2[0] = bufferC[2][_posC0]; _kvC3[0] = bufferC[3][_posC0];
			bufferC[0][_posC0] = kvB2[0]; bufferC[1][_posC0] = kvB3[0]; bufferC[2][_posC0] = kvB2[1]; bufferC[3][_posC0] = kvB3[1];
			bufferCcapsule[0][pB2].value += 1;
			
			buffer_type posC4 = bufferCcapsule[1][pB4].key + bufferCcapsule[1][pB4].value;
			#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
			actsutilityobj->checkoutofbounds(enablebufferC, "posC4", posC4, SRCBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferC, "posC4", posC4, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			#endif 
			kvC4[0] = bufferC[4][posC4]; kvC5[0] = bufferC[5][posC4]; kvC6[0] = bufferC[6][posC4]; kvC7[0] = bufferC[7][posC4]; 
			bufferC[4][posC4] = kvB4[0]; bufferC[5][posC4] = kvB5[0]; bufferC[6][posC4] = kvB4[1]; bufferC[7][posC4] = kvB5[1];
			bufferCcapsule[1][pB4].value += 1;
			
			buffer_type _posC4 = bufferCcapsule[1][pB6].key + bufferCcapsule[1][pB6].value;
			#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
			actsutilityobj->checkoutofbounds(enablebufferC, "_posC4", _posC4, SRCBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferC, "_posC4", _posC4, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			#endif 
			_kvC4[0] = bufferC[4][_posC4]; _kvC5[0] = bufferC[5][_posC4]; _kvC6[0] = bufferC[6][_posC4]; _kvC7[0] = bufferC[7][_posC4];
			bufferC[4][_posC4] = kvB6[0]; bufferC[5][_posC4] = kvB7[0]; bufferC[6][_posC4] = kvB6[1]; bufferC[7][_posC4] = kvB7[1];
			bufferCcapsule[1][pB6].value += 1;
			
			partition_type pC0 = getpartition(enablebufferC, kvC0[0], currentLOP, upperlimit, upperpartition, globalparams.batch_range_pow);
			partition_type _pC0 = getpartition(enablebufferC, _kvC0[0], currentLOP, upperlimit, upperpartition, globalparams.batch_range_pow);
			partition_type pC4 = getpartition(enablebufferC, kvC4[0], currentLOP, upperlimit, upperpartition, globalparams.batch_range_pow);
			partition_type _pC4 = getpartition(enablebufferC, _kvC4[0], currentLOP, upperlimit, upperpartition, globalparams.batch_range_pow);
			
			#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
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
			buffer_type posD0 = bufferDcapsule[pC0].key + tempbufferDcapsule[pC0];
			unsigned int yoffset0 = posD0 / 8;
			unsigned int xoffset0 = posD0 % 8;
			#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
			actsutilityobj->checkfordivisibleby(enablebufferD, "posD0", posD0, 4);
			actsutilityobj->checkoutofbounds(enablebufferD, "posD0", posD0, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferD, "yoffset0", yoffset0, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferD, "xoffset0+4", xoffset0+4, VECTOR_SIZE+1, NAp, NAp, NAp);
			#endif 
			if(xoffset0 == 0){
				bufferD[0][yoffset0] = kvC0[0]; bufferD[1][yoffset0] = kvC1[0]; bufferD[2][yoffset0] = kvC2[0]; bufferD[3][yoffset0] = kvC3[0]; 
			} else {
				bufferD[4][yoffset0] = kvC0[0]; bufferD[5][yoffset0] = kvC1[0]; bufferD[6][yoffset0] = kvC2[0]; bufferD[7][yoffset0] = kvC3[0]; 
			}
			if(!((kvC0[0].value == INVALIDDATA) && (kvC1[0].value == INVALIDDATA) && (kvC2[0].value == INVALIDDATA) && (kvC3[0].value == INVALIDDATA))){ tempbufferDcapsule[pC0] += 4; }
			
			buffer_type _posD0 = bufferDcapsule[_pC0].key + tempbufferDcapsule[_pC0];
			unsigned int yoffset1 = _posD0 / 8;
			unsigned int xoffset1 = _posD0 % 8;
			#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
			actsutilityobj->checkfordivisibleby(enablebufferD, "_posD0", _posD0, 4);
			actsutilityobj->checkoutofbounds(enablebufferD, "_posD0", _posD0, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferD, "yoffset1", yoffset1, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferD, "xoffset1+4", xoffset1+4, VECTOR_SIZE+1, NAp, NAp, NAp);
			#endif 
			if(xoffset1 == 0){
				bufferD[0][yoffset1] = _kvC0[0]; bufferD[1][yoffset1] = _kvC1[0]; bufferD[2][yoffset1] = _kvC2[0]; bufferD[3][yoffset1] = _kvC3[0]; 
			} else {
				bufferD[4][yoffset1] = _kvC0[0]; bufferD[5][yoffset1] = _kvC1[0]; bufferD[6][yoffset1] = _kvC2[0]; bufferD[7][yoffset1] = _kvC3[0]; 
			}
			if(!((_kvC0[0].value == INVALIDDATA) && (_kvC1[0].value == INVALIDDATA) && (_kvC2[0].value == INVALIDDATA) && (_kvC3[0].value == INVALIDDATA))){ tempbufferDcapsule[_pC0] += 4; }
			
			buffer_type __posD0 = bufferDcapsule[pC4].key + tempbufferDcapsule[pC4];
			unsigned int yoffset2 = __posD0 / 8;
			unsigned int xoffset2 = __posD0 % 8;
			#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
			actsutilityobj->checkfordivisibleby(enablebufferD, "__posD0", __posD0, 4);
			actsutilityobj->checkoutofbounds(enablebufferD, "__posD0", __posD0, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferD, "yoffset2", yoffset2, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferD, "xoffset2+4", xoffset2+4, VECTOR_SIZE+1, NAp, NAp, NAp);
			#endif 
			if(xoffset2 == 0){
				bufferD[0][yoffset2] = kvC4[0]; bufferD[1][yoffset2] = kvC5[0]; bufferD[2][yoffset2] = kvC6[0]; bufferD[3][yoffset2] = kvC7[0]; 
			} else {
				bufferD[4][yoffset2] = kvC4[0]; bufferD[5][yoffset2] = kvC5[0]; bufferD[6][yoffset2] = kvC6[0]; bufferD[7][yoffset2] = kvC7[0]; 
			}
			if(!((kvC4[0].value == INVALIDDATA) && (kvC5[0].value == INVALIDDATA) && (kvC6[0].value == INVALIDDATA) && (kvC7[0].value == INVALIDDATA))){ tempbufferDcapsule[pC4] += 4; }
			
			buffer_type ___posD0 = bufferDcapsule[_pC4].key + tempbufferDcapsule[_pC4];
			unsigned int yoffset3 = ___posD0 / 8;
			unsigned int xoffset3 = ___posD0 % 8;
			#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
			actsutilityobj->checkfordivisibleby(enablebufferD, "___posD0", ___posD0, 4);
			actsutilityobj->checkoutofbounds(enablebufferD, "___posD0", ___posD0, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferD, "yoffset3", yoffset3, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferD, "xoffset3+4", xoffset3+4, VECTOR_SIZE+1, NAp, NAp, NAp);
			#endif 
			if(xoffset3 == 0){
				bufferD[0][yoffset3] = _kvC4[0]; bufferD[1][yoffset3] = _kvC5[0]; bufferD[2][yoffset3] = _kvC6[0]; bufferD[3][yoffset3] = _kvC7[0]; 
			} else {
				bufferD[4][yoffset3] = _kvC4[0]; bufferD[5][yoffset3] = _kvC5[0]; bufferD[6][yoffset3] = _kvC6[0]; bufferD[7][yoffset3] = _kvC7[0]; 
			}
			if(!((_kvC4[0].value == INVALIDDATA) && (_kvC5[0].value == INVALIDDATA) && (_kvC6[0].value == INVALIDDATA) && (_kvC7[0].value == INVALIDDATA))){ tempbufferDcapsule[_pC4] += 4; }
		}
	}
	for(partition_type p=0; p<NUM_PARTITIONS; p++){ 
	#pragma HLS PIPELINE II=1
		bufferDcapsule[p].value = tempbufferDcapsule[p]; 
	}
	for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		keyvalue_t dummykv;
		dummykv.key = p; // (p << (globalparams.batch_range_pow - (NUM_PARTITIONS_POW * currentLOP))) + upperlimit; // NEWCHANGE.
		dummykv.value = INVALIDDATA;
		if(bufferDcapsule[p].value % 8 == 4){
			unsigned int yoffset = (bufferDcapsule[p].key + bufferDcapsule[p].value) / VECTOR_SIZE;
			bufferD[4][yoffset] = dummykv; bufferD[5][yoffset] = dummykv; bufferD[6][yoffset] = dummykv; bufferD[7][yoffset] = dummykv; 
			bufferDcapsule[p].value += 4;
		}
		#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
		actsutilityobj->checkfordivisibleby(enablebufferD, "bufferDcapsule[p].value", bufferDcapsule[p].value, 8);
		#endif
	}
	
	#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS) && not defined(OPTMZ) // OPTMZ enables has 'random values present from prevs'
	actsutilityobj->checkprofile(enablebufferB, "runpipeline::checkprofile::bufferB", bufferB, SRCBUFFER_SIZE, currentLOP, upperlimit, globalparams.batch_range_pow, 4, SRCBUFFER_SIZE*VECTOR_SIZE); 
	actsutilityobj->checkprofile(enablebufferC, "runpipeline::checkprofile::bufferC", bufferC, SRCBUFFER_SIZE, currentLOP, upperlimit, globalparams.batch_range_pow, 4, SRCBUFFER_SIZE*VECTOR_SIZE); 
	actsutilityobj->checkprofile(enablebufferD, "runpipeline::checkprofile::bufferD", bufferD, SRCBUFFER_SIZE, currentLOP, upperlimit, globalparams.batch_range_pow, 4, SRCBUFFER_SIZE*VECTOR_SIZE); 
	#endif 
	#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
	actsutilityobj->checkbufferprofile(enablebufferD, "runpipeline::checkbufferprofile::bufferD", bufferD, (keyvalue_t *)bufferDcapsule, currentLOP, upperlimit, globalparams.batch_range_pow); 
	#endif
	
	#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
	unsigned int after_pcountso1[NUM_PARTITIONS];
	unsigned int after_pcountso2[NUM_PARTITIONS];
	unsigned int after_pcountso4[NUM_PARTITIONS];
	unsigned int after_pcountso8[NUM_PARTITIONS];
	actsutilityobj->intrarunpipelinecheck_shifting(enablebufferD, "intrarunpipelinecheck_shifting[after]: bufferA,bufferB,bufferC,bufferD", 
						bufferA, buffer1capsule, bufferB, bufferBcapsule, bufferC, bufferCcapsule, bufferD, bufferDcapsule,
						after_pcountso1, after_pcountso2, after_pcountso4, after_pcountso8,
						currentLOP, upperlimit, globalparams.batch_range_pow);
	#endif
	#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
		#ifndef SIMPLEANDFASTPREPAREFUNC // NEWCHANGE. not applicable 
		actsutilityobj->hcheckforequal(enablebufferD, "runpipeline: checking if before_pcountso1 == after_pcountso2", before_pcountso1, after_pcountso2, NUM_PARTITIONS, NAp, NAp, NAp);
		#endif 
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

skeyvalue_t 
	#ifdef SW 
	acts::
	#endif
runpipeline_1partition(bool_type enable, keyvalue_t bufferA[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t buffer1capsule[VECTOR_SIZE], 
				keyvalue_t bufferB[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t bufferBcapsule[4], 
					keyvalue_t bufferC[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t bufferCcapsule[2],
						keyvalue_t bufferD[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t bufferDcapsule,
							unsigned int currentLOP, sweepparams_t sweepparams, buffer_type cutoff, buffer_type cutoffs[VECTOR_SIZE], batch_type shiftcount, globalparams_t globalparams){		
	analysis_type analysis_srcbuffersz = PADDEDDESTBUFFER_SIZE;
	if(enable == OFF){ return bufferDcapsule; }
	
	unsigned int upperlimit = sweepparams.upperlimit;
	unsigned int upperpartition = sweepparams.upperpartition;
	
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
	
	buffer_type tempcutoffs[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=tempcutoffs complete
	
	bool_type enablebufferA = ON;
	bool_type enablebufferB = ON;
	bool_type enablebufferC = ON;
	bool_type enablebufferD = ON;
	#ifdef OPTMZ
	if(shiftcount >= 1){ enablebufferB = ON; } else { enablebufferB = OFF; }
	if(shiftcount >= 2){ enablebufferC = ON; } else { enablebufferC = OFF; }
	if(shiftcount >= 3){ enablebufferD = ON; } else { enablebufferD = OFF; }
	#endif
	
	bufferDcapsule.key = 0;
	bufferDcapsule.value = 0;
	
	bufferCcapsule[0].key = (bufferBcapsule[0].key + bufferBcapsule[1].key) / 2;
	bufferCcapsule[1].key = (bufferBcapsule[2].key + bufferBcapsule[3].key) / 2;
	bufferCcapsule[0].value = 0;
	bufferCcapsule[1].value = 0;
	
	bufferBcapsule[0].key = (buffer1capsule[0].key + buffer1capsule[1].key) / 2;
	bufferBcapsule[1].key = (buffer1capsule[2].key + buffer1capsule[3].key) / 2;
	bufferBcapsule[2].key = (buffer1capsule[4].key + buffer1capsule[5].key) / 2;
	bufferBcapsule[3].key = (buffer1capsule[6].key + buffer1capsule[7].key) / 2;
	bufferBcapsule[0].value = 0;
	bufferBcapsule[1].value = 0;
	bufferBcapsule[2].value = 0;
	bufferBcapsule[3].value = 0;

	#if defined(_DEBUGMODE_CHECKS2) && defined(SW)
	actsutilityobj->checkoutofbounds(ON, "runp: bufferCcapsule[0].key", bufferCcapsule[0].key, PADDEDDESTBUFFER_SIZE+1, NAp, NAp, NAp);
	actsutilityobj->checkoutofbounds(ON, "runp: bufferCcapsule[1].key", bufferCcapsule[1].key, PADDEDDESTBUFFER_SIZE+1, NAp, NAp, NAp);
	
	actsutilityobj->checkoutofbounds(ON, "runp: bufferBcapsule[0].key", bufferBcapsule[0].key, PADDEDDESTBUFFER_SIZE+1, NAp, NAp, NAp);
	actsutilityobj->checkoutofbounds(ON, "runp: bufferBcapsule[1].key", bufferBcapsule[1].key, PADDEDDESTBUFFER_SIZE+1, NAp, NAp, NAp);
	actsutilityobj->checkoutofbounds(ON, "runp: bufferBcapsule[2].key", bufferBcapsule[2].key, PADDEDDESTBUFFER_SIZE+1, NAp, NAp, NAp);
	actsutilityobj->checkoutofbounds(ON, "runp: bufferBcapsule[3].key", bufferBcapsule[3].key, PADDEDDESTBUFFER_SIZE+1, NAp, NAp, NAp);
	#endif
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkfordivisibleby(enablebufferD, "bufferDcapsule.key", bufferDcapsule.key, 1);
	
	actsutilityobj->checkfordivisibleby(enablebufferC, "bufferCcapsule[0].key", bufferCcapsule[0].key, 1);
	actsutilityobj->checkfordivisibleby(enablebufferC, "bufferCcapsule[1].key", bufferCcapsule[1].key, 1);
	
	actsutilityobj->checkfordivisibleby(enablebufferB, "bufferBcapsule[0].key", bufferBcapsule[0].key, 2);
	actsutilityobj->checkfordivisibleby(enablebufferB, "bufferBcapsule[1].key", bufferBcapsule[1].key, 2);
	actsutilityobj->checkfordivisibleby(enablebufferB, "bufferBcapsule[2].key", bufferBcapsule[2].key, 2);
	actsutilityobj->checkfordivisibleby(enablebufferB, "bufferBcapsule[3].key", bufferBcapsule[3].key, 2);
	
	actsutilityobj->checkfordivisibleby(enablebufferA, "buffer1capsule[0].key", buffer1capsule[0].key, 4);
	actsutilityobj->checkfordivisibleby(enablebufferA, "buffer1capsule[1].key", buffer1capsule[1].key, 4);
	actsutilityobj->checkfordivisibleby(enablebufferA, "buffer1capsule[2].key", buffer1capsule[2].key, 4);
	actsutilityobj->checkfordivisibleby(enablebufferA, "buffer1capsule[3].key", buffer1capsule[3].key, 4);
	actsutilityobj->checkfordivisibleby(enablebufferA, "buffer1capsule[4].key", buffer1capsule[4].key, 4);
	actsutilityobj->checkfordivisibleby(enablebufferA, "buffer1capsule[5].key", buffer1capsule[5].key, 4);
	actsutilityobj->checkfordivisibleby(enablebufferA, "buffer1capsule[6].key", buffer1capsule[6].key, 4);
	actsutilityobj->checkfordivisibleby(enablebufferA, "buffer1capsule[7].key", buffer1capsule[7].key, 4);
	#endif
	#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
	keyvalue_t CHK[4]; 
	keyvalue_t CHKK[8]; 
	#endif
	
	keyvalue_t dummykv;
	dummykv.key = 0;
	dummykv.value = INVALIDDATA;
	
	value_t tempbufferDcapsule = bufferDcapsule.value; 
	for(vector_type v=0; v<VECTOR_SIZE; v++){ 
	#pragma HLS PIPELINE II=1
		tempcutoffs[v] = cutoffs[v]; 
	}
	unsigned int n=0;
	RRUNPIPELINE_LOOP1: for(n=0; n<2; n++){
		RRUNPIPELINE_LOOP1B: for(buffer_type k=0; k<PADDEDDESTBUFFER_SIZE; k+=4){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
		#pragma HLS PIPELINE II=4
		
			#ifdef _DEBUGMODE_KERNELPRINTS
			if(k % 16 == 0){ cout<<"k: "<<k<<endl; }
			#endif
			
			#ifdef SIMPLEANDFASTPREPAREFUNC
			if(k < tempcutoffs[0+n]){ kvA0[0] = bufferA[0+n][k]; kvA0[1] = bufferA[0+n][k+1]; kvA0[2] = bufferA[0+n][k+2]; kvA0[3] = bufferA[0+n][k+3]; } 
			else { kvA0[0] = dummykv; kvA0[1] = dummykv; kvA0[2] = dummykv; kvA0[3] = dummykv; }
			if(k < tempcutoffs[2+n]){ kvA2[0] = bufferA[2+n][k]; kvA2[1] = bufferA[2+n][k+1]; kvA2[2] = bufferA[2+n][k+2]; kvA2[3] = bufferA[2+n][k+3]; } 
			else { kvA2[0] = dummykv; kvA2[1] = dummykv; kvA2[2] = dummykv; kvA2[3] = dummykv; }
			if(k < tempcutoffs[4+n]){ kvA4[0] = bufferA[4+n][k]; kvA4[1] = bufferA[4+n][k+1]; kvA4[2] = bufferA[4+n][k+2]; kvA4[3] = bufferA[4+n][k+3]; } 
			else { kvA4[0] = dummykv; kvA4[1] = dummykv; kvA4[2] = dummykv; kvA4[3] = dummykv; }
			if(k < tempcutoffs[6+n]){ kvA6[0] = bufferA[6+n][k]; kvA6[1] = bufferA[6+n][k+1]; kvA6[2] = bufferA[6+n][k+2]; kvA6[3] = bufferA[6+n][k+3]; } 
			else { kvA6[0] = dummykv; kvA6[1] = dummykv; kvA6[2] = dummykv; kvA6[3] = dummykv; }
			#else 
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
			#endif 
		
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
			#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
			actsutilityobj->checkn(enablebufferA, "kvA0", kvA0, currentLOP, upperlimit, globalparams.batch_range_pow, 4);
			actsutilityobj->checkn(enablebufferA, "kvA2", kvA2, currentLOP, upperlimit, globalparams.batch_range_pow, 4);
			actsutilityobj->checkn(enablebufferA, "kvA4", kvA4, currentLOP, upperlimit, globalparams.batch_range_pow, 4);
			actsutilityobj->checkn(enablebufferA, "kvA6", kvA6, currentLOP, upperlimit, globalparams.batch_range_pow, 4);
			#endif
			
			/// LOADING FROM AND INTO B
			buffer_type posB0 = bufferBcapsule[0].key + bufferBcapsule[0].value;
			#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
			actsutilityobj->checkfordivisibleby(enablebufferB, "posB0", posB0, 2);
			actsutilityobj->checkoutofbounds(enablebufferB, "posB0", posB0, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferB, "posB0", posB0, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			#endif 
			kvB0[0] = bufferB[0][posB0]; kvB1[0] = bufferB[1][posB0];
			kvB0[1] = bufferB[0][posB0+1]; kvB1[1] = bufferB[1][posB0+1];
			bufferB[0][posB0] = kvA0[0]; bufferB[1][posB0] = kvA0[1]; 
			bufferB[0][posB0+1] = kvA0[2]; bufferB[1][posB0+1] = kvA0[3];
			bufferBcapsule[0].value += 2;
		
			buffer_type posB2 = bufferBcapsule[1].key + bufferBcapsule[1].value; 
			#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
			actsutilityobj->checkfordivisibleby(enablebufferB, "posB2", posB2, 2);
			actsutilityobj->checkoutofbounds(enablebufferB, "posB2", posB2, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferB, "posB2", posB2, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			#endif 
			kvB2[0] = bufferB[2][posB2]; kvB3[0] = bufferB[3][posB2];
			kvB2[1] = bufferB[2][posB2+1]; kvB3[1] = bufferB[3][posB2+1];
			bufferB[2][posB2] = kvA2[0]; bufferB[3][posB2] = kvA2[1];
			bufferB[2][posB2+1] = kvA2[2]; bufferB[3][posB2+1] = kvA2[3];
			bufferBcapsule[1].value += 2;
			
			buffer_type posB4 = bufferBcapsule[2].key + bufferBcapsule[2].value; 
			#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
			actsutilityobj->checkfordivisibleby(enablebufferB, "posB4", posB4, 2);
			actsutilityobj->checkoutofbounds(enablebufferB, "posB4", posB4, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferB, "posB4", posB4, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			#endif 
			kvB4[0] = bufferB[4][posB4]; kvB5[0] = bufferB[5][posB4];
			kvB4[1] = bufferB[4][posB4+1]; kvB5[1] = bufferB[5][posB4+1];
			bufferB[4][posB4] = kvA4[0]; bufferB[5][posB4] = kvA4[1];
			bufferB[4][posB4+1] = kvA4[2]; bufferB[5][posB4+1] = kvA4[3];
			bufferBcapsule[2].value += 2;
			
			buffer_type posB6 = bufferBcapsule[3].key + bufferBcapsule[3].value; 
			#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
			actsutilityobj->checkfordivisibleby(enablebufferB, "posB6", posB6, 2);
			actsutilityobj->checkoutofbounds(enablebufferB, "posB6", posB6, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferB, "posB6", posB6, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			#endif 
			kvB6[0] = bufferB[6][posB6]; kvB7[0] = bufferB[7][posB6];
			kvB6[1] = bufferB[6][posB6+1]; kvB7[1] = bufferB[7][posB6+1];
			bufferB[6][posB6] = kvA6[0]; bufferB[7][posB6] = kvA6[1];
			bufferB[6][posB6+1] = kvA6[2]; bufferB[7][posB6+1] = kvA6[3];
			bufferBcapsule[3].value += 2;
			
			#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
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
			buffer_type posC0 = bufferCcapsule[0].key + bufferCcapsule[0].value;
			#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
			actsutilityobj->checkoutofbounds(enablebufferC, "posC0", posC0, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferC, "posC0", posC0, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			#endif 
			kvC0[0] = bufferC[0][posC0]; kvC1[0] = bufferC[1][posC0]; kvC2[0] = bufferC[2][posC0]; kvC3[0] = bufferC[3][posC0];
			bufferC[0][posC0] = kvB0[0]; bufferC[1][posC0] = kvB1[0]; bufferC[2][posC0] = kvB0[1]; bufferC[3][posC0] = kvB1[1];
			bufferCcapsule[0].value += 1;
			
			buffer_type _posC0 = bufferCcapsule[0].key + bufferCcapsule[0].value;
			#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
			actsutilityobj->checkoutofbounds(enablebufferC, "_posC0", _posC0, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferC, "_posC0", _posC0, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			#endif 
			_kvC0[0] = bufferC[0][_posC0]; _kvC1[0] = bufferC[1][_posC0]; _kvC2[0] = bufferC[2][_posC0]; _kvC3[0] = bufferC[3][_posC0];
			bufferC[0][_posC0] = kvB2[0]; bufferC[1][_posC0] = kvB3[0]; bufferC[2][_posC0] = kvB2[1]; bufferC[3][_posC0] = kvB3[1];
			bufferCcapsule[0].value += 1;
			
			buffer_type posC4 = bufferCcapsule[1].key + bufferCcapsule[1].value;
			#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
			actsutilityobj->checkoutofbounds(enablebufferC, "posC4", posC4, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferC, "posC4", posC4, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			#endif 
			kvC4[0] = bufferC[4][posC4]; kvC5[0] = bufferC[5][posC4]; kvC6[0] = bufferC[6][posC4]; kvC7[0] = bufferC[7][posC4]; 
			bufferC[4][posC4] = kvB4[0]; bufferC[5][posC4] = kvB5[0]; bufferC[6][posC4] = kvB4[1]; bufferC[7][posC4] = kvB5[1];
			bufferCcapsule[1].value += 1;
			
			buffer_type _posC4 = bufferCcapsule[1].key + bufferCcapsule[1].value;
			#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
			actsutilityobj->checkoutofbounds(enablebufferC, "_posC4", _posC4, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferC, "_posC4", _posC4, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			#endif 
			_kvC4[0] = bufferC[4][_posC4]; _kvC5[0] = bufferC[5][_posC4]; _kvC6[0] = bufferC[6][_posC4]; _kvC7[0] = bufferC[7][_posC4];
			bufferC[4][_posC4] = kvB6[0]; bufferC[5][_posC4] = kvB7[0]; bufferC[6][_posC4] = kvB6[1]; bufferC[7][_posC4] = kvB7[1];
			bufferCcapsule[1].value += 1;
			
			#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
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
			buffer_type posD0 = bufferDcapsule.key + tempbufferDcapsule;
			unsigned int yoffset0 = posD0 / 8;
			unsigned int xoffset0 = posD0 % 8;
			#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
			actsutilityobj->checkfordivisibleby(enablebufferD, "posD0", posD0, 4);
			actsutilityobj->checkoutofbounds(enablebufferD, "posD0", posD0, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferD, "yoffset0", yoffset0, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferD, "xoffset0+4", xoffset0+4, VECTOR_SIZE+1, NAp, NAp, NAp);
			#endif
			if(xoffset0 == 0){
				bufferD[0][yoffset0].key = kvC0[0].key; bufferD[0][yoffset0].value = kvC1[0].key; bufferD[1][yoffset0].key = kvC2[0].key; bufferD[1][yoffset0].value = kvC3[0].key; 
			} else if(xoffset0 == 2){
				bufferD[2][yoffset0].key = kvC0[0].key; bufferD[2][yoffset0].value = kvC1[0].key; bufferD[3][yoffset0].key = kvC2[0].key; bufferD[3][yoffset0].value = kvC3[0].key; 
			} else if(xoffset0 == 4){
				bufferD[4][yoffset0].key = kvC0[0].key; bufferD[4][yoffset0].value = kvC1[0].key; bufferD[5][yoffset0].key = kvC2[0].key; bufferD[5][yoffset0].value = kvC3[0].key; 
			} else if(xoffset0 == 6){
				bufferD[6][yoffset0].key = kvC0[0].key; bufferD[6][yoffset0].value = kvC1[0].key; bufferD[7][yoffset0].key = kvC2[0].key; bufferD[7][yoffset0].value = kvC3[0].key; 
			}
			if(!((kvC0[0].value == INVALIDDATA) && (kvC1[0].value == INVALIDDATA) && (kvC2[0].value == INVALIDDATA) && (kvC3[0].value == INVALIDDATA))){ tempbufferDcapsule += 2; }
			#ifdef _DEBUGMODE_KERNELPRINTS
			if((kvC0[0].key != 0) || (kvC1[0].key != 0) || (kvC2[0].key != 0) || (kvC3[0].key != 0)){ cout<<"runpipeline: kvC0[0].key: "<<kvC0[0].key<<", kvC1[0].key: "<<kvC1[0].key<<", kvC2[0].key: "<<kvC2[0].key<<", kvC3[0].key: "<<kvC3[0].key<<endl; } /////////////////////// REMOVEME.
			#endif 
			
			buffer_type _posD0 = bufferDcapsule.key + tempbufferDcapsule;
			unsigned int yoffset1 = _posD0 / 8;
			unsigned int xoffset1 = _posD0 % 8;
			#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
			actsutilityobj->checkfordivisibleby(enablebufferD, "_posD0", _posD0, 4);
			actsutilityobj->checkoutofbounds(enablebufferD, "_posD0", _posD0, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferD, "yoffset1", yoffset1, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferD, "xoffset1+4", xoffset1+4, VECTOR_SIZE+1, NAp, NAp, NAp);
			#endif 
			if(xoffset1 == 0){
				bufferD[0][yoffset1].key = _kvC0[0].key; bufferD[0][yoffset1].value = _kvC1[0].key; bufferD[1][yoffset1].key = _kvC2[0].key; bufferD[1][yoffset1].value = _kvC3[0].key; 
			} else if(xoffset1 == 2){
				bufferD[2][yoffset1].key = _kvC0[0].key; bufferD[2][yoffset1].value = _kvC1[0].key; bufferD[3][yoffset1].key = _kvC2[0].key; bufferD[3][yoffset1].value = _kvC3[0].key; 
			} else if(xoffset1 == 4){
				bufferD[4][yoffset1].key = _kvC0[0].key; bufferD[4][yoffset1].value = _kvC1[0].key; bufferD[5][yoffset1].key = _kvC2[0].key; bufferD[5][yoffset1].value = _kvC3[0].key; 
			} else if(xoffset1 == 6){
				bufferD[6][yoffset1].key = _kvC0[0].key; bufferD[6][yoffset1].value = _kvC1[0].key; bufferD[7][yoffset1].key = _kvC2[0].key; bufferD[7][yoffset1].value = _kvC3[0].key; 
			}
			if(!((_kvC0[0].value == INVALIDDATA) && (_kvC1[0].value == INVALIDDATA) && (_kvC2[0].value == INVALIDDATA) && (_kvC3[0].value == INVALIDDATA))){ tempbufferDcapsule += 2; }
			#ifdef _DEBUGMODE_KERNELPRINTS
			if((_kvC0[0].key != 0) || (_kvC1[0].key != 0) || (_kvC2[0].key != 0) || (_kvC3[0].key != 0)){ cout<<"runpipeline: _kvC0[0].key: "<<_kvC0[0].key<<", _kvC1[0].key: "<<_kvC1[0].key<<", _kvC2[0].key: "<<_kvC2[0].key<<", _kvC3[0].key: "<<_kvC3[0].key<<endl; } /////////////////////// REMOVEME.
			#endif 
			
			buffer_type __posD0 = bufferDcapsule.key + tempbufferDcapsule;
			unsigned int yoffset2 = __posD0 / 8;
			unsigned int xoffset2 = __posD0 % 8;
			#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
			actsutilityobj->checkfordivisibleby(enablebufferD, "__posD0", __posD0, 4);
			actsutilityobj->checkoutofbounds(enablebufferD, "__posD0", __posD0, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferD, "yoffset2", yoffset2, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferD, "xoffset2+4", xoffset2+4, VECTOR_SIZE+1, NAp, NAp, NAp);
			#endif 
			if(xoffset2 == 0){
				bufferD[0][yoffset2].key = kvC4[0].key; bufferD[0][yoffset2].value = kvC5[0].key; bufferD[1][yoffset2].key = kvC6[0].key; bufferD[1][yoffset2].value = kvC7[0].key; 
			} else if(xoffset2 == 2){
				bufferD[2][yoffset2].key = kvC4[0].key; bufferD[2][yoffset2].value = kvC5[0].key; bufferD[3][yoffset2].key = kvC6[0].key; bufferD[3][yoffset2].value = kvC7[0].key; 
			} else if(xoffset2 == 4){
				bufferD[4][yoffset2].key = kvC4[0].key; bufferD[4][yoffset2].value = kvC5[0].key; bufferD[5][yoffset2].key = kvC6[0].key; bufferD[5][yoffset2].value = kvC7[0].key; 
			} else if(xoffset2 == 6){
				bufferD[6][yoffset2].key = kvC4[0].key; bufferD[6][yoffset2].value = kvC5[0].key; bufferD[7][yoffset2].key = kvC6[0].key; bufferD[7][yoffset2].value = kvC7[0].key; 
			}
			if(!((kvC4[0].value == INVALIDDATA) && (kvC5[0].value == INVALIDDATA) && (kvC6[0].value == INVALIDDATA) && (kvC7[0].value == INVALIDDATA))){ tempbufferDcapsule += 2; }
			#ifdef _DEBUGMODE_KERNELPRINTS
			if((kvC4[0].key != 0) || (kvC5[0].key != 0) || (kvC6[0].key != 0) || (kvC7[0].key != 0)){ cout<<"runpipeline: kvC4[0].key: "<<kvC4[0].key<<", kvC5[0].key: "<<kvC5[0].key<<", kvC6[0].key: "<<kvC6[0].key<<", kvC7[0].key: "<<kvC7[0].key<<endl; } /////////////////////// REMOVEME.
			#endif 
			
			buffer_type ___posD0 = bufferDcapsule.key + tempbufferDcapsule;
			unsigned int yoffset3 = ___posD0 / 8;
			unsigned int xoffset3 = ___posD0 % 8;
			#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
			actsutilityobj->checkfordivisibleby(enablebufferD, "___posD0", ___posD0, 4);
			actsutilityobj->checkoutofbounds(enablebufferD, "___posD0", ___posD0, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferD, "yoffset3", yoffset3, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferD, "xoffset3+4", xoffset3+4, VECTOR_SIZE+1, NAp, NAp, NAp);
			#endif 
			if(xoffset3 == 0){
				bufferD[0][yoffset3].key = _kvC4[0].key; bufferD[0][yoffset3].value = _kvC5[0].key; bufferD[1][yoffset3].key = _kvC6[0].key; bufferD[1][yoffset3].value = _kvC7[0].key; 
			} else if(xoffset3 == 2){
				bufferD[2][yoffset3].key = _kvC4[0].key; bufferD[2][yoffset3].value = _kvC5[0].key; bufferD[3][yoffset3].key = _kvC6[0].key; bufferD[3][yoffset3].value = _kvC7[0].key; 
			} else if(xoffset3 == 4){
				bufferD[4][yoffset3].key = _kvC4[0].key; bufferD[4][yoffset3].value = _kvC5[0].key; bufferD[5][yoffset3].key = _kvC6[0].key; bufferD[5][yoffset3].value = _kvC7[0].key; 
			} else if(xoffset3 == 6){
				bufferD[6][yoffset3].key = _kvC4[0].key; bufferD[6][yoffset3].value = _kvC5[0].key; bufferD[7][yoffset3].key = _kvC6[0].key; bufferD[7][yoffset3].value = _kvC7[0].key; 
			}
			if(!((_kvC4[0].value == INVALIDDATA) && (_kvC5[0].value == INVALIDDATA) && (_kvC6[0].value == INVALIDDATA) && (_kvC7[0].value == INVALIDDATA))){ tempbufferDcapsule += 2; }
			#ifdef _DEBUGMODE_KERNELPRINTS
			if((_kvC4[0].key != 0) || (_kvC5[0].key != 0) || (_kvC6[0].key != 0) || (_kvC7[0].key != 0)){ cout<<"runpipeline: _kvC4[0].key: "<<_kvC4[0].key<<", _kvC5[0].key: "<<_kvC5[0].key<<", _kvC6[0].key: "<<_kvC6[0].key<<", _kvC7[0].key: "<<_kvC7[0].key<<endl; } /////////////////////// REMOVEME.
			#endif
		}
	}
	bufferDcapsule.value = tempbufferDcapsule; 
	unsigned int yoffset = (bufferDcapsule.key + bufferDcapsule.value) / VECTOR_SIZE;
	if(bufferDcapsule.value % 8 == 0){
		// perfect.
	} else if(bufferDcapsule.value % 8 == 2){
		bufferD[2][yoffset].key = INVALIDDATA; bufferD[2][yoffset].value = INVALIDDATA; 
		bufferD[3][yoffset].key = INVALIDDATA; bufferD[3][yoffset].value = INVALIDDATA; 
		bufferD[4][yoffset].key = INVALIDDATA; bufferD[4][yoffset].value = INVALIDDATA; 
		bufferD[5][yoffset].key = INVALIDDATA; bufferD[5][yoffset].value = INVALIDDATA; 
		bufferD[6][yoffset].key = INVALIDDATA; bufferD[6][yoffset].value = INVALIDDATA; 
		bufferD[7][yoffset].key = INVALIDDATA; bufferD[7][yoffset].value = INVALIDDATA; 
		bufferDcapsule.value += 6;
	} else if(bufferDcapsule.value % 8 == 4){
		bufferD[4][yoffset].key = INVALIDDATA; bufferD[4][yoffset].value = INVALIDDATA; 
		bufferD[5][yoffset].key = INVALIDDATA; bufferD[5][yoffset].value = INVALIDDATA; 
		bufferD[6][yoffset].key = INVALIDDATA; bufferD[6][yoffset].value = INVALIDDATA; 
		bufferD[7][yoffset].key = INVALIDDATA; bufferD[7][yoffset].value = INVALIDDATA; 
		bufferDcapsule.value += 4;
	} else if(bufferDcapsule.value % 8 == 6){
		bufferD[6][yoffset].key = INVALIDDATA; bufferD[6][yoffset].value = INVALIDDATA; 
		bufferD[7][yoffset].key = INVALIDDATA; bufferD[7][yoffset].value = INVALIDDATA; 
		bufferDcapsule.value += 2;
	} else if(bufferDcapsule.value % 8 == 8){
		// perfect.
	} else {
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"runpipeline 87: should never get here. bufferDcapsule.value: "<<bufferDcapsule.value<<", exiting... "<<endl;
		exit(EXIT_FAILURE);
		#endif 
	}
	
	#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
	actsutilityobj->checkfordivisibleby(enablebufferD, "bufferDcapsule.value", bufferDcapsule.value, 8);
	#endif
	return bufferDcapsule;
}

// collect stats phase 
void 
	#ifdef SW 
	acts::
	#endif
collectstats(
		bool_type enable,
		uint512_dt * kvdram,
		keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ], 
		config_t config,
		globalparams_t globalparams,
		sweepparams_t sweepparams,
		travstate_t ctravstate){
	#pragma HLS INLINE
	analysis_type analysis_collectstatsloop = KVDATA_BATCHSIZE_KVS / SRCBUFFER_SIZE;
	if(enable == OFF){ return; }

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
		
		collectglobalstats(ON, sourcebuffer, destbuffer, sweepparams.currentLOP, sweepparams, ctravstate, globalparams);
	}
	prepareglobalstats(enable_generatestats, destbuffer, globalstatsbuffer, globalparams);
	return;
}

// partition updates phase
void 
	#ifdef SW 
	acts::
	#endif
partitionupdates_coarsegrainedpipeline(
		bool_type enable,
		uint512_dt * kvdram,
		keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer_setof1[8][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer_setof2[8][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer_setof4[8][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer_setof8[8][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so1[8][NUM_PARTITIONS],
		skeyvalue_t templocalcapsule_so2[4][NUM_PARTITIONS],
		skeyvalue_t templocalcapsule_so4[2][NUM_PARTITIONS],
		skeyvalue_t templocalcapsule_so8[NUM_PARTITIONS],
		keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ], 
		config_t config,
		globalparams_t globalparams,
		sweepparams_t sweepparams,
		travstate_t ptravstate,
		batch_type sourcebaseaddr_kvs,
		batch_type destbaseaddr_kvs){
	#pragma HLS INLINE
	analysis_type analysis_partitionloop = KVDATA_BATCHSIZE_KVS / (2 * SRCBUFFER_SIZE);
	if(enable == OFF){ return; } 
	
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
		
		preparekeyvalues_coarsegrainedpipeline(ON, sourcebuffer, buffer_setof1, templocalcapsule_so1, sweepparams.currentLOP, sweepparams, ptravstatepp0, SRCBUFFER_SIZE, globalparams);
		#ifdef PP1
		readkeyvalues(ON, kvdram, sourcebuffer, (sweepparams.worksourcebaseaddress_kvs + offset_kvs + SRCBUFFER_SIZE), SRCBUFFER_SIZE, ptravstatepp1);
		#endif
		#ifdef PP2 // pipeline overflow from bottom
		savekeyvalues(en, kvdram, buffer_setof8, globalstatsbuffer, templocalcapsule_so8, sweepparams.workdestbaseaddress_kvs, globalparams);
		#endif
		
		// 1s->2s
		combineSetof1stoSetof2s(ON, buffer_setof1, buffer_setof2, templocalcapsule_so1, templocalcapsule_so2, globalparams);
		#ifdef PP1
		preparekeyvalues_coarsegrainedpipeline(ON, sourcebuffer, buffer_setof1, templocalcapsule_so1, sweepparams.currentLOP, sweepparams, ptravstatepp1, SRCBUFFER_SIZE, globalparams);
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
		preparekeyvalues_coarsegrainedpipeline(ON, sourcebuffer, buffer_setof1, templocalcapsule_so1, sweepparams.currentLOP, sweepparams, ptravstatepp2, SRCBUFFER_SIZE, globalparams);
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
	acts::
	#endif
partitionupdates_finegrainedpipeline_evencutoffs(
		bool_type enable,
		uint512_dt * kvdram,
		keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer_setof1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer_setof2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer_setof4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer_setof8[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so1[8][NUM_PARTITIONS],
		skeyvalue_t templocalcapsule_so2[4][NUM_PARTITIONS],
		skeyvalue_t templocalcapsule_so4[2][NUM_PARTITIONS],
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
	analysis_type analysis_partitionloop = KVDATA_BATCHSIZE_KVS / (NUMACTSFASTPIPELINES * WORKBUFFER_SIZE);
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
	buffer_type pp0cutoff = WORKBUFFER_SIZE; 
	buffer_type pp1cutoff = WORKBUFFER_SIZE; 
	buffer_type pp0cutoffs[VECTOR_SIZE];
	buffer_type pp1cutoffs[VECTOR_SIZE];
	batch_type pp0readoffset_kvs = ptravstate.begin_kvs;
	batch_type pp1readoffset_kvs = ptravstate.begin_kvs + WORKBUFFER_SIZE; 
	buffer_type pp0readsize_kvs = WORKBUFFER_SIZE; 
	buffer_type pp1readsize_kvs = WORKBUFFER_SIZE;
	batch_type itercount = 0;
	batch_type flushsize = 0;

	batch_type paddsize_kvs = 2*ptravstate.skip_kvs;
	batch_type cutoffpaddsize_kvs = 2048*ptravstate.skip_kvs;
	batch_type approxendoffset_kvs = ptravstate.end_kvs + cutoffpaddsize_kvs + paddsize_kvs;
	batch_type approxskip_kvs = ptravstate.skip_kvs * NUMACTSFASTPIPELINES;

	MAINLOOP_PARTITION: for(batch_type offset_kvs=ptravstate.begin_kvs; offset_kvs<approxendoffset_kvs; offset_kvs+=approxskip_kvs){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_partitionloop avg=analysis_partitionloop
		#ifdef _DEBUGMODE_KERNELPRINTS
		actsutilityobj->print6("### dispatch::partition:: offset0_kvs", "size0_kvs", "begin_kvs", "end_kvs", "skip", "itercount", pp0readoffset_kvs, pp0readsize_kvs, ptravstate.begin_kvs, ptravstate.end_kvs, WORKBUFFER_SIZE, itercount);
		#ifdef FPP1
		actsutilityobj->print6("### dispatch::partition:: offset1_kvs", "size1_kvs", "begin_kvs", "end_kvs", "skip", "itercount", pp1readoffset_kvs, pp1readsize_kvs, ptravstate.begin_kvs, ptravstate.end_kvs, WORKBUFFER_SIZE, itercount);
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
		runpipeline(pp1runpipelineen, buffer_setof1, templocalcapsule_so1, buffer_setof2, templocalcapsule_so2, buffer_setof4, templocalcapsule_so4, buffer_setof8, templocalcapsule_so8, sweepparams.currentLOP, sweepparams, pp1cutoff, pp1cutoffs, (itercount-2)+1, globalparams);
		#endif 
		
		pp0cutoff = preparekeyvalues_finegrainedpipeline(ON, sourcebuffer, buffer_setof1, templocalcapsule_so1, sweepparams.currentLOP, sweepparams, ptravstatepp0, pp0readsize_kvs, pp0cutoffs, globalparams);
			#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
			actsutilityobj->collectstats(ON, buffer_setof1, pp0cutoff , sweepparams.currentLOP, sweepparams.upperlimit, globalparams.batch_range_pow, 0, itercount%MYSTATSYSIZE);
			actsutilityobj->collectstats(ON, buffer_setof1, pp0cutoff, sweepparams.currentLOP, sweepparams.upperlimit, globalparams.batch_range_pow, 7, 1);
			#endif 
		// exit(EXIT_SUCCESS); // REMOVEME.
		#ifdef FPP1
		savekeyvalues(pp1writeen, kvdram, buffer_setof8, globalstatsbuffer, templocalcapsule_so8, destbaseaddr_kvs, globalparams); 
			#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
			if(pp1writeen==ON){ actsutilityobj->collectstats(ON, buffer_setof8, (keyvalue_t *)templocalcapsule_so8, sweepparams.currentLOP, sweepparams.upperlimit, globalparams.batch_range_pow, 1, (itercount+1)%MYSTATSYSIZE); }
			#endif 
		#endif 
		
		runpipeline(ON, buffer_setof1, templocalcapsule_so1, buffer_setof2, templocalcapsule_so2, buffer_setof4, templocalcapsule_so4, buffer_setof8, templocalcapsule_so8, sweepparams.currentLOP, sweepparams, pp0cutoff, pp0cutoffs, itercount, globalparams);
		// exit(EXIT_SUCCESS); // REMOVEME.
		#ifdef FPP1
		readkeyvalues(ON, kvdram, sourcebuffer, (sourcebaseaddr_kvs + pp1readoffset_kvs), pp1readsize_kvs, ptravstatepp1);
		#endif
		
		savekeyvalues(pp0writeen, kvdram, buffer_setof8, globalstatsbuffer, templocalcapsule_so8, destbaseaddr_kvs, globalparams);
			#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
			if(pp0writeen==ON){ actsutilityobj->collectstats(ON, buffer_setof8, (keyvalue_t *)templocalcapsule_so8, sweepparams.currentLOP, sweepparams.upperlimit, globalparams.batch_range_pow, 1, itercount%MYSTATSYSIZE); }
			#endif
		#ifdef FPP1
		pp1cutoff = preparekeyvalues_finegrainedpipeline(pp1partitionen, sourcebuffer, buffer_setof1, templocalcapsule_so1, sweepparams.currentLOP, sweepparams, ptravstatepp1, pp1readsize_kvs, pp1cutoffs, globalparams);
			#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
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
		
		#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
		if(itercount%MYSTATSYSIZE == MYSTATSYSIZE-1){ actsutilityobj->intrapartitioncheck(); }
		#endif
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printglobalvars();
	actsutilityobj->clearglobalvars();
	#endif
	return;
}

void 
	#ifdef SW 
	acts::
	#endif
partitionupdates_finegrainedpipeline_unevencutoffs(
		bool_type enable,
		uint512_dt * kvdram,
		keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer_setof1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer_setof2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer_setof4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer_setof8[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so1[8][NUM_PARTITIONS],
		skeyvalue_t templocalcapsule_so2[4][NUM_PARTITIONS],
		skeyvalue_t templocalcapsule_so4[2][NUM_PARTITIONS],
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
	analysis_type analysis_partitionloop = KVDATA_BATCHSIZE_KVS / (NUMACTSFASTPIPELINES * WORKBUFFER_SIZE);
	if(enable == OFF){ return; }
	
	travstate_t ptravstatepp0 = ptravstate;
	travstate_t ptravstatepp1 = ptravstate;

	resetmanykeyandvalues(templocalcapsule_so1, NUM_PARTITIONS, 0);
	
	bool_type pp0readen = ON;
	bool_type pp1readen = ON;
	bool_type pp0runpipelineen = ON;
	bool_type pp1runpipelineen = ON;
	bool_type pp0partitionen = ON;
	bool_type pp1partitionen = ON;
	bool_type pp0writeen = ON;
	bool_type pp1writeen = ON;
	buffer_type pp0cutoffs[VECTOR_SIZE];
	buffer_type pp1cutoffs[VECTOR_SIZE];
	batch_type itercount = 0;

	MAINLOOP_PARTITION: for(batch_type offset_kvs=ptravstate.begin_kvs; offset_kvs<ptravstate.end_kvs + 3*SRCBUFFER_SIZE; offset_kvs+=WORKBUFFER_SIZE * NUMACTSFASTPIPELINES){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_partitionloop avg=analysis_partitionloop
		#ifdef _DEBUGMODE_KERNELPRINTS
		actsutilityobj->print6("### dispatch::partition:: offset0_kvs", "size0_kvs", "begin_kvs", "end_kvs", "skip", "itercount", offset_kvs, pp0readsize_kvs, ptravstate.begin_kvs, ptravstate.end_kvs, WORKBUFFER_SIZE, itercount);
		actsutilityobj->print6("### dispatch::partition:: offset1_kvs", "size1_kvs", "begin_kvs", "end_kvs", "skip", "itercount", offset_kvs + WORKBUFFER_SIZE, pp1readsize_kvs, ptravstate.begin_kvs, ptravstate.end_kvs, WORKBUFFER_SIZE, itercount);
		#endif
		
		ptravstatepp0.i_kvs =  offset_kvs;
		#ifdef FPP1
		ptravstatepp1.i_kvs = offset_kvs + WORKBUFFER_SIZE;
		#endif
		#ifdef FPP1
		if(itercount >= 2){ pp0writeen = ON; } else { pp0writeen = OFF; }
		if(itercount >= 4){ pp1writeen = ON; } else { pp1writeen = OFF; }
		if(itercount >= 2){ pp1runpipelineen = ON; } else { pp1runpipelineen = OFF; }
		#else 
		if(itercount >= 2){ pp0writeen = ON; } else { pp0writeen = OFF; }
		#endif
	
		readkeyvalues(ON, kvdram, sourcebuffer, (sourcebaseaddr_kvs + offset_kvs), WORKBUFFER_SIZE, ptravstatepp0); 
		#ifdef FPP1
		runpipeline(pp1runpipelineen, buffer_setof1, templocalcapsule_so1, buffer_setof2, templocalcapsule_so2, buffer_setof4, templocalcapsule_so4, buffer_setof8, templocalcapsule_so8, sweepparams.currentLOP, sweepparams, WORKBUFFER_SIZE, pp1cutoffs, (itercount-2)+1, globalparams);
		#endif 
		
		preparekeyvalues_finegrainedpipeline(ON, sourcebuffer, buffer_setof1, templocalcapsule_so1, sweepparams.currentLOP, sweepparams, ptravstatepp0, WORKBUFFER_SIZE, pp0cutoffs, globalparams);
		#ifdef FPP1
		savekeyvalues(pp1writeen, kvdram, buffer_setof8, globalstatsbuffer, templocalcapsule_so8, destbaseaddr_kvs, globalparams); 
		#endif 
		
		runpipeline(ON, buffer_setof1, templocalcapsule_so1, buffer_setof2, templocalcapsule_so2, buffer_setof4, templocalcapsule_so4, buffer_setof8, templocalcapsule_so8, sweepparams.currentLOP, sweepparams, WORKBUFFER_SIZE, pp0cutoffs, itercount, globalparams);
		#ifdef FPP1
		readkeyvalues(ON, kvdram, sourcebuffer, (sourcebaseaddr_kvs + offset_kvs + WORKBUFFER_SIZE), WORKBUFFER_SIZE, ptravstatepp1);
		#endif
		
		savekeyvalues(pp0writeen, kvdram, buffer_setof8, globalstatsbuffer, templocalcapsule_so8, destbaseaddr_kvs, globalparams);
		#ifdef FPP1
		preparekeyvalues_finegrainedpipeline(pp1partitionen, sourcebuffer, buffer_setof1, templocalcapsule_so1, sweepparams.currentLOP, sweepparams, ptravstatepp1, WORKBUFFER_SIZE, pp1cutoffs, globalparams);
		#endif
		
		itercount += NUMACTSFASTPIPELINES;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printglobalvars();
	actsutilityobj->clearglobalvars();
	#endif
	return;
}

// reduce phase
travstate_t 
	#ifdef SW 
	acts::
	#endif
reduceupdates(
		bool_type enable,
		uint512_dt * kvdram,
		keyvalue_t verticesbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t keyvaluesbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t tempverticesbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t actvvs[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		config_t config,
		globalparams_t globalparams,
		sweepparams_t sweepparams,
		travstate_t rtravstate,
		travstate_t actvvstravstate
		){
	// NOTE: CRITICAL AUTOMATEME: LOADFACTORFORREDUCE
	#pragma HLS INLINE
	analysis_type analysis_reduceloop = KVDATA_BATCHSIZE_KVS / SRCBUFFER_SIZE;
	if(enable == OFF){ return actvvstravstate; }

	if((sweepparams.source_partition % LOADFACTORFORREDUCE) == 0){ readkeyvalues(config.enablereduce, kvdram, verticesbuffer, (globalparams.baseoffset_verticesdata_kvs + (sweepparams.source_partition * ((globalparams.applyvertexbuffersz_kvs / VDATAPACKINGFACTOR) / 2))), PADDEDDESTBUFFER_SIZE); }
	replicatedata(config.enablereduce, verticesbuffer, tempverticesbuffer, (sweepparams.source_partition % LOADFACTORFORREDUCE) * ((globalparams.applyvertexbuffersz / VDATAPACKINGFACTOR) / 2), ((globalparams.applyvertexbuffersz / VDATAPACKINGFACTOR) / 2));
	MAIN_LOOP1E_REDUCE: for(batch_type offset_kvs=rtravstate.begin_kvs; offset_kvs<rtravstate.end_kvs; offset_kvs+=rtravstate.skip_kvs){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_reduceloop avg=analysis_reduceloop
		#ifdef _DEBUGMODE_KERNELPRINTS
		actsutilityobj->print4("### dispatch::reduce:: offset_kvs", "begin_kvs", "end_kvs", "skip", offset_kvs, rtravstate.begin_kvs, rtravstate.end_kvs, SRCBUFFER_SIZE);
		#endif
		
		rtravstate.i_kvs = offset_kvs;

		readkeyvalues(ON, kvdram, keyvaluesbuffer, (sweepparams.worksourcebaseaddress_kvs + offset_kvs), SRCBUFFER_SIZE, rtravstate);
	
		#if defined(INMEMORYGP) && defined(BFS_ALGORITHM)
		reduce_bfs(ON, ON, keyvaluesbuffer, tempverticesbuffer, sweepparams, globalparams.GraphIter, globalparams.GraphAlgo, rtravstate, globalparams);
		#else 
		reduce(ON, ON, keyvaluesbuffer, tempverticesbuffer, sweepparams, globalparams.GraphIter, globalparams.GraphAlgo, rtravstate, globalparams);
		#endif
	}
	#if defined(INMEMORYGP) && defined(BFS_ALGORITHM)
	actvvstravstate = unifydata_bfs(config.enablereduce, kvdram, tempverticesbuffer, verticesbuffer, actvvs, actvvstravstate, (sweepparams.source_partition % LOADFACTORFORREDUCE) * ((globalparams.applyvertexbuffersz / VDATAPACKINGFACTOR) / 2), ((globalparams.applyvertexbuffersz / VDATAPACKINGFACTOR) / 2), sweepparams, globalparams);
	#else 
	unifydata(config.enablereduce, tempverticesbuffer, verticesbuffer, (sweepparams.source_partition % LOADFACTORFORREDUCE) * ((globalparams.applyvertexbuffersz / VDATAPACKINGFACTOR) / 2), ((globalparams.applyvertexbuffersz / VDATAPACKINGFACTOR) / 2), globalparams.GraphAlgo);
	#endif
	if((sweepparams.source_partition % LOADFACTORFORREDUCE) == LOADFACTORFORREDUCE-1){ savekeyvalues(config.enablereduce, kvdram, verticesbuffer, (globalparams.baseoffset_verticesdata_kvs + ((sweepparams.source_partition - (LOADFACTORFORREDUCE-1)) * ((globalparams.applyvertexbuffersz_kvs / VDATAPACKINGFACTOR) / 2))), PADDEDDESTBUFFER_SIZE); }
	return actvvstravstate;
}

// process all vertices 
void 
	#ifdef SW 
	acts::
	#endif
processallvertices(
		bool_type enable,
		uint512_dt * kvdram,
		keyvalue_t buffer1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer3[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer5[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer6[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer7[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t capsule_so1[8][NUM_PARTITIONS],
		skeyvalue_t capsule_so2[4][NUM_PARTITIONS],
		skeyvalue_t capsule_so4[2][NUM_PARTITIONS],
		skeyvalue_t capsule_so8[NUM_PARTITIONS],
		keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ], 
		config_t config,
		globalparams_t globalparams,
		sweepparams_t sweepparams,
		travstate_t avtravstate){
	#pragma HLS INLINE
	analysis_type analysis_processedges_overallloop = BATCH_RANGE_KVS / PADDEDDESTBUFFER_SIZE;
	analysis_type analysis_processedges_loadedgebatch = 1;
	if(enable == OFF){ return; }
	
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
		
		readkeyvalues(ON, kvdram, buffer1, globalparams.baseoffset_vertexptr_kvs + (globalparams.srcvoffset / VECTOR_SIZE) + voffset_kvs, SRCBUFFER_SIZE + 1);
		for(batch_type v=0; v<VECTOR_SIZE; v+=1){
			replicatedata(ON, buffer1, buffer2, v * SRCBUFFER_SIZE, SRCBUFFER_SIZE + 1);	
			
			vertex_t srcvlocaloffset = (voffset_kvs * VECTOR_SIZE) + (v * SRCBUFFER_SIZE);
			vertex_t myfirstsrcvid = globalparams.srcvoffset + (voffset_kvs * VECTOR_SIZE) + (v * SRCBUFFER_SIZE);
			vertex_t mylastsrcvid = myfirstsrcvid + SRCBUFFER_SIZE;// - 1;
			if(srcvlocaloffset >= globalparams.srcvsize){ mylastsrcvid = myfirstsrcvid; }
			if((srcvlocaloffset < globalparams.srcvsize) && (srcvlocaloffset + SRCBUFFER_SIZE >= globalparams.srcvsize)){ mylastsrcvid = myfirstsrcvid + globalparams.srcvsize - srcvlocaloffset; }
			
			keyy_t beginvptr = buffer2[0][0].key;
			keyy_t endvptr = buffer2[0][SRCBUFFER_SIZE].key;
			if(srcvlocaloffset >= globalparams.srcvsize){ endvptr = beginvptr; }
			if((srcvlocaloffset < globalparams.srcvsize) && (srcvlocaloffset + SRCBUFFER_SIZE >= globalparams.srcvsize)){ endvptr = buffer2[0][globalparams.srcvsize - srcvlocaloffset - 1].key; }
			
			keyy_t localbeginvptr = beginvptr - firstvptr;
			keyy_t localendvptr = endvptr - firstvptr;
			keyy_t numedges = localendvptr - localbeginvptr + 2*VECTOR_SIZE;
			if(localbeginvptr == localendvptr){ numedges = 0; }
			
			#ifdef _DEBUGMODE_CHECKS2
			if(localendvptr < localbeginvptr){ cout<<"dispatch::ERROR: localendvptr < localbeginvptr. localbeginvptr: "<<localbeginvptr<<", localendvptr: "<<localendvptr<<endl; exit(EXIT_FAILURE); }
			if(localendvptr < globalparams.edgessize){ actsutilityobj->checkptr("processallvertices(2)", myfirstsrcvid, mylastsrcvid, localbeginvptr, localendvptr, (keyvalue_t *)&kvdram[BASEOFFSET_EDGESDATA_KVS]); }
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
			
			#ifdef MERGEPROCESSEDGESANDPARTITIONSTAGE
			travstate_t ptravstate;
			ptravstate.begin_kvs = localbeginvptr_kvs;
			ptravstate.size_kvs = numedges_kvs;
			ptravstate.end_kvs = ptravstate.begin_kvs + ptravstate.size_kvs;
			ptravstate.skip_kvs = SRCBUFFER_SIZE;
			ptravstate.i_kvs = ptravstate.begin_kvs;
			partitionupdates_finegrainedpipeline_unevencutoffs(
				ON,
				kvdram,
				buffer3,
				buffer4,
				buffer5,
				buffer6,
				buffer7,
				capsule_so1,
				capsule_so2,
				capsule_so4,
				capsule_so8,
				globalstatsbuffer,
				config,
				globalparams,
				sweepparams,
				ptravstate,
				globalparams.baseoffset_edgesdata_kvs,
				globalparams.baseoffset_kvdramworkspace_kvs);
			#endif 
			#ifndef MERGEPROCESSEDGESANDPARTITIONSTAGE
			for(batch_type eoffset_kvs=localbeginvptr_kvs; eoffset_kvs<localbeginvptr_kvs + numedges_kvs; eoffset_kvs+=SRCBUFFER_SIZE){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_processedges_loadedgebatch avg=analysis_processedges_loadedgebatch
				#ifdef _DEBUGMODE_KERNELPRINTS2
				actsutilityobj->print4("### dispatch::process_edges:: eoffset", "ebegin", "esize", "eskip", eoffset_kvs * VECTOR_SIZE, localbeginvptr_kvs * VECTOR_SIZE, numedges_kvs * VECTOR_SIZE, SRCBUFFER_SIZE * VECTOR_SIZE);
				#endif
				
				edgestravstate.i_kvs = globalparams.baseoffset_edgesdata_kvs + eoffset_kvs;
	
				readkeyvalues(ON, kvdram, buffer6, globalparams.baseoffset_edgesdata_kvs + eoffset_kvs, SRCBUFFER_SIZE, edgestravstate);
				
				#ifdef EMBEDDEDCOLLECTSTATS
				process_edges(ON, buffer5, buffer6, buffer4, globalparams.srcvoffset + (voffset_kvs * VECTOR_SIZE) + (v * SRCBUFFER_SIZE), globalparams.GraphIter, globalparams.GraphAlgo, edgestravstate, globalparams);
				#else 
				process_edges(ON, buffer5, buffer6, globalparams.srcvoffset + (voffset_kvs * VECTOR_SIZE) + (v * SRCBUFFER_SIZE), globalparams.GraphIter, globalparams.GraphAlgo, edgestravstate, globalparams);
				#endif 
				
				buffer_type savechunk_size = getchunksize_kvs(SRCBUFFER_SIZE, edgestravstate, 0);
				savekeyvalues(ON, kvdram, buffer6, globalparams.baseoffset_kvdram_kvs + kvoffset_kvs, savechunk_size);
				kvoffset_kvs += savechunk_size;
			}
			#endif 

			#ifdef _DEBUGMODE_KERNELPRINTS
			actsutilityobj->printglobalvars();
			actsutilityobj->clearglobalvars();
			#endif
		}
	}
	return;
}

// process active vertices (bfs,sssp,etc.)		
buffer_type
	#ifdef SW 
	acts::
	#endif
generateoffsets(
			uint512_dt * kvdram, 
			keyvalue_t actvvs[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
			keyvalue_t offsets[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
			travstate_t actvvtravstate, 
			globalparams_t globalparams){ 
	unsigned int analysis_loop1 = (PADDEDDESTBUFFER_SIZE / 2) * VECTOR_SIZE;
	buffer_type chunk_size = getchunksize_kvs(PADDEDDESTBUFFER_SIZE / 2, actvvtravstate, 0);
	
	uint512_vec_dt kvset1;
	uint512_vec_dt kvset2;
	#pragma HLS ARRAY_PARTITION variable=kvset1 complete
	#pragma HLS ARRAY_PARTITION variable=kvset2 complete
	
	unsigned int numactvvs = 0;
	GENERATEOFFSETS_LOOP: for(batch_type index=0; index<chunk_size * VECTOR_SIZE * 2; index++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loop1 avg=analysis_loop1
	#pragma HLS PIPELINE II=2 // CRITICAL VHLS CHECKME.
		edge_t edges_beginoffset = 0;
		edge_t edges_endoffset = 0;
		batch_type edges_size = 0;

		keyvalue_t keyvalue = actvvs[(index / 2) % VECTOR_SIZE][(index / 2) / VECTOR_SIZE];

		keyy_t actvv_id;
		if(index % 2 == 0){
			actvv_id = keyvalue.key;
		} else {
			actvv_id = keyvalue.value;
		}
		value_t sourcedata = processedgefunc(sourcedata, 1, 1, globalparams.GraphIter, globalparams.GraphAlgo);
		
		#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_PROCACTVVSPRINTS)
		if(actvv_id != INVALIDDATA){ cout<<"generateoffsets:: index: "<<index<<", active vid: "<<actvv_id<<", sourcedata: "<<sourcedata<<", num actvvs: "<<chunk_size * VECTOR_SIZE<<", num actvvs processed: "<<((actvvtravstate.i_kvs * VECTOR_SIZE) + index)<<endl; }
		#endif
	
		if((actvv_id != INVALIDDATA) && ((actvvtravstate.i_kvs * VECTOR_SIZE) + index < globalparams.actvvsize)){ // NEWCHANGE.
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("generateoffsets.actvv_id 5", actvv_id, KVDATA_RANGE, (actvvtravstate.i_kvs * VECTOR_SIZE) + index, NAp, NAp);
			#endif
			
			batch_type yloc1 = (actvv_id / 2) / VECTOR_SIZE;
			vector_type xloc1 = (actvv_id / 2) % VECTOR_SIZE;
			batch_type yloc2 = ((actvv_id + 1) / 2) / VECTOR_SIZE;
			vector_type xloc2 = ((actvv_id + 1) / 2) % VECTOR_SIZE;
			
			kvset1 = getkeyvalues(kvdram, globalparams.baseoffset_vertexptr_kvs + yloc1);
			if(yloc2 != yloc1){ kvset2 = getkeyvalues(kvdram, globalparams.baseoffset_vertexptr_kvs + yloc2); }
			
			if(actvv_id % 2 == 0){ edges_beginoffset = kvset1.data[xloc1].key; } else { edges_beginoffset = kvset1.data[xloc1].value; } 
			if(yloc2 == yloc1){
				if((actvv_id + 1) % 2 == 0){ edges_endoffset = kvset1.data[xloc2].key; } else { edges_endoffset = kvset1.data[xloc2].value; } 
			} else {
				if((actvv_id + 1) % 2 == 0){ edges_endoffset = kvset2.data[xloc2].key; } else { edges_endoffset = kvset2.data[xloc2].value; } 
			}
			edges_size = edges_endoffset - edges_beginoffset;
			
			#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_PROCACTVVSPRINTS)
			cout<<"generateoffsets:generate offsets: edges_beginoffset: "<<edges_beginoffset<<", index: "<<index<<", active vid: "<<actvv_id<<", num actvvs: "<<chunk_size * VECTOR_SIZE<<endl;
			#endif
			unsigned int X = numactvvs % VECTOR_SIZE;
			unsigned int Y = numactvvs / VECTOR_SIZE;
			offsets[X][Y].key = edges_beginoffset;
			offsets[X][Y].value = edges_size;
			numactvvs += 1;
		}
	}
	return numactvvs;		
}

batch_type
	#ifdef SW 
	acts::
	#endif
processactivevertices_compactedges(
		bool_type enable,
		uint512_dt * kvdram,
		keyvalue_t actvvs[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t offsets[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		travstate_t actvvtravstate,
		globalparams_t globalparams){
	if(enable == OFF){ return 0; }
	unsigned int analysis_loop1 = 1;
	unsigned int analysis_loop1b = 1;
	unsigned int analysis_loop1c = 1;
	unsigned int analysis_loop1d = 1;
	
	uint512_vec_dt E;
	#pragma HLS ARRAY_PARTITION variable=E complete
	
	value_t buffersize_kvs = 0;
	batch_type saveoffset_kvs = 0;
	#ifdef _DEBUGMODE_STATS
	unsigned int edges_count = 0;
	unsigned int edgesdstv_sum = 0;
	keyy_t keys[COMPACTPARAM_ITEMSIZE_TOTALDATA];
	#endif

	keyvalue_t vertexupdate0;
	keyvalue_t vertex2update0;
	bool_type en0;
	bool_type e2n0;
	keyvalue_t vertexupdate1;
	keyvalue_t vertex2update1;
	bool_type en1;
	bool_type e2n1;
	keyvalue_t vertexupdate2;
	keyvalue_t vertex2update2;
	bool_type en2;
	bool_type e2n2;
	keyvalue_t vertexupdate3;
	keyvalue_t vertex2update3;
	bool_type en3;
	bool_type e2n3;
	keyvalue_t vertexupdate4;
	keyvalue_t vertex2update4;
	bool_type en4;
	bool_type e2n4;
	keyvalue_t vertexupdate5;
	keyvalue_t vertex2update5;
	bool_type en5;
	bool_type e2n5;
	keyvalue_t vertexupdate6;
	keyvalue_t vertex2update6;
	bool_type en6;
	bool_type e2n6;
	keyvalue_t vertexupdate7;
	keyvalue_t vertex2update7;
	bool_type en7;
	bool_type e2n7;
	keyvalue_t dummyvertexupdate; dummyvertexupdate.key = INVALIDDATA; dummyvertexupdate.value = INVALIDDATA;
	
	// read active vertices
	PROCACTVVS_LOOP1: for(batch_type offset_kvs=actvvtravstate.begin_kvs; offset_kvs<actvvtravstate.begin_kvs + actvvtravstate.size_kvs; offset_kvs+=PADDEDDESTBUFFER_SIZE / 2){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loop1 avg=analysis_loop1
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"### processactivevertices_compactedges: offset_kvs: "<<offset_kvs<<", actvvtravstate.begin_kvs: "<<actvvtravstate.begin_kvs<<", actvvtravstate.size_kvs: "<<actvvtravstate.size_kvs<<endl;
		#endif
		actvvtravstate.i_kvs = offset_kvs;
		
		// get offsets
		#ifndef KOOOKOOOO
		readkeyvalues(ON, kvdram, actvvs, globalparams.baseoffset_activevertices_kvs + offset_kvs, PADDEDDESTBUFFER_SIZE / 2, actvvtravstate);
		buffer_type actvvscount = generateoffsets(kvdram, actvvs, offsets, actvvtravstate, globalparams);
		#else
		readkeyvalues(ON, kvdram, offsets, globalparams.baseoffset_activevertices_kvs + offset_kvs, PADDEDDESTBUFFER_SIZE / 2, actvvtravstate);
		#endif
		
		PROCACTVVS_LOOP1B: for(batch_type actvv_index=0; actvv_index<actvvscount; actvv_index++){	
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loop1b avg=analysis_loop1b
			#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_PROCACTVVSPRINTS)
			cout<<"processactivevertices_compactedges:generate vertex updates: actvv_index: "<<actvv_index<<", num actvvs: "<<actvvscount<<endl;
			#endif
			
			edge_t edges_beginoffset = offsets[actvv_index % VECTOR_SIZE][actvv_index / VECTOR_SIZE].key;
			batch_type edges_size = offsets[actvv_index % VECTOR_SIZE][actvv_index / VECTOR_SIZE].value;
			edge_t edges_endoffset = edges_beginoffset + edges_size;
			
			batch_type edgesbegin_kvs = edges_beginoffset / VECTOR_SIZE;
			batch_type edgesize_kvs = (allignhigher_KV(edges_endoffset) - allignlower_KV(edges_beginoffset)) / VECTOR_SIZE;
			if((offset_kvs * VECTOR_SIZE) + actvv_index >= globalparams.actvvsize){ edgesize_kvs = 0; }
			batch_type edgesend_kvs = edgesbegin_kvs + edgesize_kvs;
			batch_type edgeid_kvs = edgesbegin_kvs;
				
			vector_type colstart = edges_beginoffset % VECTOR_SIZE;
			vector_type colend = (edges_endoffset-1) % VECTOR_SIZE;
			
			edge_t edgesread_kvs = 0;
			batch_type workedgesize_kvs = edgesize_kvs;
			batch_type workedgesbegin_kvs = edgesbegin_kvs;
			if(buffersize_kvs + workedgesize_kvs > PADDEDDESTBUFFER_SIZE){ workedgesize_kvs = PADDEDDESTBUFFER_SIZE - buffersize_kvs; }
			#ifdef _DEBUGMODE_CHECKS2
			unsigned int errcount = 0;
			#endif
			
			#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_PROCACTVVSPRINTS)
			cout<<"generateoffsets:processactivevertices_compactedges: edges_beginoffset: "<<edges_beginoffset<<", actvv_index: "<<actvv_index<<endl;
			#endif
				
			PROCACTVVS_LOOP1C: while(true){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loop1c avg=analysis_loop1c
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("workedgesize_kvs 45", workedgesize_kvs, PADDEDDESTBUFFER_SIZE+1, NAp, NAp, NAp);
				#endif 
				
				PROCACTVVS_LOOP1D: for(edgeid_kvs=workedgesbegin_kvs; edgeid_kvs<workedgesbegin_kvs + workedgesize_kvs; edgeid_kvs++){
				#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loop1d avg=analysis_loop1d
				#pragma HLS PIPELINE II=1
					#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_PROCACTVVSPRINTS)
					cout<<"processactivevertices_compactedges: edgeid_kvs: "<<edgeid_kvs<<", workedgesbegin_kvs: "<<workedgesbegin_kvs<<", workedgesize_kvs: "<<workedgesize_kvs<<endl;
					#endif
					
					E = getkeyvalues(kvdram, globalparams.baseoffset_edgesdata_kvs + edgeid_kvs);
					
					vertexupdate0 = E.data[0];
					vertexupdate1 = E.data[1];
					vertexupdate2 = E.data[2];
					vertexupdate3 = E.data[3];
					vertexupdate4 = E.data[4];
					vertexupdate5 = E.data[5];
					vertexupdate6 = E.data[6];
					vertexupdate7 = E.data[7];
					
					if(((edgeid_kvs == edgesbegin_kvs) && (0 < colstart)) || ((edgeid_kvs == edgesend_kvs-1) && (0 > colend))){ en0 = OFF; }
					else { en0 = ON; }
					if(((edgeid_kvs == edgesbegin_kvs) && (1 < colstart)) || ((edgeid_kvs == edgesend_kvs-1) && (1 > colend))){ en1 = OFF; }
					else { en1 = ON; }
					if(((edgeid_kvs == edgesbegin_kvs) && (2 < colstart)) || ((edgeid_kvs == edgesend_kvs-1) && (2 > colend))){ en2 = OFF; }
					else { en2 = ON; }
					if(((edgeid_kvs == edgesbegin_kvs) && (3 < colstart)) || ((edgeid_kvs == edgesend_kvs-1) && (3 > colend))){ en3 = OFF; }
					else { en3 = ON; }
					if(((edgeid_kvs == edgesbegin_kvs) && (4 < colstart)) || ((edgeid_kvs == edgesend_kvs-1) && (4 > colend))){ en4 = OFF; }
					else { en4 = ON; }
					if(((edgeid_kvs == edgesbegin_kvs) && (5 < colstart)) || ((edgeid_kvs == edgesend_kvs-1) && (5 > colend))){ en5 = OFF; }
					else { en5 = ON; }
					if(((edgeid_kvs == edgesbegin_kvs) && (6 < colstart)) || ((edgeid_kvs == edgesend_kvs-1) && (6 > colend))){ en6 = OFF; }
					else { en6 = ON; }
					if(((edgeid_kvs == edgesbegin_kvs) && (7 < colstart)) || ((edgeid_kvs == edgesend_kvs-1) && (7 > colend))){ en7 = OFF; }
					else { en7 = ON; }
					
					if(en0 == ON){ buffer1[0][buffersize_kvs] = vertexupdate0; }
					else { buffer1[0][buffersize_kvs] = dummyvertexupdate; }
					if(en1 == ON){ buffer1[1][buffersize_kvs] = vertexupdate1; }
					else { buffer1[1][buffersize_kvs] = dummyvertexupdate; }
					if(en2 == ON){ buffer1[2][buffersize_kvs] = vertexupdate2; }
					else { buffer1[2][buffersize_kvs] = dummyvertexupdate; }
					if(en3 == ON){ buffer1[3][buffersize_kvs] = vertexupdate3; }
					else { buffer1[3][buffersize_kvs] = dummyvertexupdate; }
					if(en4 == ON){ buffer1[4][buffersize_kvs] = vertexupdate4; }
					else { buffer1[4][buffersize_kvs] = dummyvertexupdate; }
					if(en5 == ON){ buffer1[5][buffersize_kvs] = vertexupdate5; }
					else { buffer1[5][buffersize_kvs] = dummyvertexupdate; }
					if(en6 == ON){ buffer1[6][buffersize_kvs] = vertexupdate6; }
					else { buffer1[6][buffersize_kvs] = dummyvertexupdate; }
					if(en7 == ON){ buffer1[7][buffersize_kvs] = vertexupdate7; }
					else { buffer1[7][buffersize_kvs] = dummyvertexupdate; }
					
					buffersize_kvs += 1;
					
					#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_PROCACTVVSPRINTS)
					actsutilityobj->PARSE("E.data[0]", E.data[0]);
					if(en0 == ON){ actsutilityobj->PARSE("vertexupdate0", vertexupdate0); }
					actsutilityobj->PARSE("E.data[1]", E.data[1]);
					if(en1 == ON){ actsutilityobj->PARSE("vertexupdate1", vertexupdate1); }
					actsutilityobj->PARSE("E.data[2]", E.data[2]);
					if(en2 == ON){ actsutilityobj->PARSE("vertexupdate2", vertexupdate2); }
					actsutilityobj->PARSE("E.data[3]", E.data[3]);
					if(en3 == ON){ actsutilityobj->PARSE("vertexupdate3", vertexupdate3); }
					actsutilityobj->PARSE("E.data[4]", E.data[4]);
					if(en4 == ON){ actsutilityobj->PARSE("vertexupdate4", vertexupdate4); }
					actsutilityobj->PARSE("E.data[5]", E.data[5]);
					if(en5 == ON){ actsutilityobj->PARSE("vertexupdate5", vertexupdate5); }
					actsutilityobj->PARSE("E.data[6]", E.data[6]);
					if(en6 == ON){ actsutilityobj->PARSE("vertexupdate6", vertexupdate6); }
					actsutilityobj->PARSE("E.data[7]", E.data[7]);
					if(en7 == ON){ actsutilityobj->PARSE("vertexupdate7", vertexupdate7); }
					#endif 
					#ifdef _DEBUGMODE_CHECKS2
					actsutilityobj->checkoutofbounds("buffersize_kvs 45", buffersize_kvs, PADDEDDESTBUFFER_SIZE + 1, workedgesbegin_kvs, edgeid_kvs, workedgesize_kvs);
					#endif
					#ifdef _DEBUGMODE_STATS
					actsutilityobj->globalstats_countkvsprocessed(VECTOR_SIZE);
					if(en0 == ON){ unsigned int numitems0 = actsutilityobj->PARSE(vertexupdate0, keys);
						actsutilityobj->globalstats_processedges_countvalidkvsprocessed(numitems0); edges_count += numitems0; 
						for(unsigned int t=0; t<numitems0; t++){ edgesdstv_sum += keys[t]; }}
					if(en1 == ON){ unsigned int numitems1 = actsutilityobj->PARSE(vertexupdate1, keys);
						actsutilityobj->globalstats_processedges_countvalidkvsprocessed(numitems1); edges_count += numitems1; 
						for(unsigned int t=0; t<numitems1; t++){ edgesdstv_sum += keys[t]; }}
					if(en2 == ON){ unsigned int numitems2 = actsutilityobj->PARSE(vertexupdate2, keys);
						actsutilityobj->globalstats_processedges_countvalidkvsprocessed(numitems2); edges_count += numitems2; 
						for(unsigned int t=0; t<numitems2; t++){ edgesdstv_sum += keys[t]; }}
					if(en3 == ON){ unsigned int numitems3 = actsutilityobj->PARSE(vertexupdate3, keys);
						actsutilityobj->globalstats_processedges_countvalidkvsprocessed(numitems3); edges_count += numitems3; 
						for(unsigned int t=0; t<numitems3; t++){ edgesdstv_sum += keys[t]; }}
					if(en4 == ON){ unsigned int numitems4 = actsutilityobj->PARSE(vertexupdate4, keys);
						actsutilityobj->globalstats_processedges_countvalidkvsprocessed(numitems4); edges_count += numitems4; 
						for(unsigned int t=0; t<numitems4; t++){ edgesdstv_sum += keys[t]; }}
					if(en5 == ON){ unsigned int numitems5 = actsutilityobj->PARSE(vertexupdate5, keys);
						actsutilityobj->globalstats_processedges_countvalidkvsprocessed(numitems5); edges_count += numitems5; 
						for(unsigned int t=0; t<numitems5; t++){ edgesdstv_sum += keys[t]; }}
					if(en6 == ON){ unsigned int numitems6 = actsutilityobj->PARSE(vertexupdate6, keys);
						actsutilityobj->globalstats_processedges_countvalidkvsprocessed(numitems6); edges_count += numitems6; 
						for(unsigned int t=0; t<numitems6; t++){ edgesdstv_sum += keys[t]; }}
					if(en7 == ON){ unsigned int numitems7 = actsutilityobj->PARSE(vertexupdate7, keys);
						actsutilityobj->globalstats_processedges_countvalidkvsprocessed(numitems7); edges_count += numitems7; 
						for(unsigned int t=0; t<numitems7; t++){ edgesdstv_sum += keys[t]; }}
					#endif
				}
				
				if((buffersize_kvs >= PADDEDDESTBUFFER_SIZE) || ((offset_kvs * VECTOR_SIZE) + actvv_index == globalparams.actvvsize-1)){
					#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_PROCACTVVSPRINTS)
					cout<<"processactivevertices_compactedges: saving keyvalues @ actvv_index("<<actvv_index<<")... saveoffset_kvs: "<<saveoffset_kvs<<", buffersize_kvs: "<<buffersize_kvs<<endl;
					#endif 
					savekeyvalues(ON, kvdram, buffer1, globalparams.baseoffset_kvdram_kvs + saveoffset_kvs, buffersize_kvs);
					saveoffset_kvs += buffersize_kvs;
					buffersize_kvs = 0;
				}
				
				edgesread_kvs += workedgesize_kvs;
				workedgesbegin_kvs += workedgesize_kvs;
				workedgesize_kvs = edgesize_kvs - edgesread_kvs;
				if(buffersize_kvs + workedgesize_kvs > PADDEDDESTBUFFER_SIZE){ workedgesize_kvs = PADDEDDESTBUFFER_SIZE - buffersize_kvs; }
				if(workedgesize_kvs == 0){ break; }
				#ifdef _DEBUGMODE_CHECKS2 // FIXME.
				if(edgesize_kvs < workedgesize_kvs){ cout<<"processactivevertices_compactedges: ERROR: edgesize_kvs < workedgesize_kvs. exiting..."<<endl; exit(EXIT_FAILURE); }
				if(errcount++ > 64){ cout<<"processactivevertices_compactedges:ERROR: errcount("<<errcount<<") > 312. exiting..."<<endl; exit(EXIT_FAILURE); }
				#endif
			}
		}
	}
	
	#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_PROCACTVVSPRINTS)
	cout<<"processactivevertices_compactedges: saving keyvalues @ actvv_index(NAp)... saveoffset_kvs: "<<saveoffset_kvs<<", buffersize_kvs: "<<buffersize_kvs<<endl;
	#endif 
	savekeyvalues(ON, kvdram, buffer1, globalparams.baseoffset_kvdram_kvs + saveoffset_kvs, buffersize_kvs);
	saveoffset_kvs += buffersize_kvs;
	buffersize_kvs = 0;
	
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("processactivevertices_compactedges 44", saveoffset_kvs, KVDRAMSZ_KVS, NAp, NAp, NAp);
	#endif
	#ifdef _DEBUGMODE_STATS
	setkey(kvdram, PADDEDKVSOURCEDRAMSZ_KVS-1, 0, edges_count);
	setkey(kvdram, PADDEDKVSOURCEDRAMSZ_KVS-1, 1, edgesdstv_sum);
	setkey(kvdram, PADDEDKVSOURCEDRAMSZ_KVS-1, 2, saveoffset_kvs);
	#endif
	return saveoffset_kvs;
}

// process edges phase for non compacted graph (bfs,sssp,etc.)		
batch_type
	#ifdef SW 
	acts::
	#endif
processactivevertices_noncompactedges(
		bool_type enable,
		uint512_dt * kvdram,
		keyvalue_t actvvs[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		travstate_t actvvtravstate,
		globalparams_t globalparams){
	if(enable == OFF){ return 0; }
	
	uint512_vec_dt E;
	#pragma HLS ARRAY_PARTITION variable=E complete
	
	value_t buffersize_kvs = 0;
	batch_type saveoffset_kvs = 0;
	
	#ifdef _DEBUGMODE_STATS
	unsigned int edges_count = 0;
	unsigned int edgesdstv_sum = 0;
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"processactivevertices_noncompactedges: actvvtravstate.begin_kvs: "<<actvvtravstate.begin_kvs<<endl;
	cout<<"processactivevertices_noncompactedges: actvvtravstate.size_kvs: "<<actvvtravstate.size_kvs<<endl;	
	cout<<"processactivevertices_noncompactedges: globalparams.actvvsize: "<<globalparams.actvvsize<<endl;	
	#endif
	
	for(batch_type offset_kvs=actvvtravstate.begin_kvs; offset_kvs<actvvtravstate.begin_kvs + actvvtravstate.size_kvs; offset_kvs+=PADDEDDESTBUFFER_SIZE){
		#ifdef _DEBUGMODE_KERNELPRINTS2
		cout<<"### processactivevertices_noncompactedges: offset_kvs: "<<offset_kvs<<", actvvtravstate.begin_kvs: "<<actvvtravstate.begin_kvs<<", actvvtravstate.size_kvs: "<<actvvtravstate.size_kvs<<endl;
		#endif
		
		actvvtravstate.i_kvs = offset_kvs;
		readkeyvalues(ON, kvdram, actvvs, globalparams.baseoffset_activevertices_kvs + offset_kvs, PADDEDDESTBUFFER_SIZE, actvvtravstate);
		
		buffer_type chunk_size = getchunksize_kvs(PADDEDDESTBUFFER_SIZE, actvvtravstate, 0);
		for(batch_type actvv_id=0; actvv_id<chunk_size * VECTOR_SIZE; actvv_id++){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<endl<<"processactivevertices_noncompactedges: actvv_id: "<<actvv_id<<", sz: "<<chunk_size * VECTOR_SIZE<<endl;
			#endif 
		
			keyvalue_t activevertex = actvvs[actvv_id % VECTOR_SIZE][actvv_id / VECTOR_SIZE];
			value_t sourcedata = activevertex.value;
			value_t updateval = processedgefunc(sourcedata, 1, 1, globalparams.GraphIter, globalparams.GraphAlgo); 
			
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"processactivevertices_noncompactedges: actvv_id: "<<actvv_id<<endl;
			cout<<"processactivevertices_noncompactedges: activevertex.key (vid): "<<activevertex.key<<endl;
			cout<<"processactivevertices_noncompactedges: activevertex.value (vdata): "<<activevertex.value<<endl;
			#endif 
			
			vector_type yloc;
			vector_type xloc;
			keyvalue_t keyvalue;
			edge_t edges_beginoffset;
			edge_t edges_endoffset;
			
			yloc = (activevertex.key / 2) / VECTOR_SIZE;
			xloc = (activevertex.key / 2) % VECTOR_SIZE;
			// keyvalue = kvdram[globalparams.baseoffset_vertexptr_kvs + yloc].data[xloc];
			keyvalue = getkeyvalue(kvdram, globalparams.baseoffset_vertexptr_kvs + yloc, xloc);
			if(activevertex.key % 2 == 0){ edges_beginoffset = keyvalue.key; }
			else { edges_beginoffset = keyvalue.value; } 
			
			yloc = ((activevertex.key + 1) / 2) / VECTOR_SIZE;
			xloc = ((activevertex.key + 1) / 2) % VECTOR_SIZE;
			// keyvalue = kvdram[globalparams.baseoffset_vertexptr_kvs + yloc].data[xloc];
			keyvalue = getkeyvalue(kvdram, globalparams.baseoffset_vertexptr_kvs + yloc, xloc);
			if((activevertex.key + 1) % 2 == 0){ edges_endoffset = keyvalue.key; }
			else { edges_endoffset = keyvalue.value; } 
			
			batch_type edges_size = edges_endoffset - edges_beginoffset;
			
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"processactivevertices_noncompactedges: sourcedata: "<<sourcedata<<endl;
			cout<<"processactivevertices_noncompactedges: edges_beginoffset: "<<edges_beginoffset<<endl;	
			cout<<"processactivevertices_noncompactedges: edges_endoffset: "<<edges_endoffset<<endl;	
			cout<<"processactivevertices_noncompactedges: edges_size: "<<edges_size<<endl;
			#endif 
			
			batch_type edgesbegin_kvs = edges_beginoffset / VECTOR2_SIZE;
			batch_type edgesize_kvs = (allignhigherto16_KV(edges_endoffset) - allignlowerto16_KV(edges_beginoffset)) / VECTOR2_SIZE;
			
			if((offset_kvs * VECTOR_SIZE) + actvv_id >= globalparams.actvvsize){ edgesize_kvs = 0; } // IAMPROBLEM?
			batch_type edgesend_kvs = edgesbegin_kvs + edgesize_kvs;
			batch_type edgeid_kvs = edgesbegin_kvs;
			
			#ifdef _DEBUGMODE_KERNELPRINTS
			if((offset_kvs * VECTOR_SIZE) + actvv_id >= globalparams.actvvsize){ cout<<"INVALID active vertex entry. skipping..."<<endl; edgesize_kvs = 0; } //
			cout<<"processactivevertices_noncompactedges: edgesbegin_kvs: "<<edgesbegin_kvs<<endl;
			cout<<"processactivevertices_noncompactedges: edgesize_kvs: "<<edgesize_kvs<<endl;
			cout<<"processactivevertices_noncompactedges: edgesend_kvs: "<<edgesend_kvs<<endl;
			cout<<"processactivevertices_noncompactedges: edgeid_kvs: "<<edgeid_kvs<<endl;
			#endif
			
			vector_type colstart = edges_beginoffset % VECTOR2_SIZE;
			vector_type colend = (edges_endoffset-1) % VECTOR2_SIZE;
			#ifdef _DEBUGMODE_CHECKS2
			if(edges_endoffset < 1 && edgesize_kvs != 0){ cout<<"processactivevertices_noncompactedges: ERROR: edges_endoffset < 1. edges_endoffset: "<<edges_endoffset<<endl; exit(EXIT_FAILURE); }
			#endif
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"processactivevertices_noncompactedges: colstart: "<<colstart<<", colend: "<<colend<<endl;
			#endif
			
			keyvalue_t vertexupdate0;
			keyvalue_t vertex2update0;
			bool_type en0;
			bool_type e2n0;
			keyvalue_t vertexupdate1;
			keyvalue_t vertex2update1;
			bool_type en1;
			bool_type e2n1;
			keyvalue_t vertexupdate2;
			keyvalue_t vertex2update2;
			bool_type en2;
			bool_type e2n2;
			keyvalue_t vertexupdate3;
			keyvalue_t vertex2update3;
			bool_type en3;
			bool_type e2n3;
			keyvalue_t vertexupdate4;
			keyvalue_t vertex2update4;
			bool_type en4;
			bool_type e2n4;
			keyvalue_t vertexupdate5;
			keyvalue_t vertex2update5;
			bool_type en5;
			bool_type e2n5;
			keyvalue_t vertexupdate6;
			keyvalue_t vertex2update6;
			bool_type en6;
			bool_type e2n6;
			keyvalue_t vertexupdate7;
			keyvalue_t vertex2update7;
			bool_type en7;
			bool_type e2n7;
			keyvalue_t dummyvertexupdate; dummyvertexupdate.key = INVALIDDATA; dummyvertexupdate.value = INVALIDDATA;
			
			unsigned int edgesread_kvs = 0;
			batch_type workedgesize_kvs = edgesize_kvs;
			batch_type workedgesbegin_kvs = edgesbegin_kvs;
			if(buffersize_kvs + workedgesize_kvs > PADDEDDESTBUFFER_SIZE){ workedgesize_kvs = PADDEDDESTBUFFER_SIZE - buffersize_kvs; }
			#ifdef _DEBUGMODE_CHECKS2
			unsigned int errcount = 0;
			#endif
			
			while(true){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("buffer2 45", workedgesize_kvs, PADDEDDESTBUFFER_SIZE+1, NAp, NAp, NAp);
				#endif 
				
				for(edgeid_kvs=workedgesbegin_kvs; edgeid_kvs<workedgesbegin_kvs + workedgesize_kvs; edgeid_kvs++){
				#pragma HLS PIPELINE II=1
					#ifdef _DEBUGMODE_KERNELPRINTS
					cout<<"processactivevertices_noncompactedges.for: edgeid_kvs: "<<edgeid_kvs<<", workedgesbegin_kvs: "<<workedgesbegin_kvs<<", workedgesize_kvs: "<<workedgesize_kvs<<endl;
					#endif
					
					E = getkeyvalues(kvdram, globalparams.baseoffset_edgesdata_kvs + edgeid_kvs);
					#ifdef _DEBUGMODE_KERNELPRINTS
					cout<<"processactivevertices_noncompactedges: E.data[0].key: "<<E.data[0].key<<", E.data[0].value: "<<E.data[0].value<<endl;
					cout<<"processactivevertices_noncompactedges: E.data[1].key: "<<E.data[1].key<<", E.data[1].value: "<<E.data[1].value<<endl;
					cout<<"processactivevertices_noncompactedges: E.data[2].key: "<<E.data[2].key<<", E.data[2].value: "<<E.data[2].value<<endl;
					cout<<"processactivevertices_noncompactedges: E.data[3].key: "<<E.data[3].key<<", E.data[3].value: "<<E.data[3].value<<endl;
					cout<<"processactivevertices_noncompactedges: E.data[4].key: "<<E.data[4].key<<", E.data[4].value: "<<E.data[4].value<<endl;
					cout<<"processactivevertices_noncompactedges: E.data[5].key: "<<E.data[5].key<<", E.data[5].value: "<<E.data[5].value<<endl;
					cout<<"processactivevertices_noncompactedges: E.data[6].key: "<<E.data[6].key<<", E.data[6].value: "<<E.data[6].value<<endl;
					cout<<"processactivevertices_noncompactedges: E.data[7].key: "<<E.data[7].key<<", E.data[7].value: "<<E.data[7].value<<endl;
					#endif
					
					vertexupdate0.key = E.data[0].key;
					vertexupdate0.value = updateval; 
					vertexupdate1.key = E.data[0].value;
					vertexupdate1.value = updateval; 
					vertexupdate2.key = E.data[1].key;
					vertexupdate2.value = updateval; 
					vertexupdate3.key = E.data[1].value;
					vertexupdate3.value = updateval; 
					vertexupdate4.key = E.data[2].key;
					vertexupdate4.value = updateval; 
					vertexupdate5.key = E.data[2].value;
					vertexupdate5.value = updateval; 
					vertexupdate6.key = E.data[3].key;
					vertexupdate6.value = updateval; 
					vertexupdate7.key = E.data[3].value;
					vertexupdate7.value = updateval; 
					
					vertex2update0.key = E.data[4].key;
					vertex2update0.value = updateval; 
					vertex2update1.key = E.data[4].value;
					vertex2update1.value = updateval;
					vertex2update2.key = E.data[5].key;
					vertex2update2.value = updateval; 
					vertex2update3.key = E.data[5].value;
					vertex2update3.value = updateval;
					vertex2update4.key = E.data[6].key;
					vertex2update4.value = updateval; 
					vertex2update5.key = E.data[6].value;
					vertex2update5.value = updateval;
					vertex2update6.key = E.data[7].key;
					vertex2update6.value = updateval; 
					vertex2update7.key = E.data[7].value;
					vertex2update7.value = updateval;
					
					if(((edgeid_kvs == edgesbegin_kvs) && (0 < colstart)) || ((edgeid_kvs == edgesend_kvs-1) && (0 > colend))){ en0 = OFF; }
					else { en0 = ON; }
					if(((edgeid_kvs == edgesbegin_kvs) && (1 < colstart)) || ((edgeid_kvs == edgesend_kvs-1) && (1 > colend))){ en1 = OFF; }
					else { en1 = ON; }
					if(((edgeid_kvs == edgesbegin_kvs) && (2 < colstart)) || ((edgeid_kvs == edgesend_kvs-1) && (2 > colend))){ en2 = OFF; }
					else { en2 = ON; }
					if(((edgeid_kvs == edgesbegin_kvs) && (3 < colstart)) || ((edgeid_kvs == edgesend_kvs-1) && (3 > colend))){ en3 = OFF; }
					else { en3 = ON; }
					if(((edgeid_kvs == edgesbegin_kvs) && (4 < colstart)) || ((edgeid_kvs == edgesend_kvs-1) && (4 > colend))){ en4 = OFF; }
					else { en4 = ON; }
					if(((edgeid_kvs == edgesbegin_kvs) && (5 < colstart)) || ((edgeid_kvs == edgesend_kvs-1) && (5 > colend))){ en5 = OFF; }
					else { en5 = ON; }
					if(((edgeid_kvs == edgesbegin_kvs) && (6 < colstart)) || ((edgeid_kvs == edgesend_kvs-1) && (6 > colend))){ en6 = OFF; }
					else { en6 = ON; }
					if(((edgeid_kvs == edgesbegin_kvs) && (7 < colstart)) || ((edgeid_kvs == edgesend_kvs-1) && (7 > colend))){ en7 = OFF; }
					else { en7 = ON; }
					
					if(((edgeid_kvs == edgesbegin_kvs) && (8 < colstart)) || ((edgeid_kvs == edgesend_kvs-1) && (8 > colend))){ e2n0 = OFF; }
					else { e2n0 = ON; }
					if(((edgeid_kvs == edgesbegin_kvs) && (9 < colstart)) || ((edgeid_kvs == edgesend_kvs-1) && (9 > colend))){ e2n1 = OFF; }
					else { e2n1 = ON; }
					if(((edgeid_kvs == edgesbegin_kvs) && (10 < colstart)) || ((edgeid_kvs == edgesend_kvs-1) && (10 > colend))){ e2n2 = OFF; }
					else { e2n2 = ON; }
					if(((edgeid_kvs == edgesbegin_kvs) && (11 < colstart)) || ((edgeid_kvs == edgesend_kvs-1) && (11 > colend))){ e2n3 = OFF; }
					else { e2n3 = ON; }
					if(((edgeid_kvs == edgesbegin_kvs) && (12 < colstart)) || ((edgeid_kvs == edgesend_kvs-1) && (12 > colend))){ e2n4 = OFF; }
					else { e2n4 = ON; }
					if(((edgeid_kvs == edgesbegin_kvs) && (13 < colstart)) || ((edgeid_kvs == edgesend_kvs-1) && (13 > colend))){ e2n5 = OFF; }
					else { e2n5 = ON; }
					if(((edgeid_kvs == edgesbegin_kvs) && (14 < colstart)) || ((edgeid_kvs == edgesend_kvs-1) && (14 > colend))){ e2n6 = OFF; }
					else { e2n6 = ON; }
					if(((edgeid_kvs == edgesbegin_kvs) && (15 < colstart)) || ((edgeid_kvs == edgesend_kvs-1) && (15 > colend))){ e2n7 = OFF; }
					else { e2n7 = ON; }
					
					if(en0 == ON){ 
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<"processactivevertices_noncompactedges: vertexupdate0.key: "<<vertexupdate0.key<<", vertexupdate0.value: "<<vertexupdate0.value<<endl;
						#endif
						buffer1[0][buffersize_kvs] = vertexupdate0; }
					else { buffer1[0][buffersize_kvs] = dummyvertexupdate; }
					if(en1 == ON){ 
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<"processactivevertices_noncompactedges: vertexupdate1.key: "<<vertexupdate1.key<<", vertexupdate1.value: "<<vertexupdate1.value<<endl;
						#endif
						buffer1[1][buffersize_kvs] = vertexupdate1; }
					else { buffer1[1][buffersize_kvs] = dummyvertexupdate; }
					if(en2 == ON){ 
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<"processactivevertices_noncompactedges: vertexupdate2.key: "<<vertexupdate2.key<<", vertexupdate2.value: "<<vertexupdate2.value<<endl;
						#endif
						buffer1[2][buffersize_kvs] = vertexupdate2; }
					else { buffer1[2][buffersize_kvs] = dummyvertexupdate; }
					if(en3 == ON){ 
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<"processactivevertices_noncompactedges: vertexupdate3.key: "<<vertexupdate3.key<<", vertexupdate3.value: "<<vertexupdate3.value<<endl;
						#endif
						buffer1[3][buffersize_kvs] = vertexupdate3; }
					else { buffer1[3][buffersize_kvs] = dummyvertexupdate; }
					if(en4 == ON){ 
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<"processactivevertices_noncompactedges: vertexupdate4.key: "<<vertexupdate4.key<<", vertexupdate4.value: "<<vertexupdate4.value<<endl;
						#endif
						buffer1[4][buffersize_kvs] = vertexupdate4; }
					else { buffer1[4][buffersize_kvs] = dummyvertexupdate; }
					if(en5 == ON){ 
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<"processactivevertices_noncompactedges: vertexupdate5.key: "<<vertexupdate5.key<<", vertexupdate5.value: "<<vertexupdate5.value<<endl;
						#endif
						buffer1[5][buffersize_kvs] = vertexupdate5; }
					else { buffer1[5][buffersize_kvs] = dummyvertexupdate; }
					if(en6 == ON){ 
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<"processactivevertices_noncompactedges: vertexupdate6.key: "<<vertexupdate6.key<<", vertexupdate6.value: "<<vertexupdate6.value<<endl;
						#endif
						buffer1[6][buffersize_kvs] = vertexupdate6; }
					else { buffer1[6][buffersize_kvs] = dummyvertexupdate; }
					if(en7 == ON){ 
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<"processactivevertices_noncompactedges: vertexupdate7.key: "<<vertexupdate7.key<<", vertexupdate7.value: "<<vertexupdate7.value<<endl;
						#endif
						buffer1[7][buffersize_kvs] = vertexupdate7; }
					else { buffer1[7][buffersize_kvs] = dummyvertexupdate; }
					
					if(e2n0 == ON){ 
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<"processactivevertices_noncompactedges: vertex2update0.key: "<<vertex2update0.key<<", vertex2update0.value: "<<vertex2update0.value<<endl;
						#endif
						buffer2[0][buffersize_kvs] = vertex2update0; }
					else { buffer2[0][buffersize_kvs] = dummyvertexupdate; }
					if(e2n1 == ON){ 
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<"processactivevertices_noncompactedges: vertex2update1.key: "<<vertex2update1.key<<", vertex2update1.value: "<<vertex2update1.value<<endl;
						#endif
						buffer2[1][buffersize_kvs] = vertex2update1; }
					else { buffer2[1][buffersize_kvs] = dummyvertexupdate; }
					if(e2n2 == ON){ 
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<"processactivevertices_noncompactedges: vertex2update2.key: "<<vertex2update2.key<<", vertex2update2.value: "<<vertex2update2.value<<endl;
						#endif
						buffer2[2][buffersize_kvs] = vertex2update2; }
					else { buffer2[2][buffersize_kvs] = dummyvertexupdate; }
					if(e2n3 == ON){ 
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<"processactivevertices_noncompactedges: vertex2update3.key: "<<vertex2update3.key<<", vertex2update3.value: "<<vertex2update3.value<<endl;
						#endif
						buffer2[3][buffersize_kvs] = vertex2update3; }
					else { buffer2[3][buffersize_kvs] = dummyvertexupdate; }
					if(e2n4 == ON){ 
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<"processactivevertices_noncompactedges: vertex2update4.key: "<<vertex2update4.key<<", vertex2update4.value: "<<vertex2update4.value<<endl;
						#endif
						buffer2[4][buffersize_kvs] = vertex2update4; }
					else { buffer2[4][buffersize_kvs] = dummyvertexupdate; }
					if(e2n5 == ON){ 
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<"processactivevertices_noncompactedges: vertex2update5.key: "<<vertex2update5.key<<", vertex2update5.value: "<<vertex2update5.value<<endl;
						#endif
						buffer2[5][buffersize_kvs] = vertex2update5; }
					else { buffer2[5][buffersize_kvs] = dummyvertexupdate; }
					if(e2n6 == ON){ 
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<"processactivevertices_noncompactedges: vertex2update6.key: "<<vertex2update6.key<<", vertex2update6.value: "<<vertex2update6.value<<endl;
						#endif
						buffer2[6][buffersize_kvs] = vertex2update6; }
					else { buffer2[6][buffersize_kvs] = dummyvertexupdate; }
					if(e2n7 == ON){ 
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<"processactivevertices_noncompactedges: vertex2update7.key: "<<vertex2update7.key<<", vertex2update7.value: "<<vertex2update7.value<<endl;
						#endif
						buffer2[7][buffersize_kvs] = vertex2update7; }
					else { buffer2[7][buffersize_kvs] = dummyvertexupdate; }
					
					#ifdef _DEBUGMODE_CHECKS2
					actsutilityobj->checkoutofbounds("buffer2 45", buffersize_kvs, PADDEDDESTBUFFER_SIZE, workedgesbegin_kvs, edgeid_kvs, workedgesize_kvs);
					#endif
					#ifdef _DEBUGMODE_STATS
					actsutilityobj->globalstats_countkvsprocessed(VECTOR2_SIZE);
					if(en0 == ON){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); edges_count += 1; edgesdstv_sum += vertexupdate0.key; }
					if(en1 == ON){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); edges_count += 1; edgesdstv_sum += vertexupdate1.key; }
					if(en2 == ON){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); edges_count += 1; edgesdstv_sum += vertexupdate2.key; }
					if(en3 == ON){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); edges_count += 1; edgesdstv_sum += vertexupdate3.key; }
					if(en4 == ON){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); edges_count += 1; edgesdstv_sum += vertexupdate4.key; }
					if(en5 == ON){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); edges_count += 1; edgesdstv_sum += vertexupdate5.key; }
					if(en6 == ON){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); edges_count += 1; edgesdstv_sum += vertexupdate6.key; }
					if(en7 == ON){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); edges_count += 1; edgesdstv_sum += vertexupdate7.key; }
					if(e2n0 == ON){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); edges_count += 1; edgesdstv_sum += vertex2update0.key; }
					if(e2n1 == ON){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); edges_count += 1; edgesdstv_sum += vertex2update1.key; }
					if(e2n2 == ON){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); edges_count += 1; edgesdstv_sum += vertex2update2.key; }
					if(e2n3 == ON){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); edges_count += 1; edgesdstv_sum += vertex2update3.key; }
					if(e2n4 == ON){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); edges_count += 1; edgesdstv_sum += vertex2update4.key; }
					if(e2n5 == ON){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); edges_count += 1; edgesdstv_sum += vertex2update5.key; }
					if(e2n6 == ON){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); edges_count += 1; edgesdstv_sum += vertex2update6.key; }
					if(e2n7 == ON){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); edges_count += 1; edgesdstv_sum += vertex2update7.key; }
					#endif
					
					buffersize_kvs += 1;
				}
				if((buffersize_kvs >= PADDEDDESTBUFFER_SIZE) || ((offset_kvs * VECTOR_SIZE) + actvv_id == globalparams.actvvsize-1)){
					#ifdef _DEBUGMODE_KERNELPRINTS2
					cout<<"processactivevertices_noncompactedges: saving keyvalues @ actvv_id("<<actvv_id<<")... saveoffset_kvs: "<<saveoffset_kvs<<", buffersize_kvs: "<<buffersize_kvs<<endl;
					#endif 
					savekeyvalues(ON, kvdram, buffer1, globalparams.baseoffset_kvdram_kvs + saveoffset_kvs, buffersize_kvs);
					savekeyvalues(ON, kvdram, buffer2, globalparams.baseoffset_kvdram_kvs + saveoffset_kvs + buffersize_kvs, buffersize_kvs);
					saveoffset_kvs += 2 * buffersize_kvs;
					buffersize_kvs = 0;
				}
				
				edgesread_kvs += workedgesize_kvs;
				workedgesbegin_kvs += workedgesize_kvs;
				workedgesize_kvs = edgesize_kvs - edgesread_kvs;
				
				#ifdef _DEBUGMODE_CHECKS2
				if(edgesize_kvs < workedgesize_kvs){ cout<<"processactivevertices_noncompactedges: ERROR: edgesize_kvs < workedgesize_kvs. exiting..."<<endl; exit(EXIT_FAILURE); }
				#endif
				if(buffersize_kvs + workedgesize_kvs > PADDEDDESTBUFFER_SIZE){ workedgesize_kvs = PADDEDDESTBUFFER_SIZE - buffersize_kvs; }
				if(workedgesize_kvs == 0){ break; }
				#ifdef _DEBUGMODE_CHECKS2 // FIXME.
				if(errcount++ > 64){ cout<<"processactivevertices_noncompactedges:ERROR: errcount("<<errcount<<") > 312. exiting..."<<endl; exit(EXIT_FAILURE); }
				#endif
			}
		}
	}
	#ifdef _DEBUGMODE_STATS
	setkey(kvdram, PADDEDKVSOURCEDRAMSZ_KVS-1, 0, edges_count);
	setkey(kvdram, PADDEDKVSOURCEDRAMSZ_KVS-1, 1, edgesdstv_sum);
	setkey(kvdram, PADDEDKVSOURCEDRAMSZ_KVS-1, 2, saveoffset_kvs);
	#endif
	return saveoffset_kvs;
}

// main function
void
	#ifdef SW 
	acts::
	#endif 
dispatch(uint512_dt * kvdram){
	// NOTE: CRITICAL AUTOMATEME: LOADFACTORFORREDUCE
	analysis_type analysis_numllops = 1;
	analysis_type analysis_numsourcepartitions = 1;
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printparameters();
	actsutilityobj->printglobalvars();
	#endif 
	#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(_DEBUGMODE_CHECKS2)
	actsutilityobj->clearglobalvars();
	#endif
	#ifdef _DEBUGMODE_STATS
	unsigned int edges_count = 0;
	unsigned int edgesdstv_sum = 0;
	#endif
	
	keyvalue_t buffer1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer1
	keyvalue_t buffer2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer2
	keyvalue_t buffer3[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer3
	keyvalue_t buffer4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer4
	keyvalue_t buffer5[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer5
	keyvalue_t buffer6[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer6
	keyvalue_t buffer7[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer7
	
	skeyvalue_t capsule_so1[8][NUM_PARTITIONS];
	#pragma HLS array_partition variable = capsule_so1
	skeyvalue_t capsule_so2[4][NUM_PARTITIONS];
	#pragma HLS array_partition variable = capsule_so2
	skeyvalue_t capsule_so4[2][NUM_PARTITIONS];
	#pragma HLS array_partition variable = capsule_so4
	skeyvalue_t capsule_so8[NUM_PARTITIONS];
	
	keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ]; 
	batch_type skipsizes[NUM_PARTITIONS];
	keyvalue_t moretravstates[MAXLOADFACTORFORREDUCE];
	#ifdef _DEBUGMODE_CHECKS2
	keyvalue_t BIGKV[NUM_PARTITIONS];
	#endif
	
	batch_type sourcestatsmarker = 0;
	batch_type deststatsmarker = 1;
	batch_type destoffset = 0;
	config_t config;
	globalparams_t globalparams = getglobalparams(kvdram);
	sweepparams_t sweepparams;
	travstate_t actvvstravstate; actvvstravstate.i=0; actvvstravstate.i_kvs=0; actvvstravstate.v=0; actvvstravstate.k=0; 
	#ifdef _DEBUGMODE_CHECKS2
	if(globalparams.runsize >= MAXKVDATA_BATCHSIZE){ cout<<"dispatch:ERROR. runsize too large!. globalparams.runsize: "<<globalparams.runsize<<", MAXKVDATA_BATCHSIZE: "<<MAXKVDATA_BATCHSIZE<<". EXITING"<<endl; exit(EXIT_FAILURE); }
	#endif
	
	// start launch
	MAIN_LOOP1: for(step_type currentLOP=globalparams.beginLOP; currentLOP<(globalparams.beginLOP + globalparams.numLOPs); currentLOP++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_numllops avg=analysis_numllops	
	
		batch_type num_source_partitions = get_num_source_partitions(currentLOP);
		destoffset = 0;
		bool_type enreduce = ON;
		
		MAIN_LOOP1B: for(batch_type source_partition=0; source_partition<num_source_partitions; source_partition+=1){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_numsourcepartitions avg=analysis_numsourcepartitions

			#ifdef EMBEDDEDCOLLECTSTATS
			resetmanykeyandvalues(buffer4, GLOBALSTATSBUFFERSZ, 0);
			#else 
			resetmanykeyandvalues(buffer4, NUM_PARTITIONS, 0);
			#endif 
			resetmanykeyandvalues(globalstatsbuffer, NUM_PARTITIONS, 0);
		
			sweepparams = getsweepparams(globalparams, currentLOP, source_partition);
			travstate_t travstate = gettravstate(ON, kvdram, globalparams, currentLOP, sourcestatsmarker, source_partition, moretravstates);
			travstate_t ctravstate = travstate;
			travstate_t ptravstate = travstate;
			travstate_t rtravstate = travstate;
			travstate_t avtravstate;
			if((source_partition % LOADFACTORFORREDUCE) == 0){
				batch_type ntravszs = 0;
				for(batch_type k=0; k<LOADFACTORFORREDUCE; k++){ ntravszs += moretravstates[k].value; }
				if(ntravszs > 0){ enreduce = ON; } else { enreduce = OFF; }}
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->setstructs(config, sweepparams, travstate);
			actsutilityobj->clearallstats();
			#endif
			
			// process all edges
			#ifdef PROCESSALLEDGES
			if(inprocessedgesstage(currentLOP) == true){ avtravstate.begin_kvs = 0; }
			#ifdef PR_ALGORITHM
			if(inprocessedgesstage(currentLOP) == true){ avtravstate.end_kvs = avtravstate.begin_kvs + globalparams.srcvsize_kvs; avtravstate.size_kvs = globalparams.srcvsize_kvs; }
			#else 
			if(inprocessedgesstage(currentLOP) == true){ avtravstate.size_kvs = allignhigher_KV(globalparams.actvvsize) / VECTOR_SIZE; avtravstate.end_kvs = avtravstate.begin_kvs + avtravstate.size_kvs; }
			#endif 
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
			#ifdef PR_ALGORITHM
			processallvertices(
				config.enableprocessedges,
				kvdram,
				buffer1,
				buffer2,
				buffer3,
				buffer4,
				buffer5,
				buffer6,
				buffer7,
				capsule_so1,
				capsule_so2,
				capsule_so4,
				capsule_so8,
				globalstatsbuffer,
				config,
				globalparams,
				sweepparams,
				avtravstate);
			#else
			globalparams.runsize_kvs = 
				#ifdef COMPACTEDGES
				processactivevertices_compactedges
				#else 
				processactivevertices_noncompactedges	
				#endif 
				(
				config.enableprocessedges,
				kvdram,
				buffer1,
				buffer2,
				buffer3,
				avtravstate,
				globalparams
				);
			globalparams.runsize = globalparams.runsize_kvs * VECTOR_SIZE;
			#endif 
			#ifdef EMBEDDEDCOLLECTSTATS
			prepareglobalstats2(config.enableprocessedges, buffer4, globalstatsbuffer, NUM_PARTITIONS, globalparams);
			#endif
			#if defined(_DEBUGMODE_CHECKS2) & defined(ENABLE_PERFECTACCURACY) && defined(PR_ALGORITHM)
			if(config.enableprocessedges == ON){ actsutilityobj->checkforlessthanthan("dispatch::finished process_edges function.", actsutilityobj->globalstats_getcountnumvalidprocessedges(), globalparams.runsize, 100000); }
			#endif
			#endif
			
			// collect stats
			#ifdef COLLECTSTATS
			if(incollectstatsstage(currentLOP, globalparams) == true){ config.enableprocessedges = OFF; config.enablecollectglobalstats = ON; config.enablepartition = OFF; config.enablereduce = OFF; } 
			else { ctravstate.begin_kvs = 0; ctravstate.end_kvs = 0; config.enablecollectglobalstats = OFF; }
			if(incollectstatsstage(currentLOP, globalparams) == true && (ctravstate.end_kvs - ctravstate.begin_kvs) == 0){ ctravstate.begin_kvs = 0; ctravstate.end_kvs = 0; config.enablecollectglobalstats = OFF; } 
			#ifdef _DEBUGMODE_KERNELPRINTS2
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
					buffer3,
					buffer4,
					globalstatsbuffer,
					config,
					globalparams,
					sweepparams,
					ctravstate);
				calculateglobaloffsets(globalstatsbuffer, skipsizes, destoffset, NUM_PARTITIONS);
				if(incollectstatsstage(currentLOP, globalparams) == true){ saveglobalstats(config.enablecollectglobalstats, kvdram, globalstatsbuffer, globalparams.baseoffset_statsdram_kvs + deststatsmarker); }
			#endif
			#if defined(_DEBUGMODE_CHECKS2) && defined(COLLECTSTATSOFFLINE)
			if(config.enablecollectglobalstats == ON){ actsutilityobj->copykeyvalues(actsutilityobj->getmykeyvalues(7), globalstatsbuffer, NUM_PARTITIONS); }
			#endif
			resetvalues(globalstatsbuffer, NUM_PARTITIONS, 0);
			#endif
			
			// partition
			#ifdef PARTITIONUPDATES
			if(inpartitionstage(currentLOP, globalparams) == true && (ptravstate.size_kvs > 0)){ config.enableprocessedges = OFF; config.enablecollectglobalstats = OFF; config.enablepartition = ON; config.enablereduce = OFF; } 
			else { ptravstate.begin_kvs = 0; ptravstate.end_kvs = 0; config.enablepartition = OFF; }
			if(ptravstate.size_kvs == 0){ ptravstate.begin_kvs = 0; ptravstate.end_kvs = 0; config.enablepartition = OFF; } 
			#ifdef _DEBUGMODE_KERNELPRINTS2
			if((config.enablepartition == ON) && (currentLOP >= 1) && (currentLOP <= globalparams.treedepth)){ actsutilityobj->print7("### dispatch::partition:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, ptravstate.begin_kvs * VECTOR_SIZE, ptravstate.end_kvs * VECTOR_SIZE, ptravstate.size_kvs * VECTOR_SIZE, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams.currentLOP)), sweepparams.currentLOP); }	
			#endif
			resetvalues(globalstatsbuffer, NUM_PARTITIONS, 0);
			#ifdef ACTSFAST
			partitionupdates_finegrainedpipeline_unevencutoffs
			#else 
			partitionupdates_coarsegrainedpipeline
			#endif
				(
				config.enablepartition,
				kvdram,
				buffer3,
				buffer4,
				buffer5,
				buffer6,
				buffer7,
				capsule_so1,
				capsule_so2,
				capsule_so4,
				capsule_so8,
				globalstatsbuffer,
				config,
				globalparams,
				sweepparams,
				ptravstate,
				sweepparams.worksourcebaseaddress_kvs,
				sweepparams.workdestbaseaddress_kvs);
			if(inpartitionstage(currentLOP, globalparams) == true){ saveglobalstats(config.enablepartition, kvdram, globalstatsbuffer, globalparams.baseoffset_statsdram_kvs + deststatsmarker); }
			#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS) && defined(COLLECTSTATSOFFLINE)// && defined(PR_ALGORITHM) // FIXME. FIXFORBFS.
			if(config.enablepartition == ON){ actsutilityobj->postpartitioncheck(kvdram, globalstatsbuffer, ptravstate, sweepparams, globalparams); }
			#endif
			#endif
			
			// reduce 
			#ifdef REDUCEUPDATES
			batch_type totalsz = 0;
			if(inreducestage(currentLOP, globalparams) == true){ config.enableprocessedges = OFF; config.enablecollectglobalstats = OFF; config.enablepartition = OFF; config.enablereduce = ON; } 
			else { rtravstate.begin_kvs = 0; rtravstate.end_kvs = 0; config.enablereduce = OFF; }
			if(enreduce == OFF){ rtravstate.begin_kvs = 0; rtravstate.end_kvs = 0; config.enablereduce = OFF; }
			#ifdef _DEBUGMODE_KERNELPRINTS2
			if(config.enablereduce == ON){ actsutilityobj->print7("### dispatch::reduce:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, rtravstate.begin_kvs * VECTOR_SIZE, rtravstate.end_kvs * VECTOR_SIZE, (rtravstate.end_kvs - rtravstate.begin_kvs) * VECTOR_SIZE, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams.currentLOP)), sweepparams.currentLOP); }							
			#endif
			actvvstravstate = reduceupdates(
				config.enablereduce,
				kvdram,
				buffer3,
				buffer4,
				buffer5,
				buffer6,
				config,
				globalparams,
				sweepparams,
				rtravstate,
				actvvstravstate);
			#endif 
			
			if(currentLOP > 0){
				sourcestatsmarker += 1;
				deststatsmarker += NUM_PARTITIONS;
				destoffset = globalstatsbuffer[NUM_PARTITIONS-1].key + globalstatsbuffer[NUM_PARTITIONS-1].value + 64; }
			
			#ifdef _DEBUGMODE_CHECKS
			if(config.enablereduce == ON){ actsutilityobj->printpartitionresult2(ON, kvdram, globalstatsbuffer, sweepparams); }
			#endif
			#ifdef _DEBUGMODE_STATS
			if(config.enablereduce == ON){ 
				edges_count = actsutilityobj->globalstats_getcountvalidkvsreduced(); 
				edgesdstv_sum = actsutilityobj->globalstats_getreducevar1(); }
			#endif
			#ifdef _DEBUGMODE_KERNELPRINTS
			actsutilityobj->printglobalvars();
			actsutilityobj->clearglobalvars();
			#endif
		}
		#ifdef _DEBUGMODE_KERNELPRINTS2
		actsutilityobj->printglobalvars();
		#endif 
		#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(_DEBUGMODE_CHECKS2)
		actsutilityobj->clearglobalvars();
		#endif
	}
	
	#if defined(INMEMORYGP) && defined(BFS_ALGORITHM)
	savekeyvalues(ON, kvdram, buffer6, globalparams.baseoffset_activevertices_kvs + actvvstravstate.i_kvs, actvvstravstate.k + 1);
	actvvstravstate.i += (actvvstravstate.k * (VECTOR_SIZE * 2)) + actvvstravstate.v;
	actvvstravstate.i_kvs += actvvstravstate.k;
	#endif
	
	#ifdef _DEBUGMODE_CHECKS
	actsutilityobj->countvalueslessthan("dispatch", (value_t *)&kvdram[BASEOFFSET_VERTICESDATA_KVS], BATCH_RANGE, INFINITI);
	#endif
	#ifdef _DEBUGMODE_STATS
	// kvdram[PADDEDKVSOURCEDRAMSZ_KVS-1].data[3].key = edges_count;
	// kvdram[PADDEDKVSOURCEDRAMSZ_KVS-1].data[4].key = edgesdstv_sum;
	// kvdram[PADDEDKVSOURCEDRAMSZ_KVS-1].data[5].key = actvvstravstate.i;
	
	setkey(kvdram, PADDEDKVSOURCEDRAMSZ_KVS-1, 3, edges_count);
	setkey(kvdram, PADDEDKVSOURCEDRAMSZ_KVS-1, 4, edgesdstv_sum);
	setkey(kvdram, PADDEDKVSOURCEDRAMSZ_KVS-1, 5, actvvstravstate.i);
	
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"========== dispatch:: actvvstravstate.i: "<<actvvstravstate.i<<endl;
	cout<<"========== dispatch:: actvvstravstate.i_kvs: "<<actvvstravstate.i_kvs<<endl;
	cout<<"========== dispatch:: actvvstravstate.v: "<<actvvstravstate.v<<endl;
	cout<<"========== dispatch:: actvvstravstate.k: "<<actvvstravstate.k<<endl;
	#endif 
	return;
}

void
	#ifdef SW 
	acts::
	#endif 
dispatch_partitiononly(uint512_dt * kvdram, 
			keyvalue_t buffer1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
			keyvalue_t buffer4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
			keyvalue_t buffer5[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
			globalparams_t globalparams){
	analysis_type analysis_numllops = 1;
	analysis_type analysis_numsourcepartitions = 1;
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printparameters();
	actsutilityobj->printglobalvars();
	#endif 
	#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(_DEBUGMODE_CHECKS2)
	actsutilityobj->clearglobalvars();
	#endif
	#ifdef _DEBUGMODE_STATS
	unsigned int edges_count = 0;
	unsigned int edgesdstv_sum = 0;
	#endif
	
	/** keyvalue_t buffer1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer1 */
	keyvalue_t buffer2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer2
	keyvalue_t buffer3[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer3
	/** keyvalue_t buffer4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer4
	keyvalue_t buffer5[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer5 */
	
	/** keyvalue_t buffer6[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer6
	keyvalue_t buffer7[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer7 */
	
	skeyvalue_t capsule_so1[8][NUM_PARTITIONS];
	#pragma HLS array_partition variable = capsule_so1
	skeyvalue_t capsule_so2[4][NUM_PARTITIONS];
	#pragma HLS array_partition variable = capsule_so2
	skeyvalue_t capsule_so4[2][NUM_PARTITIONS];
	#pragma HLS array_partition variable = capsule_so4
	skeyvalue_t capsule_so8[NUM_PARTITIONS];
	
	keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ]; 
	batch_type skipsizes[NUM_PARTITIONS];
	keyvalue_t moretravstates[MAXLOADFACTORFORREDUCE];
	#ifdef _DEBUGMODE_CHECKS2
	keyvalue_t BIGKV[NUM_PARTITIONS];
	#endif
	
	for(vector_type i=0; i<8; i++){ resetmanykeyandvalues(capsule_so1[i], NUM_PARTITIONS, 0); }
	for(vector_type i=0; i<4; i++){ resetmanykeyandvalues(capsule_so2[i], NUM_PARTITIONS, 0); }
	for(vector_type i=0; i<2; i++){ resetmanykeyandvalues(capsule_so4[i], NUM_PARTITIONS, 0); }
	for(vector_type i=0; i<1; i++){ resetmanykeyandvalues(capsule_so8, NUM_PARTITIONS, 0); }
	
	batch_type sourcestatsmarker = 0;
	batch_type deststatsmarker = 1;
	batch_type destoffset = 0;
	config_t config;
	sweepparams_t sweepparams;
	travstate_t actvvstravstate; actvvstravstate.i=0; actvvstravstate.i_kvs=0; actvvstravstate.v=0; actvvstravstate.k=0; 
	#ifdef _DEBUGMODE_CHECKS2
	if(globalparams.runsize >= MAXKVDATA_BATCHSIZE){ cout<<"dispatch:ERROR. runsize too large!. globalparams.runsize: "<<globalparams.runsize<<", MAXKVDATA_BATCHSIZE: "<<MAXKVDATA_BATCHSIZE<<". EXITING"<<endl; exit(EXIT_FAILURE); }
	#endif
	
	// start launch
	MAIN_LOOP1: for(step_type currentLOP=globalparams.beginLOP; currentLOP<(globalparams.beginLOP + globalparams.numLOPs - 1); currentLOP++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_numllops avg=analysis_numllops	
	
		batch_type num_source_partitions = get_num_source_partitions(currentLOP);
		destoffset = 0;
		bool_type enreduce = ON;
		
		MAIN_LOOP1B: for(batch_type source_partition=0; source_partition<num_source_partitions; source_partition+=1){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_numsourcepartitions avg=analysis_numsourcepartitions

			#ifdef EMBEDDEDCOLLECTSTATS
			resetmanykeyandvalues(buffer4, GLOBALSTATSBUFFERSZ, 0);
			#else 
			resetmanykeyandvalues(buffer4, NUM_PARTITIONS, 0);
			#endif 
			resetmanykeyandvalues(globalstatsbuffer, NUM_PARTITIONS, 0);
		
			sweepparams = getsweepparams(globalparams, currentLOP, source_partition);
			travstate_t travstate = gettravstate(ON, kvdram, globalparams, currentLOP, sourcestatsmarker, source_partition, moretravstates);
			travstate_t ctravstate = travstate;
			travstate_t ptravstate = travstate;
			travstate_t avtravstate;
			
			// process all edges
			#ifdef PROCESSALLEDGES
			if(inprocessedgesstage(currentLOP) == true){ avtravstate.begin_kvs = 0; }
			#ifdef PR_ALGORITHM
			if(inprocessedgesstage(currentLOP) == true){ avtravstate.end_kvs = avtravstate.begin_kvs + globalparams.srcvsize_kvs; avtravstate.size_kvs = globalparams.srcvsize_kvs; }
			#else 
			if(inprocessedgesstage(currentLOP) == true){ avtravstate.size_kvs = allignhigherto16_KV(globalparams.actvvsize) / (2 * VECTOR_SIZE); avtravstate.end_kvs = avtravstate.begin_kvs + avtravstate.size_kvs; }
			#endif 
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
			#ifdef PR_ALGORITHM
			processallvertices(
				config.enableprocessedges,
				kvdram,
				buffer1,
				buffer2,
				buffer3,
				buffer4,
				buffer5,
				buffer6,
				buffer7,
				capsule_so1,
				capsule_so2,
				capsule_so4,
				capsule_so8,
				globalstatsbuffer,
				config,
				globalparams,
				sweepparams,
				avtravstate);
			#else
			globalparams.runsize_kvs = 
				#ifdef COMPACTEDGES
				processactivevertices_compactedges
				#else 
				processactivevertices_noncompactedges	
				#endif 
				(
				config.enableprocessedges,
				kvdram,
				buffer1,
				buffer2,
				buffer3,
				avtravstate,
				globalparams
				);
			globalparams.runsize = globalparams.runsize_kvs * VECTOR_SIZE;
			#endif 
			#ifdef EMBEDDEDCOLLECTSTATS
			prepareglobalstats2(config.enableprocessedges, buffer4, globalstatsbuffer, NUM_PARTITIONS, globalparams);
			#endif
			#if defined(_DEBUGMODE_CHECKS2) & defined(ENABLE_PERFECTACCURACY) && defined(PR_ALGORITHM)
			if(config.enableprocessedges == ON){ actsutilityobj->checkforlessthanthan("dispatch::finished process_edges function.", actsutilityobj->globalstats_getcountnumvalidprocessedges(), globalparams.runsize, 100000); }
			#endif
			#endif
			
			// collect stats
			#ifdef COLLECTSTATS
			if(incollectstatsstage(currentLOP, globalparams) == true){ config.enableprocessedges = OFF; config.enablecollectglobalstats = ON; config.enablepartition = OFF; config.enablereduce = OFF; } 
			else { ctravstate.begin_kvs = 0; ctravstate.end_kvs = 0; config.enablecollectglobalstats = OFF; }
			if(incollectstatsstage(currentLOP, globalparams) == true && (ctravstate.end_kvs - ctravstate.begin_kvs) == 0){ ctravstate.begin_kvs = 0; ctravstate.end_kvs = 0; config.enablecollectglobalstats = OFF; } 
			#ifdef _DEBUGMODE_KERNELPRINTS2
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
					buffer3,
					buffer4,
					globalstatsbuffer,
					config,
					globalparams,
					sweepparams,
					ctravstate);
				calculateglobaloffsets(globalstatsbuffer, skipsizes, destoffset, NUM_PARTITIONS);
				if(incollectstatsstage(currentLOP, globalparams) == true){ saveglobalstats(config.enablecollectglobalstats, kvdram, globalstatsbuffer, globalparams.baseoffset_statsdram_kvs + deststatsmarker); }
			#endif
			#if defined(_DEBUGMODE_CHECKS2) && defined(COLLECTSTATSOFFLINE)
			if(config.enablecollectglobalstats == ON){ actsutilityobj->copykeyvalues(actsutilityobj->getmykeyvalues(7), globalstatsbuffer, NUM_PARTITIONS); }
			#endif
			resetvalues(globalstatsbuffer, NUM_PARTITIONS, 0);
			#endif
			
			// partition
			#ifdef PARTITIONUPDATES
			if(inpartitionstage(currentLOP, globalparams) == true && (ptravstate.size_kvs > 0)){ config.enableprocessedges = OFF; config.enablecollectglobalstats = OFF; config.enablepartition = ON; config.enablereduce = OFF; } 
			else { ptravstate.begin_kvs = 0; ptravstate.end_kvs = 0; config.enablepartition = OFF; }
			if(ptravstate.size_kvs == 0){ ptravstate.begin_kvs = 0; ptravstate.end_kvs = 0; config.enablepartition = OFF; } 
			#ifdef _DEBUGMODE_KERNELPRINTS2
			if((config.enablepartition == ON) && (currentLOP >= 1) && (currentLOP <= globalparams.treedepth)){ actsutilityobj->print7("### dispatch::partition:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, ptravstate.begin_kvs * VECTOR_SIZE, ptravstate.end_kvs * VECTOR_SIZE, ptravstate.size_kvs * VECTOR_SIZE, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams.currentLOP)), sweepparams.currentLOP); }	
			#endif
			resetvalues(globalstatsbuffer, NUM_PARTITIONS, 0);
			#ifdef ACTSFAST
			partitionupdates_finegrainedpipeline_unevencutoffs
			#else 
			partitionupdates_coarsegrainedpipeline
			#endif
				(
				config.enablepartition,
				kvdram,
				buffer1,
				buffer2,
				buffer3,
				buffer4,
				buffer5,
				capsule_so1,
				capsule_so2,
				capsule_so4,
				capsule_so8,
				globalstatsbuffer,
				config,
				globalparams,
				sweepparams,
				ptravstate,
				sweepparams.worksourcebaseaddress_kvs,
				sweepparams.workdestbaseaddress_kvs);
			if(inpartitionstage(currentLOP, globalparams) == true){ saveglobalstats(config.enablepartition, kvdram, globalstatsbuffer, globalparams.baseoffset_statsdram_kvs + deststatsmarker); }
			#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS) && defined(COLLECTSTATSOFFLINE)// && defined(PR_ALGORITHM) // FIXME. FIXFORBFS.
			if(config.enablepartition == ON){ actsutilityobj->postpartitioncheck(kvdram, globalstatsbuffer, ptravstate, sweepparams, globalparams); }
			#endif
			#endif
			
			if(currentLOP > 0){
				sourcestatsmarker += 1;
				deststatsmarker += NUM_PARTITIONS;
				destoffset = globalstatsbuffer[NUM_PARTITIONS-1].key + globalstatsbuffer[NUM_PARTITIONS-1].value + 64; }
			
			#ifdef _DEBUGMODE_CHECKS
			if(config.enablereduce == ON){ actsutilityobj->printpartitionresult2(ON, kvdram, globalstatsbuffer, sweepparams); }
			#endif
			#ifdef _DEBUGMODE_STATS
			if(config.enablereduce == ON){ 
				edges_count = actsutilityobj->globalstats_getcountvalidkvsreduced(); 
				edgesdstv_sum = actsutilityobj->globalstats_getreducevar1(); }
			#endif
			#ifdef _DEBUGMODE_KERNELPRINTS
			actsutilityobj->printglobalvars();
			actsutilityobj->clearglobalvars();
			#endif
		}
		#ifdef _DEBUGMODE_KERNELPRINTS2
		actsutilityobj->printglobalvars();
		#endif 
		#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(_DEBUGMODE_CHECKS2)
		actsutilityobj->clearglobalvars();
		#endif
	}
	return;
}

void
	#ifdef SW 
	acts::
	#endif 
dispatch_reduceonly(uint512_dt * kvdram, globalparams_t globalparams){
	// NOTE: CRITICAL AUTOMATEME: LOADFACTORFORREDUCE
	analysis_type analysis_numllops = 1;
	analysis_type analysis_numsourcepartitions = 1;
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printparameters();
	actsutilityobj->printglobalvars();
	#endif 
	#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(_DEBUGMODE_CHECKS2)
	actsutilityobj->clearglobalvars();
	#endif
	#ifdef _DEBUGMODE_STATS
	unsigned int edges_count = 0;
	unsigned int edgesdstv_sum = 0;
	#endif
	
	keyvalue_t buffer1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer1
	keyvalue_t buffer2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer2
	keyvalue_t buffer3[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer3
	keyvalue_t buffer4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer4
	keyvalue_t moretravstates[MAXLOADFACTORFORREDUCE];
	
	config_t config;
	sweepparams_t sweepparams;
	travstate_t actvvstravstate; actvvstravstate.i=0; actvvstravstate.i_kvs=0; actvvstravstate.v=0; actvvstravstate.k=0; 
	#ifdef _DEBUGMODE_CHECKS2
	if(globalparams.runsize >= MAXKVDATA_BATCHSIZE){ cout<<"dispatch:ERROR. runsize too large!. globalparams.runsize: "<<globalparams.runsize<<", MAXKVDATA_BATCHSIZE: "<<MAXKVDATA_BATCHSIZE<<". EXITING"<<endl; exit(EXIT_FAILURE); }
	#endif
	
	step_type currentLOP = globalparams.beginLOP + globalparams.numLOPs - 1;
	batch_type num_source_partitions = get_num_source_partitions(currentLOP);
	unsigned int sourcestatsmarker = 0;
	bool_type enreduce = ON;
	for(unsigned int k=0; k<globalparams.treedepth; k++){ sourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); }
			
	MAIN_LOOP: for(batch_type source_partition=0; source_partition<num_source_partitions; source_partition+=1){
		#ifdef EMBEDDEDCOLLECTSTATS
		resetmanykeyandvalues(buffer2, GLOBALSTATSBUFFERSZ, 0);
		#else 
		resetmanykeyandvalues(buffer2, NUM_PARTITIONS, 0);
		#endif 
		
		sweepparams = getsweepparams(globalparams, currentLOP, source_partition);
		travstate_t travstate = gettravstate(ON, kvdram, globalparams, currentLOP, sourcestatsmarker, source_partition, moretravstates);
		travstate_t rtravstate = travstate;
		if((source_partition % LOADFACTORFORREDUCE) == 0){
			batch_type ntravszs = 0;
			for(batch_type k=0; k<LOADFACTORFORREDUCE; k++){ ntravszs += moretravstates[k].value; }
			if(ntravszs > 0){ enreduce = ON; } else { enreduce = OFF; }}
	
		config.enablereduce = ON;
		if(enreduce == OFF){ rtravstate.begin_kvs = 0; rtravstate.end_kvs = 0; config.enablereduce = OFF; }
		#ifdef _DEBUGMODE_KERNELPRINTS2
		if(config.enablereduce == ON){ actsutilityobj->print7("### dispatch::reduce:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, rtravstate.begin_kvs * VECTOR_SIZE, rtravstate.end_kvs * VECTOR_SIZE, (rtravstate.end_kvs - rtravstate.begin_kvs) * VECTOR_SIZE, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams.currentLOP)), sweepparams.currentLOP); }							
		#endif
		actvvstravstate = reduceupdates(
			config.enablereduce,
			kvdram,
			buffer1,
			buffer2,
			buffer3,
			buffer4,
			config,
			globalparams,
			sweepparams,
			rtravstate,
			actvvstravstate);
		sourcestatsmarker += 1;
		
		#ifdef _DEBUGMODE_KERNELPRINTS
		actsutilityobj->printglobalvars();
		actsutilityobj->clearglobalvars();
		#endif
	}
	#if defined(INMEMORYGP) && defined(BFS_ALGORITHM)
	savekeyvalues(ON, kvdram, buffer4, globalparams.baseoffset_activevertices_kvs + actvvstravstate.i_kvs, actvvstravstate.k + 1);
	actvvstravstate.i += (actvvstravstate.k * (VECTOR_SIZE * 2)) + actvvstravstate.v;
	actvvstravstate.i_kvs += actvvstravstate.k;
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS2 // 
	actsutilityobj->printglobalvars();
	#endif
	#ifdef _DEBUGMODE_CHECKS
	actsutilityobj->countvalueslessthan("dispatch", (value_t *)&kvdram[BASEOFFSET_VERTICESDATA_KVS], BATCH_RANGE, INFINITI);
	#endif
	#ifdef _DEBUGMODE_STATS
	setkey(kvdram, PADDEDKVSOURCEDRAMSZ_KVS-1, 3, actsutilityobj->globalstats_getcountvalidkvsreduced());
	setkey(kvdram, PADDEDKVSOURCEDRAMSZ_KVS-1, 4, actsutilityobj->globalstats_getreducevar1());
	setkey(kvdram, PADDEDKVSOURCEDRAMSZ_KVS-1, 5, actvvstravstate.i);
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"dispatch_reduceonly:: actvvstravstate.i: "<<actvvstravstate.i<<endl;
	cout<<"dispatch_reduceonly:: actvvstravstate.i_kvs: "<<actvvstravstate.i_kvs<<endl;
	cout<<"dispatch_reduceonly:: actvvstravstate.v: "<<actvvstravstate.v<<endl;
	cout<<"dispatch_reduceonly:: actvvstravstate.k: "<<actvvstravstate.k<<endl;
	#endif 
	return;
}

void 
	#ifdef SW 
	acts::
	#endif 
dispatch_reduceonly_serialsync(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11, travstate_t actvvstravstate[NUMSUBCPUTHREADS], globalparams_t globalparams[NUMSUBCPUTHREADS]){
	// NOTE: CRITICAL AUTOMATEME: LOADFACTORFORREDUCE
	analysis_type analysis_numllops = 1;
	analysis_type analysis_numsourcepartitions = 1;
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printparameters();
	actsutilityobj->printglobalvars();
	#endif 
	#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(_DEBUGMODE_CHECKS2)
	actsutilityobj->clearglobalvars();
	#endif
	#ifdef _DEBUGMODE_STATS
	unsigned int edges_count = 0;
	unsigned int edgesdstv_sum = 0;
	#endif
	
	keyvalue_t verticesbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = verticesbuffer
	keyvalue_t actvvs[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = actvvs
	keyvalue_t keyvaluesbuffer[NUMSUBCPUTHREADS][VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = keyvaluesbuffer
	keyvalue_t tempverticesbuffer0[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = tempverticesbuffer0
	keyvalue_t tempverticesbuffer1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = tempverticesbuffer1
	keyvalue_t tempverticesbuffer2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = tempverticesbuffer2
	keyvalue_t tempverticesbuffer3[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = tempverticesbuffer3
	keyvalue_t tempverticesbuffer4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = tempverticesbuffer4
	keyvalue_t tempverticesbuffer5[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = tempverticesbuffer5
	keyvalue_t tempverticesbuffer6[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = tempverticesbuffer6
	keyvalue_t tempverticesbuffer7[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = tempverticesbuffer7
	keyvalue_t tempverticesbuffer8[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = tempverticesbuffer8
	keyvalue_t tempverticesbuffer9[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = tempverticesbuffer9
	keyvalue_t tempverticesbuffer10[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = tempverticesbuffer10
	keyvalue_t tempverticesbuffer11[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = tempverticesbuffer11
	keyvalue_t moretravstates[NUMSUBCPUTHREADS][MAXLOADFACTORFORREDUCE]; //
	
	config_t config[NUMSUBCPUTHREADS];
	sweepparams_t sweepparams;
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ actvvstravstate[i].i=0; actvvstravstate[i].i_kvs=0; actvvstravstate[i].v=0; actvvstravstate[i].k=0; }
	#ifdef _DEBUGMODE_CHECKS2
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ if(globalparams[i].runsize >= MAXKVDATA_BATCHSIZE){ cout<<"dispatch:ERROR. runsize too large!. globalparams[i].runsize: "<<globalparams[i].runsize<<", MAXKVDATA_BATCHSIZE: "<<MAXKVDATA_BATCHSIZE<<". EXITING"<<endl; exit(EXIT_FAILURE); }}
	#endif
	travstate_t rtravstate[NUMSUBCPUTHREADS];// FIXME? DEPRECIATED?
	
	step_type currentLOP = globalparams[0].beginLOP + globalparams[0].numLOPs - 1;
	batch_type num_source_partitions = get_num_source_partitions(currentLOP);
	unsigned int sourcestatsmarker = 0;
	bool_type enreduce[NUMSUBCPUTHREADS];
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ enreduce[i] = ON; }
	for(unsigned int k=0; k<globalparams[0].treedepth; k++){ sourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); }
			
	MAIN_LOOP: for(batch_type source_partition=0; source_partition<num_source_partitions; source_partition+=1){
		sweepparams = getsweepparams(globalparams[0], currentLOP, source_partition);
		rtravstate[0] = gettravstate(ON, kvdram0, globalparams[0], currentLOP, sourcestatsmarker, source_partition, moretravstates[0]);
		rtravstate[1] = gettravstate(ON, kvdram1, globalparams[1], currentLOP, sourcestatsmarker, source_partition, moretravstates[1]);
		rtravstate[2] = gettravstate(ON, kvdram2, globalparams[2], currentLOP, sourcestatsmarker, source_partition, moretravstates[2]);
		rtravstate[3] = gettravstate(ON, kvdram3, globalparams[3], currentLOP, sourcestatsmarker, source_partition, moretravstates[3]);
		rtravstate[4] = gettravstate(ON, kvdram4, globalparams[4], currentLOP, sourcestatsmarker, source_partition, moretravstates[4]);
		rtravstate[5] = gettravstate(ON, kvdram5, globalparams[5], currentLOP, sourcestatsmarker, source_partition, moretravstates[5]);
		rtravstate[6] = gettravstate(ON, kvdram6, globalparams[6], currentLOP, sourcestatsmarker, source_partition, moretravstates[6]);
		rtravstate[7] = gettravstate(ON, kvdram7, globalparams[7], currentLOP, sourcestatsmarker, source_partition, moretravstates[7]);
		rtravstate[8] = gettravstate(ON, kvdram8, globalparams[8], currentLOP, sourcestatsmarker, source_partition, moretravstates[8]);
		rtravstate[9] = gettravstate(ON, kvdram9, globalparams[9], currentLOP, sourcestatsmarker, source_partition, moretravstates[9]);
		rtravstate[10] = gettravstate(ON, kvdram10, globalparams[10], currentLOP, sourcestatsmarker, source_partition, moretravstates[10]);
		rtravstate[11] = gettravstate(ON, kvdram11, globalparams[11], currentLOP, sourcestatsmarker, source_partition, moretravstates[11]);
		for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){
			if((source_partition % LOADFACTORFORREDUCE) == 0){
				batch_type ntravszs = 0;
				for(batch_type k=0; k<LOADFACTORFORREDUCE; k++){ ntravszs += moretravstates[i][k].value; }
				if(ntravszs > 0){ enreduce[i] = ON; } else { enreduce[i] = OFF; }}
			config[i].enablereduce = ON;
			if(enreduce[i] == OFF){ rtravstate[i].begin_kvs = 0; rtravstate[i].end_kvs = 0; config[i].enablereduce = OFF; }
			#ifdef _DEBUGMODE_KERNELPRINTS3
			if(config[i].enablereduce == ON){ actsutilityobj->print7("### dispatch::reduce:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, rtravstate[i].begin_kvs * VECTOR_SIZE, rtravstate[i].end_kvs * VECTOR_SIZE, (rtravstate[i].end_kvs - rtravstate[i].begin_kvs) * VECTOR_SIZE, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams.currentLOP)), sweepparams.currentLOP); }							
			#endif
		}
		
		config_t _config = config[0];
		globalparams_t _globalparams = globalparams[0];
		travstate_t _actvvstravstate = actvvstravstate[0];
		if(_config.enablereduce == ON){ 
			if((sweepparams.source_partition % LOADFACTORFORREDUCE) == 0){ readkeyvalues(_config.enablereduce, kvdram0, verticesbuffer, (_globalparams.baseoffset_verticesdata_kvs + (sweepparams.source_partition * ((_globalparams.applyvertexbuffersz_kvs / VDATAPACKINGFACTOR) / 2))), PADDEDDESTBUFFER_SIZE); }
			replicatedata_syn(_config.enablereduce, verticesbuffer, tempverticesbuffer0,tempverticesbuffer1,tempverticesbuffer2,tempverticesbuffer3,tempverticesbuffer4,tempverticesbuffer5,tempverticesbuffer6,tempverticesbuffer7,tempverticesbuffer8,tempverticesbuffer9,tempverticesbuffer10,tempverticesbuffer11, (sweepparams.source_partition % LOADFACTORFORREDUCE) * ((_globalparams.applyvertexbuffersz / VDATAPACKINGFACTOR) / 2), ((_globalparams.applyvertexbuffersz / VDATAPACKINGFACTOR) / 2));
			
			batch_type maxsz_kvs = 0;
			for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ if(rtravstate[i].size_kvs > maxsz_kvs){ maxsz_kvs = rtravstate[i].size_kvs; }}
			unsigned int en[NUMSUBCPUTHREADS];
			
			MAIN_LOOP1E_REDUCE: for(batch_type offset_kvs=0; offset_kvs<maxsz_kvs; offset_kvs+=rtravstate[0].skip_kvs){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_reduceloop avg=analysis_reduceloop
				#ifdef _DEBUGMODE_KERNELPRINTS
				for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ actsutilityobj->print4("### dispatch::reduce:: offset_kvs", "begin_kvs", "end_kvs", "skip", rtravstate[i].begin_kvs + offset_kvs, rtravstate[i].begin_kvs, rtravstate[i].end_kvs, SRCBUFFER_SIZE); }
				#endif
				
				for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ 
				#pragma HLS UNROLL
					if(offset_kvs < rtravstate[i].size_kvs){ en[i] = ON; } else { en[i] = OFF; }
				}
				
				for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ 
				#pragma HLS UNROLL
					if(en[i] == ON){
						rtravstate[i].i_kvs = rtravstate[i].begin_kvs + offset_kvs; 
					}
				}

				readkeyvalues(en[0], kvdram0, keyvaluesbuffer[0], (sweepparams.worksourcebaseaddress_kvs + rtravstate[0].begin_kvs + offset_kvs), SRCBUFFER_SIZE, rtravstate[0]);
				readkeyvalues(en[1], kvdram1, keyvaluesbuffer[1], (sweepparams.worksourcebaseaddress_kvs + rtravstate[1].begin_kvs + offset_kvs), SRCBUFFER_SIZE, rtravstate[1]);
				readkeyvalues(en[2], kvdram2, keyvaluesbuffer[2], (sweepparams.worksourcebaseaddress_kvs + rtravstate[2].begin_kvs + offset_kvs), SRCBUFFER_SIZE, rtravstate[2]);
				readkeyvalues(en[3], kvdram3, keyvaluesbuffer[3], (sweepparams.worksourcebaseaddress_kvs + rtravstate[3].begin_kvs + offset_kvs), SRCBUFFER_SIZE, rtravstate[3]);
				readkeyvalues(en[4], kvdram4, keyvaluesbuffer[4], (sweepparams.worksourcebaseaddress_kvs + rtravstate[4].begin_kvs + offset_kvs), SRCBUFFER_SIZE, rtravstate[4]);
				readkeyvalues(en[5], kvdram5, keyvaluesbuffer[5], (sweepparams.worksourcebaseaddress_kvs + rtravstate[5].begin_kvs + offset_kvs), SRCBUFFER_SIZE, rtravstate[5]);
				readkeyvalues(en[6], kvdram6, keyvaluesbuffer[6], (sweepparams.worksourcebaseaddress_kvs + rtravstate[6].begin_kvs + offset_kvs), SRCBUFFER_SIZE, rtravstate[6]);
				readkeyvalues(en[7], kvdram7, keyvaluesbuffer[7], (sweepparams.worksourcebaseaddress_kvs + rtravstate[7].begin_kvs + offset_kvs), SRCBUFFER_SIZE, rtravstate[7]);
				readkeyvalues(en[8], kvdram8, keyvaluesbuffer[8], (sweepparams.worksourcebaseaddress_kvs + rtravstate[8].begin_kvs + offset_kvs), SRCBUFFER_SIZE, rtravstate[8]);
				readkeyvalues(en[9], kvdram9, keyvaluesbuffer[9], (sweepparams.worksourcebaseaddress_kvs + rtravstate[9].begin_kvs + offset_kvs), SRCBUFFER_SIZE, rtravstate[9]);
				readkeyvalues(en[10], kvdram10, keyvaluesbuffer[10], (sweepparams.worksourcebaseaddress_kvs + rtravstate[10].begin_kvs + offset_kvs), SRCBUFFER_SIZE, rtravstate[10]);
				readkeyvalues(en[11], kvdram11, keyvaluesbuffer[11], (sweepparams.worksourcebaseaddress_kvs + rtravstate[11].begin_kvs + offset_kvs), SRCBUFFER_SIZE, rtravstate[11]);
				
				#if defined(INMEMORYGP) && defined(BFS_ALGORITHM)
				reduce_bfs(ON, en[0], keyvaluesbuffer[0], tempverticesbuffer0, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[0], _globalparams);
				#else 
				reduce(ON, en[0], keyvaluesbuffer[0], tempverticesbuffer0, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[0], _globalparams);
				#endif
				#if defined(INMEMORYGP) && defined(BFS_ALGORITHM)
				reduce_bfs(ON, en[1], keyvaluesbuffer[1], tempverticesbuffer1, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[1], _globalparams);
				#else 
				reduce(ON, en[1], keyvaluesbuffer[1], tempverticesbuffer1, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[1], _globalparams);
				#endif
				#if defined(INMEMORYGP) && defined(BFS_ALGORITHM)
				reduce_bfs(ON, en[2], keyvaluesbuffer[2], tempverticesbuffer2, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[2], _globalparams);
				#else 
				reduce(ON, en[2], keyvaluesbuffer[2], tempverticesbuffer2, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[2], _globalparams);
				#endif
				#if defined(INMEMORYGP) && defined(BFS_ALGORITHM)
				reduce_bfs(ON, en[3], keyvaluesbuffer[3], tempverticesbuffer3, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[3], _globalparams);
				#else 
				reduce(ON, en[3], keyvaluesbuffer[3], tempverticesbuffer3, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[3], _globalparams);
				#endif
				#if defined(INMEMORYGP) && defined(BFS_ALGORITHM)
				reduce_bfs(ON, en[4], keyvaluesbuffer[4], tempverticesbuffer4, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[4], _globalparams);
				#else 
				reduce(ON, en[4], keyvaluesbuffer[4], tempverticesbuffer4, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[4], _globalparams);
				#endif
				#if defined(INMEMORYGP) && defined(BFS_ALGORITHM)
				reduce_bfs(ON, en[5], keyvaluesbuffer[5], tempverticesbuffer5, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[5], _globalparams);
				#else 
				reduce(ON, en[5], keyvaluesbuffer[5], tempverticesbuffer5, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[5], _globalparams);
				#endif
				#if defined(INMEMORYGP) && defined(BFS_ALGORITHM)
				reduce_bfs(ON, en[6], keyvaluesbuffer[6], tempverticesbuffer6, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[6], _globalparams);
				#else 
				reduce(ON, en[6], keyvaluesbuffer[6], tempverticesbuffer6, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[6], _globalparams);
				#endif
				#if defined(INMEMORYGP) && defined(BFS_ALGORITHM)
				reduce_bfs(ON, en[7], keyvaluesbuffer[7], tempverticesbuffer7, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[7], _globalparams);
				#else 
				reduce(ON, en[7], keyvaluesbuffer[7], tempverticesbuffer7, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[7], _globalparams);
				#endif
				#if defined(INMEMORYGP) && defined(BFS_ALGORITHM)
				reduce_bfs(ON, en[8], keyvaluesbuffer[8], tempverticesbuffer8, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[8], _globalparams);
				#else 
				reduce(ON, en[8], keyvaluesbuffer[8], tempverticesbuffer8, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[8], _globalparams);
				#endif
				#if defined(INMEMORYGP) && defined(BFS_ALGORITHM)
				reduce_bfs(ON, en[9], keyvaluesbuffer[9], tempverticesbuffer9, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[9], _globalparams);
				#else 
				reduce(ON, en[9], keyvaluesbuffer[9], tempverticesbuffer9, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[9], _globalparams);
				#endif
				#if defined(INMEMORYGP) && defined(BFS_ALGORITHM)
				reduce_bfs(ON, en[10], keyvaluesbuffer[10], tempverticesbuffer10, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[10], _globalparams);
				#else 
				reduce(ON, en[10], keyvaluesbuffer[10], tempverticesbuffer10, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[10], _globalparams);
				#endif
				#if defined(INMEMORYGP) && defined(BFS_ALGORITHM)
				reduce_bfs(ON, en[11], keyvaluesbuffer[11], tempverticesbuffer11, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[11], _globalparams);
				#else 
				reduce(ON, en[11], keyvaluesbuffer[11], tempverticesbuffer11, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[11], _globalparams);
				#endif
			}
			
			#if defined(INMEMORYGP) && defined(BFS_ALGORITHM)
			_actvvstravstate = unifydata_bfs_syn(_config.enablereduce, kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9,kvdram10,kvdram11, tempverticesbuffer0,tempverticesbuffer1,tempverticesbuffer2,tempverticesbuffer3,tempverticesbuffer4,tempverticesbuffer5,tempverticesbuffer6,tempverticesbuffer7,tempverticesbuffer8,tempverticesbuffer9,tempverticesbuffer10,tempverticesbuffer11, verticesbuffer, actvvs, _actvvstravstate, (sweepparams.source_partition % LOADFACTORFORREDUCE) * ((_globalparams.applyvertexbuffersz / VDATAPACKINGFACTOR) / 2), ((_globalparams.applyvertexbuffersz / VDATAPACKINGFACTOR) / 2), sweepparams, _globalparams);
			#else 
			unifydata_syn(_config.enablereduce, tempverticesbuffer0,tempverticesbuffer1,tempverticesbuffer2,tempverticesbuffer3,tempverticesbuffer4,tempverticesbuffer5,tempverticesbuffer6,tempverticesbuffer7,tempverticesbuffer8,tempverticesbuffer9,tempverticesbuffer10,tempverticesbuffer11,, verticesbuffer, (sweepparams.source_partition % LOADFACTORFORREDUCE) * ((_globalparams.applyvertexbuffersz / VDATAPACKINGFACTOR) / 2), ((_globalparams.applyvertexbuffersz / VDATAPACKINGFACTOR) / 2), _globalparams.GraphAlgo);
			#endif
			if((sweepparams.source_partition % LOADFACTORFORREDUCE) == LOADFACTORFORREDUCE-1){ savekeyvalues(_config.enablereduce, kvdram0, verticesbuffer, (_globalparams.baseoffset_verticesdata_kvs + ((sweepparams.source_partition - (LOADFACTORFORREDUCE-1)) * ((_globalparams.applyvertexbuffersz_kvs / VDATAPACKINGFACTOR) / 2))), PADDEDDESTBUFFER_SIZE); }
			actvvstravstate[0] = _actvvstravstate;
		
		}
		sourcestatsmarker += 1;
		
		#ifdef _DEBUGMODE_KERNELPRINTS
		actsutilityobj->printglobalvars();
		actsutilityobj->clearglobalvars();
		#endif
	}
	
	#if defined(INMEMORYGP) && defined(BFS_ALGORITHM)
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"unifydata_bfs[ec]: saving and clearning actvvs... i: "<<actvvstravstate[0].i<<", i_kvs: "<<actvvstravstate[0].i_kvs<<", (v: "<<actvvstravstate[0].v<<", k: "<<actvvstravstate[0].k<<")"<<endl;
	#endif
	savekeyvalues(ON, kvdram0, actvvs, globalparams[0].baseoffset_activevertices_kvs + actvvstravstate[0].i_kvs, actvvstravstate[0].k + 1);
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"unifydata_bfs[ec]: saving and clearning actvvs... i: "<<actvvstravstate[1].i<<", i_kvs: "<<actvvstravstate[1].i_kvs<<", (v: "<<actvvstravstate[0].v<<", k: "<<actvvstravstate[0].k<<")"<<endl;
	#endif
	savekeyvalues(ON, kvdram1, actvvs, globalparams[1].baseoffset_activevertices_kvs + actvvstravstate[1].i_kvs, actvvstravstate[0].k + 1);
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"unifydata_bfs[ec]: saving and clearning actvvs... i: "<<actvvstravstate[2].i<<", i_kvs: "<<actvvstravstate[2].i_kvs<<", (v: "<<actvvstravstate[0].v<<", k: "<<actvvstravstate[0].k<<")"<<endl;
	#endif
	savekeyvalues(ON, kvdram2, actvvs, globalparams[2].baseoffset_activevertices_kvs + actvvstravstate[2].i_kvs, actvvstravstate[0].k + 1);
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"unifydata_bfs[ec]: saving and clearning actvvs... i: "<<actvvstravstate[3].i<<", i_kvs: "<<actvvstravstate[3].i_kvs<<", (v: "<<actvvstravstate[0].v<<", k: "<<actvvstravstate[0].k<<")"<<endl;
	#endif
	savekeyvalues(ON, kvdram3, actvvs, globalparams[3].baseoffset_activevertices_kvs + actvvstravstate[3].i_kvs, actvvstravstate[0].k + 1);
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"unifydata_bfs[ec]: saving and clearning actvvs... i: "<<actvvstravstate[4].i<<", i_kvs: "<<actvvstravstate[4].i_kvs<<", (v: "<<actvvstravstate[0].v<<", k: "<<actvvstravstate[0].k<<")"<<endl;
	#endif
	savekeyvalues(ON, kvdram4, actvvs, globalparams[4].baseoffset_activevertices_kvs + actvvstravstate[4].i_kvs, actvvstravstate[0].k + 1);
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"unifydata_bfs[ec]: saving and clearning actvvs... i: "<<actvvstravstate[5].i<<", i_kvs: "<<actvvstravstate[5].i_kvs<<", (v: "<<actvvstravstate[0].v<<", k: "<<actvvstravstate[0].k<<")"<<endl;
	#endif
	savekeyvalues(ON, kvdram5, actvvs, globalparams[5].baseoffset_activevertices_kvs + actvvstravstate[5].i_kvs, actvvstravstate[0].k + 1);
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"unifydata_bfs[ec]: saving and clearning actvvs... i: "<<actvvstravstate[6].i<<", i_kvs: "<<actvvstravstate[6].i_kvs<<", (v: "<<actvvstravstate[0].v<<", k: "<<actvvstravstate[0].k<<")"<<endl;
	#endif
	savekeyvalues(ON, kvdram6, actvvs, globalparams[6].baseoffset_activevertices_kvs + actvvstravstate[6].i_kvs, actvvstravstate[0].k + 1);
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"unifydata_bfs[ec]: saving and clearning actvvs... i: "<<actvvstravstate[7].i<<", i_kvs: "<<actvvstravstate[7].i_kvs<<", (v: "<<actvvstravstate[0].v<<", k: "<<actvvstravstate[0].k<<")"<<endl;
	#endif
	savekeyvalues(ON, kvdram7, actvvs, globalparams[7].baseoffset_activevertices_kvs + actvvstravstate[7].i_kvs, actvvstravstate[0].k + 1);
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"unifydata_bfs[ec]: saving and clearning actvvs... i: "<<actvvstravstate[8].i<<", i_kvs: "<<actvvstravstate[8].i_kvs<<", (v: "<<actvvstravstate[0].v<<", k: "<<actvvstravstate[0].k<<")"<<endl;
	#endif
	savekeyvalues(ON, kvdram8, actvvs, globalparams[8].baseoffset_activevertices_kvs + actvvstravstate[8].i_kvs, actvvstravstate[0].k + 1);
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"unifydata_bfs[ec]: saving and clearning actvvs... i: "<<actvvstravstate[9].i<<", i_kvs: "<<actvvstravstate[9].i_kvs<<", (v: "<<actvvstravstate[0].v<<", k: "<<actvvstravstate[0].k<<")"<<endl;
	#endif
	savekeyvalues(ON, kvdram9, actvvs, globalparams[9].baseoffset_activevertices_kvs + actvvstravstate[9].i_kvs, actvvstravstate[0].k + 1);
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"unifydata_bfs[ec]: saving and clearning actvvs... i: "<<actvvstravstate[10].i<<", i_kvs: "<<actvvstravstate[10].i_kvs<<", (v: "<<actvvstravstate[0].v<<", k: "<<actvvstravstate[0].k<<")"<<endl;
	#endif
	savekeyvalues(ON, kvdram10, actvvs, globalparams[10].baseoffset_activevertices_kvs + actvvstravstate[10].i_kvs, actvvstravstate[0].k + 1);
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"unifydata_bfs[ec]: saving and clearning actvvs... i: "<<actvvstravstate[11].i<<", i_kvs: "<<actvvstravstate[11].i_kvs<<", (v: "<<actvvstravstate[0].v<<", k: "<<actvvstravstate[0].k<<")"<<endl;
	#endif
	savekeyvalues(ON, kvdram11, actvvs, globalparams[11].baseoffset_activevertices_kvs + actvvstravstate[11].i_kvs, actvvstravstate[0].k + 1);
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ 
	#pragma HLS UNROLL
		actvvstravstate[i].i += (actvvstravstate[0].k * VECTOR_SIZE) + actvvstravstate[0].v;
		actvvstravstate[i].i_kvs += actvvstravstate[0].k;
	}
	#endif
	
	#ifdef _DEBUGMODE_CHECKS
	actsutilityobj->countvalueslessthan("dispatch", (value_t *)&kvdram[BASEOFFSET_VERTICESDATA_KVS], BATCH_RANGE, INFINITI);
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->printglobalvars();
	#endif
	#ifdef _DEBUGMODE_STATS
	setkey(kvdram0, PADDEDKVSOURCEDRAMSZ_KVS-1, 3, actsutilityobj->globalstats_getcountvalidkvsreduced());
	setkey(kvdram0, PADDEDKVSOURCEDRAMSZ_KVS-1, 4, actsutilityobj->globalstats_getreducevar1());
	setkey(kvdram0, PADDEDKVSOURCEDRAMSZ_KVS-1, 5, actvvstravstate[0].i);
		setkey(kvdram1, PADDEDKVSOURCEDRAMSZ_KVS-1, 3, 0);
		setkey(kvdram1, PADDEDKVSOURCEDRAMSZ_KVS-1, 4, 0);
		setkey(kvdram1, PADDEDKVSOURCEDRAMSZ_KVS-1, 5, 0);
		setkey(kvdram2, PADDEDKVSOURCEDRAMSZ_KVS-1, 3, 0);
		setkey(kvdram2, PADDEDKVSOURCEDRAMSZ_KVS-1, 4, 0);
		setkey(kvdram2, PADDEDKVSOURCEDRAMSZ_KVS-1, 5, 0);
		setkey(kvdram3, PADDEDKVSOURCEDRAMSZ_KVS-1, 3, 0);
		setkey(kvdram3, PADDEDKVSOURCEDRAMSZ_KVS-1, 4, 0);
		setkey(kvdram3, PADDEDKVSOURCEDRAMSZ_KVS-1, 5, 0);
		setkey(kvdram4, PADDEDKVSOURCEDRAMSZ_KVS-1, 3, 0);
		setkey(kvdram4, PADDEDKVSOURCEDRAMSZ_KVS-1, 4, 0);
		setkey(kvdram4, PADDEDKVSOURCEDRAMSZ_KVS-1, 5, 0);
		setkey(kvdram5, PADDEDKVSOURCEDRAMSZ_KVS-1, 3, 0);
		setkey(kvdram5, PADDEDKVSOURCEDRAMSZ_KVS-1, 4, 0);
		setkey(kvdram5, PADDEDKVSOURCEDRAMSZ_KVS-1, 5, 0);
		setkey(kvdram6, PADDEDKVSOURCEDRAMSZ_KVS-1, 3, 0);
		setkey(kvdram6, PADDEDKVSOURCEDRAMSZ_KVS-1, 4, 0);
		setkey(kvdram6, PADDEDKVSOURCEDRAMSZ_KVS-1, 5, 0);
		setkey(kvdram7, PADDEDKVSOURCEDRAMSZ_KVS-1, 3, 0);
		setkey(kvdram7, PADDEDKVSOURCEDRAMSZ_KVS-1, 4, 0);
		setkey(kvdram7, PADDEDKVSOURCEDRAMSZ_KVS-1, 5, 0);
		setkey(kvdram8, PADDEDKVSOURCEDRAMSZ_KVS-1, 3, 0);
		setkey(kvdram8, PADDEDKVSOURCEDRAMSZ_KVS-1, 4, 0);
		setkey(kvdram8, PADDEDKVSOURCEDRAMSZ_KVS-1, 5, 0);
		setkey(kvdram9, PADDEDKVSOURCEDRAMSZ_KVS-1, 3, 0);
		setkey(kvdram9, PADDEDKVSOURCEDRAMSZ_KVS-1, 4, 0);
		setkey(kvdram9, PADDEDKVSOURCEDRAMSZ_KVS-1, 5, 0);
		setkey(kvdram10, PADDEDKVSOURCEDRAMSZ_KVS-1, 3, 0);
		setkey(kvdram10, PADDEDKVSOURCEDRAMSZ_KVS-1, 4, 0);
		setkey(kvdram10, PADDEDKVSOURCEDRAMSZ_KVS-1, 5, 0);
		setkey(kvdram11, PADDEDKVSOURCEDRAMSZ_KVS-1, 3, 0);
		setkey(kvdram11, PADDEDKVSOURCEDRAMSZ_KVS-1, 4, 0);
		setkey(kvdram11, PADDEDKVSOURCEDRAMSZ_KVS-1, 5, 0);
	cout<<"dispatch_reduceonly: number of active vertices for next iteration: "<<actvvstravstate[0].i<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ 
		cout<<"dispatch_reduceonly:: actvvstravstate[i].i: "<<actvvstravstate[i].i<<endl;
		cout<<"dispatch_reduceonly:: actvvstravstate[i].i_kvs: "<<actvvstravstate[i].i_kvs<<endl;
		cout<<"dispatch_reduceonly:: actvvstravstate[i].v: "<<actvvstravstate[i].v<<endl;
		cout<<"dispatch_reduceonly:: actvvstravstate[i].k: "<<actvvstravstate[i].k<<endl;
	}
	#endif
	return;
}
 
travstate_t
	#ifdef SW 
	acts::
	#endif 
dispatch_reduceonly_parallelsync(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11, 
keyvalue_t tempverticesbuffer0[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t tempverticesbuffer1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t tempverticesbuffer2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t tempverticesbuffer3[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t tempverticesbuffer4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t tempverticesbuffer5[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t tempverticesbuffer6[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t tempverticesbuffer7[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t tempverticesbuffer8[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t tempverticesbuffer9[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t tempverticesbuffer10[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t tempverticesbuffer11[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
keyvalue_t vubufferpp00[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t vubufferpp01[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t vubufferpp02[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t vubufferpp03[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t vubufferpp04[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t vubufferpp05[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t vubufferpp06[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t vubufferpp07[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t vubufferpp08[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t vubufferpp09[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t vubufferpp010[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t vubufferpp011[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
keyvalue_t vubufferpp10[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t vubufferpp11[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t vubufferpp12[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t vubufferpp13[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t vubufferpp14[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t vubufferpp15[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t vubufferpp16[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t vubufferpp17[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t vubufferpp18[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t vubufferpp19[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t vubufferpp110[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t vubufferpp111[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],			travstate_t actvvstravstate, globalparams_t globalparams[NUMSUBCPUTHREADS]){
	analysis_type analysis_sourceploop = (1 << (NUM_PARTITIONS_POW * TREE_DEPTH));
	analysis_type analysis_reduceloop = (MAXKVDATA_BATCHSIZE / (1 << (NUM_PARTITIONS_POW * TREE_DEPTH))) / SRCBUFFER_SIZE;
	analysis_type analysis_treedepth = TREE_DEPTH;
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printparameters();
	actsutilityobj->printglobalvars();
	#endif 
	#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(_DEBUGMODE_CHECKS2)
	actsutilityobj->clearglobalvars();
	#endif
	
	keyvalue_t sourcevbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = sourcevbuffer

	keyvalue_t destvbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = destvbuffer
	
	// 	// keyvalue_t tempverticesbuffer0[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	// #pragma HLS array_partition variable = tempverticesbuffer0
	// 	// keyvalue_t tempverticesbuffer1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	// #pragma HLS array_partition variable = tempverticesbuffer1
	// 	// keyvalue_t tempverticesbuffer2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	// #pragma HLS array_partition variable = tempverticesbuffer2
	// 	// keyvalue_t tempverticesbuffer3[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	// #pragma HLS array_partition variable = tempverticesbuffer3
	// 	// keyvalue_t tempverticesbuffer4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	// #pragma HLS array_partition variable = tempverticesbuffer4
	// 	// keyvalue_t tempverticesbuffer5[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	// #pragma HLS array_partition variable = tempverticesbuffer5
	// 	// keyvalue_t tempverticesbuffer6[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	// #pragma HLS array_partition variable = tempverticesbuffer6
	// 	// keyvalue_t tempverticesbuffer7[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	// #pragma HLS array_partition variable = tempverticesbuffer7
	// 	// keyvalue_t tempverticesbuffer8[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	// #pragma HLS array_partition variable = tempverticesbuffer8
	// 	// keyvalue_t tempverticesbuffer9[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	// #pragma HLS array_partition variable = tempverticesbuffer9
	// 	// keyvalue_t tempverticesbuffer10[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	// #pragma HLS array_partition variable = tempverticesbuffer10
	// 	// keyvalue_t tempverticesbuffer11[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	// #pragma HLS array_partition variable = tempverticesbuffer11
	// 	
	#ifdef HW 
	
	// 	// keyvalue_t vubufferpp00[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE]; // share?
	// #pragma HLS array_partition variable = vubufferpp00
	// keyvalue_t vubufferpp10[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	// #pragma HLS array_partition variable = vubufferpp10
	// 	// keyvalue_t vubufferpp01[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE]; // share?
	// #pragma HLS array_partition variable = vubufferpp01
	// keyvalue_t vubufferpp11[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	// #pragma HLS array_partition variable = vubufferpp11
	// 	// keyvalue_t vubufferpp02[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE]; // share?
	// #pragma HLS array_partition variable = vubufferpp02
	// keyvalue_t vubufferpp12[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	// #pragma HLS array_partition variable = vubufferpp12
	// 	// keyvalue_t vubufferpp03[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE]; // share?
	// #pragma HLS array_partition variable = vubufferpp03
	// keyvalue_t vubufferpp13[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	// #pragma HLS array_partition variable = vubufferpp13
	// 	// keyvalue_t vubufferpp04[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE]; // share?
	// #pragma HLS array_partition variable = vubufferpp04
	// keyvalue_t vubufferpp14[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	// #pragma HLS array_partition variable = vubufferpp14
	// 	// keyvalue_t vubufferpp05[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE]; // share?
	// #pragma HLS array_partition variable = vubufferpp05
	// keyvalue_t vubufferpp15[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	// #pragma HLS array_partition variable = vubufferpp15
	// 	// keyvalue_t vubufferpp06[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE]; // share?
	// #pragma HLS array_partition variable = vubufferpp06
	// keyvalue_t vubufferpp16[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	// #pragma HLS array_partition variable = vubufferpp16
	// 	// keyvalue_t vubufferpp07[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE]; // share?
	// #pragma HLS array_partition variable = vubufferpp07
	// keyvalue_t vubufferpp17[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	// #pragma HLS array_partition variable = vubufferpp17
	// 	// keyvalue_t vubufferpp08[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE]; // share?
	// #pragma HLS array_partition variable = vubufferpp08
	// keyvalue_t vubufferpp18[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	// #pragma HLS array_partition variable = vubufferpp18
	// 	// keyvalue_t vubufferpp09[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE]; // share?
	// #pragma HLS array_partition variable = vubufferpp09
	// keyvalue_t vubufferpp19[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	// #pragma HLS array_partition variable = vubufferpp19
	// 	// keyvalue_t vubufferpp010[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE]; // share?
	// #pragma HLS array_partition variable = vubufferpp010
	// keyvalue_t vubufferpp110[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	// #pragma HLS array_partition variable = vubufferpp110
	// 	// keyvalue_t vubufferpp011[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE]; // share?
	// #pragma HLS array_partition variable = vubufferpp011
	// keyvalue_t vubufferpp111[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	// #pragma HLS array_partition variable = vubufferpp111
	// 	
	#else 
	keyvalue_t vubufferpp0[NUMSUBCPUTHREADS][VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	keyvalue_t vubufferpp1[NUMSUBCPUTHREADS][VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#endif

	keyvalue_t actvvs0[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = actvvs0 

	keyvalue_t buffer0_setof2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer0_setof2
	keyvalue_t buffer0_setof4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer0_setof4
	keyvalue_t buffer0_setof8[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer0_setof8

	skeyvalue_t capsule0_so1[8];
	#pragma HLS ARRAY_PARTITION variable=capsule0_so1 complete
	skeyvalue_t capsule0_so2[4];
	#pragma HLS ARRAY_PARTITION variable=capsule0_so2 complete
	skeyvalue_t capsule0_so4[2];
	#pragma HLS ARRAY_PARTITION variable=capsule0_so4 complete
	skeyvalue_t capsule0_so8;
	
	buffer_type cutoffs0[VECTOR_SIZE];
	keyvalue_t actvvs1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = actvvs1 

	keyvalue_t buffer1_setof2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer1_setof2
	keyvalue_t buffer1_setof4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer1_setof4
	keyvalue_t buffer1_setof8[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer1_setof8

	skeyvalue_t capsule1_so1[8];
	#pragma HLS ARRAY_PARTITION variable=capsule1_so1 complete
	skeyvalue_t capsule1_so2[4];
	#pragma HLS ARRAY_PARTITION variable=capsule1_so2 complete
	skeyvalue_t capsule1_so4[2];
	#pragma HLS ARRAY_PARTITION variable=capsule1_so4 complete
	skeyvalue_t capsule1_so8;
	
	buffer_type cutoffs1[VECTOR_SIZE];
	keyvalue_t actvvs2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = actvvs2 

	keyvalue_t buffer2_setof2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer2_setof2
	keyvalue_t buffer2_setof4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer2_setof4
	keyvalue_t buffer2_setof8[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer2_setof8

	skeyvalue_t capsule2_so1[8];
	#pragma HLS ARRAY_PARTITION variable=capsule2_so1 complete
	skeyvalue_t capsule2_so2[4];
	#pragma HLS ARRAY_PARTITION variable=capsule2_so2 complete
	skeyvalue_t capsule2_so4[2];
	#pragma HLS ARRAY_PARTITION variable=capsule2_so4 complete
	skeyvalue_t capsule2_so8;
	
	buffer_type cutoffs2[VECTOR_SIZE];
	keyvalue_t actvvs3[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = actvvs3 

	keyvalue_t buffer3_setof2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer3_setof2
	keyvalue_t buffer3_setof4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer3_setof4
	keyvalue_t buffer3_setof8[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer3_setof8

	skeyvalue_t capsule3_so1[8];
	#pragma HLS ARRAY_PARTITION variable=capsule3_so1 complete
	skeyvalue_t capsule3_so2[4];
	#pragma HLS ARRAY_PARTITION variable=capsule3_so2 complete
	skeyvalue_t capsule3_so4[2];
	#pragma HLS ARRAY_PARTITION variable=capsule3_so4 complete
	skeyvalue_t capsule3_so8;
	
	buffer_type cutoffs3[VECTOR_SIZE];
	
	travstate_t rtravstate[NUMSUBCPUTHREADS];
	#pragma HLS ARRAY_PARTITION variable=rtravstate complete
	travstate_t rtravstatepp0[NUMSUBCPUTHREADS];
	#pragma HLS ARRAY_PARTITION variable=rtravstatepp0 complete
	travstate_t rtravstatepp1[NUMSUBCPUTHREADS];
	#pragma HLS ARRAY_PARTITION variable=rtravstatepp1 complete
	keyvalue_t moretravstates[NUMSUBCPUTHREADS][MAXLOADFACTORFORREDUCE]; // CRITICAL AUTOMATEME (dataset dependent).
	#pragma HLS array_partition variable = moretravstates
	
	unsigned int en[NUMSUBCPUTHREADS];
	#pragma HLS ARRAY_PARTITION variable=en complete
	unsigned int enpp0[NUMSUBCPUTHREADS];
	#pragma HLS ARRAY_PARTITION variable=enpp0 complete
	unsigned int enpp1[NUMSUBCPUTHREADS];
	#pragma HLS ARRAY_PARTITION variable=enpp1 complete
	sweepparams_t sweepparams;
	keyvalue_t offsetsandsizes[NUMCOMPUTEUNITS];
	#pragma HLS ARRAY_PARTITION variable=offsetsandsizes complete
	
	unsigned int itercount = 0;
	unsigned int itercount_actvvs = 0; //
	bool_type pp0writeen = ON;
	bool_type writeen_actvvs = ON; //
	bool_type nonzeroactvvsreturned = ON;
	
	batch_type offset1_kvs;
	batch_type offset2_kvs;
	batch_type offset_kvs;
	
	actvvstravstate.i=0; actvvstravstate.i_kvs=0; actvvstravstate.v=0; actvvstravstate.k=0; 
	step_type currentLOP = globalparams[0].beginLOP + globalparams[0].numLOPs - 1;
	batch_type num_source_partitions = get_num_source_partitions(currentLOP);
	unsigned int sourcestatsmarker = 0;
	for(unsigned int k=0; k<globalparams[0].treedepth; k++){ 
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_treedepth avg=analysis_treedepth
		sourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); 
	}
	batch_type ntravszs = 0;
	globalparams_t _globalparams = globalparams[0];
	for(unsigned int i=1; i<NUMCOMPUTEUNITS; i++){ offsetsandsizes[i].key = 0; offsetsandsizes[i].value = 0; }
	unsigned int reducesubchunksz = (_globalparams.applyvertexbuffersz / VDATAPACKINGFACTOR) / 2; // NOT USED.
	unsigned int reducechunksz_kvs = (reducesubchunksz * LOADFACTORFORREDUCE) / VECTOR_SIZE;

	MAIN_LOOP: for(batch_type source_partition=0; source_partition<num_source_partitions + 1; source_partition+=_globalparams.loadfactorforreduce){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_sourceploop avg=analysis_sourceploop
		#ifdef _DEBUGMODE_KERNELPRINTS2
		actsutilityobj->print2("### dispatch::reduce:: source_partition", "currentLOP", source_partition, currentLOP); 							
		#endif
		
		bool_type enablereduce = ON;
		bool_type enableflush = OFF;
		
		if(source_partition >= num_source_partitions){ enableflush = ON; } else { enableflush = OFF; }
		
		if(enableflush == OFF){
 
		rtravstate[0] = gettravstate(ON, kvdram0, globalparams[0], currentLOP, sourcestatsmarker, source_partition, moretravstates[0]);
 
		rtravstate[1] = gettravstate(ON, kvdram1, globalparams[1], currentLOP, sourcestatsmarker, source_partition, moretravstates[1]);
 
		rtravstate[2] = gettravstate(ON, kvdram2, globalparams[2], currentLOP, sourcestatsmarker, source_partition, moretravstates[2]);
 
		rtravstate[3] = gettravstate(ON, kvdram3, globalparams[3], currentLOP, sourcestatsmarker, source_partition, moretravstates[3]);
 
		rtravstate[4] = gettravstate(ON, kvdram4, globalparams[4], currentLOP, sourcestatsmarker, source_partition, moretravstates[4]);
 
		rtravstate[5] = gettravstate(ON, kvdram5, globalparams[5], currentLOP, sourcestatsmarker, source_partition, moretravstates[5]);
 
		rtravstate[6] = gettravstate(ON, kvdram6, globalparams[6], currentLOP, sourcestatsmarker, source_partition, moretravstates[6]);
 
		rtravstate[7] = gettravstate(ON, kvdram7, globalparams[7], currentLOP, sourcestatsmarker, source_partition, moretravstates[7]);
 
		rtravstate[8] = gettravstate(ON, kvdram8, globalparams[8], currentLOP, sourcestatsmarker, source_partition, moretravstates[8]);
 
		rtravstate[9] = gettravstate(ON, kvdram9, globalparams[9], currentLOP, sourcestatsmarker, source_partition, moretravstates[9]);
 
		rtravstate[10] = gettravstate(ON, kvdram10, globalparams[10], currentLOP, sourcestatsmarker, source_partition, moretravstates[10]);
 
		rtravstate[11] = gettravstate(ON, kvdram11, globalparams[11], currentLOP, sourcestatsmarker, source_partition, moretravstates[11]);
 }
		ntravszs = 0;
		for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ for(batch_type k=0; k<_globalparams.loadfactorforreduce; k++){ ntravszs += moretravstates[i][k].value; }}
		if((ntravszs == 0) || (source_partition >= num_source_partitions)){ enablereduce = OFF; } else { enablereduce = ON; }
		
		if(enablereduce == ON || enableflush == ON){
			readkeyvalues(enablereduce, kvdram0, sourcevbuffer, (_globalparams.baseoffset_verticesdata_kvs + (source_partition * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE); // reducechunksz_kvs?
			replicatedata_syn(enablereduce, sourcevbuffer, tempverticesbuffer0,tempverticesbuffer1,tempverticesbuffer2,tempverticesbuffer3,tempverticesbuffer4,tempverticesbuffer5,tempverticesbuffer6,tempverticesbuffer7,tempverticesbuffer8,tempverticesbuffer9,tempverticesbuffer10,tempverticesbuffer11, 0, reducesubchunksz);
			
			MAIN_LOOP2: for(batch_type index=0; index<_globalparams.loadfactorforreduce; index+=1){
				unsigned int spartition = source_partition + index;
				sweepparams = getsweepparams(_globalparams, currentLOP, spartition);
				
				#ifdef _DEBUGMODE_KERNELPRINTS2
				if(enablereduce == ON){ for(unsigned int i = 0; i < 1; i++){ actsutilityobj->print7("dispatch::reduce:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", source_partition + index, sweepparams.upperlimit, rtravstate[i].begin_kvs * VECTOR_SIZE, rtravstate[i].end_kvs * VECTOR_SIZE, rtravstate[i].size_kvs * VECTOR_SIZE, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams.currentLOP)), sweepparams.currentLOP); }}
				else { actsutilityobj->print7("dispatch::flush:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", source_partition + index, sweepparams.upperlimit, NAp, NAp, NAp, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams.currentLOP)), sweepparams.currentLOP); }
				#endif
					
				if(itercount >= 2){ pp0writeen = ON; } else { pp0writeen = OFF; }
				for(unsigned int v=0; v<8; v++){ 
				#pragma HLS UNROLL
 
					capsule0_so1[v].key = 0; 
					capsule0_so1[v].value = 0; 
 
					capsule1_so1[v].key = 0; 
					capsule1_so1[v].value = 0; 
 
					capsule2_so1[v].key = 0; 
					capsule2_so1[v].value = 0; 
 
					capsule3_so1[v].key = 0; 
					capsule3_so1[v].value = 0; 
 
				}
				
				for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ 
				#pragma HLS UNROLL
					rtravstatepp0[i] = rtravstate[i]; 
					rtravstatepp1[i] = rtravstate[i]; 
				}
				
				batch_type maxsz_kvs = 0;
				for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ if(rtravstate[i].size_kvs > maxsz_kvs){ maxsz_kvs = rtravstate[i].size_kvs; }}
				unsigned int enpp1f = ON;
				#ifdef PP1
				unsigned int totsz_kvs = maxsz_kvs + rtravstate[0].skip_kvs;
				#else 
				unsigned int totsz_kvs = maxsz_kvs;
				#endif
				if(enablereduce == OFF){ totsz_kvs = 0; }

				#ifdef PP1
				MAIN_LOOP1E_REDUCE: for(offset_kvs=0; offset_kvs<totsz_kvs; offset_kvs+=2*rtravstate[0].skip_kvs){
				#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_reduceloop avg=analysis_reduceloop
					#ifdef _DEBUGMODE_KERNELPRINTS
					for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ actsutilityobj->print5("### dispatch::reduce:: offset1_kvs", "offset2_kvs", "begin_kvs", "end_kvs", "skip", rtravstate[i].begin_kvs + offset1_kvs, rtravstate[i].begin_kvs + offset2_kvs, rtravstate[i].begin_kvs, rtravstate[i].end_kvs, SRCBUFFER_SIZE); }
					#endif
					
					if(offset_kvs > 0){ enpp1f = ON; } else { enpp1f = OFF; }
					
					// read 0
					offset1_kvs = offset_kvs;
					for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ 
					#pragma HLS UNROLL
						if(offset1_kvs < rtravstate[i].size_kvs){ enpp0[i] = ON; } else { enpp0[i] = OFF; }
					}
					for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ 
					#pragma HLS UNROLL
						if(enpp0[i] == ON){ rtravstatepp0[i].i_kvs = rtravstatepp0[i].begin_kvs + offset1_kvs; }
					}
					readkeyvalues(enpp0[0], kvdram0, vubufferpp0[0], (sweepparams.worksourcebaseaddress_kvs + rtravstatepp0[0].begin_kvs + offset1_kvs), SRCBUFFER_SIZE, rtravstatepp0[0]);
					readkeyvalues(enpp0[1], kvdram1, vubufferpp0[1], (sweepparams.worksourcebaseaddress_kvs + rtravstatepp0[1].begin_kvs + offset1_kvs), SRCBUFFER_SIZE, rtravstatepp0[1]);
					readkeyvalues(enpp0[2], kvdram2, vubufferpp0[2], (sweepparams.worksourcebaseaddress_kvs + rtravstatepp0[2].begin_kvs + offset1_kvs), SRCBUFFER_SIZE, rtravstatepp0[2]);
					readkeyvalues(enpp0[3], kvdram3, vubufferpp0[3], (sweepparams.worksourcebaseaddress_kvs + rtravstatepp0[3].begin_kvs + offset1_kvs), SRCBUFFER_SIZE, rtravstatepp0[3]);
					readkeyvalues(enpp0[4], kvdram4, vubufferpp0[4], (sweepparams.worksourcebaseaddress_kvs + rtravstatepp0[4].begin_kvs + offset1_kvs), SRCBUFFER_SIZE, rtravstatepp0[4]);
					readkeyvalues(enpp0[5], kvdram5, vubufferpp0[5], (sweepparams.worksourcebaseaddress_kvs + rtravstatepp0[5].begin_kvs + offset1_kvs), SRCBUFFER_SIZE, rtravstatepp0[5]);
					readkeyvalues(enpp0[6], kvdram6, vubufferpp0[6], (sweepparams.worksourcebaseaddress_kvs + rtravstatepp0[6].begin_kvs + offset1_kvs), SRCBUFFER_SIZE, rtravstatepp0[6]);
					readkeyvalues(enpp0[7], kvdram7, vubufferpp0[7], (sweepparams.worksourcebaseaddress_kvs + rtravstatepp0[7].begin_kvs + offset1_kvs), SRCBUFFER_SIZE, rtravstatepp0[7]);
					readkeyvalues(enpp0[8], kvdram8, vubufferpp0[8], (sweepparams.worksourcebaseaddress_kvs + rtravstatepp0[8].begin_kvs + offset1_kvs), SRCBUFFER_SIZE, rtravstatepp0[8]);
					readkeyvalues(enpp0[9], kvdram9, vubufferpp0[9], (sweepparams.worksourcebaseaddress_kvs + rtravstatepp0[9].begin_kvs + offset1_kvs), SRCBUFFER_SIZE, rtravstatepp0[9]);
					readkeyvalues(enpp0[10], kvdram10, vubufferpp0[10], (sweepparams.worksourcebaseaddress_kvs + rtravstatepp0[10].begin_kvs + offset1_kvs), SRCBUFFER_SIZE, rtravstatepp0[10]);
					readkeyvalues(enpp0[11], kvdram11, vubufferpp0[11], (sweepparams.worksourcebaseaddress_kvs + rtravstatepp0[11].begin_kvs + offset1_kvs), SRCBUFFER_SIZE, rtravstatepp0[11]);
					// reduce 1
					#if defined(INMEMORYGP) && defined(BFS_ALGORITHM)
					reduce_bfs(enpp1f, enpp1[0], vubufferpp1[0], tempverticesbuffer0, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp1[0], _globalparams); 
					reduce_bfs(enpp1f, enpp1[1], vubufferpp1[1], tempverticesbuffer1, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp1[1], _globalparams); 
					reduce_bfs(enpp1f, enpp1[2], vubufferpp1[2], tempverticesbuffer2, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp1[2], _globalparams); 
					reduce_bfs(enpp1f, enpp1[3], vubufferpp1[3], tempverticesbuffer3, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp1[3], _globalparams); 
					reduce_bfs(enpp1f, enpp1[4], vubufferpp1[4], tempverticesbuffer4, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp1[4], _globalparams); 
					reduce_bfs(enpp1f, enpp1[5], vubufferpp1[5], tempverticesbuffer5, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp1[5], _globalparams); 
					reduce_bfs(enpp1f, enpp1[6], vubufferpp1[6], tempverticesbuffer6, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp1[6], _globalparams); 
					reduce_bfs(enpp1f, enpp1[7], vubufferpp1[7], tempverticesbuffer7, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp1[7], _globalparams); 
					reduce_bfs(enpp1f, enpp1[8], vubufferpp1[8], tempverticesbuffer8, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp1[8], _globalparams); 
					reduce_bfs(enpp1f, enpp1[9], vubufferpp1[9], tempverticesbuffer9, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp1[9], _globalparams); 
					reduce_bfs(enpp1f, enpp1[10], vubufferpp1[10], tempverticesbuffer10, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp1[10], _globalparams); 
					reduce_bfs(enpp1f, enpp1[11], vubufferpp1[11], tempverticesbuffer11, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp1[11], _globalparams); 
					#else 
					reduce(enpp1f, enpp1[0], vubufferpp1[0], tempverticesbuffer0, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp1[0], _globalparams); 
					reduce(enpp1f, enpp1[1], vubufferpp1[1], tempverticesbuffer1, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp1[1], _globalparams); 
					reduce(enpp1f, enpp1[2], vubufferpp1[2], tempverticesbuffer2, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp1[2], _globalparams); 
					reduce(enpp1f, enpp1[3], vubufferpp1[3], tempverticesbuffer3, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp1[3], _globalparams); 
					reduce(enpp1f, enpp1[4], vubufferpp1[4], tempverticesbuffer4, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp1[4], _globalparams); 
					reduce(enpp1f, enpp1[5], vubufferpp1[5], tempverticesbuffer5, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp1[5], _globalparams); 
					reduce(enpp1f, enpp1[6], vubufferpp1[6], tempverticesbuffer6, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp1[6], _globalparams); 
					reduce(enpp1f, enpp1[7], vubufferpp1[7], tempverticesbuffer7, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp1[7], _globalparams); 
					reduce(enpp1f, enpp1[8], vubufferpp1[8], tempverticesbuffer8, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp1[8], _globalparams); 
					reduce(enpp1f, enpp1[9], vubufferpp1[9], tempverticesbuffer9, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp1[9], _globalparams); 
					reduce(enpp1f, enpp1[10], vubufferpp1[10], tempverticesbuffer10, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp1[10], _globalparams); 
					reduce(enpp1f, enpp1[11], vubufferpp1[11], tempverticesbuffer11, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp1[11], _globalparams); 
					#endif
					
					// reduce 0
					#if defined(INMEMORYGP) && defined(BFS_ALGORITHM)
					reduce_bfs(ON, enpp0[0], vubufferpp0[0], tempverticesbuffer0, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp0[0], _globalparams);
					reduce_bfs(ON, enpp0[1], vubufferpp0[1], tempverticesbuffer1, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp0[1], _globalparams);
					reduce_bfs(ON, enpp0[2], vubufferpp0[2], tempverticesbuffer2, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp0[2], _globalparams);
					reduce_bfs(ON, enpp0[3], vubufferpp0[3], tempverticesbuffer3, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp0[3], _globalparams);
					reduce_bfs(ON, enpp0[4], vubufferpp0[4], tempverticesbuffer4, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp0[4], _globalparams);
					reduce_bfs(ON, enpp0[5], vubufferpp0[5], tempverticesbuffer5, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp0[5], _globalparams);
					reduce_bfs(ON, enpp0[6], vubufferpp0[6], tempverticesbuffer6, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp0[6], _globalparams);
					reduce_bfs(ON, enpp0[7], vubufferpp0[7], tempverticesbuffer7, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp0[7], _globalparams);
					reduce_bfs(ON, enpp0[8], vubufferpp0[8], tempverticesbuffer8, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp0[8], _globalparams);
					reduce_bfs(ON, enpp0[9], vubufferpp0[9], tempverticesbuffer9, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp0[9], _globalparams);
					reduce_bfs(ON, enpp0[10], vubufferpp0[10], tempverticesbuffer10, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp0[10], _globalparams);
					reduce_bfs(ON, enpp0[11], vubufferpp0[11], tempverticesbuffer11, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp0[11], _globalparams);
					#else 
					reduce(ON, enpp0[0], vubufferpp0[0], tempverticesbuffer0, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp0[0], _globalparams);
					reduce(ON, enpp0[1], vubufferpp0[1], tempverticesbuffer1, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp0[1], _globalparams);
					reduce(ON, enpp0[2], vubufferpp0[2], tempverticesbuffer2, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp0[2], _globalparams);
					reduce(ON, enpp0[3], vubufferpp0[3], tempverticesbuffer3, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp0[3], _globalparams);
					reduce(ON, enpp0[4], vubufferpp0[4], tempverticesbuffer4, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp0[4], _globalparams);
					reduce(ON, enpp0[5], vubufferpp0[5], tempverticesbuffer5, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp0[5], _globalparams);
					reduce(ON, enpp0[6], vubufferpp0[6], tempverticesbuffer6, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp0[6], _globalparams);
					reduce(ON, enpp0[7], vubufferpp0[7], tempverticesbuffer7, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp0[7], _globalparams);
					reduce(ON, enpp0[8], vubufferpp0[8], tempverticesbuffer8, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp0[8], _globalparams);
					reduce(ON, enpp0[9], vubufferpp0[9], tempverticesbuffer9, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp0[9], _globalparams);
					reduce(ON, enpp0[10], vubufferpp0[10], tempverticesbuffer10, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp0[10], _globalparams);
					reduce(ON, enpp0[11], vubufferpp0[11], tempverticesbuffer11, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp0[11], _globalparams);
					#endif
					// read 1
					offset2_kvs = offset_kvs + rtravstate[0].skip_kvs;
					for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ 
					#pragma HLS UNROLL
						if(offset2_kvs < rtravstate[i].size_kvs){ enpp1[i] = ON; } else { enpp1[i] = OFF; }
					}
					for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ 
					#pragma HLS UNROLL
						if(enpp1[i] == ON){ rtravstatepp1[i].i_kvs = rtravstatepp1[i].begin_kvs + offset2_kvs; }
					}
					readkeyvalues(enpp1[0], kvdram0, vubufferpp1[0], (sweepparams.worksourcebaseaddress_kvs + rtravstatepp1[0].begin_kvs + offset2_kvs), SRCBUFFER_SIZE, rtravstatepp1[0]);
					readkeyvalues(enpp1[1], kvdram1, vubufferpp1[1], (sweepparams.worksourcebaseaddress_kvs + rtravstatepp1[1].begin_kvs + offset2_kvs), SRCBUFFER_SIZE, rtravstatepp1[1]);
					readkeyvalues(enpp1[2], kvdram2, vubufferpp1[2], (sweepparams.worksourcebaseaddress_kvs + rtravstatepp1[2].begin_kvs + offset2_kvs), SRCBUFFER_SIZE, rtravstatepp1[2]);
					readkeyvalues(enpp1[3], kvdram3, vubufferpp1[3], (sweepparams.worksourcebaseaddress_kvs + rtravstatepp1[3].begin_kvs + offset2_kvs), SRCBUFFER_SIZE, rtravstatepp1[3]);
					readkeyvalues(enpp1[4], kvdram4, vubufferpp1[4], (sweepparams.worksourcebaseaddress_kvs + rtravstatepp1[4].begin_kvs + offset2_kvs), SRCBUFFER_SIZE, rtravstatepp1[4]);
					readkeyvalues(enpp1[5], kvdram5, vubufferpp1[5], (sweepparams.worksourcebaseaddress_kvs + rtravstatepp1[5].begin_kvs + offset2_kvs), SRCBUFFER_SIZE, rtravstatepp1[5]);
					readkeyvalues(enpp1[6], kvdram6, vubufferpp1[6], (sweepparams.worksourcebaseaddress_kvs + rtravstatepp1[6].begin_kvs + offset2_kvs), SRCBUFFER_SIZE, rtravstatepp1[6]);
					readkeyvalues(enpp1[7], kvdram7, vubufferpp1[7], (sweepparams.worksourcebaseaddress_kvs + rtravstatepp1[7].begin_kvs + offset2_kvs), SRCBUFFER_SIZE, rtravstatepp1[7]);
					readkeyvalues(enpp1[8], kvdram8, vubufferpp1[8], (sweepparams.worksourcebaseaddress_kvs + rtravstatepp1[8].begin_kvs + offset2_kvs), SRCBUFFER_SIZE, rtravstatepp1[8]);
					readkeyvalues(enpp1[9], kvdram9, vubufferpp1[9], (sweepparams.worksourcebaseaddress_kvs + rtravstatepp1[9].begin_kvs + offset2_kvs), SRCBUFFER_SIZE, rtravstatepp1[9]);
					readkeyvalues(enpp1[10], kvdram10, vubufferpp1[10], (sweepparams.worksourcebaseaddress_kvs + rtravstatepp1[10].begin_kvs + offset2_kvs), SRCBUFFER_SIZE, rtravstatepp1[10]);
					readkeyvalues(enpp1[11], kvdram11, vubufferpp1[11], (sweepparams.worksourcebaseaddress_kvs + rtravstatepp1[11].begin_kvs + offset2_kvs), SRCBUFFER_SIZE, rtravstatepp1[11]);
				}
				#else 
				MAIN_LOOP1E_REDUCE: for(offset_kvs=0; offset_kvs<totsz_kvs; offset_kvs+=rtravstate[0].skip_kvs){
				#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_reduceloop avg=analysis_reduceloop
					#ifdef _DEBUGMODE_KERNELPRINTS
					for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ actsutilityobj->print4("### dispatch::reduce:: offset_kvs", "begin_kvs", "end_kvs", "skip", rtravstate[i].begin_kvs + offset_kvs, rtravstate[i].begin_kvs, rtravstate[i].end_kvs, SRCBUFFER_SIZE); }
					#endif
					
					for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ 
					#pragma HLS UNROLL
						if(offset_kvs < rtravstate[i].size_kvs){ en[i] = ON; } else { en[i] = OFF; }
					}
					
					for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ 
					#pragma HLS UNROLL
						if(en[i] == ON){ rtravstate[i].i_kvs = rtravstate[i].begin_kvs + offset_kvs; }
					}

					readkeyvalues(en[0], kvdram0, vubufferpp0[0], (sweepparams.worksourcebaseaddress_kvs + rtravstate[0].begin_kvs + offset_kvs), SRCBUFFER_SIZE, rtravstate[0]);
					readkeyvalues(en[1], kvdram1, vubufferpp0[1], (sweepparams.worksourcebaseaddress_kvs + rtravstate[1].begin_kvs + offset_kvs), SRCBUFFER_SIZE, rtravstate[1]);
					readkeyvalues(en[2], kvdram2, vubufferpp0[2], (sweepparams.worksourcebaseaddress_kvs + rtravstate[2].begin_kvs + offset_kvs), SRCBUFFER_SIZE, rtravstate[2]);
					readkeyvalues(en[3], kvdram3, vubufferpp0[3], (sweepparams.worksourcebaseaddress_kvs + rtravstate[3].begin_kvs + offset_kvs), SRCBUFFER_SIZE, rtravstate[3]);
					readkeyvalues(en[4], kvdram4, vubufferpp0[4], (sweepparams.worksourcebaseaddress_kvs + rtravstate[4].begin_kvs + offset_kvs), SRCBUFFER_SIZE, rtravstate[4]);
					readkeyvalues(en[5], kvdram5, vubufferpp0[5], (sweepparams.worksourcebaseaddress_kvs + rtravstate[5].begin_kvs + offset_kvs), SRCBUFFER_SIZE, rtravstate[5]);
					readkeyvalues(en[6], kvdram6, vubufferpp0[6], (sweepparams.worksourcebaseaddress_kvs + rtravstate[6].begin_kvs + offset_kvs), SRCBUFFER_SIZE, rtravstate[6]);
					readkeyvalues(en[7], kvdram7, vubufferpp0[7], (sweepparams.worksourcebaseaddress_kvs + rtravstate[7].begin_kvs + offset_kvs), SRCBUFFER_SIZE, rtravstate[7]);
					readkeyvalues(en[8], kvdram8, vubufferpp0[8], (sweepparams.worksourcebaseaddress_kvs + rtravstate[8].begin_kvs + offset_kvs), SRCBUFFER_SIZE, rtravstate[8]);
					readkeyvalues(en[9], kvdram9, vubufferpp0[9], (sweepparams.worksourcebaseaddress_kvs + rtravstate[9].begin_kvs + offset_kvs), SRCBUFFER_SIZE, rtravstate[9]);
					readkeyvalues(en[10], kvdram10, vubufferpp0[10], (sweepparams.worksourcebaseaddress_kvs + rtravstate[10].begin_kvs + offset_kvs), SRCBUFFER_SIZE, rtravstate[10]);
					readkeyvalues(en[11], kvdram11, vubufferpp0[11], (sweepparams.worksourcebaseaddress_kvs + rtravstate[11].begin_kvs + offset_kvs), SRCBUFFER_SIZE, rtravstate[11]);
		
					#if defined(INMEMORYGP) && defined(BFS_ALGORITHM)
					reduce_bfs(ON, en[0], vubufferpp0[0], tempverticesbuffer0, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[0], _globalparams);
					reduce_bfs(ON, en[1], vubufferpp0[1], tempverticesbuffer1, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[1], _globalparams);
					reduce_bfs(ON, en[2], vubufferpp0[2], tempverticesbuffer2, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[2], _globalparams);
					reduce_bfs(ON, en[3], vubufferpp0[3], tempverticesbuffer3, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[3], _globalparams);
					reduce_bfs(ON, en[4], vubufferpp0[4], tempverticesbuffer4, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[4], _globalparams);
					reduce_bfs(ON, en[5], vubufferpp0[5], tempverticesbuffer5, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[5], _globalparams);
					reduce_bfs(ON, en[6], vubufferpp0[6], tempverticesbuffer6, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[6], _globalparams);
					reduce_bfs(ON, en[7], vubufferpp0[7], tempverticesbuffer7, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[7], _globalparams);
					reduce_bfs(ON, en[8], vubufferpp0[8], tempverticesbuffer8, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[8], _globalparams);
					reduce_bfs(ON, en[9], vubufferpp0[9], tempverticesbuffer9, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[9], _globalparams);
					reduce_bfs(ON, en[10], vubufferpp0[10], tempverticesbuffer10, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[10], _globalparams);
					reduce_bfs(ON, en[11], vubufferpp0[11], tempverticesbuffer11, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[11], _globalparams);
					#else 
					reduce(ON, en[0], vubufferpp0[0], tempverticesbuffer0, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[0], _globalparams);
					reduce(ON, en[1], vubufferpp0[1], tempverticesbuffer1, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[1], _globalparams);
					reduce(ON, en[2], vubufferpp0[2], tempverticesbuffer2, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[2], _globalparams);
					reduce(ON, en[3], vubufferpp0[3], tempverticesbuffer3, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[3], _globalparams);
					reduce(ON, en[4], vubufferpp0[4], tempverticesbuffer4, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[4], _globalparams);
					reduce(ON, en[5], vubufferpp0[5], tempverticesbuffer5, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[5], _globalparams);
					reduce(ON, en[6], vubufferpp0[6], tempverticesbuffer6, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[6], _globalparams);
					reduce(ON, en[7], vubufferpp0[7], tempverticesbuffer7, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[7], _globalparams);
					reduce(ON, en[8], vubufferpp0[8], tempverticesbuffer8, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[8], _globalparams);
					reduce(ON, en[9], vubufferpp0[9], tempverticesbuffer9, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[9], _globalparams);
					reduce(ON, en[10], vubufferpp0[10], tempverticesbuffer10, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[10], _globalparams);
					reduce(ON, en[11], vubufferpp0[11], tempverticesbuffer11, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[11], _globalparams);
					#endif
				}	
				#endif
				
				bool_type nonzeroactvvsreturned = unifydata_bfs_syn(enablereduce, tempverticesbuffer0,tempverticesbuffer1,tempverticesbuffer2,tempverticesbuffer3,tempverticesbuffer4,tempverticesbuffer5,tempverticesbuffer6,tempverticesbuffer7,tempverticesbuffer8,tempverticesbuffer9,tempverticesbuffer10,tempverticesbuffer11, destvbuffer, 
actvvs0, capsule0_so1, cutoffs0, actvvs1, capsule1_so1, cutoffs1, actvvs2, capsule2_so1, cutoffs2, actvvs3, capsule3_so1, cutoffs3, 						index * reducesubchunksz, reducesubchunksz, sweepparams, _globalparams);
				
				// overlap functions for next iteration
				if(index < _globalparams.loadfactorforreduce-1){ 
					replicatedata_syn(enablereduce, sourcevbuffer, tempverticesbuffer0,tempverticesbuffer1,tempverticesbuffer2,tempverticesbuffer3,tempverticesbuffer4,tempverticesbuffer5,tempverticesbuffer6,tempverticesbuffer7,tempverticesbuffer8,tempverticesbuffer9,tempverticesbuffer10,tempverticesbuffer11, (index + 1) * reducesubchunksz, reducesubchunksz); // pp1
					rtravstate[0] = gettravstate(enablereduce, kvdram0, globalparams[0], currentLOP, sourcestatsmarker + index + 1, spartition + 1, moretravstates[0]);
					rtravstate[1] = gettravstate(enablereduce, kvdram1, globalparams[1], currentLOP, sourcestatsmarker + index + 1, spartition + 1, moretravstates[1]);
					rtravstate[2] = gettravstate(enablereduce, kvdram2, globalparams[2], currentLOP, sourcestatsmarker + index + 1, spartition + 1, moretravstates[2]);
					rtravstate[3] = gettravstate(enablereduce, kvdram3, globalparams[3], currentLOP, sourcestatsmarker + index + 1, spartition + 1, moretravstates[3]);
					rtravstate[4] = gettravstate(enablereduce, kvdram4, globalparams[4], currentLOP, sourcestatsmarker + index + 1, spartition + 1, moretravstates[4]);
					rtravstate[5] = gettravstate(enablereduce, kvdram5, globalparams[5], currentLOP, sourcestatsmarker + index + 1, spartition + 1, moretravstates[5]);
					rtravstate[6] = gettravstate(enablereduce, kvdram6, globalparams[6], currentLOP, sourcestatsmarker + index + 1, spartition + 1, moretravstates[6]);
					rtravstate[7] = gettravstate(enablereduce, kvdram7, globalparams[7], currentLOP, sourcestatsmarker + index + 1, spartition + 1, moretravstates[7]);
					rtravstate[8] = gettravstate(enablereduce, kvdram8, globalparams[8], currentLOP, sourcestatsmarker + index + 1, spartition + 1, moretravstates[8]);
					rtravstate[9] = gettravstate(enablereduce, kvdram9, globalparams[9], currentLOP, sourcestatsmarker + index + 1, spartition + 1, moretravstates[9]);
					rtravstate[10] = gettravstate(enablereduce, kvdram10, globalparams[10], currentLOP, sourcestatsmarker + index + 1, spartition + 1, moretravstates[10]);
					rtravstate[11] = gettravstate(enablereduce, kvdram11, globalparams[11], currentLOP, sourcestatsmarker + index + 1, spartition + 1, moretravstates[11]);
				}
				
				if(nonzeroactvvsreturned == ON || (enableflush == ON && index < 4)){
					#ifdef PIPELINED_SYNCACTVVS
					if(itercount_actvvs >= 2){ writeen_actvvs = ON; } else { writeen_actvvs = OFF; }
					capsule0_so8 = runpipeline_1partition(ON, actvvs0, capsule0_so1, buffer0_setof2, capsule0_so2, buffer0_setof4, capsule0_so4, buffer0_setof8, capsule0_so8, sweepparams.currentLOP, sweepparams, WORKBUFFER_SIZE, cutoffs0, itercount_actvvs, _globalparams);
					// savekeyvalues_pipeline_sync
					// savekeyvalues_pipeline_2sync
					savekeyvalues_pipeline_2sync(writeen_actvvs, kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9,kvdram10,kvdram11, offsetsandsizes, buffer0_setof8, actvvstravstate.i_kvs, capsule0_so8.value / VECTOR_SIZE, _globalparams); 
					if(writeen_actvvs == ON){ actvvstravstate.i_kvs += capsule0_so8.value / VECTOR_SIZE; actvvstravstate.i += 2 * capsule0_so8.value; }
					itercount_actvvs += 1;
					if(itercount_actvvs >= 2){ writeen_actvvs = ON; } else { writeen_actvvs = OFF; }
					capsule1_so8 = runpipeline_1partition(ON, actvvs1, capsule1_so1, buffer0_setof2, capsule0_so2, buffer0_setof4, capsule0_so4, buffer1_setof8, capsule1_so8, sweepparams.currentLOP, sweepparams, WORKBUFFER_SIZE, cutoffs1, itercount_actvvs, _globalparams);
					// savekeyvalues_pipeline_sync
					// savekeyvalues_pipeline_2sync
					savekeyvalues_pipeline_2sync(writeen_actvvs, kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9,kvdram10,kvdram11, offsetsandsizes, buffer1_setof8, actvvstravstate.i_kvs, capsule1_so8.value / VECTOR_SIZE, _globalparams); 
					if(writeen_actvvs == ON){ actvvstravstate.i_kvs += capsule1_so8.value / VECTOR_SIZE; actvvstravstate.i += 2 * capsule1_so8.value; }
					itercount_actvvs += 1;
					if(itercount_actvvs >= 2){ writeen_actvvs = ON; } else { writeen_actvvs = OFF; }
					capsule0_so8 = runpipeline_1partition(ON, actvvs2, capsule2_so1, buffer0_setof2, capsule0_so2, buffer0_setof4, capsule0_so4, buffer0_setof8, capsule0_so8, sweepparams.currentLOP, sweepparams, WORKBUFFER_SIZE, cutoffs2, itercount_actvvs, _globalparams);
					// savekeyvalues_pipeline_sync
					// savekeyvalues_pipeline_2sync
					savekeyvalues_pipeline_2sync(writeen_actvvs, kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9,kvdram10,kvdram11, offsetsandsizes, buffer0_setof8, actvvstravstate.i_kvs, capsule0_so8.value / VECTOR_SIZE, _globalparams); 
					if(writeen_actvvs == ON){ actvvstravstate.i_kvs += capsule0_so8.value / VECTOR_SIZE; actvvstravstate.i += 2 * capsule0_so8.value; }
					itercount_actvvs += 1;
					if(itercount_actvvs >= 2){ writeen_actvvs = ON; } else { writeen_actvvs = OFF; }
					capsule1_so8 = runpipeline_1partition(ON, actvvs3, capsule3_so1, buffer0_setof2, capsule0_so2, buffer0_setof4, capsule0_so4, buffer1_setof8, capsule1_so8, sweepparams.currentLOP, sweepparams, WORKBUFFER_SIZE, cutoffs3, itercount_actvvs, _globalparams);
					// savekeyvalues_pipeline_sync
					// savekeyvalues_pipeline_2sync
					savekeyvalues_pipeline_2sync(writeen_actvvs, kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9,kvdram10,kvdram11, offsetsandsizes, buffer1_setof8, actvvstravstate.i_kvs, capsule1_so8.value / VECTOR_SIZE, _globalparams); 
					if(writeen_actvvs == ON){ actvvstravstate.i_kvs += capsule1_so8.value / VECTOR_SIZE; actvvstravstate.i += 2 * capsule1_so8.value; }
					itercount_actvvs += 1;
					#else 
					if(itercount_actvvs >= 2){ writeen_actvvs = ON; } else { writeen_actvvs = OFF; } // k_seq, 1_seq
					capsule0_so8 = runpipeline_1partition(ON, actvvs0, capsule0_so1, buffer0_setof2, capsule0_so2, buffer0_setof4, capsule0_so4, buffer0_setof8, capsule0_so8, sweepparams.currentLOP, sweepparams, WORKBUFFER_SIZE, cutoffs0, itercount_actvvs, _globalparams);
					savekeyvalues_pipeline_sync(writeen_actvvs, kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9,kvdram10,kvdram11, buffer0_setof8, _globalparams.baseoffset_activevertices_kvs + actvvstravstate.i_kvs, capsule0_so8.value / VECTOR_SIZE);
					if(writeen_actvvs == ON){ actvvstravstate.i_kvs += capsule0_so8.value / VECTOR_SIZE; actvvstravstate.i += 2 * capsule0_so8.value; }
					capsule1_so8 = runpipeline_1partition(ON, actvvs1, capsule1_so1, buffer1_setof2, capsule1_so2, buffer1_setof4, capsule1_so4, buffer1_setof8, capsule1_so8, sweepparams.currentLOP, sweepparams, WORKBUFFER_SIZE, cutoffs1, itercount_actvvs, _globalparams);
					savekeyvalues_pipeline_sync(writeen_actvvs, kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9,kvdram10,kvdram11, buffer1_setof8, _globalparams.baseoffset_activevertices_kvs + actvvstravstate.i_kvs, capsule1_so8.value / VECTOR_SIZE);
					if(writeen_actvvs == ON){ actvvstravstate.i_kvs += capsule1_so8.value / VECTOR_SIZE; actvvstravstate.i += 2 * capsule1_so8.value; }
					capsule2_so8 = runpipeline_1partition(ON, actvvs2, capsule2_so1, buffer2_setof2, capsule2_so2, buffer2_setof4, capsule2_so4, buffer2_setof8, capsule2_so8, sweepparams.currentLOP, sweepparams, WORKBUFFER_SIZE, cutoffs2, itercount_actvvs, _globalparams);
					savekeyvalues_pipeline_sync(writeen_actvvs, kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9,kvdram10,kvdram11, buffer2_setof8, _globalparams.baseoffset_activevertices_kvs + actvvstravstate.i_kvs, capsule2_so8.value / VECTOR_SIZE);
					if(writeen_actvvs == ON){ actvvstravstate.i_kvs += capsule2_so8.value / VECTOR_SIZE; actvvstravstate.i += 2 * capsule2_so8.value; }
					capsule3_so8 = runpipeline_1partition(ON, actvvs3, capsule3_so1, buffer3_setof2, capsule3_so2, buffer3_setof4, capsule3_so4, buffer3_setof8, capsule3_so8, sweepparams.currentLOP, sweepparams, WORKBUFFER_SIZE, cutoffs3, itercount_actvvs, _globalparams);
					savekeyvalues_pipeline_sync(writeen_actvvs, kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9,kvdram10,kvdram11, buffer3_setof8, _globalparams.baseoffset_activevertices_kvs + actvvstravstate.i_kvs, capsule3_so8.value / VECTOR_SIZE);
					if(writeen_actvvs == ON){ actvvstravstate.i_kvs += capsule3_so8.value / VECTOR_SIZE; actvvstravstate.i += 2 * capsule3_so8.value; }
					itercount_actvvs += 1;
					#endif 
				}
				
				itercount += 1;
			}
	
			savekeyvalues(enablereduce, kvdram0, destvbuffer, (_globalparams.baseoffset_verticesdata_kvs + (source_partition * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE); // reducechunksz_kvs?
		}
		
		sourcestatsmarker += _globalparams.loadfactorforreduce;
		
		#ifdef _DEBUGMODE_KERNELPRINTS
		actsutilityobj->printglobalvars();
		actsutilityobj->clearglobalvars();
		#endif
	}
			
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->printglobalvars();
	#endif
	#ifdef _DEBUGMODE_STATS
	setkey(kvdram0, PADDEDKVSOURCEDRAMSZ_KVS-1, 3, actsutilityobj->globalstats_getcountvalidkvsreduced());
	setkey(kvdram0, PADDEDKVSOURCEDRAMSZ_KVS-1, 4, actsutilityobj->globalstats_getreducevar1());
	setkey(kvdram0, PADDEDKVSOURCEDRAMSZ_KVS-1, 5, actvvstravstate.i);
	setkey(kvdram1, PADDEDKVSOURCEDRAMSZ_KVS-1, 3, actsutilityobj->globalstats_getcountvalidkvsreduced());
	setkey(kvdram1, PADDEDKVSOURCEDRAMSZ_KVS-1, 4, actsutilityobj->globalstats_getreducevar1());
	setkey(kvdram1, PADDEDKVSOURCEDRAMSZ_KVS-1, 5, actvvstravstate.i);
	setkey(kvdram2, PADDEDKVSOURCEDRAMSZ_KVS-1, 3, actsutilityobj->globalstats_getcountvalidkvsreduced());
	setkey(kvdram2, PADDEDKVSOURCEDRAMSZ_KVS-1, 4, actsutilityobj->globalstats_getreducevar1());
	setkey(kvdram2, PADDEDKVSOURCEDRAMSZ_KVS-1, 5, actvvstravstate.i);
	setkey(kvdram3, PADDEDKVSOURCEDRAMSZ_KVS-1, 3, actsutilityobj->globalstats_getcountvalidkvsreduced());
	setkey(kvdram3, PADDEDKVSOURCEDRAMSZ_KVS-1, 4, actsutilityobj->globalstats_getreducevar1());
	setkey(kvdram3, PADDEDKVSOURCEDRAMSZ_KVS-1, 5, actvvstravstate.i);
	setkey(kvdram4, PADDEDKVSOURCEDRAMSZ_KVS-1, 3, actsutilityobj->globalstats_getcountvalidkvsreduced());
	setkey(kvdram4, PADDEDKVSOURCEDRAMSZ_KVS-1, 4, actsutilityobj->globalstats_getreducevar1());
	setkey(kvdram4, PADDEDKVSOURCEDRAMSZ_KVS-1, 5, actvvstravstate.i);
	setkey(kvdram5, PADDEDKVSOURCEDRAMSZ_KVS-1, 3, actsutilityobj->globalstats_getcountvalidkvsreduced());
	setkey(kvdram5, PADDEDKVSOURCEDRAMSZ_KVS-1, 4, actsutilityobj->globalstats_getreducevar1());
	setkey(kvdram5, PADDEDKVSOURCEDRAMSZ_KVS-1, 5, actvvstravstate.i);
	setkey(kvdram6, PADDEDKVSOURCEDRAMSZ_KVS-1, 3, actsutilityobj->globalstats_getcountvalidkvsreduced());
	setkey(kvdram6, PADDEDKVSOURCEDRAMSZ_KVS-1, 4, actsutilityobj->globalstats_getreducevar1());
	setkey(kvdram6, PADDEDKVSOURCEDRAMSZ_KVS-1, 5, actvvstravstate.i);
	setkey(kvdram7, PADDEDKVSOURCEDRAMSZ_KVS-1, 3, actsutilityobj->globalstats_getcountvalidkvsreduced());
	setkey(kvdram7, PADDEDKVSOURCEDRAMSZ_KVS-1, 4, actsutilityobj->globalstats_getreducevar1());
	setkey(kvdram7, PADDEDKVSOURCEDRAMSZ_KVS-1, 5, actvvstravstate.i);
	setkey(kvdram8, PADDEDKVSOURCEDRAMSZ_KVS-1, 3, actsutilityobj->globalstats_getcountvalidkvsreduced());
	setkey(kvdram8, PADDEDKVSOURCEDRAMSZ_KVS-1, 4, actsutilityobj->globalstats_getreducevar1());
	setkey(kvdram8, PADDEDKVSOURCEDRAMSZ_KVS-1, 5, actvvstravstate.i);
	setkey(kvdram9, PADDEDKVSOURCEDRAMSZ_KVS-1, 3, actsutilityobj->globalstats_getcountvalidkvsreduced());
	setkey(kvdram9, PADDEDKVSOURCEDRAMSZ_KVS-1, 4, actsutilityobj->globalstats_getreducevar1());
	setkey(kvdram9, PADDEDKVSOURCEDRAMSZ_KVS-1, 5, actvvstravstate.i);
	setkey(kvdram10, PADDEDKVSOURCEDRAMSZ_KVS-1, 3, actsutilityobj->globalstats_getcountvalidkvsreduced());
	setkey(kvdram10, PADDEDKVSOURCEDRAMSZ_KVS-1, 4, actsutilityobj->globalstats_getreducevar1());
	setkey(kvdram10, PADDEDKVSOURCEDRAMSZ_KVS-1, 5, actvvstravstate.i);
	setkey(kvdram11, PADDEDKVSOURCEDRAMSZ_KVS-1, 3, actsutilityobj->globalstats_getcountvalidkvsreduced());
	setkey(kvdram11, PADDEDKVSOURCEDRAMSZ_KVS-1, 4, actsutilityobj->globalstats_getreducevar1());
	setkey(kvdram11, PADDEDKVSOURCEDRAMSZ_KVS-1, 5, actvvstravstate.i);
	cout<<"dispatch_reduceonly_parallelsync: number of active vertices for iteration "<<_globalparams.GraphIter + 1<<": (including INVALIDDATAs): "<<actvvstravstate.i<<endl;
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ 
		cout<<"dispatch_reduceonly:: actvvstravstate[i].i: "<<actvvstravstate[i].i<<endl;
		cout<<"dispatch_reduceonly:: actvvstravstate[i].i_kvs: "<<actvvstravstate[i].i_kvs<<endl;
		cout<<"dispatch_reduceonly:: actvvstravstate[i].v: "<<actvvstravstate[i].v<<endl;
		cout<<"dispatch_reduceonly:: actvvstravstate[i].k: "<<actvvstravstate[i].k<<endl;
	}
	#endif
	return actvvstravstate;
}

// top
extern "C" {
void 
	#ifdef SW 
	acts:: 
	#endif

#ifndef MULTIACTSINSTANCES
topkernel(uint512_dt * kvdram){
#pragma HLS INTERFACE m_axi port = kvdram offset = slave bundle = gmem0 // max_read_burst_length=64 max_write_burst_length=64

#pragma HLS INTERFACE s_axilite port = kvdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATA_PACK variable = kvdram

	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printparameters();
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
#endif
 
#if defined(MULTIACTSINSTANCES)
topkernel(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11){
	
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = kvdram6 offset = slave bundle = gmem6
#pragma HLS INTERFACE m_axi port = kvdram7 offset = slave bundle = gmem7
#pragma HLS INTERFACE m_axi port = kvdram8 offset = slave bundle = gmem8
#pragma HLS INTERFACE m_axi port = kvdram9 offset = slave bundle = gmem9
#pragma HLS INTERFACE m_axi port = kvdram10 offset = slave bundle = gmem10
#pragma HLS INTERFACE m_axi port = kvdram11 offset = slave bundle = gmem11
		
#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram3 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram4 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram5 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram6 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram7 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram8 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram9 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram10 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram11 bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = kvdram1
#pragma HLS DATA_PACK variable = kvdram2
#pragma HLS DATA_PACK variable = kvdram3
#pragma HLS DATA_PACK variable = kvdram4
#pragma HLS DATA_PACK variable = kvdram5
#pragma HLS DATA_PACK variable = kvdram6
#pragma HLS DATA_PACK variable = kvdram7
#pragma HLS DATA_PACK variable = kvdram8
#pragma HLS DATA_PACK variable = kvdram9
#pragma HLS DATA_PACK variable = kvdram10
#pragma HLS DATA_PACK variable = kvdram11

	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printparameters();
	#endif 
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	#ifdef _WIDEWORD
 
	cout<<">>> Light weight ACTS (_L2) Launched... size: "<<(unsigned int)(kvdram0[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].range(31, 0))<<endl; 
 
	cout<<">>> Light weight ACTS (_L2) Launched... size: "<<(unsigned int)(kvdram1[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].range(31, 0))<<endl; 
 
	cout<<">>> Light weight ACTS (_L2) Launched... size: "<<(unsigned int)(kvdram2[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].range(31, 0))<<endl; 
 
	cout<<">>> Light weight ACTS (_L2) Launched... size: "<<(unsigned int)(kvdram3[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].range(31, 0))<<endl; 
 
	cout<<">>> Light weight ACTS (_L2) Launched... size: "<<(unsigned int)(kvdram4[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].range(31, 0))<<endl; 
 
	cout<<">>> Light weight ACTS (_L2) Launched... size: "<<(unsigned int)(kvdram5[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].range(31, 0))<<endl; 
 
	cout<<">>> Light weight ACTS (_L2) Launched... size: "<<(unsigned int)(kvdram6[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].range(31, 0))<<endl; 
 
	cout<<">>> Light weight ACTS (_L2) Launched... size: "<<(unsigned int)(kvdram7[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].range(31, 0))<<endl; 
 
	cout<<">>> Light weight ACTS (_L2) Launched... size: "<<(unsigned int)(kvdram8[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].range(31, 0))<<endl; 
 
	cout<<">>> Light weight ACTS (_L2) Launched... size: "<<(unsigned int)(kvdram9[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].range(31, 0))<<endl; 
 
	cout<<">>> Light weight ACTS (_L2) Launched... size: "<<(unsigned int)(kvdram10[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].range(31, 0))<<endl; 
 
	cout<<">>> Light weight ACTS (_L2) Launched... size: "<<(unsigned int)(kvdram11[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].range(31, 0))<<endl; 
	#else
 
	cout<<">>> Light weight ACTS (_L2) Launched... size: "<<kvdram0[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key<<endl; 
 
	cout<<">>> Light weight ACTS (_L2) Launched... size: "<<kvdram1[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key<<endl; 
 
	cout<<">>> Light weight ACTS (_L2) Launched... size: "<<kvdram2[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key<<endl; 
 
	cout<<">>> Light weight ACTS (_L2) Launched... size: "<<kvdram3[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key<<endl; 
 
	cout<<">>> Light weight ACTS (_L2) Launched... size: "<<kvdram4[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key<<endl; 
 
	cout<<">>> Light weight ACTS (_L2) Launched... size: "<<kvdram5[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key<<endl; 
 
	cout<<">>> Light weight ACTS (_L2) Launched... size: "<<kvdram6[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key<<endl; 
 
	cout<<">>> Light weight ACTS (_L2) Launched... size: "<<kvdram7[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key<<endl; 
 
	cout<<">>> Light weight ACTS (_L2) Launched... size: "<<kvdram8[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key<<endl; 
 
	cout<<">>> Light weight ACTS (_L2) Launched... size: "<<kvdram9[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key<<endl; 
 
	cout<<">>> Light weight ACTS (_L2) Launched... size: "<<kvdram10[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key<<endl; 
 
	cout<<">>> Light weight ACTS (_L2) Launched... size: "<<kvdram11[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key<<endl; 
	#endif
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"acts::topkernel:: LOADFACTORFORREDUCE: "<<LOADFACTORFORREDUCE<<endl;
	cout<<"acts::topkernel:: APPLYVERTEXBUFFERSZ: "<<APPLYVERTEXBUFFERSZ<<endl;
	cout<<"acts::topkernel:: VDATAPACKINGFACTOR: "<<VDATAPACKINGFACTOR<<endl;
	// exit(EXIT_SUCCESS);
	#endif 
	
 
	keyvalue_t buffer10[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer10
	keyvalue_t buffer20[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer20
	keyvalue_t buffer30[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer30
 
	keyvalue_t buffer11[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer11
	keyvalue_t buffer21[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer21
	keyvalue_t buffer31[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer31
 
	keyvalue_t buffer12[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer12
	keyvalue_t buffer22[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer22
	keyvalue_t buffer32[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer32
 
	keyvalue_t buffer13[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer13
	keyvalue_t buffer23[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer23
	keyvalue_t buffer33[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer33
 
	keyvalue_t buffer14[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer14
	keyvalue_t buffer24[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer24
	keyvalue_t buffer34[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer34
 
	keyvalue_t buffer15[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer15
	keyvalue_t buffer25[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer25
	keyvalue_t buffer35[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer35
 
	keyvalue_t buffer16[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer16
	keyvalue_t buffer26[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer26
	keyvalue_t buffer36[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer36
 
	keyvalue_t buffer17[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer17
	keyvalue_t buffer27[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer27
	keyvalue_t buffer37[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer37
 
	keyvalue_t buffer18[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer18
	keyvalue_t buffer28[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer28
	keyvalue_t buffer38[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer38
 
	keyvalue_t buffer19[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer19
	keyvalue_t buffer29[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer29
	keyvalue_t buffer39[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer39
 
	keyvalue_t buffer110[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer110
	keyvalue_t buffer210[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer210
	keyvalue_t buffer310[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer310
 
	keyvalue_t buffer111[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer111
	keyvalue_t buffer211[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer211
	keyvalue_t buffer311[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer311
	globalparams_t globalparams[NUMSUBCPUTHREADS];
	#pragma HLS ARRAY_PARTITION variable=globalparams complete
	travstate_t actvvstravstate[NUMSUBCPUTHREADS];
	#pragma HLS ARRAY_PARTITION variable=actvvstravstate complete
	
	globalparams[0] = getglobalparams(kvdram0);
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ globalparams[i] = globalparams[0]; }
	
	unsigned int numGraphIters = globalparams[0].GraphIter;
	
	RUNITERATIONS_LOOP: for(unsigned int GraphIter=0; GraphIter<numGraphIters; GraphIter++){
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<">>> Light weight ACTS: Graph Iteration: "<<GraphIter<<endl;
		#endif 
		
		for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ globalparams[i].GraphIter = GraphIter; }
	
		if(globalparams[0].partitioncommand == ON){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> Light weight ACTS: Partitioning Instance: ";
			#endif
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"0 ";
			#endif
			dispatch_partitiononly(kvdram0, buffer10, buffer20, buffer30, globalparams[0]);
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"1 ";
			#endif
			dispatch_partitiononly(kvdram1, buffer11, buffer21, buffer31, globalparams[1]);
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"2 ";
			#endif
			dispatch_partitiononly(kvdram2, buffer12, buffer22, buffer32, globalparams[2]);
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"3 ";
			#endif
			dispatch_partitiononly(kvdram3, buffer13, buffer23, buffer33, globalparams[3]);
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"4 ";
			#endif
			dispatch_partitiononly(kvdram4, buffer14, buffer24, buffer34, globalparams[4]);
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"5 ";
			#endif
			dispatch_partitiononly(kvdram5, buffer15, buffer25, buffer35, globalparams[5]);
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"6 ";
			#endif
			dispatch_partitiononly(kvdram6, buffer16, buffer26, buffer36, globalparams[6]);
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"7 ";
			#endif
			dispatch_partitiononly(kvdram7, buffer17, buffer27, buffer37, globalparams[7]);
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"8 ";
			#endif
			dispatch_partitiononly(kvdram8, buffer18, buffer28, buffer38, globalparams[8]);
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"9 ";
			#endif
			dispatch_partitiononly(kvdram9, buffer19, buffer29, buffer39, globalparams[9]);
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"10 ";
			#endif
			dispatch_partitiononly(kvdram10, buffer110, buffer210, buffer310, globalparams[10]);
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"11 ";
			#endif
			dispatch_partitiononly(kvdram11, buffer111, buffer211, buffer311, globalparams[11]);
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<endl;
			#endif 
		}
		
		#ifdef REDUCEUPDATES
		if(globalparams[0].reducecommand == ON){
			actvvstravstate[0] = dispatch_reduceonly_parallelsync(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9,kvdram10,kvdram11, 
buffer10,buffer11,buffer12,buffer13,buffer14,buffer15,buffer16,buffer17,buffer18,buffer19,buffer110,buffer111, 
buffer20,buffer21,buffer22,buffer23,buffer24,buffer25,buffer26,buffer27,buffer28,buffer29,buffer210,buffer211, 
buffer30,buffer31,buffer32,buffer33,buffer34,buffer35,buffer36,buffer37,buffer38,buffer39,buffer310,buffer311, 
										actvvstravstate[0], globalparams);
		}
		#endif 
		
		#ifdef LOGKERNELSTATS
		#ifdef _WIDEWORD
		kvdram0[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BASEOFFSET_RETURNVALUES + GraphIter + 1].range(31, 0) = actvvstravstate[0].i;
		#else 
		kvdram0[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BASEOFFSET_RETURNVALUES + GraphIter + 1].data[0].key = actvvstravstate[0].i;	
		#endif 
		#endif 
		
		for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){
		#pragma HLS UNROLL
			globalparams[i].actvvsize = actvvstravstate[0].i;
		}
		if(actvvstravstate[0].i == 0){ break; }
	}
	return;
}
#endif
}




