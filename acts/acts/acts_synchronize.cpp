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
keyvalue_t 
	#ifdef SW 
	acts_synchronize::
	#endif 
GETKV2(keyvalue_vbuffer_t data){
	#pragma HLS INLINE
	keyvalue_t res;
	#ifdef _WIDEWORD
	res.key = data.range(15, 0); //
	res.value = data.range(31, 16);
	// res.key = data.range(31, 0);
	// res.value = data.range(63, 32);
	#else 
	res.key = data.key;
	res.value = data.value;	
	// res.key = data.data; // CRITICAL REMOVEME.
	// res.value = data.data;	
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
	res.range(15, 0) = data.key; //
	res.range(31, 16) = data.value;
	// res.range(31, 0) = data.key;
	// res.range(63, 32) = data.value;
	#else
	res.key = data.key;
	res.value = data.value;	
	// res.data = data.key; // CRITICAL REMOVEME.
	// res.data = data.value;	
	#endif 
	return res;
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
savevmaskp(bool_type enable, uint512_dt * kvdram, unsigned int offset_kvs, uint32_type vmask_p, globalparams_t globalparams){
	#pragma HLS function_instantiate variable=kvdram
	if(enable == OFF){ return; }
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
readvdata(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	#pragma HLS function_instantiate variable=kvdram
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
	acts_synchronize::
	#endif 
savevdata(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	#pragma HLS function_instantiate variable=kvdram
	if(enable == OFF){ return; }
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
readvdata(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type buffer1offset_kvs, keyvalue_vbuffer_t buffer2[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], batch_type buffer2offset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
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
synchronize(bool_type enable, keyvalue_vbuffer_t buffer0[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer3[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t res[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], globalparams_t globalparams){
	#pragma HLS function_instantiate variable=res
	if(enable == OFF){ return; }
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
	cout<<"synchronize: synchronize function called."<<endl;
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
	
	
	for (buffer_type i=0; i<reducebuffersz; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE II=1
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
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE //
		if(GETKV2(res[0][i]).key < 0xFFFFFFFF){ cout<<"actvvid: "<<(0*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(res[0][i]).value < 0xFFFFFFFF){ cout<<"actvvid: "<<(0*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKV2(res[1][i]).key < 0xFFFFFFFF){ cout<<"actvvid: "<<(1*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(res[1][i]).value < 0xFFFFFFFF){ cout<<"actvvid: "<<(1*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKV2(res[2][i]).key < 0xFFFFFFFF){ cout<<"actvvid: "<<(2*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(res[2][i]).value < 0xFFFFFFFF){ cout<<"actvvid: "<<(2*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKV2(res[3][i]).key < 0xFFFFFFFF){ cout<<"actvvid: "<<(3*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(res[3][i]).value < 0xFFFFFFFF){ cout<<"actvvid: "<<(3*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKV2(res[4][i]).key < 0xFFFFFFFF){ cout<<"actvvid: "<<(4*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(res[4][i]).value < 0xFFFFFFFF){ cout<<"actvvid: "<<(4*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKV2(res[5][i]).key < 0xFFFFFFFF){ cout<<"actvvid: "<<(5*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(res[5][i]).value < 0xFFFFFFFF){ cout<<"actvvid: "<<(5*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKV2(res[6][i]).key < 0xFFFFFFFF){ cout<<"actvvid: "<<(6*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(res[6][i]).value < 0xFFFFFFFF){ cout<<"actvvid: "<<(6*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKV2(res[7][i]).key < 0xFFFFFFFF){ cout<<"actvvid: "<<(7*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(res[7][i]).value < 0xFFFFFFFF){ cout<<"actvvid: "<<(7*REDUCESZ + 2*i + 1)<<endl; } 
		#endif
	}
	return;
}

uint32_type
	#ifdef SW 
	acts_synchronize::
	#endif
apply(bool_type enable, 
		keyvalue_vbuffer_t vbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t synvbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask[BLOCKRAM_SIZE],
		unsigned int colindex, batch_type voffset_kvs, globalparams_t globalparams){
	if(enable == OFF){ return 0; }
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
	cout<<"apply: apply function called."<<endl;
	#endif 
	analysis_type analysis_loopcount = REDUCEBUFFERSZ;
	
	unsigned int vid[NUM_PARTITIONS][2];
	#pragma HLS ARRAY_PARTITION variable=vid complete
	
	keyvalue_t data0;
	keyvalue_t res0;
	keyvalue_t data1;
	keyvalue_t res1;
	keyvalue_t data2;
	keyvalue_t res2;
	keyvalue_t data3;
	keyvalue_t res3;
	keyvalue_t data4;
	keyvalue_t res4;
	keyvalue_t data5;
	keyvalue_t res5;
	keyvalue_t data6;
	keyvalue_t res6;
	keyvalue_t data7;
	keyvalue_t res7;
	
	uint32_type cummvmask_sp = 0;
	buffer_type reducebuffersz = globalparams.SIZE_REDUCE / 2;
	
	APPLY_LOOP1: for(buffer_type i=0; i<reducebuffersz; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
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
		#endif 
	
		data0 = GETKV2(vbuffer[0][i]);
		data1 = GETKV2(vbuffer[1][i]);
		data2 = GETKV2(vbuffer[2][i]);
		data3 = GETKV2(vbuffer[3][i]);
		data4 = GETKV2(vbuffer[4][i]);
		data5 = GETKV2(vbuffer[5][i]);
		data6 = GETKV2(vbuffer[6][i]);
		data7 = GETKV2(vbuffer[7][i]);
		
		keyvalue_t udata0 = GETKV2(synvbuffer[0][i]);
		keyvalue_t udata1 = GETKV2(synvbuffer[1][i]);
		keyvalue_t udata2 = GETKV2(synvbuffer[2][i]);
		keyvalue_t udata3 = GETKV2(synvbuffer[3][i]);
		keyvalue_t udata4 = GETKV2(synvbuffer[4][i]);
		keyvalue_t udata5 = GETKV2(synvbuffer[5][i]);
		keyvalue_t udata6 = GETKV2(synvbuffer[6][i]);
		keyvalue_t udata7 = GETKV2(synvbuffer[7][i]);
		
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
		
		#ifdef INCORRECTAPPLY // CRITICAL REMOVEME. CAUSEOFERROR
		if(res0.key != udata0.key || res0.key < 0xFFFFFFFF){ vmask[i].data[colindex + 0].key = 1; } else { vmask[i].data[colindex + 0].key = 0; }
		if(res0.value != udata0.value || res0.value < 0xFFFFFFFF){ vmask[i].data[colindex + 0].value = 1; } else { vmask[i].data[colindex + 0].value = 0; }
		if(res1.key != udata1.key || res1.key < 0xFFFFFFFF){ vmask[i].data[colindex + 1].key = 1; } else { vmask[i].data[colindex + 1].key = 0; }
		if(res1.value != udata1.value || res1.value < 0xFFFFFFFF){ vmask[i].data[colindex + 1].value = 1; } else { vmask[i].data[colindex + 1].value = 0; }
		if(res2.key != udata2.key || res2.key < 0xFFFFFFFF){ vmask[i].data[colindex + 2].key = 1; } else { vmask[i].data[colindex + 2].key = 0; }
		if(res2.value != udata2.value || res2.value < 0xFFFFFFFF){ vmask[i].data[colindex + 2].value = 1; } else { vmask[i].data[colindex + 2].value = 0; }
		if(res3.key != udata3.key || res3.key < 0xFFFFFFFF){ vmask[i].data[colindex + 3].key = 1; } else { vmask[i].data[colindex + 3].key = 0; }
		if(res3.value != udata3.value || res3.value < 0xFFFFFFFF){ vmask[i].data[colindex + 3].value = 1; } else { vmask[i].data[colindex + 3].value = 0; }
		if(res4.key != udata4.key || res4.key < 0xFFFFFFFF){ vmask[i].data[colindex + 4].key = 1; } else { vmask[i].data[colindex + 4].key = 0; }
		if(res4.value != udata4.value || res4.value < 0xFFFFFFFF){ vmask[i].data[colindex + 4].value = 1; } else { vmask[i].data[colindex + 4].value = 0; }
		if(res5.key != udata5.key || res5.key < 0xFFFFFFFF){ vmask[i].data[colindex + 5].key = 1; } else { vmask[i].data[colindex + 5].key = 0; }
		if(res5.value != udata5.value || res5.value < 0xFFFFFFFF){ vmask[i].data[colindex + 5].value = 1; } else { vmask[i].data[colindex + 5].value = 0; }
		if(res6.key != udata6.key || res6.key < 0xFFFFFFFF){ vmask[i].data[colindex + 6].key = 1; } else { vmask[i].data[colindex + 6].key = 0; }
		if(res6.value != udata6.value || res6.value < 0xFFFFFFFF){ vmask[i].data[colindex + 6].value = 1; } else { vmask[i].data[colindex + 6].value = 0; }
		if(res7.key != udata7.key || res7.key < 0xFFFFFFFF){ vmask[i].data[colindex + 7].key = 1; } else { vmask[i].data[colindex + 7].key = 0; }
		if(res7.value != udata7.value || res7.value < 0xFFFFFFFF){ vmask[i].data[colindex + 7].value = 1; } else { vmask[i].data[colindex + 7].value = 0; }
	
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
		cummvmask_sp = cummvmask_sp | mask; // CRITICAL FIXME.
		
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
		if(false){ cout<<"APPLY FUNC SEEN @ (vid1: "<<vid[0][0]<<" & vid2: "<<vid[0][1]<<"): res0.key: "<<res0.key<<", res0.value: "<<res0.value<<": udata0.key: "<<udata0.key<<", udata0.value: "<<udata0.value<<", data0.key: "<<data0.key<<", data0.value: "<<data0.value<<endl; }
		if(res0.key != udata0.key || res0.key < 0xFFFFFFFF){ cout<<"apply: vid[0][0]: "<<vid[0][0]<<", vmask["<<i<<"].data["<<colindex + 0<<"].key: "<<vmask[i].data[colindex + 0].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res0.value != udata0.value || res0.value < 0xFFFFFFFF){ cout<<"apply: vid[0][1]: "<<vid[0][1]<<", vmask["<<i<<"].data["<<colindex + 0<<"].value: "<<vmask[i].data[colindex + 0].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(false){ cout<<"APPLY FUNC SEEN @ (vid1: "<<vid[1][0]<<" & vid2: "<<vid[1][1]<<"): res1.key: "<<res1.key<<", res1.value: "<<res1.value<<": udata1.key: "<<udata1.key<<", udata1.value: "<<udata1.value<<", data1.key: "<<data1.key<<", data1.value: "<<data1.value<<endl; }
		if(res1.key != udata1.key || res1.key < 0xFFFFFFFF){ cout<<"apply: vid[1][0]: "<<vid[1][0]<<", vmask["<<i<<"].data["<<colindex + 1<<"].key: "<<vmask[i].data[colindex + 1].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res1.value != udata1.value || res1.value < 0xFFFFFFFF){ cout<<"apply: vid[1][1]: "<<vid[1][1]<<", vmask["<<i<<"].data["<<colindex + 1<<"].value: "<<vmask[i].data[colindex + 1].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(false){ cout<<"APPLY FUNC SEEN @ (vid1: "<<vid[2][0]<<" & vid2: "<<vid[2][1]<<"): res2.key: "<<res2.key<<", res2.value: "<<res2.value<<": udata2.key: "<<udata2.key<<", udata2.value: "<<udata2.value<<", data2.key: "<<data2.key<<", data2.value: "<<data2.value<<endl; }
		if(res2.key != udata2.key || res2.key < 0xFFFFFFFF){ cout<<"apply: vid[2][0]: "<<vid[2][0]<<", vmask["<<i<<"].data["<<colindex + 2<<"].key: "<<vmask[i].data[colindex + 2].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res2.value != udata2.value || res2.value < 0xFFFFFFFF){ cout<<"apply: vid[2][1]: "<<vid[2][1]<<", vmask["<<i<<"].data["<<colindex + 2<<"].value: "<<vmask[i].data[colindex + 2].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(false){ cout<<"APPLY FUNC SEEN @ (vid1: "<<vid[3][0]<<" & vid2: "<<vid[3][1]<<"): res3.key: "<<res3.key<<", res3.value: "<<res3.value<<": udata3.key: "<<udata3.key<<", udata3.value: "<<udata3.value<<", data3.key: "<<data3.key<<", data3.value: "<<data3.value<<endl; }
		if(res3.key != udata3.key || res3.key < 0xFFFFFFFF){ cout<<"apply: vid[3][0]: "<<vid[3][0]<<", vmask["<<i<<"].data["<<colindex + 3<<"].key: "<<vmask[i].data[colindex + 3].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res3.value != udata3.value || res3.value < 0xFFFFFFFF){ cout<<"apply: vid[3][1]: "<<vid[3][1]<<", vmask["<<i<<"].data["<<colindex + 3<<"].value: "<<vmask[i].data[colindex + 3].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(false){ cout<<"APPLY FUNC SEEN @ (vid1: "<<vid[4][0]<<" & vid2: "<<vid[4][1]<<"): res4.key: "<<res4.key<<", res4.value: "<<res4.value<<": udata4.key: "<<udata4.key<<", udata4.value: "<<udata4.value<<", data4.key: "<<data4.key<<", data4.value: "<<data4.value<<endl; }
		if(res4.key != udata4.key || res4.key < 0xFFFFFFFF){ cout<<"apply: vid[4][0]: "<<vid[4][0]<<", vmask["<<i<<"].data["<<colindex + 4<<"].key: "<<vmask[i].data[colindex + 4].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res4.value != udata4.value || res4.value < 0xFFFFFFFF){ cout<<"apply: vid[4][1]: "<<vid[4][1]<<", vmask["<<i<<"].data["<<colindex + 4<<"].value: "<<vmask[i].data[colindex + 4].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(false){ cout<<"APPLY FUNC SEEN @ (vid1: "<<vid[5][0]<<" & vid2: "<<vid[5][1]<<"): res5.key: "<<res5.key<<", res5.value: "<<res5.value<<": udata5.key: "<<udata5.key<<", udata5.value: "<<udata5.value<<", data5.key: "<<data5.key<<", data5.value: "<<data5.value<<endl; }
		if(res5.key != udata5.key || res5.key < 0xFFFFFFFF){ cout<<"apply: vid[5][0]: "<<vid[5][0]<<", vmask["<<i<<"].data["<<colindex + 5<<"].key: "<<vmask[i].data[colindex + 5].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res5.value != udata5.value || res5.value < 0xFFFFFFFF){ cout<<"apply: vid[5][1]: "<<vid[5][1]<<", vmask["<<i<<"].data["<<colindex + 5<<"].value: "<<vmask[i].data[colindex + 5].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(false){ cout<<"APPLY FUNC SEEN @ (vid1: "<<vid[6][0]<<" & vid2: "<<vid[6][1]<<"): res6.key: "<<res6.key<<", res6.value: "<<res6.value<<": udata6.key: "<<udata6.key<<", udata6.value: "<<udata6.value<<", data6.key: "<<data6.key<<", data6.value: "<<data6.value<<endl; }
		if(res6.key != udata6.key || res6.key < 0xFFFFFFFF){ cout<<"apply: vid[6][0]: "<<vid[6][0]<<", vmask["<<i<<"].data["<<colindex + 6<<"].key: "<<vmask[i].data[colindex + 6].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res6.value != udata6.value || res6.value < 0xFFFFFFFF){ cout<<"apply: vid[6][1]: "<<vid[6][1]<<", vmask["<<i<<"].data["<<colindex + 6<<"].value: "<<vmask[i].data[colindex + 6].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(false){ cout<<"APPLY FUNC SEEN @ (vid1: "<<vid[7][0]<<" & vid2: "<<vid[7][1]<<"): res7.key: "<<res7.key<<", res7.value: "<<res7.value<<": udata7.key: "<<udata7.key<<", udata7.value: "<<udata7.value<<", data7.key: "<<data7.key<<", data7.value: "<<data7.value<<endl; }
		if(res7.key != udata7.key || res7.key < 0xFFFFFFFF){ cout<<"apply: vid[7][0]: "<<vid[7][0]<<", vmask["<<i<<"].data["<<colindex + 7<<"].key: "<<vmask[i].data[colindex + 7].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res7.value != udata7.value || res7.value < 0xFFFFFFFF){ cout<<"apply: vid[7][1]: "<<vid[7][1]<<", vmask["<<i<<"].data["<<colindex + 7<<"].value: "<<vmask[i].data[colindex + 7].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		#endif
		#ifdef _DEBUGMODE_STATS
		#ifdef INCORRECTAPPLY // CRITICAL REMOVEME. CAUSEOFERROR
		if(res0.key != udata0.key || res0.key < 0xFFFFFFFF){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res0.value != udata0.value || res0.key < 0xFFFFFFFF){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res1.key != udata1.key || res1.key < 0xFFFFFFFF){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res1.value != udata1.value || res1.key < 0xFFFFFFFF){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res2.key != udata2.key || res2.key < 0xFFFFFFFF){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res2.value != udata2.value || res2.key < 0xFFFFFFFF){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res3.key != udata3.key || res3.key < 0xFFFFFFFF){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res3.value != udata3.value || res3.key < 0xFFFFFFFF){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res4.key != udata4.key || res4.key < 0xFFFFFFFF){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res4.value != udata4.value || res4.key < 0xFFFFFFFF){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res5.key != udata5.key || res5.key < 0xFFFFFFFF){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res5.value != udata5.value || res5.key < 0xFFFFFFFF){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res6.key != udata6.key || res6.key < 0xFFFFFFFF){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res6.value != udata6.value || res6.key < 0xFFFFFFFF){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res7.key != udata7.key || res7.key < 0xFFFFFFFF){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res7.value != udata7.value || res7.key < 0xFFFFFFFF){ actsutilityobj->globalstats_countactvvsseen(1); }
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
uint32_type
	#ifdef SW 
	acts_synchronize::
	#endif
synchronizeandapply(bool_type enable, keyvalue_vbuffer_t buffer0[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer3[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t res[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t refbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask[BLOCKRAM_SIZE], unsigned int colindex, batch_type voffset_kvs, globalparams_t globalparams){
	if(enable == OFF){ return 0; }
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
		if(GETKV2(res[0][i]).key < 0xFFFFFFFF){ cout<<"actvvid: "<<(0*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(res[0][i]).value < 0xFFFFFFFF){ cout<<"actvvid: "<<(0*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKV2(res[1][i]).key < 0xFFFFFFFF){ cout<<"actvvid: "<<(1*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(res[1][i]).value < 0xFFFFFFFF){ cout<<"actvvid: "<<(1*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKV2(res[2][i]).key < 0xFFFFFFFF){ cout<<"actvvid: "<<(2*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(res[2][i]).value < 0xFFFFFFFF){ cout<<"actvvid: "<<(2*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKV2(res[3][i]).key < 0xFFFFFFFF){ cout<<"actvvid: "<<(3*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(res[3][i]).value < 0xFFFFFFFF){ cout<<"actvvid: "<<(3*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKV2(res[4][i]).key < 0xFFFFFFFF){ cout<<"actvvid: "<<(4*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(res[4][i]).value < 0xFFFFFFFF){ cout<<"actvvid: "<<(4*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKV2(res[5][i]).key < 0xFFFFFFFF){ cout<<"actvvid: "<<(5*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(res[5][i]).value < 0xFFFFFFFF){ cout<<"actvvid: "<<(5*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKV2(res[6][i]).key < 0xFFFFFFFF){ cout<<"actvvid: "<<(6*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(res[6][i]).value < 0xFFFFFFFF){ cout<<"actvvid: "<<(6*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKV2(res[7][i]).key < 0xFFFFFFFF){ cout<<"actvvid: "<<(7*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(res[7][i]).value < 0xFFFFFFFF){ cout<<"actvvid: "<<(7*REDUCESZ + 2*i + 1)<<endl; } 
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
		if(res0.key != udata0.key || res0.key < 0xFFFFFFFF){ vmask[i].data[colindex + 0].key = 1; } else { vmask[i].data[colindex + 0].key = 0; }
		if(res0.value != udata0.value || res0.value < 0xFFFFFFFF){ vmask[i].data[colindex + 0].value = 1; } else { vmask[i].data[colindex + 0].value = 0; }
		if(res1.key != udata1.key || res1.key < 0xFFFFFFFF){ vmask[i].data[colindex + 1].key = 1; } else { vmask[i].data[colindex + 1].key = 0; }
		if(res1.value != udata1.value || res1.value < 0xFFFFFFFF){ vmask[i].data[colindex + 1].value = 1; } else { vmask[i].data[colindex + 1].value = 0; }
		if(res2.key != udata2.key || res2.key < 0xFFFFFFFF){ vmask[i].data[colindex + 2].key = 1; } else { vmask[i].data[colindex + 2].key = 0; }
		if(res2.value != udata2.value || res2.value < 0xFFFFFFFF){ vmask[i].data[colindex + 2].value = 1; } else { vmask[i].data[colindex + 2].value = 0; }
		if(res3.key != udata3.key || res3.key < 0xFFFFFFFF){ vmask[i].data[colindex + 3].key = 1; } else { vmask[i].data[colindex + 3].key = 0; }
		if(res3.value != udata3.value || res3.value < 0xFFFFFFFF){ vmask[i].data[colindex + 3].value = 1; } else { vmask[i].data[colindex + 3].value = 0; }
		if(res4.key != udata4.key || res4.key < 0xFFFFFFFF){ vmask[i].data[colindex + 4].key = 1; } else { vmask[i].data[colindex + 4].key = 0; }
		if(res4.value != udata4.value || res4.value < 0xFFFFFFFF){ vmask[i].data[colindex + 4].value = 1; } else { vmask[i].data[colindex + 4].value = 0; }
		if(res5.key != udata5.key || res5.key < 0xFFFFFFFF){ vmask[i].data[colindex + 5].key = 1; } else { vmask[i].data[colindex + 5].key = 0; }
		if(res5.value != udata5.value || res5.value < 0xFFFFFFFF){ vmask[i].data[colindex + 5].value = 1; } else { vmask[i].data[colindex + 5].value = 0; }
		if(res6.key != udata6.key || res6.key < 0xFFFFFFFF){ vmask[i].data[colindex + 6].key = 1; } else { vmask[i].data[colindex + 6].key = 0; }
		if(res6.value != udata6.value || res6.value < 0xFFFFFFFF){ vmask[i].data[colindex + 6].value = 1; } else { vmask[i].data[colindex + 6].value = 0; }
		if(res7.key != udata7.key || res7.key < 0xFFFFFFFF){ vmask[i].data[colindex + 7].key = 1; } else { vmask[i].data[colindex + 7].key = 0; }
		if(res7.value != udata7.value || res7.value < 0xFFFFFFFF){ vmask[i].data[colindex + 7].value = 1; } else { vmask[i].data[colindex + 7].value = 0; }
	
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
		if(res0.key != udata0.key || res0.key < 0xFFFFFFFF){ cout<<"apply: vid[0][0]: "<<vid[0][0]<<", vmask["<<i<<"].data["<<colindex + 0<<"].key: "<<vmask[i].data[colindex + 0].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res0.value != udata0.value || res0.value < 0xFFFFFFFF){ cout<<"apply: vid[0][1]: "<<vid[0][1]<<", vmask["<<i<<"].data["<<colindex + 0<<"].value: "<<vmask[i].data[colindex + 0].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(false){ cout<<"APPLY FUNC SEEN @ (vid1: "<<vid[1][0]<<" & vid2: "<<vid[1][1]<<"): res1.key: "<<res1.key<<", res1.value: "<<res1.value<<": udata1.key: "<<udata1.key<<", udata1.value: "<<udata1.value<<", data1.key: "<<data1.key<<", data1.value: "<<data1.value<<endl; }
		if(res1.key != udata1.key || res1.key < 0xFFFFFFFF){ cout<<"apply: vid[1][0]: "<<vid[1][0]<<", vmask["<<i<<"].data["<<colindex + 1<<"].key: "<<vmask[i].data[colindex + 1].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res1.value != udata1.value || res1.value < 0xFFFFFFFF){ cout<<"apply: vid[1][1]: "<<vid[1][1]<<", vmask["<<i<<"].data["<<colindex + 1<<"].value: "<<vmask[i].data[colindex + 1].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(false){ cout<<"APPLY FUNC SEEN @ (vid1: "<<vid[2][0]<<" & vid2: "<<vid[2][1]<<"): res2.key: "<<res2.key<<", res2.value: "<<res2.value<<": udata2.key: "<<udata2.key<<", udata2.value: "<<udata2.value<<", data2.key: "<<data2.key<<", data2.value: "<<data2.value<<endl; }
		if(res2.key != udata2.key || res2.key < 0xFFFFFFFF){ cout<<"apply: vid[2][0]: "<<vid[2][0]<<", vmask["<<i<<"].data["<<colindex + 2<<"].key: "<<vmask[i].data[colindex + 2].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res2.value != udata2.value || res2.value < 0xFFFFFFFF){ cout<<"apply: vid[2][1]: "<<vid[2][1]<<", vmask["<<i<<"].data["<<colindex + 2<<"].value: "<<vmask[i].data[colindex + 2].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(false){ cout<<"APPLY FUNC SEEN @ (vid1: "<<vid[3][0]<<" & vid2: "<<vid[3][1]<<"): res3.key: "<<res3.key<<", res3.value: "<<res3.value<<": udata3.key: "<<udata3.key<<", udata3.value: "<<udata3.value<<", data3.key: "<<data3.key<<", data3.value: "<<data3.value<<endl; }
		if(res3.key != udata3.key || res3.key < 0xFFFFFFFF){ cout<<"apply: vid[3][0]: "<<vid[3][0]<<", vmask["<<i<<"].data["<<colindex + 3<<"].key: "<<vmask[i].data[colindex + 3].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res3.value != udata3.value || res3.value < 0xFFFFFFFF){ cout<<"apply: vid[3][1]: "<<vid[3][1]<<", vmask["<<i<<"].data["<<colindex + 3<<"].value: "<<vmask[i].data[colindex + 3].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(false){ cout<<"APPLY FUNC SEEN @ (vid1: "<<vid[4][0]<<" & vid2: "<<vid[4][1]<<"): res4.key: "<<res4.key<<", res4.value: "<<res4.value<<": udata4.key: "<<udata4.key<<", udata4.value: "<<udata4.value<<", data4.key: "<<data4.key<<", data4.value: "<<data4.value<<endl; }
		if(res4.key != udata4.key || res4.key < 0xFFFFFFFF){ cout<<"apply: vid[4][0]: "<<vid[4][0]<<", vmask["<<i<<"].data["<<colindex + 4<<"].key: "<<vmask[i].data[colindex + 4].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res4.value != udata4.value || res4.value < 0xFFFFFFFF){ cout<<"apply: vid[4][1]: "<<vid[4][1]<<", vmask["<<i<<"].data["<<colindex + 4<<"].value: "<<vmask[i].data[colindex + 4].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(false){ cout<<"APPLY FUNC SEEN @ (vid1: "<<vid[5][0]<<" & vid2: "<<vid[5][1]<<"): res5.key: "<<res5.key<<", res5.value: "<<res5.value<<": udata5.key: "<<udata5.key<<", udata5.value: "<<udata5.value<<", data5.key: "<<data5.key<<", data5.value: "<<data5.value<<endl; }
		if(res5.key != udata5.key || res5.key < 0xFFFFFFFF){ cout<<"apply: vid[5][0]: "<<vid[5][0]<<", vmask["<<i<<"].data["<<colindex + 5<<"].key: "<<vmask[i].data[colindex + 5].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res5.value != udata5.value || res5.value < 0xFFFFFFFF){ cout<<"apply: vid[5][1]: "<<vid[5][1]<<", vmask["<<i<<"].data["<<colindex + 5<<"].value: "<<vmask[i].data[colindex + 5].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(false){ cout<<"APPLY FUNC SEEN @ (vid1: "<<vid[6][0]<<" & vid2: "<<vid[6][1]<<"): res6.key: "<<res6.key<<", res6.value: "<<res6.value<<": udata6.key: "<<udata6.key<<", udata6.value: "<<udata6.value<<", data6.key: "<<data6.key<<", data6.value: "<<data6.value<<endl; }
		if(res6.key != udata6.key || res6.key < 0xFFFFFFFF){ cout<<"apply: vid[6][0]: "<<vid[6][0]<<", vmask["<<i<<"].data["<<colindex + 6<<"].key: "<<vmask[i].data[colindex + 6].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res6.value != udata6.value || res6.value < 0xFFFFFFFF){ cout<<"apply: vid[6][1]: "<<vid[6][1]<<", vmask["<<i<<"].data["<<colindex + 6<<"].value: "<<vmask[i].data[colindex + 6].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(false){ cout<<"APPLY FUNC SEEN @ (vid1: "<<vid[7][0]<<" & vid2: "<<vid[7][1]<<"): res7.key: "<<res7.key<<", res7.value: "<<res7.value<<": udata7.key: "<<udata7.key<<", udata7.value: "<<udata7.value<<", data7.key: "<<data7.key<<", data7.value: "<<data7.value<<endl; }
		if(res7.key != udata7.key || res7.key < 0xFFFFFFFF){ cout<<"apply: vid[7][0]: "<<vid[7][0]<<", vmask["<<i<<"].data["<<colindex + 7<<"].key: "<<vmask[i].data[colindex + 7].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		if(res7.value != udata7.value || res7.value < 0xFFFFFFFF){ cout<<"apply: vid[7][1]: "<<vid[7][1]<<", vmask["<<i<<"].data["<<colindex + 7<<"].value: "<<vmask[i].data[colindex + 7].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		#endif
		#ifdef _DEBUGMODE_STATS
		#ifdef INCORRECTAPPLY
		if(res0.key != udata0.key || res0.key < 0xFFFFFFFF){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res0.value != udata0.value || res0.key < 0xFFFFFFFF){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res1.key != udata1.key || res1.key < 0xFFFFFFFF){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res1.value != udata1.value || res1.key < 0xFFFFFFFF){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res2.key != udata2.key || res2.key < 0xFFFFFFFF){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res2.value != udata2.value || res2.key < 0xFFFFFFFF){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res3.key != udata3.key || res3.key < 0xFFFFFFFF){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res3.value != udata3.value || res3.key < 0xFFFFFFFF){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res4.key != udata4.key || res4.key < 0xFFFFFFFF){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res4.value != udata4.value || res4.key < 0xFFFFFFFF){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res5.key != udata5.key || res5.key < 0xFFFFFFFF){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res5.value != udata5.value || res5.key < 0xFFFFFFFF){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res6.key != udata6.key || res6.key < 0xFFFFFFFF){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res6.value != udata6.value || res6.key < 0xFFFFFFFF){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res7.key != udata7.key || res7.key < 0xFFFFFFFF){ actsutilityobj->globalstats_countactvvsseen(1); }
		if(res7.value != udata7.value || res7.key < 0xFFFFFFFF){ actsutilityobj->globalstats_countactvvsseen(1); }
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
spreadvdata(bool_type enable, keyvalue_vbuffer_t source[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t buffer0[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer3[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], globalparams_t globalparams){
	#pragma HLS function_instantiate variable=source
	if(enable == OFF){ return; }
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
		if(GETKV2(buffer3[0][i]).key < 0xFFFFFFFF){ cout<<"actvvid: "<<(0*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(buffer3[0][i]).value < 0xFFFFFFFF){ cout<<"actvvid: "<<(0*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKV2(buffer3[1][i]).key < 0xFFFFFFFF){ cout<<"actvvid: "<<(1*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(buffer3[1][i]).value < 0xFFFFFFFF){ cout<<"actvvid: "<<(1*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKV2(buffer3[2][i]).key < 0xFFFFFFFF){ cout<<"actvvid: "<<(2*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(buffer3[2][i]).value < 0xFFFFFFFF){ cout<<"actvvid: "<<(2*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKV2(buffer3[3][i]).key < 0xFFFFFFFF){ cout<<"actvvid: "<<(3*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(buffer3[3][i]).value < 0xFFFFFFFF){ cout<<"actvvid: "<<(3*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKV2(buffer3[4][i]).key < 0xFFFFFFFF){ cout<<"actvvid: "<<(4*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(buffer3[4][i]).value < 0xFFFFFFFF){ cout<<"actvvid: "<<(4*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKV2(buffer3[5][i]).key < 0xFFFFFFFF){ cout<<"actvvid: "<<(5*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(buffer3[5][i]).value < 0xFFFFFFFF){ cout<<"actvvid: "<<(5*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKV2(buffer3[6][i]).key < 0xFFFFFFFF){ cout<<"actvvid: "<<(6*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(buffer3[6][i]).value < 0xFFFFFFFF){ cout<<"actvvid: "<<(6*REDUCESZ + 2*i + 1)<<endl; } 
		if(GETKV2(buffer3[7][i]).key < 0xFFFFFFFF){ cout<<"actvvid: "<<(7*REDUCESZ + 2*i)<<endl; } 
		if(GETKV2(buffer3[7][i]).value < 0xFFFFFFFF){ cout<<"actvvid: "<<(7*REDUCESZ + 2*i + 1)<<endl; } 
		#endif
	}
	return;
}

void
	#ifdef SW 
	acts_synchronize::
	#endif
spreadvmask(bool_type enable1, bool_type enable2, uintNUMPby2_type vmask[BLOCKRAM_SIZE], uintNUMPby2_type vmask0[BLOCKRAM_SIZE],uintNUMPby2_type vmask1[BLOCKRAM_SIZE],uintNUMPby2_type vmask2[BLOCKRAM_SIZE],uintNUMPby2_type vmask3[BLOCKRAM_SIZE],uintNUMPby2_type vmask4[BLOCKRAM_SIZE],uintNUMPby2_type vmask5[BLOCKRAM_SIZE],uintNUMPby2_type vmask6[BLOCKRAM_SIZE],uintNUMPby2_type vmask7[BLOCKRAM_SIZE],uintNUMPby2_type vmask8[BLOCKRAM_SIZE],uintNUMPby2_type vmask9[BLOCKRAM_SIZE],uintNUMPby2_type vmask10[BLOCKRAM_SIZE],uintNUMPby2_type vmask11[BLOCKRAM_SIZE],uintNUMPby2_type vmask12[BLOCKRAM_SIZE],uintNUMPby2_type vmask13[BLOCKRAM_SIZE],uintNUMPby2_type vmask14[BLOCKRAM_SIZE],uintNUMPby2_type vmask15[BLOCKRAM_SIZE], globalparams_t globalparams){
	#pragma HLS INLINE OFF //
	if(enable1 == OFF || enable2 == OFF){ return; }
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

// top
/* extern "C" {
void 
	#ifdef SW 
	acts_synchronize:: 
	#endif
topkernel(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11,uint512_dt * kvdram12,uint512_dt * kvdram13,uint512_dt * kvdram14,uint512_dt * kvdram15){
 
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

	#ifdef _DEBUGMODE_KERNELPRINTS2
	cout<<">>> Light weight ACTS (NACTS_IN_NCOMPUTEUNITS.SYNC) Launched... "<<endl; 
	#endif
	
	keyvalue_vbuffer_t refbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = refbuffer
	
	travstate_t rtravstate[NUMCOMPUTEUNITS];
	#pragma HLS ARRAY_PARTITION variable=rtravstate complete
	
	keyvalue_vbuffer_t vbuffer0[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	keyvalue_vbuffer_t vbuffer1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	keyvalue_vbuffer_t vbuffer2[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer2
	keyvalue_vbuffer_t vbuffer3[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer3
	keyvalue_vbuffer_t vbuffer4[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer4
	keyvalue_vbuffer_t vbuffer5[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer5
	keyvalue_vbuffer_t vbuffer6[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer6
	keyvalue_vbuffer_t vbuffer7[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer7
	keyvalue_vbuffer_t vbuffer8[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer8
	keyvalue_vbuffer_t vbuffer9[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer9
	keyvalue_vbuffer_t vbuffer10[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer10
	keyvalue_vbuffer_t vbuffer11[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer11
	keyvalue_vbuffer_t vbuffer12[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer12
	keyvalue_vbuffer_t vbuffer13[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer13
	keyvalue_vbuffer_t vbuffer14[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer14
	keyvalue_vbuffer_t vbuffer15[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer15
	
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
	
	keyvalue_vbuffer_t vbuffer0_level4[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer0_level4
	keyvalue_vbuffer_t vbuffer1_level4[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer1_level4
	keyvalue_vbuffer_t vbuffer2_level4[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer2_level4
	keyvalue_vbuffer_t vbuffer3_level4[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer3_level4
	keyvalue_vbuffer_t vbuffer4_level4[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer4_level4
	keyvalue_vbuffer_t vbuffer5_level4[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer5_level4
	keyvalue_vbuffer_t vbuffer6_level4[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer6_level4
	keyvalue_vbuffer_t vbuffer7_level4[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer7_level4
	keyvalue_vbuffer_t vbuffer8_level4[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer8_level4
	keyvalue_vbuffer_t vbuffer9_level4[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer9_level4
	keyvalue_vbuffer_t vbuffer10_level4[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer10_level4
	keyvalue_vbuffer_t vbuffer11_level4[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer11_level4
	keyvalue_vbuffer_t vbuffer12_level4[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer12_level4
	keyvalue_vbuffer_t vbuffer13_level4[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer13_level4
	keyvalue_vbuffer_t vbuffer14_level4[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer14_level4
	keyvalue_vbuffer_t vbuffer15_level4[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer15_level4
	
	uintNUMPby2_type vmask0[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask0
	uint32_type vmask_p0[BLOCKRAM_SIZE];
	uintNUMPby2_type vmask1[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask1
	uint32_type vmask_p1[BLOCKRAM_SIZE];
	uintNUMPby2_type vmask2[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask2
	uint32_type vmask_p2[BLOCKRAM_SIZE];
	uintNUMPby2_type vmask3[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask3
	uint32_type vmask_p3[BLOCKRAM_SIZE];
	uintNUMPby2_type vmask4[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask4
	uint32_type vmask_p4[BLOCKRAM_SIZE];
	uintNUMPby2_type vmask5[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask5
	uint32_type vmask_p5[BLOCKRAM_SIZE];
	uintNUMPby2_type vmask6[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask6
	uint32_type vmask_p6[BLOCKRAM_SIZE];
	uintNUMPby2_type vmask7[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask7
	uint32_type vmask_p7[BLOCKRAM_SIZE];
	uintNUMPby2_type vmask8[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask8
	uint32_type vmask_p8[BLOCKRAM_SIZE];
	uintNUMPby2_type vmask9[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask9
	uint32_type vmask_p9[BLOCKRAM_SIZE];
	uintNUMPby2_type vmask10[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask10
	uint32_type vmask_p10[BLOCKRAM_SIZE];
	uintNUMPby2_type vmask11[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask11
	uint32_type vmask_p11[BLOCKRAM_SIZE];
	uintNUMPby2_type vmask12[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask12
	uint32_type vmask_p12[BLOCKRAM_SIZE];
	uintNUMPby2_type vmask13[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask13
	uint32_type vmask_p13[BLOCKRAM_SIZE];
	uintNUMPby2_type vmask14[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask14
	uint32_type vmask_p14[BLOCKRAM_SIZE];
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
	
	uint32_type vmask_p_temp[2];
	vmask_p_temp[0] = 0; 
	vmask_p_temp[1] = 0;

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
	
	unsigned int vreadoffset_kvs = 0;
	unsigned int vmaskreadoffset_kvs = 0;
	buffer_type reducebuffersz = _globalparams.SIZE_REDUCE / 2; // 512
	buffer_type vmaskbuffersz_kvs = (_globalparams.SIZE_REDUCE * NUM_PARTITIONS) / 512; // 32
	
	step_type currentLOP = _globalparams.ACTSPARAMS_TREEDEPTH;
	batch_type num_source_partitions = get_num_source_partitions(currentLOP);

	for(unsigned int k=0; k<num_source_partitions; k++){ vmask_p[k] = 0; }

	TOPKERNELSYNC_MAINLOOP: for(batch_type source_partition=0; source_partition<num_source_partitions; source_partition+=1){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loop1 avg=analysis_loop1
	
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
	
		vmask_p_temp[0] = 0; 
		vmask_p_temp[1] = 0;
		
		TOPKERNELSYNC_MAINLOOP1B: for(batch_type it=0; it<2; it+=1){
			#ifdef _DEBUGMODE_KERNELPRINTS
			actsutilityobj->print3("### topkernel:: source_partition", "currentLOP", "NAp", source_partition, currentLOP, NAp); 							
			#endif
			
			batch_type voffset_kvs = (source_partition * reducebuffersz * FETFACTOR) + (it * reducebuffersz);
			
			// R
	
			readvdata(enablereduce, kvdram0, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer0, 0, 0, refbuffer, 0, reducebuffersz, _globalparams);
	
			readvdata(enablereduce, kvdram1, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer1, 0, 0,  reducebuffersz, _globalparams);
	
			readvdata(enablereduce, kvdram2, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer2, 0, 0,  reducebuffersz, _globalparams);
	
			readvdata(enablereduce, kvdram3, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer3, 0, 0,  reducebuffersz, _globalparams);
	
			readvdata(enablereduce, kvdram4, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer4, 0, 0,  reducebuffersz, _globalparams);
	
			readvdata(enablereduce, kvdram5, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer5, 0, 0,  reducebuffersz, _globalparams);
	
			readvdata(enablereduce, kvdram6, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer6, 0, 0,  reducebuffersz, _globalparams);
	
			readvdata(enablereduce, kvdram7, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer7, 0, 0,  reducebuffersz, _globalparams);
	
			readvdata(enablereduce, kvdram8, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer8, 0, 0,  reducebuffersz, _globalparams);
	
			readvdata(enablereduce, kvdram9, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer9, 0, 0,  reducebuffersz, _globalparams);
	
			readvdata(enablereduce, kvdram10, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer10, 0, 0,  reducebuffersz, _globalparams);
	
			readvdata(enablereduce, kvdram11, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer11, 0, 0,  reducebuffersz, _globalparams);
	
			readvdata(enablereduce, kvdram12, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer12, 0, 0,  reducebuffersz, _globalparams);
	
			readvdata(enablereduce, kvdram13, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer13, 0, 0,  reducebuffersz, _globalparams);
	
			readvdata(enablereduce, kvdram14, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer14, 0, 0,  reducebuffersz, _globalparams);
	
			readvdata(enablereduce, kvdram15, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer15, 0, 0,  reducebuffersz, _globalparams);
			
			// I1
			synchronize(enablereduce, vbuffer0, vbuffer1, vbuffer2, vbuffer3, vbuffer0_level1, _globalparams);
			synchronize(enablereduce, vbuffer4, vbuffer5, vbuffer6, vbuffer7, vbuffer1_level1, _globalparams);
			synchronize(enablereduce, vbuffer8, vbuffer9, vbuffer10, vbuffer11, vbuffer2_level1, _globalparams);
			synchronize(enablereduce, vbuffer12, vbuffer13, vbuffer14, vbuffer15, vbuffer3_level1, _globalparams);
			
			// I2
			vmask_p_temp[it] = synchronizeandapply(enablereduce, vbuffer0_level1, vbuffer1_level1, vbuffer2_level1, vbuffer3_level1, vbuffer0_level2, refbuffer, vmask0, it*VECTOR_SIZE, voffset_kvs, _globalparams);
			
			// I3
			spreadvdata(enablereduce, vbuffer0_level2, vbuffer0_level3, vbuffer1_level3, vbuffer2_level3, vbuffer3_level3, _globalparams);
			
			// I4
			spreadvdata(enablereduce, vbuffer0_level3, vbuffer0_level4, vbuffer1_level4, vbuffer2_level4, vbuffer3_level4, _globalparams);
			spreadvdata(enablereduce, vbuffer1_level3, vbuffer4_level4, vbuffer5_level4, vbuffer6_level4, vbuffer7_level4, _globalparams);
			spreadvdata(enablereduce, vbuffer2_level3, vbuffer8_level4, vbuffer9_level4, vbuffer10_level4, vbuffer11_level4, _globalparams);
			spreadvdata(enablereduce, vbuffer3_level3, vbuffer12_level4, vbuffer13_level4, vbuffer14_level4, vbuffer15_level4, _globalparams);
			
			// S
	
			savevdata(enablereduce, kvdram0, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer0_level4, 0, 0, reducebuffersz, _globalparams);
	
			savevdata(enablereduce, kvdram1, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer1_level4, 0, 0, reducebuffersz, _globalparams);
	
			savevdata(enablereduce, kvdram2, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer2_level4, 0, 0, reducebuffersz, _globalparams);
	
			savevdata(enablereduce, kvdram3, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer3_level4, 0, 0, reducebuffersz, _globalparams);
	
			savevdata(enablereduce, kvdram4, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer4_level4, 0, 0, reducebuffersz, _globalparams);
	
			savevdata(enablereduce, kvdram5, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer5_level4, 0, 0, reducebuffersz, _globalparams);
	
			savevdata(enablereduce, kvdram6, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer6_level4, 0, 0, reducebuffersz, _globalparams);
	
			savevdata(enablereduce, kvdram7, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer7_level4, 0, 0, reducebuffersz, _globalparams);
	
			savevdata(enablereduce, kvdram8, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer8_level4, 0, 0, reducebuffersz, _globalparams);
	
			savevdata(enablereduce, kvdram9, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer9_level4, 0, 0, reducebuffersz, _globalparams);
	
			savevdata(enablereduce, kvdram10, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer10_level4, 0, 0, reducebuffersz, _globalparams);
	
			savevdata(enablereduce, kvdram11, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer11_level4, 0, 0, reducebuffersz, _globalparams);
	
			savevdata(enablereduce, kvdram12, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer12_level4, 0, 0, reducebuffersz, _globalparams);
	
			savevdata(enablereduce, kvdram13, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer13_level4, 0, 0, reducebuffersz, _globalparams);
	
			savevdata(enablereduce, kvdram14, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer14_level4, 0, 0, reducebuffersz, _globalparams);
	
			savevdata(enablereduce, kvdram15, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer15_level4, 0, 0, reducebuffersz, _globalparams);
			
			vreadoffset_kvs += (reducebuffersz * 2) / 2;
		}
		
		// save mask
		#ifndef ALLVERTEXISACTIVE_ALGORITHM
		spreadvmask(enablereduce, vmask0, vmask0,vmask1,vmask2,vmask3,vmask4,vmask5,vmask6,vmask7,vmask8,vmask9,vmask10,vmask11,vmask12,vmask13,vmask14,vmask15, _globalparams);
		#endif
		#ifndef ALLVERTEXISACTIVE_ALGORITHM
	
		savevmasks(enablereduce, kvdram0, vmask0, vbuffer0, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs, _globalparams);
	
		savevmasks(enablereduce, kvdram1, vmask1, vbuffer1, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs, _globalparams);
	
		savevmasks(enablereduce, kvdram2, vmask2, vbuffer2, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs, _globalparams);
	
		savevmasks(enablereduce, kvdram3, vmask3, vbuffer3, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs, _globalparams);
	
		savevmasks(enablereduce, kvdram4, vmask4, vbuffer4, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs, _globalparams);
	
		savevmasks(enablereduce, kvdram5, vmask5, vbuffer5, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs, _globalparams);
	
		savevmasks(enablereduce, kvdram6, vmask6, vbuffer6, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs, _globalparams);
	
		savevmasks(enablereduce, kvdram7, vmask7, vbuffer7, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs, _globalparams);
	
		savevmasks(enablereduce, kvdram8, vmask8, vbuffer8, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs, _globalparams);
	
		savevmasks(enablereduce, kvdram9, vmask9, vbuffer9, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs, _globalparams);
	
		savevmasks(enablereduce, kvdram10, vmask10, vbuffer10, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs, _globalparams);
	
		savevmasks(enablereduce, kvdram11, vmask11, vbuffer11, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs, _globalparams);
	
		savevmasks(enablereduce, kvdram12, vmask12, vbuffer12, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs, _globalparams);
	
		savevmasks(enablereduce, kvdram13, vmask13, vbuffer13, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs, _globalparams);
	
		savevmasks(enablereduce, kvdram14, vmask14, vbuffer14, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs, _globalparams);
	
		savevmasks(enablereduce, kvdram15, vmask15, vbuffer15, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs, _globalparams);
	
		#endif
			
		vmask_p[source_partition] = vmask_p_temp[0] | vmask_p_temp[1]; // CRITICAL FIXME. INCORRECT.
		savevmaskp(kvdram0, source_partition, vmask_p[source_partition], _globalparams);
		savevmaskp(kvdram1, source_partition, vmask_p[source_partition], _globalparams);
		savevmaskp(kvdram2, source_partition, vmask_p[source_partition], _globalparams);
		savevmaskp(kvdram3, source_partition, vmask_p[source_partition], _globalparams);
		savevmaskp(kvdram4, source_partition, vmask_p[source_partition], _globalparams);
		savevmaskp(kvdram5, source_partition, vmask_p[source_partition], _globalparams);
		savevmaskp(kvdram6, source_partition, vmask_p[source_partition], _globalparams);
		savevmaskp(kvdram7, source_partition, vmask_p[source_partition], _globalparams);
		savevmaskp(kvdram8, source_partition, vmask_p[source_partition], _globalparams);
		savevmaskp(kvdram9, source_partition, vmask_p[source_partition], _globalparams);
		savevmaskp(kvdram10, source_partition, vmask_p[source_partition], _globalparams);
		savevmaskp(kvdram11, source_partition, vmask_p[source_partition], _globalparams);
		savevmaskp(kvdram12, source_partition, vmask_p[source_partition], _globalparams);
		savevmaskp(kvdram13, source_partition, vmask_p[source_partition], _globalparams);
		savevmaskp(kvdram14, source_partition, vmask_p[source_partition], _globalparams);
		savevmaskp(kvdram15, source_partition, vmask_p[source_partition], _globalparams);
		
		vmaskreadoffset_kvs += vmaskbuffersz_kvs;
		sourcestatsmarker += 1;
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
} */
extern "C" {
void 
	#ifdef SW 
	acts_synchronize:: 
	#endif
topkernel(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11,uint512_dt * kvdram12,uint512_dt * kvdram13,uint512_dt * kvdram14,uint512_dt * kvdram15){
 
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

	#ifdef _DEBUGMODE_KERNELPRINTS2
	cout<<">>> Light weight ACTS (NACTS_IN_NCOMPUTEUNITS.SYNC) Launched... "<<endl; 
	#endif
	
	keyvalue_vbuffer_t refbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = refbuffer
	
	travstate_t rtravstate[NUMCOMPUTEUNITS];
	#pragma HLS ARRAY_PARTITION variable=rtravstate complete
	
	keyvalue_vbuffer_t vbuffer0[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	keyvalue_vbuffer_t vbuffer1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	keyvalue_vbuffer_t vbuffer2[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer2
	keyvalue_vbuffer_t vbuffer3[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer3
	keyvalue_vbuffer_t vbuffer4[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer4
	keyvalue_vbuffer_t vbuffer5[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer5
	keyvalue_vbuffer_t vbuffer6[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer6
	keyvalue_vbuffer_t vbuffer7[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer7
	keyvalue_vbuffer_t vbuffer8[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer8
	keyvalue_vbuffer_t vbuffer9[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer9
	keyvalue_vbuffer_t vbuffer10[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer10
	keyvalue_vbuffer_t vbuffer11[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer11
	keyvalue_vbuffer_t vbuffer12[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer12
	keyvalue_vbuffer_t vbuffer13[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer13
	keyvalue_vbuffer_t vbuffer14[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer14
	keyvalue_vbuffer_t vbuffer15[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer15
	
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
	
	keyvalue_vbuffer_t vbuffer0_level4[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer0_level4
	keyvalue_vbuffer_t vbuffer1_level4[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer1_level4
	keyvalue_vbuffer_t vbuffer2_level4[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer2_level4
	keyvalue_vbuffer_t vbuffer3_level4[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer3_level4
	keyvalue_vbuffer_t vbuffer4_level4[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer4_level4
	keyvalue_vbuffer_t vbuffer5_level4[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer5_level4
	keyvalue_vbuffer_t vbuffer6_level4[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer6_level4
	keyvalue_vbuffer_t vbuffer7_level4[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer7_level4
	keyvalue_vbuffer_t vbuffer8_level4[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer8_level4
	keyvalue_vbuffer_t vbuffer9_level4[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer9_level4
	keyvalue_vbuffer_t vbuffer10_level4[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer10_level4
	keyvalue_vbuffer_t vbuffer11_level4[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer11_level4
	keyvalue_vbuffer_t vbuffer12_level4[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer12_level4
	keyvalue_vbuffer_t vbuffer13_level4[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer13_level4
	keyvalue_vbuffer_t vbuffer14_level4[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer14_level4
	keyvalue_vbuffer_t vbuffer15_level4[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer15_level4
	
	uintNUMPby2_type vmask0[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask0
	uint32_type vmask_p0[BLOCKRAM_SIZE];
	uintNUMPby2_type vmask1[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask1
	uint32_type vmask_p1[BLOCKRAM_SIZE];
	uintNUMPby2_type vmask2[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask2
	uint32_type vmask_p2[BLOCKRAM_SIZE];
	uintNUMPby2_type vmask3[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask3
	uint32_type vmask_p3[BLOCKRAM_SIZE];
	uintNUMPby2_type vmask4[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask4
	uint32_type vmask_p4[BLOCKRAM_SIZE];
	uintNUMPby2_type vmask5[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask5
	uint32_type vmask_p5[BLOCKRAM_SIZE];
	uintNUMPby2_type vmask6[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask6
	uint32_type vmask_p6[BLOCKRAM_SIZE];
	uintNUMPby2_type vmask7[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask7
	uint32_type vmask_p7[BLOCKRAM_SIZE];
	uintNUMPby2_type vmask8[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask8
	uint32_type vmask_p8[BLOCKRAM_SIZE];
	uintNUMPby2_type vmask9[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask9
	uint32_type vmask_p9[BLOCKRAM_SIZE];
	uintNUMPby2_type vmask10[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask10
	uint32_type vmask_p10[BLOCKRAM_SIZE];
	uintNUMPby2_type vmask11[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask11
	uint32_type vmask_p11[BLOCKRAM_SIZE];
	uintNUMPby2_type vmask12[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask12
	uint32_type vmask_p12[BLOCKRAM_SIZE];
	uintNUMPby2_type vmask13[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask13
	uint32_type vmask_p13[BLOCKRAM_SIZE];
	uintNUMPby2_type vmask14[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask14
	uint32_type vmask_p14[BLOCKRAM_SIZE];
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
	
	uint32_type vmask_p_temp[2];
	vmask_p_temp[0] = 0; 
	vmask_p_temp[1] = 0;

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
	
	unsigned int vreadoffset_kvs = 0;
	unsigned int vmaskreadoffset_kvs = 0;
	buffer_type reducebuffersz = _globalparams.SIZE_REDUCE / 2; // 512
	buffer_type vmaskbuffersz_kvs = (_globalparams.SIZE_REDUCE * NUM_PARTITIONS) / 512; // 32
	
	step_type currentLOP = _globalparams.ACTSPARAMS_TREEDEPTH;
	batch_type num_source_partitions = get_num_source_partitions(currentLOP);

	for(unsigned int k=0; k<num_source_partitions; k++){ vmask_p[k] = 0; }
	
	bool_type enablereduce;
	
	bool_type it0en = OFF;
	bool_type it1en = OFF;

	TOPKERNELSYNC_MAINLOOP: for(batch_type source_partition=0; source_partition<num_source_partitions; source_partition+=1){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loop1 avg=analysis_loop1
	
		// bool_type enablereduce = ON;
		// unsigned int ntravszs = 0;
		// 		// rtravstate[0] = gettravstate(ON, kvdram0, globalparams[0], currentLOP, sourcestatsmarker);
		// 		// rtravstate[1] = gettravstate(ON, kvdram1, globalparams[1], currentLOP, sourcestatsmarker);
		// 		// rtravstate[2] = gettravstate(ON, kvdram2, globalparams[2], currentLOP, sourcestatsmarker);
		// 		// rtravstate[3] = gettravstate(ON, kvdram3, globalparams[3], currentLOP, sourcestatsmarker);
		// 		// rtravstate[4] = gettravstate(ON, kvdram4, globalparams[4], currentLOP, sourcestatsmarker);
		// 		// rtravstate[5] = gettravstate(ON, kvdram5, globalparams[5], currentLOP, sourcestatsmarker);
		// 		// rtravstate[6] = gettravstate(ON, kvdram6, globalparams[6], currentLOP, sourcestatsmarker);
		// 		// rtravstate[7] = gettravstate(ON, kvdram7, globalparams[7], currentLOP, sourcestatsmarker);
		// 		// rtravstate[8] = gettravstate(ON, kvdram8, globalparams[8], currentLOP, sourcestatsmarker);
		// 		// rtravstate[9] = gettravstate(ON, kvdram9, globalparams[9], currentLOP, sourcestatsmarker);
		// 		// rtravstate[10] = gettravstate(ON, kvdram10, globalparams[10], currentLOP, sourcestatsmarker);
		// 		// rtravstate[11] = gettravstate(ON, kvdram11, globalparams[11], currentLOP, sourcestatsmarker);
		// 		// rtravstate[12] = gettravstate(ON, kvdram12, globalparams[12], currentLOP, sourcestatsmarker);
		// 		// rtravstate[13] = gettravstate(ON, kvdram13, globalparams[13], currentLOP, sourcestatsmarker);
		// 		// rtravstate[14] = gettravstate(ON, kvdram14, globalparams[14], currentLOP, sourcestatsmarker);
		// 		// rtravstate[15] = gettravstate(ON, kvdram15, globalparams[15], currentLOP, sourcestatsmarker);
		// 		// for(unsigned int i = 0; i < NUMCOMPUTEUNITS; i++){ ntravszs += rtravstate[i].size_kvs; }
		// if(ntravszs > 0){ enablereduce = ON; } else { enablereduce = OFF; }
		
		// #ifndef ALLVERTEXISACTIVE_ALGORITHM
		// 		// resetvmask(vmask0);
		// 		// resetvmask(vmask1);
		// 		// resetvmask(vmask2);
		// 		// resetvmask(vmask3);
		// 		// resetvmask(vmask4);
		// 		// resetvmask(vmask5);
		// 		// resetvmask(vmask6);
		// 		// resetvmask(vmask7);
		// 		// resetvmask(vmask8);
		// 		// resetvmask(vmask9);
		// 		// resetvmask(vmask10);
		// 		// resetvmask(vmask11);
		// 		// resetvmask(vmask12);
		// 		// resetvmask(vmask13);
		// 		// resetvmask(vmask14);
		// 		// resetvmask(vmask15);
		// 		// #endif
	
		// vmask_p_temp[0] = 0; 
		// vmask_p_temp[1] = 0;
		
		TOPKERNELSYNC_MAINLOOP1B: for(batch_type it=0; it<2; it+=1){
			#ifdef _DEBUGMODE_KERNELPRINTS
			actsutilityobj->print3("### topkernel:: source_partition", "currentLOP", "NAp", source_partition, currentLOP, NAp); 							
			#endif
			
			if(it==0){ it0en = ON; } else { it0en = OFF; }
			if(it==1){ it1en = ON; } else { it1en = OFF; }
			batch_type voffset_kvs = (source_partition * reducebuffersz * FETFACTOR) + (it * reducebuffersz);
			if(it0en==ON){ vmask_p_temp[0] = 0; vmask_p_temp[1] = 0; }
			
			if(it0en==ON){
			enablereduce = ON; // CRITICAL FIXME
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
			}
			
			#ifndef ALLVERTEXISACTIVE_ALGORITHM
			resetvmask(it0en, vmask0);
			resetvmask(it0en, vmask1);
			resetvmask(it0en, vmask2);
			resetvmask(it0en, vmask3);
			resetvmask(it0en, vmask4);
			resetvmask(it0en, vmask5);
			resetvmask(it0en, vmask6);
			resetvmask(it0en, vmask7);
			resetvmask(it0en, vmask8);
			resetvmask(it0en, vmask9);
			resetvmask(it0en, vmask10);
			resetvmask(it0en, vmask11);
			resetvmask(it0en, vmask12);
			resetvmask(it0en, vmask13);
			resetvmask(it0en, vmask14);
			resetvmask(it0en, vmask15);
			#endif
			
			// if(it0en==ON){ vmask_p_temp[0] = 0; vmask_p_temp[1] = 0; }
			
			// batch_type voffset_kvs = (source_partition * reducebuffersz * FETFACTOR) + (it * reducebuffersz);
			
			// R
	
			readvdata(enablereduce, kvdram0, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer0, 0, 0, refbuffer, 0, reducebuffersz, _globalparams);
	
			readvdata(enablereduce, kvdram1, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer1, 0, 0,  reducebuffersz, _globalparams);
	
			readvdata(enablereduce, kvdram2, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer2, 0, 0,  reducebuffersz, _globalparams);
	
			readvdata(enablereduce, kvdram3, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer3, 0, 0,  reducebuffersz, _globalparams);
	
			readvdata(enablereduce, kvdram4, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer4, 0, 0,  reducebuffersz, _globalparams);
	
			readvdata(enablereduce, kvdram5, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer5, 0, 0,  reducebuffersz, _globalparams);
	
			readvdata(enablereduce, kvdram6, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer6, 0, 0,  reducebuffersz, _globalparams);
	
			readvdata(enablereduce, kvdram7, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer7, 0, 0,  reducebuffersz, _globalparams);
	
			readvdata(enablereduce, kvdram8, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer8, 0, 0,  reducebuffersz, _globalparams);
	
			readvdata(enablereduce, kvdram9, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer9, 0, 0,  reducebuffersz, _globalparams);
	
			readvdata(enablereduce, kvdram10, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer10, 0, 0,  reducebuffersz, _globalparams);
	
			readvdata(enablereduce, kvdram11, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer11, 0, 0,  reducebuffersz, _globalparams);
	
			readvdata(enablereduce, kvdram12, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer12, 0, 0,  reducebuffersz, _globalparams);
	
			readvdata(enablereduce, kvdram13, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer13, 0, 0,  reducebuffersz, _globalparams);
	
			readvdata(enablereduce, kvdram14, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer14, 0, 0,  reducebuffersz, _globalparams);
	
			readvdata(enablereduce, kvdram15, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer15, 0, 0,  reducebuffersz, _globalparams);
			
			// I1
			synchronize(enablereduce, vbuffer0, vbuffer1, vbuffer2, vbuffer3, vbuffer0_level1, _globalparams);
			synchronize(enablereduce, vbuffer4, vbuffer5, vbuffer6, vbuffer7, vbuffer1_level1, _globalparams);
			synchronize(enablereduce, vbuffer8, vbuffer9, vbuffer10, vbuffer11, vbuffer2_level1, _globalparams);
			synchronize(enablereduce, vbuffer12, vbuffer13, vbuffer14, vbuffer15, vbuffer3_level1, _globalparams);
			
			// I2
			vmask_p_temp[it] = synchronizeandapply(enablereduce, vbuffer0_level1, vbuffer1_level1, vbuffer2_level1, vbuffer3_level1, vbuffer0_level2, refbuffer, vmask0, it*VECTOR_SIZE, voffset_kvs, _globalparams);
			
			// I3
			spreadvdata(enablereduce, vbuffer0_level2, vbuffer0_level3, vbuffer1_level3, vbuffer2_level3, vbuffer3_level3, _globalparams);
			#ifndef ALLVERTEXISACTIVE_ALGORITHM
			spreadvmask(enablereduce, it1en, vmask0, vmask0,vmask1,vmask2,vmask3,vmask4,vmask5,vmask6,vmask7,vmask8,vmask9,vmask10,vmask11,vmask12,vmask13,vmask14,vmask15, _globalparams);
			#endif
		
			// I4
			spreadvdata(enablereduce, vbuffer0_level3, vbuffer0_level4, vbuffer1_level4, vbuffer2_level4, vbuffer3_level4, _globalparams);
			spreadvdata(enablereduce, vbuffer1_level3, vbuffer4_level4, vbuffer5_level4, vbuffer6_level4, vbuffer7_level4, _globalparams);
			spreadvdata(enablereduce, vbuffer2_level3, vbuffer8_level4, vbuffer9_level4, vbuffer10_level4, vbuffer11_level4, _globalparams);
			spreadvdata(enablereduce, vbuffer3_level3, vbuffer12_level4, vbuffer13_level4, vbuffer14_level4, vbuffer15_level4, _globalparams);
			
			// S
	
			savevdata(enablereduce, kvdram0, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer0_level4, 0, 0, reducebuffersz, _globalparams);
	
			savevdata(enablereduce, kvdram1, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer1_level4, 0, 0, reducebuffersz, _globalparams);
	
			savevdata(enablereduce, kvdram2, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer2_level4, 0, 0, reducebuffersz, _globalparams);
	
			savevdata(enablereduce, kvdram3, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer3_level4, 0, 0, reducebuffersz, _globalparams);
	
			savevdata(enablereduce, kvdram4, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer4_level4, 0, 0, reducebuffersz, _globalparams);
	
			savevdata(enablereduce, kvdram5, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer5_level4, 0, 0, reducebuffersz, _globalparams);
	
			savevdata(enablereduce, kvdram6, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer6_level4, 0, 0, reducebuffersz, _globalparams);
	
			savevdata(enablereduce, kvdram7, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer7_level4, 0, 0, reducebuffersz, _globalparams);
	
			savevdata(enablereduce, kvdram8, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer8_level4, 0, 0, reducebuffersz, _globalparams);
	
			savevdata(enablereduce, kvdram9, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer9_level4, 0, 0, reducebuffersz, _globalparams);
	
			savevdata(enablereduce, kvdram10, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer10_level4, 0, 0, reducebuffersz, _globalparams);
	
			savevdata(enablereduce, kvdram11, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer11_level4, 0, 0, reducebuffersz, _globalparams);
	
			savevdata(enablereduce, kvdram12, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer12_level4, 0, 0, reducebuffersz, _globalparams);
	
			savevdata(enablereduce, kvdram13, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer13_level4, 0, 0, reducebuffersz, _globalparams);
	
			savevdata(enablereduce, kvdram14, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer14_level4, 0, 0, reducebuffersz, _globalparams);
	
			savevdata(enablereduce, kvdram15, _globalparams.BASEOFFSETKVS_VERTICESDATA + vreadoffset_kvs, vbuffer15_level4, 0, 0, reducebuffersz, _globalparams);
			#ifndef ALLVERTEXISACTIVE_ALGORITHM
	
			savevmasks(enablereduce, it1en, kvdram0, vmask0, vbuffer0, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs, _globalparams);
	
			savevmasks(enablereduce, it1en, kvdram1, vmask1, vbuffer1, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs, _globalparams);
	
			savevmasks(enablereduce, it1en, kvdram2, vmask2, vbuffer2, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs, _globalparams);
	
			savevmasks(enablereduce, it1en, kvdram3, vmask3, vbuffer3, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs, _globalparams);
	
			savevmasks(enablereduce, it1en, kvdram4, vmask4, vbuffer4, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs, _globalparams);
	
			savevmasks(enablereduce, it1en, kvdram5, vmask5, vbuffer5, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs, _globalparams);
	
			savevmasks(enablereduce, it1en, kvdram6, vmask6, vbuffer6, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs, _globalparams);
	
			savevmasks(enablereduce, it1en, kvdram7, vmask7, vbuffer7, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs, _globalparams);
	
			savevmasks(enablereduce, it1en, kvdram8, vmask8, vbuffer8, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs, _globalparams);
	
			savevmasks(enablereduce, it1en, kvdram9, vmask9, vbuffer9, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs, _globalparams);
	
			savevmasks(enablereduce, it1en, kvdram10, vmask10, vbuffer10, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs, _globalparams);
	
			savevmasks(enablereduce, it1en, kvdram11, vmask11, vbuffer11, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs, _globalparams);
	
			savevmasks(enablereduce, it1en, kvdram12, vmask12, vbuffer12, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs, _globalparams);
	
			savevmasks(enablereduce, it1en, kvdram13, vmask13, vbuffer13, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs, _globalparams);
	
			savevmasks(enablereduce, it1en, kvdram14, vmask14, vbuffer14, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs, _globalparams);
	
			savevmasks(enablereduce, it1en, kvdram15, vmask15, vbuffer15, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs, _globalparams);
			#endif
			if(it1en==ON){ vmask_p[source_partition] = vmask_p_temp[0] | vmask_p_temp[1]; }// CRITICAL FIXME. INCORRECT.
			savevmaskp(it1en, kvdram0, source_partition, vmask_p[source_partition], _globalparams);
			savevmaskp(it1en, kvdram1, source_partition, vmask_p[source_partition], _globalparams);
			savevmaskp(it1en, kvdram2, source_partition, vmask_p[source_partition], _globalparams);
			savevmaskp(it1en, kvdram3, source_partition, vmask_p[source_partition], _globalparams);
			savevmaskp(it1en, kvdram4, source_partition, vmask_p[source_partition], _globalparams);
			savevmaskp(it1en, kvdram5, source_partition, vmask_p[source_partition], _globalparams);
			savevmaskp(it1en, kvdram6, source_partition, vmask_p[source_partition], _globalparams);
			savevmaskp(it1en, kvdram7, source_partition, vmask_p[source_partition], _globalparams);
			savevmaskp(it1en, kvdram8, source_partition, vmask_p[source_partition], _globalparams);
			savevmaskp(it1en, kvdram9, source_partition, vmask_p[source_partition], _globalparams);
			savevmaskp(it1en, kvdram10, source_partition, vmask_p[source_partition], _globalparams);
			savevmaskp(it1en, kvdram11, source_partition, vmask_p[source_partition], _globalparams);
			savevmaskp(it1en, kvdram12, source_partition, vmask_p[source_partition], _globalparams);
			savevmaskp(it1en, kvdram13, source_partition, vmask_p[source_partition], _globalparams);
			savevmaskp(it1en, kvdram14, source_partition, vmask_p[source_partition], _globalparams);
			savevmaskp(it1en, kvdram15, source_partition, vmask_p[source_partition], _globalparams);
			
			vreadoffset_kvs += (reducebuffersz * 2) / 2;
			if(it1en==ON){ vmaskreadoffset_kvs += vmaskbuffersz_kvs; sourcestatsmarker += 1; }
		}
		
		// save mask
		// #ifndef ALLVERTEXISACTIVE_ALGORITHM
		// spreadvmask(enablereduce, vmask0, vmask0,vmask1,vmask2,vmask3,vmask4,vmask5,vmask6,vmask7,vmask8,vmask9,vmask10,vmask11,vmask12,vmask13,vmask14,vmask15, _globalparams);
		// #endif
		// #ifndef ALLVERTEXISACTIVE_ALGORITHM
		// 	
		// savevmasks(enablereduce, kvdram0, vmask0, vbuffer0, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs, _globalparams);
		// 	
		// savevmasks(enablereduce, kvdram1, vmask1, vbuffer1, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs, _globalparams);
		// 	
		// savevmasks(enablereduce, kvdram2, vmask2, vbuffer2, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs, _globalparams);
		// 	
		// savevmasks(enablereduce, kvdram3, vmask3, vbuffer3, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs, _globalparams);
		// 	
		// savevmasks(enablereduce, kvdram4, vmask4, vbuffer4, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs, _globalparams);
		// 	
		// savevmasks(enablereduce, kvdram5, vmask5, vbuffer5, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs, _globalparams);
		// 	
		// savevmasks(enablereduce, kvdram6, vmask6, vbuffer6, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs, _globalparams);
		// 	
		// savevmasks(enablereduce, kvdram7, vmask7, vbuffer7, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs, _globalparams);
		// 	
		// savevmasks(enablereduce, kvdram8, vmask8, vbuffer8, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs, _globalparams);
		// 	
		// savevmasks(enablereduce, kvdram9, vmask9, vbuffer9, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs, _globalparams);
		// 	
		// savevmasks(enablereduce, kvdram10, vmask10, vbuffer10, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs, _globalparams);
		// 	
		// savevmasks(enablereduce, kvdram11, vmask11, vbuffer11, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs, _globalparams);
		// 	
		// savevmasks(enablereduce, kvdram12, vmask12, vbuffer12, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs, _globalparams);
		// 	
		// savevmasks(enablereduce, kvdram13, vmask13, vbuffer13, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs, _globalparams);
		// 	
		// savevmasks(enablereduce, kvdram14, vmask14, vbuffer14, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs, _globalparams);
		// 	
		// savevmasks(enablereduce, kvdram15, vmask15, vbuffer15, _globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs, vmaskbuffersz_kvs, _globalparams);
		// 	
		// #endif
			
		// vmask_p[source_partition] = vmask_p_temp[0] | vmask_p_temp[1]; // CRITICAL FIXME. INCORRECT.
		// 		// savevmaskp(kvdram0, source_partition, vmask_p[source_partition], _globalparams);
		// 		// savevmaskp(kvdram1, source_partition, vmask_p[source_partition], _globalparams);
		// 		// savevmaskp(kvdram2, source_partition, vmask_p[source_partition], _globalparams);
		// 		// savevmaskp(kvdram3, source_partition, vmask_p[source_partition], _globalparams);
		// 		// savevmaskp(kvdram4, source_partition, vmask_p[source_partition], _globalparams);
		// 		// savevmaskp(kvdram5, source_partition, vmask_p[source_partition], _globalparams);
		// 		// savevmaskp(kvdram6, source_partition, vmask_p[source_partition], _globalparams);
		// 		// savevmaskp(kvdram7, source_partition, vmask_p[source_partition], _globalparams);
		// 		// savevmaskp(kvdram8, source_partition, vmask_p[source_partition], _globalparams);
		// 		// savevmaskp(kvdram9, source_partition, vmask_p[source_partition], _globalparams);
		// 		// savevmaskp(kvdram10, source_partition, vmask_p[source_partition], _globalparams);
		// 		// savevmaskp(kvdram11, source_partition, vmask_p[source_partition], _globalparams);
		// 		// savevmaskp(kvdram12, source_partition, vmask_p[source_partition], _globalparams);
		// 		// savevmaskp(kvdram13, source_partition, vmask_p[source_partition], _globalparams);
		// 		// savevmaskp(kvdram14, source_partition, vmask_p[source_partition], _globalparams);
		// 		// savevmaskp(kvdram15, source_partition, vmask_p[source_partition], _globalparams);
		// 		
		// vmaskreadoffset_kvs += vmaskbuffersz_kvs;
		// sourcestatsmarker += 1;
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







