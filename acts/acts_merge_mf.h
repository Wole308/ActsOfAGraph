void mergeP0_trace(uint512_dt * kvdram, unsigned int reduce_partition, unsigned int k, unsigned int offset_kvs, globalparams_t globalparams);
	
void MERGEP0_print_active_masks(uint512_dt * vdram, globalparams_t globalparams, unsigned int offset_kvs);

unsigned int MERGEP0_mergeVs(uint512_dt * kvdram, uint512_dt * vdram);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
unsigned int MERGEP0__broadcastACTVVs(uint512_dt * vdram, uint512_dt * kvdram0, unsigned int source_partition, globalparams_t globalparamsK, globalparams_t globalparamsV);
	
unsigned int MERGEP0__broadcastACTVVs(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1, unsigned int source_partition, globalparams_t globalparamsK, globalparams_t globalparamsV);
	
unsigned int MERGEP0__broadcastACTVVs(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2, unsigned int source_partition, globalparams_t globalparamsK, globalparams_t globalparamsV);
	
unsigned int MERGEP0__broadcastACTVVs(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3, unsigned int source_partition, globalparams_t globalparamsK, globalparams_t globalparamsV);
	
unsigned int MERGEP0__broadcastACTVVs(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4, unsigned int source_partition, globalparams_t globalparamsK, globalparams_t globalparamsV);
	
unsigned int MERGEP0__broadcastACTVVs(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5, unsigned int source_partition, globalparams_t globalparamsK, globalparams_t globalparamsV);
	
unsigned int MERGEP0__broadcastACTVVs(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6, unsigned int source_partition, globalparams_t globalparamsK, globalparams_t globalparamsV);
	
unsigned int MERGEP0__broadcastACTVVs(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7, unsigned int source_partition, globalparams_t globalparamsK, globalparams_t globalparamsV);
	
unsigned int MERGEP0__broadcastACTVVs(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8, unsigned int source_partition, globalparams_t globalparamsK, globalparams_t globalparamsV);
	
unsigned int MERGEP0__broadcastACTVVs(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9, unsigned int source_partition, globalparams_t globalparamsK, globalparams_t globalparamsV);
	
unsigned int MERGEP0__broadcastACTVVs(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10, unsigned int source_partition, globalparams_t globalparamsK, globalparams_t globalparamsV);
	
unsigned int MERGEP0__broadcastACTVVs(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11, unsigned int source_partition, globalparams_t globalparamsK, globalparams_t globalparamsV);

unsigned int MERGEP0_mergeACTVVs(uint512_dt * kvdram, uint512_dt * vdram);

unsigned int MERGEP0_copyACTVVs(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC, unsigned int begin_computeunitid, unsigned int num_compute_units);

unsigned int MERGEP0_exchangeACTVVs(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MERGEP0_broadcastVs(uint512_dt * vdram, uint512_dt * kvdram, unsigned int offset1_kvs, unsigned int offset2_kvs, unsigned int size_kvs);

	
void MERGEP0_broadcastVs1(uint512_dt * vdram, uint512_dt * kvdram0, unsigned int offsetSRC_kvs, unsigned int offsetDST_kvs, unsigned int size_kvs);
	
void MERGEP0_broadcastVs2(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1, unsigned int offsetSRC_kvs, unsigned int offsetDST_kvs, unsigned int size_kvs);
	
void MERGEP0_broadcastVs3(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2, unsigned int offsetSRC_kvs, unsigned int offsetDST_kvs, unsigned int size_kvs);
	
void MERGEP0_broadcastVs4(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3, unsigned int offsetSRC_kvs, unsigned int offsetDST_kvs, unsigned int size_kvs);
	
void MERGEP0_broadcastVs5(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4, unsigned int offsetSRC_kvs, unsigned int offsetDST_kvs, unsigned int size_kvs);
	
void MERGEP0_broadcastVs6(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5, unsigned int offsetSRC_kvs, unsigned int offsetDST_kvs, unsigned int size_kvs);
	
void MERGEP0_broadcastVs7(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6, unsigned int offsetSRC_kvs, unsigned int offsetDST_kvs, unsigned int size_kvs);
	
void MERGEP0_broadcastVs8(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7, unsigned int offsetSRC_kvs, unsigned int offsetDST_kvs, unsigned int size_kvs);
	
void MERGEP0_broadcastVs9(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8, unsigned int offsetSRC_kvs, unsigned int offsetDST_kvs, unsigned int size_kvs);
	
void MERGEP0_broadcastVs10(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9, unsigned int offsetSRC_kvs, unsigned int offsetDST_kvs, unsigned int size_kvs);
	
void MERGEP0_broadcastVs11(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10, unsigned int offsetSRC_kvs, unsigned int offsetDST_kvs, unsigned int size_kvs);
	
void MERGEP0_broadcastVs12(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11, unsigned int offsetSRC_kvs, unsigned int offsetDST_kvs, unsigned int size_kvs);

unsigned int MERGEP0_copy(unsigned int idx, unsigned int reduce_partition, uint512_dt * vdramSRC, uint512_dt * vdramDST1, uint512_dt * vdramDST2, uint512_dt * vdramREF, unsigned int stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int voffset_kvs, unsigned int firstinstance_id, unsigned int num_compute_units, 
	pmask_dt vpartition_stats___[BLOCKRAM_CURRPMASK_SIZE], wide_word_bits_3t edgeblock_stats[MAX_RED_SRAMSZ],
	globalparams_t globalparamsSRC, globalparams_t globalparamsDST1, globalparams_t globalparamsDST2);

void MERGEP0_exchange(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC);

void TOPP0_topkernelS(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC);







