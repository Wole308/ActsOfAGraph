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

// #define HYBRIDACCESSMODE

#define PROCESSMODE 0
#define PARTITIONMODE 1
#define REDUCEMODE 2

#define PROCFETCHINDEX 2//2

class acts {
public:
	acts();
	~acts();
	
	// functions (utility)
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
	batch_type getrangeforeachllop(globalparams_t globalparams);
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
	buffer_type getwritesz1(buffer_type realsize_kvs);
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
	sweepparams_t updatesweepparams(sweepparams_t sweepparams, step_type currentLOP, batch_type source_partition, batch_type sourceskipsize, batch_type llopsz, globalparams_t globalparams);
	travstate_t gettravstate(bool_type enable, uint512_dt * kvdram, globalparams_t globalparams, step_type currentLOP, batch_type sourcestatsmarker, keyvalue_t moretravstates[LOADFACTORFORREDUCE]);
	travstate_t updatetravstate(bool_type enable, keyvalue_t travstatekv, step_type currentLOP, globalparams_t globalparams);
	bool inprocessedgesstage(unsigned int currentLOP);
	bool incollectstatsstage(unsigned int currentLOP, globalparams_t globalparams);
	bool inpartitionstage(unsigned int currentLOP, globalparams_t globalparams);
	bool inreducestage(unsigned int currentLOP, globalparams_t globalparams);
	void calculateglobaloffsets(keyvalue_t * globalstatsbuffer, batch_type * skipsizes,  batch_type offset, batch_type size);
	keyvalue_t getkeyvalue(uint512_dt * keyvalues, unsigned int offset_kvs, vector_type v);
	uint512_vec_dt getkeyvalues(uint512_dt * keyvalues, unsigned int offset_kvs);
	unsigned int getdata(keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], unsigned int index);
	void setkeyvalue(uint512_dt * keyvalues, unsigned int offset_kvs, vector_type v, keyvalue_t keyvalue);
	void setkeyvalues(uint512_dt * keyvalues, unsigned int offset_kvs, uint512_vec_dt D);
	void setkey(uint512_dt * keyvalues, unsigned int offset_kvs, vector_type v, keyy_t key);
	unsigned int fillintheblancks(keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], unsigned int index);
	unsigned int gethybridthreshold(globalparams_t globalparams);
	void dummywrite(keyvalue_t buffer0[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t buffer1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], unsigned int dummyx, unsigned int dummyy);
	partition_type getpartition(bool_type enable, keyvalue_t keyvalue, step_type currentLOP, vertex_t upperlimit, unsigned int upperpartition, unsigned int batch_range_pow);

	// functions (dram access)
	void readkeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], batch_type offset_kvs, batch_type size_kvs, travstate_t travstate);
	
	void savekeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_t buffer[8][PADDEDDESTBUFFER_SIZE], keyvalue_t * globalcapsule, skeyvalue_t localcapsule[NUM_PARTITIONS], batch_type globalbaseaddress_kvs, globalparams_t globalparams);
	
	void readkeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], batch_type offset_kvs, buffer_type size_kvs);

	void savekeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], batch_type offset_kvs, buffer_type size_kvs);
	
	void readkeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_t buffer[NUM_KVDRAMBUFFERS][PADDEDDESTBUFFER_SIZE], unsigned int startv, batch_type offset_kvs, buffer_type size_kvs);
	
	void readkeyvalues2(bool_type enable, uint512_dt * kvdram, keyvalue_t buffer[NUM_KVDRAMBUFFERS][PADDEDDESTBUFFER_SIZE], batch_type offset1_kvs, batch_type offset2_kvs, buffer_type size1_kvs, buffer_type size2_kvs);
	
	void savekeyvalues2(bool_type enable, uint512_dt * kvdram, keyvalue_t buffer[NUM_KVDRAMBUFFERS][PADDEDDESTBUFFER_SIZE], batch_type offset1_kvs, batch_type offset2_kvs, buffer_type size1_kvs, buffer_type size2_kvs);
	
	// function (actit)
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
		
	// functions (process)
	value_t processfunc(value_t udata, value_t edgew, unsigned int GraphAlgo);
	
	void readandprocess(bool_type enable, uint512_dt * kvdram, keyvalue_t kvdrambuffer[NUM_KVDRAMBUFFERS][PADDEDDESTBUFFER_SIZE], keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, globalparams_t globalparams);

	void replicatevdata(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t kvdrambuffer[NUM_KVDRAMBUFFERS][PADDEDDESTBUFFER_SIZE], buffer_type sourceoffset, buffer_type size);
	
	buffer_type generateoffsets_random(
		uint512_dt * kvdram, 
		keyvalue_t actvvs[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t offsets[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
		travstate_t actvvtravstate, globalparams_t globalparams, batch_type actvvsoffset_kvs);
			
	buffer_type generateoffsets_stream(
		uint512_dt * kvdram, 
		keyvalue_t vptrs[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t offsets[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
		travstate_t actvvtravstate, globalparams_t globalparams, batch_type actvvsoffset_kvs);
		
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
		batch_type * _saveoffset_kvs,
		batch_type * _cachebeginoffset_kvs
		#ifdef _DEBUGMODE_STATS
		,unsigned int * _edges_count
		,unsigned int * _edgesdstv_sum
		#endif
		);
		
	// functions (partition)
	void readglobalstats(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ], batch_type offset_kvs);

	void collectglobalstats(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], step_type currentLOP, sweepparams_t sweepparams, travstate_t travstate, globalparams_t globalparams);

	void prepareglobalstats(bool_type enable, keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ], globalparams_t globalparams);

	void saveglobalstats(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ], batch_type offset_kvs);
	
	void collectstats(bool_type enable, uint512_dt * kvdram, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ], config_t config, globalparams_t globalparams, sweepparams_t sweepparams, travstate_t ctravstate);
	
	buffer_type preparekeyvalues(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t localcapsule[VECTOR_SIZE][NUM_PARTITIONS], step_type currentLOP, sweepparams_t sweepparams, travstate_t travstate, buffer_type size_kvs, buffer_type cutoffs[VECTOR_SIZE], globalparams_t globalparams);
	
	void partitionpipeline(
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
		globalparams_t globalparams,
		sweepparams_t sweepparams,
		travstate_t ptravstate,
		batch_type sourcebaseaddr_kvs,
		batch_type destbaseaddr_kvs);
		
	// functions (reduce)
	void replicatedata_syn(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer0[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer3[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer5[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer6[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer7[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer8[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer9[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer10[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer11[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer12[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer13[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer14[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer15[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], buffer_type sourceoffset, buffer_type size);

	void replicatedata_syn(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE] ,keyvalue_t destbuffer0[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer3[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer5[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer6[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer7[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer8[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer9[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer10[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer11[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer12[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer13[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer14[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t destbuffer15[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE]);
	
	void replicatedata_bfs_syn(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_at destbuffer10[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer20[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_at destbuffer11[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer21[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_at destbuffer12[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer22[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_at destbuffer13[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer23[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_at destbuffer14[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer24[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_at destbuffer15[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer25[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_at destbuffer16[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer26[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_at destbuffer17[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer27[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_at destbuffer18[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer28[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_at destbuffer19[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer29[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_at destbuffer110[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer210[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_at destbuffer111[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer211[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_at destbuffer112[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer212[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_at destbuffer113[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer213[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_at destbuffer114[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer214[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_at destbuffer115[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer215[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],  buffer_type sourceoffset, buffer_type size);
	
	value_t reducefunc(value_t vtemp, value_t res, unsigned int GraphIter, unsigned int GraphAlgo);
	
	value_t reducefunc_bfs(value_t vtemp, value_t res, unsigned int GraphIter, unsigned int GraphAlgo);
	
	void reduce(bool_type enable1, bool_type enable2, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], sweepparams_t sweepparams, unsigned int GraphIter, unsigned int GraphAlgo, travstate_t travstate, globalparams_t globalparams);
	
	void reduce_bfs(bool_type enable1, bool_type enable2, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_at destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], sweepparams_t sweepparams, unsigned int GraphIter, unsigned int GraphAlgo, travstate_t travstate, globalparams_t globalparams);

	void reduce_sssp(bool_type enable1, bool_type enable2, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], sweepparams_t sweepparams, unsigned int GraphIter, unsigned int GraphAlgo, travstate_t travstate, globalparams_t globalparams);

	void reducevector(keyvalue_t kvdata, keyvalue_t destbuffer[PADDEDDESTBUFFER_SIZE], unsigned int upperlimit, sweepparams_t sweepparams, globalparams_t globalparams);
	
	value_t mergefunc(value_t value1, value_t value2, unsigned int GraphAlgo);
	
	keyvalue_t mergefunc_bfs(keyvalue_t data1, keyvalue_t data2, unsigned int GraphAlgo);
	
	void unifydata(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], buffer_type destoffset, buffer_type size, unsigned int GraphAlgo);

	void unifydata_pr_parallelsyn(bool_type enable, keyvalue_t sourcebuffer0[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer3[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer5[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer6[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer7[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer8[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer9[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer10[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer11[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer12[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer13[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer14[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer15[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
					buffer_type destoffset, buffer_type size, sweepparams_t sweepparams, globalparams_t globalparams);
					
	void unifydata_bfs_parallelsyn(bool_type enable, keyvalue_at sourcebuffer0[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_at sourcebuffer1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_at sourcebuffer2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_at sourcebuffer3[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_at sourcebuffer4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_at sourcebuffer5[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_at sourcebuffer6[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_at sourcebuffer7[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_at sourcebuffer8[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_at sourcebuffer9[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_at sourcebuffer10[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_at sourcebuffer11[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_at sourcebuffer12[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_at sourcebuffer13[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_at sourcebuffer14[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_at sourcebuffer15[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
					buffer_type destoffset, buffer_type size, globalparams_t globalparams);
					
	bool_type unifydata_sssp_parallelsyn(bool_type enable, keyvalue_t sourcebuffer0[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer3[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer5[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer6[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer7[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer8[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer9[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer10[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer11[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer12[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer13[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer14[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t sourcebuffer15[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
					keyvalue_t actvvs[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], buffer_type * actvvtracker,
					buffer_type destoffset, buffer_type size, sweepparams_t sweepparams, globalparams_t globalparams);
	
	bool_type collectactvvs_bfs(uint512_dt * vdram, bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
					bool_type itercount_actvvs0,
					bool_type itercount_actvvs1,
					bool_type enableflush, unsigned int index, travstate_t * actvvstravstate, 
					buffer_type destoffset, buffer_type size, sweepparams_t sweepparams, globalparams_t globalparams);
					
	void reducebufferpartitions(bool_type enable, keyvalue_t kvdrambuffer[NUM_KVDRAMBUFFERS][PADDEDDESTBUFFER_SIZE], keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t localcapsule[NUM_PARTITIONS], sweepparams_t sweepparams, globalparams_t globalparams);
		
	void reducepartition(
		bool_type enablereduce, 
		keyvalue_t sourcevbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11,uint512_dt * kvdram12,uint512_dt * kvdram13,uint512_dt * kvdram14,uint512_dt * kvdram15,keyvalue_at tempverticesbuffer0[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_at tempverticesbuffer1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_at tempverticesbuffer2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_at tempverticesbuffer3[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_at tempverticesbuffer4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_at tempverticesbuffer5[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_at tempverticesbuffer6[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_at tempverticesbuffer7[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_at tempverticesbuffer8[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_at tempverticesbuffer9[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_at tempverticesbuffer10[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_at tempverticesbuffer11[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_at tempverticesbuffer12[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_at tempverticesbuffer13[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_at tempverticesbuffer14[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_at tempverticesbuffer15[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],		unsigned int indexpp0, unsigned int reducesubchunksz,
		travstate_t rtravstate[NUMSUBCPUTHREADS], sweepparams_t sweepparams, globalparams_t _globalparams);
	
	// actit 
	void resetenvbuffers(skeyvalue_t capsule_so1[8][NUM_PARTITIONS], skeyvalue_t capsule_so2[4][NUM_PARTITIONS], skeyvalue_t capsule_so4[2][NUM_PARTITIONS], skeyvalue_t capsule_so8[NUM_PARTITIONS]);

	void fetchkeyvalues(bool_type enable, unsigned int mode, uint512_dt * kvdram, keyvalue_t kvdrambuffer[NUM_KVDRAMBUFFERS][PADDEDDESTBUFFER_SIZE], keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, globalparams_t globalparams);
	
	void commitkeyvalues(bool_type enable, unsigned int mode, uint512_dt * kvdram, keyvalue_t kvdrambuffer[NUM_KVDRAMBUFFERS][PADDEDDESTBUFFER_SIZE], keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t globalcapsule[GLOBALSTATSBUFFERSZ], skeyvalue_t localcapsule[NUM_PARTITIONS], batch_type destbaseaddr_kvs, sweepparams_t sweepparams, globalparams_t globalparams);
	
	void actit(bool_type enable, unsigned int mode, bool_type resetenv,
		uint512_dt * kvdram, keyvalue_t kvdrambuffer[NUM_KVDRAMBUFFERS][PADDEDDESTBUFFER_SIZE], keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ],
		globalparams_t globalparams, sweepparams_t sweepparams, travstate_t ptravstate, batch_type sourcebaseaddr_kvs, batch_type destbaseaddr_kvs);

	// dispatches (type 1)
	void processit(uint512_dt * kvdram, keyvalue_t kvdrambuffer[NUM_KVDRAMBUFFERS][PADDEDDESTBUFFER_SIZE], globalparams_t globalparams);
	
	void processit_v2(uint512_dt * kvdram, keyvalue_t kvdrambuffer[NUM_KVDRAMBUFFERS][PADDEDDESTBUFFER_SIZE], globalparams_t globalparams);

	void partitionit(uint512_dt * kvdram, keyvalue_t kvdrambuffer[NUM_KVDRAMBUFFERS][PADDEDDESTBUFFER_SIZE], globalparams_t globalparams);
		
	void reduceit(uint512_dt * kvdram, keyvalue_t kvdrambuffer[NUM_KVDRAMBUFFERS][PADDEDDESTBUFFER_SIZE], globalparams_t globalparams);

	void reduceit(uint512_dt * kvdram, keyvalue_t kvdrambuffer[NUM_KVDRAMBUFFERS][PADDEDDESTBUFFER_SIZE], batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparams);

	void dispatch(uint512_dt * kvdram, keyvalue_t kvdrambuffer[NUM_KVDRAMBUFFERS][PADDEDDESTBUFFER_SIZE], globalparams_t globalparams);
	
	void dispatch(bool_type en_process, bool_type en_partition, bool_type en_reduce, uint512_dt * kvdram, keyvalue_t kvdrambuffer[NUM_KVDRAMBUFFERS][PADDEDDESTBUFFER_SIZE],
				batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparams);
	
	travstate_t start_reduce(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11,uint512_dt * kvdram12,uint512_dt * kvdram13,uint512_dt * kvdram14,uint512_dt * kvdram15, keyvalue_t kvdrambuffer0[NUM_KVDRAMBUFFERS][PADDEDDESTBUFFER_SIZE],keyvalue_t kvdrambuffer1[NUM_KVDRAMBUFFERS][PADDEDDESTBUFFER_SIZE],keyvalue_t kvdrambuffer2[NUM_KVDRAMBUFFERS][PADDEDDESTBUFFER_SIZE],keyvalue_t kvdrambuffer3[NUM_KVDRAMBUFFERS][PADDEDDESTBUFFER_SIZE],keyvalue_t kvdrambuffer4[NUM_KVDRAMBUFFERS][PADDEDDESTBUFFER_SIZE],keyvalue_t kvdrambuffer5[NUM_KVDRAMBUFFERS][PADDEDDESTBUFFER_SIZE],keyvalue_t kvdrambuffer6[NUM_KVDRAMBUFFERS][PADDEDDESTBUFFER_SIZE],keyvalue_t kvdrambuffer7[NUM_KVDRAMBUFFERS][PADDEDDESTBUFFER_SIZE],keyvalue_t kvdrambuffer8[NUM_KVDRAMBUFFERS][PADDEDDESTBUFFER_SIZE],keyvalue_t kvdrambuffer9[NUM_KVDRAMBUFFERS][PADDEDDESTBUFFER_SIZE],keyvalue_t kvdrambuffer10[NUM_KVDRAMBUFFERS][PADDEDDESTBUFFER_SIZE],keyvalue_t kvdrambuffer11[NUM_KVDRAMBUFFERS][PADDEDDESTBUFFER_SIZE],keyvalue_t kvdrambuffer12[NUM_KVDRAMBUFFERS][PADDEDDESTBUFFER_SIZE],keyvalue_t kvdrambuffer13[NUM_KVDRAMBUFFERS][PADDEDDESTBUFFER_SIZE],keyvalue_t kvdrambuffer14[NUM_KVDRAMBUFFERS][PADDEDDESTBUFFER_SIZE],keyvalue_t kvdrambuffer15[NUM_KVDRAMBUFFERS][PADDEDDESTBUFFER_SIZE], globalparams_t globalparams[NUMSUBCPUTHREADS]);

	travstate_t start_reduce2(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11,uint512_dt * kvdram12,uint512_dt * kvdram13,uint512_dt * kvdram14,uint512_dt * kvdram15, travstate_t actvvstravstate, globalparams_t globalparams[NUMSUBCPUTHREADS]);
	
	void start(uint512_dt * kvdram, globalparams_t globalparams);
	
	void start(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11,uint512_dt * kvdram12,uint512_dt * kvdram13,uint512_dt * kvdram14,uint512_dt * kvdram15, globalparams_t globalparams[NUMSUBCPUTHREADS]);
	
	// dispatches (type 2)
	void processit_sync(
		uint512_dt * vdram,
uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11,uint512_dt * kvdram12,uint512_dt * kvdram13,uint512_dt * kvdram14,uint512_dt * kvdram15,keyvalue_t offsets0[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t offsets1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t offsets2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t offsets3[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t offsets4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t offsets5[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t offsets6[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t offsets7[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t offsets8[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t offsets9[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t offsets10[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t offsets11[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t offsets12[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t offsets13[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t offsets14[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t offsets15[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],	
keyvalue_t kvbuffer10[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer11[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer12[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer13[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer14[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer15[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer16[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer17[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer18[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer19[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer110[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer111[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer112[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer113[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer114[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer115[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],	
		#ifndef COMPACTEDGES	
keyvalue_t kvbuffer20[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer21[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer22[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer23[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer24[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer25[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer26[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer27[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer28[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer29[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer210[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer211[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer212[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer213[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer214[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],keyvalue_t kvbuffer215[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],	
		#endif	 
		globalparams_t globalparams[NUMCOMPUTEUNITS], batch_type * _sourcestatsmarker, batch_type * _deststatsmarker, batch_type * _destoffset);

	void partitionit2(
		uint512_dt * kvdram, 
		keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
		keyvalue_t buffer_setof1[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
		keyvalue_t buffer_setof2[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
		keyvalue_t buffer_setof4[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
		keyvalue_t buffer_setof8[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], 
		globalparams_t globalparams,
		batch_type * _sourcestatsmarker,
		batch_type * _deststatsmarker,
		batch_type * _destoffset);
			
	#if defined(MULTIACTSINSTANCES)
	void topkernel(uint512_dt * vdram ,uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11,uint512_dt * kvdram12,uint512_dt * kvdram13,uint512_dt * kvdram14,uint512_dt * kvdram15);
	#endif 
private:
	#ifndef FPGA_IMPL
	actsutility * actsutilityobj;
	#endif
};
#endif 






