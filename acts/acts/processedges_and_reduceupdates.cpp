value_t PANDRP0_processfunc(value_t udata, value_t edgew, unsigned int GraphAlgo){
	value_t res = 0;
	#ifdef CUSTOMLOGICFOREACHALGORITHM
		#if defined(PR_ALGORITHM)
			res = udata + edgew;
		#elif defined(CF_ALGORITHM)
			// source: https://mrmgroup.cs.princeton.edu/papers/taejun_micro16.pdf (Graphicionado)
			// process edge & process combined here (source: graphicionado paper)
			/* --- Collaborative Filtering ---:
			Process Edges: Executed in Reduce. ({Ew, Uprop} from source is sent to destination vertex)
			Reduce: Function of (uprop, Ew, Vprop) is executed 
			Apply: Function of (Vprop, Vtemp) is executed 
			Finish: */
			res = udata;
		#elif defined(CC_ALGORITHM)
			// source: https://www.baeldung.com/cs/graph-connected-components
			// source: https://cs.usm.maine.edu/~briggs/webPage/c161/projects/graphColoring.htmls
			// source: https://www.usenix.org/system/files/conference/osdi12/osdi12-final-126.pdf (GraphChi)
			/* --- Connected Components ---:
			Process Edges: Each vertex writes its id ("label") to its edges. 
			Reduce: Vertex chooses the minimum label of its neighbors; 
			Apply: A neighbor is scheduled only if a label in a connecting edge changes, which we implement by using selective scheduling. 
			Finish: sets of vertices with equal labels are interpreted as connected components or communities, respectively. */
			res = udata;
		#elif defined(BFS_ALGORITHM)
			res = NAp;
		#elif defined(SSSP_ALGORITHM)
			res = udata + edgew;
		#else
			NOT DEFINED.
		#endif
	#else 
	if(GraphAlgo == PAGERANK){
		res = udata + edgew;
	} else if(GraphAlgo == CF){
		res = udata;
	} else if(GraphAlgo == CC){
		res = udata;
	} else if(GraphAlgo == BFS){
		res = NAp;
	} else if(GraphAlgo == SSSP){
		res = udata + edgew;
	} else {
		res = NAp;
	}
	#endif
	return res;
}

value_t PANDRP0_reducefunc(value_t vtemp, value_t res, unsigned int GraphIter, unsigned int GraphAlgo){
	value_t temp = 0;
	#ifdef CUSTOMLOGICFOREACHALGORITHM
		#if defined(PR_ALGORITHM)
			temp = vtemp + res;
		#elif defined(CF_ALGORITHM)
			unsigned int ew = 1;
			unsigned int lamda = 1;
			temp = vtemp + ((ew - vtemp*res)*res - lamda*vtemp);
		#elif defined(CC_ALGORITHM)
			temp = UTILP0_amin(vtemp, res);
		#elif defined(BFS_ALGORITHM)
			temp = UTILP0_amin(vtemp, GraphIter);
		#elif defined(SSSP_ALGORITHM)
			temp = UTILP0_amin(vtemp, res);
		#else 
			NOT DEFINED.
		#endif
	#else 
	if(GraphAlgo == PAGERANK){
		temp = vtemp + res;
	} else if(GraphAlgo == CF){
		temp = vtemp + ((ew - vtemp*res)*res - lamda*vtemp);
	} else if(GraphAlgo == CC){
		temp = UTILP0_amin(vtemp, res);
	} else if(GraphAlgo == BFS){
		temp = UTILP0_amin(vtemp, GraphIter);
	} else if(GraphAlgo == SSSP){
		temp = UTILP0_amin(vtemp, res);
	} else {
		NOT DEFINED
	}
	#endif
	return temp;
}

void PANDRP0_GetXYLayoutV(unsigned int s, unsigned int depths[PE_SETSZ], unsigned int basedepth){
	unsigned int s_ = s % VECTOR2_SIZE;
	
 if(s_==0){ 
		depths[0] = 0; 
		depths[1] = 1; 
		depths[2] = 2; 
		depths[3] = 3; 
		depths[4] = 4; 
		depths[5] = 5; 
		depths[6] = 6; 
		depths[7] = 7; 
		depths[8] = 8; 
		depths[9] = 9; 
		depths[10] = 10; 
		depths[11] = 11; 
		depths[12] = 12; 
		depths[13] = 13; 
		depths[14] = 14; 
		depths[15] = 15; 
	}
else if(s_==1){ 
		depths[0] = 15; 
		depths[1] = 0; 
		depths[2] = 1; 
		depths[3] = 2; 
		depths[4] = 3; 
		depths[5] = 4; 
		depths[6] = 5; 
		depths[7] = 6; 
		depths[8] = 7; 
		depths[9] = 8; 
		depths[10] = 9; 
		depths[11] = 10; 
		depths[12] = 11; 
		depths[13] = 12; 
		depths[14] = 13; 
		depths[15] = 14; 
	}
else if(s_==2){ 
		depths[0] = 14; 
		depths[1] = 15; 
		depths[2] = 0; 
		depths[3] = 1; 
		depths[4] = 2; 
		depths[5] = 3; 
		depths[6] = 4; 
		depths[7] = 5; 
		depths[8] = 6; 
		depths[9] = 7; 
		depths[10] = 8; 
		depths[11] = 9; 
		depths[12] = 10; 
		depths[13] = 11; 
		depths[14] = 12; 
		depths[15] = 13; 
	}
else if(s_==3){ 
		depths[0] = 13; 
		depths[1] = 14; 
		depths[2] = 15; 
		depths[3] = 0; 
		depths[4] = 1; 
		depths[5] = 2; 
		depths[6] = 3; 
		depths[7] = 4; 
		depths[8] = 5; 
		depths[9] = 6; 
		depths[10] = 7; 
		depths[11] = 8; 
		depths[12] = 9; 
		depths[13] = 10; 
		depths[14] = 11; 
		depths[15] = 12; 
	}
else if(s_==4){ 
		depths[0] = 12; 
		depths[1] = 13; 
		depths[2] = 14; 
		depths[3] = 15; 
		depths[4] = 0; 
		depths[5] = 1; 
		depths[6] = 2; 
		depths[7] = 3; 
		depths[8] = 4; 
		depths[9] = 5; 
		depths[10] = 6; 
		depths[11] = 7; 
		depths[12] = 8; 
		depths[13] = 9; 
		depths[14] = 10; 
		depths[15] = 11; 
	}
else if(s_==5){ 
		depths[0] = 11; 
		depths[1] = 12; 
		depths[2] = 13; 
		depths[3] = 14; 
		depths[4] = 15; 
		depths[5] = 0; 
		depths[6] = 1; 
		depths[7] = 2; 
		depths[8] = 3; 
		depths[9] = 4; 
		depths[10] = 5; 
		depths[11] = 6; 
		depths[12] = 7; 
		depths[13] = 8; 
		depths[14] = 9; 
		depths[15] = 10; 
	}
else if(s_==6){ 
		depths[0] = 10; 
		depths[1] = 11; 
		depths[2] = 12; 
		depths[3] = 13; 
		depths[4] = 14; 
		depths[5] = 15; 
		depths[6] = 0; 
		depths[7] = 1; 
		depths[8] = 2; 
		depths[9] = 3; 
		depths[10] = 4; 
		depths[11] = 5; 
		depths[12] = 6; 
		depths[13] = 7; 
		depths[14] = 8; 
		depths[15] = 9; 
	}
else if(s_==7){ 
		depths[0] = 9; 
		depths[1] = 10; 
		depths[2] = 11; 
		depths[3] = 12; 
		depths[4] = 13; 
		depths[5] = 14; 
		depths[6] = 15; 
		depths[7] = 0; 
		depths[8] = 1; 
		depths[9] = 2; 
		depths[10] = 3; 
		depths[11] = 4; 
		depths[12] = 5; 
		depths[13] = 6; 
		depths[14] = 7; 
		depths[15] = 8; 
	}
else if(s_==8){ 
		depths[0] = 8; 
		depths[1] = 9; 
		depths[2] = 10; 
		depths[3] = 11; 
		depths[4] = 12; 
		depths[5] = 13; 
		depths[6] = 14; 
		depths[7] = 15; 
		depths[8] = 0; 
		depths[9] = 1; 
		depths[10] = 2; 
		depths[11] = 3; 
		depths[12] = 4; 
		depths[13] = 5; 
		depths[14] = 6; 
		depths[15] = 7; 
	}
else if(s_==9){ 
		depths[0] = 7; 
		depths[1] = 8; 
		depths[2] = 9; 
		depths[3] = 10; 
		depths[4] = 11; 
		depths[5] = 12; 
		depths[6] = 13; 
		depths[7] = 14; 
		depths[8] = 15; 
		depths[9] = 0; 
		depths[10] = 1; 
		depths[11] = 2; 
		depths[12] = 3; 
		depths[13] = 4; 
		depths[14] = 5; 
		depths[15] = 6; 
	}
else if(s_==10){ 
		depths[0] = 6; 
		depths[1] = 7; 
		depths[2] = 8; 
		depths[3] = 9; 
		depths[4] = 10; 
		depths[5] = 11; 
		depths[6] = 12; 
		depths[7] = 13; 
		depths[8] = 14; 
		depths[9] = 15; 
		depths[10] = 0; 
		depths[11] = 1; 
		depths[12] = 2; 
		depths[13] = 3; 
		depths[14] = 4; 
		depths[15] = 5; 
	}
else if(s_==11){ 
		depths[0] = 5; 
		depths[1] = 6; 
		depths[2] = 7; 
		depths[3] = 8; 
		depths[4] = 9; 
		depths[5] = 10; 
		depths[6] = 11; 
		depths[7] = 12; 
		depths[8] = 13; 
		depths[9] = 14; 
		depths[10] = 15; 
		depths[11] = 0; 
		depths[12] = 1; 
		depths[13] = 2; 
		depths[14] = 3; 
		depths[15] = 4; 
	}
else if(s_==12){ 
		depths[0] = 4; 
		depths[1] = 5; 
		depths[2] = 6; 
		depths[3] = 7; 
		depths[4] = 8; 
		depths[5] = 9; 
		depths[6] = 10; 
		depths[7] = 11; 
		depths[8] = 12; 
		depths[9] = 13; 
		depths[10] = 14; 
		depths[11] = 15; 
		depths[12] = 0; 
		depths[13] = 1; 
		depths[14] = 2; 
		depths[15] = 3; 
	}
else if(s_==13){ 
		depths[0] = 3; 
		depths[1] = 4; 
		depths[2] = 5; 
		depths[3] = 6; 
		depths[4] = 7; 
		depths[5] = 8; 
		depths[6] = 9; 
		depths[7] = 10; 
		depths[8] = 11; 
		depths[9] = 12; 
		depths[10] = 13; 
		depths[11] = 14; 
		depths[12] = 15; 
		depths[13] = 0; 
		depths[14] = 1; 
		depths[15] = 2; 
	}
else if(s_==14){ 
		depths[0] = 2; 
		depths[1] = 3; 
		depths[2] = 4; 
		depths[3] = 5; 
		depths[4] = 6; 
		depths[5] = 7; 
		depths[6] = 8; 
		depths[7] = 9; 
		depths[8] = 10; 
		depths[9] = 11; 
		depths[10] = 12; 
		depths[11] = 13; 
		depths[12] = 14; 
		depths[13] = 15; 
		depths[14] = 0; 
		depths[15] = 1; 
	}
else { 
		depths[0] = 1; 
		depths[1] = 2; 
		depths[2] = 3; 
		depths[3] = 4; 
		depths[4] = 5; 
		depths[5] = 6; 
		depths[6] = 7; 
		depths[7] = 8; 
		depths[8] = 9; 
		depths[9] = 10; 
		depths[10] = 11; 
		depths[11] = 12; 
		depths[12] = 13; 
		depths[13] = 14; 
		depths[14] = 15; 
		depths[15] = 0; 
	}
	return;
}

parsededge_t PANDRP0_PARSEEDGE(uint32_type data){ 
	parsededge_t parsededge;
	#ifdef _WIDEWORD
	parsededge.incr = data.range(31, 28);
	parsededge.dstvid = data.range(28, 0);
	#else
	parsededge.incr = UTILP0_READFROM_UINT(data, 28, 4);
	parsededge.dstvid = UTILP0_READFROM_UINT(data, 0, 28);
	#endif
	return parsededge; 
}

void PANDRP0_processorreducevector(bool enx, unsigned int mode, unsigned int col, unsigned int _loc, keyvalue_t kvdata,
		keyvalue_vbuffer_t vbuffer[BLOCKRAM_VDATA_SIZE], keyvalue_buffer_t buffer[SOURCEBLOCKRAM_SIZE], unsigned int * loadcount, 
			unsigned int GraphAlgoClass, unsigned int upperlimit, sweepparams_t sweepparams, globalparams_t globalparams){
	#pragma HLS INLINE
	bool en = true; 
	keyvalue_t mykeyvalue;
	unsigned int loc = 0;
	
	if(mode == ACTSPROCESSMODE){ 
		loc = _loc;
		if(kvdata.key != INVALIDDATA && kvdata.value != INVALIDDATA && enx == true){ en = true; } else { en = false; }
	} else if(mode == ACTSREDUCEMODE){
		mykeyvalue = UTILP0_GETKV(kvdata);
		loc = ((mykeyvalue.key - upperlimit) - col) >> NUM_PARTITIONS_POW;
		if(mykeyvalue.key != UTILP0_GETK(INVALIDDATA) && mykeyvalue.value != UTILP0_GETV(INVALIDDATA) && enx == true){ en = true; } else { en = false; }
	} else {}
	
	if(loc >= globalparams.SIZEKVS2_REDUCEPARTITION && en == true){
		#ifdef _DEBUGMODE_CHECKS2X
		if(true){ cout<<"REDUCEP0_processvector::ERROR SEEN @ loc("<<loc<<") >= globalparams.SIZE_REDUCE("<<globalparams.SIZE_REDUCE<<"). kvdata.key: "<<kvdata.key<<", upperlimit: "<<upperlimit<<", col: "<<col<<". EXITING... "<<endl; exit(EXIT_FAILURE); }
		#endif 
		loc = 0; }
	
	// read 
	vmdata_t vmdata;
	if(en == true){ vmdata = MEMCAP0_READFROMBUFFER_VDATAWITHVMASK(loc, vbuffer, 0); }
	if(mode == ACTSPROCESSMODE && GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ vmdata.vmask = 1; } 
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	if(mode == ACTSPROCESSMODE && vmdata.vmask == 1){ 
		if(en == true){ cout<<"PANDRP0_processorreducevector:: PROCESS SEEN @ vid: "<<UTILP0_GETREALVID(kvdata.key, globalparams.ACTSPARAMS_INSTID)<<", loc: "<<loc<<", edata.key(dstvid): "<<edata.key<<", edata.value(srcvid): "<<edata.value<<", upperlimit: "<<upperlimit<<endl; }
	} else if(mode == ACTSREDUCEMODE){
		if(en == true){ cout<<"PANDRP0_processorreducevector:: REDUCE SEEN @ vid: "<<UTILP0_GETREALVID(mykeyvalue.key, globalparams.ACTSPARAMS_INSTID)<<", loc: "<<loc<<", mykeyvalue.key: "<<mykeyvalue.key<<", mykeyvalue.value: "<<mykeyvalue.value<<", upperlimit: "<<upperlimit<<endl; }
	} else {}
	#endif 
			
	// process
	if(mode == ACTSPROCESSMODE){ 
		value_t res = PANDRP0_processfunc(vmdata.vdata, 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		keyvalue_t mykeyvalue; mykeyvalue.key = kvdata.key; mykeyvalue.value = res;
	} else if(mode == ACTSREDUCEMODE){
		value_t new_vprop = PANDRP0_reducefunc(vmdata.vdata, mykeyvalue.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		if(en == true && new_vprop != vmdata.vdata){ vmdata.vmask = 1; }
	} else {}
	
	// write 
	if(mode == ACTSPROCESSMODE){
		if(en == true && vmdata.vmask == 1){ buffer[*loadcount] = UTILP0_GETKV(mykeyvalue); }
		if(en == true && vmdata.vmask == 1){ *loadcount += 1; }
	} else if(mode == ACTSREDUCEMODE){
		if(en == true){ MEMCAP0_WRITETOBUFFER_VDATAWITHVMASK(loc, vbuffer, vmdata.vdata, vmdata.vmask, 0); }
	} else {}	
	
	#ifdef _DEBUGMODE_STATS
	if(mode == ACTSPROCESSMODE){ 
		actsutilityobj->globalstats_countkvsprocessed(1);
		if(en == true && vmdata.vmask == 1){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); }
	} else if(mode == ACTSREDUCEMODE){
		actsutilityobj->globalstats_countkvsreduced(1); 
		if(en == true){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(1); }
	} else {}
	#endif 
	return;
}

fetchmessage_t PANDRP0_processandreduce(bool_type enable, unsigned int mode, 
		uint512_dt * edges, uint512_dt * kvdram, 
			keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE],
				keyvalue_t globalcapsule[MAX_NUM_PARTITIONS], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS],
					batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, 
						travstate_t travstate, sweepparams_t sweepparams, globalparams_t globalparams){
	fetchmessage_t fetchmessage;
	fetchmessage.chunksize_kvs = -1;
	fetchmessage.nextoffset_kvs = -1;
	if(enable == OFF){ return fetchmessage; }
	analysis_type analysis_loopcount = (DESTBLOCKRAM_SIZE / (NUM_PARTITIONS / 2));
	
	buffer_type reducebuffersz = globalparams.SIZE_REDUCE / 2;
	unsigned int validbound = reducebuffersz * FETFACTOR * VECTOR2_SIZE;
	
	travstate.i_kvs = travstate.i_kvs / 2;
	travstate.end_kvs = travstate.end_kvs / 2;
	loffset_kvs = loffset_kvs / 2;
	buffer_type edgessize_kvs = size_kvs / 2;
	
	batch_type offset_kvs = goffset_kvs + loffset_kvs;
	
	unsigned int lsrcvids[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=lsrcvids complete
	unsigned int ldstvids[VECTOR2_SIZE];	
	#pragma HLS ARRAY_PARTITION variable=ldstvids complete
	value_t res[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=res complete
	unsigned int loadcount[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=loadcount complete
	unsigned int depths[PE_SETSZ];
	#pragma HLS ARRAY_PARTITION variable=depths complete
	unsigned int d[PE_SETSZ];
	#pragma HLS ARRAY_PARTITION variable=d complete
	bool enx[PE_SETSZ];
	#pragma HLS ARRAY_PARTITION variable=enx complete
	value_t EDATA[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=EDATA complete
	keyvalue_buffer_t KVDATAA[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=KVDATAA complete
	keyvalue_t kvdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=kvdata complete
	buffer_type size_kvs[MAX_NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=size_kvs complete
	value_t tempbuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE];
	#pragma HLS array_partition variable = tempbuffer
	
	buffer_type maxsize_kvs = 0;
	buffer_type totalsize_kvs = 0;
	unsigned int key_kvs[NUM_PARTITIONS];
	
	batch_type vptrbaseoffset_kvs = globalparams.BASEOFFSETKVS_VERTEXPTR + (globalparams.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE);
	batch_type voffset_kvs = sweepparams.source_partition * reducebuffersz * FETFACTOR;

	fetchmessage.chunksize_kvs = edgessize_kvs;
	fetchmessage.nextoffset_kvs = -1;
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;

	buffer_type chunk_size = 0;
	if(mode == ACTSPROCESSMODE){
		chunk_size = UTILP0_getchunksize_kvs(edgessize_kvs, travstate, 0);
	} else if(mode == ACTSREDUCEMODE){
		chunk_size = DESTBLOCKRAM_SIZE;
	} else {}
	
	if(mode == ACTSPROCESSMODE){
		// FIXME. Get this is a metadata in DRAM
		unsigned int modelsz = chunk_size / NUM_PARTITIONS;
		for(unsigned int i=0; i<NUM_PARTITIONS; i++){ localcapsule[i].key = i * modelsz; localcapsule[i].value = modelsz; } // FIXME.
		for(unsigned int t=0; t<VECTOR2_SIZE; t++){ loadcount[t] = 0; }
		
		#ifdef _DEBUGMODE_KERNELPRINTS
		actsutilityobj->printkeyvalues("processandbuffer.localcapsule", (keyvalue_t *)localcapsule, NUM_PARTITIONS);
		#endif
		
		PANDR_BUFFERPARTITIONS_LOOP1: for(buffer_type i=0; i<chunk_size; i++){
		#pragma HLS PIPELINE II=1
			#ifdef _WIDEWORD
			tempbuffer[0][i] = edges[offset_kvs + i].range(31, 0); 
			tempbuffer[1][i] = edges[offset_kvs + i].range(63, 32); 
			tempbuffer[2][i] = edges[offset_kvs + i].range(95, 64); 
			tempbuffer[3][i] = edges[offset_kvs + i].range(127, 96); 
			tempbuffer[4][i] = edges[offset_kvs + i].range(159, 128); 
			tempbuffer[5][i] = edges[offset_kvs + i].range(191, 160); 
			tempbuffer[6][i] = edges[offset_kvs + i].range(223, 192); 
			tempbuffer[7][i] = edges[offset_kvs + i].range(255, 224); 
			tempbuffer[8][i] = edges[offset_kvs + i].range(287, 256); 
			tempbuffer[9][i] = edges[offset_kvs + i].range(319, 288); 
			tempbuffer[10][i] = edges[offset_kvs + i].range(351, 320); 
			tempbuffer[11][i] = edges[offset_kvs + i].range(383, 352); 
			tempbuffer[12][i] = edges[offset_kvs + i].range(415, 384); 
			tempbuffer[13][i] = edges[offset_kvs + i].range(447, 416); 
			tempbuffer[14][i] = edges[offset_kvs + i].range(479, 448); 
			tempbuffer[15][i] = edges[offset_kvs + i].range(511, 480); 
			#else 
			tempbuffer[0][i] = edges[offset_kvs + i].data[0].key; 
			tempbuffer[1][i] = edges[offset_kvs + i].data[0].value;	
			tempbuffer[2][i] = edges[offset_kvs + i].data[1].key; 
			tempbuffer[3][i] = edges[offset_kvs + i].data[1].value;	
			tempbuffer[4][i] = edges[offset_kvs + i].data[2].key; 
			tempbuffer[5][i] = edges[offset_kvs + i].data[2].value;	
			tempbuffer[6][i] = edges[offset_kvs + i].data[3].key; 
			tempbuffer[7][i] = edges[offset_kvs + i].data[3].value;	
			tempbuffer[8][i] = edges[offset_kvs + i].data[4].key; 
			tempbuffer[9][i] = edges[offset_kvs + i].data[4].value;	
			tempbuffer[10][i] = edges[offset_kvs + i].data[5].key; 
			tempbuffer[11][i] = edges[offset_kvs + i].data[5].value;	
			tempbuffer[12][i] = edges[offset_kvs + i].data[6].key; 
			tempbuffer[13][i] = edges[offset_kvs + i].data[6].value;	
			tempbuffer[14][i] = edges[offset_kvs + i].data[7].key; 
			tempbuffer[15][i] = edges[offset_kvs + i].data[7].value;	
			#endif
		}
	}
	buffer_type height_kvs = (localcapsule[NUM_PARTITIONS-1].key + localcapsule[NUM_PARTITIONS-1].value) / VECTOR_SIZE;

	buffer_type blockoffset=0;
	PANDR_BUFFERPARTITIONS_LOOP3: for(buffer_type blockoffset=0; blockoffset<NUM_PARTITIONS; blockoffset+=8){
		
		PANDR_BUFFERPARTITIONS_LOOP3B: for(partition_type p=blockoffset; p<blockoffset + 8; p++){
		#pragma HLS PIPELINE II=1
			size_kvs[p] = localcapsule[blockoffset + p].value / VECTOR_SIZE;
			if(maxsize_kvs < size_kvs[p]){ maxsize_kvs = size_kvs[p]; }
			totalsize_kvs += size_kvs[p];
			key_kvs[p] = localcapsule[blockoffset + p].key / VECTOR_SIZE;
		}
		
		PANDR_BUFFERPARTITIONS_LOOP3C: for(unsigned int r=0; r<NUM_PARTITIONS; r++){
			PANDR_BUFFERPARTITIONS_LOOP3D: for(buffer_type i=0; i<maxsize_kvs; i++){ // maxsize // FIXME
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
			#pragma HLS PIPELINE II=4
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("readandprocess2(12)::DEBUG CODE 1::1", i, SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("readandprocess2(12)::DEBUG CODE 14::1", blockoffset + depths[0], NUM_PARTITIONS, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("readandprocess2(12)::DEBUG CODE 1::1", i, SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("readandprocess2(12)::DEBUG CODE 14::1", blockoffset + depths[1], NUM_PARTITIONS, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("readandprocess2(12)::DEBUG CODE 1::1", i, SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("readandprocess2(12)::DEBUG CODE 14::1", blockoffset + depths[2], NUM_PARTITIONS, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("readandprocess2(12)::DEBUG CODE 1::1", i, SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("readandprocess2(12)::DEBUG CODE 14::1", blockoffset + depths[3], NUM_PARTITIONS, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("readandprocess2(12)::DEBUG CODE 1::1", i, SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("readandprocess2(12)::DEBUG CODE 14::1", blockoffset + depths[4], NUM_PARTITIONS, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("readandprocess2(12)::DEBUG CODE 1::1", i, SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("readandprocess2(12)::DEBUG CODE 14::1", blockoffset + depths[5], NUM_PARTITIONS, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("readandprocess2(12)::DEBUG CODE 1::1", i, SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("readandprocess2(12)::DEBUG CODE 14::1", blockoffset + depths[6], NUM_PARTITIONS, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("readandprocess2(12)::DEBUG CODE 1::1", i, SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("readandprocess2(12)::DEBUG CODE 14::1", blockoffset + depths[7], NUM_PARTITIONS, NAp, NAp, NAp);
 
				#endif	
				
				// read edges . FIXME
				PANDRP0_GetXYLayoutV(r, depths, 0);
				d[0] = (localcapsule[blockoffset + depths[0]].key / VECTOR_SIZE) + i; 	
				d[1] = (localcapsule[blockoffset + depths[1]].key / VECTOR_SIZE) + i; 	
				d[2] = (localcapsule[blockoffset + depths[2]].key / VECTOR_SIZE) + i; 	
				d[3] = (localcapsule[blockoffset + depths[3]].key / VECTOR_SIZE) + i; 	
				d[4] = (localcapsule[blockoffset + depths[4]].key / VECTOR_SIZE) + i; 	
				d[5] = (localcapsule[blockoffset + depths[5]].key / VECTOR_SIZE) + i; 	
				d[6] = (localcapsule[blockoffset + depths[6]].key / VECTOR_SIZE) + i; 	
				d[7] = (localcapsule[blockoffset + depths[7]].key / VECTOR_SIZE) + i; 	
 
				if(mode == ACTSPROCESSMODE){
					if(d[0] < key_kvs[1]){ EDATA[0] = tempbuffer[0][d[0]]; enx[0] = true; } else { enx[0] = false; }
					if(d[1] < key_kvs[2]){ EDATA[1] = tempbuffer[1][d[1]]; enx[1] = true; } else { enx[1] = false; }
					if(d[2] < key_kvs[3]){ EDATA[2] = tempbuffer[2][d[2]]; enx[2] = true; } else { enx[2] = false; }
					if(d[3] < key_kvs[4]){ EDATA[3] = tempbuffer[3][d[3]]; enx[3] = true; } else { enx[3] = false; }
					if(d[4] < key_kvs[5]){ EDATA[4] = tempbuffer[4][d[4]]; enx[4] = true; } else { enx[4] = false; }
					if(d[5] < key_kvs[6]){ EDATA[5] = tempbuffer[5][d[5]]; enx[5] = true; } else { enx[5] = false; }
					if(d[6] < key_kvs[7]){ EDATA[6] = tempbuffer[6][d[6]]; enx[6] = true; } else { enx[6] = false; }
					if(d[7] < height_kvs){ EDATA[7] = tempbuffer[7][d[7]]; enx[7] = true; } else { enx[7] = false; }
 
				} else if(mode == ACTSREDUCEMODE){
					if(d[0] < key_kvs[1]){ KVDATAA[0] = buffer[0][d[0]]; enx[0] = true; } else { enx[0] = false; }
					if(d[1] < key_kvs[2]){ KVDATAA[1] = buffer[1][d[1]]; enx[1] = true; } else { enx[1] = false; }
					if(d[2] < key_kvs[3]){ KVDATAA[2] = buffer[2][d[2]]; enx[2] = true; } else { enx[2] = false; }
					if(d[3] < key_kvs[4]){ KVDATAA[3] = buffer[3][d[3]]; enx[3] = true; } else { enx[3] = false; }
					if(d[4] < key_kvs[5]){ KVDATAA[4] = buffer[4][d[4]]; enx[4] = true; } else { enx[4] = false; }
					if(d[5] < key_kvs[6]){ KVDATAA[5] = buffer[5][d[5]]; enx[5] = true; } else { enx[5] = false; }
					if(d[6] < key_kvs[7]){ KVDATAA[6] = buffer[6][d[6]]; enx[6] = true; } else { enx[6] = false; }
					if(d[7] < key_kvs[8]){ KVDATAA[7] = buffer[7][d[7]]; enx[7] = true; } else { enx[7] = false; }
 					
				} else {}
				
				// parse {srcvid, dstvid}
				if(mode == ACTSPROCESSMODE){
					parsededge_t parsed_edge0 = PANDRP0_PARSEEDGE(EDATA[0]);
					kvdata[0].value = parsed_edge0.incr; // source info
					kvdata[0].key = parsed_edge0.dstvid;	
					parsededge_t parsed_edge1 = PANDRP0_PARSEEDGE(EDATA[1]);
					kvdata[1].value = parsed_edge1.incr; // source info
					kvdata[1].key = parsed_edge1.dstvid;	
					parsededge_t parsed_edge2 = PANDRP0_PARSEEDGE(EDATA[2]);
					kvdata[2].value = parsed_edge2.incr; // source info
					kvdata[2].key = parsed_edge2.dstvid;	
					parsededge_t parsed_edge3 = PANDRP0_PARSEEDGE(EDATA[3]);
					kvdata[3].value = parsed_edge3.incr; // source info
					kvdata[3].key = parsed_edge3.dstvid;	
					parsededge_t parsed_edge4 = PANDRP0_PARSEEDGE(EDATA[4]);
					kvdata[4].value = parsed_edge4.incr; // source info
					kvdata[4].key = parsed_edge4.dstvid;	
					parsededge_t parsed_edge5 = PANDRP0_PARSEEDGE(EDATA[5]);
					kvdata[5].value = parsed_edge5.incr; // source info
					kvdata[5].key = parsed_edge5.dstvid;	
					parsededge_t parsed_edge6 = PANDRP0_PARSEEDGE(EDATA[6]);
					kvdata[6].value = parsed_edge6.incr; // source info
					kvdata[6].key = parsed_edge6.dstvid;	
					parsededge_t parsed_edge7 = PANDRP0_PARSEEDGE(EDATA[7]);
					kvdata[7].value = parsed_edge7.incr; // source info
					kvdata[7].key = parsed_edge7.dstvid;	
				} else if(mode == ACTSREDUCEMODE){
					kvdata[0].key = UTILP0_GETKV(KVDATAA[0]).key;
					kvdata[0].value = UTILP0_GETKV(KVDATAA[1]).value;
					kvdata[1].key = UTILP0_GETKV(KVDATAA[2]).key;
					kvdata[1].value = UTILP0_GETKV(KVDATAA[3]).value;
					kvdata[2].key = UTILP0_GETKV(KVDATAA[4]).key;
					kvdata[2].value = UTILP0_GETKV(KVDATAA[5]).value;
					kvdata[3].key = UTILP0_GETKV(KVDATAA[6]).key;
					kvdata[3].value = UTILP0_GETKV(KVDATAA[7]).value;
					kvdata[4].key = UTILP0_GETKV(KVDATAA[8]).key;
					kvdata[4].value = UTILP0_GETKV(KVDATAA[9]).value;
					kvdata[5].key = UTILP0_GETKV(KVDATAA[10]).key;
					kvdata[5].value = UTILP0_GETKV(KVDATAA[11]).value;
					kvdata[6].key = UTILP0_GETKV(KVDATAA[12]).key;
					kvdata[6].value = UTILP0_GETKV(KVDATAA[13]).value;
					kvdata[7].key = UTILP0_GETKV(KVDATAA[14]).key;
					kvdata[7].value = UTILP0_GETKV(KVDATAA[15]).value;
 
				} else {}
				
				// process
				if(mode == ACTSPROCESSMODE){ enx[0] = false; }
				if(EDATA[0] == INVALIDDATA){ enx[0] = false; }
				PANDRP0_processorreducevector(enx[0], mode, 0, kvdata[0].value, kvdata[0],
						vbuffer[blockoffset + 0], buffer[0], &loadcount[0], 
							GraphAlgoClass, sweepparams.upperlimit, sweepparams, globalparams);
				if(EDATA[1] == INVALIDDATA){ enx[1] = false; }
				PANDRP0_processorreducevector(enx[1], mode, 1, kvdata[1].value, kvdata[1],
						vbuffer[blockoffset + 1], buffer[1], &loadcount[1], 
							GraphAlgoClass, sweepparams.upperlimit, sweepparams, globalparams);
				if(EDATA[2] == INVALIDDATA){ enx[2] = false; }
				PANDRP0_processorreducevector(enx[2], mode, 2, kvdata[2].value, kvdata[2],
						vbuffer[blockoffset + 2], buffer[2], &loadcount[2], 
							GraphAlgoClass, sweepparams.upperlimit, sweepparams, globalparams);
				if(EDATA[3] == INVALIDDATA){ enx[3] = false; }
				PANDRP0_processorreducevector(enx[3], mode, 3, kvdata[3].value, kvdata[3],
						vbuffer[blockoffset + 3], buffer[3], &loadcount[3], 
							GraphAlgoClass, sweepparams.upperlimit, sweepparams, globalparams);
				if(EDATA[4] == INVALIDDATA){ enx[4] = false; }
				PANDRP0_processorreducevector(enx[4], mode, 4, kvdata[4].value, kvdata[4],
						vbuffer[blockoffset + 4], buffer[4], &loadcount[4], 
							GraphAlgoClass, sweepparams.upperlimit, sweepparams, globalparams);
				if(EDATA[5] == INVALIDDATA){ enx[5] = false; }
				PANDRP0_processorreducevector(enx[5], mode, 5, kvdata[5].value, kvdata[5],
						vbuffer[blockoffset + 5], buffer[5], &loadcount[5], 
							GraphAlgoClass, sweepparams.upperlimit, sweepparams, globalparams);
				if(EDATA[6] == INVALIDDATA){ enx[6] = false; }
				PANDRP0_processorreducevector(enx[6], mode, 6, kvdata[6].value, kvdata[6],
						vbuffer[blockoffset + 6], buffer[6], &loadcount[6], 
							GraphAlgoClass, sweepparams.upperlimit, sweepparams, globalparams);
				if(EDATA[7] == INVALIDDATA){ enx[7] = false; }
				PANDRP0_processorreducevector(enx[7], mode, 7, kvdata[7].value, kvdata[7],
						vbuffer[blockoffset + 7], buffer[7], &loadcount[7], 
							GraphAlgoClass, sweepparams.upperlimit, sweepparams, globalparams);
			}
		}
	}
	
	// cout<<"classname__processedges2_splitdstvxs SUCCESSFULL HERE...."<<endl;
	// for(unsigned int t=0; t<VECTOR_SIZE; t++){ cout<<"--- loadcount["<<t<<"]: "<<loadcount[t]<<endl; }
	fetchmessage.chunksize_kvs = chunk_size * 2; // loadcount; // CRITICAL FIXME
	// exit(EXIT_SUCCESS); ////
	return fetchmessage;
}

#ifdef BASIC_PARTITION_AND_REDUCE_STRETEGY
void REDUCEP0_priorreduceandbuffer(bool_type enable, keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], buffer_type chunk_size, sweepparams_t sweepparams, globalparams_t globalparams){				
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"reduceupdates2: REDUCEP0_tradreduceandbuffer NOT DEFINED HERE."<<endl;
	exit(EXIT_FAILURE);
	#endif 
	return;
}
#endif 

#ifdef TRAD_PARTITION_AND_REDUCE_STRETEGY
void REDUCEP0_tradreduceandbuffer(bool_type enable, uint512_dt * kvdram, keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], buffer_type chunk_size, keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], sweepparams_t sweepparams, globalparams_t globalparams){				
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"reduceupdates2: REDUCEP0_tradreduceandbuffer NOT DEFINED HERE."<<endl;
	exit(EXIT_FAILURE);
	#endif 
	return;
}
#endif 


