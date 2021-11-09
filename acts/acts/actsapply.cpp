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
#include "../../acts/actsutility/actsutility.h"
#include "actsapply.h"
using namespace std;

#ifdef SW
actsapply::actsapply(){ actsutilityobj = new actsutility(); }
actsapply::~actsapply(){}
#endif
#ifdef FPGA_IMPL
actsutility * actsutilityobj = new actsutility();
#endif 

unsigned int
	#ifdef SW 
	actsapply::
	#endif 
SYNC_amin(unsigned int val1, unsigned int val2){
	if(val1 < val2){ return val1; }
	else { return val2; }
}
value_t 
	#ifdef SW 
	actsapply::
	#endif 
mergefunc(value_t value1, value_t value2, unsigned int GraphAlgo){
	return SYNC_amin(value1, value2);
}
value_t 
	#ifdef SW 
	actsapply::
	#endif 
applyfunc(value_t value1, value_t value2, unsigned int GraphAlgo){
	return SYNC_amin(value1, value2);
}
globalparams_t 
	#ifdef SW 
	actsapply::
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
	globalparams.BASEOFFSETKVS_SRCVERTICESDATA = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_SRCVERTICESDATA].range(31, 0);
	globalparams.BASEOFFSETKVS_DESTVERTICESDATA = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_DESTVERTICESDATA].range(31, 0);
	globalparams.BASEOFFSETKVS_ACTIVEVERTICES = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_ACTIVEVERTICES].range(31, 0);
	globalparams.BASEOFFSETKVS_VERTICESDATAMASK = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESDATAMASK].range(31, 0);
	globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESPARTITIONMASK].range(31, 0);
	globalparams.BASEOFFSETKVS_STATSDRAM = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_STATSDRAM].range(31, 0);
	globalparams.BASEOFFSETKVS_EDGESSTATSDRAM = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_EDGESSTATSDRAM].range(31, 0); //
	globalparams.BASEOFFSETKVS_KVDRAM = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_KVDRAM].range(31, 0);
	globalparams.BASEOFFSETKVS_KVDRAMWORKSPACE = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_KVDRAMWORKSPACE].range(31, 0);
	
	globalparams.SIZE_MESSAGESDRAM = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_MESSAGESDATA].range(31, 0);
	globalparams.SIZE_EDGES = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_EDGES].range(31, 0);
	globalparams.SIZE_VERTEXPTRS = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_VERTEXPTRS].range(31, 0);
	globalparams.SIZE_SRCVERTICESDATA = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_SRCVERTICESDATA].range(31, 0);
	globalparams.SIZE_DESTVERTICESDATA = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_DESTVERTICESDATA].range(31, 0);
	globalparams.SIZE_ACTIVEVERTICES = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_ACTIVEVERTICES].range(31, 0);
	globalparams.SIZE_VERTICESDATAMASK = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_VERTICESDATAMASK].range(31, 0);
	globalparams.SIZE_VERTICESPARTITIONMASK = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_VERTICESPARTITIONMASK].range(31, 0);
	globalparams.SIZE_KVSTATSDRAM = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_KVSTATSDRAM].range(31, 0);
	globalparams.SIZE_EDGESSTATSDRAM = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_EDGESSTATSDRAM].range(31, 0); //
	globalparams.SIZE_KVDRAM = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_KVDRAM].range(31, 0);
	globalparams.SIZE_KVDRAMWORKSPACE = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_KVDRAMWORKSPACE].range(31, 0);
	globalparams.SIZE_REDUCE = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_REDUCE].range(31, 0);
	globalparams.SIZE_BATCHRANGE = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_BATCHRANGE].range(31, 0);
	globalparams.SIZE_RUN = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN].range(31, 0);

	globalparams.POW_MESSAGESDRAM = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_MESSAGESDRAM].range(31, 0);
	globalparams.POW_EDGES = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_EDGES].range(31, 0);
	globalparams.POW_VERTEXPTRS = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_VERTEXPTRS].range(31, 0);
	globalparams.POW_SRCVERTICESDATA = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_SRCVERTICESDATA].range(31, 0);
	globalparams.POW_DESTVERTICESDATA = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_DESTVERTICESDATA].range(31, 0);
	globalparams.POW_ACTIVEVERTICES = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_ACTIVEVERTICES].range(31, 0);
	globalparams.POW_VERTICESDATAMASK = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_VERTICESDATAMASK].range(31, 0);
	globalparams.POW_KVSTATSDRAM = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_KVSTATSDRAM].range(31, 0);
	globalparams.POW_EDGESSTATSDRAM = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_EDGESSTATSDRAM].range(31, 0); //
	globalparams.POW_KVDRAM = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_KVDRAM].range(31, 0);
	globalparams.POW_KVDRAMWORKSPACE = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_KVDRAMWORKSPACE].range(31, 0);
	globalparams.POW_REDUCE = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_REDUCE].range(31, 0);
	globalparams.POW_BATCHRANGE = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_BATCHRANGE].range(31, 0);
	
	globalparams.ALGORITHMINFO_GRAPHITERATIONID = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) - 1; // NEWCHANGE. actsproc.retrievekvdramstats has incremented it
	globalparams.ALGORITHMINFO_GRAPHALGORITHMID = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHALGORITHMID].range(31, 0);

	globalparams.ACTSPARAMS_BEGINLOP = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_BEGINLOP].range(31, 0);
	globalparams.ACTSPARAMS_NUMLOPS = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_NUMLOPS].range(31, 0);
	globalparams.ACTSPARAMS_TREEDEPTH = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_TREEDEPTH].range(31, 0);
	globalparams.ACTSPARAMS_FINALNUMPARTITIONS = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_FINALNUMPARTITIONS].range(31, 0);
	globalparams.ACTSPARAMS_SRCVOFFSET = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_SRCVOFFSET].range(31, 0);
	globalparams.ACTSPARAMS_SRCVSIZE = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_SRCVSIZE].range(31, 0);
	globalparams.ACTSPARAMS_DESTVOFFSET = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_DESTVOFFSET].range(31, 0);
	globalparams.ACTSPARAMS_NUMEDGECHUNKSINABUFFER = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_NUMEDGECHUNKSINABUFFER].range(31, 0);
	
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
	globalparams.BASEOFFSETKVS_SRCVERTICESDATA = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_SRCVERTICESDATA].data[0].key;
	globalparams.BASEOFFSETKVS_DESTVERTICESDATA = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_DESTVERTICESDATA].data[0].key;
	globalparams.BASEOFFSETKVS_ACTIVEVERTICES = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_ACTIVEVERTICES].data[0].key;
	globalparams.BASEOFFSETKVS_VERTICESDATAMASK = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESDATAMASK].data[0].key;
	globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESPARTITIONMASK].data[0].key;
	globalparams.BASEOFFSETKVS_STATSDRAM = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_STATSDRAM].data[0].key;
	globalparams.BASEOFFSETKVS_EDGESSTATSDRAM = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_EDGESSTATSDRAM].data[0].key;//
	globalparams.BASEOFFSETKVS_KVDRAM = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_KVDRAM].data[0].key;
	globalparams.BASEOFFSETKVS_KVDRAMWORKSPACE = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_KVDRAMWORKSPACE].data[0].key;
	
	globalparams.SIZE_MESSAGESDRAM = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_MESSAGESDATA].data[0].key;
	globalparams.SIZE_EDGES = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_EDGES].data[0].key;
	globalparams.SIZE_VERTEXPTRS = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_VERTEXPTRS].data[0].key;
	globalparams.SIZE_SRCVERTICESDATA = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_SRCVERTICESDATA].data[0].key;
	globalparams.SIZE_DESTVERTICESDATA = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_DESTVERTICESDATA].data[0].key;
	globalparams.SIZE_ACTIVEVERTICES = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_ACTIVEVERTICES].data[0].key;
	globalparams.SIZE_VERTICESDATAMASK = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_VERTICESDATAMASK].data[0].key;
	globalparams.SIZE_VERTICESPARTITIONMASK = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_VERTICESPARTITIONMASK].data[0].key;
	globalparams.SIZE_KVSTATSDRAM = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_KVSTATSDRAM].data[0].key;
	globalparams.SIZE_EDGESSTATSDRAM = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_EDGESSTATSDRAM].data[0].key;//
	globalparams.SIZE_KVDRAM = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_KVDRAM].data[0].key;
	globalparams.SIZE_KVDRAMWORKSPACE = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_KVDRAMWORKSPACE].data[0].key;
	globalparams.SIZE_REDUCE = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_REDUCE].data[0].key;
	globalparams.SIZE_BATCHRANGE = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_BATCHRANGE].data[0].key;
	globalparams.SIZE_RUN = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN].data[0].key;

	globalparams.POW_MESSAGESDRAM = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_MESSAGESDRAM].data[0].key;
	globalparams.POW_EDGES = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_EDGES].data[0].key;
	globalparams.POW_VERTEXPTRS = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_VERTEXPTRS].data[0].key;
	globalparams.POW_SRCVERTICESDATA = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_SRCVERTICESDATA].data[0].key;
	globalparams.POW_DESTVERTICESDATA = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_DESTVERTICESDATA].data[0].key;
	globalparams.POW_ACTIVEVERTICES = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_ACTIVEVERTICES].data[0].key;
	globalparams.POW_VERTICESDATAMASK = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_VERTICESDATAMASK].data[0].key;
	globalparams.POW_KVSTATSDRAM = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_KVSTATSDRAM].data[0].key;
	globalparams.POW_EDGESSTATSDRAM = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_EDGESSTATSDRAM].data[0].key;//
	globalparams.POW_KVDRAM = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_KVDRAM].data[0].key;
	globalparams.POW_KVDRAMWORKSPACE = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_KVDRAMWORKSPACE].data[0].key;
	globalparams.POW_REDUCE = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_REDUCE].data[0].key;
	globalparams.POW_BATCHRANGE = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_BATCHRANGE].data[0].key;
	
	globalparams.ALGORITHMINFO_GRAPHITERATIONID = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key - 1; // NEWCHANGE. actsproc.retrievekvdramstats has incremented it
	globalparams.ALGORITHMINFO_GRAPHALGORITHMID = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHALGORITHMID].data[0].key;

	globalparams.ACTSPARAMS_BEGINLOP = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_BEGINLOP].data[0].key;
	globalparams.ACTSPARAMS_NUMLOPS = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_NUMLOPS].data[0].key;
	globalparams.ACTSPARAMS_TREEDEPTH = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_TREEDEPTH].data[0].key;
	globalparams.ACTSPARAMS_FINALNUMPARTITIONS = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_FINALNUMPARTITIONS].data[0].key;
	globalparams.ACTSPARAMS_SRCVOFFSET = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_SRCVOFFSET].data[0].key;
	globalparams.ACTSPARAMS_SRCVSIZE = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_SRCVSIZE].data[0].key;
	globalparams.ACTSPARAMS_DESTVOFFSET = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_DESTVOFFSET].data[0].key;
	globalparams.ACTSPARAMS_NUMEDGECHUNKSINABUFFER = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_NUMEDGECHUNKSINABUFFER].data[0].key;
	
	globalparams.RETURN_RETURNVALUES = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURN_RETURNVALUES].data[0].key;
	#endif  
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printglobalparameters("actssync::SYNC_getglobalparams:: printing global parameters", globalparams);
	#endif
	return globalparams;
}

extern "C" {
void
	#if defined(SW)
	actsapply::
	#endif
topkernelapply(uint512_dt * kvdram0, uint512_dt * kvdram1, uint512_dt * kvdram2, uint512_dt * kvdram3, uint512_dt * vdram){
// #pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
// #pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
// #pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
// #pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem3
// #pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem0

#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram3 bundle = control
#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = kvdram1
#pragma HLS DATA_PACK variable = kvdram2
#pragma HLS DATA_PACK variable = kvdram3
#pragma HLS DATA_PACK variable = vdram

	keyvalue_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = buffer
	globalparams_t globalparams = SYNC_getglobalparams(kvdram0);
	unsigned int dramoffset_kvs = globalparams.BASEOFFSETKVS_DESTVERTICESDATA;
	
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
	keyvalue_t refkeyvalue0;
	keyvalue_t refkeyvalue1;
	keyvalue_t refkeyvalue2;
	keyvalue_t refkeyvalue3;
	keyvalue_t refkeyvalue4;
	keyvalue_t refkeyvalue5;
	keyvalue_t refkeyvalue6;
	keyvalue_t refkeyvalue7;

	READANDMERGE_LOOP1: for (buffer_type i=0; i<(globalparams.SIZE_DESTVERTICESDATA / 2); i++){
		READANDMERGE_LOOP1A: for (buffer_type j=0; j<BLOCKRAM_SIZE; j++){
		#pragma HLS PIPELINE II=1
			#ifdef _WIDEWORD
			mykeyvalue00.key = kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(31, 0); 
			mykeyvalue00.value = kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(63, 32); 
			mykeyvalue01.key = kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(95, 64); 
			mykeyvalue01.value = kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(127, 96); 
			mykeyvalue02.key = kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(159, 128); 
			mykeyvalue02.value = kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(191, 160); 
			mykeyvalue03.key = kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(223, 192); 
			mykeyvalue03.value = kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(255, 224); 
			mykeyvalue04.key = kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(287, 256); 
			mykeyvalue04.value = kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(319, 288); 
			mykeyvalue05.key = kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(351, 320); 
			mykeyvalue05.value = kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(383, 352); 
			mykeyvalue06.key = kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(415, 384); 
			mykeyvalue06.value = kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(447, 416); 
			mykeyvalue07.key = kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(479, 448); 
			mykeyvalue07.value = kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(511, 480); 
			mykeyvalue10.key = kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(31, 0); 
			mykeyvalue10.value = kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(63, 32); 
			mykeyvalue11.key = kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(95, 64); 
			mykeyvalue11.value = kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(127, 96); 
			mykeyvalue12.key = kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(159, 128); 
			mykeyvalue12.value = kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(191, 160); 
			mykeyvalue13.key = kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(223, 192); 
			mykeyvalue13.value = kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(255, 224); 
			mykeyvalue14.key = kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(287, 256); 
			mykeyvalue14.value = kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(319, 288); 
			mykeyvalue15.key = kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(351, 320); 
			mykeyvalue15.value = kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(383, 352); 
			mykeyvalue16.key = kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(415, 384); 
			mykeyvalue16.value = kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(447, 416); 
			mykeyvalue17.key = kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(479, 448); 
			mykeyvalue17.value = kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(511, 480); 
			mykeyvalue20.key = kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(31, 0); 
			mykeyvalue20.value = kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(63, 32); 
			mykeyvalue21.key = kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(95, 64); 
			mykeyvalue21.value = kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(127, 96); 
			mykeyvalue22.key = kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(159, 128); 
			mykeyvalue22.value = kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(191, 160); 
			mykeyvalue23.key = kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(223, 192); 
			mykeyvalue23.value = kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(255, 224); 
			mykeyvalue24.key = kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(287, 256); 
			mykeyvalue24.value = kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(319, 288); 
			mykeyvalue25.key = kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(351, 320); 
			mykeyvalue25.value = kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(383, 352); 
			mykeyvalue26.key = kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(415, 384); 
			mykeyvalue26.value = kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(447, 416); 
			mykeyvalue27.key = kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(479, 448); 
			mykeyvalue27.value = kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(511, 480); 
			mykeyvalue30.key = kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(31, 0); 
			mykeyvalue30.value = kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(63, 32); 
			mykeyvalue31.key = kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(95, 64); 
			mykeyvalue31.value = kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(127, 96); 
			mykeyvalue32.key = kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(159, 128); 
			mykeyvalue32.value = kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(191, 160); 
			mykeyvalue33.key = kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(223, 192); 
			mykeyvalue33.value = kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(255, 224); 
			mykeyvalue34.key = kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(287, 256); 
			mykeyvalue34.value = kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(319, 288); 
			mykeyvalue35.key = kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(351, 320); 
			mykeyvalue35.value = kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(383, 352); 
			mykeyvalue36.key = kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(415, 384); 
			mykeyvalue36.value = kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(447, 416); 
			mykeyvalue37.key = kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(479, 448); 
			mykeyvalue37.value = kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(511, 480); 
			#else 
			mykeyvalue00.key = kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[0].key; 
			mykeyvalue00.value = kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[0].value; 
			mykeyvalue01.key = kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[1].key; 
			mykeyvalue01.value = kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[1].value; 
			mykeyvalue02.key = kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[2].key; 
			mykeyvalue02.value = kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[2].value; 
			mykeyvalue03.key = kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[3].key; 
			mykeyvalue03.value = kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[3].value; 
			mykeyvalue04.key = kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[4].key; 
			mykeyvalue04.value = kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[4].value; 
			mykeyvalue05.key = kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[5].key; 
			mykeyvalue05.value = kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[5].value; 
			mykeyvalue06.key = kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[6].key; 
			mykeyvalue06.value = kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[6].value; 
			mykeyvalue07.key = kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[7].key; 
			mykeyvalue07.value = kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[7].value; 
			mykeyvalue10.key = kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[0].key; 
			mykeyvalue10.value = kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[0].value; 
			mykeyvalue11.key = kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[1].key; 
			mykeyvalue11.value = kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[1].value; 
			mykeyvalue12.key = kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[2].key; 
			mykeyvalue12.value = kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[2].value; 
			mykeyvalue13.key = kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[3].key; 
			mykeyvalue13.value = kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[3].value; 
			mykeyvalue14.key = kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[4].key; 
			mykeyvalue14.value = kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[4].value; 
			mykeyvalue15.key = kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[5].key; 
			mykeyvalue15.value = kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[5].value; 
			mykeyvalue16.key = kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[6].key; 
			mykeyvalue16.value = kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[6].value; 
			mykeyvalue17.key = kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[7].key; 
			mykeyvalue17.value = kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[7].value; 
			mykeyvalue20.key = kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[0].key; 
			mykeyvalue20.value = kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[0].value; 
			mykeyvalue21.key = kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[1].key; 
			mykeyvalue21.value = kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[1].value; 
			mykeyvalue22.key = kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[2].key; 
			mykeyvalue22.value = kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[2].value; 
			mykeyvalue23.key = kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[3].key; 
			mykeyvalue23.value = kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[3].value; 
			mykeyvalue24.key = kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[4].key; 
			mykeyvalue24.value = kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[4].value; 
			mykeyvalue25.key = kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[5].key; 
			mykeyvalue25.value = kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[5].value; 
			mykeyvalue26.key = kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[6].key; 
			mykeyvalue26.value = kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[6].value; 
			mykeyvalue27.key = kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[7].key; 
			mykeyvalue27.value = kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[7].value; 
			mykeyvalue30.key = kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[0].key; 
			mykeyvalue30.value = kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[0].value; 
			mykeyvalue31.key = kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[1].key; 
			mykeyvalue31.value = kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[1].value; 
			mykeyvalue32.key = kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[2].key; 
			mykeyvalue32.value = kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[2].value; 
			mykeyvalue33.key = kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[3].key; 
			mykeyvalue33.value = kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[3].value; 
			mykeyvalue34.key = kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[4].key; 
			mykeyvalue34.value = kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[4].value; 
			mykeyvalue35.key = kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[5].key; 
			mykeyvalue35.value = kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[5].value; 
			mykeyvalue36.key = kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[6].key; 
			mykeyvalue36.value = kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[6].value; 
			mykeyvalue37.key = kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[7].key; 
			mykeyvalue37.value = kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[7].value; 
			#endif
			refkeyvalue0.key = vdram[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(31, 0); 
			refkeyvalue0.value = vdram[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(63, 32); 
			refkeyvalue1.key = vdram[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(95, 64); 
			refkeyvalue1.value = vdram[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(127, 96); 
			refkeyvalue2.key = vdram[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(159, 128); 
			refkeyvalue2.value = vdram[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(191, 160); 
			refkeyvalue3.key = vdram[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(223, 192); 
			refkeyvalue3.value = vdram[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(255, 224); 
			refkeyvalue4.key = vdram[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(287, 256); 
			refkeyvalue4.value = vdram[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(319, 288); 
			refkeyvalue5.key = vdram[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(351, 320); 
			refkeyvalue5.value = vdram[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(383, 352); 
			refkeyvalue6.key = vdram[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(415, 384); 
			refkeyvalue6.value = vdram[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(447, 416); 
			refkeyvalue7.key = vdram[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(479, 448); 
			refkeyvalue7.value = vdram[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(511, 480); 
			
			value_t keyA0 = mergefunc(mykeyvalue00.key, mykeyvalue10.key, NAp);
			value_t keyB0 = mergefunc(mykeyvalue20.key, mykeyvalue30.key, NAp);
			value_t keyC0 = mergefunc(keyA0, keyB0, NAp);
			value_t keyA1 = mergefunc(mykeyvalue01.key, mykeyvalue11.key, NAp);
			value_t keyB1 = mergefunc(mykeyvalue21.key, mykeyvalue31.key, NAp);
			value_t keyC1 = mergefunc(keyA1, keyB1, NAp);
			value_t keyA2 = mergefunc(mykeyvalue02.key, mykeyvalue12.key, NAp);
			value_t keyB2 = mergefunc(mykeyvalue22.key, mykeyvalue32.key, NAp);
			value_t keyC2 = mergefunc(keyA2, keyB2, NAp);
			value_t keyA3 = mergefunc(mykeyvalue03.key, mykeyvalue13.key, NAp);
			value_t keyB3 = mergefunc(mykeyvalue23.key, mykeyvalue33.key, NAp);
			value_t keyC3 = mergefunc(keyA3, keyB3, NAp);
			value_t keyA4 = mergefunc(mykeyvalue04.key, mykeyvalue14.key, NAp);
			value_t keyB4 = mergefunc(mykeyvalue24.key, mykeyvalue34.key, NAp);
			value_t keyC4 = mergefunc(keyA4, keyB4, NAp);
			value_t keyA5 = mergefunc(mykeyvalue05.key, mykeyvalue15.key, NAp);
			value_t keyB5 = mergefunc(mykeyvalue25.key, mykeyvalue35.key, NAp);
			value_t keyC5 = mergefunc(keyA5, keyB5, NAp);
			value_t keyA6 = mergefunc(mykeyvalue06.key, mykeyvalue16.key, NAp);
			value_t keyB6 = mergefunc(mykeyvalue26.key, mykeyvalue36.key, NAp);
			value_t keyC6 = mergefunc(keyA6, keyB6, NAp);
			value_t keyA7 = mergefunc(mykeyvalue07.key, mykeyvalue17.key, NAp);
			value_t keyB7 = mergefunc(mykeyvalue27.key, mykeyvalue37.key, NAp);
			value_t keyC7 = mergefunc(keyA7, keyB7, NAp);
			
			value_t valA0 = mergefunc(mykeyvalue00.value, mykeyvalue10.value, NAp);
			value_t valB0 = mergefunc(mykeyvalue20.value, mykeyvalue30.value, NAp);
			value_t valC0 = mergefunc(valA0, valB0, NAp);
			value_t valA1 = mergefunc(mykeyvalue01.value, mykeyvalue11.value, NAp);
			value_t valB1 = mergefunc(mykeyvalue21.value, mykeyvalue31.value, NAp);
			value_t valC1 = mergefunc(valA1, valB1, NAp);
			value_t valA2 = mergefunc(mykeyvalue02.value, mykeyvalue12.value, NAp);
			value_t valB2 = mergefunc(mykeyvalue22.value, mykeyvalue32.value, NAp);
			value_t valC2 = mergefunc(valA2, valB2, NAp);
			value_t valA3 = mergefunc(mykeyvalue03.value, mykeyvalue13.value, NAp);
			value_t valB3 = mergefunc(mykeyvalue23.value, mykeyvalue33.value, NAp);
			value_t valC3 = mergefunc(valA3, valB3, NAp);
			value_t valA4 = mergefunc(mykeyvalue04.value, mykeyvalue14.value, NAp);
			value_t valB4 = mergefunc(mykeyvalue24.value, mykeyvalue34.value, NAp);
			value_t valC4 = mergefunc(valA4, valB4, NAp);
			value_t valA5 = mergefunc(mykeyvalue05.value, mykeyvalue15.value, NAp);
			value_t valB5 = mergefunc(mykeyvalue25.value, mykeyvalue35.value, NAp);
			value_t valC5 = mergefunc(valA5, valB5, NAp);
			value_t valA6 = mergefunc(mykeyvalue06.value, mykeyvalue16.value, NAp);
			value_t valB6 = mergefunc(mykeyvalue26.value, mykeyvalue36.value, NAp);
			value_t valC6 = mergefunc(valA6, valB6, NAp);
			value_t valA7 = mergefunc(mykeyvalue07.value, mykeyvalue17.value, NAp);
			value_t valB7 = mergefunc(mykeyvalue27.value, mykeyvalue37.value, NAp);
			value_t valC7 = mergefunc(valA7, valB7, NAp);
			
			buffer[dramoffset_kvs + j][0].key = applyfunc(keyC0, refkeyvalue0.key, NAp);
			buffer[dramoffset_kvs + j][0].value = applyfunc(valC0, refkeyvalue0.value, NAp);
			buffer[dramoffset_kvs + j][1].key = applyfunc(keyC1, refkeyvalue1.key, NAp);
			buffer[dramoffset_kvs + j][1].value = applyfunc(valC1, refkeyvalue1.value, NAp);
			buffer[dramoffset_kvs + j][2].key = applyfunc(keyC2, refkeyvalue2.key, NAp);
			buffer[dramoffset_kvs + j][2].value = applyfunc(valC2, refkeyvalue2.value, NAp);
			buffer[dramoffset_kvs + j][3].key = applyfunc(keyC3, refkeyvalue3.key, NAp);
			buffer[dramoffset_kvs + j][3].value = applyfunc(valC3, refkeyvalue3.value, NAp);
			buffer[dramoffset_kvs + j][4].key = applyfunc(keyC4, refkeyvalue4.key, NAp);
			buffer[dramoffset_kvs + j][4].value = applyfunc(valC4, refkeyvalue4.value, NAp);
			buffer[dramoffset_kvs + j][5].key = applyfunc(keyC5, refkeyvalue5.key, NAp);
			buffer[dramoffset_kvs + j][5].value = applyfunc(valC5, refkeyvalue5.value, NAp);
			buffer[dramoffset_kvs + j][6].key = applyfunc(keyC6, refkeyvalue6.key, NAp);
			buffer[dramoffset_kvs + j][6].value = applyfunc(valC6, refkeyvalue6.value, NAp);
			buffer[dramoffset_kvs + j][7].key = applyfunc(keyC7, refkeyvalue7.key, NAp);
			buffer[dramoffset_kvs + j][7].value = applyfunc(valC7, refkeyvalue7.value, NAp);
		}
		
		READANDMERGE_LOOP1B: for (buffer_type j=0; j<BLOCKRAM_SIZE; j++){
		#pragma HLS PIPELINE II=1
			#ifdef _WIDEWORD
			kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(31, 0) = buffer[dramoffset_kvs + j][0].key;
			kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(63, 32) = buffer[dramoffset_kvs + j][0].value;
			kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(95, 64) = buffer[dramoffset_kvs + j][1].key;
			kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(127, 96) = buffer[dramoffset_kvs + j][1].value;
			kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(159, 128) = buffer[dramoffset_kvs + j][2].key;
			kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(191, 160) = buffer[dramoffset_kvs + j][2].value;
			kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(223, 192) = buffer[dramoffset_kvs + j][3].key;
			kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(255, 224) = buffer[dramoffset_kvs + j][3].value;
			kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(287, 256) = buffer[dramoffset_kvs + j][4].key;
			kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(319, 288) = buffer[dramoffset_kvs + j][4].value;
			kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(351, 320) = buffer[dramoffset_kvs + j][5].key;
			kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(383, 352) = buffer[dramoffset_kvs + j][5].value;
			kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(415, 384) = buffer[dramoffset_kvs + j][6].key;
			kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(447, 416) = buffer[dramoffset_kvs + j][6].value;
			kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(479, 448) = buffer[dramoffset_kvs + j][7].key;
			kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(511, 480) = buffer[dramoffset_kvs + j][7].value;
			kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(31, 0) = buffer[dramoffset_kvs + j][0].key;
			kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(63, 32) = buffer[dramoffset_kvs + j][0].value;
			kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(95, 64) = buffer[dramoffset_kvs + j][1].key;
			kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(127, 96) = buffer[dramoffset_kvs + j][1].value;
			kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(159, 128) = buffer[dramoffset_kvs + j][2].key;
			kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(191, 160) = buffer[dramoffset_kvs + j][2].value;
			kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(223, 192) = buffer[dramoffset_kvs + j][3].key;
			kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(255, 224) = buffer[dramoffset_kvs + j][3].value;
			kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(287, 256) = buffer[dramoffset_kvs + j][4].key;
			kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(319, 288) = buffer[dramoffset_kvs + j][4].value;
			kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(351, 320) = buffer[dramoffset_kvs + j][5].key;
			kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(383, 352) = buffer[dramoffset_kvs + j][5].value;
			kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(415, 384) = buffer[dramoffset_kvs + j][6].key;
			kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(447, 416) = buffer[dramoffset_kvs + j][6].value;
			kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(479, 448) = buffer[dramoffset_kvs + j][7].key;
			kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(511, 480) = buffer[dramoffset_kvs + j][7].value;
			kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(31, 0) = buffer[dramoffset_kvs + j][0].key;
			kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(63, 32) = buffer[dramoffset_kvs + j][0].value;
			kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(95, 64) = buffer[dramoffset_kvs + j][1].key;
			kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(127, 96) = buffer[dramoffset_kvs + j][1].value;
			kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(159, 128) = buffer[dramoffset_kvs + j][2].key;
			kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(191, 160) = buffer[dramoffset_kvs + j][2].value;
			kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(223, 192) = buffer[dramoffset_kvs + j][3].key;
			kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(255, 224) = buffer[dramoffset_kvs + j][3].value;
			kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(287, 256) = buffer[dramoffset_kvs + j][4].key;
			kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(319, 288) = buffer[dramoffset_kvs + j][4].value;
			kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(351, 320) = buffer[dramoffset_kvs + j][5].key;
			kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(383, 352) = buffer[dramoffset_kvs + j][5].value;
			kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(415, 384) = buffer[dramoffset_kvs + j][6].key;
			kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(447, 416) = buffer[dramoffset_kvs + j][6].value;
			kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(479, 448) = buffer[dramoffset_kvs + j][7].key;
			kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(511, 480) = buffer[dramoffset_kvs + j][7].value;
			kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(31, 0) = buffer[dramoffset_kvs + j][0].key;
			kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(63, 32) = buffer[dramoffset_kvs + j][0].value;
			kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(95, 64) = buffer[dramoffset_kvs + j][1].key;
			kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(127, 96) = buffer[dramoffset_kvs + j][1].value;
			kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(159, 128) = buffer[dramoffset_kvs + j][2].key;
			kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(191, 160) = buffer[dramoffset_kvs + j][2].value;
			kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(223, 192) = buffer[dramoffset_kvs + j][3].key;
			kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(255, 224) = buffer[dramoffset_kvs + j][3].value;
			kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(287, 256) = buffer[dramoffset_kvs + j][4].key;
			kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(319, 288) = buffer[dramoffset_kvs + j][4].value;
			kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(351, 320) = buffer[dramoffset_kvs + j][5].key;
			kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(383, 352) = buffer[dramoffset_kvs + j][5].value;
			kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(415, 384) = buffer[dramoffset_kvs + j][6].key;
			kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(447, 416) = buffer[dramoffset_kvs + j][6].value;
			kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(479, 448) = buffer[dramoffset_kvs + j][7].key;
			kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].range(511, 480) = buffer[dramoffset_kvs + j][7].value;
			#else 
			kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[0].key = buffer[dramoffset_kvs + j][0].key;
			kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[0].value = buffer[dramoffset_kvs + j][0].value;
			kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[1].key = buffer[dramoffset_kvs + j][1].key;
			kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[1].value = buffer[dramoffset_kvs + j][1].value;
			kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[2].key = buffer[dramoffset_kvs + j][2].key;
			kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[2].value = buffer[dramoffset_kvs + j][2].value;
			kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[3].key = buffer[dramoffset_kvs + j][3].key;
			kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[3].value = buffer[dramoffset_kvs + j][3].value;
			kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[4].key = buffer[dramoffset_kvs + j][4].key;
			kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[4].value = buffer[dramoffset_kvs + j][4].value;
			kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[5].key = buffer[dramoffset_kvs + j][5].key;
			kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[5].value = buffer[dramoffset_kvs + j][5].value;
			kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[6].key = buffer[dramoffset_kvs + j][6].key;
			kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[6].value = buffer[dramoffset_kvs + j][6].value;
			kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[7].key = buffer[dramoffset_kvs + j][7].key;
			kvdram0[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[7].value = buffer[dramoffset_kvs + j][7].value;
			kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[0].key = buffer[dramoffset_kvs + j][0].key;
			kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[0].value = buffer[dramoffset_kvs + j][0].value;
			kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[1].key = buffer[dramoffset_kvs + j][1].key;
			kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[1].value = buffer[dramoffset_kvs + j][1].value;
			kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[2].key = buffer[dramoffset_kvs + j][2].key;
			kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[2].value = buffer[dramoffset_kvs + j][2].value;
			kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[3].key = buffer[dramoffset_kvs + j][3].key;
			kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[3].value = buffer[dramoffset_kvs + j][3].value;
			kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[4].key = buffer[dramoffset_kvs + j][4].key;
			kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[4].value = buffer[dramoffset_kvs + j][4].value;
			kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[5].key = buffer[dramoffset_kvs + j][5].key;
			kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[5].value = buffer[dramoffset_kvs + j][5].value;
			kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[6].key = buffer[dramoffset_kvs + j][6].key;
			kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[6].value = buffer[dramoffset_kvs + j][6].value;
			kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[7].key = buffer[dramoffset_kvs + j][7].key;
			kvdram1[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[7].value = buffer[dramoffset_kvs + j][7].value;
			kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[0].key = buffer[dramoffset_kvs + j][0].key;
			kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[0].value = buffer[dramoffset_kvs + j][0].value;
			kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[1].key = buffer[dramoffset_kvs + j][1].key;
			kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[1].value = buffer[dramoffset_kvs + j][1].value;
			kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[2].key = buffer[dramoffset_kvs + j][2].key;
			kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[2].value = buffer[dramoffset_kvs + j][2].value;
			kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[3].key = buffer[dramoffset_kvs + j][3].key;
			kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[3].value = buffer[dramoffset_kvs + j][3].value;
			kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[4].key = buffer[dramoffset_kvs + j][4].key;
			kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[4].value = buffer[dramoffset_kvs + j][4].value;
			kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[5].key = buffer[dramoffset_kvs + j][5].key;
			kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[5].value = buffer[dramoffset_kvs + j][5].value;
			kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[6].key = buffer[dramoffset_kvs + j][6].key;
			kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[6].value = buffer[dramoffset_kvs + j][6].value;
			kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[7].key = buffer[dramoffset_kvs + j][7].key;
			kvdram2[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[7].value = buffer[dramoffset_kvs + j][7].value;
			kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[0].key = buffer[dramoffset_kvs + j][0].key;
			kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[0].value = buffer[dramoffset_kvs + j][0].value;
			kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[1].key = buffer[dramoffset_kvs + j][1].key;
			kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[1].value = buffer[dramoffset_kvs + j][1].value;
			kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[2].key = buffer[dramoffset_kvs + j][2].key;
			kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[2].value = buffer[dramoffset_kvs + j][2].value;
			kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[3].key = buffer[dramoffset_kvs + j][3].key;
			kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[3].value = buffer[dramoffset_kvs + j][3].value;
			kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[4].key = buffer[dramoffset_kvs + j][4].key;
			kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[4].value = buffer[dramoffset_kvs + j][4].value;
			kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[5].key = buffer[dramoffset_kvs + j][5].key;
			kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[5].value = buffer[dramoffset_kvs + j][5].value;
			kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[6].key = buffer[dramoffset_kvs + j][6].key;
			kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[6].value = buffer[dramoffset_kvs + j][6].value;
			kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[7].key = buffer[dramoffset_kvs + j][7].key;
			kvdram3[dramoffset_kvs + i*BLOCKRAM_SIZE + j].data[7].value = buffer[dramoffset_kvs + j][7].value;
			#endif
		}
	}
	return;
}
}





