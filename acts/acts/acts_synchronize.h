#ifndef ACTS_SYNCHRONIZE_H
#define ACTS_SYNCHRONIZE_H
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

#define VBUFFER_VECTOR_SIZE NUM_PARTITIONS

#define INCORRECTAPPLY // CRITICAL REMOVEME. CAUSEOFERROR

class acts_synchronize {
public:
	acts_synchronize();
	~acts_synchronize();
	
	// functions (basic)
	unsigned int amin(unsigned int val1, unsigned int val2);
	unsigned int aplus(unsigned int val1, unsigned int val2);
	
	// functions (bit manipulation)
	unsigned int GETMASK_UINT(unsigned int index, unsigned int size);
	unsigned int READFROM_UINT(unsigned int data, unsigned int index, unsigned int size);
	void WRITETO_UINT(unsigned int * data, unsigned int index, unsigned int size, unsigned int value);
	
	// functions (converters)
	keyvalue_t GETKV(keyvalue_buffer_t data);
	keyvalue_buffer_t GETKV(keyvalue_t data);
	keyy_t GETK(uint32_type data);
	value_t GETV(uint32_type data);
	keyvalue_t GETKV2(keyvalue_vbuffer_t data);
	keyvalue_vbuffer_t GETKV2(keyvalue_t data);
	keyy_t GETK2(uint32_type data);
	value_t GETV2(uint32_type data);
	uint32_type convertvmasktouint32(uintNUMPby2_type vmask[BLOCKRAM_SIZE], unsigned int index);
	keyy_t GETKEYENTRY(uint512_dt data, unsigned int v);
	value_t GETVALUEENTRY(uint512_dt data, unsigned int v);
	
	// functions (acts_synchronize utilities)
	batch_type get_num_source_partitions(step_type currentLOP);
	globalparams_t getglobalparams(uint512_dt * kvdram);
	travstate_t gettravstate(bool_type enable, uint512_dt * kvdram, globalparams_t globalparams, step_type currentLOP, batch_type sourcestatsmarker);
	void savevmaskp(bool_type enable1, bool_type enable2, uint512_dt * kvdram, unsigned int offset_kvs, uint32_type vmask_p, globalparams_t globalparams);
	
	// functions (resets)
	void resetvmask(bool_type enable, uintNUMPby2_type vmask[BLOCKRAM_SIZE]);
	
	// functions (accessors)
	void readvdata(bool_type enable1, bool_type enable2, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);

	void savevdata(bool_type enable1, bool_type enable2, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	
	void readvdata(bool_type enable1, bool_type enable2, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type buffer1offset_kvs, keyvalue_vbuffer_t buffer2[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], batch_type buffer2offset_kvs, buffer_type size_kvs, globalparams_t globalparams);				
	
	void savevmasks(bool_type enable1, bool_type enable2, uint512_dt * kvdram, uintNUMPby2_type vmask[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);

	void saves(
		uint512_dt * kvdram, bool_type enable1, bool_type enable2, bool_type enable3, 
			batch_type vdata_offset_kvs, keyvalue_vbuffer_t buffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type vdata_bufferoffset_kvs, buffer_type vdata_size_kvs,
				uintNUMPby2_type vmask[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], batch_type vmask_offset_kvs, buffer_type vmask_size_kvs, 
					unsigned int vmaskp_offset_kvs, uint32_type vmask_p_temp[2],
						globalparams_t globalparams);
	
	// synchronize
	value_t applyfunc(value_t vtemp, value_t res, unsigned int GraphIter, unsigned int GraphAlgo);
	
	value_t mergefunc(value_t value1, value_t value2, unsigned int GraphAlgo);
	
	void readandsynchronize(bool_type enable1, bool_type enable2, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3, keyvalue_vbuffer_t res[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], batch_type dramoffset_kvs, globalparams_t globalparams);

	void readandsynchronize(bool_type enable1, bool_type enable2, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3, keyvalue_vbuffer_t res[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], batch_type dramoffset_kvs, keyvalue_vbuffer_t refbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], globalparams_t globalparams);

	uint32_type synchronizeandapply(bool_type enable1, bool_type enable2, keyvalue_vbuffer_t buffer0[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer3[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t res[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t refbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], uintNUMPby2_type vmask[BLOCKRAM_SIZE], unsigned int colindex, batch_type voffset_kvs, globalparams_t globalparams);

	void spreadvdata(bool_type enable1, bool_type enable2, keyvalue_vbuffer_t source[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t buffer0[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer3[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], globalparams_t globalparams);
	
	void spreadvmask(bool_type enable1, bool_type enable2, bool_type enable3, uintNUMPby2_type vmask[BLOCKRAM_SIZE], uintNUMPby2_type vmask0[BLOCKRAM_SIZE],uintNUMPby2_type vmask1[BLOCKRAM_SIZE],uintNUMPby2_type vmask2[BLOCKRAM_SIZE],uintNUMPby2_type vmask3[BLOCKRAM_SIZE], globalparams_t globalparams);

	void spreadandwrite(bool_type enable1, bool_type enable2, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,			keyvalue_vbuffer_t source[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], batch_type vbuffer_offset_kvs,  buffer_type vbuffer_size_kvs, 
			uintNUMPby2_type vmask[BLOCKRAM_SIZE], batch_type vmask_offset_kvs, buffer_type vmask_size_kvs, 
			unsigned int vmaskp_offset_kvs, uint32_type vmask_p_temp[2],
			globalparams_t globalparams);

	void topkernelsync(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11,uint512_dt * kvdram12,uint512_dt * kvdram13,uint512_dt * kvdram14,uint512_dt * kvdram15);

private:
	#ifndef FPGA_IMPL
	actsutility * actsutilityobj;
	#endif
};
#endif 






