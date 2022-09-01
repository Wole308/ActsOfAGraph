void ACTSP0_actit(bool_type enable, unsigned int mode,
		uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], workload_t edgeptrload_kvs[EDGESTATSRAM_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE],
			keyvalue_t globalcapsule[BLOCKRAM_GLOBALSTATS_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition, sweepparams_t sweepparams, batch_type sourcebaseaddr_kvs, batch_type destbaseaddr_kvs,
				bool_type resetenv, bool_type flush, unsigned int edgebankID, collection_t collections[COLLECTIONS_BUFFERSZ]);

	
						