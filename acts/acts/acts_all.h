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
#endif
#include "mydebug.h"
using namespace std;

class acts_all {
public:
	acts_all(mydebug * _mydebugobj);
	~acts_all();
	
	#ifdef CONFIG_ENABLECLASS_ACTS_UTIL
// functions (basic)
unsigned int UTIL_amin(unsigned int val1, unsigned int val2);
unsigned int UTIL_aplus(unsigned int val1, unsigned int val2);

// functions (allignment)
batch_type UTIL_allignlower_KV(batch_type val);
batch_type UTIL_allignhigher_KV(batch_type val);
batch_type UTIL_allignlower_KV2(batch_type val);
batch_type UTIL_allignhigher_KV2(batch_type val);

// functions (bit manipulation)
unsigned int UTIL_GETMASK_UINT(unsigned int index, unsigned int size);
unsigned int UTIL_READFROM_UINT(unsigned int data, unsigned int index, unsigned int size);
void UTIL_WRITETO_UINT(unsigned int * data, unsigned int index, unsigned int size, unsigned int value);
unsigned int UTIL_READBITSFROM_UINTV(uint32_type data, unsigned int index, unsigned int size);
unsigned int UTIL_READBITSFROM_UINT32(uint32_type data, unsigned int index, unsigned int size);
void UTIL_WRITEBITSTO_UINTV(uint32_type * data, unsigned int index, unsigned int size, unsigned int value);
void UTIL_WRITEBITSTO_UINT32(uint32_type * data, unsigned int index, unsigned int size, unsigned int value);

// functions (converters)
keyvalue_t UTIL_GETKV(keyvalue_buffer_t data);
keyvalue_buffer_t UTIL_GETKV(keyvalue_t data);
keyy_t UTIL_GETK(uint32_type data);
value_t UTIL_GETV(uint32_type data);
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
void UTIL_set_graphiteration(uint512_dt * kvdram, unsigned int GraphIter);
void UTIL_resetenvbuffers(keyvalue_capsule_t capsule_so1[VECTOR_SIZE][MAX_NUM_PARTITIONS], keyvalue_capsule_t capsule_so8[MAX_NUM_PARTITIONS]);
void UTIL_resetenvbuffer(keyvalue_capsule_t capsule_so8[MAX_NUM_PARTITIONS]);






	
	#endif 
	#ifdef CONFIG_ENABLECLASS_MEM_CONVERT_AND_ACCESS
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

	
	#endif
	#ifdef CONFIG_ENABLECLASS_MEM_ACCESS_SPLITDSTVXS
// -------------------- key values -------------------- //
fetchmessage_t MEMACCESS_SPL_readkeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs, travstate_t travstate, globalparams_t globalparams);
	
void MEMACCESS_SPL_savekeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_buffer_t buffer[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_t * globalcapsule, keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], batch_type globalbaseaddress_kvs, globalparams_t globalparams);

void MEMACCESS_SPL_readkeyvalues(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_buffer_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);

void MEMACCESS_SPL_savekeyvalues(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_buffer_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	
// -------------------- vdata -------------------- //
void MEMACCESS_SPL_GetXYLayoutV(unsigned int s, value_t vdata[VECTOR2_SIZE], value_t vdata2[VECTOR2_SIZE], unsigned int depths[VECTOR2_SIZE], unsigned int basedepth);

void MEMACCESS_SPL_RearrangeLayoutV(unsigned int s, value_t vdata[VECTOR2_SIZE], value_t vdata2[VECTOR2_SIZE]);

void MEMACCESS_SPL_readvdata(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);

void MEMACCESS_SPL_readvdata_slide(bool_type enable, unsigned int s, uint512_dt * kvdram, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);

void MEMACCESS_SPL_readvdatachunks(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalparams_t globalparams);

void MEMACCESS_SPL_savevdataandmasks(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);

// -------------------- vmasks -------------------- //
void MEMACCESS_SPL_GetXYLayoutVM(unsigned int s, unit1_type vmdata[VECTOR2_SIZE], unit1_type vmdata2[VECTOR2_SIZE], unsigned int depths[VECTOR2_SIZE], unsigned int basedepth);

void MEMACCESS_SPL_RearrangeLayoutVM(unsigned int s, unit1_type vmdata[VECTOR2_SIZE], unit1_type vmdata2[VECTOR2_SIZE]);

void MEMACCESS_SPL_readvmasks_slide(bool_type enable, unsigned int s, uint512_dt * kvdram, unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type baseoffset_kvs, batch_type lbaseoffset_kvs, batch_type offset_kvs, buffer_type bufferoffset, buffer_type size_kvs);

void MEMACCESS_SPL_readvmaskschunks(bool_type enable, uint512_dt * kvdram, unit1_type vmaskBITS[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type basevoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalparams_t globalparams);

void MEMACCESS_SPL_savemasks(bool_type enable, uint512_dt * kvdram, unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs, unsigned int vmaskp_offset_kvs, globalparams_t globalparams);

// -------------------- pmasks -------------------- //
void MEMACCESS_SPL_readpmask(uint512_dt * kvdram, uint32_type vmask_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);

// -------------------- stats -------------------- //
void MEMACCESS_SPL_readglobalstats(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], batch_type offset_kvs, globalparams_t globalparams);

void MEMACCESS_SPL_saveglobalstats(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], batch_type offset_kvs, globalparams_t globalparams);

tuple_t MEMACCESS_SPL_getvptrs( uint512_dt *  kvdram, unsigned int beginoffset, unsigned int endoffset, unsigned int edgebankID);

unsigned int MEMACCESS_SPL_getvptr(uint512_dt * kvdram, unsigned int baseoffset_kvs, unsigned int offset);

tuple_t MEMACCESS_SPL_getvptrs_opt( uint512_dt *  kvdram, unsigned int baseoffset_kvs, unsigned int beginoffset, unsigned int endoffset, unsigned int edgebankID);

void MEMACCESS_SPL_copystats(uint512_dt * edges, uint512_dt * kvdram, globalparams_t globalparamsE, globalparams_t globalparamsK);

void MEMACCESS_SPL_copyallstats( uint512_dt * kvdram, globalparams_t globalparamsE, globalparams_t globalparamsK, unsigned int edgebankID);

void MEMACCESS_SPL_commitkvstats(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams, unsigned int offset);

void MEMACCESS_SPL_commitkvstats2(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams, unsigned int offset, unsigned int size);

void MEMACCESS_SPL_retreievekvstats(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams, unsigned int offset, unsigned int size);

// -------------------- multiple accesses -------------------- //
void MEMACCESS_SPL_readmanyvmasks1(bool_type enable, uint512_dt * kvdram, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
void MEMACCESS_SPL_readmanyvmasks2(bool_type enable, uint512_dt * kvdram, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
void MEMACCESS_SPL_readmanyvmasks3(bool_type enable, uint512_dt * kvdram, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask2[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
void MEMACCESS_SPL_readmanyvmasks4(bool_type enable, uint512_dt * kvdram, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask2[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask3[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
void MEMACCESS_SPL_readmanyvmasks5(bool_type enable, uint512_dt * kvdram, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask2[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask3[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask4[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
void MEMACCESS_SPL_readmanyvmasks6(bool_type enable, uint512_dt * kvdram, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask2[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask3[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask4[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask5[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
void MEMACCESS_SPL_readmanyvmasks7(bool_type enable, uint512_dt * kvdram, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask2[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask3[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask4[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask5[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask6[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
void MEMACCESS_SPL_readmanyvmasks8(bool_type enable, uint512_dt * kvdram, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask2[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask3[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask4[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask5[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask6[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask7[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
void MEMACCESS_SPL_readmanyvmasks9(bool_type enable, uint512_dt * kvdram, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask2[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask3[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask4[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask5[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask6[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask7[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask8[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
void MEMACCESS_SPL_readmanyvmasks10(bool_type enable, uint512_dt * kvdram, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask2[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask3[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask4[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask5[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask6[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask7[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask8[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask9[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
void MEMACCESS_SPL_readmanyvmasks11(bool_type enable, uint512_dt * kvdram, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask2[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask3[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask4[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask5[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask6[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask7[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask8[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask9[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask10[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
void MEMACCESS_SPL_readmanyvmasks12(bool_type enable, uint512_dt * kvdram, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask2[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask3[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask4[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask5[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask6[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask7[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask8[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask9[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask10[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask11[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					

void MEMACCESS_SPL_readmanyspmask1(bool_type enable, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask0_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], buffer_type size_kvs);					
void MEMACCESS_SPL_readmanyspmask2(bool_type enable, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask0_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], buffer_type size_kvs);					
void MEMACCESS_SPL_readmanyspmask3(bool_type enable, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask0_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask2_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], buffer_type size_kvs);					
void MEMACCESS_SPL_readmanyspmask4(bool_type enable, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask0_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask2_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask3_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], buffer_type size_kvs);					
void MEMACCESS_SPL_readmanyspmask5(bool_type enable, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask0_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask2_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask3_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask4_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], buffer_type size_kvs);					
void MEMACCESS_SPL_readmanyspmask6(bool_type enable, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask0_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask2_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask3_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask4_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask5_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], buffer_type size_kvs);					
void MEMACCESS_SPL_readmanyspmask7(bool_type enable, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask0_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask2_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask3_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask4_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask5_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask6_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], buffer_type size_kvs);					
void MEMACCESS_SPL_readmanyspmask8(bool_type enable, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask0_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask2_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask3_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask4_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask5_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask6_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask7_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], buffer_type size_kvs);					
void MEMACCESS_SPL_readmanyspmask9(bool_type enable, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask0_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask2_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask3_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask4_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask5_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask6_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask7_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask8_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], buffer_type size_kvs);					
void MEMACCESS_SPL_readmanyspmask10(bool_type enable, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask0_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask2_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask3_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask4_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask5_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask6_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask7_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask8_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask9_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], buffer_type size_kvs);					
void MEMACCESS_SPL_readmanyspmask11(bool_type enable, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask0_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask2_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask3_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask4_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask5_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask6_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask7_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask8_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask9_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask10_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], buffer_type size_kvs);					
void MEMACCESS_SPL_readmanyspmask12(bool_type enable, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask0_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask2_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask3_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask4_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask5_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask6_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask7_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask8_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask9_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask10_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask11_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], buffer_type size_kvs);					

void MEMACCESS_SPL_readmanypmask1(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
void MEMACCESS_SPL_readmanypmask2(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
void MEMACCESS_SPL_readmanypmask3(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
void MEMACCESS_SPL_readmanypmask4(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
void MEMACCESS_SPL_readmanypmask5(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
void MEMACCESS_SPL_readmanypmask6(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
void MEMACCESS_SPL_readmanypmask7(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
void MEMACCESS_SPL_readmanypmask8(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE],uint32_type vmask7_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
void MEMACCESS_SPL_readmanypmask9(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE],uint32_type vmask7_p[BLOCKRAM_SIZE],uint32_type vmask8_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
void MEMACCESS_SPL_readmanypmask10(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE],uint32_type vmask7_p[BLOCKRAM_SIZE],uint32_type vmask8_p[BLOCKRAM_SIZE],uint32_type vmask9_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
void MEMACCESS_SPL_readmanypmask11(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE],uint32_type vmask7_p[BLOCKRAM_SIZE],uint32_type vmask8_p[BLOCKRAM_SIZE],uint32_type vmask9_p[BLOCKRAM_SIZE],uint32_type vmask10_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
void MEMACCESS_SPL_readmanypmask12(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE],uint32_type vmask7_p[BLOCKRAM_SIZE],uint32_type vmask8_p[BLOCKRAM_SIZE],uint32_type vmask9_p[BLOCKRAM_SIZE],uint32_type vmask10_p[BLOCKRAM_SIZE],uint32_type vmask11_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);




	
	#endif
	#ifdef CONFIG_ENABLECLASS_PROCESSEDGES_SPLITDSTVXS
value_t PROCESS_SPL_processfunc(value_t udata, value_t edgew, unsigned int GraphAlgo);

parsededge_t PARSE_EDGE(uint32_type data);

void PROCESS_SPL_debug(unsigned int debugid,
	unsigned int i, value_t E[VECTOR2_SIZE], bool_type ens[VECTOR2_SIZE], unsigned int mask[VECTOR2_SIZE],
		value_t udataset[MAX_NUM_UNIQ_EDGES_PER_VEC], value_t maskset[VECTOR2_SIZE], value_t Vset[MAX_NUM_UNIQ_EDGES_PER_VEC], unit1_type VMset[MAX_NUM_UNIQ_EDGES_PER_VEC], vertex_t lvids[VECTOR2_SIZE],
			unsigned int incr[VECTOR2_SIZE], unsigned int lsrcvids[VECTOR2_SIZE], unsigned int ldstvids[VECTOR2_SIZE], value_t res[VECTOR2_SIZE], keyvalue_t mykeyvalue[VECTOR2_SIZE], sweepparams_t sweepparams, globalparams_t globalparams,
				unsigned int lvid_head, unsigned int srcvid_head, travstate_t travstate, unsigned int chunk_size, sliceinfos_t sliceinfos, unsigned int * activeloadcount, unsigned int * inactiveloadcount, unsigned int * debug_numinvalidheads
				);
				
void PROCESS_SPL_getslice(sliceinfo_t slice, int id, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], value_t Vdatas[VECTOR2_SIZE], unit1_type VMdatas[VECTOR2_SIZE], unsigned int depths[16], globalparams_t globalparams);

//
value_t PROCESS_SPL_GETVTXMASK(unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unsigned int loc, globalparams_t globalparams);

value_t PROCESS_SPL_GETVTXMASK_SLIDED(unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unsigned int loc, globalparams_t globalparams);

sliceinfos_t PROCESS_SPL_GETVTXSLICES(unsigned int loc, globalparams_t globalparams);

sliceinfo_t PROCESS_SPL_GETVTXSLICE(unsigned int id, unsigned int loc, globalparams_t globalparams);

void PROCESS_SPL_GETVTXDATASET(sliceinfo_t slice, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], value_t Vdatas[VECTOR2_SIZE], unit1_type VMdatas[VECTOR2_SIZE], unsigned int depths[16], globalparams_t globalparams);

void PROCESS_SPL1_GETVTXDATASET(unsigned int loc, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], value_t Vdatas[MAX_NUM_UNIQ_EDGES_PER_VEC], unit1_type VMdatas[MAX_NUM_UNIQ_EDGES_PER_VEC], globalparams_t globalparams);

void PROCESS_SPL2_GETVTXDATASET(unsigned int loc, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], value_t Vdatas[MAX_NUM_UNIQ_EDGES_PER_VEC], unit1_type VMdatas[MAX_NUM_UNIQ_EDGES_PER_VEC], globalparams_t globalparams);

unsigned int PROCESS_SPL3_GETROW(unsigned int loc);
void PROCESS_SPL3_GETSET(keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type VMdatas_tmp[32], value_t Vdatas_tmp[32], unsigned int row, unsigned int offset, globalparams_t globalparams);
void PROCESS_SPL3_GETVTXDATASET(unsigned int loc, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], value_t Vdatas[MAX_NUM_UNIQ_EDGES_PER_VEC], unit1_type VMdatas[MAX_NUM_UNIQ_EDGES_PER_VEC], globalparams_t globalparams);

fetchmessage_t PROCESS_SPL_readandprocess(bool_type enable, uint512_dt * edges, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], 
		batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, sweepparams_t sweepparams, globalparams_t globalparams);	







	
	#endif 
	#ifdef CONFIG_ENABLECLASS_PARTITIONUPDATES
void PARTITION_preparekeyvalues(bool_type enable1, bool_type enable2, unsigned int mode, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_buffer_t destbuffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[VECTOR_SIZE][MAX_NUM_PARTITIONS], step_type currentLOP, sweepparams_t sweepparams, buffer_type size_kvs, buffer_type cutoffs[VECTOR_SIZE], globalparams_t globalparams);				

void PARTITION_priorpartitionkeyvalues(bool_type enable1, bool_type enable2, unsigned int mode, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_buffer_t destbuffer[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], step_type currentLOP, sweepparams_t sweepparams, buffer_type size_kvs, globalparams_t globalparams);
	






	
	#endif 
	#ifdef CONFIG_ENABLECLASS_REDUCEUPDATES
value_t REDUCE_reducefunc(value_t vtemp, value_t res, unsigned int GraphIter, unsigned int GraphAlgo);

void REDUCE_reducevector(unsigned int col, keyvalue_buffer_t kvdata, keyvalue_vbuffer_t destbuffer[BLOCKRAM_SIZE], unit1_type vmaskBITS[DOUBLE_BLOCKRAM_SIZE], buffer_type destoffset, unsigned int upperlimit, sweepparams_t sweepparams, globalparams_t globalparams);

void REDUCE_reduceandbuffer(bool_type enable, keyvalue_buffer_t buffer[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskBITS[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], sweepparams_t sweepparams, globalparams_t globalparams);				

void REDUCE_priorreduceandbuffer(bool_type enable, keyvalue_buffer_t buffer[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskBITS[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], buffer_type chunk_size, sweepparams_t sweepparams, globalparams_t globalparams);			

void REDUCE_tradreduceandbuffer(bool_type enable, uint512_dt * kvdram, keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], buffer_type chunk_size, keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], sweepparams_t sweepparams, globalparams_t globalparams);







	
	#endif
	#ifdef CONFIG_ENABLECLASS_ACTS_MERGE_SPLITDSTVXS
unsigned int MERGE_SPLIT_actvpstatsoffset(globalparams_t globalparams);

value_t MERGE_SPLIT_applyfunc(value_t vtemp, value_t res, unsigned int GraphIter, unsigned int GraphAlgo);

void MERGE_SPLIT_broadcastVs1(uint512_dt * kvdram0, uint512_dt * vdram);
void MERGE_SPLIT_broadcastVs2(uint512_dt * kvdram0,uint512_dt * kvdram1, uint512_dt * vdram);
void MERGE_SPLIT_broadcastVs3(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2, uint512_dt * vdram);
void MERGE_SPLIT_broadcastVs4(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3, uint512_dt * vdram);
void MERGE_SPLIT_broadcastVs5(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4, uint512_dt * vdram);
void MERGE_SPLIT_broadcastVs6(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5, uint512_dt * vdram);
void MERGE_SPLIT_broadcastVs7(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6, uint512_dt * vdram);
void MERGE_SPLIT_broadcastVs8(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7, uint512_dt * vdram);
void MERGE_SPLIT_broadcastVs9(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8, uint512_dt * vdram);
void MERGE_SPLIT_broadcastVs10(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9, uint512_dt * vdram);
void MERGE_SPLIT_broadcastVs11(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10, uint512_dt * vdram);
void MERGE_SPLIT_broadcastVs12(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11, uint512_dt * vdram);

void MERGE_SPLIT_BASICbroadcastVs1(uint512_dt * kvdram0, uint512_dt * vdram);
void MERGE_SPLIT_BASICbroadcastVs2(uint512_dt * kvdram0,uint512_dt * kvdram1, uint512_dt * vdram);
void MERGE_SPLIT_BASICbroadcastVs3(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2, uint512_dt * vdram);
void MERGE_SPLIT_BASICbroadcastVs4(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3, uint512_dt * vdram);
void MERGE_SPLIT_BASICbroadcastVs5(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4, uint512_dt * vdram);
void MERGE_SPLIT_BASICbroadcastVs6(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5, uint512_dt * vdram);
void MERGE_SPLIT_BASICbroadcastVs7(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6, uint512_dt * vdram);
void MERGE_SPLIT_BASICbroadcastVs8(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7, uint512_dt * vdram);
void MERGE_SPLIT_BASICbroadcastVs9(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8, uint512_dt * vdram);
void MERGE_SPLIT_BASICbroadcastVs10(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9, uint512_dt * vdram);
void MERGE_SPLIT_BASICbroadcastVs11(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10, uint512_dt * vdram);
void MERGE_SPLIT_BASICbroadcastVs12(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11, uint512_dt * vdram);

void MERGE_SPLIT_mergeVs1(uint512_dt * kvdram0, uint512_dt * vdram);
void MERGE_SPLIT_mergeVs2(uint512_dt * kvdram0,uint512_dt * kvdram1, uint512_dt * vdram);
void MERGE_SPLIT_mergeVs3(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2, uint512_dt * vdram);
void MERGE_SPLIT_mergeVs4(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3, uint512_dt * vdram);
void MERGE_SPLIT_mergeVs5(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4, uint512_dt * vdram);
void MERGE_SPLIT_mergeVs6(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5, uint512_dt * vdram);
void MERGE_SPLIT_mergeVs7(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6, uint512_dt * vdram);
void MERGE_SPLIT_mergeVs8(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7, uint512_dt * vdram);
void MERGE_SPLIT_mergeVs9(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8, uint512_dt * vdram);
void MERGE_SPLIT_mergeVs10(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9, uint512_dt * vdram);
void MERGE_SPLIT_mergeVs11(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10, uint512_dt * vdram);
void MERGE_SPLIT_mergeVs12(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11, uint512_dt * vdram);

void MERGE_SPLIT_exchangeVs(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC, uint512_dt * vdram);








	
	#endif 
	#ifdef CONFIG_ENABLECLASS_ACTS
fetchmessage_t ACTS_fetchkeyvalues(bool_type enable, unsigned int mode,  uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], 
	batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, sweepparams_t sweepparams, globalparams_t globalparams, unsigned int edgebankID);

void ACTS_commitkeyvalues(bool_type enable1, bool_type enable2, unsigned int mode, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskBITS[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],
	keyvalue_buffer_t destbuffer[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_t globalcapsule[MAX_NUM_PARTITIONS], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], 
		batch_type destbaseaddr_kvs, buffer_type chunk_size, sweepparams_t sweepparams, globalparams_t globalparams);

void ACTSprior_commitkeyvalues(bool_type enable1, bool_type enable2, unsigned int mode, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskBITS[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],
	keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_buffer_t destbuffer[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_t globalcapsule[MAX_NUM_PARTITIONS], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], 
			batch_type destbaseaddr_kvs, buffer_type chunk_size, sweepparams_t sweepparams, globalparams_t globalparams);
			
void ACTS_actspipeline(bool_type enable1, bool_type enable2, unsigned int mode, keyvalue_buffer_t buffer_setof1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_capsule_t capsule_so1[VECTOR_SIZE][MAX_NUM_PARTITIONS], 
					keyvalue_buffer_t buffer_setof8[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_capsule_t capsule_so8[MAX_NUM_PARTITIONS],
						unsigned int currentLOP, sweepparams_t sweepparams, buffer_type cutoffs[VECTOR_SIZE], batch_type shiftcount, globalparams_t globalparams);

void ACTS_actit(bool_type enable, unsigned int mode,
 uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskREAD[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmaskWRITE[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS],					 
		globalparams_t globalparams, sweepparams_t sweepparams, travstate_t ptravstate, batch_type sourcebaseaddr_kvs, batch_type destbaseaddr_kvs,
		bool_type resetenv, bool_type flush, unsigned int edgebankID);

void ACTS_priorit(bool_type enable, unsigned int mode,
 uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskREAD[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmaskWRITE[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], 
		globalparams_t globalparams, sweepparams_t sweepparams, travstate_t ptravstate, batch_type sourcebaseaddr_kvs, batch_type destbaseaddr_kvs,
		bool_type resetenv, bool_type flush, unsigned int edgebankID);
		
void ACTS_tradit(bool_type enable, unsigned int mode,
 uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskREAD[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmaskWRITE[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], 
		globalparams_t globalparams, sweepparams_t sweepparams, travstate_t ptravstate, batch_type sourcebaseaddr_kvs, batch_type destbaseaddr_kvs,
		bool_type resetenv, bool_type flush, unsigned int edgebankID);	
	#endif 
	#ifdef CONFIG_ENABLECLASS_TOP_USRCV_UDSTV
void processit( uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskREAD[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmaskWRITE[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition,
		unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID, unsigned int hybridmode);							

void partitionit( uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskREAD[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmaskWRITE[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], globalparams_t globalparams, globalposition_t globalposition, unsigned int edgebankID);
	
void reduceit( uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskREAD[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmaskWRITE[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], globalparams_t globalparams, globalposition_t globalposition, unsigned int edgebankID);					

void dispatch(bool_type en_process, bool_type en_partition, bool_type en_reduce,  uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskREAD[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmaskWRITE[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS],
		globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition,
			unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID, unsigned int hybridmode);

void dispatch_reduce(bool_type en_reduce,  uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskREAD[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmaskWRITE[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition,					
		unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID, unsigned int hybridmode);

void topkernelproc_embedded(unsigned int en_process, unsigned int en_partition, unsigned int en_reduce,  uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], unit1_type vmask_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmaskREAD[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmaskWRITE[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], globalposition_t globalposition, unsigned int hybridmode);
	
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
	#ifdef CONFIG_ENABLECLASS_TOPNUSRCV_NUDSTV
void processit_unsplitdstvxs( uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskREAD[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmaskWRITE[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK,
		unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID);	

void processit_splitdstvxs( uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskREAD[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmaskWRITE[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK,
		unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID);

void partitionit( uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskREAD[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmaskWRITE[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], globalparams_t globalparams, unsigned int edgebankID);
	
void reduceit( uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskREAD[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmaskWRITE[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparams, unsigned int edgebankID);					

void dispatch(bool_type en_process, bool_type en_partition, bool_type en_reduce,  uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskREAD[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmaskWRITE[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE],
		batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparamsE, globalparams_t globalparamsK,
			unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID);

void dispatch_reduce( uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskREAD[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmaskWRITE[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK,
		unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID);

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
	
private:
	#ifdef SW_ALLINONE 
	actsutility * actsutilityobj;
	mydebug * mydebugobj;
	#endif 
};
#endif 




