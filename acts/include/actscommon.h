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

typedef unsigned int batch_type;
typedef unsigned int buffer_type;
// typedef unsigned int partition_type;
// typedef unsigned int vector_type;

// typedef short buffer_type; // REMOVEME.?
typedef short partition_type;
typedef short vector_type;

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
	unsigned int skip_kvs;
	unsigned int info;
} travstate_t;

typedef struct {
	unsigned int enablecollectglobalstats;
	unsigned int enablepartition;
	unsigned int enablereduce;
} config_t;

typedef struct {
	unsigned int runkernelcommand;
	unsigned int processcommand;
	unsigned int collectstatscommand;
	unsigned int partitioncommand;
	unsigned int reducecommand;
	unsigned int baseaddr_destkvs_kvs;
	unsigned int iteration_idx;
	unsigned int ssdpartitonid;
	unsigned int vsize;
	unsigned int treedepth;
	unsigned int LLOPnumpartitions;
	unsigned int paddedkvdatabatchszkvs;
	unsigned int GraphIter;
	unsigned int GraphAlgo;
	unsigned int IterCount;
	unsigned int baseoffset_kvs;
	unsigned int basesize_kvs;
	unsigned int steps;
	unsigned int finalnumpartitions;
	unsigned int batchsize;
	unsigned int runsize;
	unsigned int nextbatchoffset;
	unsigned int capsulebatchsz_kvs;
	unsigned int vbegin;
	unsigned int statsalreadycollected;
} globalparams_t;
#endif 







