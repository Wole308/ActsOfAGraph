value_t PROCESSP0_SPL_processfunc(value_t udata, value_t edgew, unsigned int GraphAlgo);

parsededge_t PROCESSP0_PARSEEDGE(uint32_type data);

void PROCESSP0_SPL_debug(unsigned int debugid,
	unsigned int i, value_t E[VECTOR2_SIZE], bool_type ens[VECTOR2_SIZE], unsigned int mask[VECTOR2_SIZE],
		value_t udataset[MAX_NUM_UNIQ_EDGES_PER_VEC], value_t maskset[VECTOR2_SIZE], value_t Vset[MAX_NUM_UNIQ_EDGES_PER_VEC], unit1_type VMset[MAX_NUM_UNIQ_EDGES_PER_VEC], vertex_t lvids[VECTOR2_SIZE],
			unsigned int incr[VECTOR2_SIZE], unsigned int lsrcvids[VECTOR2_SIZE], unsigned int ldstvids[VECTOR2_SIZE], value_t res[VECTOR2_SIZE], keyvalue_t mykeyvalue[VECTOR2_SIZE], sweepparams_t sweepparams, globalparams_t globalparams,
				unsigned int lvid_head, unsigned int srcvid_head, travstate_t travstate, unsigned int chunk_size, sliceinfos_t sliceinfos, unsigned int * activeloadcount, unsigned int * inactiveloadcount, unsigned int * debug_numinvalidheads);
				
unsigned int PROCESSP0_SPL3_GETROW(unsigned int loc);

unsigned int PROCESSP0_SPL3_GETVTXDATASET(unsigned int loc, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], value_t Vdatas[32], unit1_type VMdatas[32], globalparams_t globalparams);

fetchmessage_t PROCESSP0_SPL_readandprocess(bool_type enable, uint512_dt * edges, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], 
		batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, sweepparams_t sweepparams, globalparams_t globalparams);	






