#ifndef MEM_ACCESS_SPLITDSTVXS_H
#define MEM_ACCESS_SPLITDSTVXS_H
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
#include "../../acts/actsutility/actsutility.h"
#include "../../src/utility/utility.h"
#endif
#include "acts_util.h"
#include "mem_convert_and_access.h"
#include "mydebug.h"
using namespace std;

class mem_access_splitdstvxs {
public:
	mem_access_splitdstvxs(mydebug * _mydebugobj);
	~mem_access_splitdstvxs();
	
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




	
	
private:
	#ifdef SW
	actsutility * actsutilityobj;
	acts_util * acts_utilobj;
	mem_convert_and_access * mem_convert_and_accessobj;
	mydebug * mydebugobj;
	#endif
};
#endif 




