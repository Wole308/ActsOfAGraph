void acts_all::REDUCEP0_reducevector(unsigned int col, keyvalue_buffer_t kvdata, keyvalue_vbuffer_t destbuffer[BLOCKRAM_VDATA_SIZE], buffer_type destoffset, unsigned int upperlimit, sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams){
	#pragma HLS PIPELINE II=3
	keyvalue_t mykeyvalue = UTILP0_GETKV(kvdata);
	vertex_t loc = ((mykeyvalue.key - upperlimit) - col) >> NUM_PARTITIONS_POW;
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"REDUCEP0_reducevector:: col: "<<col<<", loc: "<<loc<<", mykeyvalue.key: "<<mykeyvalue.key<<", mykeyvalue.value: "<<mykeyvalue.value<<endl;
	#endif
	
	bool en = true;
	// if(mykeyvalue.key != UTILP0_GETK(INVALIDDATA) && mykeyvalue.value != UTILP0_GETV(INVALIDDATA)){ en = true; } else { en = false; }
	if(mykeyvalue.key == UTILP0_GETK(INVALIDDATA) || mykeyvalue.value == UTILP0_GETV(INVALIDDATA) || mykeyvalue.key == MAXVDATA || mykeyvalue.value == MAXVDATA){ en = false; } // REMOVEME?????????????????????????????
	if(mykeyvalue.key == 0 && mykeyvalue.value == 0){ en = false; }
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	if(en == true){ cout<<"reducevector:: REDUCE SEEN @ instance "<<globalparams.ACTSPARAMS_INSTID<<", col: "<<col<<", loc: "<<loc<<", vid: "<<UTILP0_GETREALVID(mykeyvalue.key, globalparams.ACTSPARAMS_INSTID)<<", partition: "<<UTILP0_GETREALVID(mykeyvalue.key, globalparams.ACTSPARAMS_INSTID) / PROCESSPARTITIONSZ<<", mykeyvalue.key: "<<mykeyvalue.key<<", mykeyvalue.value: "<<mykeyvalue.value<<", upperlimit: "<<upperlimit<<", reduce size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<endl; }
	#endif 
	
	if(loc >= globalparams.SIZEKVS2_REDUCEPARTITION && en == true){ 
		#ifdef ENABLE_PERFECTACCURACY
		if(true){ cout<<"REDUCEP0_reducevector::ERROR SEEN @ loc("<<loc<<") >= globalparams.SIZE_REDUCE("<<globalparams.SIZE_REDUCE<<"). mykeyvalue.key: "<<mykeyvalue.key<<", upperlimit: "<<upperlimit<<", col: "<<col<<". EXITING... "<<endl; exit(EXIT_FAILURE); }
		actsutilityobj->reducehelper_checkreduceloc(0, loc, mykeyvalue, sweepparams, globalparams); 
		#endif 
		loc = 0; }
		
	// read 
	vmdata_t vmdata;
	#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
		if(en == true){ vmdata = MEMCAP0_READFROMBUFFER_VDATAWITHVMASK2(loc, NAp, destbuffer, 0); }
			#else 
				if(en == true){ vmdata = MEMCAP0_READFROMBUFFER_VDATAWITHVMASK(loc, destbuffer, 0); }
					#endif
	
	// reduce 
	value_t new_vprop = REDUCEP0_reducefunc(vmdata.vdata, mykeyvalue.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	if(en == true && new_vprop != vmdata.vdata){ vmdata.vmask = 1; }
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
	if(en == true && new_vprop != vmdata.vdata){ cout<<"REDUCEP0_reducevector:: ACTIVE MASK SEEN AT: "<<loc<<""<<endl; }
	#endif
	
	// write
	#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS // (globalposition.source_partition % VDATA_SHRINK_RATIO)
		if(en == true){ MEMCAP0_WRITETOBUFFER_VDATAWITHVMASK2(loc, destbuffer, new_vprop, vmdata.vmask, 0); }
			#else 
				if(en == true){ MEMCAP0_WRITETOBUFFER_VDATAWITHVMASK(loc, destbuffer, new_vprop, vmdata.vmask, 0); }
					#endif

	#ifdef _DEBUGMODE_STATS
	actsutilityobj->globalstats_countkvsreduced(1);
	if(en == true){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(1); }
	#endif
	return;
}

void acts_all::REDUCEP0_reduceandbuffer(bool_type enable, keyvalue_buffer_t buffer[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams){				
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount = (DESTBLOCKRAM_SIZE / (NUM_PARTITIONS / 2)); // =46: '2' is safety padding.
	// cout<<"--------------- REDUCEP0_reduceandbuffer"<<endl;
	// exit(EXIT_SUCCESS); //// 
	
	keyvalue_buffer_t kvdata0;
	keyvalue_buffer_t kvdata1;
	keyvalue_buffer_t kvdata2;
	keyvalue_buffer_t kvdata3;
	keyvalue_buffer_t kvdata4;
	keyvalue_buffer_t kvdata5;
	keyvalue_buffer_t kvdata6;
	keyvalue_buffer_t kvdata7;
	keyvalue_buffer_t kvdata8;
	keyvalue_buffer_t kvdata9;
	keyvalue_buffer_t kvdata10;
	keyvalue_buffer_t kvdata11;
	keyvalue_buffer_t kvdata12;
	keyvalue_buffer_t kvdata13;
	keyvalue_buffer_t kvdata14;
	keyvalue_buffer_t kvdata15;
	buffer_type bramoffset_kvs[MAX_NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=bramoffset_kvs complete
	buffer_type size_kvs[MAX_NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=size_kvs complete
	unsigned int upperlimits[MAX_NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=upperlimits complete
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printkeyvalues("reduceandbuffer.localcapsule", (keyvalue_t *)localcapsule, NUM_PARTITIONS);
	#endif
	
	REDUCEBUFFERPARTITIONS_LOOP1: for(unsigned int it=0; it<NUM_PARTITIONS; it+=NUM_PARTITIONS/2){
		buffer_type maxsize_kvs = 0;
		REDUCEBUFFERPARTITIONS_LOOP1B: for(partition_type p=0; p<NUM_PARTITIONS/2; p++){
		#pragma HLS PIPELINE II=1
			bramoffset_kvs[p] = localcapsule[it+p].key / VECTOR_SIZE;
			size_kvs[p] = localcapsule[it+p].value / VECTOR_SIZE;
			if(maxsize_kvs < size_kvs[p]){ maxsize_kvs = size_kvs[p]; }
		}
		
		REDUCEBUFFERPARTITIONS_LOOP1D: for(buffer_type i=0; i<maxsize_kvs; i++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
		// #pragma HLS PIPELINE II=16
		#pragma HLS PIPELINE II=32 // 16, 32, 48, FIXME? USE THIS INSTEAD?
		// #pragma HLS PIPELINE II=48
			for(vector_type v=0; v<VECTOR_SIZE; v++){
				kvdata0 = buffer[v][bramoffset_kvs[0] + i]; 	
				kvdata1 = buffer[v][bramoffset_kvs[1] + i]; 	
				kvdata2 = buffer[v][bramoffset_kvs[2] + i]; 	
				kvdata3 = buffer[v][bramoffset_kvs[3] + i]; 	
				kvdata4 = buffer[v][bramoffset_kvs[4] + i]; 	
				kvdata5 = buffer[v][bramoffset_kvs[5] + i]; 	
				kvdata6 = buffer[v][bramoffset_kvs[6] + i]; 	
				kvdata7 = buffer[v][bramoffset_kvs[7] + i]; 	

				if(i< size_kvs[0]){ REDUCEP0_reducevector(it+0, kvdata0, vbuffer[it+0], 0, sweepparams.upperlimit, sweepparams, globalposition, globalparams); }
				if(i< size_kvs[1]){ REDUCEP0_reducevector(it+1, kvdata1, vbuffer[it+1], 0, sweepparams.upperlimit, sweepparams, globalposition, globalparams); }
				if(i< size_kvs[2]){ REDUCEP0_reducevector(it+2, kvdata2, vbuffer[it+2], 0, sweepparams.upperlimit, sweepparams, globalposition, globalparams); }
				if(i< size_kvs[3]){ REDUCEP0_reducevector(it+3, kvdata3, vbuffer[it+3], 0, sweepparams.upperlimit, sweepparams, globalposition, globalparams); }
				if(i< size_kvs[4]){ REDUCEP0_reducevector(it+4, kvdata4, vbuffer[it+4], 0, sweepparams.upperlimit, sweepparams, globalposition, globalparams); }
				if(i< size_kvs[5]){ REDUCEP0_reducevector(it+5, kvdata5, vbuffer[it+5], 0, sweepparams.upperlimit, sweepparams, globalposition, globalparams); }
				if(i< size_kvs[6]){ REDUCEP0_reducevector(it+6, kvdata6, vbuffer[it+6], 0, sweepparams.upperlimit, sweepparams, globalposition, globalparams); }
				if(i< size_kvs[7]){ REDUCEP0_reducevector(it+7, kvdata7, vbuffer[it+7], 0, sweepparams.upperlimit, sweepparams, globalposition, globalparams); }
			}
		}
	}
	return;
}

void acts_all::REDUCEP0_priorreduceandbuffer(bool_type enable, keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], buffer_type chunk_size, sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams){				
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount = SOURCEBLOCKRAM_SIZE;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printkeyvalues("priorreduceandbuffer.localcapsule", (keyvalue_t *)localcapsule, 8);
	#endif 
	
	PRIORREDUCEANDBUFFER_LOOP1: for(buffer_type i=0; i<chunk_size; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE II=16 // NEWCHANGE.
		PRIORREDUCEANDBUFFER_LOOP1B: for(unsigned int v=0; v<VECTOR_SIZE; v++){
		// #pragma HLS PIPELINE II=1 // NEWCHANGE.
			keyvalue_buffer_t kv = buffer[v][i];
			keyvalue_t kv2 = UTILP0_GETKV(kv);
		
			partition_type p = ((kv2.key - sweepparams.upperlimit) >> (REDUCESZ_POW));
			
			unsigned int upperlimit = sweepparams.upperlimit + p*REDUCESZ;
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			cout<<"priorreduceandbuffer: kv2.key: "<<kv2.key<<", kv2.value: "<<kv2.value<<", p: "<<p<<", upperlimit: "<<upperlimit<<", sweepparams.upperlimit: "<<sweepparams.upperlimit<<", currentLOP: "<<sweepparams.currentLOP<<endl;
			#endif 
			
			if(p < VDATA_PACKINGSIZE){ REDUCEP0_reducevector(p, kv, vbuffer[p], 0, upperlimit, sweepparams, globalposition, globalparams); } // REMOVEME.
		}
	}
	return;
}

void acts_all::REDUCEP0_tradreduceandbuffer(bool_type enable, uint512_dt * kvdram, keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], buffer_type chunk_size, keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams){				
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount = SOURCEBLOCKRAM_SIZE;
	
	TRADREDUCEANDBUFFER_LOOP1: for(buffer_type i=0; i<chunk_size; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
		TRADREDUCEANDBUFFER_LOOP1B: for(unsigned int v=0; v<VECTOR_SIZE; v++){
		#pragma HLS PIPELINE II=1 // NEWCHANGE.
			keyvalue_buffer_t kv = buffer[v][i];
			keyvalue_t kv2 = UTILP0_GETKV(kv);
			
			if(kv2.key != UTILP0_GETV(INVALIDDATA) && kv2.value != UTILP0_GETV(INVALIDDATA)){
			
				unsigned int block = kv2.key / (REDUCESZ * VECTOR_SIZE); // REDUCESZ, globalparams.SIZE_REDUCE 
				unsigned int globalpos_offset = block * (REDUCESZ * VECTOR_SIZE);
				unsigned int globalrow_offset = globalpos_offset / VECTOR2_SIZE;
				
				unsigned int localpos = kv2.key % (REDUCESZ * VECTOR_SIZE); // 8192;
				unsigned int localrow = (localpos % REDUCESZ) / 2;
				unsigned int localcol = localpos / REDUCESZ;
				
				unsigned int row = globalrow_offset + localrow;
				unsigned int col = localcol;
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				cout<<"REDUCE SEEN @ tradreduceandbuffer:: kv2.key: "<<kv2.key<<", kv2.value: "<<kv2.value<<" || block: "<<block<<", row: "<<row<<", col: "<<col<<" || localpos: "<<localpos<<", localrow: "<<localrow<<", localcol: "<<localcol<<endl;
				#endif
				
				value_t temp;
				uint512_vec_dt vdata;
				#pragma HLS data_pack variable=vdata
				
				#ifdef _WIDEWORD
				vdata.data[0].key = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].range(31, 0);
				vdata.data[0].value = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].range(63, 32);
				vdata.data[1].key = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].range(95, 64);
				vdata.data[1].value = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].range(127, 96);
				vdata.data[2].key = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].range(159, 128);
				vdata.data[2].value = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].range(191, 160);
				vdata.data[3].key = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].range(223, 192);
				vdata.data[3].value = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].range(255, 224);
				vdata.data[4].key = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].range(287, 256);
				vdata.data[4].value = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].range(319, 288);
				vdata.data[5].key = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].range(351, 320);
				vdata.data[5].value = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].range(383, 352);
				vdata.data[6].key = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].range(415, 384);
				vdata.data[6].value = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].range(447, 416);
				vdata.data[7].key = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].range(479, 448);
				vdata.data[7].value = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].range(511, 480);
				#else 
				vdata.data[0].key = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].data[0].key; 
				vdata.data[0].value = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].data[0].value; 
				vdata.data[1].key = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].data[1].key; 
				vdata.data[1].value = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].data[1].value; 
				vdata.data[2].key = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].data[2].key; 
				vdata.data[2].value = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].data[2].value; 
				vdata.data[3].key = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].data[3].key; 
				vdata.data[3].value = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].data[3].value; 
				vdata.data[4].key = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].data[4].key; 
				vdata.data[4].value = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].data[4].value; 
				vdata.data[5].key = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].data[5].key; 
				vdata.data[5].value = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].data[5].value; 
				vdata.data[6].key = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].data[6].key; 
				vdata.data[6].value = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].data[6].value; 
				vdata.data[7].key = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].data[7].key; 
				vdata.data[7].value = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].data[7].value; 
				#endif
				
				if(localpos % 2 == 0){ temp = vdata.data[localcol].key; }
				else { temp = vdata.data[localcol].value; }
				
				value_t new_vprop = REDUCEP0_reducefunc(temp, kv2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				if(localpos % 2 == 0){ vdata.data[localcol].key = new_vprop; }
				else { vdata.data[localcol].value = new_vprop; }
				
				#ifdef _WIDEWORD
				kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].range(31, 0) = vdata.data[0].key;
				kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].range(63, 32) = vdata.data[0].value;
				kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].range(95, 64) = vdata.data[1].key;
				kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].range(127, 96) = vdata.data[1].value;
				kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].range(159, 128) = vdata.data[2].key;
				kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].range(191, 160) = vdata.data[2].value;
				kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].range(223, 192) = vdata.data[3].key;
				kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].range(255, 224) = vdata.data[3].value;
				kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].range(287, 256) = vdata.data[4].key;
				kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].range(319, 288) = vdata.data[4].value;
				kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].range(351, 320) = vdata.data[5].key;
				kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].range(383, 352) = vdata.data[5].value;
				kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].range(415, 384) = vdata.data[6].key;
				kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].range(447, 416) = vdata.data[6].value;
				kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].range(479, 448) = vdata.data[7].key;
				kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].range(511, 480) = vdata.data[7].value;
				#else 
				kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].data[0].key = vdata.data[0].key; 
				kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].data[0].value = vdata.data[0].value; 
				kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].data[1].key = vdata.data[1].key; 
				kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].data[1].value = vdata.data[1].value; 
				kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].data[2].key = vdata.data[2].key; 
				kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].data[2].value = vdata.data[2].value; 
				kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].data[3].key = vdata.data[3].key; 
				kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].data[3].value = vdata.data[3].value; 
				kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].data[4].key = vdata.data[4].key; 
				kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].data[4].value = vdata.data[4].value; 
				kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].data[5].key = vdata.data[5].key; 
				kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].data[5].value = vdata.data[5].value; 
				kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].data[6].key = vdata.data[6].key; 
				kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].data[6].value = vdata.data[6].value; 
				kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].data[7].key = vdata.data[7].key; 
				kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].data[7].value = vdata.data[7].value; 
				#endif
				
				partition_type p = UTILP0_getpartition(ON, ACTSREDUCEMODE, kv, sweepparams.currentLOP, sweepparams.upperlimit, sweepparams.upperpartition, globalparams.POW_BATCHRANGE);
				globalstatsbuffer[p].value += 1;
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(localpos % 2 == 0){ cout<<"--- tradreduceandbuffer :: kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + "<<row<<"].data["<<localcol<<"].key: "<<kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].data[localcol].key<<endl; }
				else { cout<<"--- tradreduceandbuffer :: kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + "<<row<<"].data["<<localcol<<"].value: "<<kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + row].data[localcol].value<<endl; }
				#endif
			}
		}
	}
	return;
}


