void TOPP0_processit(uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], workload_t edgestats[EDGESTATSRAM_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], keyvalue_t globalcapsule[BLOCKRAM_GLOBALSTATS_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition,
		unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID, collection_t collections[COLLECTIONS_BUFFERSZ]);							

void TOPP0_partitionit(uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], workload_t edgestats[EDGESTATSRAM_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK, unsigned int edgebankID, collection_t collections[COLLECTIONS_BUFFERSZ]);
	
void TOPP0_reduceit(uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], workload_t edgestats[EDGESTATSRAM_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], keyvalue_t globalcapsule[BLOCKRAM_GLOBALSTATS_SIZE], batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparamsE, globalparams_t globalparamsK, unsigned int edgebankID, collection_t collections[COLLECTIONS_BUFFERSZ]);					

void TOPP0_dispatch(bool_type en_process, bool_type en_partition, bool_type en_reduce,  uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], workload_t edgestats[EDGESTATSRAM_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], keyvalue_t globalcapsule[BLOCKRAM_GLOBALSTATS_SIZE],
		batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition,
			unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID, collection_t collections[COLLECTIONS_BUFFERSZ]);

void TOPP0_dispatch_reduce(uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], workload_t edgestats[EDGESTATSRAM_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], keyvalue_t globalcapsule[BLOCKRAM_GLOBALSTATS_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK, globalparams_t globalparamsV, globalposition_t globalposition,	
					unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID, collection_t collections[COLLECTIONS_BUFFERSZ]);
					
void TOPP0_topkernelproc_embedded(unsigned int GraphIter, unsigned int globalid, unsigned int localid, unsigned int en_process, unsigned int en_partition, unsigned int en_reduce,  uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], workload_t edgestats[EDGESTATSRAM_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], keyvalue_t globalcapsule[BLOCKRAM_GLOBALSTATS_SIZE], globalposition_t globalposition, globalparams_t globalparamsV, collection_t collections[COLLECTIONS_BUFFERSZ]);									
	
// top
void TOPP0_topkernelP1(
	uint512_dt * kvdram0,
	uint512_dt * vdram
	);
void TOPP0_topkernelP2(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * vdram
	);
void TOPP0_topkernelP3(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * vdram
	);
void TOPP0_topkernelP4(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * vdram
	);
void TOPP0_topkernelP5(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * kvdram4,
	uint512_dt * vdram
	);
void TOPP0_topkernelP6(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * kvdram4,
	uint512_dt * kvdram5,
	uint512_dt * vdram
	);
void TOPP0_topkernelP7(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * kvdram4,
	uint512_dt * kvdram5,
	uint512_dt * kvdram6,
	uint512_dt * vdram
	);
void TOPP0_topkernelP8(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * kvdram4,
	uint512_dt * kvdram5,
	uint512_dt * kvdram6,
	uint512_dt * kvdram7,
	uint512_dt * vdram
	);
void TOPP0_topkernelP9(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * kvdram4,
	uint512_dt * kvdram5,
	uint512_dt * kvdram6,
	uint512_dt * kvdram7,
	uint512_dt * kvdram8,
	uint512_dt * vdram
	);
void TOPP0_topkernelP10(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * kvdram4,
	uint512_dt * kvdram5,
	uint512_dt * kvdram6,
	uint512_dt * kvdram7,
	uint512_dt * kvdram8,
	uint512_dt * kvdram9,
	uint512_dt * vdram
	);
void TOPP0_topkernelP11(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * kvdram4,
	uint512_dt * kvdram5,
	uint512_dt * kvdram6,
	uint512_dt * kvdram7,
	uint512_dt * kvdram8,
	uint512_dt * kvdram9,
	uint512_dt * kvdram10,
	uint512_dt * vdram
	);
void TOPP0_topkernelP12(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * kvdram4,
	uint512_dt * kvdram5,
	uint512_dt * kvdram6,
	uint512_dt * kvdram7,
	uint512_dt * kvdram8,
	uint512_dt * kvdram9,
	uint512_dt * kvdram10,
	uint512_dt * kvdram11,
	uint512_dt * vdram
	);








