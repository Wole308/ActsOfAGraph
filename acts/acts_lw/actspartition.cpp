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
#include "../actsutility/actsutility.h"
#endif 
#include "actspartition.h"
using namespace std;

#ifdef SW
actspartition::actspartition(){ actsutilityobj = new actsutility(); }
actspartition::~actspartition(){}
#endif 

unsigned int 
	#ifdef SW 
	actspartition::
	#endif 
allignhigher_KV(unsigned int val){
	unsigned int fac = (val + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	return (fac * VECTOR_SIZE);
}
void 
	#ifdef SW 
	actspartition::
	#endif 
getglobalparams(uint512_dt * sourcevolume, alw_globalparams_t * globalparams){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	globalparams->runkernelcommand = sourcevolume[BASEOFFSET2_MESSAGESDRAM_KVS + MESSAGES_RUNKERNELCOMMANDID].range(31, 0);
	globalparams->processcommand = sourcevolume[BASEOFFSET2_MESSAGESDRAM_KVS + MESSAGES_PROCESSCOMMANDID].range(31, 0);
	globalparams->collectstatscommand = sourcevolume[BASEOFFSET2_MESSAGESDRAM_KVS + MESSAGES_COLLECTSTATSCOMMANDID].range(31, 0);
	globalparams->partitioncommand = sourcevolume[BASEOFFSET2_MESSAGESDRAM_KVS + MESSAGES_PARTITIONCOMMANDID].range(31, 0);
	globalparams->reducecommand = sourcevolume[BASEOFFSET2_MESSAGESDRAM_KVS + MESSAGES_APPLYUPDATESCOMMANDID].range(31, 0);
	globalparams->vbegin = sourcevolume[BASEOFFSET2_MESSAGESDRAM_KVS + MESSAGES_VOFFSET].range(31, 0); 
	globalparams->vsize = sourcevolume[BASEOFFSET2_MESSAGESDRAM_KVS + MESSAGES_VSIZE].range(31, 0); 
	globalparams->treedepth = sourcevolume[BASEOFFSET2_MESSAGESDRAM_KVS + MESSAGES_TREEDEPTH].range(31, 0); 
	globalparams->LLOPnumpartitions = sourcevolume[BASEOFFSET2_MESSAGESDRAM_KVS + MESSAGES_FINALNUMPARTITIONS].range(31, 0);
	globalparams->GraphIter = sourcevolume[BASEOFFSET2_MESSAGESDRAM_KVS + MESSAGES_GRAPHITERATIONID].range(31, 0);
	globalparams->batchsize = sourcevolume[BASEOFFSET2_MESSAGESDRAM_KVS + MESSAGES_BATCHSIZE].range(31, 0);
	globalparams->runsize = sourcevolume[BASEOFFSET2_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].range(31, 0);
	globalparams->nextbatchoffset = sourcevolume[BASEOFFSET2_MESSAGESDRAM_KVS + MESSAGES_NEXTBATCHOFFSET].range(31, 0);
	globalparams->statsalreadycollected = sourcevolume[BASEOFFSET2_MESSAGESDRAM_KVS + MESSAGES_STATSALREADYCOLLECTED].range(31, 0); // false;
	globalparams->baseaddr_destkvs_kvs = 0;
	#else 
	globalparams->runkernelcommand = sourcevolume[BASEOFFSET2_MESSAGESDRAM_KVS + MESSAGES_RUNKERNELCOMMANDID].data[0].key;
	globalparams->processcommand = sourcevolume[BASEOFFSET2_MESSAGESDRAM_KVS + MESSAGES_PROCESSCOMMANDID].data[0].key;
	globalparams->collectstatscommand = sourcevolume[BASEOFFSET2_MESSAGESDRAM_KVS + MESSAGES_COLLECTSTATSCOMMANDID].data[0].key;
	globalparams->partitioncommand = sourcevolume[BASEOFFSET2_MESSAGESDRAM_KVS + MESSAGES_PARTITIONCOMMANDID].data[0].key;
	globalparams->reducecommand = sourcevolume[BASEOFFSET2_MESSAGESDRAM_KVS + MESSAGES_APPLYUPDATESCOMMANDID].data[0].key;
	globalparams->vbegin = sourcevolume[BASEOFFSET2_MESSAGESDRAM_KVS + MESSAGES_VOFFSET].data[0].key; 
	globalparams->vsize = sourcevolume[BASEOFFSET2_MESSAGESDRAM_KVS + MESSAGES_VSIZE].data[0].key; 
	globalparams->treedepth = sourcevolume[BASEOFFSET2_MESSAGESDRAM_KVS + MESSAGES_TREEDEPTH].data[0].key; 
	globalparams->LLOPnumpartitions = sourcevolume[BASEOFFSET2_MESSAGESDRAM_KVS + MESSAGES_FINALNUMPARTITIONS].data[0].key;
	globalparams->GraphIter = sourcevolume[BASEOFFSET2_MESSAGESDRAM_KVS + MESSAGES_GRAPHITERATIONID].data[0].key;
	globalparams->batchsize = sourcevolume[BASEOFFSET2_MESSAGESDRAM_KVS + MESSAGES_BATCHSIZE].data[0].key;
	globalparams->runsize = sourcevolume[BASEOFFSET2_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key;
	globalparams->nextbatchoffset = sourcevolume[BASEOFFSET2_MESSAGESDRAM_KVS + MESSAGES_NEXTBATCHOFFSET].data[0].key;
	globalparams->statsalreadycollected = sourcevolume[BASEOFFSET2_MESSAGESDRAM_KVS + MESSAGES_STATSALREADYCOLLECTED].data[0].key; // false;
	globalparams->baseaddr_destkvs_kvs = 0;
	#endif 
}
void 
	#ifdef SW 
	actspartition::
	#endif 
copykeyandvalues(keyvalue_t * buffer1, keyvalue_t * buffer2, unsigned int size){
	#pragma HLS INLINE
	for(partition_type i=0; i<size; i++){ buffer1[i] = buffer2[i]; }
}
void 
	#ifdef SW 
	actspartition::
	#endif 
calculatemanyoffsets(keyvalue_t buffer[NUMSUBWORKERS][NUM_PARTITIONS], unsigned int size){
	#pragma HLS INLINE
	for(unsigned int sw=0; sw<NUMSUBWORKERS; sw++){
		for(partition_type i=1; i<size; i++){ 
		#pragma HLS UNROLL
			buffer[sw][i].key = allignhigher_KV(buffer[sw][i-1].key + buffer[sw][i-1].value); 
		}
	}
}
void 
	#ifdef SW 
	actspartition::
	#endif 
resetkeyandvalues(keyvalue_t * buffer, unsigned int size){
	#pragma HLS INLINE
	for(unsigned int i=0; i<size; i++){ buffer[i].key = 0; buffer[i].value = 0; }
}
void 
	#ifdef SW 
	actspartition::
	#endif 
resetkeyandvalues(atp_uint512_dt destbuffer[NUMSUBWORKERS][ALW_PADDEDDESTBUFFER_SIZE]){
	uint512_dt ZeroKVS;
	keyvalue_t ZeroKV; ZeroKV.key = 0; ZeroKV.value = 0;
	#ifdef _WIDEWORD
	ZeroKVS = 0;
	#else 
	for(vector_type v=0; v<VECTOR_SIZE; v++){ ZeroKVS.data[v] = ZeroKV; }
	#endif 
	for(partition_type i=0; i<ALW_PADDEDDESTBUFFER_SIZE; i++){ 
		for(unsigned int sw=0; sw<NUMSUBWORKERS; sw++){
		#pragma HLS UNROLL
			destbuffer[sw][i] = ZeroKVS;
		}
	}
	return;
}
void 
	#ifdef SW 
	actspartition::
	#endif 
resetmanykeyandvalues(keyvalue_t buffer[NUMSUBWORKERS][NUM_PARTITIONS], unsigned int size){
	#pragma HLS INLINE
	for(unsigned int sw=0; sw<NUMSUBWORKERS; sw++){
		for(unsigned int i=0; i<size; i++){ 
		#pragma HLS UNROLL
			buffer[sw][i].key = 0; buffer[sw][i].value = 0; 
		}
	}
}
unsigned int 
	#ifdef SW 
	actspartition::
	#endif 
checkandforce(unsigned int val, unsigned int limit){ if(val >= limit){ return 0; } else { return val; }}
buffer_type 
	#ifdef SW 
	actspartition::
	#endif 
getchunksize(buffer_type buffer_size, atp_travstate_t travstate, unsigned int localoffset){
	#pragma HLS INLINE
	buffer_type chunk_size = buffer_size;
	batch_type i = travstate.i_kvs + localoffset;
	if (i > travstate.end_kvs){ chunk_size = 0; }
	else if ((i + buffer_size) > travstate.end_kvs){ chunk_size = travstate.end_kvs - i; }
	else {}
	return chunk_size;
}
unsigned int 
	#ifdef SW 
	actspartition::
	#endif 
getpartition(keyvalue_t keyvalue, unsigned int currentLOP, vertex_t upperlimit){
	partition_type partition = ((keyvalue.key - upperlimit) >> (BATCH_RANGE_POW - (NUM_PARTITIONS_POW * currentLOP)));
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("actspartition::getpartition", partition, NUM_PARTITIONS, keyvalue.key, upperlimit, currentLOP);
	#endif
	// partition = checkandforce(partition, NUM_PARTITIONS);
	return partition;
}
unsigned int 
	#ifdef SW 
	actspartition::
	#endif 
getglobalpartition(keyvalue_t keyvalue, vertex_t upperlimit){
	partition_type globalpartition = ((keyvalue.key - upperlimit) >> (BATCH_RANGE_POW - (NUM_PARTITIONS_POW * TREE_DEPTH)));
	// cout<<"getglobalpartition:: globalpartition: "<<globalpartition<<", NUMLASTLEVELPARTITIONS: "<<NUMLASTLEVELPARTITIONS<<", keyvalue.key: "<<keyvalue.key<<", upperlimit: "<<upperlimit<<", Size of each LLop"<<(1 << (BATCH_RANGE_POW - (NUM_PARTITIONS_POW * TREE_DEPTH)))<<endl;
	// exit(EXIT_SUCCESS);
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("actspartition::getpartition", globalpartition, NUMLASTLEVELPARTITIONS, keyvalue.key, upperlimit, NAp);
	#endif
	return globalpartition;
}
keyvalue_t 
	#ifdef SW 
	actspartition::
	#endif 
getkeyvalue(uint512_dt * buffer, batch_type offset, batch_type addr){
	#pragma HLS INLINE
	keyvalue_t keyvalue;
	batch_type idy = addr / VECTOR_SIZE;
	vector_type idx = addr % VECTOR_SIZE;
	
	#ifdef _WIDEWORD
if(idx==0){	
		keyvalue.key = buffer[offset + idy].range(31, 0);
		keyvalue.value = buffer[offset + idy].range(63, 32);
	}
else if(idx==1){	
		keyvalue.key = buffer[offset + idy].range(95, 64);
		keyvalue.value = buffer[offset + idy].range(127, 96);
	}
else if(idx==2){	
		keyvalue.key = buffer[offset + idy].range(159, 128);
		keyvalue.value = buffer[offset + idy].range(191, 160);
	}
else if(idx==3){	
		keyvalue.key = buffer[offset + idy].range(223, 192);
		keyvalue.value = buffer[offset + idy].range(255, 224);
	}
else if(idx==4){	
		keyvalue.key = buffer[offset + idy].range(287, 256);
		keyvalue.value = buffer[offset + idy].range(319, 288);
	}
else if(idx==5){	
		keyvalue.key = buffer[offset + idy].range(351, 320);
		keyvalue.value = buffer[offset + idy].range(383, 352);
	}
else if(idx==6){	
		keyvalue.key = buffer[offset + idy].range(415, 384);
		keyvalue.value = buffer[offset + idy].range(447, 416);
	}
else if(idx==7){	
		keyvalue.key = buffer[offset + idy].range(479, 448);
		keyvalue.value = buffer[offset + idy].range(511, 480);
	}
	else {}
	#else
	keyvalue = buffer[offset + idy].data[idx];
	#endif
	return keyvalue;
}
void 
	#ifdef SW 
	actspartition::
	#endif 
setkeyvalue(uint512_dt * buffer, batch_type offset, batch_type addr, keyvalue_t keyvalue){
	#pragma HLS INLINE
	batch_type idy = addr / VECTOR_SIZE;
	vector_type idx = addr % VECTOR_SIZE;
	
	#ifdef _WIDEWORD
if(idx==0){	
		buffer[offset + idy].range(31, 0) = keyvalue.key;
		buffer[offset + idy].range(63, 32) = keyvalue.value;
	}
else if(idx==1){	
		buffer[offset + idy].range(95, 64) = keyvalue.key;
		buffer[offset + idy].range(127, 96) = keyvalue.value;
	}
else if(idx==2){	
		buffer[offset + idy].range(159, 128) = keyvalue.key;
		buffer[offset + idy].range(191, 160) = keyvalue.value;
	}
else if(idx==3){	
		buffer[offset + idy].range(223, 192) = keyvalue.key;
		buffer[offset + idy].range(255, 224) = keyvalue.value;
	}
else if(idx==4){	
		buffer[offset + idy].range(287, 256) = keyvalue.key;
		buffer[offset + idy].range(319, 288) = keyvalue.value;
	}
else if(idx==5){	
		buffer[offset + idy].range(351, 320) = keyvalue.key;
		buffer[offset + idy].range(383, 352) = keyvalue.value;
	}
else if(idx==6){	
		buffer[offset + idy].range(415, 384) = keyvalue.key;
		buffer[offset + idy].range(447, 416) = keyvalue.value;
	}
else if(idx==7){	
		buffer[offset + idy].range(479, 448) = keyvalue.key;
		buffer[offset + idy].range(511, 480) = keyvalue.value;
	}
	else {}
	#else
	buffer[offset + idy].data[idx] = keyvalue;
	#endif
	return;
}

void 
	#ifdef SW 
	actspartition::
	#endif 
read(uint512_dt * kvdram, keyvalue_t * buffer, batch_type offset_kvs, buffer_type size_kvs){
	unsigned int analysis_srcbuffersz = NUM_PARTITIONS;
	READ_LOOP: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS PIPELINE II=1
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
		#ifdef _WIDEWORD
		buffer[i].key = kvdram[offset_kvs + i].range(31, 0);
		buffer[i].value = kvdram[offset_kvs + i].range(63, 32);
		#else 
		buffer[i] = kvdram[offset_kvs + i].data[0];
		#endif 
	}
	return;
}
void 
	#ifdef SW 
	actspartition::
	#endif 
save(uint512_dt * kvdram, keyvalue_t * buffer, batch_type offset_kvs, buffer_type size_kvs){
	unsigned int analysis_srcbuffersz = NUM_PARTITIONS;
	SAVE_LOOP: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS PIPELINE II=1
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
		
		#ifdef _WIDEWORD
		kvdram[offset_kvs + i].range(31, 0) = buffer[i].key;
		kvdram[offset_kvs + i].range(63, 32) = buffer[i].value;
		#else 
		kvdram[offset_kvs + i].data[0] = buffer[i];
		#endif 
	}
	return;
}

void 
	#ifdef SW 
	actspartition::
	#endif 
readKVS0(uint512_dt * kvdram, atp_uint512_dt buffer[NUMSUBWORKERS][ALW_SRCBUFFER_SIZE], batch_type offset_kvs, atp_travstate_t travstate){
	unsigned int analysis_srcbuffersz = ALW_SRCBUFFER_SIZE;
	for(unsigned int sw=0; sw<NUMSUBWORKERS; sw++){
		buffer_type chunk_size = getchunksize(ALW_SRCBUFFER_SIZE, travstate, sw * ALW_SRCBUFFER_SIZE);
		batch_type dramoffset_kvs = offset_kvs + (sw * ALW_SRCBUFFER_SIZE);
		READKVS_LOOP: for (buffer_type i=0; i<chunk_size; i++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
		#pragma HLS PIPELINE II=1
			buffer[sw][i] = kvdram[dramoffset_kvs + i]; 
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
			#endif
		}
	}
	return;
}

void 
	#ifdef SW 
	actspartition::
	#endif 
collectglobalstats0(atp_uint512_dt sourcebuffer[NUMSUBWORKERS][ALW_SRCBUFFER_SIZE], atp_uint512_dt destbuffer[NUMSUBWORKERS][ALW_PADDEDDESTBUFFER_SIZE], sweepparams_t sweepparams, atp_travstate_t travstate){
	unsigned int analysis_srcbuffersz = ALW_SRCBUFFER_SIZE * VECTOR_SIZE;
	
	buffer_type chunk_size[NUMSUBWORKERS];
	for(unsigned int sw=0; sw<NUMSUBWORKERS; sw++){ chunk_size[sw] = getchunksize(ALW_SRCBUFFER_SIZE, travstate, sw * ALW_SRCBUFFER_SIZE); }

	COLLECTGLOBALSTATS_LOOP: for(buffer_type k=0; k<chunk_size[0] * VECTOR_SIZE; k++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
	#pragma HLS PIPELINE II=1
		keyvalue_t keyvalue0 = getkeyvalue(sourcebuffer[0], 0, k); // same
		partition_type gp0 = getglobalpartition(keyvalue0, sweepparams.upperlimit); // same
		
		buffer_type index0 = gp0; // same
		
		keyvalue_t destkeyvalue0 = getkeyvalue(destbuffer[0], 0, index0);
		destkeyvalue0.value += 1;
		setkeyvalue(destbuffer[0], 0, index0, destkeyvalue0); // same
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->globalvar_collectstats_counttotalkvsread(1 * NUMSUBWORKERS);
		#endif 
	}
	return;
}

void 
	#ifdef SW 
	actspartition::
	#endif 
partition0(atp_uint512_dt sourcebuffer[NUMSUBWORKERS][ALW_SRCBUFFER_SIZE], atp_uint512_dt destbuffer[NUMSUBWORKERS][ALW_PADDEDDESTBUFFER_SIZE], keyvalue_t _localcapsule[NUMSUBWORKERS][NUM_PARTITIONS], sweepparams_t sweepparams, atp_travstate_t travstate){
	unsigned int analysis_srcbuffersz = ALW_SRCBUFFER_SIZE * VECTOR_SIZE;
	// keyvalue_t _localcapsule[NUM_PARTITIONS];
	// #pragma HLS ARRAY_PARTITION variable=_localcapsule complete
	
	buffer_type chunk_size[NUMSUBWORKERS];
	for(unsigned int sw=0; sw<NUMSUBWORKERS; sw++){ chunk_size[sw] = getchunksize(ALW_SRCBUFFER_SIZE, travstate, sw * ALW_SRCBUFFER_SIZE); }

	resetmanykeyandvalues(_localcapsule, NUM_PARTITIONS);
	COLLECTSTATS_LOOP: for(buffer_type k=0; k<chunk_size[0] * VECTOR_SIZE; k++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
	#pragma HLS PIPELINE II=1
			keyvalue_t keyvalue0 = getkeyvalue(sourcebuffer[0], 0, k);
			partition_type p0 = getpartition(keyvalue0, sweepparams.currentLOP, sweepparams.upperlimit);
			
			_localcapsule[0][p0].value += 1;
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->globalvar_collectstats_counttotalkvsread(1 * NUMSUBWORKERS);
			#endif 
	}
	calculatemanyoffsets(_localcapsule, NUM_PARTITIONS);
	resetmanykeyandvalues(_localcapsule, NUM_PARTITIONS);
	
	PARTITION_LOOP: for(buffer_type k=0; k<chunk_size[0] * VECTOR_SIZE; k++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
	#pragma HLS PIPELINE II=1
		keyvalue_t keyvalue0 = getkeyvalue(sourcebuffer[0], 0, k);
		partition_type p0 = getpartition(keyvalue0, sweepparams.currentLOP, sweepparams.upperlimit);
		
		buffer_type index0 = _localcapsule[0][p0].key + _localcapsule[0][p0].value;
		
		setkeyvalue(destbuffer[0], 0, index0, keyvalue0);
		_localcapsule[0][p0].value += 1;
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->globalstats_countkvspartitioned(1 * NUMSUBWORKERS);
		#endif 
	}
	
	keyvalue_t InvalidKV; InvalidKV.key = INVALIDDATA; InvalidKV.value = INVALIDDATA;
	APPENDINVALIDS_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		buffer_type ovsize0 = allignhigher_KV(_localcapsule[0][p].value) - _localcapsule[0][p].value;
	
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalvar_savepartitions_countinvalids(ovsize0);
		#endif
		
		buffer_type index0 = _localcapsule[0][p].key + _localcapsule[0][p].value;
	
		APPENDINVALIDS_LOOP1B: for(vector_type v=0; v<VECTOR_SIZE; v++){ 
		#pragma HLS PIPELINE II
			if(v < ovsize0){ setkeyvalue(destbuffer[0], 0, index0 + v, InvalidKV); }
		}
	}
	// copykeyandvalues(localcapsule, _localcapsule, NUM_PARTITIONS);
	return;
}

void 
	#ifdef SW 
	actspartition::
	#endif 
saveKVS0(uint512_dt * kvdram, atp_uint512_dt buffer[NUMSUBWORKERS][ALW_PADDEDDESTBUFFER_SIZE], keyvalue_t * globalcapsule, keyvalue_t localcapsule[NUMSUBWORKERS][NUM_PARTITIONS], batch_type globalbaseaddress_kvs){				
	unsigned int analysis_destpartitionsz = ALW_PADDEDDESTBUFFER_SIZE / NUM_PARTITIONS;
	
	for(unsigned int sw=0; sw<NUMSUBWORKERS; sw++){
		SAVEPARTITIONS_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
			batch_type dramoffset_kvs = globalbaseaddress_kvs + ((globalcapsule[p].key + globalcapsule[p].value) / VECTOR_SIZE);
			buffer_type bramoffset_kvs = localcapsule[sw][p].key / VECTOR_SIZE;
			#ifdef FPGA_IMPL
			buffer_type size_kvs = ALW_PADDEDDESTBUFFER_SIZE / NUM_PARTITIONS;
			#else 
			buffer_type size_kvs = (localcapsule[sw][p].value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			#endif 
			size_kvs = checkandforce(bramoffset_kvs + size_kvs, ALW_PADDEDDESTBUFFER_SIZE);
			
			SAVEPARTITIONS_LOOP1B: for(buffer_type i=0; i<size_kvs; i++){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_destpartitionsz avg=analysis_destpartitionsz
			#pragma HLS PIPELINE II=1
				kvdram[dramoffset_kvs + i] = buffer[sw][bramoffset_kvs + i];
				#ifdef _DEBUGMODE_STATS
				actsutilityobj->globalstats_countkvspartitionswritten_actual(VECTOR_SIZE);
				#endif
			}
			globalcapsule[p].value += allignhigher_KV(localcapsule[sw][p].value);
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countkvspartitionswritten(size_kvs * VECTOR_SIZE);
			actsutilityobj->globalvar_inmemory_counttotalvalidkeyvalues(localcapsule[sw][p].value);
			#endif
		}
	}
	return;
}

void 
	#ifdef SW 
	actspartition::
	#endif 
saveglobalstats0(uint512_dt * kvdram, atp_uint512_dt buffer[NUMSUBWORKERS][ALW_PADDEDDESTBUFFER_SIZE], unsigned int offset_kvs, unsigned int size_kvs){
	for(unsigned sw=0; sw<NUMSUBWORKERS; sw++){
		batch_type dramoffset_kvs = offset_kvs + (sw * ALW_PADDEDDESTBUFFER_SIZE);
		SAVEGLOBALSTATS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
		#pragma HLS PIPELINE II=1
			kvdram[offset_kvs + i] = buffer[sw][i]; 
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalvar_savestats_counttotalstatswritten(VECTOR_SIZE);
			#endif
		}
	}
}

extern "C" {
void 
	#ifdef SW 
	actspartition:: 
	#endif
topkernel( uint512_dt * sourceAvolume ){
#pragma HLS INTERFACE m_axi port = sourceAvolume offset = slave bundle = gmem0

#pragma HLS INTERFACE s_axilite port = sourceAvolume bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATA_PACK variable = sourceAvolume

	atp_uint512_dt sourceAbuffer[NUMSUBWORKERS][ALW_SRCBUFFER_SIZE];
	atp_uint512_dt destAbuffer[NUMSUBWORKERS][ALW_PADDEDDESTBUFFER_SIZE];
	keyvalue_t localcapsuleA[NUMSUBWORKERS][NUM_PARTITIONS];
	keyvalue_t globalcapsuleA[NUM_PARTITIONS];
	
	#pragma HLS array_partition variable = sourceAbuffer //block dim=2
	#pragma HLS array_partition variable = destAbuffer //block dim=2
	#pragma HLS array_partition variable = localcapsuleA //block dim=2
	#pragma HLS DATA_PACK variable = sourceAbuffer
	#pragma HLS DATA_PACK variable = destAbuffer
	
	unsigned int analysis_mainloop = KVDATA_BATCHSIZE_KVS / ALW_SRCBUFFER_SIZE;
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->printglobalvars();
	actsutilityobj->clearglobalvars();
	#endif
	
	alw_globalparams_t globalparamsA;
	getglobalparams(sourceAvolume, &globalparamsA);
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->printglobalparameters("actspartition:: printing global parameters for instance A", globalparamsA);
	#endif
	
	batch_type currentreadindex = 0;
	batch_type currentsaveindex = NUM_PARTITIONS;
	batch_type sourcebaseaddress_kvs = 0;
	batch_type destbaseaddress_kvs = BASEOFFSET_KVDRAMWORKSPACE_KVS;
	sweepparams_t sweepparams; sweepparams.upperlimit = globalparamsA.vbegin;
	batch_type runsize_kvs = globalparamsA.runsize / VECTOR_SIZE;
	atp_travstate_t travstate; travstate.begin_kvs = 0; travstate.end_kvs = runsize_kvs; travstate.skip_kvs = ALW_SRCBUFFER_SIZE;
	resetkeyandvalues(globalcapsuleA, NUM_PARTITIONS);
	resetkeyandvalues(destAbuffer);
	
	COLLECTSTATS_MAINLOOP: for(unsigned int offset_kvs=0; offset_kvs<runsize_kvs; offset_kvs+=NUMSUBWORKERS * ALW_SRCBUFFER_SIZE){
		travstate.i_kvs = offset_kvs;
			
		readKVS0(sourceAvolume, sourceAbuffer, (sourcebaseaddress_kvs + offset_kvs), travstate);
		
		collectglobalstats0(sourceAbuffer, destAbuffer, sweepparams, travstate);
	}
	saveglobalstats0(sourceAvolume, destAbuffer, BASEOFFSET2_STATSDRAM_KVS, ALW_PADDEDDESTBUFFER_SIZE);
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->printkeyvalues("actspartition::collectglobalstats::", (keyvalue_t *)destAbuffer, NUM_PARTITIONS);
	actsutilityobj->printglobalvars();
	actsutilityobj->clearglobalvars();
	#endif 
	exit(EXIT_SUCCESS);
	
	for(unsigned int currentreadindex=1; currentreadindex<2; currentreadindex+=1){
		if(currentreadindex == 1){ sweepparams.currentLOP = 1; } else if(currentreadindex == 16){ sweepparams.currentLOP = 1; } else if(currentreadindex == 256){ sweepparams.currentLOP = 1; } else if(currentreadindex == 4096){ sweepparams.currentLOP = 1; } else if(currentreadindex == 65536){} else if(currentreadindex == 1048576){ sweepparams.currentLOP = 1; } else { sweepparams.currentLOP = 1; }				
	
		read(sourceAvolume, globalcapsuleA, BASEOFFSET_STATSDRAM + currentreadindex, NUM_PARTITIONS);
		
		resetkeyandvalues(globalcapsuleA, NUM_PARTITIONS); // REMOVEME.

		MAIN_LOOP: for(unsigned int offset_kvs=0; offset_kvs<runsize_kvs; offset_kvs+=NUMSUBWORKERS * ALW_SRCBUFFER_SIZE){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_mainloop avg=analysis_mainloop	
			travstate.i_kvs = offset_kvs;
			
			readKVS0(sourceAvolume, sourceAbuffer, (sourcebaseaddress_kvs + offset_kvs), travstate);
			
			partition0(sourceAbuffer, destAbuffer, localcapsuleA, sweepparams, travstate);
			
			saveKVS0(sourceAvolume, destAbuffer, globalcapsuleA, localcapsuleA, destbaseaddress_kvs);
		}
		
		save(sourceAvolume, globalcapsuleA, BASEOFFSET_STATSDRAM + currentsaveindex, NUM_PARTITIONS);
		
		currentsaveindex += NUM_PARTITIONS;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->printkeyvalues("actspartition::head::globalcapsule", (keyvalue_t *)globalcapsuleA, NUM_PARTITIONS);
	actsutilityobj->printglobalvars();
	actsutilityobj->clearglobalvars();
	#endif 
	return;
}
}








