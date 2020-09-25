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

#define USEBRAMFORGLOBALSTATS

#define ENABLEPARTITIONANDREDUCE
#define ENABLEMERGER

class actslw {
public:
	actslw();
	~actslw();
	
	// kernel utility functions
	buffer_type allignhigher_KV(buffer_type val);
	batch_type getskipsize(step_type currentLOP, bool_type sourceORdest);
	void copykeyandvalues(keyvalue_t * buffer1, keyvalue_t * buffer2, buffer_type size);
	void resetkeyandvalues(skeyvalue_t * buffer, buffer_type size);
	void resetvalues(keyvalue_t * buffer, buffer_type size);
	void resetkeyandvalues(bool_type enable, uint512_dt destbuffer[PADDEDDESTBUFFER_SIZE]);
	void resetmanykeyandvalues(skeyvalue_t buffer[VECTOR_SIZE][NUM_PARTITIONS], buffer_type size);
	void resetmanykeyandvalues(keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], buffer_type size);
	void resetmanyvalues(skeyvalue_t buffer[VECTOR_SIZE][NUM_PARTITIONS], buffer_type size);
	void accumkeysandvalues(keyvalue_t * buffer1, keyvalue_t * buffer2, buffer_type size);
	void accumkeysandvalues(keyvalue_t * buffer1, keyvalue_t * buffer2, keyvalue_t * buffer3, buffer_type size);
	unsigned int checkandforce(unsigned int val, unsigned int limit);
	buffer_type getchunksize(buffer_type buffer_size, travstate_t travstate, buffer_type localoffset);
	partition_type getpartition(keyvalue_t keyvalue, step_type currentLOP, vertex_t upperlimit);
	buffer_type getglobalpartition(keyvalue_t keyvalue, vertex_t upperlimit);
	unsigned int reducefunc(keyy_t vid, value_t value, value_t edgeval, unsigned int GraphIter, unsigned int GraphAlgo);
	void copykeyvalues(keyvalue_t * buffer1, keyvalue_t * buffer2, buffer_type size);
	buffer_type getpartitionwritesz(buffer_type realsize_kvs, buffer_type bramoffset_kvs);
	unsigned int withinvalidrange(buffer_type val1, buffer_type val2);
	void calculateoffsets(keyvalue_t * buffer, buffer_type size, buffer_type base, buffer_type skipspacing);
	void calculateunallignedoffsets(skeyvalue_t buffer[NUM_PARTITIONS], buffer_type size, buffer_type base, buffer_type skipspacing);
	void calculatemanyoffsets(skeyvalue_t buffer[VECTOR_SIZE][NUM_PARTITIONS], buffer_type size, buffer_type base, buffer_type skipspacing);
	void calculatemanyunallignedoffsets(skeyvalue_t buffer[VECTOR_SIZE][NUM_PARTITIONS], buffer_type size, buffer_type base, buffer_type skipspacing);
	batch_type get_num_source_partitions(step_type currentLOP);
	globalparams_t getglobalparams(uint512_dt * kvdram);
	sweepparams_t getsweepparams(globalparams_t globalparams, step_type currentLOP, batch_type source_partition);
	config_t getconfig(step_type currentLOP);
	travstate_t gettravstate(uint512_dt * kvdram, keyvalue_t globalstatsbuffer[NUMLASTLEVELPARTITIONS], globalparams_t globalparams, config_t config, step_type currentLOP, batch_type source_partition, batch_type num_source_partitions);
	
	// functions - dispatch
	void readglobalstats0(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[NUMLASTLEVELPARTITIONS], keyvalue_t buffer[NUM_PARTITIONS], buffer_type offset_kvs, step_type currentLOP, bool_type sourceORdest);
	
	void saveglobalstats0(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[NUMLASTLEVELPARTITIONS], keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE]);
	
	void updateglobalstats0(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[NUMLASTLEVELPARTITIONS], keyvalue_t buffer[NUM_PARTITIONS], buffer_type offset_kvs, step_type currentLOP, bool_type sourceORdest);
	
	void prepareglobalstats0(bool_type enable, uint512_dt * kvdram);
	
	void readkeyvalues0(bool_type enable, uint512_dt * kvdram, keyvalue_t buffer[VECTOR_SIZE][SRCBUFFER_SIZE], batch_type offset_kvs, travstate_t travstate);
	
	void readvertices0(bool_type enable, uint512_dt * kvdram, keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], batch_type offset_kvs);
	
	void savekeyvalues0(bool_type enable, uint512_dt * kvdram, uint512_dt buffer[PADDEDDESTBUFFER_SIZE], keyvalue_t * globalcapsule, skeyvalue_t localcapsule[NUM_PARTITIONS], batch_type globalbaseaddress_kvs);
	
	void savevertices0(bool_type enable, uint512_dt * kvdram, keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], batch_type offset_kvs);
	
	void collectglobalstats0(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][SRCBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], vertex_t upperlimit);
	
	void prepareglobalstats0(bool_type enable, keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE]);
	
	void partitionkeyvalues0(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][SRCBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t localcapsule[VECTOR_SIZE][NUM_PARTITIONS], step_type currentLOP, vertex_t upperlimit, travstate_t travstate);
	
	void reduce0(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][SRCBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], vertex_t upperlimit, unsigned int GraphIter, unsigned int GraphAlgo, travstate_t travstate);
	
	void combineSetof1stoSetof20_I0(bool_type enable, keyvalue_t buffer_setof1M[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof1N[PADDEDDESTBUFFER_SIZE], uint128_dt buffer_setof2[PADDEDDESTBUFFER_SIZE], skeyvalue_t localcapsuleM[NUM_PARTITIONS], skeyvalue_t localcapsuleN[NUM_PARTITIONS], skeyvalue_t localcapsuleR[NUM_PARTITIONS]);
	void combineSetof1stoSetof20_I1(bool_type enable, keyvalue_t buffer_setof1M[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof1N[PADDEDDESTBUFFER_SIZE], uint128_dt buffer_setof2[PADDEDDESTBUFFER_SIZE], skeyvalue_t localcapsuleM[NUM_PARTITIONS], skeyvalue_t localcapsuleN[NUM_PARTITIONS], skeyvalue_t localcapsuleR[NUM_PARTITIONS]);
	void combineSetof1stoSetof20_I2(bool_type enable, keyvalue_t buffer_setof1M[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof1N[PADDEDDESTBUFFER_SIZE], uint128_dt buffer_setof2[PADDEDDESTBUFFER_SIZE], skeyvalue_t localcapsuleM[NUM_PARTITIONS], skeyvalue_t localcapsuleN[NUM_PARTITIONS], skeyvalue_t localcapsuleR[NUM_PARTITIONS]);
	void combineSetof1stoSetof20_I3(bool_type enable, keyvalue_t buffer_setof1M[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof1N[PADDEDDESTBUFFER_SIZE], uint128_dt buffer_setof2[PADDEDDESTBUFFER_SIZE], skeyvalue_t localcapsuleM[NUM_PARTITIONS], skeyvalue_t localcapsuleN[NUM_PARTITIONS], skeyvalue_t localcapsuleR[NUM_PARTITIONS]);
	void combineSetof2stoSetof40_I0(bool_type enable, uint128_dt buffer_setof2M[PADDEDDESTBUFFER_SIZE], uint128_dt buffer_setof2N[PADDEDDESTBUFFER_SIZE], uint256_dt buffer_setof4[PADDEDDESTBUFFER_SIZE], skeyvalue_t localcapsuleM[NUM_PARTITIONS], skeyvalue_t localcapsuleN[NUM_PARTITIONS], skeyvalue_t localcapsuleR[NUM_PARTITIONS]);
	void combineSetof2stoSetof40_I1(bool_type enable, uint128_dt buffer_setof2M[PADDEDDESTBUFFER_SIZE], uint128_dt buffer_setof2N[PADDEDDESTBUFFER_SIZE], uint256_dt buffer_setof4[PADDEDDESTBUFFER_SIZE], skeyvalue_t localcapsuleM[NUM_PARTITIONS], skeyvalue_t localcapsuleN[NUM_PARTITIONS], skeyvalue_t localcapsuleR[NUM_PARTITIONS]);
	void combineSetof4stoSetof80_I0(bool_type enable, uint256_dt buffer_setof4M[PADDEDDESTBUFFER_SIZE], uint256_dt buffer_setof4N[PADDEDDESTBUFFER_SIZE], uint512_dt buffer_setof8[PADDEDDESTBUFFER_SIZE], skeyvalue_t localcapsuleM[NUM_PARTITIONS], skeyvalue_t localcapsuleN[NUM_PARTITIONS], skeyvalue_t localcapsuleR[NUM_PARTITIONS]);		
	
	// functions - merge 
	void readmany(unsigned int offset_kvs  ,uint512_dt * kvdram0 ,keyvalue_t source0buffer[VECTOR_SIZE][SRCBUFFER_SIZE]  ,uint512_dt * kvdram1 ,keyvalue_t source1buffer[VECTOR_SIZE][SRCBUFFER_SIZE]  ,uint512_dt * kvdram2 ,keyvalue_t source2buffer[VECTOR_SIZE][SRCBUFFER_SIZE]  ,uint512_dt * kvdram3 ,keyvalue_t source3buffer[VECTOR_SIZE][SRCBUFFER_SIZE]  ,uint512_dt * kvdram4 ,keyvalue_t source4buffer[VECTOR_SIZE][SRCBUFFER_SIZE]  ,uint512_dt * kvdram5 ,keyvalue_t source5buffer[VECTOR_SIZE][SRCBUFFER_SIZE]  ,uint512_dt * kvdram6 ,keyvalue_t source6buffer[VECTOR_SIZE][SRCBUFFER_SIZE]  ,uint512_dt * kvdram7 ,keyvalue_t source7buffer[VECTOR_SIZE][SRCBUFFER_SIZE]  ,uint512_dt * kvdram8 ,keyvalue_t source8buffer[VECTOR_SIZE][SRCBUFFER_SIZE]  ,uint512_dt * kvdram9 ,keyvalue_t source9buffer[VECTOR_SIZE][SRCBUFFER_SIZE]  ,uint512_dt * kvdram10 ,keyvalue_t source10buffer[VECTOR_SIZE][SRCBUFFER_SIZE]  ,uint512_dt * kvdram11 ,keyvalue_t source11buffer[VECTOR_SIZE][SRCBUFFER_SIZE]  ,uint512_dt * kvdram12 ,keyvalue_t source12buffer[VECTOR_SIZE][SRCBUFFER_SIZE]  ,uint512_dt * kvdram13 ,keyvalue_t source13buffer[VECTOR_SIZE][SRCBUFFER_SIZE]  ,uint512_dt * kvdram14 ,keyvalue_t source14buffer[VECTOR_SIZE][SRCBUFFER_SIZE]  ,uint512_dt * kvdram15 ,keyvalue_t source15buffer[VECTOR_SIZE][SRCBUFFER_SIZE]  );
			
	void saveone(unsigned int offset_kvs, uint512_dt * kvdram, uint512_dt sourcebuffer[PADDEDDESTBUFFER_SIZE]);

	keyvalue_t mergefunc(keyvalue_t keyvalue1, keyvalue_t keyvalue2, keyvalue_t keyvalue3, keyvalue_t keyvalue4);
	
	void merge_I0(unsigned int enable, keyvalue_t source1buffer[8][SRCBUFFER_SIZE], keyvalue_t source2buffer[8][SRCBUFFER_SIZE], keyvalue_t source3buffer[8][SRCBUFFER_SIZE], keyvalue_t source4buffer[8][SRCBUFFER_SIZE], keyvalue_t destbuffer[8][PADDEDDESTBUFFER_SIZE]);
	void merge_I1(unsigned int enable, keyvalue_t source1buffer[8][SRCBUFFER_SIZE], keyvalue_t source2buffer[8][SRCBUFFER_SIZE], keyvalue_t source3buffer[8][SRCBUFFER_SIZE], keyvalue_t source4buffer[8][SRCBUFFER_SIZE], keyvalue_t destbuffer[8][PADDEDDESTBUFFER_SIZE]);
	void merge_I2(unsigned int enable, keyvalue_t source1buffer[8][SRCBUFFER_SIZE], keyvalue_t source2buffer[8][SRCBUFFER_SIZE], keyvalue_t source3buffer[8][SRCBUFFER_SIZE], keyvalue_t source4buffer[8][SRCBUFFER_SIZE], keyvalue_t destbuffer[8][PADDEDDESTBUFFER_SIZE]);
	void merge_I3(unsigned int enable, keyvalue_t source1buffer[8][SRCBUFFER_SIZE], keyvalue_t source2buffer[8][SRCBUFFER_SIZE], keyvalue_t source3buffer[8][SRCBUFFER_SIZE], keyvalue_t source4buffer[8][SRCBUFFER_SIZE], keyvalue_t destbuffer[8][PADDEDDESTBUFFER_SIZE]);

	void merge_I0(unsigned int enable, keyvalue_t source1buffer[8][PADDEDDESTBUFFER_SIZE], keyvalue_t source2buffer[8][PADDEDDESTBUFFER_SIZE], keyvalue_t source3buffer[8][PADDEDDESTBUFFER_SIZE], keyvalue_t source4buffer[8][PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer[PADDEDDESTBUFFER_SIZE]);

	// group functions 
	void combineSetof1stoSetof2s0(bool_type enable, keyvalue_t buffer_setof1[8][PADDEDDESTBUFFER_SIZE], uint128_dt buffer_setof2[4][PADDEDDESTBUFFER_SIZE], skeyvalue_t templocalcapsule_so1[8][NUM_PARTITIONS], skeyvalue_t templocalcapsule_so2[4][NUM_PARTITIONS]);
	void combineSetof2stoSetof4s0(bool_type enable, uint128_dt buffer_setof2[4][PADDEDDESTBUFFER_SIZE], uint256_dt buffer_setof4[2][PADDEDDESTBUFFER_SIZE], skeyvalue_t templocalcapsule_so2[4][NUM_PARTITIONS], skeyvalue_t templocalcapsule_so4[2][NUM_PARTITIONS]);
	void combineSetof4stoSetof8s0(bool_type enable, uint256_dt buffer_setof4[2][PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer[PADDEDDESTBUFFER_SIZE], skeyvalue_t templocalcapsule_so4[2][NUM_PARTITIONS], skeyvalue_t templocalcapsule_so8[NUM_PARTITIONS]);
	
	void dispatch0(uint512_dt * kvdram, keyvalue_t sourcebuffer[VECTOR_SIZE][SRCBUFFER_SIZE], keyvalue_t buffer_setof1[8][PADDEDDESTBUFFER_SIZE], uint128_dt buffer_setof2[4][PADDEDDESTBUFFER_SIZE], uint256_dt buffer_setof4[2][PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer[PADDEDDESTBUFFER_SIZE]);
	
	void topkernel( uint512_dt * sourceAvolume );						
private:
	#ifndef FPGA_IMPL
	actsutility * actsutilityobj;
	#endif 
};
#endif 






