void mergeP0_trace(uint512_dt * kvdram, unsigned int reduce_partition, unsigned int k, unsigned int offset_kvs, globalparams_t globalparams);
	
void MERGEP0_print_active_masks(uint512_dt * vdram, globalparams_t globalparams, unsigned int offset_kvs);

unsigned int mergeP0_get_updateblock_workload(unsigned int en, unsigned int reduce_partition, unsigned int * stats_offsets, unsigned int * stats_metadata, globalparams_t globalparams, workload_t xload_kvs[BLOCKRAM_SIZE], unsigned int buffer_offsets[BLOCKRAM_SIZE]);

unsigned int mergeP0_get_upropblock_workload(unsigned int en, unsigned int reduce_partition, uint512_dt * dram, unsigned int num_active_edgeblocks, globalparams_t globalparams, workload_t xload_kvs[BLOCKRAM_SIZE], unsigned int buffer_offsets[BLOCKRAM_SIZE], unsigned int graphiterationid);

unsigned int mergeP0_get_edgeblock_workload(unsigned int en, unsigned int process_partition, unsigned int llp_set, uint512_dt * dram, unsigned int num_active_edgeblocks, globalparams_t globalparamsK, globalparams_t globalparamsE, workload_t xload_kvs[DOUBLE_BLOCKRAM_SIZE], unsigned int graphiterationid);

void MERGEP0_mergeVs(uint512_dt * kvdram, uint512_dt * vdram);

void MERGEP0_broadcastVs(uint512_dt * vdram, uint512_dt * kvdram, unsigned int offset1_kvs, unsigned int offset2_kvs, unsigned int size_kvs);

	
void MERGEP0_broadcastVs1(uint512_dt * vdram, uint512_dt * kvdram0, 
		unsigned int offset1_kvs, unsigned int offset2_kvs, unsigned int size_kvs,
			unsigned int offset1b_kvs, unsigned int offset2b_kvs, unsigned int sizeb_kvs,
				unsigned int cmd, globalposition_t globalposition, globalparams_t globalparamsK, globalparams_t globalparamsV);
	
void MERGEP0_broadcastVs2(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1, 
		unsigned int offset1_kvs, unsigned int offset2_kvs, unsigned int size_kvs,
			unsigned int offset1b_kvs, unsigned int offset2b_kvs, unsigned int sizeb_kvs,
				unsigned int cmd, globalposition_t globalposition, globalparams_t globalparamsK, globalparams_t globalparamsV);
	
void MERGEP0_broadcastVs3(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2, 
		unsigned int offset1_kvs, unsigned int offset2_kvs, unsigned int size_kvs,
			unsigned int offset1b_kvs, unsigned int offset2b_kvs, unsigned int sizeb_kvs,
				unsigned int cmd, globalposition_t globalposition, globalparams_t globalparamsK, globalparams_t globalparamsV);
	
void MERGEP0_broadcastVs4(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3, 
		unsigned int offset1_kvs, unsigned int offset2_kvs, unsigned int size_kvs,
			unsigned int offset1b_kvs, unsigned int offset2b_kvs, unsigned int sizeb_kvs,
				unsigned int cmd, globalposition_t globalposition, globalparams_t globalparamsK, globalparams_t globalparamsV);
	
void MERGEP0_broadcastVs5(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4, 
		unsigned int offset1_kvs, unsigned int offset2_kvs, unsigned int size_kvs,
			unsigned int offset1b_kvs, unsigned int offset2b_kvs, unsigned int sizeb_kvs,
				unsigned int cmd, globalposition_t globalposition, globalparams_t globalparamsK, globalparams_t globalparamsV);
	
void MERGEP0_broadcastVs6(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5, 
		unsigned int offset1_kvs, unsigned int offset2_kvs, unsigned int size_kvs,
			unsigned int offset1b_kvs, unsigned int offset2b_kvs, unsigned int sizeb_kvs,
				unsigned int cmd, globalposition_t globalposition, globalparams_t globalparamsK, globalparams_t globalparamsV);
	
void MERGEP0_broadcastVs7(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6, 
		unsigned int offset1_kvs, unsigned int offset2_kvs, unsigned int size_kvs,
			unsigned int offset1b_kvs, unsigned int offset2b_kvs, unsigned int sizeb_kvs,
				unsigned int cmd, globalposition_t globalposition, globalparams_t globalparamsK, globalparams_t globalparamsV);
	
void MERGEP0_broadcastVs8(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7, 
		unsigned int offset1_kvs, unsigned int offset2_kvs, unsigned int size_kvs,
			unsigned int offset1b_kvs, unsigned int offset2b_kvs, unsigned int sizeb_kvs,
				unsigned int cmd, globalposition_t globalposition, globalparams_t globalparamsK, globalparams_t globalparamsV);
	
void MERGEP0_broadcastVs9(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8, 
		unsigned int offset1_kvs, unsigned int offset2_kvs, unsigned int size_kvs,
			unsigned int offset1b_kvs, unsigned int offset2b_kvs, unsigned int sizeb_kvs,
				unsigned int cmd, globalposition_t globalposition, globalparams_t globalparamsK, globalparams_t globalparamsV);
	
void MERGEP0_broadcastVs10(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9, 
		unsigned int offset1_kvs, unsigned int offset2_kvs, unsigned int size_kvs,
			unsigned int offset1b_kvs, unsigned int offset2b_kvs, unsigned int sizeb_kvs,
				unsigned int cmd, globalposition_t globalposition, globalparams_t globalparamsK, globalparams_t globalparamsV);
	
void MERGEP0_broadcastVs11(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10, 
		unsigned int offset1_kvs, unsigned int offset2_kvs, unsigned int size_kvs,
			unsigned int offset1b_kvs, unsigned int offset2b_kvs, unsigned int sizeb_kvs,
				unsigned int cmd, globalposition_t globalposition, globalparams_t globalparamsK, globalparams_t globalparamsV);
	
void MERGEP0_broadcastVs12(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11, 
		unsigned int offset1_kvs, unsigned int offset2_kvs, unsigned int size_kvs,
			unsigned int offset1b_kvs, unsigned int offset2b_kvs, unsigned int sizeb_kvs,
				unsigned int cmd, globalposition_t globalposition, globalparams_t globalparamsK, globalparams_t globalparamsV);

unsigned int MERGEP0_copy(unsigned int idx, unsigned int reduce_partition, uint512_dt * vdramSRC, uint512_dt * vdramDST1, uint512_dt * vdramDST2, unsigned int stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int voffset_kvs, unsigned int firstinstance_id, unsigned int num_compute_units, 
	pmask_dt vpartition_stats___[BLOCKRAM_CURRPMASK_SIZE], wide_word_bits_3t edgeblock_stats[MAX_RED_SRAMSZ],
	globalparams_t globalparamsSRC, globalparams_t globalparamsDST1, globalparams_t globalparamsDST2);

void MERGEP0_exchange(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC);

void TOPP0_topkernelS(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC);







