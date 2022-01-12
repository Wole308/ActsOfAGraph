#ifndef ACTSUTIL_H
#define ACTSUTIL_H
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
#include "../actsutility/actsutility.h"
#endif
using namespace std;

class acts_util {
public:
	acts_util();
	~acts_util();
	
	// functions (basic)
	unsigned int UTIL_amin(unsigned int val1, unsigned int val2);
	unsigned int UTIL_aplus(unsigned int val1, unsigned int val2);
	
	// functions (allignment)
	batch_type UTIL_allignlower_KV(batch_type val);
	batch_type UTIL_allignhigher_KV(batch_type val);
	
	// functions (bit manipulation)
	unsigned int UTIL_GETMASK_UINT(unsigned int index, unsigned int size);
	unsigned int UTIL_READFROM_UINT(unsigned int data, unsigned int index, unsigned int size);
	void UTIL_WRITETO_UINT(unsigned int * data, unsigned int index, unsigned int size, unsigned int value);
	
	// functions (converters)
	keyvalue_t UTIL_GETKV(keyvalue_buffer_t data);
	keyvalue_buffer_t UTIL_GETKV(keyvalue_t data);
	keyy_t UTIL_GETK(uint32_type data);
	value_t UTIL_GETV(uint32_type data);
	keyvalue_t UTIL_GETKV2(keyvalue_vbuffer_t data);
	keyvalue_vbuffer_t UTIL_GETKV2(keyvalue_t data);
	keyy_t UTIL_GETK2(uint32_type data);
	value_t UTIL_GETV2(uint32_type data);
	keyy_t UTIL_GETKEYENTRY(uint512_dt data, unsigned int v);
	value_t UTIL_GETVTXMASK_SUBP(unit1_type vmask[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unsigned int loc, globalparams_t globalparams);	
	uint32_type UTIL_CONVERTVMASKTOUINT32(unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unsigned int index);	
	unsigned int UTIL_GETLOCALVID(unsigned int vid, unsigned int instid);
	unsigned int UTIL_GETREALVID(unsigned int lvid, unsigned int instid);
	
	// functions (actsproc utilities)
	batch_type UTIL_getskipsize(step_type currentLOP, bool_type sourceORdest, globalparams_t globalparams);
	batch_type UTIL_getrangeforeachllop(globalparams_t globalparams);
	buffer_type UTIL_getchunksize_kvs(buffer_type buffer_size, travstate_t travstate, buffer_type localoffset);
	buffer_type UTIL_getpartitionwritesz(buffer_type realsize_kvs, buffer_type bramoffset_kvs);
	void UTIL_calculateoffsets(keyvalue_capsule_t * buffer, buffer_type size);
	void UTIL_calculatemanyunallignedoffsets(keyvalue_capsule_t buffer[VECTOR_SIZE][MAX_NUM_PARTITIONS], buffer_type size, batch_type base, batch_type skipspacing);
	batch_type UTIL_get_num_source_partitions(step_type currentLOP);
	globalparams_t UTIL_getglobalparams(uint512_dt * kvdram);
	sweepparams_t UTIL_getsweepparams(globalparams_t globalparams, step_type currentLOP, batch_type source_partition);
	travstate_t UTIL_gettravstate(bool_type enable, uint512_dt * kvdram, globalparams_t globalparams, step_type currentLOP, batch_type sourcestatsmarker);
	void UTIL_settravstate(bool_type enable, uint512_dt * kvdram, globalparams_t globalparams, batch_type offset, unsigned int value);
	partition_type UTIL_getpartition(bool_type enable, unsigned int mode, keyvalue_buffer_t keyvalue, step_type currentLOP, vertex_t upperlimit, unsigned int upperpartition, unsigned int batch_range_pow);
	
	// functions (resets)
	void UTIL_resetvalues(keyvalue_t * buffer, buffer_type size, unsigned int resetval);
	void UTIL_resetvalues(keyvalue_capsule_t * buffer, buffer_type size, unsigned int resetval);
	void UTIL_resetvalues(value_t * buffer, buffer_type size, unsigned int resetval);
	void UTIL_resetkeysandvalues(keyvalue_t * buffer, buffer_type size, unsigned int resetval);
	void UTIL_resetkvstatvalues(uint512_dt * kvdram, globalparams_t globalparams);
	void UTIL_resetkvstatvalues(uint512_dt * kvdram, unsigned int size_kvs, globalparams_t globalparams);
	void UTIL_reset(unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE]);
	void UTIL_accumkvstats(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams);
	void UTIL_increment_graphiteration(uint512_dt * kvdram, globalparams_t globalparams);
	void UTIL_resetenvbuffers(keyvalue_capsule_t capsule_so1[VECTOR_SIZE][MAX_NUM_PARTITIONS], keyvalue_capsule_t capsule_so8[MAX_NUM_PARTITIONS]);
	void UTIL_resetenvbuffer(keyvalue_capsule_t capsule_so8[MAX_NUM_PARTITIONS]);
	
private:
	#ifndef FPGA_IMPL
	actsutility * actsutilityobj;
	#endif
};
#endif 





