#ifndef TOP_NONUNIFIEDVTXS_H
#define TOP_NONUNIFIEDVTXS_H
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
#include "../../src/utility/utility.h"
#include "../../acts/actsutility/actsutility.h"
#endif
#include "processedgesu.h"
#include "processedges_splitdstvxs.h"
#include "partitionupdates.h"
#include "reduceupdates.h"
#include "mem_access.h"
#include "acts_util.h"
#include "acts_merge.h"
#include "acts.h"
using namespace std;

class top_nusrcv_nudstv {
public:
	top_nusrcv_nudstv();
	~top_nusrcv_nudstv();
	
	void processit_unsliceddstvtxs( uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskREAD[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmaskWRITE[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK,
			unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID);	

	void processit_sliceddstvtxs( uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskREAD[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmaskWRITE[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK,
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
	
private:
	#ifdef SW
	actsutility * actsutilityobj;
	acts_util * acts_utilobj;
	processedgesu * processedgesobj;
	processedges_splitdstvxs * processedges_splitdstvxsobj;
	partitionupdates * partitionupdatesobj;
	reduceupdates * reduceupdatesobj;
	mem_access * mem_accessobj;
	// acts_merge * acts_mergeobj;
	acts_merge * acts_mergeobj;
	acts * actsobj;
	#endif
};
#endif 





