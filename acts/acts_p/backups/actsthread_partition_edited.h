#ifndef ACTSTHREAD_PARTITION_H
#define ACTSTHREAD_PARTITION_H
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
using namespace std;

#define ATP_PADDEDDESTBUFFER_SIZE 512
#define ATP_SRCBUFFER_SIZE (ATP_PADDEDDESTBUFFER_SIZE - NUM_PARTITIONS)

typedef unsigned int batch_type;
typedef unsigned int buffer_type;
typedef unsigned int partition_type;
typedef unsigned int vector_type;

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

class actsthread_partition {
public:
	actsthread_partition();
	~actsthread_partition();
	
	unsigned int checkandforce(unsigned int val, unsigned int limit);
	unsigned int allignhigher_KV(unsigned int val);
	buffer_type getchunksize(buffer_type buffer_size, atp_travstate_t travstate, unsigned int localoffset);
	unsigned int getpartition(keyvalue_t keyvalue, unsigned int currentLOP, vertex_t upperlimit);

	void read(keyvalue_t * kvdram, keyvalue_t * buffer, batch_type offset_kvs, buffer_type size_kvs);
	void save(keyvalue_t * kvdram, keyvalue_t * buffer, batch_type offset_kvs, buffer_type size_kvs);

	void readKVS(uint512_dt * kvdram, uint512_dt * buffer, batch_type offset_kvs, atp_travstate_t travstate);
	void partition(uint512_dt * sourcebuffer, uint512_dt * destbuffer, keyvalue_t * localcapsule, sweepparams_t sweepparams, atp_travstate_t travstate);
	void saveKVS(uint512_dt * kvdram, uint512_dt * buffer, keyvalue_t * globalcapsule, keyvalue_t * localcapsule, batch_type globalbaseaddress_kvs);

	void topkernel(uint512_dt * sourceAvolume, uint512_dt * sourceBvolume, uint512_dt * sourceCvolume, keyvalue_t * statsA, keyvalue_t * statsB, keyvalue_t * statsC);
private:
	#ifndef FPGA_IMPL
	actsutility * actsutilityobj;
	#endif 
};
#endif 







