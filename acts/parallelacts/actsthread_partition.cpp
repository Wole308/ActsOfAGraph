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

unsigned int analysis_srcbuffersz = ATP_SRCBUFFER_SIZE;
unsigned int analysis_destbuffersz = ATP_PADDEDDESTBUFFER_SIZE;
unsigned int analysis_destpartitionsz = ATP_PADDEDDESTBUFFER_SIZE / NUM_PARTITIONS;

#ifdef SW
actsthread_partition::actsthread_partition(){ actsutilityobj = new actsutility(); }
actsthread_partition::~actsthread_partition(){}
#endif 

unsigned int 
	#ifdef SW 
	actsthread_partition::
	#endif 
allignhigher_KV(unsigned int val){
	unsigned int fac = (val + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	return (fac * VECTOR_SIZE);
}
unsigned int 
	#ifdef SW 
	actsthread_partition::
	#endif 
checkandforce(unsigned int val, unsigned int limit){ if(val >= limit){ return 0; } else { return val; }}
unsigned int 
	#ifdef SW 
	actsthread_partition::
	#endif 
getpartition(keyvalue_t keyvalue, unsigned int currentLOP, vertex_t upperlimit){
	partition_type partition = 2;//((keyvalue.key - upperlimit) >> (BATCH_RANGE_POW - (NUM_PARTITIONS_POW * currentLOP)));
	partition = checkandforce(partition, NUM_PARTITIONS);
	// cout<<"--- actsthread_partition::getpartition:: partition: "<<partition<<endl; 
	return partition;
}

void 
	#ifdef SW 
	actsthread_partition::
	#endif 
read(keyvalue_t * kvdram, keyvalue_t * buffer, batch_type offset_kvs, buffer_type size_kvs){
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
read(uint512_dt * kvdram, uint512_dt * buffer, batch_type offset_kvs, buffer_type size_kvs){
	READ_LOOP: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
		for(vector_type v=0; v<VECTOR_SIZE; v++){
		#pragma HLS PIPELINE II=1
			buffer[i].data[v] = kvdram[offset_kvs + i].data[v];
		}
	}
	return;
}
void 
	#ifdef SW 
	actsthread_partition::
	#endif 
partition(uint512_dt * sourcebuffer, uint512_dt * destbuffer, keyvalue_t * localcapsule, buffer_type size_kvs, sweepparams_t sweepparams){
	COLLECTSTATS_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ localcapsule[p].key = 0; localcapsule[p].value = 0; }
	COLLECTSTATS_LOOP2: for(buffer_type i=0; i<size_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
		COLLECTSTATS_LOOP2B: for(buffer_type j=0; j<VECTOR_SIZE; j++){
		#pragma HLS PIPELINE II=1
			keyvalue_t keyvalue = sourcebuffer[i].data[j];
			partition_type p = getpartition(keyvalue, sweepparams.currentLOP, sweepparams.upperlimit);

			localcapsule[p].value += 1;
			// actsutilityobj->globalvar_collectstats_counttotalkvsread(1);
		}
	}
	PARTITION_LOOP3: for(partition_type p=1; p<NUM_PARTITIONS; p++){ localcapsule[p].key = allignhigher_KV(localcapsule[p-1].key + localcapsule[p-1].value); }
	PARTITION_LOOP4: for(partition_type p=0; p<NUM_PARTITIONS; p++){ localcapsule[p].key = 0; localcapsule[p].value = 0; }
	PARTITION_LOOP5: for(buffer_type i=0; i<size_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_srcbuffersz avg=analysis_srcbuffersz	
		PARTITION_LOOP5B: for(buffer_type j=0; j<VECTOR_SIZE; j++){
		#pragma HLS PIPELINE II=1
			keyvalue_t keyvalue = sourcebuffer[i].data[j];
			partition_type p = getpartition(keyvalue, sweepparams.currentLOP, sweepparams.upperlimit);
			
			buffer_type index = localcapsule[p].key + localcapsule[p].value;
			batch_type idy = index / VECTOR_SIZE;
			vector_type idx = index % VECTOR_SIZE;
			
			destbuffer[idy].data[idx] = keyvalue;
			localcapsule[p].value += 1;
			// actsutilityobj->globalstats_countkvspartitioned(1);
		}
	}
}
void 
	#ifdef SW 
	actsthread_partition::
	#endif 
save(uint512_dt * kvdram, uint512_dt * buffer, keyvalue_t * globalcapsule, keyvalue_t * localcapsule, batch_type globalbaseaddress_kvs){				
	SAVEPARTITIONS_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		batch_type dramoffset_kvs = globalbaseaddress_kvs + ((globalcapsule[p].key + globalcapsule[p].value) / VECTOR_SIZE);
		buffer_type bramoffset_kvs = localcapsule[p].key / VECTOR_SIZE;
		// buffer_type size_kvs = (localcapsule[p].value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
		buffer_type size_kvs = ATP_PADDEDDESTBUFFER_SIZE / NUM_PARTITIONS; // FIXME.
		size_kvs = checkandforce(bramoffset_kvs + size_kvs, ATP_PADDEDDESTBUFFER_SIZE);
		
		SAVEPARTITIONS_LOOP1B: for(buffer_type i=0; i<size_kvs; i++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_destpartitionsz avg=analysis_destpartitionsz
			for(vector_type v=0; v<VECTOR_SIZE; v++){
			#pragma HLS PIPELINE II=1
				kvdram[dramoffset_kvs + i].data[v] = buffer[bramoffset_kvs + i].data[v];
			}
		}
		// actsutilityobj->globalstats_countkvspartitionswritten_actual(size_kvs * VECTOR_SIZE);
		globalcapsule[p].value += allignhigher_KV(localcapsule[p].value);
	}
	return;
}

extern "C" {
void 
	#ifdef SW 
	actsthread_partition:: 
	#endif
topkernel(uint512_dt * sourcevolume, uint512_dt * targetvolume, keyvalue_t * stats){
#pragma HLS INTERFACE m_axi port = sourcevolume offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = targetvolume offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = stats offset = slave bundle = gmem2

#pragma HLS INTERFACE s_axilite port = sourcevolume bundle = control
#pragma HLS INTERFACE s_axilite port = targetvolume bundle = control
#pragma HLS INTERFACE s_axilite port = stats bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATA_PACK variable = sourcevolume
#pragma HLS DATA_PACK variable = targetvolume
#pragma HLS DATA_PACK variable = stats
	uint512_dt sourcebuffer[ATP_SRCBUFFER_SIZE];
	uint512_dt destbuffer[ATP_PADDEDDESTBUFFER_SIZE];
	keyvalue_t globalcapsule[NUM_PARTITIONS];
	keyvalue_t localcapsule[NUM_PARTITIONS];
	batch_type currentreadindex = 0;
	batch_type currentsaveindex = NUM_PARTITIONS;
	batch_type sourcebaseaddress_kvs = 0;
	batch_type destbaseaddress_kvs = 0; // BASEOFFSET_KVDRAMWORKSPACE_KVS;
	sweepparams_t sweepparams; sweepparams.upperlimit = 0;
	batch_type runsize_kvs = 16; // stats[BASEOFFSET_MESSAGESDRAM + MESSAGES_RUNSIZE].key / VECTOR_SIZE;
	for(partition_type p=0; p<NUM_PARTITIONS; p++){ globalcapsule[p].key = 0; globalcapsule[p].value = 0; }
	
	for(unsigned int currentreadindex=1; currentreadindex<2; currentreadindex+=1){
		if(currentreadindex == 1){ sweepparams.currentLOP = 1; } else if(currentreadindex == 16){ sweepparams.currentLOP = 1; } else if(currentreadindex == 256){ sweepparams.currentLOP = 1; } else if(currentreadindex == 4096){ sweepparams.currentLOP = 1; } else if(currentreadindex == 65536){} else if(currentreadindex == 1048576){ sweepparams.currentLOP = 1; } else { sweepparams.currentLOP = 1; }				
		read(stats, globalcapsule, BASEOFFSET_STATSDRAM + currentreadindex, NUM_PARTITIONS);
		for(partition_type p=0; p<NUM_PARTITIONS; p++){ globalcapsule[p].key = 0; globalcapsule[p].value = 0; } // REMOVEME.
		for(unsigned int offset_kvs=0; offset_kvs<runsize_kvs; offset_kvs+=ATP_SRCBUFFER_SIZE){
			read(sourcevolume, sourcebuffer, (sourcebaseaddress_kvs + offset_kvs), ATP_SRCBUFFER_SIZE);
			partition(sourcebuffer, destbuffer, localcapsule, ATP_SRCBUFFER_SIZE, sweepparams);
			save(sourcevolume, destbuffer, globalcapsule, localcapsule, destbaseaddress_kvs);
		}
		save(stats, globalcapsule, BASEOFFSET_STATSDRAM + currentsaveindex, NUM_PARTITIONS);
		currentsaveindex += NUM_PARTITIONS;
	}
	// actsutilityobj->printkeyvalues("actsthread_partition::head::globalcapsule", (keyvalue_t *)globalcapsule, NUM_PARTITIONS);
	return;
}
}









