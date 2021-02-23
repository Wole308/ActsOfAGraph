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

#define PROCESSMODE 0
#define PARTITIONMODE 1
#define REDUCEMODE 2

#define NUM_VBUFFERS NUM_PARTITIONS

// #define _DEBUGMODE_KERNELPRINTS_UNKNOWN //

#define SHIFTSYNCHRONIZE

// #define AUTOMATEMODEON //

class acts {
public:
	acts();
	~acts();
	
	// functions (basic)
	unsigned int amin(unsigned int val1, unsigned int val2);
	unsigned int aplus(unsigned int val1, unsigned int val2);
	
	// functions (allignment)
	batch_type allignlower_KV(batch_type val);
	batch_type allignhigher_KV(batch_type val);
	
	// functions (converters)
	ulong_dt CONVERTTOLONG_KV(keyvalue_t keyvalue);
	keyvalue_t CONVERTTOKV_ULONG(ulong_dt data);
	uint32_type convertvmasktouint32(uintNUMPby2_type vmask[BLOCKRAM_SIZE], unsigned int index);
	
	// functions (bit manipulation)
	unsigned int GETMASK_UINT(unsigned int index, unsigned int size);
	ulong_dt GETMASK_ULONG(ulong_dt index, ulong_dt size);
	unsigned int READFROM_UINT(unsigned int data, unsigned int index, unsigned int size);
	void WRITETO_UINT(unsigned int * data, unsigned int index, unsigned int size, unsigned int value);
	unsigned int READFROM_ULONG(ulong_dt data, ulong_dt index, ulong_dt size);
	unsigned int READFROM_ULONG(keyvalue_t keyvalue, ulong_dt index, ulong_dt size);
	void WRITETO_ULONG(ulong_dt * data, ulong_dt index, ulong_dt size, ulong_dt value);
	void WRITETO_ULONG(keyvalue_t * keyvalue, ulong_dt index, ulong_dt size, ulong_dt value);
	
	// functions (acts utilities)
	batch_type getskipsize(step_type currentLOP, bool_type sourceORdest, globalparams_t globalparams);
	batch_type getrangeforeachllop(globalparams_t globalparams);
	buffer_type getchunksize_kvs(buffer_type buffer_size, travstate_t travstate, buffer_type localoffset);
	buffer_type getpartitionwritesz(buffer_type realsize_kvs, buffer_type bramoffset_kvs);
	void calculatemanyunallignedoffsets(keyvalue_t buffer[VECTOR_SIZE][NUM_PARTITIONS], buffer_type size, batch_type base, batch_type skipspacing);
	batch_type get_num_source_partitions(step_type currentLOP);
	globalparams_t getglobalparams(uint512_dt * kvdram);
	sweepparams_t getsweepparams(globalparams_t globalparams, step_type currentLOP, batch_type source_partition);
	travstate_t gettravstate(bool_type enable, uint512_dt * kvdram, globalparams_t globalparams, step_type currentLOP, batch_type sourcestatsmarker);
	partition_type getpartition(bool_type enable, keyvalue_t keyvalue, step_type currentLOP, vertex_t upperlimit, unsigned int upperpartition, unsigned int batch_range_pow);

	// functions (resets)
	void resetvalues(keyvalue_t * buffer, buffer_type size, unsigned int resetval);
	void resetvalues(value_t * buffer, buffer_type size, unsigned int resetval);
	void resetkeysandvalues(keyvalue_t buffer[VECTOR_SIZE][NUM_PARTITIONS], buffer_type size, unsigned int resetval);
	void resetkeysandvalues(keyvalue_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], buffer_type size, unsigned int resetval);
	void resetkeysandvalues(keyvalue_t * buffer, buffer_type size, unsigned int resetval);
	void resetvmask(uintNUMPby2_type vmask[BLOCKRAM_SIZE]);
	
	// functions (accessors)
	void readkeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs, travstate_t travstate);
	
	void savekeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_t buffer[8][BLOCKRAM_SIZE], keyvalue_t * globalcapsule, keyvalue_t localcapsule[NUM_PARTITIONS], batch_type globalbaseaddress_kvs, globalparams_t globalparams);
	
	void readkeyvalues(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], batch_type bufferoffset_kvs, buffer_type size_kvs);

	void savekeyvalues(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], batch_type bufferoffset_kvs, buffer_type size_kvs);
	
	void readkeyvalues(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_t buffer1[VECTOR_SIZE][BLOCKRAM_SIZE], batch_type buffer1offset_kvs, keyvalue_t buffer2[VECTOR_SIZE][BLOCKRAM_SIZE], batch_type buffer2offset_kvs, buffer_type size_kvs);

	void loadvdata(bool_type enable, uint512_dt * kvdram, keyvalue_t vdata[NUM_VBUFFERS][BLOCKRAM_SIZE], batch_type offset1_kvs, buffer_type size1_kvs, batch_type offset2_kvs, buffer_type size2_kvs);
	
	void savevdata(bool_type enable, uint512_dt * kvdram, keyvalue_t vdata[NUM_VBUFFERS][BLOCKRAM_SIZE], batch_type offset1_kvs, buffer_type size1_kvs, batch_type offset2_kvs, buffer_type size2_kvs);
	
	void loadvmasks(bool_type enable, uint512_dt * kvdram, uintNUMPby2_type vmask[BLOCKRAM_SIZE], keyvalue_t tempbuffer[VECTOR_SIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);

	void savevmasks(bool_type enable, uint512_dt * kvdram, uintNUMPby2_type vmask[BLOCKRAM_SIZE], keyvalue_t tempbuffer[VECTOR_SIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);

	value_t readfromvbuffer(keyvalue_t vbuffer1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t vbuffer2[VECTOR_SIZE][BLOCKRAM_SIZE], unsigned int loc, globalparams_t globalparams);

	value_t readfromvmask(uintNUMPby2_type vmask[BLOCKRAM_SIZE], unsigned int loc, globalparams_t globalparams);

	// functions (process)
	value_t processfunc(value_t udata, value_t edgew, unsigned int GraphAlgo);
	
	bool_type readandprocess(bool_type enable, uint512_dt * kvdram, keyvalue_t vbuffer1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t vbuffer2[VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask[BLOCKRAM_SIZE], keyvalue_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], 
			batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, sweepparams_t sweepparams, globalparams_t globalparams);

	// functions (partition)
	void readglobalstats(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[NUM_PARTITIONS], batch_type offset_kvs);

	void saveglobalstats(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[NUM_PARTITIONS], batch_type offset_kvs);
	
	buffer_type preparekeyvalues(bool_type enable1, bool_type enable2, keyvalue_t sourcebuffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t localcapsule[VECTOR_SIZE][NUM_PARTITIONS], step_type currentLOP, sweepparams_t sweepparams, travstate_t travstate, buffer_type size_kvs, buffer_type cutoffs[VECTOR_SIZE], globalparams_t globalparams);

	// functions (reduce)
	value_t reducefunc(value_t vtemp, value_t res, unsigned int GraphIter, unsigned int GraphAlgo);
	
	value_t applyfunc(value_t vtemp, value_t res, unsigned int GraphIter, unsigned int GraphAlgo);

	void reducevector(keyvalue_t kvdata, 
			keyvalue_t destbuffer[BLOCKRAM_SIZE], 
			buffer_type destoffset, unsigned int upperlimit, sweepparams_t sweepparams, globalparams_t globalparams);

	void reduceandbuffer(bool_type enable, keyvalue_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t localcapsule[NUM_PARTITIONS], keyvalue_t vbuffer1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t vbuffer2[VECTOR_SIZE][BLOCKRAM_SIZE], sweepparams_t sweepparams, globalparams_t globalparams);
	
	// synchronize
	value_t mergefunc(value_t value1, value_t value2, unsigned int GraphAlgo);
	
	value_t mergefunc_NUMPto1(value_t v0,value_t v1,value_t v2,value_t v3,value_t v4,value_t v5,value_t v6,value_t v7,value_t v8,value_t v9,value_t v10,value_t v11, unsigned int GraphAlgo);
	
	uint32_type synchronize(bool_type enable, keyvalue_t synvbuffer_1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t synvbuffer_2[VECTOR_SIZE][BLOCKRAM_SIZE], 
keyvalue_t vbuffer0_1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t vbuffer0_2[VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask0[BLOCKRAM_SIZE],keyvalue_t vbuffer1_1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t vbuffer1_2[VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask1[BLOCKRAM_SIZE],keyvalue_t vbuffer2_1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t vbuffer2_2[VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask2[BLOCKRAM_SIZE],keyvalue_t vbuffer3_1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t vbuffer3_2[VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask3[BLOCKRAM_SIZE],keyvalue_t vbuffer4_1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t vbuffer4_2[VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask4[BLOCKRAM_SIZE],keyvalue_t vbuffer5_1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t vbuffer5_2[VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask5[BLOCKRAM_SIZE],keyvalue_t vbuffer6_1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t vbuffer6_2[VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask6[BLOCKRAM_SIZE],keyvalue_t vbuffer7_1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t vbuffer7_2[VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask7[BLOCKRAM_SIZE],keyvalue_t vbuffer8_1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t vbuffer8_2[VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask8[BLOCKRAM_SIZE],keyvalue_t vbuffer9_1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t vbuffer9_2[VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask9[BLOCKRAM_SIZE],keyvalue_t vbuffer10_1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t vbuffer10_2[VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask10[BLOCKRAM_SIZE],keyvalue_t vbuffer11_1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t vbuffer11_2[VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask11[BLOCKRAM_SIZE], 
			globalparams_t globalparams);

	void shiftsynchronize1(bool_type enable, keyvalue_t head[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t buffer0[VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_t buffer1[VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_t buffer2[VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_t buffer3[VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_t buffer4[VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_t buffer5[VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_t buffer6[VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_t buffer7[VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_t buffer8[VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_t buffer9[VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_t buffer10[VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_t buffer11[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t tail[VECTOR_SIZE][BLOCKRAM_SIZE], globalparams_t globalparams);
	
	void shiftsynchronize2(bool_type enable, keyvalue_t head[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t buffer0[VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_t buffer1[VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_t buffer2[VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_t buffer3[VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_t buffer4[VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_t buffer5[VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_t buffer6[VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_t buffer7[VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_t buffer8[VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_t buffer9[VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_t buffer10[VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_t buffer11[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t tail[VECTOR_SIZE][BLOCKRAM_SIZE], globalparams_t globalparams);
	
	uint32_type apply(bool_type enable, 
		keyvalue_t vbuffer_1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t vbuffer_2[VECTOR_SIZE][BLOCKRAM_SIZE],
		keyvalue_t synvbuffer_1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t synvbuffer_2[VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask[BLOCKRAM_SIZE],
		batch_type voffset_kvs, globalparams_t globalparams);
		
	void spreadvdata1(bool_type enable, keyvalue_t head[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t buffer0[VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_t buffer1[VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_t buffer2[VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_t buffer3[VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_t buffer4[VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_t buffer5[VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_t buffer6[VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_t buffer7[VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_t buffer8[VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_t buffer9[VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_t buffer10[VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_t buffer11[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t tail[VECTOR_SIZE][BLOCKRAM_SIZE], globalparams_t globalparams);

	void spreadvdata2(bool_type enable, keyvalue_t head[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t buffer0[VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_t buffer1[VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_t buffer2[VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_t buffer3[VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_t buffer4[VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_t buffer5[VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_t buffer6[VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_t buffer7[VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_t buffer8[VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_t buffer9[VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_t buffer10[VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_t buffer11[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t tail[VECTOR_SIZE][BLOCKRAM_SIZE], globalparams_t globalparams);

	void spreadvmask1(bool_type enable, uintNUMPby2_type vmask[BLOCKRAM_SIZE], uintNUMPby2_type vmask0[BLOCKRAM_SIZE],uintNUMPby2_type vmask1[BLOCKRAM_SIZE],uintNUMPby2_type vmask2[BLOCKRAM_SIZE],uintNUMPby2_type vmask3[BLOCKRAM_SIZE],uintNUMPby2_type vmask4[BLOCKRAM_SIZE],uintNUMPby2_type vmask5[BLOCKRAM_SIZE],uintNUMPby2_type vmask6[BLOCKRAM_SIZE],uintNUMPby2_type vmask7[BLOCKRAM_SIZE],uintNUMPby2_type vmask8[BLOCKRAM_SIZE],uintNUMPby2_type vmask9[BLOCKRAM_SIZE],uintNUMPby2_type vmask10[BLOCKRAM_SIZE],uintNUMPby2_type vmask11[BLOCKRAM_SIZE], globalparams_t globalparams);
		
	void spreadvmask2(bool_type enable, uintNUMPby2_type vmask[BLOCKRAM_SIZE], uintNUMPby2_type vmask0[BLOCKRAM_SIZE],uintNUMPby2_type vmask1[BLOCKRAM_SIZE],uintNUMPby2_type vmask2[BLOCKRAM_SIZE],uintNUMPby2_type vmask3[BLOCKRAM_SIZE],uintNUMPby2_type vmask4[BLOCKRAM_SIZE],uintNUMPby2_type vmask5[BLOCKRAM_SIZE],uintNUMPby2_type vmask6[BLOCKRAM_SIZE],uintNUMPby2_type vmask7[BLOCKRAM_SIZE],uintNUMPby2_type vmask8[BLOCKRAM_SIZE],uintNUMPby2_type vmask9[BLOCKRAM_SIZE],uintNUMPby2_type vmask10[BLOCKRAM_SIZE],uintNUMPby2_type vmask11[BLOCKRAM_SIZE], globalparams_t globalparams);
	
	// acts
	void resetenvbuffers(keyvalue_t capsule_so1[8][NUM_PARTITIONS], keyvalue_t capsule_so2[4][NUM_PARTITIONS], keyvalue_t capsule_so4[2][NUM_PARTITIONS], keyvalue_t capsule_so8[NUM_PARTITIONS]);

	bool_type fetchkeyvalues(bool_type enable, unsigned int mode, uint512_dt * kvdram, keyvalue_t vbuffer1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t vbuffer2[VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask[BLOCKRAM_SIZE], keyvalue_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], 
			batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, sweepparams_t sweepparams, globalparams_t globalparams);
	
	void commitkeyvalues(bool_type enable1, bool_type enable2, unsigned int mode, uint512_dt * kvdram, keyvalue_t vbuffer1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t vbuffer2[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t globalcapsule[NUM_PARTITIONS], keyvalue_t localcapsule[NUM_PARTITIONS], 
			batch_type destbaseaddr_kvs, sweepparams_t sweepparams, globalparams_t globalparams);
	
	void actspipeline(bool_type enable1, bool_type enable2, keyvalue_t bufferA[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t buffer1capsule[VECTOR_SIZE][NUM_PARTITIONS], 
						keyvalue_t bufferB[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t bufferBcapsule[4][NUM_PARTITIONS], 
							keyvalue_t bufferC[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t bufferCcapsule[2][NUM_PARTITIONS],
								keyvalue_t bufferD[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t bufferDcapsule[NUM_PARTITIONS],
									unsigned int currentLOP, sweepparams_t sweepparams, buffer_type cutoff, buffer_type cutoffs[VECTOR_SIZE], batch_type shiftcount, globalparams_t globalparams);

	void actit(bool_type enable, unsigned int mode,
			uint512_dt * kvdram, keyvalue_t vbuffer1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t vbuffer2[VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask[BLOCKRAM_SIZE], keyvalue_t globalstatsbuffer[NUM_PARTITIONS], 
			globalparams_t globalparams, sweepparams_t sweepparams, travstate_t ptravstate, batch_type sourcebaseaddr_kvs, batch_type destbaseaddr_kvs,
			bool_type resetenv, bool_type flush);

	// dispatches (sync 1)
	void processit(uint512_dt * kvdram, keyvalue_t vbuffer1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t vbuffer2[VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask[BLOCKRAM_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], globalparams_t globalparams);

	void partitionit(uint512_dt * kvdram, keyvalue_t vbuffer1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t vbuffer2[VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask[BLOCKRAM_SIZE], globalparams_t globalparams);
		
	void reduceit(uint512_dt * kvdram, keyvalue_t vbuffer1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t vbuffer2[VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask[BLOCKRAM_SIZE], batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparams);

	void dispatch(bool_type en_process, bool_type en_partition, bool_type en_reduce, uint512_dt * kvdram, keyvalue_t vbuffer1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t vbuffer2[VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask[BLOCKRAM_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE],
			batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparams);
	
	void start_reduce(uint512_dt * kvdram0, keyvalue_t vbuffer0_1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t vbuffer0_2[VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask0[BLOCKRAM_SIZE], uint32_type vmask_p0[BLOCKRAM_SIZE],uint512_dt * kvdram1, keyvalue_t vbuffer1_1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t vbuffer1_2[VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask1[BLOCKRAM_SIZE], uint32_type vmask_p1[BLOCKRAM_SIZE],uint512_dt * kvdram2, keyvalue_t vbuffer2_1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t vbuffer2_2[VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask2[BLOCKRAM_SIZE], uint32_type vmask_p2[BLOCKRAM_SIZE],uint512_dt * kvdram3, keyvalue_t vbuffer3_1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t vbuffer3_2[VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask3[BLOCKRAM_SIZE], uint32_type vmask_p3[BLOCKRAM_SIZE],uint512_dt * kvdram4, keyvalue_t vbuffer4_1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t vbuffer4_2[VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask4[BLOCKRAM_SIZE], uint32_type vmask_p4[BLOCKRAM_SIZE],uint512_dt * kvdram5, keyvalue_t vbuffer5_1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t vbuffer5_2[VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask5[BLOCKRAM_SIZE], uint32_type vmask_p5[BLOCKRAM_SIZE],uint512_dt * kvdram6, keyvalue_t vbuffer6_1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t vbuffer6_2[VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask6[BLOCKRAM_SIZE], uint32_type vmask_p6[BLOCKRAM_SIZE],uint512_dt * kvdram7, keyvalue_t vbuffer7_1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t vbuffer7_2[VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask7[BLOCKRAM_SIZE], uint32_type vmask_p7[BLOCKRAM_SIZE],uint512_dt * kvdram8, keyvalue_t vbuffer8_1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t vbuffer8_2[VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask8[BLOCKRAM_SIZE], uint32_type vmask_p8[BLOCKRAM_SIZE],uint512_dt * kvdram9, keyvalue_t vbuffer9_1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t vbuffer9_2[VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask9[BLOCKRAM_SIZE], uint32_type vmask_p9[BLOCKRAM_SIZE],uint512_dt * kvdram10, keyvalue_t vbuffer10_1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t vbuffer10_2[VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask10[BLOCKRAM_SIZE], uint32_type vmask_p10[BLOCKRAM_SIZE],uint512_dt * kvdram11, keyvalue_t vbuffer11_1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t vbuffer11_2[VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask11[BLOCKRAM_SIZE], uint32_type vmask_p11[BLOCKRAM_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], globalparams_t globalparams[NUMCOMPUTEUNITS]);

	void start(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11, uint32_type vmask_p[BLOCKRAM_SIZE], globalparams_t globalparams[NUMCOMPUTEUNITS]);
	
	void topkernel(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11);
private:
	#ifndef FPGA_IMPL
	actsutility * actsutilityobj;
	#endif
};
#endif 






