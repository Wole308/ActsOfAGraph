#ifndef ACTS_H
#define ACTS_H
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
#include "../include/actscommon.h"
#ifndef FPGA_IMPL
#include "../../src/utility/utility.h"
#endif
#include "../../acts/actsutility/actsutility.h"
using namespace std;

#define PROCESSALLEDGES
#define COLLECTSTATS
#define PARTITIONUPDATES
#define REDUCEUPDATES

#define NUMPIPELINES 3
#if NUMPIPELINES==2
#define PP0
#define PP1
#endif 
#if NUMPIPELINES==3
#define PP0
#define PP1
#define PP2
#endif

#define MAXLOADFACTORFORREDUCE 64

// [keyvalue]100000000000 00110000000000010000000000000101
// [value]100000000000 
// [key]00110000000000010000000000000101 

#define PIPELINED_SYNCACTVVS
#define LOGKERNELSTATS
#define RUNSTATSSZ 64

class acts {
public:
	acts();
	~acts();
	
	unsigned int amin(unsigned int val1, unsigned int val2);
	unsigned int aplus(unsigned int val1, unsigned int val2);
	batch_type allignlowerto4_KV(batch_type val);
	batch_type allignhigherto4_KV(batch_type val);
	batch_type allignlower_KV(batch_type val);
	batch_type allignhigher_KV(batch_type val);
	batch_type allignlowerto16_KV(batch_type val);
	batch_type allignhigherto16_KV(batch_type val);
	ulong_dt CONVERTTOLONG_KV(keyvalue_t keyvalue);
	keyvalue_t CONVERTTOKV_ULONG(ulong_dt data);
	unsigned int GETMASK_UINT(unsigned int index, unsigned int size);
	ulong_dt GETMASK_ULONG(ulong_dt index, ulong_dt size);
	unsigned int READFROM_UINT(unsigned int data, unsigned int index, unsigned int size);
	void WRITETO_UINT(unsigned int * data, unsigned int index, unsigned int size, unsigned int value);
	unsigned int READFROM_ULONG(ulong_dt data, ulong_dt index, ulong_dt size);
	unsigned int READFROM_ULONG(keyvalue_t keyvalue, ulong_dt index, ulong_dt size);
	void WRITETO_ULONG(ulong_dt * data, ulong_dt index, ulong_dt size, ulong_dt value);
	void WRITETO_ULONG(keyvalue_t * keyvalue, ulong_dt index, ulong_dt size, ulong_dt value);
	batch_type getskipsize(step_type currentLOP, bool_type sourceORdest, globalparams_t globalparams);
	void resetkeyandvalues(skeyvalue_t * buffer, buffer_type size, unsigned int resetval);
	void resetvalues(keyvalue_t * buffer, buffer_type size, unsigned int resetval);
	void resetvalues(value_t * buffer, buffer_type size, unsigned int resetval);
	void resetmanykeyandvalues(skeyvalue_t buffer[VECTOR_SIZE][NUM_PARTITIONS], buffer_type size, unsigned int resetval);
	void resetmanykeyandvalues(keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], buffer_type size, unsigned int resetval);
	void resetmanykeyandvalues(keyvalue_t * buffer, buffer_type size, unsigned int resetval);
	void resetmanykeyandvalues(skeyvalue_t * buffer, buffer_type size, unsigned int resetval);
	void resetmanyvalues(skeyvalue_t buffer[VECTOR_SIZE][NUM_PARTITIONS], buffer_type size, unsigned int resetval);
	void resetmanyvalues(buffer_type buffer0[NUM_PARTITIONS],buffer_type buffer1[NUM_PARTITIONS],buffer_type buffer2[NUM_PARTITIONS],buffer_type buffer3[NUM_PARTITIONS],buffer_type buffer4[NUM_PARTITIONS],buffer_type buffer5[NUM_PARTITIONS],buffer_type buffer6[NUM_PARTITIONS],buffer_type buffer7[NUM_PARTITIONS], buffer_type size, unsigned int resetval);
	buffer_type getchunksize_kvs(buffer_type buffer_size, travstate_t travstate, buffer_type localoffset);
	buffer_type getchunksize(buffer_type buffersz, travstate_t travstate, buffer_type localoffset);
	partition_type getglobalpartition(keyvalue_t keyvalue, vertex_t upperlimit, unsigned int batch_range_pow);
	value_t processedgefunc(value_t Uprop, unsigned int edgeweight, unsigned int voutdegree, unsigned int GraphIter, unsigned int GraphAlgo);
	buffer_type getpartitionwritesz(buffer_type realsize_kvs, buffer_type bramoffset_kvs);
	unsigned int withinvalidrange(buffer_type val1, buffer_type val2);
	void calculateoffsets(keyvalue_t * buffer, buffer_type size, batch_type base, batch_type skipspacing[NUM_PARTITIONS]);
	void calculateunallignedoffsets(skeyvalue_t buffer[NUM_PARTITIONS], buffer_type size, batch_type base, batch_type skipspacing);
	void calculatemanyoffsets(skeyvalue_t buffer[VECTOR_SIZE][NUM_PARTITIONS], buffer_type size, batch_type base, batch_type skipspacing);
	void calculatemanyoffsets_allignby4(skeyvalue_t buffer[VECTOR_SIZE][NUM_PARTITIONS], buffer_type size, batch_type base, batch_type skipspacing);
	void calculatemanyoffsets_allignby4_lower(skeyvalue_t buffer[VECTOR_SIZE][NUM_PARTITIONS], buffer_type limitsz[VECTOR_SIZE][NUM_PARTITIONS], buffer_type size, buffer_type cutoff);
	void calculatemanyunallignedoffsets(skeyvalue_t buffer[VECTOR_SIZE][NUM_PARTITIONS], buffer_type size, batch_type base, batch_type skipspacing);
	batch_type getvaluecount(keyvalue_t * keyvalues, unsigned int size);
	batch_type get_num_source_partitions(step_type currentLOP);
	globalparams_t getglobalparams(uint512_dt * kvdram);
	sweepparams_t getsweepparams(globalparams_t globalparams, step_type currentLOP, batch_type source_partition);
	travstate_t gettravstate(bool_type enable, uint512_dt * kvdram, globalparams_t globalparams, step_type currentLOP, batch_type sourcestatsmarker, batch_type source_partition, keyvalue_t moretravstates[LOADFACTORFORREDUCE]);
	bool inprocessedgesstage(unsigned int currentLOP);
	bool incollectstatsstage(unsigned int currentLOP, globalparams_t globalparams);
	bool inpartitionstage(unsigned int currentLOP, globalparams_t globalparams);
	bool inreducestage(unsigned int currentLOP, globalparams_t globalparams);
	void calculateglobaloffsets(keyvalue_t * globalstatsbuffer, batch_type * skipsizes,  batch_type offset, batch_type size);
	keyvalue_t getkeyvalue(uint512_dt * keyvalues, unsigned int offset_kvs, vector_type v);
	uint512_vec_dt getkeyvalues(uint512_dt * keyvalues, unsigned int offset_kvs);
	void setkeyvalue(uint512_dt * keyvalues, unsigned int offset_kvs, vector_type v, keyvalue_t keyvalue);
	void setkeyvalues(uint512_dt * keyvalues, unsigned int offset_kvs, uint512_vec_dt D);
	void setkey(uint512_dt * keyvalues, unsigned int offset_kvs, vector_type v, keyy_t key);
	unsigned int fillintheblancks(keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], unsigned int index);
	
	// partition function 
	partition_type getpartition(bool_type enable, keyvalue_t keyvalue, step_type currentLOP, vertex_t upperlimit, unsigned int upperpartition, unsigned int batch_range_pow);

	// collect globalstats functions
	void readglobalstats(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ], batch_type offset_kvs);

	void collectglobalstats(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], step_type currentLOP, sweepparams_t sweepparams, travstate_t travstate, globalparams_t globalparams);

	void fastcollectglobalstats(bool_type enable, keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ], step_type currentLOP, batch_type source_partition);
	
	void prepareglobalstats(bool_type enable, keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ], globalparams_t globalparams);

	void prepareglobalstats2(bool_type enable, keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ], batch_type offset, globalparams_t globalparams);
	
	void saveglobalstats(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ], batch_type offset_kvs);
	
	// partition functions
	void readkeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], batch_type offset_kvs, batch_type size_kvs, travstate_t travstate);
	
	void preparekeyvalues_coarsegrainedpipeline(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t localcapsule[VECTOR_SIZE][NUM_PARTITIONS], step_type currentLOP, sweepparams_t sweepparams, travstate_t travstate, buffer_type size_kvs, globalparams_t globalparams);

	buffer_type preparekeyvalues1_evencutoffs(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t localcapsule[VECTOR_SIZE][NUM_PARTITIONS], step_type currentLOP, sweepparams_t sweepparams, travstate_t travstate, buffer_type size_kvs, globalparams_t globalparams);

	buffer_type preparekeyvalues2_evencutoffs(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t localcapsule[VECTOR_SIZE][NUM_PARTITIONS], step_type currentLOP, sweepparams_t sweepparams, travstate_t travstate, buffer_type size_kvs, globalparams_t globalparams);

	buffer_type preparekeyvalues_unevencutoffs(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t localcapsule[VECTOR_SIZE][NUM_PARTITIONS], step_type currentLOP, sweepparams_t sweepparams, travstate_t travstate, buffer_type size_kvs, buffer_type cutoffs[VECTOR_SIZE], globalparams_t globalparams);
	
	buffer_type preparekeyvalues_finegrainedpipeline(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t localcapsule[VECTOR_SIZE][NUM_PARTITIONS], step_type currentLOP, sweepparams_t sweepparams, travstate_t travstate, buffer_type size_kvs, buffer_type cutoffs[VECTOR_SIZE], globalparams_t globalparams);

	void savekeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_t buffer[8][PADDEDDESTBUFFER_SIZE], keyvalue_t * globalcapsule, skeyvalue_t localcapsule[NUM_PARTITIONS], batch_type globalbaseaddress_kvs, globalparams_t globalparams);
	
	// reduce functions
	void readkeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], batch_type offset_kvs, buffer_type size_kvs);

	void savekeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], batch_type offset_kvs, buffer_type size_kvs);
	
	void replicatedata(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], buffer_type sourceoffset, buffer_type size);

	void replicatedata_syn(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer0[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer3[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer5[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer6[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer7[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer8[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer9[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer10[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer11[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer12[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer13[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer14[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer15[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], buffer_type sourceoffset, buffer_type size);

	value_t reducefunc(value_t vtemp, value_t res, unsigned int GraphIter, unsigned int GraphAlgo);
	
	value_t reducefunc_bfs(value_t vtemp, value_t res, unsigned int GraphIter, unsigned int GraphAlgo);
	
	void reduce(bool_type enable1, bool_type enable2, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], sweepparams_t sweepparams, unsigned int GraphIter, unsigned int GraphAlgo, travstate_t travstate, globalparams_t globalparams);
	
	void reduce_bfs(bool_type enable1, bool_type enable2, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], sweepparams_t sweepparams, unsigned int GraphIter, unsigned int GraphAlgo, travstate_t travstate, globalparams_t globalparams);

	void reduce_sssp(bool_type enable1, bool_type enable2, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], sweepparams_t sweepparams, unsigned int GraphIter, unsigned int GraphAlgo, travstate_t travstate, globalparams_t globalparams);

	value_t mergefunc(value_t value1, value_t value2, unsigned int GraphAlgo);
	
	keyvalue_t mergefunc_bfs(keyvalue_t data1, keyvalue_t data2, unsigned int GraphAlgo);
	
	void unifydata(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], buffer_type destoffset, buffer_type size, unsigned int GraphAlgo);

	void unifydata_pr_parallelsyn(bool_type enable, keyvalue_t sourcebuffer0[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer3[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer5[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer6[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer7[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer8[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer9[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer10[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer11[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer12[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer13[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer14[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer15[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
					buffer_type destoffset, buffer_type size, sweepparams_t sweepparams, globalparams_t globalparams);
					
	bool_type unifydata_bfs_parallelsyn(bool_type enable, keyvalue_t sourcebuffer0[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer3[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer5[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer6[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer7[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer8[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer9[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer10[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer11[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer12[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer13[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer14[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer15[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
					keyvalue_t actvvs0[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t actvvscapsule0[8], buffer_type cutoffs0[VECTOR_SIZE],
					keyvalue_t actvvs1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t actvvscapsule1[8], buffer_type cutoffs1[VECTOR_SIZE],
					keyvalue_t actvvs2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t actvvscapsule2[8], buffer_type cutoffs2[VECTOR_SIZE],
					keyvalue_t actvvs3[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t actvvscapsule3[8], buffer_type cutoffs3[VECTOR_SIZE],
					buffer_type destoffset, buffer_type size, sweepparams_t sweepparams, globalparams_t globalparams);
					
	bool_type unifydata_sssp_parallelsyn(bool_type enable, keyvalue_t sourcebuffer0[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer3[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer5[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer6[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer7[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer8[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer9[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer10[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer11[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer12[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer13[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer14[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer15[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
					keyvalue_t actvvs[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], buffer_type * actvvtracker,
					buffer_type destoffset, buffer_type size, sweepparams_t sweepparams, globalparams_t globalparams);
	
	// apply functions 
	void apply_bfs(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE]);

	// process-edges function
	void process_edges(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], unsigned int upperlimit, unsigned int GraphIter, unsigned int GraphAlgo, travstate_t travstate, globalparams_t globalparams);
	
	void process_edges(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t statsbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], unsigned int upperlimit, unsigned int GraphIter, unsigned int GraphAlgo, travstate_t travstate, globalparams_t globalparams);

	// in-memory partition functions (1)
	void combineSetof1stoSetof2_I0(bool_type enable, keyvalue_t buffer_setof1M0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof1M1[PADDEDDESTBUFFER_SIZE], 
																keyvalue_t buffer_setof2R0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2R1[PADDEDDESTBUFFER_SIZE], 
																	skeyvalue_t localcapsuleM[NUM_PARTITIONS], skeyvalue_t localcapsuleN[NUM_PARTITIONS], skeyvalue_t localcapsuleR[NUM_PARTITIONS], globalparams_t globalparams);
	void combineSetof1stoSetof2_I1(bool_type enable, keyvalue_t buffer_setof1M0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof1M1[PADDEDDESTBUFFER_SIZE], 
																keyvalue_t buffer_setof2R0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2R1[PADDEDDESTBUFFER_SIZE], 
																	skeyvalue_t localcapsuleM[NUM_PARTITIONS], skeyvalue_t localcapsuleN[NUM_PARTITIONS], skeyvalue_t localcapsuleR[NUM_PARTITIONS], globalparams_t globalparams);
	void combineSetof1stoSetof2_I2(bool_type enable, keyvalue_t buffer_setof1M0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof1M1[PADDEDDESTBUFFER_SIZE], 
																keyvalue_t buffer_setof2R0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2R1[PADDEDDESTBUFFER_SIZE], 
																	skeyvalue_t localcapsuleM[NUM_PARTITIONS], skeyvalue_t localcapsuleN[NUM_PARTITIONS], skeyvalue_t localcapsuleR[NUM_PARTITIONS], globalparams_t globalparams);
	void combineSetof1stoSetof2_I3(bool_type enable, keyvalue_t buffer_setof1M0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof1M1[PADDEDDESTBUFFER_SIZE], 
																keyvalue_t buffer_setof2R0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2R1[PADDEDDESTBUFFER_SIZE], 
																	skeyvalue_t localcapsuleM[NUM_PARTITIONS], skeyvalue_t localcapsuleN[NUM_PARTITIONS], skeyvalue_t localcapsuleR[NUM_PARTITIONS], globalparams_t globalparams);
	void combineSetof2stoSetof4_I0(bool_type enable, keyvalue_t buffer_setof2M0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2M1[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2M2[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2M3[PADDEDDESTBUFFER_SIZE], 
																keyvalue_t buffer_setof4R0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4R1[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4R2[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4R3[PADDEDDESTBUFFER_SIZE], 
																	skeyvalue_t localcapsuleM[NUM_PARTITIONS], skeyvalue_t localcapsuleN[NUM_PARTITIONS], skeyvalue_t localcapsuleR[NUM_PARTITIONS], globalparams_t globalparams);
	void combineSetof2stoSetof4_I1(bool_type enable, keyvalue_t buffer_setof2M0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2M1[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2M2[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2M3[PADDEDDESTBUFFER_SIZE], 
																keyvalue_t buffer_setof4R0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4R1[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4R2[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4R3[PADDEDDESTBUFFER_SIZE], 
																	skeyvalue_t localcapsuleM[NUM_PARTITIONS], skeyvalue_t localcapsuleN[NUM_PARTITIONS], skeyvalue_t localcapsuleR[NUM_PARTITIONS], globalparams_t globalparams);
	void combineSetof4stoSetof8_I0(bool_type enable, keyvalue_t buffer_setof4M0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4M1[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4M2[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4M3[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4M4[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4M5[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4M6[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4M7[PADDEDDESTBUFFER_SIZE],				
																keyvalue_t buffer_setof8R0[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof8R1[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof8R2[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof8R3[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof8R4[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof8R5[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof8R6[PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof8R7[PADDEDDESTBUFFER_SIZE],
																	skeyvalue_t localcapsuleM[NUM_PARTITIONS], skeyvalue_t localcapsuleN[NUM_PARTITIONS], skeyvalue_t localcapsuleR[NUM_PARTITIONS], globalparams_t globalparams);

	void combineSetof1stoSetof2s(bool_type enable, keyvalue_t buffer_setof1[8][PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof2[8][PADDEDDESTBUFFER_SIZE], skeyvalue_t templocalcapsule_so1[8][NUM_PARTITIONS], skeyvalue_t templocalcapsule_so2[4][NUM_PARTITIONS], globalparams_t globalparams);
	void combineSetof2stoSetof4s(bool_type enable, keyvalue_t buffer_setof2[8][PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof4[8][PADDEDDESTBUFFER_SIZE], skeyvalue_t templocalcapsule_so2[4][NUM_PARTITIONS], skeyvalue_t templocalcapsule_so4[2][NUM_PARTITIONS], globalparams_t globalparams);
	void combineSetof4stoSetof8s(bool_type enable, keyvalue_t buffer_setof4[8][PADDEDDESTBUFFER_SIZE], keyvalue_t buffer_setof8[8][PADDEDDESTBUFFER_SIZE], skeyvalue_t templocalcapsule_so4[2][NUM_PARTITIONS], skeyvalue_t templocalcapsule_so8[NUM_PARTITIONS], globalparams_t globalparams);

	// in-memory partition functions (2)
	void runpipeline(bool_type enable, keyvalue_t bufferA[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t buffer1capsule[VECTOR_SIZE][NUM_PARTITIONS], 
						keyvalue_t bufferB[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t bufferBcapsule[4][NUM_PARTITIONS], 
							keyvalue_t bufferC[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t bufferCcapsule[2][NUM_PARTITIONS],
								keyvalue_t bufferD[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t bufferDcapsule[NUM_PARTITIONS],
									unsigned int currentLOP, sweepparams_t sweepparams, buffer_type cutoff, buffer_type cutoffs[VECTOR_SIZE], batch_type shiftcount, globalparams_t globalparams);
	
	skeyvalue_t runpipeline_1partition(bool_type enable, keyvalue_t bufferA[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t buffer1capsule[VECTOR_SIZE], 
				keyvalue_t bufferB[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t bufferBcapsule[4], 
					keyvalue_t bufferC[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t bufferCcapsule[2],
						keyvalue_t bufferD[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t bufferDcapsule,
							unsigned int currentLOP, sweepparams_t sweepparams, buffer_type cutoff, buffer_type cutoffs[VECTOR_SIZE], batch_type shiftcount, globalparams_t globalparams);
									
	// collectstats phase
	void collectstats(
		bool_type enable,
		uint512_dt * kvdram,
		keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ], 
		config_t config,
		globalparams_t globalparams,
		sweepparams_t sweepparams,
		travstate_t ctravstate);
	
	// partition updates phase
	void partitionupdates_coarsegrainedpipeline(
		bool_type enable,
		uint512_dt * kvdram,
		keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer_setof1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer_setof2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer_setof4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer_setof8[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so1[8][NUM_PARTITIONS],
		skeyvalue_t templocalcapsule_so2[4][NUM_PARTITIONS],
		skeyvalue_t templocalcapsule_so4[2][NUM_PARTITIONS],
		skeyvalue_t templocalcapsule_so8[NUM_PARTITIONS],
		keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ],
		config_t config,
		globalparams_t globalparams,
		sweepparams_t sweepparams,
		travstate_t ptravstate,
		batch_type sourcebaseaddr_kvs,
		batch_type destbaseaddr_kvs);
		
	void partitionupdates_finegrainedpipeline_unevencutoffs(
		bool_type enable,
		uint512_dt * kvdram,
		keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer_setof1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer_setof2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer_setof4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer_setof8[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so1[8][NUM_PARTITIONS],
		skeyvalue_t templocalcapsule_so2[4][NUM_PARTITIONS],
		skeyvalue_t templocalcapsule_so4[2][NUM_PARTITIONS],
		skeyvalue_t templocalcapsule_so8[NUM_PARTITIONS],
		keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ],
		config_t config,
		globalparams_t globalparams,
		sweepparams_t sweepparams,
		travstate_t ptravstate,
		batch_type sourcebaseaddr_kvs,
		batch_type destbaseaddr_kvs);
		
	void partitionupdates_finegrainedpipeline_evencutoffs(
		bool_type enable,
		uint512_dt * kvdram,
		keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer_setof1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer_setof2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer_setof4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer_setof8[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so1[8][NUM_PARTITIONS],
		skeyvalue_t templocalcapsule_so2[4][NUM_PARTITIONS],
		skeyvalue_t templocalcapsule_so4[2][NUM_PARTITIONS],
		skeyvalue_t templocalcapsule_so8[NUM_PARTITIONS],
		keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ],
		config_t config,
		globalparams_t globalparams,
		sweepparams_t sweepparams,
		travstate_t ptravstate,
		batch_type sourcebaseaddr_kvs,
		batch_type destbaseaddr_kvs);
		
	// reduce phase
	travstate_t reduceupdates(
		bool_type enable,
		uint512_dt * kvdram,
		keyvalue_t verticesbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t keyvaluesbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t tempverticesbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t actvvs[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		config_t config,
		globalparams_t globalparams,
		sweepparams_t sweepparams,
		travstate_t rtravstate,
		travstate_t actvvstravstate,
		unsigned int local_source_partition);
		
	// process vertices
	void processallvertices(
		bool_type enable,
		uint512_dt * kvdram,
		keyvalue_t buffer1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer3[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer5[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer6[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer7[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t capsule_so1[8][NUM_PARTITIONS],
		skeyvalue_t capsule_so2[4][NUM_PARTITIONS],
		skeyvalue_t capsule_so4[2][NUM_PARTITIONS],
		skeyvalue_t capsule_so8[NUM_PARTITIONS],
		keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ], 
		config_t config,
		globalparams_t globalparams,
		sweepparams_t sweepparams,
		travstate_t avtravstate);
		
	buffer_type generateoffsets(
			uint512_dt * kvdram, 
			keyvalue_t actvvs[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
			keyvalue_t offsets[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
			travstate_t actvvtravstate, 
			globalparams_t globalparams);
			
	void processoffsets(
		uint512_dt * kvdram,
		keyvalue_t offsets[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t kvbuffer1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		#ifndef COMPACTEDGES
		keyvalue_t kvbuffer2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		#endif
		globalparams_t globalparams,
		batch_type offset_kvs,
		buffer_type actvvscount,
		travstate_t actvvtravstate,
		value_t * _buffersize_kvs,
		batch_type * _saveoffset_kvs
		#ifdef _DEBUGMODE_STATS
		,unsigned int * _edges_count
		,unsigned int * _edgesdstv_sum
		#endif
		);
	
	batch_type processactivevertices(
		bool_type enable,
		uint512_dt * kvdram,
		keyvalue_t actvvs[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t offsets[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		#ifndef COMPACTEDGES
		keyvalue_t buffer2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		#endif 
		travstate_t actvvtravstate,
		globalparams_t globalparams);
		
	void dispatch(uint512_dt * kvdram);
	
	void dispatch_processandpartitiononly(uint512_dt * kvdram, 
		keyvalue_t buffer1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
		keyvalue_t buffer4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
		keyvalue_t buffer5[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
		globalparams_t globalparams);
			
	void dispatch_processonly(
		uint512_dt * vdram,
uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11,uint512_dt * kvdram12,uint512_dt * kvdram13,uint512_dt * kvdram14,uint512_dt * kvdram15, 
keyvalue_t actvvs0[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t actvvs1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t actvvs2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t actvvs3[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t actvvs4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t actvvs5[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t actvvs6[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t actvvs7[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t actvvs8[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t actvvs9[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t actvvs10[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t actvvs11[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t actvvs12[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t actvvs13[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t actvvs14[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t actvvs15[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],  
keyvalue_t offsets0[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t offsets1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t offsets2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t offsets3[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t offsets4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t offsets5[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t offsets6[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t offsets7[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t offsets8[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t offsets9[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t offsets10[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t offsets11[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t offsets12[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t offsets13[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t offsets14[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t offsets15[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer10[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer11[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer12[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer13[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer14[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer15[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer16[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer17[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer18[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer19[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer110[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer111[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer112[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer113[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer114[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer115[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer20[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer21[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer22[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer23[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer24[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer25[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer26[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer27[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer28[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer29[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer210[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer211[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer212[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer213[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer214[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer215[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],		globalparams_t globalparams[NUMCOMPUTEUNITS], batch_type * _sourcestatsmarker, batch_type * _deststatsmarker, batch_type * _destoffset);

	void dispatch_partitiononly(
		uint512_dt * kvdram, 
		keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
		keyvalue_t buffer_setof1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
		keyvalue_t buffer_setof2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
		keyvalue_t buffer_setof4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
		keyvalue_t buffer_setof8[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
		globalparams_t * _globalparams,
		batch_type * _sourcestatsmarker,
		batch_type * _deststatsmarker,
		batch_type * _destoffset
		);
	
	void dispatch_reduceonly(uint512_dt * kvdram, globalparams_t globalparams);
	
	void dispatch_reduceonly(
			uint512_dt * kvdram,
			keyvalue_t tempverticesbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
			keyvalue_t vubufferpp0[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
			keyvalue_t vubufferpp1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
			globalparams_t globalparams);

	travstate_t dispatch_reduceonly_parallelsync(
			uint512_dt * vdram, 
uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11,uint512_dt * kvdram12,uint512_dt * kvdram13,uint512_dt * kvdram14,uint512_dt * kvdram15,keyvalue_t tempverticesbuffer0[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t tempverticesbuffer1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t tempverticesbuffer2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t tempverticesbuffer3[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t tempverticesbuffer4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t tempverticesbuffer5[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t tempverticesbuffer6[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t tempverticesbuffer7[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t tempverticesbuffer8[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t tempverticesbuffer9[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t tempverticesbuffer10[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t tempverticesbuffer11[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t tempverticesbuffer12[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t tempverticesbuffer13[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t tempverticesbuffer14[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t tempverticesbuffer15[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
keyvalue_t vubufferpp00[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t vubufferpp01[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t vubufferpp02[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t vubufferpp03[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t vubufferpp04[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t vubufferpp05[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t vubufferpp06[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t vubufferpp07[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t vubufferpp08[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t vubufferpp09[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t vubufferpp010[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t vubufferpp011[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t vubufferpp012[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t vubufferpp013[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t vubufferpp014[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t vubufferpp015[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
keyvalue_t vubufferpp10[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t vubufferpp11[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t vubufferpp12[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t vubufferpp13[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t vubufferpp14[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t vubufferpp15[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t vubufferpp16[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t vubufferpp17[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t vubufferpp18[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t vubufferpp19[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t vubufferpp110[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t vubufferpp111[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t vubufferpp112[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t vubufferpp113[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t vubufferpp114[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t vubufferpp115[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],			travstate_t actvvstravstate, globalparams_t globalparams[NUMSUBCPUTHREADS]);

	#ifndef MULTIACTSINSTANCES
	void topkernel(uint512_dt * kvdram);
	#endif 
	
	#if defined(MULTIACTSINSTANCES)
	void topkernel(uint512_dt * vdram ,uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11,uint512_dt * kvdram12,uint512_dt * kvdram13,uint512_dt * kvdram14,uint512_dt * kvdram15);
	#endif 
private:
	#ifndef FPGA_IMPL
	actsutility * actsutilityobj;
	#endif
};
#endif 






