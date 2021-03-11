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
#include "acts_synchronize.h"
using namespace std;

// CRITICAL FIXME. having made keyvalue_buffer_t & keyvalue_vbuffer_t arbitrary precision, fix INVALIDDATA

#define NUMSYNCPIPELINES 2
#if NUMSYNCPIPELINES==1
#define SUP0
#endif 
#if NUMSYNCPIPELINES==2
#define SUP0
#define SUP1
#endif

#ifdef SW
acts_synchronize::acts_synchronize(){ actsutilityobj = new actsutility(); }
acts_synchronize::~acts_synchronize(){}
#endif
#ifdef SWEMU
actsutility * actsutilityobj = new actsutility();
#endif

// NOTE: ACTS NOT automated for all datasets. see CRITICAL AUTOMATEME. (dataset dependent).

// functions (basic)
unsigned int
	#ifdef SW 
	acts_synchronize::
	#endif 
amin(unsigned int val1, unsigned int val2){
	if(val1 < val2){ return val1; }
	else { return val2; }
}
unsigned int
	#ifdef SW 
	acts_synchronize::
	#endif 
aplus(unsigned int val1, unsigned int val2){
	return val1 + val2;
}

// functions (bit manipulation)
unsigned int 
	#ifdef SW 
	acts_synchronize::
	#endif
GETMASK_UINT(unsigned int index, unsigned int size){
	unsigned int A = ((1 << (size)) - 1);
	unsigned int B = A << index;
	return B;
}
unsigned int 
	#ifdef SW 
	acts_synchronize::
	#endif
READFROM_UINT(unsigned int data, unsigned int index, unsigned int size){ 
	return (((data) & GETMASK_UINT((index), (size))) >> (index)); 
}
void
	#ifdef SW 
	acts_synchronize::
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
	acts_synchronize::
	#endif 
GETKV(keyvalue_buffer_t data){
	#pragma HLS INLINE
	keyvalue_t res; 
	#ifdef _WIDEWORD
	res.key = data.range(21, 0);
	res.value = data.range(31, 22);
	#else 
	res.key = data.key;
	res.value = data.value;	
	#endif 
	return res;
}
keyvalue_buffer_t 
	#ifdef SW 
	acts_synchronize::
	#endif 
GETKV(keyvalue_t data){
	#pragma HLS INLINE
	keyvalue_buffer_t res;
	#ifdef _WIDEWORD
	res.range(21, 0) = data.key;
	res.range(31, 22) = data.value;
	#else
	res.key = data.key;
	res.value = data.value;	
	#endif 
	return res;
}
keyy_t 
	#ifdef SW 
	acts_synchronize::
	#endif 
GETK(uint32_type data){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	return data.range(21, 0);
	#else
	return data;
	#endif
}
value_t 
	#ifdef SW 
	acts_synchronize::
	#endif 
GETV(uint32_type data){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	return data.range(9, 0);
	#else
	return data;
	#endif
}
keyvalue_t 
	#ifdef SW 
	acts_synchronize::
	#endif 
GETKV2(keyvalue_vbuffer_t data){
	#pragma HLS INLINE
	keyvalue_t res;
	#ifdef _WIDEWORD
	res.key = data.range(15, 0); 
	res.value = data.range(31, 16);
	#else 
	res.key = data.key;
	res.value = data.value;	
	#endif 
	return res;
}
keyvalue_vbuffer_t 
	#ifdef SW 
	acts_synchronize::
	#endif 
GETKV2(keyvalue_t data){
	#pragma HLS INLINE
	keyvalue_vbuffer_t res;
	#ifdef _WIDEWORD
	res.range(15, 0) = data.key; 
	res.range(31, 16) = data.value;
	#else
	res.key = data.key;
	res.value = data.value;
	#endif 
	return res;
}
keyy_t 
	#ifdef SW 
	acts_process::
	#endif 
GETK2(uint32_type data){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	return data.range(15, 0);
	#else
	return data;
	#endif
}
value_t 
	#ifdef SW 
	acts_process::
	#endif 
GETV2(uint32_type data){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	return data.range(15, 0);
	#else
	return data;
	#endif
}
uint32_type 
	#ifdef SW 
	acts_synchronize::
	#endif 
convertvmasktouint32(uintNUMPby2_type vmask[BLOCKRAM_SIZE], unsigned int index){
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
keyy_t 
	#ifdef SW 
	acts_synchronize::
	#endif 
GETKEYENTRY(uint512_dt data, unsigned int v){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	return data.range(32 * ((v * 2) + 1) - 1, (v * 2) * 32);
	#else 
	return data.data[vindex].key;	
	#endif
}
value_t 
	#ifdef SW 
	acts_synchronize::
	#endif 
GETVALUEENTRY(uint512_dt data, unsigned int v){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	return data.range(32 * (((v * 2) + 1) + 1) - 1, (v * 2 + 1) * 32);
	#else 
	return data.data[vindex].value;	
	#endif
}

// functions (acts_synchronize utilities)
batch_type
	#ifdef SW 
	acts_synchronize::
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
	acts_synchronize::
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
	actsutilityobj->printglobalparameters("acts_synchronize::getglobalparams:: printing global parameters", globalparams);
	#endif
	return globalparams;
}
travstate_t 
	#ifdef SW 
	acts_synchronize::
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
void 
	#ifdef SW 
	acts_synchronize::
	#endif  
savevmaskp(bool_type enable1, bool_type enable2, uint512_dt * kvdram, unsigned int offset_kvs, uint32_type vmask_p, globalparams_t globalparams){
	#pragma HLS function_instantiate variable=kvdram
	if(enable1 == OFF || enable2 == OFF){ return; }
	#ifdef _WIDEWORD
	kvdram[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + offset_kvs].range(31, 0) = vmask_p;
	#else
	kvdram[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + offset_kvs].data[0].key = vmask_p;
	#endif 
	return;
}

// functions (resets)
void 
	#ifdef SW 
	acts_synchronize::
	#endif 
resetvmask(bool_type enable, uintNUMPby2_type vmask[BLOCKRAM_SIZE]){
	#pragma HLS function_instantiate variable=vmask
	#pragma HLS INLINE OFF //
	if(enable == OFF){ return; }
	for(buffer_type i=0; i<BLOCKRAM_SIZE; i++){ 
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

// functions (accessors)
void // 
	#ifdef SW 
	acts_synchronize::
	#endif 
readvdata(bool_type enable1, bool_type enable2, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams){
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
	acts_synchronize::
	#endif 
savevdata(bool_type enable1, bool_type enable2, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	#pragma HLS function_instantiate variable=kvdram
	if(enable1 == OFF || enable2 == OFF){ return; }
	analysis_type analysis_loopcount =  APPLYVERTEXBUFFERSZ / 2;
	
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
	if(dramoffset_kvs >= globalparams.BASEOFFSETKVS_VERTEXPTR && dramoffset_kvs < globalparams.BASEOFFSETKVS_VERTEXPTR + VERTEXPTRSSZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"savevdata:: vertices saved: offset: "<<dramoffset_kvs-globalparams.BASEOFFSETKVS_VERTEXPTR * VECTOR_SIZE<<"-"<<((dramoffset_kvs + size_kvs)-globalparams.BASEOFFSETKVS_VERTEXPTR) * VECTOR_SIZE<<", number of vertex datas written: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	if(dramoffset_kvs >= globalparams.BASEOFFSETKVS_VERTICESDATA && dramoffset_kvs < globalparams.BASEOFFSETKVS_VERTICESDATA + VERTICESDATASZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"savevdata:: vertices saved: offset: "<<(dramoffset_kvs-globalparams.BASEOFFSETKVS_VERTICESDATA) * VECTOR_SIZE<<"-"<<((dramoffset_kvs-globalparams.BASEOFFSETKVS_VERTICESDATA) + size_kvs) * VECTOR_SIZE<<", number of vertex datas written: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }					
	#endif
	return;
}

void 
	#ifdef SW 
	acts_synchronize::
	#endif 
readvdata(bool_type enable1, bool_type enable2, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type buffer1offset_kvs, keyvalue_vbuffer_t buffer2[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], batch_type buffer2offset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable1 == OFF || enable2 == OFF){ return; }
	analysis_type analysis_loopcount = BLOCKRAM_SIZE;
		
	READVDATA_LOOP: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE II=1
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
		
		#ifdef _WIDEWORD
		mykeyvalue10.key = kvdram[dramoffset_kvs + i].range(31, 0); 
		mykeyvalue10.value = kvdram[dramoffset_kvs + i].range(63, 32); 
		mykeyvalue11.key = kvdram[dramoffset_kvs + i].range(95, 64); 
		mykeyvalue11.value = kvdram[dramoffset_kvs + i].range(127, 96); 
		mykeyvalue12.key = kvdram[dramoffset_kvs + i].range(159, 128); 
		mykeyvalue12.value = kvdram[dramoffset_kvs + i].range(191, 160); 
		mykeyvalue13.key = kvdram[dramoffset_kvs + i].range(223, 192); 
		mykeyvalue13.value = kvdram[dramoffset_kvs + i].range(255, 224); 
		mykeyvalue14.key = kvdram[dramoffset_kvs + i].range(287, 256); 
		mykeyvalue14.value = kvdram[dramoffset_kvs + i].range(319, 288); 
		mykeyvalue15.key = kvdram[dramoffset_kvs + i].range(351, 320); 
		mykeyvalue15.value = kvdram[dramoffset_kvs + i].range(383, 352); 
		mykeyvalue16.key = kvdram[dramoffset_kvs + i].range(415, 384); 
		mykeyvalue16.value = kvdram[dramoffset_kvs + i].range(447, 416); 
		mykeyvalue17.key = kvdram[dramoffset_kvs + i].range(479, 448); 
		mykeyvalue17.value = kvdram[dramoffset_kvs + i].range(511, 480); 
		mykeyvalue20.key = kvdram[dramoffset_kvs + i].range(31, 0); 
		mykeyvalue20.value = kvdram[dramoffset_kvs + i].range(63, 32); 
		mykeyvalue21.key = kvdram[dramoffset_kvs + i].range(95, 64); 
		mykeyvalue21.value = kvdram[dramoffset_kvs + i].range(127, 96); 
		mykeyvalue22.key = kvdram[dramoffset_kvs + i].range(159, 128); 
		mykeyvalue22.value = kvdram[dramoffset_kvs + i].range(191, 160); 
		mykeyvalue23.key = kvdram[dramoffset_kvs + i].range(223, 192); 
		mykeyvalue23.value = kvdram[dramoffset_kvs + i].range(255, 224); 
		mykeyvalue24.key = kvdram[dramoffset_kvs + i].range(287, 256); 
		mykeyvalue24.value = kvdram[dramoffset_kvs + i].range(319, 288); 
		mykeyvalue25.key = kvdram[dramoffset_kvs + i].range(351, 320); 
		mykeyvalue25.value = kvdram[dramoffset_kvs + i].range(383, 352); 
		mykeyvalue26.key = kvdram[dramoffset_kvs + i].range(415, 384); 
		mykeyvalue26.value = kvdram[dramoffset_kvs + i].range(447, 416); 
		mykeyvalue27.key = kvdram[dramoffset_kvs + i].range(479, 448); 
		mykeyvalue27.value = kvdram[dramoffset_kvs + i].range(511, 480); 
		#else 
		mykeyvalue10.key = kvdram[dramoffset_kvs + i].data[0].key; 
		mykeyvalue10.value = kvdram[dramoffset_kvs + i].data[0].value; 
		mykeyvalue11.key = kvdram[dramoffset_kvs + i].data[1].key; 
		mykeyvalue11.value = kvdram[dramoffset_kvs + i].data[1].value; 
		mykeyvalue12.key = kvdram[dramoffset_kvs + i].data[2].key; 
		mykeyvalue12.value = kvdram[dramoffset_kvs + i].data[2].value; 
		mykeyvalue13.key = kvdram[dramoffset_kvs + i].data[3].key; 
		mykeyvalue13.value = kvdram[dramoffset_kvs + i].data[3].value; 
		mykeyvalue14.key = kvdram[dramoffset_kvs + i].data[4].key; 
		mykeyvalue14.value = kvdram[dramoffset_kvs + i].data[4].value; 
		mykeyvalue15.key = kvdram[dramoffset_kvs + i].data[5].key; 
		mykeyvalue15.value = kvdram[dramoffset_kvs + i].data[5].value; 
		mykeyvalue16.key = kvdram[dramoffset_kvs + i].data[6].key; 
		mykeyvalue16.value = kvdram[dramoffset_kvs + i].data[6].value; 
		mykeyvalue17.key = kvdram[dramoffset_kvs + i].data[7].key; 
		mykeyvalue17.value = kvdram[dramoffset_kvs + i].data[7].value; 
		mykeyvalue20.key = kvdram[dramoffset_kvs + i].data[0].key; 
		mykeyvalue20.value = kvdram[dramoffset_kvs + i].data[0].value; 
		mykeyvalue21.key = kvdram[dramoffset_kvs + i].data[1].key; 
		mykeyvalue21.value = kvdram[dramoffset_kvs + i].data[1].value; 
		mykeyvalue22.key = kvdram[dramoffset_kvs + i].data[2].key; 
		mykeyvalue22.value = kvdram[dramoffset_kvs + i].data[2].value; 
		mykeyvalue23.key = kvdram[dramoffset_kvs + i].data[3].key; 
		mykeyvalue23.value = kvdram[dramoffset_kvs + i].data[3].value; 
		mykeyvalue24.key = kvdram[dramoffset_kvs + i].data[4].key; 
		mykeyvalue24.value = kvdram[dramoffset_kvs + i].data[4].value; 
		mykeyvalue25.key = kvdram[dramoffset_kvs + i].data[5].key; 
		mykeyvalue25.value = kvdram[dramoffset_kvs + i].data[5].value; 
		mykeyvalue26.key = kvdram[dramoffset_kvs + i].data[6].key; 
		mykeyvalue26.value = kvdram[dramoffset_kvs + i].data[6].value; 
		mykeyvalue27.key = kvdram[dramoffset_kvs + i].data[7].key; 
		mykeyvalue27.value = kvdram[dramoffset_kvs + i].data[7].value; 
		#endif 
		
		buffer1[begincol + 0][buffer1offset_kvs + i] = GETKV2(mykeyvalue10);
		buffer1[begincol + 1][buffer1offset_kvs + i] = GETKV2(mykeyvalue11);
		buffer1[begincol + 2][buffer1offset_kvs + i] = GETKV2(mykeyvalue12);
		buffer1[begincol + 3][buffer1offset_kvs + i] = GETKV2(mykeyvalue13);
		buffer1[begincol + 4][buffer1offset_kvs + i] = GETKV2(mykeyvalue14);
		buffer1[begincol + 5][buffer1offset_kvs + i] = GETKV2(mykeyvalue15);
		buffer1[begincol + 6][buffer1offset_kvs + i] = GETKV2(mykeyvalue16);
		buffer1[begincol + 7][buffer1offset_kvs + i] = GETKV2(mykeyvalue17);
		buffer2[begincol + 0][buffer2offset_kvs + i] = GETKV2(mykeyvalue20);
		buffer2[begincol + 1][buffer2offset_kvs + i] = GETKV2(mykeyvalue21);
		buffer2[begincol + 2][buffer2offset_kvs + i] = GETKV2(mykeyvalue22);
		buffer2[begincol + 3][buffer2offset_kvs + i] = GETKV2(mykeyvalue23);
		buffer2[begincol + 4][buffer2offset_kvs + i] = GETKV2(mykeyvalue24);
		buffer2[begincol + 5][buffer2offset_kvs + i] = GETKV2(mykeyvalue25);
		buffer2[begincol + 6][buffer2offset_kvs + i] = GETKV2(mykeyvalue26);
		buffer2[begincol + 7][buffer2offset_kvs + i] = GETKV2(mykeyvalue27);
		
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<< TIMINGRESULTSCOLOR<<"readvdata:: vertices read: offset: "<<dramoffset_kvs * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl;
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(dramoffset_kvs >= globalparams.BASEOFFSETKVS_VERTEXPTR && dramoffset_kvs < globalparams.BASEOFFSETKVS_VERTEXPTR + VERTEXPTRSSZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"readvdata:: vertices read: offset: "<<(dramoffset_kvs - BASEOFFSETKVS_VERTEXPTR) * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs - BASEOFFSETKVS_VERTEXPTR) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	if(dramoffset_kvs >= globalparams.BASEOFFSETKVS_VERTICESDATA && dramoffset_kvs < globalparams.BASEOFFSETKVS_VERTICESDATA + VERTICESDATASZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"readvdata:: vertices read: offset: "<<(dramoffset_kvs - globalparams.BASEOFFSETKVS_VERTICESDATA) * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs - globalparams.BASEOFFSETKVS_VERTICESDATA) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	#endif
	return;
}

void 
	#ifdef SW 
	acts_synchronize::
	#endif 
savevmasks(bool_type enable1, bool_type enable2, uint512_dt * kvdram, uintNUMPby2_type vmask[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable1 == OFF || enable2 == OFF){ return; }
	analysis_type analysis_loopcount1 = BLOCKRAM_SIZE;
	analysis_type analysis_loopcount2 = BLOCKRAM_SIZE / 16;
	uint32_type bitsbuffer[MAXREDUCEBUFFERSZ];
	
	buffer_type transfsize = size_kvs * 16;
	
	SAVEVMASKS_LOOP1: for (buffer_type i=0; i<transfsize; i++){ // transfsize, reducebuffersz
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount1 avg=analysis_loopcount1
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
	SAVEVMASKS_LOOP2: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount2 avg=analysis_loopcount2
	#pragma HLS PIPELINE II=8	
		#ifdef _WIDEWORD
		tempbuffer[0][i] = bitsbuffer[index + 0];
		tempbuffer[1][i] = bitsbuffer[index + 1];
		tempbuffer[2][i] = bitsbuffer[index + 2];
		tempbuffer[3][i] = bitsbuffer[index + 3];
		tempbuffer[4][i] = bitsbuffer[index + 4];
		tempbuffer[5][i] = bitsbuffer[index + 5];
		tempbuffer[6][i] = bitsbuffer[index + 6];
		tempbuffer[7][i] = bitsbuffer[index + 7];
		tempbuffer[8][i] = bitsbuffer[index + 8];
		tempbuffer[9][i] = bitsbuffer[index + 9];
		tempbuffer[10][i] = bitsbuffer[index + 10];
		tempbuffer[11][i] = bitsbuffer[index + 11];
		tempbuffer[12][i] = bitsbuffer[index + 12];
		tempbuffer[13][i] = bitsbuffer[index + 13];
		tempbuffer[14][i] = bitsbuffer[index + 14];
		tempbuffer[15][i] = bitsbuffer[index + 15];
		#else 
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
		#endif 
		
		index += VECTOR_SIZE * 2;
	}
	
	SAVEKEYVALUES2_LOOP: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount2 avg=analysis_loopcount2
	#pragma HLS PIPELINE II=1
	
		#ifdef _WIDEWORD
		kvdram[offset_kvs + i].range(31, 0) = tempbuffer[0][i]; 
		kvdram[offset_kvs + i].range(63, 32) = tempbuffer[1][i]; 
		kvdram[offset_kvs + i].range(95, 64) = tempbuffer[2][i]; 
		kvdram[offset_kvs + i].range(127, 96) = tempbuffer[3][i]; 
		kvdram[offset_kvs + i].range(159, 128) = tempbuffer[4][i]; 
		kvdram[offset_kvs + i].range(191, 160) = tempbuffer[5][i]; 
		kvdram[offset_kvs + i].range(223, 192) = tempbuffer[6][i]; 
		kvdram[offset_kvs + i].range(255, 224) = tempbuffer[7][i]; 
		kvdram[offset_kvs + i].range(287, 256) = tempbuffer[8][i]; 
		kvdram[offset_kvs + i].range(319, 288) = tempbuffer[9][i]; 
		kvdram[offset_kvs + i].range(351, 320) = tempbuffer[10][i]; 
		kvdram[offset_kvs + i].range(383, 352) = tempbuffer[11][i]; 
		kvdram[offset_kvs + i].range(415, 384) = tempbuffer[12][i]; 
		kvdram[offset_kvs + i].range(447, 416) = tempbuffer[13][i]; 
		kvdram[offset_kvs + i].range(479, 448) = tempbuffer[14][i]; 
		kvdram[offset_kvs + i].range(511, 480) = tempbuffer[15][i]; 
		#else 
		kvdram[offset_kvs + i].data[0].key = tempbuffer[0][i].key;
		kvdram[offset_kvs + i].data[0].value = tempbuffer[0][i].value; 
		kvdram[offset_kvs + i].data[1].key = tempbuffer[1][i].key;
		kvdram[offset_kvs + i].data[1].value = tempbuffer[1][i].value; 
		kvdram[offset_kvs + i].data[2].key = tempbuffer[2][i].key;
		kvdram[offset_kvs + i].data[2].value = tempbuffer[2][i].value; 
		kvdram[offset_kvs + i].data[3].key = tempbuffer[3][i].key;
		kvdram[offset_kvs + i].data[3].value = tempbuffer[3][i].value; 
		kvdram[offset_kvs + i].data[4].key = tempbuffer[4][i].key;
		kvdram[offset_kvs + i].data[4].value = tempbuffer[4][i].value; 
		kvdram[offset_kvs + i].data[5].key = tempbuffer[5][i].key;
		kvdram[offset_kvs + i].data[5].value = tempbuffer[5][i].value; 
		kvdram[offset_kvs + i].data[6].key = tempbuffer[6][i].key;
		kvdram[offset_kvs + i].data[6].value = tempbuffer[6][i].value; 
		kvdram[offset_kvs + i].data[7].key = tempbuffer[7][i].key;
		kvdram[offset_kvs + i].data[7].value = tempbuffer[7][i].value; 
		#endif 
		
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvswritten(VECTOR_SIZE);
		#endif
	}
	return;
}

// functions (synchronize)
value_t 
	#ifdef SW 
	acts_synchronize::
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
	acts_synchronize::
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
	acts_synchronize::
	#endif
readandsynchronize(bool_type enable1, bool_type enable2, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3, keyvalue_vbuffer_t res[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], batch_type dramoffset_kvs, globalparams_t globalparams){
	#pragma HLS function_instantiate variable=res
	if(enable1 == OFF || enable2 == OFF){ return; }
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
	cout<<"readandsynchronize: readandsynchronize function called."<<endl;
	#endif 
	analysis_type analysis_loopcount = REDUCEBUFFERSZ;
	
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
		keyvalue0_vault2.key = mergefunc(keyvalue0_vault1.key, mykeyvalue10.key, NAp);
		keyvalue0_vault2.value = mergefunc(keyvalue0_vault1.value, mykeyvalue10.value, NAp);
		keyvalue1_vault2.key = mergefunc(keyvalue1_vault1.key, mykeyvalue11.key, NAp);
		keyvalue1_vault2.value = mergefunc(keyvalue1_vault1.value, mykeyvalue11.value, NAp);
		keyvalue2_vault2.key = mergefunc(keyvalue2_vault1.key, mykeyvalue12.key, NAp);
		keyvalue2_vault2.value = mergefunc(keyvalue2_vault1.value, mykeyvalue12.value, NAp);
		keyvalue3_vault2.key = mergefunc(keyvalue3_vault1.key, mykeyvalue13.key, NAp);
		keyvalue3_vault2.value = mergefunc(keyvalue3_vault1.value, mykeyvalue13.value, NAp);
		keyvalue4_vault2.key = mergefunc(keyvalue4_vault1.key, mykeyvalue14.key, NAp);
		keyvalue4_vault2.value = mergefunc(keyvalue4_vault1.value, mykeyvalue14.value, NAp);
		keyvalue5_vault2.key = mergefunc(keyvalue5_vault1.key, mykeyvalue15.key, NAp);
		keyvalue5_vault2.value = mergefunc(keyvalue5_vault1.value, mykeyvalue15.value, NAp);
		keyvalue6_vault2.key = mergefunc(keyvalue6_vault1.key, mykeyvalue16.key, NAp);
		keyvalue6_vault2.value = mergefunc(keyvalue6_vault1.value, mykeyvalue16.value, NAp);
		keyvalue7_vault2.key = mergefunc(keyvalue7_vault1.key, mykeyvalue17.key, NAp);
		keyvalue7_vault2.value = mergefunc(keyvalue7_vault1.value, mykeyvalue17.value, NAp);
	
		
		
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
		keyvalue0_vault3.key = mergefunc(keyvalue0_vault2.key, mykeyvalue20.key, NAp);
		keyvalue0_vault3.value = mergefunc(keyvalue0_vault2.value, mykeyvalue20.value, NAp);
		keyvalue1_vault3.key = mergefunc(keyvalue1_vault2.key, mykeyvalue21.key, NAp);
		keyvalue1_vault3.value = mergefunc(keyvalue1_vault2.value, mykeyvalue21.value, NAp);
		keyvalue2_vault3.key = mergefunc(keyvalue2_vault2.key, mykeyvalue22.key, NAp);
		keyvalue2_vault3.value = mergefunc(keyvalue2_vault2.value, mykeyvalue22.value, NAp);
		keyvalue3_vault3.key = mergefunc(keyvalue3_vault2.key, mykeyvalue23.key, NAp);
		keyvalue3_vault3.value = mergefunc(keyvalue3_vault2.value, mykeyvalue23.value, NAp);
		keyvalue4_vault3.key = mergefunc(keyvalue4_vault2.key, mykeyvalue24.key, NAp);
		keyvalue4_vault3.value = mergefunc(keyvalue4_vault2.value, mykeyvalue24.value, NAp);
		keyvalue5_vault3.key = mergefunc(keyvalue5_vault2.key, mykeyvalue25.key, NAp);
		keyvalue5_vault3.value = mergefunc(keyvalue5_vault2.value, mykeyvalue25.value, NAp);
		keyvalue6_vault3.key = mergefunc(keyvalue6_vault2.key, mykeyvalue26.key, NAp);
		keyvalue6_vault3.value = mergefunc(keyvalue6_vault2.value, mykeyvalue26.value, NAp);
		keyvalue7_vault3.key = mergefunc(keyvalue7_vault2.key, mykeyvalue27.key, NAp);
		keyvalue7_vault3.value = mergefunc(keyvalue7_vault2.value, mykeyvalue27.value, NAp);
	
		
		
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
		keyvalue0_vault4.key = mergefunc(keyvalue0_vault3.key, mykeyvalue30.key, NAp);
		keyvalue0_vault4.value = mergefunc(keyvalue0_vault3.value, mykeyvalue30.value, NAp);
		keyvalue1_vault4.key = mergefunc(keyvalue1_vault3.key, mykeyvalue31.key, NAp);
		keyvalue1_vault4.value = mergefunc(keyvalue1_vault3.value, mykeyvalue31.value, NAp);
		keyvalue2_vault4.key = mergefunc(keyvalue2_vault3.key, mykeyvalue32.key, NAp);
		keyvalue2_vault4.value = mergefunc(keyvalue2_vault3.value, mykeyvalue32.value, NAp);
		keyvalue3_vault4.key = mergefunc(keyvalue3_vault3.key, mykeyvalue33.key, NAp);
		keyvalue3_vault4.value = mergefunc(keyvalue3_vault3.value, mykeyvalue33.value, NAp);
		keyvalue4_vault4.key = mergefunc(keyvalue4_vault3.key, mykeyvalue34.key, NAp);
		keyvalue4_vault4.value = mergefunc(keyvalue4_vault3.value, mykeyvalue34.value, NAp);
		keyvalue5_vault4.key = mergefunc(keyvalue5_vault3.key, mykeyvalue35.key, NAp);
		keyvalue5_vault4.value = mergefunc(keyvalue5_vault3.value, mykeyvalue35.value, NAp);
		keyvalue6_vault4.key = mergefunc(keyvalue6_vault3.key, mykeyvalue36.key, NAp);
		keyvalue6_vault4.value = mergefunc(keyvalue6_vault3.value, mykeyvalue36.value, NAp);
		keyvalue7_vault4.key = mergefunc(keyvalue7_vault3.key, mykeyvalue37.key, NAp);
		keyvalue7_vault4.value = mergefunc(keyvalue7_vault3.value, mykeyvalue37.value, NAp);
	
		

		res[0][i] = GETKV2(keyvalue0_vault4);
		res[1][i] = GETKV2(keyvalue1_vault4);
		res[2][i] = GETKV2(keyvalue2_vault4);
		res[3][i] = GETKV2(keyvalue3_vault4);
		res[4][i] = GETKV2(keyvalue4_vault4);
		res[5][i] = GETKV2(keyvalue5_vault4);
		res[6][i] = GETKV2(keyvalue6_vault4);
		res[7][i] = GETKV2(keyvalue7_vault4);
	
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE //
		if(GETKV2(res[0][i]).key < GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(0*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(res[0][i]).value < GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(0*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKV2(res[1][i]).key < GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(1*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(res[1][i]).value < GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(1*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKV2(res[2][i]).key < GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(2*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(res[2][i]).value < GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(2*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKV2(res[3][i]).key < GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(3*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(res[3][i]).value < GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(3*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKV2(res[4][i]).key < GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(4*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(res[4][i]).value < GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(4*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKV2(res[5][i]).key < GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(5*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(res[5][i]).value < GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(5*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKV2(res[6][i]).key < GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(6*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(res[6][i]).value < GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(6*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKV2(res[7][i]).key < GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(7*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(res[7][i]).value < GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(7*REDUCESZ + 2*i + 1)<<endl; } 
		#endif
	}
	return;
}

void
	#ifdef SW 
	acts_synchronize::
	#endif
readandsynchronize(bool_type enable1, bool_type enable2, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3, keyvalue_vbuffer_t res[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], batch_type dramoffset_kvs, keyvalue_vbuffer_t refbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], globalparams_t globalparams){
	#pragma HLS function_instantiate variable=res
	if(enable1 == OFF || enable2 == OFF){ return; }
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
	cout<<"readandsynchronize: readandsynchronize function called."<<endl;
	#endif 
	analysis_type analysis_loopcount = REDUCEBUFFERSZ;
	
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
		keyvalue0_vault2.key = mergefunc(keyvalue0_vault1.key, mykeyvalue10.key, NAp);
		keyvalue0_vault2.value = mergefunc(keyvalue0_vault1.value, mykeyvalue10.value, NAp);
		keyvalue1_vault2.key = mergefunc(keyvalue1_vault1.key, mykeyvalue11.key, NAp);
		keyvalue1_vault2.value = mergefunc(keyvalue1_vault1.value, mykeyvalue11.value, NAp);
		keyvalue2_vault2.key = mergefunc(keyvalue2_vault1.key, mykeyvalue12.key, NAp);
		keyvalue2_vault2.value = mergefunc(keyvalue2_vault1.value, mykeyvalue12.value, NAp);
		keyvalue3_vault2.key = mergefunc(keyvalue3_vault1.key, mykeyvalue13.key, NAp);
		keyvalue3_vault2.value = mergefunc(keyvalue3_vault1.value, mykeyvalue13.value, NAp);
		keyvalue4_vault2.key = mergefunc(keyvalue4_vault1.key, mykeyvalue14.key, NAp);
		keyvalue4_vault2.value = mergefunc(keyvalue4_vault1.value, mykeyvalue14.value, NAp);
		keyvalue5_vault2.key = mergefunc(keyvalue5_vault1.key, mykeyvalue15.key, NAp);
		keyvalue5_vault2.value = mergefunc(keyvalue5_vault1.value, mykeyvalue15.value, NAp);
		keyvalue6_vault2.key = mergefunc(keyvalue6_vault1.key, mykeyvalue16.key, NAp);
		keyvalue6_vault2.value = mergefunc(keyvalue6_vault1.value, mykeyvalue16.value, NAp);
		keyvalue7_vault2.key = mergefunc(keyvalue7_vault1.key, mykeyvalue17.key, NAp);
		keyvalue7_vault2.value = mergefunc(keyvalue7_vault1.value, mykeyvalue17.value, NAp);
	
		
		
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
		keyvalue0_vault3.key = mergefunc(keyvalue0_vault2.key, mykeyvalue20.key, NAp);
		keyvalue0_vault3.value = mergefunc(keyvalue0_vault2.value, mykeyvalue20.value, NAp);
		keyvalue1_vault3.key = mergefunc(keyvalue1_vault2.key, mykeyvalue21.key, NAp);
		keyvalue1_vault3.value = mergefunc(keyvalue1_vault2.value, mykeyvalue21.value, NAp);
		keyvalue2_vault3.key = mergefunc(keyvalue2_vault2.key, mykeyvalue22.key, NAp);
		keyvalue2_vault3.value = mergefunc(keyvalue2_vault2.value, mykeyvalue22.value, NAp);
		keyvalue3_vault3.key = mergefunc(keyvalue3_vault2.key, mykeyvalue23.key, NAp);
		keyvalue3_vault3.value = mergefunc(keyvalue3_vault2.value, mykeyvalue23.value, NAp);
		keyvalue4_vault3.key = mergefunc(keyvalue4_vault2.key, mykeyvalue24.key, NAp);
		keyvalue4_vault3.value = mergefunc(keyvalue4_vault2.value, mykeyvalue24.value, NAp);
		keyvalue5_vault3.key = mergefunc(keyvalue5_vault2.key, mykeyvalue25.key, NAp);
		keyvalue5_vault3.value = mergefunc(keyvalue5_vault2.value, mykeyvalue25.value, NAp);
		keyvalue6_vault3.key = mergefunc(keyvalue6_vault2.key, mykeyvalue26.key, NAp);
		keyvalue6_vault3.value = mergefunc(keyvalue6_vault2.value, mykeyvalue26.value, NAp);
		keyvalue7_vault3.key = mergefunc(keyvalue7_vault2.key, mykeyvalue27.key, NAp);
		keyvalue7_vault3.value = mergefunc(keyvalue7_vault2.value, mykeyvalue27.value, NAp);
	
		
		
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
		keyvalue0_vault4.key = mergefunc(keyvalue0_vault3.key, mykeyvalue30.key, NAp);
		keyvalue0_vault4.value = mergefunc(keyvalue0_vault3.value, mykeyvalue30.value, NAp);
		keyvalue1_vault4.key = mergefunc(keyvalue1_vault3.key, mykeyvalue31.key, NAp);
		keyvalue1_vault4.value = mergefunc(keyvalue1_vault3.value, mykeyvalue31.value, NAp);
		keyvalue2_vault4.key = mergefunc(keyvalue2_vault3.key, mykeyvalue32.key, NAp);
		keyvalue2_vault4.value = mergefunc(keyvalue2_vault3.value, mykeyvalue32.value, NAp);
		keyvalue3_vault4.key = mergefunc(keyvalue3_vault3.key, mykeyvalue33.key, NAp);
		keyvalue3_vault4.value = mergefunc(keyvalue3_vault3.value, mykeyvalue33.value, NAp);
		keyvalue4_vault4.key = mergefunc(keyvalue4_vault3.key, mykeyvalue34.key, NAp);
		keyvalue4_vault4.value = mergefunc(keyvalue4_vault3.value, mykeyvalue34.value, NAp);
		keyvalue5_vault4.key = mergefunc(keyvalue5_vault3.key, mykeyvalue35.key, NAp);
		keyvalue5_vault4.value = mergefunc(keyvalue5_vault3.value, mykeyvalue35.value, NAp);
		keyvalue6_vault4.key = mergefunc(keyvalue6_vault3.key, mykeyvalue36.key, NAp);
		keyvalue6_vault4.value = mergefunc(keyvalue6_vault3.value, mykeyvalue36.value, NAp);
		keyvalue7_vault4.key = mergefunc(keyvalue7_vault3.key, mykeyvalue37.key, NAp);
		keyvalue7_vault4.value = mergefunc(keyvalue7_vault3.value, mykeyvalue37.value, NAp);
	
		

		res[0][i] = GETKV2(keyvalue0_vault4);
		res[1][i] = GETKV2(keyvalue1_vault4);
		res[2][i] = GETKV2(keyvalue2_vault4);
		res[3][i] = GETKV2(keyvalue3_vault4);
		res[4][i] = GETKV2(keyvalue4_vault4);
		res[5][i] = GETKV2(keyvalue5_vault4);
		res[6][i] = GETKV2(keyvalue6_vault4);
		res[7][i] = GETKV2(keyvalue7_vault4);
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE //
		if(GETKV2(res[0][i]).key < GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(0*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(res[0][i]).value < GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(0*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKV2(res[1][i]).key < GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(1*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(res[1][i]).value < GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(1*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKV2(res[2][i]).key < GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(2*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(res[2][i]).value < GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(2*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKV2(res[3][i]).key < GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(3*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(res[3][i]).value < GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(3*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKV2(res[4][i]).key < GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(4*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(res[4][i]).value < GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(4*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKV2(res[5][i]).key < GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(5*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(res[5][i]).value < GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(5*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKV2(res[6][i]).key < GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(6*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(res[6][i]).value < GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(6*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKV2(res[7][i]).key < GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(7*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(res[7][i]).value < GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(7*REDUCESZ + 2*i + 1)<<endl; } 
		#endif
		
		// combined
		refbuffer[0][i] = GETKV2(mykeyvalue00);
		refbuffer[1][i] = GETKV2(mykeyvalue01);
		refbuffer[2][i] = GETKV2(mykeyvalue02);
		refbuffer[3][i] = GETKV2(mykeyvalue03);
		refbuffer[4][i] = GETKV2(mykeyvalue04);
		refbuffer[5][i] = GETKV2(mykeyvalue05);
		refbuffer[6][i] = GETKV2(mykeyvalue06);
		refbuffer[7][i] = GETKV2(mykeyvalue07);
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	return;
}

uint32_type
	#ifdef SW 
	acts_synchronize::
	#endif
synchronizeandapply(bool_type enable1, bool_type enable2, keyvalue_vbuffer_t buffer0[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer3[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t res[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t refbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask[BLOCKRAM_SIZE], unsigned int colindex, batch_type voffset_kvs, globalparams_t globalparams){					
	if(enable1 == OFF || enable2 == OFF){ return 0; }
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
	cout<<"synchronizeandapply: synchronize and apply function called."<<endl;
	#endif 
	analysis_type analysis_loopcount = REDUCEBUFFERSZ;
	
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
	
	uint32_type cummvmask_sp = 0;
	
	for (buffer_type i=0; i<reducebuffersz; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE II=1
		// synchronize
		keyvalue0_vault1 = GETKV2(buffer0[0][i]); 
		keyvalue1_vault1 = GETKV2(buffer0[1][i]); 
		keyvalue2_vault1 = GETKV2(buffer0[2][i]); 
		keyvalue3_vault1 = GETKV2(buffer0[3][i]); 
		keyvalue4_vault1 = GETKV2(buffer0[4][i]); 
		keyvalue5_vault1 = GETKV2(buffer0[5][i]); 
		keyvalue6_vault1 = GETKV2(buffer0[6][i]); 
		keyvalue7_vault1 = GETKV2(buffer0[7][i]); 
			
		keyvalue0_vault2.key = mergefunc(keyvalue0_vault1.key, GETKV2(buffer1[0][i]).key, NAp);
		keyvalue0_vault2.value = mergefunc(keyvalue0_vault1.value, GETKV2(buffer1[0][i]).value, NAp);
		keyvalue1_vault2.key = mergefunc(keyvalue1_vault1.key, GETKV2(buffer1[1][i]).key, NAp);
		keyvalue1_vault2.value = mergefunc(keyvalue1_vault1.value, GETKV2(buffer1[1][i]).value, NAp);
		keyvalue2_vault2.key = mergefunc(keyvalue2_vault1.key, GETKV2(buffer1[2][i]).key, NAp);
		keyvalue2_vault2.value = mergefunc(keyvalue2_vault1.value, GETKV2(buffer1[2][i]).value, NAp);
		keyvalue3_vault2.key = mergefunc(keyvalue3_vault1.key, GETKV2(buffer1[3][i]).key, NAp);
		keyvalue3_vault2.value = mergefunc(keyvalue3_vault1.value, GETKV2(buffer1[3][i]).value, NAp);
		keyvalue4_vault2.key = mergefunc(keyvalue4_vault1.key, GETKV2(buffer1[4][i]).key, NAp);
		keyvalue4_vault2.value = mergefunc(keyvalue4_vault1.value, GETKV2(buffer1[4][i]).value, NAp);
		keyvalue5_vault2.key = mergefunc(keyvalue5_vault1.key, GETKV2(buffer1[5][i]).key, NAp);
		keyvalue5_vault2.value = mergefunc(keyvalue5_vault1.value, GETKV2(buffer1[5][i]).value, NAp);
		keyvalue6_vault2.key = mergefunc(keyvalue6_vault1.key, GETKV2(buffer1[6][i]).key, NAp);
		keyvalue6_vault2.value = mergefunc(keyvalue6_vault1.value, GETKV2(buffer1[6][i]).value, NAp);
		keyvalue7_vault2.key = mergefunc(keyvalue7_vault1.key, GETKV2(buffer1[7][i]).key, NAp);
		keyvalue7_vault2.value = mergefunc(keyvalue7_vault1.value, GETKV2(buffer1[7][i]).value, NAp);
	
		keyvalue0_vault3.key = mergefunc(keyvalue0_vault2.key, GETKV2(buffer2[0][i]).key, NAp);
		keyvalue0_vault3.value = mergefunc(keyvalue0_vault2.value, GETKV2(buffer2[0][i]).value, NAp);
		keyvalue1_vault3.key = mergefunc(keyvalue1_vault2.key, GETKV2(buffer2[1][i]).key, NAp);
		keyvalue1_vault3.value = mergefunc(keyvalue1_vault2.value, GETKV2(buffer2[1][i]).value, NAp);
		keyvalue2_vault3.key = mergefunc(keyvalue2_vault2.key, GETKV2(buffer2[2][i]).key, NAp);
		keyvalue2_vault3.value = mergefunc(keyvalue2_vault2.value, GETKV2(buffer2[2][i]).value, NAp);
		keyvalue3_vault3.key = mergefunc(keyvalue3_vault2.key, GETKV2(buffer2[3][i]).key, NAp);
		keyvalue3_vault3.value = mergefunc(keyvalue3_vault2.value, GETKV2(buffer2[3][i]).value, NAp);
		keyvalue4_vault3.key = mergefunc(keyvalue4_vault2.key, GETKV2(buffer2[4][i]).key, NAp);
		keyvalue4_vault3.value = mergefunc(keyvalue4_vault2.value, GETKV2(buffer2[4][i]).value, NAp);
		keyvalue5_vault3.key = mergefunc(keyvalue5_vault2.key, GETKV2(buffer2[5][i]).key, NAp);
		keyvalue5_vault3.value = mergefunc(keyvalue5_vault2.value, GETKV2(buffer2[5][i]).value, NAp);
		keyvalue6_vault3.key = mergefunc(keyvalue6_vault2.key, GETKV2(buffer2[6][i]).key, NAp);
		keyvalue6_vault3.value = mergefunc(keyvalue6_vault2.value, GETKV2(buffer2[6][i]).value, NAp);
		keyvalue7_vault3.key = mergefunc(keyvalue7_vault2.key, GETKV2(buffer2[7][i]).key, NAp);
		keyvalue7_vault3.value = mergefunc(keyvalue7_vault2.value, GETKV2(buffer2[7][i]).value, NAp);
	
		keyvalue0_vault4.key = mergefunc(keyvalue0_vault3.key, GETKV2(buffer3[0][i]).key, NAp);
		keyvalue0_vault4.value = mergefunc(keyvalue0_vault3.value, GETKV2(buffer3[0][i]).value, NAp);
		keyvalue1_vault4.key = mergefunc(keyvalue1_vault3.key, GETKV2(buffer3[1][i]).key, NAp);
		keyvalue1_vault4.value = mergefunc(keyvalue1_vault3.value, GETKV2(buffer3[1][i]).value, NAp);
		keyvalue2_vault4.key = mergefunc(keyvalue2_vault3.key, GETKV2(buffer3[2][i]).key, NAp);
		keyvalue2_vault4.value = mergefunc(keyvalue2_vault3.value, GETKV2(buffer3[2][i]).value, NAp);
		keyvalue3_vault4.key = mergefunc(keyvalue3_vault3.key, GETKV2(buffer3[3][i]).key, NAp);
		keyvalue3_vault4.value = mergefunc(keyvalue3_vault3.value, GETKV2(buffer3[3][i]).value, NAp);
		keyvalue4_vault4.key = mergefunc(keyvalue4_vault3.key, GETKV2(buffer3[4][i]).key, NAp);
		keyvalue4_vault4.value = mergefunc(keyvalue4_vault3.value, GETKV2(buffer3[4][i]).value, NAp);
		keyvalue5_vault4.key = mergefunc(keyvalue5_vault3.key, GETKV2(buffer3[5][i]).key, NAp);
		keyvalue5_vault4.value = mergefunc(keyvalue5_vault3.value, GETKV2(buffer3[5][i]).value, NAp);
		keyvalue6_vault4.key = mergefunc(keyvalue6_vault3.key, GETKV2(buffer3[6][i]).key, NAp);
		keyvalue6_vault4.value = mergefunc(keyvalue6_vault3.value, GETKV2(buffer3[6][i]).value, NAp);
		keyvalue7_vault4.key = mergefunc(keyvalue7_vault3.key, GETKV2(buffer3[7][i]).key, NAp);
		keyvalue7_vault4.value = mergefunc(keyvalue7_vault3.value, GETKV2(buffer3[7][i]).value, NAp);
	

		res[0][i] = GETKV2(keyvalue0_vault4);
		res[1][i] = GETKV2(keyvalue1_vault4);
		res[2][i] = GETKV2(keyvalue2_vault4);
		res[3][i] = GETKV2(keyvalue3_vault4);
		res[4][i] = GETKV2(keyvalue4_vault4);
		res[5][i] = GETKV2(keyvalue5_vault4);
		res[6][i] = GETKV2(keyvalue6_vault4);
		res[7][i] = GETKV2(keyvalue7_vault4);
		
		#ifdef _DEBUGMODE_KERNELPRINTS //
		if(GETKV2(res[0][i]).key < GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(0*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(res[0][i]).value < GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(0*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKV2(res[1][i]).key < GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(1*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(res[1][i]).value < GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(1*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKV2(res[2][i]).key < GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(2*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(res[2][i]).value < GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(2*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKV2(res[3][i]).key < GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(3*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(res[3][i]).value < GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(3*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKV2(res[4][i]).key < GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(4*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(res[4][i]).value < GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(4*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKV2(res[5][i]).key < GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(5*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(res[5][i]).value < GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(5*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKV2(res[6][i]).key < GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(6*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(res[6][i]).value < GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(6*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKV2(res[7][i]).key < GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(7*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(res[7][i]).value < GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(7*REDUCESZ + 2*i + 1)<<endl; } 
		#endif
		
		// apply
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
		#endif 
		
		keyvalue_t data0 = keyvalue0_vault4;
		keyvalue_t data1 = keyvalue1_vault4;
		keyvalue_t data2 = keyvalue2_vault4;
		keyvalue_t data3 = keyvalue3_vault4;
		keyvalue_t data4 = keyvalue4_vault4;
		keyvalue_t data5 = keyvalue5_vault4;
		keyvalue_t data6 = keyvalue6_vault4;
		keyvalue_t data7 = keyvalue7_vault4;
		keyvalue_t udata0 = GETKV2(refbuffer[0][i]);
		keyvalue_t udata1 = GETKV2(refbuffer[1][i]);
		keyvalue_t udata2 = GETKV2(refbuffer[2][i]);
		keyvalue_t udata3 = GETKV2(refbuffer[3][i]);
		keyvalue_t udata4 = GETKV2(refbuffer[4][i]);
		keyvalue_t udata5 = GETKV2(refbuffer[5][i]);
		keyvalue_t udata6 = GETKV2(refbuffer[6][i]);
		keyvalue_t udata7 = GETKV2(refbuffer[7][i]);
		
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
		
		#ifdef INCORRECTAPPLY
		if(res0.key != udata0.key || res0.key < GETK2(0xFFFFFFFF)){ vmask[i].data[colindex + 0].key = 1; } else { vmask[i].data[colindex + 0].key = 0; }
		if(res0.value != udata0.value || res0.value < GETV2(0xFFFFFFFF)){ vmask[i].data[colindex + 0].value = 1; } else { vmask[i].data[colindex + 0].value = 0; }
		if(res1.key != udata1.key || res1.key < GETK2(0xFFFFFFFF)){ vmask[i].data[colindex + 1].key = 1; } else { vmask[i].data[colindex + 1].key = 0; }
		if(res1.value != udata1.value || res1.value < GETV2(0xFFFFFFFF)){ vmask[i].data[colindex + 1].value = 1; } else { vmask[i].data[colindex + 1].value = 0; }
		if(res2.key != udata2.key || res2.key < GETK2(0xFFFFFFFF)){ vmask[i].data[colindex + 2].key = 1; } else { vmask[i].data[colindex + 2].key = 0; }
		if(res2.value != udata2.value || res2.value < GETV2(0xFFFFFFFF)){ vmask[i].data[colindex + 2].value = 1; } else { vmask[i].data[colindex + 2].value = 0; }
		if(res3.key != udata3.key || res3.key < GETK2(0xFFFFFFFF)){ vmask[i].data[colindex + 3].key = 1; } else { vmask[i].data[colindex + 3].key = 0; }
		if(res3.value != udata3.value || res3.value < GETV2(0xFFFFFFFF)){ vmask[i].data[colindex + 3].value = 1; } else { vmask[i].data[colindex + 3].value = 0; }
		if(res4.key != udata4.key || res4.key < GETK2(0xFFFFFFFF)){ vmask[i].data[colindex + 4].key = 1; } else { vmask[i].data[colindex + 4].key = 0; }
		if(res4.value != udata4.value || res4.value < GETV2(0xFFFFFFFF)){ vmask[i].data[colindex + 4].value = 1; } else { vmask[i].data[colindex + 4].value = 0; }
		if(res5.key != udata5.key || res5.key < GETK2(0xFFFFFFFF)){ vmask[i].data[colindex + 5].key = 1; } else { vmask[i].data[colindex + 5].key = 0; }
		if(res5.value != udata5.value || res5.value < GETV2(0xFFFFFFFF)){ vmask[i].data[colindex + 5].value = 1; } else { vmask[i].data[colindex + 5].value = 0; }
		if(res6.key != udata6.key || res6.key < GETK2(0xFFFFFFFF)){ vmask[i].data[colindex + 6].key = 1; } else { vmask[i].data[colindex + 6].key = 0; }
		if(res6.value != udata6.value || res6.value < GETV2(0xFFFFFFFF)){ vmask[i].data[colindex + 6].value = 1; } else { vmask[i].data[colindex + 6].value = 0; }
		if(res7.key != udata7.key || res7.key < GETK2(0xFFFFFFFF)){ vmask[i].data[colindex + 7].key = 1; } else { vmask[i].data[colindex + 7].key = 0; }
		if(res7.value != udata7.value || res7.value < GETV2(0xFFFFFFFF)){ vmask[i].data[colindex + 7].value = 1; } else { vmask[i].data[colindex + 7].value = 0; }
	
		#else
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
		#endif 
		
		uint32_type mask = convertvmasktouint32(vmask, i);
		cummvmask_sp = cummvmask_sp | mask;
		
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
		if(false){ cout<<"APPLY FUNC SEEN @ (vid1: "<<vid[0][0]<<" & vid2: "<<vid[0][1]<<"): res0.key: "<<res0.key<<", res0.value: "<<res0.value<<": udata0.key: "<<udata0.key<<", udata0.value: "<<udata0.value<<", data0.key: "<<data0.key<<", data0.value: "<<data0.value<<endl; }
		if(res0.key != udata0.key || res0.key < GETK2(0xFFFFFFFF)){ cout<<"apply: vid[0][0]: "<<vid[0][0]<<", vmask["<<i<<"].data["<<colindex + 0<<"].key: "<<vmask[i].data[colindex + 0].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res0.value != udata0.value || res0.value < GETV2(0xFFFFFFFF)){ cout<<"apply: vid[0][1]: "<<vid[0][1]<<", vmask["<<i<<"].data["<<colindex + 0<<"].value: "<<vmask[i].data[colindex + 0].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(false){ cout<<"APPLY FUNC SEEN @ (vid1: "<<vid[1][0]<<" & vid2: "<<vid[1][1]<<"): res1.key: "<<res1.key<<", res1.value: "<<res1.value<<": udata1.key: "<<udata1.key<<", udata1.value: "<<udata1.value<<", data1.key: "<<data1.key<<", data1.value: "<<data1.value<<endl; }
		if(res1.key != udata1.key || res1.key < GETK2(0xFFFFFFFF)){ cout<<"apply: vid[1][0]: "<<vid[1][0]<<", vmask["<<i<<"].data["<<colindex + 1<<"].key: "<<vmask[i].data[colindex + 1].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res1.value != udata1.value || res1.value < GETV2(0xFFFFFFFF)){ cout<<"apply: vid[1][1]: "<<vid[1][1]<<", vmask["<<i<<"].data["<<colindex + 1<<"].value: "<<vmask[i].data[colindex + 1].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(false){ cout<<"APPLY FUNC SEEN @ (vid1: "<<vid[2][0]<<" & vid2: "<<vid[2][1]<<"): res2.key: "<<res2.key<<", res2.value: "<<res2.value<<": udata2.key: "<<udata2.key<<", udata2.value: "<<udata2.value<<", data2.key: "<<data2.key<<", data2.value: "<<data2.value<<endl; }
		if(res2.key != udata2.key || res2.key < GETK2(0xFFFFFFFF)){ cout<<"apply: vid[2][0]: "<<vid[2][0]<<", vmask["<<i<<"].data["<<colindex + 2<<"].key: "<<vmask[i].data[colindex + 2].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res2.value != udata2.value || res2.value < GETV2(0xFFFFFFFF)){ cout<<"apply: vid[2][1]: "<<vid[2][1]<<", vmask["<<i<<"].data["<<colindex + 2<<"].value: "<<vmask[i].data[colindex + 2].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(false){ cout<<"APPLY FUNC SEEN @ (vid1: "<<vid[3][0]<<" & vid2: "<<vid[3][1]<<"): res3.key: "<<res3.key<<", res3.value: "<<res3.value<<": udata3.key: "<<udata3.key<<", udata3.value: "<<udata3.value<<", data3.key: "<<data3.key<<", data3.value: "<<data3.value<<endl; }
		if(res3.key != udata3.key || res3.key < GETK2(0xFFFFFFFF)){ cout<<"apply: vid[3][0]: "<<vid[3][0]<<", vmask["<<i<<"].data["<<colindex + 3<<"].key: "<<vmask[i].data[colindex + 3].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res3.value != udata3.value || res3.value < GETV2(0xFFFFFFFF)){ cout<<"apply: vid[3][1]: "<<vid[3][1]<<", vmask["<<i<<"].data["<<colindex + 3<<"].value: "<<vmask[i].data[colindex + 3].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(false){ cout<<"APPLY FUNC SEEN @ (vid1: "<<vid[4][0]<<" & vid2: "<<vid[4][1]<<"): res4.key: "<<res4.key<<", res4.value: "<<res4.value<<": udata4.key: "<<udata4.key<<", udata4.value: "<<udata4.value<<", data4.key: "<<data4.key<<", data4.value: "<<data4.value<<endl; }
		if(res4.key != udata4.key || res4.key < GETK2(0xFFFFFFFF)){ cout<<"apply: vid[4][0]: "<<vid[4][0]<<", vmask["<<i<<"].data["<<colindex + 4<<"].key: "<<vmask[i].data[colindex + 4].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res4.value != udata4.value || res4.value < GETV2(0xFFFFFFFF)){ cout<<"apply: vid[4][1]: "<<vid[4][1]<<", vmask["<<i<<"].data["<<colindex + 4<<"].value: "<<vmask[i].data[colindex + 4].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(false){ cout<<"APPLY FUNC SEEN @ (vid1: "<<vid[5][0]<<" & vid2: "<<vid[5][1]<<"): res5.key: "<<res5.key<<", res5.value: "<<res5.value<<": udata5.key: "<<udata5.key<<", udata5.value: "<<udata5.value<<", data5.key: "<<data5.key<<", data5.value: "<<data5.value<<endl; }
		if(res5.key != udata5.key || res5.key < GETK2(0xFFFFFFFF)){ cout<<"apply: vid[5][0]: "<<vid[5][0]<<", vmask["<<i<<"].data["<<colindex + 5<<"].key: "<<vmask[i].data[colindex + 5].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res5.value != udata5.value || res5.value < GETV2(0xFFFFFFFF)){ cout<<"apply: vid[5][1]: "<<vid[5][1]<<", vmask["<<i<<"].data["<<colindex + 5<<"].value: "<<vmask[i].data[colindex + 5].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(false){ cout<<"APPLY FUNC SEEN @ (vid1: "<<vid[6][0]<<" & vid2: "<<vid[6][1]<<"): res6.key: "<<res6.key<<", res6.value: "<<res6.value<<": udata6.key: "<<udata6.key<<", udata6.value: "<<udata6.value<<", data6.key: "<<data6.key<<", data6.value: "<<data6.value<<endl; }
		if(res6.key != udata6.key || res6.key < GETK2(0xFFFFFFFF)){ cout<<"apply: vid[6][0]: "<<vid[6][0]<<", vmask["<<i<<"].data["<<colindex + 6<<"].key: "<<vmask[i].data[colindex + 6].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res6.value != udata6.value || res6.value < GETV2(0xFFFFFFFF)){ cout<<"apply: vid[6][1]: "<<vid[6][1]<<", vmask["<<i<<"].data["<<colindex + 6<<"].value: "<<vmask[i].data[colindex + 6].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(false){ cout<<"APPLY FUNC SEEN @ (vid1: "<<vid[7][0]<<" & vid2: "<<vid[7][1]<<"): res7.key: "<<res7.key<<", res7.value: "<<res7.value<<": udata7.key: "<<udata7.key<<", udata7.value: "<<udata7.value<<", data7.key: "<<data7.key<<", data7.value: "<<data7.value<<endl; }
		if(res7.key != udata7.key || res7.key < GETK2(0xFFFFFFFF)){ cout<<"apply: vid[7][0]: "<<vid[7][0]<<", vmask["<<i<<"].data["<<colindex + 7<<"].key: "<<vmask[i].data[colindex + 7].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res7.value != udata7.value || res7.value < GETV2(0xFFFFFFFF)){ cout<<"apply: vid[7][1]: "<<vid[7][1]<<", vmask["<<i<<"].data["<<colindex + 7<<"].value: "<<vmask[i].data[colindex + 7].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		#endif
		#ifdef _DEBUGMODE_STATS
		#ifdef INCORRECTAPPLY
		if(res0.key != udata0.key || res0.key < GETK2(0xFFFFFFFF)){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res0.value != udata0.value || res0.key < GETV2(0xFFFFFFFF)){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res1.key != udata1.key || res1.key < GETK2(0xFFFFFFFF)){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res1.value != udata1.value || res1.key < GETV2(0xFFFFFFFF)){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res2.key != udata2.key || res2.key < GETK2(0xFFFFFFFF)){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res2.value != udata2.value || res2.key < GETV2(0xFFFFFFFF)){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res3.key != udata3.key || res3.key < GETK2(0xFFFFFFFF)){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res3.value != udata3.value || res3.key < GETV2(0xFFFFFFFF)){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res4.key != udata4.key || res4.key < GETK2(0xFFFFFFFF)){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res4.value != udata4.value || res4.key < GETV2(0xFFFFFFFF)){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res5.key != udata5.key || res5.key < GETK2(0xFFFFFFFF)){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res5.value != udata5.value || res5.key < GETV2(0xFFFFFFFF)){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res6.key != udata6.key || res6.key < GETK2(0xFFFFFFFF)){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res6.value != udata6.value || res6.key < GETV2(0xFFFFFFFF)){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res7.key != udata7.key || res7.key < GETK2(0xFFFFFFFF)){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res7.value != udata7.value || res7.key < GETV2(0xFFFFFFFF)){ actsutilityobj->globalstats_countactvvsseen(1); }
		#else 
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
		#endif
	}
	return cummvmask_sp;
}

void
	#ifdef SW 
	acts_synchronize::
	#endif
spreadvdata(bool_type enable1, bool_type enable2, keyvalue_vbuffer_t source[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t buffer0[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer3[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], globalparams_t globalparams){
	#pragma HLS function_instantiate variable=source
	if(enable1 == OFF || enable2 == OFF){ return; }
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
	cout<<"spreadvdata: spread (vdata) function called."<<endl;
	#endif 
	analysis_type analysis_loopcount = REDUCEBUFFERSZ;
	
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
	
	
	for (buffer_type i=0; i<reducebuffersz; i++){ // 8, 16, BLOCKRAM_SIZE
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE II=1
		keyvalue0_vault0 = GETKV2(source[0][i]); 
		keyvalue1_vault0 = GETKV2(source[1][i]); 
		keyvalue2_vault0 = GETKV2(source[2][i]); 
		keyvalue3_vault0 = GETKV2(source[3][i]); 
		keyvalue4_vault0 = GETKV2(source[4][i]); 
		keyvalue5_vault0 = GETKV2(source[5][i]); 
		keyvalue6_vault0 = GETKV2(source[6][i]); 
		keyvalue7_vault0 = GETKV2(source[7][i]); 
			
		buffer0[0][i] = GETKV2(keyvalue0_vault0);
		keyvalue0_vault1 = keyvalue0_vault0;
		buffer0[1][i] = GETKV2(keyvalue1_vault0);
		keyvalue1_vault1 = keyvalue1_vault0;
		buffer0[2][i] = GETKV2(keyvalue2_vault0);
		keyvalue2_vault1 = keyvalue2_vault0;
		buffer0[3][i] = GETKV2(keyvalue3_vault0);
		keyvalue3_vault1 = keyvalue3_vault0;
		buffer0[4][i] = GETKV2(keyvalue4_vault0);
		keyvalue4_vault1 = keyvalue4_vault0;
		buffer0[5][i] = GETKV2(keyvalue5_vault0);
		keyvalue5_vault1 = keyvalue5_vault0;
		buffer0[6][i] = GETKV2(keyvalue6_vault0);
		keyvalue6_vault1 = keyvalue6_vault0;
		buffer0[7][i] = GETKV2(keyvalue7_vault0);
		keyvalue7_vault1 = keyvalue7_vault0;
	
		buffer1[0][i] = GETKV2(keyvalue0_vault1);
		keyvalue0_vault2 = keyvalue0_vault1;
		buffer1[1][i] = GETKV2(keyvalue1_vault1);
		keyvalue1_vault2 = keyvalue1_vault1;
		buffer1[2][i] = GETKV2(keyvalue2_vault1);
		keyvalue2_vault2 = keyvalue2_vault1;
		buffer1[3][i] = GETKV2(keyvalue3_vault1);
		keyvalue3_vault2 = keyvalue3_vault1;
		buffer1[4][i] = GETKV2(keyvalue4_vault1);
		keyvalue4_vault2 = keyvalue4_vault1;
		buffer1[5][i] = GETKV2(keyvalue5_vault1);
		keyvalue5_vault2 = keyvalue5_vault1;
		buffer1[6][i] = GETKV2(keyvalue6_vault1);
		keyvalue6_vault2 = keyvalue6_vault1;
		buffer1[7][i] = GETKV2(keyvalue7_vault1);
		keyvalue7_vault2 = keyvalue7_vault1;
	
		buffer2[0][i] = GETKV2(keyvalue0_vault2);
		keyvalue0_vault3 = keyvalue0_vault2;
		buffer2[1][i] = GETKV2(keyvalue1_vault2);
		keyvalue1_vault3 = keyvalue1_vault2;
		buffer2[2][i] = GETKV2(keyvalue2_vault2);
		keyvalue2_vault3 = keyvalue2_vault2;
		buffer2[3][i] = GETKV2(keyvalue3_vault2);
		keyvalue3_vault3 = keyvalue3_vault2;
		buffer2[4][i] = GETKV2(keyvalue4_vault2);
		keyvalue4_vault3 = keyvalue4_vault2;
		buffer2[5][i] = GETKV2(keyvalue5_vault2);
		keyvalue5_vault3 = keyvalue5_vault2;
		buffer2[6][i] = GETKV2(keyvalue6_vault2);
		keyvalue6_vault3 = keyvalue6_vault2;
		buffer2[7][i] = GETKV2(keyvalue7_vault2);
		keyvalue7_vault3 = keyvalue7_vault2;
	
		buffer3[0][i] = GETKV2(keyvalue0_vault3);
		keyvalue0_vault4 = keyvalue0_vault3;
		buffer3[1][i] = GETKV2(keyvalue1_vault3);
		keyvalue1_vault4 = keyvalue1_vault3;
		buffer3[2][i] = GETKV2(keyvalue2_vault3);
		keyvalue2_vault4 = keyvalue2_vault3;
		buffer3[3][i] = GETKV2(keyvalue3_vault3);
		keyvalue3_vault4 = keyvalue3_vault3;
		buffer3[4][i] = GETKV2(keyvalue4_vault3);
		keyvalue4_vault4 = keyvalue4_vault3;
		buffer3[5][i] = GETKV2(keyvalue5_vault3);
		keyvalue5_vault4 = keyvalue5_vault3;
		buffer3[6][i] = GETKV2(keyvalue6_vault3);
		keyvalue6_vault4 = keyvalue6_vault3;
		buffer3[7][i] = GETKV2(keyvalue7_vault3);
		keyvalue7_vault4 = keyvalue7_vault3;
	
		
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE //
		if(GETKV2(buffer3[0][i]).key < GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(0*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(buffer3[0][i]).value < GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(0*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKV2(buffer3[1][i]).key < GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(1*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(buffer3[1][i]).value < GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(1*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKV2(buffer3[2][i]).key < GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(2*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(buffer3[2][i]).value < GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(2*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKV2(buffer3[3][i]).key < GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(3*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(buffer3[3][i]).value < GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(3*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKV2(buffer3[4][i]).key < GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(4*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(buffer3[4][i]).value < GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(4*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKV2(buffer3[5][i]).key < GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(5*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(buffer3[5][i]).value < GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(5*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKV2(buffer3[6][i]).key < GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(6*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(buffer3[6][i]).value < GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(6*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKV2(buffer3[7][i]).key < GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(7*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(buffer3[7][i]).value < GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(7*REDUCESZ + 2*i + 1)<<endl; } 
		#endif
	}
	return;
}

void
	#ifdef SW 
	acts_synchronize::
	#endif
spreadvmask(bool_type enable1, bool_type enable2, bool_type enable3, uintNUMPby2_type vmask[BLOCKRAM_SIZE], uintNUMPby2_type vmask0[BLOCKRAM_SIZE],uintNUMPby2_type vmask1[BLOCKRAM_SIZE],uintNUMPby2_type vmask2[BLOCKRAM_SIZE],uintNUMPby2_type vmask3[BLOCKRAM_SIZE], globalparams_t globalparams){
	#pragma HLS INLINE OFF //
	if(enable1 == OFF || enable2 == OFF || enable3 == OFF){ return; }
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
	
	}
	return;
}

void
	#ifdef SW 
	acts_synchronize::
	#endif
spreadandwrite(bool_type enable1, bool_type enable2, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,		keyvalue_vbuffer_t source[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], batch_type vbuffer_offset_kvs, buffer_type vbuffer_size_kvs, 
		uintNUMPby2_type vmask[BLOCKRAM_SIZE], batch_type vmask_offset_kvs, buffer_type vmask_size_kvs, 
		unsigned int vmaskp_offset_kvs, uint32_type vmask_p_temp[2],
		globalparams_t globalparams){
	#pragma HLS function_instantiate variable=source
	if(enable1 == OFF || enable2 == OFF){ return; }
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
	cout<<"spreadvdata: spread (vdata) function called."<<endl;
	#endif 
	analysis_type analysis_loopcount = REDUCEBUFFERSZ;
	analysis_type analysis_loopcount2 = BLOCKRAM_SIZE;
	analysis_type analysis_loopcount3 = BLOCKRAM_SIZE / 16;
	
	buffer_type reducebuffersz = globalparams.SIZE_REDUCE / 2;
	
	uint32_type tempbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE]; // prepare vmask variables
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
	
	
	SPREADANDSAVE_COMBINEDWITH_PREPAREVMASKS_LOOP: for (buffer_type i=0; i<reducebuffersz; i++){ // 8, 16, BLOCKRAM_SIZE
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE II=1
		// spread operation
		keyvalue0_vault0 = GETKV2(source[0][i]); 
		keyvalue1_vault0 = GETKV2(source[1][i]); 
		keyvalue2_vault0 = GETKV2(source[2][i]); 
		keyvalue3_vault0 = GETKV2(source[3][i]); 
		keyvalue4_vault0 = GETKV2(source[4][i]); 
		keyvalue5_vault0 = GETKV2(source[5][i]); 
		keyvalue6_vault0 = GETKV2(source[6][i]); 
		keyvalue7_vault0 = GETKV2(source[7][i]); 
		
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
		
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE //
		if(GETKEYENTRY(kvdram3[vbuffer_offset_kvs + i], 0) < GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(0*REDUCESZ + 2*i)<<endl; } 
		if(GETVALUEENTRY(kvdram3[vbuffer_offset_kvs + i], 0) < GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(0*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKEYENTRY(kvdram3[vbuffer_offset_kvs + i], 1) < GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(1*REDUCESZ + 2*i)<<endl; } 
		if(GETVALUEENTRY(kvdram3[vbuffer_offset_kvs + i], 1) < GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(1*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKEYENTRY(kvdram3[vbuffer_offset_kvs + i], 2) < GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(2*REDUCESZ + 2*i)<<endl; } 
		if(GETVALUEENTRY(kvdram3[vbuffer_offset_kvs + i], 2) < GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(2*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKEYENTRY(kvdram3[vbuffer_offset_kvs + i], 3) < GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(3*REDUCESZ + 2*i)<<endl; } 
		if(GETVALUEENTRY(kvdram3[vbuffer_offset_kvs + i], 3) < GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(3*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKEYENTRY(kvdram3[vbuffer_offset_kvs + i], 4) < GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(4*REDUCESZ + 2*i)<<endl; } 
		if(GETVALUEENTRY(kvdram3[vbuffer_offset_kvs + i], 4) < GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(4*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKEYENTRY(kvdram3[vbuffer_offset_kvs + i], 5) < GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(5*REDUCESZ + 2*i)<<endl; } 
		if(GETVALUEENTRY(kvdram3[vbuffer_offset_kvs + i], 5) < GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(5*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKEYENTRY(kvdram3[vbuffer_offset_kvs + i], 6) < GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(6*REDUCESZ + 2*i)<<endl; } 
		if(GETVALUEENTRY(kvdram3[vbuffer_offset_kvs + i], 6) < GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(6*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKEYENTRY(kvdram3[vbuffer_offset_kvs + i], 7) < GETK2(0xFFFFFFFF)){ cout<<"actvvid: "<<(7*REDUCESZ + 2*i)<<endl; } 
		if(GETVALUEENTRY(kvdram3[vbuffer_offset_kvs + i], 7) < GETV2(0xFFFFFFFF)){ cout<<"actvvid: "<<(7*REDUCESZ + 2*i + 1)<<endl; } 
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
		WRITETO_UINT(&tempbuffer[X][Y], 0, 1, vmask[i].data[0].key);
		WRITETO_UINT(&tempbuffer[X][Y], 1, 1, vmask[i].data[0].value);
		WRITETO_UINT(&tempbuffer[X][Y], 2, 1, vmask[i].data[1].key);
		WRITETO_UINT(&tempbuffer[X][Y], 3, 1, vmask[i].data[1].value);
		WRITETO_UINT(&tempbuffer[X][Y], 4, 1, vmask[i].data[2].key);
		WRITETO_UINT(&tempbuffer[X][Y], 5, 1, vmask[i].data[2].value);
		WRITETO_UINT(&tempbuffer[X][Y], 6, 1, vmask[i].data[3].key);
		WRITETO_UINT(&tempbuffer[X][Y], 7, 1, vmask[i].data[3].value);
		WRITETO_UINT(&tempbuffer[X][Y], 8, 1, vmask[i].data[4].key);
		WRITETO_UINT(&tempbuffer[X][Y], 9, 1, vmask[i].data[4].value);
		WRITETO_UINT(&tempbuffer[X][Y], 10, 1, vmask[i].data[5].key);
		WRITETO_UINT(&tempbuffer[X][Y], 11, 1, vmask[i].data[5].value);
		WRITETO_UINT(&tempbuffer[X][Y], 12, 1, vmask[i].data[6].key);
		WRITETO_UINT(&tempbuffer[X][Y], 13, 1, vmask[i].data[6].value);
		WRITETO_UINT(&tempbuffer[X][Y], 14, 1, vmask[i].data[7].key);
		WRITETO_UINT(&tempbuffer[X][Y], 15, 1, vmask[i].data[7].value);
		WRITETO_UINT(&tempbuffer[X][Y], 16, 1, vmask[i].data[8].key);
		WRITETO_UINT(&tempbuffer[X][Y], 17, 1, vmask[i].data[8].value);
		WRITETO_UINT(&tempbuffer[X][Y], 18, 1, vmask[i].data[9].key);
		WRITETO_UINT(&tempbuffer[X][Y], 19, 1, vmask[i].data[9].value);
		WRITETO_UINT(&tempbuffer[X][Y], 20, 1, vmask[i].data[10].key);
		WRITETO_UINT(&tempbuffer[X][Y], 21, 1, vmask[i].data[10].value);
		WRITETO_UINT(&tempbuffer[X][Y], 22, 1, vmask[i].data[11].key);
		WRITETO_UINT(&tempbuffer[X][Y], 23, 1, vmask[i].data[11].value);
		WRITETO_UINT(&tempbuffer[X][Y], 24, 1, vmask[i].data[12].key);
		WRITETO_UINT(&tempbuffer[X][Y], 25, 1, vmask[i].data[12].value);
		WRITETO_UINT(&tempbuffer[X][Y], 26, 1, vmask[i].data[13].key);
		WRITETO_UINT(&tempbuffer[X][Y], 27, 1, vmask[i].data[13].value);
		WRITETO_UINT(&tempbuffer[X][Y], 28, 1, vmask[i].data[14].key);
		WRITETO_UINT(&tempbuffer[X][Y], 29, 1, vmask[i].data[14].value);
		WRITETO_UINT(&tempbuffer[X][Y], 30, 1, vmask[i].data[15].key);
		WRITETO_UINT(&tempbuffer[X][Y], 31, 1, vmask[i].data[15].value);
		#endif
	}
	
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
	
	// savevmaskp
	uint32_type vmask_p = vmask_p_temp[0] | vmask_p_temp[1];
	#ifdef _WIDEWORD
	kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vmaskp_offset_kvs].range(31, 0) = vmask_p;
	kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vmaskp_offset_kvs].range(31, 0) = vmask_p;
	kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vmaskp_offset_kvs].range(31, 0) = vmask_p;
	kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vmaskp_offset_kvs].range(31, 0) = vmask_p;
	#else
	kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vmaskp_offset_kvs].data[0].key = vmask_p;
	kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vmaskp_offset_kvs].data[0].key = vmask_p;
	kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vmaskp_offset_kvs].data[0].key = vmask_p;
	kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vmaskp_offset_kvs].data[0].key = vmask_p;
	#endif 
	return;
}

// top
extern "C" {
void
	#ifdef SW 
	acts_synchronize:: 
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

	#ifdef _DEBUGMODE_KERNELPRINTS3
	// #ifdef _WIDEWORD
	// cout<<">>> Light weight ACTS (NACTS_IN_NCOMPUTEUNITS.SYNC) Launched... size: "<<(unsigned int)(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN].range(31, 0))<<endl; 
	// #else
	// cout<<">>> Light weight ACTS (NACTS_IN_NCOMPUTEUNITS.SYNC) Launched... size: "<<kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN].data[0].key<<endl; 
	// #endif
	cout<<">>> Light weight ACTS (NACTS_IN_NCOMPUTEUNITS.SYNC) Launched... size: "<<GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN], 0)<<endl; 
	#endif
	
	keyvalue_vbuffer_t refbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = refbuffer
	
	travstate_t rtravstate[NUMCOMPUTEUNITS];
	#pragma HLS ARRAY_PARTITION variable=rtravstate complete
	
	//
	keyvalue_vbuffer_t vbuffer0_level1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer0_level1
	keyvalue_vbuffer_t vbuffer1_level1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer1_level1
	keyvalue_vbuffer_t vbuffer2_level1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer2_level1
	keyvalue_vbuffer_t vbuffer3_level1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer3_level1
	
	keyvalue_vbuffer_t vbuffer0_level2[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer0_level2
	
	keyvalue_vbuffer_t vbuffer0_level3[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer0_level3
	keyvalue_vbuffer_t vbuffer1_level3[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer1_level3
	keyvalue_vbuffer_t vbuffer2_level3[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer2_level3
	keyvalue_vbuffer_t vbuffer3_level3[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer3_level3

	//
	uintNUMPby2_type vmask0_level2[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask0_level2
	
	uintNUMPby2_type vmask0_level3[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask0_level3
	uintNUMPby2_type vmask1_level3[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask1_level3
	uintNUMPby2_type vmask2_level3[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask2_level3
	uintNUMPby2_type vmask3_level3[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask3_level3
	
	//
	uint32_type vmaskptemp0_level2[2];
	
	uint32_type vmaskptemp0_level3[2];
	uint32_type vmaskptemp1_level3[2];
	uint32_type vmaskptemp2_level3[2];
	uint32_type vmaskptemp3_level3[2];
	
	//
	globalparams_t globalparams[NUMCOMPUTEUNITS];
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
	
	buffer_type reducebuffersz = _globalparams.SIZE_REDUCE / 2; // 512
	buffer_type vmaskbuffersz_kvs = (_globalparams.SIZE_REDUCE * NUM_PARTITIONS) / 512; // 32
	
	unsigned int vreadoffset_kvs = 0;
	unsigned int vreadoffsetpp0_kvs = 0;
	unsigned int vreadoffsetpp1_kvs = vreadoffsetpp0_kvs + reducebuffersz;
	unsigned int vmaskreadoffset_kvs = 0;
	
	step_type currentLOP = _globalparams.ACTSPARAMS_TREEDEPTH;
	batch_type num_source_partitions = get_num_source_partitions(currentLOP);

	for(unsigned int k=0; k<num_source_partitions; k++){ vmask_p[k] = 0; }
	
	unsigned int total_num_iterations = num_source_partitions*2;
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
	
	TOPKERNELSYNC_MAINLOOP: for(batch_type iterationidx=0; iterationidx<total_num_iterations; iterationidx+=NUMSYNCPIPELINES){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loop1 avg=analysis_loop1
	
		enablereduce = ON;
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
	
		#ifdef SUP1
		if(iterationidx > 0){ pp1en_spreadvdata = ON; pp1en_spreadvmask = ON; pp1en_spreadandwrite = ON; } 
		else { pp1en_spreadvdata = OFF; pp1en_spreadvmask = OFF; pp1en_spreadandwrite = OFF; }
		#endif
	
		source_partition = iterationidx / 2;
		#ifdef _DEBUGMODE_KERNELPRINTS
		actsutilityobj->print3("### topkernelsync:: source_partition", "currentLOP", "NAp", source_partition, currentLOP, NAp); 							
		#endif
		
		// R_and_I1
		enablereducepp0 = enablereduce;
		readandsynchronize(enablereduce, ON, kvdram0, kvdram1, kvdram2, kvdram3, vbuffer0_level1, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffsetpp0_kvs, refbuffer, _globalparams);
		readandsynchronize(enablereduce, ON, kvdram4, kvdram5, kvdram6, kvdram7, vbuffer1_level1, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffsetpp0_kvs, _globalparams);
		readandsynchronize(enablereduce, ON, kvdram8, kvdram9, kvdram10, kvdram11, vbuffer2_level1, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffsetpp0_kvs, _globalparams);
		readandsynchronize(enablereduce, ON, kvdram12, kvdram13, kvdram14, kvdram15, vbuffer3_level1, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffsetpp0_kvs, _globalparams);
		#ifdef SUP1 // I3 - pp1
		spreadvdata(enablereducepp1, pp1en_spreadvdata, vbuffer0_level2, vbuffer0_level3, vbuffer1_level3, vbuffer2_level3, vbuffer3_level3, _globalparams);
		spreadvmask(enablereducepp1, pp1en_spreadvmask, ON, vmask0_level2, vmask0_level3, vmask1_level3, vmask2_level3, vmask3_level3, _globalparams); 
		if(enablereducepp1 == ON && pp1en_spreadvmask == ON){ 
			vmaskptemp0_level3[0] = vmaskptemp0_level2[0]; vmaskptemp0_level3[1] = vmaskptemp0_level2[1]; 
			vmaskptemp1_level3[0] = vmaskptemp0_level2[0]; vmaskptemp1_level3[1] = vmaskptemp0_level2[1]; 
			vmaskptemp2_level3[0] = vmaskptemp0_level2[0]; vmaskptemp2_level3[1] = vmaskptemp0_level2[1]; 
			vmaskptemp3_level3[0] = vmaskptemp0_level2[0]; vmaskptemp3_level3[1] = vmaskptemp0_level2[1]; 
		}
		#endif
		
		// I2
		if(SWITCHcount % 2 == 0){ begincol_vmask = 0; } else { begincol_vmask = 8; };
		vmaskptemp0_level2[SWITCHcount % 2] = synchronizeandapply(enablereduce, ON, vbuffer0_level1, vbuffer1_level1, vbuffer2_level1, vbuffer3_level1, vbuffer0_level2, refbuffer, vmask0_level2, begincol_vmask, vreadoffsetpp0_kvs, _globalparams);
		SWITCHcount += 1;
		#ifdef SUP1 // S_and_I4
		spreadandwrite(enablereducepp1, pp1en_spreadandwrite, kvdram0, kvdram1, kvdram2, kvdram3, 
			vbuffer0_level3, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffsetpp1_kvs, reducebuffersz,
			vmask0_level3, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs,
			source_partition, vmaskptemp0_level3,
			_globalparams);
		spreadandwrite(enablereducepp1, pp1en_spreadandwrite, kvdram4, kvdram5, kvdram6, kvdram7, 
			vbuffer1_level3, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffsetpp1_kvs, reducebuffersz,
			vmask1_level3, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs,
			source_partition, vmaskptemp1_level3,
			_globalparams);
		spreadandwrite(enablereducepp1, pp1en_spreadandwrite, kvdram8, kvdram9, kvdram10, kvdram11, 
			vbuffer2_level3, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffsetpp1_kvs, reducebuffersz,
			vmask2_level3, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs,
			source_partition, vmaskptemp2_level3,
			_globalparams);
		spreadandwrite(enablereducepp1, pp1en_spreadandwrite, kvdram12, kvdram13, kvdram14, kvdram15, 
			vbuffer3_level3, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffsetpp1_kvs, reducebuffersz,
			vmask3_level3, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs,
			source_partition, vmaskptemp3_level3,
			_globalparams);
		if(pp1en_spreadandwrite == ON){ vreadoffsetpp1_kvs += reducebuffersz * NUMSYNCPIPELINES; }
		if(pp1en_spreadandwrite == ON){ MOVEcount += 1; if(MOVEcount % 2 == 0){ vmaskreadoffset_kvs += vmaskbuffersz_kvs; sourcestatsmarker += 1; }} //+++
		#endif
		
		// I3
		spreadvdata(enablereduce, ON, vbuffer0_level2, vbuffer0_level3, vbuffer1_level3, vbuffer2_level3, vbuffer3_level3, _globalparams);
		spreadvmask(enablereduce, ON, ON, vmask0_level2, vmask0_level3, vmask1_level3, vmask2_level3, vmask3_level3, _globalparams); 
		if(enablereduce == ON){ 
			vmaskptemp0_level3[0] = vmaskptemp0_level2[0]; vmaskptemp0_level3[1] = vmaskptemp0_level2[1]; 
			vmaskptemp1_level3[0] = vmaskptemp0_level2[0]; vmaskptemp1_level3[1] = vmaskptemp0_level2[1]; 
			vmaskptemp2_level3[0] = vmaskptemp0_level2[0]; vmaskptemp2_level3[1] = vmaskptemp0_level2[1]; 
			vmaskptemp3_level3[0] = vmaskptemp0_level2[0]; vmaskptemp3_level3[1] = vmaskptemp0_level2[1]; 
		}
		#ifdef SUP1 // // R_and_I1
		enablereducepp1 = enablereducepp0;
		readandsynchronize(enablereducepp1, pp1en_readandsynchronize, kvdram0, kvdram1, kvdram2, kvdram3, vbuffer0_level1, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffsetpp1_kvs, refbuffer, _globalparams);
		readandsynchronize(enablereducepp1, pp1en_readandsynchronize, kvdram4, kvdram5, kvdram6, kvdram7, vbuffer1_level1, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffsetpp1_kvs, _globalparams);
		readandsynchronize(enablereducepp1, pp1en_readandsynchronize, kvdram8, kvdram9, kvdram10, kvdram11, vbuffer2_level1, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffsetpp1_kvs, _globalparams);
		readandsynchronize(enablereducepp1, pp1en_readandsynchronize, kvdram12, kvdram13, kvdram14, kvdram15, vbuffer3_level1, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffsetpp1_kvs, _globalparams);
		#endif 
		
		// S_and_I4
		spreadandwrite(enablereduce, ON, kvdram0, kvdram1, kvdram2, kvdram3, 
			vbuffer0_level3, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffsetpp0_kvs, reducebuffersz,
			vmask0_level3, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs,
			source_partition, vmaskptemp0_level3,
			_globalparams);
		spreadandwrite(enablereduce, ON, kvdram4, kvdram5, kvdram6, kvdram7, 
			vbuffer1_level3, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffsetpp0_kvs, reducebuffersz,
			vmask1_level3, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs,
			source_partition, vmaskptemp1_level3,
			_globalparams);
		spreadandwrite(enablereduce, ON, kvdram8, kvdram9, kvdram10, kvdram11, 
			vbuffer2_level3, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffsetpp0_kvs, reducebuffersz,
			vmask2_level3, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs,
			source_partition, vmaskptemp2_level3,
			_globalparams);
		spreadandwrite(enablereduce, ON, kvdram12, kvdram13, kvdram14, kvdram15, 
			vbuffer3_level3, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffsetpp0_kvs, reducebuffersz,
			vmask3_level3, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs,
			source_partition, vmaskptemp3_level3,
			_globalparams);
		vreadoffsetpp0_kvs += reducebuffersz * NUMSYNCPIPELINES; //+++
		MOVEcount += 1; if(MOVEcount % 2 == 0){ vmaskreadoffset_kvs += vmaskbuffersz_kvs; sourcestatsmarker += 1; } //+++
		#ifdef SUP1 // I2 - pp1
		if(SWITCHcount % 2 == 0){ begincol_vmask = 0; } else { begincol_vmask = 8; };
		vmaskptemp0_level2[SWITCHcount % 2] = synchronizeandapply(enablereducepp1, pp1en_syncandapply, vbuffer0_level1, vbuffer1_level1, vbuffer2_level1, vbuffer3_level1, vbuffer0_level2, refbuffer, vmask0_level2, begincol_vmask, vreadoffsetpp1_kvs, _globalparams);
		if(enablereducepp1 == ON && pp1en_syncandapply == ON){ SWITCHcount += 1; }
		#endif
	}
	
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
	cout<< TIMINGRESULTSCOLOR <<"num active vertices for this iteration: "<<actsutilityobj->globalstats_getactvvsseen()<< RESET <<endl;
	actsutilityobj->globalstats_setactvvsseen(0);
	#endif 
	return;
}
}








