// util 
unsigned int MEMCA_READVDATA0(keyvalue_vbuffer_t wideword);
unsigned int MEMCA_READVDATA1(keyvalue_vbuffer_t wideword);
unsigned int MEMCA_READVMASK0(keyvalue_vbuffer_t wideword);
unsigned int MEMCA_READVMASK1(keyvalue_vbuffer_t wideword);
unsigned int MEMCA_READVDATA0ANDVMASK0(keyvalue_vbuffer_t wideword);
unsigned int MEMCA_READVDATA1ANDVMASK1(keyvalue_vbuffer_t wideword);

void MEMCA_WRITEVDATA0(keyvalue_vbuffer_t * wideword, value_t vdata);
void MEMCA_WRITEVDATA1(keyvalue_vbuffer_t * wideword, value_t vdata);
void MEMCA_WRITEVMASK0(keyvalue_vbuffer_t * wideword, unit1_type vmask);
void MEMCA_WRITEVMASK1(keyvalue_vbuffer_t * wideword, unit1_type vmask);

keyvalue_vbuffer_t MEMCA_CREATEVBUFFERSTRUCT(value_t data0, value_t data1);

tuple_t MEMCA_READVDATA0WITHVMASK0(keyvalue_vbuffer_t wideword);
tuple_t MEMCA_READVDATA1WITHVMASK1(keyvalue_vbuffer_t wideword);

void MEMCA_WRITEVDATA0WITHVMASK0(keyvalue_vbuffer_t * wideword, value_t vdata, unit1_type vmask);
void MEMCA_WRITEVDATA1WITHVMASK1(keyvalue_vbuffer_t * wideword, value_t vdata, unit1_type vmask);

// vdatas
void MEMCA_WRITETOBUFFER_VDATA(unsigned int v, unsigned int index, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], value_t vdata, batch_type bufferoffset_kvs);

void MEMCA_WRITETOBUFFER_VDATA(unsigned int index, keyvalue_vbuffer_t buffer[BLOCKRAM_SIZE], value_t vdata, batch_type bufferoffset_kvs);

void MEMCA_WRITETOBUFFER_VDATAS(unsigned int index, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], value_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);

void MEMCA_WRITETOBUFFERWITHDEPTHS_VDATAS(unsigned int indexes[VDATA_PACKINGSIZE], keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], value_t vdatas[VDATA_PACKINGSIZE], batch_type bufferoffset_kvs);			

value_t MEMCA_READFROMBUFFER_VDATA(unsigned int index, keyvalue_vbuffer_t buffer[BLOCKRAM_SIZE], batch_type bufferoffset_kvs);

void MEMCA_READFROMBUFFER_VDATAS(unsigned int index, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], value_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);

// vmasks
unit1_type MEMCA_READFROMBUFFER_VMASK(unsigned int index, unit1_type vmaskBITS[DOUBLE_BLOCKRAM_SIZE], batch_type bufferoffset_kvs);

void MEMCA_READFROMBUFFER_VMASKS(unsigned int index, unit1_type vmaskBITS[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE], unit1_type vmdatas[VMASK_PACKINGSIZE], batch_type bufferoffset_kvs);
 
void MEMCA_WRITETOBUFFER_VMASKS(unsigned int index, unit1_type vmaskBITS[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE], unit1_type vmdatas[VMASK_PACKINGSIZE], batch_type bufferoffset_kvs);
 
void MEMCA_WRITETOBUFFER_VMASKS1_ANDREPLICATE(unsigned int index, unit1_type vmaskBITS0[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE], unit1_type vmdatas[VMASK_PACKINGSIZE], batch_type bufferoffset_kvs);
void MEMCA_WRITETOBUFFER_VMASKS2_ANDREPLICATE(unsigned int index, unit1_type vmaskBITS0[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS1[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE], unit1_type vmdatas[VMASK_PACKINGSIZE], batch_type bufferoffset_kvs);
void MEMCA_WRITETOBUFFER_VMASKS3_ANDREPLICATE(unsigned int index, unit1_type vmaskBITS0[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS1[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS2[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE], unit1_type vmdatas[VMASK_PACKINGSIZE], batch_type bufferoffset_kvs);
void MEMCA_WRITETOBUFFER_VMASKS4_ANDREPLICATE(unsigned int index, unit1_type vmaskBITS0[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS1[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS2[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS3[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE], unit1_type vmdatas[VMASK_PACKINGSIZE], batch_type bufferoffset_kvs);
void MEMCA_WRITETOBUFFER_VMASKS5_ANDREPLICATE(unsigned int index, unit1_type vmaskBITS0[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS1[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS2[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS3[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS4[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE], unit1_type vmdatas[VMASK_PACKINGSIZE], batch_type bufferoffset_kvs);
void MEMCA_WRITETOBUFFER_VMASKS6_ANDREPLICATE(unsigned int index, unit1_type vmaskBITS0[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS1[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS2[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS3[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS4[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS5[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE], unit1_type vmdatas[VMASK_PACKINGSIZE], batch_type bufferoffset_kvs);
void MEMCA_WRITETOBUFFER_VMASKS7_ANDREPLICATE(unsigned int index, unit1_type vmaskBITS0[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS1[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS2[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS3[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS4[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS5[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS6[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE], unit1_type vmdatas[VMASK_PACKINGSIZE], batch_type bufferoffset_kvs);
void MEMCA_WRITETOBUFFER_VMASKS8_ANDREPLICATE(unsigned int index, unit1_type vmaskBITS0[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS1[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS2[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS3[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS4[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS5[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS6[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS7[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE], unit1_type vmdatas[VMASK_PACKINGSIZE], batch_type bufferoffset_kvs);
void MEMCA_WRITETOBUFFER_VMASKS9_ANDREPLICATE(unsigned int index, unit1_type vmaskBITS0[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS1[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS2[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS3[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS4[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS5[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS6[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS7[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS8[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE], unit1_type vmdatas[VMASK_PACKINGSIZE], batch_type bufferoffset_kvs);
void MEMCA_WRITETOBUFFER_VMASKS10_ANDREPLICATE(unsigned int index, unit1_type vmaskBITS0[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS1[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS2[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS3[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS4[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS5[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS6[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS7[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS8[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS9[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE], unit1_type vmdatas[VMASK_PACKINGSIZE], batch_type bufferoffset_kvs);
void MEMCA_WRITETOBUFFER_VMASKS11_ANDREPLICATE(unsigned int index, unit1_type vmaskBITS0[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS1[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS2[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS3[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS4[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS5[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS6[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS7[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS8[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS9[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS10[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE], unit1_type vmdatas[VMASK_PACKINGSIZE], batch_type bufferoffset_kvs);
void MEMCA_WRITETOBUFFER_VMASKS12_ANDREPLICATE(unsigned int index, unit1_type vmaskBITS0[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS1[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS2[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS3[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS4[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS5[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS6[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS7[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS8[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS9[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS10[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS11[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE], unit1_type vmdatas[VMASK_PACKINGSIZE], batch_type bufferoffset_kvs);
 
void MEMCA_WRITETOBUFFER_VMASKS_WITHDEPTHS(unsigned int indexes[VDATA_PACKINGSIZE], unit1_type vmaskBITS[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE], unit1_type vmdatas[VMASK_PACKINGSIZE], batch_type bufferoffset_kvs);

// vdatas & vmasks 
void MEMCA_READFROMBUFFER_VDATASWITHVMASKS(unsigned int index, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], value_t vdatas[VECTOR2_SIZE], unit1_type vmdatas[VMASK_PACKINGSIZE], batch_type bufferoffset_kvs);

void MEMCA_READFROMBUFFER_VDATASANDVMASKS(unsigned int index, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], value_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);

void MEMCA_WRITETOBUFFER_VDATAWITHVMASK(unsigned int index, keyvalue_vbuffer_t buffer[BLOCKRAM_SIZE], value_t vdata, unit1_type vmdata, batch_type bufferoffset_kvs);

void MEMCA_WRITETOBUFFER_VDATASANDVMASKS(unsigned int index, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t datas[VECTOR2_SIZE], batch_type bufferoffset_kvs);

void MEMCA_WRITETOBUFFER_VDATASANDVMASKS1_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCA_WRITETOBUFFER_VDATASANDVMASKS2_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCA_WRITETOBUFFER_VDATASANDVMASKS3_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCA_WRITETOBUFFER_VDATASANDVMASKS4_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCA_WRITETOBUFFER_VDATASANDVMASKS5_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCA_WRITETOBUFFER_VDATASANDVMASKS6_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCA_WRITETOBUFFER_VDATASANDVMASKS7_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCA_WRITETOBUFFER_VDATASANDVMASKS8_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCA_WRITETOBUFFER_VDATASANDVMASKS9_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCA_WRITETOBUFFER_VDATASANDVMASKS10_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCA_WRITETOBUFFER_VDATASANDVMASKS11_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer10[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);
void MEMCA_WRITETOBUFFER_VDATASANDVMASKS12_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer10[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer11[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs);

void MEMCA_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS(unsigned int indexes[VDATA_PACKINGSIZE], keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VDATA_PACKINGSIZE], batch_type bufferoffset_kvs);

void MEMCA_READFROMKVDRAM_VDATASANDVMASKS(unsigned int index, uint512_dt * kvdram, value_t vdatas[VECTOR2_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs);

void MEMCA_WRITETOKVDRAM_VDATASANDVMASKS(unsigned int index, uint512_dt * kvdram, value_t vdatas[VECTOR2_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs);

