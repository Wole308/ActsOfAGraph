value_t REDUCEP0_reducefunc(value_t vtemp, value_t res, unsigned int GraphIter, unsigned int GraphAlgo);

void REDUCEP0_reducevector(unsigned int col, keyvalue_buffer_t kvdata, keyvalue_vbuffer_t destbuffer[BLOCKRAM_SIZE], unit1_type vmaskBITS[BLOCKRAM_VMASK_SIZE], buffer_type destoffset, unsigned int upperlimit, sweepparams_t sweepparams, globalparams_t globalparams);

void REDUCEP0_reduceandbuffer(bool_type enable, keyvalue_buffer_t buffer[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], unit1_type vmaskBITS[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE], sweepparams_t sweepparams, globalparams_t globalparams);				

void REDUCEP0_priorreduceandbuffer(bool_type enable, keyvalue_buffer_t buffer[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], unit1_type vmaskBITS[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE], buffer_type chunk_size, sweepparams_t sweepparams, globalparams_t globalparams);			

void REDUCEP0_tradreduceandbuffer(bool_type enable, uint512_dt * kvdram, keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], buffer_type chunk_size, keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], sweepparams_t sweepparams, globalparams_t globalparams);







