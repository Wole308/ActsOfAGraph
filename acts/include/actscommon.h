#ifndef ACTSCOMMON_H
#define ACTSCOMMON_H
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
#include "../../include/config_params.h"
#include "../../include/common.h"
#ifdef ACTSMODEL
#include "../../acts/include/acts_common.h"
#endif 
#ifdef ACTSMODEL_LW
#include "../../acts/include/actslw_common.h"
#endif 
using namespace std;

#ifdef FPGA_IMPL
typedef unsigned int batch_type;
typedef ap_uint<13> buffer_type;
typedef ap_uint<8> partition_type;
typedef ap_uint<8> vector_type;
typedef ap_uint<8> step_type;
typedef ap_uint<8> bool_type;
typedef unsigned int analysis_type;
#else 
typedef unsigned int batch_type;
typedef unsigned int buffer_type;
typedef unsigned int partition_type;
typedef unsigned int vector_type;
typedef unsigned int step_type;
typedef unsigned int bool_type;
typedef unsigned int analysis_type;
#endif

typedef struct {
	unsigned int key;
	unsigned int value;
	// short key;
	// short value;
} skeyvalue_t;

typedef struct {
	unsigned int currentLOP;
	unsigned int upperlimit;
	unsigned int source_partition;
	unsigned int worksourcebaseaddress_kvs;
	unsigned int workdestbaseaddress_kvs;
} sweepparams_t;

typedef struct {
	unsigned int topi_kvs;
	unsigned int i_kvs;
	unsigned int begin_kvs;
	unsigned int end_kvs;
	unsigned int size_kvs;
	unsigned int skip_kvs;
	unsigned int info;
} travstate_t;

typedef struct {
	unsigned int enablecollectglobalstats;
	unsigned int enablepartition;
	unsigned int enablereduce;
	unsigned int enableprocessedges;
} config_t;

typedef struct {
	unsigned int command;
	unsigned int runkernelcommand;
	unsigned int processcommand;
	unsigned int collectstatscommand;
	unsigned int partitioncommand;
	unsigned int reducecommand;
	unsigned int finalnumpartitions;
	unsigned int treedepthid;
	unsigned int ssdpartitionid;
	unsigned int srcvoffset;
	unsigned int srcvsize;
	unsigned int srcvsize_kvs;
	unsigned int destvoffset;
	unsigned int beginvid;
	unsigned int beginkey;
	unsigned int beginvalue;
	unsigned int treedepth;
	unsigned int LLOPnumpartitions;
	unsigned int batchsize;
	unsigned int runsize;
	unsigned int runsize_kvs;
	unsigned int nextbatchoffset;
	unsigned int GraphIter;
	unsigned int GraphAlgo;
	unsigned int statsalreadycollected;
	unsigned int groupid;
	unsigned int beginLOP;
	unsigned int endLOP;
	unsigned int numLOPs;
	unsigned int batch_range;
	unsigned int batch_range_kvs;
	unsigned int batch_range_pow;
	unsigned int applyvertexbuffersz;
	unsigned int applyvertexbuffersz_kvs;
	unsigned int baseaddr_destkvs_kvs;
} globalparams_t;

typedef struct {
	unsigned int baseaddr_worksourcekvs_kvs;
	unsigned int baseaddr_workdestkvs_kvs;
	unsigned int baseaddr_capsule_kvs;
	unsigned int baseaddr_stats;
	unsigned int num_source_partitions;
	unsigned int nextsourceoffset_partition;
	unsigned int nextdestoffset_partition;
	unsigned int nextsourceoffset_kv;
	unsigned int nextdestoffset_kv;
	unsigned int sourceskipsize_partition;
	unsigned int destskipsize_partition;
	unsigned int sourceskipsize_kv;
	unsigned int destskipsize_kv;
	unsigned int destpartitionrange;
	unsigned int currentLOP;
	unsigned int upperlimit;
} clopparams_t;

typedef struct {
	unsigned int begin_kvs;
	unsigned int size_kvs;
	unsigned int step_kvs;
	unsigned int end_kvs;
} partitionparams_t;

typedef struct {
	unsigned int globaloffset;
	unsigned int localoffset;
} offset_t;
#endif 







