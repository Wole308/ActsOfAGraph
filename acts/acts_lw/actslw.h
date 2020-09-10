#ifndef ACTSLW_H
#define ACTSLW_H
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
#include "../include/actscommon.h"
#include "../../include/common.h"
#ifndef FPGA_IMPL
#include "../../src/utility/utility.h"
#endif
#include "../../acts/actsutility/actsutility.h"
using namespace std;

#define NUMPIPELINES 1
#if NUMPIPELINES==3
#define PP0
#define PP1
#define PP2
#endif 

typedef unsigned int batch_type;
typedef unsigned int buffer_type;
typedef unsigned int partition_type;
typedef unsigned int vector_type;

typedef struct {
	unsigned int currentLOP;
	unsigned int upperlimit;
	unsigned int source_partition;
	unsigned int worksourcebaseaddress_kvs;
	unsigned int workdestbaseaddress_kvs;
} alw_sweepparams_t;

typedef struct {
	unsigned int topi_kvs;
	unsigned int i_kvs;
	unsigned int begin_kvs;
	unsigned int end_kvs;
	unsigned int skip_kvs;
	unsigned int info;
} alw_travstate_t;

typedef struct {
	unsigned int enablecollectglobalstats;
	unsigned int enablepartition;
	unsigned int enablereduce;
} alw_config;

class actslw {
public:
	actslw();
	~actslw();
	
	unsigned int subandcap(unsigned int val1, unsigned int val2);
	unsigned int lowercapto(unsigned int val, unsigned int capval);
	unsigned int allignhigher_KV(unsigned int val);
	batch_type getskipsize(unsigned int currentLOP, unsigned int sourceORdest);
	void getglobalparams(uint512_dt * sourcevolume, globalparams_t * globalparams);
	void copykeyandvalues(keyvalue_t * buffer1, keyvalue_t * buffer2, unsigned int size);
	void resetkeyandvalues(keyvalue_t * buffer, unsigned int size);
	void resetvalues(keyvalue_t * buffer, unsigned int size);
	void resetkeyandvalues(unsigned int enable, uint512_dt destbuffer[PADDEDDESTBUFFER_SIZE]);
	void resetmanykeyandvalues(keyvalue_t buffer[VECTOR_SIZE][NUM_PARTITIONS], unsigned int size);
	void resetmanykeyandvalues(keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], unsigned int size);
	void resetmanyvalues(keyvalue_t buffer[VECTOR_SIZE][NUM_PARTITIONS], unsigned int size);
	void accumkeysandvalues(keyvalue_t * buffer1, keyvalue_t * buffer2, unsigned int size);
	void accumkeysandvalues(keyvalue_t * buffer1, keyvalue_t * buffer2, keyvalue_t * buffer3, unsigned int size);
	unsigned int checkandforce(unsigned int val, unsigned int limit);
	buffer_type getchunksize(buffer_type buffer_size, alw_travstate_t travstate, unsigned int localoffset);
	unsigned int getpartition(keyvalue_t keyvalue, unsigned int currentLOP, vertex_t upperlimit);
	unsigned int getglobalpartition(keyvalue_t keyvalue, vertex_t upperlimit);
	keyvalue_t getkeyvalue(uint512_dt * buffer, batch_type offset_kvs, batch_type addr);
	keyvalue_t getkeyvalue(uint512_dt * buffer, vector_type idx);
	void setkeyvalue(uint512_dt * buffer, batch_type offset_kvs, batch_type addr, keyvalue_t keyvalue);
	void setkeyvalue(uint512_dt * buffer, vector_type idx, keyvalue_t keyvalue);
	void setkey(uint512_dt * buffer, vector_type idx, keyy_t key);
	void copykeyvalues(keyvalue_t * buffer1, keyvalue_t * buffer2, unsigned int size);
	buffer_type getpartitionwritesz(buffer_type realsize_kvs, buffer_type bramoffset_kvs);
	unsigned int WithinValidRange(unsigned int val1, unsigned int val2);
	void customcalculateoffsets(uint512_dt * buffer, unsigned int size, unsigned int base);
	void calculateoffsets(keyvalue_t * buffer, unsigned int size, unsigned int base, unsigned int skipspacing);
	void calculateunallignedoffsets(keyvalue_t buffer[NUM_PARTITIONS], unsigned int size, unsigned int base, unsigned int skipspacing);
	void calculatemanyunallignedoffsets(keyvalue_t buffer[VECTOR_SIZE][NUM_PARTITIONS], unsigned int size, unsigned int base, unsigned int skipspacing);
	unsigned int get_num_source_partitions(unsigned int currentLOP);
	
	void readglobalstats0(unsigned int enable, uint512_dt * kvdram, keyvalue_t buffer[NUM_PARTITIONS], unsigned int offset_kvs, unsigned int currentLOP, unsigned int sourceORdest);
	
	void saveglobalstats0(unsigned int enable, uint512_dt * kvdram, keyvalue_t buffer[PADDEDDESTBUFFER_SIZE]);
	
	void prepareglobalstats0(unsigned int enable, uint512_dt * kvdram);
	
	void read0(unsigned int enable, uint512_dt * kvdram, keyvalue_t buffer[VECTOR_SIZE][SRCBUFFER_SIZE], batch_type offset_kvs, alw_travstate_t travstate);
	
	void save0(unsigned int enable, uint512_dt * kvdram, uint512_dt buffer[PADDEDDESTBUFFER_SIZE], keyvalue_t * globalcapsule, keyvalue_t localcapsule[NUM_PARTITIONS], batch_type globalbaseaddress_kvs);
	
	void collectglobalstats0(unsigned int enable, keyvalue_t sourcebuffer[VECTOR_SIZE][SRCBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], unsigned int upperlimit);
	
	void prepareglobalstats0(unsigned int enable, keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE]);
	
	void partitionkeyvalues0(unsigned int enable, keyvalue_t sourcebuffer[VECTOR_SIZE][SRCBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t localcapsule[VECTOR_SIZE][NUM_PARTITIONS], unsigned int currentLOP, unsigned int upperlimit);
	
	void combineSetof1stoSetof20_I0(unsigned int enable, keyvalue_t buffer_setof1M[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof1N[PADDEDDESTBUFFER_SIZE], uint128_dt buffer_setof2[PADDEDDESTBUFFER_SIZE], keyvalue_t localcapsuleM[NUM_PARTITIONS], keyvalue_t localcapsuleN[NUM_PARTITIONS], keyvalue_t localcapsuleR[NUM_PARTITIONS]);
	void combineSetof1stoSetof20_I1(unsigned int enable, keyvalue_t buffer_setof1M[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof1N[PADDEDDESTBUFFER_SIZE], uint128_dt buffer_setof2[PADDEDDESTBUFFER_SIZE], keyvalue_t localcapsuleM[NUM_PARTITIONS], keyvalue_t localcapsuleN[NUM_PARTITIONS], keyvalue_t localcapsuleR[NUM_PARTITIONS]);
	void combineSetof1stoSetof20_I2(unsigned int enable, keyvalue_t buffer_setof1M[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof1N[PADDEDDESTBUFFER_SIZE], uint128_dt buffer_setof2[PADDEDDESTBUFFER_SIZE], keyvalue_t localcapsuleM[NUM_PARTITIONS], keyvalue_t localcapsuleN[NUM_PARTITIONS], keyvalue_t localcapsuleR[NUM_PARTITIONS]);
	void combineSetof1stoSetof20_I3(unsigned int enable, keyvalue_t buffer_setof1M[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof1N[PADDEDDESTBUFFER_SIZE], uint128_dt buffer_setof2[PADDEDDESTBUFFER_SIZE], keyvalue_t localcapsuleM[NUM_PARTITIONS], keyvalue_t localcapsuleN[NUM_PARTITIONS], keyvalue_t localcapsuleR[NUM_PARTITIONS]);
	void combineSetof2stoSetof40_I0(unsigned int enable, uint128_dt buffer_setof2M[PADDEDDESTBUFFER_SIZE], uint128_dt buffer_setof2N[PADDEDDESTBUFFER_SIZE], uint256_dt buffer_setof4[PADDEDDESTBUFFER_SIZE], keyvalue_t localcapsuleM[NUM_PARTITIONS], keyvalue_t localcapsuleN[NUM_PARTITIONS], keyvalue_t localcapsuleR[NUM_PARTITIONS]);
	void combineSetof2stoSetof40_I1(unsigned int enable, uint128_dt buffer_setof2M[PADDEDDESTBUFFER_SIZE], uint128_dt buffer_setof2N[PADDEDDESTBUFFER_SIZE], uint256_dt buffer_setof4[PADDEDDESTBUFFER_SIZE], keyvalue_t localcapsuleM[NUM_PARTITIONS], keyvalue_t localcapsuleN[NUM_PARTITIONS], keyvalue_t localcapsuleR[NUM_PARTITIONS]);
	void combineSetof4stoSetof80_I0(unsigned int enable, uint256_dt buffer_setof4M[PADDEDDESTBUFFER_SIZE], uint256_dt buffer_setof4N[PADDEDDESTBUFFER_SIZE], uint512_dt buffer_setof8[PADDEDDESTBUFFER_SIZE], keyvalue_t localcapsuleM[NUM_PARTITIONS], keyvalue_t localcapsuleN[NUM_PARTITIONS], keyvalue_t localcapsuleR[NUM_PARTITIONS]);		
	
	void combineSetof1stoSetof2s0(unsigned int enable, keyvalue_t buffer_setof1[8][PADDEDDESTBUFFER_SIZE], uint128_dt buffer_setof2[4][PADDEDDESTBUFFER_SIZE], keyvalue_t templocalcapsule[14][NUM_PARTITIONS]);
	void combineSetof2stoSetof4s0(unsigned int enable, uint128_dt buffer_setof2[4][PADDEDDESTBUFFER_SIZE], uint256_dt buffer_setof4[2][PADDEDDESTBUFFER_SIZE], keyvalue_t templocalcapsule[14][NUM_PARTITIONS]);
	void combineSetof4stoSetof8s0(unsigned int enable, uint256_dt buffer_setof4[2][PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer[PADDEDDESTBUFFER_SIZE], keyvalue_t templocalcapsule[14][NUM_PARTITIONS]);
	
	void dispatch0(uint512_dt * kvdram, keyvalue_t globalcapsule[NUM_PARTITIONS], alw_config config, alw_sweepparams_t sweepparams, alw_travstate_t travstate);
	
	void topkernel( uint512_dt * sourceAvolume );						
private:
	#ifndef FPGA_IMPL
	actsutility * actsutilityobj;
	#endif 
};
#endif 






