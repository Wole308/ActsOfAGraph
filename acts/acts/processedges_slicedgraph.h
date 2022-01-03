#ifndef PROCESSEDGES_H
#define PROCESSEDGES_H
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
#include "mem_access.h"
#include "acts_util.h"
using namespace std;

class processedges_slicedgraph {
public:
	processedges_slicedgraph();
	~processedges_slicedgraph();
	
	value_t PROCESS_processfunc(value_t udata, value_t edgew, unsigned int GraphAlgo);
	
	fetchmessage_t PROCESS_readandprocess(bool_type enable, uint512_dt * edges, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unitBRAMwidth_type vmask_subp[BLOCKRAM_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], 
			batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, sweepparams_t sweepparams, globalparams_t globalparams);	
			
private:
	#ifdef SW
	actsutility * actsutilityobj;
	acts_util * acts_utilobj;
	mem_access * mem_accessobj;
	#endif
};
#endif 





