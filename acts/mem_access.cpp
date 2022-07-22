void acts_all::MEMACCESSP0_readV(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs, batch_type bufferoffset_kvs, buffer_type size_kvs, globalposition_t globalposition, globalparams_t globalparams){
	if(enable == OFF){ return; }

	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	value_t datas[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=datas complete
	value_t datas2[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=datas2 complete
	
	READVDATA_LOOP1: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS PIPELINE II=1
		UTILP0_ReadDatas(kvdram, baseoffset_kvs + offset_kvs + i, datas);
	
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
		for(unsigned int v=0; v<VECTOR2_SIZE; v++){ 
			value_t vdata = datas[v] >> 1; value_t mask = datas[v] & 0x1;
			if(vdata < 1000){ cout<<"readV: ACTIVE VDATA SEEN: @ i: "<<i<<" v: "<<v<<", vdata: "<<vdata<<", mask: "<<mask<<endl; }}
		#endif
		
		// clear masks
		for(unsigned int v=0; v<VECTOR2_SIZE; v++){ 
		#pragma HLS UNROLL
			datas2[v] = datas[v] & 0xFFFFFFFE; 
			// datas2[v] = datas[v]; // CRITICAL REMOVEME.
		}
		// for(unsigned int v=0; v<VECTOR2_SIZE; v++){ datas2[v] = datas[v]; }
		
		#ifdef BIT_TRAVERSAL_ALGORITHM
  vdata[0].data[0] = datas2[0];  vdata[0].data[1] = datas2[0];  vdata[0].data[2] = datas2[0];  vdata[0].data[3] = datas2[0];  vdata[0].data[4] = datas2[0];  vdata[0].data[5] = datas2[0];  vdata[0].data[6] = datas2[0];  vdata[0].data[7] = datas2[0];  vdata[0].data[8] = datas2[0];  vdata[0].data[9] = datas2[0];  vdata[0].data[10] = datas2[0];  vdata[0].data[11] = datas2[0];  vdata[0].data[12] = datas2[0];  vdata[0].data[13] = datas2[0];  vdata[0].data[14] = datas2[0];  vdata[0].data[15] = datas2[0];    vdata[1].data[0] = datas2[1];  vdata[1].data[1] = datas2[1];  vdata[1].data[2] = datas2[1];  vdata[1].data[3] = datas2[1];  vdata[1].data[4] = datas2[1];  vdata[1].data[5] = datas2[1];  vdata[1].data[6] = datas2[1];  vdata[1].data[7] = datas2[1];  vdata[1].data[8] = datas2[1];  vdata[1].data[9] = datas2[1];  vdata[1].data[10] = datas2[1];  vdata[1].data[11] = datas2[1];  vdata[1].data[12] = datas2[1];  vdata[1].data[13] = datas2[1];  vdata[1].data[14] = datas2[1];  vdata[1].data[15] = datas2[1];    vdata[2].data[0] = datas2[2];  vdata[2].data[1] = datas2[2];  vdata[2].data[2] = datas2[2];  vdata[2].data[3] = datas2[2];  vdata[2].data[4] = datas2[2];  vdata[2].data[5] = datas2[2];  vdata[2].data[6] = datas2[2];  vdata[2].data[7] = datas2[2];  vdata[2].data[8] = datas2[2];  vdata[2].data[9] = datas2[2];  vdata[2].data[10] = datas2[2];  vdata[2].data[11] = datas2[2];  vdata[2].data[12] = datas2[2];  vdata[2].data[13] = datas2[2];  vdata[2].data[14] = datas2[2];  vdata[2].data[15] = datas2[2];    vdata[3].data[0] = datas2[3];  vdata[3].data[1] = datas2[3];  vdata[3].data[2] = datas2[3];  vdata[3].data[3] = datas2[3];  vdata[3].data[4] = datas2[3];  vdata[3].data[5] = datas2[3];  vdata[3].data[6] = datas2[3];  vdata[3].data[7] = datas2[3];  vdata[3].data[8] = datas2[3];  vdata[3].data[9] = datas2[3];  vdata[3].data[10] = datas2[3];  vdata[3].data[11] = datas2[3];  vdata[3].data[12] = datas2[3];  vdata[3].data[13] = datas2[3];  vdata[3].data[14] = datas2[3];  vdata[3].data[15] = datas2[3];    vdata[4].data[0] = datas2[4];  vdata[4].data[1] = datas2[4];  vdata[4].data[2] = datas2[4];  vdata[4].data[3] = datas2[4];  vdata[4].data[4] = datas2[4];  vdata[4].data[5] = datas2[4];  vdata[4].data[6] = datas2[4];  vdata[4].data[7] = datas2[4];  vdata[4].data[8] = datas2[4];  vdata[4].data[9] = datas2[4];  vdata[4].data[10] = datas2[4];  vdata[4].data[11] = datas2[4];  vdata[4].data[12] = datas2[4];  vdata[4].data[13] = datas2[4];  vdata[4].data[14] = datas2[4];  vdata[4].data[15] = datas2[4];    vdata[5].data[0] = datas2[5];  vdata[5].data[1] = datas2[5];  vdata[5].data[2] = datas2[5];  vdata[5].data[3] = datas2[5];  vdata[5].data[4] = datas2[5];  vdata[5].data[5] = datas2[5];  vdata[5].data[6] = datas2[5];  vdata[5].data[7] = datas2[5];  vdata[5].data[8] = datas2[5];  vdata[5].data[9] = datas2[5];  vdata[5].data[10] = datas2[5];  vdata[5].data[11] = datas2[5];  vdata[5].data[12] = datas2[5];  vdata[5].data[13] = datas2[5];  vdata[5].data[14] = datas2[5];  vdata[5].data[15] = datas2[5];    vdata[6].data[0] = datas2[6];  vdata[6].data[1] = datas2[6];  vdata[6].data[2] = datas2[6];  vdata[6].data[3] = datas2[6];  vdata[6].data[4] = datas2[6];  vdata[6].data[5] = datas2[6];  vdata[6].data[6] = datas2[6];  vdata[6].data[7] = datas2[6];  vdata[6].data[8] = datas2[6];  vdata[6].data[9] = datas2[6];  vdata[6].data[10] = datas2[6];  vdata[6].data[11] = datas2[6];  vdata[6].data[12] = datas2[6];  vdata[6].data[13] = datas2[6];  vdata[6].data[14] = datas2[6];  vdata[6].data[15] = datas2[6];    vdata[7].data[0] = datas2[7];  vdata[7].data[1] = datas2[7];  vdata[7].data[2] = datas2[7];  vdata[7].data[3] = datas2[7];  vdata[7].data[4] = datas2[7];  vdata[7].data[5] = datas2[7];  vdata[7].data[6] = datas2[7];  vdata[7].data[7] = datas2[7];  vdata[7].data[8] = datas2[7];  vdata[7].data[9] = datas2[7];  vdata[7].data[10] = datas2[7];  vdata[7].data[11] = datas2[7];  vdata[7].data[12] = datas2[7];  vdata[7].data[13] = datas2[7];  vdata[7].data[14] = datas2[7];  vdata[7].data[15] = datas2[7];    vdata[8].data[0] = datas2[8];  vdata[8].data[1] = datas2[8];  vdata[8].data[2] = datas2[8];  vdata[8].data[3] = datas2[8];  vdata[8].data[4] = datas2[8];  vdata[8].data[5] = datas2[8];  vdata[8].data[6] = datas2[8];  vdata[8].data[7] = datas2[8];  vdata[8].data[8] = datas2[8];  vdata[8].data[9] = datas2[8];  vdata[8].data[10] = datas2[8];  vdata[8].data[11] = datas2[8];  vdata[8].data[12] = datas2[8];  vdata[8].data[13] = datas2[8];  vdata[8].data[14] = datas2[8];  vdata[8].data[15] = datas2[8];    vdata[9].data[0] = datas2[9];  vdata[9].data[1] = datas2[9];  vdata[9].data[2] = datas2[9];  vdata[9].data[3] = datas2[9];  vdata[9].data[4] = datas2[9];  vdata[9].data[5] = datas2[9];  vdata[9].data[6] = datas2[9];  vdata[9].data[7] = datas2[9];  vdata[9].data[8] = datas2[9];  vdata[9].data[9] = datas2[9];  vdata[9].data[10] = datas2[9];  vdata[9].data[11] = datas2[9];  vdata[9].data[12] = datas2[9];  vdata[9].data[13] = datas2[9];  vdata[9].data[14] = datas2[9];  vdata[9].data[15] = datas2[9];    vdata[10].data[0] = datas2[10];  vdata[10].data[1] = datas2[10];  vdata[10].data[2] = datas2[10];  vdata[10].data[3] = datas2[10];  vdata[10].data[4] = datas2[10];  vdata[10].data[5] = datas2[10];  vdata[10].data[6] = datas2[10];  vdata[10].data[7] = datas2[10];  vdata[10].data[8] = datas2[10];  vdata[10].data[9] = datas2[10];  vdata[10].data[10] = datas2[10];  vdata[10].data[11] = datas2[10];  vdata[10].data[12] = datas2[10];  vdata[10].data[13] = datas2[10];  vdata[10].data[14] = datas2[10];  vdata[10].data[15] = datas2[10];    vdata[11].data[0] = datas2[11];  vdata[11].data[1] = datas2[11];  vdata[11].data[2] = datas2[11];  vdata[11].data[3] = datas2[11];  vdata[11].data[4] = datas2[11];  vdata[11].data[5] = datas2[11];  vdata[11].data[6] = datas2[11];  vdata[11].data[7] = datas2[11];  vdata[11].data[8] = datas2[11];  vdata[11].data[9] = datas2[11];  vdata[11].data[10] = datas2[11];  vdata[11].data[11] = datas2[11];  vdata[11].data[12] = datas2[11];  vdata[11].data[13] = datas2[11];  vdata[11].data[14] = datas2[11];  vdata[11].data[15] = datas2[11];    vdata[12].data[0] = datas2[12];  vdata[12].data[1] = datas2[12];  vdata[12].data[2] = datas2[12];  vdata[12].data[3] = datas2[12];  vdata[12].data[4] = datas2[12];  vdata[12].data[5] = datas2[12];  vdata[12].data[6] = datas2[12];  vdata[12].data[7] = datas2[12];  vdata[12].data[8] = datas2[12];  vdata[12].data[9] = datas2[12];  vdata[12].data[10] = datas2[12];  vdata[12].data[11] = datas2[12];  vdata[12].data[12] = datas2[12];  vdata[12].data[13] = datas2[12];  vdata[12].data[14] = datas2[12];  vdata[12].data[15] = datas2[12];    vdata[13].data[0] = datas2[13];  vdata[13].data[1] = datas2[13];  vdata[13].data[2] = datas2[13];  vdata[13].data[3] = datas2[13];  vdata[13].data[4] = datas2[13];  vdata[13].data[5] = datas2[13];  vdata[13].data[6] = datas2[13];  vdata[13].data[7] = datas2[13];  vdata[13].data[8] = datas2[13];  vdata[13].data[9] = datas2[13];  vdata[13].data[10] = datas2[13];  vdata[13].data[11] = datas2[13];  vdata[13].data[12] = datas2[13];  vdata[13].data[13] = datas2[13];  vdata[13].data[14] = datas2[13];  vdata[13].data[15] = datas2[13];    vdata[14].data[0] = datas2[14];  vdata[14].data[1] = datas2[14];  vdata[14].data[2] = datas2[14];  vdata[14].data[3] = datas2[14];  vdata[14].data[4] = datas2[14];  vdata[14].data[5] = datas2[14];  vdata[14].data[6] = datas2[14];  vdata[14].data[7] = datas2[14];  vdata[14].data[8] = datas2[14];  vdata[14].data[9] = datas2[14];  vdata[14].data[10] = datas2[14];  vdata[14].data[11] = datas2[14];  vdata[14].data[12] = datas2[14];  vdata[14].data[13] = datas2[14];  vdata[14].data[14] = datas2[14];  vdata[14].data[15] = datas2[14];    vdata[15].data[0] = datas2[15];  vdata[15].data[1] = datas2[15];  vdata[15].data[2] = datas2[15];  vdata[15].data[3] = datas2[15];  vdata[15].data[4] = datas2[15];  vdata[15].data[5] = datas2[15];  vdata[15].data[6] = datas2[15];  vdata[15].data[7] = datas2[15];  vdata[15].data[8] = datas2[15];  vdata[15].data[9] = datas2[15];  vdata[15].data[10] = datas2[15];  vdata[15].data[11] = datas2[15];  vdata[15].data[12] = datas2[15];  vdata[15].data[13] = datas2[15];  vdata[15].data[14] = datas2[15];  vdata[15].data[15] = datas2[15];   // FIXME.
		#else 
 vdata[0].data = datas2[0];  vdata[1].data = datas2[1];  vdata[2].data = datas2[2];  vdata[3].data = datas2[3];  vdata[4].data = datas2[4];  vdata[5].data = datas2[5];  vdata[6].data = datas2[6];  vdata[7].data = datas2[7];  vdata[8].data = datas2[8];  vdata[9].data = datas2[9];  vdata[10].data = datas2[10];  vdata[11].data = datas2[11];  vdata[12].data = datas2[12];  vdata[13].data = datas2[13];  vdata[14].data = datas2[14];  vdata[15].data = datas2[15]; 	
		#endif 
		
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MEMACCESSRRRP0_readV 21", bufferoffset_kvs + i, MAX_BLOCKRAM_VDESTDATA_SIZE, size_kvs, bufferoffset_kvs, i);
		#endif
		buffer[0][bufferoffset_kvs + i] = vdata[0];
		buffer[1][bufferoffset_kvs + i] = vdata[1];
		buffer[2][bufferoffset_kvs + i] = vdata[2];
		buffer[3][bufferoffset_kvs + i] = vdata[3];
		buffer[4][bufferoffset_kvs + i] = vdata[4];
		buffer[5][bufferoffset_kvs + i] = vdata[5];
		buffer[6][bufferoffset_kvs + i] = vdata[6];
		buffer[7][bufferoffset_kvs + i] = vdata[7];
		buffer[8][bufferoffset_kvs + i] = vdata[8];
		buffer[9][bufferoffset_kvs + i] = vdata[9];
		buffer[10][bufferoffset_kvs + i] = vdata[10];
		buffer[11][bufferoffset_kvs + i] = vdata[11];
		buffer[12][bufferoffset_kvs + i] = vdata[12];
		buffer[13][bufferoffset_kvs + i] = vdata[13];
		buffer[14][bufferoffset_kvs + i] = vdata[14];
		buffer[15][bufferoffset_kvs + i] = vdata[15];
	}
	return;
}

void acts_all::MEMACCESSP0_saveV(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs, batch_type bufferoffset_kvs, buffer_type size_kvs, globalposition_t globalposition, globalparams_t globalparams){				
	if(enable == OFF){ return; }
	
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	value_t datas[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	SAVEVDATA_LOOP1: for(buffer_type i=0; i<size_kvs; i++){
	#pragma HLS PIPELINE II=1
	
		vdata[0] = vbuffer[0][bufferoffset_kvs + i];
	
		vdata[1] = vbuffer[1][bufferoffset_kvs + i];
	
		vdata[2] = vbuffer[2][bufferoffset_kvs + i];
	
		vdata[3] = vbuffer[3][bufferoffset_kvs + i];
	
		vdata[4] = vbuffer[4][bufferoffset_kvs + i];
	
		vdata[5] = vbuffer[5][bufferoffset_kvs + i];
	
		vdata[6] = vbuffer[6][bufferoffset_kvs + i];
	
		vdata[7] = vbuffer[7][bufferoffset_kvs + i];
	
		vdata[8] = vbuffer[8][bufferoffset_kvs + i];
	
		vdata[9] = vbuffer[9][bufferoffset_kvs + i];
	
		vdata[10] = vbuffer[10][bufferoffset_kvs + i];
	
		vdata[11] = vbuffer[11][bufferoffset_kvs + i];
	
		vdata[12] = vbuffer[12][bufferoffset_kvs + i];
	
		vdata[13] = vbuffer[13][bufferoffset_kvs + i];
	
		vdata[14] = vbuffer[14][bufferoffset_kvs + i];
	
		vdata[15] = vbuffer[15][bufferoffset_kvs + i];
		
		#ifdef BIT_TRAVERSAL_ALGORITHM
  datas[0] = vdata[0].data[0];  datas[0] = vdata[0].data[1];  datas[0] = vdata[0].data[2];  datas[0] = vdata[0].data[3];  datas[0] = vdata[0].data[4];  datas[0] = vdata[0].data[5];  datas[0] = vdata[0].data[6];  datas[0] = vdata[0].data[7];  datas[0] = vdata[0].data[8];  datas[0] = vdata[0].data[9];  datas[0] = vdata[0].data[10];  datas[0] = vdata[0].data[11];  datas[0] = vdata[0].data[12];  datas[0] = vdata[0].data[13];  datas[0] = vdata[0].data[14];  datas[0] = vdata[0].data[15];    datas[1] = vdata[1].data[0];  datas[1] = vdata[1].data[1];  datas[1] = vdata[1].data[2];  datas[1] = vdata[1].data[3];  datas[1] = vdata[1].data[4];  datas[1] = vdata[1].data[5];  datas[1] = vdata[1].data[6];  datas[1] = vdata[1].data[7];  datas[1] = vdata[1].data[8];  datas[1] = vdata[1].data[9];  datas[1] = vdata[1].data[10];  datas[1] = vdata[1].data[11];  datas[1] = vdata[1].data[12];  datas[1] = vdata[1].data[13];  datas[1] = vdata[1].data[14];  datas[1] = vdata[1].data[15];    datas[2] = vdata[2].data[0];  datas[2] = vdata[2].data[1];  datas[2] = vdata[2].data[2];  datas[2] = vdata[2].data[3];  datas[2] = vdata[2].data[4];  datas[2] = vdata[2].data[5];  datas[2] = vdata[2].data[6];  datas[2] = vdata[2].data[7];  datas[2] = vdata[2].data[8];  datas[2] = vdata[2].data[9];  datas[2] = vdata[2].data[10];  datas[2] = vdata[2].data[11];  datas[2] = vdata[2].data[12];  datas[2] = vdata[2].data[13];  datas[2] = vdata[2].data[14];  datas[2] = vdata[2].data[15];    datas[3] = vdata[3].data[0];  datas[3] = vdata[3].data[1];  datas[3] = vdata[3].data[2];  datas[3] = vdata[3].data[3];  datas[3] = vdata[3].data[4];  datas[3] = vdata[3].data[5];  datas[3] = vdata[3].data[6];  datas[3] = vdata[3].data[7];  datas[3] = vdata[3].data[8];  datas[3] = vdata[3].data[9];  datas[3] = vdata[3].data[10];  datas[3] = vdata[3].data[11];  datas[3] = vdata[3].data[12];  datas[3] = vdata[3].data[13];  datas[3] = vdata[3].data[14];  datas[3] = vdata[3].data[15];    datas[4] = vdata[4].data[0];  datas[4] = vdata[4].data[1];  datas[4] = vdata[4].data[2];  datas[4] = vdata[4].data[3];  datas[4] = vdata[4].data[4];  datas[4] = vdata[4].data[5];  datas[4] = vdata[4].data[6];  datas[4] = vdata[4].data[7];  datas[4] = vdata[4].data[8];  datas[4] = vdata[4].data[9];  datas[4] = vdata[4].data[10];  datas[4] = vdata[4].data[11];  datas[4] = vdata[4].data[12];  datas[4] = vdata[4].data[13];  datas[4] = vdata[4].data[14];  datas[4] = vdata[4].data[15];    datas[5] = vdata[5].data[0];  datas[5] = vdata[5].data[1];  datas[5] = vdata[5].data[2];  datas[5] = vdata[5].data[3];  datas[5] = vdata[5].data[4];  datas[5] = vdata[5].data[5];  datas[5] = vdata[5].data[6];  datas[5] = vdata[5].data[7];  datas[5] = vdata[5].data[8];  datas[5] = vdata[5].data[9];  datas[5] = vdata[5].data[10];  datas[5] = vdata[5].data[11];  datas[5] = vdata[5].data[12];  datas[5] = vdata[5].data[13];  datas[5] = vdata[5].data[14];  datas[5] = vdata[5].data[15];    datas[6] = vdata[6].data[0];  datas[6] = vdata[6].data[1];  datas[6] = vdata[6].data[2];  datas[6] = vdata[6].data[3];  datas[6] = vdata[6].data[4];  datas[6] = vdata[6].data[5];  datas[6] = vdata[6].data[6];  datas[6] = vdata[6].data[7];  datas[6] = vdata[6].data[8];  datas[6] = vdata[6].data[9];  datas[6] = vdata[6].data[10];  datas[6] = vdata[6].data[11];  datas[6] = vdata[6].data[12];  datas[6] = vdata[6].data[13];  datas[6] = vdata[6].data[14];  datas[6] = vdata[6].data[15];    datas[7] = vdata[7].data[0];  datas[7] = vdata[7].data[1];  datas[7] = vdata[7].data[2];  datas[7] = vdata[7].data[3];  datas[7] = vdata[7].data[4];  datas[7] = vdata[7].data[5];  datas[7] = vdata[7].data[6];  datas[7] = vdata[7].data[7];  datas[7] = vdata[7].data[8];  datas[7] = vdata[7].data[9];  datas[7] = vdata[7].data[10];  datas[7] = vdata[7].data[11];  datas[7] = vdata[7].data[12];  datas[7] = vdata[7].data[13];  datas[7] = vdata[7].data[14];  datas[7] = vdata[7].data[15];    datas[8] = vdata[8].data[0];  datas[8] = vdata[8].data[1];  datas[8] = vdata[8].data[2];  datas[8] = vdata[8].data[3];  datas[8] = vdata[8].data[4];  datas[8] = vdata[8].data[5];  datas[8] = vdata[8].data[6];  datas[8] = vdata[8].data[7];  datas[8] = vdata[8].data[8];  datas[8] = vdata[8].data[9];  datas[8] = vdata[8].data[10];  datas[8] = vdata[8].data[11];  datas[8] = vdata[8].data[12];  datas[8] = vdata[8].data[13];  datas[8] = vdata[8].data[14];  datas[8] = vdata[8].data[15];    datas[9] = vdata[9].data[0];  datas[9] = vdata[9].data[1];  datas[9] = vdata[9].data[2];  datas[9] = vdata[9].data[3];  datas[9] = vdata[9].data[4];  datas[9] = vdata[9].data[5];  datas[9] = vdata[9].data[6];  datas[9] = vdata[9].data[7];  datas[9] = vdata[9].data[8];  datas[9] = vdata[9].data[9];  datas[9] = vdata[9].data[10];  datas[9] = vdata[9].data[11];  datas[9] = vdata[9].data[12];  datas[9] = vdata[9].data[13];  datas[9] = vdata[9].data[14];  datas[9] = vdata[9].data[15];    datas[10] = vdata[10].data[0];  datas[10] = vdata[10].data[1];  datas[10] = vdata[10].data[2];  datas[10] = vdata[10].data[3];  datas[10] = vdata[10].data[4];  datas[10] = vdata[10].data[5];  datas[10] = vdata[10].data[6];  datas[10] = vdata[10].data[7];  datas[10] = vdata[10].data[8];  datas[10] = vdata[10].data[9];  datas[10] = vdata[10].data[10];  datas[10] = vdata[10].data[11];  datas[10] = vdata[10].data[12];  datas[10] = vdata[10].data[13];  datas[10] = vdata[10].data[14];  datas[10] = vdata[10].data[15];    datas[11] = vdata[11].data[0];  datas[11] = vdata[11].data[1];  datas[11] = vdata[11].data[2];  datas[11] = vdata[11].data[3];  datas[11] = vdata[11].data[4];  datas[11] = vdata[11].data[5];  datas[11] = vdata[11].data[6];  datas[11] = vdata[11].data[7];  datas[11] = vdata[11].data[8];  datas[11] = vdata[11].data[9];  datas[11] = vdata[11].data[10];  datas[11] = vdata[11].data[11];  datas[11] = vdata[11].data[12];  datas[11] = vdata[11].data[13];  datas[11] = vdata[11].data[14];  datas[11] = vdata[11].data[15];    datas[12] = vdata[12].data[0];  datas[12] = vdata[12].data[1];  datas[12] = vdata[12].data[2];  datas[12] = vdata[12].data[3];  datas[12] = vdata[12].data[4];  datas[12] = vdata[12].data[5];  datas[12] = vdata[12].data[6];  datas[12] = vdata[12].data[7];  datas[12] = vdata[12].data[8];  datas[12] = vdata[12].data[9];  datas[12] = vdata[12].data[10];  datas[12] = vdata[12].data[11];  datas[12] = vdata[12].data[12];  datas[12] = vdata[12].data[13];  datas[12] = vdata[12].data[14];  datas[12] = vdata[12].data[15];    datas[13] = vdata[13].data[0];  datas[13] = vdata[13].data[1];  datas[13] = vdata[13].data[2];  datas[13] = vdata[13].data[3];  datas[13] = vdata[13].data[4];  datas[13] = vdata[13].data[5];  datas[13] = vdata[13].data[6];  datas[13] = vdata[13].data[7];  datas[13] = vdata[13].data[8];  datas[13] = vdata[13].data[9];  datas[13] = vdata[13].data[10];  datas[13] = vdata[13].data[11];  datas[13] = vdata[13].data[12];  datas[13] = vdata[13].data[13];  datas[13] = vdata[13].data[14];  datas[13] = vdata[13].data[15];    datas[14] = vdata[14].data[0];  datas[14] = vdata[14].data[1];  datas[14] = vdata[14].data[2];  datas[14] = vdata[14].data[3];  datas[14] = vdata[14].data[4];  datas[14] = vdata[14].data[5];  datas[14] = vdata[14].data[6];  datas[14] = vdata[14].data[7];  datas[14] = vdata[14].data[8];  datas[14] = vdata[14].data[9];  datas[14] = vdata[14].data[10];  datas[14] = vdata[14].data[11];  datas[14] = vdata[14].data[12];  datas[14] = vdata[14].data[13];  datas[14] = vdata[14].data[14];  datas[14] = vdata[14].data[15];    datas[15] = vdata[15].data[0];  datas[15] = vdata[15].data[1];  datas[15] = vdata[15].data[2];  datas[15] = vdata[15].data[3];  datas[15] = vdata[15].data[4];  datas[15] = vdata[15].data[5];  datas[15] = vdata[15].data[6];  datas[15] = vdata[15].data[7];  datas[15] = vdata[15].data[8];  datas[15] = vdata[15].data[9];  datas[15] = vdata[15].data[10];  datas[15] = vdata[15].data[11];  datas[15] = vdata[15].data[12];  datas[15] = vdata[15].data[13];  datas[15] = vdata[15].data[14];  datas[15] = vdata[15].data[15];   // FIXME.
		#else 
 datas[0] = vdata[0].data;  datas[1] = vdata[1].data;  datas[2] = vdata[2].data;  datas[3] = vdata[3].data;  datas[4] = vdata[4].data;  datas[5] = vdata[5].data;  datas[6] = vdata[6].data;  datas[7] = vdata[7].data;  datas[8] = vdata[8].data;  datas[9] = vdata[9].data;  datas[10] = vdata[10].data;  datas[11] = vdata[11].data;  datas[12] = vdata[12].data;  datas[13] = vdata[13].data;  datas[14] = vdata[14].data;  datas[15] = vdata[15].data; 	
		#endif
		
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
		for(unsigned int v=0; v<VECTOR2_SIZE; v++){ 
			value_t vdata = datas[v] >> 1; value_t mask = datas[v] & 0x1;
			if(vdata < 1000){ cout<<"saveV: ACTIVE VDATA SEEN: @ i: "<<i<<" v: "<<v<<", vdata: "<<vdata<<", mask: "<<mask<<endl; }}
		#endif
		
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MEMACCESSP0_saveV 23", baseoffset_kvs + offset_kvs + i, globalparams.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
		#endif
		UTILP0_WriteDatas(kvdram, baseoffset_kvs + offset_kvs + i, datas);
	}
	return;
}

void acts_all::MEMACCESSP0_readANDRVchunks1(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	// return;//
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int limit = globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE;
	
	READANDRVCHUNKS_LOOP2: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): vsz_kvs: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		unsigned int index = 0;
		// unsigned int index = s;
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			// unsigned int offset_kvs; if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			unsigned int offset_kvs = voffset_kvs + depth_i + i;
			UTILP0_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, datas);	
			
			#ifdef BIT_TRAVERSAL_ALGORITHM
  vdata[0].data[0] = datas[0];  vdata[0].data[1] = datas[0];  vdata[0].data[2] = datas[0];  vdata[0].data[3] = datas[0];  vdata[0].data[4] = datas[0];  vdata[0].data[5] = datas[0];  vdata[0].data[6] = datas[0];  vdata[0].data[7] = datas[0];  vdata[0].data[8] = datas[0];  vdata[0].data[9] = datas[0];  vdata[0].data[10] = datas[0];  vdata[0].data[11] = datas[0];  vdata[0].data[12] = datas[0];  vdata[0].data[13] = datas[0];  vdata[0].data[14] = datas[0];  vdata[0].data[15] = datas[0];    vdata[1].data[0] = datas[1];  vdata[1].data[1] = datas[1];  vdata[1].data[2] = datas[1];  vdata[1].data[3] = datas[1];  vdata[1].data[4] = datas[1];  vdata[1].data[5] = datas[1];  vdata[1].data[6] = datas[1];  vdata[1].data[7] = datas[1];  vdata[1].data[8] = datas[1];  vdata[1].data[9] = datas[1];  vdata[1].data[10] = datas[1];  vdata[1].data[11] = datas[1];  vdata[1].data[12] = datas[1];  vdata[1].data[13] = datas[1];  vdata[1].data[14] = datas[1];  vdata[1].data[15] = datas[1];    vdata[2].data[0] = datas[2];  vdata[2].data[1] = datas[2];  vdata[2].data[2] = datas[2];  vdata[2].data[3] = datas[2];  vdata[2].data[4] = datas[2];  vdata[2].data[5] = datas[2];  vdata[2].data[6] = datas[2];  vdata[2].data[7] = datas[2];  vdata[2].data[8] = datas[2];  vdata[2].data[9] = datas[2];  vdata[2].data[10] = datas[2];  vdata[2].data[11] = datas[2];  vdata[2].data[12] = datas[2];  vdata[2].data[13] = datas[2];  vdata[2].data[14] = datas[2];  vdata[2].data[15] = datas[2];    vdata[3].data[0] = datas[3];  vdata[3].data[1] = datas[3];  vdata[3].data[2] = datas[3];  vdata[3].data[3] = datas[3];  vdata[3].data[4] = datas[3];  vdata[3].data[5] = datas[3];  vdata[3].data[6] = datas[3];  vdata[3].data[7] = datas[3];  vdata[3].data[8] = datas[3];  vdata[3].data[9] = datas[3];  vdata[3].data[10] = datas[3];  vdata[3].data[11] = datas[3];  vdata[3].data[12] = datas[3];  vdata[3].data[13] = datas[3];  vdata[3].data[14] = datas[3];  vdata[3].data[15] = datas[3];    vdata[4].data[0] = datas[4];  vdata[4].data[1] = datas[4];  vdata[4].data[2] = datas[4];  vdata[4].data[3] = datas[4];  vdata[4].data[4] = datas[4];  vdata[4].data[5] = datas[4];  vdata[4].data[6] = datas[4];  vdata[4].data[7] = datas[4];  vdata[4].data[8] = datas[4];  vdata[4].data[9] = datas[4];  vdata[4].data[10] = datas[4];  vdata[4].data[11] = datas[4];  vdata[4].data[12] = datas[4];  vdata[4].data[13] = datas[4];  vdata[4].data[14] = datas[4];  vdata[4].data[15] = datas[4];    vdata[5].data[0] = datas[5];  vdata[5].data[1] = datas[5];  vdata[5].data[2] = datas[5];  vdata[5].data[3] = datas[5];  vdata[5].data[4] = datas[5];  vdata[5].data[5] = datas[5];  vdata[5].data[6] = datas[5];  vdata[5].data[7] = datas[5];  vdata[5].data[8] = datas[5];  vdata[5].data[9] = datas[5];  vdata[5].data[10] = datas[5];  vdata[5].data[11] = datas[5];  vdata[5].data[12] = datas[5];  vdata[5].data[13] = datas[5];  vdata[5].data[14] = datas[5];  vdata[5].data[15] = datas[5];    vdata[6].data[0] = datas[6];  vdata[6].data[1] = datas[6];  vdata[6].data[2] = datas[6];  vdata[6].data[3] = datas[6];  vdata[6].data[4] = datas[6];  vdata[6].data[5] = datas[6];  vdata[6].data[6] = datas[6];  vdata[6].data[7] = datas[6];  vdata[6].data[8] = datas[6];  vdata[6].data[9] = datas[6];  vdata[6].data[10] = datas[6];  vdata[6].data[11] = datas[6];  vdata[6].data[12] = datas[6];  vdata[6].data[13] = datas[6];  vdata[6].data[14] = datas[6];  vdata[6].data[15] = datas[6];    vdata[7].data[0] = datas[7];  vdata[7].data[1] = datas[7];  vdata[7].data[2] = datas[7];  vdata[7].data[3] = datas[7];  vdata[7].data[4] = datas[7];  vdata[7].data[5] = datas[7];  vdata[7].data[6] = datas[7];  vdata[7].data[7] = datas[7];  vdata[7].data[8] = datas[7];  vdata[7].data[9] = datas[7];  vdata[7].data[10] = datas[7];  vdata[7].data[11] = datas[7];  vdata[7].data[12] = datas[7];  vdata[7].data[13] = datas[7];  vdata[7].data[14] = datas[7];  vdata[7].data[15] = datas[7];    vdata[8].data[0] = datas[8];  vdata[8].data[1] = datas[8];  vdata[8].data[2] = datas[8];  vdata[8].data[3] = datas[8];  vdata[8].data[4] = datas[8];  vdata[8].data[5] = datas[8];  vdata[8].data[6] = datas[8];  vdata[8].data[7] = datas[8];  vdata[8].data[8] = datas[8];  vdata[8].data[9] = datas[8];  vdata[8].data[10] = datas[8];  vdata[8].data[11] = datas[8];  vdata[8].data[12] = datas[8];  vdata[8].data[13] = datas[8];  vdata[8].data[14] = datas[8];  vdata[8].data[15] = datas[8];    vdata[9].data[0] = datas[9];  vdata[9].data[1] = datas[9];  vdata[9].data[2] = datas[9];  vdata[9].data[3] = datas[9];  vdata[9].data[4] = datas[9];  vdata[9].data[5] = datas[9];  vdata[9].data[6] = datas[9];  vdata[9].data[7] = datas[9];  vdata[9].data[8] = datas[9];  vdata[9].data[9] = datas[9];  vdata[9].data[10] = datas[9];  vdata[9].data[11] = datas[9];  vdata[9].data[12] = datas[9];  vdata[9].data[13] = datas[9];  vdata[9].data[14] = datas[9];  vdata[9].data[15] = datas[9];    vdata[10].data[0] = datas[10];  vdata[10].data[1] = datas[10];  vdata[10].data[2] = datas[10];  vdata[10].data[3] = datas[10];  vdata[10].data[4] = datas[10];  vdata[10].data[5] = datas[10];  vdata[10].data[6] = datas[10];  vdata[10].data[7] = datas[10];  vdata[10].data[8] = datas[10];  vdata[10].data[9] = datas[10];  vdata[10].data[10] = datas[10];  vdata[10].data[11] = datas[10];  vdata[10].data[12] = datas[10];  vdata[10].data[13] = datas[10];  vdata[10].data[14] = datas[10];  vdata[10].data[15] = datas[10];    vdata[11].data[0] = datas[11];  vdata[11].data[1] = datas[11];  vdata[11].data[2] = datas[11];  vdata[11].data[3] = datas[11];  vdata[11].data[4] = datas[11];  vdata[11].data[5] = datas[11];  vdata[11].data[6] = datas[11];  vdata[11].data[7] = datas[11];  vdata[11].data[8] = datas[11];  vdata[11].data[9] = datas[11];  vdata[11].data[10] = datas[11];  vdata[11].data[11] = datas[11];  vdata[11].data[12] = datas[11];  vdata[11].data[13] = datas[11];  vdata[11].data[14] = datas[11];  vdata[11].data[15] = datas[11];    vdata[12].data[0] = datas[12];  vdata[12].data[1] = datas[12];  vdata[12].data[2] = datas[12];  vdata[12].data[3] = datas[12];  vdata[12].data[4] = datas[12];  vdata[12].data[5] = datas[12];  vdata[12].data[6] = datas[12];  vdata[12].data[7] = datas[12];  vdata[12].data[8] = datas[12];  vdata[12].data[9] = datas[12];  vdata[12].data[10] = datas[12];  vdata[12].data[11] = datas[12];  vdata[12].data[12] = datas[12];  vdata[12].data[13] = datas[12];  vdata[12].data[14] = datas[12];  vdata[12].data[15] = datas[12];    vdata[13].data[0] = datas[13];  vdata[13].data[1] = datas[13];  vdata[13].data[2] = datas[13];  vdata[13].data[3] = datas[13];  vdata[13].data[4] = datas[13];  vdata[13].data[5] = datas[13];  vdata[13].data[6] = datas[13];  vdata[13].data[7] = datas[13];  vdata[13].data[8] = datas[13];  vdata[13].data[9] = datas[13];  vdata[13].data[10] = datas[13];  vdata[13].data[11] = datas[13];  vdata[13].data[12] = datas[13];  vdata[13].data[13] = datas[13];  vdata[13].data[14] = datas[13];  vdata[13].data[15] = datas[13];    vdata[14].data[0] = datas[14];  vdata[14].data[1] = datas[14];  vdata[14].data[2] = datas[14];  vdata[14].data[3] = datas[14];  vdata[14].data[4] = datas[14];  vdata[14].data[5] = datas[14];  vdata[14].data[6] = datas[14];  vdata[14].data[7] = datas[14];  vdata[14].data[8] = datas[14];  vdata[14].data[9] = datas[14];  vdata[14].data[10] = datas[14];  vdata[14].data[11] = datas[14];  vdata[14].data[12] = datas[14];  vdata[14].data[13] = datas[14];  vdata[14].data[14] = datas[14];  vdata[14].data[15] = datas[14];    vdata[15].data[0] = datas[15];  vdata[15].data[1] = datas[15];  vdata[15].data[2] = datas[15];  vdata[15].data[3] = datas[15];  vdata[15].data[4] = datas[15];  vdata[15].data[5] = datas[15];  vdata[15].data[6] = datas[15];  vdata[15].data[7] = datas[15];  vdata[15].data[8] = datas[15];  vdata[15].data[9] = datas[15];  vdata[15].data[10] = datas[15];  vdata[15].data[11] = datas[15];  vdata[15].data[12] = datas[15];  vdata[15].data[13] = datas[15];  vdata[15].data[14] = datas[15];  vdata[15].data[15] = datas[15];   // FIXME.
			#else 
 vdata[0].data = datas[0];  vdata[1].data = datas[1];  vdata[2].data = datas[2];  vdata[3].data = datas[3];  vdata[4].data = datas[4];  vdata[5].data = datas[5];  vdata[6].data = datas[6];  vdata[7].data = datas[7];  vdata[8].data = datas[8];  vdata[9].data = datas[9];  vdata[10].data = datas[10];  vdata[11].data = datas[11];  vdata[12].data = datas[12];  vdata[13].data = datas[13];  vdata[14].data = datas[14];  vdata[15].data = datas[15]; 	
			#endif
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(datas[v] == 1 || datas[v] < 1000){ cout<<"readANDRVchunks: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<" v: "<<v<<", voffset_kvs: "<<voffset_kvs<<", index + s: "<<index + s<<", datas["<<v<<"]: "<<datas[v]<<endl; }}
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP0_readANDRVchunks1 25", index + s, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, index, i);
			#endif
			vbuffer0[0][index + s] = vdata[0];
			vbuffer0[1][index + s] = vdata[1];
			vbuffer0[2][index + s] = vdata[2];
			vbuffer0[3][index + s] = vdata[3];
			vbuffer0[4][index + s] = vdata[4];
			vbuffer0[5][index + s] = vdata[5];
			vbuffer0[6][index + s] = vdata[6];
			vbuffer0[7][index + s] = vdata[7];
			vbuffer0[8][index + s] = vdata[8];
			vbuffer0[9][index + s] = vdata[9];
			vbuffer0[10][index + s] = vdata[10];
			vbuffer0[11][index + s] = vdata[11];
			vbuffer0[12][index + s] = vdata[12];
			vbuffer0[13][index + s] = vdata[13];
			vbuffer0[14][index + s] = vdata[14];
			vbuffer0[15][index + s] = vdata[15];
			index += NUM_PEs;

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
	}
	// exit(EXIT_SUCCESS); /////////
	return;
}
void acts_all::MEMACCESSP0_readANDRVchunks2(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	// return;//
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int limit = globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE;
	
	READANDRVCHUNKS_LOOP2: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): vsz_kvs: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		unsigned int index = 0;
		// unsigned int index = s;
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			// unsigned int offset_kvs; if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			unsigned int offset_kvs = voffset_kvs + depth_i + i;
			UTILP0_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, datas);	
			
			#ifdef BIT_TRAVERSAL_ALGORITHM
  vdata[0].data[0] = datas[0];  vdata[0].data[1] = datas[0];  vdata[0].data[2] = datas[0];  vdata[0].data[3] = datas[0];  vdata[0].data[4] = datas[0];  vdata[0].data[5] = datas[0];  vdata[0].data[6] = datas[0];  vdata[0].data[7] = datas[0];  vdata[0].data[8] = datas[0];  vdata[0].data[9] = datas[0];  vdata[0].data[10] = datas[0];  vdata[0].data[11] = datas[0];  vdata[0].data[12] = datas[0];  vdata[0].data[13] = datas[0];  vdata[0].data[14] = datas[0];  vdata[0].data[15] = datas[0];    vdata[1].data[0] = datas[1];  vdata[1].data[1] = datas[1];  vdata[1].data[2] = datas[1];  vdata[1].data[3] = datas[1];  vdata[1].data[4] = datas[1];  vdata[1].data[5] = datas[1];  vdata[1].data[6] = datas[1];  vdata[1].data[7] = datas[1];  vdata[1].data[8] = datas[1];  vdata[1].data[9] = datas[1];  vdata[1].data[10] = datas[1];  vdata[1].data[11] = datas[1];  vdata[1].data[12] = datas[1];  vdata[1].data[13] = datas[1];  vdata[1].data[14] = datas[1];  vdata[1].data[15] = datas[1];    vdata[2].data[0] = datas[2];  vdata[2].data[1] = datas[2];  vdata[2].data[2] = datas[2];  vdata[2].data[3] = datas[2];  vdata[2].data[4] = datas[2];  vdata[2].data[5] = datas[2];  vdata[2].data[6] = datas[2];  vdata[2].data[7] = datas[2];  vdata[2].data[8] = datas[2];  vdata[2].data[9] = datas[2];  vdata[2].data[10] = datas[2];  vdata[2].data[11] = datas[2];  vdata[2].data[12] = datas[2];  vdata[2].data[13] = datas[2];  vdata[2].data[14] = datas[2];  vdata[2].data[15] = datas[2];    vdata[3].data[0] = datas[3];  vdata[3].data[1] = datas[3];  vdata[3].data[2] = datas[3];  vdata[3].data[3] = datas[3];  vdata[3].data[4] = datas[3];  vdata[3].data[5] = datas[3];  vdata[3].data[6] = datas[3];  vdata[3].data[7] = datas[3];  vdata[3].data[8] = datas[3];  vdata[3].data[9] = datas[3];  vdata[3].data[10] = datas[3];  vdata[3].data[11] = datas[3];  vdata[3].data[12] = datas[3];  vdata[3].data[13] = datas[3];  vdata[3].data[14] = datas[3];  vdata[3].data[15] = datas[3];    vdata[4].data[0] = datas[4];  vdata[4].data[1] = datas[4];  vdata[4].data[2] = datas[4];  vdata[4].data[3] = datas[4];  vdata[4].data[4] = datas[4];  vdata[4].data[5] = datas[4];  vdata[4].data[6] = datas[4];  vdata[4].data[7] = datas[4];  vdata[4].data[8] = datas[4];  vdata[4].data[9] = datas[4];  vdata[4].data[10] = datas[4];  vdata[4].data[11] = datas[4];  vdata[4].data[12] = datas[4];  vdata[4].data[13] = datas[4];  vdata[4].data[14] = datas[4];  vdata[4].data[15] = datas[4];    vdata[5].data[0] = datas[5];  vdata[5].data[1] = datas[5];  vdata[5].data[2] = datas[5];  vdata[5].data[3] = datas[5];  vdata[5].data[4] = datas[5];  vdata[5].data[5] = datas[5];  vdata[5].data[6] = datas[5];  vdata[5].data[7] = datas[5];  vdata[5].data[8] = datas[5];  vdata[5].data[9] = datas[5];  vdata[5].data[10] = datas[5];  vdata[5].data[11] = datas[5];  vdata[5].data[12] = datas[5];  vdata[5].data[13] = datas[5];  vdata[5].data[14] = datas[5];  vdata[5].data[15] = datas[5];    vdata[6].data[0] = datas[6];  vdata[6].data[1] = datas[6];  vdata[6].data[2] = datas[6];  vdata[6].data[3] = datas[6];  vdata[6].data[4] = datas[6];  vdata[6].data[5] = datas[6];  vdata[6].data[6] = datas[6];  vdata[6].data[7] = datas[6];  vdata[6].data[8] = datas[6];  vdata[6].data[9] = datas[6];  vdata[6].data[10] = datas[6];  vdata[6].data[11] = datas[6];  vdata[6].data[12] = datas[6];  vdata[6].data[13] = datas[6];  vdata[6].data[14] = datas[6];  vdata[6].data[15] = datas[6];    vdata[7].data[0] = datas[7];  vdata[7].data[1] = datas[7];  vdata[7].data[2] = datas[7];  vdata[7].data[3] = datas[7];  vdata[7].data[4] = datas[7];  vdata[7].data[5] = datas[7];  vdata[7].data[6] = datas[7];  vdata[7].data[7] = datas[7];  vdata[7].data[8] = datas[7];  vdata[7].data[9] = datas[7];  vdata[7].data[10] = datas[7];  vdata[7].data[11] = datas[7];  vdata[7].data[12] = datas[7];  vdata[7].data[13] = datas[7];  vdata[7].data[14] = datas[7];  vdata[7].data[15] = datas[7];    vdata[8].data[0] = datas[8];  vdata[8].data[1] = datas[8];  vdata[8].data[2] = datas[8];  vdata[8].data[3] = datas[8];  vdata[8].data[4] = datas[8];  vdata[8].data[5] = datas[8];  vdata[8].data[6] = datas[8];  vdata[8].data[7] = datas[8];  vdata[8].data[8] = datas[8];  vdata[8].data[9] = datas[8];  vdata[8].data[10] = datas[8];  vdata[8].data[11] = datas[8];  vdata[8].data[12] = datas[8];  vdata[8].data[13] = datas[8];  vdata[8].data[14] = datas[8];  vdata[8].data[15] = datas[8];    vdata[9].data[0] = datas[9];  vdata[9].data[1] = datas[9];  vdata[9].data[2] = datas[9];  vdata[9].data[3] = datas[9];  vdata[9].data[4] = datas[9];  vdata[9].data[5] = datas[9];  vdata[9].data[6] = datas[9];  vdata[9].data[7] = datas[9];  vdata[9].data[8] = datas[9];  vdata[9].data[9] = datas[9];  vdata[9].data[10] = datas[9];  vdata[9].data[11] = datas[9];  vdata[9].data[12] = datas[9];  vdata[9].data[13] = datas[9];  vdata[9].data[14] = datas[9];  vdata[9].data[15] = datas[9];    vdata[10].data[0] = datas[10];  vdata[10].data[1] = datas[10];  vdata[10].data[2] = datas[10];  vdata[10].data[3] = datas[10];  vdata[10].data[4] = datas[10];  vdata[10].data[5] = datas[10];  vdata[10].data[6] = datas[10];  vdata[10].data[7] = datas[10];  vdata[10].data[8] = datas[10];  vdata[10].data[9] = datas[10];  vdata[10].data[10] = datas[10];  vdata[10].data[11] = datas[10];  vdata[10].data[12] = datas[10];  vdata[10].data[13] = datas[10];  vdata[10].data[14] = datas[10];  vdata[10].data[15] = datas[10];    vdata[11].data[0] = datas[11];  vdata[11].data[1] = datas[11];  vdata[11].data[2] = datas[11];  vdata[11].data[3] = datas[11];  vdata[11].data[4] = datas[11];  vdata[11].data[5] = datas[11];  vdata[11].data[6] = datas[11];  vdata[11].data[7] = datas[11];  vdata[11].data[8] = datas[11];  vdata[11].data[9] = datas[11];  vdata[11].data[10] = datas[11];  vdata[11].data[11] = datas[11];  vdata[11].data[12] = datas[11];  vdata[11].data[13] = datas[11];  vdata[11].data[14] = datas[11];  vdata[11].data[15] = datas[11];    vdata[12].data[0] = datas[12];  vdata[12].data[1] = datas[12];  vdata[12].data[2] = datas[12];  vdata[12].data[3] = datas[12];  vdata[12].data[4] = datas[12];  vdata[12].data[5] = datas[12];  vdata[12].data[6] = datas[12];  vdata[12].data[7] = datas[12];  vdata[12].data[8] = datas[12];  vdata[12].data[9] = datas[12];  vdata[12].data[10] = datas[12];  vdata[12].data[11] = datas[12];  vdata[12].data[12] = datas[12];  vdata[12].data[13] = datas[12];  vdata[12].data[14] = datas[12];  vdata[12].data[15] = datas[12];    vdata[13].data[0] = datas[13];  vdata[13].data[1] = datas[13];  vdata[13].data[2] = datas[13];  vdata[13].data[3] = datas[13];  vdata[13].data[4] = datas[13];  vdata[13].data[5] = datas[13];  vdata[13].data[6] = datas[13];  vdata[13].data[7] = datas[13];  vdata[13].data[8] = datas[13];  vdata[13].data[9] = datas[13];  vdata[13].data[10] = datas[13];  vdata[13].data[11] = datas[13];  vdata[13].data[12] = datas[13];  vdata[13].data[13] = datas[13];  vdata[13].data[14] = datas[13];  vdata[13].data[15] = datas[13];    vdata[14].data[0] = datas[14];  vdata[14].data[1] = datas[14];  vdata[14].data[2] = datas[14];  vdata[14].data[3] = datas[14];  vdata[14].data[4] = datas[14];  vdata[14].data[5] = datas[14];  vdata[14].data[6] = datas[14];  vdata[14].data[7] = datas[14];  vdata[14].data[8] = datas[14];  vdata[14].data[9] = datas[14];  vdata[14].data[10] = datas[14];  vdata[14].data[11] = datas[14];  vdata[14].data[12] = datas[14];  vdata[14].data[13] = datas[14];  vdata[14].data[14] = datas[14];  vdata[14].data[15] = datas[14];    vdata[15].data[0] = datas[15];  vdata[15].data[1] = datas[15];  vdata[15].data[2] = datas[15];  vdata[15].data[3] = datas[15];  vdata[15].data[4] = datas[15];  vdata[15].data[5] = datas[15];  vdata[15].data[6] = datas[15];  vdata[15].data[7] = datas[15];  vdata[15].data[8] = datas[15];  vdata[15].data[9] = datas[15];  vdata[15].data[10] = datas[15];  vdata[15].data[11] = datas[15];  vdata[15].data[12] = datas[15];  vdata[15].data[13] = datas[15];  vdata[15].data[14] = datas[15];  vdata[15].data[15] = datas[15];   // FIXME.
			#else 
 vdata[0].data = datas[0];  vdata[1].data = datas[1];  vdata[2].data = datas[2];  vdata[3].data = datas[3];  vdata[4].data = datas[4];  vdata[5].data = datas[5];  vdata[6].data = datas[6];  vdata[7].data = datas[7];  vdata[8].data = datas[8];  vdata[9].data = datas[9];  vdata[10].data = datas[10];  vdata[11].data = datas[11];  vdata[12].data = datas[12];  vdata[13].data = datas[13];  vdata[14].data = datas[14];  vdata[15].data = datas[15]; 	
			#endif
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(datas[v] == 1 || datas[v] < 1000){ cout<<"readANDRVchunks: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<" v: "<<v<<", voffset_kvs: "<<voffset_kvs<<", index + s: "<<index + s<<", datas["<<v<<"]: "<<datas[v]<<endl; }}
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP0_readANDRVchunks2 25", index + s, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, index, i);
			#endif
			vbuffer0[0][index + s] = vdata[0];
			vbuffer0[1][index + s] = vdata[1];
			vbuffer0[2][index + s] = vdata[2];
			vbuffer0[3][index + s] = vdata[3];
			vbuffer0[4][index + s] = vdata[4];
			vbuffer0[5][index + s] = vdata[5];
			vbuffer0[6][index + s] = vdata[6];
			vbuffer0[7][index + s] = vdata[7];
			vbuffer0[8][index + s] = vdata[8];
			vbuffer0[9][index + s] = vdata[9];
			vbuffer0[10][index + s] = vdata[10];
			vbuffer0[11][index + s] = vdata[11];
			vbuffer0[12][index + s] = vdata[12];
			vbuffer0[13][index + s] = vdata[13];
			vbuffer0[14][index + s] = vdata[14];
			vbuffer0[15][index + s] = vdata[15];
			vbuffer1[0][index + s] = vdata[0];
			vbuffer1[1][index + s] = vdata[1];
			vbuffer1[2][index + s] = vdata[2];
			vbuffer1[3][index + s] = vdata[3];
			vbuffer1[4][index + s] = vdata[4];
			vbuffer1[5][index + s] = vdata[5];
			vbuffer1[6][index + s] = vdata[6];
			vbuffer1[7][index + s] = vdata[7];
			vbuffer1[8][index + s] = vdata[8];
			vbuffer1[9][index + s] = vdata[9];
			vbuffer1[10][index + s] = vdata[10];
			vbuffer1[11][index + s] = vdata[11];
			vbuffer1[12][index + s] = vdata[12];
			vbuffer1[13][index + s] = vdata[13];
			vbuffer1[14][index + s] = vdata[14];
			vbuffer1[15][index + s] = vdata[15];
			index += NUM_PEs;

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
	}
	// exit(EXIT_SUCCESS); /////////
	return;
}
void acts_all::MEMACCESSP0_readANDRVchunks3(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	// return;//
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int limit = globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE;
	
	READANDRVCHUNKS_LOOP2: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): vsz_kvs: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		unsigned int index = 0;
		// unsigned int index = s;
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			// unsigned int offset_kvs; if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			unsigned int offset_kvs = voffset_kvs + depth_i + i;
			UTILP0_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, datas);	
			
			#ifdef BIT_TRAVERSAL_ALGORITHM
  vdata[0].data[0] = datas[0];  vdata[0].data[1] = datas[0];  vdata[0].data[2] = datas[0];  vdata[0].data[3] = datas[0];  vdata[0].data[4] = datas[0];  vdata[0].data[5] = datas[0];  vdata[0].data[6] = datas[0];  vdata[0].data[7] = datas[0];  vdata[0].data[8] = datas[0];  vdata[0].data[9] = datas[0];  vdata[0].data[10] = datas[0];  vdata[0].data[11] = datas[0];  vdata[0].data[12] = datas[0];  vdata[0].data[13] = datas[0];  vdata[0].data[14] = datas[0];  vdata[0].data[15] = datas[0];    vdata[1].data[0] = datas[1];  vdata[1].data[1] = datas[1];  vdata[1].data[2] = datas[1];  vdata[1].data[3] = datas[1];  vdata[1].data[4] = datas[1];  vdata[1].data[5] = datas[1];  vdata[1].data[6] = datas[1];  vdata[1].data[7] = datas[1];  vdata[1].data[8] = datas[1];  vdata[1].data[9] = datas[1];  vdata[1].data[10] = datas[1];  vdata[1].data[11] = datas[1];  vdata[1].data[12] = datas[1];  vdata[1].data[13] = datas[1];  vdata[1].data[14] = datas[1];  vdata[1].data[15] = datas[1];    vdata[2].data[0] = datas[2];  vdata[2].data[1] = datas[2];  vdata[2].data[2] = datas[2];  vdata[2].data[3] = datas[2];  vdata[2].data[4] = datas[2];  vdata[2].data[5] = datas[2];  vdata[2].data[6] = datas[2];  vdata[2].data[7] = datas[2];  vdata[2].data[8] = datas[2];  vdata[2].data[9] = datas[2];  vdata[2].data[10] = datas[2];  vdata[2].data[11] = datas[2];  vdata[2].data[12] = datas[2];  vdata[2].data[13] = datas[2];  vdata[2].data[14] = datas[2];  vdata[2].data[15] = datas[2];    vdata[3].data[0] = datas[3];  vdata[3].data[1] = datas[3];  vdata[3].data[2] = datas[3];  vdata[3].data[3] = datas[3];  vdata[3].data[4] = datas[3];  vdata[3].data[5] = datas[3];  vdata[3].data[6] = datas[3];  vdata[3].data[7] = datas[3];  vdata[3].data[8] = datas[3];  vdata[3].data[9] = datas[3];  vdata[3].data[10] = datas[3];  vdata[3].data[11] = datas[3];  vdata[3].data[12] = datas[3];  vdata[3].data[13] = datas[3];  vdata[3].data[14] = datas[3];  vdata[3].data[15] = datas[3];    vdata[4].data[0] = datas[4];  vdata[4].data[1] = datas[4];  vdata[4].data[2] = datas[4];  vdata[4].data[3] = datas[4];  vdata[4].data[4] = datas[4];  vdata[4].data[5] = datas[4];  vdata[4].data[6] = datas[4];  vdata[4].data[7] = datas[4];  vdata[4].data[8] = datas[4];  vdata[4].data[9] = datas[4];  vdata[4].data[10] = datas[4];  vdata[4].data[11] = datas[4];  vdata[4].data[12] = datas[4];  vdata[4].data[13] = datas[4];  vdata[4].data[14] = datas[4];  vdata[4].data[15] = datas[4];    vdata[5].data[0] = datas[5];  vdata[5].data[1] = datas[5];  vdata[5].data[2] = datas[5];  vdata[5].data[3] = datas[5];  vdata[5].data[4] = datas[5];  vdata[5].data[5] = datas[5];  vdata[5].data[6] = datas[5];  vdata[5].data[7] = datas[5];  vdata[5].data[8] = datas[5];  vdata[5].data[9] = datas[5];  vdata[5].data[10] = datas[5];  vdata[5].data[11] = datas[5];  vdata[5].data[12] = datas[5];  vdata[5].data[13] = datas[5];  vdata[5].data[14] = datas[5];  vdata[5].data[15] = datas[5];    vdata[6].data[0] = datas[6];  vdata[6].data[1] = datas[6];  vdata[6].data[2] = datas[6];  vdata[6].data[3] = datas[6];  vdata[6].data[4] = datas[6];  vdata[6].data[5] = datas[6];  vdata[6].data[6] = datas[6];  vdata[6].data[7] = datas[6];  vdata[6].data[8] = datas[6];  vdata[6].data[9] = datas[6];  vdata[6].data[10] = datas[6];  vdata[6].data[11] = datas[6];  vdata[6].data[12] = datas[6];  vdata[6].data[13] = datas[6];  vdata[6].data[14] = datas[6];  vdata[6].data[15] = datas[6];    vdata[7].data[0] = datas[7];  vdata[7].data[1] = datas[7];  vdata[7].data[2] = datas[7];  vdata[7].data[3] = datas[7];  vdata[7].data[4] = datas[7];  vdata[7].data[5] = datas[7];  vdata[7].data[6] = datas[7];  vdata[7].data[7] = datas[7];  vdata[7].data[8] = datas[7];  vdata[7].data[9] = datas[7];  vdata[7].data[10] = datas[7];  vdata[7].data[11] = datas[7];  vdata[7].data[12] = datas[7];  vdata[7].data[13] = datas[7];  vdata[7].data[14] = datas[7];  vdata[7].data[15] = datas[7];    vdata[8].data[0] = datas[8];  vdata[8].data[1] = datas[8];  vdata[8].data[2] = datas[8];  vdata[8].data[3] = datas[8];  vdata[8].data[4] = datas[8];  vdata[8].data[5] = datas[8];  vdata[8].data[6] = datas[8];  vdata[8].data[7] = datas[8];  vdata[8].data[8] = datas[8];  vdata[8].data[9] = datas[8];  vdata[8].data[10] = datas[8];  vdata[8].data[11] = datas[8];  vdata[8].data[12] = datas[8];  vdata[8].data[13] = datas[8];  vdata[8].data[14] = datas[8];  vdata[8].data[15] = datas[8];    vdata[9].data[0] = datas[9];  vdata[9].data[1] = datas[9];  vdata[9].data[2] = datas[9];  vdata[9].data[3] = datas[9];  vdata[9].data[4] = datas[9];  vdata[9].data[5] = datas[9];  vdata[9].data[6] = datas[9];  vdata[9].data[7] = datas[9];  vdata[9].data[8] = datas[9];  vdata[9].data[9] = datas[9];  vdata[9].data[10] = datas[9];  vdata[9].data[11] = datas[9];  vdata[9].data[12] = datas[9];  vdata[9].data[13] = datas[9];  vdata[9].data[14] = datas[9];  vdata[9].data[15] = datas[9];    vdata[10].data[0] = datas[10];  vdata[10].data[1] = datas[10];  vdata[10].data[2] = datas[10];  vdata[10].data[3] = datas[10];  vdata[10].data[4] = datas[10];  vdata[10].data[5] = datas[10];  vdata[10].data[6] = datas[10];  vdata[10].data[7] = datas[10];  vdata[10].data[8] = datas[10];  vdata[10].data[9] = datas[10];  vdata[10].data[10] = datas[10];  vdata[10].data[11] = datas[10];  vdata[10].data[12] = datas[10];  vdata[10].data[13] = datas[10];  vdata[10].data[14] = datas[10];  vdata[10].data[15] = datas[10];    vdata[11].data[0] = datas[11];  vdata[11].data[1] = datas[11];  vdata[11].data[2] = datas[11];  vdata[11].data[3] = datas[11];  vdata[11].data[4] = datas[11];  vdata[11].data[5] = datas[11];  vdata[11].data[6] = datas[11];  vdata[11].data[7] = datas[11];  vdata[11].data[8] = datas[11];  vdata[11].data[9] = datas[11];  vdata[11].data[10] = datas[11];  vdata[11].data[11] = datas[11];  vdata[11].data[12] = datas[11];  vdata[11].data[13] = datas[11];  vdata[11].data[14] = datas[11];  vdata[11].data[15] = datas[11];    vdata[12].data[0] = datas[12];  vdata[12].data[1] = datas[12];  vdata[12].data[2] = datas[12];  vdata[12].data[3] = datas[12];  vdata[12].data[4] = datas[12];  vdata[12].data[5] = datas[12];  vdata[12].data[6] = datas[12];  vdata[12].data[7] = datas[12];  vdata[12].data[8] = datas[12];  vdata[12].data[9] = datas[12];  vdata[12].data[10] = datas[12];  vdata[12].data[11] = datas[12];  vdata[12].data[12] = datas[12];  vdata[12].data[13] = datas[12];  vdata[12].data[14] = datas[12];  vdata[12].data[15] = datas[12];    vdata[13].data[0] = datas[13];  vdata[13].data[1] = datas[13];  vdata[13].data[2] = datas[13];  vdata[13].data[3] = datas[13];  vdata[13].data[4] = datas[13];  vdata[13].data[5] = datas[13];  vdata[13].data[6] = datas[13];  vdata[13].data[7] = datas[13];  vdata[13].data[8] = datas[13];  vdata[13].data[9] = datas[13];  vdata[13].data[10] = datas[13];  vdata[13].data[11] = datas[13];  vdata[13].data[12] = datas[13];  vdata[13].data[13] = datas[13];  vdata[13].data[14] = datas[13];  vdata[13].data[15] = datas[13];    vdata[14].data[0] = datas[14];  vdata[14].data[1] = datas[14];  vdata[14].data[2] = datas[14];  vdata[14].data[3] = datas[14];  vdata[14].data[4] = datas[14];  vdata[14].data[5] = datas[14];  vdata[14].data[6] = datas[14];  vdata[14].data[7] = datas[14];  vdata[14].data[8] = datas[14];  vdata[14].data[9] = datas[14];  vdata[14].data[10] = datas[14];  vdata[14].data[11] = datas[14];  vdata[14].data[12] = datas[14];  vdata[14].data[13] = datas[14];  vdata[14].data[14] = datas[14];  vdata[14].data[15] = datas[14];    vdata[15].data[0] = datas[15];  vdata[15].data[1] = datas[15];  vdata[15].data[2] = datas[15];  vdata[15].data[3] = datas[15];  vdata[15].data[4] = datas[15];  vdata[15].data[5] = datas[15];  vdata[15].data[6] = datas[15];  vdata[15].data[7] = datas[15];  vdata[15].data[8] = datas[15];  vdata[15].data[9] = datas[15];  vdata[15].data[10] = datas[15];  vdata[15].data[11] = datas[15];  vdata[15].data[12] = datas[15];  vdata[15].data[13] = datas[15];  vdata[15].data[14] = datas[15];  vdata[15].data[15] = datas[15];   // FIXME.
			#else 
 vdata[0].data = datas[0];  vdata[1].data = datas[1];  vdata[2].data = datas[2];  vdata[3].data = datas[3];  vdata[4].data = datas[4];  vdata[5].data = datas[5];  vdata[6].data = datas[6];  vdata[7].data = datas[7];  vdata[8].data = datas[8];  vdata[9].data = datas[9];  vdata[10].data = datas[10];  vdata[11].data = datas[11];  vdata[12].data = datas[12];  vdata[13].data = datas[13];  vdata[14].data = datas[14];  vdata[15].data = datas[15]; 	
			#endif
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(datas[v] == 1 || datas[v] < 1000){ cout<<"readANDRVchunks: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<" v: "<<v<<", voffset_kvs: "<<voffset_kvs<<", index + s: "<<index + s<<", datas["<<v<<"]: "<<datas[v]<<endl; }}
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP0_readANDRVchunks3 25", index + s, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, index, i);
			#endif
			vbuffer0[0][index + s] = vdata[0];
			vbuffer0[1][index + s] = vdata[1];
			vbuffer0[2][index + s] = vdata[2];
			vbuffer0[3][index + s] = vdata[3];
			vbuffer0[4][index + s] = vdata[4];
			vbuffer0[5][index + s] = vdata[5];
			vbuffer0[6][index + s] = vdata[6];
			vbuffer0[7][index + s] = vdata[7];
			vbuffer0[8][index + s] = vdata[8];
			vbuffer0[9][index + s] = vdata[9];
			vbuffer0[10][index + s] = vdata[10];
			vbuffer0[11][index + s] = vdata[11];
			vbuffer0[12][index + s] = vdata[12];
			vbuffer0[13][index + s] = vdata[13];
			vbuffer0[14][index + s] = vdata[14];
			vbuffer0[15][index + s] = vdata[15];
			vbuffer1[0][index + s] = vdata[0];
			vbuffer1[1][index + s] = vdata[1];
			vbuffer1[2][index + s] = vdata[2];
			vbuffer1[3][index + s] = vdata[3];
			vbuffer1[4][index + s] = vdata[4];
			vbuffer1[5][index + s] = vdata[5];
			vbuffer1[6][index + s] = vdata[6];
			vbuffer1[7][index + s] = vdata[7];
			vbuffer1[8][index + s] = vdata[8];
			vbuffer1[9][index + s] = vdata[9];
			vbuffer1[10][index + s] = vdata[10];
			vbuffer1[11][index + s] = vdata[11];
			vbuffer1[12][index + s] = vdata[12];
			vbuffer1[13][index + s] = vdata[13];
			vbuffer1[14][index + s] = vdata[14];
			vbuffer1[15][index + s] = vdata[15];
			vbuffer2[0][index + s] = vdata[0];
			vbuffer2[1][index + s] = vdata[1];
			vbuffer2[2][index + s] = vdata[2];
			vbuffer2[3][index + s] = vdata[3];
			vbuffer2[4][index + s] = vdata[4];
			vbuffer2[5][index + s] = vdata[5];
			vbuffer2[6][index + s] = vdata[6];
			vbuffer2[7][index + s] = vdata[7];
			vbuffer2[8][index + s] = vdata[8];
			vbuffer2[9][index + s] = vdata[9];
			vbuffer2[10][index + s] = vdata[10];
			vbuffer2[11][index + s] = vdata[11];
			vbuffer2[12][index + s] = vdata[12];
			vbuffer2[13][index + s] = vdata[13];
			vbuffer2[14][index + s] = vdata[14];
			vbuffer2[15][index + s] = vdata[15];
			index += NUM_PEs;

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
	}
	// exit(EXIT_SUCCESS); /////////
	return;
}
void acts_all::MEMACCESSP0_readANDRVchunks4(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	// return;//
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int limit = globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE;
	
	READANDRVCHUNKS_LOOP2: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): vsz_kvs: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		unsigned int index = 0;
		// unsigned int index = s;
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			// unsigned int offset_kvs; if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			unsigned int offset_kvs = voffset_kvs + depth_i + i;
			UTILP0_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, datas);	
			
			#ifdef BIT_TRAVERSAL_ALGORITHM
  vdata[0].data[0] = datas[0];  vdata[0].data[1] = datas[0];  vdata[0].data[2] = datas[0];  vdata[0].data[3] = datas[0];  vdata[0].data[4] = datas[0];  vdata[0].data[5] = datas[0];  vdata[0].data[6] = datas[0];  vdata[0].data[7] = datas[0];  vdata[0].data[8] = datas[0];  vdata[0].data[9] = datas[0];  vdata[0].data[10] = datas[0];  vdata[0].data[11] = datas[0];  vdata[0].data[12] = datas[0];  vdata[0].data[13] = datas[0];  vdata[0].data[14] = datas[0];  vdata[0].data[15] = datas[0];    vdata[1].data[0] = datas[1];  vdata[1].data[1] = datas[1];  vdata[1].data[2] = datas[1];  vdata[1].data[3] = datas[1];  vdata[1].data[4] = datas[1];  vdata[1].data[5] = datas[1];  vdata[1].data[6] = datas[1];  vdata[1].data[7] = datas[1];  vdata[1].data[8] = datas[1];  vdata[1].data[9] = datas[1];  vdata[1].data[10] = datas[1];  vdata[1].data[11] = datas[1];  vdata[1].data[12] = datas[1];  vdata[1].data[13] = datas[1];  vdata[1].data[14] = datas[1];  vdata[1].data[15] = datas[1];    vdata[2].data[0] = datas[2];  vdata[2].data[1] = datas[2];  vdata[2].data[2] = datas[2];  vdata[2].data[3] = datas[2];  vdata[2].data[4] = datas[2];  vdata[2].data[5] = datas[2];  vdata[2].data[6] = datas[2];  vdata[2].data[7] = datas[2];  vdata[2].data[8] = datas[2];  vdata[2].data[9] = datas[2];  vdata[2].data[10] = datas[2];  vdata[2].data[11] = datas[2];  vdata[2].data[12] = datas[2];  vdata[2].data[13] = datas[2];  vdata[2].data[14] = datas[2];  vdata[2].data[15] = datas[2];    vdata[3].data[0] = datas[3];  vdata[3].data[1] = datas[3];  vdata[3].data[2] = datas[3];  vdata[3].data[3] = datas[3];  vdata[3].data[4] = datas[3];  vdata[3].data[5] = datas[3];  vdata[3].data[6] = datas[3];  vdata[3].data[7] = datas[3];  vdata[3].data[8] = datas[3];  vdata[3].data[9] = datas[3];  vdata[3].data[10] = datas[3];  vdata[3].data[11] = datas[3];  vdata[3].data[12] = datas[3];  vdata[3].data[13] = datas[3];  vdata[3].data[14] = datas[3];  vdata[3].data[15] = datas[3];    vdata[4].data[0] = datas[4];  vdata[4].data[1] = datas[4];  vdata[4].data[2] = datas[4];  vdata[4].data[3] = datas[4];  vdata[4].data[4] = datas[4];  vdata[4].data[5] = datas[4];  vdata[4].data[6] = datas[4];  vdata[4].data[7] = datas[4];  vdata[4].data[8] = datas[4];  vdata[4].data[9] = datas[4];  vdata[4].data[10] = datas[4];  vdata[4].data[11] = datas[4];  vdata[4].data[12] = datas[4];  vdata[4].data[13] = datas[4];  vdata[4].data[14] = datas[4];  vdata[4].data[15] = datas[4];    vdata[5].data[0] = datas[5];  vdata[5].data[1] = datas[5];  vdata[5].data[2] = datas[5];  vdata[5].data[3] = datas[5];  vdata[5].data[4] = datas[5];  vdata[5].data[5] = datas[5];  vdata[5].data[6] = datas[5];  vdata[5].data[7] = datas[5];  vdata[5].data[8] = datas[5];  vdata[5].data[9] = datas[5];  vdata[5].data[10] = datas[5];  vdata[5].data[11] = datas[5];  vdata[5].data[12] = datas[5];  vdata[5].data[13] = datas[5];  vdata[5].data[14] = datas[5];  vdata[5].data[15] = datas[5];    vdata[6].data[0] = datas[6];  vdata[6].data[1] = datas[6];  vdata[6].data[2] = datas[6];  vdata[6].data[3] = datas[6];  vdata[6].data[4] = datas[6];  vdata[6].data[5] = datas[6];  vdata[6].data[6] = datas[6];  vdata[6].data[7] = datas[6];  vdata[6].data[8] = datas[6];  vdata[6].data[9] = datas[6];  vdata[6].data[10] = datas[6];  vdata[6].data[11] = datas[6];  vdata[6].data[12] = datas[6];  vdata[6].data[13] = datas[6];  vdata[6].data[14] = datas[6];  vdata[6].data[15] = datas[6];    vdata[7].data[0] = datas[7];  vdata[7].data[1] = datas[7];  vdata[7].data[2] = datas[7];  vdata[7].data[3] = datas[7];  vdata[7].data[4] = datas[7];  vdata[7].data[5] = datas[7];  vdata[7].data[6] = datas[7];  vdata[7].data[7] = datas[7];  vdata[7].data[8] = datas[7];  vdata[7].data[9] = datas[7];  vdata[7].data[10] = datas[7];  vdata[7].data[11] = datas[7];  vdata[7].data[12] = datas[7];  vdata[7].data[13] = datas[7];  vdata[7].data[14] = datas[7];  vdata[7].data[15] = datas[7];    vdata[8].data[0] = datas[8];  vdata[8].data[1] = datas[8];  vdata[8].data[2] = datas[8];  vdata[8].data[3] = datas[8];  vdata[8].data[4] = datas[8];  vdata[8].data[5] = datas[8];  vdata[8].data[6] = datas[8];  vdata[8].data[7] = datas[8];  vdata[8].data[8] = datas[8];  vdata[8].data[9] = datas[8];  vdata[8].data[10] = datas[8];  vdata[8].data[11] = datas[8];  vdata[8].data[12] = datas[8];  vdata[8].data[13] = datas[8];  vdata[8].data[14] = datas[8];  vdata[8].data[15] = datas[8];    vdata[9].data[0] = datas[9];  vdata[9].data[1] = datas[9];  vdata[9].data[2] = datas[9];  vdata[9].data[3] = datas[9];  vdata[9].data[4] = datas[9];  vdata[9].data[5] = datas[9];  vdata[9].data[6] = datas[9];  vdata[9].data[7] = datas[9];  vdata[9].data[8] = datas[9];  vdata[9].data[9] = datas[9];  vdata[9].data[10] = datas[9];  vdata[9].data[11] = datas[9];  vdata[9].data[12] = datas[9];  vdata[9].data[13] = datas[9];  vdata[9].data[14] = datas[9];  vdata[9].data[15] = datas[9];    vdata[10].data[0] = datas[10];  vdata[10].data[1] = datas[10];  vdata[10].data[2] = datas[10];  vdata[10].data[3] = datas[10];  vdata[10].data[4] = datas[10];  vdata[10].data[5] = datas[10];  vdata[10].data[6] = datas[10];  vdata[10].data[7] = datas[10];  vdata[10].data[8] = datas[10];  vdata[10].data[9] = datas[10];  vdata[10].data[10] = datas[10];  vdata[10].data[11] = datas[10];  vdata[10].data[12] = datas[10];  vdata[10].data[13] = datas[10];  vdata[10].data[14] = datas[10];  vdata[10].data[15] = datas[10];    vdata[11].data[0] = datas[11];  vdata[11].data[1] = datas[11];  vdata[11].data[2] = datas[11];  vdata[11].data[3] = datas[11];  vdata[11].data[4] = datas[11];  vdata[11].data[5] = datas[11];  vdata[11].data[6] = datas[11];  vdata[11].data[7] = datas[11];  vdata[11].data[8] = datas[11];  vdata[11].data[9] = datas[11];  vdata[11].data[10] = datas[11];  vdata[11].data[11] = datas[11];  vdata[11].data[12] = datas[11];  vdata[11].data[13] = datas[11];  vdata[11].data[14] = datas[11];  vdata[11].data[15] = datas[11];    vdata[12].data[0] = datas[12];  vdata[12].data[1] = datas[12];  vdata[12].data[2] = datas[12];  vdata[12].data[3] = datas[12];  vdata[12].data[4] = datas[12];  vdata[12].data[5] = datas[12];  vdata[12].data[6] = datas[12];  vdata[12].data[7] = datas[12];  vdata[12].data[8] = datas[12];  vdata[12].data[9] = datas[12];  vdata[12].data[10] = datas[12];  vdata[12].data[11] = datas[12];  vdata[12].data[12] = datas[12];  vdata[12].data[13] = datas[12];  vdata[12].data[14] = datas[12];  vdata[12].data[15] = datas[12];    vdata[13].data[0] = datas[13];  vdata[13].data[1] = datas[13];  vdata[13].data[2] = datas[13];  vdata[13].data[3] = datas[13];  vdata[13].data[4] = datas[13];  vdata[13].data[5] = datas[13];  vdata[13].data[6] = datas[13];  vdata[13].data[7] = datas[13];  vdata[13].data[8] = datas[13];  vdata[13].data[9] = datas[13];  vdata[13].data[10] = datas[13];  vdata[13].data[11] = datas[13];  vdata[13].data[12] = datas[13];  vdata[13].data[13] = datas[13];  vdata[13].data[14] = datas[13];  vdata[13].data[15] = datas[13];    vdata[14].data[0] = datas[14];  vdata[14].data[1] = datas[14];  vdata[14].data[2] = datas[14];  vdata[14].data[3] = datas[14];  vdata[14].data[4] = datas[14];  vdata[14].data[5] = datas[14];  vdata[14].data[6] = datas[14];  vdata[14].data[7] = datas[14];  vdata[14].data[8] = datas[14];  vdata[14].data[9] = datas[14];  vdata[14].data[10] = datas[14];  vdata[14].data[11] = datas[14];  vdata[14].data[12] = datas[14];  vdata[14].data[13] = datas[14];  vdata[14].data[14] = datas[14];  vdata[14].data[15] = datas[14];    vdata[15].data[0] = datas[15];  vdata[15].data[1] = datas[15];  vdata[15].data[2] = datas[15];  vdata[15].data[3] = datas[15];  vdata[15].data[4] = datas[15];  vdata[15].data[5] = datas[15];  vdata[15].data[6] = datas[15];  vdata[15].data[7] = datas[15];  vdata[15].data[8] = datas[15];  vdata[15].data[9] = datas[15];  vdata[15].data[10] = datas[15];  vdata[15].data[11] = datas[15];  vdata[15].data[12] = datas[15];  vdata[15].data[13] = datas[15];  vdata[15].data[14] = datas[15];  vdata[15].data[15] = datas[15];   // FIXME.
			#else 
 vdata[0].data = datas[0];  vdata[1].data = datas[1];  vdata[2].data = datas[2];  vdata[3].data = datas[3];  vdata[4].data = datas[4];  vdata[5].data = datas[5];  vdata[6].data = datas[6];  vdata[7].data = datas[7];  vdata[8].data = datas[8];  vdata[9].data = datas[9];  vdata[10].data = datas[10];  vdata[11].data = datas[11];  vdata[12].data = datas[12];  vdata[13].data = datas[13];  vdata[14].data = datas[14];  vdata[15].data = datas[15]; 	
			#endif
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(datas[v] == 1 || datas[v] < 1000){ cout<<"readANDRVchunks: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<" v: "<<v<<", voffset_kvs: "<<voffset_kvs<<", index + s: "<<index + s<<", datas["<<v<<"]: "<<datas[v]<<endl; }}
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP0_readANDRVchunks4 25", index + s, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, index, i);
			#endif
			vbuffer0[0][index + s] = vdata[0];
			vbuffer0[1][index + s] = vdata[1];
			vbuffer0[2][index + s] = vdata[2];
			vbuffer0[3][index + s] = vdata[3];
			vbuffer0[4][index + s] = vdata[4];
			vbuffer0[5][index + s] = vdata[5];
			vbuffer0[6][index + s] = vdata[6];
			vbuffer0[7][index + s] = vdata[7];
			vbuffer0[8][index + s] = vdata[8];
			vbuffer0[9][index + s] = vdata[9];
			vbuffer0[10][index + s] = vdata[10];
			vbuffer0[11][index + s] = vdata[11];
			vbuffer0[12][index + s] = vdata[12];
			vbuffer0[13][index + s] = vdata[13];
			vbuffer0[14][index + s] = vdata[14];
			vbuffer0[15][index + s] = vdata[15];
			vbuffer1[0][index + s] = vdata[0];
			vbuffer1[1][index + s] = vdata[1];
			vbuffer1[2][index + s] = vdata[2];
			vbuffer1[3][index + s] = vdata[3];
			vbuffer1[4][index + s] = vdata[4];
			vbuffer1[5][index + s] = vdata[5];
			vbuffer1[6][index + s] = vdata[6];
			vbuffer1[7][index + s] = vdata[7];
			vbuffer1[8][index + s] = vdata[8];
			vbuffer1[9][index + s] = vdata[9];
			vbuffer1[10][index + s] = vdata[10];
			vbuffer1[11][index + s] = vdata[11];
			vbuffer1[12][index + s] = vdata[12];
			vbuffer1[13][index + s] = vdata[13];
			vbuffer1[14][index + s] = vdata[14];
			vbuffer1[15][index + s] = vdata[15];
			vbuffer2[0][index + s] = vdata[0];
			vbuffer2[1][index + s] = vdata[1];
			vbuffer2[2][index + s] = vdata[2];
			vbuffer2[3][index + s] = vdata[3];
			vbuffer2[4][index + s] = vdata[4];
			vbuffer2[5][index + s] = vdata[5];
			vbuffer2[6][index + s] = vdata[6];
			vbuffer2[7][index + s] = vdata[7];
			vbuffer2[8][index + s] = vdata[8];
			vbuffer2[9][index + s] = vdata[9];
			vbuffer2[10][index + s] = vdata[10];
			vbuffer2[11][index + s] = vdata[11];
			vbuffer2[12][index + s] = vdata[12];
			vbuffer2[13][index + s] = vdata[13];
			vbuffer2[14][index + s] = vdata[14];
			vbuffer2[15][index + s] = vdata[15];
			vbuffer3[0][index + s] = vdata[0];
			vbuffer3[1][index + s] = vdata[1];
			vbuffer3[2][index + s] = vdata[2];
			vbuffer3[3][index + s] = vdata[3];
			vbuffer3[4][index + s] = vdata[4];
			vbuffer3[5][index + s] = vdata[5];
			vbuffer3[6][index + s] = vdata[6];
			vbuffer3[7][index + s] = vdata[7];
			vbuffer3[8][index + s] = vdata[8];
			vbuffer3[9][index + s] = vdata[9];
			vbuffer3[10][index + s] = vdata[10];
			vbuffer3[11][index + s] = vdata[11];
			vbuffer3[12][index + s] = vdata[12];
			vbuffer3[13][index + s] = vdata[13];
			vbuffer3[14][index + s] = vdata[14];
			vbuffer3[15][index + s] = vdata[15];
			index += NUM_PEs;

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
	}
	// exit(EXIT_SUCCESS); /////////
	return;
}
void acts_all::MEMACCESSP0_readANDRVchunks5(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	// return;//
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int limit = globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE;
	
	READANDRVCHUNKS_LOOP2: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): vsz_kvs: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		unsigned int index = 0;
		// unsigned int index = s;
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			// unsigned int offset_kvs; if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			unsigned int offset_kvs = voffset_kvs + depth_i + i;
			UTILP0_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, datas);	
			
			#ifdef BIT_TRAVERSAL_ALGORITHM
  vdata[0].data[0] = datas[0];  vdata[0].data[1] = datas[0];  vdata[0].data[2] = datas[0];  vdata[0].data[3] = datas[0];  vdata[0].data[4] = datas[0];  vdata[0].data[5] = datas[0];  vdata[0].data[6] = datas[0];  vdata[0].data[7] = datas[0];  vdata[0].data[8] = datas[0];  vdata[0].data[9] = datas[0];  vdata[0].data[10] = datas[0];  vdata[0].data[11] = datas[0];  vdata[0].data[12] = datas[0];  vdata[0].data[13] = datas[0];  vdata[0].data[14] = datas[0];  vdata[0].data[15] = datas[0];    vdata[1].data[0] = datas[1];  vdata[1].data[1] = datas[1];  vdata[1].data[2] = datas[1];  vdata[1].data[3] = datas[1];  vdata[1].data[4] = datas[1];  vdata[1].data[5] = datas[1];  vdata[1].data[6] = datas[1];  vdata[1].data[7] = datas[1];  vdata[1].data[8] = datas[1];  vdata[1].data[9] = datas[1];  vdata[1].data[10] = datas[1];  vdata[1].data[11] = datas[1];  vdata[1].data[12] = datas[1];  vdata[1].data[13] = datas[1];  vdata[1].data[14] = datas[1];  vdata[1].data[15] = datas[1];    vdata[2].data[0] = datas[2];  vdata[2].data[1] = datas[2];  vdata[2].data[2] = datas[2];  vdata[2].data[3] = datas[2];  vdata[2].data[4] = datas[2];  vdata[2].data[5] = datas[2];  vdata[2].data[6] = datas[2];  vdata[2].data[7] = datas[2];  vdata[2].data[8] = datas[2];  vdata[2].data[9] = datas[2];  vdata[2].data[10] = datas[2];  vdata[2].data[11] = datas[2];  vdata[2].data[12] = datas[2];  vdata[2].data[13] = datas[2];  vdata[2].data[14] = datas[2];  vdata[2].data[15] = datas[2];    vdata[3].data[0] = datas[3];  vdata[3].data[1] = datas[3];  vdata[3].data[2] = datas[3];  vdata[3].data[3] = datas[3];  vdata[3].data[4] = datas[3];  vdata[3].data[5] = datas[3];  vdata[3].data[6] = datas[3];  vdata[3].data[7] = datas[3];  vdata[3].data[8] = datas[3];  vdata[3].data[9] = datas[3];  vdata[3].data[10] = datas[3];  vdata[3].data[11] = datas[3];  vdata[3].data[12] = datas[3];  vdata[3].data[13] = datas[3];  vdata[3].data[14] = datas[3];  vdata[3].data[15] = datas[3];    vdata[4].data[0] = datas[4];  vdata[4].data[1] = datas[4];  vdata[4].data[2] = datas[4];  vdata[4].data[3] = datas[4];  vdata[4].data[4] = datas[4];  vdata[4].data[5] = datas[4];  vdata[4].data[6] = datas[4];  vdata[4].data[7] = datas[4];  vdata[4].data[8] = datas[4];  vdata[4].data[9] = datas[4];  vdata[4].data[10] = datas[4];  vdata[4].data[11] = datas[4];  vdata[4].data[12] = datas[4];  vdata[4].data[13] = datas[4];  vdata[4].data[14] = datas[4];  vdata[4].data[15] = datas[4];    vdata[5].data[0] = datas[5];  vdata[5].data[1] = datas[5];  vdata[5].data[2] = datas[5];  vdata[5].data[3] = datas[5];  vdata[5].data[4] = datas[5];  vdata[5].data[5] = datas[5];  vdata[5].data[6] = datas[5];  vdata[5].data[7] = datas[5];  vdata[5].data[8] = datas[5];  vdata[5].data[9] = datas[5];  vdata[5].data[10] = datas[5];  vdata[5].data[11] = datas[5];  vdata[5].data[12] = datas[5];  vdata[5].data[13] = datas[5];  vdata[5].data[14] = datas[5];  vdata[5].data[15] = datas[5];    vdata[6].data[0] = datas[6];  vdata[6].data[1] = datas[6];  vdata[6].data[2] = datas[6];  vdata[6].data[3] = datas[6];  vdata[6].data[4] = datas[6];  vdata[6].data[5] = datas[6];  vdata[6].data[6] = datas[6];  vdata[6].data[7] = datas[6];  vdata[6].data[8] = datas[6];  vdata[6].data[9] = datas[6];  vdata[6].data[10] = datas[6];  vdata[6].data[11] = datas[6];  vdata[6].data[12] = datas[6];  vdata[6].data[13] = datas[6];  vdata[6].data[14] = datas[6];  vdata[6].data[15] = datas[6];    vdata[7].data[0] = datas[7];  vdata[7].data[1] = datas[7];  vdata[7].data[2] = datas[7];  vdata[7].data[3] = datas[7];  vdata[7].data[4] = datas[7];  vdata[7].data[5] = datas[7];  vdata[7].data[6] = datas[7];  vdata[7].data[7] = datas[7];  vdata[7].data[8] = datas[7];  vdata[7].data[9] = datas[7];  vdata[7].data[10] = datas[7];  vdata[7].data[11] = datas[7];  vdata[7].data[12] = datas[7];  vdata[7].data[13] = datas[7];  vdata[7].data[14] = datas[7];  vdata[7].data[15] = datas[7];    vdata[8].data[0] = datas[8];  vdata[8].data[1] = datas[8];  vdata[8].data[2] = datas[8];  vdata[8].data[3] = datas[8];  vdata[8].data[4] = datas[8];  vdata[8].data[5] = datas[8];  vdata[8].data[6] = datas[8];  vdata[8].data[7] = datas[8];  vdata[8].data[8] = datas[8];  vdata[8].data[9] = datas[8];  vdata[8].data[10] = datas[8];  vdata[8].data[11] = datas[8];  vdata[8].data[12] = datas[8];  vdata[8].data[13] = datas[8];  vdata[8].data[14] = datas[8];  vdata[8].data[15] = datas[8];    vdata[9].data[0] = datas[9];  vdata[9].data[1] = datas[9];  vdata[9].data[2] = datas[9];  vdata[9].data[3] = datas[9];  vdata[9].data[4] = datas[9];  vdata[9].data[5] = datas[9];  vdata[9].data[6] = datas[9];  vdata[9].data[7] = datas[9];  vdata[9].data[8] = datas[9];  vdata[9].data[9] = datas[9];  vdata[9].data[10] = datas[9];  vdata[9].data[11] = datas[9];  vdata[9].data[12] = datas[9];  vdata[9].data[13] = datas[9];  vdata[9].data[14] = datas[9];  vdata[9].data[15] = datas[9];    vdata[10].data[0] = datas[10];  vdata[10].data[1] = datas[10];  vdata[10].data[2] = datas[10];  vdata[10].data[3] = datas[10];  vdata[10].data[4] = datas[10];  vdata[10].data[5] = datas[10];  vdata[10].data[6] = datas[10];  vdata[10].data[7] = datas[10];  vdata[10].data[8] = datas[10];  vdata[10].data[9] = datas[10];  vdata[10].data[10] = datas[10];  vdata[10].data[11] = datas[10];  vdata[10].data[12] = datas[10];  vdata[10].data[13] = datas[10];  vdata[10].data[14] = datas[10];  vdata[10].data[15] = datas[10];    vdata[11].data[0] = datas[11];  vdata[11].data[1] = datas[11];  vdata[11].data[2] = datas[11];  vdata[11].data[3] = datas[11];  vdata[11].data[4] = datas[11];  vdata[11].data[5] = datas[11];  vdata[11].data[6] = datas[11];  vdata[11].data[7] = datas[11];  vdata[11].data[8] = datas[11];  vdata[11].data[9] = datas[11];  vdata[11].data[10] = datas[11];  vdata[11].data[11] = datas[11];  vdata[11].data[12] = datas[11];  vdata[11].data[13] = datas[11];  vdata[11].data[14] = datas[11];  vdata[11].data[15] = datas[11];    vdata[12].data[0] = datas[12];  vdata[12].data[1] = datas[12];  vdata[12].data[2] = datas[12];  vdata[12].data[3] = datas[12];  vdata[12].data[4] = datas[12];  vdata[12].data[5] = datas[12];  vdata[12].data[6] = datas[12];  vdata[12].data[7] = datas[12];  vdata[12].data[8] = datas[12];  vdata[12].data[9] = datas[12];  vdata[12].data[10] = datas[12];  vdata[12].data[11] = datas[12];  vdata[12].data[12] = datas[12];  vdata[12].data[13] = datas[12];  vdata[12].data[14] = datas[12];  vdata[12].data[15] = datas[12];    vdata[13].data[0] = datas[13];  vdata[13].data[1] = datas[13];  vdata[13].data[2] = datas[13];  vdata[13].data[3] = datas[13];  vdata[13].data[4] = datas[13];  vdata[13].data[5] = datas[13];  vdata[13].data[6] = datas[13];  vdata[13].data[7] = datas[13];  vdata[13].data[8] = datas[13];  vdata[13].data[9] = datas[13];  vdata[13].data[10] = datas[13];  vdata[13].data[11] = datas[13];  vdata[13].data[12] = datas[13];  vdata[13].data[13] = datas[13];  vdata[13].data[14] = datas[13];  vdata[13].data[15] = datas[13];    vdata[14].data[0] = datas[14];  vdata[14].data[1] = datas[14];  vdata[14].data[2] = datas[14];  vdata[14].data[3] = datas[14];  vdata[14].data[4] = datas[14];  vdata[14].data[5] = datas[14];  vdata[14].data[6] = datas[14];  vdata[14].data[7] = datas[14];  vdata[14].data[8] = datas[14];  vdata[14].data[9] = datas[14];  vdata[14].data[10] = datas[14];  vdata[14].data[11] = datas[14];  vdata[14].data[12] = datas[14];  vdata[14].data[13] = datas[14];  vdata[14].data[14] = datas[14];  vdata[14].data[15] = datas[14];    vdata[15].data[0] = datas[15];  vdata[15].data[1] = datas[15];  vdata[15].data[2] = datas[15];  vdata[15].data[3] = datas[15];  vdata[15].data[4] = datas[15];  vdata[15].data[5] = datas[15];  vdata[15].data[6] = datas[15];  vdata[15].data[7] = datas[15];  vdata[15].data[8] = datas[15];  vdata[15].data[9] = datas[15];  vdata[15].data[10] = datas[15];  vdata[15].data[11] = datas[15];  vdata[15].data[12] = datas[15];  vdata[15].data[13] = datas[15];  vdata[15].data[14] = datas[15];  vdata[15].data[15] = datas[15];   // FIXME.
			#else 
 vdata[0].data = datas[0];  vdata[1].data = datas[1];  vdata[2].data = datas[2];  vdata[3].data = datas[3];  vdata[4].data = datas[4];  vdata[5].data = datas[5];  vdata[6].data = datas[6];  vdata[7].data = datas[7];  vdata[8].data = datas[8];  vdata[9].data = datas[9];  vdata[10].data = datas[10];  vdata[11].data = datas[11];  vdata[12].data = datas[12];  vdata[13].data = datas[13];  vdata[14].data = datas[14];  vdata[15].data = datas[15]; 	
			#endif
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(datas[v] == 1 || datas[v] < 1000){ cout<<"readANDRVchunks: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<" v: "<<v<<", voffset_kvs: "<<voffset_kvs<<", index + s: "<<index + s<<", datas["<<v<<"]: "<<datas[v]<<endl; }}
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP0_readANDRVchunks5 25", index + s, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, index, i);
			#endif
			vbuffer0[0][index + s] = vdata[0];
			vbuffer0[1][index + s] = vdata[1];
			vbuffer0[2][index + s] = vdata[2];
			vbuffer0[3][index + s] = vdata[3];
			vbuffer0[4][index + s] = vdata[4];
			vbuffer0[5][index + s] = vdata[5];
			vbuffer0[6][index + s] = vdata[6];
			vbuffer0[7][index + s] = vdata[7];
			vbuffer0[8][index + s] = vdata[8];
			vbuffer0[9][index + s] = vdata[9];
			vbuffer0[10][index + s] = vdata[10];
			vbuffer0[11][index + s] = vdata[11];
			vbuffer0[12][index + s] = vdata[12];
			vbuffer0[13][index + s] = vdata[13];
			vbuffer0[14][index + s] = vdata[14];
			vbuffer0[15][index + s] = vdata[15];
			vbuffer1[0][index + s] = vdata[0];
			vbuffer1[1][index + s] = vdata[1];
			vbuffer1[2][index + s] = vdata[2];
			vbuffer1[3][index + s] = vdata[3];
			vbuffer1[4][index + s] = vdata[4];
			vbuffer1[5][index + s] = vdata[5];
			vbuffer1[6][index + s] = vdata[6];
			vbuffer1[7][index + s] = vdata[7];
			vbuffer1[8][index + s] = vdata[8];
			vbuffer1[9][index + s] = vdata[9];
			vbuffer1[10][index + s] = vdata[10];
			vbuffer1[11][index + s] = vdata[11];
			vbuffer1[12][index + s] = vdata[12];
			vbuffer1[13][index + s] = vdata[13];
			vbuffer1[14][index + s] = vdata[14];
			vbuffer1[15][index + s] = vdata[15];
			vbuffer2[0][index + s] = vdata[0];
			vbuffer2[1][index + s] = vdata[1];
			vbuffer2[2][index + s] = vdata[2];
			vbuffer2[3][index + s] = vdata[3];
			vbuffer2[4][index + s] = vdata[4];
			vbuffer2[5][index + s] = vdata[5];
			vbuffer2[6][index + s] = vdata[6];
			vbuffer2[7][index + s] = vdata[7];
			vbuffer2[8][index + s] = vdata[8];
			vbuffer2[9][index + s] = vdata[9];
			vbuffer2[10][index + s] = vdata[10];
			vbuffer2[11][index + s] = vdata[11];
			vbuffer2[12][index + s] = vdata[12];
			vbuffer2[13][index + s] = vdata[13];
			vbuffer2[14][index + s] = vdata[14];
			vbuffer2[15][index + s] = vdata[15];
			vbuffer3[0][index + s] = vdata[0];
			vbuffer3[1][index + s] = vdata[1];
			vbuffer3[2][index + s] = vdata[2];
			vbuffer3[3][index + s] = vdata[3];
			vbuffer3[4][index + s] = vdata[4];
			vbuffer3[5][index + s] = vdata[5];
			vbuffer3[6][index + s] = vdata[6];
			vbuffer3[7][index + s] = vdata[7];
			vbuffer3[8][index + s] = vdata[8];
			vbuffer3[9][index + s] = vdata[9];
			vbuffer3[10][index + s] = vdata[10];
			vbuffer3[11][index + s] = vdata[11];
			vbuffer3[12][index + s] = vdata[12];
			vbuffer3[13][index + s] = vdata[13];
			vbuffer3[14][index + s] = vdata[14];
			vbuffer3[15][index + s] = vdata[15];
			vbuffer4[0][index + s] = vdata[0];
			vbuffer4[1][index + s] = vdata[1];
			vbuffer4[2][index + s] = vdata[2];
			vbuffer4[3][index + s] = vdata[3];
			vbuffer4[4][index + s] = vdata[4];
			vbuffer4[5][index + s] = vdata[5];
			vbuffer4[6][index + s] = vdata[6];
			vbuffer4[7][index + s] = vdata[7];
			vbuffer4[8][index + s] = vdata[8];
			vbuffer4[9][index + s] = vdata[9];
			vbuffer4[10][index + s] = vdata[10];
			vbuffer4[11][index + s] = vdata[11];
			vbuffer4[12][index + s] = vdata[12];
			vbuffer4[13][index + s] = vdata[13];
			vbuffer4[14][index + s] = vdata[14];
			vbuffer4[15][index + s] = vdata[15];
			index += NUM_PEs;

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
	}
	// exit(EXIT_SUCCESS); /////////
	return;
}
void acts_all::MEMACCESSP0_readANDRVchunks6(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	// return;//
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int limit = globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE;
	
	READANDRVCHUNKS_LOOP2: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): vsz_kvs: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		unsigned int index = 0;
		// unsigned int index = s;
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			// unsigned int offset_kvs; if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			unsigned int offset_kvs = voffset_kvs + depth_i + i;
			UTILP0_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, datas);	
			
			#ifdef BIT_TRAVERSAL_ALGORITHM
  vdata[0].data[0] = datas[0];  vdata[0].data[1] = datas[0];  vdata[0].data[2] = datas[0];  vdata[0].data[3] = datas[0];  vdata[0].data[4] = datas[0];  vdata[0].data[5] = datas[0];  vdata[0].data[6] = datas[0];  vdata[0].data[7] = datas[0];  vdata[0].data[8] = datas[0];  vdata[0].data[9] = datas[0];  vdata[0].data[10] = datas[0];  vdata[0].data[11] = datas[0];  vdata[0].data[12] = datas[0];  vdata[0].data[13] = datas[0];  vdata[0].data[14] = datas[0];  vdata[0].data[15] = datas[0];    vdata[1].data[0] = datas[1];  vdata[1].data[1] = datas[1];  vdata[1].data[2] = datas[1];  vdata[1].data[3] = datas[1];  vdata[1].data[4] = datas[1];  vdata[1].data[5] = datas[1];  vdata[1].data[6] = datas[1];  vdata[1].data[7] = datas[1];  vdata[1].data[8] = datas[1];  vdata[1].data[9] = datas[1];  vdata[1].data[10] = datas[1];  vdata[1].data[11] = datas[1];  vdata[1].data[12] = datas[1];  vdata[1].data[13] = datas[1];  vdata[1].data[14] = datas[1];  vdata[1].data[15] = datas[1];    vdata[2].data[0] = datas[2];  vdata[2].data[1] = datas[2];  vdata[2].data[2] = datas[2];  vdata[2].data[3] = datas[2];  vdata[2].data[4] = datas[2];  vdata[2].data[5] = datas[2];  vdata[2].data[6] = datas[2];  vdata[2].data[7] = datas[2];  vdata[2].data[8] = datas[2];  vdata[2].data[9] = datas[2];  vdata[2].data[10] = datas[2];  vdata[2].data[11] = datas[2];  vdata[2].data[12] = datas[2];  vdata[2].data[13] = datas[2];  vdata[2].data[14] = datas[2];  vdata[2].data[15] = datas[2];    vdata[3].data[0] = datas[3];  vdata[3].data[1] = datas[3];  vdata[3].data[2] = datas[3];  vdata[3].data[3] = datas[3];  vdata[3].data[4] = datas[3];  vdata[3].data[5] = datas[3];  vdata[3].data[6] = datas[3];  vdata[3].data[7] = datas[3];  vdata[3].data[8] = datas[3];  vdata[3].data[9] = datas[3];  vdata[3].data[10] = datas[3];  vdata[3].data[11] = datas[3];  vdata[3].data[12] = datas[3];  vdata[3].data[13] = datas[3];  vdata[3].data[14] = datas[3];  vdata[3].data[15] = datas[3];    vdata[4].data[0] = datas[4];  vdata[4].data[1] = datas[4];  vdata[4].data[2] = datas[4];  vdata[4].data[3] = datas[4];  vdata[4].data[4] = datas[4];  vdata[4].data[5] = datas[4];  vdata[4].data[6] = datas[4];  vdata[4].data[7] = datas[4];  vdata[4].data[8] = datas[4];  vdata[4].data[9] = datas[4];  vdata[4].data[10] = datas[4];  vdata[4].data[11] = datas[4];  vdata[4].data[12] = datas[4];  vdata[4].data[13] = datas[4];  vdata[4].data[14] = datas[4];  vdata[4].data[15] = datas[4];    vdata[5].data[0] = datas[5];  vdata[5].data[1] = datas[5];  vdata[5].data[2] = datas[5];  vdata[5].data[3] = datas[5];  vdata[5].data[4] = datas[5];  vdata[5].data[5] = datas[5];  vdata[5].data[6] = datas[5];  vdata[5].data[7] = datas[5];  vdata[5].data[8] = datas[5];  vdata[5].data[9] = datas[5];  vdata[5].data[10] = datas[5];  vdata[5].data[11] = datas[5];  vdata[5].data[12] = datas[5];  vdata[5].data[13] = datas[5];  vdata[5].data[14] = datas[5];  vdata[5].data[15] = datas[5];    vdata[6].data[0] = datas[6];  vdata[6].data[1] = datas[6];  vdata[6].data[2] = datas[6];  vdata[6].data[3] = datas[6];  vdata[6].data[4] = datas[6];  vdata[6].data[5] = datas[6];  vdata[6].data[6] = datas[6];  vdata[6].data[7] = datas[6];  vdata[6].data[8] = datas[6];  vdata[6].data[9] = datas[6];  vdata[6].data[10] = datas[6];  vdata[6].data[11] = datas[6];  vdata[6].data[12] = datas[6];  vdata[6].data[13] = datas[6];  vdata[6].data[14] = datas[6];  vdata[6].data[15] = datas[6];    vdata[7].data[0] = datas[7];  vdata[7].data[1] = datas[7];  vdata[7].data[2] = datas[7];  vdata[7].data[3] = datas[7];  vdata[7].data[4] = datas[7];  vdata[7].data[5] = datas[7];  vdata[7].data[6] = datas[7];  vdata[7].data[7] = datas[7];  vdata[7].data[8] = datas[7];  vdata[7].data[9] = datas[7];  vdata[7].data[10] = datas[7];  vdata[7].data[11] = datas[7];  vdata[7].data[12] = datas[7];  vdata[7].data[13] = datas[7];  vdata[7].data[14] = datas[7];  vdata[7].data[15] = datas[7];    vdata[8].data[0] = datas[8];  vdata[8].data[1] = datas[8];  vdata[8].data[2] = datas[8];  vdata[8].data[3] = datas[8];  vdata[8].data[4] = datas[8];  vdata[8].data[5] = datas[8];  vdata[8].data[6] = datas[8];  vdata[8].data[7] = datas[8];  vdata[8].data[8] = datas[8];  vdata[8].data[9] = datas[8];  vdata[8].data[10] = datas[8];  vdata[8].data[11] = datas[8];  vdata[8].data[12] = datas[8];  vdata[8].data[13] = datas[8];  vdata[8].data[14] = datas[8];  vdata[8].data[15] = datas[8];    vdata[9].data[0] = datas[9];  vdata[9].data[1] = datas[9];  vdata[9].data[2] = datas[9];  vdata[9].data[3] = datas[9];  vdata[9].data[4] = datas[9];  vdata[9].data[5] = datas[9];  vdata[9].data[6] = datas[9];  vdata[9].data[7] = datas[9];  vdata[9].data[8] = datas[9];  vdata[9].data[9] = datas[9];  vdata[9].data[10] = datas[9];  vdata[9].data[11] = datas[9];  vdata[9].data[12] = datas[9];  vdata[9].data[13] = datas[9];  vdata[9].data[14] = datas[9];  vdata[9].data[15] = datas[9];    vdata[10].data[0] = datas[10];  vdata[10].data[1] = datas[10];  vdata[10].data[2] = datas[10];  vdata[10].data[3] = datas[10];  vdata[10].data[4] = datas[10];  vdata[10].data[5] = datas[10];  vdata[10].data[6] = datas[10];  vdata[10].data[7] = datas[10];  vdata[10].data[8] = datas[10];  vdata[10].data[9] = datas[10];  vdata[10].data[10] = datas[10];  vdata[10].data[11] = datas[10];  vdata[10].data[12] = datas[10];  vdata[10].data[13] = datas[10];  vdata[10].data[14] = datas[10];  vdata[10].data[15] = datas[10];    vdata[11].data[0] = datas[11];  vdata[11].data[1] = datas[11];  vdata[11].data[2] = datas[11];  vdata[11].data[3] = datas[11];  vdata[11].data[4] = datas[11];  vdata[11].data[5] = datas[11];  vdata[11].data[6] = datas[11];  vdata[11].data[7] = datas[11];  vdata[11].data[8] = datas[11];  vdata[11].data[9] = datas[11];  vdata[11].data[10] = datas[11];  vdata[11].data[11] = datas[11];  vdata[11].data[12] = datas[11];  vdata[11].data[13] = datas[11];  vdata[11].data[14] = datas[11];  vdata[11].data[15] = datas[11];    vdata[12].data[0] = datas[12];  vdata[12].data[1] = datas[12];  vdata[12].data[2] = datas[12];  vdata[12].data[3] = datas[12];  vdata[12].data[4] = datas[12];  vdata[12].data[5] = datas[12];  vdata[12].data[6] = datas[12];  vdata[12].data[7] = datas[12];  vdata[12].data[8] = datas[12];  vdata[12].data[9] = datas[12];  vdata[12].data[10] = datas[12];  vdata[12].data[11] = datas[12];  vdata[12].data[12] = datas[12];  vdata[12].data[13] = datas[12];  vdata[12].data[14] = datas[12];  vdata[12].data[15] = datas[12];    vdata[13].data[0] = datas[13];  vdata[13].data[1] = datas[13];  vdata[13].data[2] = datas[13];  vdata[13].data[3] = datas[13];  vdata[13].data[4] = datas[13];  vdata[13].data[5] = datas[13];  vdata[13].data[6] = datas[13];  vdata[13].data[7] = datas[13];  vdata[13].data[8] = datas[13];  vdata[13].data[9] = datas[13];  vdata[13].data[10] = datas[13];  vdata[13].data[11] = datas[13];  vdata[13].data[12] = datas[13];  vdata[13].data[13] = datas[13];  vdata[13].data[14] = datas[13];  vdata[13].data[15] = datas[13];    vdata[14].data[0] = datas[14];  vdata[14].data[1] = datas[14];  vdata[14].data[2] = datas[14];  vdata[14].data[3] = datas[14];  vdata[14].data[4] = datas[14];  vdata[14].data[5] = datas[14];  vdata[14].data[6] = datas[14];  vdata[14].data[7] = datas[14];  vdata[14].data[8] = datas[14];  vdata[14].data[9] = datas[14];  vdata[14].data[10] = datas[14];  vdata[14].data[11] = datas[14];  vdata[14].data[12] = datas[14];  vdata[14].data[13] = datas[14];  vdata[14].data[14] = datas[14];  vdata[14].data[15] = datas[14];    vdata[15].data[0] = datas[15];  vdata[15].data[1] = datas[15];  vdata[15].data[2] = datas[15];  vdata[15].data[3] = datas[15];  vdata[15].data[4] = datas[15];  vdata[15].data[5] = datas[15];  vdata[15].data[6] = datas[15];  vdata[15].data[7] = datas[15];  vdata[15].data[8] = datas[15];  vdata[15].data[9] = datas[15];  vdata[15].data[10] = datas[15];  vdata[15].data[11] = datas[15];  vdata[15].data[12] = datas[15];  vdata[15].data[13] = datas[15];  vdata[15].data[14] = datas[15];  vdata[15].data[15] = datas[15];   // FIXME.
			#else 
 vdata[0].data = datas[0];  vdata[1].data = datas[1];  vdata[2].data = datas[2];  vdata[3].data = datas[3];  vdata[4].data = datas[4];  vdata[5].data = datas[5];  vdata[6].data = datas[6];  vdata[7].data = datas[7];  vdata[8].data = datas[8];  vdata[9].data = datas[9];  vdata[10].data = datas[10];  vdata[11].data = datas[11];  vdata[12].data = datas[12];  vdata[13].data = datas[13];  vdata[14].data = datas[14];  vdata[15].data = datas[15]; 	
			#endif
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(datas[v] == 1 || datas[v] < 1000){ cout<<"readANDRVchunks: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<" v: "<<v<<", voffset_kvs: "<<voffset_kvs<<", index + s: "<<index + s<<", datas["<<v<<"]: "<<datas[v]<<endl; }}
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP0_readANDRVchunks6 25", index + s, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, index, i);
			#endif
			vbuffer0[0][index + s] = vdata[0];
			vbuffer0[1][index + s] = vdata[1];
			vbuffer0[2][index + s] = vdata[2];
			vbuffer0[3][index + s] = vdata[3];
			vbuffer0[4][index + s] = vdata[4];
			vbuffer0[5][index + s] = vdata[5];
			vbuffer0[6][index + s] = vdata[6];
			vbuffer0[7][index + s] = vdata[7];
			vbuffer0[8][index + s] = vdata[8];
			vbuffer0[9][index + s] = vdata[9];
			vbuffer0[10][index + s] = vdata[10];
			vbuffer0[11][index + s] = vdata[11];
			vbuffer0[12][index + s] = vdata[12];
			vbuffer0[13][index + s] = vdata[13];
			vbuffer0[14][index + s] = vdata[14];
			vbuffer0[15][index + s] = vdata[15];
			vbuffer1[0][index + s] = vdata[0];
			vbuffer1[1][index + s] = vdata[1];
			vbuffer1[2][index + s] = vdata[2];
			vbuffer1[3][index + s] = vdata[3];
			vbuffer1[4][index + s] = vdata[4];
			vbuffer1[5][index + s] = vdata[5];
			vbuffer1[6][index + s] = vdata[6];
			vbuffer1[7][index + s] = vdata[7];
			vbuffer1[8][index + s] = vdata[8];
			vbuffer1[9][index + s] = vdata[9];
			vbuffer1[10][index + s] = vdata[10];
			vbuffer1[11][index + s] = vdata[11];
			vbuffer1[12][index + s] = vdata[12];
			vbuffer1[13][index + s] = vdata[13];
			vbuffer1[14][index + s] = vdata[14];
			vbuffer1[15][index + s] = vdata[15];
			vbuffer2[0][index + s] = vdata[0];
			vbuffer2[1][index + s] = vdata[1];
			vbuffer2[2][index + s] = vdata[2];
			vbuffer2[3][index + s] = vdata[3];
			vbuffer2[4][index + s] = vdata[4];
			vbuffer2[5][index + s] = vdata[5];
			vbuffer2[6][index + s] = vdata[6];
			vbuffer2[7][index + s] = vdata[7];
			vbuffer2[8][index + s] = vdata[8];
			vbuffer2[9][index + s] = vdata[9];
			vbuffer2[10][index + s] = vdata[10];
			vbuffer2[11][index + s] = vdata[11];
			vbuffer2[12][index + s] = vdata[12];
			vbuffer2[13][index + s] = vdata[13];
			vbuffer2[14][index + s] = vdata[14];
			vbuffer2[15][index + s] = vdata[15];
			vbuffer3[0][index + s] = vdata[0];
			vbuffer3[1][index + s] = vdata[1];
			vbuffer3[2][index + s] = vdata[2];
			vbuffer3[3][index + s] = vdata[3];
			vbuffer3[4][index + s] = vdata[4];
			vbuffer3[5][index + s] = vdata[5];
			vbuffer3[6][index + s] = vdata[6];
			vbuffer3[7][index + s] = vdata[7];
			vbuffer3[8][index + s] = vdata[8];
			vbuffer3[9][index + s] = vdata[9];
			vbuffer3[10][index + s] = vdata[10];
			vbuffer3[11][index + s] = vdata[11];
			vbuffer3[12][index + s] = vdata[12];
			vbuffer3[13][index + s] = vdata[13];
			vbuffer3[14][index + s] = vdata[14];
			vbuffer3[15][index + s] = vdata[15];
			vbuffer4[0][index + s] = vdata[0];
			vbuffer4[1][index + s] = vdata[1];
			vbuffer4[2][index + s] = vdata[2];
			vbuffer4[3][index + s] = vdata[3];
			vbuffer4[4][index + s] = vdata[4];
			vbuffer4[5][index + s] = vdata[5];
			vbuffer4[6][index + s] = vdata[6];
			vbuffer4[7][index + s] = vdata[7];
			vbuffer4[8][index + s] = vdata[8];
			vbuffer4[9][index + s] = vdata[9];
			vbuffer4[10][index + s] = vdata[10];
			vbuffer4[11][index + s] = vdata[11];
			vbuffer4[12][index + s] = vdata[12];
			vbuffer4[13][index + s] = vdata[13];
			vbuffer4[14][index + s] = vdata[14];
			vbuffer4[15][index + s] = vdata[15];
			vbuffer5[0][index + s] = vdata[0];
			vbuffer5[1][index + s] = vdata[1];
			vbuffer5[2][index + s] = vdata[2];
			vbuffer5[3][index + s] = vdata[3];
			vbuffer5[4][index + s] = vdata[4];
			vbuffer5[5][index + s] = vdata[5];
			vbuffer5[6][index + s] = vdata[6];
			vbuffer5[7][index + s] = vdata[7];
			vbuffer5[8][index + s] = vdata[8];
			vbuffer5[9][index + s] = vdata[9];
			vbuffer5[10][index + s] = vdata[10];
			vbuffer5[11][index + s] = vdata[11];
			vbuffer5[12][index + s] = vdata[12];
			vbuffer5[13][index + s] = vdata[13];
			vbuffer5[14][index + s] = vdata[14];
			vbuffer5[15][index + s] = vdata[15];
			index += NUM_PEs;

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
	}
	// exit(EXIT_SUCCESS); /////////
	return;
}
void acts_all::MEMACCESSP0_readANDRVchunks7(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	// return;//
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int limit = globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE;
	
	READANDRVCHUNKS_LOOP2: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): vsz_kvs: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		unsigned int index = 0;
		// unsigned int index = s;
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			// unsigned int offset_kvs; if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			unsigned int offset_kvs = voffset_kvs + depth_i + i;
			UTILP0_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, datas);	
			
			#ifdef BIT_TRAVERSAL_ALGORITHM
  vdata[0].data[0] = datas[0];  vdata[0].data[1] = datas[0];  vdata[0].data[2] = datas[0];  vdata[0].data[3] = datas[0];  vdata[0].data[4] = datas[0];  vdata[0].data[5] = datas[0];  vdata[0].data[6] = datas[0];  vdata[0].data[7] = datas[0];  vdata[0].data[8] = datas[0];  vdata[0].data[9] = datas[0];  vdata[0].data[10] = datas[0];  vdata[0].data[11] = datas[0];  vdata[0].data[12] = datas[0];  vdata[0].data[13] = datas[0];  vdata[0].data[14] = datas[0];  vdata[0].data[15] = datas[0];    vdata[1].data[0] = datas[1];  vdata[1].data[1] = datas[1];  vdata[1].data[2] = datas[1];  vdata[1].data[3] = datas[1];  vdata[1].data[4] = datas[1];  vdata[1].data[5] = datas[1];  vdata[1].data[6] = datas[1];  vdata[1].data[7] = datas[1];  vdata[1].data[8] = datas[1];  vdata[1].data[9] = datas[1];  vdata[1].data[10] = datas[1];  vdata[1].data[11] = datas[1];  vdata[1].data[12] = datas[1];  vdata[1].data[13] = datas[1];  vdata[1].data[14] = datas[1];  vdata[1].data[15] = datas[1];    vdata[2].data[0] = datas[2];  vdata[2].data[1] = datas[2];  vdata[2].data[2] = datas[2];  vdata[2].data[3] = datas[2];  vdata[2].data[4] = datas[2];  vdata[2].data[5] = datas[2];  vdata[2].data[6] = datas[2];  vdata[2].data[7] = datas[2];  vdata[2].data[8] = datas[2];  vdata[2].data[9] = datas[2];  vdata[2].data[10] = datas[2];  vdata[2].data[11] = datas[2];  vdata[2].data[12] = datas[2];  vdata[2].data[13] = datas[2];  vdata[2].data[14] = datas[2];  vdata[2].data[15] = datas[2];    vdata[3].data[0] = datas[3];  vdata[3].data[1] = datas[3];  vdata[3].data[2] = datas[3];  vdata[3].data[3] = datas[3];  vdata[3].data[4] = datas[3];  vdata[3].data[5] = datas[3];  vdata[3].data[6] = datas[3];  vdata[3].data[7] = datas[3];  vdata[3].data[8] = datas[3];  vdata[3].data[9] = datas[3];  vdata[3].data[10] = datas[3];  vdata[3].data[11] = datas[3];  vdata[3].data[12] = datas[3];  vdata[3].data[13] = datas[3];  vdata[3].data[14] = datas[3];  vdata[3].data[15] = datas[3];    vdata[4].data[0] = datas[4];  vdata[4].data[1] = datas[4];  vdata[4].data[2] = datas[4];  vdata[4].data[3] = datas[4];  vdata[4].data[4] = datas[4];  vdata[4].data[5] = datas[4];  vdata[4].data[6] = datas[4];  vdata[4].data[7] = datas[4];  vdata[4].data[8] = datas[4];  vdata[4].data[9] = datas[4];  vdata[4].data[10] = datas[4];  vdata[4].data[11] = datas[4];  vdata[4].data[12] = datas[4];  vdata[4].data[13] = datas[4];  vdata[4].data[14] = datas[4];  vdata[4].data[15] = datas[4];    vdata[5].data[0] = datas[5];  vdata[5].data[1] = datas[5];  vdata[5].data[2] = datas[5];  vdata[5].data[3] = datas[5];  vdata[5].data[4] = datas[5];  vdata[5].data[5] = datas[5];  vdata[5].data[6] = datas[5];  vdata[5].data[7] = datas[5];  vdata[5].data[8] = datas[5];  vdata[5].data[9] = datas[5];  vdata[5].data[10] = datas[5];  vdata[5].data[11] = datas[5];  vdata[5].data[12] = datas[5];  vdata[5].data[13] = datas[5];  vdata[5].data[14] = datas[5];  vdata[5].data[15] = datas[5];    vdata[6].data[0] = datas[6];  vdata[6].data[1] = datas[6];  vdata[6].data[2] = datas[6];  vdata[6].data[3] = datas[6];  vdata[6].data[4] = datas[6];  vdata[6].data[5] = datas[6];  vdata[6].data[6] = datas[6];  vdata[6].data[7] = datas[6];  vdata[6].data[8] = datas[6];  vdata[6].data[9] = datas[6];  vdata[6].data[10] = datas[6];  vdata[6].data[11] = datas[6];  vdata[6].data[12] = datas[6];  vdata[6].data[13] = datas[6];  vdata[6].data[14] = datas[6];  vdata[6].data[15] = datas[6];    vdata[7].data[0] = datas[7];  vdata[7].data[1] = datas[7];  vdata[7].data[2] = datas[7];  vdata[7].data[3] = datas[7];  vdata[7].data[4] = datas[7];  vdata[7].data[5] = datas[7];  vdata[7].data[6] = datas[7];  vdata[7].data[7] = datas[7];  vdata[7].data[8] = datas[7];  vdata[7].data[9] = datas[7];  vdata[7].data[10] = datas[7];  vdata[7].data[11] = datas[7];  vdata[7].data[12] = datas[7];  vdata[7].data[13] = datas[7];  vdata[7].data[14] = datas[7];  vdata[7].data[15] = datas[7];    vdata[8].data[0] = datas[8];  vdata[8].data[1] = datas[8];  vdata[8].data[2] = datas[8];  vdata[8].data[3] = datas[8];  vdata[8].data[4] = datas[8];  vdata[8].data[5] = datas[8];  vdata[8].data[6] = datas[8];  vdata[8].data[7] = datas[8];  vdata[8].data[8] = datas[8];  vdata[8].data[9] = datas[8];  vdata[8].data[10] = datas[8];  vdata[8].data[11] = datas[8];  vdata[8].data[12] = datas[8];  vdata[8].data[13] = datas[8];  vdata[8].data[14] = datas[8];  vdata[8].data[15] = datas[8];    vdata[9].data[0] = datas[9];  vdata[9].data[1] = datas[9];  vdata[9].data[2] = datas[9];  vdata[9].data[3] = datas[9];  vdata[9].data[4] = datas[9];  vdata[9].data[5] = datas[9];  vdata[9].data[6] = datas[9];  vdata[9].data[7] = datas[9];  vdata[9].data[8] = datas[9];  vdata[9].data[9] = datas[9];  vdata[9].data[10] = datas[9];  vdata[9].data[11] = datas[9];  vdata[9].data[12] = datas[9];  vdata[9].data[13] = datas[9];  vdata[9].data[14] = datas[9];  vdata[9].data[15] = datas[9];    vdata[10].data[0] = datas[10];  vdata[10].data[1] = datas[10];  vdata[10].data[2] = datas[10];  vdata[10].data[3] = datas[10];  vdata[10].data[4] = datas[10];  vdata[10].data[5] = datas[10];  vdata[10].data[6] = datas[10];  vdata[10].data[7] = datas[10];  vdata[10].data[8] = datas[10];  vdata[10].data[9] = datas[10];  vdata[10].data[10] = datas[10];  vdata[10].data[11] = datas[10];  vdata[10].data[12] = datas[10];  vdata[10].data[13] = datas[10];  vdata[10].data[14] = datas[10];  vdata[10].data[15] = datas[10];    vdata[11].data[0] = datas[11];  vdata[11].data[1] = datas[11];  vdata[11].data[2] = datas[11];  vdata[11].data[3] = datas[11];  vdata[11].data[4] = datas[11];  vdata[11].data[5] = datas[11];  vdata[11].data[6] = datas[11];  vdata[11].data[7] = datas[11];  vdata[11].data[8] = datas[11];  vdata[11].data[9] = datas[11];  vdata[11].data[10] = datas[11];  vdata[11].data[11] = datas[11];  vdata[11].data[12] = datas[11];  vdata[11].data[13] = datas[11];  vdata[11].data[14] = datas[11];  vdata[11].data[15] = datas[11];    vdata[12].data[0] = datas[12];  vdata[12].data[1] = datas[12];  vdata[12].data[2] = datas[12];  vdata[12].data[3] = datas[12];  vdata[12].data[4] = datas[12];  vdata[12].data[5] = datas[12];  vdata[12].data[6] = datas[12];  vdata[12].data[7] = datas[12];  vdata[12].data[8] = datas[12];  vdata[12].data[9] = datas[12];  vdata[12].data[10] = datas[12];  vdata[12].data[11] = datas[12];  vdata[12].data[12] = datas[12];  vdata[12].data[13] = datas[12];  vdata[12].data[14] = datas[12];  vdata[12].data[15] = datas[12];    vdata[13].data[0] = datas[13];  vdata[13].data[1] = datas[13];  vdata[13].data[2] = datas[13];  vdata[13].data[3] = datas[13];  vdata[13].data[4] = datas[13];  vdata[13].data[5] = datas[13];  vdata[13].data[6] = datas[13];  vdata[13].data[7] = datas[13];  vdata[13].data[8] = datas[13];  vdata[13].data[9] = datas[13];  vdata[13].data[10] = datas[13];  vdata[13].data[11] = datas[13];  vdata[13].data[12] = datas[13];  vdata[13].data[13] = datas[13];  vdata[13].data[14] = datas[13];  vdata[13].data[15] = datas[13];    vdata[14].data[0] = datas[14];  vdata[14].data[1] = datas[14];  vdata[14].data[2] = datas[14];  vdata[14].data[3] = datas[14];  vdata[14].data[4] = datas[14];  vdata[14].data[5] = datas[14];  vdata[14].data[6] = datas[14];  vdata[14].data[7] = datas[14];  vdata[14].data[8] = datas[14];  vdata[14].data[9] = datas[14];  vdata[14].data[10] = datas[14];  vdata[14].data[11] = datas[14];  vdata[14].data[12] = datas[14];  vdata[14].data[13] = datas[14];  vdata[14].data[14] = datas[14];  vdata[14].data[15] = datas[14];    vdata[15].data[0] = datas[15];  vdata[15].data[1] = datas[15];  vdata[15].data[2] = datas[15];  vdata[15].data[3] = datas[15];  vdata[15].data[4] = datas[15];  vdata[15].data[5] = datas[15];  vdata[15].data[6] = datas[15];  vdata[15].data[7] = datas[15];  vdata[15].data[8] = datas[15];  vdata[15].data[9] = datas[15];  vdata[15].data[10] = datas[15];  vdata[15].data[11] = datas[15];  vdata[15].data[12] = datas[15];  vdata[15].data[13] = datas[15];  vdata[15].data[14] = datas[15];  vdata[15].data[15] = datas[15];   // FIXME.
			#else 
 vdata[0].data = datas[0];  vdata[1].data = datas[1];  vdata[2].data = datas[2];  vdata[3].data = datas[3];  vdata[4].data = datas[4];  vdata[5].data = datas[5];  vdata[6].data = datas[6];  vdata[7].data = datas[7];  vdata[8].data = datas[8];  vdata[9].data = datas[9];  vdata[10].data = datas[10];  vdata[11].data = datas[11];  vdata[12].data = datas[12];  vdata[13].data = datas[13];  vdata[14].data = datas[14];  vdata[15].data = datas[15]; 	
			#endif
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(datas[v] == 1 || datas[v] < 1000){ cout<<"readANDRVchunks: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<" v: "<<v<<", voffset_kvs: "<<voffset_kvs<<", index + s: "<<index + s<<", datas["<<v<<"]: "<<datas[v]<<endl; }}
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP0_readANDRVchunks7 25", index + s, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, index, i);
			#endif
			vbuffer0[0][index + s] = vdata[0];
			vbuffer0[1][index + s] = vdata[1];
			vbuffer0[2][index + s] = vdata[2];
			vbuffer0[3][index + s] = vdata[3];
			vbuffer0[4][index + s] = vdata[4];
			vbuffer0[5][index + s] = vdata[5];
			vbuffer0[6][index + s] = vdata[6];
			vbuffer0[7][index + s] = vdata[7];
			vbuffer0[8][index + s] = vdata[8];
			vbuffer0[9][index + s] = vdata[9];
			vbuffer0[10][index + s] = vdata[10];
			vbuffer0[11][index + s] = vdata[11];
			vbuffer0[12][index + s] = vdata[12];
			vbuffer0[13][index + s] = vdata[13];
			vbuffer0[14][index + s] = vdata[14];
			vbuffer0[15][index + s] = vdata[15];
			vbuffer1[0][index + s] = vdata[0];
			vbuffer1[1][index + s] = vdata[1];
			vbuffer1[2][index + s] = vdata[2];
			vbuffer1[3][index + s] = vdata[3];
			vbuffer1[4][index + s] = vdata[4];
			vbuffer1[5][index + s] = vdata[5];
			vbuffer1[6][index + s] = vdata[6];
			vbuffer1[7][index + s] = vdata[7];
			vbuffer1[8][index + s] = vdata[8];
			vbuffer1[9][index + s] = vdata[9];
			vbuffer1[10][index + s] = vdata[10];
			vbuffer1[11][index + s] = vdata[11];
			vbuffer1[12][index + s] = vdata[12];
			vbuffer1[13][index + s] = vdata[13];
			vbuffer1[14][index + s] = vdata[14];
			vbuffer1[15][index + s] = vdata[15];
			vbuffer2[0][index + s] = vdata[0];
			vbuffer2[1][index + s] = vdata[1];
			vbuffer2[2][index + s] = vdata[2];
			vbuffer2[3][index + s] = vdata[3];
			vbuffer2[4][index + s] = vdata[4];
			vbuffer2[5][index + s] = vdata[5];
			vbuffer2[6][index + s] = vdata[6];
			vbuffer2[7][index + s] = vdata[7];
			vbuffer2[8][index + s] = vdata[8];
			vbuffer2[9][index + s] = vdata[9];
			vbuffer2[10][index + s] = vdata[10];
			vbuffer2[11][index + s] = vdata[11];
			vbuffer2[12][index + s] = vdata[12];
			vbuffer2[13][index + s] = vdata[13];
			vbuffer2[14][index + s] = vdata[14];
			vbuffer2[15][index + s] = vdata[15];
			vbuffer3[0][index + s] = vdata[0];
			vbuffer3[1][index + s] = vdata[1];
			vbuffer3[2][index + s] = vdata[2];
			vbuffer3[3][index + s] = vdata[3];
			vbuffer3[4][index + s] = vdata[4];
			vbuffer3[5][index + s] = vdata[5];
			vbuffer3[6][index + s] = vdata[6];
			vbuffer3[7][index + s] = vdata[7];
			vbuffer3[8][index + s] = vdata[8];
			vbuffer3[9][index + s] = vdata[9];
			vbuffer3[10][index + s] = vdata[10];
			vbuffer3[11][index + s] = vdata[11];
			vbuffer3[12][index + s] = vdata[12];
			vbuffer3[13][index + s] = vdata[13];
			vbuffer3[14][index + s] = vdata[14];
			vbuffer3[15][index + s] = vdata[15];
			vbuffer4[0][index + s] = vdata[0];
			vbuffer4[1][index + s] = vdata[1];
			vbuffer4[2][index + s] = vdata[2];
			vbuffer4[3][index + s] = vdata[3];
			vbuffer4[4][index + s] = vdata[4];
			vbuffer4[5][index + s] = vdata[5];
			vbuffer4[6][index + s] = vdata[6];
			vbuffer4[7][index + s] = vdata[7];
			vbuffer4[8][index + s] = vdata[8];
			vbuffer4[9][index + s] = vdata[9];
			vbuffer4[10][index + s] = vdata[10];
			vbuffer4[11][index + s] = vdata[11];
			vbuffer4[12][index + s] = vdata[12];
			vbuffer4[13][index + s] = vdata[13];
			vbuffer4[14][index + s] = vdata[14];
			vbuffer4[15][index + s] = vdata[15];
			vbuffer5[0][index + s] = vdata[0];
			vbuffer5[1][index + s] = vdata[1];
			vbuffer5[2][index + s] = vdata[2];
			vbuffer5[3][index + s] = vdata[3];
			vbuffer5[4][index + s] = vdata[4];
			vbuffer5[5][index + s] = vdata[5];
			vbuffer5[6][index + s] = vdata[6];
			vbuffer5[7][index + s] = vdata[7];
			vbuffer5[8][index + s] = vdata[8];
			vbuffer5[9][index + s] = vdata[9];
			vbuffer5[10][index + s] = vdata[10];
			vbuffer5[11][index + s] = vdata[11];
			vbuffer5[12][index + s] = vdata[12];
			vbuffer5[13][index + s] = vdata[13];
			vbuffer5[14][index + s] = vdata[14];
			vbuffer5[15][index + s] = vdata[15];
			vbuffer6[0][index + s] = vdata[0];
			vbuffer6[1][index + s] = vdata[1];
			vbuffer6[2][index + s] = vdata[2];
			vbuffer6[3][index + s] = vdata[3];
			vbuffer6[4][index + s] = vdata[4];
			vbuffer6[5][index + s] = vdata[5];
			vbuffer6[6][index + s] = vdata[6];
			vbuffer6[7][index + s] = vdata[7];
			vbuffer6[8][index + s] = vdata[8];
			vbuffer6[9][index + s] = vdata[9];
			vbuffer6[10][index + s] = vdata[10];
			vbuffer6[11][index + s] = vdata[11];
			vbuffer6[12][index + s] = vdata[12];
			vbuffer6[13][index + s] = vdata[13];
			vbuffer6[14][index + s] = vdata[14];
			vbuffer6[15][index + s] = vdata[15];
			index += NUM_PEs;

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
	}
	// exit(EXIT_SUCCESS); /////////
	return;
}
void acts_all::MEMACCESSP0_readANDRVchunks8(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	// return;//
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int limit = globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE;
	
	READANDRVCHUNKS_LOOP2: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): vsz_kvs: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		unsigned int index = 0;
		// unsigned int index = s;
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			// unsigned int offset_kvs; if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			unsigned int offset_kvs = voffset_kvs + depth_i + i;
			UTILP0_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, datas);	
			
			#ifdef BIT_TRAVERSAL_ALGORITHM
  vdata[0].data[0] = datas[0];  vdata[0].data[1] = datas[0];  vdata[0].data[2] = datas[0];  vdata[0].data[3] = datas[0];  vdata[0].data[4] = datas[0];  vdata[0].data[5] = datas[0];  vdata[0].data[6] = datas[0];  vdata[0].data[7] = datas[0];  vdata[0].data[8] = datas[0];  vdata[0].data[9] = datas[0];  vdata[0].data[10] = datas[0];  vdata[0].data[11] = datas[0];  vdata[0].data[12] = datas[0];  vdata[0].data[13] = datas[0];  vdata[0].data[14] = datas[0];  vdata[0].data[15] = datas[0];    vdata[1].data[0] = datas[1];  vdata[1].data[1] = datas[1];  vdata[1].data[2] = datas[1];  vdata[1].data[3] = datas[1];  vdata[1].data[4] = datas[1];  vdata[1].data[5] = datas[1];  vdata[1].data[6] = datas[1];  vdata[1].data[7] = datas[1];  vdata[1].data[8] = datas[1];  vdata[1].data[9] = datas[1];  vdata[1].data[10] = datas[1];  vdata[1].data[11] = datas[1];  vdata[1].data[12] = datas[1];  vdata[1].data[13] = datas[1];  vdata[1].data[14] = datas[1];  vdata[1].data[15] = datas[1];    vdata[2].data[0] = datas[2];  vdata[2].data[1] = datas[2];  vdata[2].data[2] = datas[2];  vdata[2].data[3] = datas[2];  vdata[2].data[4] = datas[2];  vdata[2].data[5] = datas[2];  vdata[2].data[6] = datas[2];  vdata[2].data[7] = datas[2];  vdata[2].data[8] = datas[2];  vdata[2].data[9] = datas[2];  vdata[2].data[10] = datas[2];  vdata[2].data[11] = datas[2];  vdata[2].data[12] = datas[2];  vdata[2].data[13] = datas[2];  vdata[2].data[14] = datas[2];  vdata[2].data[15] = datas[2];    vdata[3].data[0] = datas[3];  vdata[3].data[1] = datas[3];  vdata[3].data[2] = datas[3];  vdata[3].data[3] = datas[3];  vdata[3].data[4] = datas[3];  vdata[3].data[5] = datas[3];  vdata[3].data[6] = datas[3];  vdata[3].data[7] = datas[3];  vdata[3].data[8] = datas[3];  vdata[3].data[9] = datas[3];  vdata[3].data[10] = datas[3];  vdata[3].data[11] = datas[3];  vdata[3].data[12] = datas[3];  vdata[3].data[13] = datas[3];  vdata[3].data[14] = datas[3];  vdata[3].data[15] = datas[3];    vdata[4].data[0] = datas[4];  vdata[4].data[1] = datas[4];  vdata[4].data[2] = datas[4];  vdata[4].data[3] = datas[4];  vdata[4].data[4] = datas[4];  vdata[4].data[5] = datas[4];  vdata[4].data[6] = datas[4];  vdata[4].data[7] = datas[4];  vdata[4].data[8] = datas[4];  vdata[4].data[9] = datas[4];  vdata[4].data[10] = datas[4];  vdata[4].data[11] = datas[4];  vdata[4].data[12] = datas[4];  vdata[4].data[13] = datas[4];  vdata[4].data[14] = datas[4];  vdata[4].data[15] = datas[4];    vdata[5].data[0] = datas[5];  vdata[5].data[1] = datas[5];  vdata[5].data[2] = datas[5];  vdata[5].data[3] = datas[5];  vdata[5].data[4] = datas[5];  vdata[5].data[5] = datas[5];  vdata[5].data[6] = datas[5];  vdata[5].data[7] = datas[5];  vdata[5].data[8] = datas[5];  vdata[5].data[9] = datas[5];  vdata[5].data[10] = datas[5];  vdata[5].data[11] = datas[5];  vdata[5].data[12] = datas[5];  vdata[5].data[13] = datas[5];  vdata[5].data[14] = datas[5];  vdata[5].data[15] = datas[5];    vdata[6].data[0] = datas[6];  vdata[6].data[1] = datas[6];  vdata[6].data[2] = datas[6];  vdata[6].data[3] = datas[6];  vdata[6].data[4] = datas[6];  vdata[6].data[5] = datas[6];  vdata[6].data[6] = datas[6];  vdata[6].data[7] = datas[6];  vdata[6].data[8] = datas[6];  vdata[6].data[9] = datas[6];  vdata[6].data[10] = datas[6];  vdata[6].data[11] = datas[6];  vdata[6].data[12] = datas[6];  vdata[6].data[13] = datas[6];  vdata[6].data[14] = datas[6];  vdata[6].data[15] = datas[6];    vdata[7].data[0] = datas[7];  vdata[7].data[1] = datas[7];  vdata[7].data[2] = datas[7];  vdata[7].data[3] = datas[7];  vdata[7].data[4] = datas[7];  vdata[7].data[5] = datas[7];  vdata[7].data[6] = datas[7];  vdata[7].data[7] = datas[7];  vdata[7].data[8] = datas[7];  vdata[7].data[9] = datas[7];  vdata[7].data[10] = datas[7];  vdata[7].data[11] = datas[7];  vdata[7].data[12] = datas[7];  vdata[7].data[13] = datas[7];  vdata[7].data[14] = datas[7];  vdata[7].data[15] = datas[7];    vdata[8].data[0] = datas[8];  vdata[8].data[1] = datas[8];  vdata[8].data[2] = datas[8];  vdata[8].data[3] = datas[8];  vdata[8].data[4] = datas[8];  vdata[8].data[5] = datas[8];  vdata[8].data[6] = datas[8];  vdata[8].data[7] = datas[8];  vdata[8].data[8] = datas[8];  vdata[8].data[9] = datas[8];  vdata[8].data[10] = datas[8];  vdata[8].data[11] = datas[8];  vdata[8].data[12] = datas[8];  vdata[8].data[13] = datas[8];  vdata[8].data[14] = datas[8];  vdata[8].data[15] = datas[8];    vdata[9].data[0] = datas[9];  vdata[9].data[1] = datas[9];  vdata[9].data[2] = datas[9];  vdata[9].data[3] = datas[9];  vdata[9].data[4] = datas[9];  vdata[9].data[5] = datas[9];  vdata[9].data[6] = datas[9];  vdata[9].data[7] = datas[9];  vdata[9].data[8] = datas[9];  vdata[9].data[9] = datas[9];  vdata[9].data[10] = datas[9];  vdata[9].data[11] = datas[9];  vdata[9].data[12] = datas[9];  vdata[9].data[13] = datas[9];  vdata[9].data[14] = datas[9];  vdata[9].data[15] = datas[9];    vdata[10].data[0] = datas[10];  vdata[10].data[1] = datas[10];  vdata[10].data[2] = datas[10];  vdata[10].data[3] = datas[10];  vdata[10].data[4] = datas[10];  vdata[10].data[5] = datas[10];  vdata[10].data[6] = datas[10];  vdata[10].data[7] = datas[10];  vdata[10].data[8] = datas[10];  vdata[10].data[9] = datas[10];  vdata[10].data[10] = datas[10];  vdata[10].data[11] = datas[10];  vdata[10].data[12] = datas[10];  vdata[10].data[13] = datas[10];  vdata[10].data[14] = datas[10];  vdata[10].data[15] = datas[10];    vdata[11].data[0] = datas[11];  vdata[11].data[1] = datas[11];  vdata[11].data[2] = datas[11];  vdata[11].data[3] = datas[11];  vdata[11].data[4] = datas[11];  vdata[11].data[5] = datas[11];  vdata[11].data[6] = datas[11];  vdata[11].data[7] = datas[11];  vdata[11].data[8] = datas[11];  vdata[11].data[9] = datas[11];  vdata[11].data[10] = datas[11];  vdata[11].data[11] = datas[11];  vdata[11].data[12] = datas[11];  vdata[11].data[13] = datas[11];  vdata[11].data[14] = datas[11];  vdata[11].data[15] = datas[11];    vdata[12].data[0] = datas[12];  vdata[12].data[1] = datas[12];  vdata[12].data[2] = datas[12];  vdata[12].data[3] = datas[12];  vdata[12].data[4] = datas[12];  vdata[12].data[5] = datas[12];  vdata[12].data[6] = datas[12];  vdata[12].data[7] = datas[12];  vdata[12].data[8] = datas[12];  vdata[12].data[9] = datas[12];  vdata[12].data[10] = datas[12];  vdata[12].data[11] = datas[12];  vdata[12].data[12] = datas[12];  vdata[12].data[13] = datas[12];  vdata[12].data[14] = datas[12];  vdata[12].data[15] = datas[12];    vdata[13].data[0] = datas[13];  vdata[13].data[1] = datas[13];  vdata[13].data[2] = datas[13];  vdata[13].data[3] = datas[13];  vdata[13].data[4] = datas[13];  vdata[13].data[5] = datas[13];  vdata[13].data[6] = datas[13];  vdata[13].data[7] = datas[13];  vdata[13].data[8] = datas[13];  vdata[13].data[9] = datas[13];  vdata[13].data[10] = datas[13];  vdata[13].data[11] = datas[13];  vdata[13].data[12] = datas[13];  vdata[13].data[13] = datas[13];  vdata[13].data[14] = datas[13];  vdata[13].data[15] = datas[13];    vdata[14].data[0] = datas[14];  vdata[14].data[1] = datas[14];  vdata[14].data[2] = datas[14];  vdata[14].data[3] = datas[14];  vdata[14].data[4] = datas[14];  vdata[14].data[5] = datas[14];  vdata[14].data[6] = datas[14];  vdata[14].data[7] = datas[14];  vdata[14].data[8] = datas[14];  vdata[14].data[9] = datas[14];  vdata[14].data[10] = datas[14];  vdata[14].data[11] = datas[14];  vdata[14].data[12] = datas[14];  vdata[14].data[13] = datas[14];  vdata[14].data[14] = datas[14];  vdata[14].data[15] = datas[14];    vdata[15].data[0] = datas[15];  vdata[15].data[1] = datas[15];  vdata[15].data[2] = datas[15];  vdata[15].data[3] = datas[15];  vdata[15].data[4] = datas[15];  vdata[15].data[5] = datas[15];  vdata[15].data[6] = datas[15];  vdata[15].data[7] = datas[15];  vdata[15].data[8] = datas[15];  vdata[15].data[9] = datas[15];  vdata[15].data[10] = datas[15];  vdata[15].data[11] = datas[15];  vdata[15].data[12] = datas[15];  vdata[15].data[13] = datas[15];  vdata[15].data[14] = datas[15];  vdata[15].data[15] = datas[15];   // FIXME.
			#else 
 vdata[0].data = datas[0];  vdata[1].data = datas[1];  vdata[2].data = datas[2];  vdata[3].data = datas[3];  vdata[4].data = datas[4];  vdata[5].data = datas[5];  vdata[6].data = datas[6];  vdata[7].data = datas[7];  vdata[8].data = datas[8];  vdata[9].data = datas[9];  vdata[10].data = datas[10];  vdata[11].data = datas[11];  vdata[12].data = datas[12];  vdata[13].data = datas[13];  vdata[14].data = datas[14];  vdata[15].data = datas[15]; 	
			#endif
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(datas[v] == 1 || datas[v] < 1000){ cout<<"readANDRVchunks: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<" v: "<<v<<", voffset_kvs: "<<voffset_kvs<<", index + s: "<<index + s<<", datas["<<v<<"]: "<<datas[v]<<endl; }}
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP0_readANDRVchunks8 25", index + s, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, index, i);
			#endif
			vbuffer0[0][index + s] = vdata[0];
			vbuffer0[1][index + s] = vdata[1];
			vbuffer0[2][index + s] = vdata[2];
			vbuffer0[3][index + s] = vdata[3];
			vbuffer0[4][index + s] = vdata[4];
			vbuffer0[5][index + s] = vdata[5];
			vbuffer0[6][index + s] = vdata[6];
			vbuffer0[7][index + s] = vdata[7];
			vbuffer0[8][index + s] = vdata[8];
			vbuffer0[9][index + s] = vdata[9];
			vbuffer0[10][index + s] = vdata[10];
			vbuffer0[11][index + s] = vdata[11];
			vbuffer0[12][index + s] = vdata[12];
			vbuffer0[13][index + s] = vdata[13];
			vbuffer0[14][index + s] = vdata[14];
			vbuffer0[15][index + s] = vdata[15];
			vbuffer1[0][index + s] = vdata[0];
			vbuffer1[1][index + s] = vdata[1];
			vbuffer1[2][index + s] = vdata[2];
			vbuffer1[3][index + s] = vdata[3];
			vbuffer1[4][index + s] = vdata[4];
			vbuffer1[5][index + s] = vdata[5];
			vbuffer1[6][index + s] = vdata[6];
			vbuffer1[7][index + s] = vdata[7];
			vbuffer1[8][index + s] = vdata[8];
			vbuffer1[9][index + s] = vdata[9];
			vbuffer1[10][index + s] = vdata[10];
			vbuffer1[11][index + s] = vdata[11];
			vbuffer1[12][index + s] = vdata[12];
			vbuffer1[13][index + s] = vdata[13];
			vbuffer1[14][index + s] = vdata[14];
			vbuffer1[15][index + s] = vdata[15];
			vbuffer2[0][index + s] = vdata[0];
			vbuffer2[1][index + s] = vdata[1];
			vbuffer2[2][index + s] = vdata[2];
			vbuffer2[3][index + s] = vdata[3];
			vbuffer2[4][index + s] = vdata[4];
			vbuffer2[5][index + s] = vdata[5];
			vbuffer2[6][index + s] = vdata[6];
			vbuffer2[7][index + s] = vdata[7];
			vbuffer2[8][index + s] = vdata[8];
			vbuffer2[9][index + s] = vdata[9];
			vbuffer2[10][index + s] = vdata[10];
			vbuffer2[11][index + s] = vdata[11];
			vbuffer2[12][index + s] = vdata[12];
			vbuffer2[13][index + s] = vdata[13];
			vbuffer2[14][index + s] = vdata[14];
			vbuffer2[15][index + s] = vdata[15];
			vbuffer3[0][index + s] = vdata[0];
			vbuffer3[1][index + s] = vdata[1];
			vbuffer3[2][index + s] = vdata[2];
			vbuffer3[3][index + s] = vdata[3];
			vbuffer3[4][index + s] = vdata[4];
			vbuffer3[5][index + s] = vdata[5];
			vbuffer3[6][index + s] = vdata[6];
			vbuffer3[7][index + s] = vdata[7];
			vbuffer3[8][index + s] = vdata[8];
			vbuffer3[9][index + s] = vdata[9];
			vbuffer3[10][index + s] = vdata[10];
			vbuffer3[11][index + s] = vdata[11];
			vbuffer3[12][index + s] = vdata[12];
			vbuffer3[13][index + s] = vdata[13];
			vbuffer3[14][index + s] = vdata[14];
			vbuffer3[15][index + s] = vdata[15];
			vbuffer4[0][index + s] = vdata[0];
			vbuffer4[1][index + s] = vdata[1];
			vbuffer4[2][index + s] = vdata[2];
			vbuffer4[3][index + s] = vdata[3];
			vbuffer4[4][index + s] = vdata[4];
			vbuffer4[5][index + s] = vdata[5];
			vbuffer4[6][index + s] = vdata[6];
			vbuffer4[7][index + s] = vdata[7];
			vbuffer4[8][index + s] = vdata[8];
			vbuffer4[9][index + s] = vdata[9];
			vbuffer4[10][index + s] = vdata[10];
			vbuffer4[11][index + s] = vdata[11];
			vbuffer4[12][index + s] = vdata[12];
			vbuffer4[13][index + s] = vdata[13];
			vbuffer4[14][index + s] = vdata[14];
			vbuffer4[15][index + s] = vdata[15];
			vbuffer5[0][index + s] = vdata[0];
			vbuffer5[1][index + s] = vdata[1];
			vbuffer5[2][index + s] = vdata[2];
			vbuffer5[3][index + s] = vdata[3];
			vbuffer5[4][index + s] = vdata[4];
			vbuffer5[5][index + s] = vdata[5];
			vbuffer5[6][index + s] = vdata[6];
			vbuffer5[7][index + s] = vdata[7];
			vbuffer5[8][index + s] = vdata[8];
			vbuffer5[9][index + s] = vdata[9];
			vbuffer5[10][index + s] = vdata[10];
			vbuffer5[11][index + s] = vdata[11];
			vbuffer5[12][index + s] = vdata[12];
			vbuffer5[13][index + s] = vdata[13];
			vbuffer5[14][index + s] = vdata[14];
			vbuffer5[15][index + s] = vdata[15];
			vbuffer6[0][index + s] = vdata[0];
			vbuffer6[1][index + s] = vdata[1];
			vbuffer6[2][index + s] = vdata[2];
			vbuffer6[3][index + s] = vdata[3];
			vbuffer6[4][index + s] = vdata[4];
			vbuffer6[5][index + s] = vdata[5];
			vbuffer6[6][index + s] = vdata[6];
			vbuffer6[7][index + s] = vdata[7];
			vbuffer6[8][index + s] = vdata[8];
			vbuffer6[9][index + s] = vdata[9];
			vbuffer6[10][index + s] = vdata[10];
			vbuffer6[11][index + s] = vdata[11];
			vbuffer6[12][index + s] = vdata[12];
			vbuffer6[13][index + s] = vdata[13];
			vbuffer6[14][index + s] = vdata[14];
			vbuffer6[15][index + s] = vdata[15];
			vbuffer7[0][index + s] = vdata[0];
			vbuffer7[1][index + s] = vdata[1];
			vbuffer7[2][index + s] = vdata[2];
			vbuffer7[3][index + s] = vdata[3];
			vbuffer7[4][index + s] = vdata[4];
			vbuffer7[5][index + s] = vdata[5];
			vbuffer7[6][index + s] = vdata[6];
			vbuffer7[7][index + s] = vdata[7];
			vbuffer7[8][index + s] = vdata[8];
			vbuffer7[9][index + s] = vdata[9];
			vbuffer7[10][index + s] = vdata[10];
			vbuffer7[11][index + s] = vdata[11];
			vbuffer7[12][index + s] = vdata[12];
			vbuffer7[13][index + s] = vdata[13];
			vbuffer7[14][index + s] = vdata[14];
			vbuffer7[15][index + s] = vdata[15];
			index += NUM_PEs;

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
	}
	// exit(EXIT_SUCCESS); /////////
	return;
}
void acts_all::MEMACCESSP0_readANDRVchunks9(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	// return;//
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int limit = globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE;
	
	READANDRVCHUNKS_LOOP2: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): vsz_kvs: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		unsigned int index = 0;
		// unsigned int index = s;
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			// unsigned int offset_kvs; if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			unsigned int offset_kvs = voffset_kvs + depth_i + i;
			UTILP0_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, datas);	
			
			#ifdef BIT_TRAVERSAL_ALGORITHM
  vdata[0].data[0] = datas[0];  vdata[0].data[1] = datas[0];  vdata[0].data[2] = datas[0];  vdata[0].data[3] = datas[0];  vdata[0].data[4] = datas[0];  vdata[0].data[5] = datas[0];  vdata[0].data[6] = datas[0];  vdata[0].data[7] = datas[0];  vdata[0].data[8] = datas[0];  vdata[0].data[9] = datas[0];  vdata[0].data[10] = datas[0];  vdata[0].data[11] = datas[0];  vdata[0].data[12] = datas[0];  vdata[0].data[13] = datas[0];  vdata[0].data[14] = datas[0];  vdata[0].data[15] = datas[0];    vdata[1].data[0] = datas[1];  vdata[1].data[1] = datas[1];  vdata[1].data[2] = datas[1];  vdata[1].data[3] = datas[1];  vdata[1].data[4] = datas[1];  vdata[1].data[5] = datas[1];  vdata[1].data[6] = datas[1];  vdata[1].data[7] = datas[1];  vdata[1].data[8] = datas[1];  vdata[1].data[9] = datas[1];  vdata[1].data[10] = datas[1];  vdata[1].data[11] = datas[1];  vdata[1].data[12] = datas[1];  vdata[1].data[13] = datas[1];  vdata[1].data[14] = datas[1];  vdata[1].data[15] = datas[1];    vdata[2].data[0] = datas[2];  vdata[2].data[1] = datas[2];  vdata[2].data[2] = datas[2];  vdata[2].data[3] = datas[2];  vdata[2].data[4] = datas[2];  vdata[2].data[5] = datas[2];  vdata[2].data[6] = datas[2];  vdata[2].data[7] = datas[2];  vdata[2].data[8] = datas[2];  vdata[2].data[9] = datas[2];  vdata[2].data[10] = datas[2];  vdata[2].data[11] = datas[2];  vdata[2].data[12] = datas[2];  vdata[2].data[13] = datas[2];  vdata[2].data[14] = datas[2];  vdata[2].data[15] = datas[2];    vdata[3].data[0] = datas[3];  vdata[3].data[1] = datas[3];  vdata[3].data[2] = datas[3];  vdata[3].data[3] = datas[3];  vdata[3].data[4] = datas[3];  vdata[3].data[5] = datas[3];  vdata[3].data[6] = datas[3];  vdata[3].data[7] = datas[3];  vdata[3].data[8] = datas[3];  vdata[3].data[9] = datas[3];  vdata[3].data[10] = datas[3];  vdata[3].data[11] = datas[3];  vdata[3].data[12] = datas[3];  vdata[3].data[13] = datas[3];  vdata[3].data[14] = datas[3];  vdata[3].data[15] = datas[3];    vdata[4].data[0] = datas[4];  vdata[4].data[1] = datas[4];  vdata[4].data[2] = datas[4];  vdata[4].data[3] = datas[4];  vdata[4].data[4] = datas[4];  vdata[4].data[5] = datas[4];  vdata[4].data[6] = datas[4];  vdata[4].data[7] = datas[4];  vdata[4].data[8] = datas[4];  vdata[4].data[9] = datas[4];  vdata[4].data[10] = datas[4];  vdata[4].data[11] = datas[4];  vdata[4].data[12] = datas[4];  vdata[4].data[13] = datas[4];  vdata[4].data[14] = datas[4];  vdata[4].data[15] = datas[4];    vdata[5].data[0] = datas[5];  vdata[5].data[1] = datas[5];  vdata[5].data[2] = datas[5];  vdata[5].data[3] = datas[5];  vdata[5].data[4] = datas[5];  vdata[5].data[5] = datas[5];  vdata[5].data[6] = datas[5];  vdata[5].data[7] = datas[5];  vdata[5].data[8] = datas[5];  vdata[5].data[9] = datas[5];  vdata[5].data[10] = datas[5];  vdata[5].data[11] = datas[5];  vdata[5].data[12] = datas[5];  vdata[5].data[13] = datas[5];  vdata[5].data[14] = datas[5];  vdata[5].data[15] = datas[5];    vdata[6].data[0] = datas[6];  vdata[6].data[1] = datas[6];  vdata[6].data[2] = datas[6];  vdata[6].data[3] = datas[6];  vdata[6].data[4] = datas[6];  vdata[6].data[5] = datas[6];  vdata[6].data[6] = datas[6];  vdata[6].data[7] = datas[6];  vdata[6].data[8] = datas[6];  vdata[6].data[9] = datas[6];  vdata[6].data[10] = datas[6];  vdata[6].data[11] = datas[6];  vdata[6].data[12] = datas[6];  vdata[6].data[13] = datas[6];  vdata[6].data[14] = datas[6];  vdata[6].data[15] = datas[6];    vdata[7].data[0] = datas[7];  vdata[7].data[1] = datas[7];  vdata[7].data[2] = datas[7];  vdata[7].data[3] = datas[7];  vdata[7].data[4] = datas[7];  vdata[7].data[5] = datas[7];  vdata[7].data[6] = datas[7];  vdata[7].data[7] = datas[7];  vdata[7].data[8] = datas[7];  vdata[7].data[9] = datas[7];  vdata[7].data[10] = datas[7];  vdata[7].data[11] = datas[7];  vdata[7].data[12] = datas[7];  vdata[7].data[13] = datas[7];  vdata[7].data[14] = datas[7];  vdata[7].data[15] = datas[7];    vdata[8].data[0] = datas[8];  vdata[8].data[1] = datas[8];  vdata[8].data[2] = datas[8];  vdata[8].data[3] = datas[8];  vdata[8].data[4] = datas[8];  vdata[8].data[5] = datas[8];  vdata[8].data[6] = datas[8];  vdata[8].data[7] = datas[8];  vdata[8].data[8] = datas[8];  vdata[8].data[9] = datas[8];  vdata[8].data[10] = datas[8];  vdata[8].data[11] = datas[8];  vdata[8].data[12] = datas[8];  vdata[8].data[13] = datas[8];  vdata[8].data[14] = datas[8];  vdata[8].data[15] = datas[8];    vdata[9].data[0] = datas[9];  vdata[9].data[1] = datas[9];  vdata[9].data[2] = datas[9];  vdata[9].data[3] = datas[9];  vdata[9].data[4] = datas[9];  vdata[9].data[5] = datas[9];  vdata[9].data[6] = datas[9];  vdata[9].data[7] = datas[9];  vdata[9].data[8] = datas[9];  vdata[9].data[9] = datas[9];  vdata[9].data[10] = datas[9];  vdata[9].data[11] = datas[9];  vdata[9].data[12] = datas[9];  vdata[9].data[13] = datas[9];  vdata[9].data[14] = datas[9];  vdata[9].data[15] = datas[9];    vdata[10].data[0] = datas[10];  vdata[10].data[1] = datas[10];  vdata[10].data[2] = datas[10];  vdata[10].data[3] = datas[10];  vdata[10].data[4] = datas[10];  vdata[10].data[5] = datas[10];  vdata[10].data[6] = datas[10];  vdata[10].data[7] = datas[10];  vdata[10].data[8] = datas[10];  vdata[10].data[9] = datas[10];  vdata[10].data[10] = datas[10];  vdata[10].data[11] = datas[10];  vdata[10].data[12] = datas[10];  vdata[10].data[13] = datas[10];  vdata[10].data[14] = datas[10];  vdata[10].data[15] = datas[10];    vdata[11].data[0] = datas[11];  vdata[11].data[1] = datas[11];  vdata[11].data[2] = datas[11];  vdata[11].data[3] = datas[11];  vdata[11].data[4] = datas[11];  vdata[11].data[5] = datas[11];  vdata[11].data[6] = datas[11];  vdata[11].data[7] = datas[11];  vdata[11].data[8] = datas[11];  vdata[11].data[9] = datas[11];  vdata[11].data[10] = datas[11];  vdata[11].data[11] = datas[11];  vdata[11].data[12] = datas[11];  vdata[11].data[13] = datas[11];  vdata[11].data[14] = datas[11];  vdata[11].data[15] = datas[11];    vdata[12].data[0] = datas[12];  vdata[12].data[1] = datas[12];  vdata[12].data[2] = datas[12];  vdata[12].data[3] = datas[12];  vdata[12].data[4] = datas[12];  vdata[12].data[5] = datas[12];  vdata[12].data[6] = datas[12];  vdata[12].data[7] = datas[12];  vdata[12].data[8] = datas[12];  vdata[12].data[9] = datas[12];  vdata[12].data[10] = datas[12];  vdata[12].data[11] = datas[12];  vdata[12].data[12] = datas[12];  vdata[12].data[13] = datas[12];  vdata[12].data[14] = datas[12];  vdata[12].data[15] = datas[12];    vdata[13].data[0] = datas[13];  vdata[13].data[1] = datas[13];  vdata[13].data[2] = datas[13];  vdata[13].data[3] = datas[13];  vdata[13].data[4] = datas[13];  vdata[13].data[5] = datas[13];  vdata[13].data[6] = datas[13];  vdata[13].data[7] = datas[13];  vdata[13].data[8] = datas[13];  vdata[13].data[9] = datas[13];  vdata[13].data[10] = datas[13];  vdata[13].data[11] = datas[13];  vdata[13].data[12] = datas[13];  vdata[13].data[13] = datas[13];  vdata[13].data[14] = datas[13];  vdata[13].data[15] = datas[13];    vdata[14].data[0] = datas[14];  vdata[14].data[1] = datas[14];  vdata[14].data[2] = datas[14];  vdata[14].data[3] = datas[14];  vdata[14].data[4] = datas[14];  vdata[14].data[5] = datas[14];  vdata[14].data[6] = datas[14];  vdata[14].data[7] = datas[14];  vdata[14].data[8] = datas[14];  vdata[14].data[9] = datas[14];  vdata[14].data[10] = datas[14];  vdata[14].data[11] = datas[14];  vdata[14].data[12] = datas[14];  vdata[14].data[13] = datas[14];  vdata[14].data[14] = datas[14];  vdata[14].data[15] = datas[14];    vdata[15].data[0] = datas[15];  vdata[15].data[1] = datas[15];  vdata[15].data[2] = datas[15];  vdata[15].data[3] = datas[15];  vdata[15].data[4] = datas[15];  vdata[15].data[5] = datas[15];  vdata[15].data[6] = datas[15];  vdata[15].data[7] = datas[15];  vdata[15].data[8] = datas[15];  vdata[15].data[9] = datas[15];  vdata[15].data[10] = datas[15];  vdata[15].data[11] = datas[15];  vdata[15].data[12] = datas[15];  vdata[15].data[13] = datas[15];  vdata[15].data[14] = datas[15];  vdata[15].data[15] = datas[15];   // FIXME.
			#else 
 vdata[0].data = datas[0];  vdata[1].data = datas[1];  vdata[2].data = datas[2];  vdata[3].data = datas[3];  vdata[4].data = datas[4];  vdata[5].data = datas[5];  vdata[6].data = datas[6];  vdata[7].data = datas[7];  vdata[8].data = datas[8];  vdata[9].data = datas[9];  vdata[10].data = datas[10];  vdata[11].data = datas[11];  vdata[12].data = datas[12];  vdata[13].data = datas[13];  vdata[14].data = datas[14];  vdata[15].data = datas[15]; 	
			#endif
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(datas[v] == 1 || datas[v] < 1000){ cout<<"readANDRVchunks: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<" v: "<<v<<", voffset_kvs: "<<voffset_kvs<<", index + s: "<<index + s<<", datas["<<v<<"]: "<<datas[v]<<endl; }}
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP0_readANDRVchunks9 25", index + s, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, index, i);
			#endif
			vbuffer0[0][index + s] = vdata[0];
			vbuffer0[1][index + s] = vdata[1];
			vbuffer0[2][index + s] = vdata[2];
			vbuffer0[3][index + s] = vdata[3];
			vbuffer0[4][index + s] = vdata[4];
			vbuffer0[5][index + s] = vdata[5];
			vbuffer0[6][index + s] = vdata[6];
			vbuffer0[7][index + s] = vdata[7];
			vbuffer0[8][index + s] = vdata[8];
			vbuffer0[9][index + s] = vdata[9];
			vbuffer0[10][index + s] = vdata[10];
			vbuffer0[11][index + s] = vdata[11];
			vbuffer0[12][index + s] = vdata[12];
			vbuffer0[13][index + s] = vdata[13];
			vbuffer0[14][index + s] = vdata[14];
			vbuffer0[15][index + s] = vdata[15];
			vbuffer1[0][index + s] = vdata[0];
			vbuffer1[1][index + s] = vdata[1];
			vbuffer1[2][index + s] = vdata[2];
			vbuffer1[3][index + s] = vdata[3];
			vbuffer1[4][index + s] = vdata[4];
			vbuffer1[5][index + s] = vdata[5];
			vbuffer1[6][index + s] = vdata[6];
			vbuffer1[7][index + s] = vdata[7];
			vbuffer1[8][index + s] = vdata[8];
			vbuffer1[9][index + s] = vdata[9];
			vbuffer1[10][index + s] = vdata[10];
			vbuffer1[11][index + s] = vdata[11];
			vbuffer1[12][index + s] = vdata[12];
			vbuffer1[13][index + s] = vdata[13];
			vbuffer1[14][index + s] = vdata[14];
			vbuffer1[15][index + s] = vdata[15];
			vbuffer2[0][index + s] = vdata[0];
			vbuffer2[1][index + s] = vdata[1];
			vbuffer2[2][index + s] = vdata[2];
			vbuffer2[3][index + s] = vdata[3];
			vbuffer2[4][index + s] = vdata[4];
			vbuffer2[5][index + s] = vdata[5];
			vbuffer2[6][index + s] = vdata[6];
			vbuffer2[7][index + s] = vdata[7];
			vbuffer2[8][index + s] = vdata[8];
			vbuffer2[9][index + s] = vdata[9];
			vbuffer2[10][index + s] = vdata[10];
			vbuffer2[11][index + s] = vdata[11];
			vbuffer2[12][index + s] = vdata[12];
			vbuffer2[13][index + s] = vdata[13];
			vbuffer2[14][index + s] = vdata[14];
			vbuffer2[15][index + s] = vdata[15];
			vbuffer3[0][index + s] = vdata[0];
			vbuffer3[1][index + s] = vdata[1];
			vbuffer3[2][index + s] = vdata[2];
			vbuffer3[3][index + s] = vdata[3];
			vbuffer3[4][index + s] = vdata[4];
			vbuffer3[5][index + s] = vdata[5];
			vbuffer3[6][index + s] = vdata[6];
			vbuffer3[7][index + s] = vdata[7];
			vbuffer3[8][index + s] = vdata[8];
			vbuffer3[9][index + s] = vdata[9];
			vbuffer3[10][index + s] = vdata[10];
			vbuffer3[11][index + s] = vdata[11];
			vbuffer3[12][index + s] = vdata[12];
			vbuffer3[13][index + s] = vdata[13];
			vbuffer3[14][index + s] = vdata[14];
			vbuffer3[15][index + s] = vdata[15];
			vbuffer4[0][index + s] = vdata[0];
			vbuffer4[1][index + s] = vdata[1];
			vbuffer4[2][index + s] = vdata[2];
			vbuffer4[3][index + s] = vdata[3];
			vbuffer4[4][index + s] = vdata[4];
			vbuffer4[5][index + s] = vdata[5];
			vbuffer4[6][index + s] = vdata[6];
			vbuffer4[7][index + s] = vdata[7];
			vbuffer4[8][index + s] = vdata[8];
			vbuffer4[9][index + s] = vdata[9];
			vbuffer4[10][index + s] = vdata[10];
			vbuffer4[11][index + s] = vdata[11];
			vbuffer4[12][index + s] = vdata[12];
			vbuffer4[13][index + s] = vdata[13];
			vbuffer4[14][index + s] = vdata[14];
			vbuffer4[15][index + s] = vdata[15];
			vbuffer5[0][index + s] = vdata[0];
			vbuffer5[1][index + s] = vdata[1];
			vbuffer5[2][index + s] = vdata[2];
			vbuffer5[3][index + s] = vdata[3];
			vbuffer5[4][index + s] = vdata[4];
			vbuffer5[5][index + s] = vdata[5];
			vbuffer5[6][index + s] = vdata[6];
			vbuffer5[7][index + s] = vdata[7];
			vbuffer5[8][index + s] = vdata[8];
			vbuffer5[9][index + s] = vdata[9];
			vbuffer5[10][index + s] = vdata[10];
			vbuffer5[11][index + s] = vdata[11];
			vbuffer5[12][index + s] = vdata[12];
			vbuffer5[13][index + s] = vdata[13];
			vbuffer5[14][index + s] = vdata[14];
			vbuffer5[15][index + s] = vdata[15];
			vbuffer6[0][index + s] = vdata[0];
			vbuffer6[1][index + s] = vdata[1];
			vbuffer6[2][index + s] = vdata[2];
			vbuffer6[3][index + s] = vdata[3];
			vbuffer6[4][index + s] = vdata[4];
			vbuffer6[5][index + s] = vdata[5];
			vbuffer6[6][index + s] = vdata[6];
			vbuffer6[7][index + s] = vdata[7];
			vbuffer6[8][index + s] = vdata[8];
			vbuffer6[9][index + s] = vdata[9];
			vbuffer6[10][index + s] = vdata[10];
			vbuffer6[11][index + s] = vdata[11];
			vbuffer6[12][index + s] = vdata[12];
			vbuffer6[13][index + s] = vdata[13];
			vbuffer6[14][index + s] = vdata[14];
			vbuffer6[15][index + s] = vdata[15];
			vbuffer7[0][index + s] = vdata[0];
			vbuffer7[1][index + s] = vdata[1];
			vbuffer7[2][index + s] = vdata[2];
			vbuffer7[3][index + s] = vdata[3];
			vbuffer7[4][index + s] = vdata[4];
			vbuffer7[5][index + s] = vdata[5];
			vbuffer7[6][index + s] = vdata[6];
			vbuffer7[7][index + s] = vdata[7];
			vbuffer7[8][index + s] = vdata[8];
			vbuffer7[9][index + s] = vdata[9];
			vbuffer7[10][index + s] = vdata[10];
			vbuffer7[11][index + s] = vdata[11];
			vbuffer7[12][index + s] = vdata[12];
			vbuffer7[13][index + s] = vdata[13];
			vbuffer7[14][index + s] = vdata[14];
			vbuffer7[15][index + s] = vdata[15];
			vbuffer8[0][index + s] = vdata[0];
			vbuffer8[1][index + s] = vdata[1];
			vbuffer8[2][index + s] = vdata[2];
			vbuffer8[3][index + s] = vdata[3];
			vbuffer8[4][index + s] = vdata[4];
			vbuffer8[5][index + s] = vdata[5];
			vbuffer8[6][index + s] = vdata[6];
			vbuffer8[7][index + s] = vdata[7];
			vbuffer8[8][index + s] = vdata[8];
			vbuffer8[9][index + s] = vdata[9];
			vbuffer8[10][index + s] = vdata[10];
			vbuffer8[11][index + s] = vdata[11];
			vbuffer8[12][index + s] = vdata[12];
			vbuffer8[13][index + s] = vdata[13];
			vbuffer8[14][index + s] = vdata[14];
			vbuffer8[15][index + s] = vdata[15];
			index += NUM_PEs;

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
	}
	// exit(EXIT_SUCCESS); /////////
	return;
}
void acts_all::MEMACCESSP0_readANDRVchunks10(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer9[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	// return;//
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int limit = globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE;
	
	READANDRVCHUNKS_LOOP2: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): vsz_kvs: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		unsigned int index = 0;
		// unsigned int index = s;
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			// unsigned int offset_kvs; if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			unsigned int offset_kvs = voffset_kvs + depth_i + i;
			UTILP0_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, datas);	
			
			#ifdef BIT_TRAVERSAL_ALGORITHM
  vdata[0].data[0] = datas[0];  vdata[0].data[1] = datas[0];  vdata[0].data[2] = datas[0];  vdata[0].data[3] = datas[0];  vdata[0].data[4] = datas[0];  vdata[0].data[5] = datas[0];  vdata[0].data[6] = datas[0];  vdata[0].data[7] = datas[0];  vdata[0].data[8] = datas[0];  vdata[0].data[9] = datas[0];  vdata[0].data[10] = datas[0];  vdata[0].data[11] = datas[0];  vdata[0].data[12] = datas[0];  vdata[0].data[13] = datas[0];  vdata[0].data[14] = datas[0];  vdata[0].data[15] = datas[0];    vdata[1].data[0] = datas[1];  vdata[1].data[1] = datas[1];  vdata[1].data[2] = datas[1];  vdata[1].data[3] = datas[1];  vdata[1].data[4] = datas[1];  vdata[1].data[5] = datas[1];  vdata[1].data[6] = datas[1];  vdata[1].data[7] = datas[1];  vdata[1].data[8] = datas[1];  vdata[1].data[9] = datas[1];  vdata[1].data[10] = datas[1];  vdata[1].data[11] = datas[1];  vdata[1].data[12] = datas[1];  vdata[1].data[13] = datas[1];  vdata[1].data[14] = datas[1];  vdata[1].data[15] = datas[1];    vdata[2].data[0] = datas[2];  vdata[2].data[1] = datas[2];  vdata[2].data[2] = datas[2];  vdata[2].data[3] = datas[2];  vdata[2].data[4] = datas[2];  vdata[2].data[5] = datas[2];  vdata[2].data[6] = datas[2];  vdata[2].data[7] = datas[2];  vdata[2].data[8] = datas[2];  vdata[2].data[9] = datas[2];  vdata[2].data[10] = datas[2];  vdata[2].data[11] = datas[2];  vdata[2].data[12] = datas[2];  vdata[2].data[13] = datas[2];  vdata[2].data[14] = datas[2];  vdata[2].data[15] = datas[2];    vdata[3].data[0] = datas[3];  vdata[3].data[1] = datas[3];  vdata[3].data[2] = datas[3];  vdata[3].data[3] = datas[3];  vdata[3].data[4] = datas[3];  vdata[3].data[5] = datas[3];  vdata[3].data[6] = datas[3];  vdata[3].data[7] = datas[3];  vdata[3].data[8] = datas[3];  vdata[3].data[9] = datas[3];  vdata[3].data[10] = datas[3];  vdata[3].data[11] = datas[3];  vdata[3].data[12] = datas[3];  vdata[3].data[13] = datas[3];  vdata[3].data[14] = datas[3];  vdata[3].data[15] = datas[3];    vdata[4].data[0] = datas[4];  vdata[4].data[1] = datas[4];  vdata[4].data[2] = datas[4];  vdata[4].data[3] = datas[4];  vdata[4].data[4] = datas[4];  vdata[4].data[5] = datas[4];  vdata[4].data[6] = datas[4];  vdata[4].data[7] = datas[4];  vdata[4].data[8] = datas[4];  vdata[4].data[9] = datas[4];  vdata[4].data[10] = datas[4];  vdata[4].data[11] = datas[4];  vdata[4].data[12] = datas[4];  vdata[4].data[13] = datas[4];  vdata[4].data[14] = datas[4];  vdata[4].data[15] = datas[4];    vdata[5].data[0] = datas[5];  vdata[5].data[1] = datas[5];  vdata[5].data[2] = datas[5];  vdata[5].data[3] = datas[5];  vdata[5].data[4] = datas[5];  vdata[5].data[5] = datas[5];  vdata[5].data[6] = datas[5];  vdata[5].data[7] = datas[5];  vdata[5].data[8] = datas[5];  vdata[5].data[9] = datas[5];  vdata[5].data[10] = datas[5];  vdata[5].data[11] = datas[5];  vdata[5].data[12] = datas[5];  vdata[5].data[13] = datas[5];  vdata[5].data[14] = datas[5];  vdata[5].data[15] = datas[5];    vdata[6].data[0] = datas[6];  vdata[6].data[1] = datas[6];  vdata[6].data[2] = datas[6];  vdata[6].data[3] = datas[6];  vdata[6].data[4] = datas[6];  vdata[6].data[5] = datas[6];  vdata[6].data[6] = datas[6];  vdata[6].data[7] = datas[6];  vdata[6].data[8] = datas[6];  vdata[6].data[9] = datas[6];  vdata[6].data[10] = datas[6];  vdata[6].data[11] = datas[6];  vdata[6].data[12] = datas[6];  vdata[6].data[13] = datas[6];  vdata[6].data[14] = datas[6];  vdata[6].data[15] = datas[6];    vdata[7].data[0] = datas[7];  vdata[7].data[1] = datas[7];  vdata[7].data[2] = datas[7];  vdata[7].data[3] = datas[7];  vdata[7].data[4] = datas[7];  vdata[7].data[5] = datas[7];  vdata[7].data[6] = datas[7];  vdata[7].data[7] = datas[7];  vdata[7].data[8] = datas[7];  vdata[7].data[9] = datas[7];  vdata[7].data[10] = datas[7];  vdata[7].data[11] = datas[7];  vdata[7].data[12] = datas[7];  vdata[7].data[13] = datas[7];  vdata[7].data[14] = datas[7];  vdata[7].data[15] = datas[7];    vdata[8].data[0] = datas[8];  vdata[8].data[1] = datas[8];  vdata[8].data[2] = datas[8];  vdata[8].data[3] = datas[8];  vdata[8].data[4] = datas[8];  vdata[8].data[5] = datas[8];  vdata[8].data[6] = datas[8];  vdata[8].data[7] = datas[8];  vdata[8].data[8] = datas[8];  vdata[8].data[9] = datas[8];  vdata[8].data[10] = datas[8];  vdata[8].data[11] = datas[8];  vdata[8].data[12] = datas[8];  vdata[8].data[13] = datas[8];  vdata[8].data[14] = datas[8];  vdata[8].data[15] = datas[8];    vdata[9].data[0] = datas[9];  vdata[9].data[1] = datas[9];  vdata[9].data[2] = datas[9];  vdata[9].data[3] = datas[9];  vdata[9].data[4] = datas[9];  vdata[9].data[5] = datas[9];  vdata[9].data[6] = datas[9];  vdata[9].data[7] = datas[9];  vdata[9].data[8] = datas[9];  vdata[9].data[9] = datas[9];  vdata[9].data[10] = datas[9];  vdata[9].data[11] = datas[9];  vdata[9].data[12] = datas[9];  vdata[9].data[13] = datas[9];  vdata[9].data[14] = datas[9];  vdata[9].data[15] = datas[9];    vdata[10].data[0] = datas[10];  vdata[10].data[1] = datas[10];  vdata[10].data[2] = datas[10];  vdata[10].data[3] = datas[10];  vdata[10].data[4] = datas[10];  vdata[10].data[5] = datas[10];  vdata[10].data[6] = datas[10];  vdata[10].data[7] = datas[10];  vdata[10].data[8] = datas[10];  vdata[10].data[9] = datas[10];  vdata[10].data[10] = datas[10];  vdata[10].data[11] = datas[10];  vdata[10].data[12] = datas[10];  vdata[10].data[13] = datas[10];  vdata[10].data[14] = datas[10];  vdata[10].data[15] = datas[10];    vdata[11].data[0] = datas[11];  vdata[11].data[1] = datas[11];  vdata[11].data[2] = datas[11];  vdata[11].data[3] = datas[11];  vdata[11].data[4] = datas[11];  vdata[11].data[5] = datas[11];  vdata[11].data[6] = datas[11];  vdata[11].data[7] = datas[11];  vdata[11].data[8] = datas[11];  vdata[11].data[9] = datas[11];  vdata[11].data[10] = datas[11];  vdata[11].data[11] = datas[11];  vdata[11].data[12] = datas[11];  vdata[11].data[13] = datas[11];  vdata[11].data[14] = datas[11];  vdata[11].data[15] = datas[11];    vdata[12].data[0] = datas[12];  vdata[12].data[1] = datas[12];  vdata[12].data[2] = datas[12];  vdata[12].data[3] = datas[12];  vdata[12].data[4] = datas[12];  vdata[12].data[5] = datas[12];  vdata[12].data[6] = datas[12];  vdata[12].data[7] = datas[12];  vdata[12].data[8] = datas[12];  vdata[12].data[9] = datas[12];  vdata[12].data[10] = datas[12];  vdata[12].data[11] = datas[12];  vdata[12].data[12] = datas[12];  vdata[12].data[13] = datas[12];  vdata[12].data[14] = datas[12];  vdata[12].data[15] = datas[12];    vdata[13].data[0] = datas[13];  vdata[13].data[1] = datas[13];  vdata[13].data[2] = datas[13];  vdata[13].data[3] = datas[13];  vdata[13].data[4] = datas[13];  vdata[13].data[5] = datas[13];  vdata[13].data[6] = datas[13];  vdata[13].data[7] = datas[13];  vdata[13].data[8] = datas[13];  vdata[13].data[9] = datas[13];  vdata[13].data[10] = datas[13];  vdata[13].data[11] = datas[13];  vdata[13].data[12] = datas[13];  vdata[13].data[13] = datas[13];  vdata[13].data[14] = datas[13];  vdata[13].data[15] = datas[13];    vdata[14].data[0] = datas[14];  vdata[14].data[1] = datas[14];  vdata[14].data[2] = datas[14];  vdata[14].data[3] = datas[14];  vdata[14].data[4] = datas[14];  vdata[14].data[5] = datas[14];  vdata[14].data[6] = datas[14];  vdata[14].data[7] = datas[14];  vdata[14].data[8] = datas[14];  vdata[14].data[9] = datas[14];  vdata[14].data[10] = datas[14];  vdata[14].data[11] = datas[14];  vdata[14].data[12] = datas[14];  vdata[14].data[13] = datas[14];  vdata[14].data[14] = datas[14];  vdata[14].data[15] = datas[14];    vdata[15].data[0] = datas[15];  vdata[15].data[1] = datas[15];  vdata[15].data[2] = datas[15];  vdata[15].data[3] = datas[15];  vdata[15].data[4] = datas[15];  vdata[15].data[5] = datas[15];  vdata[15].data[6] = datas[15];  vdata[15].data[7] = datas[15];  vdata[15].data[8] = datas[15];  vdata[15].data[9] = datas[15];  vdata[15].data[10] = datas[15];  vdata[15].data[11] = datas[15];  vdata[15].data[12] = datas[15];  vdata[15].data[13] = datas[15];  vdata[15].data[14] = datas[15];  vdata[15].data[15] = datas[15];   // FIXME.
			#else 
 vdata[0].data = datas[0];  vdata[1].data = datas[1];  vdata[2].data = datas[2];  vdata[3].data = datas[3];  vdata[4].data = datas[4];  vdata[5].data = datas[5];  vdata[6].data = datas[6];  vdata[7].data = datas[7];  vdata[8].data = datas[8];  vdata[9].data = datas[9];  vdata[10].data = datas[10];  vdata[11].data = datas[11];  vdata[12].data = datas[12];  vdata[13].data = datas[13];  vdata[14].data = datas[14];  vdata[15].data = datas[15]; 	
			#endif
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(datas[v] == 1 || datas[v] < 1000){ cout<<"readANDRVchunks: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<" v: "<<v<<", voffset_kvs: "<<voffset_kvs<<", index + s: "<<index + s<<", datas["<<v<<"]: "<<datas[v]<<endl; }}
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP0_readANDRVchunks10 25", index + s, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, index, i);
			#endif
			vbuffer0[0][index + s] = vdata[0];
			vbuffer0[1][index + s] = vdata[1];
			vbuffer0[2][index + s] = vdata[2];
			vbuffer0[3][index + s] = vdata[3];
			vbuffer0[4][index + s] = vdata[4];
			vbuffer0[5][index + s] = vdata[5];
			vbuffer0[6][index + s] = vdata[6];
			vbuffer0[7][index + s] = vdata[7];
			vbuffer0[8][index + s] = vdata[8];
			vbuffer0[9][index + s] = vdata[9];
			vbuffer0[10][index + s] = vdata[10];
			vbuffer0[11][index + s] = vdata[11];
			vbuffer0[12][index + s] = vdata[12];
			vbuffer0[13][index + s] = vdata[13];
			vbuffer0[14][index + s] = vdata[14];
			vbuffer0[15][index + s] = vdata[15];
			vbuffer1[0][index + s] = vdata[0];
			vbuffer1[1][index + s] = vdata[1];
			vbuffer1[2][index + s] = vdata[2];
			vbuffer1[3][index + s] = vdata[3];
			vbuffer1[4][index + s] = vdata[4];
			vbuffer1[5][index + s] = vdata[5];
			vbuffer1[6][index + s] = vdata[6];
			vbuffer1[7][index + s] = vdata[7];
			vbuffer1[8][index + s] = vdata[8];
			vbuffer1[9][index + s] = vdata[9];
			vbuffer1[10][index + s] = vdata[10];
			vbuffer1[11][index + s] = vdata[11];
			vbuffer1[12][index + s] = vdata[12];
			vbuffer1[13][index + s] = vdata[13];
			vbuffer1[14][index + s] = vdata[14];
			vbuffer1[15][index + s] = vdata[15];
			vbuffer2[0][index + s] = vdata[0];
			vbuffer2[1][index + s] = vdata[1];
			vbuffer2[2][index + s] = vdata[2];
			vbuffer2[3][index + s] = vdata[3];
			vbuffer2[4][index + s] = vdata[4];
			vbuffer2[5][index + s] = vdata[5];
			vbuffer2[6][index + s] = vdata[6];
			vbuffer2[7][index + s] = vdata[7];
			vbuffer2[8][index + s] = vdata[8];
			vbuffer2[9][index + s] = vdata[9];
			vbuffer2[10][index + s] = vdata[10];
			vbuffer2[11][index + s] = vdata[11];
			vbuffer2[12][index + s] = vdata[12];
			vbuffer2[13][index + s] = vdata[13];
			vbuffer2[14][index + s] = vdata[14];
			vbuffer2[15][index + s] = vdata[15];
			vbuffer3[0][index + s] = vdata[0];
			vbuffer3[1][index + s] = vdata[1];
			vbuffer3[2][index + s] = vdata[2];
			vbuffer3[3][index + s] = vdata[3];
			vbuffer3[4][index + s] = vdata[4];
			vbuffer3[5][index + s] = vdata[5];
			vbuffer3[6][index + s] = vdata[6];
			vbuffer3[7][index + s] = vdata[7];
			vbuffer3[8][index + s] = vdata[8];
			vbuffer3[9][index + s] = vdata[9];
			vbuffer3[10][index + s] = vdata[10];
			vbuffer3[11][index + s] = vdata[11];
			vbuffer3[12][index + s] = vdata[12];
			vbuffer3[13][index + s] = vdata[13];
			vbuffer3[14][index + s] = vdata[14];
			vbuffer3[15][index + s] = vdata[15];
			vbuffer4[0][index + s] = vdata[0];
			vbuffer4[1][index + s] = vdata[1];
			vbuffer4[2][index + s] = vdata[2];
			vbuffer4[3][index + s] = vdata[3];
			vbuffer4[4][index + s] = vdata[4];
			vbuffer4[5][index + s] = vdata[5];
			vbuffer4[6][index + s] = vdata[6];
			vbuffer4[7][index + s] = vdata[7];
			vbuffer4[8][index + s] = vdata[8];
			vbuffer4[9][index + s] = vdata[9];
			vbuffer4[10][index + s] = vdata[10];
			vbuffer4[11][index + s] = vdata[11];
			vbuffer4[12][index + s] = vdata[12];
			vbuffer4[13][index + s] = vdata[13];
			vbuffer4[14][index + s] = vdata[14];
			vbuffer4[15][index + s] = vdata[15];
			vbuffer5[0][index + s] = vdata[0];
			vbuffer5[1][index + s] = vdata[1];
			vbuffer5[2][index + s] = vdata[2];
			vbuffer5[3][index + s] = vdata[3];
			vbuffer5[4][index + s] = vdata[4];
			vbuffer5[5][index + s] = vdata[5];
			vbuffer5[6][index + s] = vdata[6];
			vbuffer5[7][index + s] = vdata[7];
			vbuffer5[8][index + s] = vdata[8];
			vbuffer5[9][index + s] = vdata[9];
			vbuffer5[10][index + s] = vdata[10];
			vbuffer5[11][index + s] = vdata[11];
			vbuffer5[12][index + s] = vdata[12];
			vbuffer5[13][index + s] = vdata[13];
			vbuffer5[14][index + s] = vdata[14];
			vbuffer5[15][index + s] = vdata[15];
			vbuffer6[0][index + s] = vdata[0];
			vbuffer6[1][index + s] = vdata[1];
			vbuffer6[2][index + s] = vdata[2];
			vbuffer6[3][index + s] = vdata[3];
			vbuffer6[4][index + s] = vdata[4];
			vbuffer6[5][index + s] = vdata[5];
			vbuffer6[6][index + s] = vdata[6];
			vbuffer6[7][index + s] = vdata[7];
			vbuffer6[8][index + s] = vdata[8];
			vbuffer6[9][index + s] = vdata[9];
			vbuffer6[10][index + s] = vdata[10];
			vbuffer6[11][index + s] = vdata[11];
			vbuffer6[12][index + s] = vdata[12];
			vbuffer6[13][index + s] = vdata[13];
			vbuffer6[14][index + s] = vdata[14];
			vbuffer6[15][index + s] = vdata[15];
			vbuffer7[0][index + s] = vdata[0];
			vbuffer7[1][index + s] = vdata[1];
			vbuffer7[2][index + s] = vdata[2];
			vbuffer7[3][index + s] = vdata[3];
			vbuffer7[4][index + s] = vdata[4];
			vbuffer7[5][index + s] = vdata[5];
			vbuffer7[6][index + s] = vdata[6];
			vbuffer7[7][index + s] = vdata[7];
			vbuffer7[8][index + s] = vdata[8];
			vbuffer7[9][index + s] = vdata[9];
			vbuffer7[10][index + s] = vdata[10];
			vbuffer7[11][index + s] = vdata[11];
			vbuffer7[12][index + s] = vdata[12];
			vbuffer7[13][index + s] = vdata[13];
			vbuffer7[14][index + s] = vdata[14];
			vbuffer7[15][index + s] = vdata[15];
			vbuffer8[0][index + s] = vdata[0];
			vbuffer8[1][index + s] = vdata[1];
			vbuffer8[2][index + s] = vdata[2];
			vbuffer8[3][index + s] = vdata[3];
			vbuffer8[4][index + s] = vdata[4];
			vbuffer8[5][index + s] = vdata[5];
			vbuffer8[6][index + s] = vdata[6];
			vbuffer8[7][index + s] = vdata[7];
			vbuffer8[8][index + s] = vdata[8];
			vbuffer8[9][index + s] = vdata[9];
			vbuffer8[10][index + s] = vdata[10];
			vbuffer8[11][index + s] = vdata[11];
			vbuffer8[12][index + s] = vdata[12];
			vbuffer8[13][index + s] = vdata[13];
			vbuffer8[14][index + s] = vdata[14];
			vbuffer8[15][index + s] = vdata[15];
			vbuffer9[0][index + s] = vdata[0];
			vbuffer9[1][index + s] = vdata[1];
			vbuffer9[2][index + s] = vdata[2];
			vbuffer9[3][index + s] = vdata[3];
			vbuffer9[4][index + s] = vdata[4];
			vbuffer9[5][index + s] = vdata[5];
			vbuffer9[6][index + s] = vdata[6];
			vbuffer9[7][index + s] = vdata[7];
			vbuffer9[8][index + s] = vdata[8];
			vbuffer9[9][index + s] = vdata[9];
			vbuffer9[10][index + s] = vdata[10];
			vbuffer9[11][index + s] = vdata[11];
			vbuffer9[12][index + s] = vdata[12];
			vbuffer9[13][index + s] = vdata[13];
			vbuffer9[14][index + s] = vdata[14];
			vbuffer9[15][index + s] = vdata[15];
			index += NUM_PEs;

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
	}
	// exit(EXIT_SUCCESS); /////////
	return;
}
void acts_all::MEMACCESSP0_readANDRVchunks11(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer9[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer10[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	// return;//
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int limit = globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE;
	
	READANDRVCHUNKS_LOOP2: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): vsz_kvs: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		unsigned int index = 0;
		// unsigned int index = s;
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			// unsigned int offset_kvs; if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			unsigned int offset_kvs = voffset_kvs + depth_i + i;
			UTILP0_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, datas);	
			
			#ifdef BIT_TRAVERSAL_ALGORITHM
  vdata[0].data[0] = datas[0];  vdata[0].data[1] = datas[0];  vdata[0].data[2] = datas[0];  vdata[0].data[3] = datas[0];  vdata[0].data[4] = datas[0];  vdata[0].data[5] = datas[0];  vdata[0].data[6] = datas[0];  vdata[0].data[7] = datas[0];  vdata[0].data[8] = datas[0];  vdata[0].data[9] = datas[0];  vdata[0].data[10] = datas[0];  vdata[0].data[11] = datas[0];  vdata[0].data[12] = datas[0];  vdata[0].data[13] = datas[0];  vdata[0].data[14] = datas[0];  vdata[0].data[15] = datas[0];    vdata[1].data[0] = datas[1];  vdata[1].data[1] = datas[1];  vdata[1].data[2] = datas[1];  vdata[1].data[3] = datas[1];  vdata[1].data[4] = datas[1];  vdata[1].data[5] = datas[1];  vdata[1].data[6] = datas[1];  vdata[1].data[7] = datas[1];  vdata[1].data[8] = datas[1];  vdata[1].data[9] = datas[1];  vdata[1].data[10] = datas[1];  vdata[1].data[11] = datas[1];  vdata[1].data[12] = datas[1];  vdata[1].data[13] = datas[1];  vdata[1].data[14] = datas[1];  vdata[1].data[15] = datas[1];    vdata[2].data[0] = datas[2];  vdata[2].data[1] = datas[2];  vdata[2].data[2] = datas[2];  vdata[2].data[3] = datas[2];  vdata[2].data[4] = datas[2];  vdata[2].data[5] = datas[2];  vdata[2].data[6] = datas[2];  vdata[2].data[7] = datas[2];  vdata[2].data[8] = datas[2];  vdata[2].data[9] = datas[2];  vdata[2].data[10] = datas[2];  vdata[2].data[11] = datas[2];  vdata[2].data[12] = datas[2];  vdata[2].data[13] = datas[2];  vdata[2].data[14] = datas[2];  vdata[2].data[15] = datas[2];    vdata[3].data[0] = datas[3];  vdata[3].data[1] = datas[3];  vdata[3].data[2] = datas[3];  vdata[3].data[3] = datas[3];  vdata[3].data[4] = datas[3];  vdata[3].data[5] = datas[3];  vdata[3].data[6] = datas[3];  vdata[3].data[7] = datas[3];  vdata[3].data[8] = datas[3];  vdata[3].data[9] = datas[3];  vdata[3].data[10] = datas[3];  vdata[3].data[11] = datas[3];  vdata[3].data[12] = datas[3];  vdata[3].data[13] = datas[3];  vdata[3].data[14] = datas[3];  vdata[3].data[15] = datas[3];    vdata[4].data[0] = datas[4];  vdata[4].data[1] = datas[4];  vdata[4].data[2] = datas[4];  vdata[4].data[3] = datas[4];  vdata[4].data[4] = datas[4];  vdata[4].data[5] = datas[4];  vdata[4].data[6] = datas[4];  vdata[4].data[7] = datas[4];  vdata[4].data[8] = datas[4];  vdata[4].data[9] = datas[4];  vdata[4].data[10] = datas[4];  vdata[4].data[11] = datas[4];  vdata[4].data[12] = datas[4];  vdata[4].data[13] = datas[4];  vdata[4].data[14] = datas[4];  vdata[4].data[15] = datas[4];    vdata[5].data[0] = datas[5];  vdata[5].data[1] = datas[5];  vdata[5].data[2] = datas[5];  vdata[5].data[3] = datas[5];  vdata[5].data[4] = datas[5];  vdata[5].data[5] = datas[5];  vdata[5].data[6] = datas[5];  vdata[5].data[7] = datas[5];  vdata[5].data[8] = datas[5];  vdata[5].data[9] = datas[5];  vdata[5].data[10] = datas[5];  vdata[5].data[11] = datas[5];  vdata[5].data[12] = datas[5];  vdata[5].data[13] = datas[5];  vdata[5].data[14] = datas[5];  vdata[5].data[15] = datas[5];    vdata[6].data[0] = datas[6];  vdata[6].data[1] = datas[6];  vdata[6].data[2] = datas[6];  vdata[6].data[3] = datas[6];  vdata[6].data[4] = datas[6];  vdata[6].data[5] = datas[6];  vdata[6].data[6] = datas[6];  vdata[6].data[7] = datas[6];  vdata[6].data[8] = datas[6];  vdata[6].data[9] = datas[6];  vdata[6].data[10] = datas[6];  vdata[6].data[11] = datas[6];  vdata[6].data[12] = datas[6];  vdata[6].data[13] = datas[6];  vdata[6].data[14] = datas[6];  vdata[6].data[15] = datas[6];    vdata[7].data[0] = datas[7];  vdata[7].data[1] = datas[7];  vdata[7].data[2] = datas[7];  vdata[7].data[3] = datas[7];  vdata[7].data[4] = datas[7];  vdata[7].data[5] = datas[7];  vdata[7].data[6] = datas[7];  vdata[7].data[7] = datas[7];  vdata[7].data[8] = datas[7];  vdata[7].data[9] = datas[7];  vdata[7].data[10] = datas[7];  vdata[7].data[11] = datas[7];  vdata[7].data[12] = datas[7];  vdata[7].data[13] = datas[7];  vdata[7].data[14] = datas[7];  vdata[7].data[15] = datas[7];    vdata[8].data[0] = datas[8];  vdata[8].data[1] = datas[8];  vdata[8].data[2] = datas[8];  vdata[8].data[3] = datas[8];  vdata[8].data[4] = datas[8];  vdata[8].data[5] = datas[8];  vdata[8].data[6] = datas[8];  vdata[8].data[7] = datas[8];  vdata[8].data[8] = datas[8];  vdata[8].data[9] = datas[8];  vdata[8].data[10] = datas[8];  vdata[8].data[11] = datas[8];  vdata[8].data[12] = datas[8];  vdata[8].data[13] = datas[8];  vdata[8].data[14] = datas[8];  vdata[8].data[15] = datas[8];    vdata[9].data[0] = datas[9];  vdata[9].data[1] = datas[9];  vdata[9].data[2] = datas[9];  vdata[9].data[3] = datas[9];  vdata[9].data[4] = datas[9];  vdata[9].data[5] = datas[9];  vdata[9].data[6] = datas[9];  vdata[9].data[7] = datas[9];  vdata[9].data[8] = datas[9];  vdata[9].data[9] = datas[9];  vdata[9].data[10] = datas[9];  vdata[9].data[11] = datas[9];  vdata[9].data[12] = datas[9];  vdata[9].data[13] = datas[9];  vdata[9].data[14] = datas[9];  vdata[9].data[15] = datas[9];    vdata[10].data[0] = datas[10];  vdata[10].data[1] = datas[10];  vdata[10].data[2] = datas[10];  vdata[10].data[3] = datas[10];  vdata[10].data[4] = datas[10];  vdata[10].data[5] = datas[10];  vdata[10].data[6] = datas[10];  vdata[10].data[7] = datas[10];  vdata[10].data[8] = datas[10];  vdata[10].data[9] = datas[10];  vdata[10].data[10] = datas[10];  vdata[10].data[11] = datas[10];  vdata[10].data[12] = datas[10];  vdata[10].data[13] = datas[10];  vdata[10].data[14] = datas[10];  vdata[10].data[15] = datas[10];    vdata[11].data[0] = datas[11];  vdata[11].data[1] = datas[11];  vdata[11].data[2] = datas[11];  vdata[11].data[3] = datas[11];  vdata[11].data[4] = datas[11];  vdata[11].data[5] = datas[11];  vdata[11].data[6] = datas[11];  vdata[11].data[7] = datas[11];  vdata[11].data[8] = datas[11];  vdata[11].data[9] = datas[11];  vdata[11].data[10] = datas[11];  vdata[11].data[11] = datas[11];  vdata[11].data[12] = datas[11];  vdata[11].data[13] = datas[11];  vdata[11].data[14] = datas[11];  vdata[11].data[15] = datas[11];    vdata[12].data[0] = datas[12];  vdata[12].data[1] = datas[12];  vdata[12].data[2] = datas[12];  vdata[12].data[3] = datas[12];  vdata[12].data[4] = datas[12];  vdata[12].data[5] = datas[12];  vdata[12].data[6] = datas[12];  vdata[12].data[7] = datas[12];  vdata[12].data[8] = datas[12];  vdata[12].data[9] = datas[12];  vdata[12].data[10] = datas[12];  vdata[12].data[11] = datas[12];  vdata[12].data[12] = datas[12];  vdata[12].data[13] = datas[12];  vdata[12].data[14] = datas[12];  vdata[12].data[15] = datas[12];    vdata[13].data[0] = datas[13];  vdata[13].data[1] = datas[13];  vdata[13].data[2] = datas[13];  vdata[13].data[3] = datas[13];  vdata[13].data[4] = datas[13];  vdata[13].data[5] = datas[13];  vdata[13].data[6] = datas[13];  vdata[13].data[7] = datas[13];  vdata[13].data[8] = datas[13];  vdata[13].data[9] = datas[13];  vdata[13].data[10] = datas[13];  vdata[13].data[11] = datas[13];  vdata[13].data[12] = datas[13];  vdata[13].data[13] = datas[13];  vdata[13].data[14] = datas[13];  vdata[13].data[15] = datas[13];    vdata[14].data[0] = datas[14];  vdata[14].data[1] = datas[14];  vdata[14].data[2] = datas[14];  vdata[14].data[3] = datas[14];  vdata[14].data[4] = datas[14];  vdata[14].data[5] = datas[14];  vdata[14].data[6] = datas[14];  vdata[14].data[7] = datas[14];  vdata[14].data[8] = datas[14];  vdata[14].data[9] = datas[14];  vdata[14].data[10] = datas[14];  vdata[14].data[11] = datas[14];  vdata[14].data[12] = datas[14];  vdata[14].data[13] = datas[14];  vdata[14].data[14] = datas[14];  vdata[14].data[15] = datas[14];    vdata[15].data[0] = datas[15];  vdata[15].data[1] = datas[15];  vdata[15].data[2] = datas[15];  vdata[15].data[3] = datas[15];  vdata[15].data[4] = datas[15];  vdata[15].data[5] = datas[15];  vdata[15].data[6] = datas[15];  vdata[15].data[7] = datas[15];  vdata[15].data[8] = datas[15];  vdata[15].data[9] = datas[15];  vdata[15].data[10] = datas[15];  vdata[15].data[11] = datas[15];  vdata[15].data[12] = datas[15];  vdata[15].data[13] = datas[15];  vdata[15].data[14] = datas[15];  vdata[15].data[15] = datas[15];   // FIXME.
			#else 
 vdata[0].data = datas[0];  vdata[1].data = datas[1];  vdata[2].data = datas[2];  vdata[3].data = datas[3];  vdata[4].data = datas[4];  vdata[5].data = datas[5];  vdata[6].data = datas[6];  vdata[7].data = datas[7];  vdata[8].data = datas[8];  vdata[9].data = datas[9];  vdata[10].data = datas[10];  vdata[11].data = datas[11];  vdata[12].data = datas[12];  vdata[13].data = datas[13];  vdata[14].data = datas[14];  vdata[15].data = datas[15]; 	
			#endif
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(datas[v] == 1 || datas[v] < 1000){ cout<<"readANDRVchunks: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<" v: "<<v<<", voffset_kvs: "<<voffset_kvs<<", index + s: "<<index + s<<", datas["<<v<<"]: "<<datas[v]<<endl; }}
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP0_readANDRVchunks11 25", index + s, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, index, i);
			#endif
			vbuffer0[0][index + s] = vdata[0];
			vbuffer0[1][index + s] = vdata[1];
			vbuffer0[2][index + s] = vdata[2];
			vbuffer0[3][index + s] = vdata[3];
			vbuffer0[4][index + s] = vdata[4];
			vbuffer0[5][index + s] = vdata[5];
			vbuffer0[6][index + s] = vdata[6];
			vbuffer0[7][index + s] = vdata[7];
			vbuffer0[8][index + s] = vdata[8];
			vbuffer0[9][index + s] = vdata[9];
			vbuffer0[10][index + s] = vdata[10];
			vbuffer0[11][index + s] = vdata[11];
			vbuffer0[12][index + s] = vdata[12];
			vbuffer0[13][index + s] = vdata[13];
			vbuffer0[14][index + s] = vdata[14];
			vbuffer0[15][index + s] = vdata[15];
			vbuffer1[0][index + s] = vdata[0];
			vbuffer1[1][index + s] = vdata[1];
			vbuffer1[2][index + s] = vdata[2];
			vbuffer1[3][index + s] = vdata[3];
			vbuffer1[4][index + s] = vdata[4];
			vbuffer1[5][index + s] = vdata[5];
			vbuffer1[6][index + s] = vdata[6];
			vbuffer1[7][index + s] = vdata[7];
			vbuffer1[8][index + s] = vdata[8];
			vbuffer1[9][index + s] = vdata[9];
			vbuffer1[10][index + s] = vdata[10];
			vbuffer1[11][index + s] = vdata[11];
			vbuffer1[12][index + s] = vdata[12];
			vbuffer1[13][index + s] = vdata[13];
			vbuffer1[14][index + s] = vdata[14];
			vbuffer1[15][index + s] = vdata[15];
			vbuffer2[0][index + s] = vdata[0];
			vbuffer2[1][index + s] = vdata[1];
			vbuffer2[2][index + s] = vdata[2];
			vbuffer2[3][index + s] = vdata[3];
			vbuffer2[4][index + s] = vdata[4];
			vbuffer2[5][index + s] = vdata[5];
			vbuffer2[6][index + s] = vdata[6];
			vbuffer2[7][index + s] = vdata[7];
			vbuffer2[8][index + s] = vdata[8];
			vbuffer2[9][index + s] = vdata[9];
			vbuffer2[10][index + s] = vdata[10];
			vbuffer2[11][index + s] = vdata[11];
			vbuffer2[12][index + s] = vdata[12];
			vbuffer2[13][index + s] = vdata[13];
			vbuffer2[14][index + s] = vdata[14];
			vbuffer2[15][index + s] = vdata[15];
			vbuffer3[0][index + s] = vdata[0];
			vbuffer3[1][index + s] = vdata[1];
			vbuffer3[2][index + s] = vdata[2];
			vbuffer3[3][index + s] = vdata[3];
			vbuffer3[4][index + s] = vdata[4];
			vbuffer3[5][index + s] = vdata[5];
			vbuffer3[6][index + s] = vdata[6];
			vbuffer3[7][index + s] = vdata[7];
			vbuffer3[8][index + s] = vdata[8];
			vbuffer3[9][index + s] = vdata[9];
			vbuffer3[10][index + s] = vdata[10];
			vbuffer3[11][index + s] = vdata[11];
			vbuffer3[12][index + s] = vdata[12];
			vbuffer3[13][index + s] = vdata[13];
			vbuffer3[14][index + s] = vdata[14];
			vbuffer3[15][index + s] = vdata[15];
			vbuffer4[0][index + s] = vdata[0];
			vbuffer4[1][index + s] = vdata[1];
			vbuffer4[2][index + s] = vdata[2];
			vbuffer4[3][index + s] = vdata[3];
			vbuffer4[4][index + s] = vdata[4];
			vbuffer4[5][index + s] = vdata[5];
			vbuffer4[6][index + s] = vdata[6];
			vbuffer4[7][index + s] = vdata[7];
			vbuffer4[8][index + s] = vdata[8];
			vbuffer4[9][index + s] = vdata[9];
			vbuffer4[10][index + s] = vdata[10];
			vbuffer4[11][index + s] = vdata[11];
			vbuffer4[12][index + s] = vdata[12];
			vbuffer4[13][index + s] = vdata[13];
			vbuffer4[14][index + s] = vdata[14];
			vbuffer4[15][index + s] = vdata[15];
			vbuffer5[0][index + s] = vdata[0];
			vbuffer5[1][index + s] = vdata[1];
			vbuffer5[2][index + s] = vdata[2];
			vbuffer5[3][index + s] = vdata[3];
			vbuffer5[4][index + s] = vdata[4];
			vbuffer5[5][index + s] = vdata[5];
			vbuffer5[6][index + s] = vdata[6];
			vbuffer5[7][index + s] = vdata[7];
			vbuffer5[8][index + s] = vdata[8];
			vbuffer5[9][index + s] = vdata[9];
			vbuffer5[10][index + s] = vdata[10];
			vbuffer5[11][index + s] = vdata[11];
			vbuffer5[12][index + s] = vdata[12];
			vbuffer5[13][index + s] = vdata[13];
			vbuffer5[14][index + s] = vdata[14];
			vbuffer5[15][index + s] = vdata[15];
			vbuffer6[0][index + s] = vdata[0];
			vbuffer6[1][index + s] = vdata[1];
			vbuffer6[2][index + s] = vdata[2];
			vbuffer6[3][index + s] = vdata[3];
			vbuffer6[4][index + s] = vdata[4];
			vbuffer6[5][index + s] = vdata[5];
			vbuffer6[6][index + s] = vdata[6];
			vbuffer6[7][index + s] = vdata[7];
			vbuffer6[8][index + s] = vdata[8];
			vbuffer6[9][index + s] = vdata[9];
			vbuffer6[10][index + s] = vdata[10];
			vbuffer6[11][index + s] = vdata[11];
			vbuffer6[12][index + s] = vdata[12];
			vbuffer6[13][index + s] = vdata[13];
			vbuffer6[14][index + s] = vdata[14];
			vbuffer6[15][index + s] = vdata[15];
			vbuffer7[0][index + s] = vdata[0];
			vbuffer7[1][index + s] = vdata[1];
			vbuffer7[2][index + s] = vdata[2];
			vbuffer7[3][index + s] = vdata[3];
			vbuffer7[4][index + s] = vdata[4];
			vbuffer7[5][index + s] = vdata[5];
			vbuffer7[6][index + s] = vdata[6];
			vbuffer7[7][index + s] = vdata[7];
			vbuffer7[8][index + s] = vdata[8];
			vbuffer7[9][index + s] = vdata[9];
			vbuffer7[10][index + s] = vdata[10];
			vbuffer7[11][index + s] = vdata[11];
			vbuffer7[12][index + s] = vdata[12];
			vbuffer7[13][index + s] = vdata[13];
			vbuffer7[14][index + s] = vdata[14];
			vbuffer7[15][index + s] = vdata[15];
			vbuffer8[0][index + s] = vdata[0];
			vbuffer8[1][index + s] = vdata[1];
			vbuffer8[2][index + s] = vdata[2];
			vbuffer8[3][index + s] = vdata[3];
			vbuffer8[4][index + s] = vdata[4];
			vbuffer8[5][index + s] = vdata[5];
			vbuffer8[6][index + s] = vdata[6];
			vbuffer8[7][index + s] = vdata[7];
			vbuffer8[8][index + s] = vdata[8];
			vbuffer8[9][index + s] = vdata[9];
			vbuffer8[10][index + s] = vdata[10];
			vbuffer8[11][index + s] = vdata[11];
			vbuffer8[12][index + s] = vdata[12];
			vbuffer8[13][index + s] = vdata[13];
			vbuffer8[14][index + s] = vdata[14];
			vbuffer8[15][index + s] = vdata[15];
			vbuffer9[0][index + s] = vdata[0];
			vbuffer9[1][index + s] = vdata[1];
			vbuffer9[2][index + s] = vdata[2];
			vbuffer9[3][index + s] = vdata[3];
			vbuffer9[4][index + s] = vdata[4];
			vbuffer9[5][index + s] = vdata[5];
			vbuffer9[6][index + s] = vdata[6];
			vbuffer9[7][index + s] = vdata[7];
			vbuffer9[8][index + s] = vdata[8];
			vbuffer9[9][index + s] = vdata[9];
			vbuffer9[10][index + s] = vdata[10];
			vbuffer9[11][index + s] = vdata[11];
			vbuffer9[12][index + s] = vdata[12];
			vbuffer9[13][index + s] = vdata[13];
			vbuffer9[14][index + s] = vdata[14];
			vbuffer9[15][index + s] = vdata[15];
			vbuffer10[0][index + s] = vdata[0];
			vbuffer10[1][index + s] = vdata[1];
			vbuffer10[2][index + s] = vdata[2];
			vbuffer10[3][index + s] = vdata[3];
			vbuffer10[4][index + s] = vdata[4];
			vbuffer10[5][index + s] = vdata[5];
			vbuffer10[6][index + s] = vdata[6];
			vbuffer10[7][index + s] = vdata[7];
			vbuffer10[8][index + s] = vdata[8];
			vbuffer10[9][index + s] = vdata[9];
			vbuffer10[10][index + s] = vdata[10];
			vbuffer10[11][index + s] = vdata[11];
			vbuffer10[12][index + s] = vdata[12];
			vbuffer10[13][index + s] = vdata[13];
			vbuffer10[14][index + s] = vdata[14];
			vbuffer10[15][index + s] = vdata[15];
			index += NUM_PEs;

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
	}
	// exit(EXIT_SUCCESS); /////////
	return;
}
void acts_all::MEMACCESSP0_readANDRVchunks12(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer9[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer10[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer11[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	// return;//
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int limit = globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE;
	
	READANDRVCHUNKS_LOOP2: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): vsz_kvs: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		unsigned int index = 0;
		// unsigned int index = s;
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			// unsigned int offset_kvs; if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			unsigned int offset_kvs = voffset_kvs + depth_i + i;
			UTILP0_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, datas);	
			
			#ifdef BIT_TRAVERSAL_ALGORITHM
  vdata[0].data[0] = datas[0];  vdata[0].data[1] = datas[0];  vdata[0].data[2] = datas[0];  vdata[0].data[3] = datas[0];  vdata[0].data[4] = datas[0];  vdata[0].data[5] = datas[0];  vdata[0].data[6] = datas[0];  vdata[0].data[7] = datas[0];  vdata[0].data[8] = datas[0];  vdata[0].data[9] = datas[0];  vdata[0].data[10] = datas[0];  vdata[0].data[11] = datas[0];  vdata[0].data[12] = datas[0];  vdata[0].data[13] = datas[0];  vdata[0].data[14] = datas[0];  vdata[0].data[15] = datas[0];    vdata[1].data[0] = datas[1];  vdata[1].data[1] = datas[1];  vdata[1].data[2] = datas[1];  vdata[1].data[3] = datas[1];  vdata[1].data[4] = datas[1];  vdata[1].data[5] = datas[1];  vdata[1].data[6] = datas[1];  vdata[1].data[7] = datas[1];  vdata[1].data[8] = datas[1];  vdata[1].data[9] = datas[1];  vdata[1].data[10] = datas[1];  vdata[1].data[11] = datas[1];  vdata[1].data[12] = datas[1];  vdata[1].data[13] = datas[1];  vdata[1].data[14] = datas[1];  vdata[1].data[15] = datas[1];    vdata[2].data[0] = datas[2];  vdata[2].data[1] = datas[2];  vdata[2].data[2] = datas[2];  vdata[2].data[3] = datas[2];  vdata[2].data[4] = datas[2];  vdata[2].data[5] = datas[2];  vdata[2].data[6] = datas[2];  vdata[2].data[7] = datas[2];  vdata[2].data[8] = datas[2];  vdata[2].data[9] = datas[2];  vdata[2].data[10] = datas[2];  vdata[2].data[11] = datas[2];  vdata[2].data[12] = datas[2];  vdata[2].data[13] = datas[2];  vdata[2].data[14] = datas[2];  vdata[2].data[15] = datas[2];    vdata[3].data[0] = datas[3];  vdata[3].data[1] = datas[3];  vdata[3].data[2] = datas[3];  vdata[3].data[3] = datas[3];  vdata[3].data[4] = datas[3];  vdata[3].data[5] = datas[3];  vdata[3].data[6] = datas[3];  vdata[3].data[7] = datas[3];  vdata[3].data[8] = datas[3];  vdata[3].data[9] = datas[3];  vdata[3].data[10] = datas[3];  vdata[3].data[11] = datas[3];  vdata[3].data[12] = datas[3];  vdata[3].data[13] = datas[3];  vdata[3].data[14] = datas[3];  vdata[3].data[15] = datas[3];    vdata[4].data[0] = datas[4];  vdata[4].data[1] = datas[4];  vdata[4].data[2] = datas[4];  vdata[4].data[3] = datas[4];  vdata[4].data[4] = datas[4];  vdata[4].data[5] = datas[4];  vdata[4].data[6] = datas[4];  vdata[4].data[7] = datas[4];  vdata[4].data[8] = datas[4];  vdata[4].data[9] = datas[4];  vdata[4].data[10] = datas[4];  vdata[4].data[11] = datas[4];  vdata[4].data[12] = datas[4];  vdata[4].data[13] = datas[4];  vdata[4].data[14] = datas[4];  vdata[4].data[15] = datas[4];    vdata[5].data[0] = datas[5];  vdata[5].data[1] = datas[5];  vdata[5].data[2] = datas[5];  vdata[5].data[3] = datas[5];  vdata[5].data[4] = datas[5];  vdata[5].data[5] = datas[5];  vdata[5].data[6] = datas[5];  vdata[5].data[7] = datas[5];  vdata[5].data[8] = datas[5];  vdata[5].data[9] = datas[5];  vdata[5].data[10] = datas[5];  vdata[5].data[11] = datas[5];  vdata[5].data[12] = datas[5];  vdata[5].data[13] = datas[5];  vdata[5].data[14] = datas[5];  vdata[5].data[15] = datas[5];    vdata[6].data[0] = datas[6];  vdata[6].data[1] = datas[6];  vdata[6].data[2] = datas[6];  vdata[6].data[3] = datas[6];  vdata[6].data[4] = datas[6];  vdata[6].data[5] = datas[6];  vdata[6].data[6] = datas[6];  vdata[6].data[7] = datas[6];  vdata[6].data[8] = datas[6];  vdata[6].data[9] = datas[6];  vdata[6].data[10] = datas[6];  vdata[6].data[11] = datas[6];  vdata[6].data[12] = datas[6];  vdata[6].data[13] = datas[6];  vdata[6].data[14] = datas[6];  vdata[6].data[15] = datas[6];    vdata[7].data[0] = datas[7];  vdata[7].data[1] = datas[7];  vdata[7].data[2] = datas[7];  vdata[7].data[3] = datas[7];  vdata[7].data[4] = datas[7];  vdata[7].data[5] = datas[7];  vdata[7].data[6] = datas[7];  vdata[7].data[7] = datas[7];  vdata[7].data[8] = datas[7];  vdata[7].data[9] = datas[7];  vdata[7].data[10] = datas[7];  vdata[7].data[11] = datas[7];  vdata[7].data[12] = datas[7];  vdata[7].data[13] = datas[7];  vdata[7].data[14] = datas[7];  vdata[7].data[15] = datas[7];    vdata[8].data[0] = datas[8];  vdata[8].data[1] = datas[8];  vdata[8].data[2] = datas[8];  vdata[8].data[3] = datas[8];  vdata[8].data[4] = datas[8];  vdata[8].data[5] = datas[8];  vdata[8].data[6] = datas[8];  vdata[8].data[7] = datas[8];  vdata[8].data[8] = datas[8];  vdata[8].data[9] = datas[8];  vdata[8].data[10] = datas[8];  vdata[8].data[11] = datas[8];  vdata[8].data[12] = datas[8];  vdata[8].data[13] = datas[8];  vdata[8].data[14] = datas[8];  vdata[8].data[15] = datas[8];    vdata[9].data[0] = datas[9];  vdata[9].data[1] = datas[9];  vdata[9].data[2] = datas[9];  vdata[9].data[3] = datas[9];  vdata[9].data[4] = datas[9];  vdata[9].data[5] = datas[9];  vdata[9].data[6] = datas[9];  vdata[9].data[7] = datas[9];  vdata[9].data[8] = datas[9];  vdata[9].data[9] = datas[9];  vdata[9].data[10] = datas[9];  vdata[9].data[11] = datas[9];  vdata[9].data[12] = datas[9];  vdata[9].data[13] = datas[9];  vdata[9].data[14] = datas[9];  vdata[9].data[15] = datas[9];    vdata[10].data[0] = datas[10];  vdata[10].data[1] = datas[10];  vdata[10].data[2] = datas[10];  vdata[10].data[3] = datas[10];  vdata[10].data[4] = datas[10];  vdata[10].data[5] = datas[10];  vdata[10].data[6] = datas[10];  vdata[10].data[7] = datas[10];  vdata[10].data[8] = datas[10];  vdata[10].data[9] = datas[10];  vdata[10].data[10] = datas[10];  vdata[10].data[11] = datas[10];  vdata[10].data[12] = datas[10];  vdata[10].data[13] = datas[10];  vdata[10].data[14] = datas[10];  vdata[10].data[15] = datas[10];    vdata[11].data[0] = datas[11];  vdata[11].data[1] = datas[11];  vdata[11].data[2] = datas[11];  vdata[11].data[3] = datas[11];  vdata[11].data[4] = datas[11];  vdata[11].data[5] = datas[11];  vdata[11].data[6] = datas[11];  vdata[11].data[7] = datas[11];  vdata[11].data[8] = datas[11];  vdata[11].data[9] = datas[11];  vdata[11].data[10] = datas[11];  vdata[11].data[11] = datas[11];  vdata[11].data[12] = datas[11];  vdata[11].data[13] = datas[11];  vdata[11].data[14] = datas[11];  vdata[11].data[15] = datas[11];    vdata[12].data[0] = datas[12];  vdata[12].data[1] = datas[12];  vdata[12].data[2] = datas[12];  vdata[12].data[3] = datas[12];  vdata[12].data[4] = datas[12];  vdata[12].data[5] = datas[12];  vdata[12].data[6] = datas[12];  vdata[12].data[7] = datas[12];  vdata[12].data[8] = datas[12];  vdata[12].data[9] = datas[12];  vdata[12].data[10] = datas[12];  vdata[12].data[11] = datas[12];  vdata[12].data[12] = datas[12];  vdata[12].data[13] = datas[12];  vdata[12].data[14] = datas[12];  vdata[12].data[15] = datas[12];    vdata[13].data[0] = datas[13];  vdata[13].data[1] = datas[13];  vdata[13].data[2] = datas[13];  vdata[13].data[3] = datas[13];  vdata[13].data[4] = datas[13];  vdata[13].data[5] = datas[13];  vdata[13].data[6] = datas[13];  vdata[13].data[7] = datas[13];  vdata[13].data[8] = datas[13];  vdata[13].data[9] = datas[13];  vdata[13].data[10] = datas[13];  vdata[13].data[11] = datas[13];  vdata[13].data[12] = datas[13];  vdata[13].data[13] = datas[13];  vdata[13].data[14] = datas[13];  vdata[13].data[15] = datas[13];    vdata[14].data[0] = datas[14];  vdata[14].data[1] = datas[14];  vdata[14].data[2] = datas[14];  vdata[14].data[3] = datas[14];  vdata[14].data[4] = datas[14];  vdata[14].data[5] = datas[14];  vdata[14].data[6] = datas[14];  vdata[14].data[7] = datas[14];  vdata[14].data[8] = datas[14];  vdata[14].data[9] = datas[14];  vdata[14].data[10] = datas[14];  vdata[14].data[11] = datas[14];  vdata[14].data[12] = datas[14];  vdata[14].data[13] = datas[14];  vdata[14].data[14] = datas[14];  vdata[14].data[15] = datas[14];    vdata[15].data[0] = datas[15];  vdata[15].data[1] = datas[15];  vdata[15].data[2] = datas[15];  vdata[15].data[3] = datas[15];  vdata[15].data[4] = datas[15];  vdata[15].data[5] = datas[15];  vdata[15].data[6] = datas[15];  vdata[15].data[7] = datas[15];  vdata[15].data[8] = datas[15];  vdata[15].data[9] = datas[15];  vdata[15].data[10] = datas[15];  vdata[15].data[11] = datas[15];  vdata[15].data[12] = datas[15];  vdata[15].data[13] = datas[15];  vdata[15].data[14] = datas[15];  vdata[15].data[15] = datas[15];   // FIXME.
			#else 
 vdata[0].data = datas[0];  vdata[1].data = datas[1];  vdata[2].data = datas[2];  vdata[3].data = datas[3];  vdata[4].data = datas[4];  vdata[5].data = datas[5];  vdata[6].data = datas[6];  vdata[7].data = datas[7];  vdata[8].data = datas[8];  vdata[9].data = datas[9];  vdata[10].data = datas[10];  vdata[11].data = datas[11];  vdata[12].data = datas[12];  vdata[13].data = datas[13];  vdata[14].data = datas[14];  vdata[15].data = datas[15]; 	
			#endif
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(datas[v] == 1 || datas[v] < 1000){ cout<<"readANDRVchunks: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<" v: "<<v<<", voffset_kvs: "<<voffset_kvs<<", index + s: "<<index + s<<", datas["<<v<<"]: "<<datas[v]<<endl; }}
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP0_readANDRVchunks12 25", index + s, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, index, i);
			#endif
			vbuffer0[0][index + s] = vdata[0];
			vbuffer0[1][index + s] = vdata[1];
			vbuffer0[2][index + s] = vdata[2];
			vbuffer0[3][index + s] = vdata[3];
			vbuffer0[4][index + s] = vdata[4];
			vbuffer0[5][index + s] = vdata[5];
			vbuffer0[6][index + s] = vdata[6];
			vbuffer0[7][index + s] = vdata[7];
			vbuffer0[8][index + s] = vdata[8];
			vbuffer0[9][index + s] = vdata[9];
			vbuffer0[10][index + s] = vdata[10];
			vbuffer0[11][index + s] = vdata[11];
			vbuffer0[12][index + s] = vdata[12];
			vbuffer0[13][index + s] = vdata[13];
			vbuffer0[14][index + s] = vdata[14];
			vbuffer0[15][index + s] = vdata[15];
			vbuffer1[0][index + s] = vdata[0];
			vbuffer1[1][index + s] = vdata[1];
			vbuffer1[2][index + s] = vdata[2];
			vbuffer1[3][index + s] = vdata[3];
			vbuffer1[4][index + s] = vdata[4];
			vbuffer1[5][index + s] = vdata[5];
			vbuffer1[6][index + s] = vdata[6];
			vbuffer1[7][index + s] = vdata[7];
			vbuffer1[8][index + s] = vdata[8];
			vbuffer1[9][index + s] = vdata[9];
			vbuffer1[10][index + s] = vdata[10];
			vbuffer1[11][index + s] = vdata[11];
			vbuffer1[12][index + s] = vdata[12];
			vbuffer1[13][index + s] = vdata[13];
			vbuffer1[14][index + s] = vdata[14];
			vbuffer1[15][index + s] = vdata[15];
			vbuffer2[0][index + s] = vdata[0];
			vbuffer2[1][index + s] = vdata[1];
			vbuffer2[2][index + s] = vdata[2];
			vbuffer2[3][index + s] = vdata[3];
			vbuffer2[4][index + s] = vdata[4];
			vbuffer2[5][index + s] = vdata[5];
			vbuffer2[6][index + s] = vdata[6];
			vbuffer2[7][index + s] = vdata[7];
			vbuffer2[8][index + s] = vdata[8];
			vbuffer2[9][index + s] = vdata[9];
			vbuffer2[10][index + s] = vdata[10];
			vbuffer2[11][index + s] = vdata[11];
			vbuffer2[12][index + s] = vdata[12];
			vbuffer2[13][index + s] = vdata[13];
			vbuffer2[14][index + s] = vdata[14];
			vbuffer2[15][index + s] = vdata[15];
			vbuffer3[0][index + s] = vdata[0];
			vbuffer3[1][index + s] = vdata[1];
			vbuffer3[2][index + s] = vdata[2];
			vbuffer3[3][index + s] = vdata[3];
			vbuffer3[4][index + s] = vdata[4];
			vbuffer3[5][index + s] = vdata[5];
			vbuffer3[6][index + s] = vdata[6];
			vbuffer3[7][index + s] = vdata[7];
			vbuffer3[8][index + s] = vdata[8];
			vbuffer3[9][index + s] = vdata[9];
			vbuffer3[10][index + s] = vdata[10];
			vbuffer3[11][index + s] = vdata[11];
			vbuffer3[12][index + s] = vdata[12];
			vbuffer3[13][index + s] = vdata[13];
			vbuffer3[14][index + s] = vdata[14];
			vbuffer3[15][index + s] = vdata[15];
			vbuffer4[0][index + s] = vdata[0];
			vbuffer4[1][index + s] = vdata[1];
			vbuffer4[2][index + s] = vdata[2];
			vbuffer4[3][index + s] = vdata[3];
			vbuffer4[4][index + s] = vdata[4];
			vbuffer4[5][index + s] = vdata[5];
			vbuffer4[6][index + s] = vdata[6];
			vbuffer4[7][index + s] = vdata[7];
			vbuffer4[8][index + s] = vdata[8];
			vbuffer4[9][index + s] = vdata[9];
			vbuffer4[10][index + s] = vdata[10];
			vbuffer4[11][index + s] = vdata[11];
			vbuffer4[12][index + s] = vdata[12];
			vbuffer4[13][index + s] = vdata[13];
			vbuffer4[14][index + s] = vdata[14];
			vbuffer4[15][index + s] = vdata[15];
			vbuffer5[0][index + s] = vdata[0];
			vbuffer5[1][index + s] = vdata[1];
			vbuffer5[2][index + s] = vdata[2];
			vbuffer5[3][index + s] = vdata[3];
			vbuffer5[4][index + s] = vdata[4];
			vbuffer5[5][index + s] = vdata[5];
			vbuffer5[6][index + s] = vdata[6];
			vbuffer5[7][index + s] = vdata[7];
			vbuffer5[8][index + s] = vdata[8];
			vbuffer5[9][index + s] = vdata[9];
			vbuffer5[10][index + s] = vdata[10];
			vbuffer5[11][index + s] = vdata[11];
			vbuffer5[12][index + s] = vdata[12];
			vbuffer5[13][index + s] = vdata[13];
			vbuffer5[14][index + s] = vdata[14];
			vbuffer5[15][index + s] = vdata[15];
			vbuffer6[0][index + s] = vdata[0];
			vbuffer6[1][index + s] = vdata[1];
			vbuffer6[2][index + s] = vdata[2];
			vbuffer6[3][index + s] = vdata[3];
			vbuffer6[4][index + s] = vdata[4];
			vbuffer6[5][index + s] = vdata[5];
			vbuffer6[6][index + s] = vdata[6];
			vbuffer6[7][index + s] = vdata[7];
			vbuffer6[8][index + s] = vdata[8];
			vbuffer6[9][index + s] = vdata[9];
			vbuffer6[10][index + s] = vdata[10];
			vbuffer6[11][index + s] = vdata[11];
			vbuffer6[12][index + s] = vdata[12];
			vbuffer6[13][index + s] = vdata[13];
			vbuffer6[14][index + s] = vdata[14];
			vbuffer6[15][index + s] = vdata[15];
			vbuffer7[0][index + s] = vdata[0];
			vbuffer7[1][index + s] = vdata[1];
			vbuffer7[2][index + s] = vdata[2];
			vbuffer7[3][index + s] = vdata[3];
			vbuffer7[4][index + s] = vdata[4];
			vbuffer7[5][index + s] = vdata[5];
			vbuffer7[6][index + s] = vdata[6];
			vbuffer7[7][index + s] = vdata[7];
			vbuffer7[8][index + s] = vdata[8];
			vbuffer7[9][index + s] = vdata[9];
			vbuffer7[10][index + s] = vdata[10];
			vbuffer7[11][index + s] = vdata[11];
			vbuffer7[12][index + s] = vdata[12];
			vbuffer7[13][index + s] = vdata[13];
			vbuffer7[14][index + s] = vdata[14];
			vbuffer7[15][index + s] = vdata[15];
			vbuffer8[0][index + s] = vdata[0];
			vbuffer8[1][index + s] = vdata[1];
			vbuffer8[2][index + s] = vdata[2];
			vbuffer8[3][index + s] = vdata[3];
			vbuffer8[4][index + s] = vdata[4];
			vbuffer8[5][index + s] = vdata[5];
			vbuffer8[6][index + s] = vdata[6];
			vbuffer8[7][index + s] = vdata[7];
			vbuffer8[8][index + s] = vdata[8];
			vbuffer8[9][index + s] = vdata[9];
			vbuffer8[10][index + s] = vdata[10];
			vbuffer8[11][index + s] = vdata[11];
			vbuffer8[12][index + s] = vdata[12];
			vbuffer8[13][index + s] = vdata[13];
			vbuffer8[14][index + s] = vdata[14];
			vbuffer8[15][index + s] = vdata[15];
			vbuffer9[0][index + s] = vdata[0];
			vbuffer9[1][index + s] = vdata[1];
			vbuffer9[2][index + s] = vdata[2];
			vbuffer9[3][index + s] = vdata[3];
			vbuffer9[4][index + s] = vdata[4];
			vbuffer9[5][index + s] = vdata[5];
			vbuffer9[6][index + s] = vdata[6];
			vbuffer9[7][index + s] = vdata[7];
			vbuffer9[8][index + s] = vdata[8];
			vbuffer9[9][index + s] = vdata[9];
			vbuffer9[10][index + s] = vdata[10];
			vbuffer9[11][index + s] = vdata[11];
			vbuffer9[12][index + s] = vdata[12];
			vbuffer9[13][index + s] = vdata[13];
			vbuffer9[14][index + s] = vdata[14];
			vbuffer9[15][index + s] = vdata[15];
			vbuffer10[0][index + s] = vdata[0];
			vbuffer10[1][index + s] = vdata[1];
			vbuffer10[2][index + s] = vdata[2];
			vbuffer10[3][index + s] = vdata[3];
			vbuffer10[4][index + s] = vdata[4];
			vbuffer10[5][index + s] = vdata[5];
			vbuffer10[6][index + s] = vdata[6];
			vbuffer10[7][index + s] = vdata[7];
			vbuffer10[8][index + s] = vdata[8];
			vbuffer10[9][index + s] = vdata[9];
			vbuffer10[10][index + s] = vdata[10];
			vbuffer10[11][index + s] = vdata[11];
			vbuffer10[12][index + s] = vdata[12];
			vbuffer10[13][index + s] = vdata[13];
			vbuffer10[14][index + s] = vdata[14];
			vbuffer10[15][index + s] = vdata[15];
			vbuffer11[0][index + s] = vdata[0];
			vbuffer11[1][index + s] = vdata[1];
			vbuffer11[2][index + s] = vdata[2];
			vbuffer11[3][index + s] = vdata[3];
			vbuffer11[4][index + s] = vdata[4];
			vbuffer11[5][index + s] = vdata[5];
			vbuffer11[6][index + s] = vdata[6];
			vbuffer11[7][index + s] = vdata[7];
			vbuffer11[8][index + s] = vdata[8];
			vbuffer11[9][index + s] = vdata[9];
			vbuffer11[10][index + s] = vdata[10];
			vbuffer11[11][index + s] = vdata[11];
			vbuffer11[12][index + s] = vdata[12];
			vbuffer11[13][index + s] = vdata[13];
			vbuffer11[14][index + s] = vdata[14];
			vbuffer11[15][index + s] = vdata[15];
			index += NUM_PEs;

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
	}
	// exit(EXIT_SUCCESS); /////////
	return;
}

// -------------------- stats -------------------- //
void acts_all::MEMACCESSP0_readglobalstats(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[BLOCKRAM_GLOBALSTATS_SIZE], batch_type offset_kvs, globalparams_t globalparams){ 
	if(enable == OFF){ return; }
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("readglobalstats", offset_kvs + globalparams.ACTSPARAMS_NUM_PARTITIONS, globalparams.ACTSPARAMS_MAXHBMCAPACITY_KVS + 1, NAp, NAp, NAp);
	#endif
	
	READGLOBALSTATS_LOOP: for (buffer_type i=0; i<1 + globalparams.NUM_REDUCEPARTITIONS; i++){
	#pragma HLS PIPELINE II=1
		uint512_vec_dt vec;
		#ifdef _WIDEWORD
		vec.data[0].key = kvdram[offset_kvs + i].range(31, 0); 
		vec.data[0].value = kvdram[offset_kvs + i].range(63, 32); 
		vec.data[1].key = kvdram[offset_kvs + i].range(95, 64); 
		vec.data[1].value = kvdram[offset_kvs + i].range(127, 96); 
		vec.data[2].key = kvdram[offset_kvs + i].range(159, 128); 
		vec.data[2].value = kvdram[offset_kvs + i].range(191, 160); 
		vec.data[3].key = kvdram[offset_kvs + i].range(223, 192); 
		vec.data[3].value = kvdram[offset_kvs + i].range(255, 224); 
		vec.data[4].key = kvdram[offset_kvs + i].range(287, 256); 
		vec.data[4].value = kvdram[offset_kvs + i].range(319, 288); 
		vec.data[5].key = kvdram[offset_kvs + i].range(351, 320); 
		vec.data[5].value = kvdram[offset_kvs + i].range(383, 352); 
		vec.data[6].key = kvdram[offset_kvs + i].range(415, 384); 
		vec.data[6].value = kvdram[offset_kvs + i].range(447, 416); 
		vec.data[7].key = kvdram[offset_kvs + i].range(479, 448); 
		vec.data[7].value = kvdram[offset_kvs + i].range(511, 480); 
		#else 
		vec.data[0].key = kvdram[offset_kvs + i].data[0].key; 
		vec.data[0].value = kvdram[offset_kvs + i].data[0].value; 
		vec.data[1].key = kvdram[offset_kvs + i].data[1].key; 
		vec.data[1].value = kvdram[offset_kvs + i].data[1].value; 
		vec.data[2].key = kvdram[offset_kvs + i].data[2].key; 
		vec.data[2].value = kvdram[offset_kvs + i].data[2].value; 
		vec.data[3].key = kvdram[offset_kvs + i].data[3].key; 
		vec.data[3].value = kvdram[offset_kvs + i].data[3].value; 
		vec.data[4].key = kvdram[offset_kvs + i].data[4].key; 
		vec.data[4].value = kvdram[offset_kvs + i].data[4].value; 
		vec.data[5].key = kvdram[offset_kvs + i].data[5].key; 
		vec.data[5].value = kvdram[offset_kvs + i].data[5].value; 
		vec.data[6].key = kvdram[offset_kvs + i].data[6].key; 
		vec.data[6].value = kvdram[offset_kvs + i].data[6].value; 
		vec.data[7].key = kvdram[offset_kvs + i].data[7].key; 
		vec.data[7].value = kvdram[offset_kvs + i].data[7].value; 
		#endif 
		globalstatsbuffer[i] = vec.data[globalparams.VARS_WORKBATCH];
	}
	
	#ifdef _DEBUGMODE_CHECKS3
	for (buffer_type i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ actsutilityobj->checkoutofbounds("saveglobalstats", globalstatsbuffer[i].key + globalstatsbuffer[i].value, globalparams.SIZE_KVDRAM, NAp, NAp, NAp); }
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printkeyvalues("mem_access:: readglobalstats.globalstatsbuffer", globalstatsbuffer, 1 + globalparams.NUM_REDUCEPARTITIONS); 
	#endif
	// exit(EXIT_SUCCESS); 
	return;
}

#ifdef NOTUSED____________________________________
unsigned int acts_all::MEMACCESSP0_getvptr(uint512_dt * kvdram, unsigned int baseoffset_kvs, unsigned int offset){
	keyvalue_t vptr_kv;
	
	uint512_dt V = kvdram[baseoffset_kvs + (offset / VECTOR2_SIZE)];
	unsigned int M = (offset % VECTOR2_SIZE) / 2;
	// cout<<"-----------+++++++++++++++++++---MEMACCESSP0_getvptr: baseoffset_kvs: "<<baseoffset_kvs<<", offset: "<<offset<<", baseoffset_kvs + (offset / VECTOR2_SIZE): "<<baseoffset_kvs + (offset / VECTOR2_SIZE)<<endl;
	
	#ifdef _WIDEWORD
 if(M == 0){
		vptr_kv.key = V.range(31, 0); 
		vptr_kv.value = V.range(63, 32); 
	}
else if(M == 1){
		vptr_kv.key = V.range(95, 64); 
		vptr_kv.value = V.range(127, 96); 
	}
else if(M == 2){
		vptr_kv.key = V.range(159, 128); 
		vptr_kv.value = V.range(191, 160); 
	}
else if(M == 3){
		vptr_kv.key = V.range(223, 192); 
		vptr_kv.value = V.range(255, 224); 
	}
else if(M == 4){
		vptr_kv.key = V.range(287, 256); 
		vptr_kv.value = V.range(319, 288); 
	}
else if(M == 5){
		vptr_kv.key = V.range(351, 320); 
		vptr_kv.value = V.range(383, 352); 
	}
else if(M == 6){
		vptr_kv.key = V.range(415, 384); 
		vptr_kv.value = V.range(447, 416); 
	}
else {
		vptr_kv.key = V.range(479, 448); 
		vptr_kv.value = V.range(511, 480); 
	}
	#else 
 if(M == 0){
		vptr_kv.key = V.data[0].key; 
		vptr_kv.value = V.data[0].value; 
	}
else if(M == 1){
		vptr_kv.key = V.data[1].key; 
		vptr_kv.value = V.data[1].value; 
	}
else if(M == 2){
		vptr_kv.key = V.data[2].key; 
		vptr_kv.value = V.data[2].value; 
	}
else if(M == 3){
		vptr_kv.key = V.data[3].key; 
		vptr_kv.value = V.data[3].value; 
	}
else if(M == 4){
		vptr_kv.key = V.data[4].key; 
		vptr_kv.value = V.data[4].value; 
	}
else if(M == 5){
		vptr_kv.key = V.data[5].key; 
		vptr_kv.value = V.data[5].value; 
	}
else if(M == 6){
		vptr_kv.key = V.data[6].key; 
		vptr_kv.value = V.data[6].value; 
	}
else {
		vptr_kv.key = V.data[7].key; 
		vptr_kv.value = V.data[7].value; 
	}
	#endif
	
	if(offset % 2 == 0){ return vptr_kv.key; }
	else { return vptr_kv.value; }
}

tuple_t acts_all::MEMACCESSP0_getvptrs_opt(uint512_dt * kvdram, unsigned int baseoffset_kvs, unsigned int beginoffset, unsigned int endoffset, unsigned int edgebankID, globalparams_t globalparams){
	#pragma HLS INLINE 
	keyy_t beginvptr = 0;
	keyy_t endvptr = 0;
	
	beginoffset = beginoffset / globalparams.ACTSPARAMS_VPTR_SHRINK_RATIO; // convert-to-appropriate-skip-format
	endoffset = endoffset / globalparams.ACTSPARAMS_VPTR_SHRINK_RATIO;
	// cout<<"-----------+++++++++++++++++++---MEMACCESSP0_getvptrs_opt: baseoffset_kvs: "<<baseoffset_kvs<<", beginoffset: "<<beginoffset<<", endoffset: "<<endoffset<<", VPTR_SHRINK_RATIO: "<<NAp<<endl;
	
	beginvptr = MEMACCESSP0_getvptr(kvdram, baseoffset_kvs, beginoffset);
	endvptr = MEMACCESSP0_getvptr(kvdram, baseoffset_kvs, endoffset);

	tuple_t t; t.A = beginvptr; t.B = endvptr;
	return t;
}
#endif 

/* unsigned int acts_all::MEMACCESSP0_getdata(uint512_dt * kvdram, unsigned int baseoffset_kvs, unsigned int loc){
	keyvalue_t data;
	
	uint512_dt V = kvdram[baseoffset_kvs + (loc / VECTOR2_SIZE)];
	unsigned int index = (loc % VECTOR2_SIZE) / 2;
	
	#ifdef _WIDEWORD
 if(index == 0){
		data.key = V.range(31, 0); 
		data.value = V.range(63, 32); 
	}
else if(index == 1){
		data.key = V.range(95, 64); 
		data.value = V.range(127, 96); 
	}
else if(index == 2){
		data.key = V.range(159, 128); 
		data.value = V.range(191, 160); 
	}
else if(index == 3){
		data.key = V.range(223, 192); 
		data.value = V.range(255, 224); 
	}
else if(index == 4){
		data.key = V.range(287, 256); 
		data.value = V.range(319, 288); 
	}
else if(index == 5){
		data.key = V.range(351, 320); 
		data.value = V.range(383, 352); 
	}
else if(index == 6){
		data.key = V.range(415, 384); 
		data.value = V.range(447, 416); 
	}
else {
		data.key = V.range(479, 448); 
		data.value = V.range(511, 480); 
	}
	#else 
 if(index == 0){
		data.key = V.data[0].key; 
		data.value = V.data[0].value; 
	}
else if(index == 1){
		data.key = V.data[1].key; 
		data.value = V.data[1].value; 
	}
else if(index == 2){
		data.key = V.data[2].key; 
		data.value = V.data[2].value; 
	}
else if(index == 3){
		data.key = V.data[3].key; 
		data.value = V.data[3].value; 
	}
else if(index == 4){
		data.key = V.data[4].key; 
		data.value = V.data[4].value; 
	}
else if(index == 5){
		data.key = V.data[5].key; 
		data.value = V.data[5].value; 
	}
else if(index == 6){
		data.key = V.data[6].key; 
		data.value = V.data[6].value; 
	}
else {
		data.key = V.data[7].key; 
		data.value = V.data[7].value; 
	}
	#endif
	
	if(loc % 2 == 0){ return data.key; }
	else { return data.value; }
} */

/* void acts_all::MEMACCESSP0_setdata(uint512_dt * kvdram, unsigned int baseoffset_kvs, unsigned int loc, unsigned int data){
	uint512_dt V = kvdram[baseoffset_kvs + (loc / VECTOR2_SIZE)];
	unsigned int index = (loc % VECTOR2_SIZE);
	
	#ifdef _WIDEWORD
 if(index == 0){
		V.range(31, 0) = data; 
	}
else if(index == 1){
		V.range(63, 32) = data; 
	}
else if(index == 2){
		V.range(95, 64) = data; 
	}
else if(index == 3){
		V.range(127, 96) = data; 
	}
else if(index == 4){
		V.range(159, 128) = data; 
	}
else if(index == 5){
		V.range(191, 160) = data; 
	}
else if(index == 6){
		V.range(223, 192) = data; 
	}
else if(index == 7){
		V.range(255, 224) = data; 
	}
else if(index == 8){
		V.range(287, 256) = data; 
	}
else if(index == 9){
		V.range(319, 288) = data; 
	}
else if(index == 10){
		V.range(351, 320) = data; 
	}
else if(index == 11){
		V.range(383, 352) = data; 
	}
else if(index == 12){
		V.range(415, 384) = data; 
	}
else if(index == 13){
		V.range(447, 416) = data; 
	}
else if(index == 14){
		V.range(479, 448) = data; 
	}
else {
		V.range(511, 480) = data; 
	}
	#else 
	if(index % 2 == 0){ V.data[index/2].key = data; } else { V.data[index/2].value = data; }
	#endif
} */

void acts_all::MEMACCESSP0_commitkvstats(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams){
	unsigned int totalnumpartitionsb4last = 0;
	RETRIEVEKVSTATS_LOOP1: for(unsigned int k=0; k<globalparams.ACTSPARAMS_TREEDEPTH; k++){ totalnumpartitionsb4last += (1 << (globalparams.ACTSPARAMS_POW_PARTITIONS * k)); }
	for(unsigned int k=0; k<totalnumpartitionsb4last; k++){
		#ifdef _WIDEWORD
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].range(63, 32) = buffer[k]; 
		#else
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].data[0].value = buffer[k]; 
		#endif
	}
	
	#ifdef _WIDEWORD
	kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = globalparams.ALGORITHMINFO_GRAPHITERATIONID + 1; // CRITICAL NOTEME: Graph Iteration is incremented here
	#else
	kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = globalparams.ALGORITHMINFO_GRAPHITERATIONID + 1;
	#endif 
	return;
}

// -------------------- multiple accesses -------------------- //
void acts_all::MEMACCESSP0_readhelperstats(uint512_dt * vdram, pmask_dt pmask[BLOCKRAM_CURRPMASK_SIZE], batch_type offset_kvs, batch_type size_kvs, unsigned int GraphIter, globalparams_t globalparams){
	
	value_t datas[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	READMANYPMASKS_LOOP1: for (buffer_type i=0; i<size_kvs; i++){	
	#pragma HLS PIPELINE II=1
		UTILP0_ReadDatas(vdram, offset_kvs + i, datas);
		
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("readhelperstats. ERROR.", GraphIter, MAXNUMGRAPHITERATIONS, NAp, NAp, NAp);
		#endif
		
		pmask[i] = datas[GraphIter];
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<< TIMINGRESULTSCOLOR << ">>> readpmasks: printing active vertex partitions: GraphIter: "<<GraphIter<<": ";
	unsigned int num_actvps = 0;
	for(unsigned int t=0; t<BLOCKRAM_CURRPMASK_SIZE; t++){
		if(pmask[t] > 0  && t < 16){ cout<<t<<", "; }
		if(pmask[t] > 0){ num_actvps += 1; }
	}
	cout<<" ("<<num_actvps<<" active partitions, "<<size_kvs<<" total partitions)"<< RESET << endl;
	#endif 
	return;
}

void acts_all::MEMACCESSP0_savehelperstats(uint512_dt * vdram, pmask_dt pmask[BLOCKRAM_CURRPMASK_SIZE], batch_type offset_kvs, batch_type size_kvs, unsigned int GraphIter, globalparams_t globalparams){
	
	value_t datas[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	READMANYPMASKS_LOOP1: for (buffer_type i=0; i<size_kvs; i++){	
	#pragma HLS PIPELINE II=1
		datas[GraphIter] = pmask[i];
		UTILP0_WriteDatas(vdram, offset_kvs + i, datas);
		
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("readhelperstats. ERROR.", GraphIter, MAXNUMGRAPHITERATIONS, NAp, NAp, NAp);
		#endif
	}
	return;
}









