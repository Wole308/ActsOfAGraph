// primitives
/* unsigned int acts_all::MEMCAP0_READVDATA(keyvalue_vbuffer_t wideword){
	#pragma HLS INLINE
	#ifdef CONFIG_BITMASK_NOT_USED
	return wideword;
	#else 
	return UTILP0_READBITSFROM_UINTV(wideword.data, OFFSETOF_VDATA, SIZEOF_VDATA);
	#endif 
}
unsigned int acts_all::MEMCAP0_READVMASK(keyvalue_vbuffer_t wideword){
	#pragma HLS INLINE
	#ifdef CONFIG_BITMASK_NOT_USED
	return 0;
	#else 
	return UTILP0_READBITSFROM_UINTV(wideword.data, OFFSETOF_VMASK, SIZEOF_VMASK);
	#endif 
}

void acts_all::MEMCAP0_WRITEVDATA(keyvalue_vbuffer_t * wideword, value_t vdata){
	#pragma HLS INLINE
	#ifdef CONFIG_BITMASK_NOT_USED
	*wideword = vdata;
	#else 
	UTILP0_WRITEBITSTO_UINTV(wideword.data, OFFSETOF_VDATA, SIZEOF_VDATA, vdata);
	#endif 
	return;
}
void acts_all::MEMCAP0_WRITEVMASK(keyvalue_vbuffer_t * wideword, unit1_type vmask){
	#pragma HLS INLINE
	#ifdef CONFIG_BITMASK_NOT_USED
	// NAp
	#else 
	UTILP0_WRITEBITSTO_UINTV(wideword.data, OFFSETOF_VMASK, SIZEOF_VMASK, vmask);
	#endif 
	return;
}

unsigned int acts_all::MEMCAP0_READEDIR(unsigned int wideword){
	#pragma HLS INLINE
	return UTILP0_READBITSFROM_UINTV(wideword, OFFSETOF_EDIR, SIZEOF_EDIR);
}

// non-primitives
vmdata_t acts_all::MEMCAP0_READFROMBUFFER_VDATAWITHVMASK(keyvalue_vbuffer_t bits_vector){
	#pragma HLS INLINE
	vmdata_t vmdata;
	vmdata.vdata = MEMCAP0_READVDATA(bits_vector); 
	vmdata.vmask = MEMCAP0_READVMASK(bits_vector); 
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
	if(vmdata.vmask == 1){ cout<<">>> MEMCAP0_READFROMBUFFER_VDATAWITHVMASK:: ACTIVE VERTEX READ: @ index: "<<index<<endl; }
	#endif
	return vmdata;
}
void acts_all::MEMCAP0_WRITETOBUFFER_VDATAWITHVMASK(unsigned int index, keyvalue_vbuffer_t buffer[MAX_BLOCKRAM_VDESTDATA_SIZE], batch_type bufferoffset_kvs, keyvalue_vbuffer_t bits_vector, value_t vdata, unit1_type vmask){				
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("MEMCAP0_WRITETOBUFFER_VDATAWITHVMASK:", index/2, MAX_BLOCKRAM_VDESTDATA_SIZE, index, NAp, NAp);
	#endif

	MEMCAP0_WRITEVDATA(&bits_vector, vdata);
	MEMCAP0_WRITEVMASK(&bits_vector, vmask);
	buffer[bufferoffset_kvs + index] = bits_vector;
	return;
}

 */

