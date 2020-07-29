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

// #define COLLECTCAPSULES_TYPE1
#define COLLECTCAPSULES_TYPE2

#define _LDEBUGMODE_HEADER _DEBUGMODE_HEADER

/** #define SRCBUFFER_SIZE (512 - (2 * NUM_PARTITIONS))
#define PADDEDDESTBUFFER_SIZE 2048
 
#define PADDEDBUFFER_SIZE PADDEDDESTBUFFER_SIZE
#define CAPSULEBUFFER_SIZE 512 */

#define SRCBUFFER_SIZE 512
#define PADDEDDESTBUFFER_SIZE (2048 + (2 * NUM_PARTITIONS))
#define PADDEDBUFFER_SIZE PADDEDDESTBUFFER_SIZE
#define CAPSULEBUFFER_SIZE 512

#define NFACTOR (PADDEDDESTBUFFER_SIZE / SRCBUFFER_SIZE)

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
	unsigned int globaloffset;
	unsigned int localoffset;
} offset_t;

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
	void print2(string messagea, string messageb, unsigned int dataa, unsigned int datab);
	void print4(string messagea, string messageb, string messagec, string messaged, unsigned int dataa, unsigned int datab, unsigned int datac, unsigned int datad);
	void print5(string messagea, string messageb, string messagec, string messaged, string messagee, unsigned int dataa, unsigned int datab, unsigned int datac, unsigned int datad, unsigned int datae);
	void print6(string messagea, string messageb, string messagec, string messaged, string messagee, string messagef, unsigned int dataa, unsigned int datab, unsigned int datac, unsigned int datad, unsigned int datae, unsigned int datef);
	void printkeyvalues(string message, keyvalue_t * keyvalues, unsigned int size);
	void printkeyvalues(string message, keyvalue_t * keyvalues1, keyvalue_t * keyvalues2, unsigned int size);
	void printkeyvalues(string message, uint512_dt * keyvalues, unsigned int size_kvs);
	void printvaluecount(string message, keyvalue_t * keyvalues, unsigned int size);
	unsigned int getvaluecountexcept(keyvalue_t * keyvalues, unsigned int size, unsigned int exceptvalue);
	void setkeyvalues(string message, keyvalue_t * keyvalues, unsigned int size, keyvalue_t keyvalue);
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
	void globalvar_collectstats_counttotalkvsread(unsigned int count);
	void globalvar_extractcapsules_counttotalkvsread(unsigned int count);
	void globalvar_organizekeyvalues_counttotalkvsread(unsigned int count);
	void globalvar_savestats_counttotalstatswritten(unsigned int count);
	void globalvar_savepartitions_countinvalids(unsigned int count);
	void globalvar_inmemory_counttotalvalidkeyvalues(unsigned int count);
	void globalstats_countkvsread(unsigned int count);
	void globalstats_countkvswritten(unsigned int count);
	void globalstats_countkvspartitionswritten(unsigned int count);
	void globalstats_countkvspartitionswritten_actual(unsigned int count);
	void globalstats_countkvspartitioned(unsigned int count);
	void globalstats_countkvsreduced(unsigned int count);
	void globalstats_countkvsreducewritten(unsigned int count);
	void globalstats_reduce_countvalidkvsreduced(unsigned int count);
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
	keyvalue_t getkeyvalue(uint512_dt * Vtemp, vector_type idx);
	void setkeyvalue(uint512_dt * buffer, batch_type addr, keyvalue_t keyvalue, batch_type maxaddr_kvs);
	void setkeyvalue(uint512_dt * Vtemp, vector_type idx, keyvalue_t keyvalue);
	void setkeyvalues( uint512_dt * Vtemp00,  uint512_dt * Vtemp01,  uint512_dt * Vtemp02,  uint512_dt * Vtemp03,  uint512_dt * Vtemp04,  uint512_dt * Vtemp05,  uint512_dt * Vtemp06,  uint512_dt * Vtemp07,  uint512_dt * Vtemp10,  uint512_dt * Vtemp11,  uint512_dt * Vtemp12,  uint512_dt * Vtemp13,  uint512_dt * Vtemp14,  uint512_dt * Vtemp15,  uint512_dt * Vtemp16,  uint512_dt * Vtemp17,  uint512_dt * Vtemp20,  uint512_dt * Vtemp21,  uint512_dt * Vtemp22,  uint512_dt * Vtemp23,  uint512_dt * Vtemp24,  uint512_dt * Vtemp25,  uint512_dt * Vtemp26,  uint512_dt * Vtemp27,  uint512_dt * Vtemp30,  uint512_dt * Vtemp31,  uint512_dt * Vtemp32,  uint512_dt * Vtemp33,  uint512_dt * Vtemp34,  uint512_dt * Vtemp35,  uint512_dt * Vtemp36,  uint512_dt * Vtemp37,   keyvalue_t keyvalue00,  keyvalue_t keyvalue01,  keyvalue_t keyvalue02,  keyvalue_t keyvalue03,  keyvalue_t keyvalue04,  keyvalue_t keyvalue05,  keyvalue_t keyvalue06,  keyvalue_t keyvalue07,  keyvalue_t keyvalue10,  keyvalue_t keyvalue11,  keyvalue_t keyvalue12,  keyvalue_t keyvalue13,  keyvalue_t keyvalue14,  keyvalue_t keyvalue15,  keyvalue_t keyvalue16,  keyvalue_t keyvalue17,  keyvalue_t keyvalue20,  keyvalue_t keyvalue21,  keyvalue_t keyvalue22,  keyvalue_t keyvalue23,  keyvalue_t keyvalue24,  keyvalue_t keyvalue25,  keyvalue_t keyvalue26,  keyvalue_t keyvalue27,  keyvalue_t keyvalue30,  keyvalue_t keyvalue31,  keyvalue_t keyvalue32,  keyvalue_t keyvalue33,  keyvalue_t keyvalue34,  keyvalue_t keyvalue35,  keyvalue_t keyvalue36,  keyvalue_t keyvalue37,  vector_type idx);
	unsigned int getvaluecount(keyvalue_t * keyvalues, unsigned int size);
	
	void resetkeyandvalues(keyvalue_t * buffer, unsigned int size);
	void resetvalues(keyvalue_t * buffer, unsigned int size);
	void copy(keyvalue_t * buffer1, keyvalue_t * buffer2, unsigned int size);
	void copykeys(keyvalue_t * buffer1, keyvalue_t * buffer2, unsigned int size);
	
	buffer_type getactivesize(buffer_type i, batch_type i_end, buffer_type buffersize);
	batch_type getAddr(batch_type addr);
	batch_type getmessagesAddr(batch_type addr);
	
	unsigned int getsourceskipsize(unsigned int currentLOP, unsigned int param);
	unsigned int getdestskipsize(unsigned int currentLOP, unsigned int param);
	void storedeststats(keyvalue_t * kvstats, keyvalue_t buffer[NUM_PARTITIONS], unsigned int drambaseoffset, unsigned int dramloffset);
	unsigned int get_num_source_partitions(unsigned int currentLOP);
	
	void calculateoffsets(keyvalue_t capsule[NUM_PARTITIONS], unsigned int skipspacing);
	void calculateoffsets(keyvalue_t capsule0[NUM_PARTITIONS],keyvalue_t capsule1[NUM_PARTITIONS],keyvalue_t capsule2[NUM_PARTITIONS],keyvalue_t capsule3[NUM_PARTITIONS], unsigned int skipspacing);
	void calculateoffsets(keyvalue_t capsule[NUM_PARTITIONS], unsigned int offset, unsigned int skipspacing);
	
	void loadclopparams(globalparams_t globalparams, clopparams_t * llopparams, unsigned int currentLOP);
	value_t reducefunc(keyy_t vid, value_t value, value_t edgeval, unsigned int GraphIter);
	void getchunksize(buffer_type * chunk_size, buffer_type buffer_size, travstate_t travstate, unsigned int localoffset);
	int IsReducePhase(unsigned int currentLOP, unsigned int treedepth, globalparams_t globalparams);
	void shutdownpartitionparams(partitionparams_t * partitionparams);
	
	void resetcapsules(keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS],  int dummy);
	void appendinvalids0(unsigned int workerID  ,uint512_dt destbuffer0[PADDEDDESTBUFFER_SIZE] ,uint512_dt destbuffer1[PADDEDDESTBUFFER_SIZE] ,uint512_dt destbuffer2[PADDEDDESTBUFFER_SIZE] ,uint512_dt destbuffer3[PADDEDDESTBUFFER_SIZE]  ,keyvalue_t capsule0[NUM_PARTITIONS] ,keyvalue_t capsule1[NUM_PARTITIONS] ,keyvalue_t capsule2[NUM_PARTITIONS] ,keyvalue_t capsule3[NUM_PARTITIONS]);
	
	void readkeyvalues0(uint512_dt * kvdram, uint512_dt * buffer, batch_type baseaddress, batch_type offset_kvs, buffer_type size_kvs, batch_type maxaddress_kvs);
	void readkeyvalues0(keyvalue_t * dram, keyvalue_t * buffer, batch_type baseaddress, batch_type offset_kvs, buffer_type size_kvs);
	void readkeyvalues0(uint512_dt * kvdram  ,keyvalue_t * buffer00  ,keyvalue_t * buffer01  ,keyvalue_t * buffer02  ,keyvalue_t * buffer03  ,keyvalue_t * buffer04  ,keyvalue_t * buffer05  ,keyvalue_t * buffer06  ,keyvalue_t * buffer07  ,keyvalue_t * buffer10  ,keyvalue_t * buffer11  ,keyvalue_t * buffer12  ,keyvalue_t * buffer13  ,keyvalue_t * buffer14  ,keyvalue_t * buffer15  ,keyvalue_t * buffer16  ,keyvalue_t * buffer17  ,keyvalue_t * buffer20  ,keyvalue_t * buffer21  ,keyvalue_t * buffer22  ,keyvalue_t * buffer23  ,keyvalue_t * buffer24  ,keyvalue_t * buffer25  ,keyvalue_t * buffer26  ,keyvalue_t * buffer27  ,keyvalue_t * buffer30  ,keyvalue_t * buffer31  ,keyvalue_t * buffer32  ,keyvalue_t * buffer33  ,keyvalue_t * buffer34  ,keyvalue_t * buffer35  ,keyvalue_t * buffer36  ,keyvalue_t * buffer37  ,batch_type baseaddress, batch_type offset_kvs, buffer_type size_kvs, batch_type maxaddress_kvs);
	void readkeyvalues0(unsigned int enable, unsigned int workerID, uint512_dt * kvdram  ,uint512_dt * buffer0  ,uint512_dt * buffer1  ,uint512_dt * buffer2  ,uint512_dt * buffer3  ,batch_type baseaddress ,batch_type offset_kvs, travstate_t travstate, batch_type maxaddress_kvs);
	void readkeyvalues0(unsigned int workerID, uint512_dt * kvdram  ,uint512_dt * buffer0  ,uint512_dt * buffer1  ,uint512_dt * buffer2  ,uint512_dt * buffer3  ,batch_type baseaddress, keyvalue_t sourcestats[NUMSUBWORKERS], travstate_t travstate, batch_type maxaddress_kvs);
	offset_t readcapsules0(unsigned int workerID, uint512_dt * kvdram,  uint512_dt BIGcapsule0[CAPSULEBUFFER_SIZE], uint512_dt BIGcapsule1[CAPSULEBUFFER_SIZE], uint512_dt BIGcapsule2[CAPSULEBUFFER_SIZE], uint512_dt BIGcapsule3[CAPSULEBUFFER_SIZE], keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS],  batch_type baseaddress, batch_type offset, int enable, offset_t capsulemetadata);
	
	void collectstats00(unsigned int enable, unsigned int workerID , uint512_dt sourcebuffer0[SRCBUFFER_SIZE], uint512_dt sourcebuffer1[SRCBUFFER_SIZE], uint512_dt sourcebuffer2[SRCBUFFER_SIZE], uint512_dt sourcebuffer3[SRCBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS], clopparams_t llopparams, travstate_t travstate);
	void partitionkeyvalues00(unsigned int enable, unsigned int workerID , uint512_dt sourcebuffer0[SRCBUFFER_SIZE], uint512_dt sourcebuffer1[SRCBUFFER_SIZE], uint512_dt sourcebuffer2[SRCBUFFER_SIZE], uint512_dt sourcebuffer3[SRCBUFFER_SIZE] , uint512_dt destbuffer0[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer1[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer2[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer3[PADDEDDESTBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS], travstate_t travstate, clopparams_t llopparams);
	void reducepartitions0(unsigned int enable, unsigned int workerID  ,uint512_dt sourcebuffer0[SRCBUFFER_SIZE]  ,uint512_dt sourcebuffer1[SRCBUFFER_SIZE]  ,uint512_dt sourcebuffer2[SRCBUFFER_SIZE]  ,uint512_dt sourcebuffer3[SRCBUFFER_SIZE]   ,uint512_dt destbuffer0[PADDEDDESTBUFFER_SIZE]  ,uint512_dt destbuffer1[PADDEDDESTBUFFER_SIZE]  ,uint512_dt destbuffer2[PADDEDDESTBUFFER_SIZE]  ,uint512_dt destbuffer3[PADDEDDESTBUFFER_SIZE]  ,batch_type voffset, keyvalue_t sourcestats[NUMSUBWORKERS], travstate_t travstate, globalparams_t globalparams);
	
	void savekeyvalues0(uint512_dt * kvdram, uint512_dt * buffer, batch_type baseaddress, batch_type offset_kvs, buffer_type size_kvs, batch_type maxaddress_kvs);
	void savekeyvalues0(keyvalue_t * kvstats, keyvalue_t * buffer, batch_type baseaddress, batch_type offset_kvs, buffer_type size_kvs);
	void savekeyvalues0(uint512_dt * kvdram  ,keyvalue_t * buffer00  ,keyvalue_t * buffer01  ,keyvalue_t * buffer02  ,keyvalue_t * buffer03  ,keyvalue_t * buffer04  ,keyvalue_t * buffer05  ,keyvalue_t * buffer06  ,keyvalue_t * buffer07  ,keyvalue_t * buffer10  ,keyvalue_t * buffer11  ,keyvalue_t * buffer12  ,keyvalue_t * buffer13  ,keyvalue_t * buffer14  ,keyvalue_t * buffer15  ,keyvalue_t * buffer16  ,keyvalue_t * buffer17  ,keyvalue_t * buffer20  ,keyvalue_t * buffer21  ,keyvalue_t * buffer22  ,keyvalue_t * buffer23  ,keyvalue_t * buffer24  ,keyvalue_t * buffer25  ,keyvalue_t * buffer26  ,keyvalue_t * buffer27  ,keyvalue_t * buffer30  ,keyvalue_t * buffer31  ,keyvalue_t * buffer32  ,keyvalue_t * buffer33  ,keyvalue_t * buffer34  ,keyvalue_t * buffer35  ,keyvalue_t * buffer36  ,keyvalue_t * buffer37  ,batch_type baseaddress, batch_type offset_kvs, buffer_type size_kvs, batch_type maxaddress_kvs);
	offset_t savecapsules0(unsigned int workerID, uint512_dt * kvdram , uint512_dt BIGcapsule0[CAPSULEBUFFER_SIZE], uint512_dt BIGcapsule1[CAPSULEBUFFER_SIZE], uint512_dt BIGcapsule2[CAPSULEBUFFER_SIZE], uint512_dt BIGcapsule3[CAPSULEBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS], keyvalue_t kvdeststats_tmp[NUM_PARTITIONS], batch_type baseaddress_kvs, batch_type offset, travstate_t travstate, offset_t capsulemetadata);
	void savepartitions0(unsigned int workerID, uint512_dt * kvdram ,uint512_dt result_local0[PADDEDBUFFER_SIZE],uint512_dt result_local1[PADDEDBUFFER_SIZE],uint512_dt result_local2[PADDEDBUFFER_SIZE],uint512_dt result_local3[PADDEDBUFFER_SIZE] ,keyvalue_t capsule0[NUM_PARTITIONS],keyvalue_t capsule1[NUM_PARTITIONS],keyvalue_t capsule2[NUM_PARTITIONS],keyvalue_t capsule3[NUM_PARTITIONS], keyvalue_t kvdeststats_tmp[NUM_PARTITIONS], batch_type kvdrambaseaddress);
	
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







