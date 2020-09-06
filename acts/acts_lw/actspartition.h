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
	unsigned int collectglobalstatscmd;
	unsigned int collectstatsandpartitioncmd;
	unsigned int reducecmd;
	unsigned int enablereadkvs;
	unsigned int enabledispatch;
	unsigned int enablesavekvs;
	unsigned int enableresetdestbuffer;
	unsigned int enablereadglobalstats;
	unsigned int enablesaveglobalstats;
} alw_config;

class actspartition {
public:
	actspartition();
	~actspartition();
	
	unsigned int subandcap(unsigned int val1, unsigned int val2);
	unsigned int allignhigher_KV(unsigned int val);
	batch_type getskipsize(unsigned int currentLOP, unsigned int sourceordest);
	void getglobalparams(uint512_dt * sourcevolume, globalparams_t * globalparams);
	void copykeyandvalues(keyvalue_t * buffer1, keyvalue_t * buffer2, unsigned int size);
	void resetkeyandvalues(keyvalue_t * buffer, unsigned int size);
	void resetvalues(keyvalue_t * buffer, unsigned int size);
	void resetkeyandvalues(unsigned int enable, atp_uint512_dt destbuffer[NUMSUBWORKERS][PADDEDDESTBUFFER_SIZE]);
	void resetmanykeyandvalues(keyvalue_t buffer[NUMSUBWORKERS][NUM_PARTITIONS], unsigned int size);
	void resetmanyvalues(keyvalue_t buffer[NUMSUBWORKERS][NUM_PARTITIONS], unsigned int size);
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
	void calculateoffsets(keyvalue_t * buffer, unsigned int size, unsigned int base, unsigned int skipspacing);
	void customcalculateoffsets(uint512_dt * buffer, unsigned int size, unsigned int base);
	void calculatemanyoffsets(keyvalue_t buffer[NUMSUBWORKERS][NUM_PARTITIONS], unsigned int size, unsigned int base, unsigned int skipspacing);
	
	bool allowOp(unsigned int i_inloop, buffer_type chunk_size, keyvalue_t keyvalue, unsigned int command);
	keyvalue_t get(unsigned int enable, unsigned int command, uint512_dt * buffer, buffer_type offset_kvs, buffer_type addr);
	unsigned int compute(unsigned int enable, unsigned int command, keyvalue_t keyvalue, unsigned int currentLOP, unsigned int upperlimit, unsigned int partition);
	unsigned int getindex(unsigned int enable, unsigned int command, keyvalue_t localcapsule, unsigned int partition, unsigned int i_inloop, unsigned int ovsize);
	void set(unsigned int enable, unsigned int command, uint512_dt * destbuffer, buffer_type offset_kvs, buffer_type index, keyvalue_t keyvalue, keyvalue_t keyvalue2);
	void update(unsigned int enable, unsigned int command, keyvalue_t * localcapsule);

	void readglobalstats0(unsigned int enable, uint512_dt * kvdram, keyvalue_t buffer[NUM_PARTITIONS], unsigned int offset_kvs, unsigned int currentLOP, unsigned int sourceordest);
	
	void saveglobalstats0(unsigned int enable, uint512_dt * kvdram, atp_uint512_dt buffer[NUMSUBWORKERS][PADDEDDESTBUFFER_SIZE]);
	
	void prepareglobalstats0(unsigned int enable, uint512_dt * kvdram);
	
	void readKVS0(unsigned int enable, uint512_dt * kvdram, atp_uint512_dt buffer[NUMSUBWORKERS][SRCBUFFER_SIZE], batch_type offset_kvs, alw_travstate_t travstate);
	
	void execute0(unsigned int enable, unsigned int * enables, unsigned int command, atp_uint512_dt sourcebuffer[NUMSUBWORKERS][SRCBUFFER_SIZE], atp_uint512_dt destbuffer[NUMSUBWORKERS][PADDEDDESTBUFFER_SIZE], keyvalue_t _localcapsule[NUMSUBWORKERS][NUM_PARTITIONS], buffer_type chunk_size[NUMSUBWORKERS], alw_sweepparams_t sweepparams, partition_type partition);
	
	void launchexecute0(unsigned int enable, unsigned int collectglobalstatscmd, unsigned int collectstatsandpartitioncmd, atp_uint512_dt sourcebuffer[NUMSUBWORKERS][SRCBUFFER_SIZE], atp_uint512_dt destbuffer[NUMSUBWORKERS][PADDEDDESTBUFFER_SIZE], keyvalue_t _localcapsule[NUMSUBWORKERS][NUM_PARTITIONS], alw_sweepparams_t sweepparams, alw_travstate_t travstate);			
	
	void saveKVS0(unsigned int enable, uint512_dt * kvdram, atp_uint512_dt buffer[NUMSUBWORKERS][PADDEDDESTBUFFER_SIZE], keyvalue_t * globalcapsule, keyvalue_t localcapsule[NUMSUBWORKERS][NUM_PARTITIONS], batch_type globalbaseaddress_kvs);
	
	void dispatch0(uint512_dt * kvdram, atp_uint512_dt sourcebuffer[NUMSUBWORKERS][SRCBUFFER_SIZE], atp_uint512_dt destbuffer[NUMSUBWORKERS][PADDEDDESTBUFFER_SIZE], keyvalue_t localcapsule[NUMSUBWORKERS][NUM_PARTITIONS], keyvalue_t globalcapsule[NUM_PARTITIONS], alw_config config, alw_sweepparams_t sweepparams, alw_travstate_t travstate);
	
	void topkernel( uint512_dt * sourceAvolume );						
private:
	#ifndef FPGA_IMPL
	actsutility * actsutilityobj;
	#endif 
};
#endif 






