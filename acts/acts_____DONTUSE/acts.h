#ifndef ACTS_H
#define ACTS_H
// #include "../include/actscommon.h"
#include "../../include/common.h"
#include "../include/acts_common.h"
#ifdef FPGA_IMPL
#include <ap_int.h>
#endif
#ifndef FPGA_IMPL
#include "../actsutility/actsutility.h"
#endif 
using namespace std;

// #define COLLECTCAPSULES_TYPE1
#define COLLECTCAPSULES_TYPE2 //

// #define APPENDINVALIDS_TYPE1
#define APPENDINVALIDS_TYPE2 //

// #define WRITEPARTITION_TYPE1
// #define WRITEPARTITION_TYPE2 // 
#define WRITEPARTITION_TYPE3
// #define WRITEPARTITION_TYPE4

// #define COLLECTCAPSULES_2READPIPELINES
#ifdef COLLECTCAPSULES_2READPIPELINES
#define CS_NUM_READ_PIPELINES 2
#else 
#define CS_NUM_READ_PIPELINES 1
#endif 

// #define PARTITIONKEYVALUES_2READPIPELINES
#ifdef PARTITIONKEYVALUES_2READPIPELINES
#define PVU_NUM_READ_PIPELINES 2
#else 
#define PVU_NUM_READ_PIPELINES 1
#endif 


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

#define OPTIMALSIZE (PADDEDDESTBUFFER_SIZE / NUM_PARTITIONS) // used with ENABLE_APPROXIMATEPARTITIONWRITES 
#ifdef WRITEPARTITION_TYPE3
#if OPTIMALSIZE < 16
    #define PADSKIP 1
#else
    #define PADSKIP (OPTIMALSIZE / 16)
#endif
#endif 
#ifdef WRITEPARTITION_TYPE4
#if OPTIMALSIZE < 6
    #define PADSKIP 1
#else
    #define PADSKIP (OPTIMALSIZE / 6)
#endif
#endif

// #ifdef FPGA_IMPL
// typedef unsigned int batch_type;
// typedef ap_uint<13> buffer_type;
// typedef ap_uint<8> partition_type;
// typedef ap_uint<8> vector_type;
// typedef ap_uint<8> step_type;
// typedef ap_uint<8> bool_type;
// typedef unsigned int analysis_type;
// #else 
// typedef unsigned int batch_type;
// typedef unsigned int buffer_type;
// typedef unsigned int partition_type;
// typedef unsigned int vector_type;
// typedef unsigned int step_type;
// typedef unsigned int bool_type;
// typedef unsigned int analysis_type;
// #endif

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

#ifndef FPGA_IMPL
class acts {
public:
	acts();
	~acts();
	
	unsigned int GET_KVDATA_RANGE_PERSSDPARTITION_POW(unsigned int groupid);
	unsigned int GET_KVDATA_RANGE_PERSSDPARTITION(unsigned int groupid);
	unsigned int GET_KVDATA_RANGE_PERSSDPARTITION_KVS(unsigned int groupid);
	unsigned int GET_BATCH_RANGE_POW(unsigned int groupid);
	unsigned int GET_BATCH_RANGE(unsigned int groupid);
	unsigned int GET_BATCH_RANGE_KVS(unsigned int groupid);
	unsigned int GET_TREE_DEPTH(unsigned int groupid);
	unsigned int GET_APPLYVERTEXBUFFERSZ(unsigned int groupid);
	unsigned int GET_APPLYVERTEXBUFFERSZ_KVS(unsigned int groupid);
	
	unsigned int allignhigher_KV(unsigned int val);
	unsigned int allignlower_KV(unsigned int val);
	unsigned int ceildiv(unsigned int val1, unsigned int val2);
	unsigned int max(unsigned int val1, unsigned int val2);
	unsigned int min(unsigned int A, unsigned int B);
	unsigned int hsub(unsigned int A, unsigned int B);
	void checkandforce(buffer_type data, unsigned int upper_bound, unsigned int * datatoforce, unsigned int forceval);
	void checkandforce(batch_type data, unsigned int upper_bound, unsigned int * datatoforce, unsigned int forceval);
	int WithinValidRange(unsigned int val1, unsigned int val2);
	buffer_type getpartitionwritesz(buffer_type realsize_kvs, buffer_type bramoffset_kvs);

	unsigned int getpartition(keyvalue_t keyvalue, unsigned int currentLOP, vertex_t upperlimit, unsigned int batch_range_pow);
	keyvalue_t getkeyvalue(uint512_dt * buffer, batch_type addr, batch_type maxaddr_kvs);
	keyvalue_t getkeyvalue(uint512_dt * Vtemp, vector_type idx);
	void setkeyvalue(uint512_dt * buffer, batch_type offset, batch_type addr, keyvalue_t keyvalue, batch_type maxaddr_kvs);
	void setkeyvalue(uint512_dt * Vtemp, vector_type idx, keyvalue_t keyvalue);
	void setkeyvalues( uint512_dt * Vtemp00,  uint512_dt * Vtemp01,  uint512_dt * Vtemp02,  uint512_dt * Vtemp03,  uint512_dt * Vtemp04,  uint512_dt * Vtemp05,  uint512_dt * Vtemp06,  uint512_dt * Vtemp07,   keyvalue_t keyvalue00,  keyvalue_t keyvalue01,  keyvalue_t keyvalue02,  keyvalue_t keyvalue03,  keyvalue_t keyvalue04,  keyvalue_t keyvalue05,  keyvalue_t keyvalue06,  keyvalue_t keyvalue07,  vector_type idx);
	unsigned int getvaluecount(keyvalue_t * keyvalues, unsigned int size);
	
	void resetkeyandvalues(keyvalue_t * buffer, unsigned int size);
	void resetvalues(keyvalue_t * buffer, unsigned int size);
	void actscopy(keyvalue_t * buffer1, keyvalue_t * buffer2, unsigned int size);
	void copykeys(keyvalue_t * buffer1, keyvalue_t * buffer2, unsigned int size);
	
	buffer_type getactivesize(buffer_type i, batch_type i_end, buffer_type buffersize);
	batch_type getAddr(batch_type addr);
	batch_type getmessagesAddr(batch_type addr);
	
	unsigned int getsourceskipsize(unsigned int currentLOP, unsigned int param);
	unsigned int getdestskipsize(unsigned int currentLOP, unsigned int param);
	void storedeststats(keyvalue_t * kvstats, keyvalue_t buffer[NUM_PARTITIONS], unsigned int drambaseoffset, unsigned int dramloffset);
	unsigned int get_num_source_partitions(unsigned int currentLOP);
	
	void calculateoffsets(keyvalue_t capsule[NUM_PARTITIONS], unsigned int skipspacing);
	void calculatemanyoffsets(keyvalue_t capsule0[NUM_PARTITIONS], unsigned int skipspacing);
	void calculateoffsets(keyvalue_t capsule[NUM_PARTITIONS], unsigned int offset, unsigned int skipspacing);
	void calculateoffsets(keyvalue_t capsule[NUM_PARTITIONS], unsigned int offset, unsigned int skipspacing[NUM_PARTITIONS]);
	
	void loadclopparams(globalparams_t globalparams, clopparams_t * llopparams, unsigned int currentLOP);
	value_t reducefunc(keyy_t vid, value_t value, value_t edgeval, unsigned int GraphIter);
	void getchunksize(buffer_type * chunk_size, buffer_type buffer_size, travstate_t travstate, unsigned int localoffset);
	int IsReducePhase(unsigned int currentLOP, unsigned int treedepth, globalparams_t globalparams);
	void shutdownpartitionparams(partitionparams_t * partitionparams);
	
	void resetcapsules(keyvalue_t capsule0[NUM_PARTITIONS],  int dummy);
	void appendinvalids0(unsigned int workerID  ,uint512_dt destbuffer0[PADDEDDESTBUFFER_SIZE]  ,keyvalue_t capsule0[NUM_PARTITIONS]);
	
	void readkeyvalues0(uint512_dt * kvdram, uint512_dt * buffer, batch_type baseaddress, batch_type offset_kvs, buffer_type size_kvs, batch_type maxaddress_kvs);
	void readkeyvalues0(keyvalue_t * dram, keyvalue_t * buffer, batch_type baseaddress, batch_type offset_kvs, buffer_type size_kvs);
	void readkeyvalues0(uint512_dt * kvdram  ,keyvalue_t * buffer00  ,keyvalue_t * buffer01  ,keyvalue_t * buffer02  ,keyvalue_t * buffer03  ,keyvalue_t * buffer04  ,keyvalue_t * buffer05  ,keyvalue_t * buffer06  ,keyvalue_t * buffer07  ,batch_type baseaddress, batch_type offset_kvs, buffer_type size_kvs, batch_type maxaddress_kvs);
	void readkeyvalues0(unsigned int enable, unsigned int workerID, uint512_dt * kvdram  ,uint512_dt * buffer0  ,batch_type baseaddress ,batch_type offset_kvs, travstate_t travstate, batch_type maxaddress_kvs);
	void readkeyvalues0(unsigned int workerID, uint512_dt * kvdram  ,uint512_dt * buffer0  ,batch_type baseaddress, keyvalue_t sourcestats[NUMSUBWORKERS], travstate_t travstate, batch_type maxaddress_kvs);
	offset_t readcapsules0(unsigned int workerID, uint512_dt * kvdram,  uint512_dt BIGcapsule0[CAPSULEBUFFER_SIZE], keyvalue_t capsule0[NUM_PARTITIONS],  batch_type baseaddress, batch_type offset, int enable, offset_t capsulemetadata);
	
	void collectstats0(unsigned int enable, unsigned int workerID , uint512_dt sourcebuffer0[SRCBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], clopparams_t llopparams, travstate_t travstate, globalparams_t globalparams);
	void partitionkeyvalues0(unsigned int enable, unsigned int workerID , uint512_dt sourcebuffer0[SRCBUFFER_SIZE] , uint512_dt destbuffer0[PADDEDDESTBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], travstate_t travstate, clopparams_t llopparams, globalparams_t globalparams);
	void reducepartitions0(unsigned int enable, unsigned int workerID  ,uint512_dt sourcebuffer0[SRCBUFFER_SIZE]   ,uint512_dt destbuffer0[PADDEDDESTBUFFER_SIZE]  ,batch_type voffset, keyvalue_t sourcestats[NUMSUBWORKERS], travstate_t travstate, globalparams_t globalparams);
	
	void savekeyvalues0(uint512_dt * kvdram, uint512_dt * buffer, batch_type baseaddress, batch_type offset_kvs, buffer_type size_kvs, batch_type maxaddress_kvs);
	void savekeyvalues0(keyvalue_t * kvstats, keyvalue_t * buffer, batch_type baseaddress, batch_type offset_kvs, buffer_type size_kvs);
	void savekeyvalues0(uint512_dt * kvdram  ,keyvalue_t * buffer00  ,keyvalue_t * buffer01  ,keyvalue_t * buffer02  ,keyvalue_t * buffer03  ,keyvalue_t * buffer04  ,keyvalue_t * buffer05  ,keyvalue_t * buffer06  ,keyvalue_t * buffer07  ,batch_type baseaddress, batch_type offset_kvs, buffer_type size_kvs, batch_type maxaddress_kvs);
	offset_t savecapsules0(unsigned int workerID, uint512_dt * kvdram , uint512_dt BIGcapsule0[CAPSULEBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t kvdeststats_tmp[NUM_PARTITIONS], keyvalue_t kvdeststats3_tmp[NUM_PARTITIONS], batch_type baseaddress_kvs, batch_type offset, travstate_t travstate, offset_t capsulemetadata);
	void savepartitions0(unsigned int workerID, uint512_dt * kvdram ,uint512_dt result_local0[PADDEDBUFFER_SIZE] ,keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t kvdeststats_tmp[NUM_PARTITIONS], clopparams_t llopparams);
	
	void partitionandreduce0(uint512_dt * kvsourcedram, uint512_dt * kvdestdram, keyvalue_t * kvstats, globalparams_t globalparams);
	
	void topkernel(
uint512_dt * kvsourcedramA
		,uint512_dt * kvdestdramA
		,keyvalue_t * kvstatsA
        );
private:
	actsutility * actsutilityobj;
};
#endif 
#endif 







