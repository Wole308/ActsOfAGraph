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
#include "actsproc.h"
using namespace std;

#define NUMPIPELINES_PARTITIONUPDATES 2 
#if NUMPIPELINES_PARTITIONUPDATES==1
#define PUP0
#endif 
#if NUMPIPELINES_PARTITIONUPDATES==2
#define PUP0
#define PUP1
#endif

#ifdef SW
actsproc::actsproc(){ actsutilityobj = new actsutility(); }
actsproc::~actsproc(){}
#endif
#ifdef SWEMU
actsutility * actsutilityobj = new actsutility();
#endif

// functions (basic)
unsigned int
	#ifdef SW 
	actsproc::
	#endif 
amin(unsigned int val1, unsigned int val2){
	if(val1 < val2){ return val1; }
	else { return val2; }
}
unsigned int
	#ifdef SW 
	actsproc::
	#endif 
aplus(unsigned int val1, unsigned int val2){
	return val1 + val2;
}

// functions (allignment)
batch_type
	#ifdef SW 
	actsproc::
	#endif
allignlower_KV(batch_type val){
	batch_type fac = val / VECTOR_SIZE;
	return (fac * VECTOR_SIZE);
}
batch_type
	#ifdef SW 
	actsproc::
	#endif 
allignhigher_KV(batch_type val){
	batch_type fac = (val + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	return (fac * VECTOR_SIZE);
}

// functions (bit manipulation)
unsigned int 
	#ifdef SW 
	actsproc::
	#endif
GETMASK_UINT(unsigned int index, unsigned int size){
	unsigned int A = ((1 << (size)) - 1);
	unsigned int B = A << index;
	return B;
}
unsigned int 
	#ifdef SW 
	actsproc::
	#endif
READFROM_UINT(unsigned int data, unsigned int index, unsigned int size){ 
	return (((data) & GETMASK_UINT((index), (size))) >> (index)); 
}
void
	#ifdef SW 
	actsproc::
	#endif
WRITETO_UINT(unsigned int * data, unsigned int index, unsigned int size, unsigned int value){
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

// functions (converters)
keyvalue_t 
	#ifdef SW 
	actsproc::
	#endif 
GETKV(keyvalue_buffer_t data){
	#pragma HLS INLINE
	keyvalue_t res;
	#ifdef _WIDEWORD
	res.key = data.range(SIZEOF_KEY - 1, 0);
	res.value = data.range(SIZEOF_KEY + SIZEOF_VALUE - 1, SIZEOF_KEY);
	#else 
	res.key = data.key;
	res.value = data.value;	
	#endif 
	return res;
}
keyvalue_buffer_t 
	#ifdef SW 
	actsproc::
	#endif 
GETKV(keyvalue_t data){
	#pragma HLS INLINE
	keyvalue_buffer_t res;
	#ifdef _WIDEWORD
	res.range(SIZEOF_KEY - 1, 0) = data.key;
	res.range(SIZEOF_KEY + SIZEOF_VALUE - 1, SIZEOF_KEY) = data.value;
	#else
	res.key = data.key;
	res.value = data.value;	
	#endif 
	return res;
}
keyy_t 
	#ifdef SW 
	actsproc::
	#endif 
GETK(uint32_type data){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	return data.range(SIZEOF_KEY - 1, 0);
	#else
	return data;
	#endif
}
value_t 
	#ifdef SW 
	actsproc::
	#endif 
GETV(uint32_type data){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	return data.range(SIZEOF_VALUE - 1, 0);
	#else
	return data;
	#endif
}
keyvalue_t 
	#ifdef SW 
	actsproc::
	#endif 
GETKV2(keyvalue_vbuffer_t data){
	#pragma HLS INLINE
	keyvalue_t res;
	#ifdef _WIDEWORD
	res.key = data.range(SIZEOF_VDATAKEY - 1, 0); 
	res.value = data.range(SIZEOF_VDATAKEY + SIZEOF_VDATAVALUE - 1, SIZEOF_VDATAKEY);
	#else 
	res.key = data.key;
	res.value = data.value;	
	#endif 
	return res;
}
keyvalue_vbuffer_t 
	#ifdef SW 
	actsproc::
	#endif 
GETKV2(keyvalue_t data){
	#pragma HLS INLINE
	keyvalue_vbuffer_t res;
	#ifdef _WIDEWORD
	res.range(SIZEOF_VDATAKEY - 1, 0) = data.key; 
	res.range(SIZEOF_VDATAKEY + SIZEOF_VDATAVALUE - 1, SIZEOF_VDATAKEY) = data.value;
	#else
	res.key = data.key;
	res.value = data.value;
	#endif 
	return res;
}
keyy_t 
	#ifdef SW 
	actsproc::
	#endif 
GETK2(uint32_type data){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	return data.range(SIZEOF_VDATAKEY - 1, 0);
	#else
	return data;
	#endif
}
value_t 
	#ifdef SW 
	actsproc::
	#endif 
GETV2(uint32_type data){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	return data.range(SIZEOF_VDATAKEY - 1, 0);
	#else
	return data;
	#endif
}
keyy_t 
	#ifdef SW 
	actsproc::
	#endif 
GETKEYENTRY(uint512_dt data, unsigned int v){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	return data.range(32 * ((v * 2) + 1) - 1, (v * 2) * 32);
	#else 
	return data.data[v].key;	
	#endif
}
value_t 
	#ifdef SW 
	actsproc::
	#endif 
GETVTXDATA(keyvalue_vbuffer_t vbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unsigned int loc, globalparams_t globalparams){
	#pragma HLS INLINE
	value_t data = 0;
	
	#ifdef ACTSPROC_AUTOMATE_ACROSSDATASETS
	unsigned int col = loc >> globalparams.POW_REDUCE;
	unsigned int row;
	if(globalparams.POW_REDUCE == 11){
		row = loc % (1 << 11);
	} else if(globalparams.POW_REDUCE == 10){
		row = loc % (1 << 10);
	} else if(globalparams.POW_REDUCE == 9){
		row = loc % (1 << 9);
	} else if(globalparams.POW_REDUCE == 8){
		row = loc % (1 << 8);
	} else if(globalparams.POW_REDUCE == 7){
		row = loc % (1 << 7);
	} else {
		row = 0;
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"GETVTXDATA: ERROR: out of selection. globalparams.POW_REDUCE: "<<globalparams.POW_REDUCE<<". exiting..."<<endl;
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
	actsutilityobj->checkoutofbounds("GETVTXDATA.col", col, NUM_PARTITIONS, loc, NAp, NAp); 
	actsutilityobj->checkoutofbounds("GETVTXDATA.row", row, BLOCKRAM_SIZE, loc, NAp, NAp);
	#endif 
	
	if(loc % 2 == 0){ data = GETKV2(vbuffer[col][row]).key; } 
	else { data = GETKV2(vbuffer[col][row]).value; }
	return data;
}
value_t 
	#ifdef SW 
	actsproc::
	#endif 
GETVTXMASK(uintNUMPby2_type vmask[BLOCKRAM_SIZE], unsigned int loc, globalparams_t globalparams){
	#pragma HLS INLINE
	value_t data = 0;
	
	#ifdef ACTSPROC_AUTOMATE_ACROSSDATASETS
	unsigned int col = loc >> globalparams.POW_REDUCE;
	unsigned int row;
	if(globalparams.POW_REDUCE == 11){ 
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
		cout<<"GETVTXMASK: ERROR: out of selection. globalparams.POW_REDUCE: "<<globalparams.POW_REDUCE<<". exiting..."<<endl;
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
	actsutilityobj->checkoutofbounds("GETVTXMASK.col", col, NUM_PARTITIONS, loc, NAp, NAp);
	actsutilityobj->checkoutofbounds("GETVTXMASK.row", row, BLOCKRAM_SIZE, loc, NAp, NAp);
	#endif 
	
	if(loc % 2 == 0){ data = vmask[row].data[col].key; } 
	else { data = vmask[row].data[col].value; }
	return data;
}
value_t 
	#ifdef SW 
	actsproc::
	#endif 
GETVTXMASK_SUBP(uintNUMPby2_type vmask[BLOCKRAM_SIZE], unsigned int loc, globalparams_t globalparams){
	#pragma HLS INLINE
	value_t data = 0;

	#ifdef ACTSPROC_AUTOMATE_ACROSSDATASETS
	unsigned int col = loc >> (globalparams.POW_REDUCE - SUBPMASKFACTOR_POW);
	unsigned int row;
	if(globalparams.POW_REDUCE == 11){ 
		row = loc % (1 << (11-SUBPMASKFACTOR_POW));
	} else if(globalparams.POW_REDUCE == 10){
		row = loc % (1 << (10-SUBPMASKFACTOR_POW));
	} else if(globalparams.POW_REDUCE == 9){
		row = loc % (1 << (9-SUBPMASKFACTOR_POW));
	} else if(globalparams.POW_REDUCE == 8){
		row = loc % (1 << (8-SUBPMASKFACTOR_POW));
	} else {
		row = 0;
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"GETVTXMASK_SUBP: ERROR: out of selection. globalparams.POW_REDUCE: "<<globalparams.POW_REDUCE<<". exiting..."<<endl;
		exit(EXIT_FAILURE);
		#endif 
	}
	row = row / 2;
	#else 
	unsigned int col = loc / (REDUCESZ/SUBPMASKFACTOR); 
	unsigned int row = loc % (REDUCESZ/SUBPMASKFACTOR);
	row = row / 2;
	#endif
	
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("GETVTXMASK_SUBP.col", col, NUM_PARTITIONS, loc, NAp, NAp);
	actsutilityobj->checkoutofbounds("GETVTXMASK_SUBP.row", row, BLOCKRAM_SIZE, loc, NAp, NAp);
	#endif 
	
	if(loc % 2 == 0){ data = vmask[row].data[col].key; } 
	else { data = vmask[row].data[col].value; }

	return data;
}

// functions (actsproc utilities)
batch_type
	#ifdef SW 
	actsproc::
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
	actsproc::
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
	actsproc::
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
	actsproc::
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
	actsproc::
	#endif 
calculateoffsets(keyvalue_capsule_t * buffer, buffer_type size){
	unsigned int analysis_size = NUM_PARTITIONS;
	for(buffer_type i=1; i<size; i++){ 
	#pragma HLS PIPELINE II=2
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_size avg=analysis_size	
		buffer[i].key = allignhigher_KV(buffer[i-1].key + buffer[i-1].value); 
	}
	return;
}
void 
	#ifdef SW 
	actsproc::
	#endif 
calculatemanyunallignedoffsets(keyvalue_capsule_t buffer[VECTOR_SIZE][NUM_PARTITIONS], buffer_type size, batch_type base, batch_type skipspacing){
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
	actsproc::
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
	actsproc::
	#endif 
getglobalparams(uint512_dt * kvdram){
	globalparams_t globalparams;
	#ifdef _WIDEWORD
	globalparams.ENABLE_RUNKERNELCOMMAND = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_RUNKERNELCOMMAND].range(31, 0);
	globalparams.ENABLE_PROCESSCOMMAND = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_PROCESSCOMMAND].range(31, 0);
	globalparams.ENABLE_PARTITIONCOMMAND = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_PARTITIONCOMMAND].range(31, 0);
	globalparams.ENABLE_APPLYUPDATESCOMMAND = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_APPLYUPDATESCOMMAND].range(31, 0);
	globalparams.ENABLE_SAVEVMASK = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_SAVEVMASK].range(31, 0);
	globalparams.ENABLE_SAVEVMASKP = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_SAVEVMASKP].range(31, 0);
	
	globalparams.BASEOFFSETKVS_MESSAGESDATA = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_MESSAGESDATA].range(31, 0);
	globalparams.BASEOFFSETKVS_EDGESDATA = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_EDGESDATA].range(31, 0);
	globalparams.BASEOFFSETKVS_VERTEXPTR = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTEXPTR].range(31, 0);
	globalparams.BASEOFFSETKVS_VERTICESDATA = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESDATA].range(31, 0);
	globalparams.BASEOFFSETKVS_ACTIVEVERTICES = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_ACTIVEVERTICES].range(31, 0);
	globalparams.BASEOFFSETKVS_VERTICESDATAMASK = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESDATAMASK].range(31, 0);
	globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESPARTITIONMASK].range(31, 0);
	globalparams.BASEOFFSETKVS_STATSDRAM = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_STATSDRAM].range(31, 0);
	globalparams.BASEOFFSETKVS_KVDRAM = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_KVDRAM].range(31, 0);
	globalparams.BASEOFFSETKVS_KVDRAMWORKSPACE = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_KVDRAMWORKSPACE].range(31, 0);
	
	globalparams.SIZE_MESSAGESDRAM = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_MESSAGESDATA].range(31, 0);
	globalparams.SIZE_EDGES = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_EDGES].range(31, 0);
	globalparams.SIZE_VERTEXPTRS = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_VERTEXPTRS].range(31, 0);
	globalparams.SIZE_VERTICESDATA = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_VERTICESDATA].range(31, 0);
	globalparams.SIZE_ACTIVEVERTICES = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_ACTIVEVERTICES].range(31, 0);
	globalparams.SIZE_VERTICESDATAMASK = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_VERTICESDATAMASK].range(31, 0);
	globalparams.SIZE_VERTICESPARTITIONMASK = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_VERTICESPARTITIONMASK].range(31, 0);
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
	globalparams.ENABLE_SAVEVMASK = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_SAVEVMASK].data[0].key;
	globalparams.ENABLE_SAVEVMASKP = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_SAVEVMASKP].data[0].key;

	globalparams.BASEOFFSETKVS_MESSAGESDATA = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_MESSAGESDATA].data[0].key;
	globalparams.BASEOFFSETKVS_EDGESDATA = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_EDGESDATA].data[0].key;
	globalparams.BASEOFFSETKVS_VERTEXPTR = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTEXPTR].data[0].key;
	globalparams.BASEOFFSETKVS_VERTICESDATA = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESDATA].data[0].key;
	globalparams.BASEOFFSETKVS_ACTIVEVERTICES = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_ACTIVEVERTICES].data[0].key;
	globalparams.BASEOFFSETKVS_VERTICESDATAMASK = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESDATAMASK].data[0].key;
	globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESPARTITIONMASK].data[0].key;
	globalparams.BASEOFFSETKVS_STATSDRAM = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_STATSDRAM].data[0].key;
	globalparams.BASEOFFSETKVS_KVDRAM = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_KVDRAM].data[0].key;
	globalparams.BASEOFFSETKVS_KVDRAMWORKSPACE = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_KVDRAMWORKSPACE].data[0].key;
	
	globalparams.SIZE_MESSAGESDRAM = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_MESSAGESDATA].data[0].key;
	globalparams.SIZE_EDGES = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_EDGES].data[0].key;
	globalparams.SIZE_VERTEXPTRS = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_VERTEXPTRS].data[0].key;
	globalparams.SIZE_VERTICESDATA = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_VERTICESDATA].data[0].key;
	globalparams.SIZE_ACTIVEVERTICES = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_ACTIVEVERTICES].data[0].key;
	globalparams.SIZE_VERTICESDATAMASK = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_VERTICESDATAMASK].data[0].key;
	globalparams.SIZE_VERTICESPARTITIONMASK = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_VERTICESPARTITIONMASK].data[0].key;
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
	actsutilityobj->printglobalparameters("actsproc::getglobalparams:: printing global parameters", globalparams);
	#endif
	return globalparams;
}
sweepparams_t 
	#ifdef SW 
	actsproc::
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
	actsproc::
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
		keyvalue.key = kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker].data[0].key; 
		keyvalue.value = kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker].data[0].value; 
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
	actsproc::
	#endif 
getpartition(bool_type enable, keyvalue_buffer_t keyvalue, step_type currentLOP, vertex_t upperlimit, unsigned int upperpartition, unsigned int batch_range_pow){
	partition_type partition;
	keyvalue_t thiskeyvalue = GETKV(keyvalue);
	
	if(thiskeyvalue.value == GETV(INVALIDDATA)){ partition = thiskeyvalue.key; } 
	else { partition = ((thiskeyvalue.key - upperlimit) >> (batch_range_pow - (NUM_PARTITIONS_POW * currentLOP))); }
	
	#ifdef _DEBUGMODE_CHECKS2
	if(partition >= NUM_PARTITIONS){ actsutilityobj->globalstats_counterrorsingetpartition(1); }
	#endif 
	
	#ifdef _DEBUGMODE_CHECKS2
	if(partition >= NUM_PARTITIONS){ 
		#ifdef ENABLE_VOICEOUTKERNELERRORS
		cout<<"actsproc::getpartition::ERROR 1. partition out of bounds partition: "<<partition<<", thiskeyvalue.key: "<<thiskeyvalue.key<<", thiskeyvalue.value: "<<thiskeyvalue.value<<", NUM_PARTITIONS: "<<NUM_PARTITIONS<<", upperlimit: "<<upperlimit<<", currentLOP: "<<currentLOP<<", batch_range_pow: "<<batch_range_pow<<", div factor: "<<(1 << (batch_range_pow - (NUM_PARTITIONS_POW * currentLOP)))<<endl; 
		#endif 
		#ifdef ENABLE_PERFECTACCURACY
		exit(EXIT_FAILURE); 
		#endif 
	}
	#endif
	
	#ifndef ENABLE_PERFECTACCURACY
		if(partition >= NUM_PARTITIONS){ partition = (((1 << NUM_PARTITIONS_POW) - 1) & (partition >> (1 - 1))); } // FIXME. REMOVEME. PERFECTIONTEST.
	#endif
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("actsproc::getpartition 2", partition, NUM_PARTITIONS, thiskeyvalue.key, upperlimit, currentLOP);
	#endif
	return partition;
}

// functions (resets)
void 
	#ifdef SW 
	actsproc::
	#endif 
resetvalues(keyvalue_t * buffer, buffer_type size, unsigned int resetval){
	for(buffer_type i=0; i<size; i++){ 
	#pragma HLS PIPELINE II=1
		buffer[i].value = resetval; 
	}
}
void 
	#ifdef SW 
	actsproc::
	#endif 
resetvalues(keyvalue_capsule_t * buffer, buffer_type size, unsigned int resetval){
	for(buffer_type i=0; i<size; i++){ 
	#pragma HLS PIPELINE II=1
		buffer[i].value = resetval; 
	}
}
void 
	#ifdef SW 
	actsproc::
	#endif 
resetvalues(value_t * buffer, buffer_type size, unsigned int resetval){
	for(buffer_type i=0; i<size; i++){ 
	#pragma HLS PIPELINE II=1
		buffer[i] = resetval; 
	}
}
void 
	#ifdef SW 
	actsproc::
	#endif 
resetkeysandvalues(keyvalue_t * buffer, buffer_type size, unsigned int resetval){
	for(buffer_type i=0; i<size; i++){
		buffer[i].key = resetval; 
		buffer[i].value = resetval; 
	}
	return;
}
void
	#ifdef SW 
	actsproc::
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
fetchmessage_t //
	#ifdef SW 
	actsproc::
	#endif 
readkeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_buffer_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs, travstate_t travstate, globalparams_t globalparams){
	fetchmessage_t fetchmessage;
	fetchmessage.chunksize_kvs = -1;
	fetchmessage.nextoffset_kvs = -1;
	if(enable == OFF){ return fetchmessage; }
	
	analysis_type analysis_loopcount = BLOCKRAM_SIZE;
	buffer_type chunk_size = getchunksize_kvs(size_kvs, travstate, 0);
	
	fetchmessage.chunksize_kvs = chunk_size;
	fetchmessage.nextoffset_kvs = NAp;
	
	READKEYVALUES1_LOOP: for (buffer_type i=0; i<chunk_size; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE II=1
		keyvalue_t mykeyvalue0;
		keyvalue_t mykeyvalue1;
		keyvalue_t mykeyvalue2;
		keyvalue_t mykeyvalue3;
		keyvalue_t mykeyvalue4;
		keyvalue_t mykeyvalue5;
		keyvalue_t mykeyvalue6;
		keyvalue_t mykeyvalue7;
		
		#ifdef _WIDEWORD
		mykeyvalue0.key = kvdram[offset_kvs + i].range(31, 0);
		mykeyvalue0.value = kvdram[offset_kvs + i].range(63, 32);
		mykeyvalue1.key = kvdram[offset_kvs + i].range(95, 64);
		mykeyvalue1.value = kvdram[offset_kvs + i].range(127, 96);
		mykeyvalue2.key = kvdram[offset_kvs + i].range(159, 128);
		mykeyvalue2.value = kvdram[offset_kvs + i].range(191, 160);
		mykeyvalue3.key = kvdram[offset_kvs + i].range(223, 192);
		mykeyvalue3.value = kvdram[offset_kvs + i].range(255, 224);
		mykeyvalue4.key = kvdram[offset_kvs + i].range(287, 256);
		mykeyvalue4.value = kvdram[offset_kvs + i].range(319, 288);
		mykeyvalue5.key = kvdram[offset_kvs + i].range(351, 320);
		mykeyvalue5.value = kvdram[offset_kvs + i].range(383, 352);
		mykeyvalue6.key = kvdram[offset_kvs + i].range(415, 384);
		mykeyvalue6.value = kvdram[offset_kvs + i].range(447, 416);
		mykeyvalue7.key = kvdram[offset_kvs + i].range(479, 448);
		mykeyvalue7.value = kvdram[offset_kvs + i].range(511, 480);
		#else 
		mykeyvalue0.key = kvdram[offset_kvs + i].data[0].key; 
		mykeyvalue0.value = kvdram[offset_kvs + i].data[0].value; 
		mykeyvalue1.key = kvdram[offset_kvs + i].data[1].key; 
		mykeyvalue1.value = kvdram[offset_kvs + i].data[1].value; 
		mykeyvalue2.key = kvdram[offset_kvs + i].data[2].key; 
		mykeyvalue2.value = kvdram[offset_kvs + i].data[2].value; 
		mykeyvalue3.key = kvdram[offset_kvs + i].data[3].key; 
		mykeyvalue3.value = kvdram[offset_kvs + i].data[3].value; 
		mykeyvalue4.key = kvdram[offset_kvs + i].data[4].key; 
		mykeyvalue4.value = kvdram[offset_kvs + i].data[4].value; 
		mykeyvalue5.key = kvdram[offset_kvs + i].data[5].key; 
		mykeyvalue5.value = kvdram[offset_kvs + i].data[5].value; 
		mykeyvalue6.key = kvdram[offset_kvs + i].data[6].key; 
		mykeyvalue6.value = kvdram[offset_kvs + i].data[6].value; 
		mykeyvalue7.key = kvdram[offset_kvs + i].data[7].key; 
		mykeyvalue7.value = kvdram[offset_kvs + i].data[7].value; 
		#endif 
		
		buffer[0][i] = GETKV(mykeyvalue0);
		buffer[1][i] = GETKV(mykeyvalue1);
		buffer[2][i] = GETKV(mykeyvalue2);
		buffer[3][i] = GETKV(mykeyvalue3);
		buffer[4][i] = GETKV(mykeyvalue4);
		buffer[5][i] = GETKV(mykeyvalue5);
		buffer[6][i] = GETKV(mykeyvalue6);
		buffer[7][i] = GETKV(mykeyvalue7);
		
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"readkeyvalues:: keyvalues read: offset_kvs: "<<offset_kvs<<", size_kvs: "<<size_kvs<<", chunk_size: "<<chunk_size<<endl;
	#endif
	return fetchmessage;
}

void 
	#ifdef SW 
	actsproc::
	#endif 
savekeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_buffer_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t * globalcapsule, keyvalue_capsule_t localcapsule[NUM_PARTITIONS], batch_type globalbaseaddress_kvs, globalparams_t globalparams){				
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
		actsutilityobj->checkoutofbounds("savekeyvalues 25", ((globalcapsule[p].key + globalcapsule[p].value) / VECTOR_SIZE), globalparams.SIZE_KVDRAM + 1, p, NAp, NAp);
		#endif
		if(realsize_kvs > 0){
			SAVEPARTITIONS_LOOP1B: for(buffer_type i=0; i<size_kvs; i++){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_destpartitionsz avg=analysis_destpartitionsz
			#pragma HLS PIPELINE II=1
				keyvalue_t mykeyvalue0 = GETKV(buffer[0][bramoffset_kvs + i]);
				keyvalue_t mykeyvalue1 = GETKV(buffer[1][bramoffset_kvs + i]);
				keyvalue_t mykeyvalue2 = GETKV(buffer[2][bramoffset_kvs + i]);
				keyvalue_t mykeyvalue3 = GETKV(buffer[3][bramoffset_kvs + i]);
				keyvalue_t mykeyvalue4 = GETKV(buffer[4][bramoffset_kvs + i]);
				keyvalue_t mykeyvalue5 = GETKV(buffer[5][bramoffset_kvs + i]);
				keyvalue_t mykeyvalue6 = GETKV(buffer[6][bramoffset_kvs + i]);
				keyvalue_t mykeyvalue7 = GETKV(buffer[7][bramoffset_kvs + i]);
				
				#ifdef _WIDEWORD
				kvdram[dramoffset_kvs + i].range(31, 0) = mykeyvalue0.key; 
				kvdram[dramoffset_kvs + i].range(63, 32) = mykeyvalue0.value; 
				kvdram[dramoffset_kvs + i].range(95, 64) = mykeyvalue1.key; 
				kvdram[dramoffset_kvs + i].range(127, 96) = mykeyvalue1.value; 
				kvdram[dramoffset_kvs + i].range(159, 128) = mykeyvalue2.key; 
				kvdram[dramoffset_kvs + i].range(191, 160) = mykeyvalue2.value; 
				kvdram[dramoffset_kvs + i].range(223, 192) = mykeyvalue3.key; 
				kvdram[dramoffset_kvs + i].range(255, 224) = mykeyvalue3.value; 
				kvdram[dramoffset_kvs + i].range(287, 256) = mykeyvalue4.key; 
				kvdram[dramoffset_kvs + i].range(319, 288) = mykeyvalue4.value; 
				kvdram[dramoffset_kvs + i].range(351, 320) = mykeyvalue5.key; 
				kvdram[dramoffset_kvs + i].range(383, 352) = mykeyvalue5.value; 
				kvdram[dramoffset_kvs + i].range(415, 384) = mykeyvalue6.key; 
				kvdram[dramoffset_kvs + i].range(447, 416) = mykeyvalue6.value; 
				kvdram[dramoffset_kvs + i].range(479, 448) = mykeyvalue7.key; 
				kvdram[dramoffset_kvs + i].range(511, 480) = mykeyvalue7.value; 
				#else 
				kvdram[dramoffset_kvs + i].data[0].key = mykeyvalue0.key; 
				kvdram[dramoffset_kvs + i].data[0].value = mykeyvalue0.value; 
				kvdram[dramoffset_kvs + i].data[1].key = mykeyvalue1.key; 
				kvdram[dramoffset_kvs + i].data[1].value = mykeyvalue1.value; 
				kvdram[dramoffset_kvs + i].data[2].key = mykeyvalue2.key; 
				kvdram[dramoffset_kvs + i].data[2].value = mykeyvalue2.value; 
				kvdram[dramoffset_kvs + i].data[3].key = mykeyvalue3.key; 
				kvdram[dramoffset_kvs + i].data[3].value = mykeyvalue3.value; 
				kvdram[dramoffset_kvs + i].data[4].key = mykeyvalue4.key; 
				kvdram[dramoffset_kvs + i].data[4].value = mykeyvalue4.value; 
				kvdram[dramoffset_kvs + i].data[5].key = mykeyvalue5.key; 
				kvdram[dramoffset_kvs + i].data[5].value = mykeyvalue5.value; 
				kvdram[dramoffset_kvs + i].data[6].key = mykeyvalue6.key; 
				kvdram[dramoffset_kvs + i].data[6].value = mykeyvalue6.value; 
				kvdram[dramoffset_kvs + i].data[7].key = mykeyvalue7.key; 
				kvdram[dramoffset_kvs + i].data[7].value = mykeyvalue7.value; 
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
			actsutilityobj->printkeyvalues("savekeyvalues::globalcapsule 34", (keyvalue_t *)globalcapsule, NUM_PARTITIONS); 
			exit(EXIT_FAILURE); 
		}
	}
	if(globalcapsule[NUM_PARTITIONS-1].key + globalcapsule[NUM_PARTITIONS-1].value >= globalparams.SIZE_KVDRAM){
		cout<<"savekeyvalues::globalcapsule 36. ERROR. out of bounds. (globalcapsule["<<NUM_PARTITIONS-1<<"].key("<<globalcapsule[NUM_PARTITIONS-1].key<<") + globalcapsule["<<NUM_PARTITIONS-1<<"].value("<<globalcapsule[NUM_PARTITIONS-1].value<<") >= globalparams.SIZE_KVDRAM("<<globalparams.SIZE_KVDRAM<<")) printing and exiting..."<<endl; 
		actsutilityobj->printkeyvalues("savekeyvalues::globalcapsule 37", (keyvalue_t *)globalcapsule, NUM_PARTITIONS); 
		exit(EXIT_FAILURE); 
	}
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"savekeyvalues:: keyvalues saved: offset_kvs from: "<<globalbaseaddress_kvs + ((globalcapsule[0].key + globalcapsule[0].value) / VECTOR_SIZE)<<endl;
	actsutilityobj->printkeyvalues("actsutility::savekeyvalues: globalcapsule.", (keyvalue_t *)globalcapsule, NUM_PARTITIONS);
	#endif
	return;
}

void 
	#ifdef SW 
	actsproc::
	#endif 
readkeyvalues(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_buffer_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount = BLOCKRAM_SIZE;
		
	READKEYVALUES2_LOOP: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE II=1
		keyvalue_t mykeyvalue0;
		keyvalue_t mykeyvalue1;
		keyvalue_t mykeyvalue2;
		keyvalue_t mykeyvalue3;
		keyvalue_t mykeyvalue4;
		keyvalue_t mykeyvalue5;
		keyvalue_t mykeyvalue6;
		keyvalue_t mykeyvalue7;
		
		#ifdef _WIDEWORD
		mykeyvalue0.key = kvdram[dramoffset_kvs + i].range(31, 0); 
		mykeyvalue0.value = kvdram[dramoffset_kvs + i].range(63, 32); 
		mykeyvalue1.key = kvdram[dramoffset_kvs + i].range(95, 64); 
		mykeyvalue1.value = kvdram[dramoffset_kvs + i].range(127, 96); 
		mykeyvalue2.key = kvdram[dramoffset_kvs + i].range(159, 128); 
		mykeyvalue2.value = kvdram[dramoffset_kvs + i].range(191, 160); 
		mykeyvalue3.key = kvdram[dramoffset_kvs + i].range(223, 192); 
		mykeyvalue3.value = kvdram[dramoffset_kvs + i].range(255, 224); 
		mykeyvalue4.key = kvdram[dramoffset_kvs + i].range(287, 256); 
		mykeyvalue4.value = kvdram[dramoffset_kvs + i].range(319, 288); 
		mykeyvalue5.key = kvdram[dramoffset_kvs + i].range(351, 320); 
		mykeyvalue5.value = kvdram[dramoffset_kvs + i].range(383, 352); 
		mykeyvalue6.key = kvdram[dramoffset_kvs + i].range(415, 384); 
		mykeyvalue6.value = kvdram[dramoffset_kvs + i].range(447, 416); 
		mykeyvalue7.key = kvdram[dramoffset_kvs + i].range(479, 448); 
		mykeyvalue7.value = kvdram[dramoffset_kvs + i].range(511, 480); 
		#else 
		mykeyvalue0.key = kvdram[dramoffset_kvs + i].data[0].key;
		mykeyvalue0.value = kvdram[dramoffset_kvs + i].data[0].value; 
		mykeyvalue1.key = kvdram[dramoffset_kvs + i].data[1].key;
		mykeyvalue1.value = kvdram[dramoffset_kvs + i].data[1].value; 
		mykeyvalue2.key = kvdram[dramoffset_kvs + i].data[2].key;
		mykeyvalue2.value = kvdram[dramoffset_kvs + i].data[2].value; 
		mykeyvalue3.key = kvdram[dramoffset_kvs + i].data[3].key;
		mykeyvalue3.value = kvdram[dramoffset_kvs + i].data[3].value; 
		mykeyvalue4.key = kvdram[dramoffset_kvs + i].data[4].key;
		mykeyvalue4.value = kvdram[dramoffset_kvs + i].data[4].value; 
		mykeyvalue5.key = kvdram[dramoffset_kvs + i].data[5].key;
		mykeyvalue5.value = kvdram[dramoffset_kvs + i].data[5].value; 
		mykeyvalue6.key = kvdram[dramoffset_kvs + i].data[6].key;
		mykeyvalue6.value = kvdram[dramoffset_kvs + i].data[6].value; 
		mykeyvalue7.key = kvdram[dramoffset_kvs + i].data[7].key;
		mykeyvalue7.value = kvdram[dramoffset_kvs + i].data[7].value; 
		
		buffer[0][bufferoffset_kvs + i] = GETKV(mykeyvalue0);
		buffer[1][bufferoffset_kvs + i] = GETKV(mykeyvalue1);
		buffer[2][bufferoffset_kvs + i] = GETKV(mykeyvalue2);
		buffer[3][bufferoffset_kvs + i] = GETKV(mykeyvalue3);
		buffer[4][bufferoffset_kvs + i] = GETKV(mykeyvalue4);
		buffer[5][bufferoffset_kvs + i] = GETKV(mykeyvalue5);
		buffer[6][bufferoffset_kvs + i] = GETKV(mykeyvalue6);
		buffer[7][bufferoffset_kvs + i] = GETKV(mykeyvalue7);
		
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
	actsproc::
	#endif 
savekeyvalues(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_buffer_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount =  BLOCKRAM_SIZE;
	
	SAVEKEYVALUES2_LOOP: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE II=1
		keyvalue_t mykeyvalue0 = GETKV(buffer[0][bufferoffset_kvs + i]);
		keyvalue_t mykeyvalue1 = GETKV(buffer[1][bufferoffset_kvs + i]);
		keyvalue_t mykeyvalue2 = GETKV(buffer[2][bufferoffset_kvs + i]);
		keyvalue_t mykeyvalue3 = GETKV(buffer[3][bufferoffset_kvs + i]);
		keyvalue_t mykeyvalue4 = GETKV(buffer[4][bufferoffset_kvs + i]);
		keyvalue_t mykeyvalue5 = GETKV(buffer[5][bufferoffset_kvs + i]);
		keyvalue_t mykeyvalue6 = GETKV(buffer[6][bufferoffset_kvs + i]);
		keyvalue_t mykeyvalue7 = GETKV(buffer[7][bufferoffset_kvs + i]);
	
		#ifdef _WIDEWORD
		kvdram[dramoffset_kvs + i].range(31, 0) = mykeyvalue0.key; 
		kvdram[dramoffset_kvs + i].range(63, 32) = mykeyvalue0.value; 
		kvdram[dramoffset_kvs + i].range(95, 64) = mykeyvalue1.key; 
		kvdram[dramoffset_kvs + i].range(127, 96) = mykeyvalue1.value; 
		kvdram[dramoffset_kvs + i].range(159, 128) = mykeyvalue2.key; 
		kvdram[dramoffset_kvs + i].range(191, 160) = mykeyvalue2.value; 
		kvdram[dramoffset_kvs + i].range(223, 192) = mykeyvalue3.key; 
		kvdram[dramoffset_kvs + i].range(255, 224) = mykeyvalue3.value; 
		kvdram[dramoffset_kvs + i].range(287, 256) = mykeyvalue4.key; 
		kvdram[dramoffset_kvs + i].range(319, 288) = mykeyvalue4.value; 
		kvdram[dramoffset_kvs + i].range(351, 320) = mykeyvalue5.key; 
		kvdram[dramoffset_kvs + i].range(383, 352) = mykeyvalue5.value; 
		kvdram[dramoffset_kvs + i].range(415, 384) = mykeyvalue6.key; 
		kvdram[dramoffset_kvs + i].range(447, 416) = mykeyvalue6.value; 
		kvdram[dramoffset_kvs + i].range(479, 448) = mykeyvalue7.key; 
		kvdram[dramoffset_kvs + i].range(511, 480) = mykeyvalue7.value; 
		#else 
		kvdram[dramoffset_kvs + i].data[0].key = mykeyvalue0.key;
		kvdram[dramoffset_kvs + i].data[0].value = mykeyvalue0.value;
		kvdram[dramoffset_kvs + i].data[1].key = mykeyvalue1.key;
		kvdram[dramoffset_kvs + i].data[1].value = mykeyvalue1.value;
		kvdram[dramoffset_kvs + i].data[2].key = mykeyvalue2.key;
		kvdram[dramoffset_kvs + i].data[2].value = mykeyvalue2.value;
		kvdram[dramoffset_kvs + i].data[3].key = mykeyvalue3.key;
		kvdram[dramoffset_kvs + i].data[3].value = mykeyvalue3.value;
		kvdram[dramoffset_kvs + i].data[4].key = mykeyvalue4.key;
		kvdram[dramoffset_kvs + i].data[4].value = mykeyvalue4.value;
		kvdram[dramoffset_kvs + i].data[5].key = mykeyvalue5.key;
		kvdram[dramoffset_kvs + i].data[5].value = mykeyvalue5.value;
		kvdram[dramoffset_kvs + i].data[6].key = mykeyvalue6.key;
		kvdram[dramoffset_kvs + i].data[6].value = mykeyvalue6.value;
		kvdram[dramoffset_kvs + i].data[7].key = mykeyvalue7.key;
		kvdram[dramoffset_kvs + i].data[7].value = mykeyvalue7.value;
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

void // 
	#ifdef SW 
	actsproc::
	#endif 
readvdata(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount = BLOCKRAM_SIZE;
		
	READKEYVALUES2_LOOP: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE II=1
		keyvalue_t mykeyvalue0;
		keyvalue_t mykeyvalue1;
		keyvalue_t mykeyvalue2;
		keyvalue_t mykeyvalue3;
		keyvalue_t mykeyvalue4;
		keyvalue_t mykeyvalue5;
		keyvalue_t mykeyvalue6;
		keyvalue_t mykeyvalue7;
		
		#ifdef _WIDEWORD
		mykeyvalue0.key = kvdram[dramoffset_kvs + i].range(31, 0); 
		mykeyvalue0.value = kvdram[dramoffset_kvs + i].range(63, 32); 
		mykeyvalue1.key = kvdram[dramoffset_kvs + i].range(95, 64); 
		mykeyvalue1.value = kvdram[dramoffset_kvs + i].range(127, 96); 
		mykeyvalue2.key = kvdram[dramoffset_kvs + i].range(159, 128); 
		mykeyvalue2.value = kvdram[dramoffset_kvs + i].range(191, 160); 
		mykeyvalue3.key = kvdram[dramoffset_kvs + i].range(223, 192); 
		mykeyvalue3.value = kvdram[dramoffset_kvs + i].range(255, 224); 
		mykeyvalue4.key = kvdram[dramoffset_kvs + i].range(287, 256); 
		mykeyvalue4.value = kvdram[dramoffset_kvs + i].range(319, 288); 
		mykeyvalue5.key = kvdram[dramoffset_kvs + i].range(351, 320); 
		mykeyvalue5.value = kvdram[dramoffset_kvs + i].range(383, 352); 
		mykeyvalue6.key = kvdram[dramoffset_kvs + i].range(415, 384); 
		mykeyvalue6.value = kvdram[dramoffset_kvs + i].range(447, 416); 
		mykeyvalue7.key = kvdram[dramoffset_kvs + i].range(479, 448); 
		mykeyvalue7.value = kvdram[dramoffset_kvs + i].range(511, 480); 
		#else 
		mykeyvalue0.key = kvdram[dramoffset_kvs + i].data[0].key; 
		mykeyvalue0.value = kvdram[dramoffset_kvs + i].data[0].value; 
		mykeyvalue1.key = kvdram[dramoffset_kvs + i].data[1].key; 
		mykeyvalue1.value = kvdram[dramoffset_kvs + i].data[1].value; 
		mykeyvalue2.key = kvdram[dramoffset_kvs + i].data[2].key; 
		mykeyvalue2.value = kvdram[dramoffset_kvs + i].data[2].value; 
		mykeyvalue3.key = kvdram[dramoffset_kvs + i].data[3].key; 
		mykeyvalue3.value = kvdram[dramoffset_kvs + i].data[3].value; 
		mykeyvalue4.key = kvdram[dramoffset_kvs + i].data[4].key; 
		mykeyvalue4.value = kvdram[dramoffset_kvs + i].data[4].value; 
		mykeyvalue5.key = kvdram[dramoffset_kvs + i].data[5].key; 
		mykeyvalue5.value = kvdram[dramoffset_kvs + i].data[5].value; 
		mykeyvalue6.key = kvdram[dramoffset_kvs + i].data[6].key; 
		mykeyvalue6.value = kvdram[dramoffset_kvs + i].data[6].value; 
		mykeyvalue7.key = kvdram[dramoffset_kvs + i].data[7].key; 
		mykeyvalue7.value = kvdram[dramoffset_kvs + i].data[7].value; 
		#endif 
		
		buffer[begincol + 0][bufferoffset_kvs + i] = GETKV2(mykeyvalue0);
		buffer[begincol + 1][bufferoffset_kvs + i] = GETKV2(mykeyvalue1);
		buffer[begincol + 2][bufferoffset_kvs + i] = GETKV2(mykeyvalue2);
		buffer[begincol + 3][bufferoffset_kvs + i] = GETKV2(mykeyvalue3);
		buffer[begincol + 4][bufferoffset_kvs + i] = GETKV2(mykeyvalue4);
		buffer[begincol + 5][bufferoffset_kvs + i] = GETKV2(mykeyvalue5);
		buffer[begincol + 6][bufferoffset_kvs + i] = GETKV2(mykeyvalue6);
		buffer[begincol + 7][bufferoffset_kvs + i] = GETKV2(mykeyvalue7);
		
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<< TIMINGRESULTSCOLOR<<"readvdata:: vertices read: offset: "<<dramoffset_kvs * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl;
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(dramoffset_kvs >= globalparams.BASEOFFSETKVS_VERTEXPTR_ && dramoffset_kvs < globalparams.BASEOFFSETKVS_VERTEXPTR + VERTEXPTRSSZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"readkeyvalues:: vertices read: offset: "<<(dramoffset_kvs - globalparams.BASEOFFSETKVS_VERTEXPTR) * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs - globalparams.BASEOFFSETKVS_VERTEXPTR) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	if(dramoffset_kvs >= globalparams.BASEOFFSETKVS_VERTICESDATA_ && dramoffset_kvs < globalparams.BASEOFFSETKVS_VERTICESDATA + VERTICESDATASZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"readkeyvalues:: vertices read: offset: "<<(dramoffset_kvs - globalparams.BASEOFFSETKVS_VERTICESDATA) * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs - globalparams.BASEOFFSETKVS_VERTICESDATA) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	#endif
	return;
}

void 
	#ifdef SW 
	actsproc::
	#endif 
savevdata(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount =  REDUCESZ / 2;
	
	SAVEVDATA_LOOP: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE II=1
		keyvalue_t mykeyvalue0 = GETKV2(buffer[begincol + 0][bufferoffset_kvs + i]);
		keyvalue_t mykeyvalue1 = GETKV2(buffer[begincol + 1][bufferoffset_kvs + i]);
		keyvalue_t mykeyvalue2 = GETKV2(buffer[begincol + 2][bufferoffset_kvs + i]);
		keyvalue_t mykeyvalue3 = GETKV2(buffer[begincol + 3][bufferoffset_kvs + i]);
		keyvalue_t mykeyvalue4 = GETKV2(buffer[begincol + 4][bufferoffset_kvs + i]);
		keyvalue_t mykeyvalue5 = GETKV2(buffer[begincol + 5][bufferoffset_kvs + i]);
		keyvalue_t mykeyvalue6 = GETKV2(buffer[begincol + 6][bufferoffset_kvs + i]);
		keyvalue_t mykeyvalue7 = GETKV2(buffer[begincol + 7][bufferoffset_kvs + i]);
	
		#ifdef _WIDEWORD
		kvdram[dramoffset_kvs + i].range(31, 0) = mykeyvalue0.key; 
		kvdram[dramoffset_kvs + i].range(63, 32) = mykeyvalue0.value; 
		kvdram[dramoffset_kvs + i].range(95, 64) = mykeyvalue1.key; 
		kvdram[dramoffset_kvs + i].range(127, 96) = mykeyvalue1.value; 
		kvdram[dramoffset_kvs + i].range(159, 128) = mykeyvalue2.key; 
		kvdram[dramoffset_kvs + i].range(191, 160) = mykeyvalue2.value; 
		kvdram[dramoffset_kvs + i].range(223, 192) = mykeyvalue3.key; 
		kvdram[dramoffset_kvs + i].range(255, 224) = mykeyvalue3.value; 
		kvdram[dramoffset_kvs + i].range(287, 256) = mykeyvalue4.key; 
		kvdram[dramoffset_kvs + i].range(319, 288) = mykeyvalue4.value; 
		kvdram[dramoffset_kvs + i].range(351, 320) = mykeyvalue5.key; 
		kvdram[dramoffset_kvs + i].range(383, 352) = mykeyvalue5.value; 
		kvdram[dramoffset_kvs + i].range(415, 384) = mykeyvalue6.key; 
		kvdram[dramoffset_kvs + i].range(447, 416) = mykeyvalue6.value; 
		kvdram[dramoffset_kvs + i].range(479, 448) = mykeyvalue7.key; 
		kvdram[dramoffset_kvs + i].range(511, 480) = mykeyvalue7.value; 
		#else 
		kvdram[dramoffset_kvs + i].data[0].key = mykeyvalue0.key;
		kvdram[dramoffset_kvs + i].data[0].value = mykeyvalue0.value;
		kvdram[dramoffset_kvs + i].data[1].key = mykeyvalue1.key;
		kvdram[dramoffset_kvs + i].data[1].value = mykeyvalue1.value;
		kvdram[dramoffset_kvs + i].data[2].key = mykeyvalue2.key;
		kvdram[dramoffset_kvs + i].data[2].value = mykeyvalue2.value;
		kvdram[dramoffset_kvs + i].data[3].key = mykeyvalue3.key;
		kvdram[dramoffset_kvs + i].data[3].value = mykeyvalue3.value;
		kvdram[dramoffset_kvs + i].data[4].key = mykeyvalue4.key;
		kvdram[dramoffset_kvs + i].data[4].value = mykeyvalue4.value;
		kvdram[dramoffset_kvs + i].data[5].key = mykeyvalue5.key;
		kvdram[dramoffset_kvs + i].data[5].value = mykeyvalue5.value;
		kvdram[dramoffset_kvs + i].data[6].key = mykeyvalue6.key;
		kvdram[dramoffset_kvs + i].data[6].value = mykeyvalue6.value;
		kvdram[dramoffset_kvs + i].data[7].key = mykeyvalue7.key;
		kvdram[dramoffset_kvs + i].data[7].value = mykeyvalue7.value;
		#endif 
	
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvswritten(VECTOR_SIZE);
		#endif
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"savevdata:: vertices saved: offset: "<<dramoffset_kvs * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs) * VECTOR_SIZE<<", number of vertex datas written: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<<endl;
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS2
	if(dramoffset_kvs >= globalparams.BASEOFFSETKVS_VERTEXPTR && dramoffset_kvs < globalparams.BASEOFFSETKVS_VERTEXPTR + VERTEXPTRSSZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"savekeyvalues:: vertices saved: offset: "<<dramoffset_kvs-globalparams.BASEOFFSETKVS_VERTEXPTR * VECTOR_SIZE<<"-"<<((dramoffset_kvs + size_kvs)-globalparams.BASEOFFSETKVS_VERTEXPTR) * VECTOR_SIZE<<", number of vertex datas written: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	if(dramoffset_kvs >= globalparams.BASEOFFSETKVS_VERTICESDATA && dramoffset_kvs < globalparams.BASEOFFSETKVS_VERTICESDATA + VERTICESDATASZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"savekeyvalues:: vertices saved: offset: "<<(dramoffset_kvs-globalparams.BASEOFFSETKVS_VERTICESDATA) * VECTOR_SIZE<<"-"<<((dramoffset_kvs-globalparams.BASEOFFSETKVS_VERTICESDATA) + size_kvs) * VECTOR_SIZE<<", number of vertex datas written: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }					
	#endif
	return;
}

void //
	#ifdef SW 
	actsproc::
	#endif 
loadvmasks(bool_type enable, uint512_dt * kvdram, uintNUMPby2_type vmask[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount1 = BLOCKRAM_SIZE;
	analysis_type analysis_loopcount2 = BLOCKRAM_SIZE / 16;
	analysis_type analysis_loopcount3 = BLOCKRAM_SIZE;
	
	buffer_type transfsize = size_kvs * 16;
	uint32_type bitsbuffer[MAXREDUCEBUFFERSZ];

	READKEYVALUES2_LOOP: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount1 avg=analysis_loopcount1
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		tempbuffer[0][i] = kvdram[offset_kvs + i].range(31, 0); 
		tempbuffer[1][i] = kvdram[offset_kvs + i].range(63, 32); 
		tempbuffer[2][i] = kvdram[offset_kvs + i].range(95, 64); 
		tempbuffer[3][i] = kvdram[offset_kvs + i].range(127, 96); 
		tempbuffer[4][i] = kvdram[offset_kvs + i].range(159, 128); 
		tempbuffer[5][i] = kvdram[offset_kvs + i].range(191, 160); 
		tempbuffer[6][i] = kvdram[offset_kvs + i].range(223, 192); 
		tempbuffer[7][i] = kvdram[offset_kvs + i].range(255, 224); 
		tempbuffer[8][i] = kvdram[offset_kvs + i].range(287, 256); 
		tempbuffer[9][i] = kvdram[offset_kvs + i].range(319, 288); 
		tempbuffer[10][i] = kvdram[offset_kvs + i].range(351, 320); 
		tempbuffer[11][i] = kvdram[offset_kvs + i].range(383, 352); 
		tempbuffer[12][i] = kvdram[offset_kvs + i].range(415, 384); 
		tempbuffer[13][i] = kvdram[offset_kvs + i].range(447, 416); 
		tempbuffer[14][i] = kvdram[offset_kvs + i].range(479, 448); 
		tempbuffer[15][i] = kvdram[offset_kvs + i].range(511, 480); 
		#else 
		tempbuffer[0][i].key = kvdram[offset_kvs + i].data[0].key;
		tempbuffer[0][i].value = kvdram[offset_kvs + i].data[0].value; 
		tempbuffer[1][i].key = kvdram[offset_kvs + i].data[1].key;
		tempbuffer[1][i].value = kvdram[offset_kvs + i].data[1].value; 
		tempbuffer[2][i].key = kvdram[offset_kvs + i].data[2].key;
		tempbuffer[2][i].value = kvdram[offset_kvs + i].data[2].value; 
		tempbuffer[3][i].key = kvdram[offset_kvs + i].data[3].key;
		tempbuffer[3][i].value = kvdram[offset_kvs + i].data[3].value; 
		tempbuffer[4][i].key = kvdram[offset_kvs + i].data[4].key;
		tempbuffer[4][i].value = kvdram[offset_kvs + i].data[4].value; 
		tempbuffer[5][i].key = kvdram[offset_kvs + i].data[5].key;
		tempbuffer[5][i].value = kvdram[offset_kvs + i].data[5].value; 
		tempbuffer[6][i].key = kvdram[offset_kvs + i].data[6].key;
		tempbuffer[6][i].value = kvdram[offset_kvs + i].data[6].value; 
		tempbuffer[7][i].key = kvdram[offset_kvs + i].data[7].key;
		tempbuffer[7][i].value = kvdram[offset_kvs + i].data[7].value; 
		
		#endif 
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	
	buffer_type index = 0;
	LOADVMASKS_LOOP1: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount2 avg=analysis_loopcount2
	#pragma HLS PIPELINE II=8
		#ifdef _WIDEWORD
		bitsbuffer[index + 0] = tempbuffer[0][i];
		bitsbuffer[index + 1] = tempbuffer[1][i];
		bitsbuffer[index + 2] = tempbuffer[2][i];
		bitsbuffer[index + 3] = tempbuffer[3][i];
		bitsbuffer[index + 4] = tempbuffer[4][i];
		bitsbuffer[index + 5] = tempbuffer[5][i];
		bitsbuffer[index + 6] = tempbuffer[6][i];
		bitsbuffer[index + 7] = tempbuffer[7][i];
		bitsbuffer[index + 8] = tempbuffer[8][i];
		bitsbuffer[index + 9] = tempbuffer[9][i];
		bitsbuffer[index + 10] = tempbuffer[10][i];
		bitsbuffer[index + 11] = tempbuffer[11][i];
		bitsbuffer[index + 12] = tempbuffer[12][i];
		bitsbuffer[index + 13] = tempbuffer[13][i];
		bitsbuffer[index + 14] = tempbuffer[14][i];
		bitsbuffer[index + 15] = tempbuffer[15][i];
		#else 
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
		#endif 
		
		index += VECTOR_SIZE * 2;
	}

	uintNUMPby2_type tempvmask;
	#pragma HLS DATA_PACK variable = tempvmask
	uintNUMPby2_type tempvmask_sp; 
	#pragma HLS DATA_PACK variable = tempvmask_sp
	tempvmask_sp.data[0].key = 0;
	tempvmask_sp.data[0].value = 0;
	tempvmask_sp.data[1].key = 0;
	tempvmask_sp.data[1].value = 0;
	tempvmask_sp.data[2].key = 0;
	tempvmask_sp.data[2].value = 0;
	tempvmask_sp.data[3].key = 0;
	tempvmask_sp.data[3].value = 0;
	tempvmask_sp.data[4].key = 0;
	tempvmask_sp.data[4].value = 0;
	tempvmask_sp.data[5].key = 0;
	tempvmask_sp.data[5].value = 0;
	tempvmask_sp.data[6].key = 0;
	tempvmask_sp.data[6].value = 0;
	tempvmask_sp.data[7].key = 0;
	tempvmask_sp.data[7].value = 0;
	tempvmask_sp.data[8].key = 0;
	tempvmask_sp.data[8].value = 0;
	tempvmask_sp.data[9].key = 0;
	tempvmask_sp.data[9].value = 0;
	tempvmask_sp.data[10].key = 0;
	tempvmask_sp.data[10].value = 0;
	tempvmask_sp.data[11].key = 0;
	tempvmask_sp.data[11].value = 0;
	tempvmask_sp.data[12].key = 0;
	tempvmask_sp.data[12].value = 0;
	tempvmask_sp.data[13].key = 0;
	tempvmask_sp.data[13].value = 0;
	tempvmask_sp.data[14].key = 0;
	tempvmask_sp.data[14].value = 0;
	tempvmask_sp.data[15].key = 0;
	tempvmask_sp.data[15].value = 0;
	tempvmask_sp.data[16].key = 0;
	tempvmask_sp.data[16].value = 0;
	tempvmask_sp.data[17].key = 0;
	tempvmask_sp.data[17].value = 0;
	tempvmask_sp.data[18].key = 0;
	tempvmask_sp.data[18].value = 0;
	tempvmask_sp.data[19].key = 0;
	tempvmask_sp.data[19].value = 0;
	tempvmask_sp.data[20].key = 0;
	tempvmask_sp.data[20].value = 0;
	tempvmask_sp.data[21].key = 0;
	tempvmask_sp.data[21].value = 0;
	tempvmask_sp.data[22].key = 0;
	tempvmask_sp.data[22].value = 0;
	tempvmask_sp.data[23].key = 0;
	tempvmask_sp.data[23].value = 0;
	tempvmask_sp.data[24].key = 0;
	tempvmask_sp.data[24].value = 0;
	tempvmask_sp.data[25].key = 0;
	tempvmask_sp.data[25].value = 0;
	tempvmask_sp.data[26].key = 0;
	tempvmask_sp.data[26].value = 0;
	tempvmask_sp.data[27].key = 0;
	tempvmask_sp.data[27].value = 0;
	tempvmask_sp.data[28].key = 0;
	tempvmask_sp.data[28].value = 0;
	tempvmask_sp.data[29].key = 0;
	tempvmask_sp.data[29].value = 0;
	tempvmask_sp.data[30].key = 0;
	tempvmask_sp.data[30].value = 0;
	tempvmask_sp.data[31].key = 0;
	tempvmask_sp.data[31].value = 0;
	tempvmask_sp.data[32].key = 0;
	tempvmask_sp.data[32].value = 0;
	tempvmask_sp.data[33].key = 0;
	tempvmask_sp.data[33].value = 0;
	tempvmask_sp.data[34].key = 0;
	tempvmask_sp.data[34].value = 0;
	tempvmask_sp.data[35].key = 0;
	tempvmask_sp.data[35].value = 0;
	tempvmask_sp.data[36].key = 0;
	tempvmask_sp.data[36].value = 0;
	tempvmask_sp.data[37].key = 0;
	tempvmask_sp.data[37].value = 0;
	tempvmask_sp.data[38].key = 0;
	tempvmask_sp.data[38].value = 0;
	tempvmask_sp.data[39].key = 0;
	tempvmask_sp.data[39].value = 0;
	tempvmask_sp.data[40].key = 0;
	tempvmask_sp.data[40].value = 0;
	tempvmask_sp.data[41].key = 0;
	tempvmask_sp.data[41].value = 0;
	tempvmask_sp.data[42].key = 0;
	tempvmask_sp.data[42].value = 0;
	tempvmask_sp.data[43].key = 0;
	tempvmask_sp.data[43].value = 0;
	tempvmask_sp.data[44].key = 0;
	tempvmask_sp.data[44].value = 0;
	tempvmask_sp.data[45].key = 0;
	tempvmask_sp.data[45].value = 0;
	tempvmask_sp.data[46].key = 0;
	tempvmask_sp.data[46].value = 0;
	tempvmask_sp.data[47].key = 0;
	tempvmask_sp.data[47].value = 0;
	tempvmask_sp.data[48].key = 0;
	tempvmask_sp.data[48].value = 0;
	tempvmask_sp.data[49].key = 0;
	tempvmask_sp.data[49].value = 0;
	tempvmask_sp.data[50].key = 0;
	tempvmask_sp.data[50].value = 0;
	tempvmask_sp.data[51].key = 0;
	tempvmask_sp.data[51].value = 0;
	tempvmask_sp.data[52].key = 0;
	tempvmask_sp.data[52].value = 0;
	tempvmask_sp.data[53].key = 0;
	tempvmask_sp.data[53].value = 0;
	tempvmask_sp.data[54].key = 0;
	tempvmask_sp.data[54].value = 0;
	tempvmask_sp.data[55].key = 0;
	tempvmask_sp.data[55].value = 0;
	tempvmask_sp.data[56].key = 0;
	tempvmask_sp.data[56].value = 0;
	tempvmask_sp.data[57].key = 0;
	tempvmask_sp.data[57].value = 0;
	tempvmask_sp.data[58].key = 0;
	tempvmask_sp.data[58].value = 0;
	tempvmask_sp.data[59].key = 0;
	tempvmask_sp.data[59].value = 0;
	tempvmask_sp.data[60].key = 0;
	tempvmask_sp.data[60].value = 0;
	tempvmask_sp.data[61].key = 0;
	tempvmask_sp.data[61].value = 0;
	tempvmask_sp.data[62].key = 0;
	tempvmask_sp.data[62].value = 0;
	tempvmask_sp.data[63].key = 0;
	tempvmask_sp.data[63].value = 0;
	tempvmask_sp.data[64].key = 0;
	tempvmask_sp.data[64].value = 0;
	tempvmask_sp.data[65].key = 0;
	tempvmask_sp.data[65].value = 0;
	tempvmask_sp.data[66].key = 0;
	tempvmask_sp.data[66].value = 0;
	tempvmask_sp.data[67].key = 0;
	tempvmask_sp.data[67].value = 0;
	tempvmask_sp.data[68].key = 0;
	tempvmask_sp.data[68].value = 0;
	tempvmask_sp.data[69].key = 0;
	tempvmask_sp.data[69].value = 0;
	tempvmask_sp.data[70].key = 0;
	tempvmask_sp.data[70].value = 0;
	tempvmask_sp.data[71].key = 0;
	tempvmask_sp.data[71].value = 0;
	tempvmask_sp.data[72].key = 0;
	tempvmask_sp.data[72].value = 0;
	tempvmask_sp.data[73].key = 0;
	tempvmask_sp.data[73].value = 0;
	tempvmask_sp.data[74].key = 0;
	tempvmask_sp.data[74].value = 0;
	tempvmask_sp.data[75].key = 0;
	tempvmask_sp.data[75].value = 0;
	tempvmask_sp.data[76].key = 0;
	tempvmask_sp.data[76].value = 0;
	tempvmask_sp.data[77].key = 0;
	tempvmask_sp.data[77].value = 0;
	tempvmask_sp.data[78].key = 0;
	tempvmask_sp.data[78].value = 0;
	tempvmask_sp.data[79].key = 0;
	tempvmask_sp.data[79].value = 0;
	tempvmask_sp.data[80].key = 0;
	tempvmask_sp.data[80].value = 0;
	tempvmask_sp.data[81].key = 0;
	tempvmask_sp.data[81].value = 0;
	tempvmask_sp.data[82].key = 0;
	tempvmask_sp.data[82].value = 0;
	tempvmask_sp.data[83].key = 0;
	tempvmask_sp.data[83].value = 0;
	tempvmask_sp.data[84].key = 0;
	tempvmask_sp.data[84].value = 0;
	tempvmask_sp.data[85].key = 0;
	tempvmask_sp.data[85].value = 0;
	tempvmask_sp.data[86].key = 0;
	tempvmask_sp.data[86].value = 0;
	tempvmask_sp.data[87].key = 0;
	tempvmask_sp.data[87].value = 0;
	tempvmask_sp.data[88].key = 0;
	tempvmask_sp.data[88].value = 0;
	tempvmask_sp.data[89].key = 0;
	tempvmask_sp.data[89].value = 0;
	tempvmask_sp.data[90].key = 0;
	tempvmask_sp.data[90].value = 0;
	tempvmask_sp.data[91].key = 0;
	tempvmask_sp.data[91].value = 0;
	tempvmask_sp.data[92].key = 0;
	tempvmask_sp.data[92].value = 0;
	tempvmask_sp.data[93].key = 0;
	tempvmask_sp.data[93].value = 0;
	tempvmask_sp.data[94].key = 0;
	tempvmask_sp.data[94].value = 0;
	tempvmask_sp.data[95].key = 0;
	tempvmask_sp.data[95].value = 0;
	tempvmask_sp.data[96].key = 0;
	tempvmask_sp.data[96].value = 0;
	tempvmask_sp.data[97].key = 0;
	tempvmask_sp.data[97].value = 0;
	tempvmask_sp.data[98].key = 0;
	tempvmask_sp.data[98].value = 0;
	tempvmask_sp.data[99].key = 0;
	tempvmask_sp.data[99].value = 0;
	tempvmask_sp.data[100].key = 0;
	tempvmask_sp.data[100].value = 0;
	tempvmask_sp.data[101].key = 0;
	tempvmask_sp.data[101].value = 0;
	tempvmask_sp.data[102].key = 0;
	tempvmask_sp.data[102].value = 0;
	tempvmask_sp.data[103].key = 0;
	tempvmask_sp.data[103].value = 0;
	tempvmask_sp.data[104].key = 0;
	tempvmask_sp.data[104].value = 0;
	tempvmask_sp.data[105].key = 0;
	tempvmask_sp.data[105].value = 0;
	tempvmask_sp.data[106].key = 0;
	tempvmask_sp.data[106].value = 0;
	tempvmask_sp.data[107].key = 0;
	tempvmask_sp.data[107].value = 0;
	tempvmask_sp.data[108].key = 0;
	tempvmask_sp.data[108].value = 0;
	tempvmask_sp.data[109].key = 0;
	tempvmask_sp.data[109].value = 0;
	tempvmask_sp.data[110].key = 0;
	tempvmask_sp.data[110].value = 0;
	tempvmask_sp.data[111].key = 0;
	tempvmask_sp.data[111].value = 0;
	tempvmask_sp.data[112].key = 0;
	tempvmask_sp.data[112].value = 0;
	tempvmask_sp.data[113].key = 0;
	tempvmask_sp.data[113].value = 0;
	tempvmask_sp.data[114].key = 0;
	tempvmask_sp.data[114].value = 0;
	tempvmask_sp.data[115].key = 0;
	tempvmask_sp.data[115].value = 0;
	tempvmask_sp.data[116].key = 0;
	tempvmask_sp.data[116].value = 0;
	tempvmask_sp.data[117].key = 0;
	tempvmask_sp.data[117].value = 0;
	tempvmask_sp.data[118].key = 0;
	tempvmask_sp.data[118].value = 0;
	tempvmask_sp.data[119].key = 0;
	tempvmask_sp.data[119].value = 0;
	tempvmask_sp.data[120].key = 0;
	tempvmask_sp.data[120].value = 0;
	tempvmask_sp.data[121].key = 0;
	tempvmask_sp.data[121].value = 0;
	tempvmask_sp.data[122].key = 0;
	tempvmask_sp.data[122].value = 0;
	tempvmask_sp.data[123].key = 0;
	tempvmask_sp.data[123].value = 0;
	tempvmask_sp.data[124].key = 0;
	tempvmask_sp.data[124].value = 0;
	tempvmask_sp.data[125].key = 0;
	tempvmask_sp.data[125].value = 0;
	tempvmask_sp.data[126].key = 0;
	tempvmask_sp.data[126].value = 0;
	tempvmask_sp.data[127].key = 0;
	tempvmask_sp.data[127].value = 0;
	tempvmask_sp.data[128].key = 0;
	tempvmask_sp.data[128].value = 0;
	tempvmask_sp.data[129].key = 0;
	tempvmask_sp.data[129].value = 0;
	tempvmask_sp.data[130].key = 0;
	tempvmask_sp.data[130].value = 0;
	tempvmask_sp.data[131].key = 0;
	tempvmask_sp.data[131].value = 0;
	tempvmask_sp.data[132].key = 0;
	tempvmask_sp.data[132].value = 0;
	tempvmask_sp.data[133].key = 0;
	tempvmask_sp.data[133].value = 0;
	tempvmask_sp.data[134].key = 0;
	tempvmask_sp.data[134].value = 0;
	tempvmask_sp.data[135].key = 0;
	tempvmask_sp.data[135].value = 0;
	tempvmask_sp.data[136].key = 0;
	tempvmask_sp.data[136].value = 0;
	tempvmask_sp.data[137].key = 0;
	tempvmask_sp.data[137].value = 0;
	tempvmask_sp.data[138].key = 0;
	tempvmask_sp.data[138].value = 0;
	tempvmask_sp.data[139].key = 0;
	tempvmask_sp.data[139].value = 0;
	tempvmask_sp.data[140].key = 0;
	tempvmask_sp.data[140].value = 0;
	tempvmask_sp.data[141].key = 0;
	tempvmask_sp.data[141].value = 0;
	tempvmask_sp.data[142].key = 0;
	tempvmask_sp.data[142].value = 0;
	tempvmask_sp.data[143].key = 0;
	tempvmask_sp.data[143].value = 0;
	tempvmask_sp.data[144].key = 0;
	tempvmask_sp.data[144].value = 0;
	tempvmask_sp.data[145].key = 0;
	tempvmask_sp.data[145].value = 0;
	tempvmask_sp.data[146].key = 0;
	tempvmask_sp.data[146].value = 0;
	tempvmask_sp.data[147].key = 0;
	tempvmask_sp.data[147].value = 0;
	tempvmask_sp.data[148].key = 0;
	tempvmask_sp.data[148].value = 0;
	tempvmask_sp.data[149].key = 0;
	tempvmask_sp.data[149].value = 0;
	tempvmask_sp.data[150].key = 0;
	tempvmask_sp.data[150].value = 0;
	tempvmask_sp.data[151].key = 0;
	tempvmask_sp.data[151].value = 0;
	tempvmask_sp.data[152].key = 0;
	tempvmask_sp.data[152].value = 0;
	tempvmask_sp.data[153].key = 0;
	tempvmask_sp.data[153].value = 0;
	tempvmask_sp.data[154].key = 0;
	tempvmask_sp.data[154].value = 0;
	tempvmask_sp.data[155].key = 0;
	tempvmask_sp.data[155].value = 0;
	tempvmask_sp.data[156].key = 0;
	tempvmask_sp.data[156].value = 0;
	tempvmask_sp.data[157].key = 0;
	tempvmask_sp.data[157].value = 0;
	tempvmask_sp.data[158].key = 0;
	tempvmask_sp.data[158].value = 0;
	tempvmask_sp.data[159].key = 0;
	tempvmask_sp.data[159].value = 0;
	tempvmask_sp.data[160].key = 0;
	tempvmask_sp.data[160].value = 0;
	tempvmask_sp.data[161].key = 0;
	tempvmask_sp.data[161].value = 0;
	tempvmask_sp.data[162].key = 0;
	tempvmask_sp.data[162].value = 0;
	tempvmask_sp.data[163].key = 0;
	tempvmask_sp.data[163].value = 0;
	tempvmask_sp.data[164].key = 0;
	tempvmask_sp.data[164].value = 0;
	tempvmask_sp.data[165].key = 0;
	tempvmask_sp.data[165].value = 0;
	tempvmask_sp.data[166].key = 0;
	tempvmask_sp.data[166].value = 0;
	tempvmask_sp.data[167].key = 0;
	tempvmask_sp.data[167].value = 0;
	tempvmask_sp.data[168].key = 0;
	tempvmask_sp.data[168].value = 0;
	tempvmask_sp.data[169].key = 0;
	tempvmask_sp.data[169].value = 0;
	tempvmask_sp.data[170].key = 0;
	tempvmask_sp.data[170].value = 0;
	tempvmask_sp.data[171].key = 0;
	tempvmask_sp.data[171].value = 0;
	tempvmask_sp.data[172].key = 0;
	tempvmask_sp.data[172].value = 0;
	tempvmask_sp.data[173].key = 0;
	tempvmask_sp.data[173].value = 0;
	tempvmask_sp.data[174].key = 0;
	tempvmask_sp.data[174].value = 0;
	tempvmask_sp.data[175].key = 0;
	tempvmask_sp.data[175].value = 0;
	tempvmask_sp.data[176].key = 0;
	tempvmask_sp.data[176].value = 0;
	tempvmask_sp.data[177].key = 0;
	tempvmask_sp.data[177].value = 0;
	tempvmask_sp.data[178].key = 0;
	tempvmask_sp.data[178].value = 0;
	tempvmask_sp.data[179].key = 0;
	tempvmask_sp.data[179].value = 0;
	tempvmask_sp.data[180].key = 0;
	tempvmask_sp.data[180].value = 0;
	tempvmask_sp.data[181].key = 0;
	tempvmask_sp.data[181].value = 0;
	tempvmask_sp.data[182].key = 0;
	tempvmask_sp.data[182].value = 0;
	tempvmask_sp.data[183].key = 0;
	tempvmask_sp.data[183].value = 0;
	tempvmask_sp.data[184].key = 0;
	tempvmask_sp.data[184].value = 0;
	tempvmask_sp.data[185].key = 0;
	tempvmask_sp.data[185].value = 0;
	tempvmask_sp.data[186].key = 0;
	tempvmask_sp.data[186].value = 0;
	tempvmask_sp.data[187].key = 0;
	tempvmask_sp.data[187].value = 0;
	tempvmask_sp.data[188].key = 0;
	tempvmask_sp.data[188].value = 0;
	tempvmask_sp.data[189].key = 0;
	tempvmask_sp.data[189].value = 0;
	tempvmask_sp.data[190].key = 0;
	tempvmask_sp.data[190].value = 0;
	tempvmask_sp.data[191].key = 0;
	tempvmask_sp.data[191].value = 0;
	tempvmask_sp.data[192].key = 0;
	tempvmask_sp.data[192].value = 0;
	tempvmask_sp.data[193].key = 0;
	tempvmask_sp.data[193].value = 0;
	tempvmask_sp.data[194].key = 0;
	tempvmask_sp.data[194].value = 0;
	tempvmask_sp.data[195].key = 0;
	tempvmask_sp.data[195].value = 0;
	tempvmask_sp.data[196].key = 0;
	tempvmask_sp.data[196].value = 0;
	tempvmask_sp.data[197].key = 0;
	tempvmask_sp.data[197].value = 0;
	tempvmask_sp.data[198].key = 0;
	tempvmask_sp.data[198].value = 0;
	tempvmask_sp.data[199].key = 0;
	tempvmask_sp.data[199].value = 0;
	tempvmask_sp.data[200].key = 0;
	tempvmask_sp.data[200].value = 0;
	tempvmask_sp.data[201].key = 0;
	tempvmask_sp.data[201].value = 0;
	tempvmask_sp.data[202].key = 0;
	tempvmask_sp.data[202].value = 0;
	tempvmask_sp.data[203].key = 0;
	tempvmask_sp.data[203].value = 0;
	tempvmask_sp.data[204].key = 0;
	tempvmask_sp.data[204].value = 0;
	tempvmask_sp.data[205].key = 0;
	tempvmask_sp.data[205].value = 0;
	tempvmask_sp.data[206].key = 0;
	tempvmask_sp.data[206].value = 0;
	tempvmask_sp.data[207].key = 0;
	tempvmask_sp.data[207].value = 0;
	tempvmask_sp.data[208].key = 0;
	tempvmask_sp.data[208].value = 0;
	tempvmask_sp.data[209].key = 0;
	tempvmask_sp.data[209].value = 0;
	tempvmask_sp.data[210].key = 0;
	tempvmask_sp.data[210].value = 0;
	tempvmask_sp.data[211].key = 0;
	tempvmask_sp.data[211].value = 0;
	tempvmask_sp.data[212].key = 0;
	tempvmask_sp.data[212].value = 0;
	tempvmask_sp.data[213].key = 0;
	tempvmask_sp.data[213].value = 0;
	tempvmask_sp.data[214].key = 0;
	tempvmask_sp.data[214].value = 0;
	tempvmask_sp.data[215].key = 0;
	tempvmask_sp.data[215].value = 0;
	tempvmask_sp.data[216].key = 0;
	tempvmask_sp.data[216].value = 0;
	tempvmask_sp.data[217].key = 0;
	tempvmask_sp.data[217].value = 0;
	tempvmask_sp.data[218].key = 0;
	tempvmask_sp.data[218].value = 0;
	tempvmask_sp.data[219].key = 0;
	tempvmask_sp.data[219].value = 0;
	tempvmask_sp.data[220].key = 0;
	tempvmask_sp.data[220].value = 0;
	tempvmask_sp.data[221].key = 0;
	tempvmask_sp.data[221].value = 0;
	tempvmask_sp.data[222].key = 0;
	tempvmask_sp.data[222].value = 0;
	tempvmask_sp.data[223].key = 0;
	tempvmask_sp.data[223].value = 0;
	tempvmask_sp.data[224].key = 0;
	tempvmask_sp.data[224].value = 0;
	tempvmask_sp.data[225].key = 0;
	tempvmask_sp.data[225].value = 0;
	tempvmask_sp.data[226].key = 0;
	tempvmask_sp.data[226].value = 0;
	tempvmask_sp.data[227].key = 0;
	tempvmask_sp.data[227].value = 0;
	tempvmask_sp.data[228].key = 0;
	tempvmask_sp.data[228].value = 0;
	tempvmask_sp.data[229].key = 0;
	tempvmask_sp.data[229].value = 0;
	tempvmask_sp.data[230].key = 0;
	tempvmask_sp.data[230].value = 0;
	tempvmask_sp.data[231].key = 0;
	tempvmask_sp.data[231].value = 0;
	tempvmask_sp.data[232].key = 0;
	tempvmask_sp.data[232].value = 0;
	tempvmask_sp.data[233].key = 0;
	tempvmask_sp.data[233].value = 0;
	tempvmask_sp.data[234].key = 0;
	tempvmask_sp.data[234].value = 0;
	tempvmask_sp.data[235].key = 0;
	tempvmask_sp.data[235].value = 0;
	tempvmask_sp.data[236].key = 0;
	tempvmask_sp.data[236].value = 0;
	tempvmask_sp.data[237].key = 0;
	tempvmask_sp.data[237].value = 0;
	tempvmask_sp.data[238].key = 0;
	tempvmask_sp.data[238].value = 0;
	tempvmask_sp.data[239].key = 0;
	tempvmask_sp.data[239].value = 0;
	tempvmask_sp.data[240].key = 0;
	tempvmask_sp.data[240].value = 0;
	tempvmask_sp.data[241].key = 0;
	tempvmask_sp.data[241].value = 0;
	tempvmask_sp.data[242].key = 0;
	tempvmask_sp.data[242].value = 0;
	tempvmask_sp.data[243].key = 0;
	tempvmask_sp.data[243].value = 0;
	tempvmask_sp.data[244].key = 0;
	tempvmask_sp.data[244].value = 0;
	tempvmask_sp.data[245].key = 0;
	tempvmask_sp.data[245].value = 0;
	tempvmask_sp.data[246].key = 0;
	tempvmask_sp.data[246].value = 0;
	tempvmask_sp.data[247].key = 0;
	tempvmask_sp.data[247].value = 0;
	tempvmask_sp.data[248].key = 0;
	tempvmask_sp.data[248].value = 0;
	tempvmask_sp.data[249].key = 0;
	tempvmask_sp.data[249].value = 0;
	tempvmask_sp.data[250].key = 0;
	tempvmask_sp.data[250].value = 0;
	tempvmask_sp.data[251].key = 0;
	tempvmask_sp.data[251].value = 0;
	tempvmask_sp.data[252].key = 0;
	tempvmask_sp.data[252].value = 0;
	tempvmask_sp.data[253].key = 0;
	tempvmask_sp.data[253].value = 0;
	tempvmask_sp.data[254].key = 0;
	tempvmask_sp.data[254].value = 0;
	tempvmask_sp.data[255].key = 0;
	tempvmask_sp.data[255].value = 0;
	LOADVMASKS_LOOP2: for (buffer_type i=0; i<transfsize; i++){ // transfsize, reducebuffersz
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount3 avg=analysis_loopcount3
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		tempvmask.data[0].key = bitsbuffer[i].range(0, 0);
		tempvmask.data[0].value = bitsbuffer[i].range(1, 1);
		tempvmask.data[1].key = bitsbuffer[i].range(2, 2);
		tempvmask.data[1].value = bitsbuffer[i].range(3, 3);
		tempvmask.data[2].key = bitsbuffer[i].range(4, 4);
		tempvmask.data[2].value = bitsbuffer[i].range(5, 5);
		tempvmask.data[3].key = bitsbuffer[i].range(6, 6);
		tempvmask.data[3].value = bitsbuffer[i].range(7, 7);
		tempvmask.data[4].key = bitsbuffer[i].range(8, 8);
		tempvmask.data[4].value = bitsbuffer[i].range(9, 9);
		tempvmask.data[5].key = bitsbuffer[i].range(10, 10);
		tempvmask.data[5].value = bitsbuffer[i].range(11, 11);
		tempvmask.data[6].key = bitsbuffer[i].range(12, 12);
		tempvmask.data[6].value = bitsbuffer[i].range(13, 13);
		tempvmask.data[7].key = bitsbuffer[i].range(14, 14);
		tempvmask.data[7].value = bitsbuffer[i].range(15, 15);
		tempvmask.data[8].key = bitsbuffer[i].range(16, 16);
		tempvmask.data[8].value = bitsbuffer[i].range(17, 17);
		tempvmask.data[9].key = bitsbuffer[i].range(18, 18);
		tempvmask.data[9].value = bitsbuffer[i].range(19, 19);
		tempvmask.data[10].key = bitsbuffer[i].range(20, 20);
		tempvmask.data[10].value = bitsbuffer[i].range(21, 21);
		tempvmask.data[11].key = bitsbuffer[i].range(22, 22);
		tempvmask.data[11].value = bitsbuffer[i].range(23, 23);
		tempvmask.data[12].key = bitsbuffer[i].range(24, 24);
		tempvmask.data[12].value = bitsbuffer[i].range(25, 25);
		tempvmask.data[13].key = bitsbuffer[i].range(26, 26);
		tempvmask.data[13].value = bitsbuffer[i].range(27, 27);
		tempvmask.data[14].key = bitsbuffer[i].range(28, 28);
		tempvmask.data[14].value = bitsbuffer[i].range(29, 29);
		tempvmask.data[15].key = bitsbuffer[i].range(30, 30);
		tempvmask.data[15].value = bitsbuffer[i].range(31, 31);
		tempvmask.data[16].key = bitsbuffer[i].range(32, 32);
		tempvmask.data[16].value = bitsbuffer[i].range(33, 33);
		tempvmask.data[17].key = bitsbuffer[i].range(34, 34);
		tempvmask.data[17].value = bitsbuffer[i].range(35, 35);
		tempvmask.data[18].key = bitsbuffer[i].range(36, 36);
		tempvmask.data[18].value = bitsbuffer[i].range(37, 37);
		tempvmask.data[19].key = bitsbuffer[i].range(38, 38);
		tempvmask.data[19].value = bitsbuffer[i].range(39, 39);
		tempvmask.data[20].key = bitsbuffer[i].range(40, 40);
		tempvmask.data[20].value = bitsbuffer[i].range(41, 41);
		tempvmask.data[21].key = bitsbuffer[i].range(42, 42);
		tempvmask.data[21].value = bitsbuffer[i].range(43, 43);
		tempvmask.data[22].key = bitsbuffer[i].range(44, 44);
		tempvmask.data[22].value = bitsbuffer[i].range(45, 45);
		tempvmask.data[23].key = bitsbuffer[i].range(46, 46);
		tempvmask.data[23].value = bitsbuffer[i].range(47, 47);
		tempvmask.data[24].key = bitsbuffer[i].range(48, 48);
		tempvmask.data[24].value = bitsbuffer[i].range(49, 49);
		tempvmask.data[25].key = bitsbuffer[i].range(50, 50);
		tempvmask.data[25].value = bitsbuffer[i].range(51, 51);
		tempvmask.data[26].key = bitsbuffer[i].range(52, 52);
		tempvmask.data[26].value = bitsbuffer[i].range(53, 53);
		tempvmask.data[27].key = bitsbuffer[i].range(54, 54);
		tempvmask.data[27].value = bitsbuffer[i].range(55, 55);
		tempvmask.data[28].key = bitsbuffer[i].range(56, 56);
		tempvmask.data[28].value = bitsbuffer[i].range(57, 57);
		tempvmask.data[29].key = bitsbuffer[i].range(58, 58);
		tempvmask.data[29].value = bitsbuffer[i].range(59, 59);
		tempvmask.data[30].key = bitsbuffer[i].range(60, 60);
		tempvmask.data[30].value = bitsbuffer[i].range(61, 61);
		tempvmask.data[31].key = bitsbuffer[i].range(62, 62);
		tempvmask.data[31].value = bitsbuffer[i].range(63, 63);
		tempvmask.data[32].key = bitsbuffer[i].range(64, 64);
		tempvmask.data[32].value = bitsbuffer[i].range(65, 65);
		tempvmask.data[33].key = bitsbuffer[i].range(66, 66);
		tempvmask.data[33].value = bitsbuffer[i].range(67, 67);
		tempvmask.data[34].key = bitsbuffer[i].range(68, 68);
		tempvmask.data[34].value = bitsbuffer[i].range(69, 69);
		tempvmask.data[35].key = bitsbuffer[i].range(70, 70);
		tempvmask.data[35].value = bitsbuffer[i].range(71, 71);
		tempvmask.data[36].key = bitsbuffer[i].range(72, 72);
		tempvmask.data[36].value = bitsbuffer[i].range(73, 73);
		tempvmask.data[37].key = bitsbuffer[i].range(74, 74);
		tempvmask.data[37].value = bitsbuffer[i].range(75, 75);
		tempvmask.data[38].key = bitsbuffer[i].range(76, 76);
		tempvmask.data[38].value = bitsbuffer[i].range(77, 77);
		tempvmask.data[39].key = bitsbuffer[i].range(78, 78);
		tempvmask.data[39].value = bitsbuffer[i].range(79, 79);
		tempvmask.data[40].key = bitsbuffer[i].range(80, 80);
		tempvmask.data[40].value = bitsbuffer[i].range(81, 81);
		tempvmask.data[41].key = bitsbuffer[i].range(82, 82);
		tempvmask.data[41].value = bitsbuffer[i].range(83, 83);
		tempvmask.data[42].key = bitsbuffer[i].range(84, 84);
		tempvmask.data[42].value = bitsbuffer[i].range(85, 85);
		tempvmask.data[43].key = bitsbuffer[i].range(86, 86);
		tempvmask.data[43].value = bitsbuffer[i].range(87, 87);
		tempvmask.data[44].key = bitsbuffer[i].range(88, 88);
		tempvmask.data[44].value = bitsbuffer[i].range(89, 89);
		tempvmask.data[45].key = bitsbuffer[i].range(90, 90);
		tempvmask.data[45].value = bitsbuffer[i].range(91, 91);
		tempvmask.data[46].key = bitsbuffer[i].range(92, 92);
		tempvmask.data[46].value = bitsbuffer[i].range(93, 93);
		tempvmask.data[47].key = bitsbuffer[i].range(94, 94);
		tempvmask.data[47].value = bitsbuffer[i].range(95, 95);
		tempvmask.data[48].key = bitsbuffer[i].range(96, 96);
		tempvmask.data[48].value = bitsbuffer[i].range(97, 97);
		tempvmask.data[49].key = bitsbuffer[i].range(98, 98);
		tempvmask.data[49].value = bitsbuffer[i].range(99, 99);
		tempvmask.data[50].key = bitsbuffer[i].range(100, 100);
		tempvmask.data[50].value = bitsbuffer[i].range(101, 101);
		tempvmask.data[51].key = bitsbuffer[i].range(102, 102);
		tempvmask.data[51].value = bitsbuffer[i].range(103, 103);
		tempvmask.data[52].key = bitsbuffer[i].range(104, 104);
		tempvmask.data[52].value = bitsbuffer[i].range(105, 105);
		tempvmask.data[53].key = bitsbuffer[i].range(106, 106);
		tempvmask.data[53].value = bitsbuffer[i].range(107, 107);
		tempvmask.data[54].key = bitsbuffer[i].range(108, 108);
		tempvmask.data[54].value = bitsbuffer[i].range(109, 109);
		tempvmask.data[55].key = bitsbuffer[i].range(110, 110);
		tempvmask.data[55].value = bitsbuffer[i].range(111, 111);
		tempvmask.data[56].key = bitsbuffer[i].range(112, 112);
		tempvmask.data[56].value = bitsbuffer[i].range(113, 113);
		tempvmask.data[57].key = bitsbuffer[i].range(114, 114);
		tempvmask.data[57].value = bitsbuffer[i].range(115, 115);
		tempvmask.data[58].key = bitsbuffer[i].range(116, 116);
		tempvmask.data[58].value = bitsbuffer[i].range(117, 117);
		tempvmask.data[59].key = bitsbuffer[i].range(118, 118);
		tempvmask.data[59].value = bitsbuffer[i].range(119, 119);
		tempvmask.data[60].key = bitsbuffer[i].range(120, 120);
		tempvmask.data[60].value = bitsbuffer[i].range(121, 121);
		tempvmask.data[61].key = bitsbuffer[i].range(122, 122);
		tempvmask.data[61].value = bitsbuffer[i].range(123, 123);
		tempvmask.data[62].key = bitsbuffer[i].range(124, 124);
		tempvmask.data[62].value = bitsbuffer[i].range(125, 125);
		tempvmask.data[63].key = bitsbuffer[i].range(126, 126);
		tempvmask.data[63].value = bitsbuffer[i].range(127, 127);
		tempvmask.data[64].key = bitsbuffer[i].range(128, 128);
		tempvmask.data[64].value = bitsbuffer[i].range(129, 129);
		tempvmask.data[65].key = bitsbuffer[i].range(130, 130);
		tempvmask.data[65].value = bitsbuffer[i].range(131, 131);
		tempvmask.data[66].key = bitsbuffer[i].range(132, 132);
		tempvmask.data[66].value = bitsbuffer[i].range(133, 133);
		tempvmask.data[67].key = bitsbuffer[i].range(134, 134);
		tempvmask.data[67].value = bitsbuffer[i].range(135, 135);
		tempvmask.data[68].key = bitsbuffer[i].range(136, 136);
		tempvmask.data[68].value = bitsbuffer[i].range(137, 137);
		tempvmask.data[69].key = bitsbuffer[i].range(138, 138);
		tempvmask.data[69].value = bitsbuffer[i].range(139, 139);
		tempvmask.data[70].key = bitsbuffer[i].range(140, 140);
		tempvmask.data[70].value = bitsbuffer[i].range(141, 141);
		tempvmask.data[71].key = bitsbuffer[i].range(142, 142);
		tempvmask.data[71].value = bitsbuffer[i].range(143, 143);
		tempvmask.data[72].key = bitsbuffer[i].range(144, 144);
		tempvmask.data[72].value = bitsbuffer[i].range(145, 145);
		tempvmask.data[73].key = bitsbuffer[i].range(146, 146);
		tempvmask.data[73].value = bitsbuffer[i].range(147, 147);
		tempvmask.data[74].key = bitsbuffer[i].range(148, 148);
		tempvmask.data[74].value = bitsbuffer[i].range(149, 149);
		tempvmask.data[75].key = bitsbuffer[i].range(150, 150);
		tempvmask.data[75].value = bitsbuffer[i].range(151, 151);
		tempvmask.data[76].key = bitsbuffer[i].range(152, 152);
		tempvmask.data[76].value = bitsbuffer[i].range(153, 153);
		tempvmask.data[77].key = bitsbuffer[i].range(154, 154);
		tempvmask.data[77].value = bitsbuffer[i].range(155, 155);
		tempvmask.data[78].key = bitsbuffer[i].range(156, 156);
		tempvmask.data[78].value = bitsbuffer[i].range(157, 157);
		tempvmask.data[79].key = bitsbuffer[i].range(158, 158);
		tempvmask.data[79].value = bitsbuffer[i].range(159, 159);
		tempvmask.data[80].key = bitsbuffer[i].range(160, 160);
		tempvmask.data[80].value = bitsbuffer[i].range(161, 161);
		tempvmask.data[81].key = bitsbuffer[i].range(162, 162);
		tempvmask.data[81].value = bitsbuffer[i].range(163, 163);
		tempvmask.data[82].key = bitsbuffer[i].range(164, 164);
		tempvmask.data[82].value = bitsbuffer[i].range(165, 165);
		tempvmask.data[83].key = bitsbuffer[i].range(166, 166);
		tempvmask.data[83].value = bitsbuffer[i].range(167, 167);
		tempvmask.data[84].key = bitsbuffer[i].range(168, 168);
		tempvmask.data[84].value = bitsbuffer[i].range(169, 169);
		tempvmask.data[85].key = bitsbuffer[i].range(170, 170);
		tempvmask.data[85].value = bitsbuffer[i].range(171, 171);
		tempvmask.data[86].key = bitsbuffer[i].range(172, 172);
		tempvmask.data[86].value = bitsbuffer[i].range(173, 173);
		tempvmask.data[87].key = bitsbuffer[i].range(174, 174);
		tempvmask.data[87].value = bitsbuffer[i].range(175, 175);
		tempvmask.data[88].key = bitsbuffer[i].range(176, 176);
		tempvmask.data[88].value = bitsbuffer[i].range(177, 177);
		tempvmask.data[89].key = bitsbuffer[i].range(178, 178);
		tempvmask.data[89].value = bitsbuffer[i].range(179, 179);
		tempvmask.data[90].key = bitsbuffer[i].range(180, 180);
		tempvmask.data[90].value = bitsbuffer[i].range(181, 181);
		tempvmask.data[91].key = bitsbuffer[i].range(182, 182);
		tempvmask.data[91].value = bitsbuffer[i].range(183, 183);
		tempvmask.data[92].key = bitsbuffer[i].range(184, 184);
		tempvmask.data[92].value = bitsbuffer[i].range(185, 185);
		tempvmask.data[93].key = bitsbuffer[i].range(186, 186);
		tempvmask.data[93].value = bitsbuffer[i].range(187, 187);
		tempvmask.data[94].key = bitsbuffer[i].range(188, 188);
		tempvmask.data[94].value = bitsbuffer[i].range(189, 189);
		tempvmask.data[95].key = bitsbuffer[i].range(190, 190);
		tempvmask.data[95].value = bitsbuffer[i].range(191, 191);
		tempvmask.data[96].key = bitsbuffer[i].range(192, 192);
		tempvmask.data[96].value = bitsbuffer[i].range(193, 193);
		tempvmask.data[97].key = bitsbuffer[i].range(194, 194);
		tempvmask.data[97].value = bitsbuffer[i].range(195, 195);
		tempvmask.data[98].key = bitsbuffer[i].range(196, 196);
		tempvmask.data[98].value = bitsbuffer[i].range(197, 197);
		tempvmask.data[99].key = bitsbuffer[i].range(198, 198);
		tempvmask.data[99].value = bitsbuffer[i].range(199, 199);
		tempvmask.data[100].key = bitsbuffer[i].range(200, 200);
		tempvmask.data[100].value = bitsbuffer[i].range(201, 201);
		tempvmask.data[101].key = bitsbuffer[i].range(202, 202);
		tempvmask.data[101].value = bitsbuffer[i].range(203, 203);
		tempvmask.data[102].key = bitsbuffer[i].range(204, 204);
		tempvmask.data[102].value = bitsbuffer[i].range(205, 205);
		tempvmask.data[103].key = bitsbuffer[i].range(206, 206);
		tempvmask.data[103].value = bitsbuffer[i].range(207, 207);
		tempvmask.data[104].key = bitsbuffer[i].range(208, 208);
		tempvmask.data[104].value = bitsbuffer[i].range(209, 209);
		tempvmask.data[105].key = bitsbuffer[i].range(210, 210);
		tempvmask.data[105].value = bitsbuffer[i].range(211, 211);
		tempvmask.data[106].key = bitsbuffer[i].range(212, 212);
		tempvmask.data[106].value = bitsbuffer[i].range(213, 213);
		tempvmask.data[107].key = bitsbuffer[i].range(214, 214);
		tempvmask.data[107].value = bitsbuffer[i].range(215, 215);
		tempvmask.data[108].key = bitsbuffer[i].range(216, 216);
		tempvmask.data[108].value = bitsbuffer[i].range(217, 217);
		tempvmask.data[109].key = bitsbuffer[i].range(218, 218);
		tempvmask.data[109].value = bitsbuffer[i].range(219, 219);
		tempvmask.data[110].key = bitsbuffer[i].range(220, 220);
		tempvmask.data[110].value = bitsbuffer[i].range(221, 221);
		tempvmask.data[111].key = bitsbuffer[i].range(222, 222);
		tempvmask.data[111].value = bitsbuffer[i].range(223, 223);
		tempvmask.data[112].key = bitsbuffer[i].range(224, 224);
		tempvmask.data[112].value = bitsbuffer[i].range(225, 225);
		tempvmask.data[113].key = bitsbuffer[i].range(226, 226);
		tempvmask.data[113].value = bitsbuffer[i].range(227, 227);
		tempvmask.data[114].key = bitsbuffer[i].range(228, 228);
		tempvmask.data[114].value = bitsbuffer[i].range(229, 229);
		tempvmask.data[115].key = bitsbuffer[i].range(230, 230);
		tempvmask.data[115].value = bitsbuffer[i].range(231, 231);
		tempvmask.data[116].key = bitsbuffer[i].range(232, 232);
		tempvmask.data[116].value = bitsbuffer[i].range(233, 233);
		tempvmask.data[117].key = bitsbuffer[i].range(234, 234);
		tempvmask.data[117].value = bitsbuffer[i].range(235, 235);
		tempvmask.data[118].key = bitsbuffer[i].range(236, 236);
		tempvmask.data[118].value = bitsbuffer[i].range(237, 237);
		tempvmask.data[119].key = bitsbuffer[i].range(238, 238);
		tempvmask.data[119].value = bitsbuffer[i].range(239, 239);
		tempvmask.data[120].key = bitsbuffer[i].range(240, 240);
		tempvmask.data[120].value = bitsbuffer[i].range(241, 241);
		tempvmask.data[121].key = bitsbuffer[i].range(242, 242);
		tempvmask.data[121].value = bitsbuffer[i].range(243, 243);
		tempvmask.data[122].key = bitsbuffer[i].range(244, 244);
		tempvmask.data[122].value = bitsbuffer[i].range(245, 245);
		tempvmask.data[123].key = bitsbuffer[i].range(246, 246);
		tempvmask.data[123].value = bitsbuffer[i].range(247, 247);
		tempvmask.data[124].key = bitsbuffer[i].range(248, 248);
		tempvmask.data[124].value = bitsbuffer[i].range(249, 249);
		tempvmask.data[125].key = bitsbuffer[i].range(250, 250);
		tempvmask.data[125].value = bitsbuffer[i].range(251, 251);
		tempvmask.data[126].key = bitsbuffer[i].range(252, 252);
		tempvmask.data[126].value = bitsbuffer[i].range(253, 253);
		tempvmask.data[127].key = bitsbuffer[i].range(254, 254);
		tempvmask.data[127].value = bitsbuffer[i].range(255, 255);
		tempvmask.data[128].key = bitsbuffer[i].range(256, 256);
		tempvmask.data[128].value = bitsbuffer[i].range(257, 257);
		tempvmask.data[129].key = bitsbuffer[i].range(258, 258);
		tempvmask.data[129].value = bitsbuffer[i].range(259, 259);
		tempvmask.data[130].key = bitsbuffer[i].range(260, 260);
		tempvmask.data[130].value = bitsbuffer[i].range(261, 261);
		tempvmask.data[131].key = bitsbuffer[i].range(262, 262);
		tempvmask.data[131].value = bitsbuffer[i].range(263, 263);
		tempvmask.data[132].key = bitsbuffer[i].range(264, 264);
		tempvmask.data[132].value = bitsbuffer[i].range(265, 265);
		tempvmask.data[133].key = bitsbuffer[i].range(266, 266);
		tempvmask.data[133].value = bitsbuffer[i].range(267, 267);
		tempvmask.data[134].key = bitsbuffer[i].range(268, 268);
		tempvmask.data[134].value = bitsbuffer[i].range(269, 269);
		tempvmask.data[135].key = bitsbuffer[i].range(270, 270);
		tempvmask.data[135].value = bitsbuffer[i].range(271, 271);
		tempvmask.data[136].key = bitsbuffer[i].range(272, 272);
		tempvmask.data[136].value = bitsbuffer[i].range(273, 273);
		tempvmask.data[137].key = bitsbuffer[i].range(274, 274);
		tempvmask.data[137].value = bitsbuffer[i].range(275, 275);
		tempvmask.data[138].key = bitsbuffer[i].range(276, 276);
		tempvmask.data[138].value = bitsbuffer[i].range(277, 277);
		tempvmask.data[139].key = bitsbuffer[i].range(278, 278);
		tempvmask.data[139].value = bitsbuffer[i].range(279, 279);
		tempvmask.data[140].key = bitsbuffer[i].range(280, 280);
		tempvmask.data[140].value = bitsbuffer[i].range(281, 281);
		tempvmask.data[141].key = bitsbuffer[i].range(282, 282);
		tempvmask.data[141].value = bitsbuffer[i].range(283, 283);
		tempvmask.data[142].key = bitsbuffer[i].range(284, 284);
		tempvmask.data[142].value = bitsbuffer[i].range(285, 285);
		tempvmask.data[143].key = bitsbuffer[i].range(286, 286);
		tempvmask.data[143].value = bitsbuffer[i].range(287, 287);
		tempvmask.data[144].key = bitsbuffer[i].range(288, 288);
		tempvmask.data[144].value = bitsbuffer[i].range(289, 289);
		tempvmask.data[145].key = bitsbuffer[i].range(290, 290);
		tempvmask.data[145].value = bitsbuffer[i].range(291, 291);
		tempvmask.data[146].key = bitsbuffer[i].range(292, 292);
		tempvmask.data[146].value = bitsbuffer[i].range(293, 293);
		tempvmask.data[147].key = bitsbuffer[i].range(294, 294);
		tempvmask.data[147].value = bitsbuffer[i].range(295, 295);
		tempvmask.data[148].key = bitsbuffer[i].range(296, 296);
		tempvmask.data[148].value = bitsbuffer[i].range(297, 297);
		tempvmask.data[149].key = bitsbuffer[i].range(298, 298);
		tempvmask.data[149].value = bitsbuffer[i].range(299, 299);
		tempvmask.data[150].key = bitsbuffer[i].range(300, 300);
		tempvmask.data[150].value = bitsbuffer[i].range(301, 301);
		tempvmask.data[151].key = bitsbuffer[i].range(302, 302);
		tempvmask.data[151].value = bitsbuffer[i].range(303, 303);
		tempvmask.data[152].key = bitsbuffer[i].range(304, 304);
		tempvmask.data[152].value = bitsbuffer[i].range(305, 305);
		tempvmask.data[153].key = bitsbuffer[i].range(306, 306);
		tempvmask.data[153].value = bitsbuffer[i].range(307, 307);
		tempvmask.data[154].key = bitsbuffer[i].range(308, 308);
		tempvmask.data[154].value = bitsbuffer[i].range(309, 309);
		tempvmask.data[155].key = bitsbuffer[i].range(310, 310);
		tempvmask.data[155].value = bitsbuffer[i].range(311, 311);
		tempvmask.data[156].key = bitsbuffer[i].range(312, 312);
		tempvmask.data[156].value = bitsbuffer[i].range(313, 313);
		tempvmask.data[157].key = bitsbuffer[i].range(314, 314);
		tempvmask.data[157].value = bitsbuffer[i].range(315, 315);
		tempvmask.data[158].key = bitsbuffer[i].range(316, 316);
		tempvmask.data[158].value = bitsbuffer[i].range(317, 317);
		tempvmask.data[159].key = bitsbuffer[i].range(318, 318);
		tempvmask.data[159].value = bitsbuffer[i].range(319, 319);
		tempvmask.data[160].key = bitsbuffer[i].range(320, 320);
		tempvmask.data[160].value = bitsbuffer[i].range(321, 321);
		tempvmask.data[161].key = bitsbuffer[i].range(322, 322);
		tempvmask.data[161].value = bitsbuffer[i].range(323, 323);
		tempvmask.data[162].key = bitsbuffer[i].range(324, 324);
		tempvmask.data[162].value = bitsbuffer[i].range(325, 325);
		tempvmask.data[163].key = bitsbuffer[i].range(326, 326);
		tempvmask.data[163].value = bitsbuffer[i].range(327, 327);
		tempvmask.data[164].key = bitsbuffer[i].range(328, 328);
		tempvmask.data[164].value = bitsbuffer[i].range(329, 329);
		tempvmask.data[165].key = bitsbuffer[i].range(330, 330);
		tempvmask.data[165].value = bitsbuffer[i].range(331, 331);
		tempvmask.data[166].key = bitsbuffer[i].range(332, 332);
		tempvmask.data[166].value = bitsbuffer[i].range(333, 333);
		tempvmask.data[167].key = bitsbuffer[i].range(334, 334);
		tempvmask.data[167].value = bitsbuffer[i].range(335, 335);
		tempvmask.data[168].key = bitsbuffer[i].range(336, 336);
		tempvmask.data[168].value = bitsbuffer[i].range(337, 337);
		tempvmask.data[169].key = bitsbuffer[i].range(338, 338);
		tempvmask.data[169].value = bitsbuffer[i].range(339, 339);
		tempvmask.data[170].key = bitsbuffer[i].range(340, 340);
		tempvmask.data[170].value = bitsbuffer[i].range(341, 341);
		tempvmask.data[171].key = bitsbuffer[i].range(342, 342);
		tempvmask.data[171].value = bitsbuffer[i].range(343, 343);
		tempvmask.data[172].key = bitsbuffer[i].range(344, 344);
		tempvmask.data[172].value = bitsbuffer[i].range(345, 345);
		tempvmask.data[173].key = bitsbuffer[i].range(346, 346);
		tempvmask.data[173].value = bitsbuffer[i].range(347, 347);
		tempvmask.data[174].key = bitsbuffer[i].range(348, 348);
		tempvmask.data[174].value = bitsbuffer[i].range(349, 349);
		tempvmask.data[175].key = bitsbuffer[i].range(350, 350);
		tempvmask.data[175].value = bitsbuffer[i].range(351, 351);
		tempvmask.data[176].key = bitsbuffer[i].range(352, 352);
		tempvmask.data[176].value = bitsbuffer[i].range(353, 353);
		tempvmask.data[177].key = bitsbuffer[i].range(354, 354);
		tempvmask.data[177].value = bitsbuffer[i].range(355, 355);
		tempvmask.data[178].key = bitsbuffer[i].range(356, 356);
		tempvmask.data[178].value = bitsbuffer[i].range(357, 357);
		tempvmask.data[179].key = bitsbuffer[i].range(358, 358);
		tempvmask.data[179].value = bitsbuffer[i].range(359, 359);
		tempvmask.data[180].key = bitsbuffer[i].range(360, 360);
		tempvmask.data[180].value = bitsbuffer[i].range(361, 361);
		tempvmask.data[181].key = bitsbuffer[i].range(362, 362);
		tempvmask.data[181].value = bitsbuffer[i].range(363, 363);
		tempvmask.data[182].key = bitsbuffer[i].range(364, 364);
		tempvmask.data[182].value = bitsbuffer[i].range(365, 365);
		tempvmask.data[183].key = bitsbuffer[i].range(366, 366);
		tempvmask.data[183].value = bitsbuffer[i].range(367, 367);
		tempvmask.data[184].key = bitsbuffer[i].range(368, 368);
		tempvmask.data[184].value = bitsbuffer[i].range(369, 369);
		tempvmask.data[185].key = bitsbuffer[i].range(370, 370);
		tempvmask.data[185].value = bitsbuffer[i].range(371, 371);
		tempvmask.data[186].key = bitsbuffer[i].range(372, 372);
		tempvmask.data[186].value = bitsbuffer[i].range(373, 373);
		tempvmask.data[187].key = bitsbuffer[i].range(374, 374);
		tempvmask.data[187].value = bitsbuffer[i].range(375, 375);
		tempvmask.data[188].key = bitsbuffer[i].range(376, 376);
		tempvmask.data[188].value = bitsbuffer[i].range(377, 377);
		tempvmask.data[189].key = bitsbuffer[i].range(378, 378);
		tempvmask.data[189].value = bitsbuffer[i].range(379, 379);
		tempvmask.data[190].key = bitsbuffer[i].range(380, 380);
		tempvmask.data[190].value = bitsbuffer[i].range(381, 381);
		tempvmask.data[191].key = bitsbuffer[i].range(382, 382);
		tempvmask.data[191].value = bitsbuffer[i].range(383, 383);
		tempvmask.data[192].key = bitsbuffer[i].range(384, 384);
		tempvmask.data[192].value = bitsbuffer[i].range(385, 385);
		tempvmask.data[193].key = bitsbuffer[i].range(386, 386);
		tempvmask.data[193].value = bitsbuffer[i].range(387, 387);
		tempvmask.data[194].key = bitsbuffer[i].range(388, 388);
		tempvmask.data[194].value = bitsbuffer[i].range(389, 389);
		tempvmask.data[195].key = bitsbuffer[i].range(390, 390);
		tempvmask.data[195].value = bitsbuffer[i].range(391, 391);
		tempvmask.data[196].key = bitsbuffer[i].range(392, 392);
		tempvmask.data[196].value = bitsbuffer[i].range(393, 393);
		tempvmask.data[197].key = bitsbuffer[i].range(394, 394);
		tempvmask.data[197].value = bitsbuffer[i].range(395, 395);
		tempvmask.data[198].key = bitsbuffer[i].range(396, 396);
		tempvmask.data[198].value = bitsbuffer[i].range(397, 397);
		tempvmask.data[199].key = bitsbuffer[i].range(398, 398);
		tempvmask.data[199].value = bitsbuffer[i].range(399, 399);
		tempvmask.data[200].key = bitsbuffer[i].range(400, 400);
		tempvmask.data[200].value = bitsbuffer[i].range(401, 401);
		tempvmask.data[201].key = bitsbuffer[i].range(402, 402);
		tempvmask.data[201].value = bitsbuffer[i].range(403, 403);
		tempvmask.data[202].key = bitsbuffer[i].range(404, 404);
		tempvmask.data[202].value = bitsbuffer[i].range(405, 405);
		tempvmask.data[203].key = bitsbuffer[i].range(406, 406);
		tempvmask.data[203].value = bitsbuffer[i].range(407, 407);
		tempvmask.data[204].key = bitsbuffer[i].range(408, 408);
		tempvmask.data[204].value = bitsbuffer[i].range(409, 409);
		tempvmask.data[205].key = bitsbuffer[i].range(410, 410);
		tempvmask.data[205].value = bitsbuffer[i].range(411, 411);
		tempvmask.data[206].key = bitsbuffer[i].range(412, 412);
		tempvmask.data[206].value = bitsbuffer[i].range(413, 413);
		tempvmask.data[207].key = bitsbuffer[i].range(414, 414);
		tempvmask.data[207].value = bitsbuffer[i].range(415, 415);
		tempvmask.data[208].key = bitsbuffer[i].range(416, 416);
		tempvmask.data[208].value = bitsbuffer[i].range(417, 417);
		tempvmask.data[209].key = bitsbuffer[i].range(418, 418);
		tempvmask.data[209].value = bitsbuffer[i].range(419, 419);
		tempvmask.data[210].key = bitsbuffer[i].range(420, 420);
		tempvmask.data[210].value = bitsbuffer[i].range(421, 421);
		tempvmask.data[211].key = bitsbuffer[i].range(422, 422);
		tempvmask.data[211].value = bitsbuffer[i].range(423, 423);
		tempvmask.data[212].key = bitsbuffer[i].range(424, 424);
		tempvmask.data[212].value = bitsbuffer[i].range(425, 425);
		tempvmask.data[213].key = bitsbuffer[i].range(426, 426);
		tempvmask.data[213].value = bitsbuffer[i].range(427, 427);
		tempvmask.data[214].key = bitsbuffer[i].range(428, 428);
		tempvmask.data[214].value = bitsbuffer[i].range(429, 429);
		tempvmask.data[215].key = bitsbuffer[i].range(430, 430);
		tempvmask.data[215].value = bitsbuffer[i].range(431, 431);
		tempvmask.data[216].key = bitsbuffer[i].range(432, 432);
		tempvmask.data[216].value = bitsbuffer[i].range(433, 433);
		tempvmask.data[217].key = bitsbuffer[i].range(434, 434);
		tempvmask.data[217].value = bitsbuffer[i].range(435, 435);
		tempvmask.data[218].key = bitsbuffer[i].range(436, 436);
		tempvmask.data[218].value = bitsbuffer[i].range(437, 437);
		tempvmask.data[219].key = bitsbuffer[i].range(438, 438);
		tempvmask.data[219].value = bitsbuffer[i].range(439, 439);
		tempvmask.data[220].key = bitsbuffer[i].range(440, 440);
		tempvmask.data[220].value = bitsbuffer[i].range(441, 441);
		tempvmask.data[221].key = bitsbuffer[i].range(442, 442);
		tempvmask.data[221].value = bitsbuffer[i].range(443, 443);
		tempvmask.data[222].key = bitsbuffer[i].range(444, 444);
		tempvmask.data[222].value = bitsbuffer[i].range(445, 445);
		tempvmask.data[223].key = bitsbuffer[i].range(446, 446);
		tempvmask.data[223].value = bitsbuffer[i].range(447, 447);
		tempvmask.data[224].key = bitsbuffer[i].range(448, 448);
		tempvmask.data[224].value = bitsbuffer[i].range(449, 449);
		tempvmask.data[225].key = bitsbuffer[i].range(450, 450);
		tempvmask.data[225].value = bitsbuffer[i].range(451, 451);
		tempvmask.data[226].key = bitsbuffer[i].range(452, 452);
		tempvmask.data[226].value = bitsbuffer[i].range(453, 453);
		tempvmask.data[227].key = bitsbuffer[i].range(454, 454);
		tempvmask.data[227].value = bitsbuffer[i].range(455, 455);
		tempvmask.data[228].key = bitsbuffer[i].range(456, 456);
		tempvmask.data[228].value = bitsbuffer[i].range(457, 457);
		tempvmask.data[229].key = bitsbuffer[i].range(458, 458);
		tempvmask.data[229].value = bitsbuffer[i].range(459, 459);
		tempvmask.data[230].key = bitsbuffer[i].range(460, 460);
		tempvmask.data[230].value = bitsbuffer[i].range(461, 461);
		tempvmask.data[231].key = bitsbuffer[i].range(462, 462);
		tempvmask.data[231].value = bitsbuffer[i].range(463, 463);
		tempvmask.data[232].key = bitsbuffer[i].range(464, 464);
		tempvmask.data[232].value = bitsbuffer[i].range(465, 465);
		tempvmask.data[233].key = bitsbuffer[i].range(466, 466);
		tempvmask.data[233].value = bitsbuffer[i].range(467, 467);
		tempvmask.data[234].key = bitsbuffer[i].range(468, 468);
		tempvmask.data[234].value = bitsbuffer[i].range(469, 469);
		tempvmask.data[235].key = bitsbuffer[i].range(470, 470);
		tempvmask.data[235].value = bitsbuffer[i].range(471, 471);
		tempvmask.data[236].key = bitsbuffer[i].range(472, 472);
		tempvmask.data[236].value = bitsbuffer[i].range(473, 473);
		tempvmask.data[237].key = bitsbuffer[i].range(474, 474);
		tempvmask.data[237].value = bitsbuffer[i].range(475, 475);
		tempvmask.data[238].key = bitsbuffer[i].range(476, 476);
		tempvmask.data[238].value = bitsbuffer[i].range(477, 477);
		tempvmask.data[239].key = bitsbuffer[i].range(478, 478);
		tempvmask.data[239].value = bitsbuffer[i].range(479, 479);
		tempvmask.data[240].key = bitsbuffer[i].range(480, 480);
		tempvmask.data[240].value = bitsbuffer[i].range(481, 481);
		tempvmask.data[241].key = bitsbuffer[i].range(482, 482);
		tempvmask.data[241].value = bitsbuffer[i].range(483, 483);
		tempvmask.data[242].key = bitsbuffer[i].range(484, 484);
		tempvmask.data[242].value = bitsbuffer[i].range(485, 485);
		tempvmask.data[243].key = bitsbuffer[i].range(486, 486);
		tempvmask.data[243].value = bitsbuffer[i].range(487, 487);
		tempvmask.data[244].key = bitsbuffer[i].range(488, 488);
		tempvmask.data[244].value = bitsbuffer[i].range(489, 489);
		tempvmask.data[245].key = bitsbuffer[i].range(490, 490);
		tempvmask.data[245].value = bitsbuffer[i].range(491, 491);
		tempvmask.data[246].key = bitsbuffer[i].range(492, 492);
		tempvmask.data[246].value = bitsbuffer[i].range(493, 493);
		tempvmask.data[247].key = bitsbuffer[i].range(494, 494);
		tempvmask.data[247].value = bitsbuffer[i].range(495, 495);
		tempvmask.data[248].key = bitsbuffer[i].range(496, 496);
		tempvmask.data[248].value = bitsbuffer[i].range(497, 497);
		tempvmask.data[249].key = bitsbuffer[i].range(498, 498);
		tempvmask.data[249].value = bitsbuffer[i].range(499, 499);
		tempvmask.data[250].key = bitsbuffer[i].range(500, 500);
		tempvmask.data[250].value = bitsbuffer[i].range(501, 501);
		tempvmask.data[251].key = bitsbuffer[i].range(502, 502);
		tempvmask.data[251].value = bitsbuffer[i].range(503, 503);
		tempvmask.data[252].key = bitsbuffer[i].range(504, 504);
		tempvmask.data[252].value = bitsbuffer[i].range(505, 505);
		tempvmask.data[253].key = bitsbuffer[i].range(506, 506);
		tempvmask.data[253].value = bitsbuffer[i].range(507, 507);
		tempvmask.data[254].key = bitsbuffer[i].range(508, 508);
		tempvmask.data[254].value = bitsbuffer[i].range(509, 509);
		tempvmask.data[255].key = bitsbuffer[i].range(510, 510);
		tempvmask.data[255].value = bitsbuffer[i].range(511, 511);
		#else 
		tempvmask.data[0].key = READFROM_UINT(bitsbuffer[i], 0, 1);
		tempvmask.data[0].value = READFROM_UINT(bitsbuffer[i], 1, 1);
		tempvmask.data[1].key = READFROM_UINT(bitsbuffer[i], 2, 1);
		tempvmask.data[1].value = READFROM_UINT(bitsbuffer[i], 3, 1);
		tempvmask.data[2].key = READFROM_UINT(bitsbuffer[i], 4, 1);
		tempvmask.data[2].value = READFROM_UINT(bitsbuffer[i], 5, 1);
		tempvmask.data[3].key = READFROM_UINT(bitsbuffer[i], 6, 1);
		tempvmask.data[3].value = READFROM_UINT(bitsbuffer[i], 7, 1);
		tempvmask.data[4].key = READFROM_UINT(bitsbuffer[i], 8, 1);
		tempvmask.data[4].value = READFROM_UINT(bitsbuffer[i], 9, 1);
		tempvmask.data[5].key = READFROM_UINT(bitsbuffer[i], 10, 1);
		tempvmask.data[5].value = READFROM_UINT(bitsbuffer[i], 11, 1);
		tempvmask.data[6].key = READFROM_UINT(bitsbuffer[i], 12, 1);
		tempvmask.data[6].value = READFROM_UINT(bitsbuffer[i], 13, 1);
		tempvmask.data[7].key = READFROM_UINT(bitsbuffer[i], 14, 1);
		tempvmask.data[7].value = READFROM_UINT(bitsbuffer[i], 15, 1);
		tempvmask.data[8].key = READFROM_UINT(bitsbuffer[i], 16, 1);
		tempvmask.data[8].value = READFROM_UINT(bitsbuffer[i], 17, 1);
		tempvmask.data[9].key = READFROM_UINT(bitsbuffer[i], 18, 1);
		tempvmask.data[9].value = READFROM_UINT(bitsbuffer[i], 19, 1);
		tempvmask.data[10].key = READFROM_UINT(bitsbuffer[i], 20, 1);
		tempvmask.data[10].value = READFROM_UINT(bitsbuffer[i], 21, 1);
		tempvmask.data[11].key = READFROM_UINT(bitsbuffer[i], 22, 1);
		tempvmask.data[11].value = READFROM_UINT(bitsbuffer[i], 23, 1);
		tempvmask.data[12].key = READFROM_UINT(bitsbuffer[i], 24, 1);
		tempvmask.data[12].value = READFROM_UINT(bitsbuffer[i], 25, 1);
		tempvmask.data[13].key = READFROM_UINT(bitsbuffer[i], 26, 1);
		tempvmask.data[13].value = READFROM_UINT(bitsbuffer[i], 27, 1);
		tempvmask.data[14].key = READFROM_UINT(bitsbuffer[i], 28, 1);
		tempvmask.data[14].value = READFROM_UINT(bitsbuffer[i], 29, 1);
		tempvmask.data[15].key = READFROM_UINT(bitsbuffer[i], 30, 1);
		tempvmask.data[15].value = READFROM_UINT(bitsbuffer[i], 31, 1);
		tempvmask.data[16].key = READFROM_UINT(bitsbuffer[i], 32, 1);
		tempvmask.data[16].value = READFROM_UINT(bitsbuffer[i], 33, 1);
		tempvmask.data[17].key = READFROM_UINT(bitsbuffer[i], 34, 1);
		tempvmask.data[17].value = READFROM_UINT(bitsbuffer[i], 35, 1);
		tempvmask.data[18].key = READFROM_UINT(bitsbuffer[i], 36, 1);
		tempvmask.data[18].value = READFROM_UINT(bitsbuffer[i], 37, 1);
		tempvmask.data[19].key = READFROM_UINT(bitsbuffer[i], 38, 1);
		tempvmask.data[19].value = READFROM_UINT(bitsbuffer[i], 39, 1);
		tempvmask.data[20].key = READFROM_UINT(bitsbuffer[i], 40, 1);
		tempvmask.data[20].value = READFROM_UINT(bitsbuffer[i], 41, 1);
		tempvmask.data[21].key = READFROM_UINT(bitsbuffer[i], 42, 1);
		tempvmask.data[21].value = READFROM_UINT(bitsbuffer[i], 43, 1);
		tempvmask.data[22].key = READFROM_UINT(bitsbuffer[i], 44, 1);
		tempvmask.data[22].value = READFROM_UINT(bitsbuffer[i], 45, 1);
		tempvmask.data[23].key = READFROM_UINT(bitsbuffer[i], 46, 1);
		tempvmask.data[23].value = READFROM_UINT(bitsbuffer[i], 47, 1);
		tempvmask.data[24].key = READFROM_UINT(bitsbuffer[i], 48, 1);
		tempvmask.data[24].value = READFROM_UINT(bitsbuffer[i], 49, 1);
		tempvmask.data[25].key = READFROM_UINT(bitsbuffer[i], 50, 1);
		tempvmask.data[25].value = READFROM_UINT(bitsbuffer[i], 51, 1);
		tempvmask.data[26].key = READFROM_UINT(bitsbuffer[i], 52, 1);
		tempvmask.data[26].value = READFROM_UINT(bitsbuffer[i], 53, 1);
		tempvmask.data[27].key = READFROM_UINT(bitsbuffer[i], 54, 1);
		tempvmask.data[27].value = READFROM_UINT(bitsbuffer[i], 55, 1);
		tempvmask.data[28].key = READFROM_UINT(bitsbuffer[i], 56, 1);
		tempvmask.data[28].value = READFROM_UINT(bitsbuffer[i], 57, 1);
		tempvmask.data[29].key = READFROM_UINT(bitsbuffer[i], 58, 1);
		tempvmask.data[29].value = READFROM_UINT(bitsbuffer[i], 59, 1);
		tempvmask.data[30].key = READFROM_UINT(bitsbuffer[i], 60, 1);
		tempvmask.data[30].value = READFROM_UINT(bitsbuffer[i], 61, 1);
		tempvmask.data[31].key = READFROM_UINT(bitsbuffer[i], 62, 1);
		tempvmask.data[31].value = READFROM_UINT(bitsbuffer[i], 63, 1);
		tempvmask.data[32].key = READFROM_UINT(bitsbuffer[i], 64, 1);
		tempvmask.data[32].value = READFROM_UINT(bitsbuffer[i], 65, 1);
		tempvmask.data[33].key = READFROM_UINT(bitsbuffer[i], 66, 1);
		tempvmask.data[33].value = READFROM_UINT(bitsbuffer[i], 67, 1);
		tempvmask.data[34].key = READFROM_UINT(bitsbuffer[i], 68, 1);
		tempvmask.data[34].value = READFROM_UINT(bitsbuffer[i], 69, 1);
		tempvmask.data[35].key = READFROM_UINT(bitsbuffer[i], 70, 1);
		tempvmask.data[35].value = READFROM_UINT(bitsbuffer[i], 71, 1);
		tempvmask.data[36].key = READFROM_UINT(bitsbuffer[i], 72, 1);
		tempvmask.data[36].value = READFROM_UINT(bitsbuffer[i], 73, 1);
		tempvmask.data[37].key = READFROM_UINT(bitsbuffer[i], 74, 1);
		tempvmask.data[37].value = READFROM_UINT(bitsbuffer[i], 75, 1);
		tempvmask.data[38].key = READFROM_UINT(bitsbuffer[i], 76, 1);
		tempvmask.data[38].value = READFROM_UINT(bitsbuffer[i], 77, 1);
		tempvmask.data[39].key = READFROM_UINT(bitsbuffer[i], 78, 1);
		tempvmask.data[39].value = READFROM_UINT(bitsbuffer[i], 79, 1);
		tempvmask.data[40].key = READFROM_UINT(bitsbuffer[i], 80, 1);
		tempvmask.data[40].value = READFROM_UINT(bitsbuffer[i], 81, 1);
		tempvmask.data[41].key = READFROM_UINT(bitsbuffer[i], 82, 1);
		tempvmask.data[41].value = READFROM_UINT(bitsbuffer[i], 83, 1);
		tempvmask.data[42].key = READFROM_UINT(bitsbuffer[i], 84, 1);
		tempvmask.data[42].value = READFROM_UINT(bitsbuffer[i], 85, 1);
		tempvmask.data[43].key = READFROM_UINT(bitsbuffer[i], 86, 1);
		tempvmask.data[43].value = READFROM_UINT(bitsbuffer[i], 87, 1);
		tempvmask.data[44].key = READFROM_UINT(bitsbuffer[i], 88, 1);
		tempvmask.data[44].value = READFROM_UINT(bitsbuffer[i], 89, 1);
		tempvmask.data[45].key = READFROM_UINT(bitsbuffer[i], 90, 1);
		tempvmask.data[45].value = READFROM_UINT(bitsbuffer[i], 91, 1);
		tempvmask.data[46].key = READFROM_UINT(bitsbuffer[i], 92, 1);
		tempvmask.data[46].value = READFROM_UINT(bitsbuffer[i], 93, 1);
		tempvmask.data[47].key = READFROM_UINT(bitsbuffer[i], 94, 1);
		tempvmask.data[47].value = READFROM_UINT(bitsbuffer[i], 95, 1);
		tempvmask.data[48].key = READFROM_UINT(bitsbuffer[i], 96, 1);
		tempvmask.data[48].value = READFROM_UINT(bitsbuffer[i], 97, 1);
		tempvmask.data[49].key = READFROM_UINT(bitsbuffer[i], 98, 1);
		tempvmask.data[49].value = READFROM_UINT(bitsbuffer[i], 99, 1);
		tempvmask.data[50].key = READFROM_UINT(bitsbuffer[i], 100, 1);
		tempvmask.data[50].value = READFROM_UINT(bitsbuffer[i], 101, 1);
		tempvmask.data[51].key = READFROM_UINT(bitsbuffer[i], 102, 1);
		tempvmask.data[51].value = READFROM_UINT(bitsbuffer[i], 103, 1);
		tempvmask.data[52].key = READFROM_UINT(bitsbuffer[i], 104, 1);
		tempvmask.data[52].value = READFROM_UINT(bitsbuffer[i], 105, 1);
		tempvmask.data[53].key = READFROM_UINT(bitsbuffer[i], 106, 1);
		tempvmask.data[53].value = READFROM_UINT(bitsbuffer[i], 107, 1);
		tempvmask.data[54].key = READFROM_UINT(bitsbuffer[i], 108, 1);
		tempvmask.data[54].value = READFROM_UINT(bitsbuffer[i], 109, 1);
		tempvmask.data[55].key = READFROM_UINT(bitsbuffer[i], 110, 1);
		tempvmask.data[55].value = READFROM_UINT(bitsbuffer[i], 111, 1);
		tempvmask.data[56].key = READFROM_UINT(bitsbuffer[i], 112, 1);
		tempvmask.data[56].value = READFROM_UINT(bitsbuffer[i], 113, 1);
		tempvmask.data[57].key = READFROM_UINT(bitsbuffer[i], 114, 1);
		tempvmask.data[57].value = READFROM_UINT(bitsbuffer[i], 115, 1);
		tempvmask.data[58].key = READFROM_UINT(bitsbuffer[i], 116, 1);
		tempvmask.data[58].value = READFROM_UINT(bitsbuffer[i], 117, 1);
		tempvmask.data[59].key = READFROM_UINT(bitsbuffer[i], 118, 1);
		tempvmask.data[59].value = READFROM_UINT(bitsbuffer[i], 119, 1);
		tempvmask.data[60].key = READFROM_UINT(bitsbuffer[i], 120, 1);
		tempvmask.data[60].value = READFROM_UINT(bitsbuffer[i], 121, 1);
		tempvmask.data[61].key = READFROM_UINT(bitsbuffer[i], 122, 1);
		tempvmask.data[61].value = READFROM_UINT(bitsbuffer[i], 123, 1);
		tempvmask.data[62].key = READFROM_UINT(bitsbuffer[i], 124, 1);
		tempvmask.data[62].value = READFROM_UINT(bitsbuffer[i], 125, 1);
		tempvmask.data[63].key = READFROM_UINT(bitsbuffer[i], 126, 1);
		tempvmask.data[63].value = READFROM_UINT(bitsbuffer[i], 127, 1);
		tempvmask.data[64].key = READFROM_UINT(bitsbuffer[i], 128, 1);
		tempvmask.data[64].value = READFROM_UINT(bitsbuffer[i], 129, 1);
		tempvmask.data[65].key = READFROM_UINT(bitsbuffer[i], 130, 1);
		tempvmask.data[65].value = READFROM_UINT(bitsbuffer[i], 131, 1);
		tempvmask.data[66].key = READFROM_UINT(bitsbuffer[i], 132, 1);
		tempvmask.data[66].value = READFROM_UINT(bitsbuffer[i], 133, 1);
		tempvmask.data[67].key = READFROM_UINT(bitsbuffer[i], 134, 1);
		tempvmask.data[67].value = READFROM_UINT(bitsbuffer[i], 135, 1);
		tempvmask.data[68].key = READFROM_UINT(bitsbuffer[i], 136, 1);
		tempvmask.data[68].value = READFROM_UINT(bitsbuffer[i], 137, 1);
		tempvmask.data[69].key = READFROM_UINT(bitsbuffer[i], 138, 1);
		tempvmask.data[69].value = READFROM_UINT(bitsbuffer[i], 139, 1);
		tempvmask.data[70].key = READFROM_UINT(bitsbuffer[i], 140, 1);
		tempvmask.data[70].value = READFROM_UINT(bitsbuffer[i], 141, 1);
		tempvmask.data[71].key = READFROM_UINT(bitsbuffer[i], 142, 1);
		tempvmask.data[71].value = READFROM_UINT(bitsbuffer[i], 143, 1);
		tempvmask.data[72].key = READFROM_UINT(bitsbuffer[i], 144, 1);
		tempvmask.data[72].value = READFROM_UINT(bitsbuffer[i], 145, 1);
		tempvmask.data[73].key = READFROM_UINT(bitsbuffer[i], 146, 1);
		tempvmask.data[73].value = READFROM_UINT(bitsbuffer[i], 147, 1);
		tempvmask.data[74].key = READFROM_UINT(bitsbuffer[i], 148, 1);
		tempvmask.data[74].value = READFROM_UINT(bitsbuffer[i], 149, 1);
		tempvmask.data[75].key = READFROM_UINT(bitsbuffer[i], 150, 1);
		tempvmask.data[75].value = READFROM_UINT(bitsbuffer[i], 151, 1);
		tempvmask.data[76].key = READFROM_UINT(bitsbuffer[i], 152, 1);
		tempvmask.data[76].value = READFROM_UINT(bitsbuffer[i], 153, 1);
		tempvmask.data[77].key = READFROM_UINT(bitsbuffer[i], 154, 1);
		tempvmask.data[77].value = READFROM_UINT(bitsbuffer[i], 155, 1);
		tempvmask.data[78].key = READFROM_UINT(bitsbuffer[i], 156, 1);
		tempvmask.data[78].value = READFROM_UINT(bitsbuffer[i], 157, 1);
		tempvmask.data[79].key = READFROM_UINT(bitsbuffer[i], 158, 1);
		tempvmask.data[79].value = READFROM_UINT(bitsbuffer[i], 159, 1);
		tempvmask.data[80].key = READFROM_UINT(bitsbuffer[i], 160, 1);
		tempvmask.data[80].value = READFROM_UINT(bitsbuffer[i], 161, 1);
		tempvmask.data[81].key = READFROM_UINT(bitsbuffer[i], 162, 1);
		tempvmask.data[81].value = READFROM_UINT(bitsbuffer[i], 163, 1);
		tempvmask.data[82].key = READFROM_UINT(bitsbuffer[i], 164, 1);
		tempvmask.data[82].value = READFROM_UINT(bitsbuffer[i], 165, 1);
		tempvmask.data[83].key = READFROM_UINT(bitsbuffer[i], 166, 1);
		tempvmask.data[83].value = READFROM_UINT(bitsbuffer[i], 167, 1);
		tempvmask.data[84].key = READFROM_UINT(bitsbuffer[i], 168, 1);
		tempvmask.data[84].value = READFROM_UINT(bitsbuffer[i], 169, 1);
		tempvmask.data[85].key = READFROM_UINT(bitsbuffer[i], 170, 1);
		tempvmask.data[85].value = READFROM_UINT(bitsbuffer[i], 171, 1);
		tempvmask.data[86].key = READFROM_UINT(bitsbuffer[i], 172, 1);
		tempvmask.data[86].value = READFROM_UINT(bitsbuffer[i], 173, 1);
		tempvmask.data[87].key = READFROM_UINT(bitsbuffer[i], 174, 1);
		tempvmask.data[87].value = READFROM_UINT(bitsbuffer[i], 175, 1);
		tempvmask.data[88].key = READFROM_UINT(bitsbuffer[i], 176, 1);
		tempvmask.data[88].value = READFROM_UINT(bitsbuffer[i], 177, 1);
		tempvmask.data[89].key = READFROM_UINT(bitsbuffer[i], 178, 1);
		tempvmask.data[89].value = READFROM_UINT(bitsbuffer[i], 179, 1);
		tempvmask.data[90].key = READFROM_UINT(bitsbuffer[i], 180, 1);
		tempvmask.data[90].value = READFROM_UINT(bitsbuffer[i], 181, 1);
		tempvmask.data[91].key = READFROM_UINT(bitsbuffer[i], 182, 1);
		tempvmask.data[91].value = READFROM_UINT(bitsbuffer[i], 183, 1);
		tempvmask.data[92].key = READFROM_UINT(bitsbuffer[i], 184, 1);
		tempvmask.data[92].value = READFROM_UINT(bitsbuffer[i], 185, 1);
		tempvmask.data[93].key = READFROM_UINT(bitsbuffer[i], 186, 1);
		tempvmask.data[93].value = READFROM_UINT(bitsbuffer[i], 187, 1);
		tempvmask.data[94].key = READFROM_UINT(bitsbuffer[i], 188, 1);
		tempvmask.data[94].value = READFROM_UINT(bitsbuffer[i], 189, 1);
		tempvmask.data[95].key = READFROM_UINT(bitsbuffer[i], 190, 1);
		tempvmask.data[95].value = READFROM_UINT(bitsbuffer[i], 191, 1);
		tempvmask.data[96].key = READFROM_UINT(bitsbuffer[i], 192, 1);
		tempvmask.data[96].value = READFROM_UINT(bitsbuffer[i], 193, 1);
		tempvmask.data[97].key = READFROM_UINT(bitsbuffer[i], 194, 1);
		tempvmask.data[97].value = READFROM_UINT(bitsbuffer[i], 195, 1);
		tempvmask.data[98].key = READFROM_UINT(bitsbuffer[i], 196, 1);
		tempvmask.data[98].value = READFROM_UINT(bitsbuffer[i], 197, 1);
		tempvmask.data[99].key = READFROM_UINT(bitsbuffer[i], 198, 1);
		tempvmask.data[99].value = READFROM_UINT(bitsbuffer[i], 199, 1);
		tempvmask.data[100].key = READFROM_UINT(bitsbuffer[i], 200, 1);
		tempvmask.data[100].value = READFROM_UINT(bitsbuffer[i], 201, 1);
		tempvmask.data[101].key = READFROM_UINT(bitsbuffer[i], 202, 1);
		tempvmask.data[101].value = READFROM_UINT(bitsbuffer[i], 203, 1);
		tempvmask.data[102].key = READFROM_UINT(bitsbuffer[i], 204, 1);
		tempvmask.data[102].value = READFROM_UINT(bitsbuffer[i], 205, 1);
		tempvmask.data[103].key = READFROM_UINT(bitsbuffer[i], 206, 1);
		tempvmask.data[103].value = READFROM_UINT(bitsbuffer[i], 207, 1);
		tempvmask.data[104].key = READFROM_UINT(bitsbuffer[i], 208, 1);
		tempvmask.data[104].value = READFROM_UINT(bitsbuffer[i], 209, 1);
		tempvmask.data[105].key = READFROM_UINT(bitsbuffer[i], 210, 1);
		tempvmask.data[105].value = READFROM_UINT(bitsbuffer[i], 211, 1);
		tempvmask.data[106].key = READFROM_UINT(bitsbuffer[i], 212, 1);
		tempvmask.data[106].value = READFROM_UINT(bitsbuffer[i], 213, 1);
		tempvmask.data[107].key = READFROM_UINT(bitsbuffer[i], 214, 1);
		tempvmask.data[107].value = READFROM_UINT(bitsbuffer[i], 215, 1);
		tempvmask.data[108].key = READFROM_UINT(bitsbuffer[i], 216, 1);
		tempvmask.data[108].value = READFROM_UINT(bitsbuffer[i], 217, 1);
		tempvmask.data[109].key = READFROM_UINT(bitsbuffer[i], 218, 1);
		tempvmask.data[109].value = READFROM_UINT(bitsbuffer[i], 219, 1);
		tempvmask.data[110].key = READFROM_UINT(bitsbuffer[i], 220, 1);
		tempvmask.data[110].value = READFROM_UINT(bitsbuffer[i], 221, 1);
		tempvmask.data[111].key = READFROM_UINT(bitsbuffer[i], 222, 1);
		tempvmask.data[111].value = READFROM_UINT(bitsbuffer[i], 223, 1);
		tempvmask.data[112].key = READFROM_UINT(bitsbuffer[i], 224, 1);
		tempvmask.data[112].value = READFROM_UINT(bitsbuffer[i], 225, 1);
		tempvmask.data[113].key = READFROM_UINT(bitsbuffer[i], 226, 1);
		tempvmask.data[113].value = READFROM_UINT(bitsbuffer[i], 227, 1);
		tempvmask.data[114].key = READFROM_UINT(bitsbuffer[i], 228, 1);
		tempvmask.data[114].value = READFROM_UINT(bitsbuffer[i], 229, 1);
		tempvmask.data[115].key = READFROM_UINT(bitsbuffer[i], 230, 1);
		tempvmask.data[115].value = READFROM_UINT(bitsbuffer[i], 231, 1);
		tempvmask.data[116].key = READFROM_UINT(bitsbuffer[i], 232, 1);
		tempvmask.data[116].value = READFROM_UINT(bitsbuffer[i], 233, 1);
		tempvmask.data[117].key = READFROM_UINT(bitsbuffer[i], 234, 1);
		tempvmask.data[117].value = READFROM_UINT(bitsbuffer[i], 235, 1);
		tempvmask.data[118].key = READFROM_UINT(bitsbuffer[i], 236, 1);
		tempvmask.data[118].value = READFROM_UINT(bitsbuffer[i], 237, 1);
		tempvmask.data[119].key = READFROM_UINT(bitsbuffer[i], 238, 1);
		tempvmask.data[119].value = READFROM_UINT(bitsbuffer[i], 239, 1);
		tempvmask.data[120].key = READFROM_UINT(bitsbuffer[i], 240, 1);
		tempvmask.data[120].value = READFROM_UINT(bitsbuffer[i], 241, 1);
		tempvmask.data[121].key = READFROM_UINT(bitsbuffer[i], 242, 1);
		tempvmask.data[121].value = READFROM_UINT(bitsbuffer[i], 243, 1);
		tempvmask.data[122].key = READFROM_UINT(bitsbuffer[i], 244, 1);
		tempvmask.data[122].value = READFROM_UINT(bitsbuffer[i], 245, 1);
		tempvmask.data[123].key = READFROM_UINT(bitsbuffer[i], 246, 1);
		tempvmask.data[123].value = READFROM_UINT(bitsbuffer[i], 247, 1);
		tempvmask.data[124].key = READFROM_UINT(bitsbuffer[i], 248, 1);
		tempvmask.data[124].value = READFROM_UINT(bitsbuffer[i], 249, 1);
		tempvmask.data[125].key = READFROM_UINT(bitsbuffer[i], 250, 1);
		tempvmask.data[125].value = READFROM_UINT(bitsbuffer[i], 251, 1);
		tempvmask.data[126].key = READFROM_UINT(bitsbuffer[i], 252, 1);
		tempvmask.data[126].value = READFROM_UINT(bitsbuffer[i], 253, 1);
		tempvmask.data[127].key = READFROM_UINT(bitsbuffer[i], 254, 1);
		tempvmask.data[127].value = READFROM_UINT(bitsbuffer[i], 255, 1);
		tempvmask.data[128].key = READFROM_UINT(bitsbuffer[i], 256, 1);
		tempvmask.data[128].value = READFROM_UINT(bitsbuffer[i], 257, 1);
		tempvmask.data[129].key = READFROM_UINT(bitsbuffer[i], 258, 1);
		tempvmask.data[129].value = READFROM_UINT(bitsbuffer[i], 259, 1);
		tempvmask.data[130].key = READFROM_UINT(bitsbuffer[i], 260, 1);
		tempvmask.data[130].value = READFROM_UINT(bitsbuffer[i], 261, 1);
		tempvmask.data[131].key = READFROM_UINT(bitsbuffer[i], 262, 1);
		tempvmask.data[131].value = READFROM_UINT(bitsbuffer[i], 263, 1);
		tempvmask.data[132].key = READFROM_UINT(bitsbuffer[i], 264, 1);
		tempvmask.data[132].value = READFROM_UINT(bitsbuffer[i], 265, 1);
		tempvmask.data[133].key = READFROM_UINT(bitsbuffer[i], 266, 1);
		tempvmask.data[133].value = READFROM_UINT(bitsbuffer[i], 267, 1);
		tempvmask.data[134].key = READFROM_UINT(bitsbuffer[i], 268, 1);
		tempvmask.data[134].value = READFROM_UINT(bitsbuffer[i], 269, 1);
		tempvmask.data[135].key = READFROM_UINT(bitsbuffer[i], 270, 1);
		tempvmask.data[135].value = READFROM_UINT(bitsbuffer[i], 271, 1);
		tempvmask.data[136].key = READFROM_UINT(bitsbuffer[i], 272, 1);
		tempvmask.data[136].value = READFROM_UINT(bitsbuffer[i], 273, 1);
		tempvmask.data[137].key = READFROM_UINT(bitsbuffer[i], 274, 1);
		tempvmask.data[137].value = READFROM_UINT(bitsbuffer[i], 275, 1);
		tempvmask.data[138].key = READFROM_UINT(bitsbuffer[i], 276, 1);
		tempvmask.data[138].value = READFROM_UINT(bitsbuffer[i], 277, 1);
		tempvmask.data[139].key = READFROM_UINT(bitsbuffer[i], 278, 1);
		tempvmask.data[139].value = READFROM_UINT(bitsbuffer[i], 279, 1);
		tempvmask.data[140].key = READFROM_UINT(bitsbuffer[i], 280, 1);
		tempvmask.data[140].value = READFROM_UINT(bitsbuffer[i], 281, 1);
		tempvmask.data[141].key = READFROM_UINT(bitsbuffer[i], 282, 1);
		tempvmask.data[141].value = READFROM_UINT(bitsbuffer[i], 283, 1);
		tempvmask.data[142].key = READFROM_UINT(bitsbuffer[i], 284, 1);
		tempvmask.data[142].value = READFROM_UINT(bitsbuffer[i], 285, 1);
		tempvmask.data[143].key = READFROM_UINT(bitsbuffer[i], 286, 1);
		tempvmask.data[143].value = READFROM_UINT(bitsbuffer[i], 287, 1);
		tempvmask.data[144].key = READFROM_UINT(bitsbuffer[i], 288, 1);
		tempvmask.data[144].value = READFROM_UINT(bitsbuffer[i], 289, 1);
		tempvmask.data[145].key = READFROM_UINT(bitsbuffer[i], 290, 1);
		tempvmask.data[145].value = READFROM_UINT(bitsbuffer[i], 291, 1);
		tempvmask.data[146].key = READFROM_UINT(bitsbuffer[i], 292, 1);
		tempvmask.data[146].value = READFROM_UINT(bitsbuffer[i], 293, 1);
		tempvmask.data[147].key = READFROM_UINT(bitsbuffer[i], 294, 1);
		tempvmask.data[147].value = READFROM_UINT(bitsbuffer[i], 295, 1);
		tempvmask.data[148].key = READFROM_UINT(bitsbuffer[i], 296, 1);
		tempvmask.data[148].value = READFROM_UINT(bitsbuffer[i], 297, 1);
		tempvmask.data[149].key = READFROM_UINT(bitsbuffer[i], 298, 1);
		tempvmask.data[149].value = READFROM_UINT(bitsbuffer[i], 299, 1);
		tempvmask.data[150].key = READFROM_UINT(bitsbuffer[i], 300, 1);
		tempvmask.data[150].value = READFROM_UINT(bitsbuffer[i], 301, 1);
		tempvmask.data[151].key = READFROM_UINT(bitsbuffer[i], 302, 1);
		tempvmask.data[151].value = READFROM_UINT(bitsbuffer[i], 303, 1);
		tempvmask.data[152].key = READFROM_UINT(bitsbuffer[i], 304, 1);
		tempvmask.data[152].value = READFROM_UINT(bitsbuffer[i], 305, 1);
		tempvmask.data[153].key = READFROM_UINT(bitsbuffer[i], 306, 1);
		tempvmask.data[153].value = READFROM_UINT(bitsbuffer[i], 307, 1);
		tempvmask.data[154].key = READFROM_UINT(bitsbuffer[i], 308, 1);
		tempvmask.data[154].value = READFROM_UINT(bitsbuffer[i], 309, 1);
		tempvmask.data[155].key = READFROM_UINT(bitsbuffer[i], 310, 1);
		tempvmask.data[155].value = READFROM_UINT(bitsbuffer[i], 311, 1);
		tempvmask.data[156].key = READFROM_UINT(bitsbuffer[i], 312, 1);
		tempvmask.data[156].value = READFROM_UINT(bitsbuffer[i], 313, 1);
		tempvmask.data[157].key = READFROM_UINT(bitsbuffer[i], 314, 1);
		tempvmask.data[157].value = READFROM_UINT(bitsbuffer[i], 315, 1);
		tempvmask.data[158].key = READFROM_UINT(bitsbuffer[i], 316, 1);
		tempvmask.data[158].value = READFROM_UINT(bitsbuffer[i], 317, 1);
		tempvmask.data[159].key = READFROM_UINT(bitsbuffer[i], 318, 1);
		tempvmask.data[159].value = READFROM_UINT(bitsbuffer[i], 319, 1);
		tempvmask.data[160].key = READFROM_UINT(bitsbuffer[i], 320, 1);
		tempvmask.data[160].value = READFROM_UINT(bitsbuffer[i], 321, 1);
		tempvmask.data[161].key = READFROM_UINT(bitsbuffer[i], 322, 1);
		tempvmask.data[161].value = READFROM_UINT(bitsbuffer[i], 323, 1);
		tempvmask.data[162].key = READFROM_UINT(bitsbuffer[i], 324, 1);
		tempvmask.data[162].value = READFROM_UINT(bitsbuffer[i], 325, 1);
		tempvmask.data[163].key = READFROM_UINT(bitsbuffer[i], 326, 1);
		tempvmask.data[163].value = READFROM_UINT(bitsbuffer[i], 327, 1);
		tempvmask.data[164].key = READFROM_UINT(bitsbuffer[i], 328, 1);
		tempvmask.data[164].value = READFROM_UINT(bitsbuffer[i], 329, 1);
		tempvmask.data[165].key = READFROM_UINT(bitsbuffer[i], 330, 1);
		tempvmask.data[165].value = READFROM_UINT(bitsbuffer[i], 331, 1);
		tempvmask.data[166].key = READFROM_UINT(bitsbuffer[i], 332, 1);
		tempvmask.data[166].value = READFROM_UINT(bitsbuffer[i], 333, 1);
		tempvmask.data[167].key = READFROM_UINT(bitsbuffer[i], 334, 1);
		tempvmask.data[167].value = READFROM_UINT(bitsbuffer[i], 335, 1);
		tempvmask.data[168].key = READFROM_UINT(bitsbuffer[i], 336, 1);
		tempvmask.data[168].value = READFROM_UINT(bitsbuffer[i], 337, 1);
		tempvmask.data[169].key = READFROM_UINT(bitsbuffer[i], 338, 1);
		tempvmask.data[169].value = READFROM_UINT(bitsbuffer[i], 339, 1);
		tempvmask.data[170].key = READFROM_UINT(bitsbuffer[i], 340, 1);
		tempvmask.data[170].value = READFROM_UINT(bitsbuffer[i], 341, 1);
		tempvmask.data[171].key = READFROM_UINT(bitsbuffer[i], 342, 1);
		tempvmask.data[171].value = READFROM_UINT(bitsbuffer[i], 343, 1);
		tempvmask.data[172].key = READFROM_UINT(bitsbuffer[i], 344, 1);
		tempvmask.data[172].value = READFROM_UINT(bitsbuffer[i], 345, 1);
		tempvmask.data[173].key = READFROM_UINT(bitsbuffer[i], 346, 1);
		tempvmask.data[173].value = READFROM_UINT(bitsbuffer[i], 347, 1);
		tempvmask.data[174].key = READFROM_UINT(bitsbuffer[i], 348, 1);
		tempvmask.data[174].value = READFROM_UINT(bitsbuffer[i], 349, 1);
		tempvmask.data[175].key = READFROM_UINT(bitsbuffer[i], 350, 1);
		tempvmask.data[175].value = READFROM_UINT(bitsbuffer[i], 351, 1);
		tempvmask.data[176].key = READFROM_UINT(bitsbuffer[i], 352, 1);
		tempvmask.data[176].value = READFROM_UINT(bitsbuffer[i], 353, 1);
		tempvmask.data[177].key = READFROM_UINT(bitsbuffer[i], 354, 1);
		tempvmask.data[177].value = READFROM_UINT(bitsbuffer[i], 355, 1);
		tempvmask.data[178].key = READFROM_UINT(bitsbuffer[i], 356, 1);
		tempvmask.data[178].value = READFROM_UINT(bitsbuffer[i], 357, 1);
		tempvmask.data[179].key = READFROM_UINT(bitsbuffer[i], 358, 1);
		tempvmask.data[179].value = READFROM_UINT(bitsbuffer[i], 359, 1);
		tempvmask.data[180].key = READFROM_UINT(bitsbuffer[i], 360, 1);
		tempvmask.data[180].value = READFROM_UINT(bitsbuffer[i], 361, 1);
		tempvmask.data[181].key = READFROM_UINT(bitsbuffer[i], 362, 1);
		tempvmask.data[181].value = READFROM_UINT(bitsbuffer[i], 363, 1);
		tempvmask.data[182].key = READFROM_UINT(bitsbuffer[i], 364, 1);
		tempvmask.data[182].value = READFROM_UINT(bitsbuffer[i], 365, 1);
		tempvmask.data[183].key = READFROM_UINT(bitsbuffer[i], 366, 1);
		tempvmask.data[183].value = READFROM_UINT(bitsbuffer[i], 367, 1);
		tempvmask.data[184].key = READFROM_UINT(bitsbuffer[i], 368, 1);
		tempvmask.data[184].value = READFROM_UINT(bitsbuffer[i], 369, 1);
		tempvmask.data[185].key = READFROM_UINT(bitsbuffer[i], 370, 1);
		tempvmask.data[185].value = READFROM_UINT(bitsbuffer[i], 371, 1);
		tempvmask.data[186].key = READFROM_UINT(bitsbuffer[i], 372, 1);
		tempvmask.data[186].value = READFROM_UINT(bitsbuffer[i], 373, 1);
		tempvmask.data[187].key = READFROM_UINT(bitsbuffer[i], 374, 1);
		tempvmask.data[187].value = READFROM_UINT(bitsbuffer[i], 375, 1);
		tempvmask.data[188].key = READFROM_UINT(bitsbuffer[i], 376, 1);
		tempvmask.data[188].value = READFROM_UINT(bitsbuffer[i], 377, 1);
		tempvmask.data[189].key = READFROM_UINT(bitsbuffer[i], 378, 1);
		tempvmask.data[189].value = READFROM_UINT(bitsbuffer[i], 379, 1);
		tempvmask.data[190].key = READFROM_UINT(bitsbuffer[i], 380, 1);
		tempvmask.data[190].value = READFROM_UINT(bitsbuffer[i], 381, 1);
		tempvmask.data[191].key = READFROM_UINT(bitsbuffer[i], 382, 1);
		tempvmask.data[191].value = READFROM_UINT(bitsbuffer[i], 383, 1);
		tempvmask.data[192].key = READFROM_UINT(bitsbuffer[i], 384, 1);
		tempvmask.data[192].value = READFROM_UINT(bitsbuffer[i], 385, 1);
		tempvmask.data[193].key = READFROM_UINT(bitsbuffer[i], 386, 1);
		tempvmask.data[193].value = READFROM_UINT(bitsbuffer[i], 387, 1);
		tempvmask.data[194].key = READFROM_UINT(bitsbuffer[i], 388, 1);
		tempvmask.data[194].value = READFROM_UINT(bitsbuffer[i], 389, 1);
		tempvmask.data[195].key = READFROM_UINT(bitsbuffer[i], 390, 1);
		tempvmask.data[195].value = READFROM_UINT(bitsbuffer[i], 391, 1);
		tempvmask.data[196].key = READFROM_UINT(bitsbuffer[i], 392, 1);
		tempvmask.data[196].value = READFROM_UINT(bitsbuffer[i], 393, 1);
		tempvmask.data[197].key = READFROM_UINT(bitsbuffer[i], 394, 1);
		tempvmask.data[197].value = READFROM_UINT(bitsbuffer[i], 395, 1);
		tempvmask.data[198].key = READFROM_UINT(bitsbuffer[i], 396, 1);
		tempvmask.data[198].value = READFROM_UINT(bitsbuffer[i], 397, 1);
		tempvmask.data[199].key = READFROM_UINT(bitsbuffer[i], 398, 1);
		tempvmask.data[199].value = READFROM_UINT(bitsbuffer[i], 399, 1);
		tempvmask.data[200].key = READFROM_UINT(bitsbuffer[i], 400, 1);
		tempvmask.data[200].value = READFROM_UINT(bitsbuffer[i], 401, 1);
		tempvmask.data[201].key = READFROM_UINT(bitsbuffer[i], 402, 1);
		tempvmask.data[201].value = READFROM_UINT(bitsbuffer[i], 403, 1);
		tempvmask.data[202].key = READFROM_UINT(bitsbuffer[i], 404, 1);
		tempvmask.data[202].value = READFROM_UINT(bitsbuffer[i], 405, 1);
		tempvmask.data[203].key = READFROM_UINT(bitsbuffer[i], 406, 1);
		tempvmask.data[203].value = READFROM_UINT(bitsbuffer[i], 407, 1);
		tempvmask.data[204].key = READFROM_UINT(bitsbuffer[i], 408, 1);
		tempvmask.data[204].value = READFROM_UINT(bitsbuffer[i], 409, 1);
		tempvmask.data[205].key = READFROM_UINT(bitsbuffer[i], 410, 1);
		tempvmask.data[205].value = READFROM_UINT(bitsbuffer[i], 411, 1);
		tempvmask.data[206].key = READFROM_UINT(bitsbuffer[i], 412, 1);
		tempvmask.data[206].value = READFROM_UINT(bitsbuffer[i], 413, 1);
		tempvmask.data[207].key = READFROM_UINT(bitsbuffer[i], 414, 1);
		tempvmask.data[207].value = READFROM_UINT(bitsbuffer[i], 415, 1);
		tempvmask.data[208].key = READFROM_UINT(bitsbuffer[i], 416, 1);
		tempvmask.data[208].value = READFROM_UINT(bitsbuffer[i], 417, 1);
		tempvmask.data[209].key = READFROM_UINT(bitsbuffer[i], 418, 1);
		tempvmask.data[209].value = READFROM_UINT(bitsbuffer[i], 419, 1);
		tempvmask.data[210].key = READFROM_UINT(bitsbuffer[i], 420, 1);
		tempvmask.data[210].value = READFROM_UINT(bitsbuffer[i], 421, 1);
		tempvmask.data[211].key = READFROM_UINT(bitsbuffer[i], 422, 1);
		tempvmask.data[211].value = READFROM_UINT(bitsbuffer[i], 423, 1);
		tempvmask.data[212].key = READFROM_UINT(bitsbuffer[i], 424, 1);
		tempvmask.data[212].value = READFROM_UINT(bitsbuffer[i], 425, 1);
		tempvmask.data[213].key = READFROM_UINT(bitsbuffer[i], 426, 1);
		tempvmask.data[213].value = READFROM_UINT(bitsbuffer[i], 427, 1);
		tempvmask.data[214].key = READFROM_UINT(bitsbuffer[i], 428, 1);
		tempvmask.data[214].value = READFROM_UINT(bitsbuffer[i], 429, 1);
		tempvmask.data[215].key = READFROM_UINT(bitsbuffer[i], 430, 1);
		tempvmask.data[215].value = READFROM_UINT(bitsbuffer[i], 431, 1);
		tempvmask.data[216].key = READFROM_UINT(bitsbuffer[i], 432, 1);
		tempvmask.data[216].value = READFROM_UINT(bitsbuffer[i], 433, 1);
		tempvmask.data[217].key = READFROM_UINT(bitsbuffer[i], 434, 1);
		tempvmask.data[217].value = READFROM_UINT(bitsbuffer[i], 435, 1);
		tempvmask.data[218].key = READFROM_UINT(bitsbuffer[i], 436, 1);
		tempvmask.data[218].value = READFROM_UINT(bitsbuffer[i], 437, 1);
		tempvmask.data[219].key = READFROM_UINT(bitsbuffer[i], 438, 1);
		tempvmask.data[219].value = READFROM_UINT(bitsbuffer[i], 439, 1);
		tempvmask.data[220].key = READFROM_UINT(bitsbuffer[i], 440, 1);
		tempvmask.data[220].value = READFROM_UINT(bitsbuffer[i], 441, 1);
		tempvmask.data[221].key = READFROM_UINT(bitsbuffer[i], 442, 1);
		tempvmask.data[221].value = READFROM_UINT(bitsbuffer[i], 443, 1);
		tempvmask.data[222].key = READFROM_UINT(bitsbuffer[i], 444, 1);
		tempvmask.data[222].value = READFROM_UINT(bitsbuffer[i], 445, 1);
		tempvmask.data[223].key = READFROM_UINT(bitsbuffer[i], 446, 1);
		tempvmask.data[223].value = READFROM_UINT(bitsbuffer[i], 447, 1);
		tempvmask.data[224].key = READFROM_UINT(bitsbuffer[i], 448, 1);
		tempvmask.data[224].value = READFROM_UINT(bitsbuffer[i], 449, 1);
		tempvmask.data[225].key = READFROM_UINT(bitsbuffer[i], 450, 1);
		tempvmask.data[225].value = READFROM_UINT(bitsbuffer[i], 451, 1);
		tempvmask.data[226].key = READFROM_UINT(bitsbuffer[i], 452, 1);
		tempvmask.data[226].value = READFROM_UINT(bitsbuffer[i], 453, 1);
		tempvmask.data[227].key = READFROM_UINT(bitsbuffer[i], 454, 1);
		tempvmask.data[227].value = READFROM_UINT(bitsbuffer[i], 455, 1);
		tempvmask.data[228].key = READFROM_UINT(bitsbuffer[i], 456, 1);
		tempvmask.data[228].value = READFROM_UINT(bitsbuffer[i], 457, 1);
		tempvmask.data[229].key = READFROM_UINT(bitsbuffer[i], 458, 1);
		tempvmask.data[229].value = READFROM_UINT(bitsbuffer[i], 459, 1);
		tempvmask.data[230].key = READFROM_UINT(bitsbuffer[i], 460, 1);
		tempvmask.data[230].value = READFROM_UINT(bitsbuffer[i], 461, 1);
		tempvmask.data[231].key = READFROM_UINT(bitsbuffer[i], 462, 1);
		tempvmask.data[231].value = READFROM_UINT(bitsbuffer[i], 463, 1);
		tempvmask.data[232].key = READFROM_UINT(bitsbuffer[i], 464, 1);
		tempvmask.data[232].value = READFROM_UINT(bitsbuffer[i], 465, 1);
		tempvmask.data[233].key = READFROM_UINT(bitsbuffer[i], 466, 1);
		tempvmask.data[233].value = READFROM_UINT(bitsbuffer[i], 467, 1);
		tempvmask.data[234].key = READFROM_UINT(bitsbuffer[i], 468, 1);
		tempvmask.data[234].value = READFROM_UINT(bitsbuffer[i], 469, 1);
		tempvmask.data[235].key = READFROM_UINT(bitsbuffer[i], 470, 1);
		tempvmask.data[235].value = READFROM_UINT(bitsbuffer[i], 471, 1);
		tempvmask.data[236].key = READFROM_UINT(bitsbuffer[i], 472, 1);
		tempvmask.data[236].value = READFROM_UINT(bitsbuffer[i], 473, 1);
		tempvmask.data[237].key = READFROM_UINT(bitsbuffer[i], 474, 1);
		tempvmask.data[237].value = READFROM_UINT(bitsbuffer[i], 475, 1);
		tempvmask.data[238].key = READFROM_UINT(bitsbuffer[i], 476, 1);
		tempvmask.data[238].value = READFROM_UINT(bitsbuffer[i], 477, 1);
		tempvmask.data[239].key = READFROM_UINT(bitsbuffer[i], 478, 1);
		tempvmask.data[239].value = READFROM_UINT(bitsbuffer[i], 479, 1);
		tempvmask.data[240].key = READFROM_UINT(bitsbuffer[i], 480, 1);
		tempvmask.data[240].value = READFROM_UINT(bitsbuffer[i], 481, 1);
		tempvmask.data[241].key = READFROM_UINT(bitsbuffer[i], 482, 1);
		tempvmask.data[241].value = READFROM_UINT(bitsbuffer[i], 483, 1);
		tempvmask.data[242].key = READFROM_UINT(bitsbuffer[i], 484, 1);
		tempvmask.data[242].value = READFROM_UINT(bitsbuffer[i], 485, 1);
		tempvmask.data[243].key = READFROM_UINT(bitsbuffer[i], 486, 1);
		tempvmask.data[243].value = READFROM_UINT(bitsbuffer[i], 487, 1);
		tempvmask.data[244].key = READFROM_UINT(bitsbuffer[i], 488, 1);
		tempvmask.data[244].value = READFROM_UINT(bitsbuffer[i], 489, 1);
		tempvmask.data[245].key = READFROM_UINT(bitsbuffer[i], 490, 1);
		tempvmask.data[245].value = READFROM_UINT(bitsbuffer[i], 491, 1);
		tempvmask.data[246].key = READFROM_UINT(bitsbuffer[i], 492, 1);
		tempvmask.data[246].value = READFROM_UINT(bitsbuffer[i], 493, 1);
		tempvmask.data[247].key = READFROM_UINT(bitsbuffer[i], 494, 1);
		tempvmask.data[247].value = READFROM_UINT(bitsbuffer[i], 495, 1);
		tempvmask.data[248].key = READFROM_UINT(bitsbuffer[i], 496, 1);
		tempvmask.data[248].value = READFROM_UINT(bitsbuffer[i], 497, 1);
		tempvmask.data[249].key = READFROM_UINT(bitsbuffer[i], 498, 1);
		tempvmask.data[249].value = READFROM_UINT(bitsbuffer[i], 499, 1);
		tempvmask.data[250].key = READFROM_UINT(bitsbuffer[i], 500, 1);
		tempvmask.data[250].value = READFROM_UINT(bitsbuffer[i], 501, 1);
		tempvmask.data[251].key = READFROM_UINT(bitsbuffer[i], 502, 1);
		tempvmask.data[251].value = READFROM_UINT(bitsbuffer[i], 503, 1);
		tempvmask.data[252].key = READFROM_UINT(bitsbuffer[i], 504, 1);
		tempvmask.data[252].value = READFROM_UINT(bitsbuffer[i], 505, 1);
		tempvmask.data[253].key = READFROM_UINT(bitsbuffer[i], 506, 1);
		tempvmask.data[253].value = READFROM_UINT(bitsbuffer[i], 507, 1);
		tempvmask.data[254].key = READFROM_UINT(bitsbuffer[i], 508, 1);
		tempvmask.data[254].value = READFROM_UINT(bitsbuffer[i], 509, 1);
		tempvmask.data[255].key = READFROM_UINT(bitsbuffer[i], 510, 1);
		tempvmask.data[255].value = READFROM_UINT(bitsbuffer[i], 511, 1);
		#endif
		
		vmask[i].data[0].key = tempvmask.data[0].key;
		vmask[i].data[0].value = tempvmask.data[0].value;
		vmask[i].data[1].key = tempvmask.data[1].key;
		vmask[i].data[1].value = tempvmask.data[1].value;
		vmask[i].data[2].key = tempvmask.data[2].key;
		vmask[i].data[2].value = tempvmask.data[2].value;
		vmask[i].data[3].key = tempvmask.data[3].key;
		vmask[i].data[3].value = tempvmask.data[3].value;
		vmask[i].data[4].key = tempvmask.data[4].key;
		vmask[i].data[4].value = tempvmask.data[4].value;
		vmask[i].data[5].key = tempvmask.data[5].key;
		vmask[i].data[5].value = tempvmask.data[5].value;
		vmask[i].data[6].key = tempvmask.data[6].key;
		vmask[i].data[6].value = tempvmask.data[6].value;
		vmask[i].data[7].key = tempvmask.data[7].key;
		vmask[i].data[7].value = tempvmask.data[7].value;
		vmask[i].data[8].key = tempvmask.data[8].key;
		vmask[i].data[8].value = tempvmask.data[8].value;
		vmask[i].data[9].key = tempvmask.data[9].key;
		vmask[i].data[9].value = tempvmask.data[9].value;
		vmask[i].data[10].key = tempvmask.data[10].key;
		vmask[i].data[10].value = tempvmask.data[10].value;
		vmask[i].data[11].key = tempvmask.data[11].key;
		vmask[i].data[11].value = tempvmask.data[11].value;
		vmask[i].data[12].key = tempvmask.data[12].key;
		vmask[i].data[12].value = tempvmask.data[12].value;
		vmask[i].data[13].key = tempvmask.data[13].key;
		vmask[i].data[13].value = tempvmask.data[13].value;
		vmask[i].data[14].key = tempvmask.data[14].key;
		vmask[i].data[14].value = tempvmask.data[14].value;
		vmask[i].data[15].key = tempvmask.data[15].key;
		vmask[i].data[15].value = tempvmask.data[15].value;
		vmask[i].data[16].key = tempvmask.data[16].key;
		vmask[i].data[16].value = tempvmask.data[16].value;
		vmask[i].data[17].key = tempvmask.data[17].key;
		vmask[i].data[17].value = tempvmask.data[17].value;
		vmask[i].data[18].key = tempvmask.data[18].key;
		vmask[i].data[18].value = tempvmask.data[18].value;
		vmask[i].data[19].key = tempvmask.data[19].key;
		vmask[i].data[19].value = tempvmask.data[19].value;
		vmask[i].data[20].key = tempvmask.data[20].key;
		vmask[i].data[20].value = tempvmask.data[20].value;
		vmask[i].data[21].key = tempvmask.data[21].key;
		vmask[i].data[21].value = tempvmask.data[21].value;
		vmask[i].data[22].key = tempvmask.data[22].key;
		vmask[i].data[22].value = tempvmask.data[22].value;
		vmask[i].data[23].key = tempvmask.data[23].key;
		vmask[i].data[23].value = tempvmask.data[23].value;
		vmask[i].data[24].key = tempvmask.data[24].key;
		vmask[i].data[24].value = tempvmask.data[24].value;
		vmask[i].data[25].key = tempvmask.data[25].key;
		vmask[i].data[25].value = tempvmask.data[25].value;
		vmask[i].data[26].key = tempvmask.data[26].key;
		vmask[i].data[26].value = tempvmask.data[26].value;
		vmask[i].data[27].key = tempvmask.data[27].key;
		vmask[i].data[27].value = tempvmask.data[27].value;
		vmask[i].data[28].key = tempvmask.data[28].key;
		vmask[i].data[28].value = tempvmask.data[28].value;
		vmask[i].data[29].key = tempvmask.data[29].key;
		vmask[i].data[29].value = tempvmask.data[29].value;
		vmask[i].data[30].key = tempvmask.data[30].key;
		vmask[i].data[30].value = tempvmask.data[30].value;
		vmask[i].data[31].key = tempvmask.data[31].key;
		vmask[i].data[31].value = tempvmask.data[31].value;
		vmask[i].data[32].key = tempvmask.data[32].key;
		vmask[i].data[32].value = tempvmask.data[32].value;
		vmask[i].data[33].key = tempvmask.data[33].key;
		vmask[i].data[33].value = tempvmask.data[33].value;
		vmask[i].data[34].key = tempvmask.data[34].key;
		vmask[i].data[34].value = tempvmask.data[34].value;
		vmask[i].data[35].key = tempvmask.data[35].key;
		vmask[i].data[35].value = tempvmask.data[35].value;
		vmask[i].data[36].key = tempvmask.data[36].key;
		vmask[i].data[36].value = tempvmask.data[36].value;
		vmask[i].data[37].key = tempvmask.data[37].key;
		vmask[i].data[37].value = tempvmask.data[37].value;
		vmask[i].data[38].key = tempvmask.data[38].key;
		vmask[i].data[38].value = tempvmask.data[38].value;
		vmask[i].data[39].key = tempvmask.data[39].key;
		vmask[i].data[39].value = tempvmask.data[39].value;
		vmask[i].data[40].key = tempvmask.data[40].key;
		vmask[i].data[40].value = tempvmask.data[40].value;
		vmask[i].data[41].key = tempvmask.data[41].key;
		vmask[i].data[41].value = tempvmask.data[41].value;
		vmask[i].data[42].key = tempvmask.data[42].key;
		vmask[i].data[42].value = tempvmask.data[42].value;
		vmask[i].data[43].key = tempvmask.data[43].key;
		vmask[i].data[43].value = tempvmask.data[43].value;
		vmask[i].data[44].key = tempvmask.data[44].key;
		vmask[i].data[44].value = tempvmask.data[44].value;
		vmask[i].data[45].key = tempvmask.data[45].key;
		vmask[i].data[45].value = tempvmask.data[45].value;
		vmask[i].data[46].key = tempvmask.data[46].key;
		vmask[i].data[46].value = tempvmask.data[46].value;
		vmask[i].data[47].key = tempvmask.data[47].key;
		vmask[i].data[47].value = tempvmask.data[47].value;
		vmask[i].data[48].key = tempvmask.data[48].key;
		vmask[i].data[48].value = tempvmask.data[48].value;
		vmask[i].data[49].key = tempvmask.data[49].key;
		vmask[i].data[49].value = tempvmask.data[49].value;
		vmask[i].data[50].key = tempvmask.data[50].key;
		vmask[i].data[50].value = tempvmask.data[50].value;
		vmask[i].data[51].key = tempvmask.data[51].key;
		vmask[i].data[51].value = tempvmask.data[51].value;
		vmask[i].data[52].key = tempvmask.data[52].key;
		vmask[i].data[52].value = tempvmask.data[52].value;
		vmask[i].data[53].key = tempvmask.data[53].key;
		vmask[i].data[53].value = tempvmask.data[53].value;
		vmask[i].data[54].key = tempvmask.data[54].key;
		vmask[i].data[54].value = tempvmask.data[54].value;
		vmask[i].data[55].key = tempvmask.data[55].key;
		vmask[i].data[55].value = tempvmask.data[55].value;
		vmask[i].data[56].key = tempvmask.data[56].key;
		vmask[i].data[56].value = tempvmask.data[56].value;
		vmask[i].data[57].key = tempvmask.data[57].key;
		vmask[i].data[57].value = tempvmask.data[57].value;
		vmask[i].data[58].key = tempvmask.data[58].key;
		vmask[i].data[58].value = tempvmask.data[58].value;
		vmask[i].data[59].key = tempvmask.data[59].key;
		vmask[i].data[59].value = tempvmask.data[59].value;
		vmask[i].data[60].key = tempvmask.data[60].key;
		vmask[i].data[60].value = tempvmask.data[60].value;
		vmask[i].data[61].key = tempvmask.data[61].key;
		vmask[i].data[61].value = tempvmask.data[61].value;
		vmask[i].data[62].key = tempvmask.data[62].key;
		vmask[i].data[62].value = tempvmask.data[62].value;
		vmask[i].data[63].key = tempvmask.data[63].key;
		vmask[i].data[63].value = tempvmask.data[63].value;
		vmask[i].data[64].key = tempvmask.data[64].key;
		vmask[i].data[64].value = tempvmask.data[64].value;
		vmask[i].data[65].key = tempvmask.data[65].key;
		vmask[i].data[65].value = tempvmask.data[65].value;
		vmask[i].data[66].key = tempvmask.data[66].key;
		vmask[i].data[66].value = tempvmask.data[66].value;
		vmask[i].data[67].key = tempvmask.data[67].key;
		vmask[i].data[67].value = tempvmask.data[67].value;
		vmask[i].data[68].key = tempvmask.data[68].key;
		vmask[i].data[68].value = tempvmask.data[68].value;
		vmask[i].data[69].key = tempvmask.data[69].key;
		vmask[i].data[69].value = tempvmask.data[69].value;
		vmask[i].data[70].key = tempvmask.data[70].key;
		vmask[i].data[70].value = tempvmask.data[70].value;
		vmask[i].data[71].key = tempvmask.data[71].key;
		vmask[i].data[71].value = tempvmask.data[71].value;
		vmask[i].data[72].key = tempvmask.data[72].key;
		vmask[i].data[72].value = tempvmask.data[72].value;
		vmask[i].data[73].key = tempvmask.data[73].key;
		vmask[i].data[73].value = tempvmask.data[73].value;
		vmask[i].data[74].key = tempvmask.data[74].key;
		vmask[i].data[74].value = tempvmask.data[74].value;
		vmask[i].data[75].key = tempvmask.data[75].key;
		vmask[i].data[75].value = tempvmask.data[75].value;
		vmask[i].data[76].key = tempvmask.data[76].key;
		vmask[i].data[76].value = tempvmask.data[76].value;
		vmask[i].data[77].key = tempvmask.data[77].key;
		vmask[i].data[77].value = tempvmask.data[77].value;
		vmask[i].data[78].key = tempvmask.data[78].key;
		vmask[i].data[78].value = tempvmask.data[78].value;
		vmask[i].data[79].key = tempvmask.data[79].key;
		vmask[i].data[79].value = tempvmask.data[79].value;
		vmask[i].data[80].key = tempvmask.data[80].key;
		vmask[i].data[80].value = tempvmask.data[80].value;
		vmask[i].data[81].key = tempvmask.data[81].key;
		vmask[i].data[81].value = tempvmask.data[81].value;
		vmask[i].data[82].key = tempvmask.data[82].key;
		vmask[i].data[82].value = tempvmask.data[82].value;
		vmask[i].data[83].key = tempvmask.data[83].key;
		vmask[i].data[83].value = tempvmask.data[83].value;
		vmask[i].data[84].key = tempvmask.data[84].key;
		vmask[i].data[84].value = tempvmask.data[84].value;
		vmask[i].data[85].key = tempvmask.data[85].key;
		vmask[i].data[85].value = tempvmask.data[85].value;
		vmask[i].data[86].key = tempvmask.data[86].key;
		vmask[i].data[86].value = tempvmask.data[86].value;
		vmask[i].data[87].key = tempvmask.data[87].key;
		vmask[i].data[87].value = tempvmask.data[87].value;
		vmask[i].data[88].key = tempvmask.data[88].key;
		vmask[i].data[88].value = tempvmask.data[88].value;
		vmask[i].data[89].key = tempvmask.data[89].key;
		vmask[i].data[89].value = tempvmask.data[89].value;
		vmask[i].data[90].key = tempvmask.data[90].key;
		vmask[i].data[90].value = tempvmask.data[90].value;
		vmask[i].data[91].key = tempvmask.data[91].key;
		vmask[i].data[91].value = tempvmask.data[91].value;
		vmask[i].data[92].key = tempvmask.data[92].key;
		vmask[i].data[92].value = tempvmask.data[92].value;
		vmask[i].data[93].key = tempvmask.data[93].key;
		vmask[i].data[93].value = tempvmask.data[93].value;
		vmask[i].data[94].key = tempvmask.data[94].key;
		vmask[i].data[94].value = tempvmask.data[94].value;
		vmask[i].data[95].key = tempvmask.data[95].key;
		vmask[i].data[95].value = tempvmask.data[95].value;
		vmask[i].data[96].key = tempvmask.data[96].key;
		vmask[i].data[96].value = tempvmask.data[96].value;
		vmask[i].data[97].key = tempvmask.data[97].key;
		vmask[i].data[97].value = tempvmask.data[97].value;
		vmask[i].data[98].key = tempvmask.data[98].key;
		vmask[i].data[98].value = tempvmask.data[98].value;
		vmask[i].data[99].key = tempvmask.data[99].key;
		vmask[i].data[99].value = tempvmask.data[99].value;
		vmask[i].data[100].key = tempvmask.data[100].key;
		vmask[i].data[100].value = tempvmask.data[100].value;
		vmask[i].data[101].key = tempvmask.data[101].key;
		vmask[i].data[101].value = tempvmask.data[101].value;
		vmask[i].data[102].key = tempvmask.data[102].key;
		vmask[i].data[102].value = tempvmask.data[102].value;
		vmask[i].data[103].key = tempvmask.data[103].key;
		vmask[i].data[103].value = tempvmask.data[103].value;
		vmask[i].data[104].key = tempvmask.data[104].key;
		vmask[i].data[104].value = tempvmask.data[104].value;
		vmask[i].data[105].key = tempvmask.data[105].key;
		vmask[i].data[105].value = tempvmask.data[105].value;
		vmask[i].data[106].key = tempvmask.data[106].key;
		vmask[i].data[106].value = tempvmask.data[106].value;
		vmask[i].data[107].key = tempvmask.data[107].key;
		vmask[i].data[107].value = tempvmask.data[107].value;
		vmask[i].data[108].key = tempvmask.data[108].key;
		vmask[i].data[108].value = tempvmask.data[108].value;
		vmask[i].data[109].key = tempvmask.data[109].key;
		vmask[i].data[109].value = tempvmask.data[109].value;
		vmask[i].data[110].key = tempvmask.data[110].key;
		vmask[i].data[110].value = tempvmask.data[110].value;
		vmask[i].data[111].key = tempvmask.data[111].key;
		vmask[i].data[111].value = tempvmask.data[111].value;
		vmask[i].data[112].key = tempvmask.data[112].key;
		vmask[i].data[112].value = tempvmask.data[112].value;
		vmask[i].data[113].key = tempvmask.data[113].key;
		vmask[i].data[113].value = tempvmask.data[113].value;
		vmask[i].data[114].key = tempvmask.data[114].key;
		vmask[i].data[114].value = tempvmask.data[114].value;
		vmask[i].data[115].key = tempvmask.data[115].key;
		vmask[i].data[115].value = tempvmask.data[115].value;
		vmask[i].data[116].key = tempvmask.data[116].key;
		vmask[i].data[116].value = tempvmask.data[116].value;
		vmask[i].data[117].key = tempvmask.data[117].key;
		vmask[i].data[117].value = tempvmask.data[117].value;
		vmask[i].data[118].key = tempvmask.data[118].key;
		vmask[i].data[118].value = tempvmask.data[118].value;
		vmask[i].data[119].key = tempvmask.data[119].key;
		vmask[i].data[119].value = tempvmask.data[119].value;
		vmask[i].data[120].key = tempvmask.data[120].key;
		vmask[i].data[120].value = tempvmask.data[120].value;
		vmask[i].data[121].key = tempvmask.data[121].key;
		vmask[i].data[121].value = tempvmask.data[121].value;
		vmask[i].data[122].key = tempvmask.data[122].key;
		vmask[i].data[122].value = tempvmask.data[122].value;
		vmask[i].data[123].key = tempvmask.data[123].key;
		vmask[i].data[123].value = tempvmask.data[123].value;
		vmask[i].data[124].key = tempvmask.data[124].key;
		vmask[i].data[124].value = tempvmask.data[124].value;
		vmask[i].data[125].key = tempvmask.data[125].key;
		vmask[i].data[125].value = tempvmask.data[125].value;
		vmask[i].data[126].key = tempvmask.data[126].key;
		vmask[i].data[126].value = tempvmask.data[126].value;
		vmask[i].data[127].key = tempvmask.data[127].key;
		vmask[i].data[127].value = tempvmask.data[127].value;
		vmask[i].data[128].key = tempvmask.data[128].key;
		vmask[i].data[128].value = tempvmask.data[128].value;
		vmask[i].data[129].key = tempvmask.data[129].key;
		vmask[i].data[129].value = tempvmask.data[129].value;
		vmask[i].data[130].key = tempvmask.data[130].key;
		vmask[i].data[130].value = tempvmask.data[130].value;
		vmask[i].data[131].key = tempvmask.data[131].key;
		vmask[i].data[131].value = tempvmask.data[131].value;
		vmask[i].data[132].key = tempvmask.data[132].key;
		vmask[i].data[132].value = tempvmask.data[132].value;
		vmask[i].data[133].key = tempvmask.data[133].key;
		vmask[i].data[133].value = tempvmask.data[133].value;
		vmask[i].data[134].key = tempvmask.data[134].key;
		vmask[i].data[134].value = tempvmask.data[134].value;
		vmask[i].data[135].key = tempvmask.data[135].key;
		vmask[i].data[135].value = tempvmask.data[135].value;
		vmask[i].data[136].key = tempvmask.data[136].key;
		vmask[i].data[136].value = tempvmask.data[136].value;
		vmask[i].data[137].key = tempvmask.data[137].key;
		vmask[i].data[137].value = tempvmask.data[137].value;
		vmask[i].data[138].key = tempvmask.data[138].key;
		vmask[i].data[138].value = tempvmask.data[138].value;
		vmask[i].data[139].key = tempvmask.data[139].key;
		vmask[i].data[139].value = tempvmask.data[139].value;
		vmask[i].data[140].key = tempvmask.data[140].key;
		vmask[i].data[140].value = tempvmask.data[140].value;
		vmask[i].data[141].key = tempvmask.data[141].key;
		vmask[i].data[141].value = tempvmask.data[141].value;
		vmask[i].data[142].key = tempvmask.data[142].key;
		vmask[i].data[142].value = tempvmask.data[142].value;
		vmask[i].data[143].key = tempvmask.data[143].key;
		vmask[i].data[143].value = tempvmask.data[143].value;
		vmask[i].data[144].key = tempvmask.data[144].key;
		vmask[i].data[144].value = tempvmask.data[144].value;
		vmask[i].data[145].key = tempvmask.data[145].key;
		vmask[i].data[145].value = tempvmask.data[145].value;
		vmask[i].data[146].key = tempvmask.data[146].key;
		vmask[i].data[146].value = tempvmask.data[146].value;
		vmask[i].data[147].key = tempvmask.data[147].key;
		vmask[i].data[147].value = tempvmask.data[147].value;
		vmask[i].data[148].key = tempvmask.data[148].key;
		vmask[i].data[148].value = tempvmask.data[148].value;
		vmask[i].data[149].key = tempvmask.data[149].key;
		vmask[i].data[149].value = tempvmask.data[149].value;
		vmask[i].data[150].key = tempvmask.data[150].key;
		vmask[i].data[150].value = tempvmask.data[150].value;
		vmask[i].data[151].key = tempvmask.data[151].key;
		vmask[i].data[151].value = tempvmask.data[151].value;
		vmask[i].data[152].key = tempvmask.data[152].key;
		vmask[i].data[152].value = tempvmask.data[152].value;
		vmask[i].data[153].key = tempvmask.data[153].key;
		vmask[i].data[153].value = tempvmask.data[153].value;
		vmask[i].data[154].key = tempvmask.data[154].key;
		vmask[i].data[154].value = tempvmask.data[154].value;
		vmask[i].data[155].key = tempvmask.data[155].key;
		vmask[i].data[155].value = tempvmask.data[155].value;
		vmask[i].data[156].key = tempvmask.data[156].key;
		vmask[i].data[156].value = tempvmask.data[156].value;
		vmask[i].data[157].key = tempvmask.data[157].key;
		vmask[i].data[157].value = tempvmask.data[157].value;
		vmask[i].data[158].key = tempvmask.data[158].key;
		vmask[i].data[158].value = tempvmask.data[158].value;
		vmask[i].data[159].key = tempvmask.data[159].key;
		vmask[i].data[159].value = tempvmask.data[159].value;
		vmask[i].data[160].key = tempvmask.data[160].key;
		vmask[i].data[160].value = tempvmask.data[160].value;
		vmask[i].data[161].key = tempvmask.data[161].key;
		vmask[i].data[161].value = tempvmask.data[161].value;
		vmask[i].data[162].key = tempvmask.data[162].key;
		vmask[i].data[162].value = tempvmask.data[162].value;
		vmask[i].data[163].key = tempvmask.data[163].key;
		vmask[i].data[163].value = tempvmask.data[163].value;
		vmask[i].data[164].key = tempvmask.data[164].key;
		vmask[i].data[164].value = tempvmask.data[164].value;
		vmask[i].data[165].key = tempvmask.data[165].key;
		vmask[i].data[165].value = tempvmask.data[165].value;
		vmask[i].data[166].key = tempvmask.data[166].key;
		vmask[i].data[166].value = tempvmask.data[166].value;
		vmask[i].data[167].key = tempvmask.data[167].key;
		vmask[i].data[167].value = tempvmask.data[167].value;
		vmask[i].data[168].key = tempvmask.data[168].key;
		vmask[i].data[168].value = tempvmask.data[168].value;
		vmask[i].data[169].key = tempvmask.data[169].key;
		vmask[i].data[169].value = tempvmask.data[169].value;
		vmask[i].data[170].key = tempvmask.data[170].key;
		vmask[i].data[170].value = tempvmask.data[170].value;
		vmask[i].data[171].key = tempvmask.data[171].key;
		vmask[i].data[171].value = tempvmask.data[171].value;
		vmask[i].data[172].key = tempvmask.data[172].key;
		vmask[i].data[172].value = tempvmask.data[172].value;
		vmask[i].data[173].key = tempvmask.data[173].key;
		vmask[i].data[173].value = tempvmask.data[173].value;
		vmask[i].data[174].key = tempvmask.data[174].key;
		vmask[i].data[174].value = tempvmask.data[174].value;
		vmask[i].data[175].key = tempvmask.data[175].key;
		vmask[i].data[175].value = tempvmask.data[175].value;
		vmask[i].data[176].key = tempvmask.data[176].key;
		vmask[i].data[176].value = tempvmask.data[176].value;
		vmask[i].data[177].key = tempvmask.data[177].key;
		vmask[i].data[177].value = tempvmask.data[177].value;
		vmask[i].data[178].key = tempvmask.data[178].key;
		vmask[i].data[178].value = tempvmask.data[178].value;
		vmask[i].data[179].key = tempvmask.data[179].key;
		vmask[i].data[179].value = tempvmask.data[179].value;
		vmask[i].data[180].key = tempvmask.data[180].key;
		vmask[i].data[180].value = tempvmask.data[180].value;
		vmask[i].data[181].key = tempvmask.data[181].key;
		vmask[i].data[181].value = tempvmask.data[181].value;
		vmask[i].data[182].key = tempvmask.data[182].key;
		vmask[i].data[182].value = tempvmask.data[182].value;
		vmask[i].data[183].key = tempvmask.data[183].key;
		vmask[i].data[183].value = tempvmask.data[183].value;
		vmask[i].data[184].key = tempvmask.data[184].key;
		vmask[i].data[184].value = tempvmask.data[184].value;
		vmask[i].data[185].key = tempvmask.data[185].key;
		vmask[i].data[185].value = tempvmask.data[185].value;
		vmask[i].data[186].key = tempvmask.data[186].key;
		vmask[i].data[186].value = tempvmask.data[186].value;
		vmask[i].data[187].key = tempvmask.data[187].key;
		vmask[i].data[187].value = tempvmask.data[187].value;
		vmask[i].data[188].key = tempvmask.data[188].key;
		vmask[i].data[188].value = tempvmask.data[188].value;
		vmask[i].data[189].key = tempvmask.data[189].key;
		vmask[i].data[189].value = tempvmask.data[189].value;
		vmask[i].data[190].key = tempvmask.data[190].key;
		vmask[i].data[190].value = tempvmask.data[190].value;
		vmask[i].data[191].key = tempvmask.data[191].key;
		vmask[i].data[191].value = tempvmask.data[191].value;
		vmask[i].data[192].key = tempvmask.data[192].key;
		vmask[i].data[192].value = tempvmask.data[192].value;
		vmask[i].data[193].key = tempvmask.data[193].key;
		vmask[i].data[193].value = tempvmask.data[193].value;
		vmask[i].data[194].key = tempvmask.data[194].key;
		vmask[i].data[194].value = tempvmask.data[194].value;
		vmask[i].data[195].key = tempvmask.data[195].key;
		vmask[i].data[195].value = tempvmask.data[195].value;
		vmask[i].data[196].key = tempvmask.data[196].key;
		vmask[i].data[196].value = tempvmask.data[196].value;
		vmask[i].data[197].key = tempvmask.data[197].key;
		vmask[i].data[197].value = tempvmask.data[197].value;
		vmask[i].data[198].key = tempvmask.data[198].key;
		vmask[i].data[198].value = tempvmask.data[198].value;
		vmask[i].data[199].key = tempvmask.data[199].key;
		vmask[i].data[199].value = tempvmask.data[199].value;
		vmask[i].data[200].key = tempvmask.data[200].key;
		vmask[i].data[200].value = tempvmask.data[200].value;
		vmask[i].data[201].key = tempvmask.data[201].key;
		vmask[i].data[201].value = tempvmask.data[201].value;
		vmask[i].data[202].key = tempvmask.data[202].key;
		vmask[i].data[202].value = tempvmask.data[202].value;
		vmask[i].data[203].key = tempvmask.data[203].key;
		vmask[i].data[203].value = tempvmask.data[203].value;
		vmask[i].data[204].key = tempvmask.data[204].key;
		vmask[i].data[204].value = tempvmask.data[204].value;
		vmask[i].data[205].key = tempvmask.data[205].key;
		vmask[i].data[205].value = tempvmask.data[205].value;
		vmask[i].data[206].key = tempvmask.data[206].key;
		vmask[i].data[206].value = tempvmask.data[206].value;
		vmask[i].data[207].key = tempvmask.data[207].key;
		vmask[i].data[207].value = tempvmask.data[207].value;
		vmask[i].data[208].key = tempvmask.data[208].key;
		vmask[i].data[208].value = tempvmask.data[208].value;
		vmask[i].data[209].key = tempvmask.data[209].key;
		vmask[i].data[209].value = tempvmask.data[209].value;
		vmask[i].data[210].key = tempvmask.data[210].key;
		vmask[i].data[210].value = tempvmask.data[210].value;
		vmask[i].data[211].key = tempvmask.data[211].key;
		vmask[i].data[211].value = tempvmask.data[211].value;
		vmask[i].data[212].key = tempvmask.data[212].key;
		vmask[i].data[212].value = tempvmask.data[212].value;
		vmask[i].data[213].key = tempvmask.data[213].key;
		vmask[i].data[213].value = tempvmask.data[213].value;
		vmask[i].data[214].key = tempvmask.data[214].key;
		vmask[i].data[214].value = tempvmask.data[214].value;
		vmask[i].data[215].key = tempvmask.data[215].key;
		vmask[i].data[215].value = tempvmask.data[215].value;
		vmask[i].data[216].key = tempvmask.data[216].key;
		vmask[i].data[216].value = tempvmask.data[216].value;
		vmask[i].data[217].key = tempvmask.data[217].key;
		vmask[i].data[217].value = tempvmask.data[217].value;
		vmask[i].data[218].key = tempvmask.data[218].key;
		vmask[i].data[218].value = tempvmask.data[218].value;
		vmask[i].data[219].key = tempvmask.data[219].key;
		vmask[i].data[219].value = tempvmask.data[219].value;
		vmask[i].data[220].key = tempvmask.data[220].key;
		vmask[i].data[220].value = tempvmask.data[220].value;
		vmask[i].data[221].key = tempvmask.data[221].key;
		vmask[i].data[221].value = tempvmask.data[221].value;
		vmask[i].data[222].key = tempvmask.data[222].key;
		vmask[i].data[222].value = tempvmask.data[222].value;
		vmask[i].data[223].key = tempvmask.data[223].key;
		vmask[i].data[223].value = tempvmask.data[223].value;
		vmask[i].data[224].key = tempvmask.data[224].key;
		vmask[i].data[224].value = tempvmask.data[224].value;
		vmask[i].data[225].key = tempvmask.data[225].key;
		vmask[i].data[225].value = tempvmask.data[225].value;
		vmask[i].data[226].key = tempvmask.data[226].key;
		vmask[i].data[226].value = tempvmask.data[226].value;
		vmask[i].data[227].key = tempvmask.data[227].key;
		vmask[i].data[227].value = tempvmask.data[227].value;
		vmask[i].data[228].key = tempvmask.data[228].key;
		vmask[i].data[228].value = tempvmask.data[228].value;
		vmask[i].data[229].key = tempvmask.data[229].key;
		vmask[i].data[229].value = tempvmask.data[229].value;
		vmask[i].data[230].key = tempvmask.data[230].key;
		vmask[i].data[230].value = tempvmask.data[230].value;
		vmask[i].data[231].key = tempvmask.data[231].key;
		vmask[i].data[231].value = tempvmask.data[231].value;
		vmask[i].data[232].key = tempvmask.data[232].key;
		vmask[i].data[232].value = tempvmask.data[232].value;
		vmask[i].data[233].key = tempvmask.data[233].key;
		vmask[i].data[233].value = tempvmask.data[233].value;
		vmask[i].data[234].key = tempvmask.data[234].key;
		vmask[i].data[234].value = tempvmask.data[234].value;
		vmask[i].data[235].key = tempvmask.data[235].key;
		vmask[i].data[235].value = tempvmask.data[235].value;
		vmask[i].data[236].key = tempvmask.data[236].key;
		vmask[i].data[236].value = tempvmask.data[236].value;
		vmask[i].data[237].key = tempvmask.data[237].key;
		vmask[i].data[237].value = tempvmask.data[237].value;
		vmask[i].data[238].key = tempvmask.data[238].key;
		vmask[i].data[238].value = tempvmask.data[238].value;
		vmask[i].data[239].key = tempvmask.data[239].key;
		vmask[i].data[239].value = tempvmask.data[239].value;
		vmask[i].data[240].key = tempvmask.data[240].key;
		vmask[i].data[240].value = tempvmask.data[240].value;
		vmask[i].data[241].key = tempvmask.data[241].key;
		vmask[i].data[241].value = tempvmask.data[241].value;
		vmask[i].data[242].key = tempvmask.data[242].key;
		vmask[i].data[242].value = tempvmask.data[242].value;
		vmask[i].data[243].key = tempvmask.data[243].key;
		vmask[i].data[243].value = tempvmask.data[243].value;
		vmask[i].data[244].key = tempvmask.data[244].key;
		vmask[i].data[244].value = tempvmask.data[244].value;
		vmask[i].data[245].key = tempvmask.data[245].key;
		vmask[i].data[245].value = tempvmask.data[245].value;
		vmask[i].data[246].key = tempvmask.data[246].key;
		vmask[i].data[246].value = tempvmask.data[246].value;
		vmask[i].data[247].key = tempvmask.data[247].key;
		vmask[i].data[247].value = tempvmask.data[247].value;
		vmask[i].data[248].key = tempvmask.data[248].key;
		vmask[i].data[248].value = tempvmask.data[248].value;
		vmask[i].data[249].key = tempvmask.data[249].key;
		vmask[i].data[249].value = tempvmask.data[249].value;
		vmask[i].data[250].key = tempvmask.data[250].key;
		vmask[i].data[250].value = tempvmask.data[250].value;
		vmask[i].data[251].key = tempvmask.data[251].key;
		vmask[i].data[251].value = tempvmask.data[251].value;
		vmask[i].data[252].key = tempvmask.data[252].key;
		vmask[i].data[252].value = tempvmask.data[252].value;
		vmask[i].data[253].key = tempvmask.data[253].key;
		vmask[i].data[253].value = tempvmask.data[253].value;
		vmask[i].data[254].key = tempvmask.data[254].key;
		vmask[i].data[254].value = tempvmask.data[254].value;
		vmask[i].data[255].key = tempvmask.data[255].key;
		vmask[i].data[255].value = tempvmask.data[255].value;
	}
	return;
}

void //
	#ifdef SW 
	actsproc::
	#endif 
loadvmasks_subp(bool_type enable, uintNUMPby2_type vmask[BLOCKRAM_SIZE], uintNUMPby2_type vmask_subp[BLOCKRAM_SIZE], buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount3 = BLOCKRAM_SIZE;
	
	buffer_type transfsize = size_kvs * 16;
	
	uintNUMPby2_type tempvmask_sp; 
	#pragma HLS DATA_PACK variable = tempvmask_sp
	tempvmask_sp.data[0].key = 0;
	tempvmask_sp.data[0].value = 0;
	tempvmask_sp.data[1].key = 0;
	tempvmask_sp.data[1].value = 0;
	tempvmask_sp.data[2].key = 0;
	tempvmask_sp.data[2].value = 0;
	tempvmask_sp.data[3].key = 0;
	tempvmask_sp.data[3].value = 0;
	tempvmask_sp.data[4].key = 0;
	tempvmask_sp.data[4].value = 0;
	tempvmask_sp.data[5].key = 0;
	tempvmask_sp.data[5].value = 0;
	tempvmask_sp.data[6].key = 0;
	tempvmask_sp.data[6].value = 0;
	tempvmask_sp.data[7].key = 0;
	tempvmask_sp.data[7].value = 0;
	tempvmask_sp.data[8].key = 0;
	tempvmask_sp.data[8].value = 0;
	tempvmask_sp.data[9].key = 0;
	tempvmask_sp.data[9].value = 0;
	tempvmask_sp.data[10].key = 0;
	tempvmask_sp.data[10].value = 0;
	tempvmask_sp.data[11].key = 0;
	tempvmask_sp.data[11].value = 0;
	tempvmask_sp.data[12].key = 0;
	tempvmask_sp.data[12].value = 0;
	tempvmask_sp.data[13].key = 0;
	tempvmask_sp.data[13].value = 0;
	tempvmask_sp.data[14].key = 0;
	tempvmask_sp.data[14].value = 0;
	tempvmask_sp.data[15].key = 0;
	tempvmask_sp.data[15].value = 0;
	tempvmask_sp.data[16].key = 0;
	tempvmask_sp.data[16].value = 0;
	tempvmask_sp.data[17].key = 0;
	tempvmask_sp.data[17].value = 0;
	tempvmask_sp.data[18].key = 0;
	tempvmask_sp.data[18].value = 0;
	tempvmask_sp.data[19].key = 0;
	tempvmask_sp.data[19].value = 0;
	tempvmask_sp.data[20].key = 0;
	tempvmask_sp.data[20].value = 0;
	tempvmask_sp.data[21].key = 0;
	tempvmask_sp.data[21].value = 0;
	tempvmask_sp.data[22].key = 0;
	tempvmask_sp.data[22].value = 0;
	tempvmask_sp.data[23].key = 0;
	tempvmask_sp.data[23].value = 0;
	tempvmask_sp.data[24].key = 0;
	tempvmask_sp.data[24].value = 0;
	tempvmask_sp.data[25].key = 0;
	tempvmask_sp.data[25].value = 0;
	tempvmask_sp.data[26].key = 0;
	tempvmask_sp.data[26].value = 0;
	tempvmask_sp.data[27].key = 0;
	tempvmask_sp.data[27].value = 0;
	tempvmask_sp.data[28].key = 0;
	tempvmask_sp.data[28].value = 0;
	tempvmask_sp.data[29].key = 0;
	tempvmask_sp.data[29].value = 0;
	tempvmask_sp.data[30].key = 0;
	tempvmask_sp.data[30].value = 0;
	tempvmask_sp.data[31].key = 0;
	tempvmask_sp.data[31].value = 0;
	tempvmask_sp.data[32].key = 0;
	tempvmask_sp.data[32].value = 0;
	tempvmask_sp.data[33].key = 0;
	tempvmask_sp.data[33].value = 0;
	tempvmask_sp.data[34].key = 0;
	tempvmask_sp.data[34].value = 0;
	tempvmask_sp.data[35].key = 0;
	tempvmask_sp.data[35].value = 0;
	tempvmask_sp.data[36].key = 0;
	tempvmask_sp.data[36].value = 0;
	tempvmask_sp.data[37].key = 0;
	tempvmask_sp.data[37].value = 0;
	tempvmask_sp.data[38].key = 0;
	tempvmask_sp.data[38].value = 0;
	tempvmask_sp.data[39].key = 0;
	tempvmask_sp.data[39].value = 0;
	tempvmask_sp.data[40].key = 0;
	tempvmask_sp.data[40].value = 0;
	tempvmask_sp.data[41].key = 0;
	tempvmask_sp.data[41].value = 0;
	tempvmask_sp.data[42].key = 0;
	tempvmask_sp.data[42].value = 0;
	tempvmask_sp.data[43].key = 0;
	tempvmask_sp.data[43].value = 0;
	tempvmask_sp.data[44].key = 0;
	tempvmask_sp.data[44].value = 0;
	tempvmask_sp.data[45].key = 0;
	tempvmask_sp.data[45].value = 0;
	tempvmask_sp.data[46].key = 0;
	tempvmask_sp.data[46].value = 0;
	tempvmask_sp.data[47].key = 0;
	tempvmask_sp.data[47].value = 0;
	tempvmask_sp.data[48].key = 0;
	tempvmask_sp.data[48].value = 0;
	tempvmask_sp.data[49].key = 0;
	tempvmask_sp.data[49].value = 0;
	tempvmask_sp.data[50].key = 0;
	tempvmask_sp.data[50].value = 0;
	tempvmask_sp.data[51].key = 0;
	tempvmask_sp.data[51].value = 0;
	tempvmask_sp.data[52].key = 0;
	tempvmask_sp.data[52].value = 0;
	tempvmask_sp.data[53].key = 0;
	tempvmask_sp.data[53].value = 0;
	tempvmask_sp.data[54].key = 0;
	tempvmask_sp.data[54].value = 0;
	tempvmask_sp.data[55].key = 0;
	tempvmask_sp.data[55].value = 0;
	tempvmask_sp.data[56].key = 0;
	tempvmask_sp.data[56].value = 0;
	tempvmask_sp.data[57].key = 0;
	tempvmask_sp.data[57].value = 0;
	tempvmask_sp.data[58].key = 0;
	tempvmask_sp.data[58].value = 0;
	tempvmask_sp.data[59].key = 0;
	tempvmask_sp.data[59].value = 0;
	tempvmask_sp.data[60].key = 0;
	tempvmask_sp.data[60].value = 0;
	tempvmask_sp.data[61].key = 0;
	tempvmask_sp.data[61].value = 0;
	tempvmask_sp.data[62].key = 0;
	tempvmask_sp.data[62].value = 0;
	tempvmask_sp.data[63].key = 0;
	tempvmask_sp.data[63].value = 0;
	tempvmask_sp.data[64].key = 0;
	tempvmask_sp.data[64].value = 0;
	tempvmask_sp.data[65].key = 0;
	tempvmask_sp.data[65].value = 0;
	tempvmask_sp.data[66].key = 0;
	tempvmask_sp.data[66].value = 0;
	tempvmask_sp.data[67].key = 0;
	tempvmask_sp.data[67].value = 0;
	tempvmask_sp.data[68].key = 0;
	tempvmask_sp.data[68].value = 0;
	tempvmask_sp.data[69].key = 0;
	tempvmask_sp.data[69].value = 0;
	tempvmask_sp.data[70].key = 0;
	tempvmask_sp.data[70].value = 0;
	tempvmask_sp.data[71].key = 0;
	tempvmask_sp.data[71].value = 0;
	tempvmask_sp.data[72].key = 0;
	tempvmask_sp.data[72].value = 0;
	tempvmask_sp.data[73].key = 0;
	tempvmask_sp.data[73].value = 0;
	tempvmask_sp.data[74].key = 0;
	tempvmask_sp.data[74].value = 0;
	tempvmask_sp.data[75].key = 0;
	tempvmask_sp.data[75].value = 0;
	tempvmask_sp.data[76].key = 0;
	tempvmask_sp.data[76].value = 0;
	tempvmask_sp.data[77].key = 0;
	tempvmask_sp.data[77].value = 0;
	tempvmask_sp.data[78].key = 0;
	tempvmask_sp.data[78].value = 0;
	tempvmask_sp.data[79].key = 0;
	tempvmask_sp.data[79].value = 0;
	tempvmask_sp.data[80].key = 0;
	tempvmask_sp.data[80].value = 0;
	tempvmask_sp.data[81].key = 0;
	tempvmask_sp.data[81].value = 0;
	tempvmask_sp.data[82].key = 0;
	tempvmask_sp.data[82].value = 0;
	tempvmask_sp.data[83].key = 0;
	tempvmask_sp.data[83].value = 0;
	tempvmask_sp.data[84].key = 0;
	tempvmask_sp.data[84].value = 0;
	tempvmask_sp.data[85].key = 0;
	tempvmask_sp.data[85].value = 0;
	tempvmask_sp.data[86].key = 0;
	tempvmask_sp.data[86].value = 0;
	tempvmask_sp.data[87].key = 0;
	tempvmask_sp.data[87].value = 0;
	tempvmask_sp.data[88].key = 0;
	tempvmask_sp.data[88].value = 0;
	tempvmask_sp.data[89].key = 0;
	tempvmask_sp.data[89].value = 0;
	tempvmask_sp.data[90].key = 0;
	tempvmask_sp.data[90].value = 0;
	tempvmask_sp.data[91].key = 0;
	tempvmask_sp.data[91].value = 0;
	tempvmask_sp.data[92].key = 0;
	tempvmask_sp.data[92].value = 0;
	tempvmask_sp.data[93].key = 0;
	tempvmask_sp.data[93].value = 0;
	tempvmask_sp.data[94].key = 0;
	tempvmask_sp.data[94].value = 0;
	tempvmask_sp.data[95].key = 0;
	tempvmask_sp.data[95].value = 0;
	tempvmask_sp.data[96].key = 0;
	tempvmask_sp.data[96].value = 0;
	tempvmask_sp.data[97].key = 0;
	tempvmask_sp.data[97].value = 0;
	tempvmask_sp.data[98].key = 0;
	tempvmask_sp.data[98].value = 0;
	tempvmask_sp.data[99].key = 0;
	tempvmask_sp.data[99].value = 0;
	tempvmask_sp.data[100].key = 0;
	tempvmask_sp.data[100].value = 0;
	tempvmask_sp.data[101].key = 0;
	tempvmask_sp.data[101].value = 0;
	tempvmask_sp.data[102].key = 0;
	tempvmask_sp.data[102].value = 0;
	tempvmask_sp.data[103].key = 0;
	tempvmask_sp.data[103].value = 0;
	tempvmask_sp.data[104].key = 0;
	tempvmask_sp.data[104].value = 0;
	tempvmask_sp.data[105].key = 0;
	tempvmask_sp.data[105].value = 0;
	tempvmask_sp.data[106].key = 0;
	tempvmask_sp.data[106].value = 0;
	tempvmask_sp.data[107].key = 0;
	tempvmask_sp.data[107].value = 0;
	tempvmask_sp.data[108].key = 0;
	tempvmask_sp.data[108].value = 0;
	tempvmask_sp.data[109].key = 0;
	tempvmask_sp.data[109].value = 0;
	tempvmask_sp.data[110].key = 0;
	tempvmask_sp.data[110].value = 0;
	tempvmask_sp.data[111].key = 0;
	tempvmask_sp.data[111].value = 0;
	tempvmask_sp.data[112].key = 0;
	tempvmask_sp.data[112].value = 0;
	tempvmask_sp.data[113].key = 0;
	tempvmask_sp.data[113].value = 0;
	tempvmask_sp.data[114].key = 0;
	tempvmask_sp.data[114].value = 0;
	tempvmask_sp.data[115].key = 0;
	tempvmask_sp.data[115].value = 0;
	tempvmask_sp.data[116].key = 0;
	tempvmask_sp.data[116].value = 0;
	tempvmask_sp.data[117].key = 0;
	tempvmask_sp.data[117].value = 0;
	tempvmask_sp.data[118].key = 0;
	tempvmask_sp.data[118].value = 0;
	tempvmask_sp.data[119].key = 0;
	tempvmask_sp.data[119].value = 0;
	tempvmask_sp.data[120].key = 0;
	tempvmask_sp.data[120].value = 0;
	tempvmask_sp.data[121].key = 0;
	tempvmask_sp.data[121].value = 0;
	tempvmask_sp.data[122].key = 0;
	tempvmask_sp.data[122].value = 0;
	tempvmask_sp.data[123].key = 0;
	tempvmask_sp.data[123].value = 0;
	tempvmask_sp.data[124].key = 0;
	tempvmask_sp.data[124].value = 0;
	tempvmask_sp.data[125].key = 0;
	tempvmask_sp.data[125].value = 0;
	tempvmask_sp.data[126].key = 0;
	tempvmask_sp.data[126].value = 0;
	tempvmask_sp.data[127].key = 0;
	tempvmask_sp.data[127].value = 0;
	tempvmask_sp.data[128].key = 0;
	tempvmask_sp.data[128].value = 0;
	tempvmask_sp.data[129].key = 0;
	tempvmask_sp.data[129].value = 0;
	tempvmask_sp.data[130].key = 0;
	tempvmask_sp.data[130].value = 0;
	tempvmask_sp.data[131].key = 0;
	tempvmask_sp.data[131].value = 0;
	tempvmask_sp.data[132].key = 0;
	tempvmask_sp.data[132].value = 0;
	tempvmask_sp.data[133].key = 0;
	tempvmask_sp.data[133].value = 0;
	tempvmask_sp.data[134].key = 0;
	tempvmask_sp.data[134].value = 0;
	tempvmask_sp.data[135].key = 0;
	tempvmask_sp.data[135].value = 0;
	tempvmask_sp.data[136].key = 0;
	tempvmask_sp.data[136].value = 0;
	tempvmask_sp.data[137].key = 0;
	tempvmask_sp.data[137].value = 0;
	tempvmask_sp.data[138].key = 0;
	tempvmask_sp.data[138].value = 0;
	tempvmask_sp.data[139].key = 0;
	tempvmask_sp.data[139].value = 0;
	tempvmask_sp.data[140].key = 0;
	tempvmask_sp.data[140].value = 0;
	tempvmask_sp.data[141].key = 0;
	tempvmask_sp.data[141].value = 0;
	tempvmask_sp.data[142].key = 0;
	tempvmask_sp.data[142].value = 0;
	tempvmask_sp.data[143].key = 0;
	tempvmask_sp.data[143].value = 0;
	tempvmask_sp.data[144].key = 0;
	tempvmask_sp.data[144].value = 0;
	tempvmask_sp.data[145].key = 0;
	tempvmask_sp.data[145].value = 0;
	tempvmask_sp.data[146].key = 0;
	tempvmask_sp.data[146].value = 0;
	tempvmask_sp.data[147].key = 0;
	tempvmask_sp.data[147].value = 0;
	tempvmask_sp.data[148].key = 0;
	tempvmask_sp.data[148].value = 0;
	tempvmask_sp.data[149].key = 0;
	tempvmask_sp.data[149].value = 0;
	tempvmask_sp.data[150].key = 0;
	tempvmask_sp.data[150].value = 0;
	tempvmask_sp.data[151].key = 0;
	tempvmask_sp.data[151].value = 0;
	tempvmask_sp.data[152].key = 0;
	tempvmask_sp.data[152].value = 0;
	tempvmask_sp.data[153].key = 0;
	tempvmask_sp.data[153].value = 0;
	tempvmask_sp.data[154].key = 0;
	tempvmask_sp.data[154].value = 0;
	tempvmask_sp.data[155].key = 0;
	tempvmask_sp.data[155].value = 0;
	tempvmask_sp.data[156].key = 0;
	tempvmask_sp.data[156].value = 0;
	tempvmask_sp.data[157].key = 0;
	tempvmask_sp.data[157].value = 0;
	tempvmask_sp.data[158].key = 0;
	tempvmask_sp.data[158].value = 0;
	tempvmask_sp.data[159].key = 0;
	tempvmask_sp.data[159].value = 0;
	tempvmask_sp.data[160].key = 0;
	tempvmask_sp.data[160].value = 0;
	tempvmask_sp.data[161].key = 0;
	tempvmask_sp.data[161].value = 0;
	tempvmask_sp.data[162].key = 0;
	tempvmask_sp.data[162].value = 0;
	tempvmask_sp.data[163].key = 0;
	tempvmask_sp.data[163].value = 0;
	tempvmask_sp.data[164].key = 0;
	tempvmask_sp.data[164].value = 0;
	tempvmask_sp.data[165].key = 0;
	tempvmask_sp.data[165].value = 0;
	tempvmask_sp.data[166].key = 0;
	tempvmask_sp.data[166].value = 0;
	tempvmask_sp.data[167].key = 0;
	tempvmask_sp.data[167].value = 0;
	tempvmask_sp.data[168].key = 0;
	tempvmask_sp.data[168].value = 0;
	tempvmask_sp.data[169].key = 0;
	tempvmask_sp.data[169].value = 0;
	tempvmask_sp.data[170].key = 0;
	tempvmask_sp.data[170].value = 0;
	tempvmask_sp.data[171].key = 0;
	tempvmask_sp.data[171].value = 0;
	tempvmask_sp.data[172].key = 0;
	tempvmask_sp.data[172].value = 0;
	tempvmask_sp.data[173].key = 0;
	tempvmask_sp.data[173].value = 0;
	tempvmask_sp.data[174].key = 0;
	tempvmask_sp.data[174].value = 0;
	tempvmask_sp.data[175].key = 0;
	tempvmask_sp.data[175].value = 0;
	tempvmask_sp.data[176].key = 0;
	tempvmask_sp.data[176].value = 0;
	tempvmask_sp.data[177].key = 0;
	tempvmask_sp.data[177].value = 0;
	tempvmask_sp.data[178].key = 0;
	tempvmask_sp.data[178].value = 0;
	tempvmask_sp.data[179].key = 0;
	tempvmask_sp.data[179].value = 0;
	tempvmask_sp.data[180].key = 0;
	tempvmask_sp.data[180].value = 0;
	tempvmask_sp.data[181].key = 0;
	tempvmask_sp.data[181].value = 0;
	tempvmask_sp.data[182].key = 0;
	tempvmask_sp.data[182].value = 0;
	tempvmask_sp.data[183].key = 0;
	tempvmask_sp.data[183].value = 0;
	tempvmask_sp.data[184].key = 0;
	tempvmask_sp.data[184].value = 0;
	tempvmask_sp.data[185].key = 0;
	tempvmask_sp.data[185].value = 0;
	tempvmask_sp.data[186].key = 0;
	tempvmask_sp.data[186].value = 0;
	tempvmask_sp.data[187].key = 0;
	tempvmask_sp.data[187].value = 0;
	tempvmask_sp.data[188].key = 0;
	tempvmask_sp.data[188].value = 0;
	tempvmask_sp.data[189].key = 0;
	tempvmask_sp.data[189].value = 0;
	tempvmask_sp.data[190].key = 0;
	tempvmask_sp.data[190].value = 0;
	tempvmask_sp.data[191].key = 0;
	tempvmask_sp.data[191].value = 0;
	tempvmask_sp.data[192].key = 0;
	tempvmask_sp.data[192].value = 0;
	tempvmask_sp.data[193].key = 0;
	tempvmask_sp.data[193].value = 0;
	tempvmask_sp.data[194].key = 0;
	tempvmask_sp.data[194].value = 0;
	tempvmask_sp.data[195].key = 0;
	tempvmask_sp.data[195].value = 0;
	tempvmask_sp.data[196].key = 0;
	tempvmask_sp.data[196].value = 0;
	tempvmask_sp.data[197].key = 0;
	tempvmask_sp.data[197].value = 0;
	tempvmask_sp.data[198].key = 0;
	tempvmask_sp.data[198].value = 0;
	tempvmask_sp.data[199].key = 0;
	tempvmask_sp.data[199].value = 0;
	tempvmask_sp.data[200].key = 0;
	tempvmask_sp.data[200].value = 0;
	tempvmask_sp.data[201].key = 0;
	tempvmask_sp.data[201].value = 0;
	tempvmask_sp.data[202].key = 0;
	tempvmask_sp.data[202].value = 0;
	tempvmask_sp.data[203].key = 0;
	tempvmask_sp.data[203].value = 0;
	tempvmask_sp.data[204].key = 0;
	tempvmask_sp.data[204].value = 0;
	tempvmask_sp.data[205].key = 0;
	tempvmask_sp.data[205].value = 0;
	tempvmask_sp.data[206].key = 0;
	tempvmask_sp.data[206].value = 0;
	tempvmask_sp.data[207].key = 0;
	tempvmask_sp.data[207].value = 0;
	tempvmask_sp.data[208].key = 0;
	tempvmask_sp.data[208].value = 0;
	tempvmask_sp.data[209].key = 0;
	tempvmask_sp.data[209].value = 0;
	tempvmask_sp.data[210].key = 0;
	tempvmask_sp.data[210].value = 0;
	tempvmask_sp.data[211].key = 0;
	tempvmask_sp.data[211].value = 0;
	tempvmask_sp.data[212].key = 0;
	tempvmask_sp.data[212].value = 0;
	tempvmask_sp.data[213].key = 0;
	tempvmask_sp.data[213].value = 0;
	tempvmask_sp.data[214].key = 0;
	tempvmask_sp.data[214].value = 0;
	tempvmask_sp.data[215].key = 0;
	tempvmask_sp.data[215].value = 0;
	tempvmask_sp.data[216].key = 0;
	tempvmask_sp.data[216].value = 0;
	tempvmask_sp.data[217].key = 0;
	tempvmask_sp.data[217].value = 0;
	tempvmask_sp.data[218].key = 0;
	tempvmask_sp.data[218].value = 0;
	tempvmask_sp.data[219].key = 0;
	tempvmask_sp.data[219].value = 0;
	tempvmask_sp.data[220].key = 0;
	tempvmask_sp.data[220].value = 0;
	tempvmask_sp.data[221].key = 0;
	tempvmask_sp.data[221].value = 0;
	tempvmask_sp.data[222].key = 0;
	tempvmask_sp.data[222].value = 0;
	tempvmask_sp.data[223].key = 0;
	tempvmask_sp.data[223].value = 0;
	tempvmask_sp.data[224].key = 0;
	tempvmask_sp.data[224].value = 0;
	tempvmask_sp.data[225].key = 0;
	tempvmask_sp.data[225].value = 0;
	tempvmask_sp.data[226].key = 0;
	tempvmask_sp.data[226].value = 0;
	tempvmask_sp.data[227].key = 0;
	tempvmask_sp.data[227].value = 0;
	tempvmask_sp.data[228].key = 0;
	tempvmask_sp.data[228].value = 0;
	tempvmask_sp.data[229].key = 0;
	tempvmask_sp.data[229].value = 0;
	tempvmask_sp.data[230].key = 0;
	tempvmask_sp.data[230].value = 0;
	tempvmask_sp.data[231].key = 0;
	tempvmask_sp.data[231].value = 0;
	tempvmask_sp.data[232].key = 0;
	tempvmask_sp.data[232].value = 0;
	tempvmask_sp.data[233].key = 0;
	tempvmask_sp.data[233].value = 0;
	tempvmask_sp.data[234].key = 0;
	tempvmask_sp.data[234].value = 0;
	tempvmask_sp.data[235].key = 0;
	tempvmask_sp.data[235].value = 0;
	tempvmask_sp.data[236].key = 0;
	tempvmask_sp.data[236].value = 0;
	tempvmask_sp.data[237].key = 0;
	tempvmask_sp.data[237].value = 0;
	tempvmask_sp.data[238].key = 0;
	tempvmask_sp.data[238].value = 0;
	tempvmask_sp.data[239].key = 0;
	tempvmask_sp.data[239].value = 0;
	tempvmask_sp.data[240].key = 0;
	tempvmask_sp.data[240].value = 0;
	tempvmask_sp.data[241].key = 0;
	tempvmask_sp.data[241].value = 0;
	tempvmask_sp.data[242].key = 0;
	tempvmask_sp.data[242].value = 0;
	tempvmask_sp.data[243].key = 0;
	tempvmask_sp.data[243].value = 0;
	tempvmask_sp.data[244].key = 0;
	tempvmask_sp.data[244].value = 0;
	tempvmask_sp.data[245].key = 0;
	tempvmask_sp.data[245].value = 0;
	tempvmask_sp.data[246].key = 0;
	tempvmask_sp.data[246].value = 0;
	tempvmask_sp.data[247].key = 0;
	tempvmask_sp.data[247].value = 0;
	tempvmask_sp.data[248].key = 0;
	tempvmask_sp.data[248].value = 0;
	tempvmask_sp.data[249].key = 0;
	tempvmask_sp.data[249].value = 0;
	tempvmask_sp.data[250].key = 0;
	tempvmask_sp.data[250].value = 0;
	tempvmask_sp.data[251].key = 0;
	tempvmask_sp.data[251].value = 0;
	tempvmask_sp.data[252].key = 0;
	tempvmask_sp.data[252].value = 0;
	tempvmask_sp.data[253].key = 0;
	tempvmask_sp.data[253].value = 0;
	tempvmask_sp.data[254].key = 0;
	tempvmask_sp.data[254].value = 0;
	tempvmask_sp.data[255].key = 0;
	tempvmask_sp.data[255].value = 0;
	
	unsigned int count = 0;
	
	LOADVMASKS_LOOP2: for (buffer_type i=0; i<transfsize; i++){ // transfsize, reducebuffersz
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount3 avg=analysis_loopcount3
	#pragma HLS PIPELINE II=1
		#ifdef SUBPMASK
		tempvmask_sp.data[0].key = tempvmask_sp.data[0].key | vmask[i].data[0].key;
		tempvmask_sp.data[0].value = tempvmask_sp.data[0].value | vmask[i].data[0].value;
		tempvmask_sp.data[1].key = tempvmask_sp.data[1].key | vmask[i].data[1].key;
		tempvmask_sp.data[1].value = tempvmask_sp.data[1].value | vmask[i].data[1].value;
		tempvmask_sp.data[2].key = tempvmask_sp.data[2].key | vmask[i].data[2].key;
		tempvmask_sp.data[2].value = tempvmask_sp.data[2].value | vmask[i].data[2].value;
		tempvmask_sp.data[3].key = tempvmask_sp.data[3].key | vmask[i].data[3].key;
		tempvmask_sp.data[3].value = tempvmask_sp.data[3].value | vmask[i].data[3].value;
		tempvmask_sp.data[4].key = tempvmask_sp.data[4].key | vmask[i].data[4].key;
		tempvmask_sp.data[4].value = tempvmask_sp.data[4].value | vmask[i].data[4].value;
		tempvmask_sp.data[5].key = tempvmask_sp.data[5].key | vmask[i].data[5].key;
		tempvmask_sp.data[5].value = tempvmask_sp.data[5].value | vmask[i].data[5].value;
		tempvmask_sp.data[6].key = tempvmask_sp.data[6].key | vmask[i].data[6].key;
		tempvmask_sp.data[6].value = tempvmask_sp.data[6].value | vmask[i].data[6].value;
		tempvmask_sp.data[7].key = tempvmask_sp.data[7].key | vmask[i].data[7].key;
		tempvmask_sp.data[7].value = tempvmask_sp.data[7].value | vmask[i].data[7].value;
		tempvmask_sp.data[8].key = tempvmask_sp.data[8].key | vmask[i].data[8].key;
		tempvmask_sp.data[8].value = tempvmask_sp.data[8].value | vmask[i].data[8].value;
		tempvmask_sp.data[9].key = tempvmask_sp.data[9].key | vmask[i].data[9].key;
		tempvmask_sp.data[9].value = tempvmask_sp.data[9].value | vmask[i].data[9].value;
		tempvmask_sp.data[10].key = tempvmask_sp.data[10].key | vmask[i].data[10].key;
		tempvmask_sp.data[10].value = tempvmask_sp.data[10].value | vmask[i].data[10].value;
		tempvmask_sp.data[11].key = tempvmask_sp.data[11].key | vmask[i].data[11].key;
		tempvmask_sp.data[11].value = tempvmask_sp.data[11].value | vmask[i].data[11].value;
		tempvmask_sp.data[12].key = tempvmask_sp.data[12].key | vmask[i].data[12].key;
		tempvmask_sp.data[12].value = tempvmask_sp.data[12].value | vmask[i].data[12].value;
		tempvmask_sp.data[13].key = tempvmask_sp.data[13].key | vmask[i].data[13].key;
		tempvmask_sp.data[13].value = tempvmask_sp.data[13].value | vmask[i].data[13].value;
		tempvmask_sp.data[14].key = tempvmask_sp.data[14].key | vmask[i].data[14].key;
		tempvmask_sp.data[14].value = tempvmask_sp.data[14].value | vmask[i].data[14].value;
		tempvmask_sp.data[15].key = tempvmask_sp.data[15].key | vmask[i].data[15].key;
		tempvmask_sp.data[15].value = tempvmask_sp.data[15].value | vmask[i].data[15].value;
		tempvmask_sp.data[16].key = tempvmask_sp.data[16].key | vmask[i].data[16].key;
		tempvmask_sp.data[16].value = tempvmask_sp.data[16].value | vmask[i].data[16].value;
		tempvmask_sp.data[17].key = tempvmask_sp.data[17].key | vmask[i].data[17].key;
		tempvmask_sp.data[17].value = tempvmask_sp.data[17].value | vmask[i].data[17].value;
		tempvmask_sp.data[18].key = tempvmask_sp.data[18].key | vmask[i].data[18].key;
		tempvmask_sp.data[18].value = tempvmask_sp.data[18].value | vmask[i].data[18].value;
		tempvmask_sp.data[19].key = tempvmask_sp.data[19].key | vmask[i].data[19].key;
		tempvmask_sp.data[19].value = tempvmask_sp.data[19].value | vmask[i].data[19].value;
		tempvmask_sp.data[20].key = tempvmask_sp.data[20].key | vmask[i].data[20].key;
		tempvmask_sp.data[20].value = tempvmask_sp.data[20].value | vmask[i].data[20].value;
		tempvmask_sp.data[21].key = tempvmask_sp.data[21].key | vmask[i].data[21].key;
		tempvmask_sp.data[21].value = tempvmask_sp.data[21].value | vmask[i].data[21].value;
		tempvmask_sp.data[22].key = tempvmask_sp.data[22].key | vmask[i].data[22].key;
		tempvmask_sp.data[22].value = tempvmask_sp.data[22].value | vmask[i].data[22].value;
		tempvmask_sp.data[23].key = tempvmask_sp.data[23].key | vmask[i].data[23].key;
		tempvmask_sp.data[23].value = tempvmask_sp.data[23].value | vmask[i].data[23].value;
		tempvmask_sp.data[24].key = tempvmask_sp.data[24].key | vmask[i].data[24].key;
		tempvmask_sp.data[24].value = tempvmask_sp.data[24].value | vmask[i].data[24].value;
		tempvmask_sp.data[25].key = tempvmask_sp.data[25].key | vmask[i].data[25].key;
		tempvmask_sp.data[25].value = tempvmask_sp.data[25].value | vmask[i].data[25].value;
		tempvmask_sp.data[26].key = tempvmask_sp.data[26].key | vmask[i].data[26].key;
		tempvmask_sp.data[26].value = tempvmask_sp.data[26].value | vmask[i].data[26].value;
		tempvmask_sp.data[27].key = tempvmask_sp.data[27].key | vmask[i].data[27].key;
		tempvmask_sp.data[27].value = tempvmask_sp.data[27].value | vmask[i].data[27].value;
		tempvmask_sp.data[28].key = tempvmask_sp.data[28].key | vmask[i].data[28].key;
		tempvmask_sp.data[28].value = tempvmask_sp.data[28].value | vmask[i].data[28].value;
		tempvmask_sp.data[29].key = tempvmask_sp.data[29].key | vmask[i].data[29].key;
		tempvmask_sp.data[29].value = tempvmask_sp.data[29].value | vmask[i].data[29].value;
		tempvmask_sp.data[30].key = tempvmask_sp.data[30].key | vmask[i].data[30].key;
		tempvmask_sp.data[30].value = tempvmask_sp.data[30].value | vmask[i].data[30].value;
		tempvmask_sp.data[31].key = tempvmask_sp.data[31].key | vmask[i].data[31].key;
		tempvmask_sp.data[31].value = tempvmask_sp.data[31].value | vmask[i].data[31].value;
		tempvmask_sp.data[32].key = tempvmask_sp.data[32].key | vmask[i].data[32].key;
		tempvmask_sp.data[32].value = tempvmask_sp.data[32].value | vmask[i].data[32].value;
		tempvmask_sp.data[33].key = tempvmask_sp.data[33].key | vmask[i].data[33].key;
		tempvmask_sp.data[33].value = tempvmask_sp.data[33].value | vmask[i].data[33].value;
		tempvmask_sp.data[34].key = tempvmask_sp.data[34].key | vmask[i].data[34].key;
		tempvmask_sp.data[34].value = tempvmask_sp.data[34].value | vmask[i].data[34].value;
		tempvmask_sp.data[35].key = tempvmask_sp.data[35].key | vmask[i].data[35].key;
		tempvmask_sp.data[35].value = tempvmask_sp.data[35].value | vmask[i].data[35].value;
		tempvmask_sp.data[36].key = tempvmask_sp.data[36].key | vmask[i].data[36].key;
		tempvmask_sp.data[36].value = tempvmask_sp.data[36].value | vmask[i].data[36].value;
		tempvmask_sp.data[37].key = tempvmask_sp.data[37].key | vmask[i].data[37].key;
		tempvmask_sp.data[37].value = tempvmask_sp.data[37].value | vmask[i].data[37].value;
		tempvmask_sp.data[38].key = tempvmask_sp.data[38].key | vmask[i].data[38].key;
		tempvmask_sp.data[38].value = tempvmask_sp.data[38].value | vmask[i].data[38].value;
		tempvmask_sp.data[39].key = tempvmask_sp.data[39].key | vmask[i].data[39].key;
		tempvmask_sp.data[39].value = tempvmask_sp.data[39].value | vmask[i].data[39].value;
		tempvmask_sp.data[40].key = tempvmask_sp.data[40].key | vmask[i].data[40].key;
		tempvmask_sp.data[40].value = tempvmask_sp.data[40].value | vmask[i].data[40].value;
		tempvmask_sp.data[41].key = tempvmask_sp.data[41].key | vmask[i].data[41].key;
		tempvmask_sp.data[41].value = tempvmask_sp.data[41].value | vmask[i].data[41].value;
		tempvmask_sp.data[42].key = tempvmask_sp.data[42].key | vmask[i].data[42].key;
		tempvmask_sp.data[42].value = tempvmask_sp.data[42].value | vmask[i].data[42].value;
		tempvmask_sp.data[43].key = tempvmask_sp.data[43].key | vmask[i].data[43].key;
		tempvmask_sp.data[43].value = tempvmask_sp.data[43].value | vmask[i].data[43].value;
		tempvmask_sp.data[44].key = tempvmask_sp.data[44].key | vmask[i].data[44].key;
		tempvmask_sp.data[44].value = tempvmask_sp.data[44].value | vmask[i].data[44].value;
		tempvmask_sp.data[45].key = tempvmask_sp.data[45].key | vmask[i].data[45].key;
		tempvmask_sp.data[45].value = tempvmask_sp.data[45].value | vmask[i].data[45].value;
		tempvmask_sp.data[46].key = tempvmask_sp.data[46].key | vmask[i].data[46].key;
		tempvmask_sp.data[46].value = tempvmask_sp.data[46].value | vmask[i].data[46].value;
		tempvmask_sp.data[47].key = tempvmask_sp.data[47].key | vmask[i].data[47].key;
		tempvmask_sp.data[47].value = tempvmask_sp.data[47].value | vmask[i].data[47].value;
		tempvmask_sp.data[48].key = tempvmask_sp.data[48].key | vmask[i].data[48].key;
		tempvmask_sp.data[48].value = tempvmask_sp.data[48].value | vmask[i].data[48].value;
		tempvmask_sp.data[49].key = tempvmask_sp.data[49].key | vmask[i].data[49].key;
		tempvmask_sp.data[49].value = tempvmask_sp.data[49].value | vmask[i].data[49].value;
		tempvmask_sp.data[50].key = tempvmask_sp.data[50].key | vmask[i].data[50].key;
		tempvmask_sp.data[50].value = tempvmask_sp.data[50].value | vmask[i].data[50].value;
		tempvmask_sp.data[51].key = tempvmask_sp.data[51].key | vmask[i].data[51].key;
		tempvmask_sp.data[51].value = tempvmask_sp.data[51].value | vmask[i].data[51].value;
		tempvmask_sp.data[52].key = tempvmask_sp.data[52].key | vmask[i].data[52].key;
		tempvmask_sp.data[52].value = tempvmask_sp.data[52].value | vmask[i].data[52].value;
		tempvmask_sp.data[53].key = tempvmask_sp.data[53].key | vmask[i].data[53].key;
		tempvmask_sp.data[53].value = tempvmask_sp.data[53].value | vmask[i].data[53].value;
		tempvmask_sp.data[54].key = tempvmask_sp.data[54].key | vmask[i].data[54].key;
		tempvmask_sp.data[54].value = tempvmask_sp.data[54].value | vmask[i].data[54].value;
		tempvmask_sp.data[55].key = tempvmask_sp.data[55].key | vmask[i].data[55].key;
		tempvmask_sp.data[55].value = tempvmask_sp.data[55].value | vmask[i].data[55].value;
		tempvmask_sp.data[56].key = tempvmask_sp.data[56].key | vmask[i].data[56].key;
		tempvmask_sp.data[56].value = tempvmask_sp.data[56].value | vmask[i].data[56].value;
		tempvmask_sp.data[57].key = tempvmask_sp.data[57].key | vmask[i].data[57].key;
		tempvmask_sp.data[57].value = tempvmask_sp.data[57].value | vmask[i].data[57].value;
		tempvmask_sp.data[58].key = tempvmask_sp.data[58].key | vmask[i].data[58].key;
		tempvmask_sp.data[58].value = tempvmask_sp.data[58].value | vmask[i].data[58].value;
		tempvmask_sp.data[59].key = tempvmask_sp.data[59].key | vmask[i].data[59].key;
		tempvmask_sp.data[59].value = tempvmask_sp.data[59].value | vmask[i].data[59].value;
		tempvmask_sp.data[60].key = tempvmask_sp.data[60].key | vmask[i].data[60].key;
		tempvmask_sp.data[60].value = tempvmask_sp.data[60].value | vmask[i].data[60].value;
		tempvmask_sp.data[61].key = tempvmask_sp.data[61].key | vmask[i].data[61].key;
		tempvmask_sp.data[61].value = tempvmask_sp.data[61].value | vmask[i].data[61].value;
		tempvmask_sp.data[62].key = tempvmask_sp.data[62].key | vmask[i].data[62].key;
		tempvmask_sp.data[62].value = tempvmask_sp.data[62].value | vmask[i].data[62].value;
		tempvmask_sp.data[63].key = tempvmask_sp.data[63].key | vmask[i].data[63].key;
		tempvmask_sp.data[63].value = tempvmask_sp.data[63].value | vmask[i].data[63].value;
		tempvmask_sp.data[64].key = tempvmask_sp.data[64].key | vmask[i].data[64].key;
		tempvmask_sp.data[64].value = tempvmask_sp.data[64].value | vmask[i].data[64].value;
		tempvmask_sp.data[65].key = tempvmask_sp.data[65].key | vmask[i].data[65].key;
		tempvmask_sp.data[65].value = tempvmask_sp.data[65].value | vmask[i].data[65].value;
		tempvmask_sp.data[66].key = tempvmask_sp.data[66].key | vmask[i].data[66].key;
		tempvmask_sp.data[66].value = tempvmask_sp.data[66].value | vmask[i].data[66].value;
		tempvmask_sp.data[67].key = tempvmask_sp.data[67].key | vmask[i].data[67].key;
		tempvmask_sp.data[67].value = tempvmask_sp.data[67].value | vmask[i].data[67].value;
		tempvmask_sp.data[68].key = tempvmask_sp.data[68].key | vmask[i].data[68].key;
		tempvmask_sp.data[68].value = tempvmask_sp.data[68].value | vmask[i].data[68].value;
		tempvmask_sp.data[69].key = tempvmask_sp.data[69].key | vmask[i].data[69].key;
		tempvmask_sp.data[69].value = tempvmask_sp.data[69].value | vmask[i].data[69].value;
		tempvmask_sp.data[70].key = tempvmask_sp.data[70].key | vmask[i].data[70].key;
		tempvmask_sp.data[70].value = tempvmask_sp.data[70].value | vmask[i].data[70].value;
		tempvmask_sp.data[71].key = tempvmask_sp.data[71].key | vmask[i].data[71].key;
		tempvmask_sp.data[71].value = tempvmask_sp.data[71].value | vmask[i].data[71].value;
		tempvmask_sp.data[72].key = tempvmask_sp.data[72].key | vmask[i].data[72].key;
		tempvmask_sp.data[72].value = tempvmask_sp.data[72].value | vmask[i].data[72].value;
		tempvmask_sp.data[73].key = tempvmask_sp.data[73].key | vmask[i].data[73].key;
		tempvmask_sp.data[73].value = tempvmask_sp.data[73].value | vmask[i].data[73].value;
		tempvmask_sp.data[74].key = tempvmask_sp.data[74].key | vmask[i].data[74].key;
		tempvmask_sp.data[74].value = tempvmask_sp.data[74].value | vmask[i].data[74].value;
		tempvmask_sp.data[75].key = tempvmask_sp.data[75].key | vmask[i].data[75].key;
		tempvmask_sp.data[75].value = tempvmask_sp.data[75].value | vmask[i].data[75].value;
		tempvmask_sp.data[76].key = tempvmask_sp.data[76].key | vmask[i].data[76].key;
		tempvmask_sp.data[76].value = tempvmask_sp.data[76].value | vmask[i].data[76].value;
		tempvmask_sp.data[77].key = tempvmask_sp.data[77].key | vmask[i].data[77].key;
		tempvmask_sp.data[77].value = tempvmask_sp.data[77].value | vmask[i].data[77].value;
		tempvmask_sp.data[78].key = tempvmask_sp.data[78].key | vmask[i].data[78].key;
		tempvmask_sp.data[78].value = tempvmask_sp.data[78].value | vmask[i].data[78].value;
		tempvmask_sp.data[79].key = tempvmask_sp.data[79].key | vmask[i].data[79].key;
		tempvmask_sp.data[79].value = tempvmask_sp.data[79].value | vmask[i].data[79].value;
		tempvmask_sp.data[80].key = tempvmask_sp.data[80].key | vmask[i].data[80].key;
		tempvmask_sp.data[80].value = tempvmask_sp.data[80].value | vmask[i].data[80].value;
		tempvmask_sp.data[81].key = tempvmask_sp.data[81].key | vmask[i].data[81].key;
		tempvmask_sp.data[81].value = tempvmask_sp.data[81].value | vmask[i].data[81].value;
		tempvmask_sp.data[82].key = tempvmask_sp.data[82].key | vmask[i].data[82].key;
		tempvmask_sp.data[82].value = tempvmask_sp.data[82].value | vmask[i].data[82].value;
		tempvmask_sp.data[83].key = tempvmask_sp.data[83].key | vmask[i].data[83].key;
		tempvmask_sp.data[83].value = tempvmask_sp.data[83].value | vmask[i].data[83].value;
		tempvmask_sp.data[84].key = tempvmask_sp.data[84].key | vmask[i].data[84].key;
		tempvmask_sp.data[84].value = tempvmask_sp.data[84].value | vmask[i].data[84].value;
		tempvmask_sp.data[85].key = tempvmask_sp.data[85].key | vmask[i].data[85].key;
		tempvmask_sp.data[85].value = tempvmask_sp.data[85].value | vmask[i].data[85].value;
		tempvmask_sp.data[86].key = tempvmask_sp.data[86].key | vmask[i].data[86].key;
		tempvmask_sp.data[86].value = tempvmask_sp.data[86].value | vmask[i].data[86].value;
		tempvmask_sp.data[87].key = tempvmask_sp.data[87].key | vmask[i].data[87].key;
		tempvmask_sp.data[87].value = tempvmask_sp.data[87].value | vmask[i].data[87].value;
		tempvmask_sp.data[88].key = tempvmask_sp.data[88].key | vmask[i].data[88].key;
		tempvmask_sp.data[88].value = tempvmask_sp.data[88].value | vmask[i].data[88].value;
		tempvmask_sp.data[89].key = tempvmask_sp.data[89].key | vmask[i].data[89].key;
		tempvmask_sp.data[89].value = tempvmask_sp.data[89].value | vmask[i].data[89].value;
		tempvmask_sp.data[90].key = tempvmask_sp.data[90].key | vmask[i].data[90].key;
		tempvmask_sp.data[90].value = tempvmask_sp.data[90].value | vmask[i].data[90].value;
		tempvmask_sp.data[91].key = tempvmask_sp.data[91].key | vmask[i].data[91].key;
		tempvmask_sp.data[91].value = tempvmask_sp.data[91].value | vmask[i].data[91].value;
		tempvmask_sp.data[92].key = tempvmask_sp.data[92].key | vmask[i].data[92].key;
		tempvmask_sp.data[92].value = tempvmask_sp.data[92].value | vmask[i].data[92].value;
		tempvmask_sp.data[93].key = tempvmask_sp.data[93].key | vmask[i].data[93].key;
		tempvmask_sp.data[93].value = tempvmask_sp.data[93].value | vmask[i].data[93].value;
		tempvmask_sp.data[94].key = tempvmask_sp.data[94].key | vmask[i].data[94].key;
		tempvmask_sp.data[94].value = tempvmask_sp.data[94].value | vmask[i].data[94].value;
		tempvmask_sp.data[95].key = tempvmask_sp.data[95].key | vmask[i].data[95].key;
		tempvmask_sp.data[95].value = tempvmask_sp.data[95].value | vmask[i].data[95].value;
		tempvmask_sp.data[96].key = tempvmask_sp.data[96].key | vmask[i].data[96].key;
		tempvmask_sp.data[96].value = tempvmask_sp.data[96].value | vmask[i].data[96].value;
		tempvmask_sp.data[97].key = tempvmask_sp.data[97].key | vmask[i].data[97].key;
		tempvmask_sp.data[97].value = tempvmask_sp.data[97].value | vmask[i].data[97].value;
		tempvmask_sp.data[98].key = tempvmask_sp.data[98].key | vmask[i].data[98].key;
		tempvmask_sp.data[98].value = tempvmask_sp.data[98].value | vmask[i].data[98].value;
		tempvmask_sp.data[99].key = tempvmask_sp.data[99].key | vmask[i].data[99].key;
		tempvmask_sp.data[99].value = tempvmask_sp.data[99].value | vmask[i].data[99].value;
		tempvmask_sp.data[100].key = tempvmask_sp.data[100].key | vmask[i].data[100].key;
		tempvmask_sp.data[100].value = tempvmask_sp.data[100].value | vmask[i].data[100].value;
		tempvmask_sp.data[101].key = tempvmask_sp.data[101].key | vmask[i].data[101].key;
		tempvmask_sp.data[101].value = tempvmask_sp.data[101].value | vmask[i].data[101].value;
		tempvmask_sp.data[102].key = tempvmask_sp.data[102].key | vmask[i].data[102].key;
		tempvmask_sp.data[102].value = tempvmask_sp.data[102].value | vmask[i].data[102].value;
		tempvmask_sp.data[103].key = tempvmask_sp.data[103].key | vmask[i].data[103].key;
		tempvmask_sp.data[103].value = tempvmask_sp.data[103].value | vmask[i].data[103].value;
		tempvmask_sp.data[104].key = tempvmask_sp.data[104].key | vmask[i].data[104].key;
		tempvmask_sp.data[104].value = tempvmask_sp.data[104].value | vmask[i].data[104].value;
		tempvmask_sp.data[105].key = tempvmask_sp.data[105].key | vmask[i].data[105].key;
		tempvmask_sp.data[105].value = tempvmask_sp.data[105].value | vmask[i].data[105].value;
		tempvmask_sp.data[106].key = tempvmask_sp.data[106].key | vmask[i].data[106].key;
		tempvmask_sp.data[106].value = tempvmask_sp.data[106].value | vmask[i].data[106].value;
		tempvmask_sp.data[107].key = tempvmask_sp.data[107].key | vmask[i].data[107].key;
		tempvmask_sp.data[107].value = tempvmask_sp.data[107].value | vmask[i].data[107].value;
		tempvmask_sp.data[108].key = tempvmask_sp.data[108].key | vmask[i].data[108].key;
		tempvmask_sp.data[108].value = tempvmask_sp.data[108].value | vmask[i].data[108].value;
		tempvmask_sp.data[109].key = tempvmask_sp.data[109].key | vmask[i].data[109].key;
		tempvmask_sp.data[109].value = tempvmask_sp.data[109].value | vmask[i].data[109].value;
		tempvmask_sp.data[110].key = tempvmask_sp.data[110].key | vmask[i].data[110].key;
		tempvmask_sp.data[110].value = tempvmask_sp.data[110].value | vmask[i].data[110].value;
		tempvmask_sp.data[111].key = tempvmask_sp.data[111].key | vmask[i].data[111].key;
		tempvmask_sp.data[111].value = tempvmask_sp.data[111].value | vmask[i].data[111].value;
		tempvmask_sp.data[112].key = tempvmask_sp.data[112].key | vmask[i].data[112].key;
		tempvmask_sp.data[112].value = tempvmask_sp.data[112].value | vmask[i].data[112].value;
		tempvmask_sp.data[113].key = tempvmask_sp.data[113].key | vmask[i].data[113].key;
		tempvmask_sp.data[113].value = tempvmask_sp.data[113].value | vmask[i].data[113].value;
		tempvmask_sp.data[114].key = tempvmask_sp.data[114].key | vmask[i].data[114].key;
		tempvmask_sp.data[114].value = tempvmask_sp.data[114].value | vmask[i].data[114].value;
		tempvmask_sp.data[115].key = tempvmask_sp.data[115].key | vmask[i].data[115].key;
		tempvmask_sp.data[115].value = tempvmask_sp.data[115].value | vmask[i].data[115].value;
		tempvmask_sp.data[116].key = tempvmask_sp.data[116].key | vmask[i].data[116].key;
		tempvmask_sp.data[116].value = tempvmask_sp.data[116].value | vmask[i].data[116].value;
		tempvmask_sp.data[117].key = tempvmask_sp.data[117].key | vmask[i].data[117].key;
		tempvmask_sp.data[117].value = tempvmask_sp.data[117].value | vmask[i].data[117].value;
		tempvmask_sp.data[118].key = tempvmask_sp.data[118].key | vmask[i].data[118].key;
		tempvmask_sp.data[118].value = tempvmask_sp.data[118].value | vmask[i].data[118].value;
		tempvmask_sp.data[119].key = tempvmask_sp.data[119].key | vmask[i].data[119].key;
		tempvmask_sp.data[119].value = tempvmask_sp.data[119].value | vmask[i].data[119].value;
		tempvmask_sp.data[120].key = tempvmask_sp.data[120].key | vmask[i].data[120].key;
		tempvmask_sp.data[120].value = tempvmask_sp.data[120].value | vmask[i].data[120].value;
		tempvmask_sp.data[121].key = tempvmask_sp.data[121].key | vmask[i].data[121].key;
		tempvmask_sp.data[121].value = tempvmask_sp.data[121].value | vmask[i].data[121].value;
		tempvmask_sp.data[122].key = tempvmask_sp.data[122].key | vmask[i].data[122].key;
		tempvmask_sp.data[122].value = tempvmask_sp.data[122].value | vmask[i].data[122].value;
		tempvmask_sp.data[123].key = tempvmask_sp.data[123].key | vmask[i].data[123].key;
		tempvmask_sp.data[123].value = tempvmask_sp.data[123].value | vmask[i].data[123].value;
		tempvmask_sp.data[124].key = tempvmask_sp.data[124].key | vmask[i].data[124].key;
		tempvmask_sp.data[124].value = tempvmask_sp.data[124].value | vmask[i].data[124].value;
		tempvmask_sp.data[125].key = tempvmask_sp.data[125].key | vmask[i].data[125].key;
		tempvmask_sp.data[125].value = tempvmask_sp.data[125].value | vmask[i].data[125].value;
		tempvmask_sp.data[126].key = tempvmask_sp.data[126].key | vmask[i].data[126].key;
		tempvmask_sp.data[126].value = tempvmask_sp.data[126].value | vmask[i].data[126].value;
		tempvmask_sp.data[127].key = tempvmask_sp.data[127].key | vmask[i].data[127].key;
		tempvmask_sp.data[127].value = tempvmask_sp.data[127].value | vmask[i].data[127].value;
		tempvmask_sp.data[128].key = tempvmask_sp.data[128].key | vmask[i].data[128].key;
		tempvmask_sp.data[128].value = tempvmask_sp.data[128].value | vmask[i].data[128].value;
		tempvmask_sp.data[129].key = tempvmask_sp.data[129].key | vmask[i].data[129].key;
		tempvmask_sp.data[129].value = tempvmask_sp.data[129].value | vmask[i].data[129].value;
		tempvmask_sp.data[130].key = tempvmask_sp.data[130].key | vmask[i].data[130].key;
		tempvmask_sp.data[130].value = tempvmask_sp.data[130].value | vmask[i].data[130].value;
		tempvmask_sp.data[131].key = tempvmask_sp.data[131].key | vmask[i].data[131].key;
		tempvmask_sp.data[131].value = tempvmask_sp.data[131].value | vmask[i].data[131].value;
		tempvmask_sp.data[132].key = tempvmask_sp.data[132].key | vmask[i].data[132].key;
		tempvmask_sp.data[132].value = tempvmask_sp.data[132].value | vmask[i].data[132].value;
		tempvmask_sp.data[133].key = tempvmask_sp.data[133].key | vmask[i].data[133].key;
		tempvmask_sp.data[133].value = tempvmask_sp.data[133].value | vmask[i].data[133].value;
		tempvmask_sp.data[134].key = tempvmask_sp.data[134].key | vmask[i].data[134].key;
		tempvmask_sp.data[134].value = tempvmask_sp.data[134].value | vmask[i].data[134].value;
		tempvmask_sp.data[135].key = tempvmask_sp.data[135].key | vmask[i].data[135].key;
		tempvmask_sp.data[135].value = tempvmask_sp.data[135].value | vmask[i].data[135].value;
		tempvmask_sp.data[136].key = tempvmask_sp.data[136].key | vmask[i].data[136].key;
		tempvmask_sp.data[136].value = tempvmask_sp.data[136].value | vmask[i].data[136].value;
		tempvmask_sp.data[137].key = tempvmask_sp.data[137].key | vmask[i].data[137].key;
		tempvmask_sp.data[137].value = tempvmask_sp.data[137].value | vmask[i].data[137].value;
		tempvmask_sp.data[138].key = tempvmask_sp.data[138].key | vmask[i].data[138].key;
		tempvmask_sp.data[138].value = tempvmask_sp.data[138].value | vmask[i].data[138].value;
		tempvmask_sp.data[139].key = tempvmask_sp.data[139].key | vmask[i].data[139].key;
		tempvmask_sp.data[139].value = tempvmask_sp.data[139].value | vmask[i].data[139].value;
		tempvmask_sp.data[140].key = tempvmask_sp.data[140].key | vmask[i].data[140].key;
		tempvmask_sp.data[140].value = tempvmask_sp.data[140].value | vmask[i].data[140].value;
		tempvmask_sp.data[141].key = tempvmask_sp.data[141].key | vmask[i].data[141].key;
		tempvmask_sp.data[141].value = tempvmask_sp.data[141].value | vmask[i].data[141].value;
		tempvmask_sp.data[142].key = tempvmask_sp.data[142].key | vmask[i].data[142].key;
		tempvmask_sp.data[142].value = tempvmask_sp.data[142].value | vmask[i].data[142].value;
		tempvmask_sp.data[143].key = tempvmask_sp.data[143].key | vmask[i].data[143].key;
		tempvmask_sp.data[143].value = tempvmask_sp.data[143].value | vmask[i].data[143].value;
		tempvmask_sp.data[144].key = tempvmask_sp.data[144].key | vmask[i].data[144].key;
		tempvmask_sp.data[144].value = tempvmask_sp.data[144].value | vmask[i].data[144].value;
		tempvmask_sp.data[145].key = tempvmask_sp.data[145].key | vmask[i].data[145].key;
		tempvmask_sp.data[145].value = tempvmask_sp.data[145].value | vmask[i].data[145].value;
		tempvmask_sp.data[146].key = tempvmask_sp.data[146].key | vmask[i].data[146].key;
		tempvmask_sp.data[146].value = tempvmask_sp.data[146].value | vmask[i].data[146].value;
		tempvmask_sp.data[147].key = tempvmask_sp.data[147].key | vmask[i].data[147].key;
		tempvmask_sp.data[147].value = tempvmask_sp.data[147].value | vmask[i].data[147].value;
		tempvmask_sp.data[148].key = tempvmask_sp.data[148].key | vmask[i].data[148].key;
		tempvmask_sp.data[148].value = tempvmask_sp.data[148].value | vmask[i].data[148].value;
		tempvmask_sp.data[149].key = tempvmask_sp.data[149].key | vmask[i].data[149].key;
		tempvmask_sp.data[149].value = tempvmask_sp.data[149].value | vmask[i].data[149].value;
		tempvmask_sp.data[150].key = tempvmask_sp.data[150].key | vmask[i].data[150].key;
		tempvmask_sp.data[150].value = tempvmask_sp.data[150].value | vmask[i].data[150].value;
		tempvmask_sp.data[151].key = tempvmask_sp.data[151].key | vmask[i].data[151].key;
		tempvmask_sp.data[151].value = tempvmask_sp.data[151].value | vmask[i].data[151].value;
		tempvmask_sp.data[152].key = tempvmask_sp.data[152].key | vmask[i].data[152].key;
		tempvmask_sp.data[152].value = tempvmask_sp.data[152].value | vmask[i].data[152].value;
		tempvmask_sp.data[153].key = tempvmask_sp.data[153].key | vmask[i].data[153].key;
		tempvmask_sp.data[153].value = tempvmask_sp.data[153].value | vmask[i].data[153].value;
		tempvmask_sp.data[154].key = tempvmask_sp.data[154].key | vmask[i].data[154].key;
		tempvmask_sp.data[154].value = tempvmask_sp.data[154].value | vmask[i].data[154].value;
		tempvmask_sp.data[155].key = tempvmask_sp.data[155].key | vmask[i].data[155].key;
		tempvmask_sp.data[155].value = tempvmask_sp.data[155].value | vmask[i].data[155].value;
		tempvmask_sp.data[156].key = tempvmask_sp.data[156].key | vmask[i].data[156].key;
		tempvmask_sp.data[156].value = tempvmask_sp.data[156].value | vmask[i].data[156].value;
		tempvmask_sp.data[157].key = tempvmask_sp.data[157].key | vmask[i].data[157].key;
		tempvmask_sp.data[157].value = tempvmask_sp.data[157].value | vmask[i].data[157].value;
		tempvmask_sp.data[158].key = tempvmask_sp.data[158].key | vmask[i].data[158].key;
		tempvmask_sp.data[158].value = tempvmask_sp.data[158].value | vmask[i].data[158].value;
		tempvmask_sp.data[159].key = tempvmask_sp.data[159].key | vmask[i].data[159].key;
		tempvmask_sp.data[159].value = tempvmask_sp.data[159].value | vmask[i].data[159].value;
		tempvmask_sp.data[160].key = tempvmask_sp.data[160].key | vmask[i].data[160].key;
		tempvmask_sp.data[160].value = tempvmask_sp.data[160].value | vmask[i].data[160].value;
		tempvmask_sp.data[161].key = tempvmask_sp.data[161].key | vmask[i].data[161].key;
		tempvmask_sp.data[161].value = tempvmask_sp.data[161].value | vmask[i].data[161].value;
		tempvmask_sp.data[162].key = tempvmask_sp.data[162].key | vmask[i].data[162].key;
		tempvmask_sp.data[162].value = tempvmask_sp.data[162].value | vmask[i].data[162].value;
		tempvmask_sp.data[163].key = tempvmask_sp.data[163].key | vmask[i].data[163].key;
		tempvmask_sp.data[163].value = tempvmask_sp.data[163].value | vmask[i].data[163].value;
		tempvmask_sp.data[164].key = tempvmask_sp.data[164].key | vmask[i].data[164].key;
		tempvmask_sp.data[164].value = tempvmask_sp.data[164].value | vmask[i].data[164].value;
		tempvmask_sp.data[165].key = tempvmask_sp.data[165].key | vmask[i].data[165].key;
		tempvmask_sp.data[165].value = tempvmask_sp.data[165].value | vmask[i].data[165].value;
		tempvmask_sp.data[166].key = tempvmask_sp.data[166].key | vmask[i].data[166].key;
		tempvmask_sp.data[166].value = tempvmask_sp.data[166].value | vmask[i].data[166].value;
		tempvmask_sp.data[167].key = tempvmask_sp.data[167].key | vmask[i].data[167].key;
		tempvmask_sp.data[167].value = tempvmask_sp.data[167].value | vmask[i].data[167].value;
		tempvmask_sp.data[168].key = tempvmask_sp.data[168].key | vmask[i].data[168].key;
		tempvmask_sp.data[168].value = tempvmask_sp.data[168].value | vmask[i].data[168].value;
		tempvmask_sp.data[169].key = tempvmask_sp.data[169].key | vmask[i].data[169].key;
		tempvmask_sp.data[169].value = tempvmask_sp.data[169].value | vmask[i].data[169].value;
		tempvmask_sp.data[170].key = tempvmask_sp.data[170].key | vmask[i].data[170].key;
		tempvmask_sp.data[170].value = tempvmask_sp.data[170].value | vmask[i].data[170].value;
		tempvmask_sp.data[171].key = tempvmask_sp.data[171].key | vmask[i].data[171].key;
		tempvmask_sp.data[171].value = tempvmask_sp.data[171].value | vmask[i].data[171].value;
		tempvmask_sp.data[172].key = tempvmask_sp.data[172].key | vmask[i].data[172].key;
		tempvmask_sp.data[172].value = tempvmask_sp.data[172].value | vmask[i].data[172].value;
		tempvmask_sp.data[173].key = tempvmask_sp.data[173].key | vmask[i].data[173].key;
		tempvmask_sp.data[173].value = tempvmask_sp.data[173].value | vmask[i].data[173].value;
		tempvmask_sp.data[174].key = tempvmask_sp.data[174].key | vmask[i].data[174].key;
		tempvmask_sp.data[174].value = tempvmask_sp.data[174].value | vmask[i].data[174].value;
		tempvmask_sp.data[175].key = tempvmask_sp.data[175].key | vmask[i].data[175].key;
		tempvmask_sp.data[175].value = tempvmask_sp.data[175].value | vmask[i].data[175].value;
		tempvmask_sp.data[176].key = tempvmask_sp.data[176].key | vmask[i].data[176].key;
		tempvmask_sp.data[176].value = tempvmask_sp.data[176].value | vmask[i].data[176].value;
		tempvmask_sp.data[177].key = tempvmask_sp.data[177].key | vmask[i].data[177].key;
		tempvmask_sp.data[177].value = tempvmask_sp.data[177].value | vmask[i].data[177].value;
		tempvmask_sp.data[178].key = tempvmask_sp.data[178].key | vmask[i].data[178].key;
		tempvmask_sp.data[178].value = tempvmask_sp.data[178].value | vmask[i].data[178].value;
		tempvmask_sp.data[179].key = tempvmask_sp.data[179].key | vmask[i].data[179].key;
		tempvmask_sp.data[179].value = tempvmask_sp.data[179].value | vmask[i].data[179].value;
		tempvmask_sp.data[180].key = tempvmask_sp.data[180].key | vmask[i].data[180].key;
		tempvmask_sp.data[180].value = tempvmask_sp.data[180].value | vmask[i].data[180].value;
		tempvmask_sp.data[181].key = tempvmask_sp.data[181].key | vmask[i].data[181].key;
		tempvmask_sp.data[181].value = tempvmask_sp.data[181].value | vmask[i].data[181].value;
		tempvmask_sp.data[182].key = tempvmask_sp.data[182].key | vmask[i].data[182].key;
		tempvmask_sp.data[182].value = tempvmask_sp.data[182].value | vmask[i].data[182].value;
		tempvmask_sp.data[183].key = tempvmask_sp.data[183].key | vmask[i].data[183].key;
		tempvmask_sp.data[183].value = tempvmask_sp.data[183].value | vmask[i].data[183].value;
		tempvmask_sp.data[184].key = tempvmask_sp.data[184].key | vmask[i].data[184].key;
		tempvmask_sp.data[184].value = tempvmask_sp.data[184].value | vmask[i].data[184].value;
		tempvmask_sp.data[185].key = tempvmask_sp.data[185].key | vmask[i].data[185].key;
		tempvmask_sp.data[185].value = tempvmask_sp.data[185].value | vmask[i].data[185].value;
		tempvmask_sp.data[186].key = tempvmask_sp.data[186].key | vmask[i].data[186].key;
		tempvmask_sp.data[186].value = tempvmask_sp.data[186].value | vmask[i].data[186].value;
		tempvmask_sp.data[187].key = tempvmask_sp.data[187].key | vmask[i].data[187].key;
		tempvmask_sp.data[187].value = tempvmask_sp.data[187].value | vmask[i].data[187].value;
		tempvmask_sp.data[188].key = tempvmask_sp.data[188].key | vmask[i].data[188].key;
		tempvmask_sp.data[188].value = tempvmask_sp.data[188].value | vmask[i].data[188].value;
		tempvmask_sp.data[189].key = tempvmask_sp.data[189].key | vmask[i].data[189].key;
		tempvmask_sp.data[189].value = tempvmask_sp.data[189].value | vmask[i].data[189].value;
		tempvmask_sp.data[190].key = tempvmask_sp.data[190].key | vmask[i].data[190].key;
		tempvmask_sp.data[190].value = tempvmask_sp.data[190].value | vmask[i].data[190].value;
		tempvmask_sp.data[191].key = tempvmask_sp.data[191].key | vmask[i].data[191].key;
		tempvmask_sp.data[191].value = tempvmask_sp.data[191].value | vmask[i].data[191].value;
		tempvmask_sp.data[192].key = tempvmask_sp.data[192].key | vmask[i].data[192].key;
		tempvmask_sp.data[192].value = tempvmask_sp.data[192].value | vmask[i].data[192].value;
		tempvmask_sp.data[193].key = tempvmask_sp.data[193].key | vmask[i].data[193].key;
		tempvmask_sp.data[193].value = tempvmask_sp.data[193].value | vmask[i].data[193].value;
		tempvmask_sp.data[194].key = tempvmask_sp.data[194].key | vmask[i].data[194].key;
		tempvmask_sp.data[194].value = tempvmask_sp.data[194].value | vmask[i].data[194].value;
		tempvmask_sp.data[195].key = tempvmask_sp.data[195].key | vmask[i].data[195].key;
		tempvmask_sp.data[195].value = tempvmask_sp.data[195].value | vmask[i].data[195].value;
		tempvmask_sp.data[196].key = tempvmask_sp.data[196].key | vmask[i].data[196].key;
		tempvmask_sp.data[196].value = tempvmask_sp.data[196].value | vmask[i].data[196].value;
		tempvmask_sp.data[197].key = tempvmask_sp.data[197].key | vmask[i].data[197].key;
		tempvmask_sp.data[197].value = tempvmask_sp.data[197].value | vmask[i].data[197].value;
		tempvmask_sp.data[198].key = tempvmask_sp.data[198].key | vmask[i].data[198].key;
		tempvmask_sp.data[198].value = tempvmask_sp.data[198].value | vmask[i].data[198].value;
		tempvmask_sp.data[199].key = tempvmask_sp.data[199].key | vmask[i].data[199].key;
		tempvmask_sp.data[199].value = tempvmask_sp.data[199].value | vmask[i].data[199].value;
		tempvmask_sp.data[200].key = tempvmask_sp.data[200].key | vmask[i].data[200].key;
		tempvmask_sp.data[200].value = tempvmask_sp.data[200].value | vmask[i].data[200].value;
		tempvmask_sp.data[201].key = tempvmask_sp.data[201].key | vmask[i].data[201].key;
		tempvmask_sp.data[201].value = tempvmask_sp.data[201].value | vmask[i].data[201].value;
		tempvmask_sp.data[202].key = tempvmask_sp.data[202].key | vmask[i].data[202].key;
		tempvmask_sp.data[202].value = tempvmask_sp.data[202].value | vmask[i].data[202].value;
		tempvmask_sp.data[203].key = tempvmask_sp.data[203].key | vmask[i].data[203].key;
		tempvmask_sp.data[203].value = tempvmask_sp.data[203].value | vmask[i].data[203].value;
		tempvmask_sp.data[204].key = tempvmask_sp.data[204].key | vmask[i].data[204].key;
		tempvmask_sp.data[204].value = tempvmask_sp.data[204].value | vmask[i].data[204].value;
		tempvmask_sp.data[205].key = tempvmask_sp.data[205].key | vmask[i].data[205].key;
		tempvmask_sp.data[205].value = tempvmask_sp.data[205].value | vmask[i].data[205].value;
		tempvmask_sp.data[206].key = tempvmask_sp.data[206].key | vmask[i].data[206].key;
		tempvmask_sp.data[206].value = tempvmask_sp.data[206].value | vmask[i].data[206].value;
		tempvmask_sp.data[207].key = tempvmask_sp.data[207].key | vmask[i].data[207].key;
		tempvmask_sp.data[207].value = tempvmask_sp.data[207].value | vmask[i].data[207].value;
		tempvmask_sp.data[208].key = tempvmask_sp.data[208].key | vmask[i].data[208].key;
		tempvmask_sp.data[208].value = tempvmask_sp.data[208].value | vmask[i].data[208].value;
		tempvmask_sp.data[209].key = tempvmask_sp.data[209].key | vmask[i].data[209].key;
		tempvmask_sp.data[209].value = tempvmask_sp.data[209].value | vmask[i].data[209].value;
		tempvmask_sp.data[210].key = tempvmask_sp.data[210].key | vmask[i].data[210].key;
		tempvmask_sp.data[210].value = tempvmask_sp.data[210].value | vmask[i].data[210].value;
		tempvmask_sp.data[211].key = tempvmask_sp.data[211].key | vmask[i].data[211].key;
		tempvmask_sp.data[211].value = tempvmask_sp.data[211].value | vmask[i].data[211].value;
		tempvmask_sp.data[212].key = tempvmask_sp.data[212].key | vmask[i].data[212].key;
		tempvmask_sp.data[212].value = tempvmask_sp.data[212].value | vmask[i].data[212].value;
		tempvmask_sp.data[213].key = tempvmask_sp.data[213].key | vmask[i].data[213].key;
		tempvmask_sp.data[213].value = tempvmask_sp.data[213].value | vmask[i].data[213].value;
		tempvmask_sp.data[214].key = tempvmask_sp.data[214].key | vmask[i].data[214].key;
		tempvmask_sp.data[214].value = tempvmask_sp.data[214].value | vmask[i].data[214].value;
		tempvmask_sp.data[215].key = tempvmask_sp.data[215].key | vmask[i].data[215].key;
		tempvmask_sp.data[215].value = tempvmask_sp.data[215].value | vmask[i].data[215].value;
		tempvmask_sp.data[216].key = tempvmask_sp.data[216].key | vmask[i].data[216].key;
		tempvmask_sp.data[216].value = tempvmask_sp.data[216].value | vmask[i].data[216].value;
		tempvmask_sp.data[217].key = tempvmask_sp.data[217].key | vmask[i].data[217].key;
		tempvmask_sp.data[217].value = tempvmask_sp.data[217].value | vmask[i].data[217].value;
		tempvmask_sp.data[218].key = tempvmask_sp.data[218].key | vmask[i].data[218].key;
		tempvmask_sp.data[218].value = tempvmask_sp.data[218].value | vmask[i].data[218].value;
		tempvmask_sp.data[219].key = tempvmask_sp.data[219].key | vmask[i].data[219].key;
		tempvmask_sp.data[219].value = tempvmask_sp.data[219].value | vmask[i].data[219].value;
		tempvmask_sp.data[220].key = tempvmask_sp.data[220].key | vmask[i].data[220].key;
		tempvmask_sp.data[220].value = tempvmask_sp.data[220].value | vmask[i].data[220].value;
		tempvmask_sp.data[221].key = tempvmask_sp.data[221].key | vmask[i].data[221].key;
		tempvmask_sp.data[221].value = tempvmask_sp.data[221].value | vmask[i].data[221].value;
		tempvmask_sp.data[222].key = tempvmask_sp.data[222].key | vmask[i].data[222].key;
		tempvmask_sp.data[222].value = tempvmask_sp.data[222].value | vmask[i].data[222].value;
		tempvmask_sp.data[223].key = tempvmask_sp.data[223].key | vmask[i].data[223].key;
		tempvmask_sp.data[223].value = tempvmask_sp.data[223].value | vmask[i].data[223].value;
		tempvmask_sp.data[224].key = tempvmask_sp.data[224].key | vmask[i].data[224].key;
		tempvmask_sp.data[224].value = tempvmask_sp.data[224].value | vmask[i].data[224].value;
		tempvmask_sp.data[225].key = tempvmask_sp.data[225].key | vmask[i].data[225].key;
		tempvmask_sp.data[225].value = tempvmask_sp.data[225].value | vmask[i].data[225].value;
		tempvmask_sp.data[226].key = tempvmask_sp.data[226].key | vmask[i].data[226].key;
		tempvmask_sp.data[226].value = tempvmask_sp.data[226].value | vmask[i].data[226].value;
		tempvmask_sp.data[227].key = tempvmask_sp.data[227].key | vmask[i].data[227].key;
		tempvmask_sp.data[227].value = tempvmask_sp.data[227].value | vmask[i].data[227].value;
		tempvmask_sp.data[228].key = tempvmask_sp.data[228].key | vmask[i].data[228].key;
		tempvmask_sp.data[228].value = tempvmask_sp.data[228].value | vmask[i].data[228].value;
		tempvmask_sp.data[229].key = tempvmask_sp.data[229].key | vmask[i].data[229].key;
		tempvmask_sp.data[229].value = tempvmask_sp.data[229].value | vmask[i].data[229].value;
		tempvmask_sp.data[230].key = tempvmask_sp.data[230].key | vmask[i].data[230].key;
		tempvmask_sp.data[230].value = tempvmask_sp.data[230].value | vmask[i].data[230].value;
		tempvmask_sp.data[231].key = tempvmask_sp.data[231].key | vmask[i].data[231].key;
		tempvmask_sp.data[231].value = tempvmask_sp.data[231].value | vmask[i].data[231].value;
		tempvmask_sp.data[232].key = tempvmask_sp.data[232].key | vmask[i].data[232].key;
		tempvmask_sp.data[232].value = tempvmask_sp.data[232].value | vmask[i].data[232].value;
		tempvmask_sp.data[233].key = tempvmask_sp.data[233].key | vmask[i].data[233].key;
		tempvmask_sp.data[233].value = tempvmask_sp.data[233].value | vmask[i].data[233].value;
		tempvmask_sp.data[234].key = tempvmask_sp.data[234].key | vmask[i].data[234].key;
		tempvmask_sp.data[234].value = tempvmask_sp.data[234].value | vmask[i].data[234].value;
		tempvmask_sp.data[235].key = tempvmask_sp.data[235].key | vmask[i].data[235].key;
		tempvmask_sp.data[235].value = tempvmask_sp.data[235].value | vmask[i].data[235].value;
		tempvmask_sp.data[236].key = tempvmask_sp.data[236].key | vmask[i].data[236].key;
		tempvmask_sp.data[236].value = tempvmask_sp.data[236].value | vmask[i].data[236].value;
		tempvmask_sp.data[237].key = tempvmask_sp.data[237].key | vmask[i].data[237].key;
		tempvmask_sp.data[237].value = tempvmask_sp.data[237].value | vmask[i].data[237].value;
		tempvmask_sp.data[238].key = tempvmask_sp.data[238].key | vmask[i].data[238].key;
		tempvmask_sp.data[238].value = tempvmask_sp.data[238].value | vmask[i].data[238].value;
		tempvmask_sp.data[239].key = tempvmask_sp.data[239].key | vmask[i].data[239].key;
		tempvmask_sp.data[239].value = tempvmask_sp.data[239].value | vmask[i].data[239].value;
		tempvmask_sp.data[240].key = tempvmask_sp.data[240].key | vmask[i].data[240].key;
		tempvmask_sp.data[240].value = tempvmask_sp.data[240].value | vmask[i].data[240].value;
		tempvmask_sp.data[241].key = tempvmask_sp.data[241].key | vmask[i].data[241].key;
		tempvmask_sp.data[241].value = tempvmask_sp.data[241].value | vmask[i].data[241].value;
		tempvmask_sp.data[242].key = tempvmask_sp.data[242].key | vmask[i].data[242].key;
		tempvmask_sp.data[242].value = tempvmask_sp.data[242].value | vmask[i].data[242].value;
		tempvmask_sp.data[243].key = tempvmask_sp.data[243].key | vmask[i].data[243].key;
		tempvmask_sp.data[243].value = tempvmask_sp.data[243].value | vmask[i].data[243].value;
		tempvmask_sp.data[244].key = tempvmask_sp.data[244].key | vmask[i].data[244].key;
		tempvmask_sp.data[244].value = tempvmask_sp.data[244].value | vmask[i].data[244].value;
		tempvmask_sp.data[245].key = tempvmask_sp.data[245].key | vmask[i].data[245].key;
		tempvmask_sp.data[245].value = tempvmask_sp.data[245].value | vmask[i].data[245].value;
		tempvmask_sp.data[246].key = tempvmask_sp.data[246].key | vmask[i].data[246].key;
		tempvmask_sp.data[246].value = tempvmask_sp.data[246].value | vmask[i].data[246].value;
		tempvmask_sp.data[247].key = tempvmask_sp.data[247].key | vmask[i].data[247].key;
		tempvmask_sp.data[247].value = tempvmask_sp.data[247].value | vmask[i].data[247].value;
		tempvmask_sp.data[248].key = tempvmask_sp.data[248].key | vmask[i].data[248].key;
		tempvmask_sp.data[248].value = tempvmask_sp.data[248].value | vmask[i].data[248].value;
		tempvmask_sp.data[249].key = tempvmask_sp.data[249].key | vmask[i].data[249].key;
		tempvmask_sp.data[249].value = tempvmask_sp.data[249].value | vmask[i].data[249].value;
		tempvmask_sp.data[250].key = tempvmask_sp.data[250].key | vmask[i].data[250].key;
		tempvmask_sp.data[250].value = tempvmask_sp.data[250].value | vmask[i].data[250].value;
		tempvmask_sp.data[251].key = tempvmask_sp.data[251].key | vmask[i].data[251].key;
		tempvmask_sp.data[251].value = tempvmask_sp.data[251].value | vmask[i].data[251].value;
		tempvmask_sp.data[252].key = tempvmask_sp.data[252].key | vmask[i].data[252].key;
		tempvmask_sp.data[252].value = tempvmask_sp.data[252].value | vmask[i].data[252].value;
		tempvmask_sp.data[253].key = tempvmask_sp.data[253].key | vmask[i].data[253].key;
		tempvmask_sp.data[253].value = tempvmask_sp.data[253].value | vmask[i].data[253].value;
		tempvmask_sp.data[254].key = tempvmask_sp.data[254].key | vmask[i].data[254].key;
		tempvmask_sp.data[254].value = tempvmask_sp.data[254].value | vmask[i].data[254].value;
		tempvmask_sp.data[255].key = tempvmask_sp.data[255].key | vmask[i].data[255].key;
		tempvmask_sp.data[255].value = tempvmask_sp.data[255].value | vmask[i].data[255].value;
		
		if(i%SUBPMASKFACTOR == SUBPMASKFACTOR-1){
			vmask_subp[count].data[0].key = tempvmask_sp.data[0].key;
			vmask_subp[count].data[0].value = tempvmask_sp.data[0].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[0].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[0].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[0].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[0].value<<endl;
			vmask_subp[count].data[1].key = tempvmask_sp.data[1].key;
			vmask_subp[count].data[1].value = tempvmask_sp.data[1].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[1].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[1].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[1].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[1].value<<endl;
			vmask_subp[count].data[2].key = tempvmask_sp.data[2].key;
			vmask_subp[count].data[2].value = tempvmask_sp.data[2].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[2].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[2].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[2].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[2].value<<endl;
			vmask_subp[count].data[3].key = tempvmask_sp.data[3].key;
			vmask_subp[count].data[3].value = tempvmask_sp.data[3].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[3].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[3].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[3].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[3].value<<endl;
			vmask_subp[count].data[4].key = tempvmask_sp.data[4].key;
			vmask_subp[count].data[4].value = tempvmask_sp.data[4].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[4].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[4].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[4].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[4].value<<endl;
			vmask_subp[count].data[5].key = tempvmask_sp.data[5].key;
			vmask_subp[count].data[5].value = tempvmask_sp.data[5].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[5].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[5].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[5].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[5].value<<endl;
			vmask_subp[count].data[6].key = tempvmask_sp.data[6].key;
			vmask_subp[count].data[6].value = tempvmask_sp.data[6].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[6].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[6].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[6].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[6].value<<endl;
			vmask_subp[count].data[7].key = tempvmask_sp.data[7].key;
			vmask_subp[count].data[7].value = tempvmask_sp.data[7].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[7].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[7].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[7].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[7].value<<endl;
			vmask_subp[count].data[8].key = tempvmask_sp.data[8].key;
			vmask_subp[count].data[8].value = tempvmask_sp.data[8].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[8].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[8].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[8].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[8].value<<endl;
			vmask_subp[count].data[9].key = tempvmask_sp.data[9].key;
			vmask_subp[count].data[9].value = tempvmask_sp.data[9].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[9].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[9].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[9].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[9].value<<endl;
			vmask_subp[count].data[10].key = tempvmask_sp.data[10].key;
			vmask_subp[count].data[10].value = tempvmask_sp.data[10].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[10].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[10].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[10].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[10].value<<endl;
			vmask_subp[count].data[11].key = tempvmask_sp.data[11].key;
			vmask_subp[count].data[11].value = tempvmask_sp.data[11].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[11].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[11].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[11].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[11].value<<endl;
			vmask_subp[count].data[12].key = tempvmask_sp.data[12].key;
			vmask_subp[count].data[12].value = tempvmask_sp.data[12].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[12].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[12].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[12].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[12].value<<endl;
			vmask_subp[count].data[13].key = tempvmask_sp.data[13].key;
			vmask_subp[count].data[13].value = tempvmask_sp.data[13].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[13].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[13].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[13].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[13].value<<endl;
			vmask_subp[count].data[14].key = tempvmask_sp.data[14].key;
			vmask_subp[count].data[14].value = tempvmask_sp.data[14].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[14].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[14].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[14].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[14].value<<endl;
			vmask_subp[count].data[15].key = tempvmask_sp.data[15].key;
			vmask_subp[count].data[15].value = tempvmask_sp.data[15].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[15].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[15].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[15].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[15].value<<endl;
			vmask_subp[count].data[16].key = tempvmask_sp.data[16].key;
			vmask_subp[count].data[16].value = tempvmask_sp.data[16].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[16].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[16].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[16].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[16].value<<endl;
			vmask_subp[count].data[17].key = tempvmask_sp.data[17].key;
			vmask_subp[count].data[17].value = tempvmask_sp.data[17].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[17].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[17].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[17].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[17].value<<endl;
			vmask_subp[count].data[18].key = tempvmask_sp.data[18].key;
			vmask_subp[count].data[18].value = tempvmask_sp.data[18].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[18].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[18].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[18].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[18].value<<endl;
			vmask_subp[count].data[19].key = tempvmask_sp.data[19].key;
			vmask_subp[count].data[19].value = tempvmask_sp.data[19].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[19].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[19].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[19].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[19].value<<endl;
			vmask_subp[count].data[20].key = tempvmask_sp.data[20].key;
			vmask_subp[count].data[20].value = tempvmask_sp.data[20].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[20].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[20].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[20].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[20].value<<endl;
			vmask_subp[count].data[21].key = tempvmask_sp.data[21].key;
			vmask_subp[count].data[21].value = tempvmask_sp.data[21].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[21].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[21].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[21].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[21].value<<endl;
			vmask_subp[count].data[22].key = tempvmask_sp.data[22].key;
			vmask_subp[count].data[22].value = tempvmask_sp.data[22].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[22].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[22].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[22].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[22].value<<endl;
			vmask_subp[count].data[23].key = tempvmask_sp.data[23].key;
			vmask_subp[count].data[23].value = tempvmask_sp.data[23].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[23].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[23].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[23].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[23].value<<endl;
			vmask_subp[count].data[24].key = tempvmask_sp.data[24].key;
			vmask_subp[count].data[24].value = tempvmask_sp.data[24].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[24].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[24].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[24].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[24].value<<endl;
			vmask_subp[count].data[25].key = tempvmask_sp.data[25].key;
			vmask_subp[count].data[25].value = tempvmask_sp.data[25].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[25].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[25].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[25].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[25].value<<endl;
			vmask_subp[count].data[26].key = tempvmask_sp.data[26].key;
			vmask_subp[count].data[26].value = tempvmask_sp.data[26].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[26].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[26].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[26].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[26].value<<endl;
			vmask_subp[count].data[27].key = tempvmask_sp.data[27].key;
			vmask_subp[count].data[27].value = tempvmask_sp.data[27].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[27].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[27].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[27].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[27].value<<endl;
			vmask_subp[count].data[28].key = tempvmask_sp.data[28].key;
			vmask_subp[count].data[28].value = tempvmask_sp.data[28].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[28].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[28].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[28].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[28].value<<endl;
			vmask_subp[count].data[29].key = tempvmask_sp.data[29].key;
			vmask_subp[count].data[29].value = tempvmask_sp.data[29].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[29].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[29].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[29].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[29].value<<endl;
			vmask_subp[count].data[30].key = tempvmask_sp.data[30].key;
			vmask_subp[count].data[30].value = tempvmask_sp.data[30].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[30].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[30].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[30].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[30].value<<endl;
			vmask_subp[count].data[31].key = tempvmask_sp.data[31].key;
			vmask_subp[count].data[31].value = tempvmask_sp.data[31].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[31].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[31].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[31].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[31].value<<endl;
			vmask_subp[count].data[32].key = tempvmask_sp.data[32].key;
			vmask_subp[count].data[32].value = tempvmask_sp.data[32].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[32].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[32].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[32].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[32].value<<endl;
			vmask_subp[count].data[33].key = tempvmask_sp.data[33].key;
			vmask_subp[count].data[33].value = tempvmask_sp.data[33].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[33].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[33].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[33].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[33].value<<endl;
			vmask_subp[count].data[34].key = tempvmask_sp.data[34].key;
			vmask_subp[count].data[34].value = tempvmask_sp.data[34].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[34].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[34].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[34].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[34].value<<endl;
			vmask_subp[count].data[35].key = tempvmask_sp.data[35].key;
			vmask_subp[count].data[35].value = tempvmask_sp.data[35].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[35].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[35].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[35].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[35].value<<endl;
			vmask_subp[count].data[36].key = tempvmask_sp.data[36].key;
			vmask_subp[count].data[36].value = tempvmask_sp.data[36].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[36].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[36].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[36].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[36].value<<endl;
			vmask_subp[count].data[37].key = tempvmask_sp.data[37].key;
			vmask_subp[count].data[37].value = tempvmask_sp.data[37].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[37].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[37].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[37].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[37].value<<endl;
			vmask_subp[count].data[38].key = tempvmask_sp.data[38].key;
			vmask_subp[count].data[38].value = tempvmask_sp.data[38].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[38].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[38].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[38].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[38].value<<endl;
			vmask_subp[count].data[39].key = tempvmask_sp.data[39].key;
			vmask_subp[count].data[39].value = tempvmask_sp.data[39].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[39].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[39].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[39].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[39].value<<endl;
			vmask_subp[count].data[40].key = tempvmask_sp.data[40].key;
			vmask_subp[count].data[40].value = tempvmask_sp.data[40].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[40].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[40].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[40].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[40].value<<endl;
			vmask_subp[count].data[41].key = tempvmask_sp.data[41].key;
			vmask_subp[count].data[41].value = tempvmask_sp.data[41].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[41].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[41].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[41].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[41].value<<endl;
			vmask_subp[count].data[42].key = tempvmask_sp.data[42].key;
			vmask_subp[count].data[42].value = tempvmask_sp.data[42].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[42].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[42].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[42].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[42].value<<endl;
			vmask_subp[count].data[43].key = tempvmask_sp.data[43].key;
			vmask_subp[count].data[43].value = tempvmask_sp.data[43].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[43].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[43].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[43].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[43].value<<endl;
			vmask_subp[count].data[44].key = tempvmask_sp.data[44].key;
			vmask_subp[count].data[44].value = tempvmask_sp.data[44].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[44].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[44].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[44].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[44].value<<endl;
			vmask_subp[count].data[45].key = tempvmask_sp.data[45].key;
			vmask_subp[count].data[45].value = tempvmask_sp.data[45].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[45].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[45].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[45].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[45].value<<endl;
			vmask_subp[count].data[46].key = tempvmask_sp.data[46].key;
			vmask_subp[count].data[46].value = tempvmask_sp.data[46].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[46].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[46].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[46].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[46].value<<endl;
			vmask_subp[count].data[47].key = tempvmask_sp.data[47].key;
			vmask_subp[count].data[47].value = tempvmask_sp.data[47].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[47].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[47].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[47].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[47].value<<endl;
			vmask_subp[count].data[48].key = tempvmask_sp.data[48].key;
			vmask_subp[count].data[48].value = tempvmask_sp.data[48].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[48].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[48].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[48].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[48].value<<endl;
			vmask_subp[count].data[49].key = tempvmask_sp.data[49].key;
			vmask_subp[count].data[49].value = tempvmask_sp.data[49].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[49].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[49].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[49].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[49].value<<endl;
			vmask_subp[count].data[50].key = tempvmask_sp.data[50].key;
			vmask_subp[count].data[50].value = tempvmask_sp.data[50].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[50].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[50].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[50].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[50].value<<endl;
			vmask_subp[count].data[51].key = tempvmask_sp.data[51].key;
			vmask_subp[count].data[51].value = tempvmask_sp.data[51].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[51].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[51].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[51].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[51].value<<endl;
			vmask_subp[count].data[52].key = tempvmask_sp.data[52].key;
			vmask_subp[count].data[52].value = tempvmask_sp.data[52].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[52].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[52].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[52].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[52].value<<endl;
			vmask_subp[count].data[53].key = tempvmask_sp.data[53].key;
			vmask_subp[count].data[53].value = tempvmask_sp.data[53].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[53].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[53].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[53].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[53].value<<endl;
			vmask_subp[count].data[54].key = tempvmask_sp.data[54].key;
			vmask_subp[count].data[54].value = tempvmask_sp.data[54].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[54].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[54].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[54].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[54].value<<endl;
			vmask_subp[count].data[55].key = tempvmask_sp.data[55].key;
			vmask_subp[count].data[55].value = tempvmask_sp.data[55].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[55].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[55].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[55].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[55].value<<endl;
			vmask_subp[count].data[56].key = tempvmask_sp.data[56].key;
			vmask_subp[count].data[56].value = tempvmask_sp.data[56].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[56].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[56].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[56].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[56].value<<endl;
			vmask_subp[count].data[57].key = tempvmask_sp.data[57].key;
			vmask_subp[count].data[57].value = tempvmask_sp.data[57].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[57].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[57].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[57].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[57].value<<endl;
			vmask_subp[count].data[58].key = tempvmask_sp.data[58].key;
			vmask_subp[count].data[58].value = tempvmask_sp.data[58].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[58].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[58].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[58].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[58].value<<endl;
			vmask_subp[count].data[59].key = tempvmask_sp.data[59].key;
			vmask_subp[count].data[59].value = tempvmask_sp.data[59].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[59].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[59].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[59].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[59].value<<endl;
			vmask_subp[count].data[60].key = tempvmask_sp.data[60].key;
			vmask_subp[count].data[60].value = tempvmask_sp.data[60].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[60].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[60].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[60].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[60].value<<endl;
			vmask_subp[count].data[61].key = tempvmask_sp.data[61].key;
			vmask_subp[count].data[61].value = tempvmask_sp.data[61].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[61].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[61].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[61].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[61].value<<endl;
			vmask_subp[count].data[62].key = tempvmask_sp.data[62].key;
			vmask_subp[count].data[62].value = tempvmask_sp.data[62].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[62].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[62].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[62].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[62].value<<endl;
			vmask_subp[count].data[63].key = tempvmask_sp.data[63].key;
			vmask_subp[count].data[63].value = tempvmask_sp.data[63].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[63].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[63].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[63].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[63].value<<endl;
			vmask_subp[count].data[64].key = tempvmask_sp.data[64].key;
			vmask_subp[count].data[64].value = tempvmask_sp.data[64].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[64].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[64].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[64].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[64].value<<endl;
			vmask_subp[count].data[65].key = tempvmask_sp.data[65].key;
			vmask_subp[count].data[65].value = tempvmask_sp.data[65].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[65].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[65].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[65].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[65].value<<endl;
			vmask_subp[count].data[66].key = tempvmask_sp.data[66].key;
			vmask_subp[count].data[66].value = tempvmask_sp.data[66].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[66].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[66].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[66].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[66].value<<endl;
			vmask_subp[count].data[67].key = tempvmask_sp.data[67].key;
			vmask_subp[count].data[67].value = tempvmask_sp.data[67].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[67].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[67].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[67].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[67].value<<endl;
			vmask_subp[count].data[68].key = tempvmask_sp.data[68].key;
			vmask_subp[count].data[68].value = tempvmask_sp.data[68].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[68].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[68].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[68].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[68].value<<endl;
			vmask_subp[count].data[69].key = tempvmask_sp.data[69].key;
			vmask_subp[count].data[69].value = tempvmask_sp.data[69].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[69].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[69].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[69].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[69].value<<endl;
			vmask_subp[count].data[70].key = tempvmask_sp.data[70].key;
			vmask_subp[count].data[70].value = tempvmask_sp.data[70].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[70].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[70].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[70].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[70].value<<endl;
			vmask_subp[count].data[71].key = tempvmask_sp.data[71].key;
			vmask_subp[count].data[71].value = tempvmask_sp.data[71].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[71].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[71].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[71].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[71].value<<endl;
			vmask_subp[count].data[72].key = tempvmask_sp.data[72].key;
			vmask_subp[count].data[72].value = tempvmask_sp.data[72].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[72].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[72].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[72].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[72].value<<endl;
			vmask_subp[count].data[73].key = tempvmask_sp.data[73].key;
			vmask_subp[count].data[73].value = tempvmask_sp.data[73].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[73].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[73].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[73].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[73].value<<endl;
			vmask_subp[count].data[74].key = tempvmask_sp.data[74].key;
			vmask_subp[count].data[74].value = tempvmask_sp.data[74].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[74].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[74].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[74].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[74].value<<endl;
			vmask_subp[count].data[75].key = tempvmask_sp.data[75].key;
			vmask_subp[count].data[75].value = tempvmask_sp.data[75].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[75].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[75].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[75].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[75].value<<endl;
			vmask_subp[count].data[76].key = tempvmask_sp.data[76].key;
			vmask_subp[count].data[76].value = tempvmask_sp.data[76].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[76].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[76].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[76].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[76].value<<endl;
			vmask_subp[count].data[77].key = tempvmask_sp.data[77].key;
			vmask_subp[count].data[77].value = tempvmask_sp.data[77].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[77].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[77].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[77].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[77].value<<endl;
			vmask_subp[count].data[78].key = tempvmask_sp.data[78].key;
			vmask_subp[count].data[78].value = tempvmask_sp.data[78].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[78].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[78].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[78].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[78].value<<endl;
			vmask_subp[count].data[79].key = tempvmask_sp.data[79].key;
			vmask_subp[count].data[79].value = tempvmask_sp.data[79].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[79].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[79].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[79].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[79].value<<endl;
			vmask_subp[count].data[80].key = tempvmask_sp.data[80].key;
			vmask_subp[count].data[80].value = tempvmask_sp.data[80].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[80].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[80].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[80].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[80].value<<endl;
			vmask_subp[count].data[81].key = tempvmask_sp.data[81].key;
			vmask_subp[count].data[81].value = tempvmask_sp.data[81].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[81].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[81].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[81].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[81].value<<endl;
			vmask_subp[count].data[82].key = tempvmask_sp.data[82].key;
			vmask_subp[count].data[82].value = tempvmask_sp.data[82].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[82].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[82].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[82].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[82].value<<endl;
			vmask_subp[count].data[83].key = tempvmask_sp.data[83].key;
			vmask_subp[count].data[83].value = tempvmask_sp.data[83].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[83].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[83].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[83].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[83].value<<endl;
			vmask_subp[count].data[84].key = tempvmask_sp.data[84].key;
			vmask_subp[count].data[84].value = tempvmask_sp.data[84].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[84].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[84].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[84].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[84].value<<endl;
			vmask_subp[count].data[85].key = tempvmask_sp.data[85].key;
			vmask_subp[count].data[85].value = tempvmask_sp.data[85].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[85].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[85].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[85].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[85].value<<endl;
			vmask_subp[count].data[86].key = tempvmask_sp.data[86].key;
			vmask_subp[count].data[86].value = tempvmask_sp.data[86].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[86].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[86].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[86].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[86].value<<endl;
			vmask_subp[count].data[87].key = tempvmask_sp.data[87].key;
			vmask_subp[count].data[87].value = tempvmask_sp.data[87].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[87].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[87].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[87].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[87].value<<endl;
			vmask_subp[count].data[88].key = tempvmask_sp.data[88].key;
			vmask_subp[count].data[88].value = tempvmask_sp.data[88].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[88].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[88].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[88].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[88].value<<endl;
			vmask_subp[count].data[89].key = tempvmask_sp.data[89].key;
			vmask_subp[count].data[89].value = tempvmask_sp.data[89].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[89].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[89].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[89].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[89].value<<endl;
			vmask_subp[count].data[90].key = tempvmask_sp.data[90].key;
			vmask_subp[count].data[90].value = tempvmask_sp.data[90].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[90].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[90].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[90].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[90].value<<endl;
			vmask_subp[count].data[91].key = tempvmask_sp.data[91].key;
			vmask_subp[count].data[91].value = tempvmask_sp.data[91].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[91].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[91].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[91].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[91].value<<endl;
			vmask_subp[count].data[92].key = tempvmask_sp.data[92].key;
			vmask_subp[count].data[92].value = tempvmask_sp.data[92].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[92].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[92].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[92].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[92].value<<endl;
			vmask_subp[count].data[93].key = tempvmask_sp.data[93].key;
			vmask_subp[count].data[93].value = tempvmask_sp.data[93].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[93].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[93].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[93].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[93].value<<endl;
			vmask_subp[count].data[94].key = tempvmask_sp.data[94].key;
			vmask_subp[count].data[94].value = tempvmask_sp.data[94].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[94].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[94].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[94].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[94].value<<endl;
			vmask_subp[count].data[95].key = tempvmask_sp.data[95].key;
			vmask_subp[count].data[95].value = tempvmask_sp.data[95].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[95].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[95].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[95].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[95].value<<endl;
			vmask_subp[count].data[96].key = tempvmask_sp.data[96].key;
			vmask_subp[count].data[96].value = tempvmask_sp.data[96].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[96].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[96].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[96].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[96].value<<endl;
			vmask_subp[count].data[97].key = tempvmask_sp.data[97].key;
			vmask_subp[count].data[97].value = tempvmask_sp.data[97].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[97].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[97].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[97].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[97].value<<endl;
			vmask_subp[count].data[98].key = tempvmask_sp.data[98].key;
			vmask_subp[count].data[98].value = tempvmask_sp.data[98].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[98].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[98].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[98].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[98].value<<endl;
			vmask_subp[count].data[99].key = tempvmask_sp.data[99].key;
			vmask_subp[count].data[99].value = tempvmask_sp.data[99].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[99].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[99].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[99].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[99].value<<endl;
			vmask_subp[count].data[100].key = tempvmask_sp.data[100].key;
			vmask_subp[count].data[100].value = tempvmask_sp.data[100].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[100].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[100].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[100].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[100].value<<endl;
			vmask_subp[count].data[101].key = tempvmask_sp.data[101].key;
			vmask_subp[count].data[101].value = tempvmask_sp.data[101].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[101].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[101].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[101].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[101].value<<endl;
			vmask_subp[count].data[102].key = tempvmask_sp.data[102].key;
			vmask_subp[count].data[102].value = tempvmask_sp.data[102].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[102].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[102].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[102].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[102].value<<endl;
			vmask_subp[count].data[103].key = tempvmask_sp.data[103].key;
			vmask_subp[count].data[103].value = tempvmask_sp.data[103].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[103].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[103].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[103].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[103].value<<endl;
			vmask_subp[count].data[104].key = tempvmask_sp.data[104].key;
			vmask_subp[count].data[104].value = tempvmask_sp.data[104].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[104].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[104].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[104].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[104].value<<endl;
			vmask_subp[count].data[105].key = tempvmask_sp.data[105].key;
			vmask_subp[count].data[105].value = tempvmask_sp.data[105].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[105].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[105].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[105].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[105].value<<endl;
			vmask_subp[count].data[106].key = tempvmask_sp.data[106].key;
			vmask_subp[count].data[106].value = tempvmask_sp.data[106].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[106].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[106].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[106].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[106].value<<endl;
			vmask_subp[count].data[107].key = tempvmask_sp.data[107].key;
			vmask_subp[count].data[107].value = tempvmask_sp.data[107].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[107].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[107].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[107].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[107].value<<endl;
			vmask_subp[count].data[108].key = tempvmask_sp.data[108].key;
			vmask_subp[count].data[108].value = tempvmask_sp.data[108].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[108].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[108].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[108].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[108].value<<endl;
			vmask_subp[count].data[109].key = tempvmask_sp.data[109].key;
			vmask_subp[count].data[109].value = tempvmask_sp.data[109].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[109].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[109].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[109].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[109].value<<endl;
			vmask_subp[count].data[110].key = tempvmask_sp.data[110].key;
			vmask_subp[count].data[110].value = tempvmask_sp.data[110].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[110].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[110].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[110].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[110].value<<endl;
			vmask_subp[count].data[111].key = tempvmask_sp.data[111].key;
			vmask_subp[count].data[111].value = tempvmask_sp.data[111].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[111].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[111].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[111].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[111].value<<endl;
			vmask_subp[count].data[112].key = tempvmask_sp.data[112].key;
			vmask_subp[count].data[112].value = tempvmask_sp.data[112].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[112].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[112].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[112].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[112].value<<endl;
			vmask_subp[count].data[113].key = tempvmask_sp.data[113].key;
			vmask_subp[count].data[113].value = tempvmask_sp.data[113].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[113].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[113].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[113].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[113].value<<endl;
			vmask_subp[count].data[114].key = tempvmask_sp.data[114].key;
			vmask_subp[count].data[114].value = tempvmask_sp.data[114].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[114].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[114].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[114].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[114].value<<endl;
			vmask_subp[count].data[115].key = tempvmask_sp.data[115].key;
			vmask_subp[count].data[115].value = tempvmask_sp.data[115].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[115].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[115].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[115].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[115].value<<endl;
			vmask_subp[count].data[116].key = tempvmask_sp.data[116].key;
			vmask_subp[count].data[116].value = tempvmask_sp.data[116].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[116].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[116].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[116].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[116].value<<endl;
			vmask_subp[count].data[117].key = tempvmask_sp.data[117].key;
			vmask_subp[count].data[117].value = tempvmask_sp.data[117].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[117].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[117].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[117].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[117].value<<endl;
			vmask_subp[count].data[118].key = tempvmask_sp.data[118].key;
			vmask_subp[count].data[118].value = tempvmask_sp.data[118].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[118].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[118].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[118].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[118].value<<endl;
			vmask_subp[count].data[119].key = tempvmask_sp.data[119].key;
			vmask_subp[count].data[119].value = tempvmask_sp.data[119].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[119].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[119].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[119].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[119].value<<endl;
			vmask_subp[count].data[120].key = tempvmask_sp.data[120].key;
			vmask_subp[count].data[120].value = tempvmask_sp.data[120].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[120].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[120].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[120].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[120].value<<endl;
			vmask_subp[count].data[121].key = tempvmask_sp.data[121].key;
			vmask_subp[count].data[121].value = tempvmask_sp.data[121].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[121].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[121].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[121].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[121].value<<endl;
			vmask_subp[count].data[122].key = tempvmask_sp.data[122].key;
			vmask_subp[count].data[122].value = tempvmask_sp.data[122].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[122].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[122].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[122].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[122].value<<endl;
			vmask_subp[count].data[123].key = tempvmask_sp.data[123].key;
			vmask_subp[count].data[123].value = tempvmask_sp.data[123].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[123].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[123].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[123].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[123].value<<endl;
			vmask_subp[count].data[124].key = tempvmask_sp.data[124].key;
			vmask_subp[count].data[124].value = tempvmask_sp.data[124].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[124].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[124].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[124].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[124].value<<endl;
			vmask_subp[count].data[125].key = tempvmask_sp.data[125].key;
			vmask_subp[count].data[125].value = tempvmask_sp.data[125].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[125].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[125].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[125].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[125].value<<endl;
			vmask_subp[count].data[126].key = tempvmask_sp.data[126].key;
			vmask_subp[count].data[126].value = tempvmask_sp.data[126].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[126].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[126].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[126].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[126].value<<endl;
			vmask_subp[count].data[127].key = tempvmask_sp.data[127].key;
			vmask_subp[count].data[127].value = tempvmask_sp.data[127].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[127].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[127].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[127].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[127].value<<endl;
			vmask_subp[count].data[128].key = tempvmask_sp.data[128].key;
			vmask_subp[count].data[128].value = tempvmask_sp.data[128].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[128].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[128].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[128].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[128].value<<endl;
			vmask_subp[count].data[129].key = tempvmask_sp.data[129].key;
			vmask_subp[count].data[129].value = tempvmask_sp.data[129].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[129].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[129].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[129].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[129].value<<endl;
			vmask_subp[count].data[130].key = tempvmask_sp.data[130].key;
			vmask_subp[count].data[130].value = tempvmask_sp.data[130].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[130].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[130].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[130].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[130].value<<endl;
			vmask_subp[count].data[131].key = tempvmask_sp.data[131].key;
			vmask_subp[count].data[131].value = tempvmask_sp.data[131].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[131].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[131].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[131].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[131].value<<endl;
			vmask_subp[count].data[132].key = tempvmask_sp.data[132].key;
			vmask_subp[count].data[132].value = tempvmask_sp.data[132].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[132].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[132].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[132].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[132].value<<endl;
			vmask_subp[count].data[133].key = tempvmask_sp.data[133].key;
			vmask_subp[count].data[133].value = tempvmask_sp.data[133].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[133].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[133].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[133].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[133].value<<endl;
			vmask_subp[count].data[134].key = tempvmask_sp.data[134].key;
			vmask_subp[count].data[134].value = tempvmask_sp.data[134].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[134].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[134].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[134].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[134].value<<endl;
			vmask_subp[count].data[135].key = tempvmask_sp.data[135].key;
			vmask_subp[count].data[135].value = tempvmask_sp.data[135].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[135].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[135].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[135].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[135].value<<endl;
			vmask_subp[count].data[136].key = tempvmask_sp.data[136].key;
			vmask_subp[count].data[136].value = tempvmask_sp.data[136].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[136].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[136].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[136].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[136].value<<endl;
			vmask_subp[count].data[137].key = tempvmask_sp.data[137].key;
			vmask_subp[count].data[137].value = tempvmask_sp.data[137].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[137].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[137].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[137].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[137].value<<endl;
			vmask_subp[count].data[138].key = tempvmask_sp.data[138].key;
			vmask_subp[count].data[138].value = tempvmask_sp.data[138].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[138].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[138].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[138].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[138].value<<endl;
			vmask_subp[count].data[139].key = tempvmask_sp.data[139].key;
			vmask_subp[count].data[139].value = tempvmask_sp.data[139].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[139].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[139].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[139].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[139].value<<endl;
			vmask_subp[count].data[140].key = tempvmask_sp.data[140].key;
			vmask_subp[count].data[140].value = tempvmask_sp.data[140].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[140].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[140].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[140].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[140].value<<endl;
			vmask_subp[count].data[141].key = tempvmask_sp.data[141].key;
			vmask_subp[count].data[141].value = tempvmask_sp.data[141].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[141].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[141].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[141].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[141].value<<endl;
			vmask_subp[count].data[142].key = tempvmask_sp.data[142].key;
			vmask_subp[count].data[142].value = tempvmask_sp.data[142].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[142].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[142].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[142].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[142].value<<endl;
			vmask_subp[count].data[143].key = tempvmask_sp.data[143].key;
			vmask_subp[count].data[143].value = tempvmask_sp.data[143].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[143].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[143].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[143].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[143].value<<endl;
			vmask_subp[count].data[144].key = tempvmask_sp.data[144].key;
			vmask_subp[count].data[144].value = tempvmask_sp.data[144].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[144].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[144].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[144].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[144].value<<endl;
			vmask_subp[count].data[145].key = tempvmask_sp.data[145].key;
			vmask_subp[count].data[145].value = tempvmask_sp.data[145].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[145].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[145].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[145].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[145].value<<endl;
			vmask_subp[count].data[146].key = tempvmask_sp.data[146].key;
			vmask_subp[count].data[146].value = tempvmask_sp.data[146].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[146].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[146].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[146].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[146].value<<endl;
			vmask_subp[count].data[147].key = tempvmask_sp.data[147].key;
			vmask_subp[count].data[147].value = tempvmask_sp.data[147].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[147].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[147].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[147].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[147].value<<endl;
			vmask_subp[count].data[148].key = tempvmask_sp.data[148].key;
			vmask_subp[count].data[148].value = tempvmask_sp.data[148].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[148].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[148].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[148].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[148].value<<endl;
			vmask_subp[count].data[149].key = tempvmask_sp.data[149].key;
			vmask_subp[count].data[149].value = tempvmask_sp.data[149].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[149].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[149].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[149].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[149].value<<endl;
			vmask_subp[count].data[150].key = tempvmask_sp.data[150].key;
			vmask_subp[count].data[150].value = tempvmask_sp.data[150].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[150].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[150].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[150].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[150].value<<endl;
			vmask_subp[count].data[151].key = tempvmask_sp.data[151].key;
			vmask_subp[count].data[151].value = tempvmask_sp.data[151].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[151].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[151].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[151].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[151].value<<endl;
			vmask_subp[count].data[152].key = tempvmask_sp.data[152].key;
			vmask_subp[count].data[152].value = tempvmask_sp.data[152].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[152].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[152].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[152].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[152].value<<endl;
			vmask_subp[count].data[153].key = tempvmask_sp.data[153].key;
			vmask_subp[count].data[153].value = tempvmask_sp.data[153].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[153].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[153].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[153].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[153].value<<endl;
			vmask_subp[count].data[154].key = tempvmask_sp.data[154].key;
			vmask_subp[count].data[154].value = tempvmask_sp.data[154].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[154].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[154].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[154].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[154].value<<endl;
			vmask_subp[count].data[155].key = tempvmask_sp.data[155].key;
			vmask_subp[count].data[155].value = tempvmask_sp.data[155].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[155].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[155].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[155].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[155].value<<endl;
			vmask_subp[count].data[156].key = tempvmask_sp.data[156].key;
			vmask_subp[count].data[156].value = tempvmask_sp.data[156].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[156].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[156].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[156].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[156].value<<endl;
			vmask_subp[count].data[157].key = tempvmask_sp.data[157].key;
			vmask_subp[count].data[157].value = tempvmask_sp.data[157].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[157].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[157].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[157].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[157].value<<endl;
			vmask_subp[count].data[158].key = tempvmask_sp.data[158].key;
			vmask_subp[count].data[158].value = tempvmask_sp.data[158].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[158].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[158].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[158].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[158].value<<endl;
			vmask_subp[count].data[159].key = tempvmask_sp.data[159].key;
			vmask_subp[count].data[159].value = tempvmask_sp.data[159].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[159].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[159].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[159].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[159].value<<endl;
			vmask_subp[count].data[160].key = tempvmask_sp.data[160].key;
			vmask_subp[count].data[160].value = tempvmask_sp.data[160].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[160].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[160].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[160].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[160].value<<endl;
			vmask_subp[count].data[161].key = tempvmask_sp.data[161].key;
			vmask_subp[count].data[161].value = tempvmask_sp.data[161].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[161].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[161].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[161].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[161].value<<endl;
			vmask_subp[count].data[162].key = tempvmask_sp.data[162].key;
			vmask_subp[count].data[162].value = tempvmask_sp.data[162].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[162].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[162].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[162].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[162].value<<endl;
			vmask_subp[count].data[163].key = tempvmask_sp.data[163].key;
			vmask_subp[count].data[163].value = tempvmask_sp.data[163].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[163].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[163].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[163].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[163].value<<endl;
			vmask_subp[count].data[164].key = tempvmask_sp.data[164].key;
			vmask_subp[count].data[164].value = tempvmask_sp.data[164].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[164].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[164].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[164].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[164].value<<endl;
			vmask_subp[count].data[165].key = tempvmask_sp.data[165].key;
			vmask_subp[count].data[165].value = tempvmask_sp.data[165].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[165].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[165].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[165].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[165].value<<endl;
			vmask_subp[count].data[166].key = tempvmask_sp.data[166].key;
			vmask_subp[count].data[166].value = tempvmask_sp.data[166].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[166].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[166].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[166].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[166].value<<endl;
			vmask_subp[count].data[167].key = tempvmask_sp.data[167].key;
			vmask_subp[count].data[167].value = tempvmask_sp.data[167].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[167].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[167].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[167].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[167].value<<endl;
			vmask_subp[count].data[168].key = tempvmask_sp.data[168].key;
			vmask_subp[count].data[168].value = tempvmask_sp.data[168].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[168].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[168].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[168].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[168].value<<endl;
			vmask_subp[count].data[169].key = tempvmask_sp.data[169].key;
			vmask_subp[count].data[169].value = tempvmask_sp.data[169].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[169].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[169].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[169].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[169].value<<endl;
			vmask_subp[count].data[170].key = tempvmask_sp.data[170].key;
			vmask_subp[count].data[170].value = tempvmask_sp.data[170].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[170].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[170].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[170].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[170].value<<endl;
			vmask_subp[count].data[171].key = tempvmask_sp.data[171].key;
			vmask_subp[count].data[171].value = tempvmask_sp.data[171].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[171].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[171].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[171].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[171].value<<endl;
			vmask_subp[count].data[172].key = tempvmask_sp.data[172].key;
			vmask_subp[count].data[172].value = tempvmask_sp.data[172].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[172].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[172].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[172].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[172].value<<endl;
			vmask_subp[count].data[173].key = tempvmask_sp.data[173].key;
			vmask_subp[count].data[173].value = tempvmask_sp.data[173].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[173].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[173].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[173].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[173].value<<endl;
			vmask_subp[count].data[174].key = tempvmask_sp.data[174].key;
			vmask_subp[count].data[174].value = tempvmask_sp.data[174].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[174].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[174].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[174].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[174].value<<endl;
			vmask_subp[count].data[175].key = tempvmask_sp.data[175].key;
			vmask_subp[count].data[175].value = tempvmask_sp.data[175].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[175].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[175].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[175].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[175].value<<endl;
			vmask_subp[count].data[176].key = tempvmask_sp.data[176].key;
			vmask_subp[count].data[176].value = tempvmask_sp.data[176].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[176].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[176].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[176].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[176].value<<endl;
			vmask_subp[count].data[177].key = tempvmask_sp.data[177].key;
			vmask_subp[count].data[177].value = tempvmask_sp.data[177].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[177].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[177].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[177].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[177].value<<endl;
			vmask_subp[count].data[178].key = tempvmask_sp.data[178].key;
			vmask_subp[count].data[178].value = tempvmask_sp.data[178].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[178].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[178].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[178].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[178].value<<endl;
			vmask_subp[count].data[179].key = tempvmask_sp.data[179].key;
			vmask_subp[count].data[179].value = tempvmask_sp.data[179].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[179].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[179].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[179].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[179].value<<endl;
			vmask_subp[count].data[180].key = tempvmask_sp.data[180].key;
			vmask_subp[count].data[180].value = tempvmask_sp.data[180].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[180].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[180].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[180].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[180].value<<endl;
			vmask_subp[count].data[181].key = tempvmask_sp.data[181].key;
			vmask_subp[count].data[181].value = tempvmask_sp.data[181].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[181].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[181].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[181].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[181].value<<endl;
			vmask_subp[count].data[182].key = tempvmask_sp.data[182].key;
			vmask_subp[count].data[182].value = tempvmask_sp.data[182].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[182].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[182].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[182].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[182].value<<endl;
			vmask_subp[count].data[183].key = tempvmask_sp.data[183].key;
			vmask_subp[count].data[183].value = tempvmask_sp.data[183].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[183].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[183].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[183].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[183].value<<endl;
			vmask_subp[count].data[184].key = tempvmask_sp.data[184].key;
			vmask_subp[count].data[184].value = tempvmask_sp.data[184].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[184].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[184].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[184].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[184].value<<endl;
			vmask_subp[count].data[185].key = tempvmask_sp.data[185].key;
			vmask_subp[count].data[185].value = tempvmask_sp.data[185].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[185].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[185].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[185].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[185].value<<endl;
			vmask_subp[count].data[186].key = tempvmask_sp.data[186].key;
			vmask_subp[count].data[186].value = tempvmask_sp.data[186].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[186].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[186].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[186].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[186].value<<endl;
			vmask_subp[count].data[187].key = tempvmask_sp.data[187].key;
			vmask_subp[count].data[187].value = tempvmask_sp.data[187].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[187].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[187].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[187].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[187].value<<endl;
			vmask_subp[count].data[188].key = tempvmask_sp.data[188].key;
			vmask_subp[count].data[188].value = tempvmask_sp.data[188].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[188].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[188].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[188].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[188].value<<endl;
			vmask_subp[count].data[189].key = tempvmask_sp.data[189].key;
			vmask_subp[count].data[189].value = tempvmask_sp.data[189].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[189].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[189].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[189].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[189].value<<endl;
			vmask_subp[count].data[190].key = tempvmask_sp.data[190].key;
			vmask_subp[count].data[190].value = tempvmask_sp.data[190].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[190].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[190].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[190].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[190].value<<endl;
			vmask_subp[count].data[191].key = tempvmask_sp.data[191].key;
			vmask_subp[count].data[191].value = tempvmask_sp.data[191].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[191].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[191].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[191].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[191].value<<endl;
			vmask_subp[count].data[192].key = tempvmask_sp.data[192].key;
			vmask_subp[count].data[192].value = tempvmask_sp.data[192].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[192].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[192].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[192].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[192].value<<endl;
			vmask_subp[count].data[193].key = tempvmask_sp.data[193].key;
			vmask_subp[count].data[193].value = tempvmask_sp.data[193].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[193].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[193].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[193].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[193].value<<endl;
			vmask_subp[count].data[194].key = tempvmask_sp.data[194].key;
			vmask_subp[count].data[194].value = tempvmask_sp.data[194].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[194].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[194].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[194].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[194].value<<endl;
			vmask_subp[count].data[195].key = tempvmask_sp.data[195].key;
			vmask_subp[count].data[195].value = tempvmask_sp.data[195].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[195].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[195].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[195].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[195].value<<endl;
			vmask_subp[count].data[196].key = tempvmask_sp.data[196].key;
			vmask_subp[count].data[196].value = tempvmask_sp.data[196].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[196].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[196].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[196].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[196].value<<endl;
			vmask_subp[count].data[197].key = tempvmask_sp.data[197].key;
			vmask_subp[count].data[197].value = tempvmask_sp.data[197].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[197].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[197].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[197].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[197].value<<endl;
			vmask_subp[count].data[198].key = tempvmask_sp.data[198].key;
			vmask_subp[count].data[198].value = tempvmask_sp.data[198].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[198].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[198].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[198].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[198].value<<endl;
			vmask_subp[count].data[199].key = tempvmask_sp.data[199].key;
			vmask_subp[count].data[199].value = tempvmask_sp.data[199].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[199].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[199].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[199].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[199].value<<endl;
			vmask_subp[count].data[200].key = tempvmask_sp.data[200].key;
			vmask_subp[count].data[200].value = tempvmask_sp.data[200].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[200].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[200].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[200].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[200].value<<endl;
			vmask_subp[count].data[201].key = tempvmask_sp.data[201].key;
			vmask_subp[count].data[201].value = tempvmask_sp.data[201].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[201].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[201].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[201].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[201].value<<endl;
			vmask_subp[count].data[202].key = tempvmask_sp.data[202].key;
			vmask_subp[count].data[202].value = tempvmask_sp.data[202].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[202].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[202].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[202].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[202].value<<endl;
			vmask_subp[count].data[203].key = tempvmask_sp.data[203].key;
			vmask_subp[count].data[203].value = tempvmask_sp.data[203].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[203].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[203].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[203].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[203].value<<endl;
			vmask_subp[count].data[204].key = tempvmask_sp.data[204].key;
			vmask_subp[count].data[204].value = tempvmask_sp.data[204].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[204].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[204].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[204].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[204].value<<endl;
			vmask_subp[count].data[205].key = tempvmask_sp.data[205].key;
			vmask_subp[count].data[205].value = tempvmask_sp.data[205].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[205].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[205].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[205].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[205].value<<endl;
			vmask_subp[count].data[206].key = tempvmask_sp.data[206].key;
			vmask_subp[count].data[206].value = tempvmask_sp.data[206].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[206].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[206].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[206].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[206].value<<endl;
			vmask_subp[count].data[207].key = tempvmask_sp.data[207].key;
			vmask_subp[count].data[207].value = tempvmask_sp.data[207].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[207].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[207].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[207].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[207].value<<endl;
			vmask_subp[count].data[208].key = tempvmask_sp.data[208].key;
			vmask_subp[count].data[208].value = tempvmask_sp.data[208].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[208].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[208].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[208].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[208].value<<endl;
			vmask_subp[count].data[209].key = tempvmask_sp.data[209].key;
			vmask_subp[count].data[209].value = tempvmask_sp.data[209].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[209].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[209].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[209].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[209].value<<endl;
			vmask_subp[count].data[210].key = tempvmask_sp.data[210].key;
			vmask_subp[count].data[210].value = tempvmask_sp.data[210].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[210].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[210].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[210].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[210].value<<endl;
			vmask_subp[count].data[211].key = tempvmask_sp.data[211].key;
			vmask_subp[count].data[211].value = tempvmask_sp.data[211].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[211].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[211].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[211].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[211].value<<endl;
			vmask_subp[count].data[212].key = tempvmask_sp.data[212].key;
			vmask_subp[count].data[212].value = tempvmask_sp.data[212].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[212].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[212].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[212].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[212].value<<endl;
			vmask_subp[count].data[213].key = tempvmask_sp.data[213].key;
			vmask_subp[count].data[213].value = tempvmask_sp.data[213].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[213].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[213].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[213].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[213].value<<endl;
			vmask_subp[count].data[214].key = tempvmask_sp.data[214].key;
			vmask_subp[count].data[214].value = tempvmask_sp.data[214].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[214].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[214].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[214].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[214].value<<endl;
			vmask_subp[count].data[215].key = tempvmask_sp.data[215].key;
			vmask_subp[count].data[215].value = tempvmask_sp.data[215].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[215].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[215].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[215].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[215].value<<endl;
			vmask_subp[count].data[216].key = tempvmask_sp.data[216].key;
			vmask_subp[count].data[216].value = tempvmask_sp.data[216].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[216].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[216].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[216].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[216].value<<endl;
			vmask_subp[count].data[217].key = tempvmask_sp.data[217].key;
			vmask_subp[count].data[217].value = tempvmask_sp.data[217].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[217].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[217].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[217].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[217].value<<endl;
			vmask_subp[count].data[218].key = tempvmask_sp.data[218].key;
			vmask_subp[count].data[218].value = tempvmask_sp.data[218].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[218].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[218].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[218].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[218].value<<endl;
			vmask_subp[count].data[219].key = tempvmask_sp.data[219].key;
			vmask_subp[count].data[219].value = tempvmask_sp.data[219].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[219].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[219].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[219].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[219].value<<endl;
			vmask_subp[count].data[220].key = tempvmask_sp.data[220].key;
			vmask_subp[count].data[220].value = tempvmask_sp.data[220].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[220].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[220].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[220].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[220].value<<endl;
			vmask_subp[count].data[221].key = tempvmask_sp.data[221].key;
			vmask_subp[count].data[221].value = tempvmask_sp.data[221].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[221].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[221].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[221].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[221].value<<endl;
			vmask_subp[count].data[222].key = tempvmask_sp.data[222].key;
			vmask_subp[count].data[222].value = tempvmask_sp.data[222].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[222].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[222].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[222].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[222].value<<endl;
			vmask_subp[count].data[223].key = tempvmask_sp.data[223].key;
			vmask_subp[count].data[223].value = tempvmask_sp.data[223].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[223].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[223].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[223].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[223].value<<endl;
			vmask_subp[count].data[224].key = tempvmask_sp.data[224].key;
			vmask_subp[count].data[224].value = tempvmask_sp.data[224].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[224].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[224].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[224].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[224].value<<endl;
			vmask_subp[count].data[225].key = tempvmask_sp.data[225].key;
			vmask_subp[count].data[225].value = tempvmask_sp.data[225].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[225].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[225].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[225].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[225].value<<endl;
			vmask_subp[count].data[226].key = tempvmask_sp.data[226].key;
			vmask_subp[count].data[226].value = tempvmask_sp.data[226].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[226].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[226].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[226].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[226].value<<endl;
			vmask_subp[count].data[227].key = tempvmask_sp.data[227].key;
			vmask_subp[count].data[227].value = tempvmask_sp.data[227].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[227].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[227].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[227].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[227].value<<endl;
			vmask_subp[count].data[228].key = tempvmask_sp.data[228].key;
			vmask_subp[count].data[228].value = tempvmask_sp.data[228].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[228].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[228].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[228].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[228].value<<endl;
			vmask_subp[count].data[229].key = tempvmask_sp.data[229].key;
			vmask_subp[count].data[229].value = tempvmask_sp.data[229].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[229].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[229].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[229].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[229].value<<endl;
			vmask_subp[count].data[230].key = tempvmask_sp.data[230].key;
			vmask_subp[count].data[230].value = tempvmask_sp.data[230].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[230].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[230].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[230].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[230].value<<endl;
			vmask_subp[count].data[231].key = tempvmask_sp.data[231].key;
			vmask_subp[count].data[231].value = tempvmask_sp.data[231].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[231].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[231].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[231].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[231].value<<endl;
			vmask_subp[count].data[232].key = tempvmask_sp.data[232].key;
			vmask_subp[count].data[232].value = tempvmask_sp.data[232].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[232].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[232].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[232].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[232].value<<endl;
			vmask_subp[count].data[233].key = tempvmask_sp.data[233].key;
			vmask_subp[count].data[233].value = tempvmask_sp.data[233].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[233].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[233].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[233].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[233].value<<endl;
			vmask_subp[count].data[234].key = tempvmask_sp.data[234].key;
			vmask_subp[count].data[234].value = tempvmask_sp.data[234].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[234].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[234].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[234].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[234].value<<endl;
			vmask_subp[count].data[235].key = tempvmask_sp.data[235].key;
			vmask_subp[count].data[235].value = tempvmask_sp.data[235].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[235].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[235].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[235].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[235].value<<endl;
			vmask_subp[count].data[236].key = tempvmask_sp.data[236].key;
			vmask_subp[count].data[236].value = tempvmask_sp.data[236].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[236].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[236].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[236].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[236].value<<endl;
			vmask_subp[count].data[237].key = tempvmask_sp.data[237].key;
			vmask_subp[count].data[237].value = tempvmask_sp.data[237].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[237].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[237].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[237].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[237].value<<endl;
			vmask_subp[count].data[238].key = tempvmask_sp.data[238].key;
			vmask_subp[count].data[238].value = tempvmask_sp.data[238].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[238].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[238].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[238].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[238].value<<endl;
			vmask_subp[count].data[239].key = tempvmask_sp.data[239].key;
			vmask_subp[count].data[239].value = tempvmask_sp.data[239].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[239].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[239].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[239].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[239].value<<endl;
			vmask_subp[count].data[240].key = tempvmask_sp.data[240].key;
			vmask_subp[count].data[240].value = tempvmask_sp.data[240].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[240].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[240].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[240].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[240].value<<endl;
			vmask_subp[count].data[241].key = tempvmask_sp.data[241].key;
			vmask_subp[count].data[241].value = tempvmask_sp.data[241].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[241].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[241].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[241].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[241].value<<endl;
			vmask_subp[count].data[242].key = tempvmask_sp.data[242].key;
			vmask_subp[count].data[242].value = tempvmask_sp.data[242].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[242].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[242].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[242].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[242].value<<endl;
			vmask_subp[count].data[243].key = tempvmask_sp.data[243].key;
			vmask_subp[count].data[243].value = tempvmask_sp.data[243].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[243].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[243].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[243].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[243].value<<endl;
			vmask_subp[count].data[244].key = tempvmask_sp.data[244].key;
			vmask_subp[count].data[244].value = tempvmask_sp.data[244].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[244].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[244].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[244].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[244].value<<endl;
			vmask_subp[count].data[245].key = tempvmask_sp.data[245].key;
			vmask_subp[count].data[245].value = tempvmask_sp.data[245].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[245].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[245].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[245].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[245].value<<endl;
			vmask_subp[count].data[246].key = tempvmask_sp.data[246].key;
			vmask_subp[count].data[246].value = tempvmask_sp.data[246].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[246].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[246].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[246].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[246].value<<endl;
			vmask_subp[count].data[247].key = tempvmask_sp.data[247].key;
			vmask_subp[count].data[247].value = tempvmask_sp.data[247].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[247].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[247].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[247].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[247].value<<endl;
			vmask_subp[count].data[248].key = tempvmask_sp.data[248].key;
			vmask_subp[count].data[248].value = tempvmask_sp.data[248].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[248].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[248].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[248].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[248].value<<endl;
			vmask_subp[count].data[249].key = tempvmask_sp.data[249].key;
			vmask_subp[count].data[249].value = tempvmask_sp.data[249].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[249].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[249].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[249].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[249].value<<endl;
			vmask_subp[count].data[250].key = tempvmask_sp.data[250].key;
			vmask_subp[count].data[250].value = tempvmask_sp.data[250].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[250].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[250].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[250].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[250].value<<endl;
			vmask_subp[count].data[251].key = tempvmask_sp.data[251].key;
			vmask_subp[count].data[251].value = tempvmask_sp.data[251].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[251].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[251].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[251].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[251].value<<endl;
			vmask_subp[count].data[252].key = tempvmask_sp.data[252].key;
			vmask_subp[count].data[252].value = tempvmask_sp.data[252].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[252].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[252].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[252].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[252].value<<endl;
			vmask_subp[count].data[253].key = tempvmask_sp.data[253].key;
			vmask_subp[count].data[253].value = tempvmask_sp.data[253].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[253].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[253].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[253].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[253].value<<endl;
			vmask_subp[count].data[254].key = tempvmask_sp.data[254].key;
			vmask_subp[count].data[254].value = tempvmask_sp.data[254].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[254].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[254].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[254].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[254].value<<endl;
			vmask_subp[count].data[255].key = tempvmask_sp.data[255].key;
			vmask_subp[count].data[255].value = tempvmask_sp.data[255].value;
			// cout<<"vmask_subp["<<i/SUBPMASKFACTOR<<"].data[255].key: "<<vmask_subp[i/SUBPMASKFACTOR].data[255].key<<", vmask_subp["<<i/SUBPMASKFACTOR<<"].data[255].value: "<<vmask_subp[i/SUBPMASKFACTOR].data[255].value<<endl;
			
			tempvmask_sp.data[0].key = 0;
			tempvmask_sp.data[0].value = 0;
			tempvmask_sp.data[1].key = 0;
			tempvmask_sp.data[1].value = 0;
			tempvmask_sp.data[2].key = 0;
			tempvmask_sp.data[2].value = 0;
			tempvmask_sp.data[3].key = 0;
			tempvmask_sp.data[3].value = 0;
			tempvmask_sp.data[4].key = 0;
			tempvmask_sp.data[4].value = 0;
			tempvmask_sp.data[5].key = 0;
			tempvmask_sp.data[5].value = 0;
			tempvmask_sp.data[6].key = 0;
			tempvmask_sp.data[6].value = 0;
			tempvmask_sp.data[7].key = 0;
			tempvmask_sp.data[7].value = 0;
			tempvmask_sp.data[8].key = 0;
			tempvmask_sp.data[8].value = 0;
			tempvmask_sp.data[9].key = 0;
			tempvmask_sp.data[9].value = 0;
			tempvmask_sp.data[10].key = 0;
			tempvmask_sp.data[10].value = 0;
			tempvmask_sp.data[11].key = 0;
			tempvmask_sp.data[11].value = 0;
			tempvmask_sp.data[12].key = 0;
			tempvmask_sp.data[12].value = 0;
			tempvmask_sp.data[13].key = 0;
			tempvmask_sp.data[13].value = 0;
			tempvmask_sp.data[14].key = 0;
			tempvmask_sp.data[14].value = 0;
			tempvmask_sp.data[15].key = 0;
			tempvmask_sp.data[15].value = 0;
			tempvmask_sp.data[16].key = 0;
			tempvmask_sp.data[16].value = 0;
			tempvmask_sp.data[17].key = 0;
			tempvmask_sp.data[17].value = 0;
			tempvmask_sp.data[18].key = 0;
			tempvmask_sp.data[18].value = 0;
			tempvmask_sp.data[19].key = 0;
			tempvmask_sp.data[19].value = 0;
			tempvmask_sp.data[20].key = 0;
			tempvmask_sp.data[20].value = 0;
			tempvmask_sp.data[21].key = 0;
			tempvmask_sp.data[21].value = 0;
			tempvmask_sp.data[22].key = 0;
			tempvmask_sp.data[22].value = 0;
			tempvmask_sp.data[23].key = 0;
			tempvmask_sp.data[23].value = 0;
			tempvmask_sp.data[24].key = 0;
			tempvmask_sp.data[24].value = 0;
			tempvmask_sp.data[25].key = 0;
			tempvmask_sp.data[25].value = 0;
			tempvmask_sp.data[26].key = 0;
			tempvmask_sp.data[26].value = 0;
			tempvmask_sp.data[27].key = 0;
			tempvmask_sp.data[27].value = 0;
			tempvmask_sp.data[28].key = 0;
			tempvmask_sp.data[28].value = 0;
			tempvmask_sp.data[29].key = 0;
			tempvmask_sp.data[29].value = 0;
			tempvmask_sp.data[30].key = 0;
			tempvmask_sp.data[30].value = 0;
			tempvmask_sp.data[31].key = 0;
			tempvmask_sp.data[31].value = 0;
			tempvmask_sp.data[32].key = 0;
			tempvmask_sp.data[32].value = 0;
			tempvmask_sp.data[33].key = 0;
			tempvmask_sp.data[33].value = 0;
			tempvmask_sp.data[34].key = 0;
			tempvmask_sp.data[34].value = 0;
			tempvmask_sp.data[35].key = 0;
			tempvmask_sp.data[35].value = 0;
			tempvmask_sp.data[36].key = 0;
			tempvmask_sp.data[36].value = 0;
			tempvmask_sp.data[37].key = 0;
			tempvmask_sp.data[37].value = 0;
			tempvmask_sp.data[38].key = 0;
			tempvmask_sp.data[38].value = 0;
			tempvmask_sp.data[39].key = 0;
			tempvmask_sp.data[39].value = 0;
			tempvmask_sp.data[40].key = 0;
			tempvmask_sp.data[40].value = 0;
			tempvmask_sp.data[41].key = 0;
			tempvmask_sp.data[41].value = 0;
			tempvmask_sp.data[42].key = 0;
			tempvmask_sp.data[42].value = 0;
			tempvmask_sp.data[43].key = 0;
			tempvmask_sp.data[43].value = 0;
			tempvmask_sp.data[44].key = 0;
			tempvmask_sp.data[44].value = 0;
			tempvmask_sp.data[45].key = 0;
			tempvmask_sp.data[45].value = 0;
			tempvmask_sp.data[46].key = 0;
			tempvmask_sp.data[46].value = 0;
			tempvmask_sp.data[47].key = 0;
			tempvmask_sp.data[47].value = 0;
			tempvmask_sp.data[48].key = 0;
			tempvmask_sp.data[48].value = 0;
			tempvmask_sp.data[49].key = 0;
			tempvmask_sp.data[49].value = 0;
			tempvmask_sp.data[50].key = 0;
			tempvmask_sp.data[50].value = 0;
			tempvmask_sp.data[51].key = 0;
			tempvmask_sp.data[51].value = 0;
			tempvmask_sp.data[52].key = 0;
			tempvmask_sp.data[52].value = 0;
			tempvmask_sp.data[53].key = 0;
			tempvmask_sp.data[53].value = 0;
			tempvmask_sp.data[54].key = 0;
			tempvmask_sp.data[54].value = 0;
			tempvmask_sp.data[55].key = 0;
			tempvmask_sp.data[55].value = 0;
			tempvmask_sp.data[56].key = 0;
			tempvmask_sp.data[56].value = 0;
			tempvmask_sp.data[57].key = 0;
			tempvmask_sp.data[57].value = 0;
			tempvmask_sp.data[58].key = 0;
			tempvmask_sp.data[58].value = 0;
			tempvmask_sp.data[59].key = 0;
			tempvmask_sp.data[59].value = 0;
			tempvmask_sp.data[60].key = 0;
			tempvmask_sp.data[60].value = 0;
			tempvmask_sp.data[61].key = 0;
			tempvmask_sp.data[61].value = 0;
			tempvmask_sp.data[62].key = 0;
			tempvmask_sp.data[62].value = 0;
			tempvmask_sp.data[63].key = 0;
			tempvmask_sp.data[63].value = 0;
			tempvmask_sp.data[64].key = 0;
			tempvmask_sp.data[64].value = 0;
			tempvmask_sp.data[65].key = 0;
			tempvmask_sp.data[65].value = 0;
			tempvmask_sp.data[66].key = 0;
			tempvmask_sp.data[66].value = 0;
			tempvmask_sp.data[67].key = 0;
			tempvmask_sp.data[67].value = 0;
			tempvmask_sp.data[68].key = 0;
			tempvmask_sp.data[68].value = 0;
			tempvmask_sp.data[69].key = 0;
			tempvmask_sp.data[69].value = 0;
			tempvmask_sp.data[70].key = 0;
			tempvmask_sp.data[70].value = 0;
			tempvmask_sp.data[71].key = 0;
			tempvmask_sp.data[71].value = 0;
			tempvmask_sp.data[72].key = 0;
			tempvmask_sp.data[72].value = 0;
			tempvmask_sp.data[73].key = 0;
			tempvmask_sp.data[73].value = 0;
			tempvmask_sp.data[74].key = 0;
			tempvmask_sp.data[74].value = 0;
			tempvmask_sp.data[75].key = 0;
			tempvmask_sp.data[75].value = 0;
			tempvmask_sp.data[76].key = 0;
			tempvmask_sp.data[76].value = 0;
			tempvmask_sp.data[77].key = 0;
			tempvmask_sp.data[77].value = 0;
			tempvmask_sp.data[78].key = 0;
			tempvmask_sp.data[78].value = 0;
			tempvmask_sp.data[79].key = 0;
			tempvmask_sp.data[79].value = 0;
			tempvmask_sp.data[80].key = 0;
			tempvmask_sp.data[80].value = 0;
			tempvmask_sp.data[81].key = 0;
			tempvmask_sp.data[81].value = 0;
			tempvmask_sp.data[82].key = 0;
			tempvmask_sp.data[82].value = 0;
			tempvmask_sp.data[83].key = 0;
			tempvmask_sp.data[83].value = 0;
			tempvmask_sp.data[84].key = 0;
			tempvmask_sp.data[84].value = 0;
			tempvmask_sp.data[85].key = 0;
			tempvmask_sp.data[85].value = 0;
			tempvmask_sp.data[86].key = 0;
			tempvmask_sp.data[86].value = 0;
			tempvmask_sp.data[87].key = 0;
			tempvmask_sp.data[87].value = 0;
			tempvmask_sp.data[88].key = 0;
			tempvmask_sp.data[88].value = 0;
			tempvmask_sp.data[89].key = 0;
			tempvmask_sp.data[89].value = 0;
			tempvmask_sp.data[90].key = 0;
			tempvmask_sp.data[90].value = 0;
			tempvmask_sp.data[91].key = 0;
			tempvmask_sp.data[91].value = 0;
			tempvmask_sp.data[92].key = 0;
			tempvmask_sp.data[92].value = 0;
			tempvmask_sp.data[93].key = 0;
			tempvmask_sp.data[93].value = 0;
			tempvmask_sp.data[94].key = 0;
			tempvmask_sp.data[94].value = 0;
			tempvmask_sp.data[95].key = 0;
			tempvmask_sp.data[95].value = 0;
			tempvmask_sp.data[96].key = 0;
			tempvmask_sp.data[96].value = 0;
			tempvmask_sp.data[97].key = 0;
			tempvmask_sp.data[97].value = 0;
			tempvmask_sp.data[98].key = 0;
			tempvmask_sp.data[98].value = 0;
			tempvmask_sp.data[99].key = 0;
			tempvmask_sp.data[99].value = 0;
			tempvmask_sp.data[100].key = 0;
			tempvmask_sp.data[100].value = 0;
			tempvmask_sp.data[101].key = 0;
			tempvmask_sp.data[101].value = 0;
			tempvmask_sp.data[102].key = 0;
			tempvmask_sp.data[102].value = 0;
			tempvmask_sp.data[103].key = 0;
			tempvmask_sp.data[103].value = 0;
			tempvmask_sp.data[104].key = 0;
			tempvmask_sp.data[104].value = 0;
			tempvmask_sp.data[105].key = 0;
			tempvmask_sp.data[105].value = 0;
			tempvmask_sp.data[106].key = 0;
			tempvmask_sp.data[106].value = 0;
			tempvmask_sp.data[107].key = 0;
			tempvmask_sp.data[107].value = 0;
			tempvmask_sp.data[108].key = 0;
			tempvmask_sp.data[108].value = 0;
			tempvmask_sp.data[109].key = 0;
			tempvmask_sp.data[109].value = 0;
			tempvmask_sp.data[110].key = 0;
			tempvmask_sp.data[110].value = 0;
			tempvmask_sp.data[111].key = 0;
			tempvmask_sp.data[111].value = 0;
			tempvmask_sp.data[112].key = 0;
			tempvmask_sp.data[112].value = 0;
			tempvmask_sp.data[113].key = 0;
			tempvmask_sp.data[113].value = 0;
			tempvmask_sp.data[114].key = 0;
			tempvmask_sp.data[114].value = 0;
			tempvmask_sp.data[115].key = 0;
			tempvmask_sp.data[115].value = 0;
			tempvmask_sp.data[116].key = 0;
			tempvmask_sp.data[116].value = 0;
			tempvmask_sp.data[117].key = 0;
			tempvmask_sp.data[117].value = 0;
			tempvmask_sp.data[118].key = 0;
			tempvmask_sp.data[118].value = 0;
			tempvmask_sp.data[119].key = 0;
			tempvmask_sp.data[119].value = 0;
			tempvmask_sp.data[120].key = 0;
			tempvmask_sp.data[120].value = 0;
			tempvmask_sp.data[121].key = 0;
			tempvmask_sp.data[121].value = 0;
			tempvmask_sp.data[122].key = 0;
			tempvmask_sp.data[122].value = 0;
			tempvmask_sp.data[123].key = 0;
			tempvmask_sp.data[123].value = 0;
			tempvmask_sp.data[124].key = 0;
			tempvmask_sp.data[124].value = 0;
			tempvmask_sp.data[125].key = 0;
			tempvmask_sp.data[125].value = 0;
			tempvmask_sp.data[126].key = 0;
			tempvmask_sp.data[126].value = 0;
			tempvmask_sp.data[127].key = 0;
			tempvmask_sp.data[127].value = 0;
			tempvmask_sp.data[128].key = 0;
			tempvmask_sp.data[128].value = 0;
			tempvmask_sp.data[129].key = 0;
			tempvmask_sp.data[129].value = 0;
			tempvmask_sp.data[130].key = 0;
			tempvmask_sp.data[130].value = 0;
			tempvmask_sp.data[131].key = 0;
			tempvmask_sp.data[131].value = 0;
			tempvmask_sp.data[132].key = 0;
			tempvmask_sp.data[132].value = 0;
			tempvmask_sp.data[133].key = 0;
			tempvmask_sp.data[133].value = 0;
			tempvmask_sp.data[134].key = 0;
			tempvmask_sp.data[134].value = 0;
			tempvmask_sp.data[135].key = 0;
			tempvmask_sp.data[135].value = 0;
			tempvmask_sp.data[136].key = 0;
			tempvmask_sp.data[136].value = 0;
			tempvmask_sp.data[137].key = 0;
			tempvmask_sp.data[137].value = 0;
			tempvmask_sp.data[138].key = 0;
			tempvmask_sp.data[138].value = 0;
			tempvmask_sp.data[139].key = 0;
			tempvmask_sp.data[139].value = 0;
			tempvmask_sp.data[140].key = 0;
			tempvmask_sp.data[140].value = 0;
			tempvmask_sp.data[141].key = 0;
			tempvmask_sp.data[141].value = 0;
			tempvmask_sp.data[142].key = 0;
			tempvmask_sp.data[142].value = 0;
			tempvmask_sp.data[143].key = 0;
			tempvmask_sp.data[143].value = 0;
			tempvmask_sp.data[144].key = 0;
			tempvmask_sp.data[144].value = 0;
			tempvmask_sp.data[145].key = 0;
			tempvmask_sp.data[145].value = 0;
			tempvmask_sp.data[146].key = 0;
			tempvmask_sp.data[146].value = 0;
			tempvmask_sp.data[147].key = 0;
			tempvmask_sp.data[147].value = 0;
			tempvmask_sp.data[148].key = 0;
			tempvmask_sp.data[148].value = 0;
			tempvmask_sp.data[149].key = 0;
			tempvmask_sp.data[149].value = 0;
			tempvmask_sp.data[150].key = 0;
			tempvmask_sp.data[150].value = 0;
			tempvmask_sp.data[151].key = 0;
			tempvmask_sp.data[151].value = 0;
			tempvmask_sp.data[152].key = 0;
			tempvmask_sp.data[152].value = 0;
			tempvmask_sp.data[153].key = 0;
			tempvmask_sp.data[153].value = 0;
			tempvmask_sp.data[154].key = 0;
			tempvmask_sp.data[154].value = 0;
			tempvmask_sp.data[155].key = 0;
			tempvmask_sp.data[155].value = 0;
			tempvmask_sp.data[156].key = 0;
			tempvmask_sp.data[156].value = 0;
			tempvmask_sp.data[157].key = 0;
			tempvmask_sp.data[157].value = 0;
			tempvmask_sp.data[158].key = 0;
			tempvmask_sp.data[158].value = 0;
			tempvmask_sp.data[159].key = 0;
			tempvmask_sp.data[159].value = 0;
			tempvmask_sp.data[160].key = 0;
			tempvmask_sp.data[160].value = 0;
			tempvmask_sp.data[161].key = 0;
			tempvmask_sp.data[161].value = 0;
			tempvmask_sp.data[162].key = 0;
			tempvmask_sp.data[162].value = 0;
			tempvmask_sp.data[163].key = 0;
			tempvmask_sp.data[163].value = 0;
			tempvmask_sp.data[164].key = 0;
			tempvmask_sp.data[164].value = 0;
			tempvmask_sp.data[165].key = 0;
			tempvmask_sp.data[165].value = 0;
			tempvmask_sp.data[166].key = 0;
			tempvmask_sp.data[166].value = 0;
			tempvmask_sp.data[167].key = 0;
			tempvmask_sp.data[167].value = 0;
			tempvmask_sp.data[168].key = 0;
			tempvmask_sp.data[168].value = 0;
			tempvmask_sp.data[169].key = 0;
			tempvmask_sp.data[169].value = 0;
			tempvmask_sp.data[170].key = 0;
			tempvmask_sp.data[170].value = 0;
			tempvmask_sp.data[171].key = 0;
			tempvmask_sp.data[171].value = 0;
			tempvmask_sp.data[172].key = 0;
			tempvmask_sp.data[172].value = 0;
			tempvmask_sp.data[173].key = 0;
			tempvmask_sp.data[173].value = 0;
			tempvmask_sp.data[174].key = 0;
			tempvmask_sp.data[174].value = 0;
			tempvmask_sp.data[175].key = 0;
			tempvmask_sp.data[175].value = 0;
			tempvmask_sp.data[176].key = 0;
			tempvmask_sp.data[176].value = 0;
			tempvmask_sp.data[177].key = 0;
			tempvmask_sp.data[177].value = 0;
			tempvmask_sp.data[178].key = 0;
			tempvmask_sp.data[178].value = 0;
			tempvmask_sp.data[179].key = 0;
			tempvmask_sp.data[179].value = 0;
			tempvmask_sp.data[180].key = 0;
			tempvmask_sp.data[180].value = 0;
			tempvmask_sp.data[181].key = 0;
			tempvmask_sp.data[181].value = 0;
			tempvmask_sp.data[182].key = 0;
			tempvmask_sp.data[182].value = 0;
			tempvmask_sp.data[183].key = 0;
			tempvmask_sp.data[183].value = 0;
			tempvmask_sp.data[184].key = 0;
			tempvmask_sp.data[184].value = 0;
			tempvmask_sp.data[185].key = 0;
			tempvmask_sp.data[185].value = 0;
			tempvmask_sp.data[186].key = 0;
			tempvmask_sp.data[186].value = 0;
			tempvmask_sp.data[187].key = 0;
			tempvmask_sp.data[187].value = 0;
			tempvmask_sp.data[188].key = 0;
			tempvmask_sp.data[188].value = 0;
			tempvmask_sp.data[189].key = 0;
			tempvmask_sp.data[189].value = 0;
			tempvmask_sp.data[190].key = 0;
			tempvmask_sp.data[190].value = 0;
			tempvmask_sp.data[191].key = 0;
			tempvmask_sp.data[191].value = 0;
			tempvmask_sp.data[192].key = 0;
			tempvmask_sp.data[192].value = 0;
			tempvmask_sp.data[193].key = 0;
			tempvmask_sp.data[193].value = 0;
			tempvmask_sp.data[194].key = 0;
			tempvmask_sp.data[194].value = 0;
			tempvmask_sp.data[195].key = 0;
			tempvmask_sp.data[195].value = 0;
			tempvmask_sp.data[196].key = 0;
			tempvmask_sp.data[196].value = 0;
			tempvmask_sp.data[197].key = 0;
			tempvmask_sp.data[197].value = 0;
			tempvmask_sp.data[198].key = 0;
			tempvmask_sp.data[198].value = 0;
			tempvmask_sp.data[199].key = 0;
			tempvmask_sp.data[199].value = 0;
			tempvmask_sp.data[200].key = 0;
			tempvmask_sp.data[200].value = 0;
			tempvmask_sp.data[201].key = 0;
			tempvmask_sp.data[201].value = 0;
			tempvmask_sp.data[202].key = 0;
			tempvmask_sp.data[202].value = 0;
			tempvmask_sp.data[203].key = 0;
			tempvmask_sp.data[203].value = 0;
			tempvmask_sp.data[204].key = 0;
			tempvmask_sp.data[204].value = 0;
			tempvmask_sp.data[205].key = 0;
			tempvmask_sp.data[205].value = 0;
			tempvmask_sp.data[206].key = 0;
			tempvmask_sp.data[206].value = 0;
			tempvmask_sp.data[207].key = 0;
			tempvmask_sp.data[207].value = 0;
			tempvmask_sp.data[208].key = 0;
			tempvmask_sp.data[208].value = 0;
			tempvmask_sp.data[209].key = 0;
			tempvmask_sp.data[209].value = 0;
			tempvmask_sp.data[210].key = 0;
			tempvmask_sp.data[210].value = 0;
			tempvmask_sp.data[211].key = 0;
			tempvmask_sp.data[211].value = 0;
			tempvmask_sp.data[212].key = 0;
			tempvmask_sp.data[212].value = 0;
			tempvmask_sp.data[213].key = 0;
			tempvmask_sp.data[213].value = 0;
			tempvmask_sp.data[214].key = 0;
			tempvmask_sp.data[214].value = 0;
			tempvmask_sp.data[215].key = 0;
			tempvmask_sp.data[215].value = 0;
			tempvmask_sp.data[216].key = 0;
			tempvmask_sp.data[216].value = 0;
			tempvmask_sp.data[217].key = 0;
			tempvmask_sp.data[217].value = 0;
			tempvmask_sp.data[218].key = 0;
			tempvmask_sp.data[218].value = 0;
			tempvmask_sp.data[219].key = 0;
			tempvmask_sp.data[219].value = 0;
			tempvmask_sp.data[220].key = 0;
			tempvmask_sp.data[220].value = 0;
			tempvmask_sp.data[221].key = 0;
			tempvmask_sp.data[221].value = 0;
			tempvmask_sp.data[222].key = 0;
			tempvmask_sp.data[222].value = 0;
			tempvmask_sp.data[223].key = 0;
			tempvmask_sp.data[223].value = 0;
			tempvmask_sp.data[224].key = 0;
			tempvmask_sp.data[224].value = 0;
			tempvmask_sp.data[225].key = 0;
			tempvmask_sp.data[225].value = 0;
			tempvmask_sp.data[226].key = 0;
			tempvmask_sp.data[226].value = 0;
			tempvmask_sp.data[227].key = 0;
			tempvmask_sp.data[227].value = 0;
			tempvmask_sp.data[228].key = 0;
			tempvmask_sp.data[228].value = 0;
			tempvmask_sp.data[229].key = 0;
			tempvmask_sp.data[229].value = 0;
			tempvmask_sp.data[230].key = 0;
			tempvmask_sp.data[230].value = 0;
			tempvmask_sp.data[231].key = 0;
			tempvmask_sp.data[231].value = 0;
			tempvmask_sp.data[232].key = 0;
			tempvmask_sp.data[232].value = 0;
			tempvmask_sp.data[233].key = 0;
			tempvmask_sp.data[233].value = 0;
			tempvmask_sp.data[234].key = 0;
			tempvmask_sp.data[234].value = 0;
			tempvmask_sp.data[235].key = 0;
			tempvmask_sp.data[235].value = 0;
			tempvmask_sp.data[236].key = 0;
			tempvmask_sp.data[236].value = 0;
			tempvmask_sp.data[237].key = 0;
			tempvmask_sp.data[237].value = 0;
			tempvmask_sp.data[238].key = 0;
			tempvmask_sp.data[238].value = 0;
			tempvmask_sp.data[239].key = 0;
			tempvmask_sp.data[239].value = 0;
			tempvmask_sp.data[240].key = 0;
			tempvmask_sp.data[240].value = 0;
			tempvmask_sp.data[241].key = 0;
			tempvmask_sp.data[241].value = 0;
			tempvmask_sp.data[242].key = 0;
			tempvmask_sp.data[242].value = 0;
			tempvmask_sp.data[243].key = 0;
			tempvmask_sp.data[243].value = 0;
			tempvmask_sp.data[244].key = 0;
			tempvmask_sp.data[244].value = 0;
			tempvmask_sp.data[245].key = 0;
			tempvmask_sp.data[245].value = 0;
			tempvmask_sp.data[246].key = 0;
			tempvmask_sp.data[246].value = 0;
			tempvmask_sp.data[247].key = 0;
			tempvmask_sp.data[247].value = 0;
			tempvmask_sp.data[248].key = 0;
			tempvmask_sp.data[248].value = 0;
			tempvmask_sp.data[249].key = 0;
			tempvmask_sp.data[249].value = 0;
			tempvmask_sp.data[250].key = 0;
			tempvmask_sp.data[250].value = 0;
			tempvmask_sp.data[251].key = 0;
			tempvmask_sp.data[251].value = 0;
			tempvmask_sp.data[252].key = 0;
			tempvmask_sp.data[252].value = 0;
			tempvmask_sp.data[253].key = 0;
			tempvmask_sp.data[253].value = 0;
			tempvmask_sp.data[254].key = 0;
			tempvmask_sp.data[254].value = 0;
			tempvmask_sp.data[255].key = 0;
			tempvmask_sp.data[255].value = 0;
			
			count += 1;
		}
		#endif
	}
	return;
}

void //
	#ifdef SW 
	actsproc::
	#endif 
loadvmask_p(uint512_dt * kvdram, uint32_type vmask_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs){
	LOADACTIVEPARTITIONS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
		#ifdef _WIDEWORD
		vmask_p[i] = kvdram[offset_kvs + i].range(31, 0);
		#else
		vmask_p[i] = kvdram[offset_kvs + i].data[0].key;
		#endif 
	}
	return;
}

void //
	#ifdef SW 
	actsproc::
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
		globalstatsbuffer[i].key = kvdram[offset_kvs + i].data[0].key;
		globalstatsbuffer[i].value = kvdram[offset_kvs + i].data[0].value;
		#endif 
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printkeyvalues("readglobalstats.globalstatsbuffer", globalstatsbuffer, NUM_PARTITIONS); 
	#endif
	return;
}

void 
	#ifdef SW 
	actsproc::
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
		kvdram[offset_kvs + i].data[0].key = globalstatsbuffer[i].key;
		kvdram[offset_kvs + i].data[0].value = globalstatsbuffer[i].value;
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

// functions (process)
value_t 
	#ifdef SW 
	actsproc::
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

fetchmessage_t 
	#ifdef SW 
	actsproc::
	#endif 
readandprocess(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask[BLOCKRAM_SIZE], uintNUMPby2_type vmask_subp[BLOCKRAM_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], 
		batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, sweepparams_t sweepparams, globalparams_t globalparams){
	fetchmessage_t fetchmessage;
	fetchmessage.chunksize_kvs = -1;
	fetchmessage.nextoffset_kvs = -1;
	if(enable == OFF){ return fetchmessage; }
	
	analysis_type analysis_loop = BLOCKRAM_SIZE / 2;
	analysis_type analysis_loop1 = SUBPMASKFACTOR;
	analysis_type analysis_loop2 = 16384 / SUBPMASKFACTOR;
	
	value_t E[2][VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=E complete
	bool_type en = ON;
	
	buffer_type reducebuffersz = globalparams.SIZE_REDUCE / 2;
	unsigned int validbound = reducebuffersz * FETFACTOR * VECTOR2_SIZE;
	
	travstate.i_kvs = travstate.i_kvs / 2;
	travstate.end_kvs = travstate.end_kvs / 2;
	loffset_kvs = loffset_kvs / 2;
	buffer_type edgessize_kvs = size_kvs / 2;
	
	batch_type offset_kvs = goffset_kvs + loffset_kvs;
	
	vertex_t srcvid;
	vertex_t lvid;
	vertex_t lvid_sob = INVALIDDATA;
	vertex_t lvid_eob = INVALIDDATA; 
	vertex_t firstvalidlvid = INVALIDDATA;
	vertex_t lastvalidlvid = INVALIDDATA; 
	
	batch_type vptrbaseoffset_kvs = globalparams.BASEOFFSETKVS_VERTEXPTR + (globalparams.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE);
	batch_type voffset_kvs = sweepparams.source_partition * reducebuffersz * FETFACTOR;
	keyy_t nextbeginvptr;
	keyy_t nextbeginvptr_kvs;
	
	fetchmessage.chunksize_kvs = edgessize_kvs;
	fetchmessage.nextoffset_kvs = -1;
	buffer_type loadcount = 0;
	
	unsigned int found = 0;
	unsigned int found1 = 0;
	int nextactivei = -1;
	int subpidx = -1;
	unsigned int numVsperpartition_POW = globalparams.POW_BATCHRANGE - (NUM_PARTITIONS_POW * (globalparams.ACTSPARAMS_TREEDEPTH-1));
	unsigned int numVsperpartition = (1 << numVsperpartition_POW);
	
	buffer_type chunk_size = getchunksize_kvs(edgessize_kvs, travstate, 0);
	READANDPROCESS_LOOP1: for (buffer_type i=0; i<chunk_size; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loop avg=analysis_loop	
	#pragma HLS PIPELINE II=1

		#ifdef _WIDEWORD
		E[0][0] = kvdram[offset_kvs + i].range(31, 0); 
		E[0][1] = kvdram[offset_kvs + i].range(63, 32); 
		E[0][2] = kvdram[offset_kvs + i].range(95, 64); 
		E[0][3] = kvdram[offset_kvs + i].range(127, 96); 
		E[0][4] = kvdram[offset_kvs + i].range(159, 128); 
		E[0][5] = kvdram[offset_kvs + i].range(191, 160); 
		E[0][6] = kvdram[offset_kvs + i].range(223, 192); 
		E[0][7] = kvdram[offset_kvs + i].range(255, 224); 
		E[1][0] = kvdram[offset_kvs + i].range(287, 256); 
		E[1][1] = kvdram[offset_kvs + i].range(319, 288); 
		E[1][2] = kvdram[offset_kvs + i].range(351, 320); 
		E[1][3] = kvdram[offset_kvs + i].range(383, 352); 
		E[1][4] = kvdram[offset_kvs + i].range(415, 384); 
		E[1][5] = kvdram[offset_kvs + i].range(447, 416); 
		E[1][6] = kvdram[offset_kvs + i].range(479, 448); 
		E[1][7] = kvdram[offset_kvs + i].range(511, 480); 
		#else 
		E[0][0] = kvdram[offset_kvs + i].data[0].key; 
		E[0][1] = kvdram[offset_kvs + i].data[0].value; 
		E[0][2] = kvdram[offset_kvs + i].data[1].key; 
		E[0][3] = kvdram[offset_kvs + i].data[1].value; 
		E[0][4] = kvdram[offset_kvs + i].data[2].key; 
		E[0][5] = kvdram[offset_kvs + i].data[2].value; 
		E[0][6] = kvdram[offset_kvs + i].data[3].key; 
		E[0][7] = kvdram[offset_kvs + i].data[3].value; 
		E[1][0] = kvdram[offset_kvs + i].data[4+0].key; 
		E[1][1] = kvdram[offset_kvs + i].data[4+0].value; 
		E[1][2] = kvdram[offset_kvs + i].data[4+1].key; 
		E[1][3] = kvdram[offset_kvs + i].data[4+1].value; 
		E[1][4] = kvdram[offset_kvs + i].data[4+2].key; 
		E[1][5] = kvdram[offset_kvs + i].data[4+2].value; 
		E[1][6] = kvdram[offset_kvs + i].data[4+3].key; 
		E[1][7] = kvdram[offset_kvs + i].data[4+3].value; 
		#endif 
		
		en = ON;
		
		srcvid = E[0][0];
		lvid = srcvid - travstate.i2;
		
		if(i == 0){ lvid_sob = lvid; } 
		if(i == chunk_size-1){ lvid_eob = lvid; } 
		
		if(lvid >= validbound || srcvid == UNUSEDDATA){ en = OFF; lvid = 0; }
		#ifdef _DEBUGMODE_CHECKS2
		if(srcvid < travstate.i2){ cout<<"readandprocess: INVALID srcvid. this is an error. i: "<<i<<", srcvid: "<<srcvid<<", travstate.i2: "<<travstate.i2<<". exiting..."<<endl; 
			for(unsigned int v=0; v<VECTOR_SIZE; v++){ cout<<"readandprocess: E[0]["<<v<<"]: "<<E[0][v]<<", E[1]["<<v<<"]: "<<E[1][v]<<endl; }
			exit(EXIT_FAILURE); }
		actsutilityobj->checkoutofbounds("readandprocess.1", lvid, reducebuffersz * FETFACTOR * VECTOR2_SIZE, srcvid, travstate.i2, NAp);
		#endif
		
		value_t udata = GETVTXDATA(vbuffer, lvid, globalparams);
		#ifdef ALLVERTEXISACTIVE_ALGORITHM
		unsigned int mask = 1;
		#else
		unsigned int mask = GETVTXMASK(vmask, lvid, globalparams);
		#endif
		value_t res  = processfunc(udata, 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID); 
		
		if(en == ON && mask == 1 && firstvalidlvid == INVALIDDATA){ firstvalidlvid = lvid; } // NEWCHANGE.
		if(en == ON && mask == 1){ lastvalidlvid = lvid; }
		
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("readandprocess.1", mask, 2, NAp, NAp, NAp);
		#endif
		
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
		if(en == ON && mask == 1){
			if(true){ cout<<"readandprocess: i: "<<i<<", mask: "<<mask<<", srcvid: "<<srcvid<<", travstate.i2: "<<travstate.i2<<", lvid: "<<lvid<<", udata: "<<udata<<endl; }
			for(unsigned int v=0; v<VECTOR_SIZE; v++){ cout<<"readandprocess: udata: "<<udata<<", E[0]["<<v<<"]: "<<E[0][v]<<", sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }
			for(unsigned int v=0; v<VECTOR_SIZE; v++){ cout<<"readandprocess: udata: "<<udata<<", E[1]["<<v<<"]: "<<E[1][v]<<", sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }
		}
		#endif
		
		keyvalue_t mykeyvalue00;
		keyvalue_t mykeyvalue01;
		keyvalue_t mykeyvalue02;
		keyvalue_t mykeyvalue03;
		keyvalue_t mykeyvalue04;
		keyvalue_t mykeyvalue05;
		keyvalue_t mykeyvalue06;
		keyvalue_t mykeyvalue07;
		keyvalue_t mykeyvalue10;
		keyvalue_t mykeyvalue11;
		keyvalue_t mykeyvalue12;
		keyvalue_t mykeyvalue13;
		keyvalue_t mykeyvalue14;
		keyvalue_t mykeyvalue15;
		keyvalue_t mykeyvalue16;
		keyvalue_t mykeyvalue17;
		
		if(en == ON && mask == 1){
			mykeyvalue00.key = E[0][0]; 
			mykeyvalue00.value = res; 
			mykeyvalue01.key = E[0][1]; 
			mykeyvalue01.value = res; 
			mykeyvalue02.key = E[0][2]; 
			mykeyvalue02.value = res; 
			mykeyvalue03.key = E[0][3]; 
			mykeyvalue03.value = res; 
			mykeyvalue04.key = E[0][4]; 
			mykeyvalue04.value = res; 
			mykeyvalue05.key = E[0][5]; 
			mykeyvalue05.value = res; 
			mykeyvalue06.key = E[0][6]; 
			mykeyvalue06.value = res; 
			mykeyvalue07.key = E[0][7]; 
			mykeyvalue07.value = res; 
			mykeyvalue10.key = E[1][0]; 
			mykeyvalue10.value = res; 
			mykeyvalue11.key = E[1][1]; 
			mykeyvalue11.value = res; 
			mykeyvalue12.key = E[1][2]; 
			mykeyvalue12.value = res; 
			mykeyvalue13.key = E[1][3]; 
			mykeyvalue13.value = res; 
			mykeyvalue14.key = E[1][4]; 
			mykeyvalue14.value = res; 
			mykeyvalue15.key = E[1][5]; 
			mykeyvalue15.value = res; 
			mykeyvalue16.key = E[1][6]; 
			mykeyvalue16.value = res; 
			mykeyvalue17.key = E[1][7]; 
			mykeyvalue17.value = res; 
		} else {
			mykeyvalue00.key = INVALIDDATA; 
			mykeyvalue00.value = INVALIDDATA; 
			mykeyvalue01.key = INVALIDDATA; 
			mykeyvalue01.value = INVALIDDATA; 
			mykeyvalue02.key = INVALIDDATA; 
			mykeyvalue02.value = INVALIDDATA; 
			mykeyvalue03.key = INVALIDDATA; 
			mykeyvalue03.value = INVALIDDATA; 
			mykeyvalue04.key = INVALIDDATA; 
			mykeyvalue04.value = INVALIDDATA; 
			mykeyvalue05.key = INVALIDDATA; 
			mykeyvalue05.value = INVALIDDATA; 
			mykeyvalue06.key = INVALIDDATA; 
			mykeyvalue06.value = INVALIDDATA; 
			mykeyvalue07.key = INVALIDDATA; 
			mykeyvalue07.value = INVALIDDATA; 
			mykeyvalue10.key = INVALIDDATA; 
			mykeyvalue10.value = INVALIDDATA; 
			mykeyvalue11.key = INVALIDDATA; 
			mykeyvalue11.value = INVALIDDATA; 
			mykeyvalue12.key = INVALIDDATA; 
			mykeyvalue12.value = INVALIDDATA; 
			mykeyvalue13.key = INVALIDDATA; 
			mykeyvalue13.value = INVALIDDATA; 
			mykeyvalue14.key = INVALIDDATA; 
			mykeyvalue14.value = INVALIDDATA; 
			mykeyvalue15.key = INVALIDDATA; 
			mykeyvalue15.value = INVALIDDATA; 
			mykeyvalue16.key = INVALIDDATA; 
			mykeyvalue16.value = INVALIDDATA; 
			mykeyvalue17.key = INVALIDDATA; 
			mykeyvalue17.value = INVALIDDATA; 
		}
		
		mykeyvalue00.key = INVALIDDATA;
		mykeyvalue00.value = INVALIDDATA;
		
		buffer[0][loadcount] = GETKV(mykeyvalue00);
		buffer[0][loadcount + 1] = GETKV(mykeyvalue10);
		buffer[1][loadcount] = GETKV(mykeyvalue01);
		buffer[1][loadcount + 1] = GETKV(mykeyvalue11);
		buffer[2][loadcount] = GETKV(mykeyvalue02);
		buffer[2][loadcount + 1] = GETKV(mykeyvalue12);
		buffer[3][loadcount] = GETKV(mykeyvalue03);
		buffer[3][loadcount + 1] = GETKV(mykeyvalue13);
		buffer[4][loadcount] = GETKV(mykeyvalue04);
		buffer[4][loadcount + 1] = GETKV(mykeyvalue14);
		buffer[5][loadcount] = GETKV(mykeyvalue05);
		buffer[5][loadcount + 1] = GETKV(mykeyvalue15);
		buffer[6][loadcount] = GETKV(mykeyvalue06);
		buffer[6][loadcount + 1] = GETKV(mykeyvalue16);
		buffer[7][loadcount] = GETKV(mykeyvalue07);
		buffer[7][loadcount + 1] = GETKV(mykeyvalue17);
		if(en == ON && mask == 1){ loadcount += 2; }
		
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsprocessed(VECTOR_SIZE*2);
		if(en == ON && mask == 1){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(VECTOR_SIZE*2); }
		#endif 
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"readandprocess:: lvid_sob: "<<lvid_sob<<", lvid_eob: "<<lvid_eob<<""<<endl;
	#endif
	#ifndef ALLVERTEXISACTIVE_ALGORITHM
	#ifdef SUBPMASK_XXX
	// if(sweepparams.source_partition == 0){
	for(nextactivei=lvid_eob; nextactivei<numVsperpartition; nextactivei++){
		unsigned int mask = GETVTXMASK(vmask, nextactivei, globalparams);
		if(mask == 1){ 
		// cout<<"next active: "<<nextactivei<<" (lvid_sob: "<<lvid_sob<<", lvid_eob: "<<lvid_eob<<")"<<endl; 
		found = 1; break; } // break; }
		// if(mask == 1){ cout<<"next active: "<<nextactivei<<" (lvid_sob: "<<lvid_sob<<", lvid_eob: "<<lvid_eob<<")"<<endl; found = 1; exit(EXIT_SUCCESS);; }
		// else { cout<<"inactive: "<<nextactivei<<endl; }
	}
	// if(found == 0){ cout<<"none found"<<endl; }
	
	if(nextactivei - lvid_eob >= 1024){
		#ifdef _WIDEWORD
		nextbeginvptr = kvdram[vptrbaseoffset_kvs + voffset_kvs + (nextactivei/VECTOR2_SIZE)].range(31, 0);
		#else 
		nextbeginvptr = kvdram[vptrbaseoffset_kvs + voffset_kvs + (nextactivei/VECTOR2_SIZE)].data[0].key;
		#endif 
		nextbeginvptr_kvs = nextbeginvptr / VECTOR_SIZE;
		// returnmessage = nextbeginvptr_kvs;
		fetchmessage.nextoffset_kvs = nextbeginvptr_kvs;
		// cout<<"readandprocess: vptrbaseoffset_kvs="<<vptrbaseoffset_kvs<<", voffset_kvs="<<voffset_kvs<<", nextactivei="<<nextactivei<<endl;
		// cout<<"readandprocess: (nextactivei="<<nextactivei<<", lvid_eob="<<lvid_eob<<", nextbeginvptr="<<nextbeginvptr<<", nextbeginvptr_kvs="<<nextbeginvptr_kvs<<", returnmessage="<<returnmessage<<") gap more than 1024 found. skipping some..."<<endl;
		// exit(EXIT_SUCCESS);
	} else {
		// returnmessage = -1;
		fetchmessage.nextoffset_kvs = -1;
	}
	// }
	#endif
	#ifdef SUBPMASK
	// fine grained search
	unsigned int fac = (lvid_eob + (SUBPMASKFACTOR - 1)) / SUBPMASKFACTOR;
	unsigned int last = fac * SUBPMASKFACTOR;
	if(last > numVsperpartition){ last = numVsperpartition; }
	#ifdef _DEBUGMODE_KERNELPRINTS2
	cout<<"readandprocess: fine grained scan: scanning vmask from srcvid:"<<lvid_eob<<" to srcvid:"<<last<<endl;
	#endif 
	READANDPROCESS_SCANFORNEXT_LOOP1: for(nextactivei=lvid_eob; nextactivei<last; nextactivei++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loop1 avg=analysis_loop1	
	#pragma HLS PIPELINE II=2
		unsigned int mask = GETVTXMASK(vmask, nextactivei, globalparams);
		if(mask == 1){ 
			#ifdef _DEBUGMODE_KERNELPRINTS2
			cout<<"[fine grained scan]:next active: "<<nextactivei<<" (lvid_sob: "<<lvid_sob<<", lvid_eob: "<<lvid_eob<<")"<<endl; 
			#endif 
			found1 = 1; 
			break; 
		}
	}
	#ifdef _DEBUGMODE_KERNELPRINTS2
	if(found1 == 0){ cout<<"[fine grained scan]:none found"<<endl; }
	#endif 
	
	// coarsed grained search
	if(found1 == 0 && nextactivei+SUBPMASKFACTOR < numVsperpartition){
		#ifdef _DEBUGMODE_KERNELPRINTS2
		cout<<"readandprocess: coarse grained scan: scanning vmask_subp from "<<last / SUBPMASKFACTOR<<" to "<<numVsperpartition/SUBPMASKFACTOR<<endl;
		#endif 
		
		unsigned int found2 = 0;
		READANDPROCESS_SCANFORNEXT_LOOP2: for(subpidx=(last >> SUBPMASKFACTOR_POW); subpidx<(numVsperpartition >> SUBPMASKFACTOR_POW); subpidx++){ 
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loop2 avg=analysis_loop2	
		#pragma HLS PIPELINE II=2
			unsigned int mask_subp = GETVTXMASK_SUBP(vmask_subp, subpidx, globalparams);
			if(mask_subp == 1){
				#ifdef _DEBUGMODE_KERNELPRINTS2
				cout<<"[coarse grained scan]next active: "<<subpidx<<" (lvid_sob: "<<lvid_sob<<", lvid_eob: "<<lvid_eob<<", currentsubpidx: "<<last / SUBPMASKFACTOR<<")"<<endl; 
				#endif 
				found2 = 1;
				break; 
			}
		}
		#ifdef _DEBUGMODE_KERNELPRINTS2
		if(found2 == 0){ cout<<"[coarse grained scan]:none found"<<endl; }
		#endif 

		#ifdef _WIDEWORD
		nextbeginvptr = kvdram[vptrbaseoffset_kvs + voffset_kvs + ((subpidx*SUBPMASKFACTOR)/VECTOR2_SIZE)].range(31, 0);
		#else 
		nextbeginvptr = kvdram[vptrbaseoffset_kvs + voffset_kvs + ((subpidx*SUBPMASKFACTOR)/VECTOR2_SIZE)].data[0].key;
		#endif 
		nextbeginvptr_kvs = nextbeginvptr / VECTOR_SIZE;
		fetchmessage.nextoffset_kvs = nextbeginvptr_kvs;
		
		#ifdef _DEBUGMODE_KERNELPRINTS2
		cout<<"readandprocess:after coarse grained scan: (subpidx:"<<subpidx<<", nextbeginvptr_kvs:"<<nextbeginvptr_kvs<<endl;
		#endif
	}
	#endif
	#endif 
	
	fetchmessage.chunksize_kvs = loadcount;
	return fetchmessage;
}

// functions (partition)
void 
	#ifdef SW 
	actsproc::
	#endif
preparekeyvalues(bool_type enable1, bool_type enable2, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_buffer_t destbuffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[VECTOR_SIZE][NUM_PARTITIONS], step_type currentLOP, sweepparams_t sweepparams, buffer_type size_kvs, buffer_type cutoffs[VECTOR_SIZE], globalparams_t globalparams){				
	if(enable1 == OFF && enable2 == OFF){ return; }
	analysis_type analysis_loop1 = WORKBUFFER_SIZE;
	analysis_type analysis_dummyfiller = SRCBUFFER_SIZE - WORKBUFFER_SIZE;
	
	value_t emptyslot[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=emptyslot complete
	resetvalues(emptyslot, VECTOR_SIZE, 0);
	
	buffer_type chunk_size = size_kvs;
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
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loop1 avg=analysis_loop1	
	#pragma HLS PIPELINE II=2
		keyvalue_buffer_t keyvalue0 = sourcebuffer[0][i];
		keyvalue_buffer_t keyvalue1 = sourcebuffer[1][i];
		keyvalue_buffer_t keyvalue2 = sourcebuffer[2][i];
		keyvalue_buffer_t keyvalue3 = sourcebuffer[3][i];
		keyvalue_buffer_t keyvalue4 = sourcebuffer[4][i];
		keyvalue_buffer_t keyvalue5 = sourcebuffer[5][i];
		keyvalue_buffer_t keyvalue6 = sourcebuffer[6][i];
		keyvalue_buffer_t keyvalue7 = sourcebuffer[7][i];
		
		keyvalue_t mykeyvalue0 = GETKV(keyvalue0);
		keyvalue_t mykeyvalue1 = GETKV(keyvalue1);
		keyvalue_t mykeyvalue2 = GETKV(keyvalue2);
		keyvalue_t mykeyvalue3 = GETKV(keyvalue3);
		keyvalue_t mykeyvalue4 = GETKV(keyvalue4);
		keyvalue_t mykeyvalue5 = GETKV(keyvalue5);
		keyvalue_t mykeyvalue6 = GETKV(keyvalue6);
		keyvalue_t mykeyvalue7 = GETKV(keyvalue7);
		
		bool_type valid0 = ON;
		if(mykeyvalue0.key != GETK(INVALIDDATA) && mykeyvalue0.value != GETV(INVALIDDATA)){ valid0 = ON; } else { valid0 = OFF; }
		bool_type valid1 = ON;
		if(mykeyvalue1.key != GETK(INVALIDDATA) && mykeyvalue1.value != GETV(INVALIDDATA)){ valid1 = ON; } else { valid1 = OFF; }
		bool_type valid2 = ON;
		if(mykeyvalue2.key != GETK(INVALIDDATA) && mykeyvalue2.value != GETV(INVALIDDATA)){ valid2 = ON; } else { valid2 = OFF; }
		bool_type valid3 = ON;
		if(mykeyvalue3.key != GETK(INVALIDDATA) && mykeyvalue3.value != GETV(INVALIDDATA)){ valid3 = ON; } else { valid3 = OFF; }
		bool_type valid4 = ON;
		if(mykeyvalue4.key != GETK(INVALIDDATA) && mykeyvalue4.value != GETV(INVALIDDATA)){ valid4 = ON; } else { valid4 = OFF; }
		bool_type valid5 = ON;
		if(mykeyvalue5.key != GETK(INVALIDDATA) && mykeyvalue5.value != GETV(INVALIDDATA)){ valid5 = ON; } else { valid5 = OFF; }
		bool_type valid6 = ON;
		if(mykeyvalue6.key != GETK(INVALIDDATA) && mykeyvalue6.value != GETV(INVALIDDATA)){ valid6 = ON; } else { valid6 = OFF; }
		bool_type valid7 = ON;
		if(mykeyvalue7.key != GETK(INVALIDDATA) && mykeyvalue7.value != GETV(INVALIDDATA)){ valid7 = ON; } else { valid7 = OFF; }
		
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
		keyvalue_t mydummykv;
		mydummykv.key = p;
		mydummykv.value = INVALIDDATA;
		keyvalue_buffer_t dummykv = GETKV(mydummykv);
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
	
	localcapsule[0][0].value += (SRCBUFFER_SIZE - emptyslot[0]); //?????????????????????? FIXME. MAKE OBSOLETE?
	localcapsule[1][0].value += (SRCBUFFER_SIZE - emptyslot[1]); //?????????????????????? FIXME. MAKE OBSOLETE?
	localcapsule[2][0].value += (SRCBUFFER_SIZE - emptyslot[2]); //?????????????????????? FIXME. MAKE OBSOLETE?
	localcapsule[3][0].value += (SRCBUFFER_SIZE - emptyslot[3]); //?????????????????????? FIXME. MAKE OBSOLETE?
	localcapsule[4][0].value += (SRCBUFFER_SIZE - emptyslot[4]); //?????????????????????? FIXME. MAKE OBSOLETE?
	localcapsule[5][0].value += (SRCBUFFER_SIZE - emptyslot[5]); //?????????????????????? FIXME. MAKE OBSOLETE?
	localcapsule[6][0].value += (SRCBUFFER_SIZE - emptyslot[6]); //?????????????????????? FIXME. MAKE OBSOLETE?
	localcapsule[7][0].value += (SRCBUFFER_SIZE - emptyslot[7]); //?????????????????????? FIXME. MAKE OBSOLETE?
	
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
	actsutilityobj->printkeyvalues("[after]preparekeyvalues.localcapsule[0].2", (keyvalue_t *)localcapsule[0], NUM_PARTITIONS);
	actsutilityobj->printvalues("preparekeyvalues:cutoffs", cutoffs, VECTOR_SIZE);
	#endif
	return;
}

// functions (reduce)
value_t 
	#ifdef SW 
	actsproc::
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
	actsproc::
	#endif
reducevector(keyvalue_buffer_t kvdata, keyvalue_vbuffer_t destbuffer[BLOCKRAM_SIZE], buffer_type destoffset, unsigned int upperlimit, sweepparams_t sweepparams, globalparams_t globalparams){
	#pragma HLS PIPELINE II=3 // CRITICAL NEWCHANGE.	
	analysis_type analysis_loop1 = VECTOR_SIZE;
	
	keyvalue_t mykeyvalue = GETKV(kvdata);
	
	vertex_t loc = mykeyvalue.key - upperlimit;
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	if(mykeyvalue.key != GETK(INVALIDDATA) && mykeyvalue.value != GETV(INVALIDDATA)){ cout<<"REDUCE SEEN @ reducevector:: vid: "<<upperlimit + loc<<", loc: "<<loc<<", mykeyvalue.key: "<<mykeyvalue.key<<", mykeyvalue.value: "<<mykeyvalue.value<<", upperlimit: "<<upperlimit<<", reduce size: "<<globalparams.SIZE_REDUCE<<endl; }
	#endif 
	
	if(loc >= globalparams.SIZE_REDUCE && mykeyvalue.key != GETK(INVALIDDATA) && mykeyvalue.value != GETV(INVALIDDATA)){ 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->reducehelper_checkreduceloc(0, loc, mykeyvalue, sweepparams, globalparams); 
		#endif 
		loc = 0; }

	vertex_t rowindex = loc / 2;
	vertex_t colindex = loc % 2;
	
	keyvalue_t myvprop;
	if(mykeyvalue.key != GETV(INVALIDDATA) && mykeyvalue.value != GETV(INVALIDDATA)){ myvprop = GETKV2(destbuffer[destoffset + rowindex]); }
	
	value_t temp;
	if(colindex == 0){ temp = myvprop.key; }
	else { temp = myvprop.value; }
	
	value_t rettemp = reducefunc(temp, mykeyvalue.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(mykeyvalue.key != GETV(INVALIDDATA) && mykeyvalue.value != GETV(INVALIDDATA)){ cout<<"REDUCEFUNC RESULT @ reducevector:: rettemp: "<<rettemp<<", temp: "<<temp<<", mykeyvalue.value: "<<mykeyvalue.value<<", NAp: "<<NAp<<endl; }
	#endif 
	
	if(colindex == 0){ myvprop.key = rettemp; }
	else { myvprop.value = rettemp; }
	
	if(mykeyvalue.key != GETV(INVALIDDATA) && mykeyvalue.value != GETV(INVALIDDATA)){ destbuffer[destoffset + rowindex] = GETKV2(myvprop); }
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->globalstats_countkvsreduced(1);
	if(mykeyvalue.key != GETV(INVALIDDATA) && mykeyvalue.value != GETV(INVALIDDATA)){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(1); }
	#endif
	return;
}

void 
	#ifdef SW 
	actsproc::
	#endif 
reduceandbuffer(bool_type enable, keyvalue_buffer_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[NUM_PARTITIONS], keyvalue_vbuffer_t vbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], sweepparams_t sweepparams, globalparams_t globalparams){				
	if(enable == OFF){ return; }
	#if REDUCEBUFFERFACTOR==8
	analysis_type analysis_loopcount = (BLOCKRAM_SIZE / (NUM_PARTITIONS / 2)); // =46: '2' is safety padding.
	#endif 
	#if REDUCEBUFFERFACTOR==16
	analysis_type analysis_loopcount = (BLOCKRAM_SIZE / NUM_PARTITIONS);
	#endif 
	
	keyvalue_buffer_t kvdata0;
	keyvalue_buffer_t kvdata1;
	keyvalue_buffer_t kvdata2;
	keyvalue_buffer_t kvdata3;
	keyvalue_buffer_t kvdata4;
	keyvalue_buffer_t kvdata5;
	keyvalue_buffer_t kvdata6;
	keyvalue_buffer_t kvdata7;
	keyvalue_buffer_t kvdata8;
	keyvalue_buffer_t kvdata9;
	keyvalue_buffer_t kvdata10;
	keyvalue_buffer_t kvdata11;
	keyvalue_buffer_t kvdata12;
	keyvalue_buffer_t kvdata13;
	keyvalue_buffer_t kvdata14;
	keyvalue_buffer_t kvdata15;
	keyvalue_buffer_t kvdata16;
	keyvalue_buffer_t kvdata17;
	keyvalue_buffer_t kvdata18;
	keyvalue_buffer_t kvdata19;
	keyvalue_buffer_t kvdata20;
	keyvalue_buffer_t kvdata21;
	keyvalue_buffer_t kvdata22;
	keyvalue_buffer_t kvdata23;
	keyvalue_buffer_t kvdata24;
	keyvalue_buffer_t kvdata25;
	keyvalue_buffer_t kvdata26;
	keyvalue_buffer_t kvdata27;
	keyvalue_buffer_t kvdata28;
	keyvalue_buffer_t kvdata29;
	keyvalue_buffer_t kvdata30;
	keyvalue_buffer_t kvdata31;
	keyvalue_buffer_t kvdata32;
	keyvalue_buffer_t kvdata33;
	keyvalue_buffer_t kvdata34;
	keyvalue_buffer_t kvdata35;
	keyvalue_buffer_t kvdata36;
	keyvalue_buffer_t kvdata37;
	keyvalue_buffer_t kvdata38;
	keyvalue_buffer_t kvdata39;
	keyvalue_buffer_t kvdata40;
	keyvalue_buffer_t kvdata41;
	keyvalue_buffer_t kvdata42;
	keyvalue_buffer_t kvdata43;
	keyvalue_buffer_t kvdata44;
	keyvalue_buffer_t kvdata45;
	keyvalue_buffer_t kvdata46;
	keyvalue_buffer_t kvdata47;
	keyvalue_buffer_t kvdata48;
	keyvalue_buffer_t kvdata49;
	keyvalue_buffer_t kvdata50;
	keyvalue_buffer_t kvdata51;
	keyvalue_buffer_t kvdata52;
	keyvalue_buffer_t kvdata53;
	keyvalue_buffer_t kvdata54;
	keyvalue_buffer_t kvdata55;
	keyvalue_buffer_t kvdata56;
	keyvalue_buffer_t kvdata57;
	keyvalue_buffer_t kvdata58;
	keyvalue_buffer_t kvdata59;
	keyvalue_buffer_t kvdata60;
	keyvalue_buffer_t kvdata61;
	keyvalue_buffer_t kvdata62;
	keyvalue_buffer_t kvdata63;
	keyvalue_buffer_t kvdata64;
	keyvalue_buffer_t kvdata65;
	keyvalue_buffer_t kvdata66;
	keyvalue_buffer_t kvdata67;
	keyvalue_buffer_t kvdata68;
	keyvalue_buffer_t kvdata69;
	keyvalue_buffer_t kvdata70;
	keyvalue_buffer_t kvdata71;
	keyvalue_buffer_t kvdata72;
	keyvalue_buffer_t kvdata73;
	keyvalue_buffer_t kvdata74;
	keyvalue_buffer_t kvdata75;
	keyvalue_buffer_t kvdata76;
	keyvalue_buffer_t kvdata77;
	keyvalue_buffer_t kvdata78;
	keyvalue_buffer_t kvdata79;
	keyvalue_buffer_t kvdata80;
	keyvalue_buffer_t kvdata81;
	keyvalue_buffer_t kvdata82;
	keyvalue_buffer_t kvdata83;
	keyvalue_buffer_t kvdata84;
	keyvalue_buffer_t kvdata85;
	keyvalue_buffer_t kvdata86;
	keyvalue_buffer_t kvdata87;
	keyvalue_buffer_t kvdata88;
	keyvalue_buffer_t kvdata89;
	keyvalue_buffer_t kvdata90;
	keyvalue_buffer_t kvdata91;
	keyvalue_buffer_t kvdata92;
	keyvalue_buffer_t kvdata93;
	keyvalue_buffer_t kvdata94;
	keyvalue_buffer_t kvdata95;
	keyvalue_buffer_t kvdata96;
	keyvalue_buffer_t kvdata97;
	keyvalue_buffer_t kvdata98;
	keyvalue_buffer_t kvdata99;
	keyvalue_buffer_t kvdata100;
	keyvalue_buffer_t kvdata101;
	keyvalue_buffer_t kvdata102;
	keyvalue_buffer_t kvdata103;
	keyvalue_buffer_t kvdata104;
	keyvalue_buffer_t kvdata105;
	keyvalue_buffer_t kvdata106;
	keyvalue_buffer_t kvdata107;
	keyvalue_buffer_t kvdata108;
	keyvalue_buffer_t kvdata109;
	keyvalue_buffer_t kvdata110;
	keyvalue_buffer_t kvdata111;
	keyvalue_buffer_t kvdata112;
	keyvalue_buffer_t kvdata113;
	keyvalue_buffer_t kvdata114;
	keyvalue_buffer_t kvdata115;
	keyvalue_buffer_t kvdata116;
	keyvalue_buffer_t kvdata117;
	keyvalue_buffer_t kvdata118;
	keyvalue_buffer_t kvdata119;
	keyvalue_buffer_t kvdata120;
	keyvalue_buffer_t kvdata121;
	keyvalue_buffer_t kvdata122;
	keyvalue_buffer_t kvdata123;
	keyvalue_buffer_t kvdata124;
	keyvalue_buffer_t kvdata125;
	keyvalue_buffer_t kvdata126;
	keyvalue_buffer_t kvdata127;
	keyvalue_buffer_t kvdata128;
	keyvalue_buffer_t kvdata129;
	keyvalue_buffer_t kvdata130;
	keyvalue_buffer_t kvdata131;
	keyvalue_buffer_t kvdata132;
	keyvalue_buffer_t kvdata133;
	keyvalue_buffer_t kvdata134;
	keyvalue_buffer_t kvdata135;
	keyvalue_buffer_t kvdata136;
	keyvalue_buffer_t kvdata137;
	keyvalue_buffer_t kvdata138;
	keyvalue_buffer_t kvdata139;
	keyvalue_buffer_t kvdata140;
	keyvalue_buffer_t kvdata141;
	keyvalue_buffer_t kvdata142;
	keyvalue_buffer_t kvdata143;
	keyvalue_buffer_t kvdata144;
	keyvalue_buffer_t kvdata145;
	keyvalue_buffer_t kvdata146;
	keyvalue_buffer_t kvdata147;
	keyvalue_buffer_t kvdata148;
	keyvalue_buffer_t kvdata149;
	keyvalue_buffer_t kvdata150;
	keyvalue_buffer_t kvdata151;
	keyvalue_buffer_t kvdata152;
	keyvalue_buffer_t kvdata153;
	keyvalue_buffer_t kvdata154;
	keyvalue_buffer_t kvdata155;
	keyvalue_buffer_t kvdata156;
	keyvalue_buffer_t kvdata157;
	keyvalue_buffer_t kvdata158;
	keyvalue_buffer_t kvdata159;
	keyvalue_buffer_t kvdata160;
	keyvalue_buffer_t kvdata161;
	keyvalue_buffer_t kvdata162;
	keyvalue_buffer_t kvdata163;
	keyvalue_buffer_t kvdata164;
	keyvalue_buffer_t kvdata165;
	keyvalue_buffer_t kvdata166;
	keyvalue_buffer_t kvdata167;
	keyvalue_buffer_t kvdata168;
	keyvalue_buffer_t kvdata169;
	keyvalue_buffer_t kvdata170;
	keyvalue_buffer_t kvdata171;
	keyvalue_buffer_t kvdata172;
	keyvalue_buffer_t kvdata173;
	keyvalue_buffer_t kvdata174;
	keyvalue_buffer_t kvdata175;
	keyvalue_buffer_t kvdata176;
	keyvalue_buffer_t kvdata177;
	keyvalue_buffer_t kvdata178;
	keyvalue_buffer_t kvdata179;
	keyvalue_buffer_t kvdata180;
	keyvalue_buffer_t kvdata181;
	keyvalue_buffer_t kvdata182;
	keyvalue_buffer_t kvdata183;
	keyvalue_buffer_t kvdata184;
	keyvalue_buffer_t kvdata185;
	keyvalue_buffer_t kvdata186;
	keyvalue_buffer_t kvdata187;
	keyvalue_buffer_t kvdata188;
	keyvalue_buffer_t kvdata189;
	keyvalue_buffer_t kvdata190;
	keyvalue_buffer_t kvdata191;
	keyvalue_buffer_t kvdata192;
	keyvalue_buffer_t kvdata193;
	keyvalue_buffer_t kvdata194;
	keyvalue_buffer_t kvdata195;
	keyvalue_buffer_t kvdata196;
	keyvalue_buffer_t kvdata197;
	keyvalue_buffer_t kvdata198;
	keyvalue_buffer_t kvdata199;
	keyvalue_buffer_t kvdata200;
	keyvalue_buffer_t kvdata201;
	keyvalue_buffer_t kvdata202;
	keyvalue_buffer_t kvdata203;
	keyvalue_buffer_t kvdata204;
	keyvalue_buffer_t kvdata205;
	keyvalue_buffer_t kvdata206;
	keyvalue_buffer_t kvdata207;
	keyvalue_buffer_t kvdata208;
	keyvalue_buffer_t kvdata209;
	keyvalue_buffer_t kvdata210;
	keyvalue_buffer_t kvdata211;
	keyvalue_buffer_t kvdata212;
	keyvalue_buffer_t kvdata213;
	keyvalue_buffer_t kvdata214;
	keyvalue_buffer_t kvdata215;
	keyvalue_buffer_t kvdata216;
	keyvalue_buffer_t kvdata217;
	keyvalue_buffer_t kvdata218;
	keyvalue_buffer_t kvdata219;
	keyvalue_buffer_t kvdata220;
	keyvalue_buffer_t kvdata221;
	keyvalue_buffer_t kvdata222;
	keyvalue_buffer_t kvdata223;
	keyvalue_buffer_t kvdata224;
	keyvalue_buffer_t kvdata225;
	keyvalue_buffer_t kvdata226;
	keyvalue_buffer_t kvdata227;
	keyvalue_buffer_t kvdata228;
	keyvalue_buffer_t kvdata229;
	keyvalue_buffer_t kvdata230;
	keyvalue_buffer_t kvdata231;
	keyvalue_buffer_t kvdata232;
	keyvalue_buffer_t kvdata233;
	keyvalue_buffer_t kvdata234;
	keyvalue_buffer_t kvdata235;
	keyvalue_buffer_t kvdata236;
	keyvalue_buffer_t kvdata237;
	keyvalue_buffer_t kvdata238;
	keyvalue_buffer_t kvdata239;
	keyvalue_buffer_t kvdata240;
	keyvalue_buffer_t kvdata241;
	keyvalue_buffer_t kvdata242;
	keyvalue_buffer_t kvdata243;
	keyvalue_buffer_t kvdata244;
	keyvalue_buffer_t kvdata245;
	keyvalue_buffer_t kvdata246;
	keyvalue_buffer_t kvdata247;
	keyvalue_buffer_t kvdata248;
	keyvalue_buffer_t kvdata249;
	keyvalue_buffer_t kvdata250;
	keyvalue_buffer_t kvdata251;
	keyvalue_buffer_t kvdata252;
	keyvalue_buffer_t kvdata253;
	keyvalue_buffer_t kvdata254;
	keyvalue_buffer_t kvdata255;
	buffer_type bramoffset_kvs[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=bramoffset_kvs complete
	buffer_type size_kvs[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=size_kvs complete
	unsigned int upperlimits[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=upperlimits complete
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printkeyvalues("reduceandbuffer.localcapsule", (keyvalue_t *)localcapsule, NUM_PARTITIONS);
	#endif 

	unsigned int tmplloprange = 0;
	#if REDUCEBUFFERFACTOR==8
	REDUCEBUFFERPARTITIONS_LOOP1: for(unsigned int it=0; it<NUM_PARTITIONS; it+=NUM_PARTITIONS/2){
		buffer_type maxsize_kvs = 0;
		REDUCEBUFFERPARTITIONS_LOOP1B: for(partition_type p=0; p<NUM_PARTITIONS/2; p++){
		#pragma HLS PIPELINE II=1
			bramoffset_kvs[p] = localcapsule[it+p].key / VECTOR_SIZE;
			size_kvs[p] = localcapsule[it+p].value / VECTOR_SIZE;
			if(maxsize_kvs < size_kvs[p]){ maxsize_kvs = size_kvs[p]; }
		}
		
		unsigned int lloprange = getrangeforeachllop(globalparams);
		REDUCEBUFFERPARTITIONS_LOOP1C: for(unsigned int p=0; p<NUM_PARTITIONS/2; p++){
		#pragma HLS PIPELINE II=1
			upperlimits[p] = sweepparams.upperlimit + tmplloprange;
			tmplloprange += lloprange;
		}
		
		REDUCEBUFFERPARTITIONS_LOOP1D: for(buffer_type i=0; i<maxsize_kvs; i++){
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
	

				if(i< size_kvs[0]){ reducevector(kvdata0, vbuffer[it+0], 0, upperlimits[0], sweepparams, globalparams); }
				if(i< size_kvs[1]){ reducevector(kvdata1, vbuffer[it+1], 0, upperlimits[1], sweepparams, globalparams); }
				if(i< size_kvs[2]){ reducevector(kvdata2, vbuffer[it+2], 0, upperlimits[2], sweepparams, globalparams); }
				if(i< size_kvs[3]){ reducevector(kvdata3, vbuffer[it+3], 0, upperlimits[3], sweepparams, globalparams); }
				if(i< size_kvs[4]){ reducevector(kvdata4, vbuffer[it+4], 0, upperlimits[4], sweepparams, globalparams); }
				if(i< size_kvs[5]){ reducevector(kvdata5, vbuffer[it+5], 0, upperlimits[5], sweepparams, globalparams); }
				if(i< size_kvs[6]){ reducevector(kvdata6, vbuffer[it+6], 0, upperlimits[6], sweepparams, globalparams); }
				if(i< size_kvs[7]){ reducevector(kvdata7, vbuffer[it+7], 0, upperlimits[7], sweepparams, globalparams); }
			}
		}
	}
	#endif 
	#if REDUCEBUFFERFACTOR==16
	buffer_type maxsize_kvs = 0;
	REDUCEBUFFERPARTITIONS_LOOP1B: for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		bramoffset_kvs[p] = localcapsule[p].key / VECTOR_SIZE;
		size_kvs[p] = localcapsule[p].value / VECTOR_SIZE;
		if(maxsize_kvs < size_kvs[p]){ maxsize_kvs = size_kvs[p]; }
	}
	
	unsigned int lloprange = getrangeforeachllop(globalparams);
	REDUCEBUFFERPARTITIONS_LOOP1C: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		upperlimits[p] = sweepparams.upperlimit + tmplloprange;
		tmplloprange += lloprange;
	}
	
	REDUCEBUFFERPARTITIONS_LOOP1D: for(buffer_type i=0; i<maxsize_kvs; i++){
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
			kvdata16 = buffer[v][bramoffset_kvs[16] + i]; 	
			kvdata17 = buffer[v][bramoffset_kvs[17] + i]; 	
			kvdata18 = buffer[v][bramoffset_kvs[18] + i]; 	
			kvdata19 = buffer[v][bramoffset_kvs[19] + i]; 	
			kvdata20 = buffer[v][bramoffset_kvs[20] + i]; 	
			kvdata21 = buffer[v][bramoffset_kvs[21] + i]; 	
			kvdata22 = buffer[v][bramoffset_kvs[22] + i]; 	
			kvdata23 = buffer[v][bramoffset_kvs[23] + i]; 	
			kvdata24 = buffer[v][bramoffset_kvs[24] + i]; 	
			kvdata25 = buffer[v][bramoffset_kvs[25] + i]; 	
			kvdata26 = buffer[v][bramoffset_kvs[26] + i]; 	
			kvdata27 = buffer[v][bramoffset_kvs[27] + i]; 	
			kvdata28 = buffer[v][bramoffset_kvs[28] + i]; 	
			kvdata29 = buffer[v][bramoffset_kvs[29] + i]; 	
			kvdata30 = buffer[v][bramoffset_kvs[30] + i]; 	
			kvdata31 = buffer[v][bramoffset_kvs[31] + i]; 	
			kvdata32 = buffer[v][bramoffset_kvs[32] + i]; 	
			kvdata33 = buffer[v][bramoffset_kvs[33] + i]; 	
			kvdata34 = buffer[v][bramoffset_kvs[34] + i]; 	
			kvdata35 = buffer[v][bramoffset_kvs[35] + i]; 	
			kvdata36 = buffer[v][bramoffset_kvs[36] + i]; 	
			kvdata37 = buffer[v][bramoffset_kvs[37] + i]; 	
			kvdata38 = buffer[v][bramoffset_kvs[38] + i]; 	
			kvdata39 = buffer[v][bramoffset_kvs[39] + i]; 	
			kvdata40 = buffer[v][bramoffset_kvs[40] + i]; 	
			kvdata41 = buffer[v][bramoffset_kvs[41] + i]; 	
			kvdata42 = buffer[v][bramoffset_kvs[42] + i]; 	
			kvdata43 = buffer[v][bramoffset_kvs[43] + i]; 	
			kvdata44 = buffer[v][bramoffset_kvs[44] + i]; 	
			kvdata45 = buffer[v][bramoffset_kvs[45] + i]; 	
			kvdata46 = buffer[v][bramoffset_kvs[46] + i]; 	
			kvdata47 = buffer[v][bramoffset_kvs[47] + i]; 	
			kvdata48 = buffer[v][bramoffset_kvs[48] + i]; 	
			kvdata49 = buffer[v][bramoffset_kvs[49] + i]; 	
			kvdata50 = buffer[v][bramoffset_kvs[50] + i]; 	
			kvdata51 = buffer[v][bramoffset_kvs[51] + i]; 	
			kvdata52 = buffer[v][bramoffset_kvs[52] + i]; 	
			kvdata53 = buffer[v][bramoffset_kvs[53] + i]; 	
			kvdata54 = buffer[v][bramoffset_kvs[54] + i]; 	
			kvdata55 = buffer[v][bramoffset_kvs[55] + i]; 	
			kvdata56 = buffer[v][bramoffset_kvs[56] + i]; 	
			kvdata57 = buffer[v][bramoffset_kvs[57] + i]; 	
			kvdata58 = buffer[v][bramoffset_kvs[58] + i]; 	
			kvdata59 = buffer[v][bramoffset_kvs[59] + i]; 	
			kvdata60 = buffer[v][bramoffset_kvs[60] + i]; 	
			kvdata61 = buffer[v][bramoffset_kvs[61] + i]; 	
			kvdata62 = buffer[v][bramoffset_kvs[62] + i]; 	
			kvdata63 = buffer[v][bramoffset_kvs[63] + i]; 	
			kvdata64 = buffer[v][bramoffset_kvs[64] + i]; 	
			kvdata65 = buffer[v][bramoffset_kvs[65] + i]; 	
			kvdata66 = buffer[v][bramoffset_kvs[66] + i]; 	
			kvdata67 = buffer[v][bramoffset_kvs[67] + i]; 	
			kvdata68 = buffer[v][bramoffset_kvs[68] + i]; 	
			kvdata69 = buffer[v][bramoffset_kvs[69] + i]; 	
			kvdata70 = buffer[v][bramoffset_kvs[70] + i]; 	
			kvdata71 = buffer[v][bramoffset_kvs[71] + i]; 	
			kvdata72 = buffer[v][bramoffset_kvs[72] + i]; 	
			kvdata73 = buffer[v][bramoffset_kvs[73] + i]; 	
			kvdata74 = buffer[v][bramoffset_kvs[74] + i]; 	
			kvdata75 = buffer[v][bramoffset_kvs[75] + i]; 	
			kvdata76 = buffer[v][bramoffset_kvs[76] + i]; 	
			kvdata77 = buffer[v][bramoffset_kvs[77] + i]; 	
			kvdata78 = buffer[v][bramoffset_kvs[78] + i]; 	
			kvdata79 = buffer[v][bramoffset_kvs[79] + i]; 	
			kvdata80 = buffer[v][bramoffset_kvs[80] + i]; 	
			kvdata81 = buffer[v][bramoffset_kvs[81] + i]; 	
			kvdata82 = buffer[v][bramoffset_kvs[82] + i]; 	
			kvdata83 = buffer[v][bramoffset_kvs[83] + i]; 	
			kvdata84 = buffer[v][bramoffset_kvs[84] + i]; 	
			kvdata85 = buffer[v][bramoffset_kvs[85] + i]; 	
			kvdata86 = buffer[v][bramoffset_kvs[86] + i]; 	
			kvdata87 = buffer[v][bramoffset_kvs[87] + i]; 	
			kvdata88 = buffer[v][bramoffset_kvs[88] + i]; 	
			kvdata89 = buffer[v][bramoffset_kvs[89] + i]; 	
			kvdata90 = buffer[v][bramoffset_kvs[90] + i]; 	
			kvdata91 = buffer[v][bramoffset_kvs[91] + i]; 	
			kvdata92 = buffer[v][bramoffset_kvs[92] + i]; 	
			kvdata93 = buffer[v][bramoffset_kvs[93] + i]; 	
			kvdata94 = buffer[v][bramoffset_kvs[94] + i]; 	
			kvdata95 = buffer[v][bramoffset_kvs[95] + i]; 	
			kvdata96 = buffer[v][bramoffset_kvs[96] + i]; 	
			kvdata97 = buffer[v][bramoffset_kvs[97] + i]; 	
			kvdata98 = buffer[v][bramoffset_kvs[98] + i]; 	
			kvdata99 = buffer[v][bramoffset_kvs[99] + i]; 	
			kvdata100 = buffer[v][bramoffset_kvs[100] + i]; 	
			kvdata101 = buffer[v][bramoffset_kvs[101] + i]; 	
			kvdata102 = buffer[v][bramoffset_kvs[102] + i]; 	
			kvdata103 = buffer[v][bramoffset_kvs[103] + i]; 	
			kvdata104 = buffer[v][bramoffset_kvs[104] + i]; 	
			kvdata105 = buffer[v][bramoffset_kvs[105] + i]; 	
			kvdata106 = buffer[v][bramoffset_kvs[106] + i]; 	
			kvdata107 = buffer[v][bramoffset_kvs[107] + i]; 	
			kvdata108 = buffer[v][bramoffset_kvs[108] + i]; 	
			kvdata109 = buffer[v][bramoffset_kvs[109] + i]; 	
			kvdata110 = buffer[v][bramoffset_kvs[110] + i]; 	
			kvdata111 = buffer[v][bramoffset_kvs[111] + i]; 	
			kvdata112 = buffer[v][bramoffset_kvs[112] + i]; 	
			kvdata113 = buffer[v][bramoffset_kvs[113] + i]; 	
			kvdata114 = buffer[v][bramoffset_kvs[114] + i]; 	
			kvdata115 = buffer[v][bramoffset_kvs[115] + i]; 	
			kvdata116 = buffer[v][bramoffset_kvs[116] + i]; 	
			kvdata117 = buffer[v][bramoffset_kvs[117] + i]; 	
			kvdata118 = buffer[v][bramoffset_kvs[118] + i]; 	
			kvdata119 = buffer[v][bramoffset_kvs[119] + i]; 	
			kvdata120 = buffer[v][bramoffset_kvs[120] + i]; 	
			kvdata121 = buffer[v][bramoffset_kvs[121] + i]; 	
			kvdata122 = buffer[v][bramoffset_kvs[122] + i]; 	
			kvdata123 = buffer[v][bramoffset_kvs[123] + i]; 	
			kvdata124 = buffer[v][bramoffset_kvs[124] + i]; 	
			kvdata125 = buffer[v][bramoffset_kvs[125] + i]; 	
			kvdata126 = buffer[v][bramoffset_kvs[126] + i]; 	
			kvdata127 = buffer[v][bramoffset_kvs[127] + i]; 	
			kvdata128 = buffer[v][bramoffset_kvs[128] + i]; 	
			kvdata129 = buffer[v][bramoffset_kvs[129] + i]; 	
			kvdata130 = buffer[v][bramoffset_kvs[130] + i]; 	
			kvdata131 = buffer[v][bramoffset_kvs[131] + i]; 	
			kvdata132 = buffer[v][bramoffset_kvs[132] + i]; 	
			kvdata133 = buffer[v][bramoffset_kvs[133] + i]; 	
			kvdata134 = buffer[v][bramoffset_kvs[134] + i]; 	
			kvdata135 = buffer[v][bramoffset_kvs[135] + i]; 	
			kvdata136 = buffer[v][bramoffset_kvs[136] + i]; 	
			kvdata137 = buffer[v][bramoffset_kvs[137] + i]; 	
			kvdata138 = buffer[v][bramoffset_kvs[138] + i]; 	
			kvdata139 = buffer[v][bramoffset_kvs[139] + i]; 	
			kvdata140 = buffer[v][bramoffset_kvs[140] + i]; 	
			kvdata141 = buffer[v][bramoffset_kvs[141] + i]; 	
			kvdata142 = buffer[v][bramoffset_kvs[142] + i]; 	
			kvdata143 = buffer[v][bramoffset_kvs[143] + i]; 	
			kvdata144 = buffer[v][bramoffset_kvs[144] + i]; 	
			kvdata145 = buffer[v][bramoffset_kvs[145] + i]; 	
			kvdata146 = buffer[v][bramoffset_kvs[146] + i]; 	
			kvdata147 = buffer[v][bramoffset_kvs[147] + i]; 	
			kvdata148 = buffer[v][bramoffset_kvs[148] + i]; 	
			kvdata149 = buffer[v][bramoffset_kvs[149] + i]; 	
			kvdata150 = buffer[v][bramoffset_kvs[150] + i]; 	
			kvdata151 = buffer[v][bramoffset_kvs[151] + i]; 	
			kvdata152 = buffer[v][bramoffset_kvs[152] + i]; 	
			kvdata153 = buffer[v][bramoffset_kvs[153] + i]; 	
			kvdata154 = buffer[v][bramoffset_kvs[154] + i]; 	
			kvdata155 = buffer[v][bramoffset_kvs[155] + i]; 	
			kvdata156 = buffer[v][bramoffset_kvs[156] + i]; 	
			kvdata157 = buffer[v][bramoffset_kvs[157] + i]; 	
			kvdata158 = buffer[v][bramoffset_kvs[158] + i]; 	
			kvdata159 = buffer[v][bramoffset_kvs[159] + i]; 	
			kvdata160 = buffer[v][bramoffset_kvs[160] + i]; 	
			kvdata161 = buffer[v][bramoffset_kvs[161] + i]; 	
			kvdata162 = buffer[v][bramoffset_kvs[162] + i]; 	
			kvdata163 = buffer[v][bramoffset_kvs[163] + i]; 	
			kvdata164 = buffer[v][bramoffset_kvs[164] + i]; 	
			kvdata165 = buffer[v][bramoffset_kvs[165] + i]; 	
			kvdata166 = buffer[v][bramoffset_kvs[166] + i]; 	
			kvdata167 = buffer[v][bramoffset_kvs[167] + i]; 	
			kvdata168 = buffer[v][bramoffset_kvs[168] + i]; 	
			kvdata169 = buffer[v][bramoffset_kvs[169] + i]; 	
			kvdata170 = buffer[v][bramoffset_kvs[170] + i]; 	
			kvdata171 = buffer[v][bramoffset_kvs[171] + i]; 	
			kvdata172 = buffer[v][bramoffset_kvs[172] + i]; 	
			kvdata173 = buffer[v][bramoffset_kvs[173] + i]; 	
			kvdata174 = buffer[v][bramoffset_kvs[174] + i]; 	
			kvdata175 = buffer[v][bramoffset_kvs[175] + i]; 	
			kvdata176 = buffer[v][bramoffset_kvs[176] + i]; 	
			kvdata177 = buffer[v][bramoffset_kvs[177] + i]; 	
			kvdata178 = buffer[v][bramoffset_kvs[178] + i]; 	
			kvdata179 = buffer[v][bramoffset_kvs[179] + i]; 	
			kvdata180 = buffer[v][bramoffset_kvs[180] + i]; 	
			kvdata181 = buffer[v][bramoffset_kvs[181] + i]; 	
			kvdata182 = buffer[v][bramoffset_kvs[182] + i]; 	
			kvdata183 = buffer[v][bramoffset_kvs[183] + i]; 	
			kvdata184 = buffer[v][bramoffset_kvs[184] + i]; 	
			kvdata185 = buffer[v][bramoffset_kvs[185] + i]; 	
			kvdata186 = buffer[v][bramoffset_kvs[186] + i]; 	
			kvdata187 = buffer[v][bramoffset_kvs[187] + i]; 	
			kvdata188 = buffer[v][bramoffset_kvs[188] + i]; 	
			kvdata189 = buffer[v][bramoffset_kvs[189] + i]; 	
			kvdata190 = buffer[v][bramoffset_kvs[190] + i]; 	
			kvdata191 = buffer[v][bramoffset_kvs[191] + i]; 	
			kvdata192 = buffer[v][bramoffset_kvs[192] + i]; 	
			kvdata193 = buffer[v][bramoffset_kvs[193] + i]; 	
			kvdata194 = buffer[v][bramoffset_kvs[194] + i]; 	
			kvdata195 = buffer[v][bramoffset_kvs[195] + i]; 	
			kvdata196 = buffer[v][bramoffset_kvs[196] + i]; 	
			kvdata197 = buffer[v][bramoffset_kvs[197] + i]; 	
			kvdata198 = buffer[v][bramoffset_kvs[198] + i]; 	
			kvdata199 = buffer[v][bramoffset_kvs[199] + i]; 	
			kvdata200 = buffer[v][bramoffset_kvs[200] + i]; 	
			kvdata201 = buffer[v][bramoffset_kvs[201] + i]; 	
			kvdata202 = buffer[v][bramoffset_kvs[202] + i]; 	
			kvdata203 = buffer[v][bramoffset_kvs[203] + i]; 	
			kvdata204 = buffer[v][bramoffset_kvs[204] + i]; 	
			kvdata205 = buffer[v][bramoffset_kvs[205] + i]; 	
			kvdata206 = buffer[v][bramoffset_kvs[206] + i]; 	
			kvdata207 = buffer[v][bramoffset_kvs[207] + i]; 	
			kvdata208 = buffer[v][bramoffset_kvs[208] + i]; 	
			kvdata209 = buffer[v][bramoffset_kvs[209] + i]; 	
			kvdata210 = buffer[v][bramoffset_kvs[210] + i]; 	
			kvdata211 = buffer[v][bramoffset_kvs[211] + i]; 	
			kvdata212 = buffer[v][bramoffset_kvs[212] + i]; 	
			kvdata213 = buffer[v][bramoffset_kvs[213] + i]; 	
			kvdata214 = buffer[v][bramoffset_kvs[214] + i]; 	
			kvdata215 = buffer[v][bramoffset_kvs[215] + i]; 	
			kvdata216 = buffer[v][bramoffset_kvs[216] + i]; 	
			kvdata217 = buffer[v][bramoffset_kvs[217] + i]; 	
			kvdata218 = buffer[v][bramoffset_kvs[218] + i]; 	
			kvdata219 = buffer[v][bramoffset_kvs[219] + i]; 	
			kvdata220 = buffer[v][bramoffset_kvs[220] + i]; 	
			kvdata221 = buffer[v][bramoffset_kvs[221] + i]; 	
			kvdata222 = buffer[v][bramoffset_kvs[222] + i]; 	
			kvdata223 = buffer[v][bramoffset_kvs[223] + i]; 	
			kvdata224 = buffer[v][bramoffset_kvs[224] + i]; 	
			kvdata225 = buffer[v][bramoffset_kvs[225] + i]; 	
			kvdata226 = buffer[v][bramoffset_kvs[226] + i]; 	
			kvdata227 = buffer[v][bramoffset_kvs[227] + i]; 	
			kvdata228 = buffer[v][bramoffset_kvs[228] + i]; 	
			kvdata229 = buffer[v][bramoffset_kvs[229] + i]; 	
			kvdata230 = buffer[v][bramoffset_kvs[230] + i]; 	
			kvdata231 = buffer[v][bramoffset_kvs[231] + i]; 	
			kvdata232 = buffer[v][bramoffset_kvs[232] + i]; 	
			kvdata233 = buffer[v][bramoffset_kvs[233] + i]; 	
			kvdata234 = buffer[v][bramoffset_kvs[234] + i]; 	
			kvdata235 = buffer[v][bramoffset_kvs[235] + i]; 	
			kvdata236 = buffer[v][bramoffset_kvs[236] + i]; 	
			kvdata237 = buffer[v][bramoffset_kvs[237] + i]; 	
			kvdata238 = buffer[v][bramoffset_kvs[238] + i]; 	
			kvdata239 = buffer[v][bramoffset_kvs[239] + i]; 	
			kvdata240 = buffer[v][bramoffset_kvs[240] + i]; 	
			kvdata241 = buffer[v][bramoffset_kvs[241] + i]; 	
			kvdata242 = buffer[v][bramoffset_kvs[242] + i]; 	
			kvdata243 = buffer[v][bramoffset_kvs[243] + i]; 	
			kvdata244 = buffer[v][bramoffset_kvs[244] + i]; 	
			kvdata245 = buffer[v][bramoffset_kvs[245] + i]; 	
			kvdata246 = buffer[v][bramoffset_kvs[246] + i]; 	
			kvdata247 = buffer[v][bramoffset_kvs[247] + i]; 	
			kvdata248 = buffer[v][bramoffset_kvs[248] + i]; 	
			kvdata249 = buffer[v][bramoffset_kvs[249] + i]; 	
			kvdata250 = buffer[v][bramoffset_kvs[250] + i]; 	
			kvdata251 = buffer[v][bramoffset_kvs[251] + i]; 	
			kvdata252 = buffer[v][bramoffset_kvs[252] + i]; 	
			kvdata253 = buffer[v][bramoffset_kvs[253] + i]; 	
			kvdata254 = buffer[v][bramoffset_kvs[254] + i]; 	
			kvdata255 = buffer[v][bramoffset_kvs[255] + i]; 	
	

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
			if(i< size_kvs[16]){ reducevector(kvdata16, vbuffer[16], 0, upperlimits[16], sweepparams, globalparams); }
			if(i< size_kvs[17]){ reducevector(kvdata17, vbuffer[17], 0, upperlimits[17], sweepparams, globalparams); }
			if(i< size_kvs[18]){ reducevector(kvdata18, vbuffer[18], 0, upperlimits[18], sweepparams, globalparams); }
			if(i< size_kvs[19]){ reducevector(kvdata19, vbuffer[19], 0, upperlimits[19], sweepparams, globalparams); }
			if(i< size_kvs[20]){ reducevector(kvdata20, vbuffer[20], 0, upperlimits[20], sweepparams, globalparams); }
			if(i< size_kvs[21]){ reducevector(kvdata21, vbuffer[21], 0, upperlimits[21], sweepparams, globalparams); }
			if(i< size_kvs[22]){ reducevector(kvdata22, vbuffer[22], 0, upperlimits[22], sweepparams, globalparams); }
			if(i< size_kvs[23]){ reducevector(kvdata23, vbuffer[23], 0, upperlimits[23], sweepparams, globalparams); }
			if(i< size_kvs[24]){ reducevector(kvdata24, vbuffer[24], 0, upperlimits[24], sweepparams, globalparams); }
			if(i< size_kvs[25]){ reducevector(kvdata25, vbuffer[25], 0, upperlimits[25], sweepparams, globalparams); }
			if(i< size_kvs[26]){ reducevector(kvdata26, vbuffer[26], 0, upperlimits[26], sweepparams, globalparams); }
			if(i< size_kvs[27]){ reducevector(kvdata27, vbuffer[27], 0, upperlimits[27], sweepparams, globalparams); }
			if(i< size_kvs[28]){ reducevector(kvdata28, vbuffer[28], 0, upperlimits[28], sweepparams, globalparams); }
			if(i< size_kvs[29]){ reducevector(kvdata29, vbuffer[29], 0, upperlimits[29], sweepparams, globalparams); }
			if(i< size_kvs[30]){ reducevector(kvdata30, vbuffer[30], 0, upperlimits[30], sweepparams, globalparams); }
			if(i< size_kvs[31]){ reducevector(kvdata31, vbuffer[31], 0, upperlimits[31], sweepparams, globalparams); }
			if(i< size_kvs[32]){ reducevector(kvdata32, vbuffer[32], 0, upperlimits[32], sweepparams, globalparams); }
			if(i< size_kvs[33]){ reducevector(kvdata33, vbuffer[33], 0, upperlimits[33], sweepparams, globalparams); }
			if(i< size_kvs[34]){ reducevector(kvdata34, vbuffer[34], 0, upperlimits[34], sweepparams, globalparams); }
			if(i< size_kvs[35]){ reducevector(kvdata35, vbuffer[35], 0, upperlimits[35], sweepparams, globalparams); }
			if(i< size_kvs[36]){ reducevector(kvdata36, vbuffer[36], 0, upperlimits[36], sweepparams, globalparams); }
			if(i< size_kvs[37]){ reducevector(kvdata37, vbuffer[37], 0, upperlimits[37], sweepparams, globalparams); }
			if(i< size_kvs[38]){ reducevector(kvdata38, vbuffer[38], 0, upperlimits[38], sweepparams, globalparams); }
			if(i< size_kvs[39]){ reducevector(kvdata39, vbuffer[39], 0, upperlimits[39], sweepparams, globalparams); }
			if(i< size_kvs[40]){ reducevector(kvdata40, vbuffer[40], 0, upperlimits[40], sweepparams, globalparams); }
			if(i< size_kvs[41]){ reducevector(kvdata41, vbuffer[41], 0, upperlimits[41], sweepparams, globalparams); }
			if(i< size_kvs[42]){ reducevector(kvdata42, vbuffer[42], 0, upperlimits[42], sweepparams, globalparams); }
			if(i< size_kvs[43]){ reducevector(kvdata43, vbuffer[43], 0, upperlimits[43], sweepparams, globalparams); }
			if(i< size_kvs[44]){ reducevector(kvdata44, vbuffer[44], 0, upperlimits[44], sweepparams, globalparams); }
			if(i< size_kvs[45]){ reducevector(kvdata45, vbuffer[45], 0, upperlimits[45], sweepparams, globalparams); }
			if(i< size_kvs[46]){ reducevector(kvdata46, vbuffer[46], 0, upperlimits[46], sweepparams, globalparams); }
			if(i< size_kvs[47]){ reducevector(kvdata47, vbuffer[47], 0, upperlimits[47], sweepparams, globalparams); }
			if(i< size_kvs[48]){ reducevector(kvdata48, vbuffer[48], 0, upperlimits[48], sweepparams, globalparams); }
			if(i< size_kvs[49]){ reducevector(kvdata49, vbuffer[49], 0, upperlimits[49], sweepparams, globalparams); }
			if(i< size_kvs[50]){ reducevector(kvdata50, vbuffer[50], 0, upperlimits[50], sweepparams, globalparams); }
			if(i< size_kvs[51]){ reducevector(kvdata51, vbuffer[51], 0, upperlimits[51], sweepparams, globalparams); }
			if(i< size_kvs[52]){ reducevector(kvdata52, vbuffer[52], 0, upperlimits[52], sweepparams, globalparams); }
			if(i< size_kvs[53]){ reducevector(kvdata53, vbuffer[53], 0, upperlimits[53], sweepparams, globalparams); }
			if(i< size_kvs[54]){ reducevector(kvdata54, vbuffer[54], 0, upperlimits[54], sweepparams, globalparams); }
			if(i< size_kvs[55]){ reducevector(kvdata55, vbuffer[55], 0, upperlimits[55], sweepparams, globalparams); }
			if(i< size_kvs[56]){ reducevector(kvdata56, vbuffer[56], 0, upperlimits[56], sweepparams, globalparams); }
			if(i< size_kvs[57]){ reducevector(kvdata57, vbuffer[57], 0, upperlimits[57], sweepparams, globalparams); }
			if(i< size_kvs[58]){ reducevector(kvdata58, vbuffer[58], 0, upperlimits[58], sweepparams, globalparams); }
			if(i< size_kvs[59]){ reducevector(kvdata59, vbuffer[59], 0, upperlimits[59], sweepparams, globalparams); }
			if(i< size_kvs[60]){ reducevector(kvdata60, vbuffer[60], 0, upperlimits[60], sweepparams, globalparams); }
			if(i< size_kvs[61]){ reducevector(kvdata61, vbuffer[61], 0, upperlimits[61], sweepparams, globalparams); }
			if(i< size_kvs[62]){ reducevector(kvdata62, vbuffer[62], 0, upperlimits[62], sweepparams, globalparams); }
			if(i< size_kvs[63]){ reducevector(kvdata63, vbuffer[63], 0, upperlimits[63], sweepparams, globalparams); }
			if(i< size_kvs[64]){ reducevector(kvdata64, vbuffer[64], 0, upperlimits[64], sweepparams, globalparams); }
			if(i< size_kvs[65]){ reducevector(kvdata65, vbuffer[65], 0, upperlimits[65], sweepparams, globalparams); }
			if(i< size_kvs[66]){ reducevector(kvdata66, vbuffer[66], 0, upperlimits[66], sweepparams, globalparams); }
			if(i< size_kvs[67]){ reducevector(kvdata67, vbuffer[67], 0, upperlimits[67], sweepparams, globalparams); }
			if(i< size_kvs[68]){ reducevector(kvdata68, vbuffer[68], 0, upperlimits[68], sweepparams, globalparams); }
			if(i< size_kvs[69]){ reducevector(kvdata69, vbuffer[69], 0, upperlimits[69], sweepparams, globalparams); }
			if(i< size_kvs[70]){ reducevector(kvdata70, vbuffer[70], 0, upperlimits[70], sweepparams, globalparams); }
			if(i< size_kvs[71]){ reducevector(kvdata71, vbuffer[71], 0, upperlimits[71], sweepparams, globalparams); }
			if(i< size_kvs[72]){ reducevector(kvdata72, vbuffer[72], 0, upperlimits[72], sweepparams, globalparams); }
			if(i< size_kvs[73]){ reducevector(kvdata73, vbuffer[73], 0, upperlimits[73], sweepparams, globalparams); }
			if(i< size_kvs[74]){ reducevector(kvdata74, vbuffer[74], 0, upperlimits[74], sweepparams, globalparams); }
			if(i< size_kvs[75]){ reducevector(kvdata75, vbuffer[75], 0, upperlimits[75], sweepparams, globalparams); }
			if(i< size_kvs[76]){ reducevector(kvdata76, vbuffer[76], 0, upperlimits[76], sweepparams, globalparams); }
			if(i< size_kvs[77]){ reducevector(kvdata77, vbuffer[77], 0, upperlimits[77], sweepparams, globalparams); }
			if(i< size_kvs[78]){ reducevector(kvdata78, vbuffer[78], 0, upperlimits[78], sweepparams, globalparams); }
			if(i< size_kvs[79]){ reducevector(kvdata79, vbuffer[79], 0, upperlimits[79], sweepparams, globalparams); }
			if(i< size_kvs[80]){ reducevector(kvdata80, vbuffer[80], 0, upperlimits[80], sweepparams, globalparams); }
			if(i< size_kvs[81]){ reducevector(kvdata81, vbuffer[81], 0, upperlimits[81], sweepparams, globalparams); }
			if(i< size_kvs[82]){ reducevector(kvdata82, vbuffer[82], 0, upperlimits[82], sweepparams, globalparams); }
			if(i< size_kvs[83]){ reducevector(kvdata83, vbuffer[83], 0, upperlimits[83], sweepparams, globalparams); }
			if(i< size_kvs[84]){ reducevector(kvdata84, vbuffer[84], 0, upperlimits[84], sweepparams, globalparams); }
			if(i< size_kvs[85]){ reducevector(kvdata85, vbuffer[85], 0, upperlimits[85], sweepparams, globalparams); }
			if(i< size_kvs[86]){ reducevector(kvdata86, vbuffer[86], 0, upperlimits[86], sweepparams, globalparams); }
			if(i< size_kvs[87]){ reducevector(kvdata87, vbuffer[87], 0, upperlimits[87], sweepparams, globalparams); }
			if(i< size_kvs[88]){ reducevector(kvdata88, vbuffer[88], 0, upperlimits[88], sweepparams, globalparams); }
			if(i< size_kvs[89]){ reducevector(kvdata89, vbuffer[89], 0, upperlimits[89], sweepparams, globalparams); }
			if(i< size_kvs[90]){ reducevector(kvdata90, vbuffer[90], 0, upperlimits[90], sweepparams, globalparams); }
			if(i< size_kvs[91]){ reducevector(kvdata91, vbuffer[91], 0, upperlimits[91], sweepparams, globalparams); }
			if(i< size_kvs[92]){ reducevector(kvdata92, vbuffer[92], 0, upperlimits[92], sweepparams, globalparams); }
			if(i< size_kvs[93]){ reducevector(kvdata93, vbuffer[93], 0, upperlimits[93], sweepparams, globalparams); }
			if(i< size_kvs[94]){ reducevector(kvdata94, vbuffer[94], 0, upperlimits[94], sweepparams, globalparams); }
			if(i< size_kvs[95]){ reducevector(kvdata95, vbuffer[95], 0, upperlimits[95], sweepparams, globalparams); }
			if(i< size_kvs[96]){ reducevector(kvdata96, vbuffer[96], 0, upperlimits[96], sweepparams, globalparams); }
			if(i< size_kvs[97]){ reducevector(kvdata97, vbuffer[97], 0, upperlimits[97], sweepparams, globalparams); }
			if(i< size_kvs[98]){ reducevector(kvdata98, vbuffer[98], 0, upperlimits[98], sweepparams, globalparams); }
			if(i< size_kvs[99]){ reducevector(kvdata99, vbuffer[99], 0, upperlimits[99], sweepparams, globalparams); }
			if(i< size_kvs[100]){ reducevector(kvdata100, vbuffer[100], 0, upperlimits[100], sweepparams, globalparams); }
			if(i< size_kvs[101]){ reducevector(kvdata101, vbuffer[101], 0, upperlimits[101], sweepparams, globalparams); }
			if(i< size_kvs[102]){ reducevector(kvdata102, vbuffer[102], 0, upperlimits[102], sweepparams, globalparams); }
			if(i< size_kvs[103]){ reducevector(kvdata103, vbuffer[103], 0, upperlimits[103], sweepparams, globalparams); }
			if(i< size_kvs[104]){ reducevector(kvdata104, vbuffer[104], 0, upperlimits[104], sweepparams, globalparams); }
			if(i< size_kvs[105]){ reducevector(kvdata105, vbuffer[105], 0, upperlimits[105], sweepparams, globalparams); }
			if(i< size_kvs[106]){ reducevector(kvdata106, vbuffer[106], 0, upperlimits[106], sweepparams, globalparams); }
			if(i< size_kvs[107]){ reducevector(kvdata107, vbuffer[107], 0, upperlimits[107], sweepparams, globalparams); }
			if(i< size_kvs[108]){ reducevector(kvdata108, vbuffer[108], 0, upperlimits[108], sweepparams, globalparams); }
			if(i< size_kvs[109]){ reducevector(kvdata109, vbuffer[109], 0, upperlimits[109], sweepparams, globalparams); }
			if(i< size_kvs[110]){ reducevector(kvdata110, vbuffer[110], 0, upperlimits[110], sweepparams, globalparams); }
			if(i< size_kvs[111]){ reducevector(kvdata111, vbuffer[111], 0, upperlimits[111], sweepparams, globalparams); }
			if(i< size_kvs[112]){ reducevector(kvdata112, vbuffer[112], 0, upperlimits[112], sweepparams, globalparams); }
			if(i< size_kvs[113]){ reducevector(kvdata113, vbuffer[113], 0, upperlimits[113], sweepparams, globalparams); }
			if(i< size_kvs[114]){ reducevector(kvdata114, vbuffer[114], 0, upperlimits[114], sweepparams, globalparams); }
			if(i< size_kvs[115]){ reducevector(kvdata115, vbuffer[115], 0, upperlimits[115], sweepparams, globalparams); }
			if(i< size_kvs[116]){ reducevector(kvdata116, vbuffer[116], 0, upperlimits[116], sweepparams, globalparams); }
			if(i< size_kvs[117]){ reducevector(kvdata117, vbuffer[117], 0, upperlimits[117], sweepparams, globalparams); }
			if(i< size_kvs[118]){ reducevector(kvdata118, vbuffer[118], 0, upperlimits[118], sweepparams, globalparams); }
			if(i< size_kvs[119]){ reducevector(kvdata119, vbuffer[119], 0, upperlimits[119], sweepparams, globalparams); }
			if(i< size_kvs[120]){ reducevector(kvdata120, vbuffer[120], 0, upperlimits[120], sweepparams, globalparams); }
			if(i< size_kvs[121]){ reducevector(kvdata121, vbuffer[121], 0, upperlimits[121], sweepparams, globalparams); }
			if(i< size_kvs[122]){ reducevector(kvdata122, vbuffer[122], 0, upperlimits[122], sweepparams, globalparams); }
			if(i< size_kvs[123]){ reducevector(kvdata123, vbuffer[123], 0, upperlimits[123], sweepparams, globalparams); }
			if(i< size_kvs[124]){ reducevector(kvdata124, vbuffer[124], 0, upperlimits[124], sweepparams, globalparams); }
			if(i< size_kvs[125]){ reducevector(kvdata125, vbuffer[125], 0, upperlimits[125], sweepparams, globalparams); }
			if(i< size_kvs[126]){ reducevector(kvdata126, vbuffer[126], 0, upperlimits[126], sweepparams, globalparams); }
			if(i< size_kvs[127]){ reducevector(kvdata127, vbuffer[127], 0, upperlimits[127], sweepparams, globalparams); }
			if(i< size_kvs[128]){ reducevector(kvdata128, vbuffer[128], 0, upperlimits[128], sweepparams, globalparams); }
			if(i< size_kvs[129]){ reducevector(kvdata129, vbuffer[129], 0, upperlimits[129], sweepparams, globalparams); }
			if(i< size_kvs[130]){ reducevector(kvdata130, vbuffer[130], 0, upperlimits[130], sweepparams, globalparams); }
			if(i< size_kvs[131]){ reducevector(kvdata131, vbuffer[131], 0, upperlimits[131], sweepparams, globalparams); }
			if(i< size_kvs[132]){ reducevector(kvdata132, vbuffer[132], 0, upperlimits[132], sweepparams, globalparams); }
			if(i< size_kvs[133]){ reducevector(kvdata133, vbuffer[133], 0, upperlimits[133], sweepparams, globalparams); }
			if(i< size_kvs[134]){ reducevector(kvdata134, vbuffer[134], 0, upperlimits[134], sweepparams, globalparams); }
			if(i< size_kvs[135]){ reducevector(kvdata135, vbuffer[135], 0, upperlimits[135], sweepparams, globalparams); }
			if(i< size_kvs[136]){ reducevector(kvdata136, vbuffer[136], 0, upperlimits[136], sweepparams, globalparams); }
			if(i< size_kvs[137]){ reducevector(kvdata137, vbuffer[137], 0, upperlimits[137], sweepparams, globalparams); }
			if(i< size_kvs[138]){ reducevector(kvdata138, vbuffer[138], 0, upperlimits[138], sweepparams, globalparams); }
			if(i< size_kvs[139]){ reducevector(kvdata139, vbuffer[139], 0, upperlimits[139], sweepparams, globalparams); }
			if(i< size_kvs[140]){ reducevector(kvdata140, vbuffer[140], 0, upperlimits[140], sweepparams, globalparams); }
			if(i< size_kvs[141]){ reducevector(kvdata141, vbuffer[141], 0, upperlimits[141], sweepparams, globalparams); }
			if(i< size_kvs[142]){ reducevector(kvdata142, vbuffer[142], 0, upperlimits[142], sweepparams, globalparams); }
			if(i< size_kvs[143]){ reducevector(kvdata143, vbuffer[143], 0, upperlimits[143], sweepparams, globalparams); }
			if(i< size_kvs[144]){ reducevector(kvdata144, vbuffer[144], 0, upperlimits[144], sweepparams, globalparams); }
			if(i< size_kvs[145]){ reducevector(kvdata145, vbuffer[145], 0, upperlimits[145], sweepparams, globalparams); }
			if(i< size_kvs[146]){ reducevector(kvdata146, vbuffer[146], 0, upperlimits[146], sweepparams, globalparams); }
			if(i< size_kvs[147]){ reducevector(kvdata147, vbuffer[147], 0, upperlimits[147], sweepparams, globalparams); }
			if(i< size_kvs[148]){ reducevector(kvdata148, vbuffer[148], 0, upperlimits[148], sweepparams, globalparams); }
			if(i< size_kvs[149]){ reducevector(kvdata149, vbuffer[149], 0, upperlimits[149], sweepparams, globalparams); }
			if(i< size_kvs[150]){ reducevector(kvdata150, vbuffer[150], 0, upperlimits[150], sweepparams, globalparams); }
			if(i< size_kvs[151]){ reducevector(kvdata151, vbuffer[151], 0, upperlimits[151], sweepparams, globalparams); }
			if(i< size_kvs[152]){ reducevector(kvdata152, vbuffer[152], 0, upperlimits[152], sweepparams, globalparams); }
			if(i< size_kvs[153]){ reducevector(kvdata153, vbuffer[153], 0, upperlimits[153], sweepparams, globalparams); }
			if(i< size_kvs[154]){ reducevector(kvdata154, vbuffer[154], 0, upperlimits[154], sweepparams, globalparams); }
			if(i< size_kvs[155]){ reducevector(kvdata155, vbuffer[155], 0, upperlimits[155], sweepparams, globalparams); }
			if(i< size_kvs[156]){ reducevector(kvdata156, vbuffer[156], 0, upperlimits[156], sweepparams, globalparams); }
			if(i< size_kvs[157]){ reducevector(kvdata157, vbuffer[157], 0, upperlimits[157], sweepparams, globalparams); }
			if(i< size_kvs[158]){ reducevector(kvdata158, vbuffer[158], 0, upperlimits[158], sweepparams, globalparams); }
			if(i< size_kvs[159]){ reducevector(kvdata159, vbuffer[159], 0, upperlimits[159], sweepparams, globalparams); }
			if(i< size_kvs[160]){ reducevector(kvdata160, vbuffer[160], 0, upperlimits[160], sweepparams, globalparams); }
			if(i< size_kvs[161]){ reducevector(kvdata161, vbuffer[161], 0, upperlimits[161], sweepparams, globalparams); }
			if(i< size_kvs[162]){ reducevector(kvdata162, vbuffer[162], 0, upperlimits[162], sweepparams, globalparams); }
			if(i< size_kvs[163]){ reducevector(kvdata163, vbuffer[163], 0, upperlimits[163], sweepparams, globalparams); }
			if(i< size_kvs[164]){ reducevector(kvdata164, vbuffer[164], 0, upperlimits[164], sweepparams, globalparams); }
			if(i< size_kvs[165]){ reducevector(kvdata165, vbuffer[165], 0, upperlimits[165], sweepparams, globalparams); }
			if(i< size_kvs[166]){ reducevector(kvdata166, vbuffer[166], 0, upperlimits[166], sweepparams, globalparams); }
			if(i< size_kvs[167]){ reducevector(kvdata167, vbuffer[167], 0, upperlimits[167], sweepparams, globalparams); }
			if(i< size_kvs[168]){ reducevector(kvdata168, vbuffer[168], 0, upperlimits[168], sweepparams, globalparams); }
			if(i< size_kvs[169]){ reducevector(kvdata169, vbuffer[169], 0, upperlimits[169], sweepparams, globalparams); }
			if(i< size_kvs[170]){ reducevector(kvdata170, vbuffer[170], 0, upperlimits[170], sweepparams, globalparams); }
			if(i< size_kvs[171]){ reducevector(kvdata171, vbuffer[171], 0, upperlimits[171], sweepparams, globalparams); }
			if(i< size_kvs[172]){ reducevector(kvdata172, vbuffer[172], 0, upperlimits[172], sweepparams, globalparams); }
			if(i< size_kvs[173]){ reducevector(kvdata173, vbuffer[173], 0, upperlimits[173], sweepparams, globalparams); }
			if(i< size_kvs[174]){ reducevector(kvdata174, vbuffer[174], 0, upperlimits[174], sweepparams, globalparams); }
			if(i< size_kvs[175]){ reducevector(kvdata175, vbuffer[175], 0, upperlimits[175], sweepparams, globalparams); }
			if(i< size_kvs[176]){ reducevector(kvdata176, vbuffer[176], 0, upperlimits[176], sweepparams, globalparams); }
			if(i< size_kvs[177]){ reducevector(kvdata177, vbuffer[177], 0, upperlimits[177], sweepparams, globalparams); }
			if(i< size_kvs[178]){ reducevector(kvdata178, vbuffer[178], 0, upperlimits[178], sweepparams, globalparams); }
			if(i< size_kvs[179]){ reducevector(kvdata179, vbuffer[179], 0, upperlimits[179], sweepparams, globalparams); }
			if(i< size_kvs[180]){ reducevector(kvdata180, vbuffer[180], 0, upperlimits[180], sweepparams, globalparams); }
			if(i< size_kvs[181]){ reducevector(kvdata181, vbuffer[181], 0, upperlimits[181], sweepparams, globalparams); }
			if(i< size_kvs[182]){ reducevector(kvdata182, vbuffer[182], 0, upperlimits[182], sweepparams, globalparams); }
			if(i< size_kvs[183]){ reducevector(kvdata183, vbuffer[183], 0, upperlimits[183], sweepparams, globalparams); }
			if(i< size_kvs[184]){ reducevector(kvdata184, vbuffer[184], 0, upperlimits[184], sweepparams, globalparams); }
			if(i< size_kvs[185]){ reducevector(kvdata185, vbuffer[185], 0, upperlimits[185], sweepparams, globalparams); }
			if(i< size_kvs[186]){ reducevector(kvdata186, vbuffer[186], 0, upperlimits[186], sweepparams, globalparams); }
			if(i< size_kvs[187]){ reducevector(kvdata187, vbuffer[187], 0, upperlimits[187], sweepparams, globalparams); }
			if(i< size_kvs[188]){ reducevector(kvdata188, vbuffer[188], 0, upperlimits[188], sweepparams, globalparams); }
			if(i< size_kvs[189]){ reducevector(kvdata189, vbuffer[189], 0, upperlimits[189], sweepparams, globalparams); }
			if(i< size_kvs[190]){ reducevector(kvdata190, vbuffer[190], 0, upperlimits[190], sweepparams, globalparams); }
			if(i< size_kvs[191]){ reducevector(kvdata191, vbuffer[191], 0, upperlimits[191], sweepparams, globalparams); }
			if(i< size_kvs[192]){ reducevector(kvdata192, vbuffer[192], 0, upperlimits[192], sweepparams, globalparams); }
			if(i< size_kvs[193]){ reducevector(kvdata193, vbuffer[193], 0, upperlimits[193], sweepparams, globalparams); }
			if(i< size_kvs[194]){ reducevector(kvdata194, vbuffer[194], 0, upperlimits[194], sweepparams, globalparams); }
			if(i< size_kvs[195]){ reducevector(kvdata195, vbuffer[195], 0, upperlimits[195], sweepparams, globalparams); }
			if(i< size_kvs[196]){ reducevector(kvdata196, vbuffer[196], 0, upperlimits[196], sweepparams, globalparams); }
			if(i< size_kvs[197]){ reducevector(kvdata197, vbuffer[197], 0, upperlimits[197], sweepparams, globalparams); }
			if(i< size_kvs[198]){ reducevector(kvdata198, vbuffer[198], 0, upperlimits[198], sweepparams, globalparams); }
			if(i< size_kvs[199]){ reducevector(kvdata199, vbuffer[199], 0, upperlimits[199], sweepparams, globalparams); }
			if(i< size_kvs[200]){ reducevector(kvdata200, vbuffer[200], 0, upperlimits[200], sweepparams, globalparams); }
			if(i< size_kvs[201]){ reducevector(kvdata201, vbuffer[201], 0, upperlimits[201], sweepparams, globalparams); }
			if(i< size_kvs[202]){ reducevector(kvdata202, vbuffer[202], 0, upperlimits[202], sweepparams, globalparams); }
			if(i< size_kvs[203]){ reducevector(kvdata203, vbuffer[203], 0, upperlimits[203], sweepparams, globalparams); }
			if(i< size_kvs[204]){ reducevector(kvdata204, vbuffer[204], 0, upperlimits[204], sweepparams, globalparams); }
			if(i< size_kvs[205]){ reducevector(kvdata205, vbuffer[205], 0, upperlimits[205], sweepparams, globalparams); }
			if(i< size_kvs[206]){ reducevector(kvdata206, vbuffer[206], 0, upperlimits[206], sweepparams, globalparams); }
			if(i< size_kvs[207]){ reducevector(kvdata207, vbuffer[207], 0, upperlimits[207], sweepparams, globalparams); }
			if(i< size_kvs[208]){ reducevector(kvdata208, vbuffer[208], 0, upperlimits[208], sweepparams, globalparams); }
			if(i< size_kvs[209]){ reducevector(kvdata209, vbuffer[209], 0, upperlimits[209], sweepparams, globalparams); }
			if(i< size_kvs[210]){ reducevector(kvdata210, vbuffer[210], 0, upperlimits[210], sweepparams, globalparams); }
			if(i< size_kvs[211]){ reducevector(kvdata211, vbuffer[211], 0, upperlimits[211], sweepparams, globalparams); }
			if(i< size_kvs[212]){ reducevector(kvdata212, vbuffer[212], 0, upperlimits[212], sweepparams, globalparams); }
			if(i< size_kvs[213]){ reducevector(kvdata213, vbuffer[213], 0, upperlimits[213], sweepparams, globalparams); }
			if(i< size_kvs[214]){ reducevector(kvdata214, vbuffer[214], 0, upperlimits[214], sweepparams, globalparams); }
			if(i< size_kvs[215]){ reducevector(kvdata215, vbuffer[215], 0, upperlimits[215], sweepparams, globalparams); }
			if(i< size_kvs[216]){ reducevector(kvdata216, vbuffer[216], 0, upperlimits[216], sweepparams, globalparams); }
			if(i< size_kvs[217]){ reducevector(kvdata217, vbuffer[217], 0, upperlimits[217], sweepparams, globalparams); }
			if(i< size_kvs[218]){ reducevector(kvdata218, vbuffer[218], 0, upperlimits[218], sweepparams, globalparams); }
			if(i< size_kvs[219]){ reducevector(kvdata219, vbuffer[219], 0, upperlimits[219], sweepparams, globalparams); }
			if(i< size_kvs[220]){ reducevector(kvdata220, vbuffer[220], 0, upperlimits[220], sweepparams, globalparams); }
			if(i< size_kvs[221]){ reducevector(kvdata221, vbuffer[221], 0, upperlimits[221], sweepparams, globalparams); }
			if(i< size_kvs[222]){ reducevector(kvdata222, vbuffer[222], 0, upperlimits[222], sweepparams, globalparams); }
			if(i< size_kvs[223]){ reducevector(kvdata223, vbuffer[223], 0, upperlimits[223], sweepparams, globalparams); }
			if(i< size_kvs[224]){ reducevector(kvdata224, vbuffer[224], 0, upperlimits[224], sweepparams, globalparams); }
			if(i< size_kvs[225]){ reducevector(kvdata225, vbuffer[225], 0, upperlimits[225], sweepparams, globalparams); }
			if(i< size_kvs[226]){ reducevector(kvdata226, vbuffer[226], 0, upperlimits[226], sweepparams, globalparams); }
			if(i< size_kvs[227]){ reducevector(kvdata227, vbuffer[227], 0, upperlimits[227], sweepparams, globalparams); }
			if(i< size_kvs[228]){ reducevector(kvdata228, vbuffer[228], 0, upperlimits[228], sweepparams, globalparams); }
			if(i< size_kvs[229]){ reducevector(kvdata229, vbuffer[229], 0, upperlimits[229], sweepparams, globalparams); }
			if(i< size_kvs[230]){ reducevector(kvdata230, vbuffer[230], 0, upperlimits[230], sweepparams, globalparams); }
			if(i< size_kvs[231]){ reducevector(kvdata231, vbuffer[231], 0, upperlimits[231], sweepparams, globalparams); }
			if(i< size_kvs[232]){ reducevector(kvdata232, vbuffer[232], 0, upperlimits[232], sweepparams, globalparams); }
			if(i< size_kvs[233]){ reducevector(kvdata233, vbuffer[233], 0, upperlimits[233], sweepparams, globalparams); }
			if(i< size_kvs[234]){ reducevector(kvdata234, vbuffer[234], 0, upperlimits[234], sweepparams, globalparams); }
			if(i< size_kvs[235]){ reducevector(kvdata235, vbuffer[235], 0, upperlimits[235], sweepparams, globalparams); }
			if(i< size_kvs[236]){ reducevector(kvdata236, vbuffer[236], 0, upperlimits[236], sweepparams, globalparams); }
			if(i< size_kvs[237]){ reducevector(kvdata237, vbuffer[237], 0, upperlimits[237], sweepparams, globalparams); }
			if(i< size_kvs[238]){ reducevector(kvdata238, vbuffer[238], 0, upperlimits[238], sweepparams, globalparams); }
			if(i< size_kvs[239]){ reducevector(kvdata239, vbuffer[239], 0, upperlimits[239], sweepparams, globalparams); }
			if(i< size_kvs[240]){ reducevector(kvdata240, vbuffer[240], 0, upperlimits[240], sweepparams, globalparams); }
			if(i< size_kvs[241]){ reducevector(kvdata241, vbuffer[241], 0, upperlimits[241], sweepparams, globalparams); }
			if(i< size_kvs[242]){ reducevector(kvdata242, vbuffer[242], 0, upperlimits[242], sweepparams, globalparams); }
			if(i< size_kvs[243]){ reducevector(kvdata243, vbuffer[243], 0, upperlimits[243], sweepparams, globalparams); }
			if(i< size_kvs[244]){ reducevector(kvdata244, vbuffer[244], 0, upperlimits[244], sweepparams, globalparams); }
			if(i< size_kvs[245]){ reducevector(kvdata245, vbuffer[245], 0, upperlimits[245], sweepparams, globalparams); }
			if(i< size_kvs[246]){ reducevector(kvdata246, vbuffer[246], 0, upperlimits[246], sweepparams, globalparams); }
			if(i< size_kvs[247]){ reducevector(kvdata247, vbuffer[247], 0, upperlimits[247], sweepparams, globalparams); }
			if(i< size_kvs[248]){ reducevector(kvdata248, vbuffer[248], 0, upperlimits[248], sweepparams, globalparams); }
			if(i< size_kvs[249]){ reducevector(kvdata249, vbuffer[249], 0, upperlimits[249], sweepparams, globalparams); }
			if(i< size_kvs[250]){ reducevector(kvdata250, vbuffer[250], 0, upperlimits[250], sweepparams, globalparams); }
			if(i< size_kvs[251]){ reducevector(kvdata251, vbuffer[251], 0, upperlimits[251], sweepparams, globalparams); }
			if(i< size_kvs[252]){ reducevector(kvdata252, vbuffer[252], 0, upperlimits[252], sweepparams, globalparams); }
			if(i< size_kvs[253]){ reducevector(kvdata253, vbuffer[253], 0, upperlimits[253], sweepparams, globalparams); }
			if(i< size_kvs[254]){ reducevector(kvdata254, vbuffer[254], 0, upperlimits[254], sweepparams, globalparams); }
			if(i< size_kvs[255]){ reducevector(kvdata255, vbuffer[255], 0, upperlimits[255], sweepparams, globalparams); }
		}
	}
	#endif 
	return;
}

// actsproc 
void 
	#ifdef SW 
	actsproc::
	#endif 
resetenvbuffers(keyvalue_capsule_t capsule_so1[VECTOR_SIZE][NUM_PARTITIONS], keyvalue_capsule_t capsule_so8[NUM_PARTITIONS]){
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

fetchmessage_t 
	#ifdef SW 
	actsproc::
	#endif 
fetchkeyvalues(bool_type enable, unsigned int mode, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask[BLOCKRAM_SIZE], uintNUMPby2_type vmask_subp[BLOCKRAM_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], 
		batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, sweepparams_t sweepparams, globalparams_t globalparams){
	fetchmessage_t fetchmessage;
	if(mode == PROCESSMODE){
		fetchmessage = readandprocess(enable, kvdram, vbuffer, vmask, vmask_subp, buffer, goffset_kvs, loffset_kvs, size_kvs, travstate, sweepparams, globalparams);
	} else {
		fetchmessage = readkeyvalues(enable, kvdram, buffer, goffset_kvs + loffset_kvs, size_kvs, travstate, globalparams); 
	}
	return fetchmessage; 
}

void 
	#ifdef SW 
	actsproc::
	#endif 
commitkeyvalues(bool_type enable1, bool_type enable2, unsigned int mode, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t globalcapsule[NUM_PARTITIONS], keyvalue_capsule_t localcapsule[NUM_PARTITIONS], 
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
	actsproc::
	#endif
actspipeline(bool_type enable1, bool_type enable2, keyvalue_buffer_t buffer_setof1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_capsule_t capsule_so1[VECTOR_SIZE][NUM_PARTITIONS], 
						keyvalue_buffer_t buffer_setof8[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_capsule_t capsule_so8[NUM_PARTITIONS],
							unsigned int currentLOP, sweepparams_t sweepparams, buffer_type cutoffs[VECTOR_SIZE], batch_type shiftcount, globalparams_t globalparams){		
	analysis_type analysis_loop1 = SRCBUFFER_SIZE / 4;
	if(enable1 == OFF){ return; }
	
	unsigned int upperlimit = sweepparams.upperlimit;
	unsigned int upperpartition = sweepparams.upperpartition;
	
	keyvalue_buffer_t kvA0[4];
	keyvalue_buffer_t kvA2[4];
	keyvalue_buffer_t kvA4[4];
	keyvalue_buffer_t kvA6[4];
	#pragma HLS ARRAY_PARTITION variable=kvA0 complete
	#pragma HLS ARRAY_PARTITION variable=kvA2 complete
	#pragma HLS ARRAY_PARTITION variable=kvA4 complete
	#pragma HLS ARRAY_PARTITION variable=kvA6 complete

	buffer_type tempcutoffs[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=tempcutoffs complete

	for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		capsule_so8[p].key = 0;
		capsule_so8[p].value = capsule_so1[0][p].value + capsule_so1[1][p].value 
									+ capsule_so1[2][p].value + capsule_so1[3][p].value 
									+ capsule_so1[4][p].value + capsule_so1[5][p].value 
									+ capsule_so1[6][p].value + capsule_so1[7][p].value;
	}
	calculateoffsets(capsule_so8, NUM_PARTITIONS);
	resetvalues(capsule_so8, NUM_PARTITIONS, 0);
	
	buffer_type maxcutoff = 0;
	for(vector_type v=0; v<VECTOR_SIZE; v++){ 
	#pragma HLS PIPELINE II=1
		if(cutoffs[v] > maxcutoff){ maxcutoff = cutoffs[v]; }
	}
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkfordivisibleby(ON, "actspipeline:maxcutoff", maxcutoff, 4);
	#endif
	
	keyvalue_t mydummykv;
	mydummykv.key = 0;
	mydummykv.value = GETV(INVALIDDATA);
	keyvalue_buffer_t dummykv = GETKV(mydummykv);
	
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
		RUNPIPELINE_LOOP1B: for(buffer_type k=0; k<maxcutoff; k+=4){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loop1 avg=analysis_loop1
		#pragma HLS PIPELINE II=4
		
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
			for(unsigned int m=0; m<4; m++){ cout<<"kvA0["<<m<<"].key: "<<GETKV(kvA0[m]).key<<", kvA0["<<m<<"].value: "<<GETKV(kvA0[m]).value<<endl; }
			for(unsigned int m=0; m<4; m++){ cout<<"kvA2["<<m<<"].key: "<<GETKV(kvA2[m]).key<<", kvA2["<<m<<"].value: "<<GETKV(kvA2[m]).value<<endl; }
			for(unsigned int m=0; m<4; m++){ cout<<"kvA4["<<m<<"].key: "<<GETKV(kvA4[m]).key<<", kvA4["<<m<<"].value: "<<GETKV(kvA4[m]).value<<endl; }
			for(unsigned int m=0; m<4; m++){ cout<<"kvA6["<<m<<"].key: "<<GETKV(kvA6[m]).key<<", kvA6["<<m<<"].value: "<<GETKV(kvA6[m]).value<<endl; }
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
			// if(!((kvA0[0].value == INVALIDDATA) && (kvA0[1].value == INVALIDDATA) && (kvA0[2].value == INVALIDDATA) && (kvA0[3].value == INVALIDDATA))){ tempbufferDcapsule[pA0] += 4; }
			if(GETKV(kvA0[0]).value != GETV(INVALIDDATA)){ tempbufferDcapsule[pA0] += 4; } // ERROR CHECKPOINT.
			
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
			// if(!((kvA2[0].value == INVALIDDATA) && (kvA2[1].value == INVALIDDATA) && (kvA2[2].value == INVALIDDATA) && (kvA2[3].value == INVALIDDATA))){ tempbufferDcapsule[pA2] += 4; }
			if(GETKV(kvA2[0]).value != GETV(INVALIDDATA)){ tempbufferDcapsule[pA2] += 4; } // ERROR CHECKPOINT.
			
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
			// if(!((kvA4[0].value == INVALIDDATA) && (kvA4[1].value == INVALIDDATA) && (kvA4[2].value == INVALIDDATA) && (kvA4[3].value == INVALIDDATA))){ tempbufferDcapsule[pA4] += 4; }
			if(GETKV(kvA4[0]).value != GETV(INVALIDDATA)){ tempbufferDcapsule[pA4] += 4; } // ERROR CHECKPOINT.
			
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
			// if(!((kvA6[0].value == INVALIDDATA) && (kvA6[1].value == INVALIDDATA) && (kvA6[2].value == INVALIDDATA) && (kvA6[3].value == INVALIDDATA))){ tempbufferDcapsule[pA6] += 4; }
			if(GETKV(kvA6[0]).value != GETV(INVALIDDATA)){ tempbufferDcapsule[pA6] += 4; } // ERROR CHECKPOINT.
		}
	}
	for(partition_type p=0; p<NUM_PARTITIONS; p++){ 
	#pragma HLS PIPELINE II=1
		capsule_so8[p].value = tempbufferDcapsule[p]; 
	}
	
	for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		keyvalue_t mydummykv;
		mydummykv.key = p;
		mydummykv.value = GETV(INVALIDDATA);
		keyvalue_buffer_t dummykv = GETKV(mydummykv);
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
	actsproc::
	#endif
actit(bool_type enable, unsigned int mode,
		uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask[BLOCKRAM_SIZE], uintNUMPby2_type vmask_subp[BLOCKRAM_SIZE], keyvalue_t globalstatsbuffer[NUM_PARTITIONS], 
		globalparams_t globalparams, sweepparams_t sweepparams, travstate_t ptravstate, batch_type sourcebaseaddr_kvs, batch_type destbaseaddr_kvs,
		bool_type resetenv, bool_type flush){
	analysis_type analysis_partitionloop = MODEL_BATCHSIZE_KVS / (NUMPIPELINES_PARTITIONUPDATES * WORKBUFFER_SIZE);
	if(enable == OFF){ return; }
	
static keyvalue_buffer_t buffer_setof1[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = buffer_setof1
static keyvalue_buffer_t buffer_setof8[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = buffer_setof8
	
static keyvalue_capsule_t capsule_so1[VECTOR_SIZE][NUM_PARTITIONS];
	#pragma HLS array_partition variable = capsule_so1
static keyvalue_capsule_t capsule_so8[NUM_PARTITIONS];
	
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
	
	fetchmessage_t fetchmessagepp0;
	fetchmessage_t fetchmessagepp1;
	fetchmessagepp0.chunksize_kvs = -1; fetchmessagepp0.nextoffset_kvs = -1;
	fetchmessagepp1.chunksize_kvs = -1; fetchmessagepp1.nextoffset_kvs = -1;
	
	if(resetenv == ON){ resetenvbuffers(capsule_so1, capsule_so8); } else { itercount = 64; } // FIXME. '64' is just some number greater than 2
	if(flush == ON){ flushsz = 1*SRCBUFFER_SIZE; } else { flushsz = 0; }
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(resetenv == ON){ cout<<"actit: reset is ON"<<endl; } else { cout<<"actit: reset is OFF"<<endl;  }
	if(flush == ON){ cout<<"actit: flush is ON"<<endl; } else { cout<<"actit: flush is OFF"<<endl;  }
	#endif 
	
	batch_type offset_kvs = ptravstate.begin_kvs;
	ACTIT_MAINLOOP: while(offset_kvs < ptravstate.end_kvs + flushsz){ 
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_partitionloop avg=analysis_partitionloop
		#ifdef PUP1
		if(itercount >= 0){ pp0writeen = ON; } else { pp0writeen = OFF; }
		if(itercount >= 1){ pp1writeen = ON; } else { pp1writeen = OFF; }
		if(itercount >= 1){ pp1runpipelineen = ON; } else { pp1runpipelineen = OFF; }
		#else 
		if(itercount >= 0){ pp0writeen = ON; } else { pp0writeen = OFF; }
		#endif
		
		ptravstatepp0.i_kvs = offset_kvs;
		fetchmessagepp0 = fetchkeyvalues(ON, mode, kvdram, vbuffer, vmask, vmask_subp, sourcebuffer, sourcebaseaddr_kvs, ptravstatepp0.i_kvs, WORKBUFFER_SIZE, ptravstatepp0, sweepparams, globalparams);
		if(mode == PROCESSMODE && fetchmessagepp0.nextoffset_kvs != -1){ offset_kvs = fetchmessagepp0.nextoffset_kvs; } else { offset_kvs+=WORKBUFFER_SIZE; } 
		#ifdef PUP1
		actspipeline(pp1runpipelineen, ON, buffer_setof1, capsule_so1, buffer_setof8, capsule_so8, sweepparams.currentLOP, sweepparams, pp1cutoffs, (itercount-2)+1, globalparams);
		#endif 
		
		preparekeyvalues(ON, ON, sourcebuffer, buffer_setof1, capsule_so1, sweepparams.currentLOP, sweepparams, fetchmessagepp0.chunksize_kvs, pp0cutoffs, globalparams);
		#ifdef PUP1
		commitkeyvalues(pp1writeen, ON, mode, kvdram, vbuffer, buffer_setof8, globalstatsbuffer, capsule_so8, destbaseaddr_kvs, sweepparams, globalparams); 
		#endif 
		
		actspipeline(ON, ON, buffer_setof1, capsule_so1, buffer_setof8, capsule_so8, sweepparams.currentLOP, sweepparams, pp0cutoffs, itercount, globalparams);
		#ifdef PUP1
		ptravstatepp1.i_kvs = offset_kvs;
		fetchmessagepp1 = fetchkeyvalues(ON, mode, kvdram, vbuffer, vmask, vmask_subp, sourcebuffer, sourcebaseaddr_kvs, ptravstatepp1.i_kvs, WORKBUFFER_SIZE, ptravstatepp1, sweepparams, globalparams);
		if(mode == PROCESSMODE && fetchmessagepp1.nextoffset_kvs != -1){ offset_kvs = fetchmessagepp1.nextoffset_kvs; } else { offset_kvs+=WORKBUFFER_SIZE; } 
		#endif
		
		commitkeyvalues(pp0writeen, ON, mode, kvdram, vbuffer, buffer_setof8, globalstatsbuffer, capsule_so8, destbaseaddr_kvs, sweepparams, globalparams); 
		#ifdef PUP1
		preparekeyvalues(pp1partitionen, ON, sourcebuffer, buffer_setof1, capsule_so1, sweepparams.currentLOP, sweepparams, fetchmessagepp1.chunksize_kvs, pp1cutoffs, globalparams);
		#endif
		
		itercount += NUMPIPELINES_PARTITIONUPDATES;
	}
	return;
}

// dispatch
void
	#ifdef SW
	actsproc::
	#endif 
processit(uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask[BLOCKRAM_SIZE], uintNUMPby2_type vmask_subp[BLOCKRAM_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], globalparams_t globalparams){
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
	avtravstate.end_kvs = avtravstate.begin_kvs + (globalparams.ACTSPARAMS_SRCVSIZE / VECTOR2_SIZE); avtravstate.size_kvs = globalparams.ACTSPARAMS_SRCVSIZE / VECTOR2_SIZE;
	readglobalstats(ON, kvdram, globalstatsbuffer, globalparams.BASEOFFSETKVS_STATSDRAM + deststatsmarker, globalparams); 
	resetvalues(globalstatsbuffer, NUM_PARTITIONS, 0);
	
	batch_type vptrbaseoffset_kvs = globalparams.BASEOFFSETKVS_VERTEXPTR + (globalparams.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE);
	batch_type vdatabaseoffset_kvs = globalparams.BASEOFFSETKVS_VERTICESDATA + (globalparams.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE);
	
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
		actsutilityobj->print5("### processit:: source_partition", "voffset", "vbegin", "vend", "vskip", source_partition, voffset_kvs * VECTOR_SIZE, avtravstate.begin_kvs * VECTOR_SIZE, avtravstate.size_kvs * VECTOR_SIZE, SRCBUFFER_SIZE * VECTOR_SIZE);
		#endif
		
		loadvmasks(ON, kvdram, vmask, vbuffer, globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskoffset_kvs, vmaskbuffersz_kvs, globalparams); // NOTE: this should come before loadvdata because vbuffer is used as a temp buffer
		#ifndef ALLVERTEXISACTIVE_ALGORITHM
		#ifdef SUBPMASK
		loadvmasks_subp(ON, vmask, vmask_subp, vmaskbuffersz_kvs, globalparams);
		#endif 
		#endif 
		readvdata(ON, kvdram, vdatabaseoffset_kvs + voffset_kvs, vbuffer, 0, 0, reducebuffersz, globalparams);
		readvdata(ON, kvdram, vdatabaseoffset_kvs + voffset_kvs + reducebuffersz, vbuffer, 8, 0, reducebuffersz, globalparams);
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
		
		keyy_t localbeginvptr = beginvptr;
		keyy_t localendvptr = endvptr;
		keyy_t numedges = localendvptr - localbeginvptr + 2*VECTOR_SIZE;
		if(localbeginvptr == localendvptr){ numedges = 0; }
		keyy_t localbeginvptr_kvs = localbeginvptr / VECTOR_SIZE; // NOTE: this should be in KVS(8) terms
		keyy_t numedges_kvs = numedges / VECTOR_SIZE; // NB: this is correct.
		#ifdef _DEBUGMODE_CHECKS2
		if(localendvptr < localbeginvptr){ cout<<"processit::ERROR: localendvptr < localbeginvptr. localbeginvptr: "<<localbeginvptr<<", localendvptr: "<<localendvptr<<endl; exit(EXIT_FAILURE); }
		if(localendvptr < globalparams.SIZE_EDGES){ actsutilityobj->checkptr("processit", beginsrcvid, endsrcvid, localbeginvptr, localendvptr, (keyvalue_t *)&kvdram[globalparams.BASEOFFSETKVS_EDGESDATA]); }
		#endif
		
		#ifdef _DEBUGMODE_KERNELPRINTS
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
			kvdram, sourcebuffer, vbuffer, vmask, vmask_subp, globalstatsbuffer, 
			globalparams, sweepparams, etravstate, globalparams.BASEOFFSETKVS_EDGESDATA, globalparams.BASEOFFSETKVS_KVDRAMWORKSPACE,
			resetenv, flush);

		#ifdef _DEBUGMODE_KERNELPRINTS
		actsutilityobj->printglobalvars();
		actsutilityobj->clearglobalvars();
		#endif
	}
	saveglobalstats(ON, kvdram, globalstatsbuffer, globalparams.BASEOFFSETKVS_STATSDRAM + deststatsmarker, globalparams);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	actsutilityobj->printglobalvars();
	#endif 
	#if defined(_DEBUGMODE_KERNELPRINTS3) || defined(_DEBUGMODE_CHECKS2)
	actsutilityobj->clearglobalvars();
	#endif
	return;
}

void
	#ifdef SW 
	actsproc::
	#endif 
partitionit(uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask[BLOCKRAM_SIZE], uintNUMPby2_type vmask_subp[BLOCKRAM_SIZE], globalparams_t globalparams){
	#pragma HLS INLINE
	analysis_type analysis_numllops = 1;
	analysis_type analysis_numsourcepartitions = 1;
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printparameters();
	actsutilityobj->printglobalvars();
	actsutilityobj->printglobalparameters("actsproc::getglobalparams:: printing global parameters", globalparams);
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
					kvdram, sourcebuffer, vbuffer, vmask, vmask_subp, globalstatsbuffer,
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
		#ifdef _DEBUGMODE_KERNELPRINTS3
		actsutilityobj->printglobalvars();
		#endif 
		#if defined(_DEBUGMODE_KERNELPRINTS3) || defined(_DEBUGMODE_CHECKS2)
		actsutilityobj->clearglobalvars();
		#endif
	}	
	return;
}

void
	#ifdef SW 
	actsproc::
	#endif 
reduceit(uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask[BLOCKRAM_SIZE], uintNUMPby2_type vmask_subp[BLOCKRAM_SIZE], batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparams){	
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
			kvdram, sourcebuffer, vbuffer, vmask, vmask_subp, globalstatsbuffer,
			globalparams, sweepparams, ptravstate, sweepparams.worksourcebaseaddress_kvs, sweepparams.workdestbaseaddress_kvs,
			ON, ON);
	return;
}

void
	#ifdef SW 
	actsproc::
	#endif 
dispatch(bool_type en_process, bool_type en_partition, bool_type en_reduce, uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask[BLOCKRAM_SIZE], uintNUMPby2_type vmask_subp[BLOCKRAM_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE],
			batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparams){
	if(en_process == ON){ processit(kvdram, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, globalparams); } 
	if(en_partition == ON){ partitionit(kvdram, sourcebuffer, vbuffer, vmask, vmask_subp, globalparams); } 
	if(en_reduce == ON){ reduceit(kvdram, sourcebuffer, vbuffer, vmask, vmask_subp, sourcestatsmarker, source_partition, globalparams); } 
	return;
}

// top
void
	#ifdef SW 
	actsproc::
	#endif 
dispatch_reduce(uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask[BLOCKRAM_SIZE], uintNUMPby2_type vmask_subp[BLOCKRAM_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], globalparams_t globalparams){
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
	
	unsigned int vreadoffset_kvs = 0;
	buffer_type reducebuffersz = globalparams.SIZE_REDUCE / 2;
	
	step_type currentLOP = globalparams.ACTSPARAMS_TREEDEPTH;
	batch_type num_source_partitions = get_num_source_partitions(currentLOP);

	for(unsigned int k=0; k<num_source_partitions; k++){ vmask_p[k] = 0; } // vmask_subp[k] = 0; 
	
	bool_type enablereduce = ON;
	
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
		readvdata(enablereduce, kvdram, globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer, 0, 0, reducebuffersz, globalparams);
		readvdata(enablereduce, kvdram, globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer, 8, 0, reducebuffersz, globalparams);
		
		// reduce
		dispatch(OFF, OFF, enablereduce, kvdram, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, sourcestatsmarker, source_partition, globalparams);
		
		// writeback vertices
		savevdata(enablereduce, kvdram, globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer, 0, 0, reducebuffersz, globalparams);
		savevdata(enablereduce, kvdram, globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer, 8, 0, reducebuffersz, globalparams);
		
		sourcestatsmarker += 1;
		vreadoffset_kvs += reducebuffersz * 2;
	}
	return;
} 

extern "C" {
void 
	#ifdef SW 
	actsproc:: 
	#endif
topkernelproc(uint512_dt * kvdram){
#pragma HLS INTERFACE m_axi port = kvdram offset = slave bundle = gmem0			
		
#pragma HLS INTERFACE s_axilite port = kvdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATA_PACK variable = kvdram

	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printparameters();
	#endif
	#if defined(_DEBUGMODE_KERNELPRINTS) || defined(ALLVERTEXISACTIVE_ALGORITHM) // CRITICAL REMOVEME
	cout<<">>> ====================== Light weight ACTS (NACTS_IN_NCOMPUTEUNITS.PPR) Launched... size: "<<GETKEYENTRY(kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN], 0)<<endl; 
	#endif
	
	keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = sourcebuffer
	keyvalue_vbuffer_t vbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer
	uintNUMPby2_type vmask[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask
	uintNUMPby2_type vmask_subp[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask_subp
	uint32_type vmask_p[BLOCKRAM_SIZE];
	
	globalparams_t globalparams = getglobalparams(kvdram);
	
	#ifndef ALLVERTEXISACTIVE_ALGORITHM
	loadvmask_p(kvdram, vmask_p, globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE);
	#endif 
	
	#ifndef ALLVERTEXISACTIVE_ALGORITHM
	resetkvdramstats(kvdram, globalparams);
	#endif
	
	// process & partition
	#ifdef PROCESSMODULE
	if(globalparams.ENABLE_PROCESSCOMMAND == ON){ 
		#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(ALLVERTEXISACTIVE_ALGORITHM)
		cout<<"topkernelproc: processing instance ... "<<endl;
		#endif
		dispatch(ON, OFF, OFF, kvdram, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, NAp, NAp, globalparams);
	}
	#endif
	
	// partition
	#ifdef PARTITIONMODULE
	if(globalparams.ENABLE_PARTITIONCOMMAND == ON){ 
		#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(ALLVERTEXISACTIVE_ALGORITHM)
		cout<<"topkernelproc: partitioning instance ... "<<endl;
		#endif
		dispatch(OFF, ON, OFF, kvdram, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, NAp, NAp, globalparams);
	}
	#endif
	
	// reduce & partition
	#ifdef REDUCEMODULE
	if(globalparams.ENABLE_APPLYUPDATESCOMMAND == ON){ 
		#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(ALLVERTEXISACTIVE_ALGORITHM)
		cout<<"topkernelproc: reducing instance ... "<<endl;
		#endif
		dispatch_reduce(kvdram, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, globalparams);
	}
	#endif

	#ifdef _DEBUGMODE_KERNELPRINTS3
	actsutilityobj->printglobalvars();
	#endif 
	#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(_DEBUGMODE_CHECKS2)
	actsutilityobj->clearglobalvars();
	#endif
	return;
}
}









