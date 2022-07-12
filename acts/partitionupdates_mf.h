// single stage partitioning 
void PARTITIONP0_read_process_partition(bool_type enable, unsigned int mode, unsigned int offset_kvs, unsigned int sourcebaseaddr_kvs, 
		uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_buffer_t destbuffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_capsule_t capsule_dest[MAX_NUM_PARTITIONS], 
			travstate_t ptravstate, sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]);

// acts partitioner
void PARTITIONP0_partition_stage1(bool_type enable1, bool_type enable2, unsigned int mode, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][MAX_SOURCEBLOCKRAM_SIZE], keyvalue_buffer_t destbuffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[VECTOR_SIZE][MAX_NUM_PARTITIONS], step_type currentLOP, sweepparams_t sweepparams, globalposition_t globalposition, buffer_type size_kvs, buffer_type cutoffs[VECTOR_SIZE], globalparams_t globalparams);				

void PARTITIONP0_partition_stage1_trad(bool_type enable1, bool_type enable2, unsigned int mode, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][MAX_SOURCEBLOCKRAM_SIZE], keyvalue_buffer_t destbuffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[VECTOR_SIZE][MAX_NUM_PARTITIONS], step_type currentLOP, sweepparams_t sweepparams, globalposition_t globalposition, buffer_type size_kvs, buffer_type cutoffs[VECTOR_SIZE], globalparams_t globalparams);				
	
void PARTITIONP0_ACTS2stage1partitioning(bool_type enable1, bool_type enable2, unsigned int mode, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][MAX_SOURCEBLOCKRAM_SIZE], keyvalue_buffer_t destbuffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[VECTOR_SIZE][MAX_NUM_PARTITIONS], step_type currentLOP, sweepparams_t sweepparams, globalposition_t globalposition, buffer_type size_kvs, buffer_type cutoffs[VECTOR_SIZE], globalparams_t globalparams);

void PARTITIONP0_partitionbase_stage1(bool_type enable1, bool_type enable2, unsigned int mode, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][MAX_SOURCEBLOCKRAM_SIZE], keyvalue_buffer_t destbuffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[VECTOR_SIZE][MAX_NUM_PARTITIONS], step_type currentLOP, sweepparams_t sweepparams, globalposition_t globalposition, buffer_type size_kvs, buffer_type cutoffs[VECTOR_SIZE], globalparams_t globalparams);				
	
// acts pipeline
void PARTITIONP0_partition_stage2(bool_type enable1, bool_type enable2, unsigned int mode, keyvalue_buffer_t buffer_setof1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_capsule_t capsule_so1[VECTOR_SIZE][MAX_NUM_PARTITIONS], 
					keyvalue_buffer_t buffer_setof8[VECTOR_SIZE][MAX_DESTBLOCKRAM_SIZE], keyvalue_capsule_t capsule_so8[MAX_NUM_PARTITIONS],
						unsigned int currentLOP, sweepparams_t sweepparams, globalposition_t globalposition, buffer_type cutoffs[VECTOR_SIZE], batch_type shiftcount, globalparams_t globalparams);
						
void PARTITIONP0_partition_stage2_trad(bool_type enable1, bool_type enable2, unsigned int mode, keyvalue_buffer_t buffer_setof1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_capsule_t capsule_so1[VECTOR_SIZE][MAX_NUM_PARTITIONS], 
					keyvalue_buffer_t buffer_setof8[VECTOR_SIZE][MAX_DESTBLOCKRAM_SIZE], keyvalue_capsule_t capsule_so8[MAX_NUM_PARTITIONS],
						unsigned int currentLOP, sweepparams_t sweepparams, globalposition_t globalposition, buffer_type cutoffs[VECTOR_SIZE], batch_type shiftcount, globalparams_t globalparams);

void PARTITIONP0_partitionbase_stage2(bool_type enable1, bool_type enable2, unsigned int mode, keyvalue_buffer_t buffer_setof1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_capsule_t capsule_so1[VECTOR_SIZE][MAX_NUM_PARTITIONS], 
					keyvalue_buffer_t buffer_setof8[VECTOR_SIZE][MAX_DESTBLOCKRAM_SIZE], keyvalue_capsule_t capsule_so8[MAX_NUM_PARTITIONS],
						unsigned int currentLOP, sweepparams_t sweepparams, globalposition_t globalposition, buffer_type cutoffs[VECTOR_SIZE], batch_type shiftcount, globalparams_t globalparams);
						





