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

#define NUMPARTITIONUPDATESPIPELINES 2
#if NUMPARTITIONUPDATESPIPELINES==1
#define PUP0
#endif 
#if NUMPARTITIONUPDATESPIPELINES==2
#define PUP0
#define PUP1
#endif

#define NUMREDUCEUPDATESPIPELINES 2 //1//2 // where the error lies.
#if NUMREDUCEUPDATESPIPELINES==1
#define RUP0
#endif 
#if NUMREDUCEUPDATESPIPELINES==2
#define RUP0
#define RUP1
#endif

#define NUMREDUCEPARTITIONPIPELINES 2 // 2 //2
#if NUMREDUCEPARTITIONPIPELINES==1
#define RPP0
#endif 
#if NUMREDUCEPARTITIONPIPELINES==2
#define RPP0
#define RPP1
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
#ifdef _DEBUGMODE_STATS
unsigned int stats_greaterthan512;
unsigned int stats_lessthan512;
unsigned int stats_totals;
#endif 

// functions (utility)
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
batch_type
	#ifdef SW 
	acts::
	#endif
getrangeforeachllop(globalparams_t globalparams){
	analysis_type analysis_loop1 = TREE_DEPTH;
	unsigned int range = globalparams.batch_range;
	for(unsigned int i=0; i<globalparams.treedepth; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loop1 avg=analysis_loop1	
		range = range / NUM_PARTITIONS;
	}
	return range;
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
getwritesz1(buffer_type realsize_kvs){
	buffer_type size_kvs = 0;
	#ifdef ENABLE_APPROXIMATEWRITES1
	if(realsize_kvs == 0){ size_kvs = 0; }
	else if(realsize_kvs > 0 && realsize_kvs < (64*1)){ size_kvs = (64*1); }
	else if(realsize_kvs >= (64*1) && realsize_kvs < (64*2)){ size_kvs = (64*2); }
	else if(realsize_kvs >= (64*2) && realsize_kvs < (64*3)){ size_kvs = (64*3); }
	else if(realsize_kvs >= (64*3) && realsize_kvs < (64*4)){ size_kvs = (64*4); }
	else if(realsize_kvs >= (64*4) && realsize_kvs < (64*5)){ size_kvs = (64*5); }
	else if(realsize_kvs >= (64*5) && realsize_kvs < (64*6)){ size_kvs = (64*6); }
	else if(realsize_kvs >= (64*6) && realsize_kvs < (64*7)){ size_kvs = (64*7); }
	else if(realsize_kvs >= (64*7) && realsize_kvs < (64*8)){ size_kvs = (64*8); }
	else { size_kvs = PADDEDDESTBUFFER_SIZE; }
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
	globalparams.kvstatssz = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_SIZE_KVSTATSSZ].range(31, 0);
	globalparams.baseoffset_returnvalues = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BASEOFFSET_RETURNVALUES].range(31, 0);
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
	globalparams.kvstatssz = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_SIZE_KVSTATSSZ].data[0].key;
	globalparams.baseoffset_returnvalues = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BASEOFFSET_RETURNVALUES].data[0].key;
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
	sweepparams.upperpartition = (sweepparams.upperlimit - globalparams.destvoffset) >> (globalparams.batch_range_pow - (NUM_PARTITIONS_POW * currentLOP)); //

	sweepparams.source_partition = source_partition;
	return sweepparams;
}
sweepparams_t 
	#ifdef SW 
	acts::
	#endif 
updatesweepparams(sweepparams_t sweepparams, step_type currentLOP, batch_type source_partition, batch_type sourceskipsize, batch_type llopsz, globalparams_t globalparams){
	sweepparams.source_partition = source_partition;
	sweepparams.upperlimit = globalparams.destvoffset + (source_partition * sourceskipsize * llopsz);
	sweepparams.upperpartition = (sweepparams.upperlimit - globalparams.destvoffset) >> (globalparams.batch_range_pow - (NUM_PARTITIONS_POW * currentLOP));
	
	return sweepparams;
}
travstate_t 
	#ifdef SW 
	acts::
	#endif 
gettravstate(bool_type enable, uint512_dt * kvdram, globalparams_t globalparams, step_type currentLOP, batch_type sourcestatsmarker, keyvalue_t travstates[MAXLOADFACTORFORREDUCE]){			
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
travstate_t 
	#ifdef SW 
	acts::
	#endif 
updatetravstate(bool_type enable, keyvalue_t travstatekv, step_type currentLOP, globalparams_t globalparams){			
	travstate_t travstate;
	if(enable == OFF){ return travstate; }
	keyvalue_t nextkeyvalue;
	
	if(currentLOP == 0){ nextkeyvalue.key = globalparams.runsize; }
	else if(currentLOP == 1){ nextkeyvalue.key = globalparams.runsize; }
	else { nextkeyvalue.key = travstatekv.key + travstatekv.value; }
		
	travstate.begin_kvs = travstatekv.key / VECTOR_SIZE; 
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

// functions (dram access)
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
readkeyvalues2(bool_type enable, uint512_dt * kvdram, keyvalue_t buffer[NUM_PARTITIONS][PADDEDDESTBUFFER_SIZE], batch_type offset1_kvs, batch_type offset2_kvs, buffer_type size1_kvs, buffer_type size2_kvs){
	#pragma HLS function_instantiate variable=kvdram
	if(enable == OFF){ return; }
		
	READVERTICES_LOOP1: for (buffer_type i=0; i<size1_kvs; i++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		buffer[0][i].key = kvdram[offset1_kvs + i].range(31, 0); 
		buffer[0][i].value = kvdram[offset1_kvs + i].range(63, 32); 
		buffer[1][i].key = kvdram[offset1_kvs + i].range(95, 64); 
		buffer[1][i].value = kvdram[offset1_kvs + i].range(127, 96); 
		buffer[2][i].key = kvdram[offset1_kvs + i].range(159, 128); 
		buffer[2][i].value = kvdram[offset1_kvs + i].range(191, 160); 
		buffer[3][i].key = kvdram[offset1_kvs + i].range(223, 192); 
		buffer[3][i].value = kvdram[offset1_kvs + i].range(255, 224); 
		buffer[4][i].key = kvdram[offset1_kvs + i].range(287, 256); 
		buffer[4][i].value = kvdram[offset1_kvs + i].range(319, 288); 
		buffer[5][i].key = kvdram[offset1_kvs + i].range(351, 320); 
		buffer[5][i].value = kvdram[offset1_kvs + i].range(383, 352); 
		buffer[6][i].key = kvdram[offset1_kvs + i].range(415, 384); 
		buffer[6][i].value = kvdram[offset1_kvs + i].range(447, 416); 
		buffer[7][i].key = kvdram[offset1_kvs + i].range(479, 448); 
		buffer[7][i].value = kvdram[offset1_kvs + i].range(511, 480); 
		#else 
		buffer[0][i] = kvdram[offset1_kvs + i].data[0]; 
		buffer[1][i] = kvdram[offset1_kvs + i].data[1]; 
		buffer[2][i] = kvdram[offset1_kvs + i].data[2]; 
		buffer[3][i] = kvdram[offset1_kvs + i].data[3]; 
		buffer[4][i] = kvdram[offset1_kvs + i].data[4]; 
		buffer[5][i] = kvdram[offset1_kvs + i].data[5]; 
		buffer[6][i] = kvdram[offset1_kvs + i].data[6]; 
		buffer[7][i] = kvdram[offset1_kvs + i].data[7]; 
		#endif 
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	READVERTICES_LOOP2: for (buffer_type i=0; i<size2_kvs; i++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		buffer[8][i].key = kvdram[offset2_kvs + i].range(31, 0); 
		buffer[8][i].value = kvdram[offset2_kvs + i].range(63, 32); 
		buffer[9][i].key = kvdram[offset2_kvs + i].range(95, 64); 
		buffer[9][i].value = kvdram[offset2_kvs + i].range(127, 96); 
		buffer[10][i].key = kvdram[offset2_kvs + i].range(159, 128); 
		buffer[10][i].value = kvdram[offset2_kvs + i].range(191, 160); 
		buffer[11][i].key = kvdram[offset2_kvs + i].range(223, 192); 
		buffer[11][i].value = kvdram[offset2_kvs + i].range(255, 224); 
		buffer[12][i].key = kvdram[offset2_kvs + i].range(287, 256); 
		buffer[12][i].value = kvdram[offset2_kvs + i].range(319, 288); 
		buffer[13][i].key = kvdram[offset2_kvs + i].range(351, 320); 
		buffer[13][i].value = kvdram[offset2_kvs + i].range(383, 352); 
		buffer[14][i].key = kvdram[offset2_kvs + i].range(415, 384); 
		buffer[14][i].value = kvdram[offset2_kvs + i].range(447, 416); 
		buffer[15][i].key = kvdram[offset2_kvs + i].range(479, 448); 
		buffer[15][i].value = kvdram[offset2_kvs + i].range(511, 480); 
		#else 
		buffer[8][i] = kvdram[offset2_kvs + i].data[0]; 
		buffer[9][i] = kvdram[offset2_kvs + i].data[1]; 
		buffer[10][i] = kvdram[offset2_kvs + i].data[2]; 
		buffer[11][i] = kvdram[offset2_kvs + i].data[3]; 
		buffer[12][i] = kvdram[offset2_kvs + i].data[4]; 
		buffer[13][i] = kvdram[offset2_kvs + i].data[5]; 
		buffer[14][i] = kvdram[offset2_kvs + i].data[6]; 
		buffer[15][i] = kvdram[offset2_kvs + i].data[7]; 
		#endif 
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<< TIMINGRESULTSCOLOR<<"readkeyvalues:: vertices read: offset: "<<offset1_kvs * VECTOR_SIZE<<"-"<<(offset1_kvs + size1_kvs) * VECTOR_SIZE<<", number of vertex datas read: "<<(size1_kvs * VECTOR_SIZE * 2)<<" ("<<size1_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl;
	cout<< TIMINGRESULTSCOLOR<<"readkeyvalues:: vertices read: offset: "<<offset2_kvs * VECTOR_SIZE<<"-"<<(offset2_kvs + size2_kvs) * VECTOR_SIZE<<", number of vertex datas read: "<<(size2_kvs * VECTOR_SIZE * 2)<<" ("<<size2_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl;
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(offset1_kvs >= BASEOFFSET_VERTEXPTR_KVS && offset1_kvs < BASEOFFSET_VERTEXPTR_KVS + VERTEXPTRSSZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"readkeyvalues:: vertices read: offset: "<<(offset1_kvs - BASEOFFSET_VERTEXPTR_KVS) * VECTOR_SIZE<<"-"<<(offset1_kvs + size1_kvs - BASEOFFSET_VERTEXPTR_KVS) * VECTOR_SIZE<<", number of vertex datas read: "<<(size1_kvs * VECTOR_SIZE * 2)<<" ("<<size1_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	if(offset1_kvs >= BASEOFFSET_VERTICESDATA_KVS && offset1_kvs < BASEOFFSET_VERTICESDATA_KVS + VERTICESDATASZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"readkeyvalues:: vertices read: offset: "<<(offset1_kvs - BASEOFFSET_VERTICESDATA_KVS) * VECTOR_SIZE<<"-"<<(offset1_kvs + size1_kvs - BASEOFFSET_VERTICESDATA_KVS) * VECTOR_SIZE<<", number of vertex datas read: "<<(size1_kvs * VECTOR_SIZE * 2)<<" ("<<size1_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	if(offset2_kvs >= BASEOFFSET_VERTEXPTR_KVS && offset2_kvs < BASEOFFSET_VERTEXPTR_KVS + VERTEXPTRSSZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"readkeyvalues:: vertices read: offset: "<<(offset2_kvs - BASEOFFSET_VERTEXPTR_KVS) * VECTOR_SIZE<<"-"<<(offset2_kvs + size2_kvs - BASEOFFSET_VERTEXPTR_KVS) * VECTOR_SIZE<<", number of vertex datas read: "<<(size2_kvs * VECTOR_SIZE * 2)<<" ("<<size2_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	if(offset2_kvs >= BASEOFFSET_VERTICESDATA_KVS && offset2_kvs < BASEOFFSET_VERTICESDATA_KVS + VERTICESDATASZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"readkeyvalues:: vertices read: offset: "<<(offset2_kvs - BASEOFFSET_VERTICESDATA_KVS) * VECTOR_SIZE<<"-"<<(offset2_kvs + size2_kvs - BASEOFFSET_VERTICESDATA_KVS) * VECTOR_SIZE<<", number of vertex datas read: "<<(size2_kvs * VECTOR_SIZE * 2)<<" ("<<size2_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	#endif
	return;
}
	
void 
	#ifdef SW 
	acts::
	#endif 
savekeyvalues2(bool_type enable, uint512_dt * kvdram, keyvalue_t buffer[NUM_PARTITIONS][PADDEDDESTBUFFER_SIZE], batch_type offset1_kvs, batch_type offset2_kvs, buffer_type size1_kvs, buffer_type size2_kvs){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount =  (APPLYVERTEXBUFFERSZ / VDATAPACKINGFACTOR) / 2;
	
	SAVEVERTICES_LOOP1: for (buffer_type i=0; i<size1_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		kvdram[offset1_kvs + i].range(31, 0) = buffer[0][i].key; 
		kvdram[offset1_kvs + i].range(63, 32) = buffer[0][i].value; 
		kvdram[offset1_kvs + i].range(95, 64) = buffer[1][i].key; 
		kvdram[offset1_kvs + i].range(127, 96) = buffer[1][i].value; 
		kvdram[offset1_kvs + i].range(159, 128) = buffer[2][i].key; 
		kvdram[offset1_kvs + i].range(191, 160) = buffer[2][i].value; 
		kvdram[offset1_kvs + i].range(223, 192) = buffer[3][i].key; 
		kvdram[offset1_kvs + i].range(255, 224) = buffer[3][i].value; 
		kvdram[offset1_kvs + i].range(287, 256) = buffer[4][i].key; 
		kvdram[offset1_kvs + i].range(319, 288) = buffer[4][i].value; 
		kvdram[offset1_kvs + i].range(351, 320) = buffer[5][i].key; 
		kvdram[offset1_kvs + i].range(383, 352) = buffer[5][i].value; 
		kvdram[offset1_kvs + i].range(415, 384) = buffer[6][i].key; 
		kvdram[offset1_kvs + i].range(447, 416) = buffer[6][i].value; 
		kvdram[offset1_kvs + i].range(479, 448) = buffer[7][i].key; 
		kvdram[offset1_kvs + i].range(511, 480) = buffer[7][i].value; 
		#else 
		kvdram[offset1_kvs + i].data[0] = buffer[0][i];
		kvdram[offset1_kvs + i].data[1] = buffer[1][i];
		kvdram[offset1_kvs + i].data[2] = buffer[2][i];
		kvdram[offset1_kvs + i].data[3] = buffer[3][i];
		kvdram[offset1_kvs + i].data[4] = buffer[4][i];
		kvdram[offset1_kvs + i].data[5] = buffer[5][i];
		kvdram[offset1_kvs + i].data[6] = buffer[6][i];
		kvdram[offset1_kvs + i].data[7] = buffer[7][i];
		#endif 
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvswritten(VECTOR_SIZE);
		#endif
	}
	SAVEVERTICES_LOOP2: for (buffer_type i=0; i<size2_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		kvdram[offset2_kvs + i].range(31, 0) = buffer[8][i].key; 
		kvdram[offset2_kvs + i].range(63, 32) = buffer[8][i].value; 
		kvdram[offset2_kvs + i].range(95, 64) = buffer[9][i].key; 
		kvdram[offset2_kvs + i].range(127, 96) = buffer[9][i].value; 
		kvdram[offset2_kvs + i].range(159, 128) = buffer[10][i].key; 
		kvdram[offset2_kvs + i].range(191, 160) = buffer[10][i].value; 
		kvdram[offset2_kvs + i].range(223, 192) = buffer[11][i].key; 
		kvdram[offset2_kvs + i].range(255, 224) = buffer[11][i].value; 
		kvdram[offset2_kvs + i].range(287, 256) = buffer[12][i].key; 
		kvdram[offset2_kvs + i].range(319, 288) = buffer[12][i].value; 
		kvdram[offset2_kvs + i].range(351, 320) = buffer[13][i].key; 
		kvdram[offset2_kvs + i].range(383, 352) = buffer[13][i].value; 
		kvdram[offset2_kvs + i].range(415, 384) = buffer[14][i].key; 
		kvdram[offset2_kvs + i].range(447, 416) = buffer[14][i].value; 
		kvdram[offset2_kvs + i].range(479, 448) = buffer[15][i].key; 
		kvdram[offset2_kvs + i].range(511, 480) = buffer[15][i].value; 
		#else 
		kvdram[offset2_kvs + i].data[0] = buffer[8][i];
		kvdram[offset2_kvs + i].data[1] = buffer[9][i];
		kvdram[offset2_kvs + i].data[2] = buffer[10][i];
		kvdram[offset2_kvs + i].data[3] = buffer[11][i];
		kvdram[offset2_kvs + i].data[4] = buffer[12][i];
		kvdram[offset2_kvs + i].data[5] = buffer[13][i];
		kvdram[offset2_kvs + i].data[6] = buffer[14][i];
		kvdram[offset2_kvs + i].data[7] = buffer[15][i];
		#endif 
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvswritten(VECTOR_SIZE);
		#endif
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"savekeyvalues:: vertices saved: offset: "<<offset1_kvs * VECTOR_SIZE<<"-"<<(offset1_kvs + size1_kvs) * VECTOR_SIZE<<", number of vertex datas written: "<<(size1_kvs * VECTOR_SIZE * 2)<<" ("<<size1_kvs * VECTOR_SIZE<<" keyvalues written)"<<endl;
	cout<<"savekeyvalues:: vertices saved: offset: "<<offset2_kvs * VECTOR_SIZE<<"-"<<(offset2_kvs + size2_kvs) * VECTOR_SIZE<<", number of vertex datas written: "<<(size2_kvs * VECTOR_SIZE * 2)<<" ("<<size2_kvs * VECTOR_SIZE<<" keyvalues written)"<<endl;
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(offset1_kvs >= BASEOFFSET_VERTEXPTR_KVS && offset1_kvs < BASEOFFSET_VERTEXPTR_KVS + VERTEXPTRSSZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"savekeyvalues:: vertices saved: offset: "<<offset1_kvs-BASEOFFSET_VERTEXPTR_KVS * VECTOR_SIZE<<"-"<<((offset1_kvs + size1_kvs)-BASEOFFSET_VERTEXPTR_KVS) * VECTOR_SIZE<<", number of vertex datas written: "<<(size1_kvs * VECTOR_SIZE * 2)<<" ("<<size1_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	if(offset1_kvs >= BASEOFFSET_VERTICESDATA_KVS && offset1_kvs < BASEOFFSET_VERTICESDATA_KVS + VERTICESDATASZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"savekeyvalues:: vertices saved: offset: "<<(offset1_kvs-BASEOFFSET_VERTICESDATA_KVS) * VECTOR_SIZE<<"-"<<((offset1_kvs-BASEOFFSET_VERTICESDATA_KVS) + size1_kvs) * VECTOR_SIZE<<", number of vertex datas written: "<<(size1_kvs * VECTOR_SIZE * 2)<<" ("<<size1_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	if(offset2_kvs >= BASEOFFSET_VERTEXPTR_KVS && offset2_kvs < BASEOFFSET_VERTEXPTR_KVS + VERTEXPTRSSZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"savekeyvalues:: vertices saved: offset: "<<offset2_kvs-BASEOFFSET_VERTEXPTR_KVS * VECTOR_SIZE<<"-"<<((offset2_kvs + size2_kvs)-BASEOFFSET_VERTEXPTR_KVS) * VECTOR_SIZE<<", number of vertex datas written: "<<(size2_kvs * VECTOR_SIZE * 2)<<" ("<<size2_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	if(offset2_kvs >= BASEOFFSET_VERTICESDATA_KVS && offset2_kvs < BASEOFFSET_VERTICESDATA_KVS + VERTICESDATASZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"savekeyvalues:: vertices saved: offset: "<<(offset2_kvs-BASEOFFSET_VERTICESDATA_KVS) * VECTOR_SIZE<<"-"<<((offset2_kvs-BASEOFFSET_VERTICESDATA_KVS) + size2_kvs) * VECTOR_SIZE<<", number of vertex datas written: "<<(size2_kvs * VECTOR_SIZE * 2)<<" ("<<size2_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	#endif
	return;
}

// function (actit)
void 
	#ifdef SW 
	acts::
	#endif
runpipeline(bool_type enable, keyvalue_t bufferA[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t buffer1capsule[VECTOR_SIZE][NUM_PARTITIONS], 
				keyvalue_t bufferB[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t bufferBcapsule[4][NUM_PARTITIONS], 
					keyvalue_t bufferC[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t bufferCcapsule[2][NUM_PARTITIONS],
						keyvalue_t bufferD[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t bufferDcapsule[NUM_PARTITIONS],
							unsigned int currentLOP, sweepparams_t sweepparams, buffer_type cutoff, buffer_type cutoffs[VECTOR_SIZE], batch_type shiftcount, globalparams_t globalparams){		
	// #pragma HLS function_instantiate variable=bufferA // CRITICAL REMOVEME? NEWCHANGE.
	// #pragma HLS INLINE OFF
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
				bufferD[0][yoffset0] = kvC0[0]; bufferD[1][yoffset0] = kvC1[0]; bufferD[2][yoffset0] = kvC2[0]; bufferD[3][yoffset0] = kvC3[0]; 
			} else {
				bufferD[4][yoffset0] = kvC0[0]; bufferD[5][yoffset0] = kvC1[0]; bufferD[6][yoffset0] = kvC2[0]; bufferD[7][yoffset0] = kvC3[0]; 
			}
			if(!((kvC0[0].value == INVALIDDATA) && (kvC1[0].value == INVALIDDATA) && (kvC2[0].value == INVALIDDATA) && (kvC3[0].value == INVALIDDATA))){ tempbufferDcapsule += 4; }
			
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
				bufferD[0][yoffset1] = _kvC0[0]; bufferD[1][yoffset1] = _kvC1[0]; bufferD[2][yoffset1] = _kvC2[0]; bufferD[3][yoffset1] = _kvC3[0]; 
			} else {
				bufferD[4][yoffset1] = _kvC0[0]; bufferD[5][yoffset1] = _kvC1[0]; bufferD[6][yoffset1] = _kvC2[0]; bufferD[7][yoffset1] = _kvC3[0]; 
			}
			if(!((_kvC0[0].value == INVALIDDATA) && (_kvC1[0].value == INVALIDDATA) && (_kvC2[0].value == INVALIDDATA) && (_kvC3[0].value == INVALIDDATA))){ tempbufferDcapsule += 4; }
			
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
				bufferD[0][yoffset2] = kvC4[0]; bufferD[1][yoffset2] = kvC5[0]; bufferD[2][yoffset2] = kvC6[0]; bufferD[3][yoffset2] = kvC7[0]; 
			} else {
				bufferD[4][yoffset2] = kvC4[0]; bufferD[5][yoffset2] = kvC5[0]; bufferD[6][yoffset2] = kvC6[0]; bufferD[7][yoffset2] = kvC7[0]; 
			}
			if(!((kvC4[0].value == INVALIDDATA) && (kvC5[0].value == INVALIDDATA) && (kvC6[0].value == INVALIDDATA) && (kvC7[0].value == INVALIDDATA))){ tempbufferDcapsule += 4; }
			
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
				bufferD[0][yoffset3] = _kvC4[0]; bufferD[1][yoffset3] = _kvC5[0]; bufferD[2][yoffset3] = _kvC6[0]; bufferD[3][yoffset3] = _kvC7[0]; 
			} else {
				bufferD[4][yoffset3] = _kvC4[0]; bufferD[5][yoffset3] = _kvC5[0]; bufferD[6][yoffset3] = _kvC6[0]; bufferD[7][yoffset3] = _kvC7[0]; 
			}
			if(!((_kvC4[0].value == INVALIDDATA) && (_kvC5[0].value == INVALIDDATA) && (_kvC6[0].value == INVALIDDATA) && (_kvC7[0].value == INVALIDDATA))){ tempbufferDcapsule += 4; }
		}
	}
	for(partition_type p=0; p<NUM_PARTITIONS; p++){ 
	#pragma HLS PIPELINE II=1
		bufferDcapsule.value = tempbufferDcapsule; 
	}
	for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		keyvalue_t dummykv;
		dummykv.key = INVALIDDATA;
		dummykv.value = INVALIDDATA;
		if(bufferDcapsule.value % 8 == 4){
			unsigned int yoffset = (bufferDcapsule.key + bufferDcapsule.value) / VECTOR_SIZE;
			bufferD[4][yoffset] = dummykv; bufferD[5][yoffset] = dummykv; bufferD[6][yoffset] = dummykv; bufferD[7][yoffset] = dummykv; 
			bufferDcapsule.value += 4;
		}
		#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
		actsutilityobj->checkfordivisibleby(enablebufferD, "bufferDcapsule.value", bufferDcapsule.value, 8);
		#endif
	}
	
	#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
	actsutilityobj->checkfordivisibleby(enablebufferD, "bufferDcapsule.value", bufferDcapsule.value, 8);
	#endif
	return bufferDcapsule;
}

// functions (process)
void
	#ifdef SW 
	acts::
	#endif 
replicatevdata(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t kvdrambuffer[NUM_PARTITIONS][PADDEDDESTBUFFER_SIZE], buffer_type sourceoffset, buffer_type size){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount = PADDEDDESTBUFFER_SIZE;

	vector_type src_v = 0;
	buffer_type src_i = 0;
	buffer_type sourceoffset_kvs = sourceoffset / VECTOR_SIZE;
	REPLICATEDATA_LOOP: for(buffer_type i=0; i<size; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("replicatedata.1", i, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, sourceoffset, sourceoffset_kvs, size);
		actsutilityobj->checkoutofbounds("replicatedata.2", sourceoffset + src_i, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, sourceoffset, sourceoffset_kvs, size);
		#endif
		
		keyvalue_t keyvalue = sourcebuffer[src_v][sourceoffset_kvs + src_i]; // get a single source
		src_v++; if(src_v==VECTOR_SIZE){ src_v=0; src_i+=1; }
		
		kvdrambuffer[0][i] = keyvalue; // load to multiple destinations
		kvdrambuffer[1][i] = keyvalue; // load to multiple destinations
		kvdrambuffer[2][i] = keyvalue; // load to multiple destinations
		kvdrambuffer[3][i] = keyvalue; // load to multiple destinations
		kvdrambuffer[4][i] = keyvalue; // load to multiple destinations
		kvdrambuffer[5][i] = keyvalue; // load to multiple destinations
		kvdrambuffer[6][i] = keyvalue; // load to multiple destinations
		kvdrambuffer[7][i] = keyvalue; // load to multiple destinations
	}
	return;
}

buffer_type
	#ifdef SW 
	acts::
	#endif
generateoffsets_random(
		uint512_dt * kvdram, 
		keyvalue_t actvvs[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t offsets[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
		travstate_t actvvtravstate, globalparams_t globalparams, batch_type actvvsoffset_kvs){ 
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
		if(actvv_id != INVALIDDATA && actvv_id != UNUSEDDATA){ cout<<"generateoffsets_random:: index: "<<index<<", active vid: "<<actvv_id<<", sourcedata: "<<sourcedata<<", num actvvs: "<<chunk_size * VECTOR_SIZE<<", num actvvs processed: "<<((actvvtravstate.i_kvs * VECTOR_SIZE) + index)<<endl; }
		#endif
	
		if((actvv_id != INVALIDDATA && actvv_id != UNUSEDDATA) && ((actvvtravstate.i_kvs * VECTOR_SIZE) + index < globalparams.actvvsize)){ // NEWCHANGE.
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
		keyvalue_t vptrs[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t offsets[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
		travstate_t actvvtravstate, globalparams_t globalparams, batch_type actvvsoffset_kvs){ 
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
	// cout<<"--- processoffsets: edges_count: "<<edges_count<<endl;
		
	*_saveoffset_kvs = saveoffset_kvs;
	*_cachebeginoffset_kvs = cachebeginoffset_kvs;
	#ifdef _DEBUGMODE_STATS
	*_edges_count = edges_count;
	*_edgesdstv_sum = edgesdstv_sum;
	#endif
	return;
}

// functions (partition)
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
saveglobalstats(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ], batch_type offset_kvs){ 
	if(enable == OFF){ return; }
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("saveglobalstats", offset_kvs + NUM_PARTITIONS, BASEOFFSET_STATSDRAM_KVS + KVSTATSDRAMSZ_KVS + 1, offset_kvs, NUM_PARTITIONS, KVSTATSDRAMSZ_KVS);
	#endif
	
	SAVEGLOBALSTATS_LOOP: for (buffer_type i=0; i<NUM_PARTITIONS; i++){
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
	return;
}

void 
	#ifdef SW 
	acts::
	#endif
collectstats(bool_type enable, uint512_dt * kvdram, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ], config_t config, globalparams_t globalparams, sweepparams_t sweepparams, travstate_t ctravstate){
	#pragma HLS INLINE
	analysis_type analysis_collectstatsloop = KVDATA_BATCHSIZE_KVS / SRCBUFFER_SIZE;
	if(enable == OFF){ return; }

	unsigned int enable_generatestats = ON;
	#ifdef _DEBUGMODE_KERNELPRINTS2
	if(config.enablecollectglobalstats == ON){ cout<<"collectstats:: generating stats..."<<endl; }
	#endif 
	
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

buffer_type 
	#ifdef SW 
	acts::
	#endif
preparekeyvalues(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t localcapsule[VECTOR_SIZE][NUM_PARTITIONS], step_type currentLOP, sweepparams_t sweepparams, travstate_t travstate, buffer_type size_kvs, buffer_type cutoffs[VECTOR_SIZE], globalparams_t globalparams){
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
		actsutilityobj->checkoutofbounds("preparekeyvalues.localcapsule[0][p0].value", localcapsule[0][p0].value % 4, 4, localcapsule[0][p0].value, localcapsule[0][p0].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues.loc0", loc0, SRCBUFFER_SIZE, localcapsule[0][p0].key, localcapsule[0][p0].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues.localcapsule[1][p1].value", localcapsule[1][p1].value % 4, 4, localcapsule[1][p1].value, localcapsule[1][p1].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues.loc1", loc1, SRCBUFFER_SIZE, localcapsule[1][p1].key, localcapsule[1][p1].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues.localcapsule[2][p2].value", localcapsule[2][p2].value % 4, 4, localcapsule[2][p2].value, localcapsule[2][p2].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues.loc2", loc2, SRCBUFFER_SIZE, localcapsule[2][p2].key, localcapsule[2][p2].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues.localcapsule[3][p3].value", localcapsule[3][p3].value % 4, 4, localcapsule[3][p3].value, localcapsule[3][p3].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues.loc3", loc3, SRCBUFFER_SIZE, localcapsule[3][p3].key, localcapsule[3][p3].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues.localcapsule[4][p4].value", localcapsule[4][p4].value % 4, 4, localcapsule[4][p4].value, localcapsule[4][p4].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues.loc4", loc4, SRCBUFFER_SIZE, localcapsule[4][p4].key, localcapsule[4][p4].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues.localcapsule[5][p5].value", localcapsule[5][p5].value % 4, 4, localcapsule[5][p5].value, localcapsule[5][p5].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues.loc5", loc5, SRCBUFFER_SIZE, localcapsule[5][p5].key, localcapsule[5][p5].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues.localcapsule[6][p6].value", localcapsule[6][p6].value % 4, 4, localcapsule[6][p6].value, localcapsule[6][p6].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues.loc6", loc6, SRCBUFFER_SIZE, localcapsule[6][p6].key, localcapsule[6][p6].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues.localcapsule[7][p7].value", localcapsule[7][p7].value % 4, 4, localcapsule[7][p7].value, localcapsule[7][p7].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues.loc7", loc7, SRCBUFFER_SIZE, localcapsule[7][p7].key, localcapsule[7][p7].value, NAp);
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
			actsutilityobj->checkfordivisibleby(ON, "preparekeyvalues.localcapsule[v][p].key", localcapsule[v][p].key, 4);
			// actsutilityobj->checkfordivisibleby(ON, "preparekeyvalues.localcapsule["+std::to_string(v)+"][p].key", localcapsule[v][p].key, 4);
		}
	}
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printkeyvalues("preparekeyvalues.localcapsule[0].2", (keyvalue_t *)localcapsule[0], NUM_PARTITIONS);
	#endif
	return SRCBUFFER_SIZE;
}

void 
	#ifdef SW 
	acts::
	#endif
partitionpipeline(
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
		globalparams_t globalparams,
		sweepparams_t sweepparams,
		travstate_t ptravstate,
		batch_type sourcebaseaddr_kvs,
		batch_type destbaseaddr_kvs){
	#ifndef MERGEPROCESSEDGESANDPARTITIONSTAGE
	#pragma HLS INLINE
	#endif
	analysis_type analysis_partitionloop = KVDATA_BATCHSIZE_KVS / (NUMPARTITIONUPDATESPIPELINES * WORKBUFFER_SIZE);
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
	
	MAINLOOP_PARTITION: for(batch_type offset_kvs=ptravstate.begin_kvs; offset_kvs<ptravstate.end_kvs + 3*SRCBUFFER_SIZE; offset_kvs+=WORKBUFFER_SIZE * NUMPARTITIONUPDATESPIPELINES){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_partitionloop avg=analysis_partitionloop
		#ifdef _DEBUGMODE_KERNELPRINTS
		actsutilityobj->print6("### dispatch::partition:: offset0_kvs", "size0_kvs", "begin_kvs", "end_kvs", "skip", "itercount", offset_kvs, pp0readsize_kvs, ptravstate.begin_kvs, ptravstate.end_kvs, WORKBUFFER_SIZE, itercount);
		actsutilityobj->print6("### dispatch::partition:: offset1_kvs", "size1_kvs", "begin_kvs", "end_kvs", "skip", "itercount", offset_kvs + WORKBUFFER_SIZE, pp1readsize_kvs, ptravstate.begin_kvs, ptravstate.end_kvs, WORKBUFFER_SIZE, itercount);
		#endif
		
		ptravstatepp0.i_kvs =  offset_kvs;
		#ifdef PUP1
		ptravstatepp1.i_kvs = offset_kvs + WORKBUFFER_SIZE;
		#endif
		#ifdef PUP1
		if(itercount >= 2){ pp0writeen = ON; } else { pp0writeen = OFF; }
		if(itercount >= 4){ pp1writeen = ON; } else { pp1writeen = OFF; }
		if(itercount >= 2){ pp1runpipelineen = ON; } else { pp1runpipelineen = OFF; }
		#else 
		if(itercount >= 2){ pp0writeen = ON; } else { pp0writeen = OFF; }
		#endif
		
		readkeyvalues(ON, kvdram, sourcebuffer, (sourcebaseaddr_kvs + offset_kvs), WORKBUFFER_SIZE, ptravstatepp0); 
		#ifdef PUP1
		runpipeline(pp1runpipelineen, buffer_setof1, templocalcapsule_so1, buffer_setof2, templocalcapsule_so2, buffer_setof4, templocalcapsule_so4, buffer_setof8, templocalcapsule_so8, sweepparams.currentLOP, sweepparams, WORKBUFFER_SIZE, pp1cutoffs, (itercount-2)+1, globalparams);
		#endif 
		
		preparekeyvalues(ON, sourcebuffer, buffer_setof1, templocalcapsule_so1, sweepparams.currentLOP, sweepparams, ptravstatepp0, WORKBUFFER_SIZE, pp0cutoffs, globalparams);
		#ifdef PUP1
		savekeyvalues(pp1writeen, kvdram, buffer_setof8, globalstatsbuffer, templocalcapsule_so8, destbaseaddr_kvs, globalparams); 
		#endif 
		
		runpipeline(ON, buffer_setof1, templocalcapsule_so1, buffer_setof2, templocalcapsule_so2, buffer_setof4, templocalcapsule_so4, buffer_setof8, templocalcapsule_so8, sweepparams.currentLOP, sweepparams, WORKBUFFER_SIZE, pp0cutoffs, itercount, globalparams);
		#ifdef PUP1
		readkeyvalues(ON, kvdram, sourcebuffer, (sourcebaseaddr_kvs + offset_kvs + WORKBUFFER_SIZE), WORKBUFFER_SIZE, ptravstatepp1);
		#endif
		
		savekeyvalues(pp0writeen, kvdram, buffer_setof8, globalstatsbuffer, templocalcapsule_so8, destbaseaddr_kvs, globalparams); 
		#ifdef PUP1
		preparekeyvalues(pp1partitionen, sourcebuffer, buffer_setof1, templocalcapsule_so1, sweepparams.currentLOP, sweepparams, ptravstatepp1, WORKBUFFER_SIZE, pp1cutoffs, globalparams);
		#endif
		
		itercount += NUMPARTITIONUPDATESPIPELINES;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printglobalvars();
	actsutilityobj->clearglobalvars();
	#endif
	return;
}

// functions (reduce)
void 
	#ifdef SW 
	acts::
	#endif 
replicatedata_syn(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer0[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], buffer_type sourceoffset, buffer_type size){
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
		}
	}
	return;
}

void 
	#ifdef SW 
	acts::
	#endif 
replicatedata_syn(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE] ,keyvalue_t destbuffer0[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE]){
	REPLICATEDATA2_LOOP: for(unsigned int i=0; i<PADDEDDESTBUFFER_SIZE; i++){
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
		}
	}
	return;
}

void 
	#ifdef SW 
	acts::
	#endif 
replicatedata_bfs_syn(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer10[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer20[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],  buffer_type sourceoffset, buffer_type size){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount = PADDEDDESTBUFFER_SIZE;
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("replicatedata_bfs_syn.0", sourceoffset + size, (PADDEDDESTBUFFER_SIZE * VECTOR_SIZE) + 1, sourceoffset, size, PADDEDDESTBUFFER_SIZE);
	#endif

	vector_type src_v = 0;
	buffer_type src_i = 0;
	buffer_type sourceoffset_kvs = sourceoffset / VECTOR_SIZE;
	REPLICATEDATA_LOOP: for(buffer_type i=0; i<size; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("replicatedata_bfs_syn.1", i, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, sourceoffset, sourceoffset_kvs, size);
		actsutilityobj->checkoutofbounds("replicatedata_bfs_syn.2", sourceoffset + src_i, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, sourceoffset, sourceoffset_kvs, size);
		#endif
		
		keyvalue_t keyvalue = sourcebuffer[src_v][sourceoffset_kvs + src_i];
		src_v++; if(src_v==VECTOR_SIZE){ src_v=0; src_i+=1; }
		
		for(vector_type v=0; v<VECTOR_SIZE; v++){
		#pragma HLS UNROLL
			destbuffer10[v][i] = keyvalue; 
			
			destbuffer20[v][i] = keyvalue; 
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
reduce_bfs(bool_type enable1, bool_type enable2, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t tempdestbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], sweepparams_t sweepparams, unsigned int GraphIter, unsigned int GraphAlgo, travstate_t travstate, globalparams_t globalparams){
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

		unsigned int partitionupperlimit0 = streetaddr0 * (1 << globalparams.applyvertexbuffersz_pow);
		unsigned int partitionupperlimit1 = streetaddr1 * (1 << globalparams.applyvertexbuffersz_pow);
		unsigned int partitionupperlimit2 = streetaddr2 * (1 << globalparams.applyvertexbuffersz_pow);
		unsigned int partitionupperlimit3 = streetaddr3 * (1 << globalparams.applyvertexbuffersz_pow);
		unsigned int partitionupperlimit4 = streetaddr4 * (1 << globalparams.applyvertexbuffersz_pow);
		unsigned int partitionupperlimit5 = streetaddr5 * (1 << globalparams.applyvertexbuffersz_pow);
		unsigned int partitionupperlimit6 = streetaddr6 * (1 << globalparams.applyvertexbuffersz_pow);
		unsigned int partitionupperlimit7 = streetaddr7 * (1 << globalparams.applyvertexbuffersz_pow);
		
		REDUCE_LOOP2: for(buffer_type t=0; t<COMPACTPARAM_ITEMSIZE_TOTALDATA; t++){
		#pragma HLS PIPELINE II=2
			#ifdef _WIDEWORD
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
			} else if(t==2){ // AUTOMATEME.
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
			
			#ifdef _DEBUGMODE_STATS
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
			
			if(en0 == ON && en20 == ON){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("reduce_bfs 444a", colindexs0, 64, colindexs0, 0, NAp);
				#endif
				temps0 = READFROM_ULONG(tempdestbuffer[0][rowindexs0], colindexs0, 2);
				if(temps0 == UNVISITED){ WRITETO_ULONG(&destbuffer[0][rowindexs0], colindexs0, 2, VISITED_IN_CURRENT_ITERATION); }
			}
			if(en1 == ON && en21 == ON){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("reduce_bfs 444a", colindexs1, 64, colindexs1, 1, NAp);
				#endif
				temps1 = READFROM_ULONG(tempdestbuffer[1][rowindexs1], colindexs1, 2);
				if(temps1 == UNVISITED){ WRITETO_ULONG(&destbuffer[1][rowindexs1], colindexs1, 2, VISITED_IN_CURRENT_ITERATION); }
			}
			if(en2 == ON && en22 == ON){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("reduce_bfs 444a", colindexs2, 64, colindexs2, 2, NAp);
				#endif
				temps2 = READFROM_ULONG(tempdestbuffer[2][rowindexs2], colindexs2, 2);
				if(temps2 == UNVISITED){ WRITETO_ULONG(&destbuffer[2][rowindexs2], colindexs2, 2, VISITED_IN_CURRENT_ITERATION); }
			}
			if(en3 == ON && en23 == ON){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("reduce_bfs 444a", colindexs3, 64, colindexs3, 3, NAp);
				#endif
				temps3 = READFROM_ULONG(tempdestbuffer[3][rowindexs3], colindexs3, 2);
				if(temps3 == UNVISITED){ WRITETO_ULONG(&destbuffer[3][rowindexs3], colindexs3, 2, VISITED_IN_CURRENT_ITERATION); }
			}
			if(en4 == ON && en24 == ON){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("reduce_bfs 444a", colindexs4, 64, colindexs4, 4, NAp);
				#endif
				temps4 = READFROM_ULONG(tempdestbuffer[4][rowindexs4], colindexs4, 2);
				if(temps4 == UNVISITED){ WRITETO_ULONG(&destbuffer[4][rowindexs4], colindexs4, 2, VISITED_IN_CURRENT_ITERATION); }
			}
			if(en5 == ON && en25 == ON){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("reduce_bfs 444a", colindexs5, 64, colindexs5, 5, NAp);
				#endif
				temps5 = READFROM_ULONG(tempdestbuffer[5][rowindexs5], colindexs5, 2);
				if(temps5 == UNVISITED){ WRITETO_ULONG(&destbuffer[5][rowindexs5], colindexs5, 2, VISITED_IN_CURRENT_ITERATION); }
			}
			if(en6 == ON && en26 == ON){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("reduce_bfs 444a", colindexs6, 64, colindexs6, 6, NAp);
				#endif
				temps6 = READFROM_ULONG(tempdestbuffer[6][rowindexs6], colindexs6, 2);
				if(temps6 == UNVISITED){ WRITETO_ULONG(&destbuffer[6][rowindexs6], colindexs6, 2, VISITED_IN_CURRENT_ITERATION); }
			}
			if(en7 == ON && en27 == ON){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("reduce_bfs 444a", colindexs7, 64, colindexs7, 7, NAp);
				#endif
				temps7 = READFROM_ULONG(tempdestbuffer[7][rowindexs7], colindexs7, 2);
				if(temps7 == UNVISITED){ WRITETO_ULONG(&destbuffer[7][rowindexs7], colindexs7, 2, VISITED_IN_CURRENT_ITERATION); }
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

void 
	#ifdef SW 
	acts::
	#endif
reducevector(keyvalue_t kvdata, keyvalue_t destbuffer[PADDEDDESTBUFFER_SIZE], unsigned int upperlimit, sweepparams_t sweepparams, globalparams_t globalparams){
	#pragma HLS dependence variable=destbuffer inter false // CRITICAL FIXME. ENSURE THIS IS ALWAYS TRUE.
	analysis_type analysis_loop1 = VECTOR_SIZE;
		
	keyvalue_t keyvalue = kvdata;
	vertex_t loc = keyvalue.key - upperlimit;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(keyvalue.key != INVALIDDATA && keyvalue.value != INVALIDDATA){ cout<<"REDUCE SEEN @ reduce:: v: 0, loc: "<<loc<<", keyvalue.key: "<<keyvalue.key<<", keyvalue.value: "<<keyvalue.value<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<globalparams.applyvertexbuffersz<<endl; }
	#endif 
	
	if(loc >= globalparams.applyvertexbuffersz && keyvalue.key != INVALIDDATA && keyvalue.value != INVALIDDATA){ 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->reducehelper_checkreduceloc(0, loc, keyvalue, sweepparams, globalparams); 
		#endif 
		loc = 0; }
	// cout<<"--- loc: "<<loc<<", keyvalue.key: "<<keyvalue.key<<", keyvalue.value: "<<keyvalue.value<<endl;
	
	vertex_t rowindex = loc / 2;
	vertex_t colindex = loc % 2;
	
	keyvalue_t vprop;
	if(keyvalue.key != INVALIDDATA && keyvalue.value != INVALIDDATA){ vprop = destbuffer[rowindex]; }
	
	value_t temp;
	if(colindex == 0){ temp = vprop.key; }
	else { temp = vprop.value; }
	
	value_t rettemp = reducefunc(temp, keyvalue.value, globalparams.GraphIter, globalparams.GraphAlgo);
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(keyvalue.key != INVALIDDATA && keyvalue.value != INVALIDDATA){ cout<<"REDUCEFUNC RESULT @ reduce:: rettemp: "<<rettemp<<endl; }
	#endif 
	
	if(colindex == 0){ vprop.key = rettemp; }
	else { vprop.value = rettemp; }
	
	if(keyvalue.key != INVALIDDATA && keyvalue.value != INVALIDDATA){ destbuffer[rowindex] = vprop; }
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->globalstats_countkvsreduced(1);
	if(keyvalue.key != INVALIDDATA && keyvalue.value != INVALIDDATA){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(1); }
	#endif
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
unifydata_pr_parallelsyn(bool_type enable, keyvalue_t sourcebuffer0[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
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
			
			if(k==0){
				temp00_val = vdata00.key;
				temp01_val = vdata01.key;
				temp02_val = vdata02.key;
				temp03_val = vdata03.key;
				temp04_val = vdata04.key;
				temp05_val = vdata05.key;
				temp06_val = vdata06.key;
				temp07_val = vdata07.key;
			} else {
				temp00_val = vdata00.value;
				temp01_val = vdata01.value;
				temp02_val = vdata02.value;
				temp03_val = vdata03.value;
				temp04_val = vdata04.value;
				temp05_val = vdata05.value;
				temp06_val = vdata06.value;
				temp07_val = vdata07.value;
			}
			
			value_t a0 = aplus(temp00_val, temp01_val);
			value_t b0 = aplus(temp02_val, temp03_val);
			value_t c0 = aplus(temp04_val, temp05_val);
			value_t d0 = aplus(temp06_val, temp07_val);
			
			value_t m0 = aplus(a0, b0);
			value_t n0 = aplus(c0, d0);
			
			value_t v0 = aplus(m0, n0);
			
			#if NUMCOMPUTEUNITS==1 // AUTOMATEME.
			value_t z = v0;
			#endif 
			
			#if NUMCOMPUTEUNITS==2
			value_t z = aplus(v0, v1);
			#endif 
			
			#if NUMCOMPUTEUNITS==3
			value_t w0 = aplus(v0, v1);
			value_t w1 = aplus(v2, 0xFFFFFFFF);
			
			value_t z = aplus(w0, w1);
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
unifydata_bfs_parallelsyn(bool_type enable, keyvalue_t sourcebuffer0[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
					buffer_type destoffset, buffer_type size, globalparams_t globalparams){
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
		
		data0.key =  sourcebuffer0[0][i].key | sourcebuffer0[1][i].key | sourcebuffer0[2][i].key | sourcebuffer0[3][i].key | sourcebuffer0[4][i].key | sourcebuffer0[5][i].key | sourcebuffer0[6][i].key | sourcebuffer0[7][i].key  ;
		
		data0.value =  sourcebuffer0[0][i].value | sourcebuffer0[1][i].value | sourcebuffer0[2][i].value | sourcebuffer0[3][i].value | sourcebuffer0[4][i].value | sourcebuffer0[5][i].value | sourcebuffer0[6][i].value | sourcebuffer0[7][i].value  ;
		
		data.key =  data0.key  ;
		data.value =  data0.value  ;
		
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
unifydata_sssp_parallelsyn(bool_type enable, keyvalue_t sourcebuffer0[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
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
			
			keyvalue_t vdata00 = sourcebuffer0[0][i];
			keyvalue_t vdata01 = sourcebuffer0[1][i];
			keyvalue_t vdata02 = sourcebuffer0[2][i];
			keyvalue_t vdata03 = sourcebuffer0[3][i];
			keyvalue_t vdata04 = sourcebuffer0[4][i];
			keyvalue_t vdata05 = sourcebuffer0[5][i];
			keyvalue_t vdata06 = sourcebuffer0[6][i];
			keyvalue_t vdata07 = sourcebuffer0[7][i];
			
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
			}
				
			code0 =  temp00_code | temp01_code | temp02_code | temp03_code | temp04_code | temp05_code | temp06_code | temp07_code  ;
			code =  code0  ;
			
			value_t a0 = amin(temp00_val, temp01_val);
			value_t b0 = amin(temp02_val, temp03_val);
			value_t c0 = amin(temp04_val, temp05_val);
			value_t d0 = amin(temp06_val, temp07_val);
			
			value_t m0 = amin(a0, b0);
			value_t n0 = amin(c0, d0);
			
			value_t v0 = amin(m0, n0);
			
			// value 
			#if NUMCOMPUTEUNITS==1 // AUTOMATEME.
			value_t z = v0;
			#endif 
			
			#if NUMCOMPUTEUNITS==2
			value_t z = amin(v0, v1);
			#endif
			
			#if NUMCOMPUTEUNITS==3
			value_t w0 = amin(v0, v1);
			value_t w1 = amin(v2, 0xFFFFFFFF);
			
			value_t z = amin(w0, w1);
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

bool_type 
	#ifdef SW 
	acts::
	#endif
collectactvvs_bfs(uint512_dt * vdram, bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
					bool_type itercount_actvvs0,
					bool_type itercount_actvvs1,
					bool_type enableflush, unsigned int index, travstate_t * actvvstravstate, 
					buffer_type destoffset, buffer_type size, sweepparams_t sweepparams, globalparams_t globalparams){
	analysis_type analysis_loopcount = (APPLYVERTEXBUFFERSZ / VDATAPACKINGFACTOR) / 2;
	
	static keyvalue_t actvvs0[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = actvvs0 

	static keyvalue_t buffer0_setof2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer0_setof2
	static keyvalue_t buffer0_setof4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer0_setof4
	static keyvalue_t buffer0_setof8[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer0_setof8

	static skeyvalue_t capsule0_so1[8];
	#pragma HLS ARRAY_PARTITION variable=capsule0_so1 complete
	static skeyvalue_t capsule0_so2[4];
	#pragma HLS ARRAY_PARTITION variable=capsule0_so2 complete
	static skeyvalue_t capsule0_so4[2];
	#pragma HLS ARRAY_PARTITION variable=capsule0_so4 complete
	skeyvalue_t capsule0_so8;
	
	static buffer_type cutoffs0[VECTOR_SIZE];
	static keyvalue_t actvvs1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = actvvs1 

	static keyvalue_t buffer1_setof2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer1_setof2
	static keyvalue_t buffer1_setof4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer1_setof4
	static keyvalue_t buffer1_setof8[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer1_setof8

	static skeyvalue_t capsule1_so1[8];
	#pragma HLS ARRAY_PARTITION variable=capsule1_so1 complete
	static skeyvalue_t capsule1_so2[4];
	#pragma HLS ARRAY_PARTITION variable=capsule1_so2 complete
	static skeyvalue_t capsule1_so4[2];
	#pragma HLS ARRAY_PARTITION variable=capsule1_so4 complete
	skeyvalue_t capsule1_so8;
	
	static buffer_type cutoffs1[VECTOR_SIZE];
	
	bool_type writeen_actvvs0 = ON;
	bool_type writeen_actvvs1 = ON;
	
	buffer_type dest_v = 0;
	buffer_type dest_i = 0;
	buffer_type destoffset_kvs = destoffset / VECTOR_SIZE;
	unsigned int nonzeroactvvsreturned = OFF;
	
	for(vector_type v=0; v<VECTOR_SIZE; v++){ 
	#pragma HLS UNROLL
		capsule0_so1[v].key = 0; capsule0_so1[v].value = 0; 
		capsule1_so1[v].key = 0; capsule1_so1[v].value = 0; 
	}
	unsigned int voffset = sweepparams.source_partition * globalparams.applyvertexbuffersz;
	
	if(enable == ON){
		COLLECTACTVVS_LOOP1: for(buffer_type i=0; i<size; i++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
		#pragma HLS PIPELINE II=2
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("collectactvvs.i", i, PADDEDDESTBUFFER_SIZE, destoffset, destoffset_kvs, size);
			#endif
			
			keyvalue_t data = sourcebuffer[dest_v][destoffset_kvs + dest_i];
			#ifdef _WIDEWORD
			ulong_dt longdata = CONVERTTOLONG_KV(data);
			#endif 
			unsigned int vioffset = voffset + i*32;
			
			if(data.key != 0 || data.value != 0){
				nonzeroactvvsreturned = ON;
					
				unsigned int vid00 = vioffset + 0;
				unsigned int vid01 = vioffset + 1;
				unsigned int vid02 = vioffset + 2;
				unsigned int vid03 = vioffset + 3;
				unsigned int vid10 = vioffset + 4;
				unsigned int vid11 = vioffset + 5;
				unsigned int vid12 = vioffset + 6;
				unsigned int vid13 = vioffset + 7;
				unsigned int vid20 = vioffset + 8;
				unsigned int vid21 = vioffset + 9;
				unsigned int vid22 = vioffset + 10;
				unsigned int vid23 = vioffset + 11;
				unsigned int vid30 = vioffset + 12;
				unsigned int vid31 = vioffset + 13;
				unsigned int vid32 = vioffset + 14;
				unsigned int vid33 = vioffset + 15;
				unsigned int vid40 = vioffset + 16;
				unsigned int vid41 = vioffset + 17;
				unsigned int vid42 = vioffset + 18;
				unsigned int vid43 = vioffset + 19;
				unsigned int vid50 = vioffset + 20;
				unsigned int vid51 = vioffset + 21;
				unsigned int vid52 = vioffset + 22;
				unsigned int vid53 = vioffset + 23;
				unsigned int vid60 = vioffset + 24;
				unsigned int vid61 = vioffset + 25;
				unsigned int vid62 = vioffset + 26;
				unsigned int vid63 = vioffset + 27;
				unsigned int vid70 = vioffset + 28;
				unsigned int vid71 = vioffset + 29;
				unsigned int vid72 = vioffset + 30;
				unsigned int vid73 = vioffset + 31;
				
				#ifdef _WIDEWORD
				visitstate_type vdata00 = longdata.range(1, 0); 
				visitstate_type vdata01 = longdata.range(3, 2); 
				visitstate_type vdata02 = longdata.range(5, 4); 
				visitstate_type vdata03 = longdata.range(7, 6); 
				visitstate_type vdata10 = longdata.range(9, 8); 
				visitstate_type vdata11 = longdata.range(11, 10); 
				visitstate_type vdata12 = longdata.range(13, 12); 
				visitstate_type vdata13 = longdata.range(15, 14); 
				visitstate_type vdata20 = longdata.range(17, 16); 
				visitstate_type vdata21 = longdata.range(19, 18); 
				visitstate_type vdata22 = longdata.range(21, 20); 
				visitstate_type vdata23 = longdata.range(23, 22); 
				visitstate_type vdata30 = longdata.range(25, 24); 
				visitstate_type vdata31 = longdata.range(27, 26); 
				visitstate_type vdata32 = longdata.range(29, 28); 
				visitstate_type vdata33 = longdata.range(31, 30); 
				visitstate_type vdata40 = longdata.range(33, 32); 
				visitstate_type vdata41 = longdata.range(35, 34); 
				visitstate_type vdata42 = longdata.range(37, 36); 
				visitstate_type vdata43 = longdata.range(39, 38); 
				visitstate_type vdata50 = longdata.range(41, 40); 
				visitstate_type vdata51 = longdata.range(43, 42); 
				visitstate_type vdata52 = longdata.range(45, 44); 
				visitstate_type vdata53 = longdata.range(47, 46); 
				visitstate_type vdata60 = longdata.range(49, 48); 
				visitstate_type vdata61 = longdata.range(51, 50); 
				visitstate_type vdata62 = longdata.range(53, 52); 
				visitstate_type vdata63 = longdata.range(55, 54); 
				visitstate_type vdata70 = longdata.range(57, 56); 
				visitstate_type vdata71 = longdata.range(59, 58); 
				visitstate_type vdata72 = longdata.range(61, 60); 
				visitstate_type vdata73 = longdata.range(63, 62); 
				#else
				visitstate_type vdata00 = READFROM_ULONG(data, 0, 2); 
				visitstate_type vdata01 = READFROM_ULONG(data, 2, 2); 
				visitstate_type vdata02 = READFROM_ULONG(data, 4, 2); 
				visitstate_type vdata03 = READFROM_ULONG(data, 6, 2);
				visitstate_type vdata10 = READFROM_ULONG(data, 8, 2); 
				visitstate_type vdata11 = READFROM_ULONG(data, 10, 2); 
				visitstate_type vdata12 = READFROM_ULONG(data, 12, 2); 
				visitstate_type vdata13 = READFROM_ULONG(data, 14, 2);
				visitstate_type vdata20 = READFROM_ULONG(data, 16, 2); 
				visitstate_type vdata21 = READFROM_ULONG(data, 18, 2); 
				visitstate_type vdata22 = READFROM_ULONG(data, 20, 2); 
				visitstate_type vdata23 = READFROM_ULONG(data, 22, 2);
				visitstate_type vdata30 = READFROM_ULONG(data, 24, 2); 
				visitstate_type vdata31 = READFROM_ULONG(data, 26, 2); 
				visitstate_type vdata32 = READFROM_ULONG(data, 28, 2); 
				visitstate_type vdata33 = READFROM_ULONG(data, 30, 2);
				visitstate_type vdata40 = READFROM_ULONG(data, 32, 2); 
				visitstate_type vdata41 = READFROM_ULONG(data, 34, 2); 
				visitstate_type vdata42 = READFROM_ULONG(data, 36, 2); 
				visitstate_type vdata43 = READFROM_ULONG(data, 38, 2);
				visitstate_type vdata50 = READFROM_ULONG(data, 40, 2); 
				visitstate_type vdata51 = READFROM_ULONG(data, 42, 2); 
				visitstate_type vdata52 = READFROM_ULONG(data, 44, 2); 
				visitstate_type vdata53 = READFROM_ULONG(data, 46, 2);
				visitstate_type vdata60 = READFROM_ULONG(data, 48, 2); 
				visitstate_type vdata61 = READFROM_ULONG(data, 50, 2); 
				visitstate_type vdata62 = READFROM_ULONG(data, 52, 2); 
				visitstate_type vdata63 = READFROM_ULONG(data, 54, 2);
				visitstate_type vdata70 = READFROM_ULONG(data, 56, 2); 
				visitstate_type vdata71 = READFROM_ULONG(data, 58, 2); 
				visitstate_type vdata72 = READFROM_ULONG(data, 60, 2); 
				visitstate_type vdata73 = READFROM_ULONG(data, 62, 2);
				#endif
				
				visitstate_type tmpvdata00 = vdata00; 
				visitstate_type tmpvdata01 = vdata01; 
				visitstate_type tmpvdata02 = vdata02; 
				visitstate_type tmpvdata03 = vdata03; 
				visitstate_type tmpvdata10 = vdata10; 
				visitstate_type tmpvdata11 = vdata11; 
				visitstate_type tmpvdata12 = vdata12; 
				visitstate_type tmpvdata13 = vdata13; 
				visitstate_type tmpvdata20 = vdata20; 
				visitstate_type tmpvdata21 = vdata21; 
				visitstate_type tmpvdata22 = vdata22; 
				visitstate_type tmpvdata23 = vdata23; 
				visitstate_type tmpvdata30 = vdata30; 
				visitstate_type tmpvdata31 = vdata31; 
				visitstate_type tmpvdata32 = vdata32; 
				visitstate_type tmpvdata33 = vdata33; 
				visitstate_type tmpvdata40 = vdata40; 
				visitstate_type tmpvdata41 = vdata41; 
				visitstate_type tmpvdata42 = vdata42; 
				visitstate_type tmpvdata43 = vdata43; 
				visitstate_type tmpvdata50 = vdata50; 
				visitstate_type tmpvdata51 = vdata51; 
				visitstate_type tmpvdata52 = vdata52; 
				visitstate_type tmpvdata53 = vdata53; 
				visitstate_type tmpvdata60 = vdata60; 
				visitstate_type tmpvdata61 = vdata61; 
				visitstate_type tmpvdata62 = vdata62; 
				visitstate_type tmpvdata63 = vdata63; 
				visitstate_type tmpvdata70 = vdata70; 
				visitstate_type tmpvdata71 = vdata71; 
				visitstate_type tmpvdata72 = vdata72; 
				visitstate_type tmpvdata73 = vdata73; 
		
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"collectactvvs_bfs: ACTIVE VERTICES seen for next iteration. vid00: "<<vid00<<", vdata00: "<<vdata00<<endl;
				cout<<"collectactvvs_bfs: ACTIVE VERTICES seen for next iteration. vid01: "<<vid01<<", vdata01: "<<vdata01<<endl;
				cout<<"collectactvvs_bfs: ACTIVE VERTICES seen for next iteration. vid02: "<<vid02<<", vdata02: "<<vdata02<<endl;
				cout<<"collectactvvs_bfs: ACTIVE VERTICES seen for next iteration. vid03: "<<vid03<<", vdata03: "<<vdata03<<endl;
				cout<<"collectactvvs_bfs: ACTIVE VERTICES seen for next iteration. vid10: "<<vid10<<", vdata10: "<<vdata10<<endl;
				cout<<"collectactvvs_bfs: ACTIVE VERTICES seen for next iteration. vid11: "<<vid11<<", vdata11: "<<vdata11<<endl;
				cout<<"collectactvvs_bfs: ACTIVE VERTICES seen for next iteration. vid12: "<<vid12<<", vdata12: "<<vdata12<<endl;
				cout<<"collectactvvs_bfs: ACTIVE VERTICES seen for next iteration. vid13: "<<vid13<<", vdata13: "<<vdata13<<endl;
				cout<<"collectactvvs_bfs: ACTIVE VERTICES seen for next iteration. vid20: "<<vid20<<", vdata20: "<<vdata20<<endl;
				cout<<"collectactvvs_bfs: ACTIVE VERTICES seen for next iteration. vid21: "<<vid21<<", vdata21: "<<vdata21<<endl;
				cout<<"collectactvvs_bfs: ACTIVE VERTICES seen for next iteration. vid22: "<<vid22<<", vdata22: "<<vdata22<<endl;
				cout<<"collectactvvs_bfs: ACTIVE VERTICES seen for next iteration. vid23: "<<vid23<<", vdata23: "<<vdata23<<endl;
				cout<<"collectactvvs_bfs: ACTIVE VERTICES seen for next iteration. vid30: "<<vid30<<", vdata30: "<<vdata30<<endl;
				cout<<"collectactvvs_bfs: ACTIVE VERTICES seen for next iteration. vid31: "<<vid31<<", vdata31: "<<vdata31<<endl;
				cout<<"collectactvvs_bfs: ACTIVE VERTICES seen for next iteration. vid32: "<<vid32<<", vdata32: "<<vdata32<<endl;
				cout<<"collectactvvs_bfs: ACTIVE VERTICES seen for next iteration. vid33: "<<vid33<<", vdata33: "<<vdata33<<endl;
				cout<<"collectactvvs_bfs: ACTIVE VERTICES seen for next iteration. vid40: "<<vid40<<", vdata40: "<<vdata40<<endl;
				cout<<"collectactvvs_bfs: ACTIVE VERTICES seen for next iteration. vid41: "<<vid41<<", vdata41: "<<vdata41<<endl;
				cout<<"collectactvvs_bfs: ACTIVE VERTICES seen for next iteration. vid42: "<<vid42<<", vdata42: "<<vdata42<<endl;
				cout<<"collectactvvs_bfs: ACTIVE VERTICES seen for next iteration. vid43: "<<vid43<<", vdata43: "<<vdata43<<endl;
				cout<<"collectactvvs_bfs: ACTIVE VERTICES seen for next iteration. vid50: "<<vid50<<", vdata50: "<<vdata50<<endl;
				cout<<"collectactvvs_bfs: ACTIVE VERTICES seen for next iteration. vid51: "<<vid51<<", vdata51: "<<vdata51<<endl;
				cout<<"collectactvvs_bfs: ACTIVE VERTICES seen for next iteration. vid52: "<<vid52<<", vdata52: "<<vdata52<<endl;
				cout<<"collectactvvs_bfs: ACTIVE VERTICES seen for next iteration. vid53: "<<vid53<<", vdata53: "<<vdata53<<endl;
				cout<<"collectactvvs_bfs: ACTIVE VERTICES seen for next iteration. vid60: "<<vid60<<", vdata60: "<<vdata60<<endl;
				cout<<"collectactvvs_bfs: ACTIVE VERTICES seen for next iteration. vid61: "<<vid61<<", vdata61: "<<vdata61<<endl;
				cout<<"collectactvvs_bfs: ACTIVE VERTICES seen for next iteration. vid62: "<<vid62<<", vdata62: "<<vdata62<<endl;
				cout<<"collectactvvs_bfs: ACTIVE VERTICES seen for next iteration. vid63: "<<vid63<<", vdata63: "<<vdata63<<endl;
				cout<<"collectactvvs_bfs: ACTIVE VERTICES seen for next iteration. vid70: "<<vid70<<", vdata70: "<<vdata70<<endl;
				cout<<"collectactvvs_bfs: ACTIVE VERTICES seen for next iteration. vid71: "<<vid71<<", vdata71: "<<vdata71<<endl;
				cout<<"collectactvvs_bfs: ACTIVE VERTICES seen for next iteration. vid72: "<<vid72<<", vdata72: "<<vdata72<<endl;
				cout<<"collectactvvs_bfs: ACTIVE VERTICES seen for next iteration. vid73: "<<vid73<<", vdata73: "<<vdata73<<endl;
				#endif
				
				if(vdata00 != VISITED_IN_CURRENT_ITERATION){ vid00 = UNUSEDDATA; }
				if(vdata01 != VISITED_IN_CURRENT_ITERATION){ vid01 = UNUSEDDATA; }
				if(vdata02 != VISITED_IN_CURRENT_ITERATION){ vid02 = UNUSEDDATA; }
				if(vdata03 != VISITED_IN_CURRENT_ITERATION){ vid03 = UNUSEDDATA; }
				if(vdata10 != VISITED_IN_CURRENT_ITERATION){ vid10 = UNUSEDDATA; }
				if(vdata11 != VISITED_IN_CURRENT_ITERATION){ vid11 = UNUSEDDATA; }
				if(vdata12 != VISITED_IN_CURRENT_ITERATION){ vid12 = UNUSEDDATA; }
				if(vdata13 != VISITED_IN_CURRENT_ITERATION){ vid13 = UNUSEDDATA; }
				if(vdata20 != VISITED_IN_CURRENT_ITERATION){ vid20 = UNUSEDDATA; }
				if(vdata21 != VISITED_IN_CURRENT_ITERATION){ vid21 = UNUSEDDATA; }
				if(vdata22 != VISITED_IN_CURRENT_ITERATION){ vid22 = UNUSEDDATA; }
				if(vdata23 != VISITED_IN_CURRENT_ITERATION){ vid23 = UNUSEDDATA; }
				if(vdata30 != VISITED_IN_CURRENT_ITERATION){ vid30 = UNUSEDDATA; }
				if(vdata31 != VISITED_IN_CURRENT_ITERATION){ vid31 = UNUSEDDATA; }
				if(vdata32 != VISITED_IN_CURRENT_ITERATION){ vid32 = UNUSEDDATA; }
				if(vdata33 != VISITED_IN_CURRENT_ITERATION){ vid33 = UNUSEDDATA; }
				if(vdata40 != VISITED_IN_CURRENT_ITERATION){ vid40 = UNUSEDDATA; }
				if(vdata41 != VISITED_IN_CURRENT_ITERATION){ vid41 = UNUSEDDATA; }
				if(vdata42 != VISITED_IN_CURRENT_ITERATION){ vid42 = UNUSEDDATA; }
				if(vdata43 != VISITED_IN_CURRENT_ITERATION){ vid43 = UNUSEDDATA; }
				if(vdata50 != VISITED_IN_CURRENT_ITERATION){ vid50 = UNUSEDDATA; }
				if(vdata51 != VISITED_IN_CURRENT_ITERATION){ vid51 = UNUSEDDATA; }
				if(vdata52 != VISITED_IN_CURRENT_ITERATION){ vid52 = UNUSEDDATA; }
				if(vdata53 != VISITED_IN_CURRENT_ITERATION){ vid53 = UNUSEDDATA; }
				if(vdata60 != VISITED_IN_CURRENT_ITERATION){ vid60 = UNUSEDDATA; }
				if(vdata61 != VISITED_IN_CURRENT_ITERATION){ vid61 = UNUSEDDATA; }
				if(vdata62 != VISITED_IN_CURRENT_ITERATION){ vid62 = UNUSEDDATA; }
				if(vdata63 != VISITED_IN_CURRENT_ITERATION){ vid63 = UNUSEDDATA; }
				if(vdata70 != VISITED_IN_CURRENT_ITERATION){ vid70 = UNUSEDDATA; }
				if(vdata71 != VISITED_IN_CURRENT_ITERATION){ vid71 = UNUSEDDATA; }
				if(vdata72 != VISITED_IN_CURRENT_ITERATION){ vid72 = UNUSEDDATA; }
				if(vdata73 != VISITED_IN_CURRENT_ITERATION){ vid73 = UNUSEDDATA; }
				
				if(vdata00 == VISITED_IN_CURRENT_ITERATION){ tmpvdata00 = VISITED_IN_PAST_ITERATION; }
				if(vdata01 == VISITED_IN_CURRENT_ITERATION){ tmpvdata01 = VISITED_IN_PAST_ITERATION; }
				if(vdata02 == VISITED_IN_CURRENT_ITERATION){ tmpvdata02 = VISITED_IN_PAST_ITERATION; }
				if(vdata03 == VISITED_IN_CURRENT_ITERATION){ tmpvdata03 = VISITED_IN_PAST_ITERATION; }
				if(vdata10 == VISITED_IN_CURRENT_ITERATION){ tmpvdata10 = VISITED_IN_PAST_ITERATION; }
				if(vdata11 == VISITED_IN_CURRENT_ITERATION){ tmpvdata11 = VISITED_IN_PAST_ITERATION; }
				if(vdata12 == VISITED_IN_CURRENT_ITERATION){ tmpvdata12 = VISITED_IN_PAST_ITERATION; }
				if(vdata13 == VISITED_IN_CURRENT_ITERATION){ tmpvdata13 = VISITED_IN_PAST_ITERATION; }
				if(vdata20 == VISITED_IN_CURRENT_ITERATION){ tmpvdata20 = VISITED_IN_PAST_ITERATION; }
				if(vdata21 == VISITED_IN_CURRENT_ITERATION){ tmpvdata21 = VISITED_IN_PAST_ITERATION; }
				if(vdata22 == VISITED_IN_CURRENT_ITERATION){ tmpvdata22 = VISITED_IN_PAST_ITERATION; }
				if(vdata23 == VISITED_IN_CURRENT_ITERATION){ tmpvdata23 = VISITED_IN_PAST_ITERATION; }
				if(vdata30 == VISITED_IN_CURRENT_ITERATION){ tmpvdata30 = VISITED_IN_PAST_ITERATION; }
				if(vdata31 == VISITED_IN_CURRENT_ITERATION){ tmpvdata31 = VISITED_IN_PAST_ITERATION; }
				if(vdata32 == VISITED_IN_CURRENT_ITERATION){ tmpvdata32 = VISITED_IN_PAST_ITERATION; }
				if(vdata33 == VISITED_IN_CURRENT_ITERATION){ tmpvdata33 = VISITED_IN_PAST_ITERATION; }
				if(vdata40 == VISITED_IN_CURRENT_ITERATION){ tmpvdata40 = VISITED_IN_PAST_ITERATION; }
				if(vdata41 == VISITED_IN_CURRENT_ITERATION){ tmpvdata41 = VISITED_IN_PAST_ITERATION; }
				if(vdata42 == VISITED_IN_CURRENT_ITERATION){ tmpvdata42 = VISITED_IN_PAST_ITERATION; }
				if(vdata43 == VISITED_IN_CURRENT_ITERATION){ tmpvdata43 = VISITED_IN_PAST_ITERATION; }
				if(vdata50 == VISITED_IN_CURRENT_ITERATION){ tmpvdata50 = VISITED_IN_PAST_ITERATION; }
				if(vdata51 == VISITED_IN_CURRENT_ITERATION){ tmpvdata51 = VISITED_IN_PAST_ITERATION; }
				if(vdata52 == VISITED_IN_CURRENT_ITERATION){ tmpvdata52 = VISITED_IN_PAST_ITERATION; }
				if(vdata53 == VISITED_IN_CURRENT_ITERATION){ tmpvdata53 = VISITED_IN_PAST_ITERATION; }
				if(vdata60 == VISITED_IN_CURRENT_ITERATION){ tmpvdata60 = VISITED_IN_PAST_ITERATION; }
				if(vdata61 == VISITED_IN_CURRENT_ITERATION){ tmpvdata61 = VISITED_IN_PAST_ITERATION; }
				if(vdata62 == VISITED_IN_CURRENT_ITERATION){ tmpvdata62 = VISITED_IN_PAST_ITERATION; }
				if(vdata63 == VISITED_IN_CURRENT_ITERATION){ tmpvdata63 = VISITED_IN_PAST_ITERATION; }
				if(vdata70 == VISITED_IN_CURRENT_ITERATION){ tmpvdata70 = VISITED_IN_PAST_ITERATION; }
				if(vdata71 == VISITED_IN_CURRENT_ITERATION){ tmpvdata71 = VISITED_IN_PAST_ITERATION; }
				if(vdata72 == VISITED_IN_CURRENT_ITERATION){ tmpvdata72 = VISITED_IN_PAST_ITERATION; }
				if(vdata73 == VISITED_IN_CURRENT_ITERATION){ tmpvdata73 = VISITED_IN_PAST_ITERATION; }
				
				if(vdata00 == VISITED_IN_CURRENT_ITERATION || vdata01 == VISITED_IN_CURRENT_ITERATION){ 
					#ifdef _DEBUGMODE_CHECKS2
					actsutilityobj->checkoutofbounds("acts::collectactvvs_bfs 3", capsule0_so1[0].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
					#endif
					actvvs0[0][capsule0_so1[0].value].key = vid00; 
					actvvs0[0][capsule0_so1[0].value].value = vid01;
					capsule0_so1[0].value++;
				}
				if(vdata02 == VISITED_IN_CURRENT_ITERATION || vdata03 == VISITED_IN_CURRENT_ITERATION){ 
					#ifdef _DEBUGMODE_CHECKS2
					actsutilityobj->checkoutofbounds("acts::collectactvvs_bfs 4", capsule1_so1[0].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
					#endif
					actvvs1[0][capsule1_so1[0].value].key = vid02; 
					actvvs1[0][capsule1_so1[0].value].value = vid03;
					capsule1_so1[0].value++;
				}
				if(vdata10 == VISITED_IN_CURRENT_ITERATION || vdata11 == VISITED_IN_CURRENT_ITERATION){ 
					#ifdef _DEBUGMODE_CHECKS2
					actsutilityobj->checkoutofbounds("acts::collectactvvs_bfs 3", capsule0_so1[1].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
					#endif
					actvvs0[1][capsule0_so1[1].value].key = vid10; 
					actvvs0[1][capsule0_so1[1].value].value = vid11;
					capsule0_so1[1].value++;
				}
				if(vdata12 == VISITED_IN_CURRENT_ITERATION || vdata13 == VISITED_IN_CURRENT_ITERATION){ 
					#ifdef _DEBUGMODE_CHECKS2
					actsutilityobj->checkoutofbounds("acts::collectactvvs_bfs 4", capsule1_so1[1].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
					#endif
					actvvs1[1][capsule1_so1[1].value].key = vid12; 
					actvvs1[1][capsule1_so1[1].value].value = vid13;
					capsule1_so1[1].value++;
				}
				if(vdata20 == VISITED_IN_CURRENT_ITERATION || vdata21 == VISITED_IN_CURRENT_ITERATION){ 
					#ifdef _DEBUGMODE_CHECKS2
					actsutilityobj->checkoutofbounds("acts::collectactvvs_bfs 3", capsule0_so1[2].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
					#endif
					actvvs0[2][capsule0_so1[2].value].key = vid20; 
					actvvs0[2][capsule0_so1[2].value].value = vid21;
					capsule0_so1[2].value++;
				}
				if(vdata22 == VISITED_IN_CURRENT_ITERATION || vdata23 == VISITED_IN_CURRENT_ITERATION){ 
					#ifdef _DEBUGMODE_CHECKS2
					actsutilityobj->checkoutofbounds("acts::collectactvvs_bfs 4", capsule1_so1[2].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
					#endif
					actvvs1[2][capsule1_so1[2].value].key = vid22; 
					actvvs1[2][capsule1_so1[2].value].value = vid23;
					capsule1_so1[2].value++;
				}
				if(vdata30 == VISITED_IN_CURRENT_ITERATION || vdata31 == VISITED_IN_CURRENT_ITERATION){ 
					#ifdef _DEBUGMODE_CHECKS2
					actsutilityobj->checkoutofbounds("acts::collectactvvs_bfs 3", capsule0_so1[3].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
					#endif
					actvvs0[3][capsule0_so1[3].value].key = vid30; 
					actvvs0[3][capsule0_so1[3].value].value = vid31;
					capsule0_so1[3].value++;
				}
				if(vdata32 == VISITED_IN_CURRENT_ITERATION || vdata33 == VISITED_IN_CURRENT_ITERATION){ 
					#ifdef _DEBUGMODE_CHECKS2
					actsutilityobj->checkoutofbounds("acts::collectactvvs_bfs 4", capsule1_so1[3].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
					#endif
					actvvs1[3][capsule1_so1[3].value].key = vid32; 
					actvvs1[3][capsule1_so1[3].value].value = vid33;
					capsule1_so1[3].value++;
				}
				if(vdata40 == VISITED_IN_CURRENT_ITERATION || vdata41 == VISITED_IN_CURRENT_ITERATION){ 
					#ifdef _DEBUGMODE_CHECKS2
					actsutilityobj->checkoutofbounds("acts::collectactvvs_bfs 3", capsule0_so1[4].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
					#endif
					actvvs0[4][capsule0_so1[4].value].key = vid40; 
					actvvs0[4][capsule0_so1[4].value].value = vid41;
					capsule0_so1[4].value++;
				}
				if(vdata42 == VISITED_IN_CURRENT_ITERATION || vdata43 == VISITED_IN_CURRENT_ITERATION){ 
					#ifdef _DEBUGMODE_CHECKS2
					actsutilityobj->checkoutofbounds("acts::collectactvvs_bfs 4", capsule1_so1[4].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
					#endif
					actvvs1[4][capsule1_so1[4].value].key = vid42; 
					actvvs1[4][capsule1_so1[4].value].value = vid43;
					capsule1_so1[4].value++;
				}
				if(vdata50 == VISITED_IN_CURRENT_ITERATION || vdata51 == VISITED_IN_CURRENT_ITERATION){ 
					#ifdef _DEBUGMODE_CHECKS2
					actsutilityobj->checkoutofbounds("acts::collectactvvs_bfs 3", capsule0_so1[5].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
					#endif
					actvvs0[5][capsule0_so1[5].value].key = vid50; 
					actvvs0[5][capsule0_so1[5].value].value = vid51;
					capsule0_so1[5].value++;
				}
				if(vdata52 == VISITED_IN_CURRENT_ITERATION || vdata53 == VISITED_IN_CURRENT_ITERATION){ 
					#ifdef _DEBUGMODE_CHECKS2
					actsutilityobj->checkoutofbounds("acts::collectactvvs_bfs 4", capsule1_so1[5].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
					#endif
					actvvs1[5][capsule1_so1[5].value].key = vid52; 
					actvvs1[5][capsule1_so1[5].value].value = vid53;
					capsule1_so1[5].value++;
				}
				if(vdata60 == VISITED_IN_CURRENT_ITERATION || vdata61 == VISITED_IN_CURRENT_ITERATION){ 
					#ifdef _DEBUGMODE_CHECKS2
					actsutilityobj->checkoutofbounds("acts::collectactvvs_bfs 3", capsule0_so1[6].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
					#endif
					actvvs0[6][capsule0_so1[6].value].key = vid60; 
					actvvs0[6][capsule0_so1[6].value].value = vid61;
					capsule0_so1[6].value++;
				}
				if(vdata62 == VISITED_IN_CURRENT_ITERATION || vdata63 == VISITED_IN_CURRENT_ITERATION){ 
					#ifdef _DEBUGMODE_CHECKS2
					actsutilityobj->checkoutofbounds("acts::collectactvvs_bfs 4", capsule1_so1[6].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
					#endif
					actvvs1[6][capsule1_so1[6].value].key = vid62; 
					actvvs1[6][capsule1_so1[6].value].value = vid63;
					capsule1_so1[6].value++;
				}
				if(vdata70 == VISITED_IN_CURRENT_ITERATION || vdata71 == VISITED_IN_CURRENT_ITERATION){ 
					#ifdef _DEBUGMODE_CHECKS2
					actsutilityobj->checkoutofbounds("acts::collectactvvs_bfs 3", capsule0_so1[7].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
					#endif
					actvvs0[7][capsule0_so1[7].value].key = vid70; 
					actvvs0[7][capsule0_so1[7].value].value = vid71;
					capsule0_so1[7].value++;
				}
				if(vdata72 == VISITED_IN_CURRENT_ITERATION || vdata73 == VISITED_IN_CURRENT_ITERATION){ 
					#ifdef _DEBUGMODE_CHECKS2
					actsutilityobj->checkoutofbounds("acts::collectactvvs_bfs 4", capsule1_so1[7].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
					#endif
					actvvs1[7][capsule1_so1[7].value].key = vid72; 
					actvvs1[7][capsule1_so1[7].value].value = vid73;
					capsule1_so1[7].value++;
				}
				
				#ifdef _WIDEWORD
				longdata.range(1, 0) = tmpvdata00; 
				longdata.range(3, 2) = tmpvdata01; 
				longdata.range(5, 4) = tmpvdata02; 
				longdata.range(7, 6) = tmpvdata03; 
				longdata.range(9, 8) = tmpvdata10; 
				longdata.range(11, 10) = tmpvdata11; 
				longdata.range(13, 12) = tmpvdata12; 
				longdata.range(15, 14) = tmpvdata13; 
				longdata.range(17, 16) = tmpvdata20; 
				longdata.range(19, 18) = tmpvdata21; 
				longdata.range(21, 20) = tmpvdata22; 
				longdata.range(23, 22) = tmpvdata23; 
				longdata.range(25, 24) = tmpvdata30; 
				longdata.range(27, 26) = tmpvdata31; 
				longdata.range(29, 28) = tmpvdata32; 
				longdata.range(31, 30) = tmpvdata33; 
				longdata.range(33, 32) = tmpvdata40; 
				longdata.range(35, 34) = tmpvdata41; 
				longdata.range(37, 36) = tmpvdata42; 
				longdata.range(39, 38) = tmpvdata43; 
				longdata.range(41, 40) = tmpvdata50; 
				longdata.range(43, 42) = tmpvdata51; 
				longdata.range(45, 44) = tmpvdata52; 
				longdata.range(47, 46) = tmpvdata53; 
				longdata.range(49, 48) = tmpvdata60; 
				longdata.range(51, 50) = tmpvdata61; 
				longdata.range(53, 52) = tmpvdata62; 
				longdata.range(55, 54) = tmpvdata63; 
				longdata.range(57, 56) = tmpvdata70; 
				longdata.range(59, 58) = tmpvdata71; 
				longdata.range(61, 60) = tmpvdata72; 
				longdata.range(63, 62) = tmpvdata73; 
				#else 
				WRITETO_ULONG(&data, 0, 2, tmpvdata00); 
				WRITETO_ULONG(&data, 2, 2, tmpvdata01); 
				WRITETO_ULONG(&data, 4, 2, tmpvdata02); 
				WRITETO_ULONG(&data, 6, 2, tmpvdata03); 
				WRITETO_ULONG(&data, 8, 2, tmpvdata10); 
				WRITETO_ULONG(&data, 10, 2, tmpvdata11); 
				WRITETO_ULONG(&data, 12, 2, tmpvdata12); 
				WRITETO_ULONG(&data, 14, 2, tmpvdata13); 
				WRITETO_ULONG(&data, 16, 2, tmpvdata20); 
				WRITETO_ULONG(&data, 18, 2, tmpvdata21); 
				WRITETO_ULONG(&data, 20, 2, tmpvdata22); 
				WRITETO_ULONG(&data, 22, 2, tmpvdata23); 
				WRITETO_ULONG(&data, 24, 2, tmpvdata30); 
				WRITETO_ULONG(&data, 26, 2, tmpvdata31); 
				WRITETO_ULONG(&data, 28, 2, tmpvdata32); 
				WRITETO_ULONG(&data, 30, 2, tmpvdata33); 
				WRITETO_ULONG(&data, 32, 2, tmpvdata40); 
				WRITETO_ULONG(&data, 34, 2, tmpvdata41); 
				WRITETO_ULONG(&data, 36, 2, tmpvdata42); 
				WRITETO_ULONG(&data, 38, 2, tmpvdata43); 
				WRITETO_ULONG(&data, 40, 2, tmpvdata50); 
				WRITETO_ULONG(&data, 42, 2, tmpvdata51); 
				WRITETO_ULONG(&data, 44, 2, tmpvdata52); 
				WRITETO_ULONG(&data, 46, 2, tmpvdata53); 
				WRITETO_ULONG(&data, 48, 2, tmpvdata60); 
				WRITETO_ULONG(&data, 50, 2, tmpvdata61); 
				WRITETO_ULONG(&data, 52, 2, tmpvdata62); 
				WRITETO_ULONG(&data, 54, 2, tmpvdata63); 
				WRITETO_ULONG(&data, 56, 2, tmpvdata70); 
				WRITETO_ULONG(&data, 58, 2, tmpvdata71); 
				WRITETO_ULONG(&data, 60, 2, tmpvdata72); 
				WRITETO_ULONG(&data, 62, 2, tmpvdata73); 
				#endif
			}
			
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("acts::collectactvvs_bfs 1", dest_v, VECTOR_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("acts::collectactvvs_bfs 2", destoffset_kvs + dest_i, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			#endif
			#ifdef _WIDEWORD
			keyvalue_t data2 = CONVERTTOKV_ULONG(longdata);
			destbuffer[dest_v][destoffset_kvs + dest_i] = data2;
			#else 
			destbuffer[dest_v][destoffset_kvs + dest_i] = data;
			#endif
			dest_v+=1; if(dest_v == VECTOR_SIZE){ dest_v=0; dest_i+=1; }
		}
		
		keyvalue_t dummykv;
		dummykv.key = INVALIDDATA;
		dummykv.value = INVALIDDATA;
		COLLECTACTVVS_LOOP2: for(vector_type k=0; k<4; k++){
			for(vector_type v=0; v<VECTOR_SIZE; v++){
			#pragma HLS UNROLL
				if(capsule0_so1[v].value > 0 && (capsule0_so1[v].value % 4) > 0){
					if(k >= capsule0_so1[v].value % 4){
						actvvs0[v][capsule0_so1[v].value] = dummykv;
						capsule0_so1[v].value++;
					}
				}
				if(capsule1_so1[v].value > 0 && (capsule1_so1[v].value % 4) > 0){
					if(k >= capsule1_so1[v].value % 4){
						actvvs1[v][capsule1_so1[v].value] = dummykv;
						capsule1_so1[v].value++;
					}
				}
			}
		}
		COLLECTACTVVS_LOOP3: for(vector_type v=0; v<VECTOR_SIZE; v++){ 
		#pragma HLS UNROLL
			cutoffs0[v] = capsule0_so1[v].value;
			cutoffs1[v] = capsule1_so1[v].value;
		}
	}
	
	// run pipeline & save. // CRITICAL FIXME. cause of FPGA bug?? FPGABUGCOMPLANT
	if(nonzeroactvvsreturned == ON || (enableflush == ON && index < 8)){
		if(itercount_actvvs0 >= 2){ writeen_actvvs0 = ON; } else { writeen_actvvs0 = OFF; }
		capsule0_so8 = runpipeline_1partition(ON, actvvs0, capsule0_so1, buffer0_setof2, capsule0_so2, buffer0_setof4, capsule0_so4, buffer0_setof8, capsule0_so8, sweepparams.currentLOP, sweepparams, WORKBUFFER_SIZE, cutoffs0, itercount_actvvs0, globalparams);
		buffer_type approxwritesize0_kvs = getwritesz1((capsule0_so8.value / VECTOR_SIZE));
		savekeyvalues(writeen_actvvs0, vdram, buffer0_setof8, globalparams.baseoffset_activevertices_kvs + actvvstravstate->i_kvs, approxwritesize0_kvs); 
		if(writeen_actvvs0 == ON){ actvvstravstate->i_kvs += capsule0_so8.value / VECTOR_SIZE; actvvstravstate->i += 2 * capsule0_so8.value; }
		if(itercount_actvvs1 >= 2){ writeen_actvvs1 = ON; } else { writeen_actvvs1 = OFF; }
		capsule1_so8 = runpipeline_1partition(ON, actvvs1, capsule1_so1, buffer1_setof2, capsule1_so2, buffer1_setof4, capsule1_so4, buffer1_setof8, capsule1_so8, sweepparams.currentLOP, sweepparams, WORKBUFFER_SIZE, cutoffs1, itercount_actvvs1, globalparams);
		buffer_type approxwritesize1_kvs = getwritesz1((capsule1_so8.value / VECTOR_SIZE));
		savekeyvalues(writeen_actvvs1, vdram, buffer1_setof8, globalparams.baseoffset_activevertices_kvs + actvvstravstate->i_kvs, approxwritesize1_kvs); 
		if(writeen_actvvs1 == ON){ actvvstravstate->i_kvs += capsule1_so8.value / VECTOR_SIZE; actvvstravstate->i += 2 * capsule1_so8.value; }
	}
	return nonzeroactvvsreturned;
}

void 
	#ifdef SW 
	acts::
	#endif 
reducebufferpartitions(bool_type enable, keyvalue_t kvdrambuffer[NUM_PARTITIONS][PADDEDDESTBUFFER_SIZE], keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t localcapsule[NUM_PARTITIONS], sweepparams_t sweepparams, globalparams_t globalparams){				
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount = PADDEDDESTBUFFER_SIZE / (NUM_PARTITIONS / 2); 
	
	uint512_vec_dt data0;
	keyvalue_t kvdata0;
	uint512_vec_dt data1;
	keyvalue_t kvdata1;
	uint512_vec_dt data2;
	keyvalue_t kvdata2;
	uint512_vec_dt data3;
	keyvalue_t kvdata3;
	uint512_vec_dt data4;
	keyvalue_t kvdata4;
	uint512_vec_dt data5;
	keyvalue_t kvdata5;
	uint512_vec_dt data6;
	keyvalue_t kvdata6;
	uint512_vec_dt data7;
	keyvalue_t kvdata7;
	uint512_vec_dt data8;
	keyvalue_t kvdata8;
	uint512_vec_dt data9;
	keyvalue_t kvdata9;
	uint512_vec_dt data10;
	keyvalue_t kvdata10;
	uint512_vec_dt data11;
	keyvalue_t kvdata11;
	uint512_vec_dt data12;
	keyvalue_t kvdata12;
	uint512_vec_dt data13;
	keyvalue_t kvdata13;
	uint512_vec_dt data14;
	keyvalue_t kvdata14;
	uint512_vec_dt data15;
	keyvalue_t kvdata15;
	buffer_type bramoffset_kvs[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=bramoffset_kvs complete
	buffer_type size_kvs[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=size_kvs complete
	buffer_type maxsize_kvs = 0;
	unsigned int upperlimits[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=upperlimits complete
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printkeyvalues("reducebufferpartitions.localcapsule", (keyvalue_t *)localcapsule, NUM_PARTITIONS);
	#endif 
	#ifdef _DEBUGMODE_STATS
	unsigned int i=0;
	unsigned int isgreaterthan512 = OFF;
	unsigned int islessthan512 = OFF;
	for(i=0; i<NUM_PARTITIONS; i++){ if(localcapsule[i].value > (1024*4)){ isgreaterthan512 = ON; }} // 512, 1024
	if(isgreaterthan512 == OFF){ islessthan512 = ON; }
	if(isgreaterthan512 == ON && islessthan512 == ON){ cout<<"ERROR: should never be seen here. exiting... "<<endl; exit(EXIT_FAILURE); }
	if(isgreaterthan512 == ON){ stats_greaterthan512 += 1; }
	if(islessthan512 == ON){ stats_lessthan512 += 1; }
	stats_totals += 1;
	#endif 

	for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		bramoffset_kvs[p] = localcapsule[p].key / VECTOR_SIZE;
		size_kvs[p] = localcapsule[p].value / VECTOR_SIZE;
		if(maxsize_kvs < size_kvs[p]){ maxsize_kvs = size_kvs[p]; }
	}
	
	unsigned int lloprange = getrangeforeachllop(globalparams);
	unsigned int tmplloprange = 0;
	for(unsigned int p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		upperlimits[p] = sweepparams.upperlimit + tmplloprange;
		tmplloprange += lloprange;
	}
	
	REDUCEBUFFERPARTITIONS_LOOP: for(buffer_type i=0; i<maxsize_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE II=32 // CRITICAL FIXME. VHLS
		for(vector_type v=0; v<VECTOR_SIZE; v++){
		// #pragma HLS PIPELINE II=3
			kvdata0 = buffer[v][bramoffset_kvs[0] + i]; 	
			kvdata1 = buffer[v][bramoffset_kvs[1] + i]; 	
			kvdata2 = buffer[v][bramoffset_kvs[2] + i]; 	
			kvdata3 = buffer[v][bramoffset_kvs[3] + i]; 	
			kvdata4 = buffer[v][bramoffset_kvs[4] + i]; 	
			kvdata5 = buffer[v][bramoffset_kvs[5] + i]; 	
			kvdata6 = buffer[v][bramoffset_kvs[6] + i]; 	
			kvdata7 = buffer[v][bramoffset_kvs[7] + i]; 	
			kvdata8 = buffer[v][bramoffset_kvs[8] + i]; 	
			kvdata9 = buffer[v][bramoffset_kvs[9] + i]; 	
			kvdata10 = buffer[v][bramoffset_kvs[10] + i]; 	
			kvdata11 = buffer[v][bramoffset_kvs[11] + i]; 	
			kvdata12 = buffer[v][bramoffset_kvs[12] + i]; 	
			kvdata13 = buffer[v][bramoffset_kvs[13] + i]; 	
			kvdata14 = buffer[v][bramoffset_kvs[14] + i]; 	
			kvdata15 = buffer[v][bramoffset_kvs[15] + i]; 	
	

			if(i< size_kvs[0]){ reducevector(kvdata0, kvdrambuffer[0], upperlimits[0], sweepparams, globalparams); }
			if(i< size_kvs[1]){ reducevector(kvdata1, kvdrambuffer[1], upperlimits[1], sweepparams, globalparams); }
			if(i< size_kvs[2]){ reducevector(kvdata2, kvdrambuffer[2], upperlimits[2], sweepparams, globalparams); }
			if(i< size_kvs[3]){ reducevector(kvdata3, kvdrambuffer[3], upperlimits[3], sweepparams, globalparams); }
			if(i< size_kvs[4]){ reducevector(kvdata4, kvdrambuffer[4], upperlimits[4], sweepparams, globalparams); }
			if(i< size_kvs[5]){ reducevector(kvdata5, kvdrambuffer[5], upperlimits[5], sweepparams, globalparams); }
			if(i< size_kvs[6]){ reducevector(kvdata6, kvdrambuffer[6], upperlimits[6], sweepparams, globalparams); }
			if(i< size_kvs[7]){ reducevector(kvdata7, kvdrambuffer[7], upperlimits[7], sweepparams, globalparams); }
			if(i< size_kvs[8]){ reducevector(kvdata8, kvdrambuffer[8], upperlimits[8], sweepparams, globalparams); }
			if(i< size_kvs[9]){ reducevector(kvdata9, kvdrambuffer[9], upperlimits[9], sweepparams, globalparams); }
			if(i< size_kvs[10]){ reducevector(kvdata10, kvdrambuffer[10], upperlimits[10], sweepparams, globalparams); }
			if(i< size_kvs[11]){ reducevector(kvdata11, kvdrambuffer[11], upperlimits[11], sweepparams, globalparams); }
			if(i< size_kvs[12]){ reducevector(kvdata12, kvdrambuffer[12], upperlimits[12], sweepparams, globalparams); }
			if(i< size_kvs[13]){ reducevector(kvdata13, kvdrambuffer[13], upperlimits[13], sweepparams, globalparams); }
			if(i< size_kvs[14]){ reducevector(kvdata14, kvdrambuffer[14], upperlimits[14], sweepparams, globalparams); }
			if(i< size_kvs[15]){ reducevector(kvdata15, kvdrambuffer[15], upperlimits[15], sweepparams, globalparams); }
		}
	}
	return;
}

void
	#ifdef SW 
	acts::
	#endif 
reducepartition(
		bool_type enablereduce, 
		keyvalue_t sourcevbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
uint512_dt * kvdram0,keyvalue_t tempverticesbuffer0[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],		unsigned int indexpp, unsigned int reducesubchunksz,
		travstate_t rtravstate[NUMSUBCPUTHREADS], sweepparams_t sweepparams, globalparams_t _globalparams){
	if(enablereduce == OFF){ return; }
	analysis_type analysis_sourceploop = (1 << (NUM_PARTITIONS_POW * TREE_DEPTH));
	analysis_type analysis_reduceloop = (MAXKVDATA_BATCHSIZE / (1 << (NUM_PARTITIONS_POW * TREE_DEPTH))) / SRCBUFFER_SIZE;
	analysis_type analysis_treedepth = TREE_DEPTH;
	
	#ifdef HW 	
	keyvalue_t vubufferpp00[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = vubufferpp00
	keyvalue_t vubufferpp10[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = vubufferpp10
	
	#else 
	keyvalue_t vubufferpp0[NUMSUBCPUTHREADS][VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	keyvalue_t vubufferpp1[NUMSUBCPUTHREADS][VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#endif
	keyvalue_t temptempverticesbuffer0[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = temptempverticesbuffer0
	
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
		rtravstatepp0[i] = rtravstate[i]; 
		rtravstatepp1[i] = rtravstate[i]; 
	}
	
	batch_type maxsz_kvs = 0;
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ if(rtravstate[i].size_kvs > maxsz_kvs){ maxsz_kvs = rtravstate[i].size_kvs; }}
	unsigned int enpp1f = ON;
	#ifdef RPP1
	unsigned int totsz_kvs = maxsz_kvs + rtravstate[0].skip_kvs;
	#else 
	unsigned int totsz_kvs = maxsz_kvs;
	#endif
	if(enablereduce == OFF){ totsz_kvs = 0; }
	
	// replicatedata_syn(enablereduce, sourcevbuffer, tempverticesbuffer0, indexpp * reducesubchunksz, reducesubchunksz); 
	replicatedata_bfs_syn(enablereduce, sourcevbuffer, tempverticesbuffer0, temptempverticesbuffer0, indexpp * reducesubchunksz, reducesubchunksz); 
	
	#ifdef RPP1
	MAIN_LOOP1E_REDUCE: for(offset_kvs=0; offset_kvs<totsz_kvs; offset_kvs+=2*rtravstate[0].skip_kvs){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_reduceloop avg=analysis_reduceloop
		#ifdef _DEBUGMODE_KERNELPRINTS
		for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ actsutilityobj->print6("### dispatch::reduce:: offset1_kvs", "offset2_kvs", "begin_kvs", "end_kvs", "size_kvs", "skip", rtravstate[i].begin_kvs + offset1_kvs, rtravstate[i].begin_kvs + offset2_kvs, rtravstate[i].begin_kvs, rtravstate[i].end_kvs, rtravstate[i].size_kvs, SRCBUFFER_SIZE); }
		#endif
		
		if(offset_kvs > 0){ enpp1f = ON; } else { enpp1f = OFF; }
		
		// read 0
		offset1_kvs = offset_kvs;
		for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ 
			if(offset1_kvs < rtravstate[i].size_kvs){ enpp0[i] = ON; } else { enpp0[i] = OFF; }
		}
		for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ 
			if(enpp0[i] == ON){ rtravstatepp0[i].i_kvs = rtravstatepp0[i].begin_kvs + offset1_kvs; }
		}
		readkeyvalues(enpp0[0], kvdram0, vubufferpp00, (sweepparams.worksourcebaseaddress_kvs + rtravstatepp0[0].begin_kvs + offset1_kvs), SRCBUFFER_SIZE, rtravstatepp0[0]);
		// reduce 1
		#if defined(INMEMORYGP) && defined(PR_ALGORITHM)
		reduce(enpp1f, enpp1[0], vubufferpp10, tempverticesbuffer0, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp1[0], _globalparams); 
		#endif
		#if defined(INMEMORYGP) && defined(BFS_ALGORITHM)
		reduce_bfs(enpp1f, enpp1[0], vubufferpp10, temptempverticesbuffer0, tempverticesbuffer0, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp1[0], _globalparams); 
		#endif 
		#if defined(INMEMORYGP) && defined(SSSP_ALGORITHM)
		reduce_sssp(enpp1f, enpp1[0], vubufferpp10, tempverticesbuffer0, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp1[0], _globalparams); 
		#endif 
		
		// reduce 0
		#if defined(INMEMORYGP) && defined(PR_ALGORITHM)
		reduce(ON, enpp0[0], vubufferpp00, tempverticesbuffer0, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp0[0], _globalparams);
		#endif
		#if defined(INMEMORYGP) && defined(BFS_ALGORITHM)
		reduce_bfs(ON, enpp0[0], vubufferpp00, temptempverticesbuffer0, tempverticesbuffer0, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp0[0], _globalparams);
		#endif 
		#if defined(INMEMORYGP) && defined(SSSP_ALGORITHM)
		reduce_sssp(ON, enpp0[0], vubufferpp00, tempverticesbuffer0, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstatepp0[0], _globalparams);
		#endif 
		
		// read 1
		offset2_kvs = offset_kvs + rtravstate[0].skip_kvs;
		for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ 
			if(offset2_kvs < rtravstate[i].size_kvs){ enpp1[i] = ON; } else { enpp1[i] = OFF; }
		}
		for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ 
			if(enpp1[i] == ON){ rtravstatepp1[i].i_kvs = rtravstatepp1[i].begin_kvs + offset2_kvs; }
		}
		readkeyvalues(enpp1[0], kvdram0, vubufferpp10, (sweepparams.worksourcebaseaddress_kvs + rtravstatepp1[0].begin_kvs + offset2_kvs), SRCBUFFER_SIZE, rtravstatepp1[0]);
	}
	#else 
	MAIN_LOOP1E_REDUCE: for(offset_kvs=0; offset_kvs<totsz_kvs; offset_kvs+=rtravstate[0].skip_kvs){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_reduceloop avg=analysis_reduceloop
		#ifdef _DEBUGMODE_KERNELPRINTS
		for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ actsutilityobj->print4("### dispatch::reduce:: offset_kvs", "begin_kvs", "end_kvs", "skip", rtravstate[i].begin_kvs + offset_kvs, rtravstate[i].begin_kvs, rtravstate[i].end_kvs, SRCBUFFER_SIZE); }
		#endif
		
		for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ 
			if(offset_kvs < rtravstate[i].size_kvs){ en[i] = ON; } else { en[i] = OFF; }
		}
		
		for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ 
			if(en[i] == ON){ rtravstate[i].i_kvs = rtravstate[i].begin_kvs + offset_kvs; }
		}

		readkeyvalues(en[0], kvdram0, vubufferpp00, (sweepparams.worksourcebaseaddress_kvs + rtravstate[0].begin_kvs + offset_kvs), SRCBUFFER_SIZE, rtravstate[0]);

		#if defined(INMEMORYGP) && defined(PR_ALGORITHM) 
		reduce(ON, en[0], vubufferpp00, tempverticesbuffer0, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[0], _globalparams);
		#endif
		#if defined(INMEMORYGP) && defined(BFS_ALGORITHM) 
		reduce_bfs(ON, en[0], vubufferpp00, temptempverticesbuffer0, tempverticesbuffer0, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[0], _globalparams);
		#endif 
		#if defined(INMEMORYGP) && defined(SSSP_ALGORITHM) 
		reduce_sssp(ON, en[0], vubufferpp00, tempverticesbuffer0, sweepparams, _globalparams.GraphIter, _globalparams.GraphAlgo, rtravstate[0], _globalparams);
		#endif
	}	
	#endif
	return;
}

// actit 
void 
	#ifdef SW 
	acts::
	#endif 
resetenvbuffers(skeyvalue_t capsule_so1[8][NUM_PARTITIONS], skeyvalue_t capsule_so2[4][NUM_PARTITIONS], skeyvalue_t capsule_so4[2][NUM_PARTITIONS], skeyvalue_t capsule_so8[NUM_PARTITIONS]){
	for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		capsule_so1[0][p].key = 0;
		capsule_so1[0][p].value = 0;
		capsule_so1[1][p].key = 0;
		capsule_so1[1][p].value = 0;
		capsule_so1[2][p].key = 0;
		capsule_so1[2][p].value = 0;
		capsule_so1[3][p].key = 0;
		capsule_so1[3][p].value = 0;
		capsule_so1[4][p].key = 0;
		capsule_so1[4][p].value = 0;
		capsule_so1[5][p].key = 0;
		capsule_so1[5][p].value = 0;
		capsule_so1[6][p].key = 0;
		capsule_so1[6][p].value = 0;
		capsule_so1[7][p].key = 0;
		capsule_so1[7][p].value = 0;
		capsule_so2[0][p].key = 0;
		capsule_so2[0][p].value = 0;
		capsule_so2[1][p].key = 0;
		capsule_so2[1][p].value = 0;
		capsule_so2[2][p].key = 0;
		capsule_so2[2][p].value = 0;
		capsule_so2[3][p].key = 0;
		capsule_so2[3][p].value = 0;
		capsule_so4[0][p].key = 0;
		capsule_so4[0][p].value = 0;
		capsule_so4[1][p].key = 0;
		capsule_so4[1][p].value = 0;
		capsule_so8[p].key = 0;
		capsule_so8[p].value = 0;
	}
	return;
}

void 
	#ifdef SW 
	acts::
	#endif 
fetchkeyvalues(bool_type enable, unsigned int mode, uint512_dt * kvdram, keyvalue_t kvdrambuffer[NUM_PARTITIONS][PADDEDDESTBUFFER_SIZE], keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], batch_type offset_kvs, batch_type size_kvs, travstate_t travstate){
	if(mode == PROCESSMODE){
		// NOT IMPLEMENTED.
	} else if(mode == PARTITIONMODE){
		readkeyvalues(enable, kvdram, buffer, offset_kvs, size_kvs, travstate); 
	} else if(mode == REDUCEMODE){
		readkeyvalues(enable, kvdram, buffer, offset_kvs, size_kvs, travstate); 
	} else {}
	return;
}

void 
	#ifdef SW 
	acts::
	#endif 
commitkeyvalues(bool_type enable, unsigned int mode, uint512_dt * kvdram, keyvalue_t kvdrambuffer[NUM_PARTITIONS][PADDEDDESTBUFFER_SIZE], keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t globalcapsule[GLOBALSTATSBUFFERSZ], skeyvalue_t localcapsule[NUM_PARTITIONS], batch_type destbaseaddr_kvs, sweepparams_t sweepparams, globalparams_t globalparams){				
	if(mode == PROCESSMODE){
		// NOT IMPLEMENTED.
	} else if(mode == PARTITIONMODE){
		savekeyvalues(enable, kvdram, buffer, globalcapsule, localcapsule, destbaseaddr_kvs, globalparams); 
	} else if(mode == REDUCEMODE){
		reducebufferpartitions(enable, kvdrambuffer, buffer, localcapsule, sweepparams, globalparams);
	} else {}
	return;
}

void 
	#ifdef SW 
	acts::
	#endif
actit(bool_type enable, unsigned int mode, bool_type resetenv,
		uint512_dt * kvdram, keyvalue_t kvdrambuffer[NUM_PARTITIONS][PADDEDDESTBUFFER_SIZE], keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ],
		globalparams_t globalparams, sweepparams_t sweepparams, travstate_t ptravstate, batch_type sourcebaseaddr_kvs, batch_type destbaseaddr_kvs){
	#pragma HLS allocation function instances=reducebufferpartitions limit=1 // CRITICAL REMOVEME?????????????????????????
	analysis_type analysis_partitionloop = KVDATA_BATCHSIZE_KVS / (NUMPARTITIONUPDATESPIPELINES * WORKBUFFER_SIZE);
	if(enable == OFF){ return; }
	
	keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = sourcebuffer
	keyvalue_t buffer_setof1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer_setof1
	keyvalue_t buffer_setof2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer_setof2
	keyvalue_t buffer_setof4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer_setof4
	keyvalue_t buffer_setof8[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer_setof8
	
	skeyvalue_t capsule_so1[8][NUM_PARTITIONS];
	#pragma HLS array_partition variable = capsule_so1
	skeyvalue_t capsule_so2[4][NUM_PARTITIONS];
	#pragma HLS array_partition variable = capsule_so2
	skeyvalue_t capsule_so4[2][NUM_PARTITIONS];
	#pragma HLS array_partition variable = capsule_so4
	skeyvalue_t capsule_so8[NUM_PARTITIONS];
	
	travstate_t ptravstatepp0 = ptravstate;
	travstate_t ptravstatepp1 = ptravstate;
	
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
	
	// if(resetenv == ON){
	resetenvbuffers(capsule_so1, capsule_so2, capsule_so4, capsule_so8);
	// }
	
	ACTIT_MAINLOOP: for(batch_type offset_kvs=ptravstate.begin_kvs; offset_kvs<ptravstate.end_kvs + 3*SRCBUFFER_SIZE; offset_kvs+=WORKBUFFER_SIZE * NUMPARTITIONUPDATESPIPELINES){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_partitionloop avg=analysis_partitionloop
		#ifdef _DEBUGMODE_KERNELPRINTS
		actsutilityobj->print6("### actit:: offset0_kvs", "size0_kvs", "begin_kvs", "end_kvs", "skip", "itercount", offset_kvs, WORKBUFFER_SIZE, ptravstate.begin_kvs, ptravstate.end_kvs, WORKBUFFER_SIZE, itercount);
		actsutilityobj->print6("### actit:: offset1_kvs", "size1_kvs", "begin_kvs", "end_kvs", "skip", "itercount", offset_kvs + WORKBUFFER_SIZE, WORKBUFFER_SIZE, ptravstate.begin_kvs, ptravstate.end_kvs, WORKBUFFER_SIZE, itercount);
		#endif
		
		ptravstatepp0.i_kvs =  offset_kvs;
		#ifdef PUP1
		ptravstatepp1.i_kvs = offset_kvs + WORKBUFFER_SIZE;
		#endif
		#ifdef PUP1
		if(itercount >= 2){ pp0writeen = ON; } else { pp0writeen = OFF; }
		if(itercount >= 4){ pp1writeen = ON; } else { pp1writeen = OFF; }
		if(itercount >= 2){ pp1runpipelineen = ON; } else { pp1runpipelineen = OFF; }
		#else 
		if(itercount >= 2){ pp0writeen = ON; } else { pp0writeen = OFF; }
		#endif
		
		fetchkeyvalues(ON, mode, kvdram, kvdrambuffer, sourcebuffer, (sourcebaseaddr_kvs + offset_kvs), WORKBUFFER_SIZE, ptravstatepp0);
		#ifdef PUP1
		runpipeline(pp1runpipelineen, buffer_setof1, capsule_so1, buffer_setof2, capsule_so2, buffer_setof4, capsule_so4, buffer_setof8, capsule_so8, sweepparams.currentLOP, sweepparams, WORKBUFFER_SIZE, pp1cutoffs, (itercount-2)+1, globalparams);
		#endif 
		
		preparekeyvalues(ON, sourcebuffer, buffer_setof1, capsule_so1, sweepparams.currentLOP, sweepparams, ptravstatepp0, WORKBUFFER_SIZE, pp0cutoffs, globalparams);
		#ifdef PUP1
		commitkeyvalues(pp1writeen, mode, kvdram, kvdrambuffer, buffer_setof8, globalstatsbuffer, capsule_so8, destbaseaddr_kvs, sweepparams, globalparams); 
		#endif 
		
		runpipeline(ON, buffer_setof1, capsule_so1, buffer_setof2, capsule_so2, buffer_setof4, capsule_so4, buffer_setof8, capsule_so8, sweepparams.currentLOP, sweepparams, WORKBUFFER_SIZE, pp0cutoffs, itercount, globalparams);
		#ifdef PUP1
		fetchkeyvalues(ON, mode, kvdram, kvdrambuffer, sourcebuffer, (sourcebaseaddr_kvs + offset_kvs + WORKBUFFER_SIZE), WORKBUFFER_SIZE, ptravstatepp1);
		#endif
		
		commitkeyvalues(pp0writeen, mode, kvdram, kvdrambuffer, buffer_setof8, globalstatsbuffer, capsule_so8, destbaseaddr_kvs, sweepparams, globalparams); 
		#ifdef PUP1
		preparekeyvalues(pp1partitionen, sourcebuffer, buffer_setof1, capsule_so1, sweepparams.currentLOP, sweepparams, ptravstatepp1, WORKBUFFER_SIZE, pp1cutoffs, globalparams);
		#endif

		itercount += NUMPARTITIONUPDATESPIPELINES;
	}
	return;
}

// dispatches (type 1)
void
	#ifdef SW 
	acts::
	#endif 
processit(uint512_dt * kvdram, keyvalue_t kvdrambuffer[NUM_PARTITIONS][PADDEDDESTBUFFER_SIZE], globalparams_t globalparams){
	#pragma HLS INLINE
	analysis_type analysis_numllops = 1;
	analysis_type analysis_numsourcepartitions = 1;
	analysis_type analysis_processedges_overallloop = BATCH_RANGE_KVS / PADDEDDESTBUFFER_SIZE;
	analysis_type analysis_processedges_loadedgebatch = 1;
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
	
	keyvalue_t buffer1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer1
	keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ]; 
	
	batch_type sourcestatsmarker = 0;
	batch_type deststatsmarker = 1;
	sweepparams_t sweepparams;
	travstate_t actvvstravstate; actvvstravstate.i=0; actvvstravstate.i_kvs=0; actvvstravstate.v=0; actvvstravstate.k=0; 
	
	step_type currentLOP=globalparams.beginLOP;
	batch_type num_source_partitions = get_num_source_partitions(currentLOP);
	
	MAIN_LOOP1: for(batch_type source_partition=0; source_partition<num_source_partitions; source_partition+=1){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_numsourcepartitions avg=analysis_numsourcepartitions

		resetmanykeyandvalues(globalstatsbuffer, NUM_PARTITIONS, 0);
	
		sweepparams = getsweepparams(globalparams, currentLOP, source_partition);
		travstate_t avtravstate;
		
		avtravstate.begin_kvs = 0;
		avtravstate.end_kvs = avtravstate.begin_kvs + globalparams.srcvsize_kvs; avtravstate.size_kvs = globalparams.srcvsize_kvs;
		readglobalstats(ON, kvdram, globalstatsbuffer, globalparams.baseoffset_statsdram_kvs + deststatsmarker); 
		resetvalues(globalstatsbuffer, NUM_PARTITIONS, 0);
		
		#ifdef _WIDEWORD
		vertex_t firstvptr = kvdram[globalparams.baseoffset_vertexptr_kvs + (globalparams.srcvoffset / VECTOR_SIZE)].range(31, 0);
		#else 
		vertex_t firstvptr = kvdram[globalparams.baseoffset_vertexptr_kvs + (globalparams.srcvoffset / VECTOR_SIZE)].data[0].key;
		#endif 
		
		#ifdef _DEBUGMODE_KERNELPRINTS2
		actsutilityobj->print7("### dispatch::processit:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, avtravstate.begin_kvs * VECTOR_SIZE, avtravstate.end_kvs * VECTOR_SIZE, (avtravstate.end_kvs - avtravstate.begin_kvs) * VECTOR_SIZE, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams.currentLOP)), sweepparams.currentLOP); 							
		#endif
		MAIN_LOOP1B: for(batch_type voffset_kvs=avtravstate.begin_kvs; voffset_kvs<avtravstate.end_kvs; voffset_kvs+=SRCBUFFER_SIZE){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_processedges_overallloop avg=analysis_processedges_overallloop
			#ifdef _DEBUGMODE_KERNELPRINTS2
			actsutilityobj->print4("### dispatch::processit:: voffset", "vbegin", "vend", "vskip", voffset_kvs * VECTOR_SIZE, avtravstate.begin_kvs * VECTOR_SIZE, avtravstate.size_kvs * VECTOR_SIZE, SRCBUFFER_SIZE * VECTOR_SIZE);
			#endif
			
			readkeyvalues(ON, kvdram, buffer1, globalparams.baseoffset_vertexptr_kvs + (globalparams.srcvoffset / VECTOR_SIZE) + voffset_kvs, SRCBUFFER_SIZE + 1);
			MAIN_LOOP1C: for(batch_type v=0; v<VECTOR_SIZE; v+=1){
				replicatevdata(ON, buffer1, kvdrambuffer, v * SRCBUFFER_SIZE, SRCBUFFER_SIZE + 1);
				
				vertex_t srcvlocaloffset = (voffset_kvs * VECTOR_SIZE) + (v * SRCBUFFER_SIZE);
				vertex_t myfirstsrcvid = globalparams.srcvoffset + (voffset_kvs * VECTOR_SIZE) + (v * SRCBUFFER_SIZE);
				vertex_t mylastsrcvid = myfirstsrcvid + SRCBUFFER_SIZE;// - 1;
				if(srcvlocaloffset >= globalparams.srcvsize){ mylastsrcvid = myfirstsrcvid; }
				if((srcvlocaloffset < globalparams.srcvsize) && (srcvlocaloffset + SRCBUFFER_SIZE >= globalparams.srcvsize)){ mylastsrcvid = myfirstsrcvid + globalparams.srcvsize - srcvlocaloffset; }
				
				keyy_t beginvptr = kvdrambuffer[0][0].key;
				keyy_t endvptr = kvdrambuffer[0][SRCBUFFER_SIZE].key;
				if(srcvlocaloffset >= globalparams.srcvsize){ endvptr = beginvptr; }
				if((srcvlocaloffset < globalparams.srcvsize) && (srcvlocaloffset + SRCBUFFER_SIZE >= globalparams.srcvsize)){ endvptr = kvdrambuffer[0][globalparams.srcvsize - srcvlocaloffset - 1].key; }
				
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
				
				travstate_t ptravstate;
				ptravstate.begin_kvs = localbeginvptr_kvs;
				ptravstate.size_kvs = numedges_kvs;
				ptravstate.end_kvs = ptravstate.begin_kvs + ptravstate.size_kvs;
				ptravstate.skip_kvs = SRCBUFFER_SIZE;
				ptravstate.i_kvs = ptravstate.begin_kvs;
				bool_type resetenv;
				if(source_partition==0 && voffset_kvs==avtravstate.begin_kvs && v==0){ resetenv = ON; } else { resetenv = OFF; }
				
				actit(
					ON, PARTITIONMODE, resetenv,
					kvdram, kvdrambuffer,
					globalstatsbuffer, globalparams, sweepparams, ptravstate, globalparams.baseoffset_edgesdata_kvs, globalparams.baseoffset_kvdramworkspace_kvs);

				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->printglobalvars();
				actsutilityobj->clearglobalvars();
				#endif
			}
		}
		
		saveglobalstats(ON, kvdram, globalstatsbuffer, globalparams.baseoffset_statsdram_kvs + deststatsmarker);
		
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
	// CRITICAL REMOVEME.
	// actsutilityobj->printkeyvalues("processit::globalstatsbuffer 33", globalstatsbuffer, NUM_PARTITIONS); 
	// exit(EXIT_SUCCESS); 
	return;
}

void
	#ifdef SW 
	acts::
	#endif 
partitionit(uint512_dt * kvdram, keyvalue_t kvdrambuffer[NUM_PARTITIONS][PADDEDDESTBUFFER_SIZE], globalparams_t globalparams){
	#pragma HLS INLINE
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
	
	keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ]; 
	keyvalue_t moretravstates[MAXLOADFACTORFORREDUCE];
	
	batch_type sourcestatsmarker = 1;
	batch_type deststatsmarker = 1 + NUM_PARTITIONS;
	config_t config;
	sweepparams_t sweepparams;
	travstate_t actvvstravstate; actvvstravstate.i=0; actvvstravstate.i_kvs=0; actvvstravstate.v=0; actvvstravstate.k=0; 
	
	MAIN_LOOP1: for(step_type currentLOP=globalparams.beginLOP + 1; currentLOP<(globalparams.beginLOP + globalparams.numLOPs - 1); currentLOP++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_numllops avg=analysis_numllops	
	
		batch_type num_source_partitions = get_num_source_partitions(currentLOP);
		bool_type enreduce = ON;
		
		MAIN_LOOP1B: for(batch_type source_partition=0; source_partition<num_source_partitions; source_partition+=1){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_numsourcepartitions avg=analysis_numsourcepartitions

			resetmanykeyandvalues(globalstatsbuffer, NUM_PARTITIONS, 0);
		
			sweepparams = getsweepparams(globalparams, currentLOP, source_partition);
			travstate_t travstate = gettravstate(ON, kvdram, globalparams, currentLOP, sourcestatsmarker, moretravstates);
			travstate_t ctravstate = travstate;
			travstate_t ptravstate = travstate;
			travstate_t avtravstate;
			
			// collect stats
			if(incollectstatsstage(currentLOP, globalparams) == true){ config.enableprocessedges = OFF; config.enablecollectglobalstats = ON; config.enablepartition = OFF; config.enablereduce = OFF; } 
			else { ctravstate.begin_kvs = 0; ctravstate.end_kvs = 0; config.enablecollectglobalstats = OFF; }
			if(incollectstatsstage(currentLOP, globalparams) == true && (ctravstate.end_kvs - ctravstate.begin_kvs) == 0){ ctravstate.begin_kvs = 0; ctravstate.end_kvs = 0; config.enablecollectglobalstats = OFF; } 
			readglobalstats(config.enablecollectglobalstats, kvdram, globalstatsbuffer, globalparams.baseoffset_statsdram_kvs + deststatsmarker);
			#if defined(_DEBUGMODE_CHECKS2) && defined(COLLECTSTATSOFFLINE)
			if(config.enablecollectglobalstats == ON){ actsutilityobj->copykeyvalues(actsutilityobj->getmykeyvalues(7), globalstatsbuffer, NUM_PARTITIONS); }
			#endif
			resetvalues(globalstatsbuffer, NUM_PARTITIONS, 0);
			
			// partition
			if(inpartitionstage(currentLOP, globalparams) == true && (ptravstate.size_kvs > 0)){ config.enableprocessedges = OFF; config.enablecollectglobalstats = OFF; config.enablepartition = ON; config.enablereduce = OFF; } 
			else { ptravstate.begin_kvs = 0; ptravstate.end_kvs = 0; config.enablepartition = OFF; }
			if(ptravstate.size_kvs == 0){ ptravstate.begin_kvs = 0; ptravstate.end_kvs = 0; config.enablepartition = OFF; }
			#ifdef _DEBUGMODE_KERNELPRINTS2
			if((config.enablepartition == ON) && (currentLOP >= 1) && (currentLOP <= globalparams.treedepth)){ actsutilityobj->print7("### dispatch::partition:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, ptravstate.begin_kvs * VECTOR_SIZE, ptravstate.end_kvs * VECTOR_SIZE, ptravstate.size_kvs * VECTOR_SIZE, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams.currentLOP)), sweepparams.currentLOP); }	
			#endif
			resetvalues(globalstatsbuffer, NUM_PARTITIONS, 0);
			bool_type resetenv; if(source_partition==0){ resetenv = ON; } else { resetenv = OFF; }
			actit(config.enablepartition, PARTITIONMODE, resetenv,
				kvdram, kvdrambuffer, globalstatsbuffer,
				globalparams, sweepparams, ptravstate, sweepparams.worksourcebaseaddress_kvs, sweepparams.workdestbaseaddress_kvs);
			if(inpartitionstage(currentLOP, globalparams) == true){ saveglobalstats(config.enablepartition, kvdram, globalstatsbuffer, globalparams.baseoffset_statsdram_kvs + deststatsmarker); }
			
			if(currentLOP > 0){
				sourcestatsmarker += 1;
				deststatsmarker += NUM_PARTITIONS;
			}
			
			#ifdef _DEBUGMODE_CHECKS
			if(config.enablereduce == ON){ actsutilityobj->printpartitionresult2(ON, kvdram, globalstatsbuffer, sweepparams); }
			#endif
			#ifdef _DEBUGMODE_STATS
			if(config.enablereduce == ON){ edges_count = actsutilityobj->globalstats_getcountvalidkvsreduced(); edgesdstv_sum = actsutilityobj->globalstats_getreducevar1(); }
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
reduceit(uint512_dt * kvdram, keyvalue_t kvdrambuffer[NUM_PARTITIONS][PADDEDDESTBUFFER_SIZE], batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparams){	
	#pragma HLS INLINE
	analysis_type analysis_numllops = 1;
	analysis_type analysis_numsourcepartitions = 1;
	analysis_type analysis_treedepth = TREE_DEPTH;
	
	keyvalue_t moretravstates[MAXLOADFACTORFORREDUCE];
	keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ]; 
	
	config_t config;
	sweepparams_t sweepparams;
	
	step_type currentLOP = globalparams.beginLOP + globalparams.numLOPs - 1;
	batch_type num_source_partitions = get_num_source_partitions(currentLOP);
	
	sweepparams = getsweepparams(globalparams, currentLOP, source_partition);
	travstate_t ptravstate = gettravstate(ON, kvdram, globalparams, currentLOP, sourcestatsmarker, moretravstates);

	if(ptravstate.size_kvs == 0){ ptravstate.begin_kvs = 0; ptravstate.end_kvs = 0; config.enablereduce = OFF; }
	else { config.enablereduce = ON; }
	#ifdef _DEBUGMODE_KERNELPRINTS2
	if(ptravstate.size_kvs > 0){ actsutilityobj->print7("### dispatch::reduce:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, ptravstate.begin_kvs * VECTOR_SIZE, ptravstate.end_kvs * VECTOR_SIZE, ptravstate.size_kvs * VECTOR_SIZE, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams.currentLOP)), sweepparams.currentLOP); }	
	#endif
	bool_type resetenv; if(source_partition==0){ resetenv = ON; } else { resetenv = OFF; }
	
	actit(config.enablereduce, REDUCEMODE, resetenv,
			kvdram, kvdrambuffer, globalstatsbuffer,
			globalparams, sweepparams, ptravstate, sweepparams.worksourcebaseaddress_kvs, sweepparams.workdestbaseaddress_kvs);
	return;
}

void
	#ifdef SW 
	acts::
	#endif 
reduceit(uint512_dt * kvdram, keyvalue_t kvdrambuffer[NUM_PARTITIONS][PADDEDDESTBUFFER_SIZE], globalparams_t globalparams){	
	#pragma HLS INLINE
	analysis_type analysis_numllops = 1;
	analysis_type analysis_numsourcepartitions = 1;
	analysis_type analysis_treedepth = TREE_DEPTH;
	
	batch_type sourcestatsmarker = 0;
	batch_type deststatsmarker = NAp; 
	batch_type destoffset = NAp; 
	for(unsigned int k=0; k<globalparams.treedepth-1; k++){ 
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_treedepth avg=analysis_treedepth
		sourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); 
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printparameters();
	actsutilityobj->printglobalvars();
	actsutilityobj->printglobalparameters("acts::getglobalparams:: printing global parameters", globalparams);
	#endif 
	#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(_DEBUGMODE_CHECKS2)
	actsutilityobj->clearglobalvars();
	#endif
	
	keyvalue_t moretravstates[MAXLOADFACTORFORREDUCE];
	keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ]; 

	config_t config;
	sweepparams_t sweepparams;
	
	step_type currentLOP = globalparams.beginLOP + globalparams.numLOPs - 1;
	batch_type num_source_partitions = get_num_source_partitions(currentLOP);
	
	MAIN_LOOP1B: for(batch_type source_partition=0; source_partition<num_source_partitions; source_partition+=1){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_numsourcepartitions avg=analysis_numsourcepartitions

		sweepparams = getsweepparams(globalparams, currentLOP, source_partition);
		travstate_t ptravstate = gettravstate(ON, kvdram, globalparams, currentLOP, sourcestatsmarker, moretravstates);
	
		if(ptravstate.size_kvs == 0){ ptravstate.begin_kvs = 0; ptravstate.end_kvs = 0; config.enablereduce = OFF; }
		else { config.enablereduce = ON; }
		#ifdef _DEBUGMODE_KERNELPRINTS2
		if(ptravstate.size_kvs > 0){ actsutilityobj->print7("### dispatch::reduce:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, ptravstate.begin_kvs * VECTOR_SIZE, ptravstate.end_kvs * VECTOR_SIZE, ptravstate.size_kvs * VECTOR_SIZE, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams.currentLOP)), sweepparams.currentLOP); }	
		#endif
		bool_type resetenv; if(source_partition==0){ resetenv = ON; } else { resetenv = OFF; }
		
		readkeyvalues2(config.enablereduce, kvdram, kvdrambuffer, (globalparams.baseoffset_verticesdata_kvs + (source_partition * PADDEDDESTBUFFER_SIZE * 2)), (globalparams.baseoffset_verticesdata_kvs + (source_partition * PADDEDDESTBUFFER_SIZE * 2) + PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, PADDEDDESTBUFFER_SIZE);
		
		actit(config.enablereduce, REDUCEMODE, resetenv, 
				kvdram, kvdrambuffer, globalstatsbuffer,
				globalparams, sweepparams, ptravstate, sweepparams.worksourcebaseaddress_kvs, sweepparams.workdestbaseaddress_kvs);

		savekeyvalues2(config.enablereduce, kvdram, kvdrambuffer, (globalparams.baseoffset_verticesdata_kvs + (source_partition * PADDEDDESTBUFFER_SIZE * 2)), (globalparams.baseoffset_verticesdata_kvs + (source_partition * PADDEDDESTBUFFER_SIZE * 2) + PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, PADDEDDESTBUFFER_SIZE);
		
		sourcestatsmarker += 1;
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
	return;
}

void
	#ifdef SW 
	acts::
	#endif 
dispatch(uint512_dt * kvdram, keyvalue_t kvdrambuffer[NUM_PARTITIONS][PADDEDDESTBUFFER_SIZE], globalparams_t globalparams){
	processit(kvdram, kvdrambuffer, globalparams);
	partitionit(kvdram, kvdrambuffer, globalparams);
	reduceit(kvdram, kvdrambuffer, globalparams);	
	return;
}

void
	#ifdef SW 
	acts::
	#endif 
dispatch(bool_type en_process, bool_type en_partition, bool_type en_reduce, uint512_dt * kvdram, keyvalue_t kvdrambuffer[NUM_PARTITIONS][PADDEDDESTBUFFER_SIZE], 
				batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparams){
	if(en_process == ON){ processit(kvdram, kvdrambuffer, globalparams); }
	if(en_partition == ON){ partitionit(kvdram, kvdrambuffer, globalparams); }
	if(en_reduce == ON){ reduceit(kvdram, kvdrambuffer, sourcestatsmarker, source_partition, globalparams);	}
	return;
}

travstate_t
	#ifdef SW 
	acts::
	#endif 
start_reduce(uint512_dt * vdram, uint512_dt * kvdram0, keyvalue_t kvdrambuffer0[NUM_PARTITIONS][PADDEDDESTBUFFER_SIZE], globalparams_t globalparams[NUMSUBCPUTHREADS]){
	analysis_type analysis_loop1 = (1 << (NUM_PARTITIONS_POW * TREE_DEPTH));
	analysis_type analysis_treedepth = TREE_DEPTH;
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printparameters();
	actsutilityobj->printglobalvars();
	#endif 
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	static keyvalue_t destvbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = destvbuffer
	static keyvalue_t destvbuffer2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = destvbuffer2
	travstate_t rtravstate[NUMSUBCPUTHREADS];
	#pragma HLS ARRAY_PARTITION variable=rtravstate complete
	keyvalue_t moretravstates[NUMSUBCPUTHREADS][MAXLOADFACTORFORREDUCE];
	#pragma HLS array_partition variable = moretravstates

	travstate_t actvvstravstate;
	actvvstravstate.i=0; actvvstravstate.i_kvs=0; actvvstravstate.v=0; actvvstravstate.k=0; 
	globalparams_t _globalparams = globalparams[0];
	unsigned int sourcestatsmarker = 0;
	for(unsigned int k=0; k<_globalparams.treedepth-1; k++){ 
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_treedepth avg=analysis_treedepth
		sourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); 
	}
	step_type currentLOP = _globalparams.beginLOP + _globalparams.numLOPs - 1;
	batch_type num_source_partitions = get_num_source_partitions(currentLOP);
	
	MAIN_LOOP: for(batch_type source_partition=0; source_partition<num_source_partitions; source_partition+=1){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loop1 avg=analysis_loop1
		#ifdef _DEBUGMODE_KERNELPRINTS
		actsutilityobj->print3("### dispatch::reduceit:: source_partition", "currentLOP", "NAp", source_partition, currentLOP, NAp); 							
		#endif
		
		bool_type enablereduce = ON;
		bool_type enableflush = OFF;
		
		if(source_partition >= num_source_partitions){ enableflush = ON; } else { enableflush = OFF; }
		if(enableflush == OFF){
 
		rtravstate[0] = gettravstate(ON, kvdram0, globalparams[0], currentLOP, sourcestatsmarker, moretravstates[0]);
 }
		batch_type totsize_kvs = 0;
		for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ totsize_kvs += rtravstate[i].size_kvs; }
		if((totsize_kvs == 0) || (source_partition >= num_source_partitions)){ enablereduce = OFF; } else { enablereduce = ON; }
		#ifdef _DEBUGMODE_KERNELPRINTS3
		if(totsize_kvs > 0){
		cout<<endl; for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ cout<<"reduceit: rtravstate["<<i<<"].size_kvs: "<<rtravstate[i].size_kvs<<endl; }
		actsutilityobj->print3("### dispatch::reduceit:: source_partition", "currentLOP", "totsize_kvs", source_partition, currentLOP, totsize_kvs); }
		#endif
		
		if(enablereduce == ON || enableflush == ON){
			readkeyvalues2(enablereduce, vdram, kvdrambuffer0, (_globalparams.baseoffset_verticesdata_kvs + (source_partition * PADDEDDESTBUFFER_SIZE * 2)), (_globalparams.baseoffset_verticesdata_kvs + (source_partition * PADDEDDESTBUFFER_SIZE * 2) + PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, PADDEDDESTBUFFER_SIZE); // AUTOMATEME.
			resetmanykeyandvalues(destvbuffer, PADDEDDESTBUFFER_SIZE, 0); // c++ sw issues
			
 
			// cout<<"start: ^^^reducing instance 0... "<<endl;
			dispatch(OFF, OFF, ON, kvdram0, kvdrambuffer0,
				sourcestatsmarker, source_partition, _globalparams);
			
			// #if defined(INMEMORYGP) && defined(BFS_ALGORITHM) // FIXME. NOTYETIMPLEMENTED.
			// unifydata_bfs_parallelsyn(enablereducepp0, tempverticesbuffer0, destvbuffer, 
					// indexpp0 * reducesubchunksz, reducesubchunksz, _globalparams);
			// #endif
			
			// #if defined(INMEMORYGP) && defined(BFS_ALGORITHM) // FIXME. NOTYETIMPLEMENTED.
			// nonzeroactvvsreturnedpp0 = collectactvvs_bfs(vdram, enablereducepp0, destvbuffer, destvbuffer2,
				// itercount_actvvs0, itercount_actvvs1,  
				// enableflush, indexpp0, sweepparams0, &actvvstravstate, 
				// (indexpp0 * reducesubchunksz), reducesubchunksz, source_partition * _globalparams.applyvertexbuffersz, _globalparams);
			// if(nonzeroactvvsreturnedpp0 == ON || (enableflush == ON && indexpp0 < 8)){  itercount_actvvs0 += 1;  itercount_actvvs1 += 1;  }
			// #endif
			
			savekeyvalues2(enablereduce, vdram, kvdrambuffer0, (_globalparams.baseoffset_verticesdata_kvs + (source_partition * PADDEDDESTBUFFER_SIZE * 2)), (_globalparams.baseoffset_verticesdata_kvs + (source_partition * PADDEDDESTBUFFER_SIZE * 2) + PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, PADDEDDESTBUFFER_SIZE);
			sourcestatsmarker += 1;
		}
		
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
	cout<<"reduceit_sync: number of active vertices for iteration "<<_globalparams.GraphIter + 1<<": (including INVALIDDATAs): "<<actvvstravstate.i<<endl;
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

void
	#ifdef SW 
	acts::
	#endif 
start(uint512_dt * kvdram, globalparams_t globalparams){
	analysis_type analysis_treedepth = TREE_DEPTH;
	analysis_type analysis_numsourcepartitions = 1;
	
	keyvalue_t kvdrambuffer[NUM_PARTITIONS][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = kvdrambuffer
	keyvalue_t moretravstates[MAXLOADFACTORFORREDUCE];
	config_t config;
	sweepparams_t sweepparams;
	batch_type sourcestatsmarker = 0;
	batch_type source_partition = 0;
	
	// process
	if(globalparams.processcommand == ON){
		dispatch(ON, OFF, OFF, kvdram, kvdrambuffer,
					sourcestatsmarker, source_partition, globalparams);
	}
	
	// partition 
	if(globalparams.partitioncommand == ON){
		dispatch(OFF, ON, OFF, kvdram, kvdrambuffer,
				sourcestatsmarker, source_partition, globalparams);
	}
	
	// reduce
	if(globalparams.reducecommand == ON){
		for(unsigned int k=0; k<globalparams.treedepth-1; k++){ 
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_treedepth avg=analysis_treedepth
			sourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); 
		}
		step_type currentLOP = globalparams.beginLOP + globalparams.numLOPs - 1;
		batch_type num_source_partitions = get_num_source_partitions(currentLOP);
		MAIN_LOOP1: for(source_partition=0; source_partition<num_source_partitions; source_partition+=1){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_numsourcepartitions avg=analysis_numsourcepartitions

			travstate_t ptravstate = gettravstate(ON, kvdram, globalparams, currentLOP, sourcestatsmarker, moretravstates);
			if(ptravstate.size_kvs == 0){ ptravstate.begin_kvs = 0; ptravstate.end_kvs = 0; config.enablereduce = OFF; }
			else { config.enablereduce = ON; }

			readkeyvalues2(config.enablereduce, kvdram, kvdrambuffer, (globalparams.baseoffset_verticesdata_kvs + (source_partition * PADDEDDESTBUFFER_SIZE * 2)), (globalparams.baseoffset_verticesdata_kvs + (source_partition * PADDEDDESTBUFFER_SIZE * 2) + PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, PADDEDDESTBUFFER_SIZE);
			
			dispatch(OFF, OFF, ON, kvdram, kvdrambuffer,
					sourcestatsmarker, source_partition, globalparams);
			
			savekeyvalues2(config.enablereduce, kvdram, kvdrambuffer, (globalparams.baseoffset_verticesdata_kvs + (source_partition * PADDEDDESTBUFFER_SIZE * 2)), (globalparams.baseoffset_verticesdata_kvs + (source_partition * PADDEDDESTBUFFER_SIZE * 2) + PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, PADDEDDESTBUFFER_SIZE);
			
			sourcestatsmarker += 1;
		}
	}
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->printglobalvars();
	#endif 
	#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(_DEBUGMODE_CHECKS2)
	actsutilityobj->clearglobalvars();
	#endif
	return;
}

void
	#ifdef SW 
	acts::
	#endif 
start(uint512_dt * vdram, uint512_dt * kvdram0, globalparams_t globalparams[NUMSUBCPUTHREADS]){
	keyvalue_t kvdrambuffer0[NUM_PARTITIONS][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = kvdrambuffer0

	// process
	if(globalparams[0].processcommand == ON){
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<"start: processing instance 0... "<<endl;
		#endif
		dispatch(ON, OFF, OFF, kvdram0, kvdrambuffer0,
					NAp, NAp, globalparams[0]);
	}
	
	// partition 
	if(globalparams[0].partitioncommand == ON){
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<"start: partitioning instance 0... "<<endl;
		#endif
		dispatch(OFF, ON, OFF, kvdram0, kvdrambuffer0,
				NAp, NAp, globalparams[0]);
	}
	
	// reduce
	if(globalparams[0].reducecommand == ON){
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<"start: reducing instances 0-1... "<<endl;
		#endif
		start_reduce(vdram, kvdram0,kvdrambuffer0, globalparams);
	}
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->printglobalvars();
	#endif 
	#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(_DEBUGMODE_CHECKS2)
	actsutilityobj->clearglobalvars();
	#endif
	return;
}

// dispatches (type 2)
void
	#ifdef SW 
	acts::
	#endif 
processit_sync(
			uint512_dt * vdram,
uint512_dt * kvdram0,keyvalue_t offsets0[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer10[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],	
			#ifndef COMPACTEDGES	
keyvalue_t kvbuffer20[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],		
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
		#ifdef _DEBUGMODE_KERNELPRINTS2
		cout<<"### processit_sync: offset_kvs: "<<offset_kvs<<", avtravstate.begin_kvs: "<<avtravstate.begin_kvs<<", avtravstate.size_kvs: "<<avtravstate.size_kvs<<endl;
		#endif
		avtravstate.i_kvs = offset_kvs;
	
		readkeyvalues(ON, vdram, actvvs0, _globalparams.baseoffset_activevertices_kvs + offset_kvs, PADDEDDESTBUFFER_SIZE / 2, avtravstate);
		
		replicatedata_syn(ON, actvvs0 ,actvvs0);
	
		buffer_type actvvscount0 = generateoffsets_random(kvdram0, actvvs0, offsets0, avtravstate, _globalparams, offset_kvs);
 
		
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
partitionit2(uint512_dt * kvdram, 
			keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
			keyvalue_t buffer_setof1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
			keyvalue_t buffer_setof2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
			keyvalue_t buffer_setof4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
			keyvalue_t buffer_setof8[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
			globalparams_t globalparams, 
			batch_type * _sourcestatsmarker,
			batch_type * _deststatsmarker,
			batch_type * _destoffset){
	analysis_type analysis_numllops = 1;
	analysis_type analysis_numsourcepartitions = 1;
	
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
	#ifdef _WIDEWORD
	for(unsigned int k=0; k<globalparams.kvstatssz; k++){ kvdram[BASEOFFSET_STATSDRAM_KVS + k].range(63, 32) = 0; }
	#else 
	for(unsigned int k=0; k<globalparams.kvstatssz; k++){ kvdram[BASEOFFSET_STATSDRAM_KVS + k].data[0].value = 0; }
	#endif 
	
	config_t config;
	sweepparams_t sweepparams;
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
			travstate_t travstate = gettravstate(ON, kvdram, globalparams, currentLOP, sourcestatsmarker, moretravstates);
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
				collectstats(config.enablecollectglobalstats, kvdram, buffer_setof2, buffer_setof4, globalstatsbuffer, config, globalparams, sweepparams, ctravstate);
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
			partitionpipeline(
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
	
	// *_globalparams = globalparams;
	*_sourcestatsmarker = sourcestatsmarker;
	*_deststatsmarker = deststatsmarker;
	*_destoffset = destoffset;
	return;
}

travstate_t
	#ifdef SW 
	acts::
	#endif 
reduceit_sync(uint512_dt * vdram, uint512_dt * kvdram0, travstate_t actvvstravstate, globalparams_t globalparams[NUMSUBCPUTHREADS]){
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
	static keyvalue_t tempverticesbuffer0[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = tempverticesbuffer0
	
	static keyvalue_t destvbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = destvbuffer
	static keyvalue_t destvbuffer2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE]; // unused.
	#pragma HLS array_partition variable = destvbuffer2
	
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
	
	unsigned int itercount_actvvs0 = 0;
	bool_type writeen_actvvs0 = ON;
	unsigned int itercount_actvvs1 = 0;
	bool_type writeen_actvvs1 = ON;
	unsigned int itercount = 0;
	bool_type pp0writeen = ON;
	
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
	bool_type nonzeroactvvsreturnedpp0 = ON;
	bool_type nonzeroactvvsreturnedpp1 = ON;
	
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
	
	batch_type sourceskipsize = 1;
	unsigned int llopsz = _globalparams.batch_range;
	for(unsigned int i=0; i<_globalparams.treedepth; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_treedepth avg=analysis_treedepth	
		llopsz = llopsz / NUM_PARTITIONS;
	}
	sweepparams0 = getsweepparams(_globalparams, currentLOP, 0);
	sweepparams1 = getsweepparams(_globalparams, currentLOP, 0);
	
	MAIN_LOOP: for(batch_type source_partition=0; source_partition<num_source_partitions + 1; source_partition+=_globalparams.loadfactorforreduce){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_sourceploop avg=analysis_sourceploop
		#ifdef _DEBUGMODE_KERNELPRINTS2
		actsutilityobj->print3("### dispatch::reduce:: source_partition", "sourcepartitionblock", "currentLOP", source_partition, sourcepartitionblock, currentLOP); 							
		#endif
		
		bool_type enablereduce = ON;
		bool_type enableflush = OFF;
		
		if(source_partition >= num_source_partitions){ enableflush = ON; } else { enableflush = OFF; }
		if(enableflush == OFF){
 
		rtravstate[0] = gettravstate(ON, kvdram0, globalparams[0], currentLOP, sourcestatsmarker, moretravstates[0]);
 }
		ntravszs = 0;
		for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ for(batch_type k=0; k<_globalparams.loadfactorforreduce; k++){ ntravszs += moretravstates[i][k].value; }}
		if((ntravszs == 0) || (source_partition >= num_source_partitions)){ enablereduce = OFF; } else { enablereduce = ON; }
		
		enablereducepp0 = enablereduce;
		enablereducepp1 = enablereduce;
		enablereducepp1_flush = enablereduce;
		
		if(enablereduce == ON || enableflush == ON){
			readkeyvalues(enablereduce, vdram, sourcevbuffer, (_globalparams.baseoffset_verticesdata_kvs + (sourcepartitionblock * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE); // reducechunksz_kvs? CRITICAL AUTOMATEME.
			resetmanykeyandvalues(destvbuffer, PADDEDDESTBUFFER_SIZE, 0); // c++ sw issues
			
			MAIN_LOOP2: for(batch_type index=0; index<_globalparams.loadfactorforreduce + 1; index+=NUMREDUCEUPDATESPIPELINES){

				if(index >= _globalparams.loadfactorforreduce){ enablereducepp0 = OFF; enablereducepp1 = OFF; }
				if(index == 0){ enablereducepp1_flush = OFF; } else { enablereducepp1_flush = ON; }
				
				// proc vars 0
				indexpp0 = index;
				spartition0 = source_partition + indexpp0;
				// sweepparams0 = getsweepparams(_globalparams, currentLOP, spartition0);
				sweepparams0 = updatesweepparams(sweepparams0, currentLOP, spartition0, sourceskipsize, llopsz, _globalparams);
				// rtravstate[0] = gettravstate(enablereducepp0, kvdram0, globalparams[0], currentLOP, sourcestatsmarker + indexpp0, moretravstates[0]);
				rtravstate[0] = updatetravstate(enablereducepp0, moretravstates[0][indexpp0], currentLOP, _globalparams);
				
				// reduce 0
				reducepartition(
					enablereducepp0, 
					sourcevbuffer,
kvdram0,tempverticesbuffer0,					indexpp0, reducesubchunksz, rtravstate, sweepparams0, _globalparams);
				#if defined(RUP1) && defined(INMEMORYGP) && defined(BFS_ALGORITHM)
				// collect & save actvvs 1
				nonzeroactvvsreturnedpp1 = collectactvvs_bfs(vdram, enablereducepp1_flush, destvbuffer, destvbuffer2, 
itercount_actvvs0, itercount_actvvs1,  
					enableflush, indexpp1, &actvvstravstate, 
					(indexpp1 * reducesubchunksz), reducesubchunksz, sweepparams1, _globalparams);
				if(nonzeroactvvsreturnedpp1 == ON || (enableflush == ON && indexpp1 < 8)){  itercount_actvvs0 += 1;  itercount_actvvs1 += 1;  }
				#endif
				
				// unify 0
				#if defined(INMEMORYGP) && defined(BFS_ALGORITHM)
				unifydata_bfs_parallelsyn(enablereducepp0, tempverticesbuffer0, destvbuffer, 
						indexpp0 * reducesubchunksz, reducesubchunksz, _globalparams);
				#endif
				#if defined(RUP1)
				// proc vars 1
				indexpp1 = index + 1;
				spartition1 = source_partition + indexpp1;
				// sweepparams1 = getsweepparams(_globalparams, currentLOP, spartition1);
				sweepparams1 = updatesweepparams(sweepparams1, currentLOP, spartition1, sourceskipsize, llopsz, _globalparams);
				// rtravstate[0] = gettravstate(enablereducepp1, kvdram0, globalparams[0], currentLOP, sourcestatsmarker + indexpp1, moretravstates[0]);
				rtravstate[0] = updatetravstate(enablereducepp1, moretravstates[0][indexpp1], currentLOP, _globalparams);
				#endif 
				
				// collect & save actvvs 0
				#if defined(INMEMORYGP) && defined(BFS_ALGORITHM)
				nonzeroactvvsreturnedpp0 = collectactvvs_bfs(vdram, enablereducepp0, destvbuffer, destvbuffer2,
itercount_actvvs0, itercount_actvvs1,  
					enableflush, indexpp0, &actvvstravstate, 
					(indexpp0 * reducesubchunksz), reducesubchunksz, sweepparams0, _globalparams);
				if(nonzeroactvvsreturnedpp0 == ON || (enableflush == ON && indexpp0 < 8)){  itercount_actvvs0 += 1;  itercount_actvvs1 += 1;  }
				#endif
				#if defined(RUP1)
				// reduce 1
				reducepartition(
					enablereducepp1,
					sourcevbuffer,
kvdram0,tempverticesbuffer0,					indexpp1, reducesubchunksz, rtravstate, sweepparams1, _globalparams);
				#endif 
				
				// unify 1
				#if defined(RUP1) && defined(INMEMORYGP) && defined(BFS_ALGORITHM)
				unifydata_bfs_parallelsyn(enablereducepp1, tempverticesbuffer0, destvbuffer, 
						indexpp1 * reducesubchunksz, reducesubchunksz, _globalparams);
				#endif
			}
			
			savekeyvalues(enablereduce, vdram, destvbuffer2, (_globalparams.baseoffset_verticesdata_kvs + (sourcepartitionblock * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE); // reducechunksz_kvs? AUTOMATEME.
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
	cout<<"reduceit_sync: number of active vertices for iteration "<<_globalparams.GraphIter + 1<<": (including INVALIDDATAs): "<<actvvstravstate.i<<endl;
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
topkernel(uint512_dt * vdram ,uint512_dt * kvdram0){ 
	
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem0
 
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem1
		
#pragma HLS INTERFACE s_axilite port = vdram bundle = control
 
#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATA_PACK variable = vdram
 
#pragma HLS DATA_PACK variable = kvdram0

	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printparameters();
	#endif 
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	#ifdef _WIDEWORD
  
	cout<<">>> Light weight ACTS 0 (_L2) Launched... size: "<<(unsigned int)(kvdram0[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].range(31, 0))<<endl; 
	#else
  
	cout<<">>> Light weight ACTS 0 (_L2) Launched... size: "<<kvdram0[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key<<endl; 
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
	
	globalparams_t globalparams[NUMSUBCPUTHREADS];
	#pragma HLS ARRAY_PARTITION variable=globalparams complete
	travstate_t actvvstravstate[NUMSUBCPUTHREADS];
	#pragma HLS ARRAY_PARTITION variable=actvvstravstate complete
	
 
	globalparams[0] = getglobalparams(kvdram0);
	
	unsigned int numGraphIters = globalparams[0].GraphIter;
	
	#ifdef _DEBUGMODE_STATS
	stats_greaterthan512 = 0;
	stats_lessthan512 = 0;
	stats_totals = 0;
	#endif 
	
	RUNITERATIONS_LOOP: for(unsigned int GraphIter=0; GraphIter<numGraphIters; GraphIter++){
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<">>> Light weight ACTS: Graph Iteration: "<<GraphIter<<endl;
		#endif
		
		for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ globalparams[i].GraphIter = GraphIter; }
		
		#ifdef DISPATCHTYPE0
 // COMPUTEUNITS_seq, 1_seq
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<">>> Light weight ACTS (DISPATCHTYPE0): Processing, partitioning & reducing instance 0 "<<endl;
		#endif
		start(kvdram0, globalparams[0]);
		#endif
		
		#ifdef DISPATCHTYPE1
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<">>> Light weight ACTS (DISPATCHTYPE1): Processing, partitioning & reducing... "<<endl;
		#endif
		start(vdram, kvdram0, globalparams);
		#endif 

		#ifdef DISPATCHTYPE2
		batch_type sourcestatsmarker0 = 0;
		batch_type deststatsmarker0 = 1;
		batch_type destoffset0 = 0;
		if((GraphIter < numGraphIters-1) || (globalparams[0].processcommand == ON)){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> Light weight ACTS (DISPATCHTYPE2): Processing Instances 0-0 "<<endl;
			#endif
			processit_sync(
				vdram,
kvdram0,buffer20,		
buffer30,	
				#ifndef COMPACTEDGES	
buffer40,	
				#endif 
				globalparams, &sourcestatsmarker0, &deststatsmarker0, &destoffset0);
		
			sourcestatsmarker0 = sourcestatsmarker0; deststatsmarker0 = deststatsmarker0; destoffset0 = destoffset0;
		}
		if((GraphIter < numGraphIters-1) || (globalparams[0].partitioncommand == ON)){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> Light weight ACTS (DISPATCHTYPE2): Partitioning Instance: ";
			#endif
 
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"0 ";
			#endif
			partitionit2(
				kvdram0,
				buffer10, 
				buffer20, 
				buffer30, 
				buffer40, 
				buffer50,
				globalparams[0], &sourcestatsmarker0, &deststatsmarker0, &destoffset0);
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<endl;
			#endif 
		}
		if((GraphIter < numGraphIters-1) || (globalparams[0].reducecommand == ON)){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> Light weight ACTS (DISPATCHTYPE2): Reducing Instances 0-0 "<<endl;
			#endif
			#ifdef REDUCEUPDATES
			actvvstravstate[0] = 
				reduceit_sync(
					vdram,
kvdram0,		
					actvvstravstate[0], globalparams);	
			#endif
		}
		#ifdef LOGKERNELSTATS
		#ifdef _WIDEWORD
		vdram[BASEOFFSET_MESSAGESDRAM_KVS + globalparams[0].baseoffset_returnvalues + GraphIter + 1].range(31, 0) = actvvstravstate[0].i;
		#else 
		vdram[BASEOFFSET_MESSAGESDRAM_KVS + globalparams[0].baseoffset_returnvalues + GraphIter + 1].data[0].key = actvvstravstate[0].i;	
		#endif 
		#endif
		#endif

		for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){
			globalparams[i].actvvsize = actvvstravstate[0].i;
		}
		if(actvvstravstate[0].i == 0){ 
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"Light weight ACTS finished. No more vertices to process Iteration. exiting... "<<endl;
			#endif 
			break; 
		}
	}
	
	#ifdef _DEBUGMODE_STATS
	cout<<"topkernel:: stats_greaterthan512: "<<stats_greaterthan512<<endl;
	cout<<"topkernel:: stats_lessthan512: "<<stats_lessthan512<<endl;
	cout<<"topkernel:: stats_totals: "<<stats_totals<<endl;
	cout<<"topkernel:: total: "<<(stats_lessthan512 + stats_greaterthan512)<<endl;
	#endif
	return;
}
}




