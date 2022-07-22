void MERGEP0_print_active_masks(uint512_dt * vdram, globalparams_t globalparams, unsigned int offset_kvs);

void MERGEP0_mergeVs(uint512_dt * kvdram, uint512_dt * vdram);

unsigned int MERGEP0_copy2(unsigned int idx, uint512_dt * vdramSRC, uint512_dt * vdramDST1, uint512_dt * vdramDST2, uint512_dt * vdramSTATS, unsigned int voffset_kvs, unsigned int firstinstance_id, unsigned int num_compute_units, 
	pmask_dt vpartition_stats___[BLOCKRAM_CURRPMASK_SIZE], wide_word_bits_3t edgeblock_stats[MAX_RED_SRAMSZ], unsigned int totalnum_actvedgeblocks[1],
	globalparams_t globalparamsSRC, globalparams_t globalparamsDST1, globalparams_t globalparamsDST2);

void MERGEP0_exchange(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC);

void TOPP0_topkernelS(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC);







