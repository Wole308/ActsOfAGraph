// primitives
unsigned int MEMCAP0_READVDATA(keyvalue_vbuffer_t wideword);
unsigned int MEMCAP0_READVMASK(keyvalue_vbuffer_t wideword);
unsigned int MEMCAP0_READVDATA2(keyvalue_vbuffer_t wideword, unsigned int offsetof_vdata);
unsigned int MEMCAP0_READVMASK2(keyvalue_vbuffer_t wideword, unsigned int offsetof_vmask);

void MEMCAP0_WRITEVDATA(keyvalue_vbuffer_t * wideword, value_t vdata);
void MEMCAP0_WRITEVMASK(keyvalue_vbuffer_t * wideword, unit1_type vmask);
void MEMCAP0_WRITEVDATA2(keyvalue_vbuffer_t * wideword, value_t vdata, unsigned int offsetof_vdata);
void MEMCAP0_WRITEVMASK2(keyvalue_vbuffer_t * wideword, unit1_type vmask, unsigned int offsetof_vmask);

unsigned int MEMCAP0_READEDIR(unsigned int wideword);

// non-primitives
vmdata_t MEMCAP0_READFROMBUFFER_VDATAWITHVMASK(keyvalue_vbuffer_t bits_vector);
void MEMCAP0_WRITETOBUFFER_VDATAWITHVMASK(unsigned int index, keyvalue_vbuffer_t buffer[MAX_BLOCKRAM_VDESTDATA_SIZE], batch_type bufferoffset_kvs, keyvalue_vbuffer_t bits_vector, value_t vdata, unit1_type vmask);
void MEMCAP0_WRITETODRAM_VDATAWITHVMASK(unsigned int index, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t bits_vector, value_t vdata, unit1_type vmask);

vmdata_t MEMCAP0_READFROMBUFFER_VDATAWITHVMASK2(unsigned int index, keyvalue_vbuffer_t bits_vector);
void MEMCAP0_WRITETOBUFFER_VDATAWITHVMASK2(unsigned int index, keyvalue_vbuffer_t buffer[MAX_BLOCKRAM_VDESTDATA_SIZE], batch_type bufferoffset_kvs, keyvalue_vbuffer_t bits_vector, value_t vdata, unit1_type vmask);
void MEMCAP0_WRITETODRAM_VDATAWITHVMASK2(unsigned int index, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t bits_vector, value_t vdata, unit1_type vmask);
