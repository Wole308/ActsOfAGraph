// #ifndef ACTS_H
// #define ACTS_H
#include "../../include/config_params.h"
#include "../../include/common.h"
#include "../include/actscommon.h"
#include "actsproc.h"

// class acts {
// public:
	// acts();
	// ~acts();

	// functions (basic)
	unsigned int ACTSUTIL_amin(unsigned int val1, unsigned int val2);
	unsigned int ACTSUTIL_aplus(unsigned int val1, unsigned int val2);

	// functions (allignment)
	batch_type ACTSUTIL_allignlower_KV(batch_type val);
	batch_type ACTSUTIL_allignhigher_KV(batch_type val);

	// functions (bit manipulation)
	unsigned int ACTSUTIL_GETMASK_UINT(unsigned int index, unsigned int size);
	unsigned int ACTSUTIL_READFROM_UINT(unsigned int data, unsigned int index, unsigned int size);
	void ACTSUTIL_WRITETO_UINT(unsigned int * data, unsigned int index, unsigned int size, unsigned int value);

	// functions (converters)
	keyvalue_t ACTSUTIL_GETKV(keyvalue_buffer_t data);
	keyvalue_buffer_t ACTSUTIL_GETKV(keyvalue_t data);
	keyy_t ACTSUTIL_GETK(uint32_type data);
	value_t ACTSUTIL_GETV(uint32_type data);
	keyvalue_t ACTSUTIL_GETKV2(keyvalue_vbuffer_t data);
	keyvalue_vbuffer_t ACTSUTIL_GETKV2(keyvalue_t data);
	keyy_t ACTSUTIL_GETK2(uint32_type data);
	value_t ACTSUTIL_GETV2(uint32_type data);
	keyy_t ACTSUTIL_GETKEYENTRY(uint512_dt data, unsigned int v);
	value_t ACTSUTIL_GETVTXDATA(keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int loc, globalparams_t globalparams);
	value_t ACTSUTIL_GETVTXMASK(unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unsigned int loc, globalparams_t globalparams);
	value_t ACTSUTIL_GETVTXMASK_SUBP(unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unsigned int loc, globalparams_t globalparams);
	keyvalue1_type ACTSUTIL_GETVTXMASK_SUBPP(unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unsigned int loc, globalparams_t globalparams);

	// functions (acts utilities)
	batch_type ACTSUTIL_getskipsize(step_type currentLOP, bool_type sourceORdest, globalparams_t globalparams);
	batch_type ACTSUTIL_getrangeforeachllop(globalparams_t globalparams);
	buffer_type ACTSUTIL_getchunksize_kvs(buffer_type buffer_size, travstate_t travstate, buffer_type localoffset);
	buffer_type ACTSUTIL_getpartitionwritesz(buffer_type realsize_kvs, buffer_type bramoffset_kvs);
	void ACTSUTIL_calculateoffsets(keyvalue_capsule_t * buffer, buffer_type size);
	void ACTSUTIL_calculatemanyunallignedoffsets(keyvalue_capsule_t buffer[VECTOR_SIZE][MAX_NUM_PARTITIONS], buffer_type size, batch_type base, batch_type skipspacing);
	batch_type ACTSUTIL_get_num_source_partitions(step_type currentLOP);
	globalparams_t ACTSUTIL_getglobalparams(uint512_dt * kvdram);
	sweepparams_t ACTSUTIL_getsweepparams(globalparams_t globalparams, step_type currentLOP, batch_type source_partition);
	travstate_t ACTSUTIL_gettravstate(bool_type enable, uint512_dt * kvdram, globalparams_t globalparams, step_type currentLOP, batch_type sourcestatsmarker);
	void ACTSUTIL_settravstate(bool_type enable, uint512_dt * kvdram, globalparams_t globalparams, batch_type offset, unsigned int value);
	partition_type ACTSUTIL_getpartition(bool_type enable, keyvalue_buffer_t keyvalue, step_type currentLOP, vertex_t upperlimit, unsigned int upperpartition, unsigned int batch_range_pow);

	// functions (resets)
	void ACTSUTIL_resetvalues(keyvalue_t * buffer, buffer_type size, unsigned int resetval);
	void ACTSUTIL_resetvalues(keyvalue_capsule_t * buffer, buffer_type size, unsigned int resetval);
	void ACTSUTIL_resetvalues(value_t * buffer, buffer_type size, unsigned int resetval);
	void ACTSUTIL_resetkeysandvalues(keyvalue_t * buffer, buffer_type size, unsigned int resetval);
	void ACTSUTIL_resetkvstatvalues(uint512_dt * kvdram, globalparams_t globalparams);
	void ACTSUTIL_accumkvstats(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams);
	void ACTSUTIL_commitkvstats(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams, unsigned int offset);
	void ACTSUTIL_increment_graphiteration(uint512_dt * kvdram, globalparams_t globalparams);

	// functions (accessors)
	fetchmessage_t ACTSUTIL_readkeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs, travstate_t travstate, globalparams_t globalparams);

	void ACTSUTIL_savekeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_buffer_t buffer[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_t * globalcapsule, keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], batch_type globalbaseaddress_kvs, globalparams_t globalparams);

	void ACTSUTIL_readkeyvalues(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_buffer_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);

	void ACTSUTIL_savekeyvalues(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_buffer_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);

	void ACTSUTIL_readvdata(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);

	void ACTSUTIL_savevdata(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);

	void ACTSUTIL_loadvmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);

	void ACTSUTIL_loadmany1vmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
	void ACTSUTIL_loadmany2vmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
	void ACTSUTIL_loadmany3vmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
	void ACTSUTIL_loadmany4vmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
	void ACTSUTIL_loadmany5vmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
	void ACTSUTIL_loadmany6vmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
	void ACTSUTIL_loadmany7vmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5[BLOCKRAM_SIZE],unitBRAMwidth_type vmask6[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
	void ACTSUTIL_loadmany8vmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5[BLOCKRAM_SIZE],unitBRAMwidth_type vmask6[BLOCKRAM_SIZE],unitBRAMwidth_type vmask7[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
	void ACTSUTIL_loadmany9vmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5[BLOCKRAM_SIZE],unitBRAMwidth_type vmask6[BLOCKRAM_SIZE],unitBRAMwidth_type vmask7[BLOCKRAM_SIZE],unitBRAMwidth_type vmask8[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
	void ACTSUTIL_loadmany10vmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5[BLOCKRAM_SIZE],unitBRAMwidth_type vmask6[BLOCKRAM_SIZE],unitBRAMwidth_type vmask7[BLOCKRAM_SIZE],unitBRAMwidth_type vmask8[BLOCKRAM_SIZE],unitBRAMwidth_type vmask9[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
	void ACTSUTIL_loadmany11vmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5[BLOCKRAM_SIZE],unitBRAMwidth_type vmask6[BLOCKRAM_SIZE],unitBRAMwidth_type vmask7[BLOCKRAM_SIZE],unitBRAMwidth_type vmask8[BLOCKRAM_SIZE],unitBRAMwidth_type vmask9[BLOCKRAM_SIZE],unitBRAMwidth_type vmask10[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
	void ACTSUTIL_loadmany12vmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5[BLOCKRAM_SIZE],unitBRAMwidth_type vmask6[BLOCKRAM_SIZE],unitBRAMwidth_type vmask7[BLOCKRAM_SIZE],unitBRAMwidth_type vmask8[BLOCKRAM_SIZE],unitBRAMwidth_type vmask9[BLOCKRAM_SIZE],unitBRAMwidth_type vmask10[BLOCKRAM_SIZE],unitBRAMwidth_type vmask11[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					

	void ACTSUTIL_loadmany1vmasks_subp(bool_type enable, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], unitBRAMwidth_type vmask0_subp[BLOCKRAM_SIZE], buffer_type size_kvs);					
	void ACTSUTIL_loadmany2vmasks_subp(bool_type enable, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], unitBRAMwidth_type vmask0_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1_subp[BLOCKRAM_SIZE], buffer_type size_kvs);					
	void ACTSUTIL_loadmany3vmasks_subp(bool_type enable, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], unitBRAMwidth_type vmask0_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2_subp[BLOCKRAM_SIZE], buffer_type size_kvs);					
	void ACTSUTIL_loadmany4vmasks_subp(bool_type enable, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], unitBRAMwidth_type vmask0_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3_subp[BLOCKRAM_SIZE], buffer_type size_kvs);					
	void ACTSUTIL_loadmany5vmasks_subp(bool_type enable, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], unitBRAMwidth_type vmask0_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4_subp[BLOCKRAM_SIZE], buffer_type size_kvs);					
	void ACTSUTIL_loadmany6vmasks_subp(bool_type enable, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], unitBRAMwidth_type vmask0_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5_subp[BLOCKRAM_SIZE], buffer_type size_kvs);					
	void ACTSUTIL_loadmany7vmasks_subp(bool_type enable, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], unitBRAMwidth_type vmask0_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask6_subp[BLOCKRAM_SIZE], buffer_type size_kvs);					
	void ACTSUTIL_loadmany8vmasks_subp(bool_type enable, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], unitBRAMwidth_type vmask0_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask6_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask7_subp[BLOCKRAM_SIZE], buffer_type size_kvs);					
	void ACTSUTIL_loadmany9vmasks_subp(bool_type enable, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], unitBRAMwidth_type vmask0_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask6_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask7_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask8_subp[BLOCKRAM_SIZE], buffer_type size_kvs);					
	void ACTSUTIL_loadmany10vmasks_subp(bool_type enable, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], unitBRAMwidth_type vmask0_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask6_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask7_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask8_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask9_subp[BLOCKRAM_SIZE], buffer_type size_kvs);					
	void ACTSUTIL_loadmany11vmasks_subp(bool_type enable, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], unitBRAMwidth_type vmask0_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask6_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask7_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask8_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask9_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask10_subp[BLOCKRAM_SIZE], buffer_type size_kvs);					
	void ACTSUTIL_loadmany12vmasks_subp(bool_type enable, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], unitBRAMwidth_type vmask0_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask6_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask7_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask8_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask9_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask10_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask11_subp[BLOCKRAM_SIZE], buffer_type size_kvs);					

	void ACTSUTIL_loadvmask_p(uint512_dt * kvdram, uint32_type vmask_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
	void ACTSUTIL_loadmany1vmask_p(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
	void ACTSUTIL_loadmany2vmask_p(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
	void ACTSUTIL_loadmany3vmask_p(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
	void ACTSUTIL_loadmany4vmask_p(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
	void ACTSUTIL_loadmany5vmask_p(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
	void ACTSUTIL_loadmany6vmask_p(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
	void ACTSUTIL_loadmany7vmask_p(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
	void ACTSUTIL_loadmany8vmask_p(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE],uint32_type vmask7_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
	void ACTSUTIL_loadmany9vmask_p(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE],uint32_type vmask7_p[BLOCKRAM_SIZE],uint32_type vmask8_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
	void ACTSUTIL_loadmany10vmask_p(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE],uint32_type vmask7_p[BLOCKRAM_SIZE],uint32_type vmask8_p[BLOCKRAM_SIZE],uint32_type vmask9_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
	void ACTSUTIL_loadmany11vmask_p(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE],uint32_type vmask7_p[BLOCKRAM_SIZE],uint32_type vmask8_p[BLOCKRAM_SIZE],uint32_type vmask9_p[BLOCKRAM_SIZE],uint32_type vmask10_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
	void ACTSUTIL_loadmany12vmask_p(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE],uint32_type vmask7_p[BLOCKRAM_SIZE],uint32_type vmask8_p[BLOCKRAM_SIZE],uint32_type vmask9_p[BLOCKRAM_SIZE],uint32_type vmask10_p[BLOCKRAM_SIZE],uint32_type vmask11_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);

	void ACTSUTIL_readglobalstats(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], batch_type offset_kvs, globalparams_t globalparams);

	void ACTSUTIL_saveglobalstats(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], batch_type offset_kvs, globalparams_t globalparams);

	tuple_t ACTSUTIL_getvptrs( uint512_dt *  kvdram, unsigned int beginoffset, unsigned int endoffset, unsigned int edgebankID);

	unsigned int ACTSUTIL_getvptr(uint512_dt * kvdram, unsigned int baseoffset_kvs, unsigned int offset);
	tuple_t ACTSUTIL_getvptrs_opt( uint512_dt *  kvdram, unsigned int baseoffset_kvs, unsigned int beginoffset, unsigned int endoffset, unsigned int edgebankID);

	void ACTSUTIL_copyvs(uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], globalparams_t globalparamsK);

	void ACTSUTIL_copystats(uint512_dt * edges, uint512_dt * kvdram, globalparams_t globalparamsE, globalparams_t globalparamsK);

	void ACTSUTIL_copyallstats( uint512_dt * kvdram, globalparams_t globalparamsE, globalparams_t globalparamsK, unsigned int edgebankID);

	// functions (process)
	value_t ACTSUTIL_processfunc(value_t udata, value_t edgew, unsigned int GraphAlgo);

	fetchmessage_t ACTSUTIL_readandprocess(bool_type enable, uint512_dt * edges, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unitBRAMwidth_type vmask_subp[BLOCKRAM_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], 
			batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, sweepparams_t sweepparams, globalparams_t globalparams);

	// functions (partition)
	void ACTSUTIL_preparekeyvalues(bool_type enable1, bool_type enable2, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_buffer_t destbuffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[VECTOR_SIZE][MAX_NUM_PARTITIONS], step_type currentLOP, sweepparams_t sweepparams, buffer_type size_kvs, buffer_type cutoffs[VECTOR_SIZE], globalparams_t globalparams);				

	void ACTSUTIL_priorpartitionkeyvalues(bool_type enable1, bool_type enable2, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_buffer_t destbuffer[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], step_type currentLOP, sweepparams_t sweepparams, buffer_type size_kvs, globalparams_t globalparams);

	// functions (reduce)
	value_t ACTSUTIL_reducefunc(value_t vtemp, value_t res, unsigned int GraphIter, unsigned int GraphAlgo);

	void ACTSUTIL_reducevector(keyvalue_buffer_t kvdata, 
			keyvalue_vbuffer_t destbuffer[BLOCKRAM_SIZE], 
			buffer_type destoffset, unsigned int upperlimit, sweepparams_t sweepparams, globalparams_t globalparams);

	void ACTSUTIL_reduceandbuffer(bool_type enable, keyvalue_buffer_t buffer[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], sweepparams_t sweepparams, globalparams_t globalparams);

	void ACTSUTIL_priorreduceandbuffer(bool_type enable, keyvalue_buffer_t buffer[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], sweepparams_t sweepparams, globalparams_t globalparams);

	void ACTSUTIL_priorreduceandbuffer(bool_type enable, keyvalue_buffer_t buffer[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], buffer_type chunk_size, sweepparams_t sweepparams, globalparams_t globalparams);

	void ACTSUTIL_tradreduceandbuffer(bool_type enable, uint512_dt * kvdram, keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], buffer_type chunk_size, keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], sweepparams_t sweepparams, globalparams_t globalparams);

	// functions (merge)
	unsigned int ACTSUTIL_SYNC_amin(unsigned int val1, unsigned int val2);
	value_t ACTSUTIL_mergefunc(value_t value1, value_t value2, unsigned int GraphAlgo);

	void ACTSUTIL_readandreplicate1vdata(bool_type enable, uint512_dt * vdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	void ACTSUTIL_readandreplicate2vdata(bool_type enable, uint512_dt * vdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	void ACTSUTIL_readandreplicate3vdata(bool_type enable, uint512_dt * vdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	void ACTSUTIL_readandreplicate4vdata(bool_type enable, uint512_dt * vdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	void ACTSUTIL_readandreplicate5vdata(bool_type enable, uint512_dt * vdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	void ACTSUTIL_readandreplicate6vdata(bool_type enable, uint512_dt * vdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	void ACTSUTIL_readandreplicate7vdata(bool_type enable, uint512_dt * vdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	void ACTSUTIL_readandreplicate8vdata(bool_type enable, uint512_dt * vdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	void ACTSUTIL_readandreplicate9vdata(bool_type enable, uint512_dt * vdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	void ACTSUTIL_readandreplicate10vdata(bool_type enable, uint512_dt * vdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	void ACTSUTIL_readandreplicate11vdata(bool_type enable, uint512_dt * vdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer10[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	void ACTSUTIL_readandreplicate12vdata(bool_type enable, uint512_dt * vdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer10[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer11[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);

	void ACTSUTIL_merge1andsavevdata(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, unsigned int dramoffset_kvs); 
	void ACTSUTIL_merge2andsavevdata(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, unsigned int dramoffset_kvs); 
	void ACTSUTIL_merge3andsavevdata(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, unsigned int dramoffset_kvs); 
	void ACTSUTIL_merge4andsavevdata(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, unsigned int dramoffset_kvs); 
	void ACTSUTIL_merge5andsavevdata(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, unsigned int dramoffset_kvs); 
	void ACTSUTIL_merge6andsavevdata(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, unsigned int dramoffset_kvs); 
	void ACTSUTIL_merge7andsavevdata(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, unsigned int dramoffset_kvs); 
	void ACTSUTIL_merge8andsavevdata(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, unsigned int dramoffset_kvs); 
	void ACTSUTIL_merge9andsavevdata(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, unsigned int dramoffset_kvs); 
	void ACTSUTIL_merge10andsavevdata(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer9[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, unsigned int dramoffset_kvs); 
	void ACTSUTIL_merge11andsavevdata(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer9[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer10[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, unsigned int dramoffset_kvs); 
	void ACTSUTIL_merge12andsavevdata(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer9[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer10[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer11[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, unsigned int dramoffset_kvs); 

	// acts
	void ACTSUTIL_resetenvbuffers(keyvalue_capsule_t capsule_so1[VECTOR_SIZE][MAX_NUM_PARTITIONS], keyvalue_capsule_t capsule_so8[MAX_NUM_PARTITIONS]);

	void ACTSUTIL_resetenvbuffer(keyvalue_capsule_t capsule_so8[MAX_NUM_PARTITIONS]);

	fetchmessage_t ACTSUTIL_fetchkeyvalues(bool_type enable, unsigned int mode,  uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unitBRAMwidth_type vmask_subp[BLOCKRAM_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], 
		batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, sweepparams_t sweepparams, globalparams_t globalparams, unsigned int edgebankID);

	void ACTSUTIL_commitkeyvalues(bool_type enable1, bool_type enable2, unsigned int mode, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], 
		keyvalue_buffer_t destbuffer[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_t globalcapsule[MAX_NUM_PARTITIONS], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], 
			batch_type destbaseaddr_kvs, buffer_type chunk_size, sweepparams_t sweepparams, globalparams_t globalparams);

	void ACTSUTIL_priorcommitkeyvalues(bool_type enable1, bool_type enable2, unsigned int mode, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], 
		keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_buffer_t destbuffer[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_t globalcapsule[MAX_NUM_PARTITIONS], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], 
				batch_type destbaseaddr_kvs, buffer_type chunk_size, sweepparams_t sweepparams, globalparams_t globalparams);
				
	void ACTSUTIL_actspipeline(bool_type enable1, bool_type enable2, keyvalue_buffer_t buffer_setof1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_capsule_t capsule_so1[VECTOR_SIZE][MAX_NUM_PARTITIONS], 
						keyvalue_buffer_t buffer_setof8[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_capsule_t capsule_so8[MAX_NUM_PARTITIONS],
							unsigned int currentLOP, sweepparams_t sweepparams, buffer_type cutoffs[VECTOR_SIZE], batch_type shiftcount, globalparams_t globalparams);

	void ACTSUTIL_actit(bool_type enable, unsigned int mode,
 uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unitBRAMwidth_type vmask_subp[BLOCKRAM_SIZE], keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], 
			globalparams_t globalparams, sweepparams_t sweepparams, travstate_t ptravstate, batch_type sourcebaseaddr_kvs, batch_type destbaseaddr_kvs,
			bool_type resetenv, bool_type flush, unsigned int edgebankID);

	void ACTSUTIL_priorit(bool_type enable, unsigned int mode,
 uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unitBRAMwidth_type vmask_subp[BLOCKRAM_SIZE], keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], 
			globalparams_t globalparams, sweepparams_t sweepparams, travstate_t ptravstate, batch_type sourcebaseaddr_kvs, batch_type destbaseaddr_kvs,
			bool_type resetenv, bool_type flush, unsigned int edgebankID);
			
	void ACTSUTIL_tradit(bool_type enable, unsigned int mode,
 uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unitBRAMwidth_type vmask_subp[BLOCKRAM_SIZE], keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], 
			globalparams_t globalparams, sweepparams_t sweepparams, travstate_t ptravstate, batch_type sourcebaseaddr_kvs, batch_type destbaseaddr_kvs,
			bool_type resetenv, bool_type flush, unsigned int edgebankID);
		
// private:
	// #ifndef FPGA_IMPL
	// actsutility * actsutilityobj;
	// #endif
// };
// #endif 