#ifndef ACTSMERGE_H
#define ACTSMERGE_H
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

class acts_merge {
public:
	acts_merge();
	~acts_merge();
	
	unsigned int MERGE_actvpstatsoffset(globalparams_t globalparams);
	value_t MERGE_applyfunc(value_t vtemp, value_t res, unsigned int GraphIter, unsigned int GraphAlgo);
	uint32_type MERGE_convertvmasktouint32(unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unsigned int index);

	void MERGE_readvdata(bool_type enable1, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], batch_type bufferoffset_kvs, buffer_type size_kvs);
	void MERGE_writevdata(bool_type enable1, uint512_dt * kvdram, batch_type basedramoffset_kvs, batch_type localdramoffset_kvs, keyvalue_vbuffer_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], batch_type bufferoffset_kvs, buffer_type size_kvs);			
	
	void MERGE_broadcastVs1(uint512_dt * kvdram0, uint512_dt * vdram);
	void MERGE_broadcastVs2(uint512_dt * kvdram0,uint512_dt * kvdram1, uint512_dt * vdram);
	void MERGE_broadcastVs3(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2, uint512_dt * vdram);
	void MERGE_broadcastVs4(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3, uint512_dt * vdram);
	void MERGE_broadcastVs5(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4, uint512_dt * vdram);
	void MERGE_broadcastVs6(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5, uint512_dt * vdram);
	void MERGE_broadcastVs7(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6, uint512_dt * vdram);
	void MERGE_broadcastVs8(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7, uint512_dt * vdram);
	void MERGE_broadcastVs9(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8, uint512_dt * vdram);
	void MERGE_broadcastVs10(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9, uint512_dt * vdram);
	void MERGE_broadcastVs11(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10, uint512_dt * vdram);
	void MERGE_broadcastVs12(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11, uint512_dt * vdram);
	
	void MERGE_mergeVs1(uint512_dt * kvdram0, uint512_dt * vdram);
	void MERGE_mergeVs2(uint512_dt * kvdram0,uint512_dt * kvdram1, uint512_dt * vdram);
	void MERGE_mergeVs3(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2, uint512_dt * vdram);
	void MERGE_mergeVs4(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3, uint512_dt * vdram);
	void MERGE_mergeVs5(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4, uint512_dt * vdram);
	void MERGE_mergeVs6(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5, uint512_dt * vdram);
	void MERGE_mergeVs7(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6, uint512_dt * vdram);
	void MERGE_mergeVs8(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7, uint512_dt * vdram);
	void MERGE_mergeVs9(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8, uint512_dt * vdram);
	void MERGE_mergeVs10(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9, uint512_dt * vdram);
	void MERGE_mergeVs11(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10, uint512_dt * vdram);
	void MERGE_mergeVs12(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11, uint512_dt * vdram);
	
	void MERGE_mergeVsAcrossSLRs(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC);

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






