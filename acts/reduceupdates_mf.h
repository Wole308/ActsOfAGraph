void REDUCEP0_reducevector(bool enx, unsigned int col, keyvalue_buffer_t kvdata, keyvalue_vbuffer_t vbuffer[MAX_BLOCKRAM_VDESTDATA_SIZE], buffer_type destoffset, unsigned int upperlimit, unsigned int * memory, sweepparams_t sweepparams, globalparams_t globalparams);

void REDUCEP0_GetXYLayoutV(unsigned int s, unsigned int depths[VECTOR_SIZE], unsigned int basedepth);

void REDUCEP0_RearrangeLayoutV(unsigned int s, keyvalue_buffer_t vdata[VECTOR_SIZE], keyvalue_buffer_t vdata2[VECTOR_SIZE]);

void REDUCEP0_RearrangeLayoutEn(unsigned int s, bool en[VECTOR_SIZE], bool en2[VECTOR_SIZE]);

void REDUCEP0_reduceandbuffer(bool_type enable, keyvalue_buffer_t buffer[VECTOR_SIZE][MAX_DESTBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]);					

void REDUCEP0_reduceandbuffer2(bool_type enable, keyvalue_buffer_t buffer[VECTOR_SIZE][MAX_DESTBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]);

void REDUCEP0_reduceandbuffer3(bool_type enable, keyvalue_buffer_t buffer[VECTOR_SIZE][MAX_DESTBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]);

void REDUCEP0_TRADreduceandbuffer(bool_type enable, keyvalue_buffer_t buffer[VECTOR_SIZE][MAX_DESTBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]);					

void REDUCEP0_reduceandbuffer_base(bool_type enable, keyvalue_buffer_t buffer[VECTOR_SIZE][MAX_DESTBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]);					
