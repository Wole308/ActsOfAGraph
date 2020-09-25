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
#include "../../include/common.h"
#include "../include/actscommon.h"
#ifndef FPGA_IMPL
#include "../../src/utility/utility.h"
#endif 
#ifndef HW
#include "../../acts/actsutility/actsutility.h"
#endif 
#include "actslw.h"
using namespace std;

#ifdef SW
actslw::actslw(){ actsutilityobj = new actsutility(); }
actslw::~actslw(){}
#endif
#ifdef SWEMU
actsutility * actsutilityobj = new actsutility();
#endif

// kernel utility functions
buffer_type
	#ifdef SW 
	actslw::
	#endif 
allignhigher_KV(buffer_type val){
	buffer_type fac = (val + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	return (fac * VECTOR_SIZE);
}
batch_type
	#ifdef SW 
	actslw::
	#endif
getskipsize(step_type currentLOP, bool_type sourceORdest){
	analysis_type analysis_treedepth = TREE_DEPTH;
	batch_type result;
	
	if(currentLOP == 0){ currentLOP = 1; }
	if(sourceORdest == SOURCE){ result = NUMLASTLEVELPARTITIONS; }
	else if (sourceORdest == DEST){ result = NUMLASTLEVELPARTITIONS / NUM_PARTITIONS; }
	else {}
	for(step_type i=0; i<(currentLOP-1); i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_treedepth avg=analysis_treedepth
		result = result / NUM_PARTITIONS;
	}
	return result;
}
void 
	#ifdef SW 
	actslw::
	#endif 
copykeyandvalues(keyvalue_t * buffer1, keyvalue_t * buffer2, buffer_type size){
	for(partition_type i=0; i<size; i++){ buffer1[i] = buffer2[i]; }
}
void 
	#ifdef SW 
	actslw::
	#endif 
resetkeyandvalues(skeyvalue_t * buffer, buffer_type size){
	for(buffer_type i=0; i<size; i++){ buffer[i].key = 0; buffer[i].value = 0; }
}
void 
	#ifdef SW 
	actslw::
	#endif 
resetvalues(keyvalue_t * buffer, buffer_type size){
	for(buffer_type i=0; i<size; i++){ buffer[i].value = 0; }
}
void 
	#ifdef SW 
	actslw::
	#endif 
resetkeyandvalues(bool_type enable, uint512_dt destbuffer[PADDEDDESTBUFFER_SIZE]){
	if(enable == OFF){ return; }
	uint512_dt ZeroKVS;
	keyvalue_t ZeroKV; ZeroKV.key = 0; ZeroKV.value = 0;
	#ifdef _WIDEWORD
	ZeroKVS = 0;
	#else 
	for(vector_type v=0; v<VECTOR_SIZE; v++){ ZeroKVS.data[v] = ZeroKV; }
	#endif 
	for(partition_type i=0; i<PADDEDDESTBUFFER_SIZE; i++){ 
		destbuffer[i] = ZeroKVS;
	}
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif 
resetmanykeyandvalues(skeyvalue_t buffer[VECTOR_SIZE][NUM_PARTITIONS], buffer_type size){
	for(buffer_type i=0; i<size; i++){
		buffer[0][i].key = 0; buffer[0][i].value = 0; 
		buffer[1][i].key = 0; buffer[1][i].value = 0; 
		buffer[2][i].key = 0; buffer[2][i].value = 0; 
		buffer[3][i].key = 0; buffer[3][i].value = 0; 
		buffer[4][i].key = 0; buffer[4][i].value = 0; 
		buffer[5][i].key = 0; buffer[5][i].value = 0; 
		buffer[6][i].key = 0; buffer[6][i].value = 0; 
		buffer[7][i].key = 0; buffer[7][i].value = 0; 
	}
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif 
resetmanykeyandvalues(keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], buffer_type size){
	for(buffer_type i=0; i<size; i++){
		buffer[0][i].key = 0; buffer[0][i].value = 0; 
		buffer[1][i].key = 0; buffer[1][i].value = 0; 
		buffer[2][i].key = 0; buffer[2][i].value = 0; 
		buffer[3][i].key = 0; buffer[3][i].value = 0; 
		buffer[4][i].key = 0; buffer[4][i].value = 0; 
		buffer[5][i].key = 0; buffer[5][i].value = 0; 
		buffer[6][i].key = 0; buffer[6][i].value = 0; 
		buffer[7][i].key = 0; buffer[7][i].value = 0; 
	}
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif 
resetmanyvalues(skeyvalue_t buffer[VECTOR_SIZE][NUM_PARTITIONS], buffer_type size){
	for(buffer_type i=0; i<size; i++){ 
		buffer[0][i].value = 0;
		buffer[1][i].value = 0;
		buffer[2][i].value = 0;
		buffer[3][i].value = 0;
		buffer[4][i].value = 0;
		buffer[5][i].value = 0;
		buffer[6][i].value = 0;
		buffer[7][i].value = 0;
	}
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif
accumkeysandvalues(keyvalue_t * buffer1, keyvalue_t * buffer2, buffer_type size){
	for(partition_type i=0; i<size; i++){
	#pragma HLS PIPELINE II=1
		keyvalue_t keyvalue1 = buffer1[i];
		keyvalue_t keyvalue2 = buffer2[i];
		keyvalue1.key += keyvalue2.key;
		keyvalue1.value += keyvalue2.value;
		buffer1[i] = keyvalue1;
	}
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif
accumkeysandvalues(keyvalue_t * buffer1, keyvalue_t * buffer2, keyvalue_t * buffer3, buffer_type size){
	for(partition_type i=0; i<size; i++){
	#pragma HLS PIPELINE II=1
		keyvalue_t keyvalue1 = buffer1[i];
		keyvalue_t keyvalue2 = buffer2[i];
		keyvalue_t keyvalue3 = buffer3[i];
		keyvalue3.key = keyvalue1.key + keyvalue2.key;
		keyvalue3.value = keyvalue1.value + keyvalue2.value;
		buffer3[i] = keyvalue3;
	}
	return;
}
unsigned int 
	#ifdef SW 
	actslw::
	#endif 
checkandforce(unsigned int val, unsigned int limit){ if(val >= limit){ return 0; } else { return val; }}
buffer_type 
	#ifdef SW 
	actslw::
	#endif 
getchunksize(buffer_type buffer_size, travstate_t travstate, buffer_type localoffset){
	buffer_type chunk_size = buffer_size;
	batch_type i = travstate.i_kvs + localoffset;
	if (i > travstate.end_kvs){ chunk_size = 0; }
	else if ((i + buffer_size) > travstate.end_kvs){ chunk_size = travstate.end_kvs - i; }
	else {}
	return chunk_size;
}
partition_type
	#ifdef SW 
	actslw::
	#endif 
getpartition(keyvalue_t keyvalue, step_type currentLOP, vertex_t upperlimit){
	partition_type partition = ((keyvalue.key - upperlimit) >> (BATCH_RANGE_POW - (NUM_PARTITIONS_POW * currentLOP)));
	
	#ifdef _DEBUGMODE_CHECKS2
	if(partition >= NUM_PARTITIONS){ actsutilityobj->globalstats_counterrorsingetpartition(1); }
	#endif 
	
	#ifdef ENABLE_PERFECTACCURACY
		#ifdef _DEBUGMODE_CHECKS2
		if(partition >= NUM_PARTITIONS){ cout<<"getpartition::ERROR 1. partition out of bounds partition: "<<partition<<", keyvalue.key: "<<keyvalue.key<<", NUM_PARTITIONS: "<<NUM_PARTITIONS<<", keyvalue.key: "<<keyvalue.key<<", upperlimit: "<<upperlimit<<", currentLOP: "<<currentLOP<<endl; exit(EXIT_FAILURE); }
		#endif
	#endif 
	#ifndef ENABLE_PERFECTACCURACY
		// partition = checkandforce(partition, NUM_PARTITIONS); 
		if(partition >= NUM_PARTITIONS){ partition = (((1 << NUM_PARTITIONS_POW) - 1) & (partition >> (1 - 1))); } // FIXME. REMOVEME. PERFECTIONTEST.
	#endif
	
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("actslw::getpartition 2", partition, NUM_PARTITIONS, keyvalue.key, upperlimit, currentLOP);
	#endif
	return partition;
}
buffer_type 
	#ifdef SW 
	actslw::
	#endif 
getglobalpartition(keyvalue_t keyvalue, vertex_t upperlimit){
	partition_type globalpartition = ((keyvalue.key - upperlimit) >> (BATCH_RANGE_POW - (NUM_PARTITIONS_POW * TREE_DEPTH)));
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("actslw::getglobalpartition", globalpartition, NUMLASTLEVELPARTITIONS, keyvalue.key, upperlimit, NAp);
	#endif
	return globalpartition;
}
value_t 
	#ifdef SW 
	actslw::
	#endif 
reducefunc(keyy_t vid, value_t value, value_t edgeval, unsigned int GraphIter, unsigned int GraphAlgo){
	#pragma HLS INLINE
	value_t ret = 0;
	#ifdef PR_ALGORITHM
	ret = value + edgeval;
	#elif defined(BFS_ALGORITHM)
	ret = min(value, GraphIter);
	#elif defined(BC_ALGORITHM)
	ret = min(value, GraphIter);
	#endif
	/** if(GraphAlgo == PAGERANK){ ret = value + edgeval; }
	else if(GraphAlgo == BREADTHFIRSTSEARCH){ ret = min(value, GraphIter); }
	else if(GraphAlgo == BETWEENNESSCENTRALITY){ ret = min(value, GraphIter); }
	else {} */
	return ret;
}
void 
	#ifdef SW 
	actslw::
	#endif 
copykeyvalues(keyvalue_t * buffer1, keyvalue_t * buffer2, buffer_type size){
	COPYKEYS_LOOP: for(buffer_type i=0; i<size; i++){ buffer1[i] = buffer2[i]; }
}
buffer_type 
	#ifdef SW 
	actslw::
	#endif 
getpartitionwritesz(buffer_type realsize_kvs, buffer_type bramoffset_kvs){
	#pragma HLS INLINE
	buffer_type size_kvs;
	
	#ifdef ENABLE_APPROXIMATEPARTITIONWRITES
	if(realsize_kvs >= 0*PADSKIP && realsize_kvs < 1*PADSKIP){
		size_kvs = 1*PADSKIP;
	} else if(realsize_kvs >= 1*PADSKIP && realsize_kvs < 2*PADSKIP){
		size_kvs = 2*PADSKIP;
	} else if(realsize_kvs >= 2*PADSKIP && realsize_kvs < 3*PADSKIP){
		size_kvs = 3*PADSKIP;
	} else if(realsize_kvs >= 3*PADSKIP && realsize_kvs < 4*PADSKIP){
		size_kvs = 4*PADSKIP;
	} else if(realsize_kvs >= 4*PADSKIP && realsize_kvs < 5*PADSKIP){
		size_kvs = 5*PADSKIP;
	} else if(realsize_kvs >= 5*PADSKIP && realsize_kvs < 6*PADSKIP){
		size_kvs = 6*PADSKIP;
	} else if(realsize_kvs >= 6*PADSKIP && realsize_kvs < 7*PADSKIP){
		size_kvs = 7*PADSKIP;
	} else if(realsize_kvs >= 7*PADSKIP && realsize_kvs < 8*PADSKIP){
		size_kvs = 8*PADSKIP;
	} else if(realsize_kvs >= 8*PADSKIP && realsize_kvs < 9*PADSKIP){
		size_kvs = 9*PADSKIP;
	} else if(realsize_kvs >= 9*PADSKIP && realsize_kvs < 10*PADSKIP){
		size_kvs = 10*PADSKIP;
	} else if(realsize_kvs >= 10*PADSKIP && realsize_kvs < 11*PADSKIP){
		size_kvs = 11*PADSKIP;
	} else if(realsize_kvs >= 11*PADSKIP && realsize_kvs < 12*PADSKIP){
		size_kvs = 12*PADSKIP;
	} else if(realsize_kvs >= 12*PADSKIP && realsize_kvs < 13*PADSKIP){
		size_kvs = 13*PADSKIP;
	} else if(realsize_kvs >= 13*PADSKIP && realsize_kvs < 14*PADSKIP){
		size_kvs = 14*PADSKIP;
	} else if(realsize_kvs >= 14*PADSKIP && realsize_kvs < 15*PADSKIP){
		size_kvs = 15*PADSKIP;
	} else if(realsize_kvs >= 15*PADSKIP && realsize_kvs < 16*PADSKIP){
		size_kvs = 16*PADSKIP;
	} else if(realsize_kvs >= 16*PADSKIP && realsize_kvs < 17*PADSKIP){
		size_kvs = 17*PADSKIP;
	} else if(realsize_kvs >= 17*PADSKIP && realsize_kvs < 18*PADSKIP){
		size_kvs = 18*PADSKIP;
	} else if(realsize_kvs >= 18*PADSKIP && realsize_kvs < 19*PADSKIP){
		size_kvs = 19*PADSKIP;
	} else if(realsize_kvs >= 19*PADSKIP && realsize_kvs < 20*PADSKIP){
		size_kvs = 20*PADSKIP;
	} else if(realsize_kvs >= 20*PADSKIP && realsize_kvs < 21*PADSKIP){
		size_kvs = 21*PADSKIP;
	} else if(realsize_kvs >= 21*PADSKIP && realsize_kvs < 22*PADSKIP){
		size_kvs = 22*PADSKIP;
	} else if(realsize_kvs >= 22*PADSKIP && realsize_kvs < 23*PADSKIP){
		size_kvs = 23*PADSKIP;
	} else if(realsize_kvs >= 23*PADSKIP && realsize_kvs < 24*PADSKIP){
		size_kvs = 24*PADSKIP;
	} else {
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"WARNING:getpartitionwritesz: should not get here. something might be wrong. realsize_kvs: "<<realsize_kvs<<", size_kvs: "<<size_kvs<<", OPTIMALSIZE: "<<OPTIMALSIZE<<", PADSKIP: "<<PADSKIP<<endl; 
		exit(EXIT_FAILURE);
		#endif
		size_kvs = PADDEDDESTBUFFER_SIZE;
	}
	#ifdef SW 
	if((bramoffset_kvs + size_kvs) >= PADDEDDESTBUFFER_SIZE){ size_kvs = PADDEDDESTBUFFER_SIZE - bramoffset_kvs - 1; } 
	#endif 
	#else 
	size_kvs = realsize_kvs;
	#endif
	return size_kvs;
}
buffer_type
	#ifdef SW 
	actslw::
	#endif 
withinvalidrange(buffer_type val1, buffer_type val2){
	#pragma HLS INLINE
	if(val1 < val2){ return 1; }
	else { return 0; }
}
void 
	#ifdef SW 
	actslw::
	#endif 
calculateoffsets(keyvalue_t * buffer, buffer_type size, buffer_type base, buffer_type skipspacing){
	for(partition_type i=1; i<size; i++){ 
		buffer[i].key = allignhigher_KV(buffer[i-1].key + buffer[i-1].value + skipspacing); 
	}
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif 
calculateunallignedoffsets(skeyvalue_t buffer[NUM_PARTITIONS], buffer_type size, buffer_type base, buffer_type skipspacing){
	for(partition_type i=1; i<size; i++){ 
		buffer[i].key = buffer[i-1].key + buffer[i-1].value + skipspacing; 
	}
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif 
calculatemanyoffsets(skeyvalue_t buffer[VECTOR_SIZE][NUM_PARTITIONS], buffer_type size, buffer_type base, buffer_type skipspacing){
	for(partition_type i=1; i<size; i++){ 
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
	actslw::
	#endif 
calculatemanyunallignedoffsets(skeyvalue_t buffer[VECTOR_SIZE][NUM_PARTITIONS], buffer_type size, buffer_type base, buffer_type skipspacing){
	for(partition_type i=1; i<size; i++){ 
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
	actslw::
	#endif 
get_num_source_partitions(step_type currentLOP){
	#pragma HLS INLINE
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
	actslw::
	#endif 
getglobalparams(uint512_dt * sourcevolume){
	globalparams_t globalparams;
	#ifdef _WIDEWORD
	globalparams.runkernelcommand = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNKERNELCOMMANDID].range(31, 0);
	globalparams.processcommand = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_PROCESSCOMMANDID].range(31, 0);
	globalparams.collectstatscommand = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_COLLECTSTATSCOMMANDID].range(31, 0);
	globalparams.partitioncommand = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_PARTITIONCOMMANDID].range(31, 0);
	globalparams.reducecommand = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_APPLYUPDATESCOMMANDID].range(31, 0);
	globalparams.vbegin = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_VOFFSET].range(31, 0); 
	globalparams.vsize = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_VSIZE].range(31, 0); 
	globalparams.treedepth = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_TREEDEPTH].range(31, 0); 
	globalparams.LLOPnumpartitions = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_FINALNUMPARTITIONS].range(31, 0);
	globalparams.GraphIter = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_GRAPHITERATIONID].range(31, 0);
	globalparams.GraphAlgo = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_GRAPHALGORITHMID].range(31, 0);
	globalparams.batchsize = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BATCHSIZE].range(31, 0);
	globalparams.runsize = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].range(31, 0);
	globalparams.nextbatchoffset = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NEXTBATCHOFFSET].range(31, 0);
	globalparams.statsalreadycollected = NAp;
	globalparams.baseaddr_destkvs_kvs = 0;
	#else 
	globalparams.runkernelcommand = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNKERNELCOMMANDID].data[0].key;
	globalparams.processcommand = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_PROCESSCOMMANDID].data[0].key;
	globalparams.collectstatscommand = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_COLLECTSTATSCOMMANDID].data[0].key;
	globalparams.partitioncommand = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_PARTITIONCOMMANDID].data[0].key;
	globalparams.reducecommand = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_APPLYUPDATESCOMMANDID].data[0].key;
	globalparams.vbegin = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_VOFFSET].data[0].key; 
	globalparams.vsize = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_VSIZE].data[0].key; 
	globalparams.treedepth = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_TREEDEPTH].data[0].key; 
	globalparams.LLOPnumpartitions = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_FINALNUMPARTITIONS].data[0].key;
	globalparams.GraphIter = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_GRAPHITERATIONID].data[0].key;
	globalparams.GraphAlgo = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_GRAPHALGORITHMID].data[0].key;
	globalparams.batchsize = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BATCHSIZE].data[0].key;
	globalparams.runsize = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key;
	globalparams.nextbatchoffset = sourcevolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NEXTBATCHOFFSET].data[0].key;
	globalparams.statsalreadycollected = NAp;
	globalparams.baseaddr_destkvs_kvs = 0;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->printglobalparameters("actslw::getglobalparams:: printing global parameters", globalparams);
	#endif
	return globalparams;
}
sweepparams_t 
	#ifdef SW 
	actslw::
	#endif 
getsweepparams(globalparams_t globalparams, step_type currentLOP, batch_type source_partition){
	sweepparams_t sweepparams;
	batch_type sourceskipsize = getskipsize(currentLOP, SOURCE);
	
	sweepparams.currentLOP = currentLOP;
	if(currentLOP == 0 || (currentLOP % 2) == 1){ sweepparams.worksourcebaseaddress_kvs = BASEOFFSET_KVDRAM_KVS; sweepparams.workdestbaseaddress_kvs = BASEOFFSET_KVDRAMWORKSPACE_KVS; }
	else { sweepparams.worksourcebaseaddress_kvs = BASEOFFSET_KVDRAMWORKSPACE_KVS; sweepparams.workdestbaseaddress_kvs = BASEOFFSET_KVDRAM_KVS; }
	
	sweepparams.upperlimit = globalparams.vbegin + (source_partition * sourceskipsize * (BATCH_RANGE / NUMLASTLEVELPARTITIONS));
	sweepparams.source_partition = source_partition;
	return sweepparams;
}
config_t 
	#ifdef SW 
	actslw::
	#endif 
getconfig(step_type currentLOP){
	config_t config;
	
	if(currentLOP == 0){ config.enablecollectglobalstats = ON; config.enablepartition = OFF; config.enablereduce = OFF; } 
	else if(currentLOP > 0 && currentLOP <= TREE_DEPTH){ config.enablecollectglobalstats = OFF; config.enablepartition = ON; config.enablereduce = OFF; }
	else { config.enablecollectglobalstats = OFF; config.enablepartition = OFF; config.enablereduce = ON; }
	return config;
}
travstate_t 
	#ifdef SW 
	actslw::
	#endif 
gettravstate(uint512_dt * kvdram, keyvalue_t globalstatsbuffer[NUMLASTLEVELPARTITIONS], globalparams_t globalparams, config_t config, step_type currentLOP, batch_type source_partition, batch_type num_source_partitions){
	travstate_t travstate;
	batch_type sourceskipsize = getskipsize(currentLOP, SOURCE);
	
	keyvalue_t keyvalue;
	if(currentLOP < 2){ keyvalue.key = 0; }
	else { 
		#ifdef USEBRAMFORGLOBALSTATS
		keyvalue = globalstatsbuffer[source_partition * sourceskipsize]; 
		#else 
		keyvalue = kvdram[BASEOFFSET_STATSDRAM_KVS + (source_partition * sourceskipsize)].data[0]; 
		#endif 
	}
	
	keyvalue_t nextkeyvalue;
	nextkeyvalue.key = globalparams.runsize;
	if(currentLOP >= 2){
		nextkeyvalue.key = keyvalue.key + keyvalue.value;
	}
		
	travstate.begin_kvs = keyvalue.key / VECTOR_SIZE; 
	travstate.end_kvs = nextkeyvalue.key / VECTOR_SIZE;
	if(config.enablecollectglobalstats == ON || config.enablereduce == ON){ travstate.skip_kvs = SRCBUFFER_SIZE; } 
	else{ travstate.skip_kvs = NUMPIPELINES * SRCBUFFER_SIZE; }
	travstate.i_kvs = travstate.begin_kvs;
	return travstate;	
}

// functions - dispatch
void 
	#ifdef SW 
	actslw::
	#endif 
readglobalstats0(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[NUMLASTLEVELPARTITIONS], keyvalue_t buffer[NUM_PARTITIONS], batch_type offset_kvs, step_type currentLOP, bool_type sourceORdest){
	if(enable == OFF){ return; }
	
	batch_type skipsize = getskipsize(currentLOP, sourceORdest);
	buffer_type loffset_kvs = 0;
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("readglobalstats0", BASEOFFSET_STATSDRAM_KVS + offset_kvs + NUM_PARTITIONS, PADDEDKVSOURCEDRAMSZ_KVS + 1, NAp, NAp, NAp);
	#endif
	READGLOBALSTATS_LOOP: for (buffer_type i=0; i<NUM_PARTITIONS; i++){
	#pragma HLS PIPELINE II=1
		#ifdef USEBRAMFORGLOBALSTATS
		buffer[i] = globalstatsbuffer[offset_kvs + loffset_kvs]; 
		#else 
		buffer[i] = kvdram[BASEOFFSET_STATSDRAM_KVS + offset_kvs + loffset_kvs].data[0]; 
		#endif 
		loffset_kvs += skipsize;
	}
	resetvalues(buffer, NUM_PARTITIONS);
	return;
}

void 
	#ifdef SW 
	actslw::
	#endif 
saveglobalstats0(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[NUMLASTLEVELPARTITIONS], keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE]){
	if(enable == OFF){ return; }
	
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("saveglobalstats0", BASEOFFSET_STATSDRAM_KVS + PADDEDDESTBUFFER_SIZE, PADDEDKVSOURCEDRAMSZ_KVS + 1, NAp, NAp, NAp);
	#endif
	SAVEGLOBALSTATS_LOOP: for (buffer_type i=0; i<PADDEDDESTBUFFER_SIZE; i++){
	#pragma HLS PIPELINE II=1
		#ifdef USEBRAMFORGLOBALSTATS
		globalstatsbuffer[i] = buffer[0][i];
		#else 
		kvdram[BASEOFFSET_STATSDRAM_KVS + i].data[0] = buffer[0][i]; 
		kvdram[BASEOFFSET_STATSDRAM_KVS + i].data[1] = buffer[1][i];
		kvdram[BASEOFFSET_STATSDRAM_KVS + i].data[2] = buffer[2][i]; 
		kvdram[BASEOFFSET_STATSDRAM_KVS + i].data[3] = buffer[3][i]; 
		kvdram[BASEOFFSET_STATSDRAM_KVS + i].data[4] = buffer[4][i]; 
		kvdram[BASEOFFSET_STATSDRAM_KVS + i].data[5] = buffer[5][i]; 
		kvdram[BASEOFFSET_STATSDRAM_KVS + i].data[6] = buffer[6][i]; 
		kvdram[BASEOFFSET_STATSDRAM_KVS + i].data[7] = buffer[7][i]; 
		#endif 
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalvar_savestats_counttotalstatswritten(VECTOR_SIZE);
		#endif
	}
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif 
updateglobalstats0(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[NUMLASTLEVELPARTITIONS], keyvalue_t buffer[NUM_PARTITIONS], buffer_type offset_kvs, step_type currentLOP, bool_type sourceORdest){
	if(enable == OFF){ return; }
	
	batch_type skipsize = getskipsize(currentLOP, sourceORdest);
	batch_type loffset_kvs = 0;
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("updateglobalstats0", BASEOFFSET_STATSDRAM_KVS + offset_kvs + NUM_PARTITIONS, PADDEDKVSOURCEDRAMSZ_KVS + 1, NAp, NAp, NAp);
	#endif
	READGLOBALSTATS_LOOP: for (buffer_type i=0; i<NUM_PARTITIONS; i++){
	#pragma HLS PIPELINE II=1
		#ifdef USEBRAMFORGLOBALSTATS
		globalstatsbuffer[offset_kvs + loffset_kvs].value = buffer[i].value; 
		#else 
		kvdram[BASEOFFSET_STATSDRAM_KVS + offset_kvs + loffset_kvs].data[0].value = buffer[i].value; 
		#endif 
		loffset_kvs += skipsize;
	}
	return;
}

void 
	#ifdef SW 
	actslw::
	#endif 
readkeyvalues0(bool_type enable, uint512_dt * kvdram, keyvalue_t buffer[VECTOR_SIZE][SRCBUFFER_SIZE], batch_type offset_kvs, travstate_t travstate){
	if(enable == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE;
	buffer_type chunk_size = getchunksize(SRCBUFFER_SIZE, travstate, 0);
	
	READKVS_LOOP: for (buffer_type i=0; i<chunk_size; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
	#pragma HLS PIPELINE II=1
		buffer[0][i] = kvdram[offset_kvs + i].data[0]; 
		buffer[1][i] = kvdram[offset_kvs + i].data[1]; 
		buffer[2][i] = kvdram[offset_kvs + i].data[2]; 
		buffer[3][i] = kvdram[offset_kvs + i].data[3]; 
		buffer[4][i] = kvdram[offset_kvs + i].data[4]; 
		buffer[5][i] = kvdram[offset_kvs + i].data[5]; 
		buffer[6][i] = kvdram[offset_kvs + i].data[6]; 
		buffer[7][i] = kvdram[offset_kvs + i].data[7]; 
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	return;
}

void 
	#ifdef SW 
	actslw::
	#endif 
readvertices0(bool_type enable, uint512_dt * kvdram, keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], batch_type offset_kvs){
	if(enable == OFF){ return; }
		
	READVERTICES_LOOP: for (buffer_type i=0; i<PADDEDDESTBUFFER_SIZE; i++){
	#pragma HLS PIPELINE II=1
		buffer[0][i] = kvdram[offset_kvs + i].data[0]; 
		buffer[1][i] = kvdram[offset_kvs + i].data[1]; 
		buffer[2][i] = kvdram[offset_kvs + i].data[2]; 
		buffer[3][i] = kvdram[offset_kvs + i].data[3]; 
		buffer[4][i] = kvdram[offset_kvs + i].data[4]; 
		buffer[5][i] = kvdram[offset_kvs + i].data[5]; 
		buffer[6][i] = kvdram[offset_kvs + i].data[6]; 
		buffer[7][i] = kvdram[offset_kvs + i].data[7]; 
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	return;
}

void 
	#ifdef SW 
	actslw::
	#endif 
savekeyvalues0(bool_type enable, uint512_dt * kvdram, uint512_dt buffer[PADDEDDESTBUFFER_SIZE], keyvalue_t * globalcapsule, skeyvalue_t localcapsule[NUM_PARTITIONS], batch_type globalbaseaddress_kvs){				
	if(enable == OFF){ return; }
	
	#ifdef _DEBUGMODE_CHECKS
	actsutilityobj->printkeyvalues("savekeyvalues0::localcapsule", localcapsule, NUM_PARTITIONS);
	actsutilityobj->printvaluecount("savekeyvalues0::localcapsule", localcapsule, NUM_PARTITIONS);
	actsutilityobj->scankeyvalues("savekeyvalues0::buffer", (keyvalue_t *)buffer, localcapsule, NUM_PARTITIONS, BATCH_RANGE / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif 

	analysis_type analysis_destpartitionsz = PADDEDDESTBUFFER_SIZE / NUM_PARTITIONS;
	SAVEPARTITIONS_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		batch_type dramoffset_kvs = globalbaseaddress_kvs + ((globalcapsule[p].key + globalcapsule[p].value) / VECTOR_SIZE);
		buffer_type bramoffset_kvs = localcapsule[p].key / VECTOR_SIZE;
		buffer_type realsize_kvs = localcapsule[p].value / VECTOR_SIZE;
		buffer_type size_kvs = getpartitionwritesz(realsize_kvs, bramoffset_kvs);
		
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("savekeyvalues0 23", bramoffset_kvs + size_kvs, PADDEDDESTBUFFER_SIZE + 1, p, NAp, NAp);
		#endif
		SAVEPARTITIONS_LOOP1B: for(buffer_type i=0; i<size_kvs; i++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_destpartitionsz avg=analysis_destpartitionsz
		#pragma HLS PIPELINE II=1
			kvdram[dramoffset_kvs + i] = buffer[bramoffset_kvs + i];
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countkvspartitionswritten_actual(VECTOR_SIZE);
			#endif
		}
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvspartitionswritten(realsize_kvs * VECTOR_SIZE);
		#endif
	}
	SAVEPARTITIONS_LOOP2: for(partition_type p=0; p<NUM_PARTITIONS; p++){ globalcapsule[p].value += localcapsule[p].value; }
	
	#ifdef _DEBUGMODE_CHECKS
	actsutilityobj->printkeyvalues("savekeyvalues0::globalcapsule", globalcapsule, NUM_PARTITIONS);
	actsutilityobj->scankeyvalues("savekeyvalues0::kvdram", (keyvalue_t *)(&kvdram[BASEOFFSET_KVDRAMWORKSPACE_KVS]), globalcapsule, NUM_PARTITIONS, BATCH_RANGE / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif 
	return;
}

void 
	#ifdef SW 
	actslw::
	#endif 
savevertices0(bool_type enable, uint512_dt * kvdram, keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], batch_type offset_kvs){
	if(enable == OFF){ return; }
		
	SAVEVERTICES_LOOP: for (buffer_type i=0; i<PADDEDDESTBUFFER_SIZE; i++){
	#pragma HLS PIPELINE II=1
		kvdram[offset_kvs + i].data[0] = buffer[0][i]; 
		kvdram[offset_kvs + i].data[1] = buffer[1][i]; 
		kvdram[offset_kvs + i].data[2] = buffer[2][i]; 
		kvdram[offset_kvs + i].data[3] = buffer[3][i]; 
		kvdram[offset_kvs + i].data[4] = buffer[4][i]; 
		kvdram[offset_kvs + i].data[5] = buffer[5][i]; 
		kvdram[offset_kvs + i].data[6] = buffer[6][i]; 
		kvdram[offset_kvs + i].data[7] = buffer[7][i]; 
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvswritten(VECTOR_SIZE);
		#endif
	}
	return;
}

void 
	#ifdef SW 
	actslw::
	#endif
collectglobalstats0(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][SRCBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], vertex_t upperlimit){
	if(enable == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE;

	COLLECTGLOBALSTATS_LOOP: for(buffer_type i=0; i<SRCBUFFER_SIZE; i++){
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
		
		partition_type p0 = getglobalpartition(keyvalue0, upperlimit);
		partition_type p1 = getglobalpartition(keyvalue1, upperlimit);
		partition_type p2 = getglobalpartition(keyvalue2, upperlimit);
		partition_type p3 = getglobalpartition(keyvalue3, upperlimit);
		partition_type p4 = getglobalpartition(keyvalue4, upperlimit);
		partition_type p5 = getglobalpartition(keyvalue5, upperlimit);
		partition_type p6 = getglobalpartition(keyvalue6, upperlimit);
		partition_type p7 = getglobalpartition(keyvalue7, upperlimit);
		
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[0]", p0, PADDEDDESTBUFFER_SIZE, keyvalue0.key, BATCH_RANGE, NAp);
		#endif 
		destbuffer[0][p0].value += 1;
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[1]", p1, PADDEDDESTBUFFER_SIZE, keyvalue1.key, BATCH_RANGE, NAp);
		#endif 
		destbuffer[1][p1].value += 1;
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[2]", p2, PADDEDDESTBUFFER_SIZE, keyvalue2.key, BATCH_RANGE, NAp);
		#endif 
		destbuffer[2][p2].value += 1;
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[3]", p3, PADDEDDESTBUFFER_SIZE, keyvalue3.key, BATCH_RANGE, NAp);
		#endif 
		destbuffer[3][p3].value += 1;
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[4]", p4, PADDEDDESTBUFFER_SIZE, keyvalue4.key, BATCH_RANGE, NAp);
		#endif 
		destbuffer[4][p4].value += 1;
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[5]", p5, PADDEDDESTBUFFER_SIZE, keyvalue5.key, BATCH_RANGE, NAp);
		#endif 
		destbuffer[5][p5].value += 1;
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[6]", p6, PADDEDDESTBUFFER_SIZE, keyvalue6.key, BATCH_RANGE, NAp);
		#endif 
		destbuffer[6][p6].value += 1;
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("collectglobalstats0. out of bounds for destbuffer[7]", p7, PADDEDDESTBUFFER_SIZE, keyvalue7.key, BATCH_RANGE, NAp);
		#endif 
		destbuffer[7][p7].value += 1;
	}
	return;
}

void 
	#ifdef SW 
	actslw::
	#endif
prepareglobalstats0(bool_type enable, keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE]){
	if(enable == OFF){ return; }
	
	PREPAREGLOBALSTATS_LOOP1: for(vector_type v=1; v<VECTOR_SIZE; v++){
		PREPAREGLOBALSTATS_LOOP1B: for(buffer_type i=0; i<PADDEDDESTBUFFER_SIZE; i++){
			buffer[0][i].value += buffer[v][i].value;
		}
	}

	batch_type skipsize = KVDRAMPADDING / NUMLASTLEVELPARTITIONS;
	calculateoffsets(buffer[0], NUMLASTLEVELPARTITIONS, 0, skipsize);
	resetvalues(buffer[0], PADDEDDESTBUFFER_SIZE);
	return;
}

void 
	#ifdef SW 
	actslw::
	#endif
partitionkeyvalues0(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][SRCBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t localcapsule[VECTOR_SIZE][NUM_PARTITIONS], step_type currentLOP, vertex_t upperlimit, travstate_t travstate){
	if(enable == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE;
	buffer_type chunk_size = getchunksize(SRCBUFFER_SIZE, travstate, 0);

	resetmanykeyandvalues(localcapsule, NUM_PARTITIONS);
	
	PARTITIONKEYVALUES_LOOP1: for(step_type c=0; c<2; c++){
		PARTITIONKEYVALUES_LOOP1B: for(buffer_type i=0; i<chunk_size; i++){
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
			if(keyvalue0.key != INVALIDDATA){ p0 = getpartition(keyvalue0, currentLOP, upperlimit); } 
			partition_type p1 = 0;
			if(keyvalue1.key != INVALIDDATA){ p1 = getpartition(keyvalue1, currentLOP, upperlimit); } 
			partition_type p2 = 0;
			if(keyvalue2.key != INVALIDDATA){ p2 = getpartition(keyvalue2, currentLOP, upperlimit); } 
			partition_type p3 = 0;
			if(keyvalue3.key != INVALIDDATA){ p3 = getpartition(keyvalue3, currentLOP, upperlimit); } 
			partition_type p4 = 0;
			if(keyvalue4.key != INVALIDDATA){ p4 = getpartition(keyvalue4, currentLOP, upperlimit); } 
			partition_type p5 = 0;
			if(keyvalue5.key != INVALIDDATA){ p5 = getpartition(keyvalue5, currentLOP, upperlimit); } 
			partition_type p6 = 0;
			if(keyvalue6.key != INVALIDDATA){ p6 = getpartition(keyvalue6, currentLOP, upperlimit); } 
			partition_type p7 = 0;
			if(keyvalue7.key != INVALIDDATA){ p7 = getpartition(keyvalue7, currentLOP, upperlimit); } 
			
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", localcapsule[0][p0].key + localcapsule[0][p0].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", localcapsule[1][p1].key + localcapsule[1][p1].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", localcapsule[2][p2].key + localcapsule[2][p2].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", localcapsule[3][p3].key + localcapsule[3][p3].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", localcapsule[4][p4].key + localcapsule[4][p4].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", localcapsule[5][p5].key + localcapsule[5][p5].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", localcapsule[6][p6].key + localcapsule[6][p6].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", localcapsule[7][p7].key + localcapsule[7][p7].value, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
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
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", p0, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", p1, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", p2, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", p3, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", p4, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", p5, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", p6, NUM_PARTITIONS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("partitionkeyvalues0", p7, NUM_PARTITIONS, NAp, NAp, NAp);
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
		if(c==0){ resetmanyvalues(localcapsule, NUM_PARTITIONS); }
	}
	
	#ifdef _DEBUGMODE_STATS
	for(vector_type v=0; v<VECTOR_SIZE; v++){ actsutilityobj->globalvar_inmemory_counttotalvalidkeyvalues(actsutilityobj->ugetvaluecount((keyvalue_t *)localcapsule[v], NUM_PARTITIONS)); } // REMOVEME. unmatched data types
	#endif
	return;
}

void 
	#ifdef SW 
	actslw::
	#endif
reduce0(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][SRCBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], vertex_t upperlimit, unsigned int GraphIter, unsigned int GraphAlgo, travstate_t travstate){
	if(enable == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE;
	buffer_type chunk_size = getchunksize(SRCBUFFER_SIZE, travstate, 0);

	REDUCE_LOOP: for(buffer_type i=0; i<chunk_size; i++){
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
		
		vertex_t loc0 = keyvalue0.key - upperlimit;
		vertex_t loc1 = keyvalue1.key - upperlimit;
		vertex_t loc2 = keyvalue2.key - upperlimit;
		vertex_t loc3 = keyvalue3.key - upperlimit;
		vertex_t loc4 = keyvalue4.key - upperlimit;
		vertex_t loc5 = keyvalue5.key - upperlimit;
		vertex_t loc6 = keyvalue6.key - upperlimit;
		vertex_t loc7 = keyvalue7.key - upperlimit;
		
		if(loc0 >= APPLYVERTEXBUFFERSZ && keyvalue0.key != INVALIDDATA){
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTREDUCEERRORS
			cout<<"ERROR SEEN @ reduce0:: i: "<<i<<", loc0: "<<loc0<<", keyvalue0.key: "<<keyvalue0.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<APPLYVERTEXBUFFERSZ <<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			exit(EXIT_FAILURE); // FIXME. PERFECTIONTEST.
			#endif 
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->globalstats_counterrorsinreduce(1);
			#endif
			loc0 = 0;
		} // REMOVEME.
		if(loc1 >= APPLYVERTEXBUFFERSZ && keyvalue1.key != INVALIDDATA){
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTREDUCEERRORS
			cout<<"ERROR SEEN @ reduce0:: i: "<<i<<", loc1: "<<loc1<<", keyvalue1.key: "<<keyvalue1.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<APPLYVERTEXBUFFERSZ <<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			exit(EXIT_FAILURE); // FIXME. PERFECTIONTEST.
			#endif 
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->globalstats_counterrorsinreduce(1);
			#endif
			loc1 = 0;
		} // REMOVEME.
		if(loc2 >= APPLYVERTEXBUFFERSZ && keyvalue2.key != INVALIDDATA){
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTREDUCEERRORS
			cout<<"ERROR SEEN @ reduce0:: i: "<<i<<", loc2: "<<loc2<<", keyvalue2.key: "<<keyvalue2.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<APPLYVERTEXBUFFERSZ <<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			exit(EXIT_FAILURE); // FIXME. PERFECTIONTEST.
			#endif 
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->globalstats_counterrorsinreduce(1);
			#endif
			loc2 = 0;
		} // REMOVEME.
		if(loc3 >= APPLYVERTEXBUFFERSZ && keyvalue3.key != INVALIDDATA){
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTREDUCEERRORS
			cout<<"ERROR SEEN @ reduce0:: i: "<<i<<", loc3: "<<loc3<<", keyvalue3.key: "<<keyvalue3.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<APPLYVERTEXBUFFERSZ <<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			exit(EXIT_FAILURE); // FIXME. PERFECTIONTEST.
			#endif 
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->globalstats_counterrorsinreduce(1);
			#endif
			loc3 = 0;
		} // REMOVEME.
		if(loc4 >= APPLYVERTEXBUFFERSZ && keyvalue4.key != INVALIDDATA){
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTREDUCEERRORS
			cout<<"ERROR SEEN @ reduce0:: i: "<<i<<", loc4: "<<loc4<<", keyvalue4.key: "<<keyvalue4.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<APPLYVERTEXBUFFERSZ <<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			exit(EXIT_FAILURE); // FIXME. PERFECTIONTEST.
			#endif 
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->globalstats_counterrorsinreduce(1);
			#endif
			loc4 = 0;
		} // REMOVEME.
		if(loc5 >= APPLYVERTEXBUFFERSZ && keyvalue5.key != INVALIDDATA){
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTREDUCEERRORS
			cout<<"ERROR SEEN @ reduce0:: i: "<<i<<", loc5: "<<loc5<<", keyvalue5.key: "<<keyvalue5.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<APPLYVERTEXBUFFERSZ <<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			exit(EXIT_FAILURE); // FIXME. PERFECTIONTEST.
			#endif 
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->globalstats_counterrorsinreduce(1);
			#endif
			loc5 = 0;
		} // REMOVEME.
		if(loc6 >= APPLYVERTEXBUFFERSZ && keyvalue6.key != INVALIDDATA){
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTREDUCEERRORS
			cout<<"ERROR SEEN @ reduce0:: i: "<<i<<", loc6: "<<loc6<<", keyvalue6.key: "<<keyvalue6.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<APPLYVERTEXBUFFERSZ <<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			exit(EXIT_FAILURE); // FIXME. PERFECTIONTEST.
			#endif 
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->globalstats_counterrorsinreduce(1);
			#endif
			loc6 = 0;
		} // REMOVEME.
		if(loc7 >= APPLYVERTEXBUFFERSZ && keyvalue7.key != INVALIDDATA){
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTREDUCEERRORS
			cout<<"ERROR SEEN @ reduce0:: i: "<<i<<", loc7: "<<loc7<<", keyvalue7.key: "<<keyvalue7.key<<", upperlimit: "<<upperlimit<<", APPLYVERTEXBUFFERSZ: "<<APPLYVERTEXBUFFERSZ <<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			exit(EXIT_FAILURE); // FIXME. PERFECTIONTEST.
			#endif 
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->globalstats_counterrorsinreduce(1);
			#endif
			loc7 = 0;
		} // REMOVEME.
		
		loc0 = loc0 / 2;
		loc1 = loc1 / 2;
		loc2 = loc2 / 2;
		loc3 = loc3 / 2;
		loc4 = loc4 / 2;
		loc5 = loc5 / 2;
		loc6 = loc6 / 2;
		loc7 = loc7 / 2;
		
		keyvalue_t vprop0;
		if(keyvalue0.key != INVALIDDATA){ vprop0 = destbuffer[0][loc0]; }
		keyvalue_t vprop1;
		if(keyvalue1.key != INVALIDDATA){ vprop1 = destbuffer[1][loc1]; }
		keyvalue_t vprop2;
		if(keyvalue2.key != INVALIDDATA){ vprop2 = destbuffer[2][loc2]; }
		keyvalue_t vprop3;
		if(keyvalue3.key != INVALIDDATA){ vprop3 = destbuffer[3][loc3]; }
		keyvalue_t vprop4;
		if(keyvalue4.key != INVALIDDATA){ vprop4 = destbuffer[4][loc4]; }
		keyvalue_t vprop5;
		if(keyvalue5.key != INVALIDDATA){ vprop5 = destbuffer[5][loc5]; }
		keyvalue_t vprop6;
		if(keyvalue6.key != INVALIDDATA){ vprop6 = destbuffer[6][loc6]; }
		keyvalue_t vprop7;
		if(keyvalue7.key != INVALIDDATA){ vprop7 = destbuffer[7][loc7]; }
		
		value_t temp0 = reducefunc(vprop0.key, vprop0.value, keyvalue0.value, GraphIter, GraphAlgo);
		value_t temp1 = reducefunc(vprop1.key, vprop1.value, keyvalue1.value, GraphIter, GraphAlgo);
		value_t temp2 = reducefunc(vprop2.key, vprop2.value, keyvalue2.value, GraphIter, GraphAlgo);
		value_t temp3 = reducefunc(vprop3.key, vprop3.value, keyvalue3.value, GraphIter, GraphAlgo);
		value_t temp4 = reducefunc(vprop4.key, vprop4.value, keyvalue4.value, GraphIter, GraphAlgo);
		value_t temp5 = reducefunc(vprop5.key, vprop5.value, keyvalue5.value, GraphIter, GraphAlgo);
		value_t temp6 = reducefunc(vprop6.key, vprop6.value, keyvalue6.value, GraphIter, GraphAlgo);
		value_t temp7 = reducefunc(vprop7.key, vprop7.value, keyvalue7.value, GraphIter, GraphAlgo);
		
		vprop0.value = temp0;
		vprop1.value = temp1;
		vprop2.value = temp2;
		vprop3.value = temp3;
		vprop4.value = temp4;
		vprop5.value = temp5;
		vprop6.value = temp6;
		vprop7.value = temp7;
		
		if(keyvalue0.key != INVALIDDATA){ destbuffer[0][loc0] = vprop0; }
		if(keyvalue1.key != INVALIDDATA){ destbuffer[1][loc1] = vprop1; }
		if(keyvalue2.key != INVALIDDATA){ destbuffer[2][loc2] = vprop2; }
		if(keyvalue3.key != INVALIDDATA){ destbuffer[3][loc3] = vprop3; }
		if(keyvalue4.key != INVALIDDATA){ destbuffer[4][loc4] = vprop4; }
		if(keyvalue5.key != INVALIDDATA){ destbuffer[5][loc5] = vprop5; }
		if(keyvalue6.key != INVALIDDATA){ destbuffer[6][loc6] = vprop6; }
		if(keyvalue7.key != INVALIDDATA){ destbuffer[7][loc7] = vprop7; }
		
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsreduced(VECTOR_SIZE);
		if(keyvalue0.key != INVALIDDATA){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(1); }
		if(keyvalue1.key != INVALIDDATA){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(1); }
		if(keyvalue2.key != INVALIDDATA){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(1); }
		if(keyvalue3.key != INVALIDDATA){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(1); }
		if(keyvalue4.key != INVALIDDATA){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(1); }
		if(keyvalue5.key != INVALIDDATA){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(1); }
		if(keyvalue6.key != INVALIDDATA){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(1); }
		if(keyvalue7.key != INVALIDDATA){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(1); }
		#endif
	}
	return;
}

void 
	#ifdef SW 
	actslw::
	#endif
combineSetof1stoSetof20_I0(bool_type enable, keyvalue_t buffer_setof1M[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof1N[PADDEDDESTBUFFER_SIZE], uint128_dt buffer_setof2[PADDEDDESTBUFFER_SIZE], skeyvalue_t localcapsuleM[NUM_PARTITIONS], skeyvalue_t localcapsuleN[NUM_PARTITIONS], skeyvalue_t localcapsuleR[NUM_PARTITIONS]){
	if(enable == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE / NUM_PARTITIONS;
	buffer_type index=0, begin=0, end=0;
	keyvalue_t NullKV; NullKV.key = INVALIDDATA; NullKV.value = INVALIDDATA;
	
	resetkeyandvalues(localcapsuleR, NUM_PARTITIONS);
	
	EXECUTE_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		buffer_type counter = 0;
		
		EXECUTE_LOOP1B: for(step_type i=0; i<2; i++){
			if(i==0){ begin = localcapsuleM[p].key; end = localcapsuleM[p].key + localcapsuleM[p].value; }
			else { begin = localcapsuleN[p].key; end = localcapsuleN[p].key + localcapsuleN[p].value; }
		
			EXECUTE_LOOP1C: for(buffer_type k=begin; k<end; k++){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("combineSetof1stoSetof20_I0 1", index, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("combineSetof1stoSetof20_I0 2", k, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
				#endif
				if(i==0){
					if(counter % 2 == 0){
						buffer_setof2[index].data[0] = buffer_setof1M[k];
					} else {
						buffer_setof2[index].data[1] = buffer_setof1M[k];
					}
				} else {
					if(counter % 2 == 0){
						buffer_setof2[index].data[0] = buffer_setof1N[k];
					} else {
						buffer_setof2[index].data[1] = buffer_setof1N[k];
					}
				}
				counter+=1; if(counter % 2 == 0){ index++; }
			}
		}
		
		localcapsuleR[p].value = localcapsuleM[p].value + localcapsuleN[p].value; 
		if(counter % 2 != 0){ buffer_setof2[index].data[1] = NullKV; localcapsuleR[p].value += 1; index += 1; } // edge conditions
		#ifdef _DEBUGMODE_STATS
		if(counter % 2 != 0){ actsutilityobj->globalvar_savepartitions_countinvalids(1); }
		#endif 
	}
	
	calculateunallignedoffsets(localcapsuleR, NUM_PARTITIONS, 0, 0);
	#ifdef _DEBUGMODE_CHECKS
	actsutilityobj->scankeyvalues("combineSetof1stoSetof20_I0::buffer_setof2M", (keyvalue_t *)buffer_setof1M, localcapsuleM, NUM_PARTITIONS, BATCH_RANGE / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof1stoSetof20_I0::buffer_setof2", (keyvalue_t *)buffer_setof2, localcapsuleR, NUM_PARTITIONS, BATCH_RANGE / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif
combineSetof1stoSetof20_I1(bool_type enable, keyvalue_t buffer_setof1M[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof1N[PADDEDDESTBUFFER_SIZE], uint128_dt buffer_setof2[PADDEDDESTBUFFER_SIZE], skeyvalue_t localcapsuleM[NUM_PARTITIONS], skeyvalue_t localcapsuleN[NUM_PARTITIONS], skeyvalue_t localcapsuleR[NUM_PARTITIONS]){
	if(enable == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE / NUM_PARTITIONS;
	buffer_type index=0, begin=0, end=0;
	keyvalue_t NullKV; NullKV.key = INVALIDDATA; NullKV.value = INVALIDDATA;
	
	resetkeyandvalues(localcapsuleR, NUM_PARTITIONS);
	
	EXECUTE_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		buffer_type counter = 0;
		
		EXECUTE_LOOP1B: for(step_type i=0; i<2; i++){
			if(i==0){ begin = localcapsuleM[p].key; end = localcapsuleM[p].key + localcapsuleM[p].value; }
			else { begin = localcapsuleN[p].key; end = localcapsuleN[p].key + localcapsuleN[p].value; }
		
			EXECUTE_LOOP1C: for(buffer_type k=begin; k<end; k++){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("combineSetof1stoSetof20_I1 1", index, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("combineSetof1stoSetof20_I1 2", k, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
				#endif
				if(i==0){
					if(counter % 2 == 0){
						buffer_setof2[index].data[0] = buffer_setof1M[k];
					} else {
						buffer_setof2[index].data[1] = buffer_setof1M[k];
					}
				} else {
					if(counter % 2 == 0){
						buffer_setof2[index].data[0] = buffer_setof1N[k];
					} else {
						buffer_setof2[index].data[1] = buffer_setof1N[k];
					}
				}
				counter+=1; if(counter % 2 == 0){ index++; }
			}
		}
		
		localcapsuleR[p].value = localcapsuleM[p].value + localcapsuleN[p].value; 
		if(counter % 2 != 0){ buffer_setof2[index].data[1] = NullKV; localcapsuleR[p].value += 1; index += 1; } // edge conditions
		#ifdef _DEBUGMODE_STATS
		if(counter % 2 != 0){ actsutilityobj->globalvar_savepartitions_countinvalids(1); }
		#endif 
	}
	
	calculateunallignedoffsets(localcapsuleR, NUM_PARTITIONS, 0, 0);
	#ifdef _DEBUGMODE_CHECKS
	actsutilityobj->scankeyvalues("combineSetof1stoSetof20_I1::buffer_setof2M", (keyvalue_t *)buffer_setof1M, localcapsuleM, NUM_PARTITIONS, BATCH_RANGE / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof1stoSetof20_I1::buffer_setof2", (keyvalue_t *)buffer_setof2, localcapsuleR, NUM_PARTITIONS, BATCH_RANGE / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif
combineSetof1stoSetof20_I2(bool_type enable, keyvalue_t buffer_setof1M[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof1N[PADDEDDESTBUFFER_SIZE], uint128_dt buffer_setof2[PADDEDDESTBUFFER_SIZE], skeyvalue_t localcapsuleM[NUM_PARTITIONS], skeyvalue_t localcapsuleN[NUM_PARTITIONS], skeyvalue_t localcapsuleR[NUM_PARTITIONS]){
	if(enable == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE / NUM_PARTITIONS;
	buffer_type index=0, begin=0, end=0;
	keyvalue_t NullKV; NullKV.key = INVALIDDATA; NullKV.value = INVALIDDATA;
	
	resetkeyandvalues(localcapsuleR, NUM_PARTITIONS);
	
	EXECUTE_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		buffer_type counter = 0;
		
		EXECUTE_LOOP1B: for(step_type i=0; i<2; i++){
			if(i==0){ begin = localcapsuleM[p].key; end = localcapsuleM[p].key + localcapsuleM[p].value; }
			else { begin = localcapsuleN[p].key; end = localcapsuleN[p].key + localcapsuleN[p].value; }
		
			EXECUTE_LOOP1C: for(buffer_type k=begin; k<end; k++){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("combineSetof1stoSetof20_I2 1", index, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("combineSetof1stoSetof20_I2 2", k, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
				#endif
				if(i==0){
					if(counter % 2 == 0){
						buffer_setof2[index].data[0] = buffer_setof1M[k];
					} else {
						buffer_setof2[index].data[1] = buffer_setof1M[k];
					}
				} else {
					if(counter % 2 == 0){
						buffer_setof2[index].data[0] = buffer_setof1N[k];
					} else {
						buffer_setof2[index].data[1] = buffer_setof1N[k];
					}
				}
				counter+=1; if(counter % 2 == 0){ index++; }
			}
		}
		
		localcapsuleR[p].value = localcapsuleM[p].value + localcapsuleN[p].value; 
		if(counter % 2 != 0){ buffer_setof2[index].data[1] = NullKV; localcapsuleR[p].value += 1; index += 1; } // edge conditions
		#ifdef _DEBUGMODE_STATS
		if(counter % 2 != 0){ actsutilityobj->globalvar_savepartitions_countinvalids(1); }
		#endif 
	}
	
	calculateunallignedoffsets(localcapsuleR, NUM_PARTITIONS, 0, 0);
	#ifdef _DEBUGMODE_CHECKS
	actsutilityobj->scankeyvalues("combineSetof1stoSetof20_I2::buffer_setof2M", (keyvalue_t *)buffer_setof1M, localcapsuleM, NUM_PARTITIONS, BATCH_RANGE / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof1stoSetof20_I2::buffer_setof2", (keyvalue_t *)buffer_setof2, localcapsuleR, NUM_PARTITIONS, BATCH_RANGE / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif
combineSetof1stoSetof20_I3(bool_type enable, keyvalue_t buffer_setof1M[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof1N[PADDEDDESTBUFFER_SIZE], uint128_dt buffer_setof2[PADDEDDESTBUFFER_SIZE], skeyvalue_t localcapsuleM[NUM_PARTITIONS], skeyvalue_t localcapsuleN[NUM_PARTITIONS], skeyvalue_t localcapsuleR[NUM_PARTITIONS]){
	if(enable == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE / NUM_PARTITIONS;
	buffer_type index=0, begin=0, end=0;
	keyvalue_t NullKV; NullKV.key = INVALIDDATA; NullKV.value = INVALIDDATA;
	
	resetkeyandvalues(localcapsuleR, NUM_PARTITIONS);
	
	EXECUTE_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		buffer_type counter = 0;
		
		EXECUTE_LOOP1B: for(step_type i=0; i<2; i++){
			if(i==0){ begin = localcapsuleM[p].key; end = localcapsuleM[p].key + localcapsuleM[p].value; }
			else { begin = localcapsuleN[p].key; end = localcapsuleN[p].key + localcapsuleN[p].value; }
		
			EXECUTE_LOOP1C: for(buffer_type k=begin; k<end; k++){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("combineSetof1stoSetof20_I3 1", index, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("combineSetof1stoSetof20_I3 2", k, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
				#endif
				if(i==0){
					if(counter % 2 == 0){
						buffer_setof2[index].data[0] = buffer_setof1M[k];
					} else {
						buffer_setof2[index].data[1] = buffer_setof1M[k];
					}
				} else {
					if(counter % 2 == 0){
						buffer_setof2[index].data[0] = buffer_setof1N[k];
					} else {
						buffer_setof2[index].data[1] = buffer_setof1N[k];
					}
				}
				counter+=1; if(counter % 2 == 0){ index++; }
			}
		}
		
		localcapsuleR[p].value = localcapsuleM[p].value + localcapsuleN[p].value; 
		if(counter % 2 != 0){ buffer_setof2[index].data[1] = NullKV; localcapsuleR[p].value += 1; index += 1; } // edge conditions
		#ifdef _DEBUGMODE_STATS
		if(counter % 2 != 0){ actsutilityobj->globalvar_savepartitions_countinvalids(1); }
		#endif 
	}
	
	calculateunallignedoffsets(localcapsuleR, NUM_PARTITIONS, 0, 0);
	#ifdef _DEBUGMODE_CHECKS
	actsutilityobj->scankeyvalues("combineSetof1stoSetof20_I3::buffer_setof2M", (keyvalue_t *)buffer_setof1M, localcapsuleM, NUM_PARTITIONS, BATCH_RANGE / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof1stoSetof20_I3::buffer_setof2", (keyvalue_t *)buffer_setof2, localcapsuleR, NUM_PARTITIONS, BATCH_RANGE / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif
combineSetof2stoSetof40_I0(bool_type enable, uint128_dt buffer_setof2M[PADDEDDESTBUFFER_SIZE], uint128_dt buffer_setof2N[PADDEDDESTBUFFER_SIZE], uint256_dt buffer_setof4[PADDEDDESTBUFFER_SIZE], skeyvalue_t localcapsuleM[NUM_PARTITIONS], skeyvalue_t localcapsuleN[NUM_PARTITIONS], skeyvalue_t localcapsuleR[NUM_PARTITIONS]){			
	if(enable == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE / NUM_PARTITIONS;
	buffer_type index=0, begin=0, end=0;
	keyvalue_t NullKV; NullKV.key = INVALIDDATA; NullKV.value = INVALIDDATA;
	
	resetkeyandvalues(localcapsuleR, NUM_PARTITIONS);
	
	EXECUTE_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		buffer_type counter = 0;
		
		EXECUTE_LOOP1B: for(step_type i=0; i<2; i++){
			
			if(i==0){ begin = localcapsuleM[p].key / 2; end = (localcapsuleM[p].key + localcapsuleM[p].value) / 2; }
			else { begin = localcapsuleN[p].key / 2; end = (localcapsuleN[p].key + localcapsuleN[p].value) / 2; }
			
			EXECUTE_LOOP1C: for(buffer_type k=begin; k<end; k++){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("combineSetof2stoSetof40_I0 1", index, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("combineSetof2stoSetof40_I0 2", k, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
				#endif
				if(i==0){
					if(counter % 2 == 0){
						buffer_setof4[index].data[0] = buffer_setof2M[k].data[0];
						buffer_setof4[index].data[1] = buffer_setof2M[k].data[1]; 
					} else {
						buffer_setof4[index].data[2] = buffer_setof2M[k].data[0];
						buffer_setof4[index].data[3] = buffer_setof2M[k].data[1]; 
					}
				} else {
					if(counter % 2 == 0){
						buffer_setof4[index].data[0] = buffer_setof2N[k].data[0];
						buffer_setof4[index].data[1] = buffer_setof2N[k].data[1]; 
					} else {
						buffer_setof4[index].data[2] = buffer_setof2N[k].data[0];
						buffer_setof4[index].data[3] = buffer_setof2N[k].data[1]; 
					}
				}
				counter+=1; if(counter % 2 == 0){ index++; }
			}
		}
		
		localcapsuleR[p].value = localcapsuleM[p].value + localcapsuleN[p].value; 
		if(counter % 2 != 0){ buffer_setof4[index].data[2] = NullKV; buffer_setof4[index].data[3] = NullKV; localcapsuleR[p].value += 2; index += 1; } // edge conditions
		#ifdef _DEBUGMODE_STATS
		if(counter % 2 != 0){ actsutilityobj->globalvar_savepartitions_countinvalids(2); }
		#endif 
	}
	
	calculateunallignedoffsets(localcapsuleR, NUM_PARTITIONS, 0, 0);
	#ifdef _DEBUGMODE_CHECKS
	actsutilityobj->scankeyvalues("combineSetof2stoSetof40_I0::buffer_setof2M", (keyvalue_t *)buffer_setof2M, localcapsuleM, NUM_PARTITIONS, BATCH_RANGE / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof2stoSetof40_I0::buffer_setof2N", (keyvalue_t *)buffer_setof2N, localcapsuleN, NUM_PARTITIONS, BATCH_RANGE / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof2stoSetof40_I0::buffer_setof4", (keyvalue_t *)buffer_setof4, localcapsuleR, NUM_PARTITIONS, BATCH_RANGE / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif
combineSetof2stoSetof40_I1(bool_type enable, uint128_dt buffer_setof2M[PADDEDDESTBUFFER_SIZE], uint128_dt buffer_setof2N[PADDEDDESTBUFFER_SIZE], uint256_dt buffer_setof4[PADDEDDESTBUFFER_SIZE], skeyvalue_t localcapsuleM[NUM_PARTITIONS], skeyvalue_t localcapsuleN[NUM_PARTITIONS], skeyvalue_t localcapsuleR[NUM_PARTITIONS]){			
	if(enable == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE / NUM_PARTITIONS;
	buffer_type index=0, begin=0, end=0;
	keyvalue_t NullKV; NullKV.key = INVALIDDATA; NullKV.value = INVALIDDATA;
	
	resetkeyandvalues(localcapsuleR, NUM_PARTITIONS);
	
	EXECUTE_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		buffer_type counter = 0;
		
		EXECUTE_LOOP1B: for(step_type i=0; i<2; i++){
			
			if(i==0){ begin = localcapsuleM[p].key / 2; end = (localcapsuleM[p].key + localcapsuleM[p].value) / 2; }
			else { begin = localcapsuleN[p].key / 2; end = (localcapsuleN[p].key + localcapsuleN[p].value) / 2; }
			
			EXECUTE_LOOP1C: for(buffer_type k=begin; k<end; k++){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("combineSetof2stoSetof40_I1 1", index, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("combineSetof2stoSetof40_I1 2", k, PADDEDDESTBUFFER_SIZE, NAp, NAp, NAp);
				#endif
				if(i==0){
					if(counter % 2 == 0){
						buffer_setof4[index].data[0] = buffer_setof2M[k].data[0];
						buffer_setof4[index].data[1] = buffer_setof2M[k].data[1]; 
					} else {
						buffer_setof4[index].data[2] = buffer_setof2M[k].data[0];
						buffer_setof4[index].data[3] = buffer_setof2M[k].data[1]; 
					}
				} else {
					if(counter % 2 == 0){
						buffer_setof4[index].data[0] = buffer_setof2N[k].data[0];
						buffer_setof4[index].data[1] = buffer_setof2N[k].data[1]; 
					} else {
						buffer_setof4[index].data[2] = buffer_setof2N[k].data[0];
						buffer_setof4[index].data[3] = buffer_setof2N[k].data[1]; 
					}
				}
				counter+=1; if(counter % 2 == 0){ index++; }
			}
		}
		
		localcapsuleR[p].value = localcapsuleM[p].value + localcapsuleN[p].value; 
		if(counter % 2 != 0){ buffer_setof4[index].data[2] = NullKV; buffer_setof4[index].data[3] = NullKV; localcapsuleR[p].value += 2; index += 1; } // edge conditions
		#ifdef _DEBUGMODE_STATS
		if(counter % 2 != 0){ actsutilityobj->globalvar_savepartitions_countinvalids(2); }
		#endif 
	}
	
	calculateunallignedoffsets(localcapsuleR, NUM_PARTITIONS, 0, 0);
	#ifdef _DEBUGMODE_CHECKS
	actsutilityobj->scankeyvalues("combineSetof2stoSetof40_I1::buffer_setof2M", (keyvalue_t *)buffer_setof2M, localcapsuleM, NUM_PARTITIONS, BATCH_RANGE / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof2stoSetof40_I1::buffer_setof2N", (keyvalue_t *)buffer_setof2N, localcapsuleN, NUM_PARTITIONS, BATCH_RANGE / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	actsutilityobj->scankeyvalues("combineSetof2stoSetof40_I1::buffer_setof4", (keyvalue_t *)buffer_setof4, localcapsuleR, NUM_PARTITIONS, BATCH_RANGE / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif
combineSetof4stoSetof80_I0(bool_type enable, uint256_dt buffer_setof4M[PADDEDDESTBUFFER_SIZE], uint256_dt buffer_setof4N[PADDEDDESTBUFFER_SIZE], uint512_dt buffer_setof8[PADDEDDESTBUFFER_SIZE], skeyvalue_t localcapsuleM[NUM_PARTITIONS], skeyvalue_t localcapsuleN[NUM_PARTITIONS], skeyvalue_t localcapsuleR[NUM_PARTITIONS]){
	if(enable == OFF){ return; }
	analysis_type analysis_srcbuffersz = SRCBUFFER_SIZE / NUM_PARTITIONS;
	buffer_type index=0, begin=0, end=0;
	keyvalue_t NullKV; NullKV.key = INVALIDDATA; NullKV.value = INVALIDDATA;
	
	resetkeyandvalues(localcapsuleR, NUM_PARTITIONS);
	
	EXECUTE_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		buffer_type counter = 0;
		
		EXECUTE_LOOP1B: for(step_type i=0; i<2; i++){
			if(i==0){ begin = localcapsuleM[p].key / 4; end = (localcapsuleM[p].key + localcapsuleM[p].value) / 4; }
			else { begin = localcapsuleN[p].key / 4; end = (localcapsuleN[p].key + localcapsuleN[p].value) / 4; }
			
			EXECUTE_LOOP1C: for(buffer_type k=begin; k<end; k++){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("combineSetof4stoSetof80_I0 1", index, PADDEDDESTBUFFER_SIZE, p, counter, NAp);
				actsutilityobj->checkoutofbounds("combineSetof4stoSetof80_I0 2", k, PADDEDDESTBUFFER_SIZE, p, counter, NAp);
				#endif 
				if(i==0){
					if(counter % 2 == 0){
						buffer_setof8[index].data[0] = buffer_setof4M[k].data[0];
						buffer_setof8[index].data[1] = buffer_setof4M[k].data[1]; 
						buffer_setof8[index].data[2] = buffer_setof4M[k].data[2]; 
						buffer_setof8[index].data[3] = buffer_setof4M[k].data[3];
					} else {
						buffer_setof8[index].data[4] = buffer_setof4M[k].data[0];
						buffer_setof8[index].data[5] = buffer_setof4M[k].data[1]; 
						buffer_setof8[index].data[6] = buffer_setof4M[k].data[2]; 
						buffer_setof8[index].data[7] = buffer_setof4M[k].data[3];
					}
				} else {
					if(counter % 2 == 0){
						buffer_setof8[index].data[0] = buffer_setof4N[k].data[0];
						buffer_setof8[index].data[1] = buffer_setof4N[k].data[1]; 
						buffer_setof8[index].data[2] = buffer_setof4N[k].data[2]; 
						buffer_setof8[index].data[3] = buffer_setof4N[k].data[3];
					} else {
						buffer_setof8[index].data[4] = buffer_setof4N[k].data[0];
						buffer_setof8[index].data[5] = buffer_setof4N[k].data[1]; 
						buffer_setof8[index].data[6] = buffer_setof4N[k].data[2]; 
						buffer_setof8[index].data[7] = buffer_setof4N[k].data[3];
					}
				}
				counter+=1; if(counter % 2 == 0){ index++; }
			}
		}
		
		localcapsuleR[p].value = localcapsuleM[p].value + localcapsuleN[p].value; 
		if(counter % 2 != 0){ buffer_setof8[index].data[4] = NullKV; buffer_setof8[index].data[5] = NullKV; 
			buffer_setof8[index].data[6] = NullKV; buffer_setof8[index].data[7] = NullKV;
			localcapsuleR[p].value += 4; index += 1; }
		#ifdef _DEBUGMODE_STATS
		if(counter % 2 != 0){ actsutilityobj->globalvar_savepartitions_countinvalids(4); }
		#endif 
	}
	
	calculateunallignedoffsets(localcapsuleR, NUM_PARTITIONS, 0, 0);
	#ifdef _DEBUGMODE_CHECKS
	actsutilityobj->scankeyvalues("combineSetof4stoSetof80_I0::buffer_setof2M", (keyvalue_t *)buffer_setof8, localcapsuleR, NUM_PARTITIONS, BATCH_RANGE / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	return;
}

// functions - merge 
void 
	#ifdef SW 
	actslw::
	#endif 
readmany(unsigned int offset_kvs  ,uint512_dt * kvdram0 ,keyvalue_t source0buffer[VECTOR_SIZE][SRCBUFFER_SIZE]  ,uint512_dt * kvdram1 ,keyvalue_t source1buffer[VECTOR_SIZE][SRCBUFFER_SIZE]  ,uint512_dt * kvdram2 ,keyvalue_t source2buffer[VECTOR_SIZE][SRCBUFFER_SIZE]  ,uint512_dt * kvdram3 ,keyvalue_t source3buffer[VECTOR_SIZE][SRCBUFFER_SIZE]  ,uint512_dt * kvdram4 ,keyvalue_t source4buffer[VECTOR_SIZE][SRCBUFFER_SIZE]  ,uint512_dt * kvdram5 ,keyvalue_t source5buffer[VECTOR_SIZE][SRCBUFFER_SIZE]  ,uint512_dt * kvdram6 ,keyvalue_t source6buffer[VECTOR_SIZE][SRCBUFFER_SIZE]  ,uint512_dt * kvdram7 ,keyvalue_t source7buffer[VECTOR_SIZE][SRCBUFFER_SIZE]  ,uint512_dt * kvdram8 ,keyvalue_t source8buffer[VECTOR_SIZE][SRCBUFFER_SIZE]  ,uint512_dt * kvdram9 ,keyvalue_t source9buffer[VECTOR_SIZE][SRCBUFFER_SIZE]  ,uint512_dt * kvdram10 ,keyvalue_t source10buffer[VECTOR_SIZE][SRCBUFFER_SIZE]  ,uint512_dt * kvdram11 ,keyvalue_t source11buffer[VECTOR_SIZE][SRCBUFFER_SIZE]  ,uint512_dt * kvdram12 ,keyvalue_t source12buffer[VECTOR_SIZE][SRCBUFFER_SIZE]  ,uint512_dt * kvdram13 ,keyvalue_t source13buffer[VECTOR_SIZE][SRCBUFFER_SIZE]  ,uint512_dt * kvdram14 ,keyvalue_t source14buffer[VECTOR_SIZE][SRCBUFFER_SIZE]  ,uint512_dt * kvdram15 ,keyvalue_t source15buffer[VECTOR_SIZE][SRCBUFFER_SIZE]  ){
	for(unsigned int i=0; i<SRCBUFFER_SIZE; i++){
	#pragma HLS PIPELINE II=1

		source0buffer[0][i] = kvdram0[offset_kvs + i].data[0]; 
		source0buffer[1][i] = kvdram0[offset_kvs + i].data[1]; 
		source0buffer[2][i] = kvdram0[offset_kvs + i].data[2]; 
		source0buffer[3][i] = kvdram0[offset_kvs + i].data[3]; 
		source0buffer[4][i] = kvdram0[offset_kvs + i].data[4]; 
		source0buffer[5][i] = kvdram0[offset_kvs + i].data[5]; 
		source0buffer[6][i] = kvdram0[offset_kvs + i].data[6]; 
		source0buffer[7][i] = kvdram0[offset_kvs + i].data[7]; 
		source1buffer[0][i] = kvdram1[offset_kvs + i].data[0]; 
		source1buffer[1][i] = kvdram1[offset_kvs + i].data[1]; 
		source1buffer[2][i] = kvdram1[offset_kvs + i].data[2]; 
		source1buffer[3][i] = kvdram1[offset_kvs + i].data[3]; 
		source1buffer[4][i] = kvdram1[offset_kvs + i].data[4]; 
		source1buffer[5][i] = kvdram1[offset_kvs + i].data[5]; 
		source1buffer[6][i] = kvdram1[offset_kvs + i].data[6]; 
		source1buffer[7][i] = kvdram1[offset_kvs + i].data[7]; 
		source2buffer[0][i] = kvdram2[offset_kvs + i].data[0]; 
		source2buffer[1][i] = kvdram2[offset_kvs + i].data[1]; 
		source2buffer[2][i] = kvdram2[offset_kvs + i].data[2]; 
		source2buffer[3][i] = kvdram2[offset_kvs + i].data[3]; 
		source2buffer[4][i] = kvdram2[offset_kvs + i].data[4]; 
		source2buffer[5][i] = kvdram2[offset_kvs + i].data[5]; 
		source2buffer[6][i] = kvdram2[offset_kvs + i].data[6]; 
		source2buffer[7][i] = kvdram2[offset_kvs + i].data[7]; 
		source3buffer[0][i] = kvdram3[offset_kvs + i].data[0]; 
		source3buffer[1][i] = kvdram3[offset_kvs + i].data[1]; 
		source3buffer[2][i] = kvdram3[offset_kvs + i].data[2]; 
		source3buffer[3][i] = kvdram3[offset_kvs + i].data[3]; 
		source3buffer[4][i] = kvdram3[offset_kvs + i].data[4]; 
		source3buffer[5][i] = kvdram3[offset_kvs + i].data[5]; 
		source3buffer[6][i] = kvdram3[offset_kvs + i].data[6]; 
		source3buffer[7][i] = kvdram3[offset_kvs + i].data[7]; 
		source4buffer[0][i] = kvdram4[offset_kvs + i].data[0]; 
		source4buffer[1][i] = kvdram4[offset_kvs + i].data[1]; 
		source4buffer[2][i] = kvdram4[offset_kvs + i].data[2]; 
		source4buffer[3][i] = kvdram4[offset_kvs + i].data[3]; 
		source4buffer[4][i] = kvdram4[offset_kvs + i].data[4]; 
		source4buffer[5][i] = kvdram4[offset_kvs + i].data[5]; 
		source4buffer[6][i] = kvdram4[offset_kvs + i].data[6]; 
		source4buffer[7][i] = kvdram4[offset_kvs + i].data[7]; 
		source5buffer[0][i] = kvdram5[offset_kvs + i].data[0]; 
		source5buffer[1][i] = kvdram5[offset_kvs + i].data[1]; 
		source5buffer[2][i] = kvdram5[offset_kvs + i].data[2]; 
		source5buffer[3][i] = kvdram5[offset_kvs + i].data[3]; 
		source5buffer[4][i] = kvdram5[offset_kvs + i].data[4]; 
		source5buffer[5][i] = kvdram5[offset_kvs + i].data[5]; 
		source5buffer[6][i] = kvdram5[offset_kvs + i].data[6]; 
		source5buffer[7][i] = kvdram5[offset_kvs + i].data[7]; 
		source6buffer[0][i] = kvdram6[offset_kvs + i].data[0]; 
		source6buffer[1][i] = kvdram6[offset_kvs + i].data[1]; 
		source6buffer[2][i] = kvdram6[offset_kvs + i].data[2]; 
		source6buffer[3][i] = kvdram6[offset_kvs + i].data[3]; 
		source6buffer[4][i] = kvdram6[offset_kvs + i].data[4]; 
		source6buffer[5][i] = kvdram6[offset_kvs + i].data[5]; 
		source6buffer[6][i] = kvdram6[offset_kvs + i].data[6]; 
		source6buffer[7][i] = kvdram6[offset_kvs + i].data[7]; 
		source7buffer[0][i] = kvdram7[offset_kvs + i].data[0]; 
		source7buffer[1][i] = kvdram7[offset_kvs + i].data[1]; 
		source7buffer[2][i] = kvdram7[offset_kvs + i].data[2]; 
		source7buffer[3][i] = kvdram7[offset_kvs + i].data[3]; 
		source7buffer[4][i] = kvdram7[offset_kvs + i].data[4]; 
		source7buffer[5][i] = kvdram7[offset_kvs + i].data[5]; 
		source7buffer[6][i] = kvdram7[offset_kvs + i].data[6]; 
		source7buffer[7][i] = kvdram7[offset_kvs + i].data[7]; 
		source8buffer[0][i] = kvdram8[offset_kvs + i].data[0]; 
		source8buffer[1][i] = kvdram8[offset_kvs + i].data[1]; 
		source8buffer[2][i] = kvdram8[offset_kvs + i].data[2]; 
		source8buffer[3][i] = kvdram8[offset_kvs + i].data[3]; 
		source8buffer[4][i] = kvdram8[offset_kvs + i].data[4]; 
		source8buffer[5][i] = kvdram8[offset_kvs + i].data[5]; 
		source8buffer[6][i] = kvdram8[offset_kvs + i].data[6]; 
		source8buffer[7][i] = kvdram8[offset_kvs + i].data[7]; 
		source9buffer[0][i] = kvdram9[offset_kvs + i].data[0]; 
		source9buffer[1][i] = kvdram9[offset_kvs + i].data[1]; 
		source9buffer[2][i] = kvdram9[offset_kvs + i].data[2]; 
		source9buffer[3][i] = kvdram9[offset_kvs + i].data[3]; 
		source9buffer[4][i] = kvdram9[offset_kvs + i].data[4]; 
		source9buffer[5][i] = kvdram9[offset_kvs + i].data[5]; 
		source9buffer[6][i] = kvdram9[offset_kvs + i].data[6]; 
		source9buffer[7][i] = kvdram9[offset_kvs + i].data[7]; 
		source10buffer[0][i] = kvdram10[offset_kvs + i].data[0]; 
		source10buffer[1][i] = kvdram10[offset_kvs + i].data[1]; 
		source10buffer[2][i] = kvdram10[offset_kvs + i].data[2]; 
		source10buffer[3][i] = kvdram10[offset_kvs + i].data[3]; 
		source10buffer[4][i] = kvdram10[offset_kvs + i].data[4]; 
		source10buffer[5][i] = kvdram10[offset_kvs + i].data[5]; 
		source10buffer[6][i] = kvdram10[offset_kvs + i].data[6]; 
		source10buffer[7][i] = kvdram10[offset_kvs + i].data[7]; 
		source11buffer[0][i] = kvdram11[offset_kvs + i].data[0]; 
		source11buffer[1][i] = kvdram11[offset_kvs + i].data[1]; 
		source11buffer[2][i] = kvdram11[offset_kvs + i].data[2]; 
		source11buffer[3][i] = kvdram11[offset_kvs + i].data[3]; 
		source11buffer[4][i] = kvdram11[offset_kvs + i].data[4]; 
		source11buffer[5][i] = kvdram11[offset_kvs + i].data[5]; 
		source11buffer[6][i] = kvdram11[offset_kvs + i].data[6]; 
		source11buffer[7][i] = kvdram11[offset_kvs + i].data[7]; 
		source12buffer[0][i] = kvdram12[offset_kvs + i].data[0]; 
		source12buffer[1][i] = kvdram12[offset_kvs + i].data[1]; 
		source12buffer[2][i] = kvdram12[offset_kvs + i].data[2]; 
		source12buffer[3][i] = kvdram12[offset_kvs + i].data[3]; 
		source12buffer[4][i] = kvdram12[offset_kvs + i].data[4]; 
		source12buffer[5][i] = kvdram12[offset_kvs + i].data[5]; 
		source12buffer[6][i] = kvdram12[offset_kvs + i].data[6]; 
		source12buffer[7][i] = kvdram12[offset_kvs + i].data[7]; 
		source13buffer[0][i] = kvdram13[offset_kvs + i].data[0]; 
		source13buffer[1][i] = kvdram13[offset_kvs + i].data[1]; 
		source13buffer[2][i] = kvdram13[offset_kvs + i].data[2]; 
		source13buffer[3][i] = kvdram13[offset_kvs + i].data[3]; 
		source13buffer[4][i] = kvdram13[offset_kvs + i].data[4]; 
		source13buffer[5][i] = kvdram13[offset_kvs + i].data[5]; 
		source13buffer[6][i] = kvdram13[offset_kvs + i].data[6]; 
		source13buffer[7][i] = kvdram13[offset_kvs + i].data[7]; 
		source14buffer[0][i] = kvdram14[offset_kvs + i].data[0]; 
		source14buffer[1][i] = kvdram14[offset_kvs + i].data[1]; 
		source14buffer[2][i] = kvdram14[offset_kvs + i].data[2]; 
		source14buffer[3][i] = kvdram14[offset_kvs + i].data[3]; 
		source14buffer[4][i] = kvdram14[offset_kvs + i].data[4]; 
		source14buffer[5][i] = kvdram14[offset_kvs + i].data[5]; 
		source14buffer[6][i] = kvdram14[offset_kvs + i].data[6]; 
		source14buffer[7][i] = kvdram14[offset_kvs + i].data[7]; 
		source15buffer[0][i] = kvdram15[offset_kvs + i].data[0]; 
		source15buffer[1][i] = kvdram15[offset_kvs + i].data[1]; 
		source15buffer[2][i] = kvdram15[offset_kvs + i].data[2]; 
		source15buffer[3][i] = kvdram15[offset_kvs + i].data[3]; 
		source15buffer[4][i] = kvdram15[offset_kvs + i].data[4]; 
		source15buffer[5][i] = kvdram15[offset_kvs + i].data[5]; 
		source15buffer[6][i] = kvdram15[offset_kvs + i].data[6]; 
		source15buffer[7][i] = kvdram15[offset_kvs + i].data[7]; 
		
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	return;
}

void 
	#ifdef SW 
	actslw::
	#endif 
saveone(unsigned int offset_kvs, uint512_dt * kvdram, uint512_dt destbuffer[PADDEDDESTBUFFER_SIZE]){
	for(unsigned int i=0; i<SRCBUFFER_SIZE; i++){
	#pragma HLS PIPELINE II=1
		kvdram[offset_kvs + i] = destbuffer[i];
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_reduce_countvalidkvsreduced(VECTOR_SIZE);
		#endif
	}
	return;
}

keyvalue_t 
	#ifdef SW 
	actslw::
	#endif
mergefunc(keyvalue_t keyvalue1, keyvalue_t keyvalue2, keyvalue_t keyvalue3, keyvalue_t keyvalue4){
	#pragma HLS INLINE
	keyvalue_t keyvalue;
	keyvalue.key = keyvalue1.key + keyvalue2.key + keyvalue3.key + keyvalue4.key;
	keyvalue.value = keyvalue1.value + keyvalue2.value + keyvalue3.value + keyvalue4.value;
	return keyvalue;
}

void 
	#ifdef SW 
	actslw::
	#endif
merge_I0(unsigned int enable, keyvalue_t source1buffer[8][SRCBUFFER_SIZE], keyvalue_t source2buffer[8][SRCBUFFER_SIZE], keyvalue_t source3buffer[8][SRCBUFFER_SIZE], keyvalue_t source4buffer[8][SRCBUFFER_SIZE], keyvalue_t destbuffer[8][PADDEDDESTBUFFER_SIZE]){
	if(enable == OFF){ return; }
	
	MERGE_LOOP1B: for(buffer_type i=0; i<SRCBUFFER_SIZE; i++){
	#pragma HLS PIPELINE II=2
		keyvalue_t source1keyvalue0 = source1buffer[0][i];
		keyvalue_t source2keyvalue0 = source2buffer[0][i];
		keyvalue_t source3keyvalue0 = source3buffer[0][i];
		keyvalue_t source4keyvalue0 = source4buffer[0][i];
		keyvalue_t destkeyvalue0;
		keyvalue_t source1keyvalue1 = source1buffer[1][i];
		keyvalue_t source2keyvalue1 = source2buffer[1][i];
		keyvalue_t source3keyvalue1 = source3buffer[1][i];
		keyvalue_t source4keyvalue1 = source4buffer[1][i];
		keyvalue_t destkeyvalue1;
		keyvalue_t source1keyvalue2 = source1buffer[2][i];
		keyvalue_t source2keyvalue2 = source2buffer[2][i];
		keyvalue_t source3keyvalue2 = source3buffer[2][i];
		keyvalue_t source4keyvalue2 = source4buffer[2][i];
		keyvalue_t destkeyvalue2;
		keyvalue_t source1keyvalue3 = source1buffer[3][i];
		keyvalue_t source2keyvalue3 = source2buffer[3][i];
		keyvalue_t source3keyvalue3 = source3buffer[3][i];
		keyvalue_t source4keyvalue3 = source4buffer[3][i];
		keyvalue_t destkeyvalue3;
		keyvalue_t source1keyvalue4 = source1buffer[4][i];
		keyvalue_t source2keyvalue4 = source2buffer[4][i];
		keyvalue_t source3keyvalue4 = source3buffer[4][i];
		keyvalue_t source4keyvalue4 = source4buffer[4][i];
		keyvalue_t destkeyvalue4;
		keyvalue_t source1keyvalue5 = source1buffer[5][i];
		keyvalue_t source2keyvalue5 = source2buffer[5][i];
		keyvalue_t source3keyvalue5 = source3buffer[5][i];
		keyvalue_t source4keyvalue5 = source4buffer[5][i];
		keyvalue_t destkeyvalue5;
		keyvalue_t source1keyvalue6 = source1buffer[6][i];
		keyvalue_t source2keyvalue6 = source2buffer[6][i];
		keyvalue_t source3keyvalue6 = source3buffer[6][i];
		keyvalue_t source4keyvalue6 = source4buffer[6][i];
		keyvalue_t destkeyvalue6;
		keyvalue_t source1keyvalue7 = source1buffer[7][i];
		keyvalue_t source2keyvalue7 = source2buffer[7][i];
		keyvalue_t source3keyvalue7 = source3buffer[7][i];
		keyvalue_t source4keyvalue7 = source4buffer[7][i];
		keyvalue_t destkeyvalue7;
		
		destkeyvalue0 = mergefunc(source1keyvalue0, source2keyvalue0, source3keyvalue0, source4keyvalue0);
		destkeyvalue1 = mergefunc(source1keyvalue1, source2keyvalue1, source3keyvalue1, source4keyvalue1);
		destkeyvalue2 = mergefunc(source1keyvalue2, source2keyvalue2, source3keyvalue2, source4keyvalue2);
		destkeyvalue3 = mergefunc(source1keyvalue3, source2keyvalue3, source3keyvalue3, source4keyvalue3);
		destkeyvalue4 = mergefunc(source1keyvalue4, source2keyvalue4, source3keyvalue4, source4keyvalue4);
		destkeyvalue5 = mergefunc(source1keyvalue5, source2keyvalue5, source3keyvalue5, source4keyvalue5);
		destkeyvalue6 = mergefunc(source1keyvalue6, source2keyvalue6, source3keyvalue6, source4keyvalue6);
		destkeyvalue7 = mergefunc(source1keyvalue7, source2keyvalue7, source3keyvalue7, source4keyvalue7);
		
		destbuffer[0][i] = destkeyvalue0;
		destbuffer[1][i] = destkeyvalue1;
		destbuffer[2][i] = destkeyvalue2;
		destbuffer[3][i] = destkeyvalue3;
		destbuffer[4][i] = destkeyvalue4;
		destbuffer[5][i] = destkeyvalue5;
		destbuffer[6][i] = destkeyvalue6;
		destbuffer[7][i] = destkeyvalue7;
		
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsmerged(VECTOR_SIZE * 4);
		#endif
	}
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif
merge_I1(unsigned int enable, keyvalue_t source1buffer[8][SRCBUFFER_SIZE], keyvalue_t source2buffer[8][SRCBUFFER_SIZE], keyvalue_t source3buffer[8][SRCBUFFER_SIZE], keyvalue_t source4buffer[8][SRCBUFFER_SIZE], keyvalue_t destbuffer[8][PADDEDDESTBUFFER_SIZE]){
	if(enable == OFF){ return; }
	
	MERGE_LOOP1B: for(buffer_type i=0; i<SRCBUFFER_SIZE; i++){
	#pragma HLS PIPELINE II=2
		keyvalue_t source1keyvalue0 = source1buffer[0][i];
		keyvalue_t source2keyvalue0 = source2buffer[0][i];
		keyvalue_t source3keyvalue0 = source3buffer[0][i];
		keyvalue_t source4keyvalue0 = source4buffer[0][i];
		keyvalue_t destkeyvalue0;
		keyvalue_t source1keyvalue1 = source1buffer[1][i];
		keyvalue_t source2keyvalue1 = source2buffer[1][i];
		keyvalue_t source3keyvalue1 = source3buffer[1][i];
		keyvalue_t source4keyvalue1 = source4buffer[1][i];
		keyvalue_t destkeyvalue1;
		keyvalue_t source1keyvalue2 = source1buffer[2][i];
		keyvalue_t source2keyvalue2 = source2buffer[2][i];
		keyvalue_t source3keyvalue2 = source3buffer[2][i];
		keyvalue_t source4keyvalue2 = source4buffer[2][i];
		keyvalue_t destkeyvalue2;
		keyvalue_t source1keyvalue3 = source1buffer[3][i];
		keyvalue_t source2keyvalue3 = source2buffer[3][i];
		keyvalue_t source3keyvalue3 = source3buffer[3][i];
		keyvalue_t source4keyvalue3 = source4buffer[3][i];
		keyvalue_t destkeyvalue3;
		keyvalue_t source1keyvalue4 = source1buffer[4][i];
		keyvalue_t source2keyvalue4 = source2buffer[4][i];
		keyvalue_t source3keyvalue4 = source3buffer[4][i];
		keyvalue_t source4keyvalue4 = source4buffer[4][i];
		keyvalue_t destkeyvalue4;
		keyvalue_t source1keyvalue5 = source1buffer[5][i];
		keyvalue_t source2keyvalue5 = source2buffer[5][i];
		keyvalue_t source3keyvalue5 = source3buffer[5][i];
		keyvalue_t source4keyvalue5 = source4buffer[5][i];
		keyvalue_t destkeyvalue5;
		keyvalue_t source1keyvalue6 = source1buffer[6][i];
		keyvalue_t source2keyvalue6 = source2buffer[6][i];
		keyvalue_t source3keyvalue6 = source3buffer[6][i];
		keyvalue_t source4keyvalue6 = source4buffer[6][i];
		keyvalue_t destkeyvalue6;
		keyvalue_t source1keyvalue7 = source1buffer[7][i];
		keyvalue_t source2keyvalue7 = source2buffer[7][i];
		keyvalue_t source3keyvalue7 = source3buffer[7][i];
		keyvalue_t source4keyvalue7 = source4buffer[7][i];
		keyvalue_t destkeyvalue7;
		
		destkeyvalue0 = mergefunc(source1keyvalue0, source2keyvalue0, source3keyvalue0, source4keyvalue0);
		destkeyvalue1 = mergefunc(source1keyvalue1, source2keyvalue1, source3keyvalue1, source4keyvalue1);
		destkeyvalue2 = mergefunc(source1keyvalue2, source2keyvalue2, source3keyvalue2, source4keyvalue2);
		destkeyvalue3 = mergefunc(source1keyvalue3, source2keyvalue3, source3keyvalue3, source4keyvalue3);
		destkeyvalue4 = mergefunc(source1keyvalue4, source2keyvalue4, source3keyvalue4, source4keyvalue4);
		destkeyvalue5 = mergefunc(source1keyvalue5, source2keyvalue5, source3keyvalue5, source4keyvalue5);
		destkeyvalue6 = mergefunc(source1keyvalue6, source2keyvalue6, source3keyvalue6, source4keyvalue6);
		destkeyvalue7 = mergefunc(source1keyvalue7, source2keyvalue7, source3keyvalue7, source4keyvalue7);
		
		destbuffer[0][i] = destkeyvalue0;
		destbuffer[1][i] = destkeyvalue1;
		destbuffer[2][i] = destkeyvalue2;
		destbuffer[3][i] = destkeyvalue3;
		destbuffer[4][i] = destkeyvalue4;
		destbuffer[5][i] = destkeyvalue5;
		destbuffer[6][i] = destkeyvalue6;
		destbuffer[7][i] = destkeyvalue7;
		
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsmerged(VECTOR_SIZE * 4);
		#endif
	}
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif
merge_I2(unsigned int enable, keyvalue_t source1buffer[8][SRCBUFFER_SIZE], keyvalue_t source2buffer[8][SRCBUFFER_SIZE], keyvalue_t source3buffer[8][SRCBUFFER_SIZE], keyvalue_t source4buffer[8][SRCBUFFER_SIZE], keyvalue_t destbuffer[8][PADDEDDESTBUFFER_SIZE]){
	if(enable == OFF){ return; }
	
	MERGE_LOOP1B: for(buffer_type i=0; i<SRCBUFFER_SIZE; i++){
	#pragma HLS PIPELINE II=2
		keyvalue_t source1keyvalue0 = source1buffer[0][i];
		keyvalue_t source2keyvalue0 = source2buffer[0][i];
		keyvalue_t source3keyvalue0 = source3buffer[0][i];
		keyvalue_t source4keyvalue0 = source4buffer[0][i];
		keyvalue_t destkeyvalue0;
		keyvalue_t source1keyvalue1 = source1buffer[1][i];
		keyvalue_t source2keyvalue1 = source2buffer[1][i];
		keyvalue_t source3keyvalue1 = source3buffer[1][i];
		keyvalue_t source4keyvalue1 = source4buffer[1][i];
		keyvalue_t destkeyvalue1;
		keyvalue_t source1keyvalue2 = source1buffer[2][i];
		keyvalue_t source2keyvalue2 = source2buffer[2][i];
		keyvalue_t source3keyvalue2 = source3buffer[2][i];
		keyvalue_t source4keyvalue2 = source4buffer[2][i];
		keyvalue_t destkeyvalue2;
		keyvalue_t source1keyvalue3 = source1buffer[3][i];
		keyvalue_t source2keyvalue3 = source2buffer[3][i];
		keyvalue_t source3keyvalue3 = source3buffer[3][i];
		keyvalue_t source4keyvalue3 = source4buffer[3][i];
		keyvalue_t destkeyvalue3;
		keyvalue_t source1keyvalue4 = source1buffer[4][i];
		keyvalue_t source2keyvalue4 = source2buffer[4][i];
		keyvalue_t source3keyvalue4 = source3buffer[4][i];
		keyvalue_t source4keyvalue4 = source4buffer[4][i];
		keyvalue_t destkeyvalue4;
		keyvalue_t source1keyvalue5 = source1buffer[5][i];
		keyvalue_t source2keyvalue5 = source2buffer[5][i];
		keyvalue_t source3keyvalue5 = source3buffer[5][i];
		keyvalue_t source4keyvalue5 = source4buffer[5][i];
		keyvalue_t destkeyvalue5;
		keyvalue_t source1keyvalue6 = source1buffer[6][i];
		keyvalue_t source2keyvalue6 = source2buffer[6][i];
		keyvalue_t source3keyvalue6 = source3buffer[6][i];
		keyvalue_t source4keyvalue6 = source4buffer[6][i];
		keyvalue_t destkeyvalue6;
		keyvalue_t source1keyvalue7 = source1buffer[7][i];
		keyvalue_t source2keyvalue7 = source2buffer[7][i];
		keyvalue_t source3keyvalue7 = source3buffer[7][i];
		keyvalue_t source4keyvalue7 = source4buffer[7][i];
		keyvalue_t destkeyvalue7;
		
		destkeyvalue0 = mergefunc(source1keyvalue0, source2keyvalue0, source3keyvalue0, source4keyvalue0);
		destkeyvalue1 = mergefunc(source1keyvalue1, source2keyvalue1, source3keyvalue1, source4keyvalue1);
		destkeyvalue2 = mergefunc(source1keyvalue2, source2keyvalue2, source3keyvalue2, source4keyvalue2);
		destkeyvalue3 = mergefunc(source1keyvalue3, source2keyvalue3, source3keyvalue3, source4keyvalue3);
		destkeyvalue4 = mergefunc(source1keyvalue4, source2keyvalue4, source3keyvalue4, source4keyvalue4);
		destkeyvalue5 = mergefunc(source1keyvalue5, source2keyvalue5, source3keyvalue5, source4keyvalue5);
		destkeyvalue6 = mergefunc(source1keyvalue6, source2keyvalue6, source3keyvalue6, source4keyvalue6);
		destkeyvalue7 = mergefunc(source1keyvalue7, source2keyvalue7, source3keyvalue7, source4keyvalue7);
		
		destbuffer[0][i] = destkeyvalue0;
		destbuffer[1][i] = destkeyvalue1;
		destbuffer[2][i] = destkeyvalue2;
		destbuffer[3][i] = destkeyvalue3;
		destbuffer[4][i] = destkeyvalue4;
		destbuffer[5][i] = destkeyvalue5;
		destbuffer[6][i] = destkeyvalue6;
		destbuffer[7][i] = destkeyvalue7;
		
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsmerged(VECTOR_SIZE * 4);
		#endif
	}
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif
merge_I3(unsigned int enable, keyvalue_t source1buffer[8][SRCBUFFER_SIZE], keyvalue_t source2buffer[8][SRCBUFFER_SIZE], keyvalue_t source3buffer[8][SRCBUFFER_SIZE], keyvalue_t source4buffer[8][SRCBUFFER_SIZE], keyvalue_t destbuffer[8][PADDEDDESTBUFFER_SIZE]){
	if(enable == OFF){ return; }
	
	MERGE_LOOP1B: for(buffer_type i=0; i<SRCBUFFER_SIZE; i++){
	#pragma HLS PIPELINE II=2
		keyvalue_t source1keyvalue0 = source1buffer[0][i];
		keyvalue_t source2keyvalue0 = source2buffer[0][i];
		keyvalue_t source3keyvalue0 = source3buffer[0][i];
		keyvalue_t source4keyvalue0 = source4buffer[0][i];
		keyvalue_t destkeyvalue0;
		keyvalue_t source1keyvalue1 = source1buffer[1][i];
		keyvalue_t source2keyvalue1 = source2buffer[1][i];
		keyvalue_t source3keyvalue1 = source3buffer[1][i];
		keyvalue_t source4keyvalue1 = source4buffer[1][i];
		keyvalue_t destkeyvalue1;
		keyvalue_t source1keyvalue2 = source1buffer[2][i];
		keyvalue_t source2keyvalue2 = source2buffer[2][i];
		keyvalue_t source3keyvalue2 = source3buffer[2][i];
		keyvalue_t source4keyvalue2 = source4buffer[2][i];
		keyvalue_t destkeyvalue2;
		keyvalue_t source1keyvalue3 = source1buffer[3][i];
		keyvalue_t source2keyvalue3 = source2buffer[3][i];
		keyvalue_t source3keyvalue3 = source3buffer[3][i];
		keyvalue_t source4keyvalue3 = source4buffer[3][i];
		keyvalue_t destkeyvalue3;
		keyvalue_t source1keyvalue4 = source1buffer[4][i];
		keyvalue_t source2keyvalue4 = source2buffer[4][i];
		keyvalue_t source3keyvalue4 = source3buffer[4][i];
		keyvalue_t source4keyvalue4 = source4buffer[4][i];
		keyvalue_t destkeyvalue4;
		keyvalue_t source1keyvalue5 = source1buffer[5][i];
		keyvalue_t source2keyvalue5 = source2buffer[5][i];
		keyvalue_t source3keyvalue5 = source3buffer[5][i];
		keyvalue_t source4keyvalue5 = source4buffer[5][i];
		keyvalue_t destkeyvalue5;
		keyvalue_t source1keyvalue6 = source1buffer[6][i];
		keyvalue_t source2keyvalue6 = source2buffer[6][i];
		keyvalue_t source3keyvalue6 = source3buffer[6][i];
		keyvalue_t source4keyvalue6 = source4buffer[6][i];
		keyvalue_t destkeyvalue6;
		keyvalue_t source1keyvalue7 = source1buffer[7][i];
		keyvalue_t source2keyvalue7 = source2buffer[7][i];
		keyvalue_t source3keyvalue7 = source3buffer[7][i];
		keyvalue_t source4keyvalue7 = source4buffer[7][i];
		keyvalue_t destkeyvalue7;
		
		destkeyvalue0 = mergefunc(source1keyvalue0, source2keyvalue0, source3keyvalue0, source4keyvalue0);
		destkeyvalue1 = mergefunc(source1keyvalue1, source2keyvalue1, source3keyvalue1, source4keyvalue1);
		destkeyvalue2 = mergefunc(source1keyvalue2, source2keyvalue2, source3keyvalue2, source4keyvalue2);
		destkeyvalue3 = mergefunc(source1keyvalue3, source2keyvalue3, source3keyvalue3, source4keyvalue3);
		destkeyvalue4 = mergefunc(source1keyvalue4, source2keyvalue4, source3keyvalue4, source4keyvalue4);
		destkeyvalue5 = mergefunc(source1keyvalue5, source2keyvalue5, source3keyvalue5, source4keyvalue5);
		destkeyvalue6 = mergefunc(source1keyvalue6, source2keyvalue6, source3keyvalue6, source4keyvalue6);
		destkeyvalue7 = mergefunc(source1keyvalue7, source2keyvalue7, source3keyvalue7, source4keyvalue7);
		
		destbuffer[0][i] = destkeyvalue0;
		destbuffer[1][i] = destkeyvalue1;
		destbuffer[2][i] = destkeyvalue2;
		destbuffer[3][i] = destkeyvalue3;
		destbuffer[4][i] = destkeyvalue4;
		destbuffer[5][i] = destkeyvalue5;
		destbuffer[6][i] = destkeyvalue6;
		destbuffer[7][i] = destkeyvalue7;
		
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsmerged(VECTOR_SIZE * 4);
		#endif
	}
	return;
}

void 
	#ifdef SW 
	actslw::
	#endif
merge_I0(unsigned int enable, keyvalue_t source1buffer[8][PADDEDDESTBUFFER_SIZE], keyvalue_t source2buffer[8][PADDEDDESTBUFFER_SIZE], keyvalue_t source3buffer[8][PADDEDDESTBUFFER_SIZE], keyvalue_t source4buffer[8][PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer[PADDEDDESTBUFFER_SIZE]){
	if(enable == OFF){ return; }
	
	MERGE_LOOP1B: for(buffer_type i=0; i<SRCBUFFER_SIZE; i++){
	#pragma HLS PIPELINE II=2
		keyvalue_t source1keyvalue0 = source1buffer[0][i];
		keyvalue_t source2keyvalue0 = source2buffer[0][i];
		keyvalue_t source3keyvalue0 = source3buffer[0][i];
		keyvalue_t source4keyvalue0 = source4buffer[0][i];
		keyvalue_t destkeyvalue0;
		keyvalue_t source1keyvalue1 = source1buffer[1][i];
		keyvalue_t source2keyvalue1 = source2buffer[1][i];
		keyvalue_t source3keyvalue1 = source3buffer[1][i];
		keyvalue_t source4keyvalue1 = source4buffer[1][i];
		keyvalue_t destkeyvalue1;
		keyvalue_t source1keyvalue2 = source1buffer[2][i];
		keyvalue_t source2keyvalue2 = source2buffer[2][i];
		keyvalue_t source3keyvalue2 = source3buffer[2][i];
		keyvalue_t source4keyvalue2 = source4buffer[2][i];
		keyvalue_t destkeyvalue2;
		keyvalue_t source1keyvalue3 = source1buffer[3][i];
		keyvalue_t source2keyvalue3 = source2buffer[3][i];
		keyvalue_t source3keyvalue3 = source3buffer[3][i];
		keyvalue_t source4keyvalue3 = source4buffer[3][i];
		keyvalue_t destkeyvalue3;
		keyvalue_t source1keyvalue4 = source1buffer[4][i];
		keyvalue_t source2keyvalue4 = source2buffer[4][i];
		keyvalue_t source3keyvalue4 = source3buffer[4][i];
		keyvalue_t source4keyvalue4 = source4buffer[4][i];
		keyvalue_t destkeyvalue4;
		keyvalue_t source1keyvalue5 = source1buffer[5][i];
		keyvalue_t source2keyvalue5 = source2buffer[5][i];
		keyvalue_t source3keyvalue5 = source3buffer[5][i];
		keyvalue_t source4keyvalue5 = source4buffer[5][i];
		keyvalue_t destkeyvalue5;
		keyvalue_t source1keyvalue6 = source1buffer[6][i];
		keyvalue_t source2keyvalue6 = source2buffer[6][i];
		keyvalue_t source3keyvalue6 = source3buffer[6][i];
		keyvalue_t source4keyvalue6 = source4buffer[6][i];
		keyvalue_t destkeyvalue6;
		keyvalue_t source1keyvalue7 = source1buffer[7][i];
		keyvalue_t source2keyvalue7 = source2buffer[7][i];
		keyvalue_t source3keyvalue7 = source3buffer[7][i];
		keyvalue_t source4keyvalue7 = source4buffer[7][i];
		keyvalue_t destkeyvalue7;
		
		destkeyvalue0 = mergefunc(source1keyvalue0, source2keyvalue0, source3keyvalue0, source4keyvalue0);
		destkeyvalue1 = mergefunc(source1keyvalue1, source2keyvalue1, source3keyvalue1, source4keyvalue1);
		destkeyvalue2 = mergefunc(source1keyvalue2, source2keyvalue2, source3keyvalue2, source4keyvalue2);
		destkeyvalue3 = mergefunc(source1keyvalue3, source2keyvalue3, source3keyvalue3, source4keyvalue3);
		destkeyvalue4 = mergefunc(source1keyvalue4, source2keyvalue4, source3keyvalue4, source4keyvalue4);
		destkeyvalue5 = mergefunc(source1keyvalue5, source2keyvalue5, source3keyvalue5, source4keyvalue5);
		destkeyvalue6 = mergefunc(source1keyvalue6, source2keyvalue6, source3keyvalue6, source4keyvalue6);
		destkeyvalue7 = mergefunc(source1keyvalue7, source2keyvalue7, source3keyvalue7, source4keyvalue7);
		
		destbuffer[i].data[0] = destkeyvalue0;
		destbuffer[i].data[1] = destkeyvalue1;
		destbuffer[i].data[2] = destkeyvalue2;
		destbuffer[i].data[3] = destkeyvalue3;
		destbuffer[i].data[4] = destkeyvalue4;
		destbuffer[i].data[5] = destkeyvalue5;
		destbuffer[i].data[6] = destkeyvalue6;
		destbuffer[i].data[7] = destkeyvalue7;
		
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsmerged(VECTOR_SIZE * 4);
		#endif
	}
	return;
}

// group functions
void 
	#ifdef SW 
	actslw::
	#endif
combineSetof1stoSetof2s0(bool_type enable, keyvalue_t buffer_setof1[8][PADDEDDESTBUFFER_SIZE], uint128_dt buffer_setof2[4][PADDEDDESTBUFFER_SIZE], skeyvalue_t templocalcapsule_so1[8][NUM_PARTITIONS], skeyvalue_t templocalcapsule_so2[4][NUM_PARTITIONS]){
	#pragma HLS INLINE
	// 1s->2s
	combineSetof1stoSetof20_I0(enable, buffer_setof1[0], buffer_setof1[1], buffer_setof2[0], templocalcapsule_so1[0], templocalcapsule_so1[1], templocalcapsule_so2[0]);
	combineSetof1stoSetof20_I1(enable, buffer_setof1[2], buffer_setof1[3], buffer_setof2[1], templocalcapsule_so1[2], templocalcapsule_so1[3], templocalcapsule_so2[1]);
	combineSetof1stoSetof20_I2(enable, buffer_setof1[4], buffer_setof1[5], buffer_setof2[2], templocalcapsule_so1[4], templocalcapsule_so1[5], templocalcapsule_so2[2]);
	combineSetof1stoSetof20_I3(enable, buffer_setof1[6], buffer_setof1[7], buffer_setof2[3], templocalcapsule_so1[6], templocalcapsule_so1[7], templocalcapsule_so2[3]);
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif
combineSetof2stoSetof4s0(bool_type enable, uint128_dt buffer_setof2[4][PADDEDDESTBUFFER_SIZE], uint256_dt buffer_setof4[2][PADDEDDESTBUFFER_SIZE], skeyvalue_t templocalcapsule_so2[4][NUM_PARTITIONS], skeyvalue_t templocalcapsule_so4[2][NUM_PARTITIONS]){
	#pragma HLS INLINE
	// 2s->4s
	combineSetof2stoSetof40_I0(enable, buffer_setof2[0], buffer_setof2[1], buffer_setof4[0], templocalcapsule_so2[0], templocalcapsule_so2[1], templocalcapsule_so4[0]);
	combineSetof2stoSetof40_I1(enable, buffer_setof2[2], buffer_setof2[3], buffer_setof4[1], templocalcapsule_so2[2], templocalcapsule_so2[3], templocalcapsule_so4[1]);
	return;
}
void 
	#ifdef SW 
	actslw::
	#endif
combineSetof4stoSetof8s0(bool_type enable, uint256_dt buffer_setof4[2][PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer[PADDEDDESTBUFFER_SIZE], skeyvalue_t templocalcapsule_so4[2][NUM_PARTITIONS], skeyvalue_t templocalcapsule_so8[NUM_PARTITIONS]){
	#pragma HLS INLINE
	// 4s->8s
	combineSetof4stoSetof80_I0(enable, buffer_setof4[0], buffer_setof4[1], destbuffer, templocalcapsule_so4[0], templocalcapsule_so4[1], templocalcapsule_so8);
	return;
}

// main function - dispatch
void 
	#ifdef SW 
	actslw::
	#endif 
dispatch0(uint512_dt * kvdram, keyvalue_t sourcebuffer[VECTOR_SIZE][SRCBUFFER_SIZE], keyvalue_t buffer_setof1[8][PADDEDDESTBUFFER_SIZE], uint128_dt buffer_setof2[4][PADDEDDESTBUFFER_SIZE], uint256_dt buffer_setof4[2][PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer[PADDEDDESTBUFFER_SIZE]){
	analysis_type analysis_mainloop = KVDATA_BATCHSIZE_KVS / (NUMPIPELINES * SRCBUFFER_SIZE);
	analysis_type analysis_numllops = 1;
	analysis_type analysis_numsourcepartitions = 1;
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->printparameters();
	actsutilityobj->printglobalvars();
	actsutilityobj->clearglobalvars();
	#endif
	
	// keyvalue_t sourcebuffer[VECTOR_SIZE][SRCBUFFER_SIZE];
	// #pragma HLS array_partition variable = sourcebuffer
	
	// keyvalue_t buffer_setof1[8][PADDEDDESTBUFFER_SIZE];
	// #pragma HLS array_partition variable = buffer_setof1
	skeyvalue_t templocalcapsule_so1[8][NUM_PARTITIONS];
	#pragma HLS array_partition variable = templocalcapsule_so1
	
	// uint128_dt buffer_setof2[4][PADDEDDESTBUFFER_SIZE];
	// #pragma HLS array_partition variable = buffer_setof2
	skeyvalue_t templocalcapsule_so2[4][NUM_PARTITIONS];
	#pragma HLS array_partition variable = templocalcapsule_so2
	
	// uint256_dt buffer_setof4[2][PADDEDDESTBUFFER_SIZE];
	// #pragma HLS array_partition variable = buffer_setof4
	skeyvalue_t templocalcapsule_so4[2][NUM_PARTITIONS];
	#pragma HLS array_partition variable = templocalcapsule_so4
	
	// uint512_dt destbuffer[PADDEDDESTBUFFER_SIZE]; // 
	skeyvalue_t templocalcapsule_so8[NUM_PARTITIONS];

	keyvalue_t globaldestoffsets[NUM_PARTITIONS];
	keyvalue_t globalstatsbuffer[PADDEDDESTBUFFER_SIZE];
	
	globalparams_t globalparams = getglobalparams(kvdram);
	sweepparams_t sweepparams;
	resetmanykeyandvalues(buffer_setof1, PADDEDDESTBUFFER_SIZE);

	// start launch
	MAIN_LOOP1: for(step_type currentLOP=0; currentLOP<=globalparams.treedepth; currentLOP++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_numllops avg=analysis_numllops	
	
		config_t config = getconfig(currentLOP);
		batch_type num_source_partitions = get_num_source_partitions(currentLOP);
		
		MAIN_LOOP1B: for(batch_type source_partition=0; source_partition<num_source_partitions; source_partition+=1){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_numsourcepartitions avg=analysis_numsourcepartitions	
		
			sweepparams = getsweepparams(globalparams, currentLOP, source_partition);
			travstate_t travstate = gettravstate(kvdram, globalstatsbuffer, globalparams, config, currentLOP, source_partition, num_source_partitions);
			travstate_t travstatepp1 = travstate; 
			travstate_t travstatepp2 = travstate;
			#ifdef _DEBUGMODE_KERNELPRINTS2
			actsutilityobj->printpartitionstep(config, sweepparams, travstate, 0);
			actsutilityobj->setstructs(config, sweepparams, travstate);
			#endif
			
			// launch partition
			batch_type destskipsize = getskipsize(sweepparams.currentLOP, DEST);
			readglobalstats0(config.enablepartition, kvdram, globalstatsbuffer, globaldestoffsets, (sweepparams.source_partition * destskipsize * NUM_PARTITIONS), sweepparams.currentLOP, DEST);
			readvertices0(config.enablereduce, kvdram, buffer_setof1, (BASEOFFSET_VERTICESDATA_KVS + sweepparams.upperlimit));
			
			MAIN_LOOP1C: for(batch_type offset_kvs=travstate.begin_kvs; offset_kvs<travstate.end_kvs; offset_kvs+=travstate.skip_kvs){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_mainloop avg=analysis_mainloop	
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print4("### dispatch0:: offset_kvs", "begin_kvs", "end_kvs", "skip", offset_kvs, travstate.begin_kvs, travstate.end_kvs, SRCBUFFER_SIZE);
				#endif
				
				travstate.i_kvs = offset_kvs;
				travstatepp1.i_kvs = offset_kvs + SRCBUFFER_SIZE;
				travstatepp2.i_kvs = offset_kvs + (2 * SRCBUFFER_SIZE);
				
				readkeyvalues0(ON, kvdram, sourcebuffer, (sweepparams.worksourcebaseaddress_kvs + offset_kvs), travstate);
				#ifdef PP1 // pipeline overflow from bottom
				savekeyvalues0(config.enablepartition, kvdram, destbuffer, globaldestoffsets, templocalcapsule_so8, sweepparams.workdestbaseaddress_kvs); 
				#endif 
				#ifdef PP2 // pipeline overflow from bottom
				combineSetof4stoSetof8s0(config.enablepartition, buffer_setof4, destbuffer, templocalcapsule_so4, templocalcapsule_so8); 
				#endif
			
				partitionkeyvalues0(config.enablepartition, sourcebuffer, buffer_setof1, templocalcapsule_so1, sweepparams.currentLOP, sweepparams.upperlimit, travstate);
				#ifdef PP1
				readkeyvalues0(config.enablepartition, kvdram, sourcebuffer, (sweepparams.worksourcebaseaddress_kvs + offset_kvs), travstatepp1);
				#endif 
				#ifdef PP2 // pipeline overflow from bottom
				savekeyvalues0(config.enablepartition, kvdram, destbuffer, globaldestoffsets, templocalcapsule_so8, sweepparams.workdestbaseaddress_kvs);
				#endif 

				// 1s->2s
				combineSetof1stoSetof2s0(config.enablepartition, buffer_setof1, buffer_setof2, templocalcapsule_so1, templocalcapsule_so2);
				#ifdef PP1
				partitionkeyvalues0(config.enablepartition, sourcebuffer, buffer_setof1, templocalcapsule_so1, sweepparams.currentLOP, sweepparams.upperlimit, travstatepp1);
				#endif 
				#ifdef PP2
				readkeyvalues0(config.enablepartition, kvdram, sourcebuffer, (sweepparams.worksourcebaseaddress_kvs + offset_kvs), travstatepp2);
				#endif 

				// 2s->4s
				combineSetof2stoSetof4s0(config.enablepartition, buffer_setof2, buffer_setof4, templocalcapsule_so2, templocalcapsule_so4);
				#ifdef PP1
				combineSetof1stoSetof2s0(config.enablepartition, buffer_setof1, buffer_setof2, templocalcapsule_so1, templocalcapsule_so2);
				#endif 
				#ifdef PP2
				partitionkeyvalues0(config.enablepartition, sourcebuffer, buffer_setof1, templocalcapsule_so1, sweepparams.currentLOP, sweepparams.upperlimit, travstatepp2); 
				#endif 
				
				// 4s->8s
				combineSetof4stoSetof8s0(config.enablepartition, buffer_setof4, destbuffer, templocalcapsule_so4, templocalcapsule_so8);
				#ifdef PP1
				combineSetof2stoSetof4s0(config.enablepartition, buffer_setof2, buffer_setof4, templocalcapsule_so2, templocalcapsule_so4);
				#endif 
				#ifdef PP2
				combineSetof1stoSetof2s0(config.enablepartition, buffer_setof1, buffer_setof2, templocalcapsule_so1, templocalcapsule_so2);
				#endif 
				
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->printkeyvalues("(fin) dispatch0:: templocalcapsule_so8::", templocalcapsule_so8, NUM_PARTITIONS);
				actsutilityobj->printvaluecount("(fin) dispatch0:: templocalcapsule_so8", templocalcapsule_so8, NUM_PARTITIONS);
				#endif 
			
				savekeyvalues0(config.enablepartition, kvdram, destbuffer, globaldestoffsets, templocalcapsule_so8, sweepparams.workdestbaseaddress_kvs);
				#ifdef PP1
				combineSetof4stoSetof8s0(config.enablepartition, buffer_setof4, destbuffer, templocalcapsule_so4, templocalcapsule_so8);
				#endif
				#ifdef PP2
				combineSetof2stoSetof4s0(config.enablepartition, buffer_setof2, buffer_setof4, templocalcapsule_so2, templocalcapsule_so4);
				#endif
				///// overflow. pipeline continuing from top...
				
				collectglobalstats0(config.enablecollectglobalstats, sourcebuffer, buffer_setof1, sweepparams.upperlimit);
				
				reduce0(config.enablereduce, sourcebuffer, buffer_setof1, sweepparams.upperlimit, globalparams.GraphIter, globalparams.GraphAlgo, travstate);
			}
			
			prepareglobalstats0(config.enablecollectglobalstats, buffer_setof1);
			saveglobalstats0(config.enablecollectglobalstats, kvdram, globalstatsbuffer, buffer_setof1);
			updateglobalstats0(config.enablepartition, kvdram, globalstatsbuffer, globaldestoffsets, (sweepparams.source_partition * destskipsize * NUM_PARTITIONS), sweepparams.currentLOP, DEST);
			savevertices0(config.enablereduce, kvdram, buffer_setof1, (BASEOFFSET_VERTICESDATA_KVS + sweepparams.upperlimit));
			
			#ifdef _DEBUGMODE_KERNELPRINTS2
			actsutilityobj->printpartitionresult(OFF, kvdram, globaldestoffsets, globalstatsbuffer, sweepparams);
			#endif
		}
		#ifdef _DEBUGMODE_KERNELPRINTS2
		actsutilityobj->printglobalvars();
		actsutilityobj->clearglobalvars();
		#endif
	}
	return;
}

// top
extern "C" {
void 
	#ifdef SW 
	actslw:: 
	#endif
topkernel( uint512_dt * sourceAvolume ){
#pragma HLS INTERFACE m_axi port = sourceAvolume offset = slave bundle = gmem0 // max_read_burst_length=64 max_write_burst_length=64

#pragma HLS INTERFACE s_axilite port = sourceAvolume bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATA_PACK variable = sourceAvolume

	#ifdef _DEBUGMODE_KERNELPRINTS2
	cout<<"Light weight ACTS started."<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"... Acts Launched... size: "<<sourceAvolume[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key<<endl; 
	#endif 
	
	keyvalue_t source0buffer[VECTOR_SIZE][SRCBUFFER_SIZE]; // SRCBUFFER_SIZE
	#pragma HLS array_partition variable = source0buffer
	
	keyvalue_t buffer0_setof1[8][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer0_setof1
	
	uint128_dt buffer0_setof2[4][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer0_setof2
	
	uint256_dt buffer0_setof4[2][PADDEDDESTBUFFER_SIZE];
	#pragma HLS array_partition variable = buffer0_setof4
	
	uint512_dt dest0buffer[PADDEDDESTBUFFER_SIZE];
	
	// dispatch
	#ifdef ENABLEPARTITIONANDREDUCE
	dispatch0(sourceAvolume, source0buffer, buffer0_setof1, buffer0_setof2, buffer0_setof4, dest0buffer);
	
	// exit(EXIT_SUCCESS);
	
	#endif 
	
	// merge
	#ifdef ENABLEMERGER
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->printparameters();
	actsutilityobj->printglobalvars();
	actsutilityobj->clearglobalvars();
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->print4("### merge begin_kvs", "end_kvs", "skip", "NAp", 0, BATCH_RANGE_KVS, SRCBUFFER_SIZE, NAp);	
	#endif
	for(unsigned int offset_kvs=0; offset_kvs<BATCH_RANGE_KVS; offset_kvs+=PADDEDDESTBUFFER_SIZE){
		#ifdef _DEBUGMODE_KERNELPRINTS2
		actsutilityobj->print4("### merge:: offset_kvs", "begin_kvs", "end_kvs", "skip", offset_kvs, 0, BATCH_RANGE_KVS, SRCBUFFER_SIZE);
		#endif
		readmany(BASEOFFSET_VERTICESDATA_KVS + offset_kvs  ,sourceAvolume ,source0buffer  );

		merge_I0(ON, source0buffer, source1buffer, source2buffer, source3buffer, buffer0_setof1);
		merge_I1(ON, source4buffer, source5buffer, source6buffer, source7buffer, buffer4_setof1);
		merge_I2(ON, source8buffer, source9buffer, source10buffer, source11buffer, buffer8_setof1);
		merge_I3(ON, source12buffer, source13buffer, source14buffer, source15buffer, buffer12_setof1);
		
		merge_I0(ON, buffer0_setof1, buffer4_setof1, buffer8_setof1, buffer12_setof1, dest0buffer);

		saveone(BASEOFFSET_VERTICESDATA_KVS + offset_kvs, sourceAvolume, dest0buffer);
	}
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->printglobalvars();
	actsutilityobj->clearglobalvars();
	#endif
	#endif 
	return;
}
}







