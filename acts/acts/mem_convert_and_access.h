#ifndef MEM_CONVERT_AND_ACCESS_H
#define MEM_CONVERT_AND_ACCESS_H
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
#include "../../acts/actsutility/actsutility.h"
#include "../../src/utility/utility.h"
#endif
#include "acts_util.h"
#include "mydebug.h"
using namespace std;

class mem_convert_and_access {
public:
	mem_convert_and_access(mydebug * _mydebugobj);
	~mem_convert_and_access();
	
// vdatas
void MEMCA_WRITETOBUFFER_VDATA(unsigned int v, unsigned int index, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], value_t vdata, batch_type bufferoffset_kvs);

void MEMCA_WRITETOBUFFER_VDATA(unsigned int index, keyvalue_vbuffer_t buffer[BLOCKRAM_SIZE], value_t vdata, batch_type bufferoffset_kvs);

void MEMCA_WRITETOBUFFER_VDATAS(unsigned int index, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], value_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);

void MEMCA_WRITETOBUFFERWITHDEPTHS_VDATAS(unsigned int indexes[VDATA_PACKINGSIZE], keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], value_t vdatas[VDATA_PACKINGSIZE], batch_type bufferoffset_kvs);			

value_t MEMCA_READFROMBUFFER_VDATA(unsigned int index, keyvalue_vbuffer_t buffer[BLOCKRAM_SIZE], batch_type bufferoffset_kvs);

void MEMCA_READFROMBUFFER_VDATAS(unsigned int index, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], value_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);

// void MEMCA_READFROMBUFFER_VDATAS(unsigned int index, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], value_t vdatas[32], batch_type regoffset, batch_type bufferoffset_kvs);

// vmasks
unit1_type MEMCA_READFROMBUFFER_VMASK(unsigned int index, unit1_type vmaskBITS[DOUBLE_BLOCKRAM_SIZE], batch_type bufferoffset_kvs);

void MEMCA_READFROMBUFFER_VMASKS(unsigned int index, unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmdatas[VMASK_PACKINGSIZE], batch_type bufferoffset_kvs);
 
void MEMCA_WRITETOBUFFER_VMASKS(unsigned int index, unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmdatas[VMASK_PACKINGSIZE], batch_type bufferoffset_kvs);
 
void MEMCA_WRITETOBUFFER_VMASKS_WITHDEPTHS(unsigned int indexes[VDATA_PACKINGSIZE], unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmdatas[VMASK_PACKINGSIZE], batch_type bufferoffset_kvs);

// vdatas & vmasks 
void MEMCA_READFROMBUFFER_VDATASWITHVMASKS(unsigned int index, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], value_t vdatas[VECTOR2_SIZE], unit1_type vmdatas[VMASK_PACKINGSIZE], batch_type bufferoffset_kvs);

void MEMCA_READFROMBUFFER_VDATASANDVMASKS(unsigned int index, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], value_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);

void MEMCA_WRITETOBUFFER_VDATAWITHVMASK(unsigned int index, keyvalue_vbuffer_t buffer[BLOCKRAM_SIZE], value_t vdata, unit1_type vmdata, batch_type bufferoffset_kvs);

void MEMCA_WRITETOBUFFER_VDATASANDVMASKS(unsigned int index, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], value_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);

void MEMCA_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS(unsigned int indexes[VDATA_PACKINGSIZE], keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], value_t vdatas[VDATA_PACKINGSIZE], batch_type bufferoffset_kvs);

void MEMCA_READFROMKVDRAM_VDATASANDVMASKS(unsigned int index, uint512_dt * kvdram, value_t vdatas[VECTOR2_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs);

void MEMCA_WRITETOKVDRAM_VDATASANDVMASKS(unsigned int index, uint512_dt * kvdram, value_t vdatas[VECTOR2_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs);

	

private:
	#ifdef SW
	actsutility * actsutilityobj;
	acts_util * acts_utilobj;
	mydebug * mydebugobj;
	#endif
};
#endif 





