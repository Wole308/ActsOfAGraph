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
#if NUMPIPELINES==2
#define PP0
#define PP1
#endif 
#if NUMPIPELINES==3
#define PP0
#define PP1
#define PP2
#endif 

class actslw {
public:
	actslw();
	~actslw();
	
	unsigned int subandcap(unsigned int val1, unsigned int val2);
	unsigned int lowercapto(unsigned int val, unsigned int capval);
	unsigned int allignhigher_KV(unsigned int val);
	unsigned int allignby2higher_KV(unsigned int val);
	batch_type getskipsize(unsigned int currentLOP, unsigned int sourceORdest);
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
	buffer_type getchunksize(buffer_type buffer_size, travstate_t travstate, unsigned int localoffset);
	unsigned int getpartition(keyvalue_t keyvalue, unsigned int currentLOP, vertex_t upperlimit);
	unsigned int getglobalpartition(keyvalue_t keyvalue, vertex_t upperlimit);
	unsigned int reducefunc(keyy_t vid, value_t value, value_t edgeval, unsigned int GraphIter, unsigned int GraphAlgo);
	void copykeyvalues(keyvalue_t * buffer1, keyvalue_t * buffer2, unsigned int size);
	buffer_type getpartitionwritesz(buffer_type realsize_kvs, buffer_type bramoffset_kvs);
	unsigned int withinvalidrange(unsigned int val1, unsigned int val2);
	void calculateoffsets(keyvalue_t * buffer, unsigned int size, unsigned int base, unsigned int skipspacing);
	void calculateoffsets_allignby2(keyvalue_t * buffer, unsigned int size, unsigned int base, unsigned int skipspacing);
	void calculateunallignedoffsets(keyvalue_t buffer[NUM_PARTITIONS], unsigned int size, unsigned int base, unsigned int skipspacing);
	void calculatemanyoffsets(keyvalue_t buffer[VECTOR_SIZE][NUM_PARTITIONS], unsigned int size, unsigned int base, unsigned int skipspacing);
	void calculatemanyunallignedoffsets(keyvalue_t buffer[VECTOR_SIZE][NUM_PARTITIONS], unsigned int size, unsigned int base, unsigned int skipspacing);
	unsigned int get_num_source_partitions(unsigned int currentLOP);
	globalparams_t getglobalparams(uint512_dt * kvdram);
	sweepparams_t getsweepparams(globalparams_t globalparams, unsigned int currentLOP, unsigned int source_partition);
	config_t getconfig(unsigned int currentLOP);
	travstate_t gettravstate(uint512_dt * kvdram, globalparams_t globalparams, config_t config, unsigned int currentLOP, unsigned int source_partition, unsigned int num_source_partitions);
	
	void readglobalstats0(unsigned int enable, uint512_dt * kvdram, keyvalue_t buffer[NUM_PARTITIONS], unsigned int offset_kvs, unsigned int currentLOP, unsigned int sourceORdest);
	
	void saveglobalstats0(unsigned int enable, uint512_dt * kvdram, keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE]);
	
	void updateglobalstats0(unsigned int enable, uint512_dt * kvdram, keyvalue_t buffer[NUM_PARTITIONS], unsigned int offset_kvs, unsigned int currentLOP, unsigned int sourceORdest);
	
	void prepareglobalstats0(unsigned int enable, uint512_dt * kvdram);
	
	void readkeyvalues0(unsigned int enable, uint512_dt * kvdram, keyvalue_t buffer[VECTOR_SIZE][SRCBUFFER_SIZE], batch_type offset_kvs, travstate_t travstate);
	
	void readvertices0(unsigned int enable, uint512_dt * kvdram, keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], batch_type offset_kvs);
	
	void savekeyvalues0(unsigned int enable, uint512_dt * kvdram, uint512_dt buffer[PADDEDDESTBUFFER_SIZE], keyvalue_t * globalcapsule, keyvalue_t localcapsule[NUM_PARTITIONS], batch_type globalbaseaddress_kvs);
	
	void savevertices0(unsigned int enable, uint512_dt * kvdram, keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], batch_type offset_kvs);
	
	void collectglobalstats0(unsigned int enable, keyvalue_t sourcebuffer[VECTOR_SIZE][SRCBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], unsigned int upperlimit);
	
	void prepareglobalstats0(unsigned int enable, keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE]);
	
	void partitionkeyvalues0(unsigned int enable, keyvalue_t sourcebuffer[VECTOR_SIZE][SRCBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t localcapsule[VECTOR_SIZE][NUM_PARTITIONS], unsigned int currentLOP, unsigned int upperlimit, travstate_t travstate);
	
	void reduce0(unsigned int enable, keyvalue_t sourcebuffer[VECTOR_SIZE][SRCBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], unsigned int upperlimit, unsigned int GraphIter, unsigned int GraphAlgo);
	
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
	
	void dispatch0(uint512_dt * kvdram);
	
	void topkernel( uint512_dt * sourceAvolume );						
private:
	#ifndef FPGA_IMPL
	actsutility * actsutilityobj;
	#endif 
};
#endif 






