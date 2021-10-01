#ifndef ACTSSYNC_H
#define ACTSSYNC_H
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
#include "../../include/config_params.h"
#include "../../include/common.h"
#include "../include/actscommon.h"
#ifndef FPGA_IMPL
#include "../../src/utility/utility.h"
#endif
#include "../../acts/actsutility/actsutility.h"
using namespace std;

#define NUMSYNCTHREADS 4

#define ACTSSYNC_AUTOMATE_ACROSSALGORITHMS

class actssync {
public:
	actssync();
	~actssync();
	
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
	
	// functions (actssync utilities)
	batch_type SYNC_get_num_source_partitions(step_type currentLOP);
	globalparams_t SYNC_getglobalparams(uint512_dt * kvdram);
	travstate_t SYNC_gettravstate(bool_type enable, uint512_dt * kvdram, globalparams_t globalparams, step_type currentLOP, batch_type sourcestatsmarker);
	
	// functions (accessors)
	void readvdata(bool_type enable1, bool_type enable2, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);

	void saves(
		uint512_dt * kvdram, bool_type enable1, bool_type enable2, bool_type enable3, 
			batch_type vdata_offset_kvs, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type vdata_bufferoffset_kvs, buffer_type vdata_size_kvs,
				unitBRAMwidth_type vmask[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type vmask_offset_kvs, buffer_type vmask_size_kvs, 
					unsigned int vmaskp_offset_kvs, uint32_type vmask_p_temp[2],
						globalparams_t globalparams);
	
	// synchronize
	value_t applyfunc(value_t vtemp, value_t res, unsigned int GraphIter, unsigned int GraphAlgo);
	
	value_t mergefunc(value_t value1, value_t value2, unsigned int GraphAlgo);
	
	void readandsynchronize(bool_type enable1, bool_type enable2, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3, keyvalue_vbuffer_t res[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type dramoffset_kvs, globalparams_t globalparams);

	uint32_type synchronizeandapply(bool_type enable1, bool_type enable2, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t res[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t refbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unsigned int colindex, batch_type voffset_kvs, globalparams_t globalparams);				

	void spreadvdata(bool_type enable1, bool_type enable2, keyvalue_vbuffer_t source[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], globalparams_t globalparams);
	
	void spreadvmask(bool_type enable1, bool_type enable2, bool_type enable3, unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], globalparams_t globalparams);

	void spreadandwrite(bool_type enable1, bool_type enable2, bool_type enable_savevmask, bool_type enable_savevmaskp, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,			keyvalue_vbuffer_t source[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type vbuffer_offset_kvs, buffer_type vbuffer_size_kvs, 
			unitBRAMwidth_type vmask[BLOCKRAM_SIZE], batch_type vmask_offset_kvs, buffer_type vmask_size_kvs, 
			unsigned int vmaskp_offset_kvs, uint32_type vmask_p_temp[2],
			globalparams_t globalparams);
			
	void spreadandwrite(bool_type enable1, bool_type enable2, bool_type enable_savevmask, bool_type enable_savevmaskp, uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,			keyvalue_vbuffer_t source[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type vbuffer_offset_kvs, buffer_type vbuffer_size_kvs, 
			unitBRAMwidth_type vmask[BLOCKRAM_SIZE], batch_type vmask_offset_kvs, buffer_type vmask_size_kvs, 
			unsigned int vmaskp_offset_kvs, uint32_type vmask_p_temp[2],
			globalparams_t globalparams);
	#endif 

	void topkernelsync(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3, uint512_dt * vdram);

private:
	#ifndef FPGA_IMPL
	actsutility * actsutilityobj;
	#endif
};
#endif 






