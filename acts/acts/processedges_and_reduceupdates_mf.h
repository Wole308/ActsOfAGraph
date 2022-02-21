#define PE_SETSZ 16
#define RU_SETSZ 8
value_t PANDRP0_processfunc(value_t udata, value_t edgew, unsigned int GraphAlgo);

value_t PANDRP0_reducefunc(value_t vtemp, value_t res, unsigned int GraphIter, unsigned int GraphAlgo);

void PANDRP0_processorreducevector(bool enx, unsigned int mode, unsigned int col, unsigned int _loc, keyvalue_t kvdata, 
		keyvalue_vbuffer_t vbuffer[BLOCKRAM_VDATA_SIZE], keyvalue_buffer_t buffer[SOURCEBLOCKRAM_SIZE], unsigned int * loadcount, 
			unsigned int GraphAlgoClass, unsigned int upperlimit, sweepparams_t sweepparams, globalparams_t globalparams);

void PANDRP0_GetXYLayoutV(unsigned int s, unsigned int depths[PE_SETSZ], unsigned int basedepth);

parsededge_t PANDRP0_PARSEEDGE(uint32_type data);

fetchmessage_t PANDRP0_processandreduce(bool_type enable, unsigned int mode, 
		uint512_dt * edges, uint512_dt * kvdram, 
			keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE],
				keyvalue_t globalcapsule[MAX_NUM_PARTITIONS], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS],
					batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, 
						travstate_t travstate, sweepparams_t sweepparams, globalparams_t globalparams);

void REDUCEP0_priorreduceandbuffer(bool_type enable, keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], buffer_type chunk_size, sweepparams_t sweepparams, globalparams_t globalparams);

void REDUCEP0_tradreduceandbuffer(bool_type enable, uint512_dt * kvdram, keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], buffer_type chunk_size, keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], sweepparams_t sweepparams, globalparams_t globalparams);









