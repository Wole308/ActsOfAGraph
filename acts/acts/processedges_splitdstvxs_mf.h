value_t PROCESS_SPL_processfunc(value_t udata, value_t edgew, unsigned int GraphAlgo);

void PROCESS_SPL_debug(unsigned int debugid,
	unsigned int i, value_t E[VECTOR2_SIZE], bool_type ens[VECTOR2_SIZE], unsigned int mask[VECTOR2_SIZE],
		value_t udataset[MAX_NUM_UNIQ_EDGES_PER_VEC], value_t maskset[VECTOR2_SIZE], value_t Vset[2][VECTOR2_SIZE], unit1_type VMset[2][VECTOR2_SIZE], vertex_t lvids[VECTOR2_SIZE],
			unsigned int incr[VECTOR2_SIZE], unsigned int lsrcvids[VECTOR2_SIZE], unsigned int ldstvids[VECTOR2_SIZE], value_t res[VECTOR2_SIZE], keyvalue_t mykeyvalue[VECTOR2_SIZE], sweepparams_t sweepparams, globalparams_t globalparams,
				unsigned int lvid_head, unsigned int srcvid_head, travstate_t travstate, unsigned int chunk_size, sliceinfos_t sliceinfos, unsigned int * activeloadcount, unsigned int * inactiveloadcount, unsigned int * debug_numinvalidheads
				);
				
void PROCESS_SPL_getslice(sliceinfo_t slice, int id, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], value_t Vdatas[VECTOR2_SIZE], unit1_type VMdatas[VECTOR2_SIZE], unsigned int depths[64], globalparams_t globalparams);

//
value_t PROCESS_SPL_GETVTXDATA(keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int loc, globalparams_t globalparams);

value_t PROCESS_SPL_GETVTXMASK(unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unsigned int loc, globalparams_t globalparams);

value_t PROCESS_SPL_GETVTXDATA_SLIDED(keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int loc, globalparams_t globalparams);

value_t PROCESS_SPL_GETVTXMASK_SLIDED(unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unsigned int loc, globalparams_t globalparams);

sliceinfos_t PROCESS_SPL_GETVTXSLICES(unsigned int loc, globalparams_t globalparams);

void PROCESS_SPL_GETVTXDATASET(sliceinfo_t slice, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], value_t Vdatas[VECTOR2_SIZE], unit1_type VMdatas[VECTOR2_SIZE], unsigned int depths[64], globalparams_t globalparams);

fetchmessage_t PROCESS_SPL_readandprocess(bool_type enable, uint512_dt * edges, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], 
		batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, sweepparams_t sweepparams, globalparams_t globalparams);	







