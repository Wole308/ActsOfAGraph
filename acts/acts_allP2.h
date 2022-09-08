#ifndef ACTS_ALL_H
#define ACTS_ALL_H
#include "../include/config_params.h"
#include "../include/common.h"
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
#include "../host_srcs/utility.h"
#include "actsutility.h"
#include "mydebug.h"
using namespace std;
#endif

#ifdef SW
class acts_all {
public:
	acts_all(mydebug * _mydebugobj);
	~acts_all();
	
	#ifdef CONFIG_ENABLECLASS_ACTS_UTIL
// functions (basic)
unsigned int UTILP2_amin(unsigned int val1, unsigned int val2);
unsigned int UTILP2_amax(unsigned int val1, unsigned int val2);
unsigned int UTILP2_aplus(unsigned int val1, unsigned int val2);
uint32_type UTILP2_amin2(uint32_type val1, uint32_type val2);

void UTILP2_writetoglobalvar(bool write);

// functions (allignment)
batch_type UTILP2_allignlower_KV(batch_type val);
batch_type UTILP2_allignhigher_KV(batch_type val);
batch_type UTILP2_allignlower_KV2(batch_type val);
batch_type UTILP2_allignhigher_KV2(batch_type val);
batch_type UTILP2_allignhigher_FACTOR(batch_type val, unsigned int _FACTOR);
batch_type UTILP2_allignlower_FACTOR(batch_type val, unsigned int _FACTOR);

unsigned int UTILP2_GETNUMPARTITIONS_FIRSTSWEEP_NONRECURSIVEMODE(unsigned int tree_depth);

// functions (bit manipulation)
unsigned int UTILP2_GETMASK_UINT(uint32_type index, unsigned int size);
unsigned int UTILP2_READFROM_UINT(uint32_type data, unsigned int index, unsigned int size);
void UTILP2_WRITETO_UINT(uint32_type * data, unsigned int index, unsigned int size, unsigned int value);
unsigned int UTILP2_READBITSFROM_UINTV(uint32_type data, unsigned int index, unsigned int size);
void UTILP2_WRITEBITSTO_UINTV(uint32_type * data, unsigned int index, unsigned int size, unsigned int value);
unsigned int UTILP2_SWREADBITSFROM_UINTV(uint32_type data, unsigned int index, unsigned int size);
void UTILP2_SWWRITEBITSTO_UINTV(uint32_type * data, unsigned int index, unsigned int size, unsigned int value);

// functions (converters)
keyvalue_t UTILP2_GETKV(keyvalue_buffer_t data);
keyvalue_buffer_t UTILP2_GETKV(keyvalue_t data);
keyy_t UTILP2_GETK(uint32_type data);
value_t UTILP2_GETV(uint32_type data);
keyy_t UTILP2_GETKEYENTRY(uint512_dt data, unsigned int v);
unsigned int UTILP2_GETLOCALVID(unsigned int vid, unsigned int instid);
unsigned int UTILP2_GETREALVID(unsigned int lvid, unsigned int instid);
unsigned int UTILP2_GET_PROCESSEDGESPARTITIONSIZEKVS2(globalparams_t globalparams);

unsigned int UTILP2_GetFirstData(uint512_dt * kvdram, unsigned int offset_kvs);
void UTILP2_SetFirstData(uint512_dt * kvdram, unsigned int offset_kvs, unsigned int data);
void UTILP2_SetFirstDatas(uint512_dt * kvdram, unsigned int offset_kvs, unsigned int * data, unsigned int size);
unsigned int UTILP2_GetSecondData(uint512_dt * kvdram, unsigned int offset_kvs);
void UTILP2_SetSecondData(uint512_dt * kvdram, unsigned int offset_kvs, unsigned int data);
unsigned int UTILP2_GetThirdData(uint512_dt * kvdram, unsigned int offset_kvs);
void UTILP2_SetThirdData(uint512_dt * kvdram, unsigned int offset_kvs, unsigned int data);

void UTILP2_ReadEdges(uint512_dt * kvdram, unsigned int offset_kvs, keyvalue_t datas[EDGEDATA_PACKINGSIZE]);
void UTILP2_ReadEdges(uint512_dt * kvdram, unsigned int offset_kvs, value_t datas[VECTOR2_SIZE]);
uint512_evec_dt UTILP2_ReadEdges(uint512_dt * kvdram, unsigned int offset_kvs);
// void UTILP2_ReadUpdates(uint512_dt * kvdram, unsigned int offset_kvs, keyvalue_t datas[UPDATEDATA_PACKINGSIZE]);

void UTILP2_ReadDatas(uint512_dt * kvdram, unsigned int offset_kvs, value_t datas[VECTOR2_SIZE]);
void UTILP2_ReadDatas(uint512_dt * kvdram, unsigned int offset_kvs, keyvalue_t datas[VECTOR_SIZE]);
void UTILP2_WriteDatas(uint512_dt * kvdram, unsigned int offset_kvs, value_t datas[VECTOR2_SIZE]);
void UTILP2_WriteDatas(uint512_dt * kvdram, unsigned int offset_kvs, keyvalue_t datas[VECTOR_SIZE]);
void UTILP2_WriteDataset(uint512_dt * kvdram, unsigned int offset_kvs, uint512_uvec_dt dataset);
unsigned int UTILP2_ReadData(uint512_dt * kvdram, unsigned int offset_kvs, unsigned int index);
void UTILP2_WriteData(uint512_dt * kvdram, unsigned int offset_kvs, unsigned int index, unsigned int data);
keyvalue_t UTILP2_ReadKV(uint512_dt * kvdram, unsigned int offset_kvs, unsigned int index);

bool UTILP2__processit__i_am_first__(globalposition_t globalposition);
bool UTILP2__processit__i_am_last__(globalposition_t globalposition);
bool UTILP2__reduceit__i_am_last__(globalposition_t globalposition, sweepparams_t sweepparams, globalparams_t globalparams);

// functions (actsproc utilities)
batch_type UTILP2_getskipsize(step_type currentLOP, bool_type sourceORdest, globalparams_t globalparams);
batch_type UTILP2_getrangeforeachllop(globalparams_t globalparams);
buffer_type UTILP2_getchunksize_kvs(buffer_type buffer_size, travstate_t travstate, buffer_type localoffset);
buffer_type UTILP2_getpartitionwritesz(buffer_type realsize_kvs, buffer_type bramoffset_kvs);
void UTILP2_calculateoffsets(keyvalue_capsule_t * buffer, buffer_type size);
void UTILP2_calculatemanyunallignedoffsets(keyvalue_capsule_t buffer[VECTOR_SIZE][MAX_NUM_PARTITIONS], buffer_type size, batch_type base, batch_type skipspacing);
batch_type UTILP2_get_num_source_partitions(step_type currentLOP);
globalparams_t UTILP2_getglobalparams(uint512_dt * kvdram, unsigned int banksection);
globalparams_t UTILP2_getglobalparamsUNUSED(uint512_dt * kvdram, unsigned int banksection);
sweepparams_t UTILP2_getsweepparams(globalparams_t globalparams, step_type currentLOP, batch_type source_partition);

partition_type UTILP2_getpartition(bool_type enable, unsigned int mode, keyvalue_buffer_t keyvalue, step_type currentLOP, vertex_t upperlimit, unsigned int upperpartition, unsigned int batch_range_pow, unsigned int tree_depth);				
partition_type UTILP2_getpartition2(bool_type enable, unsigned int mode, keyvalue_buffer_t keyvalue, step_type currentLOP, vertex_t upperlimit, unsigned int upperpartition, unsigned int batch_range_pow, globalparams_t globalparams);
unsigned int UTILP2_getstatsoffset(unsigned int LOP, globalparams_t globalparams);

// functions (resets)
void UTILP2_resetvalues(keyvalue_t * buffer, buffer_type size, unsigned int resetval);
void UTILP2_resetvalues(keyvalue_capsule_t * buffer, buffer_type size, unsigned int resetval);
void UTILP2_resetvalues(value_t * buffer, buffer_type size, unsigned int resetval);
void UTILP2_resetkeysandvalues(keyvalue_t * buffer, buffer_type size, unsigned int resetval);
void UTILP2_resetkvstatvalues(uint512_dt * kvdram, globalparams_t globalparams);
void UTILP2_resetkvstatvalues(uint512_dt * kvdram, unsigned int size_kvs, globalparams_t globalparams);
void UTILP2_accumkvstats(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams);
void UTILP2_increment_graphiteration(uint512_dt * kvdram, globalparams_t globalparams);
void UTILP2_resetenvbuffers(keyvalue_capsule_t capsule_so1[VECTOR_SIZE][MAX_NUM_PARTITIONS], keyvalue_capsule_t capsule_so8[MAX_NUM_PARTITIONS]);
void UTILP2_resetenvbuffer(keyvalue_capsule_t capsule_so8[MAX_NUM_PARTITIONS]);

// checks 
bool UTILP2_isbufferused(unsigned int id);
void UTILP2_check_capsule(keyvalue_t capsule[MAX_NUM_PARTITIONS], unsigned int num_partitions, unsigned int max_value);




	
	#endif 
	#ifdef CONFIG_ENABLECLASS_ALGO_FUNCS







	
	#endif 
	#ifdef CONFIG_ENABLECLASS_MEM_ACCESS
void MEMACCESSP2_read(bool_type enable, uint512_dt * kvdram, unsigned int buffer[VECTOR2_SIZE][BLOCKRAM_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs, batch_type bufferoffset_kvs, buffer_type size_kvs);

void MEMACCESSP2_readV(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer[VECTOR2_SIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs, batch_type bufferoffset_kvs, buffer_type size_kvs);
	
// -------------------- workload -------------------- //

void rearrangeLayoutVx16F(unsigned int s, keyvalue_t in[EDGEDATA_PACKINGSIZE], keyvalue_t out[EDGEDATA_PACKINGSIZE]);

void rearrangeLayoutVx16B(unsigned int s, keyvalue_t in[EDGEDATA_PACKINGSIZE], keyvalue_t out[EDGEDATA_PACKINGSIZE]);

void check_if_contiguous(keyvalue_t keyvalue[EDGEDATA_PACKINGSIZE], keyvalue_t msg1[EDGEDATA_PACKINGSIZE], keyvalue_t msg2[EDGEDATA_PACKINGSIZE], unsigned int msg1_str, unsigned int msg2_str, unsigned int msg3_str);

keyvaluemask_t process_edge(unsigned int mode, bool enx, unsigned int v, unsigned int loc, keyvalue_t edge_data, keyvalue_vbuffer_t vbuffer[MAX_BLOCKRAM_VDESTDATA_SIZE], globalparams_t globalparams);

unsigned int MEMACCESSP2_process_and_buffer(unsigned int chunk_id, unsigned int mode, unsigned int llp_set, uint512_dt *in, uint512_evec_dt out[BLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], workload_t workload_kvs, collection_t collections[COLLECTIONS_BUFFERSZ], globalparams_t globalparamsK);

void store(unsigned int chunk_id, uint512_dt *out, uint512_evec_dt in[VDATA_PACKINGSIZE], workload_t workload_kvs, unsigned int offsetkvs, unsigned int sizekvs, collection_t collections[COLLECTIONS_BUFFERSZ], globalparams_t globalparamsK);

unsigned int MEMACCESSP2_write__process_base(unsigned int mode, unsigned int llp_set, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], workload_t workload_kvs, collection_t collections[COLLECTIONS_BUFFERSZ], globalparams_t globalparamsK);
	
void reduce_update(unsigned int mode, bool enx, unsigned int v, unsigned int loc, keyvalue_t update_data, keyvalue_vbuffer_t vbuffer[MAX_BLOCKRAM_VDESTDATA_SIZE], unsigned int stats[BLOCKRAM_SIZE], unsigned int memory[1], globalparams_t globalparams);
			
void MEMACCESSP2_read__reduce(unsigned int mode, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], workload_t workload_kvs, collection_t collections[COLLECTIONS_BUFFERSZ], globalparams_t globalparamsK);

// -------------------- stats -------------------- //

unsigned int MEMACCESSP2_get_updateblock_workload(bool en, unsigned int reduce_partition, unsigned int * stats_offsets, unsigned int * stats_metadata, globalparams_t globalparams, workload_t xload_kvs[BLOCKRAM_SIZE], unsigned int buffer_offsets[BLOCKRAM_SIZE]);

unsigned int MEMACCESSP2_get_upropblock_workload(bool en, unsigned int reduce_partition, uint512_dt * dram, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int num_active_upropblocks, globalparams_t globalparams, workload_t xload_kvs[BLOCKRAM_SIZE], unsigned int buffer_offsets[BLOCKRAM_SIZE], unsigned int graphiterationid);

unsigned int MEMACCESSP2_get_edgesMAP(unsigned int en, unsigned int process_partition, unsigned int llp_set, uint512_dt * dram, unsigned int num_active_edgeblocks, globalparams_t globalparamsK, globalparams_t globalparamsE, workload_t xload_kvs[DOUBLE_BLOCKRAM_SIZE], unsigned int graphiterationid);

void MEMACCESSP2_get_vertexblock_ids(bool en, unsigned int process_partition, uint512_dt * dram, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], unsigned int vertexblock_ids[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], collection_t collections_tmp[COLLECTIONS_BUFFERSZ], unsigned int num_active_edgeblocks, globalparams_t globalparamsK, globalparams_t globalparamsE);

unsigned int MEMACCESSP2_get_edgeblock_workload(bool en, uint512_dt * dram, unsigned int process_partition, unsigned int llp_set, unsigned int vertexblock_ids[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int num_active_edgeblocks, globalparams_t globalparamsK, globalparams_t globalparamsE, workload_t xload_kvs[BLOCKRAM_SIZE], unsigned int graphiterationid);			

#ifdef MEMACCESS_ENABLE_ONEINTERFACETODRAM
workload_t MEMACCESSP2_get_edgeblock_offset(bool en, unsigned int process_partition, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], unsigned int num_active_edgeblocks, globalparams_t globalparamsK, globalparams_t globalparamsE);

void MEMACCESSP2_get_edgesmap(bool en, unsigned int process_partition, uint512_dt * dram, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], unsigned int vertexblock_ids[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], collection_t collections_tmp[COLLECTIONS_BUFFERSZ], unsigned int num_active_edgeblocks, globalparams_t globalparamsK, globalparams_t globalparamsE);

unsigned int MEMACCESSP2_get_edgeblock_workload(bool en, uint512_dt * dram, unsigned int process_partition, unsigned int llp_set, unsigned int vertexblock_ids[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int num_active_edgeblocks, globalparams_t globalparamsK, globalparams_t globalparamsE, workload_t xload_kvs[BLOCKRAM_SIZE], unsigned int graphiterationid);
#endif 

// -------------------- stats -------------------- //
void MEMACCESSP2_readglobalstats(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], batch_type offset_kvs, globalparams_t globalparams);

void MEMACCESSP2_readhelperstats(uint512_dt * vdram, pmask_dt pmask[BLOCKRAM_CURRPMASK_SIZE], batch_type offset_kvs, batch_type size_kvs, unsigned int GraphIter, globalparams_t globalparams);

void MEMACCESSP2_savehelperstats(uint512_dt * vdram, pmask_dt pmask[BLOCKRAM_CURRPMASK_SIZE], batch_type offset_kvs, batch_type size_kvs, unsigned int GraphIter, globalparams_t globalparams);

// -------------------- workload (obsolete) -------------------- //

// void MEMACCESSP2_readV(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs, batch_type bufferoffset_kvs, buffer_type size_kvs, globalposition_t globalposition, globalparams_t globalparams);

// void MEMACCESSP2_saveV(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs, batch_type bufferoffset_kvs, buffer_type size_kvs, globalposition_t globalposition, globalparams_t globalparams);				

// // void MEMACCESSP2_readANDRVchunks1(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV);					
// // void MEMACCESSP2_readANDRVchunks2(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV);					
// // void MEMACCESSP2_readANDRVchunks3(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV);					
// // void MEMACCESSP2_readANDRVchunks4(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV);					
// // void MEMACCESSP2_readANDRVchunks5(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV);					
// // void MEMACCESSP2_readANDRVchunks6(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV);					
// // void MEMACCESSP2_readANDRVchunks7(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV);					
// // void MEMACCESSP2_readANDRVchunks8(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV);					
// // void MEMACCESSP2_readANDRVchunks9(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV);					
// // void MEMACCESSP2_readANDRVchunks10(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV);					
// // void MEMACCESSP2_readANDRVchunks11(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer10[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV);					
// // void MEMACCESSP2_readANDRVchunks12(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer10[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer11[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV);					
// 



	
	#endif
	#ifdef CONFIG_ENABLECLASS_ACTS_MERGE
void mergeP2_trace(uint512_dt * kvdram, unsigned int reduce_partition, unsigned int k, unsigned int offset_kvs, globalparams_t globalparams);
	
void MERGEP2_print_active_masks(uint512_dt * vdram, globalparams_t globalparams, unsigned int offset_kvs);

unsigned int MERGEP2_mergeVs(uint512_dt * kvdram, uint512_dt * vdram);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
unsigned int MERGEP2__broadcastACTVVs(uint512_dt * vdram, uint512_dt * kvdram0, unsigned int source_partition, globalparams_t globalparamsK, globalparams_t globalparamsV);
	
unsigned int MERGEP2__broadcastACTVVs(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1, unsigned int source_partition, globalparams_t globalparamsK, globalparams_t globalparamsV);
	
unsigned int MERGEP2__broadcastACTVVs(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2, unsigned int source_partition, globalparams_t globalparamsK, globalparams_t globalparamsV);
	
unsigned int MERGEP2__broadcastACTVVs(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3, unsigned int source_partition, globalparams_t globalparamsK, globalparams_t globalparamsV);
	
unsigned int MERGEP2__broadcastACTVVs(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4, unsigned int source_partition, globalparams_t globalparamsK, globalparams_t globalparamsV);
	
unsigned int MERGEP2__broadcastACTVVs(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5, unsigned int source_partition, globalparams_t globalparamsK, globalparams_t globalparamsV);
	
unsigned int MERGEP2__broadcastACTVVs(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6, unsigned int source_partition, globalparams_t globalparamsK, globalparams_t globalparamsV);
	
unsigned int MERGEP2__broadcastACTVVs(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7, unsigned int source_partition, globalparams_t globalparamsK, globalparams_t globalparamsV);
	
unsigned int MERGEP2__broadcastACTVVs(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8, unsigned int source_partition, globalparams_t globalparamsK, globalparams_t globalparamsV);
	
unsigned int MERGEP2__broadcastACTVVs(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9, unsigned int source_partition, globalparams_t globalparamsK, globalparams_t globalparamsV);
	
unsigned int MERGEP2__broadcastACTVVs(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10, unsigned int source_partition, globalparams_t globalparamsK, globalparams_t globalparamsV);
	
unsigned int MERGEP2__broadcastACTVVs(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11, unsigned int source_partition, globalparams_t globalparamsK, globalparams_t globalparamsV);

unsigned int MERGEP2_mergeACTVVs(uint512_dt * kvdram, uint512_dt * vdram);

unsigned int MERGEP2_copyACTVVs(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC, unsigned int begin_computeunitid, unsigned int num_compute_units);

unsigned int MERGEP2_exchangeACTVVs(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MERGEP2_broadcastVs(uint512_dt * vdram, uint512_dt * kvdram, unsigned int offset1_kvs, unsigned int offset2_kvs, unsigned int size_kvs);

	
void MERGEP2_broadcastVs1(uint512_dt * vdram, uint512_dt * kvdram0, unsigned int offsetSRC_kvs, unsigned int offsetDST_kvs, unsigned int size_kvs);
	
void MERGEP2_broadcastVs2(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1, unsigned int offsetSRC_kvs, unsigned int offsetDST_kvs, unsigned int size_kvs);
	
void MERGEP2_broadcastVs3(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2, unsigned int offsetSRC_kvs, unsigned int offsetDST_kvs, unsigned int size_kvs);
	
void MERGEP2_broadcastVs4(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3, unsigned int offsetSRC_kvs, unsigned int offsetDST_kvs, unsigned int size_kvs);
	
void MERGEP2_broadcastVs5(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4, unsigned int offsetSRC_kvs, unsigned int offsetDST_kvs, unsigned int size_kvs);
	
void MERGEP2_broadcastVs6(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5, unsigned int offsetSRC_kvs, unsigned int offsetDST_kvs, unsigned int size_kvs);
	
void MERGEP2_broadcastVs7(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6, unsigned int offsetSRC_kvs, unsigned int offsetDST_kvs, unsigned int size_kvs);
	
void MERGEP2_broadcastVs8(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7, unsigned int offsetSRC_kvs, unsigned int offsetDST_kvs, unsigned int size_kvs);
	
void MERGEP2_broadcastVs9(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8, unsigned int offsetSRC_kvs, unsigned int offsetDST_kvs, unsigned int size_kvs);
	
void MERGEP2_broadcastVs10(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9, unsigned int offsetSRC_kvs, unsigned int offsetDST_kvs, unsigned int size_kvs);
	
void MERGEP2_broadcastVs11(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10, unsigned int offsetSRC_kvs, unsigned int offsetDST_kvs, unsigned int size_kvs);
	
void MERGEP2_broadcastVs12(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11, unsigned int offsetSRC_kvs, unsigned int offsetDST_kvs, unsigned int size_kvs);

unsigned int MERGEP2_copy(unsigned int idx, unsigned int reduce_partition, uint512_dt * vdramSRC, uint512_dt * vdramDST1, uint512_dt * vdramDST2, uint512_dt * vdramREF, unsigned int stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int voffset_kvs, unsigned int firstinstance_id, unsigned int num_compute_units, 
	pmask_dt vpartition_stats___[BLOCKRAM_CURRPMASK_SIZE], wide_word_bits_3t edgeblock_stats[MAX_RED_SRAMSZ],
	globalparams_t globalparamsSRC, globalparams_t globalparamsDST1, globalparams_t globalparamsDST2);

void MERGEP2_exchange(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC);

void TOPP2_topkernelS(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC);







	
	#endif 
	#ifdef CONFIG_ENABLECLASS_ACTS
void ACTSP2_actit(bool_type enable, unsigned int mode,
		uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], workload_t edgeptrload_kvs[EDGESTATSRAM_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE],
			keyvalue_t globalcapsule[BLOCKRAM_GLOBALSTATS_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition, sweepparams_t sweepparams, batch_type sourcebaseaddr_kvs, batch_type destbaseaddr_kvs,
				bool_type resetenv, bool_type flush, unsigned int edgebankID, collection_t collections[COLLECTIONS_BUFFERSZ]);

	
							
	#endif 		
void TOPP2_processit(uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], workload_t edgestats[EDGESTATSRAM_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], keyvalue_t globalcapsule[BLOCKRAM_GLOBALSTATS_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition,
		unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID, collection_t collections[COLLECTIONS_BUFFERSZ]);							

void TOPP2_partitionit(uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], workload_t edgestats[EDGESTATSRAM_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK, unsigned int edgebankID, collection_t collections[COLLECTIONS_BUFFERSZ]);
	
void TOPP2_reduceit(uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], workload_t edgestats[EDGESTATSRAM_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], keyvalue_t globalcapsule[BLOCKRAM_GLOBALSTATS_SIZE], batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparamsE, globalparams_t globalparamsK, unsigned int edgebankID, collection_t collections[COLLECTIONS_BUFFERSZ]);					

void TOPP2_dispatch(bool_type en_process, bool_type en_partition, bool_type en_reduce,  uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], workload_t edgestats[EDGESTATSRAM_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], keyvalue_t globalcapsule[BLOCKRAM_GLOBALSTATS_SIZE],
		batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition,
			unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID, collection_t collections[COLLECTIONS_BUFFERSZ]);

void TOPP2_dispatch_reduce(uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], workload_t edgestats[EDGESTATSRAM_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], keyvalue_t globalcapsule[BLOCKRAM_GLOBALSTATS_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK, globalparams_t globalparamsV, globalposition_t globalposition,	
					unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID, collection_t collections[COLLECTIONS_BUFFERSZ]);
					
void TOPP2_topkernelproc_embedded(unsigned int GraphIter, unsigned int globalid, unsigned int localid, unsigned int en_process, unsigned int en_partition, unsigned int en_reduce,  uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], workload_t edgestats[EDGESTATSRAM_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], keyvalue_t globalcapsule[BLOCKRAM_GLOBALSTATS_SIZE], globalposition_t globalposition, globalparams_t globalparamsV, collection_t collections[COLLECTIONS_BUFFERSZ]);									
	
// top
void TOPP2_topkernelP1(
	uint512_dt * kvdram0,
	uint512_dt * vdram
	);
void TOPP2_topkernelP2(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * vdram
	);
void TOPP2_topkernelP3(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * vdram
	);
void TOPP2_topkernelP4(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * vdram
	);
void TOPP2_topkernelP5(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * kvdram4,
	uint512_dt * vdram
	);
void TOPP2_topkernelP6(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * kvdram4,
	uint512_dt * kvdram5,
	uint512_dt * vdram
	);
void TOPP2_topkernelP7(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * kvdram4,
	uint512_dt * kvdram5,
	uint512_dt * kvdram6,
	uint512_dt * vdram
	);
void TOPP2_topkernelP8(
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
void TOPP2_topkernelP9(
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
void TOPP2_topkernelP10(
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
void TOPP2_topkernelP11(
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
void TOPP2_topkernelP12(
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
	mydebug * mydebugobj;
	#endif 
};
#endif 
#endif 




