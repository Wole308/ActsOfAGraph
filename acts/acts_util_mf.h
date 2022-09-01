// functions (basic)
unsigned int UTILP0_amin(unsigned int val1, unsigned int val2);
unsigned int UTILP0_amax(unsigned int val1, unsigned int val2);
unsigned int UTILP0_aplus(unsigned int val1, unsigned int val2);
uint32_type UTILP0_amin2(uint32_type val1, uint32_type val2);

void UTILP0_writetoglobalvar(bool write);

// functions (allignment)
batch_type UTILP0_allignlower_KV(batch_type val);
batch_type UTILP0_allignhigher_KV(batch_type val);
batch_type UTILP0_allignlower_KV2(batch_type val);
batch_type UTILP0_allignhigher_KV2(batch_type val);
batch_type UTILP0_allignhigher_FACTOR(batch_type val, unsigned int _FACTOR);
batch_type UTILP0_allignlower_FACTOR(batch_type val, unsigned int _FACTOR);

unsigned int UTILP0_GETNUMPARTITIONS_FIRSTSWEEP_NONRECURSIVEMODE(unsigned int tree_depth);

// functions (bit manipulation)
unsigned int UTILP0_GETMASK_UINT(uint32_type index, unsigned int size);
unsigned int UTILP0_READFROM_UINT(uint32_type data, unsigned int index, unsigned int size);
void UTILP0_WRITETO_UINT(uint32_type * data, unsigned int index, unsigned int size, unsigned int value);
unsigned int UTILP0_READBITSFROM_UINTV(uint32_type data, unsigned int index, unsigned int size);
void UTILP0_WRITEBITSTO_UINTV(uint32_type * data, unsigned int index, unsigned int size, unsigned int value);
unsigned int UTILP0_SWREADBITSFROM_UINTV(uint32_type data, unsigned int index, unsigned int size);
void UTILP0_SWWRITEBITSTO_UINTV(uint32_type * data, unsigned int index, unsigned int size, unsigned int value);

// functions (converters)
keyvalue_t UTILP0_GETKV(keyvalue_buffer_t data);
keyvalue_buffer_t UTILP0_GETKV(keyvalue_t data);
keyy_t UTILP0_GETK(uint32_type data);
value_t UTILP0_GETV(uint32_type data);
keyy_t UTILP0_GETKEYENTRY(uint512_dt data, unsigned int v);
unsigned int UTILP0_GETLOCALVID(unsigned int vid, unsigned int instid);
unsigned int UTILP0_GETREALVID(unsigned int lvid, unsigned int instid);
unsigned int UTILP0_GET_PROCESSEDGESPARTITIONSIZEKVS2(globalparams_t globalparams);

unsigned int UTILP0_GetFirstData(uint512_dt * kvdram, unsigned int offset_kvs);
void UTILP0_SetFirstData(uint512_dt * kvdram, unsigned int offset_kvs, unsigned int data);
void UTILP0_SetFirstDatas(uint512_dt * kvdram, unsigned int offset_kvs, unsigned int * data, unsigned int size);
unsigned int UTILP0_GetSecondData(uint512_dt * kvdram, unsigned int offset_kvs);
void UTILP0_SetSecondData(uint512_dt * kvdram, unsigned int offset_kvs, unsigned int data);
unsigned int UTILP0_GetThirdData(uint512_dt * kvdram, unsigned int offset_kvs);
void UTILP0_SetThirdData(uint512_dt * kvdram, unsigned int offset_kvs, unsigned int data);

void UTILP0_ReadEdges(uint512_dt * kvdram, unsigned int offset_kvs, keyvalue_t datas[EDGEDATA_PACKINGSIZE]);
void UTILP0_ReadEdges(uint512_dt * kvdram, unsigned int offset_kvs, value_t datas[VECTOR2_SIZE]);
uint512_evec_dt UTILP0_ReadEdges(uint512_dt * kvdram, unsigned int offset_kvs);
// void UTILP0_ReadUpdates(uint512_dt * kvdram, unsigned int offset_kvs, keyvalue_t datas[UPDATEDATA_PACKINGSIZE]);

void UTILP0_ReadDatas(uint512_dt * kvdram, unsigned int offset_kvs, value_t datas[VECTOR2_SIZE]);
void UTILP0_ReadDatas(uint512_dt * kvdram, unsigned int offset_kvs, keyvalue_t datas[VECTOR_SIZE]);
void UTILP0_WriteDatas(uint512_dt * kvdram, unsigned int offset_kvs, value_t datas[VECTOR2_SIZE]);
void UTILP0_WriteDatas(uint512_dt * kvdram, unsigned int offset_kvs, keyvalue_t datas[VECTOR_SIZE]);
void UTILP0_WriteDataset(uint512_dt * kvdram, unsigned int offset_kvs, uint512_uvec_dt dataset);
unsigned int UTILP0_ReadData(uint512_dt * kvdram, unsigned int offset_kvs, unsigned int index);
void UTILP0_WriteData(uint512_dt * kvdram, unsigned int offset_kvs, unsigned int index, unsigned int data);
keyvalue_t UTILP0_ReadKV(uint512_dt * kvdram, unsigned int offset_kvs, unsigned int index);

bool UTILP0__processit__i_am_first__(globalposition_t globalposition);
bool UTILP0__processit__i_am_last__(globalposition_t globalposition);
bool UTILP0__reduceit__i_am_last__(globalposition_t globalposition, sweepparams_t sweepparams, globalparams_t globalparams);

// functions (actsproc utilities)
batch_type UTILP0_getskipsize(step_type currentLOP, bool_type sourceORdest, globalparams_t globalparams);
batch_type UTILP0_getrangeforeachllop(globalparams_t globalparams);
buffer_type UTILP0_getchunksize_kvs(buffer_type buffer_size, travstate_t travstate, buffer_type localoffset);
buffer_type UTILP0_getpartitionwritesz(buffer_type realsize_kvs, buffer_type bramoffset_kvs);
void UTILP0_calculateoffsets(keyvalue_capsule_t * buffer, buffer_type size);
void UTILP0_calculatemanyunallignedoffsets(keyvalue_capsule_t buffer[VECTOR_SIZE][MAX_NUM_PARTITIONS], buffer_type size, batch_type base, batch_type skipspacing);
batch_type UTILP0_get_num_source_partitions(step_type currentLOP);
globalparams_t UTILP0_getglobalparams(uint512_dt * kvdram, unsigned int banksection);
globalparams_t UTILP0_getglobalparamsUNUSED(uint512_dt * kvdram, unsigned int banksection);
sweepparams_t UTILP0_getsweepparams(globalparams_t globalparams, step_type currentLOP, batch_type source_partition);

partition_type UTILP0_getpartition(bool_type enable, unsigned int mode, keyvalue_buffer_t keyvalue, step_type currentLOP, vertex_t upperlimit, unsigned int upperpartition, unsigned int batch_range_pow, unsigned int tree_depth);				
partition_type UTILP0_getpartition2(bool_type enable, unsigned int mode, keyvalue_buffer_t keyvalue, step_type currentLOP, vertex_t upperlimit, unsigned int upperpartition, unsigned int batch_range_pow, globalparams_t globalparams);
unsigned int UTILP0_getstatsoffset(unsigned int LOP, globalparams_t globalparams);

// functions (resets)
void UTILP0_resetvalues(keyvalue_t * buffer, buffer_type size, unsigned int resetval);
void UTILP0_resetvalues(keyvalue_capsule_t * buffer, buffer_type size, unsigned int resetval);
void UTILP0_resetvalues(value_t * buffer, buffer_type size, unsigned int resetval);
void UTILP0_resetkeysandvalues(keyvalue_t * buffer, buffer_type size, unsigned int resetval);
void UTILP0_resetkvstatvalues(uint512_dt * kvdram, globalparams_t globalparams);
void UTILP0_resetkvstatvalues(uint512_dt * kvdram, unsigned int size_kvs, globalparams_t globalparams);
void UTILP0_accumkvstats(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams);
void UTILP0_increment_graphiteration(uint512_dt * kvdram, globalparams_t globalparams);
void UTILP0_resetenvbuffers(keyvalue_capsule_t capsule_so1[VECTOR_SIZE][MAX_NUM_PARTITIONS], keyvalue_capsule_t capsule_so8[MAX_NUM_PARTITIONS]);
void UTILP0_resetenvbuffer(keyvalue_capsule_t capsule_so8[MAX_NUM_PARTITIONS]);

// checks 
bool UTILP0_isbufferused(unsigned int id);
void UTILP0_check_capsule(keyvalue_t capsule[MAX_NUM_PARTITIONS], unsigned int num_partitions, unsigned int max_value);




