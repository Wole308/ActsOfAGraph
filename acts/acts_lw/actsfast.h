#ifndef ACTSLW_H
#define ACTSLW_H
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
// #include "../include/actslw_common.h"
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

#define NUMVERTEXPARTITIONSPERLOAD ((PADDEDDESTBUFFER_SIZE * VECTOR_SIZE) / (APPLYVERTEXBUFFERSZ / 2)) // FIXME. this removes applyv from being a variable

class actslw {
public:
	actslw();
	~actslw();
	
	unsigned int amin(unsigned int val1, unsigned int val2);
	batch_type allignlower_KV(batch_type val);
	batch_type allignhigher_KV(batch_type val);
	batch_type allignlowerto4_KV(batch_type val);
	batch_type allignhigherto4_KV(batch_type val);
	batch_type getskipsize(step_type currentLOP, bool_type sourceORdest, globalparams_t globalparams);
	void resetkeyandvalues(skeyvalue_t * buffer, buffer_type size, unsigned int resetval);
	void resetvalues(keyvalue_t * buffer, buffer_type size, unsigned int resetval);
	void resetvalues(value_t * buffer, buffer_type size, unsigned int resetval);
	void resetmanykeyandvalues(skeyvalue_t buffer[VECTOR_SIZE][NUM_PARTITIONS], buffer_type size, unsigned int resetval);
	void resetmanykeyandvalues(keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], buffer_type size, unsigned int resetval);
	void resetmanykeyandvalues(keyvalue_t * buffer, buffer_type size, unsigned int resetval);
	void resetmanyvalues(skeyvalue_t buffer[VECTOR_SIZE][NUM_PARTITIONS], buffer_type size, unsigned int resetval);
	void resetmanyvalues(buffer_type buffer0[NUM_PARTITIONS],buffer_type buffer1[NUM_PARTITIONS],buffer_type buffer2[NUM_PARTITIONS],buffer_type buffer3[NUM_PARTITIONS],buffer_type buffer4[NUM_PARTITIONS],buffer_type buffer5[NUM_PARTITIONS],buffer_type buffer6[NUM_PARTITIONS],buffer_type buffer7[NUM_PARTITIONS], buffer_type size, unsigned int resetval);
	buffer_type getchunksize_kvs(buffer_type buffer_size, travstate_t travstate, buffer_type localoffset);
	buffer_type getchunksize(buffer_type buffersz, travstate_t travstate, buffer_type localoffset);
	partition_type getpartition(bool_type enable, keyvalue_t keyvalue, step_type currentLOP, vertex_t upperlimit, unsigned int batch_range_pow);
	partition_type getglobalpartition(keyvalue_t keyvalue, step_type currentLOP, vertex_t upperlimit, unsigned int batch_range_pow);
	value_t reducefunc(value_t vtemp, value_t res, unsigned int GraphIter, unsigned int GraphAlgo);
	value_t processedgefunc(value_t Uprop, unsigned int edgeweight, unsigned int voutdegree, unsigned int GraphIter, unsigned int GraphAlgo);
	value_t mergefunc(value_t value1, value_t value2, unsigned int GraphAlgo);
	void copykeyvalues(keyvalue_t * buffer1, keyvalue_t * buffer2, buffer_type size);
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
	travstate_t gettravstate(uint512_dt * kvdram, globalparams_t globalparams, step_type currentLOP, batch_type sourcestatsmarker);
	bool inprocessedgesstage(unsigned int currentLOP);
	bool incollectstatsstage(unsigned int currentLOP, globalparams_t globalparams);
	bool inpartitionstage(unsigned int currentLOP, globalparams_t globalparams);
	bool inreducestage(unsigned int currentLOP, globalparams_t globalparams);
	void calculateglobaloffsets(keyvalue_t * globalstatsbuffer, batch_type * skipsizes,  batch_type offset, batch_type size);
	
	// collect globalstats functions
	void readglobalstats(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ], batch_type offset_kvs);

	void collectglobalstats(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], step_type currentLOP, vertex_t upperlimit, travstate_t travstate, globalparams_t globalparams);

	void fastcollectglobalstats(bool_type enable, keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ], step_type currentLOP, batch_type source_partition);
	
	void prepareglobalstats(bool_type enable, keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ], globalparams_t globalparams);

	void prepareglobalstats2(bool_type enable, keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ], batch_type offset, globalparams_t globalparams);
	
	void saveglobalstats(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ], batch_type offset_kvs);
	
	// partition functions
	void readkeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], batch_type offset_kvs, batch_type size_kvs, travstate_t travstate);
	
	void preparekeyvalues_coarsegrainedpipeline(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t localcapsule[VECTOR_SIZE][NUM_PARTITIONS], step_type currentLOP, vertex_t upperlimit, travstate_t travstate, buffer_type size_kvs, globalparams_t globalparams);

	buffer_type preparekeyvalues1(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t localcapsule[VECTOR_SIZE][NUM_PARTITIONS], step_type currentLOP, vertex_t upperlimit, travstate_t travstate, buffer_type size_kvs, globalparams_t globalparams);

	buffer_type preparekeyvalues2(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t localcapsule[VECTOR_SIZE][NUM_PARTITIONS], step_type currentLOP, vertex_t upperlimit, travstate_t travstate, buffer_type size_kvs, globalparams_t globalparams);

	buffer_type preparekeyvalues3(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t localcapsule[VECTOR_SIZE][NUM_PARTITIONS], step_type currentLOP, vertex_t upperlimit, travstate_t travstate, buffer_type size_kvs, globalparams_t globalparams);
	
	buffer_type preparekeyvalues_finegrainedpipeline(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t localcapsule[VECTOR_SIZE][NUM_PARTITIONS], step_type currentLOP, vertex_t upperlimit, travstate_t travstate, buffer_type size_kvs, globalparams_t globalparams);

	void savekeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_t buffer[8][PADDEDDESTBUFFER_SIZE], keyvalue_t * globalcapsule, skeyvalue_t localcapsule[NUM_PARTITIONS], batch_type globalbaseaddress_kvs, globalparams_t globalparams);
	
	// reduce functions
	void readvertices(bool_type enable, uint512_dt * kvdram, keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], batch_type offset_kvs, buffer_type size_kvs);

	void replicatedata(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], buffer_type sourceoffset, buffer_type size);

	void reduce(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], vertex_t upperlimit, unsigned int GraphIter, unsigned int GraphAlgo, travstate_t travstate, globalparams_t globalparams);

	void unifydata(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], buffer_type destoffset, buffer_type size, unsigned int GraphAlgo);

	void savevertices(bool_type enable, uint512_dt * kvdram, keyvalue_t buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], batch_type offset_kvs, buffer_type size_kvs);
	
	// process-edges function
	void process_edges(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], vertex_t upperlimit, unsigned int GraphIter, unsigned int GraphAlgo, travstate_t travstate, globalparams_t globalparams);
	
	void process_edges(bool_type enable, keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t statsbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], vertex_t upperlimit, unsigned int GraphIter, unsigned int GraphAlgo, travstate_t travstate, globalparams_t globalparams);

	void process_edges(bool_type enable, value_t sourcedata, keyvalue_t destbuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], unsigned int GraphIter, unsigned int GraphAlgo, travstate_t travstate, globalparams_t globalparams);
	
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
									unsigned int currentLOP, unsigned int upperlimit, buffer_type cutoff, batch_type shiftcount, globalparams_t globalparams);
	
	// process edges phase
	void processedges(
		bool_type enable,
		uint512_dt * kvdram,
		keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer_setof1[8][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so1[8][NUM_PARTITIONS],
		keyvalue_t buffer_setof2[8][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so2[4][NUM_PARTITIONS],
		keyvalue_t buffer_setof4[8][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so4[2][NUM_PARTITIONS],
		keyvalue_t buffer_setof8[8][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so8[NUM_PARTITIONS],
		keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ],
		config_t config,
		globalparams_t globalparams,
		sweepparams_t sweepparams,
		travstate_t avtravstate);
		
	void processedges(
		bool_type enable,
		uint512_dt * kvdram,
		keyvalue_t vertices1buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t vertices2buffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer_setof1[8][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so1[8][NUM_PARTITIONS],
		keyvalue_t buffer_setof2[8][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so2[4][NUM_PARTITIONS],
		keyvalue_t buffer_setof4[8][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so4[2][NUM_PARTITIONS],
		keyvalue_t buffer_setof8[8][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so8[NUM_PARTITIONS],
		keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ], 
		config_t config,
		globalparams_t globalparams,
		sweepparams_t sweepparams,
		travstate_t avtravstate);
	
	// collectstats phase
	void collectstats(
		bool_type enable,
		uint512_dt * kvdram,
		keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer_setof1[8][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so1[8][NUM_PARTITIONS],
		keyvalue_t buffer_setof2[8][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so2[4][NUM_PARTITIONS],
		keyvalue_t buffer_setof4[8][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so4[2][NUM_PARTITIONS],
		keyvalue_t buffer_setof8[8][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so8[NUM_PARTITIONS],
		keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ],
		config_t config,
		globalparams_t globalparams,
		sweepparams_t sweepparams,
		travstate_t ctravstate);
	
	// partition phase
	void partitionupdates_coarsegrainedpipeline(
		bool_type enable,
		uint512_dt * kvdram,
		keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer_setof1[8][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so1[8][NUM_PARTITIONS],
		keyvalue_t buffer_setof2[8][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so2[4][NUM_PARTITIONS],
		keyvalue_t buffer_setof4[8][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so4[2][NUM_PARTITIONS],
		keyvalue_t buffer_setof8[8][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so8[NUM_PARTITIONS],
		keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ],
		config_t config,
		globalparams_t globalparams,
		sweepparams_t sweepparams,
		travstate_t ptravstate);
		
	void partitionupdates_finegrainedpipeline(
		bool_type enable,
		uint512_dt * kvdram,
		keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer_setof1[8][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so1[8][NUM_PARTITIONS],
		keyvalue_t buffer_setof2[8][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so2[4][NUM_PARTITIONS],
		keyvalue_t buffer_setof4[8][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so4[2][NUM_PARTITIONS],
		keyvalue_t buffer_setof8[8][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so8[NUM_PARTITIONS],
		keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ],
		config_t config,
		globalparams_t globalparams,
		sweepparams_t sweepparams,
		travstate_t ptravstate,
		batch_type sourcebaseaddr_kvs,
		batch_type destbaseaddr_kvs);
		
	void reduceupdates(
		bool_type enable,
		uint512_dt * kvdram,
		keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE],
		keyvalue_t buffer_setof1[8][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so1[8][NUM_PARTITIONS],
		keyvalue_t buffer_setof2[8][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so2[4][NUM_PARTITIONS],
		keyvalue_t buffer_setof4[8][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so4[2][NUM_PARTITIONS],
		keyvalue_t buffer_setof8[8][PADDEDDESTBUFFER_SIZE],
		skeyvalue_t templocalcapsule_so8[NUM_PARTITIONS],
		keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ],
		config_t config,
		globalparams_t globalparams,
		sweepparams_t sweepparams,
		travstate_t rtravstate);

	void dispatch(uint512_dt * kvdram);
	
	void topkernel(uint512_dt * kvdram);						
private:
	#ifndef FPGA_IMPL
	actsutility * actsutilityobj;
	#endif
};
#endif 






