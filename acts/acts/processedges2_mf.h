void PROCESSP0_processvector(bool enx, unsigned int v, unsigned int loc, keyvalue_t edata, keyvalue_vbuffer_t vbuffer[BLOCKRAM_VDATA_SIZE], keyvalue_buffer_t buffer[SOURCEBLOCKRAM_SIZE], unsigned int bufferoffset_kvs, unsigned int * loadcount, unsigned int GraphAlgoClass, globalposition_t globalposition, globalparams_t globalparams 
	#ifdef CONFIG_COLLECT_DATAS1_DURING_RUN
	,collection_t collections[COLLECTIONS_BUFFERSZ]
	#endif
	);			

void PROCESSP0_GetXYLayoutV(unsigned int s, unsigned int depths[VECTOR_SIZE], unsigned int basedepth);

void PROCESSP0_RearrangeLayoutV(unsigned int s, uint32_type vdata[VECTOR_SIZE], uint32_type vdata2[VECTOR_SIZE]);

void PROCESSP0_RearrangeLayoutEn(unsigned int s, bool en[VECTOR_SIZE], bool en2[VECTOR_SIZE]);

parsededge_t PROCESSP0_PARSEEDGE(uint32_type data);

void PROCESSP0_calculateoffsets(keyvalue_capsule_t * buffer);

fetchmessage_t PROCESSP0_readandprocess(bool_type enable, uint512_dt * edges, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], 
		batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]);






