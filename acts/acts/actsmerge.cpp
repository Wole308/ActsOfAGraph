#ifdef SW 
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
#endif 
#include "../../include/config_params.h"
#include "../../include/common.h"
#include "../include/actscommon.h"
#ifndef HW
#include "../../src/utility/utility.h"
#endif 
#ifndef HW
#include "../../acts/actsutility/actsutility.h"
#endif 
#include "actsmerge.h"
using namespace std;

#ifdef SW
actsmerge::actsmerge(){ actsutilityobj = new actsutility(); }
actsmerge::~actsmerge(){}
#endif
#ifdef FPGA_IMPL
// actsutility * actsutilityobj = new actsutility();
#endif 

// functions (bit manipulation)
unsigned int 
	#ifdef SW 
	actsmerge::
	#endif
SYNC_GETMASK_UINT(unsigned int index, unsigned int size){
	unsigned int A = ((1 << (size)) - 1);
	unsigned int B = A << index;
	return B;
}
void
	#ifdef SW 
	actsmerge::
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
uint32_type 
	#ifdef SW 
	actsmerge::
	#endif 
CONVERTVMASKTOUINT32(unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unsigned int index){
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

unsigned int
	#ifdef SW 
	actsmerge::
	#endif 
SYNC_amin(unsigned int val1, unsigned int val2){
	if(val1 < val2){ return val1; }
	else { return val2; }
}
value_t 
	#ifdef SW 
	actsmerge::
	#endif 
mergefunc(value_t value1, value_t value2, unsigned int GraphAlgo){
	return SYNC_amin(value1, value2);
}
value_t 
	#ifdef SW 
	actsmerge::
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
globalparams_t 
	#ifdef SW 
	actsmerge::
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
	
	globalparams.ALGORITHMINFO_GRAPHITERATIONID = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key - 1; // CRITICAL NEWCHANGE. actsproc.retrievekvdramstats has incremented it
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
	actsutilityobj->printglobalparameters("actsmerge::SYNC_getglobalparams:: printing global parameters", globalparams);
	#endif
	return globalparams;
}
batch_type
	#ifdef SW 
	actsmerge::
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

extern "C" {
void
	#if defined(SW)
	actsmerge::
	#endif
topkernelsync(uint512_dt * kvdram0, uint512_dt * kvdram1, uint512_dt * kvdram2, uint512_dt * vdram){
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem3

#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = kvdram1
#pragma HLS DATA_PACK variable = kvdram2
#pragma HLS DATA_PACK variable = vdram

	keyvalue_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = buffer
	unsigned int vid[NUM_PARTITIONS][2];
	#pragma HLS ARRAY_PARTITION variable=vid complete
	
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
	keyvalue_t udata0;
	keyvalue_t udata1;
	keyvalue_t udata2;
	keyvalue_t udata3;
	keyvalue_t udata4;
	keyvalue_t udata5;
	keyvalue_t udata6;
	keyvalue_t udata7;
	keyy_t keyRES0;
	value_t valueRES0;
	keyy_t keyRES1;
	value_t valueRES1;
	keyy_t keyRES2;
	value_t valueRES2;
	keyy_t keyRES3;
	value_t valueRES3;
	keyy_t keyRES4;
	value_t valueRES4;
	keyy_t keyRES5;
	value_t valueRES5;
	keyy_t keyRES6;
	value_t valueRES6;
	keyy_t keyRES7;
	value_t valueRES7;
	
	unitBRAMwidth_type vmask[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask
	uint32_type tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE]; // prepare vmask variables
	#pragma HLS array_partition variable = tempbuffer
	uint32_type cummvmask_sp[2];
	#pragma HLS ARRAY_PARTITION variable=vid complete
	
	globalparams_t globalparamsk = getglobalparams(kvdram0);
	globalparams_t globalparamsv = getglobalparams(vdram);
	#ifdef ENABLERECURSIVEPARTITIONING
	step_type currentLOP = globalparamsv.ACTSPARAMS_TREEDEPTH;
	batch_type num_source_partitions = get_num_source_partitions(currentLOP);
	#else 
	step_type currentLOP = globalparamsv.ACTSPARAMS_TREEDEPTH + 1;	
	batch_type num_source_partitions = NUM_PARTITIONS;
	#endif
	
	unsigned int vreadoffset_kvs = 0;
	unsigned int vmaskreadoffset_kvs = 0;
	buffer_type reducebuffersz = globalparamsv.SIZE_REDUCE / 2; // 512
	buffer_type vmaskbuffersz_kvs = (globalparamsv.SIZE_REDUCE * VDATA_PACKINGSIZE) / 512;
	unsigned int colindex = 0;
	cummvmask_sp[0] = 0; cummvmask_sp[1] = 0;
	
	MERGE_LOOP1: for(batch_type source_partition=0; source_partition<num_source_partitions; source_partition+=1){
		// cout<<"actsmerge: --------------------------------- source_partition: "<<source_partition<<endl;
		if(source_partition % 2 == 0){ colindex = 0; } else { colindex = 8; };
		cummvmask_sp[0] = 0; cummvmask_sp[1] = 0;
		
		READANDMERGE_LOOP1: for (buffer_type i=0; i<reducebuffersz; i++){
		#pragma HLS PIPELINE II=1
			#ifdef _WIDEWORD
			mykeyvalue00.key = kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(31, 0); 
			mykeyvalue00.value = kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(63, 32); 
			mykeyvalue01.key = kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(95, 64); 
			mykeyvalue01.value = kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(127, 96); 
			mykeyvalue02.key = kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(159, 128); 
			mykeyvalue02.value = kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(191, 160); 
			mykeyvalue03.key = kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(223, 192); 
			mykeyvalue03.value = kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(255, 224); 
			mykeyvalue04.key = kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(287, 256); 
			mykeyvalue04.value = kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(319, 288); 
			mykeyvalue05.key = kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(351, 320); 
			mykeyvalue05.value = kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(383, 352); 
			mykeyvalue06.key = kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(415, 384); 
			mykeyvalue06.value = kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(447, 416); 
			mykeyvalue07.key = kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(479, 448); 
			mykeyvalue07.value = kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(511, 480); 
			mykeyvalue10.key = kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(31, 0); 
			mykeyvalue10.value = kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(63, 32); 
			mykeyvalue11.key = kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(95, 64); 
			mykeyvalue11.value = kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(127, 96); 
			mykeyvalue12.key = kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(159, 128); 
			mykeyvalue12.value = kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(191, 160); 
			mykeyvalue13.key = kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(223, 192); 
			mykeyvalue13.value = kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(255, 224); 
			mykeyvalue14.key = kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(287, 256); 
			mykeyvalue14.value = kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(319, 288); 
			mykeyvalue15.key = kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(351, 320); 
			mykeyvalue15.value = kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(383, 352); 
			mykeyvalue16.key = kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(415, 384); 
			mykeyvalue16.value = kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(447, 416); 
			mykeyvalue17.key = kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(479, 448); 
			mykeyvalue17.value = kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(511, 480); 
			mykeyvalue20.key = kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(31, 0); 
			mykeyvalue20.value = kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(63, 32); 
			mykeyvalue21.key = kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(95, 64); 
			mykeyvalue21.value = kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(127, 96); 
			mykeyvalue22.key = kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(159, 128); 
			mykeyvalue22.value = kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(191, 160); 
			mykeyvalue23.key = kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(223, 192); 
			mykeyvalue23.value = kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(255, 224); 
			mykeyvalue24.key = kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(287, 256); 
			mykeyvalue24.value = kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(319, 288); 
			mykeyvalue25.key = kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(351, 320); 
			mykeyvalue25.value = kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(383, 352); 
			mykeyvalue26.key = kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(415, 384); 
			mykeyvalue26.value = kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(447, 416); 
			mykeyvalue27.key = kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(479, 448); 
			mykeyvalue27.value = kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(511, 480); 
			#else 
			mykeyvalue00.key = kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[0].key; 
			mykeyvalue00.value = kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[0].value; 
			mykeyvalue01.key = kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[1].key; 
			mykeyvalue01.value = kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[1].value; 
			mykeyvalue02.key = kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[2].key; 
			mykeyvalue02.value = kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[2].value; 
			mykeyvalue03.key = kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[3].key; 
			mykeyvalue03.value = kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[3].value; 
			mykeyvalue04.key = kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[4].key; 
			mykeyvalue04.value = kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[4].value; 
			mykeyvalue05.key = kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[5].key; 
			mykeyvalue05.value = kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[5].value; 
			mykeyvalue06.key = kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[6].key; 
			mykeyvalue06.value = kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[6].value; 
			mykeyvalue07.key = kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[7].key; 
			mykeyvalue07.value = kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[7].value; 
			mykeyvalue10.key = kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[0].key; 
			mykeyvalue10.value = kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[0].value; 
			mykeyvalue11.key = kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[1].key; 
			mykeyvalue11.value = kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[1].value; 
			mykeyvalue12.key = kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[2].key; 
			mykeyvalue12.value = kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[2].value; 
			mykeyvalue13.key = kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[3].key; 
			mykeyvalue13.value = kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[3].value; 
			mykeyvalue14.key = kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[4].key; 
			mykeyvalue14.value = kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[4].value; 
			mykeyvalue15.key = kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[5].key; 
			mykeyvalue15.value = kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[5].value; 
			mykeyvalue16.key = kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[6].key; 
			mykeyvalue16.value = kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[6].value; 
			mykeyvalue17.key = kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[7].key; 
			mykeyvalue17.value = kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[7].value; 
			mykeyvalue20.key = kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[0].key; 
			mykeyvalue20.value = kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[0].value; 
			mykeyvalue21.key = kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[1].key; 
			mykeyvalue21.value = kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[1].value; 
			mykeyvalue22.key = kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[2].key; 
			mykeyvalue22.value = kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[2].value; 
			mykeyvalue23.key = kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[3].key; 
			mykeyvalue23.value = kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[3].value; 
			mykeyvalue24.key = kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[4].key; 
			mykeyvalue24.value = kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[4].value; 
			mykeyvalue25.key = kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[5].key; 
			mykeyvalue25.value = kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[5].value; 
			mykeyvalue26.key = kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[6].key; 
			mykeyvalue26.value = kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[6].value; 
			mykeyvalue27.key = kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[7].key; 
			mykeyvalue27.value = kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[7].value; 
			#endif
			
			#ifdef _WIDEWORD
			udata0.key = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(31, 0); 
			udata0.value = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(63, 32); 
			udata1.key = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(95, 64); 
			udata1.value = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(127, 96); 
			udata2.key = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(159, 128); 
			udata2.value = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(191, 160); 
			udata3.key = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(223, 192); 
			udata3.value = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(255, 224); 
			udata4.key = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(287, 256); 
			udata4.value = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(319, 288); 
			udata5.key = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(351, 320); 
			udata5.value = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(383, 352); 
			udata6.key = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(415, 384); 
			udata6.value = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(447, 416); 
			udata7.key = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(479, 448); 
			udata7.value = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(511, 480); 
			#else 
			udata0.key = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[0].key; 
			udata0.value = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[0].value; 
			udata1.key = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[1].key; 
			udata1.value = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[1].value; 
			udata2.key = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[2].key; 
			udata2.value = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[2].value; 
			udata3.key = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[3].key; 
			udata3.value = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[3].value; 
			udata4.key = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[4].key; 
			udata4.value = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[4].value; 
			udata5.key = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[5].key; 
			udata5.value = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[5].value; 
			udata6.key = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[6].key; 
			udata6.value = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[6].value; 
			udata7.key = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[7].key; 
			udata7.value = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[7].value; 
	
			#endif 
			
			value_t keyA0 = mergefunc(mykeyvalue00.key, mykeyvalue10.key, NAp);
			value_t keyB0 = mergefunc(mykeyvalue20.key, 0xFFFFFFFF, NAp);
			value_t keyC0 = mergefunc(keyA0, keyB0, NAp);
			value_t keyA1 = mergefunc(mykeyvalue01.key, mykeyvalue11.key, NAp);
			value_t keyB1 = mergefunc(mykeyvalue21.key, 0xFFFFFFFF, NAp);
			value_t keyC1 = mergefunc(keyA1, keyB1, NAp);
			value_t keyA2 = mergefunc(mykeyvalue02.key, mykeyvalue12.key, NAp);
			value_t keyB2 = mergefunc(mykeyvalue22.key, 0xFFFFFFFF, NAp);
			value_t keyC2 = mergefunc(keyA2, keyB2, NAp);
			value_t keyA3 = mergefunc(mykeyvalue03.key, mykeyvalue13.key, NAp);
			value_t keyB3 = mergefunc(mykeyvalue23.key, 0xFFFFFFFF, NAp);
			value_t keyC3 = mergefunc(keyA3, keyB3, NAp);
			value_t keyA4 = mergefunc(mykeyvalue04.key, mykeyvalue14.key, NAp);
			value_t keyB4 = mergefunc(mykeyvalue24.key, 0xFFFFFFFF, NAp);
			value_t keyC4 = mergefunc(keyA4, keyB4, NAp);
			value_t keyA5 = mergefunc(mykeyvalue05.key, mykeyvalue15.key, NAp);
			value_t keyB5 = mergefunc(mykeyvalue25.key, 0xFFFFFFFF, NAp);
			value_t keyC5 = mergefunc(keyA5, keyB5, NAp);
			value_t keyA6 = mergefunc(mykeyvalue06.key, mykeyvalue16.key, NAp);
			value_t keyB6 = mergefunc(mykeyvalue26.key, 0xFFFFFFFF, NAp);
			value_t keyC6 = mergefunc(keyA6, keyB6, NAp);
			value_t keyA7 = mergefunc(mykeyvalue07.key, mykeyvalue17.key, NAp);
			value_t keyB7 = mergefunc(mykeyvalue27.key, 0xFFFFFFFF, NAp);
			value_t keyC7 = mergefunc(keyA7, keyB7, NAp);
			
			value_t valA0 = mergefunc(mykeyvalue00.value, mykeyvalue10.value, NAp);
			value_t valB0 = mergefunc(mykeyvalue20.value, 0xFFFFFFFF, NAp);
			value_t valC0 = mergefunc(valA0, valB0, NAp);
			value_t valA1 = mergefunc(mykeyvalue01.value, mykeyvalue11.value, NAp);
			value_t valB1 = mergefunc(mykeyvalue21.value, 0xFFFFFFFF, NAp);
			value_t valC1 = mergefunc(valA1, valB1, NAp);
			value_t valA2 = mergefunc(mykeyvalue02.value, mykeyvalue12.value, NAp);
			value_t valB2 = mergefunc(mykeyvalue22.value, 0xFFFFFFFF, NAp);
			value_t valC2 = mergefunc(valA2, valB2, NAp);
			value_t valA3 = mergefunc(mykeyvalue03.value, mykeyvalue13.value, NAp);
			value_t valB3 = mergefunc(mykeyvalue23.value, 0xFFFFFFFF, NAp);
			value_t valC3 = mergefunc(valA3, valB3, NAp);
			value_t valA4 = mergefunc(mykeyvalue04.value, mykeyvalue14.value, NAp);
			value_t valB4 = mergefunc(mykeyvalue24.value, 0xFFFFFFFF, NAp);
			value_t valC4 = mergefunc(valA4, valB4, NAp);
			value_t valA5 = mergefunc(mykeyvalue05.value, mykeyvalue15.value, NAp);
			value_t valB5 = mergefunc(mykeyvalue25.value, 0xFFFFFFFF, NAp);
			value_t valC5 = mergefunc(valA5, valB5, NAp);
			value_t valA6 = mergefunc(mykeyvalue06.value, mykeyvalue16.value, NAp);
			value_t valB6 = mergefunc(mykeyvalue26.value, 0xFFFFFFFF, NAp);
			value_t valC6 = mergefunc(valA6, valB6, NAp);
			value_t valA7 = mergefunc(mykeyvalue07.value, mykeyvalue17.value, NAp);
			value_t valB7 = mergefunc(mykeyvalue27.value, 0xFFFFFFFF, NAp);
			value_t valC7 = mergefunc(valA7, valB7, NAp);
		
			keyRES0 = applyfunc(udata0.key, keyC0, globalparamsv.ALGORITHMINFO_GRAPHITERATIONID, globalparamsv.ALGORITHMINFO_GRAPHALGORITHMID);
			keyRES1 = applyfunc(udata1.key, keyC1, globalparamsv.ALGORITHMINFO_GRAPHITERATIONID, globalparamsv.ALGORITHMINFO_GRAPHALGORITHMID);
			keyRES2 = applyfunc(udata2.key, keyC2, globalparamsv.ALGORITHMINFO_GRAPHITERATIONID, globalparamsv.ALGORITHMINFO_GRAPHALGORITHMID);
			keyRES3 = applyfunc(udata3.key, keyC3, globalparamsv.ALGORITHMINFO_GRAPHITERATIONID, globalparamsv.ALGORITHMINFO_GRAPHALGORITHMID);
			keyRES4 = applyfunc(udata4.key, keyC4, globalparamsv.ALGORITHMINFO_GRAPHITERATIONID, globalparamsv.ALGORITHMINFO_GRAPHALGORITHMID);
			keyRES5 = applyfunc(udata5.key, keyC5, globalparamsv.ALGORITHMINFO_GRAPHITERATIONID, globalparamsv.ALGORITHMINFO_GRAPHALGORITHMID);
			keyRES6 = applyfunc(udata6.key, keyC6, globalparamsv.ALGORITHMINFO_GRAPHITERATIONID, globalparamsv.ALGORITHMINFO_GRAPHALGORITHMID);
			keyRES7 = applyfunc(udata7.key, keyC7, globalparamsv.ALGORITHMINFO_GRAPHITERATIONID, globalparamsv.ALGORITHMINFO_GRAPHALGORITHMID);
			valueRES0 = applyfunc(udata0.value, valC0, globalparamsv.ALGORITHMINFO_GRAPHITERATIONID, globalparamsv.ALGORITHMINFO_GRAPHALGORITHMID);
			valueRES1 = applyfunc(udata1.value, valC1, globalparamsv.ALGORITHMINFO_GRAPHITERATIONID, globalparamsv.ALGORITHMINFO_GRAPHALGORITHMID);
			valueRES2 = applyfunc(udata2.value, valC2, globalparamsv.ALGORITHMINFO_GRAPHITERATIONID, globalparamsv.ALGORITHMINFO_GRAPHALGORITHMID);
			valueRES3 = applyfunc(udata3.value, valC3, globalparamsv.ALGORITHMINFO_GRAPHITERATIONID, globalparamsv.ALGORITHMINFO_GRAPHALGORITHMID);
			valueRES4 = applyfunc(udata4.value, valC4, globalparamsv.ALGORITHMINFO_GRAPHITERATIONID, globalparamsv.ALGORITHMINFO_GRAPHALGORITHMID);
			valueRES5 = applyfunc(udata5.value, valC5, globalparamsv.ALGORITHMINFO_GRAPHITERATIONID, globalparamsv.ALGORITHMINFO_GRAPHALGORITHMID);
			valueRES6 = applyfunc(udata6.value, valC6, globalparamsv.ALGORITHMINFO_GRAPHITERATIONID, globalparamsv.ALGORITHMINFO_GRAPHALGORITHMID);
			valueRES7 = applyfunc(udata7.value, valC7, globalparamsv.ALGORITHMINFO_GRAPHITERATIONID, globalparamsv.ALGORITHMINFO_GRAPHALGORITHMID);
			
			// 
			buffer[0][i].key = keyC0;
			buffer[0][i].value = valC0;
			buffer[1][i].key = keyC1;
			buffer[1][i].value = valC1;
			buffer[2][i].key = keyC2;
			buffer[2][i].value = valC2;
			buffer[3][i].key = keyC3;
			buffer[3][i].value = valC3;
			buffer[4][i].key = keyC4;
			buffer[4][i].value = valC4;
			buffer[5][i].key = keyC5;
			buffer[5][i].value = valC5;
			buffer[6][i].key = keyC6;
			buffer[6][i].value = valC6;
			buffer[7][i].key = keyC7;
			buffer[7][i].value = valC7;
			
			if(keyRES0 != udata0.key){ vmask[i].data[colindex + 0].key = 1; } else { vmask[i].data[colindex + 0].key = 0; }
			if(valueRES0 != udata0.value){ vmask[i].data[colindex + 0].value = 1; } else { vmask[i].data[colindex + 0].value = 0; }
			if(keyRES1 != udata1.key){ vmask[i].data[colindex + 1].key = 1; } else { vmask[i].data[colindex + 1].key = 0; }
			if(valueRES1 != udata1.value){ vmask[i].data[colindex + 1].value = 1; } else { vmask[i].data[colindex + 1].value = 0; }
			if(keyRES2 != udata2.key){ vmask[i].data[colindex + 2].key = 1; } else { vmask[i].data[colindex + 2].key = 0; }
			if(valueRES2 != udata2.value){ vmask[i].data[colindex + 2].value = 1; } else { vmask[i].data[colindex + 2].value = 0; }
			if(keyRES3 != udata3.key){ vmask[i].data[colindex + 3].key = 1; } else { vmask[i].data[colindex + 3].key = 0; }
			if(valueRES3 != udata3.value){ vmask[i].data[colindex + 3].value = 1; } else { vmask[i].data[colindex + 3].value = 0; }
			if(keyRES4 != udata4.key){ vmask[i].data[colindex + 4].key = 1; } else { vmask[i].data[colindex + 4].key = 0; }
			if(valueRES4 != udata4.value){ vmask[i].data[colindex + 4].value = 1; } else { vmask[i].data[colindex + 4].value = 0; }
			if(keyRES5 != udata5.key){ vmask[i].data[colindex + 5].key = 1; } else { vmask[i].data[colindex + 5].key = 0; }
			if(valueRES5 != udata5.value){ vmask[i].data[colindex + 5].value = 1; } else { vmask[i].data[colindex + 5].value = 0; }
			if(keyRES6 != udata6.key){ vmask[i].data[colindex + 6].key = 1; } else { vmask[i].data[colindex + 6].key = 0; }
			if(valueRES6 != udata6.value){ vmask[i].data[colindex + 6].value = 1; } else { vmask[i].data[colindex + 6].value = 0; }
			if(keyRES7 != udata7.key){ vmask[i].data[colindex + 7].key = 1; } else { vmask[i].data[colindex + 7].key = 0; }
			if(valueRES7 != udata7.value){ vmask[i].data[colindex + 7].value = 1; } else { vmask[i].data[colindex + 7].value = 0; }
			
			uint32_type mask = CONVERTVMASKTOUINT32(vmask, i);
			cummvmask_sp[source_partition % 2] = cummvmask_sp[source_partition % 2] | mask;
			
			// apply
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			vid[0][0] = vreadoffset_kvs*VECTOR_SIZE*2 + ((0*globalparamsv.SIZE_REDUCE) + i*2);
			vid[0][1] = vreadoffset_kvs*VECTOR_SIZE*2 + ((0*globalparamsv.SIZE_REDUCE) + i*2 + 1);
			vid[1][0] = vreadoffset_kvs*VECTOR_SIZE*2 + ((1*globalparamsv.SIZE_REDUCE) + i*2);
			vid[1][1] = vreadoffset_kvs*VECTOR_SIZE*2 + ((1*globalparamsv.SIZE_REDUCE) + i*2 + 1);
			vid[2][0] = vreadoffset_kvs*VECTOR_SIZE*2 + ((2*globalparamsv.SIZE_REDUCE) + i*2);
			vid[2][1] = vreadoffset_kvs*VECTOR_SIZE*2 + ((2*globalparamsv.SIZE_REDUCE) + i*2 + 1);
			vid[3][0] = vreadoffset_kvs*VECTOR_SIZE*2 + ((3*globalparamsv.SIZE_REDUCE) + i*2);
			vid[3][1] = vreadoffset_kvs*VECTOR_SIZE*2 + ((3*globalparamsv.SIZE_REDUCE) + i*2 + 1);
			vid[4][0] = vreadoffset_kvs*VECTOR_SIZE*2 + ((4*globalparamsv.SIZE_REDUCE) + i*2);
			vid[4][1] = vreadoffset_kvs*VECTOR_SIZE*2 + ((4*globalparamsv.SIZE_REDUCE) + i*2 + 1);
			vid[5][0] = vreadoffset_kvs*VECTOR_SIZE*2 + ((5*globalparamsv.SIZE_REDUCE) + i*2);
			vid[5][1] = vreadoffset_kvs*VECTOR_SIZE*2 + ((5*globalparamsv.SIZE_REDUCE) + i*2 + 1);
			vid[6][0] = vreadoffset_kvs*VECTOR_SIZE*2 + ((6*globalparamsv.SIZE_REDUCE) + i*2);
			vid[6][1] = vreadoffset_kvs*VECTOR_SIZE*2 + ((6*globalparamsv.SIZE_REDUCE) + i*2 + 1);
			vid[7][0] = vreadoffset_kvs*VECTOR_SIZE*2 + ((7*globalparamsv.SIZE_REDUCE) + i*2);
			vid[7][1] = vreadoffset_kvs*VECTOR_SIZE*2 + ((7*globalparamsv.SIZE_REDUCE) + i*2 + 1);
	
			#endif 
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3	
			if(keyRES0 != udata0.key){ cout<<"synchronizeandapply: vid[0][0]: "<<vid[0][0]<<", vmask["<<i<<"].data["<<colindex + 0<<"].key: "<<vmask[i].data[colindex + 0].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
			if(valueRES0 != udata0.value){ cout<<"synchronizeandapply: vid[0][1]: "<<vid[0][1]<<", vmask["<<i<<"].data["<<colindex + 0<<"].value: "<<vmask[i].data[colindex + 0].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
			if(keyRES1 != udata1.key){ cout<<"synchronizeandapply: vid[1][0]: "<<vid[1][0]<<", vmask["<<i<<"].data["<<colindex + 1<<"].key: "<<vmask[i].data[colindex + 1].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
			if(valueRES1 != udata1.value){ cout<<"synchronizeandapply: vid[1][1]: "<<vid[1][1]<<", vmask["<<i<<"].data["<<colindex + 1<<"].value: "<<vmask[i].data[colindex + 1].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
			if(keyRES2 != udata2.key){ cout<<"synchronizeandapply: vid[2][0]: "<<vid[2][0]<<", vmask["<<i<<"].data["<<colindex + 2<<"].key: "<<vmask[i].data[colindex + 2].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
			if(valueRES2 != udata2.value){ cout<<"synchronizeandapply: vid[2][1]: "<<vid[2][1]<<", vmask["<<i<<"].data["<<colindex + 2<<"].value: "<<vmask[i].data[colindex + 2].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
			if(keyRES3 != udata3.key){ cout<<"synchronizeandapply: vid[3][0]: "<<vid[3][0]<<", vmask["<<i<<"].data["<<colindex + 3<<"].key: "<<vmask[i].data[colindex + 3].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
			if(valueRES3 != udata3.value){ cout<<"synchronizeandapply: vid[3][1]: "<<vid[3][1]<<", vmask["<<i<<"].data["<<colindex + 3<<"].value: "<<vmask[i].data[colindex + 3].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
			if(keyRES4 != udata4.key){ cout<<"synchronizeandapply: vid[4][0]: "<<vid[4][0]<<", vmask["<<i<<"].data["<<colindex + 4<<"].key: "<<vmask[i].data[colindex + 4].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
			if(valueRES4 != udata4.value){ cout<<"synchronizeandapply: vid[4][1]: "<<vid[4][1]<<", vmask["<<i<<"].data["<<colindex + 4<<"].value: "<<vmask[i].data[colindex + 4].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
			if(keyRES5 != udata5.key){ cout<<"synchronizeandapply: vid[5][0]: "<<vid[5][0]<<", vmask["<<i<<"].data["<<colindex + 5<<"].key: "<<vmask[i].data[colindex + 5].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
			if(valueRES5 != udata5.value){ cout<<"synchronizeandapply: vid[5][1]: "<<vid[5][1]<<", vmask["<<i<<"].data["<<colindex + 5<<"].value: "<<vmask[i].data[colindex + 5].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
			if(keyRES6 != udata6.key){ cout<<"synchronizeandapply: vid[6][0]: "<<vid[6][0]<<", vmask["<<i<<"].data["<<colindex + 6<<"].key: "<<vmask[i].data[colindex + 6].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
			if(valueRES6 != udata6.value){ cout<<"synchronizeandapply: vid[6][1]: "<<vid[6][1]<<", vmask["<<i<<"].data["<<colindex + 6<<"].value: "<<vmask[i].data[colindex + 6].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
			if(keyRES7 != udata7.key){ cout<<"synchronizeandapply: vid[7][0]: "<<vid[7][0]<<", vmask["<<i<<"].data["<<colindex + 7<<"].key: "<<vmask[i].data[colindex + 7].key<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
			if(valueRES7 != udata7.value){ cout<<"synchronizeandapply: vid[7][1]: "<<vid[7][1]<<", vmask["<<i<<"].data["<<colindex + 7<<"].value: "<<vmask[i].data[colindex + 7].value<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
			#endif
		}
		
		// write vertices
		WRITEBACK_LOOP2: for (buffer_type i=0; i<reducebuffersz; i++){
		#pragma HLS PIPELINE II=1
			#ifdef _WIDEWORD
			kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(31, 0) = buffer[0][i].key;
			kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(63, 32) = buffer[0][i].value;
			kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(95, 64) = buffer[1][i].key;
			kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(127, 96) = buffer[1][i].value;
			kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(159, 128) = buffer[2][i].key;
			kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(191, 160) = buffer[2][i].value;
			kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(223, 192) = buffer[3][i].key;
			kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(255, 224) = buffer[3][i].value;
			kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(287, 256) = buffer[4][i].key;
			kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(319, 288) = buffer[4][i].value;
			kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(351, 320) = buffer[5][i].key;
			kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(383, 352) = buffer[5][i].value;
			kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(415, 384) = buffer[6][i].key;
			kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(447, 416) = buffer[6][i].value;
			kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(479, 448) = buffer[7][i].key;
			kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(511, 480) = buffer[7][i].value;
			kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(31, 0) = buffer[0][i].key;
			kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(63, 32) = buffer[0][i].value;
			kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(95, 64) = buffer[1][i].key;
			kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(127, 96) = buffer[1][i].value;
			kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(159, 128) = buffer[2][i].key;
			kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(191, 160) = buffer[2][i].value;
			kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(223, 192) = buffer[3][i].key;
			kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(255, 224) = buffer[3][i].value;
			kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(287, 256) = buffer[4][i].key;
			kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(319, 288) = buffer[4][i].value;
			kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(351, 320) = buffer[5][i].key;
			kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(383, 352) = buffer[5][i].value;
			kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(415, 384) = buffer[6][i].key;
			kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(447, 416) = buffer[6][i].value;
			kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(479, 448) = buffer[7][i].key;
			kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(511, 480) = buffer[7][i].value;
			kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(31, 0) = buffer[0][i].key;
			kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(63, 32) = buffer[0][i].value;
			kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(95, 64) = buffer[1][i].key;
			kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(127, 96) = buffer[1][i].value;
			kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(159, 128) = buffer[2][i].key;
			kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(191, 160) = buffer[2][i].value;
			kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(223, 192) = buffer[3][i].key;
			kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(255, 224) = buffer[3][i].value;
			kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(287, 256) = buffer[4][i].key;
			kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(319, 288) = buffer[4][i].value;
			kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(351, 320) = buffer[5][i].key;
			kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(383, 352) = buffer[5][i].value;
			kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(415, 384) = buffer[6][i].key;
			kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(447, 416) = buffer[6][i].value;
			kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(479, 448) = buffer[7][i].key;
			kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(511, 480) = buffer[7][i].value;
			#else 
			kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[0].key = buffer[0][i].key;
			kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[0].value = buffer[0][i].value;
			kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[1].key = buffer[1][i].key;
			kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[1].value = buffer[1][i].value;
			kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[2].key = buffer[2][i].key;
			kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[2].value = buffer[2][i].value;
			kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[3].key = buffer[3][i].key;
			kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[3].value = buffer[3][i].value;
			kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[4].key = buffer[4][i].key;
			kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[4].value = buffer[4][i].value;
			kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[5].key = buffer[5][i].key;
			kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[5].value = buffer[5][i].value;
			kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[6].key = buffer[6][i].key;
			kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[6].value = buffer[6][i].value;
			kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[7].key = buffer[7][i].key;
			kvdram0[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[7].value = buffer[7][i].value;
	
			kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[0].key = buffer[0][i].key;
			kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[0].value = buffer[0][i].value;
			kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[1].key = buffer[1][i].key;
			kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[1].value = buffer[1][i].value;
			kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[2].key = buffer[2][i].key;
			kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[2].value = buffer[2][i].value;
			kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[3].key = buffer[3][i].key;
			kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[3].value = buffer[3][i].value;
			kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[4].key = buffer[4][i].key;
			kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[4].value = buffer[4][i].value;
			kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[5].key = buffer[5][i].key;
			kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[5].value = buffer[5][i].value;
			kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[6].key = buffer[6][i].key;
			kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[6].value = buffer[6][i].value;
			kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[7].key = buffer[7][i].key;
			kvdram1[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[7].value = buffer[7][i].value;
	
			kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[0].key = buffer[0][i].key;
			kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[0].value = buffer[0][i].value;
			kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[1].key = buffer[1][i].key;
			kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[1].value = buffer[1][i].value;
			kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[2].key = buffer[2][i].key;
			kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[2].value = buffer[2][i].value;
			kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[3].key = buffer[3][i].key;
			kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[3].value = buffer[3][i].value;
			kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[4].key = buffer[4][i].key;
			kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[4].value = buffer[4][i].value;
			kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[5].key = buffer[5][i].key;
			kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[5].value = buffer[5][i].value;
			kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[6].key = buffer[6][i].key;
			kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[6].value = buffer[6][i].value;
			kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[7].key = buffer[7][i].key;
			kvdram2[globalparamsk.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[7].value = buffer[7][i].value;
	
			#endif
			
			#ifdef _WIDEWORD
			vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(31, 0) = buffer[0][i].key;
			vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(63, 32) = buffer[0][i].value;
			vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(95, 64) = buffer[1][i].key;
			vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(127, 96) = buffer[1][i].value;
			vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(159, 128) = buffer[2][i].key;
			vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(191, 160) = buffer[2][i].value;
			vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(223, 192) = buffer[3][i].key;
			vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(255, 224) = buffer[3][i].value;
			vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(287, 256) = buffer[4][i].key;
			vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(319, 288) = buffer[4][i].value;
			vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(351, 320) = buffer[5][i].key;
			vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(383, 352) = buffer[5][i].value;
			vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(415, 384) = buffer[6][i].key;
			vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(447, 416) = buffer[6][i].value;
			vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(479, 448) = buffer[7][i].key;
			vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].range(511, 480) = buffer[7][i].value;
			#else
			vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[0].key = buffer[0][i].key;
			vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[0].value = buffer[0][i].value;
			vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[1].key = buffer[1][i].key;
			vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[1].value = buffer[1][i].value;
			vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[2].key = buffer[2][i].key;
			vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[2].value = buffer[2][i].value;
			vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[3].key = buffer[3][i].key;
			vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[3].value = buffer[3][i].value;
			vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[4].key = buffer[4][i].key;
			vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[4].value = buffer[4][i].value;
			vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[5].key = buffer[5][i].key;
			vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[5].value = buffer[5][i].value;
			vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[6].key = buffer[6][i].key;
			vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[6].value = buffer[6][i].value;
			vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[7].key = buffer[7][i].key;
			vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + i].data[7].value = buffer[7][i].value;
			#endif
		}
		
		// prepare masks
		WRITEBACK_LOOP3: for (buffer_type i=0; i<reducebuffersz; i++){
		#pragma HLS PIPELINE II=1
			unsigned int X = i % 16; // AUTOMATEME.
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
		
		// save masks
		SAVEVMASKS_LOOP: for(buffer_type i=0; i<vmaskbuffersz_kvs; i++){
		#pragma HLS PIPELINE II=1
			#ifdef _WIDEWORD
			kvdram0[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].range(31, 0) = tempbuffer[0][i];
			kvdram0[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].range(63, 32) = tempbuffer[1][i];
			kvdram0[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].range(95, 64) = tempbuffer[2][i];
			kvdram0[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].range(127, 96) = tempbuffer[3][i];
			kvdram0[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].range(159, 128) = tempbuffer[4][i];
			kvdram0[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].range(191, 160) = tempbuffer[5][i];
			kvdram0[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].range(223, 192) = tempbuffer[6][i];
			kvdram0[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].range(255, 224) = tempbuffer[7][i];
			kvdram0[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].range(287, 256) = tempbuffer[8][i];
			kvdram0[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].range(319, 288) = tempbuffer[9][i];
			kvdram0[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].range(351, 320) = tempbuffer[10][i];
			kvdram0[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].range(383, 352) = tempbuffer[11][i];
			kvdram0[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].range(415, 384) = tempbuffer[12][i];
			kvdram0[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].range(447, 416) = tempbuffer[13][i];
			kvdram0[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].range(479, 448) = tempbuffer[14][i];
			kvdram0[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].range(511, 480) = tempbuffer[15][i];
			kvdram1[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].range(31, 0) = tempbuffer[0][i];
			kvdram1[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].range(63, 32) = tempbuffer[1][i];
			kvdram1[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].range(95, 64) = tempbuffer[2][i];
			kvdram1[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].range(127, 96) = tempbuffer[3][i];
			kvdram1[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].range(159, 128) = tempbuffer[4][i];
			kvdram1[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].range(191, 160) = tempbuffer[5][i];
			kvdram1[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].range(223, 192) = tempbuffer[6][i];
			kvdram1[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].range(255, 224) = tempbuffer[7][i];
			kvdram1[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].range(287, 256) = tempbuffer[8][i];
			kvdram1[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].range(319, 288) = tempbuffer[9][i];
			kvdram1[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].range(351, 320) = tempbuffer[10][i];
			kvdram1[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].range(383, 352) = tempbuffer[11][i];
			kvdram1[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].range(415, 384) = tempbuffer[12][i];
			kvdram1[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].range(447, 416) = tempbuffer[13][i];
			kvdram1[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].range(479, 448) = tempbuffer[14][i];
			kvdram1[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].range(511, 480) = tempbuffer[15][i];
			kvdram2[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].range(31, 0) = tempbuffer[0][i];
			kvdram2[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].range(63, 32) = tempbuffer[1][i];
			kvdram2[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].range(95, 64) = tempbuffer[2][i];
			kvdram2[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].range(127, 96) = tempbuffer[3][i];
			kvdram2[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].range(159, 128) = tempbuffer[4][i];
			kvdram2[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].range(191, 160) = tempbuffer[5][i];
			kvdram2[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].range(223, 192) = tempbuffer[6][i];
			kvdram2[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].range(255, 224) = tempbuffer[7][i];
			kvdram2[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].range(287, 256) = tempbuffer[8][i];
			kvdram2[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].range(319, 288) = tempbuffer[9][i];
			kvdram2[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].range(351, 320) = tempbuffer[10][i];
			kvdram2[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].range(383, 352) = tempbuffer[11][i];
			kvdram2[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].range(415, 384) = tempbuffer[12][i];
			kvdram2[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].range(447, 416) = tempbuffer[13][i];
			kvdram2[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].range(479, 448) = tempbuffer[14][i];
			kvdram2[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].range(511, 480) = tempbuffer[15][i];
			#else 
			kvdram0[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].data[0].key = tempbuffer[0][i];
			kvdram0[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].data[0].value = tempbuffer[1][i]; 
			kvdram0[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].data[1].key = tempbuffer[2][i];
			kvdram0[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].data[1].value = tempbuffer[3][i]; 
			kvdram0[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].data[2].key = tempbuffer[4][i];
			kvdram0[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].data[2].value = tempbuffer[5][i]; 
			kvdram0[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].data[3].key = tempbuffer[6][i];
			kvdram0[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].data[3].value = tempbuffer[7][i]; 
			kvdram0[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].data[4].key = tempbuffer[8][i];
			kvdram0[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].data[4].value = tempbuffer[9][i]; 
			kvdram0[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].data[5].key = tempbuffer[10][i];
			kvdram0[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].data[5].value = tempbuffer[11][i]; 
			kvdram0[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].data[6].key = tempbuffer[12][i];
			kvdram0[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].data[6].value = tempbuffer[13][i]; 
			kvdram0[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].data[7].key = tempbuffer[14][i];
			kvdram0[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].data[7].value = tempbuffer[15][i]; 
			kvdram1[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].data[0].key = tempbuffer[0][i];
			kvdram1[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].data[0].value = tempbuffer[1][i]; 
			kvdram1[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].data[1].key = tempbuffer[2][i];
			kvdram1[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].data[1].value = tempbuffer[3][i]; 
			kvdram1[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].data[2].key = tempbuffer[4][i];
			kvdram1[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].data[2].value = tempbuffer[5][i]; 
			kvdram1[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].data[3].key = tempbuffer[6][i];
			kvdram1[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].data[3].value = tempbuffer[7][i]; 
			kvdram1[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].data[4].key = tempbuffer[8][i];
			kvdram1[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].data[4].value = tempbuffer[9][i]; 
			kvdram1[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].data[5].key = tempbuffer[10][i];
			kvdram1[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].data[5].value = tempbuffer[11][i]; 
			kvdram1[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].data[6].key = tempbuffer[12][i];
			kvdram1[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].data[6].value = tempbuffer[13][i]; 
			kvdram1[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].data[7].key = tempbuffer[14][i];
			kvdram1[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].data[7].value = tempbuffer[15][i]; 
			kvdram2[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].data[0].key = tempbuffer[0][i];
			kvdram2[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].data[0].value = tempbuffer[1][i]; 
			kvdram2[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].data[1].key = tempbuffer[2][i];
			kvdram2[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].data[1].value = tempbuffer[3][i]; 
			kvdram2[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].data[2].key = tempbuffer[4][i];
			kvdram2[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].data[2].value = tempbuffer[5][i]; 
			kvdram2[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].data[3].key = tempbuffer[6][i];
			kvdram2[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].data[3].value = tempbuffer[7][i]; 
			kvdram2[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].data[4].key = tempbuffer[8][i];
			kvdram2[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].data[4].value = tempbuffer[9][i]; 
			kvdram2[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].data[5].key = tempbuffer[10][i];
			kvdram2[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].data[5].value = tempbuffer[11][i]; 
			kvdram2[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].data[6].key = tempbuffer[12][i];
			kvdram2[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].data[6].value = tempbuffer[13][i]; 
			kvdram2[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].data[7].key = tempbuffer[14][i];
			kvdram2[globalparamsk.BASEOFFSETKVS_VERTICESDATAMASK + vmaskreadoffset_kvs + i].data[7].value = tempbuffer[15][i]; 
			#endif
		}
		
		// save vmaskp
		#ifdef _WIDEWORD
		kvdram0[globalparamsk.BASEOFFSETKVS_VERTICESPARTITIONMASK + (source_partition/2)].range(31, 0) = cummvmask_sp[0] | cummvmask_sp[1]; // cummvmask_sp;
		kvdram1[globalparamsk.BASEOFFSETKVS_VERTICESPARTITIONMASK + (source_partition/2)].range(31, 0) = cummvmask_sp[0] | cummvmask_sp[1]; // cummvmask_sp;
		kvdram2[globalparamsk.BASEOFFSETKVS_VERTICESPARTITIONMASK + (source_partition/2)].range(31, 0) = cummvmask_sp[0] | cummvmask_sp[1]; // cummvmask_sp;
		#else
		kvdram0[globalparamsk.BASEOFFSETKVS_VERTICESPARTITIONMASK + (source_partition/2)].data[0].key = cummvmask_sp[0] | cummvmask_sp[1]; // cummvmask_sp;
		kvdram1[globalparamsk.BASEOFFSETKVS_VERTICESPARTITIONMASK + (source_partition/2)].data[0].key = cummvmask_sp[0] | cummvmask_sp[1]; // cummvmask_sp;
		kvdram2[globalparamsk.BASEOFFSETKVS_VERTICESPARTITIONMASK + (source_partition/2)].data[0].key = cummvmask_sp[0] | cummvmask_sp[1]; // cummvmask_sp;
		#endif
		if(source_partition % 2 == 1){ cummvmask_sp[0] = 0; cummvmask_sp[1] = 0; } // reset.
		
		vreadoffset_kvs += reducebuffersz;
		vmaskreadoffset_kvs += vmaskbuffersz_kvs; 
	}
	return;
}
}


