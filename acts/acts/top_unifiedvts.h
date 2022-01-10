#ifndef TOP_UNIFIEDVTXS_H
#define TOP_UNIFIEDVTXS_H
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
#include "processedges_unslicedgraph.h"
#include "processedges_slicedgraph.h"
#include "partitionupdates.h"
#include "reduceupdates.h"
#include "mem_access.h"
#include "acts_util.h"
#include "acts.h"
#include "merge_vtxs.h"
using namespace std;

class top_unifiedvts {
public:
	top_unifiedvts();
	~top_unifiedvts();
	
	void processit( uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unitBRAMwidth_type vmask_subp[BLOCKRAM_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition,
			unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID, unsigned int hybridmode);							

	void partitionit( uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unitBRAMwidth_type vmask_subp[BLOCKRAM_SIZE], keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], globalparams_t globalparams, globalposition_t globalposition, unsigned int edgebankID);
		
	void reduceit( uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unitBRAMwidth_type vmask_subp[BLOCKRAM_SIZE], keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], globalparams_t globalparams, globalposition_t globalposition, unsigned int edgebankID);					

	void dispatch(bool_type en_process, bool_type en_partition, bool_type en_reduce,  uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unitBRAMwidth_type vmask_subp[BLOCKRAM_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS],
			globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition,
				unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID, unsigned int hybridmode);
	
	void dispatch_reduce(bool_type en_reduce,  uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unitBRAMwidth_type vmask_subp[BLOCKRAM_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition,
			unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID, unsigned int hybridmode);

	void topkernelproc_embedded(unsigned int en_process, unsigned int en_partition, unsigned int en_reduce,  uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], unitBRAMwidth_type vmask_subp[BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], globalposition_t globalposition, unsigned int hybridmode);
		
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
	
private:
	#ifdef SW
	actsutility * actsutilityobj;
	acts_util * acts_utilobj;
	processedges_unslicedgraph * processedges_unslicedgraphobj;
	processedges_slicedgraph * processedges_slicedgraphobj;
	partitionupdates * partitionupdatesobj;
	reduceupdates * reduceupdatesobj;
	mem_access * mem_accessobj;
	acts * actsobj;
	merge_vtxs * mergeobj;
	#endif
};
#endif 




