unsigned int MERGEP0_actvpstatsoffset(globalparams_t globalparams);

void MERGEP0_print_active_masks(uint512_dt * vdram, globalparams_t globalparams, unsigned int offset_kvs);

void MERGEP0_mergeVs(uint512_dt * kvdram, uint512_dt * vdram);

void MERGEP0_exchange(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC, uint512_dt * mdram);

void TOPP0_topkernelS(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC, uint512_dt * mdram);







