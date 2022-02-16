// primitives
unsigned int acts_all::MEMCAP0_READVDATA0(keyvalue_vbuffer_t wideword){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	return UTILP0_READBITSFROM_UINTV(wideword, 0, SIZEOF_VDATA0);
	#else 
	return wideword.vmdata0.vdata;
	#endif
}
unsigned int acts_all::MEMCAP0_READVDATA1(keyvalue_vbuffer_t wideword){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	return UTILP0_READBITSFROM_UINTV(wideword, SIZEOF_VDATAKEY, SIZEOF_VDATA1);
	#else 
	return wideword.vmdata1.vdata;
	#endif
}
unsigned int acts_all::MEMCAP0_READVMASK0(keyvalue_vbuffer_t wideword){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	return UTILP0_READBITSFROM_UINTV(wideword, SIZEOF_VDATA0, SIZEOF_VMASK0);
	#else 
	return wideword.vmdata0.vmask;
	#endif
}
unsigned int acts_all::MEMCAP0_READVMASK1(keyvalue_vbuffer_t wideword){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	return UTILP0_READBITSFROM_UINTV(wideword, SIZEOF_VDATAKEY + SIZEOF_VDATA1, SIZEOF_VMASK1);
	#else 
	return wideword.vmdata1.vmask;
	#endif
}
vmdata_t acts_all::MEMCAP0_READVDATA0ANDVMASK0(keyvalue_vbuffer_t wideword){
	#pragma HLS INLINE
	vmdata_t vmdata;
	#ifdef _WIDEWORD
	vmdata.vdata = UTILP0_READBITSFROM_UINTV(wideword, 0, SIZEOF_VDATA0);
	vmdata.vmask = UTILP0_READBITSFROM_UINTV(wideword, SIZEOF_VDATA0, SIZEOF_VMASK0);
	#else 
	vmdata.vdata = wideword.vmdata0.vdata;
	vmdata.vmask = wideword.vmdata0.vmask;
	#endif
	return vmdata;
}
vmdata_t acts_all::MEMCAP0_READVDATA1ANDVMASK1(keyvalue_vbuffer_t wideword){
	#pragma HLS INLINE
	vmdata_t vmdata;	
	#ifdef _WIDEWORD
	vmdata.vmask = UTILP0_READBITSFROM_UINTV(wideword, SIZEOF_VDATAKEY, SIZEOF_VDATA1);
	vmdata.vdata = UTILP0_READBITSFROM_UINTV(wideword, SIZEOF_VDATAKEY + SIZEOF_VDATA1, SIZEOF_VMASK1);
	#else 
	vmdata.vmask = wideword.vmdata1.vmask;
	vmdata.vdata = wideword.vmdata1.vdata;
	#endif
	return vmdata;
}	

void acts_all::MEMCAP0_WRITEVDATA0(keyvalue_vbuffer_t * wideword, value_t vdata){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	UTILP0_WRITEBITSTO_UINTV(wideword, 0, SIZEOF_VDATA0, vdata);
	#else 
	wideword->vmdata0.vdata = vdata;
	#endif 
	return;
}
void acts_all::MEMCAP0_WRITEVDATA1(keyvalue_vbuffer_t * wideword, value_t vdata){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	UTILP0_WRITEBITSTO_UINTV(wideword, SIZEOF_VDATAKEY, SIZEOF_VDATA1, vdata);
	#else 
	wideword->vmdata1.vdata = vdata;
	#endif 
	return;
}
void acts_all::MEMCAP0_WRITEVMASK0(keyvalue_vbuffer_t * wideword, unit1_type vmask){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	UTILP0_WRITEBITSTO_UINTV(wideword, SIZEOF_VDATA0, SIZEOF_VMASK0, vmask);
	#else
	wideword->vmdata0.vmask = vmask;
	#endif 
	return;
}
void acts_all::MEMCAP0_WRITEVMASK1(keyvalue_vbuffer_t * wideword, unit1_type vmask){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	UTILP0_WRITEBITSTO_UINTV(wideword, SIZEOF_VDATAKEY + SIZEOF_VDATA1, SIZEOF_VMASK1, vmask);
	#else 
	wideword->vmdata1.vmask = vmask;
	#endif 
	return;
}

keyvalue_vbuffer_t acts_all::MEMCAP0_CREATEVBUFFERSTRUCT(vmdata_t data0, vmdata_t data1){
	#pragma HLS INLINE
	keyvalue_vbuffer_t _wideword;
	#ifdef _WIDEWORD
	_wideword = 0;
	UTILP0_WRITEBITSTO_UINTV(&_wideword, 0, SIZEOF_VDATA0, data0.vdata);
	UTILP0_WRITEBITSTO_UINTV(&_wideword, SIZEOF_VDATA0, SIZEOF_VMASK0, data0.vmask);
	UTILP0_WRITEBITSTO_UINTV(&_wideword, SIZEOF_VDATAKEY, SIZEOF_VDATA1, data1.vdata);
	UTILP0_WRITEBITSTO_UINTV(&_wideword, SIZEOF_VDATAKEY + SIZEOF_VDATA1, SIZEOF_VMASK1, data1.vmask);
	#else
	_wideword.vmdata0.vmask = data0.vmask; 
	_wideword.vmdata0.vdata = data0.vdata;
	_wideword.vmdata1.vmask = data1.vmask; 
	_wideword.vmdata1.vdata = data1.vdata;
	#endif
	return _wideword;
}

void acts_all::MEMCAP0_READFROMKVDRAM_VDATASANDVMASKS(unsigned int index, uint512_dt * kvdram, vmdata_t vdatas[VECTOR2_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("acts_all::MEMCAP0_READFROMKVDRAM_VDATASANDVMASKS:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	value_t datas[VECTOR2_SIZE];
	#ifdef _WIDEWORD // CRITICAL FIXME.
	datas[0] = kvdram[baseoffset_kvs + offset_kvs + index].range(31, 0); 
	datas[1] = kvdram[baseoffset_kvs + offset_kvs + index].range(63, 32); 
	datas[2] = kvdram[baseoffset_kvs + offset_kvs + index].range(95, 64); 
	datas[3] = kvdram[baseoffset_kvs + offset_kvs + index].range(127, 96); 
	datas[4] = kvdram[baseoffset_kvs + offset_kvs + index].range(159, 128); 
	datas[5] = kvdram[baseoffset_kvs + offset_kvs + index].range(191, 160); 
	datas[6] = kvdram[baseoffset_kvs + offset_kvs + index].range(223, 192); 
	datas[7] = kvdram[baseoffset_kvs + offset_kvs + index].range(255, 224); 
	datas[8] = kvdram[baseoffset_kvs + offset_kvs + index].range(287, 256); 
	datas[9] = kvdram[baseoffset_kvs + offset_kvs + index].range(319, 288); 
	datas[10] = kvdram[baseoffset_kvs + offset_kvs + index].range(351, 320); 
	datas[11] = kvdram[baseoffset_kvs + offset_kvs + index].range(383, 352); 
	datas[12] = kvdram[baseoffset_kvs + offset_kvs + index].range(415, 384); 
	datas[13] = kvdram[baseoffset_kvs + offset_kvs + index].range(447, 416); 
	datas[14] = kvdram[baseoffset_kvs + offset_kvs + index].range(479, 448); 
	datas[15] = kvdram[baseoffset_kvs + offset_kvs + index].range(511, 480); 
	#else 
	datas[0] = kvdram[baseoffset_kvs + offset_kvs + index].data[0].key;
	datas[1] = kvdram[baseoffset_kvs + offset_kvs + index].data[0].value; 
	datas[2] = kvdram[baseoffset_kvs + offset_kvs + index].data[1].key;
	datas[3] = kvdram[baseoffset_kvs + offset_kvs + index].data[1].value; 
	datas[4] = kvdram[baseoffset_kvs + offset_kvs + index].data[2].key;
	datas[5] = kvdram[baseoffset_kvs + offset_kvs + index].data[2].value; 
	datas[6] = kvdram[baseoffset_kvs + offset_kvs + index].data[3].key;
	datas[7] = kvdram[baseoffset_kvs + offset_kvs + index].data[3].value; 
	datas[8] = kvdram[baseoffset_kvs + offset_kvs + index].data[4].key;
	datas[9] = kvdram[baseoffset_kvs + offset_kvs + index].data[4].value; 
	datas[10] = kvdram[baseoffset_kvs + offset_kvs + index].data[5].key;
	datas[11] = kvdram[baseoffset_kvs + offset_kvs + index].data[5].value; 
	datas[12] = kvdram[baseoffset_kvs + offset_kvs + index].data[6].key;
	datas[13] = kvdram[baseoffset_kvs + offset_kvs + index].data[6].value; 
	datas[14] = kvdram[baseoffset_kvs + offset_kvs + index].data[7].key;
	datas[15] = kvdram[baseoffset_kvs + offset_kvs + index].data[7].value; 
	#endif
	
 // CRITICAL FIXME. can be better for _WIDEWORD
	vdatas[0].vdata = UTILP0_READBITSFROM_UINTV(datas[0], 0, SIZEOF_VDATA0); 
	vdatas[0].vmask = UTILP0_READBITSFROM_UINTV(datas[0], SIZEOF_VDATA0, SIZEOF_VMASK0); 	
	// if(vdatas[0].vmask > 0){ cout<<"------------- MEMCAP0_READFROMKVDRAM_VDATASANDVMASKS:: vdatas[0].vmask: "<<vdatas[0].vmask<<endl; }	
 // CRITICAL FIXME. can be better for _WIDEWORD
	vdatas[1].vdata = UTILP0_READBITSFROM_UINTV(datas[1], 0, SIZEOF_VDATA0); 
	vdatas[1].vmask = UTILP0_READBITSFROM_UINTV(datas[1], SIZEOF_VDATA0, SIZEOF_VMASK0); 	
	// if(vdatas[1].vmask > 0){ cout<<"------------- MEMCAP0_READFROMKVDRAM_VDATASANDVMASKS:: vdatas[1].vmask: "<<vdatas[1].vmask<<endl; }	
 // CRITICAL FIXME. can be better for _WIDEWORD
	vdatas[2].vdata = UTILP0_READBITSFROM_UINTV(datas[2], 0, SIZEOF_VDATA0); 
	vdatas[2].vmask = UTILP0_READBITSFROM_UINTV(datas[2], SIZEOF_VDATA0, SIZEOF_VMASK0); 	
	// if(vdatas[2].vmask > 0){ cout<<"------------- MEMCAP0_READFROMKVDRAM_VDATASANDVMASKS:: vdatas[2].vmask: "<<vdatas[2].vmask<<endl; }	
 // CRITICAL FIXME. can be better for _WIDEWORD
	vdatas[3].vdata = UTILP0_READBITSFROM_UINTV(datas[3], 0, SIZEOF_VDATA0); 
	vdatas[3].vmask = UTILP0_READBITSFROM_UINTV(datas[3], SIZEOF_VDATA0, SIZEOF_VMASK0); 	
	// if(vdatas[3].vmask > 0){ cout<<"------------- MEMCAP0_READFROMKVDRAM_VDATASANDVMASKS:: vdatas[3].vmask: "<<vdatas[3].vmask<<endl; }	
 // CRITICAL FIXME. can be better for _WIDEWORD
	vdatas[4].vdata = UTILP0_READBITSFROM_UINTV(datas[4], 0, SIZEOF_VDATA0); 
	vdatas[4].vmask = UTILP0_READBITSFROM_UINTV(datas[4], SIZEOF_VDATA0, SIZEOF_VMASK0); 	
	// if(vdatas[4].vmask > 0){ cout<<"------------- MEMCAP0_READFROMKVDRAM_VDATASANDVMASKS:: vdatas[4].vmask: "<<vdatas[4].vmask<<endl; }	
 // CRITICAL FIXME. can be better for _WIDEWORD
	vdatas[5].vdata = UTILP0_READBITSFROM_UINTV(datas[5], 0, SIZEOF_VDATA0); 
	vdatas[5].vmask = UTILP0_READBITSFROM_UINTV(datas[5], SIZEOF_VDATA0, SIZEOF_VMASK0); 	
	// if(vdatas[5].vmask > 0){ cout<<"------------- MEMCAP0_READFROMKVDRAM_VDATASANDVMASKS:: vdatas[5].vmask: "<<vdatas[5].vmask<<endl; }	
 // CRITICAL FIXME. can be better for _WIDEWORD
	vdatas[6].vdata = UTILP0_READBITSFROM_UINTV(datas[6], 0, SIZEOF_VDATA0); 
	vdatas[6].vmask = UTILP0_READBITSFROM_UINTV(datas[6], SIZEOF_VDATA0, SIZEOF_VMASK0); 	
	// if(vdatas[6].vmask > 0){ cout<<"------------- MEMCAP0_READFROMKVDRAM_VDATASANDVMASKS:: vdatas[6].vmask: "<<vdatas[6].vmask<<endl; }	
 // CRITICAL FIXME. can be better for _WIDEWORD
	vdatas[7].vdata = UTILP0_READBITSFROM_UINTV(datas[7], 0, SIZEOF_VDATA0); 
	vdatas[7].vmask = UTILP0_READBITSFROM_UINTV(datas[7], SIZEOF_VDATA0, SIZEOF_VMASK0); 	
	// if(vdatas[7].vmask > 0){ cout<<"------------- MEMCAP0_READFROMKVDRAM_VDATASANDVMASKS:: vdatas[7].vmask: "<<vdatas[7].vmask<<endl; }	
 // CRITICAL FIXME. can be better for _WIDEWORD
	vdatas[8].vdata = UTILP0_READBITSFROM_UINTV(datas[8], 0, SIZEOF_VDATA0); 
	vdatas[8].vmask = UTILP0_READBITSFROM_UINTV(datas[8], SIZEOF_VDATA0, SIZEOF_VMASK0); 	
	// if(vdatas[8].vmask > 0){ cout<<"------------- MEMCAP0_READFROMKVDRAM_VDATASANDVMASKS:: vdatas[8].vmask: "<<vdatas[8].vmask<<endl; }	
 // CRITICAL FIXME. can be better for _WIDEWORD
	vdatas[9].vdata = UTILP0_READBITSFROM_UINTV(datas[9], 0, SIZEOF_VDATA0); 
	vdatas[9].vmask = UTILP0_READBITSFROM_UINTV(datas[9], SIZEOF_VDATA0, SIZEOF_VMASK0); 	
	// if(vdatas[9].vmask > 0){ cout<<"------------- MEMCAP0_READFROMKVDRAM_VDATASANDVMASKS:: vdatas[9].vmask: "<<vdatas[9].vmask<<endl; }	
 // CRITICAL FIXME. can be better for _WIDEWORD
	vdatas[10].vdata = UTILP0_READBITSFROM_UINTV(datas[10], 0, SIZEOF_VDATA0); 
	vdatas[10].vmask = UTILP0_READBITSFROM_UINTV(datas[10], SIZEOF_VDATA0, SIZEOF_VMASK0); 	
	// if(vdatas[10].vmask > 0){ cout<<"------------- MEMCAP0_READFROMKVDRAM_VDATASANDVMASKS:: vdatas[10].vmask: "<<vdatas[10].vmask<<endl; }	
 // CRITICAL FIXME. can be better for _WIDEWORD
	vdatas[11].vdata = UTILP0_READBITSFROM_UINTV(datas[11], 0, SIZEOF_VDATA0); 
	vdatas[11].vmask = UTILP0_READBITSFROM_UINTV(datas[11], SIZEOF_VDATA0, SIZEOF_VMASK0); 	
	// if(vdatas[11].vmask > 0){ cout<<"------------- MEMCAP0_READFROMKVDRAM_VDATASANDVMASKS:: vdatas[11].vmask: "<<vdatas[11].vmask<<endl; }	
 // CRITICAL FIXME. can be better for _WIDEWORD
	vdatas[12].vdata = UTILP0_READBITSFROM_UINTV(datas[12], 0, SIZEOF_VDATA0); 
	vdatas[12].vmask = UTILP0_READBITSFROM_UINTV(datas[12], SIZEOF_VDATA0, SIZEOF_VMASK0); 	
	// if(vdatas[12].vmask > 0){ cout<<"------------- MEMCAP0_READFROMKVDRAM_VDATASANDVMASKS:: vdatas[12].vmask: "<<vdatas[12].vmask<<endl; }	
 // CRITICAL FIXME. can be better for _WIDEWORD
	vdatas[13].vdata = UTILP0_READBITSFROM_UINTV(datas[13], 0, SIZEOF_VDATA0); 
	vdatas[13].vmask = UTILP0_READBITSFROM_UINTV(datas[13], SIZEOF_VDATA0, SIZEOF_VMASK0); 	
	// if(vdatas[13].vmask > 0){ cout<<"------------- MEMCAP0_READFROMKVDRAM_VDATASANDVMASKS:: vdatas[13].vmask: "<<vdatas[13].vmask<<endl; }	
 // CRITICAL FIXME. can be better for _WIDEWORD
	vdatas[14].vdata = UTILP0_READBITSFROM_UINTV(datas[14], 0, SIZEOF_VDATA0); 
	vdatas[14].vmask = UTILP0_READBITSFROM_UINTV(datas[14], SIZEOF_VDATA0, SIZEOF_VMASK0); 	
	// if(vdatas[14].vmask > 0){ cout<<"------------- MEMCAP0_READFROMKVDRAM_VDATASANDVMASKS:: vdatas[14].vmask: "<<vdatas[14].vmask<<endl; }	
 // CRITICAL FIXME. can be better for _WIDEWORD
	vdatas[15].vdata = UTILP0_READBITSFROM_UINTV(datas[15], 0, SIZEOF_VDATA0); 
	vdatas[15].vmask = UTILP0_READBITSFROM_UINTV(datas[15], SIZEOF_VDATA0, SIZEOF_VMASK0); 	
	// if(vdatas[15].vmask > 0){ cout<<"------------- MEMCAP0_READFROMKVDRAM_VDATASANDVMASKS:: vdatas[15].vmask: "<<vdatas[15].vmask<<endl; }	
	return;
}

void acts_all::MEMCAP0_WRITETOKVDRAM_VDATASANDVMASKS(unsigned int index, uint512_dt * kvdram, vmdata_t vmdata[VECTOR2_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs){			
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("acts_all::MEMCAP0_WRITETOKVDRAM_VDATASANDVMASKS:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	uint32_type datas[VECTOR2_SIZE];
 // CRITICAL FIXME. can be better for _WIDEWORD
	UTILP0_WRITEBITSTO_UINTV(&datas[0], 0, SIZEOF_VDATA0, vmdata[0].vdata); 
	UTILP0_WRITEBITSTO_UINTV(&datas[0], SIZEOF_VDATA0, SIZEOF_VMASK0, vmdata[0].vmask); 
 // CRITICAL FIXME. can be better for _WIDEWORD
	UTILP0_WRITEBITSTO_UINTV(&datas[1], 0, SIZEOF_VDATA0, vmdata[1].vdata); 
	UTILP0_WRITEBITSTO_UINTV(&datas[1], SIZEOF_VDATA0, SIZEOF_VMASK0, vmdata[1].vmask); 
 // CRITICAL FIXME. can be better for _WIDEWORD
	UTILP0_WRITEBITSTO_UINTV(&datas[2], 0, SIZEOF_VDATA0, vmdata[2].vdata); 
	UTILP0_WRITEBITSTO_UINTV(&datas[2], SIZEOF_VDATA0, SIZEOF_VMASK0, vmdata[2].vmask); 
 // CRITICAL FIXME. can be better for _WIDEWORD
	UTILP0_WRITEBITSTO_UINTV(&datas[3], 0, SIZEOF_VDATA0, vmdata[3].vdata); 
	UTILP0_WRITEBITSTO_UINTV(&datas[3], SIZEOF_VDATA0, SIZEOF_VMASK0, vmdata[3].vmask); 
 // CRITICAL FIXME. can be better for _WIDEWORD
	UTILP0_WRITEBITSTO_UINTV(&datas[4], 0, SIZEOF_VDATA0, vmdata[4].vdata); 
	UTILP0_WRITEBITSTO_UINTV(&datas[4], SIZEOF_VDATA0, SIZEOF_VMASK0, vmdata[4].vmask); 
 // CRITICAL FIXME. can be better for _WIDEWORD
	UTILP0_WRITEBITSTO_UINTV(&datas[5], 0, SIZEOF_VDATA0, vmdata[5].vdata); 
	UTILP0_WRITEBITSTO_UINTV(&datas[5], SIZEOF_VDATA0, SIZEOF_VMASK0, vmdata[5].vmask); 
 // CRITICAL FIXME. can be better for _WIDEWORD
	UTILP0_WRITEBITSTO_UINTV(&datas[6], 0, SIZEOF_VDATA0, vmdata[6].vdata); 
	UTILP0_WRITEBITSTO_UINTV(&datas[6], SIZEOF_VDATA0, SIZEOF_VMASK0, vmdata[6].vmask); 
 // CRITICAL FIXME. can be better for _WIDEWORD
	UTILP0_WRITEBITSTO_UINTV(&datas[7], 0, SIZEOF_VDATA0, vmdata[7].vdata); 
	UTILP0_WRITEBITSTO_UINTV(&datas[7], SIZEOF_VDATA0, SIZEOF_VMASK0, vmdata[7].vmask); 
 // CRITICAL FIXME. can be better for _WIDEWORD
	UTILP0_WRITEBITSTO_UINTV(&datas[8], 0, SIZEOF_VDATA0, vmdata[8].vdata); 
	UTILP0_WRITEBITSTO_UINTV(&datas[8], SIZEOF_VDATA0, SIZEOF_VMASK0, vmdata[8].vmask); 
 // CRITICAL FIXME. can be better for _WIDEWORD
	UTILP0_WRITEBITSTO_UINTV(&datas[9], 0, SIZEOF_VDATA0, vmdata[9].vdata); 
	UTILP0_WRITEBITSTO_UINTV(&datas[9], SIZEOF_VDATA0, SIZEOF_VMASK0, vmdata[9].vmask); 
 // CRITICAL FIXME. can be better for _WIDEWORD
	UTILP0_WRITEBITSTO_UINTV(&datas[10], 0, SIZEOF_VDATA0, vmdata[10].vdata); 
	UTILP0_WRITEBITSTO_UINTV(&datas[10], SIZEOF_VDATA0, SIZEOF_VMASK0, vmdata[10].vmask); 
 // CRITICAL FIXME. can be better for _WIDEWORD
	UTILP0_WRITEBITSTO_UINTV(&datas[11], 0, SIZEOF_VDATA0, vmdata[11].vdata); 
	UTILP0_WRITEBITSTO_UINTV(&datas[11], SIZEOF_VDATA0, SIZEOF_VMASK0, vmdata[11].vmask); 
 // CRITICAL FIXME. can be better for _WIDEWORD
	UTILP0_WRITEBITSTO_UINTV(&datas[12], 0, SIZEOF_VDATA0, vmdata[12].vdata); 
	UTILP0_WRITEBITSTO_UINTV(&datas[12], SIZEOF_VDATA0, SIZEOF_VMASK0, vmdata[12].vmask); 
 // CRITICAL FIXME. can be better for _WIDEWORD
	UTILP0_WRITEBITSTO_UINTV(&datas[13], 0, SIZEOF_VDATA0, vmdata[13].vdata); 
	UTILP0_WRITEBITSTO_UINTV(&datas[13], SIZEOF_VDATA0, SIZEOF_VMASK0, vmdata[13].vmask); 
 // CRITICAL FIXME. can be better for _WIDEWORD
	UTILP0_WRITEBITSTO_UINTV(&datas[14], 0, SIZEOF_VDATA0, vmdata[14].vdata); 
	UTILP0_WRITEBITSTO_UINTV(&datas[14], SIZEOF_VDATA0, SIZEOF_VMASK0, vmdata[14].vmask); 
 // CRITICAL FIXME. can be better for _WIDEWORD
	UTILP0_WRITEBITSTO_UINTV(&datas[15], 0, SIZEOF_VDATA0, vmdata[15].vdata); 
	UTILP0_WRITEBITSTO_UINTV(&datas[15], SIZEOF_VDATA0, SIZEOF_VMASK0, vmdata[15].vmask); 
	
	#ifdef _WIDEWORD // CRITICAL FIXME.
	kvdram[baseoffset_kvs + offset_kvs + index].range(31, 0) = datas[0];
	kvdram[baseoffset_kvs + offset_kvs + index].range(63, 32) = datas[1];
	kvdram[baseoffset_kvs + offset_kvs + index].range(95, 64) = datas[2];
	kvdram[baseoffset_kvs + offset_kvs + index].range(127, 96) = datas[3];
	kvdram[baseoffset_kvs + offset_kvs + index].range(159, 128) = datas[4];
	kvdram[baseoffset_kvs + offset_kvs + index].range(191, 160) = datas[5];
	kvdram[baseoffset_kvs + offset_kvs + index].range(223, 192) = datas[6];
	kvdram[baseoffset_kvs + offset_kvs + index].range(255, 224) = datas[7];
	kvdram[baseoffset_kvs + offset_kvs + index].range(287, 256) = datas[8];
	kvdram[baseoffset_kvs + offset_kvs + index].range(319, 288) = datas[9];
	kvdram[baseoffset_kvs + offset_kvs + index].range(351, 320) = datas[10];
	kvdram[baseoffset_kvs + offset_kvs + index].range(383, 352) = datas[11];
	kvdram[baseoffset_kvs + offset_kvs + index].range(415, 384) = datas[12];
	kvdram[baseoffset_kvs + offset_kvs + index].range(447, 416) = datas[13];
	kvdram[baseoffset_kvs + offset_kvs + index].range(479, 448) = datas[14];
	kvdram[baseoffset_kvs + offset_kvs + index].range(511, 480) = datas[15];
	#else 
	kvdram[baseoffset_kvs + offset_kvs + index].data[0].key = datas[0];
	kvdram[baseoffset_kvs + offset_kvs + index].data[0].value = datas[1];
	kvdram[baseoffset_kvs + offset_kvs + index].data[1].key = datas[2];
	kvdram[baseoffset_kvs + offset_kvs + index].data[1].value = datas[3];
	kvdram[baseoffset_kvs + offset_kvs + index].data[2].key = datas[4];
	kvdram[baseoffset_kvs + offset_kvs + index].data[2].value = datas[5];
	kvdram[baseoffset_kvs + offset_kvs + index].data[3].key = datas[6];
	kvdram[baseoffset_kvs + offset_kvs + index].data[3].value = datas[7];
	kvdram[baseoffset_kvs + offset_kvs + index].data[4].key = datas[8];
	kvdram[baseoffset_kvs + offset_kvs + index].data[4].value = datas[9];
	kvdram[baseoffset_kvs + offset_kvs + index].data[5].key = datas[10];
	kvdram[baseoffset_kvs + offset_kvs + index].data[5].value = datas[11];
	kvdram[baseoffset_kvs + offset_kvs + index].data[6].key = datas[12];
	kvdram[baseoffset_kvs + offset_kvs + index].data[6].value = datas[13];
	kvdram[baseoffset_kvs + offset_kvs + index].data[7].key = datas[14];
	kvdram[baseoffset_kvs + offset_kvs + index].data[7].value = datas[15];
	#endif
	return;
}

// non-primitives
tuple_t acts_all::MEMCAP0_READVDATA0WITHVMASK0(keyvalue_vbuffer_t wideword){
	#pragma HLS INLINE
	tuple_t res;
	res.A = MEMCAP0_READVDATA0(wideword); 
	res.B = MEMCAP0_READVMASK0(wideword); 
	return res;
}
tuple_t acts_all::MEMCAP0_READVDATA1WITHVMASK1(keyvalue_vbuffer_t wideword){
	#pragma HLS INLINE
	tuple_t res;
	res.A = MEMCAP0_READVDATA1(wideword); 
	res.B = MEMCAP0_READVMASK1(wideword); 
	return res;
}

void acts_all::MEMCAP0_WRITEVDATA0WITHVMASK0(keyvalue_vbuffer_t * wideword, value_t vdata, unit1_type vmask){
	#pragma HLS INLINE
	MEMCAP0_WRITEVDATA0(wideword, vdata);
	MEMCAP0_WRITEVMASK0(wideword, vmask);
	return;
}
void acts_all::MEMCAP0_WRITEVDATA1WITHVMASK1(keyvalue_vbuffer_t * wideword, value_t vdata, unit1_type vmask){
	#pragma HLS INLINE
	MEMCAP0_WRITEVDATA1(wideword, vdata);
	MEMCAP0_WRITEVMASK1(wideword, vmask);
	return;
}

// vdata 
// vdata:: used in {reduceupdates.cpp} 
void acts_all::MEMCAP0_WRITETOBUFFER_VDATA(unsigned int index, keyvalue_vbuffer_t buffer[BLOCKRAM_SIZE], value_t vdata, batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_WRITETOBUFFER_VDATA:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	if(index%2==0){
		MEMCAP0_WRITEVDATA0(&buffer[bufferoffset_kvs + index/2], vdata);
	} else{
		MEMCAP0_WRITEVDATA1(&buffer[bufferoffset_kvs + index/2], vdata);
	}
	return;
}

// vdata:: used in {dispatch_reduce -> mem_access_splitdstvxs.cpp -> MEMACCESSP0_SPL_readV} 
void acts_all::MEMCAP0_WRITETOBUFFER_VDATAS(unsigned int index, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], value_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("acts_all::MEMCAP0_WRITETOBUFFER_VDATAS:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	if(index%2==0){
		MEMCAP0_WRITEVDATA0WITHVMASK0(&buffer[0][bufferoffset_kvs + index/2], vdatas[0], 0);
		MEMCAP0_WRITEVDATA0WITHVMASK0(&buffer[1][bufferoffset_kvs + index/2], vdatas[1], 0);
		MEMCAP0_WRITEVDATA0WITHVMASK0(&buffer[2][bufferoffset_kvs + index/2], vdatas[2], 0);
		MEMCAP0_WRITEVDATA0WITHVMASK0(&buffer[3][bufferoffset_kvs + index/2], vdatas[3], 0);
		MEMCAP0_WRITEVDATA0WITHVMASK0(&buffer[4][bufferoffset_kvs + index/2], vdatas[4], 0);
		MEMCAP0_WRITEVDATA0WITHVMASK0(&buffer[5][bufferoffset_kvs + index/2], vdatas[5], 0);
		MEMCAP0_WRITEVDATA0WITHVMASK0(&buffer[6][bufferoffset_kvs + index/2], vdatas[6], 0);
		MEMCAP0_WRITEVDATA0WITHVMASK0(&buffer[7][bufferoffset_kvs + index/2], vdatas[7], 0);
		MEMCAP0_WRITEVDATA0WITHVMASK0(&buffer[8][bufferoffset_kvs + index/2], vdatas[8], 0);
		MEMCAP0_WRITEVDATA0WITHVMASK0(&buffer[9][bufferoffset_kvs + index/2], vdatas[9], 0);
		MEMCAP0_WRITEVDATA0WITHVMASK0(&buffer[10][bufferoffset_kvs + index/2], vdatas[10], 0);
		MEMCAP0_WRITEVDATA0WITHVMASK0(&buffer[11][bufferoffset_kvs + index/2], vdatas[11], 0);
		MEMCAP0_WRITEVDATA0WITHVMASK0(&buffer[12][bufferoffset_kvs + index/2], vdatas[12], 0);
		MEMCAP0_WRITEVDATA0WITHVMASK0(&buffer[13][bufferoffset_kvs + index/2], vdatas[13], 0);
		MEMCAP0_WRITEVDATA0WITHVMASK0(&buffer[14][bufferoffset_kvs + index/2], vdatas[14], 0);
		MEMCAP0_WRITEVDATA0WITHVMASK0(&buffer[15][bufferoffset_kvs + index/2], vdatas[15], 0);
	} else{
		MEMCAP0_WRITEVDATA1WITHVMASK1(&buffer[0][bufferoffset_kvs + index/2], vdatas[0], 0);
		MEMCAP0_WRITEVDATA1WITHVMASK1(&buffer[1][bufferoffset_kvs + index/2], vdatas[1], 0);
		MEMCAP0_WRITEVDATA1WITHVMASK1(&buffer[2][bufferoffset_kvs + index/2], vdatas[2], 0);
		MEMCAP0_WRITEVDATA1WITHVMASK1(&buffer[3][bufferoffset_kvs + index/2], vdatas[3], 0);
		MEMCAP0_WRITEVDATA1WITHVMASK1(&buffer[4][bufferoffset_kvs + index/2], vdatas[4], 0);
		MEMCAP0_WRITEVDATA1WITHVMASK1(&buffer[5][bufferoffset_kvs + index/2], vdatas[5], 0);
		MEMCAP0_WRITEVDATA1WITHVMASK1(&buffer[6][bufferoffset_kvs + index/2], vdatas[6], 0);
		MEMCAP0_WRITEVDATA1WITHVMASK1(&buffer[7][bufferoffset_kvs + index/2], vdatas[7], 0);
		MEMCAP0_WRITEVDATA1WITHVMASK1(&buffer[8][bufferoffset_kvs + index/2], vdatas[8], 0);
		MEMCAP0_WRITEVDATA1WITHVMASK1(&buffer[9][bufferoffset_kvs + index/2], vdatas[9], 0);
		MEMCAP0_WRITEVDATA1WITHVMASK1(&buffer[10][bufferoffset_kvs + index/2], vdatas[10], 0);
		MEMCAP0_WRITEVDATA1WITHVMASK1(&buffer[11][bufferoffset_kvs + index/2], vdatas[11], 0);
		MEMCAP0_WRITEVDATA1WITHVMASK1(&buffer[12][bufferoffset_kvs + index/2], vdatas[12], 0);
		MEMCAP0_WRITEVDATA1WITHVMASK1(&buffer[13][bufferoffset_kvs + index/2], vdatas[13], 0);
		MEMCAP0_WRITEVDATA1WITHVMASK1(&buffer[14][bufferoffset_kvs + index/2], vdatas[14], 0);
		MEMCAP0_WRITEVDATA1WITHVMASK1(&buffer[15][bufferoffset_kvs + index/2], vdatas[15], 0);
	}
	return;
}

// vdata:: used in {reduceupdates.cpp, processedges_splitdstvxs.cpp} 
value_t acts_all::MEMCAP0_READFROMBUFFER_VDATA(unsigned int index, keyvalue_vbuffer_t buffer[BLOCKRAM_SIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("acts_all::MEMCAP0_READFROMBUFFER_VDATA:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	value_t vdata = 0;
	if(index%2==0){
		vdata = MEMCAP0_READVDATA0(buffer[bufferoffset_kvs + index/2]);
	} else{
		vdata = MEMCAP0_READVDATA1(buffer[bufferoffset_kvs + index/2]);
	}
	return vdata;
}

// vdata:: used in {processedges_splitdstvxs.cpp} // soon obsolete 
void acts_all::MEMCAP0_READFROMBUFFER_VDATAS(unsigned int index, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], value_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("acts_all::MEMCAP0_READFROMBUFFER_VDATAS:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	if(index%2==0){
		vdatas[0] = MEMCAP0_READVDATA0(buffer[0][bufferoffset_kvs + index/2]);
		vdatas[1] = MEMCAP0_READVDATA0(buffer[1][bufferoffset_kvs + index/2]);
		vdatas[2] = MEMCAP0_READVDATA0(buffer[2][bufferoffset_kvs + index/2]);
		vdatas[3] = MEMCAP0_READVDATA0(buffer[3][bufferoffset_kvs + index/2]);
		vdatas[4] = MEMCAP0_READVDATA0(buffer[4][bufferoffset_kvs + index/2]);
		vdatas[5] = MEMCAP0_READVDATA0(buffer[5][bufferoffset_kvs + index/2]);
		vdatas[6] = MEMCAP0_READVDATA0(buffer[6][bufferoffset_kvs + index/2]);
		vdatas[7] = MEMCAP0_READVDATA0(buffer[7][bufferoffset_kvs + index/2]);
		vdatas[8] = MEMCAP0_READVDATA0(buffer[8][bufferoffset_kvs + index/2]);
		vdatas[9] = MEMCAP0_READVDATA0(buffer[9][bufferoffset_kvs + index/2]);
		vdatas[10] = MEMCAP0_READVDATA0(buffer[10][bufferoffset_kvs + index/2]);
		vdatas[11] = MEMCAP0_READVDATA0(buffer[11][bufferoffset_kvs + index/2]);
		vdatas[12] = MEMCAP0_READVDATA0(buffer[12][bufferoffset_kvs + index/2]);
		vdatas[13] = MEMCAP0_READVDATA0(buffer[13][bufferoffset_kvs + index/2]);
		vdatas[14] = MEMCAP0_READVDATA0(buffer[14][bufferoffset_kvs + index/2]);
		vdatas[15] = MEMCAP0_READVDATA0(buffer[15][bufferoffset_kvs + index/2]);
	} else{
		vdatas[0] = MEMCAP0_READVDATA1(buffer[0][bufferoffset_kvs + index/2]);
		vdatas[1] = MEMCAP0_READVDATA1(buffer[1][bufferoffset_kvs + index/2]);
		vdatas[2] = MEMCAP0_READVDATA1(buffer[2][bufferoffset_kvs + index/2]);
		vdatas[3] = MEMCAP0_READVDATA1(buffer[3][bufferoffset_kvs + index/2]);
		vdatas[4] = MEMCAP0_READVDATA1(buffer[4][bufferoffset_kvs + index/2]);
		vdatas[5] = MEMCAP0_READVDATA1(buffer[5][bufferoffset_kvs + index/2]);
		vdatas[6] = MEMCAP0_READVDATA1(buffer[6][bufferoffset_kvs + index/2]);
		vdatas[7] = MEMCAP0_READVDATA1(buffer[7][bufferoffset_kvs + index/2]);
		vdatas[8] = MEMCAP0_READVDATA1(buffer[8][bufferoffset_kvs + index/2]);
		vdatas[9] = MEMCAP0_READVDATA1(buffer[9][bufferoffset_kvs + index/2]);
		vdatas[10] = MEMCAP0_READVDATA1(buffer[10][bufferoffset_kvs + index/2]);
		vdatas[11] = MEMCAP0_READVDATA1(buffer[11][bufferoffset_kvs + index/2]);
		vdatas[12] = MEMCAP0_READVDATA1(buffer[12][bufferoffset_kvs + index/2]);
		vdatas[13] = MEMCAP0_READVDATA1(buffer[13][bufferoffset_kvs + index/2]);
		vdatas[14] = MEMCAP0_READVDATA1(buffer[14][bufferoffset_kvs + index/2]);
		vdatas[15] = MEMCAP0_READVDATA1(buffer[15][bufferoffset_kvs + index/2]);
	}
	return;
}

// vmasks (soon OBSOLETE)
unit1_type acts_all::MEMCAP0_READFROMBUFFER_VMASK(unsigned int index, unit1_type vmaskBITS[DOUBLE_BLOCKRAM_SIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_READFROMBUFFER_VMASK:", index, DOUBLE_BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	unit1_type vmdata = vmaskBITS[bufferoffset_kvs + index];
	return vmdata;
}

void acts_all::MEMCAP0_READFROMBUFFER_VMASKS(unsigned int index, unit1_type vmaskBITS[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE], unit1_type vmdatas[VMASK_PACKINGSIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_READFROMBUFFER_VMASKS:", index, DOUBLE_BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	vmdatas[0] = vmaskBITS[0][bufferoffset_kvs + index];
	vmdatas[1] = vmaskBITS[1][bufferoffset_kvs + index];
	vmdatas[2] = vmaskBITS[2][bufferoffset_kvs + index];
	vmdatas[3] = vmaskBITS[3][bufferoffset_kvs + index];
	vmdatas[4] = vmaskBITS[4][bufferoffset_kvs + index];
	vmdatas[5] = vmaskBITS[5][bufferoffset_kvs + index];
	vmdatas[6] = vmaskBITS[6][bufferoffset_kvs + index];
	vmdatas[7] = vmaskBITS[7][bufferoffset_kvs + index];
	vmdatas[8] = vmaskBITS[8][bufferoffset_kvs + index];
	vmdatas[9] = vmaskBITS[9][bufferoffset_kvs + index];
	vmdatas[10] = vmaskBITS[10][bufferoffset_kvs + index];
	vmdatas[11] = vmaskBITS[11][bufferoffset_kvs + index];
	vmdatas[12] = vmaskBITS[12][bufferoffset_kvs + index];
	vmdatas[13] = vmaskBITS[13][bufferoffset_kvs + index];
	vmdatas[14] = vmaskBITS[14][bufferoffset_kvs + index];
	vmdatas[15] = vmaskBITS[15][bufferoffset_kvs + index];
	return;
}

void acts_all::MEMCAP0_WRITETOBUFFER_VMASKS(unsigned int index, unit1_type vmaskBITS[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE], unit1_type vmdatas[VMASK_PACKINGSIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_WRITETOBUFFER_VMASKS:", index, DOUBLE_BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	vmaskBITS[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS[15][bufferoffset_kvs + index] = vmdatas[15];
	return;
}

void acts_all::MEMCAP0_WRITETOBUFFER_VMASKS1_ANDREPLICATE(unsigned int index, unit1_type vmaskBITS0[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE], unit1_type vmdatas[VMASK_PACKINGSIZE], batch_type bufferoffset_kvs){					
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_WRITETOBUFFER_VMASKS:", index, DOUBLE_BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	vmaskBITS0[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS0[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS0[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS0[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS0[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS0[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS0[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS0[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS0[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS0[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS0[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS0[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS0[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS0[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS0[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS0[15][bufferoffset_kvs + index] = vmdatas[15];
	return;
}
void acts_all::MEMCAP0_WRITETOBUFFER_VMASKS2_ANDREPLICATE(unsigned int index, unit1_type vmaskBITS0[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS1[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE], unit1_type vmdatas[VMASK_PACKINGSIZE], batch_type bufferoffset_kvs){					
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_WRITETOBUFFER_VMASKS:", index, DOUBLE_BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	vmaskBITS0[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS0[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS0[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS0[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS0[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS0[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS0[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS0[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS0[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS0[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS0[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS0[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS0[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS0[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS0[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS0[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS1[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS1[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS1[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS1[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS1[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS1[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS1[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS1[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS1[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS1[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS1[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS1[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS1[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS1[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS1[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS1[15][bufferoffset_kvs + index] = vmdatas[15];
	return;
}
void acts_all::MEMCAP0_WRITETOBUFFER_VMASKS3_ANDREPLICATE(unsigned int index, unit1_type vmaskBITS0[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS1[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS2[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE], unit1_type vmdatas[VMASK_PACKINGSIZE], batch_type bufferoffset_kvs){					
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_WRITETOBUFFER_VMASKS:", index, DOUBLE_BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	vmaskBITS0[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS0[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS0[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS0[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS0[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS0[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS0[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS0[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS0[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS0[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS0[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS0[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS0[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS0[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS0[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS0[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS1[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS1[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS1[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS1[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS1[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS1[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS1[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS1[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS1[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS1[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS1[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS1[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS1[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS1[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS1[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS1[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS2[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS2[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS2[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS2[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS2[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS2[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS2[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS2[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS2[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS2[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS2[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS2[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS2[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS2[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS2[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS2[15][bufferoffset_kvs + index] = vmdatas[15];
	return;
}
void acts_all::MEMCAP0_WRITETOBUFFER_VMASKS4_ANDREPLICATE(unsigned int index, unit1_type vmaskBITS0[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS1[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS2[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS3[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE], unit1_type vmdatas[VMASK_PACKINGSIZE], batch_type bufferoffset_kvs){					
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_WRITETOBUFFER_VMASKS:", index, DOUBLE_BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	vmaskBITS0[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS0[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS0[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS0[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS0[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS0[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS0[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS0[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS0[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS0[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS0[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS0[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS0[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS0[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS0[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS0[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS1[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS1[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS1[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS1[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS1[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS1[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS1[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS1[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS1[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS1[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS1[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS1[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS1[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS1[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS1[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS1[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS2[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS2[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS2[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS2[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS2[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS2[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS2[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS2[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS2[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS2[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS2[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS2[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS2[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS2[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS2[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS2[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS3[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS3[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS3[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS3[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS3[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS3[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS3[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS3[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS3[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS3[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS3[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS3[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS3[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS3[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS3[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS3[15][bufferoffset_kvs + index] = vmdatas[15];
	return;
}
void acts_all::MEMCAP0_WRITETOBUFFER_VMASKS5_ANDREPLICATE(unsigned int index, unit1_type vmaskBITS0[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS1[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS2[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS3[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS4[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE], unit1_type vmdatas[VMASK_PACKINGSIZE], batch_type bufferoffset_kvs){					
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_WRITETOBUFFER_VMASKS:", index, DOUBLE_BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	vmaskBITS0[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS0[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS0[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS0[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS0[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS0[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS0[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS0[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS0[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS0[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS0[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS0[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS0[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS0[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS0[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS0[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS1[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS1[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS1[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS1[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS1[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS1[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS1[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS1[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS1[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS1[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS1[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS1[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS1[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS1[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS1[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS1[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS2[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS2[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS2[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS2[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS2[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS2[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS2[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS2[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS2[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS2[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS2[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS2[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS2[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS2[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS2[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS2[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS3[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS3[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS3[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS3[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS3[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS3[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS3[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS3[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS3[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS3[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS3[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS3[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS3[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS3[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS3[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS3[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS4[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS4[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS4[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS4[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS4[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS4[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS4[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS4[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS4[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS4[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS4[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS4[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS4[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS4[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS4[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS4[15][bufferoffset_kvs + index] = vmdatas[15];
	return;
}
void acts_all::MEMCAP0_WRITETOBUFFER_VMASKS6_ANDREPLICATE(unsigned int index, unit1_type vmaskBITS0[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS1[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS2[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS3[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS4[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS5[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE], unit1_type vmdatas[VMASK_PACKINGSIZE], batch_type bufferoffset_kvs){					
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_WRITETOBUFFER_VMASKS:", index, DOUBLE_BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	vmaskBITS0[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS0[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS0[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS0[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS0[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS0[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS0[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS0[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS0[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS0[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS0[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS0[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS0[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS0[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS0[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS0[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS1[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS1[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS1[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS1[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS1[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS1[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS1[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS1[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS1[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS1[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS1[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS1[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS1[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS1[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS1[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS1[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS2[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS2[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS2[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS2[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS2[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS2[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS2[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS2[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS2[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS2[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS2[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS2[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS2[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS2[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS2[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS2[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS3[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS3[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS3[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS3[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS3[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS3[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS3[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS3[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS3[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS3[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS3[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS3[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS3[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS3[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS3[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS3[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS4[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS4[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS4[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS4[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS4[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS4[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS4[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS4[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS4[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS4[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS4[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS4[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS4[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS4[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS4[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS4[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS5[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS5[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS5[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS5[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS5[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS5[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS5[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS5[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS5[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS5[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS5[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS5[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS5[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS5[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS5[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS5[15][bufferoffset_kvs + index] = vmdatas[15];
	return;
}
void acts_all::MEMCAP0_WRITETOBUFFER_VMASKS7_ANDREPLICATE(unsigned int index, unit1_type vmaskBITS0[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS1[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS2[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS3[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS4[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS5[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS6[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE], unit1_type vmdatas[VMASK_PACKINGSIZE], batch_type bufferoffset_kvs){					
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_WRITETOBUFFER_VMASKS:", index, DOUBLE_BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	vmaskBITS0[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS0[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS0[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS0[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS0[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS0[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS0[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS0[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS0[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS0[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS0[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS0[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS0[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS0[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS0[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS0[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS1[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS1[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS1[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS1[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS1[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS1[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS1[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS1[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS1[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS1[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS1[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS1[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS1[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS1[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS1[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS1[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS2[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS2[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS2[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS2[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS2[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS2[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS2[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS2[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS2[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS2[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS2[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS2[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS2[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS2[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS2[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS2[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS3[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS3[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS3[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS3[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS3[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS3[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS3[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS3[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS3[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS3[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS3[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS3[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS3[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS3[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS3[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS3[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS4[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS4[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS4[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS4[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS4[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS4[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS4[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS4[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS4[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS4[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS4[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS4[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS4[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS4[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS4[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS4[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS5[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS5[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS5[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS5[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS5[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS5[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS5[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS5[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS5[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS5[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS5[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS5[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS5[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS5[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS5[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS5[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS6[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS6[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS6[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS6[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS6[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS6[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS6[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS6[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS6[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS6[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS6[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS6[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS6[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS6[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS6[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS6[15][bufferoffset_kvs + index] = vmdatas[15];
	return;
}
void acts_all::MEMCAP0_WRITETOBUFFER_VMASKS8_ANDREPLICATE(unsigned int index, unit1_type vmaskBITS0[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS1[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS2[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS3[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS4[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS5[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS6[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS7[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE], unit1_type vmdatas[VMASK_PACKINGSIZE], batch_type bufferoffset_kvs){					
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_WRITETOBUFFER_VMASKS:", index, DOUBLE_BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	vmaskBITS0[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS0[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS0[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS0[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS0[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS0[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS0[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS0[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS0[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS0[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS0[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS0[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS0[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS0[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS0[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS0[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS1[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS1[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS1[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS1[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS1[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS1[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS1[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS1[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS1[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS1[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS1[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS1[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS1[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS1[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS1[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS1[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS2[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS2[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS2[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS2[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS2[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS2[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS2[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS2[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS2[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS2[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS2[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS2[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS2[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS2[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS2[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS2[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS3[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS3[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS3[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS3[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS3[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS3[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS3[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS3[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS3[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS3[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS3[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS3[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS3[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS3[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS3[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS3[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS4[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS4[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS4[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS4[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS4[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS4[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS4[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS4[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS4[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS4[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS4[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS4[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS4[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS4[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS4[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS4[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS5[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS5[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS5[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS5[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS5[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS5[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS5[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS5[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS5[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS5[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS5[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS5[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS5[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS5[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS5[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS5[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS6[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS6[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS6[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS6[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS6[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS6[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS6[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS6[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS6[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS6[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS6[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS6[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS6[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS6[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS6[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS6[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS7[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS7[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS7[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS7[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS7[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS7[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS7[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS7[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS7[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS7[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS7[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS7[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS7[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS7[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS7[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS7[15][bufferoffset_kvs + index] = vmdatas[15];
	return;
}
void acts_all::MEMCAP0_WRITETOBUFFER_VMASKS9_ANDREPLICATE(unsigned int index, unit1_type vmaskBITS0[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS1[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS2[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS3[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS4[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS5[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS6[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS7[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS8[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE], unit1_type vmdatas[VMASK_PACKINGSIZE], batch_type bufferoffset_kvs){					
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_WRITETOBUFFER_VMASKS:", index, DOUBLE_BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	vmaskBITS0[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS0[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS0[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS0[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS0[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS0[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS0[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS0[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS0[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS0[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS0[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS0[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS0[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS0[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS0[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS0[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS1[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS1[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS1[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS1[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS1[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS1[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS1[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS1[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS1[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS1[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS1[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS1[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS1[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS1[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS1[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS1[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS2[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS2[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS2[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS2[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS2[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS2[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS2[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS2[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS2[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS2[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS2[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS2[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS2[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS2[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS2[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS2[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS3[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS3[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS3[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS3[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS3[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS3[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS3[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS3[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS3[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS3[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS3[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS3[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS3[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS3[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS3[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS3[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS4[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS4[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS4[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS4[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS4[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS4[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS4[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS4[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS4[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS4[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS4[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS4[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS4[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS4[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS4[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS4[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS5[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS5[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS5[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS5[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS5[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS5[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS5[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS5[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS5[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS5[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS5[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS5[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS5[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS5[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS5[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS5[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS6[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS6[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS6[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS6[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS6[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS6[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS6[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS6[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS6[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS6[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS6[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS6[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS6[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS6[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS6[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS6[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS7[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS7[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS7[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS7[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS7[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS7[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS7[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS7[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS7[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS7[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS7[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS7[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS7[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS7[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS7[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS7[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS8[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS8[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS8[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS8[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS8[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS8[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS8[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS8[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS8[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS8[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS8[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS8[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS8[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS8[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS8[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS8[15][bufferoffset_kvs + index] = vmdatas[15];
	return;
}
void acts_all::MEMCAP0_WRITETOBUFFER_VMASKS10_ANDREPLICATE(unsigned int index, unit1_type vmaskBITS0[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS1[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS2[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS3[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS4[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS5[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS6[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS7[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS8[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS9[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE], unit1_type vmdatas[VMASK_PACKINGSIZE], batch_type bufferoffset_kvs){					
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_WRITETOBUFFER_VMASKS:", index, DOUBLE_BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	vmaskBITS0[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS0[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS0[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS0[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS0[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS0[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS0[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS0[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS0[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS0[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS0[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS0[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS0[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS0[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS0[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS0[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS1[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS1[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS1[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS1[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS1[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS1[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS1[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS1[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS1[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS1[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS1[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS1[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS1[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS1[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS1[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS1[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS2[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS2[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS2[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS2[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS2[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS2[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS2[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS2[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS2[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS2[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS2[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS2[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS2[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS2[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS2[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS2[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS3[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS3[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS3[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS3[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS3[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS3[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS3[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS3[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS3[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS3[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS3[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS3[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS3[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS3[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS3[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS3[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS4[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS4[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS4[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS4[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS4[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS4[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS4[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS4[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS4[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS4[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS4[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS4[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS4[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS4[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS4[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS4[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS5[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS5[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS5[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS5[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS5[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS5[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS5[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS5[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS5[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS5[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS5[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS5[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS5[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS5[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS5[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS5[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS6[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS6[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS6[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS6[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS6[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS6[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS6[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS6[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS6[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS6[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS6[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS6[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS6[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS6[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS6[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS6[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS7[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS7[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS7[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS7[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS7[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS7[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS7[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS7[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS7[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS7[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS7[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS7[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS7[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS7[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS7[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS7[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS8[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS8[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS8[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS8[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS8[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS8[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS8[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS8[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS8[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS8[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS8[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS8[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS8[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS8[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS8[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS8[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS9[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS9[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS9[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS9[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS9[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS9[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS9[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS9[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS9[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS9[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS9[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS9[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS9[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS9[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS9[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS9[15][bufferoffset_kvs + index] = vmdatas[15];
	return;
}
void acts_all::MEMCAP0_WRITETOBUFFER_VMASKS11_ANDREPLICATE(unsigned int index, unit1_type vmaskBITS0[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS1[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS2[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS3[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS4[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS5[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS6[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS7[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS8[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS9[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS10[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE], unit1_type vmdatas[VMASK_PACKINGSIZE], batch_type bufferoffset_kvs){					
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_WRITETOBUFFER_VMASKS:", index, DOUBLE_BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	vmaskBITS0[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS0[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS0[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS0[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS0[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS0[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS0[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS0[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS0[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS0[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS0[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS0[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS0[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS0[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS0[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS0[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS1[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS1[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS1[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS1[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS1[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS1[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS1[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS1[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS1[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS1[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS1[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS1[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS1[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS1[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS1[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS1[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS2[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS2[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS2[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS2[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS2[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS2[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS2[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS2[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS2[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS2[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS2[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS2[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS2[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS2[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS2[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS2[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS3[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS3[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS3[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS3[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS3[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS3[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS3[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS3[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS3[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS3[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS3[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS3[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS3[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS3[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS3[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS3[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS4[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS4[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS4[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS4[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS4[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS4[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS4[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS4[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS4[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS4[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS4[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS4[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS4[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS4[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS4[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS4[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS5[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS5[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS5[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS5[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS5[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS5[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS5[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS5[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS5[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS5[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS5[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS5[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS5[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS5[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS5[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS5[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS6[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS6[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS6[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS6[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS6[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS6[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS6[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS6[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS6[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS6[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS6[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS6[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS6[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS6[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS6[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS6[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS7[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS7[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS7[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS7[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS7[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS7[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS7[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS7[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS7[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS7[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS7[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS7[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS7[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS7[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS7[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS7[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS8[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS8[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS8[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS8[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS8[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS8[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS8[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS8[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS8[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS8[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS8[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS8[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS8[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS8[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS8[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS8[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS9[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS9[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS9[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS9[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS9[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS9[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS9[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS9[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS9[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS9[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS9[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS9[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS9[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS9[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS9[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS9[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS10[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS10[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS10[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS10[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS10[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS10[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS10[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS10[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS10[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS10[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS10[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS10[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS10[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS10[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS10[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS10[15][bufferoffset_kvs + index] = vmdatas[15];
	return;
}
void acts_all::MEMCAP0_WRITETOBUFFER_VMASKS12_ANDREPLICATE(unsigned int index, unit1_type vmaskBITS0[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS1[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS2[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS3[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS4[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS5[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS6[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS7[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS8[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS9[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS10[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE],unit1_type vmaskBITS11[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE], unit1_type vmdatas[VMASK_PACKINGSIZE], batch_type bufferoffset_kvs){					
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_WRITETOBUFFER_VMASKS:", index, DOUBLE_BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	vmaskBITS0[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS0[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS0[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS0[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS0[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS0[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS0[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS0[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS0[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS0[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS0[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS0[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS0[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS0[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS0[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS0[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS1[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS1[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS1[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS1[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS1[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS1[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS1[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS1[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS1[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS1[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS1[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS1[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS1[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS1[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS1[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS1[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS2[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS2[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS2[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS2[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS2[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS2[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS2[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS2[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS2[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS2[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS2[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS2[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS2[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS2[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS2[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS2[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS3[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS3[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS3[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS3[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS3[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS3[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS3[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS3[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS3[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS3[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS3[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS3[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS3[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS3[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS3[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS3[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS4[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS4[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS4[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS4[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS4[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS4[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS4[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS4[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS4[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS4[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS4[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS4[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS4[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS4[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS4[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS4[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS5[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS5[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS5[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS5[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS5[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS5[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS5[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS5[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS5[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS5[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS5[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS5[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS5[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS5[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS5[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS5[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS6[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS6[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS6[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS6[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS6[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS6[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS6[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS6[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS6[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS6[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS6[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS6[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS6[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS6[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS6[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS6[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS7[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS7[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS7[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS7[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS7[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS7[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS7[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS7[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS7[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS7[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS7[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS7[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS7[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS7[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS7[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS7[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS8[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS8[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS8[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS8[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS8[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS8[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS8[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS8[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS8[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS8[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS8[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS8[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS8[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS8[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS8[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS8[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS9[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS9[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS9[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS9[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS9[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS9[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS9[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS9[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS9[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS9[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS9[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS9[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS9[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS9[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS9[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS9[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS10[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS10[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS10[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS10[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS10[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS10[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS10[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS10[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS10[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS10[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS10[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS10[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS10[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS10[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS10[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS10[15][bufferoffset_kvs + index] = vmdatas[15];
	vmaskBITS11[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS11[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS11[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS11[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS11[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS11[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS11[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS11[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS11[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS11[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS11[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS11[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS11[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS11[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS11[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS11[15][bufferoffset_kvs + index] = vmdatas[15];
	return;
}

void acts_all::MEMCAP0_WRITETOBUFFER_VMASKS_WITHDEPTHS(unsigned int indexes[VDATA_PACKINGSIZE], unit1_type vmaskBITS[VMASK_PACKINGSIZE][BLOCKRAM_VMASK_SIZE], unit1_type vmdatas[VMASK_PACKINGSIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_WRITETOBUFFER_VMASKS_WITHDEPTHS:", indexes[0], DOUBLE_BLOCKRAM_SIZE, indexes[0], NAp, NAp);
	#endif
	vmaskBITS[0][bufferoffset_kvs + indexes[0]] = vmdatas[0];
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_WRITETOBUFFER_VMASKS_WITHDEPTHS:", indexes[1], DOUBLE_BLOCKRAM_SIZE, indexes[1], NAp, NAp);
	#endif
	vmaskBITS[1][bufferoffset_kvs + indexes[1]] = vmdatas[1];
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_WRITETOBUFFER_VMASKS_WITHDEPTHS:", indexes[2], DOUBLE_BLOCKRAM_SIZE, indexes[2], NAp, NAp);
	#endif
	vmaskBITS[2][bufferoffset_kvs + indexes[2]] = vmdatas[2];
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_WRITETOBUFFER_VMASKS_WITHDEPTHS:", indexes[3], DOUBLE_BLOCKRAM_SIZE, indexes[3], NAp, NAp);
	#endif
	vmaskBITS[3][bufferoffset_kvs + indexes[3]] = vmdatas[3];
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_WRITETOBUFFER_VMASKS_WITHDEPTHS:", indexes[4], DOUBLE_BLOCKRAM_SIZE, indexes[4], NAp, NAp);
	#endif
	vmaskBITS[4][bufferoffset_kvs + indexes[4]] = vmdatas[4];
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_WRITETOBUFFER_VMASKS_WITHDEPTHS:", indexes[5], DOUBLE_BLOCKRAM_SIZE, indexes[5], NAp, NAp);
	#endif
	vmaskBITS[5][bufferoffset_kvs + indexes[5]] = vmdatas[5];
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_WRITETOBUFFER_VMASKS_WITHDEPTHS:", indexes[6], DOUBLE_BLOCKRAM_SIZE, indexes[6], NAp, NAp);
	#endif
	vmaskBITS[6][bufferoffset_kvs + indexes[6]] = vmdatas[6];
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_WRITETOBUFFER_VMASKS_WITHDEPTHS:", indexes[7], DOUBLE_BLOCKRAM_SIZE, indexes[7], NAp, NAp);
	#endif
	vmaskBITS[7][bufferoffset_kvs + indexes[7]] = vmdatas[7];
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_WRITETOBUFFER_VMASKS_WITHDEPTHS:", indexes[8], DOUBLE_BLOCKRAM_SIZE, indexes[8], NAp, NAp);
	#endif
	vmaskBITS[8][bufferoffset_kvs + indexes[8]] = vmdatas[8];
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_WRITETOBUFFER_VMASKS_WITHDEPTHS:", indexes[9], DOUBLE_BLOCKRAM_SIZE, indexes[9], NAp, NAp);
	#endif
	vmaskBITS[9][bufferoffset_kvs + indexes[9]] = vmdatas[9];
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_WRITETOBUFFER_VMASKS_WITHDEPTHS:", indexes[10], DOUBLE_BLOCKRAM_SIZE, indexes[10], NAp, NAp);
	#endif
	vmaskBITS[10][bufferoffset_kvs + indexes[10]] = vmdatas[10];
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_WRITETOBUFFER_VMASKS_WITHDEPTHS:", indexes[11], DOUBLE_BLOCKRAM_SIZE, indexes[11], NAp, NAp);
	#endif
	vmaskBITS[11][bufferoffset_kvs + indexes[11]] = vmdatas[11];
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_WRITETOBUFFER_VMASKS_WITHDEPTHS:", indexes[12], DOUBLE_BLOCKRAM_SIZE, indexes[12], NAp, NAp);
	#endif
	vmaskBITS[12][bufferoffset_kvs + indexes[12]] = vmdatas[12];
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_WRITETOBUFFER_VMASKS_WITHDEPTHS:", indexes[13], DOUBLE_BLOCKRAM_SIZE, indexes[13], NAp, NAp);
	#endif
	vmaskBITS[13][bufferoffset_kvs + indexes[13]] = vmdatas[13];
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_WRITETOBUFFER_VMASKS_WITHDEPTHS:", indexes[14], DOUBLE_BLOCKRAM_SIZE, indexes[14], NAp, NAp);
	#endif
	vmaskBITS[14][bufferoffset_kvs + indexes[14]] = vmdatas[14];
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_WRITETOBUFFER_VMASKS_WITHDEPTHS:", indexes[15], DOUBLE_BLOCKRAM_SIZE, indexes[15], NAp, NAp);
	#endif
	vmaskBITS[15][bufferoffset_kvs + indexes[15]] = vmdatas[15];
	return;
}

// vdata & vmasks 
// used in {classname__processedges_splitdstvxs.cpp} 
void acts_all::MEMCAP0_READFROMBUFFER_VDATASWITHVMASKS(unsigned int index, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], value_t vdatas[VECTOR2_SIZE], unit1_type vmdatas[VMASK_PACKINGSIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("acts_all::MEMCAP0_READFROMBUFFER_VDATASWITHVMASKS:", bufferoffset_kvs + index/2, BLOCKRAM_VDATA_SIZE, index, NAp, NAp);
	#endif
	
	if(index%2==0){
		tuple_t tup0 = MEMCAP0_READVDATA0WITHVMASK0(buffer[0][bufferoffset_kvs + index/2]);
		vdatas[0] = tup0.A;
		vmdatas[0] = tup0.B;
		tuple_t tup1 = MEMCAP0_READVDATA0WITHVMASK0(buffer[1][bufferoffset_kvs + index/2]);
		vdatas[1] = tup1.A;
		vmdatas[1] = tup1.B;
		tuple_t tup2 = MEMCAP0_READVDATA0WITHVMASK0(buffer[2][bufferoffset_kvs + index/2]);
		vdatas[2] = tup2.A;
		vmdatas[2] = tup2.B;
		tuple_t tup3 = MEMCAP0_READVDATA0WITHVMASK0(buffer[3][bufferoffset_kvs + index/2]);
		vdatas[3] = tup3.A;
		vmdatas[3] = tup3.B;
		tuple_t tup4 = MEMCAP0_READVDATA0WITHVMASK0(buffer[4][bufferoffset_kvs + index/2]);
		vdatas[4] = tup4.A;
		vmdatas[4] = tup4.B;
		tuple_t tup5 = MEMCAP0_READVDATA0WITHVMASK0(buffer[5][bufferoffset_kvs + index/2]);
		vdatas[5] = tup5.A;
		vmdatas[5] = tup5.B;
		tuple_t tup6 = MEMCAP0_READVDATA0WITHVMASK0(buffer[6][bufferoffset_kvs + index/2]);
		vdatas[6] = tup6.A;
		vmdatas[6] = tup6.B;
		tuple_t tup7 = MEMCAP0_READVDATA0WITHVMASK0(buffer[7][bufferoffset_kvs + index/2]);
		vdatas[7] = tup7.A;
		vmdatas[7] = tup7.B;
		tuple_t tup8 = MEMCAP0_READVDATA0WITHVMASK0(buffer[8][bufferoffset_kvs + index/2]);
		vdatas[8] = tup8.A;
		vmdatas[8] = tup8.B;
		tuple_t tup9 = MEMCAP0_READVDATA0WITHVMASK0(buffer[9][bufferoffset_kvs + index/2]);
		vdatas[9] = tup9.A;
		vmdatas[9] = tup9.B;
		tuple_t tup10 = MEMCAP0_READVDATA0WITHVMASK0(buffer[10][bufferoffset_kvs + index/2]);
		vdatas[10] = tup10.A;
		vmdatas[10] = tup10.B;
		tuple_t tup11 = MEMCAP0_READVDATA0WITHVMASK0(buffer[11][bufferoffset_kvs + index/2]);
		vdatas[11] = tup11.A;
		vmdatas[11] = tup11.B;
		tuple_t tup12 = MEMCAP0_READVDATA0WITHVMASK0(buffer[12][bufferoffset_kvs + index/2]);
		vdatas[12] = tup12.A;
		vmdatas[12] = tup12.B;
		tuple_t tup13 = MEMCAP0_READVDATA0WITHVMASK0(buffer[13][bufferoffset_kvs + index/2]);
		vdatas[13] = tup13.A;
		vmdatas[13] = tup13.B;
		tuple_t tup14 = MEMCAP0_READVDATA0WITHVMASK0(buffer[14][bufferoffset_kvs + index/2]);
		vdatas[14] = tup14.A;
		vmdatas[14] = tup14.B;
		tuple_t tup15 = MEMCAP0_READVDATA0WITHVMASK0(buffer[15][bufferoffset_kvs + index/2]);
		vdatas[15] = tup15.A;
		vmdatas[15] = tup15.B;
	} else{
		tuple_t tup0 = MEMCAP0_READVDATA1WITHVMASK1(buffer[0][bufferoffset_kvs + index/2]);
		vdatas[0] = tup0.A;
		vmdatas[0] = tup0.B;
		tuple_t tup1 = MEMCAP0_READVDATA1WITHVMASK1(buffer[1][bufferoffset_kvs + index/2]);
		vdatas[1] = tup1.A;
		vmdatas[1] = tup1.B;
		tuple_t tup2 = MEMCAP0_READVDATA1WITHVMASK1(buffer[2][bufferoffset_kvs + index/2]);
		vdatas[2] = tup2.A;
		vmdatas[2] = tup2.B;
		tuple_t tup3 = MEMCAP0_READVDATA1WITHVMASK1(buffer[3][bufferoffset_kvs + index/2]);
		vdatas[3] = tup3.A;
		vmdatas[3] = tup3.B;
		tuple_t tup4 = MEMCAP0_READVDATA1WITHVMASK1(buffer[4][bufferoffset_kvs + index/2]);
		vdatas[4] = tup4.A;
		vmdatas[4] = tup4.B;
		tuple_t tup5 = MEMCAP0_READVDATA1WITHVMASK1(buffer[5][bufferoffset_kvs + index/2]);
		vdatas[5] = tup5.A;
		vmdatas[5] = tup5.B;
		tuple_t tup6 = MEMCAP0_READVDATA1WITHVMASK1(buffer[6][bufferoffset_kvs + index/2]);
		vdatas[6] = tup6.A;
		vmdatas[6] = tup6.B;
		tuple_t tup7 = MEMCAP0_READVDATA1WITHVMASK1(buffer[7][bufferoffset_kvs + index/2]);
		vdatas[7] = tup7.A;
		vmdatas[7] = tup7.B;
		tuple_t tup8 = MEMCAP0_READVDATA1WITHVMASK1(buffer[8][bufferoffset_kvs + index/2]);
		vdatas[8] = tup8.A;
		vmdatas[8] = tup8.B;
		tuple_t tup9 = MEMCAP0_READVDATA1WITHVMASK1(buffer[9][bufferoffset_kvs + index/2]);
		vdatas[9] = tup9.A;
		vmdatas[9] = tup9.B;
		tuple_t tup10 = MEMCAP0_READVDATA1WITHVMASK1(buffer[10][bufferoffset_kvs + index/2]);
		vdatas[10] = tup10.A;
		vmdatas[10] = tup10.B;
		tuple_t tup11 = MEMCAP0_READVDATA1WITHVMASK1(buffer[11][bufferoffset_kvs + index/2]);
		vdatas[11] = tup11.A;
		vmdatas[11] = tup11.B;
		tuple_t tup12 = MEMCAP0_READVDATA1WITHVMASK1(buffer[12][bufferoffset_kvs + index/2]);
		vdatas[12] = tup12.A;
		vmdatas[12] = tup12.B;
		tuple_t tup13 = MEMCAP0_READVDATA1WITHVMASK1(buffer[13][bufferoffset_kvs + index/2]);
		vdatas[13] = tup13.A;
		vmdatas[13] = tup13.B;
		tuple_t tup14 = MEMCAP0_READVDATA1WITHVMASK1(buffer[14][bufferoffset_kvs + index/2]);
		vdatas[14] = tup14.A;
		vmdatas[14] = tup14.B;
		tuple_t tup15 = MEMCAP0_READVDATA1WITHVMASK1(buffer[15][bufferoffset_kvs + index/2]);
		vdatas[15] = tup15.A;
		vmdatas[15] = tup15.B;
	}
	return;
}

// used in {classname__top_nusrcv_nudstv.cpp->processit_splitdstvxs->MEMACCESSP0_SPL_readVchunks} 
void acts_all::MEMCAP0_READFROMBUFFER_VDATASANDVMASKS(unsigned int index, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], vmdata_t datas[VECTOR2_SIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("acts_all::MEMCAP0_READFROMBUFFER_VDATASANDVMASKS:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	if(index%2==0){
	
		datas[0] = MEMCAP0_READVDATA0ANDVMASK0(buffer[0][bufferoffset_kvs + index/2]);	
	
		datas[1] = MEMCAP0_READVDATA0ANDVMASK0(buffer[1][bufferoffset_kvs + index/2]);	
	
		datas[2] = MEMCAP0_READVDATA0ANDVMASK0(buffer[2][bufferoffset_kvs + index/2]);	
	
		datas[3] = MEMCAP0_READVDATA0ANDVMASK0(buffer[3][bufferoffset_kvs + index/2]);	
	
		datas[4] = MEMCAP0_READVDATA0ANDVMASK0(buffer[4][bufferoffset_kvs + index/2]);	
	
		datas[5] = MEMCAP0_READVDATA0ANDVMASK0(buffer[5][bufferoffset_kvs + index/2]);	
	
		datas[6] = MEMCAP0_READVDATA0ANDVMASK0(buffer[6][bufferoffset_kvs + index/2]);	
	
		datas[7] = MEMCAP0_READVDATA0ANDVMASK0(buffer[7][bufferoffset_kvs + index/2]);	
	
		datas[8] = MEMCAP0_READVDATA0ANDVMASK0(buffer[8][bufferoffset_kvs + index/2]);	
	
		datas[9] = MEMCAP0_READVDATA0ANDVMASK0(buffer[9][bufferoffset_kvs + index/2]);	
	
		datas[10] = MEMCAP0_READVDATA0ANDVMASK0(buffer[10][bufferoffset_kvs + index/2]);	
	
		datas[11] = MEMCAP0_READVDATA0ANDVMASK0(buffer[11][bufferoffset_kvs + index/2]);	
	
		datas[12] = MEMCAP0_READVDATA0ANDVMASK0(buffer[12][bufferoffset_kvs + index/2]);	
	
		datas[13] = MEMCAP0_READVDATA0ANDVMASK0(buffer[13][bufferoffset_kvs + index/2]);	
	
		datas[14] = MEMCAP0_READVDATA0ANDVMASK0(buffer[14][bufferoffset_kvs + index/2]);	
	
		datas[15] = MEMCAP0_READVDATA0ANDVMASK0(buffer[15][bufferoffset_kvs + index/2]);	
	} else{
	
		datas[0] = MEMCAP0_READVDATA1ANDVMASK1(buffer[0][bufferoffset_kvs + index/2]);	
	
		datas[1] = MEMCAP0_READVDATA1ANDVMASK1(buffer[1][bufferoffset_kvs + index/2]);	
	
		datas[2] = MEMCAP0_READVDATA1ANDVMASK1(buffer[2][bufferoffset_kvs + index/2]);	
	
		datas[3] = MEMCAP0_READVDATA1ANDVMASK1(buffer[3][bufferoffset_kvs + index/2]);	
	
		datas[4] = MEMCAP0_READVDATA1ANDVMASK1(buffer[4][bufferoffset_kvs + index/2]);	
	
		datas[5] = MEMCAP0_READVDATA1ANDVMASK1(buffer[5][bufferoffset_kvs + index/2]);	
	
		datas[6] = MEMCAP0_READVDATA1ANDVMASK1(buffer[6][bufferoffset_kvs + index/2]);	
	
		datas[7] = MEMCAP0_READVDATA1ANDVMASK1(buffer[7][bufferoffset_kvs + index/2]);	
	
		datas[8] = MEMCAP0_READVDATA1ANDVMASK1(buffer[8][bufferoffset_kvs + index/2]);	
	
		datas[9] = MEMCAP0_READVDATA1ANDVMASK1(buffer[9][bufferoffset_kvs + index/2]);	
	
		datas[10] = MEMCAP0_READVDATA1ANDVMASK1(buffer[10][bufferoffset_kvs + index/2]);	
	
		datas[11] = MEMCAP0_READVDATA1ANDVMASK1(buffer[11][bufferoffset_kvs + index/2]);	
	
		datas[12] = MEMCAP0_READVDATA1ANDVMASK1(buffer[12][bufferoffset_kvs + index/2]);	
	
		datas[13] = MEMCAP0_READVDATA1ANDVMASK1(buffer[13][bufferoffset_kvs + index/2]);	
	
		datas[14] = MEMCAP0_READVDATA1ANDVMASK1(buffer[14][bufferoffset_kvs + index/2]);	
	
		datas[15] = MEMCAP0_READVDATA1ANDVMASK1(buffer[15][bufferoffset_kvs + index/2]);	
	}
	return;
}
 
void acts_all::MEMCAP0_WRITETOBUFFER_VDATAWITHVMASK(unsigned int index, keyvalue_vbuffer_t buffer[BLOCKRAM_SIZE], value_t vdata, unit1_type vmdata, batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_WRITETOBUFFER_VDATAWITHVMASK:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	
	
	if(index%2==0){
		MEMCAP0_WRITEVDATA0WITHVMASK0(&buffer[bufferoffset_kvs + index/2], vdata, vmdata);
	} else{
		MEMCAP0_WRITEVDATA1WITHVMASK1(&buffer[bufferoffset_kvs + index/2], vdata, vmdata);
	}
	return;
}

void acts_all::MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS(unsigned int index, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t datas[VECTOR2_SIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("acts_all::MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	buffer[0][bufferoffset_kvs + index/2] = datas[0];
	buffer[1][bufferoffset_kvs + index/2] = datas[1];
	buffer[2][bufferoffset_kvs + index/2] = datas[2];
	buffer[3][bufferoffset_kvs + index/2] = datas[3];
	buffer[4][bufferoffset_kvs + index/2] = datas[4];
	buffer[5][bufferoffset_kvs + index/2] = datas[5];
	buffer[6][bufferoffset_kvs + index/2] = datas[6];
	buffer[7][bufferoffset_kvs + index/2] = datas[7];
	buffer[8][bufferoffset_kvs + index/2] = datas[8];
	buffer[9][bufferoffset_kvs + index/2] = datas[9];
	buffer[10][bufferoffset_kvs + index/2] = datas[10];
	buffer[11][bufferoffset_kvs + index/2] = datas[11];
	buffer[12][bufferoffset_kvs + index/2] = datas[12];
	buffer[13][bufferoffset_kvs + index/2] = datas[13];
	buffer[14][bufferoffset_kvs + index/2] = datas[14];
	buffer[15][bufferoffset_kvs + index/2] = datas[15];
	return;
}

void acts_all::MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS(unsigned int indexes[VDATA_PACKINGSIZE], keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VDATA_PACKINGSIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATAS:", bufferoffset_kvs + indexes[0]/2, BLOCKRAM_VDATA_SIZE, indexes[0], NAp, NAp);
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATAS:", bufferoffset_kvs + indexes[1]/2, BLOCKRAM_VDATA_SIZE, indexes[1], NAp, NAp);
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATAS:", bufferoffset_kvs + indexes[2]/2, BLOCKRAM_VDATA_SIZE, indexes[2], NAp, NAp);
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATAS:", bufferoffset_kvs + indexes[3]/2, BLOCKRAM_VDATA_SIZE, indexes[3], NAp, NAp);
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATAS:", bufferoffset_kvs + indexes[4]/2, BLOCKRAM_VDATA_SIZE, indexes[4], NAp, NAp);
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATAS:", bufferoffset_kvs + indexes[5]/2, BLOCKRAM_VDATA_SIZE, indexes[5], NAp, NAp);
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATAS:", bufferoffset_kvs + indexes[6]/2, BLOCKRAM_VDATA_SIZE, indexes[6], NAp, NAp);
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATAS:", bufferoffset_kvs + indexes[7]/2, BLOCKRAM_VDATA_SIZE, indexes[7], NAp, NAp);
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATAS:", bufferoffset_kvs + indexes[8]/2, BLOCKRAM_VDATA_SIZE, indexes[8], NAp, NAp);
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATAS:", bufferoffset_kvs + indexes[9]/2, BLOCKRAM_VDATA_SIZE, indexes[9], NAp, NAp);
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATAS:", bufferoffset_kvs + indexes[10]/2, BLOCKRAM_VDATA_SIZE, indexes[10], NAp, NAp);
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATAS:", bufferoffset_kvs + indexes[11]/2, BLOCKRAM_VDATA_SIZE, indexes[11], NAp, NAp);
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATAS:", bufferoffset_kvs + indexes[12]/2, BLOCKRAM_VDATA_SIZE, indexes[12], NAp, NAp);
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATAS:", bufferoffset_kvs + indexes[13]/2, BLOCKRAM_VDATA_SIZE, indexes[13], NAp, NAp);
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATAS:", bufferoffset_kvs + indexes[14]/2, BLOCKRAM_VDATA_SIZE, indexes[14], NAp, NAp);
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATAS:", bufferoffset_kvs + indexes[15]/2, BLOCKRAM_VDATA_SIZE, indexes[15], NAp, NAp);
	#endif
	
	buffer[0][bufferoffset_kvs + indexes[0]/2] = vdatas[0];
	buffer[1][bufferoffset_kvs + indexes[1]/2] = vdatas[1];
	buffer[2][bufferoffset_kvs + indexes[2]/2] = vdatas[2];
	buffer[3][bufferoffset_kvs + indexes[3]/2] = vdatas[3];
	buffer[4][bufferoffset_kvs + indexes[4]/2] = vdatas[4];
	buffer[5][bufferoffset_kvs + indexes[5]/2] = vdatas[5];
	buffer[6][bufferoffset_kvs + indexes[6]/2] = vdatas[6];
	buffer[7][bufferoffset_kvs + indexes[7]/2] = vdatas[7];
	buffer[8][bufferoffset_kvs + indexes[8]/2] = vdatas[8];
	buffer[9][bufferoffset_kvs + indexes[9]/2] = vdatas[9];
	buffer[10][bufferoffset_kvs + indexes[10]/2] = vdatas[10];
	buffer[11][bufferoffset_kvs + indexes[11]/2] = vdatas[11];
	buffer[12][bufferoffset_kvs + indexes[12]/2] = vdatas[12];
	buffer[13][bufferoffset_kvs + indexes[13]/2] = vdatas[13];
	buffer[14][bufferoffset_kvs + indexes[14]/2] = vdatas[14];
	buffer[15][bufferoffset_kvs + indexes[15]/2] = vdatas[15];
	
	return;
}

void acts_all::MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS1_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("acts_all::MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	buffer0[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer0[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer0[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer0[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer0[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer0[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer0[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer0[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer0[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer0[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer0[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer0[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer0[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer0[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer0[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer0[15][bufferoffset_kvs + index/2] = vdatas[15];
	return;
}
void acts_all::MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS2_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("acts_all::MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	buffer0[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer0[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer0[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer0[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer0[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer0[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer0[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer0[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer0[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer0[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer0[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer0[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer0[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer0[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer0[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer0[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer1[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer1[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer1[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer1[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer1[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer1[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer1[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer1[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer1[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer1[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer1[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer1[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer1[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer1[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer1[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer1[15][bufferoffset_kvs + index/2] = vdatas[15];
	return;
}
void acts_all::MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS3_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("acts_all::MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	buffer0[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer0[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer0[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer0[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer0[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer0[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer0[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer0[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer0[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer0[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer0[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer0[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer0[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer0[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer0[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer0[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer1[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer1[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer1[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer1[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer1[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer1[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer1[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer1[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer1[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer1[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer1[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer1[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer1[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer1[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer1[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer1[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer2[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer2[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer2[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer2[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer2[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer2[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer2[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer2[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer2[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer2[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer2[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer2[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer2[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer2[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer2[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer2[15][bufferoffset_kvs + index/2] = vdatas[15];
	return;
}
void acts_all::MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS4_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("acts_all::MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	buffer0[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer0[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer0[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer0[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer0[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer0[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer0[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer0[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer0[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer0[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer0[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer0[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer0[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer0[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer0[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer0[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer1[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer1[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer1[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer1[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer1[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer1[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer1[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer1[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer1[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer1[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer1[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer1[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer1[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer1[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer1[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer1[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer2[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer2[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer2[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer2[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer2[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer2[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer2[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer2[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer2[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer2[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer2[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer2[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer2[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer2[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer2[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer2[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer3[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer3[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer3[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer3[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer3[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer3[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer3[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer3[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer3[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer3[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer3[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer3[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer3[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer3[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer3[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer3[15][bufferoffset_kvs + index/2] = vdatas[15];
	return;
}
void acts_all::MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS5_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("acts_all::MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	buffer0[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer0[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer0[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer0[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer0[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer0[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer0[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer0[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer0[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer0[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer0[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer0[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer0[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer0[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer0[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer0[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer1[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer1[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer1[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer1[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer1[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer1[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer1[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer1[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer1[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer1[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer1[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer1[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer1[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer1[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer1[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer1[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer2[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer2[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer2[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer2[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer2[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer2[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer2[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer2[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer2[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer2[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer2[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer2[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer2[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer2[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer2[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer2[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer3[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer3[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer3[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer3[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer3[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer3[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer3[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer3[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer3[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer3[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer3[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer3[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer3[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer3[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer3[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer3[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer4[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer4[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer4[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer4[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer4[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer4[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer4[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer4[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer4[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer4[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer4[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer4[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer4[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer4[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer4[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer4[15][bufferoffset_kvs + index/2] = vdatas[15];
	return;
}
void acts_all::MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS6_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("acts_all::MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	buffer0[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer0[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer0[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer0[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer0[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer0[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer0[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer0[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer0[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer0[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer0[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer0[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer0[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer0[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer0[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer0[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer1[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer1[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer1[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer1[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer1[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer1[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer1[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer1[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer1[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer1[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer1[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer1[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer1[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer1[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer1[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer1[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer2[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer2[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer2[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer2[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer2[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer2[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer2[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer2[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer2[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer2[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer2[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer2[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer2[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer2[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer2[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer2[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer3[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer3[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer3[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer3[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer3[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer3[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer3[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer3[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer3[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer3[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer3[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer3[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer3[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer3[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer3[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer3[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer4[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer4[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer4[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer4[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer4[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer4[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer4[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer4[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer4[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer4[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer4[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer4[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer4[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer4[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer4[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer4[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer5[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer5[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer5[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer5[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer5[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer5[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer5[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer5[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer5[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer5[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer5[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer5[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer5[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer5[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer5[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer5[15][bufferoffset_kvs + index/2] = vdatas[15];
	return;
}
void acts_all::MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS7_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("acts_all::MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	buffer0[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer0[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer0[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer0[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer0[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer0[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer0[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer0[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer0[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer0[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer0[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer0[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer0[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer0[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer0[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer0[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer1[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer1[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer1[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer1[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer1[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer1[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer1[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer1[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer1[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer1[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer1[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer1[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer1[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer1[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer1[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer1[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer2[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer2[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer2[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer2[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer2[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer2[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer2[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer2[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer2[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer2[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer2[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer2[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer2[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer2[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer2[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer2[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer3[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer3[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer3[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer3[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer3[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer3[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer3[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer3[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer3[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer3[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer3[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer3[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer3[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer3[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer3[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer3[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer4[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer4[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer4[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer4[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer4[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer4[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer4[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer4[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer4[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer4[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer4[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer4[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer4[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer4[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer4[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer4[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer5[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer5[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer5[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer5[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer5[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer5[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer5[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer5[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer5[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer5[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer5[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer5[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer5[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer5[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer5[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer5[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer6[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer6[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer6[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer6[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer6[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer6[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer6[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer6[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer6[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer6[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer6[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer6[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer6[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer6[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer6[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer6[15][bufferoffset_kvs + index/2] = vdatas[15];
	return;
}
void acts_all::MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS8_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("acts_all::MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	buffer0[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer0[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer0[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer0[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer0[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer0[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer0[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer0[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer0[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer0[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer0[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer0[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer0[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer0[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer0[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer0[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer1[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer1[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer1[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer1[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer1[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer1[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer1[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer1[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer1[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer1[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer1[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer1[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer1[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer1[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer1[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer1[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer2[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer2[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer2[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer2[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer2[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer2[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer2[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer2[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer2[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer2[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer2[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer2[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer2[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer2[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer2[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer2[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer3[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer3[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer3[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer3[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer3[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer3[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer3[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer3[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer3[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer3[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer3[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer3[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer3[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer3[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer3[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer3[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer4[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer4[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer4[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer4[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer4[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer4[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer4[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer4[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer4[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer4[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer4[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer4[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer4[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer4[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer4[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer4[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer5[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer5[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer5[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer5[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer5[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer5[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer5[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer5[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer5[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer5[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer5[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer5[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer5[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer5[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer5[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer5[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer6[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer6[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer6[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer6[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer6[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer6[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer6[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer6[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer6[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer6[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer6[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer6[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer6[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer6[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer6[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer6[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer7[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer7[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer7[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer7[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer7[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer7[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer7[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer7[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer7[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer7[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer7[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer7[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer7[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer7[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer7[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer7[15][bufferoffset_kvs + index/2] = vdatas[15];
	return;
}
void acts_all::MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS9_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("acts_all::MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	buffer0[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer0[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer0[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer0[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer0[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer0[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer0[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer0[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer0[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer0[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer0[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer0[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer0[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer0[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer0[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer0[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer1[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer1[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer1[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer1[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer1[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer1[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer1[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer1[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer1[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer1[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer1[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer1[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer1[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer1[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer1[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer1[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer2[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer2[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer2[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer2[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer2[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer2[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer2[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer2[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer2[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer2[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer2[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer2[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer2[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer2[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer2[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer2[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer3[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer3[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer3[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer3[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer3[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer3[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer3[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer3[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer3[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer3[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer3[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer3[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer3[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer3[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer3[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer3[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer4[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer4[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer4[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer4[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer4[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer4[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer4[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer4[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer4[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer4[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer4[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer4[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer4[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer4[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer4[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer4[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer5[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer5[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer5[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer5[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer5[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer5[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer5[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer5[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer5[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer5[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer5[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer5[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer5[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer5[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer5[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer5[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer6[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer6[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer6[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer6[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer6[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer6[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer6[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer6[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer6[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer6[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer6[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer6[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer6[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer6[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer6[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer6[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer7[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer7[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer7[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer7[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer7[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer7[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer7[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer7[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer7[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer7[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer7[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer7[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer7[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer7[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer7[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer7[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer8[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer8[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer8[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer8[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer8[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer8[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer8[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer8[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer8[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer8[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer8[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer8[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer8[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer8[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer8[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer8[15][bufferoffset_kvs + index/2] = vdatas[15];
	return;
}
void acts_all::MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS10_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("acts_all::MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	buffer0[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer0[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer0[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer0[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer0[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer0[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer0[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer0[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer0[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer0[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer0[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer0[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer0[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer0[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer0[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer0[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer1[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer1[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer1[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer1[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer1[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer1[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer1[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer1[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer1[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer1[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer1[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer1[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer1[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer1[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer1[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer1[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer2[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer2[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer2[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer2[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer2[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer2[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer2[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer2[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer2[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer2[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer2[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer2[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer2[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer2[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer2[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer2[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer3[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer3[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer3[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer3[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer3[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer3[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer3[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer3[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer3[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer3[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer3[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer3[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer3[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer3[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer3[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer3[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer4[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer4[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer4[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer4[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer4[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer4[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer4[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer4[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer4[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer4[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer4[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer4[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer4[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer4[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer4[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer4[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer5[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer5[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer5[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer5[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer5[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer5[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer5[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer5[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer5[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer5[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer5[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer5[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer5[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer5[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer5[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer5[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer6[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer6[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer6[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer6[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer6[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer6[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer6[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer6[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer6[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer6[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer6[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer6[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer6[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer6[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer6[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer6[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer7[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer7[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer7[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer7[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer7[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer7[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer7[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer7[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer7[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer7[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer7[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer7[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer7[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer7[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer7[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer7[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer8[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer8[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer8[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer8[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer8[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer8[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer8[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer8[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer8[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer8[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer8[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer8[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer8[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer8[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer8[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer8[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer9[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer9[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer9[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer9[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer9[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer9[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer9[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer9[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer9[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer9[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer9[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer9[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer9[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer9[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer9[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer9[15][bufferoffset_kvs + index/2] = vdatas[15];
	return;
}
void acts_all::MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS11_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer10[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("acts_all::MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	buffer0[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer0[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer0[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer0[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer0[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer0[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer0[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer0[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer0[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer0[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer0[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer0[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer0[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer0[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer0[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer0[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer1[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer1[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer1[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer1[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer1[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer1[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer1[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer1[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer1[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer1[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer1[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer1[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer1[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer1[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer1[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer1[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer2[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer2[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer2[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer2[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer2[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer2[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer2[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer2[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer2[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer2[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer2[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer2[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer2[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer2[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer2[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer2[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer3[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer3[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer3[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer3[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer3[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer3[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer3[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer3[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer3[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer3[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer3[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer3[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer3[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer3[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer3[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer3[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer4[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer4[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer4[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer4[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer4[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer4[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer4[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer4[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer4[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer4[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer4[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer4[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer4[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer4[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer4[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer4[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer5[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer5[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer5[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer5[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer5[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer5[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer5[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer5[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer5[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer5[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer5[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer5[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer5[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer5[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer5[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer5[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer6[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer6[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer6[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer6[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer6[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer6[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer6[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer6[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer6[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer6[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer6[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer6[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer6[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer6[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer6[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer6[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer7[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer7[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer7[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer7[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer7[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer7[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer7[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer7[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer7[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer7[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer7[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer7[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer7[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer7[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer7[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer7[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer8[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer8[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer8[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer8[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer8[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer8[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer8[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer8[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer8[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer8[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer8[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer8[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer8[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer8[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer8[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer8[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer9[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer9[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer9[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer9[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer9[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer9[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer9[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer9[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer9[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer9[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer9[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer9[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer9[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer9[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer9[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer9[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer10[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer10[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer10[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer10[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer10[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer10[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer10[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer10[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer10[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer10[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer10[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer10[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer10[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer10[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer10[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer10[15][bufferoffset_kvs + index/2] = vdatas[15];
	return;
}
void acts_all::MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS12_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer10[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer11[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("acts_all::MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	buffer0[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer0[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer0[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer0[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer0[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer0[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer0[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer0[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer0[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer0[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer0[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer0[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer0[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer0[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer0[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer0[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer1[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer1[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer1[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer1[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer1[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer1[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer1[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer1[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer1[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer1[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer1[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer1[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer1[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer1[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer1[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer1[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer2[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer2[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer2[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer2[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer2[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer2[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer2[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer2[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer2[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer2[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer2[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer2[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer2[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer2[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer2[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer2[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer3[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer3[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer3[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer3[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer3[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer3[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer3[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer3[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer3[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer3[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer3[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer3[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer3[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer3[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer3[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer3[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer4[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer4[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer4[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer4[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer4[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer4[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer4[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer4[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer4[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer4[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer4[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer4[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer4[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer4[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer4[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer4[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer5[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer5[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer5[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer5[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer5[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer5[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer5[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer5[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer5[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer5[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer5[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer5[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer5[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer5[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer5[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer5[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer6[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer6[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer6[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer6[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer6[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer6[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer6[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer6[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer6[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer6[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer6[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer6[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer6[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer6[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer6[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer6[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer7[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer7[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer7[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer7[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer7[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer7[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer7[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer7[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer7[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer7[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer7[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer7[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer7[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer7[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer7[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer7[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer8[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer8[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer8[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer8[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer8[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer8[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer8[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer8[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer8[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer8[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer8[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer8[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer8[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer8[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer8[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer8[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer9[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer9[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer9[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer9[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer9[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer9[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer9[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer9[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer9[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer9[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer9[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer9[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer9[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer9[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer9[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer9[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer10[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer10[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer10[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer10[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer10[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer10[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer10[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer10[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer10[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer10[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer10[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer10[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer10[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer10[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer10[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer10[15][bufferoffset_kvs + index/2] = vdatas[15];
	buffer11[0][bufferoffset_kvs + index/2] = vdatas[0];
	buffer11[1][bufferoffset_kvs + index/2] = vdatas[1];
	buffer11[2][bufferoffset_kvs + index/2] = vdatas[2];
	buffer11[3][bufferoffset_kvs + index/2] = vdatas[3];
	buffer11[4][bufferoffset_kvs + index/2] = vdatas[4];
	buffer11[5][bufferoffset_kvs + index/2] = vdatas[5];
	buffer11[6][bufferoffset_kvs + index/2] = vdatas[6];
	buffer11[7][bufferoffset_kvs + index/2] = vdatas[7];
	buffer11[8][bufferoffset_kvs + index/2] = vdatas[8];
	buffer11[9][bufferoffset_kvs + index/2] = vdatas[9];
	buffer11[10][bufferoffset_kvs + index/2] = vdatas[10];
	buffer11[11][bufferoffset_kvs + index/2] = vdatas[11];
	buffer11[12][bufferoffset_kvs + index/2] = vdatas[12];
	buffer11[13][bufferoffset_kvs + index/2] = vdatas[13];
	buffer11[14][bufferoffset_kvs + index/2] = vdatas[14];
	buffer11[15][bufferoffset_kvs + index/2] = vdatas[15];
	return;
}
