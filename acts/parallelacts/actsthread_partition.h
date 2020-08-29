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

class actsthread_partition {
public:
	actsthread_partition();
	~actsthread_partition();
	
	unsigned int allignhigher_KV(unsigned int val);
	unsigned int checkandforce(unsigned int val, unsigned int limit);
	unsigned int getpartition(keyvalue_t keyvalue, unsigned int currentLOP, vertex_t upperlimit);

	void read(keyvalue_t * kvdram, keyvalue_t * buffer, batch_type offset_kvs, buffer_type size_kvs);
	void save(keyvalue_t * kvdram, keyvalue_t * buffer, batch_type offset_kvs, buffer_type size_kvs);

	void read(uint512_dt * kvdram, uint512_dt * buffer, batch_type offset_kvs, buffer_type size_kvs);
	void partition(uint512_dt * sourcebuffer, uint512_dt * destbuffer, keyvalue_t * localcapsule, buffer_type size_kvs, sweepparams_t sweepparams);
	void save(uint512_dt * kvdram, uint512_dt * buffer, keyvalue_t * globalcapsule, keyvalue_t * localcapsule, batch_type globalbaseaddress_kvs);

	void topkernel(uint512_dt * sourcevolume, uint512_dt * targetvolume, keyvalue_t * stats);
private:
	#ifndef FPGA_IMPL
	actsutility * actsutilityobj;
	#endif 
};
#endif 







