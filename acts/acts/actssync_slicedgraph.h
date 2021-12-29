#ifndef ACTSSYNC_SLICEDGRAPH_H
#define ACTSSYNC_SLICEDGRAPH_H
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
#include "acts_merge.h"
#include "acts.h"
using namespace std;

#define NUMPIPELINES_SYNC 1 // 2 // CRITICAL FIXME. CORRECTME.
#if NUMPIPELINES_SYNC==1
#define SUP0
#endif 
#if NUMPIPELINES_SYNC==2
#define SUP0
#define SUP1
#endif

class actssync_slicedgraph {
public:
	actssync_slicedgraph();
	~actssync_slicedgraph();
	
	#ifdef SW  // CRITICAL REMOVEME.
	// functions (basic)
	unsigned int SYNC_amin(unsigned int val1, unsigned int val2);
	unsigned int SYNC_aplus(unsigned int val1, unsigned int val2);
	
	// functions (bit manipulation)
	unsigned int SYNC_GETMASK_UINT(unsigned int index, unsigned int size);
	unsigned int SYNC_READFROM_UINT(unsigned int data, unsigned int index, unsigned int size);
	void SYNC_WRITETO_UINT(unsigned int * data, unsigned int index, unsigned int size, unsigned int value);
	
	// functions (converters)
	keyvalue_t SYNC_GETKV(keyvalue_buffer_t data);
	keyvalue_buffer_t SYNC_GETKV(keyvalue_t data);
	keyy_t SYNC_GETK(uint32_type data);
	value_t SYNC_GETV(uint32_type data);
	keyvalue_t SYNC_GETKV2(keyvalue_vbuffer_t data);
	keyvalue_vbuffer_t SYNC_GETKV2(keyvalue_t data);
	keyy_t SYNC_GETK2(uint32_type data);
	value_t SYNC_GETV2(uint32_type data);
	uint32_type convertvmasktouint32(unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unsigned int index);
	keyy_t SYNC_GETKEYENTRY(uint512_dt data, unsigned int v);
	value_t GETVALUEENTRY(uint512_dt data, unsigned int v);
	
	// functions (actssync_slicedgraph utilities)
	batch_type SYNC_get_num_source_partitions(step_type currentLOP);
	globalparams_t SYNC_getglobalparams(uint512_dt * kvdram);
	travstate_t SYNC_gettravstate(bool_type enable, uint512_dt * kvdram, globalparams_t globalparams, step_type currentLOP, batch_type sourcestatsmarker);
	
	// functions (accessors)
	void SYNC_readvdata(bool_type enable1, bool_type enable2, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	
	// synchronize
	value_t SYNC_applyfunc(value_t vtemp, value_t res, unsigned int GraphIter, unsigned int GraphAlgo);
	
	value_t SYNC_mergefunc(value_t value1, value_t value2, unsigned int GraphAlgo);
	
	void SYNC_readandsynchronize3(bool_type enable1, bool_type enable2, unsigned int nk, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,			keyvalue_vbuffer_t res[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type dramoffset_kvs, globalparams_t globalparams);
	
	uint32_type SYNC_synchronizeandapply(bool_type enable1, bool_type enable2, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t res[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t refbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unsigned int colindex, batch_type voffset_kvs, globalparams_t globalparams);				

	void SYNC_spreadvdata(bool_type enable1, bool_type enable2, keyvalue_vbuffer_t source[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], globalparams_t globalparams);
	
	void SYNC_spreadvmask(bool_type enable1, bool_type enable2, bool_type enable3, unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], globalparams_t globalparams);

	void SYNC_spreadandwrite1(bool_type enable1, bool_type enable2, bool_type enable_savevmask, bool_type enable_savevmaskp, unsigned int nk, 
uint512_dt * kvdram0,				keyvalue_vbuffer_t source[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type dramoffset_kvs, buffer_type vbuffer_size_kvs, 
				unitBRAMwidth_type vmask[BLOCKRAM_SIZE], batch_type vmask_offset_kvs, buffer_type vmask_size_kvs, 
				unsigned int vmaskp_offset_kvs, uint32_type vmask_p_temp[2], unsigned int GraphIter,
				globalparams_t globalparams);
	void SYNC_spreadandwrite2(bool_type enable1, bool_type enable2, bool_type enable_savevmask, bool_type enable_savevmaskp, unsigned int nk, 
uint512_dt * kvdram0,uint512_dt * kvdram1,				keyvalue_vbuffer_t source[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type dramoffset_kvs, buffer_type vbuffer_size_kvs, 
				unitBRAMwidth_type vmask[BLOCKRAM_SIZE], batch_type vmask_offset_kvs, buffer_type vmask_size_kvs, 
				unsigned int vmaskp_offset_kvs, uint32_type vmask_p_temp[2], unsigned int GraphIter,
				globalparams_t globalparams);
	void SYNC_spreadandwrite3(bool_type enable1, bool_type enable2, bool_type enable_savevmask, bool_type enable_savevmaskp, unsigned int nk, 
uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,				keyvalue_vbuffer_t source[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type dramoffset_kvs, buffer_type vbuffer_size_kvs, 
				unitBRAMwidth_type vmask[BLOCKRAM_SIZE], batch_type vmask_offset_kvs, buffer_type vmask_size_kvs, 
				unsigned int vmaskp_offset_kvs, uint32_type vmask_p_temp[2], unsigned int GraphIter,
				globalparams_t globalparams);
	void SYNC_spreadandwrite4(bool_type enable1, bool_type enable2, bool_type enable_savevmask, bool_type enable_savevmaskp, unsigned int nk, 
uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,				keyvalue_vbuffer_t source[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type dramoffset_kvs, buffer_type vbuffer_size_kvs, 
				unitBRAMwidth_type vmask[BLOCKRAM_SIZE], batch_type vmask_offset_kvs, buffer_type vmask_size_kvs, 
				unsigned int vmaskp_offset_kvs, uint32_type vmask_p_temp[2], unsigned int GraphIter,
				globalparams_t globalparams);
			
	void SYNC_spreadandwrite1(bool_type enable1, bool_type enable2, bool_type enable_savevmask, bool_type enable_savevmaskp, unsigned int nk, uint512_dt * vdram, 
uint512_dt * kvdram0,				keyvalue_vbuffer_t source[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type vdramoffset_kvs, batch_type dramoffset_kvs, buffer_type vbuffer_size_kvs, 
				unitBRAMwidth_type vmask[BLOCKRAM_SIZE], batch_type vmask_offset_kvs, buffer_type vmask_size_kvs, 
				unsigned int vmaskp_offset_kvs, uint32_type vmask_p_temp[2], unsigned int GraphIter,
				globalparams_t globalparams);
	void SYNC_spreadandwrite2(bool_type enable1, bool_type enable2, bool_type enable_savevmask, bool_type enable_savevmaskp, unsigned int nk, uint512_dt * vdram, 
uint512_dt * kvdram0,uint512_dt * kvdram1,				keyvalue_vbuffer_t source[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type vdramoffset_kvs, batch_type dramoffset_kvs, buffer_type vbuffer_size_kvs, 
				unitBRAMwidth_type vmask[BLOCKRAM_SIZE], batch_type vmask_offset_kvs, buffer_type vmask_size_kvs, 
				unsigned int vmaskp_offset_kvs, uint32_type vmask_p_temp[2], unsigned int GraphIter,
				globalparams_t globalparams);
	void SYNC_spreadandwrite3(bool_type enable1, bool_type enable2, bool_type enable_savevmask, bool_type enable_savevmaskp, unsigned int nk, uint512_dt * vdram, 
uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,				keyvalue_vbuffer_t source[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type vdramoffset_kvs, batch_type dramoffset_kvs, buffer_type vbuffer_size_kvs, 
				unitBRAMwidth_type vmask[BLOCKRAM_SIZE], batch_type vmask_offset_kvs, buffer_type vmask_size_kvs, 
				unsigned int vmaskp_offset_kvs, uint32_type vmask_p_temp[2], unsigned int GraphIter,
				globalparams_t globalparams);
	void SYNC_spreadandwrite4(bool_type enable1, bool_type enable2, bool_type enable_savevmask, bool_type enable_savevmaskp, unsigned int nk, uint512_dt * vdram, 
uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,				keyvalue_vbuffer_t source[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type vdramoffset_kvs, batch_type dramoffset_kvs, buffer_type vbuffer_size_kvs, 
				unitBRAMwidth_type vmask[BLOCKRAM_SIZE], batch_type vmask_offset_kvs, buffer_type vmask_size_kvs, 
				unsigned int vmaskp_offset_kvs, uint32_type vmask_p_temp[2], unsigned int GraphIter,
				globalparams_t globalparams);
	#endif 

	void topkernelsync(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2, uint512_dt * vdram);

private:
	#ifdef SW
	actsutility * actsutilityobj;
	acts_util * acts_utilobj;
	processedges_unslicedgraph * processedges_unslicedgraphobj;
	processedges_slicedgraph * processedges_slicedgraphobj;
	partitionupdates * partitionupdatesobj;
	reduceupdates * reduceupdatesobj;
	mem_access * mem_accessobj;
	acts_merge * acts_mergeobj;
	acts * actsobj;
	#endif
};
#endif 






