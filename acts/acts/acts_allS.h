#ifndef ACTS_ALL_H
#define ACTS_ALL_H
#include "../../include/config_params.h"
#include "../../include/common.h"
#include "../include/actscommon.h"
#ifdef SW_ALLINONE
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
#ifdef SW_ALLINONE
#include "../../src/utility/utility.h"
#include "../../acts/actsutility/actsutility.h"
#include "mydebug.h"
using namespace std;
#endif

#ifdef SW_ALLINONE
class acts_all {
public:
	acts_all(mydebug * _mydebugobj);
	~acts_all();
	
	#ifdef CONFIG_ENABLECLASS_ACTS_UTIL
// functions (basic)
unsigned int UTILS_amin(unsigned int val1, unsigned int val2);
unsigned int UTILS_aplus(unsigned int val1, unsigned int val2);

// functions (allignment)
batch_type UTILS_allignlower_KV(batch_type val);
batch_type UTILS_allignhigher_KV(batch_type val);
batch_type UTILS_allignlower_KV2(batch_type val);
batch_type UTILS_allignhigher_KV2(batch_type val);

// functions (bit manipulation)
unsigned int UTILS_GETMASK_UINT(unsigned int index, unsigned int size);
unsigned int UTILS_READFROM_UINT(unsigned int data, unsigned int index, unsigned int size);
void UTILS_WRITETO_UINT(unsigned int * data, unsigned int index, unsigned int size, unsigned int value);
unsigned int UTILS_READBITSFROM_UINTV(uint32_type data, unsigned int index, unsigned int size);
void UTILS_WRITEBITSTO_UINTV(uint32_type * data, unsigned int index, unsigned int size, unsigned int value);

// functions (converters)
keyvalue_t UTILS_GETKV(keyvalue_buffer_t data);
keyvalue_buffer_t UTILS_GETKV(keyvalue_t data);
keyy_t UTILS_GETK(uint32_type data);
value_t UTILS_GETV(uint32_type data);
keyy_t UTILS_GETKEYENTRY(uint512_dt data, unsigned int v);
unsigned int UTILS_GETLOCALVID(unsigned int vid, unsigned int instid);
unsigned int UTILS_GETREALVID(unsigned int lvid, unsigned int instid);

// functions (actsproc utilities)
batch_type UTILS_getskipsize(step_type currentLOP, bool_type sourceORdest, globalparams_t globalparams);
batch_type UTILS_getrangeforeachllop(globalparams_t globalparams);
buffer_type UTILS_getchunksize_kvs(buffer_type buffer_size, travstate_t travstate, buffer_type localoffset);
buffer_type UTILS_getpartitionwritesz(buffer_type realsize_kvs, buffer_type bramoffset_kvs);
void UTILS_calculateoffsets(keyvalue_capsule_t * buffer, buffer_type size);
void UTILS_calculatemanyunallignedoffsets(keyvalue_capsule_t buffer[VECTOR_SIZE][MAX_NUM_PARTITIONS], buffer_type size, batch_type base, batch_type skipspacing);
batch_type UTILS_get_num_source_partitions(step_type currentLOP);
globalparams_t UTILS_getglobalparams(uint512_dt * kvdram, unsigned int banksection);
globalparams_t UTILS_getglobalparamsUNUSED(uint512_dt * kvdram, unsigned int banksection);
sweepparams_t UTILS_getsweepparams(globalparams_t globalparams, step_type currentLOP, batch_type source_partition);
travstate_t UTILS_gettravstate(bool_type enable, uint512_dt * kvdram, globalparams_t globalparams, step_type currentLOP, batch_type sourcestatsmarker);
void UTILS_settravstate(bool_type enable, uint512_dt * kvdram, globalparams_t globalparams, batch_type offset, unsigned int value);
partition_type UTILS_getpartition(bool_type enable, unsigned int mode, keyvalue_buffer_t keyvalue, step_type currentLOP, vertex_t upperlimit, unsigned int upperpartition, unsigned int batch_range_pow);

// functions (resets)
void UTILS_resetvalues(keyvalue_t * buffer, buffer_type size, unsigned int resetval);
void UTILS_resetvalues(keyvalue_capsule_t * buffer, buffer_type size, unsigned int resetval);
void UTILS_resetvalues(value_t * buffer, buffer_type size, unsigned int resetval);
void UTILS_resetkeysandvalues(keyvalue_t * buffer, buffer_type size, unsigned int resetval);
void UTILS_resetkvstatvalues(uint512_dt * kvdram, globalparams_t globalparams);
void UTILS_resetkvstatvalues(uint512_dt * kvdram, unsigned int size_kvs, globalparams_t globalparams);
void UTILS_accumkvstats(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams);
void UTILS_increment_graphiteration(uint512_dt * kvdram, globalparams_t globalparams);
void UTILS_resetenvbuffers(keyvalue_capsule_t capsule_so1[VECTOR_SIZE][MAX_NUM_PARTITIONS], keyvalue_capsule_t capsule_so8[MAX_NUM_PARTITIONS]);
void UTILS_resetenvbuffer(keyvalue_capsule_t capsule_so8[MAX_NUM_PARTITIONS]);






	
	#endif 
	#ifdef CONFIG_ENABLECLASS_ALGO_FUNCS
value_t PROCESSS_processfunc(value_t udata, value_t edgew, unsigned int GraphAlgo);

value_t REDUCES_reducefunc(value_t vtemp, value_t res, unsigned int GraphIter, unsigned int GraphAlgo);






	
	#endif 
	#ifdef CONFIG_ENABLECLASS_MEM_CONVERT_AND_ACCESS
// util 
unsigned int MEMCAS_READVDATA(keyvalue_vbuffer_t wideword);
unsigned int MEMCAS_READVMASK(keyvalue_vbuffer_t wideword);
vmdata_t MEMCAS_READVDATAANDVMASK(keyvalue_vbuffer_t wideword);
void MEMCAS_WRITEVDATA(keyvalue_vbuffer_t * wideword, value_t vdata);
void MEMCAS_WRITEVMASK(keyvalue_vbuffer_t * wideword, unit1_type vmask);
tuple_t MEMCAS_READVDATAWITHVMASK(keyvalue_vbuffer_t wideword);
void MEMCAS_WRITEVDATAWITHVMASK(keyvalue_vbuffer_t * wideword, value_t vdata, unit1_type vmask);

// any data 
uint32_type MEMCAS_READDATAFROMBUFFER(unsigned int index, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type bufferoffset_kvs);

void MEMCAS_WRITEDATATOBUFFER(unsigned int index, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], uint32_type data, batch_type bufferoffset_kvs);

keyvalue_vbuffer_t MEMCAS_READDATAFROMDRAM(unsigned int index, uint512_dt * kvdram, batch_type baseoffset_kvs, batch_type offset_kvs);

void MEMCAS_WRITEDATATODRAM(unsigned int index, uint512_dt * kvdram, uint32_type data, batch_type baseoffset_kvs, batch_type offset_kvs);

// vdatas
void MEMCAS_WRITETOBUFFER_VDATA(unsigned int v, unsigned int index, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], value_t vdata, batch_type bufferoffset_kvs);

void MEMCAS_WRITETOBUFFER_VDATA(unsigned int index, keyvalue_vbuffer_t buffer[BLOCKRAM_SIZE], value_t vdata, batch_type bufferoffset_kvs);

void MEMCAS_WRITETOBUFFER_VDATAS(unsigned int index, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], value_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);

void MEMCAS_WRITETOBUFFERWITHDEPTHS_VDATAS(unsigned int indexes[VDATA_PACKINGSIZE], keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], value_t vdatas[VDATA_PACKINGSIZE], batch_type bufferoffset_kvs);			

value_t MEMCAS_READFROMBUFFER_VDATA(unsigned int index, keyvalue_vbuffer_t buffer[BLOCKRAM_VDATA_SIZE], batch_type bufferoffset_kvs);

void MEMCAS_READFROMBUFFER_VDATAS(unsigned int index, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], value_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);

// vdatas & vmasks 
vmdata_t MEMCAS_READFROMBUFFER_VDATAWITHVMASK(unsigned int index, keyvalue_vbuffer_t buffer[BLOCKRAM_VDATA_SIZE], batch_type bufferoffset_kvs);

void MEMCAS_READFROMBUFFER_VDATASWITHVMASKS(unsigned int index, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], value_t vdatas[VECTOR2_SIZE], unit1_type vmdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);

void MEMCAS_READFROMBUFFER_VDATASANDVMASKS(unsigned int index, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], vmdata_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);

void MEMCAS_WRITETOBUFFER_VDATAWITHVMASK(unsigned int index, keyvalue_vbuffer_t buffer[BLOCKRAM_SIZE], value_t vdata, unit1_type vmdata, batch_type bufferoffset_kvs);

void MEMCAS_WRITETOBUFFER_VDATASANDVMASKS(unsigned int index, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t datas[VECTOR2_SIZE], batch_type bufferoffset_kvs);

void MEMCAS_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS(unsigned int indexes[VDATA_PACKINGSIZE], keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VDATA_PACKINGSIZE], batch_type bufferoffset_kvs);

void MEMCAS_WRITETOBUFFER_VDATASANDVMASKS1_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCAS_WRITETOBUFFER_VDATASANDVMASKS2_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCAS_WRITETOBUFFER_VDATASANDVMASKS3_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCAS_WRITETOBUFFER_VDATASANDVMASKS4_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCAS_WRITETOBUFFER_VDATASANDVMASKS5_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCAS_WRITETOBUFFER_VDATASANDVMASKS6_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCAS_WRITETOBUFFER_VDATASANDVMASKS7_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCAS_WRITETOBUFFER_VDATASANDVMASKS8_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCAS_WRITETOBUFFER_VDATASANDVMASKS9_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCAS_WRITETOBUFFER_VDATASANDVMASKS10_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCAS_WRITETOBUFFER_VDATASANDVMASKS11_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer10[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCAS_WRITETOBUFFER_VDATASANDVMASKS12_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer10[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer11[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);

void MEMCAS_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS1_ANDREPLICATE(unsigned int indexes[VDATA_PACKINGSIZE], keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCAS_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS2_ANDREPLICATE(unsigned int indexes[VDATA_PACKINGSIZE], keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCAS_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS3_ANDREPLICATE(unsigned int indexes[VDATA_PACKINGSIZE], keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCAS_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS4_ANDREPLICATE(unsigned int indexes[VDATA_PACKINGSIZE], keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCAS_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS5_ANDREPLICATE(unsigned int indexes[VDATA_PACKINGSIZE], keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCAS_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS6_ANDREPLICATE(unsigned int indexes[VDATA_PACKINGSIZE], keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCAS_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS7_ANDREPLICATE(unsigned int indexes[VDATA_PACKINGSIZE], keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCAS_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS8_ANDREPLICATE(unsigned int indexes[VDATA_PACKINGSIZE], keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCAS_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS9_ANDREPLICATE(unsigned int indexes[VDATA_PACKINGSIZE], keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCAS_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS10_ANDREPLICATE(unsigned int indexes[VDATA_PACKINGSIZE], keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCAS_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS11_ANDREPLICATE(unsigned int indexes[VDATA_PACKINGSIZE], keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer10[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCAS_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS12_ANDREPLICATE(unsigned int indexes[VDATA_PACKINGSIZE], keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer10[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer11[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);

// kvdram 
void MEMCAS_READFROMKVDRAM_VDATASANDVMASKS(unsigned int index, uint512_dt * kvdram, keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs);

void MEMCAS_WRITETOKVDRAM_VDATASANDVMASKS(unsigned int index, uint512_dt * kvdram, keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs);



	
	#endif
	#ifdef CONFIG_ENABLECLASS_MEM_ACCESS
// -------------------- key values -------------------- //
fetchmessage_t MEMACCESSS_readkeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs, travstate_t travstate, globalparams_t globalparams);
	
void MEMACCESSS_savekeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_buffer_t buffer[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_t globalcapsule[MAX_NUM_PARTITIONS], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], batch_type globalbaseaddress_kvs, globalparams_t globalparams);

void MEMACCESSS_readkeyvalues(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_buffer_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);

void MEMACCESSS_savekeyvalues(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_buffer_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	
// -------------------- vdata -------------------- //
void MEMACCESSS_GetXYLayoutV(unsigned int s, vmdata_t vdata[VECTOR2_SIZE], vmdata_t vdata2[VECTOR2_SIZE], unsigned int depths[VECTOR2_SIZE], unsigned int basedepth);

void MEMACCESSS_RearrangeLayoutV(unsigned int s, keyvalue_vbuffer_t vdata[VECTOR2_SIZE], keyvalue_vbuffer_t vdata2[VECTOR2_SIZE]);

void MEMACCESSS_readV(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);

void MEMACCESSS_saveV(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);

void MEMACCESSS_readANDRVchunks1(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalparams_t globalparams);
void MEMACCESSS_readANDRVchunks2(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalparams_t globalparams);
void MEMACCESSS_readANDRVchunks3(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalparams_t globalparams);
void MEMACCESSS_readANDRVchunks4(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalparams_t globalparams);
void MEMACCESSS_readANDRVchunks5(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalparams_t globalparams);
void MEMACCESSS_readANDRVchunks6(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalparams_t globalparams);
void MEMACCESSS_readANDRVchunks7(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalparams_t globalparams);
void MEMACCESSS_readANDRVchunks8(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalparams_t globalparams);
void MEMACCESSS_readANDRVchunks9(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalparams_t globalparams);
void MEMACCESSS_readANDRVchunks10(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalparams_t globalparams);
void MEMACCESSS_readANDRVchunks11(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer10[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalparams_t globalparams);
void MEMACCESSS_readANDRVchunks12(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer10[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer11[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalparams_t globalparams);

#ifdef RANDOMVERTEXISACTIVE_ALGORITHM_XXXXXXXXXXXXXX
void MEMACCESSS_RANDreadANDRVchunks1(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, unsigned int randvids[BLOCKRAM_SIZE], unsigned int randvids_size, globalparams_t globalparams);			
void MEMACCESSS_RANDreadANDRVchunks2(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, unsigned int randvids[BLOCKRAM_SIZE], unsigned int randvids_size, globalparams_t globalparams);			
void MEMACCESSS_RANDreadANDRVchunks3(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, unsigned int randvids[BLOCKRAM_SIZE], unsigned int randvids_size, globalparams_t globalparams);			
void MEMACCESSS_RANDreadANDRVchunks4(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, unsigned int randvids[BLOCKRAM_SIZE], unsigned int randvids_size, globalparams_t globalparams);			
void MEMACCESSS_RANDreadANDRVchunks5(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, unsigned int randvids[BLOCKRAM_SIZE], unsigned int randvids_size, globalparams_t globalparams);			
void MEMACCESSS_RANDreadANDRVchunks6(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, unsigned int randvids[BLOCKRAM_SIZE], unsigned int randvids_size, globalparams_t globalparams);			
void MEMACCESSS_RANDreadANDRVchunks7(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, unsigned int randvids[BLOCKRAM_SIZE], unsigned int randvids_size, globalparams_t globalparams);			
void MEMACCESSS_RANDreadANDRVchunks8(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, unsigned int randvids[BLOCKRAM_SIZE], unsigned int randvids_size, globalparams_t globalparams);			
void MEMACCESSS_RANDreadANDRVchunks9(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, unsigned int randvids[BLOCKRAM_SIZE], unsigned int randvids_size, globalparams_t globalparams);			
void MEMACCESSS_RANDreadANDRVchunks10(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, unsigned int randvids[BLOCKRAM_SIZE], unsigned int randvids_size, globalparams_t globalparams);			
void MEMACCESSS_RANDreadANDRVchunks11(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer10[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, unsigned int randvids[BLOCKRAM_SIZE], unsigned int randvids_size, globalparams_t globalparams);			
void MEMACCESSS_RANDreadANDRVchunks12(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer10[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer11[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, unsigned int randvids[BLOCKRAM_SIZE], unsigned int randvids_size, globalparams_t globalparams);			
#endif 

// -------------------- pmasks -------------------- //
void MEMACCESSS_readpmask(uint512_dt * kvdram, uint32_type vmask_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);

// -------------------- stats -------------------- //
void MEMACCESSS_readglobalstats(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], batch_type offset_kvs, globalparams_t globalparams);

void MEMACCESSS_saveglobalstats(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], batch_type offset_kvs, globalparams_t globalparams);

tuple_t MEMACCESSS_getvptrs( uint512_dt *  kvdram, unsigned int beginoffset, unsigned int endoffset, unsigned int edgebankID);

unsigned int MEMACCESSS_getvptr(uint512_dt * kvdram, unsigned int baseoffset_kvs, unsigned int offset);

tuple_t MEMACCESSS_getvptrs_opt( uint512_dt *  kvdram, unsigned int baseoffset_kvs, unsigned int beginoffset, unsigned int endoffset, unsigned int edgebankID);

void MEMACCESSS_commitkvstats(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams, unsigned int offset);

void MEMACCESSS_commitkvstats2(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams, unsigned int offset, unsigned int size);
void MEMACCESSS_commitkvstats2(uint512_dt * kvdram, keyvalue_t * buffer, globalparams_t globalparams, unsigned int offset, unsigned int size);

void MEMACCESSS_retreievekvstats(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams, unsigned int offset, unsigned int size);
void MEMACCESSS_retreievekvstats(uint512_dt * kvdram, keyvalue_t * buffer, globalparams_t globalparams, unsigned int offset, unsigned int size);

void MEMACCESSS_accumkvstats(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams);

void MEMACCESSS_commitkvstats(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams);

// -------------------- multiple accesses -------------------- //
void MEMACCESSS_readmanypmask1(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
void MEMACCESSS_readmanypmask2(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
void MEMACCESSS_readmanypmask3(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
void MEMACCESSS_readmanypmask4(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
void MEMACCESSS_readmanypmask5(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
void MEMACCESSS_readmanypmask6(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
void MEMACCESSS_readmanypmask7(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
void MEMACCESSS_readmanypmask8(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE],uint32_type vmask7_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
void MEMACCESSS_readmanypmask9(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE],uint32_type vmask7_p[BLOCKRAM_SIZE],uint32_type vmask8_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
void MEMACCESSS_readmanypmask10(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE],uint32_type vmask7_p[BLOCKRAM_SIZE],uint32_type vmask8_p[BLOCKRAM_SIZE],uint32_type vmask9_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
void MEMACCESSS_readmanypmask11(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE],uint32_type vmask7_p[BLOCKRAM_SIZE],uint32_type vmask8_p[BLOCKRAM_SIZE],uint32_type vmask9_p[BLOCKRAM_SIZE],uint32_type vmask10_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
void MEMACCESSS_readmanypmask12(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE],uint32_type vmask7_p[BLOCKRAM_SIZE],uint32_type vmask8_p[BLOCKRAM_SIZE],uint32_type vmask9_p[BLOCKRAM_SIZE],uint32_type vmask10_p[BLOCKRAM_SIZE],uint32_type vmask11_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);

// -------------------- others -------------------- //
void MEMACCESSS_copyvs(uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], globalparams_t globalparamsK);




	
	#endif
	#ifdef CONFIG_ENABLECLASS_PROCESSEDGES_AND_REDUCEUPDATES //////////
#define PE_SETSZ 16
#define RU_SETSZ 8
value_t PANDRS_processfunc(value_t udata, value_t edgew, unsigned int GraphAlgo);

value_t PANDRS_reducefunc(value_t vtemp, value_t res, unsigned int GraphIter, unsigned int GraphAlgo);

void PANDRS_processorreducevector(bool enx, unsigned int mode, unsigned int col, unsigned int _loc, keyvalue_t kvdata, 
		keyvalue_vbuffer_t vbuffer[BLOCKRAM_VDATA_SIZE], keyvalue_buffer_t buffer[SOURCEBLOCKRAM_SIZE], unsigned int * loadcount, 
			unsigned int GraphAlgoClass, unsigned int upperlimit, sweepparams_t sweepparams, globalparams_t globalparams);

void PANDRS_GetXYLayoutV(unsigned int s, unsigned int depths[PE_SETSZ], unsigned int basedepth);

parsededge_t PANDRS_PARSEEDGE(uint32_type data);

fetchmessage_t PANDRS_processandreduce(bool_type enable, unsigned int mode, 
		uint512_dt * edges, uint512_dt * kvdram, 
			keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE],
				keyvalue_t globalcapsule[MAX_NUM_PARTITIONS], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS],
					batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, 
						travstate_t travstate, sweepparams_t sweepparams, globalparams_t globalparams);

void REDUCES_priorreduceandbuffer(bool_type enable, keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], buffer_type chunk_size, sweepparams_t sweepparams, globalparams_t globalparams);

void REDUCES_tradreduceandbuffer(bool_type enable, uint512_dt * kvdram, keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], buffer_type chunk_size, keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], sweepparams_t sweepparams, globalparams_t globalparams);









	
	#endif 
	#ifdef CONFIG_ENABLECLASS_PROCESSEDGES
value_t PROCESSS_SPL_processfunc(value_t udata, value_t edgew, unsigned int GraphAlgo);

parsededge_t PROCESSS_PARSEEDGE(uint32_type data);

void PROCESSS_SPL_debug(unsigned int debugid,
	unsigned int i, value_t E[VECTOR2_SIZE], bool_type ens[VECTOR2_SIZE], unsigned int mask[VECTOR2_SIZE],
		value_t udataset[MAX_NUM_UNIQ_EDGES_PER_VEC], value_t maskset[VECTOR2_SIZE], value_t Vset[MAX_NUM_UNIQ_EDGES_PER_VEC], unit1_type VMset[MAX_NUM_UNIQ_EDGES_PER_VEC], vertex_t lvids[VECTOR2_SIZE],
			unsigned int incr[VECTOR2_SIZE], unsigned int lsrcvids[VECTOR2_SIZE], unsigned int ldstvids[VECTOR2_SIZE], value_t res[VECTOR2_SIZE], keyvalue_t mykeyvalue[VECTOR2_SIZE], sweepparams_t sweepparams, globalparams_t globalparams,
				unsigned int lvid_head, unsigned int srcvid_head, travstate_t travstate, unsigned int chunk_size, sliceinfos_t sliceinfos, unsigned int * activeloadcount, unsigned int * inactiveloadcount, unsigned int * debug_numinvalidheads);
				
unsigned int PROCESSS_SPL3_GETROW(unsigned int loc);

unsigned int PROCESSS_SPL3_GETVTXDATASET(unsigned int loc, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], value_t Vdatas[32], unit1_type VMdatas[32], globalparams_t globalparams);

fetchmessage_t PROCESSS_SPL_readandprocess(bool_type enable, uint512_dt * edges, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], 
		batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, sweepparams_t sweepparams, globalparams_t globalparams);	







	
	#endif 
	#ifdef CONFIG_ENABLECLASS_PROCESSEDGES2
#define PE_SETSZ 16
void PROCESSS_processvector(bool enx, unsigned int v, unsigned int loc, keyvalue_t edata, keyvalue_vbuffer_t vbuffer[BLOCKRAM_VDATA_SIZE], keyvalue_buffer_t buffer[SOURCEBLOCKRAM_SIZE], unsigned int * loadcount, unsigned int GraphAlgoClass, globalparams_t globalparams);

void PROCESSS_GetXYLayoutV(unsigned int s, unsigned int depths[VECTOR_SIZE], unsigned int basedepth);

void PROCESSS_RearrangeLayoutV(unsigned int s, uint32_type vdata[VECTOR_SIZE], uint32_type vdata2[VECTOR_SIZE]);

parsededge_t PROCESSS_PARSEEDGE(uint32_type data);

void PROCESSS_calculateoffsets(keyvalue_capsule_t * buffer);

fetchmessage_t PROCESSS_SPL_readandprocess(bool_type enable, uint512_dt * edges, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], 
		batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, sweepparams_t sweepparams, globalparams_t globalparams);






	
	#endif 
	#ifdef CONFIG_ENABLECLASS_PARTITIONUPDATES
void PARTITIONS_preparekeyvalues(bool_type enable1, bool_type enable2, unsigned int mode, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_buffer_t destbuffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[VECTOR_SIZE][MAX_NUM_PARTITIONS], step_type currentLOP, sweepparams_t sweepparams, buffer_type size_kvs, buffer_type cutoffs[VECTOR_SIZE], globalparams_t globalparams);				

void PARTITIONS_priorpartitionkeyvalues(bool_type enable1, bool_type enable2, unsigned int mode, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_buffer_t destbuffer[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], step_type currentLOP, sweepparams_t sweepparams, buffer_type size_kvs, globalparams_t globalparams);
	






	
	#endif 
	#ifdef CONFIG_ENABLECLASS_REDUCEUPDATES
#define RU_SETSZ 8
void REDUCES_reducevector(unsigned int col, keyvalue_buffer_t kvdata, keyvalue_vbuffer_t destbuffer[BLOCKRAM_SIZE], buffer_type destoffset, unsigned int upperlimit, sweepparams_t sweepparams, globalparams_t globalparams);

void REDUCES_reduceandbuffer(bool_type enable, keyvalue_buffer_t buffer[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], sweepparams_t sweepparams, globalparams_t globalparams);				

void REDUCES_priorreduceandbuffer(bool_type enable, keyvalue_buffer_t buffer[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], buffer_type chunk_size, sweepparams_t sweepparams, globalparams_t globalparams);			

void REDUCES_tradreduceandbuffer(bool_type enable, uint512_dt * kvdram, keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], buffer_type chunk_size, keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], sweepparams_t sweepparams, globalparams_t globalparams);







	
	#endif
	#ifdef CONFIG_ENABLECLASS_REDUCEUPDATES2
#define RU_SETSZ 8
void REDUCES_reducevector(bool enx, unsigned int col, keyvalue_buffer_t kvdata, keyvalue_vbuffer_t vbuffer[BLOCKRAM_VDATA_SIZE], buffer_type destoffset, unsigned int upperlimit, sweepparams_t sweepparams, globalparams_t globalparams);

void REDUCES_GetXYLayoutV(unsigned int s, unsigned int depths[VECTOR_SIZE], unsigned int basedepth);

void REDUCES_RearrangeLayoutV(unsigned int s, keyvalue_buffer_t vdata[VECTOR_SIZE], keyvalue_buffer_t vdata2[VECTOR_SIZE]);

void REDUCES_reduceandbuffer(bool_type enable, keyvalue_buffer_t buffer[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], sweepparams_t sweepparams, globalparams_t globalparams);					

void REDUCES_priorreduceandbuffer(bool_type enable, keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], buffer_type chunk_size, sweepparams_t sweepparams, globalparams_t globalparams);

void REDUCES_tradreduceandbuffer(bool_type enable, uint512_dt * kvdram, keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], buffer_type chunk_size, keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], sweepparams_t sweepparams, globalparams_t globalparams);


	
	#endif
	#ifdef CONFIG_ENABLECLASS_ACTS_MERGE
unsigned int MERGES_SPLIT_actvpstatsoffset(globalparams_t globalparams);

value_t MERGES_SPLIT_applyfunc(value_t vtemp, value_t res, unsigned int GraphIter, unsigned int GraphAlgo);

void MERGES_SPLIT_broadcastVs1(uint512_dt * kvdram0, uint512_dt * vdram);
void MERGES_SPLIT_broadcastVs2(uint512_dt * kvdram0,uint512_dt * kvdram1, uint512_dt * vdram);
void MERGES_SPLIT_broadcastVs3(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2, uint512_dt * vdram);
void MERGES_SPLIT_broadcastVs4(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3, uint512_dt * vdram);
void MERGES_SPLIT_broadcastVs5(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4, uint512_dt * vdram);
void MERGES_SPLIT_broadcastVs6(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5, uint512_dt * vdram);
void MERGES_SPLIT_broadcastVs7(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6, uint512_dt * vdram);
void MERGES_SPLIT_broadcastVs8(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7, uint512_dt * vdram);
void MERGES_SPLIT_broadcastVs9(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8, uint512_dt * vdram);
void MERGES_SPLIT_broadcastVs10(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9, uint512_dt * vdram);
void MERGES_SPLIT_broadcastVs11(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10, uint512_dt * vdram);
void MERGES_SPLIT_broadcastVs12(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11, uint512_dt * vdram);

void MERGES_SPLIT_BASICbroadcastVs1(uint512_dt * kvdram0, uint512_dt * vdram);
void MERGES_SPLIT_BASICbroadcastVs2(uint512_dt * kvdram0,uint512_dt * kvdram1, uint512_dt * vdram);
void MERGES_SPLIT_BASICbroadcastVs3(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2, uint512_dt * vdram);
void MERGES_SPLIT_BASICbroadcastVs4(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3, uint512_dt * vdram);
void MERGES_SPLIT_BASICbroadcastVs5(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4, uint512_dt * vdram);
void MERGES_SPLIT_BASICbroadcastVs6(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5, uint512_dt * vdram);
void MERGES_SPLIT_BASICbroadcastVs7(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6, uint512_dt * vdram);
void MERGES_SPLIT_BASICbroadcastVs8(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7, uint512_dt * vdram);
void MERGES_SPLIT_BASICbroadcastVs9(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8, uint512_dt * vdram);
void MERGES_SPLIT_BASICbroadcastVs10(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9, uint512_dt * vdram);
void MERGES_SPLIT_BASICbroadcastVs11(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10, uint512_dt * vdram);
void MERGES_SPLIT_BASICbroadcastVs12(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11, uint512_dt * vdram);

void MERGES_SPLIT_mergeVs1(uint512_dt * kvdram0, uint512_dt * vdram, keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE], globalparams_t globalparams, globalparams_t globalparamsv);				
void MERGES_SPLIT_mergeVs2(uint512_dt * kvdram0,uint512_dt * kvdram1, uint512_dt * vdram, keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE], globalparams_t globalparams, globalparams_t globalparamsv);				
void MERGES_SPLIT_mergeVs3(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2, uint512_dt * vdram, keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE], globalparams_t globalparams, globalparams_t globalparamsv);				
void MERGES_SPLIT_mergeVs4(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3, uint512_dt * vdram, keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE], globalparams_t globalparams, globalparams_t globalparamsv);				
void MERGES_SPLIT_mergeVs5(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4, uint512_dt * vdram, keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE], globalparams_t globalparams, globalparams_t globalparamsv);				
void MERGES_SPLIT_mergeVs6(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5, uint512_dt * vdram, keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE], globalparams_t globalparams, globalparams_t globalparamsv);				
void MERGES_SPLIT_mergeVs7(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6, uint512_dt * vdram, keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE], globalparams_t globalparams, globalparams_t globalparamsv);				
void MERGES_SPLIT_mergeVs8(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7, uint512_dt * vdram, keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer7[BLOCKRAM_SIZE], globalparams_t globalparams, globalparams_t globalparamsv);				
void MERGES_SPLIT_mergeVs9(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8, uint512_dt * vdram, keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer7[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer8[BLOCKRAM_SIZE], globalparams_t globalparams, globalparams_t globalparamsv);				
void MERGES_SPLIT_mergeVs10(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9, uint512_dt * vdram, keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer7[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer8[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer9[BLOCKRAM_SIZE], globalparams_t globalparams, globalparams_t globalparamsv);				
void MERGES_SPLIT_mergeVs11(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10, uint512_dt * vdram, keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer7[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer8[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer9[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer10[BLOCKRAM_SIZE], globalparams_t globalparams, globalparams_t globalparamsv);				
void MERGES_SPLIT_mergeVs12(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11, uint512_dt * vdram, keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer7[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer8[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer9[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer10[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer11[BLOCKRAM_SIZE], globalparams_t globalparams, globalparams_t globalparamsv);				

void TOPS_topkernelS(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC, uint512_dt * vdram);








	
	#endif 
	#ifdef CONFIG_ENABLECLASS_ACTS
fetchmessage_t ACTSS_fetchkeyvalues(bool_type enable, unsigned int mode,  uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], 
	batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, sweepparams_t sweepparams, globalparams_t globalparams, unsigned int edgebankID);

void ACTSS_commitkeyvalues(bool_type enable1, bool_type enable2, unsigned int mode, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],
	keyvalue_buffer_t destbuffer[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_t globalcapsule[MAX_NUM_PARTITIONS], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], 
		batch_type destbaseaddr_kvs, buffer_type chunk_size, sweepparams_t sweepparams, globalparams_t globalparams);

void ACTSS_PRIOR_commitkeyvalues(bool_type enable1, bool_type enable2, unsigned int mode, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],
	keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_buffer_t destbuffer[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_t globalcapsule[MAX_NUM_PARTITIONS], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], 
			batch_type destbaseaddr_kvs, buffer_type chunk_size, sweepparams_t sweepparams, globalparams_t globalparams);
			
void ACTSS_actspipeline(bool_type enable1, bool_type enable2, unsigned int mode, keyvalue_buffer_t buffer_setof1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_capsule_t capsule_so1[VECTOR_SIZE][MAX_NUM_PARTITIONS], 
					keyvalue_buffer_t buffer_setof8[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_capsule_t capsule_so8[MAX_NUM_PARTITIONS],
						unsigned int currentLOP, sweepparams_t sweepparams, buffer_type cutoffs[VECTOR_SIZE], batch_type shiftcount, globalparams_t globalparams);

void ACTSS_actit(bool_type enable, unsigned int mode,
		 uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS],					 
		globalparams_t globalparams, globalposition_t globalposition, sweepparams_t sweepparams, travstate_t ptravstate, batch_type sourcebaseaddr_kvs, batch_type destbaseaddr_kvs,
		bool_type resetenv, bool_type flush, unsigned int edgebankID);

void ACTSS_priorit(bool_type enable, unsigned int mode,
		 uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], 
		globalparams_t globalparams, sweepparams_t sweepparams, travstate_t ptravstate, batch_type sourcebaseaddr_kvs, batch_type destbaseaddr_kvs,
		bool_type resetenv, bool_type flush, unsigned int edgebankID);
		
void ACTSS_tradit(bool_type enable, unsigned int mode,
		 uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], 
		globalparams_t globalparams, sweepparams_t sweepparams, travstate_t ptravstate, batch_type sourcebaseaddr_kvs, batch_type destbaseaddr_kvs,
		bool_type resetenv, bool_type flush, unsigned int edgebankID);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

fetchmessage_t ACTSS_magic(bool_type enable, unsigned int mode, unsigned int mode2,
		 uint512_dt * kvdram, 
			keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], 
				keyvalue_t globalcapsule[MAX_NUM_PARTITIONS], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS],
					batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, 
						travstate_t travstate, sweepparams_t sweepparams, globalparams_t globalparams, unsigned int edgebankID);
						
						
							
	#endif 		
void TOPS_NU_processit_splitdstvxs( uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK,
		unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID);

void TOPS_NU_partitionit( uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], globalparams_t globalparams, unsigned int edgebankID);
	
void TOPS_NU_reduceit( uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparams, unsigned int edgebankID);					

void TOPS_NU_dispatch(bool_type en_process, bool_type en_partition, bool_type en_reduce,  uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE],
		batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparamsE, globalparams_t globalparamsK,
			unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID);

void TOPS_NU_dispatch_reduce( uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK,
		unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID);

#ifdef CONFIG_ENABLECLASS_TOPNUSRCV_NUDSTV
void TOPS_NU_topkernelproc_embedded(
		
	uint512_dt * kvdram);
	
void TOPS_NU_topkernelP1(
	uint512_dt * kvdram0,
	uint512_dt * vdram
	);
void TOPS_NU_topkernelP2(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * vdram
	);
void TOPS_NU_topkernelP3(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * vdram
	);
void TOPS_NU_topkernelP4(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * vdram
	);
void TOPS_NU_topkernelP5(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * kvdram4,
	uint512_dt * vdram
	);
void TOPS_NU_topkernelP6(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * kvdram4,
	uint512_dt * kvdram5,
	uint512_dt * vdram
	);
void TOPS_NU_topkernelP7(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * kvdram4,
	uint512_dt * kvdram5,
	uint512_dt * kvdram6,
	uint512_dt * vdram
	);
void TOPS_NU_topkernelP8(
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
void TOPS_NU_topkernelP9(
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
void TOPS_NU_topkernelP10(
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
void TOPS_NU_topkernelP11(
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
void TOPS_NU_topkernelP12(
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






void TOPS_U_processit( uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], keyvalue_t globalstatsbuffer[BLOCKRAM_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition,
		unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID);							

void TOPS_U_dispatch(bool_type en_process, bool_type en_partition, bool_type en_reduce,  uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], keyvalue_t globalstatsbuffer[BLOCKRAM_SIZE],
		batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition,
			unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID);

void TOPS_U_dispatch_reduce(uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK,	
					unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID);
					
#ifdef CONFIG_ENABLECLASS_TOP_USRCV_UDSTV
void TOPS_U_topkernelproc_embedded(unsigned int globalid, unsigned int localid, unsigned int en_process, unsigned int en_partition, unsigned int en_reduce,  uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], keyvalue_t globalstatsbuffer[BLOCKRAM_SIZE], globalposition_t globalposition);					
	
// top
void TOPS_U_topkernelP1(
	uint512_dt * kvdram0,
	uint512_dt * vdram
	);
void TOPS_U_topkernelP2(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * vdram
	);
void TOPS_U_topkernelP3(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * vdram
	);
void TOPS_U_topkernelP4(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * vdram
	);
void TOPS_U_topkernelP5(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * kvdram4,
	uint512_dt * vdram
	);
void TOPS_U_topkernelP6(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * kvdram4,
	uint512_dt * kvdram5,
	uint512_dt * vdram
	);
void TOPS_U_topkernelP7(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * kvdram4,
	uint512_dt * kvdram5,
	uint512_dt * kvdram6,
	uint512_dt * vdram
	);
void TOPS_U_topkernelP8(
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
void TOPS_U_topkernelP9(
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
void TOPS_U_topkernelP10(
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
void TOPS_U_topkernelP11(
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
void TOPS_U_topkernelP12(
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
	#ifdef SW_ALLINONE 
	actsutility * actsutilityobj;
	mydebug * mydebugobj;
	#endif 
};
#endif 
#endif 




