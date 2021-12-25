#ifndef ACTSPROC_H
#define ACTSPROC_H
#include "../../include/config_params.h"
#include "../../include/common.h"
#include "../include/actscommon.h"
#ifdef SW 
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
#endif
#ifdef SW 
#include "../../src/utility/utility.h"
#endif
#ifdef SW 
#include "../../acts/actsutility/actsutility.h"
#endif
using namespace std;

#define ACTSPROC_AUTOMATE_ACROSSDATASETS

// #define ENABLE_SUBVMASKING
#define SUBPMASKFACTOR_POW 4
#define SUBPMASKFACTOR 16 // NOTE: CONSTANT

#define REDUCEBUFFERFACTOR 8 // CONSTANT

// #define DATA_SENSITIVE_ACTS
#ifdef DATA_SENSITIVE_ACTS
	#define MAX_NUM_PARTITIONS (NUM_PARTITIONS + 2)
#else 
	#define MAX_NUM_PARTITIONS NUM_PARTITIONS
#endif

/* typedef struct {
	unsigned int first;
	unsigned int last;
	unsigned int edgebankID;
	unsigned int v_chunkid;
	unsigned int stage;
	unsigned int laststage;
	unsigned int currentLOP;
	unsigned int lastLOP;
	unsigned int source_partition;
	unsigned int first_source_partition;
	unsigned int last_source_partition;
	unsigned int num_source_partitions;
	unsigned int sourcestatsmarker;
	unsigned int deststatsmarker;
	unsigned int EN_PROCESS; 
	unsigned int EN_PARTITION; 
	unsigned int EN_REDUCE;
	unsigned int EN_PROCESSANDREDUCE;
	unsigned int EN_PROCESSANDPARTITION;
} globalposition_t; */

class actsproc {
public:
	actsproc();
	~actsproc();
	
	unsigned int test(uint512_dt * kvdram);
	
	// functions (basic)
	unsigned int amin(unsigned int val1, unsigned int val2);
	unsigned int aplus(unsigned int val1, unsigned int val2);
	
	// functions (allignment)
	batch_type allignlower_KV(batch_type val);
	batch_type allignhigher_KV(batch_type val);
	
	// functions (bit manipulation)
	unsigned int GETMASK_UINT(unsigned int index, unsigned int size);
	unsigned int READFROM_UINT(unsigned int data, unsigned int index, unsigned int size);
	void WRITETO_UINT(unsigned int * data, unsigned int index, unsigned int size, unsigned int value);
	
	// functions (converters)
	keyvalue_t GETKV(keyvalue_buffer_t data);
	keyvalue_buffer_t GETKV(keyvalue_t data);
	keyy_t GETK(uint32_type data);
	value_t GETV(uint32_type data);
	keyvalue_t GETKV2(keyvalue_vbuffer_t data);
	keyvalue_vbuffer_t GETKV2(keyvalue_t data);
	keyy_t GETK2(uint32_type data);
	value_t GETV2(uint32_type data);
	keyy_t GETKEYENTRY(uint512_dt data, unsigned int v);
	value_t GETVTXDATA(keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int loc, globalparams_t globalparams);
	value_t GETVTXMASK(unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unsigned int loc, globalparams_t globalparams);
	value_t GETVTXMASK_SUBP(unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unsigned int loc, globalparams_t globalparams);
	keyvalue1_type GETVTXMASK_SUBPP(unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unsigned int loc, globalparams_t globalparams);
	
	// functions (actsproc utilities)
	batch_type getskipsize(step_type currentLOP, bool_type sourceORdest, globalparams_t globalparams);
	batch_type getrangeforeachllop(globalparams_t globalparams);
	buffer_type getchunksize_kvs(buffer_type buffer_size, travstate_t travstate, buffer_type localoffset);
	buffer_type getpartitionwritesz(buffer_type realsize_kvs, buffer_type bramoffset_kvs);
	void calculateoffsets(keyvalue_capsule_t * buffer, buffer_type size);
	void calculatemanyunallignedoffsets(keyvalue_capsule_t buffer[VECTOR_SIZE][MAX_NUM_PARTITIONS], buffer_type size, batch_type base, batch_type skipspacing);
	batch_type get_num_source_partitions(step_type currentLOP);
	globalparams_t getglobalparams(uint512_dt * kvdram);
	sweepparams_t getsweepparams(globalparams_t globalparams, step_type currentLOP, batch_type source_partition);
	travstate_t gettravstate(bool_type enable, uint512_dt * kvdram, globalparams_t globalparams, step_type currentLOP, batch_type sourcestatsmarker);
	void settravstate(bool_type enable, uint512_dt * kvdram, globalparams_t globalparams, batch_type offset, unsigned int value);
	partition_type getpartition(bool_type enable, keyvalue_buffer_t keyvalue, step_type currentLOP, vertex_t upperlimit, unsigned int upperpartition, unsigned int batch_range_pow);

	// functions (resets)
	void resetvalues(keyvalue_t * buffer, buffer_type size, unsigned int resetval);
	void resetvalues(keyvalue_capsule_t * buffer, buffer_type size, unsigned int resetval);
	void resetvalues(value_t * buffer, buffer_type size, unsigned int resetval);
	void resetkeysandvalues(keyvalue_t * buffer, buffer_type size, unsigned int resetval);
	void resetkvstatvalues(uint512_dt * kvdram, globalparams_t globalparams);
	void accumkvstats(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams);
	void commitkvstats(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams, unsigned int offset);
	void increment_graphiteration(uint512_dt * kvdram, globalparams_t globalparams);
	
	// functions (accessors)
	fetchmessage_t readkeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs, travstate_t travstate, globalparams_t globalparams);
	
	void savekeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_buffer_t buffer[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_t * globalcapsule, keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], batch_type globalbaseaddress_kvs, globalparams_t globalparams);
	
	void readkeyvalues(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_buffer_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);

	void savekeyvalues(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_buffer_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	
	void readvdata(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);

	void savevdata(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	
	void loadvmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	
	void loadmany1vmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
	void loadmany2vmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
	void loadmany3vmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
	void loadmany4vmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
	void loadmany5vmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
	void loadmany6vmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
	void loadmany7vmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5[BLOCKRAM_SIZE],unitBRAMwidth_type vmask6[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
	void loadmany8vmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5[BLOCKRAM_SIZE],unitBRAMwidth_type vmask6[BLOCKRAM_SIZE],unitBRAMwidth_type vmask7[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
	void loadmany9vmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5[BLOCKRAM_SIZE],unitBRAMwidth_type vmask6[BLOCKRAM_SIZE],unitBRAMwidth_type vmask7[BLOCKRAM_SIZE],unitBRAMwidth_type vmask8[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
	void loadmany10vmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5[BLOCKRAM_SIZE],unitBRAMwidth_type vmask6[BLOCKRAM_SIZE],unitBRAMwidth_type vmask7[BLOCKRAM_SIZE],unitBRAMwidth_type vmask8[BLOCKRAM_SIZE],unitBRAMwidth_type vmask9[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
	void loadmany11vmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5[BLOCKRAM_SIZE],unitBRAMwidth_type vmask6[BLOCKRAM_SIZE],unitBRAMwidth_type vmask7[BLOCKRAM_SIZE],unitBRAMwidth_type vmask8[BLOCKRAM_SIZE],unitBRAMwidth_type vmask9[BLOCKRAM_SIZE],unitBRAMwidth_type vmask10[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
	void loadmany12vmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5[BLOCKRAM_SIZE],unitBRAMwidth_type vmask6[BLOCKRAM_SIZE],unitBRAMwidth_type vmask7[BLOCKRAM_SIZE],unitBRAMwidth_type vmask8[BLOCKRAM_SIZE],unitBRAMwidth_type vmask9[BLOCKRAM_SIZE],unitBRAMwidth_type vmask10[BLOCKRAM_SIZE],unitBRAMwidth_type vmask11[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
	
	void loadmany1vmasks_subp(bool_type enable, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], unitBRAMwidth_type vmask0_subp[BLOCKRAM_SIZE], buffer_type size_kvs);					
	void loadmany2vmasks_subp(bool_type enable, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], unitBRAMwidth_type vmask0_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1_subp[BLOCKRAM_SIZE], buffer_type size_kvs);					
	void loadmany3vmasks_subp(bool_type enable, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], unitBRAMwidth_type vmask0_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2_subp[BLOCKRAM_SIZE], buffer_type size_kvs);					
	void loadmany4vmasks_subp(bool_type enable, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], unitBRAMwidth_type vmask0_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3_subp[BLOCKRAM_SIZE], buffer_type size_kvs);					
	void loadmany5vmasks_subp(bool_type enable, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], unitBRAMwidth_type vmask0_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4_subp[BLOCKRAM_SIZE], buffer_type size_kvs);					
	void loadmany6vmasks_subp(bool_type enable, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], unitBRAMwidth_type vmask0_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5_subp[BLOCKRAM_SIZE], buffer_type size_kvs);					
	void loadmany7vmasks_subp(bool_type enable, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], unitBRAMwidth_type vmask0_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask6_subp[BLOCKRAM_SIZE], buffer_type size_kvs);					
	void loadmany8vmasks_subp(bool_type enable, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], unitBRAMwidth_type vmask0_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask6_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask7_subp[BLOCKRAM_SIZE], buffer_type size_kvs);					
	void loadmany9vmasks_subp(bool_type enable, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], unitBRAMwidth_type vmask0_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask6_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask7_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask8_subp[BLOCKRAM_SIZE], buffer_type size_kvs);					
	void loadmany10vmasks_subp(bool_type enable, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], unitBRAMwidth_type vmask0_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask6_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask7_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask8_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask9_subp[BLOCKRAM_SIZE], buffer_type size_kvs);					
	void loadmany11vmasks_subp(bool_type enable, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], unitBRAMwidth_type vmask0_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask6_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask7_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask8_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask9_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask10_subp[BLOCKRAM_SIZE], buffer_type size_kvs);					
	void loadmany12vmasks_subp(bool_type enable, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], unitBRAMwidth_type vmask0_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask6_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask7_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask8_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask9_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask10_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask11_subp[BLOCKRAM_SIZE], buffer_type size_kvs);					
	
	void loadvmask_p(uint512_dt * kvdram, uint32_type vmask_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
	void loadmany1vmask_p(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
	void loadmany2vmask_p(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
	void loadmany3vmask_p(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
	void loadmany4vmask_p(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
	void loadmany5vmask_p(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
	void loadmany6vmask_p(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
	void loadmany7vmask_p(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
	void loadmany8vmask_p(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE],uint32_type vmask7_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
	void loadmany9vmask_p(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE],uint32_type vmask7_p[BLOCKRAM_SIZE],uint32_type vmask8_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
	void loadmany10vmask_p(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE],uint32_type vmask7_p[BLOCKRAM_SIZE],uint32_type vmask8_p[BLOCKRAM_SIZE],uint32_type vmask9_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
	void loadmany11vmask_p(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE],uint32_type vmask7_p[BLOCKRAM_SIZE],uint32_type vmask8_p[BLOCKRAM_SIZE],uint32_type vmask9_p[BLOCKRAM_SIZE],uint32_type vmask10_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
	void loadmany12vmask_p(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE],uint32_type vmask7_p[BLOCKRAM_SIZE],uint32_type vmask8_p[BLOCKRAM_SIZE],uint32_type vmask9_p[BLOCKRAM_SIZE],uint32_type vmask10_p[BLOCKRAM_SIZE],uint32_type vmask11_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);

	void readglobalstats(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], batch_type offset_kvs, globalparams_t globalparams);

	void saveglobalstats(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], batch_type offset_kvs, globalparams_t globalparams);
	
	tuple_t getvptrs( uint512_dt *  kvdram, unsigned int beginoffset, unsigned int endoffset, unsigned int edgebankID);
	
	unsigned int getvptr(uint512_dt * kvdram, unsigned int baseoffset_kvs, unsigned int offset);
	tuple_t getvptrs_opt( uint512_dt *  kvdram, unsigned int baseoffset_kvs, unsigned int beginoffset, unsigned int endoffset, unsigned int edgebankID);
	
	void copyvs(uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], globalparams_t globalparamsK);
	
	void copystats(uint512_dt * edges, uint512_dt * kvdram, globalparams_t globalparamsE, globalparams_t globalparamsK);
	
	void copyallstats( uint512_dt * kvdram, globalparams_t globalparamsE, globalparams_t globalparamsK, unsigned int edgebankID);
	
	// functions (process)
	value_t processfunc(value_t udata, value_t edgew, unsigned int GraphAlgo);
	
	fetchmessage_t readandprocess(bool_type enable, uint512_dt * edges, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unitBRAMwidth_type vmask_subp[BLOCKRAM_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], 
			batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, sweepparams_t sweepparams, globalparams_t globalparams);

	// functions (partition)
	void preparekeyvalues(bool_type enable1, bool_type enable2, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_buffer_t destbuffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[VECTOR_SIZE][MAX_NUM_PARTITIONS], step_type currentLOP, sweepparams_t sweepparams, buffer_type size_kvs, buffer_type cutoffs[VECTOR_SIZE], globalparams_t globalparams);				
	
	void priorpartitionkeyvalues(bool_type enable1, bool_type enable2, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_buffer_t destbuffer[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], step_type currentLOP, sweepparams_t sweepparams, buffer_type size_kvs, globalparams_t globalparams);
	
	// functions (reduce)
	value_t reducefunc(value_t vtemp, value_t res, unsigned int GraphIter, unsigned int GraphAlgo);
	
	void reducevector(keyvalue_buffer_t kvdata, 
			keyvalue_vbuffer_t destbuffer[BLOCKRAM_SIZE], 
			buffer_type destoffset, unsigned int upperlimit, sweepparams_t sweepparams, globalparams_t globalparams);

	void reduceandbuffer(bool_type enable, keyvalue_buffer_t buffer[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], sweepparams_t sweepparams, globalparams_t globalparams);
	
	void priorreduceandbuffer(bool_type enable, keyvalue_buffer_t buffer[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], sweepparams_t sweepparams, globalparams_t globalparams);
	
	void priorreduceandbuffer(bool_type enable, keyvalue_buffer_t buffer[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], buffer_type chunk_size, sweepparams_t sweepparams, globalparams_t globalparams);
	
	void tradreduceandbuffer(bool_type enable, uint512_dt * kvdram, keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], buffer_type chunk_size, keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], sweepparams_t sweepparams, globalparams_t globalparams);

	// functions (merge)
	unsigned int SYNC_amin(unsigned int val1, unsigned int val2);
	value_t mergefunc(value_t value1, value_t value2, unsigned int GraphAlgo);
	
	void readandreplicate1vdata(bool_type enable, uint512_dt * vdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	void readandreplicate2vdata(bool_type enable, uint512_dt * vdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	void readandreplicate3vdata(bool_type enable, uint512_dt * vdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	void readandreplicate4vdata(bool_type enable, uint512_dt * vdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	void readandreplicate5vdata(bool_type enable, uint512_dt * vdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	void readandreplicate6vdata(bool_type enable, uint512_dt * vdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	void readandreplicate7vdata(bool_type enable, uint512_dt * vdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	void readandreplicate8vdata(bool_type enable, uint512_dt * vdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	void readandreplicate9vdata(bool_type enable, uint512_dt * vdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	void readandreplicate10vdata(bool_type enable, uint512_dt * vdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	void readandreplicate11vdata(bool_type enable, uint512_dt * vdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer10[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	void readandreplicate12vdata(bool_type enable, uint512_dt * vdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer10[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer11[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	
	void merge1andsavevdata(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, unsigned int dramoffset_kvs); 
	void merge2andsavevdata(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, unsigned int dramoffset_kvs); 
	void merge3andsavevdata(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, unsigned int dramoffset_kvs); 
	void merge4andsavevdata(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, unsigned int dramoffset_kvs); 
	void merge5andsavevdata(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, unsigned int dramoffset_kvs); 
	void merge6andsavevdata(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, unsigned int dramoffset_kvs); 
	void merge7andsavevdata(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, unsigned int dramoffset_kvs); 
	void merge8andsavevdata(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, unsigned int dramoffset_kvs); 
	void merge9andsavevdata(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, unsigned int dramoffset_kvs); 
	void merge10andsavevdata(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer9[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, unsigned int dramoffset_kvs); 
	void merge11andsavevdata(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer9[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer10[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, unsigned int dramoffset_kvs); 
	void merge12andsavevdata(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer9[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer10[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer11[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, unsigned int dramoffset_kvs); 
	
	// actsproc
	void resetenvbuffers(keyvalue_capsule_t capsule_so1[VECTOR_SIZE][MAX_NUM_PARTITIONS], keyvalue_capsule_t capsule_so8[MAX_NUM_PARTITIONS]);
	
	void resetenvbuffer(keyvalue_capsule_t capsule_so8[MAX_NUM_PARTITIONS]);

	fetchmessage_t fetchkeyvalues(bool_type enable, unsigned int mode,  uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unitBRAMwidth_type vmask_subp[BLOCKRAM_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], 
		batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, sweepparams_t sweepparams, globalparams_t globalparams, unsigned int edgebankID);
	
	void commitkeyvalues(bool_type enable1, bool_type enable2, unsigned int mode, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], 
		keyvalue_buffer_t destbuffer[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_t globalcapsule[MAX_NUM_PARTITIONS], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], 
			batch_type destbaseaddr_kvs, buffer_type chunk_size, sweepparams_t sweepparams, globalparams_t globalparams);
	
	void priorcommitkeyvalues(bool_type enable1, bool_type enable2, unsigned int mode, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], 
		keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_buffer_t destbuffer[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_t globalcapsule[MAX_NUM_PARTITIONS], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], 
				batch_type destbaseaddr_kvs, buffer_type chunk_size, sweepparams_t sweepparams, globalparams_t globalparams);
				
	void actspipeline(bool_type enable1, bool_type enable2, keyvalue_buffer_t buffer_setof1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_capsule_t capsule_so1[VECTOR_SIZE][MAX_NUM_PARTITIONS], 
						keyvalue_buffer_t buffer_setof8[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_capsule_t capsule_so8[MAX_NUM_PARTITIONS],
							unsigned int currentLOP, sweepparams_t sweepparams, buffer_type cutoffs[VECTOR_SIZE], batch_type shiftcount, globalparams_t globalparams);

	void actit(bool_type enable, unsigned int mode,
 uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unitBRAMwidth_type vmask_subp[BLOCKRAM_SIZE], keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], 
			globalparams_t globalparams, sweepparams_t sweepparams, travstate_t ptravstate, batch_type sourcebaseaddr_kvs, batch_type destbaseaddr_kvs,
			bool_type resetenv, bool_type flush, unsigned int edgebankID);
	
	void priorit(bool_type enable, unsigned int mode,
 uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unitBRAMwidth_type vmask_subp[BLOCKRAM_SIZE], keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], 
			globalparams_t globalparams, sweepparams_t sweepparams, travstate_t ptravstate, batch_type sourcebaseaddr_kvs, batch_type destbaseaddr_kvs,
			bool_type resetenv, bool_type flush, unsigned int edgebankID);
			
	void tradit(bool_type enable, unsigned int mode,
 uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unitBRAMwidth_type vmask_subp[BLOCKRAM_SIZE], keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], 
			globalparams_t globalparams, sweepparams_t sweepparams, travstate_t ptravstate, batch_type sourcebaseaddr_kvs, batch_type destbaseaddr_kvs,
			bool_type resetenv, bool_type flush, unsigned int edgebankID);

	#ifndef CONFIG_UNIFIED_VDRAM
	// dispatch
	void processit( uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unitBRAMwidth_type vmask_subp[BLOCKRAM_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK,
			unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID);							

	void partitionit( uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unitBRAMwidth_type vmask_subp[BLOCKRAM_SIZE], globalparams_t globalparams, unsigned int edgebankID);
		
	void reduceit( uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unitBRAMwidth_type vmask_subp[BLOCKRAM_SIZE], batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparams, unsigned int edgebankID);					

	void dispatch(bool_type en_process, bool_type en_partition, bool_type en_reduce,  uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unitBRAMwidth_type vmask_subp[BLOCKRAM_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE],
			batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparamsE, globalparams_t globalparamsK,
				unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID);
	
	void dispatch_reduce( uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unitBRAMwidth_type vmask_subp[BLOCKRAM_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK,
			unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID);

	// top 
	void topkernelproc_embedded(
	
		uint512_dt * kvdram);
		
	void topkernelP1(
		uint512_dt * kvdram0,
		uint512_dt * vdram
		);
	void topkernelP2(
		uint512_dt * kvdram0,
		uint512_dt * kvdram1,
		uint512_dt * vdram
		);
	void topkernelP3(
		uint512_dt * kvdram0,
		uint512_dt * kvdram1,
		uint512_dt * kvdram2,
		uint512_dt * vdram
		);
	void topkernelP4(
		uint512_dt * kvdram0,
		uint512_dt * kvdram1,
		uint512_dt * kvdram2,
		uint512_dt * kvdram3,
		uint512_dt * vdram
		);
	void topkernelP5(
		uint512_dt * kvdram0,
		uint512_dt * kvdram1,
		uint512_dt * kvdram2,
		uint512_dt * kvdram3,
		uint512_dt * kvdram4,
		uint512_dt * vdram
		);
	void topkernelP6(
		uint512_dt * kvdram0,
		uint512_dt * kvdram1,
		uint512_dt * kvdram2,
		uint512_dt * kvdram3,
		uint512_dt * kvdram4,
		uint512_dt * kvdram5,
		uint512_dt * vdram
		);
	void topkernelP7(
		uint512_dt * kvdram0,
		uint512_dt * kvdram1,
		uint512_dt * kvdram2,
		uint512_dt * kvdram3,
		uint512_dt * kvdram4,
		uint512_dt * kvdram5,
		uint512_dt * kvdram6,
		uint512_dt * vdram
		);
	void topkernelP8(
		uint512_dt * kvdram0,
		uint512_dt * kvdram1,
		uint512_dt * kvdram2,
		uint512_dt * kvdram3,
		uint512_dt * kvdram4,
		uint512_dt * kvdram5,
		uint512_dt * kvdram6,
		uint512_dt * kvdram7,
		uint512_dt * vdram
		);
	void topkernelP9(
		uint512_dt * kvdram0,
		uint512_dt * kvdram1,
		uint512_dt * kvdram2,
		uint512_dt * kvdram3,
		uint512_dt * kvdram4,
		uint512_dt * kvdram5,
		uint512_dt * kvdram6,
		uint512_dt * kvdram7,
		uint512_dt * kvdram8,
		uint512_dt * vdram
		);
	void topkernelP10(
		uint512_dt * kvdram0,
		uint512_dt * kvdram1,
		uint512_dt * kvdram2,
		uint512_dt * kvdram3,
		uint512_dt * kvdram4,
		uint512_dt * kvdram5,
		uint512_dt * kvdram6,
		uint512_dt * kvdram7,
		uint512_dt * kvdram8,
		uint512_dt * kvdram9,
		uint512_dt * vdram
		);
	void topkernelP11(
		uint512_dt * kvdram0,
		uint512_dt * kvdram1,
		uint512_dt * kvdram2,
		uint512_dt * kvdram3,
		uint512_dt * kvdram4,
		uint512_dt * kvdram5,
		uint512_dt * kvdram6,
		uint512_dt * kvdram7,
		uint512_dt * kvdram8,
		uint512_dt * kvdram9,
		uint512_dt * kvdram10,
		uint512_dt * vdram
		);
	void topkernelP12(
		uint512_dt * kvdram0,
		uint512_dt * kvdram1,
		uint512_dt * kvdram2,
		uint512_dt * kvdram3,
		uint512_dt * kvdram4,
		uint512_dt * kvdram5,
		uint512_dt * kvdram6,
		uint512_dt * kvdram7,
		uint512_dt * kvdram8,
		uint512_dt * kvdram9,
		uint512_dt * kvdram10,
		uint512_dt * kvdram11,
		uint512_dt * vdram
		);
	#endif 
	
	#ifdef CONFIG_UNIFIED_VDRAM
	// dispatch
	void processit( uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unitBRAMwidth_type vmask_subp[BLOCKRAM_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition,
			unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID, unsigned int hybridmode);							

	void partitionit( uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unitBRAMwidth_type vmask_subp[BLOCKRAM_SIZE], keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], globalparams_t globalparams, globalposition_t globalposition, unsigned int edgebankID);
		
	void reduceit( uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unitBRAMwidth_type vmask_subp[BLOCKRAM_SIZE], keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], globalparams_t globalparams, globalposition_t globalposition, unsigned int edgebankID);					

	void dispatch(bool_type en_process, bool_type en_partition, bool_type en_reduce,  uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unitBRAMwidth_type vmask_subp[BLOCKRAM_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS],
			globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition,
				unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID, unsigned int hybridmode);
	
	void dispatch_reduce(bool_type en_reduce,  uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unitBRAMwidth_type vmask_subp[BLOCKRAM_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition,
			unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID, unsigned int hybridmode);

	void topkernelproc_embedded(unsigned int en_process, unsigned int en_partition, unsigned int en_reduce,  uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], unitBRAMwidth_type vmask_subp[BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], globalposition_t globalposition, unsigned int hybridmode);
		
	// top
	void topkernelP1(
		uint512_dt * kvdram0,
		uint512_dt * vdram
		);
	void topkernelP2(
		uint512_dt * kvdram0,
		uint512_dt * kvdram1,
		uint512_dt * vdram
		);
	void topkernelP3(
		uint512_dt * kvdram0,
		uint512_dt * kvdram1,
		uint512_dt * kvdram2,
		uint512_dt * vdram
		);
	void topkernelP4(
		uint512_dt * kvdram0,
		uint512_dt * kvdram1,
		uint512_dt * kvdram2,
		uint512_dt * kvdram3,
		uint512_dt * vdram
		);
	void topkernelP5(
		uint512_dt * kvdram0,
		uint512_dt * kvdram1,
		uint512_dt * kvdram2,
		uint512_dt * kvdram3,
		uint512_dt * kvdram4,
		uint512_dt * vdram
		);
	void topkernelP6(
		uint512_dt * kvdram0,
		uint512_dt * kvdram1,
		uint512_dt * kvdram2,
		uint512_dt * kvdram3,
		uint512_dt * kvdram4,
		uint512_dt * kvdram5,
		uint512_dt * vdram
		);
	void topkernelP7(
		uint512_dt * kvdram0,
		uint512_dt * kvdram1,
		uint512_dt * kvdram2,
		uint512_dt * kvdram3,
		uint512_dt * kvdram4,
		uint512_dt * kvdram5,
		uint512_dt * kvdram6,
		uint512_dt * vdram
		);
	void topkernelP8(
		uint512_dt * kvdram0,
		uint512_dt * kvdram1,
		uint512_dt * kvdram2,
		uint512_dt * kvdram3,
		uint512_dt * kvdram4,
		uint512_dt * kvdram5,
		uint512_dt * kvdram6,
		uint512_dt * kvdram7,
		uint512_dt * vdram
		);
	void topkernelP9(
		uint512_dt * kvdram0,
		uint512_dt * kvdram1,
		uint512_dt * kvdram2,
		uint512_dt * kvdram3,
		uint512_dt * kvdram4,
		uint512_dt * kvdram5,
		uint512_dt * kvdram6,
		uint512_dt * kvdram7,
		uint512_dt * kvdram8,
		uint512_dt * vdram
		);
	void topkernelP10(
		uint512_dt * kvdram0,
		uint512_dt * kvdram1,
		uint512_dt * kvdram2,
		uint512_dt * kvdram3,
		uint512_dt * kvdram4,
		uint512_dt * kvdram5,
		uint512_dt * kvdram6,
		uint512_dt * kvdram7,
		uint512_dt * kvdram8,
		uint512_dt * kvdram9,
		uint512_dt * vdram
		);
	void topkernelP11(
		uint512_dt * kvdram0,
		uint512_dt * kvdram1,
		uint512_dt * kvdram2,
		uint512_dt * kvdram3,
		uint512_dt * kvdram4,
		uint512_dt * kvdram5,
		uint512_dt * kvdram6,
		uint512_dt * kvdram7,
		uint512_dt * kvdram8,
		uint512_dt * kvdram9,
		uint512_dt * kvdram10,
		uint512_dt * vdram
		);
	void topkernelP12(
		uint512_dt * kvdram0,
		uint512_dt * kvdram1,
		uint512_dt * kvdram2,
		uint512_dt * kvdram3,
		uint512_dt * kvdram4,
		uint512_dt * kvdram5,
		uint512_dt * kvdram6,
		uint512_dt * kvdram7,
		uint512_dt * kvdram8,
		uint512_dt * kvdram9,
		uint512_dt * kvdram10,
		uint512_dt * kvdram11,
		uint512_dt * vdram
		);
	#endif 
	
private:
	#ifndef FPGA_IMPL
	actsutility * actsutilityobj;
	#endif
};
#endif 






