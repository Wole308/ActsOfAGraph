#ifndef PROCESSEDGESU_H
#define PROCESSEDGESU_H
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
#include "mem_access.h"
#include "acts_util.h"
using namespace std;

class processedgesu {
public:
	processedgesu();
	~processedgesu();
	
	value_t PROCESS_processfunc(value_t udata, value_t edgew, unsigned int GraphAlgo);
	
	value_t PROCESS_GETVTXDATA(keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int loc, globalparams_t globalparams);
	
	value_t PROCESS_GETVTXMASK(unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unsigned int loc, globalparams_t globalparams);
	
	fetchmessage_t PROCESS_readandprocess(bool_type enable, uint512_dt * edges, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], 
			batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, sweepparams_t sweepparams, globalparams_t globalparams);

private:
	#ifdef SW
	actsutility * actsutilityobj;
	acts_util * acts_utilobj;
	mem_access * mem_accessobj;
	#endif
};
#endif 





