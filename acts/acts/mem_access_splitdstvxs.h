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
#include "mem_access.h"
#include "mem_convert_and_access.h"
#include "mydebug.h"
using namespace std;

class mem_access_splitdstvxs {
public:
	mem_access_splitdstvxs(mydebug * _mydebugobj);
	~mem_access_splitdstvxs();
	
// -------------------- vdata -------------------- //
void MEMACCESS_SPL_GetXYLayoutV(unsigned int s, value_t vdata[VECTOR2_SIZE], value_t vdata2[VECTOR2_SIZE], unsigned int depths[VECTOR2_SIZE], unsigned int basedepth);

void MEMACCESS_SPL_readvdata(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);

void MEMACCESS_SPL_readvdata_slide(bool_type enable, unsigned int s, uint512_dt * kvdram, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);

void MEMACCESS_SPL_readvdatachunks(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalparams_t globalparams);

void MEMACCESS_SPL_savevdata(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);

// -------------------- vmasks -------------------- //
void MEMACCESS_SPL_GetXYLayoutVM(unsigned int s, unit1_type vmdata[VECTOR2_SIZE], unit1_type vmdata2[VECTOR2_SIZE], unsigned int depths[VECTOR2_SIZE], unsigned int basedepth);

void MEMACCESS_SPL_readvmasks_slide(bool_type enable, unsigned int s, uint512_dt * kvdram, unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type baseoffset_kvs, batch_type lbaseoffset_kvs, batch_type offset_kvs, buffer_type bufferoffset, buffer_type size_kvs);

void MEMACCESS_SPL_readvmaskschunks(bool_type enable, uint512_dt * kvdram, unit1_type vmaskBITS[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type basevoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalparams_t globalparams);

void MEMACCESS_SPL_savemasks(bool_type enable, uint512_dt * kvdram, unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs, unsigned int vmaskp_offset_kvs, globalparams_t globalparams);







	
	
private:
	#ifdef SW
	actsutility * actsutilityobj;
	acts_util * acts_utilobj;
	mem_access * mem_accessobj;
	mem_convert_and_access * mem_convert_and_accessobj;
	mydebug * mydebugobj;
	#endif
};
#endif 





