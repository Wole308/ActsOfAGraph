#define CONFIG_MERGE_VDATAS
#ifndef ALLVERTEXISACTIVE_ALGORITHM
#define CONFIG_MERGE_VPARTITIONS
#endif 

unsigned int acts_all::MERGEP0_actvpstatsoffset(globalparams_t globalparams){
	// this returns the stats of the last level of partitioning
	unsigned int _offset = 0;
	#ifdef ENABLERECURSIVEPARTITIONING
	if(globalparams.ACTSPARAMS_TREEDEPTH > 1){ for(unsigned int k=0; k<globalparams.ACTSPARAMS_TREEDEPTH-1; k++){ _offset += (1 << (NUM_PARTITIONS_POW * k)); }} else { _offset = 1; }
	#else 
	for(unsigned int k=0; k<globalparams.ACTSPARAMS_TREEDEPTH; k++){ _offset += (1 << (NUM_PARTITIONS_POW * k)); }
	#endif
	return _offset;
}

void acts_all::MERGEP0_RearrangeLayoutV(unsigned int s, unsigned int data[VECTOR2_SIZE], unsigned int data2[VECTOR2_SIZE]){
	unsigned int s_ = s % VECTOR2_SIZE;
 if(s_==0){ 
		data2[0] = data[0]; 
		data2[1] = data[1]; 
		data2[2] = data[2]; 
		data2[3] = data[3]; 
		data2[4] = data[4]; 
		data2[5] = data[5]; 
		data2[6] = data[6]; 
		data2[7] = data[7]; 
		data2[8] = data[8]; 
		data2[9] = data[9]; 
		data2[10] = data[10]; 
		data2[11] = data[11]; 
		data2[12] = data[12]; 
		data2[13] = data[13]; 
		data2[14] = data[14]; 
		data2[15] = data[15]; 
	}
else if(s_==1){ 
		data2[15] = data[0]; 
		data2[0] = data[1]; 
		data2[1] = data[2]; 
		data2[2] = data[3]; 
		data2[3] = data[4]; 
		data2[4] = data[5]; 
		data2[5] = data[6]; 
		data2[6] = data[7]; 
		data2[7] = data[8]; 
		data2[8] = data[9]; 
		data2[9] = data[10]; 
		data2[10] = data[11]; 
		data2[11] = data[12]; 
		data2[12] = data[13]; 
		data2[13] = data[14]; 
		data2[14] = data[15]; 
	}
else if(s_==2){ 
		data2[14] = data[0]; 
		data2[15] = data[1]; 
		data2[0] = data[2]; 
		data2[1] = data[3]; 
		data2[2] = data[4]; 
		data2[3] = data[5]; 
		data2[4] = data[6]; 
		data2[5] = data[7]; 
		data2[6] = data[8]; 
		data2[7] = data[9]; 
		data2[8] = data[10]; 
		data2[9] = data[11]; 
		data2[10] = data[12]; 
		data2[11] = data[13]; 
		data2[12] = data[14]; 
		data2[13] = data[15]; 
	}
else if(s_==3){ 
		data2[13] = data[0]; 
		data2[14] = data[1]; 
		data2[15] = data[2]; 
		data2[0] = data[3]; 
		data2[1] = data[4]; 
		data2[2] = data[5]; 
		data2[3] = data[6]; 
		data2[4] = data[7]; 
		data2[5] = data[8]; 
		data2[6] = data[9]; 
		data2[7] = data[10]; 
		data2[8] = data[11]; 
		data2[9] = data[12]; 
		data2[10] = data[13]; 
		data2[11] = data[14]; 
		data2[12] = data[15]; 
	}
else if(s_==4){ 
		data2[12] = data[0]; 
		data2[13] = data[1]; 
		data2[14] = data[2]; 
		data2[15] = data[3]; 
		data2[0] = data[4]; 
		data2[1] = data[5]; 
		data2[2] = data[6]; 
		data2[3] = data[7]; 
		data2[4] = data[8]; 
		data2[5] = data[9]; 
		data2[6] = data[10]; 
		data2[7] = data[11]; 
		data2[8] = data[12]; 
		data2[9] = data[13]; 
		data2[10] = data[14]; 
		data2[11] = data[15]; 
	}
else if(s_==5){ 
		data2[11] = data[0]; 
		data2[12] = data[1]; 
		data2[13] = data[2]; 
		data2[14] = data[3]; 
		data2[15] = data[4]; 
		data2[0] = data[5]; 
		data2[1] = data[6]; 
		data2[2] = data[7]; 
		data2[3] = data[8]; 
		data2[4] = data[9]; 
		data2[5] = data[10]; 
		data2[6] = data[11]; 
		data2[7] = data[12]; 
		data2[8] = data[13]; 
		data2[9] = data[14]; 
		data2[10] = data[15]; 
	}
else if(s_==6){ 
		data2[10] = data[0]; 
		data2[11] = data[1]; 
		data2[12] = data[2]; 
		data2[13] = data[3]; 
		data2[14] = data[4]; 
		data2[15] = data[5]; 
		data2[0] = data[6]; 
		data2[1] = data[7]; 
		data2[2] = data[8]; 
		data2[3] = data[9]; 
		data2[4] = data[10]; 
		data2[5] = data[11]; 
		data2[6] = data[12]; 
		data2[7] = data[13]; 
		data2[8] = data[14]; 
		data2[9] = data[15]; 
	}
else if(s_==7){ 
		data2[9] = data[0]; 
		data2[10] = data[1]; 
		data2[11] = data[2]; 
		data2[12] = data[3]; 
		data2[13] = data[4]; 
		data2[14] = data[5]; 
		data2[15] = data[6]; 
		data2[0] = data[7]; 
		data2[1] = data[8]; 
		data2[2] = data[9]; 
		data2[3] = data[10]; 
		data2[4] = data[11]; 
		data2[5] = data[12]; 
		data2[6] = data[13]; 
		data2[7] = data[14]; 
		data2[8] = data[15]; 
	}
else if(s_==8){ 
		data2[8] = data[0]; 
		data2[9] = data[1]; 
		data2[10] = data[2]; 
		data2[11] = data[3]; 
		data2[12] = data[4]; 
		data2[13] = data[5]; 
		data2[14] = data[6]; 
		data2[15] = data[7]; 
		data2[0] = data[8]; 
		data2[1] = data[9]; 
		data2[2] = data[10]; 
		data2[3] = data[11]; 
		data2[4] = data[12]; 
		data2[5] = data[13]; 
		data2[6] = data[14]; 
		data2[7] = data[15]; 
	}
else if(s_==9){ 
		data2[7] = data[0]; 
		data2[8] = data[1]; 
		data2[9] = data[2]; 
		data2[10] = data[3]; 
		data2[11] = data[4]; 
		data2[12] = data[5]; 
		data2[13] = data[6]; 
		data2[14] = data[7]; 
		data2[15] = data[8]; 
		data2[0] = data[9]; 
		data2[1] = data[10]; 
		data2[2] = data[11]; 
		data2[3] = data[12]; 
		data2[4] = data[13]; 
		data2[5] = data[14]; 
		data2[6] = data[15]; 
	}
else if(s_==10){ 
		data2[6] = data[0]; 
		data2[7] = data[1]; 
		data2[8] = data[2]; 
		data2[9] = data[3]; 
		data2[10] = data[4]; 
		data2[11] = data[5]; 
		data2[12] = data[6]; 
		data2[13] = data[7]; 
		data2[14] = data[8]; 
		data2[15] = data[9]; 
		data2[0] = data[10]; 
		data2[1] = data[11]; 
		data2[2] = data[12]; 
		data2[3] = data[13]; 
		data2[4] = data[14]; 
		data2[5] = data[15]; 
	}
else if(s_==11){ 
		data2[5] = data[0]; 
		data2[6] = data[1]; 
		data2[7] = data[2]; 
		data2[8] = data[3]; 
		data2[9] = data[4]; 
		data2[10] = data[5]; 
		data2[11] = data[6]; 
		data2[12] = data[7]; 
		data2[13] = data[8]; 
		data2[14] = data[9]; 
		data2[15] = data[10]; 
		data2[0] = data[11]; 
		data2[1] = data[12]; 
		data2[2] = data[13]; 
		data2[3] = data[14]; 
		data2[4] = data[15]; 
	}
else if(s_==12){ 
		data2[4] = data[0]; 
		data2[5] = data[1]; 
		data2[6] = data[2]; 
		data2[7] = data[3]; 
		data2[8] = data[4]; 
		data2[9] = data[5]; 
		data2[10] = data[6]; 
		data2[11] = data[7]; 
		data2[12] = data[8]; 
		data2[13] = data[9]; 
		data2[14] = data[10]; 
		data2[15] = data[11]; 
		data2[0] = data[12]; 
		data2[1] = data[13]; 
		data2[2] = data[14]; 
		data2[3] = data[15]; 
	}
else if(s_==13){ 
		data2[3] = data[0]; 
		data2[4] = data[1]; 
		data2[5] = data[2]; 
		data2[6] = data[3]; 
		data2[7] = data[4]; 
		data2[8] = data[5]; 
		data2[9] = data[6]; 
		data2[10] = data[7]; 
		data2[11] = data[8]; 
		data2[12] = data[9]; 
		data2[13] = data[10]; 
		data2[14] = data[11]; 
		data2[15] = data[12]; 
		data2[0] = data[13]; 
		data2[1] = data[14]; 
		data2[2] = data[15]; 
	}
else if(s_==14){ 
		data2[2] = data[0]; 
		data2[3] = data[1]; 
		data2[4] = data[2]; 
		data2[5] = data[3]; 
		data2[6] = data[4]; 
		data2[7] = data[5]; 
		data2[8] = data[6]; 
		data2[9] = data[7]; 
		data2[10] = data[8]; 
		data2[11] = data[9]; 
		data2[12] = data[10]; 
		data2[13] = data[11]; 
		data2[14] = data[12]; 
		data2[15] = data[13]; 
		data2[0] = data[14]; 
		data2[1] = data[15]; 
	}
else { 
		data2[1] = data[0]; 
		data2[2] = data[1]; 
		data2[3] = data[2]; 
		data2[4] = data[3]; 
		data2[5] = data[4]; 
		data2[6] = data[5]; 
		data2[7] = data[6]; 
		data2[8] = data[7]; 
		data2[9] = data[8]; 
		data2[10] = data[9]; 
		data2[11] = data[10]; 
		data2[12] = data[11]; 
		data2[13] = data[12]; 
		data2[14] = data[13]; 
		data2[15] = data[14]; 
		data2[0] = data[15]; 
	}
	return;
}

void acts_all::MERGEP0_RearrangeLayoutA(unsigned int s, unit1_type data[VECTOR2_SIZE], unit1_type data2[VECTOR2_SIZE]){
	unsigned int s_ = s % VECTOR2_SIZE;
 if(s_==0){ 
		data2[0] = data[0]; 
		data2[1] = data[1]; 
		data2[2] = data[2]; 
		data2[3] = data[3]; 
		data2[4] = data[4]; 
		data2[5] = data[5]; 
		data2[6] = data[6]; 
		data2[7] = data[7]; 
		data2[8] = data[8]; 
		data2[9] = data[9]; 
		data2[10] = data[10]; 
		data2[11] = data[11]; 
		data2[12] = data[12]; 
		data2[13] = data[13]; 
		data2[14] = data[14]; 
		data2[15] = data[15]; 
	}
else if(s_==1){ 
		data2[15] = data[0]; 
		data2[0] = data[1]; 
		data2[1] = data[2]; 
		data2[2] = data[3]; 
		data2[3] = data[4]; 
		data2[4] = data[5]; 
		data2[5] = data[6]; 
		data2[6] = data[7]; 
		data2[7] = data[8]; 
		data2[8] = data[9]; 
		data2[9] = data[10]; 
		data2[10] = data[11]; 
		data2[11] = data[12]; 
		data2[12] = data[13]; 
		data2[13] = data[14]; 
		data2[14] = data[15]; 
	}
else if(s_==2){ 
		data2[14] = data[0]; 
		data2[15] = data[1]; 
		data2[0] = data[2]; 
		data2[1] = data[3]; 
		data2[2] = data[4]; 
		data2[3] = data[5]; 
		data2[4] = data[6]; 
		data2[5] = data[7]; 
		data2[6] = data[8]; 
		data2[7] = data[9]; 
		data2[8] = data[10]; 
		data2[9] = data[11]; 
		data2[10] = data[12]; 
		data2[11] = data[13]; 
		data2[12] = data[14]; 
		data2[13] = data[15]; 
	}
else if(s_==3){ 
		data2[13] = data[0]; 
		data2[14] = data[1]; 
		data2[15] = data[2]; 
		data2[0] = data[3]; 
		data2[1] = data[4]; 
		data2[2] = data[5]; 
		data2[3] = data[6]; 
		data2[4] = data[7]; 
		data2[5] = data[8]; 
		data2[6] = data[9]; 
		data2[7] = data[10]; 
		data2[8] = data[11]; 
		data2[9] = data[12]; 
		data2[10] = data[13]; 
		data2[11] = data[14]; 
		data2[12] = data[15]; 
	}
else if(s_==4){ 
		data2[12] = data[0]; 
		data2[13] = data[1]; 
		data2[14] = data[2]; 
		data2[15] = data[3]; 
		data2[0] = data[4]; 
		data2[1] = data[5]; 
		data2[2] = data[6]; 
		data2[3] = data[7]; 
		data2[4] = data[8]; 
		data2[5] = data[9]; 
		data2[6] = data[10]; 
		data2[7] = data[11]; 
		data2[8] = data[12]; 
		data2[9] = data[13]; 
		data2[10] = data[14]; 
		data2[11] = data[15]; 
	}
else if(s_==5){ 
		data2[11] = data[0]; 
		data2[12] = data[1]; 
		data2[13] = data[2]; 
		data2[14] = data[3]; 
		data2[15] = data[4]; 
		data2[0] = data[5]; 
		data2[1] = data[6]; 
		data2[2] = data[7]; 
		data2[3] = data[8]; 
		data2[4] = data[9]; 
		data2[5] = data[10]; 
		data2[6] = data[11]; 
		data2[7] = data[12]; 
		data2[8] = data[13]; 
		data2[9] = data[14]; 
		data2[10] = data[15]; 
	}
else if(s_==6){ 
		data2[10] = data[0]; 
		data2[11] = data[1]; 
		data2[12] = data[2]; 
		data2[13] = data[3]; 
		data2[14] = data[4]; 
		data2[15] = data[5]; 
		data2[0] = data[6]; 
		data2[1] = data[7]; 
		data2[2] = data[8]; 
		data2[3] = data[9]; 
		data2[4] = data[10]; 
		data2[5] = data[11]; 
		data2[6] = data[12]; 
		data2[7] = data[13]; 
		data2[8] = data[14]; 
		data2[9] = data[15]; 
	}
else if(s_==7){ 
		data2[9] = data[0]; 
		data2[10] = data[1]; 
		data2[11] = data[2]; 
		data2[12] = data[3]; 
		data2[13] = data[4]; 
		data2[14] = data[5]; 
		data2[15] = data[6]; 
		data2[0] = data[7]; 
		data2[1] = data[8]; 
		data2[2] = data[9]; 
		data2[3] = data[10]; 
		data2[4] = data[11]; 
		data2[5] = data[12]; 
		data2[6] = data[13]; 
		data2[7] = data[14]; 
		data2[8] = data[15]; 
	}
else if(s_==8){ 
		data2[8] = data[0]; 
		data2[9] = data[1]; 
		data2[10] = data[2]; 
		data2[11] = data[3]; 
		data2[12] = data[4]; 
		data2[13] = data[5]; 
		data2[14] = data[6]; 
		data2[15] = data[7]; 
		data2[0] = data[8]; 
		data2[1] = data[9]; 
		data2[2] = data[10]; 
		data2[3] = data[11]; 
		data2[4] = data[12]; 
		data2[5] = data[13]; 
		data2[6] = data[14]; 
		data2[7] = data[15]; 
	}
else if(s_==9){ 
		data2[7] = data[0]; 
		data2[8] = data[1]; 
		data2[9] = data[2]; 
		data2[10] = data[3]; 
		data2[11] = data[4]; 
		data2[12] = data[5]; 
		data2[13] = data[6]; 
		data2[14] = data[7]; 
		data2[15] = data[8]; 
		data2[0] = data[9]; 
		data2[1] = data[10]; 
		data2[2] = data[11]; 
		data2[3] = data[12]; 
		data2[4] = data[13]; 
		data2[5] = data[14]; 
		data2[6] = data[15]; 
	}
else if(s_==10){ 
		data2[6] = data[0]; 
		data2[7] = data[1]; 
		data2[8] = data[2]; 
		data2[9] = data[3]; 
		data2[10] = data[4]; 
		data2[11] = data[5]; 
		data2[12] = data[6]; 
		data2[13] = data[7]; 
		data2[14] = data[8]; 
		data2[15] = data[9]; 
		data2[0] = data[10]; 
		data2[1] = data[11]; 
		data2[2] = data[12]; 
		data2[3] = data[13]; 
		data2[4] = data[14]; 
		data2[5] = data[15]; 
	}
else if(s_==11){ 
		data2[5] = data[0]; 
		data2[6] = data[1]; 
		data2[7] = data[2]; 
		data2[8] = data[3]; 
		data2[9] = data[4]; 
		data2[10] = data[5]; 
		data2[11] = data[6]; 
		data2[12] = data[7]; 
		data2[13] = data[8]; 
		data2[14] = data[9]; 
		data2[15] = data[10]; 
		data2[0] = data[11]; 
		data2[1] = data[12]; 
		data2[2] = data[13]; 
		data2[3] = data[14]; 
		data2[4] = data[15]; 
	}
else if(s_==12){ 
		data2[4] = data[0]; 
		data2[5] = data[1]; 
		data2[6] = data[2]; 
		data2[7] = data[3]; 
		data2[8] = data[4]; 
		data2[9] = data[5]; 
		data2[10] = data[6]; 
		data2[11] = data[7]; 
		data2[12] = data[8]; 
		data2[13] = data[9]; 
		data2[14] = data[10]; 
		data2[15] = data[11]; 
		data2[0] = data[12]; 
		data2[1] = data[13]; 
		data2[2] = data[14]; 
		data2[3] = data[15]; 
	}
else if(s_==13){ 
		data2[3] = data[0]; 
		data2[4] = data[1]; 
		data2[5] = data[2]; 
		data2[6] = data[3]; 
		data2[7] = data[4]; 
		data2[8] = data[5]; 
		data2[9] = data[6]; 
		data2[10] = data[7]; 
		data2[11] = data[8]; 
		data2[12] = data[9]; 
		data2[13] = data[10]; 
		data2[14] = data[11]; 
		data2[15] = data[12]; 
		data2[0] = data[13]; 
		data2[1] = data[14]; 
		data2[2] = data[15]; 
	}
else if(s_==14){ 
		data2[2] = data[0]; 
		data2[3] = data[1]; 
		data2[4] = data[2]; 
		data2[5] = data[3]; 
		data2[6] = data[4]; 
		data2[7] = data[5]; 
		data2[8] = data[6]; 
		data2[9] = data[7]; 
		data2[10] = data[8]; 
		data2[11] = data[9]; 
		data2[12] = data[10]; 
		data2[13] = data[11]; 
		data2[14] = data[12]; 
		data2[15] = data[13]; 
		data2[0] = data[14]; 
		data2[1] = data[15]; 
	}
else { 
		data2[1] = data[0]; 
		data2[2] = data[1]; 
		data2[3] = data[2]; 
		data2[4] = data[3]; 
		data2[5] = data[4]; 
		data2[6] = data[5]; 
		data2[7] = data[6]; 
		data2[8] = data[7]; 
		data2[9] = data[8]; 
		data2[10] = data[9]; 
		data2[11] = data[10]; 
		data2[12] = data[11]; 
		data2[13] = data[12]; 
		data2[14] = data[13]; 
		data2[15] = data[14]; 
		data2[0] = data[15]; 
	}
	return;
}

void acts_all::MERGEP0_collects(
		unsigned int instanceid,
		unsigned int voffset_kvs,
		unsigned int gvoffset_kvs,
		value_t vdatas[VECTOR2_SIZE],
		keyvalue_t actvvbuffer[VECTOR2_SIZE][ACTVVBUFFER_SIZE],
		unsigned int vpmaskbuffer[BLOCKRAM_CUMMTVPMASK_SIZE],
		unsigned int loadcount[VECTOR2_SIZE],
		unsigned int numactvvs[VECTOR2_SIZE],
		unsigned int local_actvv[VECTOR2_SIZE],
		unsigned int local_actvv2[VECTOR2_SIZE],
		unsigned int global_actvvs[VECTOR2_SIZE],
		uint32_type vpmaskVecSum[VECTOR2_SIZE]
		){
	#ifndef ALLVERTEXISACTIVE_ALGORITHM
	#pragma HLS INLINE
	// collect vertex partition masks for next iteration
	unit1_type vmaskVec[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vmaskVec complete
	
	#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
	// {1st 16 is masks}{2nd 16 is vdatas} (see classname__mem_convert_and_access.cpp)
		uint16_type vmaskVec16[VECTOR2_SIZE];
		#pragma HLS ARRAY_PARTITION variable=vmaskVec16 complete
		vmaskVec16[0] = UTILP0_READBITSFROM_UINTV(vdatas[0], BEGINOFFSETOF_VMASK, 16);
		vmaskVec16[1] = UTILP0_READBITSFROM_UINTV(vdatas[1], BEGINOFFSETOF_VMASK, 16);
		vmaskVec16[2] = UTILP0_READBITSFROM_UINTV(vdatas[2], BEGINOFFSETOF_VMASK, 16);
		vmaskVec16[3] = UTILP0_READBITSFROM_UINTV(vdatas[3], BEGINOFFSETOF_VMASK, 16);
		vmaskVec16[4] = UTILP0_READBITSFROM_UINTV(vdatas[4], BEGINOFFSETOF_VMASK, 16);
		vmaskVec16[5] = UTILP0_READBITSFROM_UINTV(vdatas[5], BEGINOFFSETOF_VMASK, 16);
		vmaskVec16[6] = UTILP0_READBITSFROM_UINTV(vdatas[6], BEGINOFFSETOF_VMASK, 16);
		vmaskVec16[7] = UTILP0_READBITSFROM_UINTV(vdatas[7], BEGINOFFSETOF_VMASK, 16);
		vmaskVec16[8] = UTILP0_READBITSFROM_UINTV(vdatas[8], BEGINOFFSETOF_VMASK, 16);
		vmaskVec16[9] = UTILP0_READBITSFROM_UINTV(vdatas[9], BEGINOFFSETOF_VMASK, 16);
		vmaskVec16[10] = UTILP0_READBITSFROM_UINTV(vdatas[10], BEGINOFFSETOF_VMASK, 16);
		vmaskVec16[11] = UTILP0_READBITSFROM_UINTV(vdatas[11], BEGINOFFSETOF_VMASK, 16);
		vmaskVec16[12] = UTILP0_READBITSFROM_UINTV(vdatas[12], BEGINOFFSETOF_VMASK, 16);
		vmaskVec16[13] = UTILP0_READBITSFROM_UINTV(vdatas[13], BEGINOFFSETOF_VMASK, 16);
		vmaskVec16[14] = UTILP0_READBITSFROM_UINTV(vdatas[14], BEGINOFFSETOF_VMASK, 16);
		vmaskVec16[15] = UTILP0_READBITSFROM_UINTV(vdatas[15], BEGINOFFSETOF_VMASK, 16);
		
		if(vmaskVec16[0] > 0){ vmaskVec[0] = 1; } else{ vmaskVec[0] = 0; }
		if(vmaskVec16[1] > 0){ vmaskVec[1] = 1; } else{ vmaskVec[1] = 0; }
		if(vmaskVec16[2] > 0){ vmaskVec[2] = 1; } else{ vmaskVec[2] = 0; }
		if(vmaskVec16[3] > 0){ vmaskVec[3] = 1; } else{ vmaskVec[3] = 0; }
		if(vmaskVec16[4] > 0){ vmaskVec[4] = 1; } else{ vmaskVec[4] = 0; }
		if(vmaskVec16[5] > 0){ vmaskVec[5] = 1; } else{ vmaskVec[5] = 0; }
		if(vmaskVec16[6] > 0){ vmaskVec[6] = 1; } else{ vmaskVec[6] = 0; }
		if(vmaskVec16[7] > 0){ vmaskVec[7] = 1; } else{ vmaskVec[7] = 0; }
		if(vmaskVec16[8] > 0){ vmaskVec[8] = 1; } else{ vmaskVec[8] = 0; }
		if(vmaskVec16[9] > 0){ vmaskVec[9] = 1; } else{ vmaskVec[9] = 0; }
		if(vmaskVec16[10] > 0){ vmaskVec[10] = 1; } else{ vmaskVec[10] = 0; }
		if(vmaskVec16[11] > 0){ vmaskVec[11] = 1; } else{ vmaskVec[11] = 0; }
		if(vmaskVec16[12] > 0){ vmaskVec[12] = 1; } else{ vmaskVec[12] = 0; }
		if(vmaskVec16[13] > 0){ vmaskVec[13] = 1; } else{ vmaskVec[13] = 0; }
		if(vmaskVec16[14] > 0){ vmaskVec[14] = 1; } else{ vmaskVec[14] = 0; }
		if(vmaskVec16[15] > 0){ vmaskVec[15] = 1; } else{ vmaskVec[15] = 0; }
	#else
		vmaskVec[0] = MEMCAP0_READVMASK(vdatas[0]); 
		vmaskVec[1] = MEMCAP0_READVMASK(vdatas[1]); 
		vmaskVec[2] = MEMCAP0_READVMASK(vdatas[2]); 
		vmaskVec[3] = MEMCAP0_READVMASK(vdatas[3]); 
		vmaskVec[4] = MEMCAP0_READVMASK(vdatas[4]); 
		vmaskVec[5] = MEMCAP0_READVMASK(vdatas[5]); 
		vmaskVec[6] = MEMCAP0_READVMASK(vdatas[6]); 
		vmaskVec[7] = MEMCAP0_READVMASK(vdatas[7]); 
		vmaskVec[8] = MEMCAP0_READVMASK(vdatas[8]); 
		vmaskVec[9] = MEMCAP0_READVMASK(vdatas[9]); 
		vmaskVec[10] = MEMCAP0_READVMASK(vdatas[10]); 
		vmaskVec[11] = MEMCAP0_READVMASK(vdatas[11]); 
		vmaskVec[12] = MEMCAP0_READVMASK(vdatas[12]); 
		vmaskVec[13] = MEMCAP0_READVMASK(vdatas[13]); 
		vmaskVec[14] = MEMCAP0_READVMASK(vdatas[14]); 
		vmaskVec[15] = MEMCAP0_READVMASK(vdatas[15]); 
	#endif 
	
	uint32_type pmaski = 0; unsigned int vid = 0;
	UTILP0_WRITEBITSTO_UINTV(&pmaski, 0, 1, vmaskVec[0]);
	UTILP0_WRITEBITSTO_UINTV(&pmaski, 1, 1, vmaskVec[1]);
	UTILP0_WRITEBITSTO_UINTV(&pmaski, 2, 1, vmaskVec[2]);
	UTILP0_WRITEBITSTO_UINTV(&pmaski, 3, 1, vmaskVec[3]);
	UTILP0_WRITEBITSTO_UINTV(&pmaski, 4, 1, vmaskVec[4]);
	UTILP0_WRITEBITSTO_UINTV(&pmaski, 5, 1, vmaskVec[5]);
	UTILP0_WRITEBITSTO_UINTV(&pmaski, 6, 1, vmaskVec[6]);
	UTILP0_WRITEBITSTO_UINTV(&pmaski, 7, 1, vmaskVec[7]);
	UTILP0_WRITEBITSTO_UINTV(&pmaski, 8, 1, vmaskVec[8]);
	UTILP0_WRITEBITSTO_UINTV(&pmaski, 9, 1, vmaskVec[9]);
	UTILP0_WRITEBITSTO_UINTV(&pmaski, 10, 1, vmaskVec[10]);
	UTILP0_WRITEBITSTO_UINTV(&pmaski, 11, 1, vmaskVec[11]);
	UTILP0_WRITEBITSTO_UINTV(&pmaski, 12, 1, vmaskVec[12]);
	UTILP0_WRITEBITSTO_UINTV(&pmaski, 13, 1, vmaskVec[13]);
	UTILP0_WRITEBITSTO_UINTV(&pmaski, 14, 1, vmaskVec[14]);
	UTILP0_WRITEBITSTO_UINTV(&pmaski, 15, 1, vmaskVec[15]);
	if(pmaski > 0){
		#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS	
		vid = UTILP0_GETREALVID((voffset_kvs * VECTOR2_SIZE * VDATA_SHRINK_RATIO), instanceid);
		#else 	
		vid = UTILP0_GETREALVID((voffset_kvs * VECTOR2_SIZE), instanceid); 
		#endif 	
		
		if(vid >= KVDATA_RANGE){ vid = 0; }
		vpmaskbuffer[vid / PROCESSPARTITIONSZ] = 1;
	}
	
	#ifdef _DEBUGMODE_CHECKS3
	actsutilityobj->checkoutofbounds("MERGEP0_collects: ERROR 20", vid / PROCESSPARTITIONSZ, BLOCKRAM_CUMMTVPMASK_SIZE, pmaski, voffset_kvs, instanceid);
	actsutilityobj->checkoutofbounds("MERGEP0_collects: ERROR 21", vid, KVDATA_RANGE, pmaski, voffset_kvs, instanceid);
	#endif
		
	// collect active vertices
	#ifdef CONFIG_HYBRIDGPMODE
	unit1_type vmaskVec2[VECTOR2_SIZE];
	if(vmaskVec[0] == 1){ local_actvv[0] = (voffset_kvs * VECTOR2_SIZE) + 0; } else{ local_actvv[0] = INVALIDDATA; }; 
	if(vmaskVec[1] == 1){ local_actvv[1] = (voffset_kvs * VECTOR2_SIZE) + 1; } else{ local_actvv[1] = INVALIDDATA; }; 
	if(vmaskVec[2] == 1){ local_actvv[2] = (voffset_kvs * VECTOR2_SIZE) + 2; } else{ local_actvv[2] = INVALIDDATA; }; 
	if(vmaskVec[3] == 1){ local_actvv[3] = (voffset_kvs * VECTOR2_SIZE) + 3; } else{ local_actvv[3] = INVALIDDATA; }; 
	if(vmaskVec[4] == 1){ local_actvv[4] = (voffset_kvs * VECTOR2_SIZE) + 4; } else{ local_actvv[4] = INVALIDDATA; }; 
	if(vmaskVec[5] == 1){ local_actvv[5] = (voffset_kvs * VECTOR2_SIZE) + 5; } else{ local_actvv[5] = INVALIDDATA; }; 
	if(vmaskVec[6] == 1){ local_actvv[6] = (voffset_kvs * VECTOR2_SIZE) + 6; } else{ local_actvv[6] = INVALIDDATA; }; 
	if(vmaskVec[7] == 1){ local_actvv[7] = (voffset_kvs * VECTOR2_SIZE) + 7; } else{ local_actvv[7] = INVALIDDATA; }; 
	if(vmaskVec[8] == 1){ local_actvv[8] = (voffset_kvs * VECTOR2_SIZE) + 8; } else{ local_actvv[8] = INVALIDDATA; }; 
	if(vmaskVec[9] == 1){ local_actvv[9] = (voffset_kvs * VECTOR2_SIZE) + 9; } else{ local_actvv[9] = INVALIDDATA; }; 
	if(vmaskVec[10] == 1){ local_actvv[10] = (voffset_kvs * VECTOR2_SIZE) + 10; } else{ local_actvv[10] = INVALIDDATA; }; 
	if(vmaskVec[11] == 1){ local_actvv[11] = (voffset_kvs * VECTOR2_SIZE) + 11; } else{ local_actvv[11] = INVALIDDATA; }; 
	if(vmaskVec[12] == 1){ local_actvv[12] = (voffset_kvs * VECTOR2_SIZE) + 12; } else{ local_actvv[12] = INVALIDDATA; }; 
	if(vmaskVec[13] == 1){ local_actvv[13] = (voffset_kvs * VECTOR2_SIZE) + 13; } else{ local_actvv[13] = INVALIDDATA; }; 
	if(vmaskVec[14] == 1){ local_actvv[14] = (voffset_kvs * VECTOR2_SIZE) + 14; } else{ local_actvv[14] = INVALIDDATA; }; 
	if(vmaskVec[15] == 1){ local_actvv[15] = (voffset_kvs * VECTOR2_SIZE) + 15; } else{ local_actvv[15] = INVALIDDATA; }; 
	MERGEP0_RearrangeLayoutV(gvoffset_kvs, local_actvv, local_actvv2);
	MERGEP0_RearrangeLayoutA(gvoffset_kvs, vmaskVec, vmaskVec2);
	if(vmaskVec2[0] == 1 && loadcount[0] < ACTVVBUFFER_SIZE){ actvvbuffer[0][loadcount[0]].key = instanceid; actvvbuffer[0][loadcount[0]].value = local_actvv2[0]; loadcount[0] += 1; numactvvs[0] += 1; }
	if(vmaskVec2[1] == 1 && loadcount[1] < ACTVVBUFFER_SIZE){ actvvbuffer[1][loadcount[1]].key = instanceid; actvvbuffer[1][loadcount[1]].value = local_actvv2[1]; loadcount[1] += 1; numactvvs[1] += 1; }
	if(vmaskVec2[2] == 1 && loadcount[2] < ACTVVBUFFER_SIZE){ actvvbuffer[2][loadcount[2]].key = instanceid; actvvbuffer[2][loadcount[2]].value = local_actvv2[2]; loadcount[2] += 1; numactvvs[2] += 1; }
	if(vmaskVec2[3] == 1 && loadcount[3] < ACTVVBUFFER_SIZE){ actvvbuffer[3][loadcount[3]].key = instanceid; actvvbuffer[3][loadcount[3]].value = local_actvv2[3]; loadcount[3] += 1; numactvvs[3] += 1; }
	if(vmaskVec2[4] == 1 && loadcount[4] < ACTVVBUFFER_SIZE){ actvvbuffer[4][loadcount[4]].key = instanceid; actvvbuffer[4][loadcount[4]].value = local_actvv2[4]; loadcount[4] += 1; numactvvs[4] += 1; }
	if(vmaskVec2[5] == 1 && loadcount[5] < ACTVVBUFFER_SIZE){ actvvbuffer[5][loadcount[5]].key = instanceid; actvvbuffer[5][loadcount[5]].value = local_actvv2[5]; loadcount[5] += 1; numactvvs[5] += 1; }
	if(vmaskVec2[6] == 1 && loadcount[6] < ACTVVBUFFER_SIZE){ actvvbuffer[6][loadcount[6]].key = instanceid; actvvbuffer[6][loadcount[6]].value = local_actvv2[6]; loadcount[6] += 1; numactvvs[6] += 1; }
	if(vmaskVec2[7] == 1 && loadcount[7] < ACTVVBUFFER_SIZE){ actvvbuffer[7][loadcount[7]].key = instanceid; actvvbuffer[7][loadcount[7]].value = local_actvv2[7]; loadcount[7] += 1; numactvvs[7] += 1; }
	if(vmaskVec2[8] == 1 && loadcount[8] < ACTVVBUFFER_SIZE){ actvvbuffer[8][loadcount[8]].key = instanceid; actvvbuffer[8][loadcount[8]].value = local_actvv2[8]; loadcount[8] += 1; numactvvs[8] += 1; }
	if(vmaskVec2[9] == 1 && loadcount[9] < ACTVVBUFFER_SIZE){ actvvbuffer[9][loadcount[9]].key = instanceid; actvvbuffer[9][loadcount[9]].value = local_actvv2[9]; loadcount[9] += 1; numactvvs[9] += 1; }
	if(vmaskVec2[10] == 1 && loadcount[10] < ACTVVBUFFER_SIZE){ actvvbuffer[10][loadcount[10]].key = instanceid; actvvbuffer[10][loadcount[10]].value = local_actvv2[10]; loadcount[10] += 1; numactvvs[10] += 1; }
	if(vmaskVec2[11] == 1 && loadcount[11] < ACTVVBUFFER_SIZE){ actvvbuffer[11][loadcount[11]].key = instanceid; actvvbuffer[11][loadcount[11]].value = local_actvv2[11]; loadcount[11] += 1; numactvvs[11] += 1; }
	if(vmaskVec2[12] == 1 && loadcount[12] < ACTVVBUFFER_SIZE){ actvvbuffer[12][loadcount[12]].key = instanceid; actvvbuffer[12][loadcount[12]].value = local_actvv2[12]; loadcount[12] += 1; numactvvs[12] += 1; }
	if(vmaskVec2[13] == 1 && loadcount[13] < ACTVVBUFFER_SIZE){ actvvbuffer[13][loadcount[13]].key = instanceid; actvvbuffer[13][loadcount[13]].value = local_actvv2[13]; loadcount[13] += 1; numactvvs[13] += 1; }
	if(vmaskVec2[14] == 1 && loadcount[14] < ACTVVBUFFER_SIZE){ actvvbuffer[14][loadcount[14]].key = instanceid; actvvbuffer[14][loadcount[14]].value = local_actvv2[14]; loadcount[14] += 1; numactvvs[14] += 1; }
	if(vmaskVec2[15] == 1 && loadcount[15] < ACTVVBUFFER_SIZE){ actvvbuffer[15][loadcount[15]].key = instanceid; actvvbuffer[15][loadcount[15]].value = local_actvv2[15]; loadcount[15] += 1; numactvvs[15] += 1; }
	#endif 
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
	if(vmaskVec[0] == 1){ cout<<"MERGE::EXCHANGE(A):: local_actvv[0]: "<<local_actvv[0]<<endl; }
	if(vmaskVec[1] == 1){ cout<<"MERGE::EXCHANGE(A):: local_actvv[1]: "<<local_actvv[1]<<endl; }
	if(vmaskVec[2] == 1){ cout<<"MERGE::EXCHANGE(A):: local_actvv[2]: "<<local_actvv[2]<<endl; }
	if(vmaskVec[3] == 1){ cout<<"MERGE::EXCHANGE(A):: local_actvv[3]: "<<local_actvv[3]<<endl; }
	if(vmaskVec[4] == 1){ cout<<"MERGE::EXCHANGE(A):: local_actvv[4]: "<<local_actvv[4]<<endl; }
	if(vmaskVec[5] == 1){ cout<<"MERGE::EXCHANGE(A):: local_actvv[5]: "<<local_actvv[5]<<endl; }
	if(vmaskVec[6] == 1){ cout<<"MERGE::EXCHANGE(A):: local_actvv[6]: "<<local_actvv[6]<<endl; }
	if(vmaskVec[7] == 1){ cout<<"MERGE::EXCHANGE(A):: local_actvv[7]: "<<local_actvv[7]<<endl; }
	if(vmaskVec[8] == 1){ cout<<"MERGE::EXCHANGE(A):: local_actvv[8]: "<<local_actvv[8]<<endl; }
	if(vmaskVec[9] == 1){ cout<<"MERGE::EXCHANGE(A):: local_actvv[9]: "<<local_actvv[9]<<endl; }
	if(vmaskVec[10] == 1){ cout<<"MERGE::EXCHANGE(A):: local_actvv[10]: "<<local_actvv[10]<<endl; }
	if(vmaskVec[11] == 1){ cout<<"MERGE::EXCHANGE(A):: local_actvv[11]: "<<local_actvv[11]<<endl; }
	if(vmaskVec[12] == 1){ cout<<"MERGE::EXCHANGE(A):: local_actvv[12]: "<<local_actvv[12]<<endl; }
	if(vmaskVec[13] == 1){ cout<<"MERGE::EXCHANGE(A):: local_actvv[13]: "<<local_actvv[13]<<endl; }
	if(vmaskVec[14] == 1){ cout<<"MERGE::EXCHANGE(A):: local_actvv[14]: "<<local_actvv[14]<<endl; }
	if(vmaskVec[15] == 1){ cout<<"MERGE::EXCHANGE(A):: local_actvv[15]: "<<local_actvv[15]<<endl; }
	#endif
	#endif 
	return;
}

void acts_all::MERGEP0_broadcastVs(uint512_dt * vdram, uint512_dt * kvdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	// return;
	
	globalparams_t globalparams = UTILP0_getglobalparams(kvdram, 0);
	globalparams_t globalparamsv = UTILP0_getglobalparams(vdram, 0);
	unsigned int offset_kvs = globalparams.ACTSPARAMS_INSTID * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::BROADCAST:: broadcasting vertices data from vdram to kvdram."<<endl; 	
	#endif
	MERGEP0_BROADCASTVTXS_LOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){	
		MERGEP0_BROADCASTVTXS_LOOP1B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
		#pragma HLS PIPELINE II=1
			kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k];
		}
	}
	return;
}

void acts_all::MERGEP0_mergeVs(uint512_dt * kvdram, uint512_dt * vdram){
	#if defined(_DEBUGMODE_KERNELPRINTS3)// && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	globalparams_t globalparams = UTILP0_getglobalparams(kvdram, 0);
	globalparams_t globalparamsv = UTILP0_getglobalparams(vdram, 0);
	
	keyvalue_t globalstatsbuffer[BLOCKRAM_GLOBALSTATS_SIZE];
	unsigned int _offset = MERGEP0_actvpstatsoffset(globalparams);
	MEMACCESSP0_retreievekvstats(kvdram, globalstatsbuffer, globalparams, _offset, globalparams.NUM_REDUCEPARTITIONS);
	
	unsigned int i = globalparams.ACTSPARAMS_INSTID % NUMCOMPUTEUNITS_SLR2;
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	cout<<"MERGEP0_mergeVs:: globalparams.ACTSPARAMS_INSTID: "<<globalparams.ACTSPARAMS_INSTID<<", i: "<<i<<endl;
	#endif 
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	
	// expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
	unsigned int _NUM_VALIDREDUCEPARTITIONS = (globalparams.NUM_REDUCEPARTITIONS + (VDATA_SHRINK_RATIO - 1)) / VDATA_SHRINK_RATIO;
	for(unsigned int reduce_partition=0; reduce_partition<globalparams.NUM_REDUCEPARTITIONS; reduce_partition++){
		if(reduce_partition >= _NUM_VALIDREDUCEPARTITIONS){ globalstatsbuffer[reduce_partition].value = 0; }
	}
	#endif 	
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	cout<<"Printing active global stats..."<<endl;
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer[i].value > 0){ cout<<"MERGE: globalstatsbuffer["<<i<<"].key : "<<globalstatsbuffer[i].key<<", globalstatsbuffer["<<i<<"].value : "<<globalstatsbuffer[i].value<<endl; }}
	#endif 
	
	bool pass3 = false; // true;
	
	// vertices data
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 	
	#endif 
	unsigned int voffset_kvs2 = i * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	unsigned int voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		// unsigned int voffset_kvs2 = (i * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION) + (partition * globalparams.SIZEKVS2_REDUCEPARTITION);
		// unsigned int voffseti_kvs2 = partition * globalparams.SIZEKVS2_REDUCEPARTITION;
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices :: [instance "<<globalparams.ACTSPARAMS_INSTID<<", partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3 == true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices :: [instance "<<globalparams.ACTSPARAMS_INSTID<<", partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOPB: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	
	// active reduce partitions
	unsigned int offset_kvs = i * globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer[reduce_partition].value);
	}
	// exit(EXIT_SUCCESS); // 
	return;
}

void acts_all::MERGEP0_mergeVs1(uint512_dt * kvdram0, uint512_dt * vdram, 
keyvalue_t globalstatsbuffer0[BLOCKRAM_GLOBALSTATS_SIZE],			globalparams_t globalparams, globalparams_t globalparamsv){
	#if defined(_DEBUGMODE_KERNELPRINTS3)// && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	// unsigned int actvpstats_beginoffset = MERGEP0_actvpstatsoffset(globalparamsv);
	
	// expensive variables 
	unsigned int lbasevoffset_kvs2 = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
	unsigned int _NUM_VALIDREDUCEPARTITIONS = (globalparams.NUM_REDUCEPARTITIONS + (VDATA_SHRINK_RATIO - 1)) / VDATA_SHRINK_RATIO;
	for(unsigned int reduce_partition=0; reduce_partition<globalparams.NUM_REDUCEPARTITIONS; reduce_partition++){
		if(reduce_partition >= _NUM_VALIDREDUCEPARTITIONS){ 
			globalstatsbuffer0[reduce_partition].value = 0; 
		}
	}
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
	cout<<"Printing active global stats..."<<endl;
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer0[i].value > 0){ cout<<"MERGE: globalstatsbuffer0["<<i<<"].key : "<<globalstatsbuffer0[i].key<<", globalstatsbuffer0["<<i<<"].value : "<<globalstatsbuffer0[i].value<<endl; }}
	#endif 
	
	bool pass3 = false; // true;
	
	// vertices data
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 1:: [instance 0, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs1: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer0[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 1:: [instance 0, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP0B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs1---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs1: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	
	// active reduce partitions
	unsigned int offset_kvs = 0;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs1: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer0[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	// exit(EXIT_SUCCESS); // 
	return;
}
void acts_all::MERGEP0_mergeVs2(uint512_dt * kvdram0,uint512_dt * kvdram1, uint512_dt * vdram, 
keyvalue_t globalstatsbuffer0[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_GLOBALSTATS_SIZE],			globalparams_t globalparams, globalparams_t globalparamsv){
	#if defined(_DEBUGMODE_KERNELPRINTS3)// && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	// unsigned int actvpstats_beginoffset = MERGEP0_actvpstatsoffset(globalparamsv);
	
	// expensive variables 
	unsigned int lbasevoffset_kvs2 = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
	unsigned int _NUM_VALIDREDUCEPARTITIONS = (globalparams.NUM_REDUCEPARTITIONS + (VDATA_SHRINK_RATIO - 1)) / VDATA_SHRINK_RATIO;
	for(unsigned int reduce_partition=0; reduce_partition<globalparams.NUM_REDUCEPARTITIONS; reduce_partition++){
		if(reduce_partition >= _NUM_VALIDREDUCEPARTITIONS){ 
			globalstatsbuffer0[reduce_partition].value = 0; 
			globalstatsbuffer1[reduce_partition].value = 0; 
		}
	}
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
	cout<<"Printing active global stats..."<<endl;
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer0[i].value > 0){ cout<<"MERGE: globalstatsbuffer0["<<i<<"].key : "<<globalstatsbuffer0[i].key<<", globalstatsbuffer0["<<i<<"].value : "<<globalstatsbuffer0[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer1[i].value > 0){ cout<<"MERGE: globalstatsbuffer1["<<i<<"].key : "<<globalstatsbuffer1[i].key<<", globalstatsbuffer1["<<i<<"].value : "<<globalstatsbuffer1[i].value<<endl; }}
	#endif 
	
	bool pass3 = false; // true;
	
	// vertices data
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 2:: [instance 0, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs2: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer0[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 2:: [instance 0, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP0B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs2---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs2: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 2:: [instance 1, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs2: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer1[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 2:: [instance 1, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP1B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs2---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs2: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	
	// active reduce partitions
	unsigned int offset_kvs = 0;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs2: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer0[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs2: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer1[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	// exit(EXIT_SUCCESS); // 
	return;
}
void acts_all::MERGEP0_mergeVs3(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2, uint512_dt * vdram, 
keyvalue_t globalstatsbuffer0[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_GLOBALSTATS_SIZE],			globalparams_t globalparams, globalparams_t globalparamsv){
	#if defined(_DEBUGMODE_KERNELPRINTS3)// && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	// unsigned int actvpstats_beginoffset = MERGEP0_actvpstatsoffset(globalparamsv);
	
	// expensive variables 
	unsigned int lbasevoffset_kvs2 = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
	unsigned int _NUM_VALIDREDUCEPARTITIONS = (globalparams.NUM_REDUCEPARTITIONS + (VDATA_SHRINK_RATIO - 1)) / VDATA_SHRINK_RATIO;
	for(unsigned int reduce_partition=0; reduce_partition<globalparams.NUM_REDUCEPARTITIONS; reduce_partition++){
		if(reduce_partition >= _NUM_VALIDREDUCEPARTITIONS){ 
			globalstatsbuffer0[reduce_partition].value = 0; 
			globalstatsbuffer1[reduce_partition].value = 0; 
			globalstatsbuffer2[reduce_partition].value = 0; 
		}
	}
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
	cout<<"Printing active global stats..."<<endl;
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer0[i].value > 0){ cout<<"MERGE: globalstatsbuffer0["<<i<<"].key : "<<globalstatsbuffer0[i].key<<", globalstatsbuffer0["<<i<<"].value : "<<globalstatsbuffer0[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer1[i].value > 0){ cout<<"MERGE: globalstatsbuffer1["<<i<<"].key : "<<globalstatsbuffer1[i].key<<", globalstatsbuffer1["<<i<<"].value : "<<globalstatsbuffer1[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer2[i].value > 0){ cout<<"MERGE: globalstatsbuffer2["<<i<<"].key : "<<globalstatsbuffer2[i].key<<", globalstatsbuffer2["<<i<<"].value : "<<globalstatsbuffer2[i].value<<endl; }}
	#endif 
	
	bool pass3 = false; // true;
	
	// vertices data
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 3:: [instance 0, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs3: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer0[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 3:: [instance 0, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP0B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs3---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs3: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 3:: [instance 1, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs3: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer1[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 3:: [instance 1, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP1B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs3---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs3: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 3:: [instance 2, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs3: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer2[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 3:: [instance 2, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP2B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs3---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs3: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	
	// active reduce partitions
	unsigned int offset_kvs = 0;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs3: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer0[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs3: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer1[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs3: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer2[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	// exit(EXIT_SUCCESS); // 
	return;
}
void acts_all::MERGEP0_mergeVs4(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3, uint512_dt * vdram, 
keyvalue_t globalstatsbuffer0[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_GLOBALSTATS_SIZE],			globalparams_t globalparams, globalparams_t globalparamsv){
	#if defined(_DEBUGMODE_KERNELPRINTS3)// && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	// unsigned int actvpstats_beginoffset = MERGEP0_actvpstatsoffset(globalparamsv);
	
	// expensive variables 
	unsigned int lbasevoffset_kvs2 = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
	unsigned int _NUM_VALIDREDUCEPARTITIONS = (globalparams.NUM_REDUCEPARTITIONS + (VDATA_SHRINK_RATIO - 1)) / VDATA_SHRINK_RATIO;
	for(unsigned int reduce_partition=0; reduce_partition<globalparams.NUM_REDUCEPARTITIONS; reduce_partition++){
		if(reduce_partition >= _NUM_VALIDREDUCEPARTITIONS){ 
			globalstatsbuffer0[reduce_partition].value = 0; 
			globalstatsbuffer1[reduce_partition].value = 0; 
			globalstatsbuffer2[reduce_partition].value = 0; 
			globalstatsbuffer3[reduce_partition].value = 0; 
		}
	}
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
	cout<<"Printing active global stats..."<<endl;
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer0[i].value > 0){ cout<<"MERGE: globalstatsbuffer0["<<i<<"].key : "<<globalstatsbuffer0[i].key<<", globalstatsbuffer0["<<i<<"].value : "<<globalstatsbuffer0[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer1[i].value > 0){ cout<<"MERGE: globalstatsbuffer1["<<i<<"].key : "<<globalstatsbuffer1[i].key<<", globalstatsbuffer1["<<i<<"].value : "<<globalstatsbuffer1[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer2[i].value > 0){ cout<<"MERGE: globalstatsbuffer2["<<i<<"].key : "<<globalstatsbuffer2[i].key<<", globalstatsbuffer2["<<i<<"].value : "<<globalstatsbuffer2[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer3[i].value > 0){ cout<<"MERGE: globalstatsbuffer3["<<i<<"].key : "<<globalstatsbuffer3[i].key<<", globalstatsbuffer3["<<i<<"].value : "<<globalstatsbuffer3[i].value<<endl; }}
	#endif 
	
	bool pass3 = false; // true;
	
	// vertices data
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 4:: [instance 0, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs4: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer0[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 4:: [instance 0, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP0B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs4---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs4: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 4:: [instance 1, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs4: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer1[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 4:: [instance 1, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP1B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs4---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs4: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 4:: [instance 2, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs4: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer2[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 4:: [instance 2, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP2B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs4---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs4: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 4:: [instance 3, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs4: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer3[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 4:: [instance 3, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP3B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs4---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs4: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	
	// active reduce partitions
	unsigned int offset_kvs = 0;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs4: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer0[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs4: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer1[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs4: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer2[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs4: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer3[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	// exit(EXIT_SUCCESS); // 
	return;
}
void acts_all::MERGEP0_mergeVs5(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4, uint512_dt * vdram, 
keyvalue_t globalstatsbuffer0[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer4[BLOCKRAM_GLOBALSTATS_SIZE],			globalparams_t globalparams, globalparams_t globalparamsv){
	#if defined(_DEBUGMODE_KERNELPRINTS3)// && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	// unsigned int actvpstats_beginoffset = MERGEP0_actvpstatsoffset(globalparamsv);
	
	// expensive variables 
	unsigned int lbasevoffset_kvs2 = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
	unsigned int _NUM_VALIDREDUCEPARTITIONS = (globalparams.NUM_REDUCEPARTITIONS + (VDATA_SHRINK_RATIO - 1)) / VDATA_SHRINK_RATIO;
	for(unsigned int reduce_partition=0; reduce_partition<globalparams.NUM_REDUCEPARTITIONS; reduce_partition++){
		if(reduce_partition >= _NUM_VALIDREDUCEPARTITIONS){ 
			globalstatsbuffer0[reduce_partition].value = 0; 
			globalstatsbuffer1[reduce_partition].value = 0; 
			globalstatsbuffer2[reduce_partition].value = 0; 
			globalstatsbuffer3[reduce_partition].value = 0; 
			globalstatsbuffer4[reduce_partition].value = 0; 
		}
	}
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
	cout<<"Printing active global stats..."<<endl;
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer0[i].value > 0){ cout<<"MERGE: globalstatsbuffer0["<<i<<"].key : "<<globalstatsbuffer0[i].key<<", globalstatsbuffer0["<<i<<"].value : "<<globalstatsbuffer0[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer1[i].value > 0){ cout<<"MERGE: globalstatsbuffer1["<<i<<"].key : "<<globalstatsbuffer1[i].key<<", globalstatsbuffer1["<<i<<"].value : "<<globalstatsbuffer1[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer2[i].value > 0){ cout<<"MERGE: globalstatsbuffer2["<<i<<"].key : "<<globalstatsbuffer2[i].key<<", globalstatsbuffer2["<<i<<"].value : "<<globalstatsbuffer2[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer3[i].value > 0){ cout<<"MERGE: globalstatsbuffer3["<<i<<"].key : "<<globalstatsbuffer3[i].key<<", globalstatsbuffer3["<<i<<"].value : "<<globalstatsbuffer3[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer4[i].value > 0){ cout<<"MERGE: globalstatsbuffer4["<<i<<"].key : "<<globalstatsbuffer4[i].key<<", globalstatsbuffer4["<<i<<"].value : "<<globalstatsbuffer4[i].value<<endl; }}
	#endif 
	
	bool pass3 = false; // true;
	
	// vertices data
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 5:: [instance 0, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs5: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer0[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 5:: [instance 0, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP0B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs5---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs5: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 5:: [instance 1, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs5: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer1[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 5:: [instance 1, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP1B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs5---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs5: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 5:: [instance 2, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs5: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer2[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 5:: [instance 2, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP2B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs5---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs5: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 5:: [instance 3, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs5: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer3[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 5:: [instance 3, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP3B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs5---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs5: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 5:: [instance 4, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs5: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer4[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 5:: [instance 4, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP4B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs5---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs5: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	
	// active reduce partitions
	unsigned int offset_kvs = 0;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs5: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer0[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs5: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer1[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs5: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer2[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs5: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer3[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs5: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer4[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	// exit(EXIT_SUCCESS); // 
	return;
}
void acts_all::MERGEP0_mergeVs6(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5, uint512_dt * vdram, 
keyvalue_t globalstatsbuffer0[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer4[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer5[BLOCKRAM_GLOBALSTATS_SIZE],			globalparams_t globalparams, globalparams_t globalparamsv){
	#if defined(_DEBUGMODE_KERNELPRINTS3)// && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	// unsigned int actvpstats_beginoffset = MERGEP0_actvpstatsoffset(globalparamsv);
	
	// expensive variables 
	unsigned int lbasevoffset_kvs2 = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
	unsigned int _NUM_VALIDREDUCEPARTITIONS = (globalparams.NUM_REDUCEPARTITIONS + (VDATA_SHRINK_RATIO - 1)) / VDATA_SHRINK_RATIO;
	for(unsigned int reduce_partition=0; reduce_partition<globalparams.NUM_REDUCEPARTITIONS; reduce_partition++){
		if(reduce_partition >= _NUM_VALIDREDUCEPARTITIONS){ 
			globalstatsbuffer0[reduce_partition].value = 0; 
			globalstatsbuffer1[reduce_partition].value = 0; 
			globalstatsbuffer2[reduce_partition].value = 0; 
			globalstatsbuffer3[reduce_partition].value = 0; 
			globalstatsbuffer4[reduce_partition].value = 0; 
			globalstatsbuffer5[reduce_partition].value = 0; 
		}
	}
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
	cout<<"Printing active global stats..."<<endl;
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer0[i].value > 0){ cout<<"MERGE: globalstatsbuffer0["<<i<<"].key : "<<globalstatsbuffer0[i].key<<", globalstatsbuffer0["<<i<<"].value : "<<globalstatsbuffer0[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer1[i].value > 0){ cout<<"MERGE: globalstatsbuffer1["<<i<<"].key : "<<globalstatsbuffer1[i].key<<", globalstatsbuffer1["<<i<<"].value : "<<globalstatsbuffer1[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer2[i].value > 0){ cout<<"MERGE: globalstatsbuffer2["<<i<<"].key : "<<globalstatsbuffer2[i].key<<", globalstatsbuffer2["<<i<<"].value : "<<globalstatsbuffer2[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer3[i].value > 0){ cout<<"MERGE: globalstatsbuffer3["<<i<<"].key : "<<globalstatsbuffer3[i].key<<", globalstatsbuffer3["<<i<<"].value : "<<globalstatsbuffer3[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer4[i].value > 0){ cout<<"MERGE: globalstatsbuffer4["<<i<<"].key : "<<globalstatsbuffer4[i].key<<", globalstatsbuffer4["<<i<<"].value : "<<globalstatsbuffer4[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer5[i].value > 0){ cout<<"MERGE: globalstatsbuffer5["<<i<<"].key : "<<globalstatsbuffer5[i].key<<", globalstatsbuffer5["<<i<<"].value : "<<globalstatsbuffer5[i].value<<endl; }}
	#endif 
	
	bool pass3 = false; // true;
	
	// vertices data
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 6:: [instance 0, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer0[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 6:: [instance 0, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP0B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 6:: [instance 1, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer1[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 6:: [instance 1, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP1B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 6:: [instance 2, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer2[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 6:: [instance 2, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP2B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 6:: [instance 3, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer3[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 6:: [instance 3, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP3B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 6:: [instance 4, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer4[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 6:: [instance 4, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP4B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 6:: [instance 5, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer5[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 6:: [instance 5, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP5B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	
	// active reduce partitions
	unsigned int offset_kvs = 0;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer0[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer1[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer2[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer3[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer4[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer5[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	// exit(EXIT_SUCCESS); // 
	return;
}
void acts_all::MERGEP0_mergeVs7(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6, uint512_dt * vdram, 
keyvalue_t globalstatsbuffer0[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer4[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer5[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer6[BLOCKRAM_GLOBALSTATS_SIZE],			globalparams_t globalparams, globalparams_t globalparamsv){
	#if defined(_DEBUGMODE_KERNELPRINTS3)// && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	// unsigned int actvpstats_beginoffset = MERGEP0_actvpstatsoffset(globalparamsv);
	
	// expensive variables 
	unsigned int lbasevoffset_kvs2 = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
	unsigned int _NUM_VALIDREDUCEPARTITIONS = (globalparams.NUM_REDUCEPARTITIONS + (VDATA_SHRINK_RATIO - 1)) / VDATA_SHRINK_RATIO;
	for(unsigned int reduce_partition=0; reduce_partition<globalparams.NUM_REDUCEPARTITIONS; reduce_partition++){
		if(reduce_partition >= _NUM_VALIDREDUCEPARTITIONS){ 
			globalstatsbuffer0[reduce_partition].value = 0; 
			globalstatsbuffer1[reduce_partition].value = 0; 
			globalstatsbuffer2[reduce_partition].value = 0; 
			globalstatsbuffer3[reduce_partition].value = 0; 
			globalstatsbuffer4[reduce_partition].value = 0; 
			globalstatsbuffer5[reduce_partition].value = 0; 
			globalstatsbuffer6[reduce_partition].value = 0; 
		}
	}
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
	cout<<"Printing active global stats..."<<endl;
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer0[i].value > 0){ cout<<"MERGE: globalstatsbuffer0["<<i<<"].key : "<<globalstatsbuffer0[i].key<<", globalstatsbuffer0["<<i<<"].value : "<<globalstatsbuffer0[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer1[i].value > 0){ cout<<"MERGE: globalstatsbuffer1["<<i<<"].key : "<<globalstatsbuffer1[i].key<<", globalstatsbuffer1["<<i<<"].value : "<<globalstatsbuffer1[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer2[i].value > 0){ cout<<"MERGE: globalstatsbuffer2["<<i<<"].key : "<<globalstatsbuffer2[i].key<<", globalstatsbuffer2["<<i<<"].value : "<<globalstatsbuffer2[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer3[i].value > 0){ cout<<"MERGE: globalstatsbuffer3["<<i<<"].key : "<<globalstatsbuffer3[i].key<<", globalstatsbuffer3["<<i<<"].value : "<<globalstatsbuffer3[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer4[i].value > 0){ cout<<"MERGE: globalstatsbuffer4["<<i<<"].key : "<<globalstatsbuffer4[i].key<<", globalstatsbuffer4["<<i<<"].value : "<<globalstatsbuffer4[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer5[i].value > 0){ cout<<"MERGE: globalstatsbuffer5["<<i<<"].key : "<<globalstatsbuffer5[i].key<<", globalstatsbuffer5["<<i<<"].value : "<<globalstatsbuffer5[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer6[i].value > 0){ cout<<"MERGE: globalstatsbuffer6["<<i<<"].key : "<<globalstatsbuffer6[i].key<<", globalstatsbuffer6["<<i<<"].value : "<<globalstatsbuffer6[i].value<<endl; }}
	#endif 
	
	bool pass3 = false; // true;
	
	// vertices data
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 7:: [instance 0, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer0[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 7:: [instance 0, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP0B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 7:: [instance 1, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer1[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 7:: [instance 1, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP1B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 7:: [instance 2, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer2[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 7:: [instance 2, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP2B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 7:: [instance 3, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer3[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 7:: [instance 3, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP3B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 7:: [instance 4, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer4[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 7:: [instance 4, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP4B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 7:: [instance 5, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer5[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 7:: [instance 5, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP5B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 7:: [instance 6, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer6[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 7:: [instance 6, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP6B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	
	// active reduce partitions
	unsigned int offset_kvs = 0;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer0[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer1[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer2[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer3[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer4[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer5[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer6[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	// exit(EXIT_SUCCESS); // 
	return;
}
void acts_all::MERGEP0_mergeVs8(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7, uint512_dt * vdram, 
keyvalue_t globalstatsbuffer0[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer4[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer5[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer6[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer7[BLOCKRAM_GLOBALSTATS_SIZE],			globalparams_t globalparams, globalparams_t globalparamsv){
	#if defined(_DEBUGMODE_KERNELPRINTS3)// && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	// unsigned int actvpstats_beginoffset = MERGEP0_actvpstatsoffset(globalparamsv);
	
	// expensive variables 
	unsigned int lbasevoffset_kvs2 = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
	unsigned int _NUM_VALIDREDUCEPARTITIONS = (globalparams.NUM_REDUCEPARTITIONS + (VDATA_SHRINK_RATIO - 1)) / VDATA_SHRINK_RATIO;
	for(unsigned int reduce_partition=0; reduce_partition<globalparams.NUM_REDUCEPARTITIONS; reduce_partition++){
		if(reduce_partition >= _NUM_VALIDREDUCEPARTITIONS){ 
			globalstatsbuffer0[reduce_partition].value = 0; 
			globalstatsbuffer1[reduce_partition].value = 0; 
			globalstatsbuffer2[reduce_partition].value = 0; 
			globalstatsbuffer3[reduce_partition].value = 0; 
			globalstatsbuffer4[reduce_partition].value = 0; 
			globalstatsbuffer5[reduce_partition].value = 0; 
			globalstatsbuffer6[reduce_partition].value = 0; 
			globalstatsbuffer7[reduce_partition].value = 0; 
		}
	}
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
	cout<<"Printing active global stats..."<<endl;
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer0[i].value > 0){ cout<<"MERGE: globalstatsbuffer0["<<i<<"].key : "<<globalstatsbuffer0[i].key<<", globalstatsbuffer0["<<i<<"].value : "<<globalstatsbuffer0[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer1[i].value > 0){ cout<<"MERGE: globalstatsbuffer1["<<i<<"].key : "<<globalstatsbuffer1[i].key<<", globalstatsbuffer1["<<i<<"].value : "<<globalstatsbuffer1[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer2[i].value > 0){ cout<<"MERGE: globalstatsbuffer2["<<i<<"].key : "<<globalstatsbuffer2[i].key<<", globalstatsbuffer2["<<i<<"].value : "<<globalstatsbuffer2[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer3[i].value > 0){ cout<<"MERGE: globalstatsbuffer3["<<i<<"].key : "<<globalstatsbuffer3[i].key<<", globalstatsbuffer3["<<i<<"].value : "<<globalstatsbuffer3[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer4[i].value > 0){ cout<<"MERGE: globalstatsbuffer4["<<i<<"].key : "<<globalstatsbuffer4[i].key<<", globalstatsbuffer4["<<i<<"].value : "<<globalstatsbuffer4[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer5[i].value > 0){ cout<<"MERGE: globalstatsbuffer5["<<i<<"].key : "<<globalstatsbuffer5[i].key<<", globalstatsbuffer5["<<i<<"].value : "<<globalstatsbuffer5[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer6[i].value > 0){ cout<<"MERGE: globalstatsbuffer6["<<i<<"].key : "<<globalstatsbuffer6[i].key<<", globalstatsbuffer6["<<i<<"].value : "<<globalstatsbuffer6[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer7[i].value > 0){ cout<<"MERGE: globalstatsbuffer7["<<i<<"].key : "<<globalstatsbuffer7[i].key<<", globalstatsbuffer7["<<i<<"].value : "<<globalstatsbuffer7[i].value<<endl; }}
	#endif 
	
	bool pass3 = false; // true;
	
	// vertices data
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 8:: [instance 0, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer0[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 8:: [instance 0, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP0B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 8:: [instance 1, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer1[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 8:: [instance 1, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP1B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 8:: [instance 2, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer2[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 8:: [instance 2, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP2B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 8:: [instance 3, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer3[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 8:: [instance 3, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP3B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 8:: [instance 4, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer4[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 8:: [instance 4, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP4B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 8:: [instance 5, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer5[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 8:: [instance 5, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP5B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 8:: [instance 6, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer6[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 8:: [instance 6, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP6B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP7: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 8:: [instance 7, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer7[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 8:: [instance 7, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP7B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram7[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	
	// active reduce partitions
	unsigned int offset_kvs = 0;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer0[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer1[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer2[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer3[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer4[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer5[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer6[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer7[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	// exit(EXIT_SUCCESS); // 
	return;
}
void acts_all::MERGEP0_mergeVs9(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8, uint512_dt * vdram, 
keyvalue_t globalstatsbuffer0[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer4[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer5[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer6[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer7[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer8[BLOCKRAM_GLOBALSTATS_SIZE],			globalparams_t globalparams, globalparams_t globalparamsv){
	#if defined(_DEBUGMODE_KERNELPRINTS3)// && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	// unsigned int actvpstats_beginoffset = MERGEP0_actvpstatsoffset(globalparamsv);
	
	// expensive variables 
	unsigned int lbasevoffset_kvs2 = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
	unsigned int _NUM_VALIDREDUCEPARTITIONS = (globalparams.NUM_REDUCEPARTITIONS + (VDATA_SHRINK_RATIO - 1)) / VDATA_SHRINK_RATIO;
	for(unsigned int reduce_partition=0; reduce_partition<globalparams.NUM_REDUCEPARTITIONS; reduce_partition++){
		if(reduce_partition >= _NUM_VALIDREDUCEPARTITIONS){ 
			globalstatsbuffer0[reduce_partition].value = 0; 
			globalstatsbuffer1[reduce_partition].value = 0; 
			globalstatsbuffer2[reduce_partition].value = 0; 
			globalstatsbuffer3[reduce_partition].value = 0; 
			globalstatsbuffer4[reduce_partition].value = 0; 
			globalstatsbuffer5[reduce_partition].value = 0; 
			globalstatsbuffer6[reduce_partition].value = 0; 
			globalstatsbuffer7[reduce_partition].value = 0; 
			globalstatsbuffer8[reduce_partition].value = 0; 
		}
	}
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
	cout<<"Printing active global stats..."<<endl;
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer0[i].value > 0){ cout<<"MERGE: globalstatsbuffer0["<<i<<"].key : "<<globalstatsbuffer0[i].key<<", globalstatsbuffer0["<<i<<"].value : "<<globalstatsbuffer0[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer1[i].value > 0){ cout<<"MERGE: globalstatsbuffer1["<<i<<"].key : "<<globalstatsbuffer1[i].key<<", globalstatsbuffer1["<<i<<"].value : "<<globalstatsbuffer1[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer2[i].value > 0){ cout<<"MERGE: globalstatsbuffer2["<<i<<"].key : "<<globalstatsbuffer2[i].key<<", globalstatsbuffer2["<<i<<"].value : "<<globalstatsbuffer2[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer3[i].value > 0){ cout<<"MERGE: globalstatsbuffer3["<<i<<"].key : "<<globalstatsbuffer3[i].key<<", globalstatsbuffer3["<<i<<"].value : "<<globalstatsbuffer3[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer4[i].value > 0){ cout<<"MERGE: globalstatsbuffer4["<<i<<"].key : "<<globalstatsbuffer4[i].key<<", globalstatsbuffer4["<<i<<"].value : "<<globalstatsbuffer4[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer5[i].value > 0){ cout<<"MERGE: globalstatsbuffer5["<<i<<"].key : "<<globalstatsbuffer5[i].key<<", globalstatsbuffer5["<<i<<"].value : "<<globalstatsbuffer5[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer6[i].value > 0){ cout<<"MERGE: globalstatsbuffer6["<<i<<"].key : "<<globalstatsbuffer6[i].key<<", globalstatsbuffer6["<<i<<"].value : "<<globalstatsbuffer6[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer7[i].value > 0){ cout<<"MERGE: globalstatsbuffer7["<<i<<"].key : "<<globalstatsbuffer7[i].key<<", globalstatsbuffer7["<<i<<"].value : "<<globalstatsbuffer7[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer8[i].value > 0){ cout<<"MERGE: globalstatsbuffer8["<<i<<"].key : "<<globalstatsbuffer8[i].key<<", globalstatsbuffer8["<<i<<"].value : "<<globalstatsbuffer8[i].value<<endl; }}
	#endif 
	
	bool pass3 = false; // true;
	
	// vertices data
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 9:: [instance 0, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer0[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 9:: [instance 0, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP0B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 9:: [instance 1, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer1[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 9:: [instance 1, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP1B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 9:: [instance 2, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer2[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 9:: [instance 2, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP2B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 9:: [instance 3, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer3[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 9:: [instance 3, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP3B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 9:: [instance 4, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer4[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 9:: [instance 4, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP4B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 9:: [instance 5, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer5[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 9:: [instance 5, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP5B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 9:: [instance 6, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer6[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 9:: [instance 6, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP6B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP7: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 9:: [instance 7, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer7[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 9:: [instance 7, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP7B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram7[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP8: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 9:: [instance 8, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer8[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 9:: [instance 8, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP8B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram8[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	
	// active reduce partitions
	unsigned int offset_kvs = 0;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer0[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer1[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer2[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer3[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer4[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer5[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer6[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer7[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer8[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	// exit(EXIT_SUCCESS); // 
	return;
}
void acts_all::MERGEP0_mergeVs10(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9, uint512_dt * vdram, 
keyvalue_t globalstatsbuffer0[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer4[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer5[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer6[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer7[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer8[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer9[BLOCKRAM_GLOBALSTATS_SIZE],			globalparams_t globalparams, globalparams_t globalparamsv){
	#if defined(_DEBUGMODE_KERNELPRINTS3)// && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	// unsigned int actvpstats_beginoffset = MERGEP0_actvpstatsoffset(globalparamsv);
	
	// expensive variables 
	unsigned int lbasevoffset_kvs2 = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
	unsigned int _NUM_VALIDREDUCEPARTITIONS = (globalparams.NUM_REDUCEPARTITIONS + (VDATA_SHRINK_RATIO - 1)) / VDATA_SHRINK_RATIO;
	for(unsigned int reduce_partition=0; reduce_partition<globalparams.NUM_REDUCEPARTITIONS; reduce_partition++){
		if(reduce_partition >= _NUM_VALIDREDUCEPARTITIONS){ 
			globalstatsbuffer0[reduce_partition].value = 0; 
			globalstatsbuffer1[reduce_partition].value = 0; 
			globalstatsbuffer2[reduce_partition].value = 0; 
			globalstatsbuffer3[reduce_partition].value = 0; 
			globalstatsbuffer4[reduce_partition].value = 0; 
			globalstatsbuffer5[reduce_partition].value = 0; 
			globalstatsbuffer6[reduce_partition].value = 0; 
			globalstatsbuffer7[reduce_partition].value = 0; 
			globalstatsbuffer8[reduce_partition].value = 0; 
			globalstatsbuffer9[reduce_partition].value = 0; 
		}
	}
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
	cout<<"Printing active global stats..."<<endl;
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer0[i].value > 0){ cout<<"MERGE: globalstatsbuffer0["<<i<<"].key : "<<globalstatsbuffer0[i].key<<", globalstatsbuffer0["<<i<<"].value : "<<globalstatsbuffer0[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer1[i].value > 0){ cout<<"MERGE: globalstatsbuffer1["<<i<<"].key : "<<globalstatsbuffer1[i].key<<", globalstatsbuffer1["<<i<<"].value : "<<globalstatsbuffer1[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer2[i].value > 0){ cout<<"MERGE: globalstatsbuffer2["<<i<<"].key : "<<globalstatsbuffer2[i].key<<", globalstatsbuffer2["<<i<<"].value : "<<globalstatsbuffer2[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer3[i].value > 0){ cout<<"MERGE: globalstatsbuffer3["<<i<<"].key : "<<globalstatsbuffer3[i].key<<", globalstatsbuffer3["<<i<<"].value : "<<globalstatsbuffer3[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer4[i].value > 0){ cout<<"MERGE: globalstatsbuffer4["<<i<<"].key : "<<globalstatsbuffer4[i].key<<", globalstatsbuffer4["<<i<<"].value : "<<globalstatsbuffer4[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer5[i].value > 0){ cout<<"MERGE: globalstatsbuffer5["<<i<<"].key : "<<globalstatsbuffer5[i].key<<", globalstatsbuffer5["<<i<<"].value : "<<globalstatsbuffer5[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer6[i].value > 0){ cout<<"MERGE: globalstatsbuffer6["<<i<<"].key : "<<globalstatsbuffer6[i].key<<", globalstatsbuffer6["<<i<<"].value : "<<globalstatsbuffer6[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer7[i].value > 0){ cout<<"MERGE: globalstatsbuffer7["<<i<<"].key : "<<globalstatsbuffer7[i].key<<", globalstatsbuffer7["<<i<<"].value : "<<globalstatsbuffer7[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer8[i].value > 0){ cout<<"MERGE: globalstatsbuffer8["<<i<<"].key : "<<globalstatsbuffer8[i].key<<", globalstatsbuffer8["<<i<<"].value : "<<globalstatsbuffer8[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer9[i].value > 0){ cout<<"MERGE: globalstatsbuffer9["<<i<<"].key : "<<globalstatsbuffer9[i].key<<", globalstatsbuffer9["<<i<<"].value : "<<globalstatsbuffer9[i].value<<endl; }}
	#endif 
	
	bool pass3 = false; // true;
	
	// vertices data
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 10:: [instance 0, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer0[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 10:: [instance 0, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP0B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 10:: [instance 1, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer1[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 10:: [instance 1, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP1B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 10:: [instance 2, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer2[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 10:: [instance 2, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP2B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 10:: [instance 3, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer3[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 10:: [instance 3, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP3B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 10:: [instance 4, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer4[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 10:: [instance 4, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP4B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 10:: [instance 5, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer5[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 10:: [instance 5, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP5B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 10:: [instance 6, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer6[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 10:: [instance 6, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP6B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP7: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 10:: [instance 7, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer7[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 10:: [instance 7, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP7B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram7[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP8: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 10:: [instance 8, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer8[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 10:: [instance 8, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP8B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram8[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP9: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 10:: [instance 9, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer9[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 10:: [instance 9, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP9B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram9[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	
	// active reduce partitions
	unsigned int offset_kvs = 0;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer0[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer1[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer2[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer3[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer4[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer5[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer6[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer7[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer8[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer9[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	// exit(EXIT_SUCCESS); // 
	return;
}
void acts_all::MERGEP0_mergeVs11(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10, uint512_dt * vdram, 
keyvalue_t globalstatsbuffer0[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer4[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer5[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer6[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer7[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer8[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer9[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer10[BLOCKRAM_GLOBALSTATS_SIZE],			globalparams_t globalparams, globalparams_t globalparamsv){
	#if defined(_DEBUGMODE_KERNELPRINTS3)// && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	// unsigned int actvpstats_beginoffset = MERGEP0_actvpstatsoffset(globalparamsv);
	
	// expensive variables 
	unsigned int lbasevoffset_kvs2 = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
	unsigned int _NUM_VALIDREDUCEPARTITIONS = (globalparams.NUM_REDUCEPARTITIONS + (VDATA_SHRINK_RATIO - 1)) / VDATA_SHRINK_RATIO;
	for(unsigned int reduce_partition=0; reduce_partition<globalparams.NUM_REDUCEPARTITIONS; reduce_partition++){
		if(reduce_partition >= _NUM_VALIDREDUCEPARTITIONS){ 
			globalstatsbuffer0[reduce_partition].value = 0; 
			globalstatsbuffer1[reduce_partition].value = 0; 
			globalstatsbuffer2[reduce_partition].value = 0; 
			globalstatsbuffer3[reduce_partition].value = 0; 
			globalstatsbuffer4[reduce_partition].value = 0; 
			globalstatsbuffer5[reduce_partition].value = 0; 
			globalstatsbuffer6[reduce_partition].value = 0; 
			globalstatsbuffer7[reduce_partition].value = 0; 
			globalstatsbuffer8[reduce_partition].value = 0; 
			globalstatsbuffer9[reduce_partition].value = 0; 
			globalstatsbuffer10[reduce_partition].value = 0; 
		}
	}
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
	cout<<"Printing active global stats..."<<endl;
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer0[i].value > 0){ cout<<"MERGE: globalstatsbuffer0["<<i<<"].key : "<<globalstatsbuffer0[i].key<<", globalstatsbuffer0["<<i<<"].value : "<<globalstatsbuffer0[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer1[i].value > 0){ cout<<"MERGE: globalstatsbuffer1["<<i<<"].key : "<<globalstatsbuffer1[i].key<<", globalstatsbuffer1["<<i<<"].value : "<<globalstatsbuffer1[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer2[i].value > 0){ cout<<"MERGE: globalstatsbuffer2["<<i<<"].key : "<<globalstatsbuffer2[i].key<<", globalstatsbuffer2["<<i<<"].value : "<<globalstatsbuffer2[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer3[i].value > 0){ cout<<"MERGE: globalstatsbuffer3["<<i<<"].key : "<<globalstatsbuffer3[i].key<<", globalstatsbuffer3["<<i<<"].value : "<<globalstatsbuffer3[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer4[i].value > 0){ cout<<"MERGE: globalstatsbuffer4["<<i<<"].key : "<<globalstatsbuffer4[i].key<<", globalstatsbuffer4["<<i<<"].value : "<<globalstatsbuffer4[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer5[i].value > 0){ cout<<"MERGE: globalstatsbuffer5["<<i<<"].key : "<<globalstatsbuffer5[i].key<<", globalstatsbuffer5["<<i<<"].value : "<<globalstatsbuffer5[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer6[i].value > 0){ cout<<"MERGE: globalstatsbuffer6["<<i<<"].key : "<<globalstatsbuffer6[i].key<<", globalstatsbuffer6["<<i<<"].value : "<<globalstatsbuffer6[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer7[i].value > 0){ cout<<"MERGE: globalstatsbuffer7["<<i<<"].key : "<<globalstatsbuffer7[i].key<<", globalstatsbuffer7["<<i<<"].value : "<<globalstatsbuffer7[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer8[i].value > 0){ cout<<"MERGE: globalstatsbuffer8["<<i<<"].key : "<<globalstatsbuffer8[i].key<<", globalstatsbuffer8["<<i<<"].value : "<<globalstatsbuffer8[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer9[i].value > 0){ cout<<"MERGE: globalstatsbuffer9["<<i<<"].key : "<<globalstatsbuffer9[i].key<<", globalstatsbuffer9["<<i<<"].value : "<<globalstatsbuffer9[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer10[i].value > 0){ cout<<"MERGE: globalstatsbuffer10["<<i<<"].key : "<<globalstatsbuffer10[i].key<<", globalstatsbuffer10["<<i<<"].value : "<<globalstatsbuffer10[i].value<<endl; }}
	#endif 
	
	bool pass3 = false; // true;
	
	// vertices data
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 11:: [instance 0, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer0[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 11:: [instance 0, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP0B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 11:: [instance 1, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer1[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 11:: [instance 1, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP1B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 11:: [instance 2, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer2[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 11:: [instance 2, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP2B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 11:: [instance 3, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer3[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 11:: [instance 3, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP3B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 11:: [instance 4, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer4[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 11:: [instance 4, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP4B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 11:: [instance 5, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer5[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 11:: [instance 5, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP5B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 11:: [instance 6, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer6[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 11:: [instance 6, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP6B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP7: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 11:: [instance 7, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer7[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 11:: [instance 7, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP7B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram7[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP8: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 11:: [instance 8, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer8[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 11:: [instance 8, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP8B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram8[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP9: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 11:: [instance 9, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer9[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 11:: [instance 9, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP9B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram9[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP10: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 11:: [instance 10, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer10[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 11:: [instance 10, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP10B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram10[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	
	// active reduce partitions
	unsigned int offset_kvs = 0;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer0[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer1[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer2[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer3[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer4[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer5[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer6[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer7[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer8[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer9[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer10[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	// exit(EXIT_SUCCESS); // 
	return;
}
void acts_all::MERGEP0_mergeVs12(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11, uint512_dt * vdram, 
keyvalue_t globalstatsbuffer0[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer4[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer5[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer6[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer7[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer8[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer9[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer10[BLOCKRAM_GLOBALSTATS_SIZE],keyvalue_t globalstatsbuffer11[BLOCKRAM_GLOBALSTATS_SIZE],			globalparams_t globalparams, globalparams_t globalparamsv){
	#if defined(_DEBUGMODE_KERNELPRINTS3)// && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	// unsigned int actvpstats_beginoffset = MERGEP0_actvpstatsoffset(globalparamsv);
	
	// expensive variables 
	unsigned int lbasevoffset_kvs2 = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
	unsigned int _NUM_VALIDREDUCEPARTITIONS = (globalparams.NUM_REDUCEPARTITIONS + (VDATA_SHRINK_RATIO - 1)) / VDATA_SHRINK_RATIO;
	for(unsigned int reduce_partition=0; reduce_partition<globalparams.NUM_REDUCEPARTITIONS; reduce_partition++){
		if(reduce_partition >= _NUM_VALIDREDUCEPARTITIONS){ 
			globalstatsbuffer0[reduce_partition].value = 0; 
			globalstatsbuffer1[reduce_partition].value = 0; 
			globalstatsbuffer2[reduce_partition].value = 0; 
			globalstatsbuffer3[reduce_partition].value = 0; 
			globalstatsbuffer4[reduce_partition].value = 0; 
			globalstatsbuffer5[reduce_partition].value = 0; 
			globalstatsbuffer6[reduce_partition].value = 0; 
			globalstatsbuffer7[reduce_partition].value = 0; 
			globalstatsbuffer8[reduce_partition].value = 0; 
			globalstatsbuffer9[reduce_partition].value = 0; 
			globalstatsbuffer10[reduce_partition].value = 0; 
			globalstatsbuffer11[reduce_partition].value = 0; 
		}
	}
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
	cout<<"Printing active global stats..."<<endl;
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer0[i].value > 0){ cout<<"MERGE: globalstatsbuffer0["<<i<<"].key : "<<globalstatsbuffer0[i].key<<", globalstatsbuffer0["<<i<<"].value : "<<globalstatsbuffer0[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer1[i].value > 0){ cout<<"MERGE: globalstatsbuffer1["<<i<<"].key : "<<globalstatsbuffer1[i].key<<", globalstatsbuffer1["<<i<<"].value : "<<globalstatsbuffer1[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer2[i].value > 0){ cout<<"MERGE: globalstatsbuffer2["<<i<<"].key : "<<globalstatsbuffer2[i].key<<", globalstatsbuffer2["<<i<<"].value : "<<globalstatsbuffer2[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer3[i].value > 0){ cout<<"MERGE: globalstatsbuffer3["<<i<<"].key : "<<globalstatsbuffer3[i].key<<", globalstatsbuffer3["<<i<<"].value : "<<globalstatsbuffer3[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer4[i].value > 0){ cout<<"MERGE: globalstatsbuffer4["<<i<<"].key : "<<globalstatsbuffer4[i].key<<", globalstatsbuffer4["<<i<<"].value : "<<globalstatsbuffer4[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer5[i].value > 0){ cout<<"MERGE: globalstatsbuffer5["<<i<<"].key : "<<globalstatsbuffer5[i].key<<", globalstatsbuffer5["<<i<<"].value : "<<globalstatsbuffer5[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer6[i].value > 0){ cout<<"MERGE: globalstatsbuffer6["<<i<<"].key : "<<globalstatsbuffer6[i].key<<", globalstatsbuffer6["<<i<<"].value : "<<globalstatsbuffer6[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer7[i].value > 0){ cout<<"MERGE: globalstatsbuffer7["<<i<<"].key : "<<globalstatsbuffer7[i].key<<", globalstatsbuffer7["<<i<<"].value : "<<globalstatsbuffer7[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer8[i].value > 0){ cout<<"MERGE: globalstatsbuffer8["<<i<<"].key : "<<globalstatsbuffer8[i].key<<", globalstatsbuffer8["<<i<<"].value : "<<globalstatsbuffer8[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer9[i].value > 0){ cout<<"MERGE: globalstatsbuffer9["<<i<<"].key : "<<globalstatsbuffer9[i].key<<", globalstatsbuffer9["<<i<<"].value : "<<globalstatsbuffer9[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer10[i].value > 0){ cout<<"MERGE: globalstatsbuffer10["<<i<<"].key : "<<globalstatsbuffer10[i].key<<", globalstatsbuffer10["<<i<<"].value : "<<globalstatsbuffer10[i].value<<endl; }}
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer11[i].value > 0){ cout<<"MERGE: globalstatsbuffer11["<<i<<"].key : "<<globalstatsbuffer11[i].key<<", globalstatsbuffer11["<<i<<"].value : "<<globalstatsbuffer11[i].value<<endl; }}
	#endif 
	
	bool pass3 = false; // true;
	
	// vertices data
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 12:: [instance 0, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer0[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 12:: [instance 0, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP0B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 12:: [instance 1, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer1[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 12:: [instance 1, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP1B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 12:: [instance 2, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer2[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 12:: [instance 2, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP2B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 12:: [instance 3, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer3[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 12:: [instance 3, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP3B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 12:: [instance 4, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer4[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 12:: [instance 4, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP4B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 12:: [instance 5, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer5[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 12:: [instance 5, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP5B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 12:: [instance 6, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer6[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 12:: [instance 6, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP6B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP7: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 12:: [instance 7, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer7[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 12:: [instance 7, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP7B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram7[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP8: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 12:: [instance 8, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer8[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 12:: [instance 8, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP8B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram8[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP9: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 12:: [instance 9, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer9[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 12:: [instance 9, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP9B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram9[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP10: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 12:: [instance 10, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer10[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 12:: [instance 10, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP10B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram10[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP11: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 12:: [instance 11, partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer11[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 12:: [instance 11, partition "<<partition<<"] is active: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP11B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram11[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];

				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	
	// active reduce partitions
	unsigned int offset_kvs = 0;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer0[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer1[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer2[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer3[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer4[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer5[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer6[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer7[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer8[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer9[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer10[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer11[reduce_partition].value);
	}
	offset_kvs += globalparamsv.NUM_REDUCEPARTITIONS;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	// exit(EXIT_SUCCESS); // 
	return;
}

#define FACTOR101 32
void acts_all::MERGEP0_exchange(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC, uint512_dt * mdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::EXCHANGE:: ACTS EXCHANGE LAUNCHED. Exchanging vertices across different SLRs..."<<endl; 
	#endif
	
	bool exchangeall = false; // true;//false;
	
	uint32_vec_dt globalstatsbuffer0[BLOCKRAM_GLOBALSTATS_BIGSIZE];
	#pragma HLS DATA_PACK variable = globalstatsbuffer0
	uint32_vec_dt globalstatsbuffer1[BLOCKRAM_GLOBALSTATS_BIGSIZE];
	#pragma HLS DATA_PACK variable = globalstatsbuffer1
	uint32_vec_dt globalstatsbuffer2[BLOCKRAM_GLOBALSTATS_BIGSIZE];
	#pragma HLS DATA_PACK variable = globalstatsbuffer2
	#ifndef ALLVERTEXISACTIVE_ALGORITHM
	keyvalue_t actvvbuffer[VECTOR2_SIZE][ACTVVBUFFER_SIZE];
	#pragma HLS array_partition variable = actvvbuffer
	unsigned int vpmaskbuffer[BLOCKRAM_CUMMTVPMASK_SIZE];
	unsigned int loadcount[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=loadcount complete
	unsigned int numactvvs[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=numactvvs complete
	unit1_type vpmaskVec[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vpmaskVec complete
	unsigned int local_actvv[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=local_actvv complete
	unsigned int local_actvv2[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=local_actvv2 complete
	unsigned int global_actvvs[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=global_actvvs complete
	uint32_type vpmaskVecSum[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vpmaskVecSum complete	
	#endif 	
	value_t vdatas[VECTOR2_SIZE];
	
	unsigned int reduce_partition = 0;
	
	globalparams_t globalparamsvA = UTILP0_getglobalparams(vdramA, 0);
	globalparams_t globalparamsvB = UTILP0_getglobalparams(vdramB, 0);
	globalparams_t globalparamsvC = UTILP0_getglobalparams(vdramC, 0);
	globalparams_t globalparamsm = UTILP0_getglobalparams(mdram, 0);
	unsigned int actvpstats_beginoffset = MERGEP0_actvpstatsoffset(globalparamsvA);
	
	UTILP0_SetFirstData(mdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID);
	unsigned int nextGraphIter = globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID;
	
	unsigned int _TOTALNUMREDUCEPARTITIONS_SLR2 = NUMCOMPUTEUNITS_SLR2 * globalparamsvA.NUM_REDUCEPARTITIONS;
	unsigned int _TOTALNUMREDUCEPARTITIONS_SLR1 = NUMCOMPUTEUNITS_SLR1 * globalparamsvA.NUM_REDUCEPARTITIONS;
	unsigned int _TOTALNUMREDUCEPARTITIONS_SLR0 = NUMCOMPUTEUNITS_SLR0 * globalparamsvA.NUM_REDUCEPARTITIONS;
	
	#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
	unsigned int _NUM_VALIDREDUCEPARTITIONS = (globalparamsvA.NUM_REDUCEPARTITIONS + (VDATA_SHRINK_RATIO - 1)) / VDATA_SHRINK_RATIO;
	#else 
	unsigned int _NUM_VALIDREDUCEPARTITIONS = globalparamsvA.NUM_REDUCEPARTITIONS;//
	#endif 
	
	unsigned int voffsetA_kvs = 0;
	unsigned int voffsetB_kvs = NUMCOMPUTEUNITS_SLR2 * globalparamsvB.NUM_REDUCEPARTITIONS * globalparamsvB.SIZEKVS2_REDUCEPARTITION;
	unsigned int voffsetC_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparamsvC.NUM_REDUCEPARTITIONS * globalparamsvC.SIZEKVS2_REDUCEPARTITION;
	
	uint32_type pmaski = 0;
	
	#ifndef ALLVERTEXISACTIVE_ALGORITHM
	for(unsigned int v=0; v<VECTOR2_SIZE; v++){ loadcount[v] = 0; numactvvs[v] = 0; }
	#endif 
	for(unsigned int v=0; v<VECTOR2_SIZE; v++){ vpmaskVecSum[v] = 0; }
	for(unsigned int v=0; v<globalparamsvA.NUM_PROCESSEDGESPARTITIONS; v++){ vpmaskbuffer[v] = 0; }
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::EXCHANGE:: retrieving stats from vdramA, vdramB & vdramC. "<<endl; 
	#endif
	// active reduce partitions
	for(unsigned int reduce_partition=0; reduce_partition<_TOTALNUMREDUCEPARTITIONS_SLR2; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_exchange: ERROR 1s20", reduce_partition / FACTOR101, BLOCKRAM_GLOBALSTATS_BIGSIZE, reduce_partition, NAp, NAp);
		#endif 
		unsigned int tempdata = UTILP0_GetSecondData(vdramA, globalparamsvA.BASEOFFSETKVS_STATSDRAM + reduce_partition);
		if(tempdata > 0){ globalstatsbuffer0[reduce_partition / FACTOR101].data[reduce_partition % FACTOR101] = 1; }
	}
	
	for(unsigned int reduce_partition=0; reduce_partition<_TOTALNUMREDUCEPARTITIONS_SLR1; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_exchange: ERROR 1s21", reduce_partition / FACTOR101, BLOCKRAM_GLOBALSTATS_BIGSIZE, reduce_partition, NAp, NAp);
		#endif
		unsigned int tempdata = UTILP0_GetSecondData(vdramB, globalparamsvB.BASEOFFSETKVS_STATSDRAM + reduce_partition);
		if(tempdata > 0){ globalstatsbuffer1[reduce_partition / FACTOR101].data[reduce_partition % FACTOR101]; }
	}
	
	for(unsigned int reduce_partition=0; reduce_partition<_TOTALNUMREDUCEPARTITIONS_SLR0; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_exchange: ERROR 1s22", reduce_partition / FACTOR101, BLOCKRAM_GLOBALSTATS_BIGSIZE, reduce_partition, NAp, NAp);
		#endif
		unsigned int tempdata = UTILP0_GetSecondData(vdramC, globalparamsvC.BASEOFFSETKVS_STATSDRAM + reduce_partition);
		if(tempdata > 0){ globalstatsbuffer2[reduce_partition / FACTOR101].data[reduce_partition % FACTOR101]; }
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
	cout<<"Printing active global stats..."<<endl;
	for(unsigned int i=0; i<_TOTALNUMREDUCEPARTITIONS_SLR0; i++){ if(globalstatsbuffer0[i / 32].data[i % 32] > 0 || globalstatsbuffer1[i / 32].data[i % 32] > 0 || globalstatsbuffer2[i / 32].data[i % 32] > 0){ cout<<"EXCHANGE: globalstatsbuffer0["<<i / 32<<"].data["<<i % 32<<"] : "<<globalstatsbuffer0[i][i / 32].data[i % 32]<<", globalstatsbuffer1["<<i / 32<<"].data["<<i % 32<<"] : "<<globalstatsbuffer1[i / 32].data[i % 32]<<", globalstatsbuffer2["<<i / 32<<"].data["<<i % 32<<"] : "<<globalstatsbuffer2[i / 32].data[i % 32]<<endl; }}					
	#endif 
	
	// vertices
	reduce_partition = 0;
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::EXCHANGE:: transferring vertices from vdramA to vdramB & vdramC. [begin offset: "<<voffsetA_kvs<<"]"<<endl; 
	#endif
	MERGEP0_EXCHANGEVS_LOOP1A: for(unsigned int i=0; i<NUMCOMPUTEUNITS_SLR2; i++){
		unsigned int voffset_kvs = 0;
		for(unsigned int partition=0; partition<globalparamsvA.NUM_REDUCEPARTITIONS; partition++){ 
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("TOPP0_topkernelS: ERROR 20", reduce_partition / FACTOR101, BLOCKRAM_GLOBALSTATS_BIGSIZE, globalparamsvA.NUM_REDUCEPARTITIONS, NAp, NAp);
			#endif
			if(globalstatsbuffer0[reduce_partition / FACTOR101].data[reduce_partition % FACTOR101] > 0 || exchangeall == true){ // CRITICAL REMOVEME. true.
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"acts_merge::exchange vertices ::[A->B,C] [instance "<<i<<", partition "<<partition<<"] is active: [voffset_kvs: "<<voffset_kvs<<", offset: "<<voffsetA_kvs*VECTOR2_SIZE<<", size: "<<globalparamsvA.SIZEKVS2_REDUCEPARTITION<<", globalstatsbuffer0["<<reduce_partition<<"]: "<<globalstatsbuffer0[reduce_partition]<<"] "<<endl;
				#endif
				MERGEP0_EXCHANGEVS_LOOP1B: for(unsigned int k=0; k<globalparamsvA.SIZEKVS2_REDUCEPARTITION; k++){
				#pragma HLS PIPELINE II=1
					#ifdef _DEBUGMODE_CHECKS3
					actsutilityobj->checkoutofbounds("MERGE::EXCHANGE::: ERROR 21", globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k, TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
					actsutilityobj->checkoutofbounds("MERGE::EXCHANGE::: ERROR 22", globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k, TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
					#endif
					
					// collect active vertices
					#ifdef ALLVERTEXISACTIVE_ALGORITHM
					vdramB[globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k] = vdramA[globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k];
					vdramC[globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k] = vdramA[globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k];
					#else 
					UTILP0_GetDataset(vdramA, globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k, vdatas);
					UTILP0_SetDataset(vdramB, globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k, vdatas);
					UTILP0_SetDataset(vdramC, globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k, vdatas);
					#endif 
					
					#ifndef ALLVERTEXISACTIVE_ALGORITHM
					unsigned int rand = i + voffsetA_kvs + k;
					MERGEP0_collects(i, voffset_kvs + k, rand, vdatas, actvvbuffer, vpmaskbuffer, loadcount, numactvvs, local_actvv, local_actvv2, global_actvvs, vpmaskVecSum);
					#endif 
				}
			}
			reduce_partition += 1;
			voffset_kvs += globalparamsvC.SIZEKVS2_REDUCEPARTITION;
			voffsetA_kvs += globalparamsvA.SIZEKVS2_REDUCEPARTITION;
		}
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::EXCHANGE:: FINISHED transferring vertices from vdramA to vdramB & vdramC. [end offset: "<<voffsetA_kvs<<"]"<<endl; 
	#endif

	reduce_partition = 0;
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::EXCHANGE:: transferring vertices from vdramB to vdramA & vdramC. [begin offset: "<<voffsetB_kvs<<"]"<<endl; 
	#endif
	MERGEP0_EXCHANGEVS_LOOP2A: for(unsigned int i=0; i<NUMCOMPUTEUNITS_SLR1; i++){
		unsigned int voffset_kvs = 0;
		for(unsigned int partition=0; partition<globalparamsvA.NUM_REDUCEPARTITIONS; partition++){ 
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("TOPP0_topkernelS: ERROR 21", reduce_partition / FACTOR101, BLOCKRAM_SIZE, NAp, NAp, NAp);
			#endif
			if(globalstatsbuffer1[reduce_partition / FACTOR101].data[reduce_partition % FACTOR101] > 0 || exchangeall == true){ // CRITICAL REMOVEME. true.
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"acts_merge::exchange vertices ::[B->A,C] [instance "<<i<<", partition "<<partition<<"] is active: [voffset_kvs: "<<voffset_kvs<<", offset: "<<voffsetB_kvs*VECTOR2_SIZE<<", size: "<<globalparamsvB.SIZEKVS2_REDUCEPARTITION<<", globalstatsbuffer[0]["<<reduce_partition<<"]: "<<globalstatsbuffer[0][reduce_partition]<<"] "<<endl;
				#endif
				MERGEP0_EXCHANGEVS_LOOP2B: for(unsigned int k=0; k<globalparamsvB.SIZEKVS2_REDUCEPARTITION; k++){
				#pragma HLS PIPELINE II=1
					#ifdef _DEBUGMODE_CHECKS3
					actsutilityobj->checkoutofbounds("MERGE::EXCHANGE::: ERROR 24", globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k, TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
					actsutilityobj->checkoutofbounds("MERGE::EXCHANGE::: ERROR 25", globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k, TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
					#endif
					
					// collect active vertices 
					#ifdef ALLVERTEXISACTIVE_ALGORITHM
					vdramA[globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k] = vdramB[globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k];
					vdramC[globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k] = vdramB[globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k];
					#else 
					UTILP0_GetDataset(vdramB, globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k, vdatas);
					UTILP0_SetDataset(vdramA, globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k, vdatas);
					UTILP0_SetDataset(vdramC, globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k, vdatas);	
					#endif
					
					#ifndef ALLVERTEXISACTIVE_ALGORITHM
					unsigned int rand = NUMCOMPUTEUNITS_SLR2 + i + voffsetB_kvs + k;
					MERGEP0_collects(NUMCOMPUTEUNITS_SLR2 + i, voffset_kvs + k, rand, vdatas, actvvbuffer, vpmaskbuffer, loadcount, numactvvs, local_actvv, local_actvv2, global_actvvs, vpmaskVecSum);
					#endif 
				}
			}
			reduce_partition += 1;
			voffset_kvs += globalparamsvC.SIZEKVS2_REDUCEPARTITION;
			voffsetB_kvs += globalparamsvB.SIZEKVS2_REDUCEPARTITION;
		}
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::EXCHANGE:: FINISHED transferring vertices from vdramB to vdramA & vdramC. [end offset: "<<voffsetB_kvs<<"]"<<endl; 
	#endif
	
	reduce_partition = 0;
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::EXCHANGE:: transferring vertices from vdramC to vdramA & vdramB. [begin offset: "<<voffsetC_kvs<<"]"<<endl; 
	#endif
	MERGEP0_EXCHANGEVS_LOOP3A: for(unsigned int i=0; i<NUMCOMPUTEUNITS_SLR0; i++){
		unsigned int voffset_kvs = 0;
		for(unsigned int partition=0; partition<globalparamsvA.NUM_REDUCEPARTITIONS; partition++){ 
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("TOPP0_topkernelS: ERROR 22", reduce_partition / FACTOR101, BLOCKRAM_SIZE, NAp, NAp, NAp);
			#endif
			if(globalstatsbuffer2[reduce_partition / FACTOR101].data[reduce_partition % FACTOR101] > 0 || exchangeall == true){ // CRITICAL REMOVEME. true.
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"acts_merge::exchange vertices ::[C->A,B] [instance "<<i<<", partition "<<partition<<"] is active: [voffset_kvs: "<<voffset_kvs<<", offset: "<<voffsetC_kvs*VECTOR2_SIZE<<", size: "<<globalparamsvC.SIZEKVS2_REDUCEPARTITION<<", globalstatsbuffer0["<<reduce_partition<<"]: "<<globalstatsbuffer0[reduce_partition]<<"] "<<endl;
				#endif
				MERGEP0_EXCHANGEVS_LOOP3B: for(unsigned int k=0; k<globalparamsvC.SIZEKVS2_REDUCEPARTITION; k++){
				#pragma HLS PIPELINE II=1
					#ifdef _DEBUGMODE_CHECKS3
					actsutilityobj->checkoutofbounds("MERGE::EXCHANGE::: ERROR 27", globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k, TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
					actsutilityobj->checkoutofbounds("MERGE::EXCHANGE::: ERROR 28", globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k, TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
					#endif
					
					#ifdef ALLVERTEXISACTIVE_ALGORITHM
					vdramA[globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k] = vdramC[globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k];
					vdramB[globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k] = vdramC[globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k];
					#else 
					UTILP0_GetDataset(vdramC, globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k, vdatas);
					UTILP0_SetDataset(vdramA, globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k, vdatas);
					UTILP0_SetDataset(vdramB, globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k, vdatas);		
					#endif 
					
					#ifndef ALLVERTEXISACTIVE_ALGORITHM
					unsigned int rand = NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + i + voffsetC_kvs + k;
					MERGEP0_collects(NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + i, voffset_kvs + k, rand, vdatas, actvvbuffer, vpmaskbuffer, loadcount, numactvvs, local_actvv, local_actvv2, global_actvvs, vpmaskVecSum);	
					#endif 
				}
			}
			reduce_partition += 1;
			voffset_kvs += globalparamsvC.SIZEKVS2_REDUCEPARTITION;
			voffsetC_kvs += globalparamsvC.SIZEKVS2_REDUCEPARTITION;			
		}
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::EXCHANGE:: FINISHED transferring vertices from vdramC to vdramA & vdramB. [end offset: "<<voffsetC_kvs<<"]"<<endl; 
	#endif

	// vertices partition masks
	#ifdef CONFIG_MERGE_VPARTITIONS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"#################################################### MERGE::EXCHANGE:: merging vertices partition masks across vdramA, vdramB & vdramC."<<endl; 
	#endif
	unsigned int pA = 0; unsigned int pB = 0; unsigned int pC = 0; 
	MERGEP0_EXCHANGEVPMS_LOOP1: for(unsigned int partition=0; partition<globalparamsvA.NUM_PROCESSEDGESPARTITIONS; partition++){ // BLOCKRAM_NEXTPMASK_SIZE, globalparamsvA.NUM_PROCESSEDGESPARTITIONS
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGE::EXCHANGE::: ERROR 31", globalparamsvA.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition, TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
		actsutilityobj->checkoutofbounds("MERGE::EXCHANGE::: ERROR 32", globalparamsvB.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition, TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
		actsutilityobj->checkoutofbounds("MERGE::EXCHANGE::: ERROR 33", globalparamsvC.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition, TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
		#endif
		
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
		// cout<<"+++ TOPP0_topkernelS: vpmaskbuffer["<<partition<<"]: "<<vpmaskbuffer[partition]<<endl;
		if(vpmaskbuffer[partition] > 0){ cout<<"--- TOPP0_topkernelS: vpmaskbuffer["<<partition<<"]: "<<vpmaskbuffer[partition]<<endl; }
		#endif
		
		UTILP0_SetFirstData(vdramA, globalparamsvA.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition, vpmaskbuffer[partition]);
		UTILP0_SetFirstData(vdramB, globalparamsvB.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition, vpmaskbuffer[partition]);
		UTILP0_SetFirstData(vdramC, globalparamsvC.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition, vpmaskbuffer[partition]);
		// UTILP0_SetFirstData(vdramA, globalparamsvA.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition, 1);
		// UTILP0_SetFirstData(vdramB, globalparamsvB.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition, 1);
		// UTILP0_SetFirstData(vdramC, globalparamsvC.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition, 1);
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"--- TOPP0_topkernelS: vdramA["<<partition<<"]: "<<p<<endl; 
		#endif
	}
	#endif 
	
	// collect active vertices 
	#if not defined (ALLVERTEXISACTIVE_ALGORITHM) && defined(CONFIG_HYBRIDGPMODE)
	unsigned int maxloadcount_vs = 0;
	unsigned int maxnumactvvs_vs = 0;
	unsigned int sumloadcount = 0;
	unsigned int num_vertices_per_bank = globalparamsvC.SIZEKVS2_REDUCEPARTITION * globalparamsvC.NUM_REDUCEPARTITIONS * VECTOR2_SIZE;
	for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(maxloadcount_vs < loadcount[v]){ maxloadcount_vs = loadcount[v]; }}
	for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(maxnumactvvs_vs < numactvvs[v]){ maxnumactvvs_vs = numactvvs[v]; }}
	for(unsigned int v=0; v<VECTOR2_SIZE; v++){ sumloadcount += loadcount[v]; }
	
	unsigned int offsetkvs_actvvs;
	if(nextGraphIter % 2 == 0){ offsetkvs_actvvs = 0; } 
	else { offsetkvs_actvvs = globalparamsm.SIZE_VERTEXPTRS / VECTOR_SIZE; }
	
	for(unsigned int i=0; i<maxloadcount_vs; i++){
	#pragma HLS PIPELINE II=1
	
		keyvalue_t actvv0 = actvvbuffer[0][i];
		global_actvvs[0] = UTILP0_GETREALVID(actvv0.value, actvv0.key);
	
		keyvalue_t actvv1 = actvvbuffer[1][i];
		global_actvvs[1] = UTILP0_GETREALVID(actvv1.value, actvv1.key);
	
		keyvalue_t actvv2 = actvvbuffer[2][i];
		global_actvvs[2] = UTILP0_GETREALVID(actvv2.value, actvv2.key);
	
		keyvalue_t actvv3 = actvvbuffer[3][i];
		global_actvvs[3] = UTILP0_GETREALVID(actvv3.value, actvv3.key);
	
		keyvalue_t actvv4 = actvvbuffer[4][i];
		global_actvvs[4] = UTILP0_GETREALVID(actvv4.value, actvv4.key);
	
		keyvalue_t actvv5 = actvvbuffer[5][i];
		global_actvvs[5] = UTILP0_GETREALVID(actvv5.value, actvv5.key);
	
		keyvalue_t actvv6 = actvvbuffer[6][i];
		global_actvvs[6] = UTILP0_GETREALVID(actvv6.value, actvv6.key);
	
		keyvalue_t actvv7 = actvvbuffer[7][i];
		global_actvvs[7] = UTILP0_GETREALVID(actvv7.value, actvv7.key);
	
		keyvalue_t actvv8 = actvvbuffer[8][i];
		global_actvvs[8] = UTILP0_GETREALVID(actvv8.value, actvv8.key);
	
		keyvalue_t actvv9 = actvvbuffer[9][i];
		global_actvvs[9] = UTILP0_GETREALVID(actvv9.value, actvv9.key);
	
		keyvalue_t actvv10 = actvvbuffer[10][i];
		global_actvvs[10] = UTILP0_GETREALVID(actvv10.value, actvv10.key);
	
		keyvalue_t actvv11 = actvvbuffer[11][i];
		global_actvvs[11] = UTILP0_GETREALVID(actvv11.value, actvv11.key);
	
		keyvalue_t actvv12 = actvvbuffer[12][i];
		global_actvvs[12] = UTILP0_GETREALVID(actvv12.value, actvv12.key);
	
		keyvalue_t actvv13 = actvvbuffer[13][i];
		global_actvvs[13] = UTILP0_GETREALVID(actvv13.value, actvv13.key);
	
		keyvalue_t actvv14 = actvvbuffer[14][i];
		global_actvvs[14] = UTILP0_GETREALVID(actvv14.value, actvv14.key);
	
		keyvalue_t actvv15 = actvvbuffer[15][i];
		global_actvvs[15] = UTILP0_GETREALVID(actvv15.value, actvv15.key);
		
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
		if(i < loadcount[0] && actvvbuffer[0][i].value != INVALIDDATA){ cout<<"~~~~~~ MERGE::EXCHANGE(X):: actvv: "<<global_actvvs[0]<<", localvid: "<<actvv0.value<<", instance: "<<actvv0.key<<endl; }
		if(i < loadcount[1] && actvvbuffer[1][i].value != INVALIDDATA){ cout<<"~~~~~~ MERGE::EXCHANGE(X):: actvv: "<<global_actvvs[1]<<", localvid: "<<actvv1.value<<", instance: "<<actvv1.key<<endl; }
		if(i < loadcount[2] && actvvbuffer[2][i].value != INVALIDDATA){ cout<<"~~~~~~ MERGE::EXCHANGE(X):: actvv: "<<global_actvvs[2]<<", localvid: "<<actvv2.value<<", instance: "<<actvv2.key<<endl; }
		if(i < loadcount[3] && actvvbuffer[3][i].value != INVALIDDATA){ cout<<"~~~~~~ MERGE::EXCHANGE(X):: actvv: "<<global_actvvs[3]<<", localvid: "<<actvv3.value<<", instance: "<<actvv3.key<<endl; }
		if(i < loadcount[4] && actvvbuffer[4][i].value != INVALIDDATA){ cout<<"~~~~~~ MERGE::EXCHANGE(X):: actvv: "<<global_actvvs[4]<<", localvid: "<<actvv4.value<<", instance: "<<actvv4.key<<endl; }
		if(i < loadcount[5] && actvvbuffer[5][i].value != INVALIDDATA){ cout<<"~~~~~~ MERGE::EXCHANGE(X):: actvv: "<<global_actvvs[5]<<", localvid: "<<actvv5.value<<", instance: "<<actvv5.key<<endl; }
		if(i < loadcount[6] && actvvbuffer[6][i].value != INVALIDDATA){ cout<<"~~~~~~ MERGE::EXCHANGE(X):: actvv: "<<global_actvvs[6]<<", localvid: "<<actvv6.value<<", instance: "<<actvv6.key<<endl; }
		if(i < loadcount[7] && actvvbuffer[7][i].value != INVALIDDATA){ cout<<"~~~~~~ MERGE::EXCHANGE(X):: actvv: "<<global_actvvs[7]<<", localvid: "<<actvv7.value<<", instance: "<<actvv7.key<<endl; }
		if(i < loadcount[8] && actvvbuffer[8][i].value != INVALIDDATA){ cout<<"~~~~~~ MERGE::EXCHANGE(X):: actvv: "<<global_actvvs[8]<<", localvid: "<<actvv8.value<<", instance: "<<actvv8.key<<endl; }
		if(i < loadcount[9] && actvvbuffer[9][i].value != INVALIDDATA){ cout<<"~~~~~~ MERGE::EXCHANGE(X):: actvv: "<<global_actvvs[9]<<", localvid: "<<actvv9.value<<", instance: "<<actvv9.key<<endl; }
		if(i < loadcount[10] && actvvbuffer[10][i].value != INVALIDDATA){ cout<<"~~~~~~ MERGE::EXCHANGE(X):: actvv: "<<global_actvvs[10]<<", localvid: "<<actvv10.value<<", instance: "<<actvv10.key<<endl; }
		if(i < loadcount[11] && actvvbuffer[11][i].value != INVALIDDATA){ cout<<"~~~~~~ MERGE::EXCHANGE(X):: actvv: "<<global_actvvs[11]<<", localvid: "<<actvv11.value<<", instance: "<<actvv11.key<<endl; }
		if(i < loadcount[12] && actvvbuffer[12][i].value != INVALIDDATA){ cout<<"~~~~~~ MERGE::EXCHANGE(X):: actvv: "<<global_actvvs[12]<<", localvid: "<<actvv12.value<<", instance: "<<actvv12.key<<endl; }
		if(i < loadcount[13] && actvvbuffer[13][i].value != INVALIDDATA){ cout<<"~~~~~~ MERGE::EXCHANGE(X):: actvv: "<<global_actvvs[13]<<", localvid: "<<actvv13.value<<", instance: "<<actvv13.key<<endl; }
		if(i < loadcount[14] && actvvbuffer[14][i].value != INVALIDDATA){ cout<<"~~~~~~ MERGE::EXCHANGE(X):: actvv: "<<global_actvvs[14]<<", localvid: "<<actvv14.value<<", instance: "<<actvv14.key<<endl; }
		if(i < loadcount[15] && actvvbuffer[15][i].value != INVALIDDATA){ cout<<"~~~~~~ MERGE::EXCHANGE(X):: actvv: "<<global_actvvs[15]<<", localvid: "<<actvv15.value<<", instance: "<<actvv15.key<<endl; }
		#endif	
		
		UTILP0_SetDataset(mdram, globalparamsm.BASEOFFSETKVS_ACTIVEVERTICES + offsetkvs_actvvs + i, global_actvvs);
	}
	UTILP0_SetFirstData(mdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_ACTIVEVERTICES, maxloadcount_vs * VECTOR2_SIZE);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	for(unsigned int v=0; v<VECTOR2_SIZE; v++){ cout<<"MERGE::EXCHANGE:: loadcount["<<v<<"]: "<<loadcount[v]<<endl; }
	if(maxloadcount_vs < maxnumactvvs_vs){ cout<<"MERGE::EXCHANGE:: too many active vertices. Use ACTS in next iteration. [maxloadcount_vs: "<<maxloadcount_vs<<", maxnumactvvs_vs: "<<maxnumactvvs_vs<<"]"<<endl; }
	else{ cout<<"MERGE::EXCHANGE:: few active vertices. Use TradGP in next iteration. [maxloadcount_vs: "<<maxloadcount_vs<<", maxnumactvvs_vs: "<<maxnumactvvs_vs<<"]"<<endl; }
	#endif 
	#endif 
	
	// judging what mode (ACTS or TradGP) to use for next iteration
	#if not defined (ALLVERTEXISACTIVE_ALGORITHM) && defined(CONFIG_HYBRIDGPMODE) && defined(CONFIG_HYBRIDGPMODE_ONLINESWITCHING)
	unsigned int _mode = ON; if(maxloadcount_vs <= maxnumactvvs_vs){ _mode = OFF; } else { _mode = ON; }
	UTILP0_SetFirstData(mdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_MAILBOX + nextGraphIter, _mode);
	UTILP0_SetFirstData(vdramA, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_MAILBOX + nextGraphIter, _mode);
	UTILP0_SetFirstData(vdramB, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_MAILBOX + nextGraphIter, _mode);
	UTILP0_SetFirstData(vdramC, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_MAILBOX + nextGraphIter, _mode);
	#ifdef _DEBUGMODE_KERNELPRINTS3
	if(_mode == OFF){ cout<<"MERGE::EXCHANGE:: few active vertices. Use TradGP in next iteration. [maxloadcount_vs: "<<maxloadcount_vs<<", maxnumactvvs_vs: "<<maxnumactvvs_vs<<"]"<<endl; }	
	else{ cout<<"MERGE::EXCHANGE:: too many active vertices. Use ACTS in next iteration. [maxloadcount_vs: "<<maxloadcount_vs<<", maxnumactvvs_vs: "<<maxnumactvvs_vs<<"]"<<endl; }
	#endif 
	#endif 
	
	#ifdef _DEBUGMODE_STATS
	unsigned int num_acvvs = 0;
	num_acvvs += vdramA[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID - 1].data[0].key;
	num_acvvs += vdramB[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID - 1].data[0].key;
	num_acvvs += vdramC[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID - 1].data[0].key;
	mdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID - 1].data[0].key = num_acvvs;
	cout<< TIMINGRESULTSCOLOR << ">>> MERGE::EXCHANGE:: number of active vertices for next iteration (iteration "<<globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID<<"): "<<num_acvvs<< RESET << endl;
	#endif 
	
	// exit(EXIT_SUCCESS); //
	return;
}

void acts_all::TradGPP0(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC, uint512_dt * mdram){
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<"TradGP:: running traditional sssp... "<<endl;
	#endif 
	
	globalparams_t globalparamsm = UTILP0_getglobalparams(mdram, 0);
	globalparams_t globalparamsvA = UTILP0_getglobalparams(vdramA, 0);
	globalparams_t globalparamsvB = UTILP0_getglobalparams(vdramB, 0);
	globalparams_t globalparamsvC = UTILP0_getglobalparams(vdramC, 0);
	
	unsigned int vpmaskbuffer[BLOCKRAM_CUMMTVPMASK_SIZE];
	
	unsigned int GraphIter = globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID;
	unsigned int actvvcount_currit = globalparamsm.SIZE_ACTIVEVERTICES;
	unsigned int actvvcount_nextit = 0;
	unsigned int nextGraphIter = GraphIter + 1;
	cout<<"TradGP:: running traditional sssp... (iteration "<<GraphIter<<", number of active vertices to process: "<<actvvcount_currit<<")"<<endl;
	
	unsigned int offsetkvs_curractvvs;
	unsigned int offsetkvs_nxtactvvs;
	if(GraphIter % 2 == 0){ offsetkvs_curractvvs = 0; offsetkvs_nxtactvvs = globalparamsm.SIZE_VERTEXPTRS / VECTOR_SIZE; } 
	else { offsetkvs_curractvvs = globalparamsm.SIZE_VERTEXPTRS / VECTOR_SIZE; offsetkvs_nxtactvvs = 0; }
	
	unsigned int total_edges_processed = 0;
	long double edgesprocessedinGraphIter[128]; for(unsigned int i=0; i<128; i++){ edgesprocessedinGraphIter[i] = 0; }
	for(unsigned int v=0; v<globalparamsvA.NUM_PROCESSEDGESPARTITIONS; v++){ vpmaskbuffer[v] = 0; }
	
	for(unsigned int i=0; i<actvvcount_currit; i++){
		unsigned int vid = UTILP0_GetData(mdram, globalparamsm.BASEOFFSETKVS_ACTIVEVERTICES + offsetkvs_curractvvs, i); 
		if(vid == INVALIDDATA || vid > globalparamsm.SIZE_VERTEXPTRS){ continue; } // if(vid == INVALIDDATA){ continue; }
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_collects: ERROR 20", vid, globalparamsm.SIZE_VERTEXPTRS, actvvcount_currit, offsetkvs_curractvvs, NAp);
		#endif
		edge_t vptr_begin = UTILP0_GetData(mdram, globalparamsm.BASEOFFSETKVS_VERTEXPTR, vid); 
		edge_t vptr_end = UTILP0_GetData(mdram, globalparamsm.BASEOFFSETKVS_VERTEXPTR, vid + 1); 
		
		edge_t edges_size = vptr_end - vptr_begin;
		if(vptr_end < vptr_begin){ edges_size = 0; } // continue; // FIXME.
		#ifdef _DEBUGMODE_CHECKS
		if(vptr_end < vptr_begin){ cout<<"TradGP:: ERROR: vptr_end("<<vptr_end<<") < vptr_begin("<<vptr_begin<<"). vid: "<<vid<<". exiting..."<<endl; exit(EXIT_FAILURE); }
		#endif
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
		cout<<"~~~ TradGP:: VERTEX: vid: "<<vid<<", vptr_begin: "<<vptr_begin<<", vptr_end: "<<vptr_end<<", edges_size: "<<edges_size<<endl;
		#endif
		
		unsigned int s = vid % NUM_PEs;
		unsigned int lvid = (vid - s) / NUM_PEs; 
		unsigned int offset_kvs = s * globalparamsvA.NUM_REDUCEPARTITIONS * globalparamsvA.SIZEKVS2_REDUCEPARTITION;
		keyvalue_vbuffer_t res = MEMCAP0_READVDATA(UTILP0_GetData(vdramA, globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, lvid)) + 1; 
		
		for(unsigned int k=0; k<edges_size; k++){
			unsigned int dstvid = UTILP0_GetData(mdram, globalparamsm.BASEOFFSETKVS_EDGESDATA, vptr_begin + k);
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			cout<<"TradGP:: EDGE: vid: "<<vid<<", dstvid: "<<dstvid<<endl;
			#endif 
			
			unsigned int s = dstvid % NUM_PEs;
			unsigned int ldstvid = (dstvid - s) / NUM_PEs; 
			unsigned int offset_kvs = s * globalparamsvA.NUM_REDUCEPARTITIONS * globalparamsvA.SIZEKVS2_REDUCEPARTITION;
			
			value_t vprop = MEMCAP0_READVDATA(UTILP0_GetData(vdramA, globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, ldstvid)); 
			value_t vtemp = min(vprop, res);
			
			if(vtemp != vprop){
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				cout<<"TradGP:: ACTIVE VERTEX for next iteration: dstvid: "<<dstvid<<endl;
				#endif
				UTILP0_SetData(mdram, globalparamsm.BASEOFFSETKVS_ACTIVEVERTICES + offsetkvs_nxtactvvs, actvvcount_nextit, dstvid); actvvcount_nextit += 1; 
			}
			
			if(vtemp != vprop){
				UTILP0_WRITETO_UINT((unsigned int *)&vtemp, OFFSETOF_VMASK, SIZEOF_VMASK, 1);  // write active mask before save
				UTILP0_SetData(vdramA, globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, ldstvid, vtemp);
				UTILP0_SetData(vdramB, globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, ldstvid, vtemp);
				UTILP0_SetData(vdramC, globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, ldstvid, vtemp);
				
				vpmaskbuffer[dstvid / PROCESSPARTITIONSZ] = 1; 
			}
		
			total_edges_processed += 1;
			edgesprocessedinGraphIter[GraphIter] += 1;
		}
	}
	
	UTILP0_SetFirstData(mdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_ACTIVEVERTICES, actvvcount_nextit);
	if(actvvcount_nextit == 0){ UTILP0_SetFirstData(mdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES, 1); } 
	else { UTILP0_SetFirstData(mdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES, 0); }
	
	UTILP0_SetFirstData(mdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID, nextGraphIter);
	UTILP0_SetFirstData(vdramA, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID, nextGraphIter);
	UTILP0_SetFirstData(vdramB, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID, nextGraphIter);
	UTILP0_SetFirstData(vdramC, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID, nextGraphIter);
	
	cout<<"TradGP: number of active vertices for iteration "<<nextGraphIter<<": "<<actvvcount_nextit<<""<<endl;
	if(actvvcount_nextit == 0){ cout<<"TradGP: no more active vertices to process. breaking out... "<<endl; }
	
	// vertices partition masks
	#ifdef CONFIG_MERGE_VPARTITIONS
	MERGEP0_EXCHANGEVPMS_LOOP1: for(unsigned int partition=0; partition<BLOCKRAM_NEXTPMASK_SIZE; partition++){
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGE::EXCHANGE::: ERROR 31", globalparamsvA.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition, TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
		actsutilityobj->checkoutofbounds("MERGE::EXCHANGE::: ERROR 32", globalparamsvB.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition, TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
		actsutilityobj->checkoutofbounds("MERGE::EXCHANGE::: ERROR 33", globalparamsvC.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition, TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
		#endif
		
		#ifdef _WIDEWORD
		vdramA[globalparamsvA.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0) = vpmaskbuffer[partition];
		vdramB[globalparamsvB.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0) = vpmaskbuffer[partition];
		vdramC[globalparamsvC.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0) = vpmaskbuffer[partition];
		#else 
		vdramA[globalparamsvA.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key = vpmaskbuffer[partition];
		vdramB[globalparamsvB.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key = vpmaskbuffer[partition];
		vdramC[globalparamsvC.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key = vpmaskbuffer[partition];
		#endif 
		
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"--- TOPP0_topkernelS: vdramA["<<partition<<"]: "<<p<<endl; 
		#endif
	}
	#endif 
	
	// judging what mode (ACTS or TradGP) to use for next iteration
	unsigned int _mode = ON; if(actvvcount_nextit < (ACTVVBUFFER_SIZE * VECTOR2_SIZE)){ _mode = OFF; } else { _mode = ON; }
	#ifdef CONFIG_HYBRIDGPMODE_ONLINESWITCHING
	UTILP0_SetFirstData(mdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_MAILBOX + nextGraphIter, _mode);
	UTILP0_SetFirstData(vdramA, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_MAILBOX + nextGraphIter, _mode);
	UTILP0_SetFirstData(vdramB, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_MAILBOX + nextGraphIter, _mode);
	UTILP0_SetFirstData(vdramC, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_MAILBOX + nextGraphIter, _mode);
	#ifdef _DEBUGMODE_KERNELPRINTS3
	if(_mode == OFF){ cout<<"TradGP:: few active vertices. Use TradGP in next iteration. [actvvcount_nextit: "<<actvvcount_nextit<<", ACTVVBUFFER_SIZE * VECTOR2_SIZE: "<<ACTVVBUFFER_SIZE * VECTOR2_SIZE<<"]"<<endl; }	
	else{ cout<<"TradGP:: too many active vertices. Use ACTS in next iteration. [actvvcount_nextit: "<<actvvcount_nextit<<", ACTVVBUFFER_SIZE * VECTOR2_SIZE: "<<ACTVVBUFFER_SIZE * VECTOR2_SIZE<<"]"<<endl; }
	#endif 
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}

extern "C" {
void acts_all::TOPP0_topkernelS(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC, uint512_dt * mdram){
#pragma HLS INTERFACE m_axi port = vdramA offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = vdramB offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = vdramC offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = mdram offset = slave bundle = gmem3

#pragma HLS INTERFACE s_axilite port = vdramA bundle = control
#pragma HLS INTERFACE s_axilite port = vdramB bundle = control
#pragma HLS INTERFACE s_axilite port = vdramC bundle = control
#pragma HLS INTERFACE s_axilite port = mdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATA_PACK variable = vdramA
#pragma HLS DATA_PACK variable = vdramB
#pragma HLS DATA_PACK variable = vdramC
#pragma HLS DATA_PACK variable = mdram

	globalparams_t globalparamsvA = UTILP0_getglobalparams(vdramA, 0);
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"~~~ TOPP0_topkernelS:: GraphIter: "<<globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID<<", globalparamsvA.MAILBOX["<<globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID<<"]: "<<globalparamsvA.MAILBOX[globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID]<<endl;
	#endif
	
	#ifdef ALLVERTEXISACTIVE_ALGORITHM
	MERGEP0_exchange(vdramA, vdramB, vdramC, mdram);
	#else
		#ifdef CONFIG_HYBRIDGPMODE
			if(globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID > 0){
				if(globalparamsvA.MAILBOX[globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID - 1] == ON){
					#ifdef _DEBUGMODE_KERNELPRINTS3
					cout<<"TOPP0_topkernelS:: HybridMode: ACTS executing..."<<endl;
					#endif
					MERGEP0_exchange(vdramA, vdramB, vdramC, mdram);
				}
			}
			if(globalparamsvA.MAILBOX[globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID] != ON){
				#ifdef _DEBUGMODE_KERNELPRINTS3
				cout<<"TOPP0_topkernelS:: HybridMode: TradGP executing..."<<endl;
				#endif 
				TradGPP0(vdramA, vdramB, vdramC, mdram);
			}
		#elif defined(CONFIG_ACTSONLYMODE)
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"TOPP0_topkernelS:: ACTS Only Mode executing..."<<endl;
			#endif 
			MERGEP0_exchange(vdramA, vdramB, vdramC, mdram);	
		#elif defined(CONFIG_TRADGPONLYMODE)
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"TOPP0_topkernelS:: TradGP Only Mode executing..."<<endl;
			#endif 
			TradGPP0(vdramA, vdramB, vdramC, mdram);
		#else 
		NOT DEFINED.
		#endif 
	#endif
	return;
}
}
	



