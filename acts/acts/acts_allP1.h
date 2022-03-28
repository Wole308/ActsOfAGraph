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
unsigned int UTILP1_amin(unsigned int val1, unsigned int val2);
unsigned int UTILP1_amax(unsigned int val1, unsigned int val2);
unsigned int UTILP1_aplus(unsigned int val1, unsigned int val2);

// functions (allignment)
batch_type UTILP1_allignlower_KV(batch_type val);
batch_type UTILP1_allignhigher_KV(batch_type val);
batch_type UTILP1_allignlower_KV2(batch_type val);
batch_type UTILP1_allignhigher_KV2(batch_type val);
batch_type UTILP1_allignhigher_FACTOR(batch_type val, unsigned int _FACTOR);
batch_type UTILP1_allignlower_FACTOR(batch_type val, unsigned int _FACTOR);

// functions (bit manipulation)
unsigned int UTILP1_GETMASK_UINT(unsigned int index, unsigned int size);
unsigned int UTILP1_READFROM_UINT(unsigned int data, unsigned int index, unsigned int size);
void UTILP1_WRITETO_UINT(unsigned int * data, unsigned int index, unsigned int size, unsigned int value);
unsigned int UTILP1_READBITSFROM_UINTV(uint32_type data, unsigned int index, unsigned int size);
void UTILP1_WRITEBITSTO_UINTV(uint32_type * data, unsigned int index, unsigned int size, unsigned int value);
unsigned int UTILP1_SWREADBITSFROM_UINTV(uint32_type data, unsigned int index, unsigned int size);
void UTILP1_SWWRITEBITSTO_UINTV(uint32_type * data, unsigned int index, unsigned int size, unsigned int value);

// functions (converters)
keyvalue_t UTILP1_GETKV(keyvalue_buffer_t data);
keyvalue_buffer_t UTILP1_GETKV(keyvalue_t data);
keyy_t UTILP1_GETK(uint32_type data);
value_t UTILP1_GETV(uint32_type data);
keyy_t UTILP1_GETKEYENTRY(uint512_dt data, unsigned int v);
unsigned int UTILP1_GETLOCALVID(unsigned int vid, unsigned int instid);
unsigned int UTILP1_GETREALVID(unsigned int lvid, unsigned int instid);
unsigned int UTILP1_GET_PROCESSEDGESPARTITIONSIZEKVS2(globalparams_t globalparams);

unsigned int UTILP1_GetData(uint512_dt * kvdram, unsigned int offset_kvs, unsigned int index);
void UTILP1_SetData(uint512_dt * kvdram, unsigned int offset_kvs, unsigned int index, unsigned int data);
unsigned int UTILP1_GetFirstData(uint512_dt * kvdram, unsigned int offset_kvs);
void UTILP1_SetFirstData(uint512_dt * kvdram, unsigned int offset_kvs, unsigned int data);
void UTILP1_GetDataset(uint512_dt * kvdram, unsigned int offset_kvs, value_t datas[VECTOR2_SIZE]);
void UTILP1_SetDataset(uint512_dt * kvdram, unsigned int offset_kvs, value_t datas[VECTOR2_SIZE]);

// functions (actsproc utilities)
batch_type UTILP1_getskipsize(step_type currentLOP, bool_type sourceORdest, globalparams_t globalparams);
batch_type UTILP1_getrangeforeachllop(globalparams_t globalparams);
buffer_type UTILP1_getchunksize_kvs(buffer_type buffer_size, travstate_t travstate, buffer_type localoffset);
buffer_type UTILP1_getpartitionwritesz(buffer_type realsize_kvs, buffer_type bramoffset_kvs);
void UTILP1_calculateoffsets(keyvalue_capsule_t * buffer, buffer_type size);
void UTILP1_calculatemanyunallignedoffsets(keyvalue_capsule_t buffer[VECTOR_SIZE][MAX_NUM_PARTITIONS], buffer_type size, batch_type base, batch_type skipspacing);
batch_type UTILP1_get_num_source_partitions(step_type currentLOP);
globalparams_t UTILP1_getglobalparams(uint512_dt * kvdram, unsigned int banksection);
globalparams_t UTILP1_getglobalparamsUNUSED(uint512_dt * kvdram, unsigned int banksection);
sweepparams_t UTILP1_getsweepparams(globalparams_t globalparams, step_type currentLOP, batch_type source_partition);
travstate_t UTILP1_gettravstate(bool_type enable, uint512_dt * kvdram, globalparams_t globalparams, step_type currentLOP, batch_type sourcestatsmarker);
void UTILP1_settravstate(bool_type enable, uint512_dt * kvdram, globalparams_t globalparams, batch_type offset, unsigned int value);
partition_type UTILP1_getpartition(bool_type enable, unsigned int mode, keyvalue_buffer_t keyvalue, step_type currentLOP, vertex_t upperlimit, unsigned int upperpartition, unsigned int batch_range_pow);

// functions (resets)
void UTILP1_resetvalues(keyvalue_t * buffer, buffer_type size, unsigned int resetval);
void UTILP1_resetvalues(keyvalue_capsule_t * buffer, buffer_type size, unsigned int resetval);
void UTILP1_resetvalues(value_t * buffer, buffer_type size, unsigned int resetval);
void UTILP1_resetkeysandvalues(keyvalue_t * buffer, buffer_type size, unsigned int resetval);
void UTILP1_resetkvstatvalues(uint512_dt * kvdram, globalparams_t globalparams);
void UTILP1_resetkvstatvalues(uint512_dt * kvdram, unsigned int size_kvs, globalparams_t globalparams);
void UTILP1_accumkvstats(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams);
void UTILP1_increment_graphiteration(uint512_dt * kvdram, globalparams_t globalparams);
void UTILP1_resetenvbuffers(keyvalue_capsule_t capsule_so1[VECTOR_SIZE][MAX_NUM_PARTITIONS], keyvalue_capsule_t capsule_so8[MAX_NUM_PARTITIONS]);
void UTILP1_resetenvbuffer(keyvalue_capsule_t capsule_so8[MAX_NUM_PARTITIONS]);






	
	#endif 
	#ifdef CONFIG_ENABLECLASS_ALGO_FUNCS
value_t PROCESSP1_processfunc(value_t udata, value_t edgew, unsigned int GraphAlgo);

value_t REDUCEP1_reducefunc(value_t vtemp, value_t res, unsigned int GraphIter, unsigned int GraphAlgo);






	
	#endif 
	#ifdef CONFIG_ENABLECLASS_MEM_CONVERT_AND_ACCESS
// util 
unsigned int MEMCAP1_READVDATA(keyvalue_vbuffer_t wideword);
unsigned int MEMCAP1_READVMASK(keyvalue_vbuffer_t wideword);
unsigned int MEMCAP1_READVDATA2(keyvalue_vbuffer_t wideword, unsigned int offsetof_vdata);
unsigned int MEMCAP1_READVMASK2(keyvalue_vbuffer_t wideword, unsigned int offsetof_vmask);

void MEMCAP1_WRITEVDATA(keyvalue_vbuffer_t * wideword, value_t vdata);
void MEMCAP1_WRITEVMASK(keyvalue_vbuffer_t * wideword, unit1_type vmask);
void MEMCAP1_WRITEVDATA2(keyvalue_vbuffer_t * wideword, value_t vdata, unsigned int offsetof_vdata);
void MEMCAP1_WRITEVMASK2(keyvalue_vbuffer_t * wideword, unit1_type vmask, unsigned int offsetof_vmask);

tuple_t MEMCAP1_READVDATAWITHVMASK(keyvalue_vbuffer_t wideword);
void MEMCAP1_WRITEVDATAWITHVMASK(keyvalue_vbuffer_t * wideword, value_t vdata, unit1_type vmask);
tuple_t MEMCAP1_READVDATAWITHVMASK2(keyvalue_vbuffer_t wideword, unsigned int offsetof_vdata, unsigned int offsetof_vmask);
void MEMCAP1_WRITEVDATAWITHVMASK2(keyvalue_vbuffer_t * wideword, value_t vdata, unit1_type vmask, unsigned int offsetof_vdata, unsigned int offsetof_vmask);

// any data 
uint32_type MEMCAP1_READDATAFROMBUFFER(unsigned int index, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type bufferoffset_kvs);

void MEMCAP1_WRITEDATATOBUFFER(unsigned int index, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], uint32_type data, batch_type bufferoffset_kvs);

keyvalue_vbuffer_t MEMCAP1_READDATAFROMDRAM(unsigned int index, uint512_dt * kvdram, batch_type baseoffset_kvs, batch_type offset_kvs);

void MEMCAP1_WRITEDATATODRAM(unsigned int index, uint512_dt * kvdram, uint32_type data, batch_type baseoffset_kvs, batch_type offset_kvs);

// vdatas
void MEMCAP1_WRITETOBUFFER_VDATA(unsigned int v, unsigned int index, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], value_t vdata, batch_type bufferoffset_kvs);

void MEMCAP1_WRITETOBUFFER_VDATA(unsigned int index, keyvalue_vbuffer_t buffer[BLOCKRAM_SIZE], value_t vdata, batch_type bufferoffset_kvs);

void MEMCAP1_WRITETOBUFFER_VDATAS(unsigned int index, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], value_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);

void MEMCAP1_WRITETOBUFFERWITHDEPTHS_VDATAS(unsigned int indexes[VDATA_PACKINGSIZE], keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], value_t vdatas[VDATA_PACKINGSIZE], batch_type bufferoffset_kvs);			

value_t MEMCAP1_READFROMBUFFER_VDATA(unsigned int index, keyvalue_vbuffer_t buffer[BLOCKRAM_VDATA_SIZE], batch_type bufferoffset_kvs);

void MEMCAP1_READFROMBUFFER_VDATAS(unsigned int index, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], value_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);

// vdatas & vmasks 
vmdata_t MEMCAP1_READFROMBUFFER_VDATAWITHVMASK(unsigned int index, keyvalue_vbuffer_t buffer[BLOCKRAM_VDATA_SIZE], batch_type bufferoffset_kvs);
void MEMCAP1_WRITETOBUFFER_VDATAWITHVMASK(unsigned int index, keyvalue_vbuffer_t buffer[BLOCKRAM_SIZE], value_t vdata, unit1_type vmdata, batch_type bufferoffset_kvs);

vmdata_t MEMCAP1_READFROMBUFFER_VDATAWITHVMASK2(unsigned int index, unsigned int bitoffset, keyvalue_vbuffer_t buffer[BLOCKRAM_VDATA_SIZE], batch_type bufferoffset_kvs);
void MEMCAP1_WRITETOBUFFER_VDATAWITHVMASK2(unsigned int index, keyvalue_vbuffer_t buffer[BLOCKRAM_SIZE], value_t vdata, unit1_type vmdata, batch_type bufferoffset_kvs);

void MEMCAP1_WRITETOBUFFER_VDATASANDVMASKS(unsigned int index, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t datas[VECTOR2_SIZE], batch_type bufferoffset_kvs);

void MEMCAP1_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS(unsigned int indexes[VDATA_PACKINGSIZE], keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VDATA_PACKINGSIZE], batch_type bufferoffset_kvs);

void MEMCAP1_WRITETOBUFFER_VDATASANDVMASKS1_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCAP1_WRITETOBUFFER_VDATASANDVMASKS2_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCAP1_WRITETOBUFFER_VDATASANDVMASKS3_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCAP1_WRITETOBUFFER_VDATASANDVMASKS4_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCAP1_WRITETOBUFFER_VDATASANDVMASKS5_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCAP1_WRITETOBUFFER_VDATASANDVMASKS6_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCAP1_WRITETOBUFFER_VDATASANDVMASKS7_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCAP1_WRITETOBUFFER_VDATASANDVMASKS8_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCAP1_WRITETOBUFFER_VDATASANDVMASKS9_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCAP1_WRITETOBUFFER_VDATASANDVMASKS10_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCAP1_WRITETOBUFFER_VDATASANDVMASKS11_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer10[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCAP1_WRITETOBUFFER_VDATASANDVMASKS12_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer10[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer11[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);

void MEMCAP1_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS1_ANDREPLICATE(unsigned int indexes[VDATA_PACKINGSIZE], keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCAP1_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS2_ANDREPLICATE(unsigned int indexes[VDATA_PACKINGSIZE], keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCAP1_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS3_ANDREPLICATE(unsigned int indexes[VDATA_PACKINGSIZE], keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCAP1_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS4_ANDREPLICATE(unsigned int indexes[VDATA_PACKINGSIZE], keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCAP1_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS5_ANDREPLICATE(unsigned int indexes[VDATA_PACKINGSIZE], keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCAP1_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS6_ANDREPLICATE(unsigned int indexes[VDATA_PACKINGSIZE], keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCAP1_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS7_ANDREPLICATE(unsigned int indexes[VDATA_PACKINGSIZE], keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCAP1_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS8_ANDREPLICATE(unsigned int indexes[VDATA_PACKINGSIZE], keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCAP1_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS9_ANDREPLICATE(unsigned int indexes[VDATA_PACKINGSIZE], keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCAP1_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS10_ANDREPLICATE(unsigned int indexes[VDATA_PACKINGSIZE], keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCAP1_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS11_ANDREPLICATE(unsigned int indexes[VDATA_PACKINGSIZE], keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer10[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCAP1_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS12_ANDREPLICATE(unsigned int indexes[VDATA_PACKINGSIZE], keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer10[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer11[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);

// kvdram 
void MEMCAP1_READFROMKVDRAM_VDATASANDVMASKS(unsigned int index, uint512_dt * kvdram, keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs);

void MEMCAP1_WRITETOKVDRAM_VDATASANDVMASKS(unsigned int index, uint512_dt * kvdram, keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs);



	
	#endif
	#ifdef CONFIG_ENABLECLASS_MEM_ACCESS
// -------------------- key values -------------------- //
fetchmessage_t MEMACCESSP1_readkeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs, travstate_t travstate, globalparams_t globalparams);
	
void MEMACCESSP1_savekeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_buffer_t buffer[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_t globalcapsule[MAX_NUM_PARTITIONS], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], batch_type globalbaseaddress_kvs, globalparams_t globalparams);

// -------------------- vdata -------------------- //
void MEMACCESSP1_GetXYLayoutV(unsigned int s, vmdata_t vdata[VECTOR2_SIZE], vmdata_t vdata2[VECTOR2_SIZE], unsigned int depths[VECTOR2_SIZE], unsigned int basedepth);

void MEMACCESSP1_RearrangeLayoutV(unsigned int s, keyvalue_vbuffer_t vdata[VECTOR2_SIZE], keyvalue_vbuffer_t vdata2[VECTOR2_SIZE]);

void MEMACCESSP1_readV(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);

void MEMACCESSP1_saveV(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], pmask_dt pmask[BLOCKRAM_CURRPMASK_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs, batch_type bufferoffset_kvs, buffer_type size_kvs, globalposition_t globalposition, globalparams_t globalparams);				

void MEMACCESSP1_readANDRVchunks1(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparams);
void MEMACCESSP1_readANDRVchunks2(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparams);
void MEMACCESSP1_readANDRVchunks3(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparams);
void MEMACCESSP1_readANDRVchunks4(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparams);
void MEMACCESSP1_readANDRVchunks5(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparams);
void MEMACCESSP1_readANDRVchunks6(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparams);
void MEMACCESSP1_readANDRVchunks7(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparams);
void MEMACCESSP1_readANDRVchunks8(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparams);
void MEMACCESSP1_readANDRVchunks9(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparams);
void MEMACCESSP1_readANDRVchunks10(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparams);
void MEMACCESSP1_readANDRVchunks11(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer10[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparams);
void MEMACCESSP1_readANDRVchunks12(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer10[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer11[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparams);

// -------------------- pmasks -------------------- //
void MEMACCESSP1_readpmask(uint512_dt * kvdram, pmask_dt pmask[BLOCKRAM_CURRPMASK_SIZE], batch_type offset_kvs, batch_type size_kvs);

// -------------------- stats -------------------- //
void MEMACCESSP1_readglobalstats(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], batch_type offset_kvs, globalparams_t globalparams);

void MEMACCESSP1_saveglobalstats(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], batch_type offset_kvs, globalparams_t globalparams);

tuple_t MEMACCESSP1_getvptrs( uint512_dt *  kvdram, unsigned int beginoffset, unsigned int endoffset, unsigned int edgebankID);

unsigned int MEMACCESSP1_getvptr(uint512_dt * kvdram, unsigned int baseoffset_kvs, unsigned int offset);

tuple_t MEMACCESSP1_getvptrs_opt( uint512_dt *  kvdram, unsigned int baseoffset_kvs, unsigned int beginoffset, unsigned int endoffset, unsigned int edgebankID);

void MEMACCESSP1_commitkvstats(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams, unsigned int offset);

void MEMACCESSP1_commitkvstats2(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams, unsigned int offset, unsigned int size);
void MEMACCESSP1_commitkvstats2(uint512_dt * kvdram, keyvalue_t * buffer, globalparams_t globalparams, unsigned int offset, unsigned int size);

void MEMACCESSP1_retreievekvstats(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams, unsigned int offset, unsigned int size);
void MEMACCESSP1_retreievekvstats(uint512_dt * kvdram, keyvalue_t * buffer, globalparams_t globalparams, unsigned int offset, unsigned int size);

void MEMACCESSP1_accumkvstats(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams);

void MEMACCESSP1_commitkvstats(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams);

// -------------------- multiple accesses -------------------- //
void MEMACCESSP1_readmanypmask1(uint512_dt * kvdram, pmask_dt pmask0[BLOCKRAM_CURRPMASK_SIZE], batch_type offset_kvs, batch_type size_kvs);
void MEMACCESSP1_readmanypmask2(uint512_dt * kvdram, pmask_dt pmask0[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask1[BLOCKRAM_CURRPMASK_SIZE], batch_type offset_kvs, batch_type size_kvs);
void MEMACCESSP1_readmanypmask3(uint512_dt * kvdram, pmask_dt pmask0[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask1[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask2[BLOCKRAM_CURRPMASK_SIZE], batch_type offset_kvs, batch_type size_kvs);
void MEMACCESSP1_readmanypmask4(uint512_dt * kvdram, pmask_dt pmask0[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask1[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask2[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask3[BLOCKRAM_CURRPMASK_SIZE], batch_type offset_kvs, batch_type size_kvs);
void MEMACCESSP1_readmanypmask5(uint512_dt * kvdram, pmask_dt pmask0[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask1[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask2[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask3[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask4[BLOCKRAM_CURRPMASK_SIZE], batch_type offset_kvs, batch_type size_kvs);
void MEMACCESSP1_readmanypmask6(uint512_dt * kvdram, pmask_dt pmask0[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask1[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask2[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask3[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask4[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask5[BLOCKRAM_CURRPMASK_SIZE], batch_type offset_kvs, batch_type size_kvs);
void MEMACCESSP1_readmanypmask7(uint512_dt * kvdram, pmask_dt pmask0[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask1[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask2[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask3[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask4[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask5[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask6[BLOCKRAM_CURRPMASK_SIZE], batch_type offset_kvs, batch_type size_kvs);
void MEMACCESSP1_readmanypmask8(uint512_dt * kvdram, pmask_dt pmask0[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask1[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask2[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask3[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask4[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask5[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask6[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask7[BLOCKRAM_CURRPMASK_SIZE], batch_type offset_kvs, batch_type size_kvs);
void MEMACCESSP1_readmanypmask9(uint512_dt * kvdram, pmask_dt pmask0[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask1[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask2[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask3[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask4[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask5[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask6[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask7[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask8[BLOCKRAM_CURRPMASK_SIZE], batch_type offset_kvs, batch_type size_kvs);
void MEMACCESSP1_readmanypmask10(uint512_dt * kvdram, pmask_dt pmask0[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask1[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask2[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask3[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask4[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask5[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask6[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask7[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask8[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask9[BLOCKRAM_CURRPMASK_SIZE], batch_type offset_kvs, batch_type size_kvs);
void MEMACCESSP1_readmanypmask11(uint512_dt * kvdram, pmask_dt pmask0[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask1[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask2[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask3[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask4[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask5[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask6[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask7[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask8[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask9[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask10[BLOCKRAM_CURRPMASK_SIZE], batch_type offset_kvs, batch_type size_kvs);
void MEMACCESSP1_readmanypmask12(uint512_dt * kvdram, pmask_dt pmask0[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask1[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask2[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask3[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask4[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask5[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask6[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask7[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask8[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask9[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask10[BLOCKRAM_CURRPMASK_SIZE],pmask_dt pmask11[BLOCKRAM_CURRPMASK_SIZE], batch_type offset_kvs, batch_type size_kvs);

// -------------------- others -------------------- //
void MEMACCESSP1_copyvs(uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], globalparams_t globalparamsK);




	
	#endif
	#ifdef CONFIG_ENABLECLASS_PROCESSEDGES
keyvalue_t PROCESSP1_processvector(bool enx, unsigned int v, unsigned int loc, keyvalue_t edata, keyvalue_vbuffer_t vbuffer[BLOCKRAM_VDATA_SIZE], keyvalue_buffer_t buffer[SOURCEBLOCKRAM_SIZE], unsigned int bufferoffset_kvs, unsigned int * loadcount, unsigned int GraphAlgoClass, globalposition_t globalposition, globalparams_t globalparams);				

void PROCESSP1_processvectorB(bool enx, unsigned int v, unsigned int loc, keyvalue_t edata, keyvalue_vbuffer_t vbuffer[BLOCKRAM_VDATA_SIZE], keyvalue_buffer_t buffer[SOURCEBLOCKRAM_SIZE], unsigned int * loadcount, unsigned int GraphAlgoClass, globalposition_t globalposition, globalparams_t globalparams);

void PROCESSP1_GetXYLayoutV(unsigned int s, unsigned int depths[VECTOR_SIZE], unsigned int basedepth);

void PROCESSP1_RearrangeLayoutV(unsigned int s, keyvalue_t vdata[VECTOR_SIZE], keyvalue_t vdata2[VECTOR_SIZE]);

parsededge_t PROCESSP1_PARSEEDGE(uint32_type data);

void PROCESSP1_calculateoffsets(keyvalue_capsule_t * buffer);

fetchmessage_t PROCESSP1_readandprocess(bool_type enable, uint512_dt * edges, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], 
		batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams);






	
	#endif 
	#ifdef CONFIG_ENABLECLASS_PROCESSEDGES2
void PROCESSP1_processvector(bool enx, unsigned int v, unsigned int loc, keyvalue_t edata, keyvalue_vbuffer_t vbuffer[BLOCKRAM_VDATA_SIZE], keyvalue_buffer_t buffer[SOURCEBLOCKRAM_SIZE], unsigned int bufferoffset_kvs, unsigned int * loadcount, unsigned int GraphAlgoClass, globalposition_t globalposition, globalparams_t globalparams);			

void PROCESSP1_GetXYLayoutV(unsigned int s, unsigned int depths[VECTOR_SIZE], unsigned int basedepth);

void PROCESSP1_RearrangeLayoutV(unsigned int s, uint32_type vdata[VECTOR_SIZE], uint32_type vdata2[VECTOR_SIZE]);

parsededge_t PROCESSP1_PARSEEDGE(uint32_type data);

void PROCESSP1_calculateoffsets1(keyvalue_capsule_t * buffer);

void PROCESSP1_calculateoffsets2(keyvalue_capsule_t * buffer);

fetchmessage_t PROCESSP1_readandprocess(bool_type enable, uint512_dt * edges, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], 
		batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams);






	
	#endif 
	#ifdef CONFIG_ENABLECLASS_PARTITIONUPDATES
void PARTITIONP1_preparekeyvalues(bool_type enable1, bool_type enable2, unsigned int mode, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_buffer_t destbuffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[VECTOR_SIZE][MAX_NUM_PARTITIONS], step_type currentLOP, sweepparams_t sweepparams, buffer_type size_kvs, buffer_type cutoffs[VECTOR_SIZE], globalparams_t globalparams);				

void PARTITIONP1_priorpartitionkeyvalues(bool_type enable1, bool_type enable2, unsigned int mode, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_buffer_t destbuffer[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], step_type currentLOP, sweepparams_t sweepparams, buffer_type size_kvs, globalparams_t globalparams);
	






	
	#endif 
	#ifdef CONFIG_ENABLECLASS_REDUCEUPDATES
#define RU_SETSZ 8
void REDUCEP1_reducevector(unsigned int col, keyvalue_buffer_t kvdata, keyvalue_vbuffer_t destbuffer[BLOCKRAM_SIZE], buffer_type destoffset, unsigned int upperlimit, sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams);

void REDUCEP1_reduceandbuffer(bool_type enable, keyvalue_buffer_t buffer[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams);				

void REDUCEP1_priorreduceandbuffer(bool_type enable, keyvalue_buffer_t buffer[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], buffer_type chunk_size, sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams);			

void REDUCEP1_tradreduceandbuffer(bool_type enable, uint512_dt * kvdram, keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], buffer_type chunk_size, keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams);







	
	#endif
	#ifdef CONFIG_ENABLECLASS_REDUCEUPDATES2
void REDUCEP1_reducevector(bool enx, unsigned int col, keyvalue_buffer_t kvdata, keyvalue_vbuffer_t vbuffer[BLOCKRAM_VDATA_SIZE], buffer_type destoffset, unsigned int upperlimit, sweepparams_t sweepparams, globalparams_t globalparams);

void REDUCEP1_GetXYLayoutV(unsigned int s, unsigned int depths[VECTOR_SIZE], unsigned int basedepth);

void REDUCEP1_RearrangeLayoutV(unsigned int s, keyvalue_buffer_t vdata[VECTOR_SIZE], keyvalue_buffer_t vdata2[VECTOR_SIZE]);

void REDUCEP1_reduceandbuffer(bool_type enable, keyvalue_buffer_t buffer[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams);					

void REDUCEP1_priorreduceandbuffer(bool_type enable, keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], buffer_type chunk_size, sweepparams_t sweepparams, globalparams_t globalparams);

void REDUCEP1_tradreduceandbuffer(bool_type enable, uint512_dt * kvdram, keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], buffer_type chunk_size, keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], sweepparams_t sweepparams, globalparams_t globalparams);


	
	#endif
	#ifdef CONFIG_ENABLECLASS_ACTS_MERGE
unsigned int MERGEP1_actvpstatsoffset(globalparams_t globalparams);

void MERGEP1_RearrangeLayoutV(unsigned int s, unsigned int data[VECTOR2_SIZE], unsigned int data2[VECTOR2_SIZE]);

void MERGEP1_RearrangeLayoutA(unsigned int s, unit1_type data[VECTOR2_SIZE], unit1_type data2[VECTOR2_SIZE]);

void MERGEP1_collects(
		unsigned int instanceid,
		unsigned int voffset_kvs,
		unsigned int gvoffset_kvs,
		value_t vdatas[VECTOR2_SIZE],
		keyvalue_t actvvbuffer[VECTOR2_SIZE][ACTVVBUFFER_SIZE],
		unsigned int vpmaskbuffer[BLOCKRAM_CUMMTVPMASK_SIZE],
		unsigned int loadcount[VECTOR2_SIZE],
		unsigned int numactvvs[VECTOR2_SIZE],
		unsigned int local_actvv[VECTOR2_SIZE],
		unsigned int local_actvv2[VECTOR2_SIZE],
		unsigned int global_actvvs[VECTOR2_SIZE],
		uint32_type vpmaskVecSum[VECTOR2_SIZE]
		);

void MERGEP1_broadcastVs(uint512_dt * vdram, uint512_dt * kvdram);

void MERGEP1_mergeVs(uint512_dt * kvdram, uint512_dt * vdram);
			
void MERGEP1_mergeVs1(uint512_dt * kvdram0, uint512_dt * vdram, 
keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],			globalparams_t globalparams, globalparams_t globalparamsv);				
void MERGEP1_mergeVs2(uint512_dt * kvdram0,uint512_dt * kvdram1, uint512_dt * vdram, 
keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE],			globalparams_t globalparams, globalparams_t globalparamsv);				
void MERGEP1_mergeVs3(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2, uint512_dt * vdram, 
keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE],			globalparams_t globalparams, globalparams_t globalparamsv);				
void MERGEP1_mergeVs4(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3, uint512_dt * vdram, 
keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE],			globalparams_t globalparams, globalparams_t globalparamsv);				
void MERGEP1_mergeVs5(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4, uint512_dt * vdram, 
keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE],			globalparams_t globalparams, globalparams_t globalparamsv);				
void MERGEP1_mergeVs6(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5, uint512_dt * vdram, 
keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE],			globalparams_t globalparams, globalparams_t globalparamsv);				
void MERGEP1_mergeVs7(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6, uint512_dt * vdram, 
keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE],			globalparams_t globalparams, globalparams_t globalparamsv);				
void MERGEP1_mergeVs8(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7, uint512_dt * vdram, 
keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer7[BLOCKRAM_SIZE],			globalparams_t globalparams, globalparams_t globalparamsv);				
void MERGEP1_mergeVs9(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8, uint512_dt * vdram, 
keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer7[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer8[BLOCKRAM_SIZE],			globalparams_t globalparams, globalparams_t globalparamsv);				
void MERGEP1_mergeVs10(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9, uint512_dt * vdram, 
keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer7[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer8[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer9[BLOCKRAM_SIZE],			globalparams_t globalparams, globalparams_t globalparamsv);				
void MERGEP1_mergeVs11(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10, uint512_dt * vdram, 
keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer7[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer8[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer9[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer10[BLOCKRAM_SIZE],			globalparams_t globalparams, globalparams_t globalparamsv);				
void MERGEP1_mergeVs12(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11, uint512_dt * vdram, 
keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer7[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer8[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer9[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer10[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer11[BLOCKRAM_SIZE],			globalparams_t globalparams, globalparams_t globalparamsv);				

void MERGEP1_exchange(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC, uint512_dt * mdram);

void TradGPP1(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC, uint512_dt * mdram);

void TOPP1_topkernelS(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC, uint512_dt * mdram);







	
	#endif 
	#ifdef CONFIG_ENABLECLASS_ACTS
fetchmessage_t ACTSP1_fetchkeyvalues(bool_type enable, unsigned int mode,  uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], 
	batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams, unsigned int edgebankID);

void ACTSP1_commitkeyvalues(bool_type enable1, bool_type enable2, unsigned int mode, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],
	keyvalue_buffer_t destbuffer[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_t globalcapsule[MAX_NUM_PARTITIONS], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], 
		batch_type destbaseaddr_kvs, buffer_type chunk_size, sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams);

void ACTSP1_PRIOR_commitkeyvalues(bool_type enable1, bool_type enable2, unsigned int mode, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],
	keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_buffer_t destbuffer[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_t globalcapsule[MAX_NUM_PARTITIONS], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], 
			batch_type destbaseaddr_kvs, buffer_type chunk_size, sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams);
			
void ACTSP1_actspipeline(bool_type enable1, bool_type enable2, unsigned int mode, keyvalue_buffer_t buffer_setof1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_capsule_t capsule_so1[VECTOR_SIZE][MAX_NUM_PARTITIONS], 
					keyvalue_buffer_t buffer_setof8[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_capsule_t capsule_so8[MAX_NUM_PARTITIONS],
						unsigned int currentLOP, sweepparams_t sweepparams, buffer_type cutoffs[VECTOR_SIZE], batch_type shiftcount, globalparams_t globalparams);

void ACTSP1_actit(bool_type enable, unsigned int mode,
		 uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS],  				 
		globalparams_t globalparams, globalposition_t globalposition, sweepparams_t sweepparams, travstate_t ptravstate, batch_type sourcebaseaddr_kvs, batch_type destbaseaddr_kvs,
		bool_type resetenv, bool_type flush, unsigned int edgebankID);

void ACTSP1_priorit(bool_type enable, unsigned int mode,
		uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], 
		globalparams_t globalparams, globalposition_t globalposition, sweepparams_t sweepparams, travstate_t ptravstate, batch_type sourcebaseaddr_kvs, batch_type destbaseaddr_kvs,
		bool_type resetenv, bool_type flush, unsigned int edgebankID);
		
void ACTSP1_tradit(bool_type enable, unsigned int mode,
		 uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], 
		globalparams_t globalparams, sweepparams_t sweepparams, travstate_t ptravstate, batch_type sourcebaseaddr_kvs, batch_type destbaseaddr_kvs,
		bool_type resetenv, bool_type flush, unsigned int edgebankID);
						
						
							
	#endif 		
void TOPP1_NU_processit_splitdstvxs( uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], pmask_dt pmask_curr[BLOCKRAM_NEXTPMASK_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK,
		unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID);

void TOPP1_NU_partitionit( uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], globalparams_t globalparams, unsigned int edgebankID);
	
void TOPP1_NU_reduceit( uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparams, unsigned int edgebankID);					

void TOPP1_NU_dispatch(bool_type en_process, bool_type en_partition, bool_type en_reduce,  uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], pmask_dt pmask_curr[BLOCKRAM_NEXTPMASK_SIZE],
		batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparamsE, globalparams_t globalparamsK,
			unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID);








void TOPP1_U_processit( uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], pmask_dt pmask_curr[BLOCKRAM_NEXTPMASK_SIZE], keyvalue_t globalstatsbuffer[BLOCKRAM_GLOBALSTATS_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition,
		unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID);							

void TOPP1_U_dispatch(bool_type en_process, bool_type en_partition, bool_type en_reduce,  uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], pmask_dt pmask_curr[BLOCKRAM_NEXTPMASK_SIZE], keyvalue_t globalstatsbuffer[BLOCKRAM_GLOBALSTATS_SIZE],
		batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition,
			unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID);

void TOPP1_U_dispatch_reduce(uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], pmask_dt pmask_curr[BLOCKRAM_NEXTPMASK_SIZE], pmask_dt pmask_next[BLOCKRAM_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK, globalparams_t globalparamsV, globalposition_t globalposition,	
					unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID);
					
#ifdef CONFIG_ENABLECLASS_TOP_USRCV_UDSTV
void TOPP1_U_topkernelproc_embedded(unsigned int GraphIter, unsigned int globalid, unsigned int localid, unsigned int en_process, unsigned int en_partition, unsigned int en_reduce,  uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE], pmask_dt pmask_next[BLOCKRAM_NEXTPMASK_SIZE], keyvalue_t globalstatsbuffer[BLOCKRAM_GLOBALSTATS_SIZE], globalposition_t globalposition, globalparams_t globalparamsV);									
	
// top
void TOPP1_U_topkernelP1(
	uint512_dt * kvdram0,
	uint512_dt * vdram
	);
void TOPP1_U_topkernelP2(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * vdram
	);
void TOPP1_U_topkernelP3(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * vdram
	);
void TOPP1_U_topkernelP4(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * vdram
	);
void TOPP1_U_topkernelP5(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * kvdram4,
	uint512_dt * vdram
	);
void TOPP1_U_topkernelP6(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * kvdram4,
	uint512_dt * kvdram5,
	uint512_dt * vdram
	);
void TOPP1_U_topkernelP7(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * kvdram4,
	uint512_dt * kvdram5,
	uint512_dt * kvdram6,
	uint512_dt * vdram
	);
void TOPP1_U_topkernelP8(
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
void TOPP1_U_topkernelP9(
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
void TOPP1_U_topkernelP10(
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
void TOPP1_U_topkernelP11(
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
void TOPP1_U_topkernelP12(
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




