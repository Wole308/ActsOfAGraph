void TOPP0_U_processit(uint512_dt * edges0, uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], unit1_type vmaskREAD[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE], unit1_type vmaskWRITE[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE], unit1_type vmask_subp[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition,
		unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID);							

void TOPP0_U_dispatch(bool_type en_process, bool_type en_partition, bool_type en_reduce, uint512_dt * edges0, uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], unit1_type vmaskREAD[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE], unit1_type vmaskWRITE[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE], unit1_type vmask_subp[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS],
		batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition,
			unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID);

void TOPP0_U_dispatch_reduce(uint512_dt * edges0, uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], unit1_type vmaskREAD[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE], unit1_type vmaskWRITE[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE], unit1_type vmask_subp[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK,	
					unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID);
					
#ifdef CONFIG_ENABLECLASS_TOP_USRCV_UDSTV
void TOPP0_U_topkernelproc_embedded(unsigned int globalid, unsigned int localid, unsigned int en_process, unsigned int en_partition, unsigned int en_reduce, uint512_dt * edges0, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], unit1_type vmask_subp[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE], unit1_type vmaskREAD[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE], unit1_type vmaskWRITE[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE], keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], globalposition_t globalposition);
	
// top
void TOPP0_U_topkernelP1(
uint512_dt * edges00,	uint512_dt * kvdram0,
	uint512_dt * vdram
	);
void TOPP0_U_topkernelP2(
uint512_dt * edges00,	uint512_dt * kvdram0,
uint512_dt * edges10,	uint512_dt * kvdram1,
	uint512_dt * vdram
	);
void TOPP0_U_topkernelP3(
uint512_dt * edges00,	uint512_dt * kvdram0,
uint512_dt * edges10,	uint512_dt * kvdram1,
uint512_dt * edges20,	uint512_dt * kvdram2,
	uint512_dt * vdram
	);
void TOPP0_U_topkernelP4(
uint512_dt * edges00,	uint512_dt * kvdram0,
uint512_dt * edges10,	uint512_dt * kvdram1,
uint512_dt * edges20,	uint512_dt * kvdram2,
uint512_dt * edges30,	uint512_dt * kvdram3,
	uint512_dt * vdram
	);
void TOPP0_U_topkernelP5(
uint512_dt * edges00,	uint512_dt * kvdram0,
uint512_dt * edges10,	uint512_dt * kvdram1,
uint512_dt * edges20,	uint512_dt * kvdram2,
uint512_dt * edges30,	uint512_dt * kvdram3,
uint512_dt * edges40,	uint512_dt * kvdram4,
	uint512_dt * vdram
	);
void TOPP0_U_topkernelP6(
uint512_dt * edges00,	uint512_dt * kvdram0,
uint512_dt * edges10,	uint512_dt * kvdram1,
uint512_dt * edges20,	uint512_dt * kvdram2,
uint512_dt * edges30,	uint512_dt * kvdram3,
uint512_dt * edges40,	uint512_dt * kvdram4,
uint512_dt * edges50,	uint512_dt * kvdram5,
	uint512_dt * vdram
	);
void TOPP0_U_topkernelP7(
uint512_dt * edges00,	uint512_dt * kvdram0,
uint512_dt * edges10,	uint512_dt * kvdram1,
uint512_dt * edges20,	uint512_dt * kvdram2,
uint512_dt * edges30,	uint512_dt * kvdram3,
uint512_dt * edges40,	uint512_dt * kvdram4,
uint512_dt * edges50,	uint512_dt * kvdram5,
uint512_dt * edges60,	uint512_dt * kvdram6,
	uint512_dt * vdram
	);
void TOPP0_U_topkernelP8(
uint512_dt * edges00,	uint512_dt * kvdram0,
uint512_dt * edges10,	uint512_dt * kvdram1,
uint512_dt * edges20,	uint512_dt * kvdram2,
uint512_dt * edges30,	uint512_dt * kvdram3,
uint512_dt * edges40,	uint512_dt * kvdram4,
uint512_dt * edges50,	uint512_dt * kvdram5,
uint512_dt * edges60,	uint512_dt * kvdram6,
uint512_dt * edges70,	uint512_dt * kvdram7,
	uint512_dt * vdram
	);
void TOPP0_U_topkernelP9(
uint512_dt * edges00,	uint512_dt * kvdram0,
uint512_dt * edges10,	uint512_dt * kvdram1,
uint512_dt * edges20,	uint512_dt * kvdram2,
uint512_dt * edges30,	uint512_dt * kvdram3,
uint512_dt * edges40,	uint512_dt * kvdram4,
uint512_dt * edges50,	uint512_dt * kvdram5,
uint512_dt * edges60,	uint512_dt * kvdram6,
uint512_dt * edges70,	uint512_dt * kvdram7,
uint512_dt * edges80,	uint512_dt * kvdram8,
	uint512_dt * vdram
	);
void TOPP0_U_topkernelP10(
uint512_dt * edges00,	uint512_dt * kvdram0,
uint512_dt * edges10,	uint512_dt * kvdram1,
uint512_dt * edges20,	uint512_dt * kvdram2,
uint512_dt * edges30,	uint512_dt * kvdram3,
uint512_dt * edges40,	uint512_dt * kvdram4,
uint512_dt * edges50,	uint512_dt * kvdram5,
uint512_dt * edges60,	uint512_dt * kvdram6,
uint512_dt * edges70,	uint512_dt * kvdram7,
uint512_dt * edges80,	uint512_dt * kvdram8,
uint512_dt * edges90,	uint512_dt * kvdram9,
	uint512_dt * vdram
	);
void TOPP0_U_topkernelP11(
uint512_dt * edges00,	uint512_dt * kvdram0,
uint512_dt * edges10,	uint512_dt * kvdram1,
uint512_dt * edges20,	uint512_dt * kvdram2,
uint512_dt * edges30,	uint512_dt * kvdram3,
uint512_dt * edges40,	uint512_dt * kvdram4,
uint512_dt * edges50,	uint512_dt * kvdram5,
uint512_dt * edges60,	uint512_dt * kvdram6,
uint512_dt * edges70,	uint512_dt * kvdram7,
uint512_dt * edges80,	uint512_dt * kvdram8,
uint512_dt * edges90,	uint512_dt * kvdram9,
uint512_dt * edges100,	uint512_dt * kvdram10,
	uint512_dt * vdram
	);
void TOPP0_U_topkernelP12(
uint512_dt * edges00,	uint512_dt * kvdram0,
uint512_dt * edges10,	uint512_dt * kvdram1,
uint512_dt * edges20,	uint512_dt * kvdram2,
uint512_dt * edges30,	uint512_dt * kvdram3,
uint512_dt * edges40,	uint512_dt * kvdram4,
uint512_dt * edges50,	uint512_dt * kvdram5,
uint512_dt * edges60,	uint512_dt * kvdram6,
uint512_dt * edges70,	uint512_dt * kvdram7,
uint512_dt * edges80,	uint512_dt * kvdram8,
uint512_dt * edges90,	uint512_dt * kvdram9,
uint512_dt * edges100,	uint512_dt * kvdram10,
uint512_dt * edges110,	uint512_dt * kvdram11,
	uint512_dt * vdram
	);
#endif 








