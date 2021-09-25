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
// #ifndef FPGA_IMPL
#include "actsproc.h"
#include "actssync.h"
// #endif 
using namespace std;

#ifdef SW
acts::acts(){}
acts::~acts(){}
#endif
#ifdef SW
actsproc actsprocobj;
actssync actssyncobj;
#endif 

// INCLUDE PROC & SYNC FILE ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define INCLUDEPROC
#define INCLUDESYNC
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// INCLUDE PROC FILE ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined(FPGA_IMPL) & defined(INCLUDEPROC)
#ifdef ACTS_PARTITION_AND_REDUCE_STRETEGY
#define NUMPIPELINES_PARTITIONUPDATES 2 // REMOVEME.
#else 
#define NUMPIPELINES_PARTITIONUPDATES 2 //1 // REMOVEME.
#endif 
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
GETVTXDATA(keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int loc, globalparams_t globalparams){
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
GETVTXMASK(unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unsigned int loc, globalparams_t globalparams){
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
GETVTXMASK_SUBP(unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unsigned int loc, globalparams_t globalparams){
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
	
	#ifdef ENABLE_APPROXIMATEPARTITIONWRITES // DESTBLOCKRAM_SIZE
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
		
		else if(realsize_kvs == DESTBLOCKRAM_SIZE){ size_kvs = DESTBLOCKRAM_SIZE; }
		
		else {
			cout<<"WARNING:getpartitionwritesz: should not get here. something might be wrong. realsize_kvs: "<<realsize_kvs<<", size_kvs: "<<size_kvs<<", DESTBLOCKRAM_SIZE: "<<DESTBLOCKRAM_SIZE<<endl;				 
			
			#ifdef ENABLE_PERFECTACCURACY
			#ifdef _DEBUGMODE_CHECKS2
			cout<<"WARNING:getpartitionwritesz: should not get here. something might be wrong. realsize_kvs: "<<realsize_kvs<<", size_kvs: "<<size_kvs<<", DESTBLOCKRAM_SIZE: "<<DESTBLOCKRAM_SIZE<<endl;				 
			exit(EXIT_FAILURE);
			#endif
			#else
			size_kvs = DESTBLOCKRAM_SIZE / NUM_PARTITIONS;
			#endif 
		}
		#if defined(SW) || defined(SWEMU) || defined(HW) 
		if((bramoffset_kvs + size_kvs) >= DESTBLOCKRAM_SIZE){ size_kvs = DESTBLOCKRAM_SIZE - bramoffset_kvs - 1; } 
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
readkeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs, travstate_t travstate, globalparams_t globalparams){
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
	
	// cout<<"-------------------- readkeyvalues II "<<endl; // REMOVEME.
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"readkeyvalues:: keyvalues read: offset_kvs: "<<offset_kvs<<", size_kvs: "<<size_kvs<<", chunk_size: "<<chunk_size<<endl;
	#endif
	return fetchmessage;
}

void 
	#ifdef SW 
	actsproc::
	#endif 
savekeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_buffer_t buffer[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_t * globalcapsule, keyvalue_capsule_t localcapsule[NUM_PARTITIONS], batch_type globalbaseaddress_kvs, globalparams_t globalparams){				
	if(enable == OFF){ return; }
	
	#ifdef _DEBUGMODE_CHECKS
	actsutilityobj->printkeyvalues("savekeyvalues::localcapsule", localcapsule, NUM_PARTITIONS);
	actsutilityobj->printvaluecount("savekeyvalues::localcapsule", localcapsule, NUM_PARTITIONS);
	actsutilityobj->scankeyvalues("savekeyvalues::buffer", (keyvalue_t *)buffer, localcapsule, NUM_PARTITIONS, globalparams.SIZE_BATCHRANGE / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	
	analysis_type analysis_destpartitionsz = DESTBLOCKRAM_SIZE / NUM_PARTITIONS;
	SAVEPARTITIONS_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		batch_type dramoffset_kvs = globalbaseaddress_kvs + ((globalcapsule[p].key + globalcapsule[p].value) / VECTOR_SIZE);
		buffer_type bramoffset_kvs = localcapsule[p].key / VECTOR_SIZE;
		buffer_type realsize_kvs = localcapsule[p].value / VECTOR_SIZE;
		buffer_type size_kvs = getpartitionwritesz(realsize_kvs, bramoffset_kvs);
		
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("savekeyvalues 23", bramoffset_kvs + size_kvs, DESTBLOCKRAM_SIZE + 1, p, NAp, NAp);
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
readvdata(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams){
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
	if(dramoffset_kvs >= globalparams.BASEOFFSETKVS_VERTEXPTR_ && dramoffset_kvs < globalparams.BASEOFFSETKVS_VERTEXPTR + VERTEXPTRSSZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"readvdata:: vertices read: offset: "<<(dramoffset_kvs - globalparams.BASEOFFSETKVS_VERTEXPTR) * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs - globalparams.BASEOFFSETKVS_VERTEXPTR) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	if(dramoffset_kvs >= globalparams.BASEOFFSETKVS_VERTICESDATA_ && dramoffset_kvs < globalparams.BASEOFFSETKVS_VERTICESDATA + VERTICESDATASZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"readvdata:: vertices read: offset: "<<(dramoffset_kvs - globalparams.BASEOFFSETKVS_VERTICESDATA) * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs - globalparams.BASEOFFSETKVS_VERTICESDATA) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	#endif
	return;
}

void 
	#ifdef SW 
	actsproc::
	#endif 
savevdata(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams){
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
loadvmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams){
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

	unitBRAMwidth_type tempvmask;
	#pragma HLS DATA_PACK variable = tempvmask
	unitBRAMwidth_type tempvmask_sp; 
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
	}
	return;
}

void //
	#ifdef SW 
	actsproc::
	#endif 
loadvmasks_subp(bool_type enable, unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unitBRAMwidth_type vmask_subp[BLOCKRAM_SIZE], buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount3 = BLOCKRAM_SIZE;
	
	buffer_type transfsize = size_kvs * 16;
	
	unitBRAMwidth_type tempvmask_sp; 
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
	if(GraphAlgo == PAGERANK){
		res = udata;
	} else if(GraphAlgo == BFS){
		res = NAp;
	} else if(GraphAlgo == SSSP){
		res = udata + edgew;
	} else {
		res = NAp;
	}
	return res;
}

fetchmessage_t 
	#ifdef SW 
	actsproc::
	#endif 
readandprocess(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unitBRAMwidth_type vmask_subp[BLOCKRAM_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], 
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
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	
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
		unsigned int mask;
		if(GraphAlgo == PAGERANK){ mask = 1; } 
		else { mask = GETVTXMASK(vmask, lvid, globalparams); }
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
	
	fetchmessage.chunksize_kvs = loadcount;
	return fetchmessage;
}

// functions (partition)
void 
	#ifdef SW 
	actsproc::
	#endif
preparekeyvalues(bool_type enable1, bool_type enable2, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_buffer_t destbuffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[VECTOR_SIZE][NUM_PARTITIONS], step_type currentLOP, sweepparams_t sweepparams, buffer_type size_kvs, buffer_type cutoffs[VECTOR_SIZE], globalparams_t globalparams){				
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

void 
	#ifdef SW 
	actsproc::
	#endif
priorpartitionkeyvalues(bool_type enable1, bool_type enable2, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_buffer_t destbuffer[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[NUM_PARTITIONS], step_type currentLOP, sweepparams_t sweepparams, buffer_type size_kvs, globalparams_t globalparams){				
	#ifdef ENABLERECURSIVEPARTITIONING
	if(currentLOP == globalparams.ACTSPARAMS_TREEDEPTH){ return; } /// NEWCHANGE.
	#else 
	if(currentLOP > globalparams.ACTSPARAMS_TREEDEPTH){ return; } /// NEWCHANGE.	
	#endif 
	if(enable1 == OFF && enable2 == OFF){ return; }
	analysis_type analysis_loopcount = SOURCEBLOCKRAM_SIZE;
	analysis_type analysis_loopcount2 = VECTOR_SIZE;
	
	buffer_type chunk_size = size_kvs;
	unsigned int upperlimit = sweepparams.upperlimit;
	unsigned int upperpartition = sweepparams.upperpartition;
	
	for(partition_type p=0; p<NUM_PARTITIONS; p++){ 
	#pragma HLS PIPELINE II=1
		localcapsule[p].key = 0;
		localcapsule[p].value = 0; 
		localcapsule[p].key = 0;
		localcapsule[p].value = 0; 
		localcapsule[p].key = 0;
		localcapsule[p].value = 0; 
		localcapsule[p].key = 0;
		localcapsule[p].value = 0; 
		localcapsule[p].key = 0;
		localcapsule[p].value = 0; 
		localcapsule[p].key = 0;
		localcapsule[p].value = 0; 
		localcapsule[p].key = 0;
		localcapsule[p].value = 0; 
		localcapsule[p].key = 0;
		localcapsule[p].value = 0; 
	}
	
	BASICPARTITIONKEYVALUES_LOOP1: for(buffer_type i=0; i<chunk_size; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
		BASICPARTITIONKEYVALUES_LOOP1B: for(unsigned int v=0; v<VECTOR_SIZE; v++){
		#pragma HLS PIPELINE II=2
			keyvalue_buffer_t kv = sourcebuffer[v][i];
			
			partition_type p = getpartition(ON, kv, currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE);
			if(GETKV(kv).key != GETV(INVALIDDATA) && GETKV(kv).value != GETV(INVALIDDATA)){ localcapsule[p].value += 1; }
		}
	}
	
	calculateoffsets(localcapsule, NUM_PARTITIONS);
	resetvalues(localcapsule, NUM_PARTITIONS, 0);
	
	BASICPARTITIONKEYVALUES_LOOP2: for(buffer_type i=0; i<chunk_size; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
		BASICPARTITIONKEYVALUES_LOOP2B: for(unsigned int v=0; v<VECTOR_SIZE; v++){
		#pragma HLS PIPELINE II=2
			keyvalue_buffer_t kv = sourcebuffer[v][i];
			// cout<<"--- priorpartitionkeyvalues: kv.key: "<<kv.key<<endl; // REMOVEME.
			partition_type p = getpartition(ON, kv, currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE);
			buffer_type pos = localcapsule[p].key + localcapsule[p].value;
			
			if(GETKV(kv).key != GETV(INVALIDDATA) && GETKV(kv).value != GETV(INVALIDDATA)){ destbuffer[pos % VECTOR_SIZE][pos / VECTOR_SIZE] = kv; } // NOTE: could this be the cause of slight imperfection in results?
			if(GETKV(kv).key != GETV(INVALIDDATA) && GETKV(kv).value != GETV(INVALIDDATA)){ localcapsule[p].value += 1; }
		}
	}
	
	BASICPARTITIONKEYVALUES_LOOP3: for(partition_type p=0; p<NUM_PARTITIONS; p++){
	// #pragma HLS PIPELINE II=8
		keyvalue_t mydummykv;
		mydummykv.key = p;
		mydummykv.value = GETV(INVALIDDATA);
		keyvalue_buffer_t dummykv = GETKV(mydummykv);
	
		unsigned int endoffset = localcapsule[p].key + localcapsule[p].value;
		unsigned int xpos = endoffset % VECTOR_SIZE;
		unsigned int ypos = endoffset / VECTOR_SIZE;
		
		if(localcapsule[p].value > 0){
			BASICPARTITIONKEYVALUES_LOOP3B: for(vector_type v=xpos; v<VECTOR_SIZE; v++){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount2 avg=analysis_loopcount2
			#pragma HLS PIPELINE II=2
				destbuffer[v][ypos] = dummykv;
				localcapsule[p].value += 1;
			}
		}
	}
	return;
}

// functions (reduce)
value_t 
	#ifdef SW 
	actsproc::
	#endif 
reducefunc(value_t vtemp, value_t res, unsigned int GraphIter, unsigned int GraphAlgo){
	value_t temp = 0;
	if(GraphAlgo == PAGERANK){
		temp = vtemp + res;
	} else if(GraphAlgo == BFS){
		temp = amin(vtemp, GraphIter);
	} else if(GraphAlgo == SSSP){
		temp = amin(vtemp, res);
	} else {
		temp = NAp;
	}
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
reduceandbuffer(bool_type enable, keyvalue_buffer_t buffer[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[NUM_PARTITIONS], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], sweepparams_t sweepparams, globalparams_t globalparams){				
	if(enable == OFF){ return; }
	#if REDUCEBUFFERFACTOR==8
	analysis_type analysis_loopcount = (DESTBLOCKRAM_SIZE / (NUM_PARTITIONS / 2)); // =46: '2' is safety padding.
	#endif 
	#if REDUCEBUFFERFACTOR==16
	analysis_type analysis_loopcount = (DESTBLOCKRAM_SIZE / NUM_PARTITIONS);
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
	#endif 
	return;
}

void 
	#ifdef SW 
	actsproc::
	#endif 
priorreduceandbuffer(bool_type enable, keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[NUM_PARTITIONS], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], buffer_type chunk_size, sweepparams_t sweepparams, globalparams_t globalparams){				
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount = SOURCEBLOCKRAM_SIZE;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printkeyvalues("priorreduceandbuffer.localcapsule", (keyvalue_t *)localcapsule, 8);
	#endif 
	
	PRIORREDUCEANDBUFFER_LOOP1: for(buffer_type i=0; i<chunk_size; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE II=16 // NEWCHANGE.
		PRIORREDUCEANDBUFFER_LOOP1B: for(unsigned int v=0; v<VECTOR_SIZE; v++){
		// #pragma HLS PIPELINE II=1 // NEWCHANGE.
			keyvalue_buffer_t kv = buffer[v][i];
			keyvalue_t kv2 = GETKV(kv);
		
			partition_type p = ((kv2.key - sweepparams.upperlimit) >> (REDUCESZ_POW));
			
			unsigned int upperlimit = sweepparams.upperlimit + p*REDUCESZ;
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			cout<<"priorreduceandbuffer: kv2.key: "<<kv2.key<<", kv2.value: "<<kv2.value<<", p: "<<p<<", upperlimit: "<<upperlimit<<", sweepparams.upperlimit: "<<sweepparams.upperlimit<<", currentLOP: "<<sweepparams.currentLOP<<endl;
			#endif 
			
			if(p < VDATA_PACKINGSIZE){ reducevector(kv, vbuffer[p], 0, upperlimit, sweepparams, globalparams); } // REMOVEME.
		}
	}
	return;
}

/* void 
	#ifdef SW 
	actsproc::
	#endif 
tradreduceandbuffer(bool_type enable, uint512_dt * kvdram, keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], buffer_type chunk_size, globalparams_t globalparams){				
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount = SOURCEBLOCKRAM_SIZE;
	
	BASICREDUCEANDBUFFER_LOOP1: for(buffer_type i=0; i<chunk_size; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE II=16 // NEWCHANGE.
		BASICREDUCEANDBUFFER_LOOP1B: for(unsigned int v=0; v<VECTOR_SIZE; v++){
			keyvalue_buffer_t kv = buffer[v][i];
			keyvalue_t kv2 = GETKV(kv);
			
			if(kv2.key != GETV(INVALIDDATA) && kv2.value != GETV(INVALIDDATA)){
			
				unsigned int block = kv2.key / (globalparams.SIZE_REDUCE * VECTOR_SIZE);
				unsigned int globalpos_offset = block * (globalparams.SIZE_REDUCE * VECTOR_SIZE);
				unsigned int globalrow_offset = globalpos_offset / VECTOR2_SIZE;
				
				unsigned int localpos = kv2.key % (globalparams.SIZE_REDUCE * VECTOR_SIZE); // 8192;
				unsigned int localrow = (localpos % globalparams.SIZE_REDUCE) / 2;
				unsigned int localcol = localpos / globalparams.SIZE_REDUCE;
				
				unsigned int row = globalrow_offset + localrow;
				unsigned int col = localcol;
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				cout<<"REDUCE SEEN @ tradreduceandbuffer:: kv2.key: "<<kv2.key<<", kv2.value: "<<kv2.value<<" || block: "<<block<<", posoffset: "<<posoffset<<", rowoffset: "<<rowoffset<<", row: "<<row<<", col: "<<col<<" || localpos: "<<localpos<<", localrow: "<<localrow<<", localcol: "<<localcol<<endl;
				#endif
				
				value_t temp;
				#ifdef _WIDEWORD
				if(localpos % 2 == 0){ temp = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(32 * ((localcol * 2) + 1) - 1, (localcol * 2) * 32); }
				else { temp = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(32 * (((localcol * 2) + 1) + 1) - 1, (localcol * 2 + 1) * 32); }
				#else 
				if(localpos % 2 == 0){ temp = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[localcol].key; }
				else { temp = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[localcol].value; }	
				#endif 
				
				value_t rettemp = reducefunc(temp, kv2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _WIDEWORD
				if(localpos % 2 == 0){ kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(32 * ((localcol * 2) + 1) - 1, (localcol * 2) * 32) = rettemp; }
				else { kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(32 * (((localcol * 2) + 1) + 1) - 1, (localcol * 2 + 1) * 32) = rettemp; }
				#else 
				if(localpos % 2 == 0){ kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[localcol].key = rettemp; }
				else { kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[localcol].value = rettemp; }
				#endif 
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(localpos % 2 == 0){ cout<<"--- tradreduceandbuffer :: kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + "<<row<<"].data["<<localcol<<"].key: "<<kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[localcol].key<<endl; }
				else { cout<<"--- tradreduceandbuffer :: kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + "<<row<<"].data["<<localcol<<"].value: "<<kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[localcol].value<<endl; }
				#endif
			}
		}
	}
	return;
} */
/* void
	#ifdef SW
	actsproc::
	#endif 
tradreduceandbuffer(bool_type enable, uint512_dt * kvdram, keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], buffer_type chunk_size, globalparams_t globalparams){				
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount = SOURCEBLOCKRAM_SIZE;
	
	TRADREDUCEANDBUFFER_LOOP1: for(buffer_type i=0; i<chunk_size; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	// #pragma HLS PIPELINE II=16 // NEWCHANGE.
		TRADREDUCEANDBUFFER_LOOP1B: for(unsigned int v=0; v<VECTOR_SIZE; v++){
		#pragma HLS PIPELINE II=1 // NEWCHANGE.
			keyvalue_buffer_t kv = buffer[v][i];
			keyvalue_t kv2 = GETKV(kv);
			
			if(kv2.key != GETV(INVALIDDATA) && kv2.value != GETV(INVALIDDATA)){
			
				unsigned int block = kv2.key / (globalparams.SIZE_REDUCE * VECTOR_SIZE);
				unsigned int globalpos_offset = block * (globalparams.SIZE_REDUCE * VECTOR_SIZE);
				unsigned int globalrow_offset = globalpos_offset / VECTOR2_SIZE;
				
				unsigned int localpos = kv2.key % (globalparams.SIZE_REDUCE * VECTOR_SIZE); // 8192;
				unsigned int localrow = (localpos % globalparams.SIZE_REDUCE) / 2;
				unsigned int localcol = localpos / globalparams.SIZE_REDUCE;
				
				unsigned int row = globalrow_offset + localrow;
				unsigned int col = localcol;
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				cout<<"REDUCE SEEN @ tradreduceandbuffer:: kv2.key: "<<kv2.key<<", kv2.value: "<<kv2.value<<" || block: "<<block<<", posoffset: "<<posoffset<<", rowoffset: "<<rowoffset<<", row: "<<row<<", col: "<<col<<" || localpos: "<<localpos<<", localrow: "<<localrow<<", localcol: "<<localcol<<endl;
				#endif
				
				value_t temp;
				uint512_vec_dt vdata;
				
				// #ifdef _WIDEWORD
				// 				// vdata.data[0].key = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(31, 0);
				// vdata.data[0].value = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(63, 32);
				// 				// vdata.data[1].key = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(95, 64);
				// vdata.data[1].value = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(127, 96);
				// 				// vdata.data[2].key = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(159, 128);
				// vdata.data[2].value = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(191, 160);
				// 				// vdata.data[3].key = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(223, 192);
				// vdata.data[3].value = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(255, 224);
				// 				// vdata.data[4].key = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(287, 256);
				// vdata.data[4].value = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(319, 288);
				// 				// vdata.data[5].key = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(351, 320);
				// vdata.data[5].value = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(383, 352);
				// 				// vdata.data[6].key = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(415, 384);
				// vdata.data[6].value = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(447, 416);
				// 				// vdata.data[7].key = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(479, 448);
				// vdata.data[7].value = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(511, 480);
				// 				// #else 
				// 				// vdata.data[0].key = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[0].key; 
				// vdata.data[0].value = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[0].value; 
				// 				// vdata.data[1].key = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[1].key; 
				// vdata.data[1].value = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[1].value; 
				// 				// vdata.data[2].key = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[2].key; 
				// vdata.data[2].value = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[2].value; 
				// 				// vdata.data[3].key = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[3].key; 
				// vdata.data[3].value = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[3].value; 
				// 				// vdata.data[4].key = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[4].key; 
				// vdata.data[4].value = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[4].value; 
				// 				// vdata.data[5].key = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[5].key; 
				// vdata.data[5].value = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[5].value; 
				// 				// vdata.data[6].key = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[6].key; 
				// vdata.data[6].value = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[6].value; 
				// 				// vdata.data[7].key = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[7].key; 
				// vdata.data[7].value = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[7].value; 
				// 				// #endif 
				#ifdef _WIDEWORD // CRITICAL REMOVEME.
				vdata.data[0].key = 0 * globalparams.BASEOFFSETKVS_VERTICESDATA;
				vdata.data[0].value = 0 * globalparams.BASEOFFSETKVS_VERTICESDATA + globalparams.ALGORITHMINFO_GRAPHITERATIONID;
				vdata.data[1].key = 1 * globalparams.BASEOFFSETKVS_VERTICESDATA;
				vdata.data[1].value = 1 * globalparams.BASEOFFSETKVS_VERTICESDATA + globalparams.ALGORITHMINFO_GRAPHITERATIONID;
				vdata.data[2].key = 2 * globalparams.BASEOFFSETKVS_VERTICESDATA;
				vdata.data[2].value = 2 * globalparams.BASEOFFSETKVS_VERTICESDATA + globalparams.ALGORITHMINFO_GRAPHITERATIONID;
				vdata.data[3].key = 3 * globalparams.BASEOFFSETKVS_VERTICESDATA;
				vdata.data[3].value = 3 * globalparams.BASEOFFSETKVS_VERTICESDATA + globalparams.ALGORITHMINFO_GRAPHITERATIONID;
				vdata.data[4].key = 4 * globalparams.BASEOFFSETKVS_VERTICESDATA;
				vdata.data[4].value = 4 * globalparams.BASEOFFSETKVS_VERTICESDATA + globalparams.ALGORITHMINFO_GRAPHITERATIONID;
				vdata.data[5].key = 5 * globalparams.BASEOFFSETKVS_VERTICESDATA;
				vdata.data[5].value = 5 * globalparams.BASEOFFSETKVS_VERTICESDATA + globalparams.ALGORITHMINFO_GRAPHITERATIONID;
				vdata.data[6].key = 6 * globalparams.BASEOFFSETKVS_VERTICESDATA;
				vdata.data[6].value = 6 * globalparams.BASEOFFSETKVS_VERTICESDATA + globalparams.ALGORITHMINFO_GRAPHITERATIONID;
				vdata.data[7].key = 7 * globalparams.BASEOFFSETKVS_VERTICESDATA;
				vdata.data[7].value = 7 * globalparams.BASEOFFSETKVS_VERTICESDATA + globalparams.ALGORITHMINFO_GRAPHITERATIONID;
				#endif 
				
				if(localpos % 2 == 0){ temp = vdata.data[localcol].key; }
				else { temp = vdata.data[localcol].value; }
				
				value_t rettemp = reducefunc(temp, kv2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				if(localpos % 2 == 0){ vdata.data[localcol].key = rettemp; }
				else { vdata.data[localcol].value = rettemp; }
				
				#ifdef _WIDEWORD
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(31, 0) = vdata.data[0].key;
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(63, 32) = vdata.data[0].value;
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(95, 64) = vdata.data[1].key;
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(127, 96) = vdata.data[1].value;
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(159, 128) = vdata.data[2].key;
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(191, 160) = vdata.data[2].value;
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(223, 192) = vdata.data[3].key;
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(255, 224) = vdata.data[3].value;
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(287, 256) = vdata.data[4].key;
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(319, 288) = vdata.data[4].value;
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(351, 320) = vdata.data[5].key;
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(383, 352) = vdata.data[5].value;
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(415, 384) = vdata.data[6].key;
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(447, 416) = vdata.data[6].value;
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(479, 448) = vdata.data[7].key;
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(511, 480) = vdata.data[7].value;
				#else 
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[0].key = vdata.data[0].key; 
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[0].value = vdata.data[0].value; 
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[1].key = vdata.data[1].key; 
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[1].value = vdata.data[1].value; 
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[2].key = vdata.data[2].key; 
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[2].value = vdata.data[2].value; 
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[3].key = vdata.data[3].key; 
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[3].value = vdata.data[3].value; 
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[4].key = vdata.data[4].key; 
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[4].value = vdata.data[4].value; 
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[5].key = vdata.data[5].key; 
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[5].value = vdata.data[5].value; 
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[6].key = vdata.data[6].key; 
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[6].value = vdata.data[6].value; 
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[7].key = vdata.data[7].key; 
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[7].value = vdata.data[7].value; 
				#endif
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(localpos % 2 == 0){ cout<<"--- tradreduceandbuffer :: kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + "<<row<<"].data["<<localcol<<"].key: "<<kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[localcol].key<<endl; }
				else { cout<<"--- tradreduceandbuffer :: kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + "<<row<<"].data["<<localcol<<"].value: "<<kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[localcol].value<<endl; }
				#endif
			}
		}
	}
	return;
} */
void
	#ifdef SW
	actsproc::
	#endif 
tradreduceandbuffer(bool_type enable, uint512_dt * kvdram, keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], buffer_type chunk_size, keyvalue_t globalstatsbuffer[NUM_PARTITIONS], sweepparams_t sweepparams, globalparams_t globalparams){				
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount = SOURCEBLOCKRAM_SIZE;
	
	TRADREDUCEANDBUFFER_LOOP1: for(buffer_type i=0; i<chunk_size; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
		TRADREDUCEANDBUFFER_LOOP1B: for(unsigned int v=0; v<VECTOR_SIZE; v++){
		#pragma HLS PIPELINE II=1 // NEWCHANGE.
			keyvalue_buffer_t kv = buffer[v][i];
			keyvalue_t kv2 = GETKV(kv);
			
			if(kv2.key != GETV(INVALIDDATA) && kv2.value != GETV(INVALIDDATA)){
			
				unsigned int block = kv2.key / (REDUCESZ * VECTOR_SIZE); // REDUCESZ, globalparams.SIZE_REDUCE 
				unsigned int globalpos_offset = block * (REDUCESZ * VECTOR_SIZE);
				unsigned int globalrow_offset = globalpos_offset / VECTOR2_SIZE;
				
				unsigned int localpos = kv2.key % (REDUCESZ * VECTOR_SIZE); // 8192;
				unsigned int localrow = (localpos % REDUCESZ) / 2;
				unsigned int localcol = localpos / REDUCESZ;
				
				unsigned int row = globalrow_offset + localrow;
				unsigned int col = localcol;
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				cout<<"REDUCE SEEN @ tradreduceandbuffer:: kv2.key: "<<kv2.key<<", kv2.value: "<<kv2.value<<" || block: "<<block<<", posoffset: "<<posoffset<<", rowoffset: "<<rowoffset<<", row: "<<row<<", col: "<<col<<" || localpos: "<<localpos<<", localrow: "<<localrow<<", localcol: "<<localcol<<endl;
				#endif
				
				value_t temp;
				uint512_vec_dt vdata;
				#pragma HLS data_pack variable=vdata
				
				#ifdef _WIDEWORD
				vdata.data[0].key = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(31, 0);
				vdata.data[0].value = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(63, 32);
				vdata.data[1].key = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(95, 64);
				vdata.data[1].value = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(127, 96);
				vdata.data[2].key = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(159, 128);
				vdata.data[2].value = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(191, 160);
				vdata.data[3].key = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(223, 192);
				vdata.data[3].value = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(255, 224);
				vdata.data[4].key = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(287, 256);
				vdata.data[4].value = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(319, 288);
				vdata.data[5].key = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(351, 320);
				vdata.data[5].value = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(383, 352);
				vdata.data[6].key = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(415, 384);
				vdata.data[6].value = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(447, 416);
				vdata.data[7].key = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(479, 448);
				vdata.data[7].value = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(511, 480);
				#else 
				vdata.data[0].key = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[0].key; 
				vdata.data[0].value = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[0].value; 
				vdata.data[1].key = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[1].key; 
				vdata.data[1].value = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[1].value; 
				vdata.data[2].key = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[2].key; 
				vdata.data[2].value = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[2].value; 
				vdata.data[3].key = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[3].key; 
				vdata.data[3].value = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[3].value; 
				vdata.data[4].key = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[4].key; 
				vdata.data[4].value = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[4].value; 
				vdata.data[5].key = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[5].key; 
				vdata.data[5].value = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[5].value; 
				vdata.data[6].key = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[6].key; 
				vdata.data[6].value = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[6].value; 
				vdata.data[7].key = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[7].key; 
				vdata.data[7].value = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[7].value; 
				#endif 
				/* #ifdef _WIDEWORD // CRITICAL REMOVEME.
				vdata.data[0].key = 0 * globalparams.BASEOFFSETKVS_VERTICESDATA;
				vdata.data[0].value = 0 * globalparams.BASEOFFSETKVS_VERTICESDATA + globalparams.ALGORITHMINFO_GRAPHITERATIONID;
				vdata.data[1].key = 1 * globalparams.BASEOFFSETKVS_VERTICESDATA;
				vdata.data[1].value = 1 * globalparams.BASEOFFSETKVS_VERTICESDATA + globalparams.ALGORITHMINFO_GRAPHITERATIONID;
				vdata.data[2].key = 2 * globalparams.BASEOFFSETKVS_VERTICESDATA;
				vdata.data[2].value = 2 * globalparams.BASEOFFSETKVS_VERTICESDATA + globalparams.ALGORITHMINFO_GRAPHITERATIONID;
				vdata.data[3].key = 3 * globalparams.BASEOFFSETKVS_VERTICESDATA;
				vdata.data[3].value = 3 * globalparams.BASEOFFSETKVS_VERTICESDATA + globalparams.ALGORITHMINFO_GRAPHITERATIONID;
				vdata.data[4].key = 4 * globalparams.BASEOFFSETKVS_VERTICESDATA;
				vdata.data[4].value = 4 * globalparams.BASEOFFSETKVS_VERTICESDATA + globalparams.ALGORITHMINFO_GRAPHITERATIONID;
				vdata.data[5].key = 5 * globalparams.BASEOFFSETKVS_VERTICESDATA;
				vdata.data[5].value = 5 * globalparams.BASEOFFSETKVS_VERTICESDATA + globalparams.ALGORITHMINFO_GRAPHITERATIONID;
				vdata.data[6].key = 6 * globalparams.BASEOFFSETKVS_VERTICESDATA;
				vdata.data[6].value = 6 * globalparams.BASEOFFSETKVS_VERTICESDATA + globalparams.ALGORITHMINFO_GRAPHITERATIONID;
				vdata.data[7].key = 7 * globalparams.BASEOFFSETKVS_VERTICESDATA;
				vdata.data[7].value = 7 * globalparams.BASEOFFSETKVS_VERTICESDATA + globalparams.ALGORITHMINFO_GRAPHITERATIONID;
				#endif  */
				
				if(localpos % 2 == 0){ temp = vdata.data[localcol].key; }
				else { temp = vdata.data[localcol].value; }
				
				value_t rettemp = reducefunc(temp, kv2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				if(localpos % 2 == 0){ vdata.data[localcol].key = rettemp; }
				else { vdata.data[localcol].value = rettemp; }
				
				#ifdef _WIDEWORD
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(31, 0) = vdata.data[0].key;
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(63, 32) = vdata.data[0].value;
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(95, 64) = vdata.data[1].key;
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(127, 96) = vdata.data[1].value;
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(159, 128) = vdata.data[2].key;
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(191, 160) = vdata.data[2].value;
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(223, 192) = vdata.data[3].key;
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(255, 224) = vdata.data[3].value;
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(287, 256) = vdata.data[4].key;
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(319, 288) = vdata.data[4].value;
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(351, 320) = vdata.data[5].key;
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(383, 352) = vdata.data[5].value;
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(415, 384) = vdata.data[6].key;
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(447, 416) = vdata.data[6].value;
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(479, 448) = vdata.data[7].key;
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].range(511, 480) = vdata.data[7].value;
				#else 
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[0].key = vdata.data[0].key; 
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[0].value = vdata.data[0].value; 
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[1].key = vdata.data[1].key; 
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[1].value = vdata.data[1].value; 
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[2].key = vdata.data[2].key; 
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[2].value = vdata.data[2].value; 
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[3].key = vdata.data[3].key; 
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[3].value = vdata.data[3].value; 
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[4].key = vdata.data[4].key; 
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[4].value = vdata.data[4].value; 
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[5].key = vdata.data[5].key; 
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[5].value = vdata.data[5].value; 
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[6].key = vdata.data[6].key; 
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[6].value = vdata.data[6].value; 
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[7].key = vdata.data[7].key; 
				kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[7].value = vdata.data[7].value; 
				#endif
				
				partition_type p = getpartition(ON, kv, sweepparams.currentLOP, sweepparams.upperlimit, sweepparams.upperpartition, globalparams.POW_BATCHRANGE);
				globalstatsbuffer[p].value += 1;
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(localpos % 2 == 0){ cout<<"--- tradreduceandbuffer :: kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + "<<row<<"].data["<<localcol<<"].key: "<<kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[localcol].key<<endl; }
				else { cout<<"--- tradreduceandbuffer :: kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + "<<row<<"].data["<<localcol<<"].value: "<<kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + row].data[localcol].value<<endl; }
				#endif
			}
		}
	}
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

void 
	#ifdef SW 
	actsproc::
	#endif 
resetenvbuffer(keyvalue_capsule_t capsule_so8[NUM_PARTITIONS]){
	for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		capsule_so8[p].key = 0;
		capsule_so8[p].value = 0;
	}
	return;
}

fetchmessage_t 
	#ifdef SW 
	actsproc::
	#endif 
fetchkeyvalues(bool_type enable, unsigned int mode, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unitBRAMwidth_type vmask_subp[BLOCKRAM_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], 
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
commitkeyvalues(bool_type enable1, bool_type enable2, unsigned int mode, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], 
		keyvalue_buffer_t destbuffer[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_t globalcapsule[NUM_PARTITIONS], keyvalue_capsule_t localcapsule[NUM_PARTITIONS], 
			batch_type destbaseaddr_kvs, buffer_type chunk_size, sweepparams_t sweepparams, globalparams_t globalparams){
	if(mode == REDUCEMODE){
		reduceandbuffer(enable1, destbuffer, localcapsule, vbuffer, sweepparams, globalparams);
	} else {
		savekeyvalues(enable1, kvdram, destbuffer, globalcapsule, localcapsule, destbaseaddr_kvs, globalparams); 
	}
	return;
}

void 
	#ifdef SW 
	actsproc::
	#endif 
priorcommitkeyvalues(bool_type enable1, bool_type enable2, unsigned int mode, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], 
		keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_buffer_t destbuffer[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_t globalcapsule[NUM_PARTITIONS], keyvalue_capsule_t localcapsule[NUM_PARTITIONS], 
				batch_type destbaseaddr_kvs, buffer_type chunk_size, sweepparams_t sweepparams, globalparams_t globalparams){
	if(mode == REDUCEMODE){
		priorreduceandbuffer(enable1, sourcebuffer, localcapsule, vbuffer, chunk_size, sweepparams, globalparams); // REMOVEME.
	} else {
		// actsutilityobj->printkeyvalues("**************************************************priorcommitkeyvalues: localcapsule", (keyvalue_t *)localcapsule, 8); // REMOVEME. // NUM_PARTITIONS
		savekeyvalues(enable1, kvdram, destbuffer, globalcapsule, localcapsule, destbaseaddr_kvs, globalparams); 
		// actsutilityobj->printkeyvalues("++++++++++++++++++++++++++++++++++++++++++++++++++priorcommitkeyvalues: globalcapsule", (keyvalue_t *)globalcapsule, 8); // REMOVEME. // NUM_PARTITIONS
	}
	return;
}

void 
	#ifdef SW 
	actsproc::
	#endif
actspipeline(bool_type enable1, bool_type enable2, keyvalue_buffer_t buffer_setof1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_capsule_t capsule_so1[VECTOR_SIZE][NUM_PARTITIONS], 
						keyvalue_buffer_t buffer_setof8[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_capsule_t capsule_so8[NUM_PARTITIONS],
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
			actsutilityobj->checkoutofbounds(enablebufferD, "posD0", posD0, DESTBLOCKRAM_SIZE * VECTOR_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferD, "yoffset0", yoffset0, DESTBLOCKRAM_SIZE, NAp, NAp, NAp);
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
			actsutilityobj->checkoutofbounds(enablebufferD, "_posD0", _posD0, DESTBLOCKRAM_SIZE * VECTOR_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferD, "yoffset1", yoffset1, DESTBLOCKRAM_SIZE, NAp, NAp, NAp);
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
			actsutilityobj->checkoutofbounds(enablebufferD, "__posD0", __posD0, DESTBLOCKRAM_SIZE * VECTOR_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferD, "yoffset2", yoffset2, DESTBLOCKRAM_SIZE, NAp, NAp, NAp);
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
			actsutilityobj->checkoutofbounds(enablebufferD, "___posD0", ___posD0, DESTBLOCKRAM_SIZE * VECTOR_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferD, "yoffset3", yoffset3, DESTBLOCKRAM_SIZE, NAp, NAp, NAp);
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
		uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unitBRAMwidth_type vmask_subp[BLOCKRAM_SIZE], keyvalue_t globalstatsbuffer[NUM_PARTITIONS], 
		globalparams_t globalparams, sweepparams_t sweepparams, travstate_t ptravstate, batch_type sourcebaseaddr_kvs, batch_type destbaseaddr_kvs,
		bool_type resetenv, bool_type flush){
	analysis_type analysis_partitionloop = MODEL_BATCHSIZE_KVS / (NUMPIPELINES_PARTITIONUPDATES * WORKBUFFER_SIZE);
	if(enable == OFF){ return; }
	
static keyvalue_buffer_t buffer_setof1[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = buffer_setof1
static keyvalue_buffer_t buffer_setof8[VECTOR_SIZE][DESTBLOCKRAM_SIZE];
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
		commitkeyvalues(pp1writeen, ON, mode, kvdram, vbuffer, buffer_setof8, globalstatsbuffer, capsule_so8, destbaseaddr_kvs, fetchmessagepp1.chunksize_kvs, sweepparams, globalparams); 
		#endif 
		
		actspipeline(ON, ON, buffer_setof1, capsule_so1, buffer_setof8, capsule_so8, sweepparams.currentLOP, sweepparams, pp0cutoffs, itercount, globalparams);
		#ifdef PUP1
		ptravstatepp1.i_kvs = offset_kvs;
		fetchmessagepp1 = fetchkeyvalues(ON, mode, kvdram, vbuffer, vmask, vmask_subp, sourcebuffer, sourcebaseaddr_kvs, ptravstatepp1.i_kvs, WORKBUFFER_SIZE, ptravstatepp1, sweepparams, globalparams);
		if(mode == PROCESSMODE && fetchmessagepp1.nextoffset_kvs != -1){ offset_kvs = fetchmessagepp1.nextoffset_kvs; } else { offset_kvs+=WORKBUFFER_SIZE; } 
		#endif
		
		commitkeyvalues(pp0writeen, ON, mode, kvdram, vbuffer, buffer_setof8, globalstatsbuffer, capsule_so8, destbaseaddr_kvs, fetchmessagepp0.chunksize_kvs, sweepparams, globalparams); 
		#ifdef PUP1
		preparekeyvalues(pp1partitionen, ON, sourcebuffer, buffer_setof1, capsule_so1, sweepparams.currentLOP, sweepparams, fetchmessagepp1.chunksize_kvs, pp1cutoffs, globalparams);
		#endif
		
		itercount += NUMPIPELINES_PARTITIONUPDATES;
	}
	return;
}

void 
	#ifdef SW 
	actsproc::
	#endif
priorit(bool_type enable, unsigned int mode,
		uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unitBRAMwidth_type vmask_subp[BLOCKRAM_SIZE], keyvalue_t globalstatsbuffer[NUM_PARTITIONS], 
		globalparams_t globalparams, sweepparams_t sweepparams, travstate_t ptravstate, batch_type sourcebaseaddr_kvs, batch_type destbaseaddr_kvs,
		bool_type resetenv, bool_type flush){
	analysis_type analysis_partitionloop = MODEL_BATCHSIZE_KVS / (NUMPIPELINES_PARTITIONUPDATES * WORKBUFFER_SIZE);
	if(enable == OFF){ return; }
	
	#ifdef PUP1
	keyvalue_buffer_t sourcebufferpp1[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE];
	#pragma HLS array_partition variable = sourcebufferpp1
	#endif 
	
static keyvalue_buffer_t buffer_setof8[VECTOR_SIZE][DESTBLOCKRAM_SIZE];
	#pragma HLS array_partition variable = buffer_setof8
	#ifdef PUP1
static keyvalue_buffer_t bufferpp1_setof8[VECTOR_SIZE][DESTBLOCKRAM_SIZE];
	#pragma HLS array_partition variable = bufferpp1_setof8
	#endif 
	
static keyvalue_capsule_t capsule_so8[NUM_PARTITIONS];
	#ifdef PUP1
static keyvalue_capsule_t capsulepp1_so8[NUM_PARTITIONS];
	#endif 
	
	travstate_t ptravstatepp0 = ptravstate;
	travstate_t ptravstatepp1 = ptravstate;
	
	fetchmessage_t fetchmessagepp0;
	fetchmessage_t fetchmessagepp1;
	fetchmessagepp0.chunksize_kvs = -1; fetchmessagepp0.nextoffset_kvs = -1;
	fetchmessagepp1.chunksize_kvs = -1; fetchmessagepp1.nextoffset_kvs = -1;
	
	if(resetenv == ON){ resetenvbuffer(capsule_so8); }
	#ifdef PUP1
	if(resetenv == ON){ resetenvbuffer(capsulepp1_so8); }
	#endif 
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(resetenv == ON){ cout<<"priorit: reset is ON"<<endl; } else { cout<<"priorit: reset is OFF"<<endl;  }
	#endif 
	
	batch_type offset_kvs = ptravstate.begin_kvs;

	PRIORIT_MAINLOOP: while(offset_kvs < ptravstate.end_kvs){ // NEWCHANGE.
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_partitionloop avg=analysis_partitionloop

		ptravstatepp0.i_kvs = offset_kvs;
		fetchmessagepp0 = fetchkeyvalues(ON, mode, kvdram, vbuffer, vmask, vmask_subp, sourcebuffer, sourcebaseaddr_kvs, ptravstatepp0.i_kvs, WORKBUFFER_SIZE, ptravstatepp0, sweepparams, globalparams);
		if(mode == PROCESSMODE && fetchmessagepp0.nextoffset_kvs != -1){ offset_kvs = fetchmessagepp0.nextoffset_kvs; } else { offset_kvs+=WORKBUFFER_SIZE; } 
		
		priorpartitionkeyvalues(ON, ON, sourcebuffer, buffer_setof8, capsule_so8, sweepparams.currentLOP, sweepparams, fetchmessagepp0.chunksize_kvs, globalparams);
		#ifdef PUP1
		ptravstatepp0.i_kvs = offset_kvs;
		fetchmessagepp1 = fetchkeyvalues(ON, mode, kvdram, vbuffer, vmask, vmask_subp, sourcebufferpp1, sourcebaseaddr_kvs, ptravstatepp0.i_kvs, WORKBUFFER_SIZE, ptravstatepp0, sweepparams, globalparams);
		if(mode == PROCESSMODE && fetchmessagepp1.nextoffset_kvs != -1){ offset_kvs = fetchmessagepp1.nextoffset_kvs; } else { offset_kvs+=WORKBUFFER_SIZE; } 
		#endif 
		
		priorcommitkeyvalues(ON, ON, mode, kvdram, vbuffer, sourcebuffer, buffer_setof8, globalstatsbuffer, capsule_so8, destbaseaddr_kvs, fetchmessagepp0.chunksize_kvs, sweepparams, globalparams); 
		#ifdef PUP1
		priorpartitionkeyvalues(ON, ON, sourcebufferpp1, bufferpp1_setof8, capsulepp1_so8, sweepparams.currentLOP, sweepparams, fetchmessagepp1.chunksize_kvs, globalparams);
		#endif 
		
		#ifdef PUP1
		priorcommitkeyvalues(ON, ON, mode, kvdram, vbuffer, sourcebufferpp1, bufferpp1_setof8, globalstatsbuffer, capsulepp1_so8, destbaseaddr_kvs, fetchmessagepp1.chunksize_kvs, sweepparams, globalparams); 
		#endif
	}
	return;
}

void 
	#ifdef SW 
	actsproc::
	#endif
tradit(bool_type enable, unsigned int mode,
		uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unitBRAMwidth_type vmask_subp[BLOCKRAM_SIZE], keyvalue_t globalstatsbuffer[NUM_PARTITIONS], 
		globalparams_t globalparams, sweepparams_t sweepparams, travstate_t ptravstate, batch_type sourcebaseaddr_kvs, batch_type destbaseaddr_kvs,
		bool_type resetenv, bool_type flush){
	analysis_type analysis_partitionloop = MODEL_BATCHSIZE_KVS / (NUMPIPELINES_PARTITIONUPDATES * WORKBUFFER_SIZE);
	if(enable == OFF){ return; }

	travstate_t ptravstatepp0 = ptravstate;
	travstate_t ptravstatepp1 = ptravstate;
	
	fetchmessage_t fetchmessagepp0;
	fetchmessage_t fetchmessagepp1;
	fetchmessagepp0.chunksize_kvs = -1; fetchmessagepp0.nextoffset_kvs = -1;
	fetchmessagepp1.chunksize_kvs = -1; fetchmessagepp1.nextoffset_kvs = -1;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(resetenv == ON){ cout<<"tradit: reset is ON"<<endl; } else { cout<<"tradit: reset is OFF"<<endl;  }
	#endif 
	
	batch_type offset_kvs = ptravstate.begin_kvs;

	PRIORIT_MAINLOOP: while(offset_kvs < ptravstate.end_kvs){ // NEWCHANGE.
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_partitionloop avg=analysis_partitionloop

		ptravstatepp0.i_kvs = offset_kvs;
		fetchmessagepp0 = fetchkeyvalues(ON, mode, kvdram, vbuffer, vmask, vmask_subp, sourcebuffer, sourcebaseaddr_kvs, ptravstatepp0.i_kvs, WORKBUFFER_SIZE, ptravstatepp0, sweepparams, globalparams);
		if(mode == PROCESSMODE && fetchmessagepp0.nextoffset_kvs != -1){ offset_kvs = fetchmessagepp0.nextoffset_kvs; } else { offset_kvs+=WORKBUFFER_SIZE; } 
		
		// tradreduceandbuffer(ON, kvdram, sourcebuffer, fetchmessagepp0.chunksize_kvs, globalparams);
		tradreduceandbuffer(ON, kvdram, sourcebuffer, fetchmessagepp0.chunksize_kvs, globalstatsbuffer, sweepparams, globalparams);
	}
	return;
}

// dispatch
void
	#ifdef SW
	actsproc::
	#endif 
processit(uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unitBRAMwidth_type vmask_subp[BLOCKRAM_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], globalparams_t globalparams){
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
	buffer_type vmaskbuffersz_kvs = (globalparams.SIZE_REDUCE * VDATA_PACKINGSIZE) / 512;
	
	step_type currentLOP=globalparams.ACTSPARAMS_BEGINLOP;
	resetkeysandvalues(globalstatsbuffer, NUM_PARTITIONS, 0);
	sweepparams = getsweepparams(globalparams, currentLOP, 0);
	travstate_t avtravstate;
	batch_type vmaskoffset_kvs = 0;
	
	step_type tempcurrentLOP = globalparams.ACTSPARAMS_TREEDEPTH;
	#ifdef ENABLERECURSIVEPARTITIONING
	batch_type tempnum_source_partitions = get_num_source_partitions(tempcurrentLOP);
	#else 
	batch_type tempnum_source_partitions = NUM_PARTITIONS;	
	#endif 
	
	avtravstate.begin_kvs = 0;
	avtravstate.end_kvs = avtravstate.begin_kvs + (globalparams.ACTSPARAMS_SRCVSIZE / VECTOR2_SIZE); avtravstate.size_kvs = globalparams.ACTSPARAMS_SRCVSIZE / VECTOR2_SIZE;
	readglobalstats(ON, kvdram, globalstatsbuffer, globalparams.BASEOFFSETKVS_STATSDRAM + deststatsmarker, globalparams); 
	resetvalues(globalstatsbuffer, NUM_PARTITIONS, 0);
	
	batch_type vptrbaseoffset_kvs = globalparams.BASEOFFSETKVS_VERTEXPTR + (globalparams.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE);
	batch_type vdatabaseoffset_kvs = globalparams.BASEOFFSETKVS_VERTICESDATA + (globalparams.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE);
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->print7("### processit:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, avtravstate.begin_kvs * VECTOR_SIZE, avtravstate.end_kvs * VECTOR_SIZE, (avtravstate.end_kvs - avtravstate.begin_kvs) * VECTOR_SIZE, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams.currentLOP)), sweepparams.currentLOP); 							
	#endif
	PROCESSIT_MAINLOOP: for(batch_type source_partition=0; source_partition<tempnum_source_partitions; source_partition+=1){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loop1 avg=analysis_loop1

		batch_type voffset_kvs = source_partition * reducebuffersz * FETFACTOR;
		if(voffset_kvs >= avtravstate.end_kvs){ continue; }
		if(GraphAlgo != PAGERANK) { if(vmask_p[source_partition] == 0){ vmaskoffset_kvs += vmaskbuffersz_kvs; continue; }}
		
		sweepparams.source_partition = source_partition;
		avtravstate.i_kvs = voffset_kvs;
		
		#ifdef _DEBUGMODE_KERNELPRINTS2
		actsutilityobj->print5("### processit:: source_partition", "voffset", "vbegin", "vend", "vskip", source_partition, voffset_kvs * VECTOR_SIZE, avtravstate.begin_kvs * VECTOR_SIZE, avtravstate.size_kvs * VECTOR_SIZE, SRCBUFFER_SIZE * VECTOR_SIZE);
		#endif
		
		loadvmasks(ON, kvdram, vmask, vbuffer, globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskoffset_kvs, vmaskbuffersz_kvs, globalparams); // NOTE: this should come before loadvdata because vbuffer is used as a temp buffer
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
		/* #ifndef ALLVERTEXISACTIVE_ALGORITHM
		resetenv = ON;
		flush = ON; // CRITICAL FIXME. flushing disabled issues with hollywood
		#endif  */
		if(GraphAlgo != PAGERANK){ resetenv = ON; flush = ON; } // CRITICAL NEWCHANGE.
	
		#if defined(ACTS_PARTITION_AND_REDUCE_STRETEGY)
		actit
		#elif defined(BASIC_PARTITION_AND_REDUCE_STRETEGY)
		priorit	
		#elif defined(TRAD_PARTITION_AND_REDUCE_STRETEGY)
		tradit
		#else 
		tradit
		#endif 
		(
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
	actsproc::
	#endif 
partitionit(uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unitBRAMwidth_type vmask_subp[BLOCKRAM_SIZE], globalparams_t globalparams){
	#pragma HLS INLINE
	analysis_type analysis_numllops = 1;
	analysis_type analysis_numsourcepartitions = 1;
	#ifdef TRAD_PARTITION_AND_REDUCE_STRETEGY
	return; // no partitionit for TRAD_PARTITION_AND_REDUCE_STRETEGY
	#endif 
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
	
	PARTITIONIT_MAINLOOP1: for(step_type currentLOP=globalparams.ACTSPARAMS_BEGINLOP + 1; currentLOP<globalparams.ACTSPARAMS_BEGINLOP + 1 + (globalparams.ACTSPARAMS_NUMLOPS-2); currentLOP++){ // REMOVEME.
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
			
			#if defined(ACTS_PARTITION_AND_REDUCE_STRETEGY)
			actit
			#elif defined(BASIC_PARTITION_AND_REDUCE_STRETEGY)
			priorit	
			#elif defined(TRAD_PARTITION_AND_REDUCE_STRETEGY)
			tradit
			#else 
			tradit
			#endif
			(config.enablepartition, PARTITIONMODE,
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
	actsproc::
	#endif 
reduceit(uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unitBRAMwidth_type vmask_subp[BLOCKRAM_SIZE], batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparams){	
	#pragma HLS INLINE
	analysis_type analysis_numllops = 1;
	analysis_type analysis_numsourcepartitions = 1;
	analysis_type analysis_treedepth = TREE_DEPTH;
	
	keyvalue_t globalstatsbuffer[NUM_PARTITIONS]; 
	
	config_t config;
	sweepparams_t sweepparams;
	
	#ifdef ENABLERECURSIVEPARTITIONING
	step_type currentLOP = globalparams.ACTSPARAMS_TREEDEPTH;
	#else 
	step_type currentLOP = globalparams.ACTSPARAMS_TREEDEPTH + 1;
	#endif 
	
	sweepparams = getsweepparams(globalparams, currentLOP, source_partition);
	travstate_t ptravstate = gettravstate(ON, kvdram, globalparams, currentLOP, sourcestatsmarker);

	if(ptravstate.size_kvs == 0){ ptravstate.begin_kvs = 0; ptravstate.end_kvs = 0; config.enablereduce = OFF; }
	else { config.enablereduce = ON; }
	#ifdef _DEBUGMODE_KERNELPRINTS2
	if(ptravstate.size_kvs > 0){ actsutilityobj->print7("### reduceit:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, ptravstate.begin_kvs * VECTOR_SIZE, ptravstate.end_kvs * VECTOR_SIZE, ptravstate.size_kvs * VECTOR_SIZE, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams.currentLOP)), sweepparams.currentLOP); }	
	#endif
	bool_type resetenv; if(source_partition==0){ resetenv = ON; } else { resetenv = OFF; }
	
	#if defined(ACTS_PARTITION_AND_REDUCE_STRETEGY)
	actit
	#elif defined(BASIC_PARTITION_AND_REDUCE_STRETEGY)
	priorit	
	#elif defined(TRAD_PARTITION_AND_REDUCE_STRETEGY)
	tradit
	#else 
	tradit
	#endif
	(config.enablereduce, REDUCEMODE,
			kvdram, sourcebuffer, vbuffer, vmask, vmask_subp, globalstatsbuffer,
			globalparams, sweepparams, ptravstate, sweepparams.worksourcebaseaddress_kvs, sweepparams.workdestbaseaddress_kvs,
			ON, ON); // REMOVEME.
	return;
}

void
	#ifdef SW 
	actsproc::
	#endif 
dispatch(bool_type en_process, bool_type en_partition, bool_type en_reduce, uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unitBRAMwidth_type vmask_subp[BLOCKRAM_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE],
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
dispatch_reduce(uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unitBRAMwidth_type vmask_subp[BLOCKRAM_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], globalparams_t globalparams){
	#pragma HLS INLINE
	analysis_type analysis_loop1 = 1;
	analysis_type analysis_treedepth = TREE_DEPTH;
	#ifdef TRAD_PARTITION_AND_REDUCE_STRETEGY
	return; // no dispatch_reduce for TRAD_PARTITION_AND_REDUCE_STRETEGY
	#endif 
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	unsigned int sourcestatsmarker = 0;
	#ifdef ENABLERECURSIVEPARTITIONING
	for(unsigned int k=0; k<globalparams.ACTSPARAMS_TREEDEPTH-1; k++)
	#else 
	for(unsigned int k=0; k<globalparams.ACTSPARAMS_TREEDEPTH; k++)
	#endif 
	{
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_treedepth avg=analysis_treedepth
		sourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); 
	}
	
	unsigned int vreadoffset_kvs = 0;
	buffer_type reducebuffersz = globalparams.SIZE_REDUCE / 2;
	
	#ifdef ENABLERECURSIVEPARTITIONING
	step_type currentLOP = globalparams.ACTSPARAMS_TREEDEPTH;
	#else 
	step_type currentLOP = globalparams.ACTSPARAMS_TREEDEPTH + 1; // NEWCHANGE.	
	#endif 
	
	#ifdef ENABLERECURSIVEPARTITIONING
	batch_type num_source_partitions = get_num_source_partitions(currentLOP); // NEWCHANGE.
	#else 
	batch_type num_source_partitions = NUM_PARTITIONS;	
	#endif

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

#ifdef ACTSPROC_1by1
extern "C" {
void 
	#ifdef SW 
	actsproc:: 
	#endif
topkernelproc(uint512_dt * kvdram){
#ifndef ACTS_1by1
#pragma HLS INTERFACE m_axi port = kvdram offset = slave bundle = gmem0			
		
#pragma HLS INTERFACE s_axilite port = kvdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATA_PACK variable = kvdram
#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printparameters();
	#endif
	#if defined(_DEBUGMODE_KERNELPRINTS) || defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR: 1 ACTS IN 1 COMPUTEUNITS) Launched... size: "<<GETKEYENTRY(kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN], 0)<<endl; 
	#endif
	
	keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE];
	#pragma HLS array_partition variable = sourcebuffer
	keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer
	unitBRAMwidth_type vmask[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask
	unitBRAMwidth_type vmask_subp[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask_subp
	uint32_type vmask_p[BLOCKRAM_SIZE];
	
	globalparams_t globalparams = getglobalparams(kvdram);
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	
	if(GraphAlgo != PAGERANK){ loadvmask_p(kvdram, vmask_p, globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	
	if(GraphAlgo != PAGERANK){ resetkvdramstats(kvdram, globalparams); }
	
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
	#if defined(REDUCEMODULE)
	if(globalparams.ENABLE_APPLYUPDATESCOMMAND == ON){ 
		#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(ALLVERTEXISACTIVE_ALGORITHM)
		cout<<"topkernelproc: reducing instance ... "<<endl;
		#endif
		dispatch_reduce(kvdram, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, globalparams);
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
#endif 

#ifdef ACTSPROC_2by1
extern "C" {
void 
	#ifdef SW 
	actsproc:: 
	#endif
topkernelproc(uint512_dt * kvdramA, uint512_dt * kvdramB){
#ifndef ACTS_1by1
#pragma HLS INTERFACE m_axi port = kvdramA offset = slave bundle = gmem0		
#pragma HLS INTERFACE m_axi port = kvdramB offset = slave bundle = gmem1
		
#pragma HLS INTERFACE s_axilite port = kvdramA bundle = control
#pragma HLS INTERFACE s_axilite port = kvdramB bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATA_PACK variable = kvdramA
#pragma HLS DATA_PACK variable = kvdramB
#endif 

	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printparameters();
	#endif
	#if defined(_DEBUGMODE_KERNELPRINTS) || defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR: 4 ACTS IN 1 COMPUTEUNITS) Launched... size: "<<GETKEYENTRY(kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN], 0)<<endl; 
	#endif
	
	keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE];
	#pragma HLS array_partition variable = sourcebuffer
	keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer
	unitBRAMwidth_type vmask[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask
	unitBRAMwidth_type vmask_subp[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask_subp
	uint32_type vmask_p[BLOCKRAM_SIZE];
	
	globalparams_t globalparamsA = getglobalparams(kvdramA);
	unsigned int GraphAlgo = globalparamsA.ALGORITHMINFO_GRAPHALGORITHMID;
	if(GraphAlgo != PAGERANK){ loadvmask_p(kvdramA, vmask_p, globalparamsA.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	if(GraphAlgo != PAGERANK){ resetkvdramstats(kvdramA, globalparamsA); }
	
	globalparams_t globalparamsB = getglobalparams(kvdramB);
	if(GraphAlgo != PAGERANK){ loadvmask_p(kvdramB, vmask_p, globalparamsB.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	if(GraphAlgo != PAGERANK){ resetkvdramstats(kvdramB, globalparamsB); }
	
	// process & partition
	#ifdef PROCESSMODULE
	if(globalparamsA.ENABLE_PROCESSCOMMAND == ON){ 
		#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(ALLVERTEXISACTIVE_ALGORITHM)
		cout<<"topkernelproc: processing instance ... "<<endl;
		#endif
		dispatch(ON, OFF, OFF, kvdramA, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, NAp, NAp, globalparamsA);
		dispatch(ON, OFF, OFF, kvdramB, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, NAp, NAp, globalparamsB);
	}
	#endif
	
	// partition
	#ifdef PARTITIONMODULE
	if(globalparamsA.ENABLE_PARTITIONCOMMAND == ON){ 
		#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(ALLVERTEXISACTIVE_ALGORITHM)
		cout<<"topkernelproc: partitioning instance ... "<<endl;
		#endif
		dispatch(OFF, ON, OFF, kvdramA, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, NAp, NAp, globalparamsA);
		dispatch(OFF, ON, OFF, kvdramB, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, NAp, NAp, globalparamsB);
	}
	#endif
	
	// reduce & partition
	#if defined(REDUCEMODULE)
	if(globalparamsA.ENABLE_APPLYUPDATESCOMMAND == ON){ 
		#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(ALLVERTEXISACTIVE_ALGORITHM)
		cout<<"topkernelproc: reducing instance ... "<<endl;
		#endif
		dispatch_reduce(kvdramA, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, globalparamsA);
		dispatch_reduce(kvdramB, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, globalparamsB);
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
#endif 

#ifdef ACTSPROC_4by1
extern "C" {
void 
	#ifdef SW 
	actsproc:: 
	#endif
topkernelproc(uint512_dt * kvdramA, uint512_dt * kvdramB, uint512_dt * kvdramC, uint512_dt * kvdramD){
#ifndef ACTS_1by1
#pragma HLS INTERFACE m_axi port = kvdramA offset = slave bundle = gmem0		
#pragma HLS INTERFACE m_axi port = kvdramB offset = slave bundle = gmem1	
#pragma HLS INTERFACE m_axi port = kvdramC offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdramD offset = slave bundle = gmem3
		
#pragma HLS INTERFACE s_axilite port = kvdramA bundle = control
#pragma HLS INTERFACE s_axilite port = kvdramB bundle = control
#pragma HLS INTERFACE s_axilite port = kvdramC bundle = control
#pragma HLS INTERFACE s_axilite port = kvdramD bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATA_PACK variable = kvdramA
#pragma HLS DATA_PACK variable = kvdramB
#pragma HLS DATA_PACK variable = kvdramC
#pragma HLS DATA_PACK variable = kvdramD
#endif 

	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printparameters();
	#endif
	#if defined(_DEBUGMODE_KERNELPRINTS) || defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR: 4 ACTS IN 1 COMPUTEUNITS) Launched... size: "<<GETKEYENTRY(kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN], 0)<<endl; 
	#endif
	
	keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE];
	#pragma HLS array_partition variable = sourcebuffer
	keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer
	unitBRAMwidth_type vmask[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask
	unitBRAMwidth_type vmask_subp[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask_subp
	uint32_type vmask_p[BLOCKRAM_SIZE];
	
	globalparams_t globalparamsA = getglobalparams(kvdramA);
	unsigned int GraphAlgo = globalparamsA.ALGORITHMINFO_GRAPHALGORITHMID;
	if(GraphAlgo != PAGERANK){ loadvmask_p(kvdramA, vmask_p, globalparamsA.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	if(GraphAlgo != PAGERANK){ resetkvdramstats(kvdramA, globalparamsA); }
	
	globalparams_t globalparamsB = getglobalparams(kvdramB);
	if(GraphAlgo != PAGERANK){ loadvmask_p(kvdramB, vmask_p, globalparamsB.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	if(GraphAlgo != PAGERANK){ resetkvdramstats(kvdramB, globalparamsB); }
	
	globalparams_t globalparamsC = getglobalparams(kvdramC);
	if(GraphAlgo != PAGERANK){ loadvmask_p(kvdramC, vmask_p, globalparamsC.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	if(GraphAlgo != PAGERANK){ resetkvdramstats(kvdramC, globalparamsC); }
	
	globalparams_t globalparamsD = getglobalparams(kvdramD);
	if(GraphAlgo != PAGERANK){ loadvmask_p(kvdramD, vmask_p, globalparamsD.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	if(GraphAlgo != PAGERANK){ resetkvdramstats(kvdramD, globalparamsD); }
	
	// process & partition
	#ifdef PROCESSMODULE
	if(globalparamsA.ENABLE_PROCESSCOMMAND == ON){ 
		#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(ALLVERTEXISACTIVE_ALGORITHM)
		cout<<"topkernelproc: processing instance ... "<<endl;
		#endif
		dispatch(ON, OFF, OFF, kvdramA, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, NAp, NAp, globalparamsA);
		dispatch(ON, OFF, OFF, kvdramB, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, NAp, NAp, globalparamsB);
		dispatch(ON, OFF, OFF, kvdramC, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, NAp, NAp, globalparamsC);
		dispatch(ON, OFF, OFF, kvdramD, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, NAp, NAp, globalparamsD);
	}
	#endif
	
	// partition
	#ifdef PARTITIONMODULE
	if(globalparamsA.ENABLE_PARTITIONCOMMAND == ON){ 
		#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(ALLVERTEXISACTIVE_ALGORITHM)
		cout<<"topkernelproc: partitioning instance ... "<<endl;
		#endif
		dispatch(OFF, ON, OFF, kvdramA, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, NAp, NAp, globalparamsA);
		dispatch(OFF, ON, OFF, kvdramB, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, NAp, NAp, globalparamsB);
		dispatch(OFF, ON, OFF, kvdramC, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, NAp, NAp, globalparamsC);
		dispatch(OFF, ON, OFF, kvdramD, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, NAp, NAp, globalparamsD);
	}
	#endif
	
	// reduce & partition
	#if defined(REDUCEMODULE)
	if(globalparamsA.ENABLE_APPLYUPDATESCOMMAND == ON){ 
		#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(ALLVERTEXISACTIVE_ALGORITHM)
		cout<<"topkernelproc: reducing instance ... "<<endl;
		#endif
		dispatch_reduce(kvdramA, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, globalparamsA);
		dispatch_reduce(kvdramB, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, globalparamsB);
		dispatch_reduce(kvdramC, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, globalparamsC);
		dispatch_reduce(kvdramD, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, globalparamsD);
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
#endif 

#ifdef ACTSPROC_8by1
extern "C" {
void 
	#ifdef SW 
	actsproc:: 
	#endif
topkernelproc(uint512_dt * kvdramA, uint512_dt * kvdramB, uint512_dt * kvdramC, uint512_dt * kvdramD, uint512_dt * kvdramE, uint512_dt * kvdramF, uint512_dt * kvdramG, uint512_dt * kvdramH){
#ifndef ACTS_1by1
#pragma HLS INTERFACE m_axi port = kvdramA offset = slave bundle = gmem0		
#pragma HLS INTERFACE m_axi port = kvdramB offset = slave bundle = gmem1	
#pragma HLS INTERFACE m_axi port = kvdramC offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdramD offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = kvdramE offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = kvdramF offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = kvdramG offset = slave bundle = gmem6
#pragma HLS INTERFACE m_axi port = kvdramH offset = slave bundle = gmem7
		
#pragma HLS INTERFACE s_axilite port = kvdramA bundle = control
#pragma HLS INTERFACE s_axilite port = kvdramB bundle = control
#pragma HLS INTERFACE s_axilite port = kvdramC bundle = control
#pragma HLS INTERFACE s_axilite port = kvdramD bundle = control
#pragma HLS INTERFACE s_axilite port = kvdramE bundle = control
#pragma HLS INTERFACE s_axilite port = kvdramF bundle = control
#pragma HLS INTERFACE s_axilite port = kvdramG bundle = control
#pragma HLS INTERFACE s_axilite port = kvdramH bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATA_PACK variable = kvdramA
#pragma HLS DATA_PACK variable = kvdramB
#pragma HLS DATA_PACK variable = kvdramC
#pragma HLS DATA_PACK variable = kvdramD
#pragma HLS DATA_PACK variable = kvdramE
#pragma HLS DATA_PACK variable = kvdramF
#pragma HLS DATA_PACK variable = kvdramG
#pragma HLS DATA_PACK variable = kvdramH
#endif 

	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printparameters();
	#endif
	#if defined(_DEBUGMODE_KERNELPRINTS) || defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR: 8 ACTS IN 1 COMPUTEUNITS) Launched... size: "<<GETKEYENTRY(kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN], 0)<<endl; 
	#endif
	
	keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE];
	#pragma HLS array_partition variable = sourcebuffer
	keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer
	unitBRAMwidth_type vmask[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask
	unitBRAMwidth_type vmask_subp[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask_subp
	uint32_type vmask_p[BLOCKRAM_SIZE];
	
	globalparams_t globalparamsA = getglobalparams(kvdramA);
	unsigned int GraphAlgo = globalparamsA.ALGORITHMINFO_GRAPHALGORITHMID;
	if(GraphAlgo != PAGERANK){ loadvmask_p(kvdramA, vmask_p, globalparamsA.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	if(GraphAlgo != PAGERANK){ resetkvdramstats(kvdramA, globalparamsA); }
	
	globalparams_t globalparamsB = getglobalparams(kvdramB);
	if(GraphAlgo != PAGERANK){ loadvmask_p(kvdramB, vmask_p, globalparamsB.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	if(GraphAlgo != PAGERANK){ resetkvdramstats(kvdramB, globalparamsB); }
	
	globalparams_t globalparamsC = getglobalparams(kvdramC);
	if(GraphAlgo != PAGERANK){ loadvmask_p(kvdramC, vmask_p, globalparamsC.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	if(GraphAlgo != PAGERANK){ resetkvdramstats(kvdramC, globalparamsC); }
	
	globalparams_t globalparamsD = getglobalparams(kvdramD);
	if(GraphAlgo != PAGERANK){ loadvmask_p(kvdramD, vmask_p, globalparamsD.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	if(GraphAlgo != PAGERANK){ resetkvdramstats(kvdramD, globalparamsD); }
	
	globalparams_t globalparamsE = getglobalparams(kvdramE);
	if(GraphAlgo != PAGERANK){ loadvmask_p(kvdramE, vmask_p, globalparamsE.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	if(GraphAlgo != PAGERANK){ resetkvdramstats(kvdramE, globalparamsE); }
	
	globalparams_t globalparamsF = getglobalparams(kvdramF);
	if(GraphAlgo != PAGERANK){ loadvmask_p(kvdramF, vmask_p, globalparamsF.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	if(GraphAlgo != PAGERANK){ resetkvdramstats(kvdramF, globalparamsF); }
	
	globalparams_t globalparamsG = getglobalparams(kvdramG);
	if(GraphAlgo != PAGERANK){ loadvmask_p(kvdramG, vmask_p, globalparamsG.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	if(GraphAlgo != PAGERANK){ resetkvdramstats(kvdramG, globalparamsG); }
	
	globalparams_t globalparamsH = getglobalparams(kvdramH);
	if(GraphAlgo != PAGERANK){ loadvmask_p(kvdramH, vmask_p, globalparamsH.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	if(GraphAlgo != PAGERANK){ resetkvdramstats(kvdramH, globalparamsH); }
	
	// process & partition
	#ifdef PROCESSMODULE
	if(globalparamsA.ENABLE_PROCESSCOMMAND == ON){ 
		#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(ALLVERTEXISACTIVE_ALGORITHM)
		cout<<"topkernelproc: processing instance ... "<<endl;
		#endif
		dispatch(ON, OFF, OFF, kvdramA, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, NAp, NAp, globalparamsA);
		dispatch(ON, OFF, OFF, kvdramB, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, NAp, NAp, globalparamsB);
		dispatch(ON, OFF, OFF, kvdramC, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, NAp, NAp, globalparamsC);
		dispatch(ON, OFF, OFF, kvdramD, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, NAp, NAp, globalparamsD);
		dispatch(ON, OFF, OFF, kvdramE, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, NAp, NAp, globalparamsE);
		dispatch(ON, OFF, OFF, kvdramF, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, NAp, NAp, globalparamsF);
		dispatch(ON, OFF, OFF, kvdramG, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, NAp, NAp, globalparamsG);
		dispatch(ON, OFF, OFF, kvdramH, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, NAp, NAp, globalparamsH);
	}
	#endif
	
	// partition
	#ifdef PARTITIONMODULE
	if(globalparamsA.ENABLE_PARTITIONCOMMAND == ON){ 
		#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(ALLVERTEXISACTIVE_ALGORITHM)
		cout<<"topkernelproc: partitioning instance ... "<<endl;
		#endif
		dispatch(OFF, ON, OFF, kvdramA, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, NAp, NAp, globalparamsA);
		dispatch(OFF, ON, OFF, kvdramB, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, NAp, NAp, globalparamsB);
		dispatch(OFF, ON, OFF, kvdramC, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, NAp, NAp, globalparamsC);
		dispatch(OFF, ON, OFF, kvdramD, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, NAp, NAp, globalparamsD);
		dispatch(OFF, ON, OFF, kvdramE, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, NAp, NAp, globalparamsE);
		dispatch(OFF, ON, OFF, kvdramF, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, NAp, NAp, globalparamsF);
		dispatch(OFF, ON, OFF, kvdramG, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, NAp, NAp, globalparamsG);
		dispatch(OFF, ON, OFF, kvdramH, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, NAp, NAp, globalparamsH);
	}
	#endif
	
	// reduce & partition
	#if defined(REDUCEMODULE)
	if(globalparamsA.ENABLE_APPLYUPDATESCOMMAND == ON){ 
		#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(ALLVERTEXISACTIVE_ALGORITHM)
		cout<<"topkernelproc: reducing instance ... "<<endl;
		#endif
		dispatch_reduce(kvdramA, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, globalparamsA);
		dispatch_reduce(kvdramB, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, globalparamsB);
		dispatch_reduce(kvdramC, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, globalparamsC);
		dispatch_reduce(kvdramD, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, globalparamsD);
		dispatch_reduce(kvdramE, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, globalparamsE);
		dispatch_reduce(kvdramF, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, globalparamsF);
		dispatch_reduce(kvdramG, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, globalparamsG);
		dispatch_reduce(kvdramH, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, globalparamsH);
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
#endif 

#endif 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// INCLUDE SYNC FILE ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined(FPGA_IMPL) & defined(INCLUDESYNC) 
#define NUMPIPELINES_SYNC 1 // 2 // CRITICAL FIXME. CORRECTME.
#if NUMPIPELINES_SYNC==1
#define SUP0
#endif 
#if NUMPIPELINES_SYNC==2
#define SUP0
#define SUP1
#endif

#ifdef SW
actssync::actssync(){ actsutilityobj = new actsutility(); }
actssync::~actssync(){}
#endif
#ifdef SWEMU
actssync::actssync(){}
actssync::~actssync(){}
actsutility * actsutilityobj = new actsutility();
#endif 

// functions (basic)
unsigned int
	#ifdef SW 
	actssync::
	#endif 
SYNC_amin(unsigned int val1, unsigned int val2){
	if(val1 < val2){ return val1; }
	else { return val2; }
}
unsigned int
	#ifdef SW 
	actssync::
	#endif 
SYNC_aplus(unsigned int val1, unsigned int val2){
	return val1 + val2;
}

// functions (bit manipulation)
unsigned int 
	#ifdef SW 
	actssync::
	#endif
SYNC_GETMASK_UINT(unsigned int index, unsigned int size){
	unsigned int A = ((1 << (size)) - 1);
	unsigned int B = A << index;
	return B;
}
unsigned int 
	#ifdef SW 
	actssync::
	#endif
SYNC_READFROM_UINT(unsigned int data, unsigned int index, unsigned int size){ 
	return (((data) & SYNC_GETMASK_UINT((index), (size))) >> (index)); 
}
void
	#ifdef SW 
	actssync::
	#endif
SYNC_WRITETO_UINT(unsigned int * data, unsigned int index, unsigned int size, unsigned int value){
	unsigned int tempdata = *data;
	unsigned int A = ((value) << (index));
	unsigned int B = (~SYNC_GETMASK_UINT((index), (size)));
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
	actssync::
	#endif 
SYNC_GETKV(keyvalue_buffer_t data){
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
	actssync::
	#endif 
SYNC_GETKV(keyvalue_t data){
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
	actssync::
	#endif 
SYNC_GETK(uint32_type data){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	return data.range(SIZEOF_KEY - 1, 0);
	#else
	return data;
	#endif
}
value_t 
	#ifdef SW 
	actssync::
	#endif 
SYNC_GETV(uint32_type data){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	return data.range(SIZEOF_VALUE - 1, 0);
	#else
	return data;
	#endif
}
keyvalue_t 
	#ifdef SW 
	actssync::
	#endif 
SYNC_GETKV2(keyvalue_vbuffer_t data){
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
	actssync::
	#endif 
SYNC_GETKV2(keyvalue_t data){
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
	actssync::
	#endif 
SYNC_GETK2(uint32_type data){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	return data.range(SIZEOF_VDATAKEY - 1, 0);
	#else
	return data;
	#endif
}
value_t 
	#ifdef SW 
	actssync::
	#endif 
SYNC_GETV2(uint32_type data){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	return data.range(SIZEOF_VDATAKEY - 1, 0);
	#else
	return data;
	#endif
}
uint32_type 
	#ifdef SW 
	actssync::
	#endif 
convertvmasktouint32(unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unsigned int index){
	uint32_type res;
	#ifdef _WIDEWORD
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
	SYNC_WRITETO_UINT(&res, 0, 1, vmask[index].data[0].key);
	SYNC_WRITETO_UINT(&res, 1, 1, vmask[index].data[0].value);
	SYNC_WRITETO_UINT(&res, 2, 1, vmask[index].data[1].key);
	SYNC_WRITETO_UINT(&res, 3, 1, vmask[index].data[1].value);
	SYNC_WRITETO_UINT(&res, 4, 1, vmask[index].data[2].key);
	SYNC_WRITETO_UINT(&res, 5, 1, vmask[index].data[2].value);
	SYNC_WRITETO_UINT(&res, 6, 1, vmask[index].data[3].key);
	SYNC_WRITETO_UINT(&res, 7, 1, vmask[index].data[3].value);
	SYNC_WRITETO_UINT(&res, 8, 1, vmask[index].data[4].key);
	SYNC_WRITETO_UINT(&res, 9, 1, vmask[index].data[4].value);
	SYNC_WRITETO_UINT(&res, 10, 1, vmask[index].data[5].key);
	SYNC_WRITETO_UINT(&res, 11, 1, vmask[index].data[5].value);
	SYNC_WRITETO_UINT(&res, 12, 1, vmask[index].data[6].key);
	SYNC_WRITETO_UINT(&res, 13, 1, vmask[index].data[6].value);
	SYNC_WRITETO_UINT(&res, 14, 1, vmask[index].data[7].key);
	SYNC_WRITETO_UINT(&res, 15, 1, vmask[index].data[7].value);
	SYNC_WRITETO_UINT(&res, 16, 1, vmask[index].data[8].key);
	SYNC_WRITETO_UINT(&res, 17, 1, vmask[index].data[8].value);
	SYNC_WRITETO_UINT(&res, 18, 1, vmask[index].data[9].key);
	SYNC_WRITETO_UINT(&res, 19, 1, vmask[index].data[9].value);
	SYNC_WRITETO_UINT(&res, 20, 1, vmask[index].data[10].key);
	SYNC_WRITETO_UINT(&res, 21, 1, vmask[index].data[10].value);
	SYNC_WRITETO_UINT(&res, 22, 1, vmask[index].data[11].key);
	SYNC_WRITETO_UINT(&res, 23, 1, vmask[index].data[11].value);
	SYNC_WRITETO_UINT(&res, 24, 1, vmask[index].data[12].key);
	SYNC_WRITETO_UINT(&res, 25, 1, vmask[index].data[12].value);
	SYNC_WRITETO_UINT(&res, 26, 1, vmask[index].data[13].key);
	SYNC_WRITETO_UINT(&res, 27, 1, vmask[index].data[13].value);
	SYNC_WRITETO_UINT(&res, 28, 1, vmask[index].data[14].key);
	SYNC_WRITETO_UINT(&res, 29, 1, vmask[index].data[14].value);
	SYNC_WRITETO_UINT(&res, 30, 1, vmask[index].data[15].key);
	SYNC_WRITETO_UINT(&res, 31, 1, vmask[index].data[15].value);
	#endif
	return res;
}
keyy_t 
	#ifdef SW 
	actssync::
	#endif 
SYNC_GETKEYENTRY(uint512_dt data, unsigned int v){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	return data.range(32 * ((v * 2) + 1) - 1, (v * 2) * 32);
	#else 
	return data.data[v].key;	
	#endif
}
value_t 
	#ifdef SW 
	actssync::
	#endif 
GETVALUEENTRY(uint512_dt data, unsigned int v){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	return data.range(32 * (((v * 2) + 1) + 1) - 1, (v * 2 + 1) * 32);
	#else 
	return data.data[v].value;	
	#endif
}

// functions (actssync utilities)
batch_type
	#ifdef SW 
	actssync::
	#endif 
SYNC_get_num_source_partitions(step_type currentLOP){
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
	actssync::
	#endif 
SYNC_getglobalparams(uint512_dt * kvdram){
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
	actsutilityobj->printglobalparameters("actssync::SYNC_getglobalparams:: printing global parameters", globalparams);
	#endif
	return globalparams;
}
travstate_t 
	#ifdef SW 
	actssync::
	#endif 
SYNC_gettravstate(bool_type enable, uint512_dt * kvdram, globalparams_t globalparams, step_type currentLOP, batch_type sourcestatsmarker){			
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
			
		
		if(globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker >= PADDEDKVSOURCEDRAMSZ){ cout<<"SYNC_gettravstate:: ERROR: globalparams.BASEOFFSETKVS_STATSDRAM("<<globalparams.BASEOFFSETKVS_STATSDRAM<<") + sourcestatsmarker("<<sourcestatsmarker<<") >= PADDEDKVSOURCEDRAMSZ_KVS("<<PADDEDKVSOURCEDRAMSZ_KVS<<")"<<endl; exit(EXIT_FAILURE); }
		
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

// functions (accessors)
void // 
	#ifdef SW 
	actssync::
	#endif 
readvdata(bool_type enable1, bool_type enable2, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	#pragma HLS function_instantiate variable=kvdram
	if(enable1 == OFF || enable2 == OFF){ return; }
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
		
		buffer[begincol + 0][bufferoffset_kvs + i] = SYNC_GETKV2(mykeyvalue0);
		buffer[begincol + 1][bufferoffset_kvs + i] = SYNC_GETKV2(mykeyvalue1);
		buffer[begincol + 2][bufferoffset_kvs + i] = SYNC_GETKV2(mykeyvalue2);
		buffer[begincol + 3][bufferoffset_kvs + i] = SYNC_GETKV2(mykeyvalue3);
		buffer[begincol + 4][bufferoffset_kvs + i] = SYNC_GETKV2(mykeyvalue4);
		buffer[begincol + 5][bufferoffset_kvs + i] = SYNC_GETKV2(mykeyvalue5);
		buffer[begincol + 6][bufferoffset_kvs + i] = SYNC_GETKV2(mykeyvalue6);
		buffer[begincol + 7][bufferoffset_kvs + i] = SYNC_GETKV2(mykeyvalue7);
		
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<< TIMINGRESULTSCOLOR<<"readvdata:: vertices read: offset: "<<dramoffset_kvs * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl;
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(dramoffset_kvs >= globalparams.BASEOFFSETKVS_VERTEXPTR_ && dramoffset_kvs < globalparams.BASEOFFSETKVS_VERTEXPTR + VERTEXPTRSSZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"readvdata:: vertices read: offset: "<<(dramoffset_kvs - globalparams.BASEOFFSETKVS_VERTEXPTR) * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs - globalparams.BASEOFFSETKVS_VERTEXPTR) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	if(dramoffset_kvs >= globalparams.BASEOFFSETKVS_VERTICESDATA_ && dramoffset_kvs < globalparams.BASEOFFSETKVS_VERTICESDATA + VERTICESDATASZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"readvdata:: vertices read: offset: "<<(dramoffset_kvs - globalparams.BASEOFFSETKVS_VERTICESDATA) * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs - globalparams.BASEOFFSETKVS_VERTICESDATA) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	#endif
	return;
}

// functions (synchronize)
value_t 
	#ifdef SW 
	actssync::
	#endif 
applyfunc(value_t vtemp, value_t res, unsigned int GraphIter, unsigned int GraphAlgo){
	value_t temp = 0;
	
	if(GraphAlgo == PAGERANK){
		// temp = vtemp + res;
		// temp = 0.5 + (1 - 0.5)*res / vdeg; // IDEAL
		temp = res;
	} else if(GraphAlgo == BFS){
		temp = res;
	} else if(GraphAlgo == SSSP){
		temp = SYNC_amin(vtemp, res);
	} else {
		temp = NAp;
	}
	return temp;
}

value_t 
	#ifdef SW 
	actssync::
	#endif 
mergefunc(value_t value1, value_t value2, unsigned int GraphAlgo){
	value_t res = 0;
	
	if(GraphAlgo == PAGERANK){
		res = value1 + value2;
	} else if(GraphAlgo == BFS){
		res = SYNC_amin(value1, value2);
	} else if(GraphAlgo == SSSP){
		res = SYNC_amin(value1, value2);
	} else {
		res = NAp;
	}
	return res;
}

void
	#ifdef SW 
	actssync::
	#endif
readandsynchronize(bool_type enable1, bool_type enable2, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3, keyvalue_vbuffer_t res[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type dramoffset_kvs, globalparams_t globalparams){
	#pragma HLS function_instantiate variable=res
	if(enable1 == OFF || enable2 == OFF){ return; }
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
	cout<<"readandsynchronize: readandsynchronize function called."<<endl;
	#endif 
	analysis_type analysis_loopcount = REDUCESZ / 2;
	
	buffer_type reducebuffersz = globalparams.SIZE_REDUCE / 2;
	
	keyvalue_t keyvalue0_vault0;
	keyvalue_t keyvalue1_vault0;
	keyvalue_t keyvalue2_vault0;
	keyvalue_t keyvalue3_vault0;
	keyvalue_t keyvalue4_vault0;
	keyvalue_t keyvalue5_vault0;
	keyvalue_t keyvalue6_vault0;
	keyvalue_t keyvalue7_vault0;
	
	keyvalue_t keyvalue0_vault1;
	keyvalue_t keyvalue1_vault1;
	keyvalue_t keyvalue2_vault1;
	keyvalue_t keyvalue3_vault1;
	keyvalue_t keyvalue4_vault1;
	keyvalue_t keyvalue5_vault1;
	keyvalue_t keyvalue6_vault1;
	keyvalue_t keyvalue7_vault1;
	
	keyvalue_t keyvalue0_vault2;
	keyvalue_t keyvalue1_vault2;
	keyvalue_t keyvalue2_vault2;
	keyvalue_t keyvalue3_vault2;
	keyvalue_t keyvalue4_vault2;
	keyvalue_t keyvalue5_vault2;
	keyvalue_t keyvalue6_vault2;
	keyvalue_t keyvalue7_vault2;
	
	keyvalue_t keyvalue0_vault3;
	keyvalue_t keyvalue1_vault3;
	keyvalue_t keyvalue2_vault3;
	keyvalue_t keyvalue3_vault3;
	keyvalue_t keyvalue4_vault3;
	keyvalue_t keyvalue5_vault3;
	keyvalue_t keyvalue6_vault3;
	keyvalue_t keyvalue7_vault3;
	
	keyvalue_t keyvalue0_vault4;
	keyvalue_t keyvalue1_vault4;
	keyvalue_t keyvalue2_vault4;
	keyvalue_t keyvalue3_vault4;
	keyvalue_t keyvalue4_vault4;
	keyvalue_t keyvalue5_vault4;
	keyvalue_t keyvalue6_vault4;
	keyvalue_t keyvalue7_vault4;
	
	
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
	
	keyvalue_t mykeyvalue20;
	keyvalue_t mykeyvalue21;
	keyvalue_t mykeyvalue22;
	keyvalue_t mykeyvalue23;
	keyvalue_t mykeyvalue24;
	keyvalue_t mykeyvalue25;
	keyvalue_t mykeyvalue26;
	keyvalue_t mykeyvalue27;
	
	keyvalue_t mykeyvalue30;
	keyvalue_t mykeyvalue31;
	keyvalue_t mykeyvalue32;
	keyvalue_t mykeyvalue33;
	keyvalue_t mykeyvalue34;
	keyvalue_t mykeyvalue35;
	keyvalue_t mykeyvalue36;
	keyvalue_t mykeyvalue37;
	
	
	for (buffer_type i=0; i<reducebuffersz; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		mykeyvalue00.key = kvdram0[dramoffset_kvs + i].range(31, 0); 
		mykeyvalue00.value = kvdram0[dramoffset_kvs + i].range(63, 32); 
		mykeyvalue01.key = kvdram0[dramoffset_kvs + i].range(95, 64); 
		mykeyvalue01.value = kvdram0[dramoffset_kvs + i].range(127, 96); 
		mykeyvalue02.key = kvdram0[dramoffset_kvs + i].range(159, 128); 
		mykeyvalue02.value = kvdram0[dramoffset_kvs + i].range(191, 160); 
		mykeyvalue03.key = kvdram0[dramoffset_kvs + i].range(223, 192); 
		mykeyvalue03.value = kvdram0[dramoffset_kvs + i].range(255, 224); 
		mykeyvalue04.key = kvdram0[dramoffset_kvs + i].range(287, 256); 
		mykeyvalue04.value = kvdram0[dramoffset_kvs + i].range(319, 288); 
		mykeyvalue05.key = kvdram0[dramoffset_kvs + i].range(351, 320); 
		mykeyvalue05.value = kvdram0[dramoffset_kvs + i].range(383, 352); 
		mykeyvalue06.key = kvdram0[dramoffset_kvs + i].range(415, 384); 
		mykeyvalue06.value = kvdram0[dramoffset_kvs + i].range(447, 416); 
		mykeyvalue07.key = kvdram0[dramoffset_kvs + i].range(479, 448); 
		mykeyvalue07.value = kvdram0[dramoffset_kvs + i].range(511, 480); 
		#else 
		mykeyvalue00.key = kvdram0[dramoffset_kvs + i].data[0].key; 
		mykeyvalue00.value = kvdram0[dramoffset_kvs + i].data[0].value; 
		mykeyvalue01.key = kvdram0[dramoffset_kvs + i].data[1].key; 
		mykeyvalue01.value = kvdram0[dramoffset_kvs + i].data[1].value; 
		mykeyvalue02.key = kvdram0[dramoffset_kvs + i].data[2].key; 
		mykeyvalue02.value = kvdram0[dramoffset_kvs + i].data[2].value; 
		mykeyvalue03.key = kvdram0[dramoffset_kvs + i].data[3].key; 
		mykeyvalue03.value = kvdram0[dramoffset_kvs + i].data[3].value; 
		mykeyvalue04.key = kvdram0[dramoffset_kvs + i].data[4].key; 
		mykeyvalue04.value = kvdram0[dramoffset_kvs + i].data[4].value; 
		mykeyvalue05.key = kvdram0[dramoffset_kvs + i].data[5].key; 
		mykeyvalue05.value = kvdram0[dramoffset_kvs + i].data[5].value; 
		mykeyvalue06.key = kvdram0[dramoffset_kvs + i].data[6].key; 
		mykeyvalue06.value = kvdram0[dramoffset_kvs + i].data[6].value; 
		mykeyvalue07.key = kvdram0[dramoffset_kvs + i].data[7].key; 
		mykeyvalue07.value = kvdram0[dramoffset_kvs + i].data[7].value; 
		#endif
		keyvalue0_vault1 = mykeyvalue00; 
		keyvalue1_vault1 = mykeyvalue01; 
		keyvalue2_vault1 = mykeyvalue02; 
		keyvalue3_vault1 = mykeyvalue03; 
		keyvalue4_vault1 = mykeyvalue04; 
		keyvalue5_vault1 = mykeyvalue05; 
		keyvalue6_vault1 = mykeyvalue06; 
		keyvalue7_vault1 = mykeyvalue07; 
			
		
		#ifdef _WIDEWORD
		mykeyvalue10.key = kvdram1[dramoffset_kvs + i].range(31, 0); 
		mykeyvalue10.value = kvdram1[dramoffset_kvs + i].range(63, 32); 
		mykeyvalue11.key = kvdram1[dramoffset_kvs + i].range(95, 64); 
		mykeyvalue11.value = kvdram1[dramoffset_kvs + i].range(127, 96); 
		mykeyvalue12.key = kvdram1[dramoffset_kvs + i].range(159, 128); 
		mykeyvalue12.value = kvdram1[dramoffset_kvs + i].range(191, 160); 
		mykeyvalue13.key = kvdram1[dramoffset_kvs + i].range(223, 192); 
		mykeyvalue13.value = kvdram1[dramoffset_kvs + i].range(255, 224); 
		mykeyvalue14.key = kvdram1[dramoffset_kvs + i].range(287, 256); 
		mykeyvalue14.value = kvdram1[dramoffset_kvs + i].range(319, 288); 
		mykeyvalue15.key = kvdram1[dramoffset_kvs + i].range(351, 320); 
		mykeyvalue15.value = kvdram1[dramoffset_kvs + i].range(383, 352); 
		mykeyvalue16.key = kvdram1[dramoffset_kvs + i].range(415, 384); 
		mykeyvalue16.value = kvdram1[dramoffset_kvs + i].range(447, 416); 
		mykeyvalue17.key = kvdram1[dramoffset_kvs + i].range(479, 448); 
		mykeyvalue17.value = kvdram1[dramoffset_kvs + i].range(511, 480); 
		#else 
		mykeyvalue10.key = kvdram1[dramoffset_kvs + i].data[0].key; 
		mykeyvalue10.value = kvdram1[dramoffset_kvs + i].data[0].value; 
		mykeyvalue11.key = kvdram1[dramoffset_kvs + i].data[1].key; 
		mykeyvalue11.value = kvdram1[dramoffset_kvs + i].data[1].value; 
		mykeyvalue12.key = kvdram1[dramoffset_kvs + i].data[2].key; 
		mykeyvalue12.value = kvdram1[dramoffset_kvs + i].data[2].value; 
		mykeyvalue13.key = kvdram1[dramoffset_kvs + i].data[3].key; 
		mykeyvalue13.value = kvdram1[dramoffset_kvs + i].data[3].value; 
		mykeyvalue14.key = kvdram1[dramoffset_kvs + i].data[4].key; 
		mykeyvalue14.value = kvdram1[dramoffset_kvs + i].data[4].value; 
		mykeyvalue15.key = kvdram1[dramoffset_kvs + i].data[5].key; 
		mykeyvalue15.value = kvdram1[dramoffset_kvs + i].data[5].value; 
		mykeyvalue16.key = kvdram1[dramoffset_kvs + i].data[6].key; 
		mykeyvalue16.value = kvdram1[dramoffset_kvs + i].data[6].value; 
		mykeyvalue17.key = kvdram1[dramoffset_kvs + i].data[7].key; 
		mykeyvalue17.value = kvdram1[dramoffset_kvs + i].data[7].value; 
		#endif
		keyvalue0_vault2.key = mergefunc(keyvalue0_vault1.key, mykeyvalue10.key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue0_vault2.value = mergefunc(keyvalue0_vault1.value, mykeyvalue10.value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue1_vault2.key = mergefunc(keyvalue1_vault1.key, mykeyvalue11.key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue1_vault2.value = mergefunc(keyvalue1_vault1.value, mykeyvalue11.value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue2_vault2.key = mergefunc(keyvalue2_vault1.key, mykeyvalue12.key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue2_vault2.value = mergefunc(keyvalue2_vault1.value, mykeyvalue12.value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue3_vault2.key = mergefunc(keyvalue3_vault1.key, mykeyvalue13.key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue3_vault2.value = mergefunc(keyvalue3_vault1.value, mykeyvalue13.value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue4_vault2.key = mergefunc(keyvalue4_vault1.key, mykeyvalue14.key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue4_vault2.value = mergefunc(keyvalue4_vault1.value, mykeyvalue14.value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue5_vault2.key = mergefunc(keyvalue5_vault1.key, mykeyvalue15.key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue5_vault2.value = mergefunc(keyvalue5_vault1.value, mykeyvalue15.value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue6_vault2.key = mergefunc(keyvalue6_vault1.key, mykeyvalue16.key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue6_vault2.value = mergefunc(keyvalue6_vault1.value, mykeyvalue16.value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue7_vault2.key = mergefunc(keyvalue7_vault1.key, mykeyvalue17.key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue7_vault2.value = mergefunc(keyvalue7_vault1.value, mykeyvalue17.value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		
		
		#ifdef _WIDEWORD
		mykeyvalue20.key = kvdram2[dramoffset_kvs + i].range(31, 0); 
		mykeyvalue20.value = kvdram2[dramoffset_kvs + i].range(63, 32); 
		mykeyvalue21.key = kvdram2[dramoffset_kvs + i].range(95, 64); 
		mykeyvalue21.value = kvdram2[dramoffset_kvs + i].range(127, 96); 
		mykeyvalue22.key = kvdram2[dramoffset_kvs + i].range(159, 128); 
		mykeyvalue22.value = kvdram2[dramoffset_kvs + i].range(191, 160); 
		mykeyvalue23.key = kvdram2[dramoffset_kvs + i].range(223, 192); 
		mykeyvalue23.value = kvdram2[dramoffset_kvs + i].range(255, 224); 
		mykeyvalue24.key = kvdram2[dramoffset_kvs + i].range(287, 256); 
		mykeyvalue24.value = kvdram2[dramoffset_kvs + i].range(319, 288); 
		mykeyvalue25.key = kvdram2[dramoffset_kvs + i].range(351, 320); 
		mykeyvalue25.value = kvdram2[dramoffset_kvs + i].range(383, 352); 
		mykeyvalue26.key = kvdram2[dramoffset_kvs + i].range(415, 384); 
		mykeyvalue26.value = kvdram2[dramoffset_kvs + i].range(447, 416); 
		mykeyvalue27.key = kvdram2[dramoffset_kvs + i].range(479, 448); 
		mykeyvalue27.value = kvdram2[dramoffset_kvs + i].range(511, 480); 
		#else 
		mykeyvalue20.key = kvdram2[dramoffset_kvs + i].data[0].key; 
		mykeyvalue20.value = kvdram2[dramoffset_kvs + i].data[0].value; 
		mykeyvalue21.key = kvdram2[dramoffset_kvs + i].data[1].key; 
		mykeyvalue21.value = kvdram2[dramoffset_kvs + i].data[1].value; 
		mykeyvalue22.key = kvdram2[dramoffset_kvs + i].data[2].key; 
		mykeyvalue22.value = kvdram2[dramoffset_kvs + i].data[2].value; 
		mykeyvalue23.key = kvdram2[dramoffset_kvs + i].data[3].key; 
		mykeyvalue23.value = kvdram2[dramoffset_kvs + i].data[3].value; 
		mykeyvalue24.key = kvdram2[dramoffset_kvs + i].data[4].key; 
		mykeyvalue24.value = kvdram2[dramoffset_kvs + i].data[4].value; 
		mykeyvalue25.key = kvdram2[dramoffset_kvs + i].data[5].key; 
		mykeyvalue25.value = kvdram2[dramoffset_kvs + i].data[5].value; 
		mykeyvalue26.key = kvdram2[dramoffset_kvs + i].data[6].key; 
		mykeyvalue26.value = kvdram2[dramoffset_kvs + i].data[6].value; 
		mykeyvalue27.key = kvdram2[dramoffset_kvs + i].data[7].key; 
		mykeyvalue27.value = kvdram2[dramoffset_kvs + i].data[7].value; 
		#endif
		keyvalue0_vault3.key = mergefunc(keyvalue0_vault2.key, mykeyvalue20.key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue0_vault3.value = mergefunc(keyvalue0_vault2.value, mykeyvalue20.value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue1_vault3.key = mergefunc(keyvalue1_vault2.key, mykeyvalue21.key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue1_vault3.value = mergefunc(keyvalue1_vault2.value, mykeyvalue21.value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue2_vault3.key = mergefunc(keyvalue2_vault2.key, mykeyvalue22.key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue2_vault3.value = mergefunc(keyvalue2_vault2.value, mykeyvalue22.value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue3_vault3.key = mergefunc(keyvalue3_vault2.key, mykeyvalue23.key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue3_vault3.value = mergefunc(keyvalue3_vault2.value, mykeyvalue23.value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue4_vault3.key = mergefunc(keyvalue4_vault2.key, mykeyvalue24.key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue4_vault3.value = mergefunc(keyvalue4_vault2.value, mykeyvalue24.value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue5_vault3.key = mergefunc(keyvalue5_vault2.key, mykeyvalue25.key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue5_vault3.value = mergefunc(keyvalue5_vault2.value, mykeyvalue25.value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue6_vault3.key = mergefunc(keyvalue6_vault2.key, mykeyvalue26.key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue6_vault3.value = mergefunc(keyvalue6_vault2.value, mykeyvalue26.value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue7_vault3.key = mergefunc(keyvalue7_vault2.key, mykeyvalue27.key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue7_vault3.value = mergefunc(keyvalue7_vault2.value, mykeyvalue27.value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		
		
		#ifdef _WIDEWORD
		mykeyvalue30.key = kvdram3[dramoffset_kvs + i].range(31, 0); 
		mykeyvalue30.value = kvdram3[dramoffset_kvs + i].range(63, 32); 
		mykeyvalue31.key = kvdram3[dramoffset_kvs + i].range(95, 64); 
		mykeyvalue31.value = kvdram3[dramoffset_kvs + i].range(127, 96); 
		mykeyvalue32.key = kvdram3[dramoffset_kvs + i].range(159, 128); 
		mykeyvalue32.value = kvdram3[dramoffset_kvs + i].range(191, 160); 
		mykeyvalue33.key = kvdram3[dramoffset_kvs + i].range(223, 192); 
		mykeyvalue33.value = kvdram3[dramoffset_kvs + i].range(255, 224); 
		mykeyvalue34.key = kvdram3[dramoffset_kvs + i].range(287, 256); 
		mykeyvalue34.value = kvdram3[dramoffset_kvs + i].range(319, 288); 
		mykeyvalue35.key = kvdram3[dramoffset_kvs + i].range(351, 320); 
		mykeyvalue35.value = kvdram3[dramoffset_kvs + i].range(383, 352); 
		mykeyvalue36.key = kvdram3[dramoffset_kvs + i].range(415, 384); 
		mykeyvalue36.value = kvdram3[dramoffset_kvs + i].range(447, 416); 
		mykeyvalue37.key = kvdram3[dramoffset_kvs + i].range(479, 448); 
		mykeyvalue37.value = kvdram3[dramoffset_kvs + i].range(511, 480); 
		#else 
		mykeyvalue30.key = kvdram3[dramoffset_kvs + i].data[0].key; 
		mykeyvalue30.value = kvdram3[dramoffset_kvs + i].data[0].value; 
		mykeyvalue31.key = kvdram3[dramoffset_kvs + i].data[1].key; 
		mykeyvalue31.value = kvdram3[dramoffset_kvs + i].data[1].value; 
		mykeyvalue32.key = kvdram3[dramoffset_kvs + i].data[2].key; 
		mykeyvalue32.value = kvdram3[dramoffset_kvs + i].data[2].value; 
		mykeyvalue33.key = kvdram3[dramoffset_kvs + i].data[3].key; 
		mykeyvalue33.value = kvdram3[dramoffset_kvs + i].data[3].value; 
		mykeyvalue34.key = kvdram3[dramoffset_kvs + i].data[4].key; 
		mykeyvalue34.value = kvdram3[dramoffset_kvs + i].data[4].value; 
		mykeyvalue35.key = kvdram3[dramoffset_kvs + i].data[5].key; 
		mykeyvalue35.value = kvdram3[dramoffset_kvs + i].data[5].value; 
		mykeyvalue36.key = kvdram3[dramoffset_kvs + i].data[6].key; 
		mykeyvalue36.value = kvdram3[dramoffset_kvs + i].data[6].value; 
		mykeyvalue37.key = kvdram3[dramoffset_kvs + i].data[7].key; 
		mykeyvalue37.value = kvdram3[dramoffset_kvs + i].data[7].value; 
		#endif
		keyvalue0_vault4.key = mergefunc(keyvalue0_vault3.key, mykeyvalue30.key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue0_vault4.value = mergefunc(keyvalue0_vault3.value, mykeyvalue30.value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue1_vault4.key = mergefunc(keyvalue1_vault3.key, mykeyvalue31.key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue1_vault4.value = mergefunc(keyvalue1_vault3.value, mykeyvalue31.value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue2_vault4.key = mergefunc(keyvalue2_vault3.key, mykeyvalue32.key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue2_vault4.value = mergefunc(keyvalue2_vault3.value, mykeyvalue32.value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue3_vault4.key = mergefunc(keyvalue3_vault3.key, mykeyvalue33.key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue3_vault4.value = mergefunc(keyvalue3_vault3.value, mykeyvalue33.value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue4_vault4.key = mergefunc(keyvalue4_vault3.key, mykeyvalue34.key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue4_vault4.value = mergefunc(keyvalue4_vault3.value, mykeyvalue34.value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue5_vault4.key = mergefunc(keyvalue5_vault3.key, mykeyvalue35.key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue5_vault4.value = mergefunc(keyvalue5_vault3.value, mykeyvalue35.value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue6_vault4.key = mergefunc(keyvalue6_vault3.key, mykeyvalue36.key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue6_vault4.value = mergefunc(keyvalue6_vault3.value, mykeyvalue36.value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue7_vault4.key = mergefunc(keyvalue7_vault3.key, mykeyvalue37.key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue7_vault4.value = mergefunc(keyvalue7_vault3.value, mykeyvalue37.value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		

		res[0][i] = SYNC_GETKV2(keyvalue0_vault4);
		res[1][i] = SYNC_GETKV2(keyvalue1_vault4);
		res[2][i] = SYNC_GETKV2(keyvalue2_vault4);
		res[3][i] = SYNC_GETKV2(keyvalue3_vault4);
		res[4][i] = SYNC_GETKV2(keyvalue4_vault4);
		res[5][i] = SYNC_GETKV2(keyvalue5_vault4);
		res[6][i] = SYNC_GETKV2(keyvalue6_vault4);
		res[7][i] = SYNC_GETKV2(keyvalue7_vault4);
	
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
		if(SYNC_GETKV2(res[0][i]).key < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(0*REDUCESZ + 2*i)<<endl; } 
		if(SYNC_GETKV2(res[0][i]).value < SYNC_GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(0*REDUCESZ + 2*i + 1)<<endl; } 
		if(SYNC_GETKV2(res[1][i]).key < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(1*REDUCESZ + 2*i)<<endl; } 
		if(SYNC_GETKV2(res[1][i]).value < SYNC_GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(1*REDUCESZ + 2*i + 1)<<endl; } 
		if(SYNC_GETKV2(res[2][i]).key < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(2*REDUCESZ + 2*i)<<endl; } 
		if(SYNC_GETKV2(res[2][i]).value < SYNC_GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(2*REDUCESZ + 2*i + 1)<<endl; } 
		if(SYNC_GETKV2(res[3][i]).key < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(3*REDUCESZ + 2*i)<<endl; } 
		if(SYNC_GETKV2(res[3][i]).value < SYNC_GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(3*REDUCESZ + 2*i + 1)<<endl; } 
		if(SYNC_GETKV2(res[4][i]).key < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(4*REDUCESZ + 2*i)<<endl; } 
		if(SYNC_GETKV2(res[4][i]).value < SYNC_GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(4*REDUCESZ + 2*i + 1)<<endl; } 
		if(SYNC_GETKV2(res[5][i]).key < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(5*REDUCESZ + 2*i)<<endl; } 
		if(SYNC_GETKV2(res[5][i]).value < SYNC_GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(5*REDUCESZ + 2*i + 1)<<endl; } 
		if(SYNC_GETKV2(res[6][i]).key < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(6*REDUCESZ + 2*i)<<endl; } 
		if(SYNC_GETKV2(res[6][i]).value < SYNC_GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(6*REDUCESZ + 2*i + 1)<<endl; } 
		if(SYNC_GETKV2(res[7][i]).key < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(7*REDUCESZ + 2*i)<<endl; } 
		if(SYNC_GETKV2(res[7][i]).value < SYNC_GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(7*REDUCESZ + 2*i + 1)<<endl; } 
		#endif
	}
	return;
}

#ifdef KOKOKOKOOOOOOO
uint32_type
	#ifdef SW 
	actssync::
	#endif
synchronizeandapply(bool_type enable1, bool_type enable2, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t res[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t refbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unsigned int colindex, batch_type voffset_kvs, globalparams_t globalparams){					
	uint32_type cummvmask_sp = 0;
	
	#ifndef SW_IMPL
	if(enable1 == OFF || enable2 == OFF){ return 0; }
	#ifdef _DEBUGMODE_KERNELPRINTS // _DEBUGMODE_KERNELPRINTS_TRACE //
	cout<<"synchronizeandapply: synchronize and apply function called."<<endl;
	#endif 
	analysis_type analysis_loopcount = REDUCESZ / 2;
	
	buffer_type reducebuffersz = globalparams.SIZE_REDUCE / 2;
	
	keyvalue_t keyvalue0_vault0;
	keyvalue_t keyvalue1_vault0;
	keyvalue_t keyvalue2_vault0;
	keyvalue_t keyvalue3_vault0;
	keyvalue_t keyvalue4_vault0;
	keyvalue_t keyvalue5_vault0;
	keyvalue_t keyvalue6_vault0;
	keyvalue_t keyvalue7_vault0;
	
	keyvalue_t keyvalue0_vault1;
	keyvalue_t keyvalue1_vault1;
	keyvalue_t keyvalue2_vault1;
	keyvalue_t keyvalue3_vault1;
	keyvalue_t keyvalue4_vault1;
	keyvalue_t keyvalue5_vault1;
	keyvalue_t keyvalue6_vault1;
	keyvalue_t keyvalue7_vault1;
	
	keyvalue_t keyvalue0_vault2;
	keyvalue_t keyvalue1_vault2;
	keyvalue_t keyvalue2_vault2;
	keyvalue_t keyvalue3_vault2;
	keyvalue_t keyvalue4_vault2;
	keyvalue_t keyvalue5_vault2;
	keyvalue_t keyvalue6_vault2;
	keyvalue_t keyvalue7_vault2;
	
	keyvalue_t keyvalue0_vault3;
	keyvalue_t keyvalue1_vault3;
	keyvalue_t keyvalue2_vault3;
	keyvalue_t keyvalue3_vault3;
	keyvalue_t keyvalue4_vault3;
	keyvalue_t keyvalue5_vault3;
	keyvalue_t keyvalue6_vault3;
	keyvalue_t keyvalue7_vault3;
	
	keyvalue_t keyvalue0_vault4;
	keyvalue_t keyvalue1_vault4;
	keyvalue_t keyvalue2_vault4;
	keyvalue_t keyvalue3_vault4;
	keyvalue_t keyvalue4_vault4;
	keyvalue_t keyvalue5_vault4;
	keyvalue_t keyvalue6_vault4;
	keyvalue_t keyvalue7_vault4;
	
	keyvalue_t keyvalue0_vault5;
	keyvalue_t keyvalue1_vault5;
	keyvalue_t keyvalue2_vault5;
	keyvalue_t keyvalue3_vault5;
	keyvalue_t keyvalue4_vault5;
	keyvalue_t keyvalue5_vault5;
	keyvalue_t keyvalue6_vault5;
	keyvalue_t keyvalue7_vault5;
	
	keyvalue_t keyvalue0_vault6;
	keyvalue_t keyvalue1_vault6;
	keyvalue_t keyvalue2_vault6;
	keyvalue_t keyvalue3_vault6;
	keyvalue_t keyvalue4_vault6;
	keyvalue_t keyvalue5_vault6;
	keyvalue_t keyvalue6_vault6;
	keyvalue_t keyvalue7_vault6;
	
	keyvalue_t keyvalue0_vault7;
	keyvalue_t keyvalue1_vault7;
	keyvalue_t keyvalue2_vault7;
	keyvalue_t keyvalue3_vault7;
	keyvalue_t keyvalue4_vault7;
	keyvalue_t keyvalue5_vault7;
	keyvalue_t keyvalue6_vault7;
	keyvalue_t keyvalue7_vault7;
	
	keyvalue_t keyvalue0_vault8;
	keyvalue_t keyvalue1_vault8;
	keyvalue_t keyvalue2_vault8;
	keyvalue_t keyvalue3_vault8;
	keyvalue_t keyvalue4_vault8;
	keyvalue_t keyvalue5_vault8;
	keyvalue_t keyvalue6_vault8;
	keyvalue_t keyvalue7_vault8;
	
	
	unsigned int vid[NUM_PARTITIONS][2];
	#pragma HLS ARRAY_PARTITION variable=vid complete
	
	keyvalue_t res0;
	keyvalue_t res1;
	keyvalue_t res2;
	keyvalue_t res3;
	keyvalue_t res4;
	keyvalue_t res5;
	keyvalue_t res6;
	keyvalue_t res7;
	
	for (buffer_type i=0; i<reducebuffersz; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE II=1
		// synchronize
		keyvalue0_vault1 = SYNC_GETKV2(buffer0[0][i]); 
		keyvalue1_vault1 = SYNC_GETKV2(buffer0[1][i]); 
		keyvalue2_vault1 = SYNC_GETKV2(buffer0[2][i]); 
		keyvalue3_vault1 = SYNC_GETKV2(buffer0[3][i]); 
		keyvalue4_vault1 = SYNC_GETKV2(buffer0[4][i]); 
		keyvalue5_vault1 = SYNC_GETKV2(buffer0[5][i]); 
		keyvalue6_vault1 = SYNC_GETKV2(buffer0[6][i]); 
		keyvalue7_vault1 = SYNC_GETKV2(buffer0[7][i]); 
			
		keyvalue0_vault2.key = mergefunc(keyvalue0_vault1.key, SYNC_GETKV2(buffer1[0][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue0_vault2.value = mergefunc(keyvalue0_vault1.value, SYNC_GETKV2(buffer1[0][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue1_vault2.key = mergefunc(keyvalue1_vault1.key, SYNC_GETKV2(buffer1[1][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue1_vault2.value = mergefunc(keyvalue1_vault1.value, SYNC_GETKV2(buffer1[1][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue2_vault2.key = mergefunc(keyvalue2_vault1.key, SYNC_GETKV2(buffer1[2][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue2_vault2.value = mergefunc(keyvalue2_vault1.value, SYNC_GETKV2(buffer1[2][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue3_vault2.key = mergefunc(keyvalue3_vault1.key, SYNC_GETKV2(buffer1[3][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue3_vault2.value = mergefunc(keyvalue3_vault1.value, SYNC_GETKV2(buffer1[3][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue4_vault2.key = mergefunc(keyvalue4_vault1.key, SYNC_GETKV2(buffer1[4][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue4_vault2.value = mergefunc(keyvalue4_vault1.value, SYNC_GETKV2(buffer1[4][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue5_vault2.key = mergefunc(keyvalue5_vault1.key, SYNC_GETKV2(buffer1[5][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue5_vault2.value = mergefunc(keyvalue5_vault1.value, SYNC_GETKV2(buffer1[5][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue6_vault2.key = mergefunc(keyvalue6_vault1.key, SYNC_GETKV2(buffer1[6][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue6_vault2.value = mergefunc(keyvalue6_vault1.value, SYNC_GETKV2(buffer1[6][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue7_vault2.key = mergefunc(keyvalue7_vault1.key, SYNC_GETKV2(buffer1[7][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue7_vault2.value = mergefunc(keyvalue7_vault1.value, SYNC_GETKV2(buffer1[7][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue0_vault3.key = mergefunc(keyvalue0_vault2.key, SYNC_GETKV2(buffer2[0][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue0_vault3.value = mergefunc(keyvalue0_vault2.value, SYNC_GETKV2(buffer2[0][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue1_vault3.key = mergefunc(keyvalue1_vault2.key, SYNC_GETKV2(buffer2[1][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue1_vault3.value = mergefunc(keyvalue1_vault2.value, SYNC_GETKV2(buffer2[1][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue2_vault3.key = mergefunc(keyvalue2_vault2.key, SYNC_GETKV2(buffer2[2][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue2_vault3.value = mergefunc(keyvalue2_vault2.value, SYNC_GETKV2(buffer2[2][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue3_vault3.key = mergefunc(keyvalue3_vault2.key, SYNC_GETKV2(buffer2[3][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue3_vault3.value = mergefunc(keyvalue3_vault2.value, SYNC_GETKV2(buffer2[3][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue4_vault3.key = mergefunc(keyvalue4_vault2.key, SYNC_GETKV2(buffer2[4][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue4_vault3.value = mergefunc(keyvalue4_vault2.value, SYNC_GETKV2(buffer2[4][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue5_vault3.key = mergefunc(keyvalue5_vault2.key, SYNC_GETKV2(buffer2[5][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue5_vault3.value = mergefunc(keyvalue5_vault2.value, SYNC_GETKV2(buffer2[5][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue6_vault3.key = mergefunc(keyvalue6_vault2.key, SYNC_GETKV2(buffer2[6][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue6_vault3.value = mergefunc(keyvalue6_vault2.value, SYNC_GETKV2(buffer2[6][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue7_vault3.key = mergefunc(keyvalue7_vault2.key, SYNC_GETKV2(buffer2[7][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue7_vault3.value = mergefunc(keyvalue7_vault2.value, SYNC_GETKV2(buffer2[7][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue0_vault4.key = mergefunc(keyvalue0_vault3.key, SYNC_GETKV2(buffer3[0][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue0_vault4.value = mergefunc(keyvalue0_vault3.value, SYNC_GETKV2(buffer3[0][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue1_vault4.key = mergefunc(keyvalue1_vault3.key, SYNC_GETKV2(buffer3[1][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue1_vault4.value = mergefunc(keyvalue1_vault3.value, SYNC_GETKV2(buffer3[1][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue2_vault4.key = mergefunc(keyvalue2_vault3.key, SYNC_GETKV2(buffer3[2][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue2_vault4.value = mergefunc(keyvalue2_vault3.value, SYNC_GETKV2(buffer3[2][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue3_vault4.key = mergefunc(keyvalue3_vault3.key, SYNC_GETKV2(buffer3[3][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue3_vault4.value = mergefunc(keyvalue3_vault3.value, SYNC_GETKV2(buffer3[3][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue4_vault4.key = mergefunc(keyvalue4_vault3.key, SYNC_GETKV2(buffer3[4][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue4_vault4.value = mergefunc(keyvalue4_vault3.value, SYNC_GETKV2(buffer3[4][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue5_vault4.key = mergefunc(keyvalue5_vault3.key, SYNC_GETKV2(buffer3[5][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue5_vault4.value = mergefunc(keyvalue5_vault3.value, SYNC_GETKV2(buffer3[5][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue6_vault4.key = mergefunc(keyvalue6_vault3.key, SYNC_GETKV2(buffer3[6][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue6_vault4.value = mergefunc(keyvalue6_vault3.value, SYNC_GETKV2(buffer3[6][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue7_vault4.key = mergefunc(keyvalue7_vault3.key, SYNC_GETKV2(buffer3[7][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue7_vault4.value = mergefunc(keyvalue7_vault3.value, SYNC_GETKV2(buffer3[7][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue0_vault5.key = mergefunc(keyvalue0_vault4.key, SYNC_GETKV2(buffer4[0][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue0_vault5.value = mergefunc(keyvalue0_vault4.value, SYNC_GETKV2(buffer4[0][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue1_vault5.key = mergefunc(keyvalue1_vault4.key, SYNC_GETKV2(buffer4[1][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue1_vault5.value = mergefunc(keyvalue1_vault4.value, SYNC_GETKV2(buffer4[1][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue2_vault5.key = mergefunc(keyvalue2_vault4.key, SYNC_GETKV2(buffer4[2][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue2_vault5.value = mergefunc(keyvalue2_vault4.value, SYNC_GETKV2(buffer4[2][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue3_vault5.key = mergefunc(keyvalue3_vault4.key, SYNC_GETKV2(buffer4[3][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue3_vault5.value = mergefunc(keyvalue3_vault4.value, SYNC_GETKV2(buffer4[3][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue4_vault5.key = mergefunc(keyvalue4_vault4.key, SYNC_GETKV2(buffer4[4][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue4_vault5.value = mergefunc(keyvalue4_vault4.value, SYNC_GETKV2(buffer4[4][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue5_vault5.key = mergefunc(keyvalue5_vault4.key, SYNC_GETKV2(buffer4[5][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue5_vault5.value = mergefunc(keyvalue5_vault4.value, SYNC_GETKV2(buffer4[5][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue6_vault5.key = mergefunc(keyvalue6_vault4.key, SYNC_GETKV2(buffer4[6][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue6_vault5.value = mergefunc(keyvalue6_vault4.value, SYNC_GETKV2(buffer4[6][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue7_vault5.key = mergefunc(keyvalue7_vault4.key, SYNC_GETKV2(buffer4[7][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue7_vault5.value = mergefunc(keyvalue7_vault4.value, SYNC_GETKV2(buffer4[7][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue0_vault6.key = mergefunc(keyvalue0_vault5.key, SYNC_GETKV2(buffer5[0][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue0_vault6.value = mergefunc(keyvalue0_vault5.value, SYNC_GETKV2(buffer5[0][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue1_vault6.key = mergefunc(keyvalue1_vault5.key, SYNC_GETKV2(buffer5[1][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue1_vault6.value = mergefunc(keyvalue1_vault5.value, SYNC_GETKV2(buffer5[1][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue2_vault6.key = mergefunc(keyvalue2_vault5.key, SYNC_GETKV2(buffer5[2][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue2_vault6.value = mergefunc(keyvalue2_vault5.value, SYNC_GETKV2(buffer5[2][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue3_vault6.key = mergefunc(keyvalue3_vault5.key, SYNC_GETKV2(buffer5[3][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue3_vault6.value = mergefunc(keyvalue3_vault5.value, SYNC_GETKV2(buffer5[3][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue4_vault6.key = mergefunc(keyvalue4_vault5.key, SYNC_GETKV2(buffer5[4][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue4_vault6.value = mergefunc(keyvalue4_vault5.value, SYNC_GETKV2(buffer5[4][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue5_vault6.key = mergefunc(keyvalue5_vault5.key, SYNC_GETKV2(buffer5[5][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue5_vault6.value = mergefunc(keyvalue5_vault5.value, SYNC_GETKV2(buffer5[5][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue6_vault6.key = mergefunc(keyvalue6_vault5.key, SYNC_GETKV2(buffer5[6][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue6_vault6.value = mergefunc(keyvalue6_vault5.value, SYNC_GETKV2(buffer5[6][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue7_vault6.key = mergefunc(keyvalue7_vault5.key, SYNC_GETKV2(buffer5[7][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue7_vault6.value = mergefunc(keyvalue7_vault5.value, SYNC_GETKV2(buffer5[7][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue0_vault7.key = mergefunc(keyvalue0_vault6.key, SYNC_GETKV2(buffer6[0][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue0_vault7.value = mergefunc(keyvalue0_vault6.value, SYNC_GETKV2(buffer6[0][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue1_vault7.key = mergefunc(keyvalue1_vault6.key, SYNC_GETKV2(buffer6[1][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue1_vault7.value = mergefunc(keyvalue1_vault6.value, SYNC_GETKV2(buffer6[1][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue2_vault7.key = mergefunc(keyvalue2_vault6.key, SYNC_GETKV2(buffer6[2][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue2_vault7.value = mergefunc(keyvalue2_vault6.value, SYNC_GETKV2(buffer6[2][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue3_vault7.key = mergefunc(keyvalue3_vault6.key, SYNC_GETKV2(buffer6[3][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue3_vault7.value = mergefunc(keyvalue3_vault6.value, SYNC_GETKV2(buffer6[3][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue4_vault7.key = mergefunc(keyvalue4_vault6.key, SYNC_GETKV2(buffer6[4][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue4_vault7.value = mergefunc(keyvalue4_vault6.value, SYNC_GETKV2(buffer6[4][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue5_vault7.key = mergefunc(keyvalue5_vault6.key, SYNC_GETKV2(buffer6[5][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue5_vault7.value = mergefunc(keyvalue5_vault6.value, SYNC_GETKV2(buffer6[5][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue6_vault7.key = mergefunc(keyvalue6_vault6.key, SYNC_GETKV2(buffer6[6][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue6_vault7.value = mergefunc(keyvalue6_vault6.value, SYNC_GETKV2(buffer6[6][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue7_vault7.key = mergefunc(keyvalue7_vault6.key, SYNC_GETKV2(buffer6[7][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue7_vault7.value = mergefunc(keyvalue7_vault6.value, SYNC_GETKV2(buffer6[7][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue0_vault8.key = mergefunc(keyvalue0_vault7.key, SYNC_GETKV2(buffer7[0][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue0_vault8.value = mergefunc(keyvalue0_vault7.value, SYNC_GETKV2(buffer7[0][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue1_vault8.key = mergefunc(keyvalue1_vault7.key, SYNC_GETKV2(buffer7[1][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue1_vault8.value = mergefunc(keyvalue1_vault7.value, SYNC_GETKV2(buffer7[1][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue2_vault8.key = mergefunc(keyvalue2_vault7.key, SYNC_GETKV2(buffer7[2][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue2_vault8.value = mergefunc(keyvalue2_vault7.value, SYNC_GETKV2(buffer7[2][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue3_vault8.key = mergefunc(keyvalue3_vault7.key, SYNC_GETKV2(buffer7[3][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue3_vault8.value = mergefunc(keyvalue3_vault7.value, SYNC_GETKV2(buffer7[3][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue4_vault8.key = mergefunc(keyvalue4_vault7.key, SYNC_GETKV2(buffer7[4][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue4_vault8.value = mergefunc(keyvalue4_vault7.value, SYNC_GETKV2(buffer7[4][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue5_vault8.key = mergefunc(keyvalue5_vault7.key, SYNC_GETKV2(buffer7[5][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue5_vault8.value = mergefunc(keyvalue5_vault7.value, SYNC_GETKV2(buffer7[5][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue6_vault8.key = mergefunc(keyvalue6_vault7.key, SYNC_GETKV2(buffer7[6][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue6_vault8.value = mergefunc(keyvalue6_vault7.value, SYNC_GETKV2(buffer7[6][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue7_vault8.key = mergefunc(keyvalue7_vault7.key, SYNC_GETKV2(buffer7[7][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue7_vault8.value = mergefunc(keyvalue7_vault7.value, SYNC_GETKV2(buffer7[7][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	

		res[0][i] = SYNC_GETKV2(keyvalue0_vault8);
		res[1][i] = SYNC_GETKV2(keyvalue1_vault8);
		res[2][i] = SYNC_GETKV2(keyvalue2_vault8);
		res[3][i] = SYNC_GETKV2(keyvalue3_vault8);
		res[4][i] = SYNC_GETKV2(keyvalue4_vault8);
		res[5][i] = SYNC_GETKV2(keyvalue5_vault8);
		res[6][i] = SYNC_GETKV2(keyvalue6_vault8);
		res[7][i] = SYNC_GETKV2(keyvalue7_vault8);
		
		#ifdef _DEBUGMODE_KERNELPRINTS //
		if(SYNC_GETKV2(res[0][i]).key < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(0*REDUCESZ + 2*i)<<endl; } 
		if(SYNC_GETKV2(res[0][i]).value < SYNC_GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(0*REDUCESZ + 2*i + 1)<<endl; } 
		if(SYNC_GETKV2(res[1][i]).key < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(1*REDUCESZ + 2*i)<<endl; } 
		if(SYNC_GETKV2(res[1][i]).value < SYNC_GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(1*REDUCESZ + 2*i + 1)<<endl; } 
		if(SYNC_GETKV2(res[2][i]).key < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(2*REDUCESZ + 2*i)<<endl; } 
		if(SYNC_GETKV2(res[2][i]).value < SYNC_GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(2*REDUCESZ + 2*i + 1)<<endl; } 
		if(SYNC_GETKV2(res[3][i]).key < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(3*REDUCESZ + 2*i)<<endl; } 
		if(SYNC_GETKV2(res[3][i]).value < SYNC_GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(3*REDUCESZ + 2*i + 1)<<endl; } 
		if(SYNC_GETKV2(res[4][i]).key < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(4*REDUCESZ + 2*i)<<endl; } 
		if(SYNC_GETKV2(res[4][i]).value < SYNC_GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(4*REDUCESZ + 2*i + 1)<<endl; } 
		if(SYNC_GETKV2(res[5][i]).key < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(5*REDUCESZ + 2*i)<<endl; } 
		if(SYNC_GETKV2(res[5][i]).value < SYNC_GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(5*REDUCESZ + 2*i + 1)<<endl; } 
		if(SYNC_GETKV2(res[6][i]).key < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(6*REDUCESZ + 2*i)<<endl; } 
		if(SYNC_GETKV2(res[6][i]).value < SYNC_GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(6*REDUCESZ + 2*i + 1)<<endl; } 
		if(SYNC_GETKV2(res[7][i]).key < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(7*REDUCESZ + 2*i)<<endl; } 
		if(SYNC_GETKV2(res[7][i]).value < SYNC_GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(7*REDUCESZ + 2*i + 1)<<endl; } 
		#endif
		
		// apply
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3 // _DEBUGMODE_CHECKS2
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
	
		#endif 
		
		keyvalue_t data0 = keyvalue0_vault8;
		keyvalue_t data1 = keyvalue1_vault8;
		keyvalue_t data2 = keyvalue2_vault8;
		keyvalue_t data3 = keyvalue3_vault8;
		keyvalue_t data4 = keyvalue4_vault8;
		keyvalue_t data5 = keyvalue5_vault8;
		keyvalue_t data6 = keyvalue6_vault8;
		keyvalue_t data7 = keyvalue7_vault8;
		keyvalue_t udata0 = SYNC_GETKV2(refbuffer[0][i]);
		keyvalue_t udata1 = SYNC_GETKV2(refbuffer[1][i]);
		keyvalue_t udata2 = SYNC_GETKV2(refbuffer[2][i]);
		keyvalue_t udata3 = SYNC_GETKV2(refbuffer[3][i]);
		keyvalue_t udata4 = SYNC_GETKV2(refbuffer[4][i]);
		keyvalue_t udata5 = SYNC_GETKV2(refbuffer[5][i]);
		keyvalue_t udata6 = SYNC_GETKV2(refbuffer[6][i]);
		keyvalue_t udata7 = SYNC_GETKV2(refbuffer[7][i]);
		
		res0.key = applyfunc(udata0.key, data0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res1.key = applyfunc(udata1.key, data1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res2.key = applyfunc(udata2.key, data2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res3.key = applyfunc(udata3.key, data3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res4.key = applyfunc(udata4.key, data4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res5.key = applyfunc(udata5.key, data5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res6.key = applyfunc(udata6.key, data6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res7.key = applyfunc(udata7.key, data7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res0.value = applyfunc(udata0.value, data0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res1.value = applyfunc(udata1.value, data1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res2.value = applyfunc(udata2.value, data2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res3.value = applyfunc(udata3.value, data3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res4.value = applyfunc(udata4.value, data4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res5.value = applyfunc(udata5.value, data5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res6.value = applyfunc(udata6.value, data6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res7.value = applyfunc(udata7.value, data7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		
		if(res0.key != udata0.key){ vmask[i].data[colindex + 0].key = 1; } else { vmask[i].data[colindex + 0].key = 0; }
		if(res0.value != udata0.value){ vmask[i].data[colindex + 0].value = 1; } else { vmask[i].data[colindex + 0].value = 0; }
		if(res1.key != udata1.key){ vmask[i].data[colindex + 1].key = 1; } else { vmask[i].data[colindex + 1].key = 0; }
		if(res1.value != udata1.value){ vmask[i].data[colindex + 1].value = 1; } else { vmask[i].data[colindex + 1].value = 0; }
		if(res2.key != udata2.key){ vmask[i].data[colindex + 2].key = 1; } else { vmask[i].data[colindex + 2].key = 0; }
		if(res2.value != udata2.value){ vmask[i].data[colindex + 2].value = 1; } else { vmask[i].data[colindex + 2].value = 0; }
		if(res3.key != udata3.key){ vmask[i].data[colindex + 3].key = 1; } else { vmask[i].data[colindex + 3].key = 0; }
		if(res3.value != udata3.value){ vmask[i].data[colindex + 3].value = 1; } else { vmask[i].data[colindex + 3].value = 0; }
		if(res4.key != udata4.key){ vmask[i].data[colindex + 4].key = 1; } else { vmask[i].data[colindex + 4].key = 0; }
		if(res4.value != udata4.value){ vmask[i].data[colindex + 4].value = 1; } else { vmask[i].data[colindex + 4].value = 0; }
		if(res5.key != udata5.key){ vmask[i].data[colindex + 5].key = 1; } else { vmask[i].data[colindex + 5].key = 0; }
		if(res5.value != udata5.value){ vmask[i].data[colindex + 5].value = 1; } else { vmask[i].data[colindex + 5].value = 0; }
		if(res6.key != udata6.key){ vmask[i].data[colindex + 6].key = 1; } else { vmask[i].data[colindex + 6].key = 0; }
		if(res6.value != udata6.value){ vmask[i].data[colindex + 6].value = 1; } else { vmask[i].data[colindex + 6].value = 0; }
		if(res7.key != udata7.key){ vmask[i].data[colindex + 7].key = 1; } else { vmask[i].data[colindex + 7].key = 0; }
		if(res7.value != udata7.value){ vmask[i].data[colindex + 7].value = 1; } else { vmask[i].data[colindex + 7].value = 0; }
		
		uint32_type mask = convertvmasktouint32(vmask, i);
		cummvmask_sp = cummvmask_sp | mask;
		
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3	
		if(res0.key != udata0.key){ cout<<"SYNC AND APPLY FUNC SEEN @ (vid1: "<<vid[0][0]<<" & vid2: "<<vid[0][1]<<"): res0.key: "<<res0.key<<", res0.value: "<<res0.value<<": udata0.key: "<<udata0.key<<", udata0.value: "<<udata0.value<<", data0.key: "<<data0.key<<", data0.value: "<<data0.value<<endl; }
		if(res0.key != udata0.key){ cout<<"synchronizeandapply: vid[0][0]: "<<vid[0][0]<<", vmask["<<i<<"].data["<<colindex + 0<<"].key: "<<vmask[i].data[colindex + 0].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res0.value != udata0.value){ cout<<"synchronizeandapply: vid[0][1]: "<<vid[0][1]<<", vmask["<<i<<"].data["<<colindex + 0<<"].value: "<<vmask[i].data[colindex + 0].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res1.key != udata1.key){ cout<<"SYNC AND APPLY FUNC SEEN @ (vid1: "<<vid[1][0]<<" & vid2: "<<vid[1][1]<<"): res1.key: "<<res1.key<<", res1.value: "<<res1.value<<": udata1.key: "<<udata1.key<<", udata1.value: "<<udata1.value<<", data1.key: "<<data1.key<<", data1.value: "<<data1.value<<endl; }
		if(res1.key != udata1.key){ cout<<"synchronizeandapply: vid[1][0]: "<<vid[1][0]<<", vmask["<<i<<"].data["<<colindex + 1<<"].key: "<<vmask[i].data[colindex + 1].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res1.value != udata1.value){ cout<<"synchronizeandapply: vid[1][1]: "<<vid[1][1]<<", vmask["<<i<<"].data["<<colindex + 1<<"].value: "<<vmask[i].data[colindex + 1].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res2.key != udata2.key){ cout<<"SYNC AND APPLY FUNC SEEN @ (vid1: "<<vid[2][0]<<" & vid2: "<<vid[2][1]<<"): res2.key: "<<res2.key<<", res2.value: "<<res2.value<<": udata2.key: "<<udata2.key<<", udata2.value: "<<udata2.value<<", data2.key: "<<data2.key<<", data2.value: "<<data2.value<<endl; }
		if(res2.key != udata2.key){ cout<<"synchronizeandapply: vid[2][0]: "<<vid[2][0]<<", vmask["<<i<<"].data["<<colindex + 2<<"].key: "<<vmask[i].data[colindex + 2].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res2.value != udata2.value){ cout<<"synchronizeandapply: vid[2][1]: "<<vid[2][1]<<", vmask["<<i<<"].data["<<colindex + 2<<"].value: "<<vmask[i].data[colindex + 2].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res3.key != udata3.key){ cout<<"SYNC AND APPLY FUNC SEEN @ (vid1: "<<vid[3][0]<<" & vid2: "<<vid[3][1]<<"): res3.key: "<<res3.key<<", res3.value: "<<res3.value<<": udata3.key: "<<udata3.key<<", udata3.value: "<<udata3.value<<", data3.key: "<<data3.key<<", data3.value: "<<data3.value<<endl; }
		if(res3.key != udata3.key){ cout<<"synchronizeandapply: vid[3][0]: "<<vid[3][0]<<", vmask["<<i<<"].data["<<colindex + 3<<"].key: "<<vmask[i].data[colindex + 3].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res3.value != udata3.value){ cout<<"synchronizeandapply: vid[3][1]: "<<vid[3][1]<<", vmask["<<i<<"].data["<<colindex + 3<<"].value: "<<vmask[i].data[colindex + 3].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res4.key != udata4.key){ cout<<"SYNC AND APPLY FUNC SEEN @ (vid1: "<<vid[4][0]<<" & vid2: "<<vid[4][1]<<"): res4.key: "<<res4.key<<", res4.value: "<<res4.value<<": udata4.key: "<<udata4.key<<", udata4.value: "<<udata4.value<<", data4.key: "<<data4.key<<", data4.value: "<<data4.value<<endl; }
		if(res4.key != udata4.key){ cout<<"synchronizeandapply: vid[4][0]: "<<vid[4][0]<<", vmask["<<i<<"].data["<<colindex + 4<<"].key: "<<vmask[i].data[colindex + 4].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res4.value != udata4.value){ cout<<"synchronizeandapply: vid[4][1]: "<<vid[4][1]<<", vmask["<<i<<"].data["<<colindex + 4<<"].value: "<<vmask[i].data[colindex + 4].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res5.key != udata5.key){ cout<<"SYNC AND APPLY FUNC SEEN @ (vid1: "<<vid[5][0]<<" & vid2: "<<vid[5][1]<<"): res5.key: "<<res5.key<<", res5.value: "<<res5.value<<": udata5.key: "<<udata5.key<<", udata5.value: "<<udata5.value<<", data5.key: "<<data5.key<<", data5.value: "<<data5.value<<endl; }
		if(res5.key != udata5.key){ cout<<"synchronizeandapply: vid[5][0]: "<<vid[5][0]<<", vmask["<<i<<"].data["<<colindex + 5<<"].key: "<<vmask[i].data[colindex + 5].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res5.value != udata5.value){ cout<<"synchronizeandapply: vid[5][1]: "<<vid[5][1]<<", vmask["<<i<<"].data["<<colindex + 5<<"].value: "<<vmask[i].data[colindex + 5].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res6.key != udata6.key){ cout<<"SYNC AND APPLY FUNC SEEN @ (vid1: "<<vid[6][0]<<" & vid2: "<<vid[6][1]<<"): res6.key: "<<res6.key<<", res6.value: "<<res6.value<<": udata6.key: "<<udata6.key<<", udata6.value: "<<udata6.value<<", data6.key: "<<data6.key<<", data6.value: "<<data6.value<<endl; }
		if(res6.key != udata6.key){ cout<<"synchronizeandapply: vid[6][0]: "<<vid[6][0]<<", vmask["<<i<<"].data["<<colindex + 6<<"].key: "<<vmask[i].data[colindex + 6].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res6.value != udata6.value){ cout<<"synchronizeandapply: vid[6][1]: "<<vid[6][1]<<", vmask["<<i<<"].data["<<colindex + 6<<"].value: "<<vmask[i].data[colindex + 6].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res7.key != udata7.key){ cout<<"SYNC AND APPLY FUNC SEEN @ (vid1: "<<vid[7][0]<<" & vid2: "<<vid[7][1]<<"): res7.key: "<<res7.key<<", res7.value: "<<res7.value<<": udata7.key: "<<udata7.key<<", udata7.value: "<<udata7.value<<", data7.key: "<<data7.key<<", data7.value: "<<data7.value<<endl; }
		if(res7.key != udata7.key){ cout<<"synchronizeandapply: vid[7][0]: "<<vid[7][0]<<", vmask["<<i<<"].data["<<colindex + 7<<"].key: "<<vmask[i].data[colindex + 7].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res7.value != udata7.value){ cout<<"synchronizeandapply: vid[7][1]: "<<vid[7][1]<<", vmask["<<i<<"].data["<<colindex + 7<<"].value: "<<vmask[i].data[colindex + 7].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
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
		#endif
	}
	#endif
	return cummvmask_sp;
}
#endif 
uint32_type
	#ifdef SW 
	actssync::
	#endif
synchronizeandapply(bool_type enable1, bool_type enable2, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t res[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t refbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unsigned int colindex, batch_type voffset_kvs, globalparams_t globalparams){					
	uint32_type cummvmask_sp = 0;
	
	#ifndef SW_IMPL
	if(enable1 == OFF || enable2 == OFF){ return 0; }
	#ifdef _DEBUGMODE_KERNELPRINTS // _DEBUGMODE_KERNELPRINTS_TRACE //
	cout<<"synchronizeandapply: synchronize and apply function called."<<endl;
	#endif 
	analysis_type analysis_loopcount = REDUCESZ / 2;
	
	buffer_type reducebuffersz = globalparams.SIZE_REDUCE / 2;
	
	keyvalue_t keyvalue0_vault0;
	keyvalue_t keyvalue1_vault0;
	keyvalue_t keyvalue2_vault0;
	keyvalue_t keyvalue3_vault0;
	keyvalue_t keyvalue4_vault0;
	keyvalue_t keyvalue5_vault0;
	keyvalue_t keyvalue6_vault0;
	keyvalue_t keyvalue7_vault0;
	
	keyvalue_t keyvalue0_vault1;
	keyvalue_t keyvalue1_vault1;
	keyvalue_t keyvalue2_vault1;
	keyvalue_t keyvalue3_vault1;
	keyvalue_t keyvalue4_vault1;
	keyvalue_t keyvalue5_vault1;
	keyvalue_t keyvalue6_vault1;
	keyvalue_t keyvalue7_vault1;
	
	keyvalue_t keyvalue0_vault2;
	keyvalue_t keyvalue1_vault2;
	keyvalue_t keyvalue2_vault2;
	keyvalue_t keyvalue3_vault2;
	keyvalue_t keyvalue4_vault2;
	keyvalue_t keyvalue5_vault2;
	keyvalue_t keyvalue6_vault2;
	keyvalue_t keyvalue7_vault2;
	
	keyvalue_t keyvalue0_vault3;
	keyvalue_t keyvalue1_vault3;
	keyvalue_t keyvalue2_vault3;
	keyvalue_t keyvalue3_vault3;
	keyvalue_t keyvalue4_vault3;
	keyvalue_t keyvalue5_vault3;
	keyvalue_t keyvalue6_vault3;
	keyvalue_t keyvalue7_vault3;
	
	keyvalue_t keyvalue0_vault4;
	keyvalue_t keyvalue1_vault4;
	keyvalue_t keyvalue2_vault4;
	keyvalue_t keyvalue3_vault4;
	keyvalue_t keyvalue4_vault4;
	keyvalue_t keyvalue5_vault4;
	keyvalue_t keyvalue6_vault4;
	keyvalue_t keyvalue7_vault4;
	
	keyvalue_t keyvalue0_vault5;
	keyvalue_t keyvalue1_vault5;
	keyvalue_t keyvalue2_vault5;
	keyvalue_t keyvalue3_vault5;
	keyvalue_t keyvalue4_vault5;
	keyvalue_t keyvalue5_vault5;
	keyvalue_t keyvalue6_vault5;
	keyvalue_t keyvalue7_vault5;
	
	keyvalue_t keyvalue0_vault6;
	keyvalue_t keyvalue1_vault6;
	keyvalue_t keyvalue2_vault6;
	keyvalue_t keyvalue3_vault6;
	keyvalue_t keyvalue4_vault6;
	keyvalue_t keyvalue5_vault6;
	keyvalue_t keyvalue6_vault6;
	keyvalue_t keyvalue7_vault6;
	
	keyvalue_t keyvalue0_vault7;
	keyvalue_t keyvalue1_vault7;
	keyvalue_t keyvalue2_vault7;
	keyvalue_t keyvalue3_vault7;
	keyvalue_t keyvalue4_vault7;
	keyvalue_t keyvalue5_vault7;
	keyvalue_t keyvalue6_vault7;
	keyvalue_t keyvalue7_vault7;
	
	keyvalue_t keyvalue0_vault8;
	keyvalue_t keyvalue1_vault8;
	keyvalue_t keyvalue2_vault8;
	keyvalue_t keyvalue3_vault8;
	keyvalue_t keyvalue4_vault8;
	keyvalue_t keyvalue5_vault8;
	keyvalue_t keyvalue6_vault8;
	keyvalue_t keyvalue7_vault8;
	
	
	unsigned int vid[NUM_PARTITIONS][2];
	#pragma HLS ARRAY_PARTITION variable=vid complete
	
	keyvalue_t res0;
	keyvalue_t res1;
	keyvalue_t res2;
	keyvalue_t res3;
	keyvalue_t res4;
	keyvalue_t res5;
	keyvalue_t res6;
	keyvalue_t res7;
	
	for (buffer_type i=0; i<reducebuffersz; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE II=1
	
		keyvalue_t udata0 = SYNC_GETKV2(refbuffer[0][i]);
	
		keyvalue_t udata1 = SYNC_GETKV2(refbuffer[1][i]);
	
		keyvalue_t udata2 = SYNC_GETKV2(refbuffer[2][i]);
	
		keyvalue_t udata3 = SYNC_GETKV2(refbuffer[3][i]);
	
		keyvalue_t udata4 = SYNC_GETKV2(refbuffer[4][i]);
	
		keyvalue_t udata5 = SYNC_GETKV2(refbuffer[5][i]);
	
		keyvalue_t udata6 = SYNC_GETKV2(refbuffer[6][i]);
	
		keyvalue_t udata7 = SYNC_GETKV2(refbuffer[7][i]);
		
		// synchronize // NEWCHANGE.
	
		keyvalue0_vault1 = SYNC_GETKV2(buffer0[0][i]); 
	
		keyvalue1_vault1 = SYNC_GETKV2(buffer0[1][i]); 
	
		keyvalue2_vault1 = SYNC_GETKV2(buffer0[2][i]); 
	
		keyvalue3_vault1 = SYNC_GETKV2(buffer0[3][i]); 
	
		keyvalue4_vault1 = SYNC_GETKV2(buffer0[4][i]); 
	
		keyvalue5_vault1 = SYNC_GETKV2(buffer0[5][i]); 
	
		keyvalue6_vault1 = SYNC_GETKV2(buffer0[6][i]); 
	
		keyvalue7_vault1 = SYNC_GETKV2(buffer0[7][i]); 
			
	
		keyvalue0_vault2.key = mergefunc(keyvalue0_vault1.key, SYNC_GETKV2(buffer1[0][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue0_vault2.value = mergefunc(keyvalue0_vault1.value, SYNC_GETKV2(buffer1[0][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue1_vault2.key = mergefunc(keyvalue1_vault1.key, SYNC_GETKV2(buffer1[1][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue1_vault2.value = mergefunc(keyvalue1_vault1.value, SYNC_GETKV2(buffer1[1][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue2_vault2.key = mergefunc(keyvalue2_vault1.key, SYNC_GETKV2(buffer1[2][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue2_vault2.value = mergefunc(keyvalue2_vault1.value, SYNC_GETKV2(buffer1[2][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue3_vault2.key = mergefunc(keyvalue3_vault1.key, SYNC_GETKV2(buffer1[3][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue3_vault2.value = mergefunc(keyvalue3_vault1.value, SYNC_GETKV2(buffer1[3][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue4_vault2.key = mergefunc(keyvalue4_vault1.key, SYNC_GETKV2(buffer1[4][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue4_vault2.value = mergefunc(keyvalue4_vault1.value, SYNC_GETKV2(buffer1[4][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue5_vault2.key = mergefunc(keyvalue5_vault1.key, SYNC_GETKV2(buffer1[5][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue5_vault2.value = mergefunc(keyvalue5_vault1.value, SYNC_GETKV2(buffer1[5][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue6_vault2.key = mergefunc(keyvalue6_vault1.key, SYNC_GETKV2(buffer1[6][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue6_vault2.value = mergefunc(keyvalue6_vault1.value, SYNC_GETKV2(buffer1[6][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue7_vault2.key = mergefunc(keyvalue7_vault1.key, SYNC_GETKV2(buffer1[7][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue7_vault2.value = mergefunc(keyvalue7_vault1.value, SYNC_GETKV2(buffer1[7][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
	
		keyvalue0_vault3.key = mergefunc(keyvalue0_vault2.key, SYNC_GETKV2(buffer2[0][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue0_vault3.value = mergefunc(keyvalue0_vault2.value, SYNC_GETKV2(buffer2[0][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue1_vault3.key = mergefunc(keyvalue1_vault2.key, SYNC_GETKV2(buffer2[1][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue1_vault3.value = mergefunc(keyvalue1_vault2.value, SYNC_GETKV2(buffer2[1][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue2_vault3.key = mergefunc(keyvalue2_vault2.key, SYNC_GETKV2(buffer2[2][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue2_vault3.value = mergefunc(keyvalue2_vault2.value, SYNC_GETKV2(buffer2[2][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue3_vault3.key = mergefunc(keyvalue3_vault2.key, SYNC_GETKV2(buffer2[3][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue3_vault3.value = mergefunc(keyvalue3_vault2.value, SYNC_GETKV2(buffer2[3][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue4_vault3.key = mergefunc(keyvalue4_vault2.key, SYNC_GETKV2(buffer2[4][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue4_vault3.value = mergefunc(keyvalue4_vault2.value, SYNC_GETKV2(buffer2[4][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue5_vault3.key = mergefunc(keyvalue5_vault2.key, SYNC_GETKV2(buffer2[5][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue5_vault3.value = mergefunc(keyvalue5_vault2.value, SYNC_GETKV2(buffer2[5][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue6_vault3.key = mergefunc(keyvalue6_vault2.key, SYNC_GETKV2(buffer2[6][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue6_vault3.value = mergefunc(keyvalue6_vault2.value, SYNC_GETKV2(buffer2[6][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue7_vault3.key = mergefunc(keyvalue7_vault2.key, SYNC_GETKV2(buffer2[7][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue7_vault3.value = mergefunc(keyvalue7_vault2.value, SYNC_GETKV2(buffer2[7][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
	
		keyvalue0_vault4.key = mergefunc(keyvalue0_vault3.key, SYNC_GETKV2(buffer3[0][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue0_vault4.value = mergefunc(keyvalue0_vault3.value, SYNC_GETKV2(buffer3[0][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue1_vault4.key = mergefunc(keyvalue1_vault3.key, SYNC_GETKV2(buffer3[1][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue1_vault4.value = mergefunc(keyvalue1_vault3.value, SYNC_GETKV2(buffer3[1][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue2_vault4.key = mergefunc(keyvalue2_vault3.key, SYNC_GETKV2(buffer3[2][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue2_vault4.value = mergefunc(keyvalue2_vault3.value, SYNC_GETKV2(buffer3[2][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue3_vault4.key = mergefunc(keyvalue3_vault3.key, SYNC_GETKV2(buffer3[3][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue3_vault4.value = mergefunc(keyvalue3_vault3.value, SYNC_GETKV2(buffer3[3][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue4_vault4.key = mergefunc(keyvalue4_vault3.key, SYNC_GETKV2(buffer3[4][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue4_vault4.value = mergefunc(keyvalue4_vault3.value, SYNC_GETKV2(buffer3[4][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue5_vault4.key = mergefunc(keyvalue5_vault3.key, SYNC_GETKV2(buffer3[5][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue5_vault4.value = mergefunc(keyvalue5_vault3.value, SYNC_GETKV2(buffer3[5][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue6_vault4.key = mergefunc(keyvalue6_vault3.key, SYNC_GETKV2(buffer3[6][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue6_vault4.value = mergefunc(keyvalue6_vault3.value, SYNC_GETKV2(buffer3[6][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue7_vault4.key = mergefunc(keyvalue7_vault3.key, SYNC_GETKV2(buffer3[7][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue7_vault4.value = mergefunc(keyvalue7_vault3.value, SYNC_GETKV2(buffer3[7][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
	
		keyvalue0_vault5.key = mergefunc(keyvalue0_vault4.key, SYNC_GETKV2(buffer4[0][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue0_vault5.value = mergefunc(keyvalue0_vault4.value, SYNC_GETKV2(buffer4[0][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue1_vault5.key = mergefunc(keyvalue1_vault4.key, SYNC_GETKV2(buffer4[1][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue1_vault5.value = mergefunc(keyvalue1_vault4.value, SYNC_GETKV2(buffer4[1][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue2_vault5.key = mergefunc(keyvalue2_vault4.key, SYNC_GETKV2(buffer4[2][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue2_vault5.value = mergefunc(keyvalue2_vault4.value, SYNC_GETKV2(buffer4[2][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue3_vault5.key = mergefunc(keyvalue3_vault4.key, SYNC_GETKV2(buffer4[3][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue3_vault5.value = mergefunc(keyvalue3_vault4.value, SYNC_GETKV2(buffer4[3][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue4_vault5.key = mergefunc(keyvalue4_vault4.key, SYNC_GETKV2(buffer4[4][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue4_vault5.value = mergefunc(keyvalue4_vault4.value, SYNC_GETKV2(buffer4[4][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue5_vault5.key = mergefunc(keyvalue5_vault4.key, SYNC_GETKV2(buffer4[5][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue5_vault5.value = mergefunc(keyvalue5_vault4.value, SYNC_GETKV2(buffer4[5][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue6_vault5.key = mergefunc(keyvalue6_vault4.key, SYNC_GETKV2(buffer4[6][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue6_vault5.value = mergefunc(keyvalue6_vault4.value, SYNC_GETKV2(buffer4[6][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue7_vault5.key = mergefunc(keyvalue7_vault4.key, SYNC_GETKV2(buffer4[7][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue7_vault5.value = mergefunc(keyvalue7_vault4.value, SYNC_GETKV2(buffer4[7][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
	
		keyvalue0_vault6.key = mergefunc(keyvalue0_vault5.key, SYNC_GETKV2(buffer5[0][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue0_vault6.value = mergefunc(keyvalue0_vault5.value, SYNC_GETKV2(buffer5[0][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue1_vault6.key = mergefunc(keyvalue1_vault5.key, SYNC_GETKV2(buffer5[1][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue1_vault6.value = mergefunc(keyvalue1_vault5.value, SYNC_GETKV2(buffer5[1][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue2_vault6.key = mergefunc(keyvalue2_vault5.key, SYNC_GETKV2(buffer5[2][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue2_vault6.value = mergefunc(keyvalue2_vault5.value, SYNC_GETKV2(buffer5[2][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue3_vault6.key = mergefunc(keyvalue3_vault5.key, SYNC_GETKV2(buffer5[3][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue3_vault6.value = mergefunc(keyvalue3_vault5.value, SYNC_GETKV2(buffer5[3][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue4_vault6.key = mergefunc(keyvalue4_vault5.key, SYNC_GETKV2(buffer5[4][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue4_vault6.value = mergefunc(keyvalue4_vault5.value, SYNC_GETKV2(buffer5[4][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue5_vault6.key = mergefunc(keyvalue5_vault5.key, SYNC_GETKV2(buffer5[5][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue5_vault6.value = mergefunc(keyvalue5_vault5.value, SYNC_GETKV2(buffer5[5][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue6_vault6.key = mergefunc(keyvalue6_vault5.key, SYNC_GETKV2(buffer5[6][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue6_vault6.value = mergefunc(keyvalue6_vault5.value, SYNC_GETKV2(buffer5[6][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue7_vault6.key = mergefunc(keyvalue7_vault5.key, SYNC_GETKV2(buffer5[7][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue7_vault6.value = mergefunc(keyvalue7_vault5.value, SYNC_GETKV2(buffer5[7][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
	
		keyvalue0_vault7.key = mergefunc(keyvalue0_vault6.key, SYNC_GETKV2(buffer6[0][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue0_vault7.value = mergefunc(keyvalue0_vault6.value, SYNC_GETKV2(buffer6[0][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue1_vault7.key = mergefunc(keyvalue1_vault6.key, SYNC_GETKV2(buffer6[1][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue1_vault7.value = mergefunc(keyvalue1_vault6.value, SYNC_GETKV2(buffer6[1][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue2_vault7.key = mergefunc(keyvalue2_vault6.key, SYNC_GETKV2(buffer6[2][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue2_vault7.value = mergefunc(keyvalue2_vault6.value, SYNC_GETKV2(buffer6[2][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue3_vault7.key = mergefunc(keyvalue3_vault6.key, SYNC_GETKV2(buffer6[3][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue3_vault7.value = mergefunc(keyvalue3_vault6.value, SYNC_GETKV2(buffer6[3][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue4_vault7.key = mergefunc(keyvalue4_vault6.key, SYNC_GETKV2(buffer6[4][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue4_vault7.value = mergefunc(keyvalue4_vault6.value, SYNC_GETKV2(buffer6[4][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue5_vault7.key = mergefunc(keyvalue5_vault6.key, SYNC_GETKV2(buffer6[5][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue5_vault7.value = mergefunc(keyvalue5_vault6.value, SYNC_GETKV2(buffer6[5][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue6_vault7.key = mergefunc(keyvalue6_vault6.key, SYNC_GETKV2(buffer6[6][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue6_vault7.value = mergefunc(keyvalue6_vault6.value, SYNC_GETKV2(buffer6[6][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue7_vault7.key = mergefunc(keyvalue7_vault6.key, SYNC_GETKV2(buffer6[7][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue7_vault7.value = mergefunc(keyvalue7_vault6.value, SYNC_GETKV2(buffer6[7][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
	
		keyvalue0_vault8.key = mergefunc(keyvalue0_vault7.key, SYNC_GETKV2(buffer7[0][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue0_vault8.value = mergefunc(keyvalue0_vault7.value, SYNC_GETKV2(buffer7[0][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue1_vault8.key = mergefunc(keyvalue1_vault7.key, SYNC_GETKV2(buffer7[1][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue1_vault8.value = mergefunc(keyvalue1_vault7.value, SYNC_GETKV2(buffer7[1][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue2_vault8.key = mergefunc(keyvalue2_vault7.key, SYNC_GETKV2(buffer7[2][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue2_vault8.value = mergefunc(keyvalue2_vault7.value, SYNC_GETKV2(buffer7[2][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue3_vault8.key = mergefunc(keyvalue3_vault7.key, SYNC_GETKV2(buffer7[3][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue3_vault8.value = mergefunc(keyvalue3_vault7.value, SYNC_GETKV2(buffer7[3][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue4_vault8.key = mergefunc(keyvalue4_vault7.key, SYNC_GETKV2(buffer7[4][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue4_vault8.value = mergefunc(keyvalue4_vault7.value, SYNC_GETKV2(buffer7[4][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue5_vault8.key = mergefunc(keyvalue5_vault7.key, SYNC_GETKV2(buffer7[5][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue5_vault8.value = mergefunc(keyvalue5_vault7.value, SYNC_GETKV2(buffer7[5][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue6_vault8.key = mergefunc(keyvalue6_vault7.key, SYNC_GETKV2(buffer7[6][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue6_vault8.value = mergefunc(keyvalue6_vault7.value, SYNC_GETKV2(buffer7[6][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
		keyvalue7_vault8.key = mergefunc(keyvalue7_vault7.key, SYNC_GETKV2(buffer7[7][i]).key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue7_vault8.value = mergefunc(keyvalue7_vault7.value, SYNC_GETKV2(buffer7[7][i]).value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	

	
		keyvalue_t mykeyvalue0;
		mykeyvalue0.key = mergefunc(udata0.key, keyvalue0_vault8.key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		mykeyvalue0.value = mergefunc(udata0.value, keyvalue0_vault8.value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res[0][i] = SYNC_GETKV2(mykeyvalue0);
	
		keyvalue_t mykeyvalue1;
		mykeyvalue1.key = mergefunc(udata1.key, keyvalue1_vault8.key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		mykeyvalue1.value = mergefunc(udata1.value, keyvalue1_vault8.value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res[1][i] = SYNC_GETKV2(mykeyvalue1);
	
		keyvalue_t mykeyvalue2;
		mykeyvalue2.key = mergefunc(udata2.key, keyvalue2_vault8.key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		mykeyvalue2.value = mergefunc(udata2.value, keyvalue2_vault8.value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res[2][i] = SYNC_GETKV2(mykeyvalue2);
	
		keyvalue_t mykeyvalue3;
		mykeyvalue3.key = mergefunc(udata3.key, keyvalue3_vault8.key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		mykeyvalue3.value = mergefunc(udata3.value, keyvalue3_vault8.value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res[3][i] = SYNC_GETKV2(mykeyvalue3);
	
		keyvalue_t mykeyvalue4;
		mykeyvalue4.key = mergefunc(udata4.key, keyvalue4_vault8.key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		mykeyvalue4.value = mergefunc(udata4.value, keyvalue4_vault8.value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res[4][i] = SYNC_GETKV2(mykeyvalue4);
	
		keyvalue_t mykeyvalue5;
		mykeyvalue5.key = mergefunc(udata5.key, keyvalue5_vault8.key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		mykeyvalue5.value = mergefunc(udata5.value, keyvalue5_vault8.value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res[5][i] = SYNC_GETKV2(mykeyvalue5);
	
		keyvalue_t mykeyvalue6;
		mykeyvalue6.key = mergefunc(udata6.key, keyvalue6_vault8.key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		mykeyvalue6.value = mergefunc(udata6.value, keyvalue6_vault8.value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res[6][i] = SYNC_GETKV2(mykeyvalue6);
	
		keyvalue_t mykeyvalue7;
		mykeyvalue7.key = mergefunc(udata7.key, keyvalue7_vault8.key, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		mykeyvalue7.value = mergefunc(udata7.value, keyvalue7_vault8.value, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res[7][i] = SYNC_GETKV2(mykeyvalue7);
		
		#ifdef _DEBUGMODE_KERNELPRINTS //
		if(SYNC_GETKV2(res[0][i]).key < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(0*REDUCESZ + 2*i)<<endl; } 
		if(SYNC_GETKV2(res[0][i]).value < SYNC_GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(0*REDUCESZ + 2*i + 1)<<endl; } 
		if(SYNC_GETKV2(res[1][i]).key < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(1*REDUCESZ + 2*i)<<endl; } 
		if(SYNC_GETKV2(res[1][i]).value < SYNC_GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(1*REDUCESZ + 2*i + 1)<<endl; } 
		if(SYNC_GETKV2(res[2][i]).key < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(2*REDUCESZ + 2*i)<<endl; } 
		if(SYNC_GETKV2(res[2][i]).value < SYNC_GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(2*REDUCESZ + 2*i + 1)<<endl; } 
		if(SYNC_GETKV2(res[3][i]).key < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(3*REDUCESZ + 2*i)<<endl; } 
		if(SYNC_GETKV2(res[3][i]).value < SYNC_GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(3*REDUCESZ + 2*i + 1)<<endl; } 
		if(SYNC_GETKV2(res[4][i]).key < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(4*REDUCESZ + 2*i)<<endl; } 
		if(SYNC_GETKV2(res[4][i]).value < SYNC_GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(4*REDUCESZ + 2*i + 1)<<endl; } 
		if(SYNC_GETKV2(res[5][i]).key < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(5*REDUCESZ + 2*i)<<endl; } 
		if(SYNC_GETKV2(res[5][i]).value < SYNC_GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(5*REDUCESZ + 2*i + 1)<<endl; } 
		if(SYNC_GETKV2(res[6][i]).key < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(6*REDUCESZ + 2*i)<<endl; } 
		if(SYNC_GETKV2(res[6][i]).value < SYNC_GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(6*REDUCESZ + 2*i + 1)<<endl; } 
		if(SYNC_GETKV2(res[7][i]).key < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(7*REDUCESZ + 2*i)<<endl; } 
		if(SYNC_GETKV2(res[7][i]).value < SYNC_GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(7*REDUCESZ + 2*i + 1)<<endl; } 
		#endif
		
		// apply
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3 // _DEBUGMODE_CHECKS2
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
	
		#endif 
		
		keyvalue_t data0 = keyvalue0_vault8;
		keyvalue_t data1 = keyvalue1_vault8;
		keyvalue_t data2 = keyvalue2_vault8;
		keyvalue_t data3 = keyvalue3_vault8;
		keyvalue_t data4 = keyvalue4_vault8;
		keyvalue_t data5 = keyvalue5_vault8;
		keyvalue_t data6 = keyvalue6_vault8;
		keyvalue_t data7 = keyvalue7_vault8;
	
	
		res0.key = applyfunc(udata0.key, data0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res1.key = applyfunc(udata1.key, data1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res2.key = applyfunc(udata2.key, data2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res3.key = applyfunc(udata3.key, data3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res4.key = applyfunc(udata4.key, data4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res5.key = applyfunc(udata5.key, data5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res6.key = applyfunc(udata6.key, data6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res7.key = applyfunc(udata7.key, data7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res0.value = applyfunc(udata0.value, data0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res1.value = applyfunc(udata1.value, data1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res2.value = applyfunc(udata2.value, data2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res3.value = applyfunc(udata3.value, data3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res4.value = applyfunc(udata4.value, data4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res5.value = applyfunc(udata5.value, data5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res6.value = applyfunc(udata6.value, data6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		res7.value = applyfunc(udata7.value, data7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		
		if(res0.key != udata0.key){ vmask[i].data[colindex + 0].key = 1; } else { vmask[i].data[colindex + 0].key = 0; }
		if(res0.value != udata0.value){ vmask[i].data[colindex + 0].value = 1; } else { vmask[i].data[colindex + 0].value = 0; }
		if(res1.key != udata1.key){ vmask[i].data[colindex + 1].key = 1; } else { vmask[i].data[colindex + 1].key = 0; }
		if(res1.value != udata1.value){ vmask[i].data[colindex + 1].value = 1; } else { vmask[i].data[colindex + 1].value = 0; }
		if(res2.key != udata2.key){ vmask[i].data[colindex + 2].key = 1; } else { vmask[i].data[colindex + 2].key = 0; }
		if(res2.value != udata2.value){ vmask[i].data[colindex + 2].value = 1; } else { vmask[i].data[colindex + 2].value = 0; }
		if(res3.key != udata3.key){ vmask[i].data[colindex + 3].key = 1; } else { vmask[i].data[colindex + 3].key = 0; }
		if(res3.value != udata3.value){ vmask[i].data[colindex + 3].value = 1; } else { vmask[i].data[colindex + 3].value = 0; }
		if(res4.key != udata4.key){ vmask[i].data[colindex + 4].key = 1; } else { vmask[i].data[colindex + 4].key = 0; }
		if(res4.value != udata4.value){ vmask[i].data[colindex + 4].value = 1; } else { vmask[i].data[colindex + 4].value = 0; }
		if(res5.key != udata5.key){ vmask[i].data[colindex + 5].key = 1; } else { vmask[i].data[colindex + 5].key = 0; }
		if(res5.value != udata5.value){ vmask[i].data[colindex + 5].value = 1; } else { vmask[i].data[colindex + 5].value = 0; }
		if(res6.key != udata6.key){ vmask[i].data[colindex + 6].key = 1; } else { vmask[i].data[colindex + 6].key = 0; }
		if(res6.value != udata6.value){ vmask[i].data[colindex + 6].value = 1; } else { vmask[i].data[colindex + 6].value = 0; }
		if(res7.key != udata7.key){ vmask[i].data[colindex + 7].key = 1; } else { vmask[i].data[colindex + 7].key = 0; }
		if(res7.value != udata7.value){ vmask[i].data[colindex + 7].value = 1; } else { vmask[i].data[colindex + 7].value = 0; }
		
		uint32_type mask = convertvmasktouint32(vmask, i);
		cummvmask_sp = cummvmask_sp | mask;
		
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3	
		if(false){ cout<<"SYNC AND APPLY FUNC SEEN @ (vid1: "<<vid[0][0]<<" & vid2: "<<vid[0][1]<<"): res0.key: "<<res0.key<<", res0.value: "<<res0.value<<": udata0.key: "<<udata0.key<<", udata0.value: "<<udata0.value<<", data0.key: "<<data0.key<<", data0.value: "<<data0.value<<endl; }
		if(res0.key != udata0.key){ cout<<"synchronizeandapply: vid[0][0]: "<<vid[0][0]<<", vmask["<<i<<"].data["<<colindex + 0<<"].key: "<<vmask[i].data[colindex + 0].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res0.value != udata0.value){ cout<<"synchronizeandapply: vid[0][1]: "<<vid[0][1]<<", vmask["<<i<<"].data["<<colindex + 0<<"].value: "<<vmask[i].data[colindex + 0].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(false){ cout<<"SYNC AND APPLY FUNC SEEN @ (vid1: "<<vid[1][0]<<" & vid2: "<<vid[1][1]<<"): res1.key: "<<res1.key<<", res1.value: "<<res1.value<<": udata1.key: "<<udata1.key<<", udata1.value: "<<udata1.value<<", data1.key: "<<data1.key<<", data1.value: "<<data1.value<<endl; }
		if(res1.key != udata1.key){ cout<<"synchronizeandapply: vid[1][0]: "<<vid[1][0]<<", vmask["<<i<<"].data["<<colindex + 1<<"].key: "<<vmask[i].data[colindex + 1].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res1.value != udata1.value){ cout<<"synchronizeandapply: vid[1][1]: "<<vid[1][1]<<", vmask["<<i<<"].data["<<colindex + 1<<"].value: "<<vmask[i].data[colindex + 1].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(false){ cout<<"SYNC AND APPLY FUNC SEEN @ (vid1: "<<vid[2][0]<<" & vid2: "<<vid[2][1]<<"): res2.key: "<<res2.key<<", res2.value: "<<res2.value<<": udata2.key: "<<udata2.key<<", udata2.value: "<<udata2.value<<", data2.key: "<<data2.key<<", data2.value: "<<data2.value<<endl; }
		if(res2.key != udata2.key){ cout<<"synchronizeandapply: vid[2][0]: "<<vid[2][0]<<", vmask["<<i<<"].data["<<colindex + 2<<"].key: "<<vmask[i].data[colindex + 2].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res2.value != udata2.value){ cout<<"synchronizeandapply: vid[2][1]: "<<vid[2][1]<<", vmask["<<i<<"].data["<<colindex + 2<<"].value: "<<vmask[i].data[colindex + 2].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(false){ cout<<"SYNC AND APPLY FUNC SEEN @ (vid1: "<<vid[3][0]<<" & vid2: "<<vid[3][1]<<"): res3.key: "<<res3.key<<", res3.value: "<<res3.value<<": udata3.key: "<<udata3.key<<", udata3.value: "<<udata3.value<<", data3.key: "<<data3.key<<", data3.value: "<<data3.value<<endl; }
		if(res3.key != udata3.key){ cout<<"synchronizeandapply: vid[3][0]: "<<vid[3][0]<<", vmask["<<i<<"].data["<<colindex + 3<<"].key: "<<vmask[i].data[colindex + 3].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res3.value != udata3.value){ cout<<"synchronizeandapply: vid[3][1]: "<<vid[3][1]<<", vmask["<<i<<"].data["<<colindex + 3<<"].value: "<<vmask[i].data[colindex + 3].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(false){ cout<<"SYNC AND APPLY FUNC SEEN @ (vid1: "<<vid[4][0]<<" & vid2: "<<vid[4][1]<<"): res4.key: "<<res4.key<<", res4.value: "<<res4.value<<": udata4.key: "<<udata4.key<<", udata4.value: "<<udata4.value<<", data4.key: "<<data4.key<<", data4.value: "<<data4.value<<endl; }
		if(res4.key != udata4.key){ cout<<"synchronizeandapply: vid[4][0]: "<<vid[4][0]<<", vmask["<<i<<"].data["<<colindex + 4<<"].key: "<<vmask[i].data[colindex + 4].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res4.value != udata4.value){ cout<<"synchronizeandapply: vid[4][1]: "<<vid[4][1]<<", vmask["<<i<<"].data["<<colindex + 4<<"].value: "<<vmask[i].data[colindex + 4].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(false){ cout<<"SYNC AND APPLY FUNC SEEN @ (vid1: "<<vid[5][0]<<" & vid2: "<<vid[5][1]<<"): res5.key: "<<res5.key<<", res5.value: "<<res5.value<<": udata5.key: "<<udata5.key<<", udata5.value: "<<udata5.value<<", data5.key: "<<data5.key<<", data5.value: "<<data5.value<<endl; }
		if(res5.key != udata5.key){ cout<<"synchronizeandapply: vid[5][0]: "<<vid[5][0]<<", vmask["<<i<<"].data["<<colindex + 5<<"].key: "<<vmask[i].data[colindex + 5].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res5.value != udata5.value){ cout<<"synchronizeandapply: vid[5][1]: "<<vid[5][1]<<", vmask["<<i<<"].data["<<colindex + 5<<"].value: "<<vmask[i].data[colindex + 5].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(false){ cout<<"SYNC AND APPLY FUNC SEEN @ (vid1: "<<vid[6][0]<<" & vid2: "<<vid[6][1]<<"): res6.key: "<<res6.key<<", res6.value: "<<res6.value<<": udata6.key: "<<udata6.key<<", udata6.value: "<<udata6.value<<", data6.key: "<<data6.key<<", data6.value: "<<data6.value<<endl; }
		if(res6.key != udata6.key){ cout<<"synchronizeandapply: vid[6][0]: "<<vid[6][0]<<", vmask["<<i<<"].data["<<colindex + 6<<"].key: "<<vmask[i].data[colindex + 6].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res6.value != udata6.value){ cout<<"synchronizeandapply: vid[6][1]: "<<vid[6][1]<<", vmask["<<i<<"].data["<<colindex + 6<<"].value: "<<vmask[i].data[colindex + 6].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(false){ cout<<"SYNC AND APPLY FUNC SEEN @ (vid1: "<<vid[7][0]<<" & vid2: "<<vid[7][1]<<"): res7.key: "<<res7.key<<", res7.value: "<<res7.value<<": udata7.key: "<<udata7.key<<", udata7.value: "<<udata7.value<<", data7.key: "<<data7.key<<", data7.value: "<<data7.value<<endl; }
		if(res7.key != udata7.key){ cout<<"synchronizeandapply: vid[7][0]: "<<vid[7][0]<<", vmask["<<i<<"].data["<<colindex + 7<<"].key: "<<vmask[i].data[colindex + 7].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res7.value != udata7.value){ cout<<"synchronizeandapply: vid[7][1]: "<<vid[7][1]<<", vmask["<<i<<"].data["<<colindex + 7<<"].value: "<<vmask[i].data[colindex + 7].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
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
		#endif
	}
	#endif
	return cummvmask_sp;
}

void
	#ifdef SW 
	actssync::
	#endif
spreadvdata(bool_type enable1, bool_type enable2, keyvalue_vbuffer_t source[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], globalparams_t globalparams){
	#pragma HLS function_instantiate variable=source
	if(enable1 == OFF || enable2 == OFF){ return; }
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
	cout<<"spreadvdata: spread (vdata) function called."<<endl;
	#endif 
	analysis_type analysis_loopcount = REDUCESZ / 2;
	
	buffer_type reducebuffersz = globalparams.SIZE_REDUCE / 2;
	
	keyvalue_t keyvalue0_vault0;
	keyvalue_t keyvalue1_vault0;
	keyvalue_t keyvalue2_vault0;
	keyvalue_t keyvalue3_vault0;
	keyvalue_t keyvalue4_vault0;
	keyvalue_t keyvalue5_vault0;
	keyvalue_t keyvalue6_vault0;
	keyvalue_t keyvalue7_vault0;
	
	keyvalue_t keyvalue0_vault1;
	keyvalue_t keyvalue1_vault1;
	keyvalue_t keyvalue2_vault1;
	keyvalue_t keyvalue3_vault1;
	keyvalue_t keyvalue4_vault1;
	keyvalue_t keyvalue5_vault1;
	keyvalue_t keyvalue6_vault1;
	keyvalue_t keyvalue7_vault1;
	
	keyvalue_t keyvalue0_vault2;
	keyvalue_t keyvalue1_vault2;
	keyvalue_t keyvalue2_vault2;
	keyvalue_t keyvalue3_vault2;
	keyvalue_t keyvalue4_vault2;
	keyvalue_t keyvalue5_vault2;
	keyvalue_t keyvalue6_vault2;
	keyvalue_t keyvalue7_vault2;
	
	keyvalue_t keyvalue0_vault3;
	keyvalue_t keyvalue1_vault3;
	keyvalue_t keyvalue2_vault3;
	keyvalue_t keyvalue3_vault3;
	keyvalue_t keyvalue4_vault3;
	keyvalue_t keyvalue5_vault3;
	keyvalue_t keyvalue6_vault3;
	keyvalue_t keyvalue7_vault3;
	
	keyvalue_t keyvalue0_vault4;
	keyvalue_t keyvalue1_vault4;
	keyvalue_t keyvalue2_vault4;
	keyvalue_t keyvalue3_vault4;
	keyvalue_t keyvalue4_vault4;
	keyvalue_t keyvalue5_vault4;
	keyvalue_t keyvalue6_vault4;
	keyvalue_t keyvalue7_vault4;
	
	keyvalue_t keyvalue0_vault5;
	keyvalue_t keyvalue1_vault5;
	keyvalue_t keyvalue2_vault5;
	keyvalue_t keyvalue3_vault5;
	keyvalue_t keyvalue4_vault5;
	keyvalue_t keyvalue5_vault5;
	keyvalue_t keyvalue6_vault5;
	keyvalue_t keyvalue7_vault5;
	
	keyvalue_t keyvalue0_vault6;
	keyvalue_t keyvalue1_vault6;
	keyvalue_t keyvalue2_vault6;
	keyvalue_t keyvalue3_vault6;
	keyvalue_t keyvalue4_vault6;
	keyvalue_t keyvalue5_vault6;
	keyvalue_t keyvalue6_vault6;
	keyvalue_t keyvalue7_vault6;
	
	keyvalue_t keyvalue0_vault7;
	keyvalue_t keyvalue1_vault7;
	keyvalue_t keyvalue2_vault7;
	keyvalue_t keyvalue3_vault7;
	keyvalue_t keyvalue4_vault7;
	keyvalue_t keyvalue5_vault7;
	keyvalue_t keyvalue6_vault7;
	keyvalue_t keyvalue7_vault7;
	
	keyvalue_t keyvalue0_vault8;
	keyvalue_t keyvalue1_vault8;
	keyvalue_t keyvalue2_vault8;
	keyvalue_t keyvalue3_vault8;
	keyvalue_t keyvalue4_vault8;
	keyvalue_t keyvalue5_vault8;
	keyvalue_t keyvalue6_vault8;
	keyvalue_t keyvalue7_vault8;
	
	
	for (buffer_type i=0; i<reducebuffersz; i++){ // 8, 16, BLOCKRAM_SIZE
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE II=1
		keyvalue0_vault0 = SYNC_GETKV2(source[0][i]); 
		keyvalue1_vault0 = SYNC_GETKV2(source[1][i]); 
		keyvalue2_vault0 = SYNC_GETKV2(source[2][i]); 
		keyvalue3_vault0 = SYNC_GETKV2(source[3][i]); 
		keyvalue4_vault0 = SYNC_GETKV2(source[4][i]); 
		keyvalue5_vault0 = SYNC_GETKV2(source[5][i]); 
		keyvalue6_vault0 = SYNC_GETKV2(source[6][i]); 
		keyvalue7_vault0 = SYNC_GETKV2(source[7][i]); 
			
		buffer0[0][i] = SYNC_GETKV2(keyvalue0_vault0);
		keyvalue0_vault1 = keyvalue0_vault0;
		buffer0[1][i] = SYNC_GETKV2(keyvalue1_vault0);
		keyvalue1_vault1 = keyvalue1_vault0;
		buffer0[2][i] = SYNC_GETKV2(keyvalue2_vault0);
		keyvalue2_vault1 = keyvalue2_vault0;
		buffer0[3][i] = SYNC_GETKV2(keyvalue3_vault0);
		keyvalue3_vault1 = keyvalue3_vault0;
		buffer0[4][i] = SYNC_GETKV2(keyvalue4_vault0);
		keyvalue4_vault1 = keyvalue4_vault0;
		buffer0[5][i] = SYNC_GETKV2(keyvalue5_vault0);
		keyvalue5_vault1 = keyvalue5_vault0;
		buffer0[6][i] = SYNC_GETKV2(keyvalue6_vault0);
		keyvalue6_vault1 = keyvalue6_vault0;
		buffer0[7][i] = SYNC_GETKV2(keyvalue7_vault0);
		keyvalue7_vault1 = keyvalue7_vault0;
	
		buffer1[0][i] = SYNC_GETKV2(keyvalue0_vault1);
		keyvalue0_vault2 = keyvalue0_vault1;
		buffer1[1][i] = SYNC_GETKV2(keyvalue1_vault1);
		keyvalue1_vault2 = keyvalue1_vault1;
		buffer1[2][i] = SYNC_GETKV2(keyvalue2_vault1);
		keyvalue2_vault2 = keyvalue2_vault1;
		buffer1[3][i] = SYNC_GETKV2(keyvalue3_vault1);
		keyvalue3_vault2 = keyvalue3_vault1;
		buffer1[4][i] = SYNC_GETKV2(keyvalue4_vault1);
		keyvalue4_vault2 = keyvalue4_vault1;
		buffer1[5][i] = SYNC_GETKV2(keyvalue5_vault1);
		keyvalue5_vault2 = keyvalue5_vault1;
		buffer1[6][i] = SYNC_GETKV2(keyvalue6_vault1);
		keyvalue6_vault2 = keyvalue6_vault1;
		buffer1[7][i] = SYNC_GETKV2(keyvalue7_vault1);
		keyvalue7_vault2 = keyvalue7_vault1;
	
		buffer2[0][i] = SYNC_GETKV2(keyvalue0_vault2);
		keyvalue0_vault3 = keyvalue0_vault2;
		buffer2[1][i] = SYNC_GETKV2(keyvalue1_vault2);
		keyvalue1_vault3 = keyvalue1_vault2;
		buffer2[2][i] = SYNC_GETKV2(keyvalue2_vault2);
		keyvalue2_vault3 = keyvalue2_vault2;
		buffer2[3][i] = SYNC_GETKV2(keyvalue3_vault2);
		keyvalue3_vault3 = keyvalue3_vault2;
		buffer2[4][i] = SYNC_GETKV2(keyvalue4_vault2);
		keyvalue4_vault3 = keyvalue4_vault2;
		buffer2[5][i] = SYNC_GETKV2(keyvalue5_vault2);
		keyvalue5_vault3 = keyvalue5_vault2;
		buffer2[6][i] = SYNC_GETKV2(keyvalue6_vault2);
		keyvalue6_vault3 = keyvalue6_vault2;
		buffer2[7][i] = SYNC_GETKV2(keyvalue7_vault2);
		keyvalue7_vault3 = keyvalue7_vault2;
	
		buffer3[0][i] = SYNC_GETKV2(keyvalue0_vault3);
		keyvalue0_vault4 = keyvalue0_vault3;
		buffer3[1][i] = SYNC_GETKV2(keyvalue1_vault3);
		keyvalue1_vault4 = keyvalue1_vault3;
		buffer3[2][i] = SYNC_GETKV2(keyvalue2_vault3);
		keyvalue2_vault4 = keyvalue2_vault3;
		buffer3[3][i] = SYNC_GETKV2(keyvalue3_vault3);
		keyvalue3_vault4 = keyvalue3_vault3;
		buffer3[4][i] = SYNC_GETKV2(keyvalue4_vault3);
		keyvalue4_vault4 = keyvalue4_vault3;
		buffer3[5][i] = SYNC_GETKV2(keyvalue5_vault3);
		keyvalue5_vault4 = keyvalue5_vault3;
		buffer3[6][i] = SYNC_GETKV2(keyvalue6_vault3);
		keyvalue6_vault4 = keyvalue6_vault3;
		buffer3[7][i] = SYNC_GETKV2(keyvalue7_vault3);
		keyvalue7_vault4 = keyvalue7_vault3;
	
		buffer4[0][i] = SYNC_GETKV2(keyvalue0_vault4);
		keyvalue0_vault5 = keyvalue0_vault4;
		buffer4[1][i] = SYNC_GETKV2(keyvalue1_vault4);
		keyvalue1_vault5 = keyvalue1_vault4;
		buffer4[2][i] = SYNC_GETKV2(keyvalue2_vault4);
		keyvalue2_vault5 = keyvalue2_vault4;
		buffer4[3][i] = SYNC_GETKV2(keyvalue3_vault4);
		keyvalue3_vault5 = keyvalue3_vault4;
		buffer4[4][i] = SYNC_GETKV2(keyvalue4_vault4);
		keyvalue4_vault5 = keyvalue4_vault4;
		buffer4[5][i] = SYNC_GETKV2(keyvalue5_vault4);
		keyvalue5_vault5 = keyvalue5_vault4;
		buffer4[6][i] = SYNC_GETKV2(keyvalue6_vault4);
		keyvalue6_vault5 = keyvalue6_vault4;
		buffer4[7][i] = SYNC_GETKV2(keyvalue7_vault4);
		keyvalue7_vault5 = keyvalue7_vault4;
	
		buffer5[0][i] = SYNC_GETKV2(keyvalue0_vault5);
		keyvalue0_vault6 = keyvalue0_vault5;
		buffer5[1][i] = SYNC_GETKV2(keyvalue1_vault5);
		keyvalue1_vault6 = keyvalue1_vault5;
		buffer5[2][i] = SYNC_GETKV2(keyvalue2_vault5);
		keyvalue2_vault6 = keyvalue2_vault5;
		buffer5[3][i] = SYNC_GETKV2(keyvalue3_vault5);
		keyvalue3_vault6 = keyvalue3_vault5;
		buffer5[4][i] = SYNC_GETKV2(keyvalue4_vault5);
		keyvalue4_vault6 = keyvalue4_vault5;
		buffer5[5][i] = SYNC_GETKV2(keyvalue5_vault5);
		keyvalue5_vault6 = keyvalue5_vault5;
		buffer5[6][i] = SYNC_GETKV2(keyvalue6_vault5);
		keyvalue6_vault6 = keyvalue6_vault5;
		buffer5[7][i] = SYNC_GETKV2(keyvalue7_vault5);
		keyvalue7_vault6 = keyvalue7_vault5;
	
		buffer6[0][i] = SYNC_GETKV2(keyvalue0_vault6);
		keyvalue0_vault7 = keyvalue0_vault6;
		buffer6[1][i] = SYNC_GETKV2(keyvalue1_vault6);
		keyvalue1_vault7 = keyvalue1_vault6;
		buffer6[2][i] = SYNC_GETKV2(keyvalue2_vault6);
		keyvalue2_vault7 = keyvalue2_vault6;
		buffer6[3][i] = SYNC_GETKV2(keyvalue3_vault6);
		keyvalue3_vault7 = keyvalue3_vault6;
		buffer6[4][i] = SYNC_GETKV2(keyvalue4_vault6);
		keyvalue4_vault7 = keyvalue4_vault6;
		buffer6[5][i] = SYNC_GETKV2(keyvalue5_vault6);
		keyvalue5_vault7 = keyvalue5_vault6;
		buffer6[6][i] = SYNC_GETKV2(keyvalue6_vault6);
		keyvalue6_vault7 = keyvalue6_vault6;
		buffer6[7][i] = SYNC_GETKV2(keyvalue7_vault6);
		keyvalue7_vault7 = keyvalue7_vault6;
	
		buffer7[0][i] = SYNC_GETKV2(keyvalue0_vault7);
		keyvalue0_vault8 = keyvalue0_vault7;
		buffer7[1][i] = SYNC_GETKV2(keyvalue1_vault7);
		keyvalue1_vault8 = keyvalue1_vault7;
		buffer7[2][i] = SYNC_GETKV2(keyvalue2_vault7);
		keyvalue2_vault8 = keyvalue2_vault7;
		buffer7[3][i] = SYNC_GETKV2(keyvalue3_vault7);
		keyvalue3_vault8 = keyvalue3_vault7;
		buffer7[4][i] = SYNC_GETKV2(keyvalue4_vault7);
		keyvalue4_vault8 = keyvalue4_vault7;
		buffer7[5][i] = SYNC_GETKV2(keyvalue5_vault7);
		keyvalue5_vault8 = keyvalue5_vault7;
		buffer7[6][i] = SYNC_GETKV2(keyvalue6_vault7);
		keyvalue6_vault8 = keyvalue6_vault7;
		buffer7[7][i] = SYNC_GETKV2(keyvalue7_vault7);
		keyvalue7_vault8 = keyvalue7_vault7;
	
		
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
		if(SYNC_GETKV2(buffer7[0][i]).key < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(0*REDUCESZ + 2*i)<<endl; } 
		if(SYNC_GETKV2(buffer7[0][i]).value < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(0*REDUCESZ + 2*i + 1)<<endl; } 
		if(SYNC_GETKV2(buffer7[1][i]).key < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(1*REDUCESZ + 2*i)<<endl; } 
		if(SYNC_GETKV2(buffer7[1][i]).value < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(1*REDUCESZ + 2*i + 1)<<endl; } 
		if(SYNC_GETKV2(buffer7[2][i]).key < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(2*REDUCESZ + 2*i)<<endl; } 
		if(SYNC_GETKV2(buffer7[2][i]).value < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(2*REDUCESZ + 2*i + 1)<<endl; } 
		if(SYNC_GETKV2(buffer7[3][i]).key < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(3*REDUCESZ + 2*i)<<endl; } 
		if(SYNC_GETKV2(buffer7[3][i]).value < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(3*REDUCESZ + 2*i + 1)<<endl; } 
		if(SYNC_GETKV2(buffer7[4][i]).key < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(4*REDUCESZ + 2*i)<<endl; } 
		if(SYNC_GETKV2(buffer7[4][i]).value < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(4*REDUCESZ + 2*i + 1)<<endl; } 
		if(SYNC_GETKV2(buffer7[5][i]).key < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(5*REDUCESZ + 2*i)<<endl; } 
		if(SYNC_GETKV2(buffer7[5][i]).value < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(5*REDUCESZ + 2*i + 1)<<endl; } 
		if(SYNC_GETKV2(buffer7[6][i]).key < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(6*REDUCESZ + 2*i)<<endl; } 
		if(SYNC_GETKV2(buffer7[6][i]).value < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(6*REDUCESZ + 2*i + 1)<<endl; } 
		if(SYNC_GETKV2(buffer7[7][i]).key < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(7*REDUCESZ + 2*i)<<endl; } 
		if(SYNC_GETKV2(buffer7[7][i]).value < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(7*REDUCESZ + 2*i + 1)<<endl; } 
		#endif
	}
	return;
}

void
	#ifdef SW 
	actssync::
	#endif
spreadvmask(bool_type enable1, bool_type enable2, bool_type enable3, unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unitBRAMwidth_type vmask0[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5[BLOCKRAM_SIZE],unitBRAMwidth_type vmask6[BLOCKRAM_SIZE],unitBRAMwidth_type vmask7[BLOCKRAM_SIZE], globalparams_t globalparams){
	#pragma HLS INLINE OFF //
	if(enable1 == OFF || enable2 == OFF || enable3 == OFF){ return; }
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
	cout<<"spreadvmask: spread (vmask) function called."<<endl;
	#endif 
	analysis_type analysis_loopcount = BLOCKRAM_SIZE;
	
	unitBRAMwidth_type vmask_vault0;
	unitBRAMwidth_type vmask_vault1;
	unitBRAMwidth_type vmask_vault2;
	unitBRAMwidth_type vmask_vault3;
	unitBRAMwidth_type vmask_vault4;
	unitBRAMwidth_type vmask_vault5;
	unitBRAMwidth_type vmask_vault6;
	unitBRAMwidth_type vmask_vault7;
	unitBRAMwidth_type vmask_vault8;
	unitBRAMwidth_type vmask_vault9;
	
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
	
	}
	return;
}

void
	#ifdef SW 
	actssync::
	#endif
spreadandwrite(bool_type enable1, bool_type enable2, bool_type enable_savevmask, bool_type enable_savevmaskp, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,		keyvalue_vbuffer_t source[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type vbuffer_offset_kvs, buffer_type vbuffer_size_kvs, 
		unitBRAMwidth_type vmask[BLOCKRAM_SIZE], batch_type vmask_offset_kvs, buffer_type vmask_size_kvs, 
		unsigned int vmaskp_offset_kvs, uint32_type vmask_p_temp[2],
		globalparams_t globalparams){
	#pragma HLS function_instantiate variable=source
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
	cout<<"spreadandwrite: spread (vdata) function called."<<endl;
	#endif 
	analysis_type analysis_loopcount = REDUCESZ / 2;
	analysis_type analysis_loopcount2 = BLOCKRAM_SIZE;
	analysis_type analysis_loopcount3 = BLOCKRAM_SIZE / 16;
	
	buffer_type reducebuffersz = globalparams.SIZE_REDUCE / 2;
	
	uint32_type tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE]; // prepare vmask variables
	#pragma HLS array_partition variable = tempbuffer
	uint32_type bitsbuffer[MAXREDUCEBUFFERSZ];
	buffer_type transfsize = vmask_size_kvs * 16;
	
	keyvalue_t keyvalue0_vault0;
	keyvalue_t keyvalue1_vault0;
	keyvalue_t keyvalue2_vault0;
	keyvalue_t keyvalue3_vault0;
	keyvalue_t keyvalue4_vault0;
	keyvalue_t keyvalue5_vault0;
	keyvalue_t keyvalue6_vault0;
	keyvalue_t keyvalue7_vault0;
	
	keyvalue_t keyvalue0_vault1;
	keyvalue_t keyvalue1_vault1;
	keyvalue_t keyvalue2_vault1;
	keyvalue_t keyvalue3_vault1;
	keyvalue_t keyvalue4_vault1;
	keyvalue_t keyvalue5_vault1;
	keyvalue_t keyvalue6_vault1;
	keyvalue_t keyvalue7_vault1;
	
	keyvalue_t keyvalue0_vault2;
	keyvalue_t keyvalue1_vault2;
	keyvalue_t keyvalue2_vault2;
	keyvalue_t keyvalue3_vault2;
	keyvalue_t keyvalue4_vault2;
	keyvalue_t keyvalue5_vault2;
	keyvalue_t keyvalue6_vault2;
	keyvalue_t keyvalue7_vault2;
	
	keyvalue_t keyvalue0_vault3;
	keyvalue_t keyvalue1_vault3;
	keyvalue_t keyvalue2_vault3;
	keyvalue_t keyvalue3_vault3;
	keyvalue_t keyvalue4_vault3;
	keyvalue_t keyvalue5_vault3;
	keyvalue_t keyvalue6_vault3;
	keyvalue_t keyvalue7_vault3;
	
	keyvalue_t keyvalue0_vault4;
	keyvalue_t keyvalue1_vault4;
	keyvalue_t keyvalue2_vault4;
	keyvalue_t keyvalue3_vault4;
	keyvalue_t keyvalue4_vault4;
	keyvalue_t keyvalue5_vault4;
	keyvalue_t keyvalue6_vault4;
	keyvalue_t keyvalue7_vault4;
	
	
	if(enable1 == ON && enable2 == ON){
	SPREADANDSAVE_COMBINEDWITH_PREPAREVMASKS_LOOP: for (buffer_type i=0; i<reducebuffersz; i++){ // 8, 16, BLOCKRAM_SIZE
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE II=1
		// spread operation
		keyvalue0_vault0 = SYNC_GETKV2(source[0][i]); 
		keyvalue1_vault0 = SYNC_GETKV2(source[1][i]); 
		keyvalue2_vault0 = SYNC_GETKV2(source[2][i]); 
		keyvalue3_vault0 = SYNC_GETKV2(source[3][i]); 
		keyvalue4_vault0 = SYNC_GETKV2(source[4][i]); 
		keyvalue5_vault0 = SYNC_GETKV2(source[5][i]); 
		keyvalue6_vault0 = SYNC_GETKV2(source[6][i]); 
		keyvalue7_vault0 = SYNC_GETKV2(source[7][i]); 
		
		#ifdef _WIDEWORD
		kvdram0[vbuffer_offset_kvs + i].range(31, 0) = keyvalue0_vault0.key; 
		kvdram0[vbuffer_offset_kvs + i].range(63, 32) = keyvalue0_vault0.value; 
		keyvalue0_vault1 = keyvalue0_vault0;
		kvdram0[vbuffer_offset_kvs + i].range(95, 64) = keyvalue1_vault0.key; 
		kvdram0[vbuffer_offset_kvs + i].range(127, 96) = keyvalue1_vault0.value; 
		keyvalue1_vault1 = keyvalue1_vault0;
		kvdram0[vbuffer_offset_kvs + i].range(159, 128) = keyvalue2_vault0.key; 
		kvdram0[vbuffer_offset_kvs + i].range(191, 160) = keyvalue2_vault0.value; 
		keyvalue2_vault1 = keyvalue2_vault0;
		kvdram0[vbuffer_offset_kvs + i].range(223, 192) = keyvalue3_vault0.key; 
		kvdram0[vbuffer_offset_kvs + i].range(255, 224) = keyvalue3_vault0.value; 
		keyvalue3_vault1 = keyvalue3_vault0;
		kvdram0[vbuffer_offset_kvs + i].range(287, 256) = keyvalue4_vault0.key; 
		kvdram0[vbuffer_offset_kvs + i].range(319, 288) = keyvalue4_vault0.value; 
		keyvalue4_vault1 = keyvalue4_vault0;
		kvdram0[vbuffer_offset_kvs + i].range(351, 320) = keyvalue5_vault0.key; 
		kvdram0[vbuffer_offset_kvs + i].range(383, 352) = keyvalue5_vault0.value; 
		keyvalue5_vault1 = keyvalue5_vault0;
		kvdram0[vbuffer_offset_kvs + i].range(415, 384) = keyvalue6_vault0.key; 
		kvdram0[vbuffer_offset_kvs + i].range(447, 416) = keyvalue6_vault0.value; 
		keyvalue6_vault1 = keyvalue6_vault0;
		kvdram0[vbuffer_offset_kvs + i].range(479, 448) = keyvalue7_vault0.key; 
		kvdram0[vbuffer_offset_kvs + i].range(511, 480) = keyvalue7_vault0.value; 
		keyvalue7_vault1 = keyvalue7_vault0;
		kvdram1[vbuffer_offset_kvs + i].range(31, 0) = keyvalue0_vault1.key; 
		kvdram1[vbuffer_offset_kvs + i].range(63, 32) = keyvalue0_vault1.value; 
		keyvalue0_vault2 = keyvalue0_vault1;
		kvdram1[vbuffer_offset_kvs + i].range(95, 64) = keyvalue1_vault1.key; 
		kvdram1[vbuffer_offset_kvs + i].range(127, 96) = keyvalue1_vault1.value; 
		keyvalue1_vault2 = keyvalue1_vault1;
		kvdram1[vbuffer_offset_kvs + i].range(159, 128) = keyvalue2_vault1.key; 
		kvdram1[vbuffer_offset_kvs + i].range(191, 160) = keyvalue2_vault1.value; 
		keyvalue2_vault2 = keyvalue2_vault1;
		kvdram1[vbuffer_offset_kvs + i].range(223, 192) = keyvalue3_vault1.key; 
		kvdram1[vbuffer_offset_kvs + i].range(255, 224) = keyvalue3_vault1.value; 
		keyvalue3_vault2 = keyvalue3_vault1;
		kvdram1[vbuffer_offset_kvs + i].range(287, 256) = keyvalue4_vault1.key; 
		kvdram1[vbuffer_offset_kvs + i].range(319, 288) = keyvalue4_vault1.value; 
		keyvalue4_vault2 = keyvalue4_vault1;
		kvdram1[vbuffer_offset_kvs + i].range(351, 320) = keyvalue5_vault1.key; 
		kvdram1[vbuffer_offset_kvs + i].range(383, 352) = keyvalue5_vault1.value; 
		keyvalue5_vault2 = keyvalue5_vault1;
		kvdram1[vbuffer_offset_kvs + i].range(415, 384) = keyvalue6_vault1.key; 
		kvdram1[vbuffer_offset_kvs + i].range(447, 416) = keyvalue6_vault1.value; 
		keyvalue6_vault2 = keyvalue6_vault1;
		kvdram1[vbuffer_offset_kvs + i].range(479, 448) = keyvalue7_vault1.key; 
		kvdram1[vbuffer_offset_kvs + i].range(511, 480) = keyvalue7_vault1.value; 
		keyvalue7_vault2 = keyvalue7_vault1;
		kvdram2[vbuffer_offset_kvs + i].range(31, 0) = keyvalue0_vault2.key; 
		kvdram2[vbuffer_offset_kvs + i].range(63, 32) = keyvalue0_vault2.value; 
		keyvalue0_vault3 = keyvalue0_vault2;
		kvdram2[vbuffer_offset_kvs + i].range(95, 64) = keyvalue1_vault2.key; 
		kvdram2[vbuffer_offset_kvs + i].range(127, 96) = keyvalue1_vault2.value; 
		keyvalue1_vault3 = keyvalue1_vault2;
		kvdram2[vbuffer_offset_kvs + i].range(159, 128) = keyvalue2_vault2.key; 
		kvdram2[vbuffer_offset_kvs + i].range(191, 160) = keyvalue2_vault2.value; 
		keyvalue2_vault3 = keyvalue2_vault2;
		kvdram2[vbuffer_offset_kvs + i].range(223, 192) = keyvalue3_vault2.key; 
		kvdram2[vbuffer_offset_kvs + i].range(255, 224) = keyvalue3_vault2.value; 
		keyvalue3_vault3 = keyvalue3_vault2;
		kvdram2[vbuffer_offset_kvs + i].range(287, 256) = keyvalue4_vault2.key; 
		kvdram2[vbuffer_offset_kvs + i].range(319, 288) = keyvalue4_vault2.value; 
		keyvalue4_vault3 = keyvalue4_vault2;
		kvdram2[vbuffer_offset_kvs + i].range(351, 320) = keyvalue5_vault2.key; 
		kvdram2[vbuffer_offset_kvs + i].range(383, 352) = keyvalue5_vault2.value; 
		keyvalue5_vault3 = keyvalue5_vault2;
		kvdram2[vbuffer_offset_kvs + i].range(415, 384) = keyvalue6_vault2.key; 
		kvdram2[vbuffer_offset_kvs + i].range(447, 416) = keyvalue6_vault2.value; 
		keyvalue6_vault3 = keyvalue6_vault2;
		kvdram2[vbuffer_offset_kvs + i].range(479, 448) = keyvalue7_vault2.key; 
		kvdram2[vbuffer_offset_kvs + i].range(511, 480) = keyvalue7_vault2.value; 
		keyvalue7_vault3 = keyvalue7_vault2;
		kvdram3[vbuffer_offset_kvs + i].range(31, 0) = keyvalue0_vault3.key; 
		kvdram3[vbuffer_offset_kvs + i].range(63, 32) = keyvalue0_vault3.value; 
		keyvalue0_vault4 = keyvalue0_vault3;
		kvdram3[vbuffer_offset_kvs + i].range(95, 64) = keyvalue1_vault3.key; 
		kvdram3[vbuffer_offset_kvs + i].range(127, 96) = keyvalue1_vault3.value; 
		keyvalue1_vault4 = keyvalue1_vault3;
		kvdram3[vbuffer_offset_kvs + i].range(159, 128) = keyvalue2_vault3.key; 
		kvdram3[vbuffer_offset_kvs + i].range(191, 160) = keyvalue2_vault3.value; 
		keyvalue2_vault4 = keyvalue2_vault3;
		kvdram3[vbuffer_offset_kvs + i].range(223, 192) = keyvalue3_vault3.key; 
		kvdram3[vbuffer_offset_kvs + i].range(255, 224) = keyvalue3_vault3.value; 
		keyvalue3_vault4 = keyvalue3_vault3;
		kvdram3[vbuffer_offset_kvs + i].range(287, 256) = keyvalue4_vault3.key; 
		kvdram3[vbuffer_offset_kvs + i].range(319, 288) = keyvalue4_vault3.value; 
		keyvalue4_vault4 = keyvalue4_vault3;
		kvdram3[vbuffer_offset_kvs + i].range(351, 320) = keyvalue5_vault3.key; 
		kvdram3[vbuffer_offset_kvs + i].range(383, 352) = keyvalue5_vault3.value; 
		keyvalue5_vault4 = keyvalue5_vault3;
		kvdram3[vbuffer_offset_kvs + i].range(415, 384) = keyvalue6_vault3.key; 
		kvdram3[vbuffer_offset_kvs + i].range(447, 416) = keyvalue6_vault3.value; 
		keyvalue6_vault4 = keyvalue6_vault3;
		kvdram3[vbuffer_offset_kvs + i].range(479, 448) = keyvalue7_vault3.key; 
		kvdram3[vbuffer_offset_kvs + i].range(511, 480) = keyvalue7_vault3.value; 
		keyvalue7_vault4 = keyvalue7_vault3;
		#else 
		kvdram0[vbuffer_offset_kvs + i].data[0].key = keyvalue0_vault0.key;
		kvdram0[vbuffer_offset_kvs + i].data[0].value = keyvalue0_vault0.value;
		keyvalue0_vault1 = keyvalue0_vault0;
		kvdram0[vbuffer_offset_kvs + i].data[1].key = keyvalue1_vault0.key;
		kvdram0[vbuffer_offset_kvs + i].data[1].value = keyvalue1_vault0.value;
		keyvalue1_vault1 = keyvalue1_vault0;
		kvdram0[vbuffer_offset_kvs + i].data[2].key = keyvalue2_vault0.key;
		kvdram0[vbuffer_offset_kvs + i].data[2].value = keyvalue2_vault0.value;
		keyvalue2_vault1 = keyvalue2_vault0;
		kvdram0[vbuffer_offset_kvs + i].data[3].key = keyvalue3_vault0.key;
		kvdram0[vbuffer_offset_kvs + i].data[3].value = keyvalue3_vault0.value;
		keyvalue3_vault1 = keyvalue3_vault0;
		kvdram0[vbuffer_offset_kvs + i].data[4].key = keyvalue4_vault0.key;
		kvdram0[vbuffer_offset_kvs + i].data[4].value = keyvalue4_vault0.value;
		keyvalue4_vault1 = keyvalue4_vault0;
		kvdram0[vbuffer_offset_kvs + i].data[5].key = keyvalue5_vault0.key;
		kvdram0[vbuffer_offset_kvs + i].data[5].value = keyvalue5_vault0.value;
		keyvalue5_vault1 = keyvalue5_vault0;
		kvdram0[vbuffer_offset_kvs + i].data[6].key = keyvalue6_vault0.key;
		kvdram0[vbuffer_offset_kvs + i].data[6].value = keyvalue6_vault0.value;
		keyvalue6_vault1 = keyvalue6_vault0;
		kvdram0[vbuffer_offset_kvs + i].data[7].key = keyvalue7_vault0.key;
		kvdram0[vbuffer_offset_kvs + i].data[7].value = keyvalue7_vault0.value;
		keyvalue7_vault1 = keyvalue7_vault0;
		kvdram1[vbuffer_offset_kvs + i].data[0].key = keyvalue0_vault1.key;
		kvdram1[vbuffer_offset_kvs + i].data[0].value = keyvalue0_vault1.value;
		keyvalue0_vault2 = keyvalue0_vault1;
		kvdram1[vbuffer_offset_kvs + i].data[1].key = keyvalue1_vault1.key;
		kvdram1[vbuffer_offset_kvs + i].data[1].value = keyvalue1_vault1.value;
		keyvalue1_vault2 = keyvalue1_vault1;
		kvdram1[vbuffer_offset_kvs + i].data[2].key = keyvalue2_vault1.key;
		kvdram1[vbuffer_offset_kvs + i].data[2].value = keyvalue2_vault1.value;
		keyvalue2_vault2 = keyvalue2_vault1;
		kvdram1[vbuffer_offset_kvs + i].data[3].key = keyvalue3_vault1.key;
		kvdram1[vbuffer_offset_kvs + i].data[3].value = keyvalue3_vault1.value;
		keyvalue3_vault2 = keyvalue3_vault1;
		kvdram1[vbuffer_offset_kvs + i].data[4].key = keyvalue4_vault1.key;
		kvdram1[vbuffer_offset_kvs + i].data[4].value = keyvalue4_vault1.value;
		keyvalue4_vault2 = keyvalue4_vault1;
		kvdram1[vbuffer_offset_kvs + i].data[5].key = keyvalue5_vault1.key;
		kvdram1[vbuffer_offset_kvs + i].data[5].value = keyvalue5_vault1.value;
		keyvalue5_vault2 = keyvalue5_vault1;
		kvdram1[vbuffer_offset_kvs + i].data[6].key = keyvalue6_vault1.key;
		kvdram1[vbuffer_offset_kvs + i].data[6].value = keyvalue6_vault1.value;
		keyvalue6_vault2 = keyvalue6_vault1;
		kvdram1[vbuffer_offset_kvs + i].data[7].key = keyvalue7_vault1.key;
		kvdram1[vbuffer_offset_kvs + i].data[7].value = keyvalue7_vault1.value;
		keyvalue7_vault2 = keyvalue7_vault1;
		kvdram2[vbuffer_offset_kvs + i].data[0].key = keyvalue0_vault2.key;
		kvdram2[vbuffer_offset_kvs + i].data[0].value = keyvalue0_vault2.value;
		keyvalue0_vault3 = keyvalue0_vault2;
		kvdram2[vbuffer_offset_kvs + i].data[1].key = keyvalue1_vault2.key;
		kvdram2[vbuffer_offset_kvs + i].data[1].value = keyvalue1_vault2.value;
		keyvalue1_vault3 = keyvalue1_vault2;
		kvdram2[vbuffer_offset_kvs + i].data[2].key = keyvalue2_vault2.key;
		kvdram2[vbuffer_offset_kvs + i].data[2].value = keyvalue2_vault2.value;
		keyvalue2_vault3 = keyvalue2_vault2;
		kvdram2[vbuffer_offset_kvs + i].data[3].key = keyvalue3_vault2.key;
		kvdram2[vbuffer_offset_kvs + i].data[3].value = keyvalue3_vault2.value;
		keyvalue3_vault3 = keyvalue3_vault2;
		kvdram2[vbuffer_offset_kvs + i].data[4].key = keyvalue4_vault2.key;
		kvdram2[vbuffer_offset_kvs + i].data[4].value = keyvalue4_vault2.value;
		keyvalue4_vault3 = keyvalue4_vault2;
		kvdram2[vbuffer_offset_kvs + i].data[5].key = keyvalue5_vault2.key;
		kvdram2[vbuffer_offset_kvs + i].data[5].value = keyvalue5_vault2.value;
		keyvalue5_vault3 = keyvalue5_vault2;
		kvdram2[vbuffer_offset_kvs + i].data[6].key = keyvalue6_vault2.key;
		kvdram2[vbuffer_offset_kvs + i].data[6].value = keyvalue6_vault2.value;
		keyvalue6_vault3 = keyvalue6_vault2;
		kvdram2[vbuffer_offset_kvs + i].data[7].key = keyvalue7_vault2.key;
		kvdram2[vbuffer_offset_kvs + i].data[7].value = keyvalue7_vault2.value;
		keyvalue7_vault3 = keyvalue7_vault2;
		kvdram3[vbuffer_offset_kvs + i].data[0].key = keyvalue0_vault3.key;
		kvdram3[vbuffer_offset_kvs + i].data[0].value = keyvalue0_vault3.value;
		keyvalue0_vault4 = keyvalue0_vault3;
		kvdram3[vbuffer_offset_kvs + i].data[1].key = keyvalue1_vault3.key;
		kvdram3[vbuffer_offset_kvs + i].data[1].value = keyvalue1_vault3.value;
		keyvalue1_vault4 = keyvalue1_vault3;
		kvdram3[vbuffer_offset_kvs + i].data[2].key = keyvalue2_vault3.key;
		kvdram3[vbuffer_offset_kvs + i].data[2].value = keyvalue2_vault3.value;
		keyvalue2_vault4 = keyvalue2_vault3;
		kvdram3[vbuffer_offset_kvs + i].data[3].key = keyvalue3_vault3.key;
		kvdram3[vbuffer_offset_kvs + i].data[3].value = keyvalue3_vault3.value;
		keyvalue3_vault4 = keyvalue3_vault3;
		kvdram3[vbuffer_offset_kvs + i].data[4].key = keyvalue4_vault3.key;
		kvdram3[vbuffer_offset_kvs + i].data[4].value = keyvalue4_vault3.value;
		keyvalue4_vault4 = keyvalue4_vault3;
		kvdram3[vbuffer_offset_kvs + i].data[5].key = keyvalue5_vault3.key;
		kvdram3[vbuffer_offset_kvs + i].data[5].value = keyvalue5_vault3.value;
		keyvalue5_vault4 = keyvalue5_vault3;
		kvdram3[vbuffer_offset_kvs + i].data[6].key = keyvalue6_vault3.key;
		kvdram3[vbuffer_offset_kvs + i].data[6].value = keyvalue6_vault3.value;
		keyvalue6_vault4 = keyvalue6_vault3;
		kvdram3[vbuffer_offset_kvs + i].data[7].key = keyvalue7_vault3.key;
		kvdram3[vbuffer_offset_kvs + i].data[7].value = keyvalue7_vault3.value;
		keyvalue7_vault4 = keyvalue7_vault3;
		#endif 
		
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
		if(SYNC_GETKEYENTRY(kvdram3[vbuffer_offset_kvs + i], 0) < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(0*REDUCESZ + 2*i)<<endl; } 
		if(GETVALUEENTRY(kvdram3[vbuffer_offset_kvs + i], 0) < SYNC_GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(0*REDUCESZ + 2*i + 1)<<endl; } 
		if(SYNC_GETKEYENTRY(kvdram3[vbuffer_offset_kvs + i], 1) < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(1*REDUCESZ + 2*i)<<endl; } 
		if(GETVALUEENTRY(kvdram3[vbuffer_offset_kvs + i], 1) < SYNC_GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(1*REDUCESZ + 2*i + 1)<<endl; } 
		if(SYNC_GETKEYENTRY(kvdram3[vbuffer_offset_kvs + i], 2) < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(2*REDUCESZ + 2*i)<<endl; } 
		if(GETVALUEENTRY(kvdram3[vbuffer_offset_kvs + i], 2) < SYNC_GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(2*REDUCESZ + 2*i + 1)<<endl; } 
		if(SYNC_GETKEYENTRY(kvdram3[vbuffer_offset_kvs + i], 3) < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(3*REDUCESZ + 2*i)<<endl; } 
		if(GETVALUEENTRY(kvdram3[vbuffer_offset_kvs + i], 3) < SYNC_GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(3*REDUCESZ + 2*i + 1)<<endl; } 
		if(SYNC_GETKEYENTRY(kvdram3[vbuffer_offset_kvs + i], 4) < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(4*REDUCESZ + 2*i)<<endl; } 
		if(GETVALUEENTRY(kvdram3[vbuffer_offset_kvs + i], 4) < SYNC_GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(4*REDUCESZ + 2*i + 1)<<endl; } 
		if(SYNC_GETKEYENTRY(kvdram3[vbuffer_offset_kvs + i], 5) < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(5*REDUCESZ + 2*i)<<endl; } 
		if(GETVALUEENTRY(kvdram3[vbuffer_offset_kvs + i], 5) < SYNC_GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(5*REDUCESZ + 2*i + 1)<<endl; } 
		if(SYNC_GETKEYENTRY(kvdram3[vbuffer_offset_kvs + i], 6) < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(6*REDUCESZ + 2*i)<<endl; } 
		if(GETVALUEENTRY(kvdram3[vbuffer_offset_kvs + i], 6) < SYNC_GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(6*REDUCESZ + 2*i + 1)<<endl; } 
		if(SYNC_GETKEYENTRY(kvdram3[vbuffer_offset_kvs + i], 7) < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(7*REDUCESZ + 2*i)<<endl; } 
		if(GETVALUEENTRY(kvdram3[vbuffer_offset_kvs + i], 7) < SYNC_GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(7*REDUCESZ + 2*i + 1)<<endl; } 
		#endif
		
		// combined with prepare vmask operation
		unsigned int X = i % 16;
		unsigned int Y = i / 16;
	
		#ifdef _WIDEWORD
		tempbuffer[X][Y].range(0, 0) = vmask[i].data[0].key;
		tempbuffer[X][Y].range(1, 1) = vmask[i].data[0].value;
		tempbuffer[X][Y].range(2, 2) = vmask[i].data[1].key;
		tempbuffer[X][Y].range(3, 3) = vmask[i].data[1].value;
		tempbuffer[X][Y].range(4, 4) = vmask[i].data[2].key;
		tempbuffer[X][Y].range(5, 5) = vmask[i].data[2].value;
		tempbuffer[X][Y].range(6, 6) = vmask[i].data[3].key;
		tempbuffer[X][Y].range(7, 7) = vmask[i].data[3].value;
		tempbuffer[X][Y].range(8, 8) = vmask[i].data[4].key;
		tempbuffer[X][Y].range(9, 9) = vmask[i].data[4].value;
		tempbuffer[X][Y].range(10, 10) = vmask[i].data[5].key;
		tempbuffer[X][Y].range(11, 11) = vmask[i].data[5].value;
		tempbuffer[X][Y].range(12, 12) = vmask[i].data[6].key;
		tempbuffer[X][Y].range(13, 13) = vmask[i].data[6].value;
		tempbuffer[X][Y].range(14, 14) = vmask[i].data[7].key;
		tempbuffer[X][Y].range(15, 15) = vmask[i].data[7].value;
		tempbuffer[X][Y].range(16, 16) = vmask[i].data[8].key;
		tempbuffer[X][Y].range(17, 17) = vmask[i].data[8].value;
		tempbuffer[X][Y].range(18, 18) = vmask[i].data[9].key;
		tempbuffer[X][Y].range(19, 19) = vmask[i].data[9].value;
		tempbuffer[X][Y].range(20, 20) = vmask[i].data[10].key;
		tempbuffer[X][Y].range(21, 21) = vmask[i].data[10].value;
		tempbuffer[X][Y].range(22, 22) = vmask[i].data[11].key;
		tempbuffer[X][Y].range(23, 23) = vmask[i].data[11].value;
		tempbuffer[X][Y].range(24, 24) = vmask[i].data[12].key;
		tempbuffer[X][Y].range(25, 25) = vmask[i].data[12].value;
		tempbuffer[X][Y].range(26, 26) = vmask[i].data[13].key;
		tempbuffer[X][Y].range(27, 27) = vmask[i].data[13].value;
		tempbuffer[X][Y].range(28, 28) = vmask[i].data[14].key;
		tempbuffer[X][Y].range(29, 29) = vmask[i].data[14].value;
		tempbuffer[X][Y].range(30, 30) = vmask[i].data[15].key;
		tempbuffer[X][Y].range(31, 31) = vmask[i].data[15].value;
		#else
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 0, 1, vmask[i].data[0].key);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 1, 1, vmask[i].data[0].value);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 2, 1, vmask[i].data[1].key);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 3, 1, vmask[i].data[1].value);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 4, 1, vmask[i].data[2].key);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 5, 1, vmask[i].data[2].value);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 6, 1, vmask[i].data[3].key);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 7, 1, vmask[i].data[3].value);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 8, 1, vmask[i].data[4].key);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 9, 1, vmask[i].data[4].value);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 10, 1, vmask[i].data[5].key);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 11, 1, vmask[i].data[5].value);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 12, 1, vmask[i].data[6].key);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 13, 1, vmask[i].data[6].value);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 14, 1, vmask[i].data[7].key);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 15, 1, vmask[i].data[7].value);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 16, 1, vmask[i].data[8].key);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 17, 1, vmask[i].data[8].value);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 18, 1, vmask[i].data[9].key);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 19, 1, vmask[i].data[9].value);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 20, 1, vmask[i].data[10].key);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 21, 1, vmask[i].data[10].value);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 22, 1, vmask[i].data[11].key);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 23, 1, vmask[i].data[11].value);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 24, 1, vmask[i].data[12].key);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 25, 1, vmask[i].data[12].value);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 26, 1, vmask[i].data[13].key);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 27, 1, vmask[i].data[13].value);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 28, 1, vmask[i].data[14].key);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 29, 1, vmask[i].data[14].value);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 30, 1, vmask[i].data[15].key);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 31, 1, vmask[i].data[15].value);
		#endif
	}
	
	if(enable_savevmask == ON){
	SAVEVMASKS_LOOP: for(buffer_type i=0; i<vmask_size_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount3 avg=analysis_loopcount3
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		kvdram0[vmask_offset_kvs + i].range(31, 0) = tempbuffer[0][i]; 
		kvdram0[vmask_offset_kvs + i].range(63, 32) = tempbuffer[1][i]; 
		kvdram0[vmask_offset_kvs + i].range(95, 64) = tempbuffer[2][i]; 
		kvdram0[vmask_offset_kvs + i].range(127, 96) = tempbuffer[3][i]; 
		kvdram0[vmask_offset_kvs + i].range(159, 128) = tempbuffer[4][i]; 
		kvdram0[vmask_offset_kvs + i].range(191, 160) = tempbuffer[5][i]; 
		kvdram0[vmask_offset_kvs + i].range(223, 192) = tempbuffer[6][i]; 
		kvdram0[vmask_offset_kvs + i].range(255, 224) = tempbuffer[7][i]; 
		kvdram0[vmask_offset_kvs + i].range(287, 256) = tempbuffer[8][i]; 
		kvdram0[vmask_offset_kvs + i].range(319, 288) = tempbuffer[9][i]; 
		kvdram0[vmask_offset_kvs + i].range(351, 320) = tempbuffer[10][i]; 
		kvdram0[vmask_offset_kvs + i].range(383, 352) = tempbuffer[11][i]; 
		kvdram0[vmask_offset_kvs + i].range(415, 384) = tempbuffer[12][i]; 
		kvdram0[vmask_offset_kvs + i].range(447, 416) = tempbuffer[13][i]; 
		kvdram0[vmask_offset_kvs + i].range(479, 448) = tempbuffer[14][i]; 
		kvdram0[vmask_offset_kvs + i].range(511, 480) = tempbuffer[15][i]; 
		kvdram1[vmask_offset_kvs + i].range(31, 0) = tempbuffer[0][i]; 
		kvdram1[vmask_offset_kvs + i].range(63, 32) = tempbuffer[1][i]; 
		kvdram1[vmask_offset_kvs + i].range(95, 64) = tempbuffer[2][i]; 
		kvdram1[vmask_offset_kvs + i].range(127, 96) = tempbuffer[3][i]; 
		kvdram1[vmask_offset_kvs + i].range(159, 128) = tempbuffer[4][i]; 
		kvdram1[vmask_offset_kvs + i].range(191, 160) = tempbuffer[5][i]; 
		kvdram1[vmask_offset_kvs + i].range(223, 192) = tempbuffer[6][i]; 
		kvdram1[vmask_offset_kvs + i].range(255, 224) = tempbuffer[7][i]; 
		kvdram1[vmask_offset_kvs + i].range(287, 256) = tempbuffer[8][i]; 
		kvdram1[vmask_offset_kvs + i].range(319, 288) = tempbuffer[9][i]; 
		kvdram1[vmask_offset_kvs + i].range(351, 320) = tempbuffer[10][i]; 
		kvdram1[vmask_offset_kvs + i].range(383, 352) = tempbuffer[11][i]; 
		kvdram1[vmask_offset_kvs + i].range(415, 384) = tempbuffer[12][i]; 
		kvdram1[vmask_offset_kvs + i].range(447, 416) = tempbuffer[13][i]; 
		kvdram1[vmask_offset_kvs + i].range(479, 448) = tempbuffer[14][i]; 
		kvdram1[vmask_offset_kvs + i].range(511, 480) = tempbuffer[15][i]; 
		kvdram2[vmask_offset_kvs + i].range(31, 0) = tempbuffer[0][i]; 
		kvdram2[vmask_offset_kvs + i].range(63, 32) = tempbuffer[1][i]; 
		kvdram2[vmask_offset_kvs + i].range(95, 64) = tempbuffer[2][i]; 
		kvdram2[vmask_offset_kvs + i].range(127, 96) = tempbuffer[3][i]; 
		kvdram2[vmask_offset_kvs + i].range(159, 128) = tempbuffer[4][i]; 
		kvdram2[vmask_offset_kvs + i].range(191, 160) = tempbuffer[5][i]; 
		kvdram2[vmask_offset_kvs + i].range(223, 192) = tempbuffer[6][i]; 
		kvdram2[vmask_offset_kvs + i].range(255, 224) = tempbuffer[7][i]; 
		kvdram2[vmask_offset_kvs + i].range(287, 256) = tempbuffer[8][i]; 
		kvdram2[vmask_offset_kvs + i].range(319, 288) = tempbuffer[9][i]; 
		kvdram2[vmask_offset_kvs + i].range(351, 320) = tempbuffer[10][i]; 
		kvdram2[vmask_offset_kvs + i].range(383, 352) = tempbuffer[11][i]; 
		kvdram2[vmask_offset_kvs + i].range(415, 384) = tempbuffer[12][i]; 
		kvdram2[vmask_offset_kvs + i].range(447, 416) = tempbuffer[13][i]; 
		kvdram2[vmask_offset_kvs + i].range(479, 448) = tempbuffer[14][i]; 
		kvdram2[vmask_offset_kvs + i].range(511, 480) = tempbuffer[15][i]; 
		kvdram3[vmask_offset_kvs + i].range(31, 0) = tempbuffer[0][i]; 
		kvdram3[vmask_offset_kvs + i].range(63, 32) = tempbuffer[1][i]; 
		kvdram3[vmask_offset_kvs + i].range(95, 64) = tempbuffer[2][i]; 
		kvdram3[vmask_offset_kvs + i].range(127, 96) = tempbuffer[3][i]; 
		kvdram3[vmask_offset_kvs + i].range(159, 128) = tempbuffer[4][i]; 
		kvdram3[vmask_offset_kvs + i].range(191, 160) = tempbuffer[5][i]; 
		kvdram3[vmask_offset_kvs + i].range(223, 192) = tempbuffer[6][i]; 
		kvdram3[vmask_offset_kvs + i].range(255, 224) = tempbuffer[7][i]; 
		kvdram3[vmask_offset_kvs + i].range(287, 256) = tempbuffer[8][i]; 
		kvdram3[vmask_offset_kvs + i].range(319, 288) = tempbuffer[9][i]; 
		kvdram3[vmask_offset_kvs + i].range(351, 320) = tempbuffer[10][i]; 
		kvdram3[vmask_offset_kvs + i].range(383, 352) = tempbuffer[11][i]; 
		kvdram3[vmask_offset_kvs + i].range(415, 384) = tempbuffer[12][i]; 
		kvdram3[vmask_offset_kvs + i].range(447, 416) = tempbuffer[13][i]; 
		kvdram3[vmask_offset_kvs + i].range(479, 448) = tempbuffer[14][i]; 
		kvdram3[vmask_offset_kvs + i].range(511, 480) = tempbuffer[15][i]; 
		#else 
		kvdram0[vmask_offset_kvs + i].data[0].key = tempbuffer[0][i];
		kvdram0[vmask_offset_kvs + i].data[0].value = tempbuffer[1][i]; 
		kvdram0[vmask_offset_kvs + i].data[1].key = tempbuffer[2][i];
		kvdram0[vmask_offset_kvs + i].data[1].value = tempbuffer[3][i]; 
		kvdram0[vmask_offset_kvs + i].data[2].key = tempbuffer[4][i];
		kvdram0[vmask_offset_kvs + i].data[2].value = tempbuffer[5][i]; 
		kvdram0[vmask_offset_kvs + i].data[3].key = tempbuffer[6][i];
		kvdram0[vmask_offset_kvs + i].data[3].value = tempbuffer[7][i]; 
		kvdram0[vmask_offset_kvs + i].data[4].key = tempbuffer[8][i];
		kvdram0[vmask_offset_kvs + i].data[4].value = tempbuffer[9][i]; 
		kvdram0[vmask_offset_kvs + i].data[5].key = tempbuffer[10][i];
		kvdram0[vmask_offset_kvs + i].data[5].value = tempbuffer[11][i]; 
		kvdram0[vmask_offset_kvs + i].data[6].key = tempbuffer[12][i];
		kvdram0[vmask_offset_kvs + i].data[6].value = tempbuffer[13][i]; 
		kvdram0[vmask_offset_kvs + i].data[7].key = tempbuffer[14][i];
		kvdram0[vmask_offset_kvs + i].data[7].value = tempbuffer[15][i]; 
		kvdram1[vmask_offset_kvs + i].data[0].key = tempbuffer[0][i];
		kvdram1[vmask_offset_kvs + i].data[0].value = tempbuffer[1][i]; 
		kvdram1[vmask_offset_kvs + i].data[1].key = tempbuffer[2][i];
		kvdram1[vmask_offset_kvs + i].data[1].value = tempbuffer[3][i]; 
		kvdram1[vmask_offset_kvs + i].data[2].key = tempbuffer[4][i];
		kvdram1[vmask_offset_kvs + i].data[2].value = tempbuffer[5][i]; 
		kvdram1[vmask_offset_kvs + i].data[3].key = tempbuffer[6][i];
		kvdram1[vmask_offset_kvs + i].data[3].value = tempbuffer[7][i]; 
		kvdram1[vmask_offset_kvs + i].data[4].key = tempbuffer[8][i];
		kvdram1[vmask_offset_kvs + i].data[4].value = tempbuffer[9][i]; 
		kvdram1[vmask_offset_kvs + i].data[5].key = tempbuffer[10][i];
		kvdram1[vmask_offset_kvs + i].data[5].value = tempbuffer[11][i]; 
		kvdram1[vmask_offset_kvs + i].data[6].key = tempbuffer[12][i];
		kvdram1[vmask_offset_kvs + i].data[6].value = tempbuffer[13][i]; 
		kvdram1[vmask_offset_kvs + i].data[7].key = tempbuffer[14][i];
		kvdram1[vmask_offset_kvs + i].data[7].value = tempbuffer[15][i]; 
		kvdram2[vmask_offset_kvs + i].data[0].key = tempbuffer[0][i];
		kvdram2[vmask_offset_kvs + i].data[0].value = tempbuffer[1][i]; 
		kvdram2[vmask_offset_kvs + i].data[1].key = tempbuffer[2][i];
		kvdram2[vmask_offset_kvs + i].data[1].value = tempbuffer[3][i]; 
		kvdram2[vmask_offset_kvs + i].data[2].key = tempbuffer[4][i];
		kvdram2[vmask_offset_kvs + i].data[2].value = tempbuffer[5][i]; 
		kvdram2[vmask_offset_kvs + i].data[3].key = tempbuffer[6][i];
		kvdram2[vmask_offset_kvs + i].data[3].value = tempbuffer[7][i]; 
		kvdram2[vmask_offset_kvs + i].data[4].key = tempbuffer[8][i];
		kvdram2[vmask_offset_kvs + i].data[4].value = tempbuffer[9][i]; 
		kvdram2[vmask_offset_kvs + i].data[5].key = tempbuffer[10][i];
		kvdram2[vmask_offset_kvs + i].data[5].value = tempbuffer[11][i]; 
		kvdram2[vmask_offset_kvs + i].data[6].key = tempbuffer[12][i];
		kvdram2[vmask_offset_kvs + i].data[6].value = tempbuffer[13][i]; 
		kvdram2[vmask_offset_kvs + i].data[7].key = tempbuffer[14][i];
		kvdram2[vmask_offset_kvs + i].data[7].value = tempbuffer[15][i]; 
		kvdram3[vmask_offset_kvs + i].data[0].key = tempbuffer[0][i];
		kvdram3[vmask_offset_kvs + i].data[0].value = tempbuffer[1][i]; 
		kvdram3[vmask_offset_kvs + i].data[1].key = tempbuffer[2][i];
		kvdram3[vmask_offset_kvs + i].data[1].value = tempbuffer[3][i]; 
		kvdram3[vmask_offset_kvs + i].data[2].key = tempbuffer[4][i];
		kvdram3[vmask_offset_kvs + i].data[2].value = tempbuffer[5][i]; 
		kvdram3[vmask_offset_kvs + i].data[3].key = tempbuffer[6][i];
		kvdram3[vmask_offset_kvs + i].data[3].value = tempbuffer[7][i]; 
		kvdram3[vmask_offset_kvs + i].data[4].key = tempbuffer[8][i];
		kvdram3[vmask_offset_kvs + i].data[4].value = tempbuffer[9][i]; 
		kvdram3[vmask_offset_kvs + i].data[5].key = tempbuffer[10][i];
		kvdram3[vmask_offset_kvs + i].data[5].value = tempbuffer[11][i]; 
		kvdram3[vmask_offset_kvs + i].data[6].key = tempbuffer[12][i];
		kvdram3[vmask_offset_kvs + i].data[6].value = tempbuffer[13][i]; 
		kvdram3[vmask_offset_kvs + i].data[7].key = tempbuffer[14][i];
		kvdram3[vmask_offset_kvs + i].data[7].value = tempbuffer[15][i]; 
		#endif 
		
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvswritten(VECTOR_SIZE);
		#endif
	}
	}
	}
	
	// savevmaskp
	if(enable_savevmaskp == ON){
	uint32_type vmask_p = 0;
	if(enable1 == ON && enable2 == ON){ vmask_p = vmask_p_temp[0] | vmask_p_temp[1]; }
	#ifdef _WIDEWORD
	kvdram0[vmaskp_offset_kvs].range(31, 0) = vmask_p;
	kvdram1[vmaskp_offset_kvs].range(31, 0) = vmask_p;
	kvdram2[vmaskp_offset_kvs].range(31, 0) = vmask_p;
	kvdram3[vmaskp_offset_kvs].range(31, 0) = vmask_p;
	#else
	kvdram0[vmaskp_offset_kvs].data[0].key = vmask_p;
	kvdram1[vmaskp_offset_kvs].data[0].key = vmask_p;
	kvdram2[vmaskp_offset_kvs].data[0].key = vmask_p;
	kvdram3[vmaskp_offset_kvs].data[0].key = vmask_p;
	#endif
	}
	return;
}

void
	#ifdef SW 
	actssync::
	#endif
spreadandwrite(bool_type enable1, bool_type enable2, bool_type enable_savevmask, bool_type enable_savevmaskp, uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,		keyvalue_vbuffer_t source[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type vbuffer_offset_kvs, buffer_type vbuffer_size_kvs, 
		unitBRAMwidth_type vmask[BLOCKRAM_SIZE], batch_type vmask_offset_kvs, buffer_type vmask_size_kvs, 
		unsigned int vmaskp_offset_kvs, uint32_type vmask_p_temp[2],
		globalparams_t globalparams){
	#pragma HLS function_instantiate variable=source
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
	cout<<"spreadandwrite: spread and write function called."<<endl;
	#endif 
	analysis_type analysis_loopcount = REDUCESZ / 2;
	analysis_type analysis_loopcount2 = BLOCKRAM_SIZE;
	analysis_type analysis_loopcount3 = BLOCKRAM_SIZE / 16;
	
	buffer_type reducebuffersz = globalparams.SIZE_REDUCE / 2;
	
	uint32_type tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE]; // prepare vmask variables
	#pragma HLS array_partition variable = tempbuffer
	uint32_type bitsbuffer[MAXREDUCEBUFFERSZ];
	buffer_type transfsize = vmask_size_kvs * 16;
	
	keyvalue_t keyvalue0_vault0;
	keyvalue_t keyvalue1_vault0;
	keyvalue_t keyvalue2_vault0;
	keyvalue_t keyvalue3_vault0;
	keyvalue_t keyvalue4_vault0;
	keyvalue_t keyvalue5_vault0;
	keyvalue_t keyvalue6_vault0;
	keyvalue_t keyvalue7_vault0;
	
	keyvalue_t keyvalue0_vault1;
	keyvalue_t keyvalue1_vault1;
	keyvalue_t keyvalue2_vault1;
	keyvalue_t keyvalue3_vault1;
	keyvalue_t keyvalue4_vault1;
	keyvalue_t keyvalue5_vault1;
	keyvalue_t keyvalue6_vault1;
	keyvalue_t keyvalue7_vault1;
	
	keyvalue_t keyvalue0_vault2;
	keyvalue_t keyvalue1_vault2;
	keyvalue_t keyvalue2_vault2;
	keyvalue_t keyvalue3_vault2;
	keyvalue_t keyvalue4_vault2;
	keyvalue_t keyvalue5_vault2;
	keyvalue_t keyvalue6_vault2;
	keyvalue_t keyvalue7_vault2;
	
	keyvalue_t keyvalue0_vault3;
	keyvalue_t keyvalue1_vault3;
	keyvalue_t keyvalue2_vault3;
	keyvalue_t keyvalue3_vault3;
	keyvalue_t keyvalue4_vault3;
	keyvalue_t keyvalue5_vault3;
	keyvalue_t keyvalue6_vault3;
	keyvalue_t keyvalue7_vault3;
	
	keyvalue_t keyvalue0_vault4;
	keyvalue_t keyvalue1_vault4;
	keyvalue_t keyvalue2_vault4;
	keyvalue_t keyvalue3_vault4;
	keyvalue_t keyvalue4_vault4;
	keyvalue_t keyvalue5_vault4;
	keyvalue_t keyvalue6_vault4;
	keyvalue_t keyvalue7_vault4;
	
	
	if(enable1 == ON && enable2 == ON){
	SPREADANDSAVE_COMBINEDWITH_PREPAREVMASKS_LOOP: for (buffer_type i=0; i<reducebuffersz; i++){ // 8, 16, BLOCKRAM_SIZE
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE II=1
		// spread operation
		keyvalue0_vault0 = SYNC_GETKV2(source[0][i]); 
		keyvalue1_vault0 = SYNC_GETKV2(source[1][i]); 
		keyvalue2_vault0 = SYNC_GETKV2(source[2][i]); 
		keyvalue3_vault0 = SYNC_GETKV2(source[3][i]); 
		keyvalue4_vault0 = SYNC_GETKV2(source[4][i]); 
		keyvalue5_vault0 = SYNC_GETKV2(source[5][i]); 
		keyvalue6_vault0 = SYNC_GETKV2(source[6][i]); 
		keyvalue7_vault0 = SYNC_GETKV2(source[7][i]); 
		
		#ifdef _WIDEWORD
		kvdram0[vbuffer_offset_kvs + i].range(31, 0) = keyvalue0_vault0.key; 
		kvdram0[vbuffer_offset_kvs + i].range(63, 32) = keyvalue0_vault0.value; 
		keyvalue0_vault1 = keyvalue0_vault0;
		kvdram0[vbuffer_offset_kvs + i].range(95, 64) = keyvalue1_vault0.key; 
		kvdram0[vbuffer_offset_kvs + i].range(127, 96) = keyvalue1_vault0.value; 
		keyvalue1_vault1 = keyvalue1_vault0;
		kvdram0[vbuffer_offset_kvs + i].range(159, 128) = keyvalue2_vault0.key; 
		kvdram0[vbuffer_offset_kvs + i].range(191, 160) = keyvalue2_vault0.value; 
		keyvalue2_vault1 = keyvalue2_vault0;
		kvdram0[vbuffer_offset_kvs + i].range(223, 192) = keyvalue3_vault0.key; 
		kvdram0[vbuffer_offset_kvs + i].range(255, 224) = keyvalue3_vault0.value; 
		keyvalue3_vault1 = keyvalue3_vault0;
		kvdram0[vbuffer_offset_kvs + i].range(287, 256) = keyvalue4_vault0.key; 
		kvdram0[vbuffer_offset_kvs + i].range(319, 288) = keyvalue4_vault0.value; 
		keyvalue4_vault1 = keyvalue4_vault0;
		kvdram0[vbuffer_offset_kvs + i].range(351, 320) = keyvalue5_vault0.key; 
		kvdram0[vbuffer_offset_kvs + i].range(383, 352) = keyvalue5_vault0.value; 
		keyvalue5_vault1 = keyvalue5_vault0;
		kvdram0[vbuffer_offset_kvs + i].range(415, 384) = keyvalue6_vault0.key; 
		kvdram0[vbuffer_offset_kvs + i].range(447, 416) = keyvalue6_vault0.value; 
		keyvalue6_vault1 = keyvalue6_vault0;
		kvdram0[vbuffer_offset_kvs + i].range(479, 448) = keyvalue7_vault0.key; 
		kvdram0[vbuffer_offset_kvs + i].range(511, 480) = keyvalue7_vault0.value; 
		keyvalue7_vault1 = keyvalue7_vault0;
		kvdram1[vbuffer_offset_kvs + i].range(31, 0) = keyvalue0_vault1.key; 
		kvdram1[vbuffer_offset_kvs + i].range(63, 32) = keyvalue0_vault1.value; 
		keyvalue0_vault2 = keyvalue0_vault1;
		kvdram1[vbuffer_offset_kvs + i].range(95, 64) = keyvalue1_vault1.key; 
		kvdram1[vbuffer_offset_kvs + i].range(127, 96) = keyvalue1_vault1.value; 
		keyvalue1_vault2 = keyvalue1_vault1;
		kvdram1[vbuffer_offset_kvs + i].range(159, 128) = keyvalue2_vault1.key; 
		kvdram1[vbuffer_offset_kvs + i].range(191, 160) = keyvalue2_vault1.value; 
		keyvalue2_vault2 = keyvalue2_vault1;
		kvdram1[vbuffer_offset_kvs + i].range(223, 192) = keyvalue3_vault1.key; 
		kvdram1[vbuffer_offset_kvs + i].range(255, 224) = keyvalue3_vault1.value; 
		keyvalue3_vault2 = keyvalue3_vault1;
		kvdram1[vbuffer_offset_kvs + i].range(287, 256) = keyvalue4_vault1.key; 
		kvdram1[vbuffer_offset_kvs + i].range(319, 288) = keyvalue4_vault1.value; 
		keyvalue4_vault2 = keyvalue4_vault1;
		kvdram1[vbuffer_offset_kvs + i].range(351, 320) = keyvalue5_vault1.key; 
		kvdram1[vbuffer_offset_kvs + i].range(383, 352) = keyvalue5_vault1.value; 
		keyvalue5_vault2 = keyvalue5_vault1;
		kvdram1[vbuffer_offset_kvs + i].range(415, 384) = keyvalue6_vault1.key; 
		kvdram1[vbuffer_offset_kvs + i].range(447, 416) = keyvalue6_vault1.value; 
		keyvalue6_vault2 = keyvalue6_vault1;
		kvdram1[vbuffer_offset_kvs + i].range(479, 448) = keyvalue7_vault1.key; 
		kvdram1[vbuffer_offset_kvs + i].range(511, 480) = keyvalue7_vault1.value; 
		keyvalue7_vault2 = keyvalue7_vault1;
		kvdram2[vbuffer_offset_kvs + i].range(31, 0) = keyvalue0_vault2.key; 
		kvdram2[vbuffer_offset_kvs + i].range(63, 32) = keyvalue0_vault2.value; 
		keyvalue0_vault3 = keyvalue0_vault2;
		kvdram2[vbuffer_offset_kvs + i].range(95, 64) = keyvalue1_vault2.key; 
		kvdram2[vbuffer_offset_kvs + i].range(127, 96) = keyvalue1_vault2.value; 
		keyvalue1_vault3 = keyvalue1_vault2;
		kvdram2[vbuffer_offset_kvs + i].range(159, 128) = keyvalue2_vault2.key; 
		kvdram2[vbuffer_offset_kvs + i].range(191, 160) = keyvalue2_vault2.value; 
		keyvalue2_vault3 = keyvalue2_vault2;
		kvdram2[vbuffer_offset_kvs + i].range(223, 192) = keyvalue3_vault2.key; 
		kvdram2[vbuffer_offset_kvs + i].range(255, 224) = keyvalue3_vault2.value; 
		keyvalue3_vault3 = keyvalue3_vault2;
		kvdram2[vbuffer_offset_kvs + i].range(287, 256) = keyvalue4_vault2.key; 
		kvdram2[vbuffer_offset_kvs + i].range(319, 288) = keyvalue4_vault2.value; 
		keyvalue4_vault3 = keyvalue4_vault2;
		kvdram2[vbuffer_offset_kvs + i].range(351, 320) = keyvalue5_vault2.key; 
		kvdram2[vbuffer_offset_kvs + i].range(383, 352) = keyvalue5_vault2.value; 
		keyvalue5_vault3 = keyvalue5_vault2;
		kvdram2[vbuffer_offset_kvs + i].range(415, 384) = keyvalue6_vault2.key; 
		kvdram2[vbuffer_offset_kvs + i].range(447, 416) = keyvalue6_vault2.value; 
		keyvalue6_vault3 = keyvalue6_vault2;
		kvdram2[vbuffer_offset_kvs + i].range(479, 448) = keyvalue7_vault2.key; 
		kvdram2[vbuffer_offset_kvs + i].range(511, 480) = keyvalue7_vault2.value; 
		keyvalue7_vault3 = keyvalue7_vault2;
		kvdram3[vbuffer_offset_kvs + i].range(31, 0) = keyvalue0_vault3.key; 
		kvdram3[vbuffer_offset_kvs + i].range(63, 32) = keyvalue0_vault3.value; 
		keyvalue0_vault4 = keyvalue0_vault3;
		kvdram3[vbuffer_offset_kvs + i].range(95, 64) = keyvalue1_vault3.key; 
		kvdram3[vbuffer_offset_kvs + i].range(127, 96) = keyvalue1_vault3.value; 
		keyvalue1_vault4 = keyvalue1_vault3;
		kvdram3[vbuffer_offset_kvs + i].range(159, 128) = keyvalue2_vault3.key; 
		kvdram3[vbuffer_offset_kvs + i].range(191, 160) = keyvalue2_vault3.value; 
		keyvalue2_vault4 = keyvalue2_vault3;
		kvdram3[vbuffer_offset_kvs + i].range(223, 192) = keyvalue3_vault3.key; 
		kvdram3[vbuffer_offset_kvs + i].range(255, 224) = keyvalue3_vault3.value; 
		keyvalue3_vault4 = keyvalue3_vault3;
		kvdram3[vbuffer_offset_kvs + i].range(287, 256) = keyvalue4_vault3.key; 
		kvdram3[vbuffer_offset_kvs + i].range(319, 288) = keyvalue4_vault3.value; 
		keyvalue4_vault4 = keyvalue4_vault3;
		kvdram3[vbuffer_offset_kvs + i].range(351, 320) = keyvalue5_vault3.key; 
		kvdram3[vbuffer_offset_kvs + i].range(383, 352) = keyvalue5_vault3.value; 
		keyvalue5_vault4 = keyvalue5_vault3;
		kvdram3[vbuffer_offset_kvs + i].range(415, 384) = keyvalue6_vault3.key; 
		kvdram3[vbuffer_offset_kvs + i].range(447, 416) = keyvalue6_vault3.value; 
		keyvalue6_vault4 = keyvalue6_vault3;
		kvdram3[vbuffer_offset_kvs + i].range(479, 448) = keyvalue7_vault3.key; 
		kvdram3[vbuffer_offset_kvs + i].range(511, 480) = keyvalue7_vault3.value; 
		keyvalue7_vault4 = keyvalue7_vault3;
		#else 
		kvdram0[vbuffer_offset_kvs + i].data[0].key = keyvalue0_vault0.key;
		kvdram0[vbuffer_offset_kvs + i].data[0].value = keyvalue0_vault0.value;
		keyvalue0_vault1 = keyvalue0_vault0;
		kvdram0[vbuffer_offset_kvs + i].data[1].key = keyvalue1_vault0.key;
		kvdram0[vbuffer_offset_kvs + i].data[1].value = keyvalue1_vault0.value;
		keyvalue1_vault1 = keyvalue1_vault0;
		kvdram0[vbuffer_offset_kvs + i].data[2].key = keyvalue2_vault0.key;
		kvdram0[vbuffer_offset_kvs + i].data[2].value = keyvalue2_vault0.value;
		keyvalue2_vault1 = keyvalue2_vault0;
		kvdram0[vbuffer_offset_kvs + i].data[3].key = keyvalue3_vault0.key;
		kvdram0[vbuffer_offset_kvs + i].data[3].value = keyvalue3_vault0.value;
		keyvalue3_vault1 = keyvalue3_vault0;
		kvdram0[vbuffer_offset_kvs + i].data[4].key = keyvalue4_vault0.key;
		kvdram0[vbuffer_offset_kvs + i].data[4].value = keyvalue4_vault0.value;
		keyvalue4_vault1 = keyvalue4_vault0;
		kvdram0[vbuffer_offset_kvs + i].data[5].key = keyvalue5_vault0.key;
		kvdram0[vbuffer_offset_kvs + i].data[5].value = keyvalue5_vault0.value;
		keyvalue5_vault1 = keyvalue5_vault0;
		kvdram0[vbuffer_offset_kvs + i].data[6].key = keyvalue6_vault0.key;
		kvdram0[vbuffer_offset_kvs + i].data[6].value = keyvalue6_vault0.value;
		keyvalue6_vault1 = keyvalue6_vault0;
		kvdram0[vbuffer_offset_kvs + i].data[7].key = keyvalue7_vault0.key;
		kvdram0[vbuffer_offset_kvs + i].data[7].value = keyvalue7_vault0.value;
		keyvalue7_vault1 = keyvalue7_vault0;
		kvdram1[vbuffer_offset_kvs + i].data[0].key = keyvalue0_vault1.key;
		kvdram1[vbuffer_offset_kvs + i].data[0].value = keyvalue0_vault1.value;
		keyvalue0_vault2 = keyvalue0_vault1;
		kvdram1[vbuffer_offset_kvs + i].data[1].key = keyvalue1_vault1.key;
		kvdram1[vbuffer_offset_kvs + i].data[1].value = keyvalue1_vault1.value;
		keyvalue1_vault2 = keyvalue1_vault1;
		kvdram1[vbuffer_offset_kvs + i].data[2].key = keyvalue2_vault1.key;
		kvdram1[vbuffer_offset_kvs + i].data[2].value = keyvalue2_vault1.value;
		keyvalue2_vault2 = keyvalue2_vault1;
		kvdram1[vbuffer_offset_kvs + i].data[3].key = keyvalue3_vault1.key;
		kvdram1[vbuffer_offset_kvs + i].data[3].value = keyvalue3_vault1.value;
		keyvalue3_vault2 = keyvalue3_vault1;
		kvdram1[vbuffer_offset_kvs + i].data[4].key = keyvalue4_vault1.key;
		kvdram1[vbuffer_offset_kvs + i].data[4].value = keyvalue4_vault1.value;
		keyvalue4_vault2 = keyvalue4_vault1;
		kvdram1[vbuffer_offset_kvs + i].data[5].key = keyvalue5_vault1.key;
		kvdram1[vbuffer_offset_kvs + i].data[5].value = keyvalue5_vault1.value;
		keyvalue5_vault2 = keyvalue5_vault1;
		kvdram1[vbuffer_offset_kvs + i].data[6].key = keyvalue6_vault1.key;
		kvdram1[vbuffer_offset_kvs + i].data[6].value = keyvalue6_vault1.value;
		keyvalue6_vault2 = keyvalue6_vault1;
		kvdram1[vbuffer_offset_kvs + i].data[7].key = keyvalue7_vault1.key;
		kvdram1[vbuffer_offset_kvs + i].data[7].value = keyvalue7_vault1.value;
		keyvalue7_vault2 = keyvalue7_vault1;
		kvdram2[vbuffer_offset_kvs + i].data[0].key = keyvalue0_vault2.key;
		kvdram2[vbuffer_offset_kvs + i].data[0].value = keyvalue0_vault2.value;
		keyvalue0_vault3 = keyvalue0_vault2;
		kvdram2[vbuffer_offset_kvs + i].data[1].key = keyvalue1_vault2.key;
		kvdram2[vbuffer_offset_kvs + i].data[1].value = keyvalue1_vault2.value;
		keyvalue1_vault3 = keyvalue1_vault2;
		kvdram2[vbuffer_offset_kvs + i].data[2].key = keyvalue2_vault2.key;
		kvdram2[vbuffer_offset_kvs + i].data[2].value = keyvalue2_vault2.value;
		keyvalue2_vault3 = keyvalue2_vault2;
		kvdram2[vbuffer_offset_kvs + i].data[3].key = keyvalue3_vault2.key;
		kvdram2[vbuffer_offset_kvs + i].data[3].value = keyvalue3_vault2.value;
		keyvalue3_vault3 = keyvalue3_vault2;
		kvdram2[vbuffer_offset_kvs + i].data[4].key = keyvalue4_vault2.key;
		kvdram2[vbuffer_offset_kvs + i].data[4].value = keyvalue4_vault2.value;
		keyvalue4_vault3 = keyvalue4_vault2;
		kvdram2[vbuffer_offset_kvs + i].data[5].key = keyvalue5_vault2.key;
		kvdram2[vbuffer_offset_kvs + i].data[5].value = keyvalue5_vault2.value;
		keyvalue5_vault3 = keyvalue5_vault2;
		kvdram2[vbuffer_offset_kvs + i].data[6].key = keyvalue6_vault2.key;
		kvdram2[vbuffer_offset_kvs + i].data[6].value = keyvalue6_vault2.value;
		keyvalue6_vault3 = keyvalue6_vault2;
		kvdram2[vbuffer_offset_kvs + i].data[7].key = keyvalue7_vault2.key;
		kvdram2[vbuffer_offset_kvs + i].data[7].value = keyvalue7_vault2.value;
		keyvalue7_vault3 = keyvalue7_vault2;
		kvdram3[vbuffer_offset_kvs + i].data[0].key = keyvalue0_vault3.key;
		kvdram3[vbuffer_offset_kvs + i].data[0].value = keyvalue0_vault3.value;
		keyvalue0_vault4 = keyvalue0_vault3;
		kvdram3[vbuffer_offset_kvs + i].data[1].key = keyvalue1_vault3.key;
		kvdram3[vbuffer_offset_kvs + i].data[1].value = keyvalue1_vault3.value;
		keyvalue1_vault4 = keyvalue1_vault3;
		kvdram3[vbuffer_offset_kvs + i].data[2].key = keyvalue2_vault3.key;
		kvdram3[vbuffer_offset_kvs + i].data[2].value = keyvalue2_vault3.value;
		keyvalue2_vault4 = keyvalue2_vault3;
		kvdram3[vbuffer_offset_kvs + i].data[3].key = keyvalue3_vault3.key;
		kvdram3[vbuffer_offset_kvs + i].data[3].value = keyvalue3_vault3.value;
		keyvalue3_vault4 = keyvalue3_vault3;
		kvdram3[vbuffer_offset_kvs + i].data[4].key = keyvalue4_vault3.key;
		kvdram3[vbuffer_offset_kvs + i].data[4].value = keyvalue4_vault3.value;
		keyvalue4_vault4 = keyvalue4_vault3;
		kvdram3[vbuffer_offset_kvs + i].data[5].key = keyvalue5_vault3.key;
		kvdram3[vbuffer_offset_kvs + i].data[5].value = keyvalue5_vault3.value;
		keyvalue5_vault4 = keyvalue5_vault3;
		kvdram3[vbuffer_offset_kvs + i].data[6].key = keyvalue6_vault3.key;
		kvdram3[vbuffer_offset_kvs + i].data[6].value = keyvalue6_vault3.value;
		keyvalue6_vault4 = keyvalue6_vault3;
		kvdram3[vbuffer_offset_kvs + i].data[7].key = keyvalue7_vault3.key;
		kvdram3[vbuffer_offset_kvs + i].data[7].value = keyvalue7_vault3.value;
		keyvalue7_vault4 = keyvalue7_vault3;
		#endif 
		
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
		if(SYNC_GETKEYENTRY(kvdram3[vbuffer_offset_kvs + i], 0) < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(0*REDUCESZ + 2*i)<<endl; } 
		if(GETVALUEENTRY(kvdram3[vbuffer_offset_kvs + i], 0) < SYNC_GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(0*REDUCESZ + 2*i + 1)<<endl; } 
		if(SYNC_GETKEYENTRY(kvdram3[vbuffer_offset_kvs + i], 1) < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(1*REDUCESZ + 2*i)<<endl; } 
		if(GETVALUEENTRY(kvdram3[vbuffer_offset_kvs + i], 1) < SYNC_GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(1*REDUCESZ + 2*i + 1)<<endl; } 
		if(SYNC_GETKEYENTRY(kvdram3[vbuffer_offset_kvs + i], 2) < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(2*REDUCESZ + 2*i)<<endl; } 
		if(GETVALUEENTRY(kvdram3[vbuffer_offset_kvs + i], 2) < SYNC_GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(2*REDUCESZ + 2*i + 1)<<endl; } 
		if(SYNC_GETKEYENTRY(kvdram3[vbuffer_offset_kvs + i], 3) < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(3*REDUCESZ + 2*i)<<endl; } 
		if(GETVALUEENTRY(kvdram3[vbuffer_offset_kvs + i], 3) < SYNC_GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(3*REDUCESZ + 2*i + 1)<<endl; } 
		if(SYNC_GETKEYENTRY(kvdram3[vbuffer_offset_kvs + i], 4) < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(4*REDUCESZ + 2*i)<<endl; } 
		if(GETVALUEENTRY(kvdram3[vbuffer_offset_kvs + i], 4) < SYNC_GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(4*REDUCESZ + 2*i + 1)<<endl; } 
		if(SYNC_GETKEYENTRY(kvdram3[vbuffer_offset_kvs + i], 5) < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(5*REDUCESZ + 2*i)<<endl; } 
		if(GETVALUEENTRY(kvdram3[vbuffer_offset_kvs + i], 5) < SYNC_GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(5*REDUCESZ + 2*i + 1)<<endl; } 
		if(SYNC_GETKEYENTRY(kvdram3[vbuffer_offset_kvs + i], 6) < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(6*REDUCESZ + 2*i)<<endl; } 
		if(GETVALUEENTRY(kvdram3[vbuffer_offset_kvs + i], 6) < SYNC_GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(6*REDUCESZ + 2*i + 1)<<endl; } 
		if(SYNC_GETKEYENTRY(kvdram3[vbuffer_offset_kvs + i], 7) < SYNC_GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(7*REDUCESZ + 2*i)<<endl; } 
		if(GETVALUEENTRY(kvdram3[vbuffer_offset_kvs + i], 7) < SYNC_GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(7*REDUCESZ + 2*i + 1)<<endl; } 
		#endif
		
		// combined with save to vdram operation
		#ifdef _WIDEWORD
		vdram[vbuffer_offset_kvs + i].range(31, 0) = keyvalue0_vault0.key; 
		vdram[vbuffer_offset_kvs + i].range(63, 32) = keyvalue0_vault0.value; 
		vdram[vbuffer_offset_kvs + i].range(95, 64) = keyvalue1_vault0.key; 
		vdram[vbuffer_offset_kvs + i].range(127, 96) = keyvalue1_vault0.value; 
		vdram[vbuffer_offset_kvs + i].range(159, 128) = keyvalue2_vault0.key; 
		vdram[vbuffer_offset_kvs + i].range(191, 160) = keyvalue2_vault0.value; 
		vdram[vbuffer_offset_kvs + i].range(223, 192) = keyvalue3_vault0.key; 
		vdram[vbuffer_offset_kvs + i].range(255, 224) = keyvalue3_vault0.value; 
		vdram[vbuffer_offset_kvs + i].range(287, 256) = keyvalue4_vault0.key; 
		vdram[vbuffer_offset_kvs + i].range(319, 288) = keyvalue4_vault0.value; 
		vdram[vbuffer_offset_kvs + i].range(351, 320) = keyvalue5_vault0.key; 
		vdram[vbuffer_offset_kvs + i].range(383, 352) = keyvalue5_vault0.value; 
		vdram[vbuffer_offset_kvs + i].range(415, 384) = keyvalue6_vault0.key; 
		vdram[vbuffer_offset_kvs + i].range(447, 416) = keyvalue6_vault0.value; 
		vdram[vbuffer_offset_kvs + i].range(479, 448) = keyvalue7_vault0.key; 
		vdram[vbuffer_offset_kvs + i].range(511, 480) = keyvalue7_vault0.value; 
		#else
		vdram[vbuffer_offset_kvs + i].data[0].key = keyvalue0_vault0.key;
		vdram[vbuffer_offset_kvs + i].data[0].value = keyvalue0_vault0.value;
		vdram[vbuffer_offset_kvs + i].data[1].key = keyvalue1_vault0.key;
		vdram[vbuffer_offset_kvs + i].data[1].value = keyvalue1_vault0.value;
		vdram[vbuffer_offset_kvs + i].data[2].key = keyvalue2_vault0.key;
		vdram[vbuffer_offset_kvs + i].data[2].value = keyvalue2_vault0.value;
		vdram[vbuffer_offset_kvs + i].data[3].key = keyvalue3_vault0.key;
		vdram[vbuffer_offset_kvs + i].data[3].value = keyvalue3_vault0.value;
		vdram[vbuffer_offset_kvs + i].data[4].key = keyvalue4_vault0.key;
		vdram[vbuffer_offset_kvs + i].data[4].value = keyvalue4_vault0.value;
		vdram[vbuffer_offset_kvs + i].data[5].key = keyvalue5_vault0.key;
		vdram[vbuffer_offset_kvs + i].data[5].value = keyvalue5_vault0.value;
		vdram[vbuffer_offset_kvs + i].data[6].key = keyvalue6_vault0.key;
		vdram[vbuffer_offset_kvs + i].data[6].value = keyvalue6_vault0.value;
		vdram[vbuffer_offset_kvs + i].data[7].key = keyvalue7_vault0.key;
		vdram[vbuffer_offset_kvs + i].data[7].value = keyvalue7_vault0.value;
		#endif 
		
		// combined with prepare vmask operation
		unsigned int X = i % 16;
		unsigned int Y = i / 16;
	
		#ifdef _WIDEWORD
		tempbuffer[X][Y].range(0, 0) = vmask[i].data[0].key;
		tempbuffer[X][Y].range(1, 1) = vmask[i].data[0].value;
		tempbuffer[X][Y].range(2, 2) = vmask[i].data[1].key;
		tempbuffer[X][Y].range(3, 3) = vmask[i].data[1].value;
		tempbuffer[X][Y].range(4, 4) = vmask[i].data[2].key;
		tempbuffer[X][Y].range(5, 5) = vmask[i].data[2].value;
		tempbuffer[X][Y].range(6, 6) = vmask[i].data[3].key;
		tempbuffer[X][Y].range(7, 7) = vmask[i].data[3].value;
		tempbuffer[X][Y].range(8, 8) = vmask[i].data[4].key;
		tempbuffer[X][Y].range(9, 9) = vmask[i].data[4].value;
		tempbuffer[X][Y].range(10, 10) = vmask[i].data[5].key;
		tempbuffer[X][Y].range(11, 11) = vmask[i].data[5].value;
		tempbuffer[X][Y].range(12, 12) = vmask[i].data[6].key;
		tempbuffer[X][Y].range(13, 13) = vmask[i].data[6].value;
		tempbuffer[X][Y].range(14, 14) = vmask[i].data[7].key;
		tempbuffer[X][Y].range(15, 15) = vmask[i].data[7].value;
		tempbuffer[X][Y].range(16, 16) = vmask[i].data[8].key;
		tempbuffer[X][Y].range(17, 17) = vmask[i].data[8].value;
		tempbuffer[X][Y].range(18, 18) = vmask[i].data[9].key;
		tempbuffer[X][Y].range(19, 19) = vmask[i].data[9].value;
		tempbuffer[X][Y].range(20, 20) = vmask[i].data[10].key;
		tempbuffer[X][Y].range(21, 21) = vmask[i].data[10].value;
		tempbuffer[X][Y].range(22, 22) = vmask[i].data[11].key;
		tempbuffer[X][Y].range(23, 23) = vmask[i].data[11].value;
		tempbuffer[X][Y].range(24, 24) = vmask[i].data[12].key;
		tempbuffer[X][Y].range(25, 25) = vmask[i].data[12].value;
		tempbuffer[X][Y].range(26, 26) = vmask[i].data[13].key;
		tempbuffer[X][Y].range(27, 27) = vmask[i].data[13].value;
		tempbuffer[X][Y].range(28, 28) = vmask[i].data[14].key;
		tempbuffer[X][Y].range(29, 29) = vmask[i].data[14].value;
		tempbuffer[X][Y].range(30, 30) = vmask[i].data[15].key;
		tempbuffer[X][Y].range(31, 31) = vmask[i].data[15].value;
		#else
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 0, 1, vmask[i].data[0].key);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 1, 1, vmask[i].data[0].value);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 2, 1, vmask[i].data[1].key);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 3, 1, vmask[i].data[1].value);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 4, 1, vmask[i].data[2].key);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 5, 1, vmask[i].data[2].value);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 6, 1, vmask[i].data[3].key);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 7, 1, vmask[i].data[3].value);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 8, 1, vmask[i].data[4].key);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 9, 1, vmask[i].data[4].value);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 10, 1, vmask[i].data[5].key);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 11, 1, vmask[i].data[5].value);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 12, 1, vmask[i].data[6].key);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 13, 1, vmask[i].data[6].value);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 14, 1, vmask[i].data[7].key);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 15, 1, vmask[i].data[7].value);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 16, 1, vmask[i].data[8].key);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 17, 1, vmask[i].data[8].value);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 18, 1, vmask[i].data[9].key);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 19, 1, vmask[i].data[9].value);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 20, 1, vmask[i].data[10].key);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 21, 1, vmask[i].data[10].value);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 22, 1, vmask[i].data[11].key);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 23, 1, vmask[i].data[11].value);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 24, 1, vmask[i].data[12].key);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 25, 1, vmask[i].data[12].value);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 26, 1, vmask[i].data[13].key);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 27, 1, vmask[i].data[13].value);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 28, 1, vmask[i].data[14].key);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 29, 1, vmask[i].data[14].value);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 30, 1, vmask[i].data[15].key);
		SYNC_WRITETO_UINT(&tempbuffer[X][Y], 31, 1, vmask[i].data[15].value);
		#endif
	}
	
	if(enable_savevmask == ON){
	SAVEVMASKS_LOOP: for(buffer_type i=0; i<vmask_size_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount3 avg=analysis_loopcount3
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		kvdram0[vmask_offset_kvs + i].range(31, 0) = tempbuffer[0][i]; 
		kvdram0[vmask_offset_kvs + i].range(63, 32) = tempbuffer[1][i]; 
		kvdram0[vmask_offset_kvs + i].range(95, 64) = tempbuffer[2][i]; 
		kvdram0[vmask_offset_kvs + i].range(127, 96) = tempbuffer[3][i]; 
		kvdram0[vmask_offset_kvs + i].range(159, 128) = tempbuffer[4][i]; 
		kvdram0[vmask_offset_kvs + i].range(191, 160) = tempbuffer[5][i]; 
		kvdram0[vmask_offset_kvs + i].range(223, 192) = tempbuffer[6][i]; 
		kvdram0[vmask_offset_kvs + i].range(255, 224) = tempbuffer[7][i]; 
		kvdram0[vmask_offset_kvs + i].range(287, 256) = tempbuffer[8][i]; 
		kvdram0[vmask_offset_kvs + i].range(319, 288) = tempbuffer[9][i]; 
		kvdram0[vmask_offset_kvs + i].range(351, 320) = tempbuffer[10][i]; 
		kvdram0[vmask_offset_kvs + i].range(383, 352) = tempbuffer[11][i]; 
		kvdram0[vmask_offset_kvs + i].range(415, 384) = tempbuffer[12][i]; 
		kvdram0[vmask_offset_kvs + i].range(447, 416) = tempbuffer[13][i]; 
		kvdram0[vmask_offset_kvs + i].range(479, 448) = tempbuffer[14][i]; 
		kvdram0[vmask_offset_kvs + i].range(511, 480) = tempbuffer[15][i]; 
		kvdram1[vmask_offset_kvs + i].range(31, 0) = tempbuffer[0][i]; 
		kvdram1[vmask_offset_kvs + i].range(63, 32) = tempbuffer[1][i]; 
		kvdram1[vmask_offset_kvs + i].range(95, 64) = tempbuffer[2][i]; 
		kvdram1[vmask_offset_kvs + i].range(127, 96) = tempbuffer[3][i]; 
		kvdram1[vmask_offset_kvs + i].range(159, 128) = tempbuffer[4][i]; 
		kvdram1[vmask_offset_kvs + i].range(191, 160) = tempbuffer[5][i]; 
		kvdram1[vmask_offset_kvs + i].range(223, 192) = tempbuffer[6][i]; 
		kvdram1[vmask_offset_kvs + i].range(255, 224) = tempbuffer[7][i]; 
		kvdram1[vmask_offset_kvs + i].range(287, 256) = tempbuffer[8][i]; 
		kvdram1[vmask_offset_kvs + i].range(319, 288) = tempbuffer[9][i]; 
		kvdram1[vmask_offset_kvs + i].range(351, 320) = tempbuffer[10][i]; 
		kvdram1[vmask_offset_kvs + i].range(383, 352) = tempbuffer[11][i]; 
		kvdram1[vmask_offset_kvs + i].range(415, 384) = tempbuffer[12][i]; 
		kvdram1[vmask_offset_kvs + i].range(447, 416) = tempbuffer[13][i]; 
		kvdram1[vmask_offset_kvs + i].range(479, 448) = tempbuffer[14][i]; 
		kvdram1[vmask_offset_kvs + i].range(511, 480) = tempbuffer[15][i]; 
		kvdram2[vmask_offset_kvs + i].range(31, 0) = tempbuffer[0][i]; 
		kvdram2[vmask_offset_kvs + i].range(63, 32) = tempbuffer[1][i]; 
		kvdram2[vmask_offset_kvs + i].range(95, 64) = tempbuffer[2][i]; 
		kvdram2[vmask_offset_kvs + i].range(127, 96) = tempbuffer[3][i]; 
		kvdram2[vmask_offset_kvs + i].range(159, 128) = tempbuffer[4][i]; 
		kvdram2[vmask_offset_kvs + i].range(191, 160) = tempbuffer[5][i]; 
		kvdram2[vmask_offset_kvs + i].range(223, 192) = tempbuffer[6][i]; 
		kvdram2[vmask_offset_kvs + i].range(255, 224) = tempbuffer[7][i]; 
		kvdram2[vmask_offset_kvs + i].range(287, 256) = tempbuffer[8][i]; 
		kvdram2[vmask_offset_kvs + i].range(319, 288) = tempbuffer[9][i]; 
		kvdram2[vmask_offset_kvs + i].range(351, 320) = tempbuffer[10][i]; 
		kvdram2[vmask_offset_kvs + i].range(383, 352) = tempbuffer[11][i]; 
		kvdram2[vmask_offset_kvs + i].range(415, 384) = tempbuffer[12][i]; 
		kvdram2[vmask_offset_kvs + i].range(447, 416) = tempbuffer[13][i]; 
		kvdram2[vmask_offset_kvs + i].range(479, 448) = tempbuffer[14][i]; 
		kvdram2[vmask_offset_kvs + i].range(511, 480) = tempbuffer[15][i]; 
		kvdram3[vmask_offset_kvs + i].range(31, 0) = tempbuffer[0][i]; 
		kvdram3[vmask_offset_kvs + i].range(63, 32) = tempbuffer[1][i]; 
		kvdram3[vmask_offset_kvs + i].range(95, 64) = tempbuffer[2][i]; 
		kvdram3[vmask_offset_kvs + i].range(127, 96) = tempbuffer[3][i]; 
		kvdram3[vmask_offset_kvs + i].range(159, 128) = tempbuffer[4][i]; 
		kvdram3[vmask_offset_kvs + i].range(191, 160) = tempbuffer[5][i]; 
		kvdram3[vmask_offset_kvs + i].range(223, 192) = tempbuffer[6][i]; 
		kvdram3[vmask_offset_kvs + i].range(255, 224) = tempbuffer[7][i]; 
		kvdram3[vmask_offset_kvs + i].range(287, 256) = tempbuffer[8][i]; 
		kvdram3[vmask_offset_kvs + i].range(319, 288) = tempbuffer[9][i]; 
		kvdram3[vmask_offset_kvs + i].range(351, 320) = tempbuffer[10][i]; 
		kvdram3[vmask_offset_kvs + i].range(383, 352) = tempbuffer[11][i]; 
		kvdram3[vmask_offset_kvs + i].range(415, 384) = tempbuffer[12][i]; 
		kvdram3[vmask_offset_kvs + i].range(447, 416) = tempbuffer[13][i]; 
		kvdram3[vmask_offset_kvs + i].range(479, 448) = tempbuffer[14][i]; 
		kvdram3[vmask_offset_kvs + i].range(511, 480) = tempbuffer[15][i]; 
		#else 
		kvdram0[vmask_offset_kvs + i].data[0].key = tempbuffer[0][i];
		kvdram0[vmask_offset_kvs + i].data[0].value = tempbuffer[1][i]; 
		kvdram0[vmask_offset_kvs + i].data[1].key = tempbuffer[2][i];
		kvdram0[vmask_offset_kvs + i].data[1].value = tempbuffer[3][i]; 
		kvdram0[vmask_offset_kvs + i].data[2].key = tempbuffer[4][i];
		kvdram0[vmask_offset_kvs + i].data[2].value = tempbuffer[5][i]; 
		kvdram0[vmask_offset_kvs + i].data[3].key = tempbuffer[6][i];
		kvdram0[vmask_offset_kvs + i].data[3].value = tempbuffer[7][i]; 
		kvdram0[vmask_offset_kvs + i].data[4].key = tempbuffer[8][i];
		kvdram0[vmask_offset_kvs + i].data[4].value = tempbuffer[9][i]; 
		kvdram0[vmask_offset_kvs + i].data[5].key = tempbuffer[10][i];
		kvdram0[vmask_offset_kvs + i].data[5].value = tempbuffer[11][i]; 
		kvdram0[vmask_offset_kvs + i].data[6].key = tempbuffer[12][i];
		kvdram0[vmask_offset_kvs + i].data[6].value = tempbuffer[13][i]; 
		kvdram0[vmask_offset_kvs + i].data[7].key = tempbuffer[14][i];
		kvdram0[vmask_offset_kvs + i].data[7].value = tempbuffer[15][i]; 
		kvdram1[vmask_offset_kvs + i].data[0].key = tempbuffer[0][i];
		kvdram1[vmask_offset_kvs + i].data[0].value = tempbuffer[1][i]; 
		kvdram1[vmask_offset_kvs + i].data[1].key = tempbuffer[2][i];
		kvdram1[vmask_offset_kvs + i].data[1].value = tempbuffer[3][i]; 
		kvdram1[vmask_offset_kvs + i].data[2].key = tempbuffer[4][i];
		kvdram1[vmask_offset_kvs + i].data[2].value = tempbuffer[5][i]; 
		kvdram1[vmask_offset_kvs + i].data[3].key = tempbuffer[6][i];
		kvdram1[vmask_offset_kvs + i].data[3].value = tempbuffer[7][i]; 
		kvdram1[vmask_offset_kvs + i].data[4].key = tempbuffer[8][i];
		kvdram1[vmask_offset_kvs + i].data[4].value = tempbuffer[9][i]; 
		kvdram1[vmask_offset_kvs + i].data[5].key = tempbuffer[10][i];
		kvdram1[vmask_offset_kvs + i].data[5].value = tempbuffer[11][i]; 
		kvdram1[vmask_offset_kvs + i].data[6].key = tempbuffer[12][i];
		kvdram1[vmask_offset_kvs + i].data[6].value = tempbuffer[13][i]; 
		kvdram1[vmask_offset_kvs + i].data[7].key = tempbuffer[14][i];
		kvdram1[vmask_offset_kvs + i].data[7].value = tempbuffer[15][i]; 
		kvdram2[vmask_offset_kvs + i].data[0].key = tempbuffer[0][i];
		kvdram2[vmask_offset_kvs + i].data[0].value = tempbuffer[1][i]; 
		kvdram2[vmask_offset_kvs + i].data[1].key = tempbuffer[2][i];
		kvdram2[vmask_offset_kvs + i].data[1].value = tempbuffer[3][i]; 
		kvdram2[vmask_offset_kvs + i].data[2].key = tempbuffer[4][i];
		kvdram2[vmask_offset_kvs + i].data[2].value = tempbuffer[5][i]; 
		kvdram2[vmask_offset_kvs + i].data[3].key = tempbuffer[6][i];
		kvdram2[vmask_offset_kvs + i].data[3].value = tempbuffer[7][i]; 
		kvdram2[vmask_offset_kvs + i].data[4].key = tempbuffer[8][i];
		kvdram2[vmask_offset_kvs + i].data[4].value = tempbuffer[9][i]; 
		kvdram2[vmask_offset_kvs + i].data[5].key = tempbuffer[10][i];
		kvdram2[vmask_offset_kvs + i].data[5].value = tempbuffer[11][i]; 
		kvdram2[vmask_offset_kvs + i].data[6].key = tempbuffer[12][i];
		kvdram2[vmask_offset_kvs + i].data[6].value = tempbuffer[13][i]; 
		kvdram2[vmask_offset_kvs + i].data[7].key = tempbuffer[14][i];
		kvdram2[vmask_offset_kvs + i].data[7].value = tempbuffer[15][i]; 
		kvdram3[vmask_offset_kvs + i].data[0].key = tempbuffer[0][i];
		kvdram3[vmask_offset_kvs + i].data[0].value = tempbuffer[1][i]; 
		kvdram3[vmask_offset_kvs + i].data[1].key = tempbuffer[2][i];
		kvdram3[vmask_offset_kvs + i].data[1].value = tempbuffer[3][i]; 
		kvdram3[vmask_offset_kvs + i].data[2].key = tempbuffer[4][i];
		kvdram3[vmask_offset_kvs + i].data[2].value = tempbuffer[5][i]; 
		kvdram3[vmask_offset_kvs + i].data[3].key = tempbuffer[6][i];
		kvdram3[vmask_offset_kvs + i].data[3].value = tempbuffer[7][i]; 
		kvdram3[vmask_offset_kvs + i].data[4].key = tempbuffer[8][i];
		kvdram3[vmask_offset_kvs + i].data[4].value = tempbuffer[9][i]; 
		kvdram3[vmask_offset_kvs + i].data[5].key = tempbuffer[10][i];
		kvdram3[vmask_offset_kvs + i].data[5].value = tempbuffer[11][i]; 
		kvdram3[vmask_offset_kvs + i].data[6].key = tempbuffer[12][i];
		kvdram3[vmask_offset_kvs + i].data[6].value = tempbuffer[13][i]; 
		kvdram3[vmask_offset_kvs + i].data[7].key = tempbuffer[14][i];
		kvdram3[vmask_offset_kvs + i].data[7].value = tempbuffer[15][i]; 
		#endif 
		
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvswritten(VECTOR_SIZE);
		#endif
	}
	}
	}
	
	// savevmaskp
	if(enable_savevmaskp == ON){
	uint32_type vmask_p = 0;
	if(enable1 == ON && enable2 == ON){ vmask_p = vmask_p_temp[0] | vmask_p_temp[1]; }
	#ifdef _WIDEWORD
	kvdram0[vmaskp_offset_kvs].range(31, 0) = vmask_p;
	kvdram1[vmaskp_offset_kvs].range(31, 0) = vmask_p;
	kvdram2[vmaskp_offset_kvs].range(31, 0) = vmask_p;
	kvdram3[vmaskp_offset_kvs].range(31, 0) = vmask_p;
	#else
	kvdram0[vmaskp_offset_kvs].data[0].key = vmask_p;
	kvdram1[vmaskp_offset_kvs].data[0].key = vmask_p;
	kvdram2[vmaskp_offset_kvs].data[0].key = vmask_p;
	kvdram3[vmaskp_offset_kvs].data[0].key = vmask_p;
	#endif 
	}
	return;
}

// extern "C" {
void
	#if defined(SW) || not defined(HWIMPLFOR_ACTSSYNC)
	actssync::
	#endif
topkernelsync(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11,uint512_dt * kvdram12,uint512_dt * kvdram13,uint512_dt * kvdram14,uint512_dt * kvdram15,uint512_dt * kvdram16,uint512_dt * kvdram17,uint512_dt * kvdram18,uint512_dt * kvdram19,uint512_dt * kvdram20,uint512_dt * kvdram21,uint512_dt * kvdram22,uint512_dt * kvdram23,uint512_dt * kvdram24,uint512_dt * kvdram25,uint512_dt * kvdram26,uint512_dt * kvdram27,uint512_dt * kvdram28,uint512_dt * kvdram29,uint512_dt * kvdram30,uint512_dt * kvdram31, uint512_dt * vdram){
/*  
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
 
#pragma HLS INTERFACE m_axi port = kvdram16 offset = slave bundle = gmem16
 
#pragma HLS INTERFACE m_axi port = kvdram17 offset = slave bundle = gmem17
 
#pragma HLS INTERFACE m_axi port = kvdram18 offset = slave bundle = gmem18
 
#pragma HLS INTERFACE m_axi port = kvdram19 offset = slave bundle = gmem19
 
#pragma HLS INTERFACE m_axi port = kvdram20 offset = slave bundle = gmem20
 
#pragma HLS INTERFACE m_axi port = kvdram21 offset = slave bundle = gmem21
 
#pragma HLS INTERFACE m_axi port = kvdram22 offset = slave bundle = gmem22
 
#pragma HLS INTERFACE m_axi port = kvdram23 offset = slave bundle = gmem23
 
#pragma HLS INTERFACE m_axi port = kvdram24 offset = slave bundle = gmem24
 
#pragma HLS INTERFACE m_axi port = kvdram25 offset = slave bundle = gmem25
 
#pragma HLS INTERFACE m_axi port = kvdram26 offset = slave bundle = gmem26
 
#pragma HLS INTERFACE m_axi port = kvdram27 offset = slave bundle = gmem27
 
#pragma HLS INTERFACE m_axi port = kvdram28 offset = slave bundle = gmem28
 
#pragma HLS INTERFACE m_axi port = kvdram29 offset = slave bundle = gmem29
 
#pragma HLS INTERFACE m_axi port = kvdram30 offset = slave bundle = gmem30
 
#pragma HLS INTERFACE m_axi port = kvdram31 offset = slave bundle = gmem31
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem32

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
#pragma HLS INTERFACE s_axilite port = kvdram16 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram17 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram18 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram19 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram20 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram21 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram22 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram23 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram24 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram25 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram26 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram27 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram28 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram29 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram30 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram31 bundle = control
#pragma HLS INTERFACE s_axilite port = vdram bundle = control

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
#pragma HLS DATA_PACK variable = kvdram16
#pragma HLS DATA_PACK variable = kvdram17
#pragma HLS DATA_PACK variable = kvdram18
#pragma HLS DATA_PACK variable = kvdram19
#pragma HLS DATA_PACK variable = kvdram20
#pragma HLS DATA_PACK variable = kvdram21
#pragma HLS DATA_PACK variable = kvdram22
#pragma HLS DATA_PACK variable = kvdram23
#pragma HLS DATA_PACK variable = kvdram24
#pragma HLS DATA_PACK variable = kvdram25
#pragma HLS DATA_PACK variable = kvdram26
#pragma HLS DATA_PACK variable = kvdram27
#pragma HLS DATA_PACK variable = kvdram28
#pragma HLS DATA_PACK variable = kvdram29
#pragma HLS DATA_PACK variable = kvdram30
#pragma HLS DATA_PACK variable = kvdram31
#pragma HLS DATA_PACK variable = vdram */
	
	#if defined(_DEBUGMODE_KERNELPRINTS) || defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (SYNC: NACTS_IN_NSYNCTHREADS) Launched... size: "<<SYNC_GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN], 0)<<endl; 
	#endif
	
	keyvalue_vbuffer_t refbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = refbuffer
	
	travstate_t rtravstate[NUMSYNCTHREADS];
	#pragma HLS ARRAY_PARTITION variable=rtravstate complete
	
	//
	keyvalue_vbuffer_t vbuffer0_level1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer0_level1
	keyvalue_vbuffer_t vbuffer1_level1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer1_level1
	keyvalue_vbuffer_t vbuffer2_level1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer2_level1
	keyvalue_vbuffer_t vbuffer3_level1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer3_level1
	keyvalue_vbuffer_t vbuffer4_level1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer4_level1
	keyvalue_vbuffer_t vbuffer5_level1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer5_level1
	keyvalue_vbuffer_t vbuffer6_level1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer6_level1
	keyvalue_vbuffer_t vbuffer7_level1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer7_level1
	
	keyvalue_vbuffer_t vbuffer0_level2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer0_level2
	
	keyvalue_vbuffer_t vbuffer0_level3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer0_level3
	keyvalue_vbuffer_t vbuffer1_level3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer1_level3
	keyvalue_vbuffer_t vbuffer2_level3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer2_level3
	keyvalue_vbuffer_t vbuffer3_level3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer3_level3
	keyvalue_vbuffer_t vbuffer4_level3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer4_level3
	keyvalue_vbuffer_t vbuffer5_level3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer5_level3
	keyvalue_vbuffer_t vbuffer6_level3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer6_level3
	keyvalue_vbuffer_t vbuffer7_level3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer7_level3

	//
	unitBRAMwidth_type vmask0_level2[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask0_level2
	
	unitBRAMwidth_type vmask0_level3[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask0_level3
	unitBRAMwidth_type vmask1_level3[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask1_level3
	unitBRAMwidth_type vmask2_level3[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask2_level3
	unitBRAMwidth_type vmask3_level3[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask3_level3
	unitBRAMwidth_type vmask4_level3[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask4_level3
	unitBRAMwidth_type vmask5_level3[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask5_level3
	unitBRAMwidth_type vmask6_level3[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask6_level3
	unitBRAMwidth_type vmask7_level3[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask7_level3
	
	//
	uint32_type vmaskptemp0_level2[2];
	
	uint32_type vmaskptemp0_level3[2];
	uint32_type vmaskptemp1_level3[2];
	uint32_type vmaskptemp2_level3[2];
	uint32_type vmaskptemp3_level3[2];
	uint32_type vmaskptemp4_level3[2];
	uint32_type vmaskptemp5_level3[2];
	uint32_type vmaskptemp6_level3[2];
	uint32_type vmaskptemp7_level3[2];
	
	//
	globalparams_t globalparams[NUMSYNCTHREADS];
	#pragma HLS ARRAY_PARTITION variable=globalparams complete
	uint32_type vmask_p[BLOCKRAM_SIZE];
	
	analysis_type analysis_loop1 = 1;
	analysis_type analysis_treedepth = TREE_DEPTH;
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	vmaskptemp0_level2[0] = 0; vmaskptemp0_level2[1] = 0;
	vmaskptemp0_level3[0] = 0; vmaskptemp0_level3[1] = 0;
	vmaskptemp1_level3[0] = 0; vmaskptemp1_level3[1] = 0;
	vmaskptemp2_level3[0] = 0; vmaskptemp2_level3[1] = 0;
	vmaskptemp3_level3[0] = 0; vmaskptemp3_level3[1] = 0;
	vmaskptemp4_level3[0] = 0; vmaskptemp4_level3[1] = 0;
	vmaskptemp5_level3[0] = 0; vmaskptemp5_level3[1] = 0;
	vmaskptemp6_level3[0] = 0; vmaskptemp6_level3[1] = 0;
	vmaskptemp7_level3[0] = 0; vmaskptemp7_level3[1] = 0;

	globalparams[0] = SYNC_getglobalparams(kvdram0);
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
	globalparams[16] = globalparams[0];
	globalparams[17] = globalparams[0];
	globalparams[18] = globalparams[0];
	globalparams[19] = globalparams[0];
	globalparams[20] = globalparams[0];
	globalparams[21] = globalparams[0];
	globalparams[22] = globalparams[0];
	globalparams[23] = globalparams[0];
	globalparams[24] = globalparams[0];
	globalparams[25] = globalparams[0];
	globalparams[26] = globalparams[0];
	globalparams[27] = globalparams[0];
	globalparams[28] = globalparams[0];
	globalparams[29] = globalparams[0];
	globalparams[30] = globalparams[0];
	globalparams[31] = globalparams[0];
	globalparams_t _globalparams = globalparams[0];
	
	unsigned int sourcestatsmarker = 0;
	#ifdef ENABLERECURSIVEPARTITIONING
	for(unsigned int k=0; k<_globalparams.ACTSPARAMS_TREEDEPTH-1; k++)
	#else 
	for(unsigned int k=0; k<_globalparams.ACTSPARAMS_TREEDEPTH; k++)
	#endif 
	{
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_treedepth avg=analysis_treedepth
		sourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); 
	}
	
	buffer_type reducebuffersz = _globalparams.SIZE_REDUCE / 2; // 512
	buffer_type vmaskbuffersz_kvs = (_globalparams.SIZE_REDUCE * VDATA_PACKINGSIZE) / 512; // 32
	
	unsigned int vreadoffset_kvs = 0;
	unsigned int vreadoffsetpp0_kvs = 0;
	unsigned int vreadoffsetpp1_kvs = vreadoffsetpp0_kvs + reducebuffersz;
	unsigned int vmaskreadoffset_kvs = 0;
	
	#ifdef ENABLERECURSIVEPARTITIONING
	step_type currentLOP = _globalparams.ACTSPARAMS_TREEDEPTH;
	batch_type num_source_partitions = SYNC_get_num_source_partitions(currentLOP);
	#else 
	step_type currentLOP = _globalparams.ACTSPARAMS_TREEDEPTH + 1; // NEWCHANGE.	
	batch_type num_source_partitions = NUM_PARTITIONS;
	#endif
	
	for(unsigned int k=0; k<num_source_partitions; k++){ vmask_p[k] = 0; }
	
	unsigned int total_num_iterations = num_source_partitions * 2;
	batch_type source_partition;
	
	bool_type enablereduce = ON;
	bool_type enablereducepp0 = OFF;
	bool_type enablereducepp1 = OFF;
	
	bool_type pp1en_readandsynchronize = ON;
	bool_type pp1en_syncandapply = ON;
	bool_type pp1en_spreadvdata = ON;
	bool_type pp1en_spreadvmask = ON;
	bool_type pp1en_spreadandwrite = ON;

	unsigned int SWITCHcount = 0;
	unsigned int begincol_vmask = 0;
	unsigned int MOVEcount = 0;
	
	TOPKERNELSYNC_MAINLOOP: for(batch_type iterationidx=0; iterationidx<total_num_iterations; iterationidx+=NUMPIPELINES_SYNC){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loop1 avg=analysis_loop1
	
		enablereduce = ON; 
		// #ifndef TRAD_PARTITION_AND_REDUCE_STRETEGY // NEWCHANGE.
		unsigned int ntravszs = 0;
		rtravstate[0] = SYNC_gettravstate(ON, kvdram0, globalparams[0], currentLOP, sourcestatsmarker);
		rtravstate[1] = SYNC_gettravstate(ON, kvdram1, globalparams[1], currentLOP, sourcestatsmarker);
		rtravstate[2] = SYNC_gettravstate(ON, kvdram2, globalparams[2], currentLOP, sourcestatsmarker);
		rtravstate[3] = SYNC_gettravstate(ON, kvdram3, globalparams[3], currentLOP, sourcestatsmarker);
		rtravstate[4] = SYNC_gettravstate(ON, kvdram4, globalparams[4], currentLOP, sourcestatsmarker);
		rtravstate[5] = SYNC_gettravstate(ON, kvdram5, globalparams[5], currentLOP, sourcestatsmarker);
		rtravstate[6] = SYNC_gettravstate(ON, kvdram6, globalparams[6], currentLOP, sourcestatsmarker);
		rtravstate[7] = SYNC_gettravstate(ON, kvdram7, globalparams[7], currentLOP, sourcestatsmarker);
		rtravstate[8] = SYNC_gettravstate(ON, kvdram8, globalparams[8], currentLOP, sourcestatsmarker);
		rtravstate[9] = SYNC_gettravstate(ON, kvdram9, globalparams[9], currentLOP, sourcestatsmarker);
		rtravstate[10] = SYNC_gettravstate(ON, kvdram10, globalparams[10], currentLOP, sourcestatsmarker);
		rtravstate[11] = SYNC_gettravstate(ON, kvdram11, globalparams[11], currentLOP, sourcestatsmarker);
		rtravstate[12] = SYNC_gettravstate(ON, kvdram12, globalparams[12], currentLOP, sourcestatsmarker);
		rtravstate[13] = SYNC_gettravstate(ON, kvdram13, globalparams[13], currentLOP, sourcestatsmarker);
		rtravstate[14] = SYNC_gettravstate(ON, kvdram14, globalparams[14], currentLOP, sourcestatsmarker);
		rtravstate[15] = SYNC_gettravstate(ON, kvdram15, globalparams[15], currentLOP, sourcestatsmarker);
		rtravstate[16] = SYNC_gettravstate(ON, kvdram16, globalparams[16], currentLOP, sourcestatsmarker);
		rtravstate[17] = SYNC_gettravstate(ON, kvdram17, globalparams[17], currentLOP, sourcestatsmarker);
		rtravstate[18] = SYNC_gettravstate(ON, kvdram18, globalparams[18], currentLOP, sourcestatsmarker);
		rtravstate[19] = SYNC_gettravstate(ON, kvdram19, globalparams[19], currentLOP, sourcestatsmarker);
		rtravstate[20] = SYNC_gettravstate(ON, kvdram20, globalparams[20], currentLOP, sourcestatsmarker);
		rtravstate[21] = SYNC_gettravstate(ON, kvdram21, globalparams[21], currentLOP, sourcestatsmarker);
		rtravstate[22] = SYNC_gettravstate(ON, kvdram22, globalparams[22], currentLOP, sourcestatsmarker);
		rtravstate[23] = SYNC_gettravstate(ON, kvdram23, globalparams[23], currentLOP, sourcestatsmarker);
		rtravstate[24] = SYNC_gettravstate(ON, kvdram24, globalparams[24], currentLOP, sourcestatsmarker);
		rtravstate[25] = SYNC_gettravstate(ON, kvdram25, globalparams[25], currentLOP, sourcestatsmarker);
		rtravstate[26] = SYNC_gettravstate(ON, kvdram26, globalparams[26], currentLOP, sourcestatsmarker);
		rtravstate[27] = SYNC_gettravstate(ON, kvdram27, globalparams[27], currentLOP, sourcestatsmarker);
		rtravstate[28] = SYNC_gettravstate(ON, kvdram28, globalparams[28], currentLOP, sourcestatsmarker);
		rtravstate[29] = SYNC_gettravstate(ON, kvdram29, globalparams[29], currentLOP, sourcestatsmarker);
		rtravstate[30] = SYNC_gettravstate(ON, kvdram30, globalparams[30], currentLOP, sourcestatsmarker);
		rtravstate[31] = SYNC_gettravstate(ON, kvdram31, globalparams[31], currentLOP, sourcestatsmarker);
		for(unsigned int i = 0; i < NUMSYNCTHREADS; i++){ ntravszs += rtravstate[i].size_kvs; }
		if(ntravszs > 0){ enablereduce = ON; } else { enablereduce = OFF; }
		// #endif 
		
		#ifdef SUP1
		if(iterationidx > 0){ pp1en_spreadvdata = ON; pp1en_spreadvmask = ON; pp1en_spreadandwrite = ON; } 
		else { pp1en_spreadvdata = OFF; pp1en_spreadvmask = OFF; pp1en_spreadandwrite = OFF; }
		#endif
	
		source_partition = iterationidx / 2;
		#ifdef _DEBUGMODE_KERNELPRINTS
		if(enablereduce==ON && _globalparams.ENABLE_SAVEVMASK==ON){ actsutilityobj->print3("### topkernelsync:: source_partition", "currentLOP", "NAp", source_partition, currentLOP, NAp); }
		#endif
		
		enablereducepp0 = enablereduce;	
		readvdata(enablereduce, ON, vdram, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffsetpp0_kvs, refbuffer, 0, 0, reducebuffersz, _globalparams);
		readandsynchronize(enablereduce, ON, kvdram0,kvdram1,kvdram2,kvdram3, vbuffer0_level1, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffsetpp0_kvs, _globalparams);
		readandsynchronize(enablereduce, ON, kvdram4,kvdram5,kvdram6,kvdram7, vbuffer1_level1, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffsetpp0_kvs, _globalparams);
		readandsynchronize(enablereduce, ON, kvdram8,kvdram9,kvdram10,kvdram11, vbuffer2_level1, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffsetpp0_kvs, _globalparams);
		readandsynchronize(enablereduce, ON, kvdram12,kvdram13,kvdram14,kvdram15, vbuffer3_level1, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffsetpp0_kvs, _globalparams);
		readandsynchronize(enablereduce, ON, kvdram16,kvdram17,kvdram18,kvdram19, vbuffer4_level1, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffsetpp0_kvs, _globalparams);
		readandsynchronize(enablereduce, ON, kvdram20,kvdram21,kvdram22,kvdram23, vbuffer5_level1, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffsetpp0_kvs, _globalparams);
		readandsynchronize(enablereduce, ON, kvdram24,kvdram25,kvdram26,kvdram27, vbuffer6_level1, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffsetpp0_kvs, _globalparams);
		readandsynchronize(enablereduce, ON, kvdram28,kvdram29,kvdram30,kvdram31, vbuffer7_level1, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffsetpp0_kvs, _globalparams);
		#ifdef SUP1
		spreadvdata(enablereducepp1, pp1en_spreadvdata, vbuffer0_level2, vbuffer0_level3,vbuffer1_level3,vbuffer2_level3,vbuffer3_level3,vbuffer4_level3,vbuffer5_level3,vbuffer6_level3,vbuffer7_level3, _globalparams);
		spreadvmask(enablereducepp1, pp1en_spreadvmask, ON, vmask0_level2, vmask0_level3,vmask1_level3,vmask2_level3,vmask3_level3,vmask4_level3,vmask5_level3,vmask6_level3,vmask7_level3, _globalparams); 
		if(enablereducepp1 == ON && pp1en_spreadvmask == ON){
			vmaskptemp0_level3[0] = vmaskptemp0_level2[0]; vmaskptemp0_level3[1] = vmaskptemp0_level2[1]; 
			vmaskptemp1_level3[0] = vmaskptemp0_level2[0]; vmaskptemp1_level3[1] = vmaskptemp0_level2[1]; 
			vmaskptemp2_level3[0] = vmaskptemp0_level2[0]; vmaskptemp2_level3[1] = vmaskptemp0_level2[1]; 
			vmaskptemp3_level3[0] = vmaskptemp0_level2[0]; vmaskptemp3_level3[1] = vmaskptemp0_level2[1]; 
			vmaskptemp4_level3[0] = vmaskptemp0_level2[0]; vmaskptemp4_level3[1] = vmaskptemp0_level2[1]; 
			vmaskptemp5_level3[0] = vmaskptemp0_level2[0]; vmaskptemp5_level3[1] = vmaskptemp0_level2[1]; 
			vmaskptemp6_level3[0] = vmaskptemp0_level2[0]; vmaskptemp6_level3[1] = vmaskptemp0_level2[1]; 
			vmaskptemp7_level3[0] = vmaskptemp0_level2[0]; vmaskptemp7_level3[1] = vmaskptemp0_level2[1]; 
		}
		#endif
		
		if(SWITCHcount % 2 == 0){ begincol_vmask = 0; } else { begincol_vmask = 8; };
		vmaskptemp0_level2[SWITCHcount % 2] = synchronizeandapply(enablereduce, ON, vbuffer0_level1,vbuffer1_level1,vbuffer2_level1,vbuffer3_level1,vbuffer4_level1,vbuffer5_level1,vbuffer6_level1,vbuffer7_level1, vbuffer0_level2, refbuffer, vmask0_level2, begincol_vmask, vreadoffsetpp0_kvs, _globalparams);
		SWITCHcount += 1;
		#ifdef SUP1
		spreadandwrite(enablereducepp1, pp1en_spreadandwrite, _globalparams.ENABLE_SAVEVMASK, _globalparams.ENABLE_SAVEVMASKP,  kvdram0,kvdram1,kvdram2,kvdram3,			vbuffer0_level3, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffsetpp1_kvs, reducebuffersz,
			vmask0_level3, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs,
			_globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, vmaskptemp0_level3,
			_globalparams);
		spreadandwrite(enablereducepp1, pp1en_spreadandwrite, _globalparams.ENABLE_SAVEVMASK, _globalparams.ENABLE_SAVEVMASKP,  kvdram4,kvdram5,kvdram6,kvdram7,			vbuffer1_level3, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffsetpp1_kvs, reducebuffersz,
			vmask1_level3, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs,
			_globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, vmaskptemp1_level3,
			_globalparams);
		spreadandwrite(enablereducepp1, pp1en_spreadandwrite, _globalparams.ENABLE_SAVEVMASK, _globalparams.ENABLE_SAVEVMASKP,  kvdram8,kvdram9,kvdram10,kvdram11,			vbuffer2_level3, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffsetpp1_kvs, reducebuffersz,
			vmask2_level3, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs,
			_globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, vmaskptemp2_level3,
			_globalparams);
		spreadandwrite(enablereducepp1, pp1en_spreadandwrite, _globalparams.ENABLE_SAVEVMASK, _globalparams.ENABLE_SAVEVMASKP,  kvdram12,kvdram13,kvdram14,kvdram15,			vbuffer3_level3, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffsetpp1_kvs, reducebuffersz,
			vmask3_level3, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs,
			_globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, vmaskptemp3_level3,
			_globalparams);
		spreadandwrite(enablereducepp1, pp1en_spreadandwrite, _globalparams.ENABLE_SAVEVMASK, _globalparams.ENABLE_SAVEVMASKP,  kvdram16,kvdram17,kvdram18,kvdram19,			vbuffer4_level3, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffsetpp1_kvs, reducebuffersz,
			vmask4_level3, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs,
			_globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, vmaskptemp4_level3,
			_globalparams);
		spreadandwrite(enablereducepp1, pp1en_spreadandwrite, _globalparams.ENABLE_SAVEVMASK, _globalparams.ENABLE_SAVEVMASKP,  kvdram20,kvdram21,kvdram22,kvdram23,			vbuffer5_level3, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffsetpp1_kvs, reducebuffersz,
			vmask5_level3, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs,
			_globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, vmaskptemp5_level3,
			_globalparams);
		spreadandwrite(enablereducepp1, pp1en_spreadandwrite, _globalparams.ENABLE_SAVEVMASK, _globalparams.ENABLE_SAVEVMASKP,  kvdram24,kvdram25,kvdram26,kvdram27,			vbuffer6_level3, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffsetpp1_kvs, reducebuffersz,
			vmask6_level3, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs,
			_globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, vmaskptemp6_level3,
			_globalparams);
		spreadandwrite(enablereducepp1, pp1en_spreadandwrite, _globalparams.ENABLE_SAVEVMASK, _globalparams.ENABLE_SAVEVMASKP, vdram, kvdram28,kvdram29,kvdram30,kvdram31,			vbuffer7_level3, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffsetpp1_kvs, reducebuffersz,
			vmask7_level3, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs,
			_globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, vmaskptemp7_level3,
			_globalparams);
		if(pp1en_spreadandwrite == ON){ vreadoffsetpp1_kvs += reducebuffersz * NUMPIPELINES_SYNC; }
		if(pp1en_spreadandwrite == ON){ MOVEcount += 1; if(MOVEcount % 2 == 0){ vmaskreadoffset_kvs += vmaskbuffersz_kvs; sourcestatsmarker += 1; }} 
		#endif
		
		spreadvdata(enablereduce, ON, vbuffer0_level2, vbuffer0_level3,vbuffer1_level3,vbuffer2_level3,vbuffer3_level3,vbuffer4_level3,vbuffer5_level3,vbuffer6_level3,vbuffer7_level3, _globalparams);
		spreadvmask(enablereduce, ON, ON, vmask0_level2, vmask0_level3,vmask1_level3,vmask2_level3,vmask3_level3,vmask4_level3,vmask5_level3,vmask6_level3,vmask7_level3, _globalparams); 
		if(enablereduce == ON){ 
			vmaskptemp0_level3[0] = vmaskptemp0_level2[0]; vmaskptemp0_level3[1] = vmaskptemp0_level2[1]; 
			vmaskptemp1_level3[0] = vmaskptemp0_level2[0]; vmaskptemp1_level3[1] = vmaskptemp0_level2[1]; 
			vmaskptemp2_level3[0] = vmaskptemp0_level2[0]; vmaskptemp2_level3[1] = vmaskptemp0_level2[1]; 
			vmaskptemp3_level3[0] = vmaskptemp0_level2[0]; vmaskptemp3_level3[1] = vmaskptemp0_level2[1]; 
			vmaskptemp4_level3[0] = vmaskptemp0_level2[0]; vmaskptemp4_level3[1] = vmaskptemp0_level2[1]; 
			vmaskptemp5_level3[0] = vmaskptemp0_level2[0]; vmaskptemp5_level3[1] = vmaskptemp0_level2[1]; 
			vmaskptemp6_level3[0] = vmaskptemp0_level2[0]; vmaskptemp6_level3[1] = vmaskptemp0_level2[1]; 
			vmaskptemp7_level3[0] = vmaskptemp0_level2[0]; vmaskptemp7_level3[1] = vmaskptemp0_level2[1]; 
		}
		#ifdef SUP1
		enablereducepp1 = enablereducepp0;	
		readvdata(enablereducepp1, pp1en_readandsynchronize, vdram, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffsetpp1_kvs, refbuffer, 0, 0, reducebuffersz, _globalparams);
		readandsynchronize(enablereducepp1, pp1en_readandsynchronize, kvdram0,kvdram1,kvdram2,kvdram3, vbuffer0_level1, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffsetpp1_kvs, _globalparams);
		readandsynchronize(enablereducepp1, pp1en_readandsynchronize, kvdram4,kvdram5,kvdram6,kvdram7, vbuffer1_level1, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffsetpp1_kvs, _globalparams);
		readandsynchronize(enablereducepp1, pp1en_readandsynchronize, kvdram8,kvdram9,kvdram10,kvdram11, vbuffer2_level1, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffsetpp1_kvs, _globalparams);
		readandsynchronize(enablereducepp1, pp1en_readandsynchronize, kvdram12,kvdram13,kvdram14,kvdram15, vbuffer3_level1, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffsetpp1_kvs, _globalparams);
		readandsynchronize(enablereducepp1, pp1en_readandsynchronize, kvdram16,kvdram17,kvdram18,kvdram19, vbuffer4_level1, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffsetpp1_kvs, _globalparams);
		readandsynchronize(enablereducepp1, pp1en_readandsynchronize, kvdram20,kvdram21,kvdram22,kvdram23, vbuffer5_level1, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffsetpp1_kvs, _globalparams);
		readandsynchronize(enablereducepp1, pp1en_readandsynchronize, kvdram24,kvdram25,kvdram26,kvdram27, vbuffer6_level1, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffsetpp1_kvs, _globalparams);
		readandsynchronize(enablereducepp1, pp1en_readandsynchronize, kvdram28,kvdram29,kvdram30,kvdram31, vbuffer7_level1, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffsetpp1_kvs, _globalparams);
	
		#endif 
	
		spreadandwrite(enablereduce, ON, _globalparams.ENABLE_SAVEVMASK, _globalparams.ENABLE_SAVEVMASKP,  kvdram0,kvdram1,kvdram2,kvdram3,	
			vbuffer0_level3, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffsetpp0_kvs, reducebuffersz,
			vmask0_level3, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs,
			_globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, vmaskptemp0_level3,
			_globalparams);
		spreadandwrite(enablereduce, ON, _globalparams.ENABLE_SAVEVMASK, _globalparams.ENABLE_SAVEVMASKP,  kvdram4,kvdram5,kvdram6,kvdram7,	
			vbuffer1_level3, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffsetpp0_kvs, reducebuffersz,
			vmask1_level3, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs,
			_globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, vmaskptemp1_level3,
			_globalparams);
		spreadandwrite(enablereduce, ON, _globalparams.ENABLE_SAVEVMASK, _globalparams.ENABLE_SAVEVMASKP,  kvdram8,kvdram9,kvdram10,kvdram11,	
			vbuffer2_level3, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffsetpp0_kvs, reducebuffersz,
			vmask2_level3, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs,
			_globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, vmaskptemp2_level3,
			_globalparams);
		spreadandwrite(enablereduce, ON, _globalparams.ENABLE_SAVEVMASK, _globalparams.ENABLE_SAVEVMASKP,  kvdram12,kvdram13,kvdram14,kvdram15,	
			vbuffer3_level3, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffsetpp0_kvs, reducebuffersz,
			vmask3_level3, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs,
			_globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, vmaskptemp3_level3,
			_globalparams);
		spreadandwrite(enablereduce, ON, _globalparams.ENABLE_SAVEVMASK, _globalparams.ENABLE_SAVEVMASKP,  kvdram16,kvdram17,kvdram18,kvdram19,	
			vbuffer4_level3, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffsetpp0_kvs, reducebuffersz,
			vmask4_level3, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs,
			_globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, vmaskptemp4_level3,
			_globalparams);
		spreadandwrite(enablereduce, ON, _globalparams.ENABLE_SAVEVMASK, _globalparams.ENABLE_SAVEVMASKP,  kvdram20,kvdram21,kvdram22,kvdram23,	
			vbuffer5_level3, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffsetpp0_kvs, reducebuffersz,
			vmask5_level3, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs,
			_globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, vmaskptemp5_level3,
			_globalparams);
		spreadandwrite(enablereduce, ON, _globalparams.ENABLE_SAVEVMASK, _globalparams.ENABLE_SAVEVMASKP,  kvdram24,kvdram25,kvdram26,kvdram27,	
			vbuffer6_level3, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffsetpp0_kvs, reducebuffersz,
			vmask6_level3, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs,
			_globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, vmaskptemp6_level3,
			_globalparams);
		spreadandwrite(enablereduce, ON, _globalparams.ENABLE_SAVEVMASK, _globalparams.ENABLE_SAVEVMASKP, vdram, kvdram28,kvdram29,kvdram30,kvdram31,	
			vbuffer7_level3, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffsetpp0_kvs, reducebuffersz,
			vmask7_level3, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs,
			_globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, vmaskptemp7_level3,
			_globalparams);
		vreadoffsetpp0_kvs += reducebuffersz * NUMPIPELINES_SYNC;
		MOVEcount += 1; if(MOVEcount % 2 == 0){ vmaskreadoffset_kvs += vmaskbuffersz_kvs; sourcestatsmarker += 1; } 
		#ifdef SUP1
		if(SWITCHcount % 2 == 0){ begincol_vmask = 0; } else { begincol_vmask = 8; };
		vmaskptemp0_level2[SWITCHcount % 2] = synchronizeandapply(enablereducepp1, pp1en_syncandapply, vbuffer0_level1,vbuffer1_level1,vbuffer2_level1,vbuffer3_level1,vbuffer4_level1,vbuffer5_level1,vbuffer6_level1,vbuffer7_level1, vbuffer0_level2, refbuffer, vmask0_level2, begincol_vmask, vreadoffsetpp1_kvs, _globalparams);
		if(enablereducepp1 == ON && pp1en_syncandapply == ON){ SWITCHcount += 1; }
		#endif
	}
	
	#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
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
	if(_globalparams.ENABLE_SAVEVMASK == ON){ cout<< TIMINGRESULTSCOLOR <<"num active vertices for this iteration: "<<actsutilityobj->globalstats_getactvvsseen()<< RESET <<endl; }
	actsutilityobj->globalstats_setactvvsseen(0);
	#endif 
	return;
}
// }
#endif 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" {
void 
	#ifdef SW 
	acts:: 
	#endif
topkernel(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11,uint512_dt * kvdram12,uint512_dt * kvdram13,uint512_dt * kvdram14,uint512_dt * kvdram15,uint512_dt * kvdram16,uint512_dt * kvdram17,uint512_dt * kvdram18,uint512_dt * kvdram19,uint512_dt * kvdram20,uint512_dt * kvdram21,uint512_dt * kvdram22,uint512_dt * kvdram23,uint512_dt * kvdram24,uint512_dt * kvdram25,uint512_dt * kvdram26,uint512_dt * kvdram27,uint512_dt * kvdram28,uint512_dt * kvdram29,uint512_dt * kvdram30,uint512_dt * kvdram31, uint512_dt * vdram){
 
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
 
#pragma HLS INTERFACE m_axi port = kvdram16 offset = slave bundle = gmem16
 
#pragma HLS INTERFACE m_axi port = kvdram17 offset = slave bundle = gmem17
 
#pragma HLS INTERFACE m_axi port = kvdram18 offset = slave bundle = gmem18
 
#pragma HLS INTERFACE m_axi port = kvdram19 offset = slave bundle = gmem19
 
#pragma HLS INTERFACE m_axi port = kvdram20 offset = slave bundle = gmem20
 
#pragma HLS INTERFACE m_axi port = kvdram21 offset = slave bundle = gmem21
 
#pragma HLS INTERFACE m_axi port = kvdram22 offset = slave bundle = gmem22
 
#pragma HLS INTERFACE m_axi port = kvdram23 offset = slave bundle = gmem23
 
#pragma HLS INTERFACE m_axi port = kvdram24 offset = slave bundle = gmem24
 
#pragma HLS INTERFACE m_axi port = kvdram25 offset = slave bundle = gmem25
 
#pragma HLS INTERFACE m_axi port = kvdram26 offset = slave bundle = gmem26
 
#pragma HLS INTERFACE m_axi port = kvdram27 offset = slave bundle = gmem27
 
#pragma HLS INTERFACE m_axi port = kvdram28 offset = slave bundle = gmem28
 
#pragma HLS INTERFACE m_axi port = kvdram29 offset = slave bundle = gmem29
 
#pragma HLS INTERFACE m_axi port = kvdram30 offset = slave bundle = gmem30
 
#pragma HLS INTERFACE m_axi port = kvdram31 offset = slave bundle = gmem31
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem32

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
#pragma HLS INTERFACE s_axilite port = kvdram16 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram17 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram18 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram19 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram20 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram21 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram22 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram23 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram24 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram25 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram26 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram27 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram28 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram29 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram30 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram31 bundle = control
#pragma HLS INTERFACE s_axilite port = vdram bundle = control

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
#pragma HLS DATA_PACK variable = kvdram16
#pragma HLS DATA_PACK variable = kvdram17
#pragma HLS DATA_PACK variable = kvdram18
#pragma HLS DATA_PACK variable = kvdram19
#pragma HLS DATA_PACK variable = kvdram20
#pragma HLS DATA_PACK variable = kvdram21
#pragma HLS DATA_PACK variable = kvdram22
#pragma HLS DATA_PACK variable = kvdram23
#pragma HLS DATA_PACK variable = kvdram24
#pragma HLS DATA_PACK variable = kvdram25
#pragma HLS DATA_PACK variable = kvdram26
#pragma HLS DATA_PACK variable = kvdram27
#pragma HLS DATA_PACK variable = kvdram28
#pragma HLS DATA_PACK variable = kvdram29
#pragma HLS DATA_PACK variable = kvdram30
#pragma HLS DATA_PACK variable = kvdram31
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS3) || defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	unsigned int numIters = actsprocobj.GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID], 0);
	GRAPHITER_LOOP: for(unsigned int GraphIter=0; GraphIter<numIters; GraphIter++){
		cout<<">>> swkernel::runapp(ACTS_1by1): Iteration: "<<GraphIter<<endl;
		
		#ifdef _WIDEWORD
 
		kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = GraphIter;
 
		kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = GraphIter;
 
		kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = GraphIter;
 
		kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = GraphIter;
 
		kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = GraphIter;
 
		kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = GraphIter;
 
		kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = GraphIter;
 
		kvdram7[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = GraphIter;
 
		kvdram8[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = GraphIter;
 
		kvdram9[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = GraphIter;
 
		kvdram10[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = GraphIter;
 
		kvdram11[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = GraphIter;
 
		kvdram12[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = GraphIter;
 
		kvdram13[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = GraphIter;
 
		kvdram14[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = GraphIter;
 
		kvdram15[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = GraphIter;
 
		kvdram16[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = GraphIter;
 
		kvdram17[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = GraphIter;
 
		kvdram18[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = GraphIter;
 
		kvdram19[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = GraphIter;
 
		kvdram20[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = GraphIter;
 
		kvdram21[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = GraphIter;
 
		kvdram22[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = GraphIter;
 
		kvdram23[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = GraphIter;
 
		kvdram24[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = GraphIter;
 
		kvdram25[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = GraphIter;
 
		kvdram26[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = GraphIter;
 
		kvdram27[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = GraphIter;
 
		kvdram28[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = GraphIter;
 
		kvdram29[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = GraphIter;
 
		kvdram30[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = GraphIter;
 
		kvdram31[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = GraphIter;
		#else 
 
		kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram7[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram8[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram9[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram10[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram11[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram12[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram13[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram14[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram15[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram16[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram17[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram18[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram19[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram20[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram21[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram22[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram23[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram24[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram25[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram26[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram27[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram28[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram29[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram30[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram31[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
		#endif 
		
		#ifdef ACTSPROC_1by1
actsprocobj.topkernelproc(kvdram0);	
actsprocobj.topkernelproc(kvdram1);	
actsprocobj.topkernelproc(kvdram2);	
actsprocobj.topkernelproc(kvdram3);	
actsprocobj.topkernelproc(kvdram4);	
actsprocobj.topkernelproc(kvdram5);	
actsprocobj.topkernelproc(kvdram6);	
actsprocobj.topkernelproc(kvdram7);	
actsprocobj.topkernelproc(kvdram8);	
actsprocobj.topkernelproc(kvdram9);	
actsprocobj.topkernelproc(kvdram10);	
actsprocobj.topkernelproc(kvdram11);	
actsprocobj.topkernelproc(kvdram12);	
actsprocobj.topkernelproc(kvdram13);	
actsprocobj.topkernelproc(kvdram14);	
actsprocobj.topkernelproc(kvdram15);	
actsprocobj.topkernelproc(kvdram16);	
actsprocobj.topkernelproc(kvdram17);	
actsprocobj.topkernelproc(kvdram18);	
actsprocobj.topkernelproc(kvdram19);	
actsprocobj.topkernelproc(kvdram20);	
actsprocobj.topkernelproc(kvdram21);	
actsprocobj.topkernelproc(kvdram22);	
actsprocobj.topkernelproc(kvdram23);	
actsprocobj.topkernelproc(kvdram24);	
actsprocobj.topkernelproc(kvdram25);	
actsprocobj.topkernelproc(kvdram26);	
actsprocobj.topkernelproc(kvdram27);	
actsprocobj.topkernelproc(kvdram28);	
actsprocobj.topkernelproc(kvdram29);	
actsprocobj.topkernelproc(kvdram30);	
actsprocobj.topkernelproc(kvdram31);	
		#endif 
		#ifdef ACTSPROC_2by1
actsprocobj.topkernelproc(kvdram0,kvdram1);	
actsprocobj.topkernelproc(kvdram2,kvdram3);	
actsprocobj.topkernelproc(kvdram4,kvdram5);	
actsprocobj.topkernelproc(kvdram6,kvdram7);	
actsprocobj.topkernelproc(kvdram8,kvdram9);	
actsprocobj.topkernelproc(kvdram10,kvdram11);	
actsprocobj.topkernelproc(kvdram12,kvdram13);	
actsprocobj.topkernelproc(kvdram14,kvdram15);	
actsprocobj.topkernelproc(kvdram16,kvdram17);	
actsprocobj.topkernelproc(kvdram18,kvdram19);	
actsprocobj.topkernelproc(kvdram20,kvdram21);	
actsprocobj.topkernelproc(kvdram22,kvdram23);	
actsprocobj.topkernelproc(kvdram24,kvdram25);	
actsprocobj.topkernelproc(kvdram26,kvdram27);	
actsprocobj.topkernelproc(kvdram28,kvdram29);	
actsprocobj.topkernelproc(kvdram30,kvdram31);	
		#endif 	
		#ifdef ACTSPROC_4by1
actsprocobj.topkernelproc(kvdram0,kvdram1,kvdram2,kvdram3);	
actsprocobj.topkernelproc(kvdram4,kvdram5,kvdram6,kvdram7);	
actsprocobj.topkernelproc(kvdram8,kvdram9,kvdram10,kvdram11);	
actsprocobj.topkernelproc(kvdram12,kvdram13,kvdram14,kvdram15);	
actsprocobj.topkernelproc(kvdram16,kvdram17,kvdram18,kvdram19);	
actsprocobj.topkernelproc(kvdram20,kvdram21,kvdram22,kvdram23);	
actsprocobj.topkernelproc(kvdram24,kvdram25,kvdram26,kvdram27);	
actsprocobj.topkernelproc(kvdram28,kvdram29,kvdram30,kvdram31);	
		#endif 	
		#ifdef ACTSPROC_8by1
actsprocobj.topkernelproc(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7);	
actsprocobj.topkernelproc(kvdram8,kvdram9,kvdram10,kvdram11,kvdram12,kvdram13,kvdram14,kvdram15);	
actsprocobj.topkernelproc(kvdram16,kvdram17,kvdram18,kvdram19,kvdram20,kvdram21,kvdram22,kvdram23);	
actsprocobj.topkernelproc(kvdram24,kvdram25,kvdram26,kvdram27,kvdram28,kvdram29,kvdram30,kvdram31);	
		#endif 	
actssyncobj.topkernelsync(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9,kvdram10,kvdram11,kvdram12,kvdram13,kvdram14,kvdram15,kvdram16,kvdram17,kvdram18,kvdram19,kvdram20,kvdram21,kvdram22,kvdram23,kvdram24,kvdram25,kvdram26,kvdram27,kvdram28,kvdram29,kvdram30,kvdram31, vdram);
		
		#ifdef _WIDEWORD
		unsigned int _BASEOFFSETKVS_VERTICESPARTITIONMASK = kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESPARTITIONMASK].range(31, 0);
		#else 
		unsigned int _BASEOFFSETKVS_VERTICESPARTITIONMASK = kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESPARTITIONMASK].data[0].key;
		#endif 
		unsigned int totalactvvp = 0;
		#ifdef SW 
		cout<<endl<<"active partitions for iteration "<<GraphIter+1<<": ";
		#endif 
		for(unsigned int i=0; i<256; i++){
			#ifdef _WIDEWORD
			unsigned int gmask = kvdram0[_BASEOFFSETKVS_VERTICESPARTITIONMASK + i].range(31, 0);
			#else 
			unsigned int gmask = kvdram0[_BASEOFFSETKVS_VERTICESPARTITIONMASK + i].data[0].key;
			#endif 
			totalactvvp += gmask;
			#ifdef SW 
			if(gmask > 0){ cout<<i<<", "; }
			#endif 
		}
		#ifdef SW 
		cout<<""<<endl;
		if(totalactvvp == 0){ cout<<"swkernel::runapp: no more active vertices to process. exiting... "<<endl; break; }
		#endif 
	}
	return;
}
}







