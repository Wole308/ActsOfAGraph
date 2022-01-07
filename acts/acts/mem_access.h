#ifndef MEM_ACCESS_H
#define MEM_ACCESS_H
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
#include "../../acts/actsutility/actsutility.h"
#include "../../src/utility/utility.h"
#endif
#include "acts_util.h"
using namespace std;

class mem_access {
public:
	mem_access();
	~mem_access();
	
	fetchmessage_t MEMACCESS_readkeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs, travstate_t travstate, globalparams_t globalparams);
	
	void MEMACCESS_savekeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_buffer_t buffer[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_t * globalcapsule, keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], batch_type globalbaseaddress_kvs, globalparams_t globalparams);
	
	void MEMACCESS_readkeyvalues(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_buffer_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);

	void MEMACCESS_savekeyvalues(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_buffer_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	
	void MEMACCESS_readvdata(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);

	void MEMACCESS_savevdata(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	
	void MEMACCESS_readallvdatas_sliceddstvtxs(bool_type enable, uint512_dt * kvdram, batch_type vbaseoffset_kvs, batch_type vreadoffset_kvs2, batch_type vreadsz_kvs2, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], globalparams_t globalparams);

	void MEMACCESS_loadvmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams);
	
	void MEMACCESS_savemasks(bool_type enable, uint512_dt * kvdram, unit1_type vmaskBITS[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], batch_type vmask_offset_kvs, unsigned int vmaskp_offset_kvs, globalparams_t globalparams);
	
	void MEMACCESS_loadallvmasks_sliceddstvtxs(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask[BLOCKRAM_SIZE], keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type vmbaseoffset_kvs, batch_type vmreadoffset_kvs, batch_type vmreadsz_kvs, globalparams_t globalparams);				
	
	void MEMACCESS_loadmany1vmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
	void MEMACCESS_loadmany2vmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
	void MEMACCESS_loadmany3vmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
	void MEMACCESS_loadmany4vmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
	void MEMACCESS_loadmany5vmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
	void MEMACCESS_loadmany6vmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
	void MEMACCESS_loadmany7vmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5[BLOCKRAM_SIZE],unitBRAMwidth_type vmask6[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
	void MEMACCESS_loadmany8vmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5[BLOCKRAM_SIZE],unitBRAMwidth_type vmask6[BLOCKRAM_SIZE],unitBRAMwidth_type vmask7[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
	void MEMACCESS_loadmany9vmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5[BLOCKRAM_SIZE],unitBRAMwidth_type vmask6[BLOCKRAM_SIZE],unitBRAMwidth_type vmask7[BLOCKRAM_SIZE],unitBRAMwidth_type vmask8[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
	void MEMACCESS_loadmany10vmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5[BLOCKRAM_SIZE],unitBRAMwidth_type vmask6[BLOCKRAM_SIZE],unitBRAMwidth_type vmask7[BLOCKRAM_SIZE],unitBRAMwidth_type vmask8[BLOCKRAM_SIZE],unitBRAMwidth_type vmask9[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
	void MEMACCESS_loadmany11vmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5[BLOCKRAM_SIZE],unitBRAMwidth_type vmask6[BLOCKRAM_SIZE],unitBRAMwidth_type vmask7[BLOCKRAM_SIZE],unitBRAMwidth_type vmask8[BLOCKRAM_SIZE],unitBRAMwidth_type vmask9[BLOCKRAM_SIZE],unitBRAMwidth_type vmask10[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
	void MEMACCESS_loadmany12vmasks(bool_type enable, uint512_dt * kvdram, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5[BLOCKRAM_SIZE],unitBRAMwidth_type vmask6[BLOCKRAM_SIZE],unitBRAMwidth_type vmask7[BLOCKRAM_SIZE],unitBRAMwidth_type vmask8[BLOCKRAM_SIZE],unitBRAMwidth_type vmask9[BLOCKRAM_SIZE],unitBRAMwidth_type vmask10[BLOCKRAM_SIZE],unitBRAMwidth_type vmask11[BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type offset_kvs, buffer_type size_kvs, globalparams_t globalparams);					
	
	void MEMACCESS_loadmany1vmasks_subp(bool_type enable, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], unitBRAMwidth_type vmask0_subp[BLOCKRAM_SIZE], buffer_type size_kvs);					
	void MEMACCESS_loadmany2vmasks_subp(bool_type enable, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], unitBRAMwidth_type vmask0_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1_subp[BLOCKRAM_SIZE], buffer_type size_kvs);					
	void MEMACCESS_loadmany3vmasks_subp(bool_type enable, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], unitBRAMwidth_type vmask0_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2_subp[BLOCKRAM_SIZE], buffer_type size_kvs);					
	void MEMACCESS_loadmany4vmasks_subp(bool_type enable, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], unitBRAMwidth_type vmask0_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3_subp[BLOCKRAM_SIZE], buffer_type size_kvs);					
	void MEMACCESS_loadmany5vmasks_subp(bool_type enable, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], unitBRAMwidth_type vmask0_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4_subp[BLOCKRAM_SIZE], buffer_type size_kvs);					
	void MEMACCESS_loadmany6vmasks_subp(bool_type enable, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], unitBRAMwidth_type vmask0_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5_subp[BLOCKRAM_SIZE], buffer_type size_kvs);					
	void MEMACCESS_loadmany7vmasks_subp(bool_type enable, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], unitBRAMwidth_type vmask0_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask6_subp[BLOCKRAM_SIZE], buffer_type size_kvs);					
	void MEMACCESS_loadmany8vmasks_subp(bool_type enable, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], unitBRAMwidth_type vmask0_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask6_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask7_subp[BLOCKRAM_SIZE], buffer_type size_kvs);					
	void MEMACCESS_loadmany9vmasks_subp(bool_type enable, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], unitBRAMwidth_type vmask0_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask6_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask7_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask8_subp[BLOCKRAM_SIZE], buffer_type size_kvs);					
	void MEMACCESS_loadmany10vmasks_subp(bool_type enable, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], unitBRAMwidth_type vmask0_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask6_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask7_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask8_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask9_subp[BLOCKRAM_SIZE], buffer_type size_kvs);					
	void MEMACCESS_loadmany11vmasks_subp(bool_type enable, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], unitBRAMwidth_type vmask0_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask6_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask7_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask8_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask9_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask10_subp[BLOCKRAM_SIZE], buffer_type size_kvs);					
	void MEMACCESS_loadmany12vmasks_subp(bool_type enable, unitBRAMwidth_type vmask0[BLOCKRAM_SIZE], unitBRAMwidth_type vmask0_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask1_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask2_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask3_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask4_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask5_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask6_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask7_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask8_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask9_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask10_subp[BLOCKRAM_SIZE],unitBRAMwidth_type vmask11_subp[BLOCKRAM_SIZE], buffer_type size_kvs);					
	
	void MEMACCESS_loadvmask_p(uint512_dt * kvdram, uint32_type vmask_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
	
	void MEMACCESS_loadmany1vmask_p(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
	void MEMACCESS_loadmany2vmask_p(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
	void MEMACCESS_loadmany3vmask_p(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
	void MEMACCESS_loadmany4vmask_p(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
	void MEMACCESS_loadmany5vmask_p(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
	void MEMACCESS_loadmany6vmask_p(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
	void MEMACCESS_loadmany7vmask_p(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
	void MEMACCESS_loadmany8vmask_p(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE],uint32_type vmask7_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
	void MEMACCESS_loadmany9vmask_p(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE],uint32_type vmask7_p[BLOCKRAM_SIZE],uint32_type vmask8_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
	void MEMACCESS_loadmany10vmask_p(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE],uint32_type vmask7_p[BLOCKRAM_SIZE],uint32_type vmask8_p[BLOCKRAM_SIZE],uint32_type vmask9_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
	void MEMACCESS_loadmany11vmask_p(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE],uint32_type vmask7_p[BLOCKRAM_SIZE],uint32_type vmask8_p[BLOCKRAM_SIZE],uint32_type vmask9_p[BLOCKRAM_SIZE],uint32_type vmask10_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);
	void MEMACCESS_loadmany12vmask_p(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE],uint32_type vmask7_p[BLOCKRAM_SIZE],uint32_type vmask8_p[BLOCKRAM_SIZE],uint32_type vmask9_p[BLOCKRAM_SIZE],uint32_type vmask10_p[BLOCKRAM_SIZE],uint32_type vmask11_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs);

	void MEMACCESS_readglobalstats(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], batch_type offset_kvs, globalparams_t globalparams);

	void MEMACCESS_saveglobalstats(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], batch_type offset_kvs, globalparams_t globalparams);
	
	tuple_t MEMACCESS_getvptrs( uint512_dt *  kvdram, unsigned int beginoffset, unsigned int endoffset, unsigned int edgebankID);
	
	unsigned int MEMACCESS_getvptr(uint512_dt * kvdram, unsigned int baseoffset_kvs, unsigned int offset);
	
	tuple_t MEMACCESS_getvptrs_opt( uint512_dt *  kvdram, unsigned int baseoffset_kvs, unsigned int beginoffset, unsigned int endoffset, unsigned int edgebankID);
	
	void MEMACCESS_copyvs(uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], globalparams_t globalparamsK);
	
	void MEMACCESS_copystats(uint512_dt * edges, uint512_dt * kvdram, globalparams_t globalparamsE, globalparams_t globalparamsK);
	
	void MEMACCESS_copyallstats( uint512_dt * kvdram, globalparams_t globalparamsE, globalparams_t globalparamsK, unsigned int edgebankID);
	
	void MEMACCESS_commitkvstats(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams, unsigned int offset);
	
	void MEMACCESS_commitkvstats2(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams, unsigned int offset, unsigned int size);
	
	void MEMACCESS_retreievekvstats(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams, unsigned int offset, unsigned int size);

private:
	#ifdef SW
	actsutility * actsutilityobj;
	acts_util * acts_utilobj;
	#endif
};
#endif 





