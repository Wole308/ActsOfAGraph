// primitives
unsigned int MEMCAP0_READVDATA(keyvalue_vbuffer_t wideword){
	#pragma HLS INLINE
	return UTILP0_READBITSFROM_UINTV(wideword, OFFSETOF_VDATA, SIZEOF_VDATA);
}
unsigned int MEMCAP0_READVMASK(keyvalue_vbuffer_t wideword){
	#pragma HLS INLINE
	return UTILP0_READBITSFROM_UINTV(wideword, OFFSETOF_VMASK, SIZEOF_VMASK);
}
vmdata_t MEMCAP0_READVDATAANDVMASK(keyvalue_vbuffer_t wideword){
	#pragma HLS INLINE
	vmdata_t vmdata;
	vmdata.vdata = UTILP0_READBITSFROM_UINTV(wideword, OFFSETOF_VDATA, SIZEOF_VDATA);
	vmdata.vmask = UTILP0_READBITSFROM_UINTV(wideword, OFFSETOF_VMASK, SIZEOF_VMASK);
	return vmdata;
}

void MEMCAP0_WRITEVDATA(keyvalue_vbuffer_t * wideword, value_t vdata){
	#pragma HLS INLINE
	UTILP0_WRITEBITSTO_UINTV(wideword, OFFSETOF_VDATA, SIZEOF_VDATA, vdata);
	return;
}
void MEMCAP0_WRITEVMASK(keyvalue_vbuffer_t * wideword, unit1_type vmask){
	#pragma HLS INLINE
	UTILP0_WRITEBITSTO_UINTV(wideword, OFFSETOF_VDATA, SIZEOF_VMASK, vmask);
	return;
}

tuple_t MEMCAP0_READVDATAWITHVMASK(keyvalue_vbuffer_t wideword){
	#pragma HLS INLINE
	tuple_t res;
	#ifdef _WIDEWORD
	res.A = wideword.range(OFFSETOF_VDATA + SIZEOF_VDATA - 1, OFFSETOF_VDATA); 
	res.B = wideword.range(OFFSETOF_VMASK + SIZEOF_VMASK - 1, OFFSETOF_VMASK); 
	#else 
	res.A = MEMCAP0_READVDATA(wideword); 
	res.B = MEMCAP0_READVMASK(wideword); 
	#endif 
	return res;
}
void MEMCAP0_WRITEVDATAWITHVMASK(keyvalue_vbuffer_t * wideword, value_t vdata, unit1_type vmask){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	wideword->range(OFFSETOF_VDATA + SIZEOF_VDATA - 1, OFFSETOF_VDATA) = vdata; 
	wideword->range(OFFSETOF_VMASK + SIZEOF_VMASK - 1, OFFSETOF_VMASK) = vmask; 
	#else 
	MEMCAP0_WRITEVDATA(wideword, vdata);
	MEMCAP0_WRITEVMASK(wideword, vmask);
	#endif 
	return;
}

// non-primitives
// any data 
uint32_type MEMCAP0_READDATAFROMBUFFER(unsigned int index, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_READDATAFROMBUFFER:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	return buffer[index % VECTOR2_SIZE][bufferoffset_kvs + (index / VECTOR2_SIZE)];
}

void MEMCAP0_WRITEDATATOBUFFER(unsigned int index, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], uint32_type data, batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_WRITEDATATOBUFFER:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	buffer[index % VECTOR2_SIZE][bufferoffset_kvs + (index / VECTOR2_SIZE)] = data;
	return;
}

keyvalue_vbuffer_t MEMCAP0_READDATAFROMDRAM(unsigned int index, uint512_dt * kvdram, batch_type baseoffset_kvs, batch_type offset_kvs){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("MEMCAP0_READDATAFROMDRAM:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	keyvalue_vbuffer_t data[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=data complete
	
	#ifdef _WIDEWORD // CRITICAL FIXME.
	data[0] = kvdram[baseoffset_kvs + offset_kvs + (index / VECTOR2_SIZE)].range(31, 0); 
	data[1] = kvdram[baseoffset_kvs + offset_kvs + (index / VECTOR2_SIZE)].range(63, 32); 
	data[2] = kvdram[baseoffset_kvs + offset_kvs + (index / VECTOR2_SIZE)].range(95, 64); 
	data[3] = kvdram[baseoffset_kvs + offset_kvs + (index / VECTOR2_SIZE)].range(127, 96); 
	data[4] = kvdram[baseoffset_kvs + offset_kvs + (index / VECTOR2_SIZE)].range(159, 128); 
	data[5] = kvdram[baseoffset_kvs + offset_kvs + (index / VECTOR2_SIZE)].range(191, 160); 
	data[6] = kvdram[baseoffset_kvs + offset_kvs + (index / VECTOR2_SIZE)].range(223, 192); 
	data[7] = kvdram[baseoffset_kvs + offset_kvs + (index / VECTOR2_SIZE)].range(255, 224); 
	data[8] = kvdram[baseoffset_kvs + offset_kvs + (index / VECTOR2_SIZE)].range(287, 256); 
	data[9] = kvdram[baseoffset_kvs + offset_kvs + (index / VECTOR2_SIZE)].range(319, 288); 
	data[10] = kvdram[baseoffset_kvs + offset_kvs + (index / VECTOR2_SIZE)].range(351, 320); 
	data[11] = kvdram[baseoffset_kvs + offset_kvs + (index / VECTOR2_SIZE)].range(383, 352); 
	data[12] = kvdram[baseoffset_kvs + offset_kvs + (index / VECTOR2_SIZE)].range(415, 384); 
	data[13] = kvdram[baseoffset_kvs + offset_kvs + (index / VECTOR2_SIZE)].range(447, 416); 
	data[14] = kvdram[baseoffset_kvs + offset_kvs + (index / VECTOR2_SIZE)].range(479, 448); 
	data[15] = kvdram[baseoffset_kvs + offset_kvs + (index / VECTOR2_SIZE)].range(511, 480); 
	#else 
	data[0] = kvdram[baseoffset_kvs + offset_kvs + (index / VECTOR2_SIZE)].data[0].key;
	data[1] = kvdram[baseoffset_kvs + offset_kvs + (index / VECTOR2_SIZE)].data[0].value; 
	data[2] = kvdram[baseoffset_kvs + offset_kvs + (index / VECTOR2_SIZE)].data[1].key;
	data[3] = kvdram[baseoffset_kvs + offset_kvs + (index / VECTOR2_SIZE)].data[1].value; 
	data[4] = kvdram[baseoffset_kvs + offset_kvs + (index / VECTOR2_SIZE)].data[2].key;
	data[5] = kvdram[baseoffset_kvs + offset_kvs + (index / VECTOR2_SIZE)].data[2].value; 
	data[6] = kvdram[baseoffset_kvs + offset_kvs + (index / VECTOR2_SIZE)].data[3].key;
	data[7] = kvdram[baseoffset_kvs + offset_kvs + (index / VECTOR2_SIZE)].data[3].value; 
	data[8] = kvdram[baseoffset_kvs + offset_kvs + (index / VECTOR2_SIZE)].data[4].key;
	data[9] = kvdram[baseoffset_kvs + offset_kvs + (index / VECTOR2_SIZE)].data[4].value; 
	data[10] = kvdram[baseoffset_kvs + offset_kvs + (index / VECTOR2_SIZE)].data[5].key;
	data[11] = kvdram[baseoffset_kvs + offset_kvs + (index / VECTOR2_SIZE)].data[5].value; 
	data[12] = kvdram[baseoffset_kvs + offset_kvs + (index / VECTOR2_SIZE)].data[6].key;
	data[13] = kvdram[baseoffset_kvs + offset_kvs + (index / VECTOR2_SIZE)].data[6].value; 
	data[14] = kvdram[baseoffset_kvs + offset_kvs + (index / VECTOR2_SIZE)].data[7].key;
	data[15] = kvdram[baseoffset_kvs + offset_kvs + (index / VECTOR2_SIZE)].data[7].value; 
	#endif
	
	return data[index % VECTOR2_SIZE];
}

void MEMCAP0_WRITEDATATODRAM(unsigned int index, uint512_dt * kvdram, uint32_type data, batch_type baseoffset_kvs, batch_type offset_kvs){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("MEMCAP0_WRITEDATATODRAM:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif

 if((index % VECTOR2_SIZE) == 0){
		#ifdef _WIDEWORD
		kvdram[offset_kvs + (index/VECTOR2_SIZE)].range(31, 0) = data;
		#else 
		if((index % VECTOR2_SIZE) % 2 == 0){ kvdram[offset_kvs + (index/VECTOR2_SIZE)].data[0].key = data; } 
		else { kvdram[offset_kvs + (index/VECTOR2_SIZE)].data[0].value = data;  }
		#endif 
	}
else if((index % VECTOR2_SIZE) == 1){
		#ifdef _WIDEWORD
		kvdram[offset_kvs + (index/VECTOR2_SIZE)].range(63, 32) = data;
		#else 
		if((index % VECTOR2_SIZE) % 2 == 0){ kvdram[offset_kvs + (index/VECTOR2_SIZE)].data[1].key = data; } 
		else { kvdram[offset_kvs + (index/VECTOR2_SIZE)].data[1].value = data;  }
		#endif 
	}
else if((index % VECTOR2_SIZE) == 2){
		#ifdef _WIDEWORD
		kvdram[offset_kvs + (index/VECTOR2_SIZE)].range(95, 64) = data;
		#else 
		if((index % VECTOR2_SIZE) % 2 == 0){ kvdram[offset_kvs + (index/VECTOR2_SIZE)].data[2].key = data; } 
		else { kvdram[offset_kvs + (index/VECTOR2_SIZE)].data[2].value = data;  }
		#endif 
	}
else if((index % VECTOR2_SIZE) == 3){
		#ifdef _WIDEWORD
		kvdram[offset_kvs + (index/VECTOR2_SIZE)].range(127, 96) = data;
		#else 
		if((index % VECTOR2_SIZE) % 2 == 0){ kvdram[offset_kvs + (index/VECTOR2_SIZE)].data[3].key = data; } 
		else { kvdram[offset_kvs + (index/VECTOR2_SIZE)].data[3].value = data;  }
		#endif 
	}
else if((index % VECTOR2_SIZE) == 4){
		#ifdef _WIDEWORD
		kvdram[offset_kvs + (index/VECTOR2_SIZE)].range(159, 128) = data;
		#else 
		if((index % VECTOR2_SIZE) % 2 == 0){ kvdram[offset_kvs + (index/VECTOR2_SIZE)].data[4].key = data; } 
		else { kvdram[offset_kvs + (index/VECTOR2_SIZE)].data[4].value = data;  }
		#endif 
	}
else if((index % VECTOR2_SIZE) == 5){
		#ifdef _WIDEWORD
		kvdram[offset_kvs + (index/VECTOR2_SIZE)].range(191, 160) = data;
		#else 
		if((index % VECTOR2_SIZE) % 2 == 0){ kvdram[offset_kvs + (index/VECTOR2_SIZE)].data[5].key = data; } 
		else { kvdram[offset_kvs + (index/VECTOR2_SIZE)].data[5].value = data;  }
		#endif 
	}
else if((index % VECTOR2_SIZE) == 6){
		#ifdef _WIDEWORD
		kvdram[offset_kvs + (index/VECTOR2_SIZE)].range(223, 192) = data;
		#else 
		if((index % VECTOR2_SIZE) % 2 == 0){ kvdram[offset_kvs + (index/VECTOR2_SIZE)].data[6].key = data; } 
		else { kvdram[offset_kvs + (index/VECTOR2_SIZE)].data[6].value = data;  }
		#endif 
	}
else if((index % VECTOR2_SIZE) == 7){
		#ifdef _WIDEWORD
		kvdram[offset_kvs + (index/VECTOR2_SIZE)].range(255, 224) = data;
		#else 
		if((index % VECTOR2_SIZE) % 2 == 0){ kvdram[offset_kvs + (index/VECTOR2_SIZE)].data[7].key = data; } 
		else { kvdram[offset_kvs + (index/VECTOR2_SIZE)].data[7].value = data;  }
		#endif 
	}
else if((index % VECTOR2_SIZE) == 8){
		#ifdef _WIDEWORD
		kvdram[offset_kvs + (index/VECTOR2_SIZE)].range(287, 256) = data;
		#else 
		if((index % VECTOR2_SIZE) % 2 == 0){ kvdram[offset_kvs + (index/VECTOR2_SIZE)].data[8].key = data; } 
		else { kvdram[offset_kvs + (index/VECTOR2_SIZE)].data[8].value = data;  }
		#endif 
	}
else if((index % VECTOR2_SIZE) == 9){
		#ifdef _WIDEWORD
		kvdram[offset_kvs + (index/VECTOR2_SIZE)].range(319, 288) = data;
		#else 
		if((index % VECTOR2_SIZE) % 2 == 0){ kvdram[offset_kvs + (index/VECTOR2_SIZE)].data[9].key = data; } 
		else { kvdram[offset_kvs + (index/VECTOR2_SIZE)].data[9].value = data;  }
		#endif 
	}
else if((index % VECTOR2_SIZE) == 10){
		#ifdef _WIDEWORD
		kvdram[offset_kvs + (index/VECTOR2_SIZE)].range(351, 320) = data;
		#else 
		if((index % VECTOR2_SIZE) % 2 == 0){ kvdram[offset_kvs + (index/VECTOR2_SIZE)].data[10].key = data; } 
		else { kvdram[offset_kvs + (index/VECTOR2_SIZE)].data[10].value = data;  }
		#endif 
	}
else if((index % VECTOR2_SIZE) == 11){
		#ifdef _WIDEWORD
		kvdram[offset_kvs + (index/VECTOR2_SIZE)].range(383, 352) = data;
		#else 
		if((index % VECTOR2_SIZE) % 2 == 0){ kvdram[offset_kvs + (index/VECTOR2_SIZE)].data[11].key = data; } 
		else { kvdram[offset_kvs + (index/VECTOR2_SIZE)].data[11].value = data;  }
		#endif 
	}
else if((index % VECTOR2_SIZE) == 12){
		#ifdef _WIDEWORD
		kvdram[offset_kvs + (index/VECTOR2_SIZE)].range(415, 384) = data;
		#else 
		if((index % VECTOR2_SIZE) % 2 == 0){ kvdram[offset_kvs + (index/VECTOR2_SIZE)].data[12].key = data; } 
		else { kvdram[offset_kvs + (index/VECTOR2_SIZE)].data[12].value = data;  }
		#endif 
	}
else if((index % VECTOR2_SIZE) == 13){
		#ifdef _WIDEWORD
		kvdram[offset_kvs + (index/VECTOR2_SIZE)].range(447, 416) = data;
		#else 
		if((index % VECTOR2_SIZE) % 2 == 0){ kvdram[offset_kvs + (index/VECTOR2_SIZE)].data[13].key = data; } 
		else { kvdram[offset_kvs + (index/VECTOR2_SIZE)].data[13].value = data;  }
		#endif 
	}
else if((index % VECTOR2_SIZE) == 14){
		#ifdef _WIDEWORD
		kvdram[offset_kvs + (index/VECTOR2_SIZE)].range(479, 448) = data;
		#else 
		if((index % VECTOR2_SIZE) % 2 == 0){ kvdram[offset_kvs + (index/VECTOR2_SIZE)].data[14].key = data; } 
		else { kvdram[offset_kvs + (index/VECTOR2_SIZE)].data[14].value = data;  }
		#endif 
	}
else if((index % VECTOR2_SIZE) == 15){
		#ifdef _WIDEWORD
		kvdram[offset_kvs + (index/VECTOR2_SIZE)].range(511, 480) = data;
		#else 
		if((index % VECTOR2_SIZE) % 2 == 0){ kvdram[offset_kvs + (index/VECTOR2_SIZE)].data[15].key = data; } 
		else { kvdram[offset_kvs + (index/VECTOR2_SIZE)].data[15].value = data;  }
		#endif 
	}
	else {
		#ifdef _DEBUGMODE_CHECKS3
		cout<<"MEMCAP0_WRITEDATATODRAM: NOT IMPLEMENTED. EXITING..."<<endl;
		exit(EXIT_FAILURE);
		#endif 
	}
	return;
}

// vdata 
// vdata:: used in {reduceupdates.cpp} 
void MEMCAP0_WRITETOBUFFER_VDATA(unsigned int index, keyvalue_vbuffer_t buffer[BLOCKRAM_VDATA_SIZE], value_t vdata, batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_WRITETOBUFFER_VDATA:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	MEMCAP0_WRITEVDATA(&buffer[bufferoffset_kvs + index], vdata);
	return;
}

// vdata:: used in {dispatch_reduce -> mem_access_splitdstvxs.cpp -> MEMACCESSP0_readV} 
void MEMCAP0_WRITETOBUFFER_VDATAS(unsigned int index, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], value_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("MEMCAP0_WRITETOBUFFER_VDATAS:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	MEMCAP0_WRITEVDATAWITHVMASK(&buffer[0][bufferoffset_kvs + index], vdatas[0], 0);
	MEMCAP0_WRITEVDATAWITHVMASK(&buffer[1][bufferoffset_kvs + index], vdatas[1], 0);
	MEMCAP0_WRITEVDATAWITHVMASK(&buffer[2][bufferoffset_kvs + index], vdatas[2], 0);
	MEMCAP0_WRITEVDATAWITHVMASK(&buffer[3][bufferoffset_kvs + index], vdatas[3], 0);
	MEMCAP0_WRITEVDATAWITHVMASK(&buffer[4][bufferoffset_kvs + index], vdatas[4], 0);
	MEMCAP0_WRITEVDATAWITHVMASK(&buffer[5][bufferoffset_kvs + index], vdatas[5], 0);
	MEMCAP0_WRITEVDATAWITHVMASK(&buffer[6][bufferoffset_kvs + index], vdatas[6], 0);
	MEMCAP0_WRITEVDATAWITHVMASK(&buffer[7][bufferoffset_kvs + index], vdatas[7], 0);
	MEMCAP0_WRITEVDATAWITHVMASK(&buffer[8][bufferoffset_kvs + index], vdatas[8], 0);
	MEMCAP0_WRITEVDATAWITHVMASK(&buffer[9][bufferoffset_kvs + index], vdatas[9], 0);
	MEMCAP0_WRITEVDATAWITHVMASK(&buffer[10][bufferoffset_kvs + index], vdatas[10], 0);
	MEMCAP0_WRITEVDATAWITHVMASK(&buffer[11][bufferoffset_kvs + index], vdatas[11], 0);
	MEMCAP0_WRITEVDATAWITHVMASK(&buffer[12][bufferoffset_kvs + index], vdatas[12], 0);
	MEMCAP0_WRITEVDATAWITHVMASK(&buffer[13][bufferoffset_kvs + index], vdatas[13], 0);
	MEMCAP0_WRITEVDATAWITHVMASK(&buffer[14][bufferoffset_kvs + index], vdatas[14], 0);
	MEMCAP0_WRITEVDATAWITHVMASK(&buffer[15][bufferoffset_kvs + index], vdatas[15], 0);
	return;
}

// vdata:: used in {reduceupdates.cpp, processedges_splitdstvxs.cpp} 
value_t MEMCAP0_READFROMBUFFER_VDATA(unsigned int index, keyvalue_vbuffer_t buffer[BLOCKRAM_VDATA_SIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("MEMCAP0_READFROMBUFFER_VDATA:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	value_t vdata = MEMCAP0_READVDATA(buffer[bufferoffset_kvs + index]);
	return vdata;
}

// vdata:: used in {processedges_splitdstvxs.cpp} // soon obsolete 
void MEMCAP0_READFROMBUFFER_VDATAS(unsigned int index, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], value_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("MEMCAP0_READFROMBUFFER_VDATAS:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	vdatas[0] = MEMCAP0_READVDATA(buffer[0][bufferoffset_kvs + index]);
	vdatas[1] = MEMCAP0_READVDATA(buffer[1][bufferoffset_kvs + index]);
	vdatas[2] = MEMCAP0_READVDATA(buffer[2][bufferoffset_kvs + index]);
	vdatas[3] = MEMCAP0_READVDATA(buffer[3][bufferoffset_kvs + index]);
	vdatas[4] = MEMCAP0_READVDATA(buffer[4][bufferoffset_kvs + index]);
	vdatas[5] = MEMCAP0_READVDATA(buffer[5][bufferoffset_kvs + index]);
	vdatas[6] = MEMCAP0_READVDATA(buffer[6][bufferoffset_kvs + index]);
	vdatas[7] = MEMCAP0_READVDATA(buffer[7][bufferoffset_kvs + index]);
	vdatas[8] = MEMCAP0_READVDATA(buffer[8][bufferoffset_kvs + index]);
	vdatas[9] = MEMCAP0_READVDATA(buffer[9][bufferoffset_kvs + index]);
	vdatas[10] = MEMCAP0_READVDATA(buffer[10][bufferoffset_kvs + index]);
	vdatas[11] = MEMCAP0_READVDATA(buffer[11][bufferoffset_kvs + index]);
	vdatas[12] = MEMCAP0_READVDATA(buffer[12][bufferoffset_kvs + index]);
	vdatas[13] = MEMCAP0_READVDATA(buffer[13][bufferoffset_kvs + index]);
	vdatas[14] = MEMCAP0_READVDATA(buffer[14][bufferoffset_kvs + index]);
	vdatas[15] = MEMCAP0_READVDATA(buffer[15][bufferoffset_kvs + index]);
	return;
}

// vdata & vmasks 
// used in {classname__processedges_splitdstvxs.cpp} 
vmdata_t MEMCAP0_READFROMBUFFER_VDATAWITHVMASK(unsigned int index, keyvalue_vbuffer_t buffer[BLOCKRAM_VDATA_SIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("MEMCAP0_READFROMBUFFER_VDATASWITHVMASKS:", bufferoffset_kvs + index/2, BLOCKRAM_VDATA_SIZE, index, NAp, NAp);
	#endif
	
	vmdata_t vmdata;
	tuple_t tup;
	tup = MEMCAP0_READVDATAWITHVMASK(buffer[bufferoffset_kvs + index]);
	vmdata.vdata = tup.A;
	vmdata.vmask = tup.B;
	return vmdata;
}

void MEMCAP0_READFROMBUFFER_VDATASWITHVMASKS(unsigned int index, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], value_t vdatas[VECTOR2_SIZE], unit1_type vmdatas[VDATA_PACKINGSIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("MEMCAP0_READFROMBUFFER_VDATASWITHVMASKS:", bufferoffset_kvs + index/2, BLOCKRAM_VDATA_SIZE, index, NAp, NAp);
	#endif
	
	tuple_t tup0 = MEMCAP0_READVDATAWITHVMASK(buffer[0][bufferoffset_kvs + index]);
	vdatas[0] = tup0.A;
	vmdatas[0] = tup0.B;
	tuple_t tup1 = MEMCAP0_READVDATAWITHVMASK(buffer[1][bufferoffset_kvs + index]);
	vdatas[1] = tup1.A;
	vmdatas[1] = tup1.B;
	tuple_t tup2 = MEMCAP0_READVDATAWITHVMASK(buffer[2][bufferoffset_kvs + index]);
	vdatas[2] = tup2.A;
	vmdatas[2] = tup2.B;
	tuple_t tup3 = MEMCAP0_READVDATAWITHVMASK(buffer[3][bufferoffset_kvs + index]);
	vdatas[3] = tup3.A;
	vmdatas[3] = tup3.B;
	tuple_t tup4 = MEMCAP0_READVDATAWITHVMASK(buffer[4][bufferoffset_kvs + index]);
	vdatas[4] = tup4.A;
	vmdatas[4] = tup4.B;
	tuple_t tup5 = MEMCAP0_READVDATAWITHVMASK(buffer[5][bufferoffset_kvs + index]);
	vdatas[5] = tup5.A;
	vmdatas[5] = tup5.B;
	tuple_t tup6 = MEMCAP0_READVDATAWITHVMASK(buffer[6][bufferoffset_kvs + index]);
	vdatas[6] = tup6.A;
	vmdatas[6] = tup6.B;
	tuple_t tup7 = MEMCAP0_READVDATAWITHVMASK(buffer[7][bufferoffset_kvs + index]);
	vdatas[7] = tup7.A;
	vmdatas[7] = tup7.B;
	tuple_t tup8 = MEMCAP0_READVDATAWITHVMASK(buffer[8][bufferoffset_kvs + index]);
	vdatas[8] = tup8.A;
	vmdatas[8] = tup8.B;
	tuple_t tup9 = MEMCAP0_READVDATAWITHVMASK(buffer[9][bufferoffset_kvs + index]);
	vdatas[9] = tup9.A;
	vmdatas[9] = tup9.B;
	tuple_t tup10 = MEMCAP0_READVDATAWITHVMASK(buffer[10][bufferoffset_kvs + index]);
	vdatas[10] = tup10.A;
	vmdatas[10] = tup10.B;
	tuple_t tup11 = MEMCAP0_READVDATAWITHVMASK(buffer[11][bufferoffset_kvs + index]);
	vdatas[11] = tup11.A;
	vmdatas[11] = tup11.B;
	tuple_t tup12 = MEMCAP0_READVDATAWITHVMASK(buffer[12][bufferoffset_kvs + index]);
	vdatas[12] = tup12.A;
	vmdatas[12] = tup12.B;
	tuple_t tup13 = MEMCAP0_READVDATAWITHVMASK(buffer[13][bufferoffset_kvs + index]);
	vdatas[13] = tup13.A;
	vmdatas[13] = tup13.B;
	tuple_t tup14 = MEMCAP0_READVDATAWITHVMASK(buffer[14][bufferoffset_kvs + index]);
	vdatas[14] = tup14.A;
	vmdatas[14] = tup14.B;
	tuple_t tup15 = MEMCAP0_READVDATAWITHVMASK(buffer[15][bufferoffset_kvs + index]);
	vdatas[15] = tup15.A;
	vmdatas[15] = tup15.B;
	return;
}

// used in {classname__top_nusrcv_nudstv.cpp->processit_splitdstvxs->MEMACCESSP0_readVchunks} 
void MEMCAP0_READFROMBUFFER_VDATASANDVMASKS(unsigned int index, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], vmdata_t datas[VECTOR2_SIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("MEMCAP0_READFROMBUFFER_VDATASANDVMASKS:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	
	datas[0] = MEMCAP0_READVDATAANDVMASK(buffer[0][bufferoffset_kvs + index]);	
	
	datas[1] = MEMCAP0_READVDATAANDVMASK(buffer[1][bufferoffset_kvs + index]);	
	
	datas[2] = MEMCAP0_READVDATAANDVMASK(buffer[2][bufferoffset_kvs + index]);	
	
	datas[3] = MEMCAP0_READVDATAANDVMASK(buffer[3][bufferoffset_kvs + index]);	
	
	datas[4] = MEMCAP0_READVDATAANDVMASK(buffer[4][bufferoffset_kvs + index]);	
	
	datas[5] = MEMCAP0_READVDATAANDVMASK(buffer[5][bufferoffset_kvs + index]);	
	
	datas[6] = MEMCAP0_READVDATAANDVMASK(buffer[6][bufferoffset_kvs + index]);	
	
	datas[7] = MEMCAP0_READVDATAANDVMASK(buffer[7][bufferoffset_kvs + index]);	
	
	datas[8] = MEMCAP0_READVDATAANDVMASK(buffer[8][bufferoffset_kvs + index]);	
	
	datas[9] = MEMCAP0_READVDATAANDVMASK(buffer[9][bufferoffset_kvs + index]);	
	
	datas[10] = MEMCAP0_READVDATAANDVMASK(buffer[10][bufferoffset_kvs + index]);	
	
	datas[11] = MEMCAP0_READVDATAANDVMASK(buffer[11][bufferoffset_kvs + index]);	
	
	datas[12] = MEMCAP0_READVDATAANDVMASK(buffer[12][bufferoffset_kvs + index]);	
	
	datas[13] = MEMCAP0_READVDATAANDVMASK(buffer[13][bufferoffset_kvs + index]);	
	
	datas[14] = MEMCAP0_READVDATAANDVMASK(buffer[14][bufferoffset_kvs + index]);	
	
	datas[15] = MEMCAP0_READVDATAANDVMASK(buffer[15][bufferoffset_kvs + index]);	
	return;
}
 
void MEMCAP0_WRITETOBUFFER_VDATAWITHVMASK(unsigned int index, keyvalue_vbuffer_t buffer[BLOCKRAM_VDATA_SIZE], value_t vdata, unit1_type vmdata, batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCAP0_WRITETOBUFFER_VDATAWITHVMASK:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif

	MEMCAP0_WRITEVDATAWITHVMASK(&buffer[bufferoffset_kvs + index], vdata, vmdata);
	return;
}

void MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS(unsigned int index, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t datas[VECTOR2_SIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	buffer[0][bufferoffset_kvs + index] = datas[0];
	buffer[1][bufferoffset_kvs + index] = datas[1];
	buffer[2][bufferoffset_kvs + index] = datas[2];
	buffer[3][bufferoffset_kvs + index] = datas[3];
	buffer[4][bufferoffset_kvs + index] = datas[4];
	buffer[5][bufferoffset_kvs + index] = datas[5];
	buffer[6][bufferoffset_kvs + index] = datas[6];
	buffer[7][bufferoffset_kvs + index] = datas[7];
	buffer[8][bufferoffset_kvs + index] = datas[8];
	buffer[9][bufferoffset_kvs + index] = datas[9];
	buffer[10][bufferoffset_kvs + index] = datas[10];
	buffer[11][bufferoffset_kvs + index] = datas[11];
	buffer[12][bufferoffset_kvs + index] = datas[12];
	buffer[13][bufferoffset_kvs + index] = datas[13];
	buffer[14][bufferoffset_kvs + index] = datas[14];
	buffer[15][bufferoffset_kvs + index] = datas[15];
	return;
}

void MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS(unsigned int indexes[VDATA_PACKINGSIZE], keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VDATA_PACKINGSIZE], batch_type bufferoffset_kvs){
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
	
	buffer[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	return;
}

void MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS1_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	buffer0[0][bufferoffset_kvs + index] = vdatas[0];
	buffer0[1][bufferoffset_kvs + index] = vdatas[1];
	buffer0[2][bufferoffset_kvs + index] = vdatas[2];
	buffer0[3][bufferoffset_kvs + index] = vdatas[3];
	buffer0[4][bufferoffset_kvs + index] = vdatas[4];
	buffer0[5][bufferoffset_kvs + index] = vdatas[5];
	buffer0[6][bufferoffset_kvs + index] = vdatas[6];
	buffer0[7][bufferoffset_kvs + index] = vdatas[7];
	buffer0[8][bufferoffset_kvs + index] = vdatas[8];
	buffer0[9][bufferoffset_kvs + index] = vdatas[9];
	buffer0[10][bufferoffset_kvs + index] = vdatas[10];
	buffer0[11][bufferoffset_kvs + index] = vdatas[11];
	buffer0[12][bufferoffset_kvs + index] = vdatas[12];
	buffer0[13][bufferoffset_kvs + index] = vdatas[13];
	buffer0[14][bufferoffset_kvs + index] = vdatas[14];
	buffer0[15][bufferoffset_kvs + index] = vdatas[15];
	return;
}
void MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS2_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	buffer0[0][bufferoffset_kvs + index] = vdatas[0];
	buffer0[1][bufferoffset_kvs + index] = vdatas[1];
	buffer0[2][bufferoffset_kvs + index] = vdatas[2];
	buffer0[3][bufferoffset_kvs + index] = vdatas[3];
	buffer0[4][bufferoffset_kvs + index] = vdatas[4];
	buffer0[5][bufferoffset_kvs + index] = vdatas[5];
	buffer0[6][bufferoffset_kvs + index] = vdatas[6];
	buffer0[7][bufferoffset_kvs + index] = vdatas[7];
	buffer0[8][bufferoffset_kvs + index] = vdatas[8];
	buffer0[9][bufferoffset_kvs + index] = vdatas[9];
	buffer0[10][bufferoffset_kvs + index] = vdatas[10];
	buffer0[11][bufferoffset_kvs + index] = vdatas[11];
	buffer0[12][bufferoffset_kvs + index] = vdatas[12];
	buffer0[13][bufferoffset_kvs + index] = vdatas[13];
	buffer0[14][bufferoffset_kvs + index] = vdatas[14];
	buffer0[15][bufferoffset_kvs + index] = vdatas[15];
	buffer1[0][bufferoffset_kvs + index] = vdatas[0];
	buffer1[1][bufferoffset_kvs + index] = vdatas[1];
	buffer1[2][bufferoffset_kvs + index] = vdatas[2];
	buffer1[3][bufferoffset_kvs + index] = vdatas[3];
	buffer1[4][bufferoffset_kvs + index] = vdatas[4];
	buffer1[5][bufferoffset_kvs + index] = vdatas[5];
	buffer1[6][bufferoffset_kvs + index] = vdatas[6];
	buffer1[7][bufferoffset_kvs + index] = vdatas[7];
	buffer1[8][bufferoffset_kvs + index] = vdatas[8];
	buffer1[9][bufferoffset_kvs + index] = vdatas[9];
	buffer1[10][bufferoffset_kvs + index] = vdatas[10];
	buffer1[11][bufferoffset_kvs + index] = vdatas[11];
	buffer1[12][bufferoffset_kvs + index] = vdatas[12];
	buffer1[13][bufferoffset_kvs + index] = vdatas[13];
	buffer1[14][bufferoffset_kvs + index] = vdatas[14];
	buffer1[15][bufferoffset_kvs + index] = vdatas[15];
	return;
}
void MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS3_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	buffer0[0][bufferoffset_kvs + index] = vdatas[0];
	buffer0[1][bufferoffset_kvs + index] = vdatas[1];
	buffer0[2][bufferoffset_kvs + index] = vdatas[2];
	buffer0[3][bufferoffset_kvs + index] = vdatas[3];
	buffer0[4][bufferoffset_kvs + index] = vdatas[4];
	buffer0[5][bufferoffset_kvs + index] = vdatas[5];
	buffer0[6][bufferoffset_kvs + index] = vdatas[6];
	buffer0[7][bufferoffset_kvs + index] = vdatas[7];
	buffer0[8][bufferoffset_kvs + index] = vdatas[8];
	buffer0[9][bufferoffset_kvs + index] = vdatas[9];
	buffer0[10][bufferoffset_kvs + index] = vdatas[10];
	buffer0[11][bufferoffset_kvs + index] = vdatas[11];
	buffer0[12][bufferoffset_kvs + index] = vdatas[12];
	buffer0[13][bufferoffset_kvs + index] = vdatas[13];
	buffer0[14][bufferoffset_kvs + index] = vdatas[14];
	buffer0[15][bufferoffset_kvs + index] = vdatas[15];
	buffer1[0][bufferoffset_kvs + index] = vdatas[0];
	buffer1[1][bufferoffset_kvs + index] = vdatas[1];
	buffer1[2][bufferoffset_kvs + index] = vdatas[2];
	buffer1[3][bufferoffset_kvs + index] = vdatas[3];
	buffer1[4][bufferoffset_kvs + index] = vdatas[4];
	buffer1[5][bufferoffset_kvs + index] = vdatas[5];
	buffer1[6][bufferoffset_kvs + index] = vdatas[6];
	buffer1[7][bufferoffset_kvs + index] = vdatas[7];
	buffer1[8][bufferoffset_kvs + index] = vdatas[8];
	buffer1[9][bufferoffset_kvs + index] = vdatas[9];
	buffer1[10][bufferoffset_kvs + index] = vdatas[10];
	buffer1[11][bufferoffset_kvs + index] = vdatas[11];
	buffer1[12][bufferoffset_kvs + index] = vdatas[12];
	buffer1[13][bufferoffset_kvs + index] = vdatas[13];
	buffer1[14][bufferoffset_kvs + index] = vdatas[14];
	buffer1[15][bufferoffset_kvs + index] = vdatas[15];
	buffer2[0][bufferoffset_kvs + index] = vdatas[0];
	buffer2[1][bufferoffset_kvs + index] = vdatas[1];
	buffer2[2][bufferoffset_kvs + index] = vdatas[2];
	buffer2[3][bufferoffset_kvs + index] = vdatas[3];
	buffer2[4][bufferoffset_kvs + index] = vdatas[4];
	buffer2[5][bufferoffset_kvs + index] = vdatas[5];
	buffer2[6][bufferoffset_kvs + index] = vdatas[6];
	buffer2[7][bufferoffset_kvs + index] = vdatas[7];
	buffer2[8][bufferoffset_kvs + index] = vdatas[8];
	buffer2[9][bufferoffset_kvs + index] = vdatas[9];
	buffer2[10][bufferoffset_kvs + index] = vdatas[10];
	buffer2[11][bufferoffset_kvs + index] = vdatas[11];
	buffer2[12][bufferoffset_kvs + index] = vdatas[12];
	buffer2[13][bufferoffset_kvs + index] = vdatas[13];
	buffer2[14][bufferoffset_kvs + index] = vdatas[14];
	buffer2[15][bufferoffset_kvs + index] = vdatas[15];
	return;
}
void MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS4_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	buffer0[0][bufferoffset_kvs + index] = vdatas[0];
	buffer0[1][bufferoffset_kvs + index] = vdatas[1];
	buffer0[2][bufferoffset_kvs + index] = vdatas[2];
	buffer0[3][bufferoffset_kvs + index] = vdatas[3];
	buffer0[4][bufferoffset_kvs + index] = vdatas[4];
	buffer0[5][bufferoffset_kvs + index] = vdatas[5];
	buffer0[6][bufferoffset_kvs + index] = vdatas[6];
	buffer0[7][bufferoffset_kvs + index] = vdatas[7];
	buffer0[8][bufferoffset_kvs + index] = vdatas[8];
	buffer0[9][bufferoffset_kvs + index] = vdatas[9];
	buffer0[10][bufferoffset_kvs + index] = vdatas[10];
	buffer0[11][bufferoffset_kvs + index] = vdatas[11];
	buffer0[12][bufferoffset_kvs + index] = vdatas[12];
	buffer0[13][bufferoffset_kvs + index] = vdatas[13];
	buffer0[14][bufferoffset_kvs + index] = vdatas[14];
	buffer0[15][bufferoffset_kvs + index] = vdatas[15];
	buffer1[0][bufferoffset_kvs + index] = vdatas[0];
	buffer1[1][bufferoffset_kvs + index] = vdatas[1];
	buffer1[2][bufferoffset_kvs + index] = vdatas[2];
	buffer1[3][bufferoffset_kvs + index] = vdatas[3];
	buffer1[4][bufferoffset_kvs + index] = vdatas[4];
	buffer1[5][bufferoffset_kvs + index] = vdatas[5];
	buffer1[6][bufferoffset_kvs + index] = vdatas[6];
	buffer1[7][bufferoffset_kvs + index] = vdatas[7];
	buffer1[8][bufferoffset_kvs + index] = vdatas[8];
	buffer1[9][bufferoffset_kvs + index] = vdatas[9];
	buffer1[10][bufferoffset_kvs + index] = vdatas[10];
	buffer1[11][bufferoffset_kvs + index] = vdatas[11];
	buffer1[12][bufferoffset_kvs + index] = vdatas[12];
	buffer1[13][bufferoffset_kvs + index] = vdatas[13];
	buffer1[14][bufferoffset_kvs + index] = vdatas[14];
	buffer1[15][bufferoffset_kvs + index] = vdatas[15];
	buffer2[0][bufferoffset_kvs + index] = vdatas[0];
	buffer2[1][bufferoffset_kvs + index] = vdatas[1];
	buffer2[2][bufferoffset_kvs + index] = vdatas[2];
	buffer2[3][bufferoffset_kvs + index] = vdatas[3];
	buffer2[4][bufferoffset_kvs + index] = vdatas[4];
	buffer2[5][bufferoffset_kvs + index] = vdatas[5];
	buffer2[6][bufferoffset_kvs + index] = vdatas[6];
	buffer2[7][bufferoffset_kvs + index] = vdatas[7];
	buffer2[8][bufferoffset_kvs + index] = vdatas[8];
	buffer2[9][bufferoffset_kvs + index] = vdatas[9];
	buffer2[10][bufferoffset_kvs + index] = vdatas[10];
	buffer2[11][bufferoffset_kvs + index] = vdatas[11];
	buffer2[12][bufferoffset_kvs + index] = vdatas[12];
	buffer2[13][bufferoffset_kvs + index] = vdatas[13];
	buffer2[14][bufferoffset_kvs + index] = vdatas[14];
	buffer2[15][bufferoffset_kvs + index] = vdatas[15];
	buffer3[0][bufferoffset_kvs + index] = vdatas[0];
	buffer3[1][bufferoffset_kvs + index] = vdatas[1];
	buffer3[2][bufferoffset_kvs + index] = vdatas[2];
	buffer3[3][bufferoffset_kvs + index] = vdatas[3];
	buffer3[4][bufferoffset_kvs + index] = vdatas[4];
	buffer3[5][bufferoffset_kvs + index] = vdatas[5];
	buffer3[6][bufferoffset_kvs + index] = vdatas[6];
	buffer3[7][bufferoffset_kvs + index] = vdatas[7];
	buffer3[8][bufferoffset_kvs + index] = vdatas[8];
	buffer3[9][bufferoffset_kvs + index] = vdatas[9];
	buffer3[10][bufferoffset_kvs + index] = vdatas[10];
	buffer3[11][bufferoffset_kvs + index] = vdatas[11];
	buffer3[12][bufferoffset_kvs + index] = vdatas[12];
	buffer3[13][bufferoffset_kvs + index] = vdatas[13];
	buffer3[14][bufferoffset_kvs + index] = vdatas[14];
	buffer3[15][bufferoffset_kvs + index] = vdatas[15];
	return;
}
void MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS5_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	buffer0[0][bufferoffset_kvs + index] = vdatas[0];
	buffer0[1][bufferoffset_kvs + index] = vdatas[1];
	buffer0[2][bufferoffset_kvs + index] = vdatas[2];
	buffer0[3][bufferoffset_kvs + index] = vdatas[3];
	buffer0[4][bufferoffset_kvs + index] = vdatas[4];
	buffer0[5][bufferoffset_kvs + index] = vdatas[5];
	buffer0[6][bufferoffset_kvs + index] = vdatas[6];
	buffer0[7][bufferoffset_kvs + index] = vdatas[7];
	buffer0[8][bufferoffset_kvs + index] = vdatas[8];
	buffer0[9][bufferoffset_kvs + index] = vdatas[9];
	buffer0[10][bufferoffset_kvs + index] = vdatas[10];
	buffer0[11][bufferoffset_kvs + index] = vdatas[11];
	buffer0[12][bufferoffset_kvs + index] = vdatas[12];
	buffer0[13][bufferoffset_kvs + index] = vdatas[13];
	buffer0[14][bufferoffset_kvs + index] = vdatas[14];
	buffer0[15][bufferoffset_kvs + index] = vdatas[15];
	buffer1[0][bufferoffset_kvs + index] = vdatas[0];
	buffer1[1][bufferoffset_kvs + index] = vdatas[1];
	buffer1[2][bufferoffset_kvs + index] = vdatas[2];
	buffer1[3][bufferoffset_kvs + index] = vdatas[3];
	buffer1[4][bufferoffset_kvs + index] = vdatas[4];
	buffer1[5][bufferoffset_kvs + index] = vdatas[5];
	buffer1[6][bufferoffset_kvs + index] = vdatas[6];
	buffer1[7][bufferoffset_kvs + index] = vdatas[7];
	buffer1[8][bufferoffset_kvs + index] = vdatas[8];
	buffer1[9][bufferoffset_kvs + index] = vdatas[9];
	buffer1[10][bufferoffset_kvs + index] = vdatas[10];
	buffer1[11][bufferoffset_kvs + index] = vdatas[11];
	buffer1[12][bufferoffset_kvs + index] = vdatas[12];
	buffer1[13][bufferoffset_kvs + index] = vdatas[13];
	buffer1[14][bufferoffset_kvs + index] = vdatas[14];
	buffer1[15][bufferoffset_kvs + index] = vdatas[15];
	buffer2[0][bufferoffset_kvs + index] = vdatas[0];
	buffer2[1][bufferoffset_kvs + index] = vdatas[1];
	buffer2[2][bufferoffset_kvs + index] = vdatas[2];
	buffer2[3][bufferoffset_kvs + index] = vdatas[3];
	buffer2[4][bufferoffset_kvs + index] = vdatas[4];
	buffer2[5][bufferoffset_kvs + index] = vdatas[5];
	buffer2[6][bufferoffset_kvs + index] = vdatas[6];
	buffer2[7][bufferoffset_kvs + index] = vdatas[7];
	buffer2[8][bufferoffset_kvs + index] = vdatas[8];
	buffer2[9][bufferoffset_kvs + index] = vdatas[9];
	buffer2[10][bufferoffset_kvs + index] = vdatas[10];
	buffer2[11][bufferoffset_kvs + index] = vdatas[11];
	buffer2[12][bufferoffset_kvs + index] = vdatas[12];
	buffer2[13][bufferoffset_kvs + index] = vdatas[13];
	buffer2[14][bufferoffset_kvs + index] = vdatas[14];
	buffer2[15][bufferoffset_kvs + index] = vdatas[15];
	buffer3[0][bufferoffset_kvs + index] = vdatas[0];
	buffer3[1][bufferoffset_kvs + index] = vdatas[1];
	buffer3[2][bufferoffset_kvs + index] = vdatas[2];
	buffer3[3][bufferoffset_kvs + index] = vdatas[3];
	buffer3[4][bufferoffset_kvs + index] = vdatas[4];
	buffer3[5][bufferoffset_kvs + index] = vdatas[5];
	buffer3[6][bufferoffset_kvs + index] = vdatas[6];
	buffer3[7][bufferoffset_kvs + index] = vdatas[7];
	buffer3[8][bufferoffset_kvs + index] = vdatas[8];
	buffer3[9][bufferoffset_kvs + index] = vdatas[9];
	buffer3[10][bufferoffset_kvs + index] = vdatas[10];
	buffer3[11][bufferoffset_kvs + index] = vdatas[11];
	buffer3[12][bufferoffset_kvs + index] = vdatas[12];
	buffer3[13][bufferoffset_kvs + index] = vdatas[13];
	buffer3[14][bufferoffset_kvs + index] = vdatas[14];
	buffer3[15][bufferoffset_kvs + index] = vdatas[15];
	buffer4[0][bufferoffset_kvs + index] = vdatas[0];
	buffer4[1][bufferoffset_kvs + index] = vdatas[1];
	buffer4[2][bufferoffset_kvs + index] = vdatas[2];
	buffer4[3][bufferoffset_kvs + index] = vdatas[3];
	buffer4[4][bufferoffset_kvs + index] = vdatas[4];
	buffer4[5][bufferoffset_kvs + index] = vdatas[5];
	buffer4[6][bufferoffset_kvs + index] = vdatas[6];
	buffer4[7][bufferoffset_kvs + index] = vdatas[7];
	buffer4[8][bufferoffset_kvs + index] = vdatas[8];
	buffer4[9][bufferoffset_kvs + index] = vdatas[9];
	buffer4[10][bufferoffset_kvs + index] = vdatas[10];
	buffer4[11][bufferoffset_kvs + index] = vdatas[11];
	buffer4[12][bufferoffset_kvs + index] = vdatas[12];
	buffer4[13][bufferoffset_kvs + index] = vdatas[13];
	buffer4[14][bufferoffset_kvs + index] = vdatas[14];
	buffer4[15][bufferoffset_kvs + index] = vdatas[15];
	return;
}
void MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS6_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	buffer0[0][bufferoffset_kvs + index] = vdatas[0];
	buffer0[1][bufferoffset_kvs + index] = vdatas[1];
	buffer0[2][bufferoffset_kvs + index] = vdatas[2];
	buffer0[3][bufferoffset_kvs + index] = vdatas[3];
	buffer0[4][bufferoffset_kvs + index] = vdatas[4];
	buffer0[5][bufferoffset_kvs + index] = vdatas[5];
	buffer0[6][bufferoffset_kvs + index] = vdatas[6];
	buffer0[7][bufferoffset_kvs + index] = vdatas[7];
	buffer0[8][bufferoffset_kvs + index] = vdatas[8];
	buffer0[9][bufferoffset_kvs + index] = vdatas[9];
	buffer0[10][bufferoffset_kvs + index] = vdatas[10];
	buffer0[11][bufferoffset_kvs + index] = vdatas[11];
	buffer0[12][bufferoffset_kvs + index] = vdatas[12];
	buffer0[13][bufferoffset_kvs + index] = vdatas[13];
	buffer0[14][bufferoffset_kvs + index] = vdatas[14];
	buffer0[15][bufferoffset_kvs + index] = vdatas[15];
	buffer1[0][bufferoffset_kvs + index] = vdatas[0];
	buffer1[1][bufferoffset_kvs + index] = vdatas[1];
	buffer1[2][bufferoffset_kvs + index] = vdatas[2];
	buffer1[3][bufferoffset_kvs + index] = vdatas[3];
	buffer1[4][bufferoffset_kvs + index] = vdatas[4];
	buffer1[5][bufferoffset_kvs + index] = vdatas[5];
	buffer1[6][bufferoffset_kvs + index] = vdatas[6];
	buffer1[7][bufferoffset_kvs + index] = vdatas[7];
	buffer1[8][bufferoffset_kvs + index] = vdatas[8];
	buffer1[9][bufferoffset_kvs + index] = vdatas[9];
	buffer1[10][bufferoffset_kvs + index] = vdatas[10];
	buffer1[11][bufferoffset_kvs + index] = vdatas[11];
	buffer1[12][bufferoffset_kvs + index] = vdatas[12];
	buffer1[13][bufferoffset_kvs + index] = vdatas[13];
	buffer1[14][bufferoffset_kvs + index] = vdatas[14];
	buffer1[15][bufferoffset_kvs + index] = vdatas[15];
	buffer2[0][bufferoffset_kvs + index] = vdatas[0];
	buffer2[1][bufferoffset_kvs + index] = vdatas[1];
	buffer2[2][bufferoffset_kvs + index] = vdatas[2];
	buffer2[3][bufferoffset_kvs + index] = vdatas[3];
	buffer2[4][bufferoffset_kvs + index] = vdatas[4];
	buffer2[5][bufferoffset_kvs + index] = vdatas[5];
	buffer2[6][bufferoffset_kvs + index] = vdatas[6];
	buffer2[7][bufferoffset_kvs + index] = vdatas[7];
	buffer2[8][bufferoffset_kvs + index] = vdatas[8];
	buffer2[9][bufferoffset_kvs + index] = vdatas[9];
	buffer2[10][bufferoffset_kvs + index] = vdatas[10];
	buffer2[11][bufferoffset_kvs + index] = vdatas[11];
	buffer2[12][bufferoffset_kvs + index] = vdatas[12];
	buffer2[13][bufferoffset_kvs + index] = vdatas[13];
	buffer2[14][bufferoffset_kvs + index] = vdatas[14];
	buffer2[15][bufferoffset_kvs + index] = vdatas[15];
	buffer3[0][bufferoffset_kvs + index] = vdatas[0];
	buffer3[1][bufferoffset_kvs + index] = vdatas[1];
	buffer3[2][bufferoffset_kvs + index] = vdatas[2];
	buffer3[3][bufferoffset_kvs + index] = vdatas[3];
	buffer3[4][bufferoffset_kvs + index] = vdatas[4];
	buffer3[5][bufferoffset_kvs + index] = vdatas[5];
	buffer3[6][bufferoffset_kvs + index] = vdatas[6];
	buffer3[7][bufferoffset_kvs + index] = vdatas[7];
	buffer3[8][bufferoffset_kvs + index] = vdatas[8];
	buffer3[9][bufferoffset_kvs + index] = vdatas[9];
	buffer3[10][bufferoffset_kvs + index] = vdatas[10];
	buffer3[11][bufferoffset_kvs + index] = vdatas[11];
	buffer3[12][bufferoffset_kvs + index] = vdatas[12];
	buffer3[13][bufferoffset_kvs + index] = vdatas[13];
	buffer3[14][bufferoffset_kvs + index] = vdatas[14];
	buffer3[15][bufferoffset_kvs + index] = vdatas[15];
	buffer4[0][bufferoffset_kvs + index] = vdatas[0];
	buffer4[1][bufferoffset_kvs + index] = vdatas[1];
	buffer4[2][bufferoffset_kvs + index] = vdatas[2];
	buffer4[3][bufferoffset_kvs + index] = vdatas[3];
	buffer4[4][bufferoffset_kvs + index] = vdatas[4];
	buffer4[5][bufferoffset_kvs + index] = vdatas[5];
	buffer4[6][bufferoffset_kvs + index] = vdatas[6];
	buffer4[7][bufferoffset_kvs + index] = vdatas[7];
	buffer4[8][bufferoffset_kvs + index] = vdatas[8];
	buffer4[9][bufferoffset_kvs + index] = vdatas[9];
	buffer4[10][bufferoffset_kvs + index] = vdatas[10];
	buffer4[11][bufferoffset_kvs + index] = vdatas[11];
	buffer4[12][bufferoffset_kvs + index] = vdatas[12];
	buffer4[13][bufferoffset_kvs + index] = vdatas[13];
	buffer4[14][bufferoffset_kvs + index] = vdatas[14];
	buffer4[15][bufferoffset_kvs + index] = vdatas[15];
	buffer5[0][bufferoffset_kvs + index] = vdatas[0];
	buffer5[1][bufferoffset_kvs + index] = vdatas[1];
	buffer5[2][bufferoffset_kvs + index] = vdatas[2];
	buffer5[3][bufferoffset_kvs + index] = vdatas[3];
	buffer5[4][bufferoffset_kvs + index] = vdatas[4];
	buffer5[5][bufferoffset_kvs + index] = vdatas[5];
	buffer5[6][bufferoffset_kvs + index] = vdatas[6];
	buffer5[7][bufferoffset_kvs + index] = vdatas[7];
	buffer5[8][bufferoffset_kvs + index] = vdatas[8];
	buffer5[9][bufferoffset_kvs + index] = vdatas[9];
	buffer5[10][bufferoffset_kvs + index] = vdatas[10];
	buffer5[11][bufferoffset_kvs + index] = vdatas[11];
	buffer5[12][bufferoffset_kvs + index] = vdatas[12];
	buffer5[13][bufferoffset_kvs + index] = vdatas[13];
	buffer5[14][bufferoffset_kvs + index] = vdatas[14];
	buffer5[15][bufferoffset_kvs + index] = vdatas[15];
	return;
}
void MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS7_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	buffer0[0][bufferoffset_kvs + index] = vdatas[0];
	buffer0[1][bufferoffset_kvs + index] = vdatas[1];
	buffer0[2][bufferoffset_kvs + index] = vdatas[2];
	buffer0[3][bufferoffset_kvs + index] = vdatas[3];
	buffer0[4][bufferoffset_kvs + index] = vdatas[4];
	buffer0[5][bufferoffset_kvs + index] = vdatas[5];
	buffer0[6][bufferoffset_kvs + index] = vdatas[6];
	buffer0[7][bufferoffset_kvs + index] = vdatas[7];
	buffer0[8][bufferoffset_kvs + index] = vdatas[8];
	buffer0[9][bufferoffset_kvs + index] = vdatas[9];
	buffer0[10][bufferoffset_kvs + index] = vdatas[10];
	buffer0[11][bufferoffset_kvs + index] = vdatas[11];
	buffer0[12][bufferoffset_kvs + index] = vdatas[12];
	buffer0[13][bufferoffset_kvs + index] = vdatas[13];
	buffer0[14][bufferoffset_kvs + index] = vdatas[14];
	buffer0[15][bufferoffset_kvs + index] = vdatas[15];
	buffer1[0][bufferoffset_kvs + index] = vdatas[0];
	buffer1[1][bufferoffset_kvs + index] = vdatas[1];
	buffer1[2][bufferoffset_kvs + index] = vdatas[2];
	buffer1[3][bufferoffset_kvs + index] = vdatas[3];
	buffer1[4][bufferoffset_kvs + index] = vdatas[4];
	buffer1[5][bufferoffset_kvs + index] = vdatas[5];
	buffer1[6][bufferoffset_kvs + index] = vdatas[6];
	buffer1[7][bufferoffset_kvs + index] = vdatas[7];
	buffer1[8][bufferoffset_kvs + index] = vdatas[8];
	buffer1[9][bufferoffset_kvs + index] = vdatas[9];
	buffer1[10][bufferoffset_kvs + index] = vdatas[10];
	buffer1[11][bufferoffset_kvs + index] = vdatas[11];
	buffer1[12][bufferoffset_kvs + index] = vdatas[12];
	buffer1[13][bufferoffset_kvs + index] = vdatas[13];
	buffer1[14][bufferoffset_kvs + index] = vdatas[14];
	buffer1[15][bufferoffset_kvs + index] = vdatas[15];
	buffer2[0][bufferoffset_kvs + index] = vdatas[0];
	buffer2[1][bufferoffset_kvs + index] = vdatas[1];
	buffer2[2][bufferoffset_kvs + index] = vdatas[2];
	buffer2[3][bufferoffset_kvs + index] = vdatas[3];
	buffer2[4][bufferoffset_kvs + index] = vdatas[4];
	buffer2[5][bufferoffset_kvs + index] = vdatas[5];
	buffer2[6][bufferoffset_kvs + index] = vdatas[6];
	buffer2[7][bufferoffset_kvs + index] = vdatas[7];
	buffer2[8][bufferoffset_kvs + index] = vdatas[8];
	buffer2[9][bufferoffset_kvs + index] = vdatas[9];
	buffer2[10][bufferoffset_kvs + index] = vdatas[10];
	buffer2[11][bufferoffset_kvs + index] = vdatas[11];
	buffer2[12][bufferoffset_kvs + index] = vdatas[12];
	buffer2[13][bufferoffset_kvs + index] = vdatas[13];
	buffer2[14][bufferoffset_kvs + index] = vdatas[14];
	buffer2[15][bufferoffset_kvs + index] = vdatas[15];
	buffer3[0][bufferoffset_kvs + index] = vdatas[0];
	buffer3[1][bufferoffset_kvs + index] = vdatas[1];
	buffer3[2][bufferoffset_kvs + index] = vdatas[2];
	buffer3[3][bufferoffset_kvs + index] = vdatas[3];
	buffer3[4][bufferoffset_kvs + index] = vdatas[4];
	buffer3[5][bufferoffset_kvs + index] = vdatas[5];
	buffer3[6][bufferoffset_kvs + index] = vdatas[6];
	buffer3[7][bufferoffset_kvs + index] = vdatas[7];
	buffer3[8][bufferoffset_kvs + index] = vdatas[8];
	buffer3[9][bufferoffset_kvs + index] = vdatas[9];
	buffer3[10][bufferoffset_kvs + index] = vdatas[10];
	buffer3[11][bufferoffset_kvs + index] = vdatas[11];
	buffer3[12][bufferoffset_kvs + index] = vdatas[12];
	buffer3[13][bufferoffset_kvs + index] = vdatas[13];
	buffer3[14][bufferoffset_kvs + index] = vdatas[14];
	buffer3[15][bufferoffset_kvs + index] = vdatas[15];
	buffer4[0][bufferoffset_kvs + index] = vdatas[0];
	buffer4[1][bufferoffset_kvs + index] = vdatas[1];
	buffer4[2][bufferoffset_kvs + index] = vdatas[2];
	buffer4[3][bufferoffset_kvs + index] = vdatas[3];
	buffer4[4][bufferoffset_kvs + index] = vdatas[4];
	buffer4[5][bufferoffset_kvs + index] = vdatas[5];
	buffer4[6][bufferoffset_kvs + index] = vdatas[6];
	buffer4[7][bufferoffset_kvs + index] = vdatas[7];
	buffer4[8][bufferoffset_kvs + index] = vdatas[8];
	buffer4[9][bufferoffset_kvs + index] = vdatas[9];
	buffer4[10][bufferoffset_kvs + index] = vdatas[10];
	buffer4[11][bufferoffset_kvs + index] = vdatas[11];
	buffer4[12][bufferoffset_kvs + index] = vdatas[12];
	buffer4[13][bufferoffset_kvs + index] = vdatas[13];
	buffer4[14][bufferoffset_kvs + index] = vdatas[14];
	buffer4[15][bufferoffset_kvs + index] = vdatas[15];
	buffer5[0][bufferoffset_kvs + index] = vdatas[0];
	buffer5[1][bufferoffset_kvs + index] = vdatas[1];
	buffer5[2][bufferoffset_kvs + index] = vdatas[2];
	buffer5[3][bufferoffset_kvs + index] = vdatas[3];
	buffer5[4][bufferoffset_kvs + index] = vdatas[4];
	buffer5[5][bufferoffset_kvs + index] = vdatas[5];
	buffer5[6][bufferoffset_kvs + index] = vdatas[6];
	buffer5[7][bufferoffset_kvs + index] = vdatas[7];
	buffer5[8][bufferoffset_kvs + index] = vdatas[8];
	buffer5[9][bufferoffset_kvs + index] = vdatas[9];
	buffer5[10][bufferoffset_kvs + index] = vdatas[10];
	buffer5[11][bufferoffset_kvs + index] = vdatas[11];
	buffer5[12][bufferoffset_kvs + index] = vdatas[12];
	buffer5[13][bufferoffset_kvs + index] = vdatas[13];
	buffer5[14][bufferoffset_kvs + index] = vdatas[14];
	buffer5[15][bufferoffset_kvs + index] = vdatas[15];
	buffer6[0][bufferoffset_kvs + index] = vdatas[0];
	buffer6[1][bufferoffset_kvs + index] = vdatas[1];
	buffer6[2][bufferoffset_kvs + index] = vdatas[2];
	buffer6[3][bufferoffset_kvs + index] = vdatas[3];
	buffer6[4][bufferoffset_kvs + index] = vdatas[4];
	buffer6[5][bufferoffset_kvs + index] = vdatas[5];
	buffer6[6][bufferoffset_kvs + index] = vdatas[6];
	buffer6[7][bufferoffset_kvs + index] = vdatas[7];
	buffer6[8][bufferoffset_kvs + index] = vdatas[8];
	buffer6[9][bufferoffset_kvs + index] = vdatas[9];
	buffer6[10][bufferoffset_kvs + index] = vdatas[10];
	buffer6[11][bufferoffset_kvs + index] = vdatas[11];
	buffer6[12][bufferoffset_kvs + index] = vdatas[12];
	buffer6[13][bufferoffset_kvs + index] = vdatas[13];
	buffer6[14][bufferoffset_kvs + index] = vdatas[14];
	buffer6[15][bufferoffset_kvs + index] = vdatas[15];
	return;
}
void MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS8_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	buffer0[0][bufferoffset_kvs + index] = vdatas[0];
	buffer0[1][bufferoffset_kvs + index] = vdatas[1];
	buffer0[2][bufferoffset_kvs + index] = vdatas[2];
	buffer0[3][bufferoffset_kvs + index] = vdatas[3];
	buffer0[4][bufferoffset_kvs + index] = vdatas[4];
	buffer0[5][bufferoffset_kvs + index] = vdatas[5];
	buffer0[6][bufferoffset_kvs + index] = vdatas[6];
	buffer0[7][bufferoffset_kvs + index] = vdatas[7];
	buffer0[8][bufferoffset_kvs + index] = vdatas[8];
	buffer0[9][bufferoffset_kvs + index] = vdatas[9];
	buffer0[10][bufferoffset_kvs + index] = vdatas[10];
	buffer0[11][bufferoffset_kvs + index] = vdatas[11];
	buffer0[12][bufferoffset_kvs + index] = vdatas[12];
	buffer0[13][bufferoffset_kvs + index] = vdatas[13];
	buffer0[14][bufferoffset_kvs + index] = vdatas[14];
	buffer0[15][bufferoffset_kvs + index] = vdatas[15];
	buffer1[0][bufferoffset_kvs + index] = vdatas[0];
	buffer1[1][bufferoffset_kvs + index] = vdatas[1];
	buffer1[2][bufferoffset_kvs + index] = vdatas[2];
	buffer1[3][bufferoffset_kvs + index] = vdatas[3];
	buffer1[4][bufferoffset_kvs + index] = vdatas[4];
	buffer1[5][bufferoffset_kvs + index] = vdatas[5];
	buffer1[6][bufferoffset_kvs + index] = vdatas[6];
	buffer1[7][bufferoffset_kvs + index] = vdatas[7];
	buffer1[8][bufferoffset_kvs + index] = vdatas[8];
	buffer1[9][bufferoffset_kvs + index] = vdatas[9];
	buffer1[10][bufferoffset_kvs + index] = vdatas[10];
	buffer1[11][bufferoffset_kvs + index] = vdatas[11];
	buffer1[12][bufferoffset_kvs + index] = vdatas[12];
	buffer1[13][bufferoffset_kvs + index] = vdatas[13];
	buffer1[14][bufferoffset_kvs + index] = vdatas[14];
	buffer1[15][bufferoffset_kvs + index] = vdatas[15];
	buffer2[0][bufferoffset_kvs + index] = vdatas[0];
	buffer2[1][bufferoffset_kvs + index] = vdatas[1];
	buffer2[2][bufferoffset_kvs + index] = vdatas[2];
	buffer2[3][bufferoffset_kvs + index] = vdatas[3];
	buffer2[4][bufferoffset_kvs + index] = vdatas[4];
	buffer2[5][bufferoffset_kvs + index] = vdatas[5];
	buffer2[6][bufferoffset_kvs + index] = vdatas[6];
	buffer2[7][bufferoffset_kvs + index] = vdatas[7];
	buffer2[8][bufferoffset_kvs + index] = vdatas[8];
	buffer2[9][bufferoffset_kvs + index] = vdatas[9];
	buffer2[10][bufferoffset_kvs + index] = vdatas[10];
	buffer2[11][bufferoffset_kvs + index] = vdatas[11];
	buffer2[12][bufferoffset_kvs + index] = vdatas[12];
	buffer2[13][bufferoffset_kvs + index] = vdatas[13];
	buffer2[14][bufferoffset_kvs + index] = vdatas[14];
	buffer2[15][bufferoffset_kvs + index] = vdatas[15];
	buffer3[0][bufferoffset_kvs + index] = vdatas[0];
	buffer3[1][bufferoffset_kvs + index] = vdatas[1];
	buffer3[2][bufferoffset_kvs + index] = vdatas[2];
	buffer3[3][bufferoffset_kvs + index] = vdatas[3];
	buffer3[4][bufferoffset_kvs + index] = vdatas[4];
	buffer3[5][bufferoffset_kvs + index] = vdatas[5];
	buffer3[6][bufferoffset_kvs + index] = vdatas[6];
	buffer3[7][bufferoffset_kvs + index] = vdatas[7];
	buffer3[8][bufferoffset_kvs + index] = vdatas[8];
	buffer3[9][bufferoffset_kvs + index] = vdatas[9];
	buffer3[10][bufferoffset_kvs + index] = vdatas[10];
	buffer3[11][bufferoffset_kvs + index] = vdatas[11];
	buffer3[12][bufferoffset_kvs + index] = vdatas[12];
	buffer3[13][bufferoffset_kvs + index] = vdatas[13];
	buffer3[14][bufferoffset_kvs + index] = vdatas[14];
	buffer3[15][bufferoffset_kvs + index] = vdatas[15];
	buffer4[0][bufferoffset_kvs + index] = vdatas[0];
	buffer4[1][bufferoffset_kvs + index] = vdatas[1];
	buffer4[2][bufferoffset_kvs + index] = vdatas[2];
	buffer4[3][bufferoffset_kvs + index] = vdatas[3];
	buffer4[4][bufferoffset_kvs + index] = vdatas[4];
	buffer4[5][bufferoffset_kvs + index] = vdatas[5];
	buffer4[6][bufferoffset_kvs + index] = vdatas[6];
	buffer4[7][bufferoffset_kvs + index] = vdatas[7];
	buffer4[8][bufferoffset_kvs + index] = vdatas[8];
	buffer4[9][bufferoffset_kvs + index] = vdatas[9];
	buffer4[10][bufferoffset_kvs + index] = vdatas[10];
	buffer4[11][bufferoffset_kvs + index] = vdatas[11];
	buffer4[12][bufferoffset_kvs + index] = vdatas[12];
	buffer4[13][bufferoffset_kvs + index] = vdatas[13];
	buffer4[14][bufferoffset_kvs + index] = vdatas[14];
	buffer4[15][bufferoffset_kvs + index] = vdatas[15];
	buffer5[0][bufferoffset_kvs + index] = vdatas[0];
	buffer5[1][bufferoffset_kvs + index] = vdatas[1];
	buffer5[2][bufferoffset_kvs + index] = vdatas[2];
	buffer5[3][bufferoffset_kvs + index] = vdatas[3];
	buffer5[4][bufferoffset_kvs + index] = vdatas[4];
	buffer5[5][bufferoffset_kvs + index] = vdatas[5];
	buffer5[6][bufferoffset_kvs + index] = vdatas[6];
	buffer5[7][bufferoffset_kvs + index] = vdatas[7];
	buffer5[8][bufferoffset_kvs + index] = vdatas[8];
	buffer5[9][bufferoffset_kvs + index] = vdatas[9];
	buffer5[10][bufferoffset_kvs + index] = vdatas[10];
	buffer5[11][bufferoffset_kvs + index] = vdatas[11];
	buffer5[12][bufferoffset_kvs + index] = vdatas[12];
	buffer5[13][bufferoffset_kvs + index] = vdatas[13];
	buffer5[14][bufferoffset_kvs + index] = vdatas[14];
	buffer5[15][bufferoffset_kvs + index] = vdatas[15];
	buffer6[0][bufferoffset_kvs + index] = vdatas[0];
	buffer6[1][bufferoffset_kvs + index] = vdatas[1];
	buffer6[2][bufferoffset_kvs + index] = vdatas[2];
	buffer6[3][bufferoffset_kvs + index] = vdatas[3];
	buffer6[4][bufferoffset_kvs + index] = vdatas[4];
	buffer6[5][bufferoffset_kvs + index] = vdatas[5];
	buffer6[6][bufferoffset_kvs + index] = vdatas[6];
	buffer6[7][bufferoffset_kvs + index] = vdatas[7];
	buffer6[8][bufferoffset_kvs + index] = vdatas[8];
	buffer6[9][bufferoffset_kvs + index] = vdatas[9];
	buffer6[10][bufferoffset_kvs + index] = vdatas[10];
	buffer6[11][bufferoffset_kvs + index] = vdatas[11];
	buffer6[12][bufferoffset_kvs + index] = vdatas[12];
	buffer6[13][bufferoffset_kvs + index] = vdatas[13];
	buffer6[14][bufferoffset_kvs + index] = vdatas[14];
	buffer6[15][bufferoffset_kvs + index] = vdatas[15];
	buffer7[0][bufferoffset_kvs + index] = vdatas[0];
	buffer7[1][bufferoffset_kvs + index] = vdatas[1];
	buffer7[2][bufferoffset_kvs + index] = vdatas[2];
	buffer7[3][bufferoffset_kvs + index] = vdatas[3];
	buffer7[4][bufferoffset_kvs + index] = vdatas[4];
	buffer7[5][bufferoffset_kvs + index] = vdatas[5];
	buffer7[6][bufferoffset_kvs + index] = vdatas[6];
	buffer7[7][bufferoffset_kvs + index] = vdatas[7];
	buffer7[8][bufferoffset_kvs + index] = vdatas[8];
	buffer7[9][bufferoffset_kvs + index] = vdatas[9];
	buffer7[10][bufferoffset_kvs + index] = vdatas[10];
	buffer7[11][bufferoffset_kvs + index] = vdatas[11];
	buffer7[12][bufferoffset_kvs + index] = vdatas[12];
	buffer7[13][bufferoffset_kvs + index] = vdatas[13];
	buffer7[14][bufferoffset_kvs + index] = vdatas[14];
	buffer7[15][bufferoffset_kvs + index] = vdatas[15];
	return;
}
void MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS9_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	buffer0[0][bufferoffset_kvs + index] = vdatas[0];
	buffer0[1][bufferoffset_kvs + index] = vdatas[1];
	buffer0[2][bufferoffset_kvs + index] = vdatas[2];
	buffer0[3][bufferoffset_kvs + index] = vdatas[3];
	buffer0[4][bufferoffset_kvs + index] = vdatas[4];
	buffer0[5][bufferoffset_kvs + index] = vdatas[5];
	buffer0[6][bufferoffset_kvs + index] = vdatas[6];
	buffer0[7][bufferoffset_kvs + index] = vdatas[7];
	buffer0[8][bufferoffset_kvs + index] = vdatas[8];
	buffer0[9][bufferoffset_kvs + index] = vdatas[9];
	buffer0[10][bufferoffset_kvs + index] = vdatas[10];
	buffer0[11][bufferoffset_kvs + index] = vdatas[11];
	buffer0[12][bufferoffset_kvs + index] = vdatas[12];
	buffer0[13][bufferoffset_kvs + index] = vdatas[13];
	buffer0[14][bufferoffset_kvs + index] = vdatas[14];
	buffer0[15][bufferoffset_kvs + index] = vdatas[15];
	buffer1[0][bufferoffset_kvs + index] = vdatas[0];
	buffer1[1][bufferoffset_kvs + index] = vdatas[1];
	buffer1[2][bufferoffset_kvs + index] = vdatas[2];
	buffer1[3][bufferoffset_kvs + index] = vdatas[3];
	buffer1[4][bufferoffset_kvs + index] = vdatas[4];
	buffer1[5][bufferoffset_kvs + index] = vdatas[5];
	buffer1[6][bufferoffset_kvs + index] = vdatas[6];
	buffer1[7][bufferoffset_kvs + index] = vdatas[7];
	buffer1[8][bufferoffset_kvs + index] = vdatas[8];
	buffer1[9][bufferoffset_kvs + index] = vdatas[9];
	buffer1[10][bufferoffset_kvs + index] = vdatas[10];
	buffer1[11][bufferoffset_kvs + index] = vdatas[11];
	buffer1[12][bufferoffset_kvs + index] = vdatas[12];
	buffer1[13][bufferoffset_kvs + index] = vdatas[13];
	buffer1[14][bufferoffset_kvs + index] = vdatas[14];
	buffer1[15][bufferoffset_kvs + index] = vdatas[15];
	buffer2[0][bufferoffset_kvs + index] = vdatas[0];
	buffer2[1][bufferoffset_kvs + index] = vdatas[1];
	buffer2[2][bufferoffset_kvs + index] = vdatas[2];
	buffer2[3][bufferoffset_kvs + index] = vdatas[3];
	buffer2[4][bufferoffset_kvs + index] = vdatas[4];
	buffer2[5][bufferoffset_kvs + index] = vdatas[5];
	buffer2[6][bufferoffset_kvs + index] = vdatas[6];
	buffer2[7][bufferoffset_kvs + index] = vdatas[7];
	buffer2[8][bufferoffset_kvs + index] = vdatas[8];
	buffer2[9][bufferoffset_kvs + index] = vdatas[9];
	buffer2[10][bufferoffset_kvs + index] = vdatas[10];
	buffer2[11][bufferoffset_kvs + index] = vdatas[11];
	buffer2[12][bufferoffset_kvs + index] = vdatas[12];
	buffer2[13][bufferoffset_kvs + index] = vdatas[13];
	buffer2[14][bufferoffset_kvs + index] = vdatas[14];
	buffer2[15][bufferoffset_kvs + index] = vdatas[15];
	buffer3[0][bufferoffset_kvs + index] = vdatas[0];
	buffer3[1][bufferoffset_kvs + index] = vdatas[1];
	buffer3[2][bufferoffset_kvs + index] = vdatas[2];
	buffer3[3][bufferoffset_kvs + index] = vdatas[3];
	buffer3[4][bufferoffset_kvs + index] = vdatas[4];
	buffer3[5][bufferoffset_kvs + index] = vdatas[5];
	buffer3[6][bufferoffset_kvs + index] = vdatas[6];
	buffer3[7][bufferoffset_kvs + index] = vdatas[7];
	buffer3[8][bufferoffset_kvs + index] = vdatas[8];
	buffer3[9][bufferoffset_kvs + index] = vdatas[9];
	buffer3[10][bufferoffset_kvs + index] = vdatas[10];
	buffer3[11][bufferoffset_kvs + index] = vdatas[11];
	buffer3[12][bufferoffset_kvs + index] = vdatas[12];
	buffer3[13][bufferoffset_kvs + index] = vdatas[13];
	buffer3[14][bufferoffset_kvs + index] = vdatas[14];
	buffer3[15][bufferoffset_kvs + index] = vdatas[15];
	buffer4[0][bufferoffset_kvs + index] = vdatas[0];
	buffer4[1][bufferoffset_kvs + index] = vdatas[1];
	buffer4[2][bufferoffset_kvs + index] = vdatas[2];
	buffer4[3][bufferoffset_kvs + index] = vdatas[3];
	buffer4[4][bufferoffset_kvs + index] = vdatas[4];
	buffer4[5][bufferoffset_kvs + index] = vdatas[5];
	buffer4[6][bufferoffset_kvs + index] = vdatas[6];
	buffer4[7][bufferoffset_kvs + index] = vdatas[7];
	buffer4[8][bufferoffset_kvs + index] = vdatas[8];
	buffer4[9][bufferoffset_kvs + index] = vdatas[9];
	buffer4[10][bufferoffset_kvs + index] = vdatas[10];
	buffer4[11][bufferoffset_kvs + index] = vdatas[11];
	buffer4[12][bufferoffset_kvs + index] = vdatas[12];
	buffer4[13][bufferoffset_kvs + index] = vdatas[13];
	buffer4[14][bufferoffset_kvs + index] = vdatas[14];
	buffer4[15][bufferoffset_kvs + index] = vdatas[15];
	buffer5[0][bufferoffset_kvs + index] = vdatas[0];
	buffer5[1][bufferoffset_kvs + index] = vdatas[1];
	buffer5[2][bufferoffset_kvs + index] = vdatas[2];
	buffer5[3][bufferoffset_kvs + index] = vdatas[3];
	buffer5[4][bufferoffset_kvs + index] = vdatas[4];
	buffer5[5][bufferoffset_kvs + index] = vdatas[5];
	buffer5[6][bufferoffset_kvs + index] = vdatas[6];
	buffer5[7][bufferoffset_kvs + index] = vdatas[7];
	buffer5[8][bufferoffset_kvs + index] = vdatas[8];
	buffer5[9][bufferoffset_kvs + index] = vdatas[9];
	buffer5[10][bufferoffset_kvs + index] = vdatas[10];
	buffer5[11][bufferoffset_kvs + index] = vdatas[11];
	buffer5[12][bufferoffset_kvs + index] = vdatas[12];
	buffer5[13][bufferoffset_kvs + index] = vdatas[13];
	buffer5[14][bufferoffset_kvs + index] = vdatas[14];
	buffer5[15][bufferoffset_kvs + index] = vdatas[15];
	buffer6[0][bufferoffset_kvs + index] = vdatas[0];
	buffer6[1][bufferoffset_kvs + index] = vdatas[1];
	buffer6[2][bufferoffset_kvs + index] = vdatas[2];
	buffer6[3][bufferoffset_kvs + index] = vdatas[3];
	buffer6[4][bufferoffset_kvs + index] = vdatas[4];
	buffer6[5][bufferoffset_kvs + index] = vdatas[5];
	buffer6[6][bufferoffset_kvs + index] = vdatas[6];
	buffer6[7][bufferoffset_kvs + index] = vdatas[7];
	buffer6[8][bufferoffset_kvs + index] = vdatas[8];
	buffer6[9][bufferoffset_kvs + index] = vdatas[9];
	buffer6[10][bufferoffset_kvs + index] = vdatas[10];
	buffer6[11][bufferoffset_kvs + index] = vdatas[11];
	buffer6[12][bufferoffset_kvs + index] = vdatas[12];
	buffer6[13][bufferoffset_kvs + index] = vdatas[13];
	buffer6[14][bufferoffset_kvs + index] = vdatas[14];
	buffer6[15][bufferoffset_kvs + index] = vdatas[15];
	buffer7[0][bufferoffset_kvs + index] = vdatas[0];
	buffer7[1][bufferoffset_kvs + index] = vdatas[1];
	buffer7[2][bufferoffset_kvs + index] = vdatas[2];
	buffer7[3][bufferoffset_kvs + index] = vdatas[3];
	buffer7[4][bufferoffset_kvs + index] = vdatas[4];
	buffer7[5][bufferoffset_kvs + index] = vdatas[5];
	buffer7[6][bufferoffset_kvs + index] = vdatas[6];
	buffer7[7][bufferoffset_kvs + index] = vdatas[7];
	buffer7[8][bufferoffset_kvs + index] = vdatas[8];
	buffer7[9][bufferoffset_kvs + index] = vdatas[9];
	buffer7[10][bufferoffset_kvs + index] = vdatas[10];
	buffer7[11][bufferoffset_kvs + index] = vdatas[11];
	buffer7[12][bufferoffset_kvs + index] = vdatas[12];
	buffer7[13][bufferoffset_kvs + index] = vdatas[13];
	buffer7[14][bufferoffset_kvs + index] = vdatas[14];
	buffer7[15][bufferoffset_kvs + index] = vdatas[15];
	buffer8[0][bufferoffset_kvs + index] = vdatas[0];
	buffer8[1][bufferoffset_kvs + index] = vdatas[1];
	buffer8[2][bufferoffset_kvs + index] = vdatas[2];
	buffer8[3][bufferoffset_kvs + index] = vdatas[3];
	buffer8[4][bufferoffset_kvs + index] = vdatas[4];
	buffer8[5][bufferoffset_kvs + index] = vdatas[5];
	buffer8[6][bufferoffset_kvs + index] = vdatas[6];
	buffer8[7][bufferoffset_kvs + index] = vdatas[7];
	buffer8[8][bufferoffset_kvs + index] = vdatas[8];
	buffer8[9][bufferoffset_kvs + index] = vdatas[9];
	buffer8[10][bufferoffset_kvs + index] = vdatas[10];
	buffer8[11][bufferoffset_kvs + index] = vdatas[11];
	buffer8[12][bufferoffset_kvs + index] = vdatas[12];
	buffer8[13][bufferoffset_kvs + index] = vdatas[13];
	buffer8[14][bufferoffset_kvs + index] = vdatas[14];
	buffer8[15][bufferoffset_kvs + index] = vdatas[15];
	return;
}
void MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS10_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	buffer0[0][bufferoffset_kvs + index] = vdatas[0];
	buffer0[1][bufferoffset_kvs + index] = vdatas[1];
	buffer0[2][bufferoffset_kvs + index] = vdatas[2];
	buffer0[3][bufferoffset_kvs + index] = vdatas[3];
	buffer0[4][bufferoffset_kvs + index] = vdatas[4];
	buffer0[5][bufferoffset_kvs + index] = vdatas[5];
	buffer0[6][bufferoffset_kvs + index] = vdatas[6];
	buffer0[7][bufferoffset_kvs + index] = vdatas[7];
	buffer0[8][bufferoffset_kvs + index] = vdatas[8];
	buffer0[9][bufferoffset_kvs + index] = vdatas[9];
	buffer0[10][bufferoffset_kvs + index] = vdatas[10];
	buffer0[11][bufferoffset_kvs + index] = vdatas[11];
	buffer0[12][bufferoffset_kvs + index] = vdatas[12];
	buffer0[13][bufferoffset_kvs + index] = vdatas[13];
	buffer0[14][bufferoffset_kvs + index] = vdatas[14];
	buffer0[15][bufferoffset_kvs + index] = vdatas[15];
	buffer1[0][bufferoffset_kvs + index] = vdatas[0];
	buffer1[1][bufferoffset_kvs + index] = vdatas[1];
	buffer1[2][bufferoffset_kvs + index] = vdatas[2];
	buffer1[3][bufferoffset_kvs + index] = vdatas[3];
	buffer1[4][bufferoffset_kvs + index] = vdatas[4];
	buffer1[5][bufferoffset_kvs + index] = vdatas[5];
	buffer1[6][bufferoffset_kvs + index] = vdatas[6];
	buffer1[7][bufferoffset_kvs + index] = vdatas[7];
	buffer1[8][bufferoffset_kvs + index] = vdatas[8];
	buffer1[9][bufferoffset_kvs + index] = vdatas[9];
	buffer1[10][bufferoffset_kvs + index] = vdatas[10];
	buffer1[11][bufferoffset_kvs + index] = vdatas[11];
	buffer1[12][bufferoffset_kvs + index] = vdatas[12];
	buffer1[13][bufferoffset_kvs + index] = vdatas[13];
	buffer1[14][bufferoffset_kvs + index] = vdatas[14];
	buffer1[15][bufferoffset_kvs + index] = vdatas[15];
	buffer2[0][bufferoffset_kvs + index] = vdatas[0];
	buffer2[1][bufferoffset_kvs + index] = vdatas[1];
	buffer2[2][bufferoffset_kvs + index] = vdatas[2];
	buffer2[3][bufferoffset_kvs + index] = vdatas[3];
	buffer2[4][bufferoffset_kvs + index] = vdatas[4];
	buffer2[5][bufferoffset_kvs + index] = vdatas[5];
	buffer2[6][bufferoffset_kvs + index] = vdatas[6];
	buffer2[7][bufferoffset_kvs + index] = vdatas[7];
	buffer2[8][bufferoffset_kvs + index] = vdatas[8];
	buffer2[9][bufferoffset_kvs + index] = vdatas[9];
	buffer2[10][bufferoffset_kvs + index] = vdatas[10];
	buffer2[11][bufferoffset_kvs + index] = vdatas[11];
	buffer2[12][bufferoffset_kvs + index] = vdatas[12];
	buffer2[13][bufferoffset_kvs + index] = vdatas[13];
	buffer2[14][bufferoffset_kvs + index] = vdatas[14];
	buffer2[15][bufferoffset_kvs + index] = vdatas[15];
	buffer3[0][bufferoffset_kvs + index] = vdatas[0];
	buffer3[1][bufferoffset_kvs + index] = vdatas[1];
	buffer3[2][bufferoffset_kvs + index] = vdatas[2];
	buffer3[3][bufferoffset_kvs + index] = vdatas[3];
	buffer3[4][bufferoffset_kvs + index] = vdatas[4];
	buffer3[5][bufferoffset_kvs + index] = vdatas[5];
	buffer3[6][bufferoffset_kvs + index] = vdatas[6];
	buffer3[7][bufferoffset_kvs + index] = vdatas[7];
	buffer3[8][bufferoffset_kvs + index] = vdatas[8];
	buffer3[9][bufferoffset_kvs + index] = vdatas[9];
	buffer3[10][bufferoffset_kvs + index] = vdatas[10];
	buffer3[11][bufferoffset_kvs + index] = vdatas[11];
	buffer3[12][bufferoffset_kvs + index] = vdatas[12];
	buffer3[13][bufferoffset_kvs + index] = vdatas[13];
	buffer3[14][bufferoffset_kvs + index] = vdatas[14];
	buffer3[15][bufferoffset_kvs + index] = vdatas[15];
	buffer4[0][bufferoffset_kvs + index] = vdatas[0];
	buffer4[1][bufferoffset_kvs + index] = vdatas[1];
	buffer4[2][bufferoffset_kvs + index] = vdatas[2];
	buffer4[3][bufferoffset_kvs + index] = vdatas[3];
	buffer4[4][bufferoffset_kvs + index] = vdatas[4];
	buffer4[5][bufferoffset_kvs + index] = vdatas[5];
	buffer4[6][bufferoffset_kvs + index] = vdatas[6];
	buffer4[7][bufferoffset_kvs + index] = vdatas[7];
	buffer4[8][bufferoffset_kvs + index] = vdatas[8];
	buffer4[9][bufferoffset_kvs + index] = vdatas[9];
	buffer4[10][bufferoffset_kvs + index] = vdatas[10];
	buffer4[11][bufferoffset_kvs + index] = vdatas[11];
	buffer4[12][bufferoffset_kvs + index] = vdatas[12];
	buffer4[13][bufferoffset_kvs + index] = vdatas[13];
	buffer4[14][bufferoffset_kvs + index] = vdatas[14];
	buffer4[15][bufferoffset_kvs + index] = vdatas[15];
	buffer5[0][bufferoffset_kvs + index] = vdatas[0];
	buffer5[1][bufferoffset_kvs + index] = vdatas[1];
	buffer5[2][bufferoffset_kvs + index] = vdatas[2];
	buffer5[3][bufferoffset_kvs + index] = vdatas[3];
	buffer5[4][bufferoffset_kvs + index] = vdatas[4];
	buffer5[5][bufferoffset_kvs + index] = vdatas[5];
	buffer5[6][bufferoffset_kvs + index] = vdatas[6];
	buffer5[7][bufferoffset_kvs + index] = vdatas[7];
	buffer5[8][bufferoffset_kvs + index] = vdatas[8];
	buffer5[9][bufferoffset_kvs + index] = vdatas[9];
	buffer5[10][bufferoffset_kvs + index] = vdatas[10];
	buffer5[11][bufferoffset_kvs + index] = vdatas[11];
	buffer5[12][bufferoffset_kvs + index] = vdatas[12];
	buffer5[13][bufferoffset_kvs + index] = vdatas[13];
	buffer5[14][bufferoffset_kvs + index] = vdatas[14];
	buffer5[15][bufferoffset_kvs + index] = vdatas[15];
	buffer6[0][bufferoffset_kvs + index] = vdatas[0];
	buffer6[1][bufferoffset_kvs + index] = vdatas[1];
	buffer6[2][bufferoffset_kvs + index] = vdatas[2];
	buffer6[3][bufferoffset_kvs + index] = vdatas[3];
	buffer6[4][bufferoffset_kvs + index] = vdatas[4];
	buffer6[5][bufferoffset_kvs + index] = vdatas[5];
	buffer6[6][bufferoffset_kvs + index] = vdatas[6];
	buffer6[7][bufferoffset_kvs + index] = vdatas[7];
	buffer6[8][bufferoffset_kvs + index] = vdatas[8];
	buffer6[9][bufferoffset_kvs + index] = vdatas[9];
	buffer6[10][bufferoffset_kvs + index] = vdatas[10];
	buffer6[11][bufferoffset_kvs + index] = vdatas[11];
	buffer6[12][bufferoffset_kvs + index] = vdatas[12];
	buffer6[13][bufferoffset_kvs + index] = vdatas[13];
	buffer6[14][bufferoffset_kvs + index] = vdatas[14];
	buffer6[15][bufferoffset_kvs + index] = vdatas[15];
	buffer7[0][bufferoffset_kvs + index] = vdatas[0];
	buffer7[1][bufferoffset_kvs + index] = vdatas[1];
	buffer7[2][bufferoffset_kvs + index] = vdatas[2];
	buffer7[3][bufferoffset_kvs + index] = vdatas[3];
	buffer7[4][bufferoffset_kvs + index] = vdatas[4];
	buffer7[5][bufferoffset_kvs + index] = vdatas[5];
	buffer7[6][bufferoffset_kvs + index] = vdatas[6];
	buffer7[7][bufferoffset_kvs + index] = vdatas[7];
	buffer7[8][bufferoffset_kvs + index] = vdatas[8];
	buffer7[9][bufferoffset_kvs + index] = vdatas[9];
	buffer7[10][bufferoffset_kvs + index] = vdatas[10];
	buffer7[11][bufferoffset_kvs + index] = vdatas[11];
	buffer7[12][bufferoffset_kvs + index] = vdatas[12];
	buffer7[13][bufferoffset_kvs + index] = vdatas[13];
	buffer7[14][bufferoffset_kvs + index] = vdatas[14];
	buffer7[15][bufferoffset_kvs + index] = vdatas[15];
	buffer8[0][bufferoffset_kvs + index] = vdatas[0];
	buffer8[1][bufferoffset_kvs + index] = vdatas[1];
	buffer8[2][bufferoffset_kvs + index] = vdatas[2];
	buffer8[3][bufferoffset_kvs + index] = vdatas[3];
	buffer8[4][bufferoffset_kvs + index] = vdatas[4];
	buffer8[5][bufferoffset_kvs + index] = vdatas[5];
	buffer8[6][bufferoffset_kvs + index] = vdatas[6];
	buffer8[7][bufferoffset_kvs + index] = vdatas[7];
	buffer8[8][bufferoffset_kvs + index] = vdatas[8];
	buffer8[9][bufferoffset_kvs + index] = vdatas[9];
	buffer8[10][bufferoffset_kvs + index] = vdatas[10];
	buffer8[11][bufferoffset_kvs + index] = vdatas[11];
	buffer8[12][bufferoffset_kvs + index] = vdatas[12];
	buffer8[13][bufferoffset_kvs + index] = vdatas[13];
	buffer8[14][bufferoffset_kvs + index] = vdatas[14];
	buffer8[15][bufferoffset_kvs + index] = vdatas[15];
	buffer9[0][bufferoffset_kvs + index] = vdatas[0];
	buffer9[1][bufferoffset_kvs + index] = vdatas[1];
	buffer9[2][bufferoffset_kvs + index] = vdatas[2];
	buffer9[3][bufferoffset_kvs + index] = vdatas[3];
	buffer9[4][bufferoffset_kvs + index] = vdatas[4];
	buffer9[5][bufferoffset_kvs + index] = vdatas[5];
	buffer9[6][bufferoffset_kvs + index] = vdatas[6];
	buffer9[7][bufferoffset_kvs + index] = vdatas[7];
	buffer9[8][bufferoffset_kvs + index] = vdatas[8];
	buffer9[9][bufferoffset_kvs + index] = vdatas[9];
	buffer9[10][bufferoffset_kvs + index] = vdatas[10];
	buffer9[11][bufferoffset_kvs + index] = vdatas[11];
	buffer9[12][bufferoffset_kvs + index] = vdatas[12];
	buffer9[13][bufferoffset_kvs + index] = vdatas[13];
	buffer9[14][bufferoffset_kvs + index] = vdatas[14];
	buffer9[15][bufferoffset_kvs + index] = vdatas[15];
	return;
}
void MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS11_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer10[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	buffer0[0][bufferoffset_kvs + index] = vdatas[0];
	buffer0[1][bufferoffset_kvs + index] = vdatas[1];
	buffer0[2][bufferoffset_kvs + index] = vdatas[2];
	buffer0[3][bufferoffset_kvs + index] = vdatas[3];
	buffer0[4][bufferoffset_kvs + index] = vdatas[4];
	buffer0[5][bufferoffset_kvs + index] = vdatas[5];
	buffer0[6][bufferoffset_kvs + index] = vdatas[6];
	buffer0[7][bufferoffset_kvs + index] = vdatas[7];
	buffer0[8][bufferoffset_kvs + index] = vdatas[8];
	buffer0[9][bufferoffset_kvs + index] = vdatas[9];
	buffer0[10][bufferoffset_kvs + index] = vdatas[10];
	buffer0[11][bufferoffset_kvs + index] = vdatas[11];
	buffer0[12][bufferoffset_kvs + index] = vdatas[12];
	buffer0[13][bufferoffset_kvs + index] = vdatas[13];
	buffer0[14][bufferoffset_kvs + index] = vdatas[14];
	buffer0[15][bufferoffset_kvs + index] = vdatas[15];
	buffer1[0][bufferoffset_kvs + index] = vdatas[0];
	buffer1[1][bufferoffset_kvs + index] = vdatas[1];
	buffer1[2][bufferoffset_kvs + index] = vdatas[2];
	buffer1[3][bufferoffset_kvs + index] = vdatas[3];
	buffer1[4][bufferoffset_kvs + index] = vdatas[4];
	buffer1[5][bufferoffset_kvs + index] = vdatas[5];
	buffer1[6][bufferoffset_kvs + index] = vdatas[6];
	buffer1[7][bufferoffset_kvs + index] = vdatas[7];
	buffer1[8][bufferoffset_kvs + index] = vdatas[8];
	buffer1[9][bufferoffset_kvs + index] = vdatas[9];
	buffer1[10][bufferoffset_kvs + index] = vdatas[10];
	buffer1[11][bufferoffset_kvs + index] = vdatas[11];
	buffer1[12][bufferoffset_kvs + index] = vdatas[12];
	buffer1[13][bufferoffset_kvs + index] = vdatas[13];
	buffer1[14][bufferoffset_kvs + index] = vdatas[14];
	buffer1[15][bufferoffset_kvs + index] = vdatas[15];
	buffer2[0][bufferoffset_kvs + index] = vdatas[0];
	buffer2[1][bufferoffset_kvs + index] = vdatas[1];
	buffer2[2][bufferoffset_kvs + index] = vdatas[2];
	buffer2[3][bufferoffset_kvs + index] = vdatas[3];
	buffer2[4][bufferoffset_kvs + index] = vdatas[4];
	buffer2[5][bufferoffset_kvs + index] = vdatas[5];
	buffer2[6][bufferoffset_kvs + index] = vdatas[6];
	buffer2[7][bufferoffset_kvs + index] = vdatas[7];
	buffer2[8][bufferoffset_kvs + index] = vdatas[8];
	buffer2[9][bufferoffset_kvs + index] = vdatas[9];
	buffer2[10][bufferoffset_kvs + index] = vdatas[10];
	buffer2[11][bufferoffset_kvs + index] = vdatas[11];
	buffer2[12][bufferoffset_kvs + index] = vdatas[12];
	buffer2[13][bufferoffset_kvs + index] = vdatas[13];
	buffer2[14][bufferoffset_kvs + index] = vdatas[14];
	buffer2[15][bufferoffset_kvs + index] = vdatas[15];
	buffer3[0][bufferoffset_kvs + index] = vdatas[0];
	buffer3[1][bufferoffset_kvs + index] = vdatas[1];
	buffer3[2][bufferoffset_kvs + index] = vdatas[2];
	buffer3[3][bufferoffset_kvs + index] = vdatas[3];
	buffer3[4][bufferoffset_kvs + index] = vdatas[4];
	buffer3[5][bufferoffset_kvs + index] = vdatas[5];
	buffer3[6][bufferoffset_kvs + index] = vdatas[6];
	buffer3[7][bufferoffset_kvs + index] = vdatas[7];
	buffer3[8][bufferoffset_kvs + index] = vdatas[8];
	buffer3[9][bufferoffset_kvs + index] = vdatas[9];
	buffer3[10][bufferoffset_kvs + index] = vdatas[10];
	buffer3[11][bufferoffset_kvs + index] = vdatas[11];
	buffer3[12][bufferoffset_kvs + index] = vdatas[12];
	buffer3[13][bufferoffset_kvs + index] = vdatas[13];
	buffer3[14][bufferoffset_kvs + index] = vdatas[14];
	buffer3[15][bufferoffset_kvs + index] = vdatas[15];
	buffer4[0][bufferoffset_kvs + index] = vdatas[0];
	buffer4[1][bufferoffset_kvs + index] = vdatas[1];
	buffer4[2][bufferoffset_kvs + index] = vdatas[2];
	buffer4[3][bufferoffset_kvs + index] = vdatas[3];
	buffer4[4][bufferoffset_kvs + index] = vdatas[4];
	buffer4[5][bufferoffset_kvs + index] = vdatas[5];
	buffer4[6][bufferoffset_kvs + index] = vdatas[6];
	buffer4[7][bufferoffset_kvs + index] = vdatas[7];
	buffer4[8][bufferoffset_kvs + index] = vdatas[8];
	buffer4[9][bufferoffset_kvs + index] = vdatas[9];
	buffer4[10][bufferoffset_kvs + index] = vdatas[10];
	buffer4[11][bufferoffset_kvs + index] = vdatas[11];
	buffer4[12][bufferoffset_kvs + index] = vdatas[12];
	buffer4[13][bufferoffset_kvs + index] = vdatas[13];
	buffer4[14][bufferoffset_kvs + index] = vdatas[14];
	buffer4[15][bufferoffset_kvs + index] = vdatas[15];
	buffer5[0][bufferoffset_kvs + index] = vdatas[0];
	buffer5[1][bufferoffset_kvs + index] = vdatas[1];
	buffer5[2][bufferoffset_kvs + index] = vdatas[2];
	buffer5[3][bufferoffset_kvs + index] = vdatas[3];
	buffer5[4][bufferoffset_kvs + index] = vdatas[4];
	buffer5[5][bufferoffset_kvs + index] = vdatas[5];
	buffer5[6][bufferoffset_kvs + index] = vdatas[6];
	buffer5[7][bufferoffset_kvs + index] = vdatas[7];
	buffer5[8][bufferoffset_kvs + index] = vdatas[8];
	buffer5[9][bufferoffset_kvs + index] = vdatas[9];
	buffer5[10][bufferoffset_kvs + index] = vdatas[10];
	buffer5[11][bufferoffset_kvs + index] = vdatas[11];
	buffer5[12][bufferoffset_kvs + index] = vdatas[12];
	buffer5[13][bufferoffset_kvs + index] = vdatas[13];
	buffer5[14][bufferoffset_kvs + index] = vdatas[14];
	buffer5[15][bufferoffset_kvs + index] = vdatas[15];
	buffer6[0][bufferoffset_kvs + index] = vdatas[0];
	buffer6[1][bufferoffset_kvs + index] = vdatas[1];
	buffer6[2][bufferoffset_kvs + index] = vdatas[2];
	buffer6[3][bufferoffset_kvs + index] = vdatas[3];
	buffer6[4][bufferoffset_kvs + index] = vdatas[4];
	buffer6[5][bufferoffset_kvs + index] = vdatas[5];
	buffer6[6][bufferoffset_kvs + index] = vdatas[6];
	buffer6[7][bufferoffset_kvs + index] = vdatas[7];
	buffer6[8][bufferoffset_kvs + index] = vdatas[8];
	buffer6[9][bufferoffset_kvs + index] = vdatas[9];
	buffer6[10][bufferoffset_kvs + index] = vdatas[10];
	buffer6[11][bufferoffset_kvs + index] = vdatas[11];
	buffer6[12][bufferoffset_kvs + index] = vdatas[12];
	buffer6[13][bufferoffset_kvs + index] = vdatas[13];
	buffer6[14][bufferoffset_kvs + index] = vdatas[14];
	buffer6[15][bufferoffset_kvs + index] = vdatas[15];
	buffer7[0][bufferoffset_kvs + index] = vdatas[0];
	buffer7[1][bufferoffset_kvs + index] = vdatas[1];
	buffer7[2][bufferoffset_kvs + index] = vdatas[2];
	buffer7[3][bufferoffset_kvs + index] = vdatas[3];
	buffer7[4][bufferoffset_kvs + index] = vdatas[4];
	buffer7[5][bufferoffset_kvs + index] = vdatas[5];
	buffer7[6][bufferoffset_kvs + index] = vdatas[6];
	buffer7[7][bufferoffset_kvs + index] = vdatas[7];
	buffer7[8][bufferoffset_kvs + index] = vdatas[8];
	buffer7[9][bufferoffset_kvs + index] = vdatas[9];
	buffer7[10][bufferoffset_kvs + index] = vdatas[10];
	buffer7[11][bufferoffset_kvs + index] = vdatas[11];
	buffer7[12][bufferoffset_kvs + index] = vdatas[12];
	buffer7[13][bufferoffset_kvs + index] = vdatas[13];
	buffer7[14][bufferoffset_kvs + index] = vdatas[14];
	buffer7[15][bufferoffset_kvs + index] = vdatas[15];
	buffer8[0][bufferoffset_kvs + index] = vdatas[0];
	buffer8[1][bufferoffset_kvs + index] = vdatas[1];
	buffer8[2][bufferoffset_kvs + index] = vdatas[2];
	buffer8[3][bufferoffset_kvs + index] = vdatas[3];
	buffer8[4][bufferoffset_kvs + index] = vdatas[4];
	buffer8[5][bufferoffset_kvs + index] = vdatas[5];
	buffer8[6][bufferoffset_kvs + index] = vdatas[6];
	buffer8[7][bufferoffset_kvs + index] = vdatas[7];
	buffer8[8][bufferoffset_kvs + index] = vdatas[8];
	buffer8[9][bufferoffset_kvs + index] = vdatas[9];
	buffer8[10][bufferoffset_kvs + index] = vdatas[10];
	buffer8[11][bufferoffset_kvs + index] = vdatas[11];
	buffer8[12][bufferoffset_kvs + index] = vdatas[12];
	buffer8[13][bufferoffset_kvs + index] = vdatas[13];
	buffer8[14][bufferoffset_kvs + index] = vdatas[14];
	buffer8[15][bufferoffset_kvs + index] = vdatas[15];
	buffer9[0][bufferoffset_kvs + index] = vdatas[0];
	buffer9[1][bufferoffset_kvs + index] = vdatas[1];
	buffer9[2][bufferoffset_kvs + index] = vdatas[2];
	buffer9[3][bufferoffset_kvs + index] = vdatas[3];
	buffer9[4][bufferoffset_kvs + index] = vdatas[4];
	buffer9[5][bufferoffset_kvs + index] = vdatas[5];
	buffer9[6][bufferoffset_kvs + index] = vdatas[6];
	buffer9[7][bufferoffset_kvs + index] = vdatas[7];
	buffer9[8][bufferoffset_kvs + index] = vdatas[8];
	buffer9[9][bufferoffset_kvs + index] = vdatas[9];
	buffer9[10][bufferoffset_kvs + index] = vdatas[10];
	buffer9[11][bufferoffset_kvs + index] = vdatas[11];
	buffer9[12][bufferoffset_kvs + index] = vdatas[12];
	buffer9[13][bufferoffset_kvs + index] = vdatas[13];
	buffer9[14][bufferoffset_kvs + index] = vdatas[14];
	buffer9[15][bufferoffset_kvs + index] = vdatas[15];
	buffer10[0][bufferoffset_kvs + index] = vdatas[0];
	buffer10[1][bufferoffset_kvs + index] = vdatas[1];
	buffer10[2][bufferoffset_kvs + index] = vdatas[2];
	buffer10[3][bufferoffset_kvs + index] = vdatas[3];
	buffer10[4][bufferoffset_kvs + index] = vdatas[4];
	buffer10[5][bufferoffset_kvs + index] = vdatas[5];
	buffer10[6][bufferoffset_kvs + index] = vdatas[6];
	buffer10[7][bufferoffset_kvs + index] = vdatas[7];
	buffer10[8][bufferoffset_kvs + index] = vdatas[8];
	buffer10[9][bufferoffset_kvs + index] = vdatas[9];
	buffer10[10][bufferoffset_kvs + index] = vdatas[10];
	buffer10[11][bufferoffset_kvs + index] = vdatas[11];
	buffer10[12][bufferoffset_kvs + index] = vdatas[12];
	buffer10[13][bufferoffset_kvs + index] = vdatas[13];
	buffer10[14][bufferoffset_kvs + index] = vdatas[14];
	buffer10[15][bufferoffset_kvs + index] = vdatas[15];
	return;
}
void MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS12_ANDREPLICATE(unsigned int index, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer10[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer11[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	buffer0[0][bufferoffset_kvs + index] = vdatas[0];
	buffer0[1][bufferoffset_kvs + index] = vdatas[1];
	buffer0[2][bufferoffset_kvs + index] = vdatas[2];
	buffer0[3][bufferoffset_kvs + index] = vdatas[3];
	buffer0[4][bufferoffset_kvs + index] = vdatas[4];
	buffer0[5][bufferoffset_kvs + index] = vdatas[5];
	buffer0[6][bufferoffset_kvs + index] = vdatas[6];
	buffer0[7][bufferoffset_kvs + index] = vdatas[7];
	buffer0[8][bufferoffset_kvs + index] = vdatas[8];
	buffer0[9][bufferoffset_kvs + index] = vdatas[9];
	buffer0[10][bufferoffset_kvs + index] = vdatas[10];
	buffer0[11][bufferoffset_kvs + index] = vdatas[11];
	buffer0[12][bufferoffset_kvs + index] = vdatas[12];
	buffer0[13][bufferoffset_kvs + index] = vdatas[13];
	buffer0[14][bufferoffset_kvs + index] = vdatas[14];
	buffer0[15][bufferoffset_kvs + index] = vdatas[15];
	buffer1[0][bufferoffset_kvs + index] = vdatas[0];
	buffer1[1][bufferoffset_kvs + index] = vdatas[1];
	buffer1[2][bufferoffset_kvs + index] = vdatas[2];
	buffer1[3][bufferoffset_kvs + index] = vdatas[3];
	buffer1[4][bufferoffset_kvs + index] = vdatas[4];
	buffer1[5][bufferoffset_kvs + index] = vdatas[5];
	buffer1[6][bufferoffset_kvs + index] = vdatas[6];
	buffer1[7][bufferoffset_kvs + index] = vdatas[7];
	buffer1[8][bufferoffset_kvs + index] = vdatas[8];
	buffer1[9][bufferoffset_kvs + index] = vdatas[9];
	buffer1[10][bufferoffset_kvs + index] = vdatas[10];
	buffer1[11][bufferoffset_kvs + index] = vdatas[11];
	buffer1[12][bufferoffset_kvs + index] = vdatas[12];
	buffer1[13][bufferoffset_kvs + index] = vdatas[13];
	buffer1[14][bufferoffset_kvs + index] = vdatas[14];
	buffer1[15][bufferoffset_kvs + index] = vdatas[15];
	buffer2[0][bufferoffset_kvs + index] = vdatas[0];
	buffer2[1][bufferoffset_kvs + index] = vdatas[1];
	buffer2[2][bufferoffset_kvs + index] = vdatas[2];
	buffer2[3][bufferoffset_kvs + index] = vdatas[3];
	buffer2[4][bufferoffset_kvs + index] = vdatas[4];
	buffer2[5][bufferoffset_kvs + index] = vdatas[5];
	buffer2[6][bufferoffset_kvs + index] = vdatas[6];
	buffer2[7][bufferoffset_kvs + index] = vdatas[7];
	buffer2[8][bufferoffset_kvs + index] = vdatas[8];
	buffer2[9][bufferoffset_kvs + index] = vdatas[9];
	buffer2[10][bufferoffset_kvs + index] = vdatas[10];
	buffer2[11][bufferoffset_kvs + index] = vdatas[11];
	buffer2[12][bufferoffset_kvs + index] = vdatas[12];
	buffer2[13][bufferoffset_kvs + index] = vdatas[13];
	buffer2[14][bufferoffset_kvs + index] = vdatas[14];
	buffer2[15][bufferoffset_kvs + index] = vdatas[15];
	buffer3[0][bufferoffset_kvs + index] = vdatas[0];
	buffer3[1][bufferoffset_kvs + index] = vdatas[1];
	buffer3[2][bufferoffset_kvs + index] = vdatas[2];
	buffer3[3][bufferoffset_kvs + index] = vdatas[3];
	buffer3[4][bufferoffset_kvs + index] = vdatas[4];
	buffer3[5][bufferoffset_kvs + index] = vdatas[5];
	buffer3[6][bufferoffset_kvs + index] = vdatas[6];
	buffer3[7][bufferoffset_kvs + index] = vdatas[7];
	buffer3[8][bufferoffset_kvs + index] = vdatas[8];
	buffer3[9][bufferoffset_kvs + index] = vdatas[9];
	buffer3[10][bufferoffset_kvs + index] = vdatas[10];
	buffer3[11][bufferoffset_kvs + index] = vdatas[11];
	buffer3[12][bufferoffset_kvs + index] = vdatas[12];
	buffer3[13][bufferoffset_kvs + index] = vdatas[13];
	buffer3[14][bufferoffset_kvs + index] = vdatas[14];
	buffer3[15][bufferoffset_kvs + index] = vdatas[15];
	buffer4[0][bufferoffset_kvs + index] = vdatas[0];
	buffer4[1][bufferoffset_kvs + index] = vdatas[1];
	buffer4[2][bufferoffset_kvs + index] = vdatas[2];
	buffer4[3][bufferoffset_kvs + index] = vdatas[3];
	buffer4[4][bufferoffset_kvs + index] = vdatas[4];
	buffer4[5][bufferoffset_kvs + index] = vdatas[5];
	buffer4[6][bufferoffset_kvs + index] = vdatas[6];
	buffer4[7][bufferoffset_kvs + index] = vdatas[7];
	buffer4[8][bufferoffset_kvs + index] = vdatas[8];
	buffer4[9][bufferoffset_kvs + index] = vdatas[9];
	buffer4[10][bufferoffset_kvs + index] = vdatas[10];
	buffer4[11][bufferoffset_kvs + index] = vdatas[11];
	buffer4[12][bufferoffset_kvs + index] = vdatas[12];
	buffer4[13][bufferoffset_kvs + index] = vdatas[13];
	buffer4[14][bufferoffset_kvs + index] = vdatas[14];
	buffer4[15][bufferoffset_kvs + index] = vdatas[15];
	buffer5[0][bufferoffset_kvs + index] = vdatas[0];
	buffer5[1][bufferoffset_kvs + index] = vdatas[1];
	buffer5[2][bufferoffset_kvs + index] = vdatas[2];
	buffer5[3][bufferoffset_kvs + index] = vdatas[3];
	buffer5[4][bufferoffset_kvs + index] = vdatas[4];
	buffer5[5][bufferoffset_kvs + index] = vdatas[5];
	buffer5[6][bufferoffset_kvs + index] = vdatas[6];
	buffer5[7][bufferoffset_kvs + index] = vdatas[7];
	buffer5[8][bufferoffset_kvs + index] = vdatas[8];
	buffer5[9][bufferoffset_kvs + index] = vdatas[9];
	buffer5[10][bufferoffset_kvs + index] = vdatas[10];
	buffer5[11][bufferoffset_kvs + index] = vdatas[11];
	buffer5[12][bufferoffset_kvs + index] = vdatas[12];
	buffer5[13][bufferoffset_kvs + index] = vdatas[13];
	buffer5[14][bufferoffset_kvs + index] = vdatas[14];
	buffer5[15][bufferoffset_kvs + index] = vdatas[15];
	buffer6[0][bufferoffset_kvs + index] = vdatas[0];
	buffer6[1][bufferoffset_kvs + index] = vdatas[1];
	buffer6[2][bufferoffset_kvs + index] = vdatas[2];
	buffer6[3][bufferoffset_kvs + index] = vdatas[3];
	buffer6[4][bufferoffset_kvs + index] = vdatas[4];
	buffer6[5][bufferoffset_kvs + index] = vdatas[5];
	buffer6[6][bufferoffset_kvs + index] = vdatas[6];
	buffer6[7][bufferoffset_kvs + index] = vdatas[7];
	buffer6[8][bufferoffset_kvs + index] = vdatas[8];
	buffer6[9][bufferoffset_kvs + index] = vdatas[9];
	buffer6[10][bufferoffset_kvs + index] = vdatas[10];
	buffer6[11][bufferoffset_kvs + index] = vdatas[11];
	buffer6[12][bufferoffset_kvs + index] = vdatas[12];
	buffer6[13][bufferoffset_kvs + index] = vdatas[13];
	buffer6[14][bufferoffset_kvs + index] = vdatas[14];
	buffer6[15][bufferoffset_kvs + index] = vdatas[15];
	buffer7[0][bufferoffset_kvs + index] = vdatas[0];
	buffer7[1][bufferoffset_kvs + index] = vdatas[1];
	buffer7[2][bufferoffset_kvs + index] = vdatas[2];
	buffer7[3][bufferoffset_kvs + index] = vdatas[3];
	buffer7[4][bufferoffset_kvs + index] = vdatas[4];
	buffer7[5][bufferoffset_kvs + index] = vdatas[5];
	buffer7[6][bufferoffset_kvs + index] = vdatas[6];
	buffer7[7][bufferoffset_kvs + index] = vdatas[7];
	buffer7[8][bufferoffset_kvs + index] = vdatas[8];
	buffer7[9][bufferoffset_kvs + index] = vdatas[9];
	buffer7[10][bufferoffset_kvs + index] = vdatas[10];
	buffer7[11][bufferoffset_kvs + index] = vdatas[11];
	buffer7[12][bufferoffset_kvs + index] = vdatas[12];
	buffer7[13][bufferoffset_kvs + index] = vdatas[13];
	buffer7[14][bufferoffset_kvs + index] = vdatas[14];
	buffer7[15][bufferoffset_kvs + index] = vdatas[15];
	buffer8[0][bufferoffset_kvs + index] = vdatas[0];
	buffer8[1][bufferoffset_kvs + index] = vdatas[1];
	buffer8[2][bufferoffset_kvs + index] = vdatas[2];
	buffer8[3][bufferoffset_kvs + index] = vdatas[3];
	buffer8[4][bufferoffset_kvs + index] = vdatas[4];
	buffer8[5][bufferoffset_kvs + index] = vdatas[5];
	buffer8[6][bufferoffset_kvs + index] = vdatas[6];
	buffer8[7][bufferoffset_kvs + index] = vdatas[7];
	buffer8[8][bufferoffset_kvs + index] = vdatas[8];
	buffer8[9][bufferoffset_kvs + index] = vdatas[9];
	buffer8[10][bufferoffset_kvs + index] = vdatas[10];
	buffer8[11][bufferoffset_kvs + index] = vdatas[11];
	buffer8[12][bufferoffset_kvs + index] = vdatas[12];
	buffer8[13][bufferoffset_kvs + index] = vdatas[13];
	buffer8[14][bufferoffset_kvs + index] = vdatas[14];
	buffer8[15][bufferoffset_kvs + index] = vdatas[15];
	buffer9[0][bufferoffset_kvs + index] = vdatas[0];
	buffer9[1][bufferoffset_kvs + index] = vdatas[1];
	buffer9[2][bufferoffset_kvs + index] = vdatas[2];
	buffer9[3][bufferoffset_kvs + index] = vdatas[3];
	buffer9[4][bufferoffset_kvs + index] = vdatas[4];
	buffer9[5][bufferoffset_kvs + index] = vdatas[5];
	buffer9[6][bufferoffset_kvs + index] = vdatas[6];
	buffer9[7][bufferoffset_kvs + index] = vdatas[7];
	buffer9[8][bufferoffset_kvs + index] = vdatas[8];
	buffer9[9][bufferoffset_kvs + index] = vdatas[9];
	buffer9[10][bufferoffset_kvs + index] = vdatas[10];
	buffer9[11][bufferoffset_kvs + index] = vdatas[11];
	buffer9[12][bufferoffset_kvs + index] = vdatas[12];
	buffer9[13][bufferoffset_kvs + index] = vdatas[13];
	buffer9[14][bufferoffset_kvs + index] = vdatas[14];
	buffer9[15][bufferoffset_kvs + index] = vdatas[15];
	buffer10[0][bufferoffset_kvs + index] = vdatas[0];
	buffer10[1][bufferoffset_kvs + index] = vdatas[1];
	buffer10[2][bufferoffset_kvs + index] = vdatas[2];
	buffer10[3][bufferoffset_kvs + index] = vdatas[3];
	buffer10[4][bufferoffset_kvs + index] = vdatas[4];
	buffer10[5][bufferoffset_kvs + index] = vdatas[5];
	buffer10[6][bufferoffset_kvs + index] = vdatas[6];
	buffer10[7][bufferoffset_kvs + index] = vdatas[7];
	buffer10[8][bufferoffset_kvs + index] = vdatas[8];
	buffer10[9][bufferoffset_kvs + index] = vdatas[9];
	buffer10[10][bufferoffset_kvs + index] = vdatas[10];
	buffer10[11][bufferoffset_kvs + index] = vdatas[11];
	buffer10[12][bufferoffset_kvs + index] = vdatas[12];
	buffer10[13][bufferoffset_kvs + index] = vdatas[13];
	buffer10[14][bufferoffset_kvs + index] = vdatas[14];
	buffer10[15][bufferoffset_kvs + index] = vdatas[15];
	buffer11[0][bufferoffset_kvs + index] = vdatas[0];
	buffer11[1][bufferoffset_kvs + index] = vdatas[1];
	buffer11[2][bufferoffset_kvs + index] = vdatas[2];
	buffer11[3][bufferoffset_kvs + index] = vdatas[3];
	buffer11[4][bufferoffset_kvs + index] = vdatas[4];
	buffer11[5][bufferoffset_kvs + index] = vdatas[5];
	buffer11[6][bufferoffset_kvs + index] = vdatas[6];
	buffer11[7][bufferoffset_kvs + index] = vdatas[7];
	buffer11[8][bufferoffset_kvs + index] = vdatas[8];
	buffer11[9][bufferoffset_kvs + index] = vdatas[9];
	buffer11[10][bufferoffset_kvs + index] = vdatas[10];
	buffer11[11][bufferoffset_kvs + index] = vdatas[11];
	buffer11[12][bufferoffset_kvs + index] = vdatas[12];
	buffer11[13][bufferoffset_kvs + index] = vdatas[13];
	buffer11[14][bufferoffset_kvs + index] = vdatas[14];
	buffer11[15][bufferoffset_kvs + index] = vdatas[15];
	return;
}

void MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS1_ANDREPLICATE(unsigned int indexes[VDATA_PACKINGSIZE], keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs){			
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
	
	buffer0[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer0[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer0[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer0[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer0[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer0[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer0[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer0[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer0[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer0[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer0[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer0[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer0[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer0[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer0[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer0[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	return;
}
void MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS2_ANDREPLICATE(unsigned int indexes[VDATA_PACKINGSIZE], keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs){			
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
	
	buffer0[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer0[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer0[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer0[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer0[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer0[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer0[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer0[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer0[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer0[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer0[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer0[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer0[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer0[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer0[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer0[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer1[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer1[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer1[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer1[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer1[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer1[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer1[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer1[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer1[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer1[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer1[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer1[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer1[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer1[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer1[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer1[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	return;
}
void MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS3_ANDREPLICATE(unsigned int indexes[VDATA_PACKINGSIZE], keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs){			
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
	
	buffer0[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer0[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer0[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer0[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer0[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer0[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer0[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer0[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer0[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer0[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer0[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer0[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer0[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer0[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer0[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer0[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer1[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer1[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer1[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer1[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer1[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer1[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer1[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer1[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer1[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer1[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer1[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer1[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer1[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer1[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer1[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer1[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer2[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer2[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer2[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer2[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer2[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer2[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer2[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer2[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer2[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer2[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer2[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer2[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer2[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer2[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer2[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer2[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	return;
}
void MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS4_ANDREPLICATE(unsigned int indexes[VDATA_PACKINGSIZE], keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs){			
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
	
	buffer0[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer0[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer0[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer0[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer0[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer0[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer0[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer0[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer0[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer0[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer0[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer0[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer0[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer0[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer0[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer0[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer1[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer1[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer1[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer1[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer1[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer1[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer1[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer1[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer1[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer1[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer1[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer1[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer1[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer1[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer1[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer1[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer2[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer2[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer2[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer2[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer2[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer2[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer2[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer2[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer2[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer2[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer2[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer2[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer2[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer2[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer2[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer2[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer3[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer3[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer3[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer3[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer3[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer3[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer3[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer3[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer3[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer3[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer3[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer3[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer3[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer3[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer3[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer3[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	return;
}
void MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS5_ANDREPLICATE(unsigned int indexes[VDATA_PACKINGSIZE], keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs){			
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
	
	buffer0[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer0[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer0[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer0[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer0[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer0[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer0[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer0[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer0[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer0[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer0[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer0[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer0[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer0[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer0[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer0[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer1[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer1[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer1[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer1[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer1[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer1[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer1[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer1[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer1[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer1[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer1[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer1[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer1[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer1[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer1[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer1[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer2[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer2[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer2[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer2[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer2[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer2[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer2[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer2[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer2[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer2[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer2[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer2[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer2[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer2[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer2[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer2[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer3[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer3[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer3[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer3[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer3[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer3[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer3[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer3[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer3[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer3[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer3[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer3[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer3[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer3[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer3[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer3[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer4[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer4[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer4[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer4[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer4[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer4[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer4[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer4[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer4[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer4[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer4[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer4[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer4[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer4[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer4[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer4[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	return;
}
void MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS6_ANDREPLICATE(unsigned int indexes[VDATA_PACKINGSIZE], keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs){			
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
	
	buffer0[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer0[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer0[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer0[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer0[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer0[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer0[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer0[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer0[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer0[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer0[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer0[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer0[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer0[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer0[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer0[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer1[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer1[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer1[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer1[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer1[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer1[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer1[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer1[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer1[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer1[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer1[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer1[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer1[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer1[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer1[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer1[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer2[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer2[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer2[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer2[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer2[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer2[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer2[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer2[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer2[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer2[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer2[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer2[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer2[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer2[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer2[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer2[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer3[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer3[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer3[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer3[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer3[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer3[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer3[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer3[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer3[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer3[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer3[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer3[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer3[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer3[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer3[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer3[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer4[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer4[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer4[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer4[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer4[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer4[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer4[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer4[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer4[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer4[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer4[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer4[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer4[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer4[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer4[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer4[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer5[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer5[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer5[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer5[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer5[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer5[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer5[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer5[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer5[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer5[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer5[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer5[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer5[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer5[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer5[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer5[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	return;
}
void MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS7_ANDREPLICATE(unsigned int indexes[VDATA_PACKINGSIZE], keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs){			
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
	
	buffer0[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer0[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer0[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer0[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer0[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer0[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer0[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer0[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer0[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer0[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer0[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer0[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer0[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer0[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer0[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer0[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer1[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer1[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer1[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer1[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer1[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer1[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer1[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer1[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer1[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer1[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer1[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer1[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer1[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer1[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer1[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer1[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer2[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer2[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer2[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer2[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer2[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer2[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer2[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer2[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer2[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer2[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer2[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer2[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer2[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer2[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer2[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer2[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer3[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer3[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer3[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer3[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer3[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer3[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer3[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer3[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer3[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer3[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer3[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer3[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer3[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer3[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer3[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer3[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer4[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer4[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer4[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer4[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer4[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer4[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer4[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer4[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer4[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer4[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer4[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer4[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer4[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer4[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer4[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer4[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer5[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer5[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer5[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer5[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer5[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer5[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer5[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer5[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer5[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer5[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer5[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer5[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer5[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer5[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer5[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer5[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer6[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer6[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer6[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer6[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer6[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer6[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer6[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer6[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer6[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer6[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer6[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer6[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer6[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer6[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer6[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer6[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	return;
}
void MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS8_ANDREPLICATE(unsigned int indexes[VDATA_PACKINGSIZE], keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs){			
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
	
	buffer0[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer0[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer0[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer0[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer0[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer0[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer0[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer0[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer0[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer0[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer0[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer0[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer0[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer0[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer0[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer0[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer1[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer1[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer1[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer1[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer1[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer1[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer1[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer1[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer1[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer1[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer1[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer1[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer1[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer1[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer1[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer1[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer2[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer2[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer2[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer2[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer2[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer2[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer2[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer2[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer2[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer2[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer2[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer2[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer2[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer2[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer2[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer2[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer3[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer3[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer3[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer3[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer3[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer3[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer3[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer3[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer3[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer3[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer3[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer3[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer3[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer3[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer3[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer3[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer4[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer4[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer4[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer4[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer4[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer4[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer4[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer4[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer4[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer4[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer4[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer4[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer4[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer4[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer4[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer4[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer5[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer5[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer5[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer5[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer5[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer5[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer5[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer5[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer5[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer5[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer5[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer5[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer5[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer5[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer5[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer5[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer6[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer6[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer6[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer6[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer6[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer6[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer6[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer6[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer6[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer6[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer6[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer6[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer6[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer6[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer6[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer6[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer7[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer7[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer7[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer7[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer7[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer7[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer7[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer7[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer7[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer7[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer7[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer7[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer7[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer7[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer7[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer7[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	return;
}
void MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS9_ANDREPLICATE(unsigned int indexes[VDATA_PACKINGSIZE], keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs){			
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
	
	buffer0[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer0[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer0[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer0[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer0[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer0[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer0[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer0[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer0[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer0[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer0[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer0[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer0[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer0[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer0[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer0[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer1[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer1[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer1[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer1[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer1[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer1[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer1[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer1[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer1[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer1[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer1[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer1[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer1[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer1[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer1[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer1[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer2[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer2[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer2[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer2[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer2[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer2[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer2[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer2[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer2[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer2[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer2[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer2[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer2[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer2[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer2[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer2[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer3[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer3[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer3[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer3[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer3[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer3[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer3[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer3[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer3[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer3[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer3[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer3[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer3[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer3[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer3[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer3[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer4[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer4[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer4[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer4[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer4[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer4[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer4[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer4[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer4[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer4[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer4[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer4[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer4[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer4[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer4[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer4[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer5[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer5[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer5[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer5[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer5[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer5[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer5[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer5[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer5[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer5[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer5[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer5[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer5[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer5[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer5[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer5[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer6[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer6[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer6[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer6[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer6[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer6[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer6[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer6[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer6[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer6[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer6[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer6[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer6[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer6[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer6[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer6[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer7[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer7[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer7[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer7[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer7[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer7[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer7[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer7[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer7[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer7[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer7[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer7[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer7[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer7[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer7[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer7[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer8[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer8[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer8[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer8[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer8[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer8[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer8[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer8[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer8[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer8[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer8[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer8[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer8[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer8[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer8[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer8[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	return;
}
void MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS10_ANDREPLICATE(unsigned int indexes[VDATA_PACKINGSIZE], keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs){			
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
	
	buffer0[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer0[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer0[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer0[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer0[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer0[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer0[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer0[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer0[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer0[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer0[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer0[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer0[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer0[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer0[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer0[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer1[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer1[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer1[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer1[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer1[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer1[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer1[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer1[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer1[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer1[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer1[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer1[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer1[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer1[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer1[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer1[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer2[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer2[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer2[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer2[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer2[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer2[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer2[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer2[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer2[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer2[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer2[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer2[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer2[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer2[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer2[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer2[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer3[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer3[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer3[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer3[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer3[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer3[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer3[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer3[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer3[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer3[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer3[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer3[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer3[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer3[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer3[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer3[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer4[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer4[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer4[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer4[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer4[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer4[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer4[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer4[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer4[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer4[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer4[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer4[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer4[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer4[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer4[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer4[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer5[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer5[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer5[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer5[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer5[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer5[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer5[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer5[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer5[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer5[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer5[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer5[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer5[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer5[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer5[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer5[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer6[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer6[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer6[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer6[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer6[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer6[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer6[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer6[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer6[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer6[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer6[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer6[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer6[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer6[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer6[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer6[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer7[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer7[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer7[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer7[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer7[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer7[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer7[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer7[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer7[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer7[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer7[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer7[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer7[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer7[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer7[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer7[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer8[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer8[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer8[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer8[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer8[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer8[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer8[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer8[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer8[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer8[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer8[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer8[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer8[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer8[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer8[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer8[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer9[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer9[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer9[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer9[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer9[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer9[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer9[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer9[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer9[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer9[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer9[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer9[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer9[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer9[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer9[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer9[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	return;
}
void MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS11_ANDREPLICATE(unsigned int indexes[VDATA_PACKINGSIZE], keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer10[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs){			
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
	
	buffer0[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer0[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer0[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer0[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer0[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer0[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer0[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer0[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer0[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer0[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer0[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer0[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer0[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer0[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer0[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer0[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer1[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer1[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer1[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer1[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer1[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer1[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer1[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer1[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer1[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer1[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer1[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer1[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer1[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer1[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer1[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer1[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer2[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer2[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer2[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer2[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer2[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer2[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer2[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer2[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer2[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer2[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer2[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer2[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer2[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer2[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer2[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer2[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer3[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer3[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer3[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer3[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer3[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer3[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer3[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer3[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer3[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer3[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer3[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer3[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer3[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer3[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer3[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer3[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer4[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer4[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer4[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer4[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer4[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer4[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer4[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer4[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer4[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer4[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer4[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer4[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer4[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer4[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer4[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer4[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer5[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer5[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer5[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer5[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer5[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer5[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer5[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer5[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer5[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer5[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer5[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer5[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer5[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer5[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer5[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer5[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer6[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer6[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer6[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer6[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer6[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer6[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer6[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer6[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer6[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer6[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer6[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer6[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer6[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer6[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer6[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer6[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer7[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer7[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer7[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer7[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer7[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer7[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer7[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer7[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer7[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer7[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer7[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer7[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer7[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer7[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer7[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer7[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer8[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer8[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer8[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer8[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer8[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer8[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer8[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer8[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer8[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer8[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer8[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer8[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer8[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer8[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer8[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer8[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer9[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer9[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer9[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer9[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer9[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer9[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer9[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer9[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer9[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer9[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer9[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer9[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer9[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer9[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer9[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer9[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer10[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer10[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer10[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer10[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer10[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer10[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer10[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer10[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer10[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer10[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer10[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer10[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer10[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer10[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer10[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer10[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	return;
}
void MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS12_ANDREPLICATE(unsigned int indexes[VDATA_PACKINGSIZE], keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer10[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer11[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs){			
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
	
	buffer0[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer0[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer0[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer0[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer0[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer0[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer0[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer0[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer0[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer0[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer0[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer0[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer0[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer0[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer0[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer0[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer1[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer1[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer1[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer1[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer1[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer1[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer1[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer1[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer1[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer1[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer1[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer1[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer1[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer1[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer1[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer1[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer2[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer2[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer2[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer2[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer2[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer2[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer2[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer2[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer2[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer2[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer2[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer2[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer2[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer2[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer2[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer2[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer3[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer3[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer3[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer3[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer3[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer3[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer3[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer3[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer3[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer3[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer3[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer3[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer3[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer3[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer3[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer3[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer4[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer4[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer4[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer4[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer4[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer4[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer4[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer4[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer4[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer4[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer4[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer4[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer4[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer4[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer4[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer4[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer5[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer5[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer5[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer5[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer5[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer5[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer5[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer5[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer5[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer5[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer5[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer5[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer5[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer5[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer5[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer5[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer6[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer6[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer6[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer6[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer6[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer6[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer6[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer6[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer6[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer6[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer6[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer6[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer6[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer6[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer6[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer6[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer7[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer7[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer7[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer7[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer7[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer7[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer7[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer7[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer7[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer7[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer7[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer7[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer7[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer7[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer7[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer7[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer8[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer8[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer8[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer8[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer8[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer8[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer8[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer8[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer8[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer8[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer8[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer8[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer8[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer8[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer8[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer8[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer9[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer9[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer9[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer9[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer9[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer9[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer9[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer9[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer9[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer9[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer9[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer9[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer9[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer9[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer9[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer9[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer10[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer10[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer10[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer10[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer10[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer10[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer10[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer10[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer10[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer10[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer10[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer10[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer10[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer10[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer10[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer10[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	buffer11[0][bufferoffset_kvs + indexes[0]] = vdatas[0];
	buffer11[1][bufferoffset_kvs + indexes[1]] = vdatas[1];
	buffer11[2][bufferoffset_kvs + indexes[2]] = vdatas[2];
	buffer11[3][bufferoffset_kvs + indexes[3]] = vdatas[3];
	buffer11[4][bufferoffset_kvs + indexes[4]] = vdatas[4];
	buffer11[5][bufferoffset_kvs + indexes[5]] = vdatas[5];
	buffer11[6][bufferoffset_kvs + indexes[6]] = vdatas[6];
	buffer11[7][bufferoffset_kvs + indexes[7]] = vdatas[7];
	buffer11[8][bufferoffset_kvs + indexes[8]] = vdatas[8];
	buffer11[9][bufferoffset_kvs + indexes[9]] = vdatas[9];
	buffer11[10][bufferoffset_kvs + indexes[10]] = vdatas[10];
	buffer11[11][bufferoffset_kvs + indexes[11]] = vdatas[11];
	buffer11[12][bufferoffset_kvs + indexes[12]] = vdatas[12];
	buffer11[13][bufferoffset_kvs + indexes[13]] = vdatas[13];
	buffer11[14][bufferoffset_kvs + indexes[14]] = vdatas[14];
	buffer11[15][bufferoffset_kvs + indexes[15]] = vdatas[15];
	
	return;
}

// kvdram 
void MEMCAP0_READFROMKVDRAM_VDATASANDVMASKS(unsigned int index, uint512_dt * kvdram, keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("MEMCAP0_READFROMKVDRAM_VDATASANDVMASKS:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	#ifdef _WIDEWORD // CRITICAL FIXME.
	vdatas[0] = kvdram[baseoffset_kvs + offset_kvs + index].range(31, 0); 
	vdatas[1] = kvdram[baseoffset_kvs + offset_kvs + index].range(63, 32); 
	vdatas[2] = kvdram[baseoffset_kvs + offset_kvs + index].range(95, 64); 
	vdatas[3] = kvdram[baseoffset_kvs + offset_kvs + index].range(127, 96); 
	vdatas[4] = kvdram[baseoffset_kvs + offset_kvs + index].range(159, 128); 
	vdatas[5] = kvdram[baseoffset_kvs + offset_kvs + index].range(191, 160); 
	vdatas[6] = kvdram[baseoffset_kvs + offset_kvs + index].range(223, 192); 
	vdatas[7] = kvdram[baseoffset_kvs + offset_kvs + index].range(255, 224); 
	vdatas[8] = kvdram[baseoffset_kvs + offset_kvs + index].range(287, 256); 
	vdatas[9] = kvdram[baseoffset_kvs + offset_kvs + index].range(319, 288); 
	vdatas[10] = kvdram[baseoffset_kvs + offset_kvs + index].range(351, 320); 
	vdatas[11] = kvdram[baseoffset_kvs + offset_kvs + index].range(383, 352); 
	vdatas[12] = kvdram[baseoffset_kvs + offset_kvs + index].range(415, 384); 
	vdatas[13] = kvdram[baseoffset_kvs + offset_kvs + index].range(447, 416); 
	vdatas[14] = kvdram[baseoffset_kvs + offset_kvs + index].range(479, 448); 
	vdatas[15] = kvdram[baseoffset_kvs + offset_kvs + index].range(511, 480); 
	#else 
	vdatas[0] = kvdram[baseoffset_kvs + offset_kvs + index].data[0].key;
	vdatas[1] = kvdram[baseoffset_kvs + offset_kvs + index].data[0].value; 
	vdatas[2] = kvdram[baseoffset_kvs + offset_kvs + index].data[1].key;
	vdatas[3] = kvdram[baseoffset_kvs + offset_kvs + index].data[1].value; 
	vdatas[4] = kvdram[baseoffset_kvs + offset_kvs + index].data[2].key;
	vdatas[5] = kvdram[baseoffset_kvs + offset_kvs + index].data[2].value; 
	vdatas[6] = kvdram[baseoffset_kvs + offset_kvs + index].data[3].key;
	vdatas[7] = kvdram[baseoffset_kvs + offset_kvs + index].data[3].value; 
	vdatas[8] = kvdram[baseoffset_kvs + offset_kvs + index].data[4].key;
	vdatas[9] = kvdram[baseoffset_kvs + offset_kvs + index].data[4].value; 
	vdatas[10] = kvdram[baseoffset_kvs + offset_kvs + index].data[5].key;
	vdatas[11] = kvdram[baseoffset_kvs + offset_kvs + index].data[5].value; 
	vdatas[12] = kvdram[baseoffset_kvs + offset_kvs + index].data[6].key;
	vdatas[13] = kvdram[baseoffset_kvs + offset_kvs + index].data[6].value; 
	vdatas[14] = kvdram[baseoffset_kvs + offset_kvs + index].data[7].key;
	vdatas[15] = kvdram[baseoffset_kvs + offset_kvs + index].data[7].value; 
	#endif
	return;
}

void MEMCAP0_WRITETOKVDRAM_VDATASANDVMASKS(unsigned int index, uint512_dt * kvdram, keyvalue_vbuffer_t vdatas[VECTOR2_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs){			
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("MEMCAP0_WRITETOKVDRAM_VDATASANDVMASKS:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif

	#ifdef _WIDEWORD // CRITICAL FIXME.
	kvdram[baseoffset_kvs + offset_kvs + index].range(31, 0) = vdatas[0];
	kvdram[baseoffset_kvs + offset_kvs + index].range(63, 32) = vdatas[1];
	kvdram[baseoffset_kvs + offset_kvs + index].range(95, 64) = vdatas[2];
	kvdram[baseoffset_kvs + offset_kvs + index].range(127, 96) = vdatas[3];
	kvdram[baseoffset_kvs + offset_kvs + index].range(159, 128) = vdatas[4];
	kvdram[baseoffset_kvs + offset_kvs + index].range(191, 160) = vdatas[5];
	kvdram[baseoffset_kvs + offset_kvs + index].range(223, 192) = vdatas[6];
	kvdram[baseoffset_kvs + offset_kvs + index].range(255, 224) = vdatas[7];
	kvdram[baseoffset_kvs + offset_kvs + index].range(287, 256) = vdatas[8];
	kvdram[baseoffset_kvs + offset_kvs + index].range(319, 288) = vdatas[9];
	kvdram[baseoffset_kvs + offset_kvs + index].range(351, 320) = vdatas[10];
	kvdram[baseoffset_kvs + offset_kvs + index].range(383, 352) = vdatas[11];
	kvdram[baseoffset_kvs + offset_kvs + index].range(415, 384) = vdatas[12];
	kvdram[baseoffset_kvs + offset_kvs + index].range(447, 416) = vdatas[13];
	kvdram[baseoffset_kvs + offset_kvs + index].range(479, 448) = vdatas[14];
	kvdram[baseoffset_kvs + offset_kvs + index].range(511, 480) = vdatas[15];
	#else 
	kvdram[baseoffset_kvs + offset_kvs + index].data[0].key = vdatas[0];
	kvdram[baseoffset_kvs + offset_kvs + index].data[0].value = vdatas[1];
	kvdram[baseoffset_kvs + offset_kvs + index].data[1].key = vdatas[2];
	kvdram[baseoffset_kvs + offset_kvs + index].data[1].value = vdatas[3];
	kvdram[baseoffset_kvs + offset_kvs + index].data[2].key = vdatas[4];
	kvdram[baseoffset_kvs + offset_kvs + index].data[2].value = vdatas[5];
	kvdram[baseoffset_kvs + offset_kvs + index].data[3].key = vdatas[6];
	kvdram[baseoffset_kvs + offset_kvs + index].data[3].value = vdatas[7];
	kvdram[baseoffset_kvs + offset_kvs + index].data[4].key = vdatas[8];
	kvdram[baseoffset_kvs + offset_kvs + index].data[4].value = vdatas[9];
	kvdram[baseoffset_kvs + offset_kvs + index].data[5].key = vdatas[10];
	kvdram[baseoffset_kvs + offset_kvs + index].data[5].value = vdatas[11];
	kvdram[baseoffset_kvs + offset_kvs + index].data[6].key = vdatas[12];
	kvdram[baseoffset_kvs + offset_kvs + index].data[6].value = vdatas[13];
	kvdram[baseoffset_kvs + offset_kvs + index].data[7].key = vdatas[14];
	kvdram[baseoffset_kvs + offset_kvs + index].data[7].value = vdatas[15];
	#endif
	return;
}