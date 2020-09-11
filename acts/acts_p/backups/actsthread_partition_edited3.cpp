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
#ifndef FPGA_IMPL
#include "../../src/utility/utility.h"
#include "../actsutility/actsutility.h"
#endif 
#include "actsthread_partition.h"
using namespace std;

#ifdef SW
actsthread_partition::actsthread_partition(){ actsutilityobj = new actsutility(); }
actsthread_partition::~actsthread_partition(){}
#endif 

unsigned int 
	#ifdef SW 
	actsthread_partition::
	#endif 
checkandforce(unsigned int val, unsigned int limit){ if(val >= limit){ return 0; } else { return val; }}
unsigned int 
	#ifdef SW 
	actsthread_partition::
	#endif 
allignhigher_KV(unsigned int val){
	unsigned int fac = (val + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	return (fac * VECTOR_SIZE);
}
buffer_type 
	#ifdef SW 
	actsthread_partition::
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
	actsthread_partition::
	#endif 
getpartition(keyvalue_t keyvalue, unsigned int currentLOP, vertex_t upperlimit){
	partition_type partition = keyvalue.key - upperlimit; // ((keyvalue.key - upperlimit) >> (BATCH_RANGE_POW - (NUM_PARTITIONS_POW * currentLOP))); // REMOVEME.
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("actsthread_partition::getpartition", partition, NUM_PARTITIONS, keyvalue.key, upperlimit, currentLOP);
	#endif
	// partition = checkandforce(partition, NUM_PARTITIONS);
	return partition;
}

void 
	#ifdef SW 
	actsthread_partition::
	#endif 
read(keyvalue_t * kvdram, keyvalue_t * buffer, batch_type offset_kvs, buffer_type size_kvs){
	unsigned int analysis_srcbuffersz = NUM_PARTITIONS;
	READ_LOOP: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS PIPELINE II=1
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
		buffer[i] = kvdram[offset_kvs + i];
	}
	return;
}
void 
	#ifdef SW 
	actsthread_partition::
	#endif 
save(keyvalue_t * kvdram, keyvalue_t * buffer, batch_type offset_kvs, buffer_type size_kvs){
	unsigned int analysis_srcbuffersz = NUM_PARTITIONS;
	READ_LOOP: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS PIPELINE II=1
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
		kvdram[offset_kvs + i] = buffer[i];
	}
	return;
}

void 
	#ifdef SW 
	actsthread_partition::
	#endif 
readmany(uint512_dt * kvdram1, uint512_dt * kvdram2, uint512_dt * kvdram3, uint512_dt * buffer1, uint512_dt * buffer2, uint512_dt * buffer3, batch_type offset1_kvs, batch_type offset2_kvs, batch_type offset3_kvs, atp_travstate_t travstate){
	unsigned int analysis_srcbuffersz = ATP_SRCBUFFER_SIZE;
	buffer_type chunk_size = getchunksize(ATP_SRCBUFFER_SIZE, travstate, 0);
	READ_LOOP: for (buffer_type i=0; i<chunk_size; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
	#pragma HLS PIPELINE II=1
		for(vector_type v=0; v<VECTOR_SIZE; v++){
		#pragma HLS UNROLL
			buffer1[i].data[v] = kvdram1[offset1_kvs + i].data[v];
			buffer2[i].data[v] = kvdram2[offset2_kvs + i].data[v];
			buffer3[i].data[v] = kvdram3[offset3_kvs + i].data[v];
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countkvsread(1);
			#endif
		}
	}
	return;
}
void 
	#ifdef SW 
	actsthread_partition::
	#endif 
partitionmany(uint512_dt * source1buffer, uint512_dt * source2buffer, uint512_dt * source3buffer, uint512_dt * dest1buffer, uint512_dt * dest2buffer, uint512_dt * dest3buffer, keyvalue_st * localcapsule1, keyvalue_st * localcapsule2, keyvalue_st * localcapsule3, sweepparams_t sweepparams, atp_travstate_t travstate){
	unsigned int analysis_srcbuffersz = ATP_SRCBUFFER_SIZE;
	buffer_type chunk_size = getchunksize(ATP_SRCBUFFER_SIZE, travstate, 0);
	for(partition_type p=0; p<NUM_PARTITIONS; p++){ localcapsule1[p].key = 0; localcapsule1[p].value = 0; localcapsule2[p].key = 0; localcapsule2[p].value = 0; localcapsule3[p].key = 0; localcapsule3[p].value = 0; }
	PARTITION_LOOP1: for(unsigned int si=0; si<2; si++){
		for(partition_type p=0; p<NUM_PARTITIONS; p++){ localcapsule1[p].value = 0; localcapsule2[p].value = 0; localcapsule3[p].value = 0; }
		PARTITION_LOOP2: for(buffer_type i=0; i<chunk_size; i++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
			PARTITION_LOOP5B: for(buffer_stype j=0; j<VECTOR_SIZE; j++){
			#pragma HLS PIPELINE II=1
				keyvalue_t keyvalue1 = source1buffer[i].data[j];
				keyvalue_t keyvalue2 = source2buffer[i].data[j];
				keyvalue_t keyvalue3 = source3buffer[i].data[j];
				partition_type p1 = getpartition(keyvalue1, sweepparams.currentLOP, sweepparams.upperlimit);
				partition_type p2 = getpartition(keyvalue2, sweepparams.currentLOP, sweepparams.upperlimit);
				partition_type p3 = getpartition(keyvalue3, sweepparams.currentLOP, sweepparams.upperlimit);
				
				buffer_stype index1 = localcapsule1[p1].key + localcapsule1[p1].value;
				buffer_type index2 = localcapsule2[p2].key + localcapsule2[p2].value;
				buffer_type index3 = localcapsule3[p3].key + localcapsule3[p3].value;
				buffer_stype idy1 = index1 / VECTOR_SIZE; vector_type idx1 = index1 % VECTOR_SIZE; 
				batch_type idy2 = index2 / VECTOR_SIZE; vector_type idx2 = index2 % VECTOR_SIZE;
				batch_type idy3 = index3 / VECTOR_SIZE; vector_type idx3 = index3 % VECTOR_SIZE;
				
				if(si>0){
				dest1buffer[idy1].data[idx1] = keyvalue1;
				dest2buffer[idy2].data[idx2] = keyvalue2;
				dest3buffer[idy3].data[idx3] = keyvalue3;}
				
				localcapsule1[p1].value += 1;
				localcapsule2[p2].value += 1;
				localcapsule3[p3].value += 1;
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->globalstats_countkvspartitioned(1);
				#endif 
			}
		}
		PARTITION_LOOP3: for(partition_type p=1; p<NUM_PARTITIONS; p++){ localcapsule1[p].key = allignhigher_KV(localcapsule1[p-1].key + localcapsule1[p-1].value); localcapsule2[p].key = allignhigher_KV(localcapsule2[p-1].key + localcapsule2[p-1].value); localcapsule3[p].key = allignhigher_KV(localcapsule3[p-1].key + localcapsule3[p-1].value); }
	}
	keyvalue_t InvalidKV; InvalidKV.key = INVALIDDATA; InvalidKV.value = INVALIDDATA;
	APPENDINVALIDS_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		buffer_stype ovsize1 = allignhigher_KV(localcapsule1[p].value) - localcapsule1[p].value;
		buffer_type ovsize2 = allignhigher_KV(localcapsule2[p].value) - localcapsule2[p].value;
		buffer_type ovsize3 = allignhigher_KV(localcapsule3[p].value) - localcapsule3[p].value;
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalvar_savepartitions_countinvalids(ovsize);
		#endif
		
		buffer_stype index1 = localcapsule1[p].key + localcapsule1[p].value;
		buffer_type index2 = localcapsule2[p].key + localcapsule2[p].value;
		buffer_type index3 = localcapsule3[p].key + localcapsule3[p].value;
		buffer_stype idy1 = index1 / VECTOR_SIZE; vector_type idx1 = index1 % VECTOR_SIZE;
		batch_type idy2 = index2 / VECTOR_SIZE; vector_type idx2 = index2 % VECTOR_SIZE;
		batch_type idy3 = index3 / VECTOR_SIZE; vector_type idx3 = index3 % VECTOR_SIZE;
	
		APPENDINVALIDS_LOOP1B: for(vector_type v=0; v<VECTOR_SIZE; v++){
		#pragma HLS PIPELINE
			if(v < ovsize1){ dest1buffer[idy1].data[idx1 + v] = InvalidKV; }
			if(v < ovsize2){ dest2buffer[idy2].data[idx2 + v] = InvalidKV; }
			if(v < ovsize3){ dest3buffer[idy3].data[idx3 + v] = InvalidKV; }
		}
	}
	return;
}
void 
	#ifdef SW 
	actsthread_partition::
	#endif 
saveKVS1(uint512_dt * kvdram, uint512_dt * buffer, keyvalue_t * globalcapsule, keyvalue_st * localcapsule, batch_type globalbaseaddress_kvs){				
	unsigned int analysis_destpartitionsz = ATP_PADDEDDESTBUFFER_SIZE / NUM_PARTITIONS;
	SAVEPARTITIONS_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		batch_type dramoffset_kvs = globalbaseaddress_kvs + ((globalcapsule[p].key + globalcapsule[p].value) / VECTOR_SIZE);
		buffer_type bramoffset_kvs = localcapsule[p].key / VECTOR_SIZE;
		#ifdef FPGA_IMPL
		buffer_type size_kvs = ATP_PADDEDDESTBUFFER_SIZE / NUM_PARTITIONS;
		#else 
		buffer_type size_kvs = (localcapsule[p].value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
		#endif 
		size_kvs = checkandforce(bramoffset_kvs + size_kvs, ATP_PADDEDDESTBUFFER_SIZE);
		
		SAVEPARTITIONS_LOOP1B: for(buffer_type i=0; i<size_kvs; i++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_destpartitionsz avg=analysis_destpartitionsz
		#pragma HLS PIPELINE II=1
			SAVEPARTITIONS_LOOP1C: for(vector_type v=0; v<VECTOR_SIZE; v++){
			#pragma HLS UNROLL
				kvdram[dramoffset_kvs + i].data[v] = buffer[bramoffset_kvs + i].data[v];
				#ifdef _DEBUGMODE_STATS
				actsutilityobj->globalstats_countkvspartitionswritten_actual(VECTOR_SIZE);
				#endif
			}
		}
		globalcapsule[p].value += allignhigher_KV(localcapsule[p].value);
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvspartitionswritten(size_kvs * VECTOR_SIZE);
		actsutilityobj->globalvar_inmemory_counttotalvalidkeyvalues(localcapsule[p].value);
		#endif
	}
	return;
}
void 
	#ifdef SW 
	actsthread_partition::
	#endif 
saveKVS2(uint512_dt * kvdram, uint512_dt * buffer, keyvalue_t * globalcapsule, keyvalue_st * localcapsule, batch_type globalbaseaddress_kvs){				
	unsigned int analysis_destpartitionsz = ATP_PADDEDDESTBUFFER_SIZE / NUM_PARTITIONS;
	SAVEPARTITIONS_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		batch_type dramoffset_kvs = globalbaseaddress_kvs + ((globalcapsule[p].key + globalcapsule[p].value) / VECTOR_SIZE);
		buffer_type bramoffset_kvs = localcapsule[p].key / VECTOR_SIZE;
		#ifdef FPGA_IMPL
		buffer_type size_kvs = ATP_PADDEDDESTBUFFER_SIZE / NUM_PARTITIONS;
		#else 
		buffer_type size_kvs = (localcapsule[p].value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
		#endif 
		size_kvs = checkandforce(bramoffset_kvs + size_kvs, ATP_PADDEDDESTBUFFER_SIZE);
		
		SAVEPARTITIONS_LOOP1B: for(buffer_type i=0; i<size_kvs; i++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_destpartitionsz avg=analysis_destpartitionsz
		#pragma HLS PIPELINE II=1
			SAVEPARTITIONS_LOOP1C: for(vector_type v=0; v<VECTOR_SIZE; v++){
			#pragma HLS UNROLL
				kvdram[dramoffset_kvs + i].data[v] = buffer[bramoffset_kvs + i].data[v];
				#ifdef _DEBUGMODE_STATS
				actsutilityobj->globalstats_countkvspartitionswritten_actual(VECTOR_SIZE);
				#endif
			}
		}
		globalcapsule[p].value += allignhigher_KV(localcapsule[p].value);
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvspartitionswritten(size_kvs * VECTOR_SIZE);
		actsutilityobj->globalvar_inmemory_counttotalvalidkeyvalues(localcapsule[p].value);
		#endif
	}
	return;
}
void 
	#ifdef SW 
	actsthread_partition::
	#endif 
saveKVS3(uint512_dt * kvdram, uint512_dt * buffer, keyvalue_t * globalcapsule, keyvalue_st * localcapsule, batch_type globalbaseaddress_kvs){				
	unsigned int analysis_destpartitionsz = ATP_PADDEDDESTBUFFER_SIZE / NUM_PARTITIONS;
	SAVEPARTITIONS_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		batch_type dramoffset_kvs = globalbaseaddress_kvs + ((globalcapsule[p].key + globalcapsule[p].value) / VECTOR_SIZE);
		buffer_type bramoffset_kvs = localcapsule[p].key / VECTOR_SIZE;
		#ifdef FPGA_IMPL
		buffer_type size_kvs = ATP_PADDEDDESTBUFFER_SIZE / NUM_PARTITIONS;
		#else 
		buffer_type size_kvs = (localcapsule[p].value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
		#endif 
		size_kvs = checkandforce(bramoffset_kvs + size_kvs, ATP_PADDEDDESTBUFFER_SIZE);
		
		SAVEPARTITIONS_LOOP1B: for(buffer_type i=0; i<size_kvs; i++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_destpartitionsz avg=analysis_destpartitionsz
		#pragma HLS PIPELINE II=1
			SAVEPARTITIONS_LOOP1C: for(vector_type v=0; v<VECTOR_SIZE; v++){
			#pragma HLS UNROLL
				kvdram[dramoffset_kvs + i].data[v] = buffer[bramoffset_kvs + i].data[v];
				#ifdef _DEBUGMODE_STATS
				actsutilityobj->globalstats_countkvspartitionswritten_actual(VECTOR_SIZE);
				#endif
			}
		}
		globalcapsule[p].value += allignhigher_KV(localcapsule[p].value);
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvspartitionswritten(size_kvs * VECTOR_SIZE);
		actsutilityobj->globalvar_inmemory_counttotalvalidkeyvalues(localcapsule[p].value);
		#endif
	}
	return;
}

extern "C" {
void 
	#ifdef SW 
	actsthread_partition:: 
	#endif
topkernel(uint512_dt * sourceAvolume, uint512_dt * sourceBvolume, uint512_dt * sourceCvolume, keyvalue_t * statsA, keyvalue_t * statsB, keyvalue_t * statsC){
#pragma HLS INTERFACE m_axi port = sourceAvolume offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = statsA offset = slave bundle = gmem0

#pragma HLS INTERFACE m_axi port = sourceBvolume offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = statsB offset = slave bundle = gmem1

#pragma HLS INTERFACE m_axi port = sourceCvolume offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = statsC offset = slave bundle = gmem2

// #pragma HLS INTERFACE m_axi port = sourceAvolume offset = slave bundle = gmem0
// #pragma HLS INTERFACE m_axi port = statsA offset = slave bundle = gmem1

// #pragma HLS INTERFACE m_axi port = sourceBvolume offset = slave bundle = gmem2
// #pragma HLS INTERFACE m_axi port = statsB offset = slave bundle = gmem3

// #pragma HLS INTERFACE m_axi port = sourceCvolume offset = slave bundle = gmem4
// #pragma HLS INTERFACE m_axi port = statsC offset = slave bundle = gmem5

#pragma HLS INTERFACE s_axilite port = sourceAvolume bundle = control
#pragma HLS INTERFACE s_axilite port = statsA bundle = control

#pragma HLS INTERFACE s_axilite port = sourceBvolume bundle = control
#pragma HLS INTERFACE s_axilite port = statsB bundle = control

#pragma HLS INTERFACE s_axilite port = sourceCvolume bundle = control
#pragma HLS INTERFACE s_axilite port = statsC bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATA_PACK variable = sourceAvolume
#pragma HLS DATA_PACK variable = statsA

#pragma HLS DATA_PACK variable = sourceBvolume
#pragma HLS DATA_PACK variable = statsB

#pragma HLS DATA_PACK variable = sourceCvolume
#pragma HLS DATA_PACK variable = statsC

	uint512_dt sourceAbuffer[ATP_SRCBUFFER_SIZE];
	uint512_dt destAbuffer[ATP_PADDEDDESTBUFFER_SIZE];
	keyvalue_t globalcapsuleA[NUM_PARTITIONS];
	keyvalue_st localcapsuleA[NUM_PARTITIONS];
	
	uint512_dt sourceBbuffer[ATP_SRCBUFFER_SIZE];
	uint512_dt destBbuffer[ATP_PADDEDDESTBUFFER_SIZE];
	keyvalue_t globalcapsuleB[NUM_PARTITIONS];
	keyvalue_st localcapsuleB[NUM_PARTITIONS];
	
	uint512_dt sourceCbuffer[ATP_SRCBUFFER_SIZE];
	uint512_dt destCbuffer[ATP_PADDEDDESTBUFFER_SIZE];
	keyvalue_t globalcapsuleC[NUM_PARTITIONS];
	keyvalue_st localcapsuleC[NUM_PARTITIONS];
	
	#pragma HLS DATA_PACK variable = sourceAbuffer
	#pragma HLS DATA_PACK variable = destAbuffer

	#pragma HLS DATA_PACK variable = sourceBbuffer
	#pragma HLS DATA_PACK variable = destBbuffer
	
	#pragma HLS DATA_PACK variable = sourceCbuffer
	#pragma HLS DATA_PACK variable = destCbuffer
	
	unsigned int analysis_mainloop = KVDATA_BATCHSIZE_KVS / ATP_SRCBUFFER_SIZE;
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->printglobalvars();
	actsutilityobj->clearglobalvars();
	actsutilityobj->print4("acts::printparameters:: ATP_PADDEDDESTBUFFER_SIZE (KVS)", "ATP_PADDEDDESTBUFFER_SIZE (KV)", "ATP_SRCBUFFER_SIZE (KVS)", "ATP_SRCBUFFER_SIZE (KV)", ATP_PADDEDDESTBUFFER_SIZE, ATP_PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, ATP_SRCBUFFER_SIZE, ATP_SRCBUFFER_SIZE * VECTOR_SIZE);
	#endif
	
	batch_type currentreadindex = 0;
	batch_type currentsaveindex = NUM_PARTITIONS;
	batch_type sourcebaseaddress_kvs = 0;
	batch_type destbaseaddress_kvs = BASEOFFSET_KVDRAMWORKSPACE_KVS;
	sweepparams_t sweepparams; sweepparams.upperlimit = 0;
	batch_type runsize_kvs = KVDATA_BATCHSIZE_KVS; // 16; // stats[BASEOFFSET_MESSAGESDRAM + MESSAGES_RUNSIZE].key / VECTOR_SIZE;
	atp_travstate_t travstate; travstate.begin_kvs = 0; travstate.end_kvs = runsize_kvs; travstate.skip_kvs = ATP_SRCBUFFER_SIZE;
	
	for(partition_type p=0; p<NUM_PARTITIONS; p++){ globalcapsuleA[p].key = 0; globalcapsuleA[p].value = 0; }
	for(partition_type p=0; p<NUM_PARTITIONS; p++){ globalcapsuleB[p].key = 0; globalcapsuleB[p].value = 0; }
	for(partition_type p=0; p<NUM_PARTITIONS; p++){ globalcapsuleC[p].key = 0; globalcapsuleC[p].value = 0; }
	
	for(unsigned int currentreadindex=1; currentreadindex<2; currentreadindex+=1){
		if(currentreadindex == 1){ sweepparams.currentLOP = 1; } else if(currentreadindex == 16){ sweepparams.currentLOP = 1; } else if(currentreadindex == 256){ sweepparams.currentLOP = 1; } else if(currentreadindex == 4096){ sweepparams.currentLOP = 1; } else if(currentreadindex == 65536){} else if(currentreadindex == 1048576){ sweepparams.currentLOP = 1; } else { sweepparams.currentLOP = 1; }				
		read(statsA, globalcapsuleA, BASEOFFSET_STATSDRAM + currentreadindex, NUM_PARTITIONS);
		read(statsB, globalcapsuleB, BASEOFFSET_STATSDRAM + currentreadindex, NUM_PARTITIONS);
		read(statsC, globalcapsuleC, BASEOFFSET_STATSDRAM + currentreadindex, NUM_PARTITIONS);
		
		for(partition_type p=0; p<NUM_PARTITIONS; p++){ globalcapsuleA[p].key = 0; globalcapsuleA[p].value = 0; } // REMOVEME.
		for(partition_type p=0; p<NUM_PARTITIONS; p++){ globalcapsuleB[p].key = 0; globalcapsuleB[p].value = 0; } // REMOVEME.
		for(partition_type p=0; p<NUM_PARTITIONS; p++){ globalcapsuleC[p].key = 0; globalcapsuleC[p].value = 0; } // REMOVEME.

		MAIN_LOOP: for(unsigned int offset_kvs=0; offset_kvs<runsize_kvs; offset_kvs+=ATP_SRCBUFFER_SIZE){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_mainloop avg=analysis_mainloop	
			travstate.i_kvs = offset_kvs;
			
			readmany(sourceAvolume, sourceBvolume, sourceCvolume, sourceAbuffer, sourceBbuffer, sourceCbuffer, (sourcebaseaddress_kvs + offset_kvs), (sourcebaseaddress_kvs + offset_kvs), (sourcebaseaddress_kvs + offset_kvs), travstate);
			
			partitionmany(sourceAbuffer, sourceBbuffer, sourceCbuffer, destAbuffer, destBbuffer, destCbuffer, localcapsuleA, localcapsuleB, localcapsuleC, sweepparams, travstate);
			
			saveKVS1(sourceAvolume, destAbuffer, globalcapsuleA, localcapsuleA, destbaseaddress_kvs);
			saveKVS2(sourceBvolume, destBbuffer, globalcapsuleB, localcapsuleB, destbaseaddress_kvs);
			saveKVS3(sourceCvolume, destCbuffer, globalcapsuleC, localcapsuleC, destbaseaddress_kvs);
		}
		
		save(statsA, globalcapsuleA, BASEOFFSET_STATSDRAM + currentsaveindex, NUM_PARTITIONS);
		save(statsB, globalcapsuleB, BASEOFFSET_STATSDRAM + currentsaveindex, NUM_PARTITIONS);
		save(statsC, globalcapsuleC, BASEOFFSET_STATSDRAM + currentsaveindex, NUM_PARTITIONS);
		
		currentsaveindex += NUM_PARTITIONS;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->printkeyvalues("actsthread_partition::head::globalcapsule", (keyvalue_t *)globalcapsuleA, NUM_PARTITIONS);
	actsutilityobj->printglobalvars();
	actsutilityobj->clearglobalvars();
	#endif 
	return;
}
}









