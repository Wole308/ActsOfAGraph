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
#include "processedgesu.h"
#include "processedges_splitdstvxs.h"
#include "partitionupdates.h"
#include "reduceupdates.h"
#include "mem_access.h"
#include "acts_util.h"
#include "acts.h"
using namespace std;

class acts_merge {
public:
	acts_merge();
	~acts_merge();
	
	// for usrcv udstv 
	unsigned int MERGE_amin(unsigned int val1, unsigned int val2);
	value_t MERGE_mergefunc(value_t value1, value_t value2, unsigned int GraphAlgo);
	
	void MERGE_readandreplicate1vdata(bool_type enable, uint512_dt * vdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	void MERGE_readandreplicate2vdata(bool_type enable, uint512_dt * vdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	void MERGE_readandreplicate3vdata(bool_type enable, uint512_dt * vdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	void MERGE_readandreplicate4vdata(bool_type enable, uint512_dt * vdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	void MERGE_readandreplicate5vdata(bool_type enable, uint512_dt * vdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	void MERGE_readandreplicate6vdata(bool_type enable, uint512_dt * vdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	void MERGE_readandreplicate7vdata(bool_type enable, uint512_dt * vdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	void MERGE_readandreplicate8vdata(bool_type enable, uint512_dt * vdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	void MERGE_readandreplicate9vdata(bool_type enable, uint512_dt * vdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	void MERGE_readandreplicate10vdata(bool_type enable, uint512_dt * vdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	void MERGE_readandreplicate11vdata(bool_type enable, uint512_dt * vdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer10[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	void MERGE_readandreplicate12vdata(bool_type enable, uint512_dt * vdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer10[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer11[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	
	void MERGE_merge1andsavevdata(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, unsigned int dramoffset_kvs); 
	void MERGE_merge2andsavevdata(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, unsigned int dramoffset_kvs); 
	void MERGE_merge3andsavevdata(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, unsigned int dramoffset_kvs); 
	void MERGE_merge4andsavevdata(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, unsigned int dramoffset_kvs); 
	void MERGE_merge5andsavevdata(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, unsigned int dramoffset_kvs); 
	void MERGE_merge6andsavevdata(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, unsigned int dramoffset_kvs); 
	void MERGE_merge7andsavevdata(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, unsigned int dramoffset_kvs); 
	void MERGE_merge8andsavevdata(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, unsigned int dramoffset_kvs); 
	void MERGE_merge9andsavevdata(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, unsigned int dramoffset_kvs); 
	void MERGE_merge10andsavevdata(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer9[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, unsigned int dramoffset_kvs); 
	void MERGE_merge11andsavevdata(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer9[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer10[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, unsigned int dramoffset_kvs); 
	void MERGE_merge12andsavevdata(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer9[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer10[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer11[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, unsigned int dramoffset_kvs); 

private:
	#ifdef SW
	actsutility * actsutilityobj;
	acts_util * acts_utilobj;
	processedgesu * processedgesobj;
	processedges_splitdstvxs * processedges_splitdstvxsobj;
	partitionupdates * partitionupdatesobj;
	reduceupdates * reduceupdatesobj;
	mem_access * mem_accessobj;
	acts * actsobj;
	#endif
};
#endif 






