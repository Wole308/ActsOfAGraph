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
	
void MEMCA_WRITEVDATATOBUFFER(unsigned int v, unsigned int index, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], value_t vdata);

void MEMCA_WRITEVDATASTOBUFFER(unsigned int index, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], value_t vdatas[VECTOR2_SIZE]);

void MEMCA_WRITEVDATASTOBUFFER_WITHDEPTHS(unsigned int indexes[VDATA_PACKINGSIZE], keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], value_t vdatas[VDATA_PACKINGSIZE]);

void MEMCA_WRITEVMASKSTOBUFFER(unsigned int index, unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmdatas[VMASK_PACKINGSIZE]);
 
void MEMCA_WRITEVMASKSTOBUFFER_WITHDEPTHS(unsigned int indexes[VDATA_PACKINGSIZE], unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmdatas[VMASK_PACKINGSIZE]);




	

private:
	#ifdef SW
	actsutility * actsutilityobj;
	acts_util * acts_utilobj;
	mydebug * mydebugobj;
	#endif
};
#endif 





