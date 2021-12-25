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

class actssync {
public:
	actssync();
	~actssync();
	
	value_t MERGE_applyfunc(value_t vtemp, value_t res, unsigned int GraphIter, unsigned int GraphAlgo);
	
	void MERGE_readvdata(bool_type enable1, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], batch_type bufferoffset_kvs, buffer_type size_kvs);
	void MERGE_writevdata(bool_type enable1, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], batch_type bufferoffset_kvs, buffer_type size_kvs);
	
	void topkernelsync(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11,uint512_dt * kvdram12,uint512_dt * kvdram13,uint512_dt * kvdram14,uint512_dt * kvdram15,uint512_dt * kvdram16,uint512_dt * kvdram17,uint512_dt * kvdram18,uint512_dt * kvdram19,uint512_dt * kvdram20,uint512_dt * kvdram21,uint512_dt * kvdram22,uint512_dt * kvdram23,uint512_dt * kvdram24,uint512_dt * kvdram25,uint512_dt * kvdram26,uint512_dt * kvdram27,uint512_dt * kvdram28,uint512_dt * kvdram29,uint512_dt * kvdram30,uint512_dt * kvdram31, uint512_dt * vdram);

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






