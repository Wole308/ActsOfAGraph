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
	res.range(1, 0) = vmask[index].data[0].key;
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
	globalparams.baseoffset_kvdram_kvs = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BASEOFFSET_KVDRAM_KVS].range(31, 0);
	globalparams.baseoffset_kvdramworkspace_kvs = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BASEOFFSET_KVDRAMWORKSPACE_KVS].range(31, 0);
	globalparams.baseoffset_statsdram_kvs = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BASEOFFSET_STATSDRAM_KVS].range(31, 0);
	globalparams.baseoffset_edgesdata_kvs = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BASEOFFSET_EDGESDATA_KVS].range(31, 0);
	globalparams.baseoffset_vertexptr_kvs = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BASEOFFSET_VERTEXPTR_KVS].range(31, 0);
	globalparams.baseoffset_verticesdata_kvs = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BASEOFFSET_VERTICESDATA_KVS].range(31, 0);
	globalparams.baseoffset_activevertices_kvs = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BASEOFFSET_ACTIVEVERTICES_KVS].range(31, 0);
	globalparams.baseoffset_verticesdatamask_kvs = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BASEOFFSET_VERTICESDATAMASK_KVS].range(31, 0);
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
	globalparams.baseoffset_kvdram_kvs = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BASEOFFSET_KVDRAM_KVS].data[0].key;
	globalparams.baseoffset_kvdramworkspace_kvs = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BASEOFFSET_KVDRAMWORKSPACE_KVS].data[0].key;
	globalparams.baseoffset_statsdram_kvs = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BASEOFFSET_STATSDRAM_KVS].data[0].key;
	globalparams.baseoffset_edgesdata_kvs = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BASEOFFSET_EDGESDATA_KVS].data[0].key;
	globalparams.baseoffset_vertexptr_kvs = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BASEOFFSET_VERTEXPTR_KVS].data[0].key;
	globalparams.baseoffset_verticesdata_kvs = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BASEOFFSET_VERTICESDATA_KVS].data[0].key;
	globalparams.baseoffset_activevertices_kvs = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BASEOFFSET_ACTIVEVERTICES_KVS].data[0].key;
	globalparams.baseoffset_verticesdatamask_kvs = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BASEOFFSET_VERTICESDATAMASK_KVS].data[0].key;
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

	if(currentLOP == 0 || (currentLOP % 2) == 1){ sweepparams.worksourcebaseaddress_kvs = globalparams.baseoffset_kvdram_kvs; sweepparams.workdestbaseaddress_kvs = globalparams.baseoffset_kvdramworkspace_kvs; }
	else { sweepparams.worksourcebaseaddress_kvs = globalparams.baseoffset_kvdramworkspace_kvs; sweepparams.workdestbaseaddress_kvs = globalparams.baseoffset_kvdram_kvs; }

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
travstate_t 
	#ifdef SW 
	acts::
	#endif 
gettravstate(bool_type enable, uint512_dt * kvdram, globalparams_t globalparams, step_type currentLOP, batch_type sourcestatsmarker){			
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
		#else
		keyvalue = kvdram[globalparams.baseoffset_statsdram_kvs + sourcestatsmarker].data[0]; 
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
resetkeysandvalues(keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], buffer_type size, unsigned int resetval){
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
	for(buffer_type k=0; k<BLOCKRAM_SIZE; k++){ 
		for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS UNROLL
			vmask[k].data[p].key = 0;
			vmask[k].data[p].value = 0;
		}
	}
	return;
}

// functions (accessors)
void 
	#ifdef SW 
	acts::
	#endif 
readkeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], batch_type offset_kvs, batch_type size_kvs, travstate_t travstate){
	#pragma HLS function_instantiate variable=kvdram 
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
savekeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_t buffer[8][PADDEDDESTBUFFER_SIZE], keyvalue_t * globalcapsule, keyvalue_t localcapsule[NUM_PARTITIONS], batch_type globalbaseaddress_kvs, globalparams_t globalparams){				
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
readkeyvalues(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], batch_type bufferoffset_kvs, buffer_type size_kvs){
	#pragma HLS function_instantiate variable=kvdram
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
	if(dramoffset_kvs >= BASEOFFSET_VERTEXPTR_KVS && dramoffset_kvs < BASEOFFSET_VERTEXPTR_KVS + VERTEXPTRSSZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"readkeyvalues:: vertices read: offset: "<<(dramoffset_kvs - BASEOFFSET_VERTEXPTR_KVS) * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs - BASEOFFSET_VERTEXPTR_KVS) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	if(dramoffset_kvs >= BASEOFFSET_VERTICESDATA_KVS && dramoffset_kvs < BASEOFFSET_VERTICESDATA_KVS + VERTICESDATASZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"readkeyvalues:: vertices read: offset: "<<(dramoffset_kvs - BASEOFFSET_VERTICESDATA_KVS) * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs - BASEOFFSET_VERTICESDATA_KVS) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	#endif
	return;
}

void 
	#ifdef SW 
	acts::
	#endif 
savekeyvalues(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], batch_type bufferoffset_kvs, buffer_type size_kvs){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount =  (APPLYVERTEXBUFFERSZ / VDATAPACKINGFACTOR) / 2;
	
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
	if(dramoffset_kvs >= BASEOFFSET_VERTEXPTR_KVS && dramoffset_kvs < BASEOFFSET_VERTEXPTR_KVS + VERTEXPTRSSZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"savekeyvalues:: vertices saved: offset: "<<dramoffset_kvs-BASEOFFSET_VERTEXPTR_KVS * VECTOR_SIZE<<"-"<<((dramoffset_kvs + size_kvs)-BASEOFFSET_VERTEXPTR_KVS) * VECTOR_SIZE<<", number of vertex datas written: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	if(dramoffset_kvs >= BASEOFFSET_VERTICESDATA_KVS && dramoffset_kvs < BASEOFFSET_VERTICESDATA_KVS + VERTICESDATASZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"savekeyvalues:: vertices saved: offset: "<<(dramoffset_kvs-BASEOFFSET_VERTICESDATA_KVS) * VECTOR_SIZE<<"-"<<((dramoffset_kvs-BASEOFFSET_VERTICESDATA_KVS) + size_kvs) * VECTOR_SIZE<<", number of vertex datas written: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	#endif
	return;
}

void 
	#ifdef SW 
	acts::
	#endif 
readkeyvalues(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_t buffer[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], batch_type bufferoffset_kvs, buffer_type size_kvs){
	#pragma HLS function_instantiate variable=kvdram
	if(enable == OFF){ return; }
		
	READKEYVALUES3_LOOP: for (buffer_type i=0; i<size_kvs; i++){
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
	if(dramoffset_kvs >= BASEOFFSET_VERTEXPTR_KVS && dramoffset_kvs < BASEOFFSET_VERTEXPTR_KVS + VERTEXPTRSSZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"readkeyvalues:: vertices read: offset: "<<(dramoffset_kvs - BASEOFFSET_VERTEXPTR_KVS) * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs - BASEOFFSET_VERTEXPTR_KVS) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	if(dramoffset_kvs >= BASEOFFSET_VERTICESDATA_KVS && dramoffset_kvs < BASEOFFSET_VERTICESDATA_KVS + VERTICESDATASZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"readkeyvalues:: vertices read: offset: "<<(dramoffset_kvs - BASEOFFSET_VERTICESDATA_KVS) * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs - BASEOFFSET_VERTICESDATA_KVS) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	#endif
	return;
}

void 
	#ifdef SW 
	acts::
	#endif 
savekeyvalues(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_t buffer[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], batch_type bufferoffset_kvs, buffer_type size_kvs){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount =  (APPLYVERTEXBUFFERSZ / VDATAPACKINGFACTOR) / 2;
	
	SAVEKEYVALUES3_LOOP: for (buffer_type i=0; i<size_kvs; i++){
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
	if(dramoffset_kvs >= BASEOFFSET_VERTEXPTR_KVS && dramoffset_kvs < BASEOFFSET_VERTEXPTR_KVS + VERTEXPTRSSZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"savekeyvalues:: vertices saved: offset: "<<dramoffset_kvs-BASEOFFSET_VERTEXPTR_KVS * VECTOR_SIZE<<"-"<<((dramoffset_kvs + size_kvs)-BASEOFFSET_VERTEXPTR_KVS) * VECTOR_SIZE<<", number of vertex datas written: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	if(dramoffset_kvs >= BASEOFFSET_VERTICESDATA_KVS && dramoffset_kvs < BASEOFFSET_VERTICESDATA_KVS + VERTICESDATASZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"savekeyvalues:: vertices saved: offset: "<<(dramoffset_kvs-BASEOFFSET_VERTICESDATA_KVS) * VECTOR_SIZE<<"-"<<((dramoffset_kvs-BASEOFFSET_VERTICESDATA_KVS) + size_kvs) * VECTOR_SIZE<<", number of vertex datas written: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	#endif
	return;
}

void 
	#ifdef SW 
	acts::
	#endif 
savekeyvalues(bool_type enable, 
		uint512_dt * kvdram0, uint512_dt * kvdram1, uint512_dt * kvdram2, uint512_dt * kvdram3, 
		batch_type dramoffset_kvs, keyvalue_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], batch_type bufferoffset_kvs, buffer_type size_kvs){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount =  (APPLYVERTEXBUFFERSZ / VDATAPACKINGFACTOR) / 2;
	
	SAVEKEYVALUES4_LOOP: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		kvdram0[dramoffset_kvs + i].range(31, 0) = buffer[0][bufferoffset_kvs + i].key; 
		kvdram0[dramoffset_kvs + i].range(63, 32) = buffer[0][bufferoffset_kvs + i].value; 
		kvdram0[dramoffset_kvs + i].range(95, 64) = buffer[1][bufferoffset_kvs + i].key; 
		kvdram0[dramoffset_kvs + i].range(127, 96) = buffer[1][bufferoffset_kvs + i].value; 
		kvdram0[dramoffset_kvs + i].range(159, 128) = buffer[2][bufferoffset_kvs + i].key; 
		kvdram0[dramoffset_kvs + i].range(191, 160) = buffer[2][bufferoffset_kvs + i].value; 
		kvdram0[dramoffset_kvs + i].range(223, 192) = buffer[3][bufferoffset_kvs + i].key; 
		kvdram0[dramoffset_kvs + i].range(255, 224) = buffer[3][bufferoffset_kvs + i].value; 
		kvdram0[dramoffset_kvs + i].range(287, 256) = buffer[4][bufferoffset_kvs + i].key; 
		kvdram0[dramoffset_kvs + i].range(319, 288) = buffer[4][bufferoffset_kvs + i].value; 
		kvdram0[dramoffset_kvs + i].range(351, 320) = buffer[5][bufferoffset_kvs + i].key; 
		kvdram0[dramoffset_kvs + i].range(383, 352) = buffer[5][bufferoffset_kvs + i].value; 
		kvdram0[dramoffset_kvs + i].range(415, 384) = buffer[6][bufferoffset_kvs + i].key; 
		kvdram0[dramoffset_kvs + i].range(447, 416) = buffer[6][bufferoffset_kvs + i].value; 
		kvdram0[dramoffset_kvs + i].range(479, 448) = buffer[7][bufferoffset_kvs + i].key; 
		kvdram0[dramoffset_kvs + i].range(511, 480) = buffer[7][bufferoffset_kvs + i].value; 
		kvdram1[dramoffset_kvs + i].range(31, 0) = buffer[0][bufferoffset_kvs + i].key; 
		kvdram1[dramoffset_kvs + i].range(63, 32) = buffer[0][bufferoffset_kvs + i].value; 
		kvdram1[dramoffset_kvs + i].range(95, 64) = buffer[1][bufferoffset_kvs + i].key; 
		kvdram1[dramoffset_kvs + i].range(127, 96) = buffer[1][bufferoffset_kvs + i].value; 
		kvdram1[dramoffset_kvs + i].range(159, 128) = buffer[2][bufferoffset_kvs + i].key; 
		kvdram1[dramoffset_kvs + i].range(191, 160) = buffer[2][bufferoffset_kvs + i].value; 
		kvdram1[dramoffset_kvs + i].range(223, 192) = buffer[3][bufferoffset_kvs + i].key; 
		kvdram1[dramoffset_kvs + i].range(255, 224) = buffer[3][bufferoffset_kvs + i].value; 
		kvdram1[dramoffset_kvs + i].range(287, 256) = buffer[4][bufferoffset_kvs + i].key; 
		kvdram1[dramoffset_kvs + i].range(319, 288) = buffer[4][bufferoffset_kvs + i].value; 
		kvdram1[dramoffset_kvs + i].range(351, 320) = buffer[5][bufferoffset_kvs + i].key; 
		kvdram1[dramoffset_kvs + i].range(383, 352) = buffer[5][bufferoffset_kvs + i].value; 
		kvdram1[dramoffset_kvs + i].range(415, 384) = buffer[6][bufferoffset_kvs + i].key; 
		kvdram1[dramoffset_kvs + i].range(447, 416) = buffer[6][bufferoffset_kvs + i].value; 
		kvdram1[dramoffset_kvs + i].range(479, 448) = buffer[7][bufferoffset_kvs + i].key; 
		kvdram1[dramoffset_kvs + i].range(511, 480) = buffer[7][bufferoffset_kvs + i].value; 
		kvdram2[dramoffset_kvs + i].range(31, 0) = buffer[0][bufferoffset_kvs + i].key; 
		kvdram2[dramoffset_kvs + i].range(63, 32) = buffer[0][bufferoffset_kvs + i].value; 
		kvdram2[dramoffset_kvs + i].range(95, 64) = buffer[1][bufferoffset_kvs + i].key; 
		kvdram2[dramoffset_kvs + i].range(127, 96) = buffer[1][bufferoffset_kvs + i].value; 
		kvdram2[dramoffset_kvs + i].range(159, 128) = buffer[2][bufferoffset_kvs + i].key; 
		kvdram2[dramoffset_kvs + i].range(191, 160) = buffer[2][bufferoffset_kvs + i].value; 
		kvdram2[dramoffset_kvs + i].range(223, 192) = buffer[3][bufferoffset_kvs + i].key; 
		kvdram2[dramoffset_kvs + i].range(255, 224) = buffer[3][bufferoffset_kvs + i].value; 
		kvdram2[dramoffset_kvs + i].range(287, 256) = buffer[4][bufferoffset_kvs + i].key; 
		kvdram2[dramoffset_kvs + i].range(319, 288) = buffer[4][bufferoffset_kvs + i].value; 
		kvdram2[dramoffset_kvs + i].range(351, 320) = buffer[5][bufferoffset_kvs + i].key; 
		kvdram2[dramoffset_kvs + i].range(383, 352) = buffer[5][bufferoffset_kvs + i].value; 
		kvdram2[dramoffset_kvs + i].range(415, 384) = buffer[6][bufferoffset_kvs + i].key; 
		kvdram2[dramoffset_kvs + i].range(447, 416) = buffer[6][bufferoffset_kvs + i].value; 
		kvdram2[dramoffset_kvs + i].range(479, 448) = buffer[7][bufferoffset_kvs + i].key; 
		kvdram2[dramoffset_kvs + i].range(511, 480) = buffer[7][bufferoffset_kvs + i].value; 
		kvdram3[dramoffset_kvs + i].range(31, 0) = buffer[0][bufferoffset_kvs + i].key; 
		kvdram3[dramoffset_kvs + i].range(63, 32) = buffer[0][bufferoffset_kvs + i].value; 
		kvdram3[dramoffset_kvs + i].range(95, 64) = buffer[1][bufferoffset_kvs + i].key; 
		kvdram3[dramoffset_kvs + i].range(127, 96) = buffer[1][bufferoffset_kvs + i].value; 
		kvdram3[dramoffset_kvs + i].range(159, 128) = buffer[2][bufferoffset_kvs + i].key; 
		kvdram3[dramoffset_kvs + i].range(191, 160) = buffer[2][bufferoffset_kvs + i].value; 
		kvdram3[dramoffset_kvs + i].range(223, 192) = buffer[3][bufferoffset_kvs + i].key; 
		kvdram3[dramoffset_kvs + i].range(255, 224) = buffer[3][bufferoffset_kvs + i].value; 
		kvdram3[dramoffset_kvs + i].range(287, 256) = buffer[4][bufferoffset_kvs + i].key; 
		kvdram3[dramoffset_kvs + i].range(319, 288) = buffer[4][bufferoffset_kvs + i].value; 
		kvdram3[dramoffset_kvs + i].range(351, 320) = buffer[5][bufferoffset_kvs + i].key; 
		kvdram3[dramoffset_kvs + i].range(383, 352) = buffer[5][bufferoffset_kvs + i].value; 
		kvdram3[dramoffset_kvs + i].range(415, 384) = buffer[6][bufferoffset_kvs + i].key; 
		kvdram3[dramoffset_kvs + i].range(447, 416) = buffer[6][bufferoffset_kvs + i].value; 
		kvdram3[dramoffset_kvs + i].range(479, 448) = buffer[7][bufferoffset_kvs + i].key; 
		kvdram3[dramoffset_kvs + i].range(511, 480) = buffer[7][bufferoffset_kvs + i].value; 
		#else 
		kvdram0[dramoffset_kvs + i].data[0] = buffer[0][bufferoffset_kvs + i];
		kvdram0[dramoffset_kvs + i].data[1] = buffer[1][bufferoffset_kvs + i];
		kvdram0[dramoffset_kvs + i].data[2] = buffer[2][bufferoffset_kvs + i];
		kvdram0[dramoffset_kvs + i].data[3] = buffer[3][bufferoffset_kvs + i];
		kvdram0[dramoffset_kvs + i].data[4] = buffer[4][bufferoffset_kvs + i];
		kvdram0[dramoffset_kvs + i].data[5] = buffer[5][bufferoffset_kvs + i];
		kvdram0[dramoffset_kvs + i].data[6] = buffer[6][bufferoffset_kvs + i];
		kvdram0[dramoffset_kvs + i].data[7] = buffer[7][bufferoffset_kvs + i];
		kvdram1[dramoffset_kvs + i].data[0] = buffer[0][bufferoffset_kvs + i];
		kvdram1[dramoffset_kvs + i].data[1] = buffer[1][bufferoffset_kvs + i];
		kvdram1[dramoffset_kvs + i].data[2] = buffer[2][bufferoffset_kvs + i];
		kvdram1[dramoffset_kvs + i].data[3] = buffer[3][bufferoffset_kvs + i];
		kvdram1[dramoffset_kvs + i].data[4] = buffer[4][bufferoffset_kvs + i];
		kvdram1[dramoffset_kvs + i].data[5] = buffer[5][bufferoffset_kvs + i];
		kvdram1[dramoffset_kvs + i].data[6] = buffer[6][bufferoffset_kvs + i];
		kvdram1[dramoffset_kvs + i].data[7] = buffer[7][bufferoffset_kvs + i];
		kvdram2[dramoffset_kvs + i].data[0] = buffer[0][bufferoffset_kvs + i];
		kvdram2[dramoffset_kvs + i].data[1] = buffer[1][bufferoffset_kvs + i];
		kvdram2[dramoffset_kvs + i].data[2] = buffer[2][bufferoffset_kvs + i];
		kvdram2[dramoffset_kvs + i].data[3] = buffer[3][bufferoffset_kvs + i];
		kvdram2[dramoffset_kvs + i].data[4] = buffer[4][bufferoffset_kvs + i];
		kvdram2[dramoffset_kvs + i].data[5] = buffer[5][bufferoffset_kvs + i];
		kvdram2[dramoffset_kvs + i].data[6] = buffer[6][bufferoffset_kvs + i];
		kvdram2[dramoffset_kvs + i].data[7] = buffer[7][bufferoffset_kvs + i];
		kvdram3[dramoffset_kvs + i].data[0] = buffer[0][bufferoffset_kvs + i];
		kvdram3[dramoffset_kvs + i].data[1] = buffer[1][bufferoffset_kvs + i];
		kvdram3[dramoffset_kvs + i].data[2] = buffer[2][bufferoffset_kvs + i];
		kvdram3[dramoffset_kvs + i].data[3] = buffer[3][bufferoffset_kvs + i];
		kvdram3[dramoffset_kvs + i].data[4] = buffer[4][bufferoffset_kvs + i];
		kvdram3[dramoffset_kvs + i].data[5] = buffer[5][bufferoffset_kvs + i];
		kvdram3[dramoffset_kvs + i].data[6] = buffer[6][bufferoffset_kvs + i];
		kvdram3[dramoffset_kvs + i].data[7] = buffer[7][bufferoffset_kvs + i];
		#endif 
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvswritten(VECTOR_SIZE*4);
		#endif
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"savekeyvalues:: vertices saved: offset: "<<dramoffset_kvs * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs) * VECTOR_SIZE<<", number of vertex datas written: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<<endl;
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS2
	if(dramoffset_kvs >= BASEOFFSET_VERTEXPTR_KVS && dramoffset_kvs < BASEOFFSET_VERTEXPTR_KVS + VERTEXPTRSSZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"savekeyvalues:: vertices saved: offset: "<<dramoffset_kvs-BASEOFFSET_VERTEXPTR_KVS * VECTOR_SIZE<<"-"<<((dramoffset_kvs + size_kvs)-BASEOFFSET_VERTEXPTR_KVS) * VECTOR_SIZE<<", number of vertex datas written: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	if(dramoffset_kvs >= BASEOFFSET_VERTICESDATA_KVS && dramoffset_kvs < BASEOFFSET_VERTICESDATA_KVS + VERTICESDATASZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"savekeyvalues:: vertices saved: offset: "<<(dramoffset_kvs-BASEOFFSET_VERTICESDATA_KVS) * VECTOR_SIZE<<"-"<<((dramoffset_kvs-BASEOFFSET_VERTICESDATA_KVS) + size_kvs) * VECTOR_SIZE<<", number of vertex datas written: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	#endif
	return;
}

void 
	#ifdef SW 
	acts::
	#endif 
loadvdata(bool_type enable, uint512_dt * kvdram, keyvalue_t vdata[NUM_VBUFFERS][BLOCKRAM_SIZE], batch_type offset1_kvs, buffer_type size1_kvs, batch_type offset2_kvs, buffer_type size2_kvs){
	#pragma HLS function_instantiate variable=kvdram
	if(enable == OFF){ return; }
		
	LOADVDATA3_LOOP1: for (buffer_type i=0; i<size1_kvs; i++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		vdata[0][i].key = kvdram[offset1_kvs + i].range(31, 0); 
		vdata[0][i].value = kvdram[offset1_kvs + i].range(63, 32); 
		vdata[1][i].key = kvdram[offset1_kvs + i].range(95, 64); 
		vdata[1][i].value = kvdram[offset1_kvs + i].range(127, 96); 
		vdata[2][i].key = kvdram[offset1_kvs + i].range(159, 128); 
		vdata[2][i].value = kvdram[offset1_kvs + i].range(191, 160); 
		vdata[3][i].key = kvdram[offset1_kvs + i].range(223, 192); 
		vdata[3][i].value = kvdram[offset1_kvs + i].range(255, 224); 
		vdata[4][i].key = kvdram[offset1_kvs + i].range(287, 256); 
		vdata[4][i].value = kvdram[offset1_kvs + i].range(319, 288); 
		vdata[5][i].key = kvdram[offset1_kvs + i].range(351, 320); 
		vdata[5][i].value = kvdram[offset1_kvs + i].range(383, 352); 
		vdata[6][i].key = kvdram[offset1_kvs + i].range(415, 384); 
		vdata[6][i].value = kvdram[offset1_kvs + i].range(447, 416); 
		vdata[7][i].key = kvdram[offset1_kvs + i].range(479, 448); 
		vdata[7][i].value = kvdram[offset1_kvs + i].range(511, 480); 
		#else 
		vdata[0][i] = kvdram[offset1_kvs + i].data[0]; 
		vdata[1][i] = kvdram[offset1_kvs + i].data[1]; 
		vdata[2][i] = kvdram[offset1_kvs + i].data[2]; 
		vdata[3][i] = kvdram[offset1_kvs + i].data[3]; 
		vdata[4][i] = kvdram[offset1_kvs + i].data[4]; 
		vdata[5][i] = kvdram[offset1_kvs + i].data[5]; 
		vdata[6][i] = kvdram[offset1_kvs + i].data[6]; 
		vdata[7][i] = kvdram[offset1_kvs + i].data[7]; 
		#endif 
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	
	LOADVDATA3_LOOP2: for (buffer_type i=0; i<size2_kvs; i++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
	
		vdata[8][i].key = kvdram[offset2_kvs + i].range(31, 0); 
		vdata[8][i].value = kvdram[offset2_kvs + i].range(63, 32); 
	
		vdata[9][i].key = kvdram[offset2_kvs + i].range(95, 64); 
		vdata[9][i].value = kvdram[offset2_kvs + i].range(127, 96); 
	
		vdata[10][i].key = kvdram[offset2_kvs + i].range(159, 128); 
		vdata[10][i].value = kvdram[offset2_kvs + i].range(191, 160); 
	
		vdata[11][i].key = kvdram[offset2_kvs + i].range(223, 192); 
		vdata[11][i].value = kvdram[offset2_kvs + i].range(255, 224); 
	
		vdata[12][i].key = kvdram[offset2_kvs + i].range(287, 256); 
		vdata[12][i].value = kvdram[offset2_kvs + i].range(319, 288); 
	
		vdata[13][i].key = kvdram[offset2_kvs + i].range(351, 320); 
		vdata[13][i].value = kvdram[offset2_kvs + i].range(383, 352); 
	
		vdata[14][i].key = kvdram[offset2_kvs + i].range(415, 384); 
		vdata[14][i].value = kvdram[offset2_kvs + i].range(447, 416); 
	
		vdata[15][i].key = kvdram[offset2_kvs + i].range(479, 448); 
		vdata[15][i].value = kvdram[offset2_kvs + i].range(511, 480); 
		#else 
		vdata[8][i] = kvdram[offset2_kvs + i].data[0]; 
		vdata[9][i] = kvdram[offset2_kvs + i].data[1]; 
		vdata[10][i] = kvdram[offset2_kvs + i].data[2]; 
		vdata[11][i] = kvdram[offset2_kvs + i].data[3]; 
		vdata[12][i] = kvdram[offset2_kvs + i].data[4]; 
		vdata[13][i] = kvdram[offset2_kvs + i].data[5]; 
		vdata[14][i] = kvdram[offset2_kvs + i].data[6]; 
		vdata[15][i] = kvdram[offset2_kvs + i].data[7]; 
		#endif 
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<< TIMINGRESULTSCOLOR<<"loadvdata:: vertices read: offset: "<<offset1_kvs * VECTOR_SIZE<<"-"<<(offset1_kvs + size1_kvs) * VECTOR_SIZE<<", number of vertex datas read: "<<(size1_kvs * VECTOR_SIZE * 2)<<" ("<<size1_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl;
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS2
	if(offset1_kvs >= BASEOFFSET_VERTEXPTR_KVS && offset1_kvs < BASEOFFSET_VERTEXPTR_KVS + VERTEXPTRSSZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"loadvdata:: vertices read: offset: "<<(offset1_kvs - BASEOFFSET_VERTEXPTR_KVS) * VECTOR_SIZE<<"-"<<(offset1_kvs + size1_kvs - BASEOFFSET_VERTEXPTR_KVS) * VECTOR_SIZE<<", number of vertex datas read: "<<(size1_kvs * VECTOR_SIZE * 2)<<" ("<<size1_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	if(offset1_kvs >= BASEOFFSET_VERTICESDATA_KVS && offset1_kvs < BASEOFFSET_VERTICESDATA_KVS + VERTICESDATASZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"loadvdata:: vertices read: offset: "<<(offset1_kvs - BASEOFFSET_VERTICESDATA_KVS) * VECTOR_SIZE<<"-"<<(offset1_kvs + size1_kvs - BASEOFFSET_VERTICESDATA_KVS) * VECTOR_SIZE<<", number of vertex datas read: "<<(size1_kvs * VECTOR_SIZE * 2)<<" ("<<size1_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	#endif
	return;
}

void 
	#ifdef SW 
	acts::
	#endif 
savevdata(bool_type enable, uint512_dt * kvdram, keyvalue_t vdata[NUM_VBUFFERS][BLOCKRAM_SIZE], batch_type offset1_kvs, buffer_type size1_kvs, batch_type offset2_kvs, buffer_type size2_kvs){
	#pragma HLS function_instantiate variable=kvdram
	if(enable == OFF){ return; }
		
	SAVEVDATA_LOOP1: for (buffer_type i=0; i<size1_kvs; i++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		kvdram[offset1_kvs + i].range(31, 0) = vdata[0][i].key; 
		kvdram[offset1_kvs + i].range(63, 32) = vdata[0][i].value; 
		kvdram[offset1_kvs + i].range(95, 64) = vdata[1][i].key; 
		kvdram[offset1_kvs + i].range(127, 96) = vdata[1][i].value; 
		kvdram[offset1_kvs + i].range(159, 128) = vdata[2][i].key; 
		kvdram[offset1_kvs + i].range(191, 160) = vdata[2][i].value; 
		kvdram[offset1_kvs + i].range(223, 192) = vdata[3][i].key; 
		kvdram[offset1_kvs + i].range(255, 224) = vdata[3][i].value; 
		kvdram[offset1_kvs + i].range(287, 256) = vdata[4][i].key; 
		kvdram[offset1_kvs + i].range(319, 288) = vdata[4][i].value; 
		kvdram[offset1_kvs + i].range(351, 320) = vdata[5][i].key; 
		kvdram[offset1_kvs + i].range(383, 352) = vdata[5][i].value; 
		kvdram[offset1_kvs + i].range(415, 384) = vdata[6][i].key; 
		kvdram[offset1_kvs + i].range(447, 416) = vdata[6][i].value; 
		kvdram[offset1_kvs + i].range(479, 448) = vdata[7][i].key; 
		kvdram[offset1_kvs + i].range(511, 480) = vdata[7][i].value; 
		#else 
		kvdram[offset1_kvs + i].data[0] = vdata[0][i]; 
		kvdram[offset1_kvs + i].data[1] = vdata[1][i]; 
		kvdram[offset1_kvs + i].data[2] = vdata[2][i]; 
		kvdram[offset1_kvs + i].data[3] = vdata[3][i]; 
		kvdram[offset1_kvs + i].data[4] = vdata[4][i]; 
		kvdram[offset1_kvs + i].data[5] = vdata[5][i]; 
		kvdram[offset1_kvs + i].data[6] = vdata[6][i]; 
		kvdram[offset1_kvs + i].data[7] = vdata[7][i]; 
		#endif 
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvswritten(VECTOR_SIZE);
		#endif
	}
	
	SAVEVDATA3_LOOP2: for (buffer_type i=0; i<size2_kvs; i++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
	
		kvdram[offset2_kvs + i].range(31, 0) = vdata[8][i].key; 
		kvdram[offset2_kvs + i].range(63, 32) = vdata[8][i].value; 
	
		kvdram[offset2_kvs + i].range(95, 64) = vdata[9][i].key; 
		kvdram[offset2_kvs + i].range(127, 96) = vdata[9][i].value; 
	
		kvdram[offset2_kvs + i].range(159, 128) = vdata[10][i].key; 
		kvdram[offset2_kvs + i].range(191, 160) = vdata[10][i].value; 
	
		kvdram[offset2_kvs + i].range(223, 192) = vdata[11][i].key; 
		kvdram[offset2_kvs + i].range(255, 224) = vdata[11][i].value; 
	
		kvdram[offset2_kvs + i].range(287, 256) = vdata[12][i].key; 
		kvdram[offset2_kvs + i].range(319, 288) = vdata[12][i].value; 
	
		kvdram[offset2_kvs + i].range(351, 320) = vdata[13][i].key; 
		kvdram[offset2_kvs + i].range(383, 352) = vdata[13][i].value; 
	
		kvdram[offset2_kvs + i].range(415, 384) = vdata[14][i].key; 
		kvdram[offset2_kvs + i].range(447, 416) = vdata[14][i].value; 
	
		kvdram[offset2_kvs + i].range(479, 448) = vdata[15][i].key; 
		kvdram[offset2_kvs + i].range(511, 480) = vdata[15][i].value; 
		#else 
		kvdram[offset2_kvs + i].data[0] = vdata[8][i]; 
		kvdram[offset2_kvs + i].data[1] = vdata[9][i]; 
		kvdram[offset2_kvs + i].data[2] = vdata[10][i]; 
		kvdram[offset2_kvs + i].data[3] = vdata[11][i]; 
		kvdram[offset2_kvs + i].data[4] = vdata[12][i]; 
		kvdram[offset2_kvs + i].data[5] = vdata[13][i]; 
		kvdram[offset2_kvs + i].data[6] = vdata[14][i]; 
		kvdram[offset2_kvs + i].data[7] = vdata[15][i]; 
		#endif 
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvswritten(VECTOR_SIZE);
		#endif
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<< TIMINGRESULTSCOLOR<<"savevdata:: vertices read: offset: "<<offset1_kvs * VECTOR_SIZE<<"-"<<(offset1_kvs + size1_kvs) * VECTOR_SIZE<<", number of vertex datas saved: "<<(size1_kvs * VECTOR_SIZE * 2)<<" ("<<size1_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl;
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS2
	if(offset1_kvs >= BASEOFFSET_VERTEXPTR_KVS && offset1_kvs < BASEOFFSET_VERTEXPTR_KVS + VERTEXPTRSSZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"savevdata:: vertices saved: offset: "<<(offset1_kvs - BASEOFFSET_VERTEXPTR_KVS) * VECTOR_SIZE<<"-"<<(offset1_kvs + size1_kvs - BASEOFFSET_VERTEXPTR_KVS) * VECTOR_SIZE<<", number of vertex datas saved: "<<(size1_kvs * VECTOR_SIZE * 2)<<" ("<<size1_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	if(offset1_kvs >= BASEOFFSET_VERTICESDATA_KVS && offset1_kvs < BASEOFFSET_VERTICESDATA_KVS + VERTICESDATASZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"savevdata:: vertices saved: offset: "<<(offset1_kvs - BASEOFFSET_VERTICESDATA_KVS) * VECTOR_SIZE<<"-"<<(offset1_kvs + size1_kvs - BASEOFFSET_VERTICESDATA_KVS) * VECTOR_SIZE<<", number of vertex datas saved: "<<(size1_kvs * VECTOR_SIZE * 2)<<" ("<<size1_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	#endif
	return;
}

void 
	#ifdef SW 
	acts::
	#endif 
loadvmasks(bool_type enable, uint512_dt * kvdram, uintNUMPby2_type vmask[BLOCKRAM_SIZE], keyvalue_t tempbuffer[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs){
	if(enable == OFF){ return; }
	uint32_type bitsbuffer[BLOCKRAM_SIZE];
	
	readkeyvalues(ON, kvdram, offset_kvs, tempbuffer, BUFFERBASE_VDATA, size_kvs);
	buffer_type index = 0;
	LOADVMASKS_LOOP1: for (buffer_type i=0; i<VMASKBUFFERSZ_KVS; i++){
	#pragma HLS PIPELINE II=8
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("acts::loadvmasks 1", i*16, REDUCEBUFFERSZ, NAp, NAp, NAp);
		actsutilityobj->checkoutofbounds("acts::loadvmasks 1", i, VMASKBUFFERSZ_KVS, NAp, NAp, NAp);
		#endif
	
		bitsbuffer[index + 0] = tempbuffer[0][BUFFERBASE_VDATA + i].key;
		bitsbuffer[index + 0 + 1] = tempbuffer[0][BUFFERBASE_VDATA + i].value;
		bitsbuffer[index + 2] = tempbuffer[1][BUFFERBASE_VDATA + i].key;
		bitsbuffer[index + 2 + 1] = tempbuffer[1][BUFFERBASE_VDATA + i].value;
		bitsbuffer[index + 4] = tempbuffer[2][BUFFERBASE_VDATA + i].key;
		bitsbuffer[index + 4 + 1] = tempbuffer[2][BUFFERBASE_VDATA + i].value;
		bitsbuffer[index + 6] = tempbuffer[3][BUFFERBASE_VDATA + i].key;
		bitsbuffer[index + 6 + 1] = tempbuffer[3][BUFFERBASE_VDATA + i].value;
		bitsbuffer[index + 8] = tempbuffer[4][BUFFERBASE_VDATA + i].key;
		bitsbuffer[index + 8 + 1] = tempbuffer[4][BUFFERBASE_VDATA + i].value;
		bitsbuffer[index + 10] = tempbuffer[5][BUFFERBASE_VDATA + i].key;
		bitsbuffer[index + 10 + 1] = tempbuffer[5][BUFFERBASE_VDATA + i].value;
		bitsbuffer[index + 12] = tempbuffer[6][BUFFERBASE_VDATA + i].key;
		bitsbuffer[index + 12 + 1] = tempbuffer[6][BUFFERBASE_VDATA + i].value;
		bitsbuffer[index + 14] = tempbuffer[7][BUFFERBASE_VDATA + i].key;
		bitsbuffer[index + 14 + 1] = tempbuffer[7][BUFFERBASE_VDATA + i].value;
		
		index += NUM_PARTITIONS;
	}
	
	LOADVMASKS_LOOP2: for (buffer_type i=0; i<REDUCEBUFFERSZ; i++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		vmask[i].data[0].key = bitsbuffer[i].range(1, 0);
		vmask[i].data[0].value = bitsbuffer[i].range(2, 1);
		vmask[i].data[1].key = bitsbuffer[i].range(3, 2);
		vmask[i].data[1].value = bitsbuffer[i].range(4, 3);
		vmask[i].data[2].key = bitsbuffer[i].range(5, 4);
		vmask[i].data[2].value = bitsbuffer[i].range(6, 5);
		vmask[i].data[3].key = bitsbuffer[i].range(7, 6);
		vmask[i].data[3].value = bitsbuffer[i].range(8, 7);
		vmask[i].data[4].key = bitsbuffer[i].range(9, 8);
		vmask[i].data[4].value = bitsbuffer[i].range(10, 9);
		vmask[i].data[5].key = bitsbuffer[i].range(11, 10);
		vmask[i].data[5].value = bitsbuffer[i].range(12, 11);
		vmask[i].data[6].key = bitsbuffer[i].range(13, 12);
		vmask[i].data[6].value = bitsbuffer[i].range(14, 13);
		vmask[i].data[7].key = bitsbuffer[i].range(15, 14);
		vmask[i].data[7].value = bitsbuffer[i].range(16, 15);
		vmask[i].data[8].key = bitsbuffer[i].range(17, 16);
		vmask[i].data[8].value = bitsbuffer[i].range(18, 17);
		vmask[i].data[9].key = bitsbuffer[i].range(19, 18);
		vmask[i].data[9].value = bitsbuffer[i].range(20, 19);
		vmask[i].data[10].key = bitsbuffer[i].range(21, 20);
		vmask[i].data[10].value = bitsbuffer[i].range(22, 21);
		vmask[i].data[11].key = bitsbuffer[i].range(23, 22);
		vmask[i].data[11].value = bitsbuffer[i].range(24, 23);
		vmask[i].data[12].key = bitsbuffer[i].range(25, 24);
		vmask[i].data[12].value = bitsbuffer[i].range(26, 25);
		vmask[i].data[13].key = bitsbuffer[i].range(27, 26);
		vmask[i].data[13].value = bitsbuffer[i].range(28, 27);
		vmask[i].data[14].key = bitsbuffer[i].range(29, 28);
		vmask[i].data[14].value = bitsbuffer[i].range(30, 29);
		vmask[i].data[15].key = bitsbuffer[i].range(31, 30);
		vmask[i].data[15].value = bitsbuffer[i].range(32, 31);
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
savevmasks(bool_type enable, uint512_dt * kvdram, uintNUMPby2_type vmask[BLOCKRAM_SIZE], keyvalue_t tempbuffer[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs){
	if(enable == OFF){ return; }
	uint32_type bitsbuffer[REDUCEBUFFERSZ];

	SAVEVMASKS_LOOP1: for (buffer_type i=0; i<REDUCEBUFFERSZ; i++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		bitsbuffer[i].range(1, 0) = vmask[i].data[0].key;
		bitsbuffer[i].range(2, 1) = vmask[i].data[0].value;
		bitsbuffer[i].range(3, 2) = vmask[i].data[1].key;
		bitsbuffer[i].range(4, 3) = vmask[i].data[1].value;
		bitsbuffer[i].range(5, 4) = vmask[i].data[2].key;
		bitsbuffer[i].range(6, 5) = vmask[i].data[2].value;
		bitsbuffer[i].range(7, 6) = vmask[i].data[3].key;
		bitsbuffer[i].range(8, 7) = vmask[i].data[3].value;
		bitsbuffer[i].range(9, 8) = vmask[i].data[4].key;
		bitsbuffer[i].range(10, 9) = vmask[i].data[4].value;
		bitsbuffer[i].range(11, 10) = vmask[i].data[5].key;
		bitsbuffer[i].range(12, 11) = vmask[i].data[5].value;
		bitsbuffer[i].range(13, 12) = vmask[i].data[6].key;
		bitsbuffer[i].range(14, 13) = vmask[i].data[6].value;
		bitsbuffer[i].range(15, 14) = vmask[i].data[7].key;
		bitsbuffer[i].range(16, 15) = vmask[i].data[7].value;
		bitsbuffer[i].range(17, 16) = vmask[i].data[8].key;
		bitsbuffer[i].range(18, 17) = vmask[i].data[8].value;
		bitsbuffer[i].range(19, 18) = vmask[i].data[9].key;
		bitsbuffer[i].range(20, 19) = vmask[i].data[9].value;
		bitsbuffer[i].range(21, 20) = vmask[i].data[10].key;
		bitsbuffer[i].range(22, 21) = vmask[i].data[10].value;
		bitsbuffer[i].range(23, 22) = vmask[i].data[11].key;
		bitsbuffer[i].range(24, 23) = vmask[i].data[11].value;
		bitsbuffer[i].range(25, 24) = vmask[i].data[12].key;
		bitsbuffer[i].range(26, 25) = vmask[i].data[12].value;
		bitsbuffer[i].range(27, 26) = vmask[i].data[13].key;
		bitsbuffer[i].range(28, 27) = vmask[i].data[13].value;
		bitsbuffer[i].range(29, 28) = vmask[i].data[14].key;
		bitsbuffer[i].range(30, 29) = vmask[i].data[14].value;
		bitsbuffer[i].range(31, 30) = vmask[i].data[15].key;
		bitsbuffer[i].range(32, 31) = vmask[i].data[15].value;
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
	SAVEVMASKS_LOOP2: for (buffer_type i=0; i<VMASKBUFFERSZ_KVS; i++){
	#pragma HLS PIPELINE II=8
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("acts::savevmasks 1", i*16, REDUCEBUFFERSZ, NAp, NAp, NAp);
		actsutilityobj->checkoutofbounds("acts::savevmasks 1", i, VMASKBUFFERSZ_KVS, NAp, NAp, NAp);
		#endif
		
		tempbuffer[0][BUFFERBASE_VDATA + i].key = bitsbuffer[index + 0]; // BUFFERBASE_VDATA
		tempbuffer[0][BUFFERBASE_VDATA + i].value = bitsbuffer[index + 0 + 1]; // BUFFERBASE_VDATA
		tempbuffer[1][BUFFERBASE_VDATA + i].key = bitsbuffer[index + 2]; // BUFFERBASE_VDATA
		tempbuffer[1][BUFFERBASE_VDATA + i].value = bitsbuffer[index + 2 + 1]; // BUFFERBASE_VDATA
		tempbuffer[2][BUFFERBASE_VDATA + i].key = bitsbuffer[index + 4]; // BUFFERBASE_VDATA
		tempbuffer[2][BUFFERBASE_VDATA + i].value = bitsbuffer[index + 4 + 1]; // BUFFERBASE_VDATA
		tempbuffer[3][BUFFERBASE_VDATA + i].key = bitsbuffer[index + 6]; // BUFFERBASE_VDATA
		tempbuffer[3][BUFFERBASE_VDATA + i].value = bitsbuffer[index + 6 + 1]; // BUFFERBASE_VDATA
		tempbuffer[4][BUFFERBASE_VDATA + i].key = bitsbuffer[index + 8]; // BUFFERBASE_VDATA
		tempbuffer[4][BUFFERBASE_VDATA + i].value = bitsbuffer[index + 8 + 1]; // BUFFERBASE_VDATA
		tempbuffer[5][BUFFERBASE_VDATA + i].key = bitsbuffer[index + 10]; // BUFFERBASE_VDATA
		tempbuffer[5][BUFFERBASE_VDATA + i].value = bitsbuffer[index + 10 + 1]; // BUFFERBASE_VDATA
		tempbuffer[6][BUFFERBASE_VDATA + i].key = bitsbuffer[index + 12]; // BUFFERBASE_VDATA
		tempbuffer[6][BUFFERBASE_VDATA + i].value = bitsbuffer[index + 12 + 1]; // BUFFERBASE_VDATA
		tempbuffer[7][BUFFERBASE_VDATA + i].key = bitsbuffer[index + 14]; // BUFFERBASE_VDATA
		tempbuffer[7][BUFFERBASE_VDATA + i].value = bitsbuffer[index + 14 + 1]; // BUFFERBASE_VDATA
		
		index += NUM_PARTITIONS;
		
	}
	savekeyvalues(ON, kvdram, offset_kvs, tempbuffer, BUFFERBASE_VDATA, size_kvs); // BUFFERBASE_VDATA
	return;
}

value_t 
	#ifdef SW 
	acts::
	#endif 
readfromvbuffer(keyvalue_t vbuffer1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], unsigned int loc){
	#pragma HLS INLINE
	value_t data = 0;
	
	unsigned int col = loc / REDUCESZ;
	unsigned int row = loc % REDUCESZ;
	row = row / 2;
	
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("readfromvbuffer.col", col, NUM_PARTITIONS, loc, NAp, NAp); // AUTOMATEME.
	actsutilityobj->checkoutofbounds("readfromvbuffer.row", row, BLOCKRAM_SIZE, loc, NAp, NAp);
	#endif 
	
	if(col < NUM_PARTITIONS/2){
		if(loc % 2 == 0){ data = vbuffer1[col][BUFFERBASE_VDATA + row].key; } 
		else { data = vbuffer1[col][BUFFERBASE_VDATA + row].value; }
	} else {
		if(loc % 2 == 0){ data = vbuffer2[col - (NUM_PARTITIONS/2)][BUFFERBASE_VDATA + row].key; } 
		else { data = vbuffer2[col - (NUM_PARTITIONS/2)][BUFFERBASE_VDATA + row].value; }
	}
	return data;
}

value_t 
	#ifdef SW 
	acts::
	#endif 
readfromvmask(uintNUMPby2_type vmask[BLOCKRAM_SIZE], unsigned int loc){
	#pragma HLS INLINE
	value_t data = 0;
	value_t returndata = 0;
	
	unsigned int col = loc / REDUCESZ; // AUTOMATEME.
	unsigned int row = loc % REDUCESZ;
	row = row / 2;
	
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
	res = NAp;
	#endif 
	#ifdef SSSP_ALGORITHM
	res = udata + edgew;
	#endif
	return res;
}

void 
	#ifdef SW 
	acts::
	#endif 
readandprocess(bool_type enable, uint512_dt * kvdram, keyvalue_t vbuffer1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], uintNUMPby2_type vmask[BLOCKRAM_SIZE], keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
		batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, globalparams_t globalparams){	
	if(enable == OFF){ return; }
	analysis_type analysis_srcbuffersz = WORKBUFFER_SIZE / 2;
	keyvalue_t tempvbuffer[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = tempvbuffer
	value_t E[2][VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=E complete
	bool_type en = ON;
	
	travstate.i_kvs = travstate.i_kvs / 2; // edges is singlevaluetype
	travstate.end_kvs = travstate.end_kvs / 2;
	loffset_kvs = loffset_kvs / 2;
	size_kvs = size_kvs / 2;
	
	readkeyvalues(ON, kvdram, tempvbuffer, goffset_kvs + loffset_kvs, size_kvs, travstate);
	
	buffer_type chunk_size = getchunksize_kvs(size_kvs, travstate, 0);
	READANDPROCESS_LOOP2: for (buffer_type i=0; i<chunk_size; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
	#pragma HLS PIPELINE II=2
		unsigned int srcvid = tempvbuffer[0][i].key;
		vertex_t vid = srcvid;
		en = ON;
		
		READANDPROCESS_LOOP1: for (unsigned int n=0; n<2; n++){
			E[0][0] = tempvbuffer[4*n + 0][i].key;
			E[0][1] = tempvbuffer[4*n + 0][i].value;
			E[0][2] = tempvbuffer[4*n + 1][i].key;
			E[0][3] = tempvbuffer[4*n + 1][i].value;
			E[0][4] = tempvbuffer[4*n + 2][i].key;
			E[0][5] = tempvbuffer[4*n + 2][i].value;
			E[0][6] = tempvbuffer[4*n + 3][i].key;
			E[0][7] = tempvbuffer[4*n + 3][i].value;
			
			en = ON;
			
			vertex_t lvid = vid - travstate.i2;
			if(lvid >= (REDUCEBUFFERSZ * FETFACTOR * VECTOR2_SIZE) || vid == UNUSEDDATA){ en = OFF; lvid = 0; }
			#ifdef _DEBUGMODE_CHECKS2
			if(vid < travstate.i2){ cout<<"readandprocess: INVALID vid. this is an error. vid: "<<vid<<", travstate.i2: "<<travstate.i2<<". exiting..."<<endl; exit(EXIT_FAILURE); }
			actsutilityobj->checkoutofbounds("readandprocess.1", lvid, REDUCEBUFFERSZ * FETFACTOR * VECTOR2_SIZE, vid, travstate.i2, NAp);
			#endif
			
			value_t udata = readfromvbuffer(vbuffer1, vbuffer2, lvid);
			#ifdef ALLVERTEXISACTIVE_ALGORITHM
			unsigned int mask = 1;
			#else 
			unsigned int mask = readfromvmask(vmask, lvid);
			#endif 
			value_t res  = processfunc(udata, 1, globalparams.GraphAlgo); 
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("readandprocess.1", mask, 2, NAp, NAp, NAp);
			#endif
			
			if(en == ON && mask == 1){
				#ifdef _DEBUGMODE_KERNELPRINTS // _DEBUGMODE_KERNELPRINTS3
				if(false){ cout<<"readandprocess: i: "<<i<<", mask: "<<mask<<", vid: "<<vid<<", travstate.i2: "<<travstate.i2<<", lvid: "<<lvid<<", udata: "<<udata<<endl; }
				for(unsigned int v=0; v<VECTOR_SIZE; v++){ cout<<"+++ readandprocess: udata: "<<udata<<", E[0]["<<v<<"]: "<<E[0][v]<<endl; }
				for(unsigned int v=0; v<VECTOR_SIZE; v++){ cout<<"+++ readandprocess: udata: "<<udata<<", E[1]["<<v<<"]: "<<E[1][v]<<endl; }
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
			actsutilityobj->globalstats_countkvsprocessed(VECTOR_SIZE*2);
			if(en == ON && mask == 1){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(VECTOR_SIZE*2); }
			#endif 
		}
	}
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

buffer_type 
	#ifdef SW 
	acts::
	#endif
preparekeyvalues(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t localcapsule[VECTOR_SIZE][NUM_PARTITIONS], step_type currentLOP, sweepparams_t sweepparams, travstate_t travstate, buffer_type size_kvs, buffer_type cutoffs[VECTOR_SIZE], globalparams_t globalparams){
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
	temp = amin(vtemp, GraphIter);
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
applyfunc(value_t vtemp, value_t res, unsigned int GraphIter, unsigned int GraphAlgo){
	value_t temp = 0;
	#ifdef PR_ALGORITHM
	temp = vtemp + res;
	#endif
	#ifdef BFS_ALGORITHM
	temp = amin(vtemp, GraphIter);
	#endif
	#ifdef SSSP_ALGORITHM
	temp = amin(vtemp, res);
	#endif
	return temp;
}

/* value_t 
	#ifdef SW 
	acts::
	#endif 
mergefunc(value_t value1, value_t value2, unsigned int GraphAlgo){
	value_t res = 0;
	#ifdef PR_ALGORITHM
	res = value1 + value2;
	#endif 
	#ifdef BFS_ALGORITHM
	res = amin(value1, value2);
	#endif 
	#ifdef SSSP_ALGORITHM
	res = amin(value1, value2);
	#endif
	return res;
}

value_t 
	#ifdef SW 
	acts::
	#endif 
mergefunc_NUMPto1(value_t v0,value_t v1,value_t v2,value_t v3,value_t v4,value_t v5,value_t v6,value_t v7,value_t v8,value_t v9,value_t v10,value_t v11,value_t v12,value_t v13,value_t v14,value_t v15, unsigned int GraphAlgo){
	value_t z = 0;
	#if NUMCOMPUTEUNITS==1
	z = v0;
	#endif 
	
	#if NUMCOMPUTEUNITS==2
	z = mergefunc(v0, v1, GraphAlgo);
	#endif 
	
	#if NUMCOMPUTEUNITS==16
	value_t w0 = mergefunc(v0, v1, GraphAlgo);
	value_t w1 = mergefunc(v2, v3, GraphAlgo);
	value_t w2 = mergefunc(v4, v5, GraphAlgo);
	value_t w3 = mergefunc(v6, v7, GraphAlgo);
	value_t w4 = mergefunc(v8, v9, GraphAlgo);
	value_t w5 = mergefunc(v10, v11, GraphAlgo);
	value_t w6 = mergefunc(v12, v13, GraphAlgo);
	value_t w7 = mergefunc(v14, v15, GraphAlgo);
	
	value_t x0 = mergefunc(w0, w1, GraphAlgo);
	value_t x1 = mergefunc(w2, w3, GraphAlgo);
	value_t x2 = mergefunc(w4, w5, GraphAlgo);
	value_t x3 = mergefunc(w6, w7, GraphAlgo);
	
	value_t y0 = mergefunc(x0, x1, GraphAlgo);
	value_t y1 = mergefunc(x2, x3, GraphAlgo);
	
	z = mergefunc(y0, y1, GraphAlgo);
	#endif 
	return z;
}

value_t 
	#ifdef SW 
	acts::
	#endif 
mergefunc_4to1(value_t v0,value_t v1,value_t v2,value_t v3, unsigned int GraphAlgo){
	value_t w0 = mergefunc(v0, v1, GraphAlgo);
	value_t w1 = mergefunc(v2, v3, GraphAlgo);

	value_t z = mergefunc(w0, w1, GraphAlgo);
	return z;
}
 */
void 
	#ifdef SW 
	acts::
	#endif
reducevector(keyvalue_t kvdata, keyvalue_t destbuffer[DOUBLE_BLOCKRAM_SIZE], buffer_type destoffset, unsigned int upperlimit, sweepparams_t sweepparams, globalparams_t globalparams){
	#pragma HLS dependence variable=destbuffer inter false // CRITICAL FIXME. ENSURE THIS IS ALWAYS TRUE.
	analysis_type analysis_loop1 = VECTOR_SIZE;
		
	keyvalue_t keyvalue = kvdata;
	vertex_t loc = keyvalue.key - upperlimit;
	
	#ifdef _DEBUGMODE_KERNELPRINTS // _DEBUGMODE_KERNELPRINTS3
	if(keyvalue.key != INVALIDDATA && keyvalue.value != INVALIDDATA){ cout<<"REDUCE SEEN @ reducevector:: vid: "<<upperlimit + loc<<", loc: "<<loc<<", keyvalue.key: "<<keyvalue.key<<", keyvalue.value: "<<keyvalue.value<<", upperlimit: "<<upperlimit<<", REDUCESZ: "<<REDUCESZ<<endl; }
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
	if(keyvalue.key != INVALIDDATA && keyvalue.value != INVALIDDATA){ vprop = destbuffer[destoffset + rowindex]; }
	
	value_t temp;
	if(colindex == 0){ temp = vprop.key; }
	else { temp = vprop.value; }
	
	value_t rettemp = reducefunc(temp, keyvalue.value, globalparams.GraphIter, globalparams.GraphAlgo);
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(keyvalue.key != INVALIDDATA && keyvalue.value != INVALIDDATA){ cout<<"REDUCEFUNC RESULT @ reducevector:: rettemp: "<<rettemp<<", temp: "<<temp<<", keyvalue.value: "<<keyvalue.value<<", NAp: "<<NAp<<endl; }
	#endif 
	
	if(colindex == 0){ vprop.key = rettemp; }
	else { vprop.value = rettemp; }
	
	if(keyvalue.key != INVALIDDATA && keyvalue.value != INVALIDDATA){ destbuffer[destoffset + rowindex] = vprop; }
	
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
reduceandbuffer(bool_type enable, keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t localcapsule[NUM_PARTITIONS], keyvalue_t vbuffer1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], sweepparams_t sweepparams, globalparams_t globalparams){				
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount = (PADDEDDESTBUFFER_SIZE / (NUM_PARTITIONS / 2)); // '2' is safety padding.
	
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
	#pragma HLS PIPELINE II=16 // CRITICAL FIXME. VHLS
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
	

			if(i< size_kvs[0]){ reducevector(kvdata0, vbuffer1[0], BUFFERBASE_VDATA, upperlimits[0], sweepparams, globalparams); }
			if(i< size_kvs[1]){ reducevector(kvdata1, vbuffer1[1], BUFFERBASE_VDATA, upperlimits[1], sweepparams, globalparams); }
			if(i< size_kvs[2]){ reducevector(kvdata2, vbuffer1[2], BUFFERBASE_VDATA, upperlimits[2], sweepparams, globalparams); }
			if(i< size_kvs[3]){ reducevector(kvdata3, vbuffer1[3], BUFFERBASE_VDATA, upperlimits[3], sweepparams, globalparams); }
			if(i< size_kvs[4]){ reducevector(kvdata4, vbuffer1[4], BUFFERBASE_VDATA, upperlimits[4], sweepparams, globalparams); }
			if(i< size_kvs[5]){ reducevector(kvdata5, vbuffer1[5], BUFFERBASE_VDATA, upperlimits[5], sweepparams, globalparams); }
			if(i< size_kvs[6]){ reducevector(kvdata6, vbuffer1[6], BUFFERBASE_VDATA, upperlimits[6], sweepparams, globalparams); }
			if(i< size_kvs[7]){ reducevector(kvdata7, vbuffer1[7], BUFFERBASE_VDATA, upperlimits[7], sweepparams, globalparams); }
			if(i< size_kvs[8]){ reducevector(kvdata8, vbuffer2[0], BUFFERBASE_VDATA, upperlimits[8], sweepparams, globalparams); }
			if(i< size_kvs[9]){ reducevector(kvdata9, vbuffer2[1], BUFFERBASE_VDATA, upperlimits[9], sweepparams, globalparams); }
			if(i< size_kvs[10]){ reducevector(kvdata10, vbuffer2[2], BUFFERBASE_VDATA, upperlimits[10], sweepparams, globalparams); }
			if(i< size_kvs[11]){ reducevector(kvdata11, vbuffer2[3], BUFFERBASE_VDATA, upperlimits[11], sweepparams, globalparams); }
			if(i< size_kvs[12]){ reducevector(kvdata12, vbuffer2[4], BUFFERBASE_VDATA, upperlimits[12], sweepparams, globalparams); }
			if(i< size_kvs[13]){ reducevector(kvdata13, vbuffer2[5], BUFFERBASE_VDATA, upperlimits[13], sweepparams, globalparams); }
			if(i< size_kvs[14]){ reducevector(kvdata14, vbuffer2[6], BUFFERBASE_VDATA, upperlimits[14], sweepparams, globalparams); }
			if(i< size_kvs[15]){ reducevector(kvdata15, vbuffer2[7], BUFFERBASE_VDATA, upperlimits[15], sweepparams, globalparams); }
		}
	}
	return;
}

// functions (synchronize)
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
	res = amin(value1, value2);
	#endif 
	#ifdef SSSP_ALGORITHM
	res = amin(value1, value2);
	#endif
	return res;
}

/* value_t 
	#ifdef SW 
	acts::
	#endif 
mergefunc_4to1(value_t v0,value_t v1,value_t v2,value_t v3, unsigned int GraphAlgo){
	value_t w0 = mergefunc(v0, v1, GraphAlgo);
	value_t w1 = mergefunc(v2, v3, GraphAlgo);

	value_t z = mergefunc(w0, w1, GraphAlgo);
	return z;
}
 */
value_t 
	#ifdef SW 
	acts::
	#endif 
mergefunc_NUMPto1(value_t v0,value_t v1,value_t v2,value_t v3,value_t v4,value_t v5,value_t v6,value_t v7,value_t v8,value_t v9,value_t v10,value_t v11,value_t v12,value_t v13,value_t v14,value_t v15, unsigned int GraphAlgo){
	value_t z = 0;
	#if NUMCOMPUTEUNITS==1
	z = v0;
	#endif 
	
	#if NUMCOMPUTEUNITS==2
	z = mergefunc(v0, v1, GraphAlgo);
	#endif 
	
	#if NUMCOMPUTEUNITS==16
	value_t w0 = mergefunc(v0, v1, GraphAlgo);
	value_t w1 = mergefunc(v2, v3, GraphAlgo);
	value_t w2 = mergefunc(v4, v5, GraphAlgo);
	value_t w3 = mergefunc(v6, v7, GraphAlgo);
	value_t w4 = mergefunc(v8, v9, GraphAlgo);
	value_t w5 = mergefunc(v10, v11, GraphAlgo);
	value_t w6 = mergefunc(v12, v13, GraphAlgo);
	value_t w7 = mergefunc(v14, v15, GraphAlgo);
	
	value_t x0 = mergefunc(w0, w1, GraphAlgo);
	value_t x1 = mergefunc(w2, w3, GraphAlgo);
	value_t x2 = mergefunc(w4, w5, GraphAlgo);
	value_t x3 = mergefunc(w6, w7, GraphAlgo);
	
	value_t y0 = mergefunc(x0, x1, GraphAlgo);
	value_t y1 = mergefunc(x2, x3, GraphAlgo);
	
	z = mergefunc(y0, y1, GraphAlgo);
	#endif 
	return z;
}

/* bool_type 
	#ifdef SW 
	acts::
	#endif
pmerge_4to1(bool_type enable, 
keyvalue_t vbuffer0_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer0_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer1_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer1_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer2_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer2_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer3_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer3_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE],  
		keyvalue_t synvbuffer1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t synvbuffer2[VECTOR_SIZE][BLOCKRAM_SIZE],
		globalparams_t globalparams){
	if(enable == OFF){ return OFF; }
	
	unsigned int vid[NUM_PARTITIONS][2];
	#pragma HLS ARRAY_PARTITION variable=vid complete
	
	keyvalue_t data0;
	keyvalue_t res0;
	keyvalue_t data0_0;
	keyvalue_t data0_1;
	keyvalue_t data0_2;
	keyvalue_t data0_3;
	keyvalue_t data1;
	keyvalue_t res1;
	keyvalue_t data1_0;
	keyvalue_t data1_1;
	keyvalue_t data1_2;
	keyvalue_t data1_3;
	keyvalue_t data2;
	keyvalue_t res2;
	keyvalue_t data2_0;
	keyvalue_t data2_1;
	keyvalue_t data2_2;
	keyvalue_t data2_3;
	keyvalue_t data3;
	keyvalue_t res3;
	keyvalue_t data3_0;
	keyvalue_t data3_1;
	keyvalue_t data3_2;
	keyvalue_t data3_3;
	keyvalue_t data4;
	keyvalue_t res4;
	keyvalue_t data4_0;
	keyvalue_t data4_1;
	keyvalue_t data4_2;
	keyvalue_t data4_3;
	keyvalue_t data5;
	keyvalue_t res5;
	keyvalue_t data5_0;
	keyvalue_t data5_1;
	keyvalue_t data5_2;
	keyvalue_t data5_3;
	keyvalue_t data6;
	keyvalue_t res6;
	keyvalue_t data6_0;
	keyvalue_t data6_1;
	keyvalue_t data6_2;
	keyvalue_t data6_3;
	keyvalue_t data7;
	keyvalue_t res7;
	keyvalue_t data7_0;
	keyvalue_t data7_1;
	keyvalue_t data7_2;
	keyvalue_t data7_3;
	keyvalue_t data8;
	keyvalue_t res8;
	keyvalue_t data8_0;
	keyvalue_t data8_1;
	keyvalue_t data8_2;
	keyvalue_t data8_3;
	keyvalue_t data9;
	keyvalue_t res9;
	keyvalue_t data9_0;
	keyvalue_t data9_1;
	keyvalue_t data9_2;
	keyvalue_t data9_3;
	keyvalue_t data10;
	keyvalue_t res10;
	keyvalue_t data10_0;
	keyvalue_t data10_1;
	keyvalue_t data10_2;
	keyvalue_t data10_3;
	keyvalue_t data11;
	keyvalue_t res11;
	keyvalue_t data11_0;
	keyvalue_t data11_1;
	keyvalue_t data11_2;
	keyvalue_t data11_3;
	keyvalue_t data12;
	keyvalue_t res12;
	keyvalue_t data12_0;
	keyvalue_t data12_1;
	keyvalue_t data12_2;
	keyvalue_t data12_3;
	keyvalue_t data13;
	keyvalue_t res13;
	keyvalue_t data13_0;
	keyvalue_t data13_1;
	keyvalue_t data13_2;
	keyvalue_t data13_3;
	keyvalue_t data14;
	keyvalue_t res14;
	keyvalue_t data14_0;
	keyvalue_t data14_1;
	keyvalue_t data14_2;
	keyvalue_t data14_3;
	keyvalue_t data15;
	keyvalue_t res15;
	keyvalue_t data15_0;
	keyvalue_t data15_1;
	keyvalue_t data15_2;
	keyvalue_t data15_3;
	
	PMERGE_LOOP1: for(buffer_type i=0; i<REDUCEBUFFERSZ; i++){
	#pragma HLS dependence variable=vbuffer1 inter false
	#pragma HLS dependence variable=vbuffer2 inter false
	#pragma HLS PIPELINE II=1

		data0_0 = vbuffer0_1[0][BUFFERBASE_VDATA + i];
		data0_1 = vbuffer1_1[0][BUFFERBASE_VDATA + i];
		data0_2 = vbuffer2_1[0][BUFFERBASE_VDATA + i];
		data0_3 = vbuffer3_1[0][BUFFERBASE_VDATA + i];
		data1_0 = vbuffer0_1[1][BUFFERBASE_VDATA + i];
		data1_1 = vbuffer1_1[1][BUFFERBASE_VDATA + i];
		data1_2 = vbuffer2_1[1][BUFFERBASE_VDATA + i];
		data1_3 = vbuffer3_1[1][BUFFERBASE_VDATA + i];
		data2_0 = vbuffer0_1[2][BUFFERBASE_VDATA + i];
		data2_1 = vbuffer1_1[2][BUFFERBASE_VDATA + i];
		data2_2 = vbuffer2_1[2][BUFFERBASE_VDATA + i];
		data2_3 = vbuffer3_1[2][BUFFERBASE_VDATA + i];
		data3_0 = vbuffer0_1[3][BUFFERBASE_VDATA + i];
		data3_1 = vbuffer1_1[3][BUFFERBASE_VDATA + i];
		data3_2 = vbuffer2_1[3][BUFFERBASE_VDATA + i];
		data3_3 = vbuffer3_1[3][BUFFERBASE_VDATA + i];
		data4_0 = vbuffer0_1[4][BUFFERBASE_VDATA + i];
		data4_1 = vbuffer1_1[4][BUFFERBASE_VDATA + i];
		data4_2 = vbuffer2_1[4][BUFFERBASE_VDATA + i];
		data4_3 = vbuffer3_1[4][BUFFERBASE_VDATA + i];
		data5_0 = vbuffer0_1[5][BUFFERBASE_VDATA + i];
		data5_1 = vbuffer1_1[5][BUFFERBASE_VDATA + i];
		data5_2 = vbuffer2_1[5][BUFFERBASE_VDATA + i];
		data5_3 = vbuffer3_1[5][BUFFERBASE_VDATA + i];
		data6_0 = vbuffer0_1[6][BUFFERBASE_VDATA + i];
		data6_1 = vbuffer1_1[6][BUFFERBASE_VDATA + i];
		data6_2 = vbuffer2_1[6][BUFFERBASE_VDATA + i];
		data6_3 = vbuffer3_1[6][BUFFERBASE_VDATA + i];
		data7_0 = vbuffer0_1[7][BUFFERBASE_VDATA + i];
		data7_1 = vbuffer1_1[7][BUFFERBASE_VDATA + i];
		data7_2 = vbuffer2_1[7][BUFFERBASE_VDATA + i];
		data7_3 = vbuffer3_1[7][BUFFERBASE_VDATA + i];
		data8_0 = vbuffer0_2[0][BUFFERBASE_VDATA + i];
		data8_1 = vbuffer1_2[0][BUFFERBASE_VDATA + i];
		data8_2 = vbuffer2_2[0][BUFFERBASE_VDATA + i];
		data8_3 = vbuffer3_2[0][BUFFERBASE_VDATA + i];
		data9_0 = vbuffer0_2[1][BUFFERBASE_VDATA + i];
		data9_1 = vbuffer1_2[1][BUFFERBASE_VDATA + i];
		data9_2 = vbuffer2_2[1][BUFFERBASE_VDATA + i];
		data9_3 = vbuffer3_2[1][BUFFERBASE_VDATA + i];
		data10_0 = vbuffer0_2[2][BUFFERBASE_VDATA + i];
		data10_1 = vbuffer1_2[2][BUFFERBASE_VDATA + i];
		data10_2 = vbuffer2_2[2][BUFFERBASE_VDATA + i];
		data10_3 = vbuffer3_2[2][BUFFERBASE_VDATA + i];
		data11_0 = vbuffer0_2[3][BUFFERBASE_VDATA + i];
		data11_1 = vbuffer1_2[3][BUFFERBASE_VDATA + i];
		data11_2 = vbuffer2_2[3][BUFFERBASE_VDATA + i];
		data11_3 = vbuffer3_2[3][BUFFERBASE_VDATA + i];
		data12_0 = vbuffer0_2[4][BUFFERBASE_VDATA + i];
		data12_1 = vbuffer1_2[4][BUFFERBASE_VDATA + i];
		data12_2 = vbuffer2_2[4][BUFFERBASE_VDATA + i];
		data12_3 = vbuffer3_2[4][BUFFERBASE_VDATA + i];
		data13_0 = vbuffer0_2[5][BUFFERBASE_VDATA + i];
		data13_1 = vbuffer1_2[5][BUFFERBASE_VDATA + i];
		data13_2 = vbuffer2_2[5][BUFFERBASE_VDATA + i];
		data13_3 = vbuffer3_2[5][BUFFERBASE_VDATA + i];
		data14_0 = vbuffer0_2[6][BUFFERBASE_VDATA + i];
		data14_1 = vbuffer1_2[6][BUFFERBASE_VDATA + i];
		data14_2 = vbuffer2_2[6][BUFFERBASE_VDATA + i];
		data14_3 = vbuffer3_2[6][BUFFERBASE_VDATA + i];
		data15_0 = vbuffer0_2[7][BUFFERBASE_VDATA + i];
		data15_1 = vbuffer1_2[7][BUFFERBASE_VDATA + i];
		data15_2 = vbuffer2_2[7][BUFFERBASE_VDATA + i];
		data15_3 = vbuffer3_2[7][BUFFERBASE_VDATA + i];
		
		data0.key = mergefunc_4to1(data0_0.key,data0_1.key,data0_2.key,data0_3.key, globalparams.GraphAlgo);
		data1.key = mergefunc_4to1(data1_0.key,data1_1.key,data1_2.key,data1_3.key, globalparams.GraphAlgo);
		data2.key = mergefunc_4to1(data2_0.key,data2_1.key,data2_2.key,data2_3.key, globalparams.GraphAlgo);
		data3.key = mergefunc_4to1(data3_0.key,data3_1.key,data3_2.key,data3_3.key, globalparams.GraphAlgo);
		data4.key = mergefunc_4to1(data4_0.key,data4_1.key,data4_2.key,data4_3.key, globalparams.GraphAlgo);
		data5.key = mergefunc_4to1(data5_0.key,data5_1.key,data5_2.key,data5_3.key, globalparams.GraphAlgo);
		data6.key = mergefunc_4to1(data6_0.key,data6_1.key,data6_2.key,data6_3.key, globalparams.GraphAlgo);
		data7.key = mergefunc_4to1(data7_0.key,data7_1.key,data7_2.key,data7_3.key, globalparams.GraphAlgo);
		data8.key = mergefunc_4to1(data8_0.key,data8_1.key,data8_2.key,data8_3.key, globalparams.GraphAlgo);
		data9.key = mergefunc_4to1(data9_0.key,data9_1.key,data9_2.key,data9_3.key, globalparams.GraphAlgo);
		data10.key = mergefunc_4to1(data10_0.key,data10_1.key,data10_2.key,data10_3.key, globalparams.GraphAlgo);
		data11.key = mergefunc_4to1(data11_0.key,data11_1.key,data11_2.key,data11_3.key, globalparams.GraphAlgo);
		data12.key = mergefunc_4to1(data12_0.key,data12_1.key,data12_2.key,data12_3.key, globalparams.GraphAlgo);
		data13.key = mergefunc_4to1(data13_0.key,data13_1.key,data13_2.key,data13_3.key, globalparams.GraphAlgo);
		data14.key = mergefunc_4to1(data14_0.key,data14_1.key,data14_2.key,data14_3.key, globalparams.GraphAlgo);
		data15.key = mergefunc_4to1(data15_0.key,data15_1.key,data15_2.key,data15_3.key, globalparams.GraphAlgo);
		data0.value = mergefunc_4to1(data0_0.value,data0_1.value,data0_2.value,data0_3.value, globalparams.GraphAlgo);
		data1.value = mergefunc_4to1(data1_0.value,data1_1.value,data1_2.value,data1_3.value, globalparams.GraphAlgo);
		data2.value = mergefunc_4to1(data2_0.value,data2_1.value,data2_2.value,data2_3.value, globalparams.GraphAlgo);
		data3.value = mergefunc_4to1(data3_0.value,data3_1.value,data3_2.value,data3_3.value, globalparams.GraphAlgo);
		data4.value = mergefunc_4to1(data4_0.value,data4_1.value,data4_2.value,data4_3.value, globalparams.GraphAlgo);
		data5.value = mergefunc_4to1(data5_0.value,data5_1.value,data5_2.value,data5_3.value, globalparams.GraphAlgo);
		data6.value = mergefunc_4to1(data6_0.value,data6_1.value,data6_2.value,data6_3.value, globalparams.GraphAlgo);
		data7.value = mergefunc_4to1(data7_0.value,data7_1.value,data7_2.value,data7_3.value, globalparams.GraphAlgo);
		data8.value = mergefunc_4to1(data8_0.value,data8_1.value,data8_2.value,data8_3.value, globalparams.GraphAlgo);
		data9.value = mergefunc_4to1(data9_0.value,data9_1.value,data9_2.value,data9_3.value, globalparams.GraphAlgo);
		data10.value = mergefunc_4to1(data10_0.value,data10_1.value,data10_2.value,data10_3.value, globalparams.GraphAlgo);
		data11.value = mergefunc_4to1(data11_0.value,data11_1.value,data11_2.value,data11_3.value, globalparams.GraphAlgo);
		data12.value = mergefunc_4to1(data12_0.value,data12_1.value,data12_2.value,data12_3.value, globalparams.GraphAlgo);
		data13.value = mergefunc_4to1(data13_0.value,data13_1.value,data13_2.value,data13_3.value, globalparams.GraphAlgo);
		data14.value = mergefunc_4to1(data14_0.value,data14_1.value,data14_2.value,data14_3.value, globalparams.GraphAlgo);
		data15.value = mergefunc_4to1(data15_0.value,data15_1.value,data15_2.value,data15_3.value, globalparams.GraphAlgo);
		
		res0.key = data0.key; 
		res1.key = data1.key; 
		res2.key = data2.key; 
		res3.key = data3.key; 
		res4.key = data4.key; 
		res5.key = data5.key; 
		res6.key = data6.key; 
		res7.key = data7.key; 
		res8.key = data8.key; 
		res9.key = data9.key; 
		res10.key = data10.key; 
		res11.key = data11.key; 
		res12.key = data12.key; 
		res13.key = data13.key; 
		res14.key = data14.key; 
		res15.key = data15.key; 
		res0.value = data0.value; 
		res1.value = data1.value; 
		res2.value = data2.value; 
		res3.value = data3.value; 
		res4.value = data4.value; 
		res5.value = data5.value; 
		res6.value = data6.value; 
		res7.value = data7.value; 
		res8.value = data8.value; 
		res9.value = data9.value; 
		res10.value = data10.value; 
		res11.value = data11.value; 
		res12.value = data12.value; 
		res13.value = data13.value; 
		res14.value = data14.value; 
		res15.value = data15.value; 
		
	
		synvbuffer1[0][i] = res0;	
	
		synvbuffer1[1][i] = res1;	
	
		synvbuffer1[2][i] = res2;	
	
		synvbuffer1[3][i] = res3;	
	
		synvbuffer1[4][i] = res4;	
	
		synvbuffer1[5][i] = res5;	
	
		synvbuffer1[6][i] = res6;	
	
		synvbuffer1[7][i] = res7;	
	
		synvbuffer2[0][i] = res8;	
	
		synvbuffer2[1][i] = res9;	
	
		synvbuffer2[2][i] = res10;	
	
		synvbuffer2[3][i] = res11;	
	
		synvbuffer2[4][i] = res12;	
	
		synvbuffer2[5][i] = res13;	
	
		synvbuffer2[6][i] = res14;	
	
		synvbuffer2[7][i] = res15;	
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printkeyvalues("synchronize.vbuffer1", vbuffer1, NUM_VBUFFERS, 4);
	actsutilityobj->printkeyvalues("synchronize.vbuffer2", vbuffer2, NUM_VBUFFERS, 4);
	#endif
	return NAp;
}

bool_type 
	#ifdef SW 
	acts::
	#endif
pmerge_16to4(bool_type enable, 
keyvalue_t vbuffer0_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer0_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE],keyvalue_t vbuffer1_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer1_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE],keyvalue_t vbuffer2_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer2_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE],keyvalue_t vbuffer3_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer3_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE],keyvalue_t vbuffer4_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer4_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE],keyvalue_t vbuffer5_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer5_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE],keyvalue_t vbuffer6_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer6_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE],keyvalue_t vbuffer7_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer7_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE],keyvalue_t vbuffer8_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer8_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE],keyvalue_t vbuffer9_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer9_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE],keyvalue_t vbuffer10_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer10_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE],keyvalue_t vbuffer11_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer11_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE],keyvalue_t vbuffer12_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer12_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE],keyvalue_t vbuffer13_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer13_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE],keyvalue_t vbuffer14_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer14_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE],keyvalue_t vbuffer15_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer15_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], 
keyvalue_t synvbuffer0_1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t synvbuffer0_2[VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_t synvbuffer1_1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t synvbuffer1_2[VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_t synvbuffer2_1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t synvbuffer2_2[VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_t synvbuffer3_1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t synvbuffer3_2[VECTOR_SIZE][BLOCKRAM_SIZE],		globalparams_t globalparams){
	pmerge_4to1(enable, vbuffer0_1, vbuffer0_2, vbuffer1_1, vbuffer1_2, vbuffer2_1, vbuffer2_2, vbuffer3_1, vbuffer3_2,  synvbuffer0_1, synvbuffer0_2, globalparams);
	pmerge_4to1(enable, vbuffer4_1, vbuffer4_2, vbuffer5_1, vbuffer5_2, vbuffer6_1, vbuffer6_2, vbuffer7_1, vbuffer7_2,  synvbuffer1_1, synvbuffer1_2, globalparams);
	pmerge_4to1(enable, vbuffer8_1, vbuffer8_2, vbuffer9_1, vbuffer9_2, vbuffer10_1, vbuffer10_2, vbuffer11_1, vbuffer11_2,  synvbuffer2_1, synvbuffer2_2, globalparams);
	pmerge_4to1(enable, vbuffer12_1, vbuffer12_2, vbuffer13_1, vbuffer13_2, vbuffer14_1, vbuffer14_2, vbuffer15_1, vbuffer15_2,  synvbuffer3_1, synvbuffer3_2, globalparams);
	return NAp;
}

bool_type 
	#ifdef SW 
	acts::
	#endif
pmergeandapply_4to1(bool_type enable, 
keyvalue_t vbuffer0_1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t vbuffer0_2[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t vbuffer1_1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t vbuffer1_2[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t vbuffer2_1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t vbuffer2_2[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t vbuffer3_1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t vbuffer3_2[VECTOR_SIZE][BLOCKRAM_SIZE],  
		keyvalue_t synvbuffer1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t synvbuffer2[VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask[BLOCKRAM_SIZE],
		globalparams_t globalparams){
	if(enable == OFF){ return OFF; }
	
	unsigned int vid[NUM_PARTITIONS][2];
	#pragma HLS ARRAY_PARTITION variable=vid complete
	
	keyvalue_t data0;
	keyvalue_t res0;
	keyvalue_t data0_0;
	keyvalue_t data0_1;
	keyvalue_t data0_2;
	keyvalue_t data0_3;
	keyvalue_t data0_4;
	keyvalue_t data0_5;
	keyvalue_t data0_6;
	keyvalue_t data0_7;
	keyvalue_t data0_8;
	keyvalue_t data0_9;
	keyvalue_t data0_10;
	keyvalue_t data0_11;
	keyvalue_t data0_12;
	keyvalue_t data0_13;
	keyvalue_t data0_14;
	keyvalue_t data0_15;
	keyvalue_t data1;
	keyvalue_t res1;
	keyvalue_t data1_0;
	keyvalue_t data1_1;
	keyvalue_t data1_2;
	keyvalue_t data1_3;
	keyvalue_t data1_4;
	keyvalue_t data1_5;
	keyvalue_t data1_6;
	keyvalue_t data1_7;
	keyvalue_t data1_8;
	keyvalue_t data1_9;
	keyvalue_t data1_10;
	keyvalue_t data1_11;
	keyvalue_t data1_12;
	keyvalue_t data1_13;
	keyvalue_t data1_14;
	keyvalue_t data1_15;
	keyvalue_t data2;
	keyvalue_t res2;
	keyvalue_t data2_0;
	keyvalue_t data2_1;
	keyvalue_t data2_2;
	keyvalue_t data2_3;
	keyvalue_t data2_4;
	keyvalue_t data2_5;
	keyvalue_t data2_6;
	keyvalue_t data2_7;
	keyvalue_t data2_8;
	keyvalue_t data2_9;
	keyvalue_t data2_10;
	keyvalue_t data2_11;
	keyvalue_t data2_12;
	keyvalue_t data2_13;
	keyvalue_t data2_14;
	keyvalue_t data2_15;
	keyvalue_t data3;
	keyvalue_t res3;
	keyvalue_t data3_0;
	keyvalue_t data3_1;
	keyvalue_t data3_2;
	keyvalue_t data3_3;
	keyvalue_t data3_4;
	keyvalue_t data3_5;
	keyvalue_t data3_6;
	keyvalue_t data3_7;
	keyvalue_t data3_8;
	keyvalue_t data3_9;
	keyvalue_t data3_10;
	keyvalue_t data3_11;
	keyvalue_t data3_12;
	keyvalue_t data3_13;
	keyvalue_t data3_14;
	keyvalue_t data3_15;
	keyvalue_t data4;
	keyvalue_t res4;
	keyvalue_t data4_0;
	keyvalue_t data4_1;
	keyvalue_t data4_2;
	keyvalue_t data4_3;
	keyvalue_t data4_4;
	keyvalue_t data4_5;
	keyvalue_t data4_6;
	keyvalue_t data4_7;
	keyvalue_t data4_8;
	keyvalue_t data4_9;
	keyvalue_t data4_10;
	keyvalue_t data4_11;
	keyvalue_t data4_12;
	keyvalue_t data4_13;
	keyvalue_t data4_14;
	keyvalue_t data4_15;
	keyvalue_t data5;
	keyvalue_t res5;
	keyvalue_t data5_0;
	keyvalue_t data5_1;
	keyvalue_t data5_2;
	keyvalue_t data5_3;
	keyvalue_t data5_4;
	keyvalue_t data5_5;
	keyvalue_t data5_6;
	keyvalue_t data5_7;
	keyvalue_t data5_8;
	keyvalue_t data5_9;
	keyvalue_t data5_10;
	keyvalue_t data5_11;
	keyvalue_t data5_12;
	keyvalue_t data5_13;
	keyvalue_t data5_14;
	keyvalue_t data5_15;
	keyvalue_t data6;
	keyvalue_t res6;
	keyvalue_t data6_0;
	keyvalue_t data6_1;
	keyvalue_t data6_2;
	keyvalue_t data6_3;
	keyvalue_t data6_4;
	keyvalue_t data6_5;
	keyvalue_t data6_6;
	keyvalue_t data6_7;
	keyvalue_t data6_8;
	keyvalue_t data6_9;
	keyvalue_t data6_10;
	keyvalue_t data6_11;
	keyvalue_t data6_12;
	keyvalue_t data6_13;
	keyvalue_t data6_14;
	keyvalue_t data6_15;
	keyvalue_t data7;
	keyvalue_t res7;
	keyvalue_t data7_0;
	keyvalue_t data7_1;
	keyvalue_t data7_2;
	keyvalue_t data7_3;
	keyvalue_t data7_4;
	keyvalue_t data7_5;
	keyvalue_t data7_6;
	keyvalue_t data7_7;
	keyvalue_t data7_8;
	keyvalue_t data7_9;
	keyvalue_t data7_10;
	keyvalue_t data7_11;
	keyvalue_t data7_12;
	keyvalue_t data7_13;
	keyvalue_t data7_14;
	keyvalue_t data7_15;
	keyvalue_t data8;
	keyvalue_t res8;
	keyvalue_t data8_0;
	keyvalue_t data8_1;
	keyvalue_t data8_2;
	keyvalue_t data8_3;
	keyvalue_t data8_4;
	keyvalue_t data8_5;
	keyvalue_t data8_6;
	keyvalue_t data8_7;
	keyvalue_t data8_8;
	keyvalue_t data8_9;
	keyvalue_t data8_10;
	keyvalue_t data8_11;
	keyvalue_t data8_12;
	keyvalue_t data8_13;
	keyvalue_t data8_14;
	keyvalue_t data8_15;
	keyvalue_t data9;
	keyvalue_t res9;
	keyvalue_t data9_0;
	keyvalue_t data9_1;
	keyvalue_t data9_2;
	keyvalue_t data9_3;
	keyvalue_t data9_4;
	keyvalue_t data9_5;
	keyvalue_t data9_6;
	keyvalue_t data9_7;
	keyvalue_t data9_8;
	keyvalue_t data9_9;
	keyvalue_t data9_10;
	keyvalue_t data9_11;
	keyvalue_t data9_12;
	keyvalue_t data9_13;
	keyvalue_t data9_14;
	keyvalue_t data9_15;
	keyvalue_t data10;
	keyvalue_t res10;
	keyvalue_t data10_0;
	keyvalue_t data10_1;
	keyvalue_t data10_2;
	keyvalue_t data10_3;
	keyvalue_t data10_4;
	keyvalue_t data10_5;
	keyvalue_t data10_6;
	keyvalue_t data10_7;
	keyvalue_t data10_8;
	keyvalue_t data10_9;
	keyvalue_t data10_10;
	keyvalue_t data10_11;
	keyvalue_t data10_12;
	keyvalue_t data10_13;
	keyvalue_t data10_14;
	keyvalue_t data10_15;
	keyvalue_t data11;
	keyvalue_t res11;
	keyvalue_t data11_0;
	keyvalue_t data11_1;
	keyvalue_t data11_2;
	keyvalue_t data11_3;
	keyvalue_t data11_4;
	keyvalue_t data11_5;
	keyvalue_t data11_6;
	keyvalue_t data11_7;
	keyvalue_t data11_8;
	keyvalue_t data11_9;
	keyvalue_t data11_10;
	keyvalue_t data11_11;
	keyvalue_t data11_12;
	keyvalue_t data11_13;
	keyvalue_t data11_14;
	keyvalue_t data11_15;
	keyvalue_t data12;
	keyvalue_t res12;
	keyvalue_t data12_0;
	keyvalue_t data12_1;
	keyvalue_t data12_2;
	keyvalue_t data12_3;
	keyvalue_t data12_4;
	keyvalue_t data12_5;
	keyvalue_t data12_6;
	keyvalue_t data12_7;
	keyvalue_t data12_8;
	keyvalue_t data12_9;
	keyvalue_t data12_10;
	keyvalue_t data12_11;
	keyvalue_t data12_12;
	keyvalue_t data12_13;
	keyvalue_t data12_14;
	keyvalue_t data12_15;
	keyvalue_t data13;
	keyvalue_t res13;
	keyvalue_t data13_0;
	keyvalue_t data13_1;
	keyvalue_t data13_2;
	keyvalue_t data13_3;
	keyvalue_t data13_4;
	keyvalue_t data13_5;
	keyvalue_t data13_6;
	keyvalue_t data13_7;
	keyvalue_t data13_8;
	keyvalue_t data13_9;
	keyvalue_t data13_10;
	keyvalue_t data13_11;
	keyvalue_t data13_12;
	keyvalue_t data13_13;
	keyvalue_t data13_14;
	keyvalue_t data13_15;
	keyvalue_t data14;
	keyvalue_t res14;
	keyvalue_t data14_0;
	keyvalue_t data14_1;
	keyvalue_t data14_2;
	keyvalue_t data14_3;
	keyvalue_t data14_4;
	keyvalue_t data14_5;
	keyvalue_t data14_6;
	keyvalue_t data14_7;
	keyvalue_t data14_8;
	keyvalue_t data14_9;
	keyvalue_t data14_10;
	keyvalue_t data14_11;
	keyvalue_t data14_12;
	keyvalue_t data14_13;
	keyvalue_t data14_14;
	keyvalue_t data14_15;
	keyvalue_t data15;
	keyvalue_t res15;
	keyvalue_t data15_0;
	keyvalue_t data15_1;
	keyvalue_t data15_2;
	keyvalue_t data15_3;
	keyvalue_t data15_4;
	keyvalue_t data15_5;
	keyvalue_t data15_6;
	keyvalue_t data15_7;
	keyvalue_t data15_8;
	keyvalue_t data15_9;
	keyvalue_t data15_10;
	keyvalue_t data15_11;
	keyvalue_t data15_12;
	keyvalue_t data15_13;
	keyvalue_t data15_14;
	keyvalue_t data15_15;
	
	bool_type partitionisactive = OFF;
	
	// sync and apply
	SYNCHRONIZE_LOOP1: for(buffer_type i=0; i<REDUCEBUFFERSZ; i++){
	#pragma HLS dependence variable=synvbuffer1 inter false
	#pragma HLS dependence variable=synvbuffer2 inter false
	#pragma HLS PIPELINE II=1
	
		vid[0][0] = ((0*REDUCESZ) + i*2);
		vid[0][1] = ((0*REDUCESZ) + i*2 + 1);
		vid[1][0] = ((1*REDUCESZ) + i*2);
		vid[1][1] = ((1*REDUCESZ) + i*2 + 1);
		vid[2][0] = ((2*REDUCESZ) + i*2);
		vid[2][1] = ((2*REDUCESZ) + i*2 + 1);
		vid[3][0] = ((3*REDUCESZ) + i*2);
		vid[3][1] = ((3*REDUCESZ) + i*2 + 1);
		vid[4][0] = ((4*REDUCESZ) + i*2);
		vid[4][1] = ((4*REDUCESZ) + i*2 + 1);
		vid[5][0] = ((5*REDUCESZ) + i*2);
		vid[5][1] = ((5*REDUCESZ) + i*2 + 1);
		vid[6][0] = ((6*REDUCESZ) + i*2);
		vid[6][1] = ((6*REDUCESZ) + i*2 + 1);
		vid[7][0] = ((7*REDUCESZ) + i*2);
		vid[7][1] = ((7*REDUCESZ) + i*2 + 1);
		vid[8][0] = ((8*REDUCESZ) + i*2);
		vid[8][1] = ((8*REDUCESZ) + i*2 + 1);
		vid[9][0] = ((9*REDUCESZ) + i*2);
		vid[9][1] = ((9*REDUCESZ) + i*2 + 1);
		vid[10][0] = ((10*REDUCESZ) + i*2);
		vid[10][1] = ((10*REDUCESZ) + i*2 + 1);
		vid[11][0] = ((11*REDUCESZ) + i*2);
		vid[11][1] = ((11*REDUCESZ) + i*2 + 1);
		vid[12][0] = ((12*REDUCESZ) + i*2);
		vid[12][1] = ((12*REDUCESZ) + i*2 + 1);
		vid[13][0] = ((13*REDUCESZ) + i*2);
		vid[13][1] = ((13*REDUCESZ) + i*2 + 1);
		vid[14][0] = ((14*REDUCESZ) + i*2);
		vid[14][1] = ((14*REDUCESZ) + i*2 + 1);
		vid[15][0] = ((15*REDUCESZ) + i*2);
		vid[15][1] = ((15*REDUCESZ) + i*2 + 1);
	
 // AUTOMATEME.
		data0_0 = vbuffer0_1[0][i];
		data0_1 = vbuffer1_1[0][i];
		data0_2 = vbuffer2_1[0][i];
		data0_3 = vbuffer3_1[0][i];
 // AUTOMATEME.
		data1_0 = vbuffer0_1[1][i];
		data1_1 = vbuffer1_1[1][i];
		data1_2 = vbuffer2_1[1][i];
		data1_3 = vbuffer3_1[1][i];
 // AUTOMATEME.
		data2_0 = vbuffer0_1[2][i];
		data2_1 = vbuffer1_1[2][i];
		data2_2 = vbuffer2_1[2][i];
		data2_3 = vbuffer3_1[2][i];
 // AUTOMATEME.
		data3_0 = vbuffer0_1[3][i];
		data3_1 = vbuffer1_1[3][i];
		data3_2 = vbuffer2_1[3][i];
		data3_3 = vbuffer3_1[3][i];
 // AUTOMATEME.
		data4_0 = vbuffer0_1[4][i];
		data4_1 = vbuffer1_1[4][i];
		data4_2 = vbuffer2_1[4][i];
		data4_3 = vbuffer3_1[4][i];
 // AUTOMATEME.
		data5_0 = vbuffer0_1[5][i];
		data5_1 = vbuffer1_1[5][i];
		data5_2 = vbuffer2_1[5][i];
		data5_3 = vbuffer3_1[5][i];
 // AUTOMATEME.
		data6_0 = vbuffer0_1[6][i];
		data6_1 = vbuffer1_1[6][i];
		data6_2 = vbuffer2_1[6][i];
		data6_3 = vbuffer3_1[6][i];
 // AUTOMATEME.
		data7_0 = vbuffer0_1[7][i];
		data7_1 = vbuffer1_1[7][i];
		data7_2 = vbuffer2_1[7][i];
		data7_3 = vbuffer3_1[7][i];
		data8_0 = vbuffer0_2[0][i];
		data8_1 = vbuffer1_2[0][i];
		data8_2 = vbuffer2_2[0][i];
		data8_3 = vbuffer3_2[0][i];
		data9_0 = vbuffer0_2[1][i];
		data9_1 = vbuffer1_2[1][i];
		data9_2 = vbuffer2_2[1][i];
		data9_3 = vbuffer3_2[1][i];
		data10_0 = vbuffer0_2[2][i];
		data10_1 = vbuffer1_2[2][i];
		data10_2 = vbuffer2_2[2][i];
		data10_3 = vbuffer3_2[2][i];
		data11_0 = vbuffer0_2[3][i];
		data11_1 = vbuffer1_2[3][i];
		data11_2 = vbuffer2_2[3][i];
		data11_3 = vbuffer3_2[3][i];
		data12_0 = vbuffer0_2[4][i];
		data12_1 = vbuffer1_2[4][i];
		data12_2 = vbuffer2_2[4][i];
		data12_3 = vbuffer3_2[4][i];
		data13_0 = vbuffer0_2[5][i];
		data13_1 = vbuffer1_2[5][i];
		data13_2 = vbuffer2_2[5][i];
		data13_3 = vbuffer3_2[5][i];
		data14_0 = vbuffer0_2[6][i];
		data14_1 = vbuffer1_2[6][i];
		data14_2 = vbuffer2_2[6][i];
		data14_3 = vbuffer3_2[6][i];
		data15_0 = vbuffer0_2[7][i];
		data15_1 = vbuffer1_2[7][i];
		data15_2 = vbuffer2_2[7][i];
		data15_3 = vbuffer3_2[7][i];
		
		keyvalue_t udata0 = synvbuffer1[0][i];
		keyvalue_t udata1 = synvbuffer1[1][i];
		keyvalue_t udata2 = synvbuffer1[2][i];
		keyvalue_t udata3 = synvbuffer1[3][i];
		keyvalue_t udata4 = synvbuffer1[4][i];
		keyvalue_t udata5 = synvbuffer1[5][i];
		keyvalue_t udata6 = synvbuffer1[6][i];
		keyvalue_t udata7 = synvbuffer1[7][i];
		keyvalue_t udata8 = synvbuffer2[0][i];
		keyvalue_t udata9 = synvbuffer2[1][i];
		keyvalue_t udata10 = synvbuffer2[2][i];
		keyvalue_t udata11 = synvbuffer2[3][i];
		keyvalue_t udata12 = synvbuffer2[4][i];
		keyvalue_t udata13 = synvbuffer2[5][i];
		keyvalue_t udata14 = synvbuffer2[6][i];
		keyvalue_t udata15 = synvbuffer2[7][i];
		
		data0.key = mergefunc_4to1(data0_0.key,data0_1.key,data0_2.key,data0_3.key, globalparams.GraphAlgo);
		data1.key = mergefunc_4to1(data1_0.key,data1_1.key,data1_2.key,data1_3.key, globalparams.GraphAlgo);
		data2.key = mergefunc_4to1(data2_0.key,data2_1.key,data2_2.key,data2_3.key, globalparams.GraphAlgo);
		data3.key = mergefunc_4to1(data3_0.key,data3_1.key,data3_2.key,data3_3.key, globalparams.GraphAlgo);
		data4.key = mergefunc_4to1(data4_0.key,data4_1.key,data4_2.key,data4_3.key, globalparams.GraphAlgo);
		data5.key = mergefunc_4to1(data5_0.key,data5_1.key,data5_2.key,data5_3.key, globalparams.GraphAlgo);
		data6.key = mergefunc_4to1(data6_0.key,data6_1.key,data6_2.key,data6_3.key, globalparams.GraphAlgo);
		data7.key = mergefunc_4to1(data7_0.key,data7_1.key,data7_2.key,data7_3.key, globalparams.GraphAlgo);
		data8.key = mergefunc_4to1(data8_0.key,data8_1.key,data8_2.key,data8_3.key, globalparams.GraphAlgo);
		data9.key = mergefunc_4to1(data9_0.key,data9_1.key,data9_2.key,data9_3.key, globalparams.GraphAlgo);
		data10.key = mergefunc_4to1(data10_0.key,data10_1.key,data10_2.key,data10_3.key, globalparams.GraphAlgo);
		data11.key = mergefunc_4to1(data11_0.key,data11_1.key,data11_2.key,data11_3.key, globalparams.GraphAlgo);
		data12.key = mergefunc_4to1(data12_0.key,data12_1.key,data12_2.key,data12_3.key, globalparams.GraphAlgo);
		data13.key = mergefunc_4to1(data13_0.key,data13_1.key,data13_2.key,data13_3.key, globalparams.GraphAlgo);
		data14.key = mergefunc_4to1(data14_0.key,data14_1.key,data14_2.key,data14_3.key, globalparams.GraphAlgo);
		data15.key = mergefunc_4to1(data15_0.key,data15_1.key,data15_2.key,data15_3.key, globalparams.GraphAlgo);
		data0.value = mergefunc_4to1(data0_0.value,data0_1.value,data0_2.value,data0_3.value, globalparams.GraphAlgo);
		data1.value = mergefunc_4to1(data1_0.value,data1_1.value,data1_2.value,data1_3.value, globalparams.GraphAlgo);
		data2.value = mergefunc_4to1(data2_0.value,data2_1.value,data2_2.value,data2_3.value, globalparams.GraphAlgo);
		data3.value = mergefunc_4to1(data3_0.value,data3_1.value,data3_2.value,data3_3.value, globalparams.GraphAlgo);
		data4.value = mergefunc_4to1(data4_0.value,data4_1.value,data4_2.value,data4_3.value, globalparams.GraphAlgo);
		data5.value = mergefunc_4to1(data5_0.value,data5_1.value,data5_2.value,data5_3.value, globalparams.GraphAlgo);
		data6.value = mergefunc_4to1(data6_0.value,data6_1.value,data6_2.value,data6_3.value, globalparams.GraphAlgo);
		data7.value = mergefunc_4to1(data7_0.value,data7_1.value,data7_2.value,data7_3.value, globalparams.GraphAlgo);
		data8.value = mergefunc_4to1(data8_0.value,data8_1.value,data8_2.value,data8_3.value, globalparams.GraphAlgo);
		data9.value = mergefunc_4to1(data9_0.value,data9_1.value,data9_2.value,data9_3.value, globalparams.GraphAlgo);
		data10.value = mergefunc_4to1(data10_0.value,data10_1.value,data10_2.value,data10_3.value, globalparams.GraphAlgo);
		data11.value = mergefunc_4to1(data11_0.value,data11_1.value,data11_2.value,data11_3.value, globalparams.GraphAlgo);
		data12.value = mergefunc_4to1(data12_0.value,data12_1.value,data12_2.value,data12_3.value, globalparams.GraphAlgo);
		data13.value = mergefunc_4to1(data13_0.value,data13_1.value,data13_2.value,data13_3.value, globalparams.GraphAlgo);
		data14.value = mergefunc_4to1(data14_0.value,data14_1.value,data14_2.value,data14_3.value, globalparams.GraphAlgo);
		data15.value = mergefunc_4to1(data15_0.value,data15_1.value,data15_2.value,data15_3.value, globalparams.GraphAlgo);
		
		res0.key = applyfunc(udata0.key, data0.key, globalparams.GraphIter, globalparams.GraphAlgo);
		res1.key = applyfunc(udata1.key, data1.key, globalparams.GraphIter, globalparams.GraphAlgo);
		res2.key = applyfunc(udata2.key, data2.key, globalparams.GraphIter, globalparams.GraphAlgo);
		res3.key = applyfunc(udata3.key, data3.key, globalparams.GraphIter, globalparams.GraphAlgo);
		res4.key = applyfunc(udata4.key, data4.key, globalparams.GraphIter, globalparams.GraphAlgo);
		res5.key = applyfunc(udata5.key, data5.key, globalparams.GraphIter, globalparams.GraphAlgo);
		res6.key = applyfunc(udata6.key, data6.key, globalparams.GraphIter, globalparams.GraphAlgo);
		res7.key = applyfunc(udata7.key, data7.key, globalparams.GraphIter, globalparams.GraphAlgo);
		res8.key = applyfunc(udata8.key, data8.key, globalparams.GraphIter, globalparams.GraphAlgo);
		res9.key = applyfunc(udata9.key, data9.key, globalparams.GraphIter, globalparams.GraphAlgo);
		res10.key = applyfunc(udata10.key, data10.key, globalparams.GraphIter, globalparams.GraphAlgo);
		res11.key = applyfunc(udata11.key, data11.key, globalparams.GraphIter, globalparams.GraphAlgo);
		res12.key = applyfunc(udata12.key, data12.key, globalparams.GraphIter, globalparams.GraphAlgo);
		res13.key = applyfunc(udata13.key, data13.key, globalparams.GraphIter, globalparams.GraphAlgo);
		res14.key = applyfunc(udata14.key, data14.key, globalparams.GraphIter, globalparams.GraphAlgo);
		res15.key = applyfunc(udata15.key, data15.key, globalparams.GraphIter, globalparams.GraphAlgo);
		res0.value = applyfunc(udata0.value, data0.value, globalparams.GraphIter, globalparams.GraphAlgo);
		res1.value = applyfunc(udata1.value, data1.value, globalparams.GraphIter, globalparams.GraphAlgo);
		res2.value = applyfunc(udata2.value, data2.value, globalparams.GraphIter, globalparams.GraphAlgo);
		res3.value = applyfunc(udata3.value, data3.value, globalparams.GraphIter, globalparams.GraphAlgo);
		res4.value = applyfunc(udata4.value, data4.value, globalparams.GraphIter, globalparams.GraphAlgo);
		res5.value = applyfunc(udata5.value, data5.value, globalparams.GraphIter, globalparams.GraphAlgo);
		res6.value = applyfunc(udata6.value, data6.value, globalparams.GraphIter, globalparams.GraphAlgo);
		res7.value = applyfunc(udata7.value, data7.value, globalparams.GraphIter, globalparams.GraphAlgo);
		res8.value = applyfunc(udata8.value, data8.value, globalparams.GraphIter, globalparams.GraphAlgo);
		res9.value = applyfunc(udata9.value, data9.value, globalparams.GraphIter, globalparams.GraphAlgo);
		res10.value = applyfunc(udata10.value, data10.value, globalparams.GraphIter, globalparams.GraphAlgo);
		res11.value = applyfunc(udata11.value, data11.value, globalparams.GraphIter, globalparams.GraphAlgo);
		res12.value = applyfunc(udata12.value, data12.value, globalparams.GraphIter, globalparams.GraphAlgo);
		res13.value = applyfunc(udata13.value, data13.value, globalparams.GraphIter, globalparams.GraphAlgo);
		res14.value = applyfunc(udata14.value, data14.value, globalparams.GraphIter, globalparams.GraphAlgo);
		res15.value = applyfunc(udata15.value, data15.value, globalparams.GraphIter, globalparams.GraphAlgo);
		
	
		synvbuffer1[0][i] = res0;	
	
		synvbuffer1[1][i] = res1;	
	
		synvbuffer1[2][i] = res2;	
	
		synvbuffer1[3][i] = res3;	
	
		synvbuffer1[4][i] = res4;	
	
		synvbuffer1[5][i] = res5;	
	
		synvbuffer1[6][i] = res6;	
	
		synvbuffer1[7][i] = res7;	
	
		synvbuffer2[0][i] = res8;	
	
		synvbuffer2[1][i] = res9;	
	
		synvbuffer2[2][i] = res10;	
	
		synvbuffer2[3][i] = res11;	
	
		synvbuffer2[4][i] = res12;	
	
		synvbuffer2[5][i] = res13;	
	
		synvbuffer2[6][i] = res14;	
	
		synvbuffer2[7][i] = res15;	
	
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
		
		#ifdef _DEBUGMODE_KERNELPRINTS // _DEBUGMODE_KERNELPRINTS3
		if(false){ cout<<"SYNCHRONIZE FUNC SEEN @ (vid1: "<<vid[0][0]<<" & vid2: "<<vid[0][1]<<"): res0.key: "<<res0.key<<", res0.value: "<<res0.value<<": udata0.key: "<<udata0.key<<", udata0.value: "<<udata0.value<<", data0.key: "<<data0.key<<", data0.value: "<<data0.value<<endl; }
		if(res0.key != udata0.key){ cout<<"synchronize: vid: "<<vid[0][0]<<", vmask["<<i<<"].data[0].key: "<<vmask[i].data[0].key<<endl; }
		if(res0.value != udata0.value){ cout<<"synchronize: vid: "<<vid[0][1]<<", vmask["<<i<<"].data[0].value: "<<vmask[i].data[0].value<<endl; }
		if(false){ cout<<"SYNCHRONIZE FUNC SEEN @ (vid1: "<<vid[1][0]<<" & vid2: "<<vid[1][1]<<"): res1.key: "<<res1.key<<", res1.value: "<<res1.value<<": udata1.key: "<<udata1.key<<", udata1.value: "<<udata1.value<<", data1.key: "<<data1.key<<", data1.value: "<<data1.value<<endl; }
		if(res1.key != udata1.key){ cout<<"synchronize: vid: "<<vid[1][0]<<", vmask["<<i<<"].data[1].key: "<<vmask[i].data[1].key<<endl; }
		if(res1.value != udata1.value){ cout<<"synchronize: vid: "<<vid[1][1]<<", vmask["<<i<<"].data[1].value: "<<vmask[i].data[1].value<<endl; }
		if(false){ cout<<"SYNCHRONIZE FUNC SEEN @ (vid1: "<<vid[2][0]<<" & vid2: "<<vid[2][1]<<"): res2.key: "<<res2.key<<", res2.value: "<<res2.value<<": udata2.key: "<<udata2.key<<", udata2.value: "<<udata2.value<<", data2.key: "<<data2.key<<", data2.value: "<<data2.value<<endl; }
		if(res2.key != udata2.key){ cout<<"synchronize: vid: "<<vid[2][0]<<", vmask["<<i<<"].data[2].key: "<<vmask[i].data[2].key<<endl; }
		if(res2.value != udata2.value){ cout<<"synchronize: vid: "<<vid[2][1]<<", vmask["<<i<<"].data[2].value: "<<vmask[i].data[2].value<<endl; }
		if(false){ cout<<"SYNCHRONIZE FUNC SEEN @ (vid1: "<<vid[3][0]<<" & vid2: "<<vid[3][1]<<"): res3.key: "<<res3.key<<", res3.value: "<<res3.value<<": udata3.key: "<<udata3.key<<", udata3.value: "<<udata3.value<<", data3.key: "<<data3.key<<", data3.value: "<<data3.value<<endl; }
		if(res3.key != udata3.key){ cout<<"synchronize: vid: "<<vid[3][0]<<", vmask["<<i<<"].data[3].key: "<<vmask[i].data[3].key<<endl; }
		if(res3.value != udata3.value){ cout<<"synchronize: vid: "<<vid[3][1]<<", vmask["<<i<<"].data[3].value: "<<vmask[i].data[3].value<<endl; }
		if(false){ cout<<"SYNCHRONIZE FUNC SEEN @ (vid1: "<<vid[4][0]<<" & vid2: "<<vid[4][1]<<"): res4.key: "<<res4.key<<", res4.value: "<<res4.value<<": udata4.key: "<<udata4.key<<", udata4.value: "<<udata4.value<<", data4.key: "<<data4.key<<", data4.value: "<<data4.value<<endl; }
		if(res4.key != udata4.key){ cout<<"synchronize: vid: "<<vid[4][0]<<", vmask["<<i<<"].data[4].key: "<<vmask[i].data[4].key<<endl; }
		if(res4.value != udata4.value){ cout<<"synchronize: vid: "<<vid[4][1]<<", vmask["<<i<<"].data[4].value: "<<vmask[i].data[4].value<<endl; }
		if(false){ cout<<"SYNCHRONIZE FUNC SEEN @ (vid1: "<<vid[5][0]<<" & vid2: "<<vid[5][1]<<"): res5.key: "<<res5.key<<", res5.value: "<<res5.value<<": udata5.key: "<<udata5.key<<", udata5.value: "<<udata5.value<<", data5.key: "<<data5.key<<", data5.value: "<<data5.value<<endl; }
		if(res5.key != udata5.key){ cout<<"synchronize: vid: "<<vid[5][0]<<", vmask["<<i<<"].data[5].key: "<<vmask[i].data[5].key<<endl; }
		if(res5.value != udata5.value){ cout<<"synchronize: vid: "<<vid[5][1]<<", vmask["<<i<<"].data[5].value: "<<vmask[i].data[5].value<<endl; }
		if(false){ cout<<"SYNCHRONIZE FUNC SEEN @ (vid1: "<<vid[6][0]<<" & vid2: "<<vid[6][1]<<"): res6.key: "<<res6.key<<", res6.value: "<<res6.value<<": udata6.key: "<<udata6.key<<", udata6.value: "<<udata6.value<<", data6.key: "<<data6.key<<", data6.value: "<<data6.value<<endl; }
		if(res6.key != udata6.key){ cout<<"synchronize: vid: "<<vid[6][0]<<", vmask["<<i<<"].data[6].key: "<<vmask[i].data[6].key<<endl; }
		if(res6.value != udata6.value){ cout<<"synchronize: vid: "<<vid[6][1]<<", vmask["<<i<<"].data[6].value: "<<vmask[i].data[6].value<<endl; }
		if(false){ cout<<"SYNCHRONIZE FUNC SEEN @ (vid1: "<<vid[7][0]<<" & vid2: "<<vid[7][1]<<"): res7.key: "<<res7.key<<", res7.value: "<<res7.value<<": udata7.key: "<<udata7.key<<", udata7.value: "<<udata7.value<<", data7.key: "<<data7.key<<", data7.value: "<<data7.value<<endl; }
		if(res7.key != udata7.key){ cout<<"synchronize: vid: "<<vid[7][0]<<", vmask["<<i<<"].data[7].key: "<<vmask[i].data[7].key<<endl; }
		if(res7.value != udata7.value){ cout<<"synchronize: vid: "<<vid[7][1]<<", vmask["<<i<<"].data[7].value: "<<vmask[i].data[7].value<<endl; }
		if(false){ cout<<"SYNCHRONIZE FUNC SEEN @ (vid1: "<<vid[8][0]<<" & vid2: "<<vid[8][1]<<"): res8.key: "<<res8.key<<", res8.value: "<<res8.value<<": udata8.key: "<<udata8.key<<", udata8.value: "<<udata8.value<<", data8.key: "<<data8.key<<", data8.value: "<<data8.value<<endl; }
		if(res8.key != udata8.key){ cout<<"synchronize: vid: "<<vid[8][0]<<", vmask["<<i<<"].data[8].key: "<<vmask[i].data[8].key<<endl; }
		if(res8.value != udata8.value){ cout<<"synchronize: vid: "<<vid[8][1]<<", vmask["<<i<<"].data[8].value: "<<vmask[i].data[8].value<<endl; }
		if(false){ cout<<"SYNCHRONIZE FUNC SEEN @ (vid1: "<<vid[9][0]<<" & vid2: "<<vid[9][1]<<"): res9.key: "<<res9.key<<", res9.value: "<<res9.value<<": udata9.key: "<<udata9.key<<", udata9.value: "<<udata9.value<<", data9.key: "<<data9.key<<", data9.value: "<<data9.value<<endl; }
		if(res9.key != udata9.key){ cout<<"synchronize: vid: "<<vid[9][0]<<", vmask["<<i<<"].data[9].key: "<<vmask[i].data[9].key<<endl; }
		if(res9.value != udata9.value){ cout<<"synchronize: vid: "<<vid[9][1]<<", vmask["<<i<<"].data[9].value: "<<vmask[i].data[9].value<<endl; }
		if(false){ cout<<"SYNCHRONIZE FUNC SEEN @ (vid1: "<<vid[10][0]<<" & vid2: "<<vid[10][1]<<"): res10.key: "<<res10.key<<", res10.value: "<<res10.value<<": udata10.key: "<<udata10.key<<", udata10.value: "<<udata10.value<<", data10.key: "<<data10.key<<", data10.value: "<<data10.value<<endl; }
		if(res10.key != udata10.key){ cout<<"synchronize: vid: "<<vid[10][0]<<", vmask["<<i<<"].data[10].key: "<<vmask[i].data[10].key<<endl; }
		if(res10.value != udata10.value){ cout<<"synchronize: vid: "<<vid[10][1]<<", vmask["<<i<<"].data[10].value: "<<vmask[i].data[10].value<<endl; }
		if(false){ cout<<"SYNCHRONIZE FUNC SEEN @ (vid1: "<<vid[11][0]<<" & vid2: "<<vid[11][1]<<"): res11.key: "<<res11.key<<", res11.value: "<<res11.value<<": udata11.key: "<<udata11.key<<", udata11.value: "<<udata11.value<<", data11.key: "<<data11.key<<", data11.value: "<<data11.value<<endl; }
		if(res11.key != udata11.key){ cout<<"synchronize: vid: "<<vid[11][0]<<", vmask["<<i<<"].data[11].key: "<<vmask[i].data[11].key<<endl; }
		if(res11.value != udata11.value){ cout<<"synchronize: vid: "<<vid[11][1]<<", vmask["<<i<<"].data[11].value: "<<vmask[i].data[11].value<<endl; }
		if(false){ cout<<"SYNCHRONIZE FUNC SEEN @ (vid1: "<<vid[12][0]<<" & vid2: "<<vid[12][1]<<"): res12.key: "<<res12.key<<", res12.value: "<<res12.value<<": udata12.key: "<<udata12.key<<", udata12.value: "<<udata12.value<<", data12.key: "<<data12.key<<", data12.value: "<<data12.value<<endl; }
		if(res12.key != udata12.key){ cout<<"synchronize: vid: "<<vid[12][0]<<", vmask["<<i<<"].data[12].key: "<<vmask[i].data[12].key<<endl; }
		if(res12.value != udata12.value){ cout<<"synchronize: vid: "<<vid[12][1]<<", vmask["<<i<<"].data[12].value: "<<vmask[i].data[12].value<<endl; }
		if(false){ cout<<"SYNCHRONIZE FUNC SEEN @ (vid1: "<<vid[13][0]<<" & vid2: "<<vid[13][1]<<"): res13.key: "<<res13.key<<", res13.value: "<<res13.value<<": udata13.key: "<<udata13.key<<", udata13.value: "<<udata13.value<<", data13.key: "<<data13.key<<", data13.value: "<<data13.value<<endl; }
		if(res13.key != udata13.key){ cout<<"synchronize: vid: "<<vid[13][0]<<", vmask["<<i<<"].data[13].key: "<<vmask[i].data[13].key<<endl; }
		if(res13.value != udata13.value){ cout<<"synchronize: vid: "<<vid[13][1]<<", vmask["<<i<<"].data[13].value: "<<vmask[i].data[13].value<<endl; }
		if(false){ cout<<"SYNCHRONIZE FUNC SEEN @ (vid1: "<<vid[14][0]<<" & vid2: "<<vid[14][1]<<"): res14.key: "<<res14.key<<", res14.value: "<<res14.value<<": udata14.key: "<<udata14.key<<", udata14.value: "<<udata14.value<<", data14.key: "<<data14.key<<", data14.value: "<<data14.value<<endl; }
		if(res14.key != udata14.key){ cout<<"synchronize: vid: "<<vid[14][0]<<", vmask["<<i<<"].data[14].key: "<<vmask[i].data[14].key<<endl; }
		if(res14.value != udata14.value){ cout<<"synchronize: vid: "<<vid[14][1]<<", vmask["<<i<<"].data[14].value: "<<vmask[i].data[14].value<<endl; }
		if(false){ cout<<"SYNCHRONIZE FUNC SEEN @ (vid1: "<<vid[15][0]<<" & vid2: "<<vid[15][1]<<"): res15.key: "<<res15.key<<", res15.value: "<<res15.value<<": udata15.key: "<<udata15.key<<", udata15.value: "<<udata15.value<<", data15.key: "<<data15.key<<", data15.value: "<<data15.value<<endl; }
		if(res15.key != udata15.key){ cout<<"synchronize: vid: "<<vid[15][0]<<", vmask["<<i<<"].data[15].key: "<<vmask[i].data[15].key<<endl; }
		if(res15.value != udata15.value){ cout<<"synchronize: vid: "<<vid[15][1]<<", vmask["<<i<<"].data[15].value: "<<vmask[i].data[15].value<<endl; }
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
	
	// get mask
	SYNCHRONIZE_LOOP3: for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){
	#pragma HLS PIPELINE II=1
		uint32_type mask = convertvmasktouint32(vmask, k);
		if(mask > 0){ partitionisactive = ON; }
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printkeyvalues("synchronize.synvbuffer1", synvbuffer1, NUM_VBUFFERS, 4);
	actsutilityobj->printkeyvalues("synchronize.vmask", vmask, NUM_PARTITIONS, 4);
	#endif
	return partitionisactive;
}

bool_type 
	#ifdef SW 
	acts::
	#endif
preplicate_1to4(bool_type enable,
		keyvalue_t synvbuffer1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t synvbuffer2[VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type synvmask[BLOCKRAM_SIZE], 
keyvalue_t vbuffer0_1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t vbuffer0_2[VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask0[BLOCKRAM_SIZE],keyvalue_t vbuffer1_1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t vbuffer1_2[VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask1[BLOCKRAM_SIZE],keyvalue_t vbuffer2_1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t vbuffer2_2[VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask2[BLOCKRAM_SIZE],keyvalue_t vbuffer3_1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t vbuffer3_2[VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask3[BLOCKRAM_SIZE], 
		globalparams_t globalparams){
	if(enable == OFF){ return OFF; }
	
	uintNUMPby2_type _vmask[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = _vmask

	// replicate to vbuffers
	SYNCHRONIZE_LOOP2: for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){
	#pragma HLS PIPELINE II=1
		vbuffer0_1[0][k] = synvbuffer1[0][k];
		vbuffer0_1[1][k] = synvbuffer1[1][k];
		vbuffer0_1[2][k] = synvbuffer1[2][k];
		vbuffer0_1[3][k] = synvbuffer1[3][k];
		vbuffer0_1[4][k] = synvbuffer1[4][k];
		vbuffer0_1[5][k] = synvbuffer1[5][k];
		vbuffer0_1[6][k] = synvbuffer1[6][k];
		vbuffer0_1[7][k] = synvbuffer1[7][k];
		vbuffer1_1[0][k] = synvbuffer1[0][k];
		vbuffer1_1[1][k] = synvbuffer1[1][k];
		vbuffer1_1[2][k] = synvbuffer1[2][k];
		vbuffer1_1[3][k] = synvbuffer1[3][k];
		vbuffer1_1[4][k] = synvbuffer1[4][k];
		vbuffer1_1[5][k] = synvbuffer1[5][k];
		vbuffer1_1[6][k] = synvbuffer1[6][k];
		vbuffer1_1[7][k] = synvbuffer1[7][k];
		vbuffer2_1[0][k] = synvbuffer1[0][k];
		vbuffer2_1[1][k] = synvbuffer1[1][k];
		vbuffer2_1[2][k] = synvbuffer1[2][k];
		vbuffer2_1[3][k] = synvbuffer1[3][k];
		vbuffer2_1[4][k] = synvbuffer1[4][k];
		vbuffer2_1[5][k] = synvbuffer1[5][k];
		vbuffer2_1[6][k] = synvbuffer1[6][k];
		vbuffer2_1[7][k] = synvbuffer1[7][k];
		vbuffer3_1[0][k] = synvbuffer1[0][k];
		vbuffer3_1[1][k] = synvbuffer1[1][k];
		vbuffer3_1[2][k] = synvbuffer1[2][k];
		vbuffer3_1[3][k] = synvbuffer1[3][k];
		vbuffer3_1[4][k] = synvbuffer1[4][k];
		vbuffer3_1[5][k] = synvbuffer1[5][k];
		vbuffer3_1[6][k] = synvbuffer1[6][k];
		vbuffer3_1[7][k] = synvbuffer1[7][k];
		vbuffer0_2[0][k] = synvbuffer2[0][k];
		vbuffer0_2[1][k] = synvbuffer2[1][k];
		vbuffer0_2[2][k] = synvbuffer2[2][k];
		vbuffer0_2[3][k] = synvbuffer2[3][k];
		vbuffer0_2[4][k] = synvbuffer2[4][k];
		vbuffer0_2[5][k] = synvbuffer2[5][k];
		vbuffer0_2[6][k] = synvbuffer2[6][k];
		vbuffer0_2[7][k] = synvbuffer2[7][k];
		vbuffer1_2[0][k] = synvbuffer2[0][k];
		vbuffer1_2[1][k] = synvbuffer2[1][k];
		vbuffer1_2[2][k] = synvbuffer2[2][k];
		vbuffer1_2[3][k] = synvbuffer2[3][k];
		vbuffer1_2[4][k] = synvbuffer2[4][k];
		vbuffer1_2[5][k] = synvbuffer2[5][k];
		vbuffer1_2[6][k] = synvbuffer2[6][k];
		vbuffer1_2[7][k] = synvbuffer2[7][k];
		vbuffer2_2[0][k] = synvbuffer2[0][k];
		vbuffer2_2[1][k] = synvbuffer2[1][k];
		vbuffer2_2[2][k] = synvbuffer2[2][k];
		vbuffer2_2[3][k] = synvbuffer2[3][k];
		vbuffer2_2[4][k] = synvbuffer2[4][k];
		vbuffer2_2[5][k] = synvbuffer2[5][k];
		vbuffer2_2[6][k] = synvbuffer2[6][k];
		vbuffer2_2[7][k] = synvbuffer2[7][k];
		vbuffer3_2[0][k] = synvbuffer2[0][k];
		vbuffer3_2[1][k] = synvbuffer2[1][k];
		vbuffer3_2[2][k] = synvbuffer2[2][k];
		vbuffer3_2[3][k] = synvbuffer2[3][k];
		vbuffer3_2[4][k] = synvbuffer2[4][k];
		vbuffer3_2[5][k] = synvbuffer2[5][k];
		vbuffer3_2[6][k] = synvbuffer2[6][k];
		vbuffer3_2[7][k] = synvbuffer2[7][k];
	}
	
	// replicate to vmasks
	SYNCHRONIZE_LOOP3: for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){
	#pragma HLS PIPELINE II=1
		vmask0[k] = synvmask[k];
		vmask1[k] = synvmask[k];
		vmask2[k] = synvmask[k];
		vmask3[k] = synvmask[k];
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printkeyvalues("synchronize.synvbuffer1", synvbuffer1, NUM_VBUFFERS, 4);
	actsutilityobj->printkeyvalues("synchronize.synvmask", synvmask, NUM_PARTITIONS, 4);
	#endif
	return NAp;
}
 */
bool_type 
	#ifdef SW 
	acts::
	#endif
synchronize(bool_type enable, keyvalue_t synvbuffer[NUM_VBUFFERS][BLOCKRAM_SIZE], keyvalue_t vbuffer0_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer0_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], uintNUMPby2_type vmask0[BLOCKRAM_SIZE],keyvalue_t vbuffer1_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer1_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], uintNUMPby2_type vmask1[BLOCKRAM_SIZE],keyvalue_t vbuffer2_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer2_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], uintNUMPby2_type vmask2[BLOCKRAM_SIZE],keyvalue_t vbuffer3_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer3_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], uintNUMPby2_type vmask3[BLOCKRAM_SIZE],keyvalue_t vbuffer4_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer4_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], uintNUMPby2_type vmask4[BLOCKRAM_SIZE],keyvalue_t vbuffer5_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer5_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], uintNUMPby2_type vmask5[BLOCKRAM_SIZE],keyvalue_t vbuffer6_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer6_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], uintNUMPby2_type vmask6[BLOCKRAM_SIZE],keyvalue_t vbuffer7_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer7_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], uintNUMPby2_type vmask7[BLOCKRAM_SIZE],keyvalue_t vbuffer8_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer8_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], uintNUMPby2_type vmask8[BLOCKRAM_SIZE],keyvalue_t vbuffer9_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer9_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], uintNUMPby2_type vmask9[BLOCKRAM_SIZE],keyvalue_t vbuffer10_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer10_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], uintNUMPby2_type vmask10[BLOCKRAM_SIZE],keyvalue_t vbuffer11_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer11_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], uintNUMPby2_type vmask11[BLOCKRAM_SIZE],keyvalue_t vbuffer12_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer12_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], uintNUMPby2_type vmask12[BLOCKRAM_SIZE],keyvalue_t vbuffer13_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer13_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], uintNUMPby2_type vmask13[BLOCKRAM_SIZE],keyvalue_t vbuffer14_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer14_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], uintNUMPby2_type vmask14[BLOCKRAM_SIZE],keyvalue_t vbuffer15_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer15_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], uintNUMPby2_type vmask15[BLOCKRAM_SIZE], globalparams_t globalparams){
	if(enable == OFF){ return OFF; }
	
	uintNUMPby2_type _vmask[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = _vmask
	
	unsigned int vid[NUM_PARTITIONS][2];
	#pragma HLS ARRAY_PARTITION variable=vid complete
	
	keyvalue_t data0;
	keyvalue_t res0;
	keyvalue_t data0_0;
	keyvalue_t data0_1;
	keyvalue_t data0_2;
	keyvalue_t data0_3;
	keyvalue_t data0_4;
	keyvalue_t data0_5;
	keyvalue_t data0_6;
	keyvalue_t data0_7;
	keyvalue_t data0_8;
	keyvalue_t data0_9;
	keyvalue_t data0_10;
	keyvalue_t data0_11;
	keyvalue_t data0_12;
	keyvalue_t data0_13;
	keyvalue_t data0_14;
	keyvalue_t data0_15;
	keyvalue_t data1;
	keyvalue_t res1;
	keyvalue_t data1_0;
	keyvalue_t data1_1;
	keyvalue_t data1_2;
	keyvalue_t data1_3;
	keyvalue_t data1_4;
	keyvalue_t data1_5;
	keyvalue_t data1_6;
	keyvalue_t data1_7;
	keyvalue_t data1_8;
	keyvalue_t data1_9;
	keyvalue_t data1_10;
	keyvalue_t data1_11;
	keyvalue_t data1_12;
	keyvalue_t data1_13;
	keyvalue_t data1_14;
	keyvalue_t data1_15;
	keyvalue_t data2;
	keyvalue_t res2;
	keyvalue_t data2_0;
	keyvalue_t data2_1;
	keyvalue_t data2_2;
	keyvalue_t data2_3;
	keyvalue_t data2_4;
	keyvalue_t data2_5;
	keyvalue_t data2_6;
	keyvalue_t data2_7;
	keyvalue_t data2_8;
	keyvalue_t data2_9;
	keyvalue_t data2_10;
	keyvalue_t data2_11;
	keyvalue_t data2_12;
	keyvalue_t data2_13;
	keyvalue_t data2_14;
	keyvalue_t data2_15;
	keyvalue_t data3;
	keyvalue_t res3;
	keyvalue_t data3_0;
	keyvalue_t data3_1;
	keyvalue_t data3_2;
	keyvalue_t data3_3;
	keyvalue_t data3_4;
	keyvalue_t data3_5;
	keyvalue_t data3_6;
	keyvalue_t data3_7;
	keyvalue_t data3_8;
	keyvalue_t data3_9;
	keyvalue_t data3_10;
	keyvalue_t data3_11;
	keyvalue_t data3_12;
	keyvalue_t data3_13;
	keyvalue_t data3_14;
	keyvalue_t data3_15;
	keyvalue_t data4;
	keyvalue_t res4;
	keyvalue_t data4_0;
	keyvalue_t data4_1;
	keyvalue_t data4_2;
	keyvalue_t data4_3;
	keyvalue_t data4_4;
	keyvalue_t data4_5;
	keyvalue_t data4_6;
	keyvalue_t data4_7;
	keyvalue_t data4_8;
	keyvalue_t data4_9;
	keyvalue_t data4_10;
	keyvalue_t data4_11;
	keyvalue_t data4_12;
	keyvalue_t data4_13;
	keyvalue_t data4_14;
	keyvalue_t data4_15;
	keyvalue_t data5;
	keyvalue_t res5;
	keyvalue_t data5_0;
	keyvalue_t data5_1;
	keyvalue_t data5_2;
	keyvalue_t data5_3;
	keyvalue_t data5_4;
	keyvalue_t data5_5;
	keyvalue_t data5_6;
	keyvalue_t data5_7;
	keyvalue_t data5_8;
	keyvalue_t data5_9;
	keyvalue_t data5_10;
	keyvalue_t data5_11;
	keyvalue_t data5_12;
	keyvalue_t data5_13;
	keyvalue_t data5_14;
	keyvalue_t data5_15;
	keyvalue_t data6;
	keyvalue_t res6;
	keyvalue_t data6_0;
	keyvalue_t data6_1;
	keyvalue_t data6_2;
	keyvalue_t data6_3;
	keyvalue_t data6_4;
	keyvalue_t data6_5;
	keyvalue_t data6_6;
	keyvalue_t data6_7;
	keyvalue_t data6_8;
	keyvalue_t data6_9;
	keyvalue_t data6_10;
	keyvalue_t data6_11;
	keyvalue_t data6_12;
	keyvalue_t data6_13;
	keyvalue_t data6_14;
	keyvalue_t data6_15;
	keyvalue_t data7;
	keyvalue_t res7;
	keyvalue_t data7_0;
	keyvalue_t data7_1;
	keyvalue_t data7_2;
	keyvalue_t data7_3;
	keyvalue_t data7_4;
	keyvalue_t data7_5;
	keyvalue_t data7_6;
	keyvalue_t data7_7;
	keyvalue_t data7_8;
	keyvalue_t data7_9;
	keyvalue_t data7_10;
	keyvalue_t data7_11;
	keyvalue_t data7_12;
	keyvalue_t data7_13;
	keyvalue_t data7_14;
	keyvalue_t data7_15;
	keyvalue_t data8;
	keyvalue_t res8;
	keyvalue_t data8_0;
	keyvalue_t data8_1;
	keyvalue_t data8_2;
	keyvalue_t data8_3;
	keyvalue_t data8_4;
	keyvalue_t data8_5;
	keyvalue_t data8_6;
	keyvalue_t data8_7;
	keyvalue_t data8_8;
	keyvalue_t data8_9;
	keyvalue_t data8_10;
	keyvalue_t data8_11;
	keyvalue_t data8_12;
	keyvalue_t data8_13;
	keyvalue_t data8_14;
	keyvalue_t data8_15;
	keyvalue_t data9;
	keyvalue_t res9;
	keyvalue_t data9_0;
	keyvalue_t data9_1;
	keyvalue_t data9_2;
	keyvalue_t data9_3;
	keyvalue_t data9_4;
	keyvalue_t data9_5;
	keyvalue_t data9_6;
	keyvalue_t data9_7;
	keyvalue_t data9_8;
	keyvalue_t data9_9;
	keyvalue_t data9_10;
	keyvalue_t data9_11;
	keyvalue_t data9_12;
	keyvalue_t data9_13;
	keyvalue_t data9_14;
	keyvalue_t data9_15;
	keyvalue_t data10;
	keyvalue_t res10;
	keyvalue_t data10_0;
	keyvalue_t data10_1;
	keyvalue_t data10_2;
	keyvalue_t data10_3;
	keyvalue_t data10_4;
	keyvalue_t data10_5;
	keyvalue_t data10_6;
	keyvalue_t data10_7;
	keyvalue_t data10_8;
	keyvalue_t data10_9;
	keyvalue_t data10_10;
	keyvalue_t data10_11;
	keyvalue_t data10_12;
	keyvalue_t data10_13;
	keyvalue_t data10_14;
	keyvalue_t data10_15;
	keyvalue_t data11;
	keyvalue_t res11;
	keyvalue_t data11_0;
	keyvalue_t data11_1;
	keyvalue_t data11_2;
	keyvalue_t data11_3;
	keyvalue_t data11_4;
	keyvalue_t data11_5;
	keyvalue_t data11_6;
	keyvalue_t data11_7;
	keyvalue_t data11_8;
	keyvalue_t data11_9;
	keyvalue_t data11_10;
	keyvalue_t data11_11;
	keyvalue_t data11_12;
	keyvalue_t data11_13;
	keyvalue_t data11_14;
	keyvalue_t data11_15;
	keyvalue_t data12;
	keyvalue_t res12;
	keyvalue_t data12_0;
	keyvalue_t data12_1;
	keyvalue_t data12_2;
	keyvalue_t data12_3;
	keyvalue_t data12_4;
	keyvalue_t data12_5;
	keyvalue_t data12_6;
	keyvalue_t data12_7;
	keyvalue_t data12_8;
	keyvalue_t data12_9;
	keyvalue_t data12_10;
	keyvalue_t data12_11;
	keyvalue_t data12_12;
	keyvalue_t data12_13;
	keyvalue_t data12_14;
	keyvalue_t data12_15;
	keyvalue_t data13;
	keyvalue_t res13;
	keyvalue_t data13_0;
	keyvalue_t data13_1;
	keyvalue_t data13_2;
	keyvalue_t data13_3;
	keyvalue_t data13_4;
	keyvalue_t data13_5;
	keyvalue_t data13_6;
	keyvalue_t data13_7;
	keyvalue_t data13_8;
	keyvalue_t data13_9;
	keyvalue_t data13_10;
	keyvalue_t data13_11;
	keyvalue_t data13_12;
	keyvalue_t data13_13;
	keyvalue_t data13_14;
	keyvalue_t data13_15;
	keyvalue_t data14;
	keyvalue_t res14;
	keyvalue_t data14_0;
	keyvalue_t data14_1;
	keyvalue_t data14_2;
	keyvalue_t data14_3;
	keyvalue_t data14_4;
	keyvalue_t data14_5;
	keyvalue_t data14_6;
	keyvalue_t data14_7;
	keyvalue_t data14_8;
	keyvalue_t data14_9;
	keyvalue_t data14_10;
	keyvalue_t data14_11;
	keyvalue_t data14_12;
	keyvalue_t data14_13;
	keyvalue_t data14_14;
	keyvalue_t data14_15;
	keyvalue_t data15;
	keyvalue_t res15;
	keyvalue_t data15_0;
	keyvalue_t data15_1;
	keyvalue_t data15_2;
	keyvalue_t data15_3;
	keyvalue_t data15_4;
	keyvalue_t data15_5;
	keyvalue_t data15_6;
	keyvalue_t data15_7;
	keyvalue_t data15_8;
	keyvalue_t data15_9;
	keyvalue_t data15_10;
	keyvalue_t data15_11;
	keyvalue_t data15_12;
	keyvalue_t data15_13;
	keyvalue_t data15_14;
	keyvalue_t data15_15;
	
	bool_type partitionisactive = OFF;
	
	// sync
	SYNCHRONIZE_LOOP1: for(buffer_type i=0; i<REDUCEBUFFERSZ; i++){
	#pragma HLS dependence variable=synvbuffer inter false
	#pragma HLS PIPELINE II=1
	
		vid[0][0] = ((0*REDUCESZ) + i*2);
		vid[0][1] = ((0*REDUCESZ) + i*2 + 1);
		vid[1][0] = ((1*REDUCESZ) + i*2);
		vid[1][1] = ((1*REDUCESZ) + i*2 + 1);
		vid[2][0] = ((2*REDUCESZ) + i*2);
		vid[2][1] = ((2*REDUCESZ) + i*2 + 1);
		vid[3][0] = ((3*REDUCESZ) + i*2);
		vid[3][1] = ((3*REDUCESZ) + i*2 + 1);
		vid[4][0] = ((4*REDUCESZ) + i*2);
		vid[4][1] = ((4*REDUCESZ) + i*2 + 1);
		vid[5][0] = ((5*REDUCESZ) + i*2);
		vid[5][1] = ((5*REDUCESZ) + i*2 + 1);
		vid[6][0] = ((6*REDUCESZ) + i*2);
		vid[6][1] = ((6*REDUCESZ) + i*2 + 1);
		vid[7][0] = ((7*REDUCESZ) + i*2);
		vid[7][1] = ((7*REDUCESZ) + i*2 + 1);
		vid[8][0] = ((8*REDUCESZ) + i*2);
		vid[8][1] = ((8*REDUCESZ) + i*2 + 1);
		vid[9][0] = ((9*REDUCESZ) + i*2);
		vid[9][1] = ((9*REDUCESZ) + i*2 + 1);
		vid[10][0] = ((10*REDUCESZ) + i*2);
		vid[10][1] = ((10*REDUCESZ) + i*2 + 1);
		vid[11][0] = ((11*REDUCESZ) + i*2);
		vid[11][1] = ((11*REDUCESZ) + i*2 + 1);
		vid[12][0] = ((12*REDUCESZ) + i*2);
		vid[12][1] = ((12*REDUCESZ) + i*2 + 1);
		vid[13][0] = ((13*REDUCESZ) + i*2);
		vid[13][1] = ((13*REDUCESZ) + i*2 + 1);
		vid[14][0] = ((14*REDUCESZ) + i*2);
		vid[14][1] = ((14*REDUCESZ) + i*2 + 1);
		vid[15][0] = ((15*REDUCESZ) + i*2);
		vid[15][1] = ((15*REDUCESZ) + i*2 + 1);
	
 // AUTOMATEME.
		data0_0 = vbuffer0_1[0][BUFFERBASE_VDATA + i];
		data0_1 = vbuffer1_1[0][BUFFERBASE_VDATA + i];
		data0_2 = vbuffer2_1[0][BUFFERBASE_VDATA + i];
		data0_3 = vbuffer3_1[0][BUFFERBASE_VDATA + i];
		data0_4 = vbuffer4_1[0][BUFFERBASE_VDATA + i];
		data0_5 = vbuffer5_1[0][BUFFERBASE_VDATA + i];
		data0_6 = vbuffer6_1[0][BUFFERBASE_VDATA + i];
		data0_7 = vbuffer7_1[0][BUFFERBASE_VDATA + i];
		data0_8 = vbuffer8_1[0][BUFFERBASE_VDATA + i];
		data0_9 = vbuffer9_1[0][BUFFERBASE_VDATA + i];
		data0_10 = vbuffer10_1[0][BUFFERBASE_VDATA + i];
		data0_11 = vbuffer11_1[0][BUFFERBASE_VDATA + i];
		data0_12 = vbuffer12_1[0][BUFFERBASE_VDATA + i];
		data0_13 = vbuffer13_1[0][BUFFERBASE_VDATA + i];
		data0_14 = vbuffer14_1[0][BUFFERBASE_VDATA + i];
		data0_15 = vbuffer15_1[0][BUFFERBASE_VDATA + i];
 // AUTOMATEME.
		data1_0 = vbuffer0_1[1][BUFFERBASE_VDATA + i];
		data1_1 = vbuffer1_1[1][BUFFERBASE_VDATA + i];
		data1_2 = vbuffer2_1[1][BUFFERBASE_VDATA + i];
		data1_3 = vbuffer3_1[1][BUFFERBASE_VDATA + i];
		data1_4 = vbuffer4_1[1][BUFFERBASE_VDATA + i];
		data1_5 = vbuffer5_1[1][BUFFERBASE_VDATA + i];
		data1_6 = vbuffer6_1[1][BUFFERBASE_VDATA + i];
		data1_7 = vbuffer7_1[1][BUFFERBASE_VDATA + i];
		data1_8 = vbuffer8_1[1][BUFFERBASE_VDATA + i];
		data1_9 = vbuffer9_1[1][BUFFERBASE_VDATA + i];
		data1_10 = vbuffer10_1[1][BUFFERBASE_VDATA + i];
		data1_11 = vbuffer11_1[1][BUFFERBASE_VDATA + i];
		data1_12 = vbuffer12_1[1][BUFFERBASE_VDATA + i];
		data1_13 = vbuffer13_1[1][BUFFERBASE_VDATA + i];
		data1_14 = vbuffer14_1[1][BUFFERBASE_VDATA + i];
		data1_15 = vbuffer15_1[1][BUFFERBASE_VDATA + i];
 // AUTOMATEME.
		data2_0 = vbuffer0_1[2][BUFFERBASE_VDATA + i];
		data2_1 = vbuffer1_1[2][BUFFERBASE_VDATA + i];
		data2_2 = vbuffer2_1[2][BUFFERBASE_VDATA + i];
		data2_3 = vbuffer3_1[2][BUFFERBASE_VDATA + i];
		data2_4 = vbuffer4_1[2][BUFFERBASE_VDATA + i];
		data2_5 = vbuffer5_1[2][BUFFERBASE_VDATA + i];
		data2_6 = vbuffer6_1[2][BUFFERBASE_VDATA + i];
		data2_7 = vbuffer7_1[2][BUFFERBASE_VDATA + i];
		data2_8 = vbuffer8_1[2][BUFFERBASE_VDATA + i];
		data2_9 = vbuffer9_1[2][BUFFERBASE_VDATA + i];
		data2_10 = vbuffer10_1[2][BUFFERBASE_VDATA + i];
		data2_11 = vbuffer11_1[2][BUFFERBASE_VDATA + i];
		data2_12 = vbuffer12_1[2][BUFFERBASE_VDATA + i];
		data2_13 = vbuffer13_1[2][BUFFERBASE_VDATA + i];
		data2_14 = vbuffer14_1[2][BUFFERBASE_VDATA + i];
		data2_15 = vbuffer15_1[2][BUFFERBASE_VDATA + i];
 // AUTOMATEME.
		data3_0 = vbuffer0_1[3][BUFFERBASE_VDATA + i];
		data3_1 = vbuffer1_1[3][BUFFERBASE_VDATA + i];
		data3_2 = vbuffer2_1[3][BUFFERBASE_VDATA + i];
		data3_3 = vbuffer3_1[3][BUFFERBASE_VDATA + i];
		data3_4 = vbuffer4_1[3][BUFFERBASE_VDATA + i];
		data3_5 = vbuffer5_1[3][BUFFERBASE_VDATA + i];
		data3_6 = vbuffer6_1[3][BUFFERBASE_VDATA + i];
		data3_7 = vbuffer7_1[3][BUFFERBASE_VDATA + i];
		data3_8 = vbuffer8_1[3][BUFFERBASE_VDATA + i];
		data3_9 = vbuffer9_1[3][BUFFERBASE_VDATA + i];
		data3_10 = vbuffer10_1[3][BUFFERBASE_VDATA + i];
		data3_11 = vbuffer11_1[3][BUFFERBASE_VDATA + i];
		data3_12 = vbuffer12_1[3][BUFFERBASE_VDATA + i];
		data3_13 = vbuffer13_1[3][BUFFERBASE_VDATA + i];
		data3_14 = vbuffer14_1[3][BUFFERBASE_VDATA + i];
		data3_15 = vbuffer15_1[3][BUFFERBASE_VDATA + i];
 // AUTOMATEME.
		data4_0 = vbuffer0_1[4][BUFFERBASE_VDATA + i];
		data4_1 = vbuffer1_1[4][BUFFERBASE_VDATA + i];
		data4_2 = vbuffer2_1[4][BUFFERBASE_VDATA + i];
		data4_3 = vbuffer3_1[4][BUFFERBASE_VDATA + i];
		data4_4 = vbuffer4_1[4][BUFFERBASE_VDATA + i];
		data4_5 = vbuffer5_1[4][BUFFERBASE_VDATA + i];
		data4_6 = vbuffer6_1[4][BUFFERBASE_VDATA + i];
		data4_7 = vbuffer7_1[4][BUFFERBASE_VDATA + i];
		data4_8 = vbuffer8_1[4][BUFFERBASE_VDATA + i];
		data4_9 = vbuffer9_1[4][BUFFERBASE_VDATA + i];
		data4_10 = vbuffer10_1[4][BUFFERBASE_VDATA + i];
		data4_11 = vbuffer11_1[4][BUFFERBASE_VDATA + i];
		data4_12 = vbuffer12_1[4][BUFFERBASE_VDATA + i];
		data4_13 = vbuffer13_1[4][BUFFERBASE_VDATA + i];
		data4_14 = vbuffer14_1[4][BUFFERBASE_VDATA + i];
		data4_15 = vbuffer15_1[4][BUFFERBASE_VDATA + i];
 // AUTOMATEME.
		data5_0 = vbuffer0_1[5][BUFFERBASE_VDATA + i];
		data5_1 = vbuffer1_1[5][BUFFERBASE_VDATA + i];
		data5_2 = vbuffer2_1[5][BUFFERBASE_VDATA + i];
		data5_3 = vbuffer3_1[5][BUFFERBASE_VDATA + i];
		data5_4 = vbuffer4_1[5][BUFFERBASE_VDATA + i];
		data5_5 = vbuffer5_1[5][BUFFERBASE_VDATA + i];
		data5_6 = vbuffer6_1[5][BUFFERBASE_VDATA + i];
		data5_7 = vbuffer7_1[5][BUFFERBASE_VDATA + i];
		data5_8 = vbuffer8_1[5][BUFFERBASE_VDATA + i];
		data5_9 = vbuffer9_1[5][BUFFERBASE_VDATA + i];
		data5_10 = vbuffer10_1[5][BUFFERBASE_VDATA + i];
		data5_11 = vbuffer11_1[5][BUFFERBASE_VDATA + i];
		data5_12 = vbuffer12_1[5][BUFFERBASE_VDATA + i];
		data5_13 = vbuffer13_1[5][BUFFERBASE_VDATA + i];
		data5_14 = vbuffer14_1[5][BUFFERBASE_VDATA + i];
		data5_15 = vbuffer15_1[5][BUFFERBASE_VDATA + i];
 // AUTOMATEME.
		data6_0 = vbuffer0_1[6][BUFFERBASE_VDATA + i];
		data6_1 = vbuffer1_1[6][BUFFERBASE_VDATA + i];
		data6_2 = vbuffer2_1[6][BUFFERBASE_VDATA + i];
		data6_3 = vbuffer3_1[6][BUFFERBASE_VDATA + i];
		data6_4 = vbuffer4_1[6][BUFFERBASE_VDATA + i];
		data6_5 = vbuffer5_1[6][BUFFERBASE_VDATA + i];
		data6_6 = vbuffer6_1[6][BUFFERBASE_VDATA + i];
		data6_7 = vbuffer7_1[6][BUFFERBASE_VDATA + i];
		data6_8 = vbuffer8_1[6][BUFFERBASE_VDATA + i];
		data6_9 = vbuffer9_1[6][BUFFERBASE_VDATA + i];
		data6_10 = vbuffer10_1[6][BUFFERBASE_VDATA + i];
		data6_11 = vbuffer11_1[6][BUFFERBASE_VDATA + i];
		data6_12 = vbuffer12_1[6][BUFFERBASE_VDATA + i];
		data6_13 = vbuffer13_1[6][BUFFERBASE_VDATA + i];
		data6_14 = vbuffer14_1[6][BUFFERBASE_VDATA + i];
		data6_15 = vbuffer15_1[6][BUFFERBASE_VDATA + i];
 // AUTOMATEME.
		data7_0 = vbuffer0_1[7][BUFFERBASE_VDATA + i];
		data7_1 = vbuffer1_1[7][BUFFERBASE_VDATA + i];
		data7_2 = vbuffer2_1[7][BUFFERBASE_VDATA + i];
		data7_3 = vbuffer3_1[7][BUFFERBASE_VDATA + i];
		data7_4 = vbuffer4_1[7][BUFFERBASE_VDATA + i];
		data7_5 = vbuffer5_1[7][BUFFERBASE_VDATA + i];
		data7_6 = vbuffer6_1[7][BUFFERBASE_VDATA + i];
		data7_7 = vbuffer7_1[7][BUFFERBASE_VDATA + i];
		data7_8 = vbuffer8_1[7][BUFFERBASE_VDATA + i];
		data7_9 = vbuffer9_1[7][BUFFERBASE_VDATA + i];
		data7_10 = vbuffer10_1[7][BUFFERBASE_VDATA + i];
		data7_11 = vbuffer11_1[7][BUFFERBASE_VDATA + i];
		data7_12 = vbuffer12_1[7][BUFFERBASE_VDATA + i];
		data7_13 = vbuffer13_1[7][BUFFERBASE_VDATA + i];
		data7_14 = vbuffer14_1[7][BUFFERBASE_VDATA + i];
		data7_15 = vbuffer15_1[7][BUFFERBASE_VDATA + i];
 // AUTOMATEME.
		data8_0 = vbuffer0_2[0][BUFFERBASE_VDATA + i];
		data8_1 = vbuffer1_2[0][BUFFERBASE_VDATA + i];
		data8_2 = vbuffer2_2[0][BUFFERBASE_VDATA + i];
		data8_3 = vbuffer3_2[0][BUFFERBASE_VDATA + i];
		data8_4 = vbuffer4_2[0][BUFFERBASE_VDATA + i];
		data8_5 = vbuffer5_2[0][BUFFERBASE_VDATA + i];
		data8_6 = vbuffer6_2[0][BUFFERBASE_VDATA + i];
		data8_7 = vbuffer7_2[0][BUFFERBASE_VDATA + i];
		data8_8 = vbuffer8_2[0][BUFFERBASE_VDATA + i];
		data8_9 = vbuffer9_2[0][BUFFERBASE_VDATA + i];
		data8_10 = vbuffer10_2[0][BUFFERBASE_VDATA + i];
		data8_11 = vbuffer11_2[0][BUFFERBASE_VDATA + i];
		data8_12 = vbuffer12_2[0][BUFFERBASE_VDATA + i];
		data8_13 = vbuffer13_2[0][BUFFERBASE_VDATA + i];
		data8_14 = vbuffer14_2[0][BUFFERBASE_VDATA + i];
		data8_15 = vbuffer15_2[0][BUFFERBASE_VDATA + i];
 // AUTOMATEME.
		data9_0 = vbuffer0_2[1][BUFFERBASE_VDATA + i];
		data9_1 = vbuffer1_2[1][BUFFERBASE_VDATA + i];
		data9_2 = vbuffer2_2[1][BUFFERBASE_VDATA + i];
		data9_3 = vbuffer3_2[1][BUFFERBASE_VDATA + i];
		data9_4 = vbuffer4_2[1][BUFFERBASE_VDATA + i];
		data9_5 = vbuffer5_2[1][BUFFERBASE_VDATA + i];
		data9_6 = vbuffer6_2[1][BUFFERBASE_VDATA + i];
		data9_7 = vbuffer7_2[1][BUFFERBASE_VDATA + i];
		data9_8 = vbuffer8_2[1][BUFFERBASE_VDATA + i];
		data9_9 = vbuffer9_2[1][BUFFERBASE_VDATA + i];
		data9_10 = vbuffer10_2[1][BUFFERBASE_VDATA + i];
		data9_11 = vbuffer11_2[1][BUFFERBASE_VDATA + i];
		data9_12 = vbuffer12_2[1][BUFFERBASE_VDATA + i];
		data9_13 = vbuffer13_2[1][BUFFERBASE_VDATA + i];
		data9_14 = vbuffer14_2[1][BUFFERBASE_VDATA + i];
		data9_15 = vbuffer15_2[1][BUFFERBASE_VDATA + i];
 // AUTOMATEME.
		data10_0 = vbuffer0_2[2][BUFFERBASE_VDATA + i];
		data10_1 = vbuffer1_2[2][BUFFERBASE_VDATA + i];
		data10_2 = vbuffer2_2[2][BUFFERBASE_VDATA + i];
		data10_3 = vbuffer3_2[2][BUFFERBASE_VDATA + i];
		data10_4 = vbuffer4_2[2][BUFFERBASE_VDATA + i];
		data10_5 = vbuffer5_2[2][BUFFERBASE_VDATA + i];
		data10_6 = vbuffer6_2[2][BUFFERBASE_VDATA + i];
		data10_7 = vbuffer7_2[2][BUFFERBASE_VDATA + i];
		data10_8 = vbuffer8_2[2][BUFFERBASE_VDATA + i];
		data10_9 = vbuffer9_2[2][BUFFERBASE_VDATA + i];
		data10_10 = vbuffer10_2[2][BUFFERBASE_VDATA + i];
		data10_11 = vbuffer11_2[2][BUFFERBASE_VDATA + i];
		data10_12 = vbuffer12_2[2][BUFFERBASE_VDATA + i];
		data10_13 = vbuffer13_2[2][BUFFERBASE_VDATA + i];
		data10_14 = vbuffer14_2[2][BUFFERBASE_VDATA + i];
		data10_15 = vbuffer15_2[2][BUFFERBASE_VDATA + i];
 // AUTOMATEME.
		data11_0 = vbuffer0_2[3][BUFFERBASE_VDATA + i];
		data11_1 = vbuffer1_2[3][BUFFERBASE_VDATA + i];
		data11_2 = vbuffer2_2[3][BUFFERBASE_VDATA + i];
		data11_3 = vbuffer3_2[3][BUFFERBASE_VDATA + i];
		data11_4 = vbuffer4_2[3][BUFFERBASE_VDATA + i];
		data11_5 = vbuffer5_2[3][BUFFERBASE_VDATA + i];
		data11_6 = vbuffer6_2[3][BUFFERBASE_VDATA + i];
		data11_7 = vbuffer7_2[3][BUFFERBASE_VDATA + i];
		data11_8 = vbuffer8_2[3][BUFFERBASE_VDATA + i];
		data11_9 = vbuffer9_2[3][BUFFERBASE_VDATA + i];
		data11_10 = vbuffer10_2[3][BUFFERBASE_VDATA + i];
		data11_11 = vbuffer11_2[3][BUFFERBASE_VDATA + i];
		data11_12 = vbuffer12_2[3][BUFFERBASE_VDATA + i];
		data11_13 = vbuffer13_2[3][BUFFERBASE_VDATA + i];
		data11_14 = vbuffer14_2[3][BUFFERBASE_VDATA + i];
		data11_15 = vbuffer15_2[3][BUFFERBASE_VDATA + i];
 // AUTOMATEME.
		data12_0 = vbuffer0_2[4][BUFFERBASE_VDATA + i];
		data12_1 = vbuffer1_2[4][BUFFERBASE_VDATA + i];
		data12_2 = vbuffer2_2[4][BUFFERBASE_VDATA + i];
		data12_3 = vbuffer3_2[4][BUFFERBASE_VDATA + i];
		data12_4 = vbuffer4_2[4][BUFFERBASE_VDATA + i];
		data12_5 = vbuffer5_2[4][BUFFERBASE_VDATA + i];
		data12_6 = vbuffer6_2[4][BUFFERBASE_VDATA + i];
		data12_7 = vbuffer7_2[4][BUFFERBASE_VDATA + i];
		data12_8 = vbuffer8_2[4][BUFFERBASE_VDATA + i];
		data12_9 = vbuffer9_2[4][BUFFERBASE_VDATA + i];
		data12_10 = vbuffer10_2[4][BUFFERBASE_VDATA + i];
		data12_11 = vbuffer11_2[4][BUFFERBASE_VDATA + i];
		data12_12 = vbuffer12_2[4][BUFFERBASE_VDATA + i];
		data12_13 = vbuffer13_2[4][BUFFERBASE_VDATA + i];
		data12_14 = vbuffer14_2[4][BUFFERBASE_VDATA + i];
		data12_15 = vbuffer15_2[4][BUFFERBASE_VDATA + i];
 // AUTOMATEME.
		data13_0 = vbuffer0_2[5][BUFFERBASE_VDATA + i];
		data13_1 = vbuffer1_2[5][BUFFERBASE_VDATA + i];
		data13_2 = vbuffer2_2[5][BUFFERBASE_VDATA + i];
		data13_3 = vbuffer3_2[5][BUFFERBASE_VDATA + i];
		data13_4 = vbuffer4_2[5][BUFFERBASE_VDATA + i];
		data13_5 = vbuffer5_2[5][BUFFERBASE_VDATA + i];
		data13_6 = vbuffer6_2[5][BUFFERBASE_VDATA + i];
		data13_7 = vbuffer7_2[5][BUFFERBASE_VDATA + i];
		data13_8 = vbuffer8_2[5][BUFFERBASE_VDATA + i];
		data13_9 = vbuffer9_2[5][BUFFERBASE_VDATA + i];
		data13_10 = vbuffer10_2[5][BUFFERBASE_VDATA + i];
		data13_11 = vbuffer11_2[5][BUFFERBASE_VDATA + i];
		data13_12 = vbuffer12_2[5][BUFFERBASE_VDATA + i];
		data13_13 = vbuffer13_2[5][BUFFERBASE_VDATA + i];
		data13_14 = vbuffer14_2[5][BUFFERBASE_VDATA + i];
		data13_15 = vbuffer15_2[5][BUFFERBASE_VDATA + i];
 // AUTOMATEME.
		data14_0 = vbuffer0_2[6][BUFFERBASE_VDATA + i];
		data14_1 = vbuffer1_2[6][BUFFERBASE_VDATA + i];
		data14_2 = vbuffer2_2[6][BUFFERBASE_VDATA + i];
		data14_3 = vbuffer3_2[6][BUFFERBASE_VDATA + i];
		data14_4 = vbuffer4_2[6][BUFFERBASE_VDATA + i];
		data14_5 = vbuffer5_2[6][BUFFERBASE_VDATA + i];
		data14_6 = vbuffer6_2[6][BUFFERBASE_VDATA + i];
		data14_7 = vbuffer7_2[6][BUFFERBASE_VDATA + i];
		data14_8 = vbuffer8_2[6][BUFFERBASE_VDATA + i];
		data14_9 = vbuffer9_2[6][BUFFERBASE_VDATA + i];
		data14_10 = vbuffer10_2[6][BUFFERBASE_VDATA + i];
		data14_11 = vbuffer11_2[6][BUFFERBASE_VDATA + i];
		data14_12 = vbuffer12_2[6][BUFFERBASE_VDATA + i];
		data14_13 = vbuffer13_2[6][BUFFERBASE_VDATA + i];
		data14_14 = vbuffer14_2[6][BUFFERBASE_VDATA + i];
		data14_15 = vbuffer15_2[6][BUFFERBASE_VDATA + i];
 // AUTOMATEME.
		data15_0 = vbuffer0_2[7][BUFFERBASE_VDATA + i];
		data15_1 = vbuffer1_2[7][BUFFERBASE_VDATA + i];
		data15_2 = vbuffer2_2[7][BUFFERBASE_VDATA + i];
		data15_3 = vbuffer3_2[7][BUFFERBASE_VDATA + i];
		data15_4 = vbuffer4_2[7][BUFFERBASE_VDATA + i];
		data15_5 = vbuffer5_2[7][BUFFERBASE_VDATA + i];
		data15_6 = vbuffer6_2[7][BUFFERBASE_VDATA + i];
		data15_7 = vbuffer7_2[7][BUFFERBASE_VDATA + i];
		data15_8 = vbuffer8_2[7][BUFFERBASE_VDATA + i];
		data15_9 = vbuffer9_2[7][BUFFERBASE_VDATA + i];
		data15_10 = vbuffer10_2[7][BUFFERBASE_VDATA + i];
		data15_11 = vbuffer11_2[7][BUFFERBASE_VDATA + i];
		data15_12 = vbuffer12_2[7][BUFFERBASE_VDATA + i];
		data15_13 = vbuffer13_2[7][BUFFERBASE_VDATA + i];
		data15_14 = vbuffer14_2[7][BUFFERBASE_VDATA + i];
		data15_15 = vbuffer15_2[7][BUFFERBASE_VDATA + i];
		
		keyvalue_t udata0 = synvbuffer[0][i];
		keyvalue_t udata1 = synvbuffer[1][i];
		keyvalue_t udata2 = synvbuffer[2][i];
		keyvalue_t udata3 = synvbuffer[3][i];
		keyvalue_t udata4 = synvbuffer[4][i];
		keyvalue_t udata5 = synvbuffer[5][i];
		keyvalue_t udata6 = synvbuffer[6][i];
		keyvalue_t udata7 = synvbuffer[7][i];
		keyvalue_t udata8 = synvbuffer[8][i];
		keyvalue_t udata9 = synvbuffer[9][i];
		keyvalue_t udata10 = synvbuffer[10][i];
		keyvalue_t udata11 = synvbuffer[11][i];
		keyvalue_t udata12 = synvbuffer[12][i];
		keyvalue_t udata13 = synvbuffer[13][i];
		keyvalue_t udata14 = synvbuffer[14][i];
		keyvalue_t udata15 = synvbuffer[15][i];
		
		data0.key = mergefunc_NUMPto1(data0_0.key,data0_1.key,data0_2.key,data0_3.key,data0_4.key,data0_5.key,data0_6.key,data0_7.key,data0_8.key,data0_9.key,data0_10.key,data0_11.key,data0_12.key,data0_13.key,data0_14.key,data0_15.key, globalparams.GraphAlgo);
		data1.key = mergefunc_NUMPto1(data1_0.key,data1_1.key,data1_2.key,data1_3.key,data1_4.key,data1_5.key,data1_6.key,data1_7.key,data1_8.key,data1_9.key,data1_10.key,data1_11.key,data1_12.key,data1_13.key,data1_14.key,data1_15.key, globalparams.GraphAlgo);
		data2.key = mergefunc_NUMPto1(data2_0.key,data2_1.key,data2_2.key,data2_3.key,data2_4.key,data2_5.key,data2_6.key,data2_7.key,data2_8.key,data2_9.key,data2_10.key,data2_11.key,data2_12.key,data2_13.key,data2_14.key,data2_15.key, globalparams.GraphAlgo);
		data3.key = mergefunc_NUMPto1(data3_0.key,data3_1.key,data3_2.key,data3_3.key,data3_4.key,data3_5.key,data3_6.key,data3_7.key,data3_8.key,data3_9.key,data3_10.key,data3_11.key,data3_12.key,data3_13.key,data3_14.key,data3_15.key, globalparams.GraphAlgo);
		data4.key = mergefunc_NUMPto1(data4_0.key,data4_1.key,data4_2.key,data4_3.key,data4_4.key,data4_5.key,data4_6.key,data4_7.key,data4_8.key,data4_9.key,data4_10.key,data4_11.key,data4_12.key,data4_13.key,data4_14.key,data4_15.key, globalparams.GraphAlgo);
		data5.key = mergefunc_NUMPto1(data5_0.key,data5_1.key,data5_2.key,data5_3.key,data5_4.key,data5_5.key,data5_6.key,data5_7.key,data5_8.key,data5_9.key,data5_10.key,data5_11.key,data5_12.key,data5_13.key,data5_14.key,data5_15.key, globalparams.GraphAlgo);
		data6.key = mergefunc_NUMPto1(data6_0.key,data6_1.key,data6_2.key,data6_3.key,data6_4.key,data6_5.key,data6_6.key,data6_7.key,data6_8.key,data6_9.key,data6_10.key,data6_11.key,data6_12.key,data6_13.key,data6_14.key,data6_15.key, globalparams.GraphAlgo);
		data7.key = mergefunc_NUMPto1(data7_0.key,data7_1.key,data7_2.key,data7_3.key,data7_4.key,data7_5.key,data7_6.key,data7_7.key,data7_8.key,data7_9.key,data7_10.key,data7_11.key,data7_12.key,data7_13.key,data7_14.key,data7_15.key, globalparams.GraphAlgo);
		data8.key = mergefunc_NUMPto1(data8_0.key,data8_1.key,data8_2.key,data8_3.key,data8_4.key,data8_5.key,data8_6.key,data8_7.key,data8_8.key,data8_9.key,data8_10.key,data8_11.key,data8_12.key,data8_13.key,data8_14.key,data8_15.key, globalparams.GraphAlgo);
		data9.key = mergefunc_NUMPto1(data9_0.key,data9_1.key,data9_2.key,data9_3.key,data9_4.key,data9_5.key,data9_6.key,data9_7.key,data9_8.key,data9_9.key,data9_10.key,data9_11.key,data9_12.key,data9_13.key,data9_14.key,data9_15.key, globalparams.GraphAlgo);
		data10.key = mergefunc_NUMPto1(data10_0.key,data10_1.key,data10_2.key,data10_3.key,data10_4.key,data10_5.key,data10_6.key,data10_7.key,data10_8.key,data10_9.key,data10_10.key,data10_11.key,data10_12.key,data10_13.key,data10_14.key,data10_15.key, globalparams.GraphAlgo);
		data11.key = mergefunc_NUMPto1(data11_0.key,data11_1.key,data11_2.key,data11_3.key,data11_4.key,data11_5.key,data11_6.key,data11_7.key,data11_8.key,data11_9.key,data11_10.key,data11_11.key,data11_12.key,data11_13.key,data11_14.key,data11_15.key, globalparams.GraphAlgo);
		data12.key = mergefunc_NUMPto1(data12_0.key,data12_1.key,data12_2.key,data12_3.key,data12_4.key,data12_5.key,data12_6.key,data12_7.key,data12_8.key,data12_9.key,data12_10.key,data12_11.key,data12_12.key,data12_13.key,data12_14.key,data12_15.key, globalparams.GraphAlgo);
		data13.key = mergefunc_NUMPto1(data13_0.key,data13_1.key,data13_2.key,data13_3.key,data13_4.key,data13_5.key,data13_6.key,data13_7.key,data13_8.key,data13_9.key,data13_10.key,data13_11.key,data13_12.key,data13_13.key,data13_14.key,data13_15.key, globalparams.GraphAlgo);
		data14.key = mergefunc_NUMPto1(data14_0.key,data14_1.key,data14_2.key,data14_3.key,data14_4.key,data14_5.key,data14_6.key,data14_7.key,data14_8.key,data14_9.key,data14_10.key,data14_11.key,data14_12.key,data14_13.key,data14_14.key,data14_15.key, globalparams.GraphAlgo);
		data15.key = mergefunc_NUMPto1(data15_0.key,data15_1.key,data15_2.key,data15_3.key,data15_4.key,data15_5.key,data15_6.key,data15_7.key,data15_8.key,data15_9.key,data15_10.key,data15_11.key,data15_12.key,data15_13.key,data15_14.key,data15_15.key, globalparams.GraphAlgo);
		data0.value = mergefunc_NUMPto1(data0_0.value,data0_1.value,data0_2.value,data0_3.value,data0_4.value,data0_5.value,data0_6.value,data0_7.value,data0_8.value,data0_9.value,data0_10.value,data0_11.value,data0_12.value,data0_13.value,data0_14.value,data0_15.value, globalparams.GraphAlgo);
		data1.value = mergefunc_NUMPto1(data1_0.value,data1_1.value,data1_2.value,data1_3.value,data1_4.value,data1_5.value,data1_6.value,data1_7.value,data1_8.value,data1_9.value,data1_10.value,data1_11.value,data1_12.value,data1_13.value,data1_14.value,data1_15.value, globalparams.GraphAlgo);
		data2.value = mergefunc_NUMPto1(data2_0.value,data2_1.value,data2_2.value,data2_3.value,data2_4.value,data2_5.value,data2_6.value,data2_7.value,data2_8.value,data2_9.value,data2_10.value,data2_11.value,data2_12.value,data2_13.value,data2_14.value,data2_15.value, globalparams.GraphAlgo);
		data3.value = mergefunc_NUMPto1(data3_0.value,data3_1.value,data3_2.value,data3_3.value,data3_4.value,data3_5.value,data3_6.value,data3_7.value,data3_8.value,data3_9.value,data3_10.value,data3_11.value,data3_12.value,data3_13.value,data3_14.value,data3_15.value, globalparams.GraphAlgo);
		data4.value = mergefunc_NUMPto1(data4_0.value,data4_1.value,data4_2.value,data4_3.value,data4_4.value,data4_5.value,data4_6.value,data4_7.value,data4_8.value,data4_9.value,data4_10.value,data4_11.value,data4_12.value,data4_13.value,data4_14.value,data4_15.value, globalparams.GraphAlgo);
		data5.value = mergefunc_NUMPto1(data5_0.value,data5_1.value,data5_2.value,data5_3.value,data5_4.value,data5_5.value,data5_6.value,data5_7.value,data5_8.value,data5_9.value,data5_10.value,data5_11.value,data5_12.value,data5_13.value,data5_14.value,data5_15.value, globalparams.GraphAlgo);
		data6.value = mergefunc_NUMPto1(data6_0.value,data6_1.value,data6_2.value,data6_3.value,data6_4.value,data6_5.value,data6_6.value,data6_7.value,data6_8.value,data6_9.value,data6_10.value,data6_11.value,data6_12.value,data6_13.value,data6_14.value,data6_15.value, globalparams.GraphAlgo);
		data7.value = mergefunc_NUMPto1(data7_0.value,data7_1.value,data7_2.value,data7_3.value,data7_4.value,data7_5.value,data7_6.value,data7_7.value,data7_8.value,data7_9.value,data7_10.value,data7_11.value,data7_12.value,data7_13.value,data7_14.value,data7_15.value, globalparams.GraphAlgo);
		data8.value = mergefunc_NUMPto1(data8_0.value,data8_1.value,data8_2.value,data8_3.value,data8_4.value,data8_5.value,data8_6.value,data8_7.value,data8_8.value,data8_9.value,data8_10.value,data8_11.value,data8_12.value,data8_13.value,data8_14.value,data8_15.value, globalparams.GraphAlgo);
		data9.value = mergefunc_NUMPto1(data9_0.value,data9_1.value,data9_2.value,data9_3.value,data9_4.value,data9_5.value,data9_6.value,data9_7.value,data9_8.value,data9_9.value,data9_10.value,data9_11.value,data9_12.value,data9_13.value,data9_14.value,data9_15.value, globalparams.GraphAlgo);
		data10.value = mergefunc_NUMPto1(data10_0.value,data10_1.value,data10_2.value,data10_3.value,data10_4.value,data10_5.value,data10_6.value,data10_7.value,data10_8.value,data10_9.value,data10_10.value,data10_11.value,data10_12.value,data10_13.value,data10_14.value,data10_15.value, globalparams.GraphAlgo);
		data11.value = mergefunc_NUMPto1(data11_0.value,data11_1.value,data11_2.value,data11_3.value,data11_4.value,data11_5.value,data11_6.value,data11_7.value,data11_8.value,data11_9.value,data11_10.value,data11_11.value,data11_12.value,data11_13.value,data11_14.value,data11_15.value, globalparams.GraphAlgo);
		data12.value = mergefunc_NUMPto1(data12_0.value,data12_1.value,data12_2.value,data12_3.value,data12_4.value,data12_5.value,data12_6.value,data12_7.value,data12_8.value,data12_9.value,data12_10.value,data12_11.value,data12_12.value,data12_13.value,data12_14.value,data12_15.value, globalparams.GraphAlgo);
		data13.value = mergefunc_NUMPto1(data13_0.value,data13_1.value,data13_2.value,data13_3.value,data13_4.value,data13_5.value,data13_6.value,data13_7.value,data13_8.value,data13_9.value,data13_10.value,data13_11.value,data13_12.value,data13_13.value,data13_14.value,data13_15.value, globalparams.GraphAlgo);
		data14.value = mergefunc_NUMPto1(data14_0.value,data14_1.value,data14_2.value,data14_3.value,data14_4.value,data14_5.value,data14_6.value,data14_7.value,data14_8.value,data14_9.value,data14_10.value,data14_11.value,data14_12.value,data14_13.value,data14_14.value,data14_15.value, globalparams.GraphAlgo);
		data15.value = mergefunc_NUMPto1(data15_0.value,data15_1.value,data15_2.value,data15_3.value,data15_4.value,data15_5.value,data15_6.value,data15_7.value,data15_8.value,data15_9.value,data15_10.value,data15_11.value,data15_12.value,data15_13.value,data15_14.value,data15_15.value, globalparams.GraphAlgo);
		
		res0.key = applyfunc(udata0.key, data0.key, globalparams.GraphIter, globalparams.GraphAlgo);
		res1.key = applyfunc(udata1.key, data1.key, globalparams.GraphIter, globalparams.GraphAlgo);
		res2.key = applyfunc(udata2.key, data2.key, globalparams.GraphIter, globalparams.GraphAlgo);
		res3.key = applyfunc(udata3.key, data3.key, globalparams.GraphIter, globalparams.GraphAlgo);
		res4.key = applyfunc(udata4.key, data4.key, globalparams.GraphIter, globalparams.GraphAlgo);
		res5.key = applyfunc(udata5.key, data5.key, globalparams.GraphIter, globalparams.GraphAlgo);
		res6.key = applyfunc(udata6.key, data6.key, globalparams.GraphIter, globalparams.GraphAlgo);
		res7.key = applyfunc(udata7.key, data7.key, globalparams.GraphIter, globalparams.GraphAlgo);
		res8.key = applyfunc(udata8.key, data8.key, globalparams.GraphIter, globalparams.GraphAlgo);
		res9.key = applyfunc(udata9.key, data9.key, globalparams.GraphIter, globalparams.GraphAlgo);
		res10.key = applyfunc(udata10.key, data10.key, globalparams.GraphIter, globalparams.GraphAlgo);
		res11.key = applyfunc(udata11.key, data11.key, globalparams.GraphIter, globalparams.GraphAlgo);
		res12.key = applyfunc(udata12.key, data12.key, globalparams.GraphIter, globalparams.GraphAlgo);
		res13.key = applyfunc(udata13.key, data13.key, globalparams.GraphIter, globalparams.GraphAlgo);
		res14.key = applyfunc(udata14.key, data14.key, globalparams.GraphIter, globalparams.GraphAlgo);
		res15.key = applyfunc(udata15.key, data15.key, globalparams.GraphIter, globalparams.GraphAlgo);
		res0.value = applyfunc(udata0.value, data0.value, globalparams.GraphIter, globalparams.GraphAlgo);
		res1.value = applyfunc(udata1.value, data1.value, globalparams.GraphIter, globalparams.GraphAlgo);
		res2.value = applyfunc(udata2.value, data2.value, globalparams.GraphIter, globalparams.GraphAlgo);
		res3.value = applyfunc(udata3.value, data3.value, globalparams.GraphIter, globalparams.GraphAlgo);
		res4.value = applyfunc(udata4.value, data4.value, globalparams.GraphIter, globalparams.GraphAlgo);
		res5.value = applyfunc(udata5.value, data5.value, globalparams.GraphIter, globalparams.GraphAlgo);
		res6.value = applyfunc(udata6.value, data6.value, globalparams.GraphIter, globalparams.GraphAlgo);
		res7.value = applyfunc(udata7.value, data7.value, globalparams.GraphIter, globalparams.GraphAlgo);
		res8.value = applyfunc(udata8.value, data8.value, globalparams.GraphIter, globalparams.GraphAlgo);
		res9.value = applyfunc(udata9.value, data9.value, globalparams.GraphIter, globalparams.GraphAlgo);
		res10.value = applyfunc(udata10.value, data10.value, globalparams.GraphIter, globalparams.GraphAlgo);
		res11.value = applyfunc(udata11.value, data11.value, globalparams.GraphIter, globalparams.GraphAlgo);
		res12.value = applyfunc(udata12.value, data12.value, globalparams.GraphIter, globalparams.GraphAlgo);
		res13.value = applyfunc(udata13.value, data13.value, globalparams.GraphIter, globalparams.GraphAlgo);
		res14.value = applyfunc(udata14.value, data14.value, globalparams.GraphIter, globalparams.GraphAlgo);
		res15.value = applyfunc(udata15.value, data15.value, globalparams.GraphIter, globalparams.GraphAlgo);
		
	
		synvbuffer[0][i] = res0;	
	
		synvbuffer[1][i] = res1;	
	
		synvbuffer[2][i] = res2;	
	
		synvbuffer[3][i] = res3;	
	
		synvbuffer[4][i] = res4;	
	
		synvbuffer[5][i] = res5;	
	
		synvbuffer[6][i] = res6;	
	
		synvbuffer[7][i] = res7;	
	
		synvbuffer[8][i] = res8;	
	
		synvbuffer[9][i] = res9;	
	
		synvbuffer[10][i] = res10;	
	
		synvbuffer[11][i] = res11;	
	
		synvbuffer[12][i] = res12;	
	
		synvbuffer[13][i] = res13;	
	
		synvbuffer[14][i] = res14;	
	
		synvbuffer[15][i] = res15;	
	
		if(res0.key != udata0.key){ _vmask[i].data[0].key = 1; } else { _vmask[i].data[0].key = 0; }
		if(res0.value != udata0.value){ _vmask[i].data[0].value = 1; } else { _vmask[i].data[0].value = 0; }
		if(res1.key != udata1.key){ _vmask[i].data[1].key = 1; } else { _vmask[i].data[1].key = 0; }
		if(res1.value != udata1.value){ _vmask[i].data[1].value = 1; } else { _vmask[i].data[1].value = 0; }
		if(res2.key != udata2.key){ _vmask[i].data[2].key = 1; } else { _vmask[i].data[2].key = 0; }
		if(res2.value != udata2.value){ _vmask[i].data[2].value = 1; } else { _vmask[i].data[2].value = 0; }
		if(res3.key != udata3.key){ _vmask[i].data[3].key = 1; } else { _vmask[i].data[3].key = 0; }
		if(res3.value != udata3.value){ _vmask[i].data[3].value = 1; } else { _vmask[i].data[3].value = 0; }
		if(res4.key != udata4.key){ _vmask[i].data[4].key = 1; } else { _vmask[i].data[4].key = 0; }
		if(res4.value != udata4.value){ _vmask[i].data[4].value = 1; } else { _vmask[i].data[4].value = 0; }
		if(res5.key != udata5.key){ _vmask[i].data[5].key = 1; } else { _vmask[i].data[5].key = 0; }
		if(res5.value != udata5.value){ _vmask[i].data[5].value = 1; } else { _vmask[i].data[5].value = 0; }
		if(res6.key != udata6.key){ _vmask[i].data[6].key = 1; } else { _vmask[i].data[6].key = 0; }
		if(res6.value != udata6.value){ _vmask[i].data[6].value = 1; } else { _vmask[i].data[6].value = 0; }
		if(res7.key != udata7.key){ _vmask[i].data[7].key = 1; } else { _vmask[i].data[7].key = 0; }
		if(res7.value != udata7.value){ _vmask[i].data[7].value = 1; } else { _vmask[i].data[7].value = 0; }
		if(res8.key != udata8.key){ _vmask[i].data[8].key = 1; } else { _vmask[i].data[8].key = 0; }
		if(res8.value != udata8.value){ _vmask[i].data[8].value = 1; } else { _vmask[i].data[8].value = 0; }
		if(res9.key != udata9.key){ _vmask[i].data[9].key = 1; } else { _vmask[i].data[9].key = 0; }
		if(res9.value != udata9.value){ _vmask[i].data[9].value = 1; } else { _vmask[i].data[9].value = 0; }
		if(res10.key != udata10.key){ _vmask[i].data[10].key = 1; } else { _vmask[i].data[10].key = 0; }
		if(res10.value != udata10.value){ _vmask[i].data[10].value = 1; } else { _vmask[i].data[10].value = 0; }
		if(res11.key != udata11.key){ _vmask[i].data[11].key = 1; } else { _vmask[i].data[11].key = 0; }
		if(res11.value != udata11.value){ _vmask[i].data[11].value = 1; } else { _vmask[i].data[11].value = 0; }
		if(res12.key != udata12.key){ _vmask[i].data[12].key = 1; } else { _vmask[i].data[12].key = 0; }
		if(res12.value != udata12.value){ _vmask[i].data[12].value = 1; } else { _vmask[i].data[12].value = 0; }
		if(res13.key != udata13.key){ _vmask[i].data[13].key = 1; } else { _vmask[i].data[13].key = 0; }
		if(res13.value != udata13.value){ _vmask[i].data[13].value = 1; } else { _vmask[i].data[13].value = 0; }
		if(res14.key != udata14.key){ _vmask[i].data[14].key = 1; } else { _vmask[i].data[14].key = 0; }
		if(res14.value != udata14.value){ _vmask[i].data[14].value = 1; } else { _vmask[i].data[14].value = 0; }
		if(res15.key != udata15.key){ _vmask[i].data[15].key = 1; } else { _vmask[i].data[15].key = 0; }
		if(res15.value != udata15.value){ _vmask[i].data[15].value = 1; } else { _vmask[i].data[15].value = 0; }
		
		#ifdef _DEBUGMODE_KERNELPRINTS // _DEBUGMODE_KERNELPRINTS3
		if(false){ cout<<"SYNCHRONIZE FUNC SEEN @ (vid1: "<<vid[0][0]<<" & vid2: "<<vid[0][1]<<"): res0.key: "<<res0.key<<", res0.value: "<<res0.value<<": udata0.key: "<<udata0.key<<", udata0.value: "<<udata0.value<<", data0.key: "<<data0.key<<", data0.value: "<<data0.value<<endl; }
		if(res0.key != udata0.key){ cout<<"synchronize: vid: "<<vid[0][0]<<", _vmask["<<i<<"].data[0].key: "<<_vmask[i].data[0].key<<endl; }
		if(res0.value != udata0.value){ cout<<"synchronize: vid: "<<vid[0][1]<<", _vmask["<<i<<"].data[0].value: "<<_vmask[i].data[0].value<<endl; }
		if(false){ cout<<"SYNCHRONIZE FUNC SEEN @ (vid1: "<<vid[1][0]<<" & vid2: "<<vid[1][1]<<"): res1.key: "<<res1.key<<", res1.value: "<<res1.value<<": udata1.key: "<<udata1.key<<", udata1.value: "<<udata1.value<<", data1.key: "<<data1.key<<", data1.value: "<<data1.value<<endl; }
		if(res1.key != udata1.key){ cout<<"synchronize: vid: "<<vid[1][0]<<", _vmask["<<i<<"].data[1].key: "<<_vmask[i].data[1].key<<endl; }
		if(res1.value != udata1.value){ cout<<"synchronize: vid: "<<vid[1][1]<<", _vmask["<<i<<"].data[1].value: "<<_vmask[i].data[1].value<<endl; }
		if(false){ cout<<"SYNCHRONIZE FUNC SEEN @ (vid1: "<<vid[2][0]<<" & vid2: "<<vid[2][1]<<"): res2.key: "<<res2.key<<", res2.value: "<<res2.value<<": udata2.key: "<<udata2.key<<", udata2.value: "<<udata2.value<<", data2.key: "<<data2.key<<", data2.value: "<<data2.value<<endl; }
		if(res2.key != udata2.key){ cout<<"synchronize: vid: "<<vid[2][0]<<", _vmask["<<i<<"].data[2].key: "<<_vmask[i].data[2].key<<endl; }
		if(res2.value != udata2.value){ cout<<"synchronize: vid: "<<vid[2][1]<<", _vmask["<<i<<"].data[2].value: "<<_vmask[i].data[2].value<<endl; }
		if(false){ cout<<"SYNCHRONIZE FUNC SEEN @ (vid1: "<<vid[3][0]<<" & vid2: "<<vid[3][1]<<"): res3.key: "<<res3.key<<", res3.value: "<<res3.value<<": udata3.key: "<<udata3.key<<", udata3.value: "<<udata3.value<<", data3.key: "<<data3.key<<", data3.value: "<<data3.value<<endl; }
		if(res3.key != udata3.key){ cout<<"synchronize: vid: "<<vid[3][0]<<", _vmask["<<i<<"].data[3].key: "<<_vmask[i].data[3].key<<endl; }
		if(res3.value != udata3.value){ cout<<"synchronize: vid: "<<vid[3][1]<<", _vmask["<<i<<"].data[3].value: "<<_vmask[i].data[3].value<<endl; }
		if(false){ cout<<"SYNCHRONIZE FUNC SEEN @ (vid1: "<<vid[4][0]<<" & vid2: "<<vid[4][1]<<"): res4.key: "<<res4.key<<", res4.value: "<<res4.value<<": udata4.key: "<<udata4.key<<", udata4.value: "<<udata4.value<<", data4.key: "<<data4.key<<", data4.value: "<<data4.value<<endl; }
		if(res4.key != udata4.key){ cout<<"synchronize: vid: "<<vid[4][0]<<", _vmask["<<i<<"].data[4].key: "<<_vmask[i].data[4].key<<endl; }
		if(res4.value != udata4.value){ cout<<"synchronize: vid: "<<vid[4][1]<<", _vmask["<<i<<"].data[4].value: "<<_vmask[i].data[4].value<<endl; }
		if(false){ cout<<"SYNCHRONIZE FUNC SEEN @ (vid1: "<<vid[5][0]<<" & vid2: "<<vid[5][1]<<"): res5.key: "<<res5.key<<", res5.value: "<<res5.value<<": udata5.key: "<<udata5.key<<", udata5.value: "<<udata5.value<<", data5.key: "<<data5.key<<", data5.value: "<<data5.value<<endl; }
		if(res5.key != udata5.key){ cout<<"synchronize: vid: "<<vid[5][0]<<", _vmask["<<i<<"].data[5].key: "<<_vmask[i].data[5].key<<endl; }
		if(res5.value != udata5.value){ cout<<"synchronize: vid: "<<vid[5][1]<<", _vmask["<<i<<"].data[5].value: "<<_vmask[i].data[5].value<<endl; }
		if(false){ cout<<"SYNCHRONIZE FUNC SEEN @ (vid1: "<<vid[6][0]<<" & vid2: "<<vid[6][1]<<"): res6.key: "<<res6.key<<", res6.value: "<<res6.value<<": udata6.key: "<<udata6.key<<", udata6.value: "<<udata6.value<<", data6.key: "<<data6.key<<", data6.value: "<<data6.value<<endl; }
		if(res6.key != udata6.key){ cout<<"synchronize: vid: "<<vid[6][0]<<", _vmask["<<i<<"].data[6].key: "<<_vmask[i].data[6].key<<endl; }
		if(res6.value != udata6.value){ cout<<"synchronize: vid: "<<vid[6][1]<<", _vmask["<<i<<"].data[6].value: "<<_vmask[i].data[6].value<<endl; }
		if(false){ cout<<"SYNCHRONIZE FUNC SEEN @ (vid1: "<<vid[7][0]<<" & vid2: "<<vid[7][1]<<"): res7.key: "<<res7.key<<", res7.value: "<<res7.value<<": udata7.key: "<<udata7.key<<", udata7.value: "<<udata7.value<<", data7.key: "<<data7.key<<", data7.value: "<<data7.value<<endl; }
		if(res7.key != udata7.key){ cout<<"synchronize: vid: "<<vid[7][0]<<", _vmask["<<i<<"].data[7].key: "<<_vmask[i].data[7].key<<endl; }
		if(res7.value != udata7.value){ cout<<"synchronize: vid: "<<vid[7][1]<<", _vmask["<<i<<"].data[7].value: "<<_vmask[i].data[7].value<<endl; }
		if(false){ cout<<"SYNCHRONIZE FUNC SEEN @ (vid1: "<<vid[8][0]<<" & vid2: "<<vid[8][1]<<"): res8.key: "<<res8.key<<", res8.value: "<<res8.value<<": udata8.key: "<<udata8.key<<", udata8.value: "<<udata8.value<<", data8.key: "<<data8.key<<", data8.value: "<<data8.value<<endl; }
		if(res8.key != udata8.key){ cout<<"synchronize: vid: "<<vid[8][0]<<", _vmask["<<i<<"].data[8].key: "<<_vmask[i].data[8].key<<endl; }
		if(res8.value != udata8.value){ cout<<"synchronize: vid: "<<vid[8][1]<<", _vmask["<<i<<"].data[8].value: "<<_vmask[i].data[8].value<<endl; }
		if(false){ cout<<"SYNCHRONIZE FUNC SEEN @ (vid1: "<<vid[9][0]<<" & vid2: "<<vid[9][1]<<"): res9.key: "<<res9.key<<", res9.value: "<<res9.value<<": udata9.key: "<<udata9.key<<", udata9.value: "<<udata9.value<<", data9.key: "<<data9.key<<", data9.value: "<<data9.value<<endl; }
		if(res9.key != udata9.key){ cout<<"synchronize: vid: "<<vid[9][0]<<", _vmask["<<i<<"].data[9].key: "<<_vmask[i].data[9].key<<endl; }
		if(res9.value != udata9.value){ cout<<"synchronize: vid: "<<vid[9][1]<<", _vmask["<<i<<"].data[9].value: "<<_vmask[i].data[9].value<<endl; }
		if(false){ cout<<"SYNCHRONIZE FUNC SEEN @ (vid1: "<<vid[10][0]<<" & vid2: "<<vid[10][1]<<"): res10.key: "<<res10.key<<", res10.value: "<<res10.value<<": udata10.key: "<<udata10.key<<", udata10.value: "<<udata10.value<<", data10.key: "<<data10.key<<", data10.value: "<<data10.value<<endl; }
		if(res10.key != udata10.key){ cout<<"synchronize: vid: "<<vid[10][0]<<", _vmask["<<i<<"].data[10].key: "<<_vmask[i].data[10].key<<endl; }
		if(res10.value != udata10.value){ cout<<"synchronize: vid: "<<vid[10][1]<<", _vmask["<<i<<"].data[10].value: "<<_vmask[i].data[10].value<<endl; }
		if(false){ cout<<"SYNCHRONIZE FUNC SEEN @ (vid1: "<<vid[11][0]<<" & vid2: "<<vid[11][1]<<"): res11.key: "<<res11.key<<", res11.value: "<<res11.value<<": udata11.key: "<<udata11.key<<", udata11.value: "<<udata11.value<<", data11.key: "<<data11.key<<", data11.value: "<<data11.value<<endl; }
		if(res11.key != udata11.key){ cout<<"synchronize: vid: "<<vid[11][0]<<", _vmask["<<i<<"].data[11].key: "<<_vmask[i].data[11].key<<endl; }
		if(res11.value != udata11.value){ cout<<"synchronize: vid: "<<vid[11][1]<<", _vmask["<<i<<"].data[11].value: "<<_vmask[i].data[11].value<<endl; }
		if(false){ cout<<"SYNCHRONIZE FUNC SEEN @ (vid1: "<<vid[12][0]<<" & vid2: "<<vid[12][1]<<"): res12.key: "<<res12.key<<", res12.value: "<<res12.value<<": udata12.key: "<<udata12.key<<", udata12.value: "<<udata12.value<<", data12.key: "<<data12.key<<", data12.value: "<<data12.value<<endl; }
		if(res12.key != udata12.key){ cout<<"synchronize: vid: "<<vid[12][0]<<", _vmask["<<i<<"].data[12].key: "<<_vmask[i].data[12].key<<endl; }
		if(res12.value != udata12.value){ cout<<"synchronize: vid: "<<vid[12][1]<<", _vmask["<<i<<"].data[12].value: "<<_vmask[i].data[12].value<<endl; }
		if(false){ cout<<"SYNCHRONIZE FUNC SEEN @ (vid1: "<<vid[13][0]<<" & vid2: "<<vid[13][1]<<"): res13.key: "<<res13.key<<", res13.value: "<<res13.value<<": udata13.key: "<<udata13.key<<", udata13.value: "<<udata13.value<<", data13.key: "<<data13.key<<", data13.value: "<<data13.value<<endl; }
		if(res13.key != udata13.key){ cout<<"synchronize: vid: "<<vid[13][0]<<", _vmask["<<i<<"].data[13].key: "<<_vmask[i].data[13].key<<endl; }
		if(res13.value != udata13.value){ cout<<"synchronize: vid: "<<vid[13][1]<<", _vmask["<<i<<"].data[13].value: "<<_vmask[i].data[13].value<<endl; }
		if(false){ cout<<"SYNCHRONIZE FUNC SEEN @ (vid1: "<<vid[14][0]<<" & vid2: "<<vid[14][1]<<"): res14.key: "<<res14.key<<", res14.value: "<<res14.value<<": udata14.key: "<<udata14.key<<", udata14.value: "<<udata14.value<<", data14.key: "<<data14.key<<", data14.value: "<<data14.value<<endl; }
		if(res14.key != udata14.key){ cout<<"synchronize: vid: "<<vid[14][0]<<", _vmask["<<i<<"].data[14].key: "<<_vmask[i].data[14].key<<endl; }
		if(res14.value != udata14.value){ cout<<"synchronize: vid: "<<vid[14][1]<<", _vmask["<<i<<"].data[14].value: "<<_vmask[i].data[14].value<<endl; }
		if(false){ cout<<"SYNCHRONIZE FUNC SEEN @ (vid1: "<<vid[15][0]<<" & vid2: "<<vid[15][1]<<"): res15.key: "<<res15.key<<", res15.value: "<<res15.value<<": udata15.key: "<<udata15.key<<", udata15.value: "<<udata15.value<<", data15.key: "<<data15.key<<", data15.value: "<<data15.value<<endl; }
		if(res15.key != udata15.key){ cout<<"synchronize: vid: "<<vid[15][0]<<", _vmask["<<i<<"].data[15].key: "<<_vmask[i].data[15].key<<endl; }
		if(res15.value != udata15.value){ cout<<"synchronize: vid: "<<vid[15][1]<<", _vmask["<<i<<"].data[15].value: "<<_vmask[i].data[15].value<<endl; }
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
	
	// replicate to vbuffers
	SYNCHRONIZE_LOOP2: for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){
	#pragma HLS PIPELINE II=1
		vbuffer0_1[0][BUFFERBASE_VDATA + k] = synvbuffer[0][k];
		vbuffer0_1[1][BUFFERBASE_VDATA + k] = synvbuffer[1][k];
		vbuffer0_1[2][BUFFERBASE_VDATA + k] = synvbuffer[2][k];
		vbuffer0_1[3][BUFFERBASE_VDATA + k] = synvbuffer[3][k];
		vbuffer0_1[4][BUFFERBASE_VDATA + k] = synvbuffer[4][k];
		vbuffer0_1[5][BUFFERBASE_VDATA + k] = synvbuffer[5][k];
		vbuffer0_1[6][BUFFERBASE_VDATA + k] = synvbuffer[6][k];
		vbuffer0_1[7][BUFFERBASE_VDATA + k] = synvbuffer[7][k];
		vbuffer1_1[0][BUFFERBASE_VDATA + k] = synvbuffer[0][k];
		vbuffer1_1[1][BUFFERBASE_VDATA + k] = synvbuffer[1][k];
		vbuffer1_1[2][BUFFERBASE_VDATA + k] = synvbuffer[2][k];
		vbuffer1_1[3][BUFFERBASE_VDATA + k] = synvbuffer[3][k];
		vbuffer1_1[4][BUFFERBASE_VDATA + k] = synvbuffer[4][k];
		vbuffer1_1[5][BUFFERBASE_VDATA + k] = synvbuffer[5][k];
		vbuffer1_1[6][BUFFERBASE_VDATA + k] = synvbuffer[6][k];
		vbuffer1_1[7][BUFFERBASE_VDATA + k] = synvbuffer[7][k];
		vbuffer2_1[0][BUFFERBASE_VDATA + k] = synvbuffer[0][k];
		vbuffer2_1[1][BUFFERBASE_VDATA + k] = synvbuffer[1][k];
		vbuffer2_1[2][BUFFERBASE_VDATA + k] = synvbuffer[2][k];
		vbuffer2_1[3][BUFFERBASE_VDATA + k] = synvbuffer[3][k];
		vbuffer2_1[4][BUFFERBASE_VDATA + k] = synvbuffer[4][k];
		vbuffer2_1[5][BUFFERBASE_VDATA + k] = synvbuffer[5][k];
		vbuffer2_1[6][BUFFERBASE_VDATA + k] = synvbuffer[6][k];
		vbuffer2_1[7][BUFFERBASE_VDATA + k] = synvbuffer[7][k];
		vbuffer3_1[0][BUFFERBASE_VDATA + k] = synvbuffer[0][k];
		vbuffer3_1[1][BUFFERBASE_VDATA + k] = synvbuffer[1][k];
		vbuffer3_1[2][BUFFERBASE_VDATA + k] = synvbuffer[2][k];
		vbuffer3_1[3][BUFFERBASE_VDATA + k] = synvbuffer[3][k];
		vbuffer3_1[4][BUFFERBASE_VDATA + k] = synvbuffer[4][k];
		vbuffer3_1[5][BUFFERBASE_VDATA + k] = synvbuffer[5][k];
		vbuffer3_1[6][BUFFERBASE_VDATA + k] = synvbuffer[6][k];
		vbuffer3_1[7][BUFFERBASE_VDATA + k] = synvbuffer[7][k];
		vbuffer4_1[0][BUFFERBASE_VDATA + k] = synvbuffer[0][k];
		vbuffer4_1[1][BUFFERBASE_VDATA + k] = synvbuffer[1][k];
		vbuffer4_1[2][BUFFERBASE_VDATA + k] = synvbuffer[2][k];
		vbuffer4_1[3][BUFFERBASE_VDATA + k] = synvbuffer[3][k];
		vbuffer4_1[4][BUFFERBASE_VDATA + k] = synvbuffer[4][k];
		vbuffer4_1[5][BUFFERBASE_VDATA + k] = synvbuffer[5][k];
		vbuffer4_1[6][BUFFERBASE_VDATA + k] = synvbuffer[6][k];
		vbuffer4_1[7][BUFFERBASE_VDATA + k] = synvbuffer[7][k];
		vbuffer5_1[0][BUFFERBASE_VDATA + k] = synvbuffer[0][k];
		vbuffer5_1[1][BUFFERBASE_VDATA + k] = synvbuffer[1][k];
		vbuffer5_1[2][BUFFERBASE_VDATA + k] = synvbuffer[2][k];
		vbuffer5_1[3][BUFFERBASE_VDATA + k] = synvbuffer[3][k];
		vbuffer5_1[4][BUFFERBASE_VDATA + k] = synvbuffer[4][k];
		vbuffer5_1[5][BUFFERBASE_VDATA + k] = synvbuffer[5][k];
		vbuffer5_1[6][BUFFERBASE_VDATA + k] = synvbuffer[6][k];
		vbuffer5_1[7][BUFFERBASE_VDATA + k] = synvbuffer[7][k];
		vbuffer6_1[0][BUFFERBASE_VDATA + k] = synvbuffer[0][k];
		vbuffer6_1[1][BUFFERBASE_VDATA + k] = synvbuffer[1][k];
		vbuffer6_1[2][BUFFERBASE_VDATA + k] = synvbuffer[2][k];
		vbuffer6_1[3][BUFFERBASE_VDATA + k] = synvbuffer[3][k];
		vbuffer6_1[4][BUFFERBASE_VDATA + k] = synvbuffer[4][k];
		vbuffer6_1[5][BUFFERBASE_VDATA + k] = synvbuffer[5][k];
		vbuffer6_1[6][BUFFERBASE_VDATA + k] = synvbuffer[6][k];
		vbuffer6_1[7][BUFFERBASE_VDATA + k] = synvbuffer[7][k];
		vbuffer7_1[0][BUFFERBASE_VDATA + k] = synvbuffer[0][k];
		vbuffer7_1[1][BUFFERBASE_VDATA + k] = synvbuffer[1][k];
		vbuffer7_1[2][BUFFERBASE_VDATA + k] = synvbuffer[2][k];
		vbuffer7_1[3][BUFFERBASE_VDATA + k] = synvbuffer[3][k];
		vbuffer7_1[4][BUFFERBASE_VDATA + k] = synvbuffer[4][k];
		vbuffer7_1[5][BUFFERBASE_VDATA + k] = synvbuffer[5][k];
		vbuffer7_1[6][BUFFERBASE_VDATA + k] = synvbuffer[6][k];
		vbuffer7_1[7][BUFFERBASE_VDATA + k] = synvbuffer[7][k];
		vbuffer8_1[0][BUFFERBASE_VDATA + k] = synvbuffer[0][k];
		vbuffer8_1[1][BUFFERBASE_VDATA + k] = synvbuffer[1][k];
		vbuffer8_1[2][BUFFERBASE_VDATA + k] = synvbuffer[2][k];
		vbuffer8_1[3][BUFFERBASE_VDATA + k] = synvbuffer[3][k];
		vbuffer8_1[4][BUFFERBASE_VDATA + k] = synvbuffer[4][k];
		vbuffer8_1[5][BUFFERBASE_VDATA + k] = synvbuffer[5][k];
		vbuffer8_1[6][BUFFERBASE_VDATA + k] = synvbuffer[6][k];
		vbuffer8_1[7][BUFFERBASE_VDATA + k] = synvbuffer[7][k];
		vbuffer9_1[0][BUFFERBASE_VDATA + k] = synvbuffer[0][k];
		vbuffer9_1[1][BUFFERBASE_VDATA + k] = synvbuffer[1][k];
		vbuffer9_1[2][BUFFERBASE_VDATA + k] = synvbuffer[2][k];
		vbuffer9_1[3][BUFFERBASE_VDATA + k] = synvbuffer[3][k];
		vbuffer9_1[4][BUFFERBASE_VDATA + k] = synvbuffer[4][k];
		vbuffer9_1[5][BUFFERBASE_VDATA + k] = synvbuffer[5][k];
		vbuffer9_1[6][BUFFERBASE_VDATA + k] = synvbuffer[6][k];
		vbuffer9_1[7][BUFFERBASE_VDATA + k] = synvbuffer[7][k];
		vbuffer10_1[0][BUFFERBASE_VDATA + k] = synvbuffer[0][k];
		vbuffer10_1[1][BUFFERBASE_VDATA + k] = synvbuffer[1][k];
		vbuffer10_1[2][BUFFERBASE_VDATA + k] = synvbuffer[2][k];
		vbuffer10_1[3][BUFFERBASE_VDATA + k] = synvbuffer[3][k];
		vbuffer10_1[4][BUFFERBASE_VDATA + k] = synvbuffer[4][k];
		vbuffer10_1[5][BUFFERBASE_VDATA + k] = synvbuffer[5][k];
		vbuffer10_1[6][BUFFERBASE_VDATA + k] = synvbuffer[6][k];
		vbuffer10_1[7][BUFFERBASE_VDATA + k] = synvbuffer[7][k];
		vbuffer11_1[0][BUFFERBASE_VDATA + k] = synvbuffer[0][k];
		vbuffer11_1[1][BUFFERBASE_VDATA + k] = synvbuffer[1][k];
		vbuffer11_1[2][BUFFERBASE_VDATA + k] = synvbuffer[2][k];
		vbuffer11_1[3][BUFFERBASE_VDATA + k] = synvbuffer[3][k];
		vbuffer11_1[4][BUFFERBASE_VDATA + k] = synvbuffer[4][k];
		vbuffer11_1[5][BUFFERBASE_VDATA + k] = synvbuffer[5][k];
		vbuffer11_1[6][BUFFERBASE_VDATA + k] = synvbuffer[6][k];
		vbuffer11_1[7][BUFFERBASE_VDATA + k] = synvbuffer[7][k];
		vbuffer12_1[0][BUFFERBASE_VDATA + k] = synvbuffer[0][k];
		vbuffer12_1[1][BUFFERBASE_VDATA + k] = synvbuffer[1][k];
		vbuffer12_1[2][BUFFERBASE_VDATA + k] = synvbuffer[2][k];
		vbuffer12_1[3][BUFFERBASE_VDATA + k] = synvbuffer[3][k];
		vbuffer12_1[4][BUFFERBASE_VDATA + k] = synvbuffer[4][k];
		vbuffer12_1[5][BUFFERBASE_VDATA + k] = synvbuffer[5][k];
		vbuffer12_1[6][BUFFERBASE_VDATA + k] = synvbuffer[6][k];
		vbuffer12_1[7][BUFFERBASE_VDATA + k] = synvbuffer[7][k];
		vbuffer13_1[0][BUFFERBASE_VDATA + k] = synvbuffer[0][k];
		vbuffer13_1[1][BUFFERBASE_VDATA + k] = synvbuffer[1][k];
		vbuffer13_1[2][BUFFERBASE_VDATA + k] = synvbuffer[2][k];
		vbuffer13_1[3][BUFFERBASE_VDATA + k] = synvbuffer[3][k];
		vbuffer13_1[4][BUFFERBASE_VDATA + k] = synvbuffer[4][k];
		vbuffer13_1[5][BUFFERBASE_VDATA + k] = synvbuffer[5][k];
		vbuffer13_1[6][BUFFERBASE_VDATA + k] = synvbuffer[6][k];
		vbuffer13_1[7][BUFFERBASE_VDATA + k] = synvbuffer[7][k];
		vbuffer14_1[0][BUFFERBASE_VDATA + k] = synvbuffer[0][k];
		vbuffer14_1[1][BUFFERBASE_VDATA + k] = synvbuffer[1][k];
		vbuffer14_1[2][BUFFERBASE_VDATA + k] = synvbuffer[2][k];
		vbuffer14_1[3][BUFFERBASE_VDATA + k] = synvbuffer[3][k];
		vbuffer14_1[4][BUFFERBASE_VDATA + k] = synvbuffer[4][k];
		vbuffer14_1[5][BUFFERBASE_VDATA + k] = synvbuffer[5][k];
		vbuffer14_1[6][BUFFERBASE_VDATA + k] = synvbuffer[6][k];
		vbuffer14_1[7][BUFFERBASE_VDATA + k] = synvbuffer[7][k];
		vbuffer15_1[0][BUFFERBASE_VDATA + k] = synvbuffer[0][k];
		vbuffer15_1[1][BUFFERBASE_VDATA + k] = synvbuffer[1][k];
		vbuffer15_1[2][BUFFERBASE_VDATA + k] = synvbuffer[2][k];
		vbuffer15_1[3][BUFFERBASE_VDATA + k] = synvbuffer[3][k];
		vbuffer15_1[4][BUFFERBASE_VDATA + k] = synvbuffer[4][k];
		vbuffer15_1[5][BUFFERBASE_VDATA + k] = synvbuffer[5][k];
		vbuffer15_1[6][BUFFERBASE_VDATA + k] = synvbuffer[6][k];
		vbuffer15_1[7][BUFFERBASE_VDATA + k] = synvbuffer[7][k];
		vbuffer0_2[0][BUFFERBASE_VDATA + k] = synvbuffer[8][k];
		vbuffer0_2[1][BUFFERBASE_VDATA + k] = synvbuffer[9][k];
		vbuffer0_2[2][BUFFERBASE_VDATA + k] = synvbuffer[10][k];
		vbuffer0_2[3][BUFFERBASE_VDATA + k] = synvbuffer[11][k];
		vbuffer0_2[4][BUFFERBASE_VDATA + k] = synvbuffer[12][k];
		vbuffer0_2[5][BUFFERBASE_VDATA + k] = synvbuffer[13][k];
		vbuffer0_2[6][BUFFERBASE_VDATA + k] = synvbuffer[14][k];
		vbuffer0_2[7][BUFFERBASE_VDATA + k] = synvbuffer[15][k];
		vbuffer1_2[0][BUFFERBASE_VDATA + k] = synvbuffer[8][k];
		vbuffer1_2[1][BUFFERBASE_VDATA + k] = synvbuffer[9][k];
		vbuffer1_2[2][BUFFERBASE_VDATA + k] = synvbuffer[10][k];
		vbuffer1_2[3][BUFFERBASE_VDATA + k] = synvbuffer[11][k];
		vbuffer1_2[4][BUFFERBASE_VDATA + k] = synvbuffer[12][k];
		vbuffer1_2[5][BUFFERBASE_VDATA + k] = synvbuffer[13][k];
		vbuffer1_2[6][BUFFERBASE_VDATA + k] = synvbuffer[14][k];
		vbuffer1_2[7][BUFFERBASE_VDATA + k] = synvbuffer[15][k];
		vbuffer2_2[0][BUFFERBASE_VDATA + k] = synvbuffer[8][k];
		vbuffer2_2[1][BUFFERBASE_VDATA + k] = synvbuffer[9][k];
		vbuffer2_2[2][BUFFERBASE_VDATA + k] = synvbuffer[10][k];
		vbuffer2_2[3][BUFFERBASE_VDATA + k] = synvbuffer[11][k];
		vbuffer2_2[4][BUFFERBASE_VDATA + k] = synvbuffer[12][k];
		vbuffer2_2[5][BUFFERBASE_VDATA + k] = synvbuffer[13][k];
		vbuffer2_2[6][BUFFERBASE_VDATA + k] = synvbuffer[14][k];
		vbuffer2_2[7][BUFFERBASE_VDATA + k] = synvbuffer[15][k];
		vbuffer3_2[0][BUFFERBASE_VDATA + k] = synvbuffer[8][k];
		vbuffer3_2[1][BUFFERBASE_VDATA + k] = synvbuffer[9][k];
		vbuffer3_2[2][BUFFERBASE_VDATA + k] = synvbuffer[10][k];
		vbuffer3_2[3][BUFFERBASE_VDATA + k] = synvbuffer[11][k];
		vbuffer3_2[4][BUFFERBASE_VDATA + k] = synvbuffer[12][k];
		vbuffer3_2[5][BUFFERBASE_VDATA + k] = synvbuffer[13][k];
		vbuffer3_2[6][BUFFERBASE_VDATA + k] = synvbuffer[14][k];
		vbuffer3_2[7][BUFFERBASE_VDATA + k] = synvbuffer[15][k];
		vbuffer4_2[0][BUFFERBASE_VDATA + k] = synvbuffer[8][k];
		vbuffer4_2[1][BUFFERBASE_VDATA + k] = synvbuffer[9][k];
		vbuffer4_2[2][BUFFERBASE_VDATA + k] = synvbuffer[10][k];
		vbuffer4_2[3][BUFFERBASE_VDATA + k] = synvbuffer[11][k];
		vbuffer4_2[4][BUFFERBASE_VDATA + k] = synvbuffer[12][k];
		vbuffer4_2[5][BUFFERBASE_VDATA + k] = synvbuffer[13][k];
		vbuffer4_2[6][BUFFERBASE_VDATA + k] = synvbuffer[14][k];
		vbuffer4_2[7][BUFFERBASE_VDATA + k] = synvbuffer[15][k];
		vbuffer5_2[0][BUFFERBASE_VDATA + k] = synvbuffer[8][k];
		vbuffer5_2[1][BUFFERBASE_VDATA + k] = synvbuffer[9][k];
		vbuffer5_2[2][BUFFERBASE_VDATA + k] = synvbuffer[10][k];
		vbuffer5_2[3][BUFFERBASE_VDATA + k] = synvbuffer[11][k];
		vbuffer5_2[4][BUFFERBASE_VDATA + k] = synvbuffer[12][k];
		vbuffer5_2[5][BUFFERBASE_VDATA + k] = synvbuffer[13][k];
		vbuffer5_2[6][BUFFERBASE_VDATA + k] = synvbuffer[14][k];
		vbuffer5_2[7][BUFFERBASE_VDATA + k] = synvbuffer[15][k];
		vbuffer6_2[0][BUFFERBASE_VDATA + k] = synvbuffer[8][k];
		vbuffer6_2[1][BUFFERBASE_VDATA + k] = synvbuffer[9][k];
		vbuffer6_2[2][BUFFERBASE_VDATA + k] = synvbuffer[10][k];
		vbuffer6_2[3][BUFFERBASE_VDATA + k] = synvbuffer[11][k];
		vbuffer6_2[4][BUFFERBASE_VDATA + k] = synvbuffer[12][k];
		vbuffer6_2[5][BUFFERBASE_VDATA + k] = synvbuffer[13][k];
		vbuffer6_2[6][BUFFERBASE_VDATA + k] = synvbuffer[14][k];
		vbuffer6_2[7][BUFFERBASE_VDATA + k] = synvbuffer[15][k];
		vbuffer7_2[0][BUFFERBASE_VDATA + k] = synvbuffer[8][k];
		vbuffer7_2[1][BUFFERBASE_VDATA + k] = synvbuffer[9][k];
		vbuffer7_2[2][BUFFERBASE_VDATA + k] = synvbuffer[10][k];
		vbuffer7_2[3][BUFFERBASE_VDATA + k] = synvbuffer[11][k];
		vbuffer7_2[4][BUFFERBASE_VDATA + k] = synvbuffer[12][k];
		vbuffer7_2[5][BUFFERBASE_VDATA + k] = synvbuffer[13][k];
		vbuffer7_2[6][BUFFERBASE_VDATA + k] = synvbuffer[14][k];
		vbuffer7_2[7][BUFFERBASE_VDATA + k] = synvbuffer[15][k];
		vbuffer8_2[0][BUFFERBASE_VDATA + k] = synvbuffer[8][k];
		vbuffer8_2[1][BUFFERBASE_VDATA + k] = synvbuffer[9][k];
		vbuffer8_2[2][BUFFERBASE_VDATA + k] = synvbuffer[10][k];
		vbuffer8_2[3][BUFFERBASE_VDATA + k] = synvbuffer[11][k];
		vbuffer8_2[4][BUFFERBASE_VDATA + k] = synvbuffer[12][k];
		vbuffer8_2[5][BUFFERBASE_VDATA + k] = synvbuffer[13][k];
		vbuffer8_2[6][BUFFERBASE_VDATA + k] = synvbuffer[14][k];
		vbuffer8_2[7][BUFFERBASE_VDATA + k] = synvbuffer[15][k];
		vbuffer9_2[0][BUFFERBASE_VDATA + k] = synvbuffer[8][k];
		vbuffer9_2[1][BUFFERBASE_VDATA + k] = synvbuffer[9][k];
		vbuffer9_2[2][BUFFERBASE_VDATA + k] = synvbuffer[10][k];
		vbuffer9_2[3][BUFFERBASE_VDATA + k] = synvbuffer[11][k];
		vbuffer9_2[4][BUFFERBASE_VDATA + k] = synvbuffer[12][k];
		vbuffer9_2[5][BUFFERBASE_VDATA + k] = synvbuffer[13][k];
		vbuffer9_2[6][BUFFERBASE_VDATA + k] = synvbuffer[14][k];
		vbuffer9_2[7][BUFFERBASE_VDATA + k] = synvbuffer[15][k];
		vbuffer10_2[0][BUFFERBASE_VDATA + k] = synvbuffer[8][k];
		vbuffer10_2[1][BUFFERBASE_VDATA + k] = synvbuffer[9][k];
		vbuffer10_2[2][BUFFERBASE_VDATA + k] = synvbuffer[10][k];
		vbuffer10_2[3][BUFFERBASE_VDATA + k] = synvbuffer[11][k];
		vbuffer10_2[4][BUFFERBASE_VDATA + k] = synvbuffer[12][k];
		vbuffer10_2[5][BUFFERBASE_VDATA + k] = synvbuffer[13][k];
		vbuffer10_2[6][BUFFERBASE_VDATA + k] = synvbuffer[14][k];
		vbuffer10_2[7][BUFFERBASE_VDATA + k] = synvbuffer[15][k];
		vbuffer11_2[0][BUFFERBASE_VDATA + k] = synvbuffer[8][k];
		vbuffer11_2[1][BUFFERBASE_VDATA + k] = synvbuffer[9][k];
		vbuffer11_2[2][BUFFERBASE_VDATA + k] = synvbuffer[10][k];
		vbuffer11_2[3][BUFFERBASE_VDATA + k] = synvbuffer[11][k];
		vbuffer11_2[4][BUFFERBASE_VDATA + k] = synvbuffer[12][k];
		vbuffer11_2[5][BUFFERBASE_VDATA + k] = synvbuffer[13][k];
		vbuffer11_2[6][BUFFERBASE_VDATA + k] = synvbuffer[14][k];
		vbuffer11_2[7][BUFFERBASE_VDATA + k] = synvbuffer[15][k];
		vbuffer12_2[0][BUFFERBASE_VDATA + k] = synvbuffer[8][k];
		vbuffer12_2[1][BUFFERBASE_VDATA + k] = synvbuffer[9][k];
		vbuffer12_2[2][BUFFERBASE_VDATA + k] = synvbuffer[10][k];
		vbuffer12_2[3][BUFFERBASE_VDATA + k] = synvbuffer[11][k];
		vbuffer12_2[4][BUFFERBASE_VDATA + k] = synvbuffer[12][k];
		vbuffer12_2[5][BUFFERBASE_VDATA + k] = synvbuffer[13][k];
		vbuffer12_2[6][BUFFERBASE_VDATA + k] = synvbuffer[14][k];
		vbuffer12_2[7][BUFFERBASE_VDATA + k] = synvbuffer[15][k];
		vbuffer13_2[0][BUFFERBASE_VDATA + k] = synvbuffer[8][k];
		vbuffer13_2[1][BUFFERBASE_VDATA + k] = synvbuffer[9][k];
		vbuffer13_2[2][BUFFERBASE_VDATA + k] = synvbuffer[10][k];
		vbuffer13_2[3][BUFFERBASE_VDATA + k] = synvbuffer[11][k];
		vbuffer13_2[4][BUFFERBASE_VDATA + k] = synvbuffer[12][k];
		vbuffer13_2[5][BUFFERBASE_VDATA + k] = synvbuffer[13][k];
		vbuffer13_2[6][BUFFERBASE_VDATA + k] = synvbuffer[14][k];
		vbuffer13_2[7][BUFFERBASE_VDATA + k] = synvbuffer[15][k];
		vbuffer14_2[0][BUFFERBASE_VDATA + k] = synvbuffer[8][k];
		vbuffer14_2[1][BUFFERBASE_VDATA + k] = synvbuffer[9][k];
		vbuffer14_2[2][BUFFERBASE_VDATA + k] = synvbuffer[10][k];
		vbuffer14_2[3][BUFFERBASE_VDATA + k] = synvbuffer[11][k];
		vbuffer14_2[4][BUFFERBASE_VDATA + k] = synvbuffer[12][k];
		vbuffer14_2[5][BUFFERBASE_VDATA + k] = synvbuffer[13][k];
		vbuffer14_2[6][BUFFERBASE_VDATA + k] = synvbuffer[14][k];
		vbuffer14_2[7][BUFFERBASE_VDATA + k] = synvbuffer[15][k];
		vbuffer15_2[0][BUFFERBASE_VDATA + k] = synvbuffer[8][k];
		vbuffer15_2[1][BUFFERBASE_VDATA + k] = synvbuffer[9][k];
		vbuffer15_2[2][BUFFERBASE_VDATA + k] = synvbuffer[10][k];
		vbuffer15_2[3][BUFFERBASE_VDATA + k] = synvbuffer[11][k];
		vbuffer15_2[4][BUFFERBASE_VDATA + k] = synvbuffer[12][k];
		vbuffer15_2[5][BUFFERBASE_VDATA + k] = synvbuffer[13][k];
		vbuffer15_2[6][BUFFERBASE_VDATA + k] = synvbuffer[14][k];
		vbuffer15_2[7][BUFFERBASE_VDATA + k] = synvbuffer[15][k];
	}
	
	// replicate to vmasks
	SYNCHRONIZE_LOOP3: for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){
	#pragma HLS PIPELINE II=1
		uint32_type mask = convertvmasktouint32(_vmask, k);
		#ifdef _WIDEWORD
		vmask0[k].data[0].key = mask.range(1, 0);
		vmask0[k].data[0].value = mask.range(2, 1);
		vmask0[k].data[1].key = mask.range(3, 2);
		vmask0[k].data[1].value = mask.range(4, 3);
		vmask0[k].data[2].key = mask.range(5, 4);
		vmask0[k].data[2].value = mask.range(6, 5);
		vmask0[k].data[3].key = mask.range(7, 6);
		vmask0[k].data[3].value = mask.range(8, 7);
		vmask0[k].data[4].key = mask.range(9, 8);
		vmask0[k].data[4].value = mask.range(10, 9);
		vmask0[k].data[5].key = mask.range(11, 10);
		vmask0[k].data[5].value = mask.range(12, 11);
		vmask0[k].data[6].key = mask.range(13, 12);
		vmask0[k].data[6].value = mask.range(14, 13);
		vmask0[k].data[7].key = mask.range(15, 14);
		vmask0[k].data[7].value = mask.range(16, 15);
		vmask0[k].data[8].key = mask.range(17, 16);
		vmask0[k].data[8].value = mask.range(18, 17);
		vmask0[k].data[9].key = mask.range(19, 18);
		vmask0[k].data[9].value = mask.range(20, 19);
		vmask0[k].data[10].key = mask.range(21, 20);
		vmask0[k].data[10].value = mask.range(22, 21);
		vmask0[k].data[11].key = mask.range(23, 22);
		vmask0[k].data[11].value = mask.range(24, 23);
		vmask0[k].data[12].key = mask.range(25, 24);
		vmask0[k].data[12].value = mask.range(26, 25);
		vmask0[k].data[13].key = mask.range(27, 26);
		vmask0[k].data[13].value = mask.range(28, 27);
		vmask0[k].data[14].key = mask.range(29, 28);
		vmask0[k].data[14].value = mask.range(30, 29);
		vmask0[k].data[15].key = mask.range(31, 30);
		vmask0[k].data[15].value = mask.range(32, 31);
		vmask1[k].data[0].key = mask.range(1, 0);
		vmask1[k].data[0].value = mask.range(2, 1);
		vmask1[k].data[1].key = mask.range(3, 2);
		vmask1[k].data[1].value = mask.range(4, 3);
		vmask1[k].data[2].key = mask.range(5, 4);
		vmask1[k].data[2].value = mask.range(6, 5);
		vmask1[k].data[3].key = mask.range(7, 6);
		vmask1[k].data[3].value = mask.range(8, 7);
		vmask1[k].data[4].key = mask.range(9, 8);
		vmask1[k].data[4].value = mask.range(10, 9);
		vmask1[k].data[5].key = mask.range(11, 10);
		vmask1[k].data[5].value = mask.range(12, 11);
		vmask1[k].data[6].key = mask.range(13, 12);
		vmask1[k].data[6].value = mask.range(14, 13);
		vmask1[k].data[7].key = mask.range(15, 14);
		vmask1[k].data[7].value = mask.range(16, 15);
		vmask1[k].data[8].key = mask.range(17, 16);
		vmask1[k].data[8].value = mask.range(18, 17);
		vmask1[k].data[9].key = mask.range(19, 18);
		vmask1[k].data[9].value = mask.range(20, 19);
		vmask1[k].data[10].key = mask.range(21, 20);
		vmask1[k].data[10].value = mask.range(22, 21);
		vmask1[k].data[11].key = mask.range(23, 22);
		vmask1[k].data[11].value = mask.range(24, 23);
		vmask1[k].data[12].key = mask.range(25, 24);
		vmask1[k].data[12].value = mask.range(26, 25);
		vmask1[k].data[13].key = mask.range(27, 26);
		vmask1[k].data[13].value = mask.range(28, 27);
		vmask1[k].data[14].key = mask.range(29, 28);
		vmask1[k].data[14].value = mask.range(30, 29);
		vmask1[k].data[15].key = mask.range(31, 30);
		vmask1[k].data[15].value = mask.range(32, 31);
		vmask2[k].data[0].key = mask.range(1, 0);
		vmask2[k].data[0].value = mask.range(2, 1);
		vmask2[k].data[1].key = mask.range(3, 2);
		vmask2[k].data[1].value = mask.range(4, 3);
		vmask2[k].data[2].key = mask.range(5, 4);
		vmask2[k].data[2].value = mask.range(6, 5);
		vmask2[k].data[3].key = mask.range(7, 6);
		vmask2[k].data[3].value = mask.range(8, 7);
		vmask2[k].data[4].key = mask.range(9, 8);
		vmask2[k].data[4].value = mask.range(10, 9);
		vmask2[k].data[5].key = mask.range(11, 10);
		vmask2[k].data[5].value = mask.range(12, 11);
		vmask2[k].data[6].key = mask.range(13, 12);
		vmask2[k].data[6].value = mask.range(14, 13);
		vmask2[k].data[7].key = mask.range(15, 14);
		vmask2[k].data[7].value = mask.range(16, 15);
		vmask2[k].data[8].key = mask.range(17, 16);
		vmask2[k].data[8].value = mask.range(18, 17);
		vmask2[k].data[9].key = mask.range(19, 18);
		vmask2[k].data[9].value = mask.range(20, 19);
		vmask2[k].data[10].key = mask.range(21, 20);
		vmask2[k].data[10].value = mask.range(22, 21);
		vmask2[k].data[11].key = mask.range(23, 22);
		vmask2[k].data[11].value = mask.range(24, 23);
		vmask2[k].data[12].key = mask.range(25, 24);
		vmask2[k].data[12].value = mask.range(26, 25);
		vmask2[k].data[13].key = mask.range(27, 26);
		vmask2[k].data[13].value = mask.range(28, 27);
		vmask2[k].data[14].key = mask.range(29, 28);
		vmask2[k].data[14].value = mask.range(30, 29);
		vmask2[k].data[15].key = mask.range(31, 30);
		vmask2[k].data[15].value = mask.range(32, 31);
		vmask3[k].data[0].key = mask.range(1, 0);
		vmask3[k].data[0].value = mask.range(2, 1);
		vmask3[k].data[1].key = mask.range(3, 2);
		vmask3[k].data[1].value = mask.range(4, 3);
		vmask3[k].data[2].key = mask.range(5, 4);
		vmask3[k].data[2].value = mask.range(6, 5);
		vmask3[k].data[3].key = mask.range(7, 6);
		vmask3[k].data[3].value = mask.range(8, 7);
		vmask3[k].data[4].key = mask.range(9, 8);
		vmask3[k].data[4].value = mask.range(10, 9);
		vmask3[k].data[5].key = mask.range(11, 10);
		vmask3[k].data[5].value = mask.range(12, 11);
		vmask3[k].data[6].key = mask.range(13, 12);
		vmask3[k].data[6].value = mask.range(14, 13);
		vmask3[k].data[7].key = mask.range(15, 14);
		vmask3[k].data[7].value = mask.range(16, 15);
		vmask3[k].data[8].key = mask.range(17, 16);
		vmask3[k].data[8].value = mask.range(18, 17);
		vmask3[k].data[9].key = mask.range(19, 18);
		vmask3[k].data[9].value = mask.range(20, 19);
		vmask3[k].data[10].key = mask.range(21, 20);
		vmask3[k].data[10].value = mask.range(22, 21);
		vmask3[k].data[11].key = mask.range(23, 22);
		vmask3[k].data[11].value = mask.range(24, 23);
		vmask3[k].data[12].key = mask.range(25, 24);
		vmask3[k].data[12].value = mask.range(26, 25);
		vmask3[k].data[13].key = mask.range(27, 26);
		vmask3[k].data[13].value = mask.range(28, 27);
		vmask3[k].data[14].key = mask.range(29, 28);
		vmask3[k].data[14].value = mask.range(30, 29);
		vmask3[k].data[15].key = mask.range(31, 30);
		vmask3[k].data[15].value = mask.range(32, 31);
		vmask4[k].data[0].key = mask.range(1, 0);
		vmask4[k].data[0].value = mask.range(2, 1);
		vmask4[k].data[1].key = mask.range(3, 2);
		vmask4[k].data[1].value = mask.range(4, 3);
		vmask4[k].data[2].key = mask.range(5, 4);
		vmask4[k].data[2].value = mask.range(6, 5);
		vmask4[k].data[3].key = mask.range(7, 6);
		vmask4[k].data[3].value = mask.range(8, 7);
		vmask4[k].data[4].key = mask.range(9, 8);
		vmask4[k].data[4].value = mask.range(10, 9);
		vmask4[k].data[5].key = mask.range(11, 10);
		vmask4[k].data[5].value = mask.range(12, 11);
		vmask4[k].data[6].key = mask.range(13, 12);
		vmask4[k].data[6].value = mask.range(14, 13);
		vmask4[k].data[7].key = mask.range(15, 14);
		vmask4[k].data[7].value = mask.range(16, 15);
		vmask4[k].data[8].key = mask.range(17, 16);
		vmask4[k].data[8].value = mask.range(18, 17);
		vmask4[k].data[9].key = mask.range(19, 18);
		vmask4[k].data[9].value = mask.range(20, 19);
		vmask4[k].data[10].key = mask.range(21, 20);
		vmask4[k].data[10].value = mask.range(22, 21);
		vmask4[k].data[11].key = mask.range(23, 22);
		vmask4[k].data[11].value = mask.range(24, 23);
		vmask4[k].data[12].key = mask.range(25, 24);
		vmask4[k].data[12].value = mask.range(26, 25);
		vmask4[k].data[13].key = mask.range(27, 26);
		vmask4[k].data[13].value = mask.range(28, 27);
		vmask4[k].data[14].key = mask.range(29, 28);
		vmask4[k].data[14].value = mask.range(30, 29);
		vmask4[k].data[15].key = mask.range(31, 30);
		vmask4[k].data[15].value = mask.range(32, 31);
		vmask5[k].data[0].key = mask.range(1, 0);
		vmask5[k].data[0].value = mask.range(2, 1);
		vmask5[k].data[1].key = mask.range(3, 2);
		vmask5[k].data[1].value = mask.range(4, 3);
		vmask5[k].data[2].key = mask.range(5, 4);
		vmask5[k].data[2].value = mask.range(6, 5);
		vmask5[k].data[3].key = mask.range(7, 6);
		vmask5[k].data[3].value = mask.range(8, 7);
		vmask5[k].data[4].key = mask.range(9, 8);
		vmask5[k].data[4].value = mask.range(10, 9);
		vmask5[k].data[5].key = mask.range(11, 10);
		vmask5[k].data[5].value = mask.range(12, 11);
		vmask5[k].data[6].key = mask.range(13, 12);
		vmask5[k].data[6].value = mask.range(14, 13);
		vmask5[k].data[7].key = mask.range(15, 14);
		vmask5[k].data[7].value = mask.range(16, 15);
		vmask5[k].data[8].key = mask.range(17, 16);
		vmask5[k].data[8].value = mask.range(18, 17);
		vmask5[k].data[9].key = mask.range(19, 18);
		vmask5[k].data[9].value = mask.range(20, 19);
		vmask5[k].data[10].key = mask.range(21, 20);
		vmask5[k].data[10].value = mask.range(22, 21);
		vmask5[k].data[11].key = mask.range(23, 22);
		vmask5[k].data[11].value = mask.range(24, 23);
		vmask5[k].data[12].key = mask.range(25, 24);
		vmask5[k].data[12].value = mask.range(26, 25);
		vmask5[k].data[13].key = mask.range(27, 26);
		vmask5[k].data[13].value = mask.range(28, 27);
		vmask5[k].data[14].key = mask.range(29, 28);
		vmask5[k].data[14].value = mask.range(30, 29);
		vmask5[k].data[15].key = mask.range(31, 30);
		vmask5[k].data[15].value = mask.range(32, 31);
		vmask6[k].data[0].key = mask.range(1, 0);
		vmask6[k].data[0].value = mask.range(2, 1);
		vmask6[k].data[1].key = mask.range(3, 2);
		vmask6[k].data[1].value = mask.range(4, 3);
		vmask6[k].data[2].key = mask.range(5, 4);
		vmask6[k].data[2].value = mask.range(6, 5);
		vmask6[k].data[3].key = mask.range(7, 6);
		vmask6[k].data[3].value = mask.range(8, 7);
		vmask6[k].data[4].key = mask.range(9, 8);
		vmask6[k].data[4].value = mask.range(10, 9);
		vmask6[k].data[5].key = mask.range(11, 10);
		vmask6[k].data[5].value = mask.range(12, 11);
		vmask6[k].data[6].key = mask.range(13, 12);
		vmask6[k].data[6].value = mask.range(14, 13);
		vmask6[k].data[7].key = mask.range(15, 14);
		vmask6[k].data[7].value = mask.range(16, 15);
		vmask6[k].data[8].key = mask.range(17, 16);
		vmask6[k].data[8].value = mask.range(18, 17);
		vmask6[k].data[9].key = mask.range(19, 18);
		vmask6[k].data[9].value = mask.range(20, 19);
		vmask6[k].data[10].key = mask.range(21, 20);
		vmask6[k].data[10].value = mask.range(22, 21);
		vmask6[k].data[11].key = mask.range(23, 22);
		vmask6[k].data[11].value = mask.range(24, 23);
		vmask6[k].data[12].key = mask.range(25, 24);
		vmask6[k].data[12].value = mask.range(26, 25);
		vmask6[k].data[13].key = mask.range(27, 26);
		vmask6[k].data[13].value = mask.range(28, 27);
		vmask6[k].data[14].key = mask.range(29, 28);
		vmask6[k].data[14].value = mask.range(30, 29);
		vmask6[k].data[15].key = mask.range(31, 30);
		vmask6[k].data[15].value = mask.range(32, 31);
		vmask7[k].data[0].key = mask.range(1, 0);
		vmask7[k].data[0].value = mask.range(2, 1);
		vmask7[k].data[1].key = mask.range(3, 2);
		vmask7[k].data[1].value = mask.range(4, 3);
		vmask7[k].data[2].key = mask.range(5, 4);
		vmask7[k].data[2].value = mask.range(6, 5);
		vmask7[k].data[3].key = mask.range(7, 6);
		vmask7[k].data[3].value = mask.range(8, 7);
		vmask7[k].data[4].key = mask.range(9, 8);
		vmask7[k].data[4].value = mask.range(10, 9);
		vmask7[k].data[5].key = mask.range(11, 10);
		vmask7[k].data[5].value = mask.range(12, 11);
		vmask7[k].data[6].key = mask.range(13, 12);
		vmask7[k].data[6].value = mask.range(14, 13);
		vmask7[k].data[7].key = mask.range(15, 14);
		vmask7[k].data[7].value = mask.range(16, 15);
		vmask7[k].data[8].key = mask.range(17, 16);
		vmask7[k].data[8].value = mask.range(18, 17);
		vmask7[k].data[9].key = mask.range(19, 18);
		vmask7[k].data[9].value = mask.range(20, 19);
		vmask7[k].data[10].key = mask.range(21, 20);
		vmask7[k].data[10].value = mask.range(22, 21);
		vmask7[k].data[11].key = mask.range(23, 22);
		vmask7[k].data[11].value = mask.range(24, 23);
		vmask7[k].data[12].key = mask.range(25, 24);
		vmask7[k].data[12].value = mask.range(26, 25);
		vmask7[k].data[13].key = mask.range(27, 26);
		vmask7[k].data[13].value = mask.range(28, 27);
		vmask7[k].data[14].key = mask.range(29, 28);
		vmask7[k].data[14].value = mask.range(30, 29);
		vmask7[k].data[15].key = mask.range(31, 30);
		vmask7[k].data[15].value = mask.range(32, 31);
		vmask8[k].data[0].key = mask.range(1, 0);
		vmask8[k].data[0].value = mask.range(2, 1);
		vmask8[k].data[1].key = mask.range(3, 2);
		vmask8[k].data[1].value = mask.range(4, 3);
		vmask8[k].data[2].key = mask.range(5, 4);
		vmask8[k].data[2].value = mask.range(6, 5);
		vmask8[k].data[3].key = mask.range(7, 6);
		vmask8[k].data[3].value = mask.range(8, 7);
		vmask8[k].data[4].key = mask.range(9, 8);
		vmask8[k].data[4].value = mask.range(10, 9);
		vmask8[k].data[5].key = mask.range(11, 10);
		vmask8[k].data[5].value = mask.range(12, 11);
		vmask8[k].data[6].key = mask.range(13, 12);
		vmask8[k].data[6].value = mask.range(14, 13);
		vmask8[k].data[7].key = mask.range(15, 14);
		vmask8[k].data[7].value = mask.range(16, 15);
		vmask8[k].data[8].key = mask.range(17, 16);
		vmask8[k].data[8].value = mask.range(18, 17);
		vmask8[k].data[9].key = mask.range(19, 18);
		vmask8[k].data[9].value = mask.range(20, 19);
		vmask8[k].data[10].key = mask.range(21, 20);
		vmask8[k].data[10].value = mask.range(22, 21);
		vmask8[k].data[11].key = mask.range(23, 22);
		vmask8[k].data[11].value = mask.range(24, 23);
		vmask8[k].data[12].key = mask.range(25, 24);
		vmask8[k].data[12].value = mask.range(26, 25);
		vmask8[k].data[13].key = mask.range(27, 26);
		vmask8[k].data[13].value = mask.range(28, 27);
		vmask8[k].data[14].key = mask.range(29, 28);
		vmask8[k].data[14].value = mask.range(30, 29);
		vmask8[k].data[15].key = mask.range(31, 30);
		vmask8[k].data[15].value = mask.range(32, 31);
		vmask9[k].data[0].key = mask.range(1, 0);
		vmask9[k].data[0].value = mask.range(2, 1);
		vmask9[k].data[1].key = mask.range(3, 2);
		vmask9[k].data[1].value = mask.range(4, 3);
		vmask9[k].data[2].key = mask.range(5, 4);
		vmask9[k].data[2].value = mask.range(6, 5);
		vmask9[k].data[3].key = mask.range(7, 6);
		vmask9[k].data[3].value = mask.range(8, 7);
		vmask9[k].data[4].key = mask.range(9, 8);
		vmask9[k].data[4].value = mask.range(10, 9);
		vmask9[k].data[5].key = mask.range(11, 10);
		vmask9[k].data[5].value = mask.range(12, 11);
		vmask9[k].data[6].key = mask.range(13, 12);
		vmask9[k].data[6].value = mask.range(14, 13);
		vmask9[k].data[7].key = mask.range(15, 14);
		vmask9[k].data[7].value = mask.range(16, 15);
		vmask9[k].data[8].key = mask.range(17, 16);
		vmask9[k].data[8].value = mask.range(18, 17);
		vmask9[k].data[9].key = mask.range(19, 18);
		vmask9[k].data[9].value = mask.range(20, 19);
		vmask9[k].data[10].key = mask.range(21, 20);
		vmask9[k].data[10].value = mask.range(22, 21);
		vmask9[k].data[11].key = mask.range(23, 22);
		vmask9[k].data[11].value = mask.range(24, 23);
		vmask9[k].data[12].key = mask.range(25, 24);
		vmask9[k].data[12].value = mask.range(26, 25);
		vmask9[k].data[13].key = mask.range(27, 26);
		vmask9[k].data[13].value = mask.range(28, 27);
		vmask9[k].data[14].key = mask.range(29, 28);
		vmask9[k].data[14].value = mask.range(30, 29);
		vmask9[k].data[15].key = mask.range(31, 30);
		vmask9[k].data[15].value = mask.range(32, 31);
		vmask10[k].data[0].key = mask.range(1, 0);
		vmask10[k].data[0].value = mask.range(2, 1);
		vmask10[k].data[1].key = mask.range(3, 2);
		vmask10[k].data[1].value = mask.range(4, 3);
		vmask10[k].data[2].key = mask.range(5, 4);
		vmask10[k].data[2].value = mask.range(6, 5);
		vmask10[k].data[3].key = mask.range(7, 6);
		vmask10[k].data[3].value = mask.range(8, 7);
		vmask10[k].data[4].key = mask.range(9, 8);
		vmask10[k].data[4].value = mask.range(10, 9);
		vmask10[k].data[5].key = mask.range(11, 10);
		vmask10[k].data[5].value = mask.range(12, 11);
		vmask10[k].data[6].key = mask.range(13, 12);
		vmask10[k].data[6].value = mask.range(14, 13);
		vmask10[k].data[7].key = mask.range(15, 14);
		vmask10[k].data[7].value = mask.range(16, 15);
		vmask10[k].data[8].key = mask.range(17, 16);
		vmask10[k].data[8].value = mask.range(18, 17);
		vmask10[k].data[9].key = mask.range(19, 18);
		vmask10[k].data[9].value = mask.range(20, 19);
		vmask10[k].data[10].key = mask.range(21, 20);
		vmask10[k].data[10].value = mask.range(22, 21);
		vmask10[k].data[11].key = mask.range(23, 22);
		vmask10[k].data[11].value = mask.range(24, 23);
		vmask10[k].data[12].key = mask.range(25, 24);
		vmask10[k].data[12].value = mask.range(26, 25);
		vmask10[k].data[13].key = mask.range(27, 26);
		vmask10[k].data[13].value = mask.range(28, 27);
		vmask10[k].data[14].key = mask.range(29, 28);
		vmask10[k].data[14].value = mask.range(30, 29);
		vmask10[k].data[15].key = mask.range(31, 30);
		vmask10[k].data[15].value = mask.range(32, 31);
		vmask11[k].data[0].key = mask.range(1, 0);
		vmask11[k].data[0].value = mask.range(2, 1);
		vmask11[k].data[1].key = mask.range(3, 2);
		vmask11[k].data[1].value = mask.range(4, 3);
		vmask11[k].data[2].key = mask.range(5, 4);
		vmask11[k].data[2].value = mask.range(6, 5);
		vmask11[k].data[3].key = mask.range(7, 6);
		vmask11[k].data[3].value = mask.range(8, 7);
		vmask11[k].data[4].key = mask.range(9, 8);
		vmask11[k].data[4].value = mask.range(10, 9);
		vmask11[k].data[5].key = mask.range(11, 10);
		vmask11[k].data[5].value = mask.range(12, 11);
		vmask11[k].data[6].key = mask.range(13, 12);
		vmask11[k].data[6].value = mask.range(14, 13);
		vmask11[k].data[7].key = mask.range(15, 14);
		vmask11[k].data[7].value = mask.range(16, 15);
		vmask11[k].data[8].key = mask.range(17, 16);
		vmask11[k].data[8].value = mask.range(18, 17);
		vmask11[k].data[9].key = mask.range(19, 18);
		vmask11[k].data[9].value = mask.range(20, 19);
		vmask11[k].data[10].key = mask.range(21, 20);
		vmask11[k].data[10].value = mask.range(22, 21);
		vmask11[k].data[11].key = mask.range(23, 22);
		vmask11[k].data[11].value = mask.range(24, 23);
		vmask11[k].data[12].key = mask.range(25, 24);
		vmask11[k].data[12].value = mask.range(26, 25);
		vmask11[k].data[13].key = mask.range(27, 26);
		vmask11[k].data[13].value = mask.range(28, 27);
		vmask11[k].data[14].key = mask.range(29, 28);
		vmask11[k].data[14].value = mask.range(30, 29);
		vmask11[k].data[15].key = mask.range(31, 30);
		vmask11[k].data[15].value = mask.range(32, 31);
		vmask12[k].data[0].key = mask.range(1, 0);
		vmask12[k].data[0].value = mask.range(2, 1);
		vmask12[k].data[1].key = mask.range(3, 2);
		vmask12[k].data[1].value = mask.range(4, 3);
		vmask12[k].data[2].key = mask.range(5, 4);
		vmask12[k].data[2].value = mask.range(6, 5);
		vmask12[k].data[3].key = mask.range(7, 6);
		vmask12[k].data[3].value = mask.range(8, 7);
		vmask12[k].data[4].key = mask.range(9, 8);
		vmask12[k].data[4].value = mask.range(10, 9);
		vmask12[k].data[5].key = mask.range(11, 10);
		vmask12[k].data[5].value = mask.range(12, 11);
		vmask12[k].data[6].key = mask.range(13, 12);
		vmask12[k].data[6].value = mask.range(14, 13);
		vmask12[k].data[7].key = mask.range(15, 14);
		vmask12[k].data[7].value = mask.range(16, 15);
		vmask12[k].data[8].key = mask.range(17, 16);
		vmask12[k].data[8].value = mask.range(18, 17);
		vmask12[k].data[9].key = mask.range(19, 18);
		vmask12[k].data[9].value = mask.range(20, 19);
		vmask12[k].data[10].key = mask.range(21, 20);
		vmask12[k].data[10].value = mask.range(22, 21);
		vmask12[k].data[11].key = mask.range(23, 22);
		vmask12[k].data[11].value = mask.range(24, 23);
		vmask12[k].data[12].key = mask.range(25, 24);
		vmask12[k].data[12].value = mask.range(26, 25);
		vmask12[k].data[13].key = mask.range(27, 26);
		vmask12[k].data[13].value = mask.range(28, 27);
		vmask12[k].data[14].key = mask.range(29, 28);
		vmask12[k].data[14].value = mask.range(30, 29);
		vmask12[k].data[15].key = mask.range(31, 30);
		vmask12[k].data[15].value = mask.range(32, 31);
		vmask13[k].data[0].key = mask.range(1, 0);
		vmask13[k].data[0].value = mask.range(2, 1);
		vmask13[k].data[1].key = mask.range(3, 2);
		vmask13[k].data[1].value = mask.range(4, 3);
		vmask13[k].data[2].key = mask.range(5, 4);
		vmask13[k].data[2].value = mask.range(6, 5);
		vmask13[k].data[3].key = mask.range(7, 6);
		vmask13[k].data[3].value = mask.range(8, 7);
		vmask13[k].data[4].key = mask.range(9, 8);
		vmask13[k].data[4].value = mask.range(10, 9);
		vmask13[k].data[5].key = mask.range(11, 10);
		vmask13[k].data[5].value = mask.range(12, 11);
		vmask13[k].data[6].key = mask.range(13, 12);
		vmask13[k].data[6].value = mask.range(14, 13);
		vmask13[k].data[7].key = mask.range(15, 14);
		vmask13[k].data[7].value = mask.range(16, 15);
		vmask13[k].data[8].key = mask.range(17, 16);
		vmask13[k].data[8].value = mask.range(18, 17);
		vmask13[k].data[9].key = mask.range(19, 18);
		vmask13[k].data[9].value = mask.range(20, 19);
		vmask13[k].data[10].key = mask.range(21, 20);
		vmask13[k].data[10].value = mask.range(22, 21);
		vmask13[k].data[11].key = mask.range(23, 22);
		vmask13[k].data[11].value = mask.range(24, 23);
		vmask13[k].data[12].key = mask.range(25, 24);
		vmask13[k].data[12].value = mask.range(26, 25);
		vmask13[k].data[13].key = mask.range(27, 26);
		vmask13[k].data[13].value = mask.range(28, 27);
		vmask13[k].data[14].key = mask.range(29, 28);
		vmask13[k].data[14].value = mask.range(30, 29);
		vmask13[k].data[15].key = mask.range(31, 30);
		vmask13[k].data[15].value = mask.range(32, 31);
		vmask14[k].data[0].key = mask.range(1, 0);
		vmask14[k].data[0].value = mask.range(2, 1);
		vmask14[k].data[1].key = mask.range(3, 2);
		vmask14[k].data[1].value = mask.range(4, 3);
		vmask14[k].data[2].key = mask.range(5, 4);
		vmask14[k].data[2].value = mask.range(6, 5);
		vmask14[k].data[3].key = mask.range(7, 6);
		vmask14[k].data[3].value = mask.range(8, 7);
		vmask14[k].data[4].key = mask.range(9, 8);
		vmask14[k].data[4].value = mask.range(10, 9);
		vmask14[k].data[5].key = mask.range(11, 10);
		vmask14[k].data[5].value = mask.range(12, 11);
		vmask14[k].data[6].key = mask.range(13, 12);
		vmask14[k].data[6].value = mask.range(14, 13);
		vmask14[k].data[7].key = mask.range(15, 14);
		vmask14[k].data[7].value = mask.range(16, 15);
		vmask14[k].data[8].key = mask.range(17, 16);
		vmask14[k].data[8].value = mask.range(18, 17);
		vmask14[k].data[9].key = mask.range(19, 18);
		vmask14[k].data[9].value = mask.range(20, 19);
		vmask14[k].data[10].key = mask.range(21, 20);
		vmask14[k].data[10].value = mask.range(22, 21);
		vmask14[k].data[11].key = mask.range(23, 22);
		vmask14[k].data[11].value = mask.range(24, 23);
		vmask14[k].data[12].key = mask.range(25, 24);
		vmask14[k].data[12].value = mask.range(26, 25);
		vmask14[k].data[13].key = mask.range(27, 26);
		vmask14[k].data[13].value = mask.range(28, 27);
		vmask14[k].data[14].key = mask.range(29, 28);
		vmask14[k].data[14].value = mask.range(30, 29);
		vmask14[k].data[15].key = mask.range(31, 30);
		vmask14[k].data[15].value = mask.range(32, 31);
		vmask15[k].data[0].key = mask.range(1, 0);
		vmask15[k].data[0].value = mask.range(2, 1);
		vmask15[k].data[1].key = mask.range(3, 2);
		vmask15[k].data[1].value = mask.range(4, 3);
		vmask15[k].data[2].key = mask.range(5, 4);
		vmask15[k].data[2].value = mask.range(6, 5);
		vmask15[k].data[3].key = mask.range(7, 6);
		vmask15[k].data[3].value = mask.range(8, 7);
		vmask15[k].data[4].key = mask.range(9, 8);
		vmask15[k].data[4].value = mask.range(10, 9);
		vmask15[k].data[5].key = mask.range(11, 10);
		vmask15[k].data[5].value = mask.range(12, 11);
		vmask15[k].data[6].key = mask.range(13, 12);
		vmask15[k].data[6].value = mask.range(14, 13);
		vmask15[k].data[7].key = mask.range(15, 14);
		vmask15[k].data[7].value = mask.range(16, 15);
		vmask15[k].data[8].key = mask.range(17, 16);
		vmask15[k].data[8].value = mask.range(18, 17);
		vmask15[k].data[9].key = mask.range(19, 18);
		vmask15[k].data[9].value = mask.range(20, 19);
		vmask15[k].data[10].key = mask.range(21, 20);
		vmask15[k].data[10].value = mask.range(22, 21);
		vmask15[k].data[11].key = mask.range(23, 22);
		vmask15[k].data[11].value = mask.range(24, 23);
		vmask15[k].data[12].key = mask.range(25, 24);
		vmask15[k].data[12].value = mask.range(26, 25);
		vmask15[k].data[13].key = mask.range(27, 26);
		vmask15[k].data[13].value = mask.range(28, 27);
		vmask15[k].data[14].key = mask.range(29, 28);
		vmask15[k].data[14].value = mask.range(30, 29);
		vmask15[k].data[15].key = mask.range(31, 30);
		vmask15[k].data[15].value = mask.range(32, 31);
		#else 
		vmask0[k].data[0].key = READFROM_UINT(mask, 0, 1);
		vmask0[k].data[0].value = READFROM_UINT(mask, 1, 1);
		vmask0[k].data[1].key = READFROM_UINT(mask, 2, 1);
		vmask0[k].data[1].value = READFROM_UINT(mask, 3, 1);
		vmask0[k].data[2].key = READFROM_UINT(mask, 4, 1);
		vmask0[k].data[2].value = READFROM_UINT(mask, 5, 1);
		vmask0[k].data[3].key = READFROM_UINT(mask, 6, 1);
		vmask0[k].data[3].value = READFROM_UINT(mask, 7, 1);
		vmask0[k].data[4].key = READFROM_UINT(mask, 8, 1);
		vmask0[k].data[4].value = READFROM_UINT(mask, 9, 1);
		vmask0[k].data[5].key = READFROM_UINT(mask, 10, 1);
		vmask0[k].data[5].value = READFROM_UINT(mask, 11, 1);
		vmask0[k].data[6].key = READFROM_UINT(mask, 12, 1);
		vmask0[k].data[6].value = READFROM_UINT(mask, 13, 1);
		vmask0[k].data[7].key = READFROM_UINT(mask, 14, 1);
		vmask0[k].data[7].value = READFROM_UINT(mask, 15, 1);
		vmask0[k].data[8].key = READFROM_UINT(mask, 16, 1);
		vmask0[k].data[8].value = READFROM_UINT(mask, 17, 1);
		vmask0[k].data[9].key = READFROM_UINT(mask, 18, 1);
		vmask0[k].data[9].value = READFROM_UINT(mask, 19, 1);
		vmask0[k].data[10].key = READFROM_UINT(mask, 20, 1);
		vmask0[k].data[10].value = READFROM_UINT(mask, 21, 1);
		vmask0[k].data[11].key = READFROM_UINT(mask, 22, 1);
		vmask0[k].data[11].value = READFROM_UINT(mask, 23, 1);
		vmask0[k].data[12].key = READFROM_UINT(mask, 24, 1);
		vmask0[k].data[12].value = READFROM_UINT(mask, 25, 1);
		vmask0[k].data[13].key = READFROM_UINT(mask, 26, 1);
		vmask0[k].data[13].value = READFROM_UINT(mask, 27, 1);
		vmask0[k].data[14].key = READFROM_UINT(mask, 28, 1);
		vmask0[k].data[14].value = READFROM_UINT(mask, 29, 1);
		vmask0[k].data[15].key = READFROM_UINT(mask, 30, 1);
		vmask0[k].data[15].value = READFROM_UINT(mask, 31, 1);
		vmask1[k].data[0].key = READFROM_UINT(mask, 0, 1);
		vmask1[k].data[0].value = READFROM_UINT(mask, 1, 1);
		vmask1[k].data[1].key = READFROM_UINT(mask, 2, 1);
		vmask1[k].data[1].value = READFROM_UINT(mask, 3, 1);
		vmask1[k].data[2].key = READFROM_UINT(mask, 4, 1);
		vmask1[k].data[2].value = READFROM_UINT(mask, 5, 1);
		vmask1[k].data[3].key = READFROM_UINT(mask, 6, 1);
		vmask1[k].data[3].value = READFROM_UINT(mask, 7, 1);
		vmask1[k].data[4].key = READFROM_UINT(mask, 8, 1);
		vmask1[k].data[4].value = READFROM_UINT(mask, 9, 1);
		vmask1[k].data[5].key = READFROM_UINT(mask, 10, 1);
		vmask1[k].data[5].value = READFROM_UINT(mask, 11, 1);
		vmask1[k].data[6].key = READFROM_UINT(mask, 12, 1);
		vmask1[k].data[6].value = READFROM_UINT(mask, 13, 1);
		vmask1[k].data[7].key = READFROM_UINT(mask, 14, 1);
		vmask1[k].data[7].value = READFROM_UINT(mask, 15, 1);
		vmask1[k].data[8].key = READFROM_UINT(mask, 16, 1);
		vmask1[k].data[8].value = READFROM_UINT(mask, 17, 1);
		vmask1[k].data[9].key = READFROM_UINT(mask, 18, 1);
		vmask1[k].data[9].value = READFROM_UINT(mask, 19, 1);
		vmask1[k].data[10].key = READFROM_UINT(mask, 20, 1);
		vmask1[k].data[10].value = READFROM_UINT(mask, 21, 1);
		vmask1[k].data[11].key = READFROM_UINT(mask, 22, 1);
		vmask1[k].data[11].value = READFROM_UINT(mask, 23, 1);
		vmask1[k].data[12].key = READFROM_UINT(mask, 24, 1);
		vmask1[k].data[12].value = READFROM_UINT(mask, 25, 1);
		vmask1[k].data[13].key = READFROM_UINT(mask, 26, 1);
		vmask1[k].data[13].value = READFROM_UINT(mask, 27, 1);
		vmask1[k].data[14].key = READFROM_UINT(mask, 28, 1);
		vmask1[k].data[14].value = READFROM_UINT(mask, 29, 1);
		vmask1[k].data[15].key = READFROM_UINT(mask, 30, 1);
		vmask1[k].data[15].value = READFROM_UINT(mask, 31, 1);
		vmask2[k].data[0].key = READFROM_UINT(mask, 0, 1);
		vmask2[k].data[0].value = READFROM_UINT(mask, 1, 1);
		vmask2[k].data[1].key = READFROM_UINT(mask, 2, 1);
		vmask2[k].data[1].value = READFROM_UINT(mask, 3, 1);
		vmask2[k].data[2].key = READFROM_UINT(mask, 4, 1);
		vmask2[k].data[2].value = READFROM_UINT(mask, 5, 1);
		vmask2[k].data[3].key = READFROM_UINT(mask, 6, 1);
		vmask2[k].data[3].value = READFROM_UINT(mask, 7, 1);
		vmask2[k].data[4].key = READFROM_UINT(mask, 8, 1);
		vmask2[k].data[4].value = READFROM_UINT(mask, 9, 1);
		vmask2[k].data[5].key = READFROM_UINT(mask, 10, 1);
		vmask2[k].data[5].value = READFROM_UINT(mask, 11, 1);
		vmask2[k].data[6].key = READFROM_UINT(mask, 12, 1);
		vmask2[k].data[6].value = READFROM_UINT(mask, 13, 1);
		vmask2[k].data[7].key = READFROM_UINT(mask, 14, 1);
		vmask2[k].data[7].value = READFROM_UINT(mask, 15, 1);
		vmask2[k].data[8].key = READFROM_UINT(mask, 16, 1);
		vmask2[k].data[8].value = READFROM_UINT(mask, 17, 1);
		vmask2[k].data[9].key = READFROM_UINT(mask, 18, 1);
		vmask2[k].data[9].value = READFROM_UINT(mask, 19, 1);
		vmask2[k].data[10].key = READFROM_UINT(mask, 20, 1);
		vmask2[k].data[10].value = READFROM_UINT(mask, 21, 1);
		vmask2[k].data[11].key = READFROM_UINT(mask, 22, 1);
		vmask2[k].data[11].value = READFROM_UINT(mask, 23, 1);
		vmask2[k].data[12].key = READFROM_UINT(mask, 24, 1);
		vmask2[k].data[12].value = READFROM_UINT(mask, 25, 1);
		vmask2[k].data[13].key = READFROM_UINT(mask, 26, 1);
		vmask2[k].data[13].value = READFROM_UINT(mask, 27, 1);
		vmask2[k].data[14].key = READFROM_UINT(mask, 28, 1);
		vmask2[k].data[14].value = READFROM_UINT(mask, 29, 1);
		vmask2[k].data[15].key = READFROM_UINT(mask, 30, 1);
		vmask2[k].data[15].value = READFROM_UINT(mask, 31, 1);
		vmask3[k].data[0].key = READFROM_UINT(mask, 0, 1);
		vmask3[k].data[0].value = READFROM_UINT(mask, 1, 1);
		vmask3[k].data[1].key = READFROM_UINT(mask, 2, 1);
		vmask3[k].data[1].value = READFROM_UINT(mask, 3, 1);
		vmask3[k].data[2].key = READFROM_UINT(mask, 4, 1);
		vmask3[k].data[2].value = READFROM_UINT(mask, 5, 1);
		vmask3[k].data[3].key = READFROM_UINT(mask, 6, 1);
		vmask3[k].data[3].value = READFROM_UINT(mask, 7, 1);
		vmask3[k].data[4].key = READFROM_UINT(mask, 8, 1);
		vmask3[k].data[4].value = READFROM_UINT(mask, 9, 1);
		vmask3[k].data[5].key = READFROM_UINT(mask, 10, 1);
		vmask3[k].data[5].value = READFROM_UINT(mask, 11, 1);
		vmask3[k].data[6].key = READFROM_UINT(mask, 12, 1);
		vmask3[k].data[6].value = READFROM_UINT(mask, 13, 1);
		vmask3[k].data[7].key = READFROM_UINT(mask, 14, 1);
		vmask3[k].data[7].value = READFROM_UINT(mask, 15, 1);
		vmask3[k].data[8].key = READFROM_UINT(mask, 16, 1);
		vmask3[k].data[8].value = READFROM_UINT(mask, 17, 1);
		vmask3[k].data[9].key = READFROM_UINT(mask, 18, 1);
		vmask3[k].data[9].value = READFROM_UINT(mask, 19, 1);
		vmask3[k].data[10].key = READFROM_UINT(mask, 20, 1);
		vmask3[k].data[10].value = READFROM_UINT(mask, 21, 1);
		vmask3[k].data[11].key = READFROM_UINT(mask, 22, 1);
		vmask3[k].data[11].value = READFROM_UINT(mask, 23, 1);
		vmask3[k].data[12].key = READFROM_UINT(mask, 24, 1);
		vmask3[k].data[12].value = READFROM_UINT(mask, 25, 1);
		vmask3[k].data[13].key = READFROM_UINT(mask, 26, 1);
		vmask3[k].data[13].value = READFROM_UINT(mask, 27, 1);
		vmask3[k].data[14].key = READFROM_UINT(mask, 28, 1);
		vmask3[k].data[14].value = READFROM_UINT(mask, 29, 1);
		vmask3[k].data[15].key = READFROM_UINT(mask, 30, 1);
		vmask3[k].data[15].value = READFROM_UINT(mask, 31, 1);
		vmask4[k].data[0].key = READFROM_UINT(mask, 0, 1);
		vmask4[k].data[0].value = READFROM_UINT(mask, 1, 1);
		vmask4[k].data[1].key = READFROM_UINT(mask, 2, 1);
		vmask4[k].data[1].value = READFROM_UINT(mask, 3, 1);
		vmask4[k].data[2].key = READFROM_UINT(mask, 4, 1);
		vmask4[k].data[2].value = READFROM_UINT(mask, 5, 1);
		vmask4[k].data[3].key = READFROM_UINT(mask, 6, 1);
		vmask4[k].data[3].value = READFROM_UINT(mask, 7, 1);
		vmask4[k].data[4].key = READFROM_UINT(mask, 8, 1);
		vmask4[k].data[4].value = READFROM_UINT(mask, 9, 1);
		vmask4[k].data[5].key = READFROM_UINT(mask, 10, 1);
		vmask4[k].data[5].value = READFROM_UINT(mask, 11, 1);
		vmask4[k].data[6].key = READFROM_UINT(mask, 12, 1);
		vmask4[k].data[6].value = READFROM_UINT(mask, 13, 1);
		vmask4[k].data[7].key = READFROM_UINT(mask, 14, 1);
		vmask4[k].data[7].value = READFROM_UINT(mask, 15, 1);
		vmask4[k].data[8].key = READFROM_UINT(mask, 16, 1);
		vmask4[k].data[8].value = READFROM_UINT(mask, 17, 1);
		vmask4[k].data[9].key = READFROM_UINT(mask, 18, 1);
		vmask4[k].data[9].value = READFROM_UINT(mask, 19, 1);
		vmask4[k].data[10].key = READFROM_UINT(mask, 20, 1);
		vmask4[k].data[10].value = READFROM_UINT(mask, 21, 1);
		vmask4[k].data[11].key = READFROM_UINT(mask, 22, 1);
		vmask4[k].data[11].value = READFROM_UINT(mask, 23, 1);
		vmask4[k].data[12].key = READFROM_UINT(mask, 24, 1);
		vmask4[k].data[12].value = READFROM_UINT(mask, 25, 1);
		vmask4[k].data[13].key = READFROM_UINT(mask, 26, 1);
		vmask4[k].data[13].value = READFROM_UINT(mask, 27, 1);
		vmask4[k].data[14].key = READFROM_UINT(mask, 28, 1);
		vmask4[k].data[14].value = READFROM_UINT(mask, 29, 1);
		vmask4[k].data[15].key = READFROM_UINT(mask, 30, 1);
		vmask4[k].data[15].value = READFROM_UINT(mask, 31, 1);
		vmask5[k].data[0].key = READFROM_UINT(mask, 0, 1);
		vmask5[k].data[0].value = READFROM_UINT(mask, 1, 1);
		vmask5[k].data[1].key = READFROM_UINT(mask, 2, 1);
		vmask5[k].data[1].value = READFROM_UINT(mask, 3, 1);
		vmask5[k].data[2].key = READFROM_UINT(mask, 4, 1);
		vmask5[k].data[2].value = READFROM_UINT(mask, 5, 1);
		vmask5[k].data[3].key = READFROM_UINT(mask, 6, 1);
		vmask5[k].data[3].value = READFROM_UINT(mask, 7, 1);
		vmask5[k].data[4].key = READFROM_UINT(mask, 8, 1);
		vmask5[k].data[4].value = READFROM_UINT(mask, 9, 1);
		vmask5[k].data[5].key = READFROM_UINT(mask, 10, 1);
		vmask5[k].data[5].value = READFROM_UINT(mask, 11, 1);
		vmask5[k].data[6].key = READFROM_UINT(mask, 12, 1);
		vmask5[k].data[6].value = READFROM_UINT(mask, 13, 1);
		vmask5[k].data[7].key = READFROM_UINT(mask, 14, 1);
		vmask5[k].data[7].value = READFROM_UINT(mask, 15, 1);
		vmask5[k].data[8].key = READFROM_UINT(mask, 16, 1);
		vmask5[k].data[8].value = READFROM_UINT(mask, 17, 1);
		vmask5[k].data[9].key = READFROM_UINT(mask, 18, 1);
		vmask5[k].data[9].value = READFROM_UINT(mask, 19, 1);
		vmask5[k].data[10].key = READFROM_UINT(mask, 20, 1);
		vmask5[k].data[10].value = READFROM_UINT(mask, 21, 1);
		vmask5[k].data[11].key = READFROM_UINT(mask, 22, 1);
		vmask5[k].data[11].value = READFROM_UINT(mask, 23, 1);
		vmask5[k].data[12].key = READFROM_UINT(mask, 24, 1);
		vmask5[k].data[12].value = READFROM_UINT(mask, 25, 1);
		vmask5[k].data[13].key = READFROM_UINT(mask, 26, 1);
		vmask5[k].data[13].value = READFROM_UINT(mask, 27, 1);
		vmask5[k].data[14].key = READFROM_UINT(mask, 28, 1);
		vmask5[k].data[14].value = READFROM_UINT(mask, 29, 1);
		vmask5[k].data[15].key = READFROM_UINT(mask, 30, 1);
		vmask5[k].data[15].value = READFROM_UINT(mask, 31, 1);
		vmask6[k].data[0].key = READFROM_UINT(mask, 0, 1);
		vmask6[k].data[0].value = READFROM_UINT(mask, 1, 1);
		vmask6[k].data[1].key = READFROM_UINT(mask, 2, 1);
		vmask6[k].data[1].value = READFROM_UINT(mask, 3, 1);
		vmask6[k].data[2].key = READFROM_UINT(mask, 4, 1);
		vmask6[k].data[2].value = READFROM_UINT(mask, 5, 1);
		vmask6[k].data[3].key = READFROM_UINT(mask, 6, 1);
		vmask6[k].data[3].value = READFROM_UINT(mask, 7, 1);
		vmask6[k].data[4].key = READFROM_UINT(mask, 8, 1);
		vmask6[k].data[4].value = READFROM_UINT(mask, 9, 1);
		vmask6[k].data[5].key = READFROM_UINT(mask, 10, 1);
		vmask6[k].data[5].value = READFROM_UINT(mask, 11, 1);
		vmask6[k].data[6].key = READFROM_UINT(mask, 12, 1);
		vmask6[k].data[6].value = READFROM_UINT(mask, 13, 1);
		vmask6[k].data[7].key = READFROM_UINT(mask, 14, 1);
		vmask6[k].data[7].value = READFROM_UINT(mask, 15, 1);
		vmask6[k].data[8].key = READFROM_UINT(mask, 16, 1);
		vmask6[k].data[8].value = READFROM_UINT(mask, 17, 1);
		vmask6[k].data[9].key = READFROM_UINT(mask, 18, 1);
		vmask6[k].data[9].value = READFROM_UINT(mask, 19, 1);
		vmask6[k].data[10].key = READFROM_UINT(mask, 20, 1);
		vmask6[k].data[10].value = READFROM_UINT(mask, 21, 1);
		vmask6[k].data[11].key = READFROM_UINT(mask, 22, 1);
		vmask6[k].data[11].value = READFROM_UINT(mask, 23, 1);
		vmask6[k].data[12].key = READFROM_UINT(mask, 24, 1);
		vmask6[k].data[12].value = READFROM_UINT(mask, 25, 1);
		vmask6[k].data[13].key = READFROM_UINT(mask, 26, 1);
		vmask6[k].data[13].value = READFROM_UINT(mask, 27, 1);
		vmask6[k].data[14].key = READFROM_UINT(mask, 28, 1);
		vmask6[k].data[14].value = READFROM_UINT(mask, 29, 1);
		vmask6[k].data[15].key = READFROM_UINT(mask, 30, 1);
		vmask6[k].data[15].value = READFROM_UINT(mask, 31, 1);
		vmask7[k].data[0].key = READFROM_UINT(mask, 0, 1);
		vmask7[k].data[0].value = READFROM_UINT(mask, 1, 1);
		vmask7[k].data[1].key = READFROM_UINT(mask, 2, 1);
		vmask7[k].data[1].value = READFROM_UINT(mask, 3, 1);
		vmask7[k].data[2].key = READFROM_UINT(mask, 4, 1);
		vmask7[k].data[2].value = READFROM_UINT(mask, 5, 1);
		vmask7[k].data[3].key = READFROM_UINT(mask, 6, 1);
		vmask7[k].data[3].value = READFROM_UINT(mask, 7, 1);
		vmask7[k].data[4].key = READFROM_UINT(mask, 8, 1);
		vmask7[k].data[4].value = READFROM_UINT(mask, 9, 1);
		vmask7[k].data[5].key = READFROM_UINT(mask, 10, 1);
		vmask7[k].data[5].value = READFROM_UINT(mask, 11, 1);
		vmask7[k].data[6].key = READFROM_UINT(mask, 12, 1);
		vmask7[k].data[6].value = READFROM_UINT(mask, 13, 1);
		vmask7[k].data[7].key = READFROM_UINT(mask, 14, 1);
		vmask7[k].data[7].value = READFROM_UINT(mask, 15, 1);
		vmask7[k].data[8].key = READFROM_UINT(mask, 16, 1);
		vmask7[k].data[8].value = READFROM_UINT(mask, 17, 1);
		vmask7[k].data[9].key = READFROM_UINT(mask, 18, 1);
		vmask7[k].data[9].value = READFROM_UINT(mask, 19, 1);
		vmask7[k].data[10].key = READFROM_UINT(mask, 20, 1);
		vmask7[k].data[10].value = READFROM_UINT(mask, 21, 1);
		vmask7[k].data[11].key = READFROM_UINT(mask, 22, 1);
		vmask7[k].data[11].value = READFROM_UINT(mask, 23, 1);
		vmask7[k].data[12].key = READFROM_UINT(mask, 24, 1);
		vmask7[k].data[12].value = READFROM_UINT(mask, 25, 1);
		vmask7[k].data[13].key = READFROM_UINT(mask, 26, 1);
		vmask7[k].data[13].value = READFROM_UINT(mask, 27, 1);
		vmask7[k].data[14].key = READFROM_UINT(mask, 28, 1);
		vmask7[k].data[14].value = READFROM_UINT(mask, 29, 1);
		vmask7[k].data[15].key = READFROM_UINT(mask, 30, 1);
		vmask7[k].data[15].value = READFROM_UINT(mask, 31, 1);
		vmask8[k].data[0].key = READFROM_UINT(mask, 0, 1);
		vmask8[k].data[0].value = READFROM_UINT(mask, 1, 1);
		vmask8[k].data[1].key = READFROM_UINT(mask, 2, 1);
		vmask8[k].data[1].value = READFROM_UINT(mask, 3, 1);
		vmask8[k].data[2].key = READFROM_UINT(mask, 4, 1);
		vmask8[k].data[2].value = READFROM_UINT(mask, 5, 1);
		vmask8[k].data[3].key = READFROM_UINT(mask, 6, 1);
		vmask8[k].data[3].value = READFROM_UINT(mask, 7, 1);
		vmask8[k].data[4].key = READFROM_UINT(mask, 8, 1);
		vmask8[k].data[4].value = READFROM_UINT(mask, 9, 1);
		vmask8[k].data[5].key = READFROM_UINT(mask, 10, 1);
		vmask8[k].data[5].value = READFROM_UINT(mask, 11, 1);
		vmask8[k].data[6].key = READFROM_UINT(mask, 12, 1);
		vmask8[k].data[6].value = READFROM_UINT(mask, 13, 1);
		vmask8[k].data[7].key = READFROM_UINT(mask, 14, 1);
		vmask8[k].data[7].value = READFROM_UINT(mask, 15, 1);
		vmask8[k].data[8].key = READFROM_UINT(mask, 16, 1);
		vmask8[k].data[8].value = READFROM_UINT(mask, 17, 1);
		vmask8[k].data[9].key = READFROM_UINT(mask, 18, 1);
		vmask8[k].data[9].value = READFROM_UINT(mask, 19, 1);
		vmask8[k].data[10].key = READFROM_UINT(mask, 20, 1);
		vmask8[k].data[10].value = READFROM_UINT(mask, 21, 1);
		vmask8[k].data[11].key = READFROM_UINT(mask, 22, 1);
		vmask8[k].data[11].value = READFROM_UINT(mask, 23, 1);
		vmask8[k].data[12].key = READFROM_UINT(mask, 24, 1);
		vmask8[k].data[12].value = READFROM_UINT(mask, 25, 1);
		vmask8[k].data[13].key = READFROM_UINT(mask, 26, 1);
		vmask8[k].data[13].value = READFROM_UINT(mask, 27, 1);
		vmask8[k].data[14].key = READFROM_UINT(mask, 28, 1);
		vmask8[k].data[14].value = READFROM_UINT(mask, 29, 1);
		vmask8[k].data[15].key = READFROM_UINT(mask, 30, 1);
		vmask8[k].data[15].value = READFROM_UINT(mask, 31, 1);
		vmask9[k].data[0].key = READFROM_UINT(mask, 0, 1);
		vmask9[k].data[0].value = READFROM_UINT(mask, 1, 1);
		vmask9[k].data[1].key = READFROM_UINT(mask, 2, 1);
		vmask9[k].data[1].value = READFROM_UINT(mask, 3, 1);
		vmask9[k].data[2].key = READFROM_UINT(mask, 4, 1);
		vmask9[k].data[2].value = READFROM_UINT(mask, 5, 1);
		vmask9[k].data[3].key = READFROM_UINT(mask, 6, 1);
		vmask9[k].data[3].value = READFROM_UINT(mask, 7, 1);
		vmask9[k].data[4].key = READFROM_UINT(mask, 8, 1);
		vmask9[k].data[4].value = READFROM_UINT(mask, 9, 1);
		vmask9[k].data[5].key = READFROM_UINT(mask, 10, 1);
		vmask9[k].data[5].value = READFROM_UINT(mask, 11, 1);
		vmask9[k].data[6].key = READFROM_UINT(mask, 12, 1);
		vmask9[k].data[6].value = READFROM_UINT(mask, 13, 1);
		vmask9[k].data[7].key = READFROM_UINT(mask, 14, 1);
		vmask9[k].data[7].value = READFROM_UINT(mask, 15, 1);
		vmask9[k].data[8].key = READFROM_UINT(mask, 16, 1);
		vmask9[k].data[8].value = READFROM_UINT(mask, 17, 1);
		vmask9[k].data[9].key = READFROM_UINT(mask, 18, 1);
		vmask9[k].data[9].value = READFROM_UINT(mask, 19, 1);
		vmask9[k].data[10].key = READFROM_UINT(mask, 20, 1);
		vmask9[k].data[10].value = READFROM_UINT(mask, 21, 1);
		vmask9[k].data[11].key = READFROM_UINT(mask, 22, 1);
		vmask9[k].data[11].value = READFROM_UINT(mask, 23, 1);
		vmask9[k].data[12].key = READFROM_UINT(mask, 24, 1);
		vmask9[k].data[12].value = READFROM_UINT(mask, 25, 1);
		vmask9[k].data[13].key = READFROM_UINT(mask, 26, 1);
		vmask9[k].data[13].value = READFROM_UINT(mask, 27, 1);
		vmask9[k].data[14].key = READFROM_UINT(mask, 28, 1);
		vmask9[k].data[14].value = READFROM_UINT(mask, 29, 1);
		vmask9[k].data[15].key = READFROM_UINT(mask, 30, 1);
		vmask9[k].data[15].value = READFROM_UINT(mask, 31, 1);
		vmask10[k].data[0].key = READFROM_UINT(mask, 0, 1);
		vmask10[k].data[0].value = READFROM_UINT(mask, 1, 1);
		vmask10[k].data[1].key = READFROM_UINT(mask, 2, 1);
		vmask10[k].data[1].value = READFROM_UINT(mask, 3, 1);
		vmask10[k].data[2].key = READFROM_UINT(mask, 4, 1);
		vmask10[k].data[2].value = READFROM_UINT(mask, 5, 1);
		vmask10[k].data[3].key = READFROM_UINT(mask, 6, 1);
		vmask10[k].data[3].value = READFROM_UINT(mask, 7, 1);
		vmask10[k].data[4].key = READFROM_UINT(mask, 8, 1);
		vmask10[k].data[4].value = READFROM_UINT(mask, 9, 1);
		vmask10[k].data[5].key = READFROM_UINT(mask, 10, 1);
		vmask10[k].data[5].value = READFROM_UINT(mask, 11, 1);
		vmask10[k].data[6].key = READFROM_UINT(mask, 12, 1);
		vmask10[k].data[6].value = READFROM_UINT(mask, 13, 1);
		vmask10[k].data[7].key = READFROM_UINT(mask, 14, 1);
		vmask10[k].data[7].value = READFROM_UINT(mask, 15, 1);
		vmask10[k].data[8].key = READFROM_UINT(mask, 16, 1);
		vmask10[k].data[8].value = READFROM_UINT(mask, 17, 1);
		vmask10[k].data[9].key = READFROM_UINT(mask, 18, 1);
		vmask10[k].data[9].value = READFROM_UINT(mask, 19, 1);
		vmask10[k].data[10].key = READFROM_UINT(mask, 20, 1);
		vmask10[k].data[10].value = READFROM_UINT(mask, 21, 1);
		vmask10[k].data[11].key = READFROM_UINT(mask, 22, 1);
		vmask10[k].data[11].value = READFROM_UINT(mask, 23, 1);
		vmask10[k].data[12].key = READFROM_UINT(mask, 24, 1);
		vmask10[k].data[12].value = READFROM_UINT(mask, 25, 1);
		vmask10[k].data[13].key = READFROM_UINT(mask, 26, 1);
		vmask10[k].data[13].value = READFROM_UINT(mask, 27, 1);
		vmask10[k].data[14].key = READFROM_UINT(mask, 28, 1);
		vmask10[k].data[14].value = READFROM_UINT(mask, 29, 1);
		vmask10[k].data[15].key = READFROM_UINT(mask, 30, 1);
		vmask10[k].data[15].value = READFROM_UINT(mask, 31, 1);
		vmask11[k].data[0].key = READFROM_UINT(mask, 0, 1);
		vmask11[k].data[0].value = READFROM_UINT(mask, 1, 1);
		vmask11[k].data[1].key = READFROM_UINT(mask, 2, 1);
		vmask11[k].data[1].value = READFROM_UINT(mask, 3, 1);
		vmask11[k].data[2].key = READFROM_UINT(mask, 4, 1);
		vmask11[k].data[2].value = READFROM_UINT(mask, 5, 1);
		vmask11[k].data[3].key = READFROM_UINT(mask, 6, 1);
		vmask11[k].data[3].value = READFROM_UINT(mask, 7, 1);
		vmask11[k].data[4].key = READFROM_UINT(mask, 8, 1);
		vmask11[k].data[4].value = READFROM_UINT(mask, 9, 1);
		vmask11[k].data[5].key = READFROM_UINT(mask, 10, 1);
		vmask11[k].data[5].value = READFROM_UINT(mask, 11, 1);
		vmask11[k].data[6].key = READFROM_UINT(mask, 12, 1);
		vmask11[k].data[6].value = READFROM_UINT(mask, 13, 1);
		vmask11[k].data[7].key = READFROM_UINT(mask, 14, 1);
		vmask11[k].data[7].value = READFROM_UINT(mask, 15, 1);
		vmask11[k].data[8].key = READFROM_UINT(mask, 16, 1);
		vmask11[k].data[8].value = READFROM_UINT(mask, 17, 1);
		vmask11[k].data[9].key = READFROM_UINT(mask, 18, 1);
		vmask11[k].data[9].value = READFROM_UINT(mask, 19, 1);
		vmask11[k].data[10].key = READFROM_UINT(mask, 20, 1);
		vmask11[k].data[10].value = READFROM_UINT(mask, 21, 1);
		vmask11[k].data[11].key = READFROM_UINT(mask, 22, 1);
		vmask11[k].data[11].value = READFROM_UINT(mask, 23, 1);
		vmask11[k].data[12].key = READFROM_UINT(mask, 24, 1);
		vmask11[k].data[12].value = READFROM_UINT(mask, 25, 1);
		vmask11[k].data[13].key = READFROM_UINT(mask, 26, 1);
		vmask11[k].data[13].value = READFROM_UINT(mask, 27, 1);
		vmask11[k].data[14].key = READFROM_UINT(mask, 28, 1);
		vmask11[k].data[14].value = READFROM_UINT(mask, 29, 1);
		vmask11[k].data[15].key = READFROM_UINT(mask, 30, 1);
		vmask11[k].data[15].value = READFROM_UINT(mask, 31, 1);
		vmask12[k].data[0].key = READFROM_UINT(mask, 0, 1);
		vmask12[k].data[0].value = READFROM_UINT(mask, 1, 1);
		vmask12[k].data[1].key = READFROM_UINT(mask, 2, 1);
		vmask12[k].data[1].value = READFROM_UINT(mask, 3, 1);
		vmask12[k].data[2].key = READFROM_UINT(mask, 4, 1);
		vmask12[k].data[2].value = READFROM_UINT(mask, 5, 1);
		vmask12[k].data[3].key = READFROM_UINT(mask, 6, 1);
		vmask12[k].data[3].value = READFROM_UINT(mask, 7, 1);
		vmask12[k].data[4].key = READFROM_UINT(mask, 8, 1);
		vmask12[k].data[4].value = READFROM_UINT(mask, 9, 1);
		vmask12[k].data[5].key = READFROM_UINT(mask, 10, 1);
		vmask12[k].data[5].value = READFROM_UINT(mask, 11, 1);
		vmask12[k].data[6].key = READFROM_UINT(mask, 12, 1);
		vmask12[k].data[6].value = READFROM_UINT(mask, 13, 1);
		vmask12[k].data[7].key = READFROM_UINT(mask, 14, 1);
		vmask12[k].data[7].value = READFROM_UINT(mask, 15, 1);
		vmask12[k].data[8].key = READFROM_UINT(mask, 16, 1);
		vmask12[k].data[8].value = READFROM_UINT(mask, 17, 1);
		vmask12[k].data[9].key = READFROM_UINT(mask, 18, 1);
		vmask12[k].data[9].value = READFROM_UINT(mask, 19, 1);
		vmask12[k].data[10].key = READFROM_UINT(mask, 20, 1);
		vmask12[k].data[10].value = READFROM_UINT(mask, 21, 1);
		vmask12[k].data[11].key = READFROM_UINT(mask, 22, 1);
		vmask12[k].data[11].value = READFROM_UINT(mask, 23, 1);
		vmask12[k].data[12].key = READFROM_UINT(mask, 24, 1);
		vmask12[k].data[12].value = READFROM_UINT(mask, 25, 1);
		vmask12[k].data[13].key = READFROM_UINT(mask, 26, 1);
		vmask12[k].data[13].value = READFROM_UINT(mask, 27, 1);
		vmask12[k].data[14].key = READFROM_UINT(mask, 28, 1);
		vmask12[k].data[14].value = READFROM_UINT(mask, 29, 1);
		vmask12[k].data[15].key = READFROM_UINT(mask, 30, 1);
		vmask12[k].data[15].value = READFROM_UINT(mask, 31, 1);
		vmask13[k].data[0].key = READFROM_UINT(mask, 0, 1);
		vmask13[k].data[0].value = READFROM_UINT(mask, 1, 1);
		vmask13[k].data[1].key = READFROM_UINT(mask, 2, 1);
		vmask13[k].data[1].value = READFROM_UINT(mask, 3, 1);
		vmask13[k].data[2].key = READFROM_UINT(mask, 4, 1);
		vmask13[k].data[2].value = READFROM_UINT(mask, 5, 1);
		vmask13[k].data[3].key = READFROM_UINT(mask, 6, 1);
		vmask13[k].data[3].value = READFROM_UINT(mask, 7, 1);
		vmask13[k].data[4].key = READFROM_UINT(mask, 8, 1);
		vmask13[k].data[4].value = READFROM_UINT(mask, 9, 1);
		vmask13[k].data[5].key = READFROM_UINT(mask, 10, 1);
		vmask13[k].data[5].value = READFROM_UINT(mask, 11, 1);
		vmask13[k].data[6].key = READFROM_UINT(mask, 12, 1);
		vmask13[k].data[6].value = READFROM_UINT(mask, 13, 1);
		vmask13[k].data[7].key = READFROM_UINT(mask, 14, 1);
		vmask13[k].data[7].value = READFROM_UINT(mask, 15, 1);
		vmask13[k].data[8].key = READFROM_UINT(mask, 16, 1);
		vmask13[k].data[8].value = READFROM_UINT(mask, 17, 1);
		vmask13[k].data[9].key = READFROM_UINT(mask, 18, 1);
		vmask13[k].data[9].value = READFROM_UINT(mask, 19, 1);
		vmask13[k].data[10].key = READFROM_UINT(mask, 20, 1);
		vmask13[k].data[10].value = READFROM_UINT(mask, 21, 1);
		vmask13[k].data[11].key = READFROM_UINT(mask, 22, 1);
		vmask13[k].data[11].value = READFROM_UINT(mask, 23, 1);
		vmask13[k].data[12].key = READFROM_UINT(mask, 24, 1);
		vmask13[k].data[12].value = READFROM_UINT(mask, 25, 1);
		vmask13[k].data[13].key = READFROM_UINT(mask, 26, 1);
		vmask13[k].data[13].value = READFROM_UINT(mask, 27, 1);
		vmask13[k].data[14].key = READFROM_UINT(mask, 28, 1);
		vmask13[k].data[14].value = READFROM_UINT(mask, 29, 1);
		vmask13[k].data[15].key = READFROM_UINT(mask, 30, 1);
		vmask13[k].data[15].value = READFROM_UINT(mask, 31, 1);
		vmask14[k].data[0].key = READFROM_UINT(mask, 0, 1);
		vmask14[k].data[0].value = READFROM_UINT(mask, 1, 1);
		vmask14[k].data[1].key = READFROM_UINT(mask, 2, 1);
		vmask14[k].data[1].value = READFROM_UINT(mask, 3, 1);
		vmask14[k].data[2].key = READFROM_UINT(mask, 4, 1);
		vmask14[k].data[2].value = READFROM_UINT(mask, 5, 1);
		vmask14[k].data[3].key = READFROM_UINT(mask, 6, 1);
		vmask14[k].data[3].value = READFROM_UINT(mask, 7, 1);
		vmask14[k].data[4].key = READFROM_UINT(mask, 8, 1);
		vmask14[k].data[4].value = READFROM_UINT(mask, 9, 1);
		vmask14[k].data[5].key = READFROM_UINT(mask, 10, 1);
		vmask14[k].data[5].value = READFROM_UINT(mask, 11, 1);
		vmask14[k].data[6].key = READFROM_UINT(mask, 12, 1);
		vmask14[k].data[6].value = READFROM_UINT(mask, 13, 1);
		vmask14[k].data[7].key = READFROM_UINT(mask, 14, 1);
		vmask14[k].data[7].value = READFROM_UINT(mask, 15, 1);
		vmask14[k].data[8].key = READFROM_UINT(mask, 16, 1);
		vmask14[k].data[8].value = READFROM_UINT(mask, 17, 1);
		vmask14[k].data[9].key = READFROM_UINT(mask, 18, 1);
		vmask14[k].data[9].value = READFROM_UINT(mask, 19, 1);
		vmask14[k].data[10].key = READFROM_UINT(mask, 20, 1);
		vmask14[k].data[10].value = READFROM_UINT(mask, 21, 1);
		vmask14[k].data[11].key = READFROM_UINT(mask, 22, 1);
		vmask14[k].data[11].value = READFROM_UINT(mask, 23, 1);
		vmask14[k].data[12].key = READFROM_UINT(mask, 24, 1);
		vmask14[k].data[12].value = READFROM_UINT(mask, 25, 1);
		vmask14[k].data[13].key = READFROM_UINT(mask, 26, 1);
		vmask14[k].data[13].value = READFROM_UINT(mask, 27, 1);
		vmask14[k].data[14].key = READFROM_UINT(mask, 28, 1);
		vmask14[k].data[14].value = READFROM_UINT(mask, 29, 1);
		vmask14[k].data[15].key = READFROM_UINT(mask, 30, 1);
		vmask14[k].data[15].value = READFROM_UINT(mask, 31, 1);
		vmask15[k].data[0].key = READFROM_UINT(mask, 0, 1);
		vmask15[k].data[0].value = READFROM_UINT(mask, 1, 1);
		vmask15[k].data[1].key = READFROM_UINT(mask, 2, 1);
		vmask15[k].data[1].value = READFROM_UINT(mask, 3, 1);
		vmask15[k].data[2].key = READFROM_UINT(mask, 4, 1);
		vmask15[k].data[2].value = READFROM_UINT(mask, 5, 1);
		vmask15[k].data[3].key = READFROM_UINT(mask, 6, 1);
		vmask15[k].data[3].value = READFROM_UINT(mask, 7, 1);
		vmask15[k].data[4].key = READFROM_UINT(mask, 8, 1);
		vmask15[k].data[4].value = READFROM_UINT(mask, 9, 1);
		vmask15[k].data[5].key = READFROM_UINT(mask, 10, 1);
		vmask15[k].data[5].value = READFROM_UINT(mask, 11, 1);
		vmask15[k].data[6].key = READFROM_UINT(mask, 12, 1);
		vmask15[k].data[6].value = READFROM_UINT(mask, 13, 1);
		vmask15[k].data[7].key = READFROM_UINT(mask, 14, 1);
		vmask15[k].data[7].value = READFROM_UINT(mask, 15, 1);
		vmask15[k].data[8].key = READFROM_UINT(mask, 16, 1);
		vmask15[k].data[8].value = READFROM_UINT(mask, 17, 1);
		vmask15[k].data[9].key = READFROM_UINT(mask, 18, 1);
		vmask15[k].data[9].value = READFROM_UINT(mask, 19, 1);
		vmask15[k].data[10].key = READFROM_UINT(mask, 20, 1);
		vmask15[k].data[10].value = READFROM_UINT(mask, 21, 1);
		vmask15[k].data[11].key = READFROM_UINT(mask, 22, 1);
		vmask15[k].data[11].value = READFROM_UINT(mask, 23, 1);
		vmask15[k].data[12].key = READFROM_UINT(mask, 24, 1);
		vmask15[k].data[12].value = READFROM_UINT(mask, 25, 1);
		vmask15[k].data[13].key = READFROM_UINT(mask, 26, 1);
		vmask15[k].data[13].value = READFROM_UINT(mask, 27, 1);
		vmask15[k].data[14].key = READFROM_UINT(mask, 28, 1);
		vmask15[k].data[14].value = READFROM_UINT(mask, 29, 1);
		vmask15[k].data[15].key = READFROM_UINT(mask, 30, 1);
		vmask15[k].data[15].value = READFROM_UINT(mask, 31, 1);
		#endif
		if(mask > 0){ partitionisactive = ON; }
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printkeyvalues("synchronize.synvbuffer", synvbuffer, NUM_VBUFFERS, 4);
	actsutilityobj->printkeyvalues("synchronize.vmask", _vmask, NUM_PARTITIONS, 4);
	#endif 
	// exit(EXIT_SUCCESS); // 
	return partitionisactive;
}

// acts 
void 
	#ifdef SW 
	acts::
	#endif 
resetenvbuffers(keyvalue_t capsule_so1[8][NUM_PARTITIONS], keyvalue_t capsule_so2[4][NUM_PARTITIONS], keyvalue_t capsule_so4[2][NUM_PARTITIONS], keyvalue_t capsule_so8[NUM_PARTITIONS]){
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
fetchkeyvalues(bool_type enable, unsigned int mode, uint512_dt * kvdram, keyvalue_t vbuffer1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], uintNUMPby2_type vmask[BLOCKRAM_SIZE], keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
		batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, globalparams_t globalparams){
	if(mode == PROCESSMODE){
		readandprocess(enable, kvdram, vbuffer1, vbuffer2, vmask, buffer, goffset_kvs, loffset_kvs, size_kvs, travstate, globalparams);
	} else if(mode == PARTITIONMODE){
		readkeyvalues(enable, kvdram, buffer, goffset_kvs + loffset_kvs, size_kvs, travstate); 
	} else if(mode == REDUCEMODE){
		readkeyvalues(enable, kvdram, buffer, goffset_kvs + loffset_kvs, size_kvs, travstate); 
	} else {}
	return;
}

void 
	#ifdef SW 
	acts::
	#endif 
commitkeyvalues(bool_type enable, unsigned int mode, uint512_dt * kvdram, keyvalue_t vbuffer1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t globalcapsule[GLOBALSTATSBUFFERSZ], keyvalue_t localcapsule[NUM_PARTITIONS], 
		batch_type destbaseaddr_kvs, sweepparams_t sweepparams, globalparams_t globalparams){				
	if(mode == PROCESSMODE){
		savekeyvalues(enable, kvdram, buffer, globalcapsule, localcapsule, destbaseaddr_kvs, globalparams); 
	} else if(mode == PARTITIONMODE){
		savekeyvalues(enable, kvdram, buffer, globalcapsule, localcapsule, destbaseaddr_kvs, globalparams); 
	} else if(mode == REDUCEMODE){
		reduceandbuffer(enable, buffer, localcapsule, vbuffer1, vbuffer2, sweepparams, globalparams);
	} else {}
	return;
}

void 
	#ifdef SW 
	acts::
	#endif
actspipeline(bool_type enable, keyvalue_t bufferA[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t buffer1capsule[VECTOR_SIZE][NUM_PARTITIONS], 
				keyvalue_t bufferB[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t bufferBcapsule[4][NUM_PARTITIONS], 
					keyvalue_t bufferC[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t bufferCcapsule[2][NUM_PARTITIONS],
						keyvalue_t bufferD[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t bufferDcapsule[NUM_PARTITIONS],
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
	}
	
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
	return;
}

void 
	#ifdef SW 
	acts::
	#endif
actit(bool_type enable, unsigned int mode,
		uint512_dt * kvdram, keyvalue_t buffer_setof2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t buffer_setof4[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], uintNUMPby2_type vmask[BLOCKRAM_SIZE], keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ], 
		globalparams_t globalparams, sweepparams_t sweepparams, travstate_t ptravstate, batch_type sourcebaseaddr_kvs, batch_type destbaseaddr_kvs,
		bool_type resetenv, bool_type flush){
	analysis_type analysis_partitionloop = KVDATA_BATCHSIZE_KVS / (NUMPARTITIONUPDATESPIPELINES * WORKBUFFER_SIZE);
	if(enable == OFF){ return; }
	
	static keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = sourcebuffer
	static keyvalue_t buffer_setof1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer_setof1
	static keyvalue_t unused_buffer_setof2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = unused_buffer_setof2
	static keyvalue_t unused_buffer_setof4[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = unused_buffer_setof4
	static keyvalue_t buffer_setof8[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer_setof8
	
	static keyvalue_t capsule_so1[VECTOR_SIZE][NUM_PARTITIONS];
	#pragma HLS array_partition variable = capsule_so1
	static keyvalue_t capsule_so2[4][NUM_PARTITIONS];
	#pragma HLS array_partition variable = capsule_so2
	static keyvalue_t capsule_so4[2][NUM_PARTITIONS];
	#pragma HLS array_partition variable = capsule_so4
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
	
	if(resetenv == ON){ resetenvbuffers(capsule_so1, capsule_so2, capsule_so4, capsule_so8); } else { itercount = 64; } // FIXME. '64' is just some number greater than 2
	if(flush == ON){ flushsz = 3*SRCBUFFER_SIZE; } else { flushsz = 0; }
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
		if(itercount >= 2){ pp0writeen = ON; } else { pp0writeen = OFF; }
		if(itercount >= 4){ pp1writeen = ON; } else { pp1writeen = OFF; }
		if(itercount >= 2){ pp1runpipelineen = ON; } else { pp1runpipelineen = OFF; }
		#else 
		if(itercount >= 2){ pp0writeen = ON; } else { pp0writeen = OFF; }
		#endif
		
		fetchkeyvalues(ON, mode, kvdram, buffer_setof2, buffer_setof4, vmask, sourcebuffer, sourcebaseaddr_kvs, offset_kvs, WORKBUFFER_SIZE, ptravstatepp0, globalparams);
		#ifdef PUP1
		actspipeline(pp1runpipelineen, buffer_setof1, capsule_so1, buffer_setof2, capsule_so2, buffer_setof4, capsule_so4, buffer_setof8, capsule_so8, sweepparams.currentLOP, sweepparams, WORKBUFFER_SIZE, pp1cutoffs, (itercount-2)+1, globalparams);
		#endif 
		
		preparekeyvalues(ON, sourcebuffer, buffer_setof1, capsule_so1, sweepparams.currentLOP, sweepparams, ptravstatepp0, WORKBUFFER_SIZE, pp0cutoffs, globalparams);
		#ifdef PUP1
		commitkeyvalues(pp1writeen, mode, kvdram, buffer_setof2, buffer_setof4, buffer_setof8, globalstatsbuffer, capsule_so8, destbaseaddr_kvs, sweepparams, globalparams); 
		#endif 
		
		actspipeline(ON, buffer_setof1, capsule_so1, buffer_setof2, capsule_so2, buffer_setof4, capsule_so4, buffer_setof8, capsule_so8, sweepparams.currentLOP, sweepparams, WORKBUFFER_SIZE, pp0cutoffs, itercount, globalparams);
		#ifdef PUP1
		fetchkeyvalues(ON, mode, kvdram, buffer_setof2, buffer_setof4, vmask, sourcebuffer, sourcebaseaddr_kvs, offset_kvs + WORKBUFFER_SIZE, WORKBUFFER_SIZE, ptravstatepp1, globalparams);
		#endif
		
		commitkeyvalues(pp0writeen, mode, kvdram, buffer_setof2, buffer_setof4, buffer_setof8, globalstatsbuffer, capsule_so8, destbaseaddr_kvs, sweepparams, globalparams); 
		#ifdef PUP1
		preparekeyvalues(pp1partitionen, sourcebuffer, buffer_setof1, capsule_so1, sweepparams.currentLOP, sweepparams, ptravstatepp1, WORKBUFFER_SIZE, pp1cutoffs, globalparams);
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
processit(uint512_dt * kvdram, keyvalue_t vbuffer1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], uintNUMPby2_type vmask[BLOCKRAM_SIZE], bool_type vpmask[BLOCKRAM_SIZE], globalparams_t globalparams){
	#pragma HLS INLINE
	analysis_type analysis_loop1 = 1;
	#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(_DEBUGMODE_CHECKS2)
	actsutilityobj->clearglobalvars();
	#endif
	
	keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ]; 
	
	batch_type sourcestatsmarker = 0;
	batch_type deststatsmarker = 1;
	sweepparams_t sweepparams;
	
	step_type currentLOP=globalparams.beginLOP;
	resetkeysandvalues(globalstatsbuffer, NUM_PARTITIONS, 0);
	sweepparams = getsweepparams(globalparams, currentLOP, 0);
	travstate_t avtravstate;
	batch_type vmaskoffset_kvs = 0;
	
	step_type ccurrentLOP = globalparams.beginLOP + globalparams.numLOPs - 1;
	batch_type nnum_source_partitions = get_num_source_partitions(ccurrentLOP);
	
	avtravstate.begin_kvs = 0;
	avtravstate.end_kvs = avtravstate.begin_kvs + globalparams.srcvsize / VECTOR2_SIZE; avtravstate.size_kvs = globalparams.srcvsize / VECTOR2_SIZE;
	readglobalstats(ON, kvdram, globalstatsbuffer, globalparams.baseoffset_statsdram_kvs + deststatsmarker); 
	resetvalues(globalstatsbuffer, NUM_PARTITIONS, 0);
	
	batch_type vptrbaseoffset_kvs = globalparams.baseoffset_vertexptr_kvs + (globalparams.srcvoffset / VECTOR_SIZE);
	batch_type vdatabaseoffset_kvs = globalparams.baseoffset_verticesdata_kvs + (globalparams.srcvoffset / VECTOR_SIZE);
	
	#ifdef _WIDEWORD
	vertex_t firstvptr = kvdram[vptrbaseoffset_kvs].range(31, 0);
	#else 
	vertex_t firstvptr = kvdram[vptrbaseoffset_kvs].data[0].key;
	#endif 
	
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->print7("### processit:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, avtravstate.begin_kvs * VECTOR_SIZE, avtravstate.end_kvs * VECTOR_SIZE, (avtravstate.end_kvs - avtravstate.begin_kvs) * VECTOR_SIZE, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams.currentLOP)), sweepparams.currentLOP); 							
	#endif
	MAIN_LOOP: for(batch_type source_partition=0; source_partition<nnum_source_partitions; source_partition+=1){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loop1 avg=analysis_loop1
		
		batch_type voffset_kvs = source_partition * REDUCEBUFFERSZ * (NUM_PARTITIONS / VECTOR_SIZE);
		// if(voffset_kvs >= avtravstate.end_kvs || vpmask[source_partition] == OFF){ continue; }
		if(voffset_kvs >= avtravstate.end_kvs){ continue; }
		#ifndef ALLVERTEXISACTIVE_ALGORITHM // CRITICAL FIXME.
		if(vpmask[source_partition] == OFF){ continue; }
		#endif 
	
		#ifdef _DEBUGMODE_KERNELPRINTS2
		actsutilityobj->print4("### processit:: voffset", "vbegin", "vend", "vskip", voffset_kvs * VECTOR_SIZE, avtravstate.begin_kvs * VECTOR_SIZE, avtravstate.size_kvs * VECTOR_SIZE, SRCBUFFER_SIZE * VECTOR_SIZE);
		#endif
		
		avtravstate.i_kvs = voffset_kvs;
		
		loadvmasks(ON, kvdram, vmask, vbuffer1, globalparams.baseoffset_verticesdatamask_kvs + vmaskoffset_kvs, VMASKBUFFERSZ_KVS); // NOTE: this should come before loadvdata because buffer_setof2 is used as a temp buffer
		readkeyvalues(ON, kvdram, vdatabaseoffset_kvs + voffset_kvs, vbuffer1, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
		readkeyvalues(ON, kvdram, vdatabaseoffset_kvs + voffset_kvs + REDUCEBUFFERSZ, vbuffer2, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
		vmaskoffset_kvs += VMASKBUFFERSZ_KVS;
	
		vertex_t srcvlocaloffset = (voffset_kvs * VECTOR2_SIZE);
		vertex_t beginsrcvid = globalparams.srcvoffset + (voffset_kvs * VECTOR2_SIZE);
		vertex_t endsrcvid = beginsrcvid + ((REDUCEBUFFERSZ * VECTOR2_SIZE) * FETFACTOR);
		if(srcvlocaloffset >= globalparams.srcvsize){ endsrcvid = beginsrcvid; }
		if((srcvlocaloffset < globalparams.srcvsize) && (srcvlocaloffset + ((REDUCEBUFFERSZ * VECTOR2_SIZE) * FETFACTOR) >= globalparams.srcvsize)){ endsrcvid = beginsrcvid + globalparams.srcvsize - srcvlocaloffset; }
		
		#ifdef _WIDEWORD
		keyy_t beginvptr = kvdram[vptrbaseoffset_kvs + voffset_kvs].range(31, 0);
		keyy_t endvptr = kvdram[vptrbaseoffset_kvs + voffset_kvs + (REDUCEBUFFERSZ * FETFACTOR) + 1].range(31, 0); 
		#else 
		keyy_t beginvptr = kvdram[vptrbaseoffset_kvs + voffset_kvs].data[0].key;
		keyy_t endvptr = kvdram[vptrbaseoffset_kvs + voffset_kvs + (REDUCEBUFFERSZ * FETFACTOR) + 1].data[0].key;
		#endif 
		if(srcvlocaloffset >= globalparams.srcvsize){ endvptr = beginvptr; }
		
		keyy_t localbeginvptr = beginvptr - firstvptr;
		keyy_t localendvptr = endvptr - firstvptr;
		keyy_t numedges = localendvptr - localbeginvptr + 2*VECTOR_SIZE;
		if(localbeginvptr == localendvptr){ numedges = 0; }
		keyy_t localbeginvptr_kvs = localbeginvptr / VECTOR_SIZE; // NOTE: this should be in KVS(8) terms
		keyy_t numedges_kvs = numedges / VECTOR_SIZE;
		#ifdef _DEBUGMODE_CHECKS2
		if(localendvptr < localbeginvptr){ cout<<"processit::ERROR: localendvptr < localbeginvptr. localbeginvptr: "<<localbeginvptr<<", localendvptr: "<<localendvptr<<endl; exit(EXIT_FAILURE); }
		if(localendvptr < globalparams.edgessize){ actsutilityobj->checkptr("processit", beginsrcvid, endsrcvid, localbeginvptr, localendvptr, (keyvalue_t *)&kvdram[BASEOFFSET_EDGESDATA_KVS]); }
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
		if((voffset_kvs + (REDUCEBUFFERSZ * FETFACTOR)) >= avtravstate.end_kvs){ flush = ON; } else { flush = OFF; }
		
		actit(
			ON, PROCESSMODE,
			kvdram, vbuffer1, vbuffer2, vmask, globalstatsbuffer, 
			globalparams, sweepparams, etravstate, globalparams.baseoffset_edgesdata_kvs, globalparams.baseoffset_kvdramworkspace_kvs,
			resetenv, flush);

		#ifdef _DEBUGMODE_KERNELPRINTS
		actsutilityobj->printglobalvars();
		actsutilityobj->clearglobalvars();
		#endif
	}
	saveglobalstats(ON, kvdram, globalstatsbuffer, globalparams.baseoffset_statsdram_kvs + deststatsmarker);
	
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
partitionit(uint512_dt * kvdram, keyvalue_t vbuffer1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], uintNUMPby2_type vmask[BLOCKRAM_SIZE], globalparams_t globalparams){
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
	
	batch_type sourcestatsmarker = 1;
	batch_type deststatsmarker = 1 + NUM_PARTITIONS;
	config_t config;
	sweepparams_t sweepparams;
	travstate_t actvvstravstate; actvvstravstate.i=0; actvvstravstate.i_kvs=0; // actvvstravstate.v=0; actvvstravstate.k=0; 
	
	MAIN_LOOP1: for(step_type currentLOP=globalparams.beginLOP + 1; currentLOP<(globalparams.beginLOP + globalparams.numLOPs - 1); currentLOP++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_numllops avg=analysis_numllops	
	
		batch_type num_source_partitions = get_num_source_partitions(currentLOP);
		bool_type enreduce = ON;
		
		MAIN_LOOP1B: for(batch_type source_partition=0; source_partition<num_source_partitions; source_partition+=1){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_numsourcepartitions avg=analysis_numsourcepartitions

			resetkeysandvalues(globalstatsbuffer, NUM_PARTITIONS, 0);
		
			sweepparams = getsweepparams(globalparams, currentLOP, source_partition);
			travstate_t travstate = gettravstate(ON, kvdram, globalparams, currentLOP, sourcestatsmarker);
			travstate_t ctravstate = travstate;
			travstate_t ptravstate = travstate;
			travstate_t avtravstate;
			
			// collect stats
			config.enablecollectglobalstats = ON;
			if((ctravstate.end_kvs - ctravstate.begin_kvs) == 0){ ctravstate.begin_kvs = 0; ctravstate.end_kvs = 0; config.enablecollectglobalstats = OFF; }
			readglobalstats(config.enablecollectglobalstats, kvdram, globalstatsbuffer, globalparams.baseoffset_statsdram_kvs + deststatsmarker);
			resetvalues(globalstatsbuffer, NUM_PARTITIONS, 0);
			
			// partition
			if(ptravstate.size_kvs > 0){ config.enablepartition = ON; } 
			else { ptravstate.begin_kvs = 0; ptravstate.end_kvs = 0; config.enablepartition = OFF; }
			if(ptravstate.size_kvs == 0){ ptravstate.begin_kvs = 0; ptravstate.end_kvs = 0; config.enablepartition = OFF; }
			#ifdef _DEBUGMODE_KERNELPRINTS2
			if((config.enablepartition == ON) && (currentLOP >= 1) && (currentLOP <= globalparams.treedepth)){ actsutilityobj->print7("### partitionit:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, ptravstate.begin_kvs * VECTOR_SIZE, ptravstate.end_kvs * VECTOR_SIZE, ptravstate.size_kvs * VECTOR_SIZE, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams.currentLOP)), sweepparams.currentLOP); }	
			#endif
			resetvalues(globalstatsbuffer, NUM_PARTITIONS, 0);
			bool_type resetenv; if(source_partition==0){ resetenv = ON; } else { resetenv = OFF; }
			actit(config.enablepartition, PARTITIONMODE,
					kvdram, vbuffer1, vbuffer2, vmask, globalstatsbuffer,
					globalparams, sweepparams, ptravstate, sweepparams.worksourcebaseaddress_kvs, sweepparams.workdestbaseaddress_kvs,
					ON, ON);
			saveglobalstats(config.enablepartition, kvdram, globalstatsbuffer, globalparams.baseoffset_statsdram_kvs + deststatsmarker); 
			
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
reduceit(uint512_dt * kvdram, keyvalue_t vbuffer1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], uintNUMPby2_type vmask[BLOCKRAM_SIZE], batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparams){	
	#pragma HLS INLINE
	analysis_type analysis_numllops = 1;
	analysis_type analysis_numsourcepartitions = 1;
	analysis_type analysis_treedepth = TREE_DEPTH;
	
	keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ]; 
	
	config_t config;
	sweepparams_t sweepparams;
	
	step_type currentLOP = globalparams.beginLOP + globalparams.numLOPs - 1;
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
			kvdram, vbuffer1, vbuffer2, vmask, globalstatsbuffer,
			globalparams, sweepparams, ptravstate, sweepparams.worksourcebaseaddress_kvs, sweepparams.workdestbaseaddress_kvs,
			ON, ON);
	return;
}

void
	#ifdef SW 
	acts::
	#endif 
dispatch(bool_type en_process, bool_type en_partition, bool_type en_reduce, uint512_dt * kvdram, keyvalue_t vbuffer1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], uintNUMPby2_type vmask[BLOCKRAM_SIZE], bool_type vpmask[BLOCKRAM_SIZE],
			batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparams){
	if(en_process == ON){ processit(kvdram, vbuffer1, vbuffer2, vmask, vpmask, globalparams); }
	if(en_partition == ON){ partitionit(kvdram, vbuffer1, vbuffer2, vmask, globalparams); }
	if(en_reduce == ON){ reduceit(kvdram, vbuffer1, vbuffer2, vmask, sourcestatsmarker, source_partition, globalparams); }
	return;
}

void
	#ifdef SW 
	acts::
	#endif 
start_reduce(uint512_dt * kvdram0, keyvalue_t vbuffer0_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer0_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], uintNUMPby2_type vmask0[BLOCKRAM_SIZE],uint512_dt * kvdram1, keyvalue_t vbuffer1_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer1_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], uintNUMPby2_type vmask1[BLOCKRAM_SIZE],uint512_dt * kvdram2, keyvalue_t vbuffer2_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer2_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], uintNUMPby2_type vmask2[BLOCKRAM_SIZE],uint512_dt * kvdram3, keyvalue_t vbuffer3_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer3_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], uintNUMPby2_type vmask3[BLOCKRAM_SIZE],uint512_dt * kvdram4, keyvalue_t vbuffer4_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer4_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], uintNUMPby2_type vmask4[BLOCKRAM_SIZE],uint512_dt * kvdram5, keyvalue_t vbuffer5_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer5_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], uintNUMPby2_type vmask5[BLOCKRAM_SIZE],uint512_dt * kvdram6, keyvalue_t vbuffer6_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer6_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], uintNUMPby2_type vmask6[BLOCKRAM_SIZE],uint512_dt * kvdram7, keyvalue_t vbuffer7_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer7_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], uintNUMPby2_type vmask7[BLOCKRAM_SIZE],uint512_dt * kvdram8, keyvalue_t vbuffer8_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer8_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], uintNUMPby2_type vmask8[BLOCKRAM_SIZE],uint512_dt * kvdram9, keyvalue_t vbuffer9_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer9_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], uintNUMPby2_type vmask9[BLOCKRAM_SIZE],uint512_dt * kvdram10, keyvalue_t vbuffer10_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer10_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], uintNUMPby2_type vmask10[BLOCKRAM_SIZE],uint512_dt * kvdram11, keyvalue_t vbuffer11_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer11_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], uintNUMPby2_type vmask11[BLOCKRAM_SIZE],uint512_dt * kvdram12, keyvalue_t vbuffer12_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer12_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], uintNUMPby2_type vmask12[BLOCKRAM_SIZE],uint512_dt * kvdram13, keyvalue_t vbuffer13_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer13_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], uintNUMPby2_type vmask13[BLOCKRAM_SIZE],uint512_dt * kvdram14, keyvalue_t vbuffer14_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer14_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], uintNUMPby2_type vmask14[BLOCKRAM_SIZE],uint512_dt * kvdram15, keyvalue_t vbuffer15_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t vbuffer15_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE], uintNUMPby2_type vmask15[BLOCKRAM_SIZE], bool_type vpmask[BLOCKRAM_SIZE], globalparams_t globalparams[NUMCOMPUTEUNITS]){
	#pragma HLS INLINE
	analysis_type analysis_loop1 = 1;
	analysis_type analysis_treedepth = TREE_DEPTH;
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printparameters();
	actsutilityobj->printglobalvars();
	#endif 
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	keyvalue_t synvbuffer0_1[VECTOR_SIZE][BLOCKRAM_SIZE]; 
	#pragma HLS array_partition variable = synvbuffer0_1
	keyvalue_t synvbuffer0_2[VECTOR_SIZE][BLOCKRAM_SIZE]; 
	#pragma HLS array_partition variable = synvbuffer0_2
	uintNUMPby2_type synvmask0[BLOCKRAM_SIZE];
	keyvalue_t synvbuffer1_1[VECTOR_SIZE][BLOCKRAM_SIZE]; 
	#pragma HLS array_partition variable = synvbuffer1_1
	keyvalue_t synvbuffer1_2[VECTOR_SIZE][BLOCKRAM_SIZE]; 
	#pragma HLS array_partition variable = synvbuffer1_2
	uintNUMPby2_type synvmask1[BLOCKRAM_SIZE];
	keyvalue_t synvbuffer2_1[VECTOR_SIZE][BLOCKRAM_SIZE]; 
	#pragma HLS array_partition variable = synvbuffer2_1
	keyvalue_t synvbuffer2_2[VECTOR_SIZE][BLOCKRAM_SIZE]; 
	#pragma HLS array_partition variable = synvbuffer2_2
	uintNUMPby2_type synvmask2[BLOCKRAM_SIZE];
	keyvalue_t synvbuffer3_1[VECTOR_SIZE][BLOCKRAM_SIZE]; 
	#pragma HLS array_partition variable = synvbuffer3_1
	keyvalue_t synvbuffer3_2[VECTOR_SIZE][BLOCKRAM_SIZE]; 
	#pragma HLS array_partition variable = synvbuffer3_2
	uintNUMPby2_type synvmask3[BLOCKRAM_SIZE];
	keyvalue_t synvbuffer4_1[VECTOR_SIZE][BLOCKRAM_SIZE]; 
	#pragma HLS array_partition variable = synvbuffer4_1
	keyvalue_t synvbuffer4_2[VECTOR_SIZE][BLOCKRAM_SIZE]; 
	#pragma HLS array_partition variable = synvbuffer4_2
	uintNUMPby2_type synvmask4[BLOCKRAM_SIZE];
	
	keyvalue_t synvbuffer[NUM_VBUFFERS][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = synvbuffer
	
	bool_type vpmask0[BLOCKRAM_SIZE];
	bool_type vpmask1[BLOCKRAM_SIZE];
	bool_type vpmask2[BLOCKRAM_SIZE];
	bool_type vpmask3[BLOCKRAM_SIZE];
	bool_type vpmask4[BLOCKRAM_SIZE];
	bool_type vpmask5[BLOCKRAM_SIZE];
	bool_type vpmask6[BLOCKRAM_SIZE];
	bool_type vpmask7[BLOCKRAM_SIZE];
	bool_type vpmask8[BLOCKRAM_SIZE];
	bool_type vpmask9[BLOCKRAM_SIZE];
	bool_type vpmask10[BLOCKRAM_SIZE];
	bool_type vpmask11[BLOCKRAM_SIZE];
	bool_type vpmask12[BLOCKRAM_SIZE];
	bool_type vpmask13[BLOCKRAM_SIZE];
	bool_type vpmask14[BLOCKRAM_SIZE];
	bool_type vpmask15[BLOCKRAM_SIZE];
	travstate_t rtravstate[NUMCOMPUTEUNITS];
	#pragma HLS ARRAY_PARTITION variable=rtravstate complete
	
	globalparams_t _globalparams = globalparams[0];
	unsigned int sourcestatsmarker = 0;
	for(unsigned int k=0; k<_globalparams.treedepth-1; k++){ 
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_treedepth avg=analysis_treedepth
		sourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); 
	}
	step_type currentLOP = _globalparams.beginLOP + _globalparams.numLOPs - 1;
	batch_type num_source_partitions = get_num_source_partitions(currentLOP);
	for(unsigned int k=0; k<num_source_partitions; k++){ vpmask[k] = OFF; }
	
	bool_type enablereduce = ON;
	unsigned int ntravszs = 0;
	
	MAIN_LOOP: for(batch_type source_partition=0; source_partition<num_source_partitions; source_partition+=1){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loop1 avg=analysis_loop1
		#ifdef _DEBUGMODE_KERNELPRINTS
		actsutilityobj->print3("### start_reduce:: source_partition", "currentLOP", "NAp", source_partition, currentLOP, NAp); 							
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
		
		// read vertices 
		loadvdata(enablereduce, kvdram0, synvbuffer, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2), REDUCEBUFFERSZ, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2) + REDUCEBUFFERSZ, REDUCEBUFFERSZ); // CRITICAL FIXME. too expensive here
	
		readkeyvalues(enablereduce, kvdram0, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2), vbuffer0_1, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
		readkeyvalues(enablereduce, kvdram0, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2) + REDUCEBUFFERSZ, vbuffer0_2, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
	
		readkeyvalues(enablereduce, kvdram1, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2), vbuffer1_1, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
		readkeyvalues(enablereduce, kvdram1, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2) + REDUCEBUFFERSZ, vbuffer1_2, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
	
		readkeyvalues(enablereduce, kvdram2, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2), vbuffer2_1, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
		readkeyvalues(enablereduce, kvdram2, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2) + REDUCEBUFFERSZ, vbuffer2_2, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
	
		readkeyvalues(enablereduce, kvdram3, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2), vbuffer3_1, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
		readkeyvalues(enablereduce, kvdram3, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2) + REDUCEBUFFERSZ, vbuffer3_2, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
	
		readkeyvalues(enablereduce, kvdram4, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2), vbuffer4_1, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
		readkeyvalues(enablereduce, kvdram4, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2) + REDUCEBUFFERSZ, vbuffer4_2, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
	
		readkeyvalues(enablereduce, kvdram5, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2), vbuffer5_1, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
		readkeyvalues(enablereduce, kvdram5, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2) + REDUCEBUFFERSZ, vbuffer5_2, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
	
		readkeyvalues(enablereduce, kvdram6, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2), vbuffer6_1, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
		readkeyvalues(enablereduce, kvdram6, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2) + REDUCEBUFFERSZ, vbuffer6_2, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
	
		readkeyvalues(enablereduce, kvdram7, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2), vbuffer7_1, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
		readkeyvalues(enablereduce, kvdram7, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2) + REDUCEBUFFERSZ, vbuffer7_2, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
	
		readkeyvalues(enablereduce, kvdram8, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2), vbuffer8_1, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
		readkeyvalues(enablereduce, kvdram8, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2) + REDUCEBUFFERSZ, vbuffer8_2, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
	
		readkeyvalues(enablereduce, kvdram9, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2), vbuffer9_1, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
		readkeyvalues(enablereduce, kvdram9, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2) + REDUCEBUFFERSZ, vbuffer9_2, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
	
		readkeyvalues(enablereduce, kvdram10, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2), vbuffer10_1, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
		readkeyvalues(enablereduce, kvdram10, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2) + REDUCEBUFFERSZ, vbuffer10_2, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
	
		readkeyvalues(enablereduce, kvdram11, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2), vbuffer11_1, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
		readkeyvalues(enablereduce, kvdram11, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2) + REDUCEBUFFERSZ, vbuffer11_2, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
	
		readkeyvalues(enablereduce, kvdram12, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2), vbuffer12_1, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
		readkeyvalues(enablereduce, kvdram12, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2) + REDUCEBUFFERSZ, vbuffer12_2, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
	
		readkeyvalues(enablereduce, kvdram13, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2), vbuffer13_1, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
		readkeyvalues(enablereduce, kvdram13, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2) + REDUCEBUFFERSZ, vbuffer13_2, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
	
		readkeyvalues(enablereduce, kvdram14, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2), vbuffer14_1, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
		readkeyvalues(enablereduce, kvdram14, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2) + REDUCEBUFFERSZ, vbuffer14_2, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
	
		readkeyvalues(enablereduce, kvdram15, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2), vbuffer15_1, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
		readkeyvalues(enablereduce, kvdram15, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2) + REDUCEBUFFERSZ, vbuffer15_2, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
	
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
		
		// reduce
		dispatch(OFF, OFF, enablereduce, kvdram0, vbuffer0_1, vbuffer0_2, vmask0, vpmask0, sourcestatsmarker, source_partition, _globalparams);
		dispatch(OFF, OFF, enablereduce, kvdram1, vbuffer1_1, vbuffer1_2, vmask1, vpmask1, sourcestatsmarker, source_partition, _globalparams);
		dispatch(OFF, OFF, enablereduce, kvdram2, vbuffer2_1, vbuffer2_2, vmask2, vpmask2, sourcestatsmarker, source_partition, _globalparams);
		dispatch(OFF, OFF, enablereduce, kvdram3, vbuffer3_1, vbuffer3_2, vmask3, vpmask3, sourcestatsmarker, source_partition, _globalparams);
		dispatch(OFF, OFF, enablereduce, kvdram4, vbuffer4_1, vbuffer4_2, vmask4, vpmask4, sourcestatsmarker, source_partition, _globalparams);
		dispatch(OFF, OFF, enablereduce, kvdram5, vbuffer5_1, vbuffer5_2, vmask5, vpmask5, sourcestatsmarker, source_partition, _globalparams);
		dispatch(OFF, OFF, enablereduce, kvdram6, vbuffer6_1, vbuffer6_2, vmask6, vpmask6, sourcestatsmarker, source_partition, _globalparams);
		dispatch(OFF, OFF, enablereduce, kvdram7, vbuffer7_1, vbuffer7_2, vmask7, vpmask7, sourcestatsmarker, source_partition, _globalparams);
		dispatch(OFF, OFF, enablereduce, kvdram8, vbuffer8_1, vbuffer8_2, vmask8, vpmask8, sourcestatsmarker, source_partition, _globalparams);
		dispatch(OFF, OFF, enablereduce, kvdram9, vbuffer9_1, vbuffer9_2, vmask9, vpmask9, sourcestatsmarker, source_partition, _globalparams);
		dispatch(OFF, OFF, enablereduce, kvdram10, vbuffer10_1, vbuffer10_2, vmask10, vpmask10, sourcestatsmarker, source_partition, _globalparams);
		dispatch(OFF, OFF, enablereduce, kvdram11, vbuffer11_1, vbuffer11_2, vmask11, vpmask11, sourcestatsmarker, source_partition, _globalparams);
		dispatch(OFF, OFF, enablereduce, kvdram12, vbuffer12_1, vbuffer12_2, vmask12, vpmask12, sourcestatsmarker, source_partition, _globalparams);
		dispatch(OFF, OFF, enablereduce, kvdram13, vbuffer13_1, vbuffer13_2, vmask13, vpmask13, sourcestatsmarker, source_partition, _globalparams);
		dispatch(OFF, OFF, enablereduce, kvdram14, vbuffer14_1, vbuffer14_2, vmask14, vpmask14, sourcestatsmarker, source_partition, _globalparams);
		dispatch(OFF, OFF, enablereduce, kvdram15, vbuffer15_1, vbuffer15_2, vmask15, vpmask15, sourcestatsmarker, source_partition, _globalparams);
		
		// synchronize
		#ifdef DISPATCHTYPE_SYNC // CRITICAL REMOVEME.
		vpmask[source_partition] = synchronize(enablereduce, synvbuffer, vbuffer0_1, vbuffer0_2, vmask0,vbuffer1_1, vbuffer1_2, vmask1,vbuffer2_1, vbuffer2_2, vmask2,vbuffer3_1, vbuffer3_2, vmask3,vbuffer4_1, vbuffer4_2, vmask4,vbuffer5_1, vbuffer5_2, vmask5,vbuffer6_1, vbuffer6_2, vmask6,vbuffer7_1, vbuffer7_2, vmask7,vbuffer8_1, vbuffer8_2, vmask8,vbuffer9_1, vbuffer9_2, vmask9,vbuffer10_1, vbuffer10_2, vmask10,vbuffer11_1, vbuffer11_2, vmask11,vbuffer12_1, vbuffer12_2, vmask12,vbuffer13_1, vbuffer13_2, vmask13,vbuffer14_1, vbuffer14_2, vmask14,vbuffer15_1, vbuffer15_2, vmask15, _globalparams);
		#endif
		
		// writeback vertices
		savekeyvalues(enablereduce, kvdram0, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2), vbuffer0_1, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
		savekeyvalues(enablereduce, kvdram0, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2) + REDUCEBUFFERSZ, vbuffer0_2, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
		savekeyvalues(enablereduce, kvdram1, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2), vbuffer1_1, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
		savekeyvalues(enablereduce, kvdram1, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2) + REDUCEBUFFERSZ, vbuffer1_2, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
		savekeyvalues(enablereduce, kvdram2, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2), vbuffer2_1, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
		savekeyvalues(enablereduce, kvdram2, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2) + REDUCEBUFFERSZ, vbuffer2_2, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
		savekeyvalues(enablereduce, kvdram3, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2), vbuffer3_1, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
		savekeyvalues(enablereduce, kvdram3, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2) + REDUCEBUFFERSZ, vbuffer3_2, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
		savekeyvalues(enablereduce, kvdram4, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2), vbuffer4_1, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
		savekeyvalues(enablereduce, kvdram4, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2) + REDUCEBUFFERSZ, vbuffer4_2, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
		savekeyvalues(enablereduce, kvdram5, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2), vbuffer5_1, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
		savekeyvalues(enablereduce, kvdram5, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2) + REDUCEBUFFERSZ, vbuffer5_2, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
		savekeyvalues(enablereduce, kvdram6, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2), vbuffer6_1, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
		savekeyvalues(enablereduce, kvdram6, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2) + REDUCEBUFFERSZ, vbuffer6_2, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
		savekeyvalues(enablereduce, kvdram7, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2), vbuffer7_1, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
		savekeyvalues(enablereduce, kvdram7, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2) + REDUCEBUFFERSZ, vbuffer7_2, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
		savekeyvalues(enablereduce, kvdram8, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2), vbuffer8_1, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
		savekeyvalues(enablereduce, kvdram8, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2) + REDUCEBUFFERSZ, vbuffer8_2, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
		savekeyvalues(enablereduce, kvdram9, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2), vbuffer9_1, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
		savekeyvalues(enablereduce, kvdram9, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2) + REDUCEBUFFERSZ, vbuffer9_2, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
		savekeyvalues(enablereduce, kvdram10, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2), vbuffer10_1, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
		savekeyvalues(enablereduce, kvdram10, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2) + REDUCEBUFFERSZ, vbuffer10_2, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
		savekeyvalues(enablereduce, kvdram11, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2), vbuffer11_1, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
		savekeyvalues(enablereduce, kvdram11, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2) + REDUCEBUFFERSZ, vbuffer11_2, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
		savekeyvalues(enablereduce, kvdram12, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2), vbuffer12_1, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
		savekeyvalues(enablereduce, kvdram12, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2) + REDUCEBUFFERSZ, vbuffer12_2, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
		savekeyvalues(enablereduce, kvdram13, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2), vbuffer13_1, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
		savekeyvalues(enablereduce, kvdram13, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2) + REDUCEBUFFERSZ, vbuffer13_2, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
		savekeyvalues(enablereduce, kvdram14, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2), vbuffer14_1, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
		savekeyvalues(enablereduce, kvdram14, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2) + REDUCEBUFFERSZ, vbuffer14_2, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
		savekeyvalues(enablereduce, kvdram15, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2), vbuffer15_1, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
		savekeyvalues(enablereduce, kvdram15, _globalparams.baseoffset_verticesdata_kvs + (source_partition * REDUCEBUFFERSZ * 2) + REDUCEBUFFERSZ, vbuffer15_2, BUFFERBASE_VDATA, REDUCEBUFFERSZ);
		savevmasks(enablereduce, kvdram0, vmask0, vbuffer0_1, _globalparams.baseoffset_verticesdatamask_kvs + (source_partition * VMASKBUFFERSZ_KVS), VMASKBUFFERSZ_KVS);
		savevmasks(enablereduce, kvdram1, vmask1, vbuffer1_1, _globalparams.baseoffset_verticesdatamask_kvs + (source_partition * VMASKBUFFERSZ_KVS), VMASKBUFFERSZ_KVS);
		savevmasks(enablereduce, kvdram2, vmask2, vbuffer2_1, _globalparams.baseoffset_verticesdatamask_kvs + (source_partition * VMASKBUFFERSZ_KVS), VMASKBUFFERSZ_KVS);
		savevmasks(enablereduce, kvdram3, vmask3, vbuffer3_1, _globalparams.baseoffset_verticesdatamask_kvs + (source_partition * VMASKBUFFERSZ_KVS), VMASKBUFFERSZ_KVS);
		savevmasks(enablereduce, kvdram4, vmask4, vbuffer4_1, _globalparams.baseoffset_verticesdatamask_kvs + (source_partition * VMASKBUFFERSZ_KVS), VMASKBUFFERSZ_KVS);
		savevmasks(enablereduce, kvdram5, vmask5, vbuffer5_1, _globalparams.baseoffset_verticesdatamask_kvs + (source_partition * VMASKBUFFERSZ_KVS), VMASKBUFFERSZ_KVS);
		savevmasks(enablereduce, kvdram6, vmask6, vbuffer6_1, _globalparams.baseoffset_verticesdatamask_kvs + (source_partition * VMASKBUFFERSZ_KVS), VMASKBUFFERSZ_KVS);
		savevmasks(enablereduce, kvdram7, vmask7, vbuffer7_1, _globalparams.baseoffset_verticesdatamask_kvs + (source_partition * VMASKBUFFERSZ_KVS), VMASKBUFFERSZ_KVS);
		savevmasks(enablereduce, kvdram8, vmask8, vbuffer8_1, _globalparams.baseoffset_verticesdatamask_kvs + (source_partition * VMASKBUFFERSZ_KVS), VMASKBUFFERSZ_KVS);
		savevmasks(enablereduce, kvdram9, vmask9, vbuffer9_1, _globalparams.baseoffset_verticesdatamask_kvs + (source_partition * VMASKBUFFERSZ_KVS), VMASKBUFFERSZ_KVS);
		savevmasks(enablereduce, kvdram10, vmask10, vbuffer10_1, _globalparams.baseoffset_verticesdatamask_kvs + (source_partition * VMASKBUFFERSZ_KVS), VMASKBUFFERSZ_KVS);
		savevmasks(enablereduce, kvdram11, vmask11, vbuffer11_1, _globalparams.baseoffset_verticesdatamask_kvs + (source_partition * VMASKBUFFERSZ_KVS), VMASKBUFFERSZ_KVS);
		savevmasks(enablereduce, kvdram12, vmask12, vbuffer12_1, _globalparams.baseoffset_verticesdatamask_kvs + (source_partition * VMASKBUFFERSZ_KVS), VMASKBUFFERSZ_KVS);
		savevmasks(enablereduce, kvdram13, vmask13, vbuffer13_1, _globalparams.baseoffset_verticesdatamask_kvs + (source_partition * VMASKBUFFERSZ_KVS), VMASKBUFFERSZ_KVS);
		savevmasks(enablereduce, kvdram14, vmask14, vbuffer14_1, _globalparams.baseoffset_verticesdatamask_kvs + (source_partition * VMASKBUFFERSZ_KVS), VMASKBUFFERSZ_KVS);
		savevmasks(enablereduce, kvdram15, vmask15, vbuffer15_1, _globalparams.baseoffset_verticesdatamask_kvs + (source_partition * VMASKBUFFERSZ_KVS), VMASKBUFFERSZ_KVS);
		
		sourcestatsmarker += 1;
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printglobalvars();
	#endif
	return;
} 

void
	#ifdef SW 
	acts::
	#endif 
start(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11,uint512_dt * kvdram12,uint512_dt * kvdram13,uint512_dt * kvdram14,uint512_dt * kvdram15, bool_type vpmask[BLOCKRAM_SIZE], globalparams_t globalparams[NUMCOMPUTEUNITS]){
	
	uintNUMPby2_type vmask0[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask0
	uintNUMPby2_type vmask1[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask1
	uintNUMPby2_type vmask2[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask2
	uintNUMPby2_type vmask3[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask3
	uintNUMPby2_type vmask4[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask4
	uintNUMPby2_type vmask5[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask5
	uintNUMPby2_type vmask6[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask6
	uintNUMPby2_type vmask7[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask7
	uintNUMPby2_type vmask8[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask8
	uintNUMPby2_type vmask9[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask9
	uintNUMPby2_type vmask10[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask10
	uintNUMPby2_type vmask11[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask11
	uintNUMPby2_type vmask12[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask12
	uintNUMPby2_type vmask13[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask13
	uintNUMPby2_type vmask14[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask14
	uintNUMPby2_type vmask15[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask15
	
	keyvalue_t vbuffer0_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer0_1	
	keyvalue_t vbuffer0_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer0_2
	keyvalue_t vbuffer1_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer1_1	
	keyvalue_t vbuffer1_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer1_2
	keyvalue_t vbuffer2_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer2_1	
	keyvalue_t vbuffer2_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer2_2
	keyvalue_t vbuffer3_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer3_1	
	keyvalue_t vbuffer3_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer3_2
	keyvalue_t vbuffer4_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer4_1	
	keyvalue_t vbuffer4_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer4_2
	keyvalue_t vbuffer5_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer5_1	
	keyvalue_t vbuffer5_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer5_2
	keyvalue_t vbuffer6_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer6_1	
	keyvalue_t vbuffer6_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer6_2
	keyvalue_t vbuffer7_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer7_1	
	keyvalue_t vbuffer7_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer7_2
	keyvalue_t vbuffer8_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer8_1	
	keyvalue_t vbuffer8_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer8_2
	keyvalue_t vbuffer9_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer9_1	
	keyvalue_t vbuffer9_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer9_2
	keyvalue_t vbuffer10_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer10_1	
	keyvalue_t vbuffer10_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer10_2
	keyvalue_t vbuffer11_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer11_1	
	keyvalue_t vbuffer11_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer11_2
	keyvalue_t vbuffer12_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer12_1	
	keyvalue_t vbuffer12_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer12_2
	keyvalue_t vbuffer13_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer13_1	
	keyvalue_t vbuffer13_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer13_2
	keyvalue_t vbuffer14_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer14_1	
	keyvalue_t vbuffer14_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer14_2
	keyvalue_t vbuffer15_1[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer15_1	
	keyvalue_t vbuffer15_2[VECTOR_SIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer15_2

	// process & partition
	if(globalparams[0].processcommand == ON){ 
 // COMPUTEUNITS_seq
		#ifdef _DEBUGMODE_KERNELPRINTS2
		cout<<"start: processing instance 0... "<<endl;
		#endif
		dispatch(ON, OFF, OFF, kvdram0, vbuffer0_1, vbuffer0_2, vmask0, vpmask, NAp, NAp, globalparams[0]);
 // COMPUTEUNITS_seq
		#ifdef _DEBUGMODE_KERNELPRINTS2
		cout<<"start: processing instance 1... "<<endl;
		#endif
		dispatch(ON, OFF, OFF, kvdram1, vbuffer1_1, vbuffer1_2, vmask1, vpmask, NAp, NAp, globalparams[1]);
 // COMPUTEUNITS_seq
		#ifdef _DEBUGMODE_KERNELPRINTS2
		cout<<"start: processing instance 2... "<<endl;
		#endif
		dispatch(ON, OFF, OFF, kvdram2, vbuffer2_1, vbuffer2_2, vmask2, vpmask, NAp, NAp, globalparams[2]);
 // COMPUTEUNITS_seq
		#ifdef _DEBUGMODE_KERNELPRINTS2
		cout<<"start: processing instance 3... "<<endl;
		#endif
		dispatch(ON, OFF, OFF, kvdram3, vbuffer3_1, vbuffer3_2, vmask3, vpmask, NAp, NAp, globalparams[3]);
 // COMPUTEUNITS_seq
		#ifdef _DEBUGMODE_KERNELPRINTS2
		cout<<"start: processing instance 4... "<<endl;
		#endif
		dispatch(ON, OFF, OFF, kvdram4, vbuffer4_1, vbuffer4_2, vmask4, vpmask, NAp, NAp, globalparams[4]);
 // COMPUTEUNITS_seq
		#ifdef _DEBUGMODE_KERNELPRINTS2
		cout<<"start: processing instance 5... "<<endl;
		#endif
		dispatch(ON, OFF, OFF, kvdram5, vbuffer5_1, vbuffer5_2, vmask5, vpmask, NAp, NAp, globalparams[5]);
 // COMPUTEUNITS_seq
		#ifdef _DEBUGMODE_KERNELPRINTS2
		cout<<"start: processing instance 6... "<<endl;
		#endif
		dispatch(ON, OFF, OFF, kvdram6, vbuffer6_1, vbuffer6_2, vmask6, vpmask, NAp, NAp, globalparams[6]);
 // COMPUTEUNITS_seq
		#ifdef _DEBUGMODE_KERNELPRINTS2
		cout<<"start: processing instance 7... "<<endl;
		#endif
		dispatch(ON, OFF, OFF, kvdram7, vbuffer7_1, vbuffer7_2, vmask7, vpmask, NAp, NAp, globalparams[7]);
 // COMPUTEUNITS_seq
		#ifdef _DEBUGMODE_KERNELPRINTS2
		cout<<"start: processing instance 8... "<<endl;
		#endif
		dispatch(ON, OFF, OFF, kvdram8, vbuffer8_1, vbuffer8_2, vmask8, vpmask, NAp, NAp, globalparams[8]);
 // COMPUTEUNITS_seq
		#ifdef _DEBUGMODE_KERNELPRINTS2
		cout<<"start: processing instance 9... "<<endl;
		#endif
		dispatch(ON, OFF, OFF, kvdram9, vbuffer9_1, vbuffer9_2, vmask9, vpmask, NAp, NAp, globalparams[9]);
 // COMPUTEUNITS_seq
		#ifdef _DEBUGMODE_KERNELPRINTS2
		cout<<"start: processing instance 10... "<<endl;
		#endif
		dispatch(ON, OFF, OFF, kvdram10, vbuffer10_1, vbuffer10_2, vmask10, vpmask, NAp, NAp, globalparams[10]);
 // COMPUTEUNITS_seq
		#ifdef _DEBUGMODE_KERNELPRINTS2
		cout<<"start: processing instance 11... "<<endl;
		#endif
		dispatch(ON, OFF, OFF, kvdram11, vbuffer11_1, vbuffer11_2, vmask11, vpmask, NAp, NAp, globalparams[11]);
 // COMPUTEUNITS_seq
		#ifdef _DEBUGMODE_KERNELPRINTS2
		cout<<"start: processing instance 12... "<<endl;
		#endif
		dispatch(ON, OFF, OFF, kvdram12, vbuffer12_1, vbuffer12_2, vmask12, vpmask, NAp, NAp, globalparams[12]);
 // COMPUTEUNITS_seq
		#ifdef _DEBUGMODE_KERNELPRINTS2
		cout<<"start: processing instance 13... "<<endl;
		#endif
		dispatch(ON, OFF, OFF, kvdram13, vbuffer13_1, vbuffer13_2, vmask13, vpmask, NAp, NAp, globalparams[13]);
 // COMPUTEUNITS_seq
		#ifdef _DEBUGMODE_KERNELPRINTS2
		cout<<"start: processing instance 14... "<<endl;
		#endif
		dispatch(ON, OFF, OFF, kvdram14, vbuffer14_1, vbuffer14_2, vmask14, vpmask, NAp, NAp, globalparams[14]);
 // COMPUTEUNITS_seq
		#ifdef _DEBUGMODE_KERNELPRINTS2
		cout<<"start: processing instance 15... "<<endl;
		#endif
		dispatch(ON, OFF, OFF, kvdram15, vbuffer15_1, vbuffer15_2, vmask15, vpmask, NAp, NAp, globalparams[15]);
	}
	
	// partition 
	if(globalparams[0].partitioncommand == ON){ 
 // COMPUTEUNITS_seq
		#ifdef _DEBUGMODE_KERNELPRINTS2
		cout<<"start: partitioning instance 0... "<<endl;
		#endif
		dispatch(OFF, ON, OFF, kvdram0, vbuffer0_1, vbuffer0_2, vmask0, vpmask, NAp, NAp, globalparams[0]);
 // COMPUTEUNITS_seq
		#ifdef _DEBUGMODE_KERNELPRINTS2
		cout<<"start: partitioning instance 1... "<<endl;
		#endif
		dispatch(OFF, ON, OFF, kvdram1, vbuffer1_1, vbuffer1_2, vmask1, vpmask, NAp, NAp, globalparams[1]);
 // COMPUTEUNITS_seq
		#ifdef _DEBUGMODE_KERNELPRINTS2
		cout<<"start: partitioning instance 2... "<<endl;
		#endif
		dispatch(OFF, ON, OFF, kvdram2, vbuffer2_1, vbuffer2_2, vmask2, vpmask, NAp, NAp, globalparams[2]);
 // COMPUTEUNITS_seq
		#ifdef _DEBUGMODE_KERNELPRINTS2
		cout<<"start: partitioning instance 3... "<<endl;
		#endif
		dispatch(OFF, ON, OFF, kvdram3, vbuffer3_1, vbuffer3_2, vmask3, vpmask, NAp, NAp, globalparams[3]);
 // COMPUTEUNITS_seq
		#ifdef _DEBUGMODE_KERNELPRINTS2
		cout<<"start: partitioning instance 4... "<<endl;
		#endif
		dispatch(OFF, ON, OFF, kvdram4, vbuffer4_1, vbuffer4_2, vmask4, vpmask, NAp, NAp, globalparams[4]);
 // COMPUTEUNITS_seq
		#ifdef _DEBUGMODE_KERNELPRINTS2
		cout<<"start: partitioning instance 5... "<<endl;
		#endif
		dispatch(OFF, ON, OFF, kvdram5, vbuffer5_1, vbuffer5_2, vmask5, vpmask, NAp, NAp, globalparams[5]);
 // COMPUTEUNITS_seq
		#ifdef _DEBUGMODE_KERNELPRINTS2
		cout<<"start: partitioning instance 6... "<<endl;
		#endif
		dispatch(OFF, ON, OFF, kvdram6, vbuffer6_1, vbuffer6_2, vmask6, vpmask, NAp, NAp, globalparams[6]);
 // COMPUTEUNITS_seq
		#ifdef _DEBUGMODE_KERNELPRINTS2
		cout<<"start: partitioning instance 7... "<<endl;
		#endif
		dispatch(OFF, ON, OFF, kvdram7, vbuffer7_1, vbuffer7_2, vmask7, vpmask, NAp, NAp, globalparams[7]);
 // COMPUTEUNITS_seq
		#ifdef _DEBUGMODE_KERNELPRINTS2
		cout<<"start: partitioning instance 8... "<<endl;
		#endif
		dispatch(OFF, ON, OFF, kvdram8, vbuffer8_1, vbuffer8_2, vmask8, vpmask, NAp, NAp, globalparams[8]);
 // COMPUTEUNITS_seq
		#ifdef _DEBUGMODE_KERNELPRINTS2
		cout<<"start: partitioning instance 9... "<<endl;
		#endif
		dispatch(OFF, ON, OFF, kvdram9, vbuffer9_1, vbuffer9_2, vmask9, vpmask, NAp, NAp, globalparams[9]);
 // COMPUTEUNITS_seq
		#ifdef _DEBUGMODE_KERNELPRINTS2
		cout<<"start: partitioning instance 10... "<<endl;
		#endif
		dispatch(OFF, ON, OFF, kvdram10, vbuffer10_1, vbuffer10_2, vmask10, vpmask, NAp, NAp, globalparams[10]);
 // COMPUTEUNITS_seq
		#ifdef _DEBUGMODE_KERNELPRINTS2
		cout<<"start: partitioning instance 11... "<<endl;
		#endif
		dispatch(OFF, ON, OFF, kvdram11, vbuffer11_1, vbuffer11_2, vmask11, vpmask, NAp, NAp, globalparams[11]);
 // COMPUTEUNITS_seq
		#ifdef _DEBUGMODE_KERNELPRINTS2
		cout<<"start: partitioning instance 12... "<<endl;
		#endif
		dispatch(OFF, ON, OFF, kvdram12, vbuffer12_1, vbuffer12_2, vmask12, vpmask, NAp, NAp, globalparams[12]);
 // COMPUTEUNITS_seq
		#ifdef _DEBUGMODE_KERNELPRINTS2
		cout<<"start: partitioning instance 13... "<<endl;
		#endif
		dispatch(OFF, ON, OFF, kvdram13, vbuffer13_1, vbuffer13_2, vmask13, vpmask, NAp, NAp, globalparams[13]);
 // COMPUTEUNITS_seq
		#ifdef _DEBUGMODE_KERNELPRINTS2
		cout<<"start: partitioning instance 14... "<<endl;
		#endif
		dispatch(OFF, ON, OFF, kvdram14, vbuffer14_1, vbuffer14_2, vmask14, vpmask, NAp, NAp, globalparams[14]);
 // COMPUTEUNITS_seq
		#ifdef _DEBUGMODE_KERNELPRINTS2
		cout<<"start: partitioning instance 15... "<<endl;
		#endif
		dispatch(OFF, ON, OFF, kvdram15, vbuffer15_1, vbuffer15_2, vmask15, vpmask, NAp, NAp, globalparams[15]);
	}
	
	// reduce & partition
	if(globalparams[0].reducecommand == ON){
		#ifdef _DEBUGMODE_KERNELPRINTS2
		cout<<"start: reducing instances 0-16... "<<endl;
		#endif
		start_reduce(kvdram0, vbuffer0_1, vbuffer0_2, vmask0,kvdram1, vbuffer1_1, vbuffer1_2, vmask1,kvdram2, vbuffer2_1, vbuffer2_2, vmask2,kvdram3, vbuffer3_1, vbuffer3_2, vmask3,kvdram4, vbuffer4_1, vbuffer4_2, vmask4,kvdram5, vbuffer5_1, vbuffer5_2, vmask5,kvdram6, vbuffer6_1, vbuffer6_2, vmask6,kvdram7, vbuffer7_1, vbuffer7_2, vmask7,kvdram8, vbuffer8_1, vbuffer8_2, vmask8,kvdram9, vbuffer9_1, vbuffer9_2, vmask9,kvdram10, vbuffer10_1, vbuffer10_2, vmask10,kvdram11, vbuffer11_1, vbuffer11_2, vmask11,kvdram12, vbuffer12_1, vbuffer12_2, vmask12,kvdram13, vbuffer13_1, vbuffer13_2, vmask13,kvdram14, vbuffer14_1, vbuffer14_2, vmask14,kvdram15, vbuffer15_1, vbuffer15_2, vmask15, vpmask, globalparams);
	}
	
	#if defined(_DEBUGMODE_KERNELPRINTS3) && not defined (ALLVERTEXISACTIVE_ALGORITHM)
	cout<<"active partitions: ";
	for(unsigned int k=0; k<256; k++){ if(vpmask[k]==ON){ cout<<k<<", "; }}
	cout<<""<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->printglobalvars();
	#endif 
	#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(_DEBUGMODE_CHECKS2)
	actsutilityobj->clearglobalvars();
	#endif
	return;
}

// top
extern "C" {
void 
	#ifdef SW 
	acts:: 
	#endif
topkernel(uint512_dt * vdram ,uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11,uint512_dt * kvdram12,uint512_dt * kvdram13,uint512_dt * kvdram14,uint512_dt * kvdram15){ 
	
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
 
#pragma HLS INTERFACE m_axi port = kvdram12 offset = slave bundle = gmem13
 
#pragma HLS INTERFACE m_axi port = kvdram13 offset = slave bundle = gmem14
 
#pragma HLS INTERFACE m_axi port = kvdram14 offset = slave bundle = gmem15
 
#pragma HLS INTERFACE m_axi port = kvdram15 offset = slave bundle = gmem16
		
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
 
#pragma HLS INTERFACE s_axilite port = kvdram12 bundle = control
 
#pragma HLS INTERFACE s_axilite port = kvdram13 bundle = control
 
#pragma HLS INTERFACE s_axilite port = kvdram14 bundle = control
 
#pragma HLS INTERFACE s_axilite port = kvdram15 bundle = control

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
 
#pragma HLS DATA_PACK variable = kvdram12
 
#pragma HLS DATA_PACK variable = kvdram13
 
#pragma HLS DATA_PACK variable = kvdram14
 
#pragma HLS DATA_PACK variable = kvdram15

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
  
	cout<<">>> Light weight ACTS 12 (_L2) Launched... size: "<<(unsigned int)(kvdram12[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].range(31, 0))<<endl; 
  
	cout<<">>> Light weight ACTS 13 (_L2) Launched... size: "<<(unsigned int)(kvdram13[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].range(31, 0))<<endl; 
  
	cout<<">>> Light weight ACTS 14 (_L2) Launched... size: "<<(unsigned int)(kvdram14[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].range(31, 0))<<endl; 
  
	cout<<">>> Light weight ACTS 15 (_L2) Launched... size: "<<(unsigned int)(kvdram15[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].range(31, 0))<<endl; 
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
  
	cout<<">>> Light weight ACTS 12 (_L2) Launched... size: "<<kvdram12[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key<<endl; 
  
	cout<<">>> Light weight ACTS 13 (_L2) Launched... size: "<<kvdram13[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key<<endl; 
  
	cout<<">>> Light weight ACTS 14 (_L2) Launched... size: "<<kvdram14[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key<<endl; 
  
	cout<<">>> Light weight ACTS 15 (_L2) Launched... size: "<<kvdram15[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key<<endl; 
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
	keyvalue_t buffer112[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer112
	keyvalue_t buffer212[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer212
	keyvalue_t buffer312[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer312
	keyvalue_t buffer412[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer412
	keyvalue_t buffer512[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer512
	keyvalue_t unused_buffer612[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer612
	keyvalue_t unused_buffer712[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer712
	keyvalue_t buffer113[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer113
	keyvalue_t buffer213[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer213
	keyvalue_t buffer313[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer313
	keyvalue_t buffer413[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer413
	keyvalue_t buffer513[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer513
	keyvalue_t unused_buffer613[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer613
	keyvalue_t unused_buffer713[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer713
	keyvalue_t buffer114[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer114
	keyvalue_t buffer214[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer214
	keyvalue_t buffer314[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer314
	keyvalue_t buffer414[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer414
	keyvalue_t buffer514[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer514
	keyvalue_t unused_buffer614[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer614
	keyvalue_t unused_buffer714[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer714
	keyvalue_t buffer115[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer115
	keyvalue_t buffer215[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer215
	keyvalue_t buffer315[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer315
	keyvalue_t buffer415[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer415
	keyvalue_t buffer515[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer515
	keyvalue_t unused_buffer615[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer615
	keyvalue_t unused_buffer715[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = unused_buffer715
	
	globalparams_t globalparams[NUMCOMPUTEUNITS];
	#pragma HLS ARRAY_PARTITION variable=globalparams complete
 
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
 
	globalparams[12] = getglobalparams(kvdram12);
 
	globalparams[13] = getglobalparams(kvdram13);
 
	globalparams[14] = getglobalparams(kvdram14);
 
	globalparams[15] = getglobalparams(kvdram15);
	
	bool_type vpmask[BLOCKRAM_SIZE];
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ vpmask[k] = OFF; }
	vpmask[0] = ON; // just for test. assuming rootvid=1
	
	unsigned int numGraphIters = globalparams[0].GraphIter;
	unsigned int numactvvs = 1;
	
	#ifdef _DEBUGMODE_STATS
	stats_greaterthan512 = 0;
	stats_lessthan512 = 0;
	stats_totals = 0;
	#endif 
	
	RUNITERATIONS_LOOP: for(unsigned int GraphIter=0; GraphIter<numGraphIters; GraphIter++){
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<">>> Light weight ACTS: Graph Iteration: "<<GraphIter<<": ("<<numactvvs<<" active vertices)"<<endl;
		#endif
		
		for(unsigned int i=0; i<NUMCOMPUTEUNITS; i++){ globalparams[i].GraphIter = GraphIter; }
		start(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9,kvdram10,kvdram11,kvdram12,kvdram13,kvdram14,kvdram15, vpmask, globalparams);

		#ifdef _DEBUGMODE_STATS
		numactvvs = actsutilityobj->globalstats_getactvvsseen();
		cout<<"num active vertices for iteration "<<GraphIter+1<<": "<<numactvvs<<endl;
		actsutilityobj->globalstats_setactvvsseen(0);
		if(numactvvs == 0){ break; }
		#endif 
	}
	
	#ifdef _DEBUGMODE_STATSX
	cout<<"topkernel:: stats_greaterthan512: "<<stats_greaterthan512<<endl;
	cout<<"topkernel:: stats_lessthan512: "<<stats_lessthan512<<endl;
	cout<<"topkernel:: stats_totals: "<<stats_totals<<endl;
	cout<<"topkernel:: total: "<<(stats_lessthan512 + stats_greaterthan512)<<endl;
	#endif
	return;
}
}




