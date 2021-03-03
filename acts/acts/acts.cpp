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

#define NUMREDUCEUPDATESPIPELINES 2 
#if NUMREDUCEUPDATESPIPELINES==1
#define RUP0
#endif 
#if NUMREDUCEUPDATESPIPELINES==2
#define RUP0
#define RUP1
#endif

#define NUMREDUCEPARTITIONPIPELINES 2 
#if NUMREDUCEPARTITIONPIPELINES==1
#define RPP0
#endif 
#if NUMREDUCEPARTITIONPIPELINES==2
#define RPP0
#define RPP1
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

// functions (basic)
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

// functions (allignment)
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

// functions (converters)
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
uint32_type 
	#ifdef SW 
	acts::
	#endif 
convertvmasktouint32(uintNUMPby2_type vmask[BLOCKRAM_SIZE], unsigned int index){
	uint32_type res;
	#ifdef _WIDEWORD
	/* 	res.range(1, 0) = vmask[index].data[0].key;
	res.range(2, 1) = vmask[index].data[0].value;
	res.range(3, 2) = vmask[index].data[1].key;
	res.range(4, 3) = vmask[index].data[1].value;
	res.range(5, 4) = vmask[index].data[2].key;
	res.range(6, 5) = vmask[index].data[2].value;
	res.range(7, 6) = vmask[index].data[3].key;
	res.range(8, 7) = vmask[index].data[3].value;
	res.range(9, 8) = vmask[index].data[4].key;
	res.range(10, 9) = vmask[index].data[4].value;
	res.range(11, 10) = vmask[index].data[5].key;
	res.range(12, 11) = vmask[index].data[5].value;
	res.range(13, 12) = vmask[index].data[6].key;
	res.range(14, 13) = vmask[index].data[6].value;
	res.range(15, 14) = vmask[index].data[7].key;
	res.range(16, 15) = vmask[index].data[7].value;
	res.range(17, 16) = vmask[index].data[8].key;
	res.range(18, 17) = vmask[index].data[8].value;
	res.range(19, 18) = vmask[index].data[9].key;
	res.range(20, 19) = vmask[index].data[9].value;
	res.range(21, 20) = vmask[index].data[10].key;
	res.range(22, 21) = vmask[index].data[10].value;
	res.range(23, 22) = vmask[index].data[11].key;
	res.range(24, 23) = vmask[index].data[11].value;
	res.range(25, 24) = vmask[index].data[12].key;
	res.range(26, 25) = vmask[index].data[12].value;
	res.range(27, 26) = vmask[index].data[13].key;
	res.range(28, 27) = vmask[index].data[13].value;
	res.range(29, 28) = vmask[index].data[14].key;
	res.range(30, 29) = vmask[index].data[14].value;
	res.range(31, 30) = vmask[index].data[15].key;
	res.range(32, 31) = vmask[index].data[15].value;
 */
	res.range(0, 0) = vmask[index].data[0].key;
	res.range(1, 1) = vmask[index].data[0].value;
	res.range(2, 2) = vmask[index].data[1].key;
	res.range(3, 3) = vmask[index].data[1].value;
	res.range(4, 4) = vmask[index].data[2].key;
	res.range(5, 5) = vmask[index].data[2].value;
	res.range(6, 6) = vmask[index].data[3].key;
	res.range(7, 7) = vmask[index].data[3].value;
	res.range(8, 8) = vmask[index].data[4].key;
	res.range(9, 9) = vmask[index].data[4].value;
	res.range(10, 10) = vmask[index].data[5].key;
	res.range(11, 11) = vmask[index].data[5].value;
	res.range(12, 12) = vmask[index].data[6].key;
	res.range(13, 13) = vmask[index].data[6].value;
	res.range(14, 14) = vmask[index].data[7].key;
	res.range(15, 15) = vmask[index].data[7].value;
	res.range(16, 16) = vmask[index].data[8].key;
	res.range(17, 17) = vmask[index].data[8].value;
	res.range(18, 18) = vmask[index].data[9].key;
	res.range(19, 19) = vmask[index].data[9].value;
	res.range(20, 20) = vmask[index].data[10].key;
	res.range(21, 21) = vmask[index].data[10].value;
	res.range(22, 22) = vmask[index].data[11].key;
	res.range(23, 23) = vmask[index].data[11].value;
	res.range(24, 24) = vmask[index].data[12].key;
	res.range(25, 25) = vmask[index].data[12].value;
	res.range(26, 26) = vmask[index].data[13].key;
	res.range(27, 27) = vmask[index].data[13].value;
	res.range(28, 28) = vmask[index].data[14].key;
	res.range(29, 29) = vmask[index].data[14].value;
	res.range(30, 30) = vmask[index].data[15].key;
	res.range(31, 31) = vmask[index].data[15].value;
	#else 
	WRITETO_UINT(&res, 0, 1, vmask[index].data[0].key);
	WRITETO_UINT(&res, 1, 1, vmask[index].data[0].value);
	WRITETO_UINT(&res, 2, 1, vmask[index].data[1].key);
	WRITETO_UINT(&res, 3, 1, vmask[index].data[1].value);
	WRITETO_UINT(&res, 4, 1, vmask[index].data[2].key);
	WRITETO_UINT(&res, 5, 1, vmask[index].data[2].value);
	WRITETO_UINT(&res, 6, 1, vmask[index].data[3].key);
	WRITETO_UINT(&res, 7, 1, vmask[index].data[3].value);
	WRITETO_UINT(&res, 8, 1, vmask[index].data[4].key);
	WRITETO_UINT(&res, 9, 1, vmask[index].data[4].value);
	WRITETO_UINT(&res, 10, 1, vmask[index].data[5].key);
	WRITETO_UINT(&res, 11, 1, vmask[index].data[5].value);
	WRITETO_UINT(&res, 12, 1, vmask[index].data[6].key);
	WRITETO_UINT(&res, 13, 1, vmask[index].data[6].value);
	WRITETO_UINT(&res, 14, 1, vmask[index].data[7].key);
	WRITETO_UINT(&res, 15, 1, vmask[index].data[7].value);
	WRITETO_UINT(&res, 16, 1, vmask[index].data[8].key);
	WRITETO_UINT(&res, 17, 1, vmask[index].data[8].value);
	WRITETO_UINT(&res, 18, 1, vmask[index].data[9].key);
	WRITETO_UINT(&res, 19, 1, vmask[index].data[9].value);
	WRITETO_UINT(&res, 20, 1, vmask[index].data[10].key);
	WRITETO_UINT(&res, 21, 1, vmask[index].data[10].value);
	WRITETO_UINT(&res, 22, 1, vmask[index].data[11].key);
	WRITETO_UINT(&res, 23, 1, vmask[index].data[11].value);
	WRITETO_UINT(&res, 24, 1, vmask[index].data[12].key);
	WRITETO_UINT(&res, 25, 1, vmask[index].data[12].value);
	WRITETO_UINT(&res, 26, 1, vmask[index].data[13].key);
	WRITETO_UINT(&res, 27, 1, vmask[index].data[13].value);
	WRITETO_UINT(&res, 28, 1, vmask[index].data[14].key);
	WRITETO_UINT(&res, 29, 1, vmask[index].data[14].value);
	WRITETO_UINT(&res, 30, 1, vmask[index].data[15].key);
	WRITETO_UINT(&res, 31, 1, vmask[index].data[15].value);
	#endif
	return res;
}

// functions (bit manipulation)
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

// functions (acts utilities)
batch_type
	#ifdef SW 
	acts::
	#endif
getskipsize(step_type currentLOP, bool_type sourceORdest, globalparams_t globalparams){
	analysis_type analysis_treedepth = TREE_DEPTH;
	batch_type result;
	
	if(currentLOP == 0){ currentLOP = 1; }
	if(sourceORdest == SOURCE){ result = globalparams.ACTSPARAMS_FINALNUMPARTITIONS; }
	else if (sourceORdest == DEST){ result = globalparams.ACTSPARAMS_FINALNUMPARTITIONS / NUM_PARTITIONS; } // FIXME. use TREE_DEPTH for less lut?
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
	unsigned int range = globalparams.SIZE_BATCHRANGE;
	for(unsigned int i=0; i<globalparams.ACTSPARAMS_TREEDEPTH; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loop1 avg=analysis_loop1	
		range = range / NUM_PARTITIONS;
	}
	return range;
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
		
		else if(realsize_kvs == BLOCKRAM_SIZE){ size_kvs = BLOCKRAM_SIZE; }
		
		else {
			cout<<"WARNING:getpartitionwritesz: should not get here. something might be wrong. realsize_kvs: "<<realsize_kvs<<", size_kvs: "<<size_kvs<<", BLOCKRAM_SIZE: "<<BLOCKRAM_SIZE<<endl;				 
			
			#ifdef ENABLE_PERFECTACCURACY
			#ifdef _DEBUGMODE_CHECKS2
			cout<<"WARNING:getpartitionwritesz: should not get here. something might be wrong. realsize_kvs: "<<realsize_kvs<<", size_kvs: "<<size_kvs<<", BLOCKRAM_SIZE: "<<BLOCKRAM_SIZE<<endl;				 
			exit(EXIT_FAILURE);
			#endif
			#else
			size_kvs = BLOCKRAM_SIZE / NUM_PARTITIONS;
			#endif 
		}
		#if defined(SW) || defined(SWEMU) || defined(HW) 
		if((bramoffset_kvs + size_kvs) >= BLOCKRAM_SIZE){ size_kvs = BLOCKRAM_SIZE - bramoffset_kvs - 1; } 
		#endif
	#else 
		size_kvs = realsize_kvs;
	#endif
	return size_kvs;
}
void 
	#ifdef SW 
	acts::
	#endif 
calculateoffsets(keyvalue_t * buffer, buffer_type size){
	unsigned int analysis_size = NUM_PARTITIONS;
	for(buffer_type i=1; i<size; i++){ 
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_size avg=analysis_size	
		buffer[i].key = allignhigher_KV(buffer[i-1].key + buffer[i-1].value); 
	}
	return;
}
void 
	#ifdef SW 
	acts::
	#endif 
calculatemanyunallignedoffsets(keyvalue_t buffer[VECTOR_SIZE][NUM_PARTITIONS], buffer_type size, batch_type base, batch_type skipspacing){
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
	globalparams.ENABLE_RUNKERNELCOMMAND = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_RUNKERNELCOMMAND].range(31, 0);
	globalparams.ENABLE_PROCESSCOMMAND = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_PROCESSCOMMAND].range(31, 0);
	globalparams.ENABLE_PARTITIONCOMMAND = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_PARTITIONCOMMAND].range(31, 0);
	globalparams.ENABLE_APPLYUPDATESCOMMAND = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_APPLYUPDATESCOMMAND].range(31, 0);

	globalparams.BASEOFFSETKVS_MESSAGESDATA = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_MESSAGESDATA].range(31, 0);
	globalparams.BASEOFFSETKVS_EDGESDATA = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_EDGESDATA].range(31, 0);
	globalparams.BASEOFFSETKVS_VERTEXPTR = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTEXPTR].range(31, 0);
	globalparams.BASEOFFSETKVS_VERTICESDATA = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESDATA].range(31, 0);
	globalparams.BASEOFFSETKVS_ACTIVEVERTICES = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_ACTIVEVERTICES].range(31, 0);
	globalparams.BASEOFFSETKVS_VERTICESDATAMASK = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESDATAMASK].range(31, 0);
	globalparams.BASEOFFSETKVS_STATSDRAM = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_STATSDRAM].range(31, 0);
	globalparams.BASEOFFSETKVS_KVDRAM = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_KVDRAM].range(31, 0);
	globalparams.BASEOFFSETKVS_KVDRAMWORKSPACE = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_KVDRAMWORKSPACE].range(31, 0);
	
	globalparams.SIZE_MESSAGESDRAM = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_MESSAGESDATA].range(31, 0);
	globalparams.SIZE_EDGES = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_EDGES].range(31, 0);
	globalparams.SIZE_VERTEXPTRS = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_VERTEXPTRS].range(31, 0);
	globalparams.SIZE_VERTICESDATA = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_VERTICESDATA].range(31, 0);
	globalparams.SIZE_ACTIVEVERTICES = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_ACTIVEVERTICES].range(31, 0);
	globalparams.SIZE_VERTICESDATAMASK = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_VERTICESDATAMASK].range(31, 0);
	globalparams.SIZE_KVSTATSDRAM = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_KVSTATSDRAM].range(31, 0);
	globalparams.SIZE_KVDRAM = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_KVDRAM].range(31, 0);
	globalparams.SIZE_KVDRAMWORKSPACE = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_KVDRAMWORKSPACE].range(31, 0);
	globalparams.SIZE_REDUCE = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_REDUCE].range(31, 0);
	globalparams.SIZE_BATCHRANGE = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_BATCHRANGE].range(31, 0);
	globalparams.SIZE_RUN = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN].range(31, 0);

	globalparams.POW_MESSAGESDRAM = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_MESSAGESDRAM].range(31, 0);
	globalparams.POW_EDGES = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_EDGES].range(31, 0);
	globalparams.POW_VERTEXPTRS = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_VERTEXPTRS].range(31, 0);
	globalparams.POW_VERTICESDATA = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_VERTICESDATA].range(31, 0);
	globalparams.POW_ACTIVEVERTICES = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_ACTIVEVERTICES].range(31, 0);
	globalparams.POW_VERTICESDATAMASK = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_VERTICESDATAMASK].range(31, 0);
	globalparams.POW_KVSTATSDRAM = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_KVSTATSDRAM].range(31, 0);
	globalparams.POW_KVDRAM = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_KVDRAM].range(31, 0);
	globalparams.POW_KVDRAMWORKSPACE = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_KVDRAMWORKSPACE].range(31, 0);
	globalparams.POW_REDUCE = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_REDUCE].range(31, 0);
	globalparams.POW_BATCHRANGE = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_BATCHRANGE].range(31, 0);
	
	globalparams.ALGORITHMINFO_GRAPHITERATIONID = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0);
	globalparams.ALGORITHMINFO_GRAPHALGORITHMID = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHALGORITHMID].range(31, 0);

	globalparams.ACTSPARAMS_BEGINLOP = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_BEGINLOP].range(31, 0);
	globalparams.ACTSPARAMS_NUMLOPS = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_NUMLOPS].range(31, 0);
	globalparams.ACTSPARAMS_TREEDEPTH = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_TREEDEPTH].range(31, 0);
	globalparams.ACTSPARAMS_FINALNUMPARTITIONS = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_FINALNUMPARTITIONS].range(31, 0);
	globalparams.ACTSPARAMS_SRCVOFFSET = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_SRCVOFFSET].range(31, 0);
	globalparams.ACTSPARAMS_SRCVSIZE = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_SRCVSIZE].range(31, 0);
	globalparams.ACTSPARAMS_DESTVOFFSET = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_DESTVOFFSET].range(31, 0);
	
	globalparams.RETURN_RETURNVALUES = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURN_RETURNVALUES].range(31, 0);
	#else 
	globalparams.ENABLE_RUNKERNELCOMMAND = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_RUNKERNELCOMMAND].data[0].key;
	globalparams.ENABLE_PROCESSCOMMAND = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_PROCESSCOMMAND].data[0].key;
	globalparams.ENABLE_PARTITIONCOMMAND = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_PARTITIONCOMMAND].data[0].key;
	globalparams.ENABLE_APPLYUPDATESCOMMAND = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_APPLYUPDATESCOMMAND].data[0].key;

	globalparams.BASEOFFSETKVS_MESSAGESDATA = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_MESSAGESDATA].data[0].key;
	globalparams.BASEOFFSETKVS_EDGESDATA = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_EDGESDATA].data[0].key;
	globalparams.BASEOFFSETKVS_VERTEXPTR = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTEXPTR].data[0].key;
	globalparams.BASEOFFSETKVS_VERTICESDATA = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESDATA].data[0].key;
	globalparams.BASEOFFSETKVS_ACTIVEVERTICES = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_ACTIVEVERTICES].data[0].key;
	globalparams.BASEOFFSETKVS_VERTICESDATAMASK = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESDATAMASK].data[0].key;
	globalparams.BASEOFFSETKVS_STATSDRAM = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_STATSDRAM].data[0].key;
	globalparams.BASEOFFSETKVS_KVDRAM = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_KVDRAM].data[0].key;
	globalparams.BASEOFFSETKVS_KVDRAMWORKSPACE = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_KVDRAMWORKSPACE].data[0].key;
	
	globalparams.SIZE_MESSAGESDRAM = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_MESSAGESDATA].data[0].key;
	globalparams.SIZE_EDGES = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_EDGES].data[0].key;
	globalparams.SIZE_VERTEXPTRS = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_VERTEXPTRS].data[0].key;
	globalparams.SIZE_VERTICESDATA = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_VERTICESDATA].data[0].key;
	globalparams.SIZE_ACTIVEVERTICES = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_ACTIVEVERTICES].data[0].key;
	globalparams.SIZE_VERTICESDATAMASK = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_VERTICESDATAMASK].data[0].key;
	globalparams.SIZE_KVSTATSDRAM = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_KVSTATSDRAM].data[0].key;
	globalparams.SIZE_KVDRAM = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_KVDRAM].data[0].key;
	globalparams.SIZE_KVDRAMWORKSPACE = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_KVDRAMWORKSPACE].data[0].key;
	globalparams.SIZE_REDUCE = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_REDUCE].data[0].key;
	globalparams.SIZE_BATCHRANGE = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_BATCHRANGE].data[0].key;
	globalparams.SIZE_RUN = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN].data[0].key;

	globalparams.POW_MESSAGESDRAM = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_MESSAGESDRAM].data[0].key;
	globalparams.POW_EDGES = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_EDGES].data[0].key;
	globalparams.POW_VERTEXPTRS = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_VERTEXPTRS].data[0].key;
	globalparams.POW_VERTICESDATA = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_VERTICESDATA].data[0].key;
	globalparams.POW_ACTIVEVERTICES = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_ACTIVEVERTICES].data[0].key;
	globalparams.POW_VERTICESDATAMASK = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_VERTICESDATAMASK].data[0].key;
	globalparams.POW_KVSTATSDRAM = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_KVSTATSDRAM].data[0].key;
	globalparams.POW_KVDRAM = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_KVDRAM].data[0].key;
	globalparams.POW_KVDRAMWORKSPACE = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_KVDRAMWORKSPACE].data[0].key;
	globalparams.POW_REDUCE = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_REDUCE].data[0].key;
	globalparams.POW_BATCHRANGE = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_BATCHRANGE].data[0].key;
	
	globalparams.ALGORITHMINFO_GRAPHITERATIONID = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key;
	globalparams.ALGORITHMINFO_GRAPHALGORITHMID = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHALGORITHMID].data[0].key;

	globalparams.ACTSPARAMS_BEGINLOP = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_BEGINLOP].data[0].key;
	globalparams.ACTSPARAMS_NUMLOPS = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_NUMLOPS].data[0].key;
	globalparams.ACTSPARAMS_TREEDEPTH = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_TREEDEPTH].data[0].key;
	globalparams.ACTSPARAMS_FINALNUMPARTITIONS = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_FINALNUMPARTITIONS].data[0].key;
	globalparams.ACTSPARAMS_SRCVOFFSET = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_SRCVOFFSET].data[0].key;
	globalparams.ACTSPARAMS_SRCVSIZE = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_SRCVSIZE].data[0].key;
	globalparams.ACTSPARAMS_DESTVOFFSET = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_DESTVOFFSET].data[0].key;
	
	globalparams.RETURN_RETURNVALUES = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURN_RETURNVALUES].data[0].key;
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

	if(currentLOP == 0 || (currentLOP % 2) == 1){ sweepparams.worksourcebaseaddress_kvs = globalparams.BASEOFFSETKVS_KVDRAM; sweepparams.workdestbaseaddress_kvs = globalparams.BASEOFFSETKVS_KVDRAMWORKSPACE; }
	else { sweepparams.worksourcebaseaddress_kvs = globalparams.BASEOFFSETKVS_KVDRAMWORKSPACE; sweepparams.workdestbaseaddress_kvs = globalparams.BASEOFFSETKVS_KVDRAM; }

	unsigned int div = globalparams.SIZE_BATCHRANGE;
	analysis_type analysis_lc = TREE_DEPTH;
	for(unsigned int i=0; i<globalparams.ACTSPARAMS_TREEDEPTH; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_lc avg=analysis_lc	
		div = div / NUM_PARTITIONS;
	}
	sweepparams.upperlimit = globalparams.ACTSPARAMS_DESTVOFFSET + (source_partition * sourceskipsize * div); // POW_BATCHRANGE
	sweepparams.upperpartition = (sweepparams.upperlimit - globalparams.ACTSPARAMS_DESTVOFFSET) >> (globalparams.POW_BATCHRANGE - (NUM_PARTITIONS_POW * currentLOP)); //

	sweepparams.source_partition = source_partition;
	return sweepparams;
}
travstate_t 
	#ifdef SW 
	acts::
	#endif 
gettravstate(bool_type enable, uint512_dt * kvdram, globalparams_t globalparams, step_type currentLOP, batch_type sourcestatsmarker){			
	travstate_t travstate;
	if(enable == OFF){ return travstate; }
	keyvalue_t keyvalue;
	keyvalue_t nextkeyvalue;
	
	if(currentLOP == 0){ keyvalue.key = 0; }
	else if(currentLOP == 1){ keyvalue.key = 0; }
	else { 
		#ifdef _WIDEWORD
		keyvalue.key = kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker].range(31, 0); 
		keyvalue.value = kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker].range(63, 32);
		#else
		keyvalue = kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker].data[0]; 
		#endif
	}
	
	if(currentLOP == 0){ nextkeyvalue.key = globalparams.SIZE_RUN; }
	else if(currentLOP == 1){ nextkeyvalue.key = globalparams.SIZE_RUN; }
	else { nextkeyvalue.key = keyvalue.key + keyvalue.value; }
		
	travstate.begin_kvs = keyvalue.key / VECTOR_SIZE; 
	travstate.end_kvs = (nextkeyvalue.key + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	
	travstate.size_kvs = travstate.end_kvs - travstate.begin_kvs;
	travstate.skip_kvs = SRCBUFFER_SIZE;
	travstate.i_kvs = travstate.begin_kvs;
	return travstate;	
}
partition_type
	#ifdef SW 
	acts::
	#endif 
getpartition(bool_type enable, keyvalue_t keyvalue, step_type currentLOP, vertex_t upperlimit, unsigned int upperpartition, unsigned int batch_range_pow){
	partition_type partition;
	if(enable == ON){ 
		if(keyvalue.value == INVALIDDATA){ partition = keyvalue.key; } 
		else { partition = ((keyvalue.key - upperlimit) >> (batch_range_pow - (NUM_PARTITIONS_POW * currentLOP))); }
	} else { partition = 0; }
	
	#ifdef _DEBUGMODE_CHECKS2
	if(partition >= NUM_PARTITIONS){ actsutilityobj->globalstats_counterrorsingetpartition(1); }
	#endif 
	
	#ifdef _DEBUGMODE_CHECKS2
	if(partition >= NUM_PARTITIONS){ 
		#ifdef ENABLE_VOICEOUTKERNELERRORS
		cout<<"acts::getpartition::ERROR 1. partition out of bounds partition: "<<partition<<", keyvalue.key: "<<keyvalue.key<<", keyvalue.value: "<<keyvalue.value<<", NUM_PARTITIONS: "<<NUM_PARTITIONS<<", upperlimit: "<<upperlimit<<", currentLOP: "<<currentLOP<<", batch_range_pow: "<<batch_range_pow<<", div factor: "<<(1 << (batch_range_pow - (NUM_PARTITIONS_POW * currentLOP)))<<endl; 
		#endif 
		#ifdef ENABLE_PERFECTACCURACY
		exit(EXIT_FAILURE); 
		#endif 
	}
	#endif
	
	// #ifdef ENABLE_PERFECTACCURACY
		// #ifdef _DEBUGMODE_CHECKS2
		// if(partition >= NUM_PARTITIONS){ cout<<"acts::getpartition::ERROR 1. partition out of bounds partition: "<<partition<<", keyvalue.key: "<<keyvalue.key<<", keyvalue.value: "<<keyvalue.value<<", NUM_PARTITIONS: "<<NUM_PARTITIONS<<", upperlimit: "<<upperlimit<<", currentLOP: "<<currentLOP<<", batch_range_pow: "<<batch_range_pow<<", div factor: "<<(1 << (batch_range_pow - (NUM_PARTITIONS_POW * currentLOP)))<<endl; exit(EXIT_FAILURE); }
		// #endif
	// #endif 
	#ifndef ENABLE_PERFECTACCURACY
		if(partition >= NUM_PARTITIONS){ partition = (((1 << NUM_PARTITIONS_POW) - 1) & (partition >> (1 - 1))); } // FIXME. REMOVEME. PERFECTIONTEST.
	#endif
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("acts::getpartition 2", partition, NUM_PARTITIONS, keyvalue.key, upperlimit, currentLOP);
	#endif
	return partition;
}

// functions (resets)
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
resetkeysandvalues(keyvalue_t buffer[VECTOR_SIZE][NUM_PARTITIONS], buffer_type size, unsigned int resetval){
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
resetkeysandvalues(keyvalue_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], buffer_type size, unsigned int resetval){
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
resetkeysandvalues(keyvalue_t * buffer, buffer_type size, unsigned int resetval){
	for(buffer_type i=0; i<size; i++){
		buffer[i].key = resetval; buffer[i].value = resetval; 
	}
	return;
}
void 
	#ifdef SW 
	acts::
	#endif 
resetvmask(uintNUMPby2_type vmask[BLOCKRAM_SIZE]){
	for(buffer_type i=0; i<BLOCKRAM_SIZE; i++){ 
		// for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		// #pragma HLS UNROLL
			// vmask[i].data[p].key = 0;
			// vmask[i].data[p].value = 0;
		// }	
		vmask[i].data[0].key = 0;
		vmask[i].data[0].value = 0;
		vmask[i].data[1].key = 0;
		vmask[i].data[1].value = 0;
		vmask[i].data[2].key = 0;
		vmask[i].data[2].value = 0;
		vmask[i].data[3].key = 0;
		vmask[i].data[3].value = 0;
		vmask[i].data[4].key = 0;
		vmask[i].data[4].value = 0;
		vmask[i].data[5].key = 0;
		vmask[i].data[5].value = 0;
		vmask[i].data[6].key = 0;
		vmask[i].data[6].value = 0;
		vmask[i].data[7].key = 0;
		vmask[i].data[7].value = 0;
		vmask[i].data[8].key = 0;
		vmask[i].data[8].value = 0;
		vmask[i].data[9].key = 0;
		vmask[i].data[9].value = 0;
		vmask[i].data[10].key = 0;
		vmask[i].data[10].value = 0;
		vmask[i].data[11].key = 0;
		vmask[i].data[11].value = 0;
		vmask[i].data[12].key = 0;
		vmask[i].data[12].value = 0;
		vmask[i].data[13].key = 0;
		vmask[i].data[13].value = 0;
		vmask[i].data[14].key = 0;
		vmask[i].data[14].value = 0;
		vmask[i].data[15].key = 0;
		vmask[i].data[15].value = 0;
		
	}
	return;
}
void
	#ifdef SW 
	acts::
	#endif 
resetkvdramstats(uint512_dt * kvdram, globalparams_t globalparams){
	unsigned int totalnumpartitionsb4last = 0;
	RESETKVSTATS_LOOP1: for(unsigned int k=0; k<TREE_DEPTH; k++){ totalnumpartitionsb4last += (1 << (NUM_PARTITIONS_POW * k)); }
	for(unsigned int k=0; k<totalnumpartitionsb4last; k++){
		#ifdef _WIDEWORD
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].range(63, 32) = 0; 
		#else
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].data[0].value = 0; 
		#endif 
	}
	return;
}

// functions (accessors)
void 
	#ifdef SW 
	acts::
	#endif 
readkeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs, travstate_t travstate, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE;
	buffer_type chunk_size = getchunksize_kvs(size_kvs, travstate, 0);
	
	READKEYVALUES1_LOOP: for (buffer_type i=0; i<chunk_size; i++){
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
savekeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t * globalcapsule, keyvalue_t localcapsule[NUM_PARTITIONS], batch_type globalbaseaddress_kvs, globalparams_t globalparams){				
	if(enable == OFF){ return; }

	#ifdef _DEBUGMODE_CHECKS
	actsutilityobj->printkeyvalues("savekeyvalues::localcapsule", localcapsule, NUM_PARTITIONS);
	actsutilityobj->printvaluecount("savekeyvalues::localcapsule", localcapsule, NUM_PARTITIONS);
	actsutilityobj->scankeyvalues("savekeyvalues::buffer", (keyvalue_t *)buffer, localcapsule, NUM_PARTITIONS, globalparams.SIZE_BATCHRANGE / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	
	analysis_type analysis_destpartitionsz = BLOCKRAM_SIZE / NUM_PARTITIONS;
	SAVEPARTITIONS_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		batch_type dramoffset_kvs = globalbaseaddress_kvs + ((globalcapsule[p].key + globalcapsule[p].value) / VECTOR_SIZE);
		buffer_type bramoffset_kvs = localcapsule[p].key / VECTOR_SIZE;
		buffer_type realsize_kvs = localcapsule[p].value / VECTOR_SIZE;
		buffer_type size_kvs = getpartitionwritesz(realsize_kvs, bramoffset_kvs);
		
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("savekeyvalues 23", bramoffset_kvs + size_kvs, BLOCKRAM_SIZE + 1, p, NAp, NAp);
		actsutilityobj->checkoutofbounds("savekeyvalues 25", ((globalcapsule[p].key + globalcapsule[p].value) / VECTOR_SIZE), KVDRAMSZ + 1, p, NAp, NAp);
		#endif
		if(realsize_kvs > 0){ // CRITICAL NEWCHANGE.
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
	}
	SAVEPARTITIONS_LOOP2: for(partition_type p=0; p<NUM_PARTITIONS; p++){ globalcapsule[p].value += localcapsule[p].value; }
	
	#if defined(ENABLE_PERFECTACCURACY) && defined(_DEBUGMODE_CHECKS2)
	for(unsigned int i=0; i<NUM_PARTITIONS-1; i++){ 
		if(globalcapsule[i].key + globalcapsule[i].value >= globalcapsule[i+1].key && globalcapsule[i].value > 0){ 
			cout<<"savekeyvalues::globalcapsule 33. ERROR. out of bounds. (globalcapsule["<<i<<"].key("<<globalcapsule[i].key<<") + globalcapsule["<<i<<"].value("<<globalcapsule[i].value<<") >= globalcapsule["<<i+1<<"].key("<<globalcapsule[i+1].key<<")) printing and exiting..."<<endl; 
			actsutilityobj->printkeyvalues("savekeyvalues::globalcapsule 34", globalcapsule, NUM_PARTITIONS); 
			exit(EXIT_FAILURE); 
		}
	}
	if(globalcapsule[NUM_PARTITIONS-1].key + globalcapsule[NUM_PARTITIONS-1].value >= globalparams.SIZE_KVDRAM){
		cout<<"savekeyvalues::globalcapsule 36. ERROR. out of bounds. (globalcapsule["<<NUM_PARTITIONS-1<<"].key("<<globalcapsule[NUM_PARTITIONS-1].key<<") + globalcapsule["<<NUM_PARTITIONS-1<<"].value("<<globalcapsule[NUM_PARTITIONS-1].value<<") >= globalparams.SIZE_KVDRAM("<<globalparams.SIZE_KVDRAM<<")) printing and exiting..."<<endl; 
		actsutilityobj->printkeyvalues("savekeyvalues::globalcapsule 37", globalcapsule, NUM_PARTITIONS); 
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
readkeyvalues(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
		
	READKEYVALUES2_LOOP: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		buffer[0][bufferoffset_kvs + i].key = kvdram[dramoffset_kvs + i].range(31, 0); 
		buffer[0][bufferoffset_kvs + i].value = kvdram[dramoffset_kvs + i].range(63, 32); 
		buffer[1][bufferoffset_kvs + i].key = kvdram[dramoffset_kvs + i].range(95, 64); 
		buffer[1][bufferoffset_kvs + i].value = kvdram[dramoffset_kvs + i].range(127, 96); 
		buffer[2][bufferoffset_kvs + i].key = kvdram[dramoffset_kvs + i].range(159, 128); 
		buffer[2][bufferoffset_kvs + i].value = kvdram[dramoffset_kvs + i].range(191, 160); 
		buffer[3][bufferoffset_kvs + i].key = kvdram[dramoffset_kvs + i].range(223, 192); 
		buffer[3][bufferoffset_kvs + i].value = kvdram[dramoffset_kvs + i].range(255, 224); 
		buffer[4][bufferoffset_kvs + i].key = kvdram[dramoffset_kvs + i].range(287, 256); 
		buffer[4][bufferoffset_kvs + i].value = kvdram[dramoffset_kvs + i].range(319, 288); 
		buffer[5][bufferoffset_kvs + i].key = kvdram[dramoffset_kvs + i].range(351, 320); 
		buffer[5][bufferoffset_kvs + i].value = kvdram[dramoffset_kvs + i].range(383, 352); 
		buffer[6][bufferoffset_kvs + i].key = kvdram[dramoffset_kvs + i].range(415, 384); 
		buffer[6][bufferoffset_kvs + i].value = kvdram[dramoffset_kvs + i].range(447, 416); 
		buffer[7][bufferoffset_kvs + i].key = kvdram[dramoffset_kvs + i].range(479, 448); 
		buffer[7][bufferoffset_kvs + i].value = kvdram[dramoffset_kvs + i].range(511, 480); 
		#else 
		buffer[0][bufferoffset_kvs + i] = kvdram[dramoffset_kvs + i].data[0]; 
		buffer[1][bufferoffset_kvs + i] = kvdram[dramoffset_kvs + i].data[1]; 
		buffer[2][bufferoffset_kvs + i] = kvdram[dramoffset_kvs + i].data[2]; 
		buffer[3][bufferoffset_kvs + i] = kvdram[dramoffset_kvs + i].data[3]; 
		buffer[4][bufferoffset_kvs + i] = kvdram[dramoffset_kvs + i].data[4]; 
		buffer[5][bufferoffset_kvs + i] = kvdram[dramoffset_kvs + i].data[5]; 
		buffer[6][bufferoffset_kvs + i] = kvdram[dramoffset_kvs + i].data[6]; 
		buffer[7][bufferoffset_kvs + i] = kvdram[dramoffset_kvs + i].data[7]; 
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
	if(dramoffset_kvs >= globalparams.BASEOFFSETKVS_VERTICESDATA_ && dramoffset_kvs < globalparams.BASEOFFSETKVS_VERTICESDATA + VERTICESDATASZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"readkeyvalues:: vertices read: offset: "<<(dramoffset_kvs - globalparams.BASEOFFSETKVS_VERTICESDATA) * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs - globalparams.BASEOFFSETKVS_VERTICESDATA) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	#endif
	return;
}

void 
	#ifdef SW 
	acts::
	#endif 
savekeyvalues(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount =  APPLYVERTEXBUFFERSZ / 2;
	
	SAVEKEYVALUES2_LOOP: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		kvdram[dramoffset_kvs + i].range(31, 0) = buffer[0][bufferoffset_kvs + i].key; 
		kvdram[dramoffset_kvs + i].range(63, 32) = buffer[0][bufferoffset_kvs + i].value; 
		kvdram[dramoffset_kvs + i].range(95, 64) = buffer[1][bufferoffset_kvs + i].key; 
		kvdram[dramoffset_kvs + i].range(127, 96) = buffer[1][bufferoffset_kvs + i].value; 
		kvdram[dramoffset_kvs + i].range(159, 128) = buffer[2][bufferoffset_kvs + i].key; 
		kvdram[dramoffset_kvs + i].range(191, 160) = buffer[2][bufferoffset_kvs + i].value; 
		kvdram[dramoffset_kvs + i].range(223, 192) = buffer[3][bufferoffset_kvs + i].key; 
		kvdram[dramoffset_kvs + i].range(255, 224) = buffer[3][bufferoffset_kvs + i].value; 
		kvdram[dramoffset_kvs + i].range(287, 256) = buffer[4][bufferoffset_kvs + i].key; 
		kvdram[dramoffset_kvs + i].range(319, 288) = buffer[4][bufferoffset_kvs + i].value; 
		kvdram[dramoffset_kvs + i].range(351, 320) = buffer[5][bufferoffset_kvs + i].key; 
		kvdram[dramoffset_kvs + i].range(383, 352) = buffer[5][bufferoffset_kvs + i].value; 
		kvdram[dramoffset_kvs + i].range(415, 384) = buffer[6][bufferoffset_kvs + i].key; 
		kvdram[dramoffset_kvs + i].range(447, 416) = buffer[6][bufferoffset_kvs + i].value; 
		kvdram[dramoffset_kvs + i].range(479, 448) = buffer[7][bufferoffset_kvs + i].key; 
		kvdram[dramoffset_kvs + i].range(511, 480) = buffer[7][bufferoffset_kvs + i].value; 
		#else 
		kvdram[dramoffset_kvs + i].data[0] = buffer[0][bufferoffset_kvs + i];
		kvdram[dramoffset_kvs + i].data[1] = buffer[1][bufferoffset_kvs + i];
		kvdram[dramoffset_kvs + i].data[2] = buffer[2][bufferoffset_kvs + i];
		kvdram[dramoffset_kvs + i].data[3] = buffer[3][bufferoffset_kvs + i];
		kvdram[dramoffset_kvs + i].data[4] = buffer[4][bufferoffset_kvs + i];
		kvdram[dramoffset_kvs + i].data[5] = buffer[5][bufferoffset_kvs + i];
		kvdram[dramoffset_kvs + i].data[6] = buffer[6][bufferoffset_kvs + i];
		kvdram[dramoffset_kvs + i].data[7] = buffer[7][bufferoffset_kvs + i];
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
	if(dramoffset_kvs >= globalparams.BASEOFFSETKVS_VERTICESDATA && dramoffset_kvs < globalparams.BASEOFFSETKVS_VERTICESDATA + VERTICESDATASZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"savekeyvalues:: vertices saved: offset: "<<(dramoffset_kvs-globalparams.BASEOFFSETKVS_VERTICESDATA) * VECTOR_SIZE<<"-"<<((dramoffset_kvs-globalparams.BASEOFFSETKVS_VERTICESDATA) + size_kvs) * VECTOR_SIZE<<", number of vertex datas written: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }					
	#endif
	return;
}

void 
	#ifdef SW 
	acts::
	#endif 
readkeyvalues(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue16_type buffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
		
	READKEYVALUES2_LOOP: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		buffer[begincol + 0][bufferoffset_kvs + i].key = kvdram[dramoffset_kvs + i].range(31, 0); 
		buffer[begincol + 0][bufferoffset_kvs + i].value = kvdram[dramoffset_kvs + i].range(63, 32); 
		buffer[begincol + 1][bufferoffset_kvs + i].key = kvdram[dramoffset_kvs + i].range(95, 64); 
		buffer[begincol + 1][bufferoffset_kvs + i].value = kvdram[dramoffset_kvs + i].range(127, 96); 
		buffer[begincol + 2][bufferoffset_kvs + i].key = kvdram[dramoffset_kvs + i].range(159, 128); 
		buffer[begincol + 2][bufferoffset_kvs + i].value = kvdram[dramoffset_kvs + i].range(191, 160); 
		buffer[begincol + 3][bufferoffset_kvs + i].key = kvdram[dramoffset_kvs + i].range(223, 192); 
		buffer[begincol + 3][bufferoffset_kvs + i].value = kvdram[dramoffset_kvs + i].range(255, 224); 
		buffer[begincol + 4][bufferoffset_kvs + i].key = kvdram[dramoffset_kvs + i].range(287, 256); 
		buffer[begincol + 4][bufferoffset_kvs + i].value = kvdram[dramoffset_kvs + i].range(319, 288); 
		buffer[begincol + 5][bufferoffset_kvs + i].key = kvdram[dramoffset_kvs + i].range(351, 320); 
		buffer[begincol + 5][bufferoffset_kvs + i].value = kvdram[dramoffset_kvs + i].range(383, 352); 
		buffer[begincol + 6][bufferoffset_kvs + i].key = kvdram[dramoffset_kvs + i].range(415, 384); 
		buffer[begincol + 6][bufferoffset_kvs + i].value = kvdram[dramoffset_kvs + i].range(447, 416); 
		buffer[begincol + 7][bufferoffset_kvs + i].key = kvdram[dramoffset_kvs + i].range(479, 448); 
		buffer[begincol + 7][bufferoffset_kvs + i].value = kvdram[dramoffset_kvs + i].range(511, 480); 
		#else 
		buffer[begincol + 0][bufferoffset_kvs + i].key = kvdram[dramoffset_kvs + i].data[0].key; 
		buffer[begincol + 0][bufferoffset_kvs + i].value = kvdram[dramoffset_kvs + i].data[0].value; 
		buffer[begincol + 1][bufferoffset_kvs + i].key = kvdram[dramoffset_kvs + i].data[1].key; 
		buffer[begincol + 1][bufferoffset_kvs + i].value = kvdram[dramoffset_kvs + i].data[1].value; 
		buffer[begincol + 2][bufferoffset_kvs + i].key = kvdram[dramoffset_kvs + i].data[2].key; 
		buffer[begincol + 2][bufferoffset_kvs + i].value = kvdram[dramoffset_kvs + i].data[2].value; 
		buffer[begincol + 3][bufferoffset_kvs + i].key = kvdram[dramoffset_kvs + i].data[3].key; 
		buffer[begincol + 3][bufferoffset_kvs + i].value = kvdram[dramoffset_kvs + i].data[3].value; 
		buffer[begincol + 4][bufferoffset_kvs + i].key = kvdram[dramoffset_kvs + i].data[4].key; 
		buffer[begincol + 4][bufferoffset_kvs + i].value = kvdram[dramoffset_kvs + i].data[4].value; 
		buffer[begincol + 5][bufferoffset_kvs + i].key = kvdram[dramoffset_kvs + i].data[5].key; 
		buffer[begincol + 5][bufferoffset_kvs + i].value = kvdram[dramoffset_kvs + i].data[5].value; 
		buffer[begincol + 6][bufferoffset_kvs + i].key = kvdram[dramoffset_kvs + i].data[6].key; 
		buffer[begincol + 6][bufferoffset_kvs + i].value = kvdram[dramoffset_kvs + i].data[6].value; 
		buffer[begincol + 7][bufferoffset_kvs + i].key = kvdram[dramoffset_kvs + i].data[7].key; 
		buffer[begincol + 7][bufferoffset_kvs + i].value = kvdram[dramoffset_kvs + i].data[7].value; 
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
	if(dramoffset_kvs >= globalparams.BASEOFFSETKVS_VERTICESDATA_ && dramoffset_kvs < globalparams.BASEOFFSETKVS_VERTICESDATA + VERTICESDATASZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"readkeyvalues:: vertices read: offset: "<<(dramoffset_kvs - globalparams.BASEOFFSETKVS_VERTICESDATA) * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs - globalparams.BASEOFFSETKVS_VERTICESDATA) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	#endif
	return;
}

void 
	#ifdef SW 
	acts::
	#endif 
savekeyvalues(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue16_type buffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount =  APPLYVERTEXBUFFERSZ / 2;
	
	SAVEKEYVALUES2_LOOP: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		kvdram[dramoffset_kvs + i].range(31, 0) = buffer[begincol + 0][bufferoffset_kvs + i].key; 
		kvdram[dramoffset_kvs + i].range(63, 32) = buffer[begincol + 0][bufferoffset_kvs + i].value; 
		kvdram[dramoffset_kvs + i].range(95, 64) = buffer[begincol + 1][bufferoffset_kvs + i].key; 
		kvdram[dramoffset_kvs + i].range(127, 96) = buffer[begincol + 1][bufferoffset_kvs + i].value; 
		kvdram[dramoffset_kvs + i].range(159, 128) = buffer[begincol + 2][bufferoffset_kvs + i].key; 
		kvdram[dramoffset_kvs + i].range(191, 160) = buffer[begincol + 2][bufferoffset_kvs + i].value; 
		kvdram[dramoffset_kvs + i].range(223, 192) = buffer[begincol + 3][bufferoffset_kvs + i].key; 
		kvdram[dramoffset_kvs + i].range(255, 224) = buffer[begincol + 3][bufferoffset_kvs + i].value; 
		kvdram[dramoffset_kvs + i].range(287, 256) = buffer[begincol + 4][bufferoffset_kvs + i].key; 
		kvdram[dramoffset_kvs + i].range(319, 288) = buffer[begincol + 4][bufferoffset_kvs + i].value; 
		kvdram[dramoffset_kvs + i].range(351, 320) = buffer[begincol + 5][bufferoffset_kvs + i].key; 
		kvdram[dramoffset_kvs + i].range(383, 352) = buffer[begincol + 5][bufferoffset_kvs + i].value; 
		kvdram[dramoffset_kvs + i].range(415, 384) = buffer[begincol + 6][bufferoffset_kvs + i].key; 
		kvdram[dramoffset_kvs + i].range(447, 416) = buffer[begincol + 6][bufferoffset_kvs + i].value; 
		kvdram[dramoffset_kvs + i].range(479, 448) = buffer[begincol + 7][bufferoffset_kvs + i].key; 
		kvdram[dramoffset_kvs + i].range(511, 480) = buffer[begincol + 7][bufferoffset_kvs + i].value; 
		#else 
		kvdram[dramoffset_kvs + i].data[0].key = buffer[begincol + 0][bufferoffset_kvs + i].key;
		kvdram[dramoffset_kvs + i].data[0].value = buffer[begincol + 0][bufferoffset_kvs + i].value;
		kvdram[dramoffset_kvs + i].data[1].key = buffer[begincol + 1][bufferoffset_kvs + i].key;
		kvdram[dramoffset_kvs + i].data[1].value = buffer[begincol + 1][bufferoffset_kvs + i].value;
		kvdram[dramoffset_kvs + i].data[2].key = buffer[begincol + 2][bufferoffset_kvs + i].key;
		kvdram[dramoffset_kvs + i].data[2].value = buffer[begincol + 2][bufferoffset_kvs + i].value;
		kvdram[dramoffset_kvs + i].data[3].key = buffer[begincol + 3][bufferoffset_kvs + i].key;
		kvdram[dramoffset_kvs + i].data[3].value = buffer[begincol + 3][bufferoffset_kvs + i].value;
		kvdram[dramoffset_kvs + i].data[4].key = buffer[begincol + 4][bufferoffset_kvs + i].key;
		kvdram[dramoffset_kvs + i].data[4].value = buffer[begincol + 4][bufferoffset_kvs + i].value;
		kvdram[dramoffset_kvs + i].data[5].key = buffer[begincol + 5][bufferoffset_kvs + i].key;
		kvdram[dramoffset_kvs + i].data[5].value = buffer[begincol + 5][bufferoffset_kvs + i].value;
		kvdram[dramoffset_kvs + i].data[6].key = buffer[begincol + 6][bufferoffset_kvs + i].key;
		kvdram[dramoffset_kvs + i].data[6].value = buffer[begincol + 6][bufferoffset_kvs + i].value;
		kvdram[dramoffset_kvs + i].data[7].key = buffer[begincol + 7][bufferoffset_kvs + i].key;
		kvdram[dramoffset_kvs + i].data[7].value = buffer[begincol + 7][bufferoffset_kvs + i].value;
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
	if(dramoffset_kvs >= globalparams.BASEOFFSETKVS_VERTICESDATA && dramoffset_kvs < globalparams.BASEOFFSETKVS_VERTICESDATA + VERTICESDATASZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"savekeyvalues:: vertices saved: offset: "<<(dramoffset_kvs-globalparams.BASEOFFSETKVS_VERTICESDATA) * VECTOR_SIZE<<"-"<<((dramoffset_kvs-globalparams.BASEOFFSETKVS_VERTICESDATA) + size_kvs) * VECTOR_SIZE<<", number of vertex datas written: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }					
	#endif
	return;
}

void 
	#ifdef SW 
	acts::
	#endif 
readkeyvalues(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue16_type buffer1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type buffer1offset_kvs, keyvalue16_type buffer2[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], batch_type buffer2offset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
		
	READKEYVALUES4_LOOP: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		buffer1[begincol + 0][buffer1offset_kvs + i].key = kvdram[dramoffset_kvs + i].range(31, 0); 
		buffer1[begincol + 0][buffer1offset_kvs + i].value = kvdram[dramoffset_kvs + i].range(63, 32); 
		buffer1[begincol + 1][buffer1offset_kvs + i].key = kvdram[dramoffset_kvs + i].range(95, 64); 
		buffer1[begincol + 1][buffer1offset_kvs + i].value = kvdram[dramoffset_kvs + i].range(127, 96); 
		buffer1[begincol + 2][buffer1offset_kvs + i].key = kvdram[dramoffset_kvs + i].range(159, 128); 
		buffer1[begincol + 2][buffer1offset_kvs + i].value = kvdram[dramoffset_kvs + i].range(191, 160); 
		buffer1[begincol + 3][buffer1offset_kvs + i].key = kvdram[dramoffset_kvs + i].range(223, 192); 
		buffer1[begincol + 3][buffer1offset_kvs + i].value = kvdram[dramoffset_kvs + i].range(255, 224); 
		buffer1[begincol + 4][buffer1offset_kvs + i].key = kvdram[dramoffset_kvs + i].range(287, 256); 
		buffer1[begincol + 4][buffer1offset_kvs + i].value = kvdram[dramoffset_kvs + i].range(319, 288); 
		buffer1[begincol + 5][buffer1offset_kvs + i].key = kvdram[dramoffset_kvs + i].range(351, 320); 
		buffer1[begincol + 5][buffer1offset_kvs + i].value = kvdram[dramoffset_kvs + i].range(383, 352); 
		buffer1[begincol + 6][buffer1offset_kvs + i].key = kvdram[dramoffset_kvs + i].range(415, 384); 
		buffer1[begincol + 6][buffer1offset_kvs + i].value = kvdram[dramoffset_kvs + i].range(447, 416); 
		buffer1[begincol + 7][buffer1offset_kvs + i].key = kvdram[dramoffset_kvs + i].range(479, 448); 
		buffer1[begincol + 7][buffer1offset_kvs + i].value = kvdram[dramoffset_kvs + i].range(511, 480); 
		buffer2[begincol + 0][buffer2offset_kvs + i].key = kvdram[dramoffset_kvs + i].range(31, 0); 
		buffer2[begincol + 0][buffer2offset_kvs + i].value = kvdram[dramoffset_kvs + i].range(63, 32); 
		buffer2[begincol + 1][buffer2offset_kvs + i].key = kvdram[dramoffset_kvs + i].range(95, 64); 
		buffer2[begincol + 1][buffer2offset_kvs + i].value = kvdram[dramoffset_kvs + i].range(127, 96); 
		buffer2[begincol + 2][buffer2offset_kvs + i].key = kvdram[dramoffset_kvs + i].range(159, 128); 
		buffer2[begincol + 2][buffer2offset_kvs + i].value = kvdram[dramoffset_kvs + i].range(191, 160); 
		buffer2[begincol + 3][buffer2offset_kvs + i].key = kvdram[dramoffset_kvs + i].range(223, 192); 
		buffer2[begincol + 3][buffer2offset_kvs + i].value = kvdram[dramoffset_kvs + i].range(255, 224); 
		buffer2[begincol + 4][buffer2offset_kvs + i].key = kvdram[dramoffset_kvs + i].range(287, 256); 
		buffer2[begincol + 4][buffer2offset_kvs + i].value = kvdram[dramoffset_kvs + i].range(319, 288); 
		buffer2[begincol + 5][buffer2offset_kvs + i].key = kvdram[dramoffset_kvs + i].range(351, 320); 
		buffer2[begincol + 5][buffer2offset_kvs + i].value = kvdram[dramoffset_kvs + i].range(383, 352); 
		buffer2[begincol + 6][buffer2offset_kvs + i].key = kvdram[dramoffset_kvs + i].range(415, 384); 
		buffer2[begincol + 6][buffer2offset_kvs + i].value = kvdram[dramoffset_kvs + i].range(447, 416); 
		buffer2[begincol + 7][buffer2offset_kvs + i].key = kvdram[dramoffset_kvs + i].range(479, 448); 
		buffer2[begincol + 7][buffer2offset_kvs + i].value = kvdram[dramoffset_kvs + i].range(511, 480); 
		#else 
		buffer1[begincol + 0][buffer1offset_kvs + i].key = kvdram[dramoffset_kvs + i].data[0].key; 
		buffer1[begincol + 0][buffer1offset_kvs + i].value = kvdram[dramoffset_kvs + i].data[0].value; 
		buffer1[begincol + 1][buffer1offset_kvs + i].key = kvdram[dramoffset_kvs + i].data[1].key; 
		buffer1[begincol + 1][buffer1offset_kvs + i].value = kvdram[dramoffset_kvs + i].data[1].value; 
		buffer1[begincol + 2][buffer1offset_kvs + i].key = kvdram[dramoffset_kvs + i].data[2].key; 
		buffer1[begincol + 2][buffer1offset_kvs + i].value = kvdram[dramoffset_kvs + i].data[2].value; 
		buffer1[begincol + 3][buffer1offset_kvs + i].key = kvdram[dramoffset_kvs + i].data[3].key; 
		buffer1[begincol + 3][buffer1offset_kvs + i].value = kvdram[dramoffset_kvs + i].data[3].value; 
		buffer1[begincol + 4][buffer1offset_kvs + i].key = kvdram[dramoffset_kvs + i].data[4].key; 
		buffer1[begincol + 4][buffer1offset_kvs + i].value = kvdram[dramoffset_kvs + i].data[4].value; 
		buffer1[begincol + 5][buffer1offset_kvs + i].key = kvdram[dramoffset_kvs + i].data[5].key; 
		buffer1[begincol + 5][buffer1offset_kvs + i].value = kvdram[dramoffset_kvs + i].data[5].value; 
		buffer1[begincol + 6][buffer1offset_kvs + i].key = kvdram[dramoffset_kvs + i].data[6].key; 
		buffer1[begincol + 6][buffer1offset_kvs + i].value = kvdram[dramoffset_kvs + i].data[6].value; 
		buffer1[begincol + 7][buffer1offset_kvs + i].key = kvdram[dramoffset_kvs + i].data[7].key; 
		buffer1[begincol + 7][buffer1offset_kvs + i].value = kvdram[dramoffset_kvs + i].data[7].value; 
		buffer2[begincol + 0][buffer2offset_kvs + i].key = kvdram[dramoffset_kvs + i].data[0].key; 
		buffer2[begincol + 0][buffer2offset_kvs + i].value = kvdram[dramoffset_kvs + i].data[0].value; 
		buffer2[begincol + 1][buffer2offset_kvs + i].key = kvdram[dramoffset_kvs + i].data[1].key; 
		buffer2[begincol + 1][buffer2offset_kvs + i].value = kvdram[dramoffset_kvs + i].data[1].value; 
		buffer2[begincol + 2][buffer2offset_kvs + i].key = kvdram[dramoffset_kvs + i].data[2].key; 
		buffer2[begincol + 2][buffer2offset_kvs + i].value = kvdram[dramoffset_kvs + i].data[2].value; 
		buffer2[begincol + 3][buffer2offset_kvs + i].key = kvdram[dramoffset_kvs + i].data[3].key; 
		buffer2[begincol + 3][buffer2offset_kvs + i].value = kvdram[dramoffset_kvs + i].data[3].value; 
		buffer2[begincol + 4][buffer2offset_kvs + i].key = kvdram[dramoffset_kvs + i].data[4].key; 
		buffer2[begincol + 4][buffer2offset_kvs + i].value = kvdram[dramoffset_kvs + i].data[4].value; 
		buffer2[begincol + 5][buffer2offset_kvs + i].key = kvdram[dramoffset_kvs + i].data[5].key; 
		buffer2[begincol + 5][buffer2offset_kvs + i].value = kvdram[dramoffset_kvs + i].data[5].value; 
		buffer2[begincol + 6][buffer2offset_kvs + i].key = kvdram[dramoffset_kvs + i].data[6].key; 
		buffer2[begincol + 6][buffer2offset_kvs + i].value = kvdram[dramoffset_kvs + i].data[6].value; 
		buffer2[begincol + 7][buffer2offset_kvs + i].key = kvdram[dramoffset_kvs + i].data[7].key; 
		buffer2[begincol + 7][buffer2offset_kvs + i].value = kvdram[dramoffset_kvs + i].data[7].value; 
		#endif 
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<< TIMINGRESULTSCOLOR<<"readkeyvalues:: vertices read: offset: "<<dramoffset_kvs * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl;
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(dramoffset_kvs >= globalparams.BASEOFFSETKVS_VERTEXPTR && dramoffset_kvs < globalparams.BASEOFFSETKVS_VERTEXPTR + VERTEXPTRSSZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"readkeyvalues:: vertices read: offset: "<<(dramoffset_kvs - BASEOFFSETKVS_VERTEXPTR) * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs - BASEOFFSETKVS_VERTEXPTR) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	if(dramoffset_kvs >= globalparams.BASEOFFSETKVS_VERTICESDATA && dramoffset_kvs < globalparams.BASEOFFSETKVS_VERTICESDATA + VERTICESDATASZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"readkeyvalues:: vertices read: offset: "<<(dramoffset_kvs - globalparams.BASEOFFSETKVS_VERTICESDATA) * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs - globalparams.BASEOFFSETKVS_VERTICESDATA) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	#endif
	return;
}

void 
	#ifdef SW 
	acts::
	#endif 
loadvmasks(bool_type enable, uint512_dt * kvdram, uintNUMPby2_type vmask[BLOCKRAM_SIZE], keyvalue_t tempbuffer[VECTOR_SIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	uint32_type bitsbuffer[MAXREDUCEBUFFERSZ];
	
	buffer_type reducebuffersz = globalparams.SIZE_REDUCE / 2;
	buffer_type vmaskbuffersz_kvs = (globalparams.SIZE_REDUCE * NUM_PARTITIONS) / 512;
	
	readkeyvalues(ON, kvdram, offset_kvs, tempbuffer, 0, size_kvs, globalparams);
	buffer_type index = 0;
	LOADVMASKS_LOOP1: for (buffer_type i=0; i<vmaskbuffersz_kvs; i++){
	#pragma HLS PIPELINE II=8
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("acts::loadvmasks 1", i*16, reducebuffersz, NAp, NAp, NAp);
		actsutilityobj->checkoutofbounds("acts::loadvmasks 1", i, vmaskbuffersz_kvs, NAp, NAp, NAp);
		#endif
	
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
		
		index += NUM_PARTITIONS;
	}
	
	LOADVMASKS_LOOP2: for (buffer_type i=0; i<reducebuffersz; i++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		vmask[i].data[0].key = bitsbuffer[i].range(0, 0);
		vmask[i].data[0].value = bitsbuffer[i].range(1, 1);
		vmask[i].data[1].key = bitsbuffer[i].range(2, 2);
		vmask[i].data[1].value = bitsbuffer[i].range(3, 3);
		vmask[i].data[2].key = bitsbuffer[i].range(4, 4);
		vmask[i].data[2].value = bitsbuffer[i].range(5, 5);
		vmask[i].data[3].key = bitsbuffer[i].range(6, 6);
		vmask[i].data[3].value = bitsbuffer[i].range(7, 7);
		vmask[i].data[4].key = bitsbuffer[i].range(8, 8);
		vmask[i].data[4].value = bitsbuffer[i].range(9, 9);
		vmask[i].data[5].key = bitsbuffer[i].range(10, 10);
		vmask[i].data[5].value = bitsbuffer[i].range(11, 11);
		vmask[i].data[6].key = bitsbuffer[i].range(12, 12);
		vmask[i].data[6].value = bitsbuffer[i].range(13, 13);
		vmask[i].data[7].key = bitsbuffer[i].range(14, 14);
		vmask[i].data[7].value = bitsbuffer[i].range(15, 15);
		vmask[i].data[8].key = bitsbuffer[i].range(16, 16);
		vmask[i].data[8].value = bitsbuffer[i].range(17, 17);
		vmask[i].data[9].key = bitsbuffer[i].range(18, 18);
		vmask[i].data[9].value = bitsbuffer[i].range(19, 19);
		vmask[i].data[10].key = bitsbuffer[i].range(20, 20);
		vmask[i].data[10].value = bitsbuffer[i].range(21, 21);
		vmask[i].data[11].key = bitsbuffer[i].range(22, 22);
		vmask[i].data[11].value = bitsbuffer[i].range(23, 23);
		vmask[i].data[12].key = bitsbuffer[i].range(24, 24);
		vmask[i].data[12].value = bitsbuffer[i].range(25, 25);
		vmask[i].data[13].key = bitsbuffer[i].range(26, 26);
		vmask[i].data[13].value = bitsbuffer[i].range(27, 27);
		vmask[i].data[14].key = bitsbuffer[i].range(28, 28);
		vmask[i].data[14].value = bitsbuffer[i].range(29, 29);
		vmask[i].data[15].key = bitsbuffer[i].range(30, 30);
		vmask[i].data[15].value = bitsbuffer[i].range(31, 31);
		#else 
		vmask[i].data[0].key = READFROM_UINT(bitsbuffer[i], 0, 1);
		vmask[i].data[0].value = READFROM_UINT(bitsbuffer[i], 1, 1);
		vmask[i].data[1].key = READFROM_UINT(bitsbuffer[i], 2, 1);
		vmask[i].data[1].value = READFROM_UINT(bitsbuffer[i], 3, 1);
		vmask[i].data[2].key = READFROM_UINT(bitsbuffer[i], 4, 1);
		vmask[i].data[2].value = READFROM_UINT(bitsbuffer[i], 5, 1);
		vmask[i].data[3].key = READFROM_UINT(bitsbuffer[i], 6, 1);
		vmask[i].data[3].value = READFROM_UINT(bitsbuffer[i], 7, 1);
		vmask[i].data[4].key = READFROM_UINT(bitsbuffer[i], 8, 1);
		vmask[i].data[4].value = READFROM_UINT(bitsbuffer[i], 9, 1);
		vmask[i].data[5].key = READFROM_UINT(bitsbuffer[i], 10, 1);
		vmask[i].data[5].value = READFROM_UINT(bitsbuffer[i], 11, 1);
		vmask[i].data[6].key = READFROM_UINT(bitsbuffer[i], 12, 1);
		vmask[i].data[6].value = READFROM_UINT(bitsbuffer[i], 13, 1);
		vmask[i].data[7].key = READFROM_UINT(bitsbuffer[i], 14, 1);
		vmask[i].data[7].value = READFROM_UINT(bitsbuffer[i], 15, 1);
		vmask[i].data[8].key = READFROM_UINT(bitsbuffer[i], 16, 1);
		vmask[i].data[8].value = READFROM_UINT(bitsbuffer[i], 17, 1);
		vmask[i].data[9].key = READFROM_UINT(bitsbuffer[i], 18, 1);
		vmask[i].data[9].value = READFROM_UINT(bitsbuffer[i], 19, 1);
		vmask[i].data[10].key = READFROM_UINT(bitsbuffer[i], 20, 1);
		vmask[i].data[10].value = READFROM_UINT(bitsbuffer[i], 21, 1);
		vmask[i].data[11].key = READFROM_UINT(bitsbuffer[i], 22, 1);
		vmask[i].data[11].value = READFROM_UINT(bitsbuffer[i], 23, 1);
		vmask[i].data[12].key = READFROM_UINT(bitsbuffer[i], 24, 1);
		vmask[i].data[12].value = READFROM_UINT(bitsbuffer[i], 25, 1);
		vmask[i].data[13].key = READFROM_UINT(bitsbuffer[i], 26, 1);
		vmask[i].data[13].value = READFROM_UINT(bitsbuffer[i], 27, 1);
		vmask[i].data[14].key = READFROM_UINT(bitsbuffer[i], 28, 1);
		vmask[i].data[14].value = READFROM_UINT(bitsbuffer[i], 29, 1);
		vmask[i].data[15].key = READFROM_UINT(bitsbuffer[i], 30, 1);
		vmask[i].data[15].value = READFROM_UINT(bitsbuffer[i], 31, 1);
		#endif
	}
	return;
}

void 
	#ifdef SW 
	acts::
	#endif 
savevmasks(bool_type enable, uint512_dt * kvdram, uintNUMPby2_type vmask[BLOCKRAM_SIZE], keyvalue_t tempbuffer[VECTOR_SIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	uint32_type bitsbuffer[MAXREDUCEBUFFERSZ];
	
	buffer_type reducebuffersz = globalparams.SIZE_REDUCE / 2;
	buffer_type vmaskbuffersz_kvs = (globalparams.SIZE_REDUCE * NUM_PARTITIONS) / 512;

	SAVEVMASKS_LOOP1: for (buffer_type i=0; i<reducebuffersz; i++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		bitsbuffer[i].range(0, 0) = vmask[i].data[0].key;
		bitsbuffer[i].range(1, 1) = vmask[i].data[0].value;
		bitsbuffer[i].range(2, 2) = vmask[i].data[1].key;
		bitsbuffer[i].range(3, 3) = vmask[i].data[1].value;
		bitsbuffer[i].range(4, 4) = vmask[i].data[2].key;
		bitsbuffer[i].range(5, 5) = vmask[i].data[2].value;
		bitsbuffer[i].range(6, 6) = vmask[i].data[3].key;
		bitsbuffer[i].range(7, 7) = vmask[i].data[3].value;
		bitsbuffer[i].range(8, 8) = vmask[i].data[4].key;
		bitsbuffer[i].range(9, 9) = vmask[i].data[4].value;
		bitsbuffer[i].range(10, 10) = vmask[i].data[5].key;
		bitsbuffer[i].range(11, 11) = vmask[i].data[5].value;
		bitsbuffer[i].range(12, 12) = vmask[i].data[6].key;
		bitsbuffer[i].range(13, 13) = vmask[i].data[6].value;
		bitsbuffer[i].range(14, 14) = vmask[i].data[7].key;
		bitsbuffer[i].range(15, 15) = vmask[i].data[7].value;
		bitsbuffer[i].range(16, 16) = vmask[i].data[8].key;
		bitsbuffer[i].range(17, 17) = vmask[i].data[8].value;
		bitsbuffer[i].range(18, 18) = vmask[i].data[9].key;
		bitsbuffer[i].range(19, 19) = vmask[i].data[9].value;
		bitsbuffer[i].range(20, 20) = vmask[i].data[10].key;
		bitsbuffer[i].range(21, 21) = vmask[i].data[10].value;
		bitsbuffer[i].range(22, 22) = vmask[i].data[11].key;
		bitsbuffer[i].range(23, 23) = vmask[i].data[11].value;
		bitsbuffer[i].range(24, 24) = vmask[i].data[12].key;
		bitsbuffer[i].range(25, 25) = vmask[i].data[12].value;
		bitsbuffer[i].range(26, 26) = vmask[i].data[13].key;
		bitsbuffer[i].range(27, 27) = vmask[i].data[13].value;
		bitsbuffer[i].range(28, 28) = vmask[i].data[14].key;
		bitsbuffer[i].range(29, 29) = vmask[i].data[14].value;
		bitsbuffer[i].range(30, 30) = vmask[i].data[15].key;
		bitsbuffer[i].range(31, 31) = vmask[i].data[15].value;
		#else 
		WRITETO_UINT(&bitsbuffer[i], 0, 1, vmask[i].data[0].key);
		WRITETO_UINT(&bitsbuffer[i], 1, 1, vmask[i].data[0].value);
		WRITETO_UINT(&bitsbuffer[i], 2, 1, vmask[i].data[1].key);
		WRITETO_UINT(&bitsbuffer[i], 3, 1, vmask[i].data[1].value);
		WRITETO_UINT(&bitsbuffer[i], 4, 1, vmask[i].data[2].key);
		WRITETO_UINT(&bitsbuffer[i], 5, 1, vmask[i].data[2].value);
		WRITETO_UINT(&bitsbuffer[i], 6, 1, vmask[i].data[3].key);
		WRITETO_UINT(&bitsbuffer[i], 7, 1, vmask[i].data[3].value);
		WRITETO_UINT(&bitsbuffer[i], 8, 1, vmask[i].data[4].key);
		WRITETO_UINT(&bitsbuffer[i], 9, 1, vmask[i].data[4].value);
		WRITETO_UINT(&bitsbuffer[i], 10, 1, vmask[i].data[5].key);
		WRITETO_UINT(&bitsbuffer[i], 11, 1, vmask[i].data[5].value);
		WRITETO_UINT(&bitsbuffer[i], 12, 1, vmask[i].data[6].key);
		WRITETO_UINT(&bitsbuffer[i], 13, 1, vmask[i].data[6].value);
		WRITETO_UINT(&bitsbuffer[i], 14, 1, vmask[i].data[7].key);
		WRITETO_UINT(&bitsbuffer[i], 15, 1, vmask[i].data[7].value);
		WRITETO_UINT(&bitsbuffer[i], 16, 1, vmask[i].data[8].key);
		WRITETO_UINT(&bitsbuffer[i], 17, 1, vmask[i].data[8].value);
		WRITETO_UINT(&bitsbuffer[i], 18, 1, vmask[i].data[9].key);
		WRITETO_UINT(&bitsbuffer[i], 19, 1, vmask[i].data[9].value);
		WRITETO_UINT(&bitsbuffer[i], 20, 1, vmask[i].data[10].key);
		WRITETO_UINT(&bitsbuffer[i], 21, 1, vmask[i].data[10].value);
		WRITETO_UINT(&bitsbuffer[i], 22, 1, vmask[i].data[11].key);
		WRITETO_UINT(&bitsbuffer[i], 23, 1, vmask[i].data[11].value);
		WRITETO_UINT(&bitsbuffer[i], 24, 1, vmask[i].data[12].key);
		WRITETO_UINT(&bitsbuffer[i], 25, 1, vmask[i].data[12].value);
		WRITETO_UINT(&bitsbuffer[i], 26, 1, vmask[i].data[13].key);
		WRITETO_UINT(&bitsbuffer[i], 27, 1, vmask[i].data[13].value);
		WRITETO_UINT(&bitsbuffer[i], 28, 1, vmask[i].data[14].key);
		WRITETO_UINT(&bitsbuffer[i], 29, 1, vmask[i].data[14].value);
		WRITETO_UINT(&bitsbuffer[i], 30, 1, vmask[i].data[15].key);
		WRITETO_UINT(&bitsbuffer[i], 31, 1, vmask[i].data[15].value);
		#endif
	}
	
	buffer_type index = 0;
	SAVEVMASKS_LOOP2: for (buffer_type i=0; i<vmaskbuffersz_kvs; i++){
	#pragma HLS PIPELINE II=8
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("acts::savevmasks 1", i*16, reducebuffersz, NAp, NAp, NAp);
		actsutilityobj->checkoutofbounds("acts::savevmasks 1", i, vmaskbuffersz_kvs, NAp, NAp, NAp);
		#endif
		
		tempbuffer[0][i].key = bitsbuffer[index + 0];
		tempbuffer[0][i].value = bitsbuffer[index + 0 + 1]; 
		tempbuffer[1][i].key = bitsbuffer[index + 2];
		tempbuffer[1][i].value = bitsbuffer[index + 2 + 1]; 
		tempbuffer[2][i].key = bitsbuffer[index + 4];
		tempbuffer[2][i].value = bitsbuffer[index + 4 + 1]; 
		tempbuffer[3][i].key = bitsbuffer[index + 6];
		tempbuffer[3][i].value = bitsbuffer[index + 6 + 1]; 
		tempbuffer[4][i].key = bitsbuffer[index + 8];
		tempbuffer[4][i].value = bitsbuffer[index + 8 + 1]; 
		tempbuffer[5][i].key = bitsbuffer[index + 10];
		tempbuffer[5][i].value = bitsbuffer[index + 10 + 1]; 
		tempbuffer[6][i].key = bitsbuffer[index + 12];
		tempbuffer[6][i].value = bitsbuffer[index + 12 + 1]; 
		tempbuffer[7][i].key = bitsbuffer[index + 14];
		tempbuffer[7][i].value = bitsbuffer[index + 14 + 1]; 
		
		index += NUM_PARTITIONS;
		
	}
	savekeyvalues(ON, kvdram, offset_kvs, tempbuffer, 0, size_kvs, globalparams); 
	return;
}

value_t 
	#ifdef SW 
	acts::
	#endif 
readfromvbuffer(keyvalue16_type vbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unsigned int loc, globalparams_t globalparams){
	#pragma HLS INLINE
	value_t data = 0;
	
	#ifdef AUTOMATEMODEON
	unsigned int col = loc >> globalparams.POW_REDUCE;
	unsigned int row;
	if(globalparams.POW_REDUCE == 11){ // AUTOMATEME. VHLS CHECKME.
		row = loc % (1 << 11);
	} else if(globalparams.POW_REDUCE == 10){
		row = loc % (1 << 10);
	} else if(globalparams.POW_REDUCE == 9){
		row = loc % (1 << 9);
	} else if(globalparams.POW_REDUCE == 8){
		row = loc % (1 << 8);
	} else {
		row = 0;
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"readfromvbuffer: ERROR: out of selection. globalparams.POW_REDUCE: "<<globalparams.POW_REDUCE<<". exiting..."<<endl;
		exit(EXIT_FAILURE);
		#endif 
	}
	row = row / 2;
	#else 
	unsigned int col = loc / REDUCESZ; 
	unsigned int row = loc % REDUCESZ;
	row = row / 2;
	#endif 
	
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("readfromvbuffer.col", col, NUM_PARTITIONS, loc, NAp, NAp); // AUTOMATEME.
	actsutilityobj->checkoutofbounds("readfromvbuffer.row", row, BLOCKRAM_SIZE, loc, NAp, NAp);
	#endif 
	
	if(loc % 2 == 0){ data = vbuffer[col][row].key; } 
	else { data = vbuffer[col][row].value; }
	
	// if(col < NUM_PARTITIONS/2){
		// if(loc % 2 == 0){ data = vbuffer[col][row].key; } 
		// else { data = vbuffer[col][row].value; }
	// } else {
		// if(loc % 2 == 0){ data = vbuffer2[col - (NUM_PARTITIONS/2)][row].key; } 
		// else { data = vbuffer2[col - (NUM_PARTITIONS/2)][row].value; }
	// }
	return data;
}

value_t 
	#ifdef SW 
	acts::
	#endif 
readfromvmask(uintNUMPby2_type vmask[BLOCKRAM_SIZE], unsigned int loc, globalparams_t globalparams){
	#pragma HLS INLINE
	value_t data = 0;
	
	#ifdef AUTOMATEMODEON
	unsigned int col = loc >> globalparams.POW_REDUCE;
	unsigned int row;
	if(globalparams.POW_REDUCE == 11){ // AUTOMATEME. VHLS CHECKME.
		row = loc % (1 << 11);
	} else if(globalparams.POW_REDUCE == 10){
		row = loc % (1 << 10);
	} else if(globalparams.POW_REDUCE == 9){
		row = loc % (1 << 9);
	} else if(globalparams.POW_REDUCE == 8){
		row = loc % (1 << 8);
	} else {
		row = 0;
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"readfromvbuffer: ERROR: out of selection. globalparams.POW_REDUCE: "<<globalparams.POW_REDUCE<<". exiting..."<<endl;
		exit(EXIT_FAILURE);
		#endif 
	}
	row = row / 2;
	#else 
	unsigned int col = loc / REDUCESZ;
	unsigned int row = loc % REDUCESZ;
	row = row / 2;
	#endif
	
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("readfromvmask.col", col, NUM_PARTITIONS, loc, NAp, NAp);
	actsutilityobj->checkoutofbounds("readfromvmask.row", row, BLOCKRAM_SIZE, loc, NAp, NAp);
	#endif 
	
	if(loc % 2 == 0){ data = vmask[row].data[col].key; } 
	else { data = vmask[row].data[col].value; }
	return data;
}

// functions (process)
value_t 
	#ifdef SW 
	acts::
	#endif 
processfunc(value_t udata, value_t edgew, unsigned int GraphAlgo){
	value_t res = 0;
	#ifdef PR_ALGORITHM
	res = udata;
	#endif
	#ifdef BFS_ALGORITHM
	// res = NAp;  // BFS
	res = udata + edgew;
	#endif 
	#ifdef SSSP_ALGORITHM
	res = udata + edgew;
	#endif
	return res;
}

#ifdef KOKOOOOOOO
bool_type 
	#ifdef SW 
	acts::
	#endif 
readandprocess(bool_type enable, uint512_dt * kvdram, keyvalue16_type vbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask[BLOCKRAM_SIZE], keyvalue_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], 
		batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, sweepparams_t sweepparams, globalparams_t globalparams){	
	if(enable == OFF){ return OFF; }
	analysis_type analysis_srcbuffersz = WORKBUFFER_SIZE / 2;
	
	keyvalue_t tempbuffer[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = tempbuffer
	value_t E[2][VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=E complete
	bool_type en = ON;
	bool_type validfetch = OFF;
	buffer_type reducebuffersz = globalparams.SIZE_REDUCE / 2;
	unsigned int validbound = reducebuffersz * FETFACTOR * VECTOR2_SIZE;
	
	travstate.i_kvs = travstate.i_kvs / 2; // edges is singlevaluetype
	travstate.end_kvs = travstate.end_kvs / 2;
	loffset_kvs = loffset_kvs / 2;
	size_kvs = size_kvs / 2;
	
	readkeyvalues(ON, kvdram, tempbuffer, goffset_kvs + loffset_kvs, size_kvs, travstate, globalparams);
	
	buffer_type chunk_size = getchunksize_kvs(size_kvs, travstate, 0);
	READANDPROCESS_LOOP1: for (buffer_type i=0; i<chunk_size; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
	#pragma HLS PIPELINE II=2 // CRITICAL NEWCHANGE.
		E[0][0] = tempbuffer[0][i].key;
		E[0][1] = tempbuffer[0][i].value;
		E[0][2] = tempbuffer[1][i].key;
		E[0][3] = tempbuffer[1][i].value;
		E[0][4] = tempbuffer[2][i].key;
		E[0][5] = tempbuffer[2][i].value;
		E[0][6] = tempbuffer[3][i].key;
		E[0][7] = tempbuffer[3][i].value;
		E[1][0] = tempbuffer[4][i].key;
		E[1][1] = tempbuffer[4][i].value;
		E[1][2] = tempbuffer[5][i].key;
		E[1][3] = tempbuffer[5][i].value;
		E[1][4] = tempbuffer[6][i].key;
		E[1][5] = tempbuffer[6][i].value;
		E[1][6] = tempbuffer[7][i].key;
		E[1][7] = tempbuffer[7][i].value;
		
		en = ON;
		
		vertex_t srcvid = E[0][0]; // first data is srcvid
		vertex_t lvid = srcvid - travstate.i2;
		if(lvid >= validbound || srcvid == UNUSEDDATA){ en = OFF; lvid = 0; }
		#ifdef _DEBUGMODE_CHECKS2
		if(srcvid < travstate.i2){ cout<<"readandprocess: INVALID srcvid. this is an error. i: "<<i<<", srcvid: "<<srcvid<<", travstate.i2: "<<travstate.i2<<". exiting..."<<endl; exit(EXIT_FAILURE); }
		actsutilityobj->checkoutofbounds("readandprocess.1", lvid, reducebuffersz * FETFACTOR * VECTOR2_SIZE, srcvid, travstate.i2, NAp);
		#endif
		
		value_t udata = readfromvbuffer(vbuffer, lvid, globalparams);
		#ifdef ALLVERTEXISACTIVE_ALGORITHM
		unsigned int mask = 1;
		#else
		unsigned int mask = readfromvmask(vmask, lvid, globalparams);
		#endif
		if(mask == 1){ validfetch = ON; }
		value_t res  = processfunc(udata, 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID); 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("readandprocess.1", mask, 2, NAp, NAp, NAp);
		#endif
		
		if(en == ON && mask == 1){
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
			if(false){ cout<<"readandprocess: i: "<<i<<", mask: "<<mask<<", srcvid: "<<srcvid<<", travstate.i2: "<<travstate.i2<<", lvid: "<<lvid<<", udata: "<<udata<<endl; }
			for(unsigned int v=0; v<VECTOR_SIZE; v++){ cout<<"readandprocess: udata: "<<udata<<", E[0]["<<v<<"]: "<<E[0][v]<<", sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }
			for(unsigned int v=0; v<VECTOR_SIZE; v++){ cout<<"readandprocess: udata: "<<udata<<", E[1]["<<v<<"]: "<<E[1][v]<<", sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }
			#endif 
			
			buffer[0][2*i].key = E[0][0]; 
			buffer[0][2*i].value = res; 
			buffer[1][2*i].key = E[0][1]; 
			buffer[1][2*i].value = res; 
			buffer[2][2*i].key = E[0][2]; 
			buffer[2][2*i].value = res; 
			buffer[3][2*i].key = E[0][3]; 
			buffer[3][2*i].value = res; 
			buffer[4][2*i].key = E[0][4]; 
			buffer[4][2*i].value = res; 
			buffer[5][2*i].key = E[0][5]; 
			buffer[5][2*i].value = res; 
			buffer[6][2*i].key = E[0][6]; 
			buffer[6][2*i].value = res; 
			buffer[7][2*i].key = E[0][7]; 
			buffer[7][2*i].value = res; 
			buffer[0][2*i + 1].key = E[1][0]; 
			buffer[0][2*i + 1].value = res; 
			buffer[1][2*i + 1].key = E[1][1]; 
			buffer[1][2*i + 1].value = res; 
			buffer[2][2*i + 1].key = E[1][2]; 
			buffer[2][2*i + 1].value = res; 
			buffer[3][2*i + 1].key = E[1][3]; 
			buffer[3][2*i + 1].value = res; 
			buffer[4][2*i + 1].key = E[1][4]; 
			buffer[4][2*i + 1].value = res; 
			buffer[5][2*i + 1].key = E[1][5]; 
			buffer[5][2*i + 1].value = res; 
			buffer[6][2*i + 1].key = E[1][6]; 
			buffer[6][2*i + 1].value = res; 
			buffer[7][2*i + 1].key = E[1][7]; 
			buffer[7][2*i + 1].value = res; 
		} else {
			buffer[0][2*i].key = INVALIDDATA; 
			buffer[0][2*i].value = INVALIDDATA; 
			buffer[1][2*i].key = INVALIDDATA; 
			buffer[1][2*i].value = INVALIDDATA; 
			buffer[2][2*i].key = INVALIDDATA; 
			buffer[2][2*i].value = INVALIDDATA; 
			buffer[3][2*i].key = INVALIDDATA; 
			buffer[3][2*i].value = INVALIDDATA; 
			buffer[4][2*i].key = INVALIDDATA; 
			buffer[4][2*i].value = INVALIDDATA; 
			buffer[5][2*i].key = INVALIDDATA; 
			buffer[5][2*i].value = INVALIDDATA; 
			buffer[6][2*i].key = INVALIDDATA; 
			buffer[6][2*i].value = INVALIDDATA; 
			buffer[7][2*i].key = INVALIDDATA; 
			buffer[7][2*i].value = INVALIDDATA; 
			buffer[0][2*i + 1].key = INVALIDDATA; 
			buffer[0][2*i + 1].value = INVALIDDATA; 
			buffer[1][2*i + 1].key = INVALIDDATA; 
			buffer[1][2*i + 1].value = INVALIDDATA; 
			buffer[2][2*i + 1].key = INVALIDDATA; 
			buffer[2][2*i + 1].value = INVALIDDATA; 
			buffer[3][2*i + 1].key = INVALIDDATA; 
			buffer[3][2*i + 1].value = INVALIDDATA; 
			buffer[4][2*i + 1].key = INVALIDDATA; 
			buffer[4][2*i + 1].value = INVALIDDATA; 
			buffer[5][2*i + 1].key = INVALIDDATA; 
			buffer[5][2*i + 1].value = INVALIDDATA; 
			buffer[6][2*i + 1].key = INVALIDDATA; 
			buffer[6][2*i + 1].value = INVALIDDATA; 
			buffer[7][2*i + 1].key = INVALIDDATA; 
			buffer[7][2*i + 1].value = INVALIDDATA; 
		}
		
		buffer[0][2*i].key = INVALIDDATA;
		buffer[0][2*i].value = INVALIDDATA;
		
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsprocessed(VECTOR_SIZE*2);
		if(en == ON && mask == 1){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(VECTOR_SIZE*2); }
		#endif 
	}
	return validfetch;
}
#endif
bool_type 
	#ifdef SW 
	acts::
	#endif 
readandprocess(bool_type enable, uint512_dt * kvdram, keyvalue16_type vbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask[BLOCKRAM_SIZE], keyvalue_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], 
		batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, sweepparams_t sweepparams, globalparams_t globalparams){
	
	if(enable == OFF){ return OFF; }
	analysis_type analysis_srcbuffersz = WORKBUFFER_SIZE / 2;
	
	keyvalue_t tempvbuffer[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = tempvbuffer
	
	value_t E[2][VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=E complete
	
	bool_type en = ON;
	bool_type validfetch = OFF;
	buffer_type reducebuffersz = globalparams.SIZE_REDUCE / 2;
	unsigned int validbound = reducebuffersz * FETFACTOR * VECTOR2_SIZE;
	
	travstate.i_kvs = travstate.i_kvs / 2; // edges is singlevaluetype
	travstate.end_kvs = travstate.end_kvs / 2;
	loffset_kvs = loffset_kvs / 2;
	size_kvs = size_kvs / 2;
	
	readkeyvalues(ON, kvdram, tempvbuffer, goffset_kvs + loffset_kvs, size_kvs, travstate, globalparams);
	
	buffer_type chunk_size = getchunksize_kvs(size_kvs, travstate, 0);
	READANDPROCESS_LOOP1: for (buffer_type i=0; i<chunk_size; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
		vertex_t srcvid = tempvbuffer[0][i].key;
		
		READANDPROCESS_LOOP1B: for (unsigned int n=0; n<2; n++){
		#pragma HLS PIPELINE II=2
			E[0][0] = tempvbuffer[4*n + 0][i].key;
			E[0][1] = tempvbuffer[4*n + 0][i].value;
			E[0][2] = tempvbuffer[4*n + 1][i].key;
			E[0][3] = tempvbuffer[4*n + 1][i].value;
			E[0][4] = tempvbuffer[4*n + 2][i].key;
			E[0][5] = tempvbuffer[4*n + 2][i].value;
			E[0][6] = tempvbuffer[4*n + 3][i].key;
			E[0][7] = tempvbuffer[4*n + 3][i].value;
			
			en = ON;
			
			vertex_t lvid = srcvid - travstate.i2;
			if(lvid >= validbound || srcvid == UNUSEDDATA){ en = OFF; lvid = 0; }
			#ifdef _DEBUGMODE_CHECKS2
			if(srcvid < travstate.i2){ cout<<"readandprocess: INVALID srcvid. this is an error. srcvid: "<<srcvid<<", travstate.i2: "<<travstate.i2<<". exiting..."<<endl; exit(EXIT_FAILURE); }
			actsutilityobj->checkoutofbounds("readandprocess.1", lvid, REDUCEBUFFERSZ * FETFACTOR * VECTOR2_SIZE, srcvid, travstate.i2, NAp);
			#endif
			
			value_t udata = readfromvbuffer(vbuffer, lvid, globalparams);
			#ifdef ALLVERTEXISACTIVE_ALGORITHM
			unsigned int mask = 1;
			#else 
			unsigned int mask = readfromvmask(vmask, lvid, globalparams);
			#endif 
			
			if(mask == 1){ validfetch = ON; }
			value_t res  = processfunc(udata, 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID); 
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("readandprocess.1", mask, 2, NAp, NAp, NAp);
			#endif
			
			if(en == ON && mask == 1){
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(false){ cout<<"readandprocess: i: "<<i<<", mask: "<<mask<<", srcvid: "<<srcvid<<", travstate.i2: "<<travstate.i2<<", lvid: "<<lvid<<", udata: "<<udata<<endl; }
				for(unsigned int v=0; v<VECTOR_SIZE; v++){ cout<<"readandprocess: udata: "<<udata<<", E[0]["<<v<<"]: "<<E[0][v]<<", sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }
				// for(unsigned int v=0; v<VECTOR_SIZE; v++){ cout<<"readandprocess: udata: "<<udata<<", E[1]["<<v<<"]: "<<E[1][v]<<", sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }
				#endif 
				
				buffer[0][2*i + n].key = E[0][0]; 
				buffer[0][2*i + n].value = res; 
				buffer[1][2*i + n].key = E[0][1]; 
				buffer[1][2*i + n].value = res; 
				buffer[2][2*i + n].key = E[0][2]; 
				buffer[2][2*i + n].value = res; 
				buffer[3][2*i + n].key = E[0][3]; 
				buffer[3][2*i + n].value = res; 
				buffer[4][2*i + n].key = E[0][4]; 
				buffer[4][2*i + n].value = res; 
				buffer[5][2*i + n].key = E[0][5]; 
				buffer[5][2*i + n].value = res; 
				buffer[6][2*i + n].key = E[0][6]; 
				buffer[6][2*i + n].value = res; 
				buffer[7][2*i + n].key = E[0][7]; 
				buffer[7][2*i + n].value = res; 
			} else {
				buffer[0][2*i + n].key = INVALIDDATA; 
				buffer[0][2*i + n].value = INVALIDDATA; 
				buffer[1][2*i + n].key = INVALIDDATA; 
				buffer[1][2*i + n].value = INVALIDDATA; 
				buffer[2][2*i + n].key = INVALIDDATA; 
				buffer[2][2*i + n].value = INVALIDDATA; 
				buffer[3][2*i + n].key = INVALIDDATA; 
				buffer[3][2*i + n].value = INVALIDDATA; 
				buffer[4][2*i + n].key = INVALIDDATA; 
				buffer[4][2*i + n].value = INVALIDDATA; 
				buffer[5][2*i + n].key = INVALIDDATA; 
				buffer[5][2*i + n].value = INVALIDDATA; 
				buffer[6][2*i + n].key = INVALIDDATA; 
				buffer[6][2*i + n].value = INVALIDDATA; 
				buffer[7][2*i + n].key = INVALIDDATA; 
				buffer[7][2*i + n].value = INVALIDDATA; 
	
			}
			
			if(n==0){
				buffer[0][2*i].key = INVALIDDATA;
				buffer[0][2*i].value = INVALIDDATA;
			}
			
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countkvsprocessed(VECTOR_SIZE);
			if(en == ON && mask == 1){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(VECTOR_SIZE); }
			#endif 
		}
	}
	return validfetch;
}

// functions (partition)
void 
	#ifdef SW 
	acts::
	#endif 
readglobalstats(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[NUM_PARTITIONS], batch_type offset_kvs, globalparams_t globalparams){ 
	if(enable == OFF){ return; }
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("readglobalstats", offset_kvs + NUM_PARTITIONS, globalparams.BASEOFFSETKVS_STATSDRAM + KVSTATSDRAMSZ_KVS + 1, NAp, NAp, NAp);
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
saveglobalstats(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[NUM_PARTITIONS], batch_type offset_kvs, globalparams_t globalparams){ 
	if(enable == OFF){ return; }
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("saveglobalstats", offset_kvs + NUM_PARTITIONS, globalparams.BASEOFFSETKVS_STATSDRAM + KVSTATSDRAMSZ_KVS + 1, offset_kvs, NUM_PARTITIONS, KVSTATSDRAMSZ_KVS);
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

buffer_type 
	#ifdef SW 
	acts::
	#endif
preparekeyvalues(bool_type enable1, bool_type enable2, keyvalue_t sourcebuffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t localcapsule[VECTOR_SIZE][NUM_PARTITIONS], step_type currentLOP, sweepparams_t sweepparams, travstate_t travstate, buffer_type size_kvs, buffer_type cutoffs[VECTOR_SIZE], globalparams_t globalparams){
	if(enable1 == OFF && enable2 == OFF){ return 0; }
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
		if(valid0 == ON){ p0 = getpartition(ON, keyvalue0, currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE); }
		partition_type p1 = 0;
		if(valid1 == ON){ p1 = getpartition(ON, keyvalue1, currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE); }
		partition_type p2 = 0;
		if(valid2 == ON){ p2 = getpartition(ON, keyvalue2, currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE); }
		partition_type p3 = 0;
		if(valid3 == ON){ p3 = getpartition(ON, keyvalue3, currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE); }
		partition_type p4 = 0;
		if(valid4 == ON){ p4 = getpartition(ON, keyvalue4, currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE); }
		partition_type p5 = 0;
		if(valid5 == ON){ p5 = getpartition(ON, keyvalue5, currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE); }
		partition_type p6 = 0;
		if(valid6 == ON){ p6 = getpartition(ON, keyvalue6, currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE); }
		partition_type p7 = 0;
		if(valid7 == ON){ p7 = getpartition(ON, keyvalue7, currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE); }
		
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
		}
	}
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printkeyvalues("preparekeyvalues.localcapsule[0].2", (keyvalue_t *)localcapsule[0], NUM_PARTITIONS);
	#endif
	return SRCBUFFER_SIZE;
}

// functions (reduce)
value_t 
	#ifdef SW 
	acts::
	#endif 
reducefunc(value_t vtemp, value_t res, unsigned int GraphIter, unsigned int GraphAlgo){
	value_t temp = 0;
	#ifdef PR_ALGORITHM
	temp = vtemp + res;
	#endif 
	#ifdef BFS_ALGORITHM
	// temp = amin(vtemp, GraphIter); // BFS
	temp = amin(vtemp, res);
	#endif 
	#ifdef SSSP_ALGORITHM
	temp = amin(vtemp, res);
	#endif
	return temp;
}

void 
	#ifdef SW 
	acts::
	#endif
reducevector(keyvalue_t kvdata, keyvalue16_type destbuffer[BLOCKRAM_SIZE], buffer_type destoffset, unsigned int upperlimit, sweepparams_t sweepparams, globalparams_t globalparams){
	#ifdef VERTEXCOLORING
	#pragma HLS PIPELINE II=2
	#else 
	#pragma HLS PIPELINE II=3	
	#endif 
	analysis_type analysis_loop1 = VECTOR_SIZE;
		
	keyvalue_t keyvalue = kvdata;
	vertex_t loc = keyvalue.key - upperlimit;
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
	if(keyvalue.key != INVALIDDATA && keyvalue.value != INVALIDDATA){ cout<<"REDUCE SEEN @ reducevector:: vid: "<<upperlimit + loc<<", loc: "<<loc<<", keyvalue.key: "<<keyvalue.key<<", keyvalue.value: "<<keyvalue.value<<", upperlimit: "<<upperlimit<<", reduce size: "<<globalparams.SIZE_REDUCE<<endl; }
	#endif 
	
	if(loc >= globalparams.SIZE_REDUCE && keyvalue.key != INVALIDDATA && keyvalue.value != INVALIDDATA){ 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->reducehelper_checkreduceloc(0, loc, keyvalue, sweepparams, globalparams); 
		#endif 
		loc = 0; }

	vertex_t rowindex = loc / 2;
	vertex_t colindex = loc % 2;
	
	#ifdef VERTEXCOLORING
	if(keyvalue.key == INVALIDDATA || keyvalue.value == INVALIDDATA){ rowindex = 0; colindex = 0; }
	if(colindex == 0){ destbuffer[destoffset + rowindex].key = 1; }
	else { destbuffer[destoffset + rowindex].value = 1; }
	#else
	keyvalue16_type vprop;
	if(keyvalue.key != INVALIDDATA && keyvalue.value != INVALIDDATA){ vprop = destbuffer[destoffset + rowindex]; }
	
	value_t temp;
	if(colindex == 0){ temp = vprop.key; }
	else { temp = vprop.value; }
	
	value_t rettemp = reducefunc(temp, keyvalue.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(keyvalue.key != INVALIDDATA && keyvalue.value != INVALIDDATA){ cout<<"REDUCEFUNC RESULT @ reducevector:: rettemp: "<<rettemp<<", temp: "<<temp<<", keyvalue.value: "<<keyvalue.value<<", NAp: "<<NAp<<endl; }
	#endif 
	
	if(colindex == 0){ vprop.key = rettemp; }
	else { vprop.value = rettemp; }
	
	if(keyvalue.key != INVALIDDATA && keyvalue.value != INVALIDDATA){ destbuffer[destoffset + rowindex] = vprop; }
	#endif 
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->globalstats_countkvsreduced(1);
	if(keyvalue.key != INVALIDDATA && keyvalue.value != INVALIDDATA){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(1); }
	#endif
	return;
}

void 
	#ifdef SW 
	acts::
	#endif 
reduceandbuffer(bool_type enable, keyvalue_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t localcapsule[NUM_PARTITIONS], keyvalue16_type vbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], sweepparams_t sweepparams, globalparams_t globalparams){				
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount = (BLOCKRAM_SIZE / (NUM_PARTITIONS / 2)); // '2' is safety padding.
	
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
	actsutilityobj->printkeyvalues("reduceandbuffer.localcapsule", (keyvalue_t *)localcapsule, NUM_PARTITIONS);
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
	#pragma HLS PIPELINE II=16
		for(vector_type v=0; v<VECTOR_SIZE; v++){
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
	

			if(i< size_kvs[0]){ reducevector(kvdata0, vbuffer[0], 0, upperlimits[0], sweepparams, globalparams); }
			if(i< size_kvs[1]){ reducevector(kvdata1, vbuffer[1], 0, upperlimits[1], sweepparams, globalparams); }
			if(i< size_kvs[2]){ reducevector(kvdata2, vbuffer[2], 0, upperlimits[2], sweepparams, globalparams); }
			if(i< size_kvs[3]){ reducevector(kvdata3, vbuffer[3], 0, upperlimits[3], sweepparams, globalparams); }
			if(i< size_kvs[4]){ reducevector(kvdata4, vbuffer[4], 0, upperlimits[4], sweepparams, globalparams); }
			if(i< size_kvs[5]){ reducevector(kvdata5, vbuffer[5], 0, upperlimits[5], sweepparams, globalparams); }
			if(i< size_kvs[6]){ reducevector(kvdata6, vbuffer[6], 0, upperlimits[6], sweepparams, globalparams); }
			if(i< size_kvs[7]){ reducevector(kvdata7, vbuffer[7], 0, upperlimits[7], sweepparams, globalparams); }
			if(i< size_kvs[8]){ reducevector(kvdata8, vbuffer[8], 0, upperlimits[8], sweepparams, globalparams); }
			if(i< size_kvs[9]){ reducevector(kvdata9, vbuffer[9], 0, upperlimits[9], sweepparams, globalparams); }
			if(i< size_kvs[10]){ reducevector(kvdata10, vbuffer[10], 0, upperlimits[10], sweepparams, globalparams); }
			if(i< size_kvs[11]){ reducevector(kvdata11, vbuffer[11], 0, upperlimits[11], sweepparams, globalparams); }
			if(i< size_kvs[12]){ reducevector(kvdata12, vbuffer[12], 0, upperlimits[12], sweepparams, globalparams); }
			if(i< size_kvs[13]){ reducevector(kvdata13, vbuffer[13], 0, upperlimits[13], sweepparams, globalparams); }
			if(i< size_kvs[14]){ reducevector(kvdata14, vbuffer[14], 0, upperlimits[14], sweepparams, globalparams); }
			if(i< size_kvs[15]){ reducevector(kvdata15, vbuffer[15], 0, upperlimits[15], sweepparams, globalparams); }
		}
	}
	return;
}

// functions (synchronize)
value_t 
	#ifdef SW 
	acts::
	#endif 
applyfunc(value_t vtemp, value_t res, unsigned int GraphIter, unsigned int GraphAlgo){
	value_t temp = 0;
	#ifdef PR_ALGORITHM
	temp = vtemp + res;
	#endif
	#ifdef BFS_ALGORITHM
	// temp = amin(vtemp, GraphIter); // BFS
	temp = amin(vtemp, res);
	#endif
	#ifdef SSSP_ALGORITHM
	temp = amin(vtemp, res);
	#endif
	return temp;
}

value_t 
	#ifdef SW 
	acts::
	#endif 
mergefunc(value_t value1, value_t value2, unsigned int GraphAlgo){
	value_t res = 0;
	#ifdef PR_ALGORITHM
	res = value1 + value2;
	#endif 
	#ifdef BFS_ALGORITHM
	res = amin(value1, value2); // BFS
	#endif 
	#ifdef SSSP_ALGORITHM
	res = amin(value1, value2);
	#endif
	return res;
}

void
	#ifdef SW 
	acts::
	#endif
synchronize(bool_type enable, keyvalue16_type buffer0[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue16_type buffer1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue16_type buffer2[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue16_type buffer3[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue16_type buffer4[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue16_type buffer5[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue16_type buffer6[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue16_type buffer7[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue16_type buffer8[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue16_type buffer9[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue16_type buffer10[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue16_type buffer11[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue16_type buffer12[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue16_type buffer13[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue16_type buffer14[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue16_type buffer15[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue16_type buffer16[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue16_type tail[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], globalparams_t globalparams){
	if(enable == OFF){ return; }
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
	cout<<"synchronize: synchronize function called."<<endl;
	#endif 
	analysis_type analysis_loopcount = REDUCEBUFFERSZ;
	
	buffer_type reducebuffersz = globalparams.SIZE_REDUCE / 2;
	
	keyvalue16_type keyvalue0_vault0;
	keyvalue16_type keyvalue1_vault0;
	keyvalue16_type keyvalue2_vault0;
	keyvalue16_type keyvalue3_vault0;
	keyvalue16_type keyvalue4_vault0;
	keyvalue16_type keyvalue5_vault0;
	keyvalue16_type keyvalue6_vault0;
	keyvalue16_type keyvalue7_vault0;
	keyvalue16_type keyvalue8_vault0;
	keyvalue16_type keyvalue9_vault0;
	keyvalue16_type keyvalue10_vault0;
	keyvalue16_type keyvalue11_vault0;
	keyvalue16_type keyvalue12_vault0;
	keyvalue16_type keyvalue13_vault0;
	keyvalue16_type keyvalue14_vault0;
	keyvalue16_type keyvalue15_vault0;
	
	keyvalue16_type keyvalue0_vault1;
	keyvalue16_type keyvalue1_vault1;
	keyvalue16_type keyvalue2_vault1;
	keyvalue16_type keyvalue3_vault1;
	keyvalue16_type keyvalue4_vault1;
	keyvalue16_type keyvalue5_vault1;
	keyvalue16_type keyvalue6_vault1;
	keyvalue16_type keyvalue7_vault1;
	keyvalue16_type keyvalue8_vault1;
	keyvalue16_type keyvalue9_vault1;
	keyvalue16_type keyvalue10_vault1;
	keyvalue16_type keyvalue11_vault1;
	keyvalue16_type keyvalue12_vault1;
	keyvalue16_type keyvalue13_vault1;
	keyvalue16_type keyvalue14_vault1;
	keyvalue16_type keyvalue15_vault1;
	
	keyvalue16_type keyvalue0_vault2;
	keyvalue16_type keyvalue1_vault2;
	keyvalue16_type keyvalue2_vault2;
	keyvalue16_type keyvalue3_vault2;
	keyvalue16_type keyvalue4_vault2;
	keyvalue16_type keyvalue5_vault2;
	keyvalue16_type keyvalue6_vault2;
	keyvalue16_type keyvalue7_vault2;
	keyvalue16_type keyvalue8_vault2;
	keyvalue16_type keyvalue9_vault2;
	keyvalue16_type keyvalue10_vault2;
	keyvalue16_type keyvalue11_vault2;
	keyvalue16_type keyvalue12_vault2;
	keyvalue16_type keyvalue13_vault2;
	keyvalue16_type keyvalue14_vault2;
	keyvalue16_type keyvalue15_vault2;
	
	keyvalue16_type keyvalue0_vault3;
	keyvalue16_type keyvalue1_vault3;
	keyvalue16_type keyvalue2_vault3;
	keyvalue16_type keyvalue3_vault3;
	keyvalue16_type keyvalue4_vault3;
	keyvalue16_type keyvalue5_vault3;
	keyvalue16_type keyvalue6_vault3;
	keyvalue16_type keyvalue7_vault3;
	keyvalue16_type keyvalue8_vault3;
	keyvalue16_type keyvalue9_vault3;
	keyvalue16_type keyvalue10_vault3;
	keyvalue16_type keyvalue11_vault3;
	keyvalue16_type keyvalue12_vault3;
	keyvalue16_type keyvalue13_vault3;
	keyvalue16_type keyvalue14_vault3;
	keyvalue16_type keyvalue15_vault3;
	
	keyvalue16_type keyvalue0_vault4;
	keyvalue16_type keyvalue1_vault4;
	keyvalue16_type keyvalue2_vault4;
	keyvalue16_type keyvalue3_vault4;
	keyvalue16_type keyvalue4_vault4;
	keyvalue16_type keyvalue5_vault4;
	keyvalue16_type keyvalue6_vault4;
	keyvalue16_type keyvalue7_vault4;
	keyvalue16_type keyvalue8_vault4;
	keyvalue16_type keyvalue9_vault4;
	keyvalue16_type keyvalue10_vault4;
	keyvalue16_type keyvalue11_vault4;
	keyvalue16_type keyvalue12_vault4;
	keyvalue16_type keyvalue13_vault4;
	keyvalue16_type keyvalue14_vault4;
	keyvalue16_type keyvalue15_vault4;
	
	keyvalue16_type keyvalue0_vault5;
	keyvalue16_type keyvalue1_vault5;
	keyvalue16_type keyvalue2_vault5;
	keyvalue16_type keyvalue3_vault5;
	keyvalue16_type keyvalue4_vault5;
	keyvalue16_type keyvalue5_vault5;
	keyvalue16_type keyvalue6_vault5;
	keyvalue16_type keyvalue7_vault5;
	keyvalue16_type keyvalue8_vault5;
	keyvalue16_type keyvalue9_vault5;
	keyvalue16_type keyvalue10_vault5;
	keyvalue16_type keyvalue11_vault5;
	keyvalue16_type keyvalue12_vault5;
	keyvalue16_type keyvalue13_vault5;
	keyvalue16_type keyvalue14_vault5;
	keyvalue16_type keyvalue15_vault5;
	
	keyvalue16_type keyvalue0_vault6;
	keyvalue16_type keyvalue1_vault6;
	keyvalue16_type keyvalue2_vault6;
	keyvalue16_type keyvalue3_vault6;
	keyvalue16_type keyvalue4_vault6;
	keyvalue16_type keyvalue5_vault6;
	keyvalue16_type keyvalue6_vault6;
	keyvalue16_type keyvalue7_vault6;
	keyvalue16_type keyvalue8_vault6;
	keyvalue16_type keyvalue9_vault6;
	keyvalue16_type keyvalue10_vault6;
	keyvalue16_type keyvalue11_vault6;
	keyvalue16_type keyvalue12_vault6;
	keyvalue16_type keyvalue13_vault6;
	keyvalue16_type keyvalue14_vault6;
	keyvalue16_type keyvalue15_vault6;
	
	keyvalue16_type keyvalue0_vault7;
	keyvalue16_type keyvalue1_vault7;
	keyvalue16_type keyvalue2_vault7;
	keyvalue16_type keyvalue3_vault7;
	keyvalue16_type keyvalue4_vault7;
	keyvalue16_type keyvalue5_vault7;
	keyvalue16_type keyvalue6_vault7;
	keyvalue16_type keyvalue7_vault7;
	keyvalue16_type keyvalue8_vault7;
	keyvalue16_type keyvalue9_vault7;
	keyvalue16_type keyvalue10_vault7;
	keyvalue16_type keyvalue11_vault7;
	keyvalue16_type keyvalue12_vault7;
	keyvalue16_type keyvalue13_vault7;
	keyvalue16_type keyvalue14_vault7;
	keyvalue16_type keyvalue15_vault7;
	
	keyvalue16_type keyvalue0_vault8;
	keyvalue16_type keyvalue1_vault8;
	keyvalue16_type keyvalue2_vault8;
	keyvalue16_type keyvalue3_vault8;
	keyvalue16_type keyvalue4_vault8;
	keyvalue16_type keyvalue5_vault8;
	keyvalue16_type keyvalue6_vault8;
	keyvalue16_type keyvalue7_vault8;
	keyvalue16_type keyvalue8_vault8;
	keyvalue16_type keyvalue9_vault8;
	keyvalue16_type keyvalue10_vault8;
	keyvalue16_type keyvalue11_vault8;
	keyvalue16_type keyvalue12_vault8;
	keyvalue16_type keyvalue13_vault8;
	keyvalue16_type keyvalue14_vault8;
	keyvalue16_type keyvalue15_vault8;
	
	keyvalue16_type keyvalue0_vault9;
	keyvalue16_type keyvalue1_vault9;
	keyvalue16_type keyvalue2_vault9;
	keyvalue16_type keyvalue3_vault9;
	keyvalue16_type keyvalue4_vault9;
	keyvalue16_type keyvalue5_vault9;
	keyvalue16_type keyvalue6_vault9;
	keyvalue16_type keyvalue7_vault9;
	keyvalue16_type keyvalue8_vault9;
	keyvalue16_type keyvalue9_vault9;
	keyvalue16_type keyvalue10_vault9;
	keyvalue16_type keyvalue11_vault9;
	keyvalue16_type keyvalue12_vault9;
	keyvalue16_type keyvalue13_vault9;
	keyvalue16_type keyvalue14_vault9;
	keyvalue16_type keyvalue15_vault9;
	
	keyvalue16_type keyvalue0_vault10;
	keyvalue16_type keyvalue1_vault10;
	keyvalue16_type keyvalue2_vault10;
	keyvalue16_type keyvalue3_vault10;
	keyvalue16_type keyvalue4_vault10;
	keyvalue16_type keyvalue5_vault10;
	keyvalue16_type keyvalue6_vault10;
	keyvalue16_type keyvalue7_vault10;
	keyvalue16_type keyvalue8_vault10;
	keyvalue16_type keyvalue9_vault10;
	keyvalue16_type keyvalue10_vault10;
	keyvalue16_type keyvalue11_vault10;
	keyvalue16_type keyvalue12_vault10;
	keyvalue16_type keyvalue13_vault10;
	keyvalue16_type keyvalue14_vault10;
	keyvalue16_type keyvalue15_vault10;
	
	keyvalue16_type keyvalue0_vault11;
	keyvalue16_type keyvalue1_vault11;
	keyvalue16_type keyvalue2_vault11;
	keyvalue16_type keyvalue3_vault11;
	keyvalue16_type keyvalue4_vault11;
	keyvalue16_type keyvalue5_vault11;
	keyvalue16_type keyvalue6_vault11;
	keyvalue16_type keyvalue7_vault11;
	keyvalue16_type keyvalue8_vault11;
	keyvalue16_type keyvalue9_vault11;
	keyvalue16_type keyvalue10_vault11;
	keyvalue16_type keyvalue11_vault11;
	keyvalue16_type keyvalue12_vault11;
	keyvalue16_type keyvalue13_vault11;
	keyvalue16_type keyvalue14_vault11;
	keyvalue16_type keyvalue15_vault11;
	
	keyvalue16_type keyvalue0_vault12;
	keyvalue16_type keyvalue1_vault12;
	keyvalue16_type keyvalue2_vault12;
	keyvalue16_type keyvalue3_vault12;
	keyvalue16_type keyvalue4_vault12;
	keyvalue16_type keyvalue5_vault12;
	keyvalue16_type keyvalue6_vault12;
	keyvalue16_type keyvalue7_vault12;
	keyvalue16_type keyvalue8_vault12;
	keyvalue16_type keyvalue9_vault12;
	keyvalue16_type keyvalue10_vault12;
	keyvalue16_type keyvalue11_vault12;
	keyvalue16_type keyvalue12_vault12;
	keyvalue16_type keyvalue13_vault12;
	keyvalue16_type keyvalue14_vault12;
	keyvalue16_type keyvalue15_vault12;
	
	keyvalue16_type keyvalue0_vault13;
	keyvalue16_type keyvalue1_vault13;
	keyvalue16_type keyvalue2_vault13;
	keyvalue16_type keyvalue3_vault13;
	keyvalue16_type keyvalue4_vault13;
	keyvalue16_type keyvalue5_vault13;
	keyvalue16_type keyvalue6_vault13;
	keyvalue16_type keyvalue7_vault13;
	keyvalue16_type keyvalue8_vault13;
	keyvalue16_type keyvalue9_vault13;
	keyvalue16_type keyvalue10_vault13;
	keyvalue16_type keyvalue11_vault13;
	keyvalue16_type keyvalue12_vault13;
	keyvalue16_type keyvalue13_vault13;
	keyvalue16_type keyvalue14_vault13;
	keyvalue16_type keyvalue15_vault13;
	
	keyvalue16_type keyvalue0_vault14;
	keyvalue16_type keyvalue1_vault14;
	keyvalue16_type keyvalue2_vault14;
	keyvalue16_type keyvalue3_vault14;
	keyvalue16_type keyvalue4_vault14;
	keyvalue16_type keyvalue5_vault14;
	keyvalue16_type keyvalue6_vault14;
	keyvalue16_type keyvalue7_vault14;
	keyvalue16_type keyvalue8_vault14;
	keyvalue16_type keyvalue9_vault14;
	keyvalue16_type keyvalue10_vault14;
	keyvalue16_type keyvalue11_vault14;
	keyvalue16_type keyvalue12_vault14;
	keyvalue16_type keyvalue13_vault14;
	keyvalue16_type keyvalue14_vault14;
	keyvalue16_type keyvalue15_vault14;
	
	keyvalue16_type keyvalue0_vault15;
	keyvalue16_type keyvalue1_vault15;
	keyvalue16_type keyvalue2_vault15;
	keyvalue16_type keyvalue3_vault15;
	keyvalue16_type keyvalue4_vault15;
	keyvalue16_type keyvalue5_vault15;
	keyvalue16_type keyvalue6_vault15;
	keyvalue16_type keyvalue7_vault15;
	keyvalue16_type keyvalue8_vault15;
	keyvalue16_type keyvalue9_vault15;
	keyvalue16_type keyvalue10_vault15;
	keyvalue16_type keyvalue11_vault15;
	keyvalue16_type keyvalue12_vault15;
	keyvalue16_type keyvalue13_vault15;
	keyvalue16_type keyvalue14_vault15;
	keyvalue16_type keyvalue15_vault15;
	
	keyvalue16_type keyvalue0_vault16;
	keyvalue16_type keyvalue1_vault16;
	keyvalue16_type keyvalue2_vault16;
	keyvalue16_type keyvalue3_vault16;
	keyvalue16_type keyvalue4_vault16;
	keyvalue16_type keyvalue5_vault16;
	keyvalue16_type keyvalue6_vault16;
	keyvalue16_type keyvalue7_vault16;
	keyvalue16_type keyvalue8_vault16;
	keyvalue16_type keyvalue9_vault16;
	keyvalue16_type keyvalue10_vault16;
	keyvalue16_type keyvalue11_vault16;
	keyvalue16_type keyvalue12_vault16;
	keyvalue16_type keyvalue13_vault16;
	keyvalue16_type keyvalue14_vault16;
	keyvalue16_type keyvalue15_vault16;
	
	keyvalue16_type keyvalue0_vault17;
	keyvalue16_type keyvalue1_vault17;
	keyvalue16_type keyvalue2_vault17;
	keyvalue16_type keyvalue3_vault17;
	keyvalue16_type keyvalue4_vault17;
	keyvalue16_type keyvalue5_vault17;
	keyvalue16_type keyvalue6_vault17;
	keyvalue16_type keyvalue7_vault17;
	keyvalue16_type keyvalue8_vault17;
	keyvalue16_type keyvalue9_vault17;
	keyvalue16_type keyvalue10_vault17;
	keyvalue16_type keyvalue11_vault17;
	keyvalue16_type keyvalue12_vault17;
	keyvalue16_type keyvalue13_vault17;
	keyvalue16_type keyvalue14_vault17;
	keyvalue16_type keyvalue15_vault17;
	
	
	for (buffer_type i=0; i<reducebuffersz; i++){ // 8, 16, BLOCKRAM_SIZE
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE II=1
		keyvalue0_vault1 = buffer0[0][i]; 
		keyvalue1_vault1 = buffer0[1][i]; 
		keyvalue2_vault1 = buffer0[2][i]; 
		keyvalue3_vault1 = buffer0[3][i]; 
		keyvalue4_vault1 = buffer0[4][i]; 
		keyvalue5_vault1 = buffer0[5][i]; 
		keyvalue6_vault1 = buffer0[6][i]; 
		keyvalue7_vault1 = buffer0[7][i]; 
		keyvalue8_vault1 = buffer0[8][i]; 
		keyvalue9_vault1 = buffer0[9][i]; 
		keyvalue10_vault1 = buffer0[10][i]; 
		keyvalue11_vault1 = buffer0[11][i]; 
		keyvalue12_vault1 = buffer0[12][i]; 
		keyvalue13_vault1 = buffer0[13][i]; 
		keyvalue14_vault1 = buffer0[14][i]; 
		keyvalue15_vault1 = buffer0[15][i]; 
			
		keyvalue0_vault2.key = mergefunc(keyvalue0_vault1.key, buffer1[0][i].key, NAp);
		keyvalue0_vault2.value = mergefunc(keyvalue0_vault1.value, buffer1[0][i].value, NAp);
		keyvalue1_vault2.key = mergefunc(keyvalue1_vault1.key, buffer1[1][i].key, NAp);
		keyvalue1_vault2.value = mergefunc(keyvalue1_vault1.value, buffer1[1][i].value, NAp);
		keyvalue2_vault2.key = mergefunc(keyvalue2_vault1.key, buffer1[2][i].key, NAp);
		keyvalue2_vault2.value = mergefunc(keyvalue2_vault1.value, buffer1[2][i].value, NAp);
		keyvalue3_vault2.key = mergefunc(keyvalue3_vault1.key, buffer1[3][i].key, NAp);
		keyvalue3_vault2.value = mergefunc(keyvalue3_vault1.value, buffer1[3][i].value, NAp);
		keyvalue4_vault2.key = mergefunc(keyvalue4_vault1.key, buffer1[4][i].key, NAp);
		keyvalue4_vault2.value = mergefunc(keyvalue4_vault1.value, buffer1[4][i].value, NAp);
		keyvalue5_vault2.key = mergefunc(keyvalue5_vault1.key, buffer1[5][i].key, NAp);
		keyvalue5_vault2.value = mergefunc(keyvalue5_vault1.value, buffer1[5][i].value, NAp);
		keyvalue6_vault2.key = mergefunc(keyvalue6_vault1.key, buffer1[6][i].key, NAp);
		keyvalue6_vault2.value = mergefunc(keyvalue6_vault1.value, buffer1[6][i].value, NAp);
		keyvalue7_vault2.key = mergefunc(keyvalue7_vault1.key, buffer1[7][i].key, NAp);
		keyvalue7_vault2.value = mergefunc(keyvalue7_vault1.value, buffer1[7][i].value, NAp);
		keyvalue8_vault2.key = mergefunc(keyvalue8_vault1.key, buffer1[8][i].key, NAp);
		keyvalue8_vault2.value = mergefunc(keyvalue8_vault1.value, buffer1[8][i].value, NAp);
		keyvalue9_vault2.key = mergefunc(keyvalue9_vault1.key, buffer1[9][i].key, NAp);
		keyvalue9_vault2.value = mergefunc(keyvalue9_vault1.value, buffer1[9][i].value, NAp);
		keyvalue10_vault2.key = mergefunc(keyvalue10_vault1.key, buffer1[10][i].key, NAp);
		keyvalue10_vault2.value = mergefunc(keyvalue10_vault1.value, buffer1[10][i].value, NAp);
		keyvalue11_vault2.key = mergefunc(keyvalue11_vault1.key, buffer1[11][i].key, NAp);
		keyvalue11_vault2.value = mergefunc(keyvalue11_vault1.value, buffer1[11][i].value, NAp);
		keyvalue12_vault2.key = mergefunc(keyvalue12_vault1.key, buffer1[12][i].key, NAp);
		keyvalue12_vault2.value = mergefunc(keyvalue12_vault1.value, buffer1[12][i].value, NAp);
		keyvalue13_vault2.key = mergefunc(keyvalue13_vault1.key, buffer1[13][i].key, NAp);
		keyvalue13_vault2.value = mergefunc(keyvalue13_vault1.value, buffer1[13][i].value, NAp);
		keyvalue14_vault2.key = mergefunc(keyvalue14_vault1.key, buffer1[14][i].key, NAp);
		keyvalue14_vault2.value = mergefunc(keyvalue14_vault1.value, buffer1[14][i].value, NAp);
		keyvalue15_vault2.key = mergefunc(keyvalue15_vault1.key, buffer1[15][i].key, NAp);
		keyvalue15_vault2.value = mergefunc(keyvalue15_vault1.value, buffer1[15][i].value, NAp);
	
		keyvalue0_vault3.key = mergefunc(keyvalue0_vault2.key, buffer2[0][i].key, NAp);
		keyvalue0_vault3.value = mergefunc(keyvalue0_vault2.value, buffer2[0][i].value, NAp);
		keyvalue1_vault3.key = mergefunc(keyvalue1_vault2.key, buffer2[1][i].key, NAp);
		keyvalue1_vault3.value = mergefunc(keyvalue1_vault2.value, buffer2[1][i].value, NAp);
		keyvalue2_vault3.key = mergefunc(keyvalue2_vault2.key, buffer2[2][i].key, NAp);
		keyvalue2_vault3.value = mergefunc(keyvalue2_vault2.value, buffer2[2][i].value, NAp);
		keyvalue3_vault3.key = mergefunc(keyvalue3_vault2.key, buffer2[3][i].key, NAp);
		keyvalue3_vault3.value = mergefunc(keyvalue3_vault2.value, buffer2[3][i].value, NAp);
		keyvalue4_vault3.key = mergefunc(keyvalue4_vault2.key, buffer2[4][i].key, NAp);
		keyvalue4_vault3.value = mergefunc(keyvalue4_vault2.value, buffer2[4][i].value, NAp);
		keyvalue5_vault3.key = mergefunc(keyvalue5_vault2.key, buffer2[5][i].key, NAp);
		keyvalue5_vault3.value = mergefunc(keyvalue5_vault2.value, buffer2[5][i].value, NAp);
		keyvalue6_vault3.key = mergefunc(keyvalue6_vault2.key, buffer2[6][i].key, NAp);
		keyvalue6_vault3.value = mergefunc(keyvalue6_vault2.value, buffer2[6][i].value, NAp);
		keyvalue7_vault3.key = mergefunc(keyvalue7_vault2.key, buffer2[7][i].key, NAp);
		keyvalue7_vault3.value = mergefunc(keyvalue7_vault2.value, buffer2[7][i].value, NAp);
		keyvalue8_vault3.key = mergefunc(keyvalue8_vault2.key, buffer2[8][i].key, NAp);
		keyvalue8_vault3.value = mergefunc(keyvalue8_vault2.value, buffer2[8][i].value, NAp);
		keyvalue9_vault3.key = mergefunc(keyvalue9_vault2.key, buffer2[9][i].key, NAp);
		keyvalue9_vault3.value = mergefunc(keyvalue9_vault2.value, buffer2[9][i].value, NAp);
		keyvalue10_vault3.key = mergefunc(keyvalue10_vault2.key, buffer2[10][i].key, NAp);
		keyvalue10_vault3.value = mergefunc(keyvalue10_vault2.value, buffer2[10][i].value, NAp);
		keyvalue11_vault3.key = mergefunc(keyvalue11_vault2.key, buffer2[11][i].key, NAp);
		keyvalue11_vault3.value = mergefunc(keyvalue11_vault2.value, buffer2[11][i].value, NAp);
		keyvalue12_vault3.key = mergefunc(keyvalue12_vault2.key, buffer2[12][i].key, NAp);
		keyvalue12_vault3.value = mergefunc(keyvalue12_vault2.value, buffer2[12][i].value, NAp);
		keyvalue13_vault3.key = mergefunc(keyvalue13_vault2.key, buffer2[13][i].key, NAp);
		keyvalue13_vault3.value = mergefunc(keyvalue13_vault2.value, buffer2[13][i].value, NAp);
		keyvalue14_vault3.key = mergefunc(keyvalue14_vault2.key, buffer2[14][i].key, NAp);
		keyvalue14_vault3.value = mergefunc(keyvalue14_vault2.value, buffer2[14][i].value, NAp);
		keyvalue15_vault3.key = mergefunc(keyvalue15_vault2.key, buffer2[15][i].key, NAp);
		keyvalue15_vault3.value = mergefunc(keyvalue15_vault2.value, buffer2[15][i].value, NAp);
	
		keyvalue0_vault4.key = mergefunc(keyvalue0_vault3.key, buffer3[0][i].key, NAp);
		keyvalue0_vault4.value = mergefunc(keyvalue0_vault3.value, buffer3[0][i].value, NAp);
		keyvalue1_vault4.key = mergefunc(keyvalue1_vault3.key, buffer3[1][i].key, NAp);
		keyvalue1_vault4.value = mergefunc(keyvalue1_vault3.value, buffer3[1][i].value, NAp);
		keyvalue2_vault4.key = mergefunc(keyvalue2_vault3.key, buffer3[2][i].key, NAp);
		keyvalue2_vault4.value = mergefunc(keyvalue2_vault3.value, buffer3[2][i].value, NAp);
		keyvalue3_vault4.key = mergefunc(keyvalue3_vault3.key, buffer3[3][i].key, NAp);
		keyvalue3_vault4.value = mergefunc(keyvalue3_vault3.value, buffer3[3][i].value, NAp);
		keyvalue4_vault4.key = mergefunc(keyvalue4_vault3.key, buffer3[4][i].key, NAp);
		keyvalue4_vault4.value = mergefunc(keyvalue4_vault3.value, buffer3[4][i].value, NAp);
		keyvalue5_vault4.key = mergefunc(keyvalue5_vault3.key, buffer3[5][i].key, NAp);
		keyvalue5_vault4.value = mergefunc(keyvalue5_vault3.value, buffer3[5][i].value, NAp);
		keyvalue6_vault4.key = mergefunc(keyvalue6_vault3.key, buffer3[6][i].key, NAp);
		keyvalue6_vault4.value = mergefunc(keyvalue6_vault3.value, buffer3[6][i].value, NAp);
		keyvalue7_vault4.key = mergefunc(keyvalue7_vault3.key, buffer3[7][i].key, NAp);
		keyvalue7_vault4.value = mergefunc(keyvalue7_vault3.value, buffer3[7][i].value, NAp);
		keyvalue8_vault4.key = mergefunc(keyvalue8_vault3.key, buffer3[8][i].key, NAp);
		keyvalue8_vault4.value = mergefunc(keyvalue8_vault3.value, buffer3[8][i].value, NAp);
		keyvalue9_vault4.key = mergefunc(keyvalue9_vault3.key, buffer3[9][i].key, NAp);
		keyvalue9_vault4.value = mergefunc(keyvalue9_vault3.value, buffer3[9][i].value, NAp);
		keyvalue10_vault4.key = mergefunc(keyvalue10_vault3.key, buffer3[10][i].key, NAp);
		keyvalue10_vault4.value = mergefunc(keyvalue10_vault3.value, buffer3[10][i].value, NAp);
		keyvalue11_vault4.key = mergefunc(keyvalue11_vault3.key, buffer3[11][i].key, NAp);
		keyvalue11_vault4.value = mergefunc(keyvalue11_vault3.value, buffer3[11][i].value, NAp);
		keyvalue12_vault4.key = mergefunc(keyvalue12_vault3.key, buffer3[12][i].key, NAp);
		keyvalue12_vault4.value = mergefunc(keyvalue12_vault3.value, buffer3[12][i].value, NAp);
		keyvalue13_vault4.key = mergefunc(keyvalue13_vault3.key, buffer3[13][i].key, NAp);
		keyvalue13_vault4.value = mergefunc(keyvalue13_vault3.value, buffer3[13][i].value, NAp);
		keyvalue14_vault4.key = mergefunc(keyvalue14_vault3.key, buffer3[14][i].key, NAp);
		keyvalue14_vault4.value = mergefunc(keyvalue14_vault3.value, buffer3[14][i].value, NAp);
		keyvalue15_vault4.key = mergefunc(keyvalue15_vault3.key, buffer3[15][i].key, NAp);
		keyvalue15_vault4.value = mergefunc(keyvalue15_vault3.value, buffer3[15][i].value, NAp);
	
		keyvalue0_vault5.key = mergefunc(keyvalue0_vault4.key, buffer4[0][i].key, NAp);
		keyvalue0_vault5.value = mergefunc(keyvalue0_vault4.value, buffer4[0][i].value, NAp);
		keyvalue1_vault5.key = mergefunc(keyvalue1_vault4.key, buffer4[1][i].key, NAp);
		keyvalue1_vault5.value = mergefunc(keyvalue1_vault4.value, buffer4[1][i].value, NAp);
		keyvalue2_vault5.key = mergefunc(keyvalue2_vault4.key, buffer4[2][i].key, NAp);
		keyvalue2_vault5.value = mergefunc(keyvalue2_vault4.value, buffer4[2][i].value, NAp);
		keyvalue3_vault5.key = mergefunc(keyvalue3_vault4.key, buffer4[3][i].key, NAp);
		keyvalue3_vault5.value = mergefunc(keyvalue3_vault4.value, buffer4[3][i].value, NAp);
		keyvalue4_vault5.key = mergefunc(keyvalue4_vault4.key, buffer4[4][i].key, NAp);
		keyvalue4_vault5.value = mergefunc(keyvalue4_vault4.value, buffer4[4][i].value, NAp);
		keyvalue5_vault5.key = mergefunc(keyvalue5_vault4.key, buffer4[5][i].key, NAp);
		keyvalue5_vault5.value = mergefunc(keyvalue5_vault4.value, buffer4[5][i].value, NAp);
		keyvalue6_vault5.key = mergefunc(keyvalue6_vault4.key, buffer4[6][i].key, NAp);
		keyvalue6_vault5.value = mergefunc(keyvalue6_vault4.value, buffer4[6][i].value, NAp);
		keyvalue7_vault5.key = mergefunc(keyvalue7_vault4.key, buffer4[7][i].key, NAp);
		keyvalue7_vault5.value = mergefunc(keyvalue7_vault4.value, buffer4[7][i].value, NAp);
		keyvalue8_vault5.key = mergefunc(keyvalue8_vault4.key, buffer4[8][i].key, NAp);
		keyvalue8_vault5.value = mergefunc(keyvalue8_vault4.value, buffer4[8][i].value, NAp);
		keyvalue9_vault5.key = mergefunc(keyvalue9_vault4.key, buffer4[9][i].key, NAp);
		keyvalue9_vault5.value = mergefunc(keyvalue9_vault4.value, buffer4[9][i].value, NAp);
		keyvalue10_vault5.key = mergefunc(keyvalue10_vault4.key, buffer4[10][i].key, NAp);
		keyvalue10_vault5.value = mergefunc(keyvalue10_vault4.value, buffer4[10][i].value, NAp);
		keyvalue11_vault5.key = mergefunc(keyvalue11_vault4.key, buffer4[11][i].key, NAp);
		keyvalue11_vault5.value = mergefunc(keyvalue11_vault4.value, buffer4[11][i].value, NAp);
		keyvalue12_vault5.key = mergefunc(keyvalue12_vault4.key, buffer4[12][i].key, NAp);
		keyvalue12_vault5.value = mergefunc(keyvalue12_vault4.value, buffer4[12][i].value, NAp);
		keyvalue13_vault5.key = mergefunc(keyvalue13_vault4.key, buffer4[13][i].key, NAp);
		keyvalue13_vault5.value = mergefunc(keyvalue13_vault4.value, buffer4[13][i].value, NAp);
		keyvalue14_vault5.key = mergefunc(keyvalue14_vault4.key, buffer4[14][i].key, NAp);
		keyvalue14_vault5.value = mergefunc(keyvalue14_vault4.value, buffer4[14][i].value, NAp);
		keyvalue15_vault5.key = mergefunc(keyvalue15_vault4.key, buffer4[15][i].key, NAp);
		keyvalue15_vault5.value = mergefunc(keyvalue15_vault4.value, buffer4[15][i].value, NAp);
	
		keyvalue0_vault6.key = mergefunc(keyvalue0_vault5.key, buffer5[0][i].key, NAp);
		keyvalue0_vault6.value = mergefunc(keyvalue0_vault5.value, buffer5[0][i].value, NAp);
		keyvalue1_vault6.key = mergefunc(keyvalue1_vault5.key, buffer5[1][i].key, NAp);
		keyvalue1_vault6.value = mergefunc(keyvalue1_vault5.value, buffer5[1][i].value, NAp);
		keyvalue2_vault6.key = mergefunc(keyvalue2_vault5.key, buffer5[2][i].key, NAp);
		keyvalue2_vault6.value = mergefunc(keyvalue2_vault5.value, buffer5[2][i].value, NAp);
		keyvalue3_vault6.key = mergefunc(keyvalue3_vault5.key, buffer5[3][i].key, NAp);
		keyvalue3_vault6.value = mergefunc(keyvalue3_vault5.value, buffer5[3][i].value, NAp);
		keyvalue4_vault6.key = mergefunc(keyvalue4_vault5.key, buffer5[4][i].key, NAp);
		keyvalue4_vault6.value = mergefunc(keyvalue4_vault5.value, buffer5[4][i].value, NAp);
		keyvalue5_vault6.key = mergefunc(keyvalue5_vault5.key, buffer5[5][i].key, NAp);
		keyvalue5_vault6.value = mergefunc(keyvalue5_vault5.value, buffer5[5][i].value, NAp);
		keyvalue6_vault6.key = mergefunc(keyvalue6_vault5.key, buffer5[6][i].key, NAp);
		keyvalue6_vault6.value = mergefunc(keyvalue6_vault5.value, buffer5[6][i].value, NAp);
		keyvalue7_vault6.key = mergefunc(keyvalue7_vault5.key, buffer5[7][i].key, NAp);
		keyvalue7_vault6.value = mergefunc(keyvalue7_vault5.value, buffer5[7][i].value, NAp);
		keyvalue8_vault6.key = mergefunc(keyvalue8_vault5.key, buffer5[8][i].key, NAp);
		keyvalue8_vault6.value = mergefunc(keyvalue8_vault5.value, buffer5[8][i].value, NAp);
		keyvalue9_vault6.key = mergefunc(keyvalue9_vault5.key, buffer5[9][i].key, NAp);
		keyvalue9_vault6.value = mergefunc(keyvalue9_vault5.value, buffer5[9][i].value, NAp);
		keyvalue10_vault6.key = mergefunc(keyvalue10_vault5.key, buffer5[10][i].key, NAp);
		keyvalue10_vault6.value = mergefunc(keyvalue10_vault5.value, buffer5[10][i].value, NAp);
		keyvalue11_vault6.key = mergefunc(keyvalue11_vault5.key, buffer5[11][i].key, NAp);
		keyvalue11_vault6.value = mergefunc(keyvalue11_vault5.value, buffer5[11][i].value, NAp);
		keyvalue12_vault6.key = mergefunc(keyvalue12_vault5.key, buffer5[12][i].key, NAp);
		keyvalue12_vault6.value = mergefunc(keyvalue12_vault5.value, buffer5[12][i].value, NAp);
		keyvalue13_vault6.key = mergefunc(keyvalue13_vault5.key, buffer5[13][i].key, NAp);
		keyvalue13_vault6.value = mergefunc(keyvalue13_vault5.value, buffer5[13][i].value, NAp);
		keyvalue14_vault6.key = mergefunc(keyvalue14_vault5.key, buffer5[14][i].key, NAp);
		keyvalue14_vault6.value = mergefunc(keyvalue14_vault5.value, buffer5[14][i].value, NAp);
		keyvalue15_vault6.key = mergefunc(keyvalue15_vault5.key, buffer5[15][i].key, NAp);
		keyvalue15_vault6.value = mergefunc(keyvalue15_vault5.value, buffer5[15][i].value, NAp);
	
		keyvalue0_vault7.key = mergefunc(keyvalue0_vault6.key, buffer6[0][i].key, NAp);
		keyvalue0_vault7.value = mergefunc(keyvalue0_vault6.value, buffer6[0][i].value, NAp);
		keyvalue1_vault7.key = mergefunc(keyvalue1_vault6.key, buffer6[1][i].key, NAp);
		keyvalue1_vault7.value = mergefunc(keyvalue1_vault6.value, buffer6[1][i].value, NAp);
		keyvalue2_vault7.key = mergefunc(keyvalue2_vault6.key, buffer6[2][i].key, NAp);
		keyvalue2_vault7.value = mergefunc(keyvalue2_vault6.value, buffer6[2][i].value, NAp);
		keyvalue3_vault7.key = mergefunc(keyvalue3_vault6.key, buffer6[3][i].key, NAp);
		keyvalue3_vault7.value = mergefunc(keyvalue3_vault6.value, buffer6[3][i].value, NAp);
		keyvalue4_vault7.key = mergefunc(keyvalue4_vault6.key, buffer6[4][i].key, NAp);
		keyvalue4_vault7.value = mergefunc(keyvalue4_vault6.value, buffer6[4][i].value, NAp);
		keyvalue5_vault7.key = mergefunc(keyvalue5_vault6.key, buffer6[5][i].key, NAp);
		keyvalue5_vault7.value = mergefunc(keyvalue5_vault6.value, buffer6[5][i].value, NAp);
		keyvalue6_vault7.key = mergefunc(keyvalue6_vault6.key, buffer6[6][i].key, NAp);
		keyvalue6_vault7.value = mergefunc(keyvalue6_vault6.value, buffer6[6][i].value, NAp);
		keyvalue7_vault7.key = mergefunc(keyvalue7_vault6.key, buffer6[7][i].key, NAp);
		keyvalue7_vault7.value = mergefunc(keyvalue7_vault6.value, buffer6[7][i].value, NAp);
		keyvalue8_vault7.key = mergefunc(keyvalue8_vault6.key, buffer6[8][i].key, NAp);
		keyvalue8_vault7.value = mergefunc(keyvalue8_vault6.value, buffer6[8][i].value, NAp);
		keyvalue9_vault7.key = mergefunc(keyvalue9_vault6.key, buffer6[9][i].key, NAp);
		keyvalue9_vault7.value = mergefunc(keyvalue9_vault6.value, buffer6[9][i].value, NAp);
		keyvalue10_vault7.key = mergefunc(keyvalue10_vault6.key, buffer6[10][i].key, NAp);
		keyvalue10_vault7.value = mergefunc(keyvalue10_vault6.value, buffer6[10][i].value, NAp);
		keyvalue11_vault7.key = mergefunc(keyvalue11_vault6.key, buffer6[11][i].key, NAp);
		keyvalue11_vault7.value = mergefunc(keyvalue11_vault6.value, buffer6[11][i].value, NAp);
		keyvalue12_vault7.key = mergefunc(keyvalue12_vault6.key, buffer6[12][i].key, NAp);
		keyvalue12_vault7.value = mergefunc(keyvalue12_vault6.value, buffer6[12][i].value, NAp);
		keyvalue13_vault7.key = mergefunc(keyvalue13_vault6.key, buffer6[13][i].key, NAp);
		keyvalue13_vault7.value = mergefunc(keyvalue13_vault6.value, buffer6[13][i].value, NAp);
		keyvalue14_vault7.key = mergefunc(keyvalue14_vault6.key, buffer6[14][i].key, NAp);
		keyvalue14_vault7.value = mergefunc(keyvalue14_vault6.value, buffer6[14][i].value, NAp);
		keyvalue15_vault7.key = mergefunc(keyvalue15_vault6.key, buffer6[15][i].key, NAp);
		keyvalue15_vault7.value = mergefunc(keyvalue15_vault6.value, buffer6[15][i].value, NAp);
	
		keyvalue0_vault8.key = mergefunc(keyvalue0_vault7.key, buffer7[0][i].key, NAp);
		keyvalue0_vault8.value = mergefunc(keyvalue0_vault7.value, buffer7[0][i].value, NAp);
		keyvalue1_vault8.key = mergefunc(keyvalue1_vault7.key, buffer7[1][i].key, NAp);
		keyvalue1_vault8.value = mergefunc(keyvalue1_vault7.value, buffer7[1][i].value, NAp);
		keyvalue2_vault8.key = mergefunc(keyvalue2_vault7.key, buffer7[2][i].key, NAp);
		keyvalue2_vault8.value = mergefunc(keyvalue2_vault7.value, buffer7[2][i].value, NAp);
		keyvalue3_vault8.key = mergefunc(keyvalue3_vault7.key, buffer7[3][i].key, NAp);
		keyvalue3_vault8.value = mergefunc(keyvalue3_vault7.value, buffer7[3][i].value, NAp);
		keyvalue4_vault8.key = mergefunc(keyvalue4_vault7.key, buffer7[4][i].key, NAp);
		keyvalue4_vault8.value = mergefunc(keyvalue4_vault7.value, buffer7[4][i].value, NAp);
		keyvalue5_vault8.key = mergefunc(keyvalue5_vault7.key, buffer7[5][i].key, NAp);
		keyvalue5_vault8.value = mergefunc(keyvalue5_vault7.value, buffer7[5][i].value, NAp);
		keyvalue6_vault8.key = mergefunc(keyvalue6_vault7.key, buffer7[6][i].key, NAp);
		keyvalue6_vault8.value = mergefunc(keyvalue6_vault7.value, buffer7[6][i].value, NAp);
		keyvalue7_vault8.key = mergefunc(keyvalue7_vault7.key, buffer7[7][i].key, NAp);
		keyvalue7_vault8.value = mergefunc(keyvalue7_vault7.value, buffer7[7][i].value, NAp);
		keyvalue8_vault8.key = mergefunc(keyvalue8_vault7.key, buffer7[8][i].key, NAp);
		keyvalue8_vault8.value = mergefunc(keyvalue8_vault7.value, buffer7[8][i].value, NAp);
		keyvalue9_vault8.key = mergefunc(keyvalue9_vault7.key, buffer7[9][i].key, NAp);
		keyvalue9_vault8.value = mergefunc(keyvalue9_vault7.value, buffer7[9][i].value, NAp);
		keyvalue10_vault8.key = mergefunc(keyvalue10_vault7.key, buffer7[10][i].key, NAp);
		keyvalue10_vault8.value = mergefunc(keyvalue10_vault7.value, buffer7[10][i].value, NAp);
		keyvalue11_vault8.key = mergefunc(keyvalue11_vault7.key, buffer7[11][i].key, NAp);
		keyvalue11_vault8.value = mergefunc(keyvalue11_vault7.value, buffer7[11][i].value, NAp);
		keyvalue12_vault8.key = mergefunc(keyvalue12_vault7.key, buffer7[12][i].key, NAp);
		keyvalue12_vault8.value = mergefunc(keyvalue12_vault7.value, buffer7[12][i].value, NAp);
		keyvalue13_vault8.key = mergefunc(keyvalue13_vault7.key, buffer7[13][i].key, NAp);
		keyvalue13_vault8.value = mergefunc(keyvalue13_vault7.value, buffer7[13][i].value, NAp);
		keyvalue14_vault8.key = mergefunc(keyvalue14_vault7.key, buffer7[14][i].key, NAp);
		keyvalue14_vault8.value = mergefunc(keyvalue14_vault7.value, buffer7[14][i].value, NAp);
		keyvalue15_vault8.key = mergefunc(keyvalue15_vault7.key, buffer7[15][i].key, NAp);
		keyvalue15_vault8.value = mergefunc(keyvalue15_vault7.value, buffer7[15][i].value, NAp);
	
		keyvalue0_vault9.key = mergefunc(keyvalue0_vault8.key, buffer8[0][i].key, NAp);
		keyvalue0_vault9.value = mergefunc(keyvalue0_vault8.value, buffer8[0][i].value, NAp);
		keyvalue1_vault9.key = mergefunc(keyvalue1_vault8.key, buffer8[1][i].key, NAp);
		keyvalue1_vault9.value = mergefunc(keyvalue1_vault8.value, buffer8[1][i].value, NAp);
		keyvalue2_vault9.key = mergefunc(keyvalue2_vault8.key, buffer8[2][i].key, NAp);
		keyvalue2_vault9.value = mergefunc(keyvalue2_vault8.value, buffer8[2][i].value, NAp);
		keyvalue3_vault9.key = mergefunc(keyvalue3_vault8.key, buffer8[3][i].key, NAp);
		keyvalue3_vault9.value = mergefunc(keyvalue3_vault8.value, buffer8[3][i].value, NAp);
		keyvalue4_vault9.key = mergefunc(keyvalue4_vault8.key, buffer8[4][i].key, NAp);
		keyvalue4_vault9.value = mergefunc(keyvalue4_vault8.value, buffer8[4][i].value, NAp);
		keyvalue5_vault9.key = mergefunc(keyvalue5_vault8.key, buffer8[5][i].key, NAp);
		keyvalue5_vault9.value = mergefunc(keyvalue5_vault8.value, buffer8[5][i].value, NAp);
		keyvalue6_vault9.key = mergefunc(keyvalue6_vault8.key, buffer8[6][i].key, NAp);
		keyvalue6_vault9.value = mergefunc(keyvalue6_vault8.value, buffer8[6][i].value, NAp);
		keyvalue7_vault9.key = mergefunc(keyvalue7_vault8.key, buffer8[7][i].key, NAp);
		keyvalue7_vault9.value = mergefunc(keyvalue7_vault8.value, buffer8[7][i].value, NAp);
		keyvalue8_vault9.key = mergefunc(keyvalue8_vault8.key, buffer8[8][i].key, NAp);
		keyvalue8_vault9.value = mergefunc(keyvalue8_vault8.value, buffer8[8][i].value, NAp);
		keyvalue9_vault9.key = mergefunc(keyvalue9_vault8.key, buffer8[9][i].key, NAp);
		keyvalue9_vault9.value = mergefunc(keyvalue9_vault8.value, buffer8[9][i].value, NAp);
		keyvalue10_vault9.key = mergefunc(keyvalue10_vault8.key, buffer8[10][i].key, NAp);
		keyvalue10_vault9.value = mergefunc(keyvalue10_vault8.value, buffer8[10][i].value, NAp);
		keyvalue11_vault9.key = mergefunc(keyvalue11_vault8.key, buffer8[11][i].key, NAp);
		keyvalue11_vault9.value = mergefunc(keyvalue11_vault8.value, buffer8[11][i].value, NAp);
		keyvalue12_vault9.key = mergefunc(keyvalue12_vault8.key, buffer8[12][i].key, NAp);
		keyvalue12_vault9.value = mergefunc(keyvalue12_vault8.value, buffer8[12][i].value, NAp);
		keyvalue13_vault9.key = mergefunc(keyvalue13_vault8.key, buffer8[13][i].key, NAp);
		keyvalue13_vault9.value = mergefunc(keyvalue13_vault8.value, buffer8[13][i].value, NAp);
		keyvalue14_vault9.key = mergefunc(keyvalue14_vault8.key, buffer8[14][i].key, NAp);
		keyvalue14_vault9.value = mergefunc(keyvalue14_vault8.value, buffer8[14][i].value, NAp);
		keyvalue15_vault9.key = mergefunc(keyvalue15_vault8.key, buffer8[15][i].key, NAp);
		keyvalue15_vault9.value = mergefunc(keyvalue15_vault8.value, buffer8[15][i].value, NAp);
	
		keyvalue0_vault10.key = mergefunc(keyvalue0_vault9.key, buffer9[0][i].key, NAp);
		keyvalue0_vault10.value = mergefunc(keyvalue0_vault9.value, buffer9[0][i].value, NAp);
		keyvalue1_vault10.key = mergefunc(keyvalue1_vault9.key, buffer9[1][i].key, NAp);
		keyvalue1_vault10.value = mergefunc(keyvalue1_vault9.value, buffer9[1][i].value, NAp);
		keyvalue2_vault10.key = mergefunc(keyvalue2_vault9.key, buffer9[2][i].key, NAp);
		keyvalue2_vault10.value = mergefunc(keyvalue2_vault9.value, buffer9[2][i].value, NAp);
		keyvalue3_vault10.key = mergefunc(keyvalue3_vault9.key, buffer9[3][i].key, NAp);
		keyvalue3_vault10.value = mergefunc(keyvalue3_vault9.value, buffer9[3][i].value, NAp);
		keyvalue4_vault10.key = mergefunc(keyvalue4_vault9.key, buffer9[4][i].key, NAp);
		keyvalue4_vault10.value = mergefunc(keyvalue4_vault9.value, buffer9[4][i].value, NAp);
		keyvalue5_vault10.key = mergefunc(keyvalue5_vault9.key, buffer9[5][i].key, NAp);
		keyvalue5_vault10.value = mergefunc(keyvalue5_vault9.value, buffer9[5][i].value, NAp);
		keyvalue6_vault10.key = mergefunc(keyvalue6_vault9.key, buffer9[6][i].key, NAp);
		keyvalue6_vault10.value = mergefunc(keyvalue6_vault9.value, buffer9[6][i].value, NAp);
		keyvalue7_vault10.key = mergefunc(keyvalue7_vault9.key, buffer9[7][i].key, NAp);
		keyvalue7_vault10.value = mergefunc(keyvalue7_vault9.value, buffer9[7][i].value, NAp);
		keyvalue8_vault10.key = mergefunc(keyvalue8_vault9.key, buffer9[8][i].key, NAp);
		keyvalue8_vault10.value = mergefunc(keyvalue8_vault9.value, buffer9[8][i].value, NAp);
		keyvalue9_vault10.key = mergefunc(keyvalue9_vault9.key, buffer9[9][i].key, NAp);
		keyvalue9_vault10.value = mergefunc(keyvalue9_vault9.value, buffer9[9][i].value, NAp);
		keyvalue10_vault10.key = mergefunc(keyvalue10_vault9.key, buffer9[10][i].key, NAp);
		keyvalue10_vault10.value = mergefunc(keyvalue10_vault9.value, buffer9[10][i].value, NAp);
		keyvalue11_vault10.key = mergefunc(keyvalue11_vault9.key, buffer9[11][i].key, NAp);
		keyvalue11_vault10.value = mergefunc(keyvalue11_vault9.value, buffer9[11][i].value, NAp);
		keyvalue12_vault10.key = mergefunc(keyvalue12_vault9.key, buffer9[12][i].key, NAp);
		keyvalue12_vault10.value = mergefunc(keyvalue12_vault9.value, buffer9[12][i].value, NAp);
		keyvalue13_vault10.key = mergefunc(keyvalue13_vault9.key, buffer9[13][i].key, NAp);
		keyvalue13_vault10.value = mergefunc(keyvalue13_vault9.value, buffer9[13][i].value, NAp);
		keyvalue14_vault10.key = mergefunc(keyvalue14_vault9.key, buffer9[14][i].key, NAp);
		keyvalue14_vault10.value = mergefunc(keyvalue14_vault9.value, buffer9[14][i].value, NAp);
		keyvalue15_vault10.key = mergefunc(keyvalue15_vault9.key, buffer9[15][i].key, NAp);
		keyvalue15_vault10.value = mergefunc(keyvalue15_vault9.value, buffer9[15][i].value, NAp);
	
		keyvalue0_vault11.key = mergefunc(keyvalue0_vault10.key, buffer10[0][i].key, NAp);
		keyvalue0_vault11.value = mergefunc(keyvalue0_vault10.value, buffer10[0][i].value, NAp);
		keyvalue1_vault11.key = mergefunc(keyvalue1_vault10.key, buffer10[1][i].key, NAp);
		keyvalue1_vault11.value = mergefunc(keyvalue1_vault10.value, buffer10[1][i].value, NAp);
		keyvalue2_vault11.key = mergefunc(keyvalue2_vault10.key, buffer10[2][i].key, NAp);
		keyvalue2_vault11.value = mergefunc(keyvalue2_vault10.value, buffer10[2][i].value, NAp);
		keyvalue3_vault11.key = mergefunc(keyvalue3_vault10.key, buffer10[3][i].key, NAp);
		keyvalue3_vault11.value = mergefunc(keyvalue3_vault10.value, buffer10[3][i].value, NAp);
		keyvalue4_vault11.key = mergefunc(keyvalue4_vault10.key, buffer10[4][i].key, NAp);
		keyvalue4_vault11.value = mergefunc(keyvalue4_vault10.value, buffer10[4][i].value, NAp);
		keyvalue5_vault11.key = mergefunc(keyvalue5_vault10.key, buffer10[5][i].key, NAp);
		keyvalue5_vault11.value = mergefunc(keyvalue5_vault10.value, buffer10[5][i].value, NAp);
		keyvalue6_vault11.key = mergefunc(keyvalue6_vault10.key, buffer10[6][i].key, NAp);
		keyvalue6_vault11.value = mergefunc(keyvalue6_vault10.value, buffer10[6][i].value, NAp);
		keyvalue7_vault11.key = mergefunc(keyvalue7_vault10.key, buffer10[7][i].key, NAp);
		keyvalue7_vault11.value = mergefunc(keyvalue7_vault10.value, buffer10[7][i].value, NAp);
		keyvalue8_vault11.key = mergefunc(keyvalue8_vault10.key, buffer10[8][i].key, NAp);
		keyvalue8_vault11.value = mergefunc(keyvalue8_vault10.value, buffer10[8][i].value, NAp);
		keyvalue9_vault11.key = mergefunc(keyvalue9_vault10.key, buffer10[9][i].key, NAp);
		keyvalue9_vault11.value = mergefunc(keyvalue9_vault10.value, buffer10[9][i].value, NAp);
		keyvalue10_vault11.key = mergefunc(keyvalue10_vault10.key, buffer10[10][i].key, NAp);
		keyvalue10_vault11.value = mergefunc(keyvalue10_vault10.value, buffer10[10][i].value, NAp);
		keyvalue11_vault11.key = mergefunc(keyvalue11_vault10.key, buffer10[11][i].key, NAp);
		keyvalue11_vault11.value = mergefunc(keyvalue11_vault10.value, buffer10[11][i].value, NAp);
		keyvalue12_vault11.key = mergefunc(keyvalue12_vault10.key, buffer10[12][i].key, NAp);
		keyvalue12_vault11.value = mergefunc(keyvalue12_vault10.value, buffer10[12][i].value, NAp);
		keyvalue13_vault11.key = mergefunc(keyvalue13_vault10.key, buffer10[13][i].key, NAp);
		keyvalue13_vault11.value = mergefunc(keyvalue13_vault10.value, buffer10[13][i].value, NAp);
		keyvalue14_vault11.key = mergefunc(keyvalue14_vault10.key, buffer10[14][i].key, NAp);
		keyvalue14_vault11.value = mergefunc(keyvalue14_vault10.value, buffer10[14][i].value, NAp);
		keyvalue15_vault11.key = mergefunc(keyvalue15_vault10.key, buffer10[15][i].key, NAp);
		keyvalue15_vault11.value = mergefunc(keyvalue15_vault10.value, buffer10[15][i].value, NAp);
	
		keyvalue0_vault12.key = mergefunc(keyvalue0_vault11.key, buffer11[0][i].key, NAp);
		keyvalue0_vault12.value = mergefunc(keyvalue0_vault11.value, buffer11[0][i].value, NAp);
		keyvalue1_vault12.key = mergefunc(keyvalue1_vault11.key, buffer11[1][i].key, NAp);
		keyvalue1_vault12.value = mergefunc(keyvalue1_vault11.value, buffer11[1][i].value, NAp);
		keyvalue2_vault12.key = mergefunc(keyvalue2_vault11.key, buffer11[2][i].key, NAp);
		keyvalue2_vault12.value = mergefunc(keyvalue2_vault11.value, buffer11[2][i].value, NAp);
		keyvalue3_vault12.key = mergefunc(keyvalue3_vault11.key, buffer11[3][i].key, NAp);
		keyvalue3_vault12.value = mergefunc(keyvalue3_vault11.value, buffer11[3][i].value, NAp);
		keyvalue4_vault12.key = mergefunc(keyvalue4_vault11.key, buffer11[4][i].key, NAp);
		keyvalue4_vault12.value = mergefunc(keyvalue4_vault11.value, buffer11[4][i].value, NAp);
		keyvalue5_vault12.key = mergefunc(keyvalue5_vault11.key, buffer11[5][i].key, NAp);
		keyvalue5_vault12.value = mergefunc(keyvalue5_vault11.value, buffer11[5][i].value, NAp);
		keyvalue6_vault12.key = mergefunc(keyvalue6_vault11.key, buffer11[6][i].key, NAp);
		keyvalue6_vault12.value = mergefunc(keyvalue6_vault11.value, buffer11[6][i].value, NAp);
		keyvalue7_vault12.key = mergefunc(keyvalue7_vault11.key, buffer11[7][i].key, NAp);
		keyvalue7_vault12.value = mergefunc(keyvalue7_vault11.value, buffer11[7][i].value, NAp);
		keyvalue8_vault12.key = mergefunc(keyvalue8_vault11.key, buffer11[8][i].key, NAp);
		keyvalue8_vault12.value = mergefunc(keyvalue8_vault11.value, buffer11[8][i].value, NAp);
		keyvalue9_vault12.key = mergefunc(keyvalue9_vault11.key, buffer11[9][i].key, NAp);
		keyvalue9_vault12.value = mergefunc(keyvalue9_vault11.value, buffer11[9][i].value, NAp);
		keyvalue10_vault12.key = mergefunc(keyvalue10_vault11.key, buffer11[10][i].key, NAp);
		keyvalue10_vault12.value = mergefunc(keyvalue10_vault11.value, buffer11[10][i].value, NAp);
		keyvalue11_vault12.key = mergefunc(keyvalue11_vault11.key, buffer11[11][i].key, NAp);
		keyvalue11_vault12.value = mergefunc(keyvalue11_vault11.value, buffer11[11][i].value, NAp);
		keyvalue12_vault12.key = mergefunc(keyvalue12_vault11.key, buffer11[12][i].key, NAp);
		keyvalue12_vault12.value = mergefunc(keyvalue12_vault11.value, buffer11[12][i].value, NAp);
		keyvalue13_vault12.key = mergefunc(keyvalue13_vault11.key, buffer11[13][i].key, NAp);
		keyvalue13_vault12.value = mergefunc(keyvalue13_vault11.value, buffer11[13][i].value, NAp);
		keyvalue14_vault12.key = mergefunc(keyvalue14_vault11.key, buffer11[14][i].key, NAp);
		keyvalue14_vault12.value = mergefunc(keyvalue14_vault11.value, buffer11[14][i].value, NAp);
		keyvalue15_vault12.key = mergefunc(keyvalue15_vault11.key, buffer11[15][i].key, NAp);
		keyvalue15_vault12.value = mergefunc(keyvalue15_vault11.value, buffer11[15][i].value, NAp);
	
		keyvalue0_vault13.key = mergefunc(keyvalue0_vault12.key, buffer12[0][i].key, NAp);
		keyvalue0_vault13.value = mergefunc(keyvalue0_vault12.value, buffer12[0][i].value, NAp);
		keyvalue1_vault13.key = mergefunc(keyvalue1_vault12.key, buffer12[1][i].key, NAp);
		keyvalue1_vault13.value = mergefunc(keyvalue1_vault12.value, buffer12[1][i].value, NAp);
		keyvalue2_vault13.key = mergefunc(keyvalue2_vault12.key, buffer12[2][i].key, NAp);
		keyvalue2_vault13.value = mergefunc(keyvalue2_vault12.value, buffer12[2][i].value, NAp);
		keyvalue3_vault13.key = mergefunc(keyvalue3_vault12.key, buffer12[3][i].key, NAp);
		keyvalue3_vault13.value = mergefunc(keyvalue3_vault12.value, buffer12[3][i].value, NAp);
		keyvalue4_vault13.key = mergefunc(keyvalue4_vault12.key, buffer12[4][i].key, NAp);
		keyvalue4_vault13.value = mergefunc(keyvalue4_vault12.value, buffer12[4][i].value, NAp);
		keyvalue5_vault13.key = mergefunc(keyvalue5_vault12.key, buffer12[5][i].key, NAp);
		keyvalue5_vault13.value = mergefunc(keyvalue5_vault12.value, buffer12[5][i].value, NAp);
		keyvalue6_vault13.key = mergefunc(keyvalue6_vault12.key, buffer12[6][i].key, NAp);
		keyvalue6_vault13.value = mergefunc(keyvalue6_vault12.value, buffer12[6][i].value, NAp);
		keyvalue7_vault13.key = mergefunc(keyvalue7_vault12.key, buffer12[7][i].key, NAp);
		keyvalue7_vault13.value = mergefunc(keyvalue7_vault12.value, buffer12[7][i].value, NAp);
		keyvalue8_vault13.key = mergefunc(keyvalue8_vault12.key, buffer12[8][i].key, NAp);
		keyvalue8_vault13.value = mergefunc(keyvalue8_vault12.value, buffer12[8][i].value, NAp);
		keyvalue9_vault13.key = mergefunc(keyvalue9_vault12.key, buffer12[9][i].key, NAp);
		keyvalue9_vault13.value = mergefunc(keyvalue9_vault12.value, buffer12[9][i].value, NAp);
		keyvalue10_vault13.key = mergefunc(keyvalue10_vault12.key, buffer12[10][i].key, NAp);
		keyvalue10_vault13.value = mergefunc(keyvalue10_vault12.value, buffer12[10][i].value, NAp);
		keyvalue11_vault13.key = mergefunc(keyvalue11_vault12.key, buffer12[11][i].key, NAp);
		keyvalue11_vault13.value = mergefunc(keyvalue11_vault12.value, buffer12[11][i].value, NAp);
		keyvalue12_vault13.key = mergefunc(keyvalue12_vault12.key, buffer12[12][i].key, NAp);
		keyvalue12_vault13.value = mergefunc(keyvalue12_vault12.value, buffer12[12][i].value, NAp);
		keyvalue13_vault13.key = mergefunc(keyvalue13_vault12.key, buffer12[13][i].key, NAp);
		keyvalue13_vault13.value = mergefunc(keyvalue13_vault12.value, buffer12[13][i].value, NAp);
		keyvalue14_vault13.key = mergefunc(keyvalue14_vault12.key, buffer12[14][i].key, NAp);
		keyvalue14_vault13.value = mergefunc(keyvalue14_vault12.value, buffer12[14][i].value, NAp);
		keyvalue15_vault13.key = mergefunc(keyvalue15_vault12.key, buffer12[15][i].key, NAp);
		keyvalue15_vault13.value = mergefunc(keyvalue15_vault12.value, buffer12[15][i].value, NAp);
	
		keyvalue0_vault14.key = mergefunc(keyvalue0_vault13.key, buffer13[0][i].key, NAp);
		keyvalue0_vault14.value = mergefunc(keyvalue0_vault13.value, buffer13[0][i].value, NAp);
		keyvalue1_vault14.key = mergefunc(keyvalue1_vault13.key, buffer13[1][i].key, NAp);
		keyvalue1_vault14.value = mergefunc(keyvalue1_vault13.value, buffer13[1][i].value, NAp);
		keyvalue2_vault14.key = mergefunc(keyvalue2_vault13.key, buffer13[2][i].key, NAp);
		keyvalue2_vault14.value = mergefunc(keyvalue2_vault13.value, buffer13[2][i].value, NAp);
		keyvalue3_vault14.key = mergefunc(keyvalue3_vault13.key, buffer13[3][i].key, NAp);
		keyvalue3_vault14.value = mergefunc(keyvalue3_vault13.value, buffer13[3][i].value, NAp);
		keyvalue4_vault14.key = mergefunc(keyvalue4_vault13.key, buffer13[4][i].key, NAp);
		keyvalue4_vault14.value = mergefunc(keyvalue4_vault13.value, buffer13[4][i].value, NAp);
		keyvalue5_vault14.key = mergefunc(keyvalue5_vault13.key, buffer13[5][i].key, NAp);
		keyvalue5_vault14.value = mergefunc(keyvalue5_vault13.value, buffer13[5][i].value, NAp);
		keyvalue6_vault14.key = mergefunc(keyvalue6_vault13.key, buffer13[6][i].key, NAp);
		keyvalue6_vault14.value = mergefunc(keyvalue6_vault13.value, buffer13[6][i].value, NAp);
		keyvalue7_vault14.key = mergefunc(keyvalue7_vault13.key, buffer13[7][i].key, NAp);
		keyvalue7_vault14.value = mergefunc(keyvalue7_vault13.value, buffer13[7][i].value, NAp);
		keyvalue8_vault14.key = mergefunc(keyvalue8_vault13.key, buffer13[8][i].key, NAp);
		keyvalue8_vault14.value = mergefunc(keyvalue8_vault13.value, buffer13[8][i].value, NAp);
		keyvalue9_vault14.key = mergefunc(keyvalue9_vault13.key, buffer13[9][i].key, NAp);
		keyvalue9_vault14.value = mergefunc(keyvalue9_vault13.value, buffer13[9][i].value, NAp);
		keyvalue10_vault14.key = mergefunc(keyvalue10_vault13.key, buffer13[10][i].key, NAp);
		keyvalue10_vault14.value = mergefunc(keyvalue10_vault13.value, buffer13[10][i].value, NAp);
		keyvalue11_vault14.key = mergefunc(keyvalue11_vault13.key, buffer13[11][i].key, NAp);
		keyvalue11_vault14.value = mergefunc(keyvalue11_vault13.value, buffer13[11][i].value, NAp);
		keyvalue12_vault14.key = mergefunc(keyvalue12_vault13.key, buffer13[12][i].key, NAp);
		keyvalue12_vault14.value = mergefunc(keyvalue12_vault13.value, buffer13[12][i].value, NAp);
		keyvalue13_vault14.key = mergefunc(keyvalue13_vault13.key, buffer13[13][i].key, NAp);
		keyvalue13_vault14.value = mergefunc(keyvalue13_vault13.value, buffer13[13][i].value, NAp);
		keyvalue14_vault14.key = mergefunc(keyvalue14_vault13.key, buffer13[14][i].key, NAp);
		keyvalue14_vault14.value = mergefunc(keyvalue14_vault13.value, buffer13[14][i].value, NAp);
		keyvalue15_vault14.key = mergefunc(keyvalue15_vault13.key, buffer13[15][i].key, NAp);
		keyvalue15_vault14.value = mergefunc(keyvalue15_vault13.value, buffer13[15][i].value, NAp);
	
		keyvalue0_vault15.key = mergefunc(keyvalue0_vault14.key, buffer14[0][i].key, NAp);
		keyvalue0_vault15.value = mergefunc(keyvalue0_vault14.value, buffer14[0][i].value, NAp);
		keyvalue1_vault15.key = mergefunc(keyvalue1_vault14.key, buffer14[1][i].key, NAp);
		keyvalue1_vault15.value = mergefunc(keyvalue1_vault14.value, buffer14[1][i].value, NAp);
		keyvalue2_vault15.key = mergefunc(keyvalue2_vault14.key, buffer14[2][i].key, NAp);
		keyvalue2_vault15.value = mergefunc(keyvalue2_vault14.value, buffer14[2][i].value, NAp);
		keyvalue3_vault15.key = mergefunc(keyvalue3_vault14.key, buffer14[3][i].key, NAp);
		keyvalue3_vault15.value = mergefunc(keyvalue3_vault14.value, buffer14[3][i].value, NAp);
		keyvalue4_vault15.key = mergefunc(keyvalue4_vault14.key, buffer14[4][i].key, NAp);
		keyvalue4_vault15.value = mergefunc(keyvalue4_vault14.value, buffer14[4][i].value, NAp);
		keyvalue5_vault15.key = mergefunc(keyvalue5_vault14.key, buffer14[5][i].key, NAp);
		keyvalue5_vault15.value = mergefunc(keyvalue5_vault14.value, buffer14[5][i].value, NAp);
		keyvalue6_vault15.key = mergefunc(keyvalue6_vault14.key, buffer14[6][i].key, NAp);
		keyvalue6_vault15.value = mergefunc(keyvalue6_vault14.value, buffer14[6][i].value, NAp);
		keyvalue7_vault15.key = mergefunc(keyvalue7_vault14.key, buffer14[7][i].key, NAp);
		keyvalue7_vault15.value = mergefunc(keyvalue7_vault14.value, buffer14[7][i].value, NAp);
		keyvalue8_vault15.key = mergefunc(keyvalue8_vault14.key, buffer14[8][i].key, NAp);
		keyvalue8_vault15.value = mergefunc(keyvalue8_vault14.value, buffer14[8][i].value, NAp);
		keyvalue9_vault15.key = mergefunc(keyvalue9_vault14.key, buffer14[9][i].key, NAp);
		keyvalue9_vault15.value = mergefunc(keyvalue9_vault14.value, buffer14[9][i].value, NAp);
		keyvalue10_vault15.key = mergefunc(keyvalue10_vault14.key, buffer14[10][i].key, NAp);
		keyvalue10_vault15.value = mergefunc(keyvalue10_vault14.value, buffer14[10][i].value, NAp);
		keyvalue11_vault15.key = mergefunc(keyvalue11_vault14.key, buffer14[11][i].key, NAp);
		keyvalue11_vault15.value = mergefunc(keyvalue11_vault14.value, buffer14[11][i].value, NAp);
		keyvalue12_vault15.key = mergefunc(keyvalue12_vault14.key, buffer14[12][i].key, NAp);
		keyvalue12_vault15.value = mergefunc(keyvalue12_vault14.value, buffer14[12][i].value, NAp);
		keyvalue13_vault15.key = mergefunc(keyvalue13_vault14.key, buffer14[13][i].key, NAp);
		keyvalue13_vault15.value = mergefunc(keyvalue13_vault14.value, buffer14[13][i].value, NAp);
		keyvalue14_vault15.key = mergefunc(keyvalue14_vault14.key, buffer14[14][i].key, NAp);
		keyvalue14_vault15.value = mergefunc(keyvalue14_vault14.value, buffer14[14][i].value, NAp);
		keyvalue15_vault15.key = mergefunc(keyvalue15_vault14.key, buffer14[15][i].key, NAp);
		keyvalue15_vault15.value = mergefunc(keyvalue15_vault14.value, buffer14[15][i].value, NAp);
	
		keyvalue0_vault16.key = mergefunc(keyvalue0_vault15.key, buffer15[0][i].key, NAp);
		keyvalue0_vault16.value = mergefunc(keyvalue0_vault15.value, buffer15[0][i].value, NAp);
		keyvalue1_vault16.key = mergefunc(keyvalue1_vault15.key, buffer15[1][i].key, NAp);
		keyvalue1_vault16.value = mergefunc(keyvalue1_vault15.value, buffer15[1][i].value, NAp);
		keyvalue2_vault16.key = mergefunc(keyvalue2_vault15.key, buffer15[2][i].key, NAp);
		keyvalue2_vault16.value = mergefunc(keyvalue2_vault15.value, buffer15[2][i].value, NAp);
		keyvalue3_vault16.key = mergefunc(keyvalue3_vault15.key, buffer15[3][i].key, NAp);
		keyvalue3_vault16.value = mergefunc(keyvalue3_vault15.value, buffer15[3][i].value, NAp);
		keyvalue4_vault16.key = mergefunc(keyvalue4_vault15.key, buffer15[4][i].key, NAp);
		keyvalue4_vault16.value = mergefunc(keyvalue4_vault15.value, buffer15[4][i].value, NAp);
		keyvalue5_vault16.key = mergefunc(keyvalue5_vault15.key, buffer15[5][i].key, NAp);
		keyvalue5_vault16.value = mergefunc(keyvalue5_vault15.value, buffer15[5][i].value, NAp);
		keyvalue6_vault16.key = mergefunc(keyvalue6_vault15.key, buffer15[6][i].key, NAp);
		keyvalue6_vault16.value = mergefunc(keyvalue6_vault15.value, buffer15[6][i].value, NAp);
		keyvalue7_vault16.key = mergefunc(keyvalue7_vault15.key, buffer15[7][i].key, NAp);
		keyvalue7_vault16.value = mergefunc(keyvalue7_vault15.value, buffer15[7][i].value, NAp);
		keyvalue8_vault16.key = mergefunc(keyvalue8_vault15.key, buffer15[8][i].key, NAp);
		keyvalue8_vault16.value = mergefunc(keyvalue8_vault15.value, buffer15[8][i].value, NAp);
		keyvalue9_vault16.key = mergefunc(keyvalue9_vault15.key, buffer15[9][i].key, NAp);
		keyvalue9_vault16.value = mergefunc(keyvalue9_vault15.value, buffer15[9][i].value, NAp);
		keyvalue10_vault16.key = mergefunc(keyvalue10_vault15.key, buffer15[10][i].key, NAp);
		keyvalue10_vault16.value = mergefunc(keyvalue10_vault15.value, buffer15[10][i].value, NAp);
		keyvalue11_vault16.key = mergefunc(keyvalue11_vault15.key, buffer15[11][i].key, NAp);
		keyvalue11_vault16.value = mergefunc(keyvalue11_vault15.value, buffer15[11][i].value, NAp);
		keyvalue12_vault16.key = mergefunc(keyvalue12_vault15.key, buffer15[12][i].key, NAp);
		keyvalue12_vault16.value = mergefunc(keyvalue12_vault15.value, buffer15[12][i].value, NAp);
		keyvalue13_vault16.key = mergefunc(keyvalue13_vault15.key, buffer15[13][i].key, NAp);
		keyvalue13_vault16.value = mergefunc(keyvalue13_vault15.value, buffer15[13][i].value, NAp);
		keyvalue14_vault16.key = mergefunc(keyvalue14_vault15.key, buffer15[14][i].key, NAp);
		keyvalue14_vault16.value = mergefunc(keyvalue14_vault15.value, buffer15[14][i].value, NAp);
		keyvalue15_vault16.key = mergefunc(keyvalue15_vault15.key, buffer15[15][i].key, NAp);
		keyvalue15_vault16.value = mergefunc(keyvalue15_vault15.value, buffer15[15][i].value, NAp);
	
		keyvalue0_vault17.key = mergefunc(keyvalue0_vault16.key, buffer16[0][i].key, NAp);
		keyvalue0_vault17.value = mergefunc(keyvalue0_vault16.value, buffer16[0][i].value, NAp);
		keyvalue1_vault17.key = mergefunc(keyvalue1_vault16.key, buffer16[1][i].key, NAp);
		keyvalue1_vault17.value = mergefunc(keyvalue1_vault16.value, buffer16[1][i].value, NAp);
		keyvalue2_vault17.key = mergefunc(keyvalue2_vault16.key, buffer16[2][i].key, NAp);
		keyvalue2_vault17.value = mergefunc(keyvalue2_vault16.value, buffer16[2][i].value, NAp);
		keyvalue3_vault17.key = mergefunc(keyvalue3_vault16.key, buffer16[3][i].key, NAp);
		keyvalue3_vault17.value = mergefunc(keyvalue3_vault16.value, buffer16[3][i].value, NAp);
		keyvalue4_vault17.key = mergefunc(keyvalue4_vault16.key, buffer16[4][i].key, NAp);
		keyvalue4_vault17.value = mergefunc(keyvalue4_vault16.value, buffer16[4][i].value, NAp);
		keyvalue5_vault17.key = mergefunc(keyvalue5_vault16.key, buffer16[5][i].key, NAp);
		keyvalue5_vault17.value = mergefunc(keyvalue5_vault16.value, buffer16[5][i].value, NAp);
		keyvalue6_vault17.key = mergefunc(keyvalue6_vault16.key, buffer16[6][i].key, NAp);
		keyvalue6_vault17.value = mergefunc(keyvalue6_vault16.value, buffer16[6][i].value, NAp);
		keyvalue7_vault17.key = mergefunc(keyvalue7_vault16.key, buffer16[7][i].key, NAp);
		keyvalue7_vault17.value = mergefunc(keyvalue7_vault16.value, buffer16[7][i].value, NAp);
		keyvalue8_vault17.key = mergefunc(keyvalue8_vault16.key, buffer16[8][i].key, NAp);
		keyvalue8_vault17.value = mergefunc(keyvalue8_vault16.value, buffer16[8][i].value, NAp);
		keyvalue9_vault17.key = mergefunc(keyvalue9_vault16.key, buffer16[9][i].key, NAp);
		keyvalue9_vault17.value = mergefunc(keyvalue9_vault16.value, buffer16[9][i].value, NAp);
		keyvalue10_vault17.key = mergefunc(keyvalue10_vault16.key, buffer16[10][i].key, NAp);
		keyvalue10_vault17.value = mergefunc(keyvalue10_vault16.value, buffer16[10][i].value, NAp);
		keyvalue11_vault17.key = mergefunc(keyvalue11_vault16.key, buffer16[11][i].key, NAp);
		keyvalue11_vault17.value = mergefunc(keyvalue11_vault16.value, buffer16[11][i].value, NAp);
		keyvalue12_vault17.key = mergefunc(keyvalue12_vault16.key, buffer16[12][i].key, NAp);
		keyvalue12_vault17.value = mergefunc(keyvalue12_vault16.value, buffer16[12][i].value, NAp);
		keyvalue13_vault17.key = mergefunc(keyvalue13_vault16.key, buffer16[13][i].key, NAp);
		keyvalue13_vault17.value = mergefunc(keyvalue13_vault16.value, buffer16[13][i].value, NAp);
		keyvalue14_vault17.key = mergefunc(keyvalue14_vault16.key, buffer16[14][i].key, NAp);
		keyvalue14_vault17.value = mergefunc(keyvalue14_vault16.value, buffer16[14][i].value, NAp);
		keyvalue15_vault17.key = mergefunc(keyvalue15_vault16.key, buffer16[15][i].key, NAp);
		keyvalue15_vault17.value = mergefunc(keyvalue15_vault16.value, buffer16[15][i].value, NAp);
	
		
		tail[0][i] = keyvalue0_vault17;
		tail[1][i] = keyvalue1_vault17;
		tail[2][i] = keyvalue2_vault17;
		tail[3][i] = keyvalue3_vault17;
		tail[4][i] = keyvalue4_vault17;
		tail[5][i] = keyvalue5_vault17;
		tail[6][i] = keyvalue6_vault17;
		tail[7][i] = keyvalue7_vault17;
		tail[8][i] = keyvalue8_vault17;
		tail[9][i] = keyvalue9_vault17;
		tail[10][i] = keyvalue10_vault17;
		tail[11][i] = keyvalue11_vault17;
		tail[12][i] = keyvalue12_vault17;
		tail[13][i] = keyvalue13_vault17;
		tail[14][i] = keyvalue14_vault17;
		tail[15][i] = keyvalue15_vault17;
		#ifdef _DEBUGMODE_KERNELPRINTS
		if(tail[0][i].key < 0xFFFFFFFF){ cout<<"actvvid: "<<(0*REDUCESZ + 2*i)<<endl; } 
		if(tail[0][i].value < 0xFFFFFFFF){ cout<<"actvvid: "<<(0*REDUCESZ + 2*i + 1)<<endl; } 
		if(tail[1][i].key < 0xFFFFFFFF){ cout<<"actvvid: "<<(1*REDUCESZ + 2*i)<<endl; } 
		if(tail[1][i].value < 0xFFFFFFFF){ cout<<"actvvid: "<<(1*REDUCESZ + 2*i + 1)<<endl; } 
		if(tail[2][i].key < 0xFFFFFFFF){ cout<<"actvvid: "<<(2*REDUCESZ + 2*i)<<endl; } 
		if(tail[2][i].value < 0xFFFFFFFF){ cout<<"actvvid: "<<(2*REDUCESZ + 2*i + 1)<<endl; } 
		if(tail[3][i].key < 0xFFFFFFFF){ cout<<"actvvid: "<<(3*REDUCESZ + 2*i)<<endl; } 
		if(tail[3][i].value < 0xFFFFFFFF){ cout<<"actvvid: "<<(3*REDUCESZ + 2*i + 1)<<endl; } 
		if(tail[4][i].key < 0xFFFFFFFF){ cout<<"actvvid: "<<(4*REDUCESZ + 2*i)<<endl; } 
		if(tail[4][i].value < 0xFFFFFFFF){ cout<<"actvvid: "<<(4*REDUCESZ + 2*i + 1)<<endl; } 
		if(tail[5][i].key < 0xFFFFFFFF){ cout<<"actvvid: "<<(5*REDUCESZ + 2*i)<<endl; } 
		if(tail[5][i].value < 0xFFFFFFFF){ cout<<"actvvid: "<<(5*REDUCESZ + 2*i + 1)<<endl; } 
		if(tail[6][i].key < 0xFFFFFFFF){ cout<<"actvvid: "<<(6*REDUCESZ + 2*i)<<endl; } 
		if(tail[6][i].value < 0xFFFFFFFF){ cout<<"actvvid: "<<(6*REDUCESZ + 2*i + 1)<<endl; } 
		if(tail[7][i].key < 0xFFFFFFFF){ cout<<"actvvid: "<<(7*REDUCESZ + 2*i)<<endl; } 
		if(tail[7][i].value < 0xFFFFFFFF){ cout<<"actvvid: "<<(7*REDUCESZ + 2*i + 1)<<endl; } 
		if(tail[8][i].key < 0xFFFFFFFF){ cout<<"actvvid: "<<(8*REDUCESZ + 2*i)<<endl; } 
		if(tail[8][i].value < 0xFFFFFFFF){ cout<<"actvvid: "<<(8*REDUCESZ + 2*i + 1)<<endl; } 
		if(tail[9][i].key < 0xFFFFFFFF){ cout<<"actvvid: "<<(9*REDUCESZ + 2*i)<<endl; } 
		if(tail[9][i].value < 0xFFFFFFFF){ cout<<"actvvid: "<<(9*REDUCESZ + 2*i + 1)<<endl; } 
		if(tail[10][i].key < 0xFFFFFFFF){ cout<<"actvvid: "<<(10*REDUCESZ + 2*i)<<endl; } 
		if(tail[10][i].value < 0xFFFFFFFF){ cout<<"actvvid: "<<(10*REDUCESZ + 2*i + 1)<<endl; } 
		if(tail[11][i].key < 0xFFFFFFFF){ cout<<"actvvid: "<<(11*REDUCESZ + 2*i)<<endl; } 
		if(tail[11][i].value < 0xFFFFFFFF){ cout<<"actvvid: "<<(11*REDUCESZ + 2*i + 1)<<endl; } 
		if(tail[12][i].key < 0xFFFFFFFF){ cout<<"actvvid: "<<(12*REDUCESZ + 2*i)<<endl; } 
		if(tail[12][i].value < 0xFFFFFFFF){ cout<<"actvvid: "<<(12*REDUCESZ + 2*i + 1)<<endl; } 
		if(tail[13][i].key < 0xFFFFFFFF){ cout<<"actvvid: "<<(13*REDUCESZ + 2*i)<<endl; } 
		if(tail[13][i].value < 0xFFFFFFFF){ cout<<"actvvid: "<<(13*REDUCESZ + 2*i + 1)<<endl; } 
		if(tail[14][i].key < 0xFFFFFFFF){ cout<<"actvvid: "<<(14*REDUCESZ + 2*i)<<endl; } 
		if(tail[14][i].value < 0xFFFFFFFF){ cout<<"actvvid: "<<(14*REDUCESZ + 2*i + 1)<<endl; } 
		if(tail[15][i].key < 0xFFFFFFFF){ cout<<"actvvid: "<<(15*REDUCESZ + 2*i)<<endl; } 
		if(tail[15][i].value < 0xFFFFFFFF){ cout<<"actvvid: "<<(15*REDUCESZ + 2*i + 1)<<endl; } 
	
		#endif 
	}
	return;
}

uint32_type
	#ifdef SW 
	acts::
	#endif
apply(bool_type enable, 
		keyvalue16_type vbuffer_1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue16_type synvbuffer_1[VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask[BLOCKRAM_SIZE],
		batch_type voffset_kvs, globalparams_t globalparams){
	if(enable == OFF){ return 0; }
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
	cout<<"apply: apply function called."<<endl;
	#endif 
	
	unsigned int vid[NUM_PARTITIONS][2];
	#pragma HLS ARRAY_PARTITION variable=vid complete
	
	keyvalue16_type data0;
	keyvalue16_type res0;
	keyvalue16_type data1;
	keyvalue16_type res1;
	keyvalue16_type data2;
	keyvalue16_type res2;
	keyvalue16_type data3;
	keyvalue16_type res3;
	keyvalue16_type data4;
	keyvalue16_type res4;
	keyvalue16_type data5;
	keyvalue16_type res5;
	keyvalue16_type data6;
	keyvalue16_type res6;
	keyvalue16_type data7;
	keyvalue16_type res7;
	keyvalue16_type data8;
	keyvalue16_type res8;
	keyvalue16_type data9;
	keyvalue16_type res9;
	keyvalue16_type data10;
	keyvalue16_type res10;
	keyvalue16_type data11;
	keyvalue16_type res11;
	keyvalue16_type data12;
	keyvalue16_type res12;
	keyvalue16_type data13;
	keyvalue16_type res13;
	keyvalue16_type data14;
	keyvalue16_type res14;
	keyvalue16_type data15;
	keyvalue16_type res15;
	
	uint32_type cummvmask_sp = 0;
	buffer_type reducebuffersz = globalparams.SIZE_REDUCE / 2;
	
	APPLY_LOOP1: for(buffer_type i=0; i<reducebuffersz; i++){
	#pragma HLS PIPELINE II=1
	
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
		vid[0][0] = voffset_kvs*VECTOR_SIZE*2 + ((0*globalparams.SIZE_REDUCE) + i*2);
		vid[0][1] = voffset_kvs*VECTOR_SIZE*2 + ((0*globalparams.SIZE_REDUCE) + i*2 + 1);
		vid[1][0] = voffset_kvs*VECTOR_SIZE*2 + ((1*globalparams.SIZE_REDUCE) + i*2);
		vid[1][1] = voffset_kvs*VECTOR_SIZE*2 + ((1*globalparams.SIZE_REDUCE) + i*2 + 1);
		vid[2][0] = voffset_kvs*VECTOR_SIZE*2 + ((2*globalparams.SIZE_REDUCE) + i*2);
		vid[2][1] = voffset_kvs*VECTOR_SIZE*2 + ((2*globalparams.SIZE_REDUCE) + i*2 + 1);
		vid[3][0] = voffset_kvs*VECTOR_SIZE*2 + ((3*globalparams.SIZE_REDUCE) + i*2);
		vid[3][1] = voffset_kvs*VECTOR_SIZE*2 + ((3*globalparams.SIZE_REDUCE) + i*2 + 1);
		vid[4][0] = voffset_kvs*VECTOR_SIZE*2 + ((4*globalparams.SIZE_REDUCE) + i*2);
		vid[4][1] = voffset_kvs*VECTOR_SIZE*2 + ((4*globalparams.SIZE_REDUCE) + i*2 + 1);
		vid[5][0] = voffset_kvs*VECTOR_SIZE*2 + ((5*globalparams.SIZE_REDUCE) + i*2);
		vid[5][1] = voffset_kvs*VECTOR_SIZE*2 + ((5*globalparams.SIZE_REDUCE) + i*2 + 1);
		vid[6][0] = voffset_kvs*VECTOR_SIZE*2 + ((6*globalparams.SIZE_REDUCE) + i*2);
		vid[6][1] = voffset_kvs*VECTOR_SIZE*2 + ((6*globalparams.SIZE_REDUCE) + i*2 + 1);
		vid[7][0] = voffset_kvs*VECTOR_SIZE*2 + ((7*globalparams.SIZE_REDUCE) + i*2);
		vid[7][1] = voffset_kvs*VECTOR_SIZE*2 + ((7*globalparams.SIZE_REDUCE) + i*2 + 1);
		vid[8][0] = voffset_kvs*VECTOR_SIZE*2 + ((8*globalparams.SIZE_REDUCE) + i*2);
		vid[8][1] = voffset_kvs*VECTOR_SIZE*2 + ((8*globalparams.SIZE_REDUCE) + i*2 + 1);
		vid[9][0] = voffset_kvs*VECTOR_SIZE*2 + ((9*globalparams.SIZE_REDUCE) + i*2);
		vid[9][1] = voffset_kvs*VECTOR_SIZE*2 + ((9*globalparams.SIZE_REDUCE) + i*2 + 1);
		vid[10][0] = voffset_kvs*VECTOR_SIZE*2 + ((10*globalparams.SIZE_REDUCE) + i*2);
		vid[10][1] = voffset_kvs*VECTOR_SIZE*2 + ((10*globalparams.SIZE_REDUCE) + i*2 + 1);
		vid[11][0] = voffset_kvs*VECTOR_SIZE*2 + ((11*globalparams.SIZE_REDUCE) + i*2);
		vid[11][1] = voffset_kvs*VECTOR_SIZE*2 + ((11*globalparams.SIZE_REDUCE) + i*2 + 1);
		vid[12][0] = voffset_kvs*VECTOR_SIZE*2 + ((12*globalparams.SIZE_REDUCE) + i*2);
		vid[12][1] = voffset_kvs*VECTOR_SIZE*2 + ((12*globalparams.SIZE_REDUCE) + i*2 + 1);
		vid[13][0] = voffset_kvs*VECTOR_SIZE*2 + ((13*globalparams.SIZE_REDUCE) + i*2);
		vid[13][1] = voffset_kvs*VECTOR_SIZE*2 + ((13*globalparams.SIZE_REDUCE) + i*2 + 1);
		vid[14][0] = voffset_kvs*VECTOR_SIZE*2 + ((14*globalparams.SIZE_REDUCE) + i*2);
		vid[14][1] = voffset_kvs*VECTOR_SIZE*2 + ((14*globalparams.SIZE_REDUCE) + i*2 + 1);
		vid[15][0] = voffset_kvs*VECTOR_SIZE*2 + ((15*globalparams.SIZE_REDUCE) + i*2);
		vid[15][1] = voffset_kvs*VECTOR_SIZE*2 + ((15*globalparams.SIZE_REDUCE) + i*2 + 1);
		#endif 
	
		data0 = vbuffer_1[0][i];
		data1 = vbuffer_1[1][i];
		data2 = vbuffer_1[2][i];
		data3 = vbuffer_1[3][i];
		data4 = vbuffer_1[4][i];
		data5 = vbuffer_1[5][i];
		data6 = vbuffer_1[6][i];
		data7 = vbuffer_1[7][i];
		data8 = vbuffer_1[8][i];
		data9 = vbuffer_1[9][i];
		data10 = vbuffer_1[10][i];
		data11 = vbuffer_1[11][i];
		data12 = vbuffer_1[12][i];
		data13 = vbuffer_1[13][i];
		data14 = vbuffer_1[14][i];
		data15 = vbuffer_1[15][i];
		
		keyvalue16_type udata0 = synvbuffer_1[0][i];
		keyvalue16_type udata1 = synvbuffer_1[1][i];
		keyvalue16_type udata2 = synvbuffer_1[2][i];
		keyvalue16_type udata3 = synvbuffer_1[3][i];
		keyvalue16_type udata4 = synvbuffer_1[4][i];
		keyvalue16_type udata5 = synvbuffer_1[5][i];
		keyvalue16_type udata6 = synvbuffer_1[6][i];
		keyvalue16_type udata7 = synvbuffer_1[7][i];
		keyvalue16_type udata8 = synvbuffer_1[8][i];
		keyvalue16_type udata9 = synvbuffer_1[9][i];
		keyvalue16_type udata10 = synvbuffer_1[10][i];
		keyvalue16_type udata11 = synvbuffer_1[11][i];
		keyvalue16_type udata12 = synvbuffer_1[12][i];
		keyvalue16_type udata13 = synvbuffer_1[13][i];
		keyvalue16_type udata14 = synvbuffer_1[14][i];
		keyvalue16_type udata15 = synvbuffer_1[15][i];
		
		res0.key = applyfunc(udata0.key, data0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res1.key = applyfunc(udata1.key, data1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res2.key = applyfunc(udata2.key, data2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res3.key = applyfunc(udata3.key, data3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res4.key = applyfunc(udata4.key, data4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res5.key = applyfunc(udata5.key, data5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res6.key = applyfunc(udata6.key, data6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res7.key = applyfunc(udata7.key, data7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res8.key = applyfunc(udata8.key, data8.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res9.key = applyfunc(udata9.key, data9.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res10.key = applyfunc(udata10.key, data10.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res11.key = applyfunc(udata11.key, data11.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res12.key = applyfunc(udata12.key, data12.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res13.key = applyfunc(udata13.key, data13.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res14.key = applyfunc(udata14.key, data14.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res15.key = applyfunc(udata15.key, data15.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res0.value = applyfunc(udata0.value, data0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res1.value = applyfunc(udata1.value, data1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res2.value = applyfunc(udata2.value, data2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res3.value = applyfunc(udata3.value, data3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res4.value = applyfunc(udata4.value, data4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res5.value = applyfunc(udata5.value, data5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res6.value = applyfunc(udata6.value, data6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res7.value = applyfunc(udata7.value, data7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res8.value = applyfunc(udata8.value, data8.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res9.value = applyfunc(udata9.value, data9.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res10.value = applyfunc(udata10.value, data10.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res11.value = applyfunc(udata11.value, data11.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res12.value = applyfunc(udata12.value, data12.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res13.value = applyfunc(udata13.value, data13.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res14.value = applyfunc(udata14.value, data14.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res15.value = applyfunc(udata15.value, data15.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		
		if(res0.key != udata0.key){ vmask[i].data[0].key = 1; } else { vmask[i].data[0].key = 0; }
		if(res0.value != udata0.value){ vmask[i].data[0].value = 1; } else { vmask[i].data[0].value = 0; }
		if(res1.key != udata1.key){ vmask[i].data[1].key = 1; } else { vmask[i].data[1].key = 0; }
		if(res1.value != udata1.value){ vmask[i].data[1].value = 1; } else { vmask[i].data[1].value = 0; }
		if(res2.key != udata2.key){ vmask[i].data[2].key = 1; } else { vmask[i].data[2].key = 0; }
		if(res2.value != udata2.value){ vmask[i].data[2].value = 1; } else { vmask[i].data[2].value = 0; }
		if(res3.key != udata3.key){ vmask[i].data[3].key = 1; } else { vmask[i].data[3].key = 0; }
		if(res3.value != udata3.value){ vmask[i].data[3].value = 1; } else { vmask[i].data[3].value = 0; }
		if(res4.key != udata4.key){ vmask[i].data[4].key = 1; } else { vmask[i].data[4].key = 0; }
		if(res4.value != udata4.value){ vmask[i].data[4].value = 1; } else { vmask[i].data[4].value = 0; }
		if(res5.key != udata5.key){ vmask[i].data[5].key = 1; } else { vmask[i].data[5].key = 0; }
		if(res5.value != udata5.value){ vmask[i].data[5].value = 1; } else { vmask[i].data[5].value = 0; }
		if(res6.key != udata6.key){ vmask[i].data[6].key = 1; } else { vmask[i].data[6].key = 0; }
		if(res6.value != udata6.value){ vmask[i].data[6].value = 1; } else { vmask[i].data[6].value = 0; }
		if(res7.key != udata7.key){ vmask[i].data[7].key = 1; } else { vmask[i].data[7].key = 0; }
		if(res7.value != udata7.value){ vmask[i].data[7].value = 1; } else { vmask[i].data[7].value = 0; }
		if(res8.key != udata8.key){ vmask[i].data[8].key = 1; } else { vmask[i].data[8].key = 0; }
		if(res8.value != udata8.value){ vmask[i].data[8].value = 1; } else { vmask[i].data[8].value = 0; }
		if(res9.key != udata9.key){ vmask[i].data[9].key = 1; } else { vmask[i].data[9].key = 0; }
		if(res9.value != udata9.value){ vmask[i].data[9].value = 1; } else { vmask[i].data[9].value = 0; }
		if(res10.key != udata10.key){ vmask[i].data[10].key = 1; } else { vmask[i].data[10].key = 0; }
		if(res10.value != udata10.value){ vmask[i].data[10].value = 1; } else { vmask[i].data[10].value = 0; }
		if(res11.key != udata11.key){ vmask[i].data[11].key = 1; } else { vmask[i].data[11].key = 0; }
		if(res11.value != udata11.value){ vmask[i].data[11].value = 1; } else { vmask[i].data[11].value = 0; }
		if(res12.key != udata12.key){ vmask[i].data[12].key = 1; } else { vmask[i].data[12].key = 0; }
		if(res12.value != udata12.value){ vmask[i].data[12].value = 1; } else { vmask[i].data[12].value = 0; }
		if(res13.key != udata13.key){ vmask[i].data[13].key = 1; } else { vmask[i].data[13].key = 0; }
		if(res13.value != udata13.value){ vmask[i].data[13].value = 1; } else { vmask[i].data[13].value = 0; }
		if(res14.key != udata14.key){ vmask[i].data[14].key = 1; } else { vmask[i].data[14].key = 0; }
		if(res14.value != udata14.value){ vmask[i].data[14].value = 1; } else { vmask[i].data[14].value = 0; }
		if(res15.key != udata15.key){ vmask[i].data[15].key = 1; } else { vmask[i].data[15].key = 0; }
		if(res15.value != udata15.value){ vmask[i].data[15].value = 1; } else { vmask[i].data[15].value = 0; }
		
		uint32_type mask = convertvmasktouint32(vmask, i);
		cummvmask_sp = cummvmask_sp | mask;
		
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
		if(false){ cout<<"APPLY FUNC SEEN @ (vid1: "<<vid[0][0]<<" & vid2: "<<vid[0][1]<<"): res0.key: "<<res0.key<<", res0.value: "<<res0.value<<": udata0.key: "<<udata0.key<<", udata0.value: "<<udata0.value<<", data0.key: "<<data0.key<<", data0.value: "<<data0.value<<endl; }
		if(res0.key != udata0.key){ cout<<"apply: vid[0][0]: "<<vid[0][0]<<", vmask["<<i<<"].data[0].key: "<<vmask[i].data[0].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res0.value != udata0.value){ cout<<"apply: vid[0][1]: "<<vid[0][1]<<", vmask["<<i<<"].data[0].value: "<<vmask[i].data[0].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(false){ cout<<"APPLY FUNC SEEN @ (vid1: "<<vid[1][0]<<" & vid2: "<<vid[1][1]<<"): res1.key: "<<res1.key<<", res1.value: "<<res1.value<<": udata1.key: "<<udata1.key<<", udata1.value: "<<udata1.value<<", data1.key: "<<data1.key<<", data1.value: "<<data1.value<<endl; }
		if(res1.key != udata1.key){ cout<<"apply: vid[1][0]: "<<vid[1][0]<<", vmask["<<i<<"].data[1].key: "<<vmask[i].data[1].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res1.value != udata1.value){ cout<<"apply: vid[1][1]: "<<vid[1][1]<<", vmask["<<i<<"].data[1].value: "<<vmask[i].data[1].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(false){ cout<<"APPLY FUNC SEEN @ (vid1: "<<vid[2][0]<<" & vid2: "<<vid[2][1]<<"): res2.key: "<<res2.key<<", res2.value: "<<res2.value<<": udata2.key: "<<udata2.key<<", udata2.value: "<<udata2.value<<", data2.key: "<<data2.key<<", data2.value: "<<data2.value<<endl; }
		if(res2.key != udata2.key){ cout<<"apply: vid[2][0]: "<<vid[2][0]<<", vmask["<<i<<"].data[2].key: "<<vmask[i].data[2].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res2.value != udata2.value){ cout<<"apply: vid[2][1]: "<<vid[2][1]<<", vmask["<<i<<"].data[2].value: "<<vmask[i].data[2].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(false){ cout<<"APPLY FUNC SEEN @ (vid1: "<<vid[3][0]<<" & vid2: "<<vid[3][1]<<"): res3.key: "<<res3.key<<", res3.value: "<<res3.value<<": udata3.key: "<<udata3.key<<", udata3.value: "<<udata3.value<<", data3.key: "<<data3.key<<", data3.value: "<<data3.value<<endl; }
		if(res3.key != udata3.key){ cout<<"apply: vid[3][0]: "<<vid[3][0]<<", vmask["<<i<<"].data[3].key: "<<vmask[i].data[3].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res3.value != udata3.value){ cout<<"apply: vid[3][1]: "<<vid[3][1]<<", vmask["<<i<<"].data[3].value: "<<vmask[i].data[3].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(false){ cout<<"APPLY FUNC SEEN @ (vid1: "<<vid[4][0]<<" & vid2: "<<vid[4][1]<<"): res4.key: "<<res4.key<<", res4.value: "<<res4.value<<": udata4.key: "<<udata4.key<<", udata4.value: "<<udata4.value<<", data4.key: "<<data4.key<<", data4.value: "<<data4.value<<endl; }
		if(res4.key != udata4.key){ cout<<"apply: vid[4][0]: "<<vid[4][0]<<", vmask["<<i<<"].data[4].key: "<<vmask[i].data[4].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res4.value != udata4.value){ cout<<"apply: vid[4][1]: "<<vid[4][1]<<", vmask["<<i<<"].data[4].value: "<<vmask[i].data[4].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(false){ cout<<"APPLY FUNC SEEN @ (vid1: "<<vid[5][0]<<" & vid2: "<<vid[5][1]<<"): res5.key: "<<res5.key<<", res5.value: "<<res5.value<<": udata5.key: "<<udata5.key<<", udata5.value: "<<udata5.value<<", data5.key: "<<data5.key<<", data5.value: "<<data5.value<<endl; }
		if(res5.key != udata5.key){ cout<<"apply: vid[5][0]: "<<vid[5][0]<<", vmask["<<i<<"].data[5].key: "<<vmask[i].data[5].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res5.value != udata5.value){ cout<<"apply: vid[5][1]: "<<vid[5][1]<<", vmask["<<i<<"].data[5].value: "<<vmask[i].data[5].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(false){ cout<<"APPLY FUNC SEEN @ (vid1: "<<vid[6][0]<<" & vid2: "<<vid[6][1]<<"): res6.key: "<<res6.key<<", res6.value: "<<res6.value<<": udata6.key: "<<udata6.key<<", udata6.value: "<<udata6.value<<", data6.key: "<<data6.key<<", data6.value: "<<data6.value<<endl; }
		if(res6.key != udata6.key){ cout<<"apply: vid[6][0]: "<<vid[6][0]<<", vmask["<<i<<"].data[6].key: "<<vmask[i].data[6].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res6.value != udata6.value){ cout<<"apply: vid[6][1]: "<<vid[6][1]<<", vmask["<<i<<"].data[6].value: "<<vmask[i].data[6].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(false){ cout<<"APPLY FUNC SEEN @ (vid1: "<<vid[7][0]<<" & vid2: "<<vid[7][1]<<"): res7.key: "<<res7.key<<", res7.value: "<<res7.value<<": udata7.key: "<<udata7.key<<", udata7.value: "<<udata7.value<<", data7.key: "<<data7.key<<", data7.value: "<<data7.value<<endl; }
		if(res7.key != udata7.key){ cout<<"apply: vid[7][0]: "<<vid[7][0]<<", vmask["<<i<<"].data[7].key: "<<vmask[i].data[7].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res7.value != udata7.value){ cout<<"apply: vid[7][1]: "<<vid[7][1]<<", vmask["<<i<<"].data[7].value: "<<vmask[i].data[7].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(false){ cout<<"APPLY FUNC SEEN @ (vid1: "<<vid[8][0]<<" & vid2: "<<vid[8][1]<<"): res8.key: "<<res8.key<<", res8.value: "<<res8.value<<": udata8.key: "<<udata8.key<<", udata8.value: "<<udata8.value<<", data8.key: "<<data8.key<<", data8.value: "<<data8.value<<endl; }
		if(res8.key != udata8.key){ cout<<"apply: vid[8][0]: "<<vid[8][0]<<", vmask["<<i<<"].data[8].key: "<<vmask[i].data[8].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res8.value != udata8.value){ cout<<"apply: vid[8][1]: "<<vid[8][1]<<", vmask["<<i<<"].data[8].value: "<<vmask[i].data[8].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(false){ cout<<"APPLY FUNC SEEN @ (vid1: "<<vid[9][0]<<" & vid2: "<<vid[9][1]<<"): res9.key: "<<res9.key<<", res9.value: "<<res9.value<<": udata9.key: "<<udata9.key<<", udata9.value: "<<udata9.value<<", data9.key: "<<data9.key<<", data9.value: "<<data9.value<<endl; }
		if(res9.key != udata9.key){ cout<<"apply: vid[9][0]: "<<vid[9][0]<<", vmask["<<i<<"].data[9].key: "<<vmask[i].data[9].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res9.value != udata9.value){ cout<<"apply: vid[9][1]: "<<vid[9][1]<<", vmask["<<i<<"].data[9].value: "<<vmask[i].data[9].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(false){ cout<<"APPLY FUNC SEEN @ (vid1: "<<vid[10][0]<<" & vid2: "<<vid[10][1]<<"): res10.key: "<<res10.key<<", res10.value: "<<res10.value<<": udata10.key: "<<udata10.key<<", udata10.value: "<<udata10.value<<", data10.key: "<<data10.key<<", data10.value: "<<data10.value<<endl; }
		if(res10.key != udata10.key){ cout<<"apply: vid[10][0]: "<<vid[10][0]<<", vmask["<<i<<"].data[10].key: "<<vmask[i].data[10].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res10.value != udata10.value){ cout<<"apply: vid[10][1]: "<<vid[10][1]<<", vmask["<<i<<"].data[10].value: "<<vmask[i].data[10].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(false){ cout<<"APPLY FUNC SEEN @ (vid1: "<<vid[11][0]<<" & vid2: "<<vid[11][1]<<"): res11.key: "<<res11.key<<", res11.value: "<<res11.value<<": udata11.key: "<<udata11.key<<", udata11.value: "<<udata11.value<<", data11.key: "<<data11.key<<", data11.value: "<<data11.value<<endl; }
		if(res11.key != udata11.key){ cout<<"apply: vid[11][0]: "<<vid[11][0]<<", vmask["<<i<<"].data[11].key: "<<vmask[i].data[11].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res11.value != udata11.value){ cout<<"apply: vid[11][1]: "<<vid[11][1]<<", vmask["<<i<<"].data[11].value: "<<vmask[i].data[11].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(false){ cout<<"APPLY FUNC SEEN @ (vid1: "<<vid[12][0]<<" & vid2: "<<vid[12][1]<<"): res12.key: "<<res12.key<<", res12.value: "<<res12.value<<": udata12.key: "<<udata12.key<<", udata12.value: "<<udata12.value<<", data12.key: "<<data12.key<<", data12.value: "<<data12.value<<endl; }
		if(res12.key != udata12.key){ cout<<"apply: vid[12][0]: "<<vid[12][0]<<", vmask["<<i<<"].data[12].key: "<<vmask[i].data[12].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res12.value != udata12.value){ cout<<"apply: vid[12][1]: "<<vid[12][1]<<", vmask["<<i<<"].data[12].value: "<<vmask[i].data[12].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(false){ cout<<"APPLY FUNC SEEN @ (vid1: "<<vid[13][0]<<" & vid2: "<<vid[13][1]<<"): res13.key: "<<res13.key<<", res13.value: "<<res13.value<<": udata13.key: "<<udata13.key<<", udata13.value: "<<udata13.value<<", data13.key: "<<data13.key<<", data13.value: "<<data13.value<<endl; }
		if(res13.key != udata13.key){ cout<<"apply: vid[13][0]: "<<vid[13][0]<<", vmask["<<i<<"].data[13].key: "<<vmask[i].data[13].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res13.value != udata13.value){ cout<<"apply: vid[13][1]: "<<vid[13][1]<<", vmask["<<i<<"].data[13].value: "<<vmask[i].data[13].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(false){ cout<<"APPLY FUNC SEEN @ (vid1: "<<vid[14][0]<<" & vid2: "<<vid[14][1]<<"): res14.key: "<<res14.key<<", res14.value: "<<res14.value<<": udata14.key: "<<udata14.key<<", udata14.value: "<<udata14.value<<", data14.key: "<<data14.key<<", data14.value: "<<data14.value<<endl; }
		if(res14.key != udata14.key){ cout<<"apply: vid[14][0]: "<<vid[14][0]<<", vmask["<<i<<"].data[14].key: "<<vmask[i].data[14].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res14.value != udata14.value){ cout<<"apply: vid[14][1]: "<<vid[14][1]<<", vmask["<<i<<"].data[14].value: "<<vmask[i].data[14].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(false){ cout<<"APPLY FUNC SEEN @ (vid1: "<<vid[15][0]<<" & vid2: "<<vid[15][1]<<"): res15.key: "<<res15.key<<", res15.value: "<<res15.value<<": udata15.key: "<<udata15.key<<", udata15.value: "<<udata15.value<<", data15.key: "<<data15.key<<", data15.value: "<<data15.value<<endl; }
		if(res15.key != udata15.key){ cout<<"apply: vid[15][0]: "<<vid[15][0]<<", vmask["<<i<<"].data[15].key: "<<vmask[i].data[15].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res15.value != udata15.value){ cout<<"apply: vid[15][1]: "<<vid[15][1]<<", vmask["<<i<<"].data[15].value: "<<vmask[i].data[15].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		#endif
		#ifdef _DEBUGMODE_STATS
		if(res0.key != udata0.key){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res0.value != udata0.value){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res1.key != udata1.key){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res1.value != udata1.value){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res2.key != udata2.key){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res2.value != udata2.value){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res3.key != udata3.key){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res3.value != udata3.value){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res4.key != udata4.key){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res4.value != udata4.value){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res5.key != udata5.key){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res5.value != udata5.value){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res6.key != udata6.key){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res6.value != udata6.value){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res7.key != udata7.key){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res7.value != udata7.value){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res8.key != udata8.key){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res8.value != udata8.value){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res9.key != udata9.key){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res9.value != udata9.value){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res10.key != udata10.key){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res10.value != udata10.value){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res11.key != udata11.key){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res11.value != udata11.value){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res12.key != udata12.key){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res12.value != udata12.value){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res13.key != udata13.key){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res13.value != udata13.value){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res14.key != udata14.key){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res14.value != udata14.value){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res15.key != udata15.key){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res15.value != udata15.value){ actsutilityobj->globalstats_countactvvsseen(1); }
		#endif
	}
	return cummvmask_sp;
}

void
	#ifdef SW 
	acts::
	#endif
spreadvdata(bool_type enable, keyvalue16_type buffer0[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue16_type buffer1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue16_type buffer2[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue16_type buffer3[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue16_type buffer4[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue16_type buffer5[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue16_type buffer6[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue16_type buffer7[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue16_type buffer8[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue16_type buffer9[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue16_type buffer10[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue16_type buffer11[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue16_type buffer12[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue16_type buffer13[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue16_type buffer14[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue16_type buffer15[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue16_type buffer16[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue16_type tail[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], globalparams_t globalparams){
	if(enable == OFF){ return; }
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
	cout<<"spreadvdata: spread (vdata) function called."<<endl;
	#endif 
	analysis_type analysis_loopcount = REDUCEBUFFERSZ;
	
	buffer_type reducebuffersz = globalparams.SIZE_REDUCE / 2;
	
	keyvalue16_type keyvalue0_vault0;
	keyvalue16_type keyvalue1_vault0;
	keyvalue16_type keyvalue2_vault0;
	keyvalue16_type keyvalue3_vault0;
	keyvalue16_type keyvalue4_vault0;
	keyvalue16_type keyvalue5_vault0;
	keyvalue16_type keyvalue6_vault0;
	keyvalue16_type keyvalue7_vault0;
	keyvalue16_type keyvalue8_vault0;
	keyvalue16_type keyvalue9_vault0;
	keyvalue16_type keyvalue10_vault0;
	keyvalue16_type keyvalue11_vault0;
	keyvalue16_type keyvalue12_vault0;
	keyvalue16_type keyvalue13_vault0;
	keyvalue16_type keyvalue14_vault0;
	keyvalue16_type keyvalue15_vault0;
	
	keyvalue16_type keyvalue0_vault1;
	keyvalue16_type keyvalue1_vault1;
	keyvalue16_type keyvalue2_vault1;
	keyvalue16_type keyvalue3_vault1;
	keyvalue16_type keyvalue4_vault1;
	keyvalue16_type keyvalue5_vault1;
	keyvalue16_type keyvalue6_vault1;
	keyvalue16_type keyvalue7_vault1;
	keyvalue16_type keyvalue8_vault1;
	keyvalue16_type keyvalue9_vault1;
	keyvalue16_type keyvalue10_vault1;
	keyvalue16_type keyvalue11_vault1;
	keyvalue16_type keyvalue12_vault1;
	keyvalue16_type keyvalue13_vault1;
	keyvalue16_type keyvalue14_vault1;
	keyvalue16_type keyvalue15_vault1;
	
	keyvalue16_type keyvalue0_vault2;
	keyvalue16_type keyvalue1_vault2;
	keyvalue16_type keyvalue2_vault2;
	keyvalue16_type keyvalue3_vault2;
	keyvalue16_type keyvalue4_vault2;
	keyvalue16_type keyvalue5_vault2;
	keyvalue16_type keyvalue6_vault2;
	keyvalue16_type keyvalue7_vault2;
	keyvalue16_type keyvalue8_vault2;
	keyvalue16_type keyvalue9_vault2;
	keyvalue16_type keyvalue10_vault2;
	keyvalue16_type keyvalue11_vault2;
	keyvalue16_type keyvalue12_vault2;
	keyvalue16_type keyvalue13_vault2;
	keyvalue16_type keyvalue14_vault2;
	keyvalue16_type keyvalue15_vault2;
	
	keyvalue16_type keyvalue0_vault3;
	keyvalue16_type keyvalue1_vault3;
	keyvalue16_type keyvalue2_vault3;
	keyvalue16_type keyvalue3_vault3;
	keyvalue16_type keyvalue4_vault3;
	keyvalue16_type keyvalue5_vault3;
	keyvalue16_type keyvalue6_vault3;
	keyvalue16_type keyvalue7_vault3;
	keyvalue16_type keyvalue8_vault3;
	keyvalue16_type keyvalue9_vault3;
	keyvalue16_type keyvalue10_vault3;
	keyvalue16_type keyvalue11_vault3;
	keyvalue16_type keyvalue12_vault3;
	keyvalue16_type keyvalue13_vault3;
	keyvalue16_type keyvalue14_vault3;
	keyvalue16_type keyvalue15_vault3;
	
	keyvalue16_type keyvalue0_vault4;
	keyvalue16_type keyvalue1_vault4;
	keyvalue16_type keyvalue2_vault4;
	keyvalue16_type keyvalue3_vault4;
	keyvalue16_type keyvalue4_vault4;
	keyvalue16_type keyvalue5_vault4;
	keyvalue16_type keyvalue6_vault4;
	keyvalue16_type keyvalue7_vault4;
	keyvalue16_type keyvalue8_vault4;
	keyvalue16_type keyvalue9_vault4;
	keyvalue16_type keyvalue10_vault4;
	keyvalue16_type keyvalue11_vault4;
	keyvalue16_type keyvalue12_vault4;
	keyvalue16_type keyvalue13_vault4;
	keyvalue16_type keyvalue14_vault4;
	keyvalue16_type keyvalue15_vault4;
	
	keyvalue16_type keyvalue0_vault5;
	keyvalue16_type keyvalue1_vault5;
	keyvalue16_type keyvalue2_vault5;
	keyvalue16_type keyvalue3_vault5;
	keyvalue16_type keyvalue4_vault5;
	keyvalue16_type keyvalue5_vault5;
	keyvalue16_type keyvalue6_vault5;
	keyvalue16_type keyvalue7_vault5;
	keyvalue16_type keyvalue8_vault5;
	keyvalue16_type keyvalue9_vault5;
	keyvalue16_type keyvalue10_vault5;
	keyvalue16_type keyvalue11_vault5;
	keyvalue16_type keyvalue12_vault5;
	keyvalue16_type keyvalue13_vault5;
	keyvalue16_type keyvalue14_vault5;
	keyvalue16_type keyvalue15_vault5;
	
	keyvalue16_type keyvalue0_vault6;
	keyvalue16_type keyvalue1_vault6;
	keyvalue16_type keyvalue2_vault6;
	keyvalue16_type keyvalue3_vault6;
	keyvalue16_type keyvalue4_vault6;
	keyvalue16_type keyvalue5_vault6;
	keyvalue16_type keyvalue6_vault6;
	keyvalue16_type keyvalue7_vault6;
	keyvalue16_type keyvalue8_vault6;
	keyvalue16_type keyvalue9_vault6;
	keyvalue16_type keyvalue10_vault6;
	keyvalue16_type keyvalue11_vault6;
	keyvalue16_type keyvalue12_vault6;
	keyvalue16_type keyvalue13_vault6;
	keyvalue16_type keyvalue14_vault6;
	keyvalue16_type keyvalue15_vault6;
	
	keyvalue16_type keyvalue0_vault7;
	keyvalue16_type keyvalue1_vault7;
	keyvalue16_type keyvalue2_vault7;
	keyvalue16_type keyvalue3_vault7;
	keyvalue16_type keyvalue4_vault7;
	keyvalue16_type keyvalue5_vault7;
	keyvalue16_type keyvalue6_vault7;
	keyvalue16_type keyvalue7_vault7;
	keyvalue16_type keyvalue8_vault7;
	keyvalue16_type keyvalue9_vault7;
	keyvalue16_type keyvalue10_vault7;
	keyvalue16_type keyvalue11_vault7;
	keyvalue16_type keyvalue12_vault7;
	keyvalue16_type keyvalue13_vault7;
	keyvalue16_type keyvalue14_vault7;
	keyvalue16_type keyvalue15_vault7;
	
	keyvalue16_type keyvalue0_vault8;
	keyvalue16_type keyvalue1_vault8;
	keyvalue16_type keyvalue2_vault8;
	keyvalue16_type keyvalue3_vault8;
	keyvalue16_type keyvalue4_vault8;
	keyvalue16_type keyvalue5_vault8;
	keyvalue16_type keyvalue6_vault8;
	keyvalue16_type keyvalue7_vault8;
	keyvalue16_type keyvalue8_vault8;
	keyvalue16_type keyvalue9_vault8;
	keyvalue16_type keyvalue10_vault8;
	keyvalue16_type keyvalue11_vault8;
	keyvalue16_type keyvalue12_vault8;
	keyvalue16_type keyvalue13_vault8;
	keyvalue16_type keyvalue14_vault8;
	keyvalue16_type keyvalue15_vault8;
	
	keyvalue16_type keyvalue0_vault9;
	keyvalue16_type keyvalue1_vault9;
	keyvalue16_type keyvalue2_vault9;
	keyvalue16_type keyvalue3_vault9;
	keyvalue16_type keyvalue4_vault9;
	keyvalue16_type keyvalue5_vault9;
	keyvalue16_type keyvalue6_vault9;
	keyvalue16_type keyvalue7_vault9;
	keyvalue16_type keyvalue8_vault9;
	keyvalue16_type keyvalue9_vault9;
	keyvalue16_type keyvalue10_vault9;
	keyvalue16_type keyvalue11_vault9;
	keyvalue16_type keyvalue12_vault9;
	keyvalue16_type keyvalue13_vault9;
	keyvalue16_type keyvalue14_vault9;
	keyvalue16_type keyvalue15_vault9;
	
	keyvalue16_type keyvalue0_vault10;
	keyvalue16_type keyvalue1_vault10;
	keyvalue16_type keyvalue2_vault10;
	keyvalue16_type keyvalue3_vault10;
	keyvalue16_type keyvalue4_vault10;
	keyvalue16_type keyvalue5_vault10;
	keyvalue16_type keyvalue6_vault10;
	keyvalue16_type keyvalue7_vault10;
	keyvalue16_type keyvalue8_vault10;
	keyvalue16_type keyvalue9_vault10;
	keyvalue16_type keyvalue10_vault10;
	keyvalue16_type keyvalue11_vault10;
	keyvalue16_type keyvalue12_vault10;
	keyvalue16_type keyvalue13_vault10;
	keyvalue16_type keyvalue14_vault10;
	keyvalue16_type keyvalue15_vault10;
	
	keyvalue16_type keyvalue0_vault11;
	keyvalue16_type keyvalue1_vault11;
	keyvalue16_type keyvalue2_vault11;
	keyvalue16_type keyvalue3_vault11;
	keyvalue16_type keyvalue4_vault11;
	keyvalue16_type keyvalue5_vault11;
	keyvalue16_type keyvalue6_vault11;
	keyvalue16_type keyvalue7_vault11;
	keyvalue16_type keyvalue8_vault11;
	keyvalue16_type keyvalue9_vault11;
	keyvalue16_type keyvalue10_vault11;
	keyvalue16_type keyvalue11_vault11;
	keyvalue16_type keyvalue12_vault11;
	keyvalue16_type keyvalue13_vault11;
	keyvalue16_type keyvalue14_vault11;
	keyvalue16_type keyvalue15_vault11;
	
	keyvalue16_type keyvalue0_vault12;
	keyvalue16_type keyvalue1_vault12;
	keyvalue16_type keyvalue2_vault12;
	keyvalue16_type keyvalue3_vault12;
	keyvalue16_type keyvalue4_vault12;
	keyvalue16_type keyvalue5_vault12;
	keyvalue16_type keyvalue6_vault12;
	keyvalue16_type keyvalue7_vault12;
	keyvalue16_type keyvalue8_vault12;
	keyvalue16_type keyvalue9_vault12;
	keyvalue16_type keyvalue10_vault12;
	keyvalue16_type keyvalue11_vault12;
	keyvalue16_type keyvalue12_vault12;
	keyvalue16_type keyvalue13_vault12;
	keyvalue16_type keyvalue14_vault12;
	keyvalue16_type keyvalue15_vault12;
	
	keyvalue16_type keyvalue0_vault13;
	keyvalue16_type keyvalue1_vault13;
	keyvalue16_type keyvalue2_vault13;
	keyvalue16_type keyvalue3_vault13;
	keyvalue16_type keyvalue4_vault13;
	keyvalue16_type keyvalue5_vault13;
	keyvalue16_type keyvalue6_vault13;
	keyvalue16_type keyvalue7_vault13;
	keyvalue16_type keyvalue8_vault13;
	keyvalue16_type keyvalue9_vault13;
	keyvalue16_type keyvalue10_vault13;
	keyvalue16_type keyvalue11_vault13;
	keyvalue16_type keyvalue12_vault13;
	keyvalue16_type keyvalue13_vault13;
	keyvalue16_type keyvalue14_vault13;
	keyvalue16_type keyvalue15_vault13;
	
	keyvalue16_type keyvalue0_vault14;
	keyvalue16_type keyvalue1_vault14;
	keyvalue16_type keyvalue2_vault14;
	keyvalue16_type keyvalue3_vault14;
	keyvalue16_type keyvalue4_vault14;
	keyvalue16_type keyvalue5_vault14;
	keyvalue16_type keyvalue6_vault14;
	keyvalue16_type keyvalue7_vault14;
	keyvalue16_type keyvalue8_vault14;
	keyvalue16_type keyvalue9_vault14;
	keyvalue16_type keyvalue10_vault14;
	keyvalue16_type keyvalue11_vault14;
	keyvalue16_type keyvalue12_vault14;
	keyvalue16_type keyvalue13_vault14;
	keyvalue16_type keyvalue14_vault14;
	keyvalue16_type keyvalue15_vault14;
	
	keyvalue16_type keyvalue0_vault15;
	keyvalue16_type keyvalue1_vault15;
	keyvalue16_type keyvalue2_vault15;
	keyvalue16_type keyvalue3_vault15;
	keyvalue16_type keyvalue4_vault15;
	keyvalue16_type keyvalue5_vault15;
	keyvalue16_type keyvalue6_vault15;
	keyvalue16_type keyvalue7_vault15;
	keyvalue16_type keyvalue8_vault15;
	keyvalue16_type keyvalue9_vault15;
	keyvalue16_type keyvalue10_vault15;
	keyvalue16_type keyvalue11_vault15;
	keyvalue16_type keyvalue12_vault15;
	keyvalue16_type keyvalue13_vault15;
	keyvalue16_type keyvalue14_vault15;
	keyvalue16_type keyvalue15_vault15;
	
	keyvalue16_type keyvalue0_vault16;
	keyvalue16_type keyvalue1_vault16;
	keyvalue16_type keyvalue2_vault16;
	keyvalue16_type keyvalue3_vault16;
	keyvalue16_type keyvalue4_vault16;
	keyvalue16_type keyvalue5_vault16;
	keyvalue16_type keyvalue6_vault16;
	keyvalue16_type keyvalue7_vault16;
	keyvalue16_type keyvalue8_vault16;
	keyvalue16_type keyvalue9_vault16;
	keyvalue16_type keyvalue10_vault16;
	keyvalue16_type keyvalue11_vault16;
	keyvalue16_type keyvalue12_vault16;
	keyvalue16_type keyvalue13_vault16;
	keyvalue16_type keyvalue14_vault16;
	keyvalue16_type keyvalue15_vault16;
	
	keyvalue16_type keyvalue0_vault17;
	keyvalue16_type keyvalue1_vault17;
	keyvalue16_type keyvalue2_vault17;
	keyvalue16_type keyvalue3_vault17;
	keyvalue16_type keyvalue4_vault17;
	keyvalue16_type keyvalue5_vault17;
	keyvalue16_type keyvalue6_vault17;
	keyvalue16_type keyvalue7_vault17;
	keyvalue16_type keyvalue8_vault17;
	keyvalue16_type keyvalue9_vault17;
	keyvalue16_type keyvalue10_vault17;
	keyvalue16_type keyvalue11_vault17;
	keyvalue16_type keyvalue12_vault17;
	keyvalue16_type keyvalue13_vault17;
	keyvalue16_type keyvalue14_vault17;
	keyvalue16_type keyvalue15_vault17;
	
	
	for (buffer_type i=0; i<reducebuffersz; i++){ // 8, 16, BLOCKRAM_SIZE
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE II=1
		keyvalue0_vault1 = buffer0[0][i]; 
		keyvalue1_vault1 = buffer0[1][i]; 
		keyvalue2_vault1 = buffer0[2][i]; 
		keyvalue3_vault1 = buffer0[3][i]; 
		keyvalue4_vault1 = buffer0[4][i]; 
		keyvalue5_vault1 = buffer0[5][i]; 
		keyvalue6_vault1 = buffer0[6][i]; 
		keyvalue7_vault1 = buffer0[7][i]; 
		keyvalue8_vault1 = buffer0[8][i]; 
		keyvalue9_vault1 = buffer0[9][i]; 
		keyvalue10_vault1 = buffer0[10][i]; 
		keyvalue11_vault1 = buffer0[11][i]; 
		keyvalue12_vault1 = buffer0[12][i]; 
		keyvalue13_vault1 = buffer0[13][i]; 
		keyvalue14_vault1 = buffer0[14][i]; 
		keyvalue15_vault1 = buffer0[15][i]; 
			
	
		keyvalue0_vault2 = keyvalue0_vault1;
		buffer1[0][i] = keyvalue0_vault1;
		keyvalue1_vault2 = keyvalue1_vault1;
		buffer1[1][i] = keyvalue1_vault1;
		keyvalue2_vault2 = keyvalue2_vault1;
		buffer1[2][i] = keyvalue2_vault1;
		keyvalue3_vault2 = keyvalue3_vault1;
		buffer1[3][i] = keyvalue3_vault1;
		keyvalue4_vault2 = keyvalue4_vault1;
		buffer1[4][i] = keyvalue4_vault1;
		keyvalue5_vault2 = keyvalue5_vault1;
		buffer1[5][i] = keyvalue5_vault1;
		keyvalue6_vault2 = keyvalue6_vault1;
		buffer1[6][i] = keyvalue6_vault1;
		keyvalue7_vault2 = keyvalue7_vault1;
		buffer1[7][i] = keyvalue7_vault1;
		keyvalue8_vault2 = keyvalue8_vault1;
		buffer1[8][i] = keyvalue8_vault1;
		keyvalue9_vault2 = keyvalue9_vault1;
		buffer1[9][i] = keyvalue9_vault1;
		keyvalue10_vault2 = keyvalue10_vault1;
		buffer1[10][i] = keyvalue10_vault1;
		keyvalue11_vault2 = keyvalue11_vault1;
		buffer1[11][i] = keyvalue11_vault1;
		keyvalue12_vault2 = keyvalue12_vault1;
		buffer1[12][i] = keyvalue12_vault1;
		keyvalue13_vault2 = keyvalue13_vault1;
		buffer1[13][i] = keyvalue13_vault1;
		keyvalue14_vault2 = keyvalue14_vault1;
		buffer1[14][i] = keyvalue14_vault1;
		keyvalue15_vault2 = keyvalue15_vault1;
		buffer1[15][i] = keyvalue15_vault1;
	
		keyvalue0_vault3 = keyvalue0_vault2;
		buffer2[0][i] = keyvalue0_vault2;
		keyvalue1_vault3 = keyvalue1_vault2;
		buffer2[1][i] = keyvalue1_vault2;
		keyvalue2_vault3 = keyvalue2_vault2;
		buffer2[2][i] = keyvalue2_vault2;
		keyvalue3_vault3 = keyvalue3_vault2;
		buffer2[3][i] = keyvalue3_vault2;
		keyvalue4_vault3 = keyvalue4_vault2;
		buffer2[4][i] = keyvalue4_vault2;
		keyvalue5_vault3 = keyvalue5_vault2;
		buffer2[5][i] = keyvalue5_vault2;
		keyvalue6_vault3 = keyvalue6_vault2;
		buffer2[6][i] = keyvalue6_vault2;
		keyvalue7_vault3 = keyvalue7_vault2;
		buffer2[7][i] = keyvalue7_vault2;
		keyvalue8_vault3 = keyvalue8_vault2;
		buffer2[8][i] = keyvalue8_vault2;
		keyvalue9_vault3 = keyvalue9_vault2;
		buffer2[9][i] = keyvalue9_vault2;
		keyvalue10_vault3 = keyvalue10_vault2;
		buffer2[10][i] = keyvalue10_vault2;
		keyvalue11_vault3 = keyvalue11_vault2;
		buffer2[11][i] = keyvalue11_vault2;
		keyvalue12_vault3 = keyvalue12_vault2;
		buffer2[12][i] = keyvalue12_vault2;
		keyvalue13_vault3 = keyvalue13_vault2;
		buffer2[13][i] = keyvalue13_vault2;
		keyvalue14_vault3 = keyvalue14_vault2;
		buffer2[14][i] = keyvalue14_vault2;
		keyvalue15_vault3 = keyvalue15_vault2;
		buffer2[15][i] = keyvalue15_vault2;
	
		keyvalue0_vault4 = keyvalue0_vault3;
		buffer3[0][i] = keyvalue0_vault3;
		keyvalue1_vault4 = keyvalue1_vault3;
		buffer3[1][i] = keyvalue1_vault3;
		keyvalue2_vault4 = keyvalue2_vault3;
		buffer3[2][i] = keyvalue2_vault3;
		keyvalue3_vault4 = keyvalue3_vault3;
		buffer3[3][i] = keyvalue3_vault3;
		keyvalue4_vault4 = keyvalue4_vault3;
		buffer3[4][i] = keyvalue4_vault3;
		keyvalue5_vault4 = keyvalue5_vault3;
		buffer3[5][i] = keyvalue5_vault3;
		keyvalue6_vault4 = keyvalue6_vault3;
		buffer3[6][i] = keyvalue6_vault3;
		keyvalue7_vault4 = keyvalue7_vault3;
		buffer3[7][i] = keyvalue7_vault3;
		keyvalue8_vault4 = keyvalue8_vault3;
		buffer3[8][i] = keyvalue8_vault3;
		keyvalue9_vault4 = keyvalue9_vault3;
		buffer3[9][i] = keyvalue9_vault3;
		keyvalue10_vault4 = keyvalue10_vault3;
		buffer3[10][i] = keyvalue10_vault3;
		keyvalue11_vault4 = keyvalue11_vault3;
		buffer3[11][i] = keyvalue11_vault3;
		keyvalue12_vault4 = keyvalue12_vault3;
		buffer3[12][i] = keyvalue12_vault3;
		keyvalue13_vault4 = keyvalue13_vault3;
		buffer3[13][i] = keyvalue13_vault3;
		keyvalue14_vault4 = keyvalue14_vault3;
		buffer3[14][i] = keyvalue14_vault3;
		keyvalue15_vault4 = keyvalue15_vault3;
		buffer3[15][i] = keyvalue15_vault3;
	
		keyvalue0_vault5 = keyvalue0_vault4;
		buffer4[0][i] = keyvalue0_vault4;
		keyvalue1_vault5 = keyvalue1_vault4;
		buffer4[1][i] = keyvalue1_vault4;
		keyvalue2_vault5 = keyvalue2_vault4;
		buffer4[2][i] = keyvalue2_vault4;
		keyvalue3_vault5 = keyvalue3_vault4;
		buffer4[3][i] = keyvalue3_vault4;
		keyvalue4_vault5 = keyvalue4_vault4;
		buffer4[4][i] = keyvalue4_vault4;
		keyvalue5_vault5 = keyvalue5_vault4;
		buffer4[5][i] = keyvalue5_vault4;
		keyvalue6_vault5 = keyvalue6_vault4;
		buffer4[6][i] = keyvalue6_vault4;
		keyvalue7_vault5 = keyvalue7_vault4;
		buffer4[7][i] = keyvalue7_vault4;
		keyvalue8_vault5 = keyvalue8_vault4;
		buffer4[8][i] = keyvalue8_vault4;
		keyvalue9_vault5 = keyvalue9_vault4;
		buffer4[9][i] = keyvalue9_vault4;
		keyvalue10_vault5 = keyvalue10_vault4;
		buffer4[10][i] = keyvalue10_vault4;
		keyvalue11_vault5 = keyvalue11_vault4;
		buffer4[11][i] = keyvalue11_vault4;
		keyvalue12_vault5 = keyvalue12_vault4;
		buffer4[12][i] = keyvalue12_vault4;
		keyvalue13_vault5 = keyvalue13_vault4;
		buffer4[13][i] = keyvalue13_vault4;
		keyvalue14_vault5 = keyvalue14_vault4;
		buffer4[14][i] = keyvalue14_vault4;
		keyvalue15_vault5 = keyvalue15_vault4;
		buffer4[15][i] = keyvalue15_vault4;
	
		keyvalue0_vault6 = keyvalue0_vault5;
		buffer5[0][i] = keyvalue0_vault5;
		keyvalue1_vault6 = keyvalue1_vault5;
		buffer5[1][i] = keyvalue1_vault5;
		keyvalue2_vault6 = keyvalue2_vault5;
		buffer5[2][i] = keyvalue2_vault5;
		keyvalue3_vault6 = keyvalue3_vault5;
		buffer5[3][i] = keyvalue3_vault5;
		keyvalue4_vault6 = keyvalue4_vault5;
		buffer5[4][i] = keyvalue4_vault5;
		keyvalue5_vault6 = keyvalue5_vault5;
		buffer5[5][i] = keyvalue5_vault5;
		keyvalue6_vault6 = keyvalue6_vault5;
		buffer5[6][i] = keyvalue6_vault5;
		keyvalue7_vault6 = keyvalue7_vault5;
		buffer5[7][i] = keyvalue7_vault5;
		keyvalue8_vault6 = keyvalue8_vault5;
		buffer5[8][i] = keyvalue8_vault5;
		keyvalue9_vault6 = keyvalue9_vault5;
		buffer5[9][i] = keyvalue9_vault5;
		keyvalue10_vault6 = keyvalue10_vault5;
		buffer5[10][i] = keyvalue10_vault5;
		keyvalue11_vault6 = keyvalue11_vault5;
		buffer5[11][i] = keyvalue11_vault5;
		keyvalue12_vault6 = keyvalue12_vault5;
		buffer5[12][i] = keyvalue12_vault5;
		keyvalue13_vault6 = keyvalue13_vault5;
		buffer5[13][i] = keyvalue13_vault5;
		keyvalue14_vault6 = keyvalue14_vault5;
		buffer5[14][i] = keyvalue14_vault5;
		keyvalue15_vault6 = keyvalue15_vault5;
		buffer5[15][i] = keyvalue15_vault5;
	
		keyvalue0_vault7 = keyvalue0_vault6;
		buffer6[0][i] = keyvalue0_vault6;
		keyvalue1_vault7 = keyvalue1_vault6;
		buffer6[1][i] = keyvalue1_vault6;
		keyvalue2_vault7 = keyvalue2_vault6;
		buffer6[2][i] = keyvalue2_vault6;
		keyvalue3_vault7 = keyvalue3_vault6;
		buffer6[3][i] = keyvalue3_vault6;
		keyvalue4_vault7 = keyvalue4_vault6;
		buffer6[4][i] = keyvalue4_vault6;
		keyvalue5_vault7 = keyvalue5_vault6;
		buffer6[5][i] = keyvalue5_vault6;
		keyvalue6_vault7 = keyvalue6_vault6;
		buffer6[6][i] = keyvalue6_vault6;
		keyvalue7_vault7 = keyvalue7_vault6;
		buffer6[7][i] = keyvalue7_vault6;
		keyvalue8_vault7 = keyvalue8_vault6;
		buffer6[8][i] = keyvalue8_vault6;
		keyvalue9_vault7 = keyvalue9_vault6;
		buffer6[9][i] = keyvalue9_vault6;
		keyvalue10_vault7 = keyvalue10_vault6;
		buffer6[10][i] = keyvalue10_vault6;
		keyvalue11_vault7 = keyvalue11_vault6;
		buffer6[11][i] = keyvalue11_vault6;
		keyvalue12_vault7 = keyvalue12_vault6;
		buffer6[12][i] = keyvalue12_vault6;
		keyvalue13_vault7 = keyvalue13_vault6;
		buffer6[13][i] = keyvalue13_vault6;
		keyvalue14_vault7 = keyvalue14_vault6;
		buffer6[14][i] = keyvalue14_vault6;
		keyvalue15_vault7 = keyvalue15_vault6;
		buffer6[15][i] = keyvalue15_vault6;
	
		keyvalue0_vault8 = keyvalue0_vault7;
		buffer7[0][i] = keyvalue0_vault7;
		keyvalue1_vault8 = keyvalue1_vault7;
		buffer7[1][i] = keyvalue1_vault7;
		keyvalue2_vault8 = keyvalue2_vault7;
		buffer7[2][i] = keyvalue2_vault7;
		keyvalue3_vault8 = keyvalue3_vault7;
		buffer7[3][i] = keyvalue3_vault7;
		keyvalue4_vault8 = keyvalue4_vault7;
		buffer7[4][i] = keyvalue4_vault7;
		keyvalue5_vault8 = keyvalue5_vault7;
		buffer7[5][i] = keyvalue5_vault7;
		keyvalue6_vault8 = keyvalue6_vault7;
		buffer7[6][i] = keyvalue6_vault7;
		keyvalue7_vault8 = keyvalue7_vault7;
		buffer7[7][i] = keyvalue7_vault7;
		keyvalue8_vault8 = keyvalue8_vault7;
		buffer7[8][i] = keyvalue8_vault7;
		keyvalue9_vault8 = keyvalue9_vault7;
		buffer7[9][i] = keyvalue9_vault7;
		keyvalue10_vault8 = keyvalue10_vault7;
		buffer7[10][i] = keyvalue10_vault7;
		keyvalue11_vault8 = keyvalue11_vault7;
		buffer7[11][i] = keyvalue11_vault7;
		keyvalue12_vault8 = keyvalue12_vault7;
		buffer7[12][i] = keyvalue12_vault7;
		keyvalue13_vault8 = keyvalue13_vault7;
		buffer7[13][i] = keyvalue13_vault7;
		keyvalue14_vault8 = keyvalue14_vault7;
		buffer7[14][i] = keyvalue14_vault7;
		keyvalue15_vault8 = keyvalue15_vault7;
		buffer7[15][i] = keyvalue15_vault7;
	
		keyvalue0_vault9 = keyvalue0_vault8;
		buffer8[0][i] = keyvalue0_vault8;
		keyvalue1_vault9 = keyvalue1_vault8;
		buffer8[1][i] = keyvalue1_vault8;
		keyvalue2_vault9 = keyvalue2_vault8;
		buffer8[2][i] = keyvalue2_vault8;
		keyvalue3_vault9 = keyvalue3_vault8;
		buffer8[3][i] = keyvalue3_vault8;
		keyvalue4_vault9 = keyvalue4_vault8;
		buffer8[4][i] = keyvalue4_vault8;
		keyvalue5_vault9 = keyvalue5_vault8;
		buffer8[5][i] = keyvalue5_vault8;
		keyvalue6_vault9 = keyvalue6_vault8;
		buffer8[6][i] = keyvalue6_vault8;
		keyvalue7_vault9 = keyvalue7_vault8;
		buffer8[7][i] = keyvalue7_vault8;
		keyvalue8_vault9 = keyvalue8_vault8;
		buffer8[8][i] = keyvalue8_vault8;
		keyvalue9_vault9 = keyvalue9_vault8;
		buffer8[9][i] = keyvalue9_vault8;
		keyvalue10_vault9 = keyvalue10_vault8;
		buffer8[10][i] = keyvalue10_vault8;
		keyvalue11_vault9 = keyvalue11_vault8;
		buffer8[11][i] = keyvalue11_vault8;
		keyvalue12_vault9 = keyvalue12_vault8;
		buffer8[12][i] = keyvalue12_vault8;
		keyvalue13_vault9 = keyvalue13_vault8;
		buffer8[13][i] = keyvalue13_vault8;
		keyvalue14_vault9 = keyvalue14_vault8;
		buffer8[14][i] = keyvalue14_vault8;
		keyvalue15_vault9 = keyvalue15_vault8;
		buffer8[15][i] = keyvalue15_vault8;
	
		keyvalue0_vault10 = keyvalue0_vault9;
		buffer9[0][i] = keyvalue0_vault9;
		keyvalue1_vault10 = keyvalue1_vault9;
		buffer9[1][i] = keyvalue1_vault9;
		keyvalue2_vault10 = keyvalue2_vault9;
		buffer9[2][i] = keyvalue2_vault9;
		keyvalue3_vault10 = keyvalue3_vault9;
		buffer9[3][i] = keyvalue3_vault9;
		keyvalue4_vault10 = keyvalue4_vault9;
		buffer9[4][i] = keyvalue4_vault9;
		keyvalue5_vault10 = keyvalue5_vault9;
		buffer9[5][i] = keyvalue5_vault9;
		keyvalue6_vault10 = keyvalue6_vault9;
		buffer9[6][i] = keyvalue6_vault9;
		keyvalue7_vault10 = keyvalue7_vault9;
		buffer9[7][i] = keyvalue7_vault9;
		keyvalue8_vault10 = keyvalue8_vault9;
		buffer9[8][i] = keyvalue8_vault9;
		keyvalue9_vault10 = keyvalue9_vault9;
		buffer9[9][i] = keyvalue9_vault9;
		keyvalue10_vault10 = keyvalue10_vault9;
		buffer9[10][i] = keyvalue10_vault9;
		keyvalue11_vault10 = keyvalue11_vault9;
		buffer9[11][i] = keyvalue11_vault9;
		keyvalue12_vault10 = keyvalue12_vault9;
		buffer9[12][i] = keyvalue12_vault9;
		keyvalue13_vault10 = keyvalue13_vault9;
		buffer9[13][i] = keyvalue13_vault9;
		keyvalue14_vault10 = keyvalue14_vault9;
		buffer9[14][i] = keyvalue14_vault9;
		keyvalue15_vault10 = keyvalue15_vault9;
		buffer9[15][i] = keyvalue15_vault9;
	
		keyvalue0_vault11 = keyvalue0_vault10;
		buffer10[0][i] = keyvalue0_vault10;
		keyvalue1_vault11 = keyvalue1_vault10;
		buffer10[1][i] = keyvalue1_vault10;
		keyvalue2_vault11 = keyvalue2_vault10;
		buffer10[2][i] = keyvalue2_vault10;
		keyvalue3_vault11 = keyvalue3_vault10;
		buffer10[3][i] = keyvalue3_vault10;
		keyvalue4_vault11 = keyvalue4_vault10;
		buffer10[4][i] = keyvalue4_vault10;
		keyvalue5_vault11 = keyvalue5_vault10;
		buffer10[5][i] = keyvalue5_vault10;
		keyvalue6_vault11 = keyvalue6_vault10;
		buffer10[6][i] = keyvalue6_vault10;
		keyvalue7_vault11 = keyvalue7_vault10;
		buffer10[7][i] = keyvalue7_vault10;
		keyvalue8_vault11 = keyvalue8_vault10;
		buffer10[8][i] = keyvalue8_vault10;
		keyvalue9_vault11 = keyvalue9_vault10;
		buffer10[9][i] = keyvalue9_vault10;
		keyvalue10_vault11 = keyvalue10_vault10;
		buffer10[10][i] = keyvalue10_vault10;
		keyvalue11_vault11 = keyvalue11_vault10;
		buffer10[11][i] = keyvalue11_vault10;
		keyvalue12_vault11 = keyvalue12_vault10;
		buffer10[12][i] = keyvalue12_vault10;
		keyvalue13_vault11 = keyvalue13_vault10;
		buffer10[13][i] = keyvalue13_vault10;
		keyvalue14_vault11 = keyvalue14_vault10;
		buffer10[14][i] = keyvalue14_vault10;
		keyvalue15_vault11 = keyvalue15_vault10;
		buffer10[15][i] = keyvalue15_vault10;
	
		keyvalue0_vault12 = keyvalue0_vault11;
		buffer11[0][i] = keyvalue0_vault11;
		keyvalue1_vault12 = keyvalue1_vault11;
		buffer11[1][i] = keyvalue1_vault11;
		keyvalue2_vault12 = keyvalue2_vault11;
		buffer11[2][i] = keyvalue2_vault11;
		keyvalue3_vault12 = keyvalue3_vault11;
		buffer11[3][i] = keyvalue3_vault11;
		keyvalue4_vault12 = keyvalue4_vault11;
		buffer11[4][i] = keyvalue4_vault11;
		keyvalue5_vault12 = keyvalue5_vault11;
		buffer11[5][i] = keyvalue5_vault11;
		keyvalue6_vault12 = keyvalue6_vault11;
		buffer11[6][i] = keyvalue6_vault11;
		keyvalue7_vault12 = keyvalue7_vault11;
		buffer11[7][i] = keyvalue7_vault11;
		keyvalue8_vault12 = keyvalue8_vault11;
		buffer11[8][i] = keyvalue8_vault11;
		keyvalue9_vault12 = keyvalue9_vault11;
		buffer11[9][i] = keyvalue9_vault11;
		keyvalue10_vault12 = keyvalue10_vault11;
		buffer11[10][i] = keyvalue10_vault11;
		keyvalue11_vault12 = keyvalue11_vault11;
		buffer11[11][i] = keyvalue11_vault11;
		keyvalue12_vault12 = keyvalue12_vault11;
		buffer11[12][i] = keyvalue12_vault11;
		keyvalue13_vault12 = keyvalue13_vault11;
		buffer11[13][i] = keyvalue13_vault11;
		keyvalue14_vault12 = keyvalue14_vault11;
		buffer11[14][i] = keyvalue14_vault11;
		keyvalue15_vault12 = keyvalue15_vault11;
		buffer11[15][i] = keyvalue15_vault11;
	
		keyvalue0_vault13 = keyvalue0_vault12;
		buffer12[0][i] = keyvalue0_vault12;
		keyvalue1_vault13 = keyvalue1_vault12;
		buffer12[1][i] = keyvalue1_vault12;
		keyvalue2_vault13 = keyvalue2_vault12;
		buffer12[2][i] = keyvalue2_vault12;
		keyvalue3_vault13 = keyvalue3_vault12;
		buffer12[3][i] = keyvalue3_vault12;
		keyvalue4_vault13 = keyvalue4_vault12;
		buffer12[4][i] = keyvalue4_vault12;
		keyvalue5_vault13 = keyvalue5_vault12;
		buffer12[5][i] = keyvalue5_vault12;
		keyvalue6_vault13 = keyvalue6_vault12;
		buffer12[6][i] = keyvalue6_vault12;
		keyvalue7_vault13 = keyvalue7_vault12;
		buffer12[7][i] = keyvalue7_vault12;
		keyvalue8_vault13 = keyvalue8_vault12;
		buffer12[8][i] = keyvalue8_vault12;
		keyvalue9_vault13 = keyvalue9_vault12;
		buffer12[9][i] = keyvalue9_vault12;
		keyvalue10_vault13 = keyvalue10_vault12;
		buffer12[10][i] = keyvalue10_vault12;
		keyvalue11_vault13 = keyvalue11_vault12;
		buffer12[11][i] = keyvalue11_vault12;
		keyvalue12_vault13 = keyvalue12_vault12;
		buffer12[12][i] = keyvalue12_vault12;
		keyvalue13_vault13 = keyvalue13_vault12;
		buffer12[13][i] = keyvalue13_vault12;
		keyvalue14_vault13 = keyvalue14_vault12;
		buffer12[14][i] = keyvalue14_vault12;
		keyvalue15_vault13 = keyvalue15_vault12;
		buffer12[15][i] = keyvalue15_vault12;
	
		keyvalue0_vault14 = keyvalue0_vault13;
		buffer13[0][i] = keyvalue0_vault13;
		keyvalue1_vault14 = keyvalue1_vault13;
		buffer13[1][i] = keyvalue1_vault13;
		keyvalue2_vault14 = keyvalue2_vault13;
		buffer13[2][i] = keyvalue2_vault13;
		keyvalue3_vault14 = keyvalue3_vault13;
		buffer13[3][i] = keyvalue3_vault13;
		keyvalue4_vault14 = keyvalue4_vault13;
		buffer13[4][i] = keyvalue4_vault13;
		keyvalue5_vault14 = keyvalue5_vault13;
		buffer13[5][i] = keyvalue5_vault13;
		keyvalue6_vault14 = keyvalue6_vault13;
		buffer13[6][i] = keyvalue6_vault13;
		keyvalue7_vault14 = keyvalue7_vault13;
		buffer13[7][i] = keyvalue7_vault13;
		keyvalue8_vault14 = keyvalue8_vault13;
		buffer13[8][i] = keyvalue8_vault13;
		keyvalue9_vault14 = keyvalue9_vault13;
		buffer13[9][i] = keyvalue9_vault13;
		keyvalue10_vault14 = keyvalue10_vault13;
		buffer13[10][i] = keyvalue10_vault13;
		keyvalue11_vault14 = keyvalue11_vault13;
		buffer13[11][i] = keyvalue11_vault13;
		keyvalue12_vault14 = keyvalue12_vault13;
		buffer13[12][i] = keyvalue12_vault13;
		keyvalue13_vault14 = keyvalue13_vault13;
		buffer13[13][i] = keyvalue13_vault13;
		keyvalue14_vault14 = keyvalue14_vault13;
		buffer13[14][i] = keyvalue14_vault13;
		keyvalue15_vault14 = keyvalue15_vault13;
		buffer13[15][i] = keyvalue15_vault13;
	
		keyvalue0_vault15 = keyvalue0_vault14;
		buffer14[0][i] = keyvalue0_vault14;
		keyvalue1_vault15 = keyvalue1_vault14;
		buffer14[1][i] = keyvalue1_vault14;
		keyvalue2_vault15 = keyvalue2_vault14;
		buffer14[2][i] = keyvalue2_vault14;
		keyvalue3_vault15 = keyvalue3_vault14;
		buffer14[3][i] = keyvalue3_vault14;
		keyvalue4_vault15 = keyvalue4_vault14;
		buffer14[4][i] = keyvalue4_vault14;
		keyvalue5_vault15 = keyvalue5_vault14;
		buffer14[5][i] = keyvalue5_vault14;
		keyvalue6_vault15 = keyvalue6_vault14;
		buffer14[6][i] = keyvalue6_vault14;
		keyvalue7_vault15 = keyvalue7_vault14;
		buffer14[7][i] = keyvalue7_vault14;
		keyvalue8_vault15 = keyvalue8_vault14;
		buffer14[8][i] = keyvalue8_vault14;
		keyvalue9_vault15 = keyvalue9_vault14;
		buffer14[9][i] = keyvalue9_vault14;
		keyvalue10_vault15 = keyvalue10_vault14;
		buffer14[10][i] = keyvalue10_vault14;
		keyvalue11_vault15 = keyvalue11_vault14;
		buffer14[11][i] = keyvalue11_vault14;
		keyvalue12_vault15 = keyvalue12_vault14;
		buffer14[12][i] = keyvalue12_vault14;
		keyvalue13_vault15 = keyvalue13_vault14;
		buffer14[13][i] = keyvalue13_vault14;
		keyvalue14_vault15 = keyvalue14_vault14;
		buffer14[14][i] = keyvalue14_vault14;
		keyvalue15_vault15 = keyvalue15_vault14;
		buffer14[15][i] = keyvalue15_vault14;
	
		keyvalue0_vault16 = keyvalue0_vault15;
		buffer15[0][i] = keyvalue0_vault15;
		keyvalue1_vault16 = keyvalue1_vault15;
		buffer15[1][i] = keyvalue1_vault15;
		keyvalue2_vault16 = keyvalue2_vault15;
		buffer15[2][i] = keyvalue2_vault15;
		keyvalue3_vault16 = keyvalue3_vault15;
		buffer15[3][i] = keyvalue3_vault15;
		keyvalue4_vault16 = keyvalue4_vault15;
		buffer15[4][i] = keyvalue4_vault15;
		keyvalue5_vault16 = keyvalue5_vault15;
		buffer15[5][i] = keyvalue5_vault15;
		keyvalue6_vault16 = keyvalue6_vault15;
		buffer15[6][i] = keyvalue6_vault15;
		keyvalue7_vault16 = keyvalue7_vault15;
		buffer15[7][i] = keyvalue7_vault15;
		keyvalue8_vault16 = keyvalue8_vault15;
		buffer15[8][i] = keyvalue8_vault15;
		keyvalue9_vault16 = keyvalue9_vault15;
		buffer15[9][i] = keyvalue9_vault15;
		keyvalue10_vault16 = keyvalue10_vault15;
		buffer15[10][i] = keyvalue10_vault15;
		keyvalue11_vault16 = keyvalue11_vault15;
		buffer15[11][i] = keyvalue11_vault15;
		keyvalue12_vault16 = keyvalue12_vault15;
		buffer15[12][i] = keyvalue12_vault15;
		keyvalue13_vault16 = keyvalue13_vault15;
		buffer15[13][i] = keyvalue13_vault15;
		keyvalue14_vault16 = keyvalue14_vault15;
		buffer15[14][i] = keyvalue14_vault15;
		keyvalue15_vault16 = keyvalue15_vault15;
		buffer15[15][i] = keyvalue15_vault15;
	
		keyvalue0_vault17 = keyvalue0_vault16;
		buffer16[0][i] = keyvalue0_vault16;
		keyvalue1_vault17 = keyvalue1_vault16;
		buffer16[1][i] = keyvalue1_vault16;
		keyvalue2_vault17 = keyvalue2_vault16;
		buffer16[2][i] = keyvalue2_vault16;
		keyvalue3_vault17 = keyvalue3_vault16;
		buffer16[3][i] = keyvalue3_vault16;
		keyvalue4_vault17 = keyvalue4_vault16;
		buffer16[4][i] = keyvalue4_vault16;
		keyvalue5_vault17 = keyvalue5_vault16;
		buffer16[5][i] = keyvalue5_vault16;
		keyvalue6_vault17 = keyvalue6_vault16;
		buffer16[6][i] = keyvalue6_vault16;
		keyvalue7_vault17 = keyvalue7_vault16;
		buffer16[7][i] = keyvalue7_vault16;
		keyvalue8_vault17 = keyvalue8_vault16;
		buffer16[8][i] = keyvalue8_vault16;
		keyvalue9_vault17 = keyvalue9_vault16;
		buffer16[9][i] = keyvalue9_vault16;
		keyvalue10_vault17 = keyvalue10_vault16;
		buffer16[10][i] = keyvalue10_vault16;
		keyvalue11_vault17 = keyvalue11_vault16;
		buffer16[11][i] = keyvalue11_vault16;
		keyvalue12_vault17 = keyvalue12_vault16;
		buffer16[12][i] = keyvalue12_vault16;
		keyvalue13_vault17 = keyvalue13_vault16;
		buffer16[13][i] = keyvalue13_vault16;
		keyvalue14_vault17 = keyvalue14_vault16;
		buffer16[14][i] = keyvalue14_vault16;
		keyvalue15_vault17 = keyvalue15_vault16;
		buffer16[15][i] = keyvalue15_vault16;
	
		
		tail[0][i] = keyvalue0_vault17;
		tail[1][i] = keyvalue1_vault17;
		tail[2][i] = keyvalue2_vault17;
		tail[3][i] = keyvalue3_vault17;
		tail[4][i] = keyvalue4_vault17;
		tail[5][i] = keyvalue5_vault17;
		tail[6][i] = keyvalue6_vault17;
		tail[7][i] = keyvalue7_vault17;
		tail[8][i] = keyvalue8_vault17;
		tail[9][i] = keyvalue9_vault17;
		tail[10][i] = keyvalue10_vault17;
		tail[11][i] = keyvalue11_vault17;
		tail[12][i] = keyvalue12_vault17;
		tail[13][i] = keyvalue13_vault17;
		tail[14][i] = keyvalue14_vault17;
		tail[15][i] = keyvalue15_vault17;
	
		#ifdef _DEBUGMODE_KERNELPRINTS
		if(tail[0][i].key < 0xFFFFFFFF){ cout<<"actvvid: "<<(0*REDUCESZ + 2*i)<<endl; } 
		if(tail[0][i].value < 0xFFFFFFFF){ cout<<"actvvid: "<<(0*REDUCESZ + 2*i + 1)<<endl; } 
		if(tail[1][i].key < 0xFFFFFFFF){ cout<<"actvvid: "<<(1*REDUCESZ + 2*i)<<endl; } 
		if(tail[1][i].value < 0xFFFFFFFF){ cout<<"actvvid: "<<(1*REDUCESZ + 2*i + 1)<<endl; } 
		if(tail[2][i].key < 0xFFFFFFFF){ cout<<"actvvid: "<<(2*REDUCESZ + 2*i)<<endl; } 
		if(tail[2][i].value < 0xFFFFFFFF){ cout<<"actvvid: "<<(2*REDUCESZ + 2*i + 1)<<endl; } 
		if(tail[3][i].key < 0xFFFFFFFF){ cout<<"actvvid: "<<(3*REDUCESZ + 2*i)<<endl; } 
		if(tail[3][i].value < 0xFFFFFFFF){ cout<<"actvvid: "<<(3*REDUCESZ + 2*i + 1)<<endl; } 
		if(tail[4][i].key < 0xFFFFFFFF){ cout<<"actvvid: "<<(4*REDUCESZ + 2*i)<<endl; } 
		if(tail[4][i].value < 0xFFFFFFFF){ cout<<"actvvid: "<<(4*REDUCESZ + 2*i + 1)<<endl; } 
		if(tail[5][i].key < 0xFFFFFFFF){ cout<<"actvvid: "<<(5*REDUCESZ + 2*i)<<endl; } 
		if(tail[5][i].value < 0xFFFFFFFF){ cout<<"actvvid: "<<(5*REDUCESZ + 2*i + 1)<<endl; } 
		if(tail[6][i].key < 0xFFFFFFFF){ cout<<"actvvid: "<<(6*REDUCESZ + 2*i)<<endl; } 
		if(tail[6][i].value < 0xFFFFFFFF){ cout<<"actvvid: "<<(6*REDUCESZ + 2*i + 1)<<endl; } 
		if(tail[7][i].key < 0xFFFFFFFF){ cout<<"actvvid: "<<(7*REDUCESZ + 2*i)<<endl; } 
		if(tail[7][i].value < 0xFFFFFFFF){ cout<<"actvvid: "<<(7*REDUCESZ + 2*i + 1)<<endl; } 
		if(tail[8][i].key < 0xFFFFFFFF){ cout<<"actvvid: "<<(8*REDUCESZ + 2*i)<<endl; } 
		if(tail[8][i].value < 0xFFFFFFFF){ cout<<"actvvid: "<<(8*REDUCESZ + 2*i + 1)<<endl; } 
		if(tail[9][i].key < 0xFFFFFFFF){ cout<<"actvvid: "<<(9*REDUCESZ + 2*i)<<endl; } 
		if(tail[9][i].value < 0xFFFFFFFF){ cout<<"actvvid: "<<(9*REDUCESZ + 2*i + 1)<<endl; } 
		if(tail[10][i].key < 0xFFFFFFFF){ cout<<"actvvid: "<<(10*REDUCESZ + 2*i)<<endl; } 
		if(tail[10][i].value < 0xFFFFFFFF){ cout<<"actvvid: "<<(10*REDUCESZ + 2*i + 1)<<endl; } 
		if(tail[11][i].key < 0xFFFFFFFF){ cout<<"actvvid: "<<(11*REDUCESZ + 2*i)<<endl; } 
		if(tail[11][i].value < 0xFFFFFFFF){ cout<<"actvvid: "<<(11*REDUCESZ + 2*i + 1)<<endl; } 
		if(tail[12][i].key < 0xFFFFFFFF){ cout<<"actvvid: "<<(12*REDUCESZ + 2*i)<<endl; } 
		if(tail[12][i].value < 0xFFFFFFFF){ cout<<"actvvid: "<<(12*REDUCESZ + 2*i + 1)<<endl; } 
		if(tail[13][i].key < 0xFFFFFFFF){ cout<<"actvvid: "<<(13*REDUCESZ + 2*i)<<endl; } 
		if(tail[13][i].value < 0xFFFFFFFF){ cout<<"actvvid: "<<(13*REDUCESZ + 2*i + 1)<<endl; } 
		if(tail[14][i].key < 0xFFFFFFFF){ cout<<"actvvid: "<<(14*REDUCESZ + 2*i)<<endl; } 
		if(tail[14][i].value < 0xFFFFFFFF){ cout<<"actvvid: "<<(14*REDUCESZ + 2*i + 1)<<endl; } 
		if(tail[15][i].key < 0xFFFFFFFF){ cout<<"actvvid: "<<(15*REDUCESZ + 2*i)<<endl; } 
		if(tail[15][i].value < 0xFFFFFFFF){ cout<<"actvvid: "<<(15*REDUCESZ + 2*i + 1)<<endl; } 
	
		#endif 
	}
	return;
}

void
	#ifdef SW 
	acts::
	#endif
spreadvmask(bool_type enable, uintNUMPby2_type vmask[BLOCKRAM_SIZE], uintNUMPby2_type vmask0[BLOCKRAM_SIZE],uintNUMPby2_type vmask1[BLOCKRAM_SIZE],uintNUMPby2_type vmask2[BLOCKRAM_SIZE],uintNUMPby2_type vmask3[BLOCKRAM_SIZE],uintNUMPby2_type vmask4[BLOCKRAM_SIZE],uintNUMPby2_type vmask5[BLOCKRAM_SIZE],uintNUMPby2_type vmask6[BLOCKRAM_SIZE],uintNUMPby2_type vmask7[BLOCKRAM_SIZE],uintNUMPby2_type vmask8[BLOCKRAM_SIZE],uintNUMPby2_type vmask9[BLOCKRAM_SIZE],uintNUMPby2_type vmask10[BLOCKRAM_SIZE],uintNUMPby2_type vmask11[BLOCKRAM_SIZE],uintNUMPby2_type vmask12[BLOCKRAM_SIZE],uintNUMPby2_type vmask13[BLOCKRAM_SIZE],uintNUMPby2_type vmask14[BLOCKRAM_SIZE],uintNUMPby2_type vmask15[BLOCKRAM_SIZE], globalparams_t globalparams){
	if(enable == OFF){ return; }
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
	cout<<"spreadvmask: spread (vmask) function called."<<endl;
	#endif 
	analysis_type analysis_loopcount = BLOCKRAM_SIZE;
	
	uintNUMPby2_type vmask_vault0;
	uintNUMPby2_type vmask_vault1;
	uintNUMPby2_type vmask_vault2;
	uintNUMPby2_type vmask_vault3;
	uintNUMPby2_type vmask_vault4;
	uintNUMPby2_type vmask_vault5;
	uintNUMPby2_type vmask_vault6;
	uintNUMPby2_type vmask_vault7;
	uintNUMPby2_type vmask_vault8;
	uintNUMPby2_type vmask_vault9;
	uintNUMPby2_type vmask_vault10;
	uintNUMPby2_type vmask_vault11;
	uintNUMPby2_type vmask_vault12;
	uintNUMPby2_type vmask_vault13;
	uintNUMPby2_type vmask_vault14;
	uintNUMPby2_type vmask_vault15;
	uintNUMPby2_type vmask_vault16;
	uintNUMPby2_type vmask_vault17;
	
	for (buffer_type i=0; i<BLOCKRAM_SIZE; i++){ // 8, 16, BLOCKRAM_SIZE
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE II=1
		vmask_vault1 = vmask[i]; 
			
	
		vmask_vault2 = vmask_vault1;
		vmask0[i] = vmask_vault1;
	
		vmask_vault3 = vmask_vault2;
		vmask1[i] = vmask_vault2;
	
		vmask_vault4 = vmask_vault3;
		vmask2[i] = vmask_vault3;
	
		vmask_vault5 = vmask_vault4;
		vmask3[i] = vmask_vault4;
	
		vmask_vault6 = vmask_vault5;
		vmask4[i] = vmask_vault5;
	
		vmask_vault7 = vmask_vault6;
		vmask5[i] = vmask_vault6;
	
		vmask_vault8 = vmask_vault7;
		vmask6[i] = vmask_vault7;
	
		vmask_vault9 = vmask_vault8;
		vmask7[i] = vmask_vault8;
	
		vmask_vault10 = vmask_vault9;
		vmask8[i] = vmask_vault9;
	
		vmask_vault11 = vmask_vault10;
		vmask9[i] = vmask_vault10;
	
		vmask_vault12 = vmask_vault11;
		vmask10[i] = vmask_vault11;
	
		vmask_vault13 = vmask_vault12;
		vmask11[i] = vmask_vault12;
	
		vmask_vault14 = vmask_vault13;
		vmask12[i] = vmask_vault13;
	
		vmask_vault15 = vmask_vault14;
		vmask13[i] = vmask_vault14;
	
		vmask_vault16 = vmask_vault15;
		vmask14[i] = vmask_vault15;
	
		vmask_vault17 = vmask_vault16;
		vmask15[i] = vmask_vault16;
	
	}
	return;
}

// acts 
void 
	#ifdef SW 
	acts::
	#endif 
resetenvbuffers(keyvalue_t capsule_so1[VECTOR_SIZE][NUM_PARTITIONS], keyvalue_t capsule_so8[NUM_PARTITIONS]){
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
		capsule_so8[p].key = 0;
		capsule_so8[p].value = 0;
	}
	return;
}

bool_type 
	#ifdef SW 
	acts::
	#endif 
fetchkeyvalues(bool_type enable, unsigned int mode, uint512_dt * kvdram, keyvalue16_type vbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask[BLOCKRAM_SIZE], keyvalue_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], 
		batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, sweepparams_t sweepparams, globalparams_t globalparams){
	bool_type validfetch = ON;
	if(mode == PROCESSMODE){
		validfetch = readandprocess(enable, kvdram, vbuffer, vmask, buffer, goffset_kvs, loffset_kvs, size_kvs, travstate, sweepparams, globalparams);
	} else {
		readkeyvalues(enable, kvdram, buffer, goffset_kvs + loffset_kvs, size_kvs, travstate, globalparams); 
	}
	return validfetch;
}

void 
	#ifdef SW 
	acts::
	#endif 
commitkeyvalues(bool_type enable1, bool_type enable2, unsigned int mode, uint512_dt * kvdram, keyvalue16_type vbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t globalcapsule[NUM_PARTITIONS], keyvalue_t localcapsule[NUM_PARTITIONS], 
		batch_type destbaseaddr_kvs, sweepparams_t sweepparams, globalparams_t globalparams){
	if(mode == REDUCEMODE){
		reduceandbuffer(enable1, buffer, localcapsule, vbuffer, sweepparams, globalparams);
	} else {
		savekeyvalues(enable1, kvdram, buffer, globalcapsule, localcapsule, destbaseaddr_kvs, globalparams); 
	}
	return;
}

void 
	#ifdef SW 
	acts::
	#endif
actspipeline(bool_type enable1, bool_type enable2, keyvalue_t buffer_setof1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t capsule_so1[VECTOR_SIZE][NUM_PARTITIONS], 
						keyvalue_t buffer_setof8[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t capsule_so8[NUM_PARTITIONS],
							unsigned int currentLOP, sweepparams_t sweepparams, buffer_type cutoff, buffer_type cutoffs[VECTOR_SIZE], batch_type shiftcount, globalparams_t globalparams){		
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE;
	if(enable1 == OFF){ return; }
	
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

	buffer_type tempcutoffs[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=tempcutoffs complete

	for(partition_type p=0; p<NUM_PARTITIONS; p++){
		capsule_so8[p].key = 0;
		capsule_so8[p].value = capsule_so1[0][p].value + capsule_so1[1][p].value 
									+ capsule_so1[2][p].value + capsule_so1[3][p].value 
									+ capsule_so1[4][p].value + capsule_so1[5][p].value 
									+ capsule_so1[6][p].value + capsule_so1[7][p].value;
	}
	calculateoffsets(capsule_so8, NUM_PARTITIONS);
	resetvalues(capsule_so8, NUM_PARTITIONS, 0);
	
	keyvalue_t dummykv;
	dummykv.key = 0;
	dummykv.value = INVALIDDATA;
	
	value_t tempbufferDcapsule[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=tempbufferDcapsule complete

	for(partition_type p=0; p<NUM_PARTITIONS; p++){ 
	#pragma HLS PIPELINE II=1
		tempbufferDcapsule[p] = capsule_so8[p].value; 
	}
	for(vector_type v=0; v<VECTOR_SIZE; v++){ 
	#pragma HLS PIPELINE II=1
		tempcutoffs[v] = cutoffs[v]; 
	}
	unsigned int n=0;
	RUNPIPELINE_LOOP1: for(n=0; n<2; n++){
		RUNPIPELINE_LOOP1B: for(buffer_type k=0; k<SRCBUFFER_SIZE; k+=4){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
		#pragma HLS PIPELINE II=4 // CRITICAL REMOVEME.
		
			#ifdef _DEBUGMODE_KERNELPRINTS
			if(k % 16 == 0){ cout<<"k: "<<k<<endl; }
			#endif
			
			/// LOADING FROM A
			if(k < tempcutoffs[0+n]){ kvA0[0] = buffer_setof1[0+n][k]; kvA0[1] = buffer_setof1[0+n][k+1]; kvA0[2] = buffer_setof1[0+n][k+2]; kvA0[3] = buffer_setof1[0+n][k+3]; } 
			else { kvA0[0] = dummykv; kvA0[1] = dummykv; kvA0[2] = dummykv; kvA0[3] = dummykv; }
			if(k < tempcutoffs[2+n]){ kvA2[0] = buffer_setof1[2+n][k]; kvA2[1] = buffer_setof1[2+n][k+1]; kvA2[2] = buffer_setof1[2+n][k+2]; kvA2[3] = buffer_setof1[2+n][k+3]; } 
			else { kvA2[0] = dummykv; kvA2[1] = dummykv; kvA2[2] = dummykv; kvA2[3] = dummykv; }
			if(k < tempcutoffs[4+n]){ kvA4[0] = buffer_setof1[4+n][k]; kvA4[1] = buffer_setof1[4+n][k+1]; kvA4[2] = buffer_setof1[4+n][k+2]; kvA4[3] = buffer_setof1[4+n][k+3]; } 
			else { kvA4[0] = dummykv; kvA4[1] = dummykv; kvA4[2] = dummykv; kvA4[3] = dummykv; }
			if(k < tempcutoffs[6+n]){ kvA6[0] = buffer_setof1[6+n][k]; kvA6[1] = buffer_setof1[6+n][k+1]; kvA6[2] = buffer_setof1[6+n][k+2]; kvA6[3] = buffer_setof1[6+n][k+3]; } 
			else { kvA6[0] = dummykv; kvA6[1] = dummykv; kvA6[2] = dummykv; kvA6[3] = dummykv; }
		
			#ifdef _DEBUGMODE_KERNELPRINTS
			for(unsigned int m=0; m<4; m++){ cout<<"kvA0["<<m<<"].key: "<<kvA0[m].key<<", kvA0["<<m<<"].value: "<<kvA0[m].value<<endl; }
			for(unsigned int m=0; m<4; m++){ cout<<"kvA2["<<m<<"].key: "<<kvA2[m].key<<", kvA2["<<m<<"].value: "<<kvA2[m].value<<endl; }
			for(unsigned int m=0; m<4; m++){ cout<<"kvA4["<<m<<"].key: "<<kvA4[m].key<<", kvA4["<<m<<"].value: "<<kvA4[m].value<<endl; }
			for(unsigned int m=0; m<4; m++){ cout<<"kvA6["<<m<<"].key: "<<kvA6[m].key<<", kvA6["<<m<<"].value: "<<kvA6[m].value<<endl; }
			#endif
			
			#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
			actsutilityobj->checkn(enablebufferA, "kvA0", kvA0, currentLOP, upperlimit, globalparams.POW_BATCHRANGE, 4);
			actsutilityobj->checkn(enablebufferA, "kvA2", kvA2, currentLOP, upperlimit, globalparams.POW_BATCHRANGE, 4);
			actsutilityobj->checkn(enablebufferA, "kvA4", kvA4, currentLOP, upperlimit, globalparams.POW_BATCHRANGE, 4);
			actsutilityobj->checkn(enablebufferA, "kvA6", kvA6, currentLOP, upperlimit, globalparams.POW_BATCHRANGE, 4);
			#endif
			
			partition_type pA0 = getpartition(ON, kvA0[0], currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE);
			partition_type pA2 = getpartition(ON, kvA2[0], currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE);
			partition_type pA4 = getpartition(ON, kvA4[0], currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE);
			partition_type pA6 = getpartition(ON, kvA6[0], currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE);
			
			/// LOADING FROM AND INTO D
			buffer_type posD0 = capsule_so8[pA0].key + tempbufferDcapsule[pA0];
			unsigned int yoffset0 = posD0 / 8;
			unsigned int xoffset0 = posD0 % 8;
			#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
			actsutilityobj->checkfordivisibleby(enablebufferD, "posD0", posD0, 4);
			actsutilityobj->checkoutofbounds(enablebufferD, "posD0", posD0, BLOCKRAM_SIZE * VECTOR_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferD, "yoffset0", yoffset0, BLOCKRAM_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferD, "xoffset0+4", xoffset0+4, VECTOR_SIZE+1, NAp, NAp, NAp);
			#endif 
			if(xoffset0 == 0){
				buffer_setof8[0][yoffset0] = kvA0[0]; buffer_setof8[1][yoffset0] = kvA0[1]; buffer_setof8[2][yoffset0] = kvA0[2]; buffer_setof8[3][yoffset0] = kvA0[3]; 
			} else {
				buffer_setof8[4][yoffset0] = kvA0[0]; buffer_setof8[5][yoffset0] = kvA0[1]; buffer_setof8[6][yoffset0] = kvA0[2]; buffer_setof8[7][yoffset0] = kvA0[3]; 
			}
			if(!((kvA0[0].value == INVALIDDATA) && (kvA0[1].value == INVALIDDATA) && (kvA0[2].value == INVALIDDATA) && (kvA0[3].value == INVALIDDATA))){ tempbufferDcapsule[pA0] += 4; }
			
			buffer_type _posD0 = capsule_so8[pA2].key + tempbufferDcapsule[pA2];
			unsigned int yoffset1 = _posD0 / 8;
			unsigned int xoffset1 = _posD0 % 8;
			#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
			actsutilityobj->checkfordivisibleby(enablebufferD, "_posD0", _posD0, 4);
			actsutilityobj->checkoutofbounds(enablebufferD, "_posD0", _posD0, BLOCKRAM_SIZE * VECTOR_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferD, "yoffset1", yoffset1, BLOCKRAM_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferD, "xoffset1+4", xoffset1+4, VECTOR_SIZE+1, NAp, NAp, NAp);
			#endif 
			if(xoffset1 == 0){
				buffer_setof8[0][yoffset1] = kvA2[0]; buffer_setof8[1][yoffset1] = kvA2[1]; buffer_setof8[2][yoffset1] = kvA2[2]; buffer_setof8[3][yoffset1] = kvA2[3]; 
			} else {
				buffer_setof8[4][yoffset1] = kvA2[0]; buffer_setof8[5][yoffset1] = kvA2[1]; buffer_setof8[6][yoffset1] = kvA2[2]; buffer_setof8[7][yoffset1] = kvA2[3]; 
			}
			if(!((kvA2[0].value == INVALIDDATA) && (kvA2[1].value == INVALIDDATA) && (kvA2[2].value == INVALIDDATA) && (kvA2[3].value == INVALIDDATA))){ tempbufferDcapsule[pA2] += 4; }
			
			buffer_type __posD0 = capsule_so8[pA4].key + tempbufferDcapsule[pA4];
			unsigned int yoffset2 = __posD0 / 8;
			unsigned int xoffset2 = __posD0 % 8;
			#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
			actsutilityobj->checkfordivisibleby(enablebufferD, "__posD0", __posD0, 4);
			actsutilityobj->checkoutofbounds(enablebufferD, "__posD0", __posD0, BLOCKRAM_SIZE * VECTOR_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferD, "yoffset2", yoffset2, BLOCKRAM_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferD, "xoffset2+4", xoffset2+4, VECTOR_SIZE+1, NAp, NAp, NAp);
			#endif 
			if(xoffset2 == 0){
				buffer_setof8[0][yoffset2] = kvA4[0]; buffer_setof8[1][yoffset2] = kvA4[1]; buffer_setof8[2][yoffset2] = kvA4[2]; buffer_setof8[3][yoffset2] = kvA4[3]; 
			} else {
				buffer_setof8[4][yoffset2] = kvA4[0]; buffer_setof8[5][yoffset2] = kvA4[1]; buffer_setof8[6][yoffset2] = kvA4[2]; buffer_setof8[7][yoffset2] = kvA4[3]; 
			}
			if(!((kvA4[0].value == INVALIDDATA) && (kvA4[1].value == INVALIDDATA) && (kvA4[2].value == INVALIDDATA) && (kvA4[3].value == INVALIDDATA))){ tempbufferDcapsule[pA4] += 4; }
			
			buffer_type ___posD0 = capsule_so8[pA6].key + tempbufferDcapsule[pA6];
			unsigned int yoffset3 = ___posD0 / 8;
			unsigned int xoffset3 = ___posD0 % 8;
			#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
			actsutilityobj->checkfordivisibleby(enablebufferD, "___posD0", ___posD0, 4);
			actsutilityobj->checkoutofbounds(enablebufferD, "___posD0", ___posD0, BLOCKRAM_SIZE * VECTOR_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferD, "yoffset3", yoffset3, BLOCKRAM_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferD, "xoffset3+4", xoffset3+4, VECTOR_SIZE+1, NAp, NAp, NAp);
			#endif 
			if(xoffset3 == 0){
				buffer_setof8[0][yoffset3] = kvA6[0]; buffer_setof8[1][yoffset3] = kvA6[1]; buffer_setof8[2][yoffset3] = kvA6[2]; buffer_setof8[3][yoffset3] = kvA6[3]; 
			} else {
				buffer_setof8[4][yoffset3] = kvA6[0]; buffer_setof8[5][yoffset3] = kvA6[1]; buffer_setof8[6][yoffset3] = kvA6[2]; buffer_setof8[7][yoffset3] = kvA6[3]; 
			}
			if(!((kvA6[0].value == INVALIDDATA) && (kvA6[1].value == INVALIDDATA) && (kvA6[2].value == INVALIDDATA) && (kvA6[3].value == INVALIDDATA))){ tempbufferDcapsule[pA6] += 4; }
		}
	}
	for(partition_type p=0; p<NUM_PARTITIONS; p++){ 
	#pragma HLS PIPELINE II=1
		capsule_so8[p].value = tempbufferDcapsule[p]; 
	}
	for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		keyvalue_t dummykv;
		dummykv.key = p;
		dummykv.value = INVALIDDATA;
		if(capsule_so8[p].value % 8 == 4){
			unsigned int yoffset = (capsule_so8[p].key + capsule_so8[p].value) / VECTOR_SIZE;
			buffer_setof8[4][yoffset] = dummykv; buffer_setof8[5][yoffset] = dummykv; buffer_setof8[6][yoffset] = dummykv; buffer_setof8[7][yoffset] = dummykv; 
			capsule_so8[p].value += 4;
		}
		#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
		actsutilityobj->checkfordivisibleby(enablebufferD, "capsule_so8[p].value", capsule_so8[p].value, 8);
		#endif
	}
	return;
}

void 
	#ifdef SW 
	acts::
	#endif
actit(bool_type enable, unsigned int mode,
		uint512_dt * kvdram, keyvalue_t sourcebuffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue16_type vbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask[BLOCKRAM_SIZE], keyvalue_t globalstatsbuffer[NUM_PARTITIONS], 
		globalparams_t globalparams, sweepparams_t sweepparams, travstate_t ptravstate, batch_type sourcebaseaddr_kvs, batch_type destbaseaddr_kvs,
		bool_type resetenv, bool_type flush){
	analysis_type analysis_partitionloop = KVDATA_BATCHSIZE_KVS / (NUMPARTITIONUPDATESPIPELINES * WORKBUFFER_SIZE);
	if(enable == OFF){ return; }
	
static keyvalue_t buffer_setof1[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = buffer_setof1
static keyvalue_t buffer_setof8[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = buffer_setof8
	
static keyvalue_t capsule_so1[VECTOR_SIZE][NUM_PARTITIONS];
	#pragma HLS array_partition variable = capsule_so1
static keyvalue_t capsule_so8[NUM_PARTITIONS];
	
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
static buffer_type pp0cutoffs[VECTOR_SIZE];
static buffer_type pp1cutoffs[VECTOR_SIZE];
	batch_type itercount = 0;
	batch_type flushsz = 0;
	
	if(resetenv == ON){ resetenvbuffers(capsule_so1, capsule_so8); } else { itercount = 64; } // FIXME. '64' is just some number greater than 2
	if(flush == ON){ flushsz = 1*SRCBUFFER_SIZE; } else { flushsz = 0; }
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(resetenv == ON){ cout<<"actit: reset is ON"<<endl; } else { cout<<"actit: reset is OFF"<<endl;  }
	if(flush == ON){ cout<<"actit: flush is ON"<<endl; } else { cout<<"actit: flush is OFF"<<endl;  }
	#endif 
	
	ACTIT_MAINLOOP: for(batch_type offset_kvs=ptravstate.begin_kvs; offset_kvs<ptravstate.end_kvs + flushsz; offset_kvs+=WORKBUFFER_SIZE * NUMPARTITIONUPDATESPIPELINES){
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
		if(itercount >= 0){ pp0writeen = ON; } else { pp0writeen = OFF; }
		if(itercount >= 1){ pp1writeen = ON; } else { pp1writeen = OFF; }
		if(itercount >= 1){ pp1runpipelineen = ON; } else { pp1runpipelineen = OFF; }
		#else 
		if(itercount >= 0){ pp0writeen = ON; } else { pp0writeen = OFF; }
		#endif
		
		fetchkeyvalues(ON, mode, kvdram, vbuffer, vmask, sourcebuffer, sourcebaseaddr_kvs, offset_kvs, WORKBUFFER_SIZE, ptravstatepp0, sweepparams, globalparams);
		#ifdef PUP1
		actspipeline(pp1runpipelineen, ON, buffer_setof1, capsule_so1, buffer_setof8, capsule_so8, sweepparams.currentLOP, sweepparams, WORKBUFFER_SIZE, pp1cutoffs, (itercount-2)+1, globalparams);
		#endif 
		
		preparekeyvalues(ON, ON, sourcebuffer, buffer_setof1, capsule_so1, sweepparams.currentLOP, sweepparams, ptravstatepp0, WORKBUFFER_SIZE, pp0cutoffs, globalparams);
		#ifdef PUP1
		commitkeyvalues(pp1writeen, ON, mode, kvdram, vbuffer, buffer_setof8, globalstatsbuffer, capsule_so8, destbaseaddr_kvs, sweepparams, globalparams); 
		#endif 
		
		actspipeline(ON, ON, buffer_setof1, capsule_so1, buffer_setof8, capsule_so8, sweepparams.currentLOP, sweepparams, WORKBUFFER_SIZE, pp0cutoffs, itercount, globalparams);
		#ifdef PUP1
		fetchkeyvalues(ON, mode, kvdram, vbuffer, vmask, sourcebuffer, sourcebaseaddr_kvs, offset_kvs + WORKBUFFER_SIZE, WORKBUFFER_SIZE, ptravstatepp1, sweepparams, globalparams);
		#endif
		
		commitkeyvalues(pp0writeen, ON, mode, kvdram, vbuffer, buffer_setof8, globalstatsbuffer, capsule_so8, destbaseaddr_kvs, sweepparams, globalparams); 
		#ifdef PUP1
		preparekeyvalues(pp1partitionen, ON, sourcebuffer, buffer_setof1, capsule_so1, sweepparams.currentLOP, sweepparams, ptravstatepp1, WORKBUFFER_SIZE, pp1cutoffs, globalparams);
		#endif
		
		itercount += NUMPARTITIONUPDATESPIPELINES;
	}
	return;
}

// dispatch
void
	#ifdef SW 
	acts::
	#endif 
processit(uint512_dt * kvdram, keyvalue_t sourcebuffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue16_type vbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask[BLOCKRAM_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], globalparams_t globalparams){
	#pragma HLS INLINE 
	analysis_type analysis_loop1 = 1;
	#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(_DEBUGMODE_CHECKS2)
	actsutilityobj->clearglobalvars();
	#endif
	
	keyvalue_t globalstatsbuffer[NUM_PARTITIONS]; 
	
	batch_type sourcestatsmarker = 0;
	batch_type deststatsmarker = 1;
	sweepparams_t sweepparams;
	
	buffer_type reducebuffersz = globalparams.SIZE_REDUCE / 2;
	buffer_type vmaskbuffersz_kvs = (globalparams.SIZE_REDUCE * NUM_PARTITIONS) / 512;
	
	step_type currentLOP=globalparams.ACTSPARAMS_BEGINLOP;
	resetkeysandvalues(globalstatsbuffer, NUM_PARTITIONS, 0);
	sweepparams = getsweepparams(globalparams, currentLOP, 0);
	travstate_t avtravstate;
	batch_type vmaskoffset_kvs = 0;
	
	step_type tempcurrentLOP = globalparams.ACTSPARAMS_TREEDEPTH;
	batch_type tempnum_source_partitions = get_num_source_partitions(tempcurrentLOP);
	
	avtravstate.begin_kvs = 0;
	avtravstate.end_kvs = avtravstate.begin_kvs + globalparams.ACTSPARAMS_SRCVSIZE / VECTOR2_SIZE; avtravstate.size_kvs = globalparams.ACTSPARAMS_SRCVSIZE / VECTOR2_SIZE;
	readglobalstats(ON, kvdram, globalstatsbuffer, globalparams.BASEOFFSETKVS_STATSDRAM + deststatsmarker, globalparams); 
	resetvalues(globalstatsbuffer, NUM_PARTITIONS, 0);
	
	batch_type vptrbaseoffset_kvs = globalparams.BASEOFFSETKVS_VERTEXPTR + (globalparams.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE);
	batch_type vdatabaseoffset_kvs = globalparams.BASEOFFSETKVS_VERTICESDATA + (globalparams.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE);
	
	vertex_t firstvptr = 0;

	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->print7("### processit:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, avtravstate.begin_kvs * VECTOR_SIZE, avtravstate.end_kvs * VECTOR_SIZE, (avtravstate.end_kvs - avtravstate.begin_kvs) * VECTOR_SIZE, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams.currentLOP)), sweepparams.currentLOP); 							
	#endif
	PROCESSIT_MAINLOOP: for(batch_type source_partition=0; source_partition<tempnum_source_partitions; source_partition+=1){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loop1 avg=analysis_loop1

		batch_type voffset_kvs = source_partition * reducebuffersz * FETFACTOR;
		if(voffset_kvs >= avtravstate.end_kvs){ continue; }
		#ifndef ALLVERTEXISACTIVE_ALGORITHM
		if(vmask_p[source_partition] == 0){ vmaskoffset_kvs += vmaskbuffersz_kvs; continue; } // NEWFIX: update before continue.
		#endif 
		
		sweepparams.source_partition = source_partition;
		avtravstate.i_kvs = voffset_kvs;
		
		#ifdef _DEBUGMODE_KERNELPRINTS2
		actsutilityobj->print4("### processit:: voffset", "vbegin", "vend", "vskip", voffset_kvs * VECTOR_SIZE, avtravstate.begin_kvs * VECTOR_SIZE, avtravstate.size_kvs * VECTOR_SIZE, SRCBUFFER_SIZE * VECTOR_SIZE);
		#endif
		
		loadvmasks(ON, kvdram, vmask, sourcebuffer, globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskoffset_kvs, vmaskbuffersz_kvs, globalparams); // NOTE: this should come before loadvdata because buffer_setof2 is used as a temp buffer
		readkeyvalues(ON, kvdram, vdatabaseoffset_kvs + voffset_kvs, vbuffer, 0, 0, reducebuffersz, globalparams);
		readkeyvalues(ON, kvdram, vdatabaseoffset_kvs + voffset_kvs + reducebuffersz, vbuffer, 8, 0, reducebuffersz, globalparams);
		vmaskoffset_kvs += vmaskbuffersz_kvs;
	
		vertex_t srcvlocaloffset = (voffset_kvs * VECTOR2_SIZE);
		vertex_t beginsrcvid = globalparams.ACTSPARAMS_SRCVOFFSET + (voffset_kvs * VECTOR2_SIZE);
		vertex_t endsrcvid = beginsrcvid + ((reducebuffersz * VECTOR2_SIZE) * FETFACTOR);
		if(srcvlocaloffset >= globalparams.ACTSPARAMS_SRCVSIZE){ endsrcvid = beginsrcvid; }
		if((srcvlocaloffset < globalparams.ACTSPARAMS_SRCVSIZE) && (srcvlocaloffset + ((reducebuffersz * VECTOR2_SIZE) * FETFACTOR) >= globalparams.ACTSPARAMS_SRCVSIZE)){ endsrcvid = beginsrcvid + globalparams.ACTSPARAMS_SRCVSIZE - srcvlocaloffset; }
		
		#ifdef _WIDEWORD
		keyy_t beginvptr = kvdram[vptrbaseoffset_kvs + voffset_kvs].range(31, 0);
		keyy_t endvptr = kvdram[vptrbaseoffset_kvs + voffset_kvs + (reducebuffersz * FETFACTOR) + 1].range(31, 0); 
		#else 
		keyy_t beginvptr = kvdram[vptrbaseoffset_kvs + voffset_kvs].data[0].key;
		keyy_t endvptr = kvdram[vptrbaseoffset_kvs + voffset_kvs + (reducebuffersz * FETFACTOR) + 1].data[0].key;
		#endif 
		if(srcvlocaloffset >= globalparams.ACTSPARAMS_SRCVSIZE){ endvptr = beginvptr; }
		
		keyy_t localbeginvptr = beginvptr - firstvptr;
		keyy_t localendvptr = endvptr - firstvptr;
		keyy_t numedges = localendvptr - localbeginvptr + 2*VECTOR_SIZE;
		if(localbeginvptr == localendvptr){ numedges = 0; }
		keyy_t localbeginvptr_kvs = localbeginvptr / VECTOR_SIZE; // NOTE: this should be in KVS(8) terms
		keyy_t numedges_kvs = numedges / VECTOR_SIZE;
		#ifdef _DEBUGMODE_CHECKS2
		if(localendvptr < localbeginvptr){ cout<<"processit::ERROR: localendvptr < localbeginvptr. localbeginvptr: "<<localbeginvptr<<", localendvptr: "<<localendvptr<<endl; exit(EXIT_FAILURE); }
		if(localendvptr < globalparams.SIZE_EDGES){ actsutilityobj->checkptr("processit", beginsrcvid, endsrcvid, localbeginvptr, localendvptr, (keyvalue_t *)&kvdram[globalparams.BASEOFFSETKVS_EDGESDATA]); }
		#endif
		
		#ifdef _DEBUGMODE_KERNELPRINTS2
		cout<<"[index: "<<source_partition<<"][beginsrcvid: "<<beginsrcvid<<", endsrcvid: "<<endsrcvid<<"][beginvptr: "<<localbeginvptr<<", endvptr: "<<localendvptr<<", edges size: "<<numedges<<"][voffset: "<<voffset_kvs * VECTOR_SIZE<<"]"<<endl;
		#endif
		#ifdef _DEBUGMODE_CHECKS2
		if(localendvptr < localbeginvptr){ cout<<"ERROR: localendvptr < localbeginvptr. EXITING..."<<endl; exit(EXIT_FAILURE); }
		actsutilityobj->clearallstats();
		#endif
		
		travstate_t etravstate;
		etravstate.begin_kvs = localbeginvptr_kvs;
		etravstate.size_kvs = numedges_kvs;
		etravstate.end_kvs = etravstate.begin_kvs + etravstate.size_kvs;
		etravstate.skip_kvs = SRCBUFFER_SIZE;
		etravstate.i_kvs = etravstate.begin_kvs;
		etravstate.i2 = voffset_kvs * VECTOR2_SIZE;
		etravstate.i2_kvs = voffset_kvs;
		bool_type resetenv = ON;
		bool_type flush = ON;
		if(voffset_kvs == avtravstate.begin_kvs){ resetenv = ON; } else { resetenv = OFF; }
		if((voffset_kvs + (reducebuffersz * FETFACTOR)) >= avtravstate.end_kvs){ flush = ON; } else { flush = OFF; }
		#ifndef ALLVERTEXISACTIVE_ALGORITHM
		resetenv = ON;
		flush = ON; // CRITICAL FIXME. flushing disabled issues with hollywood
		#endif 
		
		actit(
			ON, PROCESSMODE,
			kvdram, sourcebuffer, vbuffer, vmask, globalstatsbuffer, 
			globalparams, sweepparams, etravstate, globalparams.BASEOFFSETKVS_EDGESDATA, globalparams.BASEOFFSETKVS_KVDRAMWORKSPACE,
			resetenv, flush);

		#ifdef _DEBUGMODE_KERNELPRINTS
		actsutilityobj->printglobalvars();
		actsutilityobj->clearglobalvars();
		#endif
	}
	saveglobalstats(ON, kvdram, globalstatsbuffer, globalparams.BASEOFFSETKVS_STATSDRAM + deststatsmarker, globalparams);
	
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
partitionit(uint512_dt * kvdram, keyvalue_t sourcebuffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue16_type vbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask[BLOCKRAM_SIZE], globalparams_t globalparams){
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
	
	keyvalue_t globalstatsbuffer[NUM_PARTITIONS]; 
	
	batch_type sourcestatsmarker = 1;
	batch_type deststatsmarker = 1 + NUM_PARTITIONS;
	config_t config;
	sweepparams_t sweepparams;
	travstate_t actvvstravstate; actvvstravstate.i=0; actvvstravstate.i_kvs=0;
	
	PARTITIONIT_MAINLOOP1: for(step_type currentLOP=globalparams.ACTSPARAMS_BEGINLOP + 1; currentLOP<globalparams.ACTSPARAMS_BEGINLOP + 1 + (globalparams.ACTSPARAMS_NUMLOPS-2); currentLOP++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_numllops avg=analysis_numllops	
	
		batch_type num_source_partitions = get_num_source_partitions(currentLOP);
		bool_type enreduce = ON;
		
		PARTITIONIT_LOOP1B: for(batch_type source_partition=0; source_partition<num_source_partitions; source_partition+=1){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_numsourcepartitions avg=analysis_numsourcepartitions

			resetkeysandvalues(globalstatsbuffer, NUM_PARTITIONS, 0);
		
			sweepparams = getsweepparams(globalparams, currentLOP, source_partition);
			travstate_t ptravstate = gettravstate(ON, kvdram, globalparams, currentLOP, sourcestatsmarker);
			
			// collect stats
			readglobalstats(ON, kvdram, globalstatsbuffer, globalparams.BASEOFFSETKVS_STATSDRAM + deststatsmarker, globalparams);
			resetvalues(globalstatsbuffer, NUM_PARTITIONS, 0);
			
			// partition
			if(ptravstate.size_kvs > 0){ config.enablepartition = ON; } 
			else { ptravstate.begin_kvs = 0; ptravstate.end_kvs = 0; config.enablepartition = OFF; }
			if(ptravstate.size_kvs == 0){ ptravstate.begin_kvs = 0; ptravstate.end_kvs = 0; config.enablepartition = OFF; }
			#ifdef _DEBUGMODE_KERNELPRINTS2
			if((config.enablepartition == ON) && (currentLOP >= 1) && (currentLOP <= globalparams.ACTSPARAMS_TREEDEPTH)){ actsutilityobj->print7("### partitionit:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, ptravstate.begin_kvs * VECTOR_SIZE, ptravstate.end_kvs * VECTOR_SIZE, ptravstate.size_kvs * VECTOR_SIZE, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams.currentLOP)), sweepparams.currentLOP); }	
			#endif
			resetvalues(globalstatsbuffer, NUM_PARTITIONS, 0);
			bool_type resetenv; if(source_partition==0){ resetenv = ON; } else { resetenv = OFF; }
			actit(config.enablepartition, PARTITIONMODE,
					kvdram, sourcebuffer, vbuffer, vmask, globalstatsbuffer,
					globalparams, sweepparams, ptravstate, sweepparams.worksourcebaseaddress_kvs, sweepparams.workdestbaseaddress_kvs,
					ON, ON);
			saveglobalstats(config.enablepartition, kvdram, globalstatsbuffer, globalparams.BASEOFFSETKVS_STATSDRAM + deststatsmarker, globalparams); 
			
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
reduceit(uint512_dt * kvdram, keyvalue_t sourcebuffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue16_type vbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask[BLOCKRAM_SIZE], batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparams){	
	#pragma HLS INLINE
	analysis_type analysis_numllops = 1;
	analysis_type analysis_numsourcepartitions = 1;
	analysis_type analysis_treedepth = TREE_DEPTH;
	
	keyvalue_t globalstatsbuffer[NUM_PARTITIONS]; 
	
	config_t config;
	sweepparams_t sweepparams;
	
	step_type currentLOP = globalparams.ACTSPARAMS_TREEDEPTH;
	batch_type num_source_partitions = get_num_source_partitions(currentLOP);
	
	sweepparams = getsweepparams(globalparams, currentLOP, source_partition);
	travstate_t ptravstate = gettravstate(ON, kvdram, globalparams, currentLOP, sourcestatsmarker);

	if(ptravstate.size_kvs == 0){ ptravstate.begin_kvs = 0; ptravstate.end_kvs = 0; config.enablereduce = OFF; }
	else { config.enablereduce = ON; }
	#ifdef _DEBUGMODE_KERNELPRINTS2
	if(ptravstate.size_kvs > 0){ actsutilityobj->print7("### reduceit:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, ptravstate.begin_kvs * VECTOR_SIZE, ptravstate.end_kvs * VECTOR_SIZE, ptravstate.size_kvs * VECTOR_SIZE, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams.currentLOP)), sweepparams.currentLOP); }	
	#endif
	bool_type resetenv; if(source_partition==0){ resetenv = ON; } else { resetenv = OFF; }
	
	actit(config.enablereduce, REDUCEMODE,
			kvdram, sourcebuffer, vbuffer, vmask, globalstatsbuffer,
			globalparams, sweepparams, ptravstate, sweepparams.worksourcebaseaddress_kvs, sweepparams.workdestbaseaddress_kvs,
			ON, ON);
	return;
}

void
	#ifdef SW 
	acts::
	#endif 
dispatch(bool_type en_process, bool_type en_partition, bool_type en_reduce, uint512_dt * kvdram, keyvalue_t sourcebuffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue16_type vbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask[BLOCKRAM_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE],
			batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparams){
	if(en_process == ON){ processit(kvdram, sourcebuffer, vbuffer, vmask, vmask_p, globalparams); } 
	if(en_partition == ON){ partitionit(kvdram, sourcebuffer, vbuffer, vmask, globalparams); } 
	if(en_reduce == ON){ reduceit(kvdram, sourcebuffer, vbuffer, vmask, sourcestatsmarker, source_partition, globalparams); } 
	return;
}

// top 
#ifdef NACTS_IN_1COMPUTEUNIT
void
	#ifdef SW 
	acts::
	#endif 
dispatch_reduce(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11,uint512_dt * kvdram12,uint512_dt * kvdram13,uint512_dt * kvdram14,uint512_dt * kvdram15, 
keyvalue_t sourcebuffer0[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue16_type vbuffer0_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask0[BLOCKRAM_SIZE], uint32_type vmask_p0[BLOCKRAM_SIZE],keyvalue_t sourcebuffer1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue16_type vbuffer1_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask1[BLOCKRAM_SIZE], uint32_type vmask_p1[BLOCKRAM_SIZE],keyvalue_t sourcebuffer2[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue16_type vbuffer2_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask2[BLOCKRAM_SIZE], uint32_type vmask_p2[BLOCKRAM_SIZE],keyvalue_t sourcebuffer3[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue16_type vbuffer3_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask3[BLOCKRAM_SIZE], uint32_type vmask_p3[BLOCKRAM_SIZE],keyvalue_t sourcebuffer4[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue16_type vbuffer4_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask4[BLOCKRAM_SIZE], uint32_type vmask_p4[BLOCKRAM_SIZE],keyvalue_t sourcebuffer5[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue16_type vbuffer5_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask5[BLOCKRAM_SIZE], uint32_type vmask_p5[BLOCKRAM_SIZE],keyvalue_t sourcebuffer6[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue16_type vbuffer6_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask6[BLOCKRAM_SIZE], uint32_type vmask_p6[BLOCKRAM_SIZE],keyvalue_t sourcebuffer7[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue16_type vbuffer7_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask7[BLOCKRAM_SIZE], uint32_type vmask_p7[BLOCKRAM_SIZE],keyvalue_t sourcebuffer8[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue16_type vbuffer8_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask8[BLOCKRAM_SIZE], uint32_type vmask_p8[BLOCKRAM_SIZE],keyvalue_t sourcebuffer9[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue16_type vbuffer9_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask9[BLOCKRAM_SIZE], uint32_type vmask_p9[BLOCKRAM_SIZE],keyvalue_t sourcebuffer10[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue16_type vbuffer10_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask10[BLOCKRAM_SIZE], uint32_type vmask_p10[BLOCKRAM_SIZE],keyvalue_t sourcebuffer11[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue16_type vbuffer11_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask11[BLOCKRAM_SIZE], uint32_type vmask_p11[BLOCKRAM_SIZE],keyvalue_t sourcebuffer12[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue16_type vbuffer12_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask12[BLOCKRAM_SIZE], uint32_type vmask_p12[BLOCKRAM_SIZE],keyvalue_t sourcebuffer13[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue16_type vbuffer13_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask13[BLOCKRAM_SIZE], uint32_type vmask_p13[BLOCKRAM_SIZE],keyvalue_t sourcebuffer14[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue16_type vbuffer14_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask14[BLOCKRAM_SIZE], uint32_type vmask_p14[BLOCKRAM_SIZE],keyvalue_t sourcebuffer15[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue16_type vbuffer15_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask15[BLOCKRAM_SIZE], uint32_type vmask_p15[BLOCKRAM_SIZE], 
		uint32_type vmask_p[BLOCKRAM_SIZE], globalparams_t globalparams[NUMCOMPUTEUNITS]){
	#pragma HLS INLINE
	analysis_type analysis_loop1 = 1;
	analysis_type analysis_treedepth = TREE_DEPTH;
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	keyvalue16_type synvbuffer_head_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = synvbuffer_head_1
	
	keyvalue16_type synvbuffer_tail_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = synvbuffer_tail_1
	
	travstate_t rtravstate[NUMCOMPUTEUNITS];
	#pragma HLS ARRAY_PARTITION variable=rtravstate complete
	
	globalparams_t _globalparams = globalparams[0];
	unsigned int sourcestatsmarker = 0;
	for(unsigned int k=0; k<_globalparams.ACTSPARAMS_TREEDEPTH-1; k++){ 
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_treedepth avg=analysis_treedepth
		sourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); 
	}
	
	unsigned int vreadoffset = 0;
	unsigned int vmaskreadoffset = 0;
	buffer_type reducebuffersz = globalparams[0].SIZE_REDUCE / 2;
	buffer_type vmaskbuffersz_kvs = (globalparams[0].SIZE_REDUCE * NUM_PARTITIONS) / 512;
	
	step_type currentLOP = _globalparams.ACTSPARAMS_TREEDEPTH;
	batch_type num_source_partitions = get_num_source_partitions(currentLOP);

	for(unsigned int k=0; k<num_source_partitions; k++){ vmask_p[k] = 0; }
	
	bool_type enablereduce = ON;
	unsigned int ntravszs = 0;
	
	STARTREDUCE_MAINLOOP: for(batch_type source_partition=0; source_partition<num_source_partitions; source_partition+=1){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loop1 avg=analysis_loop1
		#ifdef _DEBUGMODE_KERNELPRINTS
		actsutilityobj->print3("### dispatch_reduce:: source_partition", "currentLOP", "NAp", source_partition, currentLOP, NAp); 							
		#endif
		
		enablereduce = ON;
		ntravszs = 0;
		rtravstate[0] = gettravstate(ON, kvdram0, globalparams[0], currentLOP, sourcestatsmarker);
		rtravstate[1] = gettravstate(ON, kvdram1, globalparams[1], currentLOP, sourcestatsmarker);
		rtravstate[2] = gettravstate(ON, kvdram2, globalparams[2], currentLOP, sourcestatsmarker);
		rtravstate[3] = gettravstate(ON, kvdram3, globalparams[3], currentLOP, sourcestatsmarker);
		rtravstate[4] = gettravstate(ON, kvdram4, globalparams[4], currentLOP, sourcestatsmarker);
		rtravstate[5] = gettravstate(ON, kvdram5, globalparams[5], currentLOP, sourcestatsmarker);
		rtravstate[6] = gettravstate(ON, kvdram6, globalparams[6], currentLOP, sourcestatsmarker);
		rtravstate[7] = gettravstate(ON, kvdram7, globalparams[7], currentLOP, sourcestatsmarker);
		rtravstate[8] = gettravstate(ON, kvdram8, globalparams[8], currentLOP, sourcestatsmarker);
		rtravstate[9] = gettravstate(ON, kvdram9, globalparams[9], currentLOP, sourcestatsmarker);
		rtravstate[10] = gettravstate(ON, kvdram10, globalparams[10], currentLOP, sourcestatsmarker);
		rtravstate[11] = gettravstate(ON, kvdram11, globalparams[11], currentLOP, sourcestatsmarker);
		rtravstate[12] = gettravstate(ON, kvdram12, globalparams[12], currentLOP, sourcestatsmarker);
		rtravstate[13] = gettravstate(ON, kvdram13, globalparams[13], currentLOP, sourcestatsmarker);
		rtravstate[14] = gettravstate(ON, kvdram14, globalparams[14], currentLOP, sourcestatsmarker);
		rtravstate[15] = gettravstate(ON, kvdram15, globalparams[15], currentLOP, sourcestatsmarker);
		for(unsigned int i = 0; i < NUMCOMPUTEUNITS; i++){ ntravszs += rtravstate[i].size_kvs; }
		if(ntravszs > 0){ enablereduce = ON; } else { enablereduce = OFF; }
		batch_type voffset_kvs = source_partition * reducebuffersz * FETFACTOR;
		
		// read vertices
		readkeyvalues(enablereduce, kvdram0, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer0_1, 0, 0, synvbuffer_head_1, 0, reducebuffersz, _globalparams);
		readkeyvalues(enablereduce, kvdram0, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset + reducebuffersz, vbuffer0_1, 8, 0, synvbuffer_head_1, 0, reducebuffersz, _globalparams);
		readkeyvalues(enablereduce, kvdram1, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer1_1, 0, 0,  reducebuffersz, _globalparams);
		readkeyvalues(enablereduce, kvdram1, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset + reducebuffersz, vbuffer1_1, 8, 0,  reducebuffersz, _globalparams);
		readkeyvalues(enablereduce, kvdram2, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer2_1, 0, 0,  reducebuffersz, _globalparams);
		readkeyvalues(enablereduce, kvdram2, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset + reducebuffersz, vbuffer2_1, 8, 0,  reducebuffersz, _globalparams);
		readkeyvalues(enablereduce, kvdram3, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer3_1, 0, 0,  reducebuffersz, _globalparams);
		readkeyvalues(enablereduce, kvdram3, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset + reducebuffersz, vbuffer3_1, 8, 0,  reducebuffersz, _globalparams);
		readkeyvalues(enablereduce, kvdram4, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer4_1, 0, 0,  reducebuffersz, _globalparams);
		readkeyvalues(enablereduce, kvdram4, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset + reducebuffersz, vbuffer4_1, 8, 0,  reducebuffersz, _globalparams);
		readkeyvalues(enablereduce, kvdram5, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer5_1, 0, 0,  reducebuffersz, _globalparams);
		readkeyvalues(enablereduce, kvdram5, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset + reducebuffersz, vbuffer5_1, 8, 0,  reducebuffersz, _globalparams);
		readkeyvalues(enablereduce, kvdram6, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer6_1, 0, 0,  reducebuffersz, _globalparams);
		readkeyvalues(enablereduce, kvdram6, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset + reducebuffersz, vbuffer6_1, 8, 0,  reducebuffersz, _globalparams);
		readkeyvalues(enablereduce, kvdram7, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer7_1, 0, 0,  reducebuffersz, _globalparams);
		readkeyvalues(enablereduce, kvdram7, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset + reducebuffersz, vbuffer7_1, 8, 0,  reducebuffersz, _globalparams);
		readkeyvalues(enablereduce, kvdram8, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer8_1, 0, 0,  reducebuffersz, _globalparams);
		readkeyvalues(enablereduce, kvdram8, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset + reducebuffersz, vbuffer8_1, 8, 0,  reducebuffersz, _globalparams);
		readkeyvalues(enablereduce, kvdram9, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer9_1, 0, 0,  reducebuffersz, _globalparams);
		readkeyvalues(enablereduce, kvdram9, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset + reducebuffersz, vbuffer9_1, 8, 0,  reducebuffersz, _globalparams);
		readkeyvalues(enablereduce, kvdram10, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer10_1, 0, 0,  reducebuffersz, _globalparams);
		readkeyvalues(enablereduce, kvdram10, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset + reducebuffersz, vbuffer10_1, 8, 0,  reducebuffersz, _globalparams);
		readkeyvalues(enablereduce, kvdram11, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer11_1, 0, 0,  reducebuffersz, _globalparams);
		readkeyvalues(enablereduce, kvdram11, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset + reducebuffersz, vbuffer11_1, 8, 0,  reducebuffersz, _globalparams);
		readkeyvalues(enablereduce, kvdram12, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer12_1, 0, 0,  reducebuffersz, _globalparams);
		readkeyvalues(enablereduce, kvdram12, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset + reducebuffersz, vbuffer12_1, 8, 0,  reducebuffersz, _globalparams);
		readkeyvalues(enablereduce, kvdram13, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer13_1, 0, 0,  reducebuffersz, _globalparams);
		readkeyvalues(enablereduce, kvdram13, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset + reducebuffersz, vbuffer13_1, 8, 0,  reducebuffersz, _globalparams);
		readkeyvalues(enablereduce, kvdram14, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer14_1, 0, 0,  reducebuffersz, _globalparams);
		readkeyvalues(enablereduce, kvdram14, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset + reducebuffersz, vbuffer14_1, 8, 0,  reducebuffersz, _globalparams);
		readkeyvalues(enablereduce, kvdram15, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer15_1, 0, 0,  reducebuffersz, _globalparams);
		readkeyvalues(enablereduce, kvdram15, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset + reducebuffersz, vbuffer15_1, 8, 0,  reducebuffersz, _globalparams);
		#ifndef ALLVERTEXISACTIVE_ALGORITHM
		resetvmask(vmask0);
		resetvmask(vmask1);
		resetvmask(vmask2);
		resetvmask(vmask3);
		resetvmask(vmask4);
		resetvmask(vmask5);
		resetvmask(vmask6);
		resetvmask(vmask7);
		resetvmask(vmask8);
		resetvmask(vmask9);
		resetvmask(vmask10);
		resetvmask(vmask11);
		resetvmask(vmask12);
		resetvmask(vmask13);
		resetvmask(vmask14);
		resetvmask(vmask15);
		#endif 
		
		// reduce
 
		dispatch(OFF, OFF, enablereduce, kvdram0, sourcebuffer0, vbuffer0_1, vmask0, vmask_p0, sourcestatsmarker, source_partition, _globalparams);
 
		dispatch(OFF, OFF, enablereduce, kvdram1, sourcebuffer1, vbuffer1_1, vmask1, vmask_p1, sourcestatsmarker, source_partition, _globalparams);
 
		dispatch(OFF, OFF, enablereduce, kvdram2, sourcebuffer2, vbuffer2_1, vmask2, vmask_p2, sourcestatsmarker, source_partition, _globalparams);
 
		dispatch(OFF, OFF, enablereduce, kvdram3, sourcebuffer3, vbuffer3_1, vmask3, vmask_p3, sourcestatsmarker, source_partition, _globalparams);
 
		dispatch(OFF, OFF, enablereduce, kvdram4, sourcebuffer4, vbuffer4_1, vmask4, vmask_p4, sourcestatsmarker, source_partition, _globalparams);
 
		dispatch(OFF, OFF, enablereduce, kvdram5, sourcebuffer5, vbuffer5_1, vmask5, vmask_p5, sourcestatsmarker, source_partition, _globalparams);
 
		dispatch(OFF, OFF, enablereduce, kvdram6, sourcebuffer6, vbuffer6_1, vmask6, vmask_p6, sourcestatsmarker, source_partition, _globalparams);
 
		dispatch(OFF, OFF, enablereduce, kvdram7, sourcebuffer7, vbuffer7_1, vmask7, vmask_p7, sourcestatsmarker, source_partition, _globalparams);
 
		dispatch(OFF, OFF, enablereduce, kvdram8, sourcebuffer8, vbuffer8_1, vmask8, vmask_p8, sourcestatsmarker, source_partition, _globalparams);
 
		dispatch(OFF, OFF, enablereduce, kvdram9, sourcebuffer9, vbuffer9_1, vmask9, vmask_p9, sourcestatsmarker, source_partition, _globalparams);
 
		dispatch(OFF, OFF, enablereduce, kvdram10, sourcebuffer10, vbuffer10_1, vmask10, vmask_p10, sourcestatsmarker, source_partition, _globalparams);
 
		dispatch(OFF, OFF, enablereduce, kvdram11, sourcebuffer11, vbuffer11_1, vmask11, vmask_p11, sourcestatsmarker, source_partition, _globalparams);
 
		dispatch(OFF, OFF, enablereduce, kvdram12, sourcebuffer12, vbuffer12_1, vmask12, vmask_p12, sourcestatsmarker, source_partition, _globalparams);
 
		dispatch(OFF, OFF, enablereduce, kvdram13, sourcebuffer13, vbuffer13_1, vmask13, vmask_p13, sourcestatsmarker, source_partition, _globalparams);
 
		dispatch(OFF, OFF, enablereduce, kvdram14, sourcebuffer14, vbuffer14_1, vmask14, vmask_p14, sourcestatsmarker, source_partition, _globalparams);
 
		dispatch(OFF, OFF, enablereduce, kvdram15, sourcebuffer15, vbuffer15_1, vmask15, vmask_p15, sourcestatsmarker, source_partition, _globalparams);
		
		// synchronize 
		#ifdef SYNCHRONIZEMODULE
		#ifdef SHIFTSYNCHRONIZE
		synchronize(enablereduce, synvbuffer_head_1, vbuffer0_1,vbuffer1_1,vbuffer2_1,vbuffer3_1,vbuffer4_1,vbuffer5_1,vbuffer6_1,vbuffer7_1,vbuffer8_1,vbuffer9_1,vbuffer10_1,vbuffer11_1,vbuffer12_1,vbuffer13_1,vbuffer14_1,vbuffer15_1, synvbuffer_tail_1, _globalparams);
		vmask_p[source_partition] = apply(enablereduce, synvbuffer_tail_1, synvbuffer_head_1, vmask0, voffset_kvs, _globalparams);
		spreadvdata(enablereduce, synvbuffer_tail_1, vbuffer15_1,vbuffer14_1,vbuffer13_1,vbuffer12_1,vbuffer11_1,vbuffer10_1,vbuffer9_1,vbuffer8_1,vbuffer7_1,vbuffer6_1,vbuffer5_1,vbuffer4_1,vbuffer3_1,vbuffer2_1,vbuffer1_1,vbuffer0_1, synvbuffer_head_1, _globalparams);  // CRITICAL FIXME. vbuffer_1s should be reversed in order to help sdaccel linker
		#ifndef ALLVERTEXISACTIVE_ALGORITHM
		spreadvmask(enablereduce, vmask0, vmask0,vmask1,vmask2,vmask3,vmask4,vmask5,vmask6,vmask7,vmask8,vmask9,vmask10,vmask11,vmask12,vmask13,vmask14,vmask15, _globalparams);
		#endif 
		#else 
		vmask_p[source_partition] = synchronize(enablereduce, synvbuffer_head_1, vbuffer0_1, vmask0,vbuffer1_1, vmask1,vbuffer2_1, vmask2,vbuffer3_1, vmask3,vbuffer4_1, vmask4,vbuffer5_1, vmask5,vbuffer6_1, vmask6,vbuffer7_1, vmask7,vbuffer8_1, vmask8,vbuffer9_1, vmask9,vbuffer10_1, vmask10,vbuffer11_1, vmask11,vbuffer12_1, vmask12,vbuffer13_1, vmask13,vbuffer14_1, vmask14,vbuffer15_1, vmask15, _globalparams); 	
		#endif
		#endif
		
		// writeback vertices
		savekeyvalues(enablereduce, kvdram0, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer0_1, 0, 0, reducebuffersz, _globalparams);
		savekeyvalues(enablereduce, kvdram0, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset + reducebuffersz, vbuffer0_1, 8, 0, reducebuffersz, _globalparams);
		savekeyvalues(enablereduce, kvdram1, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer1_1, 0, 0, reducebuffersz, _globalparams);
		savekeyvalues(enablereduce, kvdram1, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset + reducebuffersz, vbuffer1_1, 8, 0, reducebuffersz, _globalparams);
		savekeyvalues(enablereduce, kvdram2, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer2_1, 0, 0, reducebuffersz, _globalparams);
		savekeyvalues(enablereduce, kvdram2, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset + reducebuffersz, vbuffer2_1, 8, 0, reducebuffersz, _globalparams);
		savekeyvalues(enablereduce, kvdram3, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer3_1, 0, 0, reducebuffersz, _globalparams);
		savekeyvalues(enablereduce, kvdram3, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset + reducebuffersz, vbuffer3_1, 8, 0, reducebuffersz, _globalparams);
		savekeyvalues(enablereduce, kvdram4, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer4_1, 0, 0, reducebuffersz, _globalparams);
		savekeyvalues(enablereduce, kvdram4, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset + reducebuffersz, vbuffer4_1, 8, 0, reducebuffersz, _globalparams);
		savekeyvalues(enablereduce, kvdram5, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer5_1, 0, 0, reducebuffersz, _globalparams);
		savekeyvalues(enablereduce, kvdram5, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset + reducebuffersz, vbuffer5_1, 8, 0, reducebuffersz, _globalparams);
		savekeyvalues(enablereduce, kvdram6, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer6_1, 0, 0, reducebuffersz, _globalparams);
		savekeyvalues(enablereduce, kvdram6, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset + reducebuffersz, vbuffer6_1, 8, 0, reducebuffersz, _globalparams);
		savekeyvalues(enablereduce, kvdram7, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer7_1, 0, 0, reducebuffersz, _globalparams);
		savekeyvalues(enablereduce, kvdram7, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset + reducebuffersz, vbuffer7_1, 8, 0, reducebuffersz, _globalparams);
		savekeyvalues(enablereduce, kvdram8, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer8_1, 0, 0, reducebuffersz, _globalparams);
		savekeyvalues(enablereduce, kvdram8, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset + reducebuffersz, vbuffer8_1, 8, 0, reducebuffersz, _globalparams);
		savekeyvalues(enablereduce, kvdram9, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer9_1, 0, 0, reducebuffersz, _globalparams);
		savekeyvalues(enablereduce, kvdram9, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset + reducebuffersz, vbuffer9_1, 8, 0, reducebuffersz, _globalparams);
		savekeyvalues(enablereduce, kvdram10, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer10_1, 0, 0, reducebuffersz, _globalparams);
		savekeyvalues(enablereduce, kvdram10, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset + reducebuffersz, vbuffer10_1, 8, 0, reducebuffersz, _globalparams);
		savekeyvalues(enablereduce, kvdram11, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer11_1, 0, 0, reducebuffersz, _globalparams);
		savekeyvalues(enablereduce, kvdram11, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset + reducebuffersz, vbuffer11_1, 8, 0, reducebuffersz, _globalparams);
		savekeyvalues(enablereduce, kvdram12, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer12_1, 0, 0, reducebuffersz, _globalparams);
		savekeyvalues(enablereduce, kvdram12, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset + reducebuffersz, vbuffer12_1, 8, 0, reducebuffersz, _globalparams);
		savekeyvalues(enablereduce, kvdram13, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer13_1, 0, 0, reducebuffersz, _globalparams);
		savekeyvalues(enablereduce, kvdram13, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset + reducebuffersz, vbuffer13_1, 8, 0, reducebuffersz, _globalparams);
		savekeyvalues(enablereduce, kvdram14, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer14_1, 0, 0, reducebuffersz, _globalparams);
		savekeyvalues(enablereduce, kvdram14, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset + reducebuffersz, vbuffer14_1, 8, 0, reducebuffersz, _globalparams);
		savekeyvalues(enablereduce, kvdram15, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer15_1, 0, 0, reducebuffersz, _globalparams);
		savekeyvalues(enablereduce, kvdram15, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset + reducebuffersz, vbuffer15_1, 8, 0, reducebuffersz, _globalparams);
		#ifndef ALLVERTEXISACTIVE_ALGORITHM
		savevmasks(enablereduce, kvdram0, vmask0, sourcebuffer0, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset, vmaskbuffersz_kvs, _globalparams);
		savevmasks(enablereduce, kvdram1, vmask1, sourcebuffer1, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset, vmaskbuffersz_kvs, _globalparams);
		savevmasks(enablereduce, kvdram2, vmask2, sourcebuffer2, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset, vmaskbuffersz_kvs, _globalparams);
		savevmasks(enablereduce, kvdram3, vmask3, sourcebuffer3, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset, vmaskbuffersz_kvs, _globalparams);
		savevmasks(enablereduce, kvdram4, vmask4, sourcebuffer4, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset, vmaskbuffersz_kvs, _globalparams);
		savevmasks(enablereduce, kvdram5, vmask5, sourcebuffer5, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset, vmaskbuffersz_kvs, _globalparams);
		savevmasks(enablereduce, kvdram6, vmask6, sourcebuffer6, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset, vmaskbuffersz_kvs, _globalparams);
		savevmasks(enablereduce, kvdram7, vmask7, sourcebuffer7, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset, vmaskbuffersz_kvs, _globalparams);
		savevmasks(enablereduce, kvdram8, vmask8, sourcebuffer8, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset, vmaskbuffersz_kvs, _globalparams);
		savevmasks(enablereduce, kvdram9, vmask9, sourcebuffer9, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset, vmaskbuffersz_kvs, _globalparams);
		savevmasks(enablereduce, kvdram10, vmask10, sourcebuffer10, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset, vmaskbuffersz_kvs, _globalparams);
		savevmasks(enablereduce, kvdram11, vmask11, sourcebuffer11, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset, vmaskbuffersz_kvs, _globalparams);
		savevmasks(enablereduce, kvdram12, vmask12, sourcebuffer12, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset, vmaskbuffersz_kvs, _globalparams);
		savevmasks(enablereduce, kvdram13, vmask13, sourcebuffer13, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset, vmaskbuffersz_kvs, _globalparams);
		savevmasks(enablereduce, kvdram14, vmask14, sourcebuffer14, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset, vmaskbuffersz_kvs, _globalparams);
		savevmasks(enablereduce, kvdram15, vmask15, sourcebuffer15, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset, vmaskbuffersz_kvs, _globalparams);
		#endif 
		
		sourcestatsmarker += 1;
		vreadoffset += reducebuffersz * 2;
		vmaskreadoffset += vmaskbuffersz_kvs;
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printglobalvars();
	#endif
	return;
} 

extern "C" {
void 
	#ifdef SW 
	acts:: 
	#endif
topkernel(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11,uint512_dt * kvdram12,uint512_dt * kvdram13,uint512_dt * kvdram14,uint512_dt * kvdram15){ 
	
 
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0 // max_read_burst_length=64 max_write_burst_length=64			
 
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1 // max_read_burst_length=64 max_write_burst_length=64			
 
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2 // max_read_burst_length=64 max_write_burst_length=64			
 
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem3 // max_read_burst_length=64 max_write_burst_length=64			
 
#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem4 // max_read_burst_length=64 max_write_burst_length=64			
 
#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem5 // max_read_burst_length=64 max_write_burst_length=64			
 
#pragma HLS INTERFACE m_axi port = kvdram6 offset = slave bundle = gmem6 // max_read_burst_length=64 max_write_burst_length=64			
 
#pragma HLS INTERFACE m_axi port = kvdram7 offset = slave bundle = gmem7 // max_read_burst_length=64 max_write_burst_length=64			
 
#pragma HLS INTERFACE m_axi port = kvdram8 offset = slave bundle = gmem8 // max_read_burst_length=64 max_write_burst_length=64			
 
#pragma HLS INTERFACE m_axi port = kvdram9 offset = slave bundle = gmem9 // max_read_burst_length=64 max_write_burst_length=64			
 
#pragma HLS INTERFACE m_axi port = kvdram10 offset = slave bundle = gmem10 // max_read_burst_length=64 max_write_burst_length=64			
 
#pragma HLS INTERFACE m_axi port = kvdram11 offset = slave bundle = gmem11 // max_read_burst_length=64 max_write_burst_length=64			
 
#pragma HLS INTERFACE m_axi port = kvdram12 offset = slave bundle = gmem12 // max_read_burst_length=64 max_write_burst_length=64			
 
#pragma HLS INTERFACE m_axi port = kvdram13 offset = slave bundle = gmem13 // max_read_burst_length=64 max_write_burst_length=64			
 
#pragma HLS INTERFACE m_axi port = kvdram14 offset = slave bundle = gmem14 // max_read_burst_length=64 max_write_burst_length=64			
 
#pragma HLS INTERFACE m_axi port = kvdram15 offset = slave bundle = gmem15 // max_read_burst_length=64 max_write_burst_length=64			
		
 
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
 
#pragma HLS INTERFACE s_axilite port = kvdram12 bundle = control
 
#pragma HLS INTERFACE s_axilite port = kvdram13 bundle = control
 
#pragma HLS INTERFACE s_axilite port = kvdram14 bundle = control
 
#pragma HLS INTERFACE s_axilite port = kvdram15 bundle = control

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
 
#pragma HLS DATA_PACK variable = kvdram12
 
#pragma HLS DATA_PACK variable = kvdram13
 
#pragma HLS DATA_PACK variable = kvdram14
 
#pragma HLS DATA_PACK variable = kvdram15

	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printparameters();
	#endif 
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	#ifdef _WIDEWORD
  
	cout<<">>> Light weight ACTS 0 (NACTS_IN_1COMPUTEUNIT) Launched... size: "<<(unsigned int)(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN].range(31, 0))<<endl; 
  
	cout<<">>> Light weight ACTS 1 (NACTS_IN_1COMPUTEUNIT) Launched... size: "<<(unsigned int)(kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN].range(31, 0))<<endl; 
  
	cout<<">>> Light weight ACTS 2 (NACTS_IN_1COMPUTEUNIT) Launched... size: "<<(unsigned int)(kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN].range(31, 0))<<endl; 
  
	cout<<">>> Light weight ACTS 3 (NACTS_IN_1COMPUTEUNIT) Launched... size: "<<(unsigned int)(kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN].range(31, 0))<<endl; 
  
	cout<<">>> Light weight ACTS 4 (NACTS_IN_1COMPUTEUNIT) Launched... size: "<<(unsigned int)(kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN].range(31, 0))<<endl; 
  
	cout<<">>> Light weight ACTS 5 (NACTS_IN_1COMPUTEUNIT) Launched... size: "<<(unsigned int)(kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN].range(31, 0))<<endl; 
  
	cout<<">>> Light weight ACTS 6 (NACTS_IN_1COMPUTEUNIT) Launched... size: "<<(unsigned int)(kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN].range(31, 0))<<endl; 
  
	cout<<">>> Light weight ACTS 7 (NACTS_IN_1COMPUTEUNIT) Launched... size: "<<(unsigned int)(kvdram7[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN].range(31, 0))<<endl; 
  
	cout<<">>> Light weight ACTS 8 (NACTS_IN_1COMPUTEUNIT) Launched... size: "<<(unsigned int)(kvdram8[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN].range(31, 0))<<endl; 
  
	cout<<">>> Light weight ACTS 9 (NACTS_IN_1COMPUTEUNIT) Launched... size: "<<(unsigned int)(kvdram9[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN].range(31, 0))<<endl; 
  
	cout<<">>> Light weight ACTS 10 (NACTS_IN_1COMPUTEUNIT) Launched... size: "<<(unsigned int)(kvdram10[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN].range(31, 0))<<endl; 
  
	cout<<">>> Light weight ACTS 11 (NACTS_IN_1COMPUTEUNIT) Launched... size: "<<(unsigned int)(kvdram11[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN].range(31, 0))<<endl; 
  
	cout<<">>> Light weight ACTS 12 (NACTS_IN_1COMPUTEUNIT) Launched... size: "<<(unsigned int)(kvdram12[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN].range(31, 0))<<endl; 
  
	cout<<">>> Light weight ACTS 13 (NACTS_IN_1COMPUTEUNIT) Launched... size: "<<(unsigned int)(kvdram13[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN].range(31, 0))<<endl; 
  
	cout<<">>> Light weight ACTS 14 (NACTS_IN_1COMPUTEUNIT) Launched... size: "<<(unsigned int)(kvdram14[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN].range(31, 0))<<endl; 
  
	cout<<">>> Light weight ACTS 15 (NACTS_IN_1COMPUTEUNIT) Launched... size: "<<(unsigned int)(kvdram15[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN].range(31, 0))<<endl; 
	#else
  
	cout<<">>> Light weight ACTS 0 (NACTS_IN_1COMPUTEUNIT) Launched... size: "<<kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN].data[0].key<<endl; 
  
	cout<<">>> Light weight ACTS 1 (NACTS_IN_1COMPUTEUNIT) Launched... size: "<<kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN].data[0].key<<endl; 
  
	cout<<">>> Light weight ACTS 2 (NACTS_IN_1COMPUTEUNIT) Launched... size: "<<kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN].data[0].key<<endl; 
  
	cout<<">>> Light weight ACTS 3 (NACTS_IN_1COMPUTEUNIT) Launched... size: "<<kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN].data[0].key<<endl; 
  
	cout<<">>> Light weight ACTS 4 (NACTS_IN_1COMPUTEUNIT) Launched... size: "<<kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN].data[0].key<<endl; 
  
	cout<<">>> Light weight ACTS 5 (NACTS_IN_1COMPUTEUNIT) Launched... size: "<<kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN].data[0].key<<endl; 
  
	cout<<">>> Light weight ACTS 6 (NACTS_IN_1COMPUTEUNIT) Launched... size: "<<kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN].data[0].key<<endl; 
  
	cout<<">>> Light weight ACTS 7 (NACTS_IN_1COMPUTEUNIT) Launched... size: "<<kvdram7[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN].data[0].key<<endl; 
  
	cout<<">>> Light weight ACTS 8 (NACTS_IN_1COMPUTEUNIT) Launched... size: "<<kvdram8[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN].data[0].key<<endl; 
  
	cout<<">>> Light weight ACTS 9 (NACTS_IN_1COMPUTEUNIT) Launched... size: "<<kvdram9[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN].data[0].key<<endl; 
  
	cout<<">>> Light weight ACTS 10 (NACTS_IN_1COMPUTEUNIT) Launched... size: "<<kvdram10[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN].data[0].key<<endl; 
  
	cout<<">>> Light weight ACTS 11 (NACTS_IN_1COMPUTEUNIT) Launched... size: "<<kvdram11[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN].data[0].key<<endl; 
  
	cout<<">>> Light weight ACTS 12 (NACTS_IN_1COMPUTEUNIT) Launched... size: "<<kvdram12[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN].data[0].key<<endl; 
  
	cout<<">>> Light weight ACTS 13 (NACTS_IN_1COMPUTEUNIT) Launched... size: "<<kvdram13[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN].data[0].key<<endl; 
  
	cout<<">>> Light weight ACTS 14 (NACTS_IN_1COMPUTEUNIT) Launched... size: "<<kvdram14[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN].data[0].key<<endl; 
  
	cout<<">>> Light weight ACTS 15 (NACTS_IN_1COMPUTEUNIT) Launched... size: "<<kvdram15[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN].data[0].key<<endl; 
	#endif
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"acts::topkernel:: APPLYVERTEXBUFFERSZ: "<<APPLYVERTEXBUFFERSZ<<endl;
	#endif
	
	keyvalue_t sourcebuffer0[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = sourcebuffer0
	keyvalue16_type vbuffer0_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer0_1
	uintNUMPby2_type vmask0[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask0
	uint32_type vmask_p0[BLOCKRAM_SIZE];
	keyvalue_t sourcebuffer1[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = sourcebuffer1
	keyvalue16_type vbuffer1_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer1_1
	uintNUMPby2_type vmask1[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask1
	uint32_type vmask_p1[BLOCKRAM_SIZE];
	keyvalue_t sourcebuffer2[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = sourcebuffer2
	keyvalue16_type vbuffer2_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer2_1
	uintNUMPby2_type vmask2[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask2
	uint32_type vmask_p2[BLOCKRAM_SIZE];
	keyvalue_t sourcebuffer3[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = sourcebuffer3
	keyvalue16_type vbuffer3_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer3_1
	uintNUMPby2_type vmask3[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask3
	uint32_type vmask_p3[BLOCKRAM_SIZE];
	keyvalue_t sourcebuffer4[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = sourcebuffer4
	keyvalue16_type vbuffer4_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer4_1
	uintNUMPby2_type vmask4[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask4
	uint32_type vmask_p4[BLOCKRAM_SIZE];
	keyvalue_t sourcebuffer5[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = sourcebuffer5
	keyvalue16_type vbuffer5_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer5_1
	uintNUMPby2_type vmask5[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask5
	uint32_type vmask_p5[BLOCKRAM_SIZE];
	keyvalue_t sourcebuffer6[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = sourcebuffer6
	keyvalue16_type vbuffer6_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer6_1
	uintNUMPby2_type vmask6[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask6
	uint32_type vmask_p6[BLOCKRAM_SIZE];
	keyvalue_t sourcebuffer7[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = sourcebuffer7
	keyvalue16_type vbuffer7_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer7_1
	uintNUMPby2_type vmask7[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask7
	uint32_type vmask_p7[BLOCKRAM_SIZE];
	keyvalue_t sourcebuffer8[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = sourcebuffer8
	keyvalue16_type vbuffer8_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer8_1
	uintNUMPby2_type vmask8[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask8
	uint32_type vmask_p8[BLOCKRAM_SIZE];
	keyvalue_t sourcebuffer9[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = sourcebuffer9
	keyvalue16_type vbuffer9_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer9_1
	uintNUMPby2_type vmask9[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask9
	uint32_type vmask_p9[BLOCKRAM_SIZE];
	keyvalue_t sourcebuffer10[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = sourcebuffer10
	keyvalue16_type vbuffer10_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer10_1
	uintNUMPby2_type vmask10[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask10
	uint32_type vmask_p10[BLOCKRAM_SIZE];
	keyvalue_t sourcebuffer11[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = sourcebuffer11
	keyvalue16_type vbuffer11_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer11_1
	uintNUMPby2_type vmask11[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask11
	uint32_type vmask_p11[BLOCKRAM_SIZE];
	keyvalue_t sourcebuffer12[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = sourcebuffer12
	keyvalue16_type vbuffer12_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer12_1
	uintNUMPby2_type vmask12[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask12
	uint32_type vmask_p12[BLOCKRAM_SIZE];
	keyvalue_t sourcebuffer13[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = sourcebuffer13
	keyvalue16_type vbuffer13_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer13_1
	uintNUMPby2_type vmask13[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask13
	uint32_type vmask_p13[BLOCKRAM_SIZE];
	keyvalue_t sourcebuffer14[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = sourcebuffer14
	keyvalue16_type vbuffer14_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer14_1
	uintNUMPby2_type vmask14[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask14
	uint32_type vmask_p14[BLOCKRAM_SIZE];
	keyvalue_t sourcebuffer15[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = sourcebuffer15
	keyvalue16_type vbuffer15_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer15_1
	uintNUMPby2_type vmask15[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask15
	uint32_type vmask_p15[BLOCKRAM_SIZE];
	globalparams_t globalparams[NUMCOMPUTEUNITS];
	#pragma HLS ARRAY_PARTITION variable=globalparams complete
	uint32_type vmask_p[BLOCKRAM_SIZE];
	
	globalparams[0] = getglobalparams(kvdram0);
 
 
	globalparams[1] = globalparams[0];
 
	globalparams[2] = globalparams[0];
 
	globalparams[3] = globalparams[0];
 
	globalparams[4] = globalparams[0];
 
	globalparams[5] = globalparams[0];
 
	globalparams[6] = globalparams[0];
 
	globalparams[7] = globalparams[0];
 
	globalparams[8] = globalparams[0];
 
	globalparams[9] = globalparams[0];
 
	globalparams[10] = globalparams[0];
 
	globalparams[11] = globalparams[0];
 
	globalparams[12] = globalparams[0];
 
	globalparams[13] = globalparams[0];
 
	globalparams[14] = globalparams[0];
 
	globalparams[15] = globalparams[0];

	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ vmask_p[k] = 0; }
	vmask_p[0] = 0x00000001; // just for test. assuming rootvid=1

	unsigned int numGraphIters = globalparams[0].ALGORITHMINFO_GRAPHITERATIONID;
	unsigned int numactvvs = 1;
	
	RUNITERATIONS_LOOP: for(unsigned int GraphIter=0; GraphIter<numGraphIters; GraphIter++){
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<">>> Light weight ACTS: Graph Iteration: "<<GraphIter<<": ("<<numactvvs<<" active vertices)"<<endl;
		#endif
		
		for(unsigned int i=0; i<NUMCOMPUTEUNITS; i++){ globalparams[i].ALGORITHMINFO_GRAPHITERATIONID = GraphIter; }
		START_REPLICATEVMASK_LOOP1: for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){
			vmask_p0[k] = vmask_p[k]; 
			vmask_p1[k] = vmask_p[k]; 
			vmask_p2[k] = vmask_p[k]; 
			vmask_p3[k] = vmask_p[k]; 
			vmask_p4[k] = vmask_p[k]; 
			vmask_p5[k] = vmask_p[k]; 
			vmask_p6[k] = vmask_p[k]; 
			vmask_p7[k] = vmask_p[k]; 
			vmask_p8[k] = vmask_p[k]; 
			vmask_p9[k] = vmask_p[k]; 
			vmask_p10[k] = vmask_p[k]; 
			vmask_p11[k] = vmask_p[k]; 
			vmask_p12[k] = vmask_p[k]; 
			vmask_p13[k] = vmask_p[k]; 
			vmask_p14[k] = vmask_p[k]; 
			vmask_p15[k] = vmask_p[k]; 
		} 
		
		#ifndef ALLVERTEXISACTIVE_ALGORITHM
		resetkvdramstats(kvdram0, globalparams[0]);
		resetkvdramstats(kvdram1, globalparams[0]);
		resetkvdramstats(kvdram2, globalparams[0]);
		resetkvdramstats(kvdram3, globalparams[0]);
		resetkvdramstats(kvdram4, globalparams[0]);
		resetkvdramstats(kvdram5, globalparams[0]);
		resetkvdramstats(kvdram6, globalparams[0]);
		resetkvdramstats(kvdram7, globalparams[0]);
		resetkvdramstats(kvdram8, globalparams[0]);
		resetkvdramstats(kvdram9, globalparams[0]);
		resetkvdramstats(kvdram10, globalparams[0]);
		resetkvdramstats(kvdram11, globalparams[0]);
		resetkvdramstats(kvdram12, globalparams[0]);
		resetkvdramstats(kvdram13, globalparams[0]);
		resetkvdramstats(kvdram14, globalparams[0]);
		resetkvdramstats(kvdram15, globalparams[0]);
		#endif 
		
		// process & partition
		#ifdef PROCESSMODULE
		if(globalparams[0].ENABLE_PROCESSCOMMAND == ON){ 
 
			#ifdef _DEBUGMODE_KERNELPRINTS2
			cout<<"topkernel: processing instance 0... "<<endl;
			#endif
			dispatch(ON, OFF, OFF, kvdram0, sourcebuffer0, vbuffer0_1, vmask0, vmask_p0, NAp, NAp, globalparams[0]);
 
			#ifdef _DEBUGMODE_KERNELPRINTS2
			cout<<"topkernel: processing instance 1... "<<endl;
			#endif
			dispatch(ON, OFF, OFF, kvdram1, sourcebuffer1, vbuffer1_1, vmask1, vmask_p1, NAp, NAp, globalparams[1]);
 
			#ifdef _DEBUGMODE_KERNELPRINTS2
			cout<<"topkernel: processing instance 2... "<<endl;
			#endif
			dispatch(ON, OFF, OFF, kvdram2, sourcebuffer2, vbuffer2_1, vmask2, vmask_p2, NAp, NAp, globalparams[2]);
 
			#ifdef _DEBUGMODE_KERNELPRINTS2
			cout<<"topkernel: processing instance 3... "<<endl;
			#endif
			dispatch(ON, OFF, OFF, kvdram3, sourcebuffer3, vbuffer3_1, vmask3, vmask_p3, NAp, NAp, globalparams[3]);
 
			#ifdef _DEBUGMODE_KERNELPRINTS2
			cout<<"topkernel: processing instance 4... "<<endl;
			#endif
			dispatch(ON, OFF, OFF, kvdram4, sourcebuffer4, vbuffer4_1, vmask4, vmask_p4, NAp, NAp, globalparams[4]);
 
			#ifdef _DEBUGMODE_KERNELPRINTS2
			cout<<"topkernel: processing instance 5... "<<endl;
			#endif
			dispatch(ON, OFF, OFF, kvdram5, sourcebuffer5, vbuffer5_1, vmask5, vmask_p5, NAp, NAp, globalparams[5]);
 
			#ifdef _DEBUGMODE_KERNELPRINTS2
			cout<<"topkernel: processing instance 6... "<<endl;
			#endif
			dispatch(ON, OFF, OFF, kvdram6, sourcebuffer6, vbuffer6_1, vmask6, vmask_p6, NAp, NAp, globalparams[6]);
 
			#ifdef _DEBUGMODE_KERNELPRINTS2
			cout<<"topkernel: processing instance 7... "<<endl;
			#endif
			dispatch(ON, OFF, OFF, kvdram7, sourcebuffer7, vbuffer7_1, vmask7, vmask_p7, NAp, NAp, globalparams[7]);
 
			#ifdef _DEBUGMODE_KERNELPRINTS2
			cout<<"topkernel: processing instance 8... "<<endl;
			#endif
			dispatch(ON, OFF, OFF, kvdram8, sourcebuffer8, vbuffer8_1, vmask8, vmask_p8, NAp, NAp, globalparams[8]);
 
			#ifdef _DEBUGMODE_KERNELPRINTS2
			cout<<"topkernel: processing instance 9... "<<endl;
			#endif
			dispatch(ON, OFF, OFF, kvdram9, sourcebuffer9, vbuffer9_1, vmask9, vmask_p9, NAp, NAp, globalparams[9]);
 
			#ifdef _DEBUGMODE_KERNELPRINTS2
			cout<<"topkernel: processing instance 10... "<<endl;
			#endif
			dispatch(ON, OFF, OFF, kvdram10, sourcebuffer10, vbuffer10_1, vmask10, vmask_p10, NAp, NAp, globalparams[10]);
 
			#ifdef _DEBUGMODE_KERNELPRINTS2
			cout<<"topkernel: processing instance 11... "<<endl;
			#endif
			dispatch(ON, OFF, OFF, kvdram11, sourcebuffer11, vbuffer11_1, vmask11, vmask_p11, NAp, NAp, globalparams[11]);
 
			#ifdef _DEBUGMODE_KERNELPRINTS2
			cout<<"topkernel: processing instance 12... "<<endl;
			#endif
			dispatch(ON, OFF, OFF, kvdram12, sourcebuffer12, vbuffer12_1, vmask12, vmask_p12, NAp, NAp, globalparams[12]);
 
			#ifdef _DEBUGMODE_KERNELPRINTS2
			cout<<"topkernel: processing instance 13... "<<endl;
			#endif
			dispatch(ON, OFF, OFF, kvdram13, sourcebuffer13, vbuffer13_1, vmask13, vmask_p13, NAp, NAp, globalparams[13]);
 
			#ifdef _DEBUGMODE_KERNELPRINTS2
			cout<<"topkernel: processing instance 14... "<<endl;
			#endif
			dispatch(ON, OFF, OFF, kvdram14, sourcebuffer14, vbuffer14_1, vmask14, vmask_p14, NAp, NAp, globalparams[14]);
 
			#ifdef _DEBUGMODE_KERNELPRINTS2
			cout<<"topkernel: processing instance 15... "<<endl;
			#endif
			dispatch(ON, OFF, OFF, kvdram15, sourcebuffer15, vbuffer15_1, vmask15, vmask_p15, NAp, NAp, globalparams[15]);
		}
		#endif 
		
		// partition
		#ifdef PARTITIONMODULE
		if(globalparams[0].ENABLE_PARTITIONCOMMAND == ON){ 
			#ifdef _DEBUGMODE_KERNELPRINTS2
			cout<<"topkernel: partitioning instance 0... "<<endl;
			#endif
			dispatch(OFF, ON, OFF, kvdram0, sourcebuffer0, vbuffer0_1, vmask0, vmask_p0, NAp, NAp, globalparams[0]);
			#ifdef _DEBUGMODE_KERNELPRINTS2
			cout<<"topkernel: partitioning instance 1... "<<endl;
			#endif
			dispatch(OFF, ON, OFF, kvdram1, sourcebuffer1, vbuffer1_1, vmask1, vmask_p1, NAp, NAp, globalparams[1]);
			#ifdef _DEBUGMODE_KERNELPRINTS2
			cout<<"topkernel: partitioning instance 2... "<<endl;
			#endif
			dispatch(OFF, ON, OFF, kvdram2, sourcebuffer2, vbuffer2_1, vmask2, vmask_p2, NAp, NAp, globalparams[2]);
			#ifdef _DEBUGMODE_KERNELPRINTS2
			cout<<"topkernel: partitioning instance 3... "<<endl;
			#endif
			dispatch(OFF, ON, OFF, kvdram3, sourcebuffer3, vbuffer3_1, vmask3, vmask_p3, NAp, NAp, globalparams[3]);
			#ifdef _DEBUGMODE_KERNELPRINTS2
			cout<<"topkernel: partitioning instance 4... "<<endl;
			#endif
			dispatch(OFF, ON, OFF, kvdram4, sourcebuffer4, vbuffer4_1, vmask4, vmask_p4, NAp, NAp, globalparams[4]);
			#ifdef _DEBUGMODE_KERNELPRINTS2
			cout<<"topkernel: partitioning instance 5... "<<endl;
			#endif
			dispatch(OFF, ON, OFF, kvdram5, sourcebuffer5, vbuffer5_1, vmask5, vmask_p5, NAp, NAp, globalparams[5]);
			#ifdef _DEBUGMODE_KERNELPRINTS2
			cout<<"topkernel: partitioning instance 6... "<<endl;
			#endif
			dispatch(OFF, ON, OFF, kvdram6, sourcebuffer6, vbuffer6_1, vmask6, vmask_p6, NAp, NAp, globalparams[6]);
			#ifdef _DEBUGMODE_KERNELPRINTS2
			cout<<"topkernel: partitioning instance 7... "<<endl;
			#endif
			dispatch(OFF, ON, OFF, kvdram7, sourcebuffer7, vbuffer7_1, vmask7, vmask_p7, NAp, NAp, globalparams[7]);
			#ifdef _DEBUGMODE_KERNELPRINTS2
			cout<<"topkernel: partitioning instance 8... "<<endl;
			#endif
			dispatch(OFF, ON, OFF, kvdram8, sourcebuffer8, vbuffer8_1, vmask8, vmask_p8, NAp, NAp, globalparams[8]);
			#ifdef _DEBUGMODE_KERNELPRINTS2
			cout<<"topkernel: partitioning instance 9... "<<endl;
			#endif
			dispatch(OFF, ON, OFF, kvdram9, sourcebuffer9, vbuffer9_1, vmask9, vmask_p9, NAp, NAp, globalparams[9]);
			#ifdef _DEBUGMODE_KERNELPRINTS2
			cout<<"topkernel: partitioning instance 10... "<<endl;
			#endif
			dispatch(OFF, ON, OFF, kvdram10, sourcebuffer10, vbuffer10_1, vmask10, vmask_p10, NAp, NAp, globalparams[10]);
			#ifdef _DEBUGMODE_KERNELPRINTS2
			cout<<"topkernel: partitioning instance 11... "<<endl;
			#endif
			dispatch(OFF, ON, OFF, kvdram11, sourcebuffer11, vbuffer11_1, vmask11, vmask_p11, NAp, NAp, globalparams[11]);
			#ifdef _DEBUGMODE_KERNELPRINTS2
			cout<<"topkernel: partitioning instance 12... "<<endl;
			#endif
			dispatch(OFF, ON, OFF, kvdram12, sourcebuffer12, vbuffer12_1, vmask12, vmask_p12, NAp, NAp, globalparams[12]);
			#ifdef _DEBUGMODE_KERNELPRINTS2
			cout<<"topkernel: partitioning instance 13... "<<endl;
			#endif
			dispatch(OFF, ON, OFF, kvdram13, sourcebuffer13, vbuffer13_1, vmask13, vmask_p13, NAp, NAp, globalparams[13]);
			#ifdef _DEBUGMODE_KERNELPRINTS2
			cout<<"topkernel: partitioning instance 14... "<<endl;
			#endif
			dispatch(OFF, ON, OFF, kvdram14, sourcebuffer14, vbuffer14_1, vmask14, vmask_p14, NAp, NAp, globalparams[14]);
			#ifdef _DEBUGMODE_KERNELPRINTS2
			cout<<"topkernel: partitioning instance 15... "<<endl;
			#endif
			dispatch(OFF, ON, OFF, kvdram15, sourcebuffer15, vbuffer15_1, vmask15, vmask_p15, NAp, NAp, globalparams[15]);
		}
		#endif 
		// exit(EXIT_SUCCESS); // CRITICAL REMOVEME.
		
		// reduce & partition
		#ifdef REDUCEMODULE
		if(globalparams[0].ENABLE_APPLYUPDATESCOMMAND == ON){ 
			#ifdef _DEBUGMODE_KERNELPRINTS2
			cout<<"topkernel: reducing instances 0-16... "<<endl;
			#endif
			dispatch_reduce(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9,kvdram10,kvdram11,kvdram12,kvdram13,kvdram14,kvdram15, 
sourcebuffer0, vbuffer0_1, vmask0, vmask_p0,sourcebuffer1, vbuffer1_1, vmask1, vmask_p1,sourcebuffer2, vbuffer2_1, vmask2, vmask_p2,sourcebuffer3, vbuffer3_1, vmask3, vmask_p3,sourcebuffer4, vbuffer4_1, vmask4, vmask_p4,sourcebuffer5, vbuffer5_1, vmask5, vmask_p5,sourcebuffer6, vbuffer6_1, vmask6, vmask_p6,sourcebuffer7, vbuffer7_1, vmask7, vmask_p7,sourcebuffer8, vbuffer8_1, vmask8, vmask_p8,sourcebuffer9, vbuffer9_1, vmask9, vmask_p9,sourcebuffer10, vbuffer10_1, vmask10, vmask_p10,sourcebuffer11, vbuffer11_1, vmask11, vmask_p11,sourcebuffer12, vbuffer12_1, vmask12, vmask_p12,sourcebuffer13, vbuffer13_1, vmask13, vmask_p13,sourcebuffer14, vbuffer14_1, vmask14, vmask_p14,sourcebuffer15, vbuffer15_1, vmask15, vmask_p15, 
							vmask_p, globalparams);
		}
		#endif 
		
		#if defined(_DEBUGMODE_KERNELPRINTS3) && not defined (ALLVERTEXISACTIVE_ALGORITHM)
		cout<<"active partitions: ";
		for(unsigned int k=0; k<256; k++){ if(vmask_p[k]>0){ cout<<k<<", "; }}
		cout<<""<<endl;
		#endif 
		#ifdef _DEBUGMODE_KERNELPRINTS2
		actsutilityobj->printglobalvars();
		#endif 
		#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(_DEBUGMODE_CHECKS2)
		actsutilityobj->clearglobalvars();
		#endif
		#ifdef _DEBUGMODE_STATS
		numactvvs = actsutilityobj->globalstats_getactvvsseen();
		cout<< TIMINGRESULTSCOLOR <<"num active vertices for iteration "<<GraphIter+1<<": "<<numactvvs<< RESET <<endl;
		actsutilityobj->globalstats_setactvvsseen(0);
		#endif 
		
		unsigned int actvvstatus = 0;
		for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ if(vmask_p[k] > 0){ actvvstatus = 1; }} 
		if(actvvstatus == 0){ 
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"no more active vertices to process. breaking out... "<<endl;
			#endif 
			break; }
	}
	return;
}
}
#endif 

// top
#ifdef NACTS_IN_NCOMPUTEUNITS
void
	#ifdef SW 
	acts::
	#endif 
dispatch_reduce(uint512_dt * kvdram, keyvalue_t sourcebuffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t vbuffer_1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t vbuffer_2[VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask[BLOCKRAM_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], globalparams_t globalparams){
	#pragma HLS INLINE
	analysis_type analysis_loop1 = 1;
	analysis_type analysis_treedepth = TREE_DEPTH;
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	unsigned int sourcestatsmarker = 0;
	for(unsigned int k=0; k<globalparams.ACTSPARAMS_TREEDEPTH-1; k++){ 
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_treedepth avg=analysis_treedepth
		sourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); 
	}
	
	unsigned int vreadoffset = 0;
	buffer_type reducebuffersz = globalparams.SIZE_REDUCE / 2;
	
	step_type currentLOP = globalparams.ACTSPARAMS_TREEDEPTH;
	batch_type num_source_partitions = get_num_source_partitions(currentLOP);

	for(unsigned int k=0; k<num_source_partitions; k++){ vmask_p[k] = 0; }
	
	bool_type enablereduce = ON;
	unsigned int ntravszs = 0;
	
	DISPATCHREDUCE_MAINLOOP: for(batch_type source_partition=0; source_partition<num_source_partitions; source_partition+=1){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loop1 avg=analysis_loop1
		#ifdef _DEBUGMODE_KERNELPRINTS
		actsutilityobj->print3("### dispatch_reduce:: source_partition", "currentLOP", "NAp", source_partition, currentLOP, NAp); 							
		#endif
		
		enablereduce = ON;
		travstate_t rtravstate = gettravstate(ON, kvdram, globalparams, currentLOP, sourcestatsmarker);
		if(rtravstate.size_kvs > 0){ enablereduce = ON; } else { enablereduce = OFF; }
		batch_type voffset_kvs = source_partition * reducebuffersz * FETFACTOR;
		
		// read vertices
		readkeyvalues(enablereduce, kvdram, globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer_1, 0, 0, reducebuffersz, globalparams);
		readkeyvalues(enablereduce, kvdram, globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset + reducebuffersz, vbuffer_1, 8, 0, reducebuffersz, globalparams);
		
		// reduce
		dispatch(OFF, OFF, enablereduce, kvdram, sourcebuffer, vbuffer_1, vbuffer_2, vmask, vmask_p, sourcestatsmarker, source_partition, globalparams);
		
		// writeback vertices
		savekeyvalues(enablereduce, kvdram, globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer_1, 0, 0, reducebuffersz, globalparams);
		savekeyvalues(enablereduce, kvdram, globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset + reducebuffersz, vbuffer_1, 8, 0, reducebuffersz, globalparams);
		
		sourcestatsmarker += 1;
		vreadoffset += reducebuffersz * 2;
	}
	return;
} 

extern "C" {
void 
	#ifdef SW 
	acts:: 
	#endif
topkernel(uint512_dt * kvdram){
#pragma HLS INTERFACE m_axi port = kvdram offset = slave bundle = gmem0			
		
#pragma HLS INTERFACE s_axilite port = kvdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATA_PACK variable = kvdram

	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printparameters();
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS3
	#ifdef _WIDEWORD
	cout<<">>> Light weight ACTS (NACTS_IN_NCOMPUTEUNITS) Launched... size: "<<(unsigned int)(kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN].range(31, 0))<<endl; 
	#else
	cout<<">>> Light weight ACTS (NACTS_IN_NCOMPUTEUNITS) Launched... size: "<<kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN].data[0].key<<endl; 
	#endif
	#endif
	
	keyvalue_t sourcebuffer[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = sourcebuffer
	keyvalue_t vbuffer_1[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer_1	
	keyvalue_t vbuffer_2[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer_2
	uintNUMPby2_type vmask[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask
	uint32_type vmask_p[BLOCKRAM_SIZE];
	
	globalparams_t globalparams = getglobalparams(kvdram);
	
	#ifndef ALLVERTEXISACTIVE_ALGORITHM
	resetkvdramstats(kvdram, globalparams);
	#endif
	
	// process & partition
	#ifdef PROCESSMODULE
	if(globalparams.ENABLE_PROCESSCOMMAND == ON){ 
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<"topkernel: processing instance ... "<<endl;
		#endif
		dispatch(ON, OFF, OFF, kvdram, sourcebuffer, vbuffer_1, vbuffer_2, vmask, vmask_p, NAp, NAp, globalparams);
	}
	#endif 
	
	// partition
	#ifdef PARTITIONMODULE
	if(globalparams.ENABLE_PARTITIONCOMMAND == ON){ 
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<"topkernel: partitioning instance ... "<<endl;
		#endif
		dispatch(OFF, ON, OFF, kvdram, sourcebuffer, vbuffer_1, vbuffer_2, vmask, vmask_p, NAp, NAp, globalparams);
	}
	#endif 
	
	// reduce & partition
	#ifdef REDUCEMODULE
	if(globalparams.ENABLE_APPLYUPDATESCOMMAND == ON){ 
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<"topkernel: reducing instance ... "<<endl;
		#endif
		dispatch_reduce(kvdram, sourcebuffer, vbuffer_1, vbuffer_2, vmask, vmask_p, globalparams);
	}
	#endif 
	
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->printglobalvars();
	#endif 
	#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(_DEBUGMODE_CHECKS2)
	actsutilityobj->clearglobalvars();
	#endif
	return;
}
}

extern "C" {
void 
	#ifdef SW 
	acts:: 
	#endif
topkernelsync(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11,uint512_dt * kvdram12,uint512_dt * kvdram13,uint512_dt * kvdram14,uint512_dt * kvdram15){
 
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
 
#pragma HLS INTERFACE m_axi port = kvdram12 offset = slave bundle = gmem12		
 
#pragma HLS INTERFACE m_axi port = kvdram13 offset = slave bundle = gmem13		
 
#pragma HLS INTERFACE m_axi port = kvdram14 offset = slave bundle = gmem14		
 
#pragma HLS INTERFACE m_axi port = kvdram15 offset = slave bundle = gmem15		

 
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
 
#pragma HLS INTERFACE s_axilite port = kvdram12 bundle = control
 
#pragma HLS INTERFACE s_axilite port = kvdram13 bundle = control
 
#pragma HLS INTERFACE s_axilite port = kvdram14 bundle = control
 
#pragma HLS INTERFACE s_axilite port = kvdram15 bundle = control

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
 
#pragma HLS DATA_PACK variable = kvdram12
 
#pragma HLS DATA_PACK variable = kvdram13
 
#pragma HLS DATA_PACK variable = kvdram14
 
#pragma HLS DATA_PACK variable = kvdram15

	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printparameters();
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> Light weight ACTS (NACTS_IN_NCOMPUTEUNITS.SYNC) Launched... "<<endl; 
	#endif
	
	keyvalue16_type synvbuffer_head_1[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = synvbuffer_head_1
	
	keyvalue16_type synvbuffer_tail_1[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = synvbuffer_tail_1
	
	travstate_t rtravstate[NUMCOMPUTEUNITS];
	#pragma HLS ARRAY_PARTITION variable=rtravstate complete
	
	keyvalue16_type vbuffer0_1[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer0_1
	uintNUMPby2_type vmask0[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask0
	uint32_type vmask_p0[BLOCKRAM_SIZE];
	keyvalue16_type vbuffer1_1[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer1_1
	uintNUMPby2_type vmask1[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask1
	uint32_type vmask_p1[BLOCKRAM_SIZE];
	keyvalue16_type vbuffer2_1[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer2_1
	uintNUMPby2_type vmask2[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask2
	uint32_type vmask_p2[BLOCKRAM_SIZE];
	keyvalue16_type vbuffer3_1[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer3_1
	uintNUMPby2_type vmask3[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask3
	uint32_type vmask_p3[BLOCKRAM_SIZE];
	keyvalue16_type vbuffer4_1[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer4_1
	uintNUMPby2_type vmask4[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask4
	uint32_type vmask_p4[BLOCKRAM_SIZE];
	keyvalue16_type vbuffer5_1[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer5_1
	uintNUMPby2_type vmask5[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask5
	uint32_type vmask_p5[BLOCKRAM_SIZE];
	keyvalue16_type vbuffer6_1[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer6_1
	uintNUMPby2_type vmask6[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask6
	uint32_type vmask_p6[BLOCKRAM_SIZE];
	keyvalue16_type vbuffer7_1[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer7_1
	uintNUMPby2_type vmask7[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask7
	uint32_type vmask_p7[BLOCKRAM_SIZE];
	keyvalue16_type vbuffer8_1[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer8_1
	uintNUMPby2_type vmask8[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask8
	uint32_type vmask_p8[BLOCKRAM_SIZE];
	keyvalue16_type vbuffer9_1[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer9_1
	uintNUMPby2_type vmask9[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask9
	uint32_type vmask_p9[BLOCKRAM_SIZE];
	keyvalue16_type vbuffer10_1[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer10_1
	uintNUMPby2_type vmask10[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask10
	uint32_type vmask_p10[BLOCKRAM_SIZE];
	keyvalue16_type vbuffer11_1[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer11_1
	uintNUMPby2_type vmask11[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask11
	uint32_type vmask_p11[BLOCKRAM_SIZE];
	keyvalue16_type vbuffer12_1[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer12_1
	uintNUMPby2_type vmask12[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask12
	uint32_type vmask_p12[BLOCKRAM_SIZE];
	keyvalue16_type vbuffer13_1[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer13_1
	uintNUMPby2_type vmask13[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask13
	uint32_type vmask_p13[BLOCKRAM_SIZE];
	keyvalue16_type vbuffer14_1[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer14_1
	uintNUMPby2_type vmask14[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask14
	uint32_type vmask_p14[BLOCKRAM_SIZE];
	keyvalue16_type vbuffer15_1[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer15_1
	uintNUMPby2_type vmask15[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask15
	uint32_type vmask_p15[BLOCKRAM_SIZE];
	
	globalparams_t globalparams[NUMCOMPUTEUNITS];
	#pragma HLS ARRAY_PARTITION variable=globalparams complete
	uint32_type vmask_p[BLOCKRAM_SIZE];
	
	analysis_type analysis_loop1 = 1;
	analysis_type analysis_treedepth = TREE_DEPTH;
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif

	globalparams[0] = getglobalparams(kvdram0);
	globalparams[1] = globalparams[0];
	globalparams[2] = globalparams[0];
	globalparams[3] = globalparams[0];
	globalparams[4] = globalparams[0];
	globalparams[5] = globalparams[0];
	globalparams[6] = globalparams[0];
	globalparams[7] = globalparams[0];
	globalparams[8] = globalparams[0];
	globalparams[9] = globalparams[0];
	globalparams[10] = globalparams[0];
	globalparams[11] = globalparams[0];
	globalparams[12] = globalparams[0];
	globalparams[13] = globalparams[0];
	globalparams[14] = globalparams[0];
	globalparams[15] = globalparams[0];
	globalparams_t _globalparams = globalparams[0];
	unsigned int sourcestatsmarker = 0;
	for(unsigned int k=0; k<_globalparams.ACTSPARAMS_TREEDEPTH-1; k++){ 
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_treedepth avg=analysis_treedepth
		sourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); 
	}
	
	unsigned int vreadoffset = 0;
	unsigned int vmaskreadoffset = 0;
	buffer_type reducebuffersz = _globalparams.SIZE_REDUCE / 2;
	buffer_type vmaskbuffersz_kvs = (_globalparams.SIZE_REDUCE * NUM_PARTITIONS) / 512;
	
	step_type currentLOP = _globalparams.ACTSPARAMS_TREEDEPTH;
	batch_type num_source_partitions = get_num_source_partitions(currentLOP);

	for(unsigned int k=0; k<num_source_partitions; k++){ vmask_p[k] = 0; }
	
	TOPKERNELSYNC_MAINLOOP: for(batch_type source_partition=0; source_partition<num_source_partitions; source_partition+=1){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loop1 avg=analysis_loop1
		#ifdef _DEBUGMODE_KERNELPRINTS
		actsutilityobj->print3("### topkernelsync:: source_partition", "currentLOP", "NAp", source_partition, currentLOP, NAp); 							
		#endif
		
		bool_type enablereduce = ON;
		unsigned int ntravszs = 0;
		rtravstate[0] = gettravstate(ON, kvdram0, globalparams[0], currentLOP, sourcestatsmarker);
		rtravstate[1] = gettravstate(ON, kvdram1, globalparams[1], currentLOP, sourcestatsmarker);
		rtravstate[2] = gettravstate(ON, kvdram2, globalparams[2], currentLOP, sourcestatsmarker);
		rtravstate[3] = gettravstate(ON, kvdram3, globalparams[3], currentLOP, sourcestatsmarker);
		rtravstate[4] = gettravstate(ON, kvdram4, globalparams[4], currentLOP, sourcestatsmarker);
		rtravstate[5] = gettravstate(ON, kvdram5, globalparams[5], currentLOP, sourcestatsmarker);
		rtravstate[6] = gettravstate(ON, kvdram6, globalparams[6], currentLOP, sourcestatsmarker);
		rtravstate[7] = gettravstate(ON, kvdram7, globalparams[7], currentLOP, sourcestatsmarker);
		rtravstate[8] = gettravstate(ON, kvdram8, globalparams[8], currentLOP, sourcestatsmarker);
		rtravstate[9] = gettravstate(ON, kvdram9, globalparams[9], currentLOP, sourcestatsmarker);
		rtravstate[10] = gettravstate(ON, kvdram10, globalparams[10], currentLOP, sourcestatsmarker);
		rtravstate[11] = gettravstate(ON, kvdram11, globalparams[11], currentLOP, sourcestatsmarker);
		rtravstate[12] = gettravstate(ON, kvdram12, globalparams[12], currentLOP, sourcestatsmarker);
		rtravstate[13] = gettravstate(ON, kvdram13, globalparams[13], currentLOP, sourcestatsmarker);
		rtravstate[14] = gettravstate(ON, kvdram14, globalparams[14], currentLOP, sourcestatsmarker);
		rtravstate[15] = gettravstate(ON, kvdram15, globalparams[15], currentLOP, sourcestatsmarker);
		for(unsigned int i = 0; i < NUMCOMPUTEUNITS; i++){ ntravszs += rtravstate[i].size_kvs; }
		if(ntravszs > 0){ enablereduce = ON; } else { enablereduce = OFF; }
		batch_type voffset_kvs = source_partition * reducebuffersz * FETFACTOR;
		
		// read vertices
		readkeyvalues(enablereduce, kvdram0, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer0_1, 0, synvbuffer_head_1, 0, reducebuffersz, _globalparams);
		readkeyvalues(enablereduce, kvdram1, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer1_1, 0,  reducebuffersz, _globalparams);
		readkeyvalues(enablereduce, kvdram2, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer2_1, 0,  reducebuffersz, _globalparams);
		readkeyvalues(enablereduce, kvdram3, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer3_1, 0,  reducebuffersz, _globalparams);
		readkeyvalues(enablereduce, kvdram4, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer4_1, 0,  reducebuffersz, _globalparams);
		readkeyvalues(enablereduce, kvdram5, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer5_1, 0,  reducebuffersz, _globalparams);
		readkeyvalues(enablereduce, kvdram6, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer6_1, 0,  reducebuffersz, _globalparams);
		readkeyvalues(enablereduce, kvdram7, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer7_1, 0,  reducebuffersz, _globalparams);
		readkeyvalues(enablereduce, kvdram8, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer8_1, 0,  reducebuffersz, _globalparams);
		readkeyvalues(enablereduce, kvdram9, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer9_1, 0,  reducebuffersz, _globalparams);
		readkeyvalues(enablereduce, kvdram10, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer10_1, 0,  reducebuffersz, _globalparams);
		readkeyvalues(enablereduce, kvdram11, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer11_1, 0,  reducebuffersz, _globalparams);
		readkeyvalues(enablereduce, kvdram12, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer12_1, 0,  reducebuffersz, _globalparams);
		readkeyvalues(enablereduce, kvdram13, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer13_1, 0,  reducebuffersz, _globalparams);
		readkeyvalues(enablereduce, kvdram14, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer14_1, 0,  reducebuffersz, _globalparams);
		readkeyvalues(enablereduce, kvdram15, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer15_1, 0,  reducebuffersz, _globalparams);
		#ifndef ALLVERTEXISACTIVE_ALGORITHM
		resetvmask(vmask0);
		resetvmask(vmask1);
		resetvmask(vmask2);
		resetvmask(vmask3);
		resetvmask(vmask4);
		resetvmask(vmask5);
		resetvmask(vmask6);
		resetvmask(vmask7);
		resetvmask(vmask8);
		resetvmask(vmask9);
		resetvmask(vmask10);
		resetvmask(vmask11);
		resetvmask(vmask12);
		resetvmask(vmask13);
		resetvmask(vmask14);
		resetvmask(vmask15);
		#endif
		
		// synchronize
		synchronize(enablereduce, synvbuffer_head_1, vbuffer0_1,vbuffer1_1,vbuffer2_1,vbuffer3_1,vbuffer4_1,vbuffer5_1,vbuffer6_1,vbuffer7_1,vbuffer8_1,vbuffer9_1,vbuffer10_1,vbuffer11_1,vbuffer12_1,vbuffer13_1,vbuffer14_1,vbuffer15_1, synvbuffer_tail_1, _globalparams);
		vmask_p[source_partition] = apply(enablereduce, synvbuffer_tail_1, synvbuffer_head_1, vmask0, voffset_kvs, _globalparams);
		spreadvdata(enablereduce, synvbuffer_tail_1, vbuffer15_1,vbuffer14_1,vbuffer13_1,vbuffer12_1,vbuffer11_1,vbuffer10_1,vbuffer9_1,vbuffer8_1,vbuffer7_1,vbuffer6_1,vbuffer5_1,vbuffer4_1,vbuffer3_1,vbuffer2_1,vbuffer1_1,vbuffer0_1, synvbuffer_head_1, _globalparams);  // CRITICAL FIXME. vbuffer_1s should be reversed in order to help sdaccel linker
		#ifndef ALLVERTEXISACTIVE_ALGORITHM
		spreadvmask(enablereduce, vmask0, vmask0,vmask1,vmask2,vmask3,vmask4,vmask5,vmask6,vmask7,vmask8,vmask9,vmask10,vmask11,vmask12,vmask13,vmask14,vmask15, _globalparams);
		#endif
		
		// writeback vertices
		savekeyvalues(enablereduce, kvdram0, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer0_1, 0, reducebuffersz, _globalparams);
		savekeyvalues(enablereduce, kvdram1, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer1_1, 0, reducebuffersz, _globalparams);
		savekeyvalues(enablereduce, kvdram2, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer2_1, 0, reducebuffersz, _globalparams);
		savekeyvalues(enablereduce, kvdram3, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer3_1, 0, reducebuffersz, _globalparams);
		savekeyvalues(enablereduce, kvdram4, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer4_1, 0, reducebuffersz, _globalparams);
		savekeyvalues(enablereduce, kvdram5, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer5_1, 0, reducebuffersz, _globalparams);
		savekeyvalues(enablereduce, kvdram6, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer6_1, 0, reducebuffersz, _globalparams);
		savekeyvalues(enablereduce, kvdram7, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer7_1, 0, reducebuffersz, _globalparams);
		savekeyvalues(enablereduce, kvdram8, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer8_1, 0, reducebuffersz, _globalparams);
		savekeyvalues(enablereduce, kvdram9, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer9_1, 0, reducebuffersz, _globalparams);
		savekeyvalues(enablereduce, kvdram10, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer10_1, 0, reducebuffersz, _globalparams);
		savekeyvalues(enablereduce, kvdram11, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer11_1, 0, reducebuffersz, _globalparams);
		savekeyvalues(enablereduce, kvdram12, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer12_1, 0, reducebuffersz, _globalparams);
		savekeyvalues(enablereduce, kvdram13, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer13_1, 0, reducebuffersz, _globalparams);
		savekeyvalues(enablereduce, kvdram14, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer14_1, 0, reducebuffersz, _globalparams);
		savekeyvalues(enablereduce, kvdram15, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset, vbuffer15_1, 0, reducebuffersz, _globalparams);
		#ifndef ALLVERTEXISACTIVE_ALGORITHM
		savevmasks(enablereduce, kvdram0, vmask0, vbuffer0_1, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset, vmaskbuffersz_kvs, _globalparams);
		savevmasks(enablereduce, kvdram1, vmask1, vbuffer1_1, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset, vmaskbuffersz_kvs, _globalparams);
		savevmasks(enablereduce, kvdram2, vmask2, vbuffer2_1, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset, vmaskbuffersz_kvs, _globalparams);
		savevmasks(enablereduce, kvdram3, vmask3, vbuffer3_1, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset, vmaskbuffersz_kvs, _globalparams);
		savevmasks(enablereduce, kvdram4, vmask4, vbuffer4_1, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset, vmaskbuffersz_kvs, _globalparams);
		savevmasks(enablereduce, kvdram5, vmask5, vbuffer5_1, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset, vmaskbuffersz_kvs, _globalparams);
		savevmasks(enablereduce, kvdram6, vmask6, vbuffer6_1, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset, vmaskbuffersz_kvs, _globalparams);
		savevmasks(enablereduce, kvdram7, vmask7, vbuffer7_1, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset, vmaskbuffersz_kvs, _globalparams);
		savevmasks(enablereduce, kvdram8, vmask8, vbuffer8_1, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset, vmaskbuffersz_kvs, _globalparams);
		savevmasks(enablereduce, kvdram9, vmask9, vbuffer9_1, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset, vmaskbuffersz_kvs, _globalparams);
		savevmasks(enablereduce, kvdram10, vmask10, vbuffer10_1, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset, vmaskbuffersz_kvs, _globalparams);
		savevmasks(enablereduce, kvdram11, vmask11, vbuffer11_1, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset, vmaskbuffersz_kvs, _globalparams);
		savevmasks(enablereduce, kvdram12, vmask12, vbuffer12_1, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset, vmaskbuffersz_kvs, _globalparams);
		savevmasks(enablereduce, kvdram13, vmask13, vbuffer13_1, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset, vmaskbuffersz_kvs, _globalparams);
		savevmasks(enablereduce, kvdram14, vmask14, vbuffer14_1, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset, vmaskbuffersz_kvs, _globalparams);
		savevmasks(enablereduce, kvdram15, vmask15, vbuffer15_1, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset, vmaskbuffersz_kvs, _globalparams);
		#endif 
		
		sourcestatsmarker += 1;
		vreadoffset += reducebuffersz * 1; // 2;
		vmaskreadoffset += vmaskbuffersz_kvs;
	}
	return;
}
}
#endif 





