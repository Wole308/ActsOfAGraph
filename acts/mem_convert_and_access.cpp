// primitives
unsigned int MEMCAP0_READVDATA(keyvalue_vbuffer_t wideword){
	#pragma HLS INLINE
	#ifdef CONFIG_BITMASK_NOT_USED
	return wideword;
	#else 
	return UTILP0_READBITSFROM_UINTV(wideword, OFFSETOF_VDATA, SIZEOF_VDATA);
	#endif 
}
unsigned int MEMCAP0_READVMASK(keyvalue_vbuffer_t wideword){
	#pragma HLS INLINE
	#ifdef CONFIG_BITMASK_NOT_USED
	return 0;
	#else 
	return UTILP0_READBITSFROM_UINTV(wideword, OFFSETOF_VMASK, SIZEOF_VMASK);
	#endif 
}
unsigned int MEMCAP0_READVDATA2(keyvalue_vbuffer_t wideword, unsigned int offsetof_vdata){
	#pragma HLS INLINE
	#ifdef CONFIG_BITMASK_NOT_USED
	return wideword;
	#else 
	return UTILP0_READBITSFROM_UINTV(wideword, offsetof_vdata, SIZEOF_VDATA);
	#endif 
}
unsigned int MEMCAP0_READVMASK2(keyvalue_vbuffer_t wideword, unsigned int offsetof_vmask){
	#pragma HLS INLINE
	#ifdef CONFIG_BITMASK_NOT_USED
	return 0;
	#else 
	return UTILP0_READBITSFROM_UINTV(wideword, offsetof_vmask, SIZEOF_VMASK);
	#endif 
}

void MEMCAP0_WRITEVDATA(keyvalue_vbuffer_t * wideword, value_t vdata){
	#pragma HLS INLINE
	#ifdef CONFIG_BITMASK_NOT_USED
	*wideword = vdata;
	#else 
	UTILP0_WRITEBITSTO_UINTV(wideword, OFFSETOF_VDATA, SIZEOF_VDATA, vdata);
	#endif 
	return;
}
void MEMCAP0_WRITEVMASK(keyvalue_vbuffer_t * wideword, unit1_type vmask){
	#pragma HLS INLINE
	#ifdef CONFIG_BITMASK_NOT_USED
	// NAp
	#else 
	UTILP0_WRITEBITSTO_UINTV(wideword, OFFSETOF_VMASK, SIZEOF_VMASK, vmask);
	#endif 
	return;
}
void MEMCAP0_WRITEVDATA2(keyvalue_vbuffer_t * wideword, value_t vdata, unsigned int offsetof_vdata){
	#pragma HLS INLINE
	#ifdef CONFIG_BITMASK_NOT_USED
	*wideword = vdata;
	#else 
	UTILP0_WRITEBITSTO_UINTV(wideword, offsetof_vdata, SIZEOF_VDATA, vdata);
	#endif 
	return;
}
void MEMCAP0_WRITEVMASK2(keyvalue_vbuffer_t * wideword, unit1_type vmask, unsigned int offsetof_vmask){
	#pragma HLS INLINE
	#ifdef CONFIG_BITMASK_NOT_USED
	// NAp
	#else 
	UTILP0_WRITEBITSTO_UINTV(wideword, offsetof_vmask, SIZEOF_VMASK, vmask);
	#endif 
	return;
}

unsigned int MEMCAP0_READEDIR(unsigned int wideword){
	#pragma HLS INLINE
	return UTILP0_READBITSFROM_UINTV(wideword, OFFSETOF_EDIR, SIZEOF_EDIR);
}

// non-primitives
vmdata_t MEMCAP0_READFROMBUFFER_VDATAWITHVMASK(keyvalue_vbuffer_t bits_vector){
	#pragma HLS INLINE
	vmdata_t vmdata;
	vmdata.vdata = MEMCAP0_READVDATA(bits_vector); 
	vmdata.vmask = MEMCAP0_READVMASK(bits_vector); 
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
	if(vmdata.vmask == 1){ cout<<">>> MEMCAP0_READFROMBUFFER_VDATAWITHVMASK:: ACTIVE VERTEX READ: @ index: "<<index<<endl; }
	#endif
	return vmdata;
}
void MEMCAP0_WRITETOBUFFER_VDATAWITHVMASK(unsigned int index, keyvalue_vbuffer_t buffer[MAX_BLOCKRAM_VDESTDATA_SIZE], batch_type bufferoffset_kvs, keyvalue_vbuffer_t bits_vector, value_t vdata, unit1_type vmask){				
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("MEMCAP0_WRITETOBUFFER_VDATAWITHVMASK:", index/2, MAX_BLOCKRAM_VDESTDATA_SIZE, index, NAp, NAp);
	#endif

	MEMCAP0_WRITEVDATA(&bits_vector, vdata);
	MEMCAP0_WRITEVMASK(&bits_vector, vmask);
	buffer[bufferoffset_kvs + index] = bits_vector;
	return;
}
void MEMCAP0_WRITETODRAM_VDATAWITHVMASK(unsigned int index, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t bits_vector, value_t vdata, unit1_type vmask){
	#pragma HLS INLINE
	// {1st 16 is masks}{2nd 16 is vdatas}
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("MEMCAP0_WRITETODRAM_VDATAWITHVMASK:", index, MAX_TOTALDRAMCAPACITY_KVS, index, NAp, NAp);
	#endif

	MEMCAP0_WRITEVDATA(&bits_vector, vdata);
	MEMCAP0_WRITEVMASK(&bits_vector, vmask);
	UTILP0_SetData(kvdram, dramoffset_kvs, (index / VDATA_SHRINK_RATIO), bits_vector);
	return;
}

vmdata_t MEMCAP0_READFROMBUFFER_VDATAWITHVMASK2(unsigned int index, keyvalue_vbuffer_t bits_vector){
	#pragma HLS INLINE
	// {1st 16 is masks}{2nd 16 is vdatas}
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("MEMCAP0_READFROMBUFFER_VDATAWITHVMASK2(220):", index % VDATA_SHRINK_RATIO, 32, index, NAp, NAp);
	#endif
	
	vmdata_t vmdata;
	tuple_t tup;
	unsigned int offsetof_vdata = (index % VDATA_SHRINK_RATIO);
	
	#ifdef HWBITACCESSTYPE
	tup.A = MEMCAP0_READVDATA2(bits_vector, offsetof_vdata); 
	tup.B = MEMCAP0_READVMASK2(bits_vector, BEGINOFFSETOF_DEST_VMASK + offsetof_vdata); 
	vmdata.vdata = tup.A;
	vmdata.vmask = tup.B;
	#else
	vmdata.vdata = UTILP0_SWREADBITSFROM_UINTV(bits_vector, offsetof_vdata, SIZEOF_VDATA);
	vmdata.vmask = UTILP0_SWREADBITSFROM_UINTV(bits_vector, BEGINOFFSETOF_DEST_VMASK + offsetof_vdata, SIZEOF_DEST_VMASK);
	#endif 
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	if(vmdata.vmask == 1){ cout<<">>> MEMCAP0_VDATAWITHVMASK2(224):: ACTIVE VERTEX READ: @ offsetof_vdata: "<<offsetof_vdata<<", vmdata.vdata: "<<vmdata.vdata<<", vmdata.vmask: "<<vmdata.vmask<<", sub_chunk_height: "<<(index / VDATA_SHRINK_RATIO)<<", index: "<<index<<endl; }						
	#endif
	return vmdata;
}
void MEMCAP0_WRITETOBUFFER_VDATAWITHVMASK2(unsigned int index, keyvalue_vbuffer_t buffer[MAX_BLOCKRAM_VDESTDATA_SIZE], batch_type bufferoffset_kvs, keyvalue_vbuffer_t bits_vector, value_t vdata, unit1_type vmask){
	#pragma HLS INLINE
	// {1st 16 is masks}{2nd 16 is vdatas}
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("MEMCAP0_WRITETOBUFFER_VDATAWITHVMASK2:", index/VDATA_SHRINK_RATIO, MAX_BLOCKRAM_VDESTDATA_SIZE, index, NAp, NAp);
	#endif

	unsigned int offsetof_vdata = (index % VDATA_SHRINK_RATIO);
	// keyvalue_vbuffer_t bits_vector = buffer[bufferoffset_kvs + (index / VDATA_SHRINK_RATIO)];
	
	#ifdef HWBITACCESSTYPE
	MEMCAP0_WRITEVDATA2(&bits_vector, vdata, offsetof_vdata);
	MEMCAP0_WRITEVMASK2(&bits_vector, vmdata, BEGINOFFSETOF_DEST_VMASK + offsetof_vdata);
	#else
	UTILP0_SWWRITEBITSTO_UINTV(&bits_vector, offsetof_vdata, SIZEOF_VDATA, vdata);
	UTILP0_SWWRITEBITSTO_UINTV(&bits_vector, BEGINOFFSETOF_DEST_VMASK + offsetof_vdata, SIZEOF_DEST_VMASK, vmask);	
	#endif 
	
	buffer[bufferoffset_kvs + (index / VDATA_SHRINK_RATIO)] = bits_vector;
	return;
}
void MEMCAP0_WRITETODRAM_VDATAWITHVMASK2(unsigned int index, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t bits_vector, value_t vdata, unit1_type vmask){
	#pragma HLS INLINE
	// {1st 16 is masks}{2nd 16 is vdatas}
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("MEMCAP0_WRITETODRAM_VDATAWITHVMASK2:", index/VDATA_SHRINK_RATIO, MAX_TOTALDRAMCAPACITY_KVS, index, NAp, NAp);
	#endif

	unsigned int offsetof_vdata = (index % VDATA_SHRINK_RATIO);
	
	#ifdef HWBITACCESSTYPE
	MEMCAP0_WRITEVDATA2(&bits_vector, vdata, offsetof_vdata);
	MEMCAP0_WRITEVMASK2(&bits_vector, vmask, BEGINOFFSETOF_VMASK + offsetof_vdata);
	#else
	UTILP0_SWWRITEBITSTO_UINTV(&bits_vector, offsetof_vdata, SIZEOF_VDATA, vdata);
	UTILP0_SWWRITEBITSTO_UINTV(&bits_vector, BEGINOFFSETOF_VMASK + offsetof_vdata, SIZEOF_VMASK, vmask);	
	#endif 
	
	UTILP0_SetData(kvdram, dramoffset_kvs, (index / VDATA_SHRINK_RATIO), bits_vector);
	return;
}



