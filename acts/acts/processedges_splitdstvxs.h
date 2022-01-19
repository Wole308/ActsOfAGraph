#ifndef PROCESSEDGES_SPLITDSTVTXS_H
#define PROCESSEDGES_SPLITDSTVTXS_H
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

typedef struct {
	int s;
	int v;
	int incr;
} pgroup_t;

typedef struct {
	bool active;
	int beginoffset;
	int endoffset;
} sliceinfo_t;

class processedges_splitdstvxs {
public:
	processedges_splitdstvxs();
	~processedges_splitdstvxs();
	
	value_t PROCESS_processfunc(value_t udata, value_t edgew, unsigned int GraphAlgo);
	
	void PROCESS_debug(
		value_t E[VECTOR2_SIZE], bool_type ens[VECTOR2_SIZE], unsigned int mask[VECTOR2_SIZE],
			value_t ludatas[MAX_NUM_UNIQ_EDGES_PER_VEC], value_t vdataset[VECTOR2_SIZE], unit1_type datasVM[32], value_t masks[MAX_NUM_UNIQ_EDGES_PER_VEC], vertex_t lvids[VECTOR2_SIZE],
				unsigned int incr[VECTOR2_SIZE], unsigned int lsrcvids[VECTOR2_SIZE], unsigned int ldstvids[VECTOR2_SIZE], value_t res[VECTOR2_SIZE]);
	
	//
	value_t UTIL_GETVTXDATA(keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int loc, globalparams_t globalparams);
	
	value_t UTIL_GETVTXMASK(unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unsigned int loc, globalparams_t globalparams);
	
	fetchmessage_t PROCESS_readandprocess(bool_type enable, uint512_dt * edges, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], 
			batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, sweepparams_t sweepparams, globalparams_t globalparams);	
	
	//
	value_t UTIL_GETVTXDATA2(keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int loc, unsigned int dataNNNNN[VECTOR2_SIZE], globalparams_t globalparams);
	
	value_t UTIL_GETVTXMASK2(unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unsigned int loc, unit1_type dataN[VECTOR2_SIZE], globalparams_t globalparams);

	void PROCESS_getslice(sliceinfo_t slice, int id, int data[VECTOR2_SIZE], unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], int s_base, int row, globalparams_t globalparams);
	value_t UTIL_GETVTXMASKSET(unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unsigned int loc, unit1_type dataN[VECTOR2_SIZE], globalparams_t globalparams);

	pgroup_t getChosenPair(pgroup_t g0, pgroup_t g1, pgroup_t g2, int s_origin, int smin, int smax);

	fetchmessage_t PROCESS_readandprocess2(bool_type enable, uint512_dt * edges, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], 
			batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, sweepparams_t sweepparams, globalparams_t globalparams);	
			
private:
	#ifdef SW
	actsutility * actsutilityobj;
	acts_util * acts_utilobj;
	mem_access * mem_accessobj;
	#endif
};
#endif 





