#ifndef ENIGMA_H
#define ENIGMA_H
#include "../src/common.h"
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
#ifdef FPGA_IMPL
#include <ap_int.h>
#endif
using namespace std;

#define _LDEBUGMODE_HEADER _DEBUGMODE_HEADER

#define BUFFER_PADDING NUM_PARTITIONS
#define BUFFER_SIZE 1024
#define PADDEDBUFFER_SIZE (BUFFER_SIZE + BUFFER_PADDING)

typedef unsigned int batch_type;
typedef unsigned int buffer_type;
typedef unsigned int partition_type;
typedef unsigned int code_type;
typedef unsigned int smallint_type;
typedef unsigned int vector_type;
typedef unsigned int code_type;

typedef struct {
	unsigned int coffset;
	unsigned int size;
} coffset_t;

typedef struct {
	unsigned int baseoffset_kvs;
	unsigned int basesize_kvs;
} workoffset_t;

typedef struct {
	unsigned int partitionmodule;
	unsigned int mergemodule;
} runcmd_t;

typedef struct {
	unsigned int processcommand;
	unsigned int partitioncommand;
	unsigned int reducecommand;
	unsigned int baseaddr_destkvs_kvs;
	unsigned int iteration_idx;
	unsigned int ssdpartitonid;
	unsigned int vsize;
	unsigned int treedepth;
	unsigned int LLOPnumpartitions;
	unsigned int paddedkvdatabatchszkvs;
	unsigned int IterCount;
	unsigned int baseoffset_kvs;
	unsigned int basesize_kvs;
	unsigned int steps;
	unsigned int finalnumpartitions;
	unsigned int paddedkvdatabatchsz_kvs;
	unsigned int capsulebatchsz_kvs;
	unsigned int vbegin;
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
	unsigned int i_kvs;
	unsigned int begin_kvs;
	unsigned int iend_kvs;
	unsigned int skip_kvs;
} travstate_t;

#ifndef FPGA_IMPL
class enigma {
public:
	enigma();
	~enigma();
	#ifdef _LDEBUGMODE_HEADER
	void checkoutofbounds(string message, unsigned int data, unsigned int upper_bound);
	void checkforequal(string message, unsigned int data1, unsigned int data2);
	void print1(string messagea, unsigned int dataa);
	void print4(string messagea, string messageb, string messagec, string messaged, unsigned int dataa, unsigned int datab, unsigned int datac, unsigned int datad);
	void print5(string messagea, string messageb, string messagec, string messaged, string messagee, unsigned int dataa, unsigned int datab, unsigned int datac, unsigned int datad, unsigned int datae);
	void print6(string messagea, string messageb, string messagec, string messaged, string messagee, string messagef, unsigned int dataa, unsigned int datab, unsigned int datac, unsigned int datad, unsigned int datae, unsigned int datef);
	void printkeyvalues(string message, keyvalue_t * keyvalues, unsigned int size);
	void printkeyvalues(string message, keyvalue_t * keyvalues1, keyvalue_t * keyvalues2, unsigned int size);
	void printkeyvalues(string message, uint512_dt * keyvalues, unsigned int size_kvs);
	void getvaluecount(string message, keyvalue_t * keyvalues, unsigned int size);
	void printparameters();
	#endif
	#ifdef _DEBUGMODE_STATS
	void printglobalvars();
	void clearglobalvars();
	void globalstats_countkvstatsread(unsigned int count);
	void globalstats_countkvstatscollected(unsigned int count);
	void globalstats_countkvstatswritten(unsigned int count);
	void globalstats_countkvsread(unsigned int count);
	void globalstats_countkvspartitionswritten(unsigned int count);
	void globalstats_countkvspartitioned(unsigned int count);
	void globalstats_countkvsreduced(unsigned int count);
	void globalstats_countkvsreducewritten(unsigned int count);
	#endif
	
	unsigned int allignhigher_KV(unsigned int val);
	unsigned int allignlower_KV(unsigned int val);
	unsigned int ceildiv(unsigned int val1, unsigned int val2);
	unsigned int max(unsigned int val1, unsigned int val2);
	unsigned int min(unsigned int A, unsigned int B);
	unsigned int hsub(unsigned int A, unsigned int B);

	unsigned int getpartition(keyvalue_t keyvalue, unsigned int currentLOP, vertex_t upperlimit);
	keyvalue_t getkeyvalue(uint512_dt buffer[BUFFER_SIZE], batch_type addr, batch_type maxaddr_kvs);
	void setkeyvalue(uint512_dt buffer[BUFFER_SIZE], batch_type addr, keyvalue_t keyvalue, batch_type maxaddr_kvs);
	
	buffer_type getactivesize(buffer_type i, batch_type i_end, buffer_type buffersize);
	batch_type getAddr(batch_type addr);
	batch_type getstatsAddr(batch_type addr);
	batch_type getmessagesAddr(batch_type addr);
	
	unsigned int getsourceskipsize(unsigned int currentLOP, unsigned int param);
	unsigned int getdestskipsize(unsigned int currentLOP, unsigned int param);

	void loaddeststats(keyvalue_t * kvstats, keyvalue_t buffer[NUM_PARTITIONS], unsigned int partitionoffset, unsigned int currentLOP, vertex_t LLOPnumpartitions);
	void storedeststats(keyvalue_t * kvstats, keyvalue_t buffer[NUM_PARTITIONS], unsigned int partitionoffset, unsigned int currentLOP, vertex_t LLOPnumpartitions);

	unsigned int get_num_source_partitions(unsigned int currentLOP);
	
	void prepareoffsets(buffer_type sizes[NUM_PARTITIONS], buffer_type offsets[NUM_PARTITIONS]);
	void preparecapsule(keyvalue_t capsule[NUM_PARTITIONS]);
	void preparecapsules( keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS], keyvalue_t capsule4[NUM_PARTITIONS], keyvalue_t capsule5[NUM_PARTITIONS], keyvalue_t capsule6[NUM_PARTITIONS], keyvalue_t capsule7[NUM_PARTITIONS]);
		
	void loadclopparams(globalparams_t globalparams, clopparams_t * llopparams, unsigned int currentLOP);
	
	value_t reducefunc(keyy_t vid, value_t value, value_t edgeval, unsigned int IterCount);
	
	void generatepartitions0(uint512_dt * kvsourcedram, uint512_dt * kvdestdram, keyvalue_t * kvstats, globalparams_t globalparams);
	
	void topkernel(
uint512_dt * kvsourcedramA
		,uint512_dt * kvdestdramA
		,keyvalue_t * kvstatsA
        );
private:
};
#endif 
#endif 







