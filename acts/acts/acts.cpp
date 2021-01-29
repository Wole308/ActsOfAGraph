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
unsigned int
	#ifdef SW 
	acts::
	#endif 
aplus(unsigned int val1, unsigned int val2){
	return val1 + val2;
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
void
	#ifdef SW 
	acts::
	#endif
WRITETO_UINT(unsigned int * data, unsigned int index, unsigned int size, unsigned int value){
	// NB: write(000...00, 0);
	unsigned int tempdata = *data;
	unsigned int A = ((value) << (index));
	unsigned int B = (~GETMASK_UINT((index), (size)));
	unsigned int C = ((tempdata) & (B));
	unsigned int D = (C) | A;
	*data = D;
	
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
	#ifdef _DEBUGMODE_KERNELPRINTS
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
	if(enable == OFF){ return travstate; }
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
	uint512_vec_dt D = getkeyvalues(keyvalues, offset_kvs);
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
	uint512_vec_dt D = getkeyvalues(keyvalues, offset_kvs);
	D.data[v].key = key;
	setkeyvalues(keyvalues, offset_kvs, D);
	return;
}
unsigned int 
	#ifdef SW 
	acts::
	#endif
fillintheblancks(keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], unsigned int index){
	unsigned int rowindex = index / VECTOR2_SIZE;
	unsigned int colindex = index % VECTOR2_SIZE;
	if(index % VECTOR2_SIZE > 0){
		for(unsigned int i=colindex; i<VECTOR2_SIZE; i++){
			if(index % 2 == 0){ buffer[i / 2][rowindex / 2].key = INVALIDDATA; }
			else { buffer[i / 2][rowindex / 2].value = INVALIDDATA; }
			index += 1;
		}
	}
	return index;
}
unsigned int 
	#ifdef SW 
	acts::
	#endif
gethybridthreshold(globalparams_t globalparams){
	return 0; // 4096
}
void
	#ifdef SW 
	acts::
	#endif 
dummywrite(keyvalue_t buffer0[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t buffer1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], unsigned int dummyx, unsigned int dummyy){
	for(unsigned int v=0; v<8; v++){
		for(unsigned int k=0; k<16; k++){
			buffer0[v][k].key = dummyx; // dummy to schedule properly
			buffer0[v][k].value = dummyy; // dummy to schedule properly
		}
	}
	for(unsigned int v=0; v<8; v++){
		for(unsigned int k=0; k<16; k++){
			buffer1[v][k].key = dummyx; // dummy to schedule properly
			buffer1[v][k].value = dummyy; // dummy to schedule properly
		}
	}
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
	// #pragma HLS PIPELINE II=1
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
	// exit(EXIT_SUCCESS);
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
	// #pragma HLS PIPELINE II=1
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
	
	#if defined(ENABLE_PERFECTACCURACY) && defined(_DEBUGMODE_CHECKS2)
	for(unsigned int i=0; i<NUM_PARTITIONS-1; i++){ 
		if(globalcapsule[i].key + globalcapsule[i].value >= globalcapsule[i+1].key){ 
			cout<<"savekeyvalues::globalcapsule 33. ERROR. out of bounds. (globalcapsule["<<i<<"].key("<<globalcapsule[i].key<<") + globalcapsule["<<i<<"].value("<<globalcapsule[i].value<<") >= globalcapsule["<<i+1<<"].key("<<globalcapsule[i+1].key<<")) printing and exiting..."<<endl; 
			actsutilityobj->printkeyvalues("savekeyvalues::globalcapsule 33", globalcapsule, NUM_PARTITIONS); 
			exit(EXIT_FAILURE); 
		}			
		actsutilityobj->checkoutofbounds("savekeyvalues::globalcapsule 33", globalcapsule[i].key + globalcapsule[i].value, globalcapsule[i+1].key, i, NAp, NAp); 
	}
	// actsutilityobj->checkoutofbounds("savekeyvalues::globalcapsule 34", globalcapsule[NUM_PARTITIONS-1].key + globalcapsule[NUM_PARTITIONS-1].value, KVDRAMSZ, NAp, NAp, NAp);
	if(globalcapsule[NUM_PARTITIONS-1].key + globalcapsule[NUM_PARTITIONS-1].value >= KVDRAMSZ){ 
		cout<<"savekeyvalues::globalcapsule 33. ERROR. out of bounds. (globalcapsule["<<NUM_PARTITIONS-1<<"].key("<<globalcapsule[NUM_PARTITIONS-1].key<<") + globalcapsule["<<NUM_PARTITIONS-1<<"].value("<<globalcapsule[NUM_PARTITIONS-1].value<<") >= KVDRAMSZ("<<KVDRAMSZ<<")) printing and exiting..."<<endl; 
		actsutilityobj->printkeyvalues("savekeyvalues::globalcapsule 33", globalcapsule, NUM_PARTITIONS); 
		exit(EXIT_FAILURE); 
	}
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
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<< TIMINGRESULTSCOLOR<<"readkeyvalues:: vertices read: offset: "<<offset_kvs * VECTOR_SIZE<<"-"<<(offset_kvs + size_kvs) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl;
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS2
	if(offset_kvs >= BASEOFFSET_VERTEXPTR_KVS && offset_kvs < BASEOFFSET_VERTEXPTR_KVS + VERTEXPTRSSZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"readkeyvalues:: vertices read: offset: "<<(offset_kvs - BASEOFFSET_VERTEXPTR_KVS) * VECTOR_SIZE<<"-"<<(offset_kvs + size_kvs - BASEOFFSET_VERTEXPTR_KVS) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	if(offset_kvs >= BASEOFFSET_VERTICESDATA_KVS && offset_kvs < BASEOFFSET_VERTICESDATA_KVS + VERTICESDATASZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"readkeyvalues:: vertices read: offset: "<<(offset_kvs - BASEOFFSET_VERTICESDATA_KVS) * VECTOR_SIZE<<"-"<<(offset_kvs + size_kvs - BASEOFFSET_VERTICESDATA_KVS) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
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
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"savekeyvalues:: vertices saved: offset: "<<offset_kvs * VECTOR_SIZE<<"-"<<(offset_kvs + size_kvs) * VECTOR_SIZE<<", number of vertex datas written: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<<endl;
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS2
	if(offset_kvs >= BASEOFFSET_VERTEXPTR_KVS && offset_kvs < BASEOFFSET_VERTEXPTR_KVS + VERTEXPTRSSZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"savekeyvalues:: vertices saved: offset: "<<offset_kvs-BASEOFFSET_VERTEXPTR_KVS * VECTOR_SIZE<<"-"<<((offset_kvs + size_kvs)-BASEOFFSET_VERTEXPTR_KVS) * VECTOR_SIZE<<", number of vertex datas written: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	if(offset_kvs >= BASEOFFSET_VERTICESDATA_KVS && offset_kvs < BASEOFFSET_VERTICESDATA_KVS + VERTICESDATASZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"savekeyvalues:: vertices saved: offset: "<<(offset_kvs-BASEOFFSET_VERTICESDATA_KVS) * VECTOR_SIZE<<"-"<<((offset_kvs-BASEOFFSET_VERTICESDATA_KVS) + size_kvs) * VECTOR_SIZE<<", number of vertex datas written: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
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

void 
	#ifdef SW 
	acts::
	#endif 
replicatedata_syn(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE] ,keyvalue_t destbuffer0[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer3[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer5[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer6[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer7[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer8[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer9[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer10[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer11[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE]){
	REPLICATEDATA2_LOOP: for(unsigned int i=0; i<PADDEDDESTBUFFER_SIZE; i++){
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			destbuffer1[v][i] = sourcebuffer[v][i];
			destbuffer2[v][i] = sourcebuffer[v][i];
			destbuffer3[v][i] = sourcebuffer[v][i];
			destbuffer4[v][i] = sourcebuffer[v][i];
			destbuffer5[v][i] = sourcebuffer[v][i];
			destbuffer6[v][i] = sourcebuffer[v][i];
			destbuffer7[v][i] = sourcebuffer[v][i];
			destbuffer8[v][i] = sourcebuffer[v][i];
			destbuffer9[v][i] = sourcebuffer[v][i];
			destbuffer10[v][i] = sourcebuffer[v][i];
			destbuffer11[v][i] = sourcebuffer[v][i];
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
	#pragma HLS PIPELINE II=3	
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
			#ifdef _DEBUGMODE_CHECKS2
			if(actsutilityobj->globalstats_getcounterrorsinreduce() > 40000){ cout<<"too many ("<<actsutilityobj->globalstats_getcounterrorsinreduce()<<") reduce tolerable errors. EXITING"<<endl; exit(EXIT_FAILURE); } 
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
			#ifdef _DEBUGMODE_CHECKS2
			if(actsutilityobj->globalstats_getcounterrorsinreduce() > 40000){ cout<<"too many ("<<actsutilityobj->globalstats_getcounterrorsinreduce()<<") reduce tolerable errors. EXITING"<<endl; exit(EXIT_FAILURE); } 
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
			#ifdef _DEBUGMODE_CHECKS2
			if(actsutilityobj->globalstats_getcounterrorsinreduce() > 40000){ cout<<"too many ("<<actsutilityobj->globalstats_getcounterrorsinreduce()<<") reduce tolerable errors. EXITING"<<endl; exit(EXIT_FAILURE); } 
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
			#ifdef _DEBUGMODE_CHECKS2
			if(actsutilityobj->globalstats_getcounterrorsinreduce() > 40000){ cout<<"too many ("<<actsutilityobj->globalstats_getcounterrorsinreduce()<<") reduce tolerable errors. EXITING"<<endl; exit(EXIT_FAILURE); } 
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
			#ifdef _DEBUGMODE_CHECKS2
			if(actsutilityobj->globalstats_getcounterrorsinreduce() > 40000){ cout<<"too many ("<<actsutilityobj->globalstats_getcounterrorsinreduce()<<") reduce tolerable errors. EXITING"<<endl; exit(EXIT_FAILURE); } 
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
			#ifdef _DEBUGMODE_CHECKS2
			if(actsutilityobj->globalstats_getcounterrorsinreduce() > 40000){ cout<<"too many ("<<actsutilityobj->globalstats_getcounterrorsinreduce()<<") reduce tolerable errors. EXITING"<<endl; exit(EXIT_FAILURE); } 
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
			#ifdef _DEBUGMODE_CHECKS2
			if(actsutilityobj->globalstats_getcounterrorsinreduce() > 40000){ cout<<"too many ("<<actsutilityobj->globalstats_getcounterrorsinreduce()<<") reduce tolerable errors. EXITING"<<endl; exit(EXIT_FAILURE); } 
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
			#ifdef _DEBUGMODE_CHECKS2
			if(actsutilityobj->globalstats_getcounterrorsinreduce() > 40000){ cout<<"too many ("<<actsutilityobj->globalstats_getcounterrorsinreduce()<<") reduce tolerable errors. EXITING"<<endl; exit(EXIT_FAILURE); } 
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
		unsigned int partitionupperlimit1 = streetaddr1 * (1 << globalparams.applyvertexbuffersz_pow); // CRITICAL VHLS CHECKME. (dataset dependent).
		unsigned int partitionupperlimit2 = streetaddr2 * (1 << globalparams.applyvertexbuffersz_pow); // CRITICAL VHLS CHECKME. (dataset dependent).
		unsigned int partitionupperlimit3 = streetaddr3 * (1 << globalparams.applyvertexbuffersz_pow); // CRITICAL VHLS CHECKME. (dataset dependent).
		unsigned int partitionupperlimit4 = streetaddr4 * (1 << globalparams.applyvertexbuffersz_pow); // CRITICAL VHLS CHECKME. (dataset dependent).
		unsigned int partitionupperlimit5 = streetaddr5 * (1 << globalparams.applyvertexbuffersz_pow); // CRITICAL VHLS CHECKME. (dataset dependent).
		unsigned int partitionupperlimit6 = streetaddr6 * (1 << globalparams.applyvertexbuffersz_pow); // CRITICAL VHLS CHECKME. (dataset dependent).
		unsigned int partitionupperlimit7 = streetaddr7 * (1 << globalparams.applyvertexbuffersz_pow); // CRITICAL VHLS CHECKME. (dataset dependent).
		
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
			
			#ifdef _DEBUGMODE_STATS //  _DEBUGMODE_CHECKS2
			if(en0 == ON && en20 == ON){
				if(locs0 >= globalparams.applyvertexbuffersz){ actsutilityobj->globalstats_counterrorsinreduce(1); }
				#ifdef ENABLE_VOICEOUTREDUCEERRORS
				if(locs0 >= globalparams.applyvertexbuffersz){ cout<<"ERROR SEEN @ reduce_bfs:: i: "<<i<<", locs0: "<<locs0<<", keys0: "<<keys0<<", keyvalue0.key: "<<keyvalue0.key<<", keyvalue0.value: "<<keyvalue0.value<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; }
				#endif 
				#ifdef ENABLE_PERFECTACCURACY
				if(locs0 >= globalparams.applyvertexbuffersz){ if(actsutilityobj->globalstats_getcounterrorsinreduce() > 0){ cout<<"too many ("<<actsutilityobj->globalstats_getcounterrorsinreduce()<<") reduce_bfs errors. EXITING"<<endl; exit(EXIT_FAILURE); } }
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

void 
	#ifdef SW 
	acts::
	#endif
reduce_sssp(bool_type enable1, bool_type enable2, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], sweepparams_t sweepparams, unsigned int GraphIter, unsigned int GraphAlgo, travstate_t travstate, globalparams_t globalparams){
	if(enable1 == OFF || enable2 == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE;
	buffer_type chunk_size = getchunksize_kvs(SRCBUFFER_SIZE, travstate, 0);
	unsigned int upperlimit = sweepparams.upperlimit;
	unsigned int upperpartition = sweepparams.upperpartition;
	
	REDUCE_LOOP: for(buffer_type i=0; i<chunk_size; i++){
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
		
		vertex_t loc0 = keyvalue0.key - upperlimit;
		vertex_t loc1 = keyvalue1.key - upperlimit;
		vertex_t loc2 = keyvalue2.key - upperlimit;
		vertex_t loc3 = keyvalue3.key - upperlimit;
		vertex_t loc4 = keyvalue4.key - upperlimit;
		vertex_t loc5 = keyvalue5.key - upperlimit;
		vertex_t loc6 = keyvalue6.key - upperlimit;
		vertex_t loc7 = keyvalue7.key - upperlimit;
		
		#ifdef _DEBUGMODE_KERNELPRINTS
		if(keyvalue0.key != INVALIDDATA && keyvalue0.value != INVALIDDATA){ cout<<"REDUCE_SSSP SEEN @ reduce:: i: "<<i<<", loc0: "<<loc0<<", keyvalue0.key: "<<keyvalue0.key<<", keyvalue0.value: "<<keyvalue0.value<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; }
		if(keyvalue1.key != INVALIDDATA && keyvalue1.value != INVALIDDATA){ cout<<"REDUCE_SSSP SEEN @ reduce:: i: "<<i<<", loc1: "<<loc1<<", keyvalue1.key: "<<keyvalue1.key<<", keyvalue1.value: "<<keyvalue1.value<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; }
		if(keyvalue2.key != INVALIDDATA && keyvalue2.value != INVALIDDATA){ cout<<"REDUCE_SSSP SEEN @ reduce:: i: "<<i<<", loc2: "<<loc2<<", keyvalue2.key: "<<keyvalue2.key<<", keyvalue2.value: "<<keyvalue2.value<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; }
		if(keyvalue3.key != INVALIDDATA && keyvalue3.value != INVALIDDATA){ cout<<"REDUCE_SSSP SEEN @ reduce:: i: "<<i<<", loc3: "<<loc3<<", keyvalue3.key: "<<keyvalue3.key<<", keyvalue3.value: "<<keyvalue3.value<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; }
		if(keyvalue4.key != INVALIDDATA && keyvalue4.value != INVALIDDATA){ cout<<"REDUCE_SSSP SEEN @ reduce:: i: "<<i<<", loc4: "<<loc4<<", keyvalue4.key: "<<keyvalue4.key<<", keyvalue4.value: "<<keyvalue4.value<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; }
		if(keyvalue5.key != INVALIDDATA && keyvalue5.value != INVALIDDATA){ cout<<"REDUCE_SSSP SEEN @ reduce:: i: "<<i<<", loc5: "<<loc5<<", keyvalue5.key: "<<keyvalue5.key<<", keyvalue5.value: "<<keyvalue5.value<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; }
		if(keyvalue6.key != INVALIDDATA && keyvalue6.value != INVALIDDATA){ cout<<"REDUCE_SSSP SEEN @ reduce:: i: "<<i<<", loc6: "<<loc6<<", keyvalue6.key: "<<keyvalue6.key<<", keyvalue6.value: "<<keyvalue6.value<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; }
		if(keyvalue7.key != INVALIDDATA && keyvalue7.value != INVALIDDATA){ cout<<"REDUCE_SSSP SEEN @ reduce:: i: "<<i<<", loc7: "<<loc7<<", keyvalue7.key: "<<keyvalue7.key<<", keyvalue7.value: "<<keyvalue7.value<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; }
		#endif 
		
		if(loc0 >= globalparams.applyvertexbuffersz && keyvalue0.key != INVALIDDATA && keyvalue0.value != INVALIDDATA){
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_counterrorsinreduce(1);
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTREDUCEERRORS
			cout<<"ERROR SEEN @ reduce:: i: "<<i<<", loc0: "<<loc0<<", keyvalue0.key: "<<keyvalue0.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			if(actsutilityobj->globalstats_getcounterrorsinreduce() > 0){ cout<<"too many ("<<actsutilityobj->globalstats_getcounterrorsinreduce()<<") reduce errors. EXITING"<<endl; exit(EXIT_FAILURE); } // REMOVEME. // FIXME. ENSURE PERFECTION.
			#endif 
			#ifdef _DEBUGMODE_CHECKS2
			if(actsutilityobj->globalstats_getcounterrorsinreduce() > 40000){ cout<<"too many ("<<actsutilityobj->globalstats_getcounterrorsinreduce()<<") reduce tolerable errors. EXITING"<<endl; exit(EXIT_FAILURE); } 
			#endif 
			#endif
			loc0 = 0;
		} // REMOVEME.
		if(loc1 >= globalparams.applyvertexbuffersz && keyvalue1.key != INVALIDDATA && keyvalue1.value != INVALIDDATA){
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_counterrorsinreduce(1);
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTREDUCEERRORS
			cout<<"ERROR SEEN @ reduce:: i: "<<i<<", loc1: "<<loc1<<", keyvalue1.key: "<<keyvalue1.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			if(actsutilityobj->globalstats_getcounterrorsinreduce() > 0){ cout<<"too many ("<<actsutilityobj->globalstats_getcounterrorsinreduce()<<") reduce errors. EXITING"<<endl; exit(EXIT_FAILURE); } // REMOVEME. // FIXME. ENSURE PERFECTION.
			#endif 
			#ifdef _DEBUGMODE_CHECKS2
			if(actsutilityobj->globalstats_getcounterrorsinreduce() > 40000){ cout<<"too many ("<<actsutilityobj->globalstats_getcounterrorsinreduce()<<") reduce tolerable errors. EXITING"<<endl; exit(EXIT_FAILURE); } 
			#endif 
			#endif
			loc1 = 0;
		} // REMOVEME.
		if(loc2 >= globalparams.applyvertexbuffersz && keyvalue2.key != INVALIDDATA && keyvalue2.value != INVALIDDATA){
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_counterrorsinreduce(1);
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTREDUCEERRORS
			cout<<"ERROR SEEN @ reduce:: i: "<<i<<", loc2: "<<loc2<<", keyvalue2.key: "<<keyvalue2.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			if(actsutilityobj->globalstats_getcounterrorsinreduce() > 0){ cout<<"too many ("<<actsutilityobj->globalstats_getcounterrorsinreduce()<<") reduce errors. EXITING"<<endl; exit(EXIT_FAILURE); } // REMOVEME. // FIXME. ENSURE PERFECTION.
			#endif 
			#ifdef _DEBUGMODE_CHECKS2
			if(actsutilityobj->globalstats_getcounterrorsinreduce() > 40000){ cout<<"too many ("<<actsutilityobj->globalstats_getcounterrorsinreduce()<<") reduce tolerable errors. EXITING"<<endl; exit(EXIT_FAILURE); } 
			#endif 
			#endif
			loc2 = 0;
		} // REMOVEME.
		if(loc3 >= globalparams.applyvertexbuffersz && keyvalue3.key != INVALIDDATA && keyvalue3.value != INVALIDDATA){
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_counterrorsinreduce(1);
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTREDUCEERRORS
			cout<<"ERROR SEEN @ reduce:: i: "<<i<<", loc3: "<<loc3<<", keyvalue3.key: "<<keyvalue3.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			if(actsutilityobj->globalstats_getcounterrorsinreduce() > 0){ cout<<"too many ("<<actsutilityobj->globalstats_getcounterrorsinreduce()<<") reduce errors. EXITING"<<endl; exit(EXIT_FAILURE); } // REMOVEME. // FIXME. ENSURE PERFECTION.
			#endif 
			#ifdef _DEBUGMODE_CHECKS2
			if(actsutilityobj->globalstats_getcounterrorsinreduce() > 40000){ cout<<"too many ("<<actsutilityobj->globalstats_getcounterrorsinreduce()<<") reduce tolerable errors. EXITING"<<endl; exit(EXIT_FAILURE); } 
			#endif 
			#endif
			loc3 = 0;
		} // REMOVEME.
		if(loc4 >= globalparams.applyvertexbuffersz && keyvalue4.key != INVALIDDATA && keyvalue4.value != INVALIDDATA){
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_counterrorsinreduce(1);
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTREDUCEERRORS
			cout<<"ERROR SEEN @ reduce:: i: "<<i<<", loc4: "<<loc4<<", keyvalue4.key: "<<keyvalue4.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			if(actsutilityobj->globalstats_getcounterrorsinreduce() > 0){ cout<<"too many ("<<actsutilityobj->globalstats_getcounterrorsinreduce()<<") reduce errors. EXITING"<<endl; exit(EXIT_FAILURE); } // REMOVEME. // FIXME. ENSURE PERFECTION.
			#endif 
			#ifdef _DEBUGMODE_CHECKS2
			if(actsutilityobj->globalstats_getcounterrorsinreduce() > 40000){ cout<<"too many ("<<actsutilityobj->globalstats_getcounterrorsinreduce()<<") reduce tolerable errors. EXITING"<<endl; exit(EXIT_FAILURE); } 
			#endif 
			#endif
			loc4 = 0;
		} // REMOVEME.
		if(loc5 >= globalparams.applyvertexbuffersz && keyvalue5.key != INVALIDDATA && keyvalue5.value != INVALIDDATA){
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_counterrorsinreduce(1);
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTREDUCEERRORS
			cout<<"ERROR SEEN @ reduce:: i: "<<i<<", loc5: "<<loc5<<", keyvalue5.key: "<<keyvalue5.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			if(actsutilityobj->globalstats_getcounterrorsinreduce() > 0){ cout<<"too many ("<<actsutilityobj->globalstats_getcounterrorsinreduce()<<") reduce errors. EXITING"<<endl; exit(EXIT_FAILURE); } // REMOVEME. // FIXME. ENSURE PERFECTION.
			#endif 
			#ifdef _DEBUGMODE_CHECKS2
			if(actsutilityobj->globalstats_getcounterrorsinreduce() > 40000){ cout<<"too many ("<<actsutilityobj->globalstats_getcounterrorsinreduce()<<") reduce tolerable errors. EXITING"<<endl; exit(EXIT_FAILURE); } 
			#endif 
			#endif
			loc5 = 0;
		} // REMOVEME.
		if(loc6 >= globalparams.applyvertexbuffersz && keyvalue6.key != INVALIDDATA && keyvalue6.value != INVALIDDATA){
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_counterrorsinreduce(1);
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTREDUCEERRORS
			cout<<"ERROR SEEN @ reduce:: i: "<<i<<", loc6: "<<loc6<<", keyvalue6.key: "<<keyvalue6.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			if(actsutilityobj->globalstats_getcounterrorsinreduce() > 0){ cout<<"too many ("<<actsutilityobj->globalstats_getcounterrorsinreduce()<<") reduce errors. EXITING"<<endl; exit(EXIT_FAILURE); } // REMOVEME. // FIXME. ENSURE PERFECTION.
			#endif 
			#ifdef _DEBUGMODE_CHECKS2
			if(actsutilityobj->globalstats_getcounterrorsinreduce() > 40000){ cout<<"too many ("<<actsutilityobj->globalstats_getcounterrorsinreduce()<<") reduce tolerable errors. EXITING"<<endl; exit(EXIT_FAILURE); } 
			#endif 
			#endif
			loc6 = 0;
		} // REMOVEME.
		if(loc7 >= globalparams.applyvertexbuffersz && keyvalue7.key != INVALIDDATA && keyvalue7.value != INVALIDDATA){
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_counterrorsinreduce(1);
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTREDUCEERRORS
			cout<<"ERROR SEEN @ reduce:: i: "<<i<<", loc7: "<<loc7<<", keyvalue7.key: "<<keyvalue7.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			if(actsutilityobj->globalstats_getcounterrorsinreduce() > 0){ cout<<"too many ("<<actsutilityobj->globalstats_getcounterrorsinreduce()<<") reduce errors. EXITING"<<endl; exit(EXIT_FAILURE); } // REMOVEME. // FIXME. ENSURE PERFECTION.
			#endif 
			#ifdef _DEBUGMODE_CHECKS2
			if(actsutilityobj->globalstats_getcounterrorsinreduce() > 40000){ cout<<"too many ("<<actsutilityobj->globalstats_getcounterrorsinreduce()<<") reduce tolerable errors. EXITING"<<endl; exit(EXIT_FAILURE); } 
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
		
		value_t temp0_code = READFROM_UINT(temp0, 30, 2);
		value_t temp0_val = READFROM_UINT(temp0, 0, 30);
		value_t temp1_code = READFROM_UINT(temp1, 30, 2);
		value_t temp1_val = READFROM_UINT(temp1, 0, 30);
		value_t temp2_code = READFROM_UINT(temp2, 30, 2);
		value_t temp2_val = READFROM_UINT(temp2, 0, 30);
		value_t temp3_code = READFROM_UINT(temp3, 30, 2);
		value_t temp3_val = READFROM_UINT(temp3, 0, 30);
		value_t temp4_code = READFROM_UINT(temp4, 30, 2);
		value_t temp4_val = READFROM_UINT(temp4, 0, 30);
		value_t temp5_code = READFROM_UINT(temp5, 30, 2);
		value_t temp5_val = READFROM_UINT(temp5, 0, 30);
		value_t temp6_code = READFROM_UINT(temp6, 30, 2);
		value_t temp6_val = READFROM_UINT(temp6, 0, 30);
		value_t temp7_code = READFROM_UINT(temp7, 30, 2);
		value_t temp7_val = READFROM_UINT(temp7, 0, 30);
		
		value_t rettemp0 = reducefunc(temp0_val, keyvalue0.value, GraphIter, GraphAlgo);
		value_t rettemp1 = reducefunc(temp1_val, keyvalue1.value, GraphIter, GraphAlgo);
		value_t rettemp2 = reducefunc(temp2_val, keyvalue2.value, GraphIter, GraphAlgo);
		value_t rettemp3 = reducefunc(temp3_val, keyvalue3.value, GraphIter, GraphAlgo);
		value_t rettemp4 = reducefunc(temp4_val, keyvalue4.value, GraphIter, GraphAlgo);
		value_t rettemp5 = reducefunc(temp5_val, keyvalue5.value, GraphIter, GraphAlgo);
		value_t rettemp6 = reducefunc(temp6_val, keyvalue6.value, GraphIter, GraphAlgo);
		value_t rettemp7 = reducefunc(temp7_val, keyvalue7.value, GraphIter, GraphAlgo);

		if(keyvalue0.value < temp0_val){ WRITETO_UINT(&rettemp0, 30, 2, ACTIVEINNEXTITERATION); }
		if(keyvalue1.value < temp1_val){ WRITETO_UINT(&rettemp1, 30, 2, ACTIVEINNEXTITERATION); }
		if(keyvalue2.value < temp2_val){ WRITETO_UINT(&rettemp2, 30, 2, ACTIVEINNEXTITERATION); }
		if(keyvalue3.value < temp3_val){ WRITETO_UINT(&rettemp3, 30, 2, ACTIVEINNEXTITERATION); }
		if(keyvalue4.value < temp4_val){ WRITETO_UINT(&rettemp4, 30, 2, ACTIVEINNEXTITERATION); }
		if(keyvalue5.value < temp5_val){ WRITETO_UINT(&rettemp5, 30, 2, ACTIVEINNEXTITERATION); }
		if(keyvalue6.value < temp6_val){ WRITETO_UINT(&rettemp6, 30, 2, ACTIVEINNEXTITERATION); }
		if(keyvalue7.value < temp7_val){ WRITETO_UINT(&rettemp7, 30, 2, ACTIVEINNEXTITERATION); }
		
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

void 
	#ifdef SW 
	acts::
	#endif
unifydata_pr_parallelsyn(bool_type enable, keyvalue_t sourcebuffer0[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer3[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer5[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer6[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer7[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer8[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer9[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer10[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer11[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
					buffer_type destoffset, buffer_type size, sweepparams_t sweepparams, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount = (APPLYVERTEXBUFFERSZ / VDATAPACKINGFACTOR) / 2;
	
	buffer_type dest_v = 0;
	buffer_type dest_i = 0;
	buffer_type destoffset_kvs = destoffset / VECTOR_SIZE;
	
	UNIFYDATA_LOOP1: for(buffer_type i=0; i<size; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE II=2
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("unifydata_pr_parallelsyn.i", i, PADDEDDESTBUFFER_SIZE, destoffset, destoffset_kvs, size);
		#endif
		
		for(unsigned int k=0; k<2; k++){
			keyvalue_t vdata00 = sourcebuffer0[0][i];
			keyvalue_t vdata01 = sourcebuffer0[1][i];
			keyvalue_t vdata02 = sourcebuffer0[2][i];
			keyvalue_t vdata03 = sourcebuffer0[3][i];
			keyvalue_t vdata04 = sourcebuffer0[4][i];
			keyvalue_t vdata05 = sourcebuffer0[5][i];
			keyvalue_t vdata06 = sourcebuffer0[6][i];
			keyvalue_t vdata07 = sourcebuffer0[7][i];
			keyvalue_t vdata10 = sourcebuffer1[0][i];
			keyvalue_t vdata11 = sourcebuffer1[1][i];
			keyvalue_t vdata12 = sourcebuffer1[2][i];
			keyvalue_t vdata13 = sourcebuffer1[3][i];
			keyvalue_t vdata14 = sourcebuffer1[4][i];
			keyvalue_t vdata15 = sourcebuffer1[5][i];
			keyvalue_t vdata16 = sourcebuffer1[6][i];
			keyvalue_t vdata17 = sourcebuffer1[7][i];
			keyvalue_t vdata20 = sourcebuffer2[0][i];
			keyvalue_t vdata21 = sourcebuffer2[1][i];
			keyvalue_t vdata22 = sourcebuffer2[2][i];
			keyvalue_t vdata23 = sourcebuffer2[3][i];
			keyvalue_t vdata24 = sourcebuffer2[4][i];
			keyvalue_t vdata25 = sourcebuffer2[5][i];
			keyvalue_t vdata26 = sourcebuffer2[6][i];
			keyvalue_t vdata27 = sourcebuffer2[7][i];
			keyvalue_t vdata30 = sourcebuffer3[0][i];
			keyvalue_t vdata31 = sourcebuffer3[1][i];
			keyvalue_t vdata32 = sourcebuffer3[2][i];
			keyvalue_t vdata33 = sourcebuffer3[3][i];
			keyvalue_t vdata34 = sourcebuffer3[4][i];
			keyvalue_t vdata35 = sourcebuffer3[5][i];
			keyvalue_t vdata36 = sourcebuffer3[6][i];
			keyvalue_t vdata37 = sourcebuffer3[7][i];
			keyvalue_t vdata40 = sourcebuffer4[0][i];
			keyvalue_t vdata41 = sourcebuffer4[1][i];
			keyvalue_t vdata42 = sourcebuffer4[2][i];
			keyvalue_t vdata43 = sourcebuffer4[3][i];
			keyvalue_t vdata44 = sourcebuffer4[4][i];
			keyvalue_t vdata45 = sourcebuffer4[5][i];
			keyvalue_t vdata46 = sourcebuffer4[6][i];
			keyvalue_t vdata47 = sourcebuffer4[7][i];
			keyvalue_t vdata50 = sourcebuffer5[0][i];
			keyvalue_t vdata51 = sourcebuffer5[1][i];
			keyvalue_t vdata52 = sourcebuffer5[2][i];
			keyvalue_t vdata53 = sourcebuffer5[3][i];
			keyvalue_t vdata54 = sourcebuffer5[4][i];
			keyvalue_t vdata55 = sourcebuffer5[5][i];
			keyvalue_t vdata56 = sourcebuffer5[6][i];
			keyvalue_t vdata57 = sourcebuffer5[7][i];
			keyvalue_t vdata60 = sourcebuffer6[0][i];
			keyvalue_t vdata61 = sourcebuffer6[1][i];
			keyvalue_t vdata62 = sourcebuffer6[2][i];
			keyvalue_t vdata63 = sourcebuffer6[3][i];
			keyvalue_t vdata64 = sourcebuffer6[4][i];
			keyvalue_t vdata65 = sourcebuffer6[5][i];
			keyvalue_t vdata66 = sourcebuffer6[6][i];
			keyvalue_t vdata67 = sourcebuffer6[7][i];
			keyvalue_t vdata70 = sourcebuffer7[0][i];
			keyvalue_t vdata71 = sourcebuffer7[1][i];
			keyvalue_t vdata72 = sourcebuffer7[2][i];
			keyvalue_t vdata73 = sourcebuffer7[3][i];
			keyvalue_t vdata74 = sourcebuffer7[4][i];
			keyvalue_t vdata75 = sourcebuffer7[5][i];
			keyvalue_t vdata76 = sourcebuffer7[6][i];
			keyvalue_t vdata77 = sourcebuffer7[7][i];
			keyvalue_t vdata80 = sourcebuffer8[0][i];
			keyvalue_t vdata81 = sourcebuffer8[1][i];
			keyvalue_t vdata82 = sourcebuffer8[2][i];
			keyvalue_t vdata83 = sourcebuffer8[3][i];
			keyvalue_t vdata84 = sourcebuffer8[4][i];
			keyvalue_t vdata85 = sourcebuffer8[5][i];
			keyvalue_t vdata86 = sourcebuffer8[6][i];
			keyvalue_t vdata87 = sourcebuffer8[7][i];
			keyvalue_t vdata90 = sourcebuffer9[0][i];
			keyvalue_t vdata91 = sourcebuffer9[1][i];
			keyvalue_t vdata92 = sourcebuffer9[2][i];
			keyvalue_t vdata93 = sourcebuffer9[3][i];
			keyvalue_t vdata94 = sourcebuffer9[4][i];
			keyvalue_t vdata95 = sourcebuffer9[5][i];
			keyvalue_t vdata96 = sourcebuffer9[6][i];
			keyvalue_t vdata97 = sourcebuffer9[7][i];
			keyvalue_t vdata100 = sourcebuffer10[0][i];
			keyvalue_t vdata101 = sourcebuffer10[1][i];
			keyvalue_t vdata102 = sourcebuffer10[2][i];
			keyvalue_t vdata103 = sourcebuffer10[3][i];
			keyvalue_t vdata104 = sourcebuffer10[4][i];
			keyvalue_t vdata105 = sourcebuffer10[5][i];
			keyvalue_t vdata106 = sourcebuffer10[6][i];
			keyvalue_t vdata107 = sourcebuffer10[7][i];
			keyvalue_t vdata110 = sourcebuffer11[0][i];
			keyvalue_t vdata111 = sourcebuffer11[1][i];
			keyvalue_t vdata112 = sourcebuffer11[2][i];
			keyvalue_t vdata113 = sourcebuffer11[3][i];
			keyvalue_t vdata114 = sourcebuffer11[4][i];
			keyvalue_t vdata115 = sourcebuffer11[5][i];
			keyvalue_t vdata116 = sourcebuffer11[6][i];
			keyvalue_t vdata117 = sourcebuffer11[7][i];
			
			value_t temp00_code;
			value_t temp00_val;
			value_t temp01_code;
			value_t temp01_val;
			value_t temp02_code;
			value_t temp02_val;
			value_t temp03_code;
			value_t temp03_val;
			value_t temp04_code;
			value_t temp04_val;
			value_t temp05_code;
			value_t temp05_val;
			value_t temp06_code;
			value_t temp06_val;
			value_t temp07_code;
			value_t temp07_val;
			value_t temp10_code;
			value_t temp10_val;
			value_t temp11_code;
			value_t temp11_val;
			value_t temp12_code;
			value_t temp12_val;
			value_t temp13_code;
			value_t temp13_val;
			value_t temp14_code;
			value_t temp14_val;
			value_t temp15_code;
			value_t temp15_val;
			value_t temp16_code;
			value_t temp16_val;
			value_t temp17_code;
			value_t temp17_val;
			value_t temp20_code;
			value_t temp20_val;
			value_t temp21_code;
			value_t temp21_val;
			value_t temp22_code;
			value_t temp22_val;
			value_t temp23_code;
			value_t temp23_val;
			value_t temp24_code;
			value_t temp24_val;
			value_t temp25_code;
			value_t temp25_val;
			value_t temp26_code;
			value_t temp26_val;
			value_t temp27_code;
			value_t temp27_val;
			value_t temp30_code;
			value_t temp30_val;
			value_t temp31_code;
			value_t temp31_val;
			value_t temp32_code;
			value_t temp32_val;
			value_t temp33_code;
			value_t temp33_val;
			value_t temp34_code;
			value_t temp34_val;
			value_t temp35_code;
			value_t temp35_val;
			value_t temp36_code;
			value_t temp36_val;
			value_t temp37_code;
			value_t temp37_val;
			value_t temp40_code;
			value_t temp40_val;
			value_t temp41_code;
			value_t temp41_val;
			value_t temp42_code;
			value_t temp42_val;
			value_t temp43_code;
			value_t temp43_val;
			value_t temp44_code;
			value_t temp44_val;
			value_t temp45_code;
			value_t temp45_val;
			value_t temp46_code;
			value_t temp46_val;
			value_t temp47_code;
			value_t temp47_val;
			value_t temp50_code;
			value_t temp50_val;
			value_t temp51_code;
			value_t temp51_val;
			value_t temp52_code;
			value_t temp52_val;
			value_t temp53_code;
			value_t temp53_val;
			value_t temp54_code;
			value_t temp54_val;
			value_t temp55_code;
			value_t temp55_val;
			value_t temp56_code;
			value_t temp56_val;
			value_t temp57_code;
			value_t temp57_val;
			value_t temp60_code;
			value_t temp60_val;
			value_t temp61_code;
			value_t temp61_val;
			value_t temp62_code;
			value_t temp62_val;
			value_t temp63_code;
			value_t temp63_val;
			value_t temp64_code;
			value_t temp64_val;
			value_t temp65_code;
			value_t temp65_val;
			value_t temp66_code;
			value_t temp66_val;
			value_t temp67_code;
			value_t temp67_val;
			value_t temp70_code;
			value_t temp70_val;
			value_t temp71_code;
			value_t temp71_val;
			value_t temp72_code;
			value_t temp72_val;
			value_t temp73_code;
			value_t temp73_val;
			value_t temp74_code;
			value_t temp74_val;
			value_t temp75_code;
			value_t temp75_val;
			value_t temp76_code;
			value_t temp76_val;
			value_t temp77_code;
			value_t temp77_val;
			value_t temp80_code;
			value_t temp80_val;
			value_t temp81_code;
			value_t temp81_val;
			value_t temp82_code;
			value_t temp82_val;
			value_t temp83_code;
			value_t temp83_val;
			value_t temp84_code;
			value_t temp84_val;
			value_t temp85_code;
			value_t temp85_val;
			value_t temp86_code;
			value_t temp86_val;
			value_t temp87_code;
			value_t temp87_val;
			value_t temp90_code;
			value_t temp90_val;
			value_t temp91_code;
			value_t temp91_val;
			value_t temp92_code;
			value_t temp92_val;
			value_t temp93_code;
			value_t temp93_val;
			value_t temp94_code;
			value_t temp94_val;
			value_t temp95_code;
			value_t temp95_val;
			value_t temp96_code;
			value_t temp96_val;
			value_t temp97_code;
			value_t temp97_val;
			value_t temp100_code;
			value_t temp100_val;
			value_t temp101_code;
			value_t temp101_val;
			value_t temp102_code;
			value_t temp102_val;
			value_t temp103_code;
			value_t temp103_val;
			value_t temp104_code;
			value_t temp104_val;
			value_t temp105_code;
			value_t temp105_val;
			value_t temp106_code;
			value_t temp106_val;
			value_t temp107_code;
			value_t temp107_val;
			value_t temp110_code;
			value_t temp110_val;
			value_t temp111_code;
			value_t temp111_val;
			value_t temp112_code;
			value_t temp112_val;
			value_t temp113_code;
			value_t temp113_val;
			value_t temp114_code;
			value_t temp114_val;
			value_t temp115_code;
			value_t temp115_val;
			value_t temp116_code;
			value_t temp116_val;
			value_t temp117_code;
			value_t temp117_val;
			
			if(k==0){
				temp00_val = vdata00.key;
				temp01_val = vdata01.key;
				temp02_val = vdata02.key;
				temp03_val = vdata03.key;
				temp04_val = vdata04.key;
				temp05_val = vdata05.key;
				temp06_val = vdata06.key;
				temp07_val = vdata07.key;
				temp10_val = vdata10.key;
				temp11_val = vdata11.key;
				temp12_val = vdata12.key;
				temp13_val = vdata13.key;
				temp14_val = vdata14.key;
				temp15_val = vdata15.key;
				temp16_val = vdata16.key;
				temp17_val = vdata17.key;
				temp20_val = vdata20.key;
				temp21_val = vdata21.key;
				temp22_val = vdata22.key;
				temp23_val = vdata23.key;
				temp24_val = vdata24.key;
				temp25_val = vdata25.key;
				temp26_val = vdata26.key;
				temp27_val = vdata27.key;
				temp30_val = vdata30.key;
				temp31_val = vdata31.key;
				temp32_val = vdata32.key;
				temp33_val = vdata33.key;
				temp34_val = vdata34.key;
				temp35_val = vdata35.key;
				temp36_val = vdata36.key;
				temp37_val = vdata37.key;
				temp40_val = vdata40.key;
				temp41_val = vdata41.key;
				temp42_val = vdata42.key;
				temp43_val = vdata43.key;
				temp44_val = vdata44.key;
				temp45_val = vdata45.key;
				temp46_val = vdata46.key;
				temp47_val = vdata47.key;
				temp50_val = vdata50.key;
				temp51_val = vdata51.key;
				temp52_val = vdata52.key;
				temp53_val = vdata53.key;
				temp54_val = vdata54.key;
				temp55_val = vdata55.key;
				temp56_val = vdata56.key;
				temp57_val = vdata57.key;
				temp60_val = vdata60.key;
				temp61_val = vdata61.key;
				temp62_val = vdata62.key;
				temp63_val = vdata63.key;
				temp64_val = vdata64.key;
				temp65_val = vdata65.key;
				temp66_val = vdata66.key;
				temp67_val = vdata67.key;
				temp70_val = vdata70.key;
				temp71_val = vdata71.key;
				temp72_val = vdata72.key;
				temp73_val = vdata73.key;
				temp74_val = vdata74.key;
				temp75_val = vdata75.key;
				temp76_val = vdata76.key;
				temp77_val = vdata77.key;
				temp80_val = vdata80.key;
				temp81_val = vdata81.key;
				temp82_val = vdata82.key;
				temp83_val = vdata83.key;
				temp84_val = vdata84.key;
				temp85_val = vdata85.key;
				temp86_val = vdata86.key;
				temp87_val = vdata87.key;
				temp90_val = vdata90.key;
				temp91_val = vdata91.key;
				temp92_val = vdata92.key;
				temp93_val = vdata93.key;
				temp94_val = vdata94.key;
				temp95_val = vdata95.key;
				temp96_val = vdata96.key;
				temp97_val = vdata97.key;
				temp100_val = vdata100.key;
				temp101_val = vdata101.key;
				temp102_val = vdata102.key;
				temp103_val = vdata103.key;
				temp104_val = vdata104.key;
				temp105_val = vdata105.key;
				temp106_val = vdata106.key;
				temp107_val = vdata107.key;
				temp110_val = vdata110.key;
				temp111_val = vdata111.key;
				temp112_val = vdata112.key;
				temp113_val = vdata113.key;
				temp114_val = vdata114.key;
				temp115_val = vdata115.key;
				temp116_val = vdata116.key;
				temp117_val = vdata117.key;
			} else {
				temp00_val = vdata00.value;
				temp01_val = vdata01.value;
				temp02_val = vdata02.value;
				temp03_val = vdata03.value;
				temp04_val = vdata04.value;
				temp05_val = vdata05.value;
				temp06_val = vdata06.value;
				temp07_val = vdata07.value;
				temp10_val = vdata10.value;
				temp11_val = vdata11.value;
				temp12_val = vdata12.value;
				temp13_val = vdata13.value;
				temp14_val = vdata14.value;
				temp15_val = vdata15.value;
				temp16_val = vdata16.value;
				temp17_val = vdata17.value;
				temp20_val = vdata20.value;
				temp21_val = vdata21.value;
				temp22_val = vdata22.value;
				temp23_val = vdata23.value;
				temp24_val = vdata24.value;
				temp25_val = vdata25.value;
				temp26_val = vdata26.value;
				temp27_val = vdata27.value;
				temp30_val = vdata30.value;
				temp31_val = vdata31.value;
				temp32_val = vdata32.value;
				temp33_val = vdata33.value;
				temp34_val = vdata34.value;
				temp35_val = vdata35.value;
				temp36_val = vdata36.value;
				temp37_val = vdata37.value;
				temp40_val = vdata40.value;
				temp41_val = vdata41.value;
				temp42_val = vdata42.value;
				temp43_val = vdata43.value;
				temp44_val = vdata44.value;
				temp45_val = vdata45.value;
				temp46_val = vdata46.value;
				temp47_val = vdata47.value;
				temp50_val = vdata50.value;
				temp51_val = vdata51.value;
				temp52_val = vdata52.value;
				temp53_val = vdata53.value;
				temp54_val = vdata54.value;
				temp55_val = vdata55.value;
				temp56_val = vdata56.value;
				temp57_val = vdata57.value;
				temp60_val = vdata60.value;
				temp61_val = vdata61.value;
				temp62_val = vdata62.value;
				temp63_val = vdata63.value;
				temp64_val = vdata64.value;
				temp65_val = vdata65.value;
				temp66_val = vdata66.value;
				temp67_val = vdata67.value;
				temp70_val = vdata70.value;
				temp71_val = vdata71.value;
				temp72_val = vdata72.value;
				temp73_val = vdata73.value;
				temp74_val = vdata74.value;
				temp75_val = vdata75.value;
				temp76_val = vdata76.value;
				temp77_val = vdata77.value;
				temp80_val = vdata80.value;
				temp81_val = vdata81.value;
				temp82_val = vdata82.value;
				temp83_val = vdata83.value;
				temp84_val = vdata84.value;
				temp85_val = vdata85.value;
				temp86_val = vdata86.value;
				temp87_val = vdata87.value;
				temp90_val = vdata90.value;
				temp91_val = vdata91.value;
				temp92_val = vdata92.value;
				temp93_val = vdata93.value;
				temp94_val = vdata94.value;
				temp95_val = vdata95.value;
				temp96_val = vdata96.value;
				temp97_val = vdata97.value;
				temp100_val = vdata100.value;
				temp101_val = vdata101.value;
				temp102_val = vdata102.value;
				temp103_val = vdata103.value;
				temp104_val = vdata104.value;
				temp105_val = vdata105.value;
				temp106_val = vdata106.value;
				temp107_val = vdata107.value;
				temp110_val = vdata110.value;
				temp111_val = vdata111.value;
				temp112_val = vdata112.value;
				temp113_val = vdata113.value;
				temp114_val = vdata114.value;
				temp115_val = vdata115.value;
				temp116_val = vdata116.value;
				temp117_val = vdata117.value;
			}
			
			value_t a0 = aplus(temp00_val, temp01_val);
			value_t b0 = aplus(temp02_val, temp03_val);
			value_t c0 = aplus(temp04_val, temp05_val);
			value_t d0 = aplus(temp06_val, temp07_val);
			value_t a1 = aplus(temp10_val, temp11_val);
			value_t b1 = aplus(temp12_val, temp13_val);
			value_t c1 = aplus(temp14_val, temp15_val);
			value_t d1 = aplus(temp16_val, temp17_val);
			value_t a2 = aplus(temp20_val, temp21_val);
			value_t b2 = aplus(temp22_val, temp23_val);
			value_t c2 = aplus(temp24_val, temp25_val);
			value_t d2 = aplus(temp26_val, temp27_val);
			value_t a3 = aplus(temp30_val, temp31_val);
			value_t b3 = aplus(temp32_val, temp33_val);
			value_t c3 = aplus(temp34_val, temp35_val);
			value_t d3 = aplus(temp36_val, temp37_val);
			value_t a4 = aplus(temp40_val, temp41_val);
			value_t b4 = aplus(temp42_val, temp43_val);
			value_t c4 = aplus(temp44_val, temp45_val);
			value_t d4 = aplus(temp46_val, temp47_val);
			value_t a5 = aplus(temp50_val, temp51_val);
			value_t b5 = aplus(temp52_val, temp53_val);
			value_t c5 = aplus(temp54_val, temp55_val);
			value_t d5 = aplus(temp56_val, temp57_val);
			value_t a6 = aplus(temp60_val, temp61_val);
			value_t b6 = aplus(temp62_val, temp63_val);
			value_t c6 = aplus(temp64_val, temp65_val);
			value_t d6 = aplus(temp66_val, temp67_val);
			value_t a7 = aplus(temp70_val, temp71_val);
			value_t b7 = aplus(temp72_val, temp73_val);
			value_t c7 = aplus(temp74_val, temp75_val);
			value_t d7 = aplus(temp76_val, temp77_val);
			value_t a8 = aplus(temp80_val, temp81_val);
			value_t b8 = aplus(temp82_val, temp83_val);
			value_t c8 = aplus(temp84_val, temp85_val);
			value_t d8 = aplus(temp86_val, temp87_val);
			value_t a9 = aplus(temp90_val, temp91_val);
			value_t b9 = aplus(temp92_val, temp93_val);
			value_t c9 = aplus(temp94_val, temp95_val);
			value_t d9 = aplus(temp96_val, temp97_val);
			value_t a10 = aplus(temp100_val, temp101_val);
			value_t b10 = aplus(temp102_val, temp103_val);
			value_t c10 = aplus(temp104_val, temp105_val);
			value_t d10 = aplus(temp106_val, temp107_val);
			value_t a11 = aplus(temp110_val, temp111_val);
			value_t b11 = aplus(temp112_val, temp113_val);
			value_t c11 = aplus(temp114_val, temp115_val);
			value_t d11 = aplus(temp116_val, temp117_val);
			
			value_t m0 = aplus(a0, b0);
			value_t n0 = aplus(c0, d0);
			value_t m1 = aplus(a1, b1);
			value_t n1 = aplus(c1, d1);
			value_t m2 = aplus(a2, b2);
			value_t n2 = aplus(c2, d2);
			value_t m3 = aplus(a3, b3);
			value_t n3 = aplus(c3, d3);
			value_t m4 = aplus(a4, b4);
			value_t n4 = aplus(c4, d4);
			value_t m5 = aplus(a5, b5);
			value_t n5 = aplus(c5, d5);
			value_t m6 = aplus(a6, b6);
			value_t n6 = aplus(c6, d6);
			value_t m7 = aplus(a7, b7);
			value_t n7 = aplus(c7, d7);
			value_t m8 = aplus(a8, b8);
			value_t n8 = aplus(c8, d8);
			value_t m9 = aplus(a9, b9);
			value_t n9 = aplus(c9, d9);
			value_t m10 = aplus(a10, b10);
			value_t n10 = aplus(c10, d10);
			value_t m11 = aplus(a11, b11);
			value_t n11 = aplus(c11, d11);
			
			value_t v0 = aplus(m0, n0);
			value_t v1 = aplus(m1, n1);
			value_t v2 = aplus(m2, n2);
			value_t v3 = aplus(m3, n3);
			value_t v4 = aplus(m4, n4);
			value_t v5 = aplus(m5, n5);
			value_t v6 = aplus(m6, n6);
			value_t v7 = aplus(m7, n7);
			value_t v8 = aplus(m8, n8);
			value_t v9 = aplus(m9, n9);
			value_t v10 = aplus(m10, n10);
			value_t v11 = aplus(m11, n11);
			
			#if NUMCOMPUTEUNITS==1 // AUTOMATEME.
			value_t z = v0;
			#endif 
			
			#if NUMCOMPUTEUNITS==2
			value_t z = aplus(v0, v1);
			#endif 
			
			#if NUMCOMPUTEUNITS==4
			value_t w0 = aplus(v0, v1);
			value_t w1 = aplus(v2, v3);
			
			value_t z = aplus(w0, w1);
			#endif 
			
			#if NUMCOMPUTEUNITS==12
			value_t w0 = aplus(v0, v1);
			value_t w1 = aplus(v2, v3);
			value_t w2 = aplus(v4, v5);
			value_t w3 = aplus(v6, v7);
			value_t w4 = aplus(v8, v9);
			value_t w5 = aplus(v10, v11);
			
			value_t x0 = aplus(w0, w1);
			value_t x1 = aplus(w2, w3);
			value_t x2 = aplus(w4, w5);
			
			value_t y0 = aplus(x0, x1);
			value_t y1 = aplus(x2, 0xFFFFFFFF);
			
			value_t z = aplus(y0, y1);
			#endif 
			
			#if NUMCOMPUTEUNITS==14
			value_t w0 = aplus(v0, v1);
			value_t w1 = aplus(v2, v3);
			value_t w2 = aplus(v4, v5);
			value_t w3 = aplus(v6, v7);
			value_t w4 = aplus(v8, v9);
			value_t w5 = aplus(v10, v11);
			value_t w6 = aplus(v12, v13);
			
			value_t x0 = aplus(w0, w1);
			value_t x1 = aplus(w2, w3);
			value_t x2 = aplus(w4, w5);
			value_t x3 = aplus(w6, 0xFFFFFFFF);
			
			value_t y0 = aplus(x0, x1);
			value_t y1 = aplus(x2, x3);
			
			value_t z = aplus(y0, y1);
			#endif 
			
			#if NUMCOMPUTEUNITS==16
			value_t w0 = aplus(v0, v1);
			value_t w1 = aplus(v2, v3);
			value_t w2 = aplus(v4, v5);
			value_t w3 = aplus(v6, v7);
			value_t w4 = aplus(v8, v9);
			value_t w5 = aplus(v10, v11);
			value_t w6 = aplus(v12, v13);
			value_t w7 = aplus(v14, v15);
			
			value_t x0 = aplus(w0, w1);
			value_t x1 = aplus(w2, w3);
			value_t x2 = aplus(w4, w5);
			value_t x3 = aplus(w6, w7);
			
			value_t y0 = aplus(x0, x1);
			value_t y1 = aplus(x2, x3);
			
			value_t z = aplus(y0, y1);
			#endif 
			
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("unifydata_pr_parallelsyn 8", dest_v, VECTOR_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("unifydata_pr_parallelsyn 9", destoffset_kvs + dest_i, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			#endif
			if(k==0){ 
				destbuffer[dest_v][destoffset_kvs + dest_i].key = z; 
			} else {
				destbuffer[dest_v][destoffset_kvs + dest_i].value = z;
			}
		}
		
		dest_v+=1; if(dest_v == VECTOR_SIZE){ dest_v=0; dest_i+=1; }
	}
	return;
}

void 
	#ifdef SW 
	acts::
	#endif
unifydata_bfs_parallelsyn(bool_type enable, keyvalue_t sourcebuffer0[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer3[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer5[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer6[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer7[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer8[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer9[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer10[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer11[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
					buffer_type destoffset, buffer_type size, sweepparams_t sweepparams, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount = (APPLYVERTEXBUFFERSZ / VDATAPACKINGFACTOR) / 2;
	
	buffer_type dest_v = 0;
	buffer_type dest_i = 0;
	buffer_type destoffset_kvs = destoffset / VECTOR_SIZE;
	unsigned int nonzeroactvvsreturned = OFF;
	
	UNIFYDATA_LOOP1: for(buffer_type i=0; i<size; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE II=2
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("unifydata_bfs_parallelsyn.i", i, PADDEDDESTBUFFER_SIZE, destoffset, destoffset_kvs, size);
		#endif
		
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
		
		destbuffer[dest_v][destoffset_kvs + dest_i] = data;
		dest_v+=1; if(dest_v == VECTOR_SIZE){ dest_v=0; dest_i+=1; }
	}
	return;
}

bool_type 
	#ifdef SW 
	acts::
	#endif
unifydata_sssp_parallelsyn(bool_type enable, keyvalue_t sourcebuffer0[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer3[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer5[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer6[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer7[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer8[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer9[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer10[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer11[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
					keyvalue_t actvvs[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], buffer_type * actvvtracker,
					buffer_type destoffset, buffer_type size, sweepparams_t sweepparams, globalparams_t globalparams){
	if(enable == OFF){ return OFF; }
	analysis_type analysis_loopcount = (APPLYVERTEXBUFFERSZ / VDATAPACKINGFACTOR) / 2;
	
	buffer_type dest_v = 0;
	buffer_type dest_i = 0;
	buffer_type destoffset_kvs = destoffset / VECTOR_SIZE;
	unsigned int nonzeroactvvsreturned = OFF;
	unsigned int tempactvvtracker = *actvvtracker;
	
	UNIFYDATA_LOOP1: for(buffer_type i=0; i<size; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE II=2
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("unifydata_sssp_parallelsyn.i", i, PADDEDDESTBUFFER_SIZE, destoffset, destoffset_kvs, size);
		#endif
		
		for(unsigned int k=0; k<2; k++){
			value_t code = 0;
			value_t code0 = 0;
			value_t code1 = 0;
			value_t code2 = 0;
			value_t code3 = 0;
			value_t code4 = 0;
			value_t code5 = 0;
			value_t code6 = 0;
			value_t code7 = 0;
			value_t code8 = 0;
			value_t code9 = 0;
			value_t code10 = 0;
			value_t code11 = 0;
			
			keyvalue_t vdata00 = sourcebuffer0[0][i];
			keyvalue_t vdata01 = sourcebuffer0[1][i];
			keyvalue_t vdata02 = sourcebuffer0[2][i];
			keyvalue_t vdata03 = sourcebuffer0[3][i];
			keyvalue_t vdata04 = sourcebuffer0[4][i];
			keyvalue_t vdata05 = sourcebuffer0[5][i];
			keyvalue_t vdata06 = sourcebuffer0[6][i];
			keyvalue_t vdata07 = sourcebuffer0[7][i];
			keyvalue_t vdata10 = sourcebuffer1[0][i];
			keyvalue_t vdata11 = sourcebuffer1[1][i];
			keyvalue_t vdata12 = sourcebuffer1[2][i];
			keyvalue_t vdata13 = sourcebuffer1[3][i];
			keyvalue_t vdata14 = sourcebuffer1[4][i];
			keyvalue_t vdata15 = sourcebuffer1[5][i];
			keyvalue_t vdata16 = sourcebuffer1[6][i];
			keyvalue_t vdata17 = sourcebuffer1[7][i];
			keyvalue_t vdata20 = sourcebuffer2[0][i];
			keyvalue_t vdata21 = sourcebuffer2[1][i];
			keyvalue_t vdata22 = sourcebuffer2[2][i];
			keyvalue_t vdata23 = sourcebuffer2[3][i];
			keyvalue_t vdata24 = sourcebuffer2[4][i];
			keyvalue_t vdata25 = sourcebuffer2[5][i];
			keyvalue_t vdata26 = sourcebuffer2[6][i];
			keyvalue_t vdata27 = sourcebuffer2[7][i];
			keyvalue_t vdata30 = sourcebuffer3[0][i];
			keyvalue_t vdata31 = sourcebuffer3[1][i];
			keyvalue_t vdata32 = sourcebuffer3[2][i];
			keyvalue_t vdata33 = sourcebuffer3[3][i];
			keyvalue_t vdata34 = sourcebuffer3[4][i];
			keyvalue_t vdata35 = sourcebuffer3[5][i];
			keyvalue_t vdata36 = sourcebuffer3[6][i];
			keyvalue_t vdata37 = sourcebuffer3[7][i];
			keyvalue_t vdata40 = sourcebuffer4[0][i];
			keyvalue_t vdata41 = sourcebuffer4[1][i];
			keyvalue_t vdata42 = sourcebuffer4[2][i];
			keyvalue_t vdata43 = sourcebuffer4[3][i];
			keyvalue_t vdata44 = sourcebuffer4[4][i];
			keyvalue_t vdata45 = sourcebuffer4[5][i];
			keyvalue_t vdata46 = sourcebuffer4[6][i];
			keyvalue_t vdata47 = sourcebuffer4[7][i];
			keyvalue_t vdata50 = sourcebuffer5[0][i];
			keyvalue_t vdata51 = sourcebuffer5[1][i];
			keyvalue_t vdata52 = sourcebuffer5[2][i];
			keyvalue_t vdata53 = sourcebuffer5[3][i];
			keyvalue_t vdata54 = sourcebuffer5[4][i];
			keyvalue_t vdata55 = sourcebuffer5[5][i];
			keyvalue_t vdata56 = sourcebuffer5[6][i];
			keyvalue_t vdata57 = sourcebuffer5[7][i];
			keyvalue_t vdata60 = sourcebuffer6[0][i];
			keyvalue_t vdata61 = sourcebuffer6[1][i];
			keyvalue_t vdata62 = sourcebuffer6[2][i];
			keyvalue_t vdata63 = sourcebuffer6[3][i];
			keyvalue_t vdata64 = sourcebuffer6[4][i];
			keyvalue_t vdata65 = sourcebuffer6[5][i];
			keyvalue_t vdata66 = sourcebuffer6[6][i];
			keyvalue_t vdata67 = sourcebuffer6[7][i];
			keyvalue_t vdata70 = sourcebuffer7[0][i];
			keyvalue_t vdata71 = sourcebuffer7[1][i];
			keyvalue_t vdata72 = sourcebuffer7[2][i];
			keyvalue_t vdata73 = sourcebuffer7[3][i];
			keyvalue_t vdata74 = sourcebuffer7[4][i];
			keyvalue_t vdata75 = sourcebuffer7[5][i];
			keyvalue_t vdata76 = sourcebuffer7[6][i];
			keyvalue_t vdata77 = sourcebuffer7[7][i];
			keyvalue_t vdata80 = sourcebuffer8[0][i];
			keyvalue_t vdata81 = sourcebuffer8[1][i];
			keyvalue_t vdata82 = sourcebuffer8[2][i];
			keyvalue_t vdata83 = sourcebuffer8[3][i];
			keyvalue_t vdata84 = sourcebuffer8[4][i];
			keyvalue_t vdata85 = sourcebuffer8[5][i];
			keyvalue_t vdata86 = sourcebuffer8[6][i];
			keyvalue_t vdata87 = sourcebuffer8[7][i];
			keyvalue_t vdata90 = sourcebuffer9[0][i];
			keyvalue_t vdata91 = sourcebuffer9[1][i];
			keyvalue_t vdata92 = sourcebuffer9[2][i];
			keyvalue_t vdata93 = sourcebuffer9[3][i];
			keyvalue_t vdata94 = sourcebuffer9[4][i];
			keyvalue_t vdata95 = sourcebuffer9[5][i];
			keyvalue_t vdata96 = sourcebuffer9[6][i];
			keyvalue_t vdata97 = sourcebuffer9[7][i];
			keyvalue_t vdata100 = sourcebuffer10[0][i];
			keyvalue_t vdata101 = sourcebuffer10[1][i];
			keyvalue_t vdata102 = sourcebuffer10[2][i];
			keyvalue_t vdata103 = sourcebuffer10[3][i];
			keyvalue_t vdata104 = sourcebuffer10[4][i];
			keyvalue_t vdata105 = sourcebuffer10[5][i];
			keyvalue_t vdata106 = sourcebuffer10[6][i];
			keyvalue_t vdata107 = sourcebuffer10[7][i];
			keyvalue_t vdata110 = sourcebuffer11[0][i];
			keyvalue_t vdata111 = sourcebuffer11[1][i];
			keyvalue_t vdata112 = sourcebuffer11[2][i];
			keyvalue_t vdata113 = sourcebuffer11[3][i];
			keyvalue_t vdata114 = sourcebuffer11[4][i];
			keyvalue_t vdata115 = sourcebuffer11[5][i];
			keyvalue_t vdata116 = sourcebuffer11[6][i];
			keyvalue_t vdata117 = sourcebuffer11[7][i];
			
			value_t temp00_code;
			value_t temp00_val;
			value_t temp01_code;
			value_t temp01_val;
			value_t temp02_code;
			value_t temp02_val;
			value_t temp03_code;
			value_t temp03_val;
			value_t temp04_code;
			value_t temp04_val;
			value_t temp05_code;
			value_t temp05_val;
			value_t temp06_code;
			value_t temp06_val;
			value_t temp07_code;
			value_t temp07_val;
			value_t temp10_code;
			value_t temp10_val;
			value_t temp11_code;
			value_t temp11_val;
			value_t temp12_code;
			value_t temp12_val;
			value_t temp13_code;
			value_t temp13_val;
			value_t temp14_code;
			value_t temp14_val;
			value_t temp15_code;
			value_t temp15_val;
			value_t temp16_code;
			value_t temp16_val;
			value_t temp17_code;
			value_t temp17_val;
			value_t temp20_code;
			value_t temp20_val;
			value_t temp21_code;
			value_t temp21_val;
			value_t temp22_code;
			value_t temp22_val;
			value_t temp23_code;
			value_t temp23_val;
			value_t temp24_code;
			value_t temp24_val;
			value_t temp25_code;
			value_t temp25_val;
			value_t temp26_code;
			value_t temp26_val;
			value_t temp27_code;
			value_t temp27_val;
			value_t temp30_code;
			value_t temp30_val;
			value_t temp31_code;
			value_t temp31_val;
			value_t temp32_code;
			value_t temp32_val;
			value_t temp33_code;
			value_t temp33_val;
			value_t temp34_code;
			value_t temp34_val;
			value_t temp35_code;
			value_t temp35_val;
			value_t temp36_code;
			value_t temp36_val;
			value_t temp37_code;
			value_t temp37_val;
			value_t temp40_code;
			value_t temp40_val;
			value_t temp41_code;
			value_t temp41_val;
			value_t temp42_code;
			value_t temp42_val;
			value_t temp43_code;
			value_t temp43_val;
			value_t temp44_code;
			value_t temp44_val;
			value_t temp45_code;
			value_t temp45_val;
			value_t temp46_code;
			value_t temp46_val;
			value_t temp47_code;
			value_t temp47_val;
			value_t temp50_code;
			value_t temp50_val;
			value_t temp51_code;
			value_t temp51_val;
			value_t temp52_code;
			value_t temp52_val;
			value_t temp53_code;
			value_t temp53_val;
			value_t temp54_code;
			value_t temp54_val;
			value_t temp55_code;
			value_t temp55_val;
			value_t temp56_code;
			value_t temp56_val;
			value_t temp57_code;
			value_t temp57_val;
			value_t temp60_code;
			value_t temp60_val;
			value_t temp61_code;
			value_t temp61_val;
			value_t temp62_code;
			value_t temp62_val;
			value_t temp63_code;
			value_t temp63_val;
			value_t temp64_code;
			value_t temp64_val;
			value_t temp65_code;
			value_t temp65_val;
			value_t temp66_code;
			value_t temp66_val;
			value_t temp67_code;
			value_t temp67_val;
			value_t temp70_code;
			value_t temp70_val;
			value_t temp71_code;
			value_t temp71_val;
			value_t temp72_code;
			value_t temp72_val;
			value_t temp73_code;
			value_t temp73_val;
			value_t temp74_code;
			value_t temp74_val;
			value_t temp75_code;
			value_t temp75_val;
			value_t temp76_code;
			value_t temp76_val;
			value_t temp77_code;
			value_t temp77_val;
			value_t temp80_code;
			value_t temp80_val;
			value_t temp81_code;
			value_t temp81_val;
			value_t temp82_code;
			value_t temp82_val;
			value_t temp83_code;
			value_t temp83_val;
			value_t temp84_code;
			value_t temp84_val;
			value_t temp85_code;
			value_t temp85_val;
			value_t temp86_code;
			value_t temp86_val;
			value_t temp87_code;
			value_t temp87_val;
			value_t temp90_code;
			value_t temp90_val;
			value_t temp91_code;
			value_t temp91_val;
			value_t temp92_code;
			value_t temp92_val;
			value_t temp93_code;
			value_t temp93_val;
			value_t temp94_code;
			value_t temp94_val;
			value_t temp95_code;
			value_t temp95_val;
			value_t temp96_code;
			value_t temp96_val;
			value_t temp97_code;
			value_t temp97_val;
			value_t temp100_code;
			value_t temp100_val;
			value_t temp101_code;
			value_t temp101_val;
			value_t temp102_code;
			value_t temp102_val;
			value_t temp103_code;
			value_t temp103_val;
			value_t temp104_code;
			value_t temp104_val;
			value_t temp105_code;
			value_t temp105_val;
			value_t temp106_code;
			value_t temp106_val;
			value_t temp107_code;
			value_t temp107_val;
			value_t temp110_code;
			value_t temp110_val;
			value_t temp111_code;
			value_t temp111_val;
			value_t temp112_code;
			value_t temp112_val;
			value_t temp113_code;
			value_t temp113_val;
			value_t temp114_code;
			value_t temp114_val;
			value_t temp115_code;
			value_t temp115_val;
			value_t temp116_code;
			value_t temp116_val;
			value_t temp117_code;
			value_t temp117_val;
			
			if(k==0){
				temp00_code = READFROM_UINT(vdata00.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp00_val = READFROM_UINT(vdata00.key, 0, 30); 
				temp01_code = READFROM_UINT(vdata01.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp01_val = READFROM_UINT(vdata01.key, 0, 30); 
				temp02_code = READFROM_UINT(vdata02.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp02_val = READFROM_UINT(vdata02.key, 0, 30); 
				temp03_code = READFROM_UINT(vdata03.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp03_val = READFROM_UINT(vdata03.key, 0, 30); 
				temp04_code = READFROM_UINT(vdata04.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp04_val = READFROM_UINT(vdata04.key, 0, 30); 
				temp05_code = READFROM_UINT(vdata05.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp05_val = READFROM_UINT(vdata05.key, 0, 30); 
				temp06_code = READFROM_UINT(vdata06.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp06_val = READFROM_UINT(vdata06.key, 0, 30); 
				temp07_code = READFROM_UINT(vdata07.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp07_val = READFROM_UINT(vdata07.key, 0, 30); 
				temp10_code = READFROM_UINT(vdata10.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp10_val = READFROM_UINT(vdata10.key, 0, 30); 
				temp11_code = READFROM_UINT(vdata11.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp11_val = READFROM_UINT(vdata11.key, 0, 30); 
				temp12_code = READFROM_UINT(vdata12.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp12_val = READFROM_UINT(vdata12.key, 0, 30); 
				temp13_code = READFROM_UINT(vdata13.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp13_val = READFROM_UINT(vdata13.key, 0, 30); 
				temp14_code = READFROM_UINT(vdata14.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp14_val = READFROM_UINT(vdata14.key, 0, 30); 
				temp15_code = READFROM_UINT(vdata15.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp15_val = READFROM_UINT(vdata15.key, 0, 30); 
				temp16_code = READFROM_UINT(vdata16.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp16_val = READFROM_UINT(vdata16.key, 0, 30); 
				temp17_code = READFROM_UINT(vdata17.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp17_val = READFROM_UINT(vdata17.key, 0, 30); 
				temp20_code = READFROM_UINT(vdata20.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp20_val = READFROM_UINT(vdata20.key, 0, 30); 
				temp21_code = READFROM_UINT(vdata21.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp21_val = READFROM_UINT(vdata21.key, 0, 30); 
				temp22_code = READFROM_UINT(vdata22.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp22_val = READFROM_UINT(vdata22.key, 0, 30); 
				temp23_code = READFROM_UINT(vdata23.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp23_val = READFROM_UINT(vdata23.key, 0, 30); 
				temp24_code = READFROM_UINT(vdata24.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp24_val = READFROM_UINT(vdata24.key, 0, 30); 
				temp25_code = READFROM_UINT(vdata25.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp25_val = READFROM_UINT(vdata25.key, 0, 30); 
				temp26_code = READFROM_UINT(vdata26.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp26_val = READFROM_UINT(vdata26.key, 0, 30); 
				temp27_code = READFROM_UINT(vdata27.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp27_val = READFROM_UINT(vdata27.key, 0, 30); 
				temp30_code = READFROM_UINT(vdata30.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp30_val = READFROM_UINT(vdata30.key, 0, 30); 
				temp31_code = READFROM_UINT(vdata31.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp31_val = READFROM_UINT(vdata31.key, 0, 30); 
				temp32_code = READFROM_UINT(vdata32.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp32_val = READFROM_UINT(vdata32.key, 0, 30); 
				temp33_code = READFROM_UINT(vdata33.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp33_val = READFROM_UINT(vdata33.key, 0, 30); 
				temp34_code = READFROM_UINT(vdata34.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp34_val = READFROM_UINT(vdata34.key, 0, 30); 
				temp35_code = READFROM_UINT(vdata35.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp35_val = READFROM_UINT(vdata35.key, 0, 30); 
				temp36_code = READFROM_UINT(vdata36.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp36_val = READFROM_UINT(vdata36.key, 0, 30); 
				temp37_code = READFROM_UINT(vdata37.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp37_val = READFROM_UINT(vdata37.key, 0, 30); 
				temp40_code = READFROM_UINT(vdata40.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp40_val = READFROM_UINT(vdata40.key, 0, 30); 
				temp41_code = READFROM_UINT(vdata41.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp41_val = READFROM_UINT(vdata41.key, 0, 30); 
				temp42_code = READFROM_UINT(vdata42.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp42_val = READFROM_UINT(vdata42.key, 0, 30); 
				temp43_code = READFROM_UINT(vdata43.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp43_val = READFROM_UINT(vdata43.key, 0, 30); 
				temp44_code = READFROM_UINT(vdata44.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp44_val = READFROM_UINT(vdata44.key, 0, 30); 
				temp45_code = READFROM_UINT(vdata45.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp45_val = READFROM_UINT(vdata45.key, 0, 30); 
				temp46_code = READFROM_UINT(vdata46.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp46_val = READFROM_UINT(vdata46.key, 0, 30); 
				temp47_code = READFROM_UINT(vdata47.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp47_val = READFROM_UINT(vdata47.key, 0, 30); 
				temp50_code = READFROM_UINT(vdata50.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp50_val = READFROM_UINT(vdata50.key, 0, 30); 
				temp51_code = READFROM_UINT(vdata51.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp51_val = READFROM_UINT(vdata51.key, 0, 30); 
				temp52_code = READFROM_UINT(vdata52.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp52_val = READFROM_UINT(vdata52.key, 0, 30); 
				temp53_code = READFROM_UINT(vdata53.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp53_val = READFROM_UINT(vdata53.key, 0, 30); 
				temp54_code = READFROM_UINT(vdata54.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp54_val = READFROM_UINT(vdata54.key, 0, 30); 
				temp55_code = READFROM_UINT(vdata55.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp55_val = READFROM_UINT(vdata55.key, 0, 30); 
				temp56_code = READFROM_UINT(vdata56.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp56_val = READFROM_UINT(vdata56.key, 0, 30); 
				temp57_code = READFROM_UINT(vdata57.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp57_val = READFROM_UINT(vdata57.key, 0, 30); 
				temp60_code = READFROM_UINT(vdata60.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp60_val = READFROM_UINT(vdata60.key, 0, 30); 
				temp61_code = READFROM_UINT(vdata61.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp61_val = READFROM_UINT(vdata61.key, 0, 30); 
				temp62_code = READFROM_UINT(vdata62.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp62_val = READFROM_UINT(vdata62.key, 0, 30); 
				temp63_code = READFROM_UINT(vdata63.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp63_val = READFROM_UINT(vdata63.key, 0, 30); 
				temp64_code = READFROM_UINT(vdata64.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp64_val = READFROM_UINT(vdata64.key, 0, 30); 
				temp65_code = READFROM_UINT(vdata65.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp65_val = READFROM_UINT(vdata65.key, 0, 30); 
				temp66_code = READFROM_UINT(vdata66.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp66_val = READFROM_UINT(vdata66.key, 0, 30); 
				temp67_code = READFROM_UINT(vdata67.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp67_val = READFROM_UINT(vdata67.key, 0, 30); 
				temp70_code = READFROM_UINT(vdata70.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp70_val = READFROM_UINT(vdata70.key, 0, 30); 
				temp71_code = READFROM_UINT(vdata71.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp71_val = READFROM_UINT(vdata71.key, 0, 30); 
				temp72_code = READFROM_UINT(vdata72.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp72_val = READFROM_UINT(vdata72.key, 0, 30); 
				temp73_code = READFROM_UINT(vdata73.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp73_val = READFROM_UINT(vdata73.key, 0, 30); 
				temp74_code = READFROM_UINT(vdata74.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp74_val = READFROM_UINT(vdata74.key, 0, 30); 
				temp75_code = READFROM_UINT(vdata75.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp75_val = READFROM_UINT(vdata75.key, 0, 30); 
				temp76_code = READFROM_UINT(vdata76.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp76_val = READFROM_UINT(vdata76.key, 0, 30); 
				temp77_code = READFROM_UINT(vdata77.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp77_val = READFROM_UINT(vdata77.key, 0, 30); 
				temp80_code = READFROM_UINT(vdata80.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp80_val = READFROM_UINT(vdata80.key, 0, 30); 
				temp81_code = READFROM_UINT(vdata81.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp81_val = READFROM_UINT(vdata81.key, 0, 30); 
				temp82_code = READFROM_UINT(vdata82.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp82_val = READFROM_UINT(vdata82.key, 0, 30); 
				temp83_code = READFROM_UINT(vdata83.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp83_val = READFROM_UINT(vdata83.key, 0, 30); 
				temp84_code = READFROM_UINT(vdata84.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp84_val = READFROM_UINT(vdata84.key, 0, 30); 
				temp85_code = READFROM_UINT(vdata85.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp85_val = READFROM_UINT(vdata85.key, 0, 30); 
				temp86_code = READFROM_UINT(vdata86.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp86_val = READFROM_UINT(vdata86.key, 0, 30); 
				temp87_code = READFROM_UINT(vdata87.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp87_val = READFROM_UINT(vdata87.key, 0, 30); 
				temp90_code = READFROM_UINT(vdata90.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp90_val = READFROM_UINT(vdata90.key, 0, 30); 
				temp91_code = READFROM_UINT(vdata91.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp91_val = READFROM_UINT(vdata91.key, 0, 30); 
				temp92_code = READFROM_UINT(vdata92.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp92_val = READFROM_UINT(vdata92.key, 0, 30); 
				temp93_code = READFROM_UINT(vdata93.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp93_val = READFROM_UINT(vdata93.key, 0, 30); 
				temp94_code = READFROM_UINT(vdata94.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp94_val = READFROM_UINT(vdata94.key, 0, 30); 
				temp95_code = READFROM_UINT(vdata95.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp95_val = READFROM_UINT(vdata95.key, 0, 30); 
				temp96_code = READFROM_UINT(vdata96.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp96_val = READFROM_UINT(vdata96.key, 0, 30); 
				temp97_code = READFROM_UINT(vdata97.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp97_val = READFROM_UINT(vdata97.key, 0, 30); 
				temp100_code = READFROM_UINT(vdata100.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp100_val = READFROM_UINT(vdata100.key, 0, 30); 
				temp101_code = READFROM_UINT(vdata101.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp101_val = READFROM_UINT(vdata101.key, 0, 30); 
				temp102_code = READFROM_UINT(vdata102.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp102_val = READFROM_UINT(vdata102.key, 0, 30); 
				temp103_code = READFROM_UINT(vdata103.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp103_val = READFROM_UINT(vdata103.key, 0, 30); 
				temp104_code = READFROM_UINT(vdata104.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp104_val = READFROM_UINT(vdata104.key, 0, 30); 
				temp105_code = READFROM_UINT(vdata105.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp105_val = READFROM_UINT(vdata105.key, 0, 30); 
				temp106_code = READFROM_UINT(vdata106.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp106_val = READFROM_UINT(vdata106.key, 0, 30); 
				temp107_code = READFROM_UINT(vdata107.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp107_val = READFROM_UINT(vdata107.key, 0, 30); 
				temp110_code = READFROM_UINT(vdata110.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp110_val = READFROM_UINT(vdata110.key, 0, 30); 
				temp111_code = READFROM_UINT(vdata111.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp111_val = READFROM_UINT(vdata111.key, 0, 30); 
				temp112_code = READFROM_UINT(vdata112.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp112_val = READFROM_UINT(vdata112.key, 0, 30); 
				temp113_code = READFROM_UINT(vdata113.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp113_val = READFROM_UINT(vdata113.key, 0, 30); 
				temp114_code = READFROM_UINT(vdata114.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp114_val = READFROM_UINT(vdata114.key, 0, 30); 
				temp115_code = READFROM_UINT(vdata115.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp115_val = READFROM_UINT(vdata115.key, 0, 30); 
				temp116_code = READFROM_UINT(vdata116.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp116_val = READFROM_UINT(vdata116.key, 0, 30); 
				temp117_code = READFROM_UINT(vdata117.key, 30, 2); // FIXME. use range for _WIDEWORD
				temp117_val = READFROM_UINT(vdata117.key, 0, 30); 
			} else {
				temp00_code = READFROM_UINT(vdata00.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp00_val = READFROM_UINT(vdata00.value, 0, 30); 
				temp01_code = READFROM_UINT(vdata01.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp01_val = READFROM_UINT(vdata01.value, 0, 30); 
				temp02_code = READFROM_UINT(vdata02.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp02_val = READFROM_UINT(vdata02.value, 0, 30); 
				temp03_code = READFROM_UINT(vdata03.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp03_val = READFROM_UINT(vdata03.value, 0, 30); 
				temp04_code = READFROM_UINT(vdata04.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp04_val = READFROM_UINT(vdata04.value, 0, 30); 
				temp05_code = READFROM_UINT(vdata05.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp05_val = READFROM_UINT(vdata05.value, 0, 30); 
				temp06_code = READFROM_UINT(vdata06.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp06_val = READFROM_UINT(vdata06.value, 0, 30); 
				temp07_code = READFROM_UINT(vdata07.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp07_val = READFROM_UINT(vdata07.value, 0, 30); 
				temp10_code = READFROM_UINT(vdata10.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp10_val = READFROM_UINT(vdata10.value, 0, 30); 
				temp11_code = READFROM_UINT(vdata11.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp11_val = READFROM_UINT(vdata11.value, 0, 30); 
				temp12_code = READFROM_UINT(vdata12.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp12_val = READFROM_UINT(vdata12.value, 0, 30); 
				temp13_code = READFROM_UINT(vdata13.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp13_val = READFROM_UINT(vdata13.value, 0, 30); 
				temp14_code = READFROM_UINT(vdata14.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp14_val = READFROM_UINT(vdata14.value, 0, 30); 
				temp15_code = READFROM_UINT(vdata15.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp15_val = READFROM_UINT(vdata15.value, 0, 30); 
				temp16_code = READFROM_UINT(vdata16.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp16_val = READFROM_UINT(vdata16.value, 0, 30); 
				temp17_code = READFROM_UINT(vdata17.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp17_val = READFROM_UINT(vdata17.value, 0, 30); 
				temp20_code = READFROM_UINT(vdata20.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp20_val = READFROM_UINT(vdata20.value, 0, 30); 
				temp21_code = READFROM_UINT(vdata21.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp21_val = READFROM_UINT(vdata21.value, 0, 30); 
				temp22_code = READFROM_UINT(vdata22.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp22_val = READFROM_UINT(vdata22.value, 0, 30); 
				temp23_code = READFROM_UINT(vdata23.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp23_val = READFROM_UINT(vdata23.value, 0, 30); 
				temp24_code = READFROM_UINT(vdata24.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp24_val = READFROM_UINT(vdata24.value, 0, 30); 
				temp25_code = READFROM_UINT(vdata25.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp25_val = READFROM_UINT(vdata25.value, 0, 30); 
				temp26_code = READFROM_UINT(vdata26.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp26_val = READFROM_UINT(vdata26.value, 0, 30); 
				temp27_code = READFROM_UINT(vdata27.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp27_val = READFROM_UINT(vdata27.value, 0, 30); 
				temp30_code = READFROM_UINT(vdata30.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp30_val = READFROM_UINT(vdata30.value, 0, 30); 
				temp31_code = READFROM_UINT(vdata31.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp31_val = READFROM_UINT(vdata31.value, 0, 30); 
				temp32_code = READFROM_UINT(vdata32.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp32_val = READFROM_UINT(vdata32.value, 0, 30); 
				temp33_code = READFROM_UINT(vdata33.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp33_val = READFROM_UINT(vdata33.value, 0, 30); 
				temp34_code = READFROM_UINT(vdata34.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp34_val = READFROM_UINT(vdata34.value, 0, 30); 
				temp35_code = READFROM_UINT(vdata35.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp35_val = READFROM_UINT(vdata35.value, 0, 30); 
				temp36_code = READFROM_UINT(vdata36.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp36_val = READFROM_UINT(vdata36.value, 0, 30); 
				temp37_code = READFROM_UINT(vdata37.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp37_val = READFROM_UINT(vdata37.value, 0, 30); 
				temp40_code = READFROM_UINT(vdata40.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp40_val = READFROM_UINT(vdata40.value, 0, 30); 
				temp41_code = READFROM_UINT(vdata41.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp41_val = READFROM_UINT(vdata41.value, 0, 30); 
				temp42_code = READFROM_UINT(vdata42.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp42_val = READFROM_UINT(vdata42.value, 0, 30); 
				temp43_code = READFROM_UINT(vdata43.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp43_val = READFROM_UINT(vdata43.value, 0, 30); 
				temp44_code = READFROM_UINT(vdata44.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp44_val = READFROM_UINT(vdata44.value, 0, 30); 
				temp45_code = READFROM_UINT(vdata45.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp45_val = READFROM_UINT(vdata45.value, 0, 30); 
				temp46_code = READFROM_UINT(vdata46.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp46_val = READFROM_UINT(vdata46.value, 0, 30); 
				temp47_code = READFROM_UINT(vdata47.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp47_val = READFROM_UINT(vdata47.value, 0, 30); 
				temp50_code = READFROM_UINT(vdata50.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp50_val = READFROM_UINT(vdata50.value, 0, 30); 
				temp51_code = READFROM_UINT(vdata51.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp51_val = READFROM_UINT(vdata51.value, 0, 30); 
				temp52_code = READFROM_UINT(vdata52.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp52_val = READFROM_UINT(vdata52.value, 0, 30); 
				temp53_code = READFROM_UINT(vdata53.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp53_val = READFROM_UINT(vdata53.value, 0, 30); 
				temp54_code = READFROM_UINT(vdata54.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp54_val = READFROM_UINT(vdata54.value, 0, 30); 
				temp55_code = READFROM_UINT(vdata55.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp55_val = READFROM_UINT(vdata55.value, 0, 30); 
				temp56_code = READFROM_UINT(vdata56.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp56_val = READFROM_UINT(vdata56.value, 0, 30); 
				temp57_code = READFROM_UINT(vdata57.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp57_val = READFROM_UINT(vdata57.value, 0, 30); 
				temp60_code = READFROM_UINT(vdata60.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp60_val = READFROM_UINT(vdata60.value, 0, 30); 
				temp61_code = READFROM_UINT(vdata61.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp61_val = READFROM_UINT(vdata61.value, 0, 30); 
				temp62_code = READFROM_UINT(vdata62.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp62_val = READFROM_UINT(vdata62.value, 0, 30); 
				temp63_code = READFROM_UINT(vdata63.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp63_val = READFROM_UINT(vdata63.value, 0, 30); 
				temp64_code = READFROM_UINT(vdata64.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp64_val = READFROM_UINT(vdata64.value, 0, 30); 
				temp65_code = READFROM_UINT(vdata65.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp65_val = READFROM_UINT(vdata65.value, 0, 30); 
				temp66_code = READFROM_UINT(vdata66.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp66_val = READFROM_UINT(vdata66.value, 0, 30); 
				temp67_code = READFROM_UINT(vdata67.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp67_val = READFROM_UINT(vdata67.value, 0, 30); 
				temp70_code = READFROM_UINT(vdata70.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp70_val = READFROM_UINT(vdata70.value, 0, 30); 
				temp71_code = READFROM_UINT(vdata71.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp71_val = READFROM_UINT(vdata71.value, 0, 30); 
				temp72_code = READFROM_UINT(vdata72.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp72_val = READFROM_UINT(vdata72.value, 0, 30); 
				temp73_code = READFROM_UINT(vdata73.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp73_val = READFROM_UINT(vdata73.value, 0, 30); 
				temp74_code = READFROM_UINT(vdata74.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp74_val = READFROM_UINT(vdata74.value, 0, 30); 
				temp75_code = READFROM_UINT(vdata75.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp75_val = READFROM_UINT(vdata75.value, 0, 30); 
				temp76_code = READFROM_UINT(vdata76.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp76_val = READFROM_UINT(vdata76.value, 0, 30); 
				temp77_code = READFROM_UINT(vdata77.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp77_val = READFROM_UINT(vdata77.value, 0, 30); 
				temp80_code = READFROM_UINT(vdata80.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp80_val = READFROM_UINT(vdata80.value, 0, 30); 
				temp81_code = READFROM_UINT(vdata81.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp81_val = READFROM_UINT(vdata81.value, 0, 30); 
				temp82_code = READFROM_UINT(vdata82.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp82_val = READFROM_UINT(vdata82.value, 0, 30); 
				temp83_code = READFROM_UINT(vdata83.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp83_val = READFROM_UINT(vdata83.value, 0, 30); 
				temp84_code = READFROM_UINT(vdata84.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp84_val = READFROM_UINT(vdata84.value, 0, 30); 
				temp85_code = READFROM_UINT(vdata85.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp85_val = READFROM_UINT(vdata85.value, 0, 30); 
				temp86_code = READFROM_UINT(vdata86.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp86_val = READFROM_UINT(vdata86.value, 0, 30); 
				temp87_code = READFROM_UINT(vdata87.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp87_val = READFROM_UINT(vdata87.value, 0, 30); 
				temp90_code = READFROM_UINT(vdata90.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp90_val = READFROM_UINT(vdata90.value, 0, 30); 
				temp91_code = READFROM_UINT(vdata91.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp91_val = READFROM_UINT(vdata91.value, 0, 30); 
				temp92_code = READFROM_UINT(vdata92.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp92_val = READFROM_UINT(vdata92.value, 0, 30); 
				temp93_code = READFROM_UINT(vdata93.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp93_val = READFROM_UINT(vdata93.value, 0, 30); 
				temp94_code = READFROM_UINT(vdata94.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp94_val = READFROM_UINT(vdata94.value, 0, 30); 
				temp95_code = READFROM_UINT(vdata95.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp95_val = READFROM_UINT(vdata95.value, 0, 30); 
				temp96_code = READFROM_UINT(vdata96.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp96_val = READFROM_UINT(vdata96.value, 0, 30); 
				temp97_code = READFROM_UINT(vdata97.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp97_val = READFROM_UINT(vdata97.value, 0, 30); 
				temp100_code = READFROM_UINT(vdata100.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp100_val = READFROM_UINT(vdata100.value, 0, 30); 
				temp101_code = READFROM_UINT(vdata101.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp101_val = READFROM_UINT(vdata101.value, 0, 30); 
				temp102_code = READFROM_UINT(vdata102.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp102_val = READFROM_UINT(vdata102.value, 0, 30); 
				temp103_code = READFROM_UINT(vdata103.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp103_val = READFROM_UINT(vdata103.value, 0, 30); 
				temp104_code = READFROM_UINT(vdata104.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp104_val = READFROM_UINT(vdata104.value, 0, 30); 
				temp105_code = READFROM_UINT(vdata105.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp105_val = READFROM_UINT(vdata105.value, 0, 30); 
				temp106_code = READFROM_UINT(vdata106.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp106_val = READFROM_UINT(vdata106.value, 0, 30); 
				temp107_code = READFROM_UINT(vdata107.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp107_val = READFROM_UINT(vdata107.value, 0, 30); 
				temp110_code = READFROM_UINT(vdata110.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp110_val = READFROM_UINT(vdata110.value, 0, 30); 
				temp111_code = READFROM_UINT(vdata111.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp111_val = READFROM_UINT(vdata111.value, 0, 30); 
				temp112_code = READFROM_UINT(vdata112.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp112_val = READFROM_UINT(vdata112.value, 0, 30); 
				temp113_code = READFROM_UINT(vdata113.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp113_val = READFROM_UINT(vdata113.value, 0, 30); 
				temp114_code = READFROM_UINT(vdata114.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp114_val = READFROM_UINT(vdata114.value, 0, 30); 
				temp115_code = READFROM_UINT(vdata115.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp115_val = READFROM_UINT(vdata115.value, 0, 30); 
				temp116_code = READFROM_UINT(vdata116.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp116_val = READFROM_UINT(vdata116.value, 0, 30); 
				temp117_code = READFROM_UINT(vdata117.value, 30, 2); // FIXME. use range for _WIDEWORD
				temp117_val = READFROM_UINT(vdata117.value, 0, 30); 
			}
				
			code0 =  temp00_code | temp01_code | temp02_code | temp03_code | temp04_code | temp05_code | temp06_code | temp07_code  ;
			code1 =  temp10_code | temp11_code | temp12_code | temp13_code | temp14_code | temp15_code | temp16_code | temp17_code  ;
			code2 =  temp20_code | temp21_code | temp22_code | temp23_code | temp24_code | temp25_code | temp26_code | temp27_code  ;
			code3 =  temp30_code | temp31_code | temp32_code | temp33_code | temp34_code | temp35_code | temp36_code | temp37_code  ;
			code4 =  temp40_code | temp41_code | temp42_code | temp43_code | temp44_code | temp45_code | temp46_code | temp47_code  ;
			code5 =  temp50_code | temp51_code | temp52_code | temp53_code | temp54_code | temp55_code | temp56_code | temp57_code  ;
			code6 =  temp60_code | temp61_code | temp62_code | temp63_code | temp64_code | temp65_code | temp66_code | temp67_code  ;
			code7 =  temp70_code | temp71_code | temp72_code | temp73_code | temp74_code | temp75_code | temp76_code | temp77_code  ;
			code8 =  temp80_code | temp81_code | temp82_code | temp83_code | temp84_code | temp85_code | temp86_code | temp87_code  ;
			code9 =  temp90_code | temp91_code | temp92_code | temp93_code | temp94_code | temp95_code | temp96_code | temp97_code  ;
			code10 =  temp100_code | temp101_code | temp102_code | temp103_code | temp104_code | temp105_code | temp106_code | temp107_code  ;
			code11 =  temp110_code | temp111_code | temp112_code | temp113_code | temp114_code | temp115_code | temp116_code | temp117_code  ;
			code =  code0 | code1 | code2 | code3 | code4 | code5 | code6 | code7 | code8 | code9 | code10 | code11  ;
			
			value_t a0 = amin(temp00_val, temp01_val);
			value_t b0 = amin(temp02_val, temp03_val);
			value_t c0 = amin(temp04_val, temp05_val);
			value_t d0 = amin(temp06_val, temp07_val);
			value_t a1 = amin(temp10_val, temp11_val);
			value_t b1 = amin(temp12_val, temp13_val);
			value_t c1 = amin(temp14_val, temp15_val);
			value_t d1 = amin(temp16_val, temp17_val);
			value_t a2 = amin(temp20_val, temp21_val);
			value_t b2 = amin(temp22_val, temp23_val);
			value_t c2 = amin(temp24_val, temp25_val);
			value_t d2 = amin(temp26_val, temp27_val);
			value_t a3 = amin(temp30_val, temp31_val);
			value_t b3 = amin(temp32_val, temp33_val);
			value_t c3 = amin(temp34_val, temp35_val);
			value_t d3 = amin(temp36_val, temp37_val);
			value_t a4 = amin(temp40_val, temp41_val);
			value_t b4 = amin(temp42_val, temp43_val);
			value_t c4 = amin(temp44_val, temp45_val);
			value_t d4 = amin(temp46_val, temp47_val);
			value_t a5 = amin(temp50_val, temp51_val);
			value_t b5 = amin(temp52_val, temp53_val);
			value_t c5 = amin(temp54_val, temp55_val);
			value_t d5 = amin(temp56_val, temp57_val);
			value_t a6 = amin(temp60_val, temp61_val);
			value_t b6 = amin(temp62_val, temp63_val);
			value_t c6 = amin(temp64_val, temp65_val);
			value_t d6 = amin(temp66_val, temp67_val);
			value_t a7 = amin(temp70_val, temp71_val);
			value_t b7 = amin(temp72_val, temp73_val);
			value_t c7 = amin(temp74_val, temp75_val);
			value_t d7 = amin(temp76_val, temp77_val);
			value_t a8 = amin(temp80_val, temp81_val);
			value_t b8 = amin(temp82_val, temp83_val);
			value_t c8 = amin(temp84_val, temp85_val);
			value_t d8 = amin(temp86_val, temp87_val);
			value_t a9 = amin(temp90_val, temp91_val);
			value_t b9 = amin(temp92_val, temp93_val);
			value_t c9 = amin(temp94_val, temp95_val);
			value_t d9 = amin(temp96_val, temp97_val);
			value_t a10 = amin(temp100_val, temp101_val);
			value_t b10 = amin(temp102_val, temp103_val);
			value_t c10 = amin(temp104_val, temp105_val);
			value_t d10 = amin(temp106_val, temp107_val);
			value_t a11 = amin(temp110_val, temp111_val);
			value_t b11 = amin(temp112_val, temp113_val);
			value_t c11 = amin(temp114_val, temp115_val);
			value_t d11 = amin(temp116_val, temp117_val);
			
			value_t m0 = amin(a0, b0);
			value_t n0 = amin(c0, d0);
			value_t m1 = amin(a1, b1);
			value_t n1 = amin(c1, d1);
			value_t m2 = amin(a2, b2);
			value_t n2 = amin(c2, d2);
			value_t m3 = amin(a3, b3);
			value_t n3 = amin(c3, d3);
			value_t m4 = amin(a4, b4);
			value_t n4 = amin(c4, d4);
			value_t m5 = amin(a5, b5);
			value_t n5 = amin(c5, d5);
			value_t m6 = amin(a6, b6);
			value_t n6 = amin(c6, d6);
			value_t m7 = amin(a7, b7);
			value_t n7 = amin(c7, d7);
			value_t m8 = amin(a8, b8);
			value_t n8 = amin(c8, d8);
			value_t m9 = amin(a9, b9);
			value_t n9 = amin(c9, d9);
			value_t m10 = amin(a10, b10);
			value_t n10 = amin(c10, d10);
			value_t m11 = amin(a11, b11);
			value_t n11 = amin(c11, d11);
			
			value_t v0 = amin(m0, n0);
			value_t v1 = amin(m1, n1);
			value_t v2 = amin(m2, n2);
			value_t v3 = amin(m3, n3);
			value_t v4 = amin(m4, n4);
			value_t v5 = amin(m5, n5);
			value_t v6 = amin(m6, n6);
			value_t v7 = amin(m7, n7);
			value_t v8 = amin(m8, n8);
			value_t v9 = amin(m9, n9);
			value_t v10 = amin(m10, n10);
			value_t v11 = amin(m11, n11);
			
			// value 
			#if NUMCOMPUTEUNITS==1 // AUTOMATEME.
			value_t z = v0;
			#endif 
			
			#if NUMCOMPUTEUNITS==2
			value_t z = amin(v0, v1);
			#endif
			
			#if NUMCOMPUTEUNITS==4
			value_t w0 = amin(v0, v1);
			value_t w1 = amin(v2, v3);
			
			value_t z = amin(w0, w1);
			#endif 
			
			#if NUMCOMPUTEUNITS==12
			value_t w0 = amin(v0, v1);
			value_t w1 = amin(v2, v3);
			value_t w2 = amin(v4, v5);
			value_t w3 = amin(v6, v7);
			value_t w4 = amin(v8, v9);
			value_t w5 = amin(v10, v11);
			
			value_t x0 = amin(w0, w1);
			value_t x1 = amin(w2, w3);
			value_t x2 = amin(w4, w5);
			
			value_t y0 = amin(x0, x1);
			value_t y1 = amin(x2, 0xFFFFFFFF);
			
			value_t z = amin(y0, y1);
			#endif 
			
			#if NUMCOMPUTEUNITS==14
			value_t w0 = amin(v0, v1);
			value_t w1 = amin(v2, v3);
			value_t w2 = amin(v4, v5);
			value_t w3 = amin(v6, v7);
			value_t w4 = amin(v8, v9);
			value_t w5 = amin(v10, v11);
			value_t w6 = amin(v12, v13);
			
			value_t x0 = amin(w0, w1);
			value_t x1 = amin(w2, w3);
			value_t x2 = amin(w4, w5);
			value_t x3 = amin(w6, 0xFFFFFFFF);
			
			value_t y0 = amin(x0, x1);
			value_t y1 = amin(x2, x3);
			
			value_t z = amin(y0, y1);
			#endif 
			
			#if NUMCOMPUTEUNITS==16
			value_t w0 = amin(v0, v1);
			value_t w1 = amin(v2, v3);
			value_t w2 = amin(v4, v5);
			value_t w3 = amin(v6, v7);
			value_t w4 = amin(v8, v9);
			value_t w5 = amin(v10, v11);
			value_t w6 = amin(v12, v13);
			value_t w7 = amin(v14, v15);
			
			value_t x0 = amin(w0, w1);
			value_t x1 = amin(w2, w3);
			value_t x2 = amin(w4, w5);
			value_t x3 = amin(w6, w7);
			
			value_t y0 = amin(x0, x1);
			value_t y1 = amin(x2, x3);
			
			value_t z = amin(y0, y1);
			#endif 
			
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("unifydata_sssp_parallelsyn 5", code, 2, NAp, NAp, NAp);
			#endif
			
			if(code != 0){
				nonzeroactvvsreturned = ON;
				unsigned int voffset = sweepparams.source_partition * globalparams.applyvertexbuffersz;
				unsigned int vid = voffset + i*2 + k;
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("unifydata_sssp_parallelsyn 5", vid, KVDATA_RANGE, NAp, NAp, NAp);
				#endif
				// cout<<"unifydata_sssp_parallelsyn: vid: "<<vid<<endl;
				
				unsigned int rowindex = tempactvvtracker / VECTOR2_SIZE;
				unsigned int colindex = tempactvvtracker % VECTOR2_SIZE;
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("unifydata_sssp_parallelsyn 6", colindex / 2, VECTOR_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("unifydata_sssp_parallelsyn 7", rowindex / 2, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
				#endif
				if(tempactvvtracker % 2 == 0){ actvvs[colindex / 2][rowindex / 2].key = vid; }
				else { actvvs[colindex / 2][rowindex / 2].value = vid; }
				tempactvvtracker += 1;
			}
			
			WRITETO_UINT(&z, 30, 2, INACTIVEINNEXTITERATION); // FIXME. use range for _WIDEWORD
			
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("unifydata_sssp_parallelsyn 8", dest_v, VECTOR_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("unifydata_sssp_parallelsyn 9", destoffset_kvs + dest_i, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			#endif
			if(k==0){ 
				destbuffer[dest_v][destoffset_kvs + dest_i].key = z; 
			} else {
				destbuffer[dest_v][destoffset_kvs + dest_i].value = z;
			}
		}
		
		dest_v+=1; if(dest_v == VECTOR_SIZE){ dest_v=0; dest_i+=1; }
	}
	
	*actvvtracker = tempactvvtracker;
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
		travstate_t actvvstravstate,
		unsigned int local_source_partition
		){
	// NOTE: CRITICAL AUTOMATEME: LOADFACTORFORREDUCE
	#pragma HLS INLINE
	analysis_type analysis_reduceloop = KVDATA_BATCHSIZE_KVS / SRCBUFFER_SIZE;
	if(enable == OFF){ return actvvstravstate; }
	
	unsigned int reducesubchunksz = (globalparams.applyvertexbuffersz / VDATAPACKINGFACTOR) / 2;
	unsigned int reducechunksz_kvs = (reducesubchunksz * LOADFACTORFORREDUCE) / VECTOR_SIZE; // NOT USED.

	// if((sweepparams.source_partition % LOADFACTORFORREDUCE) == 0){ readkeyvalues(config.enablereduce, kvdram, verticesbuffer, (globalparams.baseoffset_verticesdata_kvs + (sweepparams.source_partition * ((globalparams.applyvertexbuffersz_kvs / VDATAPACKINGFACTOR) / 2))), PADDEDDESTBUFFER_SIZE); }
	// replicatedata(config.enablereduce, verticesbuffer, tempverticesbuffer, (sweepparams.source_partition % LOADFACTORFORREDUCE) * ((globalparams.applyvertexbuffersz / VDATAPACKINGFACTOR) / 2), ((globalparams.applyvertexbuffersz / VDATAPACKINGFACTOR) / 2));
	
	if(local_source_partition == 0){ readkeyvalues(config.enablereduce, kvdram, verticesbuffer, (globalparams.baseoffset_verticesdata_kvs + (sweepparams.source_partition * ((globalparams.applyvertexbuffersz_kvs / VDATAPACKINGFACTOR) / 2))), PADDEDDESTBUFFER_SIZE); }
	replicatedata(config.enablereduce, verticesbuffer, tempverticesbuffer, local_source_partition * reducesubchunksz, reducesubchunksz);
	
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
	
	// unifydata(config.enablereduce, tempverticesbuffer, verticesbuffer, (sweepparams.source_partition % LOADFACTORFORREDUCE) * ((globalparams.applyvertexbuffersz / VDATAPACKINGFACTOR) / 2), ((globalparams.applyvertexbuffersz / VDATAPACKINGFACTOR) / 2), globalparams.GraphAlgo);
	// if((sweepparams.source_partition % LOADFACTORFORREDUCE) == LOADFACTORFORREDUCE-1){ savekeyvalues(config.enablereduce, kvdram, verticesbuffer, (globalparams.baseoffset_verticesdata_kvs + ((sweepparams.source_partition - (LOADFACTORFORREDUCE-1)) * ((globalparams.applyvertexbuffersz_kvs / VDATAPACKINGFACTOR) / 2))), PADDEDDESTBUFFER_SIZE); }
	
	unifydata(config.enablereduce, tempverticesbuffer, verticesbuffer, local_source_partition * reducesubchunksz, reducesubchunksz, globalparams.GraphAlgo);
	if(local_source_partition == globalparams.loadfactorforreduce-1){ savekeyvalues(config.enablereduce, kvdram, verticesbuffer, (globalparams.baseoffset_verticesdata_kvs + ((sweepparams.source_partition - (LOADFACTORFORREDUCE-1)) * ((globalparams.applyvertexbuffersz_kvs / VDATAPACKINGFACTOR) / 2))), PADDEDDESTBUFFER_SIZE); }
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
generateoffsets_random(
			uint512_dt * kvdram, 
			keyvalue_t actvvs[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
			keyvalue_t offsets[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
			travstate_t actvvtravstate, 
			globalparams_t globalparams,
			batch_type actvvsoffset_kvs){ 
	#pragma HLS function_instantiate variable=kvdram 
	unsigned int analysis_loop1 = (PADDEDDESTBUFFER_SIZE / 2) * VECTOR_SIZE;
	buffer_type chunk_size = getchunksize_kvs(PADDEDDESTBUFFER_SIZE / 2, actvvtravstate, 0);
	
	uint512_vec_dt kvset1;
	uint512_vec_dt kvset2;
	#pragma HLS ARRAY_PARTITION variable=kvset1 complete
	#pragma HLS ARRAY_PARTITION variable=kvset2 complete
	unsigned int numactvvs = 0;
	unsigned int cachednumactvvs = 0;

	#ifdef HYBRIDACCESSMODE
	if(globalparams.actvvsize > gethybridthreshold(globalparams)){
		cachednumactvvs = generateoffsets_stream(
				kvdram, 
				actvvs,
				offsets,
				actvvtravstate, 
				globalparams,
				actvvsoffset_kvs);
		return cachednumactvvs;
	}
	#endif
	
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
		if(actvv_id != INVALIDDATA){ cout<<"generateoffsets_random:: index: "<<index<<", active vid: "<<actvv_id<<", sourcedata: "<<sourcedata<<", num actvvs: "<<chunk_size * VECTOR_SIZE<<", num actvvs processed: "<<((actvvtravstate.i_kvs * VECTOR_SIZE) + index)<<endl; }
		#endif
	
		if((actvv_id != INVALIDDATA) && ((actvvtravstate.i_kvs * VECTOR_SIZE) + index < globalparams.actvvsize)){ // NEWCHANGE.
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("generateoffsets_random.actvv_id 5", actvv_id, KVDATA_RANGE, (actvvtravstate.i_kvs * VECTOR_SIZE) + index, NAp, NAp);
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
			cout<<"generateoffsets_random:generate offsets: edges_beginoffset: "<<edges_beginoffset<<", index: "<<index<<", active vid: "<<actvv_id<<", num actvvs: "<<chunk_size * VECTOR_SIZE<<endl;
			#endif
			unsigned int X = numactvvs % VECTOR_SIZE;
			unsigned int Y = numactvvs / VECTOR_SIZE;
			offsets[X][Y].key = edges_beginoffset;
			offsets[X][Y].value = edges_size;
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"generateoffsets_random:: offsets["<<X<<"]["<<Y<<"].key: "<<offsets[X][Y].key<<", offsets["<<X<<"]["<<Y<<"].value: "<<offsets[X][Y].value<<endl;
			#endif 
			numactvvs += 1;
		}
	}
	return numactvvs;		
}

buffer_type
	#ifdef SW 
	acts::
	#endif
generateoffsets_stream(
			uint512_dt * kvdram, 
			keyvalue_t vptrs[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
			keyvalue_t offsets[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
			travstate_t actvvtravstate, 
			globalparams_t globalparams,
			batch_type actvvsoffset_kvs){ 
	#pragma HLS INLINE //???
	unsigned int analysis_loop1 = (PADDEDDESTBUFFER_SIZE / 2) * VECTOR_SIZE;
	buffer_type chunk_size = getchunksize_kvs(PADDEDDESTBUFFER_SIZE / 2, actvvtravstate, 0);

	unsigned int cachenumactvvs = 0;
	readkeyvalues(ON, kvdram, vptrs, globalparams.baseoffset_vertexptr_kvs + actvvsoffset_kvs, chunk_size + 1);
	
	vertex_t U = vptrs[0][0].key;
	vertex_t V;
	GENERATEOFFSETSSTREAM_LOOP: for(batch_type index=1; index<(chunk_size * VECTOR_SIZE * 2) + 1; index++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loop1 avg=analysis_loop1
	#pragma HLS PIPELINE II=2
		if((actvvtravstate.i_kvs * VECTOR_SIZE) + index - 1 < globalparams.actvvsize){
			unsigned int col = (index / 2) % VECTOR_SIZE;
			unsigned int row = (index / 2) / VECTOR_SIZE;
			
			if(index % 2 == 0){ V = vptrs[col][row].key; }
			else { V = vptrs[col][row].value; }
			
			unsigned int Xcached = cachenumactvvs % VECTOR_SIZE;
			unsigned int Ycached = cachenumactvvs / VECTOR_SIZE;
			offsets[Xcached][Ycached].key = U; 
			offsets[Xcached][Ycached].value = V - U; 
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"generateoffsets_stream:: index: "<<index<<", offsets["<<Xcached<<"]["<<Ycached<<"].key: "<<offsets[Xcached][Ycached].key<<", offsets["<<Xcached<<"]["<<Ycached<<"].value: "<<offsets[Xcached][Ycached].value<<endl;
			#endif 
			cachenumactvvs += 1;
			
			U = V;
		}
	}
	return cachenumactvvs;
}

void
	#ifdef SW 
	acts::
	#endif
processoffsets(
		uint512_dt * kvdram,
		keyvalue_t offsets[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t kvbuffer1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		#ifndef COMPACTEDGES
		keyvalue_t kvbuffer2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		#endif
		globalparams_t globalparams,
		batch_type offset_kvs,
		buffer_type actvvscount,
		travstate_t actvvtravstate,
		batch_type * _saveoffset_kvs,
		batch_type * _cachebeginoffset_kvs
		#ifdef _DEBUGMODE_STATS
		,unsigned int * _edges_count
		,unsigned int * _edgesdstv_sum
		#endif
		){
	#pragma HLS function_instantiate variable=kvdram 
	analysis_type analysis_loop1 = 1;
	batch_type saveoffset_kvs = *_saveoffset_kvs;
	batch_type cachebeginoffset_kvs = *_cachebeginoffset_kvs;
	#ifdef _DEBUGMODE_STATS
	batch_type edges_count = *_edges_count;
	batch_type edgesdstv_sum = *_edgesdstv_sum;
	#endif 
	
	value_t buffersize_kvs = 0; 
	#ifdef HYBRIDACCESSMODE
	keyvalue_t edgescache[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#endif 
	uint512_vec_dt E;
	#pragma HLS ARRAY_PARTITION variable=E complete
	uint512_vec_dt Ecached;
	#pragma HLS ARRAY_PARTITION variable=Ecached complete
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
	value_t updateval = 1;
	#ifdef _DEBUGMODE_STATS
	keyy_t keys[COMPACTPARAM_ITEMSIZE_TOTALDATA];
	#endif
			
	PROCOFFSETS_LOOP1B: for(batch_type actvv_index=0; actvv_index<actvvscount; actvv_index++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loop1 avg=analysis_loop1
		#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_PROCACTVVSPRINTS)
		cout<<"processactivevertices_noncompactedges:generate vertex updates: actvv_index: "<<actvv_index<<", num actvvs: "<<actvvscount<<endl;
		#endif
		
		edge_t edges_beginoffset = offsets[actvv_index % VECTOR_SIZE][actvv_index / VECTOR_SIZE].key;
		batch_type edges_size = offsets[actvv_index % VECTOR_SIZE][actvv_index / VECTOR_SIZE].value;
		edge_t edges_endoffset = edges_beginoffset + edges_size;
		
		#ifdef COMPACTEDGES
		batch_type edgesbegin_kvs = edges_beginoffset / VECTOR_SIZE;
		batch_type edgesize_kvs = (allignhigher_KV(edges_endoffset) - allignlower_KV(edges_beginoffset)) / VECTOR_SIZE;
		#else 
		batch_type edgesbegin_kvs = edges_beginoffset / VECTOR2_SIZE;
		batch_type edgesize_kvs = (allignhigherto16_KV(edges_endoffset) - allignlowerto16_KV(edges_beginoffset)) / VECTOR2_SIZE;
		#endif
		if((offset_kvs * VECTOR_SIZE) + actvv_index >= globalparams.actvvsize){ edgesize_kvs = 0; }
		batch_type edgesend_kvs = edgesbegin_kvs + edgesize_kvs;
		batch_type edgeid_kvs = edgesbegin_kvs;
			
		#ifdef COMPACTEDGES
		vector_type colstart = edges_beginoffset % VECTOR_SIZE;
		vector_type colend = (edges_endoffset-1) % VECTOR_SIZE;
		#else 
		vector_type colstart = edges_beginoffset % VECTOR2_SIZE;
		vector_type colend = (edges_endoffset-1) % VECTOR2_SIZE;
		#endif 
		
		edge_t edgesread_kvs = 0;
		batch_type workedgesize_kvs = edgesize_kvs;
		batch_type workedgesbegin_kvs = edgesbegin_kvs;
		if(buffersize_kvs + workedgesize_kvs > PADDEDDESTBUFFER_SIZE){ workedgesize_kvs = PADDEDDESTBUFFER_SIZE - buffersize_kvs; }
		#ifdef _DEBUGMODE_CHECKS2
		unsigned int errcount = 0;
		#endif
		
		#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_PROCACTVVSPRINTS)
		cout<<"processoffsets:processactivevertices_noncompactedges: edges_beginoffset: "<<edges_beginoffset<<", actvv_index: "<<actvv_index<<endl;
		#endif
			
		#ifdef COMPACTEDGES
		PROCOFFSETS_LOOP1C: while(true){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loop1c avg=analysis_loop1c
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("workedgesize_kvs 45", workedgesize_kvs, PADDEDDESTBUFFER_SIZE+1, NAp, NAp, NAp);
			#endif 
			
			#ifdef HYBRIDACCESSMODE
			if((globalparams.actvvsize > gethybridthreshold(globalparams)) && (buffersize_kvs == 0)){ 
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"processoffsets:: read next chunk from kvdram @: beginoffset: "<<workedgesbegin_kvs<<", endoffset: "<<workedgesbegin_kvs + PADDEDDESTBUFFER_SIZE<<", size: "<<PADDEDDESTBUFFER_SIZE<<endl;
				#endif 
				readkeyvalues(ON, kvdram, edgescache, globalparams.baseoffset_edgesdata_kvs + workedgesbegin_kvs, PADDEDDESTBUFFER_SIZE);
				cachebeginoffset_kvs = workedgesbegin_kvs;
			}
			#endif 
			
			PROCACTVVS_LOOP1D: for(edgeid_kvs=workedgesbegin_kvs; edgeid_kvs<workedgesbegin_kvs + workedgesize_kvs; edgeid_kvs++){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loop1d avg=analysis_loop1d
			#pragma HLS PIPELINE II=1
				#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_PROCACTVVSPRINTS)
				cout<<"processactivevertices_compactedges: edgeid_kvs: "<<edgeid_kvs<<", workedgesbegin_kvs: "<<workedgesbegin_kvs<<", workedgesize_kvs: "<<workedgesize_kvs<<", cachebeginoffset_kvs: "<<cachebeginoffset_kvs<<endl;
				#endif
				
				#ifdef HYBRIDACCESSMODE
				if(globalparams.actvvsize > gethybridthreshold(globalparams)){
					for(vector_type v=0; v<VECTOR_SIZE; v++){ E.data[v] = edgescache[v][edgeid_kvs - cachebeginoffset_kvs]; }
				} else { E = getkeyvalues(kvdram, globalparams.baseoffset_edgesdata_kvs + edgeid_kvs); }
				#else 
				E = getkeyvalues(kvdram, globalparams.baseoffset_edgesdata_kvs + edgeid_kvs);
				#endif
				
				vertexupdate0 = E.data[0];
				vertexupdate1 = E.data[1];
				vertexupdate2 = E.data[2];
				vertexupdate3 = E.data[3];
				vertexupdate4 = E.data[4];
				vertexupdate5 = E.data[5];
				vertexupdate6 = E.data[6];
				vertexupdate7 = E.data[7];
				
				if(((edgeid_kvs == edgesbegin_kvs) && (0 < colstart)) || ((edgeid_kvs == edgesend_kvs-1) && (0 > colend))){ en0 = OFF; }
				else { en0 = ON; } // HYBRIDACCESSMODE. ON becomes a variable which is only on if bit='1'
				if(((edgeid_kvs == edgesbegin_kvs) && (1 < colstart)) || ((edgeid_kvs == edgesend_kvs-1) && (1 > colend))){ en1 = OFF; }
				else { en1 = ON; } // HYBRIDACCESSMODE. ON becomes a variable which is only on if bit='1'
				if(((edgeid_kvs == edgesbegin_kvs) && (2 < colstart)) || ((edgeid_kvs == edgesend_kvs-1) && (2 > colend))){ en2 = OFF; }
				else { en2 = ON; } // HYBRIDACCESSMODE. ON becomes a variable which is only on if bit='1'
				if(((edgeid_kvs == edgesbegin_kvs) && (3 < colstart)) || ((edgeid_kvs == edgesend_kvs-1) && (3 > colend))){ en3 = OFF; }
				else { en3 = ON; } // HYBRIDACCESSMODE. ON becomes a variable which is only on if bit='1'
				if(((edgeid_kvs == edgesbegin_kvs) && (4 < colstart)) || ((edgeid_kvs == edgesend_kvs-1) && (4 > colend))){ en4 = OFF; }
				else { en4 = ON; } // HYBRIDACCESSMODE. ON becomes a variable which is only on if bit='1'
				if(((edgeid_kvs == edgesbegin_kvs) && (5 < colstart)) || ((edgeid_kvs == edgesend_kvs-1) && (5 > colend))){ en5 = OFF; }
				else { en5 = ON; } // HYBRIDACCESSMODE. ON becomes a variable which is only on if bit='1'
				if(((edgeid_kvs == edgesbegin_kvs) && (6 < colstart)) || ((edgeid_kvs == edgesend_kvs-1) && (6 > colend))){ en6 = OFF; }
				else { en6 = ON; } // HYBRIDACCESSMODE. ON becomes a variable which is only on if bit='1'
				if(((edgeid_kvs == edgesbegin_kvs) && (7 < colstart)) || ((edgeid_kvs == edgesend_kvs-1) && (7 > colend))){ en7 = OFF; }
				else { en7 = ON; } // HYBRIDACCESSMODE. ON becomes a variable which is only on if bit='1'
				
				if(en0 == ON){ kvbuffer1[0][buffersize_kvs] = vertexupdate0; }
				else { kvbuffer1[0][buffersize_kvs] = dummyvertexupdate; }
				if(en1 == ON){ kvbuffer1[1][buffersize_kvs] = vertexupdate1; }
				else { kvbuffer1[1][buffersize_kvs] = dummyvertexupdate; }
				if(en2 == ON){ kvbuffer1[2][buffersize_kvs] = vertexupdate2; }
				else { kvbuffer1[2][buffersize_kvs] = dummyvertexupdate; }
				if(en3 == ON){ kvbuffer1[3][buffersize_kvs] = vertexupdate3; }
				else { kvbuffer1[3][buffersize_kvs] = dummyvertexupdate; }
				if(en4 == ON){ kvbuffer1[4][buffersize_kvs] = vertexupdate4; }
				else { kvbuffer1[4][buffersize_kvs] = dummyvertexupdate; }
				if(en5 == ON){ kvbuffer1[5][buffersize_kvs] = vertexupdate5; }
				else { kvbuffer1[5][buffersize_kvs] = dummyvertexupdate; }
				if(en6 == ON){ kvbuffer1[6][buffersize_kvs] = vertexupdate6; }
				else { kvbuffer1[6][buffersize_kvs] = dummyvertexupdate; }
				if(en7 == ON){ kvbuffer1[7][buffersize_kvs] = vertexupdate7; }
				else { kvbuffer1[7][buffersize_kvs] = dummyvertexupdate; }
				
				buffersize_kvs += 1;
				// HYBRIDACCESSMODE. only incremented if bit='1'
				
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
				savekeyvalues(ON, kvdram, kvbuffer1, globalparams.baseoffset_kvdram_kvs + saveoffset_kvs, buffersize_kvs);
				saveoffset_kvs += buffersize_kvs;
				buffersize_kvs = 0;
			}
			
			edgesread_kvs += workedgesize_kvs;
			workedgesbegin_kvs += workedgesize_kvs;
			workedgesize_kvs = edgesize_kvs - edgesread_kvs;
			if(buffersize_kvs + workedgesize_kvs > PADDEDDESTBUFFER_SIZE){ workedgesize_kvs = PADDEDDESTBUFFER_SIZE - buffersize_kvs; }
			if(workedgesize_kvs == 0){ break; }
	
			#ifdef _DEBUGMODE_CHECKS2
			if(edgesize_kvs < workedgesize_kvs){ cout<<"processactivevertices_compactedges: ERROR: edgesize_kvs < workedgesize_kvs. exiting..."<<endl; exit(EXIT_FAILURE); }
			if(errcount++ > 64){ cout<<"processactivevertices_compactedges:ERROR: errcount("<<errcount<<") > 312. exiting..."<<endl; exit(EXIT_FAILURE); }
			#endif
		}
		#endif
		#ifndef COMPACTEDGES
		PROCOFFSETS_LOOP1C: while(true){
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("kvbuffer2 45", workedgesize_kvs, PADDEDDESTBUFFER_SIZE+1, NAp, NAp, NAp);
			#endif 
			
			for(edgeid_kvs=workedgesbegin_kvs; edgeid_kvs<workedgesbegin_kvs + workedgesize_kvs; edgeid_kvs++){
			#pragma HLS PIPELINE II=1
				#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_PROCACTVVSPRINTS)
				cout<<"processactivevertices_noncompactedges.for: edgeid_kvs: "<<edgeid_kvs<<", workedgesbegin_kvs: "<<workedgesbegin_kvs<<", workedgesize_kvs: "<<workedgesize_kvs<<endl;
				#endif
				
				E = getkeyvalues(kvdram, globalparams.baseoffset_edgesdata_kvs + edgeid_kvs);
				#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_PROCACTVVSPRINTS)
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
					#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_PROCACTVVSPRINTS)
					cout<<"processactivevertices_noncompactedges: vertexupdate0.key: "<<vertexupdate0.key<<", vertexupdate0.value: "<<vertexupdate0.value<<endl;
					#endif
					kvbuffer1[0][buffersize_kvs] = vertexupdate0; }
				else { kvbuffer1[0][buffersize_kvs] = dummyvertexupdate; }
				if(en1 == ON){ 
					#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_PROCACTVVSPRINTS)
					cout<<"processactivevertices_noncompactedges: vertexupdate1.key: "<<vertexupdate1.key<<", vertexupdate1.value: "<<vertexupdate1.value<<endl;
					#endif
					kvbuffer1[1][buffersize_kvs] = vertexupdate1; }
				else { kvbuffer1[1][buffersize_kvs] = dummyvertexupdate; }
				if(en2 == ON){ 
					#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_PROCACTVVSPRINTS)
					cout<<"processactivevertices_noncompactedges: vertexupdate2.key: "<<vertexupdate2.key<<", vertexupdate2.value: "<<vertexupdate2.value<<endl;
					#endif
					kvbuffer1[2][buffersize_kvs] = vertexupdate2; }
				else { kvbuffer1[2][buffersize_kvs] = dummyvertexupdate; }
				if(en3 == ON){ 
					#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_PROCACTVVSPRINTS)
					cout<<"processactivevertices_noncompactedges: vertexupdate3.key: "<<vertexupdate3.key<<", vertexupdate3.value: "<<vertexupdate3.value<<endl;
					#endif
					kvbuffer1[3][buffersize_kvs] = vertexupdate3; }
				else { kvbuffer1[3][buffersize_kvs] = dummyvertexupdate; }
				if(en4 == ON){ 
					#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_PROCACTVVSPRINTS)
					cout<<"processactivevertices_noncompactedges: vertexupdate4.key: "<<vertexupdate4.key<<", vertexupdate4.value: "<<vertexupdate4.value<<endl;
					#endif
					kvbuffer1[4][buffersize_kvs] = vertexupdate4; }
				else { kvbuffer1[4][buffersize_kvs] = dummyvertexupdate; }
				if(en5 == ON){ 
					#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_PROCACTVVSPRINTS)
					cout<<"processactivevertices_noncompactedges: vertexupdate5.key: "<<vertexupdate5.key<<", vertexupdate5.value: "<<vertexupdate5.value<<endl;
					#endif
					kvbuffer1[5][buffersize_kvs] = vertexupdate5; }
				else { kvbuffer1[5][buffersize_kvs] = dummyvertexupdate; }
				if(en6 == ON){ 
					#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_PROCACTVVSPRINTS)
					cout<<"processactivevertices_noncompactedges: vertexupdate6.key: "<<vertexupdate6.key<<", vertexupdate6.value: "<<vertexupdate6.value<<endl;
					#endif
					kvbuffer1[6][buffersize_kvs] = vertexupdate6; }
				else { kvbuffer1[6][buffersize_kvs] = dummyvertexupdate; }
				if(en7 == ON){ 
					#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_PROCACTVVSPRINTS)
					cout<<"processactivevertices_noncompactedges: vertexupdate7.key: "<<vertexupdate7.key<<", vertexupdate7.value: "<<vertexupdate7.value<<endl;
					#endif
					kvbuffer1[7][buffersize_kvs] = vertexupdate7; }
				else { kvbuffer1[7][buffersize_kvs] = dummyvertexupdate; }
				
				if(e2n0 == ON){ 
					#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_PROCACTVVSPRINTS)
					cout<<"processactivevertices_noncompactedges: vertex2update0.key: "<<vertex2update0.key<<", vertex2update0.value: "<<vertex2update0.value<<endl;
					#endif
					kvbuffer2[0][buffersize_kvs] = vertex2update0; }
				else { kvbuffer2[0][buffersize_kvs] = dummyvertexupdate; }
				if(e2n1 == ON){ 
					#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_PROCACTVVSPRINTS)
					cout<<"processactivevertices_noncompactedges: vertex2update1.key: "<<vertex2update1.key<<", vertex2update1.value: "<<vertex2update1.value<<endl;
					#endif
					kvbuffer2[1][buffersize_kvs] = vertex2update1; }
				else { kvbuffer2[1][buffersize_kvs] = dummyvertexupdate; }
				if(e2n2 == ON){ 
					#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_PROCACTVVSPRINTS)
					cout<<"processactivevertices_noncompactedges: vertex2update2.key: "<<vertex2update2.key<<", vertex2update2.value: "<<vertex2update2.value<<endl;
					#endif
					kvbuffer2[2][buffersize_kvs] = vertex2update2; }
				else { kvbuffer2[2][buffersize_kvs] = dummyvertexupdate; }
				if(e2n3 == ON){ 
					#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_PROCACTVVSPRINTS)
					cout<<"processactivevertices_noncompactedges: vertex2update3.key: "<<vertex2update3.key<<", vertex2update3.value: "<<vertex2update3.value<<endl;
					#endif
					kvbuffer2[3][buffersize_kvs] = vertex2update3; }
				else { kvbuffer2[3][buffersize_kvs] = dummyvertexupdate; }
				if(e2n4 == ON){ 
					#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_PROCACTVVSPRINTS)
					cout<<"processactivevertices_noncompactedges: vertex2update4.key: "<<vertex2update4.key<<", vertex2update4.value: "<<vertex2update4.value<<endl;
					#endif
					kvbuffer2[4][buffersize_kvs] = vertex2update4; }
				else { kvbuffer2[4][buffersize_kvs] = dummyvertexupdate; }
				if(e2n5 == ON){ 
					#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_PROCACTVVSPRINTS)
					cout<<"processactivevertices_noncompactedges: vertex2update5.key: "<<vertex2update5.key<<", vertex2update5.value: "<<vertex2update5.value<<endl;
					#endif
					kvbuffer2[5][buffersize_kvs] = vertex2update5; }
				else { kvbuffer2[5][buffersize_kvs] = dummyvertexupdate; }
				if(e2n6 == ON){ 
					#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_PROCACTVVSPRINTS)
					cout<<"processactivevertices_noncompactedges: vertex2update6.key: "<<vertex2update6.key<<", vertex2update6.value: "<<vertex2update6.value<<endl;
					#endif
					kvbuffer2[6][buffersize_kvs] = vertex2update6; }
				else { kvbuffer2[6][buffersize_kvs] = dummyvertexupdate; }
				if(e2n7 == ON){ 
					#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_PROCACTVVSPRINTS)
					cout<<"processactivevertices_noncompactedges: vertex2update7.key: "<<vertex2update7.key<<", vertex2update7.value: "<<vertex2update7.value<<endl;
					#endif
					kvbuffer2[7][buffersize_kvs] = vertex2update7; }
				else { kvbuffer2[7][buffersize_kvs] = dummyvertexupdate; }
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("kvbuffer2 45", buffersize_kvs, PADDEDDESTBUFFER_SIZE, workedgesbegin_kvs, edgeid_kvs, workedgesize_kvs);
				#endif
				#ifdef _DEBUGMODE_STATS
				#ifdef COMPACTEDGES
				actsutilityobj->globalstats_countkvsprocessed(VECTOR_SIZE);
				#else 
				actsutilityobj->globalstats_countkvsprocessed(VECTOR2_SIZE);
				#endif 
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
			
			if((buffersize_kvs >= PADDEDDESTBUFFER_SIZE) || ((offset_kvs * VECTOR_SIZE) + actvv_index == globalparams.actvvsize-1)){
				#ifdef _DEBUGMODE_KERNELPRINTS2
				cout<<"processactivevertices_noncompactedges: saving keyvalues @ actvv_index("<<actvv_index<<")... saveoffset_kvs: "<<saveoffset_kvs<<", buffersize_kvs: "<<buffersize_kvs<<endl;
				#endif 
				savekeyvalues(ON, kvdram, kvbuffer1, globalparams.baseoffset_kvdram_kvs + saveoffset_kvs, buffersize_kvs);
				savekeyvalues(ON, kvdram, kvbuffer2, globalparams.baseoffset_kvdram_kvs + saveoffset_kvs + buffersize_kvs, buffersize_kvs);
				saveoffset_kvs += 2 * buffersize_kvs;
				buffersize_kvs = 0;
			}
			
			edgesread_kvs += workedgesize_kvs;
			workedgesbegin_kvs += workedgesize_kvs;
			workedgesize_kvs = edgesize_kvs - edgesread_kvs;
			if(buffersize_kvs + workedgesize_kvs > PADDEDDESTBUFFER_SIZE){ workedgesize_kvs = PADDEDDESTBUFFER_SIZE - buffersize_kvs; }
			if(workedgesize_kvs == 0){ break; }
			
			#ifdef _DEBUGMODE_CHECKS2 // FIXME.
			if(edgesize_kvs < workedgesize_kvs){ cout<<"processactivevertices_noncompactedges: ERROR: edgesize_kvs < workedgesize_kvs. exiting..."<<endl; exit(EXIT_FAILURE); }
			if(errcount++ > 64){ cout<<"processactivevertices_noncompactedges:ERROR: errcount("<<errcount<<") > 312. exiting..."<<endl; exit(EXIT_FAILURE); }
			#endif
		}
		#endif 
	}
	
	if(true){ 
		#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_PROCACTVVSPRINTS)
		cout<<"processactivevertices_noncompactedges [end of block]: saving keyvalues @ actvv_index(NAp)... saveoffset_kvs: "<<saveoffset_kvs<<", buffersize_kvs: "<<buffersize_kvs<<endl;
		#endif 
		#ifdef COMPACTEDGES
		savekeyvalues(ON, kvdram, kvbuffer1, globalparams.baseoffset_kvdram_kvs + saveoffset_kvs, buffersize_kvs);
		saveoffset_kvs += buffersize_kvs;
		buffersize_kvs = 0;
		#else 
		savekeyvalues(ON, kvdram, kvbuffer1, globalparams.baseoffset_kvdram_kvs + saveoffset_kvs, buffersize_kvs);
		savekeyvalues(ON, kvdram, kvbuffer2, globalparams.baseoffset_kvdram_kvs + saveoffset_kvs + buffersize_kvs, buffersize_kvs);
		saveoffset_kvs += 2 * buffersize_kvs;
		buffersize_kvs = 0;
		#endif
		
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("processactivevertices_noncompactedges 44", saveoffset_kvs, KVDRAMSZ_KVS, NAp, NAp, NAp);
		#endif
		#ifdef _DEBUGMODE_STATS
		setkey(kvdram, PADDEDKVSOURCEDRAMSZ_KVS-1, 0, edges_count);
		setkey(kvdram, PADDEDKVSOURCEDRAMSZ_KVS-1, 1, edgesdstv_sum);
		setkey(kvdram, PADDEDKVSOURCEDRAMSZ_KVS-1, 2, saveoffset_kvs);
		#endif
	}
		
	*_saveoffset_kvs = saveoffset_kvs;
	*_cachebeginoffset_kvs = cachebeginoffset_kvs;
	#ifdef _DEBUGMODE_STATS
	*_edges_count = edges_count;
	*_edgesdstv_sum = edgesdstv_sum;
	#endif
	return;
}

batch_type
	#ifdef SW 
	acts::
	#endif
processactivevertices(
		bool_type enable,
		uint512_dt * kvdram,
		keyvalue_t actvvs[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t offsets[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		#ifndef COMPACTEDGES
		keyvalue_t buffer2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		#endif 
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
	value_t updateval = 1;
	
	// read active vertices
	PROCACTVVS_LOOP1: for(batch_type offset_kvs=actvvtravstate.begin_kvs; offset_kvs<actvvtravstate.begin_kvs + actvvtravstate.size_kvs; offset_kvs+=PADDEDDESTBUFFER_SIZE / 2){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loop1 avg=analysis_loop1
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"### processactivevertices_noncompactedges: offset_kvs: "<<offset_kvs<<", actvvtravstate.begin_kvs: "<<actvvtravstate.begin_kvs<<", actvvtravstate.size_kvs: "<<actvvtravstate.size_kvs<<endl;
		#endif
		actvvtravstate.i_kvs = offset_kvs;
		
		// get offsets
		readkeyvalues(ON, kvdram, actvvs, globalparams.baseoffset_activevertices_kvs + offset_kvs, PADDEDDESTBUFFER_SIZE / 2, actvvtravstate);
		buffer_type actvvscount = generateoffsets_random(kvdram, actvvs, offsets, actvvtravstate, globalparams, offset_kvs);
		
		PROCACTVVS_LOOP1B: for(batch_type actvv_index=0; actvv_index<actvvscount; actvv_index++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loop1b avg=analysis_loop1b
			#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_PROCACTVVSPRINTS)
			cout<<"processactivevertices_noncompactedges:generate vertex updates: actvv_index: "<<actvv_index<<", num actvvs: "<<actvvscount<<endl;
			#endif
			
			edge_t edges_beginoffset = offsets[actvv_index % VECTOR_SIZE][actvv_index / VECTOR_SIZE].key;
			batch_type edges_size = offsets[actvv_index % VECTOR_SIZE][actvv_index / VECTOR_SIZE].value;
			edge_t edges_endoffset = edges_beginoffset + edges_size;
			
			#ifdef COMPACTEDGES
			batch_type edgesbegin_kvs = edges_beginoffset / VECTOR_SIZE;
			batch_type edgesize_kvs = (allignhigher_KV(edges_endoffset) - allignlower_KV(edges_beginoffset)) / VECTOR_SIZE;
			#else 
			batch_type edgesbegin_kvs = edges_beginoffset / VECTOR2_SIZE;
			batch_type edgesize_kvs = (allignhigherto16_KV(edges_endoffset) - allignlowerto16_KV(edges_beginoffset)) / VECTOR2_SIZE;
			#endif
			if((offset_kvs * VECTOR_SIZE) + actvv_index >= globalparams.actvvsize){ edgesize_kvs = 0; }
			batch_type edgesend_kvs = edgesbegin_kvs + edgesize_kvs;
			batch_type edgeid_kvs = edgesbegin_kvs;
				
			#ifdef COMPACTEDGES
			vector_type colstart = edges_beginoffset % VECTOR_SIZE;
			vector_type colend = (edges_endoffset-1) % VECTOR_SIZE;
			#else 
			vector_type colstart = edges_beginoffset % VECTOR2_SIZE;
			vector_type colend = (edges_endoffset-1) % VECTOR2_SIZE;
			#endif 
			
			edge_t edgesread_kvs = 0;
			batch_type workedgesize_kvs = edgesize_kvs;
			batch_type workedgesbegin_kvs = edgesbegin_kvs;
			if(buffersize_kvs + workedgesize_kvs > PADDEDDESTBUFFER_SIZE){ workedgesize_kvs = PADDEDDESTBUFFER_SIZE - buffersize_kvs; }
			#ifdef _DEBUGMODE_CHECKS2
			unsigned int errcount = 0;
			#endif
			
			#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_PROCACTVVSPRINTS)
			cout<<"generateoffsets_random:processactivevertices_noncompactedges: edges_beginoffset: "<<edges_beginoffset<<", actvv_index: "<<actvv_index<<endl;
			#endif
				
			#ifdef COMPACTEDGES
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
			#endif 
			
			#ifndef COMPACTEDGES
			PROCACTVVS_LOOP1C: while(true){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("buffer2 45", workedgesize_kvs, PADDEDDESTBUFFER_SIZE+1, NAp, NAp, NAp);
				#endif 
				
				for(edgeid_kvs=workedgesbegin_kvs; edgeid_kvs<workedgesbegin_kvs + workedgesize_kvs; edgeid_kvs++){
				#pragma HLS PIPELINE II=1
					#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_PROCACTVVSPRINTS)
					cout<<"processactivevertices_noncompactedges.for: edgeid_kvs: "<<edgeid_kvs<<", workedgesbegin_kvs: "<<workedgesbegin_kvs<<", workedgesize_kvs: "<<workedgesize_kvs<<endl;
					#endif
					
					E = getkeyvalues(kvdram, globalparams.baseoffset_edgesdata_kvs + edgeid_kvs);
					#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_PROCACTVVSPRINTS)
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
						#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_PROCACTVVSPRINTS)
						cout<<"processactivevertices_noncompactedges: vertexupdate0.key: "<<vertexupdate0.key<<", vertexupdate0.value: "<<vertexupdate0.value<<endl;
						#endif
						buffer1[0][buffersize_kvs] = vertexupdate0; }
					else { buffer1[0][buffersize_kvs] = dummyvertexupdate; }
					if(en1 == ON){ 
						#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_PROCACTVVSPRINTS)
						cout<<"processactivevertices_noncompactedges: vertexupdate1.key: "<<vertexupdate1.key<<", vertexupdate1.value: "<<vertexupdate1.value<<endl;
						#endif
						buffer1[1][buffersize_kvs] = vertexupdate1; }
					else { buffer1[1][buffersize_kvs] = dummyvertexupdate; }
					if(en2 == ON){ 
						#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_PROCACTVVSPRINTS)
						cout<<"processactivevertices_noncompactedges: vertexupdate2.key: "<<vertexupdate2.key<<", vertexupdate2.value: "<<vertexupdate2.value<<endl;
						#endif
						buffer1[2][buffersize_kvs] = vertexupdate2; }
					else { buffer1[2][buffersize_kvs] = dummyvertexupdate; }
					if(en3 == ON){ 
						#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_PROCACTVVSPRINTS)
						cout<<"processactivevertices_noncompactedges: vertexupdate3.key: "<<vertexupdate3.key<<", vertexupdate3.value: "<<vertexupdate3.value<<endl;
						#endif
						buffer1[3][buffersize_kvs] = vertexupdate3; }
					else { buffer1[3][buffersize_kvs] = dummyvertexupdate; }
					if(en4 == ON){ 
						#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_PROCACTVVSPRINTS)
						cout<<"processactivevertices_noncompactedges: vertexupdate4.key: "<<vertexupdate4.key<<", vertexupdate4.value: "<<vertexupdate4.value<<endl;
						#endif
						buffer1[4][buffersize_kvs] = vertexupdate4; }
					else { buffer1[4][buffersize_kvs] = dummyvertexupdate; }
					if(en5 == ON){ 
						#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_PROCACTVVSPRINTS)
						cout<<"processactivevertices_noncompactedges: vertexupdate5.key: "<<vertexupdate5.key<<", vertexupdate5.value: "<<vertexupdate5.value<<endl;
						#endif
						buffer1[5][buffersize_kvs] = vertexupdate5; }
					else { buffer1[5][buffersize_kvs] = dummyvertexupdate; }
					if(en6 == ON){ 
						#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_PROCACTVVSPRINTS)
						cout<<"processactivevertices_noncompactedges: vertexupdate6.key: "<<vertexupdate6.key<<", vertexupdate6.value: "<<vertexupdate6.value<<endl;
						#endif
						buffer1[6][buffersize_kvs] = vertexupdate6; }
					else { buffer1[6][buffersize_kvs] = dummyvertexupdate; }
					if(en7 == ON){ 
						#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_PROCACTVVSPRINTS)
						cout<<"processactivevertices_noncompactedges: vertexupdate7.key: "<<vertexupdate7.key<<", vertexupdate7.value: "<<vertexupdate7.value<<endl;
						#endif
						buffer1[7][buffersize_kvs] = vertexupdate7; }
					else { buffer1[7][buffersize_kvs] = dummyvertexupdate; }
					
					if(e2n0 == ON){ 
						#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_PROCACTVVSPRINTS)
						cout<<"processactivevertices_noncompactedges: vertex2update0.key: "<<vertex2update0.key<<", vertex2update0.value: "<<vertex2update0.value<<endl;
						#endif
						buffer2[0][buffersize_kvs] = vertex2update0; }
					else { buffer2[0][buffersize_kvs] = dummyvertexupdate; }
					if(e2n1 == ON){ 
						#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_PROCACTVVSPRINTS)
						cout<<"processactivevertices_noncompactedges: vertex2update1.key: "<<vertex2update1.key<<", vertex2update1.value: "<<vertex2update1.value<<endl;
						#endif
						buffer2[1][buffersize_kvs] = vertex2update1; }
					else { buffer2[1][buffersize_kvs] = dummyvertexupdate; }
					if(e2n2 == ON){ 
						#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_PROCACTVVSPRINTS)
						cout<<"processactivevertices_noncompactedges: vertex2update2.key: "<<vertex2update2.key<<", vertex2update2.value: "<<vertex2update2.value<<endl;
						#endif
						buffer2[2][buffersize_kvs] = vertex2update2; }
					else { buffer2[2][buffersize_kvs] = dummyvertexupdate; }
					if(e2n3 == ON){ 
						#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_PROCACTVVSPRINTS)
						cout<<"processactivevertices_noncompactedges: vertex2update3.key: "<<vertex2update3.key<<", vertex2update3.value: "<<vertex2update3.value<<endl;
						#endif
						buffer2[3][buffersize_kvs] = vertex2update3; }
					else { buffer2[3][buffersize_kvs] = dummyvertexupdate; }
					if(e2n4 == ON){ 
						#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_PROCACTVVSPRINTS)
						cout<<"processactivevertices_noncompactedges: vertex2update4.key: "<<vertex2update4.key<<", vertex2update4.value: "<<vertex2update4.value<<endl;
						#endif
						buffer2[4][buffersize_kvs] = vertex2update4; }
					else { buffer2[4][buffersize_kvs] = dummyvertexupdate; }
					if(e2n5 == ON){ 
						#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_PROCACTVVSPRINTS)
						cout<<"processactivevertices_noncompactedges: vertex2update5.key: "<<vertex2update5.key<<", vertex2update5.value: "<<vertex2update5.value<<endl;
						#endif
						buffer2[5][buffersize_kvs] = vertex2update5; }
					else { buffer2[5][buffersize_kvs] = dummyvertexupdate; }
					if(e2n6 == ON){ 
						#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_PROCACTVVSPRINTS)
						cout<<"processactivevertices_noncompactedges: vertex2update6.key: "<<vertex2update6.key<<", vertex2update6.value: "<<vertex2update6.value<<endl;
						#endif
						buffer2[6][buffersize_kvs] = vertex2update6; }
					else { buffer2[6][buffersize_kvs] = dummyvertexupdate; }
					if(e2n7 == ON){ 
						#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_PROCACTVVSPRINTS)
						cout<<"processactivevertices_noncompactedges: vertex2update7.key: "<<vertex2update7.key<<", vertex2update7.value: "<<vertex2update7.value<<endl;
						#endif
						buffer2[7][buffersize_kvs] = vertex2update7; }
					else { buffer2[7][buffersize_kvs] = dummyvertexupdate; }
					
					#ifdef _DEBUGMODE_CHECKS2
					actsutilityobj->checkoutofbounds("buffer2 45", buffersize_kvs, PADDEDDESTBUFFER_SIZE, workedgesbegin_kvs, edgeid_kvs, workedgesize_kvs);
					#endif
					#ifdef _DEBUGMODE_STATS
					#ifdef COMPACTEDGES
					actsutilityobj->globalstats_countkvsprocessed(VECTOR_SIZE);
					#else 
					actsutilityobj->globalstats_countkvsprocessed(VECTOR2_SIZE);
					#endif 
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
				
				if((buffersize_kvs >= PADDEDDESTBUFFER_SIZE) || ((offset_kvs * VECTOR_SIZE) + actvv_index == globalparams.actvvsize-1)){
					#ifdef _DEBUGMODE_KERNELPRINTS2
					cout<<"processactivevertices_noncompactedges: saving keyvalues @ actvv_index("<<actvv_index<<")... saveoffset_kvs: "<<saveoffset_kvs<<", buffersize_kvs: "<<buffersize_kvs<<endl;
					#endif 
					savekeyvalues(ON, kvdram, buffer1, globalparams.baseoffset_kvdram_kvs + saveoffset_kvs, buffersize_kvs);
					savekeyvalues(ON, kvdram, buffer2, globalparams.baseoffset_kvdram_kvs + saveoffset_kvs + buffersize_kvs, buffersize_kvs);
					saveoffset_kvs += 2 * buffersize_kvs;
					buffersize_kvs = 0;
				}
				
				edgesread_kvs += workedgesize_kvs;
				workedgesbegin_kvs += workedgesize_kvs;
				workedgesize_kvs = edgesize_kvs - edgesread_kvs;
				if(buffersize_kvs + workedgesize_kvs > PADDEDDESTBUFFER_SIZE){ workedgesize_kvs = PADDEDDESTBUFFER_SIZE - buffersize_kvs; }
				if(workedgesize_kvs == 0){ break; }
				
				#ifdef _DEBUGMODE_CHECKS2 // FIXME.
				if(edgesize_kvs < workedgesize_kvs){ cout<<"processactivevertices_noncompactedges: ERROR: edgesize_kvs < workedgesize_kvs. exiting..."<<endl; exit(EXIT_FAILURE); }
				if(errcount++ > 64){ cout<<"processactivevertices_noncompactedges:ERROR: errcount("<<errcount<<") > 312. exiting..."<<endl; exit(EXIT_FAILURE); }
				#endif
			}
			#endif 
		}
	}
	
	#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_PROCACTVVSPRINTS)
	cout<<"processactivevertices_noncompactedges: saving keyvalues @ actvv_index(NAp)... saveoffset_kvs: "<<saveoffset_kvs<<", buffersize_kvs: "<<buffersize_kvs<<endl;
	#endif 
	#ifdef COMPACTEDGES
	savekeyvalues(ON, kvdram, buffer1, globalparams.baseoffset_kvdram_kvs + saveoffset_kvs, buffersize_kvs);
	saveoffset_kvs += buffersize_kvs;
	buffersize_kvs = 0;
	#else 
	savekeyvalues(ON, kvdram, buffer1, globalparams.baseoffset_kvdram_kvs + saveoffset_kvs, buffersize_kvs); // THE DIFFERENCE.
	savekeyvalues(ON, kvdram, buffer2, globalparams.baseoffset_kvdram_kvs + saveoffset_kvs + buffersize_kvs, buffersize_kvs);
	saveoffset_kvs += 2 * buffersize_kvs;
	buffersize_kvs = 0;
	#endif 
	
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("processactivevertices_noncompactedges 44", saveoffset_kvs, KVDRAMSZ_KVS, NAp, NAp, NAp);
	#endif
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
		unsigned int local_source_partition = 0;
		unsigned int sourcepartitionblock = 0;
		
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
			
			/* if((source_partition % LOADFACTORFORREDUCE) == 0){
				batch_type ntravszs = 0;
				for(batch_type k=0; k<LOADFACTORFORREDUCE; k++){ ntravszs += moretravstates[k].value; }
				if(ntravszs > 0){ enreduce = ON; } else { enreduce = OFF; }
			} */
			
			if(local_source_partition == 0){
				batch_type ntravszs = 0;
				for(batch_type k=0; k<globalparams.loadfactorforreduce; k++){ ntravszs += moretravstates[k].value; }
				if(ntravszs > 0){ enreduce = ON; } else { enreduce = OFF; }
			}
			
			// local_source_partition += 1;
			// if(local_source_partition >= globalparams.loadfactorforreduce){ local_source_partition = 0; sourcepartitionblock += 1; }
			
			
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
				#ifdef MERGEPROCESSEDGESANDPARTITIONSTAGE
				if(inprocessedgesstage(currentLOP) == true){ saveglobalstats(config.enableprocessedges, kvdram, globalstatsbuffer, globalparams.baseoffset_statsdram_kvs + deststatsmarker); }
				#endif 
			#else
			globalparams.runsize_kvs = 
				processactivevertices(
				config.enableprocessedges,
				kvdram,
				buffer1,
				buffer2,
				buffer3,
				#ifndef COMPACTEDGES
				buffer4,
				#endif 
				avtravstate,
				globalparams
				);
			globalparams.runsize = globalparams.runsize_kvs * VECTOR_SIZE;
			#endif 
			#ifdef EMBEDDEDCOLLECTSTATS
			prepareglobalstats2(config.enableprocessedges, buffer4, globalstatsbuffer, NUM_PARTITIONS, globalparams);
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
				actvvstravstate,
				local_source_partition);
			#endif 
			
			if(currentLOP > 0){
				sourcestatsmarker += 1;
				deststatsmarker += NUM_PARTITIONS;
				destoffset = globalstatsbuffer[NUM_PARTITIONS-1].key + globalstatsbuffer[NUM_PARTITIONS-1].value + 64; }
			local_source_partition += 1;
			if(local_source_partition >= globalparams.loadfactorforreduce){ local_source_partition = 0; sourcepartitionblock += 1; }
			
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
	setkey(kvdram, PADDEDKVSOURCEDRAMSZ_KVS-1, 3, edges_count);
	setkey(kvdram, PADDEDKVSOURCEDRAMSZ_KVS-1, 4, edgesdstv_sum);
	setkey(kvdram, PADDEDKVSOURCEDRAMSZ_KVS-1, 5, actvvstravstate.i);
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"dispatch:: actvvstravstate.i: "<<actvvstravstate.i<<endl;
	cout<<"dispatch:: actvvstravstate.i_kvs: "<<actvvstravstate.i_kvs<<endl;
	cout<<"dispatch:: actvvstravstate.v: "<<actvvstravstate.v<<endl;
	cout<<"dispatch:: actvvstravstate.k: "<<actvvstravstate.k<<endl;
	#endif 
	return;
}

void
	#ifdef SW 
	acts::
	#endif 
dispatch_processandpartitiononly(uint512_dt * kvdram, 
			keyvalue_t buffer1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
			keyvalue_t buffer4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
			keyvalue_t buffer5[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
			globalparams_t globalparams){
	analysis_type analysis_numllops = 1;
	analysis_type analysis_numsourcepartitions = 1;
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printparameters();
	actsutilityobj->printglobalvars();
	actsutilityobj->printglobalparameters("acts::getglobalparams:: printing global parameters", globalparams);
	#endif 
	#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(_DEBUGMODE_CHECKS2)
	actsutilityobj->clearglobalvars();
	#endif
	#ifdef _DEBUGMODE_STATS
	unsigned int edges_count = 0;
	unsigned int edgesdstv_sum = 0;
	#endif
	
	keyvalue_t unused_buffer1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer1
	keyvalue_t buffer2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer2
	keyvalue_t buffer3[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer3
	keyvalue_t unused_buffer4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer4
	keyvalue_t unused_buffer5[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer5
	keyvalue_t buffer6[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE]; // AUTOMATEME. used for pr only?
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
				#ifdef MERGEPROCESSEDGESANDPARTITIONSTAGE
				if(inprocessedgesstage(currentLOP) == true){ saveglobalstats(config.enableprocessedges, kvdram, globalstatsbuffer, globalparams.baseoffset_statsdram_kvs + deststatsmarker); }
				#endif 
			#else
			globalparams.runsize_kvs = 
			processactivevertices(
				config.enableprocessedges,
				kvdram,
				buffer1,
				buffer2,
				buffer3,
				#ifndef COMPACTEDGES
				buffer4,
				#endif
				avtravstate,
				globalparams
				);
			globalparams.runsize = globalparams.runsize_kvs * VECTOR_SIZE;
			#endif 
			#ifdef EMBEDDEDCOLLECTSTATS // FIXME. EMBEDDEDCOLLECTSTATS is DEPRECIATED.
			prepareglobalstats2(config.enableprocessedges, buffer4, globalstatsbuffer, NUM_PARTITIONS, globalparams);
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
dispatch_processonly(
			uint512_dt * vdram,
uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11,keyvalue_t offsets0[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t offsets1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t offsets2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t offsets3[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t offsets4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t offsets5[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t offsets6[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t offsets7[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t offsets8[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t offsets9[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t offsets10[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t offsets11[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer10[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer11[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer12[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer13[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer14[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer15[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer16[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer17[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer18[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer19[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer110[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer111[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],	
			#ifndef COMPACTEDGES	
keyvalue_t kvbuffer20[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer21[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer22[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer23[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer24[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer25[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer26[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer27[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer28[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer29[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer210[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer211[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],		
			#endif 	
			globalparams_t globalparams[NUMCOMPUTEUNITS], batch_type * _sourcestatsmarker, batch_type * _deststatsmarker, batch_type * _destoffset){
	analysis_type analysis_numllops = 1;
	analysis_type analysis_numsourcepartitions = 1;
	
	batch_type sourcestatsmarker = *_sourcestatsmarker;
	batch_type deststatsmarker = *_deststatsmarker;
	batch_type destoffset = *_destoffset;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printparameters();
	actsutilityobj->printglobalvars();
	actsutilityobj->printglobalparameters("acts::getglobalparams:: printing global parameters", globalparams0);
	#endif 
	#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(_DEBUGMODE_CHECKS2)
	actsutilityobj->clearglobalvars();
	#endif
	
	keyvalue_t actvvs0[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = actvvs0
	keyvalue_t unused_offsets0[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_offsets0
	keyvalue_t unused_kvbuffer10[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_kvbuffer10
	keyvalue_t unused_buffer40[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer40
	keyvalue_t unused_buffer50[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer50
	keyvalue_t unused_buffer60[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer60
	keyvalue_t unused_buffer70[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer70
	keyvalue_t actvvs1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = actvvs1
	keyvalue_t unused_offsets1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_offsets1
	keyvalue_t unused_kvbuffer11[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_kvbuffer11
	keyvalue_t unused_buffer41[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer41
	keyvalue_t unused_buffer51[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer51
	keyvalue_t unused_buffer61[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer61
	keyvalue_t unused_buffer71[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer71
	keyvalue_t actvvs2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = actvvs2
	keyvalue_t unused_offsets2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_offsets2
	keyvalue_t unused_kvbuffer12[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_kvbuffer12
	keyvalue_t unused_buffer42[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer42
	keyvalue_t unused_buffer52[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer52
	keyvalue_t unused_buffer62[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer62
	keyvalue_t unused_buffer72[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer72
	keyvalue_t actvvs3[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = actvvs3
	keyvalue_t unused_offsets3[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_offsets3
	keyvalue_t unused_kvbuffer13[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_kvbuffer13
	keyvalue_t unused_buffer43[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer43
	keyvalue_t unused_buffer53[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer53
	keyvalue_t unused_buffer63[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer63
	keyvalue_t unused_buffer73[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer73
	keyvalue_t actvvs4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = actvvs4
	keyvalue_t unused_offsets4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_offsets4
	keyvalue_t unused_kvbuffer14[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_kvbuffer14
	keyvalue_t unused_buffer44[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer44
	keyvalue_t unused_buffer54[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer54
	keyvalue_t unused_buffer64[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer64
	keyvalue_t unused_buffer74[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer74
	keyvalue_t actvvs5[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = actvvs5
	keyvalue_t unused_offsets5[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_offsets5
	keyvalue_t unused_kvbuffer15[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_kvbuffer15
	keyvalue_t unused_buffer45[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer45
	keyvalue_t unused_buffer55[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer55
	keyvalue_t unused_buffer65[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer65
	keyvalue_t unused_buffer75[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer75
	keyvalue_t actvvs6[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = actvvs6
	keyvalue_t unused_offsets6[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_offsets6
	keyvalue_t unused_kvbuffer16[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_kvbuffer16
	keyvalue_t unused_buffer46[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer46
	keyvalue_t unused_buffer56[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer56
	keyvalue_t unused_buffer66[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer66
	keyvalue_t unused_buffer76[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer76
	keyvalue_t actvvs7[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = actvvs7
	keyvalue_t unused_offsets7[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_offsets7
	keyvalue_t unused_kvbuffer17[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_kvbuffer17
	keyvalue_t unused_buffer47[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer47
	keyvalue_t unused_buffer57[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer57
	keyvalue_t unused_buffer67[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer67
	keyvalue_t unused_buffer77[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer77
	keyvalue_t actvvs8[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = actvvs8
	keyvalue_t unused_offsets8[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_offsets8
	keyvalue_t unused_kvbuffer18[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_kvbuffer18
	keyvalue_t unused_buffer48[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer48
	keyvalue_t unused_buffer58[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer58
	keyvalue_t unused_buffer68[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer68
	keyvalue_t unused_buffer78[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer78
	keyvalue_t actvvs9[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = actvvs9
	keyvalue_t unused_offsets9[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_offsets9
	keyvalue_t unused_kvbuffer19[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_kvbuffer19
	keyvalue_t unused_buffer49[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer49
	keyvalue_t unused_buffer59[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer59
	keyvalue_t unused_buffer69[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer69
	keyvalue_t unused_buffer79[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer79
	keyvalue_t actvvs10[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = actvvs10
	keyvalue_t unused_offsets10[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_offsets10
	keyvalue_t unused_kvbuffer110[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_kvbuffer110
	keyvalue_t unused_buffer410[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer410
	keyvalue_t unused_buffer510[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer510
	keyvalue_t unused_buffer610[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer610
	keyvalue_t unused_buffer710[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer710
	keyvalue_t actvvs11[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = actvvs11
	keyvalue_t unused_offsets11[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_offsets11
	keyvalue_t unused_kvbuffer111[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_kvbuffer111
	keyvalue_t unused_buffer411[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer411
	keyvalue_t unused_buffer511[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer511
	keyvalue_t unused_buffer611[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer611
	keyvalue_t unused_buffer711[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer711
	
	batch_type saveoffset_kvs[NUMCOMPUTEUNITS];
	#pragma HLS ARRAY_PARTITION variable=saveoffset_kvs complete
	batch_type cachebeginoffset_kvs[NUMCOMPUTEUNITS];
	#pragma HLS ARRAY_PARTITION variable=cachebeginoffset_kvs complete
	batch_type offsetscachebeginoffset_kvs[NUMCOMPUTEUNITS];
	#pragma HLS ARRAY_PARTITION variable=offsetscachebeginoffset_kvs complete
	#ifdef _DEBUGMODE_STATS
	unsigned int edges_count[NUMCOMPUTEUNITS];
	unsigned int edgesdstv_sum[NUMCOMPUTEUNITS];
	#endif
	
	#ifdef _DEBUGMODE_CHECKS2
	for(unsigned int i=0; i<NUMCOMPUTEUNITS; i++){ if(globalparams[i].runsize >= MAXKVDATA_BATCHSIZE){ cout<<"dispatch:ERROR. runsize too large!. globalparams[i].runsize: "<<globalparams[i].runsize<<", MAXKVDATA_BATCHSIZE: "<<MAXKVDATA_BATCHSIZE<<". EXITING"<<endl; exit(EXIT_FAILURE); }}
	#endif

	step_type currentLOP = globalparams[0].beginLOP;
	travstate_t avtravstate;
	avtravstate.begin_kvs = 0; 
	avtravstate.size_kvs = allignhigherto16_KV(globalparams[0].actvvsize) / (2 * VECTOR_SIZE); avtravstate.end_kvs = avtravstate.begin_kvs + avtravstate.size_kvs; 
	globalparams_t _globalparams = globalparams[0];

	for(unsigned int i=0; i<NUMCOMPUTEUNITS; i++){
		saveoffset_kvs[i] = 0;
		cachebeginoffset_kvs[i] = 0;
		offsetscachebeginoffset_kvs[i] = 0;
		#ifdef _DEBUGMODE_STATS
		edges_count[i] = 0;
		edgesdstv_sum[i] = 0;
		#endif 
	}
	
	// read active vertices
	PROCACTVVS_LOOP1: for(batch_type offset_kvs=avtravstate.begin_kvs; offset_kvs<avtravstate.begin_kvs + avtravstate.size_kvs; offset_kvs+=PADDEDDESTBUFFER_SIZE / 2){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loop1 avg=analysis_loop1
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"### dispatch_processonly: offset_kvs: "<<offset_kvs<<", avtravstate.begin_kvs: "<<avtravstate.begin_kvs<<", avtravstate.size_kvs: "<<avtravstate.size_kvs<<endl;
		#endif
		avtravstate.i_kvs = offset_kvs;
	
		readkeyvalues(ON, vdram, actvvs0, _globalparams.baseoffset_activevertices_kvs + offset_kvs, PADDEDDESTBUFFER_SIZE / 2, avtravstate);
		
		replicatedata_syn(ON, actvvs0 ,actvvs0,actvvs1,actvvs2,actvvs3,actvvs4,actvvs5,actvvs6,actvvs7,actvvs8,actvvs9,actvvs10,actvvs11);
	
		buffer_type actvvscount0 = generateoffsets_random(kvdram0, actvvs0, offsets0, avtravstate, _globalparams, offset_kvs);
		buffer_type actvvscount1 = generateoffsets_random(kvdram1, actvvs1, offsets1, avtravstate, _globalparams, offset_kvs);
		buffer_type actvvscount2 = generateoffsets_random(kvdram2, actvvs2, offsets2, avtravstate, _globalparams, offset_kvs);
		buffer_type actvvscount3 = generateoffsets_random(kvdram3, actvvs3, offsets3, avtravstate, _globalparams, offset_kvs);
		buffer_type actvvscount4 = generateoffsets_random(kvdram4, actvvs4, offsets4, avtravstate, _globalparams, offset_kvs);
		buffer_type actvvscount5 = generateoffsets_random(kvdram5, actvvs5, offsets5, avtravstate, _globalparams, offset_kvs);
		buffer_type actvvscount6 = generateoffsets_random(kvdram6, actvvs6, offsets6, avtravstate, _globalparams, offset_kvs);
		buffer_type actvvscount7 = generateoffsets_random(kvdram7, actvvs7, offsets7, avtravstate, _globalparams, offset_kvs);
		buffer_type actvvscount8 = generateoffsets_random(kvdram8, actvvs8, offsets8, avtravstate, _globalparams, offset_kvs);
		buffer_type actvvscount9 = generateoffsets_random(kvdram9, actvvs9, offsets9, avtravstate, _globalparams, offset_kvs);
		buffer_type actvvscount10 = generateoffsets_random(kvdram10, actvvs10, offsets10, avtravstate, _globalparams, offset_kvs);
		buffer_type actvvscount11 = generateoffsets_random(kvdram11, actvvs11, offsets11, avtravstate, _globalparams, offset_kvs);
 
		
		processoffsets(
			kvdram0,
			offsets0,
			kvbuffer10,
			#ifndef COMPACTEDGES
			kvbuffer20,
			#endif
			_globalparams,
			offset_kvs,
			actvvscount0,
			avtravstate,
			&saveoffset_kvs[0],
			&cachebeginoffset_kvs[0]
			#ifdef _DEBUGMODE_STATS
			,&edges_count[0]
			,&edgesdstv_sum[0]
			#endif
			);
		processoffsets(
			kvdram1,
			offsets1,
			kvbuffer11,
			#ifndef COMPACTEDGES
			kvbuffer21,
			#endif
			_globalparams,
			offset_kvs,
			actvvscount1,
			avtravstate,
			&saveoffset_kvs[1],
			&cachebeginoffset_kvs[1]
			#ifdef _DEBUGMODE_STATS
			,&edges_count[1]
			,&edgesdstv_sum[1]
			#endif
			);
		processoffsets(
			kvdram2,
			offsets2,
			kvbuffer12,
			#ifndef COMPACTEDGES
			kvbuffer22,
			#endif
			_globalparams,
			offset_kvs,
			actvvscount2,
			avtravstate,
			&saveoffset_kvs[2],
			&cachebeginoffset_kvs[2]
			#ifdef _DEBUGMODE_STATS
			,&edges_count[2]
			,&edgesdstv_sum[2]
			#endif
			);
		processoffsets(
			kvdram3,
			offsets3,
			kvbuffer13,
			#ifndef COMPACTEDGES
			kvbuffer23,
			#endif
			_globalparams,
			offset_kvs,
			actvvscount3,
			avtravstate,
			&saveoffset_kvs[3],
			&cachebeginoffset_kvs[3]
			#ifdef _DEBUGMODE_STATS
			,&edges_count[3]
			,&edgesdstv_sum[3]
			#endif
			);
		processoffsets(
			kvdram4,
			offsets4,
			kvbuffer14,
			#ifndef COMPACTEDGES
			kvbuffer24,
			#endif
			_globalparams,
			offset_kvs,
			actvvscount4,
			avtravstate,
			&saveoffset_kvs[4],
			&cachebeginoffset_kvs[4]
			#ifdef _DEBUGMODE_STATS
			,&edges_count[4]
			,&edgesdstv_sum[4]
			#endif
			);
		processoffsets(
			kvdram5,
			offsets5,
			kvbuffer15,
			#ifndef COMPACTEDGES
			kvbuffer25,
			#endif
			_globalparams,
			offset_kvs,
			actvvscount5,
			avtravstate,
			&saveoffset_kvs[5],
			&cachebeginoffset_kvs[5]
			#ifdef _DEBUGMODE_STATS
			,&edges_count[5]
			,&edgesdstv_sum[5]
			#endif
			);
		processoffsets(
			kvdram6,
			offsets6,
			kvbuffer16,
			#ifndef COMPACTEDGES
			kvbuffer26,
			#endif
			_globalparams,
			offset_kvs,
			actvvscount6,
			avtravstate,
			&saveoffset_kvs[6],
			&cachebeginoffset_kvs[6]
			#ifdef _DEBUGMODE_STATS
			,&edges_count[6]
			,&edgesdstv_sum[6]
			#endif
			);
		processoffsets(
			kvdram7,
			offsets7,
			kvbuffer17,
			#ifndef COMPACTEDGES
			kvbuffer27,
			#endif
			_globalparams,
			offset_kvs,
			actvvscount7,
			avtravstate,
			&saveoffset_kvs[7],
			&cachebeginoffset_kvs[7]
			#ifdef _DEBUGMODE_STATS
			,&edges_count[7]
			,&edgesdstv_sum[7]
			#endif
			);
		processoffsets(
			kvdram8,
			offsets8,
			kvbuffer18,
			#ifndef COMPACTEDGES
			kvbuffer28,
			#endif
			_globalparams,
			offset_kvs,
			actvvscount8,
			avtravstate,
			&saveoffset_kvs[8],
			&cachebeginoffset_kvs[8]
			#ifdef _DEBUGMODE_STATS
			,&edges_count[8]
			,&edgesdstv_sum[8]
			#endif
			);
		processoffsets(
			kvdram9,
			offsets9,
			kvbuffer19,
			#ifndef COMPACTEDGES
			kvbuffer29,
			#endif
			_globalparams,
			offset_kvs,
			actvvscount9,
			avtravstate,
			&saveoffset_kvs[9],
			&cachebeginoffset_kvs[9]
			#ifdef _DEBUGMODE_STATS
			,&edges_count[9]
			,&edgesdstv_sum[9]
			#endif
			);
		processoffsets(
			kvdram10,
			offsets10,
			kvbuffer110,
			#ifndef COMPACTEDGES
			kvbuffer210,
			#endif
			_globalparams,
			offset_kvs,
			actvvscount10,
			avtravstate,
			&saveoffset_kvs[10],
			&cachebeginoffset_kvs[10]
			#ifdef _DEBUGMODE_STATS
			,&edges_count[10]
			,&edgesdstv_sum[10]
			#endif
			);
		processoffsets(
			kvdram11,
			offsets11,
			kvbuffer111,
			#ifndef COMPACTEDGES
			kvbuffer211,
			#endif
			_globalparams,
			offset_kvs,
			actvvscount11,
			avtravstate,
			&saveoffset_kvs[11],
			&cachebeginoffset_kvs[11]
			#ifdef _DEBUGMODE_STATS
			,&edges_count[11]
			,&edgesdstv_sum[11]
			#endif
			);
 
	}
	
	for(unsigned int i=0; i<NUMCOMPUTEUNITS; i++){ globalparams[i].runsize_kvs = saveoffset_kvs[i]; }
	for(unsigned int i=0; i<NUMCOMPUTEUNITS; i++){ globalparams[i].runsize = globalparams[i].runsize_kvs * VECTOR_SIZE; }
		
	if(currentLOP > 0){
		sourcestatsmarker += 1;
		deststatsmarker += NUM_PARTITIONS;
	}

	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->printglobalvars();
	#endif 
	#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(_DEBUGMODE_CHECKS2)
	actsutilityobj->clearglobalvars();
	#endif
	
	*_sourcestatsmarker = sourcestatsmarker;
	*_deststatsmarker = deststatsmarker;
	*_destoffset = destoffset;
	return;
}

void
	#ifdef SW 
	acts::
	#endif 
dispatch_partitiononly(uint512_dt * kvdram, 
			keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
			keyvalue_t buffer_setof1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
			keyvalue_t buffer_setof2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
			keyvalue_t buffer_setof4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
			keyvalue_t buffer_setof8[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
			globalparams_t * _globalparams,
			batch_type * _sourcestatsmarker,
			batch_type * _deststatsmarker,
			batch_type * _destoffset){
	analysis_type analysis_numllops = 1;
	analysis_type analysis_numsourcepartitions = 1;
	
	globalparams_t globalparams = *_globalparams;
	batch_type sourcestatsmarker = *_sourcestatsmarker;
	batch_type deststatsmarker = *_deststatsmarker;
	batch_type destoffset = *_destoffset;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printparameters();
	actsutilityobj->printglobalvars();
	actsutilityobj->printglobalparameters("acts::getglobalparams:: printing global parameters", globalparams);
	#endif 
	#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(_DEBUGMODE_CHECKS2)
	actsutilityobj->clearglobalvars();
	#endif
	#ifdef _DEBUGMODE_STATS
	unsigned int edges_count = 0;
	unsigned int edgesdstv_sum = 0;
	#endif
	
	keyvalue_t unused_sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_sourcebuffer
	
	keyvalue_t unused_buffer_setof1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer_setof1
	keyvalue_t unused_buffer_setof2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer_setof2
	keyvalue_t unused_buffer_setof4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer_setof4
	keyvalue_t unused_buffer_setof8[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer_setof8
	
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
	
	config_t config;
	sweepparams_t sweepparams;
	travstate_t actvvstravstate; actvvstravstate.i=0; actvvstravstate.i_kvs=0; actvvstravstate.v=0; actvvstravstate.k=0; 
	#ifdef _DEBUGMODE_CHECKS2
	if(globalparams.runsize >= MAXKVDATA_BATCHSIZE){ cout<<"dispatch:ERROR. runsize too large!. globalparams.runsize: "<<globalparams.runsize<<", MAXKVDATA_BATCHSIZE: "<<MAXKVDATA_BATCHSIZE<<". EXITING"<<endl; exit(EXIT_FAILURE); }
	#endif
	
	// start launch
	MAIN_LOOP1: for(step_type currentLOP=globalparams.beginLOP + 1; currentLOP<(globalparams.beginLOP + globalparams.numLOPs - 1); currentLOP++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_numllops avg=analysis_numllops	
	
		batch_type num_source_partitions = get_num_source_partitions(currentLOP);
		destoffset = 0;
		bool_type enreduce = ON;
		
		MAIN_LOOP1B: for(batch_type source_partition=0; source_partition<num_source_partitions; source_partition+=1){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_numsourcepartitions avg=analysis_numsourcepartitions

			resetmanykeyandvalues(buffer_setof4, NUM_PARTITIONS, 0);
			resetmanykeyandvalues(globalstatsbuffer, NUM_PARTITIONS, 0);
		
			sweepparams = getsweepparams(globalparams, currentLOP, source_partition);
			travstate_t travstate = gettravstate(ON, kvdram, globalparams, currentLOP, sourcestatsmarker, source_partition, moretravstates);
			travstate_t ctravstate = travstate;
			travstate_t ptravstate = travstate;
			travstate_t avtravstate;
			
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
					buffer_setof2,
					buffer_setof4,
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
				sourcebuffer,
				buffer_setof1,
				buffer_setof2,
				buffer_setof4,
				buffer_setof8,
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
	
	*_globalparams = globalparams;
	*_sourcestatsmarker = sourcestatsmarker;
	*_deststatsmarker = deststatsmarker;
	*_destoffset = destoffset;
	return;
}

// main function (reduce)
void
	#ifdef SW 
	acts::
	#endif 
reducepartition(
		bool_type enablereduce, 
		keyvalue_t sourcevbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11,	
keyvalue_t tempverticesbuffer0[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t tempverticesbuffer1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t tempverticesbuffer2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t tempverticesbuffer3[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t tempverticesbuffer4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t tempverticesbuffer5[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t tempverticesbuffer6[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t tempverticesbuffer7[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t tempverticesbuffer8[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t tempverticesbuffer9[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t tempverticesbuffer10[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t tempverticesbuffer11[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],		unsigned int indexpp, unsigned int reducesubchunksz,
		travstate_t rtravstate[NUMSUBCPUTHREADS], sweepparams_t sweepparams, globalparams_t _globalparams){
	if(enablereduce == OFF){ return; }
	analysis_type analysis_sourceploop = (1 << (NUM_PARTITIONS_POW * TREE_DEPTH));
	analysis_type analysis_reduceloop = (MAXKVDATA_BATCHSIZE / (1 << (NUM_PARTITIONS_POW * TREE_DEPTH))) / SRCBUFFER_SIZE;
	analysis_type analysis_treedepth = TREE_DEPTH;
	
	#ifdef HW 	
	keyvalue_t vubufferpp00[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE]; //
	#pragma HLS array_partition variable = vubufferpp00
	keyvalue_t vubufferpp10[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE]; //
	#pragma HLS array_partition variable = vubufferpp10
	keyvalue_t vubufferpp01[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE]; //
	#pragma HLS array_partition variable = vubufferpp01
	keyvalue_t vubufferpp11[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE]; //
	#pragma HLS array_partition variable = vubufferpp11
	keyvalue_t vubufferpp02[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE]; //
	#pragma HLS array_partition variable = vubufferpp02
	keyvalue_t vubufferpp12[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE]; //
	#pragma HLS array_partition variable = vubufferpp12
	keyvalue_t vubufferpp03[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE]; //
	#pragma HLS array_partition variable = vubufferpp03
	keyvalue_t vubufferpp13[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE]; //
	#pragma HLS array_partition variable = vubufferpp13
	keyvalue_t vubufferpp04[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE]; //
	#pragma HLS array_partition variable = vubufferpp04
	keyvalue_t vubufferpp14[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE]; //
	#pragma HLS array_partition variable = vubufferpp14
	keyvalue_t vubufferpp05[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE]; //
	#pragma HLS array_partition variable = vubufferpp05
	keyvalue_t vubufferpp15[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE]; //
	#pragma HLS array_partition variable = vubufferpp15
	keyvalue_t vubufferpp06[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE]; //
	#pragma HLS array_partition variable = vubufferpp06
	keyvalue_t vubufferpp16[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE]; //
	#pragma HLS array_partition variable = vubufferpp16
	keyvalue_t vubufferpp07[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE]; //
	#pragma HLS array_partition variable = vubufferpp07
	keyvalue_t vubufferpp17[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE]; //
	#pragma HLS array_partition variable = vubufferpp17
	keyvalue_t vubufferpp08[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE]; //
	#pragma HLS array_partition variable = vubufferpp08
	keyvalue_t vubufferpp18[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE]; //
	#pragma HLS array_partition variable = vubufferpp18
	keyvalue_t vubufferpp09[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE]; //
	#pragma HLS array_partition variable = vubufferpp09
	keyvalue_t vubufferpp19[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE]; //
	#pragma HLS array_partition variable = vubufferpp19
	keyvalue_t vubufferpp010[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE]; //
	#pragma HLS array_partition variable = vubufferpp010
	keyvalue_t vubufferpp110[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE]; //
	#pragma HLS array_partition variable = vubufferpp110
	keyvalue_t vubufferpp011[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE]; //
	#pragma HLS array_partition variable = vubufferpp011
	keyvalue_t vubufferpp111[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE]; //
	#pragma HLS array_partition variable = vubufferpp111
	
	#else 
	keyvalue_t vubufferpp0[NUMSUBCPUTHREADS][VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	keyvalue_t vubufferpp1[NUMSUBCPUTHREADS][VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#endif
	
	unsigned int en[NUMSUBCPUTHREADS];
	#pragma HLS ARRAY_PARTITION variable=en complete
	unsigned int enpp0[NUMSUBCPUTHREADS];
	#pragma HLS ARRAY_PARTITION variable=enpp0 complete
	unsigned int enpp1[NUMSUBCPUTHREADS];
	#pragma HLS ARRAY_PARTITION variable=enpp1 complete
	travstate_t rtravstatepp0[NUMSUBCPUTHREADS];
	travstate_t rtravstatepp1[NUMSUBCPUTHREADS];
	batch_type offset1_kvs;
	batch_type offset2_kvs;
	batch_type offset_kvs;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(enablereduce == ON){ for(unsigned int i = 0; i < 1; i++){ actsutilityobj->print7("dispatch::reduce:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", source_partition + index, sweepparams.upperlimit, rtravstate[i].begin_kvs * VECTOR_SIZE, rtravstate[i].end_kvs * VECTOR_SIZE, rtravstate[i].size_kvs * VECTOR_SIZE, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams.currentLOP)), sweepparams.currentLOP); }}
	else { actsutilityobj->print7("dispatch::flush:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", source_partition + index, sweepparams.upperlimit, NAp, NAp, NAp, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams.currentLOP)), sweepparams.currentLOP); }
	#endif

	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ 
	// #pragma HLS UNROLL
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
	
	replicatedata_syn(enablereduce, sourcevbuffer, tempverticesbuffer0,tempverticesbuffer1,tempverticesbuffer2,tempverticesbuffer3,tempverticesbuffer4,tempverticesbuffer5,tempverticesbuffer6,tempverticesbuffer7,tempverticesbuffer8,tempverticesbuffer9,tempverticesbuffer10,tempverticesbuffer11, indexpp * reducesubchunksz, reducesubchunksz); 
	
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
		// #pragma HLS UNROLL
			if(offset1_kvs < rtravstate[i].size_kvs){ enpp0[i] = ON; } else { enpp0[i] = OFF; }
		}
		for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ 
		// #pragma HLS UNROLL
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
		#if defined(INMEMORYGP) && defined(PR_ALGORITHM)
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
		#endif 
		#if defined(INMEMORYGP) && defined(SSSP_ALGORITHM)
		reduce_sssp(enpp1f, enpp1[0], vubufferpp1[0], tempverticesbuffer0, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp1[0], _globalparams); 
		reduce_sssp(enpp1f, enpp1[1], vubufferpp1[1], tempverticesbuffer1, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp1[1], _globalparams); 
		reduce_sssp(enpp1f, enpp1[2], vubufferpp1[2], tempverticesbuffer2, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp1[2], _globalparams); 
		reduce_sssp(enpp1f, enpp1[3], vubufferpp1[3], tempverticesbuffer3, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp1[3], _globalparams); 
		reduce_sssp(enpp1f, enpp1[4], vubufferpp1[4], tempverticesbuffer4, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp1[4], _globalparams); 
		reduce_sssp(enpp1f, enpp1[5], vubufferpp1[5], tempverticesbuffer5, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp1[5], _globalparams); 
		reduce_sssp(enpp1f, enpp1[6], vubufferpp1[6], tempverticesbuffer6, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp1[6], _globalparams); 
		reduce_sssp(enpp1f, enpp1[7], vubufferpp1[7], tempverticesbuffer7, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp1[7], _globalparams); 
		reduce_sssp(enpp1f, enpp1[8], vubufferpp1[8], tempverticesbuffer8, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp1[8], _globalparams); 
		reduce_sssp(enpp1f, enpp1[9], vubufferpp1[9], tempverticesbuffer9, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp1[9], _globalparams); 
		reduce_sssp(enpp1f, enpp1[10], vubufferpp1[10], tempverticesbuffer10, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp1[10], _globalparams); 
		reduce_sssp(enpp1f, enpp1[11], vubufferpp1[11], tempverticesbuffer11, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp1[11], _globalparams); 
		#endif 
		
		// reduce 0
		#if defined(INMEMORYGP) && defined(PR_ALGORITHM)
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
		#endif 
		#if defined(INMEMORYGP) && defined(SSSP_ALGORITHM)
		reduce_sssp(ON, enpp0[0], vubufferpp0[0], tempverticesbuffer0, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp0[0], _globalparams);
		reduce_sssp(ON, enpp0[1], vubufferpp0[1], tempverticesbuffer1, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp0[1], _globalparams);
		reduce_sssp(ON, enpp0[2], vubufferpp0[2], tempverticesbuffer2, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp0[2], _globalparams);
		reduce_sssp(ON, enpp0[3], vubufferpp0[3], tempverticesbuffer3, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp0[3], _globalparams);
		reduce_sssp(ON, enpp0[4], vubufferpp0[4], tempverticesbuffer4, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp0[4], _globalparams);
		reduce_sssp(ON, enpp0[5], vubufferpp0[5], tempverticesbuffer5, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp0[5], _globalparams);
		reduce_sssp(ON, enpp0[6], vubufferpp0[6], tempverticesbuffer6, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp0[6], _globalparams);
		reduce_sssp(ON, enpp0[7], vubufferpp0[7], tempverticesbuffer7, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp0[7], _globalparams);
		reduce_sssp(ON, enpp0[8], vubufferpp0[8], tempverticesbuffer8, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp0[8], _globalparams);
		reduce_sssp(ON, enpp0[9], vubufferpp0[9], tempverticesbuffer9, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp0[9], _globalparams);
		reduce_sssp(ON, enpp0[10], vubufferpp0[10], tempverticesbuffer10, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp0[10], _globalparams);
		reduce_sssp(ON, enpp0[11], vubufferpp0[11], tempverticesbuffer11, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp0[11], _globalparams);
		#endif 
		
		// read 1
		offset2_kvs = offset_kvs + rtravstate[0].skip_kvs;
		for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ 
		// #pragma HLS UNROLL
			if(offset2_kvs < rtravstate[i].size_kvs){ enpp1[i] = ON; } else { enpp1[i] = OFF; }
		}
		for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ 
		// #pragma HLS UNROLL
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
		// #pragma HLS UNROLL
			if(offset_kvs < rtravstate[i].size_kvs){ en[i] = ON; } else { en[i] = OFF; }
		}
		
		for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ 
		// #pragma HLS UNROLL
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

		#if defined(INMEMORYGP) && defined(PR_ALGORITHM) 
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
		#endif 
		#if defined(INMEMORYGP) && defined(SSSP_ALGORITHM) 
		reduce_sssp(ON, en[0], vubufferpp0[0], tempverticesbuffer0, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[0], _globalparams);
		reduce_sssp(ON, en[1], vubufferpp0[1], tempverticesbuffer1, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[1], _globalparams);
		reduce_sssp(ON, en[2], vubufferpp0[2], tempverticesbuffer2, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[2], _globalparams);
		reduce_sssp(ON, en[3], vubufferpp0[3], tempverticesbuffer3, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[3], _globalparams);
		reduce_sssp(ON, en[4], vubufferpp0[4], tempverticesbuffer4, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[4], _globalparams);
		reduce_sssp(ON, en[5], vubufferpp0[5], tempverticesbuffer5, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[5], _globalparams);
		reduce_sssp(ON, en[6], vubufferpp0[6], tempverticesbuffer6, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[6], _globalparams);
		reduce_sssp(ON, en[7], vubufferpp0[7], tempverticesbuffer7, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[7], _globalparams);
		reduce_sssp(ON, en[8], vubufferpp0[8], tempverticesbuffer8, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[8], _globalparams);
		reduce_sssp(ON, en[9], vubufferpp0[9], tempverticesbuffer9, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[9], _globalparams);
		reduce_sssp(ON, en[10], vubufferpp0[10], tempverticesbuffer10, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[10], _globalparams);
		reduce_sssp(ON, en[11], vubufferpp0[11], tempverticesbuffer11, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[11], _globalparams);
		#endif
	}	
	#endif
	return;
}

travstate_t 
	#ifdef SW 
	acts::
	#endif
collectactvvs(uint512_dt * vdram, bool_type enable, keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
					keyvalue_t actvvs0[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t actvvs1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t actvvscapsule[8], travstate_t actvvstravstate,
					buffer_type destoffset, buffer_type size, sweepparams_t sweepparams, globalparams_t globalparams){
	if(enable == OFF){ return actvvstravstate; }
	analysis_type analysis_loopcount = (APPLYVERTEXBUFFERSZ / VDATAPACKINGFACTOR) / 2;
	
	buffer_type dest_v = 0;
	buffer_type dest_i = 0;
	buffer_type destoffset_kvs = destoffset / VECTOR_SIZE;
	unsigned int nonzeroactvvsreturned = OFF;
	
	for(vector_type i=0; i<2; i++){ 
		actvvscapsule[i].key = 0; actvvscapsule[i].value = 0; 
	}
	unsigned int voffset = sweepparams.source_partition * globalparams.applyvertexbuffersz;
	
	COLLECTACTVVS_LOOP1: for(buffer_type i=0; i<size; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectactvvs.i", i, PADDEDDESTBUFFER_SIZE, destoffset, destoffset_kvs, size);
		#endif
		
		keyvalue_t data = destbuffer[dest_v][destoffset_kvs + dest_i];
		
		COLLECTACTVVS_LOOP1B: for(unsigned int j = 0; j < 2; j++){
			COLLECTACTVVS_LOOP1C: for(unsigned int t = 0; t < 16; t++){
				unsigned int vid = voffset + i*32 + j*16 + t;
				unsigned int pos = (j*16 + t) * 2; 
				
				unsigned int vdata = READFROM_ULONG(data, pos, 2); 
				
				if(vdata == VISITED_IN_CURRENT_ITERATION){ 
					#ifdef _DEBUGMODE_KERNELPRINTS
					cout<<"collectactvvs: ACTIVE VERTICES seen for next iteration. t: "<<t<<", pos: "<<pos<<", vid: "<<vid<<", vdata: "<<vdata<<endl;
					#endif
					nonzeroactvvsreturned = ON;
				
					unsigned int X = actvvscapsule[j].value % VECTOR2_SIZE;
					unsigned int Y = actvvscapsule[j].value / VECTOR2_SIZE;
					
					if(j == 0){
						if(actvvscapsule[j].value % 2 == 0){
							actvvs0[X/2][Y].key = vid;
						} else {
							actvvs0[X/2][Y].value = vid;
						}
					} else {
						if(actvvscapsule[j].value % 2 == 0){
							actvvs1[X/2][Y].key = vid;
						} else {
							actvvs1[X/2][Y].value = vid;
						}
					}
					
					WRITETO_ULONG(&data, pos, 2, VISITED_IN_PAST_ITERATION);
					
					actvvscapsule[j].value += 1;
				}
			}
		}
		
		destbuffer[dest_v][destoffset_kvs + dest_i] = data;
		dest_v+=1; if(dest_v == VECTOR_SIZE){ dest_v=0; dest_i+=1; }
	}

	unsigned int X = actvvscapsule[0].value % VECTOR2_SIZE;
	unsigned int Y = actvvscapsule[0].value / VECTOR2_SIZE;
	if(X > 0){
		for(vector_type v=X; v<VECTOR2_SIZE; v++){
			if(v % 2 == 0){
				actvvs0[v/2][Y].key = INVALIDDATA;
			} else {
				actvvs0[v/2][Y].value = INVALIDDATA;
			}
			actvvscapsule[0].value += 1;	
		}
	}
	X = actvvscapsule[1].value % VECTOR2_SIZE;
	Y = actvvscapsule[1].value / VECTOR2_SIZE;
	if(X > 0){
		for(vector_type v=X; v<VECTOR2_SIZE; v++){
			if(v % 2 == 0){
				actvvs1[v/2][Y].key = INVALIDDATA;
			} else {
				actvvs1[v/2][Y].value = INVALIDDATA;
			}
			actvvscapsule[1].value += 1;	
		}
	}
	
	if(nonzeroactvvsreturned == ON){
		/* for(unsigned int k=0; k<2; k++){
			savekeyvalues(ON, vdram, actvvs[k], globalparams.baseoffset_activevertices_kvs + actvvstravstate.i_kvs, actvvscapsule[k].value / VECTOR2_SIZE); 
			actvvstravstate.i_kvs += actvvscapsule[k].value / VECTOR2_SIZE; actvvstravstate.i += actvvscapsule[k].value;
		} */
		
		savekeyvalues(ON, vdram, actvvs0, globalparams.baseoffset_activevertices_kvs + actvvstravstate.i_kvs, actvvscapsule[0].value / VECTOR2_SIZE); 
		actvvstravstate.i_kvs += actvvscapsule[0].value / VECTOR2_SIZE; actvvstravstate.i += actvvscapsule[0].value;
		
		savekeyvalues(ON, vdram, actvvs1, globalparams.baseoffset_activevertices_kvs + actvvstravstate.i_kvs, actvvscapsule[1].value / VECTOR2_SIZE); 
		actvvstravstate.i_kvs += actvvscapsule[1].value / VECTOR2_SIZE; actvvstravstate.i += actvvscapsule[1].value;
	}
	return actvvstravstate; // nonzeroactvvsreturned;
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
	unsigned int local_source_partition = 0;
	unsigned int sourcepartitionblock = 0;
			
	MAIN_LOOP: for(batch_type source_partition=0; source_partition<num_source_partitions; source_partition+=1){
		#ifdef EMBEDDEDCOLLECTSTATS
		resetmanykeyandvalues(buffer2, GLOBALSTATSBUFFERSZ, 0);
		#else 
		resetmanykeyandvalues(buffer2, NUM_PARTITIONS, 0);
		#endif 
		
		sweepparams = getsweepparams(globalparams, currentLOP, source_partition);
		travstate_t travstate = gettravstate(ON, kvdram, globalparams, currentLOP, sourcestatsmarker, source_partition, moretravstates);
		travstate_t rtravstate = travstate;
		if(local_source_partition == 0){
			batch_type ntravszs = 0;
			for(batch_type k=0; k<globalparams.loadfactorforreduce; k++){ ntravszs += moretravstates[k].value; }
			if(ntravszs > 0){ enreduce = ON; } else { enreduce = OFF; }
		}
	
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
			actvvstravstate,
			local_source_partition);
		sourcestatsmarker += 1;
		
		local_source_partition += 1;
		if(local_source_partition >= globalparams.loadfactorforreduce){ local_source_partition = 0; sourcepartitionblock += 1; }
		
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
	
	#ifdef _DEBUGMODE_KERNELPRINTS2
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
dispatch_reduceonly(
			uint512_dt * kvdram,
			keyvalue_t tempverticesbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
			keyvalue_t vubufferpp0[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
			keyvalue_t vubufferpp1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
			globalparams_t globalparams){
	analysis_type analysis_mainloop = (1 << (NUM_PARTITIONS_POW * TREE_DEPTH));
	analysis_type analysis_mainloop1b = LOADFACTORFORREDUCE;
	analysis_type analysis_mainloop2 = LOADFACTORFORREDUCE;
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
	keyvalue_t unused_tempverticesbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_tempverticesbuffer	
	keyvalue_t unused_vubufferpp0[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_vubufferpp0
	keyvalue_t unused_vubufferpp1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_vubufferpp1
	
	travstate_t rtravstate;
	travstate_t rtravstatepp0;
	travstate_t rtravstatepp1;
	keyvalue_t moretravstates[MAXLOADFACTORFORREDUCE];
	
	unsigned int en;
	unsigned int enpp0;
	unsigned int enpp1;
	sweepparams_t sweepparams;
	
	bool_type pp0writeen = ON;
	bool_type writeen_actvvs = ON;
	bool_type nonzeroactvvsreturned = ON;
	
	batch_type offset1_kvs;
	batch_type offset2_kvs;
	batch_type offset_kvs;
	
	step_type currentLOP = globalparams.beginLOP + globalparams.numLOPs - 1;
	batch_type num_source_partitions = get_num_source_partitions(currentLOP);
	unsigned int sourcestatsmarker = 0;
	for(unsigned int k=0; k<globalparams.treedepth; k++){ 
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_treedepth avg=analysis_treedepth
		sourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); 
	}
	batch_type ntravszs = 0;
	unsigned int reducesubchunksz = (globalparams.applyvertexbuffersz / VDATAPACKINGFACTOR) / 2;
	unsigned int reducechunksz_kvs = (reducesubchunksz * LOADFACTORFORREDUCE) / VECTOR_SIZE; // NOT USED.
	unsigned int sourcepartitionblock = 0;

	MAIN_LOOP: for(batch_type source_partition=0; source_partition<num_source_partitions + 1; source_partition+=globalparams.loadfactorforreduce){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_mainloop avg=analysis_mainloop
		#ifdef _DEBUGMODE_KERNELPRINTS2
		actsutilityobj->print3("### dispatch::reduce:: source_partition", "sourcepartitionblock", "currentLOP", source_partition, sourcepartitionblock, currentLOP); 							
		#endif
		
		bool_type enablereduce = ON;
		bool_type enableflush = OFF;
		
		if(source_partition >= num_source_partitions){ enableflush = ON; } else { enableflush = OFF; }
		
		if(enableflush == OFF){
		rtravstate = gettravstate(ON, kvdram, globalparams, currentLOP, sourcestatsmarker, source_partition, moretravstates); }
		ntravszs = 0;
		for(batch_type k=0; k<globalparams.loadfactorforreduce; k++){ 
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_mainloop1b avg=analysis_mainloop1b
			ntravszs += moretravstates[k].value; 
		}
		if((ntravszs == 0) || (source_partition >= num_source_partitions)){ enablereduce = OFF; } else { enablereduce = ON; }
		
		if(enablereduce == ON || enableflush == ON){
			readkeyvalues(enablereduce, kvdram, sourcevbuffer, (globalparams.baseoffset_verticesdata_kvs + (sourcepartitionblock * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE); // reducechunksz_kvs? CRITICAL AUTOMATEME.
			replicatedata(enablereduce, sourcevbuffer, tempverticesbuffer, 0, reducesubchunksz);
	
			MAIN_LOOP2: for(batch_type index=0; index<globalparams.loadfactorforreduce; index+=1){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_mainloop2 avg=analysis_mainloop2
				unsigned int spartition = source_partition + index;
				sweepparams = getsweepparams(globalparams, currentLOP, spartition);
				
				#ifdef _DEBUGMODE_KERNELPRINTS
				if(enablereduce == ON){ for(unsigned int i = 0; i < 1; i++){ actsutilityobj->print7("dispatch::reduce:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", source_partition + index, sweepparams.upperlimit, rtravstate[i].begin_kvs * VECTOR_SIZE, rtravstate[i].end_kvs * VECTOR_SIZE, rtravstate[i].size_kvs * VECTOR_SIZE, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams.currentLOP)), sweepparams.currentLOP); }}
				else { actsutilityobj->print7("dispatch::flush:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", source_partition + index, sweepparams.upperlimit, NAp, NAp, NAp, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams.currentLOP)), sweepparams.currentLOP); }
				#endif
					
				rtravstatepp0 = rtravstate; 
				rtravstatepp1 = rtravstate; 
				
				unsigned int enpp1f = ON;
				#ifdef PP1
				unsigned int totsz_kvs = rtravstate.size_kvs + rtravstate.skip_kvs;
				#else 
				unsigned int totsz_kvs = rtravstate.size_kvs;
				#endif
				if(enablereduce == OFF){ totsz_kvs = 0; }

				#ifdef PP1_XXX
				MAIN_LOOP1E_REDUCE: for(offset_kvs=0; offset_kvs<totsz_kvs; offset_kvs+=2*rtravstate.skip_kvs){
				#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_reduceloop avg=analysis_reduceloop
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### dispatch::reduce:: offset1_kvs", "offset2_kvs", "begin_kvs", "end_kvs", "skip", rtravstate.begin_kvs + offset1_kvs, rtravstate.begin_kvs + offset2_kvs, rtravstate.begin_kvs, rtravstate.end_kvs, SRCBUFFER_SIZE); 
					#endif
					
					if(offset_kvs > 0){ enpp1f = ON; } else { enpp1f = OFF; }
					
					// read 0
					offset1_kvs = offset_kvs;
					if(offset1_kvs < rtravstate.size_kvs){ enpp0 = ON; } else { enpp0 = OFF; }
					if(enpp0 == ON){ rtravstatepp0.i_kvs = rtravstatepp0.begin_kvs + offset1_kvs; }
					readkeyvalues(enpp0, kvdram, vubufferpp0, (sweepparams.worksourcebaseaddress_kvs + rtravstatepp0.begin_kvs + offset1_kvs), SRCBUFFER_SIZE, rtravstatepp0);
					// reduce 1
					reduce(enpp1f, enpp1, vubufferpp1, tempverticesbuffer, sweepparams, globalparams.GraphIter, globalparams.GraphAlgo, rtravstatepp1, globalparams); 
					
					// reduce 0
					reduce(ON, enpp0, vubufferpp0, tempverticesbuffer, sweepparams, globalparams.GraphIter, globalparams.GraphAlgo, rtravstatepp0, globalparams);
					// read 1
					offset2_kvs = offset_kvs + rtravstate.skip_kvs;
					if(offset2_kvs < rtravstate.size_kvs){ enpp1 = ON; } else { enpp1 = OFF; }
					if(enpp1 == ON){ rtravstatepp1.i_kvs = rtravstatepp1.begin_kvs + offset2_kvs; }
					readkeyvalues(enpp1, kvdram, vubufferpp1, (sweepparams.worksourcebaseaddress_kvs + rtravstatepp1.begin_kvs + offset2_kvs), SRCBUFFER_SIZE, rtravstatepp1);
				}
				#else 
				MAIN_LOOP1E_REDUCE: for(offset_kvs=0; offset_kvs<totsz_kvs; offset_kvs+=rtravstate.skip_kvs){
				#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_reduceloop avg=analysis_reduceloop
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print4("### dispatch::reduce:: offset_kvs", "begin_kvs", "end_kvs", "skip", rtravstate.begin_kvs + offset_kvs, rtravstate.begin_kvs, rtravstate.end_kvs, SRCBUFFER_SIZE); 
					#endif
				
					if(offset_kvs < rtravstate.size_kvs){ en = ON; } else { en = OFF; }
					if(en == ON){ rtravstate.i_kvs = rtravstate.begin_kvs + offset_kvs; }

					readkeyvalues(en, kvdram, vubufferpp0, (sweepparams.worksourcebaseaddress_kvs + rtravstate.begin_kvs + offset_kvs), SRCBUFFER_SIZE, rtravstate);
					reduce(ON, en, vubufferpp0, tempverticesbuffer, sweepparams, globalparams.GraphIter, globalparams.GraphAlgo, rtravstate, globalparams);
				}	
				#endif
				
				unifydata(enablereduce, tempverticesbuffer, destvbuffer, index * reducesubchunksz, reducesubchunksz, globalparams.GraphAlgo);
	
				// overlap functions for next iteration
				if(index < globalparams.loadfactorforreduce-1){ 
					replicatedata(enablereduce, sourcevbuffer, tempverticesbuffer, (index + 1) * reducesubchunksz, reducesubchunksz);
					rtravstate = gettravstate(enablereduce, kvdram, globalparams, currentLOP, sourcestatsmarker + index + 1, spartition + 1, moretravstates);
				}
			}
			
			savekeyvalues(enablereduce, kvdram, destvbuffer, (globalparams.baseoffset_verticesdata_kvs + (sourcepartitionblock * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE); // reducechunksz_kvs? CRITICAL AUTOMATEME.
		}
		
		sourcestatsmarker += globalparams.loadfactorforreduce;
		sourcepartitionblock += 1;
		
		#ifdef _DEBUGMODE_KERNELPRINTS
		actsutilityobj->printglobalvars();
		actsutilityobj->clearglobalvars();
		#endif
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->printglobalvars();
	#endif
	#ifdef _DEBUGMODE_STATS
	setkey(kvdram, PADDEDKVSOURCEDRAMSZ_KVS-1, 3, actsutilityobj->globalstats_getcountvalidkvsreduced());
	setkey(kvdram, PADDEDKVSOURCEDRAMSZ_KVS-1, 4, actsutilityobj->globalstats_getreducevar1());
	setkey(kvdram, PADDEDKVSOURCEDRAMSZ_KVS-1, 5, NAp);
	#endif
	return;
}

travstate_t
	#ifdef SW 
	acts::
	#endif 
dispatch_reduceonly_parallelsync(
			uint512_dt * vdram, 
uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11,			travstate_t actvvstravstate, globalparams_t globalparams[NUMSUBCPUTHREADS]){
	analysis_type analysis_sourceploop = (1 << (NUM_PARTITIONS_POW * TREE_DEPTH));
	analysis_type analysis_reduceloop = (MAXKVDATA_BATCHSIZE / (1 << (NUM_PARTITIONS_POW * TREE_DEPTH))) / SRCBUFFER_SIZE;
	analysis_type analysis_treedepth = TREE_DEPTH;
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printparameters();
	actsutilityobj->printglobalvars();
	#endif 
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	keyvalue_t sourcevbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = sourcevbuffer
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
	
	keyvalue_t destvbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = destvbuffer
	
	keyvalue_t actvvs0[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = actvvs0
	keyvalue_t actvvs1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = actvvs1
	skeyvalue_t actvvscapsule[8];
	#pragma HLS ARRAY_PARTITION variable=actvvscapsule complete
	
	travstate_t rtravstate[NUMSUBCPUTHREADS];
	#pragma HLS ARRAY_PARTITION variable=rtravstate complete
	travstate_t rtravstatepp0[NUMSUBCPUTHREADS];
	#pragma HLS ARRAY_PARTITION variable=rtravstatepp0 complete
	travstate_t rtravstatepp1[NUMSUBCPUTHREADS];
	#pragma HLS ARRAY_PARTITION variable=rtravstatepp1 complete
	keyvalue_t moretravstates[NUMSUBCPUTHREADS][MAXLOADFACTORFORREDUCE];
	#pragma HLS array_partition variable = moretravstates
	
	unsigned int en[NUMSUBCPUTHREADS];
	#pragma HLS ARRAY_PARTITION variable=en complete
	unsigned int enpp0[NUMSUBCPUTHREADS];
	#pragma HLS ARRAY_PARTITION variable=enpp0 complete
	unsigned int enpp1[NUMSUBCPUTHREADS];
	#pragma HLS ARRAY_PARTITION variable=enpp1 complete
	keyvalue_t offsetsandsizes[NUMCOMPUTEUNITS];
	#pragma HLS ARRAY_PARTITION variable=offsetsandsizes complete
	
	unsigned int itercount = 0;
	unsigned int itercount_actvvs = 0;
	bool_type pp0writeen = ON;
	bool_type writeen_actvvs = ON;
	bool_type nonzeroactvvsreturned = ON;
	
	batch_type offset1_kvs;
	batch_type offset2_kvs;
	batch_type offset_kvs;
	batch_type indexpp0 = 0;
	batch_type indexpp1 = 0;
	unsigned int spartition0 = 0;
	unsigned int spartition1 = 0;
	sweepparams_t sweepparams0;
	sweepparams_t sweepparams1;
	bool_type enablereducepp0;
	bool_type enablereducepp1;
	bool_type enablereducepp1_flush;
	
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
	unsigned int reducesubchunksz = (_globalparams.applyvertexbuffersz / VDATAPACKINGFACTOR) / 2; 
	unsigned int reducechunksz_kvs = (reducesubchunksz * LOADFACTORFORREDUCE) / VECTOR_SIZE; // NOT USED.
	unsigned int sourcepartitionblock = 0;

	MAIN_LOOP: for(batch_type source_partition=0; source_partition<num_source_partitions; source_partition+=_globalparams.loadfactorforreduce){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_sourceploop avg=analysis_sourceploop
		#ifdef _DEBUGMODE_KERNELPRINTS2
		actsutilityobj->print3("### dispatch::reduce:: source_partition", "sourcepartitionblock", "currentLOP", source_partition, sourcepartitionblock, currentLOP); 							
		#endif
		
		bool_type enablereduce = ON;
 
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
		ntravszs = 0;
		for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ for(batch_type k=0; k<_globalparams.loadfactorforreduce; k++){ ntravszs += moretravstates[i][k].value; }}
		if((ntravszs == 0) || (source_partition >= num_source_partitions)){ enablereduce = OFF; } else { enablereduce = ON; }
		
		enablereducepp0 = enablereduce;
		enablereducepp1 = enablereduce;
		enablereducepp1_flush = enablereduce;
		
		if(enablereduce == ON){
			readkeyvalues(enablereduce, vdram, sourcevbuffer, (_globalparams.baseoffset_verticesdata_kvs + (sourcepartitionblock * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE); // reducechunksz_kvs? CRITICAL AUTOMATEME.
			
			MAIN_LOOP2: for(batch_type index=0; index<_globalparams.loadfactorforreduce + 1; index+=2){ // AUTOMATEME. for two pipelines
				if(index >= _globalparams.loadfactorforreduce){ enablereducepp0 = OFF; enablereducepp1 = OFF; }
				if(index == 0){ enablereducepp1_flush = OFF; } else { enablereducepp1_flush = ON; }
				
				// proc vars 0
				indexpp0 = index;
				spartition0 = source_partition + indexpp0;
				sweepparams0 = getsweepparams(_globalparams, currentLOP, spartition0);
				rtravstate[0] = gettravstate(enablereducepp0, kvdram0, globalparams[0], currentLOP, sourcestatsmarker + indexpp0, spartition0, moretravstates[0]);
				rtravstate[1] = gettravstate(enablereducepp0, kvdram1, globalparams[1], currentLOP, sourcestatsmarker + indexpp0, spartition0, moretravstates[1]);
				rtravstate[2] = gettravstate(enablereducepp0, kvdram2, globalparams[2], currentLOP, sourcestatsmarker + indexpp0, spartition0, moretravstates[2]);
				rtravstate[3] = gettravstate(enablereducepp0, kvdram3, globalparams[3], currentLOP, sourcestatsmarker + indexpp0, spartition0, moretravstates[3]);
				rtravstate[4] = gettravstate(enablereducepp0, kvdram4, globalparams[4], currentLOP, sourcestatsmarker + indexpp0, spartition0, moretravstates[4]);
				rtravstate[5] = gettravstate(enablereducepp0, kvdram5, globalparams[5], currentLOP, sourcestatsmarker + indexpp0, spartition0, moretravstates[5]);
				rtravstate[6] = gettravstate(enablereducepp0, kvdram6, globalparams[6], currentLOP, sourcestatsmarker + indexpp0, spartition0, moretravstates[6]);
				rtravstate[7] = gettravstate(enablereducepp0, kvdram7, globalparams[7], currentLOP, sourcestatsmarker + indexpp0, spartition0, moretravstates[7]);
				rtravstate[8] = gettravstate(enablereducepp0, kvdram8, globalparams[8], currentLOP, sourcestatsmarker + indexpp0, spartition0, moretravstates[8]);
				rtravstate[9] = gettravstate(enablereducepp0, kvdram9, globalparams[9], currentLOP, sourcestatsmarker + indexpp0, spartition0, moretravstates[9]);
				rtravstate[10] = gettravstate(enablereducepp0, kvdram10, globalparams[10], currentLOP, sourcestatsmarker + indexpp0, spartition0, moretravstates[10]);
				rtravstate[11] = gettravstate(enablereducepp0, kvdram11, globalparams[11], currentLOP, sourcestatsmarker + indexpp0, spartition0, moretravstates[11]);
				dummywrite(actvvs0, actvvs1, index, indexpp0); // dummy for optimal scheduling
				
				// reduce 0
				reducepartition(
					enablereducepp0, 
					sourcevbuffer,
kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9,kvdram10,kvdram11,	
tempverticesbuffer0,tempverticesbuffer1,tempverticesbuffer2,tempverticesbuffer3,tempverticesbuffer4,tempverticesbuffer5,tempverticesbuffer6,tempverticesbuffer7,tempverticesbuffer8,tempverticesbuffer9,tempverticesbuffer10,tempverticesbuffer11, 	
					indexpp0, reducesubchunksz, rtravstate, sweepparams0, _globalparams);
				// collect & save actvvs 1
				#if defined(INMEMORYGP) && defined(BFS_ALGORITHM)
				actvvstravstate = collectactvvs(vdram, enablereducepp1_flush, destvbuffer, actvvs0, actvvs1, actvvscapsule, actvvstravstate,
					(indexpp1 * reducesubchunksz), reducesubchunksz, sweepparams1, _globalparams);
				#endif
				
				// unify 0
				#if defined(INMEMORYGP) && defined(BFS_ALGORITHM)
				unifydata_bfs_parallelsyn(enablereducepp0, tempverticesbuffer0,tempverticesbuffer1,tempverticesbuffer2,tempverticesbuffer3,tempverticesbuffer4,tempverticesbuffer5,tempverticesbuffer6,tempverticesbuffer7,tempverticesbuffer8,tempverticesbuffer9,tempverticesbuffer10,tempverticesbuffer11, destvbuffer, 
						indexpp0 * reducesubchunksz, reducesubchunksz, sweepparams0, _globalparams);
				#endif
				// proc vars 1
				indexpp1 = index + 1;
				spartition1 = source_partition + indexpp1;
				sweepparams1 = getsweepparams(_globalparams, currentLOP, spartition1);
				rtravstate[0] = gettravstate(enablereducepp1, kvdram0, globalparams[0], currentLOP, sourcestatsmarker + indexpp1, spartition1, moretravstates[0]);
				rtravstate[1] = gettravstate(enablereducepp1, kvdram1, globalparams[1], currentLOP, sourcestatsmarker + indexpp1, spartition1, moretravstates[1]);
				rtravstate[2] = gettravstate(enablereducepp1, kvdram2, globalparams[2], currentLOP, sourcestatsmarker + indexpp1, spartition1, moretravstates[2]);
				rtravstate[3] = gettravstate(enablereducepp1, kvdram3, globalparams[3], currentLOP, sourcestatsmarker + indexpp1, spartition1, moretravstates[3]);
				rtravstate[4] = gettravstate(enablereducepp1, kvdram4, globalparams[4], currentLOP, sourcestatsmarker + indexpp1, spartition1, moretravstates[4]);
				rtravstate[5] = gettravstate(enablereducepp1, kvdram5, globalparams[5], currentLOP, sourcestatsmarker + indexpp1, spartition1, moretravstates[5]);
				rtravstate[6] = gettravstate(enablereducepp1, kvdram6, globalparams[6], currentLOP, sourcestatsmarker + indexpp1, spartition1, moretravstates[6]);
				rtravstate[7] = gettravstate(enablereducepp1, kvdram7, globalparams[7], currentLOP, sourcestatsmarker + indexpp1, spartition1, moretravstates[7]);
				rtravstate[8] = gettravstate(enablereducepp1, kvdram8, globalparams[8], currentLOP, sourcestatsmarker + indexpp1, spartition1, moretravstates[8]);
				rtravstate[9] = gettravstate(enablereducepp1, kvdram9, globalparams[9], currentLOP, sourcestatsmarker + indexpp1, spartition1, moretravstates[9]);
				rtravstate[10] = gettravstate(enablereducepp1, kvdram10, globalparams[10], currentLOP, sourcestatsmarker + indexpp1, spartition1, moretravstates[10]);
				rtravstate[11] = gettravstate(enablereducepp1, kvdram11, globalparams[11], currentLOP, sourcestatsmarker + indexpp1, spartition1, moretravstates[11]);
				dummywrite(actvvs0, actvvs1, index, indexpp0);
				
				// collect & save actvvs 0
				#if defined(INMEMORYGP) && defined(BFS_ALGORITHM)
				actvvstravstate = collectactvvs(vdram, enablereducepp0, destvbuffer, actvvs0, actvvs1, actvvscapsule, actvvstravstate,
					(indexpp0 * reducesubchunksz), reducesubchunksz, sweepparams0, _globalparams);
				#endif
				// reduce 1
				reducepartition(
					enablereducepp1,
					sourcevbuffer,
kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9,kvdram10,kvdram11,	
tempverticesbuffer0,tempverticesbuffer1,tempverticesbuffer2,tempverticesbuffer3,tempverticesbuffer4,tempverticesbuffer5,tempverticesbuffer6,tempverticesbuffer7,tempverticesbuffer8,tempverticesbuffer9,tempverticesbuffer10,tempverticesbuffer11, 
					indexpp1, reducesubchunksz, rtravstate, sweepparams1, _globalparams);
				
				// unify 1
				#if defined(INMEMORYGP) && defined(BFS_ALGORITHM)
				unifydata_bfs_parallelsyn(enablereducepp1, tempverticesbuffer0,tempverticesbuffer1,tempverticesbuffer2,tempverticesbuffer3,tempverticesbuffer4,tempverticesbuffer5,tempverticesbuffer6,tempverticesbuffer7,tempverticesbuffer8,tempverticesbuffer9,tempverticesbuffer10,tempverticesbuffer11, destvbuffer, 
						indexpp1 * reducesubchunksz, reducesubchunksz, sweepparams1, _globalparams);
				#endif
			}
			
			savekeyvalues(enablereduce, vdram, destvbuffer, (_globalparams.baseoffset_verticesdata_kvs + (sourcepartitionblock * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE); // reducechunksz_kvs? AUTOMATEME.
		}
		
		sourcestatsmarker += _globalparams.loadfactorforreduce;
		sourcepartitionblock += 1;
		
		#ifdef _DEBUGMODE_KERNELPRINTS
		actsutilityobj->printglobalvars();
		actsutilityobj->clearglobalvars();
		#endif
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->printglobalvars();
	#endif
	#ifdef _DEBUGMODE_STATS
	setkey(vdram, PADDEDVDRAMSZ_KVS-1, 3, actsutilityobj->globalstats_getcountvalidkvsreduced());
	setkey(vdram, PADDEDVDRAMSZ_KVS-1, 4, actsutilityobj->globalstats_getreducevar1());
	setkey(vdram, PADDEDVDRAMSZ_KVS-1, 5, actvvstravstate.i);
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
topkernel(uint512_dt * vdram ,uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11){ 
	
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem0
 
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem1
 
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem2
 
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem3
 
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem4
 
#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem5
 
#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem6
 
#pragma HLS INTERFACE m_axi port = kvdram6 offset = slave bundle = gmem7
 
#pragma HLS INTERFACE m_axi port = kvdram7 offset = slave bundle = gmem8
 
#pragma HLS INTERFACE m_axi port = kvdram8 offset = slave bundle = gmem9
 
#pragma HLS INTERFACE m_axi port = kvdram9 offset = slave bundle = gmem10
 
#pragma HLS INTERFACE m_axi port = kvdram10 offset = slave bundle = gmem11
 
#pragma HLS INTERFACE m_axi port = kvdram11 offset = slave bundle = gmem12
		
#pragma HLS INTERFACE s_axilite port = vdram bundle = control
 
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

#pragma HLS DATA_PACK variable = vdram
 
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
  
	cout<<">>> Light weight ACTS 0 (_L2) Launched... size: "<<(unsigned int)(kvdram0[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].range(31, 0))<<endl; 
  
	cout<<">>> Light weight ACTS 1 (_L2) Launched... size: "<<(unsigned int)(kvdram1[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].range(31, 0))<<endl; 
  
	cout<<">>> Light weight ACTS 2 (_L2) Launched... size: "<<(unsigned int)(kvdram2[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].range(31, 0))<<endl; 
  
	cout<<">>> Light weight ACTS 3 (_L2) Launched... size: "<<(unsigned int)(kvdram3[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].range(31, 0))<<endl; 
  
	cout<<">>> Light weight ACTS 4 (_L2) Launched... size: "<<(unsigned int)(kvdram4[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].range(31, 0))<<endl; 
  
	cout<<">>> Light weight ACTS 5 (_L2) Launched... size: "<<(unsigned int)(kvdram5[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].range(31, 0))<<endl; 
  
	cout<<">>> Light weight ACTS 6 (_L2) Launched... size: "<<(unsigned int)(kvdram6[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].range(31, 0))<<endl; 
  
	cout<<">>> Light weight ACTS 7 (_L2) Launched... size: "<<(unsigned int)(kvdram7[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].range(31, 0))<<endl; 
  
	cout<<">>> Light weight ACTS 8 (_L2) Launched... size: "<<(unsigned int)(kvdram8[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].range(31, 0))<<endl; 
  
	cout<<">>> Light weight ACTS 9 (_L2) Launched... size: "<<(unsigned int)(kvdram9[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].range(31, 0))<<endl; 
  
	cout<<">>> Light weight ACTS 10 (_L2) Launched... size: "<<(unsigned int)(kvdram10[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].range(31, 0))<<endl; 
  
	cout<<">>> Light weight ACTS 11 (_L2) Launched... size: "<<(unsigned int)(kvdram11[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].range(31, 0))<<endl; 
	#else
  
	cout<<">>> Light weight ACTS 0 (_L2) Launched... size: "<<kvdram0[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key<<endl; 
  
	cout<<">>> Light weight ACTS 1 (_L2) Launched... size: "<<kvdram1[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key<<endl; 
  
	cout<<">>> Light weight ACTS 2 (_L2) Launched... size: "<<kvdram2[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key<<endl; 
  
	cout<<">>> Light weight ACTS 3 (_L2) Launched... size: "<<kvdram3[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key<<endl; 
  
	cout<<">>> Light weight ACTS 4 (_L2) Launched... size: "<<kvdram4[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key<<endl; 
  
	cout<<">>> Light weight ACTS 5 (_L2) Launched... size: "<<kvdram5[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key<<endl; 
  
	cout<<">>> Light weight ACTS 6 (_L2) Launched... size: "<<kvdram6[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key<<endl; 
  
	cout<<">>> Light weight ACTS 7 (_L2) Launched... size: "<<kvdram7[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key<<endl; 
  
	cout<<">>> Light weight ACTS 8 (_L2) Launched... size: "<<kvdram8[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key<<endl; 
  
	cout<<">>> Light weight ACTS 9 (_L2) Launched... size: "<<kvdram9[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key<<endl; 
  
	cout<<">>> Light weight ACTS 10 (_L2) Launched... size: "<<kvdram10[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key<<endl; 
  
	cout<<">>> Light weight ACTS 11 (_L2) Launched... size: "<<kvdram11[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key<<endl; 
	#endif
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"acts::topkernel:: LOADFACTORFORREDUCE: "<<LOADFACTORFORREDUCE<<endl;
	cout<<"acts::topkernel:: APPLYVERTEXBUFFERSZ: "<<APPLYVERTEXBUFFERSZ<<endl;
	cout<<"acts::topkernel:: VDATAPACKINGFACTOR: "<<VDATAPACKINGFACTOR<<endl;
	#endif
	
	keyvalue_t buffer10[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer10
	keyvalue_t buffer20[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer20
	keyvalue_t buffer30[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer30
	keyvalue_t buffer40[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer40
	keyvalue_t buffer50[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer50
	keyvalue_t unused_buffer60[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer60
	keyvalue_t unused_buffer70[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer70
	keyvalue_t buffer11[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer11
	keyvalue_t buffer21[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer21
	keyvalue_t buffer31[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer31
	keyvalue_t buffer41[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer41
	keyvalue_t buffer51[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer51
	keyvalue_t unused_buffer61[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer61
	keyvalue_t unused_buffer71[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer71
	keyvalue_t buffer12[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer12
	keyvalue_t buffer22[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer22
	keyvalue_t buffer32[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer32
	keyvalue_t buffer42[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer42
	keyvalue_t buffer52[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer52
	keyvalue_t unused_buffer62[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer62
	keyvalue_t unused_buffer72[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer72
	keyvalue_t buffer13[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer13
	keyvalue_t buffer23[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer23
	keyvalue_t buffer33[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer33
	keyvalue_t buffer43[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer43
	keyvalue_t buffer53[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer53
	keyvalue_t unused_buffer63[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer63
	keyvalue_t unused_buffer73[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer73
	keyvalue_t buffer14[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer14
	keyvalue_t buffer24[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer24
	keyvalue_t buffer34[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer34
	keyvalue_t buffer44[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer44
	keyvalue_t buffer54[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer54
	keyvalue_t unused_buffer64[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer64
	keyvalue_t unused_buffer74[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer74
	keyvalue_t buffer15[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer15
	keyvalue_t buffer25[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer25
	keyvalue_t buffer35[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer35
	keyvalue_t buffer45[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer45
	keyvalue_t buffer55[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer55
	keyvalue_t unused_buffer65[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer65
	keyvalue_t unused_buffer75[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer75
	keyvalue_t buffer16[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer16
	keyvalue_t buffer26[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer26
	keyvalue_t buffer36[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer36
	keyvalue_t buffer46[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer46
	keyvalue_t buffer56[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer56
	keyvalue_t unused_buffer66[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer66
	keyvalue_t unused_buffer76[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer76
	keyvalue_t buffer17[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer17
	keyvalue_t buffer27[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer27
	keyvalue_t buffer37[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer37
	keyvalue_t buffer47[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer47
	keyvalue_t buffer57[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer57
	keyvalue_t unused_buffer67[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer67
	keyvalue_t unused_buffer77[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer77
	keyvalue_t buffer18[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer18
	keyvalue_t buffer28[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer28
	keyvalue_t buffer38[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer38
	keyvalue_t buffer48[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer48
	keyvalue_t buffer58[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer58
	keyvalue_t unused_buffer68[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer68
	keyvalue_t unused_buffer78[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer78
	keyvalue_t buffer19[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer19
	keyvalue_t buffer29[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer29
	keyvalue_t buffer39[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer39
	keyvalue_t buffer49[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer49
	keyvalue_t buffer59[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer59
	keyvalue_t unused_buffer69[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer69
	keyvalue_t unused_buffer79[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer79
	keyvalue_t buffer110[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer110
	keyvalue_t buffer210[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer210
	keyvalue_t buffer310[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer310
	keyvalue_t buffer410[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer410
	keyvalue_t buffer510[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer510
	keyvalue_t unused_buffer610[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer610
	keyvalue_t unused_buffer710[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer710
	keyvalue_t buffer111[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer111
	keyvalue_t buffer211[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer211
	keyvalue_t buffer311[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer311
	keyvalue_t buffer411[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer411
	keyvalue_t buffer511[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer511
	keyvalue_t unused_buffer611[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer611
	keyvalue_t unused_buffer711[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer711
	
	globalparams_t globalparams[NUMSUBCPUTHREADS];
	#pragma HLS ARRAY_PARTITION variable=globalparams complete
	travstate_t actvvstravstate[NUMSUBCPUTHREADS];
	#pragma HLS ARRAY_PARTITION variable=actvvstravstate complete
	
 
	globalparams[0] = getglobalparams(kvdram0);
 
	globalparams[1] = getglobalparams(kvdram1);
 
	globalparams[2] = getglobalparams(kvdram2);
 
	globalparams[3] = getglobalparams(kvdram3);
 
	globalparams[4] = getglobalparams(kvdram4);
 
	globalparams[5] = getglobalparams(kvdram5);
 
	globalparams[6] = getglobalparams(kvdram6);
 
	globalparams[7] = getglobalparams(kvdram7);
 
	globalparams[8] = getglobalparams(kvdram8);
 
	globalparams[9] = getglobalparams(kvdram9);
 
	globalparams[10] = getglobalparams(kvdram10);
 
	globalparams[11] = getglobalparams(kvdram11);
	
	unsigned int numGraphIters = globalparams[0].GraphIter;
	
	RUNITERATIONS_LOOP: for(unsigned int GraphIter=0; GraphIter<numGraphIters; GraphIter++){
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<">>> Light weight ACTS: Graph Iteration: "<<GraphIter<<endl;
		#endif 
		
		for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ globalparams[i].GraphIter = GraphIter; }
		
		#ifdef KOOOKOOOO
		if(globalparams[0].processcommand == ON){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> Light weight ACTS: Processing & partitioning instance 0 "<<endl;
			#endif
			dispatch_processandpartitiononly(kvdram0, buffer10, buffer20, buffer30, globalparams[0]);
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> Light weight ACTS: Processing & partitioning instance 1 "<<endl;
			#endif
			dispatch_processandpartitiononly(kvdram1, buffer11, buffer21, buffer31, globalparams[1]);
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> Light weight ACTS: Processing & partitioning instance 2 "<<endl;
			#endif
			dispatch_processandpartitiononly(kvdram2, buffer12, buffer22, buffer32, globalparams[2]);
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> Light weight ACTS: Processing & partitioning instance 3 "<<endl;
			#endif
			dispatch_processandpartitiononly(kvdram3, buffer13, buffer23, buffer33, globalparams[3]);
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> Light weight ACTS: Processing & partitioning instance 4 "<<endl;
			#endif
			dispatch_processandpartitiononly(kvdram4, buffer14, buffer24, buffer34, globalparams[4]);
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> Light weight ACTS: Processing & partitioning instance 5 "<<endl;
			#endif
			dispatch_processandpartitiononly(kvdram5, buffer15, buffer25, buffer35, globalparams[5]);
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> Light weight ACTS: Processing & partitioning instance 6 "<<endl;
			#endif
			dispatch_processandpartitiononly(kvdram6, buffer16, buffer26, buffer36, globalparams[6]);
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> Light weight ACTS: Processing & partitioning instance 7 "<<endl;
			#endif
			dispatch_processandpartitiononly(kvdram7, buffer17, buffer27, buffer37, globalparams[7]);
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> Light weight ACTS: Processing & partitioning instance 8 "<<endl;
			#endif
			dispatch_processandpartitiononly(kvdram8, buffer18, buffer28, buffer38, globalparams[8]);
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> Light weight ACTS: Processing & partitioning instance 9 "<<endl;
			#endif
			dispatch_processandpartitiononly(kvdram9, buffer19, buffer29, buffer39, globalparams[9]);
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> Light weight ACTS: Processing & partitioning instance 10 "<<endl;
			#endif
			dispatch_processandpartitiononly(kvdram10, buffer110, buffer210, buffer310, globalparams[10]);
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> Light weight ACTS: Processing & partitioning instance 11 "<<endl;
			#endif
			dispatch_processandpartitiononly(kvdram11, buffer111, buffer211, buffer311, globalparams[11]);
		}
		if(globalparams[0].reducecommand == ON){
			
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> Light weight ACTS: Reducing instance 0 "<<endl;
			#endif
			dispatch_reduceonly(kvdram0, buffer10, buffer20, buffer30, globalparams[0]);
			
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> Light weight ACTS: Reducing instance 1 "<<endl;
			#endif
			dispatch_reduceonly(kvdram1, buffer11, buffer21, buffer31, globalparams[1]);
			
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> Light weight ACTS: Reducing instance 2 "<<endl;
			#endif
			dispatch_reduceonly(kvdram2, buffer12, buffer22, buffer32, globalparams[2]);
			
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> Light weight ACTS: Reducing instance 3 "<<endl;
			#endif
			dispatch_reduceonly(kvdram3, buffer13, buffer23, buffer33, globalparams[3]);
			
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> Light weight ACTS: Reducing instance 4 "<<endl;
			#endif
			dispatch_reduceonly(kvdram4, buffer14, buffer24, buffer34, globalparams[4]);
			
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> Light weight ACTS: Reducing instance 5 "<<endl;
			#endif
			dispatch_reduceonly(kvdram5, buffer15, buffer25, buffer35, globalparams[5]);
			
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> Light weight ACTS: Reducing instance 6 "<<endl;
			#endif
			dispatch_reduceonly(kvdram6, buffer16, buffer26, buffer36, globalparams[6]);
			
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> Light weight ACTS: Reducing instance 7 "<<endl;
			#endif
			dispatch_reduceonly(kvdram7, buffer17, buffer27, buffer37, globalparams[7]);
			
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> Light weight ACTS: Reducing instance 8 "<<endl;
			#endif
			dispatch_reduceonly(kvdram8, buffer18, buffer28, buffer38, globalparams[8]);
			
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> Light weight ACTS: Reducing instance 9 "<<endl;
			#endif
			dispatch_reduceonly(kvdram9, buffer19, buffer29, buffer39, globalparams[9]);
			
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> Light weight ACTS: Reducing instance 10 "<<endl;
			#endif
			dispatch_reduceonly(kvdram10, buffer110, buffer210, buffer310, globalparams[10]);
			
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> Light weight ACTS: Reducing instance 11 "<<endl;
			#endif
			dispatch_reduceonly(kvdram11, buffer111, buffer211, buffer311, globalparams[11]);
		}
		#endif 

		#ifdef ALLVERTEXISACTIVE_ALGORITHM
		if(globalparams[0].processcommand == ON){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> Light weight ACTS: Processing, partitioning and reduce instance 0 "<<endl;
			#endif
			dispatch(kvdram0);
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> Light weight ACTS: Processing, partitioning and reduce instance 1 "<<endl;
			#endif
			dispatch(kvdram1);
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> Light weight ACTS: Processing, partitioning and reduce instance 2 "<<endl;
			#endif
			dispatch(kvdram2);
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> Light weight ACTS: Processing, partitioning and reduce instance 3 "<<endl;
			#endif
			dispatch(kvdram3);
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> Light weight ACTS: Processing, partitioning and reduce instance 4 "<<endl;
			#endif
			dispatch(kvdram4);
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> Light weight ACTS: Processing, partitioning and reduce instance 5 "<<endl;
			#endif
			dispatch(kvdram5);
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> Light weight ACTS: Processing, partitioning and reduce instance 6 "<<endl;
			#endif
			dispatch(kvdram6);
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> Light weight ACTS: Processing, partitioning and reduce instance 7 "<<endl;
			#endif
			dispatch(kvdram7);
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> Light weight ACTS: Processing, partitioning and reduce instance 8 "<<endl;
			#endif
			dispatch(kvdram8);
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> Light weight ACTS: Processing, partitioning and reduce instance 9 "<<endl;
			#endif
			dispatch(kvdram9);
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> Light weight ACTS: Processing, partitioning and reduce instance 10 "<<endl;
			#endif
			dispatch(kvdram10);
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> Light weight ACTS: Processing, partitioning and reduce instance 11 "<<endl;
			#endif
			dispatch(kvdram11);
		}
		#endif

		#ifdef RANDOMVERTEXISACTIVE_ALGORITHM
		batch_type sourcestatsmarker0 = 0;
		batch_type deststatsmarker0 = 1;
		batch_type destoffset0 = 0;
		batch_type sourcestatsmarker1 = 0;
		batch_type deststatsmarker1 = 1;
		batch_type destoffset1 = 0;
		batch_type sourcestatsmarker2 = 0;
		batch_type deststatsmarker2 = 1;
		batch_type destoffset2 = 0;
		batch_type sourcestatsmarker3 = 0;
		batch_type deststatsmarker3 = 1;
		batch_type destoffset3 = 0;
		batch_type sourcestatsmarker4 = 0;
		batch_type deststatsmarker4 = 1;
		batch_type destoffset4 = 0;
		batch_type sourcestatsmarker5 = 0;
		batch_type deststatsmarker5 = 1;
		batch_type destoffset5 = 0;
		batch_type sourcestatsmarker6 = 0;
		batch_type deststatsmarker6 = 1;
		batch_type destoffset6 = 0;
		batch_type sourcestatsmarker7 = 0;
		batch_type deststatsmarker7 = 1;
		batch_type destoffset7 = 0;
		batch_type sourcestatsmarker8 = 0;
		batch_type deststatsmarker8 = 1;
		batch_type destoffset8 = 0;
		batch_type sourcestatsmarker9 = 0;
		batch_type deststatsmarker9 = 1;
		batch_type destoffset9 = 0;
		batch_type sourcestatsmarker10 = 0;
		batch_type deststatsmarker10 = 1;
		batch_type destoffset10 = 0;
		batch_type sourcestatsmarker11 = 0;
		batch_type deststatsmarker11 = 1;
		batch_type destoffset11 = 0;
		if(globalparams[0].processcommand == ON){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> Light weight ACTS: Processing Instances 0-11 "<<endl;
			#endif
			dispatch_processonly(
				vdram,
kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9,kvdram10,kvdram11,	
				// buffer10,buffer11,buffer12,buffer13,buffer14,buffer15,buffer16,buffer17,buffer18,buffer19,buffer110,buffer111,	
buffer20,buffer21,buffer22,buffer23,buffer24,buffer25,buffer26,buffer27,buffer28,buffer29,buffer210,buffer211,		
buffer30,buffer31,buffer32,buffer33,buffer34,buffer35,buffer36,buffer37,buffer38,buffer39,buffer310,buffer311,	
				#ifndef COMPACTEDGES	
buffer40,buffer41,buffer42,buffer43,buffer44,buffer45,buffer46,buffer47,buffer48,buffer49,buffer410,buffer411,	
				#endif 
				globalparams, &sourcestatsmarker0, &deststatsmarker0, &destoffset0);
		
			sourcestatsmarker0 = sourcestatsmarker0; deststatsmarker0 = deststatsmarker0; destoffset0 = destoffset0;
			sourcestatsmarker1 = sourcestatsmarker0; deststatsmarker1 = deststatsmarker0; destoffset1 = destoffset0;
			sourcestatsmarker2 = sourcestatsmarker0; deststatsmarker2 = deststatsmarker0; destoffset2 = destoffset0;
			sourcestatsmarker3 = sourcestatsmarker0; deststatsmarker3 = deststatsmarker0; destoffset3 = destoffset0;
			sourcestatsmarker4 = sourcestatsmarker0; deststatsmarker4 = deststatsmarker0; destoffset4 = destoffset0;
			sourcestatsmarker5 = sourcestatsmarker0; deststatsmarker5 = deststatsmarker0; destoffset5 = destoffset0;
			sourcestatsmarker6 = sourcestatsmarker0; deststatsmarker6 = deststatsmarker0; destoffset6 = destoffset0;
			sourcestatsmarker7 = sourcestatsmarker0; deststatsmarker7 = deststatsmarker0; destoffset7 = destoffset0;
			sourcestatsmarker8 = sourcestatsmarker0; deststatsmarker8 = deststatsmarker0; destoffset8 = destoffset0;
			sourcestatsmarker9 = sourcestatsmarker0; deststatsmarker9 = deststatsmarker0; destoffset9 = destoffset0;
			sourcestatsmarker10 = sourcestatsmarker0; deststatsmarker10 = deststatsmarker0; destoffset10 = destoffset0;
			sourcestatsmarker11 = sourcestatsmarker0; deststatsmarker11 = deststatsmarker0; destoffset11 = destoffset0;
		}
		if(globalparams[0].partitioncommand == ON){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> Light weight ACTS: Partitioning Instance: ";
			#endif
 
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"0 ";
			#endif
			dispatch_partitiononly(
				kvdram0,
				buffer10, 
				buffer20, 
				buffer30, 
				buffer40, 
				buffer50, 
				&globalparams[0], &sourcestatsmarker0, &deststatsmarker0, &destoffset0);
 
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"1 ";
			#endif
			dispatch_partitiononly(
				kvdram1,
				buffer11, 
				buffer21, 
				buffer31, 
				buffer41, 
				buffer51, 
				&globalparams[1], &sourcestatsmarker1, &deststatsmarker1, &destoffset1);
 
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"2 ";
			#endif
			dispatch_partitiononly(
				kvdram2,
				buffer12, 
				buffer22, 
				buffer32, 
				buffer42, 
				buffer52, 
				&globalparams[2], &sourcestatsmarker2, &deststatsmarker2, &destoffset2);
 
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"3 ";
			#endif
			dispatch_partitiononly(
				kvdram3,
				buffer13, 
				buffer23, 
				buffer33, 
				buffer43, 
				buffer53, 
				&globalparams[3], &sourcestatsmarker3, &deststatsmarker3, &destoffset3);
 
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"4 ";
			#endif
			dispatch_partitiononly(
				kvdram4,
				buffer14, 
				buffer24, 
				buffer34, 
				buffer44, 
				buffer54, 
				&globalparams[4], &sourcestatsmarker4, &deststatsmarker4, &destoffset4);
 
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"5 ";
			#endif
			dispatch_partitiononly(
				kvdram5,
				buffer15, 
				buffer25, 
				buffer35, 
				buffer45, 
				buffer55, 
				&globalparams[5], &sourcestatsmarker5, &deststatsmarker5, &destoffset5);
 
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"6 ";
			#endif
			dispatch_partitiononly(
				kvdram6,
				buffer16, 
				buffer26, 
				buffer36, 
				buffer46, 
				buffer56, 
				&globalparams[6], &sourcestatsmarker6, &deststatsmarker6, &destoffset6);
 
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"7 ";
			#endif
			dispatch_partitiononly(
				kvdram7,
				buffer17, 
				buffer27, 
				buffer37, 
				buffer47, 
				buffer57, 
				&globalparams[7], &sourcestatsmarker7, &deststatsmarker7, &destoffset7);
 
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"8 ";
			#endif
			dispatch_partitiononly(
				kvdram8,
				buffer18, 
				buffer28, 
				buffer38, 
				buffer48, 
				buffer58, 
				&globalparams[8], &sourcestatsmarker8, &deststatsmarker8, &destoffset8);
 
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"9 ";
			#endif
			dispatch_partitiononly(
				kvdram9,
				buffer19, 
				buffer29, 
				buffer39, 
				buffer49, 
				buffer59, 
				&globalparams[9], &sourcestatsmarker9, &deststatsmarker9, &destoffset9);
 
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"10 ";
			#endif
			dispatch_partitiononly(
				kvdram10,
				buffer110, 
				buffer210, 
				buffer310, 
				buffer410, 
				buffer510, 
				&globalparams[10], &sourcestatsmarker10, &deststatsmarker10, &destoffset10);
 
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"11 ";
			#endif
			dispatch_partitiononly(
				kvdram11,
				buffer111, 
				buffer211, 
				buffer311, 
				buffer411, 
				buffer511, 
				&globalparams[11], &sourcestatsmarker11, &deststatsmarker11, &destoffset11);
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<endl;
			#endif 
		}
		if(globalparams[0].reducecommand == ON){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> Light weight ACTS: Reducing Instances 0-11 "<<endl;
			#endif
			#ifdef REDUCEUPDATES
			actvvstravstate[0] = 
				dispatch_reduceonly_parallelsync(
					vdram,
kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9,kvdram10,kvdram11,		
					actvvstravstate[0], globalparams);	
			#endif
		}
		
		#ifdef LOGKERNELSTATS
		#ifdef _WIDEWORD
		vdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BASEOFFSET_RETURNVALUES + GraphIter + 1].range(31, 0) = actvvstravstate[0].i;
		#else 
		vdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BASEOFFSET_RETURNVALUES + GraphIter + 1].data[0].key = actvvstravstate[0].i;	
		#endif 
		#endif
		#endif
		
		for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){
		// #pragma HLS UNROLL
			globalparams[i].actvvsize = actvvstravstate[0].i;
		}
		if(actvvstravstate[0].i == 0){ break; }
	}
	return;
}
#endif
}




