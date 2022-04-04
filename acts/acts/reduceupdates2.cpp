void acts_all::REDUCEP0_reducevector(bool enx, unsigned int col, keyvalue_buffer_t kvdata, keyvalue_vbuffer_t vbuffer[BLOCKRAM_VDATA_SIZE], buffer_type destoffset, unsigned int upperlimit, sweepparams_t sweepparams, globalparams_t globalparams){
	#pragma HLS PIPELINE II=3
	keyvalue_t mykeyvalue = UTILP0_GETKV(kvdata);
	vertex_t loc = ((mykeyvalue.key - upperlimit) - col) >> NUM_PARTITIONS_POW;
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"REDUCEP0_reducevector:: col: "<<col<<", loc: "<<loc<<", mykeyvalue.key: "<<mykeyvalue.key<<", upperlimit: "<<upperlimit<<", mykeyvalue.value: "<<mykeyvalue.value<<endl;
	#endif
	
	bool en = true;
	if(mykeyvalue.key == UTILP0_GETK(INVALIDDATA) || mykeyvalue.value == UTILP0_GETV(INVALIDDATA) || mykeyvalue.key == MAXVDATA || mykeyvalue.value == MAXVDATA){ en = false; } // REMOVEME?????????????????????????????
	if(mykeyvalue.key == 0 && mykeyvalue.value == 0){ en = false; }
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	if(en == true){ cout<<"$$$ REDUCE VECTOR:: REDUCE SEEN @ instance "<<globalparams.ACTSPARAMS_INSTID<<", col: "<<col<<", loc: "<<loc<<", vid: "<<UTILP0_GETREALVID(mykeyvalue.key, globalparams.ACTSPARAMS_INSTID)<<", partition: "<<UTILP0_GETREALVID(mykeyvalue.key, globalparams.ACTSPARAMS_INSTID) / PROCESSPARTITIONSZ<<", mykeyvalue.key: "<<mykeyvalue.key<<", mykeyvalue.value: "<<mykeyvalue.value<<", upperlimit: "<<upperlimit<<", reduce size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<endl; }
	#endif 
	
	if(loc >= (globalparams.SIZEKVS2_REDUCEPARTITION * VDATA_SHRINK_RATIO) && en == true){
		#ifdef ENABLE_PERFECTACCURACY
		if(true){ cout<<"REDUCEP0_reducevector::ERROR SEEN @ loc("<<loc<<") >= globalparams.SIZE_REDUCE("<<globalparams.SIZE_REDUCE<<"). mykeyvalue.key: "<<mykeyvalue.key<<", upperlimit: "<<upperlimit<<", col: "<<col<<". EXITING... "<<endl; exit(EXIT_FAILURE); }
		actsutilityobj->reducehelper_checkreduceloc(0, loc, mykeyvalue, sweepparams, globalparams); 
		#endif 
		loc = 0; }
		
	// read 
	vmdata_t vmdata; vmdata.vmask = 0;
	unsigned int bufferoffset_kvs = 0; 
	#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
	if(globalparams.ACTSPARAMS_TREEDEPTH == 1){ bufferoffset_kvs = BLOCKRAM_VDATA_SIZE / 2; } else { bufferoffset_kvs = 0; }
	#endif 
	#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
		if(en == true){ vmdata = MEMCAP0_READFROMBUFFER_VDATAWITHVMASK2(loc, NAp, vbuffer, bufferoffset_kvs); }
			#else 
				if(en == true){ vmdata = MEMCAP0_READFROMBUFFER_VDATAWITHVMASK(loc, vbuffer, 0); }
					#endif
					
	// reduce 
	value_t new_vprop = REDUCEP0_reducefunc(vmdata.vdata, mykeyvalue.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	if(en == true && new_vprop != vmdata.vdata){ vmdata.vmask = 1; } else { vmdata.vmask = 0; } // NEWCHANGE.
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	if(en == true && new_vprop != vmdata.vdata){ cout<<">>> REDUCE VECTOR:: ACTIVE MASK SEEN AT: loc: "<<loc<<", vmdata.vdata: "<<vmdata.vdata<<", vmdata.vmask: "<<vmdata.vmask<<endl; }
	if(en == true && new_vprop != vmdata.vdata){ cout<<">>> REDUCE VECTOR:: ACTIVE REDUCE SEEN @ instance "<<globalparams.ACTSPARAMS_INSTID<<", col: "<<col<<", loc: "<<loc<<", vid: "<<UTILP0_GETREALVID(mykeyvalue.key, globalparams.ACTSPARAMS_INSTID)<<endl; }
	#endif
	
	// write
	#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
		if(en == true){ MEMCAP0_WRITETOBUFFER_VDATAWITHVMASK2(loc, vbuffer, new_vprop, vmdata.vmask, bufferoffset_kvs); }
			#else 
				if(en == true){ MEMCAP0_WRITETOBUFFER_VDATAWITHVMASK(loc, vbuffer, new_vprop, vmdata.vmask, 0); }
					#endif

	#ifdef _DEBUGMODE_STATS
	actsutilityobj->globalstats_countkvsreduced(1);
	if(en == true){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(1); }
	#endif
	return;
}

void acts_all::REDUCEP0_GetXYLayoutV(unsigned int s, unsigned int depths[VECTOR_SIZE], unsigned int basedepth){
	unsigned int s_ = s % VECTOR_SIZE;
	
 if(s_==0){ 
		depths[0] = 0; 
		depths[1] = 1; 
		depths[2] = 2; 
		depths[3] = 3; 
		depths[4] = 4; 
		depths[5] = 5; 
		depths[6] = 6; 
		depths[7] = 7; 
	}
else if(s_==1){ 
		depths[0] = 7; 
		depths[1] = 0; 
		depths[2] = 1; 
		depths[3] = 2; 
		depths[4] = 3; 
		depths[5] = 4; 
		depths[6] = 5; 
		depths[7] = 6; 
	}
else if(s_==2){ 
		depths[0] = 6; 
		depths[1] = 7; 
		depths[2] = 0; 
		depths[3] = 1; 
		depths[4] = 2; 
		depths[5] = 3; 
		depths[6] = 4; 
		depths[7] = 5; 
	}
else if(s_==3){ 
		depths[0] = 5; 
		depths[1] = 6; 
		depths[2] = 7; 
		depths[3] = 0; 
		depths[4] = 1; 
		depths[5] = 2; 
		depths[6] = 3; 
		depths[7] = 4; 
	}
else if(s_==4){ 
		depths[0] = 4; 
		depths[1] = 5; 
		depths[2] = 6; 
		depths[3] = 7; 
		depths[4] = 0; 
		depths[5] = 1; 
		depths[6] = 2; 
		depths[7] = 3; 
	}
else if(s_==5){ 
		depths[0] = 3; 
		depths[1] = 4; 
		depths[2] = 5; 
		depths[3] = 6; 
		depths[4] = 7; 
		depths[5] = 0; 
		depths[6] = 1; 
		depths[7] = 2; 
	}
else if(s_==6){ 
		depths[0] = 2; 
		depths[1] = 3; 
		depths[2] = 4; 
		depths[3] = 5; 
		depths[4] = 6; 
		depths[5] = 7; 
		depths[6] = 0; 
		depths[7] = 1; 
	}
else { 
		depths[0] = 1; 
		depths[1] = 2; 
		depths[2] = 3; 
		depths[3] = 4; 
		depths[4] = 5; 
		depths[5] = 6; 
		depths[6] = 7; 
		depths[7] = 0; 
	}
	return;
}

void acts_all::REDUCEP0_RearrangeLayoutV(unsigned int s, keyvalue_buffer_t vdata[VECTOR_SIZE], keyvalue_buffer_t vdata2[VECTOR_SIZE]){
	unsigned int s_ = s;// % VECTOR_SIZE;
 if(s_==0){ 
		vdata2[0] = vdata[0]; 
		vdata2[1] = vdata[1]; 
		vdata2[2] = vdata[2]; 
		vdata2[3] = vdata[3]; 
		vdata2[4] = vdata[4]; 
		vdata2[5] = vdata[5]; 
		vdata2[6] = vdata[6]; 
		vdata2[7] = vdata[7]; 
	}
else if(s_==1){ 
		vdata2[7] = vdata[0]; 
		vdata2[0] = vdata[1]; 
		vdata2[1] = vdata[2]; 
		vdata2[2] = vdata[3]; 
		vdata2[3] = vdata[4]; 
		vdata2[4] = vdata[5]; 
		vdata2[5] = vdata[6]; 
		vdata2[6] = vdata[7]; 
	}
else if(s_==2){ 
		vdata2[6] = vdata[0]; 
		vdata2[7] = vdata[1]; 
		vdata2[0] = vdata[2]; 
		vdata2[1] = vdata[3]; 
		vdata2[2] = vdata[4]; 
		vdata2[3] = vdata[5]; 
		vdata2[4] = vdata[6]; 
		vdata2[5] = vdata[7]; 
	}
else if(s_==3){ 
		vdata2[5] = vdata[0]; 
		vdata2[6] = vdata[1]; 
		vdata2[7] = vdata[2]; 
		vdata2[0] = vdata[3]; 
		vdata2[1] = vdata[4]; 
		vdata2[2] = vdata[5]; 
		vdata2[3] = vdata[6]; 
		vdata2[4] = vdata[7]; 
	}
else if(s_==4){ 
		vdata2[4] = vdata[0]; 
		vdata2[5] = vdata[1]; 
		vdata2[6] = vdata[2]; 
		vdata2[7] = vdata[3]; 
		vdata2[0] = vdata[4]; 
		vdata2[1] = vdata[5]; 
		vdata2[2] = vdata[6]; 
		vdata2[3] = vdata[7]; 
	}
else if(s_==5){ 
		vdata2[3] = vdata[0]; 
		vdata2[4] = vdata[1]; 
		vdata2[5] = vdata[2]; 
		vdata2[6] = vdata[3]; 
		vdata2[7] = vdata[4]; 
		vdata2[0] = vdata[5]; 
		vdata2[1] = vdata[6]; 
		vdata2[2] = vdata[7]; 
	}
else if(s_==6){ 
		vdata2[2] = vdata[0]; 
		vdata2[3] = vdata[1]; 
		vdata2[4] = vdata[2]; 
		vdata2[5] = vdata[3]; 
		vdata2[6] = vdata[4]; 
		vdata2[7] = vdata[5]; 
		vdata2[0] = vdata[6]; 
		vdata2[1] = vdata[7]; 
	}
else { 
		vdata2[1] = vdata[0]; 
		vdata2[2] = vdata[1]; 
		vdata2[3] = vdata[2]; 
		vdata2[4] = vdata[3]; 
		vdata2[5] = vdata[4]; 
		vdata2[6] = vdata[5]; 
		vdata2[7] = vdata[6]; 
		vdata2[0] = vdata[7]; 
	}
	return;
}

void acts_all::REDUCEP0_RearrangeLayoutEn(unsigned int s, bool en[VECTOR_SIZE], bool en2[VECTOR_SIZE]){
	unsigned int s_ = s;// % VECTOR_SIZE;
 if(s_==0){ 
		en2[0] = en[0]; 
		en2[1] = en[1]; 
		en2[2] = en[2]; 
		en2[3] = en[3]; 
		en2[4] = en[4]; 
		en2[5] = en[5]; 
		en2[6] = en[6]; 
		en2[7] = en[7]; 
	}
else if(s_==1){ 
		en2[7] = en[0]; 
		en2[0] = en[1]; 
		en2[1] = en[2]; 
		en2[2] = en[3]; 
		en2[3] = en[4]; 
		en2[4] = en[5]; 
		en2[5] = en[6]; 
		en2[6] = en[7]; 
	}
else if(s_==2){ 
		en2[6] = en[0]; 
		en2[7] = en[1]; 
		en2[0] = en[2]; 
		en2[1] = en[3]; 
		en2[2] = en[4]; 
		en2[3] = en[5]; 
		en2[4] = en[6]; 
		en2[5] = en[7]; 
	}
else if(s_==3){ 
		en2[5] = en[0]; 
		en2[6] = en[1]; 
		en2[7] = en[2]; 
		en2[0] = en[3]; 
		en2[1] = en[4]; 
		en2[2] = en[5]; 
		en2[3] = en[6]; 
		en2[4] = en[7]; 
	}
else if(s_==4){ 
		en2[4] = en[0]; 
		en2[5] = en[1]; 
		en2[6] = en[2]; 
		en2[7] = en[3]; 
		en2[0] = en[4]; 
		en2[1] = en[5]; 
		en2[2] = en[6]; 
		en2[3] = en[7]; 
	}
else if(s_==5){ 
		en2[3] = en[0]; 
		en2[4] = en[1]; 
		en2[5] = en[2]; 
		en2[6] = en[3]; 
		en2[7] = en[4]; 
		en2[0] = en[5]; 
		en2[1] = en[6]; 
		en2[2] = en[7]; 
	}
else if(s_==6){ 
		en2[2] = en[0]; 
		en2[3] = en[1]; 
		en2[4] = en[2]; 
		en2[5] = en[3]; 
		en2[6] = en[4]; 
		en2[7] = en[5]; 
		en2[0] = en[6]; 
		en2[1] = en[7]; 
	}
else { 
		en2[1] = en[0]; 
		en2[2] = en[1]; 
		en2[3] = en[2]; 
		en2[4] = en[3]; 
		en2[5] = en[4]; 
		en2[6] = en[5]; 
		en2[7] = en[6]; 
		en2[0] = en[7]; 
	}
	return;
}

void acts_all::REDUCEP0_reduceandbuffer(bool_type enable, keyvalue_buffer_t buffer[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams){				
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount = (DESTBLOCKRAM_SIZE / (NUM_PARTITIONS / 2)); // =46: '2' is safety padding.
	// exit(EXIT_SUCCESS);
	
	keyvalue_buffer_t kvdata[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=kvdata complete
	keyvalue_buffer_t kvdata2[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=kvdata2 complete
	unsigned int depths[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=depths complete
	bool enx[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=enx complete
	bool enx2[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=enx2 complete
	unsigned int d_kvs[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=d_kvs complete
	keyvalue_capsule_t localcapsule_kvs[MAX_NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=localcapsule_kvs complete
	
	keyvalue_t invalid_dataa; invalid_dataa.key = INVALIDDATA; invalid_dataa.value = INVALIDDATA;
	keyvalue_buffer_t invalid_data = UTILP0_GETKV(invalid_dataa);
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printkeyvalues("reduceandbuffer.localcapsule", (keyvalue_t *)localcapsule, NUM_PARTITIONS);
	#endif
	
	buffer_type maxsize_kvs[2]; maxsize_kvs[0] = 0; maxsize_kvs[1] = 0;
	unsigned int _poff = 0;
	REDUCEBUFFERPARTITIONS_LOOP1: for(unsigned int cid=0; cid<2; cid++){ // NB: 'NUM_PARTITIONS is 16, but only 8 can be accessed per time, so we need 2 fetches'
		REDUCEBUFFERPARTITIONS_LOOP1B: for(partition_type p=0; p<NUM_PARTITIONS/2; p++){
		#pragma HLS PIPELINE II=1
			unsigned int ssize_kvs = localcapsule[_poff + p].value / VECTOR_SIZE;
			if(maxsize_kvs[cid] < ssize_kvs){ maxsize_kvs[cid] = ssize_kvs; }
		}
		_poff += NUM_PARTITIONS/2;
	}
	buffer_type height_kvs = (localcapsule[NUM_PARTITIONS-1].key + localcapsule[NUM_PARTITIONS-1].value) / VECTOR_SIZE;
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"reduceupdates2: maxsize_kvs[0]: "<<maxsize_kvs[0]<<", maxsize_kvs[1]: "<<maxsize_kvs[1]<<", height_kvs: "<<height_kvs<<endl;
	#endif
		
	for(partition_type p=0; p<NUM_PARTITIONS; p++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"reduceupdates2: localcapsule["<<p<<"].key: "<<localcapsule[p].key<<", localcapsule["<<p<<"].value: "<<localcapsule[p].value<<endl;
		#endif
		localcapsule_kvs[p].key = localcapsule[p].key / VECTOR_SIZE;
		localcapsule_kvs[p].value = (localcapsule[p].value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	}
	
	REDUCEBUFFERPARTITIONS_LOOP2: for(buffer_type capsule_offset=0; capsule_offset<NUM_PARTITIONS; capsule_offset+=VECTOR_SIZE){
		unsigned int mmaxsz_kvs = maxsize_kvs[capsule_offset / VECTOR_SIZE];
		REDUCEBUFFERPARTITIONS_LOOP2B: for(unsigned int r=0; r<VECTOR_SIZE; r++){
			REDUCEBUFFERPARTITIONS_LOOP2C: for(buffer_type i=0; i<mmaxsz_kvs; i++){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
			#pragma HLS PIPELINE II=3
				// get layout
				REDUCEP0_GetXYLayoutV(r, depths, 0);
			
				// calculate enable
				enx[0] = true;	
				enx[1] = true;	
				enx[2] = true;	
				enx[3] = true;	
				enx[4] = true;	
				enx[5] = true;	
				enx[6] = true;	
				enx[7] = true;	
 
				
				// read 
				unsigned int tdepth0 = capsule_offset + depths[0];
				d_kvs[0] = localcapsule_kvs[tdepth0].key + i; 	
				unsigned int tdepth1 = capsule_offset + depths[1];
				d_kvs[1] = localcapsule_kvs[tdepth1].key + i; 	
				unsigned int tdepth2 = capsule_offset + depths[2];
				d_kvs[2] = localcapsule_kvs[tdepth2].key + i; 	
				unsigned int tdepth3 = capsule_offset + depths[3];
				d_kvs[3] = localcapsule_kvs[tdepth3].key + i; 	
				unsigned int tdepth4 = capsule_offset + depths[4];
				d_kvs[4] = localcapsule_kvs[tdepth4].key + i; 	
				unsigned int tdepth5 = capsule_offset + depths[5];
				d_kvs[5] = localcapsule_kvs[tdepth5].key + i; 	
				unsigned int tdepth6 = capsule_offset + depths[6];
				d_kvs[6] = localcapsule_kvs[tdepth6].key + i; 	
				unsigned int tdepth7 = capsule_offset + depths[7];
				d_kvs[7] = localcapsule_kvs[tdepth7].key + i; 	
 	
 // FIXME.
				if(i < localcapsule_kvs[tdepth0].value){ enx[0] = true; kvdata[0] = buffer[0][d_kvs[0]]; } else { enx[0] = false; kvdata[0] = invalid_data; }	
 // FIXME.
				if(i < localcapsule_kvs[tdepth1].value){ enx[1] = true; kvdata[1] = buffer[1][d_kvs[1]]; } else { enx[1] = false; kvdata[1] = invalid_data; }	
 // FIXME.
				if(i < localcapsule_kvs[tdepth2].value){ enx[2] = true; kvdata[2] = buffer[2][d_kvs[2]]; } else { enx[2] = false; kvdata[2] = invalid_data; }	
 // FIXME.
				if(i < localcapsule_kvs[tdepth3].value){ enx[3] = true; kvdata[3] = buffer[3][d_kvs[3]]; } else { enx[3] = false; kvdata[3] = invalid_data; }	
 // FIXME.
				if(i < localcapsule_kvs[tdepth4].value){ enx[4] = true; kvdata[4] = buffer[4][d_kvs[4]]; } else { enx[4] = false; kvdata[4] = invalid_data; }	
 // FIXME.
				if(i < localcapsule_kvs[tdepth5].value){ enx[5] = true; kvdata[5] = buffer[5][d_kvs[5]]; } else { enx[5] = false; kvdata[5] = invalid_data; }	
 // FIXME.
				if(i < localcapsule_kvs[tdepth6].value){ enx[6] = true; kvdata[6] = buffer[6][d_kvs[6]]; } else { enx[6] = false; kvdata[6] = invalid_data; }	
 // FIXME.
				if(i < localcapsule_kvs[tdepth7].value){ enx[7] = true; kvdata[7] = buffer[7][d_kvs[7]]; } else { enx[7] = false; kvdata[7] = invalid_data; }	
				
				// re-arrange 
				REDUCEP0_RearrangeLayoutV(r, kvdata, kvdata2);
				REDUCEP0_RearrangeLayoutEn(r, enx, enx2); // NEWCHANGE.
			
				// reduce 
				REDUCEP0_reducevector(enx2[0], 0, kvdata2[0], vbuffer[capsule_offset + 0], 0, sweepparams.upperlimit, sweepparams, globalparams);
				REDUCEP0_reducevector(enx2[1], 1, kvdata2[1], vbuffer[capsule_offset + 1], 0, sweepparams.upperlimit, sweepparams, globalparams);
				REDUCEP0_reducevector(enx2[2], 2, kvdata2[2], vbuffer[capsule_offset + 2], 0, sweepparams.upperlimit, sweepparams, globalparams);
				REDUCEP0_reducevector(enx2[3], 3, kvdata2[3], vbuffer[capsule_offset + 3], 0, sweepparams.upperlimit, sweepparams, globalparams);
				REDUCEP0_reducevector(enx2[4], 4, kvdata2[4], vbuffer[capsule_offset + 4], 0, sweepparams.upperlimit, sweepparams, globalparams);
				REDUCEP0_reducevector(enx2[5], 5, kvdata2[5], vbuffer[capsule_offset + 5], 0, sweepparams.upperlimit, sweepparams, globalparams);
				REDUCEP0_reducevector(enx2[6], 6, kvdata2[6], vbuffer[capsule_offset + 6], 0, sweepparams.upperlimit, sweepparams, globalparams);
				REDUCEP0_reducevector(enx2[7], 7, kvdata2[7], vbuffer[capsule_offset + 7], 0, sweepparams.upperlimit, sweepparams, globalparams);
			}
		}
	}
	// actsutilityobj->printglobalvars();
	// exit(EXIT_SUCCESS); ////
	return;
}

#ifdef BASIC_PARTITION_AND_REDUCE_STRETEGY
void acts_all::REDUCEP0_priorreduceandbuffer(bool_type enable, keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], buffer_type chunk_size, sweepparams_t sweepparams, globalparams_t globalparams){				
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"reduceupdates2: REDUCEP0_tradreduceandbuffer NOT DEFINED HERE."<<endl;
	exit(EXIT_FAILURE);
	#endif 
	return;
}
#endif 

#ifdef TRAD_PARTITION_AND_REDUCE_STRETEGY
void acts_all::REDUCEP0_tradreduceandbuffer(bool_type enable, uint512_dt * kvdram, keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], buffer_type chunk_size, keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], sweepparams_t sweepparams, globalparams_t globalparams){				
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"reduceupdates2: REDUCEP0_tradreduceandbuffer NOT DEFINED HERE."<<endl;
	exit(EXIT_FAILURE);
	#endif 
	return;
}
#endif 
