#ifndef ACTSPARTITION_H
#define ACTSPARTITION_H
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
#include "../actsutility/actsutility.h"
#endif
using namespace std;

typedef unsigned int batch_type;
typedef unsigned int buffer_type;
typedef unsigned int partition_type;
typedef unsigned int vector_type;
typedef uint512_dt atp_uint512_dt;

typedef struct {
	unsigned int currentLOP;
	unsigned int upperlimit;
} sweepparams_t;

typedef struct {
	unsigned int topi_kvs;
	unsigned int i_kvs;
	unsigned int begin_kvs;
	unsigned int end_kvs;
	unsigned int skip_kvs;
	unsigned int info;
} atp_travstate_t;

class actspartition {
public:
	actspartition();
	~actspartition();
	
	unsigned int allignhigher_KV(unsigned int val);
	void getglobalparams(uint512_dt * sourcevolume, alw_globalparams_t * globalparams);
	void copykeyandvalues(keyvalue_t * buffer1, keyvalue_t * buffer2, unsigned int size);
	void calculatemanyoffsets(keyvalue_t buffer[NUMSUBWORKERS][NUM_PARTITIONS], unsigned int size);
	void resetkeyandvalues(keyvalue_t * buffer, unsigned int size);
	void resetkeyandvalues(atp_uint512_dt destbuffer[NUMSUBWORKERS][ALW_PADDEDDESTBUFFER_SIZE]);
	void resetmanykeyandvalues(keyvalue_t buffer[NUMSUBWORKERS][NUM_PARTITIONS], unsigned int size);
	unsigned int checkandforce(unsigned int val, unsigned int limit);
	buffer_type getchunksize(buffer_type buffer_size, atp_travstate_t travstate, unsigned int localoffset);
	unsigned int getpartition(keyvalue_t keyvalue, unsigned int currentLOP, vertex_t upperlimit);
	unsigned int getglobalpartition(keyvalue_t keyvalue, vertex_t upperlimit);
	keyvalue_t getkeyvalue(uint512_dt * buffer, batch_type offset, batch_type addr);
	void setkeyvalue(uint512_dt * buffer, batch_type offset, batch_type addr, keyvalue_t keyvalue);

	void read(uint512_dt * kvdram, keyvalue_t * buffer, batch_type offset_kvs, buffer_type size_kvs);
	void save(uint512_dt * kvdram, keyvalue_t * buffer, batch_type offset_kvs, buffer_type size_kvs);

	void readKVS0(uint512_dt * kvdram, atp_uint512_dt buffer[NUMSUBWORKERS][ALW_SRCBUFFER_SIZE], batch_type offset_kvs, atp_travstate_t travstate);
	
	void collectglobalstats0(atp_uint512_dt sourcebuffer[NUMSUBWORKERS][ALW_SRCBUFFER_SIZE], atp_uint512_dt destbuffer[NUMSUBWORKERS][ALW_PADDEDDESTBUFFER_SIZE], sweepparams_t sweepparams, atp_travstate_t travstate);

	void partition0(atp_uint512_dt sourcebuffer[NUMSUBWORKERS][ALW_SRCBUFFER_SIZE], atp_uint512_dt destbuffer[NUMSUBWORKERS][ALW_PADDEDDESTBUFFER_SIZE], keyvalue_t _localcapsule[NUMSUBWORKERS][NUM_PARTITIONS], sweepparams_t sweepparams, atp_travstate_t travstate);								

	void saveKVS0(uint512_dt * kvdram, atp_uint512_dt buffer[NUMSUBWORKERS][ALW_PADDEDDESTBUFFER_SIZE], keyvalue_t * globalcapsule, keyvalue_t localcapsule[NUMSUBWORKERS][NUM_PARTITIONS], batch_type globalbaseaddress_kvs);
	
	void saveglobalstats0(uint512_dt * kvdram, atp_uint512_dt buffer[NUMSUBWORKERS][ALW_PADDEDDESTBUFFER_SIZE], unsigned int offset_kvs, unsigned int size_kvs);
	
	void topkernel( uint512_dt * sourceAvolume );						
private:
	#ifndef FPGA_IMPL
	actsutility * actsutilityobj;
	#endif 
};
#endif 






