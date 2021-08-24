#ifndef ACTS_H
#define ACTS_H
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

#define PROCESSMODULE
#define PARTITIONMODULE
#define REDUCEMODULE //
#define SYNCHRONIZEMODULE //

#define PROCESSMODE 0
#define PARTITIONMODE 1
#define REDUCEMODE 2

#define VBUFFER_VECTOR_SIZE NUM_PARTITIONS

// #define _DEBUGMODE_KERNELPRINTS_TRACE //

#define SHIFTSYNCHRONIZE

// #define AUTOMATEMODEON //
// #define VERTEXCOLORING // JUST-FOR-TEST

// #define SELECTIVEREAD // FIXME. work in progress...

class acts {
public:
	acts();
	~acts();
	
	// functions (basic)
	unsigned int amin(unsigned int val1, unsigned int val2);
	unsigned int aplus(unsigned int val1, unsigned int val2);
	
	// functions (allignment)
	batch_type allignlower_KV(batch_type val);
	batch_type allignhigher_KV(batch_type val);
	
	// functions (bit manipulation)
	unsigned int GETMASK_UINT(unsigned int index, unsigned int size);
	unsigned int READFROM_UINT(unsigned int data, unsigned int index, unsigned int size);
	void WRITETO_UINT(unsigned int * data, unsigned int index, unsigned int size, unsigned int value);
	
	// functions (converters)
	keyvalue_t GETKV(keyvalue_buffer_t data);
	keyvalue_buffer_t GETKV(keyvalue_t data);
	keyvalue_t GETKV2(keyvalue_vbuffer_t data);
	keyvalue_vbuffer_t GETKV2(keyvalue_t data);
	uint32_type convertvmasktouint32(unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unsigned int index);
	
	// functions (acts utilities)
	batch_type getskipsize(step_type currentLOP, bool_type sourceORdest, globalparams_t globalparams);
	batch_type getrangeforeachllop(globalparams_t globalparams);
	buffer_type getchunksize_kvs(buffer_type buffer_size, travstate_t travstate, buffer_type localoffset);
	buffer_type getpartitionwritesz(buffer_type realsize_kvs, buffer_type bramoffset_kvs);
	void calculateoffsets(keyvalue_capsule_t * buffer, buffer_type size);
	void calculatemanyunallignedoffsets(keyvalue_capsule_t buffer[VECTOR_SIZE][NUM_PARTITIONS], buffer_type size, batch_type base, batch_type skipspacing);
	batch_type get_num_source_partitions(step_type currentLOP);
	globalparams_t getglobalparams(uint512_dt * kvdram);
	sweepparams_t getsweepparams(globalparams_t globalparams, step_type currentLOP, batch_type source_partition);
	travstate_t gettravstate(bool_type enable, uint512_dt * kvdram, globalparams_t globalparams, step_type currentLOP, batch_type sourcestatsmarker);
	void savevmaskp(uint512_dt * kvdram, unsigned int offset_kvs, uint32_type vmask_p, globalparams_t globalparams);
	partition_type getpartition(bool_type enable, keyvalue_buffer_t keyvalue, step_type currentLOP, vertex_t upperlimit, unsigned int upperpartition, unsigned int batch_range_pow);

	// functions (resets)
	void resetvalues(keyvalue_t * buffer, buffer_type size, unsigned int resetval);
	void resetvalues(keyvalue_capsule_t * buffer, buffer_type size, unsigned int resetval);
	void resetvalues(value_t * buffer, buffer_type size, unsigned int resetval);
	void resetkeysandvalues(keyvalue_t * buffer, buffer_type size, unsigned int resetval);
	void resetvmask(unitBRAMwidth_type vmask[BLOCKRAM_SIZE]);
	void resetkvdramstats(uint512_dt * kvdram, globalparams_t globalparams);
	
	// functions (accessors)
	void readkeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_buffer_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs, travstate_t travstate, globalparams_t globalparams);
	
	void savekeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_buffer_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t * globalcapsule, keyvalue_capsule_t localcapsule[NUM_PARTITIONS], batch_type globalbaseaddress_kvs, globalparams_t globalparams);
	
	void readkeyvalues(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_buffer_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);

	void savekeyvalues(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_buffer_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	
	void readkeyvalues(bool_type enable, uint512_dt * kvdram, uint32_type bitsbuffer[BLOCKRAM_SIZE], keyvalue_t tempbuffer[VECTOR_SIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	
	void savekeyvalues(bool_type enable, uint512_dt * kvdram, uint32_type bitsbuffer[BLOCKRAM_SIZE], keyvalue_t tempbuffer[VECTOR_SIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);

	void readvdata(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);

	void savevdata(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	
	void readvdata(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type buffer1offset_kvs, keyvalue_vbuffer_t buffer2[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], batch_type buffer2offset_kvs, buffer_type size_kvs, globalparams_t globalparams);				
	
	void readvdata(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer1[VECTOR_SIZE][BLOCKRAM_SIZE], batch_type buffer1offset_kvs, keyvalue_vbuffer_t buffer2[VECTOR_SIZE][BLOCKRAM_SIZE], batch_type buffer2offset_kvs, buffer_type size_kvs, globalparams_t globalparams);

	void loadvmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);

	void savevmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);

	void loadvmask_p(uint512_dt * kvdram, uint32_type vmask_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
	
	void readglobalstats(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[NUM_PARTITIONS], batch_type offset_kvs, globalparams_t globalparams);

	void saveglobalstats(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[NUM_PARTITIONS], batch_type offset_kvs, globalparams_t globalparams);
	
	// functions (process)
	value_t processfunc(value_t udata, value_t edgew, unsigned int GraphAlgo);
	
	value_t getv(keyvalue_vbuffer_t vbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unsigned int loc, globalparams_t globalparams);

	value_t getvmask(unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unsigned int loc, globalparams_t globalparams);
	
	int readandprocess(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], 
			batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, sweepparams_t sweepparams, globalparams_t globalparams);

	// functions (partition)
	buffer_type preparekeyvalues(bool_type enable1, bool_type enable2, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_buffer_t destbuffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[VECTOR_SIZE][NUM_PARTITIONS], step_type currentLOP, sweepparams_t sweepparams, travstate_t travstate, buffer_type size_kvs, buffer_type cutoffs[VECTOR_SIZE], globalparams_t globalparams);				

	// functions (reduce)
	value_t reducefunc(value_t vtemp, value_t res, unsigned int GraphIter, unsigned int GraphAlgo);
	
	void reducevector(keyvalue_buffer_t kvdata, 
			keyvalue_vbuffer_t destbuffer[BLOCKRAM_SIZE], 
			buffer_type destoffset, unsigned int upperlimit, sweepparams_t sweepparams, globalparams_t globalparams);

	void reduceandbuffer(bool_type enable, keyvalue_buffer_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[NUM_PARTITIONS], keyvalue_vbuffer_t vbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], sweepparams_t sweepparams, globalparams_t globalparams);
	
	// synchronize
	value_t applyfunc(value_t vtemp, value_t res, unsigned int GraphIter, unsigned int GraphAlgo);
	
	value_t mergefunc(value_t value1, value_t value2, unsigned int GraphAlgo);
	
	void synchronize(bool_type enable, keyvalue_vbuffer_t head[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t buffer0[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer3[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer4[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer5[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer6[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer7[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer8[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer9[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer10[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer11[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer12[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer13[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer14[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer15[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer16[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer17[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer18[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer19[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer20[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer21[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer22[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer23[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer24[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer25[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer26[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer27[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer28[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer29[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer30[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer31[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t tail[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], globalparams_t globalparams);
	void synchronize2(bool_type enable, keyvalue_vbuffer_t head[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t buffer0[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer3[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer4[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer5[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer6[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer7[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer8[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer9[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer10[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer11[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer12[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer13[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer14[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer15[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer16[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer17[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer18[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer19[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer20[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer21[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer22[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer23[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer24[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer25[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer26[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer27[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer28[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer29[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer30[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer31[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t tail[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], globalparams_t globalparams);
	
	uint32_type apply(bool_type enable, 
			keyvalue_vbuffer_t vbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t synvbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE],
			batch_type voffset_kvs, globalparams_t globalparams);
	uint32_type apply2(bool_type enable, 
			keyvalue_vbuffer_t vbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t synvbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE],
			unsigned int colindex, batch_type voffset_kvs, globalparams_t globalparams);
		
	void spreadvdata(bool_type enable, keyvalue_vbuffer_t head[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t buffer0[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer3[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer4[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer5[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer6[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer7[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer8[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer9[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer10[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer11[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer12[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer13[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer14[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer15[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer16[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer17[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer18[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer19[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer20[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer21[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer22[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer23[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer24[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer25[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer26[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer27[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer28[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer29[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer30[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer31[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t tail[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], globalparams_t globalparams);
	void spreadvdata2(bool_type enable, keyvalue_vbuffer_t head[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t buffer0[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer3[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer4[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer5[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer6[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer7[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer8[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer9[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer10[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer11[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer12[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer13[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer14[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer15[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer16[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer17[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer18[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer19[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer20[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer21[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer22[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer23[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer24[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer25[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer26[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer27[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer28[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer29[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer30[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer31[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t tail[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], globalparams_t globalparams);
	
	void spreadvmask(bool_type enable, unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unitBRAMwidth_type vmask0[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5[BLOCKRAM_SIZE],unitBRAMwidth_type vmask6[BLOCKRAM_SIZE],unitBRAMwidth_type vmask7[BLOCKRAM_SIZE],unitBRAMwidth_type vmask8[BLOCKRAM_SIZE],unitBRAMwidth_type vmask9[BLOCKRAM_SIZE],unitBRAMwidth_type vmask10[BLOCKRAM_SIZE],unitBRAMwidth_type vmask11[BLOCKRAM_SIZE],unitBRAMwidth_type vmask12[BLOCKRAM_SIZE],unitBRAMwidth_type vmask13[BLOCKRAM_SIZE],unitBRAMwidth_type vmask14[BLOCKRAM_SIZE],unitBRAMwidth_type vmask15[BLOCKRAM_SIZE],unitBRAMwidth_type vmask16[BLOCKRAM_SIZE],unitBRAMwidth_type vmask17[BLOCKRAM_SIZE],unitBRAMwidth_type vmask18[BLOCKRAM_SIZE],unitBRAMwidth_type vmask19[BLOCKRAM_SIZE],unitBRAMwidth_type vmask20[BLOCKRAM_SIZE],unitBRAMwidth_type vmask21[BLOCKRAM_SIZE],unitBRAMwidth_type vmask22[BLOCKRAM_SIZE],unitBRAMwidth_type vmask23[BLOCKRAM_SIZE],unitBRAMwidth_type vmask24[BLOCKRAM_SIZE],unitBRAMwidth_type vmask25[BLOCKRAM_SIZE],unitBRAMwidth_type vmask26[BLOCKRAM_SIZE],unitBRAMwidth_type vmask27[BLOCKRAM_SIZE],unitBRAMwidth_type vmask28[BLOCKRAM_SIZE],unitBRAMwidth_type vmask29[BLOCKRAM_SIZE],unitBRAMwidth_type vmask30[BLOCKRAM_SIZE],unitBRAMwidth_type vmask31[BLOCKRAM_SIZE], globalparams_t globalparams);
	
	// acts
	void resetenvbuffers(keyvalue_capsule_t capsule_so1[VECTOR_SIZE][NUM_PARTITIONS], keyvalue_capsule_t capsule_so8[NUM_PARTITIONS]);

	int fetchkeyvalues(bool_type enable, unsigned int mode, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][BLOCKRAM_SIZE], 
			batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, sweepparams_t sweepparams, globalparams_t globalparams);
	
	void commitkeyvalues(bool_type enable1, bool_type enable2, unsigned int mode, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_t globalcapsule[NUM_PARTITIONS], keyvalue_capsule_t localcapsule[NUM_PARTITIONS], 
			batch_type destbaseaddr_kvs, sweepparams_t sweepparams, globalparams_t globalparams);
	
	void actspipeline(bool_type enable1, bool_type enable2, keyvalue_buffer_t buffer_setof1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_capsule_t capsule_so1[VECTOR_SIZE][NUM_PARTITIONS], 
						keyvalue_buffer_t buffer_setof8[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_capsule_t capsule_so8[NUM_PARTITIONS],
							unsigned int currentLOP, sweepparams_t sweepparams, buffer_type cutoff, buffer_type cutoffs[VECTOR_SIZE], batch_type shiftcount, globalparams_t globalparams);
							
	void actit(bool_type enable, unsigned int mode,
			uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], keyvalue_t globalstatsbuffer[NUM_PARTITIONS], 
			globalparams_t globalparams, sweepparams_t sweepparams, travstate_t ptravstate, batch_type sourcebaseaddr_kvs, batch_type destbaseaddr_kvs,
			bool_type resetenv, bool_type flush);

	// dispatches (sync 1)
	void processit(uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], globalparams_t globalparams);

	void partitionit(uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], globalparams_t globalparams);
		
	void reduceit(uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparams);

	void dispatch(bool_type en_process, bool_type en_partition, bool_type en_reduce, uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE],
			batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparams);
	
	#ifdef NACTS_IN_1COMPUTEUNIT
	void dispatch_reduce(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11,uint512_dt * kvdram12,uint512_dt * kvdram13,uint512_dt * kvdram14,uint512_dt * kvdram15,uint512_dt * kvdram16,uint512_dt * kvdram17,uint512_dt * kvdram18,uint512_dt * kvdram19,uint512_dt * kvdram20,uint512_dt * kvdram21,uint512_dt * kvdram22,uint512_dt * kvdram23,uint512_dt * kvdram24,uint512_dt * kvdram25,uint512_dt * kvdram26,uint512_dt * kvdram27,uint512_dt * kvdram28,uint512_dt * kvdram29,uint512_dt * kvdram30,uint512_dt * kvdram31, 
keyvalue_buffer_t sourcebuffer0[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer0_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], uint32_type vmask_p0[BLOCKRAM_SIZE],keyvalue_buffer_t sourcebuffer1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer1_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask1[BLOCKRAM_SIZE], uint32_type vmask_p1[BLOCKRAM_SIZE],keyvalue_buffer_t sourcebuffer2[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer2_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask2[BLOCKRAM_SIZE], uint32_type vmask_p2[BLOCKRAM_SIZE],keyvalue_buffer_t sourcebuffer3[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer3_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask3[BLOCKRAM_SIZE], uint32_type vmask_p3[BLOCKRAM_SIZE],keyvalue_buffer_t sourcebuffer4[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer4_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask4[BLOCKRAM_SIZE], uint32_type vmask_p4[BLOCKRAM_SIZE],keyvalue_buffer_t sourcebuffer5[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer5_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask5[BLOCKRAM_SIZE], uint32_type vmask_p5[BLOCKRAM_SIZE],keyvalue_buffer_t sourcebuffer6[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer6_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask6[BLOCKRAM_SIZE], uint32_type vmask_p6[BLOCKRAM_SIZE],keyvalue_buffer_t sourcebuffer7[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer7_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask7[BLOCKRAM_SIZE], uint32_type vmask_p7[BLOCKRAM_SIZE],keyvalue_buffer_t sourcebuffer8[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer8_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask8[BLOCKRAM_SIZE], uint32_type vmask_p8[BLOCKRAM_SIZE],keyvalue_buffer_t sourcebuffer9[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer9_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask9[BLOCKRAM_SIZE], uint32_type vmask_p9[BLOCKRAM_SIZE],keyvalue_buffer_t sourcebuffer10[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer10_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask10[BLOCKRAM_SIZE], uint32_type vmask_p10[BLOCKRAM_SIZE],keyvalue_buffer_t sourcebuffer11[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer11_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask11[BLOCKRAM_SIZE], uint32_type vmask_p11[BLOCKRAM_SIZE],keyvalue_buffer_t sourcebuffer12[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer12_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask12[BLOCKRAM_SIZE], uint32_type vmask_p12[BLOCKRAM_SIZE],keyvalue_buffer_t sourcebuffer13[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer13_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask13[BLOCKRAM_SIZE], uint32_type vmask_p13[BLOCKRAM_SIZE],keyvalue_buffer_t sourcebuffer14[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer14_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask14[BLOCKRAM_SIZE], uint32_type vmask_p14[BLOCKRAM_SIZE],keyvalue_buffer_t sourcebuffer15[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer15_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask15[BLOCKRAM_SIZE], uint32_type vmask_p15[BLOCKRAM_SIZE],keyvalue_buffer_t sourcebuffer16[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer16_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask16[BLOCKRAM_SIZE], uint32_type vmask_p16[BLOCKRAM_SIZE],keyvalue_buffer_t sourcebuffer17[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer17_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask17[BLOCKRAM_SIZE], uint32_type vmask_p17[BLOCKRAM_SIZE],keyvalue_buffer_t sourcebuffer18[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer18_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask18[BLOCKRAM_SIZE], uint32_type vmask_p18[BLOCKRAM_SIZE],keyvalue_buffer_t sourcebuffer19[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer19_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask19[BLOCKRAM_SIZE], uint32_type vmask_p19[BLOCKRAM_SIZE],keyvalue_buffer_t sourcebuffer20[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer20_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask20[BLOCKRAM_SIZE], uint32_type vmask_p20[BLOCKRAM_SIZE],keyvalue_buffer_t sourcebuffer21[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer21_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask21[BLOCKRAM_SIZE], uint32_type vmask_p21[BLOCKRAM_SIZE],keyvalue_buffer_t sourcebuffer22[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer22_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask22[BLOCKRAM_SIZE], uint32_type vmask_p22[BLOCKRAM_SIZE],keyvalue_buffer_t sourcebuffer23[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer23_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask23[BLOCKRAM_SIZE], uint32_type vmask_p23[BLOCKRAM_SIZE],keyvalue_buffer_t sourcebuffer24[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer24_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask24[BLOCKRAM_SIZE], uint32_type vmask_p24[BLOCKRAM_SIZE],keyvalue_buffer_t sourcebuffer25[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer25_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask25[BLOCKRAM_SIZE], uint32_type vmask_p25[BLOCKRAM_SIZE],keyvalue_buffer_t sourcebuffer26[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer26_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask26[BLOCKRAM_SIZE], uint32_type vmask_p26[BLOCKRAM_SIZE],keyvalue_buffer_t sourcebuffer27[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer27_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask27[BLOCKRAM_SIZE], uint32_type vmask_p27[BLOCKRAM_SIZE],keyvalue_buffer_t sourcebuffer28[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer28_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask28[BLOCKRAM_SIZE], uint32_type vmask_p28[BLOCKRAM_SIZE],keyvalue_buffer_t sourcebuffer29[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer29_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask29[BLOCKRAM_SIZE], uint32_type vmask_p29[BLOCKRAM_SIZE],keyvalue_buffer_t sourcebuffer30[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer30_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask30[BLOCKRAM_SIZE], uint32_type vmask_p30[BLOCKRAM_SIZE],keyvalue_buffer_t sourcebuffer31[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer31_1[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask31[BLOCKRAM_SIZE], uint32_type vmask_p31[BLOCKRAM_SIZE], 
			uint32_type vmask_p[BLOCKRAM_SIZE], globalparams_t globalparams[NUMCOMPUTEUNITS]);

	void topkernel(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11,uint512_dt * kvdram12,uint512_dt * kvdram13,uint512_dt * kvdram14,uint512_dt * kvdram15,uint512_dt * kvdram16,uint512_dt * kvdram17,uint512_dt * kvdram18,uint512_dt * kvdram19,uint512_dt * kvdram20,uint512_dt * kvdram21,uint512_dt * kvdram22,uint512_dt * kvdram23,uint512_dt * kvdram24,uint512_dt * kvdram25,uint512_dt * kvdram26,uint512_dt * kvdram27,uint512_dt * kvdram28,uint512_dt * kvdram29,uint512_dt * kvdram30,uint512_dt * kvdram31);
	#endif 
	
	#ifdef NACTS_IN_NCOMPUTEUNITS
	void dispatch_reduce(uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VBUFFER_VECTOR_SIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], globalparams_t globalparams);

	void topkernelsync(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11,uint512_dt * kvdram12,uint512_dt * kvdram13,uint512_dt * kvdram14,uint512_dt * kvdram15,uint512_dt * kvdram16,uint512_dt * kvdram17,uint512_dt * kvdram18,uint512_dt * kvdram19,uint512_dt * kvdram20,uint512_dt * kvdram21,uint512_dt * kvdram22,uint512_dt * kvdram23,uint512_dt * kvdram24,uint512_dt * kvdram25,uint512_dt * kvdram26,uint512_dt * kvdram27,uint512_dt * kvdram28,uint512_dt * kvdram29,uint512_dt * kvdram30,uint512_dt * kvdram31);
	
	void topkernel(uint512_dt * kvdram);
	
	void mainkernel(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11,uint512_dt * kvdram12,uint512_dt * kvdram13,uint512_dt * kvdram14,uint512_dt * kvdram15,uint512_dt * kvdram16,uint512_dt * kvdram17,uint512_dt * kvdram18,uint512_dt * kvdram19,uint512_dt * kvdram20,uint512_dt * kvdram21,uint512_dt * kvdram22,uint512_dt * kvdram23,uint512_dt * kvdram24,uint512_dt * kvdram25,uint512_dt * kvdram26,uint512_dt * kvdram27,uint512_dt * kvdram28,uint512_dt * kvdram29,uint512_dt * kvdram30,uint512_dt * kvdram31); // NB: for CPU test only
	#endif 

private:
	#ifndef FPGA_IMPL
	actsutility * actsutilityobj;
	#endif
};
#endif 






