#ifndef ACTS_H
#define ACTS_H
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

#define ENABLEREDUCEPHASE
// #define _DEBUGMODE_CHECKSM

#define _LDEBUGMODE_HEADER _DEBUGMODE_HEADER

#define SRCBUFFER_SIZE (512 - (2 * NUM_PARTITIONS))
#define PADDEDDESTBUFFER_SIZE 2048
#define PADDEDBUFFER_SIZE PADDEDDESTBUFFER_SIZE
#define CAPSULEBUFFER_SIZE 512

#define NFACTOR (PADDEDDESTBUFFER_SIZE / SRCBUFFER_SIZE)

#define TEMPBUFFER_SIZE PADDEDBUFFER_SIZE
#define TEMPPAD 4
#define OPTIMALTEMPBUFFER_SIZE ((PADDEDBUFFER_SIZE / NUM_PARTITIONS) * NUMSUBWORKERS)
#define TEMP1BUFFER_SIZE (((1 * OPTIMALTEMPBUFFER_SIZE) / 2) + TEMPPAD)
#define TEMP2BUFFER_SIZE (((2 * OPTIMALTEMPBUFFER_SIZE) / 2) + TEMPPAD)
#define TEMP3BUFFER_SIZE (((3 * OPTIMALTEMPBUFFER_SIZE) / 2) + TEMPPAD)

#define OPTIMALBUFFER_SIZE (PADDEDBUFFER_SIZE / NUM_PARTITIONS)
#define BUFFER1_SIZE (((1 * OPTIMALBUFFER_SIZE) / 2) + TEMPPAD)
#define BUFFER2_SIZE (((2 * OPTIMALBUFFER_SIZE) / 2) + TEMPPAD)
#define BUFFER3_SIZE (((3 * OPTIMALBUFFER_SIZE) / 2) + TEMPPAD)

#define APPLYVERTEXBUFFERSZ (1 << (KVDATA_RANGE_PERSSDPARTITION_POW - (TREE_DEPTH * NUM_PARTITIONS_POW)))
#define APPLYVERTEXBUFFERSZ_KVS (APPLYVERTEXBUFFERSZ / VECTOR_SIZE)

#define ENREADKEYVALUES 1
#define ENREADKVS_COLLECTSTATS 0
#define ENSAVECAPSULES_COLLECTSTATS 1
#define ENREADKEYVALUES_PARTITION 1
#define ENREADCAPSULES_PARTITION 1
#define ENREADVPROPS_REDUCE 0
#define ENREADKEYVALUES_REDUCE 1
#define ENSAVEVERTICES_REDUCE 1

#define EXTRACTCAPSULES 5
#define PARTITIONKEYVALUES 6
#define REDUCEKEYVALUES 7

#define DEBUGGER_SIZE KVDATA_BATCHSIZE

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
	unsigned int runkernelcommand;
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
	unsigned int GraphIter;
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
	unsigned int topi_kvs;
	unsigned int i_kvs;
	unsigned int begin_kvs;
	unsigned int end_kvs;
	unsigned int skip_kvs;
	unsigned int info;
} travstate_t;

typedef struct {
	unsigned int capsuleoffset_kvs;
} travoffsets_t;

typedef struct {
	unsigned int type;
} operation_t;

#ifndef FPGA_IMPL
class acts {
public:
	acts();
	~acts();
	#ifdef _LDEBUGMODE_HEADER
	void checkoutofbounds(string message, unsigned int data, unsigned int upper_bound, unsigned int msgdata1, unsigned int msgdata2, unsigned int msgdata3);
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
	void printglobalvars();
	void clearglobalvars();
	void createDebuggers();
	void initializeDebuggers();
	void debuggerA(keyvalue_t ** Debugger_Sizes, keyvalue_t sizes[NUM_PARTITIONS], unsigned int committype);
	void IsEqual(keyvalue_t ** data1, keyvalue_t ** data2, unsigned int _1stdimsize, unsigned int _2nddimsize);
	#endif
	#ifdef _DEBUGMODE_STATS
	void globalstats_countkvstatsread(unsigned int count);
	void globalvar_countkvsread4statscollection(unsigned int count);
	void globalstats_countkvstatswritten(unsigned int count);
	void globalstats_countkvsread(unsigned int count);
	void globalstats_countkvswritten(unsigned int count);
	void globalstats_countkvspartitionswritten(unsigned int count);
	void globalstats_countkvspartitioned(unsigned int count);
	void globalstats_countkvsreduced(unsigned int count);
	void globalstats_countkvsreducewritten(unsigned int count);
	void globalstats_countkvsreadV(unsigned int count);
	#endif
	
	unsigned int allignhigher_KV(unsigned int val);
	unsigned int allignlower_KV(unsigned int val);
	unsigned int ceildiv(unsigned int val1, unsigned int val2);
	unsigned int max(unsigned int val1, unsigned int val2);
	unsigned int min(unsigned int A, unsigned int B);
	unsigned int hsub(unsigned int A, unsigned int B);
	void checkandforce(unsigned int data, unsigned int upper_bound, unsigned int * datatoforce, unsigned int forceval);
	int WithinValidRange(unsigned int val1, unsigned int val2);

	unsigned int getpartition(keyvalue_t keyvalue, unsigned int currentLOP, vertex_t upperlimit);
	keyvalue_t getkeyvalue(uint512_dt * buffer, batch_type addr, batch_type maxaddr_kvs);
	void setkeyvalue(uint512_dt * buffer, batch_type addr, keyvalue_t keyvalue, batch_type maxaddr_kvs);
	
	void resetkeyandvalues(keyvalue_t * buffer, unsigned int size);
	void resetvalues(keyvalue_t * buffer, unsigned int size);
	void copy(keyvalue_t * buffer1, keyvalue_t * buffer2, unsigned int size);
	void copykeys(keyvalue_t * buffer1, keyvalue_t * buffer2, unsigned int size);
	
	buffer_type getactivesize(buffer_type i, batch_type i_end, buffer_type buffersize);
	batch_type getAddr(batch_type addr);
	batch_type getstatsAddr(batch_type addr);
	batch_type getmessagesAddr(batch_type addr);
	
	unsigned int getsourceskipsize(unsigned int currentLOP, unsigned int param);
	unsigned int getdestskipsize(unsigned int currentLOP, unsigned int param);
	void loaddeststats(keyvalue_t * kvstats, keyvalue_t buffer[NUM_PARTITIONS], unsigned int partitionoffset, unsigned int currentLOP, vertex_t LLOPnumpartitions);
	void storedeststats(keyvalue_t * kvstats, keyvalue_t buffer[NUM_PARTITIONS], unsigned int drambaseoffset, unsigned int dramloffset);
	unsigned int get_num_source_partitions(unsigned int currentLOP);
	
	void calculateoffsets(keyvalue_t capsule[NUM_PARTITIONS]);
	void calculateoffsets(keyvalue_t capsule0[NUM_PARTITIONS],keyvalue_t capsule1[NUM_PARTITIONS],keyvalue_t capsule2[NUM_PARTITIONS],keyvalue_t capsule3[NUM_PARTITIONS], int dummy);
	void calculateoffsets(keyvalue_t capsule[NUM_PARTITIONS], unsigned int offset);
	
	void loadclopparams(globalparams_t globalparams, clopparams_t * llopparams, unsigned int currentLOP);
	value_t reducefunc(keyy_t vid, value_t value, value_t edgeval, unsigned int GraphIter);
	void getchunksize(buffer_type * chunk_size, buffer_type buffer_size, travstate_t travstate, unsigned int localoffset_kvs);
	int IsReducePhase(unsigned int currentLOP, unsigned int treedepth, globalparams_t globalparams);
	
	void readkeyvalues0(uint512_dt * kvdram, uint512_dt * buffer, batch_type baseaddress, batch_type offset_kvs, buffer_type size_kvs, batch_type maxaddress_kvs, int enable);
	travoffsets_t readcapsules0(unsigned int workerID, uint512_dt * kvdram,  uint512_dt BIGcapsule0[CAPSULEBUFFER_SIZE], uint512_dt BIGcapsule1[CAPSULEBUFFER_SIZE], uint512_dt BIGcapsule2[CAPSULEBUFFER_SIZE], uint512_dt BIGcapsule3[CAPSULEBUFFER_SIZE], keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS],  batch_type baseaddress, batch_type offset_kvs, int enable, travoffsets_t travoffsets);
	
	void extractcapsules00(unsigned int workerID , uint512_dt sourcebuffer0[SRCBUFFER_SIZE], uint512_dt sourcebuffer1[SRCBUFFER_SIZE], uint512_dt sourcebuffer2[SRCBUFFER_SIZE], uint512_dt sourcebuffer3[SRCBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS], clopparams_t llopparams, travstate_t travstate);
	void extractcapsules01(unsigned int workerID , uint512_dt sourcebuffer0[SRCBUFFER_SIZE], uint512_dt sourcebuffer1[SRCBUFFER_SIZE], uint512_dt sourcebuffer2[SRCBUFFER_SIZE], uint512_dt sourcebuffer3[SRCBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS], clopparams_t llopparams, travstate_t travstate);
	void extractcapsules02(unsigned int workerID , uint512_dt sourcebuffer0[SRCBUFFER_SIZE], uint512_dt sourcebuffer1[SRCBUFFER_SIZE], uint512_dt sourcebuffer2[SRCBUFFER_SIZE], uint512_dt sourcebuffer3[SRCBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS], clopparams_t llopparams, travstate_t travstate);
	void extractcapsules03(unsigned int workerID , uint512_dt sourcebuffer0[SRCBUFFER_SIZE], uint512_dt sourcebuffer1[SRCBUFFER_SIZE], uint512_dt sourcebuffer2[SRCBUFFER_SIZE], uint512_dt sourcebuffer3[SRCBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS], clopparams_t llopparams, travstate_t travstate);
	void partitionkeyvalues00(unsigned int workerID , uint512_dt sourcebuffer0[SRCBUFFER_SIZE], uint512_dt sourcebuffer1[SRCBUFFER_SIZE], uint512_dt sourcebuffer2[SRCBUFFER_SIZE], uint512_dt sourcebuffer3[SRCBUFFER_SIZE] , uint512_dt destbuffer0[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer1[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer2[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer3[PADDEDDESTBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS], travstate_t travstate, clopparams_t llopparams);
	void partitionkeyvalues01(unsigned int workerID , uint512_dt sourcebuffer0[SRCBUFFER_SIZE], uint512_dt sourcebuffer1[SRCBUFFER_SIZE], uint512_dt sourcebuffer2[SRCBUFFER_SIZE], uint512_dt sourcebuffer3[SRCBUFFER_SIZE] , uint512_dt destbuffer0[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer1[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer2[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer3[PADDEDDESTBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS], travstate_t travstate, clopparams_t llopparams);
	void partitionkeyvalues02(unsigned int workerID , uint512_dt sourcebuffer0[SRCBUFFER_SIZE], uint512_dt sourcebuffer1[SRCBUFFER_SIZE], uint512_dt sourcebuffer2[SRCBUFFER_SIZE], uint512_dt sourcebuffer3[SRCBUFFER_SIZE] , uint512_dt destbuffer0[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer1[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer2[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer3[PADDEDDESTBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS], travstate_t travstate, clopparams_t llopparams);
	void partitionkeyvalues03(unsigned int workerID , uint512_dt sourcebuffer0[SRCBUFFER_SIZE], uint512_dt sourcebuffer1[SRCBUFFER_SIZE], uint512_dt sourcebuffer2[SRCBUFFER_SIZE], uint512_dt sourcebuffer3[SRCBUFFER_SIZE] , uint512_dt destbuffer0[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer1[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer2[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer3[PADDEDDESTBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS], travstate_t travstate, clopparams_t llopparams);
	void reducepartitions0(unsigned int workerID  ,uint512_dt sourcebuffer0[SRCBUFFER_SIZE]  ,uint512_dt sourcebuffer1[SRCBUFFER_SIZE]  ,uint512_dt sourcebuffer2[SRCBUFFER_SIZE]  ,uint512_dt sourcebuffer3[SRCBUFFER_SIZE]   ,uint512_dt destbuffer0[PADDEDDESTBUFFER_SIZE]  ,uint512_dt destbuffer1[PADDEDDESTBUFFER_SIZE]  ,uint512_dt destbuffer2[PADDEDDESTBUFFER_SIZE]  ,uint512_dt destbuffer3[PADDEDDESTBUFFER_SIZE]  ,travstate_t travstate, globalparams_t globalparams);
	
	void savekeyvalues0(uint512_dt * kvdram, uint512_dt * buffer, batch_type baseaddress, batch_type offset_kvs, buffer_type size_kvs, batch_type maxaddress_kvs, int enable);
	travoffsets_t savecapsules0(unsigned int workerID, uint512_dt * kvdram , uint512_dt BIGcapsule0[CAPSULEBUFFER_SIZE], uint512_dt BIGcapsule1[CAPSULEBUFFER_SIZE], uint512_dt BIGcapsule2[CAPSULEBUFFER_SIZE], uint512_dt BIGcapsule3[CAPSULEBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS], keyvalue_t kvdeststats_tmp[NUM_PARTITIONS], batch_type baseaddress_kvs, batch_type offset_kvs, int enable, travstate_t travstate, travoffsets_t travoffsets);
	void savepartitions0(unsigned int workerID, uint512_dt * kvdram ,uint512_dt result_local0[PADDEDBUFFER_SIZE],uint512_dt result_local1[PADDEDBUFFER_SIZE],uint512_dt result_local2[PADDEDBUFFER_SIZE],uint512_dt result_local3[PADDEDBUFFER_SIZE] ,keyvalue_t capsule0[NUM_PARTITIONS],keyvalue_t capsule1[NUM_PARTITIONS],keyvalue_t capsule2[NUM_PARTITIONS],keyvalue_t capsule3[NUM_PARTITIONS], keyvalue_t kvdeststats_tmp[NUM_PARTITIONS], batch_type kvdrambaseaddress, int enable);
	
	void resetcapsules0(keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS],  int dummy);
	
	void partitionandreduce0(uint512_dt * kvsourcedram, uint512_dt * kvdestdram, keyvalue_t * kvstats, globalparams_t globalparams);
	
	void topkernel(
uint512_dt * kvsourcedramA
		,uint512_dt * kvdestdramA
		,keyvalue_t * kvstatsA
        );
private:
};
#endif 
#endif 







