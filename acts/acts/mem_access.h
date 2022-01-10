#ifndef MEM_ACCESS_H
#define MEM_ACCESS_H
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
using namespace std;

class mem_access {
public:
	mem_access();
	~mem_access();
	
	fetchmessage_t MEMACCESS_readkeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs, travstate_t travstate, globalparams_t globalparams);
	
	void MEMACCESS_savekeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_buffer_t buffer[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_t * globalcapsule, keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], batch_type globalbaseaddress_kvs, globalparams_t globalparams);
	
	void MEMACCESS_readkeyvalues(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_buffer_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);

	void MEMACCESS_savekeyvalues(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_buffer_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	
	void MEMACCESS_readvdata(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	void MEMACCESS_readvdata_splitdstvxs(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	void MEMACCESS_readvdatachunks_splitdstvxs(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type vbaseoffset_kvs, batch_type vreadoffset_kvs2, batch_type vreadsz_kvs2, globalparams_t globalparams);
	
	void MEMACCESS_savevdata(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	void MEMACCESS_savevdata_splitdstvxs(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	
	void MEMACCESS_readvmasks(bool_type enable, uint512_dt * kvdram, unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type baseoffset_kvs, batch_type lbaseoffset_kvs, batch_type offset_kvs, buffer_type bufferoffset_kvs, buffer_type size_kvs);				
	
	void MEMACCESS_savemasks(bool_type enable, uint512_dt * kvdram, unit1_type vmaskBITS[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs, unsigned int vmaskp_offset_kvs, globalparams_t globalparams);
	
	void MEMACCESS_readvmaskschunks_splitdstvxs(bool_type enable, uint512_dt * kvdram, unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type vmbaseoffset_kvs, batch_type vmreadoffset_kvs, batch_type vmreadsz_kvs, globalparams_t globalparams);				
	
	void MEMACCESS_readmanyvmasks1(bool_type enable, uint512_dt * kvdram, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
	void MEMACCESS_readmanyvmasks2(bool_type enable, uint512_dt * kvdram, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
	void MEMACCESS_readmanyvmasks3(bool_type enable, uint512_dt * kvdram, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask2[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
	void MEMACCESS_readmanyvmasks4(bool_type enable, uint512_dt * kvdram, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask2[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask3[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
	void MEMACCESS_readmanyvmasks5(bool_type enable, uint512_dt * kvdram, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask2[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask3[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask4[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
	void MEMACCESS_readmanyvmasks6(bool_type enable, uint512_dt * kvdram, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask2[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask3[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask4[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask5[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
	void MEMACCESS_readmanyvmasks7(bool_type enable, uint512_dt * kvdram, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask2[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask3[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask4[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask5[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask6[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
	void MEMACCESS_readmanyvmasks8(bool_type enable, uint512_dt * kvdram, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask2[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask3[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask4[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask5[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask6[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask7[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
	void MEMACCESS_readmanyvmasks9(bool_type enable, uint512_dt * kvdram, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask2[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask3[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask4[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask5[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask6[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask7[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask8[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
	void MEMACCESS_readmanyvmasks10(bool_type enable, uint512_dt * kvdram, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask2[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask3[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask4[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask5[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask6[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask7[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask8[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask9[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
	void MEMACCESS_readmanyvmasks11(bool_type enable, uint512_dt * kvdram, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask2[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask3[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask4[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask5[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask6[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask7[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask8[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask9[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask10[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
	void MEMACCESS_readmanyvmasks12(bool_type enable, uint512_dt * kvdram, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask2[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask3[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask4[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask5[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask6[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask7[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask8[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask9[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask10[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask11[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
	
	void MEMACCESS_readmanyspmask1(bool_type enable, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask0_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], buffer_type size_kvs);					
	void MEMACCESS_readmanyspmask2(bool_type enable, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask0_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], buffer_type size_kvs);					
	void MEMACCESS_readmanyspmask3(bool_type enable, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask0_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask2_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], buffer_type size_kvs);					
	void MEMACCESS_readmanyspmask4(bool_type enable, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask0_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask2_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask3_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], buffer_type size_kvs);					
	void MEMACCESS_readmanyspmask5(bool_type enable, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask0_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask2_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask3_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask4_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], buffer_type size_kvs);					
	void MEMACCESS_readmanyspmask6(bool_type enable, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask0_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask2_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask3_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask4_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask5_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], buffer_type size_kvs);					
	void MEMACCESS_readmanyspmask7(bool_type enable, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask0_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask2_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask3_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask4_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask5_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask6_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], buffer_type size_kvs);					
	void MEMACCESS_readmanyspmask8(bool_type enable, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask0_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask2_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask3_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask4_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask5_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask6_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask7_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], buffer_type size_kvs);					
	void MEMACCESS_readmanyspmask9(bool_type enable, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask0_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask2_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask3_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask4_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask5_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask6_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask7_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask8_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], buffer_type size_kvs);					
	void MEMACCESS_readmanyspmask10(bool_type enable, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask0_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask2_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask3_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask4_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask5_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask6_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask7_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask8_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask9_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], buffer_type size_kvs);					
	void MEMACCESS_readmanyspmask11(bool_type enable, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask0_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask2_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask3_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask4_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask5_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask6_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask7_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask8_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask9_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask10_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], buffer_type size_kvs);					
	void MEMACCESS_readmanyspmask12(bool_type enable, unit1_type vmask0[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask0_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask1_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask2_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask3_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask4_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask5_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask6_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask7_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask8_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask9_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask10_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE],unit1_type vmask11_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], buffer_type size_kvs);					
	
	void MEMACCESS_readpmask(uint512_dt * kvdram, uint32_type vmask_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
	
	void MEMACCESS_readmanypmask1(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
	void MEMACCESS_readmanypmask2(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
	void MEMACCESS_readmanypmask3(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
	void MEMACCESS_readmanypmask4(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
	void MEMACCESS_readmanypmask5(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
	void MEMACCESS_readmanypmask6(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
	void MEMACCESS_readmanypmask7(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
	void MEMACCESS_readmanypmask8(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE],uint32_type vmask7_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
	void MEMACCESS_readmanypmask9(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE],uint32_type vmask7_p[BLOCKRAM_SIZE],uint32_type vmask8_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
	void MEMACCESS_readmanypmask10(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE],uint32_type vmask7_p[BLOCKRAM_SIZE],uint32_type vmask8_p[BLOCKRAM_SIZE],uint32_type vmask9_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
	void MEMACCESS_readmanypmask11(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE],uint32_type vmask7_p[BLOCKRAM_SIZE],uint32_type vmask8_p[BLOCKRAM_SIZE],uint32_type vmask9_p[BLOCKRAM_SIZE],uint32_type vmask10_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
	void MEMACCESS_readmanypmask12(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE],uint32_type vmask7_p[BLOCKRAM_SIZE],uint32_type vmask8_p[BLOCKRAM_SIZE],uint32_type vmask9_p[BLOCKRAM_SIZE],uint32_type vmask10_p[BLOCKRAM_SIZE],uint32_type vmask11_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);

	void MEMACCESS_readglobalstats(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], batch_type offset_kvs, globalparams_t globalparams);

	void MEMACCESS_saveglobalstats(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], batch_type offset_kvs, globalparams_t globalparams);
	
	tuple_t MEMACCESS_getvptrs( uint512_dt *  kvdram, unsigned int beginoffset, unsigned int endoffset, unsigned int edgebankID);
	
	unsigned int MEMACCESS_getvptr(uint512_dt * kvdram, unsigned int baseoffset_kvs, unsigned int offset);
	
	tuple_t MEMACCESS_getvptrs_opt( uint512_dt *  kvdram, unsigned int baseoffset_kvs, unsigned int beginoffset, unsigned int endoffset, unsigned int edgebankID);
	
	void MEMACCESS_copyvs(uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], globalparams_t globalparamsK);
	
	void MEMACCESS_copystats(uint512_dt * edges, uint512_dt * kvdram, globalparams_t globalparamsE, globalparams_t globalparamsK);
	
	void MEMACCESS_copyallstats( uint512_dt * kvdram, globalparams_t globalparamsE, globalparams_t globalparamsK, unsigned int edgebankID);
	
	void MEMACCESS_commitkvstats(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams, unsigned int offset);
	
	void MEMACCESS_commitkvstats2(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams, unsigned int offset, unsigned int size);
	
	void MEMACCESS_retreievekvstats(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams, unsigned int offset, unsigned int size);

private:
	#ifdef SW
	actsutility * actsutilityobj;
	acts_util * acts_utilobj;
	#endif
};
#endif 





