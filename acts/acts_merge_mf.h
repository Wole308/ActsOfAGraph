void MERGEP0_print_active_masks(uint512_dt * vdram, globalparams_t globalparams, unsigned int offset_kvs);

void MERGEP0_mergeVs(uint512_dt * kvdram, uint512_dt * vdram);

unsigned int MERGEP0_copy(uint512_dt * vdramSRC, uint512_dt * vdramDST1, uint512_dt * vdramDST2, unsigned int voffset_kvs, unsigned int num_compute_units, 
		globalparams_t globalparamsSRC, globalparams_t globalparamsDST1, globalparams_t globalparamsDST2);

void MERGEP0_exchange(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC);

void TOPP0_topkernelS(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC);







